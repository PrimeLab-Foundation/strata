# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: b32d39824b8fea15839872cab8834a1b0fef1294
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
| users.json | strata | 15.893 | 16.624 | 19.620 | 57.246 | 1.00x |
| users.json | orjson | 23.644 | 24.794 | 26.177 | 57.246 | 0.67x |
| users.json | msgspec | 23.489 | 24.158 | 26.108 | 57.246 | 0.69x |
| users.json | ujson | 34.633 | 36.106 | 39.083 | 57.246 | 0.46x |
| users.json | pysimdjson | 151.576 | 153.809 | 157.785 | 57.246 | 0.11x |
| users.json | json | 38.909 | 41.637 | 43.317 | 57.246 | 0.40x |
| flat.json | strata | 1.147 | 1.184 | 1.214 | 65.496 | 1.00x |
| flat.json | orjson | 1.276 | 1.309 | 1.348 | 65.496 | 0.90x |
| flat.json | msgspec | 1.496 | 1.543 | 1.616 | 65.496 | 0.77x |
| flat.json | ujson | 2.598 | 2.657 | 2.765 | 65.496 | 0.45x |
| flat.json | pysimdjson | 13.810 | 13.884 | 14.028 | 65.496 | 0.09x |
| flat.json | json | 2.993 | 3.025 | 3.092 | 65.496 | 0.39x |
| nested.json | strata | 1.337 | 1.390 | 1.635 | 64.617 | 1.00x |
| nested.json | orjson | 1.548 | 1.566 | 1.626 | 64.617 | 0.89x |
| nested.json | msgspec | 1.676 | 1.715 | 1.753 | 64.617 | 0.81x |
| nested.json | ujson | 2.791 | 2.835 | 2.872 | 64.617 | 0.49x |
| nested.json | pysimdjson | 12.615 | 12.669 | 12.842 | 64.617 | 0.11x |
| nested.json | json | 3.606 | 3.619 | 3.695 | 64.617 | 0.38x |
| wide_arrays.json | strata | 6.544 | 6.915 | 6.989 | 70.578 | 1.00x |
| wide_arrays.json | orjson | 8.313 | 8.871 | 9.210 | 70.578 | 0.78x |
| wide_arrays.json | msgspec | 9.275 | 9.915 | 10.401 | 70.578 | 0.70x |
| wide_arrays.json | ujson | 11.587 | 12.248 | 12.442 | 70.578 | 0.56x |
| wide_arrays.json | pysimdjson | 74.721 | 75.030 | 75.778 | 70.578 | 0.09x |
| wide_arrays.json | json | 15.853 | 16.046 | 16.566 | 70.578 | 0.43x |
| mixed.json | strata | 0.304 | 0.326 | 0.348 | 63.371 | 1.00x |
| mixed.json | orjson | 0.376 | 0.409 | 0.521 | 63.371 | 0.80x |
| mixed.json | msgspec | 0.403 | 0.434 | 0.479 | 63.371 | 0.75x |
| mixed.json | ujson | 0.549 | 0.591 | 0.612 | 63.371 | 0.55x |
| mixed.json | pysimdjson | 3.023 | 3.045 | 3.122 | 63.371 | 0.11x |
| mixed.json | json | 0.782 | 0.831 | 0.855 | 63.371 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.342 | 2.488 | 2.646 | 52.543 | 1.00x |
| users.json | orjson | 3.113 | 3.162 | 3.253 | 52.543 | 0.79x |
| users.json | msgspec | 4.776 | 4.930 | 5.148 | 52.543 | 0.50x |
| users.json | ujson | 22.947 | 23.211 | 23.591 | 52.543 | 0.11x |
| users.json | json | 38.804 | 38.934 | 39.897 | 52.543 | 0.06x |
| flat.json | strata | 0.271 | 0.304 | 0.319 | 64.012 | 1.00x |
| flat.json | orjson | 0.329 | 0.370 | 0.382 | 64.012 | 0.82x |
| flat.json | msgspec | 0.445 | 0.497 | 0.527 | 64.012 | 0.61x |
| flat.json | ujson | 2.038 | 2.095 | 2.161 | 64.012 | 0.15x |
| flat.json | json | 3.255 | 3.426 | 3.464 | 64.012 | 0.09x |
| nested.json | strata | 0.203 | 0.218 | 0.232 | 64.746 | 1.00x |
| nested.json | orjson | 0.318 | 0.326 | 0.374 | 64.746 | 0.67x |
| nested.json | msgspec | 0.511 | 0.513 | 0.631 | 64.746 | 0.42x |
| nested.json | ujson | 2.086 | 2.198 | 2.253 | 64.746 | 0.10x |
| nested.json | json | 4.236 | 4.337 | 4.752 | 64.746 | 0.05x |
| wide_arrays.json | strata | 1.948 | 2.041 | 2.436 | 64.305 | 1.00x |
| wide_arrays.json | orjson | 2.299 | 2.531 | 2.668 | 64.305 | 0.81x |
| wide_arrays.json | msgspec | 3.226 | 3.510 | 3.738 | 64.305 | 0.58x |
| wide_arrays.json | ujson | 9.766 | 10.790 | 17.876 | 64.305 | 0.19x |
| wide_arrays.json | json | 32.416 | 32.591 | 33.119 | 64.305 | 0.06x |
| mixed.json | strata | 0.058 | 0.061 | 0.070 | 60.172 | 1.00x |
| mixed.json | orjson | 0.071 | 0.075 | 0.079 | 60.172 | 0.81x |
| mixed.json | msgspec | 0.101 | 0.104 | 0.119 | 60.172 | 0.59x |
| mixed.json | ujson | 0.429 | 0.431 | 0.433 | 60.172 | 0.14x |
| mixed.json | json | 0.897 | 0.907 | 0.998 | 60.172 | 0.07x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 16.391 | 16.719 | 17.633 | 62.551 | 1.00x |
| users.json | orjson | 23.650 | 24.133 | 25.235 | 62.551 | 0.69x |
| users.json | msgspec | 23.263 | 24.148 | 24.605 | 62.551 | 0.69x |
| users.json | ujson | 35.517 | 36.376 | 37.305 | 62.551 | 0.46x |
| users.json | json | 39.671 | 40.325 | 41.788 | 62.551 | 0.41x |
| flat.json | strata | 1.145 | 1.257 | 1.483 | 64.656 | 1.00x |
| flat.json | orjson | 1.302 | 1.420 | 1.449 | 64.656 | 0.89x |
| flat.json | msgspec | 1.515 | 1.678 | 1.782 | 64.656 | 0.75x |
| flat.json | ujson | 2.596 | 2.800 | 3.028 | 64.656 | 0.45x |
| flat.json | json | 3.029 | 3.125 | 3.191 | 64.656 | 0.40x |
| nested.json | strata | 1.442 | 1.477 | 1.496 | 64.746 | 1.00x |
| nested.json | orjson | 1.668 | 1.705 | 1.773 | 64.746 | 0.87x |
| nested.json | msgspec | 1.817 | 1.872 | 1.937 | 64.746 | 0.79x |
| nested.json | ujson | 2.919 | 2.995 | 3.078 | 64.746 | 0.49x |
| nested.json | json | 3.696 | 3.766 | 3.876 | 64.746 | 0.39x |
| wide_arrays.json | strata | 6.802 | 6.878 | 7.031 | 65.535 | 1.00x |
| wide_arrays.json | orjson | 8.717 | 8.911 | 9.366 | 65.535 | 0.77x |
| wide_arrays.json | msgspec | 9.701 | 9.770 | 9.949 | 65.535 | 0.70x |
| wide_arrays.json | ujson | 12.282 | 12.331 | 12.475 | 65.535 | 0.56x |
| wide_arrays.json | json | 15.908 | 16.138 | 16.749 | 65.535 | 0.43x |
| mixed.json | strata | 0.361 | 0.404 | 0.474 | 60.172 | 1.00x |
| mixed.json | orjson | 0.463 | 0.519 | 0.583 | 60.172 | 0.78x |
| mixed.json | msgspec | 0.490 | 0.545 | 0.590 | 60.172 | 0.74x |
| mixed.json | ujson | 0.660 | 0.714 | 0.742 | 60.172 | 0.57x |
| mixed.json | json | 0.859 | 0.928 | 0.954 | 60.172 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 17.108 | 17.366 | 18.336 | 65.078 | 1.00x |
| users.ndjson | orjson | 24.633 | 25.729 | 26.361 | 65.078 | 0.67x |
| users.ndjson | msgspec | 25.434 | 26.271 | 27.186 | 65.078 | 0.66x |
| users.ndjson | ujson | 36.697 | 37.963 | 39.369 | 65.078 | 0.46x |
| users.ndjson | json | 46.120 | 47.098 | 48.393 | 65.078 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.765 | 3.287 | 3.431 | 62.613 | 1.00x |
| users.json | orjson | 4.039 | 4.384 | 4.525 | 62.613 | 0.75x |
| users.json | msgspec | 5.567 | 6.004 | 6.767 | 62.613 | 0.55x |
| users.json | ujson | 24.508 | 24.894 | 25.751 | 62.613 | 0.13x |
| users.json | json | 40.043 | 40.736 | 41.805 | 62.613 | 0.08x |
| flat.json | strata | 0.625 | 0.658 | 0.761 | 64.656 | 1.00x |
| flat.json | orjson | 0.701 | 0.745 | 0.878 | 64.656 | 0.88x |
| flat.json | msgspec | 0.798 | 0.873 | 0.991 | 64.656 | 0.75x |
| flat.json | ujson | 2.467 | 2.501 | 2.681 | 64.656 | 0.26x |
| flat.json | json | 3.785 | 3.877 | 4.199 | 64.656 | 0.17x |
| nested.json | strata | 0.517 | 0.540 | 0.602 | 64.746 | 1.00x |
| nested.json | orjson | 0.645 | 0.672 | 0.850 | 64.746 | 0.80x |
| nested.json | msgspec | 0.837 | 0.845 | 0.875 | 64.746 | 0.64x |
| nested.json | ujson | 2.519 | 2.561 | 2.602 | 64.746 | 0.21x |
| nested.json | json | 4.691 | 4.726 | 5.038 | 64.746 | 0.11x |
| wide_arrays.json | strata | 2.411 | 2.568 | 2.743 | 64.305 | 1.00x |
| wide_arrays.json | orjson | 3.000 | 3.182 | 3.411 | 64.305 | 0.81x |
| wide_arrays.json | msgspec | 4.168 | 4.280 | 5.541 | 64.305 | 0.60x |
| wide_arrays.json | ujson | 11.059 | 11.475 | 12.046 | 64.305 | 0.22x |
| wide_arrays.json | json | 32.662 | 33.151 | 33.785 | 64.305 | 0.08x |
| mixed.json | strata | 0.318 | 0.333 | 0.409 | 60.172 | 1.00x |
| mixed.json | orjson | 0.351 | 0.375 | 0.410 | 60.172 | 0.89x |
| mixed.json | msgspec | 0.373 | 0.408 | 0.575 | 60.172 | 0.82x |
| mixed.json | ujson | 0.733 | 0.759 | 0.826 | 60.172 | 0.44x |
| mixed.json | json | 1.187 | 1.239 | 1.303 | 60.172 | 0.27x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.128 | 0.135 | 0.149 | 62.680 | 1.00x |
| users.json $[*].id | jmespath | 0.844 | 0.899 | 1.050 | 62.680 | 0.15x |
| users.json $[*].id | jsonpath-ng | 4.861 | 4.933 | 5.689 | 62.680 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.862 | 1.096 | 1.515 | 59.895 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.563 | 5.971 | 6.785 | 59.895 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 32.864 | 35.236 | 37.295 | 59.895 | 0.03x |
| users.json $..total | strata | 3.045 | 3.191 | 3.444 | 59.895 | 1.00x |
| users.json $..total | jsonpath-ng | 654.289 | 661.700 | 678.174 | 59.895 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.605 | 3.630 | 4.139 | 62.734 | 1.00x |
| users.json $[*].id | orjson+jmespath | 24.625 | 24.856 | 25.402 | 62.734 | 0.15x |
| users.json $[*].id | orjson+jsonpath-ng | 28.078 | 29.192 | 31.128 | 62.734 | 0.12x |
| users.json $[*].orders[*].total | strata | 4.026 | 4.050 | 4.184 | 59.895 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 28.812 | 30.884 | 33.770 | 59.895 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 61.815 | 63.790 | 66.535 | 59.895 | 0.06x |
| users.json $..total | strata | 20.248 | 21.017 | 21.579 | 59.895 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 679.066 | 685.102 | 692.018 | 59.895 | 0.03x |

