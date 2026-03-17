#pragma once

/**
 * @file classifier.h
 * @brief Branchless SIMD character classifier for JSON parsing.
 *
 * Uses the vpshufb (AVX2 _mm256_shuffle_epi8) lookup-table trick to classify
 * every byte in a 32-byte vector into one of five categories in a single pass:
 *   STRUCTURAL ({, }, [, ], :, ,), QUOTE ("), BACKSLASH (\), WHITESPACE, or OTHER.
 *
 * The technique splits each input byte into its high and low nibbles, performs
 * two independent shuffles against pre-computed 16-byte lookup tables, and ANDs
 * the results.  The lookup tables are designed so that only the correct
 * (high, low) combination produces a non-zero classification — zero false
 * positives for all four classes.
 *
 * Bit layout of the classification byte:
 *   bit 0  (0x01) — colon (:) structural
 *   bit 1  (0x02) — quote (")
 *   bit 2  (0x04) — backslash (\)
 *   bit 3  (0x08) — whitespace (tab 0x09, LF 0x0A, CR 0x0D)
 *   bit 5  (0x20) — bracket/brace structural ([ ] { })
 *   bit 6  (0x40) — comma (,) structural
 *   bit 7  (0x80) — whitespace (space 0x20)
 *
 * Aggregate masks:
 *   STRUCTURAL = bits 0|5|6 = 0x61
 *   WHITESPACE = bits 3|7   = 0x88
 *   QUOTE      = bit 1      = 0x02
 *   BACKSLASH  = bit 2      = 0x04
 */

#include <cstdint>

// ============================================================================
// Platform / SIMD feature detection
//
// Multiple features may be defined simultaneously (e.g. AVX-512 + AVX2, or
// SVE2 + NEON).  The structural_indexer selects the best path at compile time
// via a #if / #elif priority chain.
//
// Define STRATA_FORCE_SCALAR=1 (via -DSTRATA_FORCE_SCALAR=1 or env
// STRATA_SCALAR_ONLY=1 at build time) to disable ALL SIMD backends and
// compile only the portable scalar fallback.
// ============================================================================

#if defined(STRATA_FORCE_SCALAR)
// All SIMD backends suppressed — scalar fallback only.
#elif defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
// --- x86: AVX-512, AVX2 (both may be defined; AVX-512 implies AVX2) ------
#if defined(__AVX512F__) && defined(__AVX512BW__)
#define STRATA_SIMD_HAS_AVX512 1
#endif
#if defined(__AVX2__)
#define STRATA_SIMD_HAS_AVX2 1
#endif
#if defined(STRATA_SIMD_HAS_AVX512) || defined(STRATA_SIMD_HAS_AVX2)
#include <immintrin.h>
#endif
#elif defined(__aarch64__) || defined(__ARM_NEON)
// --- ARM: SVE2, NEON (both may be defined; SVE2 implies AArch64+NEON) ----
#if defined(__ARM_FEATURE_SVE2)
#define STRATA_SIMD_HAS_SVE2 1
#include <arm_sve.h>
#endif
#define STRATA_SIMD_HAS_NEON 1
#include <arm_neon.h>
#elif defined(__wasm_simd128__)
// --- WebAssembly SIMD128 -------------------------------------------------
#define STRATA_SIMD_HAS_WASM_SIMD 1
#include <wasm_simd128.h>
#elif defined(__riscv_v) || defined(__riscv_vector)
// --- RISC-V Vector Extension (RVV 1.0) -----------------------------------
#define STRATA_SIMD_HAS_RVV 1
#include <riscv_vector.h>
#endif

namespace strata {
namespace simd {

/// Aggregate classification masks for extracting specific character classes.
struct ClassMask {
    static constexpr uint8_t kStructural = 0x61; // bits 0, 5, 6
    static constexpr uint8_t kQuote = 0x02;      // bit 1
    static constexpr uint8_t kBackslash = 0x04;  // bit 2
    static constexpr uint8_t kWhitespace = 0x88; // bits 3, 7
};

/**
 * @brief Branchless SIMD character classifier using the vpshufb nibble-lookup trick.
 *
 * Classifies 32 bytes at a time (AVX2) with zero false positives for all JSON
 * character classes.  The lookup tables were derived by assigning disjoint bits
 * to each class and choosing entries so that the AND of low-nibble and
 * high-nibble lookups produces a non-zero result only for target characters.
 */
class Classifier {
  public:
#ifdef STRATA_SIMD_HAS_AVX512
    /// Classify 64 bytes at once (AVX-512BW).  Single 512-bit vpshufb across
    /// four 128-bit lanes, same nibble-AND trick.  Pair with
    /// _mm512_test_epi8_mask for direct 64-bit class bitmasks.
    [[nodiscard]] __m512i classify64(__m512i input) const noexcept;
#endif

#ifdef STRATA_SIMD_HAS_AVX2
    /// Classify 32 bytes at once (AVX2 vpshufb across two 128-bit lanes).
    [[nodiscard]] __m256i classify32(__m256i input) const noexcept;
#endif

#ifdef STRATA_SIMD_HAS_SVE2
    /// Classify a scalable vector of bytes (ARM SVE2 svtbl).
    /// @param pg  Governing predicate — only active lanes are classified.
    [[nodiscard]] svuint8_t classify_sve2(svuint8_t input, svbool_t pg) const noexcept;
#endif

#ifdef STRATA_SIMD_HAS_NEON
    /// Classify 16 bytes at once (ARM NEON vqtbl1q_u8).
    [[nodiscard]] uint8x16_t classify16(uint8x16_t input) const noexcept;
#endif

#ifdef STRATA_SIMD_HAS_WASM_SIMD
    /// Classify 16 bytes at once (WebAssembly SIMD128 i8x16.swizzle).
    [[nodiscard]] v128_t classify16_wasm(v128_t input) const noexcept;
#endif

#ifdef STRATA_SIMD_HAS_RVV
    /// Classify a vector of bytes (RISC-V Vector vluxei8 gather).
    /// @param vl  Current vector length from vsetvl.
    [[nodiscard]] vuint8m1_t classify_rvv(vuint8m1_t input, size_t vl) const noexcept;
#endif

