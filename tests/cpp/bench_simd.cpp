/**
 * @file bench_simd.cpp
 * @brief Benchmark harness for the SIMD structural indexing subsystem.
 *
 * Generates a ~1MB JSON document and measures throughput of:
 *   1. StructuralIndexer::classify() — per-chunk classification
 *   2. IndexBuilder::build() — full document indexing
 *   3. index_document() — dispatched entry point
 *
 * Reports throughput in GB/s and MB/s.
 *
 * Usage: ./bench_simd [iterations]
 *   Default: 100 iterations per benchmark.
 */

#include "strata/simd/classifier.h"
#include "strata/simd/dispatch.h"
#include "strata/simd/index_builder.h"
#include "strata/simd/structural_indexer.h"

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

using namespace strata::simd;
using Clock = std::chrono::high_resolution_clock;

// ============================================================================
// JSON document generator
// ============================================================================

/// Generate a realistic JSON document of approximately target_size bytes.
/// Produces an array of objects with string keys, numeric values, and
/// nested arrays — representative of typical API payloads.
static std::string generate_json(size_t target_size) {
    std::string json;
    json.reserve(target_size + 1024);
    json += "[\n";

    int item = 0;
    while (json.size() < target_size) {
        if (item > 0)
            json += ",\n";
        json += "  {\"id\":";
        json += std::to_string(item);
        json += ",\"name\":\"item_";
        json += std::to_string(item);
        json += "_with_a_somewhat_longer_name_for_realistic_sizing\"";
        json += ",\"value\":";
        json += std::to_string(item * 3.14159);
        json += ",\"tags\":[\"alpha\",\"beta\",\"gamma\",\"delta\"]";
        json += ",\"nested\":{\"x\":";
        json += std::to_string(item % 100);
        json += ",\"y\":";
        json += std::to_string(item % 200);
        json += ",\"label\":\"nested_value_string_";
        json += std::to_string(item);
        json += "\"}";
        json += ",\"active\":";
        json += (item % 2 == 0) ? "true" : "false";
        json += ",\"description\":\"This is item number ";
        json += std::to_string(item);
        json += " in the benchmark dataset with some extra text for padding.\"}";
        ++item;
    }

    json += "\n]";
    return json;
}

// ============================================================================
// Benchmark runner
// ============================================================================

struct BenchResult {
    double total_seconds;
    double throughput_gbps;
    double throughput_mbps;
    size_t data_size;
    int iterations;
};

static BenchResult bench_indexer_classify(const uint8_t* data, size_t length, int iterations) {
    StructuralIndexer indexer;

    auto start = Clock::now();
    for (int iter = 0; iter < iterations; ++iter) {
        indexer.reset();
        size_t blocks = length / 64;
        for (size_t b = 0; b < blocks; ++b) {
            (void)indexer.classify(data + b * 64);
        }
    }
    auto end = Clock::now();

    double secs = std::chrono::duration<double>(end - start).count();
    // Only count full 64-byte blocks.
    size_t processed = (length / 64) * 64;
    double total_bytes = static_cast<double>(processed) * iterations;
    double gbps = (total_bytes / secs) / 1e9;
    double mbps = (total_bytes / secs) / 1e6;

    return {secs, gbps, mbps, processed, iterations};
}

static BenchResult bench_index_builder(const uint8_t* data, size_t length, int iterations) {
    auto start = Clock::now();
    for (int iter = 0; iter < iterations; ++iter) {
        IndexBuilder builder;
        auto idx = builder.build(data, length);
        // Prevent optimizer from eliding the work.
        if (idx.positions.empty() && length > 10) {
            std::abort(); // Should not happen.
        }
    }
    auto end = Clock::now();

    double secs = std::chrono::duration<double>(end - start).count();
    double total_bytes = static_cast<double>(length) * iterations;
    double gbps = (total_bytes / secs) / 1e9;
    double mbps = (total_bytes / secs) / 1e6;

    return {secs, gbps, mbps, length, iterations};
}

static BenchResult bench_index_document(const uint8_t* data, size_t length, int iterations) {
    auto start = Clock::now();
    for (int iter = 0; iter < iterations; ++iter) {
        auto idx = index_document(data, length);
        if (idx.positions.empty() && length > 10) {
            std::abort();
        }
    }
    auto end = Clock::now();

    double secs = std::chrono::duration<double>(end - start).count();
    double total_bytes = static_cast<double>(length) * iterations;
    double gbps = (total_bytes / secs) / 1e9;
    double mbps = (total_bytes / secs) / 1e6;

    return {secs, gbps, mbps, length, iterations};
}

// ============================================================================
// Main
// ============================================================================

int main(int argc, char** argv) {
    int iterations = 100;
    if (argc > 1) {
        iterations = std::atoi(argv[1]);
        if (iterations <= 0)
            iterations = 100;
    }

    constexpr size_t kTargetSize = 1024 * 1024; // 1 MB
    std::printf("=== SIMD Structural Indexer Benchmark ===\n");
    std::printf("Backend: %s\n", backend_name(detect_backend()));
    std::printf("Generating ~1MB JSON document...\n");

    std::string json = generate_json(kTargetSize);
    const auto* data = reinterpret_cast<const uint8_t*>(json.data());
    size_t length = json.size();

    std::printf("Document size: %.2f MB (%zu bytes)\n", static_cast<double>(length) / 1e6, length);
    std::printf("Iterations: %d\n\n", iterations);

    // Warm up.
    {
        IndexBuilder builder;
        auto idx = builder.build(data, length);
        std::printf("Structural positions found: %zu\n\n", idx.positions.size());
    }

    // Benchmark 1: Raw classify (chunks only, no position extraction).
    {
        auto r = bench_indexer_classify(data, length, iterations);
        std::printf("1. StructuralIndexer::classify()\n");
        std::printf("   %.2f GB/s  (%.0f MB/s)  [%.3f s total, %zu bytes x %d iters]\n\n",
                    r.throughput_gbps, r.throughput_mbps, r.total_seconds, r.data_size,
                    r.iterations);
    }

    // Benchmark 2: Full index build (classify + extract positions).
    {
        auto r = bench_index_builder(data, length, iterations);
        std::printf("2. IndexBuilder::build()\n");
        std::printf("   %.2f GB/s  (%.0f MB/s)  [%.3f s total, %zu bytes x %d iters]\n\n",
                    r.throughput_gbps, r.throughput_mbps, r.total_seconds, r.data_size,
                    r.iterations);
    }

    // Benchmark 3: Dispatched entry point.
    {
        auto r = bench_index_document(data, length, iterations);
        std::printf("3. index_document() [dispatched]\n");
        std::printf("   %.2f GB/s  (%.0f MB/s)  [%.3f s total, %zu bytes x %d iters]\n\n",
                    r.throughput_gbps, r.throughput_mbps, r.total_seconds, r.data_size,
                    r.iterations);
    }

    std::printf("=== Done ===\n");
    return 0;
}
