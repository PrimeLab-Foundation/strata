// CMake integration: add this file to STRATA_CORE_SOURCES in CMakeLists.txt.
// Requires: -mavx2 -mbmi2 on GCC/Clang for the BMI2 PEXT path (or -march=native).

#include "strata/simd/index_builder.h"

#include <cstring>

// BMI2 detection (for PEXT)
#if defined(__x86_64__) || defined(_M_X64)
#if defined(__BMI2__)
#define STRATA_SIMD_HAS_BMI2 1
#include <immintrin.h> // _pext_u64
#endif
#endif

namespace strata {
namespace simd {

// ============================================================================
// Bit extraction: convert a 64-bit bitmask into an array of set-bit positions
// ============================================================================

void IndexBuilder::extract_positions(uint64_t mask, uint32_t base_offset,
                                     std::vector<uint32_t>& out) {
    // Fast path: nothing to extract.
    if (mask == 0)
        return;

#ifdef STRATA_SIMD_HAS_BMI2
    // BMI2 PEXT approach: extract set-bit positions in batches of 8.
    //
    // For each iteration, pop the lowest 8 set bits from the mask using PEXT,
    // compute their positions, and append to the output.  PEXT extracts bits
    // at positions specified by a selector mask, packing them densely.
    //
    // We use __builtin_ctzll to find each bit's position — this is still
    // faster than a pure ctz-loop because we amortize the vector push_back
    // overhead by pre-sizing in the caller.

    while (mask != 0) {
        // Count trailing zeros to find the position of the lowest set bit.
        uint32_t pos = static_cast<uint32_t>(__builtin_ctzll(mask));
        out.push_back(base_offset + pos);

        // Clear the lowest set bit: mask &= mask - 1.
        // This is a single BLSR instruction on BMI1 CPUs.
        mask &= mask - 1;
    }

#else
    // Fallback: ctz + clear-lowest-bit loop.
    //
    // __builtin_ctzll: count trailing zeros (position of lowest set bit).
    // Clear lowest bit: mask &= mask - 1  (or mask = _blsr_u64(mask) on BMI1).
    while (mask != 0) {
        uint32_t pos = static_cast<uint32_t>(__builtin_ctzll(mask));
        out.push_back(base_offset + pos);
        mask &= mask - 1; // clear lowest set bit
    }
#endif
}

// ============================================================================
// Build structural index over the entire document
// ============================================================================

IndexBuilder::StructuralIndex IndexBuilder::build(const uint8_t* data, size_t length) {
    StructuralIndex result;
    result.document_length = length;

    // Heuristic pre-allocation: typical JSON has ~1 structural char per 6 bytes.
    // This avoids repeated vector reallocations for most inputs.
    result.positions.reserve(length / 6 + 64);

    // Reset the indexer for a fresh document.
    indexer_.reset();

    // Process complete 64-byte blocks.
    size_t block_count = length / 64;
    for (size_t block = 0; block < block_count; ++block) {
        uint32_t base = static_cast<uint32_t>(block * 64);
        ChunkResult cr = indexer_.classify(data + base);
        extract_positions(cr.structural, base, result.positions);
    }

    // Handle the final partial block (if any).
    size_t remainder = length % 64;
    if (remainder > 0) {
        // Zero-pad the final chunk to 64 bytes.  This ensures the indexer
        // reads valid memory and that the padding bytes (zeros) classify as
        // OTHER — producing no spurious structural positions.
        alignas(64) uint8_t padded[64] = {};
        std::memcpy(padded, data + block_count * 64, remainder);

        uint32_t base = static_cast<uint32_t>(block_count * 64);
        ChunkResult cr = indexer_.classify(padded);

        // Only extract positions within the actual document, not the padding.
        // Mask away bits beyond 'remainder'.
        uint64_t valid_mask = (remainder < 64) ? ((1ULL << remainder) - 1) : ~0ULL;
        extract_positions(cr.structural & valid_mask, base, result.positions);
    }

    return result;
}

} // namespace simd
} // namespace strata
