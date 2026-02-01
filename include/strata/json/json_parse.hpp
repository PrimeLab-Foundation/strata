#pragma once

#include "strata/json/json_core.hpp"

#include <string_view>
#include <vector>

namespace strata {

// Strategy for handling duplicate object keys during parsing
enum class DuplicateKeyPolicy { FirstWins, LastWins, Error, Warn };

// Parse a JSON text into a JsonValue tree.
// Supports: null, true, false, numbers, strings, arrays, objects.
Result<JsonValue> parse_json(std::string_view text);

// Configure duplicate-key handling (thread-local)
void set_duplicate_key_policy(DuplicateKeyPolicy policy);
DuplicateKeyPolicy get_duplicate_key_policy();

// Retrieve and clear any warnings (e.g., duplicate keys when policy == Warn)
std::vector<std::string> consume_parse_warnings();

} // namespace strata
