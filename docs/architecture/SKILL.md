---
name: architecture
description: C++ core architecture of strata — hybrid SAX parser, DOM, 
  serializers, NDJSON, mmap, error and memory model, contributor invariants, and
  the dead-code map. Load before modifying anything under include/strata/ or 
  src/strata/{json,search,util}.
---

# Core Architecture

**Framing:** this doc describes the *previous implementation* as the blueprint.
"Current state" below says what the rebuild has actually built; everything after
it is blueprint until a milestone makes it real.

## Current state (after M3 — serializer and dtoa)

Real on this branch: the value model (M1), the parser (M2) and the serializer
(M3). Files: `include/strata/json/{json_core,json_sax_handler,json_parser_inline,json_parse,json_serialize}.hpp`,
`include/strata/util/{scan,fast_parse,dtoa}.hpp`, and under `src/strata/`:
`json/{json_parse,json_serialize}.cpp`, `util/{scan,dtoa}.cpp`. Nothing is
exposed to Python yet — that starts at M4.

Serializer departures from the blueprint:

- **One converter, `util::format_double`**, built on `std::to_chars` for the
  shortest correctly-rounded digits and then laid out to match CPython's
  `repr(float)`: fixed while the decimal point sits in (-4, 16\], scientific
  outside it, always with a fraction so a float never reads back as an integer.
  Checked against Python over 152,640 doubles with no divergence. Vendoring
  reference Dragonbox is a *performance* question deferred to M5.
- **Non-finite policy lives in the serializer.** `format_double` is never handed
  a NaN or an infinity; `serialize_json` writes `null` for both. The blueprint
  checked in two places, one of them inside the converter.
- Object keys come out in insertion order. The blueprint's header claimed they
  were sorted through a `std::map`; they never were, and now the doc comment
  says what the code does.
- `serialize_json_to` clears its buffer but keeps the capacity, so repeated
  calls stop allocating.

Parse and serialize were checked together end to end: 6,000 generated documents
parsed, serialized and re-read, with every value matching stdlib `json` after
normalising ints to doubles and non-finite values to null.

Parser departures from the blueprint:

- **`on_big_int(std::string_view)` replaces `on_uint`.** An integer literal
  that does not fit `int64_t` is handed to the handler as its raw token, so a
  handler that can be exact (the Python builder) will be. The blueprint
  accumulated only the first 19 digits and returned that truncation as the
  value, which is how it mis-parsed 20-digit integers. `on_uint` was never
  emitted and the `size_hint` parameters were always 0; neither was rebuilt.
- **Conversion goes through `std::from_chars`** — correctly rounded, and immune
  to the process locale, unlike the `strtod` the blueprint fell back to. Values
  outside the double range saturate to ±inf or 0, matching stdlib `json`.
- **Scalar, and honestly named.** The scan primitives live in
  `util/scan.hpp` as `validate_utf8` / `skip_whitespace` / `find_next_escape`;
  vectorised bodies go behind those same names at M5. The blueprint called them
  `simd_*` while at least one of them was scalar all along.
- **No speculative-key hook and no inline array fast path** yet: both are
  performance work with benchmarks attached (M5), and the parser is correct
  first.
- Recursion is still the only depth limit, as the invariant below requires.

Strictness was verified by diffing accept/reject against stdlib `json` over a
generated corpus of ~1700 documents. The only disagreements are deliberate and
pinned by test: NaN/Infinity, escaped lone surrogates, CESU-8-encoded
surrogates, and a leading UTF-8 BOM.

## Value model as built (after M1)

`include/strata/json/json_core.hpp` is header-only: `FlatMap`, `Status`,
`Result<T>` and `JsonValue`, exactly as described under "Value model" below,
with three deliberate departures from the blueprint:

- **Dead API not rebuilt.** `Result::unwrap()`, `Result::value_or()` and
  `FlatMap::count()` had zero callers anywhere in the previous implementation.
  They are omitted until a caller exists; consumers check `ok()` and read or
  `std::move()` the `value` member, which is what every blueprint call site did.
- **`Result` initialises both members** (`status = Status::Ok`, `value{}`), so
  `Result<T>{Status::KeyNotFound}` carries a default-constructed `T` rather than
  an indeterminate one, and `ok()` is `constexpr`. The blueprint left `value`
  uninitialised.
- **`FlatMap::emplace()` appends without a duplicate check** — unchanged
  behaviour, now documented as a contract and pinned by test, because the
  parser's duplicate-key policy depends on `find()` returning the *first* match
  and on the caller doing the uniqueness check.

Core translation units are enumerated once, in `src/strata/core_sources.txt`,
which both CMakeLists.txt and setup.py read — so the ctest binaries, the fuzz
targets and the shipped extension are built from the same code. Core purity is
enforced mechanically by `tests/unit/test_core_purity.py`, which scans
`include/` and `src/strata/` minus `bindings/` and carries a positive control so
it cannot pass vacuously.

