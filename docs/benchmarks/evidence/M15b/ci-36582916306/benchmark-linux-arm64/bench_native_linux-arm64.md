# Benchmark results - native-v1

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 82e3fa5f24c38cfa1150440f0cf5da6c286c01cd
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
| native.small | strata | 0.513 | 0.528 | 0.538 | 28.848 | 1.00x |
| native.small | orjson | 0.738 | 0.759 | 0.774 | 28.848 | 0.69x |
| native.small | msgspec | 0.346 | 0.353 | 0.372 | 28.848 | 1.49x |
| native.small | json | 4.486 | 4.523 | 4.557 | 28.848 | 0.12x |
| mixed.small (native flag) | strata (native=False) | 0.055 | 0.055 | 0.078 | 29.457 | - |
| mixed.small (native flag) | strata (native=True) | 0.057 | 0.058 | 0.079 | 29.457 | - |
| mixed.small (native flag) | orjson | 0.056 | 0.057 | 0.080 | 29.457 | - |
| mixed.small (native flag) | msgspec | 0.067 | 0.068 | 0.069 | 29.457 | - |
| mixed.small (native flag) | ujson | 0.228 | 0.230 | 0.258 | 29.457 | - |
| mixed.small (native flag) | json | 0.459 | 0.472 | 0.495 | 29.457 | - |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| native.small | strata | 0.928 | 0.971 | 5.886 | 29.449 | 1.00x |
| native.small | orjson | 1.155 | 1.241 | 1.297 | 29.449 | 0.78x |
| native.small | msgspec | 0.746 | 0.821 | 0.892 | 29.449 | 1.18x |
| native.small | json | 4.943 | 5.022 | 5.153 | 29.449 | 0.19x |

