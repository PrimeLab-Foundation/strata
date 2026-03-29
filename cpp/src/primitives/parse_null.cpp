#include "strata/primitives/parse_null.hpp"           // Parsed<nullptr_t>, Result<nullptr_t>, parse decl
#include <cstring>                          // std::memcmp

namespace strata {

    template <>
    Result<std::nullptr_t> parse<std::nullptr_t>(
        const char* cur,
        const char* end
    ) {
        auto remaining = static_cast<size_t>(end - cur);  // bytes left

        if (remaining < 4)                                // "null" is 4 chars — need at least 4
            return std::unexpected(ParseError{ErrorCode::UnexpectedEnd, cur});

        if (std::memcmp(cur, "null", 4) != 0)             // 4+ bytes but doesn't match "null"
            return std::unexpected(ParseError{ErrorCode::InvalidLiteral, cur});

        return Parsed<std::nullptr_t>{nullptr, cur + 4};  // success: value=nullptr, advance 4
    }

}