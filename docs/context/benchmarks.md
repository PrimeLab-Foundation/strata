# Benchmarks

Canonical benchmark contract. Deep detail (standings, harness internals, PGO):
`docs/benchmarking/SKILL.md`.

## Competitors and what each is compared on

Strata's feature set is a superset of every competitor's. Compare only where a
library natively supports the operation — strict mode: unsupported = excluded,
never emulated with Python shims. Where no native equivalent exists, the
documented composition below is the comparison target.

| Category                      | Measured                     | Competitors                                     | Composition for competitors                                             |
| ----------------------------- | ---------------------------- | ----------------------------------------------- | ----------------------------------------------------------------------- |
| `loads`                       | text/bytes → Python tree     | orjson, msgspec, ujson, stdlib json, pysimdjson | native parse                                                            |
| `dumps`                       | tree → text/bytes            | orjson, msgspec, ujson, stdlib json             | native; orjson emits bytes — compare bytes-to-bytes                     |
| `load` (.json)                | file → tree                  | orjson, msgspec, ujson, stdlib json             | `read()` + parse                                                        |
| `load` (.ndjson)              | file → list of records       | orjson, msgspec, ujson, stdlib json             | `read()` + splitlines + per-line parse                                  |
| `dump`                        | tree → file                  | orjson, msgspec, ujson, stdlib json             | serialize + `write()`                                                   |
| `query`                       | JSONPath over in-memory tree | jmespath, jsonpath-ng                           | equivalent query in each language                                       |
| `search` (.json/.ndjson)      | file → matches               | orjson+jmespath, orjson+jsonpath-ng             | full parse + query pipeline (no competitor streams)                     |
| folder `load`/`dump`/`search` | directory ↔ records          | none                                            | no competitor equivalent; benchmark vs per-file loop over strata itself |

pysimdjson: parse only (no serializer, no JSONPath). jmespath/jsonpath-ng:
query only. msgspec: no NDJSON file API — composition row applies. Cursor mode
and `config` policies are strata-only: correctness-tested, not benchmarked.

## Datasets

Deterministic generator (seed 42), users/orders/items schema, `.json` +
`.ndjson` variants: **small** 1000 users · **medium** 2000 · **large** 4000.
Plus four synthetic shapes per tier: `flat`, `nested`, `wide_arrays`, `mixed`
(records 500/2000/5000). Generated under `benchmarks/data/generated/`
(gitignored, regenerated on demand).

## How to run

```bash
make bench-data      # regenerate datasets
make bench-small     # → docs/benchmarks/bench_results_small.md
make bench-medium    # → docs/benchmarks/bench_results_medium.md
make bench-large     # → docs/benchmarks/bench_results_large.md
make bench-all       # data + all tiers
make bench-ci        # fetch the latest CI run → docs/benchmarks/ci/ + ci_summary.md
```

Protocol, per run:

1. Quiet machine; release build — PGO+LTO (`make pgo`) for headline numbers.
2. Warmup ≥ 1, repeat ≥ 3 exploratory; **regression-gate comparisons use the
   median of ≥ 10 repeats** (2% resolution needs it). Micro-benchmarks —
   single-operation standalone scripts and `experiments/` prototypes — use
   ≥ 30 repeats. `gc.collect()` between iterations; psutil present so RSS is
   captured.
3. Report min / median / p95 + peak RSS per category, plus environment: CPU,
   OS, Python, compiler flags, commit hash.
4. A run containing ERROR rows is invalid — fix or exclude explicitly, never
   publish partial tables.

## Fairness rules

Same datasets and semantics for every library; no winning by changing
semantics (document any semantic mismatch instead). Results are
machine-written to `docs/benchmarks/` — never hand-edit.

## Regression gate

Before optimizing: capture baseline. After: compare against
`benchmarks/results/baseline.json`. Median or p95 worse by **>2%** in a
touched category, or RSS worse by **>5%** ⇒ fix or revert. Refresh the
baseline (`--save-baseline`) only after an accepted improvement.

## CI standings by platform and architecture

The weekly `benchmark.yml` workflow runs this same suite on every supported
platform/architecture leg. `make bench-ci` fetches the latest completed run
into `docs/benchmarks/ci/bench_results_<os>-<arch>.md` and rebuilds
`docs/benchmarks/ci_summary.md` — strata's rank in every row on each leg,
the rows still behind, and the verdict against the goal: **#1 in every row
on every platform and architecture**. Ranks are within-run comparisons only;
absolute times never cross platforms, the supportability tripwire remains
the CI gate, and headline standings still come exclusively from the
quiet-machine protocol. Detail: `docs/benchmarking/SKILL.md`.
