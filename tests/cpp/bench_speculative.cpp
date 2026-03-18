/**
 * @file bench_speculative.cpp
 * @brief Benchmarks for the speculative parsing engine.
 *
 * Reports:
 *   - Throughput in MB/s (and GB/s for large inputs)
 *   - Speculation hit rate
 *   - Online learning warmup curve
 *   - Comparison: speculative vs. generic parsing
 */

#include "strata/json/json_core.hpp"
#include "strata/simd/index_builder.h"
#include "strata/speculative/fast_paths.h"
#include "strata/speculative/parser.h"
#include "strata/speculative/transition_model.h"
#include "strata/speculative/types.h"
#include "strata/util/arena_allocator.hpp"

#include <chrono>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

using namespace strata::speculative;

// ── Timing helper ────────────────────────────────────────────────────────────

struct BenchResult {
    double elapsed_ms;
    double throughput_mbs; // MB/s
    size_t bytes;
    int iterations;
};

template <typename Fn>
BenchResult run_bench(const char* name, size_t data_bytes, int iterations, Fn&& fn) {
    // Warmup
    for (int i = 0; i < 3; ++i)
        fn();

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i)
        fn();
    auto end = std::chrono::high_resolution_clock::now();

    double ms = std::chrono::duration<double, std::milli>(end - start).count();
    double total_bytes = static_cast<double>(data_bytes) * iterations;
    double mbs = (total_bytes / (1024.0 * 1024.0)) / (ms / 1000.0);

    std::printf("  %-45s  %8.2f ms  %8.1f MB/s  (%d iters, %zu bytes)\n", name, ms, mbs, iterations,
                data_bytes);

    return {ms, mbs, data_bytes, iterations};
}

// ── Data generators ──────────────────────────────────────────────────────────

// Generate homogeneous NDJSON (like log lines — all same schema)
static std::string generate_homogeneous_ndjson(int num_lines) {
    std::string result;
    result.reserve(num_lines * 100);
    for (int i = 0; i < num_lines; ++i) {
        char buf[256];
        std::snprintf(
            buf, sizeof(buf),
            R"({"timestamp":%d,"level":"INFO","message":"Request processed","latency_ms":%d,"status":200,"user_id":%d})",
            1700000000 + i, 10 + (i % 100), 1000 + (i % 500));
        result += buf;
        result += '\n';
    }
    return result;
}

// Generate heterogeneous JSON (mixed types, varying structure)
static std::string generate_heterogeneous_json(int num_items) {
    std::string result = "[";
    for (int i = 0; i < num_items; ++i) {
        if (i > 0)
            result += ",";
        switch (i % 5) {
        case 0:
            result += std::to_string(i * 42);
            break;
        case 1:
            result += "\"string_value_" + std::to_string(i) + "\"";
            break;
        case 2:
            result += (i % 2 == 0) ? "true" : "false";
            break;
        case 3:
            result += "null";
            break;
        case 4: {
            char buf[128];
            std::snprintf(buf, sizeof(buf), R"({"id":%d,"name":"item_%d","price":%.2f})", i, i,
                          9.99 + i * 0.1);
            result += buf;
            break;
        }
        }
    }
    result += "]";
    return result;
}

// Generate integer-heavy JSON (for fast-path integer benchmark)
static std::string generate_integer_array(int num_ints) {
    std::string result = "[";
    for (int i = 0; i < num_ints; ++i) {
        if (i > 0)
            result += ",";
        result += std::to_string(i * 1000 + 42);
    }
    result += "]";
    return result;
}

// =============================================================================
// Benchmarks
// =============================================================================

