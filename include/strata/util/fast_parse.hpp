#pragma once

/**
 * @file fast_parse.hpp
 * @brief Unified JSON number scanning and conversion.
 *
 * One pass validates the token against JSON's grammar and classifies it; the
 * conversion then runs over the exact span that was validated.
 *
 * Integers are exact at any size. A literal that does not fit `int64_t` is
 * reported as @ref NumberKind::BigInt with its span, leaving the decision of
 * how to represent it to the handler — the DOM widens to double, the Python
 * builder constructs an arbitrary-precision int. The previous implementation
 * accumulated only the first 19 digits and then silently returned that
 * truncation as the value, which is how it mis-parsed 20-digit integers
 * (docs/context/api.md: "do not reproduce").
 *
 * Conversion uses `std::from_chars`, which is correctly rounded and, unlike
 * `strtod`, immune to the process locale's decimal separator.
 */

#include <charconv>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <system_error>

namespace strata::util {

/// How a validated number token should be represented.
enum class NumberKind {
    Int64,  ///< Integral and representable as int64_t; use ParsedNumber::int_value.
    BigInt, ///< Integral but outside int64_t; the caller reads the raw span.
    Double, ///< Has a fraction or an exponent; use ParsedNumber::double_value.
};

/// Outcome of parsing one number token.
struct ParsedNumber {
    NumberKind kind = NumberKind::Int64;
    int64_t int_value = 0;
    double double_value = 0.0;
    size_t consumed = 0; ///< Bytes of input the token occupies.
};

namespace detail {

[[nodiscard]] constexpr bool is_digit(char c) noexcept { return c >= '0' && c <= '9'; }

} // namespace detail

/**
 * Validate and convert one JSON number at the start of @p str.
 *
 * Enforces JSON's grammar exactly: an optional `-`, an integer part with no
 * leading zeros, an optional fraction that must have at least one digit after
 * the point, and an optional exponent that must have at least one digit. Any
 * trailing bytes are left for the caller to reject or consume.
 *
 * @return false if the leading bytes are not a valid JSON number, leaving
 *         @p out untouched.
 */
[[nodiscard]] inline bool parse_number_unified(const char* str, size_t len,
                                               ParsedNumber& out) noexcept {
    using detail::is_digit;

    if (len == 0)
        return false;

    size_t pos = 0;
    if (str[pos] == '-')
        ++pos;

    if (pos >= len || !is_digit(str[pos]))
        return false;

    // Integer part. A leading zero may only stand alone.
    const size_t int_start = pos;
    if (str[pos] == '0') {
        ++pos;
        if (pos < len && is_digit(str[pos]))
            return false;
    } else {
        while (pos < len && is_digit(str[pos]))
            ++pos;
    }
    const size_t int_digits = pos - int_start;
    const bool int_part_is_zero = (int_digits == 1 && str[int_start] == '0');

    bool is_double = false;
    size_t fraction_leading_zeros = 0;
    bool fraction_has_nonzero = false;

    if (pos < len && str[pos] == '.') {
        ++pos;
        if (pos >= len || !is_digit(str[pos]))
            return false;
        while (pos < len && is_digit(str[pos])) {
            if (!fraction_has_nonzero) {
                if (str[pos] == '0')
                    ++fraction_leading_zeros;
                else
                    fraction_has_nonzero = true;
            }
            ++pos;
        }
        is_double = true;
    }

    long exponent = 0;
    if (pos < len && (str[pos] == 'e' || str[pos] == 'E')) {
        ++pos;
        bool exponent_negative = false;
        if (pos < len && (str[pos] == '-' || str[pos] == '+')) {
            exponent_negative = (str[pos] == '-');
            ++pos;
        }
        if (pos >= len || !is_digit(str[pos]))
            return false;
        while (pos < len && is_digit(str[pos])) {
            if (exponent < 1000000) // saturate; the magnitude is all that matters
                exponent = exponent * 10 + (str[pos] - '0');
            ++pos;
        }
        if (exponent_negative)
            exponent = -exponent;
        is_double = true;
    }

    out.consumed = pos;
    const char* const first = str;
    const char* const last = str + pos;

    if (!is_double) {
        const auto result = std::from_chars(first, last, out.int_value);
        if (result.ec == std::errc::result_out_of_range) {
            out.kind = NumberKind::BigInt;
            out.int_value = 0;
            return true;
        }
        if (result.ec != std::errc{} || result.ptr != last)
            return false;
        out.kind = NumberKind::Int64;
        return true;
    }

    out.kind = NumberKind::Double;
    const auto result = std::from_chars(first, last, out.double_value);
    if (result.ec == std::errc::result_out_of_range) {
        // Outside the representable range in one direction or the other.
        // Decide which from the decimal magnitude: a value only overflows once
        // it is far above 1, and only underflows once it is far below, so the
        // sign of the estimate is unambiguous here.
        const long magnitude = (int_part_is_zero ? -static_cast<long>(fraction_leading_zeros)
                                                 : static_cast<long>(int_digits)) +
                               exponent;
        out.double_value = magnitude > 0 ? std::numeric_limits<double>::infinity() : 0.0;
        if (str[0] == '-')
            out.double_value = -out.double_value;
        return true;
    }
    if (result.ec != std::errc{} || result.ptr != last)
        return false;
    return true;
}

} // namespace strata::util
