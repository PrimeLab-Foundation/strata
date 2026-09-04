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
  Windows leg runs `scripts/pgo_build_clang_cl.py` (clang's PGO under
  clang-cl — `-fprofile-generate` → llvm-profdata → `-fprofile-use`, the
  MSVC linker, no LTO yet; the MSVC twin `scripts/pgo_build_msvc.py` stays
  for the LTCG build — wave 18), and each report's compiler_flags
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

### The cross-platform iteration loop (how rows actually get closed)

The working cycle that took the legs from 44/108 to 77/81+, one round per
push:

1. **Push (human).** Commits and pushes are made by the human only; the
   agent prepares the change set and a ready-to-use message. CI measures
   pushed refs, so iteration cadence is bounded by pushes — batch every
   ready fix into each round.
2. **Dispatch**: `gh workflow run benchmark.yml --ref main`.
3. **Fetch**: `make bench-ci` — replaces `docs/benchmarks/ci/` with that
   run's per-leg reports and rebuilds `ci_summary.md`.
4. **Classify before chasing.** With two same-commit runs, cross-tabulate
   the behind rows: behind in *both* runs = a real gap; flipping between
   runs (1.00–1.09x) = the coin band, which code cannot settle. For
   sub-0.1 ms rows measure locally with in-process repeat rounds —
   separate-process micros swing ±60% with core placement.
5. **Fix profile-first, prove by A/B.** `sample` a spin loop of the exact
   row, change the named sink, attribute with a stash A/B on a plain
   build, gate both suites, and record the outcome — wins in the measured
   table, losses in the negative-results table — plus a decisions.md line.
   Check the negative-results table *before* attempting an idea.
6. **Repeat from 1.** A Windows-leg failure prints its own diagnosis:
   `pgo_build_clang_cl.py` (and the MSVC twin) distinguishes "never
   instrumented" from "runtime did not flush" and lists where it searched.

### Remaining to #1-everywhere (the row-by-row backlog)

**Status 2026-09-04, wave 22.** The `loads mixed` coin band had a cause:
the key predictor retired mixed.json's record depth for the life of the
thread once the other datasets had gone through the same builder, so the
harness always measured that row on the miss path (128 → 145 µs warm,
`benchmarks/predictor_state_probe.py`). With retirement scoped to one
input the small tier reads 1.06x, 1.01x and 0.98x on
that row across three samples at load 5.6–6.8, every other row #1, medium and large 27/27 on the same build; the
first clean quiet sample reads that row 1.01x (0.130 against 0.129, a tie
within a percent) with every touched median below the baseline, which is
refreshed on it, and the next two genuinely quiet draws (load 1.8–2.0 throughout) read
**27/27** with `loads mixed` 0.95x and 1.00x — the local platform first
in every row of every tier on two of three clean draws, the third a tie
within a percent. CI remains blocked by
the spending limit.

**Status 2026-09-04, review and the first 27/27 small roll.** The wave-20/21
adversarial review found no defect (13.4 M differential documents, 3.1 M
exact-size sanitizer documents, every number re-measured) and disclosed
the helper's cost on exponent floats and 20+-digit fractions, absent from
every dataset. The small tier then rolled **27/27** on de68beb at load
4.7–4.9 (`loads mixed` 0.99x, 0.135 ms against 0.137), the medium tier
27/27 on two of three rolls (the third's `dump mixed` a spiked median,
0.96x on the others) and the large tier 27/27; a second small sample read `loads mixed`
1.05x with every other row #1 — the last row is a coin band of about
five percent either way under load (27/27 and 26/27 across two samples),
medium and large 27/27 on every roll; the quiet confirmation and the
baseline refresh are next. CI remains blocked by the spending limit.

**Status 2026-09-04, later (wave 21).** The builder's one-pass string copy
(`copy_if_ascii`) moved the parse-side rows the coin band had held: in the
same-venv A/B users.json −6.5%, wide_arrays −2.5% and mixed −2 to −4%;
the small tier's roll on that build reads 26/27 at load 5–6 — `loads users` 0.80x,
`loads wide_arrays` 0.95x, `load mixed` 0.97x, and `loads mixed` 1.01x at
0.128 ms against orjson's 0.127; the baseline is not refreshed under that
load. The CI
block is unchanged.

