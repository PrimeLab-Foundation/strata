# Benchmark results - native-v1

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: f2bdc410cb4a2e379388a180bc20efaaadc53f27
- python: 3.14.7
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit-Mach-O
- machine: arm64
- processor: Apple M1 Max
- compiler_flags: see build provenance
- provenance_schema: 1

Excluded libraries (not installed, or no native equivalent):
- ujson (native dataset): no native type support; not named

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| native.small | strata | 0.383 | 0.391 | 0.436 | 33.938 | 1.00x |
| native.small | orjson | 0.487 | 0.495 | 0.515 | 33.938 | 0.79x |
| native.small | msgspec | 0.263 | 0.269 | 0.295 | 33.938 | 1.46x |
| native.small | json | 3.067 | 3.121 | 3.360 | 33.938 | 0.13x |
| mixed.small (native flag) | strata (native=False) | 0.036 | 0.043 | 0.056 | 35.172 | - |
| mixed.small (native flag) | strata (native=True) | 0.034 | 0.042 | 0.048 | 35.172 | - |
| mixed.small (native flag) | orjson | 0.045 | 0.054 | 0.070 | 35.172 | - |
| mixed.small (native flag) | msgspec | 0.049 | 0.054 | 0.065 | 35.172 | - |
| mixed.small (native flag) | ujson | 0.220 | 0.228 | 0.238 | 35.172 | - |
| mixed.small (native flag) | json | 0.359 | 0.383 | 0.428 | 35.172 | - |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| native.small | strata | 0.574 | 0.610 | 0.690 | 34.922 | 1.00x |
| native.small | orjson | 0.677 | 0.717 | 0.782 | 34.922 | 0.85x |
| native.small | msgspec | 0.438 | 0.489 | 0.646 | 34.922 | 1.25x |
| native.small | json | 3.311 | 3.414 | 3.896 | 34.922 | 0.18x |

