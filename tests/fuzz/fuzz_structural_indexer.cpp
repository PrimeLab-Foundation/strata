/**
 * @file fuzz_structural_indexer.cpp
 * @brief libFuzzer harness for the SIMD structural indexer.
 *
 * Feeds arbitrary byte sequences to IndexBuilder::build() to verify
 * that no input causes crashes, undefined behavior, or assertion failures.
 * The indexer must gracefully handle any byte sequence — it does not
 * require valid JSON.
 */

#include "strata/simd/index_builder.h"

#include <cstddef>
#include <cstdint>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Build structural index over arbitrary bytes.
    strata::simd::IndexBuilder builder;
    auto idx = builder.build(data, size);

    // Validate invariants: all positions must be within document bounds.
    for (uint32_t pos : idx.positions) {
        if (pos >= size) {
            __builtin_trap(); // Position out of bounds — bug.
        }
    }

    // Verify document_length matches input.
    if (idx.document_length != size) {
        __builtin_trap();
    }

    return 0;
}
