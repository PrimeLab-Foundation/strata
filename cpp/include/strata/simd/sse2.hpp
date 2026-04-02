#pragma once

#ifdef __SSE2__

#include <emmintrin.h>
#include <cstddef>

namespace strata::simd::sse2 {
    inline const char *find_string_special(const char *cur, const char *end) {
        auto remaining = static_cast<size_t>(end - cur);

        __m128i q_quote = _mm_set1_epi8('"');
        __m128i q_backslash = _mm_set1_epi8('\\');
        __m128i q_control = _mm_set1_epi8(0x1F);

        while (remaining >= 16) {
            __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i *>(cur));

            // c == '"' || c == '\\' || c < 0x20
            // SSE2 has no unsigned less-than, so: c < 0x20  ⟹  (c & ~0x1F) == 0 AND c != 0x20+
            // Simpler: use saturating subtract. c < 0x20 ⟹ (0x1F - c) >= 0 unsigned
            // Actually: signed compare trick. Treat as signed bytes:
            // c < 0x20 ⟹ c <= 0x1F ⟹ _mm_cmplt_epi8 is signed, but all values
            // 0x00-0x1F are positive in signed interpretation, so it works.
            __m128i eq_q = _mm_cmpeq_epi8(chunk, q_quote);
            __m128i eq_bs = _mm_cmpeq_epi8(chunk, q_backslash);

            // For < 0x20: compare (chunk - 1) < 0x1F as signed, or simpler:
            // max of chunk and 0x1F. If chunk < 0x20, then min(chunk, 0x1F) == chunk
            // Use: ~(chunk > 0x1F) = chunk <= 0x1F. _mm_cmpgt_epi8 is signed.
            // Since 0x00-0x1F are 0-31 in signed, and 0x1F = 31:
            // chunk > 0x1F means chunk > 31 signed, but bytes 0x80-0xFF are negative!
            // Those would NOT match > 31, which is correct (they're valid UTF-8 bytes).
            __m128i gt_1f = _mm_cmpgt_epi8(chunk, q_control);
            __m128i lt_20 = _mm_andnot_si128(gt_1f, _mm_set1_epi8(static_cast<char>(0xFF)));

            __m128i hit = _mm_or_si128(_mm_or_si128(eq_q, eq_bs), lt_20);
            int mask = _mm_movemask_epi8(hit);
            if (mask != 0) return cur + __builtin_ctz(mask);

            cur += 16;
            remaining -= 16;
        }

        while (cur < end) {
            char c = *cur;
            if (c == '"' || c == '\\' || static_cast<unsigned char>(c) < 0x20)
                return cur;
            ++cur;
        }
        return end;
    }

    inline const char *skip_digits(const char *cur, const char *end) {
        auto remaining = static_cast<size_t>(end - cur);

        __m128i q_zero = _mm_set1_epi8('0');

        while (remaining >= 16) {
            __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i *>(cur));

            // (c - '0') < 10 unsigned. Use: subtract '0', then check < 10.
            // Since _mm_cmplt is signed, and results of sub are 0-9 for digits,
            // 246-255 for chars below '0', and 10-207 for chars above '9':
            // Actually problematic for signed. Instead:
            // is_digit = (c >= '0') AND (c <= '9')
            // c >= '0' ⟹ NOT(c < '0') ⟹ NOT(_mm_cmplt_epi8(c, '0'))
            // c <= '9' ⟹ NOT(c > '9') ⟹ NOT(_mm_cmpgt_epi8(c, '9'))
            // But signed comparison means chars > 0x7F are "negative" < '0'. Safe: they're not digits.
            __m128i ge_0 = _mm_andnot_si128(_mm_cmplt_epi8(chunk, q_zero), _mm_set1_epi8(static_cast<char>(0xFF)));
            __m128i le_9 = _mm_andnot_si128(_mm_cmpgt_epi8(chunk, _mm_set1_epi8('9')),
                                            _mm_set1_epi8(static_cast<char>(0xFF)));
            __m128i is_digit = _mm_and_si128(ge_0, le_9);

            // Find first NON-digit
            int mask = _mm_movemask_epi8(is_digit);
            if (mask != 0xFFFF) return cur + __builtin_ctz(~mask);

            cur += 16;
            remaining -= 16;
        }

        while (cur < end && static_cast<unsigned>(*cur - '0') < 10)
            ++cur;
        return cur;
    }

    inline const char *skip_ws(const char *cur, const char *end) {
        auto remaining = static_cast<size_t>(end - cur);

        __m128i q_space = _mm_set1_epi8(' ');
        __m128i q_tab = _mm_set1_epi8('\t');
        __m128i q_nl = _mm_set1_epi8('\n');
        __m128i q_cr = _mm_set1_epi8('\r');

        while (remaining >= 16) {
            __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i *>(cur));

            __m128i is_ws = _mm_or_si128(
                _mm_or_si128(_mm_cmpeq_epi8(chunk, q_space), _mm_cmpeq_epi8(chunk, q_tab)),
                _mm_or_si128(_mm_cmpeq_epi8(chunk, q_nl), _mm_cmpeq_epi8(chunk, q_cr))
            );

            int mask = _mm_movemask_epi8(is_ws);
            if (mask != 0xFFFF) return cur + __builtin_ctz(~mask);

            cur += 16;
            remaining -= 16;
        }

        while (cur < end) {
            switch (*cur) {
                case ' ':
                case '\t':
                case '\n':
                case '\r':
                    ++cur;
                    continue;
                default:
                    return cur;
            }
        }
        return cur;
    }

    inline uint16_t digit_mask_16(const char *p) {
        __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i *>(p));
        __m128i q_zero = _mm_set1_epi8('0');
        __m128i q_nine = _mm_set1_epi8('9');
        __m128i ge_0 = _mm_andnot_si128(
            _mm_cmplt_epi8(chunk, q_zero),
            _mm_set1_epi8(static_cast<char>(0xFF)));
        __m128i le_9 = _mm_andnot_si128(
            _mm_cmpgt_epi8(chunk, q_nine),
            _mm_set1_epi8(static_cast<char>(0xFF)));
        return static_cast<uint16_t>(_mm_movemask_epi8(_mm_and_si128(ge_0, le_9)));
    }
}

#endif