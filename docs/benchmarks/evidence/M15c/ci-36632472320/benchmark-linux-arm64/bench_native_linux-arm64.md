# Benchmark results - native-v1

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: bc6d9ba83ad33f6b9e2f3b35d87b07d0b4dcc11e
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
| native.small | strata | 0.296 | 0.299 | 0.329 | 28.887 | 1.00x |
| native.small | orjson | 0.756 | 0.781 | 0.797 | 28.887 | 0.38x |
| native.small | msgspec | 0.346 | 0.353 | 0.388 | 28.887 | 0.85x |
| native.small | json | 4.505 | 4.530 | 4.563 | 28.887 | 0.07x |
| mixed.small (native flag) | strata (native=False) | 0.051 | 0.053 | 0.074 | 29.477 | - |
| mixed.small (native flag) | strata (native=True) | 0.051 | 0.053 | 0.070 | 29.477 | - |
| mixed.small (native flag) | orjson | 0.055 | 0.057 | 0.074 | 29.477 | - |
| mixed.small (native flag) | msgspec | 0.065 | 0.067 | 0.085 | 29.477 | - |
| mixed.small (native flag) | ujson | 0.225 | 0.229 | 0.251 | 29.477 | - |
| mixed.small (native flag) | json | 0.458 | 0.474 | 0.482 | 29.477 | - |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| native.small | strata | 0.530 | 0.581 | 0.621 | 29.461 | 1.00x |
| native.small | orjson | 1.040 | 1.072 | 1.180 | 29.461 | 0.54x |
| native.small | msgspec | 0.612 | 0.645 | 0.689 | 29.461 | 0.90x |
| native.small | json | 4.800 | 4.844 | 4.883 | 29.461 | 0.12x |

