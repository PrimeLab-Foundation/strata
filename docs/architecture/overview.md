# Strata Architecture Overview

Strata is a C++20 JSON/NDJSON parsing library that combines three cooperating subsystems to maximize throughput: a SIMD structural indexer that classifies every byte in the input using branchless vector operations, a speculative parser that predicts value types via a Markov transition model and dispatches to specialized fast-path parsers, and a Bloom filter subsystem that enables selective field extraction by rejecting unwanted keys without string comparison. These subsystems are layered so that each one operates on the output of the previous stage, and the Bloom filters act as a cross-cutting optimization that can short-circuit parsing at any depth.

## Data Flow

```
                         Raw JSON bytes (UTF-8)
                                |
                                v
                  +-----------------------------+
                  |   SIMD Structural Indexer    |
                  |  (64-byte chunks, vpshufb   |
                  |   classifier, PCLMUL string  |
                  |   masking, escape detection) |
                  +-----------------------------+
                                |
                     uint32_t position array
                    (byte offsets of { } [ ] : ,)
                                |
               +----------------+----------------+
               |                                 |
               v                                 v
  +------------------------+       +---------------------------+
  |    Generic Parser      |       |   Speculative Parser      |
  |  (recursive descent,   |       | (Markov prediction ->     |
  |   full value parsing)  |       |  fast-path dispatch ->    |
  |                        |       |  verify -> fallback ->    |
  |                        |       |  online model update)     |
  +------------------------+       +---------------------------+
               |                                 |
               +----------------+----------------+
                                |
                           JsonValue tree
                                |
                                v
                  +-----------------------------+
                  |   Bloom Filter Subsystem     |
                  |   (cross-cutting, optional)  |
                  |                              |
                  |  KeyFilter: reject unwanted  |
                  |    keys via Bloom + binary   |
                  |    search exact match        |
                  |                              |
                  |  SelectiveParser: skip       |
                  |    rejected values using     |
                  |    structural index offsets   |
                  |    (zero-cost value bypass)  |
                  |                              |
                  |  DedupFilter: detect         |
                  |    duplicate keys per-object |
                  |    via stacked 256-bit       |
                  |    filters per nesting level |
                  +-----------------------------+
```

When the Bloom filter subsystem is active, it intercepts parsing between the structural index and value construction. The KeyFilter checks each object key against a Bloom filter; definite negatives cause the SelectiveParser to advance through the structural index without parsing the associated value. Only Bloom-positive keys proceed to an exact binary-search check, and only confirmed matches are actually parsed.

## Component Inventory

| Class               | Header                           | Subsystem   | Description                                                                                                                                            |
| ------------------- | -------------------------------- | ----------- | ------------------------------------------------------------------------------------------------------------------------------------------------------ |
| `Classifier`        | `simd/classifier.h`              | SIMD        | Branchless vpshufb nibble-lookup classifier; categorizes 32/64 bytes into STRUCTURAL, QUOTE, BACKSLASH, WHITESPACE, OTHER with zero false positives    |
| `StructuralIndexer` | `simd/structural_indexer.h`      | SIMD        | Processes 64-byte chunks; produces structural, string-mask, quote, and whitespace bitmasks using PCLMUL prefix-XOR and add-with-carry escape detection |
| `IndexBuilder`      | `simd/index_builder.h`           | SIMD        | Drives StructuralIndexer over a full document; extracts bit positions via BMI2 PEXT or CTZ loop into a flat `uint32_t` offset array                    |
| `dispatch`          | `simd/dispatch.h`                | SIMD        | Compile-time and runtime backend selection across 7 targets (AVX-512, AVX2+BMI2, AVX2, SVE2, NEON, WASM SIMD, RVV, Scalar)                             |
| `TransitionModel`   | `speculative/transition_model.h` | Speculative | Markov model with 4096 lossy buckets, uint16_t counts per ValueType, fixed-point confidence, exponential decay every 256 observations                  |
| `FastPaths`         | `speculative/fast_paths.h`       | Speculative | Type-specialized parsers: SWAR 8-digit integer, Lemire fast_float, branchless 4-byte literal compare, single-load short string                         |
| `SpeculativeParser` | `speculative/parser.h`           | Speculative | Coordinator: predict -> fast-path -> verify -> fallback -> update model; supports online learning and NDJSON cross-document adaptation                 |
| `ParserContext`     | `speculative/types.h`            | Speculative | Encodes parsing position (key hash, nesting depth, array index, last value type) for Markov model lookup                                               |
| `BloomFilter<N,K>`  | `bloom/bloom_filter.h`           | Bloom       | Compile-time configurable filter with wyhash, Kirschner-Mitzenmacher double hashing, blocked design for cache-line containment                         |
| `Preset<N,FPR>`     | `bloom/sizing.h`                 | Bloom       | constexpr information-theoretic optimal m/k computation; TinyFilter/SmallFilter/MediumFilter/LargeFilter type aliases                                  |
| `DedupFilter`       | `bloom/dedup_filter.h`           | Bloom       | Stacked 256-bit filters (one per nesting level, up to depth 32) for duplicate key detection; 1088 bytes total                                          |
| `KeyFilter`         | `bloom/key_filter.h`             | Bloom       | Two-stage pipeline: Bloom filter fast rejection (~99% of unwanted keys), then binary search exact match on positives                                   |
| `SchemaFilter`      | `bloom/schema_filter.h`          | Bloom       | Depth-aware key validation using per-depth SmallFilters plus a global MediumFilter; batch validation of up to 64 keys via bitmask                      |
| `SelectiveParser`   | `bloom/parser_integration.h`     | Bloom       | Structural-index-based value skipping: counts braces/brackets to jump past unwanted values without parsing them                                        |

