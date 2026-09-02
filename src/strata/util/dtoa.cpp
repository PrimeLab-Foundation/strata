/**
 * @file dtoa.cpp
 * @brief Shortest round-trip double formatting.
 *
 * Three layers, fastest first:
 *
 *   1. The micro-decimal tier: values that are exactly n/10^6 emit n's digits
 *      directly (most real-world floats — prices, scores, coordinates), with
 *      an integral-product pre-filter so long-form data rejects without the
 *      membership divide.
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
 * The whole pipeline is out of line on purpose: a header-inline variant with
 * the micro tier at the call site measured 6% slower on mixed short/long
 * float arrays — the tier's branch is data-random there and mispredicts at
 * every call site, while the single call is predictable (the wave-10
 * negative result in docs/performance/SKILL.md). LTO/PGO builds still
 * inline across the boundary where the profile says it pays.
 *
 * The vendored Dragonbox header keeps upstream's exact text so it can be
 * audited (and updated) against its repository byte-for-byte; it is excluded
 * from clang-format for the same reason.
 */

#include "strata/util/dtoa.hpp"

#include "strata/third_party/dragonbox/dragonbox.h"

#include <cmath>
#include <cstring>

namespace strata::util {

namespace {

// The digit machinery — pair table, digit count, grouped fixed-width write,
// the 16-byte gap shift — lives in dtoa.hpp's detail namespace, shared with
// the header-inline format_int64.
using detail::decimal_digit_count;
using detail::kDigitPairs;
using detail::shift16_right;
using detail::write_digits_fixed;

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
 * coordinates), and for them even Dragonbox is overkill. If `value` equals
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

    // Integral-product pre-filter: for every member this tier serves, the
    // product below is *exactly* the integer n (the relative error of one
    // multiply stays under half a unit throughout the gated range), so a
    // non-integral product rejects without paying the divide — which is the
    // whole cost of running this tier over long-form float data. Truncation
    // rather than llround: llround is one instruction on arm64 but a libc
    // call on x86-64, and a mis-rounded borderline here merely fails the
    // membership check below and takes the general path — correctness never
    // rests on this rounding.
    const double product = magnitude * 1e6;
    const auto scaled = static_cast<int64_t>(product + 0.5);
    if (static_cast<double>(scaled) != product)
        return 0;

    // The divide is the membership proof and is not negotiable: an integral
    // product alone admits values that are *near* n/10^6 without being its
    // nearest double (caught by the round-trip oracle when tried — the
    // binade-boundary slack breaks the half-ulp argument). Division of two
    // exact values is correctly rounded, so equality here is exact.
    if (static_cast<double>(scaled) / 1e6 != magnitude)
        return 0;

    // Emission is two digit words and no data-dependent branch: the whole
    // part as one eight-digit word shifted down to its own length, the point,
    // then the six fraction digits as one word whose trailing zeros are
    // measured rather than looped away. The earlier shape — strip zeros in a
    // loop, count, write the digits as one run, open a gap for the point —
    // mispredicted on the strip and the length tests (both random on real
    // data) and reloaded, sixteen bytes wide, digits just stored two bytes
    // at a time, which no store buffer forwards; this reads no byte it wrote.
    const auto n = static_cast<uint64_t>(scaled);
    const uint64_t whole = n / 1000000;
    const auto fraction = static_cast<uint32_t>(n - whole * 1000000);
    if (whole >= 100000000)
        return 0; // nine or more whole digits: the general path, rarely reached

    size_t written = 0;
    if (value < 0.0)
        out[written++] = '-';

    // Whole part: byte k of the word is the k-th digit from the left, so
    // dropping the leading zero bytes is a shift by their count (0..7 bytes
    // — never the full width). The store spills scratch past the length,
    // overwritten by the point and fraction below; all of it sits inside the
    // 40-byte buffer the contract demands.
    const size_t whole_length = decimal_digit_count(whole);
    detail::store_digit_word(out + written,
                             detail::eight_digits_word(static_cast<uint32_t>(whole)) >>
                                 ((8 - whole_length) * 8));
    written += whole_length;
    out[written++] = '.';

    // Fraction: "dddddd00" — the six digits lead the word. A digit byte
    // differs from '0' exactly when its digit is non-zero, so the highest
    // such byte among the six is the last digit to keep; `| 1` makes an
    // all-zero fraction keep its first digit, the ".0" that marks a float.
    const uint64_t fraction_word = detail::eight_digits_word(fraction * 100);
    detail::store_digit_word(out + written, fraction_word);
    const uint64_t non_zero = (fraction_word ^ 0x3030303030303030ULL) & 0x0000FFFFFFFFFFFFULL;
    const size_t kept = (detail::highest_set_bit(non_zero | 1) >> 3) + 1;
    return written + kept;
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

} // namespace strata::util
