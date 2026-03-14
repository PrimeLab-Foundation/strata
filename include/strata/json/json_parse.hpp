#pragma once

#include "strata/json/json_core.hpp"
#include "strata/json/json_sax_handler.hpp"

#include <string_view>
#include <vector>

namespace strata {

// Strategy for handling duplicate object keys during parsing
enum class DuplicateKeyPolicy { FirstWins, LastWins, Error, Warn };

// Parse a JSON text into a JsonValue tree.
// Supports: null, true, false, numbers, strings, arrays, objects.
Result<JsonValue> parse_json(std::string_view text);

// Parse a JSON text using a SAX handler.
// When validate_utf8 is false the upfront SIMD UTF-8 pass is skipped; safe when
// the caller will create Python unicode objects (PyUnicode_FromStringAndSize validates
// inline) or when the input is already known-valid UTF-8.
Status parse_sax(std::string_view text, JsonSaxHandler& handler, bool validate_utf8 = true);

// Configure duplicate-key handling (thread-local)
void set_duplicate_key_policy(DuplicateKeyPolicy policy);
DuplicateKeyPolicy get_duplicate_key_policy();

// Retrieve and clear any warnings (e.g., duplicate keys when policy == Warn)
std::vector<std::string> consume_parse_warnings();

} // namespace strata
