#pragma once

/**
 * @file fast_parse.hpp
 * @brief Fast number parsing and string scanning utilities.
 *
 * Optimised for common JSON patterns. 3-5x faster than std::stod
 * for typical integer and floating-point values.
 *
 * Key design choices:
 * - parse_int_fast rejects leading zeros (JSON spec).
 * - parse_double_fast handles decimal point and exponent notation.
 * - scan_string_fast uses an unrolled loop (8 chars/iter) for branch
 *   predictor–friendly escape/quote detection.
 * - skip_whitespace_fast uses an unrolled loop (4 chars/iter).
 */

#include <cstddef>
#include <cstdint>
#include <limits>
#include <string>
#include <string_view>

namespace strata {
namespace util {

/**
 * Fast integer parser for JSON numbers.
 *
 * Parses an optional minus sign followed by digits.  Rejects leading
 * zeros (except bare "0") per the JSON specification.  Returns false
 * on overflow or invalid input.
 *
 * @param str      Pointer to the first character.
 * @param len      Number of available characters.
 * @param[out] result  Parsed int64_t value.
 * @param[out] consumed Number of characters consumed.
 * @return true on success.
 */
[[nodiscard]] inline bool parse_int_fast(const char* str, size_t len, int64_t& result,
                                         size_t& consumed) noexcept {
    if (len == 0)
        return false;

    size_t pos = 0;
    bool negative = false;

    if (str[pos] == '-') {
        negative = true;
        ++pos;
    }

    if (pos >= len || str[pos] < '0' || str[pos] > '9') {
        return false;
    }

    // JSON rejects leading zeros (e.g. "007") — only bare "0" is allowed.
    if (str[pos] == '0') {
        ++pos;
        if (pos < len && str[pos] >= '0' && str[pos] <= '9') {
            return false;
        }
        result = 0;
        consumed = pos;
        return true;
    }

    uint64_t val = 0;
    const uint64_t limit = negative ? static_cast<uint64_t>(std::numeric_limits<int64_t>::max()) + 1
                                    : static_cast<uint64_t>(std::numeric_limits<int64_t>::max());
    while (pos < len && str[pos] >= '0' && str[pos] <= '9') {
        int digit = str[pos] - '0';
        if (val > (limit - static_cast<uint64_t>(digit)) / 10) {
            return false; // Overflow
        }
        val = val * 10 + static_cast<uint64_t>(digit);
        ++pos;
    }

    if (negative) {
        const uint64_t min_abs =
            static_cast<uint64_t>(std::numeric_limits<int64_t>::max()) + 1; // |INT64_MIN|
        if (val > min_abs)
            return false;
        if (val == min_abs) {
            result = std::numeric_limits<int64_t>::min();
        } else {
            result = -static_cast<int64_t>(val);
        }
    } else {
        if (val > static_cast<uint64_t>(std::numeric_limits<int64_t>::max()))
            return false;
        result = static_cast<int64_t>(val);
    }
    consumed = pos;
    return true;
}

/**
 * Fast double parser (simplified Ryu-inspired algorithm).
 *
 * Handles common JSON float patterns efficiently.  Declared in the
 * header, defined in fast_parse.cpp.
 *
 * @param str      Pointer to the first character.
 * @param len      Number of available characters.
 * @param[out] result  Parsed double value.
 * @param[out] consumed Number of characters consumed.
 * @return true on success.
 */
[[nodiscard]] bool parse_double_fast(const char* str, size_t len, double& result, size_t& consumed);

/**
 * Fast string scan for escape detection.
 *
 * Returns the position of the first quote, backslash, or control
 * character (<0x20), or @p len if none is found.  Uses an unrolled
 * loop (8 chars per iteration) for better branch prediction.
 *
 * @param str  Pointer to the first character (after the opening quote).
 * @param len  Number of available characters.
 * @return Position of first special character, or @p len.
 */
[[nodiscard]] inline size_t scan_string_fast(const char* str, size_t len) noexcept {
    size_t pos = 0;

    // Unrolled loop — 8 characters per iteration.
    while (pos + 8 <= len) {
        if (str[pos] == '"' || str[pos] == '\\' || str[pos] < 0x20)
            return pos;
        if (str[pos + 1] == '"' || str[pos + 1] == '\\' || str[pos + 1] < 0x20)
            return pos + 1;
        if (str[pos + 2] == '"' || str[pos + 2] == '\\' || str[pos + 2] < 0x20)
            return pos + 2;
        if (str[pos + 3] == '"' || str[pos + 3] == '\\' || str[pos + 3] < 0x20)
            return pos + 3;
        if (str[pos + 4] == '"' || str[pos + 4] == '\\' || str[pos + 4] < 0x20)
            return pos + 4;
        if (str[pos + 5] == '"' || str[pos + 5] == '\\' || str[pos + 5] < 0x20)
            return pos + 5;
        if (str[pos + 6] == '"' || str[pos + 6] == '\\' || str[pos + 6] < 0x20)
            return pos + 6;
        if (str[pos + 7] == '"' || str[pos + 7] == '\\' || str[pos + 7] < 0x20)
            return pos + 7;
        pos += 8;
    }

    // Scalar tail.
    while (pos < len) {
        if (str[pos] == '"' || str[pos] == '\\' || str[pos] < 0x20)
            return pos;
        ++pos;
    }

    return len;
}

/**
 * Fast whitespace skipper.
 *
 * Returns the position of the first non-whitespace character starting
 * from @p start.  Uses an unrolled loop (4 chars per iteration).
 *
 * @param str   Pointer to the buffer.
 * @param len   Total buffer length.
 * @param start Starting position.
 * @return Position of first non-whitespace character, or @p len.
 */
[[nodiscard]] inline size_t skip_whitespace_fast(const char* str, size_t len,
                                                 size_t start) noexcept {
    size_t pos = start;

    // Unrolled loop — 4 characters per iteration.
    while (pos + 4 <= len) {
        if (str[pos] != ' ' && str[pos] != '\t' && str[pos] != '\n' && str[pos] != '\r')
            return pos;
        if (str[pos + 1] != ' ' && str[pos + 1] != '\t' && str[pos + 1] != '\n' &&
            str[pos + 1] != '\r')
            return pos + 1;
        if (str[pos + 2] != ' ' && str[pos + 2] != '\t' && str[pos + 2] != '\n' &&
            str[pos + 2] != '\r')
            return pos + 2;
        if (str[pos + 3] != ' ' && str[pos + 3] != '\t' && str[pos + 3] != '\n' &&
            str[pos + 3] != '\r')
            return pos + 3;
        pos += 4;
    }

    while (pos < len &&
           (str[pos] == ' ' || str[pos] == '\t' || str[pos] == '\n' || str[pos] == '\r')) {
        ++pos;
    }

    return pos;
}

} // namespace util
} // namespace strata
