# Strata — Reconstruction Prompts

Use these prompts in sequence to recreate the Strata project from scratch with a fresh AI agent.
Each prompt is self-contained and builds on the previous one.

---

## Prompt 1 — Project Charter & Rules

```
Create a Python extension library called "strata" — a high-performance JSON parser, serializer,
and JSONPath search engine for Python, built on a C++20 core.

The project must follow these non-negotiable rules (reference them as Rule 1–15 in all future work):

Rule 1  (North Star): Correctness first, then performance, then maintainability.

Rule 2  (Architecture boundary): The C++ core owns all CPU-heavy work — parsing, serialization,
        NDJSON streaming, search/query evaluation, memory arenas. The Python wrapper stays thin:
        argument normalization, error mapping, return-value formatting only.

Rule 3  (Core purity): Core code must not include Python.h. Structure: pure C++ core + Python
        adapter layer that may include Python.h. The adapter lives in src/strata/bindings/.

Rule 4  (No pybind11): The project must NOT depend on pybind11. Use the Python C API directly.
        If another binding approach is chosen it must be documented and justified in docs/.

Rule 5  (Allowed hot-path approaches): Any of SAX/Visitor parsing, token-tape/IR, or
        direct-to-Python single-pass construction are allowed if correctness and test/bench gates
        are met. An ADR in docs/adr/ is required for each architectural approach.

Rule 6  (Directory layout):
        - C++ production code:   src/strata/
        - C++ public headers:    include/strata/
        - Python package:        python/strata/
        - Binding/adapter code:  src/strata/bindings/
        - C++ tests:             tests/cpp/
        - Python tests:          tests/py/
        - Benchmarks:            benchmarks/
        - Docs (only):           docs/

Rule 7  (Naming):
        C++: namespace strata; types PascalCase; functions/methods snake_case;
             constants kPascalCase; filenames snake_case; #pragma once.
        Python: modules/functions snake_case; public classes PascalCase;
                internal helpers start with _.

Rule 8  (ADR requirement): Any change that introduces SAX/Visitor, a tape/IR format, changes
        ownership/lifetimes or error model, or changes public API semantics, must include a new
        ADR in docs/adr/.

Rule 9  (Error handling): Errors must include byte offset, reason/category, and optional context
        snippet. Hot paths avoid exceptions; prefer Result<T> or error codes. Python wrapper maps
        to stable exception types and messages.

Rule 10 (Memory): For hot-path changes explicitly evaluate allocation counts, ownership/lifetimes
        (RAII), O(n²) avoidance, and cache locality.

Rule 11 (Benchmarks mandatory): Any change touching parsing/serialization throughput, NDJSON
        streaming, search/query, allocators, or representation changes must run benchmarks.
        Maintain docs/benchmarks/progress_log.md (append-only, date+commit+baseline+delta).
        Regressions must be fixed or reverted.

Rule 12 (Automation): Automation logic goes in scripts/. Root Makefile is the user-facing
        interface and calls scripts. Scripts must be deterministic, fail-fast, and well-logged.

Rule 13 (Work summary format): Every work update must include: files changed, key design
        decisions (lifetimes + errors), tests run (exact commands), benchmarks run + deltas,
        risks + follow-ups.

Rule 14 (Python test gate): Python tests gate the build. They run automatically post-build via
        the custom build_ext in setup.py. SKIP_TESTS=1 is an escape hatch (strongly discouraged,
        not for CI/release).

Rule 15 (C++ test gate): C++ tests run by default pre-build via the same custom build_ext.
        SKIP_TESTS=1 skips both gates. make gate runs full compliance check.

Set up the repository structure, pyproject.toml, setup.py (with TestGatedBuildExt enforcing
Rules 14+15), and a root Makefile. Do not write any C++ implementation yet.
```

---

## Prompt 2 — Public Python API

