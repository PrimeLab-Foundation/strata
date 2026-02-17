# Python Bindings Optimization Prompts

> Generated 2026-02-18 based on profiling data showing **85.68% of runtime in Python C API calls**.
> Current gap: strata is **2.77x slower** than orjson on `loads` (136.66 MB/s vs 376.58 MB/s).
> Goal: close the gap to ≤1.5x on `loads`, maintain correctness across all 680 Python + 24 C++ tests.

---

## How to Use These Prompts

Each prompt below is a self-contained task you can give to an AI coding agent (or work on yourself). They are ordered by **expected impact** (highest first). Each follows Rule 12 (Research-Driven Experimentation): hypothesis → prototype → microbenchmark → go/no-go.

**Before running any prompt**, ensure:
1. Clean working tree on `main-v2-0.1`
2. Build works: `SKIP_TESTS=1 .venv/bin/pip install -e .`
3. Baseline benchmarks captured: `make bench-small && make bench-medium && make bench-large`

**After each prompt's work**, verify:
1. All tests pass: `make test` (680 Python + 24 C++ tests)
2. No regression > 2% on any benchmark category (Rule 17)
3. Progress logged in `docs/benchmarks/progress_log.md` (Rule 17)

---

## Prompt 1: Replace `PyDict_SetDefault` with Direct Hash-Table Insertion

**Priority:** CRITICAL — `PyDict_SetDefault` + `dict_setdefault_ref_lock_held` = 2.90% of total runtime
**Expected gain:** 5–15% on `loads` throughput

```
You are optimizing the Strata JSON parser's Python bindings for maximum loads() performance.

## Context
- Project: /Users/borysbardysh/PycharmProjects/personal/PrimeLabFoundation/strata
- Branch: main-v2-0.1
- Key file: src/strata/bindings/python_object_builder.h (PythonObjectBuilder class)
- Build: SKIP_TESTS=1 .venv/bin/pip install -e .
- Tests: make test (680 Python + 24 C++ tests must pass)
- Rules: read all files in .aiassistant/rules/ and follow them strictly

## Problem
Profiling shows PyDict_SetDefault accounts for 2.90% of total runtime (1,781 + 1,644 samples).
PyDict_SetDefault does a hash lookup, checks for existing key, and sets the value — but during
JSON parsing we KNOW the key does not exist yet (JSON objects have unique keys at parse time),
so the "check if exists" part is wasted work.

## Task
Replace PyDict_SetDefault with lower-overhead dictionary insertion. Research and implement
1–3 approaches (Rule 12):

### Approach A: PyDict_SetItem (bypass existence check)
- PyDict_SetItem skips the existence check that PyDict_SetDefault does
- It still does a full hash+probe but doesn't return the existing value
- Hypothesis: 1–3% improvement by removing redundant existence check

### Approach B: Direct CPython dict internals (_PyDict_SetItem_KnownHash)
- CPython exposes _PyDict_SetItem_KnownHash(dict, key, value, hash) in internal headers
- Since KeyCache already computes FNV-1a hashes, we can pre-compute Python hash and pass it
- Use PyObject_Hash(key) when inserting interned keys (hash is cached on the string object)
- Hypothesis: 3–8% improvement by eliminating redundant hash computation during insertion

### Approach C: Build dict from pre-sized arrays using _PyDict_NewPresized + direct ma_keys manipulation
- For each JSON object, collect all key-value pairs into a stack-allocated array
- Then build the dict in one shot using batch insertion
- Currently PythonObjectBuilder does partial batching (64 items) — extend to full object batching
- Hypothesis: 5–15% improvement by amortizing dict resize/insertion overhead

## Implementation Requirements
1. Isolate experiment in experiments/dict_insert_optimization/ first
2. Benchmark on all three datasets: small (0.96MB), medium (6.25MB), large (43.85MB)
3. Test with Python 3.11, 3.12, 3.13, 3.14 (dict internals differ across versions)
4. Ensure duplicate_key_policy still works (LastWins, FirstWins, Strict)
5. If using CPython internals, add version guards (#if PY_VERSION_HEX >= ...)
6. The current batched dict insertion code is in PythonObjectBuilder::flush_batch_to_dict()
   and on_end_object() — modify these paths
7. Measure both throughput (MB/s) and dict_dealloc overhead (currently 7.03%)

## Success Criteria
- ≥3% improvement on medium/large loads benchmarks
- No regression >2% on any benchmark category
- All 680 Python + 24 C++ tests pass
- Progress logged in docs/benchmarks/progress_log.md
```

---

## Prompt 2: Eliminate `dict_dealloc` Overhead via Deferred Cleanup

**Priority:** CRITICAL — `dict_dealloc` = 7.03% of total runtime (largest single hotspot)
**Expected gain:** 3–7% on `loads` throughput

