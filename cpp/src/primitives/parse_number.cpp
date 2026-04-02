// parse_number.cpp — SIMD-validated JSON number parser
//
//   scan_number     → NumberLayout   (validate + classify)
//   convert_digits  → uint64_t       (SWAR/SIMD tiers)
//   make_int        → Result<Number> (sign + overflow)
//   make_float      → Result<Number> (fraction + exponent + pow10)

#include "strata/primitives/parse_number.hpp"
#include "strata/simd/ops.hpp"

#include <climits>
#include <cmath>
#include <optional>

namespace strata {
    namespace {
        inline bool is_digit(char c) { return static_cast<unsigned>(c - '0') < 10; }
        inline int to_digit(char c) { return c - '0'; }

        using Err = ParseError;
        using EC = ErrorCode;

        inline auto fail(EC code, const char *where) {
            return std::unexpected(Err{code, where});
        }


        // ── NumberLayout ──
        // Stores pointers to each segment, not just counts.
        // "Explicit is better than implicit."

        struct NumberLayout {
            const char *int_start;
            const char *frac_start; // null if no fraction
            const char *exp_start; // null if no exponent
            const char *end;
            uint8_t n_int;
            uint8_t n_frac;
            uint8_t n_exp;
            bool negative;
            bool exp_negative;

            [[nodiscard]] bool is_float() const { return frac_start || exp_start; }
        };


        // ── SIMD fast path: try to classify in one 16-byte load ──
        // "Flat is better than nested."
        // Returns nullopt if number extends past the 16-byte window.

        inline int count_mask_digits(uint16_t dm, int from) {
            unsigned shifted = static_cast<unsigned>(~(dm >> from)) | (0x10000u >> from);
            return __builtin_ctz(shifted);
        }

        inline std::optional<NumberLayout> try_scan_short(
            const char *cur, uint16_t dm, bool negative) {
            int n_int = __builtin_ctz(static_cast<unsigned>(~dm) | 0x10000u);
            if (n_int >= 16)
                return {};

            int pos = n_int;
            const char *frac_start = nullptr;
            const char *exp_start = nullptr;
            uint8_t n_frac = 0;
            uint8_t n_exp = 0;
            bool exp_neg = false;

            // Fraction
            if (cur[pos] == '.') {
                pos++;
                if (pos >= 16) return {};
                n_frac = static_cast<uint8_t>(count_mask_digits(dm, pos));
                if (n_frac == 0) return {};
                frac_start = cur + pos;
                pos += n_frac;
                if (pos >= 16) return {};
            }

            // Exponent
            if (cur[pos] == 'e' || cur[pos] == 'E') {
                pos++;
                if (pos >= 16) return {};
                if (cur[pos] == '+' || cur[pos] == '-') {
                    exp_neg = (cur[pos] == '-');
                    pos++;
                    if (pos >= 16) return {};
                }
                n_exp = static_cast<uint8_t>(count_mask_digits(dm, pos));
                if (n_exp == 0) return {};
                exp_start = cur + pos;
                pos += n_exp;
                if (pos >= 16) return {};
            }

            return NumberLayout{
                cur, frac_start, exp_start, cur + pos,
                static_cast<uint8_t>(n_int), n_frac, n_exp,
                negative, exp_neg
            };
        }


        // ── Fallback: skip_digits per segment ──
        // "Simple is better than complex."

