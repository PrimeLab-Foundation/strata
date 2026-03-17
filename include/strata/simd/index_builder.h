#pragma once

/**
 * @file index_builder.h
 * @brief Builds a flat array of structural character positions over an entire document.
 *
 * Drives StructuralIndexer over the full input and collects byte offsets of
 * every structural character ({ } [ ] : ,) that appears outside of strings.
 * The resulting index is consumed by a tape-based or structural-skip parser.
 *
 * Bit extraction uses BMI2 PEXT when available for dense bit-to-index
 * conversion; falls back to a ctz (count-trailing-zeros) loop otherwise.
 */

#include "strata/simd/structural_indexer.h"

#include <cstdint>
#include <vector>

namespace strata {
namespace simd {

/**
 * @brief Drives StructuralIndexer over a complete document and produces
 *        a flat index of structural character positions.
 */
class IndexBuilder {
  public:
    /// Result of indexing a document.
    struct StructuralIndex {
        std::vector<uint32_t> positions; ///< Byte offsets of structural chars (outside strings)
        size_t document_length;          ///< Original document length in bytes
    };

    /**
     * Build the structural index for a document.
     *
     * The input does NOT need to be 64-byte aligned; the builder handles
     * padding of the final partial chunk internally.
     *
     * Pre-allocates the output vector with an estimated capacity of
     * length / 6 (heuristic: ~1 structural char per 6 bytes for typical JSON).
     *
     * @param data   Pointer to the raw JSON document bytes.
     * @param length Length of the document in bytes.
     * @return StructuralIndex with all structural character positions.
     */
    [[nodiscard]] StructuralIndex build(const uint8_t* data, size_t length);

  private:
    StructuralIndexer indexer_;

    /**
     * Extract set-bit positions from a 64-bit mask and append them to @p out.
     *
     * On BMI2 CPUs: uses _pext_u64 with a lookup approach to densely pack
     * 8-bit position values.
     *
     * Fallback: uses __builtin_ctzll / _tzcnt_u64 + clear-lowest-bit loop.
     *
     * @param mask        64-bit mask of set positions.
     * @param base_offset Added to each extracted position (chunk start offset).
     * @param out         Output vector to append positions to.
     */
    static void extract_positions(uint64_t mask, uint32_t base_offset, std::vector<uint32_t>& out);
};

} // namespace simd
} // namespace strata
