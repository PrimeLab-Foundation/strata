# Benchmark results - native-v1

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 33465c22eee0fda8e1b52898c16aac6982901a21
- python: 3.12.10
- implementation: CPython
- platform: macOS-15.7.9-x86_64-i386-64bit
- machine: x86_64
- processor: Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz
- compiler_flags: see build provenance
- provenance_schema: 1

Excluded libraries (not installed, or no native equivalent):
- ujson (native dataset): no native type support; not named

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| native.small | strata | 0.411 | 0.432 | 0.466 | 25.035 | 1.00x |
| native.small | orjson | 1.386 | 1.429 | 1.549 | 25.035 | 0.30x |
| native.small | msgspec | 0.510 | 0.541 | 0.601 | 25.035 | 0.80x |
| native.small | json | 8.582 | 8.768 | 9.503 | 25.035 | 0.05x |
| mixed.small (native flag) | strata (native=False) | 0.055 | 0.057 | 0.062 | 25.777 | - |
| mixed.small (native flag) | strata (native=True) | 0.053 | 0.056 | 0.062 | 25.777 | - |
| mixed.small (native flag) | orjson | 0.065 | 0.068 | 0.080 | 25.777 | - |
| mixed.small (native flag) | msgspec | 0.092 | 0.096 | 0.114 | 25.777 | - |
| mixed.small (native flag) | ujson | 0.424 | 0.429 | 0.436 | 25.777 | - |
| mixed.small (native flag) | json | 0.884 | 0.895 | 0.980 | 25.777 | - |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| native.small | strata | 0.629 | 0.712 | 1.472 | 25.551 | 1.00x |
| native.small | orjson | 1.657 | 1.722 | 2.173 | 25.551 | 0.41x |
| native.small | msgspec | 0.751 | 0.824 | 0.987 | 25.551 | 0.86x |
| native.small | json | 8.825 | 9.069 | 9.731 | 25.551 | 0.08x |

