# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c16eaa65c61f1cf9d4fac46e9f6d8e6b9f3b087f
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
| users.json | strata | 19.015 | 19.736 | 22.322 | 57.043 | 1.00x |
| users.json | orjson | 26.420 | 28.013 | 30.696 | 57.043 | 0.70x |
| users.json | msgspec | 27.294 | 28.063 | 30.396 | 57.043 | 0.70x |
| users.json | ujson | 39.234 | 40.540 | 45.239 | 57.043 | 0.49x |
| users.json | pysimdjson | 171.381 | 175.265 | 180.064 | 57.043 | 0.11x |
| users.json | json | 44.534 | 45.930 | 47.146 | 57.043 | 0.43x |
| flat.json | strata | 1.325 | 1.345 | 1.717 | 66.691 | 1.00x |
| flat.json | orjson | 1.474 | 1.490 | 1.538 | 66.691 | 0.90x |
| flat.json | msgspec | 1.684 | 1.705 | 1.822 | 66.691 | 0.79x |
| flat.json | ujson | 2.956 | 2.986 | 3.467 | 66.691 | 0.45x |
| flat.json | pysimdjson | 15.952 | 16.014 | 16.214 | 66.691 | 0.08x |
| flat.json | json | 3.394 | 3.415 | 3.467 | 66.691 | 0.39x |
| nested.json | strata | 1.607 | 1.672 | 1.960 | 63.207 | 1.00x |
| nested.json | orjson | 1.853 | 1.904 | 2.009 | 63.207 | 0.88x |
| nested.json | msgspec | 1.989 | 2.112 | 2.239 | 63.207 | 0.79x |
| nested.json | ujson | 3.292 | 3.420 | 3.787 | 63.207 | 0.49x |
| nested.json | pysimdjson | 14.630 | 15.021 | 15.556 | 63.207 | 0.11x |
| nested.json | json | 4.246 | 4.348 | 4.557 | 63.207 | 0.38x |
| wide_arrays.json | strata | 8.289 | 8.379 | 8.610 | 67.617 | 1.00x |
| wide_arrays.json | orjson | 10.006 | 10.061 | 10.637 | 67.617 | 0.83x |
| wide_arrays.json | msgspec | 10.979 | 11.217 | 11.623 | 67.617 | 0.75x |
| wide_arrays.json | ujson | 13.945 | 14.139 | 15.635 | 67.617 | 0.59x |
| wide_arrays.json | pysimdjson | 86.540 | 86.828 | 87.721 | 67.617 | 0.10x |
| wide_arrays.json | json | 18.385 | 18.465 | 19.288 | 67.617 | 0.45x |
| mixed.json | strata | 0.381 | 0.385 | 0.406 | 65.520 | 1.00x |
| mixed.json | orjson | 0.467 | 0.475 | 0.515 | 65.520 | 0.81x |
| mixed.json | msgspec | 0.492 | 0.506 | 0.547 | 65.520 | 0.76x |
| mixed.json | ujson | 0.675 | 0.693 | 0.755 | 65.520 | 0.55x |
| mixed.json | pysimdjson | 3.490 | 3.516 | 3.561 | 65.520 | 0.11x |
| mixed.json | json | 0.962 | 0.970 | 1.097 | 65.520 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.479 | 2.557 | 2.745 | 52.883 | 1.00x |
| users.json | orjson | 3.473 | 3.553 | 3.979 | 52.883 | 0.72x |
| users.json | msgspec | 5.314 | 5.469 | 5.531 | 52.883 | 0.47x |
| users.json | ujson | 26.251 | 26.424 | 26.803 | 52.883 | 0.10x |
| users.json | json | 44.477 | 44.791 | 46.035 | 52.883 | 0.06x |
| flat.json | strata | 0.334 | 0.348 | 0.366 | 63.078 | 1.00x |
| flat.json | orjson | 0.414 | 0.433 | 0.471 | 63.078 | 0.80x |
| flat.json | msgspec | 0.534 | 0.566 | 0.597 | 63.078 | 0.61x |
| flat.json | ujson | 2.438 | 2.472 | 2.663 | 63.078 | 0.14x |
| flat.json | json | 3.972 | 4.000 | 4.317 | 63.078 | 0.09x |
| nested.json | strata | 0.255 | 0.266 | 0.275 | 56.875 | 1.00x |
| nested.json | orjson | 0.380 | 0.393 | 0.415 | 56.875 | 0.68x |
| nested.json | msgspec | 0.592 | 0.605 | 0.756 | 56.875 | 0.44x |
| nested.json | ujson | 2.587 | 2.603 | 2.885 | 56.875 | 0.10x |
| nested.json | json | 5.033 | 5.183 | 5.570 | 56.875 | 0.05x |
| wide_arrays.json | strata | 1.903 | 2.264 | 2.390 | 63.641 | 1.00x |
| wide_arrays.json | orjson | 2.504 | 2.863 | 3.085 | 63.641 | 0.79x |
| wide_arrays.json | msgspec | 3.529 | 3.916 | 4.140 | 63.641 | 0.58x |
| wide_arrays.json | ujson | 11.303 | 11.800 | 14.206 | 63.641 | 0.19x |
| wide_arrays.json | json | 37.830 | 39.089 | 42.337 | 63.641 | 0.06x |
| mixed.json | strata | 0.065 | 0.068 | 0.086 | 62.281 | 1.00x |
| mixed.json | orjson | 0.081 | 0.086 | 0.097 | 62.281 | 0.79x |
| mixed.json | msgspec | 0.116 | 0.126 | 0.137 | 62.281 | 0.54x |
| mixed.json | ujson | 0.495 | 0.497 | 0.506 | 62.281 | 0.14x |
| mixed.json | json | 1.037 | 1.063 | 1.178 | 62.281 | 0.06x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 19.107 | 19.560 | 20.723 | 63.688 | 1.00x |
| users.json | orjson | 26.047 | 27.126 | 27.714 | 63.688 | 0.72x |
| users.json | msgspec | 26.736 | 27.714 | 29.403 | 63.688 | 0.71x |
| users.json | ujson | 39.462 | 40.801 | 42.381 | 63.688 | 0.48x |
| users.json | json | 44.958 | 46.217 | 47.527 | 63.688 | 0.42x |
| flat.json | strata | 1.397 | 1.448 | 1.549 | 63.078 | 1.00x |
| flat.json | orjson | 1.582 | 1.648 | 1.751 | 63.078 | 0.88x |
| flat.json | msgspec | 1.821 | 1.883 | 1.991 | 63.078 | 0.77x |
| flat.json | ujson | 3.154 | 3.190 | 3.344 | 63.078 | 0.45x |
| flat.json | json | 3.487 | 3.559 | 3.599 | 63.078 | 0.41x |
| nested.json | strata | 1.696 | 1.720 | 1.905 | 57.156 | 1.00x |
| nested.json | orjson | 1.965 | 1.988 | 2.460 | 57.156 | 0.87x |
| nested.json | msgspec | 2.173 | 2.203 | 2.310 | 57.156 | 0.78x |
| nested.json | ujson | 3.462 | 3.503 | 3.573 | 57.156 | 0.49x |
| nested.json | json | 4.361 | 4.392 | 4.470 | 57.156 | 0.39x |
| wide_arrays.json | strata | 8.141 | 8.225 | 8.661 | 65.793 | 1.00x |
| wide_arrays.json | orjson | 9.773 | 9.958 | 10.890 | 65.793 | 0.83x |
| wide_arrays.json | msgspec | 11.169 | 11.244 | 11.877 | 65.793 | 0.73x |
| wide_arrays.json | ujson | 14.237 | 14.437 | 14.971 | 65.793 | 0.57x |
| wide_arrays.json | json | 18.389 | 18.674 | 19.322 | 65.793 | 0.44x |
| mixed.json | strata | 0.458 | 0.464 | 0.493 | 62.281 | 1.00x |
| mixed.json | orjson | 0.591 | 0.603 | 0.705 | 62.281 | 0.77x |
| mixed.json | msgspec | 0.619 | 0.634 | 0.706 | 62.281 | 0.73x |
| mixed.json | ujson | 0.819 | 0.837 | 0.882 | 62.281 | 0.55x |
| mixed.json | json | 1.072 | 1.080 | 1.127 | 62.281 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 20.007 | 20.697 | 21.370 | 65.160 | 1.00x |
| users.ndjson | orjson | 28.569 | 29.557 | 31.611 | 65.160 | 0.70x |
| users.ndjson | msgspec | 28.891 | 29.726 | 30.070 | 65.160 | 0.70x |
| users.ndjson | ujson | 42.507 | 43.196 | 44.584 | 65.160 | 0.48x |
| users.ndjson | json | 53.038 | 54.282 | 55.039 | 65.160 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.432 | 3.829 | 5.649 | 63.703 | 1.00x |
| users.json | orjson | 4.205 | 5.009 | 6.733 | 63.703 | 0.76x |
| users.json | msgspec | 6.371 | 7.236 | 8.554 | 63.703 | 0.53x |
| users.json | ujson | 27.077 | 29.690 | 35.879 | 63.703 | 0.13x |
| users.json | json | 45.430 | 50.972 | 95.304 | 63.703 | 0.08x |
| flat.json | strata | 0.697 | 0.793 | 0.886 | 63.078 | 1.00x |
| flat.json | orjson | 0.860 | 0.884 | 1.156 | 63.078 | 0.90x |
| flat.json | msgspec | 1.000 | 1.032 | 1.146 | 63.078 | 0.77x |
| flat.json | ujson | 2.909 | 3.022 | 3.351 | 63.078 | 0.26x |
| flat.json | json | 4.506 | 4.603 | 5.155 | 63.078 | 0.17x |
| nested.json | strata | 0.581 | 0.592 | 0.665 | 57.156 | 1.00x |
| nested.json | orjson | 0.697 | 0.751 | 0.787 | 57.156 | 0.79x |
| nested.json | msgspec | 0.954 | 0.975 | 1.055 | 57.156 | 0.61x |
| nested.json | ujson | 2.937 | 2.973 | 3.177 | 57.156 | 0.20x |
| nested.json | json | 5.420 | 5.538 | 5.750 | 57.156 | 0.11x |
| wide_arrays.json | strata | 2.555 | 2.798 | 2.945 | 65.793 | 1.00x |
| wide_arrays.json | orjson | 3.320 | 3.409 | 3.626 | 65.793 | 0.82x |
| wide_arrays.json | msgspec | 4.449 | 4.551 | 4.713 | 65.793 | 0.61x |
| wide_arrays.json | ujson | 12.480 | 12.510 | 12.626 | 65.793 | 0.22x |
| wide_arrays.json | json | 38.394 | 38.852 | 39.433 | 65.793 | 0.07x |
| mixed.json | strata | 0.316 | 0.341 | 0.400 | 62.281 | 1.00x |
| mixed.json | orjson | 0.358 | 0.391 | 0.453 | 62.281 | 0.87x |
| mixed.json | msgspec | 0.408 | 0.439 | 0.510 | 62.281 | 0.78x |
| mixed.json | ujson | 0.799 | 0.859 | 0.915 | 62.281 | 0.40x |
| mixed.json | json | 1.322 | 1.388 | 1.429 | 62.281 | 0.25x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.147 | 0.164 | 0.196 | 63.770 | 1.00x |
| users.json $[*].id | jmespath | 1.045 | 1.057 | 1.114 | 63.770 | 0.16x |
| users.json $[*].id | jsonpath-ng | 5.769 | 5.887 | 6.584 | 63.770 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.904 | 0.938 | 1.101 | 61.219 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 6.242 | 6.439 | 7.461 | 61.219 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 36.854 | 38.210 | 39.613 | 61.219 | 0.02x |
| users.json $..total | strata | 3.312 | 3.521 | 3.751 | 61.266 | 1.00x |
| users.json $..total | jsonpath-ng | 730.046 | 766.672 | 884.296 | 61.266 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.429 | 4.483 | 4.731 | 63.871 | 1.00x |
| users.json $[*].id | orjson+jmespath | 29.512 | 30.003 | 31.933 | 63.871 | 0.15x |
| users.json $[*].id | orjson+jsonpath-ng | 33.184 | 34.446 | 35.735 | 63.871 | 0.13x |
| users.json $[*].orders[*].total | strata | 4.490 | 4.531 | 4.862 | 61.234 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 33.386 | 34.285 | 36.191 | 61.234 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 67.394 | 70.601 | 77.114 | 61.234 | 0.06x |
| users.json $..total | strata | 22.950 | 24.112 | 25.221 | 61.328 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 763.505 | 792.120 | 823.878 | 61.328 | 0.03x |

