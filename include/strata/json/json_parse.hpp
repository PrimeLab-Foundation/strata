#pragma once

/**
 * @file json_parse.hpp
 * @brief JSON parsing public API.
 *
 * Provides two parsing interfaces:
 * - parse_json()  — builds a full JsonValue DOM tree.
 * - parse_sax()   — streams SAX events to a user-supplied handler.
 *
 * Both are built on ParserInline<Handler> (json_parser_inline.hpp) which
 * enables the compiler to devirtualise and inline handler callbacks when
 * instantiated with a concrete handler type.
 *
 * Thread-local state:
 * - DuplicateKeyPolicy controls handling of repeated object keys.
 * - Parse warnings (e.g. duplicate keys in Warn mode) are collected in
 *   a thread-local vector and retrieved via consume_parse_warnings().
 */

#include "strata/json/json_core.hpp"
#include "strata/json/json_sax_handler.hpp"

#include <string_view>
#include <vector>

namespace strata {

/// Strategy for handling duplicate object keys during parsing.
enum class DuplicateKeyPolicy { FirstWins, LastWins, Error, Warn };

/**
 * Parse a JSON text into a JsonValue tree.
 *
 * @param text  UTF-8 encoded JSON input.
 * @return Result containing the root JsonValue on success, or
 *         Status::ParseError on malformed input.
 */
[[nodiscard]] Result<JsonValue> parse_json(std::string_view text);

/**
 * Parse a JSON text using a SAX handler.
 *
 * When @p validate_utf8 is false the upfront SIMD UTF-8 pass is skipped;
 * this is safe when the caller will create Python unicode objects
 * (PyUnicode_FromStringAndSize validates inline) or when the input is
 * already known-valid UTF-8.
 *
 * @param text          UTF-8 JSON input.
 * @param handler       SAX event sink.
 * @param validate_utf8 Run SIMD UTF-8 validation before parsing (default: true).
 * @return Status::Ok on success, Status::ParseError on failure.
 */
[[nodiscard]] Status parse_sax(std::string_view text, JsonSaxHandler& handler,
                               bool validate_utf8 = true);

/// Configure duplicate-key handling (thread-local).
void set_duplicate_key_policy(DuplicateKeyPolicy policy);

/// Query the current duplicate-key policy (thread-local).
[[nodiscard]] DuplicateKeyPolicy get_duplicate_key_policy();

/// Retrieve and clear any parse warnings (e.g. duplicate keys when policy == Warn).
[[nodiscard]] std::vector<std::string> consume_parse_warnings();

} // namespace strata
