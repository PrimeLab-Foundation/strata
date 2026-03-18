/**
 * @file ndjson_stream.cpp
 * @brief NDJSON streaming parser implementation.
 *
 * Line splitting uses SIMD-accelerated newline search
 * (find_newline_simd) for fast scanning of large buffers.
 * Batch mode (parse_all_fast) pre-counts lines via SIMD to
 * pre-allocate the result vector, reducing reallocations.
 *
 * parse_batch_chunked() collects newline positions in batches and
 * parses each line in order, supporting both skip-errors and
 * stop-on-first-error modes.
 */

#include "strata/json/ndjson_stream.hpp"

#include "strata/simd/index_builder.h"
#include "strata/speculative/parser.h"
#include "strata/util/arena_allocator.hpp"
#include "strata/util/simd_string.hpp"

namespace strata {

namespace {

static inline bool is_json_whitespace(unsigned char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v';
}

static inline bool is_whitespace_only_line(std::string_view line) {
    if (line.empty()) {
        return true;
    }

    unsigned char first = static_cast<unsigned char>(line.front());
    if (!is_json_whitespace(first)) {
        return false;
    }

    return util::is_whitespace_only_simd(line.data(), line.size());
}

} // namespace

NdjsonStream::NdjsonStream(std::string_view data)
    : data_(data), pos_(0), line_num_(1), lines_processed_(0), error_count_(0), spec_model_() {}

Result<JsonValue> NdjsonStream::parse_line_speculative(std::string_view line) {
    // First N lines: validate with parse_json() and train the speculative model.
    // After warmup, use the speculative parser for speed (skip full validation).
    constexpr size_t WARMUP_LINES = 10;

    if (lines_processed_ <= WARMUP_LINES) {
        // Validate with the strict parser
        auto result = parse_json(line);
        if (!result.ok())
            return result;

        // Train the speculative model on this validated line
        spec_model_.train_from_samples(reinterpret_cast<const uint8_t*>(line.data()), line.size(),
                                       1);

        return result;
    }

    // Post-warmup: use speculative parser for throughput
    strata::simd::IndexBuilder idx_builder;
    auto index = idx_builder.build(reinterpret_cast<const uint8_t*>(line.data()), line.size());

    if (index.positions.empty()) {
        return parse_json(line);
    }

    strata::util::Arena arena;
    speculative::SpeculativeParser::Config config;
    config.enable_speculation = true;
    config.enable_online_learning = true;
    config.online_learning_warmup = 0;
    speculative::SpeculativeParser parser(config, arena);

    parser.model() = spec_model_;

    // Apply key filter for selective parsing if set.
    if (key_filter_) {
        parser.set_key_filter(key_filter_.get());
    }

    auto result = parser.parse(reinterpret_cast<const uint8_t*>(line.data()), line.size(),
                               index.positions.data(), index.positions.size());

    spec_model_ = parser.model();

    return {Status::Ok, std::move(result)};
}

std::string_view NdjsonStream::next_line() {
    if (pos_ >= data_.size()) {
        return std::string_view();
    }

    // SIMD-accelerated newline search
    const char* start = data_.data() + pos_;
    size_t remaining = data_.size() - pos_;

    // Find newline using SIMD (16 or 32 bytes at a time)
    size_t newline_offset = util::find_newline_simd(start, remaining, 0);

    size_t line_len;
    if (newline_offset < remaining) {
        // Found newline
        line_len = newline_offset;
        pos_ += newline_offset + 1;
    } else {
        // Last line without newline
        line_len = remaining;
        pos_ = data_.size();
    }

    // Handle \r\n (Windows line endings)
    if (line_len > 0 && start[line_len - 1] == '\r') {
        --line_len;
    }

    line_num_++;
    return std::string_view(start, line_len);
}

std::string_view NdjsonStream::read_raw_line() {
    while (pos_ < data_.size()) {
        std::string_view line = next_line();
        if (is_whitespace_only_line(line)) {
            continue;
        }
        lines_processed_++;
        return line;
    }
    return std::string_view{};
}

bool NdjsonStream::parse_batch_chunked(size_t max_results, bool skip_errors,
                                       std::vector<JsonValue>& results) {
    if (pos_ >= data_.size() || max_results == 0) {
        return false;
    }

    size_t parsed = 0;
    bool hit_error = false;

    while (parsed < max_results && pos_ < data_.size()) {
        std::string_view line = next_line();

        if (is_whitespace_only_line(line)) {
            continue;
        }

        lines_processed_++;
        auto result = parse_line_speculative(line);
        if (result.ok()) {
            results.push_back(std::move(result.value));
            parsed++;
        } else if (!skip_errors) {
            error_count_++;
            hit_error = true;
            break;
        } else {
            error_count_++;
        }
    }

    return hit_error;
}

Result<JsonValue> NdjsonStream::next() {
    while (pos_ < data_.size()) {
        std::string_view line = next_line();

        // Skip empty/whitespace-only lines
        if (is_whitespace_only_line(line)) {
            continue;
        }

        // Parse JSON (speculative with online learning)
        lines_processed_++;
        auto result = parse_line_speculative(line);

        if (!result.ok()) {
            error_count_++;
            return result;
        }

        return result;
    }

    // End of stream
    return {Status::KeyNotFound, JsonValue()};
}

bool NdjsonStream::has_next() const noexcept {
    // Quick check: if we haven't reached end of data, assume there's content
    // This avoids expensive O(n) scan on every call
    return pos_ < data_.size();
}

std::vector<JsonValue> NdjsonStream::parse_all(bool skip_errors) {
    std::vector<JsonValue> results;
    results.reserve(1000); // Pre-allocate for typical case

    while (has_next()) {
        auto result = next();

        if (result.ok()) {
            results.push_back(std::move(result.value));
        } else if (!skip_errors && result.status == Status::ParseError) {
            // Stop on error
            break;
        }
        // Continue if skip_errors is true or if we hit end of stream
    }

    return results;
}

std::vector<JsonValue> NdjsonStream::parse_all_fast(bool skip_errors) {
    // Count newlines using SIMD for better allocation
    size_t remaining = data_.size() - pos_;
    size_t line_count = util::count_newlines_simd(data_.data() + pos_, remaining) + 1;

    std::vector<JsonValue> results;
    results.reserve(line_count);

    constexpr size_t kBatchLines = 256;
    while (pos_ < data_.size()) {
        bool hit_error = parse_batch_chunked(kBatchLines, skip_errors, results);
        if (hit_error && !skip_errors) {
            break;
        }
    }

    return results;
}

std::vector<JsonValue> NdjsonStream::next_batch(size_t batch_size, bool skip_errors) {
    std::vector<JsonValue> results;
    results.reserve(batch_size);

    parse_batch_chunked(batch_size, skip_errors, results);
    return results;
}

} // namespace strata
