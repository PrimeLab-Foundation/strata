# Benchmark results - native-v1

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 33465c22eee0fda8e1b52898c16aac6982901a21
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-aarch64-with-glibc2.39
- machine: aarch64
- processor: aarch64
- compiler_flags: see build provenance
- provenance_schema: 1

Excluded libraries (not installed, or no native equivalent):
- ujson (native dataset): no native type support; not named

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| native.small | strata | 0.301 | 0.304 | 0.334 | 28.906 | 1.00x |
| native.small | orjson | 0.771 | 0.797 | 0.805 | 28.906 | 0.38x |
| native.small | msgspec | 0.354 | 0.359 | 0.386 | 28.906 | 0.85x |
| native.small | json | 4.515 | 4.539 | 4.572 | 28.906 | 0.07x |
| mixed.small (native flag) | strata (native=False) | 0.054 | 0.056 | 0.081 | 29.496 | - |
| mixed.small (native flag) | strata (native=True) | 0.052 | 0.055 | 0.073 | 29.496 | - |
| mixed.small (native flag) | orjson | 0.057 | 0.058 | 0.063 | 29.496 | - |
| mixed.small (native flag) | msgspec | 0.067 | 0.069 | 0.070 | 29.496 | - |
| mixed.small (native flag) | ujson | 0.227 | 0.231 | 0.254 | 29.496 | - |
| mixed.small (native flag) | json | 0.461 | 0.472 | 0.489 | 29.496 | - |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| native.small | strata | 0.550 | 0.568 | 0.602 | 29.480 | 1.00x |
| native.small | orjson | 1.049 | 1.077 | 1.109 | 29.480 | 0.53x |
| native.small | msgspec | 0.611 | 0.648 | 0.679 | 29.480 | 0.88x |
| native.small | json | 4.812 | 4.857 | 4.896 | 29.480 | 0.12x |

