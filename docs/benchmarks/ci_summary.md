# CI benchmark standings by platform and architecture

Machine-written by `make bench-ci`. Do not hand-edit.

Goal: strata #1 in every row on every supported platform and architecture.

Ranks and ratios are computed within each platform's own CI run -- the
same-machine comparison the contract allows; absolute times are never
compared across platforms (docs/context/convention.md, Platform
supportability). Shared runners are noisy: this file tracks the goal, the
supportability tripwire stays the CI gate, and headline standings come
only from the quiet-machine protocol (docs/context/benchmarks.md).

- workflow: Benchmarks run 33750537870 (workflow_dispatch, conclusion: success)
- branch/commit: main @ e8ac86038d1b680957d6dccd911a091107e92d85
- run date: 2026-09-03T11:35:46Z
- url: https://github.com/PrimeLab-Foundation/strata/actions/runs/33750537870

## Rows at #1, by category

Cells are "#1 rows / comparable rows" within that platform's own report.

| platform-arch | loads | dumps | load | load (ndjson) | dump | query | search | total |
|---|---|---|---|---|---|---|---|---|
| linux-x86_64 | 5/5 | 4/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 26/27 |
| macos-arm64 | 5/5 | 5/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 27/27 |
| macos-x86_64 | 4/5 | 5/5 | 5/5 | 1/1 | 3/5 | 3/3 | 3/3 | 24/27 |
| windows-x86_64 | 4/5 | 2/5 | 4/5 | 1/1 | 3/5 | 3/3 | 3/3 | 20/27 |

**Goal met on 1/4 platforms -- 11 row(s) to close.**

## Rows behind, by platform

### linux-x86_64 (python 3.12.14, repeats 10, commit e8ac860)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| dumps | mixed.json | 2/5 | 1.02x | orjson |

### macos-arm64 (python 3.12.10, repeats 10, commit e8ac860)

All rows #1.

### macos-x86_64 (python 3.12.10, repeats 10, commit e8ac860)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| loads | nested.json | 3/6 | 1.03x | orjson |
| dump | flat.json | 2/5 | 1.05x | orjson |
| dump | mixed.json | 2/5 | 1.16x | orjson |

### windows-x86_64 (python 3.12.10, repeats 10, commit e8ac860)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| loads | flat.json | 2/5 | 1.13x | msgspec |
| dumps | flat.json | 2/5 | 1.03x | orjson |
| dumps | wide_arrays.json | 2/5 | 1.09x | orjson |
| dumps | mixed.json | 2/5 | 1.07x | orjson |
| load | flat.json | 2/5 | 1.03x | msgspec |
| dump | wide_arrays.json | 2/5 | 1.02x | orjson |
| dump | mixed.json | 2/5 | 1.01x | orjson |
