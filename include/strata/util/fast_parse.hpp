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
 * `strtod`, immune to the process locale's decimal separator. Where the
 * standard library lacks the floating-point overload, @ref from_chars_double
 * provides a strtod_l twin with identical observable behavior.
 */

#include <charconv>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <system_error>

// Floating-point std::from_chars reached libc++ only at LLVM 20; Apple SDKs
// through Xcode 16 ship the integral overloads alone, so those builds take
// the strtod_l twin below. STRATA_FORCE_STRTOD_FALLBACK compiles the twin on
// a machine whose library does have from_chars, so both branches run under
// the same suites locally (the scalar-twin rule applied to a library gap).
#if !defined(STRATA_FORCE_STRTOD_FALLBACK) && defined(__cpp_lib_to_chars) &&                       \
    __cpp_lib_to_chars >= 201611L
#define STRATA_HAS_FP_FROM_CHARS 1
#else
#define STRATA_HAS_FP_FROM_CHARS 0
#endif

#if !STRATA_HAS_FP_FROM_CHARS
#if !defined(__APPLE__)
#error "no floating-point std::from_chars and no Apple strtod_l fallback for this toolchain"
#endif
#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <xlocale.h>
#endif

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

#if !STRATA_HAS_FP_FROM_CHARS
/// Process-lifetime "C" locale for strtod_l; never freed by design. Darwin
/// treats a null locale_t as the C locale, so even a failed newlocale still
/// parses correctly.
[[nodiscard]] inline locale_t c_locale() noexcept {
    static const locale_t locale = newlocale(LC_ALL_MASK, "C", static_cast<locale_t>(nullptr));
    return locale;
}
#endif

} // namespace detail

/// The two std::from_chars_result fields the number call sites consume.
struct FromCharsResult {
    const char* ptr;
    std::errc ec;
};

#if STRATA_HAS_FP_FROM_CHARS

/// Correctly rounded text→double over [first, last), via std::from_chars.
[[nodiscard]] inline FromCharsResult from_chars_double(const char* first, const char* last,
                                                       double& value) noexcept {
    const auto result = std::from_chars(first, last, value);
    return {result.ptr, result.ec};
}

#else

/**
 * strtod_l twin of the from_chars branch, observably identical.
 *
 * Each difference strtod would introduce is neutralized: leading whitespace
 * and '+' are rejected up front (strtod accepts both, from_chars neither);
 * the token is copied and NUL-terminated so the parse can never read past
 * @p last (the input need not be NUL-terminated); the "C" locale is passed
 * explicitly so the process locale's decimal separator cannot leak in; and
 * ERANGE is folded to from_chars semantics — a denormal result (flagged by
 * some libcs) is a representable value and therefore success, only ±infinity
 * and total underflow report result_out_of_range. Hex floats and inf/nan
 * words cannot arise: every caller hands over a span already scanned as
 * sign/digits/point/exponent only. Allocation failure on an oversized token
 * reports not_enough_memory — an error, never a fabricated value.
 */
[[nodiscard]] inline FromCharsResult from_chars_double(const char* first, const char* last,
                                                       double& value) noexcept {
    if (first == last || *first == '+' || *first == ' ' || (*first >= '\t' && *first <= '\r'))
        return {first, std::errc::invalid_argument};

    constexpr size_t kStackCapacity = 127;
    char stack_buffer[kStackCapacity + 1];
    std::unique_ptr<char, void (*)(void*)> heap_buffer(nullptr, std::free);
    const size_t length = static_cast<size_t>(last - first);
    char* token = stack_buffer;
    if (length > kStackCapacity) {
        heap_buffer.reset(static_cast<char*>(std::malloc(length + 1)));
        if (heap_buffer == nullptr)
            return {first, std::errc::not_enough_memory};
        token = heap_buffer.get();
    }
    std::memcpy(token, first, length);
    token[length] = '\0';

    errno = 0;
    char* token_end = nullptr;
    const double parsed = strtod_l(token, &token_end, detail::c_locale());
    if (token_end == token)
        return {first, std::errc::invalid_argument};

    const char* const ptr = first + (token_end - token);
    if (errno == ERANGE && (parsed == HUGE_VAL || parsed == -HUGE_VAL || parsed == 0.0))
        return {ptr, std::errc::result_out_of_range};
    value = parsed;
    return {ptr, std::errc{}};
}