        inline Result<NumberLayout> scan_slow(
            const char *cur, const char *end, bool negative, const char *start) {
            const char *int_start = cur;
            const char *int_end = simd::skip_digits(cur, end);
            int n_int = static_cast<int>(int_end - cur);
            cur = int_end;

            const char *frac_start = nullptr;
            uint8_t n_frac = 0;

            if (cur < end && *cur == '.') {
                cur++;
                if (cur >= end || !is_digit(*cur))
                    return fail(EC::InvalidNumber, start);
                frac_start = cur;
                const char *frac_end = simd::skip_digits(cur, end);
                n_frac = static_cast<uint8_t>(frac_end - cur);
                cur = frac_end;
            }

            const char *exp_start = nullptr;
            uint8_t n_exp = 0;
            bool exp_neg = false;

            if (cur < end && (*cur == 'e' || *cur == 'E')) {
                cur++;
                if (cur < end && (*cur == '+' || *cur == '-')) {
                    exp_neg = (*cur == '-');
                    cur++;
                }
                if (cur >= end || !is_digit(*cur))
                    return fail(EC::InvalidNumber, start);
                exp_start = cur;
                const char *exp_end = simd::skip_digits(cur, end);
                n_exp = static_cast<uint8_t>(exp_end - cur);
                cur = exp_end;
            }

            return Parsed<NumberLayout>{
                NumberLayout{
                    int_start, frac_start, exp_start, cur,
                    static_cast<uint8_t>(n_int > 255 ? 255 : n_int), n_frac, n_exp,
                    negative, exp_neg
                },
                cur
            };
        }


        // ── scan_number: validate + classify ──
        // "There should be one obvious way to do it."

        inline Result<NumberLayout> scan_number(const char *cur, const char *end) {
            const char *start = cur;

            bool negative = (*cur == '-');
            if (negative && ++cur >= end)
                return fail(EC::UnexpectedEnd, start);

            if (!is_digit(*cur))
                return fail(EC::InvalidNumber, start);

            if (*cur == '0' && cur + 1 < end && is_digit(cur[1]))
                return fail(EC::LeadingZero, cur);

            // Try SIMD fast path
            if (static_cast<size_t>(end - cur) >= 16) {
                uint16_t dm = simd::digit_mask_16(cur);
                auto layout = try_scan_short(cur, dm, negative);

                if (layout)
                    return Parsed<NumberLayout>{*layout, layout->end};

                // Fast path returned nullopt.
                // Check if it was a grammar error (trailing dot / empty exponent)
                // by peeking at where we stopped.
                int n_int = __builtin_ctz(static_cast<unsigned>(~dm) | 0x10000u);
                if (n_int < 16) {
                    int pos = n_int;
                    if (cur[pos] == '.') {
                        pos++;
                        if (pos < 16 && !is_digit(cur[pos]))
                            return fail(EC::InvalidNumber, start);
                    }
                }
            }

            return scan_slow(cur, end, negative, start);
        }


        // ── convert_digits: ASCII → uint64_t ──
        // "Special cases aren't special enough to break the rules."

        inline uint64_t gpr_parse(const char *p, int n) {
            switch (n) {
                case 8: return simd::swar_8(p);
                case 7: return simd::swar_4(p) * 1000ULL
                               + to_digit(p[4]) * 100 + to_digit(p[5]) * 10 + to_digit(p[6]);
                case 6: return simd::swar_4(p) * 100ULL + to_digit(p[4]) * 10 + to_digit(p[5]);
                case 5: return simd::swar_4(p) * 10ULL + to_digit(p[4]);
                case 4: return simd::swar_4(p);
                case 3: return to_digit(p[0]) * 100 + to_digit(p[1]) * 10 + to_digit(p[2]);
                case 2: return to_digit(p[0]) * 10 + to_digit(p[1]);
                case 1: return to_digit(p[0]);
                default: return 0;
            }
        }

        inline uint64_t convert_digits(const char *p, int n) {
            static constexpr uint64_t POW10[] = {
                1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000,
                1000000000, 10000000000
            };

            if (n <= 8)
                return gpr_parse(p, n);

            if (n <= 16) {
                if (n == 16)
                    return simd::parse_16_digits(p);
                return simd::swar_8(p) * POW10[n - 8] + gpr_parse(p + 8, n - 8);
            }

            // 17-19 digits: swar_8 + swar_8 + gpr_parse
            int tail = n - 16;
            return simd::swar_8(p) * (100000000ULL * POW10[tail])
                   + simd::swar_8(p + 8) * POW10[tail]
                   + gpr_parse(p + 16, tail);
        }


        // ── make_int ──
        // "Errors should never pass silently."