```
You are optimizing the Strata JSON parser's Python bindings for maximum loads() performance.

## Context
- Project: /Users/borysbardysh/PycharmProjects/personal/PrimeLabFoundation/strata
- Branch: main-v2-0.1
- Key files: src/strata/bindings/python_object_builder.h, src/strata/bindings/python_loads.cpp
- Build: SKIP_TESTS=1 .venv/bin/pip install -e .
- Tests: make test (680 Python + 24 C++ tests must pass)
- Rules: read all files in .aiassistant/rules/ and follow them strictly

## Problem
dict_dealloc is the #1 hotspot at 7.03% of total runtime (8,316 samples). This happens when
temporary Python dicts created during parsing are freed. The dealloc chain includes:
- Decrementing refcounts on all keys and values
- Freeing the dict's internal hash table (ma_keys)
- Returning the dict to CPython's freelist or calling pymalloc_free

Most of this cost is from intermediate dicts that get REPLACED during the parse (e.g., pool
drain, failed objects, GC cycles). The final result dict is held by the caller so its dealloc
doesn't happen during parsing.

## Task
Research and implement 1–3 approaches to reduce dict_dealloc overhead (Rule 12):

### Approach A: Reduce temporary dict creation
- Audit PythonObjectBuilder for unnecessary dict creation/destruction cycles
- The object pool's drain() creates and destroys dicts — can this be avoided?
- Profile which code paths trigger dict_dealloc during parsing
- Hypothesis: 2–4% improvement by eliminating unnecessary dict lifecycle churn

### Approach B: GC pause optimization
- Currently PyGcPause disables GC for inputs ≥256KB
- dict_dealloc triggers when refcount hits 0, which can cascade
- Investigate if we can defer Py_DECREF on intermediate objects until after parsing
- Use a "deferred decref" list: collect PyObject* to decref, batch-decref after parse completes
- Hypothesis: 2–5% improvement by batching deallocation outside the hot parse loop

### Approach C: Reuse dict objects instead of create/destroy
- Instead of creating new dicts for each JSON object, clear and reuse existing dicts
- PyDict_Clear() is cheaper than PyDict_New() + dict_dealloc()
- Maintain a small ring buffer of cleared dicts (separate from the existing object pool)
- Key difference from existing pool: these dicts are CLEARED (not newly created) so
  their internal hash tables can be reused
- Hypothesis: 1–3% improvement from reduced malloc/free pressure

## Important Context
- The existing PythonObjectPool (python_object_builder.h lines 398-665) pre-creates dicts
  but this was shown to have <1% impact because pymalloc is already fast
- The key insight is that dict_dealloc's 7.03% cost is NOT from the result objects — it's
  from TEMPORARY dicts created and destroyed during parsing
- To find where temps are created, add debug logging to acquire_dict/release paths

## Implementation Requirements
1. First, instrument the code to identify WHERE dict_dealloc calls originate
2. Profile with Instruments/sample to confirm which code paths cause the 7.03%
3. Isolate experiment in experiments/dict_dealloc_reduction/
4. Benchmark on all three datasets
5. Ensure memory stability: run tests/py/test_memory_arena.py
6. dict_dealloc is in CPython — we can't change it, but we can avoid triggering it

## Success Criteria
- ≥2% improvement on large loads benchmark
- dict_dealloc drops from 7.03% to <5% in profile
- No memory leaks (valgrind/ASAN clean or memory stability tests pass)
- All 680 Python + 24 C++ tests pass
- Progress logged in docs/benchmarks/progress_log.md
```

---

## Prompt 3: Inline Small-Integer and Singleton Creation (Eliminate PyLong/PyFloat Overhead)

**Priority:** HIGH — PyFloat_FromDouble + PyLong_FromLong + GC_New = 1.22% of total runtime
**Expected gain:** 2–5% on `loads` throughput

```
You are optimizing the Strata JSON parser's Python bindings for maximum loads() performance.

## Context
- Project: /Users/borysbardysh/PycharmProjects/personal/PrimeLabFoundation/strata
- Branch: main-v2-0.1
- Key file: src/strata/bindings/python_object_builder.h (PythonObjectBuilder class)
- Build: SKIP_TESTS=1 .venv/bin/pip install -e .
- Tests: make test (680 Python + 24 C++ tests must pass)
- Rules: read all files in .aiassistant/rules/ and follow them strictly

## Problem
The dataset has 881K ints and 790K floats. Creating Python objects for each one costs:
- PyFloat_FromDouble: 313 samples (0.26%)
- PyLong_FromLong: 80 samples (0.07%)
- _PyObject_GC_New: 443 samples (0.37%)
- Plus hidden cost inside the 70.63% "other Python overhead" bucket

CPython has a small integer cache ([-5, 256]) and a float freelist, but we're not
maximally exploiting them. Additionally, JSON commonly has values like 0, 1, 0.0, 1.0,
true, false, null — these can be cached as immortal singletons.

## Task
Research and implement 1–3 approaches (Rule 12):

### Approach A: Aggressive small-integer fast-path
- CPython caches integers in [-5, 256] — use PyLong_FromLong for these (already cached)
- For integers in [257, 65535], maintain a thread-local cache of recently-used ints
- JSON data often has repeated integer values (IDs, counts, status codes)
- In PythonObjectBuilder::on_int(), check the cache before calling PyLong_FromLongLong
- Hypothesis: 1–2% improvement by avoiding allocation for repeated integer values

### Approach B: Float deduplication cache
- JSON datasets frequently repeat float values (0.0, 1.0, -1.0, common prices like 9.99)
- Maintain a small (64-entry) thread-local float cache keyed by raw bits (uint64_t)
- On PyFloat_FromDouble, check cache first; on hit, Py_INCREF and return
- Hypothesis: 1–3% improvement depending on float value repetition in data

### Approach C: Bypass GC tracking for leaf objects
- PyFloat and PyLong objects cannot contain references to other objects
- On Python 3.12+, these types may already skip GC tracking
- On older versions, we can use PyObject_GC_UnTrack after creation to remove from GC lists
- This reduces GC traversal overhead during gc.collect() and reduces _PyObject_GC_New cost
- Check: Python 3.12+ has immortal objects — use Py_None, Py_True, Py_False directly
  (already done in current code, verify completeness)
- Hypothesis: 0.5–1% improvement by reducing GC pressure

### Approach D: Direct struct initialization for PyFloat
- Instead of PyFloat_FromDouble (which goes through type machinery), use:
  PyFloat *op = PyObject_New(PyFloat, &PyFloat_Type);
  op->ob_fval = value;
- This skips the freelist check + type init overhead on versions where it's safe
- Version-guard this carefully (CPython internals change)
- Hypothesis: 0.5–1% improvement by bypassing float creation machinery

## Implementation Requirements
1. Profile the benchmark data to determine actual value distributions:
   - How many unique integer values? How many fall in [-5, 256]?
   - How many unique float values? What are the most common?
2. Isolate experiment in experiments/value_creation_optimization/
3. Thread-local caches must be bounded (64 entries for floats, 256 for ints beyond small cache)
4. All cached objects must have correct refcounts (Py_INCREF on cache hit)
5. Cache must be drained safely (Py_DECREF all entries) when thread exits
6. Benchmark on all three datasets

## Success Criteria
- ≥1.5% improvement on medium/large loads benchmarks
- No memory leaks
- All 680 Python + 24 C++ tests pass
- Progress logged in docs/benchmarks/progress_log.md
```