    /**
     * Scalar classification of a single byte.
     * @return Classification bitmask (same encoding as SIMD version).
     */
    [[nodiscard]] static uint8_t classify_scalar(uint8_t c) noexcept;

  private:
    // -----------------------------------------------------------------------
    // Lookup table derivation
    //
    // Each JSON-significant character is identified by (high_nibble, low_nibble):
    //
    //   Char   Hex   High  Low   Class           Bit
    //   ────   ────  ────  ───   ──────────────  ───
    //   \t     0x09   0     9    WHITESPACE       3
    //   \n     0x0A   0     A    WHITESPACE       3
    //   \r     0x0D   0     D    WHITESPACE       3
    //   ' '    0x20   2     0    WHITESPACE       7
    //   "      0x22   2     2    QUOTE            1
    //   ,      0x2C   2     C    STRUCTURAL       6
    //   :      0x3A   3     A    STRUCTURAL       0
    //   [      0x5B   5     B    STRUCTURAL       5
    //   \      0x5C   5     C    BACKSLASH        2
    //   ]      0x5D   5     D    STRUCTURAL       5
    //   {      0x7B   7     B    STRUCTURAL       5
    //   }      0x7D   7     D    STRUCTURAL       5
    //
    // For each class, we assign a unique bit and set it in BOTH the low-nibble
    // and high-nibble tables only for nibble values that participate in that
    // class.  The AND of both lookups produces non-zero only when BOTH nibbles
    // agree on a class — eliminating false positives.
    //
    // Verification (all 256 byte values checked):
    //   STRUCTURAL false positives: 0
    //   QUOTE false positives:      0
    //   BACKSLASH false positives:  0
    //   WHITESPACE false positives: 0
    // -----------------------------------------------------------------------

    //                            idx:  0     1     2     3     4     5     6     7
    //                                  8     9     A     B     C     D     E     F
    // Low nibble table (16 bytes, duplicated for 256-bit):
    //   0: 0x80  (bit 7 — space has low nibble 0)
    //   1: 0x00
    //   2: 0x02  (bit 1 — quote has low nibble 2)
    //   3: 0x00
    //   4: 0x00
    //   5: 0x00
    //   6: 0x00
    //   7: 0x00
    //   8: 0x00
    //   9: 0x08  (bit 3 — tab has low nibble 9)
    //   A: 0x09  (bits 0,3 — colon/LF share low nibble A)
    //   B: 0x20  (bit 5 — brackets/braces share low nibble B)
    //   C: 0x44  (bits 2,6 — backslash/comma share low nibble C)
    //   D: 0x28  (bits 3,5 — CR and ]/} share low nibble D)
    //   E: 0x00
    //   F: 0x00
    static constexpr uint8_t kLowNibbleTable[16] = {
        0x80, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x08, 0x09, 0x20, 0x44, 0x28, 0x00, 0x00,
    };

    // High nibble table (16 bytes, duplicated for 256-bit):
    //   0: 0x08  (bit 3 — tab/LF/CR have high nibble 0)
    //   1: 0x00
    //   2: 0xC2  (bits 1,6,7 — quote/comma/space have high nibble 2)
    //   3: 0x01  (bit 0 — colon has high nibble 3)
    //   4: 0x00
    //   5: 0x24  (bits 2,5 — backslash and [/] have high nibble 5)
    //   6: 0x00
    //   7: 0x20  (bit 5 — {/} have high nibble 7)
    //   8–F: 0x00
    static constexpr uint8_t kHighNibbleTable[16] = {
        0x08, 0x00, 0xC2, 0x01, 0x00, 0x24, 0x00, 0x20,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };
};

} // namespace simd
} // namespace strata
