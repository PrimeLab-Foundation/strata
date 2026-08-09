#pragma once

/**
 * @file json_parse.hpp
 * @brief Public JSON parsing API.
 *
 * Two entry points sit on the same engine:
 *
 * - parse_json() builds a full JsonValue DOM.
 * - parse_sax() streams events to a runtime-polymorphic handler.
 *
 * Both are ParserInline<Handler> underneath (json_parser_inline.hpp); callers
 * with a concrete handler should instantiate parse_sax_inline<Handler>()
 * directly and skip the virtual dispatch.
 */

#include "strata/json/json_core.hpp"
#include "strata/json/json_sax_handler.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace strata {

/// What to do when an object repeats a key.
enum class DuplicateKeyPolicy {
    FirstWins, ///< Keep the first occurrence (default).
    LastWins,  ///< Overwrite with the last occurrence.
    Error,     ///< Fail the parse.
    Warn,      ///< Keep the first, and record a warning.
};

/**
 * Parse @p text into a JsonValue tree.
 *
 * Numbers land in the DOM as doubles, so integers beyond 2^53 — including
 * every value the SAX layer reports as a big integer — lose precision here.
 * That is a property of this DOM, not of the engine: the Python builder
 * consumes the same events and keeps integers exact.
 *
 * @return The root value, or Status::ParseError for malformed input.
 */
[[nodiscard]] Result<JsonValue> parse_json(std::string_view text);

/**
 * Parse @p text, streaming events to @p handler through virtual dispatch.
 *
 * @param validate_utf8 Check the input up front; see parse_sax_inline().
 */
[[nodiscard]] Status parse_sax(std::string_view text, JsonSaxHandler& handler,
                               bool validate_utf8 = true);

/// Set the duplicate-key policy. **Thread-local**: it does not reach other threads.
void set_duplicate_key_policy(DuplicateKeyPolicy policy) noexcept;

/// Current duplicate-key policy for this thread.
[[nodiscard]] DuplicateKeyPolicy get_duplicate_key_policy() noexcept;

/// Take the warnings recorded by the last parse on this thread, clearing them.
[[nodiscard]] std::vector<std::string> consume_parse_warnings();

} // namespace strata