---

## Prompt 4: Optimize String Creation with Interning and Direct Allocation

**Priority:** HIGH — 3.5M strings created per large parse, string operations = significant hidden cost
**Expected gain:** 3–8% on `loads` throughput

```
You are optimizing the Strata JSON parser's Python bindings for maximum loads() performance.

## Context
- Project: /Users/borysbardysh/PycharmProjects/personal/PrimeLabFoundation/strata
- Branch: main-v2-0.1
- Key file: src/strata/bindings/python_object_builder.h (PythonObjectBuilder class, KeyCache)
- Build: SKIP_TESTS=1 .venv/bin/pip install -e .
- Tests: make test (680 Python + 24 C++ tests must pass)
- Rules: read all files in .aiassistant/rules/ and follow them strictly

## Problem
The large dataset has 3.5M strings. Current string creation path:
1. on_key(): KeyCache lookup → if miss, PyUnicode_FromStringAndSize → intern → cache
2. on_string(): PyUnicode_DecodeUTF8 or PyUnicode_FromStringAndSize

KeyCache (lines 104-383 in python_object_builder.h) handles dict keys well, but:
- String VALUES (not keys) are never cached or interned
- JSON commonly has repeated string values ("active", "inactive", "pending", "USD", "USA")
- PyUnicode_FromStringAndSize does: allocate → copy → hash (on first use)
- For ASCII strings (majority of JSON), this is wasteful when the value is repeated

## Task
Research and implement 1–3 approaches (Rule 12):

### Approach A: Value string interning for short repeated strings
- Maintain a thread-local value string cache (robin hood map, max 4096 entries)
- For strings ≤ 32 bytes, check cache before creating new PyUnicode
- On cache hit: Py_INCREF and return (zero allocation)
- Key insight: JSON API responses have MANY repeated short values
  ("status": "active", "type": "user", "currency": "USD")
- Use the same FNV-1a + robin hood approach as KeyCache
- Hypothesis: 3–6% improvement on real-world JSON with repeated values

### Approach B: Direct ASCII string allocation via PyUnicode_New
- PyUnicode_FromStringAndSize auto-detects encoding (ASCII vs UCS1 vs UCS2 vs UCS4)
- For strings we KNOW are ASCII (no bytes > 127), bypass detection:
  PyObject* s = PyUnicode_New(len, 127);  // max_char=127 → ASCII compact
  memcpy(PyUnicode_1BYTE_DATA(s), src, len);
- This skips the encoding detection loop inside PyUnicode_FromStringAndSize
- The parser already knows if a string is ASCII-only (no escape sequences with \uXXXX)
- Hypothesis: 1–3% improvement from faster string allocation

### Approach C: Lazy string creation with zero-copy views
- Don't create Python string objects during parsing at all
- Instead, store (offset, length, flags) tuples pointing into the original JSON buffer
- Create actual PyUnicode objects only when the value is accessed from Python
- This requires a custom "LazyStr" Python type that creates the real string on __str__
- Complexity: HIGH — affects all downstream code that expects real strings
- Hypothesis: 10–20% improvement but HIGH complexity risk
- Recommendation: Prototype in experiments/ only, do not merge without extensive testing

### Approach D: Expand KeyCache to cover ALL string values (not just keys)
- Current KeyCache only caches dict keys
- Extend it to also cache string values that appear as dict values
- Use a separate "ValueCache" with different eviction policy (LRU vs robin hood)
- For the large dataset: 3.5M strings, but how many unique? If <100K unique,
  caching could eliminate >90% of string allocations
- Hypothesis: 2–5% improvement depending on value repetition ratio

## Implementation Requirements
1. First, profile string value distributions in the benchmark datasets:
   - Total strings, unique strings, length distribution, ASCII vs non-ASCII ratio
2. KeyCache modifications must not regress key lookup performance
3. Value cache must be bounded and have efficient eviction
4. All cached strings must have correct refcounts
5. Must handle escaped strings correctly (cache the unescaped form)
6. Benchmark on all three datasets plus a dataset with high string diversity (UUIDs, timestamps)
7. Isolate experiment in experiments/string_optimization/

## Success Criteria
- ≥2% improvement on medium/large loads benchmarks
- No regression on small benchmarks (string caching overhead must be low)
- All 680 Python + 24 C++ tests pass
- Progress logged in docs/benchmarks/progress_log.md
```

---

## Prompt 5: Optimize the GC Pause Strategy and Object Lifecycle

**Priority:** HIGH — GC interaction is hidden inside the 70.63% "other Python overhead" bucket
**Expected gain:** 3–8% on `loads` throughput

