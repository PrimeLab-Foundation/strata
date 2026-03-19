# Performance Tuning Guide

This guide covers how to maximize Strata's parsing throughput: SIMD backend selection, profile-guided optimization, speculative parser configuration, Bloom filter sizing, and benchmarking methodology.

______________________________________________________________________

## SIMD Backend Detection

Strata selects the best SIMD backend at compile time based on target architecture flags. At runtime, `detect_backend()` verifies CPU feature support (on x86-64) and caches the result.

### Verifying the Active Backend

```cpp
#include <strata/simd/dispatch.h>
#include <cstdio>

int main() {
    auto backend = strata::simd::detect_backend();
    std::printf("Backend: %s\n", strata::simd::backend_name(backend));
}
```

### Backend Hierarchy

| Backend     | Architecture        | Key Instructions                  | Throughput        |
| ----------- | ------------------- | --------------------------------- | ----------------- |
| `AVX512`    | x86-64 (Skylake-X+) | 64-byte vpshufb, native kmask     | Highest on x86    |
| `AVX2_BMI2` | x86-64 (Haswell+)   | 32-byte vpshufb, PCLMUL, PEXT     | High              |
| `AVX2`      | x86-64 (Haswell+)   | 32-byte vpshufb, PCLMUL, ctz-loop | High              |
| `SVE2`      | ARM (ARMv9+)        | svtbl, scalable 128-2048 bit      | High (server ARM) |
| `NEON`      | ARM (ARMv8+)        | vqtbl1q_u8, PMULL                 | Good              |
| `WASM_SIMD` | WebAssembly         | i8x16.swizzle, native bitmask     | Good              |
| `RVV`       | RISC-V (RVV 1.0)    | vluxei8 gather                    | Varies            |
| `SCALAR`    | Any                 | Portable C++ fallback             | Baseline          |

### Compile Flags for Maximum Performance

```bash
# x86-64: Enable AVX2 + BMI2 + PCLMUL (most modern Intel/AMD)
-march=native

# Or target a specific microarchitecture:
-march=haswell       # AVX2 + BMI2
-march=skylake-avx512  # AVX-512

# ARM: NEON is typically enabled by default on AArch64
-march=armv8-a

# Force scalar fallback (for testing/debugging)
-DSTRATA_FORCE_SCALAR=1
```

The `AVX2_BMI2` backend adds PEXT-based bit extraction, which converts 64-bit structural bitmasks to position arrays faster than the ctz-loop fallback. On CPUs with fast PEXT (Intel Haswell through current; AMD Zen 3+), this provides a measurable speedup.

______________________________________________________________________

## Profile-Guided Optimization (PGO)

PGO allows the compiler to optimize branch prediction, function layout, and inlining decisions based on actual workload data.

### Two-Pass PGO Build

```bash
# Pass 1: Instrument
mkdir build-pgo && cd build-pgo
cmake .. -DCMAKE_BUILD_TYPE=Release \
         -DCMAKE_CXX_FLAGS="-fprofile-generate=/tmp/pgo-data"
cmake --build .

# Run representative workloads
./strata_bench --input your_data.json
./strata_bench --input your_ndjson.jsonl

# Pass 2: Optimize with profile data
cd .. && mkdir build-pgo-opt && cd build-pgo-opt
cmake .. -DCMAKE_BUILD_TYPE=Release \
         -DCMAKE_CXX_FLAGS="-fprofile-use=/tmp/pgo-data"
cmake --build .
```

### What PGO Improves

- **Branch layout:** Hot branches (speculation hits) are placed on the fall-through path.
- **Function inlining:** The compiler sees which fast-path functions are called most and inlines them more aggressively.
- **Code layout:** Hot code is packed together for better instruction cache utilization.

Typical PGO improvement for Strata: 5-15% throughput gain depending on workload and compiler.

### LTO (Link-Time Optimization)

