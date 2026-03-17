// CMake integration: add this file to STRATA_CORE_SOURCES in CMakeLists.txt.
// Requires: -mavx512f -mavx512bw for AVX-512, -mavx2 -mpclmul for AVX2,
//           or -march=native.

#include "strata/simd/structural_indexer.h"

#include "strata/simd/classifier.h"

#include <cstring>

// ============================================================================
// Platform detection (mirrors classifier.h — both files must agree)
// ============================================================================

#if defined(STRATA_FORCE_SCALAR)
// All SIMD backends suppressed — scalar fallback only.
#elif defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
#if defined(__AVX512F__) && defined(__AVX512BW__)
#define STRATA_SIMD_HAS_AVX512 1
#endif
#if defined(__AVX2__)
#define STRATA_SIMD_HAS_AVX2 1
#endif
#if defined(STRATA_SIMD_HAS_AVX512) || defined(STRATA_SIMD_HAS_AVX2)
#include <immintrin.h>
#endif
#if defined(__PCLMUL__)
#define STRATA_SIMD_HAS_PCLMUL 1
#include <wmmintrin.h> // _mm_clmulepi64_si128
#endif
#elif defined(__aarch64__) || defined(__ARM_NEON)
#if defined(__ARM_FEATURE_SVE2)
#define STRATA_SIMD_HAS_SVE2 1
#include <arm_sve.h>
#endif
#define STRATA_SIMD_HAS_NEON 1
#include <arm_neon.h>
#elif defined(__wasm_simd128__)
#define STRATA_SIMD_HAS_WASM_SIMD 1
#include <wasm_simd128.h>
#elif defined(__riscv_v) || defined(__riscv_vector)
#define STRATA_SIMD_HAS_RVV 1
#include <riscv_vector.h>
#endif

