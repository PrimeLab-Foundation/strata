# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 67e8915
- python: 3.14.3
- implementation: CPython
- platform: macOS-26.3-arm64-arm-64bit-Mach-O
- machine: arm64
- processor: Apple M1 Max
- compiler_flags: -std=c++20 -O3 -march=native
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.001 | 10.121 | 10.370 | 51.391 | 1.00x |
| users.json | orjson | 8.195 | 8.394 | 8.648 | 51.391 | 1.21x |
| users.json | msgspec | 8.188 | 8.385 | 8.830 | 51.391 | 1.21x |
| users.json | ujson | 10.946 | 11.201 | 11.523 | 51.391 | 0.90x |
| users.json | json | 14.544 | 14.736 | 15.089 | 51.391 | 0.69x |
| flat.json | strata | 1.031 | 1.041 | 1.096 | 57.484 | 1.00x |
| flat.json | orjson | 0.632 | 0.640 | 0.666 | 57.484 | 1.63x |
| flat.json | msgspec | 0.669 | 0.678 | 0.689 | 57.484 | 1.54x |
| flat.json | ujson | 0.985 | 0.999 | 1.036 | 57.484 | 1.04x |
| flat.json | json | 1.376 | 1.386 | 1.435 | 57.484 | 0.75x |
| nested.json | strata | 0.840 | 0.850 | 0.882 | 57.547 | 1.00x |
| nested.json | orjson | 1.063 | 1.088 | 1.118 | 57.547 | 0.78x |
| nested.json | msgspec | 1.069 | 1.086 | 1.411 | 57.547 | 0.78x |
| nested.json | ujson | 1.337 | 1.355 | 1.794 | 57.547 | 0.63x |
| nested.json | json | 1.779 | 1.816 | 1.955 | 57.547 | 0.47x |
| wide_arrays.json | strata | 4.909 | 4.940 | 5.182 | 57.625 | 1.00x |
| wide_arrays.json | orjson | 3.603 | 3.668 | 3.913 | 57.625 | 1.35x |
| wide_arrays.json | msgspec | 4.367 | 4.448 | 4.897 | 57.625 | 1.11x |
| wide_arrays.json | ujson | 5.562 | 5.637 | 6.014 | 57.625 | 0.88x |
| wide_arrays.json | json | 7.835 | 8.007 | 8.507 | 57.625 | 0.62x |
| mixed.json | strata | 0.200 | 0.203 | 0.215 | 57.641 | 1.00x |
| mixed.json | orjson | 0.124 | 0.129 | 0.152 | 57.641 | 1.58x |
| mixed.json | msgspec | 0.138 | 0.141 | 0.153 | 57.641 | 1.44x |
| mixed.json | ujson | 0.179 | 0.183 | 0.195 | 57.641 | 1.11x |
| mixed.json | json | 0.297 | 0.301 | 0.325 | 57.641 | 0.68x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 5.946 | 5.986 | 6.072 | 59.609 | 1.00x |
| users.json | orjson | 1.953 | 1.980 | 2.003 | 59.609 | 3.02x |
| users.json | msgspec | 2.599 | 2.618 | 2.657 | 59.609 | 2.29x |
| users.json | ujson | 10.474 | 10.666 | 10.915 | 59.609 | 0.56x |
| users.json | json | 14.904 | 14.966 | 16.018 | 59.609 | 0.40x |
| flat.json | strata | 0.596 | 0.606 | 0.642 | 57.531 | 1.00x |
| flat.json | orjson | 0.221 | 0.228 | 0.251 | 57.531 | 2.66x |
| flat.json | msgspec | 0.285 | 0.294 | 0.307 | 57.531 | 2.06x |
| flat.json | ujson | 0.893 | 0.899 | 0.979 | 57.531 | 0.67x |
| flat.json | json | 1.243 | 1.260 | 1.320 | 57.531 | 0.48x |
| nested.json | strata | 0.542 | 0.554 | 0.601 | 57.625 | 1.00x |
| nested.json | orjson | 0.196 | 0.201 | 0.250 | 57.625 | 2.75x |
| nested.json | msgspec | 0.259 | 0.267 | 0.400 | 57.625 | 2.07x |
| nested.json | ujson | 1.027 | 1.036 | 1.056 | 57.625 | 0.53x |
| nested.json | json | 1.485 | 1.514 | 1.628 | 57.625 | 0.37x |
| wide_arrays.json | strata | 4.273 | 4.321 | 4.461 | 57.641 | 1.00x |
| wide_arrays.json | orjson | 1.075 | 1.106 | 1.145 | 57.641 | 3.91x |
| wide_arrays.json | msgspec | 1.870 | 1.914 | 1.989 | 57.641 | 2.26x |
| wide_arrays.json | ujson | 6.000 | 6.047 | 6.183 | 57.641 | 0.71x |
| wide_arrays.json | json | 10.619 | 10.790 | 11.033 | 57.641 | 0.40x |
| mixed.json | strata | 0.100 | 0.104 | 0.112 | 57.641 | 1.00x |
| mixed.json | orjson | 0.041 | 0.044 | 0.047 | 57.641 | 2.38x |
| mixed.json | msgspec | 0.047 | 0.050 | 0.065 | 57.641 | 2.07x |
| mixed.json | ujson | 0.209 | 0.215 | 0.222 | 57.641 | 0.48x |
| mixed.json | json | 0.324 | 0.332 | 0.344 | 57.641 | 0.31x |

