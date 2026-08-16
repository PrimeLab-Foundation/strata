# CI benchmark standings by platform and architecture

Machine-written by `make bench-ci`. Do not hand-edit.

Goal: strata #1 in every row on every supported platform and architecture.

Ranks and ratios are computed within each platform's own CI run -- the
same-machine comparison the contract allows; absolute times are never
compared across platforms (docs/context/convention.md, Platform
supportability). Shared runners are noisy: this file tracks the goal, the
supportability tripwire stays the CI gate, and headline standings come
only from the quiet-machine protocol (docs/context/benchmarks.md).

- workflow: Benchmarks run 31962292215 (workflow_dispatch, conclusion: success)
- branch/commit: main @ 208e6f92aa62da49d23c81532ed8d96d70050235
- run date: 2026-08-16T17:40:13Z
- url: https://github.com/PrimeLab-Foundation/strata/actions/runs/31962292215

## Rows at #1, by category

Cells are "#1 rows / comparable rows" within that platform's own report.

| platform-arch | loads | dumps | load | load (ndjson) | dump | query | search | total |
|---|---|---|---|---|---|---|---|---|
| linux-x86_64 | 5/5 | 5/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 27/27 |
| macos-arm64 | 5/5 | 5/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 27/27 |
| macos-x86_64 | 5/5 | 5/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 27/27 |
| windows-x86_64 | 4/5 | 4/5 | 3/5 | 1/1 | 5/5 | 3/3 | 3/3 | 23/27 |

**Goal met on 3/4 platforms -- 4 row(s) to close.**

## Rows behind, by platform

### linux-x86_64 (python 3.12.13, repeats 10, commit 208e6f9)

All rows #1.

### macos-arm64 (python 3.12.10, repeats 10, commit 208e6f9)

All rows #1.

### macos-x86_64 (python 3.12.10, repeats 10, commit 208e6f9)

All rows #1.

### windows-x86_64 (python 3.12.10, repeats 10, commit 208e6f9)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| loads | mixed.json | 3/5 | 1.26x | orjson |
| dumps | users.json | 2/5 | 1.08x | orjson |
| load | flat.json | 3/5 | 1.15x | msgspec |
| load | wide_arrays.json | 3/5 | 1.27x | orjson |
