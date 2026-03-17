// CMake integration: add this file to STRATA_CORE_SOURCES in CMakeLists.txt.
// Requires: -mavx2 -mpclmul on GCC/Clang (or -march=native).

#include "strata/simd/structural_indexer.h"

#include "strata/simd/classifier.h"

#include <cstring>

// Platform detection (matches classifier.h)
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
#if defined(__AVX2__)
#define STRATA_SIMD_HAS_AVX2 1
#include <immintrin.h>
#endif
#if defined(__PCLMUL__)
#define STRATA_SIMD_HAS_PCLMUL 1
#include <wmmintrin.h> // _mm_clmulepi64_si128
#endif
#elif defined(__ARM_NEON) || defined(__aarch64__)
#define STRATA_SIMD_HAS_NEON 1
#include <arm_neon.h>
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
// For each contiguous run of backslashes, we need to know whether the
// character immediately after the run is "escaped" (i.e., the run has
// odd length).
//
// Algorithm (adapted from simdjson):
//   1. Find "start edges": backslash positions not preceded by another
//      backslash.  These are the first byte of each backslash run.
//   2. Partition starts into those at even bit-positions and odd
//      bit-positions (absolute position within the 64-bit chunk).
//   3. Add each partition to the backslash mask.  The carry propagates
//      through the consecutive 1-bits of the run.  Where the carry
//      exits (the position right after the run) tells us the parity:
//      - An even-positioned start whose carry exits at an odd position
//        means the run has odd length → the exit position is escaped.
//      - An odd-positioned start whose carry exits at an even position
//        likewise indicates an odd-length run.
//   4. Combine the two partitions to get the full escaped-positions mask.
//
// We also track a 1-bit overflow (prev_escaped_) for runs that span
// across 64-byte block boundaries.
// ============================================================================

uint64_t StructuralIndexer::compute_escape_mask(uint64_t bs) noexcept {
    // Alternating bit patterns for parity checks.
    constexpr uint64_t kEvenBits = 0x5555555555555555ULL; // bits 0,2,4,...
    constexpr uint64_t kOddBits = ~kEvenBits;             // bits 1,3,5,...

    // Start edges: positions where a backslash run begins.
    // A run starts at position i if bs[i]==1 and bs[i-1]==0.
    // The (bs << 1) accounts for the preceding bit; prev_escaped_ carries
    // the "was previous block's last byte a backslash" state.
    uint64_t start_edges = bs & ~((bs << 1) | prev_escaped_);

    // Flip the parity reference if the previous block ended mid-run.
    // If prev_escaped_ is set, the run started in the previous block at
    // some unknown absolute position, so we flip even/odd assignment.
    uint64_t even_mask = kEvenBits ^ prev_escaped_;

    // Partition starts by absolute parity.
    uint64_t even_starts = start_edges & even_mask;
    uint64_t odd_starts = start_edges & ~even_mask;

    // Add each partition to bs.  The carry propagates through consecutive 1s.
    uint64_t even_carries = bs + even_starts;

    // For odd_starts we need to detect 64-bit overflow (carry out).
    uint64_t odd_carries;
    bool overflow;
#if defined(__GNUC__) || defined(__clang__)
    overflow = __builtin_uaddll_overflow(bs, odd_starts,
                                         reinterpret_cast<unsigned long long*>(&odd_carries));
#else
    odd_carries = bs + odd_starts;
    overflow = (odd_carries < bs);
#endif
    // If the previous block had a carry-out, fold it in.
    odd_carries |= prev_escaped_;

    // Update state for the next block: only the odd-start partition can
    // produce a carry-out (even-start carries always land within 64 bits
    // for runs that start within this block).
    prev_escaped_ = overflow ? 1ULL : 0ULL;

    // Isolate the "exit" positions — bits that changed from 0 to 1 after the
    // addition, located outside the original backslash runs.
    uint64_t even_carry_ends = even_carries & ~bs;
    uint64_t odd_carry_ends = odd_carries & ~bs;

    // A carry that started at an even position and exits at an odd position
    // (or vice versa) indicates an odd-length run → that exit position is escaped.
    uint64_t even_start_odd_end = even_carry_ends & kOddBits;
    uint64_t odd_start_even_end = odd_carry_ends & kEvenBits;

    return even_start_odd_end | odd_start_even_end;
}

// ============================================================================
// Prefix-XOR: convert quote positions to string-region mask
//
// The "running parity" trick: if we compute the prefix-XOR of the quote
// bitmask, every bit between an opening quote and the next closing quote
// will be set (because an odd number of quotes precede it).
//
// On x86 with PCLMUL: carry-less multiplication of the quote mask with
// the all-ones constant computes this prefix-XOR in a single instruction.
//
// Scalar fallback: iterative XOR with doubled mask widths.
// ============================================================================

