# Benchmark results - native-v1

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 271a2a0864aa430c4690ca9b609e4201e13cfb51
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: AMD EPYC 9V45 96-Core Processor
- compiler_flags: see build provenance
- provenance_schema: 1

Excluded libraries (not installed, or no native equivalent):
- ujson (native dataset): no native type support; not named

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| native.small | strata | 0.159 | 0.162 | 0.171 | 30.195 | 1.00x |
| native.small | orjson | 0.492 | 0.498 | 0.511 | 30.195 | 0.33x |
| native.small | msgspec | 0.199 | 0.204 | 0.214 | 30.195 | 0.79x |
| native.small | json | 2.865 | 2.897 | 3.023 | 30.195 | 0.06x |
| mixed.small (native flag) | strata (native=False) | 0.028 | 0.029 | 0.038 | 30.820 | - |
| mixed.small (native flag) | strata (native=True) | 0.028 | 0.029 | 0.031 | 30.820 | - |
| mixed.small (native flag) | orjson | 0.023 | 0.024 | 0.026 | 30.820 | - |
| mixed.small (native flag) | msgspec | 0.036 | 0.037 | 0.039 | 30.820 | - |
| mixed.small (native flag) | ujson | 0.120 | 0.124 | 0.144 | 30.820 | - |
| mixed.small (native flag) | json | 0.255 | 0.264 | 0.275 | 30.820 | - |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| native.small | strata | 0.339 | 0.353 | 0.385 | 30.754 | 1.00x |
| native.small | orjson | 0.662 | 0.684 | 0.772 | 30.754 | 0.52x |
| native.small | msgspec | 0.389 | 0.400 | 0.442 | 30.754 | 0.88x |
| native.small | json | 3.034 | 3.061 | 3.125 | 30.754 | 0.12x |