namespace strata {
namespace simd {

// ============================================================================
// Construction / reset
// ============================================================================

StructuralIndexer::StructuralIndexer() noexcept : prev_escaped_{0}, prev_in_string_{0} {}

void StructuralIndexer::reset() noexcept {
    prev_escaped_ = 0;
    prev_in_string_ = 0;
}

// ============================================================================
// Escape mask computation — follows_odd_sequence_of_backslashes
//
// Uses the add-with-carry trick (adapted from simdjson) to detect characters
// following an odd-length backslash run.  Shared across all backends since
// it operates on 64-bit bitmasks, not SIMD vectors.
// ============================================================================

uint64_t StructuralIndexer::compute_escape_mask(uint64_t bs) noexcept {
    constexpr uint64_t kEvenBits = 0x5555555555555555ULL;
    constexpr uint64_t kOddBits = ~kEvenBits;

    uint64_t start_edges = bs & ~((bs << 1) | prev_escaped_);
    uint64_t even_mask = kEvenBits ^ prev_escaped_;
    uint64_t even_starts = start_edges & even_mask;
    uint64_t odd_starts = start_edges & ~even_mask;

    uint64_t even_carries = bs + even_starts;

    uint64_t odd_carries;
    bool overflow;
#if defined(__GNUC__) || defined(__clang__)
    overflow = __builtin_uaddll_overflow(bs, odd_starts,
                                         reinterpret_cast<unsigned long long*>(&odd_carries));
#else
    odd_carries = bs + odd_starts;
    overflow = (odd_carries < bs);
#endif
    odd_carries |= prev_escaped_;
    prev_escaped_ = overflow ? 1ULL : 0ULL;

    uint64_t even_carry_ends = even_carries & ~bs;
    uint64_t odd_carry_ends = odd_carries & ~bs;
    uint64_t even_start_odd_end = even_carry_ends & kOddBits;
    uint64_t odd_start_even_end = odd_carry_ends & kEvenBits;

    return even_start_odd_end | odd_start_even_end;
}

// ============================================================================
// Prefix-XOR: convert quote positions to string-region mask
//
// Priority: x86 PCLMUL → ARM NEON PMULL (Crypto) → scalar doubling.
// The scalar doubling fallback works for all platforms (WASM, RVV, etc.).
// ============================================================================

uint64_t StructuralIndexer::quotes_to_string_mask(uint64_t q) noexcept {
    uint64_t result;

#if defined(STRATA_SIMD_HAS_PCLMUL)
    // x86 PCLMUL: carry-less multiply with all-ones = prefix-XOR.
    __m128i q_128 = _mm_set_epi64x(0, static_cast<long long>(q));
    __m128i ones_128 = _mm_set_epi64x(0, -1LL);
    __m128i clmul = _mm_clmulepi64_si128(q_128, ones_128, 0x00);
    result = static_cast<uint64_t>(_mm_cvtsi128_si64(clmul));
#elif defined(STRATA_SIMD_HAS_NEON) && defined(__ARM_FEATURE_CRYPTO)
    // ARM NEON polynomial multiply (PMULL): same as x86 PCLMUL.
    // Available on Apple Silicon and ARMv8-a+crypto (also present on SVE2 CPUs).
    poly64_t a = static_cast<poly64_t>(q);
    poly64_t b = static_cast<poly64_t>(~0ULL);
    poly128_t product = vmull_p64(a, b);
    result = static_cast<uint64_t>(vgetq_lane_u64(vreinterpretq_u64_p128(product), 0));
#else
    // Scalar prefix-XOR via doubling: 6 iterations for 64 bits (log2(64)).
    result = q;
    result ^= (result << 1);
    result ^= (result << 2);
    result ^= (result << 4);
    result ^= (result << 8);
    result ^= (result << 16);
    result ^= (result << 32);
#endif

    result ^= prev_in_string_;
    prev_in_string_ = static_cast<uint64_t>(static_cast<int64_t>(result) >> 63);

    return result;
}

// ============================================================================
// NEON helper functions (used by both NEON and SVE2 classify paths)
// ============================================================================

#ifdef STRATA_SIMD_HAS_NEON

// Emulate x86 movemask: extract MSB of each of 16 bytes → 16-bit mask.
static inline uint16_t neon_movemask(uint8x16_t v) noexcept {
    static const uint8_t kBitSelector[16] = {
        1, 2, 4, 8, 16, 32, 64, 128, 1, 2, 4, 8, 16, 32, 64, 128,
    };
    const uint8x16_t bit_sel = vld1q_u8(kBitSelector);
    uint8x16_t masked = vandq_u8(v, bit_sel);
    uint64x2_t sums = vpaddlq_u32(vpaddlq_u16(vpaddlq_u8(masked)));
    uint8_t lo = static_cast<uint8_t>(vgetq_lane_u64(sums, 0));
    uint8_t hi = static_cast<uint8_t>(vgetq_lane_u64(sums, 1));
    return static_cast<uint16_t>(lo) | (static_cast<uint16_t>(hi) << 8);
}

// Extract a 64-bit class bitmask from four classified 16-byte vectors.
static inline uint64_t neon_extract_class_mask(uint8x16_t c0, uint8x16_t c1, uint8x16_t c2,
                                               uint8x16_t c3, uint8_t class_bits) noexcept {
    const uint8x16_t mask_vec = vdupq_n_u8(class_bits);
    const uint8x16_t zero = vdupq_n_u8(0);

    uint8x16_t t0 = vcgtq_u8(vandq_u8(c0, mask_vec), zero);
    uint8x16_t t1 = vcgtq_u8(vandq_u8(c1, mask_vec), zero);
    uint8x16_t t2 = vcgtq_u8(vandq_u8(c2, mask_vec), zero);
    uint8x16_t t3 = vcgtq_u8(vandq_u8(c3, mask_vec), zero);

    uint16_t m0 = neon_movemask(t0);
    uint16_t m1 = neon_movemask(t1);
    uint16_t m2 = neon_movemask(t2);
    uint16_t m3 = neon_movemask(t3);

    return static_cast<uint64_t>(m0) | (static_cast<uint64_t>(m1) << 16) |
           (static_cast<uint64_t>(m2) << 32) | (static_cast<uint64_t>(m3) << 48);
}

#endif // STRATA_SIMD_HAS_NEON

// ############################################################################
// Main classify() — compile-time priority chain
//
//   AVX-512 > AVX2 > SVE2 > NEON > WASM SIMD > RVV > Scalar
// ############################################################################

// ============================================================================
// 1. AVX-512BW: single 64-byte load, native 64-bit masks
// ============================================================================

#if defined(STRATA_SIMD_HAS_AVX512)

ChunkResult StructuralIndexer::classify(const uint8_t* input) noexcept {
    Classifier cls;

    // Single 512-bit load for the entire 64-byte chunk.
    __m512i chunk = _mm512_loadu_si512(reinterpret_cast<const __m512i*>(input));

    // Classify all 64 bytes in one shot.
    __m512i classified = cls.classify64(chunk);

    // _mm512_test_epi8_mask: for each byte, returns 1 if (a[i] AND b[i]) != 0.
    // This directly produces a 64-bit mask — no cmpeq/movemask/invert needed!
    uint64_t raw_structural = _mm512_test_epi8_mask(
        classified, _mm512_set1_epi8(static_cast<char>(ClassMask::kStructural)));
    uint64_t raw_whitespace = _mm512_test_epi8_mask(
        classified, _mm512_set1_epi8(static_cast<char>(ClassMask::kWhitespace)));
    uint64_t raw_quotes =
        _mm512_test_epi8_mask(classified, _mm512_set1_epi8(static_cast<char>(ClassMask::kQuote)));
    uint64_t raw_backslash = _mm512_test_epi8_mask(
        classified, _mm512_set1_epi8(static_cast<char>(ClassMask::kBackslash)));

    uint64_t escaped = compute_escape_mask(raw_backslash);
    uint64_t unescaped_quotes = raw_quotes & ~escaped;
    uint64_t string_mask = quotes_to_string_mask(unescaped_quotes);
    uint64_t structural = raw_structural & ~string_mask;

    return ChunkResult{structural, raw_whitespace, string_mask, unescaped_quotes};
}

// ============================================================================
// 2. AVX2: two 32-byte loads
// ============================================================================

#elif defined(STRATA_SIMD_HAS_AVX2)

ChunkResult StructuralIndexer::classify(const uint8_t* input) noexcept {
    Classifier cls;

    __m256i chunk_lo = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(input));
    __m256i chunk_hi = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(input + 32));

