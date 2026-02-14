# Strata Performance Improvement Plan

> Generated: 2026-02-14
> Goal: Achieve #1 ranking across all benchmark categories (Rule 13)
> Baseline commit: 9db40c6 | Platform: macOS arm64 | Python 3.14.2

## Current Performance Gap Analysis

### loads (JSON) — 43.85 MB `users.json`
| Library | Median (ms) | Gap vs orjson |
|---------|-------------|---------------|
| orjson | 192.50 | baseline |
| msgspec | 199.65 | 1.04x |
| strata | 319.50 | **1.66x slower** |

### loads (NDJSON) — 43.85 MB `users.ndjson`
| Library | Median (ms) | Gap vs orjson |
|---------|-------------|---------------|
| orjson | 229.30 | baseline |
| msgspec | 238.93 | 1.04x |
| strata | 240.35 | **1.05x slower** (nearly competitive) |

### loads (small JSON) — 0.96 MB `users.json`
| Library | Median (ms) | Gap vs orjson |
|---------|-------------|---------------|
| orjson | 4.02 | baseline |
| strata | 11.49 | **2.86x slower** |

### search — cursor mode on JSON
| Query | strata | jmespath | Gap |
|-------|--------|----------|-----|
| Extract all user IDs | 5.78ms | 2.07ms | 2.8x slower |
| Double wildcard | 995ms | 241ms | 4.1x slower |
| Recursive descent | 1081ms | 4163ms | **3.9x faster** |

### Key Strengths
- Best RSS memory usage (593 MB vs 724-846 MB on large JSON)
- NDJSON nearly matches orjson on large data
- Recursive descent search dominates jsonpath-ng by 3.9x
- Cursor reuse provides 2.3-3.2x speedup

### Critical Gaps to Close
1. **JSON loads**: 1.66x behind orjson (large), 2.86x behind (small)
2. **Search throughput**: 2.8-4.1x behind jmespath on traversal queries
3. **NDJSON search**: 17x behind jmespath on ID extraction

---

## Improvement Prompts — Ordered by Expected Impact

### Phase 1: Python Object Materialization (Target: -40% loads time)

#### Prompt 1 — Isolate and Profile Python Object Creation Overhead
```
Profile strata.loads on benchmarks/data/generated/large/users.json using
Python's cProfile and a C++ profiler (Instruments/perf). Produce a flamegraph
identifying the top 10 hotspots. Create a micro-benchmark in benchmarks/ that
isolates Python object creation (PyDict_New, PyList_New, PyUnicode_FromStringAndSize,
PyLong_FromLongLong, PyFloat_FromDouble) by constructing the same object graph
from pre-parsed C++ data. Compare this "materialization only" time against
orjson's total loads time. Record results in docs/benchmarks/progress_log.md.

Expected deliverables:
- flamegraph image under docs/benchmarks/
- micro-benchmark script under benchmarks/
- hotspot analysis with percentage breakdown
- baseline entry in progress_log.md
```

#### Prompt 2 — Eliminate Redundant Two-Pass Parsing for Size Hints
```
In python_loads.cpp, when use_exact_size_hints=true AND input >= 1MB, the code
does a full SizeHintCollector pass THEN a full SAX parse — two complete passes
over the input. This doubles parsing time for large inputs.

Implement single-pass size collection by integrating container size tracking
directly into the SAX parse handler (DomBuilderHandler / PythonObjectBuilder).
Track element counts per container during the primary parse and use them to
resize lists/dicts inline as they grow.

Approach:
1. Remove the separate SizeHintCollector pre-pass (lines 1376-1393 in python_loads.cpp)
2. Add size tracking to the SAX handler's on_start_array/on_start_object callbacks
3. Use geometric growth (2x) for lists/dicts instead of exact presizing
4. Benchmark before/after on small/medium/large datasets
5. Ensure no regression on small inputs where hints are not used

Follow Rules 6, 8, 11, 17. Run all tests. Record benchmark delta in progress_log.md.
```

#### Prompt 3 — Optimize Key Caching and String Interning Strategy
```
In python_object_builder.h, every dictionary key goes through:
1. KeyCache fast_common_key_lookup (20 hardcoded keys, multi-branch switch)
2. Robin hood hash table lookup (incomplete implementation per line 325-331 comment)
3. PyUnicode_InternInPlace on every key (expensive for one-off keys)

This creates ~15-20% overhead on key-heavy JSON. Optimize:

A) Fix robin hood hash: either implement proper displaced-entry relocation or
   replace with simpler open-addressing with linear probing (current approach has
   worst of both worlds — tracking distances without actually using them)

B) Conditional interning: only intern keys with len <= 16 bytes. Skip interning
   for long keys that are unlikely to repeat. Add a configurable threshold via
   KeyCache::set_max_interned_key_length().

C) Replace the 20-key switch/memcmp in fast_common_key_lookup with a 256-byte
   lookup table indexed by (len << 4 | first_char) for O(1) dispatch.

D) Benchmark loads on small/medium/large datasets. Target: 10-15% improvement
   on key-heavy JSON.

Follow Rules 6, 7, 8, 11, 17. Run all C++ and Python tests.
```

