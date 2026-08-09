/**
 * @file dtoa.cpp
 * @brief Shortest round-trip double formatting.
 *
 * Three layers, fastest first:
 *
 *   1. The micro-decimal tier: values that are exactly n/10^6 emit n's digits
 *      directly (most real-world floats — prices, scores, coordinates).
 *   2. Ryu: the shortest correctly-rounded digit string for everything else,
 *      ported from the reference implementation (github.com/ulfjack/ryu,
 *      Ulf Adams; Apache License 2.0 / Boost Software License 1.0) via this
 *      project's previous implementation. Roughly twice as fast as libc++'s
 *      `to_chars` machinery for the same digits.
 *   3. This file's own layout, applied to both: fixed notation while the
 *      scientific exponent sits in [-4, 15], scientific outside it — exactly
 *      CPython's `repr(float)` rule, so strata renders a float byte-for-byte
 *      as the standard library would. The equivalence is pinned by test
 *      against a `to_chars`-based reference over tens of millions of values.
 *
 * The Ryu core keeps the reference implementation's names and shape so it can
 * be audited line-by-line against upstream — a deliberate exception to the
 * project naming style, confined to the `ryu` namespace.
 */

#include "strata/util/dtoa.hpp"

#include <cmath>
#include <cstring>

#if defined(_MSC_VER) && !defined(__clang__)
#include <intrin.h>
#endif

