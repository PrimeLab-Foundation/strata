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
    // ═══════════════════════════════════════════════════════════
    //  SWAR digit parsing (GPR — available on all platforms)
    // ═══════════════════════════════════════════════════════════
    //
    // Treat a 64-bit register as 8 parallel byte lanes.
    // ASCII '0'-'9' → mask with 0x0F → digit values 0-9.
    // Multiply-shift chain merges adjacent pairs:
    //
    //   "12345678" → 0x0807060504030201 (after & 0x0F)
    //   × 0x0A01     >> 8  → [12, 34, 56, 78]    (4 × 16-bit)
    //   × 0x640001   >> 16 → [1234, 5678]         (2 × 32-bit)
    //   × 0x271000000001 >> 32 → [12345678]       (1 × 32-bit)
    //
    // Magic constants:
    //   2561           = 10 × 256 + 1
    //   6553601        = 100 × 65536 + 1
    //   42949672960001 = 10000 × 2^32 + 1

    inline uint32_t swar_8(const char *p) {
        uint64_t v;
        __builtin_memcpy(&v, p, 8);
        v &= 0x0F0F0F0F0F0F0F0FULL;
        v = (v * 2561ULL) >> 8;
        v &= 0x00FF00FF00FF00FFULL;
        v = (v * 6553601ULL) >> 16;
        v &= 0x0000FFFF0000FFFFULL;
        v = (v * 42949672960001ULL) >> 32;
        return static_cast<uint32_t>(v);
    }

    inline uint32_t swar_4(const char *p) {
        uint32_t v;
        __builtin_memcpy(&v, p, 4);
        uint64_t w = v & 0x0F0F0F0FULL;
        w = (w * 2561ULL) >> 8;
        w &= 0x00FF00FFULL;
        w = (w * 6553601ULL) >> 16;
        return static_cast<uint32_t>(w & 0xFFFF);
    }

    // ═══════════════════════════════════════════════════════════
    //  Literal matching via integer load
    // ═══════════════════════════════════════════════════════════

    inline uint32_t load4(const char *p) {
        uint32_t v;
        __builtin_memcpy(&v, p, 4);
        return v;
    }

    constexpr uint32_t pack4(const char s[4]) {
        return static_cast<uint32_t>(static_cast<unsigned char>(s[0]))
               | static_cast<uint32_t>(static_cast<unsigned char>(s[1])) << 8
               | static_cast<uint32_t>(static_cast<unsigned char>(s[2])) << 16
               | static_cast<uint32_t>(static_cast<unsigned char>(s[3])) << 24;
    }

    inline bool match4(const char *cur, const char *end, const char lit[4]) {
        return static_cast<size_t>(end - cur) >= 4 && load4(cur) == pack4(lit);
    }

    inline bool match5(const char *cur, const char *end, const char lit[5]) {
        return static_cast<size_t>(end - cur) >= 5 && load4(cur) == pack4(lit) && cur[4] == lit[4];
    }

    inline const char *find_string_special(const char *cur, const char *end) {
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

    inline const char *skip_digits(const char *cur, const char *end) {
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

    inline const char *skip_ws(const char *cur, const char *end) {
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

    // ═══════════════════════════════════════════════════════════
    //  Vectorized digit parsing — exactly 16 ASCII digits → uint64_t
    //
    //  NEON: true vectorized (vmull + vpaddl chain)
    //  x86/scalar: two SWAR-8 passes (still faster than scalar loop)
    // ═══════════════════════════════════════════════════════════

    inline uint64_t parse_16_digits(const char *p) {
#if defined(__ARM_NEON)
        return neon::parse_16_digits(p);
#else
        return static_cast<uint64_t>(swar_8(p)) * 100000000ULL + swar_8(p + 8);
#endif
    }

    // ═══════════════════════════════════════════════════════════
    //  digit_mask_16: load 16 bytes, return bitmask
    //
    //  Bit i of result = 1 iff p[i] is ASCII '0'-'9'.
    //  Caller must ensure at least 16 readable bytes at p.
    //
    //  NEON: vcltq + pairwise-reduce to uint16
    //  SSE2: range compare + movemask
    //  Scalar: 16-iteration loop
    // ═══════════════════════════════════════════════════════════

    inline uint16_t digit_mask_16(const char *p) {
#if defined(__AVX2__)
        return avx2::digit_mask_16(p);
#elif defined(__SSE2__)
        return sse2::digit_mask_16(p);
#elif defined(__ARM_NEON)
        return neon::digit_mask_16(p);
#else
        return scalar::digit_mask_16(p);
#endif
    }
}