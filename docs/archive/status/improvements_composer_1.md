# Strata — Improvement Plan (by Coposer 1 Cursor Agent)

- Date: 2026-01-31
- Commit/branch analyzed: 0dd6fec82903a7473ecc5709b020843b28cff0e7
- Environment assumptions: macOS 25.1.0 (arm64), Python 3.14.2, Apple clang 17.0.0

______________________________________________________________________

## 1. Executive Summary

**Biggest Risks:**

- Benchmark correctness failures (ERROR rows) indicate potential runtime bugs or harness issues
- NDJSON performance gap (92% behind #1) suggests algorithmic inefficiency
- dumps still 11.3% behind #1 despite PGO+LTO, indicating remaining hot-path overhead
- High variance in benchmark runs suggests non-deterministic behavior or measurement noise

**Biggest Wins:**

- JSONPath already #1 (4-14x faster than jmespath) — maintain this lead
- Memory efficiency is best-in-class (#1) — preserve this advantage
- Parsing can achieve #1 with PGO+LTO — validate and document
- LTO/PGO infrastructure is in place — leverage for all builds

**Critical Path to #1:**

1. Fix benchmark correctness (ERROR rows) — blocker for production confidence
1. Close dumps gap (11.3% remaining) — highest ROI for overall ranking
1. Optimize NDJSON (92% gap) — largest performance deficit
1. Stabilize parsing variance — ensure consistent #1 ranking

______________________________________________________________________

## 2. Priority Roadmap

| Priority | Area        | Item Title                           | Expected Impact | Effort | Evidence                                    |
| -------- | ----------- | ------------------------------------ | --------------- | ------ | ------------------------------------------- |
| P0       | Correctness | Fix benchmark ERROR rows             | High            | S      | `docs/benchmarks/bench_results_final.md:15` |
| P0       | Perf        | Close dumps gap (11.3% → \<5%)       | High            | M      | `docs/benchmarks/dumps_results.md:33`       |
| P0       | Perf        | Optimize NDJSON (92% gap)            | High            | L      | `docs/benchmarks/ndjson_results.md:30`      |
| P1       | Perf        | Reduce parsing variance              | Med             | M      | `docs/status/current_status.md:67`          |
| P1       | Testing     | Add regression tests for ERROR cases | High            | S      | `docs/benchmarks/bench_results_final.md:15` |
| P1       | Build       | Validate PGO+LTO in CI               | Med             | M      | `docs/build/pgo.md`, `docs/build/lto.md`    |
| P2       | Perf        | Direct PyBytes write path for dumps  | Med             | M      | `src/strata/bindings/python_dumps.cpp:409`  |
| P2       | Perf        | Chunked NDJSON parsing               | Med             | M      | `src/strata/json/ndjson_stream.cpp:147`     |
| P2       | Testing     | 100% coverage enforcement            | Low             | M      | `docs/development/coverage.md`              |
| P2       | Docs        | Consolidate benchmark docs           | Low             | S      | Multiple `docs/benchmarks/*.md` files       |

______________________________________________________________________

## 3. Detailed Recommendations

### 3.1 Fix Benchmark Correctness (P0)

**Problem:**
Benchmark results show "ERROR" rows (e.g., `strata | users | ERROR` in `docs/benchmarks/bench_results_final.md:15`), indicating runtime failures or harness bugs.

**Evidence:**

- `docs/benchmarks/bench_results_final.md:15` shows ERROR entry
- Benchmark harness may not fail-fast on errors
- No regression tests prevent ERROR rows from appearing

**Proposed Change:**

1. Reproduce ERROR deterministically
1. Fix root cause (likely in parsing or benchmark harness)
1. Add regression test: `tests/py/test_benchmark_correctness.py` that fails if any benchmark returns ERROR
1. Modify `benchmarks/bench_main.py` to exit non-zero on any library ERROR
1. Add `benchmarks/validate_results.py` script that checks for ERROR rows and fails CI

**Acceptance Criteria:**

- All benchmark runs complete without ERROR rows
- Harness exits non-zero on any library failure
- Regression test fails if ERROR reappears
- CI gates on benchmark correctness

**Risks/Tradeoffs:**

- May expose real bugs in parsing/NDJSON paths
- Benchmark runs may become slower if validation is added

**Suggested Owner:**

- `benchmarks/bench_main.py` (harness)
- `tests/py/test_benchmark_correctness.py` (regression)

______________________________________________________________________

### 3.2 Close dumps Gap (P0)

**Problem:**
dumps is #3, 11.3% behind #1 (orjson) even with PGO+LTO. Remaining overhead likely in:

- PyObject\* type checks and branches
- String escaping (SIMD already present but may have overhead)
- Float formatting (using `std::to_chars` but may need Dragonbox/Ryu)
- OutputBuffer → PyBytes/Unicode conversion

**Evidence:**

- `docs/benchmarks/dumps_results.md:33`: PGO+LTO median 6.87ms vs orjson ~6.18ms
- `src/strata/bindings/python_dumps.cpp:374-415`: Current implementation uses iterative stack
- `src/strata/bindings/python_dumps.cpp:409`: Final conversion via `PyBytes_FromStringAndSize` (copy)

**Proposed Change:**

1. **Direct PyBytes write path**: Pre-allocate PyBytes with estimated size, write directly into its memory, avoid final copy
1. **Optimize type checks**: Use `Py_TYPE(obj) == &PyLong_Type` fast-path (already present), add similar for dict/list
1. **Float formatting**: Implement Dragonbox or Ryu for shortest-roundtrip JSON floats (header-only)
1. **Profile-guided micro-optimizations**: Use `perf`/Instruments to identify remaining hot spots

**Acceptance Criteria:**

- dumps median within 5% of orjson (or #1)
- No regression in memory usage
- All tests pass (C++ + Python)
- Benchmark variance reduced

**Risks/Tradeoffs:**

- Direct PyBytes write requires careful lifetime management
- Dragonbox/Ryu adds code complexity
- May regress correctness if not tested thoroughly

**Suggested Owner:**

- `src/strata/bindings/python_dumps.cpp`
- `include/strata/util/ryu_dtoa.hpp` (if switching to Ryu)

______________________________________________________________________

### 3.3 Optimize NDJSON (P0)

**Problem:**
NDJSON is #5, 92% behind #1 (orjson). Current implementation:

- Uses SIMD newline search (good)
- Parses line-by-line (potential overhead)
- May have redundant allocations per line

**Evidence:**

- `docs/benchmarks/ndjson_results.md:30`: PGO+LTO median 46.70ms vs orjson 24.33ms
- `src/strata/json/ndjson_stream.cpp:31-61`: Line-by-line parsing with SIMD newline search
- `src/strata/json/ndjson_stream.cpp:94-111`: `parse_all` allocates vector per call

**Proposed Change:**

1. **Batch parsing**: Parse multiple lines in single pass, reduce function call overhead
1. **Zero-copy line views**: Avoid copying line data when possible
1. **Arena allocator**: Use arena for temporary JsonValue objects during batch parsing
1. **Pipelined parsing**: Overlap newline search with JSON parsing

**Acceptance Criteria:**

- NDJSON median within 20% of orjson (or #1)
- Memory usage remains best-in-class
- All tests pass
- Streaming API remains functional

**Risks/Tradeoffs:**

- Batch parsing may complicate error handling
- Arena allocator adds complexity
- May require API changes if not careful

**Suggested Owner:**

- `src/strata/json/ndjson_stream.cpp`
- `include/strata/util/arena_allocator.hpp` (if adding arena)

______________________________________________________________________

### 3.4 Reduce Parsing Variance (P1)

**Problem:**
Parsing shows higher run-to-run variance than orjson, making consistent #1 ranking difficult.

**Evidence:**

- `docs/status/current_status.md:67`: Variance mentioned as issue
- PGO+LTO can achieve #1 but variance may cause inconsistent results

**Proposed Change:**

1. **Profile variance sources**: Use statistical analysis to identify high-variance code paths
1. **Reduce non-determinism**: Eliminate any random behavior, hash table ordering issues
1. **Warmup improvements**: Ensure proper warmup before measurement
1. **Cache effects**: Profile cache misses, optimize data layout

**Acceptance Criteria:**

- Parsing variance reduced to match orjson
- Consistent #1 ranking across multiple runs
- No correctness regressions

**Risks/Tradeoffs:**

- May require significant profiling effort
- Some variance may be inherent to system

**Suggested Owner:**

- `src/strata/json/json_parse.cpp`
- Benchmark harness (`benchmarks/bench_main.py`)

______________________________________________________________________

### 3.5 Add Regression Tests for ERROR Cases (P1)

**Problem:**
No automated tests prevent benchmark ERROR rows from reappearing.

**Evidence:**

- `docs/benchmarks/bench_results_final.md:15` shows ERROR but no test catches it
- Benchmark harness doesn't fail-fast on errors

**Proposed Change:**

1. Create `tests/py/test_benchmark_correctness.py`:
   - Runs minimal benchmark suite
   - Checks for ERROR in output
   - Fails if any ERROR found
1. Add to CI: Run before/after build
1. Document in `docs/development/test_gated_builds.md`

**Acceptance Criteria:**

- Test fails if ERROR rows appear
- Test runs as part of `make gate`
- CI blocks on test failure

**Risks/Tradeoffs:**

- May slow down test suite
- Requires maintaining benchmark data

**Suggested Owner:**

- `tests/py/test_benchmark_correctness.py` (new)

______________________________________________________________________

### 3.6 Validate PGO+LTO in CI (P1)

**Problem:**
PGO+LTO infrastructure exists but not validated in CI, risking regressions.

**Evidence:**

- `docs/build/pgo.md` and `docs/build/lto.md` exist
- `scripts/pgo_build.sh` exists
- No CI validation mentioned

**Proposed Change:**

1. Add CI job that runs PGO workflow
1. Validate LTO is enabled in bench preset builds
1. Compare PGO+LTO vs baseline benchmarks
1. Fail CI if PGO+LTO doesn't improve performance

**Acceptance Criteria:**

- CI runs PGO workflow (may be slow, run nightly)
- LTO validation in CI
- Performance regression detection

**Risks/Tradeoffs:**

- PGO workflow is slow (may need nightly CI)
- Requires CI infrastructure support

**Suggested Owner:**

- `.github/workflows/` or CI config
- `scripts/pgo_build.sh` (may need CI adaptations)

______________________________________________________________________

### 3.7 Direct PyBytes Write Path for dumps (P2)

**Problem:**
Final conversion in `python_dumps.cpp:409` copies data from OutputBuffer to PyBytes, adding overhead.

**Evidence:**

- `src/strata/bindings/python_dumps.cpp:409`: `PyBytes_FromStringAndSize` copies data
- Profile likely shows this copy as hot spot

**Proposed Change:**

1. Pre-allocate PyBytes with estimated size
1. Write directly into PyBytes memory via `PyBytes_AS_STRING`
1. Handle overflow by reallocating PyBytes (rare case)
1. Avoid OutputBuffer entirely for small objects (fast path)

**Acceptance Criteria:**

- Measurable speedup in dumps benchmarks
- No correctness regressions
- Memory usage stable

**Risks/Tradeoffs:**

- PyBytes lifetime management complexity
- May require careful error handling

**Suggested Owner:**

- `src/strata/bindings/python_dumps.cpp`

______________________________________________________________________

### 3.8 Chunked NDJSON Parsing (P2)

**Problem:**
Current NDJSON parses line-by-line, adding function call overhead per line.

**Evidence:**

- `src/strata/json/ndjson_stream.cpp:31-61`: Line-by-line parsing
- Batch methods exist but may not be optimal

**Proposed Change:**

1. Parse multiple lines in single pass
1. Use SIMD to find all newlines upfront
1. Parse JSON values in batch
1. Reduce allocations per line

**Acceptance Criteria:**

- NDJSON performance improved (target: within 20% of #1)
- Streaming API remains functional
- Memory usage stable

**Risks/Tradeoffs:**

- May complicate error handling
- Batch size tuning required

**Suggested Owner:**

- `src/strata/json/ndjson_stream.cpp`

______________________________________________________________________

### 3.9 100% Coverage Enforcement (P2)

**Problem:**
Coverage target exists but enforcement may be incomplete.

**Evidence:**

- `docs/development/coverage.md` mentions coverage
- `docs/status/current_status.md:35` shows 100% Python tests
- C++ coverage may be incomplete

**Proposed Change:**

1. Add coverage gates to CI
1. Document coverage exclusions
1. Add missing C++ tests for uncovered paths
1. Enforce coverage in `make gate`

**Acceptance Criteria:**

- Coverage gates in CI
- Documented exclusions
- Coverage reports in CI artifacts

**Risks/Tradeoffs:**

- May slow down CI
- Requires maintaining coverage data

**Suggested Owner:**

- CI configuration
- `docs/development/coverage.md`

______________________________________________________________________

### 3.10 Consolidate Benchmark Docs (P2)

**Problem:**
Multiple benchmark result files may cause confusion.

**Evidence:**

- `docs/benchmarks/bench_results*.md` (multiple files)
- `docs/benchmarks/dumps_results*.md` (multiple files)
- May have outdated or duplicate information

**Proposed Change:**

1. Audit all benchmark docs
1. Consolidate into single canonical results file
1. Archive outdated results
1. Update `docs/README.md` to point to canonical file

**Acceptance Criteria:**

- Single canonical benchmark results file
- Outdated files archived
- README updated

**Risks/Tradeoffs:**

- May lose historical context
- Requires careful review

**Suggested Owner:**

- `docs/benchmarks/` directory
- `docs/README.md`

______________________________________________________________________

## 4. Algorithms & Research Ideas

### Parsing Optimizations

1. **SIMD JSON classification**: Use SIMD to classify JSON tokens (string/number/object/array) in parallel

   - Where: `src/strata/json/json_parse.cpp:33-51` (parse_value dispatch)
   - Expected win: 5-10% parsing speedup
   - Complexity: Medium (requires SIMD expertise)

1. **Branchless DFA**: Replace character-by-character parsing with branchless DFA

   - Where: `src/strata/json/json_parse.cpp` (all parse\_\* functions)
   - Expected win: 10-20% parsing speedup
   - Complexity: High (requires DFA design)

1. **Tape model**: Pre-allocate tape for JSON values, reduce allocations

   - Where: `src/strata/json/json_parse.cpp` (parse_value)
   - Expected win: 5-15% parsing speedup, memory reduction
   - Complexity: Medium (requires refactoring)

### Serialization Optimizations

4. **Dragonbox float formatting**: Implement Dragonbox for shortest-roundtrip JSON floats

   - Where: `src/strata/bindings/python_dumps.cpp:101-119` (append_double)
   - Expected win: 2-5% dumps speedup
   - Complexity: Low (header-only library)

1. **SIMD string escaping improvements**: Optimize escape scan further

   - Where: `src/strata/util/simd_string.cpp` (escape_json_string_simd)
   - Expected win: 3-7% dumps speedup for string-heavy data
   - Complexity: Medium (SIMD optimization)

1. **Direct PyBytes write**: Write directly into PyBytes memory

   - Where: `src/strata/bindings/python_dumps.cpp:396-415` (dumps_bytes)
   - Expected win: 5-10% dumps speedup
   - Complexity: Medium (lifetime management)

### NDJSON Optimizations

7. **Chunked parsing**: Parse multiple lines in single pass

   - Where: `src/strata/json/ndjson_stream.cpp:94-111` (parse_all)
   - Expected win: 20-40% NDJSON speedup
   - Complexity: Medium (batch processing)

1. **Pipelined parsing**: Overlap newline search with JSON parsing

   - Where: `src/strata/json/ndjson_stream.cpp:31-61` (next_line)
   - Expected win: 10-20% NDJSON speedup
   - Complexity: High (pipelining)

1. **Arena allocator**: Use arena for temporary JsonValue objects

   - Where: `src/strata/json/ndjson_stream.cpp` (all methods)
   - Expected win: 5-15% NDJSON speedup, memory reduction
   - Complexity: Medium (arena implementation)

### Query Optimizations

10. **Compiled query bytecode**: Compile JSONPath to bytecode VM

    - Where: `src/strata/search/jsonpath.cpp` (evaluation)
    - Expected win: 10-30% query speedup
    - Complexity: High (VM implementation)

01. **Structural indexing**: Pre-index document structure for faster queries

    - Where: `src/strata/search/jsonpath.cpp` (query execution)
    - Expected win: 50-100% speedup for repeated queries
    - Complexity: High (indexing system)

01. **Bitset filters**: Use bitsets for filter predicate evaluation

    - Where: `src/strata/search/jsonpath.cpp` (filter evaluation)
    - Expected win: 20-50% filter speedup
    - Complexity: Medium (bitset operations)

### Memory Optimizations

13. **Custom allocators**: Arena/pool allocators for hot paths

    - Where: All parsing/serialization paths
    - Expected win: 10-20% memory reduction, 5-10% speedup
    - Complexity: Medium (allocator design)

01. **Small buffer optimization**: Use stack buffers for small strings

    - Where: `src/strata/util/output_buffer.hpp` (OutputBuffer)
    - Expected win: 5-10% speedup for small objects
    - Complexity: Low (buffer management)

### Toolchain Optimizations

15. **PGO retraining**: Retrain PGO profiles on larger datasets
    - Where: `scripts/pgo_build.sh`
    - Expected win: 2-5% additional speedup
    - Complexity: Low (script updates)

______________________________________________________________________

## 5. Next Actions (Copy/Paste Checklist)

### Immediate (P0)

- [ ] Reproduce and fix benchmark ERROR rows (`docs/benchmarks/bench_results_final.md:15`)
- [ ] Add regression test for ERROR cases (`tests/py/test_benchmark_correctness.py`)
- [ ] Modify benchmark harness to fail-fast on errors (`benchmarks/bench_main.py`)
- [ ] Profile dumps hot path to identify 11.3% gap sources (`src/strata/bindings/python_dumps.cpp`)
- [ ] Implement direct PyBytes write path for dumps (avoid final copy)
- [ ] Profile NDJSON to identify 92% gap sources (`src/strata/json/ndjson_stream.cpp`)
- [ ] Design chunked NDJSON parsing approach

### Short-term (P1)

- [ ] Implement Dragonbox or Ryu float formatting for dumps
- [ ] Optimize PyObject\* type checks in dumps (reduce branches)
- [ ] Implement chunked NDJSON parsing
- [ ] Add CI validation for PGO+LTO
- [ ] Profile parsing variance sources
- [ ] Reduce parsing non-determinism

### Medium-term (P2)

- [ ] Implement arena allocator for NDJSON
- [ ] Add 100% coverage enforcement to CI
- [ ] Consolidate benchmark documentation
- [ ] Optimize SIMD string escaping further
- [ ] Add pipelined NDJSON parsing

### Research (Future)

- [ ] Evaluate SIMD JSON classification for parsing
- [ ] Research branchless DFA for JSON parsing
- [ ] Evaluate tape model for JSON parsing
- [ ] Research compiled query bytecode for JSONPath
- [ ] Evaluate structural indexing for repeated queries

______________________________________________________________________

## Appendix: Key Files Reference

**Core Implementation:**

- `src/strata/bindings/python_dumps.cpp` - dumps serialization (11.3% gap)
- `src/strata/json/ndjson_stream.cpp` - NDJSON parsing (92% gap)
- `src/strata/json/json_parse.cpp` - JSON parsing (can be #1)
- `src/strata/search/jsonpath.cpp` - JSONPath queries (#1)

**Utilities:**

- `include/strata/util/output_buffer.hpp` - OutputBuffer for serialization
- `include/strata/util/simd_string.hpp` - SIMD string operations
- `include/strata/util/ryu_dtoa.hpp` - Float formatting (may need Dragonbox)

**Build & Testing:**

- `CMakeLists.txt` - CMake build config
- `CMakePresets.json` - Build presets (dev/bench)
- `setup.py` - Python build with test gating
- `scripts/pgo_build.sh` - PGO workflow
- `run_cpp_tests.sh` - C++ test runner

**Benchmarks:**

- `benchmarks/bench_main.py` - Main benchmark harness
- `benchmarks/bench_dumps.py` - dumps benchmarks
- `benchmarks/bench_ndjson.py` - NDJSON benchmarks
- `benchmarks/bench_search.py` - Search benchmarks

**Documentation:**

- `docs/benchmarks/dumps_results.md` - dumps benchmark results
- `docs/benchmarks/ndjson_results.md` - NDJSON benchmark results
- `docs/benchmarks/search_results.md` - Search benchmark results
- `docs/build/lto.md` - LTO documentation
- `docs/build/pgo.md` - PGO documentation