static void bench_homogeneous_ndjson() {
    std::printf("\n--- Homogeneous NDJSON (log-like, 10K lines) ---\n");

    std::string ndjson = generate_homogeneous_ndjson(10000);
    strata::simd::IndexBuilder idx_builder;
    auto index = idx_builder.build(reinterpret_cast<const uint8_t*>(ndjson.data()), ndjson.size());

    // Generic (no speculation)
    run_bench("Generic parser (no speculation)", ndjson.size(), 20, [&]() {
        strata::util::Arena arena;
        SpeculativeParser::Config config;
        config.enable_speculation = false;
        SpeculativeParser parser(config, arena);
        parser.parse_ndjson(reinterpret_cast<const uint8_t*>(ndjson.data()), ndjson.size(),
                            index.positions.data(), index.positions.size());
    });

    // Speculative with online learning
    run_bench("Speculative parser (online learning)", ndjson.size(), 20, [&]() {
        strata::util::Arena arena;
        SpeculativeParser::Config config;
        config.enable_speculation = true;
        config.enable_online_learning = true;
        config.online_learning_warmup = 10;
        SpeculativeParser parser(config, arena);
        parser.parse_ndjson(reinterpret_cast<const uint8_t*>(ndjson.data()), ndjson.size(),
                            index.positions.data(), index.positions.size());
    });

    // Speculative with pre-trained model
    run_bench("Speculative parser (pre-trained schema)", ndjson.size(), 20, [&]() {
        strata::util::Arena arena;
        SpeculativeParser::Config config;
        config.enable_speculation = true;
        config.enable_online_learning = true;
        config.online_learning_warmup = 0;
        SpeculativeParser parser(config, arena);

        std::string schema = R"({
            "type":"object","properties":{
                "timestamp":{"type":"integer"},
                "level":{"type":"string"},
                "message":{"type":"string"},
                "latency_ms":{"type":"integer"},
                "status":{"type":"integer"},
                "user_id":{"type":"integer"}
            }
        })";
        parser.model().train_from_schema(schema);

        parser.parse_ndjson(reinterpret_cast<const uint8_t*>(ndjson.data()), ndjson.size(),
                            index.positions.data(), index.positions.size());
    });

    // Report speculation stats for the pre-trained run
    {
        strata::util::Arena arena;
        SpeculativeParser::Config config;
        config.enable_speculation = true;
        config.enable_online_learning = true;
        config.online_learning_warmup = 0;
        SpeculativeParser parser(config, arena);

        std::string schema = R"({
            "type":"object","properties":{
                "timestamp":{"type":"integer"},
                "level":{"type":"string"},
                "message":{"type":"string"},
                "latency_ms":{"type":"integer"},
                "status":{"type":"integer"},
                "user_id":{"type":"integer"}
            }
        })";
        parser.model().train_from_schema(schema);
        parser.model().reset_stats();

        parser.parse_ndjson(reinterpret_cast<const uint8_t*>(ndjson.data()), ndjson.size(),
                            index.positions.data(), index.positions.size());

        auto stats = parser.model().stats();
        std::printf("\n  Speculation stats (pre-trained, 10K lines):\n");
        std::printf("    Total predictions:      %lu\n", (unsigned long)stats.total_predictions);
        std::printf("    Speculations attempted:  %lu\n",
                    (unsigned long)stats.speculations_attempted);
        std::printf("    Speculations hit:        %lu\n", (unsigned long)stats.speculations_hit);
        std::printf("    Hit rate:                %.1f%%\n", stats.hit_rate() * 100.0);
        std::printf("    Speculation rate:        %.1f%%\n", stats.speculation_rate() * 100.0);
    }
}

static void bench_heterogeneous_json() {
    std::printf("\n--- Heterogeneous JSON (mixed types, 10K items) ---\n");

    std::string json = generate_heterogeneous_json(10000);
    strata::simd::IndexBuilder idx_builder;
    auto index = idx_builder.build(reinterpret_cast<const uint8_t*>(json.data()), json.size());

    run_bench("Generic parser", json.size(), 50, [&]() {
        strata::util::Arena arena;
        SpeculativeParser::Config config;
        config.enable_speculation = false;
        SpeculativeParser parser(config, arena);
        parser.parse(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                     index.positions.data(), index.positions.size());
    });

    run_bench("Speculative parser (online)", json.size(), 50, [&]() {
        strata::util::Arena arena;
        SpeculativeParser::Config config;
        config.enable_speculation = true;
        config.enable_online_learning = true;
        config.online_learning_warmup = 50;
        SpeculativeParser parser(config, arena);
        parser.parse(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                     index.positions.data(), index.positions.size());
    });
}

static void bench_integer_heavy() {
    std::printf("\n--- Integer-heavy JSON (50K integers) ---\n");

    std::string json = generate_integer_array(50000);
    strata::simd::IndexBuilder idx_builder;
    auto index = idx_builder.build(reinterpret_cast<const uint8_t*>(json.data()), json.size());

    run_bench("Generic parser", json.size(), 50, [&]() {
        strata::util::Arena arena;
        SpeculativeParser::Config config;
        config.enable_speculation = false;
        SpeculativeParser parser(config, arena);
        parser.parse(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                     index.positions.data(), index.positions.size());
    });

    run_bench("Speculative parser (online)", json.size(), 50, [&]() {
        strata::util::Arena arena;
        SpeculativeParser::Config config;
        config.enable_speculation = true;
        config.enable_online_learning = true;
        config.online_learning_warmup = 10;
        SpeculativeParser parser(config, arena);
        parser.parse(reinterpret_cast<const uint8_t*>(json.data()), json.size(),
                     index.positions.data(), index.positions.size());
    });
}

static void bench_fast_path_integer() {
    std::printf("\n--- FastPath: Integer parsing micro-benchmark ---\n");

    strata::util::Arena arena;
    FastPaths fp(arena);

    // 8-digit numbers (ideal for SWAR)
    const uint8_t* data8 = reinterpret_cast<const uint8_t*>("12345678,");
    int iters = 10000000;

    auto start = std::chrono::high_resolution_clock::now();
    volatile double sink = 0;
    for (int i = 0; i < iters; ++i) {
        auto r = fp.parse_integer(data8, 9);
        sink = r.value.as_number();
    }
    auto end = std::chrono::high_resolution_clock::now();
    double ms = std::chrono::duration<double, std::milli>(end - start).count();
    double ns_per = (ms * 1e6) / iters;

    std::printf("  parse_integer (8 digits): %.1f ns/call  (%d M calls in %.1f ms)\n", ns_per,
                iters / 1000000, ms);
    (void)sink;
}

