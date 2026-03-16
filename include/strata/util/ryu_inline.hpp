#pragma once

/**
 * @file ryu_inline.hpp
 * @brief Fully inlineable Ryu d2d computation + formatting.
 *
 * Include this header in hot serialization TUs to avoid cross-TU
 * function call overhead for float-to-string conversion.
 * The lookup tables (~10KB) are duplicated per including TU.
 */

#include <cmath>
#include <cstdint>
#include <cstring>

namespace strata {
namespace util {
namespace ryu_inline {

// ---------- IEEE 754 double constants ----------
static constexpr int MANTISSA_BITS = 52;
static constexpr int EXPONENT_BITS = 11;
static constexpr int BIAS = 1023;
static constexpr uint64_t MANTISSA_MASK = (1ULL << MANTISSA_BITS) - 1;
static constexpr int POW5_INV_BITCOUNT = 125;
static constexpr int POW5_BITCOUNT = 125;

// ---------- Utility functions ----------

static inline int log10Pow2(int e) {
    return static_cast<int>(
        (static_cast<uint64_t>(static_cast<uint32_t>(e)) * 169464822037455ULL) >> 49);
}

static inline int log10Pow5(int e) {
    return static_cast<int>(
        (static_cast<uint64_t>(static_cast<uint32_t>(e)) * 196742565691928ULL) >> 48);
}

static inline int pow5bits(int e) {
    return static_cast<int>((static_cast<uint64_t>(static_cast<uint32_t>(e)) * 1217359) >> 19) + 1;
}

static inline bool multipleOfPow5(uint64_t v, int p) {
    for (int i = 0; i < p; ++i) {
        if (v % 5 != 0)
            return false;
        v /= 5;
    }
    return true;
}

static inline bool multipleOfPow2(uint64_t v, int p) { return (v & ((1ULL << p) - 1)) == 0; }

// ---------- 128-bit multiplication ----------

static inline uint64_t mulShift64(uint64_t m, const uint64_t* mul, int j) {
    __uint128_t b0 = static_cast<__uint128_t>(m) * mul[0];
    __uint128_t b1 = static_cast<__uint128_t>(m) * mul[1];
    __uint128_t mid = (b0 >> 64) + b1;
    return static_cast<uint64_t>(mid >> (j - 64));
}

static inline uint64_t mulShiftAll64(uint64_t m, const uint64_t* mul, int j, uint64_t* vp,
                                     uint64_t* vm, uint32_t mmShift) {
    *vp = mulShift64(4 * m + 2, mul, j);
    *vm = mulShift64(4 * m - 1 - mmShift, mul, j);
    return mulShift64(4 * m, mul, j);
}

// ---------- Lookup tables (included inline) ----------

#include "strata/util/ryu_tables.inc"

// ---------- Power-of-10 lookup table (for avoiding div loop) ----------

static constexpr uint64_t POW10[18] = {
    1ULL,
    10ULL,
    100ULL,
    1000ULL,
    10000ULL,
    100000ULL,
    1000000ULL,
    10000000ULL,
    100000000ULL,
    1000000000ULL,
    10000000000ULL,
    100000000000ULL,
    1000000000000ULL,
    10000000000000ULL,
    100000000000000ULL,
    1000000000000000ULL,
    10000000000000000ULL,
    100000000000000000ULL,
};

// ---------- Digit-pair table ----------

static constexpr char DIGIT_PAIRS[201] = "00010203040506070809"
                                         "10111213141516171819"
                                         "20212223242526272829"
                                         "30313233343536373839"
                                         "40414243444546474849"
                                         "50515253545556575859"
                                         "60616263646566676869"
                                         "70717273747576777879"
                                         "80818283848586878889"
                                         "90919293949596979899";

// ---------- Core d2d ----------

struct Decimal {
    uint64_t mantissa;
    int32_t exponent;
};

static inline Decimal d2d(uint64_t ieeeMantissa, uint32_t ieeeExponent) {
    int32_t e2;
    uint64_t m2;
    if (ieeeExponent == 0) {
        e2 = 1 - BIAS - MANTISSA_BITS - 2;
        m2 = ieeeMantissa;
    } else {
        e2 = static_cast<int32_t>(ieeeExponent) - BIAS - MANTISSA_BITS - 2;
        m2 = (1ULL << MANTISSA_BITS) | ieeeMantissa;
    }

    bool even = (m2 & 1) == 0;
    bool acceptBounds = even;
    uint64_t mv = 4 * m2;
    uint32_t mmShift = (ieeeMantissa != 0 || ieeeExponent <= 1) ? 1 : 0;

    uint64_t vr, vp, vm;
    int32_t e10;
    bool vmTZ = false, vrTZ = false;

    if (e2 >= 0) {
        int32_t q = log10Pow2(e2) - (e2 > 3 ? 1 : 0);
        e10 = q;
        int32_t k = pow5bits(q) - 1 + POW5_INV_BITCOUNT;
        int32_t i = -e2 + q + k;
        vr = mulShiftAll64(m2, DOUBLE_POW5_INV_SPLIT[q], i, &vp, &vm, mmShift);
        if (q <= 21) {
            if (mv % 5 == 0) {
                vrTZ = multipleOfPow5(mv, q);
            } else if (acceptBounds) {
                vmTZ = multipleOfPow5(mv - 1 - mmShift, q);
            } else {
                vp -= multipleOfPow5(mv + 2, q) ? 1 : 0;
            }
        }
    } else {
        int32_t q = log10Pow5(-e2) - (-e2 > 1 ? 1 : 0);
        e10 = q + e2;
        int32_t i = -e2 - q;
        int32_t k = pow5bits(i) - POW5_BITCOUNT;
        int32_t j = q - k;
        vr = mulShiftAll64(m2, DOUBLE_POW5_SPLIT[i], j, &vp, &vm, mmShift);
        if (q <= 1) {
            vrTZ = true;
            if (acceptBounds)
                vmTZ = mmShift == 1;
            else
                --vp;
        } else if (q < 63) {
            vrTZ = multipleOfPow2(mv, q - 1);
        }
    }

    // Shortening
    int32_t removed = 0;
    uint64_t lastRemoved = 0;
    uint64_t output;

    if (vmTZ || vrTZ) {
        for (;;) {
            uint64_t vpD = vp / 10, vmD = vm / 10;
            if (vpD <= vmD)
                break;
            uint64_t vmM = vm - 10 * vmD;
            uint64_t vrD = vr / 10, vrM = vr - 10 * vrD;
            vmTZ &= (vmM == 0);
            vrTZ &= (lastRemoved == 0);
            lastRemoved = vrM;
            vr = vrD;
            vp = vpD;
            vm = vmD;
            ++removed;
        }
        if (vmTZ) {
            for (;;) {
                uint64_t vmD = vm / 10, vmM = vm - 10 * vmD;
                if (vmM != 0)
                    break;
                uint64_t vpD = vp / 10;
                uint64_t vrD = vr / 10, vrM = vr - 10 * vrD;
                vrTZ &= (lastRemoved == 0);
                lastRemoved = vrM;
                vr = vrD;
                vp = vpD;
                vm = vmD;
                ++removed;
            }
        }
        if (vrTZ && lastRemoved == 5 && vr % 2 == 0)
            lastRemoved = 4;
        output = vr + ((vr == vm && (!acceptBounds || !vmTZ)) || lastRemoved >= 5 ? 1 : 0);
    } else {
        bool roundUp = false;
        uint64_t vpD100 = vp / 100, vmD100 = vm / 100;
        if (vpD100 > vmD100) {
            uint64_t vrD100 = vr / 100, vrM100 = vr - 100 * vrD100;
            roundUp = vrM100 >= 50;
            vr = vrD100;
            vp = vpD100;
            vm = vmD100;
            removed += 2;
        }
        for (;;) {
            uint64_t vpD = vp / 10, vmD = vm / 10;
            if (vpD <= vmD)
                break;
            uint64_t vrD = vr / 10, vrM = vr - 10 * vrD;
            roundUp = vrM >= 5;
            vr = vrD;
            vp = vpD;
            vm = vmD;
            ++removed;
        }
        output = vr + (vr == vm || roundUp ? 1 : 0);
    }

    return {output, e10 + removed};
}

// ---------- Formatting ----------

static inline int decLen17(uint64_t v) {
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

static inline void writeDigits(uint64_t out, char* buf, int len) {
    int i = len - 1;
    while (i >= 1) {
        uint64_t q = out / 100;
        uint32_t r = static_cast<uint32_t>(out - 100 * q);
        out = q;
        memcpy(buf + i - 1, DIGIT_PAIRS + 2 * r, 2);
        i -= 2;
    }
    if (i == 0)
        buf[0] = '0' + static_cast<char>(out);
}

static inline char* writeExp(int32_t e, char* p) {
    if (e >= 100) {
        *p++ = '0' + static_cast<char>(e / 100);
        e %= 100;
    }
    memcpy(p, DIGIT_PAIRS + 2 * e, 2);
    return p + 2;
}

/**
 * Full inline d2d + format for a positive, finite, non-zero double.
 * Returns number of chars written to buffer.
 */
static inline int convert(double value, char* buffer) {
    uint64_t bits;
    memcpy(&bits, &value, sizeof(bits));
    uint64_t ieeeMant = bits & MANTISSA_MASK;
    uint32_t ieeeExp = static_cast<uint32_t>((bits >> MANTISSA_BITS) & ((1u << EXPONENT_BITS) - 1));

    Decimal dec = d2d(ieeeMant, ieeeExp);

    const uint64_t m = dec.mantissa;
    const int32_t exp = dec.exponent;
    const int ol = decLen17(m);
    const int32_t sciExp = ol + exp - 1;
    const int32_t dotPos = ol + exp;

    char* p = buffer;

    if (sciExp >= -4 && sciExp < 6) {
        if (dotPos <= 0) {
            *p++ = '0';
            *p++ = '.';
            for (int i = 0; i < -dotPos; ++i)
                *p++ = '0';
            writeDigits(m, p, ol);
            p += ol;
        } else if (dotPos >= ol) {
            writeDigits(m, p, ol);
            p += ol;
            for (int i = 0; i < dotPos - ol; ++i)
                *p++ = '0';
            *p++ = '.';
            *p++ = '0';
        } else {
            // Split mantissa around decimal point using lookup table.
            int fracLen = ol - dotPos;
            uint64_t divisor = POW10[fracLen];
            uint64_t intPart = m / divisor;
            uint64_t fracPart = m % divisor;
            writeDigits(intPart, p, dotPos);
            p += dotPos;
            *p++ = '.';
            writeDigits(fracPart, p, fracLen);
            p += fracLen;
        }
    } else {
        int32_t absExp = sciExp < 0 ? -sciExp : sciExp;
        if (ol == 1) {
            *p++ = '0' + static_cast<char>(m);
            *p++ = 'e';
            *p++ = sciExp >= 0 ? '+' : '-';
            p = writeExp(absExp, p);
        } else {
            int restLen = ol - 1;
            uint64_t divisor = POW10[restLen];
            *p++ = '0' + static_cast<char>(m / divisor);
            *p++ = '.';
            writeDigits(m % divisor, p, restLen);
            p += restLen;
            *p++ = 'e';
            *p++ = sciExp >= 0 ? '+' : '-';
            p = writeExp(absExp, p);
        }
    }

    return static_cast<int>(p - buffer);
}

} // namespace ryu_inline
} // namespace util
} // namespace strata
