#include "json_parse.hpp"

#include <cctype>
#include <stdexcept>

namespace strata {

namespace {

struct Parser {
    std::string_view s;
    size_t i = 0;

    bool eof() const { return i >= s.size(); }
    char peek() const { return eof() ? '\0' : s[i]; }
    char get() { return eof() ? '\0' : s[i++]; }

    void skip_ws() {
        while (!eof() && std::isspace(static_cast<unsigned char>(peek()))) {
            ++i;
        }
    }

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
        if (s.substr(i, 4) == "null") {
            i += 4;
            return {Status::Ok, JsonValue{}};
        }
        return {Status::ParseError, JsonValue{}};
    }

    Result<JsonValue> parse_bool() {
        skip_ws();
        if (s.substr(i, 4) == "true") {
            i += 4;
            return {Status::Ok, JsonValue(JsonValue::Variant(true))};
        }
        if (s.substr(i, 5) == "false") {
            i += 5;
            return {Status::Ok, JsonValue(JsonValue::Variant(false))};
        }
        return {Status::ParseError, JsonValue{}};
    }

    Result<JsonValue> parse_number() {
        skip_ws();
        size_t start = i;
        if (peek() == '-')
            ++i;
        if (!std::isdigit(static_cast<unsigned char>(peek())))
            return {Status::ParseError, JsonValue{}};
        if (peek() == '0') {
            ++i;
        } else {
            while (std::isdigit(static_cast<unsigned char>(peek())))
                ++i;
        }
        if (peek() == '.') {
            ++i;
            if (!std::isdigit(static_cast<unsigned char>(peek())))
                return {Status::ParseError, JsonValue{}};
            while (std::isdigit(static_cast<unsigned char>(peek())))
                ++i;
        }
        if (peek() == 'e' || peek() == 'E') {
            ++i;
            if (peek() == '+' || peek() == '-')
                ++i;
            if (!std::isdigit(static_cast<unsigned char>(peek())))
                return {Status::ParseError, JsonValue{}};
            while (std::isdigit(static_cast<unsigned char>(peek())))
                ++i;
        }
        double val = 0.0;
        try {
            val = std::stod(std::string(s.substr(start, i - start)));
        } catch (...) {
            return {Status::ParseError, JsonValue{}};
        }
        return {Status::Ok, JsonValue(JsonValue::Variant(val))};
    }

    Result<JsonValue> parse_string() {
        skip_ws();
        if (get() != '"')
            return {Status::ParseError, JsonValue{}};
        std::string out;
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
                default:
                    return {Status::ParseError, JsonValue{}};
                }
            } else {
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
            obj.emplace(std::move(key), std::move(val_res.value));
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
    Parser p{text, 0};
    auto res = p.parse_value();
    if (!res.ok())
        return res;
    p.skip_ws();
    if (!p.eof())
        return {Status::ParseError, JsonValue{}};
    return res;
}

} // namespace strata
