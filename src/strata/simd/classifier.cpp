// CMake integration: add this file to STRATA_CORE_SOURCES in CMakeLists.txt.
// Requires: -mavx2 on GCC/Clang for AVX2 path (or -march=native).

#include "strata/simd/classifier.h"

namespace strata {
namespace simd {

// ============================================================================
// Scalar fallback
// ============================================================================

uint8_t Classifier::classify_scalar(uint8_t c) noexcept {
    // Mirror the SIMD lookup-table logic using the same bit encoding.
    uint8_t lo = kLowNibbleTable[c & 0x0F];
    uint8_t hi = kHighNibbleTable[c >> 4];
    return lo & hi;
}

// ============================================================================
// AVX2 implementation
// ============================================================================

#ifdef STRATA_SIMD_HAS_AVX2

__m256i Classifier::classify32(__m256i input) const noexcept {
    // Load both 16-byte lookup tables into 256-bit registers by duplicating
    // the 16-byte table into both 128-bit lanes (vpshufb operates per-lane).
    const __m256i low_lut = _mm256_setr_epi8(
        // Lane 0 (bytes 0-15): kLowNibbleTable[0..15]
        static_cast<char>(0x80), 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x09, 0x20,
        0x44, 0x28, 0x00, 0x00,
        // Lane 1 (bytes 16-31): same table duplicated
        static_cast<char>(0x80), 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x09, 0x20,
        0x44, 0x28, 0x00, 0x00);

    const __m256i high_lut = _mm256_setr_epi8(
        // Lane 0: kHighNibbleTable[0..15]
        0x08, 0x00, static_cast<char>(0xC2), 0x01, 0x00, 0x24, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        // Lane 1: same table duplicated
        0x08, 0x00, static_cast<char>(0xC2), 0x01, 0x00, 0x24, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00);

    // Mask to isolate the low 4 bits of each byte.
    const __m256i nibble_mask = _mm256_set1_epi8(0x0F);

    // Extract low nibbles: input & 0x0F
    __m256i lo_nibbles = _mm256_and_si256(input, nibble_mask);

    // Extract high nibbles: (input >> 4) & 0x0F
    // _mm256_srli_epi16 shifts 16-bit lanes, so we mask afterward to isolate
    // each byte's high nibble (the shift puts garbage in the low byte of each
    // 16-bit pair, but the AND cleans it up).
    __m256i hi_nibbles = _mm256_and_si256(_mm256_srli_epi16(input, 4), nibble_mask);

    // vpshufb: use low nibbles as indices into the low-nibble lookup table.
    // Each byte of the result is low_lut[lo_nibbles[i]].
    __m256i lo_result = _mm256_shuffle_epi8(low_lut, lo_nibbles);

    // vpshufb: use high nibbles as indices into the high-nibble lookup table.
    __m256i hi_result = _mm256_shuffle_epi8(high_lut, hi_nibbles);

    // AND the two results.  Only bytes where BOTH nibble lookups agree on a
    // class bit will have that bit set — this is what eliminates false positives.
    return _mm256_and_si256(lo_result, hi_result);
}

#endif // STRATA_SIMD_HAS_AVX2

} // namespace simd
} // namespace strata
