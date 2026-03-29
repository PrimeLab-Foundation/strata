#pragma once

#include "strata/parse.hpp"

#include <string>

namespace strata {

    template <>
    Result<std::string> parse<std::string>(const char* cur, const char* end);

}
