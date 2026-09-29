# Benchmark results - native-v1

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 565fab210bb851772fcefe65082041084aeafc14
- python: 3.12.10
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit
- machine: arm64
- processor: Apple M2 Pro (Virtual)
- compiler_flags: see build provenance
- provenance_schema: 1

Excluded libraries (not installed, or no native equivalent):
- ujson (native dataset): no native type support; not named

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| native.small | strata | 0.342 | 0.361 | 0.442 | 32.375 | 1.00x |
| native.small | orjson | 0.417 | 0.473 | 0.514 | 32.375 | 0.76x |
| native.small | msgspec | 0.221 | 0.261 | 0.313 | 32.375 | 1.38x |
| native.small | json | 2.852 | 2.923 | 3.304 | 32.375 | 0.12x |
| mixed.small (native flag) | strata (native=False) | 0.027 | 0.029 | 0.039 | 32.906 | - |
| mixed.small (native flag) | strata (native=True) | 0.030 | 0.032 | 0.041 | 32.906 | - |
| mixed.small (native flag) | orjson | 0.036 | 0.042 | 0.057 | 32.906 | - |
| mixed.small (native flag) | msgspec | 0.042 | 0.043 | 0.104 | 32.906 | - |
| mixed.small (native flag) | ujson | 0.153 | 0.158 | 0.198 | 32.906 | - |
| mixed.small (native flag) | json | 0.320 | 0.356 | 0.378 | 32.906 | - |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| native.small | strata | 0.432 | 0.466 | 0.511 | 32.719 | 1.00x |
| native.small | orjson | 0.498 | 0.523 | 0.596 | 32.719 | 0.89x |
| native.small | msgspec | 0.305 | 0.355 | 0.392 | 32.719 | 1.31x |
| native.small | json | 3.004 | 3.074 | 3.246 | 32.719 | 0.15x |

