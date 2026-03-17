#pragma once

/**
 * @file structural_indexer.h
 * @brief SIMD structural character indexer for JSON parsing.
 *
 * Processes input in 64-byte chunks (two 256-bit AVX2 loads) and produces:
 *   - structural bitmask:  positions of { } [ ] : ,
 *   - string region mask:  positions inside JSON strings
 *   - quote mask:          positions of unescaped "
 *   - whitespace bitmask:  positions of \t \n \r and space
 *
 * Structural characters that fall inside string regions are masked out.
 *
 * Key algorithms:
 *   - vpshufb nibble-lookup for character classification (see classifier.h)
 *   - Carry-less multiplication (PCLMUL) for prefix-XOR to convert quote
 *     positions into string-region masks
 *   - Add-with-carry trick for detecting characters following an odd-length
 *     backslash sequence (escape-aware quote detection)
 */

#include <cstdint>

namespace strata {
namespace simd {

/// Result of classifying a single 64-byte chunk.
struct ChunkResult {
    uint64_t structural;  ///< Positions of { } [ ] : , (outside strings)
    uint64_t whitespace;  ///< Positions of whitespace characters
    uint64_t string_mask; ///< Positions inside JSON strings (between unescaped quotes)
    uint64_t quote_mask;  ///< Positions of unescaped " characters
};

/**
 * @brief Processes 64-byte chunks of JSON input and classifies every byte.
 *
 * Maintains inter-block state (prev_escaped_, prev_in_string_) for correct
 * streaming across chunk boundaries.  Call reset() before processing a new
 * document.
 *
 * The classify() method is fully branchless in the inner loop — all
 * classification is performed with SIMD + bitwise operations.
 */
class StructuralIndexer {
  public:
    StructuralIndexer() noexcept;

    /**
     * Process exactly 64 bytes of input.
     *
     * Caller must ensure that the input pointer is valid for 64 bytes.
     * For the final chunk of a document shorter than a 64-byte multiple,
     * the caller must zero-pad the remainder.
     *
     * @param input  Pointer to 64 bytes of JSON input.
     * @return ChunkResult with bitmasks for structural, whitespace, string,
     *         and quote positions within this 64-byte window.
     */
    [[nodiscard]] ChunkResult classify(const uint8_t* input) noexcept;

    /// Reset inter-block state for a new document.
    void reset() noexcept;

  private:
    /// Carry from previous block: 1 if the last byte was part of an
    /// odd-length backslash sequence that escapes the first byte of this block.
    uint64_t prev_escaped_{0};

    /// Running string-region state: all-ones (0xFFFF...F) if we ended inside a
    /// string, all-zeros otherwise.  XORed into the prefix-XOR result.
    uint64_t prev_in_string_{0};

    /**
     * Compute which positions follow an odd-length run of backslashes.
     *
     * Uses the add-with-carry trick from simdjson: for each contiguous run of
     * backslashes, adding the "start" bitmask to the run propagates a carry
     * through the consecutive 1-bits.  By checking whether the carry exits at
     * a position of opposite parity from the start, we determine if the run
     * has odd length.
     *
     * @param backslash_mask  64-bit mask of backslash positions in this chunk.
     * @return 64-bit mask where bit i is set if position i follows an odd
     *         number of consecutive backslashes.
     */
    uint64_t compute_escape_mask(uint64_t backslash_mask) noexcept;

    /**
     * Convert unescaped quote positions into a string-region bitmask.
     *
     * Uses carry-less multiplication (PCLMUL / _mm_clmulepi64_si128) with
     * the all-ones constant to compute a prefix-XOR (running parity).
     * Each quote toggles the "in-string" state.  The result has bit i set
     * if position i is inside a string (between an opening and closing quote).
     *
     * Falls back to a scalar loop on platforms without PCLMUL.
     *
     * @param quote_mask  64-bit mask of unescaped quote positions.
     * @return 64-bit mask of positions inside strings.
     */
    uint64_t quotes_to_string_mask(uint64_t quote_mask) noexcept;
};

} // namespace simd
} // namespace strata