#### Prompt 4 — Optimize GIL and GC Interaction During Large Parses
```
In python_loads.cpp (lines 1408-1422), GC is paused heuristically for large inputs.
The current heuristic uses division and comma-counting which adds overhead.

Optimize the GC pause strategy:
1. Replace the sample-and-extrapolate approach with a simple threshold:
   if input_size >= 64KB, pause GC (eliminates division and comma counting)
2. Profile the impact of gc.disable()/gc.enable() around the entire parse
3. Consider releasing the GIL during C++ parsing and re-acquiring only for
   Python object creation (requires careful refactoring of the SAX handler)
4. Benchmark memory usage impact — GC pause should not cause RSS blow-up

Measure: loads throughput and RSS on all three dataset sizes.
Follow Rules 6, 8, 9, 11, 17.
```

### Phase 2: C++ Parser Core (Target: -25% parse time)

#### Prompt 5 — Template-Based Parser to Eliminate Virtual Call Overhead
```
The SAX parser in json_parse.cpp uses virtual function calls for every value
event (on_null, on_bool, on_int, on_string, etc.). For a 43MB JSON file with
millions of values, this creates millions of indirect calls that the CPU branch
predictor cannot optimize.

Implement a template-based parser variant:
1. Create `template<typename Handler> class JsonParserT` alongside the existing
   virtual-dispatch parser
2. The Python binding path should use the template version directly with
   PythonObjectBuilder as the handler type
3. All handler methods become direct calls that the compiler can inline
4. Keep the virtual-dispatch version for the public C++ API (backward compat)
5. Benchmark: expect 8-15% improvement on large JSON from eliminated vtable lookups

Implementation in src/strata/json/json_parse.cpp and include/strata/json/.
Follow Rules 1, 6, 7, 8, 11, 14, 17. Add C++ tests for the template parser.
```

#### Prompt 6 — Unified Number Parser (Single-Pass)
```
In json_parse.cpp (lines 754-796), number parsing tries three sequential
strategies: parse_int_fast → parse_uint_fast → parse_double_fast. Each attempt
re-scans the input and does its own bounds checking.

Implement a unified single-pass number parser:
1. Scan digits once, tracking: sign, digit count, decimal point position,
   exponent presence
2. Based on what was encountered, branch to the correct type conversion at the end
3. Use the existing SWAR techniques from fast_parse.cpp for digit extraction
4. Replace the lookup in fast_parse.cpp line 194 (std::pow) with a precomputed
   powers-of-10 table (range: 10^-324 to 10^308)
5. Replace fractional parsing loop (fast_parse.cpp lines 119-123) with a
   precomputed 10^-N lookup table

Target: 15-25% improvement for mixed-type number-heavy JSON.
Follow Rules 1, 6, 8, 11, 12, 17. Add C++ unit tests for edge cases.
```

#### Prompt 7 — Optimize String Parsing: Combined Escape Scanning
```
In json_parse.cpp (lines 798-881), string parsing does a SIMD scan to find
escapes, then a character-by-character validation pass. In simd_escape.cpp
(lines 387-438), escape_or_copy_string_simd does a redundant double-scan:
first find_next_escape_simd, then the loop calls it again.

Optimize:
1. Create a combined SIMD function that finds escapes AND validates them in one
   pass (avoid the re-scan in simd_escape.cpp)
2. For strings without escapes (>90% of JSON strings): single SIMD scan to
   find closing quote, then memcpy the entire string — no per-char processing
3. Replace snprintf in simd_escape.cpp line 320 (escape_char) with a precomputed
   lookup table for the 32 control characters (0x00-0x1F)
4. In parse_string (json_parse.cpp), use a 128-byte lookup table for escape
   validation instead of the 7-way switch statement

Target: 20-30% improvement for string-heavy JSON.
Follow Rules 1, 6, 8, 11, 12, 17.
```

#### Prompt 8 — SIMD Whitespace + Structural Character Fusion
```
In json_parse.cpp, skip_ws() is called 7-8 times per small object parse (lines
566-633), and each call invokes skip_whitespace_simd(). Additionally,
next_structural_char() (line 241-261) calls skip_whitespace_simd AND checks the
structural tape — double work.

Optimize:
1. Create a fused skip_ws_and_peek() function that skips whitespace and returns
   the next non-whitespace character in one operation
2. Eliminate duplicate whitespace skipping between skip_ws() and
   next_structural_char()
3. In simd_structural.cpp, combine the whitespace mask generation (lines 89-96)
   into a single comparison: merge the two mask checks into one OR operation
4. For the flat object parse path (lines 566-633), inline the whitespace skip
   entirely to avoid function call overhead

Target: 5-10% improvement on small objects (high function-call-to-work ratio).
Follow Rules 1, 6, 8, 11, 17.
```

