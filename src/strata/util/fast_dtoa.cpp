/**
 * Fast double-to-ASCII using the Ryu algorithm by Ulf Adams.
 *
 * This is a self-contained C++ port of the Ryu algorithm for shortest-
 * representation double-to-decimal conversion, adapted for JSON output.
 *
 * Original: https://github.com/ulfjack/ryu
 * License: Apache 2.0 / Boost 1.0 (dual-licensed)
 *
 * Changes from original:
 * - C++ port (namespaced, no global headers)
 * - JSON-compatible output format (plain decimal, not scientific for small values)
 * - Uses __uint128_t on ARM64 for optimal performance
 * - Compressed (small) lookup table variant
 */

#include "strata/util/fast_dtoa.hpp"

#include <cstring>

namespace strata {
namespace util {

// ============================================================================
// Ryu lookup tables (compressed variant)
// ============================================================================

#define DOUBLE_POW5_INV_BITCOUNT 125
#define DOUBLE_POW5_BITCOUNT 125
#define POW5_TABLE_SIZE 26

typedef __uint128_t uint128_t;

static const uint64_t DOUBLE_POW5_INV_SPLIT2[15][2] = {
    {1u, 2305843009213693952u},
    {5955668970331000884u, 1784059615882449851u},
    {8982663654677661702u, 1380349269358112757u},
    {7286864317269821294u, 2135987035920910082u},
    {7005857020398200553u, 1652639921975621497u},
    {17965325103354776697u, 1278668206209430417u},
    {8928596168509315048u, 1978643211784836272u},
    {10075671573058298858u, 1530901034580419511u},
    {597001226353042382u, 1184477304306571148u},
    {1527430471115325346u, 1832889850782397517u},
    {12533209867169019542u, 1418129833677084982u},
    {5577825024675947042u, 2194449627517475473u},
    {11006974540203867551u, 1697873161311732311u},
    {10313493231639821582u, 1313665730009899186u},
    {12701016819766672773u, 2032799256770390445u},
};

static const uint32_t POW5_INV_OFFSETS[22] = {
    0x54544554, 0x04055545, 0x10041000, 0x00400414, 0x40010000, 0x41155555, 0x00000454, 0x00010044,
    0x40000000, 0x44000041, 0x50454450, 0x55550054, 0x51655554, 0x40004000, 0x01000001, 0x00010500,
    0x51515411, 0x05555554, 0x50411500, 0x40040000, 0x05040110, 0x00000000};

static const uint64_t DOUBLE_POW5_SPLIT2[13][2] = {
    {0u, 1152921504606846976u},
    {0u, 1490116119384765625u},
    {1032610780636961552u, 1925929944387235853u},
    {7910200175544436838u, 1244603055572228341u},
    {16941905809032713930u, 1608611746708759036u},
    {13024893955298202172u, 2079081953128979843u},
    {6607496772837067824u, 1343575221513417750u},
    {17332926989895652603u, 1736530273035216783u},
    {13037379183483547984u, 2244412773384604712u},
    {1605989338741628675u, 1450417759929778918u},
    {9630225068416591280u, 1874621017369538693u},
    {665883850346957067u, 1211445438634777304u},
    {14931890668723713708u, 1565756531257009982u},
};

static const uint32_t POW5_OFFSETS[21] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x40000000, 0x59695995, 0x55545555,
    0x56555515, 0x41150504, 0x40555410, 0x44555145, 0x44504540, 0x45555550, 0x40004000,
    0x96440440, 0x55565565, 0x54454045, 0x40154151, 0x55559155, 0x51405555, 0x00000105};

static const uint64_t DOUBLE_POW5_TABLE[POW5_TABLE_SIZE] = {1ull,
                                                            5ull,
                                                            25ull,
                                                            125ull,
                                                            625ull,
                                                            3125ull,
                                                            15625ull,
                                                            78125ull,
                                                            390625ull,
                                                            1953125ull,
                                                            9765625ull,
                                                            48828125ull,
                                                            244140625ull,
                                                            1220703125ull,
                                                            6103515625ull,
                                                            30517578125ull,
                                                            152587890625ull,
                                                            762939453125ull,
                                                            3814697265625ull,
                                                            19073486328125ull,
                                                            95367431640625ull,
                                                            476837158203125ull,
                                                            2384185791015625ull,
                                                            11920928955078125ull,
                                                            59604644775390625ull,
                                                            298023223876953125ull};

// Digit pair table
static const char DIGIT_TABLE[200] = {
    '0', '0', '0', '1', '0', '2', '0', '3', '0', '4', '0', '5', '0', '6', '0', '7', '0', '8', '0',
    '9', '1', '0', '1', '1', '1', '2', '1', '3', '1', '4', '1', '5', '1', '6', '1', '7', '1', '8',
    '1', '9', '2', '0', '2', '1', '2', '2', '2', '3', '2', '4', '2', '5', '2', '6', '2', '7', '2',
    '8', '2', '9', '3', '0', '3', '1', '3', '2', '3', '3', '3', '4', '3', '5', '3', '6', '3', '7',
    '3', '8', '3', '9', '4', '0', '4', '1', '4', '2', '4', '3', '4', '4', '4', '5', '4', '6', '4',
    '7', '4', '8', '4', '9', '5', '0', '5', '1', '5', '2', '5', '3', '5', '4', '5', '5', '5', '6',
    '5', '7', '5', '8', '5', '9', '6', '0', '6', '1', '6', '2', '6', '3', '6', '4', '6', '5', '6',
    '6', '6', '7', '6', '8', '6', '9', '7', '0', '7', '1', '7', '2', '7', '3', '7', '4', '7', '5',
    '7', '6', '7', '7', '7', '8', '7', '9', '8', '0', '8', '1', '8', '2', '8', '3', '8', '4', '8',
    '5', '8', '6', '8', '7', '8', '8', '8', '9', '9', '0', '9', '1', '9', '2', '9', '3', '9', '4',
    '9', '5', '9', '6', '9', '7', '9', '8', '9', '9'};

// ============================================================================
// Ryu helper functions
// ============================================================================

static inline int32_t pow5bits(int32_t e) {
    return static_cast<int32_t>(((static_cast<uint32_t>(e) * 1217359u) >> 19) + 1);
}

static inline uint32_t log10Pow2(int32_t e) { return (static_cast<uint32_t>(e) * 78913u) >> 18; }

static inline uint32_t log10Pow5(int32_t e) { return (static_cast<uint32_t>(e) * 732923u) >> 20; }

static inline uint64_t umulh(uint64_t a, uint64_t b) {
    return static_cast<uint64_t>((static_cast<uint128_t>(a) * b) >> 64);
}

static inline uint64_t ryu_div5(uint64_t x) { return umulh(x, 0xCCCCCCCCCCCCCCCDu) >> 2; }

static inline uint64_t ryu_div10(uint64_t x) { return umulh(x, 0xCCCCCCCCCCCCCCCDu) >> 3; }

static inline uint64_t ryu_div100(uint64_t x) { return umulh(x >> 2, 0x28F5C28F5C28F5C3u) >> 2; }

static inline uint64_t ryu_div1e8(uint64_t x) { return umulh(x, 0xABCC77118461CEFDu) >> 26; }

static inline uint32_t pow5Factor(uint64_t value) {
    const uint64_t m_inv_5 = 14757395258967641293u;
    const uint64_t n_div_5 = 3689348814741910323u;
    uint32_t count = 0;
    for (;;) {
        value *= m_inv_5;
        if (value > n_div_5)
            break;
        ++count;
    }
    return count;
}

static inline bool multipleOfPowerOf5(uint64_t value, uint32_t p) { return pow5Factor(value) >= p; }

static inline bool multipleOfPowerOf2(uint64_t value, uint32_t p) {
    return (value & ((1ull << p) - 1)) == 0;
}

// ============================================================================
// Compressed table lookup
// ============================================================================

static inline void double_computePow5(uint32_t i, uint64_t* result) {
    const uint32_t base = i / POW5_TABLE_SIZE;
    const uint32_t base2 = base * POW5_TABLE_SIZE;
    const uint32_t offset = i - base2;
    const uint64_t* mul = DOUBLE_POW5_SPLIT2[base];
    if (offset == 0) {
        result[0] = mul[0];
        result[1] = mul[1];
        return;
    }
    const uint64_t m = DOUBLE_POW5_TABLE[offset];
    const uint128_t b0 = static_cast<uint128_t>(m) * mul[0];
    const uint128_t b2 = static_cast<uint128_t>(m) * mul[1];
    const uint32_t delta =
        pow5bits(static_cast<int32_t>(i)) - pow5bits(static_cast<int32_t>(base2));
    const uint128_t shiftedSum =
        (b0 >> delta) + (b2 << (64 - delta)) + ((POW5_OFFSETS[i / 16] >> ((i % 16) << 1)) & 3);
    result[0] = static_cast<uint64_t>(shiftedSum);
    result[1] = static_cast<uint64_t>(shiftedSum >> 64);
}

static inline void double_computeInvPow5(uint32_t i, uint64_t* result) {
    const uint32_t base = (i + POW5_TABLE_SIZE - 1) / POW5_TABLE_SIZE;
    const uint32_t base2 = base * POW5_TABLE_SIZE;
    const uint32_t offset = base2 - i;
    const uint64_t* mul = DOUBLE_POW5_INV_SPLIT2[base];
    if (offset == 0) {
        result[0] = mul[0];
        result[1] = mul[1];
        return;
    }
    const uint64_t m = DOUBLE_POW5_TABLE[offset];
    const uint128_t b0 = static_cast<uint128_t>(m) * (mul[0] - 1);
    const uint128_t b2 = static_cast<uint128_t>(m) * mul[1];
    const uint32_t delta =
        pow5bits(static_cast<int32_t>(base2)) - pow5bits(static_cast<int32_t>(i));
    const uint128_t shiftedSum = ((b0 >> delta) + (b2 << (64 - delta))) + 1 +
                                 ((POW5_INV_OFFSETS[i / 16] >> ((i % 16) << 1)) & 3);
    result[0] = static_cast<uint64_t>(shiftedSum);
    result[1] = static_cast<uint64_t>(shiftedSum >> 64);
}

// ============================================================================
// Ryu 128-bit multiply-shift
// ============================================================================

static inline uint64_t mulShift64(uint64_t m, const uint64_t* mul, int32_t j) {
    const uint128_t b0 = static_cast<uint128_t>(m) * mul[0];
    const uint128_t b2 = static_cast<uint128_t>(m) * mul[1];
    return static_cast<uint64_t>(((b0 >> 64) + b2) >> (j - 64));
}

static inline uint64_t mulShiftAll64(uint64_t m, const uint64_t* mul, int32_t j, uint64_t* vp,
                                     uint64_t* vm, uint32_t mmShift) {
    *vp = mulShift64(4 * m + 2, mul, j);
    *vm = mulShift64(4 * m - 1 - mmShift, mul, j);
    return mulShift64(4 * m, mul, j);
}

// ============================================================================
// Ryu core: d2d - double to decimal
// ============================================================================

static constexpr int DOUBLE_MANTISSA_BITS = 52;
static constexpr int DOUBLE_EXPONENT_BITS = 11;
static constexpr int DOUBLE_BIAS = 1023;

struct floating_decimal_64 {
    uint64_t mantissa;
    int32_t exponent;
};

static inline uint32_t decimalLength17(uint64_t v) {
    if (v >= 10000000000000000ull)
        return 17;
    if (v >= 1000000000000000ull)
        return 16;
    if (v >= 100000000000000ull)
        return 15;
    if (v >= 10000000000000ull)
        return 14;
    if (v >= 1000000000000ull)
        return 13;
    if (v >= 100000000000ull)
        return 12;
    if (v >= 10000000000ull)
        return 11;
    if (v >= 1000000000ull)
        return 10;
    if (v >= 100000000ull)
        return 9;
    if (v >= 10000000ull)
        return 8;
    if (v >= 1000000ull)
        return 7;
    if (v >= 100000ull)
        return 6;
    if (v >= 10000ull)
        return 5;
    if (v >= 1000ull)
        return 4;
    if (v >= 100ull)
        return 3;
    if (v >= 10ull)
        return 2;
    return 1;
}

static inline bool d2d_small_int(uint64_t ieeeMantissa, uint32_t ieeeExponent,
                                 floating_decimal_64* v) {
    const uint64_t m2 = (1ull << DOUBLE_MANTISSA_BITS) | ieeeMantissa;
    const int32_t e2 = static_cast<int32_t>(ieeeExponent) - DOUBLE_BIAS - DOUBLE_MANTISSA_BITS;

    if (e2 > 0)
        return false;
    if (e2 < -52)
        return false;

    const uint64_t mask = (1ull << -e2) - 1;
    if ((m2 & mask) != 0)
        return false;

    v->mantissa = m2 >> -e2;
    v->exponent = 0;
    return true;
}

static inline floating_decimal_64 d2d(uint64_t ieeeMantissa, uint32_t ieeeExponent) {
    int32_t e2;
    uint64_t m2;
    if (ieeeExponent == 0) {
        e2 = 1 - DOUBLE_BIAS - DOUBLE_MANTISSA_BITS - 2;
        m2 = ieeeMantissa;
    } else {
        e2 = static_cast<int32_t>(ieeeExponent) - DOUBLE_BIAS - DOUBLE_MANTISSA_BITS - 2;
        m2 = (1ull << DOUBLE_MANTISSA_BITS) | ieeeMantissa;
    }
    const bool even = (m2 & 1) == 0;
    const bool acceptBounds = even;

    const uint64_t mv = 4 * m2;
    const uint32_t mmShift = ieeeMantissa != 0 || ieeeExponent <= 1;

    uint64_t vr, vp, vm;
    int32_t e10;
    bool vmIsTrailingZeros = false;
    bool vrIsTrailingZeros = false;

    if (e2 >= 0) {
        const uint32_t q = log10Pow2(e2) - (e2 > 3);
        e10 = static_cast<int32_t>(q);
        const int32_t k = DOUBLE_POW5_INV_BITCOUNT + pow5bits(static_cast<int32_t>(q)) - 1;
        const int32_t i = -e2 + static_cast<int32_t>(q) + k;
        uint64_t pow5[2];
        double_computeInvPow5(q, pow5);
        vr = mulShiftAll64(m2, pow5, i, &vp, &vm, mmShift);
        if (q <= 21) {
            const uint32_t mvMod5 =
                static_cast<uint32_t>(mv) - 5 * static_cast<uint32_t>(ryu_div5(mv));
            if (mvMod5 == 0) {
                vrIsTrailingZeros = multipleOfPowerOf5(mv, q);
            } else if (acceptBounds) {
                vmIsTrailingZeros = multipleOfPowerOf5(mv - 1 - mmShift, q);
            } else {
                vp -= multipleOfPowerOf5(mv + 2, q);
            }
        }
    } else {
        const uint32_t q = log10Pow5(-e2) - (-e2 > 1);
        e10 = static_cast<int32_t>(q) + e2;
        const int32_t i = -e2 - static_cast<int32_t>(q);
        const int32_t k = pow5bits(i) - DOUBLE_POW5_BITCOUNT;
        const int32_t j = static_cast<int32_t>(q) - k;
        uint64_t pow5[2];
        double_computePow5(static_cast<uint32_t>(i), pow5);
        vr = mulShiftAll64(m2, pow5, j, &vp, &vm, mmShift);
        if (q <= 1) {
            vrIsTrailingZeros = true;
            if (acceptBounds) {
                vmIsTrailingZeros = mmShift == 1;
            } else {
                --vp;
            }
        } else if (q < 63) {
            vrIsTrailingZeros = multipleOfPowerOf2(mv, q);
        }
    }

    // Step 4: Find shortest decimal representation
    int32_t removed = 0;
    uint8_t lastRemovedDigit = 0;
    uint64_t output;

    if (vmIsTrailingZeros || vrIsTrailingZeros) {
        for (;;) {
            const uint64_t vpDiv10 = ryu_div10(vp);
            const uint64_t vmDiv10 = ryu_div10(vm);
            if (vpDiv10 <= vmDiv10)
                break;
            const uint32_t vmMod10 =
                static_cast<uint32_t>(vm) - 10 * static_cast<uint32_t>(vmDiv10);
            const uint64_t vrDiv10 = ryu_div10(vr);
            const uint32_t vrMod10 =
                static_cast<uint32_t>(vr) - 10 * static_cast<uint32_t>(vrDiv10);
            vmIsTrailingZeros &= vmMod10 == 0;
            vrIsTrailingZeros &= lastRemovedDigit == 0;
            lastRemovedDigit = static_cast<uint8_t>(vrMod10);
            vr = vrDiv10;
            vp = vpDiv10;
            vm = vmDiv10;
            ++removed;
        }
        if (vmIsTrailingZeros) {
            for (;;) {
                const uint64_t vmDiv10 = ryu_div10(vm);
                const uint32_t vmMod10 =
                    static_cast<uint32_t>(vm) - 10 * static_cast<uint32_t>(vmDiv10);
                if (vmMod10 != 0)
                    break;
                const uint64_t vpDiv10 = ryu_div10(vp);
                const uint64_t vrDiv10 = ryu_div10(vr);
                const uint32_t vrMod10 =
                    static_cast<uint32_t>(vr) - 10 * static_cast<uint32_t>(vrDiv10);
                vrIsTrailingZeros &= lastRemovedDigit == 0;
                lastRemovedDigit = static_cast<uint8_t>(vrMod10);
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
            vr + ((vr == vm && (!acceptBounds || !vmIsTrailingZeros)) || lastRemovedDigit >= 5);
    } else {
        // Common case (~99.3%)
        bool roundUp = false;
        const uint64_t vpDiv100 = ryu_div100(vp);
        const uint64_t vmDiv100 = ryu_div100(vm);
        if (vpDiv100 > vmDiv100) {
            const uint64_t vrDiv100 = ryu_div100(vr);
            const uint32_t vrMod100 =
                static_cast<uint32_t>(vr) - 100 * static_cast<uint32_t>(vrDiv100);
            roundUp = vrMod100 >= 50;
            vr = vrDiv100;
            vp = vpDiv100;
            vm = vmDiv100;
            removed += 2;
        }
        for (;;) {
            const uint64_t vpDiv10 = ryu_div10(vp);
            const uint64_t vmDiv10 = ryu_div10(vm);
            if (vpDiv10 <= vmDiv10)
                break;
            const uint64_t vrDiv10 = ryu_div10(vr);
            const uint32_t vrMod10 =
                static_cast<uint32_t>(vr) - 10 * static_cast<uint32_t>(vrDiv10);
            roundUp = vrMod10 >= 5;
            vr = vrDiv10;
            vp = vpDiv10;
            vm = vmDiv10;
            ++removed;
        }
        output = vr + (vr == vm || roundUp);
    }

    floating_decimal_64 fd;
    fd.exponent = e10 + removed;
    fd.mantissa = output;
    return fd;
}

// ============================================================================
// JSON-compatible formatting
// ============================================================================

// Write mantissa digits into buf[0..olength-1].
// Uses Ryu's fast right-to-left digit pair extraction.
// Returns olength.
static inline uint32_t write_mantissa_digits(char* buf, uint64_t mantissa) {
    const uint32_t olength = decimalLength17(mantissa);
    // Write digits right-to-left using digit pairs
    char* p = buf + olength;
    while (mantissa >= 100) {
        const uint32_t c = static_cast<uint32_t>(mantissa % 100);
        mantissa /= 100;
        p -= 2;
        std::memcpy(p, DIGIT_TABLE + c * 2, 2);
    }
    if (mantissa >= 10) {
        p -= 2;
        std::memcpy(p, DIGIT_TABLE + static_cast<uint32_t>(mantissa) * 2, 2);
    } else {
        buf[0] = static_cast<char>('0' + mantissa);
    }
    return olength;
}

// Format mantissa * 10^exponent as JSON-compatible decimal string.
// Uses plain decimal for reasonable magnitudes, scientific for extremes.
static int format_json(char* result, uint64_t mantissa, int32_t exponent) {
    // First, write all digits into a temp buffer
    char digits[18];
    const uint32_t olength = write_mantissa_digits(digits, mantissa);

    // sci_exp = exponent + olength - 1
    const int32_t sci_exp = exponent + static_cast<int32_t>(olength) - 1;
    // dot_pos = number of digits before the decimal point
    const int32_t dot_pos = static_cast<int32_t>(olength) + exponent;

    char* p = result;

    // Use plain decimal for values in [1e-4, 1e17)
    if (sci_exp >= -4 && sci_exp < 17) {
        if (dot_pos <= 0) {
            // 0.000...0<digits>
            *p++ = '0';
            *p++ = '.';
            for (int i = 0; i < -dot_pos; i++)
                *p++ = '0';
            std::memcpy(p, digits, olength);
            p += olength;
        } else if (dot_pos >= static_cast<int32_t>(olength)) {
            // <digits><zeros>.0
            std::memcpy(p, digits, olength);
            p += olength;
            for (int i = 0; i < dot_pos - static_cast<int32_t>(olength); i++)
                *p++ = '0';
            *p++ = '.';
            *p++ = '0';
        } else {
            // <digits_before_dot>.<digits_after_dot>
            std::memcpy(p, digits, dot_pos);
            p[dot_pos] = '.';
            std::memcpy(p + dot_pos + 1, digits + dot_pos, olength - dot_pos);
            p += olength + 1;
        }
        return static_cast<int>(p - result);
    }

    // Scientific notation for extreme values
    p[0] = digits[0];
    if (olength > 1) {
        p[1] = '.';
        std::memcpy(p + 2, digits + 1, olength - 1);
        p += olength + 1;
    } else {
        ++p;
    }

    *p++ = 'e';
    int32_t exp = sci_exp;
    if (exp < 0) {
        *p++ = '-';
        exp = -exp;
    } else {
        *p++ = '+';
    }
    if (exp >= 100) {
        const int32_t c = exp % 10;
        std::memcpy(p, DIGIT_TABLE + 2 * (exp / 10), 2);
        p[2] = static_cast<char>('0' + c);
        p += 3;
    } else if (exp >= 10) {
        std::memcpy(p, DIGIT_TABLE + 2 * exp, 2);
        p += 2;
    } else {
        *p++ = static_cast<char>('0' + exp);
    }

    return static_cast<int>(p - result);
}

// ============================================================================
// Main entry point
// ============================================================================

int fast_dtoa(double value, char* buffer) {
    uint64_t bits;
    std::memcpy(&bits, &value, sizeof(bits));

    const uint64_t ieeeMantissa = bits & ((1ull << DOUBLE_MANTISSA_BITS) - 1);
    const uint32_t ieeeExponent =
        static_cast<uint32_t>((bits >> DOUBLE_MANTISSA_BITS) & ((1u << DOUBLE_EXPONENT_BITS) - 1));

    floating_decimal_64 v;
    const bool isSmallInt = d2d_small_int(ieeeMantissa, ieeeExponent, &v);
    if (isSmallInt) {
        for (;;) {
            const uint64_t q = ryu_div10(v.mantissa);
            const uint32_t r = static_cast<uint32_t>(v.mantissa) - 10 * static_cast<uint32_t>(q);
            if (r != 0)
                break;
            v.mantissa = q;
            ++v.exponent;
        }
    } else {
        v = d2d(ieeeMantissa, ieeeExponent);
    }

    return format_json(buffer, v.mantissa, v.exponent);
}

} // namespace util
} // namespace strata
