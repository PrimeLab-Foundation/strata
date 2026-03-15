#pragma once

/**
 * @file fast_dtoa.hpp
 * @brief Fast double-to-ASCII for JSON serialisation.
 *
 * Custom implementation based on the Dragonbox/Schubfach algorithm,
 * optimised for ARM64 (uses __uint128_t for 64x64→128 multiply).
 *
 * Produces the shortest round-trip representation, JSON-compatible:
 * - Integer-valued doubles get ".0" appended (e.g. 1.0).
 * - NaN/Inf are NOT handled here (caller must convert to "null").
 * - Negative zero is NOT handled here (caller should check).
 */

#include <cstdint>
#include <cstring>

namespace strata {
namespace util {

/**
 * Convert a positive, finite, non-zero double to the shortest decimal string.
 *
 * @param value  Positive, finite, non-zero double.
 * @param buffer Output buffer (at least 25 bytes).
 * @return Number of characters written.
 */
[[nodiscard]] int fast_dtoa(double value, char* buffer);

} // namespace util
} // namespace strata
