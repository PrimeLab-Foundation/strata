# Strata — Improvement Plan (by Claude Opus 4.5)

- Date: 2026-01-30
- Commit/branch analyzed: 0dd6fec82903a7473ecc5709b020843b28cff0e7
- Environment assumptions: macOS 25.1.0 (darwin), C++20, Python ≥3.10, CMake ≥3.20, Apple Clang or GCC/Clang with AVX2/SSE4.2/NEON

______________________________________________________________________

## 1. Executive Summary

**Biggest Risks:**

- Ryu float formatting is incomplete (falls back to `snprintf`), negating expected performance gains
- JSON serializer (`json_serialize.cpp`) does not use SIMD string escaping, creating inconsistency with `python_dumps.cpp`
- No `\uXXXX` Unicode escape parsing support—potential correctness issue for compliant JSON
- NDJSON performance gap (92% behind #1) indicates fundamental algorithmic inefficiency
- Missing CI/CD configuration (no `.github/workflows/` visible) risks regressions
- `std::map` used for JSON objects—suboptimal cache locality vs. flat containers

**Biggest Wins:**

- JSONPath query engine already #1 (4-14x faster than competitors)—maintain this lead
- Memory efficiency is best-in-class—preserve this advantage
- SIMD infrastructure exists for AVX2/SSE4.2/NEON—can be better leveraged
- Test-gated build system (Rules 14+15) ensures quality—extend to CI
- Arena allocator exists but unused—opportunity for memory optimization
- Parser has fast-path optimizations—extend to more hot paths

**Critical Path to Production #1:**

1. Complete Ryu/Dragonbox implementation for float formatting
1. Integrate SIMD escaping into `json_serialize.cpp` (parity with bindings)
1. Optimize NDJSON with chunked parsing and arena allocation
1. Add CI/CD pipeline with test and benchmark gates
1. Close dumps gap with direct PyBytes write path

______________________________________________________________________

## 2. Priority Roadmap

| Priority | Area        | Item Title                                           | Expected Impact | Effort | Evidence                                                    |
| -------- | ----------- | ---------------------------------------------------- | --------------- | ------ | ----------------------------------------------------------- |
| P0       | Correctness | Add `\uXXXX` Unicode escape parsing                  | High            | M      | `src/strata/json/json_parse.cpp:128-147` (no `case 'u'`)    |
| P0       | Perf        | Complete Ryu/Dragonbox float formatting              | High            | M      | `src/strata/util/ryu_dtoa.cpp:89` uses `snprintf` fallback  |
| P0       | Perf        | Optimize NDJSON (92% gap)                            | High            | L      | `src/strata/json/ndjson_stream.cpp`, benchmark results      |
| P0       | Build       | Add CI/CD pipeline (GitHub Actions)                  | High            | M      | No `.github/workflows/` found                               |
| P1       | Perf        | Use SIMD escaping in `json_serialize.cpp`            | Med             | S      | `src/strata/json/json_serialize.cpp:13-54` is scalar        |
| P1       | Perf        | Close dumps gap (11.3%) with direct PyBytes write    | Med             | M      | `src/strata/bindings/python_dumps.cpp`                      |
| P1       | Perf        | Replace `std::map` with flat container for objects   | Med             | M      | `include/strata/json/json_core.hpp:28`                      |
| P1       | Testing     | Add fuzzing infrastructure                           | High            | M      | No fuzzing setup found                                      |
| P2       | Perf        | Integrate arena allocator into hot paths             | Med             | M      | `include/strata/util/arena_allocator.hpp` unused in parsing |
| P2       | Perf        | Add integer type distinction (avoid double for ints) | Low             | L      | All numbers stored as `double`                              |
| P2       | Testing     | Add benchmark correctness tests                      | Med             | S      | Known ERROR rows in benchmarks                              |
| P2       | Docs        | Consolidate benchmark documentation                  | Low             | S      | Multiple overlapping files in `docs/benchmarks/`            |
| P2       | Build       | Enable clang-format in pre-commit for C++            | Low             | S      | `.pre-commit-config.yaml:44-47` excludes C++                |

______________________________________________________________________

## 3. Detailed Recommendations

### 3.1 Add `\uXXXX` Unicode Escape Parsing (P0)

**Problem:**
The JSON parser does not handle `\uXXXX` Unicode escape sequences, which are required by the JSON specification (RFC 8259). This is a correctness issue.

**Evidence:**

```cpp
// src/strata/json/json_parse.cpp:128-147
switch (esc) {
case '"':  out.push_back('"'); break;
case '\\': out.push_back('\\'); break;
case '/':  out.push_back('/'); break;
case 'b':  out.push_back('\b'); break;
case 'f':  out.push_back('\f'); break;
case 'n':  out.push_back('\n'); break;
case 'r':  out.push_back('\r'); break;
case 't':  out.push_back('\t'); break;
default:
    return {Status::ParseError, JsonValue{}};  // No 'case 'u'' handling
}
```

**Proposed Change:**

1. Add `case 'u':` handler that reads 4 hex digits
1. Decode UTF-16 code point (handle surrogate pairs for emoji/extended chars)
1. Encode result as UTF-8 into output string
1. Add tests for BMP characters (`\u0041` → "A") and surrogate pairs (`\uD83D\uDE00` → 😀)

**Acceptance Criteria:**

- Parse `"\u0041"` correctly as "A"
- Parse surrogate pairs for emoji
- Reject invalid hex sequences (e.g., `\uGGGG`)
- C++ tests + Python tests for all cases

**Risks/Tradeoffs:**

- Adds complexity to fast path (mitigate by detecting `\u` upfront)
- Surrogate pair handling adds edge cases

**Suggested Owner:** `src/strata/json/json_parse.cpp`

______________________________________________________________________

### 3.2 Complete Ryu/Dragonbox Float Formatting (P0)

**Problem:**
The Ryu implementation claims to use the Ryu algorithm but falls back to `snprintf`, negating performance benefits.

**Evidence:**

```cpp
// src/strata/util/ryu_dtoa.cpp:86-89
// Use snprintf for clean, accurate formatting
// %.17g provides enough precision while avoiding excessive digits
int len = snprintf(p, 32, "%.17g", value);
```

**Proposed Change:**

1. Implement actual Ryu or Dragonbox algorithm (header-only libraries available)
1. Handle edge cases: NaN, Inf, subnormals, zero
1. Ensure shortest-roundtrip representation
1. Benchmark against current `snprintf` fallback

**Acceptance Criteria:**

- 3-10x speedup in float formatting microbenchmarks
- All existing tests pass
- Roundtrip property: `parse(format(x)) == x` for all doubles
- No precision loss

**Risks/Tradeoffs:**

- Ryu/Dragonbox implementations are complex (~1000-2000 LOC)
- Consider using existing header-only implementations (e.g., fmt library)

**Suggested Owner:** `src/strata/util/ryu_dtoa.cpp`, `include/strata/util/ryu_dtoa.hpp`

______________________________________________________________________

### 3.3 Optimize NDJSON (P0)

**Problem:**
NDJSON is 92% behind #1 (orjson), the largest performance gap in the project.

**Evidence:**

- Benchmark results show ~46ms vs orjson's ~24ms
- Current implementation parses line-by-line with function call overhead
- `parse_all` allocates new vector per call

**Proposed Change:**

1. **Chunked parsing**: Pre-scan all newlines with SIMD, parse in batches
1. **Arena allocation**: Use existing `Arena` class for temporary `JsonValue` objects
1. **Zero-copy views**: Return `string_view` for string values when possible
1. **Prefetch optimization**: Prefetch next line while parsing current

**Implementation sketch:**

```cpp
// Chunked approach
std::vector<JsonValue> parse_all_chunked() {
    // 1. Count all newlines upfront (already using SIMD)
    size_t line_count = util::count_newlines_simd(data_.data(), data_.size());
    
    // 2. Pre-allocate result vector
    std::vector<JsonValue> results;
    results.reserve(line_count);
    
    // 3. Parse in batches with arena
    Arena arena(line_count * 512);  // ~512 bytes per JSON object estimate
    
    // 4. Batch parsing with reduced function call overhead
    // ...
}
```

**Acceptance Criteria:**

- NDJSON within 30% of orjson
- Memory usage remains best-in-class
- All streaming tests pass
- No API changes required

**Risks/Tradeoffs:**

- Chunked parsing complicates error handling (which line failed?)
- Arena adds memory management complexity

**Suggested Owner:** `src/strata/json/ndjson_stream.cpp`

______________________________________________________________________

### 3.4 Add CI/CD Pipeline (P0)

**Problem:**
No CI/CD configuration exists, risking regressions and making it impossible to enforce quality gates.

**Evidence:**

- No `.github/workflows/` directory
- Pre-commit hooks exist but don't run automatically in CI
- Test-gated build only works locally

**Proposed Change:**
Create `.github/workflows/ci.yml`:

1. **Test job**: Run C++ tests (`ctest`) and Python tests (`pytest`)
1. **Lint job**: Run ruff, mypy, clang-format
1. **Build job**: Build on Linux (manylinux), macOS (arm64 + x86_64)
1. **Benchmark job** (nightly): Run benchmarks, fail on regression >10%

**Acceptance Criteria:**

- CI runs on every PR
- CI blocks merge on test failure
- Nightly benchmark job tracks performance
- Coverage report generated

**Risks/Tradeoffs:**

- CI adds time to PR workflow
- Nightly benchmark may be flaky (use statistical comparison)

**Suggested Owner:** `.github/workflows/`

______________________________________________________________________

### 3.5 Use SIMD Escaping in json_serialize.cpp (P1)

**Problem:**
The standalone serializer uses scalar string escaping while Python bindings use SIMD, creating inconsistency and slower performance for C++ users.

**Evidence:**

```cpp
// src/strata/json/json_serialize.cpp:13-54 - scalar implementation
static void escape_string(std::string_view str, std::string& out) {
    for (char c : str) {
        switch (c) { /* scalar escaping */ }
    }
}
```

vs.

```cpp
// src/strata/util/simd_string.cpp - SIMD implementation exists
void escape_json_string_simd(const char* str, size_t len, std::string& out);
```

**Proposed Change:**
Replace `escape_string` in `json_serialize.cpp` with call to `escape_json_string_simd`.

**Acceptance Criteria:**

- Serializer uses SIMD escaping
- All serialization tests pass
- Measurable speedup for string-heavy data

**Risks/Tradeoffs:**

- Minimal risk—SIMD version already tested via Python bindings

**Suggested Owner:** `src/strata/json/json_serialize.cpp`

______________________________________________________________________

### 3.6 Replace std::map with Flat Container (P1)

**Problem:**
`JsonValue::Object` uses `std::map<std::string, JsonValue>`, which has poor cache locality due to node-based allocation.

**Evidence:**

```cpp
// include/strata/json/json_core.hpp:28
using Object = std::map<std::string, JsonValue>;  // Keep std::map - better cache locality
// ^ Comment is incorrect—std::map has worse cache locality than flat containers
```

**Proposed Change:**

1. Replace with `std::vector<std::pair<std::string, JsonValue>>` for small objects
1. Or use a flat hash map (e.g., `ankerl::unordered_dense` or `absl::flat_hash_map`)
1. Maintain sorted order if JSONPath relies on it

**Acceptance Criteria:**

- Parsing speedup for object-heavy JSON
- All tests pass
- JSONPath behavior unchanged

**Risks/Tradeoffs:**

- Field lookup becomes O(n) for vector (acceptable for small objects)
- Hash map adds dependency or implementation effort

**Suggested Owner:** `include/strata/json/json_core.hpp`

______________________________________________________________________

### 3.7 Add Fuzzing Infrastructure (P1)

**Problem:**
No fuzzing setup exists, leaving the parser vulnerable to edge cases and security issues.

**Evidence:**

- No `fuzz/` or `fuzzing/` directory
- No OSS-Fuzz integration
- JSON parsers are historically vulnerable to crafted inputs

**Proposed Change:**

1. Add libFuzzer targets for `parse_json`, `iter_ndjson`, `compile_jsonpath`
1. Add corpus of interesting JSON files (from JSON Test Suite)
1. Run fuzzers in CI (time-limited) and nightly (extended)
1. Add sanitizer builds (ASan, UBSan, MSan)

**Acceptance Criteria:**

- Fuzzer runs without crashes on initial corpus
- CI runs 60-second fuzz session on each PR
- Nightly runs 8-hour fuzz session
- No ASan/UBSan errors

**Risks/Tradeoffs:**

- Fuzzing may find real bugs (good but requires fixing)
- Adds CI time

**Suggested Owner:** `fuzz/` (new directory)

______________________________________________________________________

### 3.8 Integrate Arena Allocator into Hot Paths (P2)

**Problem:**
Arena allocator exists but is not used in parsing or serialization hot paths.

**Evidence:**

```cpp
// include/strata/util/arena_allocator.hpp exists but...
// src/strata/json/json_parse.cpp does not use it
```

**Proposed Change:**

1. Use `Arena` for temporary allocations during parsing
1. Provide `parse_json_with_arena(std::string_view, Arena&)` for reuse
1. Use `ArenaStringBuilder` in serialization

**Acceptance Criteria:**

- Reduced allocation count in parsing (measurable via profiler)
- 5-15% speedup in parsing benchmarks
- Memory usage stable or improved

**Risks/Tradeoffs:**

- Arena complicates API if exposed
- Lifetime management requires careful handling

**Suggested Owner:** `src/strata/json/json_parse.cpp`, `src/strata/json/json_serialize.cpp`

______________________________________________________________________

### 3.9 Add Integer Type Distinction (P2)

**Problem:**
All numbers are stored as `double`, losing precision for integers > 2^53 and causing unnecessary floating-point operations.

**Evidence:**

```cpp
// include/strata/json/json_core.hpp:29
using Number = double; // keep it simple for now
```

**Proposed Change:**

1. Use `std::variant<int64_t, double>` for Number
1. Parse integers as int64_t when possible
1. Serialize integers without decimal point
1. Maintain backward compatibility via `get_number()` → double

**Acceptance Criteria:**

- Large integers preserved exactly
- Integer serialization without `.0` suffix
- All tests pass

**Risks/Tradeoffs:**

- Increases variant size
- Adds type-checking overhead

**Suggested Owner:** `include/strata/json/json_core.hpp`, `src/strata/json/json_parse.cpp`

______________________________________________________________________

## 4. Algorithms & Research Ideas

### Parsing Optimizations

1. **SIMD Token Classification**

   - Use SIMD to classify JSON tokens (`{`, `}`, `[`, `]`, `:`, `,`, `"`, digits) in parallel
   - Where: `src/strata/json/json_parse.cpp` (value dispatch)
   - Expected: 10-20% parsing speedup
   - Reference: simdjson's stage 1 processing

1. **Branchless State Machine**

   - Replace switch-based parsing with DFA table lookup
   - Where: All `parse_*` functions
   - Expected: 15-25% speedup due to reduced branch mispredictions

1. **Structural Tape Model**

   - Pre-allocate "tape" for JSON structure, separate from string content
   - Where: Parser output format
   - Expected: 20-40% memory reduction, faster navigation

1. **Vectorized UTF-8 Validation**

   - Validate UTF-8 using SIMD during string parsing
   - Where: `parse_string()` slow path
   - Reference: simdjson's UTF-8 validation

### Serialization Optimizations

5. **Dragonbox Float Formatting**

   - State-of-the-art shortest-roundtrip float algorithm
   - Where: `src/strata/util/ryu_dtoa.cpp`
   - Expected: 3-5x faster than snprintf

1. **SIMD Integer Formatting**

   - Use SIMD for digit conversion (Terje Mathisen technique)
   - Where: Integer serialization
   - Expected: 2-3x faster for large integers

1. **Direct PyBytes Writing**

   - Pre-allocate PyBytes, write directly into its buffer
   - Where: `src/strata/bindings/python_dumps.cpp`
   - Expected: 5-10% dumps speedup by avoiding copy

### NDJSON Optimizations

8. **Parallel NDJSON Parsing**

   - Use multiple threads to parse independent lines
   - Where: `src/strata/json/ndjson_stream.cpp`
   - Expected: Linear speedup with core count (up to I/O bound)

1. **Pipelined Processing**

   - Overlap newline search with JSON parsing using prefetch
   - Expected: 10-20% improvement by hiding memory latency

1. **Zero-Copy String Views**

   - Return `string_view` into original buffer for string values
   - Requires lifetime guarantees but avoids allocation

### Query Optimizations

11. **Compiled JSONPath Bytecode**

    - Compile JSONPath to bytecode VM for evaluation
    - Where: `src/strata/search/jsonpath.cpp`
    - Expected: 30-50% query speedup

01. **Structural Indexing**

    - Build index of document structure for repeated queries
    - Expected: 10-100x speedup for repeated queries on same document

01. **Filter Pushdown**

    - Evaluate filters during navigation, not after materialization
    - Where: `eval_filter()` in `jsonpath.cpp`
    - Expected: Significant speedup for selective filters

### Memory Optimizations

14. **Small Buffer Optimization (SBO)**

    - Store small strings inline in `JsonValue` (e.g., \<16 bytes)
    - Expected: 20-30% memory reduction for typical JSON

01. **Pooled String Interning**

    - Intern common strings (field names like "id", "name", "type")
    - Expected: Memory reduction for repetitive JSON

______________________________________________________________________

## 5. Next Actions (Copy/Paste Checklist)

### Immediate (P0 - Do First)

- [ ] Add `\uXXXX` Unicode escape parsing in `json_parse.cpp`
- [ ] Add tests for Unicode escapes (BMP, surrogate pairs, invalid sequences)
- [ ] Evaluate Dragonbox header-only library for float formatting
- [ ] Replace `snprintf` fallback with actual Dragonbox/Ryu in `ryu_dtoa.cpp`
- [ ] Create `.github/workflows/ci.yml` with test + lint jobs
- [ ] Profile NDJSON to identify exact bottlenecks
- [ ] Design chunked NDJSON parsing approach with arena allocation

### Short-term (P1 - Do Soon)

- [ ] Replace `escape_string` in `json_serialize.cpp` with `escape_json_string_simd`
- [ ] Benchmark `std::map` vs flat container for JSON objects
- [ ] Implement direct PyBytes write path for `dumps_bytes`
- [ ] Add libFuzzer target for `parse_json`
- [ ] Create initial fuzzing corpus from JSON Test Suite
- [ ] Add sanitizer builds to CI (ASan, UBSan)

### Medium-term (P2 - Do Later)

- [ ] Integrate `Arena` into `parse_json` hot path
- [ ] Add `parse_json_with_arena` API for advanced users
- [ ] Evaluate `std::variant<int64_t, double>` for Number type
- [ ] Add benchmark correctness test that fails on ERROR rows
- [ ] Consolidate `docs/benchmarks/` into single canonical file
- [ ] Enable clang-format in pre-commit for C++ files
- [ ] Add coverage reporting to CI

### Research (Future Investigation)

- [ ] Prototype SIMD token classification (simdjson stage 1 approach)
- [ ] Research branchless DFA for JSON parsing
- [ ] Evaluate structural tape model for memory efficiency
- [ ] Prototype compiled JSONPath bytecode VM
- [ ] Investigate parallel NDJSON parsing safety (thread boundaries)
- [ ] Research string interning for field names

______________________________________________________________________

## Appendix: Key Files Reference

**Core Implementation:**

- `src/strata/json/json_parse.cpp` - JSON parser (missing `\uXXXX`)
- `src/strata/json/json_serialize.cpp` - JSON serializer (scalar escaping)
- `src/strata/json/ndjson_stream.cpp` - NDJSON parser (92% gap)
- `src/strata/search/jsonpath.cpp` - JSONPath engine (#1 performer)
- `src/strata/bindings/python_dumps.cpp` - Python serialization (11.3% gap)

**Utilities:**

- `include/strata/util/simd_string.hpp` - SIMD string operations (well-implemented)
- `include/strata/util/ryu_dtoa.hpp` - Float formatting (incomplete)
- `include/strata/util/arena_allocator.hpp` - Arena allocator (unused)
- `include/strata/util/output_buffer.hpp` - Output buffer (used in bindings)
- `include/strata/util/fast_parse.hpp` - Fast number parsing

**Data Model:**

- `include/strata/json/json_core.hpp` - `JsonValue` definition (uses `std::map`)
- `include/strata/json/json_cursor.hpp` - Navigation cursor
- `include/strata/json/json_document.hpp` - Owning document handle

**Build & Testing:**

- `CMakeLists.txt` - CMake config with LTO support
- `setup.py` - Python build with test gates
- `tests/cpp/` - C++ unit tests (8 test executables)
- `tests/py/` - Python tests (comprehensive coverage)
- `.pre-commit-config.yaml` - Pre-commit hooks (excludes C++)

**Benchmarks:**

- `benchmarks/bench_main.py` - Main benchmark orchestrator
- `benchmarks/bench_dumps.py` - Serialization benchmarks
- `benchmarks/bench_ndjson.py` - NDJSON benchmarks
- `benchmarks/bench_search.py` - Query benchmarks

______________________________________________________________________

## Appendix: Architecture Assessment

### Strengths

1. **Clean module boundaries**: Parsing, serialization, navigation, and queries are well-separated
1. **SIMD foundation**: Infrastructure exists for AVX2/SSE4.2/NEON with proper fallbacks
1. **Test-gated builds**: Rules 14+15 enforce quality at build time
1. **Python C API**: Direct use (vs. pybind11) enables maximum performance
1. **Error handling**: `Result<T>` type for hot paths, exceptions at boundaries

### Weaknesses

1. **Incomplete implementations**: Ryu uses snprintf, no `\uXXXX` parsing
1. **Inconsistent optimization**: SIMD escaping only in bindings, not core
1. **Suboptimal data structures**: `std::map` for objects
1. **No CI**: Quality gates only work locally
1. **No fuzzing**: Parser not stress-tested

### Opportunities

1. **Arena integration**: Allocator exists but unused
1. **Chunked NDJSON**: SIMD newline counting exists, not leveraged
1. **Compiled queries**: JSONPath already fast, could be faster with bytecode
1. **Parallel parsing**: Independent lines can be parsed concurrently

### Threats

1. **Competitors improving**: orjson/msgspec actively developed
1. **Correctness issues**: Missing Unicode escapes could cause real bugs
1. **Regression risk**: No CI to catch performance/correctness regressions

______________________________________________________________________

*Generated by Claude Opus 4.5 on 2026-01-30*
