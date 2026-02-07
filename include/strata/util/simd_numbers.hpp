#pragma once

#include <cstddef>
#include <cstdint>

namespace strata {
namespace util {

/**
 * SIMD-accelerated digit scanning for JSON numbers.
 *
 * Returns the count of consecutive decimal digits starting at str.
 * Falls back to scalar on unsupported platforms.
 */
size_t count_digits_simd(const char* str, size_t len);

/**
 * SWAR-accelerated unsigned integer parsing.
 *
 * Parses an unsigned integer from str using SIMD Within A Register (SWAR)
 * techniques for parallel digit conversion. Fast paths exist for:
 * - 1-digit: direct subtraction
 * - 2-3 digits: inline multiply
 * - 4 digits: SWAR parallel conversion
 * - 5-16 digits: SWAR with combination
 * - 17-20 digits: SWAR with overflow checking
 *
 * @param str Input string containing digits
 * @param len Length of input string
 * @param result Output: parsed value (valid only if function returns true)
 * @param consumed Output: number of characters consumed
 * @return true on success, false on invalid input or overflow
 *
 * JSON compliance:
 * - Rejects leading zeros (except "0" itself)
 * - Detects overflow for values > UINT64_MAX
 */
bool parse_uint_simd(const char* str, size_t len, uint64_t& result, size_t& consumed);

/**
 * SWAR-accelerated signed integer parsing.
 *
 * Like parse_uint_simd but handles optional leading '-' sign.
 * Validates against INT64_MIN/INT64_MAX range.
 *
 * @param str Input string (may start with '-')
 * @param len Length of input string
 * @param result Output: parsed value (valid only if function returns true)
 * @param consumed Output: number of characters consumed (including sign)
 * @return true on success, false on invalid input or overflow
 */
bool parse_int_simd(const char* str, size_t len, int64_t& result, size_t& consumed);

} // namespace util
} // namespace strata