## Value model

`include/strata/json/json_core.hpp`: `JsonValue` is
`std::variant<nullptr_t, bool, double, std::string, Array, Object>` with
`Array = std::vector<JsonValue>` and `Object = FlatMap<std::string, JsonValue>`.
`FlatMap` is an insertion-ordered vector of pairs with linear-scan lookup —
chosen for cache locality on typical small objects. There is **no tape**: despite
"tape" language in some historical docs, the DOM is a plain variant tree.

Numbers in the C++ DOM are stored as `double` only — `DomBuilderHandler::on_int`
casts int64 to double, so ints > 2^53 lose precision **in the DOM path**. The
Python SAX path (`PythonObjectBuilder`) preserves exact ints. Keep this asymmetry
in mind when comparing `query()` (exact) with `CompiledPath.execute()` (DOM-based).

## Hybrid SAX design (the load-bearing decision)

Decision record (formerly ADR-0001, implemented in `8f468d2`):

- `json_sax_handler.hpp` — abstract `JsonSaxHandler` with 11 callbacks
  (`on_null/on_bool/on_int/on_uint/on_double/on_string/on_start_object/on_key/ on_end_object/on_start_array/on_end_array`), each returning `bool` (false aborts).
- `json_parser_inline.hpp` — `ParserInline<Handler>`, a templated recursive-descent
  parser. Instantiating with a concrete handler type devirtualizes and inlines
  every callback. `parse_sax_inline<Handler>(text, handler, validate_utf8=true)`
  is the entry point; `parse_json()` instantiates it with `DomBuilderHandler`,
  the Python bindings instantiate it with `PythonObjectBuilder`.
- Rationale: eliminates "double materialization" (C++ DOM → Python conversion);
  one parse builds the final representation directly.

### Parser hot-path details

- **Dispatch:** `parse_value()` switches on first byte; `parse_array()` has a
  tight inline element loop (commit `3f0d7fe`) with a small-number fast path and
  a 23-entry `kPow10` Clinger table.
- **Numbers:** `util::parse_number_unified` (`fast_parse.hpp`) — single scan;
  exact `int64_t` when integral, Clinger fast path for doubles (≤19 sig digits,
  |exp| ≤ 22), `strtod` fallback for hard cases. Rejects leading zeros.
  Not Eisel–Lemire (that was only ever built in the archived `new_strata` rewrite).
- **Strings:** SIMD scan (`find_next_escape_simd`) for quote/backslash/control;
  escape-free strings pass a **zero-copy `string_view` into the input buffer**
  (valid only during the callback). Slow path handles all escapes incl. UTF-16
  surrogate pairs; lone surrogates and raw control chars \< 0x20 rejected.
- **Whitespace:** `skip_ws()` scalar fast-exit then `skip_whitespace_fast`
  (NEON `vminvq_u8` / SSE2 movemask, 16 B per iteration). Added `67ef7fc`;
  redundant skips removed in `aa99eea`.
- **Speculative key matching** (`aa99eea`): SFINAE trait `has_try_match_key<Handler>`;
  the parser offers the raw bytes after `"` to the handler before any string
  scanning. Contract: `try_match_key` returns `key_len + 1` (consumed through the
  closing quote) or 0, and runs **before** escape scanning, so cached keys must be
  raw-byte-identical.
- **UTF-8 validation** runs once up front (`validate_utf8_simd`) unless disabled;
  the Python bindings pass `validate_utf8=false` because PyUnicode validates
  during string creation. Caveat for `bytes` input: that makes the parser the
  only validator — invalid UTF-8 must still surface as
  `ValueError("Invalid JSON")` (see api.md). (Note: the "SIMD" UTF-8 validator
  is actually scalar byte-classification in 16/32-byte chunks — correct, but
  not vectorized.)
- Pre-counting of array/object sizes was removed (`07f85a0` — the extra scan cost
  more than it saved); `count_object_keys`/`count_array_elements` in the header
  are dead. Size prediction now lives in the Python builder (`depth_sizes_`).

## Serialization — two independent paths

1. **C++ `serialize_json`** (`json_serialize.cpp`): simple recursive descent into
   a `std::string`. Numbers via `util::dragonbox_d2s` (see naming warning below).
   NaN/Inf → `null`. **No depth limit and no cycle detection** (a `JsonValue` tree
   cannot be cyclic, but deep input can overflow the stack). Header comment
   claiming "keys sorted via std::map" is stale — output is insertion-ordered.
2. **Python `dumps`** (`bindings/python_dumps.cpp`, the heavily optimized one):
   thread-local reusable `OutputBuffer` with `unsafe_*` writes after a
   covering `reserve()`; depth limit = `Py_GetRecursionLimit()`; GC paused for
   the whole call. The full fast-path inventory (3-tier dtoa, homogeneous
   arrays, batch dict serialization, tracking-template cycle checks) lives
   canonically in `docs/bindings/SKILL.md` §dumps-side techniques.