```
You are optimizing the Strata JSON parser's Python bindings for maximum loads() performance.

## Context
- Project: /Users/borysbardysh/PycharmProjects/personal/PrimeLabFoundation/strata
- Branch: main-v2-0.1
- Key files: src/strata/bindings/python_object_builder.h, python_loads.cpp, python_types.h
- Build: SKIP_TESTS=1 .venv/bin/pip install -e .
- Tests: make test (680 Python + 24 C++ tests must pass)
- Rules: read all files in .aiassistant/rules/ and follow them strictly

## Problem
The current GC strategy (PyGcPause in python_types.h):
1. Disables GC at parse start for inputs ≥256KB
2. Re-enables GC after parsing completes
3. During parsing, all created dicts and lists are GC-tracked (PyObject_GC_Track)

The problem: even with GC disabled, every dict/list creation calls _PyObject_GC_New which
allocates from GC-tracked memory and adds to the GC generation 0 list. When GC is re-enabled,
generation 0 may have millions of objects, causing an expensive collection.

Additionally, the 70.63% "other Python overhead" is largely:
- GC tracking overhead on every dict/list creation
- Refcount manipulation (Py_INCREF/Py_DECREF) on every key/value insertion
- PyObject type checks and dispatch

## Task
Research and implement 1–3 approaches (Rule 12):

### Approach A: Delayed GC tracking
- Create dicts/lists WITHOUT GC tracking: use PyObject_Malloc + manual init instead of
  _PyObject_GC_New
- Only call PyObject_GC_Track() on the FINAL result object tree, not during construction
- During construction, we control all references, so GC tracking is unnecessary
- After parse completes, walk the result tree and GC_Track everything
- Hypothesis: 3–5% improvement by eliminating GC overhead during parse
- Risk: Must ensure all objects are tracked before returning to Python

### Approach B: Smarter GC threshold management
- Instead of blanket GC disable, manipulate GC thresholds:
  gc.set_threshold(0, 0, 0) during parse → no collections triggered
  Restore original thresholds after parse
- This is lighter than full GC disable and avoids the generation 0 explosion
- Also investigate: does the current PyGcPause implementation handle nested calls?
- Hypothesis: 1–3% improvement from smoother GC behavior

### Approach C: Minimize refcount operations during dict construction
- Currently for each key-value pair insertion:
  1. Py_INCREF(key)  — but key is already owned by KeyCache
  2. Py_INCREF(value) — value was just created with refcount 1
  3. PyDict_SetItem(dict, key, value) — internally does Py_INCREF on both again
  4. Py_DECREF(value) — release our reference
- This is 4 refcount operations per insertion when 0 are needed:
  Use PyDict_SetItem_KnownHash and STEAL references instead of borrowing
- _PyDict_SetItem_Take (CPython 3.13+) steals references to both key and value
- On older versions, use PyDict_SetItem but skip the extra Py_INCREF/DECREF pair
- Hypothesis: 2–5% improvement from halving refcount operations

### Approach D: Use PyDict_SetDefault with stolen references pattern
- Profile exactly how many Py_INCREF/Py_DECREF calls happen per parse
- For the large dataset (6.17M objects), estimate: ~20M refcount operations
- Each Py_INCREF/Py_DECREF is ~2ns (cache-friendly) but 20M × 2ns = 40ms
- That's ~12% of the 323ms parse time — significant!
- Audit every refcount operation in PythonObjectBuilder and eliminate unnecessary ones
- Hypothesis: 3–8% improvement from systematic refcount reduction

## Implementation Requirements
1. CRITICAL: All objects MUST be GC-tracked before returning to Python
2. CRITICAL: All refcounts MUST be correct — use ASAN/refcount debugging
3. Test with Python 3.12+ immortal objects (Py_None, Py_True, Py_False)
4. Test with gc.DEBUG_LEAK to verify no leaks
5. Version-guard any CPython internal API usage
6. Isolate experiment in experiments/gc_optimization/
7. Benchmark on all three datasets

## Success Criteria
- ≥2% improvement on medium/large loads benchmarks
- No memory leaks (gc.DEBUG_LEAK clean, memory stability tests pass)
- All 680 Python + 24 C++ tests pass
- Progress logged in docs/benchmarks/progress_log.md
```

---

## Prompt 6: Flatten the SAX Event Dispatch (Reduce Virtual Call Overhead)

**Priority:** MEDIUM — push_value (1.87%) + on_key (1.21%) + on_string (0.92%) = 4.0% C++ overhead
**Expected gain:** 2–4% on `loads` throughput

```
You are optimizing the Strata JSON parser's Python bindings for maximum loads() performance.

## Context
- Project: /Users/borysbardysh/PycharmProjects/personal/PrimeLabFoundation/strata
- Branch: main-v2-0.1
- Key files: src/strata/bindings/python_object_builder.h, include/strata/json/json_parse.hpp,
  include/strata/json/json_sax_handler.hpp
- Build: SKIP_TESTS=1 .venv/bin/pip install -e .
- Tests: make test (680 Python + 24 C++ tests must pass)
- Rules: read all files in .aiassistant/rules/ and follow them strictly

## Problem
The SAX parsing architecture uses virtual dispatch:
1. Parser calls handler->on_key(), handler->on_string(), handler->on_int(), etc.
2. Each is a virtual function call through JsonSaxHandler vtable
3. PythonObjectBuilder::push_value() has a switch-like dispatch for value types
4. These virtual calls prevent inlining and cause branch mispredictions

Profiler shows:
- PythonObjectBuilder::push_value: 2,580 samples (1.87% total, 13.05% of C++)
- PythonObjectBuilder::on_key: 1,674 samples (1.21% total, 8.47% of C++)
- PythonObjectBuilder::on_string: 1,267 samples (0.92% total, 6.41% of C++)

Combined: 4.0% of total runtime in C++ dispatch overhead that could be reduced.

## Task
Research and implement 1–3 approaches (Rule 12):

### Approach A: CRTP (Curiously Recurring Template Pattern) to eliminate virtual dispatch
- Replace virtual JsonSaxHandler with template-based static dispatch:
  template<typename Handler>
  Status parse_sax(std::string_view text, Handler& handler, ParseSaxOptions opts);
- PythonObjectBuilder methods become direct calls (inlinable)
- Compiler can inline on_int, on_bool, on_null directly into parse loop
- Hypothesis: 2–4% improvement from eliminated vtable indirection + inlining

### Approach B: Fused parse-and-build (no SAX events for common patterns)
- Detect common JSON patterns inline and build Python objects directly:
  - Flat object with string values: parse key, parse string, insert — no events
  - Array of integers: parse all ints, build PyList in one shot
  - Nested object: only emit SAX events for the outer structure
- This is already partially done in parse_flat_object() (python_loads.cpp lines 580-670)
- Extend to cover more patterns: arrays of objects, arrays of strings
- Hypothesis: 2–5% improvement on data that matches common patterns

### Approach C: Computed goto / threaded dispatch in parser
- Replace the main parser switch(token_type) with computed goto:
  static void* dispatch_table[] = { &&label_string, &&label_number, ... };
  goto *dispatch_table[token_type];
- This eliminates branch prediction misses on the main parse loop
- GCC and Clang both support computed goto as an extension
- Hypothesis: 1–2% improvement from better branch prediction

## Implementation Requirements
1. CRTP approach must maintain the existing SAX interface for other handlers
   (DomBuilderHandler, TapeBuilder) — use a base class with default virtual + template specialization
2. parse_sax must remain callable with both virtual and static dispatch
3. Benchmark the flat_object fast-path coverage: what % of objects in each dataset qualify?
4. Computed goto is compiler-specific — guard with __GNUC__ / __clang__
5. Isolate experiments in experiments/dispatch_optimization/
6. Benchmark on all three datasets + measure instruction cache behavior (perf stat)

## Success Criteria
- ≥2% improvement on large loads benchmark
- No regression on small benchmarks
- All 680 Python + 24 C++ tests pass
- Progress logged in docs/benchmarks/progress_log.md
```

