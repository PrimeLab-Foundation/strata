# Benchmark results - native-v1

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 33465c22eee0fda8e1b52898c16aac6982901a21
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: AMD EPYC 7763 64-Core Processor
- compiler_flags: see build provenance
- provenance_schema: 1

Excluded libraries (not installed, or no native equivalent):
- ujson (native dataset): no native type support; not named

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| native.small | strata | 0.324 | 0.334 | 0.364 | 30.066 | 1.00x |
| native.small | orjson | 0.943 | 0.960 | 0.971 | 30.066 | 0.35x |
| native.small | msgspec | 0.389 | 0.396 | 0.416 | 30.066 | 0.84x |
| native.small | json | 5.524 | 5.578 | 6.188 | 30.066 | 0.06x |
| mixed.small (native flag) | strata (native=False) | 0.052 | 0.054 | 0.057 | 30.727 | - |
| mixed.small (native flag) | strata (native=True) | 0.051 | 0.054 | 0.066 | 30.727 | - |
| mixed.small (native flag) | orjson | 0.055 | 0.057 | 0.070 | 30.727 | - |
| mixed.small (native flag) | msgspec | 0.069 | 0.071 | 0.075 | 30.727 | - |
| mixed.small (native flag) | ujson | 0.230 | 0.233 | 0.251 | 30.727 | - |
| mixed.small (native flag) | json | 0.489 | 0.507 | 0.526 | 30.727 | - |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| native.small | strata | 0.504 | 0.552 | 2.214 | 30.660 | 1.00x |
| native.small | orjson | 1.138 | 1.189 | 1.295 | 30.660 | 0.46x |
| native.small | msgspec | 0.585 | 0.650 | 1.658 | 30.660 | 0.85x |
| native.small | json | 5.676 | 5.738 | 5.885 | 30.660 | 0.10x |

