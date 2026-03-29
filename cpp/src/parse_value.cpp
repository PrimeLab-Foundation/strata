#include "strata/parse_value.hpp"
#include "strata/simd/ops.hpp"

namespace strata {

    template <>
    Result<JsonValue> parse<JsonValue>(const char* cur, const char* end) {
        cur = simd::skip_ws(cur, end);

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
            default:
                return std::unexpected(ParseError{ErrorCode::UnexpectedChar, cur});
        }
    }

}