#pragma once

/**
 * Ryū algorithm for fast double-to-ascii conversion.
 *
 * Based on Ulf Adams' Ryū algorithm:
 * https://dl.acm.org/doi/10.1145/3296979.3192369
 *
 * Simplified implementation optimized for JSON serialization.
 * Significantly faster than snprintf/dtoa for floating-point conversion.
 */

#include <cstdint>
#include <cstring>

namespace strata {
namespace util {

/**
 * Convert double to string using Ryū algorithm.
 *
 * @param value Double value to convert
 * @param buffer Output buffer (must be at least 25 bytes)
 * @return Number of characters written
 *
 * Special cases:
 * - NaN and Inf are handled by caller (converted to null)
 * - Denormals and zero are handled correctly
 * - Scientific notation used when appropriate
 */
int ryu_d2s(double value, char* buffer);

/**
 * Fast path for common case: convert double to shortest representation.
 * Uses fixed-point when possible, scientific notation when needed.
 */
int ryu_d2s_buffered(double value, char* result);

} // namespace util
} // namespace strata
