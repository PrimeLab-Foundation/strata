# CI benchmark standings by platform and architecture

Machine-written by `make bench-ci`. Do not hand-edit.

Goal: strata #1 in every row on every supported platform and architecture.

Ranks and ratios are computed within each platform's own CI run -- the
same-machine comparison the contract allows; absolute times are never
compared across platforms (docs/context/convention.md, Platform
supportability). Shared runners are noisy: this file tracks the goal, the
supportability tripwire stays the CI gate, and headline standings come
only from the quiet-machine protocol (docs/context/benchmarks.md).

- workflow: Benchmarks run 31896277797 (workflow_dispatch, conclusion: success)
- branch/commit: main @ 6520b15dbb294d3dc978ccbe8e898450b7f1b7f2
- run date: 2026-08-15T16:43:01Z
- url: https://github.com/PrimeLab-Foundation/strata/actions/runs/31896277797

## Rows at #1, by category

Cells are "#1 rows / comparable rows" within that platform's own report.

| platform-arch | loads | dumps | load | load (ndjson) | dump | query | search | total |
|---|---|---|---|---|---|---|---|---|
| linux-x86_64 | 5/5 | 3/5 | 5/5 | 1/1 | 3/5 | 3/3 | 3/3 | 23/27 |
| macos-arm64 | 5/5 | 5/5 | 5/5 | 1/1 | 4/5 | 3/3 | 3/3 | 26/27 |
| macos-x86_64 | 5/5 | 4/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 26/27 |
| windows-x86_64 | 4/5 | 3/5 | 3/5 | 1/1 | 3/5 | 3/3 | 3/3 | 20/27 |

**Goal met on 0/4 platforms -- 13 row(s) to close.**

## Rows behind, by platform

### linux-x86_64 (python 3.12.13, repeats 10, commit 6520b15)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| dumps | wide_arrays.json | 2/5 | 1.02x | orjson |
| dumps | mixed.json | 2/5 | 1.06x | orjson |
| dump | wide_arrays.json | 2/5 | 1.02x | orjson |
| dump | mixed.json | 2/5 | 1.01x | orjson |

### macos-arm64 (python 3.12.10, repeats 10, commit 6520b15)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| dump | mixed.json | 2/5 | 1.04x | orjson |

### macos-x86_64 (python 3.12.10, repeats 10, commit 6520b15)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| dumps | mixed.json | 2/5 | 1.16x | orjson |

### windows-x86_64 (python 3.12.10, repeats 10, commit 6520b15)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| loads | flat.json | 3/5 | 1.13x | orjson |
| dumps | wide_arrays.json | 2/5 | 1.23x | orjson |
| dumps | mixed.json | 2/5 | 1.23x | orjson |
| load | flat.json | 2/5 | 1.11x | msgspec |
| load | wide_arrays.json | 2/5 | 1.03x | orjson |
| dump | nested.json | 2/5 | 1.15x | orjson |
| dump | mixed.json | 3/5 | 1.09x | msgspec |
