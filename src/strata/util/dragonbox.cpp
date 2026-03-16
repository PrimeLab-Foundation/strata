#include "strata/util/dragonbox.hpp"

#include "strata/util/ryu_dtoa.hpp"

#include <cmath>
#include <cstdint>
#include <cstring>

// ============================================================================
// Ryu double-to-string implementation
//
// Based on Ulf Adams' Ryu algorithm (PLDI 2018).
// Produces shortest round-trip decimal representation identical to
// std::to_chars(p, p+32, value, std::chars_format::general).
//
// Uses __uint128_t for 128-bit multiplication (GCC/Clang).
// Lookup tables are generated from the reference Ryu implementation.
// ============================================================================

namespace {

// ---------- IEEE 754 double constants ----------
static constexpr int DOUBLE_MANTISSA_BITS = 52;
static constexpr int DOUBLE_EXPONENT_BITS = 11;
static constexpr int DOUBLE_BIAS = 1023;
static constexpr uint64_t DOUBLE_MANTISSA_MASK = (1ULL << DOUBLE_MANTISSA_BITS) - 1;

// Ryu uses DOUBLE_POW5_INV_BITCOUNT = 125, DOUBLE_POW5_BITCOUNT = 125
static constexpr int DOUBLE_POW5_INV_BITCOUNT = 125;
static constexpr int DOUBLE_POW5_BITCOUNT = 125;

// ---------- Utility functions ----------

static inline int log10Pow2(int e) {
    // floor(e * log10(2)), valid for e in [0, 1650]
    return static_cast<int>(
        (static_cast<uint64_t>(static_cast<uint32_t>(e)) * 169464822037455ULL) >> 49);
}

static inline int log10Pow5(int e) {
    // floor(e * log10(5)), valid for e in [0, 2620]
    return static_cast<int>(
        (static_cast<uint64_t>(static_cast<uint32_t>(e)) * 196742565691928ULL) >> 48);
}

static inline int pow5bits(int e) {
    // ceil(log2(5^e)) = floor(e * log2(5)) + 1
    return static_cast<int>((static_cast<uint64_t>(static_cast<uint32_t>(e)) * 1217359) >> 19) + 1;
}

static inline bool multipleOfPowerOf5(uint64_t value, int p) {
    for (int i = 0; i < p; ++i) {
        if (value % 5 != 0)
            return false;
        value /= 5;
    }
    return true;
}

static inline bool multipleOfPowerOf2(uint64_t value, int p) {
    return (value & ((1ULL << p) - 1)) == 0;
}

// ---------- 128-bit multiplication ----------

// mulShift64: compute (m * mul) >> j where mul is a 128-bit number stored as [lo, hi]
// m is at most 55 bits, mul is 128 bits, we compute bits [j, j+64) of the 192-bit product.
static inline uint64_t mulShift64(uint64_t m, const uint64_t* mul, int j) {
    __uint128_t b0 = static_cast<__uint128_t>(m) * mul[0]; // low part
    __uint128_t b1 = static_cast<__uint128_t>(m) * mul[1]; // high part
    // b0 contributes to bits [0, 128), b1 contributes to bits [64, 192)
    // We want bits [j, j+64) of the 192-bit result
    // The "mid" 128-bit value from bits [64, 192) is: (b0 >> 64) + b1
    __uint128_t mid = (b0 >> 64) + b1;
    return static_cast<uint64_t>(mid >> (j - 64));
}

static inline uint64_t mulShiftAll64(uint64_t m, const uint64_t* mul, int j, uint64_t* vp,
                                     uint64_t* vm, uint32_t mmShift) {
    *vp = mulShift64(4 * m + 2, mul, j);
    *vm = mulShift64(4 * m - 1 - mmShift, mul, j);
    return mulShift64(4 * m, mul, j);
}

// ---------- Power of 5 lookup tables ----------
// From the Ryu reference implementation (d2s_full_table.h)
// DOUBLE_POW5_INV_SPLIT[i] = ceil(2^(pow5bits(i) - 1 + DOUBLE_POW5_INV_BITCOUNT) / 5^i)
// DOUBLE_POW5_SPLIT[i] = 5^i * 2^(DOUBLE_POW5_BITCOUNT - pow5bits(i) + 1 - 1) (not exact, see Ryu
// paper)
//
// We generate these via __uint128_t computation at init time, cached in static arrays.
// This avoids shipping ~10KB of hardcoded tables while being computed once at startup.

// Precomputed lookup tables for the Ryu algorithm (generated from 5^i).
// DOUBLE_POW5_SPLIT[i] = floor(5^i / 2^max(0, pow5bits(i) - 125))
// DOUBLE_POW5_INV_SPLIT[i] = ceil(2^(pow5bits(i) + 124) / 5^i)
// Each entry is stored as {lo, hi} where value = hi * 2^64 + lo.

#include "ryu_tables.inc"

// ---------- Ryu core: d2d ----------

struct DecimalDouble {
    uint64_t mantissa;
    int32_t exponent;
};

static DecimalDouble d2d(uint64_t ieeeMantissa, uint32_t ieeeExponent) {
    int32_t e2;
    uint64_t m2;
    if (ieeeExponent == 0) {
        e2 = 1 - DOUBLE_BIAS - DOUBLE_MANTISSA_BITS - 2;
        m2 = ieeeMantissa;
    } else {
        e2 = static_cast<int32_t>(ieeeExponent) - DOUBLE_BIAS - DOUBLE_MANTISSA_BITS - 2;
        m2 = (1ULL << DOUBLE_MANTISSA_BITS) | ieeeMantissa;
    }

    bool even = (m2 & 1) == 0;
    bool acceptBounds = even;

    uint64_t mv = 4 * m2;
    uint32_t mmShift = (ieeeMantissa != 0 || ieeeExponent <= 1) ? 1 : 0;

    uint64_t vr, vp, vm;
    int32_t e10;
    bool vmIsTrailingZeros = false;
    bool vrIsTrailingZeros = false;

    if (e2 >= 0) {
        int32_t q = log10Pow2(e2) - (e2 > 3 ? 1 : 0);
        e10 = q;
        int32_t k = pow5bits(q) - 1 + DOUBLE_POW5_INV_BITCOUNT;
        int32_t i = -e2 + q + k;
        vr = mulShiftAll64(m2, DOUBLE_POW5_INV_SPLIT[q], i, &vp, &vm, mmShift);

        if (q <= 21) {
            if (mv % 5 == 0) {
                vrIsTrailingZeros = multipleOfPowerOf5(mv, q);
            } else if (acceptBounds) {
                vmIsTrailingZeros = multipleOfPowerOf5(mv - 1 - mmShift, q);
            } else {
                vp -= multipleOfPowerOf5(mv + 2, q) ? 1 : 0;
            }
        }
    } else {
        int32_t q = log10Pow5(-e2) - (-e2 > 1 ? 1 : 0);
        e10 = q + e2;
        int32_t i = -e2 - q;
        int32_t k = pow5bits(i) - DOUBLE_POW5_BITCOUNT;
        int32_t j = q - k;
        vr = mulShiftAll64(m2, DOUBLE_POW5_SPLIT[i], j, &vp, &vm, mmShift);

        if (q <= 1) {
            vrIsTrailingZeros = true;
            if (acceptBounds) {
                vmIsTrailingZeros = mmShift == 1;
            } else {
                --vp;
            }
        } else if (q < 63) {
            vrIsTrailingZeros = multipleOfPowerOf2(mv, q - 1);
        }
    }

    // Step 4: Find shortest representation
    int32_t removed = 0;
    uint64_t lastRemovedDigit = 0;
    uint64_t output;

    if (vmIsTrailingZeros || vrIsTrailingZeros) {
        for (;;) {
            uint64_t vpDiv10 = vp / 10;
            uint64_t vmDiv10 = vm / 10;
            if (vpDiv10 <= vmDiv10)
                break;
            uint64_t vmMod10 = vm - 10 * vmDiv10;
            uint64_t vrDiv10 = vr / 10;
            uint64_t vrMod10 = vr - 10 * vrDiv10;
            vmIsTrailingZeros &= (vmMod10 == 0);
            vrIsTrailingZeros &= (lastRemovedDigit == 0);
            lastRemovedDigit = vrMod10;
            vr = vrDiv10;
            vp = vpDiv10;
            vm = vmDiv10;
            ++removed;
        }
        if (vmIsTrailingZeros) {
            for (;;) {
                uint64_t vmDiv10 = vm / 10;
                uint64_t vmMod10 = vm - 10 * vmDiv10;
                if (vmMod10 != 0)
                    break;
                uint64_t vpDiv10 = vp / 10;
                uint64_t vrDiv10 = vr / 10;
                uint64_t vrMod10 = vr - 10 * vrDiv10;
                vrIsTrailingZeros &= (lastRemovedDigit == 0);
                lastRemovedDigit = vrMod10;
                vr = vrDiv10;
                vp = vpDiv10;
                vm = vmDiv10;
                ++removed;
            }
        }
        if (vrIsTrailingZeros && lastRemovedDigit == 5 && vr % 2 == 0) {
            lastRemovedDigit = 4;
        }
        output =
            vr +
            ((vr == vm && (!acceptBounds || !vmIsTrailingZeros)) || lastRemovedDigit >= 5 ? 1 : 0);
    } else {
        // Common case: no trailing zeros.
        bool roundUp = false;
        uint64_t vpDiv100 = vp / 100;
        uint64_t vmDiv100 = vm / 100;
        if (vpDiv100 > vmDiv100) {
            uint64_t vrDiv100 = vr / 100;
            uint64_t vrMod100 = vr - 100 * vrDiv100;
            roundUp = vrMod100 >= 50;
            vr = vrDiv100;
            vp = vpDiv100;
            vm = vmDiv100;
            removed += 2;
        }
        for (;;) {
            uint64_t vpDiv10 = vp / 10;
            uint64_t vmDiv10 = vm / 10;
            if (vpDiv10 <= vmDiv10)
                break;
            uint64_t vrDiv10 = vr / 10;
            uint64_t vrMod10 = vr - 10 * vrDiv10;
            roundUp = vrMod10 >= 5;
            vr = vrDiv10;
            vp = vpDiv10;
            vm = vmDiv10;
            ++removed;
        }
        output = vr + (vr == vm || roundUp ? 1 : 0);
    }

    DecimalDouble result;
    result.mantissa = output;
    result.exponent = e10 + removed;
    return result;
}

// ---------- Output formatting ----------

static inline int decimalLength17(uint64_t v) {
    if (v >= 10000000000000000ULL)
        return 17;
    if (v >= 1000000000000000ULL)
        return 16;
    if (v >= 100000000000000ULL)
        return 15;
    if (v >= 10000000000000ULL)
        return 14;
    if (v >= 1000000000000ULL)
        return 13;
    if (v >= 100000000000ULL)
        return 12;
    if (v >= 10000000000ULL)
        return 11;
    if (v >= 1000000000ULL)
        return 10;
    if (v >= 100000000ULL)
        return 9;
    if (v >= 10000000ULL)
        return 8;
    if (v >= 1000000ULL)
        return 7;
    if (v >= 100000ULL)
        return 6;
    if (v >= 10000ULL)
        return 5;
    if (v >= 1000ULL)
        return 4;
    if (v >= 100ULL)
        return 3;
    if (v >= 10ULL)
        return 2;
    return 1;
}

// Write decimal digits of output into buf[0..olength-1]
static inline void writeDigits(uint64_t output, char* buf, int olength) {
    int i = olength - 1;
    // Write two digits at a time for speed
    while (i >= 1) {
        uint64_t q = output / 100;
        uint32_t r = static_cast<uint32_t>(output - 100 * q);
        output = q;
        buf[i] = '0' + static_cast<char>(r % 10);
        buf[i - 1] = '0' + static_cast<char>(r / 10);
        i -= 2;
    }
    if (i == 0) {
        buf[0] = '0' + static_cast<char>(output);
    }
}

// Write the scientific exponent (after 'e+' or 'e-')
// std::to_chars pads exponent to minimum 2 digits (e.g., "e+06", "e-05")
static inline char* writeExponent(int32_t exp, char* p) {
    if (exp >= 100) {
        *p++ = '0' + static_cast<char>(exp / 100);
        exp %= 100;
        *p++ = '0' + static_cast<char>(exp / 10);
        *p++ = '0' + static_cast<char>(exp % 10);
    } else {
        *p++ = '0' + static_cast<char>(exp / 10);
        *p++ = '0' + static_cast<char>(exp % 10);
    }
    return p;
}

// Format using fixed-point or scientific notation.
// Matches std::to_chars(first, last, value, chars_format::general) on Apple libc++.
//
// The formatting rule is: use the %g convention with precision = 6 for format selection,
// but output ALL significant digits from the shortest representation.
// Specifically: let X = sciExp = olength + exponent - 1.
// Use fixed if -4 <= X < 6 (i.e., X in {-4, -3, ..., 5}).
// Use scientific otherwise.
// In scientific notation, exponents are zero-padded to minimum 2 digits (e.g., "e+06").
// In fixed notation, trailing fractional zeros are omitted.
// We always add ".0" for integer values (handled by this function).

static int formatResult(uint64_t mantissa, int32_t exponent, char* buffer) {
    int olength = decimalLength17(mantissa);
    int32_t sciExp = olength + exponent - 1;
    int32_t dotPos = olength + exponent; // digits before decimal point

    char* p = buffer;

    if (sciExp >= -4 && sciExp < 6) {
        // Fixed-point notation
        if (dotPos <= 0) {
            // 0.000...ddd form
            *p++ = '0';
            *p++ = '.';
            for (int i = 0; i < -dotPos; ++i) {
                *p++ = '0';
            }
            writeDigits(mantissa, p, olength);
            p += olength;
        } else if (dotPos >= olength) {
            // All digits before decimal point, possibly with trailing zeros
            writeDigits(mantissa, p, olength);
            p += olength;
            for (int i = 0; i < dotPos - olength; ++i) {
                *p++ = '0';
            }
            *p++ = '.';
            *p++ = '0';
        } else {
            // Decimal point falls within the digits
            writeDigits(mantissa, p, olength);
            memmove(p + dotPos + 1, p + dotPos, olength - dotPos);
            p[dotPos] = '.';
            p += olength + 1;
        }
    } else {
        // Scientific notation
        int32_t absExp = sciExp < 0 ? -sciExp : sciExp;

        if (olength == 1) {
            *p++ = '0' + static_cast<char>(mantissa);
            *p++ = 'e';
            *p++ = sciExp >= 0 ? '+' : '-';
            p = writeExponent(absExp, p);
        } else {
            writeDigits(mantissa, p, olength);
            memmove(p + 2, p + 1, olength - 1);
            p[1] = '.';
            p += olength + 1;
            *p++ = 'e';
            *p++ = sciExp >= 0 ? '+' : '-';
            p = writeExponent(absExp, p);
        }
    }

    return static_cast<int>(p - buffer);
}

} // anonymous namespace

