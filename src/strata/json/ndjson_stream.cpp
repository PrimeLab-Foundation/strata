#include "strata/json/ndjson_stream.hpp"

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
    : data_(data), pos_(0), line_num_(1), lines_processed_(0), error_count_(0),
      utf8_checked_(false), utf8_ok_(true) {}

bool NdjsonStream::validate_utf8_once() {
    if (!utf8_checked_) {
        utf8_ok_ = util::validate_utf8_lazy(data_.data(), data_.size());
        utf8_checked_ = true;
    }
    return utf8_ok_;
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

bool NdjsonStream::parse_batch_chunked(size_t max_results, bool skip_errors,
                                       std::vector<JsonValue>& results) {
    if (pos_ >= data_.size() || max_results == 0) {
        return false;
    }

    size_t parsed = 0;
    bool hit_error = false;

    ParseSaxOptions options;
    options.validate_utf8 = !(utf8_checked_ && utf8_ok_);

    while (parsed < max_results && pos_ < data_.size()) {
        std::string_view line = next_line();

        if (is_whitespace_only_line(line)) {
            continue;
        }

        lines_processed_++;
        auto result = parse_json(line, options, &parse_context_);
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

        // Parse JSON
        lines_processed_++;
        ParseSaxOptions options;
        options.validate_utf8 = !(utf8_checked_ && utf8_ok_);
        auto result = parse_json(line, options, &parse_context_);

        if (!result.ok()) {
            error_count_++;
            return result;
        }

        return result;
    }

    // End of stream
    return {Status::KeyNotFound, JsonValue()};
}

Status NdjsonStream::next_sax(JsonSaxHandler& handler) {
    while (pos_ < data_.size()) {
        std::string_view line = next_line();

        if (is_whitespace_only_line(line)) {
            continue;
        }

        lines_processed_++;
        ParseSaxOptions options;
        options.validate_utf8 = !(utf8_checked_ && utf8_ok_);
        Status status = parse_sax(line, handler, options, &parse_context_);
        if (status != Status::Ok) {
            error_count_++;
            return Status::ParseError;
        }
        return Status::Ok;
    }

    return Status::KeyNotFound;
}

bool NdjsonStream::has_next() const {
    // Quick check: if we haven't reached end of data, assume there's content
    // This avoids expensive O(n) scan on every call
    return pos_ < data_.size();
}

std::vector<JsonValue> NdjsonStream::parse_all(bool skip_errors) {
    std::vector<JsonValue> results;
    results.reserve(1000); // Pre-allocate for typical case

    validate_utf8_once();

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
    validate_utf8_once();

    // Count newlines using SIMD for better pre-allocation
    size_t remaining = data_.size() - pos_;
    size_t line_count = util::count_newlines_simd(data_.data() + pos_, remaining) + 1;

    std::vector<JsonValue> results;
    results.reserve(line_count);

    // Use larger batch size for better SIMD amortization
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

    validate_utf8_once();
    parse_batch_chunked(batch_size, skip_errors, results);
    return results;
}

} // namespace strata