    __m256i cls_lo = cls.classify32(chunk_lo);
    __m256i cls_hi = cls.classify32(chunk_hi);

    const __m256i zero = _mm256_setzero_si256();

    auto extract_mask = [&](uint8_t class_bits) -> uint64_t {
        __m256i mask_vec = _mm256_set1_epi8(static_cast<char>(class_bits));
        __m256i test_lo = _mm256_and_si256(cls_lo, mask_vec);
        __m256i eq_lo = _mm256_cmpeq_epi8(test_lo, zero);
        uint32_t bits_lo = ~static_cast<uint32_t>(_mm256_movemask_epi8(eq_lo));
        __m256i test_hi = _mm256_and_si256(cls_hi, mask_vec);
        __m256i eq_hi = _mm256_cmpeq_epi8(test_hi, zero);
        uint32_t bits_hi = ~static_cast<uint32_t>(_mm256_movemask_epi8(eq_hi));
        return static_cast<uint64_t>(bits_lo) | (static_cast<uint64_t>(bits_hi) << 32);
    };

    uint64_t raw_structural = extract_mask(ClassMask::kStructural);
    uint64_t raw_whitespace = extract_mask(ClassMask::kWhitespace);
    uint64_t raw_quotes = extract_mask(ClassMask::kQuote);
    uint64_t raw_backslash = extract_mask(ClassMask::kBackslash);

