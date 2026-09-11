# CI benchmark standings by platform and architecture

Machine-written by `make bench-ci`. Do not hand-edit.

Goal: strata #1 in every row on every supported platform and architecture.

Ranks and ratios are computed within each platform's own CI run -- the
same-machine comparison the contract allows; absolute times are never
compared across platforms (docs/context/convention.md, Platform
supportability). Shared runners are noisy: this file tracks the goal, the
supportability tripwire stays the CI gate, and headline standings come
only from the quiet-machine protocol (docs/context/benchmarks.md).

- workflow: Benchmarks run 34590027501 (workflow_dispatch, conclusion: success)
- branch/commit: exp/p9-fused-dicts @ c20ac86eedff410e10c973bc3b1f19f6e9a5f56e
- run date: 2026-09-11T10:36:17Z
- url: https://github.com/PrimeLab-Foundation/strata/actions/runs/34590027501

## Rows at #1, by category

Cells are "#1 rows / declared rows" within that platform's own report.

| platform-arch | loads | dumps | load | load (ndjson) | dump | query | search | total |
|---|---|---|---|---|---|---|---|---|
| linux-arm64 | 5/5 | 5/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 27/27 |
| linux-x86_64 | 5/5 | 4/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 26/27 |
| macos-arm64 | 5/5 | 5/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 27/27 |
| macos-x86_64 | 5/5 | 5/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 27/27 |
| windows-x86_64 | 5/5 | 4/5 | 5/5 | 1/1 | 4/5 | 3/3 | 3/3 | 25/27 |

**Goal met on 3/5 platforms -- 3 row(s) to close.**

## Evidence

What the counts above are made of -- the declared platforms, and only
those. A platform contributes standings only when its report is valid and
contains every declared row; the goal cannot be met on evidence that is
absent (docs/context/benchmarks.md).

| platform-arch | status | declared rows measured | comparable rows | provenance |
|---|---|---|---|---|
| linux-arm64 | complete | 27/27 | 27 | verified against run 34590027501 (c20ac86) |
| linux-x86_64 | complete | 27/27 | 27 | verified against run 34590027501 (c20ac86) |
| macos-arm64 | complete | 27/27 | 27 | verified against run 34590027501 (c20ac86) |
| macos-x86_64 | complete | 27/27 | 27 | verified against run 34590027501 (c20ac86) |
| windows-x86_64 | complete | 27/27 | 27 | verified against run 34590027501 (c20ac86) |

All 5 declared platforms reported valid, complete evidence.

## Rows behind, by platform

### linux-arm64 (python 3.12.14, repeats 10, commit c20ac86eedff410e10c973bc3b1f19f6e9a5f56e)

All rows #1.

### linux-x86_64 (python 3.12.14, repeats 10, commit c20ac86eedff410e10c973bc3b1f19f6e9a5f56e)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| dumps | mixed.json | 2/5 | 1.07x | orjson |

### macos-arm64 (python 3.12.10, repeats 10, commit c20ac86eedff410e10c973bc3b1f19f6e9a5f56e)

All rows #1.

### macos-x86_64 (python 3.12.10, repeats 10, commit c20ac86eedff410e10c973bc3b1f19f6e9a5f56e)

All rows #1.

### windows-x86_64 (python 3.12.10, repeats 10, commit c20ac86eedff410e10c973bc3b1f19f6e9a5f56e)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| dumps | mixed.json | 2/5 | 1.03x | orjson |
| dump | mixed.json | 2/5 | 1.03x | orjson |
