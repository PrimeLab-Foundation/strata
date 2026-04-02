#pragma once

#ifdef __ARM_NEON

#include <arm_neon.h>
#include <cstddef>

namespace strata::simd::neon {
    // Reduce 128-bit mask to find first set byte. Returns 16 if none.
    inline int first_set(uint8x16_t mask) {
        // Narrow to 64-bit pairs, extract as uint64, find first set bit.
        // NEON lacks movemask — use shift + narrow trick.
        static const uint8_t shift_vals[16] = {
            1, 2, 4, 8, 16, 32, 64, 128,
            1, 2, 4, 8, 16, 32, 64, 128
        };
        uint8x16_t shifts = vld1q_u8(shift_vals);
        uint8x16_t bits = vandq_u8(mask, shifts);

        // Sum each 8-byte half into a single byte via pairwise add
        uint8x8_t lo = vget_low_u8(bits);
        uint8x8_t hi = vget_high_u8(bits);

        // Horizontal OR via pairwise adds: 16→8→4→2→1 bytes per half
        lo = vpadd_u8(lo, lo);
        lo = vpadd_u8(lo, lo);
        lo = vpadd_u8(lo, lo);
        hi = vpadd_u8(hi, hi);
        hi = vpadd_u8(hi, hi);
        hi = vpadd_u8(hi, hi);

        uint8_t lo_bits = vget_lane_u8(lo, 0);
        uint8_t hi_bits = vget_lane_u8(hi, 0);

        if (lo_bits != 0) return __builtin_ctz(lo_bits);
        if (hi_bits != 0) return 8 + __builtin_ctz(hi_bits);
        return 16;
    }

    inline const char *find_string_special(const char *cur, const char *end) {
        auto remaining = static_cast<size_t>(end - cur);

        while (remaining >= 16) {
            uint8x16_t chunk = vld1q_u8(reinterpret_cast<const uint8_t *>(cur));

            // Check: c == '"' || c == '\\' || c < 0x20
            uint8x16_t eq_quote = vceqq_u8(chunk, vdupq_n_u8('"'));
            uint8x16_t eq_backslash = vceqq_u8(chunk, vdupq_n_u8('\\'));
            uint8x16_t lt_control = vcltq_u8(chunk, vdupq_n_u8(0x20));

            uint8x16_t hit = vorrq_u8(vorrq_u8(eq_quote, eq_backslash), lt_control);

            int idx = first_set(hit);
            if (idx < 16) return cur + idx;

            cur += 16;
            remaining -= 16;
        }

        // Scalar tail
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

        while (remaining >= 16) {
            uint8x16_t chunk = vld1q_u8(reinterpret_cast<const uint8_t *>(cur));

            // digit: c >= '0' && c <= '9'  ⟹  (c - '0') < 10 as unsigned
            uint8x16_t sub = vsubq_u8(chunk, vdupq_n_u8('0'));
            uint8x16_t is_digit = vcltq_u8(sub, vdupq_n_u8(10));

            // Find first NON-digit
            uint8x16_t not_digit = vmvnq_u8(is_digit);
            int idx = first_set(not_digit);
            if (idx < 16) return cur + idx;

            cur += 16;
            remaining -= 16;
        }

        while (cur < end && static_cast<unsigned>(*cur - '0') < 10)
            ++cur;
        return cur;
    }

