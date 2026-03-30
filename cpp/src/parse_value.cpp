#include "strata/parse_value.hpp"
#include "strata/simd/ops.hpp"

namespace strata {

    namespace {

        inline const char* ws(const char* cur, const char* end) {
            return simd::skip_ws(cur, end);
        }

        // --- array: [ value , value , ... ] ---

        Result<JsonValue::Array> parse_array(const char* cur, const char* end) {
            // cur points at '['
            ++cur;
            cur = ws(cur, end);

            if (cur >= end)
                return std::unexpected(ParseError{ErrorCode::UnexpectedEnd, cur});

            JsonValue::Array arr;

            // empty array
            if (*cur == ']')
                return Parsed<JsonValue::Array>{std::move(arr), cur + 1};

            while (true) {
                auto val = parse<JsonValue>(cur, end);
                if (!val) return std::unexpected(val.error());
                arr.push_back(std::move(val->value));
                cur = ws(val->rest, end);

                if (cur >= end)
                    return std::unexpected(ParseError{ErrorCode::UnexpectedEnd, cur});

                if (*cur == ']')
                    return Parsed<JsonValue::Array>{std::move(arr), cur + 1};

                if (*cur != ',')
                    return std::unexpected(ParseError{ErrorCode::UnexpectedChar, cur});

                ++cur; // skip ','
            }
        }

        // --- object: { "key" : value , ... } ---

        Result<JsonValue::Object> parse_object(const char* cur, const char* end) {
            // cur points at '{'
            ++cur;
            cur = ws(cur, end);

            if (cur >= end)
                return std::unexpected(ParseError{ErrorCode::UnexpectedEnd, cur});

            JsonValue::Object obj;

            // empty object
            if (*cur == '}')
                return Parsed<JsonValue::Object>{std::move(obj), cur + 1};

            while (true) {
                // key
                if (*cur != '"')
                    return std::unexpected(ParseError{ErrorCode::UnexpectedChar, cur});

                auto key = parse<std::string>(cur, end);
                if (!key) return std::unexpected(key.error());
                cur = ws(key->rest, end);

                // colon
                if (cur >= end)
                    return std::unexpected(ParseError{ErrorCode::UnexpectedEnd, cur});
                if (*cur != ':')
                    return std::unexpected(ParseError{ErrorCode::UnexpectedChar, cur});
                ++cur;

                // value
                auto val = parse<JsonValue>(cur, end);
                if (!val) return std::unexpected(val.error());
                obj.push_back({std::move(key->value), std::move(val->value)});
                cur = ws(val->rest, end);

                if (cur >= end)
                    return std::unexpected(ParseError{ErrorCode::UnexpectedEnd, cur});

                if (*cur == '}')
                    return Parsed<JsonValue::Object>{std::move(obj), cur + 1};

                if (*cur != ',')
                    return std::unexpected(ParseError{ErrorCode::UnexpectedChar, cur});

                ++cur; // skip ','
                cur = ws(cur, end);

                if (cur >= end)
                    return std::unexpected(ParseError{ErrorCode::UnexpectedEnd, cur});
            }
        }

    }

    template <>
    Result<JsonValue> parse<JsonValue>(const char* cur, const char* end) {
        cur = ws(cur, end);

        if (cur >= end)
            return std::unexpected(ParseError{ErrorCode::UnexpectedEnd, cur});

        switch (*cur) {
            case 'n': {
                auto r = parse<std::nullptr_t>(cur, end);
                if (!r) return std::unexpected(r.error());
                return Parsed<JsonValue>{r->value, r->rest};
            }
            case 't':
            case 'f': {
                auto r = parse<bool>(cur, end);
                if (!r) return std::unexpected(r.error());
                return Parsed<JsonValue>{r->value, r->rest};
            }
            case '"': {
                auto r = parse<std::string>(cur, end);
                if (!r) return std::unexpected(r.error());
                return Parsed<JsonValue>{std::move(r->value), r->rest};
            }
            case '-':
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9': {
                auto r = parse<Number>(cur, end);
                if (!r) return std::unexpected(r.error());
                return Parsed<JsonValue>{r->value, r->rest};
            }
            case '[': {
                auto r = parse_array(cur, end);
                if (!r) return std::unexpected(r.error());
                return Parsed<JsonValue>{std::move(r->value), r->rest};
            }
            case '{': {
                auto r = parse_object(cur, end);
                if (!r) return std::unexpected(r.error());
                return Parsed<JsonValue>{std::move(r->value), r->rest};
            }
            default:
                return std::unexpected(ParseError{ErrorCode::UnexpectedChar, cur});
        }
    }

}