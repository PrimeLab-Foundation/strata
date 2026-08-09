---
name: bindings
description: CPython C-API binding layer — KeyCache and speculative key 
  matching, dumps fast paths, config-to-policy mapping (including a known 
  cycle_policy bug), GIL/GC posture, CPython internals in use, and dead code. 
  Load before touching src/strata/bindings/ or python/strata/.
---

# Python Bindings

**Framing:** this doc describes the *previous implementation* as the blueprint.
"Current state" says what the rebuild has actually built; everything after it is
blueprint until a milestone makes it real.

## Current state (after M6 — file I/O, NDJSON, cursor mode)

Added since M4: `python_document.cpp` (the `JsonCursor` type),
`python_ndjson.cpp` (eager and lazy NDJSON), `python_files.cpp` (`load`/`dump`
in file mode), and `load`/`dump` on the facade.

- **A cursor holds a `shared_ptr` share of the tree**, not a reference to a
  document object. That makes the document-outlives-cursor invariant hold by
  construction: nothing the caller drops can leave a cursor dangling, and there
  is no document type to expose. Navigating hands the same share to the child.
- **NDJSON parses line by line straight into Python objects.** Routing lines
  through the C++ DOM would have rounded integers through a double, so
  `next_line()` exists on the stream to hand the raw line to the same builder
  `loads` uses.
- **The NDJSON iterator is a real iterator type** owning the file text, so a
  malformed line raises when iteration reaches it rather than at load time —
  which is what "parses lazily line-by-line" has to mean.
- Static `PyTypeObject`s leave their tail zeroed, as CPython prescribes; the
  resulting `-Wmissing-field-initializers` is suppressed at those two
  declarations and nowhere wider.

## Current state (after M4 — loads, dumps, config)

Real on this branch: four files under `src/strata/bindings/` — `python_types.h`,
`python_module.cpp`, `python_loads.cpp`, `python_dumps.cpp` — and the facade
`python/strata/`: `__init__.py`, `serialize.py`, `config.py`. `strata.loads`,
`strata.dumps` and `strata.config` work; everything else in api.md is still to
come.

- `python_types.h` carries the shared plumbing — `PyRef` (owning reference),
  `GcPause`, `STRATA_CPP_TRY/CATCH` — plus the declarations the other two
  translation units share, so nothing is redeclared `extern` at a use site.
- `PythonObjectBuilder` (python_loads.cpp) is a duck-typed SAX handler, not a
  `JsonSaxHandler` subclass, so `parse_sax_inline` instantiates on the concrete
  type and inlines every callback. Integers arrive exact at any size:
  `on_big_int` hands the raw token to `PyLong_FromString`.
- UTF-8 is validated up front for `bytes` input only; a `str` is already valid
  Unicode, so its encoded form needs no second pass.
- `dumps` (python_dumps.cpp) is a plain recursive walk that borrows the core's
  escape table and float formatter. `bool` is tested before `int` because it is
  a subclass of one. The depth ceiling is `Py_GetRecursionLimit()`.
- **Cycle detection runs under every policy.** `"ignore"` still has to emit
  `null`, so it cannot skip tracking — the blueprint's untracked fast path for
  `ignore` recursed to the depth limit and raised instead.
- **`cycle_policy` is Warn from process start**, and `config.get`/`config.list`
  read the live policy variables rather than a cached map, so the reported
  setting and the actual behaviour cannot drift apart. That is the structural
  fix for the bug recorded below, not merely a corrected initial value.
- `return_type="cursor"` and `iterator=True` raised `NotImplementedError` at
  this milestone; both are real as of M6, above.

Since M5 the key cache, flat-vector array building, the thread-local dumps
buffer and to_chars integer formatting are in place. Still not built: speculative
key matching, presized dicts, and the dumps fast paths (3-tier dtoa, batch
same-schema dicts, homogeneous arrays) — which is where the remaining gap to
orjson lives (docs/benchmarking/SKILL.md).

Extension module `strata._strata` (`PyInit__strata` in `python_module.cpp`),
hand-written CPython C API — **no pybind11** by policy. Pure-Python facade in
`python/strata/`. Shared headers: `python_types.h` (`PyObjectPtr`,
`STRATA_CPP_TRY/CATCH`, `LIKELY/UNLIKELY`, `PyGcPause`), `python_convert.h`,
`python_document.h`. Conventions: include `python_convert.h` rather than extern
redeclarations; wrap every exported function in `STRATA_CPP_TRY/CATCH`.

## File map

| File                                      | Responsibility                                                                             |
| ----------------------------------------- | ------------------------------------------------------------------------------------------ |
| `python_module.cpp`                       | Init, method table, `load`/`dump`, config store                                            |
| `python_loads.cpp`                        | `loads`, NDJSON direct parse, `PythonObjectBuilder`, `KeyCache`                            |
| `python_dumps.cpp`                        | `dumps` + all serialization fast paths                                                     |
| `python_jsonpath.cpp`                     | JSONPath `compile` (previously `compile_path`)/`search`/`query`, SAX search, PyObject eval |
| `python_document.cpp` / `python_mmap.cpp` | `JsonDocument`/`JsonCursor` types, cursor-mode file load                                   |
| `python_ndjson.cpp`                       | `NdjsonStream` type                                                                        |
| `python_iterator.cpp`                     | `DictIterator`/`ListIterator`/`NdjsonFileIterator` (instance-only types)                   |

## loads-side techniques (the parsing win)

