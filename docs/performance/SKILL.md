---
name: performance
description: The optimization playbook — every technique that won (with commits 
  and measured effect), every negative result (so failed ideas are not retried),
  and ranked salvageable work from unmerged branches. Load before attempting any
  performance work.
---

# Performance Playbook

Rule of engagement: measure before and after (`make bench-small` minimum),
trust PGO-build interleaved comparisons over single runs, and record go/no-go
conclusions here. The negative-results table below exists so failed experiments
are not unknowingly repeated.

## What won (chronological — all on the pre-reset main line, `backup/pre-reset-main`, tip `c0e3b5a`)

| Commit               | Technique                                                                                                                  | Effect / rationale                                                                |
| -------------------- | -------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------- |
| `8f468d2`            | Hybrid SAX architecture: parser emits events, `PythonObjectBuilder` builds PyObjects in one pass                           | Eliminated double materialization (C++ DOM → Python); the parsing-lead foundation |
| `d8985af`            | Devirtualization: templated `ParserInline<Handler>`                                                                        | +13% critical path (measured on the 0.1 campaign as static dispatch)              |
| `da3fe64`            | Instance-level key cache + SIMD escape checks                                                                              | Key `PyObject` reuse across records                                               |
| `b66049d`..`22c9816` | Memo-based cycle detection → open-frame scan / thread-local stack                                                          | Hash-set upkeep on every container dominated; cycles are rare                     |
| `20ad3a0`/`bcf6dd1`  | NDJSON batch parsing straight to PyObjects                                                                                 | Bypasses C++ DOM per line                                                         |
| `1ad8828`            | SAX streaming JSONPath search                                                                                              | ~9–10× vs orjson+jsonpath-ng; obsoleted `mem_eff` mode in one day                 |
| `512403f`            | PGO infrastructure                                                                                                         | ~+15% medium/large (0.1-branch measurement `22015c8`); LTO alone +5–6%            |
| `95fd416`            | `_PyDict_NewPresized`; homogeneous float-array dumps path                                                                  | Kills dict resize cascades                                                        |
| `67ef7fc`            | SIMD whitespace skip; int32 serialization fast path; string-array dumps path                                               |                                                                                   |
| `aa99eea`            | Speculative key matching (`try_match_key`) + fewer whitespace skips                                                        | Skips ~10.5k SIMD scans per 500 same-schema records                               |
| `3f0d7fe`            | Unified single-scan number parsing; tight array loops; inlined Ryu (`ryu_inline.hpp`)                                      | Removed int-then-double double-scan and cross-TU dtoa calls                       |
| `efd00fd`/`c0e3b5a`  | KeyCache: 75% load cap; FNV-1a 8-byte hashing; precomputed `Py_hash_t` + `_PyDict_SetItem_KnownHash`                       | Final parsing-lead commits                                                        |
| dumps side           | 3-tier dtoa (int-valued / 2-decimal / Ryu), batch same-schema dict serialization, `serialize_*_t<Tracking>` template split | Flipped dumps from behind-orjson (Mar 14) to ahead (Mar 17)                       |

## What won in the rebuild (measured, small tier, interleaved)

All measured with `make bench-small` before and after, on the same host.

| Change                                                                  | Effect (median)                               |
| ----------------------------------------------------------------------- | --------------------------------------------- |
| Escape clean runs in bulk instead of byte-at-a-time `push_back`         | part of the dumps 8-24% below                 |
| Integers via `std::to_chars` into a stack buffer, not `std::to_string`  | (same batch; `to_string` allocated per int)   |
| Thread-local dumps output buffer, reused across calls                   | dumps -8% to -24% across all datasets         |
| Arrays built into a flat vector, then one `PyList_New(n)` + `SET_ITEM`  | loads -13% to -22% across all datasets        |
| Per-document key cache (interned key reuse across records)              | (same batch)                                  |
| PGO + ThinLTO (`make pgo`), IR-level instrumentation                    | loads -8% to -12%, dumps -11% to -25%         |
| `to_chars(fixed)` directly instead of scientific-then-relayout (dtoa)   | format_double 77.8 -> 41.2 ns/value           |
| Homogeneous int/float/bool array runs, batched into a stack chunk       | dumps wide_arrays -40%; bool lists 5.2->1.4x  |
| Escape-bearing strings built through a stack chunk, one append/chunkful | escaped-string dumps 4.31x -> 2.76x vs orjson |
| SIMD (NEON/SSE2) escape scan with a checked scalar twin                 | long clean strings 2.1x -> 1.5x vs orjson     |
| Per-depth prepared-key schema cache for same-shape objects              | dumps users -20%, flat -30%, nested -30%      |