Enable LTO for cross-translation-unit inlining:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Release -DSTRATA_ENABLE_LTO=ON
```

LTO allows the compiler to inline the fast-path parsers (`parse_integer`, `parse_short_string`, etc.) into the speculative coordinator, eliminating function-pointer dispatch overhead. Combined with PGO, this can yield 10-20% improvement.

______________________________________________________________________

## Speculative Parser Configuration

The `SpeculativeParser::Config` struct controls the prediction engine behavior:

```cpp
struct Config {
    float  confidence_threshold    = 0.85f;
    bool   enable_online_learning  = true;
    bool   enable_speculation      = true;
    size_t online_learning_warmup  = 100;
};
```

### Tuning confidence_threshold

The confidence threshold determines the minimum prediction probability required to attempt a fast-path dispatch.

| Threshold | Behavior                            | Best For                                           |
| --------- | ----------------------------------- | -------------------------------------------------- |
| 0.70      | Aggressive speculation, more misses | Highly homogeneous data (same schema every record) |
| 0.85      | Balanced (default)                  | Most workloads                                     |
| 0.95      | Conservative, fewer misses          | Mixed-schema data or when miss penalty is high     |

**Why misses are cheap:** A speculation miss means the fast-path parser returned `{.success = false}` after reading 1-4 bytes. The generic parser then takes over from the same position. The total penalty is roughly 10-20 ns per miss. At 85% confidence, the expected speedup from hits far outweighs the miss penalty.

### Tuning online_learning_warmup

The warmup period determines how many values the parser observes with the generic path before activating speculation.

- **Low warmup (10-50):** Starts speculating quickly. Good if records are identical from the start.
- **Default (100):** Enough to learn patterns across ~10 records with 10 fields each.
- **High warmup (500-1000):** Use if the first few records have different schemas than the rest (e.g., header records).

### Disabling Speculation for Benchmarking

To measure the baseline (non-speculative) parse speed for A/B comparison:

```cpp
strata::speculative::SpeculativeParser::Config config;
config.enable_speculation = false;  // always use generic parser
```

______________________________________________________________________

## Bloom Filter Sizing

### Choosing the Right Filter Size

The `Preset` template computes optimal bit count and hash count from your expected key count and target false-positive rate:

```cpp
// Custom sizing: 75 keys, 0.5% FPR
using CustomFilter = strata::bloom::Preset<75, 5>::type;  // 5 = 0.5%
```

### Sizing Rules of Thumb

| Keys | FPR  | Bits  | Hashes | Memory  | Cache Lines   |
| ---- | ---- | ----- | ------ | ------- | ------------- |
| 10   | 1%   | 256   | 3      | 32 B    | 1             |
| 50   | 1%   | 512   | 3      | 64 B    | 1             |
| 200  | 1%   | 2048  | 3      | 256 B   | 4 (blocked)   |
| 1000 | 1%   | 10240 | 3      | 1.25 KB | ~20 (blocked) |
| 50   | 0.1% | 768   | 5      | 96 B    | 2             |

### Cache-Line Awareness

For filters larger than 512 bits (64 bytes), Strata uses a blocked design: all hash probes for a single key land in the same 64-byte cache-line block. This means:

- `TinyFilter` (256 bits): 1 cache line accessed per query.
- `SmallFilter` (512 bits): 1 cache line accessed per query.
- `MediumFilter` (2048 bits): Still 1 cache line per query (blocked design).
- `LargeFilter` (10240 bits): Still 1 cache line per query (blocked design).

The blocked design trades a slightly higher effective FPR for guaranteed single-cache-line access.

### Over-sizing vs. Under-sizing

- **Under-sized filter:** Higher FPR means more false-positive keys pass the Bloom filter, triggering binary-search exact matches. The cost is 15-30 ns per false positive.
- **Over-sized filter:** Wastes memory but does not degrade performance (cache-line blocking keeps access patterns efficient).
- **Recommendation:** Use the preset that matches your key count. When uncertain, round up.

______________________________________________________________________

## Benchmark Methodology

### Isolating Measurement

When benchmarking Strata, isolate the component you are measuring:

```cpp
// Measure SIMD indexing only
auto start = std::chrono::high_resolution_clock::now();
auto index = strata::simd::index_document(data, len);
auto end = std::chrono::high_resolution_clock::now();

// Measure speculative parsing only (reuse index)
start = std::chrono::high_resolution_clock::now();
auto result = parser.parse(data, len,
                           index.positions.data(),
                           index.positions.size());
