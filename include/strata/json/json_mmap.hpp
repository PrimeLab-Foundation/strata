#pragma once

/**
 * @file json_mmap.hpp
 * @brief Memory-mapped JSON file parser.
 *
 * Provides zero-copy parsing of JSON files using memory-mapped I/O.
 *
 * Benefits:
 * - Fast file loading (OS handles page loading)
 * - Zero-copy string views into file data
 * - Memory efficient (only used pages loaded)
 * - Great for large files
 *
 * Example:
 *   auto result = parse_json_file("/path/to/data.json");
 *   if (result.ok()) {
 *       auto doc = std::move(result.value);
 *       // Use document...
 *   }
 */

#include "strata/json/json_core.hpp"
#include "strata/json/json_document.hpp"

#include <string_view>

namespace strata {

/**
 * Parse a JSON file using memory-mapped I/O.
 *
 * @param filepath Path to JSON file.
 * @return Result with JsonDocument or error status.
 */
[[nodiscard]] Result<JsonDocument> parse_json_file(const char* filepath);

/**
 * Parse a JSON file and return the root cursor.
 * Convenience function that returns JsonCursor directly.
 *
 * @param filepath Path to JSON file.
 * @return Result with JsonCursor or error status.
 */
[[nodiscard]] Result<JsonCursor> parse_json_file_cursor(const char* filepath);

} // namespace strata
