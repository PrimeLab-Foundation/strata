#pragma once

#include "strata/parse.hpp"

namespace strata {

    template <>
    Result<bool> parse<bool>(const char* cur, const char* end);

}
