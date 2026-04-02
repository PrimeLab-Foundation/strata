#pragma once

#include "strata/parse.hpp"
#include "strata/primitives/parse_null.hpp"
#include "strata/primitives/parse_bool.hpp"
#include "strata/primitives/parse_number.hpp"
#include "strata/primitives/parse_string.hpp"

#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace strata {
    struct JsonValue {
        using Array = std::vector<JsonValue>;
        using Object = std::vector<std::pair<std::string, JsonValue> >;

        std::variant<std::nullptr_t, bool, Number, std::string, Array, Object> data;

        JsonValue() : data(nullptr) {
        }

        JsonValue(std::nullptr_t) : data(nullptr) {
        }

        JsonValue(bool b) : data(b) {
        }

        JsonValue(Number n) : data(n) {
        }

        JsonValue(std::string s) : data(std::move(s)) {
        }

        JsonValue(Array a) : data(std::move(a)) {
        }

        JsonValue(Object o) : data(std::move(o)) {
        }
    };

    template<>
    Result<JsonValue> parse<JsonValue>(const char *cur, const char *end);
}