namespace strata::util {

namespace {

/// "00" "01" ... "99", so digits are peeled two at a time.
constexpr char kDigitPairs[] = "00010203040506070809"
                               "10111213141516171819"
                               "20212223242526272829"
                               "30313233343536373839"
                               "40414243444546474849"
                               "50515253545556575859"
                               "60616263646566676869"
                               "70717273747576777879"
                               "80818283848586878889"
                               "90919293949596979899";

constexpr uint64_t kPow10[20] = {
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
    1000000000000000000ULL,
    10000000000000000000ULL,
};

/// Fill digits of @p value backwards, ending at @p end; returns the first byte.
[[nodiscard]] char* fill_u64_backwards(uint64_t value, char* end) noexcept {
    char* cursor = end;
    while (value >= 100) {
        const size_t pair = static_cast<size_t>(value % 100) * 2;
        value /= 100;
        *--cursor = kDigitPairs[pair + 1];
        *--cursor = kDigitPairs[pair];
    }
    if (value >= 10) {
        const size_t pair = static_cast<size_t>(value) * 2;
        *--cursor = kDigitPairs[pair + 1];
        *--cursor = kDigitPairs[pair];
    } else {
        *--cursor = static_cast<char>('0' + value);
    }
    return cursor;
}

/// Number of decimal digits in @p value (1 for zero).
[[nodiscard]] size_t decimal_digit_count(uint64_t value) noexcept {
    if (value == 0)
        return 1;
    // floor(log10) from floor(log2): multiply by log10(2) in fixed point,
    // then correct by comparing against the exact power.
    const auto bits = static_cast<size_t>(63 - __builtin_clzll(value));
    size_t digits = (bits * 1233) >> 12;
    digits += static_cast<size_t>(digits + 1 <= 19 && value >= kPow10[digits + 1]);
    return digits + 1;
}

/// Exactly eight digits of @p value (< 10^8), zero-padded.
inline void write_8_digits(uint32_t value, char* out) noexcept {
    const uint32_t high = value / 10000;
    const uint32_t low = value % 10000;
    const uint32_t pair0 = (high / 100) * 2;
    const uint32_t pair1 = (high % 100) * 2;
    const uint32_t pair2 = (low / 100) * 2;
    const uint32_t pair3 = (low % 100) * 2;
    std::memcpy(out + 0, kDigitPairs + pair0, 2);
    std::memcpy(out + 2, kDigitPairs + pair1, 2);
    std::memcpy(out + 4, kDigitPairs + pair2, 2);
    std::memcpy(out + 6, kDigitPairs + pair3, 2);
}

/// Exactly @p len digits of @p value, zero-padded on the left.
///
/// Peels eight digits at a time into 32-bit groups: the divides become
/// multiply-shifts, the groups' four pair lookups are independent, and a
/// 17-digit mantissa costs two groups plus one odd digit instead of a
/// seventeen-step serial divide chain.
void write_digits_fixed(uint64_t value, char* out, size_t len) noexcept {
    while (len >= 8) {
        const uint64_t high = value / 100000000;
        const auto low = static_cast<uint32_t>(value - high * 100000000);
        write_8_digits(low, out + len - 8);
        value = high;
        len -= 8;
    }
    auto rest = static_cast<uint32_t>(value);
    size_t index = len;
    while (index >= 2) {
        const uint32_t pair = (rest % 100) * 2;
        rest /= 100;
        out[index - 1] = kDigitPairs[pair + 1];
        out[index - 2] = kDigitPairs[pair];
        index -= 2;
    }
    if (index == 1)
        out[0] = static_cast<char>('0' + rest % 10);
}

/// Shift sixteen bytes right by @p gap places (loads before stores, so the
/// ranges may overlap). Spelled as two u64 moves because a `memmove` call --
/// which is what the extension build turned the "constant-size" form into --
/// profiled at 12% of users.json serialization.
inline void shift16_right(char* from, size_t gap) noexcept {
    uint64_t low;
    uint64_t high;
    std::memcpy(&low, from, 8);
    std::memcpy(&high, from + 8, 8);
    std::memcpy(from + gap, &low, 8);
    std::memcpy(from + gap + 8, &high, 8);
}

/// The exponent suffix digits: two for 0..99, three above (`e+05`, `e+308`).
[[nodiscard]] char* write_exponent(int value, char* out) noexcept {
    if (value >= 100) {
        *out++ = static_cast<char>('0' + value / 100);
        value %= 100;
    }
    std::memcpy(out, kDigitPairs + 2 * value, 2);
    return out + 2;
}

// ---------------------------------------------------------------------------
// Ryu d2d: IEEE double -> shortest (mantissa, exponent-of-ten).
// Reference implementation's names and structure, for auditability.
// ---------------------------------------------------------------------------
namespace ryu {

constexpr int MANTISSA_BITS = 52;
constexpr int EXPONENT_BITS = 11;
constexpr int BIAS = 1023;
constexpr uint64_t MANTISSA_MASK = (1ULL << MANTISSA_BITS) - 1;
constexpr int POW5_INV_BITCOUNT = 125;
constexpr int POW5_BITCOUNT = 125;

inline int log10Pow2(int e) {
    return static_cast<int>(
        (static_cast<uint64_t>(static_cast<uint32_t>(e)) * 169464822037455ULL) >> 49);
}

inline int log10Pow5(int e) {
    return static_cast<int>(
        (static_cast<uint64_t>(static_cast<uint32_t>(e)) * 196742565691928ULL) >> 48);
}

inline int pow5bits(int e) {
    return static_cast<int>((static_cast<uint64_t>(static_cast<uint32_t>(e)) * 1217359) >> 19) + 1;
}

inline bool multipleOfPow5(uint64_t v, int p) {
    for (int i = 0; i < p; ++i) {
        if (v % 5 != 0)
            return false;
        v /= 5;
    }
    return true;
}

inline bool multipleOfPow2(uint64_t v, int p) { return (v & ((1ULL << p) - 1)) == 0; }

#if defined(_MSC_VER) && !defined(__clang__)
inline uint64_t mulShift64(uint64_t m, const uint64_t* mul, int j) {
    // ((m * mul[0]) >> 64 + m * mul[1]) >> (j - 64), without __uint128_t.
    uint64_t high0 = 0;
    (void)_umul128(m, mul[0], &high0);
    uint64_t high1 = 0;
    const uint64_t low1 = _umul128(m, mul[1], &high1);
    const uint64_t sum_low = low1 + high0;
    high1 += (sum_low < low1) ? 1 : 0;
    const int shift = j - 64; // always in (0, 64) for the table exponents
    return (sum_low >> shift) | (high1 << (64 - shift));
}
#else
inline uint64_t mulShift64(uint64_t m, const uint64_t* mul, int j) {
    const __uint128_t b0 = static_cast<__uint128_t>(m) * mul[0];
    const __uint128_t b1 = static_cast<__uint128_t>(m) * mul[1];
    const __uint128_t mid = (b0 >> 64) + b1;
    return static_cast<uint64_t>(mid >> (j - 64));
}
#endif

inline uint64_t mulShiftAll64(uint64_t m, const uint64_t* mul, int j, uint64_t* vp, uint64_t* vm,
                              uint32_t mmShift) {
    *vp = mulShift64(4 * m + 2, mul, j);
    *vm = mulShift64(4 * m - 1 - mmShift, mul, j);
    return mulShift64(4 * m, mul, j);
}

#include "ryu_tables.inc"

struct Decimal {
    uint64_t mantissa;
    int32_t exponent;
};

inline Decimal d2d(uint64_t ieeeMantissa, uint32_t ieeeExponent) {
    int32_t e2;
    uint64_t m2;
    if (ieeeExponent == 0) {
        e2 = 1 - BIAS - MANTISSA_BITS - 2;
        m2 = ieeeMantissa;
    } else {
        e2 = static_cast<int32_t>(ieeeExponent) - BIAS - MANTISSA_BITS - 2;
        m2 = (1ULL << MANTISSA_BITS) | ieeeMantissa;
    }

    const bool even = (m2 & 1) == 0;
    const bool acceptBounds = even;
    const uint64_t mv = 4 * m2;
    const uint32_t mmShift = (ieeeMantissa != 0 || ieeeExponent <= 1) ? 1 : 0;

    uint64_t vr, vp, vm;
    int32_t e10;
    bool vmTZ = false, vrTZ = false;

    if (e2 >= 0) {
        const int32_t q = log10Pow2(e2) - (e2 > 3 ? 1 : 0);
        e10 = q;
        const int32_t k = pow5bits(q) - 1 + POW5_INV_BITCOUNT;
        const int32_t i = -e2 + q + k;
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
        const int32_t q = log10Pow5(-e2) - (-e2 > 1 ? 1 : 0);
        e10 = q + e2;
        const int32_t i = -e2 - q;
        const int32_t k = pow5bits(i) - POW5_BITCOUNT;
        const int32_t j = q - k;
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

    int32_t removed = 0;
    uint64_t lastRemoved = 0;
    uint64_t output;

    if (vmTZ || vrTZ) {
        for (;;) {
            const uint64_t vpD = vp / 10, vmD = vm / 10;
            if (vpD <= vmD)
                break;
            const uint64_t vmM = vm - 10 * vmD;
            const uint64_t vrD = vr / 10, vrM = vr - 10 * vrD;
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
                const uint64_t vmD = vm / 10, vmM = vm - 10 * vmD;
                if (vmM != 0)
                    break;
                const uint64_t vpD = vp / 10;
                const uint64_t vrD = vr / 10, vrM = vr - 10 * vrD;
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
        output = vr + (((vr == vm && (!acceptBounds || !vmTZ)) || lastRemoved >= 5) ? 1 : 0);
    } else {
        bool roundUp = false;
        const uint64_t vpD100 = vp / 100, vmD100 = vm / 100;
        if (vpD100 > vmD100) {
            const uint64_t vrD100 = vr / 100, vrM100 = vr - 100 * vrD100;
            roundUp = vrM100 >= 50;
            vr = vrD100;
            vp = vpD100;
            vm = vmD100;
            removed += 2;
        }
        for (;;) {
            const uint64_t vpD = vp / 10, vmD = vm / 10;
            if (vpD <= vmD)
                break;
            const uint64_t vrD = vr / 10, vrM = vr - 10 * vrD;
            roundUp = vrM >= 5;
            vr = vrD;
            vp = vpD;
            vm = vmD;
            ++removed;
        }
        output = vr + ((vr == vm || roundUp) ? 1 : 0);
    }

    return {output, e10 + removed};
}

} // namespace ryu

/**
 * The micro-decimal tier: values that are exactly a 6-decimal fixed number.
 *
 * Real-world floats are overwhelmingly short decimals (prices, scores,
 * coordinates), and for them even Ryu is overkill. If `value` equals
 * `n / 10^6` for some integer `n` -- checked with one exact division, since
 * both `n` and `10^6` are exactly representable -- then the digits of `n`
 * with trailing zeros stripped ARE the shortest form: within the gate below
 * a half-ulp is smaller than the 10^-6 lattice spacing, so at most one
 * 6-decimal value can round-trip, and no shorter decimal can either.
 *
 * @return Bytes written, or 0 when the value is not of this shape.
 */
[[nodiscard]] size_t format_micro_decimal(double value, char* out) noexcept {
    const double magnitude = std::fabs(value);
    // Below 1e-4 Python switches to scientific layout. The upper gate is a
    // soundness bound, not an overflow one: the digits are only *the*
    // shortest form while one ulp stays below the 10^-6 lattice spacing, so
    // that at most one 6-decimal value can round-trip. Above ~4e9 an ulp
    // exceeds 1e-6 and a non-minimal witness can pass the exactness check --
    // measured as 1513 repr() mismatches in the 2^35..2^41 range before this
    // gate was tightened.
    if (!(magnitude >= 1e-4 && magnitude < 4.0e9))
        return 0;

    // The divide is the membership proof and is not negotiable: an integral
    // product alone admits values that are *near* n/10^6 without being its
    // nearest double (caught by the round-trip oracle when tried — the
    // binade-boundary slack breaks the half-ulp argument). Division of two
    // exact values is correctly rounded, so equality here is exact.
    const auto scaled = static_cast<int64_t>(std::llround(magnitude * 1e6));
    if (static_cast<double>(scaled) / 1e6 != magnitude)
        return 0;

    uint64_t digits = static_cast<uint64_t>(scaled);
    int fraction = 6;
    while (fraction > 0 && digits % 10 == 0) {
        digits /= 10;
        --fraction;
    }

    size_t written = 0;
    if (value < 0.0)
        out[written++] = '-';

    const size_t length = decimal_digit_count(digits);

    if (fraction == 0) {
        // Integral: digits then the fraction that keeps it a float.
        write_digits_fixed(digits, out + written, length);
        written += length;
        out[written++] = '.';
        out[written++] = '0';
        return written;
    }

    const int point = static_cast<int>(length) - fraction;
    if (point <= 0) {
        // 0.00ddd — at most three leading zeros under the 1e-4 gate.
        out[written++] = '0';
        out[written++] = '.';
        for (int pad = 0; pad < -point; ++pad)
            out[written++] = '0';
        write_digits_fixed(digits, out + written, length);
        return written + length;
    }

    // dd.ddd — digits straight into place, constant-size shift for the point
    // (a variable-length copy here compiles to a libc call; see shift16_right).
    write_digits_fixed(digits, out + written, length);
    shift16_right(out + written + static_cast<size_t>(point), 1);
    out[written + static_cast<size_t>(point)] = '.';
    return written + length + 1;
}

} // namespace

size_t format_double(double value, char* out, size_t capacity) noexcept {
    if (capacity < kDoubleBufferSize)
        return 0;

    if (const size_t fast = format_micro_decimal(value, out); fast != 0)
        return fast;

    size_t written = 0;
    if (std::signbit(value))
        out[written++] = '-';
    if (value == 0.0) {
        out[written++] = '0';
        out[written++] = '.';
        out[written++] = '0';
        return written;
    }

    uint64_t bits;
    std::memcpy(&bits, &value, sizeof(bits));
    const uint64_t ieee_mantissa = bits & ryu::MANTISSA_MASK;
    const auto ieee_exponent =
        static_cast<uint32_t>((bits >> ryu::MANTISSA_BITS) & ((1U << ryu::EXPONENT_BITS) - 1));

    const ryu::Decimal decimal = ryu::d2d(ieee_mantissa, ieee_exponent);
    const uint64_t digits = decimal.mantissa;
    const auto length = decimal_digit_count(digits);
    // value = digits * 10^exponent; the scientific exponent is where the
    // leading digit sits.
    const auto scientific_exponent = static_cast<int32_t>(length) + decimal.exponent - 1;

    // CPython's repr layout rule: fixed while the exponent is in [-4, 15].
    if (scientific_exponent >= -4 && scientific_exponent < 16) {
        const int32_t point = scientific_exponent + 1;
        if (point <= 0) {
            // 0.00ddd
            out[written++] = '0';
            out[written++] = '.';
            for (int32_t pad = 0; pad < -point; ++pad)
                out[written++] = '0';
            write_digits_fixed(digits, out + written, length);
            return written + length;
        }
        if (static_cast<size_t>(point) >= length) {
            // ddd000.0 — integral, and the fraction keeps it a float.
            write_digits_fixed(digits, out + written, length);
            written += length;
            for (size_t pad = length; pad < static_cast<size_t>(point); ++pad)
                out[written++] = '0';
            out[written++] = '.';
            out[written++] = '0';
            return written;
        }
        // dd.ddd — one digit pass straight into place, then a constant-size
        // memmove opens the gap for the point. The constant length is what
        // matters: a variable-length copy compiles to a libc call, which
        // cost more than the digits themselves; sixteen bytes is two vector
        // moves and covers the longest possible fraction. Bytes shuffled
        // beyond the reported length are scratch inside the 40-byte buffer.
        write_digits_fixed(digits, out + written, length);
        shift16_right(out + written + point, 1);
        out[written + point] = '.';
        return written + length + 1;
    }

    // d.ddde±XX — and `1e+16`, not `1.0e+16`, when one digit is the digits.
    if (length == 1) {
        out[written++] = static_cast<char>('0' + digits);
    } else {
        // Same constant-size gap trick as the fixed split above.
        write_digits_fixed(digits, out + written, length);
        shift16_right(out + written + 1, 1);
        out[written + 1] = '.';
        written += length + 1;
    }
    out[written++] = 'e';
    out[written++] = scientific_exponent >= 0 ? '+' : '-';
    const int magnitude = scientific_exponent >= 0 ? scientific_exponent : -scientific_exponent;
    return static_cast<size_t>(write_exponent(magnitude, out + written) - out);
}

size_t format_int64(int64_t value, char* out) noexcept {
    uint64_t magnitude = static_cast<uint64_t>(value);
    size_t written = 0;
    if (value < 0) {
        out[written++] = '-';
        magnitude = 0 - magnitude; // wraps correctly for INT64_MIN
    }
    // Count first, then back-fill in place: the digits land exactly where
    // they belong and nothing is copied twice.
    const size_t digits = decimal_digit_count(magnitude);
    (void)fill_u64_backwards(magnitude, out + written + digits);
    return written + digits;
}

} // namespace strata::util