```
Define the public Python API for strata. The Python package lives in python/strata/.
Do not implement the C++ backend yet — use Python stubs that raise NotImplementedError
so the API surface and signatures are locked in.

Required functions:

  strata.loads(source) -> object
    source: str | bytes | bytearray | memoryview
    Returns the parsed JSON value (dict, list, str, int, float, bool, None).

  strata.dumps(obj, *, return_type='str') -> str | bytes | bytearray
    return_type: 'str', 'bytes', or 'bytearray'

  strata.load(source, *, ndjson=False) -> object | list
    source: file path (str/Path) or file-like object.
    NDJSON detected via .ndjson extension or ndjson=True; returns list of records.

  strata.dump(target, obj, *, ndjson=False, return_type=None) -> int
    target: file path or file-like object. Returns lines written.

  strata.search(source, expression, *, ndjson=None, skip_errors=False,
                on_error='error', limit=0) -> list
    source: file path, JSON text (str/bytes), dict/list, or file-like object.
    expression: JSONPath string or CompiledPath.
    Returns list of matches (JSON) or list of {"line": int, "matches": list} (NDJSON).

  strata.query(data, expression) -> list
    data: dict or list. Alias of search for in-memory data.

  strata.compile_path(expression) -> CompiledPath
    Returns a reusable compiled JSONPath object.

  strata.set_duplicate_key_policy(policy)
    policy: 'first' (default), 'last', 'warn', 'error'

Also expose strata.__version__ and a strata.ndjson submodule with:
  parse_ndjson_file(path) -> NdjsonCursor
  parse_ndjson_text(text) -> NdjsonCursor

Write Python tests for the API surface (argument types, error messages, return types).
All tests must be in tests/py/.
```

---

## Prompt 3 — Official Architecture (Hybrid Direct-to-Python)

```
Implement the official Strata architecture: hybrid direct-to-Python parsing without polluting
the C++ core. Write the ADR first (docs/adr/ADR-0001-hybrid-sax-and-python-builder.md), then
implement.

Architecture:

  Layer 1 — C++ core (include/strata/, src/strata/json/, src/strata/search/, src/strata/util/)
    • Must NOT include Python.h or any Python runtime types.
    • Exposes parsing via SAX/Visitor events into a JsonSaxHandler interface.
    • JsonSaxHandler pure-virtual interface: on_null, on_bool, on_int64, on_uint64,
      on_double, on_string(std::string_view, bool is_key), on_start_object, on_end_object,
      on_start_array, on_end_array, on_error(size_t offset, std::string_view reason).
    • ParseSaxOptions: use_structural_tape (default false), collect_structural_tape (default false),
      duplicate_key_policy enum (FirstWins=default, LastWins, Warn, Error).
    • parse_json(std::string_view, ParseSaxOptions, JsonSaxHandler*) in json_parse.hpp.

  Layer 2 — Python adapter (src/strata/bindings/)
    • May include Python.h.
    • PythonObjectBuilder : JsonSaxHandler builds PyObject* tree during parsing (single-pass).
    • Thread-local state in python_loads.cpp: arena, key cache, object builder.
    • Key cache: intern common keys (id, name, type, etc.) using PyDict as a hash map.
    • python_loads.cpp exports: loads_impl(const char* data, Py_ssize_t len) -> PyObject*

  Layer 3 — Python wrapper (python/strata/)
    • Thin: argument normalization, error formatting, dispatch only.

Build requirements:
  - C++20 standard.
  - SIMD: detect ARM NEON / x86 SSE4.2 / AVX2 at compile time.
  - No third-party C++ libraries except header-only (dragonbox for float serialization).
  - setup.py must build using setuptools Extension (not CMake for the extension itself).
  - CMake is used only for the C++ test binary (tests/cpp/).

Write C++ unit tests in tests/cpp/ and Python parity tests in tests/py/.
Follow Rule 13 work summary format when done.
```

---

## Prompt 4 — SIMD Utilities

```
Implement the SIMD utility layer for strata in src/strata/util/ and include/strata/util/.

Required components:

  simd_utf8.(hpp/cpp)
    • SIMD-accelerated UTF-8 validation.
    • Platforms: ARM NEON (128-bit), x86 SSE4.2 (128-bit), x86 AVX2 (256-bit), scalar fallback.
    • bool validate_utf8(std::string_view) — returns false on first invalid byte sequence.
    • Report invalid byte offset via the error model (byte_offset + reason).

  simd_escape.(hpp/cpp)
    • SIMD-accelerated JSON string escaping for dumps.
    • Fast scan for characters that require escaping (\n \r \t \" \\ and control chars).
    • Write escaped output into a pre-allocated buffer.

  simd_newline.(hpp/cpp)
    • SIMD-accelerated newline scanning for NDJSON streaming.
    • Find next \n position in a byte buffer efficiently.

  fast_parse.(hpp/cpp)
    • Fast integer and float parsing (no locale, no exceptions).
    • For floats, integrate dragonbox (header-only) for shortest-round-trip serialization.

Each utility must have:
  - #pragma once in headers.
  - Scalar fallback that compiles everywhere (no #error for missing SIMD).
  - C++ unit tests in tests/cpp/.
  - No Python.h (Rule 3 — core purity).

Follow Rule 9 (error model), Rule 10 (allocation evaluation), Rule 13 (work summary).
```

