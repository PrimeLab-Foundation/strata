# Benchmark results - native-v1

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: bc6d9ba83ad33f6b9e2f3b35d87b07d0b4dcc11e
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
| native.small | strata | 0.206 | 0.208 | 0.226 | 32.297 | 1.00x |
| native.small | orjson | 0.472 | 0.475 | 0.518 | 32.297 | 0.44x |
| native.small | msgspec | 0.252 | 0.255 | 0.287 | 32.297 | 0.82x |
| native.small | json | 2.951 | 2.978 | 3.179 | 32.297 | 0.07x |
| mixed.small (native flag) | strata (native=False) | 0.030 | 0.032 | 0.046 | 33.719 | - |
| mixed.small (native flag) | strata (native=True) | 0.029 | 0.031 | 0.036 | 33.719 | - |
| mixed.small (native flag) | orjson | 0.038 | 0.041 | 0.050 | 33.719 | - |
| mixed.small (native flag) | msgspec | 0.044 | 0.047 | 0.060 | 33.719 | - |
| mixed.small (native flag) | ujson | 0.161 | 0.168 | 0.194 | 33.719 | - |
| mixed.small (native flag) | json | 0.327 | 0.354 | 0.398 | 33.719 | - |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| native.small | strata | 0.324 | 0.385 | 0.783 | 33.453 | 1.00x |
| native.small | orjson | 0.590 | 0.627 | 1.173 | 33.453 | 0.61x |
| native.small | msgspec | 0.371 | 0.420 | 0.919 | 33.453 | 0.92x |
| native.small | json | 3.108 | 3.203 | 3.686 | 33.453 | 0.12x |

