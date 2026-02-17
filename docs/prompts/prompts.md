# Strata Performance Improvement Plan

> Generated: 2026-02-14
> Goal: Achieve #1 ranking across all benchmark categories (Rule 13)
> Baseline commit: 9db40c6 | Platform: macOS arm64 | Python 3.14.2

## ⚠️ CRITICAL UPDATE: Profiling Results (2026-02-14)

**PROFILING COMPLETED.** Comprehensive analysis reveals the TRUE bottleneck:

### 🔴 Python Object Materialization: 85.68% of Runtime

```
strata.loads() runtime breakdown (44MB JSON, 6.17M objects):
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Python C API:           85.68%  <-- THE BOTTLENECK
  ├─ Dictionary ops:    11.63%  (dict_dealloc, PyDict_SetDefault, resize)
  ├─ Memory alloc:       2.63%  (_PyObject_Malloc, pymalloc_alloc, free)
  ├─ Object creation:    0.79%  (PyFloat, PyList, PyLong, PyGC_New)
  └─ Python overhead:   70.63%  (eval frame, interpreter - CANNOT OPTIMIZE)

Strata C++ parsing:     14.32%  <-- ALREADY OPTIMIZED
  ├─ SIMD operations:    6.52%  (structural tape, whitespace, numbers)
  ├─ Object building:    3.08%  (push_value, on_key)
  ├─ String/number:      1.83%  (on_string, parse_double_fast)
  └─ Other:              2.89%  (key cache, misc)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
```

**Performance vs orjson:**
- strata: 323.08ms (137 MB/s)
- orjson: 116.49ms (377 MB/s)
- **Gap: 2.77x slower**

