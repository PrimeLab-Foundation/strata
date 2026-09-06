# CI benchmark standings by platform and architecture

Machine-written by `make bench-ci`. Do not hand-edit.

Goal: strata #1 in every row on every supported platform and architecture.

Ranks and ratios are computed within each platform's own CI run -- the
same-machine comparison the contract allows; absolute times are never
compared across platforms (docs/context/convention.md, Platform
supportability). Shared runners are noisy: this file tracks the goal, the
supportability tripwire stays the CI gate, and headline standings come
only from the quiet-machine protocol (docs/context/benchmarks.md).

- workflow: Benchmarks run 34012087358 (workflow_dispatch, conclusion: success)
- branch/commit: main @ 32c5fa4a2385f5aec83ddd92b9e77faa70a25ebd
- run date: 2026-09-06T04:40:18Z
- url: https://github.com/PrimeLab-Foundation/strata/actions/runs/34012087358

## Rows at #1, by category

Cells are "#1 rows / comparable rows" within that platform's own report.

| platform-arch | loads | dumps | load | load (ndjson) | dump | query | search | total |
|---|---|---|---|---|---|---|---|---|
| linux-arm64 | 5/5 | 4/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 26/27 |
| linux-x86_64 | 5/5 | 5/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 27/27 |
| macos-arm64 | 5/5 | 5/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 27/27 |
| macos-x86_64 | 5/5 | 5/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 27/27 |
| windows-x86_64 | 5/5 | 5/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 27/27 |

**Goal met on 4/5 platforms -- 1 row(s) to close.**

## Rows behind, by platform

### linux-arm64 (python 3.12.14, repeats 10, commit 32c5fa4)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| dumps | mixed.json | 2/5 | 1.03x | orjson |

### linux-x86_64 (python 3.12.14, repeats 10, commit 32c5fa4)

All rows #1.

### macos-arm64 (python 3.12.10, repeats 10, commit 32c5fa4)

All rows #1.

### macos-x86_64 (python 3.12.10, repeats 10, commit 32c5fa4)

All rows #1.

### windows-x86_64 (python 3.12.10, repeats 10, commit 32c5fa4)

All rows #1.