**dtoa naming warning:** `dragonbox.{hpp,cpp}` is actually a **Ryu**
implementation (its own comments say so); `ryu_dtoa.cpp` is actually a
`std::to_chars` wrapper (test-only); `fast_dtoa.cpp` is a third Ryu port with
**zero callers** (dead); `ryu_inline.hpp` is the live Python-dumps path.
A genuine Dragonbox swap exists unmerged in the archive (`main-v2` commit
`33d6835`) — see `docs/performance/SKILL.md`.

## NDJSON

- C++: `NdjsonStream` **borrows** a `string_view` (caller keeps data alive).
  SIMD newline find/count; `\r\n` and blank lines handled. `next()` signals
  end-of-stream by reusing `Status::KeyNotFound` — a deliberate quirk.
  `parse_all_fast()` pre-allocates via `count_newlines_simd` and parses in
  256-line chunks.
- Python: the throughput path bypasses the C++ DOM — `parse_ndjson_direct`
  scans the raw buffer and runs `parse_sax_inline<PythonObjectBuilder>` per line,
  reusing one builder (and its KeyCache) across all lines.

## mmap

`json_mmap.{hpp,cpp}` — POSIX-only RAII `MmapFile` (`MADV_SEQUENTIAL`).
`parse_json_file()` parses eagerly into the owning DOM (zero-copy is a stated
future optimization). **Footgun:** `parse_json_file_cursor()` returns a cursor
whose owning document is destroyed on return — it dangles; the Python binding
avoids it by returning a `(document, cursor)` tuple. Backs
`load(return_type="cursor")` only.

## Error model

Hybrid, no exceptions on hot paths: `Status` enum
(`Ok/TypeMismatch/KeyNotFound/IndexOutOfBounds/ParseError`) + POD `Result<T>`.
Parse failures carry no offset/message (known limitation). `JsonCursor` has a
parallel throwing convenience API used by bindings. SAX callbacks abort by
returning false. At the boundary, `STRATA_CPP_TRY/CATCH` maps C++ exceptions to
`RuntimeError`, invalid JSON to `ValueError`. Duplicate-key policy and parse
warnings are **thread-local**.

## Memory model

No arena in production (`arena_allocator.hpp` is experiments-only). DOM uses
plain owning containers; parser borrows the input for escape-free strings;
serialization reuses a thread-local buffer; the Python path adds interned keys
with precomputed hashes, a module-lifetime small-int cache (0..256),
`_PyDict_NewPresized` with depth-based prediction, and GC suspension.

## Contributor invariants

1. Recursion = stack depth: parser and serializers recurse; **no C++ depth limit
   anywhere**. C++ stress tests cap at depth 100 deliberately.
2. `JsonCursor` never owns — the `JsonDocument` must outlive every cursor.
3. `try_match_key` contract as above; runs before escape handling.
4. `KeyCache::get()` returns a borrowed `PyObject*` — full cache contract and
   constants: `docs/bindings/SKILL.md`.
5. String-view SAX callbacks are transient — copy before the buffer dies.
6. `unsafe_*` OutputBuffer ops require a prior covering `reserve()`.
7. `NdjsonStream` borrows its buffer; `Status::KeyNotFound` = EOF.
8. CPython-internal APIs in bindings (`_PyDict_SetItem_KnownHash`,
   `_PyDict_NewPresized`, `PyUnstable_Long_*`) are version-sensitive; the ≤16-byte
   NEON pad-read in `python_dumps.cpp` relies on CPython allocation slack and
   will trip ASan.
9. `duplicate_key_policy`, parse warnings, serialize buffer, seen-stack, and
   builders are thread-local (settings don't cross threads); `cycle_policy`
   (`g_cycle_policy` in `python_dumps.cpp`) is a plain process-global.

## Dead code map (do not build on these)

- `count_object_keys`/`count_array_elements` (parser pre-count, removed `07f85a0`)
- `fast_dtoa.{hpp,cpp}` (no callers), `ryu_dtoa` (test-only), duplicate
  `ryu_tables.inc` copies in include/ and src/
- `arena_allocator.hpp` (+ `ArenaStringBuilder`) — experiments only
- `fast_parse.hpp`: `parse_int_fast`, `scan_string_fast`, 309-entry `kPow10[]`
- `simd_string.hpp`/`simd_escape.cpp`: `try_copy_clean_string` (test-only),
  `string_needs_escape`, non-OutputBuffer `escape_json_string_simd` overloads
- `collect_newlines_simd` (experiment-only), `JsonSaxHandler::on_uint` (never
  emitted), `size_hint` params (always 0)
- In bindings: ~500 lines of pre-SAX JsonValue evaluators in `python_jsonpath.cpp`,
  `strata_set_cycle_policy` (unregistered), stray `emit_duplicate_key_warnings`
  copies — see `docs/bindings/SKILL.md`.
