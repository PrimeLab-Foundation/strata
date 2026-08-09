---
name: benchmarking
description: Benchmark suite — how to run it, datasets and competitors, where results are written, current performance standings, the regression baseline (currently broken in three ways), and PGO's role. Load before running or modifying benchmarks or interpreting results.
---

# Benchmarking

## Running

```bash
make bench-data     # generate datasets → benchmarks/data/generated/{small,medium,large}/
make bench-small    # → docs/benchmarks/bench_results_small.md   (records=500)
make bench-medium   # → docs/benchmarks/bench_results_medium.md  (records=2000)
make bench-large    # → docs/benchmarks/bench_results_large.md   (records=5000)
make bench-all
```

All run `python -m benchmarks.bench_main` (seed 42, repeat 3, warmup 1).
Six report sections: `loads`, `load`, `dumps`, `dump`, `search`, `query` —
NDJSON datasets appear only in loads/load; search/query cover `users.json` plus
the four random-schema shapes (`flat`, `nested`, `wide_arrays`, `mixed`).
Standalone deep-dive scripts: `bench_loads.py`, `bench_dumps.py`,
`bench_ndjson.py`, `bench_jsonpath.py` (stdout only). `profile_dumps.py` is a
dev micro-profiler that hard-imports orjson/msgspec.

**Competitors:** orjson, msgspec, ujson, stdlib json (+ pysimdjson parse-only;
jmespath and jsonpath-ng for query/search). Strict philosophy inherited from
turbojsonpath: no emulated fallbacks — unsupported = excluded, not faked
(`strict_missing` flag).

**Datasets:** deterministic users/orders/items generator
(`benchmarks/data/generate_bench_data.py`, seed 42): small = 1000 users,
medium = 2000, large = 4000 (Makefile comment saying "5k" is stale).
Metrics via `harness.py`: min/median/p95 + RSS (psutil).
`benchmarks/datasets.py` is orphaned (imported by nothing).

## Current standings (committed results @ `c0e3b5a`, macOS arm64, py3.14)

- **loads users.json:** #1 all tiers — 1.22×/1.27×/1.28× faster than orjson
  (small/medium/large); msgspec further behind.
- **loads users.ndjson:** #1 all tiers — up to ~1.5× vs orjson at medium/large.
- **dumps users.json:** #1 all tiers — ~1.5× vs orjson at large.
- **search (file JSONPath):** #1 in 11/11 queries, avg ~9–10× vs
  orjson+jsonpath-ng pipelines (e.g. `$..price` small: 1.6 ms vs 128.9 ms).
- **query (in-memory):** #1 in 12–13/13 (e.g. `$.users[*].id`: 0.070 ms vs
  jmespath 0.408 ms).
- **Known weak spots** (small tier): `loads (mixed)` #3 (~45% behind orjson —
  multi-schema key-cache misses at record boundaries), `dumps (wide_arrays)` and
  `dumps (nested)` #3 by small margins.
- Caveat: `docs/benchmarks/bench_results.md` (the un-suffixed file) is a stale
  earlier run that contradicts the sized reports — trust the `_small/_medium/_large`
  files. All numbers are single-host (dev MacBook); CI does not commit results.

## Regression gating — design vs reality

Design: `benchmarks/regression_check.py` parses the results markdown, compares
strata medians/RSS to `benchmarks/results/baseline.json`, fails CI on >5%
regression (weekly `benchmark.yml`). `--save-baseline` refreshes the baseline.
There is **no auto-revert** anywhere — the gate only fails CI.

**Reality — broken three ways** (as of the fresh-start commit):

1. `main()` crashes: references `args.save_strata` (argparse defines
   `save_baseline`) → AttributeError; also an undefined `strata` name at the
   empty-baseline check. Library functions are fine; unit tests never call `main()`.
2. Header contract drift: the parser looks for `## Parsing Benchmarks`, but
   bench_main now emits `## loads (in-memory parsing)` → zero rows parsed.
3. Stale baseline key: `baseline.json`'s JSON-dataset key `"users"` dates from
   the `path.stem` era and never matches the current `path.name` key
   (`users.json`); the `"users.ndjson"` entry still matches — moot until (2) is
   fixed, since zero parsing rows get extracted anyway.

Additionally `benchmark.yml` points at pre-tier dataset paths
(`generated/users.json`) that `make bench-data` no longer creates.
Fixing the gate = fix these four things and re-save a baseline.

## PGO

`make pgo` produces the optimized build used for headline numbers
(historically ~+15% medium/large). Weekly `pgo.yml` builds and uploads a
PGO+LTO wheel. Training data/workload: `scripts/pgo_training_data.py`
(TARGET_MB=10; docstring "~64MB" is stale) + `scripts/pgo_training.py`.

## Result-path contract (do not move these without updating writers)

`docs/benchmarks/bench_results{,_small,_medium,_large}.md` are written by
bench_main/Makefile/CI; `benchmarks/results/baseline.json` by regression_check;
`build/pgo/bench_results_pgo.md` by the PGO pipeline. Generated data under
`benchmarks/data/generated/` is gitignored.
