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

| Change                                                                 | Effect (median)                             |
| ---------------------------------------------------------------------- | ------------------------------------------- |
| Escape clean runs in bulk instead of byte-at-a-time `push_back`        | part of the dumps 8-24% below               |
| Integers via `std::to_chars` into a stack buffer, not `std::to_string` | (same batch; `to_string` allocated per int) |
| Thread-local dumps output buffer, reused across calls                  | dumps -8% to -24% across all datasets       |
| Arrays built into a flat vector, then one `PyList_New(n)` + `SET_ITEM` | loads -13% to -22% across all datasets      |
| Per-document key cache (interned key reuse across records)             | (same batch)                                |

Not yet rebuilt, and the reason `dumps` still trails orjson ~3x: the 3-tier
dtoa, batch same-schema dict serialization, and the homogeneous int/float/
str/bool array fast paths.

### Measured non-wins in the rebuild

| Idea                                     | Verdict                                                                                                                                                                    |
| ---------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Module-lifetime small-int cache (0..256) | Not built: CPython already caches small ints internally, so `PyLong_FromLongLong` returns the shared object. The blueprint's cache duplicated a cache that already exists. |
| Presized dicts via `_PyDict_NewPresized` | Not built: the API is CPython-internal and the architecture notes already flag it as version-sensitive. Deferred until it can be measured against a portable alternative.  |

## Negative results — do not retry without new evidence

| Idea                                                                             | Where tried                                      | Verdict                                                                                     |
| -------------------------------------------------------------------------------- | ------------------------------------------------ | ------------------------------------------------------------------------------------------- |
| simdjson-style structural tape                                                   | 0.1 branch (`e8443f3`; default OFF in `84fdf8c`) | **Tape OFF was +22% on large loads** — 46MB JSON → ~110MB tape → L3 thrash on Apple Silicon |
| Array/object pre-counting for pre-sizing                                         | main (`67ef7fc` → reverted `07f85a0` same day)   | Pre-scan cost more than resize savings; depth-based prediction won                          |
| `mem_eff` JSONPath mode                                                          | main (`cbc9162` → removed `5c264ca`)             | SAX search made it redundant within a day                                                   |
| mmap I/O as the primary file path + NDJSON bindings                              | removed `a41eea0`                                | Separate slower code path; file `load` was worst-in-class at the time                       |
| Memo/hash-set cycle detection                                                    | removed `b66049d` etc.                           | O(depth) open-frame scan cheaper                                                            |
| Custom `fast_dtoa` (572-line Ryu port)                                           | `ad401a8` → dropped `95fd416`                    | Superseded; file is now dead code                                                           |
| `PythonObjectPool` (dict/list pooling)                                           | 0.1 branch                                       | Bookkeeping never beat CPython's allocator                                                  |
| Adaptive EMA pre-sizing (`AdaptiveSizeEstimator`)                                | 0.1 branch                                       | Simple last-size-per-depth heuristic won                                                    |
| String-value caching/pooling                                                     | 0.1 branch (`441bcc8`)                           | Null result: string creation = 0.92% of runtime, ≤9.7% hit rate                             |
| Dict-construction micro-opts (compact builder, SIMD hash table, zero-copy views) | 0.1 branch (`66621b9`)                           | 0.1–0.5% absolute; `dict_dealloc` (~7%) unavoidable                                         |
| CRTP/fused/computed-goto dispatch                                                | 0.1 branch                                       | 0%                                                                                          |
| Parallel JSON parsing                                                            | 0.1 branch (`709de84`)                           | Never beat single-thread under the GIL                                                      |
| SIMD structural indexer + `ParserIndexed`                                        | prompt-1/4 branches                              | Built but never wired into `loads`; bench delta ≈ noise                                     |
| Markov speculative parser (`TransitionModel`)                                    | prompt-2 branch                                  | Opt-in only, unproven end-to-end                                                            |
| Bloom-filter key subsystem                                                       | prompt-3 branch                                  | Never wired into hot path                                                                   |
| Whole-repo from-scratch rewrites                                                 | `main-from-scratch-v1/v2`, `archive/new_strata`  | See `docs/history/SKILL.md` — new_strata *succeeded* technically but was never merged       |

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
