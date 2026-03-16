#pragma once

/**
 * @file dragonbox.hpp
 * @brief Dragonbox-style double-to-decimal for JSON serialisation.
 *
 * Produces the shortest round-trip decimal representation (same guarantee
 * as Dragonbox/Ryu).
 *
 * Two-level API:
 * - dragonbox_d2s(): full conversion (handles sign, zero, NaN/Inf).
 * - dragonbox_d2d(): returns raw {mantissa, exponent} for callers that
 *   handle sign/special cases themselves (e.g. append_double).
 * - dragonbox_format(): inline formatter that writes mantissa+exponent
 *   to a buffer without memmove, using digit-pair lookup.
 *
 * References:
 * - Dragonbox: https://github.com/jk-jeon/dragonbox
 * - Ryu:       https://dl.acm.org/doi/10.1145/3296979.3192369
 */

#include <cstdint>
#include <cstring>

namespace strata {
namespace util {

/// Raw decimal result from Ryu's d2d computation.
struct DecimalResult {
    uint64_t mantissa;
    int32_t exponent;
};

/**
 * Convert double to shortest decimal string (JSON-compatible).
 *
 * @param value  Double to convert (caller must handle NaN/Inf → null).
 * @param buffer Output buffer (at least 25 bytes).
 * @return Number of characters written.
 *
 * Output format: fixed-point when -4 <= sciExp < 6, else scientific.
 * Always includes a decimal point for integer-valued floats (e.g. 1.0).
 */
[[nodiscard]] int dragonbox_d2s(double value, char* buffer);

/**
 * Core Ryu d2d: decompose a positive, finite, non-zero double into
 * shortest decimal mantissa and exponent.
 *
 * @param value  Must be > 0, finite, non-zero. Caller handles sign/special.
 * @return DecimalResult with mantissa and exponent such that
 *         value ≈ mantissa * 10^exponent.
 */
[[nodiscard]] DecimalResult dragonbox_d2d(double value);

// ---- Inline formatting (avoids function-call + memmove overhead) ----

namespace detail {

/// Digit-pair lookup table: "00", "01", ..., "99"
inline constexpr char DIGIT_PAIRS[201] = "00010203040506070809"
                                         "10111213141516171819"
                                         "20212223242526272829"
                                         "30313233343536373839"
                                         "40414243444546474849"
                                         "50515253545556575859"
                                         "60616263646566676869"
                                         "70717273747576777879"
                                         "80818283848586878889"
                                         "90919293949596979899";

inline int decimalLength17(uint64_t v) {
    // Binary search on thresholds for 1..17 digits
    if (v >= 100000000ULL) {
        if (v >= 10000000000000ULL) {
            if (v >= 1000000000000000ULL) {
                if (v >= 10000000000000000ULL)
                    return 17;
                return v >= 1000000000000000ULL ? 16 : 15;
            }
            return v >= 100000000000000ULL ? 15 : 14;
        }
        if (v >= 10000000000ULL) {
            if (v >= 1000000000000ULL)
                return 13;
            return v >= 100000000000ULL ? 12 : 11;
        }
        return v >= 1000000000ULL ? 10 : 9;
    }
    if (v >= 10000ULL) {
        if (v >= 1000000ULL)
            return v >= 10000000ULL ? 8 : 7;
        return v >= 100000ULL ? 6 : 5;
    }
    if (v >= 100ULL)
        return v >= 1000ULL ? 4 : 3;
    return v >= 10ULL ? 2 : 1;
}

/// Write digits of `output` into buf[0..olength-1] using digit-pair table.
inline void writeDigitsPairs(uint64_t output, char* buf, int olength) {
    int i = olength - 1;
    while (i >= 1) {
        uint64_t q = output / 100;
        uint32_t r = static_cast<uint32_t>(output - 100 * q);
        output = q;
        memcpy(buf + i - 1, DIGIT_PAIRS + 2 * r, 2);
        i -= 2;
    }
    if (i == 0) {
        buf[0] = '0' + static_cast<char>(output);
    }
}

/// Write scientific exponent (minimum 2 digits, e.g. "06", "12", "308").
inline char* writeExponent(int32_t exp, char* p) {
    if (exp >= 100) {
        *p++ = '0' + static_cast<char>(exp / 100);
        exp %= 100;
    }
    memcpy(p, DIGIT_PAIRS + 2 * exp, 2);
    return p + 2;
}

} // namespace detail

/**
 * Format a DecimalResult into a buffer. No memmove — writes digits
 * directly to correct positions around the decimal point.
 *
 * @param dec    DecimalResult from dragonbox_d2d.
 * @param buffer Output buffer (at least 25 bytes).
 * @return Number of characters written.
 */
[[nodiscard]] inline int dragonbox_format(DecimalResult dec, char* buffer) {
    const uint64_t mantissa = dec.mantissa;
    const int32_t exponent = dec.exponent;
    const int olength = detail::decimalLength17(mantissa);
    const int32_t sciExp = olength + exponent - 1;
    const int32_t dotPos = olength + exponent; // digits before decimal point

    char* p = buffer;

    if (sciExp >= -4 && sciExp < 6) {
        // Fixed-point notation
        if (dotPos <= 0) {
            // 0.000...ddd form
            *p++ = '0';
            *p++ = '.';
            for (int i = 0; i < -dotPos; ++i)
                *p++ = '0';
            detail::writeDigitsPairs(mantissa, p, olength);
            p += olength;
        } else if (dotPos >= olength) {
            // All digits before decimal point + ".0"
            detail::writeDigitsPairs(mantissa, p, olength);
            p += olength;
            for (int i = 0; i < dotPos - olength; ++i)
                *p++ = '0';
            *p++ = '.';
            *p++ = '0';
        } else {
            // Decimal point falls within the digits.
            // Write integer part and fractional part separately — no memmove.
            // Split mantissa: intPart = mantissa / 10^(olength - dotPos)
            //                 fracPart = mantissa % 10^(olength - dotPos)
            int fracLen = olength - dotPos;
            uint64_t divisor = 1;
            for (int i = 0; i < fracLen; ++i)
                divisor *= 10;
            uint64_t intPart = mantissa / divisor;
            uint64_t fracPart = mantissa % divisor;

            // Write integer part
            int intLen = dotPos;
            detail::writeDigitsPairs(intPart, p, intLen);
            p += intLen;
            *p++ = '.';
            // Write fractional part (must be zero-padded to fracLen digits)
            detail::writeDigitsPairs(fracPart, p, fracLen);
            p += fracLen;
        }
    } else {
        // Scientific notation
        int32_t absExp = sciExp < 0 ? -sciExp : sciExp;

        if (olength == 1) {
            *p++ = '0' + static_cast<char>(mantissa);
            *p++ = 'e';
            *p++ = sciExp >= 0 ? '+' : '-';
            p = detail::writeExponent(absExp, p);
        } else {
            // Write first digit, '.', then remaining digits — no memmove.
            // Split: first digit = mantissa / 10^(olength-1)
            //        rest = mantissa % 10^(olength-1)
            int restLen = olength - 1;
            uint64_t divisor = 1;
            for (int i = 0; i < restLen; ++i)
                divisor *= 10;
            uint64_t first = mantissa / divisor;
            uint64_t rest = mantissa % divisor;

            *p++ = '0' + static_cast<char>(first);
            *p++ = '.';
            detail::writeDigitsPairs(rest, p, restLen);
            p += restLen;
            *p++ = 'e';
            *p++ = sciExp >= 0 ? '+' : '-';
            p = detail::writeExponent(absExp, p);
        }
    }

    return static_cast<int>(p - buffer);
}

} // namespace util
} // namespace strata