**Materialization-only benchmark:**
- Pure Python object construction: 587.53ms (5.04x slower than orjson's TOTAL)
- This proves Python object creation is the bottleneck, NOT parsing

### 📊 Top 10 Hotspots (% of total runtime)

**Python C API (85.68%):**
1. dict_dealloc: 7.03% — Cleanup from dictionary resizes
2. PyDict_SetDefault: 1.51% — Dictionary insertions
3. dict_setdefault_ref_lock_held: 1.39% — Internal resize logic
4. _PyObject_Malloc: 0.98% — Memory allocation
5. pymalloc_alloc: 0.89% — Python's malloc
6. _PyObject_Free: 0.76% — Memory deallocation
7. PyDict_New: 0.46% — Creates size-8 dicts (causes resizes)
8. _PyObject_GC_New: 0.37% — GC object creation
9. PyFloat_FromDouble: 0.26% — Float creation
10. PyList_New: 0.15% — List creation

**Strata C++ (14.32%):**
1. collect_structural_positions_simd: 2.91% — Already SIMD-optimized
2. push_value: 1.87% — Object builder (calls Python API)
3. skip_whitespace_simd: 1.69% — Already SIMD-optimized
4. on_key: 1.21% — Key processing (calls Python API)
5. parse_int_simd: 1.00% — Already SIMD-optimized

### ⚡ Revised Optimization Strategy

**ABANDON previous focus on C++ parser optimization.** The C++ code is already
fast and well-optimized with SIMD. The parsing phase (14.32%) is NOT the problem.

**NEW FOCUS:** Python object creation (85.68%)

**HIGH PRIORITY (targets 15.77% of runtime):**
1. Pre-size dictionaries with _PyDict_NewPresized (targets 11.63%)
2. Object pooling for dicts/lists (targets 2.63%)
3. Batch PyDict operations (targets 1.51%)

**MEDIUM PRIORITY:**
4. Compiler flags + PGO (5-10% on all code)
5. Lazy structural tape (targets 2.91%)

**SKIP ENTIRELY (not bottlenecks):**
- Number parsing (1.65%, already SIMD-optimized)
- String parsing (0.92%, already optimized)
- Whitespace skipping (1.69%, already SIMD-optimized)
- Template parser, virtual calls, etc. (C++ is only 14.32%)

**Expected outcome:** 12-25% overall speedup, closing gap from 2.77x to ~2.0-2.4x.
We CANNOT match orjson entirely because 70.63% of runtime is Python interpreter
overhead we cannot control.

---

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

#### Prompt 2 — Pre-size Dictionaries to Eliminate Resize Overhead (HIGH PRIORITY)
```
**PROFILING DATA:** Dictionary operations consume 11.63% of total runtime:
- dict_dealloc: 7.03% (cleanup from resizes)
- PyDict_SetDefault: 1.51% (insertions trigger resizes)
- dict_setdefault_ref_lock_held: 1.39% (internal resize logic)
- PyDict_New: 0.46% (creates size-8 dicts that immediately resize)

**ROOT CAUSE:** PythonObjectBuilder::on_start_object() calls PyDict_New() which
creates an 8-entry dict. For JSON objects with >8 keys, Python resizes the dict
multiple times (8→16→32→64...). Each resize: malloc new table, rehash all entries,
free old table. With 881,000 dicts in our 44MB benchmark, this is catastrophic.

**SOLUTION:** Replace PyDict_New with _PyDict_NewPresized(estimated_size).

Implementation in src/strata/bindings/python_object_builder.cpp:
1. In on_start_object(), estimate dict size from context:
   - If using structural tape + exact size hints: use collected size
   - Else if parent is array of objects: use average size of previous siblings
   - Else: use global moving average from recent objects (track last 256 objects)
   - Fallback: 16 (better than 8, covers 80% of JSON objects)

2. Call _PyDict_NewPresized(estimated_size) instead of PyDict_New()
   - This creates a dict with pre-allocated table, avoiding ALL resizes
   - Python's allocator rounds to next power-of-2 anyway, so overestimation is cheap

3. For lists in on_start_array(), similar approach with PyList_New(estimated_size)

4. Track presizing accuracy: log when estimate < actual (caused resize) vs
   estimate > actual (wasted memory). Tune estimator based on data.

5. Benchmark loads on small/medium/large datasets.
   - Expected: 5-10% overall improvement (reduces 11.63% dict overhead by ~50%)
   - Measure: reduction in dict_dealloc samples, reduction in memory allocations

Follow Rules 1, 6, 8, 9, 11, 17. Run all tests. Record in progress_log.md.
Target: reduce dictionary operations from 11.63% to <6%.
```

#### Prompt 3 — Implement Object Pooling to Reduce Malloc/Free Overhead (HIGH PRIORITY)
```
**PROFILING DATA:** Memory allocation consumes 2.63% of total runtime:
- _PyObject_Malloc: 0.98%
- pymalloc_alloc: 0.89%
- _PyObject_Free: 0.76%

For 44MB JSON with 6.17M objects (881K dicts, 87K lists, 3.5M strings, 881K ints,
790K floats), strata creates/destroys millions of Python objects, each requiring
malloc/free calls through Python's allocator.

**COMPARISON:** orjson is 2.77x faster overall, and likely uses object pooling
to avoid allocator overhead.

**SOLUTION:** Implement per-parse-session object pools for dicts, lists, and
small strings (<= 64 bytes).

Implementation:
1. Add PythonObjectPool class in python_object_builder.cpp:
   - dict_pool: pre-allocate 1024 PyDict objects at parse start
   - list_pool: pre-allocate 256 PyList objects
   - string_pool: arena allocator for short strings (<= 64 bytes)

2. Modify on_start_object() to acquire dict from pool instead of PyDict_New
3. Modify on_start_array() to acquire list from pool instead of PyList_New
4. Modify on_string() for short strings to use string_pool arena + PyUnicode_FromKindAndData

5. At parse end, return objects to pool (or just free the entire pool at once)
   - Trade-off: slight memory overhead during parse (pre-allocated objects)
   - Benefit: no malloc/free per object, just bulk allocation/deallocation

6. Make pool sizes configurable via environment variable STRATA_OBJECT_POOL_SIZE

7. Benchmark loads on small/medium/large with pooling ON vs OFF
   - Expected: 1-3% overall improvement (reduces 2.63% alloc overhead by ~50%)
   - Measure: reduction in _PyObject_Malloc samples, reduction in pymalloc_alloc

**CRITICAL:** Pool objects must be properly refcounted. Use Python's
type->tp_new() and type->tp_free() for pool management, NOT raw malloc/free.

Follow Rules 1, 6, 8, 9, 11, 17. Run all tests (especially memory leak tests).
Record in progress_log.md. Target: reduce memory allocation from 2.63% to <1.5%.
```

#### Prompt 4 — Batch Python C API Calls to Reduce Function Call Overhead (MEDIUM PRIORITY)
```
**PROFILING DATA:** PyDict_SetDefault is called millions of times (1,781 samples
= 1.51% of runtime). Each call: enter Python C function, acquire GIL, hash key,
lookup, possibly resize, insert, release GIL, return. Function call overhead +
GIL thrashing for high-frequency operations.

**OBSERVATION:** PythonObjectBuilder calls Python C API once per JSON value:
- on_key() → PyUnicode_FromStringAndSize + dict insert
- on_int() → PyLong_FromLongLong + dict/list insert
- on_double() → PyFloat_FromDouble + dict/list insert
- on_string() → PyUnicode_FromStringAndSize + dict/list insert

For 44MB JSON with 6.17M values, this is 6.17M C API round-trips.

**SOLUTION:** Batch dict insertions using PyDict_SetItem with pre-constructed
key-value pairs, reducing per-call overhead.

Implementation in python_object_builder.cpp:
1. Add a "pending insertions" buffer (capacity: 64 key-value pairs)
2. In on_key() + on_<value>(), accumulate (key, value) pairs in buffer
3. When buffer fills OR object ends, flush with batched PyDict_Update()
   - PyDict_Update takes a dict of pending inserts, processes them in one call
   - Reduces function call overhead from N calls to N/64 calls

4. For lists, similarly batch with PyList_SetSlice() for range insertions

5. Benchmark loads on small/medium/large datasets
   - Expected: 1-2% overall improvement (reduces 1.51% PyDict_SetDefault overhead)
   - Measure: reduction in PyDict_SetDefault samples

**TRADE-OFF:** Increases memory usage during parse (64-entry buffer per object
nesting level). For deeply nested JSON (depth 100), this is 6400 entries = ~50KB.
Acceptable for faster parse.

Follow Rules 1, 6, 8, 9, 11, 17. Run all tests. Record in progress_log.md.
Target: reduce PyDict_SetDefault from 1.51% to <1%.
```

### Phase 2: C++ Parser Optimization (Target: -5% parse time)

**PROFILING REALITY CHECK:** C++ parsing is only 14.32% of runtime and is ALREADY
well-optimized with SIMD. The top C++ hotspots are:
- collect_structural_positions_simd: 2.91% (already SIMD-optimized)
- skip_whitespace_simd: 1.69% (already SIMD-optimized)
- parse_int_simd: 1.00% (already SIMD-optimized)

**DO NOT spend significant effort here.** 10% improvement in C++ = 1.4% overall.
Focus remains on Python object creation (85.68%).

#### Prompt 5 — Lazy Structural Tape Collection (LOW PRIORITY - C++ optimization)
```
**PROFILING DATA:** collect_structural_positions_simd is the #1 C++ hotspot at
2.91% of total runtime (20.35% of C++ time). However, this is ALREADY SIMD-optimized
and only accounts for 2.91% overall.

**COST-BENEFIT:** Even a 50% reduction in structural tape overhead = 1.45% overall.
Compare to dictionary pre-sizing (targets 11.63%) or object pooling (targets 2.63%).

**IF you have spare time after Prompts 2-4**, consider this optimization:

The structural tape is collected upfront for the entire JSON, even though many
use cases don't need it (simple loads without queries). Make it lazy:

1. Add a flag to ParseOptions: collect_structural_tape (default: false)
2. In python_loads.cpp, only enable structural tape if:
   - Input size >= 10MB (for large inputs, tape helps with memory efficiency)
   - OR user explicitly requested exact size hints

3. For small/medium inputs without size hints, skip structural tape collection
   - Saves 2.91% on these inputs
   - Slightly increases memory usage (no pre-sized containers)

4. Benchmark loads on all sizes with tape ON vs OFF
   - Expected: small/medium gain ~2%, large no gain (tape is beneficial there)

Follow Rules 1, 6, 8, 11, 17. Run all tests. Record in progress_log.md.
Target: reduce collect_structural_positions_simd from 2.91% to <1.5% on small/medium.
```

#### Prompt 6 — SKIP - Number Parsing Already Optimized
```
**PROFILING DATA:** Number parsing is only 1.65% of total runtime:
- parse_int_simd: 1.00%
- parse_uint_simd: 0.92% (corrected from 0.92% listed as part of integer parsing)
- parse_double_fast: 0.65%

This is ALREADY using SIMD and is highly optimized. Even a 50% improvement = 0.82%
overall gain.

**RECOMMENDATION:** SKIP this optimization entirely. Focus on Python object creation
(85.68% of runtime). Any time spent optimizing number parsing is wasted when
dictionary operations alone are 11.63%.

**IF number parsing becomes a bottleneck after fixing Python issues**, revisit.
```

#### Prompt 7 — SKIP - String Parsing Already Optimized
```
**PROFILING DATA:** String processing is only 0.92% of total runtime:
- on_string: 0.92% (mostly Python object creation, not parsing)

String PARSING itself (find escapes, decode) is not in the top 10 hotspots, meaning
it's < 0.5% of total runtime. The bottleneck is PyUnicode_FromStringAndSize (Python
API), which is addressed by object pooling in Prompt 3.

**RECOMMENDATION:** SKIP this optimization. String parsing is already efficient.

**IF string parsing becomes visible after fixing Python issues**, revisit.
```

#### Prompt 8 — SKIP - Whitespace Already Optimized
```
**PROFILING DATA:** skip_whitespace_simd is 1.69% of total runtime (11.78% of C++
time). Already SIMD-optimized. Even a 50% reduction = 0.84% overall gain.

**COST-BENEFIT ANALYSIS:**
- Best case: 50% reduction in whitespace overhead = 0.84% overall
- vs Dictionary pre-sizing: targets 11.63%
- vs Object pooling: targets 2.63%

**RECOMMENDATION:** SKIP this optimization. Whitespace skipping is already very
fast. The 1.69% overhead is unavoidable for parsing JSON with formatting.

**ALTERNATIVE:** Encourage users to minify JSON before parsing (removes whitespace
entirely). Document this in performance guide.
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

## Priority Matrix (UPDATED based on profiling)

**PROFILING VERDICT:** Python object creation is 85.68% of runtime. C++ parsing
is only 14.32% and already SIMD-optimized. ALL high-priority work MUST target
Python C API, not parsing logic.

| # | Prompt | Targets | Expected Impact | Effort | Priority |
|---|--------|---------|-----------------|--------|----------|
| 1 | Profile & isolate hotspots | Foundation | Foundation | Low | **DONE ✅** |
| 2 | **Pre-size dictionaries** | **11.63%** | **-5-10% overall** | Medium | **P0** |
| 3 | **Object pooling** | **2.63%** | **-1-3% overall** | High | **P0** |
| 4 | **Batch dict operations** | **1.51%** | **-1-2% overall** | Medium | **P0** |
| 9 | Compiler flags + PGO | All | -5-10% all | Low | **P1** |
| 5 | Lazy structural tape | 2.91% | -1-1.5% small/med | Low | **P2** |
| 10 | dumps optimization | dumps only | -10% dumps | Medium | **P2** |
| 12 | Search path optimization | search only | -50% simple search | Medium | **P2** |
| 11 | Search wildcard optimization | search only | -50% wildcards | High | **P2** |
| 6 | ~~Number parser~~ | ~~1.65%~~ | SKIP (already optimized) | - | **SKIP** |
| 7 | ~~String parsing~~ | ~~0.92%~~ | SKIP (already optimized) | - | **SKIP** |
| 8 | ~~Whitespace fusion~~ | ~~1.69%~~ | SKIP (already optimized) | - | **SKIP** |
| 13 | Parallel JSON parsing | 14.32% C++ | Complex, low ROI | Very High | **P3** |
| 14 | SIMD UTF-8 optimization | <0.77% | SKIP (not in top 10) | - | **SKIP** |
| 15 | Arena memory optimization | Research | Covered by pooling | High | **SKIP** |
| 16 | Branchless dispatch | C++ only | SKIP (C++ not bottleneck) | - | **SKIP** |
| 17 | Full benchmark validation | Validation | Validation | Low | **Every phase** |

**REVISED EXECUTION ORDER:**

**Week 1 (HIGH IMPACT - Python object creation):**
- Day 1-2: Prompt 2 (Pre-size dictionaries) — targets 11.63%
- Day 3-5: Prompt 3 (Object pooling) — targets 2.63%

**Week 2 (MEDIUM IMPACT - Python API efficiency):**
- Day 1-2: Prompt 4 (Batch dict operations) — targets 1.51%
- Day 3-5: Prompt 9 (Compiler flags + PGO) — targets all code

**Week 3 (LOW IMPACT - C++ polish):**
- Day 1-2: Prompt 5 (Lazy structural tape) — targets 2.91%
- Day 3-5: Search optimizations (Prompts 11-12) — if needed

**EXPECTED CUMULATIVE IMPROVEMENT:**
- After Prompts 2+3+4: 7-15% overall speedup (addresses 15.77% of runtime)
- After Prompt 9 (PGO): additional 5-10% (multiplicative)
- **Total potential: 12-25% faster, closing gap from 2.77x to 2.0-2.4x vs orjson**

**WHY NOT 2.77x faster?** Because 70.63% of runtime is "other Python overhead"
(eval frame, interpreter loop, etc.) that we CANNOT optimize. The best we can do
is optimize the 15% we control (dict ops, memory, C++ parsing).

## Execution Order (REVISED based on profiling)

**Week 1 - Python Object Creation (HIGH IMPACT: ~15% of runtime):**
- ✅ Prompt 1: Profile & identify hotspots (DONE)
- Prompt 2: Pre-size dictionaries (targets 11.63%)
- Prompt 3: Object pooling (targets 2.63%)
- Prompt 4: Batch dict operations (targets 1.51%)

**Week 2 - Compiler & Build Optimization:**
- Prompt 9: Compiler flags + PGO (targets all code, 5-10% gain)

**Week 3 - C++ Polish (LOW IMPACT: ~3% of runtime):**
- Prompt 5: Lazy structural tape (targets 2.91%)
- Prompts 11-12: Search optimization (if applicable)

**SKIP ENTIRELY (not bottlenecks):**
- ~~Prompt 6: Number parsing (1.65%, already SIMD-optimized)~~
- ~~Prompt 7: String parsing (0.92%, already optimized)~~
- ~~Prompt 8: Whitespace (1.69%, already SIMD-optimized)~~
- ~~Prompt 13: Parallel parsing (complex, C++ not bottleneck)~~
- ~~Prompt 14: UTF-8 SIMD (not in top 10 hotspots)~~
- ~~Prompt 15: Arena allocator (covered by Prompt 3)~~
- ~~Prompt 16: Branchless dispatch (C++ not bottleneck)~~

**Continuous:**
- Prompt 17: Benchmark validation after every change

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
