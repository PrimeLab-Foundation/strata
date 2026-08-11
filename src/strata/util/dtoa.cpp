/**
 * @file dtoa.cpp
 * @brief Shortest round-trip double formatting.
 *
 * Three layers, fastest first:
 *
 *   1. The micro-decimal tier: values that are exactly n/10^6 emit n's digits
 *      directly (most real-world floats — prices, scores, coordinates).
 *   2. Dragonbox: the shortest correctly-rounded digit string for everything
 *      else, via the vendored reference implementation (Junekey Jeon,
 *      include/strata/third_party/dragonbox/, Apache-2.0 with LLVM exceptions
 *      / BSL-1.0). It replaces the Ryu d2d port this file previously carried:
 *      same unique shortest output, measurably faster digit generation.
 *   3. This file's own layout, applied to both: fixed notation while the
 *      scientific exponent sits in [-4, 15], scientific outside it — exactly
 *      CPython's `repr(float)` rule, so strata renders a float byte-for-byte
 *      as the standard library would. The equivalence is pinned by test
 *      against a `to_chars`-based reference over tens of millions of values.
 *
 * The vendored Dragonbox header keeps upstream's exact text so it can be
 * audited (and updated) against its repository byte-for-byte; it is excluded
 * from clang-format for the same reason.
 */

#include "strata/util/dtoa.hpp"

#include "strata/third_party/dragonbox/dragonbox.h"

#include <cmath>
#include <cstring>

#if defined(_MSC_VER) && !defined(__clang__)
#include <intrin.h>
#endif

namespace {

/// Index of the highest set bit. Precondition: @p value != 0.
[[nodiscard]] inline unsigned highest_set_bit(uint64_t value) noexcept {
#if defined(_MSC_VER) && !defined(__clang__)
    unsigned long index = 0;
    _BitScanReverse64(&index, value);
    return static_cast<unsigned>(index);
#else
    return static_cast<unsigned>(63 - __builtin_clzll(value));
#endif
}

} // namespace

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
    const auto bits = static_cast<size_t>(highest_set_bit(value));
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

    // Dragonbox: IEEE double -> shortest correctly-rounded (digits, 10^exp),
    // trailing zeros already removed — the same unique output the Ryu d2d it
    // replaced produced, from measurably faster digit generation. Sign and
    // zero were handled above, which is exactly this call's precondition.
    const auto decimal = jkj::dragonbox::to_decimal(value, jkj::dragonbox::policy::sign::ignore,
                                                    jkj::dragonbox::policy::trailing_zero::remove);
    const uint64_t digits = decimal.significand;
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
