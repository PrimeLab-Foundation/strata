#include "strata/parse_bool.hpp"          // Parsed<bool>, Result<bool>, parse<bool> declaration
#include <cstring>                         // std::memcmp

namespace strata {

    template <>                            // explicit specialization of the base parse<T> template
    Result<bool> parse<bool>(              // returns either {bool, cursor} or ParseError
        const char* cur,                   // pointer to current position in input
        const char* end                    // pointer to one-past-end of input
    ) {
        auto remaining = static_cast<size_t>(end - cur);  // bytes left to read

        if (remaining == 0)                                      // nothing to read at all
            return std::unexpected(ParseError{ErrorCode::UnexpectedEnd, cur});

        if (remaining >= 4 && std::memcmp(cur, "true", 4) == 0)  // enough bytes + match "true"
            return Parsed<bool>{true, cur + 4};                  // success: value=true, advance 4

        if (remaining >= 5 && std::memcmp(cur, "false", 5) == 0) // enough bytes + match "false"
            return Parsed<bool>{false, cur + 5};                 // success: value=false, advance 5

        return std::unexpected(ParseError{ErrorCode::InvalidLiteral, cur}); // had bytes but no match
    }

}