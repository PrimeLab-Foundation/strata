# Strata — Coverage Report

> Generated: 2026-03-19
> Commit: `2a4332c` (main-v2-prompt-4)
> Threshold: 95% line coverage

## Regeneration

```bash
# Option 1: Makefile (LLVM instrumentation)
make coverage-gate

# Option 2: CMake (GCC or Clang)
cmake -B build_cov -DSTRATA_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build_cov
cd build_cov && ctest --output-on-failure
cmake --build . --target coverage_check
```

## Aggregate Coverage by Subsystem

| Subsystem                               | Files | Line Coverage | Status              |
| --------------------------------------- | ----- | ------------- | ------------------- |
| SIMD (`src/strata/simd/`)               | 4     | —             | Pending measurement |
| Speculative (`src/strata/speculative/`) | 3     | —             | Pending measurement |
| Bloom (`src/strata/bloom/`)             | 5     | —             | Pending measurement |

## Per-File Coverage

| File                                          | Lines | Covered | %   | Tests Covering                                |
| --------------------------------------------- | ----- | ------- | --- | --------------------------------------------- |
| `src/strata/simd/classifier.cpp`              | 200   | —       | —   | test_simd, test_simd_edge                     |
| `src/strata/simd/structural_indexer.cpp`      | 493   | —       | —   | test_simd, test_simd_edge, test_integration   |
| `src/strata/simd/index_builder.cpp`           | 108   | —       | —   | test_simd, test_simd_edge, test_integration   |
| `src/strata/simd/dispatch.cpp`                | 133   | —       | —   | test_simd, test_integration                   |
| `src/strata/speculative/transition_model.cpp` | 598   | —       | —   | test_speculative, test_speculative_edge       |
| `src/strata/speculative/fast_paths.cpp`       | 597   | —       | —   | test_speculative, test_speculative_edge       |
| `src/strata/speculative/parser.cpp`           | 717   | —       | —   | test_speculative, test_integration            |
| `src/strata/bloom/bloom_filter.cpp`           | 25    | —       | —   | test_bloom, test_bloom_edge                   |
| `src/strata/bloom/key_filter.cpp`             | 121   | —       | —   | test_bloom, test_bloom_edge, test_integration |
| `src/strata/bloom/schema_filter.cpp`          | 230   | —       | —   | test_bloom, test_bloom_edge                   |
| `src/strata/bloom/dedup_filter.cpp`           | 13    | —       | —   | test_bloom, test_bloom_edge                   |
| `src/strata/bloom/parser_integration.cpp`     | 416   | —       | —   | test_bloom, test_integration                  |

## Known Coverage Gaps

### Platform-Specific SIMD Backends

The following code paths cannot be tested on the current platform (ARM/macOS):

- `classifier.cpp`: AVX2, AVX-512, SVE2, WASM SIMD, RVV backends (only NEON active)
- `structural_indexer.cpp`: Same platform-conditional blocks
- `dispatch.cpp`: x86 cpuid detection path (only ARM detection active)

These are excluded from the coverage threshold via `#ifdef` conditional compilation — they are simply not compiled on non-matching platforms.

### Template-Heavy Headers

Most Bloom filter logic lives in template headers (`bloom_filter.h`, `sizing.h`, `dedup_filter.h`). These are tested via their instantiations in the test files but may not appear in `.cpp` coverage reports since the code is generated at the call site.

## Test Matrix

| Test File                   | Tests   | Subsystems  | Type        |
| --------------------------- | ------- | ----------- | ----------- |
| `test_simd.cpp`             | 24      | SIMD        | Unit        |
| `test_simd_edge.cpp`        | 15      | SIMD        | Edge-case   |
| `test_speculative.cpp`      | 31      | Speculative | Unit        |
| `test_speculative_edge.cpp` | 15      | Speculative | Edge-case   |
| `test_bloom.cpp`            | 30      | Bloom       | Unit        |
| `test_bloom_edge.cpp`       | 12      | Bloom       | Edge-case   |
| `test_integration.cpp`      | 10      | All         | Integration |
| **Total**                   | **137** |             |             |

## Fuzz Harnesses

| Harness                       | Target                        | Input           |
| ----------------------------- | ----------------------------- | --------------- |
| `fuzz_structural_indexer.cpp` | `IndexBuilder::build()`       | Arbitrary bytes |
| `fuzz_speculative_parser.cpp` | `SpeculativeParser::parse()`  | Arbitrary bytes |
| `fuzz_bloom_filter.cpp`       | `BloomFilter` + `DedupFilter` | Random keys/ops |
| `fuzz_loads.cpp`              | `parse_json()`                | Arbitrary bytes |
| `fuzz_ndjson.cpp`             | `NdjsonStream`                | Arbitrary bytes |
