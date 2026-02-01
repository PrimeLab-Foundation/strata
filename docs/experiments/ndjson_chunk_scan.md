# NDJSON Chunked Newline Scan Experiment

## Hypothesis

Collecting newline positions in SIMD chunks and parsing lines in batches will
reduce per-line overhead compared to calling a SIMD `find_newline` loop for
every line. Expected gain: 10–20% lower scan time for 5–10 MB NDJSON buffers.

## Prototype

- Code: `experiments/ndjson_chunk_scan/bench_ndjson_scan.cpp`
- Technique: `util::collect_newlines_simd` gathers newline offsets in batches
  (AVX2/SSE4.2/NEON or scalar fallback).

## Microbench Setup

- Dataset: `benchmarks/data/generated/users.ndjson`
- Command:
  ```
  clang++ -std=c++20 -O3 -Iinclude -Isrc \
    experiments/ndjson_chunk_scan/bench_ndjson_scan.cpp \
    src/strata/util/simd_string.cpp \
    -o experiments/ndjson_chunk_scan/bench_ndjson_scan
  ./experiments/ndjson_chunk_scan/bench_ndjson_scan benchmarks/data/generated/users.ndjson
  ```

## Results (2026-01-31)

- Baseline (find_newline loop): min 0.725 ms, median 0.854 ms
- Chunked (collect_newlines): min 2.011 ms, median 2.426 ms

## Conclusion

- **No-go** for scan-only replacement: collecting newline positions adds overhead on
  5 MB buffers. Keep `find_newline_simd` loop in production, and treat
  `collect_newlines_simd` as experimental.
