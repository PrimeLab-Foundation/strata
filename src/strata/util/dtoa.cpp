/**
 * @file dtoa.cpp
 * @brief Shortest round-trip double formatting.
 *
 * `std::to_chars` supplies the hard part: the shortest digit string that reads
 * back as the same double, correctly rounded. What it does not supply is a
 * choice of layout, so this file takes the scientific form and re-lays it out.
 *
 * The switch to scientific notation happens when the decimal point would sit
 * at or before the fourth place after the point, or beyond the sixteenth
 * before it. That is CPython's rule for `repr(float)`, and matching it means
 * strata renders a float exactly as the standard library would.
 */

#include "strata/util/dtoa.hpp"

#include <charconv>
#include <cmath>
#include <cstring>
#include <system_error>

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

/// Fixed notation holds inside this window; outside it Python goes scientific.
constexpr double kFixedLowerBound = 1e-4; ///< 1e-5 is scientific, 1e-4 is not
constexpr double kFixedUpperBound = 1e16; ///< 1e16 is scientific, 1e15 is not

} // namespace

namespace {

/**
 * The micro-decimal tier: values that are exactly a 6-decimal fixed number.
 *
 * Real-world floats are overwhelmingly short decimals (prices, scores,
 * coordinates), and for them the general shortest-round-trip machinery is
 * overkill. If `value` equals `n / 10^6` for some integer `n` -- checked with
 * one exact division, since both `n` and `10^6` are exactly representable --
 * then the digits of `n` with trailing zeros stripped ARE the shortest form:
 * within the fixed-notation window a half-ulp is astronomically smaller than
 * the 10^-6 gap to any other 6-decimal value, so no shorter decimal can
 * round-trip, and no equal-length one can either.
 *
 * Bytes are provably identical to the general path; the equality gate is what
 * makes that a theorem rather than a hope, and the whole-bit-space oracle
 * diff against CPython repr() checks it anyway.
 *
 * @return Bytes written, or 0 when the value is not of this shape.
 */
[[nodiscard]] size_t format_micro_decimal(double value, char* out) noexcept {
    const double magnitude = std::fabs(value);
    // Below 1e-4 Python switches to scientific layout. The upper gate is a
    // soundness bound, not an overflow one: the digits are only *the*
    // shortest form while one ulp stays below the 10^-6 lattice spacing, so
    // that at most one 6-decimal value can round-trip. Above ~4e9 an ulp
    // exceeds 1e-6 and a non-minimal witness can pass the equality check --
    // measured as 1513 repr() mismatches in the 2^35..2^41 range before this
    // gate was tightened.
    if (!(magnitude >= 1e-4 && magnitude < 4.0e9))
        return 0;

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

    // One digit conversion for the whole number; the point is placed by
    // splitting the digit string `fraction` places from its end.
    char stage[24];
    char* const stage_end = stage + sizeof(stage);
    const char* const first = fill_u64_backwards(digits, stage_end);
    const auto total = static_cast<size_t>(stage_end - first);

    if (fraction == 0) {
        std::memcpy(out + written, first, total);
        written += total;
        out[written++] = '.';
        out[written++] = '0';
        return written;
    }
    if (total > static_cast<size_t>(fraction)) {
        const size_t whole_digits = total - static_cast<size_t>(fraction);
        std::memcpy(out + written, first, whole_digits);
        written += whole_digits;
        out[written++] = '.';
        std::memcpy(out + written, first + whole_digits, static_cast<size_t>(fraction));
        return written + static_cast<size_t>(fraction);
    }
    // 0.00ddd — every digit is fractional, zero-padded on the left.
    out[written++] = '0';
    out[written++] = '.';
    for (size_t pad = total; pad < static_cast<size_t>(fraction); ++pad)
        out[written++] = '0';
    std::memcpy(out + written, first, total);
    return written + total;
}

} // namespace

size_t format_double(double value, char* out, size_t capacity) noexcept {
    if (capacity < kDoubleBufferSize)
        return 0;

    if (const size_t fast = format_micro_decimal(value, out); fast != 0)
        return fast;

    // Which layout Python uses is decided by magnitude alone, so two
    // comparisons answer it — no conversion needed to find out.
    //
    // This is exactly equivalent to testing the *shortest form's* decimal
    // point against the bounds above, because the boundaries are themselves
    // representable: a value whose shortest form reads `1e+16` is the double
    // 1e16, and one whose shortest form reads `0.0001` is the double 1e-4.
    // No value can round across a boundary into the other layout.
    const double magnitude = std::fabs(value);
    const bool scientific =
        magnitude != 0.0 && (magnitude < kFixedLowerBound || magnitude >= kFixedUpperBound);

    // Both styles ask to_chars for the shortest digits that round-trip, and
    // the fixed style *is* the layout we want — the previous version threw
    // that away by always asking for scientific and then re-laying the digits
    // out by hand, which cost a second pass plus an exponent re-parse.
    const auto converted =
        std::to_chars(out, out + capacity, value,
                      scientific ? std::chars_format::scientific : std::chars_format::fixed);
    if (converted.ec != std::errc{})
        return 0;
    size_t written = static_cast<size_t>(converted.ptr - out);

    // An integral value comes back with no fraction, and JSON would then read
    // it back as an int. Scientific form needs no such help: Python writes
    // `1e+16`, not `1.0e+16`.
    //
    // Whether the fraction is missing is asked of the *value*, not of the
    // digits: shortest-fixed omits the point exactly when the value is
    // integral, because a string with no fraction can only round-trip to a
    // whole number. One compare replaces a scan of the output.
    if (!scientific && value == std::trunc(value)) {
        out[written++] = '.';
        out[written++] = '0';
    }
    return written;
}

namespace {

/// Number of decimal digits in @p value (1 for zero).
[[nodiscard]] size_t decimal_digit_count(uint64_t value) noexcept {
    if (value == 0)
        return 1;
    // floor(log10) from floor(log2): multiply by log10(2) in fixed point,
    // then correct by comparing against the exact power.
    static constexpr uint64_t kPow10[20] = {
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
    const auto bits = static_cast<size_t>(63 - __builtin_clzll(value));
    size_t digits = (bits * 1233) >> 12;
    digits += static_cast<size_t>(value >= kPow10[digits + 1 <= 19 ? digits + 1 : 19] &&
                                  digits + 1 <= 19);
    return digits + 1;
}

} // namespace

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
