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
| users.json | strata | 17.949 | 18.935 | 21.806 | 57.082 | 1.00x |
| users.json | orjson | 25.183 | 30.024 | 35.589 | 57.082 | 0.63x |
| users.json | msgspec | 25.523 | 28.502 | 31.770 | 57.082 | 0.66x |
| users.json | ujson | 38.475 | 40.926 | 46.308 | 57.082 | 0.46x |
| users.json | pysimdjson | 168.807 | 175.497 | 180.922 | 57.082 | 0.11x |
| users.json | json | 42.973 | 47.193 | 55.149 | 57.082 | 0.40x |
| flat.json | strata | 1.251 | 1.299 | 1.858 | 67.973 | 1.00x |
| flat.json | orjson | 1.400 | 1.476 | 2.018 | 67.973 | 0.88x |
| flat.json | msgspec | 1.582 | 1.702 | 2.230 | 67.973 | 0.76x |
| flat.json | ujson | 2.763 | 2.873 | 3.101 | 67.973 | 0.45x |
| flat.json | pysimdjson | 14.888 | 15.585 | 18.553 | 67.973 | 0.08x |
| flat.json | json | 3.227 | 3.393 | 4.487 | 67.973 | 0.38x |
| nested.json | strata | 1.557 | 1.588 | 1.994 | 66.453 | 1.00x |
| nested.json | orjson | 1.702 | 1.889 | 2.099 | 66.453 | 0.84x |
| nested.json | msgspec | 1.928 | 2.081 | 2.640 | 66.453 | 0.76x |
| nested.json | ujson | 3.176 | 3.394 | 4.280 | 66.453 | 0.47x |
| nested.json | pysimdjson | 14.013 | 14.793 | 16.189 | 66.453 | 0.11x |
| nested.json | json | 3.990 | 4.391 | 4.825 | 66.453 | 0.36x |
| wide_arrays.json | strata | 7.897 | 8.591 | 10.441 | 72.414 | 1.00x |
| wide_arrays.json | orjson | 9.892 | 10.945 | 12.509 | 72.414 | 0.78x |
| wide_arrays.json | msgspec | 10.665 | 11.865 | 13.694 | 72.414 | 0.72x |
| wide_arrays.json | ujson | 13.129 | 14.327 | 16.888 | 72.414 | 0.60x |
| wide_arrays.json | pysimdjson | 81.554 | 89.863 | 97.179 | 72.414 | 0.10x |
| wide_arrays.json | json | 17.362 | 18.147 | 22.904 | 72.414 | 0.47x |
| mixed.json | strata | 0.380 | 0.402 | 0.459 | 65.211 | 1.00x |
| mixed.json | orjson | 0.465 | 0.491 | 0.564 | 65.211 | 0.82x |
| mixed.json | msgspec | 0.497 | 0.519 | 0.728 | 65.211 | 0.78x |
| mixed.json | ujson | 0.670 | 0.706 | 0.810 | 65.211 | 0.57x |
| mixed.json | pysimdjson | 3.325 | 3.549 | 3.877 | 65.211 | 0.11x |
| mixed.json | json | 0.942 | 0.979 | 1.152 | 65.211 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.451 | 2.806 | 3.899 | 52.387 | 1.00x |
| users.json | orjson | 3.485 | 4.103 | 4.905 | 52.387 | 0.68x |
| users.json | msgspec | 5.039 | 6.075 | 6.841 | 52.387 | 0.46x |
| users.json | ujson | 24.909 | 26.726 | 29.729 | 52.387 | 0.10x |
| users.json | json | 41.971 | 45.281 | 51.790 | 52.387 | 0.06x |
| flat.json | strata | 0.343 | 0.427 | 0.499 | 66.492 | 1.00x |
| flat.json | orjson | 0.427 | 0.533 | 0.672 | 66.492 | 0.80x |
| flat.json | msgspec | 0.566 | 0.641 | 0.962 | 66.492 | 0.67x |
| flat.json | ujson | 2.268 | 2.739 | 4.602 | 66.492 | 0.16x |
| flat.json | json | 3.797 | 4.725 | 6.818 | 66.492 | 0.09x |
| nested.json | strata | 0.239 | 0.263 | 0.298 | 66.582 | 1.00x |
| nested.json | orjson | 0.353 | 0.382 | 0.487 | 66.582 | 0.69x |
| nested.json | msgspec | 0.558 | 0.598 | 0.722 | 66.582 | 0.44x |
| nested.json | ujson | 2.351 | 2.385 | 2.909 | 66.582 | 0.11x |
| nested.json | json | 4.646 | 4.834 | 6.343 | 66.582 | 0.05x |
| wide_arrays.json | strata | 1.975 | 2.223 | 2.740 | 66.141 | 1.00x |
| wide_arrays.json | orjson | 2.601 | 2.868 | 3.486 | 66.141 | 0.78x |
| wide_arrays.json | msgspec | 3.611 | 3.922 | 4.386 | 66.141 | 0.57x |
| wide_arrays.json | ujson | 11.183 | 12.707 | 13.490 | 66.141 | 0.17x |
| wide_arrays.json | json | 36.717 | 41.686 | 46.476 | 66.141 | 0.05x |
| mixed.json | strata | 0.066 | 0.070 | 0.126 | 62.012 | 1.00x |
| mixed.json | orjson | 0.079 | 0.095 | 0.147 | 62.012 | 0.73x |
| mixed.json | msgspec | 0.110 | 0.123 | 0.150 | 62.012 | 0.57x |
| mixed.json | ujson | 0.457 | 0.479 | 0.593 | 62.012 | 0.15x |
| mixed.json | json | 0.962 | 1.003 | 1.223 | 62.012 | 0.07x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 18.347 | 18.987 | 24.120 | 63.176 | 1.00x |
| users.json | orjson | 25.126 | 27.200 | 33.258 | 63.176 | 0.70x |
| users.json | msgspec | 24.891 | 27.152 | 31.259 | 63.176 | 0.70x |
| users.json | ujson | 37.958 | 41.514 | 45.179 | 63.176 | 0.46x |
| users.json | json | 43.385 | 46.655 | 51.964 | 63.176 | 0.41x |
| flat.json | strata | 1.440 | 1.675 | 2.216 | 66.492 | 1.00x |
| flat.json | orjson | 1.583 | 1.822 | 2.404 | 66.492 | 0.92x |
| flat.json | msgspec | 1.960 | 2.343 | 2.725 | 66.492 | 0.71x |
| flat.json | ujson | 3.051 | 3.887 | 4.515 | 66.492 | 0.43x |
| flat.json | json | 3.507 | 4.538 | 5.483 | 66.492 | 0.37x |
| nested.json | strata | 1.616 | 1.741 | 2.296 | 66.582 | 1.00x |
| nested.json | orjson | 1.844 | 1.956 | 2.389 | 66.582 | 0.89x |
| nested.json | msgspec | 2.042 | 2.147 | 2.657 | 66.582 | 0.81x |
| nested.json | ujson | 3.275 | 3.400 | 4.159 | 66.582 | 0.51x |
| nested.json | json | 4.135 | 4.323 | 5.672 | 66.582 | 0.40x |
| wide_arrays.json | strata | 7.974 | 8.431 | 10.760 | 67.371 | 1.00x |
| wide_arrays.json | orjson | 9.930 | 10.552 | 12.261 | 67.371 | 0.80x |
| wide_arrays.json | msgspec | 10.878 | 11.147 | 14.471 | 67.371 | 0.76x |
| wide_arrays.json | ujson | 13.555 | 14.092 | 16.805 | 67.371 | 0.60x |
| wide_arrays.json | json | 17.833 | 18.554 | 20.965 | 67.371 | 0.45x |
| mixed.json | strata | 0.454 | 0.483 | 0.537 | 62.012 | 1.00x |
| mixed.json | orjson | 0.583 | 0.609 | 0.831 | 62.012 | 0.79x |
| mixed.json | msgspec | 0.598 | 0.631 | 0.848 | 62.012 | 0.77x |
| mixed.json | ujson | 0.775 | 0.874 | 1.199 | 62.012 | 0.55x |
| mixed.json | json | 1.005 | 1.115 | 1.665 | 62.012 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 18.720 | 19.307 | 21.558 | 67.551 | 1.00x |
| users.ndjson | orjson | 26.851 | 28.118 | 32.505 | 67.551 | 0.69x |
| users.ndjson | msgspec | 27.524 | 28.572 | 31.855 | 67.551 | 0.68x |
| users.ndjson | ujson | 39.568 | 43.422 | 47.872 | 67.551 | 0.44x |
| users.ndjson | json | 51.382 | 54.430 | 61.137 | 67.551 | 0.35x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.372 | 3.518 | 5.347 | 63.203 | 1.00x |
| users.json | orjson | 4.320 | 4.958 | 6.103 | 63.203 | 0.71x |
| users.json | msgspec | 6.349 | 6.988 | 8.914 | 63.203 | 0.50x |
| users.json | ujson | 26.436 | 32.086 | 36.301 | 63.203 | 0.11x |
| users.json | json | 43.393 | 51.099 | 56.447 | 63.203 | 0.07x |
| flat.json | strata | 0.630 | 0.782 | 0.981 | 66.492 | 1.00x |
| flat.json | orjson | 0.797 | 0.943 | 1.218 | 66.492 | 0.83x |
| flat.json | msgspec | 0.921 | 1.096 | 1.396 | 66.492 | 0.71x |
| flat.json | ujson | 2.684 | 2.986 | 3.945 | 66.492 | 0.26x |
| flat.json | json | 4.119 | 4.354 | 6.441 | 66.492 | 0.18x |
| nested.json | strata | 0.588 | 0.636 | 0.736 | 66.582 | 1.00x |
| nested.json | orjson | 0.714 | 0.797 | 1.097 | 66.582 | 0.80x |
| nested.json | msgspec | 0.901 | 0.981 | 1.162 | 66.582 | 0.65x |
| nested.json | ujson | 2.724 | 2.860 | 3.496 | 66.582 | 0.22x |
| nested.json | json | 5.042 | 5.202 | 5.438 | 66.582 | 0.12x |
| wide_arrays.json | strata | 2.794 | 3.015 | 4.092 | 66.141 | 1.00x |
| wide_arrays.json | orjson | 3.485 | 3.718 | 4.027 | 66.141 | 0.81x |
| wide_arrays.json | msgspec | 4.546 | 4.819 | 5.618 | 66.141 | 0.63x |
| wide_arrays.json | ujson | 12.501 | 12.857 | 15.074 | 66.141 | 0.23x |
| wide_arrays.json | json | 36.041 | 36.598 | 40.868 | 66.141 | 0.08x |
| mixed.json | strata | 0.364 | 0.427 | 1.210 | 62.012 | 1.00x |
| mixed.json | orjson | 0.377 | 0.460 | 0.863 | 62.012 | 0.93x |
| mixed.json | msgspec | 0.458 | 0.505 | 1.840 | 62.012 | 0.85x |
| mixed.json | ujson | 0.830 | 0.942 | 1.522 | 62.012 | 0.45x |
| mixed.json | json | 1.315 | 1.440 | 2.810 | 62.012 | 0.30x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.143 | 0.190 | 0.273 | 63.270 | 1.00x |
| users.json $[*].id | jmespath | 0.961 | 1.025 | 1.456 | 63.270 | 0.19x |
| users.json $[*].id | jsonpath-ng | 5.245 | 5.690 | 6.804 | 63.270 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.846 | 1.127 | 1.687 | 60.473 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.888 | 6.700 | 7.821 | 60.473 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 35.751 | 37.895 | 45.469 | 60.473 | 0.03x |
| users.json $..total | strata | 3.240 | 4.036 | 5.414 | 60.523 | 1.00x |
| users.json $..total | jsonpath-ng | 734.856 | 783.609 | 894.599 | 60.523 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.857 | 4.064 | 4.726 | 63.332 | 1.00x |
| users.json $[*].id | orjson+jmespath | 26.994 | 29.617 | 30.887 | 63.332 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 31.924 | 33.758 | 37.137 | 63.332 | 0.12x |
| users.json $[*].orders[*].total | strata | 4.293 | 4.516 | 7.186 | 60.477 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 32.083 | 36.752 | 43.974 | 60.477 | 0.12x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 70.440 | 87.683 | 101.905 | 60.477 | 0.05x |
| users.json $..total | strata | 23.105 | 27.995 | 32.124 | 60.547 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 781.977 | 832.400 | 1101.199 | 60.547 | 0.03x |