### Phase 3: Build & Compiler Optimization (Target: -10% across the board)

#### Prompt 9 — Compiler Flags and LTO Optimization
```
Audit and optimize build flags in setup.py and CMakeLists.txt:

1. Add to setup.py Unix flags (line 236):
   - -ffast-math (enables FMA, faster float operations — critical for number parsing)
   - -funroll-loops (benefits SIMD-heavy inner loops)
   - -ffunction-sections -fdata-sections (enable linker dead code elimination)
   - -fvisibility=hidden (reduce shared library overhead)

2. For clang (detected at line 209), use -flto=thin instead of -flto for
   faster link times with equivalent optimization

3. In CMakeLists.txt, set default CMAKE_BUILD_TYPE to Release (currently unset,
   risks debug builds)

4. Verify -march=native is active on the benchmark machine (check with
   `python -c "import sysconfig; print(sysconfig.get_config_var('CFLAGS'))"`)

5. Run PGO build workflow (make pgo-release) and benchmark against non-PGO:
   - PGO_MODE=generate: profile with bench_loads.py large dataset
   - PGO_MODE=use: rebuild with profile data
   - Compare loads/dumps/search on all sizes

IMPORTANT: Test -ffast-math carefully — it can change float precision. Run all
float-related tests before/after. If any test fails, use -fno-math-errno
-fassociative-math instead (subset of -ffast-math without the dangerous parts).

Target: 5-15% improvement with flags alone, additional 5-10% from PGO.
Follow Rules 5, 6, 8, 14, 15, 17.
```

### Phase 4: Serialization (dumps) Optimization

#### Prompt 10 — Optimize dumps Buffer Management and Type Dispatch
```
In python_dumps.cpp, several inefficiencies exist:

1. Buffer estimation (lines 654-667) uses fixed heuristics
   (kDictEntryEstimate=96, kListEntryEstimate=64) then applies 1.5x growth.
   Replace with an exponential moving average seeded from g_last_dumps_size:
   estimate = 0.7 * g_last_dumps_size + 0.3 * heuristic_estimate

2. Type checking cascade (lines 849-892) has two paths based on ints_first flag,
   creating code cache pressure. Reorder by actual frequency:
   PyUnicode_Type (60%+) → PyLong_Type (25%) → PyFloat_Type (10%) → others

3. Arena reset (lines 1081-1202) is called multiple times per serialization.
   Reset only once at entry.

4. Integer formatting (lines 250-318) uses sequential div/mod. Add a
   combined divmod operation using compiler intrinsics or lookup tables.

5. Stack buffer size (line 669, kStackBufferCap=4096): reduce to 1024 for
   better cache locality on small outputs (covers 90% of cases).

Benchmark dumps on all three dataset sizes.
Follow Rules 1, 6, 8, 11, 17.
```

### Phase 5: Search/Query Performance

#### Prompt 11 — Optimize Search Traversal for Wildcard Queries
```
Search benchmarks show strata is 4.1x slower than jmespath on double-wildcard
queries (995ms vs 241ms on large JSON). Note: jmespath returns only 4000 results
while strata returns 789,913 — strata finds more correct results but is slower.

Investigate and optimize:
1. Profile search_eval.cpp to identify where time is spent during wildcard
   traversal. The eval loop likely creates excessive intermediate Python objects.
2. For queries like $.users[*].orders[*].items[*].price, implement a "compiled
   path" optimization that fuses consecutive wildcards into a single traversal
   rather than nested loops
3. Add batch result collection: instead of calling Python list append per result,
   collect results in C++ vector<PyObject*> then bulk-create the Python list
4. For cursor-mode search, check if the cursor is re-parsing per query or
   properly reusing the parsed structure
5. Optimize the NDJSON search path (strata is 17x behind jmespath on ID extraction
   for NDJSON): the bottleneck may be per-line parsing overhead

Target: 2-3x improvement on wildcard queries, bringing within 1.5x of jmespath.
Follow Rules 1, 6, 7, 8, 11, 12, 17.
```

#### Prompt 12 — Search Compilation and Path Optimization
```
For simple path queries like $.users[*].id, strata takes 5.78ms vs jmespath's
2.07ms (2.8x slower). This is a simple traversal that should be extremely fast
with cursor mode.

Optimize:
1. In search_compile.cpp, add a fast-path detection for "simple path" queries
   (no filters, no recursion, just child/wildcard steps). Generate an optimized
   linear scan instead of the general evaluation engine.
2. In search_eval.cpp, for wildcard-then-child patterns (e.g., [*].id),
   implement a specialized evaluator that iterates array elements and extracts
   the named field directly without general path matching overhead
3. Add compiled path caching (memoize compilation results for repeated queries)
4. Profile the overhead of cursor creation vs cursor traversal to identify if
   cursor setup is dominating small query times

Target: Match jmespath on simple path queries (< 3ms for 4000 results).
Follow Rules 1, 6, 7, 8, 11, 12, 17.
```

