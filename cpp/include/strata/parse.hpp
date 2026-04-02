#pragma once

#include "strata/error.hpp"

namespace strata {
    template<typename T>
    Result<T> parse(const char *cur, const char *end) = delete;
}
