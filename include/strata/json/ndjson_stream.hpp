#pragma once

#include "strata/json/json_core.hpp"
#include "strata/json/json_parse.hpp"

#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace strata {

/**
 * NDJSON (Newline Delimited JSON) streaming parser.
 *
 * Efficiently parses NDJSON data line-by-line without loading entire file into memory.
 *
 * Features:
 * - Line-by-line streaming (memory-efficient)
 * - Handles blank lines gracefully
 * - Skips malformed lines with error reporting
 * - Supports both string and file input
 *
 * Format: Each line is a complete, valid JSON object.
 * Example:
 *   {"name": "Alice", "age": 30}
 *   {"name": "Bob", "age": 25}
 *   {"name": "Charlie", "age": 35}
 */
class NdjsonStream {
  public:
    /**
     * Create stream from string data.
     *
     * @param data NDJSON string data
     */
    explicit NdjsonStream(std::string_view data);

    /**
     * Parse next JSON line.
     *
     * @return Result with JsonValue or error status
     *         Returns Status::Ok with value on success
     *         Returns Status::ParseError if line is invalid JSON
     *         Returns Status::KeyNotFound if no more lines (end of stream)
     */
    Result<JsonValue> next();

    /**
     * Parse next JSON line using a SAX handler.
     *
     * @param handler SAX handler invoked during parsing
     * @return Status::Ok on success
     *         Status::ParseError if line is invalid JSON
     *         Status::KeyNotFound if no more lines (end of stream)
     */
    Status next_sax(JsonSaxHandler& handler);

    /**
     * Check if more lines are available.
     *
     * @return true if more lines can be read
     */
    bool has_next() const;

    /**
     * Parse all lines into a vector (optimized batch mode).
     *
     * @param skip_errors If true, skip malformed lines; if false, stop on first error
     * @return Vector of successfully parsed JsonValues
     */
    std::vector<JsonValue> parse_all(bool skip_errors = false);

    /**
     * Parse all lines into a vector with aggressive pre-allocation.
     * Faster than parse_all for large datasets with known line counts.
     * Uses SIMD line counting for precise pre-allocation.
     *
     * @param skip_errors If true, skip malformed lines
     * @return Vector of successfully parsed JsonValues
     */
    std::vector<JsonValue> parse_all_fast(bool skip_errors = false);

    /**
     * Parse next batch of lines (up to batch_size).
     * Zero-copy batch processing for reduced Python/C++ boundary crossings.
     *
     * @param batch_size Maximum number of lines to parse
     * @param skip_errors If true, skip malformed lines
     * @return Vector of successfully parsed JsonValues (may be smaller than batch_size)
     */
    std::vector<JsonValue> next_batch(size_t batch_size, bool skip_errors = false);

    /**
     * Validate the full NDJSON buffer once for UTF-8 correctness.
     *
     * @return true if the buffer is valid UTF-8 (or empty), false otherwise.
     */
    bool validate_utf8_once();

    /**
     * Get current line number (1-indexed).
     *
     * @return Current line number
     */
    size_t line_number() const { return line_num_; }

    /**
     * Get total lines processed.
     *
     * @return Total lines read
     */
    size_t lines_processed() const { return lines_processed_; }

    /**
     * Get number of errors encountered.
     *
     * @return Error count
     */
    size_t error_count() const { return error_count_; }

  private:
    std::string_view data_;
    size_t pos_;
    size_t line_num_;
    size_t lines_processed_;
    size_t error_count_;
    ParseSaxContext parse_context_;
    bool utf8_checked_;
    bool utf8_ok_;

    // Extract next line from data
    std::string_view next_line();

    // Parse a chunk of lines using SIMD newline collection.
    // Returns true if a parse error was hit and skip_errors is false.
    bool parse_batch_chunked(size_t max_results, bool skip_errors, std::vector<JsonValue>& results);
};

} // namespace strata
