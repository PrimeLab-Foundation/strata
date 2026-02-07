/**
 * @file json_parse.hpp
 * @brief JSON parsing API for Strata.
 *
 * Provides the main parsing interface:
 * - parse_json(): Parse JSON string to JsonValue DOM
 * - parse_json_with_sax(): SAX-style parsing with custom handler
 * - DuplicateKeyPolicy: Control duplicate key handling
 *
 * Example usage:
 * @code
 * auto result = strata::parse_json(R"({"name": "Alice", "age": 30})");
 * if (result.ok()) {
 *     const auto& obj = result.value.as_object();
 *     std::cout << obj.at("name").as_string() << std::endl;
 * }
 * @endcode
 *
 * @see json_core.hpp for JsonValue type
 * @see json_sax_handler.hpp for SAX handler interface
 */

#pragma once

#include "strata/json/json_core.hpp"
#include "strata/json/json_sax_handler.hpp"
#include "strata/json/json_tape.hpp"

#include <string_view>
#include <vector>

namespace strata {

/**
 * @brief Policy for handling duplicate keys in JSON objects.
 */
enum class DuplicateKeyPolicy {
    FirstWins, ///< Keep first occurrence (default)
    LastWins,  ///< Keep last occurrence
    Error,     ///< Return error on duplicate
    Warn       ///< Keep first, emit warning
};

// Parse a JSON text into a JsonValue tree.
// Supports: null, true, false, numbers, strings, arrays, objects.
Result<JsonValue> parse_json(std::string_view text);

// Parse a JSON text using a SAX handler.
Status parse_sax(std::string_view text, JsonSaxHandler& handler);

// Parse a JSON text into a tape format for efficient repeated access.
// The tape can later be converted to DOM using tape_to_dom().
Result<JsonTape> parse_to_tape(std::string_view text);

// Configure duplicate-key handling (thread-local)
void set_duplicate_key_policy(DuplicateKeyPolicy policy);
DuplicateKeyPolicy get_duplicate_key_policy();

// Retrieve and clear any warnings (e.g., duplicate keys when policy == Warn)
std::vector<std::string> consume_parse_warnings();

} // namespace strata
