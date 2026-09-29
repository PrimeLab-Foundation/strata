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

## Current state (after M10 — performance and streaming search)

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
buffer and to_chars integer formatting are in place. M10 added, on the dumps
side: the staged output buffer (raw stores, one string append per 8KB), the
homogeneous int/float/bool/str array runs, the micro-decimal dtoa tier, the
SWAR escape-scan tier, and the per-depth schema cache — thread-local, leased
across calls, keys owned. A re-entrant call finds that lease busy and takes a
*private* state, which releases its remembered keys as the call ends, while the
shared per-thread state stays immortal and holds its own for the life of the
thread. On the loads side: single-scan number conversion
with the exact-arithmetic double path, one-lookup dict inserts
(`PyDict_SetDefault` for FirstWins), **speculative key matching**
(`try_match_key` through the parser hook, per-depth predictions owning their
raw bytes, divergence-damped), and a per-thread builder lease that keeps the
KeyCache and predictions warm across calls and NDJSON lines.
`PythonObjectBuilder` itself moved to `python_builder.h`, shared with the
streaming-search capture sink so captured matches obey the duplicate-key
policy identically.

The post-release wave added, loads side (measured in
`docs/performance/SKILL.md`): **predictions are four ways per depth** —
probe-at-position with branch-on-divergence, so interleaved schemas (and
schemas sharing a leading tag key) each keep their own remembered shape
instead of thrashing one slot to retirement, mirroring the dumps schema
cache's four-way design; **presized dicts** via `_PyDict_NewPresized` and a
fixed per-depth last-size array (the deferred item, now measured: hints above
five members skip the resize cascade; a vector first cut cost +3% on nested
and was flattened to a bounded store). Still not built: a faster
general-case shortest-float on the dumps side (libc++ `to_chars` remains
~2× orjson's converter on full-precision doubles).

Extension module `strata._strata` (`PyInit__strata` in `python_module.cpp`),
hand-written CPython C API — **no pybind11** by policy. Pure-Python facade in
`python/strata/`. Shared headers: `python_types.h` (`PyObjectPtr`,
`STRATA_CPP_TRY/CATCH`, `LIKELY/UNLIKELY`, `PyGcPause`), `python_convert.h`,
`python_document.h`. Conventions: include `python_convert.h` rather than extern
redeclarations; wrap every exported function in `STRATA_CPP_TRY/CATCH`.

## File map

| File                                      | Responsibility                                                                                                                                                                             |
| ----------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| `python_module.cpp`                       | Init, method table, `load`/`dump`, config store                                                                                                                                            |
| `python_builder.h`                        | `PythonObjectBuilder` + `KeyCache` + key predictions — the one events→PyObject definition                                                                                                  |
| `python_loads.cpp`                        | `loads` entry points and the per-thread builder lease                                                                                                                                      |
| `python_dumps.cpp`                        | `dumps` + all serialization fast paths, byte-identical to main; `write_native`, the native tail and its writers compile in only under `STRATA_DUMPS_HOOK`                                  |
| `python_dumps_hook.cpp`                   | `strata._dumps_hook`'s serializer: `python_dumps.cpp` compiled with `STRATA_DUMPS_HOOK`, serving `dumps_native`/`dump_native` (no `default`) and `dumps_with_default` (`default` supplied) |
| `python_native_types.h/.cpp`              | Native type table (from `sys.modules`), pure leaves, conversions; the serializer's `datetime.h` TU — hook image only                                                                       |
| `python_numpy_twins.h/.cpp`               | The runtime proof of the numpy type numbers the `item()` twins admit — hook image only                                                                                                     |
| `python_parse_types.h/.cpp`               | `parse_types` (hook image only): option and registry, the reviving iterator, the four typed entry points, parsing through `_strata`'s public `loads`/`load`/`search`/`compile`             |
| `python_parse_types_walk.h/.cpp`          | `parse_types`'s revival walk (explicit stack), recognition and its lazily imported runtime — hook image only                                                                               |
| `python_dumps_output.h`                   | Output staging and the per-thread schema/staged-row lease                                                                                                                                  |
| `python_rawdict.h`                        | The runtime-proved raw dict-entry walk and the general-table compaction                                                                                                                    |
| `python_jsonpath.cpp`                     | JSONPath `compile` (previously `compile_path`)/`search`/`query`, SAX search, PyObject eval                                                                                                 |
| `python_document.cpp` / `python_mmap.cpp` | `JsonDocument`/`JsonCursor` types, cursor-mode file load                                                                                                                                   |
| `python_ndjson.cpp`                       | `NdjsonStream` type                                                                                                                                                                        |
| `python_iterator.cpp`                     | `DictIterator`/`ListIterator`/`NdjsonFileIterator` (instance-only types)                                                                                                                   |

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

### `parse_types`: served entirely by the hook image

Design record: `docs/architecture/native_types.md` ("Parse contract",
"Flag shape (M15b)"); contract: `docs/context/api.md` (`parse_types`). M15b
moved this off `_strata` — it was M15's `_strata`-only cold dispatch (a third
FASTCALL keyword, tested by interned identity); `docs/decisions.md`,
2026-09-29 ("parse") records the move. `_strata`'s `loads`/`load`/`search`/
`query`/`compile` are main's, unmodified: no third keyword, no cold function,
no macro. `python_parse_types.{h,cpp}` and `python_parse_types_walk.{h,cpp}`
build only into `strata._dumps_hook`.