static void bench_fast_path_short_string() {
    std::printf("\n--- FastPath: Short string parsing micro-benchmark ---\n");

    strata::util::Arena arena;
    FastPaths fp(arena);

    const uint8_t* data = reinterpret_cast<const uint8_t*>("\"hello world\",");
    int iters = 10000000;

    auto start = std::chrono::high_resolution_clock::now();
    volatile bool sink = false;
    for (int i = 0; i < iters; ++i) {
        auto r = fp.parse_short_string(data, 14);
        sink = r.success;
    }
    auto end = std::chrono::high_resolution_clock::now();
    double ms = std::chrono::duration<double, std::milli>(end - start).count();
    double ns_per = (ms * 1e6) / iters;

    std::printf("  parse_short_string (11 chars): %.1f ns/call  (%d M calls in %.1f ms)\n", ns_per,
                iters / 1000000, ms);
    (void)sink;
}

static void bench_online_learning_warmup() {
    std::printf("\n--- Online Learning Warmup Curve ---\n");

    std::string ndjson = generate_homogeneous_ndjson(1000);
    strata::simd::IndexBuilder idx_builder;
    auto index = idx_builder.build(reinterpret_cast<const uint8_t*>(ndjson.data()), ndjson.size());

    // Parse with increasing warmup to see convergence
    for (int warmup : {0, 5, 10, 25, 50, 100, 200}) {
        strata::util::Arena arena;
        SpeculativeParser::Config config;
        config.enable_speculation = true;
        config.enable_online_learning = true;
        config.online_learning_warmup = static_cast<size_t>(warmup);
        SpeculativeParser parser(config, arena);
        parser.model().reset_stats();

        parser.parse_ndjson(reinterpret_cast<const uint8_t*>(ndjson.data()), ndjson.size(),
                            index.positions.data(), index.positions.size());

        auto stats = parser.model().stats();
        std::printf(
            "  warmup=%3d  predictions=%6lu  speculations=%6lu  hits=%6lu  hit_rate=%.1f%%\n",
            warmup, (unsigned long)stats.total_predictions,
            (unsigned long)stats.speculations_attempted, (unsigned long)stats.speculations_hit,
            stats.speculation_rate() * 100.0);
    }
}

static void bench_transition_model_predict() {
    std::printf("\n--- TransitionModel: predict() micro-benchmark ---\n");

    TransitionModel model;
    ParserContext ctx{};
    ctx.key_hash = hash_key(reinterpret_cast<const uint8_t*>("age"), 3);
    ctx.nesting_depth = 1;

    // Populate the bucket
    for (int i = 0; i < 500; ++i)
        model.observe(ctx, ValueType::INTEGER);

    int iters = 50000000;
    auto start = std::chrono::high_resolution_clock::now();
    volatile bool sink = false;
    for (int i = 0; i < iters; ++i) {
        auto pred = model.predict(ctx);
        sink = pred.should_speculate;
    }
    auto end = std::chrono::high_resolution_clock::now();
    double ms = std::chrono::duration<double, std::milli>(end - start).count();
    double ns_per = (ms * 1e6) / iters;

    std::printf("  predict(): %.2f ns/call  (%d M calls in %.1f ms)\n", ns_per, iters / 1000000,
                ms);
    (void)sink;
}

static void bench_transition_model_observe() {
    std::printf("\n--- TransitionModel: observe() micro-benchmark ---\n");

    TransitionModel model;
    ParserContext ctx{};
    ctx.key_hash = 42;
    ctx.nesting_depth = 1;

    int iters = 50000000;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iters; ++i) {
        model.observe(ctx, ValueType::INTEGER);
    }
    auto end = std::chrono::high_resolution_clock::now();
    double ms = std::chrono::duration<double, std::milli>(end - start).count();
    double ns_per = (ms * 1e6) / iters;

    std::printf("  observe(): %.2f ns/call  (%d M calls in %.1f ms)\n", ns_per, iters / 1000000,
                ms);
}

// =============================================================================
// main
// =============================================================================

int main() {
    std::printf("=== Speculative Parsing Engine Benchmarks ===\n");
    std::printf("  TransitionModel size: %zu bytes (limit: %d KB)\n", sizeof(TransitionModel), 128);

    bench_transition_model_predict();
    bench_transition_model_observe();
    bench_fast_path_integer();
    bench_fast_path_short_string();
    bench_homogeneous_ndjson();
    bench_heterogeneous_json();
    bench_integer_heavy();
    bench_online_learning_warmup();

    std::printf("\n=== Benchmarks complete ===\n");
    return 0;
}
