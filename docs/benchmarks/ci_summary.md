# CI benchmark standings by platform and architecture

Machine-written by `make bench-ci`. Do not hand-edit.

Goal: strata #1 in every row on every supported platform and architecture.

Ranks and ratios are computed within each platform's own CI run -- the
same-machine comparison the contract allows; absolute times are never
compared across platforms (docs/context/convention.md, Platform
supportability). Shared runners are noisy: this file tracks the goal, the
supportability tripwire stays the CI gate, and headline standings come
only from the quiet-machine protocol (docs/context/benchmarks.md).

- workflow: Benchmarks run 34143751498 (workflow_dispatch, conclusion: success)
- branch/commit: work/september-7-plan @ 75cfb422ede723193706740309d7509adf327f2b
- run date: 2026-09-07T16:32:37Z
- url: https://github.com/PrimeLab-Foundation/strata/actions/runs/34143751498

## Rows at #1, by category

Cells are "#1 rows / declared rows" within that platform's own report.

| platform-arch | loads | dumps | load | load (ndjson) | dump | query | search | total |
|---|---|---|---|---|---|---|---|---|
| linux-arm64 | 5/5 | 4/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 26/27 |
| linux-x86_64 | 5/5 | 5/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 27/27 |
| macos-arm64 | 5/5 | 5/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 27/27 |
| macos-x86_64 | 5/5 | 5/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 27/27 |
| windows-x86_64 | 5/5 | 4/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 26/27 |

**Goal met on 3/5 platforms -- 2 row(s) to close.**

## Evidence

What the counts above are made of -- the declared platforms, and only
those. A platform contributes standings only when its report is valid and
contains every declared row; the goal cannot be met on evidence that is
absent (docs/context/benchmarks.md).

| platform-arch | status | declared rows measured | comparable rows | provenance |
|---|---|---|---|---|
| linux-arm64 | complete | 27/27 | 27 | verified against run 34143751498 (75cfb42) |
| linux-x86_64 | complete | 27/27 | 27 | verified against run 34143751498 (75cfb42) |
| macos-arm64 | complete | 27/27 | 27 | verified against run 34143751498 (75cfb42) |
| macos-x86_64 | complete | 27/27 | 27 | verified against run 34143751498 (75cfb42) |
| windows-x86_64 | complete | 27/27 | 27 | verified against run 34143751498 (75cfb42) |

All 5 declared platforms reported valid, complete evidence.

## Rows behind, by platform

### linux-arm64 (python 3.12.14, repeats 10, commit 75cfb422ede723193706740309d7509adf327f2b)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| dumps | mixed.json | 2/5 | 1.02x | orjson |

### linux-x86_64 (python 3.12.14, repeats 10, commit 75cfb422ede723193706740309d7509adf327f2b)

All rows #1.

### macos-arm64 (python 3.12.10, repeats 10, commit 75cfb422ede723193706740309d7509adf327f2b)

All rows #1.

### macos-x86_64 (python 3.12.10, repeats 10, commit 75cfb422ede723193706740309d7509adf327f2b)

All rows #1.

### windows-x86_64 (python 3.12.10, repeats 10, commit 75cfb422ede723193706740309d7509adf327f2b)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| dumps | mixed.json | 2/5 | 1.01x | orjson |