- **Facade dispatch.** The Python facade tests `parse_types is False` by
  identity; on `False` it calls `_strata`'s entry with the caller's other
  arguments unchanged — one pointer compare, the whole per-call cost of the
  default path. Otherwise it imports `strata._dumps_hook` (first use only, not
  by `import strata`) and calls that image's `loads_typed`/`load_typed`/
  `search_typed`/`query_typed`, passing `parse_types` and the caller's other
  arguments.
- **The hook parses through `_strata`.** Each typed entry point calls
  `_strata`'s own public `loads`/`load`/`search`/`compile` to get the tree (or
  the match list), then runs the revival walk over that result — one parser,
  one `duplicate_key_policy` thread-local, because `_strata` is what parses.
  Linking a second parser into the hook was refused (native_types.md, option
  PB): `config.set` would never reach a parser the hook owned, since
  `duplicate_key_policy` is `_strata`'s thread-local. Each entry point's
  argument- and `parse_types`-validation error order matches what `_strata`'s
  own keyword parsing produced under M15, and stays test-pinned.
- **Option.** Unchanged from M15: `True`, or a dict copied with `PyDict_Copy`
  and validated into a private registry `name -> (type, init_names, required)`
  (`None, None` for an `Enum`; frozensets from `dataclasses.fields()` for a
  dataclass), so no user code can reach it. `Enum` membership is
  `PyType_IsSubtype` against `sys.modules["enum"].Enum` (no
  `__subclasscheck__`); a dataclass type is a type with `__dataclass_fields__`.
  The first valid call imports the `datetime_CAPI` capsule and `uuid.UUID`,
  held for the hook image's process lifetime (reset at its module init).
- **Recognition.** Only an exact `str` value of length 8–36 that is ASCII is
  handed to `strata::util::scan_temporal` (its bytes read in place, no UTF-8
  conversion); dates, times and date-times are built through the C API,
  `timezone.utc` for offset 0 and one cached fixed-offset `timezone` per
  minute otherwise; a UUID is `UUID(int=...)` by vectorcall. A constructor's
  `ValueError` leaves the `str`.
- **Walk.** Post-order and in place, over the tree `_strata` already built:
  list slots and existing keys' values. Registered types run user code, so
  every entry converted is held by a strong reference, a list's size is
  re-read at every step, and a result is written back only where the slot or
  key still holds the object read (`tests/py/native_types/test_parse_types.py`
  clears the containers mid-walk). Depth is bounded by `_strata`'s parser's
  1024-container cap, since the hook never builds a container of its own.
- **Iterators and search.** Lazy NDJSON and folder loads are wrapped by one
  `RevivingIterator` type in the hook (readied on first use, not at module
  init), which also walks a folder's files for a lazy `search`. A `.json`
  document read with `iterator=True` is revived whole, by `_strata`'s builder,
  and then given to `make_root_iterator`. `search_typed` always loads, revives
  and evaluates with `_strata`'s `query_object`; a scalar root, which
  `query_object` refuses, matches `$` alone (read off an empty-dict probe), as
  the default path answers. Folder discovery repeats `python_folder.cpp`'s
  error mapping rather than touching that file.

## dumps-side techniques

