# Benchmark results - native-v1

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 271a2a0864aa430c4690ca9b609e4201e13cfb51
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 25 Model 17 Stepping 1, AuthenticAMD
- compiler_flags: see build provenance
- provenance_schema: 1

Excluded libraries (not installed, or no native equivalent):
- ujson (native dataset): no native type support; not named

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| native.small | strata | 0.259 | 0.263 | 0.301 | 33.090 | 1.00x |
| native.small | orjson | 0.624 | 0.635 | 0.664 | 33.090 | 0.41x |
| native.small | msgspec | 0.309 | 0.313 | 0.470 | 33.090 | 0.84x |
| native.small | json | 3.805 | 3.843 | 4.438 | 33.090 | 0.07x |
| mixed.small (native flag) | strata (native=False) | 0.043 | 0.044 | 0.059 | 34.074 | - |
| mixed.small (native flag) | strata (native=True) | 0.044 | 0.045 | 0.067 | 34.074 | - |
| mixed.small (native flag) | orjson | 0.041 | 0.042 | 0.060 | 34.074 | - |
| mixed.small (native flag) | msgspec | 0.061 | 0.064 | 0.097 | 34.074 | - |
| mixed.small (native flag) | ujson | 0.189 | 0.193 | 0.239 | 34.074 | - |
| mixed.small (native flag) | json | 0.377 | 0.381 | 0.418 | 34.074 | - |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| native.small | strata | 0.500 | 0.550 | 0.658 | 33.941 | 1.00x |
| native.small | orjson | 0.896 | 0.943 | 1.063 | 33.941 | 0.58x |
| native.small | msgspec | 0.574 | 0.605 | 0.678 | 33.941 | 0.91x |
| native.small | json | 4.134 | 4.200 | 6.217 | 33.941 | 0.13x |