---

## Prompt 5 — SAX Parser Core

```
Implement the SAX/Visitor JSON parser in src/strata/json/ (Rule 5, ADR-0001).

Files:
  include/strata/json/json_sax_handler.hpp  — JsonSaxHandler pure-virtual interface
  include/strata/json/json_parse.hpp        — parse_json() declaration + ParseSaxOptions
  src/strata/json/json_parse.cpp            — implementation

Parser requirements:
  • Iterative stack (not recursive) to handle deep nesting safely (up to depth 5000).
  • Single-pass SAX: emit events into JsonSaxHandler* during parse.
  • Error model: on_error(byte_offset, reason) — never throw, use Result<> internally.
  • String values: pass as std::string_view (valid only during callback — document this).
  • Numbers: detect int64 / uint64 / double; emit the correct on_* callback.
  • Unicode: validate UTF-8 (use simd_utf8); handle \uXXXX escape sequences including
    surrogate pairs (\uD800–\uDFFF as UTF-16 surrogate pairs → UTF-8).
  • ParseSaxOptions: duplicate_key_policy (FirstWins default, LastWins, Warn, Error).
  • Structural tape (optional, off by default): when collect_structural_tape=true, record
    all structural character positions for two-phase use.

Edge cases with required tests:
  • Unicode: \u0000 null, \uD83D\uDE00 emoji, invalid surrogates → error.
  • Numbers: INT64_MIN, INT64_MAX, UINT64_MAX, 1e308, -0.0, NaN/Inf → error.
  • Malformed: truncated input, unmatched braces, bare keys, trailing commas.
  • Deep nesting: 5000 levels must not stack-overflow.
  • Empty input, whitespace-only input.
  • Duplicate keys under each policy.

Write C++ tests in tests/cpp/ and Python parity tests in tests/py/.
Follow Rule 13.
```

---

## Prompt 6 — Python Object Builder (Direct-to-Python)

```
Implement PythonObjectBuilder in src/strata/bindings/python_object_builder.h.
This is the hot path: it implements JsonSaxHandler and constructs PyObject* directly
during SAX parsing (single-pass, no intermediate DOM).

Design requirements:
  • Header-only (template-heavy, performance-critical).
  • Object stack: std::vector<PyObject*> for nesting (dict/list).
  • Key handling:
    - Thread-local key cache (PyDict): intern string keys seen before.
    - Use _PyDict_SetItem_KnownHash for O(1) inserts with cached hash.
    - Common keys (id, name, type, status, etc.) cached permanently.
  • Dict batch insertion (DictBatch struct):
    - Accumulate up to kBatchSize (128) key/value pairs before flush.
    - DictBatch: keys[], values[], hashes[], size, needs_first_wins_fixup flag.
    - flush_current_dict_batch(): if needs_first_wins_fixup, GetItem+SetItem loop
      (FirstWins semantics); else pure SetItem batch (LastWins).
    - Flush at on_end_object() or when batch is full.
  • Deferred GC tracking:
    - Call PyObject_GC_UnTrack() immediately after PyDict_New() / PyList_New().
    - Call PyObject_GC_Track() in on_end_object() / on_end_array() after filling.
    - Controlled by STRATA_DEFERRED_GC_TRACK env var (default 1 = enabled).
  • Error model: on_error sets a Python exception and returns false.
  • Thread safety: all state is thread-local; no global mutable state.

Implement python_loads.cpp:
  • Thread-local: arena, key cache, object builder, parse context.
  • Entry point: loads() Python C API function accepting str/bytes/bytearray/memoryview.
  • Calls parse_json(data, opts, &builder) then returns builder.result().

Write Python tests covering:
  • All JSON types (null, bool, int, float, str, list, dict).
  • Unicode round-trips.
  • Duplicate key policies (first, last, warn, error).
  • Memory: no leaks on error paths (use tracemalloc).
  • Thread safety: concurrent loads() calls (threading.Thread x 8).

Env vars for A/B testing:
  STRATA_DEFERRED_GC_TRACK=0   — disable deferred GC tracking
  SKIP_TESTS=1                  — skip build-time test gates (discouraged)

Follow Rule 9 (error model), Rule 10 (allocation evaluation), Rule 13 (work summary).
```

