# Benchmark results - native-v1

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 82e3fa5f24c38cfa1150440f0cf5da6c286c01cd
- python: 3.12.10
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit
- machine: arm64
- processor: Apple M1 (Virtual)
- compiler_flags: see build provenance
- provenance_schema: 1

Excluded libraries (not installed, or no native equivalent):
- ujson (native dataset): no native type support; not named

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| native.small | strata | 0.375 | 0.422 | 0.572 | 32.812 | 1.00x |
| native.small | orjson | 0.468 | 0.542 | 0.836 | 32.812 | 0.78x |
| native.small | msgspec | 0.266 | 0.319 | 0.397 | 32.812 | 1.32x |
| native.small | json | 3.038 | 3.632 | 4.527 | 32.812 | 0.12x |
| mixed.small (native flag) | strata (native=False) | 0.030 | 0.034 | 0.040 | 33.609 | - |
| mixed.small (native flag) | strata (native=True) | 0.033 | 0.037 | 0.047 | 33.609 | - |
| mixed.small (native flag) | orjson | 0.039 | 0.043 | 0.066 | 33.609 | - |
| mixed.small (native flag) | msgspec | 0.045 | 0.050 | 0.069 | 33.609 | - |
| mixed.small (native flag) | ujson | 0.162 | 0.172 | 0.195 | 33.609 | - |
| mixed.small (native flag) | json | 0.330 | 0.344 | 0.414 | 33.609 | - |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| native.small | strata | 0.519 | 0.536 | 0.663 | 33.391 | 1.00x |
| native.small | orjson | 0.587 | 0.643 | 0.766 | 33.391 | 0.83x |
| native.small | msgspec | 0.406 | 0.427 | 0.575 | 33.391 | 1.25x |
| native.small | json | 3.106 | 3.240 | 3.526 | 33.391 | 0.17x |

