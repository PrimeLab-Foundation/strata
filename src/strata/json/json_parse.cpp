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

struct Parser {
    const char* data;
    size_t len;
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

    Result<JsonValue> parse_value() {
        skip_ws();
        if (eof())
            return {Status::ParseError, JsonValue{}};
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
        return {Status::ParseError, JsonValue{}};
    }

    Result<JsonValue> parse_null() {
        skip_ws();
        if (i + 4 <= len && data[i] == 'n' && data[i + 1] == 'u' && data[i + 2] == 'l' &&
            data[i + 3] == 'l') {
            i += 4;
            return {Status::Ok, JsonValue{}};
        }
        return {Status::ParseError, JsonValue{}};
    }

    Result<JsonValue> parse_bool() {
        skip_ws();
        if (i + 4 <= len && data[i] == 't' && data[i + 1] == 'r' && data[i + 2] == 'u' &&
            data[i + 3] == 'e') {
            i += 4;
            return {Status::Ok, JsonValue(JsonValue::Variant(true))};
        }
        if (i + 5 <= len && data[i] == 'f' && data[i + 1] == 'a' && data[i + 2] == 'l' &&
            data[i + 3] == 's' && data[i + 4] == 'e') {
            i += 5;
            return {Status::Ok, JsonValue(JsonValue::Variant(false))};
        }
        return {Status::ParseError, JsonValue{}};
    }

    Result<JsonValue> parse_number() {
        skip_ws();
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
                return {Status::Ok, JsonValue(JsonValue::Variant(static_cast<double>(int_val)))};
            }
        }

        // Parse as double
        double double_val;
        if (util::parse_double_fast(data + start, len - start, double_val, consumed)) {
            i = start + consumed;
            return {Status::Ok, JsonValue(JsonValue::Variant(double_val))};
        }

        return {Status::ParseError, JsonValue{}};
    }

    Result<JsonValue> parse_string() {
        skip_ws();
        if (get() != '"')
            return {Status::ParseError, JsonValue{}};

        // Fast scan for end quote or escape
        size_t scan_pos = util::scan_string_fast(data + i, len - i);

        // Fast path: no escapes, just copy
        if (scan_pos < len - i && data[i + scan_pos] == '"') {
            std::string result(data + i, scan_pos);
            i += scan_pos + 1; // +1 for closing quote
            return {Status::Ok, JsonValue(JsonValue::Variant(std::move(result)))};
        }

        // Slow path: has escapes or control chars
        std::string out;
        out.reserve(scan_pos + 16); // Pre-allocate with some buffer

        while (!eof()) {
            char c = get();
            if (c == '"') {
                return {Status::Ok, JsonValue(JsonValue::Variant(std::move(out)))};
            }
            if (c == '\\') {
                if (eof())
                    return {Status::ParseError, JsonValue{}};
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
                        return {Status::ParseError, JsonValue{}};
                    if (codepoint >= 0xD800 && codepoint <= 0xDBFF) {
                        if (eof() || get() != '\\')
                            return {Status::ParseError, JsonValue{}};
                        if (eof() || get() != 'u')
                            return {Status::ParseError, JsonValue{}};
                        uint32_t low = 0;
                        if (!read_hex4(low))
                            return {Status::ParseError, JsonValue{}};
                        if (low < 0xDC00 || low > 0xDFFF)
                            return {Status::ParseError, JsonValue{}};
                        codepoint = 0x10000 + ((codepoint - 0xD800) << 10) + (low - 0xDC00);
                    } else if (codepoint >= 0xDC00 && codepoint <= 0xDFFF) {
                        return {Status::ParseError, JsonValue{}};
                    }
                    if (!append_utf8(out, codepoint))
                        return {Status::ParseError, JsonValue{}};
                    break;
                }
                default:
                    return {Status::ParseError, JsonValue{}};
                }
            } else {
                if (static_cast<unsigned char>(c) < 0x20) {
                    return {Status::ParseError, JsonValue{}};
                }
                out.push_back(c);
            }
        }
        return {Status::ParseError, JsonValue{}};
    }

    Result<JsonValue> parse_array() {
        skip_ws();
        if (!consume('['))
            return {Status::ParseError, JsonValue{}};
        JsonValue::Array arr;
        skip_ws();
        if (consume(']'))
            return {Status::Ok, JsonValue(JsonValue::Variant(std::move(arr)))};
        while (true) {
            auto elem = parse_value();
            if (!elem.ok())
                return elem;
            arr.push_back(std::move(elem.value));
            skip_ws();
            if (consume(']'))
                break;
            if (!consume(','))
                return {Status::ParseError, JsonValue{}};
        }
        return {Status::Ok, JsonValue(JsonValue::Variant(std::move(arr)))};
    }

    Result<JsonValue> parse_object() {
        skip_ws();
        if (!consume('{'))
            return {Status::ParseError, JsonValue{}};
        JsonValue::Object obj;
        skip_ws();
        if (consume('}'))
            return {Status::Ok, JsonValue(JsonValue::Variant(std::move(obj)))};
        while (true) {
            auto key_res = parse_string();
            if (!key_res.ok() || !key_res.value.is_string())
                return {Status::ParseError, JsonValue{}};
            std::string key = key_res.value.as_string();
            if (!consume(':'))
                return {Status::ParseError, JsonValue{}};
            auto val_res = parse_value();
            if (!val_res.ok())
                return val_res;
            auto existing = obj.find(key);
            if (existing != obj.end()) {
                switch (g_duplicate_policy) {
                case DuplicateKeyPolicy::FirstWins:
                    break;
                case DuplicateKeyPolicy::Warn:
                    g_parse_warnings.push_back("Duplicate key encountered: " + key);
                    break;
                case DuplicateKeyPolicy::LastWins:
                    existing->second = std::move(val_res.value);
                    break;
                case DuplicateKeyPolicy::Error:
                    return {Status::ParseError, JsonValue{}};
                }
            } else {
                obj.emplace(std::move(key), std::move(val_res.value));
            }
            skip_ws();
            if (consume('}'))
                break;
            if (!consume(','))
                return {Status::ParseError, JsonValue{}};
        }
        return {Status::Ok, JsonValue(JsonValue::Variant(std::move(obj)))};
    }
};

} // namespace

Result<JsonValue> parse_json(std::string_view text) {
    g_parse_warnings.clear();
    if (!text.empty() && !util::validate_utf8_simd(text.data(), text.size())) {
        return {Status::ParseError, JsonValue{}};
    }
    Parser p{text.data(), text.size(), 0};
    auto res = p.parse_value();
    if (!res.ok())
        return res;
    p.skip_ws();
    if (!p.eof())
        return {Status::ParseError, JsonValue{}};
    return res;
}

void set_duplicate_key_policy(DuplicateKeyPolicy policy) { g_duplicate_policy = policy; }

DuplicateKeyPolicy get_duplicate_key_policy() { return g_duplicate_policy; }

std::vector<std::string> consume_parse_warnings() {
    std::vector<std::string> warnings = std::move(g_parse_warnings);
    g_parse_warnings.clear();
    return warnings;
}

} // namespace strata
