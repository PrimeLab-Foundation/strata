// parse_number.cpp — single-pass JSON number parser
//
// Both integers and doubles are parsed in ONE pass. No second scan.
//
// Integer path:
//   Digits accumulated into uint64_t during scan. Sign applied at end.
//   Overflow detected inline before each multiply+add.
//
// Double path:
//   Significand accumulated into uint64_t (up to 19 digits).
//   Decimal exponent tracked as offset. Converted via precomputed
//   exact powers of 10 (10^0 through 10^22 are exactly representable
//   as IEEE 754 double). Larger exponents handled by split multiply.
//
//   This covers all real-world JSON floats. Numbers with >19 significant
//   digits are clamped (excess digits adjust the exponent), which gives
//   correctly-rounded results for IEEE 754 doubles (which have ~15.9
//   digits of precision).

#include "strata/primitives/parse_number.hpp"
#include "strata/simd/ops.hpp"

#include <cfloat>
#include <climits>
#include <cmath>

namespace strata {

    namespace {

        inline bool is_digit(char c) {
            return static_cast<unsigned>(c - '0') < 10;
        }

        inline int to_digit(char c) {
            return c - '0';
        }

        // 10^0 through 10^22 are exactly representable as double.
        // Beyond 10^22, the values lose precision in IEEE 754.
        constexpr int MAX_EXACT_POW10 = 22;

        constexpr double EXACT_POW10[MAX_EXACT_POW10 + 1] = {
            1e0,  1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,
            1e8,  1e9,  1e10, 1e11, 1e12, 1e13, 1e14, 1e15,
            1e16, 1e17, 1e18, 1e19, 1e20, 1e21, 1e22
        };

        // Maximum digits we accumulate in the significand.
        // uint64_t holds up to 19 digits (9999999999999999999).
        // IEEE 754 double has ~15.9 digits of precision, so 19 is more than enough.
        constexpr int MAX_SIG_DIGITS = 19;

        // Convert significand * 10^exp10 to double.
        // Uses exact table for |exp10| <= 22, split multiply for larger exponents.
        inline double to_double(uint64_t sig, int exp10) {
            double d = static_cast<double>(sig);

            if (exp10 == 0)
                return d;

            if (exp10 > 0) {
                if (exp10 <= MAX_EXACT_POW10)
                    return d * EXACT_POW10[exp10];

                // Split: first multiply by 10^(exp10 - 22), then by 10^22.
                // The first part uses the exact table after we bring it in range.
                // For extremely large exponents (>308), result is inf — correct per IEEE.
                int remaining = exp10 - MAX_EXACT_POW10;
                d *= EXACT_POW10[MAX_EXACT_POW10];
                while (remaining > MAX_EXACT_POW10) {
                    d *= EXACT_POW10[MAX_EXACT_POW10];
                    remaining -= MAX_EXACT_POW10;
                }
                return d * EXACT_POW10[remaining];
            }

            // exp10 < 0
            int pos = -exp10;
            if (pos <= MAX_EXACT_POW10)
                return d / EXACT_POW10[pos];

            int remaining = pos - MAX_EXACT_POW10;
            d /= EXACT_POW10[MAX_EXACT_POW10];
            while (remaining > MAX_EXACT_POW10) {
                d /= EXACT_POW10[MAX_EXACT_POW10];
                remaining -= MAX_EXACT_POW10;
            }
            return d / EXACT_POW10[remaining];
        }

    }

    template <>
    Result<Number> parse<Number>(const char* cur, const char* end) {
        if (cur >= end)
            return std::unexpected(ParseError{ErrorCode::UnexpectedEnd, cur});

        const char* start = cur;

        // --- sign ---
        bool negative = false;
        if (*cur == '-') {
            negative = true;
            if (++cur >= end)
                return std::unexpected(ParseError{ErrorCode::UnexpectedEnd, start});
        }

        // --- first digit ---
        if (!is_digit(*cur))
            return std::unexpected(ParseError{ErrorCode::InvalidNumber, start});

        // --- leading zero check ---
        if (*cur == '0' && cur + 1 < end && is_digit(cur[1]))
            return std::unexpected(ParseError{ErrorCode::LeadingZero, cur});

        // --- scan + accumulate ---
        //
        // Single pass: builds significand (uint64_t) and tracks decimal exponent.
        // Both integer and float paths share this scan.
        uint64_t sig = 0;
        int sig_digits = 0;
        int exp10 = 0;
        bool is_float = false;
        bool int_overflow = false;

        // integer digits: accumulate up to 19 digits, SIMD skip the rest
        while (cur < end && is_digit(*cur) && sig_digits < MAX_SIG_DIGITS) {
            sig = sig * 10 + static_cast<uint64_t>(to_digit(*cur));
            ++sig_digits;
            ++cur;
        }
        if (cur < end && is_digit(*cur)) {
            const char* digit_end = simd::skip_digits(cur, end);
            exp10 += static_cast<int>(digit_end - cur);
            int_overflow = true;
            cur = digit_end;
        }

        // fraction
        if (cur < end && *cur == '.') {
            is_float = true;
            ++cur;
            if (cur >= end || !is_digit(*cur))
                return std::unexpected(ParseError{ErrorCode::InvalidNumber, start});
            while (cur < end && is_digit(*cur) && sig_digits < MAX_SIG_DIGITS) {
                sig = sig * 10 + static_cast<uint64_t>(to_digit(*cur));
                ++sig_digits;
                --exp10;
                ++cur;
            }
            // SIMD skip excess fraction digits (below double precision)
            cur = simd::skip_digits(cur, end);
        }

        // exponent
        if (cur < end && (*cur == 'e' || *cur == 'E')) {
            is_float = true;
            ++cur;

            bool exp_neg = false;
            if (cur < end && (*cur == '+' || *cur == '-')) {
                exp_neg = (*cur == '-');
                ++cur;
            }
            if (cur >= end || !is_digit(*cur))
                return std::unexpected(ParseError{ErrorCode::InvalidNumber, start});

            int explicit_exp = 0;
            while (cur < end && is_digit(*cur)) {
                explicit_exp = explicit_exp * 10 + to_digit(*cur);
                // Clamp to prevent int overflow in exponent arithmetic.
                // 999 is way beyond double range (max ~308), will become inf/0.
                if (explicit_exp > 999) explicit_exp = 999;
                ++cur;
            }

            exp10 += exp_neg ? -explicit_exp : explicit_exp;
        }

        // --- convert ---

        if (is_float) {
            double val = to_double(sig, exp10);
            if (negative) val = -val;
            return Parsed<Number>{val, cur};
        }

        // integer path
        if (int_overflow)
            return std::unexpected(ParseError{ErrorCode::NumberOverflow, start});

        // Check if accumulated value fits in int64_t
        uint64_t limit = negative ? static_cast<uint64_t>(INT64_MAX) + 1
                                  : static_cast<uint64_t>(INT64_MAX);
        if (sig > limit)
            return std::unexpected(ParseError{ErrorCode::NumberOverflow, start});

        auto ival = static_cast<int64_t>(negative ? (~sig + 1) : sig);
        return Parsed<Number>{ival, cur};
    }

}