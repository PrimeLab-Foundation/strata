#include "strata/primitives/parse_null.hpp"
#include "strata/simd/ops.hpp"

namespace strata {
    template<>
    Result<std::nullptr_t> parse<std::nullptr_t>(const char *cur, const char *end) {
        if (simd::match4(cur, end, "null"))
            return Parsed<std::nullptr_t>{nullptr, cur + 4};

        if (static_cast<size_t>(end - cur) < 4)
            return std::unexpected(ParseError{ErrorCode::UnexpectedEnd, cur});

        return std::unexpected(ParseError{ErrorCode::InvalidLiteral, cur});
    }
}