#pragma once

#ifdef __AVX2__

#include <immintrin.h>
#include <cstddef>

namespace strata::simd::avx2 {

inline const char* find_string_special(const char* cur, const char* end) {
    auto remaining = static_cast<size_t>(end - cur);

    __m256i q_quote     = _mm256_set1_epi8('"');
    __m256i q_backslash = _mm256_set1_epi8('\\');
    __m256i q_control   = _mm256_set1_epi8(0x1F);

    while (remaining >= 32) {
        __m256i chunk = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(cur));

        __m256i eq_q  = _mm256_cmpeq_epi8(chunk, q_quote);
        __m256i eq_bs = _mm256_cmpeq_epi8(chunk, q_backslash);
        __m256i gt_1f = _mm256_cmpgt_epi8(chunk, q_control);
        __m256i lt_20 = _mm256_andnot_si256(gt_1f, _mm256_set1_epi8(static_cast<char>(0xFF)));

        __m256i hit = _mm256_or_si256(_mm256_or_si256(eq_q, eq_bs), lt_20);
        int mask = _mm256_movemask_epi8(hit);
        if (mask != 0) return cur + __builtin_ctz(mask);

        cur += 32;
        remaining -= 32;
    }

    // SSE2 tail for 16-31 remaining bytes
    if (remaining >= 16) {
        __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i*>(cur));
        __m128i eq_q  = _mm_cmpeq_epi8(chunk, _mm_set1_epi8('"'));
        __m128i eq_bs = _mm_cmpeq_epi8(chunk, _mm_set1_epi8('\\'));
        __m128i gt_1f = _mm_cmpgt_epi8(chunk, _mm_set1_epi8(0x1F));
        __m128i lt_20 = _mm_andnot_si128(gt_1f, _mm_set1_epi8(static_cast<char>(0xFF)));
        __m128i hit   = _mm_or_si128(_mm_or_si128(eq_q, eq_bs), lt_20);
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

inline const char* skip_digits(const char* cur, const char* end) {
    auto remaining = static_cast<size_t>(end - cur);

    __m256i q_zero = _mm256_set1_epi8('0');
    __m256i q_nine = _mm256_set1_epi8('9');
    __m256i ones   = _mm256_set1_epi8(static_cast<char>(0xFF));

    while (remaining >= 32) {
        __m256i chunk = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(cur));
        __m256i ge_0 = _mm256_andnot_si256(_mm256_cmpgt_epi8(q_zero, chunk), ones);
        __m256i le_9 = _mm256_andnot_si256(_mm256_cmpgt_epi8(chunk, q_nine), ones);
        __m256i is_digit = _mm256_and_si256(ge_0, le_9);
        int mask = _mm256_movemask_epi8(is_digit);
        if (mask != static_cast<int>(0xFFFFFFFF)) return cur + __builtin_ctz(~mask);
        cur += 32;
        remaining -= 32;
    }

    while (cur < end && static_cast<unsigned>(*cur - '0') < 10)
        ++cur;
    return cur;
}

inline const char* skip_ws(const char* cur, const char* end) {
    auto remaining = static_cast<size_t>(end - cur);

    __m256i q_space = _mm256_set1_epi8(' ');
    __m256i q_tab   = _mm256_set1_epi8('\t');
    __m256i q_nl    = _mm256_set1_epi8('\n');
    __m256i q_cr    = _mm256_set1_epi8('\r');

    while (remaining >= 32) {
        __m256i chunk = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(cur));
        __m256i is_ws = _mm256_or_si256(
            _mm256_or_si256(_mm256_cmpeq_epi8(chunk, q_space), _mm256_cmpeq_epi8(chunk, q_tab)),
            _mm256_or_si256(_mm256_cmpeq_epi8(chunk, q_nl),    _mm256_cmpeq_epi8(chunk, q_cr))
        );
        int mask = _mm256_movemask_epi8(is_ws);
        if (mask != static_cast<int>(0xFFFFFFFF)) return cur + __builtin_ctz(~mask);
        cur += 32;
        remaining -= 32;
    }

    while (cur < end) {
        switch (*cur) {
            case ' ': case '\t': case '\n': case '\r':
                ++cur; continue;
            default:
                return cur;
        }
    }
    return cur;
}

}

#endif