- **KeyCache** (per-builder): two tiers — (1) *cursor prediction*: same-schema
  records repeat keys in order, so the next expected key is one `memcmp`;
  (2) on miss, a flat open-addressing table: 256 slots, ≤192 entries (75% load,
  `efd00fd`), FNV-1a hashing 8 bytes at a time (`c0e3b5a`), 16-probe cap, overflow
  keys linearly scanned. Entries store the interned key `PyObject*` **plus its
  precomputed `Py_hash_t`**, fed to `_PyDict_SetItem_KnownHash`.
- **Speculative raw-byte key match**: `try_match_key` lets the parser skip the
  entire string path (SIMD scan + PyUnicode creation + cache lookup) when the
  predicted key matches raw input bytes.
- **Builder reuse:** `strata_loads` keeps a `static thread_local PythonObjectBuilder*`
  (deliberately leaked to dodge destructor-after-interpreter-shutdown), so the
  KeyCache persists across `loads()` calls per thread. NDJSON creates one builder
  per call and `reset()`s per line (reset keeps the KeyCache).
- **Other:** only object **keys** are interned; ASCII values get compact-ASCII
  `PyUnicode_New(len,127)` + memcpy; module-lifetime small-int cache 0..256;
  `_PyDict_NewPresized` sized by `depth_sizes_` (last object size per depth);
  arrays build into one flat vector then a single `PyList_New(n)` with
  ref-stealing `PyList_SET_ITEM`.

## dumps-side techniques

Thread-local `OutputBuffer g_serialize_buffer` (zero steady-state allocation);
size estimation via one-level sampling; `serialize_dict_t<bool Tracking>` /
`serialize_list_t<Tracking>` compile cycle checks out when policy is ignore;
homogeneous int/float/bool/str list fast paths (sample first ≤8 elements, bail
gracefully mid-list); `try_batch_list_of_dicts` (≤ `kMaxBatchKeys`=24) replays
pre-serialized key byte strings per same-schema element; NEON masked-load escape
check for ≤16-byte strings (pad-reads past the string — relies on CPython
allocation slack, ASan-hostile); `PyUnstable_Long_IsCompact` int extraction.

## config → policy mapping

`strata.config` is a process-global map in `python_module.cpp`; setters translate:

- `duplicate_key_policy` → `strata::set_duplicate_key_policy()` — consumed at
  parse time in `PythonObjectBuilder::push_value` (FirstWins → `PyDict_SetDefault`,
  LastWins → `_PyDict_SetItem_KnownHash`, Warn → `RuntimeWarning` + keep first,
  Error → parse failure). C++ default: `FirstWins`. **The policy variable is
  thread-local** — `config.set` only affects the calling thread.
- `cycle_policy` → file-static `g_cycle_policy` in `python_dumps.cpp`.
  **Previous-implementation bug — do not reproduce:** init seeded the config
  map with `"warn"` without calling the setter, while `g_cycle_policy` started
  as `Ignore` — reported and actual behavior disagreed until the first
  `config.set`. Target: seed both consistently to `"warn"` (contract:
  api.md §Config). (Also: a stale comment in `python_loads.cpp` called
  LastWins "the default" — it was not.)

## GIL / GC posture

The GIL is **never released** (no `Py_BEGIN_ALLOW_THREADS` anywhere) — all file
I/O and parsing hold it. Instead, `PyGcPause` (RAII `PyGC_Disable/Enable`) wraps
every bulk build/serialize. Thread safety comes from thread-local state
(serialize buffer, seen-stack, builders, file buffers, policies).

## CPython internals in use (version-sensitive)

`_PyDict_SetItem_KnownHash` (forward-declared to skip the `Py_BUILD_CORE` guard),
`_PyDict_NewPresized`, `PyUnstable_Long_IsCompact/CompactValue`,
`PyUnicode_IS_COMPACT_ASCII`. Audit these on every new CPython version.

## Build

`setup.py` compiles bindings + core + util into one extension:
`-std=c++20 -O3 -march=native` (dropped for universal2), optional
`STRATA_ENABLE_LTO=1`, `PGO_MODE=generate|use`. `TestGatedBuildExt` runs ctest
pre-build and pytest post-build (`SKIP_TESTS=1` escape hatch).

## Known sharp edges & dead code

- Previous implementation: `dump`/`search` filepaths were str-only at the C
  level and only `load`/`search` wrappers coerced `Path`. **Target: the facade
  coerces `Path` → `str` for all four file entry points** (`load`, `dump`,
  `search`, and cursor-mode `load`), per api.md. Text arguments parsed with
  `s#` accept both str and bytes.
- `JsonCursor.field()`/`at()` on a missing key/index raises `RuntimeError`
  ("field not found" / "index out of range") immediately — the `Py_RETURN_NONE`
  branches in `python_document.cpp` are dead code (the throwing C++ API never
  returns a null cursor).
- The previous implementation publicly exposed `parse_json` and `parse_ndjson`;
  both are dropped from the target API (the internal `parse_ndjson_direct`
  machinery still backs `load()`). Its `parse_ndjson(skip_errors=False)`
  returned a partial list instead of raising — do not reproduce that contract.
- Dead: `eval_step_jsonvalue` + `pyobject_to_json_value` + memo apparatus
  (~500 lines in `python_jsonpath.cpp`), `pyobj_results_to_list_steal`,
  `strata_set_cycle_policy` (complete but unregistered),
  `emit_duplicate_key_warnings` copies in `python_loads.cpp`/`python_jsonpath.cpp`,
  stale `#include json_mmap.hpp` in `python_jsonpath.cpp`, unused
  `#include dragonbox.hpp` in `python_dumps.cpp`.
- `NdjsonStream` is registered but not re-exported by the package; the three
  iterator types are reachable only as returned instances (no constructors).
