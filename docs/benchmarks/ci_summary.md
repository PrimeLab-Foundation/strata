# CI benchmark standings by platform and architecture

Machine-written by `make bench-ci`. Do not hand-edit.

Goal: strata #1 in every row on every supported platform and architecture.

Ranks and ratios are computed within each platform's own CI run -- the
same-machine comparison the contract allows; absolute times are never
compared across platforms (docs/context/convention.md, Platform
supportability). Shared runners are noisy: this file tracks the goal, the
supportability tripwire stays the CI gate, and headline standings come
only from the quiet-machine protocol (docs/context/benchmarks.md).

- workflow: Benchmarks run 33803642381 (workflow_dispatch, conclusion: success)
- branch/commit: main @ 07e47e6b97199f44bff632688ccadd888a6a6827
- run date: 2026-09-03T20:40:35Z
- url: https://github.com/PrimeLab-Foundation/strata/actions/runs/33803642381

## Rows at #1, by category

Cells are "#1 rows / comparable rows" within that platform's own report.

| platform-arch | loads | dumps | load | load (ndjson) | dump | query | search | total |
|---|---|---|---|---|---|---|---|---|
| linux-x86_64 | 5/5 | 5/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 27/27 |
| macos-arm64 | 5/5 | 4/5 | 5/5 | 1/1 | 4/5 | 3/3 | 3/3 | 25/27 |
| macos-x86_64 | 5/5 | 5/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 27/27 |
| windows-x86_64 | 5/5 | 4/5 | 5/5 | 1/1 | 4/5 | 3/3 | 3/3 | 25/27 |

**Goal met on 2/4 platforms -- 4 row(s) to close.**

## Rows behind, by platform

### linux-x86_64 (python 3.12.14, repeats 10, commit 07e47e6)

All rows #1.

### macos-arm64 (python 3.12.10, repeats 10, commit 07e47e6)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| dumps | mixed.json | 3/5 | 1.82x | orjson |
| dump | mixed.json | 2/5 | 1.06x | orjson |

### macos-x86_64 (python 3.12.10, repeats 10, commit 07e47e6)

All rows #1.

### windows-x86_64 (python 3.12.10, repeats 10, commit 07e47e6)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| dumps | users.json | 2/5 | 1.08x | orjson |
| dump | mixed.json | 2/5 | 1.01x | orjson |
