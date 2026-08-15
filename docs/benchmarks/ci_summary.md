# CI benchmark standings by platform and architecture

Machine-written by `make bench-ci`. Do not hand-edit.

Goal: strata #1 in every row on every supported platform and architecture.

Ranks and ratios are computed within each platform's own CI run -- the
same-machine comparison the contract allows; absolute times are never
compared across platforms (docs/context/convention.md, Platform
supportability). Shared runners are noisy: this file tracks the goal, the
supportability tripwire stays the CI gate, and headline standings come
only from the quiet-machine protocol (docs/context/benchmarks.md).

- workflow: Benchmarks run 31905582605 (workflow_dispatch, conclusion: success)
- branch/commit: main @ 75b7e237b6eff570cac3f27b86c286132bd0ea86
- run date: 2026-08-15T20:02:07Z
- url: https://github.com/PrimeLab-Foundation/strata/actions/runs/31905582605

## Rows at #1, by category

Cells are "#1 rows / comparable rows" within that platform's own report.

| platform-arch | loads | dumps | load | load (ndjson) | dump | query | search | total |
|---|---|---|---|---|---|---|---|---|
| linux-x86_64 | 5/5 | 3/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 25/27 |
| macos-arm64 | 4/5 | 4/5 | 4/5 | 1/1 | 5/5 | 3/3 | 3/3 | 24/27 |
| macos-x86_64 | 5/5 | 4/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 26/27 |
| windows-x86_64 | 4/5 | 3/5 | 2/5 | 1/1 | 5/5 | 3/3 | 3/3 | 21/27 |

**Goal met on 0/4 platforms -- 12 row(s) to close.**

## Rows behind, by platform

### linux-x86_64 (python 3.12.13, repeats 10, commit 75b7e23)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| dumps | nested.json | 2/5 | 1.06x | orjson |
| dumps | mixed.json | 2/5 | 1.09x | orjson |

### macos-arm64 (python 3.12.10, repeats 10, commit 75b7e23)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| loads | flat.json | 3/6 | 1.19x | msgspec |
| dumps | mixed.json | 2/5 | 1.07x | orjson |
| load | wide_arrays.json | 2/5 | 1.07x | orjson |

### macos-x86_64 (python 3.12.10, repeats 10, commit 75b7e23)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| dumps | mixed.json | 2/5 | 1.13x | orjson |

### windows-x86_64 (python 3.12.10, repeats 10, commit 75b7e23)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| loads | flat.json | 2/5 | 1.06x | msgspec |
| dumps | wide_arrays.json | 2/5 | 1.01x | orjson |
| dumps | mixed.json | 2/5 | 1.14x | orjson |
| load | flat.json | 2/5 | 1.07x | msgspec |
| load | nested.json | 2/5 | 1.06x | msgspec |
| load | wide_arrays.json | 2/5 | 1.00x | orjson |
