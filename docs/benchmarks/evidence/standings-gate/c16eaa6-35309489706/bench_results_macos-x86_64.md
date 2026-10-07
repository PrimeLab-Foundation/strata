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
| users.json | strata | 17.149 | 19.344 | 22.181 | 57.055 | 1.00x |
| users.json | orjson | 25.101 | 27.581 | 31.449 | 57.055 | 0.70x |
| users.json | msgspec | 25.704 | 27.429 | 30.415 | 57.055 | 0.71x |
| users.json | ujson | 35.016 | 39.890 | 48.432 | 57.055 | 0.48x |
| users.json | pysimdjson | 157.134 | 171.679 | 188.295 | 57.055 | 0.11x |
| users.json | json | 42.087 | 45.764 | 53.376 | 57.055 | 0.42x |
| flat.json | strata | 1.113 | 1.182 | 1.228 | 66.008 | 1.00x |
| flat.json | orjson | 1.207 | 1.315 | 1.490 | 66.008 | 0.90x |
| flat.json | msgspec | 1.357 | 1.469 | 1.532 | 66.008 | 0.81x |
| flat.json | ujson | 2.371 | 2.566 | 2.943 | 66.008 | 0.46x |
| flat.json | pysimdjson | 13.688 | 13.845 | 14.447 | 66.008 | 0.09x |
| flat.json | json | 2.911 | 2.957 | 3.069 | 66.008 | 0.40x |
| nested.json | strata | 1.350 | 1.371 | 1.480 | 62.512 | 1.00x |
| nested.json | orjson | 1.539 | 1.560 | 1.962 | 62.512 | 0.88x |
| nested.json | msgspec | 1.705 | 1.721 | 1.821 | 62.512 | 0.80x |
| nested.json | ujson | 2.818 | 2.854 | 3.358 | 62.512 | 0.48x |
| nested.json | pysimdjson | 12.619 | 12.660 | 13.253 | 62.512 | 0.11x |
| nested.json | json | 3.614 | 3.656 | 3.746 | 62.512 | 0.37x |
| wide_arrays.json | strata | 7.239 | 7.548 | 7.843 | 66.926 | 1.00x |
| wide_arrays.json | orjson | 8.743 | 9.311 | 10.051 | 66.926 | 0.81x |
| wide_arrays.json | msgspec | 9.651 | 10.100 | 10.547 | 66.926 | 0.75x |
| wide_arrays.json | ujson | 12.100 | 12.713 | 13.339 | 66.926 | 0.59x |
| wide_arrays.json | pysimdjson | 74.795 | 76.087 | 82.189 | 66.926 | 0.10x |
| wide_arrays.json | json | 15.835 | 16.698 | 17.645 | 66.926 | 0.45x |
| mixed.json | strata | 0.336 | 0.347 | 0.357 | 64.828 | 1.00x |
| mixed.json | orjson | 0.420 | 0.431 | 0.447 | 64.828 | 0.80x |
| mixed.json | msgspec | 0.433 | 0.450 | 0.465 | 64.828 | 0.77x |
| mixed.json | ujson | 0.563 | 0.616 | 0.662 | 64.828 | 0.56x |
| mixed.json | pysimdjson | 3.025 | 3.166 | 3.305 | 64.828 | 0.11x |
| mixed.json | json | 0.837 | 0.896 | 0.923 | 64.828 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.325 | 2.530 | 2.975 | 52.977 | 1.00x |
| users.json | orjson | 3.244 | 3.479 | 4.861 | 52.977 | 0.73x |
| users.json | msgspec | 4.933 | 5.200 | 5.426 | 52.977 | 0.49x |
| users.json | ujson | 23.277 | 23.629 | 26.021 | 52.977 | 0.11x |
| users.json | json | 39.510 | 40.423 | 44.277 | 52.977 | 0.06x |
| flat.json | strata | 0.299 | 0.309 | 0.328 | 62.383 | 1.00x |
| flat.json | orjson | 0.371 | 0.384 | 0.421 | 62.383 | 0.81x |
| flat.json | msgspec | 0.488 | 0.497 | 0.522 | 62.383 | 0.62x |
| flat.json | ujson | 2.060 | 2.071 | 2.374 | 62.383 | 0.15x |
| flat.json | json | 3.402 | 3.417 | 3.499 | 62.383 | 0.09x |
| nested.json | strata | 0.223 | 0.230 | 0.281 | 56.184 | 1.00x |
| nested.json | orjson | 0.306 | 0.333 | 0.366 | 56.184 | 0.69x |
| nested.json | msgspec | 0.480 | 0.526 | 0.656 | 56.184 | 0.44x |
| nested.json | ujson | 2.161 | 2.191 | 2.264 | 56.184 | 0.10x |
| nested.json | json | 4.300 | 4.337 | 5.323 | 56.184 | 0.05x |
| wide_arrays.json | strata | 1.893 | 2.048 | 2.130 | 62.949 | 1.00x |
| wide_arrays.json | orjson | 2.498 | 2.575 | 2.712 | 62.949 | 0.80x |
| wide_arrays.json | msgspec | 3.416 | 3.517 | 3.760 | 62.949 | 0.58x |
| wide_arrays.json | ujson | 10.146 | 10.525 | 11.379 | 62.949 | 0.19x |
| wide_arrays.json | json | 32.529 | 34.170 | 36.883 | 62.949 | 0.06x |
| mixed.json | strata | 0.055 | 0.064 | 0.069 | 61.590 | 1.00x |
| mixed.json | orjson | 0.075 | 0.085 | 0.130 | 61.590 | 0.74x |
| mixed.json | msgspec | 0.092 | 0.108 | 0.119 | 61.590 | 0.59x |
| mixed.json | ujson | 0.403 | 0.444 | 0.490 | 61.590 | 0.14x |
| mixed.json | json | 0.909 | 0.938 | 1.445 | 61.590 | 0.07x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 17.429 | 17.615 | 18.468 | 63.773 | 1.00x |
| users.json | orjson | 23.898 | 24.693 | 27.766 | 63.773 | 0.71x |
| users.json | msgspec | 24.070 | 24.946 | 26.516 | 63.773 | 0.71x |
| users.json | ujson | 36.123 | 37.818 | 41.757 | 63.773 | 0.47x |
| users.json | json | 39.883 | 41.365 | 46.733 | 63.773 | 0.43x |
| flat.json | strata | 1.256 | 1.283 | 1.365 | 62.383 | 1.00x |
| flat.json | orjson | 1.335 | 1.449 | 1.648 | 62.383 | 0.89x |
| flat.json | msgspec | 1.594 | 1.658 | 2.101 | 62.383 | 0.77x |
| flat.json | ujson | 2.579 | 2.738 | 3.140 | 62.383 | 0.47x |
| flat.json | json | 2.884 | 3.076 | 3.163 | 62.383 | 0.42x |
| nested.json | strata | 1.413 | 1.491 | 1.561 | 56.465 | 1.00x |
| nested.json | orjson | 1.650 | 1.720 | 1.830 | 56.465 | 0.87x |
| nested.json | msgspec | 1.762 | 1.904 | 2.005 | 56.465 | 0.78x |
| nested.json | ujson | 2.786 | 3.028 | 3.136 | 56.465 | 0.49x |
| nested.json | json | 3.546 | 3.871 | 4.043 | 56.465 | 0.39x |
| wide_arrays.json | strata | 7.012 | 7.149 | 7.571 | 65.102 | 1.00x |
| wide_arrays.json | orjson | 8.268 | 8.846 | 9.714 | 65.102 | 0.81x |
| wide_arrays.json | msgspec | 9.608 | 9.772 | 10.005 | 65.102 | 0.73x |
| wide_arrays.json | ujson | 12.349 | 12.421 | 13.327 | 65.102 | 0.58x |
| wide_arrays.json | json | 16.008 | 16.289 | 16.583 | 65.102 | 0.44x |
| mixed.json | strata | 0.398 | 0.404 | 0.438 | 61.590 | 1.00x |
| mixed.json | orjson | 0.504 | 0.526 | 0.614 | 61.590 | 0.77x |
| mixed.json | msgspec | 0.521 | 0.551 | 0.601 | 61.590 | 0.73x |
| mixed.json | ujson | 0.665 | 0.727 | 0.824 | 61.590 | 0.56x |
| mixed.json | json | 0.882 | 0.942 | 1.220 | 61.590 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 17.586 | 18.552 | 19.684 | 65.133 | 1.00x |
| users.ndjson | orjson | 24.672 | 26.024 | 28.301 | 65.133 | 0.71x |
| users.ndjson | msgspec | 25.721 | 26.646 | 28.628 | 65.133 | 0.70x |
| users.ndjson | ujson | 36.904 | 38.147 | 41.279 | 65.133 | 0.49x |
| users.ndjson | json | 46.115 | 47.330 | 51.316 | 65.133 | 0.39x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.207 | 3.300 | 3.594 | 63.785 | 1.00x |
| users.json | orjson | 3.967 | 4.051 | 4.408 | 63.785 | 0.81x |
| users.json | msgspec | 5.514 | 6.115 | 6.885 | 63.785 | 0.54x |
| users.json | ujson | 24.282 | 24.933 | 26.773 | 63.785 | 0.13x |
| users.json | json | 40.260 | 40.958 | 44.845 | 63.785 | 0.08x |
| flat.json | strata | 0.587 | 0.606 | 0.674 | 62.383 | 1.00x |
| flat.json | orjson | 0.657 | 0.716 | 0.768 | 62.383 | 0.85x |
| flat.json | msgspec | 0.777 | 0.863 | 0.933 | 62.383 | 0.70x |
| flat.json | ujson | 2.442 | 2.527 | 2.610 | 62.383 | 0.24x |
| flat.json | json | 3.776 | 3.830 | 3.956 | 62.383 | 0.16x |
| nested.json | strata | 0.476 | 0.534 | 0.572 | 56.465 | 1.00x |
| nested.json | orjson | 0.602 | 0.663 | 0.888 | 56.465 | 0.81x |
| nested.json | msgspec | 0.783 | 0.864 | 0.921 | 56.465 | 0.62x |
| nested.json | ujson | 2.408 | 2.556 | 2.645 | 56.465 | 0.21x |
| nested.json | json | 4.629 | 4.728 | 5.245 | 56.465 | 0.11x |
| wide_arrays.json | strata | 2.367 | 2.465 | 2.640 | 65.102 | 1.00x |
| wide_arrays.json | orjson | 2.947 | 3.058 | 3.619 | 65.102 | 0.81x |
| wide_arrays.json | msgspec | 3.895 | 4.082 | 4.233 | 65.102 | 0.60x |
| wide_arrays.json | ujson | 10.924 | 11.271 | 11.881 | 65.102 | 0.22x |
| wide_arrays.json | json | 32.655 | 34.651 | 37.235 | 65.102 | 0.07x |
| mixed.json | strata | 0.251 | 0.320 | 0.455 | 61.590 | 1.00x |
| mixed.json | orjson | 0.281 | 0.355 | 0.528 | 61.590 | 0.90x |
| mixed.json | msgspec | 0.307 | 0.395 | 0.454 | 61.590 | 0.81x |
| mixed.json | ujson | 0.652 | 0.727 | 0.826 | 61.590 | 0.44x |
| mixed.json | json | 1.158 | 1.232 | 1.342 | 61.590 | 0.26x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.132 | 0.180 | 0.238 | 63.848 | 1.00x |
| users.json $[*].id | jmespath | 0.900 | 1.046 | 1.503 | 63.848 | 0.17x |
| users.json $[*].id | jsonpath-ng | 4.923 | 5.373 | 6.748 | 63.848 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.780 | 0.908 | 1.073 | 61.289 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.287 | 5.670 | 6.425 | 61.289 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 32.583 | 36.535 | 39.178 | 61.289 | 0.02x |
| users.json $..total | strata | 2.983 | 3.296 | 3.776 | 61.406 | 1.00x |
| users.json $..total | jsonpath-ng | 658.767 | 682.407 | 730.373 | 61.406 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.807 | 3.859 | 4.182 | 63.918 | 1.00x |
| users.json $[*].id | orjson+jmespath | 24.480 | 26.899 | 32.554 | 63.918 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 29.998 | 32.455 | 35.379 | 63.918 | 0.12x |
| users.json $[*].orders[*].total | strata | 3.835 | 4.019 | 4.093 | 61.312 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 30.264 | 30.827 | 32.497 | 61.312 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 60.803 | 63.127 | 68.867 | 61.312 | 0.06x |
| users.json $..total | strata | 21.395 | 22.030 | 22.816 | 61.406 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 701.775 | 722.895 | 755.264 | 61.406 | 0.03x |

