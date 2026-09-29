# Benchmark results - native-v1

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 33465c22eee0fda8e1b52898c16aac6982901a21
- python: 3.12.10
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit
- machine: arm64
- processor: Apple M1 (Virtual)
- compiler_flags: see build provenance
- provenance_schema: 1

Excluded libraries (not installed, or no native equivalent):
- ujson (native dataset): no native type support; not named

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| native.small | strata | 0.213 | 0.234 | 0.361 | 32.859 | 1.00x |
| native.small | orjson | 0.508 | 0.556 | 1.528 | 32.859 | 0.42x |
| native.small | msgspec | 0.271 | 0.322 | 0.882 | 32.859 | 0.72x |
| native.small | json | 3.140 | 3.463 | 8.590 | 32.859 | 0.07x |
| mixed.small (native flag) | strata (native=False) | 0.037 | 0.045 | 0.101 | 33.578 | - |
| mixed.small (native flag) | strata (native=True) | 0.036 | 0.043 | 0.065 | 33.578 | - |
| mixed.small (native flag) | orjson | 0.046 | 0.054 | 0.100 | 33.578 | - |
| mixed.small (native flag) | msgspec | 0.054 | 0.074 | 0.104 | 33.578 | - |
| mixed.small (native flag) | ujson | 0.184 | 0.190 | 0.253 | 33.578 | - |
| mixed.small (native flag) | json | 0.372 | 0.389 | 0.458 | 33.578 | - |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| native.small | strata | 0.396 | 0.482 | 0.847 | 33.453 | 1.00x |
| native.small | orjson | 0.712 | 0.817 | 0.922 | 33.453 | 0.59x |
| native.small | msgspec | 0.451 | 0.571 | 0.946 | 33.453 | 0.84x |
| native.small | json | 3.431 | 3.684 | 4.183 | 33.453 | 0.13x |