After M10's dumps work the gap to orjson is ~1.9-2.4x on `dumps` (was
2.4-4.0x). What is still not rebuilt: a custom shortest-float converter
(`format_double` is now within 2 ns of libc++'s `to_chars`, which is itself
~40 ns/value against orjson's ~17 ns), the homogeneous *string* array path,
and the loads-side speculative key matching. The single largest remaining
item is not a micro-optimization at all: the **SAX streaming JSONPath
evaluator**, still unbuilt, which is why `search` costs a full parse plus a
query (see the standings in docs/benchmarking/SKILL.md).

### PGO, measured (M9)

Go. `make pgo` was A/B'd against a plain `-O3 -march=native` build on the same
host, 3 datasets x 15 repeats, warmup 3, and **run in both orders** because the
PGO build being measured first would otherwise flatter it on a warming machine.
The two orderings agree to within 1 percentage point, and the two plain runs to
within 0.6%:

| Category | Dataset     | plain (ms) | PGO+LTO (ms) | delta (order A / order B) |
| -------- | ----------- | ---------- | ------------ | ------------------------- |
| loads    | users.json  | 13.207     | 11.802       | -10.6% / -10.2%           |
| loads    | flat.json   | 1.385      | 1.270        | -8.3% / -9.0%             |
| loads    | nested.json | 1.104      | 0.968        | -12.3% / -12.0%           |
| dumps    | users.json  | 7.895      | 6.353        | -19.5% / -20.4%           |
| dumps    | flat.json   | 0.807      | 0.715        | -11.4% / -12.4%           |
| dumps    | nested.json | 0.724      | 0.545        | -24.7% / -25.1%           |

Absolute numbers are higher than `docs/benchmarks/bench_results_small.md`
because these ran straight after repeated full rebuilds; only the within-run
comparison is meaningful, which is the point of running both orders.

### Measured non-wins in the rebuild

| Idea                                                                         | Verdict                                                                                                                                                                           |
| ---------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| KeyCache slots storing the key bytes + digest, to keep probes out of CPython | Reverted: no measurable change (loads users 12.55 vs 12.51 ms, flat 1.293 vs 1.301 ms). `PyUnicode_AsUTF8AndSize` on a compact-ASCII key already just returns the inline pointer. |
| Single-slot (most-recent) prepared-key schema cache                          | Superseded by one slot *per depth*: a single slot thrashes on nested documents (user -> orders -> items), and users.json saw no gain until the cache was keyed by depth.          |
| Preparing a schema on first sighting                                         | Superseded by preparing on the *second*: preparing costs about what writing costs, so documents of one-off shapes paid for a cache they never used (mixed.json +20%).             |
| Module-lifetime small-int cache (0..256)                                     | Not built: CPython already caches small ints internally, so `PyLong_FromLongLong` returns the shared object. The blueprint's cache duplicated a cache that already exists.        |
| Presized dicts via `_PyDict_NewPresized`                                     | Not built: the API is CPython-internal and the architecture notes already flag it as version-sensitive. Deferred until it can be measured against a portable alternative.         |

## Negative results — do not retry without new evidence

| Idea                                                                             | Where tried                                      | Verdict                                                                                                                                                                 |
| -------------------------------------------------------------------------------- | ------------------------------------------------ | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| simdjson-style structural tape                                                   | 0.1 branch (`e8443f3`; default OFF in `84fdf8c`) | **Tape OFF was +22% on large loads** — 46MB JSON → ~110MB tape → L3 thrash on Apple Silicon                                                                             |
| Array/object pre-counting for pre-sizing                                         | main (`67ef7fc` → reverted `07f85a0` same day)   | Pre-scan cost more than resize savings; depth-based prediction won                                                                                                      |
| `mem_eff` JSONPath mode                                                          | main (`cbc9162` → removed `5c264ca`)             | SAX search made it redundant within a day                                                                                                                               |
| mmap I/O as the primary file path + NDJSON bindings                              | removed `a41eea0`                                | Separate slower code path; file `load` was worst-in-class at the time                                                                                                   |
| Memo/hash-set cycle detection                                                    | removed `b66049d` etc.                           | O(depth) open-frame scan cheaper                                                                                                                                        |
| Custom `fast_dtoa` (572-line Ryu port)                                           | `ad401a8` → dropped `95fd416`                    | Superseded; file is now dead code                                                                                                                                       |
| `PythonObjectPool` (dict/list pooling)                                           | 0.1 branch                                       | Bookkeeping never beat CPython's allocator                                                                                                                              |
| Adaptive EMA pre-sizing (`AdaptiveSizeEstimator`)                                | 0.1 branch                                       | Simple last-size-per-depth heuristic won                                                                                                                                |
| String-value caching/pooling                                                     | 0.1 branch (`441bcc8`)                           | Null result: string creation = 0.92% of runtime, ≤9.7% hit rate                                                                                                         |
| Front-end instrumentation for PGO (`-fprofile-instr-generate/-use`)              | M9, and the blueprint's `setup.py`               | **~20% slower than plain -O3.** Source-level counters map poorly onto post-inlining IR; that flag pair is for coverage. IR-level `-fprofile-generate/-use` wins instead |
| Dict-construction micro-opts (compact builder, SIMD hash table, zero-copy views) | 0.1 branch (`66621b9`)                           | 0.1–0.5% absolute; `dict_dealloc` (~7%) unavoidable                                                                                                                     |
| CRTP/fused/computed-goto dispatch                                                | 0.1 branch                                       | 0%                                                                                                                                                                      |
| Parallel JSON parsing                                                            | 0.1 branch (`709de84`)                           | Never beat single-thread under the GIL                                                                                                                                  |
| SIMD structural indexer + `ParserIndexed`                                        | prompt-1/4 branches                              | Built but never wired into `loads`; bench delta ≈ noise                                                                                                                 |
| Markov speculative parser (`TransitionModel`)                                    | prompt-2 branch                                  | Opt-in only, unproven end-to-end                                                                                                                                        |
| Bloom-filter key subsystem                                                       | prompt-3 branch                                  | Never wired into hot path                                                                                                                                               |
| Whole-repo from-scratch rewrites                                                 | `main-from-scratch-v1/v2`, `archive/new_strata`  | See `docs/history/SKILL.md` — new_strata *succeeded* technically but was never merged                                                                                   |

## Salvageable unmerged work (ranked, all in `../archive/strata`)

1. **Dragonbox swap** — `main-v2` commits `7520522`/`33d6835`/`aa7d859` (3 commits
   past current main): vendors reference Dragonbox + fast_float, superseding the
   ~400-line custom Ryu-based `dragonbox.cpp` d2s (the `ryu_*` files remain
   in-tree as dead code even on that branch); measured dumps lead extended.
   The natural next step for this repo. **Status after M3:** not taken. The
   rebuild's dtoa is `std::to_chars` plus a layout pass, which is already exact
   and correctly rounded, so Dragonbox is now purely a throughput question and
   needs a benchmark before it lands (docs/decisions.md).
2. **Escape-mask odd-carry fix** — `949c575` (prompt-4): genuine correctness fix
   for backslash-run detection, relevant if a structural indexer ever returns.
3. **`5e04a18`** (do-nothing branch): LastWins duplicate-key default (matches
   `json.loads` semantics + enables the KnownHash fast path), SWAR ASCII check,
   pending-key fast path, max-per-depth presizing, fast_float fallback.
4. **`LightweightBuilder`** (`6da7fbd`/`dfd6aa5`, prompt-3): ~30% faster claim for
   small multi-schema inputs — directly targets the known `loads (mixed)` weakness.
5. From `archive/new_strata` (ideas, not code): Eisel–Lemire number parsing,
   runtime SIMD dispatch (CPUID/XGETBV vtable), construction-free
   `valid()`/tape-based navigation, `search_many` path-trie, and its measurement
   discipline (interleaved A/B with MAD \<1%).

## Known remaining weaknesses

- `loads`/`load` on **mixed multi-schema data**: key-cache misses + schema
  switching at record boundaries (#3 behind orjson). Best current leads:
  LightweightBuilder (above) or per-schema cache partitioning.
- The C++ parser is recursive with no depth limit (stack overflow on hostile
  nesting) — an iterative stack was planned in the original SAX decision record
  but never built.
- GIL never released — no parallelism story; parallel parsing lost under the GIL
  once already.