#endif

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
    const bool negative = str[0] == '-';
    if (negative)
        ++pos;

    if (pos >= len || !is_digit(str[pos]))
        return false;

    // The validation scan doubles as the conversion: significant digits
    // accumulate into `mantissa` as they are checked, so the common cases
    // below never re-read the span. `from_chars` re-scanning the token was
    // the previous conversion strategy, and on number-dense documents that
    // second pass was a measurable share of the whole parse.
    uint64_t mantissa = 0;
    size_t significant = 0; // digits accumulated into `mantissa`
    bool truncated = false; // more significant digits than fit; fall back

    // Integer part. A leading zero may only stand alone.
    const size_t int_start = pos;
    if (str[pos] == '0') {
        ++pos;
        if (pos < len && is_digit(str[pos]))
            return false;
    } else {
        while (pos < len && is_digit(str[pos])) {
            if (significant < 19) {
                mantissa = mantissa * 10 + static_cast<uint64_t>(str[pos] - '0');
                ++significant;
            } else {
                truncated = true;
            }
            ++pos;
        }
    }
    const size_t int_digits = pos - int_start;
    const bool int_part_is_zero = (int_digits == 1 && str[int_start] == '0');

    bool is_double = false;
    size_t fraction_digits = 0;
    size_t fraction_leading_zeros = 0;

    if (pos < len && str[pos] == '.') {
        ++pos;
        if (pos >= len || !is_digit(str[pos]))
            return false;
        while (pos < len && is_digit(str[pos])) {
            ++fraction_digits;
            if (mantissa == 0 && str[pos] == '0') {
                // Leading zeros carry no significance; they only shift the
                // exponent, which `fraction_digits` already records.
                ++fraction_leading_zeros;
            } else if (significant < 19) {
                mantissa = mantissa * 10 + static_cast<uint64_t>(str[pos] - '0');
                ++significant;
            } else {
                truncated = true;
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
        // Up to 18 digits always fit int64 (999999999999999999 < 2^63); the
        // 19-digit boundary cases go to from_chars, which also classifies
        // anything larger as BigInt.
        if (!truncated && significant <= 18) {
            out.kind = NumberKind::Int64;
            out.int_value =
                negative ? -static_cast<int64_t>(mantissa) : static_cast<int64_t>(mantissa);
            return true;
        }
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

    // Exact-arithmetic fast path (Clinger): when the mantissa is exactly
    // representable (<= 2^53) and the scale is a power of ten that is itself
    // exact (10^0..10^22 -- 5^22 < 2^53), one IEEE multiply or divide of two
    // exact operands is correctly rounded by definition. This covers every
    // ordinary decimal ("-750.347674"); the digit-heavy and huge-exponent
    // remainder falls through to from_chars, which is exact everywhere.
    const long effective_exponent = exponent - static_cast<long>(fraction_digits);
    if (!truncated && mantissa <= (uint64_t{1} << 53) && effective_exponent >= -22 &&
        effective_exponent <= 22) {
        static constexpr double kPow10[23] = {
            1e0,  1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,  1e8,  1e9,  1e10, 1e11,
            1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19, 1e20, 1e21, 1e22,
        };
        const double magnitude = static_cast<double>(mantissa);
        const double value = effective_exponent < 0 ? magnitude / kPow10[-effective_exponent]
                                                    : magnitude * kPow10[effective_exponent];
        out.double_value = negative ? -value : value;
        return true;
    }

    const auto result = from_chars_double(first, last, out.double_value);
    if (result.ec == std::errc::result_out_of_range) {
        // Outside the representable range in one direction or the other.
        // Decide which from the decimal magnitude: a value only overflows once
        // it is far above 1, and only underflows once it is far below, so the
        // sign of the estimate is unambiguous here.
        const long magnitude = (int_part_is_zero ? -static_cast<long>(fraction_leading_zeros)
                                                 : static_cast<long>(int_digits)) +
                               exponent;
        out.double_value = magnitude > 0 ? std::numeric_limits<double>::infinity() : 0.0;
        if (negative)
            out.double_value = -out.double_value;
        return true;
    }
    if (result.ec != std::errc{} || result.ptr != last)
        return false;
    return true;
}

} // namespace strata::util
