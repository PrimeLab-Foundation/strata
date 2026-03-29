#include "strata/primitives/parse_bool.hpp"
#include "strata/simd/ops.hpp"

namespace strata {

template <>
Result<bool> parse<bool>(const char* cur, const char* end) {
    if (simd::match4(cur, end, "true"))
        return Parsed<bool>{true, cur + 4};

    if (simd::match5(cur, end, "false"))
        return Parsed<bool>{false, cur + 5};

    if (cur >= end)
        return std::unexpected(ParseError{ErrorCode::UnexpectedEnd, cur});

    return std::unexpected(ParseError{ErrorCode::InvalidLiteral, cur});
}

}