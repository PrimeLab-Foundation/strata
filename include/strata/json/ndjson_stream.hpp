#pragma once

/**
 * @file ndjson_stream.hpp
 * @brief NDJSON (Newline Delimited JSON) streaming parser.
 *
 * Efficiently parses NDJSON data line-by-line without loading the
 * entire file into memory.  Uses SIMD-accelerated newline detection
 * for fast line splitting.
 *
 * Features:
 * - Line-by-line streaming (memory-efficient)
 * - Handles blank lines gracefully
 * - Skips malformed lines with error reporting
 * - Supports both string and file input
 * - SIMD-accelerated batch mode (parse_all_fast)
 *
 * Format: Each line is a complete, valid JSON object.
 * Example:
 *   {"name": "Alice", "age": 30}
 *   {"name": "Bob", "age": 25}
 */

#include "strata/bloom/key_filter.h"
#include "strata/json/json_core.hpp"
#include "strata/json/json_parse.hpp"
#include "strata/simd/index_builder.h"
#include "strata/speculative/parser.h"
#include "strata/util/arena_allocator.hpp"

#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace strata {

class NdjsonStream {
  public:
    /**
     * Create stream from string data.
     *
     * @param data NDJSON string data.
     */
    explicit NdjsonStream(std::string_view data);

    /**
     * Parse next JSON line.
     *
     * @return Result with JsonValue or error status.
     *         Status::Ok on success, Status::ParseError if line is invalid,
     *         Status::KeyNotFound if no more lines (end of stream).
     */
    [[nodiscard]] Result<JsonValue> next();

    /**
     * Check if more lines are available.
     *
     * @return true if more lines can be read.
     */
    [[nodiscard]] bool has_next() const noexcept;

    /**
     * Parse all lines into a vector (optimised batch mode).
     *
     * @param skip_errors If true, skip malformed lines; if false, stop on first error.
     * @return Vector of successfully parsed JsonValues.
     */
    [[nodiscard]] std::vector<JsonValue> parse_all(bool skip_errors = false);

    /**
     * Parse all lines with aggressive SIMD-based pre-allocation.
     *
     * Faster than parse_all() for large datasets with known line counts.
     * Uses SIMD line counting for precise pre-allocation.
     *
     * @param skip_errors If true, skip malformed lines.
     * @return Vector of successfully parsed JsonValues.
     */
    [[nodiscard]] std::vector<JsonValue> parse_all_fast(bool skip_errors = false);

    /**
     * Parse next batch of lines (up to @p batch_size).
     *
     * Zero-copy batch processing for reduced Python/C++ boundary crossings.
     *
     * @param batch_size  Maximum number of lines to parse.
     * @param skip_errors If true, skip malformed lines.
     * @return Vector of successfully parsed JsonValues (may be smaller than batch_size).
     */
    [[nodiscard]] std::vector<JsonValue> next_batch(size_t batch_size, bool skip_errors = false);

    /// Get current line number (1-indexed).
    [[nodiscard]] size_t line_number() const noexcept { return line_num_; }

    /// Get total lines processed.
    [[nodiscard]] size_t lines_processed() const noexcept { return lines_processed_; }

    /// Get number of errors encountered.
    [[nodiscard]] size_t error_count() const noexcept { return error_count_; }

    /**
     * Increment the error counter.
     *
     * Used by Python bindings that call read_raw_line() directly and
     * detect parse failures outside of NdjsonStream.
     */
    void record_error() noexcept { error_count_++; }

    /**
     * Extract and return the next raw line (string_view into internal data).
     *
     * Returns an empty string_view when there is no more data.
     * Skips blank and whitespace-only lines internally.
     * Increments lines_processed_ on each non-blank line returned.
     *
     * Used by the Python binding to call parse_sax directly (no intermediate C++ DOM).
     */
    [[nodiscard]] std::string_view read_raw_line();

    /// Set an optional key filter for selective parsing of NDJSON records.
    /// When set, only desired keys are parsed; others are skipped at near-zero cost.
    /// The filter is applied via the speculative parser's structural-index skipping.
    void set_key_filter(std::unique_ptr<bloom::KeyFilter> filter) noexcept {
        key_filter_ = std::move(filter);
    }

    /// Access the speculative model (for stats / serialization).
    [[nodiscard]] const speculative::TransitionModel& speculative_model() const noexcept {
        return spec_model_;
    }

  private:
    std::string_view data_;
    size_t pos_;
    size_t line_num_;
    size_t lines_processed_;
    size_t error_count_;

    /// Speculative transition model — persists across lines for online learning.
    speculative::TransitionModel spec_model_;

    /// Optional key filter for selective parsing (owned).
    std::unique_ptr<bloom::KeyFilter> key_filter_;

    /// Extract next line from data (up to newline or end).
    std::string_view next_line();

    /// Parse a single line using the speculative parser with online learning.
    Result<JsonValue> parse_line_speculative(std::string_view line);

    /// Parse a chunk of lines using SIMD newline collection.
    /// Returns true if a parse error was hit and skip_errors is false.
    bool parse_batch_chunked(size_t max_results, bool skip_errors, std::vector<JsonValue>& results);
};

} // namespace strata
