#include "strata/json/json_parse.hpp"

#include "strata/util/fast_parse.hpp"
#include "strata/util/simd_string.hpp"

#include <cctype>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <vector>

namespace strata {

namespace {

thread_local DuplicateKeyPolicy g_duplicate_policy = DuplicateKeyPolicy::FirstWins;
thread_local std::vector<std::string> g_parse_warnings;

class DomBuilderHandler : public JsonSaxHandler {
  public:
    DomBuilderHandler() = default;

    bool on_null() override { return push_value(JsonValue()); }
    bool on_bool(bool v) override { return push_value(JsonValue(JsonValue::Variant(v))); }
    bool on_int(int64_t v) override {
        return push_value(JsonValue(JsonValue::Variant(static_cast<double>(v))));
    }
    bool on_uint(uint64_t v) override {
        return push_value(JsonValue(JsonValue::Variant(static_cast<double>(v))));
    }
    bool on_double(double v) override { return push_value(JsonValue(JsonValue::Variant(v))); }
    bool on_string(std::string_view v) override {
        return push_value(JsonValue(JsonValue::Variant(std::string(v))));
    }

    bool on_start_object(size_t) override {
        stack_.emplace_back(JsonValue::Variant(JsonValue::Object()));
        return true;
    }

    bool on_key(std::string_view v) override {
        keys_.emplace_back(std::string(v));
        return true;
    }

    bool on_end_object() override {
        if (stack_.empty())
            return false;
        auto obj = std::move(stack_.back());
        stack_.pop_back();
        return push_value(std::move(obj));
    }

    bool on_start_array(size_t) override {
        stack_.emplace_back(JsonValue::Variant(JsonValue::Array()));
        return true;
    }

    bool on_end_array() override {
        if (stack_.empty())
            return false;
        auto arr = std::move(stack_.back());
        stack_.pop_back();
        return push_value(std::move(arr));
    }

    JsonValue&& take_root() { return std::move(root_); }

  private:
    bool push_value(JsonValue&& val) {
        if (stack_.empty()) {
            root_ = std::move(val);
            return true;
        }

        auto& top = stack_.back();
        if (std::holds_alternative<JsonValue::Array>(top.data)) {
            std::get<JsonValue::Array>(top.data).push_back(std::move(val));
            return true;
        } else if (std::holds_alternative<JsonValue::Object>(top.data)) {
            if (keys_.empty())
                return false;
            auto& obj = std::get<JsonValue::Object>(top.data);
            std::string key = std::move(keys_.back());
            keys_.pop_back();

            auto existing = obj.find(key);
            if (existing != obj.end()) {
                switch (g_duplicate_policy) {
                case DuplicateKeyPolicy::FirstWins:
                    break;
                case DuplicateKeyPolicy::Warn:
                    g_parse_warnings.push_back("Duplicate key encountered: " + key);
                    break;
                case DuplicateKeyPolicy::LastWins:
                    existing->second = std::move(val);
                    break;
                case DuplicateKeyPolicy::Error:
                    return false;
                }
            } else {
                obj.emplace(std::move(key), std::move(val));
            }
            return true;
        }
        return false;
    }

    JsonValue root_;
    std::vector<JsonValue> stack_;
    std::vector<std::string> keys_;
};

struct Parser {
    const char* data;
    size_t len;
    JsonSaxHandler& handler;
    size_t i = 0;

    bool eof() const { return i >= len; }
    char peek() const { return eof() ? '\0' : data[i]; }
    char get() { return eof() ? '\0' : data[i++]; }

    static int hex_value(char c) {
        if (c >= '0' && c <= '9')
            return c - '0';
        if (c >= 'a' && c <= 'f')
            return 10 + (c - 'a');
        if (c >= 'A' && c <= 'F')
            return 10 + (c - 'A');
        return -1;
    }

    bool read_hex4(uint32_t& out) {
        if (i + 4 > len)
            return false;
        uint32_t value = 0;
        for (int idx = 0; idx < 4; ++idx) {
            int digit = hex_value(get());
            if (digit < 0)
                return false;
            value = (value << 4) | static_cast<uint32_t>(digit);
        }
        out = value;
        return true;
    }

