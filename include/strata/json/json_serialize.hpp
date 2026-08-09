#pragma once

/**
 * @file json_serialize.hpp
 * @brief JsonValue to JSON text.
 *
 * Compact output: no whitespace anywhere. Object keys keep the order they were
 * inserted, which for a parsed document is the order they appeared in the
 * input. (The previous header claimed keys were sorted through a `std::map`;
 * they never were.)
 *
 * Strings are escaped per RFC 8259 and otherwise emitted as UTF-8: only the
 * characters JSON requires to be escaped are escaped.
 *
 * JSON has no NaN or infinity, so both are written as `null`.
 *
 * Like the parser, this recurses and imposes no depth limit — deep enough
 * input will exhaust the stack (docs/architecture/SKILL.md, invariant 1). A
 * JsonValue tree cannot contain a cycle, so there is nothing to detect.
 */

#include "strata/json/json_core.hpp"

#include <string>

namespace strata {

/// Serialize @p value to a new string.
[[nodiscard]] std::string serialize_json(const JsonValue& value);

/// Serialize @p value into @p out, replacing its contents and reusing its
/// capacity across calls.
void serialize_json_to(const JsonValue& value, std::string& out);

} // namespace strata