---

## Prompt 7 — Serialization (dumps/dump)

```
Implement JSON serialization in src/strata/json/json_serialize.hpp and the Python binding
in src/strata/bindings/python_dumps.cpp.

Serializer requirements:
  • Recursive descent over Python objects (PyTypeObject dispatch).
  • Types: dict (PyDict), list/tuple (PyList/PyTuple), str (PyUnicode),
    int (PyLong), float (PyFloat), bool (PyBool), None (Py_None).
  • Strings: use simd_escape for SIMD-accelerated JSON escaping.
  • Floats: use dragonbox for shortest-round-trip representation (no trailing zeros,
    always round-trips through float()).
  • Integers: handle arbitrary Python ints (PyLong can exceed int64 — serialize as
    decimal string for large values, or raise OverflowError if not representable).
  • Output buffer: resizable byte buffer; avoid realloc on small objects.
  • return_type kwarg: 'str' → PyUnicode, 'bytes' → PyBytes, 'bytearray' → PyByteArray.

dump() implementation (python/strata/serialize.py thin wrapper):
  • Accepts file path (str/Path) or file-like object.
  • Detects NDJSON via .ndjson extension or ndjson=True.
  • For NDJSON: iterates list, dumps() each element, writes \n between records.
  • Returns int: lines written.

Write tests:
  • Round-trip: loads(dumps(obj)) == obj for all types.
  • Float edge cases: 1.0, -0.0, 1e100, smallest subnormal.
  • Unicode: BMP, emoji, null byte.
  • OverflowError for values outside JSON representable range.
  • return_type variants.

Follow Rule 13.
```

---

## Prompt 8 — NDJSON Streaming

```
Implement NDJSON streaming in src/strata/json/ndjson_stream.hpp and
src/strata/bindings/python_ndjson.cpp.

C++ core (ndjson_stream.hpp):
  • NdjsonStream: iterates a buffer or file, splitting on \n (SIMD-accelerated via simd_newline).
  • Yields std::string_view per non-empty line.
  • Handles \r\n line endings.
  • Skips blank lines.
  • Reports line number in error model.

Python adapter:
  • NdjsonCursor (Python type): lazy, yields parsed Python objects one line at a time.
  • parse_ndjson_file(path) -> NdjsonCursor.
  • parse_ndjson_text(text) -> NdjsonCursor  (text: str | bytes).
  • on_error kwarg: 'error' (default), 'warn', 'skip'.
  • __iter__ / __next__ protocol.

strata.load() with ndjson=True or .ndjson extension:
  • Reads all lines, returns list of parsed objects.
  • skip_errors=True: skips malformed lines silently.

strata.dump() with ndjson=True:
  • Input must be a list. Each element serialized on its own line.

Write ADR: docs/adr/ADR-0005-parallel-ndjson-processing.md
  (document streaming design, line-number error reporting, cursor protocol).

Write tests:
  • Empty file, file with only blank lines.
  • Mixed \n and \r\n.
  • Malformed line in middle: error/warn/skip modes.
  • Very long lines (>1 MB).
  • Unicode content per line.
  • Thread safety: multiple NdjsonCursor instances concurrently.

Follow Rule 13.
```

---

## Prompt 9 — JSONPath Search & Query

```
Implement JSONPath search in src/strata/search/ and src/strata/bindings/python_search.cpp.

C++ core (no Python.h):
  include/strata/search/jsonpath_compile.hpp  — CompiledPath type, compile(expression)
  include/strata/search/jsonpath_eval.hpp     — evaluate(CompiledPath, JsonValue&) -> results

Supported JSONPath subset (required):
  $               root
  .key            child key
  [*]             all array elements
  [n]             array index (negative allowed)
  [n:m]           array slice
  ..key           recursive descent
  [?(@.key op v)] filter expressions (op: == != < > <= >=, string/number literals)
  *               wildcard

CompiledPath: opaque C++ type, Python-exposed as strata.CompiledPath.
compile_path(expression) -> CompiledPath — cache compiled paths internally.

search_from_json_buffer (python_search.cpp):
  • Parses JSON with parse_json(data, opts) where opts.use_structural_tape = false
    (explicit — do NOT rely on default, as collect_structural_tape=false is the gate).
  • Builds DOM (strata::JsonValue), evaluates CompiledPath.
  • Returns PyList of matches.
  • limit kwarg: stop after N matches (0 = unlimited).

strata.search() Python dispatcher (python/strata/search.py):
  • Detects source type: file path → mmap or read; str/bytes → parse directly;
    dict/list → skip parse, evaluate on existing Python object via query path;
    NdjsonCursor → per-line search.
  • Returns list of matches for JSON; list of {"line": N, "matches": [...]} for NDJSON.

Write ADR: docs/adr/ADR-0006-search-optimization.md.

Write tests:
  • All JSONPath operators listed above.
  • Recursive descent on deeply nested structures.
  • Filter expressions with string and number comparisons.
  • limit kwarg: verify truncation.
  • CompiledPath reuse across multiple calls.
  • NDJSON search: per-line results format.
  • Error on invalid JSONPath expression.

Follow Rule 13.
```

