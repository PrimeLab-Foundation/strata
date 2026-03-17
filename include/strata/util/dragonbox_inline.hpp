#pragma once

/**
 * @file dragonbox_inline.hpp
 * @brief Fully inlineable Dragonbox d2d computation + formatting.
 *
 * Replaces ryu_inline.hpp with the real Dragonbox algorithm
 * (Junekey Jeon, 2020), which is 15-20% faster than Ryu for the
 * general case while providing the same shortest round-trip guarantee.
 *
 * Include this header in hot serialization TUs to avoid cross-TU
 * function call overhead for float-to-string conversion.
 */

#include "strata/third_party/dragonbox/dragonbox.h"

#include <cmath>
#include <cstdint>
#include <cstring>

namespace strata {
namespace util {
namespace dragonbox_inline {

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

// Write digits for a 32-bit value (up to 9 digits). 32-bit div is ~3x faster
// than 64-bit div on ARM.
static inline void writeDigits32(uint32_t out, char* buf, int len) {
    int i = len - 1;
    while (i >= 1) {
        uint32_t q = out / 100;
        uint32_t r = out - 100 * q;
        out = q;
        memcpy(buf + i - 1, DIGIT_PAIRS + 2 * r, 2);
        i -= 2;
    }
    if (i == 0)
        buf[0] = '0' + static_cast<char>(out);
}

static inline void writeDigits(uint64_t out, char* buf, int len) {
    // For values that fit in 32 bits, use faster 32-bit division.
    if (len <= 9) {
        writeDigits32(static_cast<uint32_t>(out), buf, len);
        return;
    }
    // Split into two halves: lo = bottom 8 digits, hi = remaining top digits.
    // Each half uses fast 32-bit division.
    static constexpr uint64_t kE8 = 100000000ULL;
    uint32_t lo = static_cast<uint32_t>(out % kE8);
    uint32_t hi = static_cast<uint32_t>(out / kE8);
    writeDigits32(lo, buf + len - 8, 8);
    writeDigits32(hi, buf, len - 8);
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
 * Full inline Dragonbox d2d + format for a positive, finite, non-zero double.
 * Returns number of chars written to buffer.
 *
 * @param value  Must be > 0, finite, non-zero. Caller handles sign/special.
 * @param buffer Output buffer (at least 25 bytes).
 * @return Number of characters written.
 */
static inline int convert(double value, char* buffer) {
    // Use the real Dragonbox algorithm for d2d conversion.
    // policy::sign::ignore — caller guarantees positive value.
    // policy::trailing_zero::remove — produce shortest representation.
    auto result = jkj::dragonbox::to_decimal(value, jkj::dragonbox::policy::sign::ignore,
                                             jkj::dragonbox::policy::trailing_zero::remove);

    const uint64_t m = static_cast<uint64_t>(result.significand);
    const int32_t exp = static_cast<int32_t>(result.exponent);
    const int ol = decLen17(m);
    const int32_t sciExp = ol + exp - 1;
    const int32_t dotPos = ol + exp;

    char* p = buffer;

    if (sciExp >= -4 && sciExp < 6) {
        if (dotPos == 0) {
            // Most common case for 0.xxx values (e.g. random.random()) —
            // no leading zeros, just "0." + all digits.
            *p++ = '0';
            *p++ = '.';
            writeDigits(m, p, ol);
            p += ol;
        } else if (dotPos < 0) {
            // 0.000...ddd form (dotPos is -1, -2, -3, -4)
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

} // namespace dragonbox_inline
} // namespace util
} // namespace strata
