# Benchmark results - native-v1

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 565fab210bb851772fcefe65082041084aeafc14
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
| native.small | strata | 0.507 | 0.510 | 0.540 | 28.852 | 1.00x |
| native.small | orjson | 0.731 | 0.752 | 0.757 | 28.852 | 0.68x |
| native.small | msgspec | 0.342 | 0.347 | 0.368 | 28.852 | 1.47x |
| native.small | json | 4.497 | 4.516 | 4.527 | 28.852 | 0.11x |
| mixed.small (native flag) | strata (native=False) | 0.051 | 0.055 | 0.056 | 29.457 | - |
| mixed.small (native flag) | strata (native=True) | 0.054 | 0.057 | 0.069 | 29.457 | - |
| mixed.small (native flag) | orjson | 0.056 | 0.056 | 0.065 | 29.457 | - |
| mixed.small (native flag) | msgspec | 0.064 | 0.066 | 0.069 | 29.457 | - |
| mixed.small (native flag) | ujson | 0.227 | 0.230 | 0.255 | 29.457 | - |
| mixed.small (native flag) | json | 0.456 | 0.472 | 0.487 | 29.457 | - |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| native.small | strata | 0.735 | 0.767 | 0.881 | 29.453 | 1.00x |
| native.small | orjson | 0.993 | 1.019 | 1.044 | 29.453 | 0.75x |
| native.small | msgspec | 0.597 | 0.610 | 0.649 | 29.453 | 1.26x |
| native.small | json | 4.764 | 4.775 | 4.785 | 29.453 | 0.16x |