    inline const char *skip_ws(const char *cur, const char *end) {
        auto remaining = static_cast<size_t>(end - cur);

        while (remaining >= 16) {
            uint8x16_t chunk = vld1q_u8(reinterpret_cast<const uint8_t *>(cur));

            uint8x16_t is_space = vceqq_u8(chunk, vdupq_n_u8(' '));
            uint8x16_t is_tab = vceqq_u8(chunk, vdupq_n_u8('\t'));
            uint8x16_t is_nl = vceqq_u8(chunk, vdupq_n_u8('\n'));
            uint8x16_t is_cr = vceqq_u8(chunk, vdupq_n_u8('\r'));

            uint8x16_t is_ws = vorrq_u8(vorrq_u8(is_space, is_tab), vorrq_u8(is_nl, is_cr));

            // Find first NON-whitespace
            uint8x16_t not_ws = vmvnq_u8(is_ws);
            int idx = first_set(not_ws);
            if (idx < 16) return cur + idx;

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

    // Load 16 bytes, return bitmask: bit i = 1 iff p[i] is '0'-'9'.
    // Reuses the first_set shift table for byte→bit conversion.
    inline uint16_t digit_mask_16(const char *p) {
        uint8x16_t chunk = vld1q_u8(reinterpret_cast<const uint8_t *>(p));
        uint8x16_t sub = vsubq_u8(chunk, vdupq_n_u8('0'));
        uint8x16_t is_dig = vcltq_u8(sub, vdupq_n_u8(10));

        static const uint8_t shift_vals[16] = {
            1, 2, 4, 8, 16, 32, 64, 128,
            1, 2, 4, 8, 16, 32, 64, 128
        };
        uint8x16_t bits = vandq_u8(is_dig, vld1q_u8(shift_vals));
        uint8x8_t lo = vget_low_u8(bits);
        uint8x8_t hi = vget_high_u8(bits);
        lo = vpadd_u8(lo, lo);
        lo = vpadd_u8(lo, lo);
        lo = vpadd_u8(lo, lo);
        hi = vpadd_u8(hi, hi);
        hi = vpadd_u8(hi, hi);
        hi = vpadd_u8(hi, hi);
        return static_cast<uint16_t>(vget_lane_u8(lo, 0))
               | static_cast<uint16_t>(static_cast<uint16_t>(vget_lane_u8(hi, 0)) << 8);
    }

    // ═══════════════════════════════════════════════════════════
    //  Vectorized 16-digit parsing
    //
    //  Load 16 ASCII digits into a NEON Q register.
    //  Subtract '0', then merge adjacent pairs via multiply + pairwise-add:
    //
    //    16 × u8  digits
    //    → vmull_u8 × [10,1,10,1...] + vpaddlq_u16 → 8 × u16 pairs
    //    → vmulq_u32 × [100,1,100,1] + vpaddlq_u32 → 4 × u32 quads
    //    → extract + scalar 10000× merge             → 2 × u32 octets
    //    → scalar 100000000× merge                   → 1 × u64
    //
    //  Total: 2 vmull, 2 vpaddl, 2 vmul, 4 lane extracts, 4 scalar ops.
    //  Both 8-digit halves processed simultaneously in the pipeline.
    // ═══════════════════════════════════════════════════════════

    inline uint64_t parse_16_digits(const char *p) {
        uint8x16_t raw = vld1q_u8(reinterpret_cast<const uint8_t *>(p));
        uint8x16_t digits = vsubq_u8(raw, vdupq_n_u8('0'));

        uint8x8_t d_hi = vget_low_u8(digits);
        uint8x8_t d_lo = vget_high_u8(digits);

        // Pair weights: d[2i]*10 + d[2i+1]
        const uint8x8_t w_10_1 = {10, 1, 10, 1, 10, 1, 10, 1};

        // Step 1: 8 digits → 4 pairs (per half)
        // vmull_u8: element-wise u8×u8 → u16
        // vpaddlq_u16: adjacent u16 pair → u32 (pairwise add long)
        uint16x8_t prod_hi = vmull_u8(d_hi, w_10_1);
        uint16x8_t prod_lo = vmull_u8(d_lo, w_10_1);
        uint32x4_t pairs_hi = vpaddlq_u16(prod_hi);
        uint32x4_t pairs_lo = vpaddlq_u16(prod_lo);

        // Step 2: 4 pairs → 2 quads (per half)
        const uint32x4_t w_100_1 = {100, 1, 100, 1};
        uint32x4_t scaled_hi = vmulq_u32(pairs_hi, w_100_1);
        uint32x4_t scaled_lo = vmulq_u32(pairs_lo, w_100_1);
        uint64x2_t quads_hi = vpaddlq_u32(scaled_hi);
        uint64x2_t quads_lo = vpaddlq_u32(scaled_lo);

        // Step 3: 2 quads → 1 octet (per half) via scalar
        uint64_t hi = vgetq_lane_u64(quads_hi, 0) * 10000
                      + vgetq_lane_u64(quads_hi, 1);
        uint64_t lo = vgetq_lane_u64(quads_lo, 0) * 10000
                      + vgetq_lane_u64(quads_lo, 1);

        return hi * 100000000ULL + lo;
    }
}

#endif