## Key Design Decisions

1. **64-byte chunk processing.** The structural indexer processes exactly 64 bytes per call, producing 64-bit bitmasks that map one-to-one with input positions. This aligns with AVX-512 register width (the widest backend) and allows AVX2 to process two 32-byte halves. The final partial chunk is zero-padded by the caller, which is safe because zero bytes produce no structural classifications.

1. **Lossy hash table for the Markov model.** The TransitionModel uses 4096 buckets with no collision resolution -- colliding contexts silently share a bucket. This bounds memory to ~120 KB (fits in L2 cache) and eliminates chaining/probing overhead. Collisions degrade prediction accuracy for the affected contexts but never produce incorrect parses, because every speculative result is verified.

1. **Speculation is always verified.** The speculative parser never trusts a fast-path result without checking that the returned bytes_consumed and value type match the actual input. On mismatch, it falls back to the generic recursive-descent parser. This means the Markov model can be arbitrarily wrong without affecting correctness -- only performance.

1. **Bloom filters sized at compile time.** The `Preset<N, FPR>` template computes optimal bit count and hash count using constexpr information-theoretic formulas, then rounds to 256-bit alignment for AVX2 operations. This makes filter parameters zero-cost decisions resolved entirely by the compiler.

1. **Blocked Bloom filter design for cache efficiency.** Filters larger than 512 bits partition the bit array into 64-byte (512-bit) blocks. All k probes for a given key land in the same block, guaranteeing exactly one cache line touch per query regardless of filter size.

1. **Structural-index-based value skipping.** The SelectiveParser skips unwanted values by walking the structural index (counting braces and brackets) rather than parsing the value. This is O(number of structural characters in the skipped value) with zero deserialization cost -- just pointer arithmetic on a `uint32_t` array.

1. **Online learning across NDJSON lines.** The TransitionModel accumulates observations across newline-delimited documents within a stream. After a configurable warmup period (default 100 values), the speculative parser begins making predictions. The exponential decay (halving counts every 256 observations per bucket) lets the model adapt when schema patterns shift mid-stream.

## Subsystem Dependencies

```
  +--------+       +-------------+       +-------+
  |  SIMD  | ----> | Speculative | <---- | Bloom |
  +--------+       +-------------+       +-------+
       |                                      |
       +--------------------------------------+
         SelectiveParser consumes the same
         structural index produced by SIMD
```

- **SIMD -> Speculative**: The SpeculativeParser receives the structural position array produced by IndexBuilder. It uses these offsets to locate value start positions and to skip values during selective parsing.
- **Bloom -> Speculative**: The SpeculativeParser accepts an optional `KeyFilter*` via `set_key_filter()`. When set, it calls `should_parse()` for each object key and uses `skip_value_structural()` to bypass rejected values.
- **SIMD -> Bloom**: The SelectiveParser directly consumes the structural index from IndexBuilder to implement zero-cost value skipping via brace/bracket counting.
- **Bloom is self-contained**: The BloomFilter, sizing utilities, DedupFilter, and SchemaFilter have no dependencies on SIMD or Speculative. They can be used independently for any Bloom filter use case.
- **Speculative is self-contained for prediction**: The TransitionModel and FastPaths have no dependency on SIMD. They operate on raw byte pointers and can be used with any structural index source.
