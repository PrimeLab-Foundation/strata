// CMake integration: add this file to STRATA_CORE_SOURCES in CMakeLists.txt.
// Requires: -mavx2 for AVX2, -mavx512f -mavx512bw for AVX-512, or -march=native.

#include "strata/simd/classifier.h"

namespace strata {
namespace simd {

// ============================================================================
// Scalar fallback (always compiled — used by tests and as reference)
// ============================================================================

uint8_t Classifier::classify_scalar(uint8_t c) noexcept {
    uint8_t lo = kLowNibbleTable[c & 0x0F];
    uint8_t hi = kHighNibbleTable[c >> 4];
    return lo & hi;
}

// ============================================================================
// AVX-512BW implementation (64 bytes in one shot)
// ============================================================================

#ifdef STRATA_SIMD_HAS_AVX512

__m512i Classifier::classify64(__m512i input) const noexcept {
    // Broadcast the 16-byte LUTs to all four 128-bit lanes of a 512-bit register.
    // _mm512_broadcast_i32x4 duplicates a 128-bit value across 4 lanes.
    const __m512i low_lut =
        _mm512_broadcast_i32x4(_mm_loadu_si128(reinterpret_cast<const __m128i*>(kLowNibbleTable)));
    const __m512i high_lut =
        _mm512_broadcast_i32x4(_mm_loadu_si128(reinterpret_cast<const __m128i*>(kHighNibbleTable)));

    const __m512i nibble_mask = _mm512_set1_epi8(0x0F);

    // Low nibbles: input & 0x0F
    __m512i lo_nibbles = _mm512_and_si512(input, nibble_mask);

    // High nibbles: (input >> 4) & 0x0F
    // _mm512_srli_epi16 shifts 16-bit lanes; mask cleans up inter-byte garbage.
    __m512i hi_nibbles = _mm512_and_si512(_mm512_srli_epi16(input, 4), nibble_mask);

    // vpshufb: per-lane table lookup (operates on each 128-bit lane independently).
    __m512i lo_result = _mm512_shuffle_epi8(low_lut, lo_nibbles);
    __m512i hi_result = _mm512_shuffle_epi8(high_lut, hi_nibbles);

    // AND: only bytes where both nibble lookups agree produce a non-zero class.
    return _mm512_and_si512(lo_result, hi_result);
}

#endif // STRATA_SIMD_HAS_AVX512

// ============================================================================
// AVX2 implementation (32 bytes)
// ============================================================================

#ifdef STRATA_SIMD_HAS_AVX2

__m256i Classifier::classify32(__m256i input) const noexcept {
    // Duplicate the 16-byte LUTs into both 128-bit lanes (vpshufb is per-lane).
    const __m256i low_lut = _mm256_setr_epi8(
        static_cast<char>(0x80), 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x09, 0x20,
        0x44, 0x28, 0x00, 0x00, static_cast<char>(0x80), 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x08, 0x09, 0x20, 0x44, 0x28, 0x00, 0x00);

    const __m256i high_lut = _mm256_setr_epi8(
        0x08, 0x00, static_cast<char>(0xC2), 0x01, 0x00, 0x24, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x08, 0x00, static_cast<char>(0xC2), 0x01, 0x00, 0x24, 0x00, 0x20,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    const __m256i nibble_mask = _mm256_set1_epi8(0x0F);

    __m256i lo_nibbles = _mm256_and_si256(input, nibble_mask);
    __m256i hi_nibbles = _mm256_and_si256(_mm256_srli_epi16(input, 4), nibble_mask);

    __m256i lo_result = _mm256_shuffle_epi8(low_lut, lo_nibbles);
    __m256i hi_result = _mm256_shuffle_epi8(high_lut, hi_nibbles);

    return _mm256_and_si256(lo_result, hi_result);
}

#endif // STRATA_SIMD_HAS_AVX2

// ============================================================================
// ARM SVE2 implementation (scalable vector — 128 to 2048 bits)
// ============================================================================

#ifdef STRATA_SIMD_HAS_SVE2

svuint8_t Classifier::classify_sve2(svuint8_t input, svbool_t pg) const noexcept {
    // Load the 16-byte LUTs into SVE vectors.  Only the first 16 elements are
    // meaningful; the rest are zero-filled (svld1 with a partial predicate
    // zeroes inactive lanes).
    svbool_t pg16 = svwhilelt_b8(static_cast<uint32_t>(0), static_cast<uint32_t>(16));
    svuint8_t low_lut = svld1_u8(pg16, kLowNibbleTable);
    svuint8_t high_lut = svld1_u8(pg16, kHighNibbleTable);

    // Low nibbles: input & 0x0F
    svuint8_t lo_nibbles = svand_u8_x(pg, input, svdup_u8(0x0F));

    // High nibbles: input >> 4  (vshr fills with zeros from the left)
    svuint8_t hi_nibbles = svlsr_n_u8_x(pg, input, 4);

    // svtbl: table lookup — for each element, result[i] = table[indices[i]].
    // Out-of-range indices (>= VL/8) produce 0.  Since our nibbles are 0–15
    // and the tables have valid entries in positions 0–15, this is correct
    // for any VL >= 128 bits (≥ 16 bytes).
    svuint8_t lo_result = svtbl_u8(low_lut, lo_nibbles);
    svuint8_t hi_result = svtbl_u8(high_lut, hi_nibbles);

    // AND: only bytes where both lookups agree produce a non-zero class.
    return svand_u8_x(pg, lo_result, hi_result);
}

#endif // STRATA_SIMD_HAS_SVE2

// ============================================================================
// ARM NEON implementation (16 bytes)
// ============================================================================

#ifdef STRATA_SIMD_HAS_NEON

uint8x16_t Classifier::classify16(uint8x16_t input) const noexcept {
    const uint8x16_t low_lut = vld1q_u8(kLowNibbleTable);
    const uint8x16_t high_lut = vld1q_u8(kHighNibbleTable);
    const uint8x16_t nibble_mask = vdupq_n_u8(0x0F);

    uint8x16_t lo_nibbles = vandq_u8(input, nibble_mask);
    uint8x16_t hi_nibbles = vshrq_n_u8(input, 4);

    // vqtbl1q_u8: indices >= 16 produce 0 (same semantics as vpshufb).
    uint8x16_t lo_result = vqtbl1q_u8(low_lut, lo_nibbles);
    uint8x16_t hi_result = vqtbl1q_u8(high_lut, hi_nibbles);

    return vandq_u8(lo_result, hi_result);
}

#endif // STRATA_SIMD_HAS_NEON

// ============================================================================
// WebAssembly SIMD128 implementation (16 bytes)
// ============================================================================

#ifdef STRATA_SIMD_HAS_WASM_SIMD

v128_t Classifier::classify16_wasm(v128_t input) const noexcept {
    // wasm_v128_load: load 16 bytes from the lookup tables.
    const v128_t low_lut = wasm_v128_load(kLowNibbleTable);
    const v128_t high_lut = wasm_v128_load(kHighNibbleTable);

    const v128_t nibble_mask = wasm_u8x16_splat(0x0F);

    // Low nibbles: input & 0x0F
    v128_t lo_nibbles = wasm_v128_and(input, nibble_mask);

    // High nibbles: input >> 4 (unsigned byte shift right)
    v128_t hi_nibbles = wasm_u8x16_shr(input, 4);

    // i8x16.swizzle: table lookup — indices >= 16 produce 0 (same as vpshufb).
    v128_t lo_result = wasm_i8x16_swizzle(low_lut, lo_nibbles);
    v128_t hi_result = wasm_i8x16_swizzle(high_lut, hi_nibbles);

    return wasm_v128_and(lo_result, hi_result);
}

#endif // STRATA_SIMD_HAS_WASM_SIMD

// ============================================================================
// RISC-V Vector (RVV 1.0) implementation (scalable vector)
// ============================================================================

#ifdef STRATA_SIMD_HAS_RVV

vuint8m1_t Classifier::classify_rvv(vuint8m1_t input, size_t vl) const noexcept {
    // vluxei8: indexed byte gather — result[i] = base[indices[i]].
    // This directly implements the table lookup: for each byte, load
    // kLowNibbleTable[lo_nibble] and kHighNibbleTable[hi_nibble].
    // Unlike vrgather (which indexes into a vector register), vluxei8
    // indexes into memory, so the 16-byte table size is not a problem
    // regardless of the hardware vector length.

    vuint8m1_t nibble_mask = __riscv_vmv_v_x_u8m1(0x0F, vl);

    // Low nibbles: input & 0x0F
    vuint8m1_t lo_nibbles = __riscv_vand_vv_u8m1(input, nibble_mask, vl);

    // High nibbles: input >> 4
    vuint8m1_t hi_nibbles = __riscv_vsrl_vx_u8m1(input, 4, vl);

    // Gather from the lookup tables using nibble values as byte offsets.
    vuint8m1_t lo_result = __riscv_vluxei8_v_u8m1(kLowNibbleTable, lo_nibbles, vl);
    vuint8m1_t hi_result = __riscv_vluxei8_v_u8m1(kHighNibbleTable, hi_nibbles, vl);

    // AND: same nibble-AND trick as all other backends.
    return __riscv_vand_vv_u8m1(lo_result, hi_result, vl);
}

#endif // STRATA_SIMD_HAS_RVV

} // namespace simd
} // namespace strata
