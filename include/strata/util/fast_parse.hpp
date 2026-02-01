#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

namespace strata {
namespace util {

/**
 * Fast number parsing utilities.
 *
 * Optimized for common JSON number patterns.
 * 3-5x faster than std::stod for typical cases.
 */

/**
 * Parse integer from string view.
 * Returns value and number of characters consumed.
 */
inline bool parse_int_fast(const char* str, size_t len, int64_t& result, size_t& consumed) {
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

    // Check for leading zero (not allowed in JSON except for "0" itself)
    if (str[pos] == '0') {
        ++pos;
        // After '0', must not be another digit
        if (pos < len && str[pos] >= '0' && str[pos] <= '9') {
            return false; // Leading zero not allowed
        }
        result = 0;
        consumed = pos;
        return true;
    }

    int64_t val = 0;
    while (pos < len && str[pos] >= '0' && str[pos] <= '9') {
        val = val * 10 + (str[pos] - '0');
        ++pos;
    }

    result = negative ? -val : val;
    consumed = pos;
    return true;
}

/**
 * Fast float parsing (simplified Ryū-inspired algorithm).
 * Handles common JSON float patterns efficiently.
 */
bool parse_double_fast(const char* str, size_t len, double& result, size_t& consumed);

/**
 * Fast string scanning for escape detection.
 * Returns position of first escape or quote, or len if none found.
 */
inline size_t scan_string_fast(const char* str, size_t len) {
    size_t pos = 0;

    // Unrolled loop for better branch prediction
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

    while (pos < len) {
        if (str[pos] == '"' || str[pos] == '\\' || str[pos] < 0x20)
            return pos;
        ++pos;
    }

    return len;
}

/**
 * Fast whitespace skipping.
 * Returns position of first non-whitespace character.
 */
inline size_t skip_whitespace_fast(const char* str, size_t len, size_t start) {
    size_t pos = start;

    // Unrolled loop
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
