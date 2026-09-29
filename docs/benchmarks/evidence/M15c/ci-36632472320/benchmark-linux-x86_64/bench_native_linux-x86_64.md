# Benchmark results - native-v1

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: bc6d9ba83ad33f6b9e2f3b35d87b07d0b4dcc11e
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
| native.small | strata | 0.325 | 0.331 | 0.378 | 29.941 | 1.00x |
| native.small | orjson | 0.979 | 0.994 | 1.956 | 29.941 | 0.33x |
| native.small | msgspec | 0.392 | 0.398 | 0.429 | 29.941 | 0.83x |
| native.small | json | 5.452 | 5.512 | 5.592 | 29.941 | 0.06x |
| mixed.small (native flag) | strata (native=False) | 0.051 | 0.053 | 0.068 | 30.586 | - |
| mixed.small (native flag) | strata (native=True) | 0.051 | 0.053 | 0.056 | 30.586 | - |
| mixed.small (native flag) | orjson | 0.056 | 0.057 | 0.071 | 30.586 | - |
| mixed.small (native flag) | msgspec | 0.070 | 0.072 | 0.074 | 30.586 | - |
| mixed.small (native flag) | ujson | 0.226 | 0.231 | 0.245 | 30.586 | - |
| mixed.small (native flag) | json | 0.488 | 0.502 | 0.569 | 30.586 | - |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| native.small | strata | 0.504 | 0.530 | 0.614 | 30.520 | 1.00x |
| native.small | orjson | 1.166 | 1.184 | 2.961 | 30.520 | 0.45x |
| native.small | msgspec | 0.583 | 0.605 | 0.670 | 30.520 | 0.88x |
| native.small | json | 5.683 | 5.724 | 8.765 | 30.520 | 0.09x |

