# Benchmark results - native-v1

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 271a2a0864aa430c4690ca9b609e4201e13cfb51
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
| native.small | strata | 0.212 | 0.224 | 0.673 | 32.953 | 1.00x |
| native.small | orjson | 0.513 | 0.550 | 0.999 | 32.953 | 0.41x |
| native.small | msgspec | 0.280 | 0.311 | 0.829 | 32.953 | 0.72x |
| native.small | json | 3.199 | 3.420 | 5.352 | 32.953 | 0.07x |
| mixed.small (native flag) | strata (native=False) | 0.035 | 0.039 | 0.059 | 33.719 | - |
| mixed.small (native flag) | strata (native=True) | 0.034 | 0.037 | 0.048 | 33.719 | - |
| mixed.small (native flag) | orjson | 0.045 | 0.050 | 0.072 | 33.719 | - |
| mixed.small (native flag) | msgspec | 0.052 | 0.060 | 0.112 | 33.719 | - |
| mixed.small (native flag) | ujson | 0.177 | 0.194 | 0.234 | 33.719 | - |
| mixed.small (native flag) | json | 0.369 | 0.381 | 0.447 | 33.719 | - |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| native.small | strata | 0.429 | 0.501 | 1.108 | 33.531 | 1.00x |
| native.small | orjson | 0.714 | 0.845 | 0.977 | 33.531 | 0.59x |
| native.small | msgspec | 0.495 | 0.567 | 0.636 | 33.531 | 0.88x |
| native.small | json | 3.443 | 3.713 | 4.394 | 33.531 | 0.14x |

