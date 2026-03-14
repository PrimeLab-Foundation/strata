#pragma once

/**
 * Fast double-to-ASCII for JSON serialization.
 *
 * Custom implementation based on Dragonbox/Schubfach algorithm,
 * optimized for ARM64 (uses __uint128_t for 64x64->128 multiply).
 *
 * Produces shortest round-trip representation, JSON-compatible:
 * - Integer-valued doubles get ".0" appended (e.g., 1.0)
 * - NaN/Inf are NOT handled here (caller must convert to "null")
 * - Negative zero is NOT handled here (caller should check)
 */

#include <cstdint>
#include <cstring>

namespace strata {
namespace util {

/**
 * Convert double to shortest decimal string.
 *
 * @param value Positive, finite, non-zero double
 * @param buffer Output buffer (at least 25 bytes)
 * @return Number of characters written
 */
int fast_dtoa(double value, char* buffer);

} // namespace util
} // namespace strata
