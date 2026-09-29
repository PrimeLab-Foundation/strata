# Benchmark results - native-v1

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 565fab210bb851772fcefe65082041084aeafc14
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: AMD EPYC 9V74 80-Core Processor
- compiler_flags: see build provenance
- provenance_schema: 1

Excluded libraries (not installed, or no native equivalent):
- ujson (native dataset): no native type support; not named

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| native.small | strata | 0.598 | 0.619 | 0.625 | 29.992 | 1.00x |
| native.small | orjson | 0.879 | 0.896 | 1.019 | 29.992 | 0.69x |
| native.small | msgspec | 0.386 | 0.388 | 0.404 | 29.992 | 1.59x |
| native.small | json | 5.170 | 5.188 | 5.566 | 29.992 | 0.12x |
| mixed.small (native flag) | strata (native=False) | 0.051 | 0.053 | 0.071 | 30.613 | - |
| mixed.small (native flag) | strata (native=True) | 0.057 | 0.058 | 0.072 | 30.613 | - |
| mixed.small (native flag) | orjson | 0.059 | 0.061 | 0.083 | 30.613 | - |
| mixed.small (native flag) | msgspec | 0.071 | 0.073 | 0.103 | 30.613 | - |
| mixed.small (native flag) | ujson | 0.221 | 0.223 | 0.230 | 30.613 | - |
| mixed.small (native flag) | json | 0.484 | 0.503 | 0.661 | 30.613 | - |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| native.small | strata | 0.792 | 0.812 | 1.224 | 30.613 | 1.00x |
| native.small | orjson | 1.084 | 1.110 | 1.150 | 30.613 | 0.73x |
| native.small | msgspec | 0.584 | 0.608 | 0.636 | 30.613 | 1.34x |
| native.small | json | 5.379 | 5.446 | 5.459 | 30.613 | 0.15x |