---

## Prompt 10 — Build System, LTO, and PGO

```
Finalize the build system in setup.py and scripts/.

setup.py requirements:
  • Custom TestGatedBuildExt(build_ext) enforcing Rules 14+15:
    - Phase 1 pre-build: CMake configure + build + ctest for C++ tests.
    - Phase 2 actual extension build.
    - Phase 3 post-build: pytest tests/py/.
    - SKIP_TESTS=1 env var: skips all gates (prints loud warning, sets non-release marker).
  • C++20 standard (-std=c++20).
  • SIMD flags: -msse4.2 on x86; ARM NEON detected automatically.
  • Optimization: -O3 -DNDEBUG by default.
  • LTO (Clang thin LTO):
    - STRATA_ENABLE_LTO env var (default '1' = enabled).
    - Flags: -flto=thin (compile + link).
    - Benchmarks show +5-6% across all dataset sizes.
  • PGO support:
    - PGO_MODE=generate: -fprofile-instr-generate flags.
    - PGO_MODE=use: -fprofile-instr-use=$STRATA_PGO_PROFILE flags + LTO.
    - STRATA_PGO_PROFILE: path to .profdata (default build/pgo/strata.profdata).
  • Parallel compilation: multiprocessing.cpu_count()-1 workers.

scripts/pgo_build.sh:
  • Phase 1: PGO_MODE=generate build → run benchmarks (30 iters, large dataset) →
    llvm-profdata merge → build/pgo/strata.profdata.
  • Phase 2: PGO_MODE=use + STRATA_ENABLE_LTO=1 build.
  • Benchmarks show PGO+LTO: +14.8% large, +14.9% medium, +3.8% small vs LTO-only.

Makefile targets:
  make all          — run all tests (default)
  make install      — install with test gates
  make install-skip-tests  — SKIP_TESTS=1 (discouraged)
  make gate         — full compliance check (build + C++ coverage + Python coverage)
  make bench-small / bench-medium / bench-large
  make pgo          — run full PGO workflow
  make clean

Env vars summary:
  SKIP_TESTS=1                  — skip build-time test gates
  STRATA_ENABLE_LTO=1           — enable Clang thin LTO (default: 1)
  PGO_MODE=generate|use         — PGO build phase
  STRATA_PGO_PROFILE=<path>     — profdata path for PGO use phase
  STRATA_DEFERRED_GC_TRACK=0    — disable deferred GC tracking (default: 1)
  PGO_DATASET_SIZE=small|medium|large  — dataset for PGO profiling run
  PGO_BENCH_REPEAT=N            — iterations for PGO profiling benchmarks

Follow Rule 12 (automation), Rule 13 (work summary).
```

---

## Prompt 11 — Benchmarks & Progress Log