uint64_t StructuralIndexer::quotes_to_string_mask(uint64_t q) noexcept {
    uint64_t result;

#if defined(STRATA_SIMD_HAS_PCLMUL)
    // _mm_clmulepi64_si128(a, b, 0x00) computes the carry-less product of
    // the low 64 bits of a and b.  When b is all-ones, the result is the
    // prefix-XOR of a: result[i] = XOR(a[0], a[1], ..., a[i]).
    __m128i q_128 = _mm_set_epi64x(0, static_cast<long long>(q));
    __m128i ones_128 = _mm_set_epi64x(0, -1LL); // 0xFFFFFFFFFFFFFFFF
    __m128i clmul = _mm_clmulepi64_si128(q_128, ones_128, 0x00);
    result = static_cast<uint64_t>(_mm_cvtsi128_si64(clmul));
#else
    // Scalar prefix-XOR via doubling.  Each step doubles the "reach" of the
    // XOR, computing the full 64-bit prefix-XOR in 6 iterations (log2(64)).
    result = q;
    result ^= (result << 1);
    result ^= (result << 2);
    result ^= (result << 4);
    result ^= (result << 8);
    result ^= (result << 16);
    result ^= (result << 32);
#endif

    // XOR with previous block's string state.  If we ended the previous
    // block inside a string, all bits in this block start as "in string"
    // until the next quote flips them back.
    result ^= prev_in_string_;

    // Update state: the sign bit of result tells us whether we end this
    // block inside a string (bit 63 set → inside string).
    prev_in_string_ = static_cast<uint64_t>(static_cast<int64_t>(result) >>
                                            63); // arithmetic shift → all-ones or all-zeros

    return result;
}

// ============================================================================
// Main classify entry point
// ============================================================================

#ifdef STRATA_SIMD_HAS_AVX2

ChunkResult StructuralIndexer::classify(const uint8_t* input) noexcept {
    Classifier cls;

    // --- Load two 32-byte halves of the 64-byte chunk ----------------------
    __m256i chunk_lo = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(input));
    __m256i chunk_hi = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(input + 32));

    // --- Classify both halves with the vpshufb lookup ----------------------
    __m256i cls_lo = cls.classify32(chunk_lo);
    __m256i cls_hi = cls.classify32(chunk_hi);

    // --- Extract per-class bitmasks ----------------------------------------
    // For each class, AND with the class mask, compare against zero, invert.

    const __m256i zero = _mm256_setzero_si256();

    // Helper lambda: extract a 64-bit bitmask for a given class across both halves.
    auto extract_mask = [&](uint8_t class_bits) -> uint64_t {
        __m256i mask_vec = _mm256_set1_epi8(static_cast<char>(class_bits));

        // Low 32 bytes: AND with class mask, compare equal to zero, movemask, invert.
        __m256i test_lo = _mm256_and_si256(cls_lo, mask_vec);
        __m256i eq_lo = _mm256_cmpeq_epi8(test_lo, zero);
        uint32_t bits_lo = ~static_cast<uint32_t>(_mm256_movemask_epi8(eq_lo));

        // High 32 bytes.
        __m256i test_hi = _mm256_and_si256(cls_hi, mask_vec);
        __m256i eq_hi = _mm256_cmpeq_epi8(test_hi, zero);
        uint32_t bits_hi = ~static_cast<uint32_t>(_mm256_movemask_epi8(eq_hi));

        return static_cast<uint64_t>(bits_lo) | (static_cast<uint64_t>(bits_hi) << 32);
    };

    uint64_t raw_structural = extract_mask(ClassMask::kStructural);
    uint64_t raw_whitespace = extract_mask(ClassMask::kWhitespace);
    uint64_t raw_quotes = extract_mask(ClassMask::kQuote);
    uint64_t raw_backslash = extract_mask(ClassMask::kBackslash);

    // --- Escape-aware quote detection --------------------------------------
    // Determine which characters follow an odd-length backslash sequence.
    uint64_t escaped = compute_escape_mask(raw_backslash);

    // Remove escaped quotes: a quote preceded by an odd number of backslashes
    // is not a real string delimiter.
    uint64_t unescaped_quotes = raw_quotes & ~escaped;

    // --- String region mask ------------------------------------------------
    // Convert unescaped quote positions to a mask of positions inside strings.
    uint64_t string_mask = quotes_to_string_mask(unescaped_quotes);

    // --- Mask out structural chars inside strings --------------------------
    uint64_t structural = raw_structural & ~string_mask;

    // Whitespace mask is reported as-is (the parser may use it for skipping,
    // and whitespace inside strings is valid).
    return ChunkResult{structural, raw_whitespace, string_mask, unescaped_quotes};
}

#else // Scalar fallback

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

#endif // STRATA_SIMD_HAS_AVX2

} // namespace simd
} // namespace strata
