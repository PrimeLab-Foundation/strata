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
#include <cstring>
#include <string_view>
#include <system_error>

namespace strata::util {

namespace {

/// Scientific notation starts once the decimal point leaves this window.
constexpr int kMinDecimalPoint = -4; ///< decpt <= -4 goes scientific (1e-5 does, 1e-4 does not)
constexpr int kMaxDecimalPoint = 16; ///< decpt > 16 goes scientific (1e16 does, 1e15 does not)

} // namespace

size_t format_double(double value, char* out, size_t capacity) noexcept {
    if (capacity < kDoubleBufferSize)
        return 0;

    // Shortest scientific form: exactly one digit before the point, so the
    // exponent read from it is the decimal exponent of the leading digit.
    char scientific[kDoubleBufferSize];
    const auto converted = std::to_chars(scientific, scientific + sizeof(scientific), value,
                                         std::chars_format::scientific);
    if (converted.ec != std::errc{})
        return 0;
    const std::string_view text(scientific, static_cast<size_t>(converted.ptr - scientific));

    // Pull the pieces apart by hand: the mantissa's point, if any, sits at a
    // known place and the digits either side of it are what we need.
    size_t cursor = 0;
    const bool negative = !text.empty() && text[0] == '-';
    if (negative)
        ++cursor;

    const size_t exponent_pos = text.find('e', cursor);
    if (exponent_pos == std::string_view::npos)
        return 0;

    char digits[kDoubleBufferSize];
    size_t digit_count = 0;
    for (size_t index = cursor; index < exponent_pos; ++index) {
        if (text[index] != '.')
            digits[digit_count++] = text[index];
    }
    if (digit_count == 0)
        return 0;

    int exponent = 0;
    {
        const char* first = text.data() + exponent_pos + 1;
        const char* last = text.data() + text.size();
        if (first < last && *first == '+')
            ++first;
        if (std::from_chars(first, last, exponent).ec != std::errc{})
            return 0;
    }

    // decpt is where the decimal point falls relative to the digit string:
    // value == 0.<digits> * 10^decpt.
    const int decimal_point = exponent + 1;

    size_t written = 0;
    const auto put = [&](char c) { out[written++] = c; };

    if (negative)
        put('-');

    if (decimal_point <= kMinDecimalPoint || decimal_point > kMaxDecimalPoint) {
        // Scientific: to_chars already produced exactly the form we want,
        // including the sign and the at-least-two-digit exponent.
        const size_t body = text.size() - (negative ? 1 : 0);
        std::memcpy(out + written, text.data() + (negative ? 1 : 0), body);
        return written + body;
    }

    if (decimal_point <= 0) {
        // 0.000ddd
        put('0');
        put('.');
        for (int zero = 0; zero < -decimal_point; ++zero)
            put('0');
        std::memcpy(out + written, digits, digit_count);
        written += digit_count;
        return written;
    }

    if (static_cast<size_t>(decimal_point) >= digit_count) {
        // ddd000.0 -- integral, so a fraction is appended to keep it a float.
        std::memcpy(out + written, digits, digit_count);
        written += digit_count;
        for (size_t zero = digit_count; zero < static_cast<size_t>(decimal_point); ++zero)
            put('0');
        put('.');
        put('0');
        return written;
    }

    // dd.ddd
    const size_t whole = static_cast<size_t>(decimal_point);
    std::memcpy(out + written, digits, whole);
    written += whole;
    put('.');
    std::memcpy(out + written, digits + whole, digit_count - whole);
    written += digit_count - whole;
    return written;
}

} // namespace strata::util
