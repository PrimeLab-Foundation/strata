# Benchmark results - native-v1

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 82e3fa5f24c38cfa1150440f0cf5da6c286c01cd
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
| native.small | strata | 0.605 | 0.622 | 0.903 | 30.051 | 1.00x |
| native.small | orjson | 0.897 | 0.916 | 0.955 | 30.051 | 0.68x |
| native.small | msgspec | 0.392 | 0.402 | 0.407 | 30.051 | 1.55x |
| native.small | json | 5.262 | 5.293 | 5.650 | 30.051 | 0.12x |
| mixed.small (native flag) | strata (native=False) | 0.051 | 0.053 | 0.057 | 30.688 | - |
| mixed.small (native flag) | strata (native=True) | 0.056 | 0.059 | 0.073 | 30.688 | - |
| mixed.small (native flag) | orjson | 0.057 | 0.059 | 0.063 | 30.688 | - |
| mixed.small (native flag) | msgspec | 0.070 | 0.072 | 0.087 | 30.688 | - |
| mixed.small (native flag) | ujson | 0.219 | 0.227 | 0.240 | 30.688 | - |
| mixed.small (native flag) | json | 0.479 | 0.493 | 0.521 | 30.688 | - |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| native.small | strata | 0.799 | 0.832 | 0.945 | 30.625 | 1.00x |
| native.small | orjson | 1.084 | 1.114 | 1.297 | 30.625 | 0.75x |
| native.small | msgspec | 0.589 | 0.616 | 0.635 | 30.625 | 1.35x |
| native.small | json | 5.420 | 5.464 | 6.254 | 30.625 | 0.15x |

