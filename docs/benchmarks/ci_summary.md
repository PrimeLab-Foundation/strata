# CI benchmark standings by platform and architecture

Machine-written by `make bench-ci`. Do not hand-edit.

Goal: strata #1 in every row on every supported platform and architecture.

Ranks and ratios are computed within each platform's own CI run -- the
same-machine comparison the contract allows; absolute times are never
compared across platforms (docs/context/convention.md, Platform
supportability). Shared runners are noisy: this file tracks the goal, the
supportability tripwire stays the CI gate, and headline standings come
only from the quiet-machine protocol (docs/context/benchmarks.md).

- workflow: Benchmarks run 31890685241 (workflow_dispatch, conclusion: success)
- branch/commit: main @ 4d4a4be8b3c5c1218c775e386cff7f2d07f7cf0f
- run date: 2026-08-15T14:43:07Z
- url: https://github.com/PrimeLab-Foundation/strata/actions/runs/31890685241

## Rows at #1, by category

Cells are "#1 rows / comparable rows" within that platform's own report.

| platform-arch | loads | dumps | load | load (ndjson) | dump | query | search | total |
|---|---|---|---|---|---|---|---|---|
| linux-x86_64 | 5/5 | 4/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 26/27 |
| macos-arm64 | 5/5 | 5/5 | 4/5 | 1/1 | 5/5 | 3/3 | 3/3 | 26/27 |
| macos-x86_64 | 5/5 | 4/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 26/27 |
| windows-x86_64 | 4/5 | 3/5 | 4/5 | 1/1 | 3/5 | 3/3 | 3/3 | 21/27 |

**Goal met on 0/4 platforms -- 9 row(s) to close.**

## Rows behind, by platform

### linux-x86_64 (python 3.12.13, repeats 10, commit 4d4a4be)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| dumps | mixed.json | 2/5 | 1.09x | orjson |

### macos-arm64 (python 3.12.10, repeats 10, commit 4d4a4be)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| load | flat.json | 2/5 | 1.05x | msgspec |

### macos-x86_64 (python 3.12.10, repeats 10, commit 4d4a4be)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| dumps | mixed.json | 2/5 | 1.10x | orjson |

### windows-x86_64 (python 3.12.10, repeats 10, commit 4d4a4be)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| loads | flat.json | 3/5 | 1.13x | orjson |
| dumps | flat.json | 2/5 | 1.06x | orjson |
| dumps | mixed.json | 2/5 | 1.23x | orjson |
| load | wide_arrays.json | 3/5 | 1.09x | orjson |
| dump | flat.json | 2/5 | 1.07x | orjson |
| dump | mixed.json | 3/5 | 1.09x | orjson |