    static bool append_utf8(std::string& out, uint32_t codepoint) {
        if (codepoint > 0x10FFFF)
            return false;
        if (codepoint >= 0xD800 && codepoint <= 0xDFFF)
            return false;
        if (codepoint <= 0x7F) {
            out.push_back(static_cast<char>(codepoint));
            return true;
        }
        if (codepoint <= 0x7FF) {
            out.push_back(static_cast<char>(0xC0 | ((codepoint >> 6) & 0x1F)));
            out.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
            return true;
        }
        if (codepoint <= 0xFFFF) {
            out.push_back(static_cast<char>(0xE0 | ((codepoint >> 12) & 0x0F)));
            out.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
            return true;
        }
        out.push_back(static_cast<char>(0xF0 | ((codepoint >> 18) & 0x07)));
        out.push_back(static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
        return true;
    }

    void skip_ws() { i = util::skip_whitespace_fast(data, len, i); }

    bool consume(char c) {
        skip_ws();
        if (peek() == c) {
            ++i;
            return true;
        }
        return false;
    }

    bool parse_value() {
        skip_ws();
        if (eof())
            return false;
        char c = peek();
        if (c == 'n')
            return parse_null();
        if (c == 't' || c == 'f')
            return parse_bool();
        if (c == '"')
            return parse_string();
        if (c == '[')
            return parse_array();
        if (c == '{')
            return parse_object();
        if (c == '-' || std::isdigit(static_cast<unsigned char>(c)))
            return parse_number();
        return false;
    }

    bool parse_null() {
        if (i + 4 <= len && data[i] == 'n' && data[i + 1] == 'u' && data[i + 2] == 'l' &&
            data[i + 3] == 'l') {
            i += 4;
            return handler.on_null();
        }
        return false;
    }

    bool parse_bool() {
        if (i + 4 <= len && data[i] == 't' && data[i + 1] == 'r' && data[i + 2] == 'u' &&
            data[i + 3] == 'e') {
            i += 4;
            return handler.on_bool(true);
        }
        if (i + 5 <= len && data[i] == 'f' && data[i + 1] == 'a' && data[i + 2] == 'l' &&
            data[i + 3] == 's' && data[i + 4] == 'e') {
            i += 5;
            return handler.on_bool(false);
        }
        return false;
    }

    bool parse_number() {
        size_t start = i;

        // Try fast integer parse first
        int64_t int_val;
        size_t consumed;
        if (util::parse_int_fast(data + i, len - i, int_val, consumed)) {
            // Check if there's more (decimal or exponent)
            if (i + consumed < len && (data[i + consumed] == '.' || data[i + consumed] == 'e' ||
                                       data[i + consumed] == 'E')) {
                // Fall through to double parsing
            } else {
                i += consumed;
                return handler.on_int(int_val);
            }
        }

        // Parse as double
        double double_val;
        if (util::parse_double_fast(data + start, len - start, double_val, consumed)) {
            i = start + consumed;
            return handler.on_double(double_val);
        }

        return false;
    }

    bool parse_string(bool is_key = false) {
        if (get() != '"')
            return false;

        // Fast scan for end quote or escape using SIMD
        size_t scan_pos = util::find_next_escape_simd(data + i, len - i);

        // Fast path: no escapes, just copy
        if (scan_pos < len - i && data[i + scan_pos] == '"') {
            std::string_view result(data + i, scan_pos);
            i += scan_pos + 1; // +1 for closing quote
            return is_key ? handler.on_key(result) : handler.on_string(result);
        }

        // Slow path: has escapes or control chars
        std::string out;
        out.reserve(scan_pos + 16); // Pre-allocate with some buffer

        while (!eof()) {
            char c = get();
            if (c == '"') {
                return is_key ? handler.on_key(out) : handler.on_string(out);
            }
            if (c == '\\') {
                if (eof())
                    return false;
                char esc = get();
                switch (esc) {
                case '"':
                    out.push_back('"');
                    break;
                case '\\':
                    out.push_back('\\');
                    break;
                case '/':
                    out.push_back('/');
                    break;
                case 'b':
                    out.push_back('\b');
                    break;
                case 'f':
                    out.push_back('\f');
                    break;
                case 'n':
                    out.push_back('\n');
                    break;
                case 'r':
                    out.push_back('\r');
                    break;
                case 't':
                    out.push_back('\t');
                    break;
                case 'u': {
                    uint32_t codepoint = 0;
                    if (!read_hex4(codepoint))
                        return false;
                    if (codepoint >= 0xD800 && codepoint <= 0xDBFF) {
                        if (eof() || get() != '\\')
                            return false;
                        if (eof() || get() != 'u')
                            return false;
                        uint32_t low = 0;
                        if (!read_hex4(low))
                            return false;
                        if (low < 0xDC00 || low > 0xDFFF)
                            return false;
                        codepoint = 0x10000 + ((codepoint - 0xD800) << 10) + (low - 0xDC00);
                    } else if (codepoint >= 0xDC00 && codepoint <= 0xDFFF) {
                        return false;
                    }
                    if (!append_utf8(out, codepoint))
                        return false;
                    break;
                }
                default:
                    return false;
                }
            } else {
                if (static_cast<unsigned char>(c) < 0x20) {
                    return false;
                }
                out.push_back(c);
            }
        }
        return false;
    }

    bool parse_array() {
        if (!consume('['))
            return false;
        if (!handler.on_start_array())
            return false;
        if (consume(']'))
            return handler.on_end_array();
        while (true) {
            if (!parse_value())
                return false;
            if (consume(']'))
                break;
            if (!consume(','))
                return false;
        }
        return handler.on_end_array();
    }

    bool parse_object() {
        if (!consume('{'))
            return false;
        if (!handler.on_start_object())
            return false;
        if (consume('}'))
            return handler.on_end_object();
        while (true) {
            skip_ws();
            if (!parse_string(true))
                return false;
            if (!consume(':'))
                return false;
            if (!parse_value())
                return false;
            if (consume('}'))
                break;
            if (!consume(','))
                return false;
        }
        return handler.on_end_object();
    }
};

} // namespace

Result<JsonValue> parse_json(std::string_view text) {
    g_parse_warnings.clear();
    DomBuilderHandler handler;
    Status status = parse_sax(text, handler);
    if (status != Status::Ok) {
        return {status, JsonValue{}};
    }
    return {Status::Ok, handler.take_root()};
}

Status parse_sax(std::string_view text, JsonSaxHandler& handler, bool validate_utf8) {
    if (validate_utf8 && !text.empty() && !util::validate_utf8_simd(text.data(), text.size())) {
        return Status::ParseError;
    }
    Parser p{text.data(), text.size(), handler, 0};
    if (!p.parse_value())
        return Status::ParseError;
    p.skip_ws();
    if (!p.eof())
        return Status::ParseError;
    return Status::Ok;
}

void set_duplicate_key_policy(DuplicateKeyPolicy policy) { g_duplicate_policy = policy; }

DuplicateKeyPolicy get_duplicate_key_policy() { return g_duplicate_policy; }

std::vector<std::string> consume_parse_warnings() {
    std::vector<std::string> warnings = std::move(g_parse_warnings);
    g_parse_warnings.clear();
    return warnings;
}

} // namespace strata
