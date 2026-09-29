# Benchmark results - native-v1

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 82e3fa5f24c38cfa1150440f0cf5da6c286c01cd
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
| native.small | strata | 0.907 | 0.958 | 2.184 | 24.934 | 1.00x |
| native.small | orjson | 1.365 | 1.416 | 2.323 | 24.934 | 0.68x |
| native.small | msgspec | 0.574 | 0.596 | 0.873 | 24.934 | 1.61x |
| native.small | json | 9.493 | 9.749 | 16.900 | 24.934 | 0.10x |
| mixed.small (native flag) | strata (native=False) | 0.059 | 0.064 | 0.119 | 25.539 | - |
| mixed.small (native flag) | strata (native=True) | 0.060 | 0.068 | 0.112 | 25.539 | - |
| mixed.small (native flag) | orjson | 0.074 | 0.080 | 0.115 | 25.539 | - |
| mixed.small (native flag) | msgspec | 0.104 | 0.114 | 0.183 | 25.539 | - |
| mixed.small (native flag) | ujson | 0.430 | 0.468 | 0.556 | 25.539 | - |
| mixed.small (native flag) | json | 0.901 | 0.965 | 1.228 | 25.539 | - |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| native.small | strata | 1.210 | 1.257 | 1.381 | 25.395 | 1.00x |
| native.small | orjson | 1.692 | 1.787 | 2.606 | 25.395 | 0.70x |
| native.small | msgspec | 0.911 | 0.944 | 1.324 | 25.395 | 1.33x |
| native.small | json | 8.991 | 9.534 | 12.025 | 25.395 | 0.13x |