### Phase 6: Advanced / Research-Grade Optimizations

#### Prompt 13 — Parallel JSON Parsing for Large Inputs
```
Rule 12 requires proposing research-grade approaches. For large JSON arrays
(the users.json benchmark is a top-level object with a "users" array of 4000
elements), implement parallel parsing:

Approach A — Structural tape partitioning:
1. Use the structural tape to find top-level array element boundaries
2. Partition elements across N threads (N = hardware concurrency)
3. Each thread parses its chunk independently into a local Python list
4. Merge results into the final Python list (requires GIL coordination)

Approach B — Speculative parsing:
1. Split input into equal byte-size chunks
2. Each chunk starts parsing from its boundary, seeking the first valid
   JSON value start
3. Chunks that straddle values are handled by the primary thread

Approach C — Pipeline parallelism:
1. Thread 1: tokenize/validate (SIMD-heavy, no Python dependency)
2. Thread 2: build C++ parse tree from tokens
3. Thread 3: materialize Python objects from parse tree

Implement as isolated experiments under experiments/parallel_json/.
Each experiment must produce: hypothesis, prototype, microbenchmark, go/no-go.
Target: 2-3x speedup on 43MB+ inputs with 4+ cores.
Follow Rules 1, 9, 12, 17.
```

#### Prompt 14 — SIMD UTF-8 Validation Optimization (ARM NEON Focus)
```
In simd_utf8.cpp, the NEON implementation accumulates results in temporary
stack arrays then loops through them scalar-ly (lines 444-472 pattern in
simd_structural.cpp applies to UTF-8 too). The AVX2 path classifies all 32
bytes into a type_buf array then does scalar checking (lines 151-196).

Optimize:
1. NEON: replace stack array accumulation with horizontal NEON reductions
   (vmaxvq_u8, vminvq_u8) for direct extraction
2. AVX2: use parallel SIMD vector comparisons for byte type classification
   instead of per-byte scalar type checks
3. For ASCII-only strings (>95% of typical JSON values), add a fast exit:
   check if all bytes < 0x80 in one SIMD comparison, skip UTF-8 validation
4. Consider deferring UTF-8 validation entirely for trusted inputs
   (add a skip_utf8_validation option)

Target: 10-15% improvement on UTF-8 heavy workloads; near-zero cost for ASCII.
Follow Rules 1, 6, 8, 12, 17.
```

#### Prompt 15 — Memory Arena Optimization for Python Object Building
```
Rule 9 requires reasoning about allocations for hot-path changes. The current
PythonObjectBuilder creates objects using individual Python allocator calls
(PyDict_New, PyList_New, PyUnicode_FromStringAndSize). Each call goes through
Python's memory allocator (pymalloc / system malloc).

Research approach:
1. Profile allocation patterns: count and categorize every Python allocator
   call during a large JSON parse. Record in docs/benchmarks/.
2. Investigate bulk string creation: for JSON with many short strings (<= 64
   bytes), pre-allocate a string pool and create PyUnicode objects from it
3. Investigate dict presizing accuracy: measure how often the size hints are
   exact vs over/under-estimated. Tune the growth strategy based on data.
4. Consider a "compact mode" for homogeneous arrays (all-int, all-float,
   all-string) that uses numpy-style contiguous allocation

Prototype under experiments/arena_builder/.
Follow Rules 1, 9, 12, 17.
```

#### Prompt 16 — Branchless JSON Value Type Dispatch
```
In json_parse.cpp line 549, parse_primitive uses if/else chains to dispatch
on the first character: digit/minus → number, quote → string, t → true,
f → false, n → null. This creates 5+ branch mispredictions per value on
mixed-type JSON.

Implement branchless dispatch:
1. Create a 256-entry function pointer table indexed by first byte:
   dispatch_table['"'] = &parse_string
   dispatch_table['0'..'9'] = &parse_number
   dispatch_table['-'] = &parse_number
   dispatch_table['t'] = &parse_true
   dispatch_table['f'] = &parse_false
   dispatch_table['n'] = &parse_null
   dispatch_table['{'] = &parse_object
   dispatch_table['['] = &parse_array
   All other entries → &parse_error

2. Replace the if/else chain with: dispatch_table[peek()]()
3. This eliminates branch misprediction for mixed-type JSON entirely
4. Measure impact on both homogeneous (all-int array) and heterogeneous
   (mixed object) workloads

Target: 3-8% improvement on heterogeneous JSON.
Follow Rules 1, 6, 8, 12, 17.
```

### Phase 7: Integration and Validation

