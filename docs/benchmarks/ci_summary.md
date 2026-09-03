# CI benchmark standings by platform and architecture

Machine-written by `make bench-ci`. Do not hand-edit.

Goal: strata #1 in every row on every supported platform and architecture.

Ranks and ratios are computed within each platform's own CI run -- the
same-machine comparison the contract allows; absolute times are never
compared across platforms (docs/context/convention.md, Platform
supportability). Shared runners are noisy: this file tracks the goal, the
supportability tripwire stays the CI gate, and headline standings come
only from the quiet-machine protocol (docs/context/benchmarks.md).

- workflow: Benchmarks run 33774537458 (workflow_dispatch, conclusion: success)
- branch/commit: main @ 49acca3191f2eef9b8ae83e494fa6bfe8cb24829
- run date: 2026-09-03T15:45:39Z
- url: https://github.com/PrimeLab-Foundation/strata/actions/runs/33774537458

## Rows at #1, by category

Cells are "#1 rows / comparable rows" within that platform's own report.

| platform-arch | loads | dumps | load | load (ndjson) | dump | query | search | total |
|---|---|---|---|---|---|---|---|---|
| linux-x86_64 | 5/5 | 3/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 25/27 |
| macos-arm64 | 5/5 | 5/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 27/27 |
| macos-x86_64 | 5/5 | 5/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 27/27 |
| windows-x86_64 | 5/5 | 4/5 | 4/5 | 1/1 | 4/5 | 3/3 | 3/3 | 24/27 |

**Goal met on 2/4 platforms -- 5 row(s) to close.**

## Rows behind, by platform

### linux-x86_64 (python 3.12.14, repeats 10, commit 49acca3)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| dumps | users.json | 2/5 | 1.00x | orjson |
| dumps | mixed.json | 2/5 | 1.10x | orjson |

### macos-arm64 (python 3.12.10, repeats 10, commit 49acca3)

All rows #1.

### macos-x86_64 (python 3.12.10, repeats 10, commit 49acca3)

All rows #1.

### windows-x86_64 (python 3.12.10, repeats 10, commit 49acca3)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| dumps | mixed.json | 2/5 | 1.09x | orjson |
| load | flat.json | 2/5 | 1.07x | msgspec |
| dump | mixed.json | 2/5 | 1.01x | orjson |
