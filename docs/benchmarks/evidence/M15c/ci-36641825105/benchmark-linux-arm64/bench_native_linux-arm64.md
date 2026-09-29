# Benchmark results - native-v1

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 271a2a0864aa430c4690ca9b609e4201e13cfb51
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
| native.small | strata | 0.293 | 0.296 | 0.317 | 28.922 | 1.00x |
| native.small | orjson | 0.752 | 0.770 | 0.821 | 28.922 | 0.38x |
| native.small | msgspec | 0.349 | 0.353 | 0.383 | 28.922 | 0.84x |
| native.small | json | 4.507 | 4.528 | 4.797 | 28.922 | 0.07x |
| mixed.small (native flag) | strata (native=False) | 0.049 | 0.050 | 0.052 | 29.496 | - |
| mixed.small (native flag) | strata (native=True) | 0.049 | 0.050 | 0.051 | 29.496 | - |
| mixed.small (native flag) | orjson | 0.053 | 0.054 | 0.066 | 29.496 | - |
| mixed.small (native flag) | msgspec | 0.062 | 0.064 | 0.080 | 29.496 | - |
| mixed.small (native flag) | ujson | 0.221 | 0.223 | 0.238 | 29.496 | - |
| mixed.small (native flag) | json | 0.444 | 0.448 | 0.463 | 29.496 | - |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| native.small | strata | 0.487 | 0.518 | 0.554 | 29.480 | 1.00x |
| native.small | orjson | 0.978 | 1.001 | 1.055 | 29.480 | 0.52x |
| native.small | msgspec | 0.556 | 0.589 | 0.618 | 29.480 | 0.88x |
| native.small | json | 4.744 | 4.784 | 4.852 | 29.480 | 0.11x |