#### Prompt 17 — Full Benchmark Suite with Regression Tracking
```
After implementing any subset of the above optimizations, run the complete
benchmark suite and produce a comprehensive comparison:

1. Generate baseline: save current results to docs/benchmarks/baseline_pre_opt.md
2. Implement changes (one at a time per Rule 17)
3. After each change:
   a. Run: make test (all C++ + Python tests must pass)
   b. Run: python benchmarks/bench_main.py --size small --repeat 5 --warmup 2
   c. Run: python benchmarks/bench_main.py --size medium --repeat 5 --warmup 2
   d. Run: python benchmarks/bench_main.py --size large --repeat 5 --warmup 2
   e. Record: date, commit, environment, key metrics (min/median/p95 + RSS)
   f. Compare: if median OR p95 worsens > 2%, or memory > 5%, REVERT
4. Update docs/benchmarks/progress_log.md with each entry
5. Final comparison table: strata vs orjson vs msgspec vs jmespath

Target state after all phases:
- loads (JSON): within 1.1x of orjson (currently 1.66x)
- loads (NDJSON): match or beat orjson (currently 1.05x — nearly there)
- search (simple paths): within 1.2x of jmespath
- search (recursive): maintain 3.9x lead over jsonpath-ng
- memory: maintain best-in-class RSS

Follow ALL rules, especially 6, 8, 13, 14, 17.
```

---

## Priority Matrix

| # | Prompt | Expected Impact | Effort | Priority |
|---|--------|-----------------|--------|----------|
| 1 | Profile & isolate hotspots | Foundation | Low | **P0** |
| 2 | Eliminate two-pass parsing | -15-20% loads | Medium | **P0** |
| 3 | Key caching & interning | -10-15% loads | Medium | **P0** |
| 5 | Template parser (no vtable) | -8-15% loads | High | **P1** |
| 7 | String escape optimization | -15-25% strings | High | **P1** |
| 6 | Unified number parser | -15-25% numbers | High | **P1** |
| 9 | Compiler flags + PGO | -10-15% all | Low | **P1** |
| 4 | GIL/GC optimization | -5-10% loads | Low | **P2** |
| 8 | Whitespace fusion | -5-10% small | Low | **P2** |
| 10 | dumps optimization | -10% dumps | Medium | **P2** |
| 12 | Search path optimization | -50% simple search | Medium | **P2** |
| 11 | Search wildcard optimization | -50% wildcards | High | **P2** |
| 16 | Branchless dispatch | -3-8% mixed | Low | **P3** |
| 14 | SIMD UTF-8 optimization | -10-15% UTF-8 | Medium | **P3** |
| 13 | Parallel JSON parsing | -50-66% large | Very High | **P3** |
| 15 | Arena memory optimization | -5-10% alloc | High | **P3** |
| 17 | Full benchmark validation | Validation | Low | **Every phase** |

## Execution Order

**Week 1**: Prompts 1 → 9 → 2 → 3 (profile first, then quick wins)
**Week 2**: Prompts 5 → 6 → 7 (core parser improvements)
**Week 3**: Prompts 8 → 4 → 10 → 16 (polish and secondary paths)
**Week 4**: Prompts 11 → 12 (search optimization)
**Week 5**: Prompts 13 → 14 → 15 (research-grade experiments)
**Continuous**: Prompt 17 (after every change)

---

### Phase 0: Benchmark Infrastructure Unification (Execute Before All Other Prompts)

