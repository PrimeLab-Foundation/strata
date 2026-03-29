#pragma once

// Compile-time SIMD backend selection.
// Priority: AVX2 > SSE2 > NEON > scalar.
// All backends expose the same three functions.
// Parsers include only this file.

#include "strata/simd/scalar.hpp"

#if defined(__AVX2__)
    #include "strata/simd/avx2.hpp"
#elif defined(__SSE2__)
    #include "strata/simd/sse2.hpp"
#elif defined(__ARM_NEON)
    #include "strata/simd/neon.hpp"
#endif

namespace strata::simd {

inline const char* find_string_special(const char* cur, const char* end) {
#if defined(__AVX2__)
    return avx2::find_string_special(cur, end);
#elif defined(__SSE2__)
    return sse2::find_string_special(cur, end);
#elif defined(__ARM_NEON)
    return neon::find_string_special(cur, end);
#else
    return scalar::find_string_special(cur, end);
#endif
}

inline const char* skip_digits(const char* cur, const char* end) {
#if defined(__AVX2__)
    return avx2::skip_digits(cur, end);
#elif defined(__SSE2__)
    return sse2::skip_digits(cur, end);
#elif defined(__ARM_NEON)
    return neon::skip_digits(cur, end);
#else
    return scalar::skip_digits(cur, end);
#endif
}

inline const char* skip_ws(const char* cur, const char* end) {
#if defined(__AVX2__)
    return avx2::skip_ws(cur, end);
#elif defined(__SSE2__)
    return sse2::skip_ws(cur, end);
#elif defined(__ARM_NEON)
    return neon::skip_ws(cur, end);
#else
    return scalar::skip_ws(cur, end);
#endif
}

}