# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c20ac86eedff410e10c973bc3b1f19f6e9a5f56e
- python: 3.12.10
- implementation: CPython
- platform: macOS-15.7.9-x86_64-i386-64bit
- machine: x86_64
- processor: Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz
- compiler_flags: -fno-strict-overflow -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -O3 -Wall -arch x86_64 -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -fomit-frame-pointer -march=native -flto=thin -fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata -bundle -undefined (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 19.604 | 19.933 | 24.675 | 57.098 | 1.00x |
| users.json | orjson | 28.013 | 29.562 | 34.992 | 57.098 | 0.67x |
| users.json | msgspec | 28.179 | 30.128 | 35.306 | 57.098 | 0.66x |
| users.json | ujson | 40.007 | 43.238 | 54.460 | 57.098 | 0.46x |
| users.json | pysimdjson | 180.392 | 183.310 | 203.364 | 57.098 | 0.11x |
| users.json | json | 46.615 | 48.283 | 53.746 | 57.098 | 0.41x |
| flat.json | strata | 1.303 | 1.342 | 1.380 | 64.367 | 1.00x |
| flat.json | orjson | 1.429 | 1.479 | 1.552 | 64.367 | 0.91x |
| flat.json | msgspec | 1.625 | 1.656 | 1.796 | 64.367 | 0.81x |
| flat.json | ujson | 2.796 | 2.902 | 3.210 | 64.367 | 0.46x |
| flat.json | pysimdjson | 15.146 | 15.433 | 16.698 | 64.367 | 0.09x |
| flat.json | json | 3.265 | 3.346 | 3.632 | 64.367 | 0.40x |
| nested.json | strata | 1.695 | 1.776 | 2.042 | 63.480 | 1.00x |
| nested.json | orjson | 1.934 | 2.023 | 2.238 | 63.480 | 0.88x |
| nested.json | msgspec | 2.191 | 2.356 | 3.360 | 63.480 | 0.75x |
| nested.json | ujson | 3.521 | 3.784 | 4.200 | 63.480 | 0.47x |
| nested.json | pysimdjson | 15.387 | 16.582 | 17.809 | 63.480 | 0.11x |
| nested.json | json | 4.584 | 4.912 | 5.322 | 63.480 | 0.36x |
| wide_arrays.json | strata | 8.286 | 8.668 | 9.099 | 69.453 | 1.00x |
| wide_arrays.json | orjson | 10.413 | 10.905 | 11.808 | 69.453 | 0.79x |
| wide_arrays.json | msgspec | 11.264 | 11.716 | 12.469 | 69.453 | 0.74x |
| wide_arrays.json | ujson | 14.429 | 14.942 | 15.506 | 69.453 | 0.58x |
| wide_arrays.json | pysimdjson | 88.746 | 91.686 | 97.064 | 69.453 | 0.09x |
| wide_arrays.json | json | 18.811 | 19.264 | 20.907 | 69.453 | 0.45x |
| mixed.json | strata | 0.370 | 0.395 | 0.418 | 62.246 | 1.00x |
| mixed.json | orjson | 0.453 | 0.491 | 0.517 | 62.246 | 0.80x |
| mixed.json | msgspec | 0.478 | 0.517 | 0.540 | 62.246 | 0.76x |
| mixed.json | ujson | 0.663 | 0.714 | 0.759 | 62.246 | 0.55x |
| mixed.json | pysimdjson | 3.363 | 3.642 | 3.765 | 62.246 | 0.11x |
| mixed.json | json | 0.932 | 1.018 | 1.176 | 62.246 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.027 | 3.534 | 5.284 | 52.387 | 1.00x |
| users.json | orjson | 3.910 | 4.553 | 7.152 | 52.387 | 0.78x |
| users.json | msgspec | 6.168 | 6.543 | 9.713 | 52.387 | 0.54x |
| users.json | ujson | 28.940 | 32.202 | 44.682 | 52.387 | 0.11x |
| users.json | json | 49.335 | 53.012 | 92.039 | 52.387 | 0.07x |
| flat.json | strata | 0.357 | 0.372 | 0.379 | 62.883 | 1.00x |
| flat.json | orjson | 0.442 | 0.449 | 0.498 | 62.883 | 0.83x |
| flat.json | msgspec | 0.599 | 0.609 | 0.649 | 62.883 | 0.61x |
| flat.json | ujson | 2.344 | 2.458 | 2.507 | 62.883 | 0.15x |
| flat.json | json | 4.068 | 4.132 | 4.217 | 62.883 | 0.09x |
| nested.json | strata | 0.301 | 0.327 | 0.380 | 63.621 | 1.00x |
| nested.json | orjson | 0.429 | 0.464 | 0.501 | 63.621 | 0.70x |
| nested.json | msgspec | 0.686 | 0.717 | 1.245 | 63.621 | 0.46x |
| nested.json | ujson | 2.903 | 3.023 | 3.334 | 63.621 | 0.11x |
| nested.json | json | 5.782 | 5.953 | 6.796 | 63.621 | 0.05x |
| wide_arrays.json | strata | 1.895 | 2.015 | 2.374 | 63.180 | 1.00x |
| wide_arrays.json | orjson | 2.424 | 2.681 | 2.803 | 63.180 | 0.75x |
| wide_arrays.json | msgspec | 3.534 | 3.759 | 4.020 | 63.180 | 0.54x |
| wide_arrays.json | ujson | 11.346 | 12.058 | 12.326 | 63.180 | 0.17x |
| wide_arrays.json | json | 36.771 | 37.312 | 39.904 | 63.180 | 0.05x |
| mixed.json | strata | 0.074 | 0.082 | 0.087 | 59.047 | 1.00x |
| mixed.json | orjson | 0.094 | 0.098 | 0.101 | 59.047 | 0.84x |
| mixed.json | msgspec | 0.125 | 0.139 | 0.144 | 59.047 | 0.59x |
| mixed.json | ujson | 0.499 | 0.523 | 0.570 | 59.047 | 0.16x |
| mixed.json | json | 1.034 | 1.088 | 1.127 | 59.047 | 0.08x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 22.889 | 24.232 | 26.038 | 63.223 | 1.00x |
| users.json | orjson | 33.590 | 39.118 | 46.392 | 63.223 | 0.62x |
| users.json | msgspec | 35.225 | 38.536 | 47.092 | 63.223 | 0.63x |
| users.json | ujson | 50.598 | 58.567 | 76.751 | 63.223 | 0.41x |
| users.json | json | 56.717 | 60.659 | 135.397 | 63.223 | 0.40x |
| flat.json | strata | 1.466 | 1.524 | 1.648 | 63.520 | 1.00x |
| flat.json | orjson | 1.657 | 1.743 | 2.003 | 63.520 | 0.87x |
| flat.json | msgspec | 1.910 | 1.945 | 2.001 | 63.520 | 0.78x |
| flat.json | ujson | 3.212 | 3.330 | 3.805 | 63.520 | 0.46x |
| flat.json | json | 3.599 | 3.705 | 4.144 | 63.520 | 0.41x |
| nested.json | strata | 1.866 | 1.920 | 1.987 | 63.621 | 1.00x |
| nested.json | orjson | 2.179 | 2.223 | 2.546 | 63.621 | 0.86x |
| nested.json | msgspec | 2.432 | 2.471 | 2.611 | 63.621 | 0.78x |
| nested.json | ujson | 3.878 | 3.968 | 4.150 | 63.621 | 0.48x |
| nested.json | json | 4.919 | 5.024 | 5.416 | 63.621 | 0.38x |
| wide_arrays.json | strata | 8.218 | 8.472 | 8.868 | 64.410 | 1.00x |
| wide_arrays.json | orjson | 9.688 | 10.419 | 10.926 | 64.410 | 0.81x |
| wide_arrays.json | msgspec | 10.828 | 11.673 | 11.819 | 64.410 | 0.73x |
| wide_arrays.json | ujson | 14.547 | 15.143 | 15.664 | 64.410 | 0.56x |
| wide_arrays.json | json | 19.710 | 19.871 | 20.314 | 64.410 | 0.43x |
| mixed.json | strata | 0.467 | 0.517 | 0.539 | 59.047 | 1.00x |
| mixed.json | orjson | 0.601 | 0.647 | 0.682 | 59.047 | 0.80x |
| mixed.json | msgspec | 0.630 | 0.700 | 0.750 | 59.047 | 0.74x |
| mixed.json | ujson | 0.829 | 0.896 | 0.971 | 59.047 | 0.58x |
| mixed.json | json | 1.052 | 1.147 | 1.215 | 59.047 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 18.611 | 19.447 | 22.159 | 63.945 | 1.00x |
| users.ndjson | orjson | 26.942 | 28.049 | 33.937 | 63.945 | 0.69x |
| users.ndjson | msgspec | 27.685 | 28.274 | 31.528 | 63.945 | 0.69x |
| users.ndjson | ujson | 39.403 | 40.629 | 44.397 | 63.945 | 0.48x |
| users.ndjson | json | 49.381 | 50.951 | 59.051 | 63.945 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.812 | 5.254 | 7.045 | 63.266 | 1.00x |
| users.json | orjson | 6.430 | 6.726 | 7.707 | 63.266 | 0.78x |
| users.json | msgspec | 8.303 | 9.180 | 12.506 | 63.266 | 0.57x |
| users.json | ujson | 36.745 | 38.997 | 44.539 | 63.266 | 0.13x |
| users.json | json | 59.496 | 61.266 | 68.131 | 63.266 | 0.09x |
| flat.json | strata | 0.743 | 0.772 | 0.925 | 63.520 | 1.00x |
| flat.json | orjson | 0.818 | 0.848 | 0.944 | 63.520 | 0.91x |
| flat.json | msgspec | 0.989 | 1.041 | 1.311 | 63.520 | 0.74x |
| flat.json | ujson | 2.926 | 2.978 | 3.085 | 63.520 | 0.26x |
| flat.json | json | 4.535 | 4.599 | 4.901 | 63.520 | 0.17x |
| nested.json | strata | 0.634 | 0.684 | 1.297 | 63.621 | 1.00x |
| nested.json | orjson | 0.827 | 0.840 | 1.061 | 63.621 | 0.81x |
| nested.json | msgspec | 1.074 | 1.106 | 1.367 | 63.621 | 0.62x |
| nested.json | ujson | 3.347 | 3.441 | 3.889 | 63.621 | 0.20x |
| nested.json | json | 6.240 | 6.365 | 6.552 | 63.621 | 0.11x |
| wide_arrays.json | strata | 2.515 | 2.643 | 2.889 | 63.180 | 1.00x |
| wide_arrays.json | orjson | 3.161 | 3.347 | 3.754 | 63.180 | 0.79x |
| wide_arrays.json | msgspec | 4.269 | 4.532 | 4.755 | 63.180 | 0.58x |
| wide_arrays.json | ujson | 12.135 | 12.584 | 13.346 | 63.180 | 0.21x |
| wide_arrays.json | json | 36.536 | 36.746 | 40.871 | 63.180 | 0.07x |
| mixed.json | strata | 0.351 | 0.431 | 0.514 | 59.047 | 1.00x |
| mixed.json | orjson | 0.390 | 0.468 | 0.519 | 59.047 | 0.92x |
| mixed.json | msgspec | 0.411 | 0.523 | 0.570 | 59.047 | 0.82x |
| mixed.json | ujson | 0.868 | 0.919 | 1.037 | 59.047 | 0.47x |
| mixed.json | json | 1.369 | 1.511 | 1.579 | 59.047 | 0.29x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.229 | 0.277 | 0.357 | 63.324 | 1.00x |
| users.json $[*].id | jmespath | 1.318 | 1.489 | 2.011 | 63.324 | 0.19x |
| users.json $[*].id | jsonpath-ng | 7.203 | 7.598 | 8.489 | 63.324 | 0.04x |
| users.json $[*].orders[*].total | strata | 1.184 | 1.259 | 1.627 | 60.504 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 7.845 | 8.111 | 9.981 | 60.504 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 46.289 | 47.446 | 48.930 | 60.504 | 0.03x |
| users.json $..total | strata | 3.501 | 3.741 | 4.514 | 60.555 | 1.00x |
| users.json $..total | jsonpath-ng | 790.071 | 805.397 | 883.103 | 60.555 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.974 | 5.353 | 8.078 | 63.379 | 1.00x |
| users.json $[*].id | orjson+jmespath | 37.574 | 42.213 | 45.555 | 63.379 | 0.13x |
| users.json $[*].id | orjson+jsonpath-ng | 41.022 | 47.166 | 54.917 | 63.379 | 0.11x |
| users.json $[*].orders[*].total | strata | 5.401 | 5.457 | 6.633 | 60.504 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 40.445 | 41.389 | 42.859 | 60.504 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 84.196 | 86.009 | 94.458 | 60.504 | 0.06x |
| users.json $..total | strata | 22.898 | 24.576 | 27.743 | 60.586 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 745.119 | 805.649 | 844.028 | 60.586 | 0.03x |

