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

## CI standings by platform and architecture (`make bench-ci`)

The goal on this axis: **strata #1 in every row on every supported platform
and architecture**, tracked in-tree rather than left in CI logs.

- `benchmark.yml` runs the small tier (repeat 10, warmup 2, CPython 3.12) on
  every supported leg — linux-x86_64, macos-x86_64, macos-arm64,
  windows-x86_64, plus a visibility-guarded linux-arm64 leg that activates
  when the repo goes public — and uploads one report artifact per leg,
  named `benchmark-<os>-<arch>`. Every leg benchmarks the **PGO build**
  (the build the release wheel ships, worth −8..−25%), since the
  competitors are their released wheels: the POSIX legs run `make pgo`, the
  Windows leg runs `scripts/pgo_build_msvc.py` (MSVC's LTCG spelling:
  `/GL`, `/GENPROFILE` → `/USEPROFILE`), and each report's compiler_flags
  line says which build its leg measured.
- `make bench-ci` (`benchmarks/ci_fetch.py`) pulls the latest completed
  run's reports into `docs/benchmarks/ci/bench_results_<os>-<arch>.md` plus
  `run_info.json` (run id, sha, date, artifact map). os/arch come from each
  report's own environment header, never from runner labels, so artifacts
  from before the per-platform artifact names fetch identically. A fetch
  replaces the previous one wholesale — the directory holds one run's
  evidence, never a mix of runs. `--run <id>` fetches a specific run.
- `benchmarks/ci_summary.py` (second step of `make bench-ci`; alone via
  `make bench-ci-summary`) writes `docs/benchmarks/ci_summary.md`: per
  platform-arch, #1 rows per category, and every row still behind with its
  rank, gap and fastest rival. A report with ERROR rows is marked INVALID
  and excluded from the counts rather than published as partial standings.

Reading the numbers: a rank is computed within one report — one machine, one
interleaved round — which is the same-machine comparison the contract
allows; absolute times never cross platforms, the supportability tripwire
stays the CI gate, and headline standings come only from the quiet-machine
protocol. Shared runners are noisy — treat a single-run rank decided inside
~1.05x as a coin flip and re-run before acting on it.

First fetch (run 31392004866, 2026-08-10, commit `16b0a58`): **44/108 rows
at #1** — that run measured plain non-PGO builds and predated the wave-8/9
work. After billing was fixed (2026-08-15), two same-commit runs on wave-9
main scored **81/108 and 78/108**: parse categories (loads/load/NDJSON/
query/search) swept on the POSIX legs, macos-arm64 26/27. Cross-tabulating
the two runs separates signal from runner noise — rows behind in *both*:
dumps mixed on every leg (1.13–1.51x, worst on macos-x86_64), dumps/dump
users-nested-flat on the x86_64 legs (1.01–1.25x), and the whole Windows
serialization block (still an /O2 leg in those runs; the MSVC PGO wiring
landed after). Rows flipping between runs (1.00–1.09x) are the coin-flip
band. Wave 10 (schema-emit — `docs/performance/SKILL.md`) targets the
stable dumps residue; re-dispatch and `make bench-ci` for the post-wave
verdict.

### Remaining to #1-everywhere (the row-by-row backlog)

Post-wave-10 state (2026-08-15), all rows and their known leads:

- **Coin-flip band** (1.00–1.03x, every leg): decided by machine state, not
  code — four consecutive deciding sweeps flipped these rows' signs with no
  change between runs. Only the gated quiet-machine protocol settles them.
  Today's instance: `dump mixed` at 1.01x on small/medium in one tier run
  and #1 (0.96x) at large in the next.
- **`loads mixed`/`loads flat` small**: closed by the wave-10 loads pass
  (prediction-probe word compares + builder-side compact-ASCII
  construction — docs/performance/SKILL.md): every small-tier loads row is
  #1 in the post-wave tier refresh. The `LightweightBuilder` salvage item
  stays unspent; the 17%-of-profile `_tlv_get_addr` under CPython 3.14's
  own allocator on macOS is recorded as non-differential — do not chase it.
- **wide_arrays serialization large**: the one standing residue. Wave 9
  closed the isolated gap; wave-10's decomposition acquits every scalar run
  individually (floats 1.01x, ints 0.97x, strings 0.73x vs orjson on
  exactly wide's data — and orjson's parts sum *slower* than its whole).
  The assembled document still reads 1.03x isolated / 1.05–1.08x
  interleaved on today's PGO roll, oscillating with each profile build:
  run-switching churn across 20k 64-element lists, still not a code gap a
  profiler names. The itoa restructuring leads remain dead
  (negative-results table).
- **Windows rows**: was the one structural gap — that leg benchmarked /O2
  while POSIX legs benchmark PGO. Closed 2026-08-15: setup.py now maps the
  same env knobs to MSVC LTCG PGO (/GL, /GENPROFILE → /USEPROFILE) and
  `scripts/pgo_build_msvc.py` drives the two phases on the CI leg, so
  Windows serialization rows shed their ~10–25% build handicap. Awaiting a
  post-change benchmark.yml run for the measured verdict.
- **The five-leg verdict**: billing was fixed 2026-08-15 and two same-commit
  wave-9 runs scored 81/108 and 78/108 (from the committed pre-wave
  44/108). The wave-10 code (schema-emit + loads pass) and the MSVC PGO
  wiring are in-tree awaiting the human push; then
  `gh workflow run benchmark.yml` + `make bench-ci` deliver the verdict on
  all four active legs (linux-arm64 activates when the repo goes public).

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
bench_main/Makefile/CI; `docs/benchmarks/ci/bench_results_<os>-<arch>.md` +
`run_info.json` and `docs/benchmarks/ci_summary.md` by `make bench-ci`
(ci_fetch → ci_summary); `benchmarks/results/baseline.json` by
regression_check; `build/pgo/bench_results_pgo.md` by the PGO pipeline.
Generated data under `benchmarks/data/generated/` stays gitignored.
