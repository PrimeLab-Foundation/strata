# CI benchmark standings by platform and architecture

Machine-written by `make bench-ci`. Do not hand-edit.

Goal: strata #1 in every row on every supported platform and architecture.

Ranks and ratios are computed within each platform's own CI run -- the
same-machine comparison the contract allows; absolute times are never
compared across platforms (docs/context/convention.md, Platform
supportability). Shared runners are noisy: this file tracks the goal, the
supportability tripwire stays the CI gate, and headline standings come
only from the quiet-machine protocol (docs/context/benchmarks.md).

- workflow: Benchmarks run 33778407659 (workflow_dispatch, conclusion: success)
- branch/commit: main @ a4f3b0e2f9bd40a8d48a430d01648dd6172737b0
- run date: 2026-09-03T16:24:17Z
- url: https://github.com/PrimeLab-Foundation/strata/actions/runs/33778407659

## Rows at #1, by category

Cells are "#1 rows / comparable rows" within that platform's own report.

| platform-arch | loads | dumps | load | load (ndjson) | dump | query | search | total |
|---|---|---|---|---|---|---|---|---|
| linux-x86_64 | 5/5 | 5/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 27/27 |
| macos-arm64 | 5/5 | 5/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 27/27 |
| macos-x86_64 | 3/5 | 5/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 25/27 |
| windows-x86_64 | 4/5 | 4/5 | 4/5 | 1/1 | 4/5 | 3/3 | 3/3 | 23/27 |

**Goal met on 2/4 platforms -- 6 row(s) to close.**

## Rows behind, by platform

### linux-x86_64 (python 3.12.14, repeats 10, commit a4f3b0e)

All rows #1.

### macos-arm64 (python 3.12.10, repeats 10, commit a4f3b0e)

All rows #1.

### macos-x86_64 (python 3.12.10, repeats 10, commit a4f3b0e)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| loads | flat.json | 2/6 | 1.09x | orjson |
| loads | nested.json | 2/6 | 1.03x | orjson |

### windows-x86_64 (python 3.12.10, repeats 10, commit a4f3b0e)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| loads | flat.json | 2/5 | 1.03x | msgspec |
| dumps | mixed.json | 2/5 | 1.26x | orjson |
| load | flat.json | 2/5 | 1.03x | msgspec |
| dump | mixed.json | 2/5 | 1.05x | orjson |
