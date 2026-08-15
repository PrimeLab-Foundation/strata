# CI benchmark standings by platform and architecture

Machine-written by `make bench-ci`. Do not hand-edit.

Goal: strata #1 in every row on every supported platform and architecture.

Ranks and ratios are computed within each platform's own CI run -- the
same-machine comparison the contract allows; absolute times are never
compared across platforms (docs/context/convention.md, Platform
supportability). Shared runners are noisy: this file tracks the goal, the
supportability tripwire stays the CI gate, and headline standings come
only from the quiet-machine protocol (docs/context/benchmarks.md).

- workflow: Benchmarks run 31882040667 (workflow_dispatch, conclusion: success)
- branch/commit: main @ dc844752a4e8deb6d2cb995d3aaad6947e0b5145
- run date: 2026-08-15T11:26:29Z
- url: https://github.com/PrimeLab-Foundation/strata/actions/runs/31882040667

## Rows at #1, by category

Cells are "#1 rows / comparable rows" within that platform's own report.

| platform-arch | loads | dumps | load | load (ndjson) | dump | query | search | total |
|---|---|---|---|---|---|---|---|---|
| linux-x86_64 | 5/5 | 2/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 24/27 |
| macos-arm64 | 4/5 | 3/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 24/27 |
| macos-x86_64 | 4/5 | 1/5 | 4/5 | 1/1 | 2/5 | 3/3 | 3/3 | 18/27 |
| windows-x86_64 | 2/5 | 0/5 | 2/5 | 1/1 | 1/5 | 3/3 | 3/3 | 12/27 |

**Goal met on 0/4 platforms -- 30 row(s) to close.**

## Rows behind, by platform

### linux-x86_64 (python 3.12.13, repeats 10, commit dc84475)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| dumps | users.json | 2/5 | 1.00x | orjson |
| dumps | nested.json | 2/5 | 1.01x | orjson |
| dumps | mixed.json | 2/5 | 1.13x | orjson |

### macos-arm64 (python 3.12.10, repeats 10, commit dc84475)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| loads | wide_arrays.json | 2/6 | 1.09x | orjson |
| dumps | flat.json | 2/5 | 1.04x | orjson |
| dumps | mixed.json | 3/5 | 1.24x | orjson |

### macos-x86_64 (python 3.12.10, repeats 10, commit dc84475)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| loads | flat.json | 2/6 | 1.03x | orjson |
| dumps | users.json | 2/5 | 1.25x | orjson |
| dumps | flat.json | 2/5 | 1.07x | orjson |
| dumps | nested.json | 2/5 | 1.10x | orjson |
| dumps | mixed.json | 2/5 | 1.51x | orjson |
| load | flat.json | 2/5 | 1.02x | orjson |
| dump | users.json | 2/5 | 1.19x | orjson |
| dump | flat.json | 2/5 | 1.08x | orjson |
| dump | nested.json | 2/5 | 1.00x | orjson |

### windows-x86_64 (python 3.12.10, repeats 10, commit dc84475)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| loads | flat.json | 2/5 | 1.06x | orjson |
| loads | wide_arrays.json | 2/5 | 1.01x | orjson |
| loads | mixed.json | 2/5 | 1.08x | orjson |
| dumps | users.json | 2/5 | 1.13x | orjson |
| dumps | flat.json | 2/5 | 1.23x | orjson |
| dumps | nested.json | 2/5 | 1.27x | orjson |
| dumps | wide_arrays.json | 2/5 | 1.04x | orjson |
| dumps | mixed.json | 3/5 | 1.47x | orjson |
| load | flat.json | 3/5 | 1.16x | msgspec |
| load | nested.json | 2/5 | 1.00x | orjson |
| load | wide_arrays.json | 3/5 | 1.08x | orjson |
| dump | users.json | 2/5 | 1.19x | orjson |
| dump | flat.json | 3/5 | 1.22x | orjson |
| dump | nested.json | 2/5 | 1.20x | orjson |
| dump | mixed.json | 3/5 | 1.30x | orjson |
