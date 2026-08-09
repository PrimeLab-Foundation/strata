---
name: benchmarking
description: Benchmark suite deep detail — harness internals, 
  previous-implementation standings, the old regression tool's defects (do not 
  reproduce), PGO's role, and result-path contracts. Load before running or 
  modifying benchmarks or interpreting results.
---

# Benchmarking

The canonical contract — competitor/feature matrix, datasets, run commands,
protocol, fairness, regression thresholds — is `docs/context/benchmarks.md`.
This doc holds the deep detail behind it, described from the previous
implementation (`backup/pre-reset-main`) as the blueprint.

## Harness internals (blueprint)

Orchestrator `benchmarks/bench_main.py` (seed 42; exploratory runs repeat 3,
warmup 1 — gating runs need ≥ 10 repeats per the contract). Six report
sections: `loads`, `load`, `dumps`, `dump`, `search`, `query` — NDJSON
datasets appear only in loads/load; search/query cover `users.json` plus the
four random-schema shapes. **Target addition:** the contract's competitor
table includes NDJSON `search` — the rebuilt harness must add it (the
previous one did not benchmark it).

Standalone deep-dive scripts: `bench_loads.py`, `bench_dumps.py`,
`bench_ndjson.py`, `bench_jsonpath.py` (stdout only). `profile_dumps.py` was
a dev micro-profiler hard-importing orjson/msgspec. Metrics via `harness.py`:
min/median/p95 + RSS (psutil). `benchmarks/datasets.py` was orphaned
(imported by nothing) — don't recreate it.

## Standings of the rebuild (quiet-machine sweep, macOS arm64 / Apple M1 Max, py3.14)

The deciding evidence run: PGO+LTO headline build, AC power, Low Power Mode
off, caffeinated, repeats 30/20/10 by tier — machine-written to
`docs/benchmarks/bench_results_{small,medium,large}.md` with the flags and
repeat counts recorded. Ranks are out of the libraries that natively do the
operation; pysimdjson has no CPython 3.14 wheel and is excluded.

| Category        | small (r30) | medium (r20) | large (r10) | vs best rival |
| --------------- | ----------- | ------------ | ----------- | ------------- |
| `loads`         | 3/5         | 4/5          | 4/5         | 0.50x – 1.26x |
| `dumps`         | 4/5         | 4/5          | 2/5         | 0.78x – 1.12x |
| `load`          | 3/5         | **5/5**      | **5/5**     | 0.47x – 1.15x |
| `load (ndjson)` | **1/1**     | **1/1**      | **1/1**     | 0.65x – 0.74x |
| `dump`          | 2/5         | 3/5          | 3/5         | 0.81x – 1.18x |
| `query`         | **3/3**     | **3/3**      | **3/3**     | 0.01x – 0.37x |
| `search`        | **3/3**     | **3/3**      | **3/3**     | 0.05x – 0.34x |

**Total: 63/81 rows at #1** (8/27 on one tier before the M10 waves).

**The headline dataset (users) is #1 in every category at every tier** with
one exception: `dumps` at large, a 1.00× tie decided against us in the third
decimal. Category sweeps: `query` 9/9 (3–100×), `search` 9/9 (2.9–20×),
NDJSON `load` 3/3 (1.3–1.5× ahead), file `load` 13/15 including 5/5 at both
medium and large. In-memory `loads` leads users at all tiers (0.90–0.94×)
and flat/mixed decisively at medium and large (down to 0.54×).

**The 18 rows still behind, all explained:** small-tier `flat`/`mixed`
`loads`/`load` (1.15–1.26×: per-object creation overhead on documents too
small to amortize the caches), `mixed` serialization (1.02–1.18×: per-call
floor on a 30 KB document), `wide_arrays` `loads` at medium/large
(1.08–1.12×: orjson's homogeneous-array parsing), and the large-tier
`dumps` users/wide ties at 1.00–1.08×. Techniques tried, adopted and
rejected: `docs/performance/SKILL.md`.

### Cross-session baselines are not comparable on this machine

Re-running an *unchanged* tree across sessions has measured ~30% apart on
every row — and Low Power Mode alone explains a large share (the deciding
sweep was gated on power state for exactly this reason). The regression gate
is a same-session before/after instrument; `benchmarks/results/baseline.json`
is refreshed alongside each published report.

## Standings at the pre-reset tip (`c0e3b5a`, macOS arm64, py3.14)

These are the numbers the previous implementation achieved — the rebuild's
target to match or beat:

- **loads users.json:** #1 all tiers — 1.22×/1.27×/1.28× faster than orjson
  (small/medium/large); msgspec further behind.
- **loads users.ndjson:** #1 all tiers — up to ~1.5× vs orjson at medium/large.
- **dumps users.json:** #1 all tiers — ~1.5× vs orjson at large.
- **search (file JSONPath):** #1 in 11/11 queries, avg ~9–10× vs
  orjson+jsonpath-ng pipelines (e.g. `$..price` small: 1.6 ms vs 128.9 ms).
- **query (in-memory):** #1 in 12–13/13 (e.g. `$.users[*].id`: 0.070 ms vs
  jmespath 0.408 ms).
- **Known weak spots** (small tier): `loads (mixed)` #3 (~45% behind orjson —
  multi-schema key-cache misses at record boundaries), `dumps (wide_arrays)`
  and `dumps (nested)` #3 by small margins.
- All numbers were single-host (dev MacBook); CI did not commit results. The
  un-suffixed `bench_results.md` was a stale earlier run contradicting the
  sized reports — the rebuilt tooling should not produce a divergent default
  output file.

## Regression tooling — previous implementation's defects (do not reproduce)

Design: `benchmarks/regression_check.py` parses the results markdown,
compares strata medians/RSS to `benchmarks/results/baseline.json`, fails CI
on regression; `--save-baseline` refreshes. No auto-revert — the gate only
fails CI. **The rebuilt checker must enforce the contract thresholds
(>2% median/p95, >5% RSS) — the old tool's 5%-only threshold was part of its
brokenness.**

At the pre-reset tip it was broken three ways: (1) `main()` crashed
(`args.save_strata` AttributeError + an undefined name at the empty-baseline
check) while unit tests only covered the library functions; (2) header
contract drift — the parser looked for `## Parsing Benchmarks` but bench_main
emitted `## loads (in-memory parsing)`, so zero rows parsed; (3) a stale
baseline key (`"users"` from the `path.stem` era) never matched. Its CI
workflow also pointed at pre-tier dataset paths. Lessons: test `main()`
end-to-end, make the writer and parser share one format definition, and key
baselines by full filename.

## PGO

`make pgo` produced the optimized build used for headline numbers
(historically ~+15% medium/large — measured wins table:
`docs/performance/SKILL.md`). Weekly `pgo.yml` built and uploaded a PGO+LTO
wheel. Training data/workload: `scripts/pgo_training_data.py` (TARGET_MB=10)

- `scripts/pgo_training.py` exercising all hot paths.

## Result-path contract (writers and readers must agree)

`docs/benchmarks/bench_results{,_small,_medium,_large}.md` written by
bench_main/Makefile/CI; `benchmarks/results/baseline.json` by
regression_check; `build/pgo/bench_results_pgo.md` by the PGO pipeline.
Generated data under `benchmarks/data/generated/` stays gitignored.