namespace strata {
namespace util {

DecimalResult dragonbox_d2d(double value) {
    // Caller guarantees: value > 0, finite, non-zero.
    uint64_t bits;
    memcpy(&bits, &value, sizeof(bits));
    uint64_t ieeeMantissa = bits & DOUBLE_MANTISSA_MASK;
    uint32_t ieeeExponent =
        static_cast<uint32_t>((bits >> DOUBLE_MANTISSA_BITS) & ((1u << DOUBLE_EXPONENT_BITS) - 1));

    DecimalDouble dec = d2d(ieeeMantissa, ieeeExponent);
    return {dec.mantissa, dec.exponent};
}

int dragonbox_d2s(double value, char* buffer) {
    // Handle zero
    if (value == 0.0) {
        if (std::signbit(value)) {
            buffer[0] = '-';
            buffer[1] = '0';
            buffer[2] = '.';
            buffer[3] = '0';
            return 4;
        }
        buffer[0] = '0';
        buffer[1] = '.';
        buffer[2] = '0';
        return 3;
    }

    // Handle NaN/Inf (caller should handle, but be safe)
    if (__builtin_expect(std::isnan(value) || std::isinf(value), 0)) {
        memcpy(buffer, "null", 4);
        return 4;
    }

    char* p = buffer;

    // Handle sign
    if (value < 0) {
        *p++ = '-';
        value = -value;
    }

    // Use d2d + inline formatter
    DecimalResult dec = dragonbox_d2d(value);
    int len = dragonbox_format(dec, p);
    return static_cast<int>(p - buffer) + len;
}

} // namespace util
} // namespace strata
