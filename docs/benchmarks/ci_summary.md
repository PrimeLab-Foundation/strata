# CI benchmark standings by platform and architecture

Machine-written by `make bench-ci`. Do not hand-edit.

Goal: strata #1 in every row on every supported platform and architecture.

Ranks and ratios are computed within each platform's own CI run -- the
same-machine comparison the contract allows; absolute times are never
compared across platforms (docs/context/convention.md, Platform
supportability). Shared runners are noisy: this file tracks the goal, the
supportability tripwire stays the CI gate, and headline standings come
only from the quiet-machine protocol (docs/context/benchmarks.md).

- workflow: Benchmarks run 33955093951 (workflow_dispatch, conclusion: success)
- branch/commit: main @ b7f31bbf9786d1c64f017b8151f5260282478b7f
- run date: 2026-09-05T08:22:46Z
- url: https://github.com/PrimeLab-Foundation/strata/actions/runs/33955093951

## Rows at #1, by category

Cells are "#1 rows / comparable rows" within that platform's own report.

| platform-arch | loads | dumps | load | load (ndjson) | dump | query | search | total |
|---|---|---|---|---|---|---|---|---|
| linux-arm64 | 4/5 | 4/5 | 4/5 | 1/1 | 5/5 | 3/3 | 3/3 | 24/27 |
| linux-x86_64 | 5/5 | 5/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 27/27 |
| macos-arm64 | 5/5 | 5/5 | 5/5 | 1/1 | 4/5 | 3/3 | 3/3 | 26/27 |
| macos-x86_64 | 5/5 | 5/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 27/27 |
| windows-x86_64 | 5/5 | 5/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 27/27 |

**Goal met on 3/5 platforms -- 4 row(s) to close.**

## Rows behind, by platform

### linux-arm64 (python 3.12.14, repeats 10, commit b7f31bb)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| loads | wide_arrays.json | 2/6 | 1.08x | orjson |
| dumps | mixed.json | 2/5 | 1.03x | orjson |
| load | wide_arrays.json | 2/5 | 1.07x | orjson |

### linux-x86_64 (python 3.12.14, repeats 10, commit b7f31bb)

All rows #1.

### macos-arm64 (python 3.12.10, repeats 10, commit b7f31bb)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| dump | flat.json | 2/5 | 1.08x | orjson |

### macos-x86_64 (python 3.12.10, repeats 10, commit b7f31bb)

All rows #1.

### windows-x86_64 (python 3.12.10, repeats 10, commit b7f31bb)

All rows #1.
