/**
 * @file fuzz_speculative_parser.cpp
 * @brief libFuzzer harness for the speculative parsing engine.
 *
 * Builds a structural index from arbitrary bytes, then feeds it to
 * the SpeculativeParser. The parser may reject invalid JSON, but must
 * never crash or trigger undefined behavior.
 */

#include "strata/simd/index_builder.h"
#include "strata/speculative/parser.h"
#include "strata/util/arena_allocator.hpp"

#include <cstddef>
#include <cstdint>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size == 0 || size > 1024 * 1024) {
        return 0; // Skip empty or very large inputs for performance.
    }

    // Build structural index.
    strata::simd::IndexBuilder builder;
    auto idx = builder.build(data, size);

    if (idx.positions.empty()) {
        return 0; // No structural chars — nothing to parse.
    }

    // Parse with speculation disabled (generic path).
    strata::util::Arena arena;
    strata::speculative::SpeculativeParser::Config config;
    config.enable_speculation = false;
    strata::speculative::SpeculativeParser parser(config, arena);

    try {
        auto result = parser.parse(data, size, idx.positions.data(), idx.positions.size());
        (void)result;
    } catch (...) {
        // Parser may throw on malformed input — that's acceptable.
    }

    return 0;
}