```
Implement the benchmark infrastructure and document the initial baseline in
docs/benchmarks/progress_log.md.

Benchmark suite (benchmarks/bench_suite.py):
  • Categories: loads_json, loads_ndjson, dumps_json, search_query.
  • Competitors: orjson, ujson, msgspec, json (stdlib).
  • Metrics per run: min, median, p95 (milliseconds), throughput (MB/s).
  • Trim: discard 2 fastest and 2 slowest iterations.
  • Output: markdown table saved to docs/benchmarks/bench_results_{size}.md.
  • Datasets: small (~1 MB), medium (~6.6 MB), large (~46 MB) in
    benchmarks/data/generated/{small,medium,large}/users.json + users.ndjson.

Dataset generator (benchmarks/generate_data.py):
  • Generates realistic nested JSON: users → orders → items → price/status.
  • Deterministic (fixed random seed).
  • Sizes controlled by --count argument.

Makefile targets:
  make bench-data           — generate all datasets
  make bench-small          — run suite on small
  make bench-medium         — run suite on medium
  make bench-large          — run suite on large
  make bench-all            — all three

progress_log.md format (append-only per Rule 11):
  ## <Title> — <date>
  **Branch:** `<branch>`
  **Commits:** `<hash>` (<description>)
  **Status:** IMPLEMENTED / REVERTED / NEUTRAL

  <narrative>

  | Dataset | baseline | post | Δ |
  |---------|----------|------|---|
  | small   | X ms     | Y ms | +Z% |

  ### Testing
  - ✅ N Python tests pass
  - ✅ N C++ tests pass

Document these implemented optimizations in progress_log.md:
  1. Structural tape OFF by default (+22% large loads)
  2. LTO default: +1.9%/+4.4%/+5.2% small/medium/large
  3. Dict batch flush (FirstWins+LastWins): +locality, same correctness
  4. PGO+LTO: +3.8%/+14.9%/+14.8% small/medium/large vs LTO-only
  5. Deferred GC tracking: +2.4-2.7% medium/large

Follow Rule 11, Rule 13.
```

---

## Prompt 12 — Final Compliance Check

```
Run the full compliance gate for strata and confirm all rules are satisfied.

Execute in order:
  1. make gate
     (Runs: build with test gates → C++ coverage → Python coverage)

  2. pytest tests/py/ -q
     (Should pass all N tests, currently 680)

  3. ctest --test-dir build --output-on-failure
     (Should pass all N C++ tests, currently 24)

  4. Verify progress_log.md exists and has all optimization entries.

  5. Verify docs/adr/ contains:
     - ADR-0001 (hybrid SAX + Python builder)
     - ADR-0002 (lazy string unescape)
     - ADR-0003 (token tape format)
     - ADR-0004 (lazy parsing cursor)
     - ADR-0005 (parallel NDJSON)
     - ADR-0006 (search optimization)

  6. Verify .junie/guidelines.md exists (Rules 1–15 reference document for agents).

  7. Run benchmark sanity check:
       SKIP_TESTS=1 .venv/bin/pip install -e .
       .venv/bin/python -c "
       import strata, time, statistics
       data = open('benchmarks/data/generated/large/users.json','rb').read()
       for _ in range(3): strata.loads(data)
       times = []; [times.append((lambda: strata.loads(data))()) for _ in range(10)]
       # min should be <300ms on Apple M-series, <500ms on Intel x86
       print('OK — strata.loads large:', min(times))
       "

  8. Confirm active build is PGO+LTO:
       PGO_MODE=use STRATA_ENABLE_LTO=1 STRATA_PGO_PROFILE=build/pgo/strata.profdata \
       SKIP_TESTS=1 .venv/bin/pip install -e .

Report Rule compliance status for each rule (1–15): PASS / FAIL / N/A.
Follow Rule 13 work summary format.
```

---

## Key Context for All Prompts

### Performance benchmarks (Apple M-series, Python 3.14, 2026-02-20)

| Build | small (1 MB) | medium (6.6 MB) | large (46 MB) |
|-------|-------------|-----------------|---------------|
| LTO-only | 10.54 ms | 43.18 ms | 311.64 ms |
| PGO+LTO  | 10.14 ms | 36.75 ms | 265.65 ms |

### Critical Python 3.14 / GC facts to keep in mind

- `gc.disable()` does NOT prevent gen0 tracking — `_PyObject_GC_TRACK` fires on every
  `PyDict_New` / `PyList_New` regardless. Only `PyObject_GC_UnTrack()` removes objects.
- Python 3.14 uses incremental GC (PEP 794): gen0 sweeps cost ~0ms even for 968K objects.
- Common JSON keys (`id`, `name`, `type`) are immortal in Python 3.14 — INCREF/DECREF on
  them are no-ops.
- `_PyDict_SetItem_KnownHash` is the fastest dict insertion path (bypasses hash computation).

### Build commands

```bash
# Fast dev build (skip tests)
SKIP_TESTS=1 .venv/bin/pip install -e .

# Full build with test gates (Rules 14+15)
.venv/bin/pip install -e .

# PGO+LTO build (best performance)
PGO_DATASET_SIZE=large bash scripts/pgo_build.sh

# Reinstall PGO+LTO after profdata exists
PGO_MODE=use STRATA_ENABLE_LTO=1 STRATA_PGO_PROFILE=build/pgo/strata.profdata \
SKIP_TESTS=1 .venv/bin/pip install -e .
```