---

## Prompt 7: Optimize `dumps` Serialization (Python → JSON) — Currently 7–8x Slower Than orjson

**Priority:** HIGH — dumps is 7–8x slower than orjson across all sizes
**Expected gain:** 30–50% on `dumps` throughput

```
You are optimizing the Strata JSON parser's Python bindings for maximum dumps() performance.

## Context
- Project: /Users/borysbardysh/PycharmProjects/personal/PrimeLabFoundation/strata
- Branch: main-v2-0.1
- Key file: src/strata/bindings/python_dumps.cpp
- Build: SKIP_TESTS=1 .venv/bin/pip install -e .
- Tests: make test (680 Python + 24 C++ tests must pass)
- Rules: read all files in .aiassistant/rules/ and follow them strictly

## Problem
Current dumps performance vs orjson:
- Small (0.96MB):  strata 7.01ms vs orjson 0.75ms — 839% slower
- Medium (6.25MB): strata 54.90ms vs orjson 6.72ms — 717% slower
- Large (43.85MB): strata 418.96ms vs orjson 48.80ms — 758% slower

This is the WORST performance gap in the entire library. The serialization path
(python_dumps.cpp) needs fundamental optimization.

## Task
Research and implement 1–3 approaches (Rule 12):

### Approach A: Reduce Python C API calls during serialization
- Profile python_dumps.cpp to identify the top hotspots
- Every PyDict_Next / PyList_GET_ITEM / PyUnicode_AsUTF8AndSize is a C API call
- For dict iteration, use _PyDict_Next (faster internal variant if available)
- For list iteration, access ob_item array directly: ((PyListObject*)list)->ob_item[i]
- For string access, avoid PyUnicode_AsUTF8AndSize when string is compact ASCII:
  Check PyUnicode_IS_COMPACT_ASCII(obj), then use _PyUnicode_COMPACT_DATA(obj)
- Hypothesis: 10–20% improvement from reduced API overhead

### Approach B: SIMD-accelerated string escaping
- JSON string escaping (replacing \, ", \n, \t, control chars) is a hot path
- Use SIMD to scan for characters that need escaping:
  Load 16/32 bytes, compare against escape set, if all clean → memcpy, else escape one-by-one
- The project already has simd_string.hpp with escape_json_string_simd
- Verify it's actually being used in dumps path and optimize further
- Hypothesis: 5–15% improvement on string-heavy data

### Approach C: Pre-sized output buffer with arena allocation
- Current implementation may grow the output buffer multiple times during serialization
- Pre-estimate output size: JSON output ≈ 1.0–1.2x the size of the Python object graph
- For dict: estimate = sum(key_len + value_estimate + 4) (for quotes, colon, comma)
- Allocate output buffer once, write directly without realloc
- Use the existing thread-local Arena for the output buffer
- Hypothesis: 5–10% improvement from eliminated realloc + better cache locality

### Approach D: Type-specialized serialization paths
- Instead of one generic serialize(PyObject*) that checks types on every call,
  create specialized paths:
  - serialize_dict_of_strings(dict) — when all values are strings
  - serialize_list_of_ints(list) — when all items are ints
  - serialize_homogeneous_list(list) — detect type uniformity on first element
- These specialized paths can be much tighter loops without type checks
- Hypothesis: 10–25% improvement on uniform data

### Approach E: Direct write to PyUnicode buffer
- Currently: write to std::string buffer → PyUnicode_FromStringAndSize(buffer)
- Instead: pre-allocate PyUnicode_New(estimated_size, 127) for ASCII
  and write directly into PyUnicode_1BYTE_DATA(result)
- Eliminates the final copy from C++ buffer to Python string
- For non-ASCII: fallback to two-pass (measure then write) or std::string buffer
- Hypothesis: 5–10% improvement from eliminated final copy

## Implementation Requirements
1. FIRST: profile python_dumps.cpp with Instruments/sample on the large dataset
2. Identify top 5 hotspots before implementing changes
3. Test with all output types: str, bytes, bytearray
4. Test with cycle detection enabled and disabled
5. Test with all type ordering policies
6. Verify SIMD string escaping is actually used in the dumps path
7. Isolate experiments in experiments/dumps_optimization/
8. Benchmark on all three datasets

## Success Criteria
- ≥30% improvement on dumps benchmarks (currently 7-8x slower than orjson)
- Target: ≤3x slower than orjson on large dataset
- No regression on loads benchmarks
- All 680 Python + 24 C++ tests pass
- Progress logged in docs/benchmarks/progress_log.md
```

---

## Prompt 8: NDJSON Parallel Parsing Optimization

**Priority:** MEDIUM — NDJSON loads is 7–44% slower than orjson
**Expected gain:** 10–30% on `loads_ndjson` throughput