        inline Result<Number> make_int(uint64_t val, bool negative,
                                       const char *rest, const char *start) {
            uint64_t limit = negative
                                 ? static_cast<uint64_t>(INT64_MAX) + 1
                                 : static_cast<uint64_t>(INT64_MAX);

            if (val > limit)
                return fail(EC::NumberOverflow, start);

            auto ival = static_cast<int64_t>(negative ? (~val + 1) : val);
            return Parsed<Number>{ival, rest};
        }


        // ── pow10 scaling ──
        // "There should be one obvious way to do it."

        constexpr int MAX_EXACT_POW10 = 22;
        constexpr int MAX_SIG_DIGITS = 19;

        constexpr double EXACT_POW10[MAX_EXACT_POW10 + 1] = {
            1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7,
            1e8, 1e9, 1e10, 1e11, 1e12, 1e13, 1e14, 1e15,
            1e16, 1e17, 1e18, 1e19, 1e20, 1e21, 1e22
        };

        inline double scale_pow10(double d, int exp) {
            if (exp == 0) return d;

            bool neg = (exp < 0);
            int rem = neg ? -exp : exp;

            if (rem <= MAX_EXACT_POW10)
                return neg ? d / EXACT_POW10[rem] : d * EXACT_POW10[rem];

            auto step = [&](int n) {
                d = neg ? d / EXACT_POW10[n] : d * EXACT_POW10[n];
            };

            while (rem > MAX_EXACT_POW10) {
                step(MAX_EXACT_POW10);
                rem -= MAX_EXACT_POW10;
            }
            step(rem);
            return d;
        }


        // ── make_float ──
        // "Readability counts."

        inline int parse_exponent(const char *p, int n) {
            int val = 0;
            for (int i = 0; i < n; i++) {
                val = val * 10 + to_digit(p[i]);
                if (val > 999) val = 999;
            }
            return val;
        }

        inline Result<Number> make_float(const NumberLayout &lay) {
            uint64_t sig = 0;
            int sig_digits = 0;
            int exp10 = 0;

            // Integer digits
            if (lay.n_int <= 16) {
                sig = convert_digits(lay.int_start, lay.n_int);
                sig_digits = lay.n_int;
            } else {
                const char *p = lay.int_start;
                const char *stop = p + lay.n_int;
                while (p < stop && sig_digits < MAX_SIG_DIGITS) {
                    sig = sig * 10 + static_cast<uint64_t>(to_digit(*p++));
                    ++sig_digits;
                }
                exp10 += static_cast<int>(stop - p);
            }

            // Fraction digits
            if (lay.frac_start) {
                const char *p = lay.frac_start;
                int remaining = lay.n_frac;
                while (remaining > 0 && sig_digits < MAX_SIG_DIGITS) {
                    sig = sig * 10 + static_cast<uint64_t>(to_digit(*p++));
                    ++sig_digits;
                    --exp10;
                    --remaining;
                }
            }

            // Exponent
            if (lay.exp_start) {
                int e = parse_exponent(lay.exp_start, lay.n_exp);
                exp10 += lay.exp_negative ? -e : e;
            }

            double val = scale_pow10(static_cast<double>(sig), exp10);
            if (lay.negative) val = -val;
            return Parsed<Number>{val, lay.end};
        }
    } // anonymous namespace


    // ── parse<Number> ──
    // "If the implementation is hard to explain, it's a bad idea."

    template<>
    Result<Number> parse<Number>(const char *cur, const char *end) {
        if (cur >= end)
            return fail(EC::UnexpectedEnd, cur);

        auto scan = scan_number(cur, end);
        if (!scan)
            return std::unexpected(scan.error());

        const auto &lay = scan->value;

        if (lay.is_float())
            return make_float(lay);

        if (lay.n_int > 19)
            return fail(EC::NumberOverflow, cur);

        uint64_t val = convert_digits(lay.int_start, lay.n_int);

        if (lay.n_int <= 8) {
            auto ival = static_cast<int64_t>(lay.negative ? (~val + 1) : val);
            return Parsed<Number>{ival, lay.end};
        }

        return make_int(val, lay.negative, lay.end, cur);
    }
}
