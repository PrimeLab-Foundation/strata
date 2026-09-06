/**
 * @file ndjson_stream.cpp
 * @brief NDJSON line splitting and per-line parsing.
 */

#include "strata/json/ndjson_stream.hpp"

#include "strata/json/json_parse.hpp"

#include <utility>

namespace strata {

namespace {

/// Trim the trailing CR of a `\r\n` ending, and any surrounding blanks.
[[nodiscard]] std::string_view trim(std::string_view line) noexcept {
    while (!line.empty() && (line.back() == '\r' || line.back() == ' ' || line.back() == '\t'))
        line.remove_suffix(1);
    while (!line.empty() && (line.front() == ' ' || line.front() == '\t'))
        line.remove_prefix(1);
    return line;
}

} // namespace

bool NdjsonStream::take_line(std::string_view& line) noexcept {
    while (position_ < data_.size()) {
        const size_t newline = data_.find('\n', position_);
        const size_t end = (newline == std::string_view::npos) ? data_.size() : newline;
        const std::string_view candidate = trim(data_.substr(position_, end - position_));
        position_ = (newline == std::string_view::npos) ? data_.size() : newline + 1;
        ++line_number_;
        if (!candidate.empty()) { // blank lines are separators, not records
            line = candidate;
            return true;
        }
    }
    return false;
}

bool NdjsonStream::has_next() const noexcept {
    size_t probe = position_;
    while (probe < data_.size()) {
        const size_t newline = data_.find('\n', probe);
        const size_t end = (newline == std::string_view::npos) ? data_.size() : newline;
        if (!trim(data_.substr(probe, end - probe)).empty())
            return true;
        probe = (newline == std::string_view::npos) ? data_.size() : newline + 1;
    }
    return false;
}

Result<JsonValue> NdjsonStream::next() {
    std::string_view line;
    if (!take_line(line))
        return {Status::KeyNotFound}; // end of stream, by documented convention

    auto parsed = parse_json(line);
    if (!parsed.ok())
        return {parsed.status}; // ParseError, or DepthExceeded for a nested line
    return {Status::Ok, std::move(parsed.value)};
}

Result<std::vector<JsonValue>> NdjsonStream::parse_all(bool skip_errors, size_t* failed_line) {
    if (failed_line != nullptr)
        *failed_line = 0;

    std::vector<JsonValue> records;
    for (;;) {
        std::string_view line;
        if (!take_line(line))
            break;

        auto parsed = parse_json(line);
        if (parsed.ok()) {
            records.push_back(std::move(parsed.value));
            continue;
        }
        if (skip_errors)
            continue; // the caller opted in to losing this line
        if (failed_line != nullptr)
            *failed_line = line_number_;
        return {parsed.status, {}};
    }
    return {Status::Ok, std::move(records)};
}

} // namespace strata