**Status 2026-09-04 (wave 20, runners still refusing jobs).** The local
small tier's last row behind, `loads mixed`, reads 1.01x after the number
head's long-fraction step (0.130 ms against orjson's 0.129; `load mixed`
1.00x; 1.03x and 0.98x on a second roll; a same-venv A/B against the previous build reads the row −0.5%, inside noise) — the parse-side coin band is now one row wide
and one percent deep. The CI standings still stand at the 060bd7b sample
(105/108): every dispatched and auto-triggered run since 2026-09-03 22:08
UTC has failed within seconds with zero steps, GitHub Actions refusing to
schedule hosted runners for the repository — the organization's Actions
spending limit is exhausted (September 2026 through the org usage endpoint:
1,833 Linux, 917 Windows and 932 macOS 3-core minutes, about $60 net past
the included allowance, macOS at its 10x multiplier most of it), so runs
resume only when the limit is raised or the billing month resets; the
wave-19 (1fb880b) and wave-20 builds have no CI sample yet.

**Status 2026-09-03 (end of the wave-15–18 day).** Fourteen clean CI samples on
the day's code line read 101–106/108; the last six have Linux and both macOS
legs at 27/27 on their clean draws and every row behind inside its leg's coin
band. The one family still flipping is Windows `dumps mixed`: six draws of the
clang-cl PGO build read 1.02x, 1.19x, 1.06x, 0.99x, 1.01x and 0.98x (1.06–1.25x
under MSVC PGO), 0.82x isolated on the same build — a coin flip centred on
parity, the runner's band and not the code's. The
Windows leg builds with clang-cl's PGO since wave 18 (`scripts/pgo_build_clang_cl.py`);
the profile job's three-arm blocks (clang-cl PGO, plain MSVC, plain clang-cl)
and the allocation, cold-state and decomposition probes are the instruments
that named each wave's lever. Locally the quiet rolls read medium and large
27/27 and small 25/27 (loads mixed 1.04x, load mixed 1.02–1.06x — the
parse-side coin band).

Post-wave-11 state (2026-09-02), all rows and their known leads:

- **Coin-flip band** (1.00–1.03x, every leg): decided by machine state, not
  code — four consecutive deciding sweeps flipped these rows' signs with no
  change between runs. Only the gated quiet-machine protocol settles them.
  Today's instance: `dump mixed` at 1.01x on small/medium in one tier run
  and #1 (0.96x) at large in the next.
- **Small-document parse under CPython 3.14 (this machine)**: reopened by
  the 3.14.7 refresh (loads mixed 1.10x, flat 1.03–1.05x, nested a coin
  flip after CPython halved orjson's nested time); wave 12 (digit runs by
  word + the short-number head, `docs/performance/SKILL.md`) moved flat
  small to 1.007x, nested to 0.93x, wide_arrays large to 0.91x and the
  medium/large parse rows further ahead; wave 13 (the resolved key cursor
  in the frame) then took every keyed row another 3–9% (flat small 0.94x,
  nested 0.87x, users 0.84x) and mixed 2%, leaving loads mixed at ~1.06x
  with PyDict_SetDefault's own lookup and the four-way first-key probe as
  the remaining per-key cost. Follow-ups on file:
  the token-loop re-scans and single-pass strings (the 17-digit head
  extension and the inline lead-key words were tried and measured flat —
  negative-results table). On the Windows leg the first loads decomposition
  (2026-09-03) reads flat at parity and names MSVC's parse-side sinks by
  kind: 7-digit int lists 1.31x behind msgspec while 1–3-digit lead, 5dp
  floats 1.14x, bools/nulls/short strings 1.06–1.13x behind msgspec — the
  per-width int buckets now in `decompose_loads_flat.py` localize the
  first of these on the next profile run.
