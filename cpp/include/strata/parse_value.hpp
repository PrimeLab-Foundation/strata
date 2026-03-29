#pragma once

#include "strata/parse.hpp"
#include "strata/primitives/parse_null.hpp"
#include "strata/primitives/parse_bool.hpp"
#include "strata/primitives/parse_number.hpp"
#include "strata/primitives/parse_string.hpp"

#include <variant>

namespace strata {

    using JsonValue = std::variant<std::nullptr_t, bool, Number, std::string>;

    template <>
    Result<JsonValue> parse<JsonValue>(const char* cur, const char* end);

}