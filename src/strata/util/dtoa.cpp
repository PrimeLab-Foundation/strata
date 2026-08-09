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
#include <system_error>

namespace strata::util {

namespace {

/// Fixed notation holds inside this window; outside it Python goes scientific.
constexpr double kFixedLowerBound = 1e-4; ///< 1e-5 is scientific, 1e-4 is not
constexpr double kFixedUpperBound = 1e16; ///< 1e16 is scientific, 1e15 is not

} // namespace

size_t format_double(double value, char* out, size_t capacity) noexcept {
    if (capacity < kDoubleBufferSize)
        return 0;

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

} // namespace strata::util