#### Prompt 18 — Unify, Generalize, and Standardize the Benchmark Suite
```
The benchmark infrastructure has grown organically into 7+ separate entry points
with overlapping functionality, inconsistent CLI interfaces, duplicated result
types, and a Makefile that chains 6-7 sequential Python invocations per size.
This makes benchmarks fragile, hard to maintain, and produces fragmented reports.

GOAL: Exactly three Makefile benchmark targets — `make bench-small`,
`make bench-medium`, `make bench-large` — each invoking a single unified Python
entry point that produces ONE comprehensive Markdown report per size.

No other benchmark interfaces should exist in the Makefile (remove bench-ndjson,
bench-json-parallel, bench-unified, bench-unified-quick, bench-unified-comprehensive,
bench-unified-save, bench-all). Keep bench-data as internal dependency only.

=== CURRENT PROBLEMS ===

1. FRAGMENTED ENTRY POINTS: Each `make bench-{size}` runs 7 separate Python
   commands (bench_main, bench_loads JSON, bench_loads NDJSON, bench_dumps,
   bench_ndjson, bench_search JSON, bench_search NDJSON). Each opens/reads the
   data file independently, each has its own warmup, each writes/appends to the
   same Markdown file with inconsistent section headers. The bench_unified.py
   (1217 lines) exists as a parallel universe that duplicates most of this logic
   with its own randomized data generator and different reporting.

2. DUPLICATED RESULT TYPES: There are 6 separate result dataclasses across files:
   - BenchResult (bench_main.py)
   - LoadsResult (bench_loads.py)
   - DumpsResult (bench_dumps.py)
   - NdjsonResult (bench_ndjson.py)
   - QueryBenchResult (bench_search.py)
   - CursorReuseResult (bench_search.py)
   Plus FeatureReport/BenchmarkReport in bench_unified.py.
   These should be unified into a single result model.

3. INCONSISTENT METRICS: bench_main reports min/median/p95/RSS but no throughput.
   bench_loads adds throughput (MB/s). bench_dumps adds output_size. bench_search
   adds result_count and throughput. bench_ndjson adds lines_parsed. The final
   Markdown report is an incoherent patchwork of append-mode sections with
   different column schemas.

4. INCONSISTENT REPEAT/WARMUP DEFAULTS:
   - bench_main: repeat=3, warmup=1
   - bench_loads: repeat=10, warmup=2
   - bench_dumps: repeat=50, warmup=3
   - bench_ndjson: repeat=30, warmup=2
   - bench_search: repeat=5, warmup=1
   The Makefile overrides most of these (repeat=3/warmup=1 for loads, repeat=2/
   warmup=1 for search) but bench_dumps and bench_ndjson use different values
   when called directly vs from Make.

5. MISSING COVERAGE: bench_dumps output is NOT captured to the Markdown report
   (no --output flag passed in Makefile). bench_ndjson output is also console-
   only from Make. So the "comprehensive" bench-{size} targets actually miss
   dumps and standalone NDJSON results in the report.

6. REDUNDANT NDJSON BENCHMARKING: bench_main already benchmarks NDJSON parsing
   (lines 121-158). bench_loads ALSO benchmarks NDJSON parsing (lines 74-134).
   bench_ndjson does it AGAIN (lines 59-97) with different runner variants
   (auto/sequential/parallel). The Makefile runs ALL THREE for each size.
   Same data, same operation, three times.

7. PARALLEL BENCH_UNIFIED: bench_unified.py (1217 lines) is an entirely separate
   system with its own data generator, runner logic, and reporters (markdown,
   JSON, CSV) that doesn't share code with the main bench scripts. Its Makefile
   targets (bench-unified, bench-unified-quick, bench-unified-comprehensive,
   bench-unified-save) are disconnected from bench-small/medium/large.

=== TARGET ARCHITECTURE ===

A single orchestrator: benchmarks/bench_suite.py (new file, replaces all others)

The orchestrator runs ALL benchmark categories in one process, in order:
  1. loads (JSON) — strata vs orjson, msgspec, ujson, stdlib
  2. loads (NDJSON) — same libraries
  3. dumps (str mode) — strata vs orjson, msgspec, ujson, stdlib
  4. dumps (bytes mode) — same libraries
  5. search (JSON, cursor mode) — strata vs jmespath, jsonpath-ng
     8 canonical queries from QUERIES dict in current bench_search.py
  6. search (NDJSON, cursor mode) — strata vs jmespath, jsonpath-ng
     9 canonical queries from NDJSON_QUERIES dict
  7. cursor reuse vs reparse (JSON + NDJSON)

One unified result model:

    @dataclass
    class BenchmarkResult:
        category: str          # "loads", "dumps", "search", "cursor_reuse"
        library: str           # "strata", "orjson", "jmespath", etc.
        dataset: str           # "users.json", "users.ndjson"
        variant: str           # "json", "ndjson", "str", "bytes", query name, etc.
        times_ms: list[float]
        result_count: int = 0
        output_size: int = 0
        rss_mb: float = 0.0
        throughput_mbps: float = 0.0
        error: str = ""

        # Computed properties: min_ms, median_ms, p95_ms (same as current harness)

One output file per size: docs/benchmarks/bench_results_{size}.md

The Markdown report structure (single file, NOT append-mode assembly):

    # Strata Benchmark Results — {SIZE}
    Generated: {timestamp}
    ## Environment
    - Commit, OS, CPU, Python, Compiler
    ## Configuration
    - Dataset paths, sizes, record counts
    - Repeat: {N}, Warmup: {N}
    ## Parsing Benchmarks (loads)
    ### JSON — {filename} ({size} MB)
    | Library | Min (ms) | Median (ms) | P95 (ms) | MB/s | RSS (MB) |
    ### NDJSON — {filename} ({size} MB, {lines} lines)
    | Library | Min (ms) | Median (ms) | P95 (ms) | MB/s | RSS (MB) |
    ## Serialization Benchmarks (dumps)
    ### dumps (str) — {filename} ({size} MB)
    | Library | Min (ms) | Median (ms) | P95 (ms) | MB/s | Output (bytes) | RSS (MB) |
    ### dumps (bytes) — {filename} ({size} MB)
    | Library | Min (ms) | Median (ms) | P95 (ms) | MB/s | Output (bytes) | RSS (MB) |
    ## Search Benchmarks (query)
    ### JSON — {filename} ({size} MB, {records} records, cursor mode)
    | Query | Library | Min (ms) | Median (ms) | P95 (ms) | MB/s | Results | RSS (MB) |
    ### NDJSON — {filename} ({size} MB, {records} records, ndjson_cursor mode)
    | Query | Library | Min (ms) | Median (ms) | P95 (ms) | MB/s | Results | RSS (MB) |
    ## Cursor Reuse
    ### JSON
    | Mode | Min (ms) | Median (ms) | P95 (ms) | MB/s | RSS (MB) |
    - Speedup: {N}x
    ### NDJSON
    | Mode | Min (ms) | Median (ms) | P95 (ms) | MB/s | RSS (MB) |
    - Speedup: {N}x
    ## Summary
    - Strata rank per category
    - Gap vs #1 per category
    - Overall assessment (Rule 13 compliance)

=== IMPLEMENTATION STEPS ===

Step 1 — Create benchmarks/bench_suite.py:
  - Import and reuse the EXISTING library runner functions from the current
    bench_* modules where possible (the _get_loads_runners, _get_dumps_runners,
    _get_ndjson_runners, QUERIES/NDJSON_QUERIES dictionaries, and
    _run_cursor_reuse_benchmarks are well-written — extract and reuse them).
  - Import harness.py's run_single_benchmark and TimingResult as-is (it's clean).
  - Import the environment collection from bench_main.py's _collect_environment.
  - Create one BenchmarkResult dataclass (above) used across all categories.
  - Create a BenchSuite class with methods:
      run_loads(data_json, data_ndjson)
      run_dumps(data_json)
      run_search(data_json, data_ndjson)
      run_cursor_reuse(data_json, data_ndjson)
      run_all(data_json, data_ndjson)
  - Output generation: generate_markdown_report() writes the single-file report.
  - CLI: --json-data PATH --ndjson-data PATH --repeat N --warmup N --output PATH
  - Also generate JSON output for regression_check.py compatibility:
    --json-output PATH (machine-readable results for CI).

Step 2 — Standardize repeat/warmup per category (consistent defaults):
  - loads: repeat=5, warmup=2
  - dumps: repeat=5, warmup=2
  - search: repeat=3, warmup=1
  - cursor_reuse: repeat=3, warmup=1
  These are the orchestrator defaults; the Makefile should NOT override them
  (remove --repeat/--warmup from Make targets). The orchestrator owns the
  methodology. If a user wants different counts, they call bench_suite.py
  directly.

Step 3 — Update Makefile:
  Replace the entire benchmarks section (lines 139-284) with:

    # ============================================================================
    # Benchmarks (Rule 8, 13, 16)
    # ============================================================================
    # Three sizes: small (~1 MB), medium (~6 MB), large (~44 MB)
    # Each target: generates data if missing, runs full suite, writes report.
    # Output: docs/benchmarks/bench_results_{size}.md
    # ============================================================================

    BENCH_GEN := benchmarks/data/generated
    BENCH_SUITE := PYTHONPATH=. $(VENV)/bin/$(PYTHON) -m benchmarks.bench_suite

    # Data generation targets (internal, not user-facing)
    $(BENCH_GEN)/small/users.json $(BENCH_GEN)/small/users.ndjson:
    	@mkdir -p $(BENCH_GEN)/small
    	PYTHONPATH=. $(VENV)/bin/$(PYTHON) -m benchmarks.data.generate_bench_data \
    		--out-dir $(BENCH_GEN)/small --num-users 1000 --max-orders 10 --max-items 5

    $(BENCH_GEN)/medium/users.json $(BENCH_GEN)/medium/users.ndjson:
    	@mkdir -p $(BENCH_GEN)/medium
    	PYTHONPATH=. $(VENV)/bin/$(PYTHON) -m benchmarks.data.generate_bench_data \
    		--out-dir $(BENCH_GEN)/medium --num-users 2000 --max-orders 20 --max-items 10

    $(BENCH_GEN)/large/users.json $(BENCH_GEN)/large/users.ndjson:
    	@mkdir -p $(BENCH_GEN)/large
    	PYTHONPATH=. $(VENV)/bin/$(PYTHON) -m benchmarks.data.generate_bench_data \
    		--out-dir $(BENCH_GEN)/large --num-users 4000 --max-orders 40 --max-items 20

    bench-small: $(BENCH_GEN)/small/users.json  ## Run full benchmark suite (small, ~1 MB)
    	$(BENCH_SUITE) \
    		--json-data $(BENCH_GEN)/small/users.json \
    		--ndjson-data $(BENCH_GEN)/small/users.ndjson \
    		--output docs/benchmarks/bench_results_small.md

    bench-medium: $(BENCH_GEN)/medium/users.json  ## Run full benchmark suite (medium, ~6 MB)
    	$(BENCH_SUITE) \
    		--json-data $(BENCH_GEN)/medium/users.json \
    		--ndjson-data $(BENCH_GEN)/medium/users.ndjson \
    		--output docs/benchmarks/bench_results_medium.md

    bench-large: $(BENCH_GEN)/large/users.json  ## Run full benchmark suite (large, ~44 MB)
    	$(BENCH_SUITE) \
    		--json-data $(BENCH_GEN)/large/users.json \
    		--ndjson-data $(BENCH_GEN)/large/users.ndjson \
    		--output docs/benchmarks/bench_results_large.md

  Remove these Makefile targets entirely:
    - bench-data (data generation is now an implicit dependency)
    - bench-ndjson
    - bench-json-parallel
    - bench-all
    - bench-unified
    - bench-unified-quick
    - bench-unified-comprehensive
    - bench-unified-save

  Update .PHONY to list only: bench-small bench-medium bench-large

Step 4 — Retain existing bench_*.py files as importable library modules:
  Do NOT delete bench_loads.py, bench_dumps.py, bench_ndjson.py, bench_search.py.
  They contain valuable runner functions and query definitions. bench_suite.py
  imports from them. But remove their `if __name__ == "__main__"` blocks and
  argparse CLI — they are no longer standalone entry points. The ONLY CLI entry
  point is bench_suite.py.

  Exception: keep bench_main.py's argparse/CLI temporarily for backward compat
  but add a deprecation warning. Remove it in a follow-up.

  Delete bench_unified.py entirely — its randomized-data approach is valuable
  but belongs in a separate testing tool, not in the benchmark suite. The
  canonical benchmarks use the deterministic generated datasets (Rule 8 requires
  "same datasets" for comparability). Move any useful data generation patterns
  from data_generator.py into benchmarks/data/ if needed.

  Keep bench_ndjson_cursor.py as-is (it's a focused micro-benchmark, not part
  of the main suite).

Step 5 — Enrich the report with what's currently missing:
  - dumps results (currently NOT in the Markdown report from Make)
  - NDJSON-specific metrics (line count, per-line throughput) in a single
    table alongside JSON results
  - Strata rank and gap-to-#1 per category in the Summary section
  - Data characteristics (file size, record count, average record size)
  - Rule 13 compliance assessment: is strata #1 in each category? If not,
    what's the gap?

Step 6 — Update reporters/ to work with the new unified result model:
  - markdown_reporter.py: refactor to accept list[BenchmarkResult] instead
    of FeatureReport. Simplify — it no longer needs to handle bench_unified's
    complex FeatureReport hierarchy.
  - json_reporter.py: adapt for BenchmarkResult; ensure regression_check.py
    compatibility.
  - csv_reporter.py: adapt for BenchmarkResult.

Step 7 — Update regression_check.py:
  - Parse the new single-file Markdown format (instead of the append-mode
    patchwork).
  - Or preferably, consume the JSON output from bench_suite.py --json-output
    for reliable structured comparison.

Step 8 — Validate:
  - Run `make bench-small` and verify: single process, single Markdown file,
    all 7 categories present in the report.
  - Compare output against current bench_results_small.md to ensure no
    benchmark category was dropped.
  - Run `make bench-medium` and `make bench-large` likewise.
  - Ensure `make help` shows exactly three bench targets.
  - Ensure no other bench-* targets exist in the Makefile.

=== RULES TO FOLLOW ===

- Rule 3: report goes in docs/benchmarks/ only
- Rule 8: same datasets, warmup + iterations, report min/median/p95, capture
  environment, measure memory
- Rule 13: report must assess whether strata is #1 per category
- Rule 14: no test regressions (run `make test` before and after)
- Rule 16: Makefile is the ONLY user-facing interface; no scripts/bench.sh
- Rule 17: maintain benchmark log compatibility (progress_log.md)

=== WHAT NOT TO DO ===

- Do NOT change the actual benchmark methodology (timing, GC collection,
  RSS capture) — harness.py is correct and well-tested.
- Do NOT change the library runner functions — they correctly handle import
  fallbacks and measure apples-to-apples.
- Do NOT add new benchmark categories yet — this prompt is about unification,
  not expansion.
- Do NOT make bench_suite.py depend on bench_unified.py's data_generator —
  use the deterministic generate_bench_data datasets only.
- Do NOT introduce Click or other CLI frameworks — argparse is fine.
- Do NOT change the data generation (users/orders/items schema, dataset sizes,
  --num-users/--max-orders/--max-items parameters) — those are proven and
  results must remain comparable to existing baselines.

=== DELIVERABLES ===

1. benchmarks/bench_suite.py — the single unified orchestrator
2. Updated Makefile — exactly 3 bench targets
3. Updated bench_loads.py, bench_dumps.py, bench_ndjson.py, bench_search.py —
   stripped of CLI/argparse, importable as libraries
4. Deleted bench_unified.py
5. Updated reporters/ to handle BenchmarkResult
6. Updated regression_check.py for new format
7. New bench_results_small.md generated by the unified suite (for verification)
8. All tests passing (`make test`)
```