    uint64_t escaped = compute_escape_mask(raw_backslash);
    uint64_t unescaped_quotes = raw_quotes & ~escaped;
    uint64_t string_mask = quotes_to_string_mask(unescaped_quotes);
    uint64_t structural = raw_structural & ~string_mask;

    return ChunkResult{structural, raw_whitespace, string_mask, unescaped_quotes};
}

// ============================================================================
// 3. ARM SVE2: scalable vector classification + NEON movemask for extraction
//
// Strategy: classify with SVE2 svtbl (scalable — 1 to 4 iterations depending
// on VL), store classified bytes to a 64-byte buffer, reload as four NEON
// vectors, use existing neon_extract_class_mask for bitmask extraction.
// This hybrid avoids the need for SVE predicate-to-integer conversion.
// ============================================================================

#elif defined(STRATA_SIMD_HAS_SVE2)

ChunkResult StructuralIndexer::classify(const uint8_t* input) noexcept {
    Classifier cls;

    // Classify 64 bytes using SVE2 in a svwhilelt loop.
    // VL determines iterations: VL=128→4, VL=256→2, VL≥512→1.
    alignas(64) uint8_t class_buf[64];
    uint32_t offset = 0;
    while (offset < 64) {
        svbool_t pg = svwhilelt_b8(offset, static_cast<uint32_t>(64));
        svuint8_t chunk = svld1_u8(pg, input + offset);
        svuint8_t classified = cls.classify_sve2(chunk, pg);
        svst1_u8(pg, class_buf + offset, classified);
        offset += static_cast<uint32_t>(svcntb()); // advance by VL bytes
    }

    // Reload classified bytes as four NEON vectors for bitmask extraction.
    uint8x16_t c0 = vld1q_u8(class_buf);
    uint8x16_t c1 = vld1q_u8(class_buf + 16);
    uint8x16_t c2 = vld1q_u8(class_buf + 32);
    uint8x16_t c3 = vld1q_u8(class_buf + 48);

    uint64_t raw_structural = neon_extract_class_mask(c0, c1, c2, c3, ClassMask::kStructural);
    uint64_t raw_whitespace = neon_extract_class_mask(c0, c1, c2, c3, ClassMask::kWhitespace);
    uint64_t raw_quotes = neon_extract_class_mask(c0, c1, c2, c3, ClassMask::kQuote);
    uint64_t raw_backslash = neon_extract_class_mask(c0, c1, c2, c3, ClassMask::kBackslash);

    uint64_t escaped = compute_escape_mask(raw_backslash);
    uint64_t unescaped_quotes = raw_quotes & ~escaped;
    uint64_t string_mask = quotes_to_string_mask(unescaped_quotes);
    uint64_t structural = raw_structural & ~string_mask;

    return ChunkResult{structural, raw_whitespace, string_mask, unescaped_quotes};
}

// ============================================================================
// 4. ARM NEON: four 16-byte loads
// ============================================================================

#elif defined(STRATA_SIMD_HAS_NEON)

ChunkResult StructuralIndexer::classify(const uint8_t* input) noexcept {
    Classifier cls;

    uint8x16_t chunk0 = vld1q_u8(input);
    uint8x16_t chunk1 = vld1q_u8(input + 16);
    uint8x16_t chunk2 = vld1q_u8(input + 32);
    uint8x16_t chunk3 = vld1q_u8(input + 48);

    uint8x16_t cls0 = cls.classify16(chunk0);
    uint8x16_t cls1 = cls.classify16(chunk1);
    uint8x16_t cls2 = cls.classify16(chunk2);
    uint8x16_t cls3 = cls.classify16(chunk3);

    uint64_t raw_structural =
        neon_extract_class_mask(cls0, cls1, cls2, cls3, ClassMask::kStructural);
    uint64_t raw_whitespace =
        neon_extract_class_mask(cls0, cls1, cls2, cls3, ClassMask::kWhitespace);
    uint64_t raw_quotes = neon_extract_class_mask(cls0, cls1, cls2, cls3, ClassMask::kQuote);
    uint64_t raw_backslash = neon_extract_class_mask(cls0, cls1, cls2, cls3, ClassMask::kBackslash);

    uint64_t escaped = compute_escape_mask(raw_backslash);
    uint64_t unescaped_quotes = raw_quotes & ~escaped;
    uint64_t string_mask = quotes_to_string_mask(unescaped_quotes);
    uint64_t structural = raw_structural & ~string_mask;

    return ChunkResult{structural, raw_whitespace, string_mask, unescaped_quotes};
}

