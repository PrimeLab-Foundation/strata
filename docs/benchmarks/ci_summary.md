# CI benchmark standings by platform and architecture

Machine-written by `make bench-ci`. Do not hand-edit.

Goal: strata #1 in every row on every supported platform and architecture.

Ranks and ratios are computed within each platform's own CI run -- the
same-machine comparison the contract allows; absolute times are never
compared across platforms (docs/context/convention.md, Platform
supportability). Shared runners are noisy: this file tracks the goal, the
supportability tripwire stays the CI gate, and headline standings come
only from the quiet-machine protocol (docs/context/benchmarks.md).

- workflow: Benchmarks run 36585989834 (workflow_dispatch, conclusion: failure)
- branch/commit: exp/native-types @ 565fab210bb851772fcefe65082041084aeafc14
- run date: 2026-09-29T14:53:19Z
- url: https://github.com/PrimeLab-Foundation/strata/actions/runs/36585989834

## Rows at #1, by category

Cells are "#1 rows / declared rows" within that platform's own report.

| platform-arch | loads | dumps | load | load (ndjson) | dump | query | search | total |
|---|---|---|---|---|---|---|---|---|
| linux-arm64 | 5/5 | 5/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 27/27 |
| linux-x86_64 | 5/5 | 5/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 27/27 |
| macos-arm64 | 5/5 | 5/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 27/27 |
| macos-x86_64 | 5/5 | 5/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 27/27 |
| windows-x86_64 | 4/5 | 5/5 | 5/5 | 1/1 | 5/5 | 3/3 | 3/3 | 26/27 |

**Goal met on 4/5 platforms -- 1 row(s) to close.**

## Evidence

What the counts above are made of -- the declared platforms, and only
those. A platform contributes standings only when its report is valid and
contains every declared row; the goal cannot be met on evidence that is
absent (docs/context/benchmarks.md).

| platform-arch | status | declared rows measured | comparable rows | provenance |
|---|---|---|---|---|
| linux-arm64 | complete | 27/27 | 27 | verified against run 36585989834 (565fab2) |
| linux-x86_64 | complete | 27/27 | 27 | verified against run 36585989834 (565fab2) |
| macos-arm64 | complete | 27/27 | 27 | verified against run 36585989834 (565fab2) |
| macos-x86_64 | complete | 27/27 | 27 | verified against run 36585989834 (565fab2) |
| windows-x86_64 | complete | 27/27 | 27 | verified against run 36585989834 (565fab2) |

All 5 declared platforms reported valid, complete evidence.

## Rows behind, by platform

### linux-arm64 (python 3.12.14, repeats 10, commit 565fab210bb851772fcefe65082041084aeafc14)

All rows #1.

### linux-x86_64 (python 3.12.14, repeats 10, commit 565fab210bb851772fcefe65082041084aeafc14)

All rows #1.

### macos-arm64 (python 3.12.10, repeats 10, commit 565fab210bb851772fcefe65082041084aeafc14)

All rows #1.

### macos-x86_64 (python 3.12.10, repeats 10, commit 565fab210bb851772fcefe65082041084aeafc14)

All rows #1.

### windows-x86_64 (python 3.12.10, repeats 10, commit 565fab210bb851772fcefe65082041084aeafc14)

| section | dataset | rank | behind best | best rival |
|---|---|---|---|---|
| loads | flat.json | 2/5 | 1.01x | msgspec |

## native-v1

A separate declared scope (`harness.WORKLOADS['native-v1']`): strata's native types against the rivals that support them, plus the cost of the `native=` flag on a document with no native object. Counts toward neither the 135-row verdict nor any platform's canonical standings.

### linux-arm64 (python 3.12.14, commit 565fab210bb851772fcefe65082041084aeafc14)

| section | dataset | rank | ratio vs best rival | best rival |
|---|---|---|---|---|
| dump | native.small | 2/4 | 1.26x | msgspec |
| dumps | native.small | 2/4 | 1.47x | msgspec |

Cost of the `native=` flag on `mixed` (no native object present):

| dataset | library | median_ms |
|---|---|---|
| mixed.small (native flag) | json | 0.472 |
| mixed.small (native flag) | msgspec | 0.066 |
| mixed.small (native flag) | orjson | 0.056 |
| mixed.small (native flag) | strata (native=False) | 0.055 |
| mixed.small (native flag) | strata (native=True) | 0.057 |
| mixed.small (native flag) | ujson | 0.230 |

### linux-x86_64 (python 3.12.14, commit 565fab210bb851772fcefe65082041084aeafc14)

| section | dataset | rank | ratio vs best rival | best rival |
|---|---|---|---|---|
| dump | native.small | 2/4 | 1.34x | msgspec |
| dumps | native.small | 2/4 | 1.59x | msgspec |

Cost of the `native=` flag on `mixed` (no native object present):

| dataset | library | median_ms |
|---|---|---|
| mixed.small (native flag) | json | 0.503 |
| mixed.small (native flag) | msgspec | 0.073 |
| mixed.small (native flag) | orjson | 0.061 |
| mixed.small (native flag) | strata (native=False) | 0.053 |
| mixed.small (native flag) | strata (native=True) | 0.058 |
| mixed.small (native flag) | ujson | 0.223 |

### macos-arm64 (python 3.12.10, commit 565fab210bb851772fcefe65082041084aeafc14)

| section | dataset | rank | ratio vs best rival | best rival |
|---|---|---|---|---|
| dump | native.small | 2/4 | 1.31x | msgspec |
| dumps | native.small | 2/4 | 1.38x | msgspec |

Cost of the `native=` flag on `mixed` (no native object present):

| dataset | library | median_ms |
|---|---|---|
| mixed.small (native flag) | json | 0.356 |
| mixed.small (native flag) | msgspec | 0.043 |
| mixed.small (native flag) | orjson | 0.042 |
| mixed.small (native flag) | strata (native=False) | 0.029 |
| mixed.small (native flag) | strata (native=True) | 0.032 |
| mixed.small (native flag) | ujson | 0.158 |

### macos-x86_64 (python 3.12.10, commit 565fab210bb851772fcefe65082041084aeafc14)

| section | dataset | rank | ratio vs best rival | best rival |
|---|---|---|---|---|
| dump | native.small | 2/4 | 1.30x | msgspec |
| dumps | native.small | 2/4 | 1.48x | msgspec |

Cost of the `native=` flag on `mixed` (no native object present):

| dataset | library | median_ms |
|---|---|---|
| mixed.small (native flag) | json | 1.307 |
| mixed.small (native flag) | msgspec | 0.149 |
| mixed.small (native flag) | orjson | 0.112 |
| mixed.small (native flag) | strata (native=False) | 0.093 |
| mixed.small (native flag) | strata (native=True) | 0.093 |
| mixed.small (native flag) | ujson | 0.667 |

### windows-x86_64 (python 3.12.10, commit 565fab210bb851772fcefe65082041084aeafc14)

| section | dataset | rank | ratio vs best rival | best rival |
|---|---|---|---|---|
| dump | native.small | 2/4 | 1.12x | msgspec |
| dumps | native.small | 2/4 | 1.27x | msgspec |

Cost of the `native=` flag on `mixed` (no native object present):

| dataset | library | median_ms |
|---|---|---|
| mixed.small (native flag) | json | 0.505 |
| mixed.small (native flag) | msgspec | 0.089 |
| mixed.small (native flag) | orjson | 0.062 |
| mixed.small (native flag) | strata (native=False) | 0.060 |
| mixed.small (native flag) | strata (native=True) | 0.064 |
| mixed.small (native flag) | ujson | 0.228 |
