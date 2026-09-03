# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 060bd7b
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
| users.json | strata | 10.412 | 10.519 | 17.789 | 58.562 | 1.00x |
| users.json | orjson | 13.869 | 14.106 | 24.581 | 58.562 | 0.75x |
| users.json | msgspec | 13.691 | 13.895 | 23.838 | 58.562 | 0.76x |
| users.json | ujson | 18.079 | 18.762 | 30.147 | 58.562 | 0.56x |
| users.json | pysimdjson | 19.366 | 19.609 | 26.940 | 58.562 | 0.54x |
| users.json | json | 21.159 | 21.520 | 38.678 | 58.562 | 0.49x |
| flat.json | strata | 0.868 | 0.891 | 0.908 | 71.898 | 1.00x |
| flat.json | orjson | 1.024 | 1.042 | 1.199 | 71.898 | 0.86x |
| flat.json | msgspec | 1.028 | 1.058 | 1.088 | 71.898 | 0.84x |
| flat.json | ujson | 1.552 | 1.634 | 1.754 | 71.898 | 0.55x |
| flat.json | pysimdjson | 1.635 | 1.753 | 1.810 | 71.898 | 0.51x |
| flat.json | json | 1.734 | 1.750 | 1.798 | 71.898 | 0.51x |
| nested.json | strata | 0.796 | 0.812 | 0.835 | 71.898 | 1.00x |
| nested.json | orjson | 0.996 | 1.011 | 1.017 | 71.898 | 0.80x |
| nested.json | msgspec | 0.982 | 0.993 | 1.217 | 71.898 | 0.82x |
| nested.json | ujson | 1.468 | 1.515 | 1.649 | 71.898 | 0.54x |
| nested.json | pysimdjson | 1.386 | 1.403 | 1.509 | 71.898 | 0.58x |
| nested.json | json | 1.815 | 1.839 | 1.934 | 71.898 | 0.44x |
| wide_arrays.json | strata | 5.032 | 5.128 | 5.354 | 74.953 | 1.00x |
| wide_arrays.json | orjson | 5.602 | 5.668 | 5.990 | 74.953 | 0.90x |
| wide_arrays.json | msgspec | 6.147 | 6.235 | 6.502 | 74.953 | 0.82x |
| wide_arrays.json | ujson | 7.800 | 7.953 | 8.212 | 74.953 | 0.64x |
| wide_arrays.json | pysimdjson | 6.429 | 6.604 | 6.909 | 74.953 | 0.78x |
| wide_arrays.json | json | 9.897 | 10.131 | 10.282 | 74.953 | 0.51x |
| mixed.json | strata | 0.198 | 0.206 | 0.224 | 75.016 | 1.00x |
| mixed.json | orjson | 0.237 | 0.245 | 0.257 | 75.016 | 0.84x |
| mixed.json | msgspec | 0.244 | 0.259 | 0.265 | 75.016 | 0.79x |
| mixed.json | ujson | 0.316 | 0.329 | 0.344 | 75.016 | 0.62x |
| mixed.json | pysimdjson | 0.309 | 0.317 | 0.335 | 75.016 | 0.65x |
| mixed.json | json | 0.455 | 0.462 | 0.472 | 75.016 | 0.44x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.385 | 2.401 | 2.459 | 57.648 | 1.00x |
| users.json | orjson | 3.083 | 3.108 | 3.206 | 57.648 | 0.77x |
| users.json | msgspec | 4.184 | 4.202 | 4.275 | 57.648 | 0.57x |
| users.json | ujson | 14.737 | 14.922 | 15.733 | 57.648 | 0.16x |
| users.json | json | 21.593 | 21.799 | 22.845 | 57.648 | 0.11x |
| flat.json | strata | 0.299 | 0.311 | 0.325 | 71.898 | 1.00x |
| flat.json | orjson | 0.368 | 0.381 | 0.405 | 71.898 | 0.81x |
| flat.json | msgspec | 0.480 | 0.505 | 0.570 | 71.898 | 0.62x |
| flat.json | ujson | 1.266 | 1.275 | 1.333 | 71.898 | 0.24x |
| flat.json | json | 1.857 | 1.872 | 1.938 | 71.898 | 0.17x |
| nested.json | strata | 0.233 | 0.249 | 0.269 | 71.898 | 1.00x |
| nested.json | orjson | 0.300 | 0.305 | 0.315 | 71.898 | 0.82x |
| nested.json | msgspec | 0.429 | 0.439 | 0.457 | 71.898 | 0.57x |
| nested.json | ujson | 1.361 | 1.375 | 1.395 | 71.898 | 0.18x |
| nested.json | json | 2.340 | 2.361 | 2.434 | 71.898 | 0.11x |
| wide_arrays.json | strata | 1.685 | 1.706 | 2.078 | 74.953 | 1.00x |
| wide_arrays.json | orjson | 1.914 | 1.930 | 1.969 | 74.953 | 0.88x |
| wide_arrays.json | msgspec | 2.973 | 2.982 | 3.005 | 74.953 | 0.57x |
| wide_arrays.json | ujson | 8.641 | 8.695 | 8.870 | 74.953 | 0.20x |
| wide_arrays.json | json | 16.541 | 16.632 | 17.270 | 74.953 | 0.10x |
| mixed.json | strata | 0.063 | 0.066 | 0.068 | 75.016 | 1.00x |
| mixed.json | orjson | 0.069 | 0.071 | 0.092 | 75.016 | 0.94x |
| mixed.json | msgspec | 0.088 | 0.091 | 0.102 | 75.016 | 0.73x |
| mixed.json | ujson | 0.305 | 0.315 | 0.326 | 75.016 | 0.21x |
| mixed.json | json | 0.509 | 0.529 | 0.536 | 75.016 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.751 | 12.741 | 21.191 | 73.922 | 1.00x |
| users.json | orjson | 14.886 | 15.707 | 16.815 | 73.922 | 0.81x |
| users.json | msgspec | 14.530 | 15.359 | 16.965 | 73.922 | 0.83x |
| users.json | ujson | 20.208 | 21.247 | 23.166 | 73.922 | 0.60x |
| users.json | json | 21.949 | 22.903 | 29.985 | 73.922 | 0.56x |
| flat.json | strata | 0.922 | 0.937 | 0.961 | 71.898 | 1.00x |
| flat.json | orjson | 1.112 | 1.122 | 1.149 | 71.898 | 0.83x |
| flat.json | msgspec | 1.116 | 1.141 | 1.202 | 71.898 | 0.82x |
| flat.json | ujson | 1.676 | 1.718 | 1.971 | 71.898 | 0.55x |
| flat.json | json | 1.800 | 1.809 | 1.860 | 71.898 | 0.52x |
| nested.json | strata | 0.850 | 0.872 | 0.908 | 71.898 | 1.00x |
| nested.json | orjson | 1.080 | 1.104 | 1.127 | 71.898 | 0.79x |
| nested.json | msgspec | 1.063 | 1.077 | 1.108 | 71.898 | 0.81x |
| nested.json | ujson | 1.497 | 1.538 | 1.570 | 71.898 | 0.57x |
| nested.json | json | 1.934 | 1.955 | 1.994 | 71.898 | 0.45x |
| wide_arrays.json | strata | 5.213 | 5.410 | 5.585 | 75.016 | 1.00x |
| wide_arrays.json | orjson | 5.932 | 6.070 | 6.264 | 75.016 | 0.89x |
| wide_arrays.json | msgspec | 6.520 | 6.677 | 6.952 | 75.016 | 0.81x |
| wide_arrays.json | ujson | 8.386 | 8.427 | 8.744 | 75.016 | 0.64x |
| wide_arrays.json | json | 10.220 | 10.441 | 10.746 | 75.016 | 0.52x |
| mixed.json | strata | 0.239 | 0.250 | 0.260 | 75.016 | 1.00x |
| mixed.json | orjson | 0.302 | 0.324 | 0.345 | 75.016 | 0.77x |
| mixed.json | msgspec | 0.312 | 0.323 | 0.340 | 75.016 | 0.77x |
| mixed.json | ujson | 0.390 | 0.412 | 0.430 | 75.016 | 0.61x |
| mixed.json | json | 0.518 | 0.537 | 0.553 | 75.016 | 0.47x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 11.440 | 13.198 | 14.503 | 69.426 | 1.00x |
| users.ndjson | orjson | 18.420 | 19.770 | 21.355 | 69.426 | 0.67x |
| users.ndjson | msgspec | 18.269 | 19.065 | 20.464 | 69.426 | 0.69x |
| users.ndjson | ujson | 23.363 | 24.722 | 26.273 | 69.426 | 0.53x |
| users.ndjson | json | 29.503 | 30.953 | 32.385 | 69.426 | 0.43x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.995 | 3.032 | 3.260 | 69.984 | 1.00x |
| users.json | orjson | 3.736 | 3.790 | 3.933 | 69.984 | 0.80x |
| users.json | msgspec | 4.807 | 4.894 | 5.013 | 69.984 | 0.62x |
| users.json | ujson | 15.464 | 15.692 | 16.129 | 69.984 | 0.19x |
| users.json | json | 22.531 | 23.010 | 24.923 | 69.984 | 0.13x |
| flat.json | strata | 0.455 | 0.495 | 0.543 | 71.898 | 1.00x |
| flat.json | orjson | 0.552 | 0.587 | 0.624 | 71.898 | 0.84x |
| flat.json | msgspec | 0.660 | 0.698 | 0.719 | 71.898 | 0.71x |
| flat.json | ujson | 1.452 | 1.528 | 1.547 | 71.898 | 0.32x |
| flat.json | json | 2.070 | 2.130 | 2.160 | 71.898 | 0.23x |
| nested.json | strata | 0.357 | 0.366 | 0.401 | 71.898 | 1.00x |
| nested.json | orjson | 0.451 | 0.468 | 0.484 | 71.898 | 0.78x |
| nested.json | msgspec | 0.569 | 0.596 | 0.609 | 71.898 | 0.61x |
| nested.json | ujson | 1.556 | 1.571 | 1.591 | 71.898 | 0.23x |
| nested.json | json | 2.537 | 2.554 | 2.611 | 71.898 | 0.14x |
| wide_arrays.json | strata | 2.166 | 2.222 | 2.271 | 75.016 | 1.00x |
| wide_arrays.json | orjson | 2.410 | 2.449 | 2.495 | 75.016 | 0.91x |
| wide_arrays.json | msgspec | 3.470 | 3.490 | 3.530 | 75.016 | 0.64x |
| wide_arrays.json | ujson | 9.520 | 9.588 | 9.663 | 75.016 | 0.23x |
| wide_arrays.json | json | 17.294 | 17.478 | 19.458 | 75.016 | 0.13x |
| mixed.json | strata | 0.164 | 0.174 | 18.485 | 75.016 | 1.00x |
| mixed.json | orjson | 0.185 | 0.192 | 0.257 | 75.016 | 0.91x |
| mixed.json | msgspec | 0.192 | 0.207 | 0.216 | 75.016 | 0.84x |
| mixed.json | ujson | 0.437 | 0.448 | 0.463 | 75.016 | 0.39x |
| mixed.json | json | 0.660 | 0.677 | 0.717 | 75.016 | 0.26x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.077 | 0.082 | 0.091 | 69.984 | 1.00x |
| users.json $[*].id | jmespath | 0.485 | 0.499 | 0.517 | 69.984 | 0.16x |
| users.json $[*].id | jsonpath-ng | 2.835 | 3.084 | 3.159 | 69.984 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.426 | 0.454 | 0.478 | 70.992 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.105 | 3.167 | 3.252 | 70.992 | 0.14x |
| users.json $[*].orders[*].total | jsonpath-ng | 20.050 | 20.397 | 20.802 | 70.992 | 0.02x |
| users.json $..total | strata | 1.832 | 1.918 | 2.076 | 73.023 | 1.00x |
| users.json $..total | jsonpath-ng | 387.326 | 390.004 | 395.766 | 73.023 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.307 | 3.317 | 3.367 | 70.992 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.592 | 15.762 | 15.982 | 70.992 | 0.21x |
| users.json $[*].id | orjson+jsonpath-ng | 18.035 | 18.283 | 18.819 | 70.992 | 0.18x |
| users.json $[*].orders[*].total | strata | 3.549 | 3.607 | 3.658 | 72.984 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.820 | 19.149 | 20.418 | 72.984 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 40.598 | 43.171 | 47.175 | 72.984 | 0.08x |
| users.json $..total | strata | 16.281 | 16.983 | 17.791 | 73.023 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 414.343 | 415.241 | 420.883 | 73.023 | 0.04x |

