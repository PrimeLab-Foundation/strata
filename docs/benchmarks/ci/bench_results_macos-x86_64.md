# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 943460734d2b79bd16c949d8d97f4d22cc202e84
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
| users.json | strata | 16.594 | 17.416 | 19.206 | 57.082 | 1.00x |
| users.json | orjson | 23.374 | 24.335 | 26.539 | 57.082 | 0.72x |
| users.json | msgspec | 23.308 | 24.400 | 27.361 | 57.082 | 0.71x |
| users.json | ujson | 34.610 | 36.727 | 41.104 | 57.082 | 0.47x |
| users.json | pysimdjson | 151.485 | 152.306 | 168.036 | 57.082 | 0.11x |
| users.json | json | 39.654 | 39.966 | 43.709 | 57.082 | 0.44x |
| flat.json | strata | 1.079 | 1.175 | 1.233 | 66.160 | 1.00x |
| flat.json | orjson | 1.197 | 1.292 | 1.312 | 66.160 | 0.91x |
| flat.json | msgspec | 1.431 | 1.475 | 1.511 | 66.160 | 0.80x |
| flat.json | ujson | 2.501 | 2.582 | 2.807 | 66.160 | 0.45x |
| flat.json | pysimdjson | 13.742 | 13.770 | 14.075 | 66.160 | 0.09x |
| flat.json | json | 2.817 | 2.987 | 3.080 | 66.160 | 0.39x |
| nested.json | strata | 1.330 | 1.350 | 1.421 | 64.641 | 1.00x |
| nested.json | orjson | 1.517 | 1.549 | 1.632 | 64.641 | 0.87x |
| nested.json | msgspec | 1.692 | 1.703 | 1.735 | 64.641 | 0.79x |
| nested.json | ujson | 2.802 | 2.822 | 2.854 | 64.641 | 0.48x |
| nested.json | pysimdjson | 12.522 | 12.582 | 12.642 | 64.641 | 0.11x |
| nested.json | json | 3.596 | 3.619 | 4.005 | 64.641 | 0.37x |
| wide_arrays.json | strata | 7.126 | 7.483 | 7.707 | 70.609 | 1.00x |
| wide_arrays.json | orjson | 8.655 | 9.431 | 10.473 | 70.609 | 0.79x |
| wide_arrays.json | msgspec | 9.477 | 10.324 | 10.956 | 70.609 | 0.72x |
| wide_arrays.json | ujson | 12.132 | 12.713 | 13.679 | 70.609 | 0.59x |
| wide_arrays.json | pysimdjson | 74.918 | 77.259 | 82.233 | 70.609 | 0.10x |
| wide_arrays.json | json | 16.452 | 16.885 | 18.100 | 70.609 | 0.44x |
| mixed.json | strata | 0.317 | 0.326 | 0.339 | 64.730 | 1.00x |
| mixed.json | orjson | 0.394 | 0.405 | 0.421 | 64.730 | 0.81x |
| mixed.json | msgspec | 0.419 | 0.427 | 0.469 | 64.730 | 0.76x |
| mixed.json | ujson | 0.570 | 0.585 | 0.604 | 64.730 | 0.56x |
| mixed.json | pysimdjson | 2.990 | 3.020 | 3.294 | 64.730 | 0.11x |
| mixed.json | json | 0.808 | 0.826 | 0.851 | 64.730 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.220 | 2.291 | 2.548 | 52.375 | 1.00x |
| users.json | orjson | 2.991 | 3.199 | 3.564 | 52.375 | 0.72x |
| users.json | msgspec | 4.800 | 4.882 | 5.115 | 52.375 | 0.47x |
| users.json | ujson | 23.120 | 24.237 | 24.788 | 52.375 | 0.09x |
| users.json | json | 38.865 | 41.978 | 43.177 | 52.375 | 0.05x |
| flat.json | strata | 0.300 | 0.308 | 0.321 | 64.676 | 1.00x |
| flat.json | orjson | 0.360 | 0.375 | 0.412 | 64.676 | 0.82x |
| flat.json | msgspec | 0.485 | 0.498 | 0.539 | 64.676 | 0.62x |
| flat.json | ujson | 2.073 | 2.083 | 2.104 | 64.676 | 0.15x |
| flat.json | json | 3.402 | 3.422 | 3.460 | 64.676 | 0.09x |
| nested.json | strata | 0.201 | 0.210 | 0.224 | 64.773 | 1.00x |
| nested.json | orjson | 0.310 | 0.324 | 0.329 | 64.773 | 0.65x |
| nested.json | msgspec | 0.496 | 0.515 | 0.535 | 64.773 | 0.41x |
| nested.json | ujson | 2.165 | 2.206 | 2.319 | 64.773 | 0.09x |
| nested.json | json | 4.299 | 4.333 | 4.383 | 64.773 | 0.05x |
| wide_arrays.json | strata | 1.680 | 1.784 | 1.896 | 63.961 | 1.00x |
| wide_arrays.json | orjson | 2.192 | 2.270 | 2.469 | 63.961 | 0.79x |
| wide_arrays.json | msgspec | 3.212 | 3.278 | 3.525 | 63.961 | 0.54x |
| wide_arrays.json | ujson | 9.829 | 9.899 | 10.365 | 63.961 | 0.18x |
| wide_arrays.json | json | 31.443 | 32.218 | 34.394 | 63.961 | 0.06x |
| mixed.json | strata | 0.060 | 0.063 | 0.067 | 60.477 | 1.00x |
| mixed.json | orjson | 0.074 | 0.077 | 0.085 | 60.477 | 0.81x |
| mixed.json | msgspec | 0.103 | 0.105 | 0.114 | 60.477 | 0.60x |
| mixed.json | ujson | 0.434 | 0.443 | 0.464 | 60.477 | 0.14x |
| mixed.json | json | 0.898 | 0.936 | 0.988 | 60.477 | 0.07x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 17.180 | 17.695 | 18.862 | 63.133 | 1.00x |
| users.json | orjson | 23.938 | 25.617 | 28.359 | 63.133 | 0.69x |
| users.json | msgspec | 25.158 | 26.466 | 30.804 | 63.133 | 0.67x |
| users.json | ujson | 35.382 | 38.354 | 42.080 | 63.133 | 0.46x |
| users.json | json | 39.892 | 43.603 | 44.630 | 63.133 | 0.41x |
| flat.json | strata | 1.223 | 1.261 | 1.309 | 64.676 | 1.00x |
| flat.json | orjson | 1.386 | 1.418 | 1.518 | 64.676 | 0.89x |
| flat.json | msgspec | 1.561 | 1.624 | 1.716 | 64.676 | 0.78x |
| flat.json | ujson | 2.632 | 2.728 | 2.837 | 64.676 | 0.46x |
| flat.json | json | 2.953 | 3.104 | 3.188 | 64.676 | 0.41x |
| nested.json | strata | 1.451 | 1.475 | 1.534 | 64.773 | 1.00x |
| nested.json | orjson | 1.661 | 1.716 | 2.044 | 64.773 | 0.86x |
| nested.json | msgspec | 1.810 | 1.880 | 1.979 | 64.773 | 0.78x |
| nested.json | ujson | 2.907 | 3.010 | 3.331 | 64.773 | 0.49x |
| nested.json | json | 3.709 | 3.801 | 5.961 | 64.773 | 0.39x |
| wide_arrays.json | strata | 6.882 | 6.916 | 7.330 | 66.129 | 1.00x |
| wide_arrays.json | orjson | 8.495 | 8.583 | 8.906 | 66.129 | 0.81x |
| wide_arrays.json | msgspec | 9.538 | 9.574 | 10.021 | 66.129 | 0.72x |
| wide_arrays.json | ujson | 12.081 | 12.221 | 13.043 | 66.129 | 0.57x |
| wide_arrays.json | json | 15.514 | 16.353 | 17.014 | 66.129 | 0.42x |
| mixed.json | strata | 0.401 | 0.412 | 0.447 | 60.477 | 1.00x |
| mixed.json | orjson | 0.516 | 0.535 | 0.544 | 60.477 | 0.77x |
| mixed.json | msgspec | 0.540 | 0.561 | 0.599 | 60.477 | 0.73x |
| mixed.json | ujson | 0.715 | 0.731 | 0.748 | 60.477 | 0.56x |
| mixed.json | json | 0.912 | 0.967 | 1.011 | 60.477 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 17.296 | 18.170 | 18.995 | 65.734 | 1.00x |
| users.ndjson | orjson | 24.920 | 26.206 | 26.710 | 65.734 | 0.69x |
| users.ndjson | msgspec | 25.241 | 26.389 | 26.874 | 65.734 | 0.69x |
| users.ndjson | ujson | 36.695 | 37.879 | 39.390 | 65.734 | 0.48x |
| users.ndjson | json | 45.520 | 48.126 | 49.561 | 65.734 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.046 | 3.182 | 3.381 | 63.184 | 1.00x |
| users.json | orjson | 4.130 | 4.315 | 4.755 | 63.184 | 0.74x |
| users.json | msgspec | 5.644 | 6.107 | 6.472 | 63.184 | 0.52x |
| users.json | ujson | 24.254 | 24.928 | 26.858 | 63.184 | 0.13x |
| users.json | json | 40.256 | 40.999 | 45.140 | 63.184 | 0.08x |
| flat.json | strata | 0.583 | 0.637 | 0.679 | 64.676 | 1.00x |
| flat.json | orjson | 0.689 | 0.744 | 0.794 | 64.676 | 0.86x |
| flat.json | msgspec | 0.840 | 0.870 | 1.909 | 64.676 | 0.73x |
| flat.json | ujson | 2.445 | 2.494 | 2.557 | 64.676 | 0.26x |
| flat.json | json | 3.763 | 3.837 | 4.094 | 64.676 | 0.17x |
| nested.json | strata | 0.487 | 0.523 | 0.682 | 64.773 | 1.00x |
| nested.json | orjson | 0.642 | 0.665 | 1.003 | 64.773 | 0.79x |
| nested.json | msgspec | 0.824 | 0.902 | 0.925 | 64.773 | 0.58x |
| nested.json | ujson | 2.540 | 2.588 | 2.687 | 64.773 | 0.20x |
| nested.json | json | 4.693 | 4.753 | 5.163 | 64.773 | 0.11x |
| wide_arrays.json | strata | 2.337 | 2.386 | 2.576 | 66.129 | 1.00x |
| wide_arrays.json | orjson | 2.912 | 3.011 | 3.131 | 66.129 | 0.79x |
| wide_arrays.json | msgspec | 3.835 | 4.074 | 4.208 | 66.129 | 0.59x |
| wide_arrays.json | ujson | 10.503 | 10.791 | 11.463 | 66.129 | 0.22x |
| wide_arrays.json | json | 32.535 | 33.523 | 36.891 | 66.129 | 0.07x |
| mixed.json | strata | 0.298 | 0.342 | 0.429 | 60.477 | 1.00x |
| mixed.json | orjson | 0.342 | 0.369 | 0.432 | 60.477 | 0.93x |
| mixed.json | msgspec | 0.372 | 0.408 | 0.427 | 60.477 | 0.84x |
| mixed.json | ujson | 0.737 | 0.756 | 0.868 | 60.477 | 0.45x |
| mixed.json | json | 1.183 | 1.211 | 1.315 | 60.477 | 0.28x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.132 | 0.137 | 0.151 | 63.258 | 1.00x |
| users.json $[*].id | jmespath | 0.835 | 0.895 | 0.943 | 63.258 | 0.15x |
| users.json $[*].id | jsonpath-ng | 4.748 | 4.886 | 4.948 | 63.258 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.766 | 0.871 | 0.913 | 60.449 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.259 | 5.573 | 5.726 | 60.449 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 32.493 | 33.776 | 35.974 | 60.449 | 0.03x |
| users.json $..total | strata | 2.993 | 3.114 | 3.450 | 60.543 | 1.00x |
| users.json $..total | jsonpath-ng | 648.525 | 666.388 | 700.214 | 60.543 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.586 | 3.615 | 3.670 | 63.316 | 1.00x |
| users.json $[*].id | orjson+jmespath | 24.091 | 25.555 | 26.588 | 63.316 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 28.516 | 30.227 | 31.395 | 63.316 | 0.12x |
| users.json $[*].orders[*].total | strata | 3.937 | 4.046 | 4.400 | 60.484 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 28.899 | 30.340 | 32.016 | 60.484 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 61.192 | 64.971 | 67.918 | 60.484 | 0.06x |
| users.json $..total | strata | 20.690 | 21.174 | 22.129 | 60.566 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 675.734 | 690.071 | 747.473 | 60.566 | 0.03x |