- **`loads mixed`/`loads flat` small (wave 10)**: closed by the wave-10 loads pass
  (prediction-probe word compares + builder-side compact-ASCII
  construction — docs/performance/SKILL.md): every small-tier loads row is
  #1 in the post-wave tier refresh. The `LightweightBuilder` salvage item
  stays unspent; the 17%-of-profile `_tlv_get_addr` under CPython 3.14's
  own allocator on macOS is recorded as non-differential — do not chase it.
- **wide_arrays serialization large**: the residue has a reproduced
  mechanism (docs/decisions.md, 2026-09-02): the same harness reads the row
  0.85x on wide_arrays alone and 1.06–1.09x when users.json ran first —
  strata +1.4 ms and orjson −1.3 ms on identical code, partly the
  allocator's large cache no longer handing strata back its resident output
  block (168 re-faulted pages per call after the users phase, 2 fresh),
  partly measurement order acting on the rivals. The answer taken is
  margin: wave 11 (SWAR digit word + branch-free micro-decimal emission,
  `docs/performance/SKILL.md`) moves the row from 0.92x to 0.85x
  in-process, past the ~12% order effect, and 6-decimal float lists from
  1.09x behind to 0.83x ahead. The itoa restructuring leads remain dead
  (negative-results table, now including the nine-digit word split).
- **Windows rows**: the /O2-vs-PGO build handicap closed 2026-08-15 (MSVC
  LTCG PGO via `scripts/pgo_build_msvc.py`; superseded 2026-09-03 by the
  clang-cl PGO build the leg measures now), and the leg's anchor row
  (dumps mixed) first crossed on 2026-08-16. Two further findings from the
  2026-09-02 sample: (1) the file reader had no sized read on Windows —
  every `load` there went through a 64 KB append-and-grow loop, which the
  leg's numbers showed as `load` trailing `loads` by 1.3–2.6 ms on 1–2 MB
  files while rivals' `read_bytes` cost 0.1–1 ms; the reader is now raw
  descriptor I/O with one sized read on both platforms (awaiting the next
  Windows sample for the load-row verdict). (2) Windows runners are not one
  machine: the 104/108 sample ran on EPYC Genoa (`Family 25 Model 17`), the
  102/108 sample on Milan (`Model 1`), and loads flat swung 0.86x → 1.10x
  between them with no code change — read every Windows row against the
  report's processor line before chasing it.
- **The five-leg verdict**: billing was fixed 2026-08-15 and two same-commit
  wave-9 runs scored 81/108 and 78/108; the M11 fused record writer then
  carried the tracker to 104/108 (2026-08-16, all three POSIX legs 27/27,
  dumps mixed #1 on MSVC). CI was dark again from 2026-08-17 (payments
  failed on the GitHub account; every workflow died in seconds) until
  2026-09-01; the first sample after (run 33593729167, 2026-09-02) reads
  102/108 — linux and arm64 27/27, macos-x86_64 26/27 (dumps mixed 1.04x),
  Windows 22/27 on a Milan runner. The wave-11 push (68d6e74) then drew a
  same-commit pair, 99/108 and 101/108; the wave-12/13 pushes (parse side:
  digit runs by word, the short-number head, the frame-owned key cursor)
  took the tracker to **106/108 on 37a96fb** — linux, arm64 and macos-x86_64
  all 27/27 in one run, Windows 25/27 with every parse row #1 and only
  dumps/dump mixed behind (1.11x/1.06x, Genoa). The cold-state probe then
  named that row's mechanism (the serializer's larger cache-cold entry
  footprint on x86) and wave 14's single mapping body took it: on b294ccd
  (105/108) Windows' whole `dumps` category reads #1 on a Milan runner for
  the first time, with only the file-write pair (dump flat 1.11x, dump
  mixed 1.05x) and macOS x86_64 dumps mixed (1.02x) in the coin band. The
  next verdict is `gh workflow run benchmark.yml` + `make bench-ci` per
  push, read as same-CPU pairs (linux-arm64 activates when the repo goes
  public).

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