// ============================================================================
// 5. WebAssembly SIMD128: four 16-byte loads, native i8x16.bitmask
// ============================================================================

#elif defined(STRATA_SIMD_HAS_WASM_SIMD)

// WASM SIMD has a native bitmask instruction — no movemask emulation needed.
static inline uint64_t wasm_extract_class_mask(v128_t c0, v128_t c1, v128_t c2, v128_t c3,
                                               uint8_t class_bits) noexcept {
    const v128_t mask_vec = wasm_u8x16_splat(class_bits);
    const v128_t zero = wasm_u8x16_splat(0);

    // AND with class mask, compare not-equal to zero → 0xFF for matches.
    // wasm_i8x16_bitmask: extract MSB of each byte → 16-bit mask (native!).
    v128_t t0 = wasm_i8x16_ne(wasm_v128_and(c0, mask_vec), zero);
    v128_t t1 = wasm_i8x16_ne(wasm_v128_and(c1, mask_vec), zero);
    v128_t t2 = wasm_i8x16_ne(wasm_v128_and(c2, mask_vec), zero);
    v128_t t3 = wasm_i8x16_ne(wasm_v128_and(c3, mask_vec), zero);

    uint16_t m0 = static_cast<uint16_t>(wasm_i8x16_bitmask(t0));
    uint16_t m1 = static_cast<uint16_t>(wasm_i8x16_bitmask(t1));
    uint16_t m2 = static_cast<uint16_t>(wasm_i8x16_bitmask(t2));
    uint16_t m3 = static_cast<uint16_t>(wasm_i8x16_bitmask(t3));

    return static_cast<uint64_t>(m0) | (static_cast<uint64_t>(m1) << 16) |
           (static_cast<uint64_t>(m2) << 32) | (static_cast<uint64_t>(m3) << 48);
}

ChunkResult StructuralIndexer::classify(const uint8_t* input) noexcept {
    Classifier cls;

    v128_t chunk0 = wasm_v128_load(input);
    v128_t chunk1 = wasm_v128_load(input + 16);
    v128_t chunk2 = wasm_v128_load(input + 32);
    v128_t chunk3 = wasm_v128_load(input + 48);

    v128_t cls0 = cls.classify16_wasm(chunk0);
    v128_t cls1 = cls.classify16_wasm(chunk1);
    v128_t cls2 = cls.classify16_wasm(chunk2);
    v128_t cls3 = cls.classify16_wasm(chunk3);

    uint64_t raw_structural =
        wasm_extract_class_mask(cls0, cls1, cls2, cls3, ClassMask::kStructural);
    uint64_t raw_whitespace =
        wasm_extract_class_mask(cls0, cls1, cls2, cls3, ClassMask::kWhitespace);
    uint64_t raw_quotes = wasm_extract_class_mask(cls0, cls1, cls2, cls3, ClassMask::kQuote);
    uint64_t raw_backslash = wasm_extract_class_mask(cls0, cls1, cls2, cls3, ClassMask::kBackslash);

    uint64_t escaped = compute_escape_mask(raw_backslash);
    uint64_t unescaped_quotes = raw_quotes & ~escaped;
    uint64_t string_mask = quotes_to_string_mask(unescaped_quotes);
    uint64_t structural = raw_structural & ~string_mask;

    return ChunkResult{structural, raw_whitespace, string_mask, unescaped_quotes};
}

