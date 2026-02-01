#pragma once

#include "strata/json/json_core.hpp"

#include <string>
#include <string_view>

namespace strata {

/**
 * Serialize a JsonValue to JSON string.
 *
 * Features:
 * - Correct JSON escaping for strings
 * - Unicode support (UTF-8)
 * - Deterministic output (object keys in insertion order)
 * - Minimal allocations (reusable buffer)
 * - NaN/Inf policy: Converts to null (JSON spec compliant)
 *
 * @param value The JsonValue to serialize
 * @return JSON string representation
 */
std::string serialize_json(const JsonValue& value);

/**
 * Serialize to pre-allocated string (for efficiency).
 *
 * @param value The JsonValue to serialize
 * @param out Output string (will be cleared and reused)
 */
void serialize_json_to(const JsonValue& value, std::string& out);

} // namespace strata