```
You are optimizing the Strata JSON parser's NDJSON parsing for maximum performance.

## Context
- Project: /Users/borysbardysh/PycharmProjects/personal/PrimeLabFoundation/strata
- Branch: main-v2-0.1
- Key files: src/strata/bindings/python_ndjson.cpp, python_loads.cpp (parse_ndjson_text)
- Build: SKIP_TESTS=1 .venv/bin/pip install -e .
- Tests: make test (680 Python + 24 C++ tests must pass)
- Rules: read all files in .aiassistant/rules/ and follow them strictly

## Problem
NDJSON performance vs orjson:
- Small (0.96MB):  strata 7.33ms vs orjson 4.02ms — 82.5% slower
- Medium (6.25MB): strata 45.75ms vs orjson 31.73ms — 44.2% slower
- Large (43.85MB): strata 200.88ms vs orjson 187.54ms — 7.1% slower

The gap is largest on small/medium inputs where parallel parsing overhead dominates.
ADR-0005 documents the parallel NDJSON architecture.

## Task
Research and implement 1–3 approaches (Rule 12):

### Approach A: Reduce per-line Python object creation overhead
- Each NDJSON line creates a full Python object graph
- For 1000-line files, that's 1000 separate parse sessions
- Currently each line goes through the full PythonObjectBuilder setup/teardown
- Optimization: reuse the same PythonObjectBuilder across lines (reset instead of recreate)
- Share KeyCache, AdaptiveSizeEstimator, and Arena across all lines
- Hypothesis: 5–15% improvement on small/medium NDJSON

### Approach B: Batch list construction
- Currently: parse line → append to result list → parse next line → append...
- Each PyList_Append may trigger realloc
- Instead: pre-count lines (SIMD newline scan), PyList_New(count), PyList_SET_ITEM
- ADR-0006 mentions batch result collection — verify this is implemented for NDJSON
- Hypothesis: 2–5% improvement from eliminated list resizing

### Approach C: Optimize parallel parsing thread coordination
- Current parallel threshold: ≥2MB
- For small files (0.96MB), sequential parsing is used but has overhead
- Profile the sequential NDJSON path vs regular loads — is there unnecessary overhead?
- For parallel: reduce chunk coordination cost, use lock-free queue for results
- Investigate: are we creating/destroying threads per parse? Use persistent thread pool
- Hypothesis: 10–20% improvement on medium NDJSON from better parallelization

### Approach D: Fused NDJSON parsing (parse all lines in one pass)
- Instead of: find newline → parse line → find newline → parse line...
- Do: scan entire buffer with SIMD → collect all line boundaries → parse all lines
- This is partially implemented (experiments/ndjson_chunk_scan/)
- Verify the experiment results and integrate if positive
- Hypothesis: 5–10% improvement from better memory access patterns

## Implementation Requirements
1. Profile NDJSON parsing separately from JSON parsing
2. Measure per-line overhead: how much time is spent in line boundary detection vs parsing?
3. Test with various line counts: 10, 100, 1000, 10000 lines
4. Test with varied line sizes: 100B, 1KB, 10KB, 100KB per line
5. Verify parallel parsing thread safety with TSAN
6. Isolate experiments in experiments/ndjson_optimization/
7. Benchmark on all three NDJSON datasets

## Success Criteria
- ≥10% improvement on small/medium NDJSON benchmarks
- ≤1.5x slower than orjson on large NDJSON
- No regression on regular JSON loads benchmarks
- All 680 Python + 24 C++ tests pass
- Progress logged in docs/benchmarks/progress_log.md
```

---

## Prompt 9: Memory Layout Optimization — Cache-Friendly Object Construction

**Priority:** MEDIUM — cache misses are a hidden cost in the 70.63% "other" bucket
**Expected gain:** 3–8% on `loads` throughput

```
You are optimizing the Strata JSON parser's Python bindings for cache-friendly performance.

## Context
- Project: /Users/borysbardysh/PycharmProjects/personal/PrimeLabFoundation/strata
- Branch: main-v2-0.1
- Key files: src/strata/bindings/python_object_builder.h, python_loads.cpp
- Build: SKIP_TESTS=1 .venv/bin/pip install -e .
- Tests: make test (680 Python + 24 C++ tests must pass)
- Rules: read all files in .aiassistant/rules/ and follow them strictly

## Problem
During JSON parsing, PythonObjectBuilder accesses many data structures:
1. The parser stack (arena-backed vector of ContainerFrame)
2. The key cache (robin hood hash table)
3. The adaptive size estimator
4. Python dicts/lists being constructed
5. The input JSON buffer
6. The structural tape (if enabled)

These are scattered across memory. L1 cache is 64KB, L2 is 256KB–4MB.
For the large dataset (43.85MB), the working set far exceeds L1/L2.

## Task
Research and implement 1–3 approaches (Rule 12):

### Approach A: Optimize ContainerFrame layout for cache efficiency
- PythonObjectBuilder uses a stack of ContainerFrames to track nesting
- Each frame contains: PyObject* container, type, size, key_count, batch buffer
- Profile the frame size: is it a full cache line (64B)? Multiple lines?
- Pack the frame to fit in 1 cache line: move rarely-used fields out
- Use __attribute__((aligned(64))) for cache line alignment
- Hypothesis: 1–3% improvement from better cache utilization

### Approach B: Prefetch upcoming data
- When parsing an array of objects, we know the NEXT object starts soon
- Use __builtin_prefetch to load the next chunk of JSON into L1 cache
- When building a dict, prefetch the next key's hash table slot
- Prefetch the structural tape entries ahead of current position
- Hypothesis: 1–3% improvement from reduced cache miss stalls

### Approach C: Arena-allocated batch buffers
- The current batch dict insertion uses std::vector (heap-allocated)
- Replace with arena-allocated fixed-size arrays on the stack
- For 64-item batch: 64 × (PyObject* key + PyObject* value) = 1024 bytes = fits in stack
- Eliminate vector allocation/deallocation per dict
- Hypothesis: 0.5–1% improvement from eliminated batch buffer allocation

### Approach D: Restructure parse loop for instruction cache efficiency
- The main parse_sax_impl function is large with many branches
- Hot paths (string, number, dict start/end) should be close together in memory
- Cold paths (error handling, deep nesting) should be in separate functions
- Use __attribute__((hot)) / __attribute__((cold)) annotations
- Use [[likely]] / [[unlikely]] on branches
- Hypothesis: 1–2% improvement from better instruction cache behavior

## Implementation Requirements
1. Use perf stat (or macOS equivalent) to measure:
   - L1-dcache-load-misses
   - LLC-load-misses
   - branch-misses
   - instructions-per-cycle (IPC)
2. Compare IPC before and after changes — target IPC > 2.0
3. Use static_assert to verify frame sizes
4. Isolate experiments in experiments/cache_optimization/
5. Benchmark on all three datasets

## Success Criteria
- ≥2% improvement on large loads benchmark
- IPC improvement measurable with perf stat
- No regression on small benchmarks
- All 680 Python + 24 C++ tests pass
- Progress logged in docs/benchmarks/progress_log.md
```