Thread-local `OutputBuffer g_serialize_buffer` (zero steady-state allocation);
size estimation via one-level sampling; `serialize_dict_t<bool Tracking>` /
`serialize_list_t<Tracking>` compile cycle checks out when policy is ignore;
homogeneous int/float/bool/str list fast paths (sample first ≤8 elements, bail
gracefully mid-list); `try_batch_list_of_dicts` (≤ `kMaxBatchKeys`=24) replays
pre-serialized key byte strings per same-schema element; NEON masked-load escape
check for ≤16-byte strings (pad-reads past the string — relies on CPython
allocation slack, ASan-hostile); `PyUnstable_Long_IsCompact` int extraction.

**Native types (M15b, docs/architecture/native_types.md, "Flag shape").**
`_strata`'s `python_dumps.cpp` is main's token stream, unchanged: `write_native`,
the per-kind writers, `NativeFrame`, `write_key_cold`, `push_open_cold` and the
numpy twins' call sites all live under `#ifdef STRATA_DUMPS_HOOK`, so a build of
`_strata` (without the macro) never compiles them and `write()`'s last branch is
exactly main's — no load, test or `Serializer` member was added to `_strata`.
`python_dumps_hook.cpp` compiles `python_dumps.cpp` a second time with
`STRATA_DUMPS_HOOK` defined, producing `dumps_native`/`dump_native` (no
`default`, so the unsupported-type arm falls through to the same
`TypeError("Object of type %s is not JSON serializable")` `_strata` raises) and
`dumps_with_default` (`default` supplied). Facade dispatch: `dumps`/`dump` test
`native is False`/`native is True` by identity
(`TypeError("native must be a bool, not %s")` otherwise) and, on `True`, import
`strata._dumps_hook` (first use only) and call its `dumps_native`/`dump_native`;
`dumps_with_default` has no `_strata` counterpart, so it always imports and
calls the hook, forwarding its `native` keyword (default `True`) on every call.
Under `native=False` the hook skips the native tail per call: its mode is a
`contextvars` variable, read by `native::classify` and `native::format_pure_leaf`
only while a process-wide count of opt-out walks is non-zero, so `classify`
answers `Kind::None` and the object falls through to `default`, as in main
`38eaa9f`'s hook (docs/architecture/native_types.md, "Opt-out").

Inside the hook image, `write_native` (`STRATA_COLD_FN`) tries a **pure leaf**
first — an exact `datetime`/`date`/`time` whose tzinfo is `None`
or exactly `datetime.timezone`, or an exact `UUID` read from its `int` slot —
formatted by `strata/util/temporal.hpp` with no latch, because it runs nothing
the user wrote and allocates nothing the collector tracks. Anything else is the
header's step 5: `latch()`, a strong reference on the object, then
`native::classify`, which resolves the type table from `sys.modules` (never
importing; a module imported later is found at the next lookup) and checks the
record's precedence: datetime, date, time (the exact types only — a subclass is
unsupported), UUID, Decimal, Enum, dataclass, set/frozenset, numpy. Per kind:
temporal and UUID text through the same formatters; `Decimal` as the raw text
of `str()` (`util::is_json_number`), non-finite as `null`; an Enum member under
a `Frame` on the member, its `value` followed in a loop bounded by
`depth_limit_`; a dataclass (field names cached per type, at most 1024 types, an
entry used only while the type's `__dataclass_fields__` is the same object at
the same length) and a set are written directly under a `Frame` on the object
itself, so cycles and depth behave as for a dict, latching before every field
read or iterator step; numpy through `item()`/`tolist()` and back into
`write()` (a result that is numpy again — a `longdouble`, on every platform —
goes to the old sink rather than looping). The last arm is the old sink: the
unsupported-type `TypeError` in `dumps_native`/`dump_native`, `default` in
`dumps_with_default`.

`python_files.cpp`/`python_folder.cpp` compile into the hook too, with
`STRATA_DUMPS_HOOK` set: their writer halves (`dump_to_file`, `dump_to_folder`
and the grouping they use, `file_is_ndjson`) link against the hook's
`dumps_to_python`, giving `dump_native` file and folder mode without a second
file writer; their reader halves sit under `#if !defined(STRATA_DUMPS_HOOK)`,
so `_strata`'s token stream of those files is unchanged. `dump_native` repeats
`strata_dump`'s dispatch (`split_by` → folder, a directory target without
`split_by` → `ValueError`) because `python_module.cpp` — and its dispatch —
stay `_strata`'s only; the dump contract tests run on both arms to pin that the
two dispatches agree.

