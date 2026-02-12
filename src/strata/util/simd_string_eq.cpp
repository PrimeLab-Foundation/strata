#include "strata/util/simd_string.hpp"

#include <cstring>

// Detect SIMD support for small string comparisons
#if (defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)) && \
    (defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2))
#define STRATA_HAS_SSE2 1
#include <emmintrin.h>
#elif defined(__ARM_NEON) || defined(__aarch64__)
#define STRATA_HAS_NEON 1
#include <arm_neon.h>
#endif

namespace strata {
namespace util {

bool simd_string_eq(std::string_view a, std::string_view b) {
    if (a.size() != b.size()) {
        return false;
    }
    const size_t len = a.size();
    if (len == 0) {
        return true;
    }
    if (len > 16) {
        return std::memcmp(a.data(), b.data(), len) == 0;
    }

#if defined(STRATA_HAS_SSE2)
    alignas(16) unsigned char a_buf[16] = {};
    alignas(16) unsigned char b_buf[16] = {};
    std::memcpy(a_buf, a.data(), len);
    std::memcpy(b_buf, b.data(), len);

    __m128i va = _mm_load_si128(reinterpret_cast<const __m128i*>(a_buf));
    __m128i vb = _mm_load_si128(reinterpret_cast<const __m128i*>(b_buf));
    __m128i cmp = _mm_cmpeq_epi8(va, vb);
    return _mm_movemask_epi8(cmp) == 0xFFFF;
#elif defined(STRATA_HAS_NEON)
    alignas(16) unsigned char a_buf[16] = {};
    alignas(16) unsigned char b_buf[16] = {};
    std::memcpy(a_buf, a.data(), len);
    std::memcpy(b_buf, b.data(), len);

    uint8x16_t va = vld1q_u8(a_buf);
    uint8x16_t vb = vld1q_u8(b_buf);
    uint8x16_t cmp = vceqq_u8(va, vb);
    uint64x2_t cmp64 = vreinterpretq_u64_u8(cmp);
    return (vgetq_lane_u64(cmp64, 0) == 0xFFFFFFFFFFFFFFFFULL) &&
           (vgetq_lane_u64(cmp64, 1) == 0xFFFFFFFFFFFFFFFFULL);
#else
    return std::memcmp(a.data(), b.data(), len) == 0;
#endif
}

} // namespace util
} // namespace strata