end = std::chrono::high_resolution_clock::now();
```

### Warm-up Iterations

Always run several warm-up iterations before timing. This ensures:

- CPU frequency scaling has reached maximum.
- Instruction and data caches are warm.
- The speculative model has completed its warmup period.

```cpp
// Warm up (discard results)
for (int i = 0; i < 10; ++i) {
    auto _ = parser.parse(data, len, positions, num_positions);
}

// Measure
auto start = std::chrono::high_resolution_clock::now();
for (int i = 0; i < iterations; ++i) {
    auto _ = parser.parse(data, len, positions, num_positions);
}
auto elapsed = std::chrono::high_resolution_clock::now() - start;
```

### Reporting Throughput

Report throughput in GB/s for comparability:

```cpp
double seconds = std::chrono::duration<double>(elapsed).count();
double gb_per_sec = (static_cast<double>(len) * iterations) / (seconds * 1e9);
std::printf("Throughput: %.2f GB/s\n", gb_per_sec);
```

### Using the Built-in Benchmarks

```bash
# Generate benchmark data
make bench-data

# Run small-input benchmarks (comparison with orjson, msgspec, ujson)
make bench-small

# Run all benchmarks
make bench-all
```

### Controlling Variance

- **Pin CPU frequency:** Disable turbo boost and frequency scaling for stable results.
- **Pin to a single core:** Use `taskset -c 0` (Linux) or equivalent to avoid core migration.
- **Disable hyperthreading:** Or at least avoid scheduling other work on the sibling core.
- **Multiple runs:** Take the minimum of 5+ runs (minimum is more stable than mean for benchmarks).

______________________________________________________________________

## Speculation Effectiveness Diagnostics

After parsing, inspect the model statistics to verify that speculation is working:

```cpp
auto stats = parser.model().stats();
std::printf("Predictions:      %llu\n", stats.total_predictions);
std::printf("Correct:          %llu\n", stats.correct_predictions);
std::printf("Hit rate:         %.1f%%\n", stats.hit_rate() * 100);
std::printf("Speculations:     %llu\n", stats.speculations_attempted);
std::printf("Speculation hits: %llu\n", stats.speculations_hit);
std::printf("Spec hit rate:    %.1f%%\n", stats.speculation_rate() * 100);
```

**What good numbers look like:**

- **Hit rate > 90%:** The model is accurately predicting value types. Speculation is paying off.
- **Hit rate 70-90%:** Moderate benefit. Consider lowering the confidence threshold or pre-training.
- **Hit rate \< 70%:** Data is too heterogeneous for speculation. Consider disabling it or pre-training from a schema.

### Pre-training the Model

For known schemas, pre-train the model to skip the warmup period:

```cpp
// From a JSON Schema
parser.model().train_from_schema(R"({
    "type": "object",
    "properties": {
        "id": {"type": "integer"},
        "name": {"type": "string"},
        "score": {"type": "number"}
    }
})");

// Or from sample documents
parser.model().train_from_samples(sample_data, sample_len, num_samples);
```

### Serializing Trained Models

Save a trained model to avoid re-learning on subsequent runs:

```cpp
// Save
auto model_bytes = parser.model().serialize();
write_file("model.bin", model_bytes.data(), model_bytes.size());

// Load
auto loaded_bytes = read_file("model.bin");
auto model = strata::speculative::TransitionModel::deserialize(
    loaded_bytes.data(), loaded_bytes.size());
```

______________________________________________________________________

## Checklist: Maximum Performance

1. Compile with `-march=native -O3` (or `-O2` if binary size matters).
1. Enable LTO: `-DSTRATA_ENABLE_LTO=ON`.
1. Run PGO with representative data.
1. Verify the backend with `detect_backend()` -- ensure you are not falling back to SCALAR.
1. Pre-train the speculative model from a schema or sample data.
1. Use selective parsing if you need fewer than ~50% of an object's keys.
1. Use `parse_all_fast()` for NDJSON batch processing.
1. Use `serialize_json_to()` with a pre-allocated buffer for serialization in loops.
1. Check `TransitionModel::Stats` -- if hit rate is below 70%, consider adjusting the threshold or disabling speculation.
1. For parallel NDJSON processing, give each thread its own `TransitionModel` and merge afterward.
