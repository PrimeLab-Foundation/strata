#pragma once

#include "strata/parse.hpp"

#include <cstdint>
#include <variant>

namespace strata {

    using Number = std::variant<int64_t, double>;

    template <>
    Result<Number> parse<Number>(const char* cur, const char* end);

}