---

## Prompt 10: PGO + LTO + AutoFDO Build Pipeline Optimization

**Priority:** MEDIUM — PGO showed 20.7% improvement on large loads, but isn't the default
**Expected gain:** 15–25% from making PGO the default build

```
You are optimizing the Strata JSON parser's build pipeline for maximum production performance.

## Context
- Project: /Users/borysbardysh/PycharmProjects/personal/PrimeLabFoundation/strata
- Branch: main-v2-0.1
- Key files: setup.py, Makefile, scripts/
- Build: SKIP_TESTS=1 .venv/bin/pip install -e .
- Tests: make test (680 Python + 24 C++ tests must pass)
- Rules: read all files in .aiassistant/rules/ and follow them strictly

## Problem
PGO (Profile-Guided Optimization) was shown to give 20.7% improvement on large JSON loads
(progress_log.md entry "Build Flag Audit"). However:
1. PGO is NOT the default build — users get non-PGO builds from pip install
2. The PGO workflow (make pgo-release) is separate from the standard build
3. Wheel distribution doesn't include PGO profiles
4. LTO (Link-Time Optimization) is enabled but thin LTO — full LTO may be better

## Task
Research and implement 1–3 approaches (Rule 12):

### Approach A: Integrate PGO into the standard pip install workflow
- Modify setup.py to support PGO_MODE env var (already partially done)
- Create a CI workflow that:
  1. Builds with PGO instrumentation
  2. Runs benchmark suite to generate profiles
  3. Rebuilds with PGO profiles applied
  4. Packages the PGO-optimized wheel
- Make this the default for `make release-wheel`
- Hypothesis: 15–20% improvement for wheel users (currently getting non-PGO builds)

### Approach B: Optimize PGO training data
- Current PGO profile is generated from benchmark suite
- Profile quality depends on training data matching real-world usage
- Create a comprehensive PGO training script that covers:
  - Small, medium, large JSON files
  - Deeply nested vs flat structures
  - String-heavy vs number-heavy data
  - NDJSON with various line sizes
  - Search queries with wildcards
  - dumps with various Python types
- Better training data → better branch prediction → faster code
- Hypothesis: 3–5% additional improvement over current PGO from better profiles

### Approach C: Full LTO instead of thin LTO
- Currently using -flto=thin (faster compile, slightly less optimization)
- Full LTO (-flto) enables cross-module inlining and whole-program optimization
- Particularly benefits the SAX handler → parser interface (Prompt 6 synergy)
- Measure compile time increase vs runtime improvement
- Hypothesis: 2–5% additional improvement from full LTO

### Approach D: AutoFDO with hardware counters
- Instead of instrumented PGO (adds runtime overhead during profiling),
  use AutoFDO which collects profiles from perf/Instruments on optimized builds
- This gives more accurate profiles (no instrumentation skew)
- Workflow: build optimized → run with perf record → convert to compiler profile → rebuild
- Hypothesis: 1–3% improvement over standard PGO from more accurate profiles

## Implementation Requirements
1. PGO must work on macOS (Apple Clang) and Linux (GCC, Clang)
2. PGO training script must complete in <60 seconds
3. Wheel build with PGO should be automated in CI
4. Document PGO build process in docs/
5. Keep SKIP_TESTS=1 escape hatch for development builds
6. Full LTO compile time must be <5 minutes
7. Benchmark all configurations on all three dataset sizes

## Success Criteria
- PGO wheels are the default for release
- ≥15% improvement in released wheel performance vs current non-PGO
- Build time increase <2x compared to non-PGO
- All 680 Python + 24 C++ tests pass
- Progress logged in docs/benchmarks/progress_log.md
```

---

## Prompt 11: Investigate orjson's Object Creation Strategy (Competitive Analysis)

**Priority:** RESEARCH — understanding the 2.77x gap
**Expected gain:** Insights that inform all other optimizations

```
You are researching how orjson achieves fast Python object creation for the Strata project.

## Context
- Project: /Users/borysbardysh/PycharmProjects/personal/PrimeLabFoundation/strata
- Branch: main-v2-0.1
- Rules: read all files in .aiassistant/rules/ and follow them strictly
- Strata is 2.77x slower than orjson on loads (136.66 MB/s vs 376.58 MB/s)
- 85.68% of Strata's runtime is Python C API calls

## Task
Research orjson's source code and document its Python object creation strategies.
This is RESEARCH ONLY — no code changes to Strata.

### Areas to investigate:

1. **Dict creation strategy**
   - Does orjson use _PyDict_NewPresized?
   - Does it use private CPython APIs for dict insertion?
   - Does it batch key-value insertions?
   - How does it handle hash computation?

2. **String creation strategy**
   - Does orjson intern/cache strings?
   - Does it use PyUnicode_New vs PyUnicode_FromStringAndSize?
   - Does it have special ASCII fast-paths?
   - How does it handle escaped strings?

3. **Memory management**
   - Does orjson use custom allocators?
   - Does it pool Python objects?
   - Does it manipulate GC state?
   - Does it use Rust's allocator (jemalloc/mimalloc) for Python objects?

4. **Parsing architecture**
   - Does orjson use SAX events or direct construction?
   - Does it have a two-pass strategy?
   - How does it handle nested structures?

5. **Build optimizations**
   - What compiler flags does orjson use?
   - Does it use PGO/LTO?
   - What Rust optimization level?

### Deliverable
Create a document at docs/research/orjson_analysis.md containing:
1. Key findings with code references (orjson version + file + line)
2. Techniques applicable to Strata (with difficulty estimates)
3. Techniques NOT applicable (and why)
4. Prioritized list of techniques to implement
5. Estimated performance impact of each technique

## Important
- orjson source: https://github.com/ijl/orjson (Rust)
- Focus on the deserialization path (equivalent to our loads)
- Don't just describe what they do — analyze WHY it's faster
- Compare with Strata's approach side-by-side
```

