#pragma once

/**
 * Dragonbox-style double-to-decimal for JSON serialization.
 *
 * Uses shortest round-trip decimal representation (same guarantee as
 * Dragonbox/Ryu). Prefers std::to_chars when available (many implementations
 * use Dragonbox or Ryu); falls back to ryu_d2s otherwise.
 *
 * References:
 * - Dragonbox: https://github.com/jk-jeon/dragonbox
 * - Ryu: https://dl.acm.org/doi/10.1145/3296979.3192369
 */

namespace strata {
namespace util {

/**
 * Convert double to shortest decimal string (JSON-compatible).
 *
 * @param value Double to convert (caller must handle NaN/Inf → null)
 * @param buffer Output buffer (at least 25 bytes)
 * @return Number of characters written
 *
 * Output: fixed-point when -6 <= exponent < 21, else scientific.
 * Integer-valued floats within safe range (±2^53) are output compactly
 * without decimal point (e.g., 1.0 → "1", 42.0 → "42").
 */
int dragonbox_d2s(double value, char* buffer);

} // namespace util
} // namespace strata
