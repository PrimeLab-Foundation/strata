#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>
#include <string>
#include <string_view>

#include "strata/util/simd_numbers.hpp"

namespace strata {
namespace util {

/**
 * Fast number parsing utilities.
 *
 * Optimized for common JSON number patterns.
 * Uses SWAR (SIMD Within A Register) techniques for parallel digit parsing.
 * 3-5x faster than std::stod for typical cases.
 */

/**
 * Parse integer from string view using SWAR-accelerated parsing.
 * Returns value and number of characters consumed.
 *
 * Fast paths for common cases:
 * - 1-digit: direct subtraction (~2 cycles)
 * - 2-3 digits: inline multiply (~4 cycles)
 * - 4 digits: SWAR parallel conversion (~6 cycles)
 * - 5-8 digits: SWAR + scalar remainder (~10 cycles)
 * - 9-16 digits: dual SWAR (~12 cycles)
 *
 * Delegates to parse_int_simd for the actual work.
 */
inline bool parse_int_fast(const char* str, size_t len, int64_t& result, size_t& consumed) {
    return parse_int_simd(str, len, result, consumed);
}

/**
 * Parse unsigned integer from string view using SWAR-accelerated parsing.
 * Returns value and number of characters consumed.
 *
 * Delegates to parse_uint_simd for the actual work.
 */
inline bool parse_uint_fast(const char* str, size_t len, uint64_t& result, size_t& consumed) {
    return parse_uint_simd(str, len, result, consumed);
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