// ============================================================================
// 6. RISC-V Vector (RVV 1.0): scalable vector classification + mask store
//
// Strategy: classify in a vsetvl loop (VL may be 16, 32, 64, or more bytes),
// extract class bitmasks using RVV's native mask operations (vmsne + vsm).
// ============================================================================

#elif defined(STRATA_SIMD_HAS_RVV)

// Extract a 64-bit class bitmask by classify-and-test in a vsetvl loop.
// RVV mask registers are 1 bit per element; vsm stores them packed into bytes.
static inline uint64_t rvv_classify_and_extract(const uint8_t* input, uint8_t class_bits,
                                                const Classifier& cls) noexcept {
    uint64_t result = 0;
    size_t offset = 0;

    while (offset < 64) {
        size_t vl = __riscv_vsetvl_e8m1(64 - offset);

        vuint8m1_t chunk = __riscv_vle8_v_u8m1(input + offset, vl);
        vuint8m1_t classified = cls.classify_rvv(chunk, vl);

        // AND with class mask, compare not-equal to zero → mask register.
        vuint8m1_t test = __riscv_vand_vx_u8m1(classified, class_bits, vl);
        vbool8_t match = __riscv_vmsne_vx_u8m1_b8(test, 0, vl);

        // vsm: store mask bits packed into bytes (1 bit per element).
        alignas(8) uint8_t mask_buf[8] = {};
        __riscv_vsm_v_b8(mask_buf, match, vl);

        // Load packed mask bits and shift into position.
        uint64_t partial;
        __builtin_memcpy(&partial, mask_buf, sizeof(partial));
        // Keep only the first 'vl' bits (upper bits may be garbage).
        if (vl < 64)
            partial &= (1ULL << vl) - 1;
        result |= partial << offset;

        offset += vl;
    }
    return result;
}

ChunkResult StructuralIndexer::classify(const uint8_t* input) noexcept {
    Classifier cls;

    uint64_t raw_structural = rvv_classify_and_extract(input, ClassMask::kStructural, cls);
    uint64_t raw_whitespace = rvv_classify_and_extract(input, ClassMask::kWhitespace, cls);
    uint64_t raw_quotes = rvv_classify_and_extract(input, ClassMask::kQuote, cls);
    uint64_t raw_backslash = rvv_classify_and_extract(input, ClassMask::kBackslash, cls);

    uint64_t escaped = compute_escape_mask(raw_backslash);
    uint64_t unescaped_quotes = raw_quotes & ~escaped;
    uint64_t string_mask = quotes_to_string_mask(unescaped_quotes);
    uint64_t structural = raw_structural & ~string_mask;

    return ChunkResult{structural, raw_whitespace, string_mask, unescaped_quotes};
}

// ============================================================================
// 7. Scalar fallback
// ============================================================================

#else

ChunkResult StructuralIndexer::classify(const uint8_t* input) noexcept {
    uint64_t raw_structural = 0;
    uint64_t raw_whitespace = 0;
    uint64_t raw_quotes = 0;
    uint64_t raw_backslash = 0;

    for (int i = 0; i < 64; ++i) {
        uint8_t c = input[i];
        uint8_t cls = Classifier::classify_scalar(c);
        uint64_t bit = 1ULL << i;

        if (cls & ClassMask::kStructural)
            raw_structural |= bit;
        if (cls & ClassMask::kWhitespace)
            raw_whitespace |= bit;
        if (cls & ClassMask::kQuote)
            raw_quotes |= bit;
        if (cls & ClassMask::kBackslash)
            raw_backslash |= bit;
    }

    uint64_t escaped = compute_escape_mask(raw_backslash);
    uint64_t unescaped_quotes = raw_quotes & ~escaped;
    uint64_t string_mask = quotes_to_string_mask(unescaped_quotes);
    uint64_t structural = raw_structural & ~string_mask;

    return ChunkResult{structural, raw_whitespace, string_mask, unescaped_quotes};
}

#endif // classify priority chain

} // namespace simd
} // namespace strata
