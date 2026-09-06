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
 * This recurses and imposes no depth limit of its own — deep enough input
 * will exhaust the stack (docs/architecture/SKILL.md, invariant 1). The parser
 * refuses past kMaxNestingDepth containers, so a tree it built is bounded;
 * one adopted through JsonDocument(JsonValue) is not. A JsonValue tree cannot
 * contain a cycle, so there is nothing to detect.
 */

#include "strata/json/json_core.hpp"

#include <string>
#include <string_view>

namespace strata {

/**
 * Append @p text to @p out as a quoted, escaped JSON string.
 *
 * Escapes exactly what RFC 8259 requires: the quote, the backslash, the five
 * short escapes, and any remaining byte below 0x20 as `\u00xx`. Everything
 * else is copied verbatim, so UTF-8 passes through unchanged.
 *
 * Shared with the binding layer, which escapes Python strings against the same
 * table rather than keeping a second copy of it.
 */
void append_escaped_json_string(std::string_view text, std::string& out);

/// Serialize @p value to a new string.
[[nodiscard]] std::string serialize_json(const JsonValue& value);

/// Serialize @p value into @p out, replacing its contents and reusing its
/// capacity across calls.
void serialize_json_to(const JsonValue& value, std::string& out);

} // namespace strata
