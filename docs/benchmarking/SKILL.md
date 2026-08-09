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
