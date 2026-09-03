# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: a969975
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: x86_64
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.900 | 9.962 | 13.316 | 58.992 | 1.00x |
| users.json | orjson | 12.787 | 12.898 | 15.674 | 58.992 | 0.77x |
| users.json | msgspec | 12.854 | 13.090 | 15.442 | 58.992 | 0.76x |
| users.json | ujson | 17.200 | 17.293 | 21.554 | 58.992 | 0.58x |
| users.json | pysimdjson | 17.510 | 17.766 | 21.113 | 58.992 | 0.56x |
| users.json | json | 22.126 | 22.233 | 23.257 | 58.992 | 0.45x |
| flat.json | strata | 0.885 | 0.888 | 1.089 | 70.879 | 1.00x |
| flat.json | orjson | 0.994 | 0.997 | 0.999 | 70.879 | 0.89x |
| flat.json | msgspec | 0.991 | 1.001 | 1.039 | 70.879 | 0.89x |
| flat.json | ujson | 1.502 | 1.516 | 1.549 | 70.879 | 0.59x |
| flat.json | pysimdjson | 1.544 | 1.555 | 1.582 | 70.879 | 0.57x |
| flat.json | json | 1.896 | 1.908 | 1.960 | 70.879 | 0.47x |
| nested.json | strata | 0.806 | 0.819 | 0.842 | 70.879 | 1.00x |
| nested.json | orjson | 0.998 | 1.000 | 1.007 | 70.879 | 0.82x |
| nested.json | msgspec | 1.010 | 1.020 | 1.252 | 70.879 | 0.80x |
| nested.json | ujson | 1.456 | 1.469 | 1.494 | 70.879 | 0.56x |
| nested.json | pysimdjson | 1.397 | 1.411 | 1.618 | 70.879 | 0.58x |
| nested.json | json | 2.024 | 2.048 | 2.097 | 70.879 | 0.40x |
| wide_arrays.json | strata | 4.591 | 4.642 | 4.864 | 73.871 | 1.00x |
| wide_arrays.json | orjson | 4.956 | 4.976 | 5.004 | 73.871 | 0.93x |
| wide_arrays.json | msgspec | 5.571 | 5.699 | 5.753 | 73.871 | 0.81x |
| wide_arrays.json | ujson | 7.077 | 7.190 | 7.335 | 73.871 | 0.65x |
| wide_arrays.json | pysimdjson | 5.923 | 6.030 | 6.132 | 73.871 | 0.77x |
| wide_arrays.json | json | 9.581 | 9.627 | 9.786 | 73.871 | 0.48x |
| mixed.json | strata | 0.193 | 0.195 | 0.209 | 73.934 | 1.00x |
| mixed.json | orjson | 0.228 | 0.236 | 0.253 | 73.934 | 0.83x |
| mixed.json | msgspec | 0.238 | 0.240 | 0.255 | 73.934 | 0.81x |
| mixed.json | ujson | 0.300 | 0.303 | 0.320 | 73.934 | 0.64x |
| mixed.json | pysimdjson | 0.294 | 0.296 | 0.385 | 73.934 | 0.66x |
| mixed.json | json | 0.472 | 0.488 | 0.493 | 73.934 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.481 | 2.488 | 2.533 | 58.113 | 1.00x |
| users.json | orjson | 2.890 | 2.907 | 2.927 | 58.113 | 0.86x |
| users.json | msgspec | 3.838 | 3.845 | 3.888 | 58.113 | 0.65x |
| users.json | ujson | 14.367 | 14.840 | 15.038 | 58.113 | 0.17x |
| users.json | json | 21.474 | 21.617 | 21.808 | 58.113 | 0.12x |
| flat.json | strata | 0.262 | 0.264 | 0.306 | 70.879 | 1.00x |
| flat.json | orjson | 0.329 | 0.332 | 0.342 | 70.879 | 0.80x |
| flat.json | msgspec | 0.428 | 0.432 | 0.537 | 70.879 | 0.61x |
| flat.json | ujson | 1.274 | 1.305 | 1.329 | 70.879 | 0.20x |
| flat.json | json | 1.832 | 1.861 | 1.882 | 70.879 | 0.14x |
| nested.json | strata | 0.253 | 0.255 | 0.281 | 70.883 | 1.00x |
| nested.json | orjson | 0.293 | 0.301 | 0.342 | 70.883 | 0.85x |
| nested.json | msgspec | 0.398 | 0.402 | 0.411 | 70.883 | 0.63x |
| nested.json | ujson | 1.423 | 1.435 | 1.446 | 70.883 | 0.18x |
| nested.json | json | 2.337 | 2.360 | 2.406 | 70.883 | 0.11x |
| wide_arrays.json | strata | 1.551 | 1.566 | 1.591 | 73.871 | 1.00x |
| wide_arrays.json | orjson | 1.743 | 1.755 | 1.971 | 73.871 | 0.89x |
| wide_arrays.json | msgspec | 2.654 | 2.666 | 2.853 | 73.871 | 0.59x |
| wide_arrays.json | ujson | 8.465 | 8.550 | 8.800 | 73.871 | 0.18x |
| wide_arrays.json | json | 16.302 | 16.346 | 17.406 | 73.871 | 0.10x |
| mixed.json | strata | 0.064 | 0.065 | 0.077 | 73.934 | 1.00x |
| mixed.json | orjson | 0.063 | 0.066 | 0.078 | 73.934 | 0.98x |
| mixed.json | msgspec | 0.083 | 0.090 | 0.101 | 73.934 | 0.71x |
| mixed.json | ujson | 0.307 | 0.313 | 0.328 | 73.934 | 0.21x |
| mixed.json | json | 0.495 | 0.498 | 0.510 | 73.934 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.399 | 10.451 | 12.102 | 77.461 | 1.00x |
| users.json | orjson | 13.322 | 13.535 | 14.165 | 77.461 | 0.77x |
| users.json | msgspec | 13.342 | 13.487 | 13.875 | 77.461 | 0.77x |
| users.json | ujson | 18.342 | 18.607 | 20.254 | 77.461 | 0.56x |
| users.json | json | 22.728 | 22.977 | 23.294 | 77.461 | 0.45x |
| flat.json | strata | 0.905 | 0.919 | 0.955 | 70.879 | 1.00x |
| flat.json | orjson | 1.120 | 1.128 | 1.135 | 70.879 | 0.81x |
| flat.json | msgspec | 1.054 | 1.061 | 1.069 | 70.879 | 0.87x |
| flat.json | ujson | 1.531 | 1.545 | 1.568 | 70.879 | 0.59x |
| flat.json | json | 1.966 | 1.985 | 2.207 | 70.879 | 0.46x |
| nested.json | strata | 0.826 | 0.839 | 0.890 | 70.883 | 1.00x |
| nested.json | orjson | 1.049 | 1.054 | 1.065 | 70.883 | 0.80x |
| nested.json | msgspec | 1.071 | 1.083 | 1.109 | 70.883 | 0.77x |
| nested.json | ujson | 1.531 | 1.551 | 2.054 | 70.883 | 0.54x |
| nested.json | json | 2.103 | 2.121 | 2.307 | 70.883 | 0.40x |
| wide_arrays.json | strata | 4.680 | 4.700 | 4.958 | 73.934 | 1.00x |
| wide_arrays.json | orjson | 5.053 | 5.070 | 5.372 | 73.934 | 0.93x |
| wide_arrays.json | msgspec | 5.682 | 5.732 | 5.965 | 73.934 | 0.82x |
| wide_arrays.json | ujson | 7.295 | 7.328 | 7.464 | 73.934 | 0.64x |
| wide_arrays.json | json | 9.679 | 9.707 | 10.301 | 73.934 | 0.48x |
| mixed.json | strata | 0.213 | 0.215 | 0.228 | 73.934 | 1.00x |
| mixed.json | orjson | 0.269 | 0.274 | 0.290 | 73.934 | 0.79x |
| mixed.json | msgspec | 0.280 | 0.281 | 0.299 | 73.934 | 0.77x |
| mixed.json | ujson | 0.355 | 0.358 | 0.378 | 73.934 | 0.60x |
| mixed.json | json | 0.511 | 0.516 | 0.532 | 73.934 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.125 | 10.250 | 13.689 | 70.879 | 1.00x |
| users.ndjson | orjson | 16.433 | 16.566 | 16.673 | 70.879 | 0.62x |
| users.ndjson | msgspec | 16.252 | 16.564 | 17.045 | 70.879 | 0.62x |
| users.ndjson | ujson | 20.802 | 20.991 | 21.275 | 70.879 | 0.49x |
| users.ndjson | json | 28.754 | 28.885 | 29.399 | 70.879 | 0.35x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.006 | 3.021 | 3.132 | 73.523 | 1.00x |
| users.json | orjson | 3.502 | 3.519 | 3.610 | 73.523 | 0.86x |
| users.json | msgspec | 4.408 | 4.424 | 4.458 | 73.523 | 0.68x |
| users.json | ujson | 15.526 | 15.651 | 15.943 | 73.523 | 0.19x |
| users.json | json | 22.305 | 22.385 | 22.639 | 73.523 | 0.13x |
| flat.json | strata | 0.401 | 0.413 | 0.439 | 70.879 | 1.00x |
| flat.json | orjson | 0.483 | 0.483 | 0.520 | 70.879 | 0.85x |
| flat.json | msgspec | 0.582 | 0.600 | 0.620 | 70.879 | 0.69x |
| flat.json | ujson | 1.475 | 1.500 | 1.692 | 70.879 | 0.28x |
| flat.json | json | 2.017 | 2.041 | 2.055 | 70.879 | 0.20x |
| nested.json | strata | 0.365 | 0.370 | 0.381 | 70.883 | 1.00x |
| nested.json | orjson | 0.423 | 0.427 | 0.463 | 70.883 | 0.87x |
| nested.json | msgspec | 0.527 | 0.542 | 0.563 | 70.883 | 0.68x |
| nested.json | ujson | 1.546 | 1.559 | 1.589 | 70.883 | 0.24x |
| nested.json | json | 2.525 | 2.562 | 2.603 | 70.883 | 0.14x |
| wide_arrays.json | strata | 1.929 | 1.941 | 1.967 | 73.934 | 1.00x |
| wide_arrays.json | orjson | 2.149 | 2.168 | 2.187 | 73.934 | 0.90x |
| wide_arrays.json | msgspec | 3.074 | 3.095 | 3.113 | 73.934 | 0.63x |
| wide_arrays.json | ujson | 9.069 | 9.108 | 9.303 | 73.934 | 0.21x |
| wide_arrays.json | json | 16.851 | 16.870 | 17.058 | 73.934 | 0.12x |
| mixed.json | strata | 0.153 | 0.155 | 0.166 | 73.934 | 1.00x |
| mixed.json | orjson | 0.169 | 0.171 | 0.204 | 73.934 | 0.90x |
| mixed.json | msgspec | 0.188 | 0.189 | 0.204 | 73.934 | 0.82x |
| mixed.json | ujson | 0.415 | 0.420 | 0.464 | 73.934 | 0.37x |
| mixed.json | json | 0.616 | 0.643 | 0.669 | 73.934 | 0.24x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.061 | 0.062 | 0.074 | 73.523 | 1.00x |
| users.json $[*].id | jmespath | 0.490 | 0.503 | 0.528 | 73.523 | 0.12x |
| users.json $[*].id | jsonpath-ng | 2.769 | 2.792 | 3.211 | 73.523 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.421 | 0.444 | 0.720 | 74.508 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.075 | 3.092 | 4.965 | 74.508 | 0.14x |
| users.json $[*].orders[*].total | jsonpath-ng | 18.851 | 19.031 | 22.036 | 74.508 | 0.02x |
| users.json $..total | strata | 1.640 | 1.656 | 1.881 | 76.477 | 1.00x |
| users.json $..total | jsonpath-ng | 388.734 | 391.287 | 395.695 | 76.477 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.193 | 3.218 | 4.508 | 74.508 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.886 | 14.042 | 31.419 | 74.508 | 0.23x |
| users.json $[*].id | orjson+jsonpath-ng | 16.224 | 16.568 | 29.024 | 74.508 | 0.19x |
| users.json $[*].orders[*].total | strata | 3.387 | 3.401 | 3.420 | 76.477 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 16.637 | 16.830 | 17.069 | 76.477 | 0.20x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 35.785 | 36.286 | 36.571 | 76.477 | 0.09x |
| users.json $..total | strata | 12.694 | 12.818 | 12.943 | 75.492 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 410.251 | 414.315 | 420.639 | 75.492 | 0.03x |

