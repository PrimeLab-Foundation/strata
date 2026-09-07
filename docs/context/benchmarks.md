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

The gate reads against a declared workload (`benchmarks/harness.py`:
`workload_rows()`, the 27 strata rows of the six datasets, and `CI_PLATFORMS`),
not against whatever rows survived parsing: `make bench-check` (which runs
`regression_check`) fails on zero matched entries, on a baseline row the
candidate lacks, or on a missing metric — missing rows are missing evidence,
not a pass — and reports rows with no baseline as ungated. Every report is
validated first (`harness.validate_report`: finite, ordered min ≤ median ≤ p95,
unique keys, no ERROR or unreadable rows, strata present in every declared
row); a report that fails validation cannot pass any gate.

The RSS column is a coarse gate, not a per-library measurement: the
harness reads the whole process's *current* resident size once after a
row's libraries have all run (`benchmarks/harness.py` `peak_rss_mb`, which
despite its name is `memory_info().rss`, not a peak) and records that one
number for every library in the row. It catches a build that holds much
more memory than before; it cannot attribute memory to a library or see a
transient peak. Memory-sensitive changes need separate evidence —
`benchmarks/alloc_roundtrip_probe.py` for allocation round trips, a
`ru_maxrss` probe over the datasets for peak memory per build (the
2026-09-04 `rss_probe` in docs/decisions.md is the pattern) — obtained
without changing the canonical latency experiment.

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

A complete-goal claim needs complete evidence: `ci_summary` counts every
row against the declared workload and every platform against the declared
five legs, cross-checks each report's commit and platform against
`run_info.json`, lists each leg as complete, unverified (no manifest — a
legacy report keeps its standings with no invented provenance), INCOMPLETE,
INVALID, MISMATCH or MISSING in an Evidence section, and exits 1 when
evidence is missing, invalid or misattributed (`--allow-incomplete` records
the gaps instead; `--expect none` scopes a deliberate subset). `ci_fetch`
verifies identity and coverage before placing anything and swaps a staged
replacement atomically, so a failed fetch leaves the previous reports and
manifest intact. The supportability tripwire keeps its 3.0x bound and now
requires strata in every declared row and category, and fails an empty
report.

New canonical reports carry `provenance_schema: 1` and a same-stem JSON
companion. The companion retains chronological samples in milliseconds,
full-precision aggregates, exclusions, dataset hashes, protocol parameters,
Python/dependency versions, checkout identity, and the measured extension's
path/hash. Build-produced `*.build.json` records the actual compiler commands,
tool versions, source identity, and PGO profile/input identities. An incremental
build preserves an existing matching identity; missing historical identities
remain unknown. Report-time environment variables are not build evidence.

All report gates validate a present companion and use its full-precision
measurements. Missing companions for marked reports, changed report hashes,
inconsistent samples, and binary/source mismatches invalidate the evidence.
Unmarked historical Markdown remains readable at its original precision.
CI fetch transports companions transactionally and refuses dirty or unknown
new build attribution. Keep each companion beside its Markdown when archiving
or comparing reports; keep build metadata beside a copied extension.

For an isolated before/candidate comparison, use identical report basenames
in separate directories. `make bench-baseline BENCH_REPORT=<before-report> BENCH_BASELINE=<evidence-baseline.json>` records that comparison's baseline;
`make bench-check BENCH_REPORT=<candidate-report> BENCH_BASELINE=<evidence-baseline.json>` applies the unchanged thresholds.
This leaves the published baseline intact. A failed comparison remains failed;
do not replace its before evidence with candidate measurements.

`make bench-supplementary BENCH_SUPPLEMENTARY_TIER=medium` produces a
separate six-row `supplementary-v1` report: three NDJSON searches and folder
load/dump/search against a Strata per-file loop. It checks ordered results
and exact dumped bytes before timing. Both folder arms include discovery or
grouping. `BENCH_REPORTS`, `BENCH_REPEAT`, and `BENCH_WARMUP` select its output
directory and protocol. Its denominator never changes the canonical 135 rows.
The monthly supplementary workflow measures medium/large on Linux x86,
macOS ARM, and Windows; it retains canonical tier reports separately too.

`make probe-file-costs` writes a separate real-file diagnostic packet (default
small mixed, 60 samples). `FILE_COST_DATASET` and `FILE_COST_OUTPUT` override
the paths. It records native dump and Python-composition phase controls,
checks newline and POSIX 0644 behavior, and retains raw samples. Phase timers
add overhead; this is not native attribution or a canonical acceptance gate.
