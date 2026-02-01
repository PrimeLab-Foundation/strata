#pragma once

#include "strata/json/json_core.hpp"

#include <string_view>

namespace strata {

// Parse a JSON text into a JsonValue tree.
// Supports: null, true, false, numbers, strings, arrays, objects.
Result<JsonValue> parse_json(std::string_view text);

} // namespace strata
