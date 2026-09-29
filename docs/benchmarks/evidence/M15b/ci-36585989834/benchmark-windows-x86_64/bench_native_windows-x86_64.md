# Benchmark results - native-v1

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 565fab210bb851772fcefe65082041084aeafc14
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: Intel64 Family 6 Model 207 Stepping 2, GenuineIntel
- compiler_flags: see build provenance
- provenance_schema: 1

Excluded libraries (not installed, or no native equivalent):
- ujson (native dataset): no native type support; not named

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| native.small | strata | 0.532 | 0.536 | 0.590 | 33.297 | 1.00x |
| native.small | orjson | 0.669 | 0.692 | 0.743 | 33.297 | 0.77x |
| native.small | msgspec | 0.407 | 0.423 | 0.617 | 33.297 | 1.27x |
| native.small | json | 4.624 | 4.680 | 4.778 | 33.297 | 0.11x |
| mixed.small (native flag) | strata (native=False) | 0.059 | 0.060 | 0.064 | 34.090 | - |
| mixed.small (native flag) | strata (native=True) | 0.061 | 0.064 | 0.068 | 34.090 | - |
| mixed.small (native flag) | orjson | 0.059 | 0.062 | 0.065 | 34.090 | - |
| mixed.small (native flag) | msgspec | 0.085 | 0.089 | 0.118 | 34.090 | - |
| mixed.small (native flag) | ujson | 0.225 | 0.228 | 0.301 | 34.090 | - |
| mixed.small (native flag) | json | 0.486 | 0.505 | 0.557 | 34.090 | - |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| native.small | strata | 0.882 | 0.937 | 1.121 | 33.969 | 1.00x |
| native.small | orjson | 1.060 | 1.132 | 1.162 | 33.969 | 0.83x |
| native.small | msgspec | 0.773 | 0.838 | 0.905 | 33.969 | 1.12x |
| native.small | json | 5.089 | 5.145 | 9.042 | 33.969 | 0.18x |