Every `datetime` C-API use is in `python_native_types.cpp` (hook image only),
which takes the types from the `datetime_CAPI` capsule so the field macros only
ever read the C layout; `prepare_native_runtime()` interns the names at the
hook's module init. The tests live under `tests/unit/native_types/` and
`tests/py/native_types/`, outside the PGO training run for `_strata`.

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
`PyUnicode_IS_COMPACT_ASCII`, and the keys-table layout the serializer mirrors:
`PyDictKeysObject`'s prefix, both entry structs (`PyDictUnicodeEntry`, 16 bytes,
and `PyDictKeyEntry`, 24 bytes with the hash first) and the `DictKeysKind` tags
`DICT_KEYS_GENERAL`/`UNICODE`/`SPLIT`. Audit these on every new CPython version
— and audit `_PyDict_NewPresized` **together with** `DICT_KEYS_GENERAL`: the
presize is what makes strata's own records general-kind, and the serializer's
compaction is what reads them, so the two halves of that invariant only make
sense checked as a pair.

### The raw dict walk's runtime proof

`rawdict` (`python_rawdict.h`) reads a dict's entry array directly —
`PyDict_Next` re-validates and re-dispatches per call, profiled at 8% of a
record-heavy dump. The layout is CPython-internal, so it is mirrored minimally,
version-gated to 3.11–3.14 (`STRATA_RAW_DICT_WALK`), and **proved at runtime**
before first use: `probe_layout()` builds witness dicts and walks each one raw
and via `PyDict_Next`, comparing key and value pointers in lock step, requiring
both walks to end together and the live count to equal `PyDict_GET_SIZE`.

The proof is a **bitmask**, `{kProvedUnicode, kProvedGeneral}`, resolved in one
guarded static (`proved_layouts()`) with two accessors, so a future CPython
changing one layout cannot cost the other its fast path. `prepare_dumps_runtime`
forces it at import — the probe allocates GC-tracked dicts, and a collection
mid-walk would be a user-code step the serializer's four-step enumeration does
not allow.

Witnesses, and what each is for:

- unicode: 4 keys with a hole, 36 keys, and 200 keys — the last because both
  older witnesses sat at `1 << dk_log2_index_bytes == DK_SIZE`, so the
  index-width scaling `entry_base` computes was never exercised;
- general: a narrow exact-`str`-keyed table reached through a `str`-subclass
  key (no internal symbol, and independent of `_PyDict_NewPresized`'s current
  behaviour) with a hole punched *after* its last resize, a hole-free twin, and
  a 200-integer-key table at `dk_log2_size == 9`. Each also checks
  `me_hash == PyObject_Hash(key)`, the only check that catches a reordering
  inside the entry struct, and both compactions are compared against
  `PyDict_Next` on the same witness.

Any mismatch disables that kind's walk for the process and every caller keeps
`PyDict_Next`, with identical bytes — that fallback is the portable twin the
convention requires. Split tables (`ma_values != nullptr`, checked *first*) and
any other kind are always refused; the general test is `== DICT_KEYS_GENERAL`
exactly, never `!= DICT_KEYS_UNICODE`, because `DK_IS_UNICODE` is true for a
split table whose `me_value` fields are meaningless.

How a writer reaches the general half is a codegen decision, not a taste one.
Each writer keeps `rawdict::entry_array` inlined exactly as it was and hands
its `nullptr` edge to a `cold` **member** of `Serializer`
(`compact_general_exact`, `compact_general_holes`) that finds the lease's
scratch off `this`. Do not "simplify" that into a free accessor taking the
scratch as an argument: an argument has to be materialized before the call, and
the compiler schedules that load onto the unicode path, where it is dead — one
instruction per record in `write_record_fused` on both ISAs, two in
`write_mapping` on x86-64 (E26-P23 in docs/performance/experiment-ledger.md,
which also records the `[[unlikely]]` that keeps the merged `write_mapping`
epilogue out of the hot prologue). Every probe in the header carries
`STRATA_COLD_FN` for the same class of reason: on ELF that is what keeps
import-only code out of the writers' `.text`.

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
