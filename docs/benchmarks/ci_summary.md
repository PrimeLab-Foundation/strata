# CI benchmark standings by platform and architecture

Machine-written by `make bench-ci`. Do not hand-edit.

Goal: strata #1 in every row on every supported platform and architecture.

Ranks and ratios are computed within each platform's own CI run -- the
same-machine comparison the contract allows; absolute times are never
compared across platforms (docs/context/convention.md, Platform
supportability). Shared runners are noisy: this file tracks the goal, the
supportability tripwire stays the CI gate, and headline standings come
only from the quiet-machine protocol (docs/context/benchmarks.md).

- workflow: Benchmarks run 33730386401 (workflow_dispatch, conclusion: success)
- branch/commit: main @ 6ef7acf8297e98f5500670b052b1b0abe957c8fa
- run date: 2026-09-03T07:52:50Z
- url: https://github.com/PrimeLab-Foundation/strata/actions/runs/33730386401

## Rows at #1, by category

Cells are "#1 rows / comparable rows" within that platform's own report.

| platform-arch | loads | dumps | load | load (ndjson) | dump | query | search | total |
|---|---|---|---|---|---|---|---|---|
| linux-x86_64 | 5/5 | 5/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 27/27 |
| macos-arm64 | 5/5 | 5/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 27/27 |
| macos-x86_64 | 5/5 | 4/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 26/27 |
| windows-x86_64 | 4/5 | 4/5 | 5/5 | 1/1 | 3/5 | 3/3 | 3/3 | 23/27 |

**Goal met on 2/4 platforms -- 5 row(s) to close.**

## Rows behind, by platform

### linux-x86_64 (python 3.12.14, repeats 10, commit 6ef7acf)

All rows #1.

### macos-arm64 (python 3.12.10, repeats 10, commit 6ef7acf)

All rows #1.

### macos-x86_64 (python 3.12.10, repeats 10, commit 6ef7acf)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| dumps | mixed.json | 2/5 | 1.05x | orjson |

### windows-x86_64 (python 3.12.10, repeats 10, commit 6ef7acf)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| loads | nested.json | 2/5 | 1.05x | msgspec |
| dumps | mixed.json | 2/5 | 1.08x | orjson |
| dump | flat.json | 2/5 | 1.01x | orjson |
| dump | mixed.json | 3/5 | 1.19x | msgspec |