---

## Prompt 12: Implement Lazy/Selective Materialization for Large Documents

**Priority:** MEDIUM-LOW — high complexity, high potential payoff for specific use cases
**Expected gain:** 2–5x for partial document access patterns

```
You are implementing lazy materialization for the Strata JSON parser.

## Context
- Project: /Users/borysbardysh/PycharmProjects/personal/PrimeLabFoundation/strata
- Branch: main-v2-0.1
- Key files: src/strata/bindings/python_lazy_cursor.cpp, include/strata/json/json_tape.hpp
- Build: SKIP_TESTS=1 .venv/bin/pip install -e .
- Tests: make test (680 Python + 24 C++ tests must pass)
- Rules: read all files in .aiassistant/rules/ and follow them strictly
- ADR-0004 documents the LazyJsonCursor architecture

## Problem
loads() materializes the ENTIRE document into Python objects even if the caller only needs
a few fields. For a 43MB document with 881K dicts, creating all dicts costs ~250ms even if
the user only accesses 10 fields.

The lazy cursor (python_lazy_cursor.cpp) already exists but is a SEPARATE API (JsonCursor).
The opportunity is to make loads() ITSELF lazier for large documents.

## Task
Research and implement selective materialization (Rule 12):

### Approach A: Tape-backed lazy dict
- Parse to tape format (fast, no Python objects created)
- Return a custom Python type that LOOKS like a dict but materializes on access
- On __getitem__("field"): find field in tape, materialize only that value
- On iteration: materialize lazily as items are accessed
- On str()/repr(): materialize fully
- Must be transparent to isinstance(result, dict) checks
- Hypothesis: 5–50x faster for single-field access on large documents

### Approach B: Depth-limited materialization
- Materialize only the top N levels of nesting
- Deeper levels stay as tape-backed lazy objects
- Default depth: 2 (cover most common access patterns)
- Configuration: strata.loads(data, depth=2) — or auto-detect based on size
- Hypothesis: 2–5x faster for shallow access on deeply nested documents

### Approach C: Schema-guided materialization
- Accept a "schema" or field list: strata.loads(data, fields=["name", "email"])
- Only materialize specified fields, skip everything else
- Uses the existing search infrastructure for field matching
- Hypothesis: 10–100x faster for narrow field access on wide documents

## Implementation Requirements
1. CRITICAL: Lazy objects must be GC-safe (properly tracked)
2. CRITICAL: Lazy objects must support pickle, deepcopy, json.dumps
3. CRITICAL: isinstance(lazy_dict, dict) should return True (or document if not)
4. Keep the original JSON buffer alive while lazy objects exist
5. Thread-safety: lazy materialization must be safe under GIL
6. Isolate experiment in experiments/lazy_materialization/
7. Benchmark: full materialization time vs lazy + access 1 field vs lazy + access all fields

## Success Criteria
- ≥5x faster for single-field access on large documents
- No regression when all fields are accessed (full materialization path)
- Transparent API (looks like regular dict/list)
- All 680 Python + 24 C++ tests pass
- Progress logged in docs/benchmarks/progress_log.md
```

---

## Execution Order Recommendation

Based on expected impact and implementation difficulty:

| Order | Prompt | Expected Gain | Difficulty | Dependencies |
|-------|--------|---------------|------------|--------------|
| 1 | **Prompt 11** (orjson research) | Insights | Low | None |
| 2 | **Prompt 1** (dict insertion) | 5–15% | Medium | None |
| 3 | **Prompt 2** (dict_dealloc) | 3–7% | Medium | None |
| 4 | **Prompt 5** (GC + refcount) | 3–8% | High | Prompts 1,2 |
| 5 | **Prompt 7** (dumps) | 30–50% | Medium | None (independent) |
| 6 | **Prompt 4** (string optimization) | 3–8% | Medium | None |
| 7 | **Prompt 3** (int/float creation) | 2–5% | Low | None |
| 8 | **Prompt 10** (PGO pipeline) | 15–25% | Low | None |
| 9 | **Prompt 6** (SAX dispatch) | 2–4% | High | None |
| 10 | **Prompt 8** (NDJSON) | 10–30% | Medium | None |
| 11 | **Prompt 9** (cache layout) | 3–8% | Medium | None |
| 12 | **Prompt 12** (lazy materialization) | 2–5x partial | High | None |

**Combined potential:** If all optimizations land, theoretical improvement is 50–80% on loads,
which would bring strata from 2.77x to ~1.5x slower than orjson. With PGO on top, potentially ≤1.3x.

---

## Rules Compliance Checklist

Each prompt follows these project rules:
- [x] Rule 1 (C++ owns CPU work) — all optimizations are in C++ bindings
- [x] Rule 2 (no fallback deps) — no dependency on other JSON libraries
- [x] Rule 5 (public API stays small) — no API changes
- [x] Rule 6 (green tests gate) — all prompts require passing tests
- [x] Rule 8 (benchmarks required) — all prompts require benchmark verification
- [x] Rule 9 (memory first-class) — memory impact tracked
- [x] Rule 11 (output format) — prompts specify deliverable format
- [x] Rule 12 (research experiments) — 1–3 approaches per prompt with go/no-go
- [x] Rule 14 (build gates) — tests gate the build
- [x] Rule 17 (benchmark tracking) — progress log updates required
- [x] Rule 18 (agents contract) — Python stays one-line pass-through
