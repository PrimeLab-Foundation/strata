#pragma once

#include "strata/parse.hpp"

namespace strata {
    template<>
    Result<std::nullptr_t> parse<std::nullptr_t>(const char *cur, const char *end);
}