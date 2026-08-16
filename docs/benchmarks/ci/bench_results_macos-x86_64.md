# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 04160cb
- python: 3.12.10
- implementation: CPython
- platform: macOS-15.7.7-x86_64-i386-64bit
- machine: x86_64
- processor: Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 23.968 | 26.759 | 30.004 | 52.629 | 1.00x |
| users.json | orjson | 31.162 | 34.169 | 39.115 | 52.629 | 0.78x |
| users.json | msgspec | 32.013 | 33.636 | 40.363 | 52.629 | 0.80x |
| users.json | ujson | 45.636 | 49.694 | 59.070 | 52.629 | 0.54x |
| users.json | pysimdjson | 201.984 | 211.948 | 238.378 | 52.629 | 0.13x |
| users.json | json | 52.270 | 55.626 | 64.440 | 52.629 | 0.48x |
| flat.json | strata | 1.527 | 1.544 | 2.081 | 61.473 | 1.00x |
| flat.json | orjson | 1.597 | 1.629 | 2.037 | 61.473 | 0.95x |
| flat.json | msgspec | 1.838 | 1.860 | 2.226 | 61.473 | 0.83x |
| flat.json | ujson | 3.208 | 3.313 | 4.108 | 61.473 | 0.47x |
| flat.json | pysimdjson | 17.172 | 17.489 | 18.360 | 61.473 | 0.09x |
| flat.json | json | 3.667 | 3.783 | 4.149 | 61.473 | 0.41x |
| nested.json | strata | 1.785 | 1.808 | 2.265 | 59.293 | 1.00x |
| nested.json | orjson | 1.970 | 1.999 | 2.376 | 59.293 | 0.90x |
| nested.json | msgspec | 2.150 | 2.226 | 2.509 | 59.293 | 0.81x |
| nested.json | ujson | 3.547 | 3.753 | 4.593 | 59.293 | 0.48x |
| nested.json | pysimdjson | 15.604 | 15.914 | 17.986 | 59.293 | 0.11x |
| nested.json | json | 4.558 | 4.750 | 5.498 | 59.293 | 0.38x |
| wide_arrays.json | strata | 9.999 | 10.333 | 11.569 | 61.930 | 1.00x |
| wide_arrays.json | orjson | 10.696 | 11.751 | 14.688 | 61.930 | 0.88x |
| wide_arrays.json | msgspec | 11.751 | 12.467 | 19.770 | 61.930 | 0.83x |
| wide_arrays.json | ujson | 15.046 | 16.580 | 48.658 | 61.930 | 0.62x |
| wide_arrays.json | pysimdjson | 92.819 | 101.601 | 136.392 | 61.930 | 0.10x |
| wide_arrays.json | json | 19.616 | 21.591 | 31.445 | 61.930 | 0.48x |
| mixed.json | strata | 0.494 | 0.620 | 0.898 | 59.527 | 1.00x |
| mixed.json | orjson | 0.552 | 0.742 | 0.972 | 59.527 | 0.84x |
| mixed.json | msgspec | 0.589 | 0.643 | 0.907 | 59.527 | 0.97x |
| mixed.json | ujson | 0.813 | 0.830 | 1.527 | 59.527 | 0.75x |
| mixed.json | pysimdjson | 4.032 | 4.517 | 6.286 | 59.527 | 0.14x |
| mixed.json | json | 1.115 | 1.286 | 2.049 | 59.527 | 0.48x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.085 | 3.389 | 4.508 | 48.215 | 1.00x |
| users.json | orjson | 4.143 | 4.481 | 5.557 | 48.215 | 0.76x |
| users.json | msgspec | 6.113 | 6.601 | 7.647 | 48.215 | 0.51x |
| users.json | ujson | 37.576 | 39.283 | 43.474 | 48.215 | 0.09x |
| users.json | json | 49.864 | 53.101 | 55.530 | 48.215 | 0.06x |
| flat.json | strata | 0.339 | 0.348 | 0.407 | 59.156 | 1.00x |
| flat.json | orjson | 0.468 | 0.476 | 0.640 | 59.156 | 0.73x |
| flat.json | msgspec | 0.618 | 0.629 | 0.644 | 59.156 | 0.55x |
| flat.json | ujson | 3.041 | 3.086 | 3.180 | 59.156 | 0.11x |
| flat.json | json | 4.216 | 4.238 | 4.473 | 59.156 | 0.08x |
| nested.json | strata | 0.270 | 0.280 | 0.404 | 54.191 | 1.00x |
| nested.json | orjson | 0.392 | 0.428 | 0.533 | 54.191 | 0.65x |
| nested.json | msgspec | 0.637 | 0.675 | 0.854 | 54.191 | 0.41x |
| nested.json | ujson | 3.286 | 3.383 | 3.904 | 54.191 | 0.08x |
| nested.json | json | 5.335 | 5.847 | 6.399 | 54.191 | 0.05x |
| wide_arrays.json | strata | 2.379 | 2.604 | 2.801 | 60.695 | 1.00x |
| wide_arrays.json | orjson | 2.818 | 2.925 | 14.701 | 60.695 | 0.89x |
| wide_arrays.json | msgspec | 3.893 | 4.046 | 5.002 | 60.695 | 0.64x |
| wide_arrays.json | ujson | 18.904 | 19.939 | 31.366 | 60.695 | 0.13x |
| wide_arrays.json | json | 42.183 | 51.742 | 78.675 | 60.695 | 0.05x |
| mixed.json | strata | 0.100 | 0.109 | 0.180 | 55.188 | 1.00x |
| mixed.json | orjson | 0.108 | 0.112 | 0.158 | 55.188 | 0.98x |
| mixed.json | msgspec | 0.149 | 0.156 | 0.213 | 55.188 | 0.70x |
| mixed.json | ujson | 0.775 | 0.798 | 0.912 | 55.188 | 0.14x |
| mixed.json | json | 1.205 | 1.291 | 1.519 | 55.188 | 0.08x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 23.851 | 24.573 | 27.704 | 60.426 | 1.00x |
| users.json | orjson | 32.363 | 33.895 | 38.460 | 60.426 | 0.72x |
| users.json | msgspec | 31.435 | 33.001 | 53.975 | 60.426 | 0.74x |
| users.json | ujson | 46.564 | 49.168 | 71.089 | 60.426 | 0.50x |
| users.json | json | 52.330 | 53.573 | 55.381 | 60.426 | 0.46x |
| flat.json | strata | 1.655 | 1.664 | 2.172 | 59.156 | 1.00x |
| flat.json | orjson | 1.774 | 1.787 | 2.157 | 59.156 | 0.93x |
| flat.json | msgspec | 2.027 | 2.058 | 2.189 | 59.156 | 0.81x |
| flat.json | ujson | 3.426 | 3.451 | 3.581 | 59.156 | 0.48x |
| flat.json | json | 3.835 | 3.854 | 4.027 | 59.156 | 0.43x |
| nested.json | strata | 1.869 | 1.880 | 2.194 | 54.191 | 1.00x |
| nested.json | orjson | 2.094 | 2.128 | 2.488 | 54.191 | 0.88x |
| nested.json | msgspec | 2.317 | 2.365 | 2.811 | 54.191 | 0.80x |
| nested.json | ujson | 3.732 | 3.790 | 4.625 | 54.191 | 0.50x |
| nested.json | json | 4.642 | 4.702 | 5.707 | 54.191 | 0.40x |
| wide_arrays.json | strata | 10.889 | 10.967 | 11.123 | 60.742 | 1.00x |
| wide_arrays.json | orjson | 11.601 | 11.880 | 12.361 | 60.742 | 0.92x |
| wide_arrays.json | msgspec | 12.832 | 13.018 | 14.282 | 60.742 | 0.84x |
| wide_arrays.json | ujson | 16.608 | 16.895 | 21.714 | 60.742 | 0.65x |
| wide_arrays.json | json | 21.037 | 21.283 | 23.035 | 60.742 | 0.52x |
| mixed.json | strata | 0.562 | 0.603 | 0.867 | 55.188 | 1.00x |
| mixed.json | orjson | 0.689 | 0.775 | 1.080 | 55.188 | 0.78x |
| mixed.json | msgspec | 0.732 | 0.757 | 0.914 | 55.188 | 0.80x |
| mixed.json | ujson | 0.979 | 1.004 | 1.051 | 55.188 | 0.60x |
| mixed.json | json | 1.241 | 1.270 | 1.952 | 55.188 | 0.48x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 23.292 | 24.772 | 34.643 | 61.871 | 1.00x |
| users.ndjson | orjson | 30.736 | 32.050 | 38.283 | 61.871 | 0.77x |
| users.ndjson | msgspec | 31.516 | 32.360 | 36.099 | 61.871 | 0.77x |
| users.ndjson | ujson | 45.267 | 46.807 | 82.247 | 61.871 | 0.53x |
| users.ndjson | json | 56.920 | 59.236 | 116.506 | 61.871 | 0.42x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.235 | 4.731 | 5.385 | 57.500 | 1.00x |
| users.json | orjson | 5.387 | 5.715 | 6.790 | 57.500 | 0.83x |
| users.json | msgspec | 7.218 | 7.509 | 7.728 | 57.500 | 0.63x |
| users.json | ujson | 38.674 | 39.388 | 41.541 | 57.500 | 0.12x |
| users.json | json | 49.253 | 51.241 | 52.872 | 57.500 | 0.09x |
| flat.json | strata | 0.692 | 0.734 | 1.006 | 59.156 | 1.00x |
| flat.json | orjson | 0.876 | 0.900 | 1.038 | 59.156 | 0.82x |
| flat.json | msgspec | 1.003 | 1.039 | 1.111 | 59.156 | 0.71x |
| flat.json | ujson | 3.470 | 3.525 | 3.929 | 59.156 | 0.21x |
| flat.json | json | 4.602 | 4.697 | 5.110 | 59.156 | 0.16x |
| nested.json | strata | 0.589 | 0.630 | 0.699 | 54.191 | 1.00x |
| nested.json | orjson | 0.690 | 0.759 | 0.811 | 54.191 | 0.83x |
| nested.json | msgspec | 0.955 | 1.007 | 1.093 | 54.191 | 0.63x |
| nested.json | ujson | 3.582 | 3.665 | 4.401 | 54.191 | 0.17x |
| nested.json | json | 5.677 | 5.780 | 5.839 | 54.191 | 0.11x |
| wide_arrays.json | strata | 3.094 | 3.447 | 3.917 | 60.742 | 1.00x |
| wide_arrays.json | orjson | 3.893 | 4.005 | 4.940 | 60.742 | 0.86x |
| wide_arrays.json | msgspec | 4.874 | 5.077 | 6.805 | 60.742 | 0.68x |
| wide_arrays.json | ujson | 19.815 | 20.142 | 22.296 | 60.742 | 0.17x |
| wide_arrays.json | json | 42.820 | 43.907 | 60.128 | 60.742 | 0.08x |
| mixed.json | strata | 0.430 | 0.467 | 0.569 | 55.188 | 1.00x |
| mixed.json | orjson | 0.472 | 0.548 | 0.628 | 55.188 | 0.85x |
| mixed.json | msgspec | 0.534 | 0.575 | 0.868 | 55.188 | 0.81x |
| mixed.json | ujson | 1.165 | 1.295 | 2.056 | 55.188 | 0.36x |
| mixed.json | json | 1.608 | 1.668 | 1.958 | 55.188 | 0.28x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.159 | 0.205 | 0.224 | 57.562 | 1.00x |
| users.json $[*].id | jmespath | 1.083 | 1.156 | 1.301 | 57.562 | 0.18x |
| users.json $[*].id | jsonpath-ng | 5.968 | 6.286 | 6.536 | 57.562 | 0.03x |
| users.json $[*].orders[*].total | strata | 1.073 | 1.412 | 1.783 | 54.766 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 7.069 | 7.702 | 11.441 | 54.766 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 41.730 | 46.982 | 52.001 | 54.766 | 0.03x |
| users.json $..total | strata | 4.033 | 4.132 | 4.718 | 56.816 | 1.00x |
| users.json $..total | jsonpath-ng | 838.720 | 857.386 | 1178.970 | 56.816 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.915 | 5.029 | 5.960 | 57.621 | 1.00x |
| users.json $[*].id | orjson+jmespath | 32.487 | 34.366 | 46.226 | 57.621 | 0.15x |
| users.json $[*].id | orjson+jsonpath-ng | 37.646 | 38.781 | 44.051 | 57.621 | 0.13x |
| users.json $[*].orders[*].total | strata | 5.449 | 5.569 | 12.004 | 56.047 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 39.355 | 41.140 | 59.475 | 56.047 | 0.14x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 84.011 | 87.902 | 193.959 | 56.047 | 0.06x |
| users.json $..total | strata | 27.978 | 29.136 | 31.114 | 56.117 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 885.622 | 911.346 | 1171.816 | 56.117 | 0.03x |

