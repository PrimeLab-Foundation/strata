# Strata — Improvement Plan (by gemini-3-pro-preview)

- Date: 2026-01-31
- Commit/branch analyzed: 0dd6fec82903a7473ecc5709b020843b28cff0e7
- Environment assumptions: macOS 25.1.0 (arm64), Python 3.14.2, Apple clang 17.0.0

## 1. Executive summary

- **Critical Risk**: The primary parsing benchmark (`users.json`) is currently failing with an **ERROR** state, invalidating performance claims for the core `loads` functionality.
- **Performance Gap (NDJSON)**: NDJSON parsing is ~92% slower than the leader (`orjson`), primarily due to per-line function call overhead and lack of batching.
- **Performance Gap (Dumps)**: Serialization is ~11% slower than the leader. The main bottleneck is the final memory copy from the C++ `OutputBuffer` to the Python `bytes` object.
- **Win**: JSONPath performance is excellent (#1), validating the query engine architecture.
- **Win**: Memory usage is best-in-class, confirming the zero-copy design strategy.
- **Opportunity**: PGO/LTO infrastructure is in place but needs CI validation to prevent regressions.

## 2. Priority roadmap

| Priority | Area        | Item Title                     | Expected Impact | Effort | Evidence                                         |
| -------- | ----------- | ------------------------------ | --------------- | ------ | ------------------------------------------------ |
| **P0**   | Correctness | **Fix Benchmark ERRORs**       | Critical        | S      | `docs/benchmarks/bench_results_final.md:15`      |
| **P0**   | Perf        | **Optimize NDJSON (Batching)** | High (92% gap)  | L      | `src/strata/json/ndjson_stream.cpp`              |
| **P0**   | Perf        | **Optimize Dumps (Zero-Copy)** | High (11% gap)  | M      | `src/strata/bindings/python_dumps.cpp:409`       |
| P1       | Testing     | Regression Test for Benchmarks | High            | S      | Missing `tests/py/test_benchmark_correctness.py` |
| P1       | Build       | CI Validation for PGO+LTO      | Med             | M      | `scripts/pgo_build.sh`                           |
| P2       | Perf        | Dragonbox Float Formatting     | Low (\<5%)      | M      | `src/strata/util/ryu_dtoa.hpp`                   |
| P2       | Docs        | Consolidate Benchmark Docs     | Low             | S      | Multiple `docs/benchmarks/*.md` files            |

## 3. Detailed recommendations

### 3.1 Fix Benchmark ERRORs (P0)

- **Problem**: The `strata` library fails to parse `users.json` in the benchmark suite, returning "ERROR" instead of a timing.
- **Evidence**: `docs/benchmarks/bench_results_final.md` line 15: `| strata | users | ERROR |`.
- **Proposed Change**: Debug `benchmarks/bench_main.py` to capture the specific exception. It is likely a `JSONDecodeError` or a C++ exception propagating to Python. Fix the underlying parser bug in `json_parse.cpp`.
- **Acceptance Criteria**: Benchmark runs successfully with no "ERROR" rows.
- **Risks**: May reveal that the "fast" parser is incorrect, requiring a rollback to a slower, safer implementation.
- **Owner**: `src/strata/json/json_parse.cpp`

### 3.2 Optimize NDJSON with Chunked/Batch Processing (P0)

- **Problem**: NDJSON parsing processes one line at a time (`next_line` -> `parse_json`), incurring significant function call and `std::string_view` construction overhead for millions of short lines.
- **Evidence**: `src/strata/json/ndjson_stream.cpp` uses `find_newline_simd` for a single newline, then returns.
- **Proposed Change**: Implement `find_newlines_simd_batch` that fills an array of offsets (e.g., 64 offsets) in one pass. The parser loop then iterates over these offsets directly within the hot loop, avoiding repeated calls to the search function.
- **Acceptance Criteria**: NDJSON median time reduces by >40%, closing the gap to `orjson`.
- **Risks**: Complexity in handling chunk boundaries and buffer refills.
- **Owner**: `src/strata/json/ndjson_stream.cpp`

### 3.3 Zero-Copy Dumps (P0)

- **Problem**: `strata.dumps` writes to a C++ `OutputBuffer` (malloc/realloc) and then *copies* the result into a new Python `bytes` object via `PyBytes_FromStringAndSize`.
- **Evidence**: `src/strata/bindings/python_dumps.cpp:409`.
- **Proposed Change**:
  1. Estimate size.
  1. Allocate uninitialized `PyBytes` object of estimated size.
  1. Write directly into `PyBytes_AS_STRING(obj)`.
  1. If overflow, use `_PyBytes_Resize` (efficient realloc).
- **Acceptance Criteria**: `dumps` performance improves by ~5-10%, matching or beating `orjson`.
- **Risks**: `_PyBytes_Resize` can fail or move memory; pointers must be refreshed.
- **Owner**: `src/strata/bindings/python_dumps.cpp`

### 3.4 Regression Test for Benchmarks (P1)

- **Problem**: The "ERROR" state in benchmarks was committed/merged without failing a build gate.
- **Evidence**: Existence of `bench_results_final.md` with errors.
- **Proposed Change**: Add a "smoke test" benchmark script (`tests/py/test_bench_smoke.py`) that runs the benchmark on a small subset of data and asserts `exit_code == 0` and no "ERROR" strings in output.
- **Acceptance Criteria**: `make gate` fails if benchmarks are broken.
- **Owner**: `tests/py/`

## 4. “Algorithms & research” ideas

1. **SIMD Structural Indexing (PDEP/PEXT)**: Use BMI2 instructions to extract structural characters (`{`, `}`, `[`, `]`, `:`, `,`) into a bitmask, allowing "jumping" over non-structural content. (Requires AVX2/BMI2).
1. **SWAR (SIMD Within A Register)**: For short strings (\< 16 bytes), use 64-bit integer operations to detect quotes/escapes instead of full SSE/AVX loads.
1. **Branchless State Machine**: Convert the JSON parser state machine to a computed goto or table-based lookup to reduce branch mispredictions.
1. **Dragonbox**: Replace `std::to_chars` (or Ryu) with Dragonbox, which is currently the fastest known float-to-string algorithm.
1. **Tape-Based Parsing**: Instead of creating a tree of `JsonValue` objects (pointers), serialize the parsed structure into a flat "tape" of 64-bit words. This improves cache locality for subsequent access (e.g., JSONPath).
1. **Speculative Parsing**: For NDJSON, assume lines have similar structure. JIT-compile a parser for the specific schema observed in the first N lines.
1. **Interning Keys**: Use a thread-local cache for object keys. If "id" is seen 1000 times, reuse the same `PyObject*` string instance.

## 5. Next actions

- [ ] **Debug Benchmark**: Run `python -m benchmarks.bench_main` locally to reproduce the `users.json` ERROR.
- [ ] **Fix Parser**: Identify if the error is a crash or exception in `json_parse.cpp` and fix it.
- [ ] **Add Smoke Test**: Create `tests/py/test_bench_smoke.py` to prevent regression.
- [ ] **Refactor Dumps**: Create a branch to implement direct-write to `PyBytes`.
- [ ] **Refactor NDJSON**: Create a branch to implement batch newline finding.
- [ ] **Verify CI**: Ensure `make gate` runs the new smoke test.
- [ ] **Doc Cleanup**: Delete `docs/benchmarks/bench_results_final.md` after fixing the error and regenerating.
