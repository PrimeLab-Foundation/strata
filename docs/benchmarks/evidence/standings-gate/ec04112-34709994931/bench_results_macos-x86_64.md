# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: ec0411225b6d58a1df905844c946a766d3c39f0a
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
| users.json | strata | 19.845 | 20.609 | 24.604 | 57.008 | 1.00x |
| users.json | orjson | 28.040 | 29.978 | 33.630 | 57.008 | 0.69x |
| users.json | msgspec | 27.899 | 30.068 | 32.575 | 57.008 | 0.69x |
| users.json | ujson | 41.433 | 44.271 | 48.326 | 57.008 | 0.47x |
| users.json | pysimdjson | 183.565 | 189.804 | 197.052 | 57.008 | 0.11x |
| users.json | json | 46.660 | 49.490 | 52.510 | 57.008 | 0.42x |
| flat.json | strata | 1.297 | 1.350 | 1.377 | 66.152 | 1.00x |
| flat.json | orjson | 1.477 | 1.495 | 1.614 | 66.152 | 0.90x |
| flat.json | msgspec | 1.686 | 1.708 | 1.850 | 66.152 | 0.79x |
| flat.json | ujson | 2.947 | 2.994 | 3.359 | 66.152 | 0.45x |
| flat.json | pysimdjson | 15.913 | 16.039 | 16.251 | 66.152 | 0.08x |
| flat.json | json | 3.385 | 3.426 | 3.773 | 66.152 | 0.39x |
| nested.json | strata | 1.544 | 1.576 | 1.629 | 64.633 | 1.00x |
| nested.json | orjson | 1.785 | 1.808 | 2.171 | 64.633 | 0.87x |
| nested.json | msgspec | 1.967 | 2.012 | 2.072 | 64.633 | 0.78x |
| nested.json | ujson | 3.261 | 3.318 | 3.473 | 64.633 | 0.48x |
| nested.json | pysimdjson | 14.615 | 14.705 | 14.958 | 64.633 | 0.11x |
| nested.json | json | 4.177 | 4.252 | 4.632 | 64.633 | 0.37x |
| wide_arrays.json | strata | 8.252 | 8.471 | 10.048 | 70.598 | 1.00x |
| wide_arrays.json | orjson | 10.554 | 10.940 | 12.942 | 70.598 | 0.77x |
| wide_arrays.json | msgspec | 11.646 | 12.300 | 13.852 | 70.598 | 0.69x |
| wide_arrays.json | ujson | 14.718 | 15.097 | 15.559 | 70.598 | 0.56x |
| wide_arrays.json | pysimdjson | 91.502 | 92.163 | 93.768 | 70.598 | 0.09x |
| wide_arrays.json | json | 19.152 | 19.598 | 23.394 | 70.598 | 0.43x |
| mixed.json | strata | 0.365 | 0.375 | 0.395 | 64.715 | 1.00x |
| mixed.json | orjson | 0.460 | 0.470 | 0.606 | 64.715 | 0.80x |
| mixed.json | msgspec | 0.493 | 0.499 | 0.538 | 64.715 | 0.75x |
| mixed.json | ujson | 0.670 | 0.686 | 0.753 | 64.715 | 0.55x |
| mixed.json | pysimdjson | 3.485 | 3.526 | 3.574 | 64.715 | 0.11x |
| mixed.json | json | 0.949 | 0.973 | 1.005 | 64.715 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.710 | 2.875 | 3.052 | 52.316 | 1.00x |
| users.json | orjson | 3.652 | 3.778 | 3.987 | 52.316 | 0.76x |
| users.json | msgspec | 5.847 | 6.005 | 6.630 | 52.316 | 0.48x |
| users.json | ujson | 27.337 | 28.262 | 28.845 | 52.316 | 0.10x |
| users.json | json | 46.300 | 47.990 | 48.993 | 52.316 | 0.06x |
| flat.json | strata | 0.333 | 0.348 | 0.401 | 64.672 | 1.00x |
| flat.json | orjson | 0.409 | 0.429 | 0.726 | 64.672 | 0.81x |
| flat.json | msgspec | 0.545 | 0.575 | 0.611 | 64.672 | 0.61x |
| flat.json | ujson | 2.383 | 2.436 | 2.457 | 64.672 | 0.14x |
| flat.json | json | 3.956 | 4.014 | 4.238 | 64.672 | 0.09x |
| nested.json | strata | 0.258 | 0.267 | 0.281 | 64.766 | 1.00x |
| nested.json | orjson | 0.383 | 0.388 | 0.465 | 64.766 | 0.69x |
| nested.json | msgspec | 0.613 | 0.622 | 0.653 | 64.766 | 0.43x |
| nested.json | ujson | 2.537 | 2.585 | 3.138 | 64.766 | 0.10x |
| nested.json | json | 5.072 | 5.101 | 5.540 | 64.766 | 0.05x |
| wide_arrays.json | strata | 2.034 | 2.103 | 2.369 | 63.949 | 1.00x |
| wide_arrays.json | orjson | 2.672 | 2.746 | 3.353 | 63.949 | 0.77x |
| wide_arrays.json | msgspec | 3.821 | 3.991 | 4.656 | 63.949 | 0.53x |
| wide_arrays.json | ujson | 11.625 | 11.986 | 12.757 | 63.949 | 0.18x |
| wide_arrays.json | json | 38.192 | 39.030 | 40.356 | 63.949 | 0.05x |
| mixed.json | strata | 0.065 | 0.067 | 0.079 | 60.461 | 1.00x |
| mixed.json | orjson | 0.081 | 0.085 | 0.103 | 60.461 | 0.79x |
| mixed.json | msgspec | 0.114 | 0.119 | 0.134 | 60.461 | 0.56x |
| mixed.json | ujson | 0.493 | 0.496 | 0.549 | 60.461 | 0.14x |
| mixed.json | json | 1.033 | 1.039 | 1.152 | 60.461 | 0.06x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 19.408 | 19.890 | 20.931 | 63.102 | 1.00x |
| users.json | orjson | 27.276 | 27.901 | 30.778 | 63.102 | 0.71x |
| users.json | msgspec | 27.693 | 28.462 | 32.149 | 63.102 | 0.70x |
| users.json | ujson | 40.720 | 42.660 | 45.891 | 63.102 | 0.47x |
| users.json | json | 45.901 | 48.350 | 50.231 | 63.102 | 0.41x |
| flat.json | strata | 1.413 | 1.447 | 1.518 | 64.672 | 1.00x |
| flat.json | orjson | 1.604 | 1.649 | 1.702 | 64.672 | 0.88x |
| flat.json | msgspec | 1.840 | 1.861 | 1.916 | 64.672 | 0.78x |
| flat.json | ujson | 3.061 | 3.165 | 3.317 | 64.672 | 0.46x |
| flat.json | json | 3.495 | 3.545 | 3.630 | 64.672 | 0.41x |
| nested.json | strata | 1.665 | 1.808 | 2.432 | 64.766 | 1.00x |
| nested.json | orjson | 1.967 | 2.115 | 2.525 | 64.766 | 0.86x |
| nested.json | msgspec | 2.200 | 2.412 | 2.745 | 64.766 | 0.75x |
| nested.json | ujson | 3.613 | 3.906 | 5.426 | 64.766 | 0.46x |
| nested.json | json | 4.459 | 4.973 | 6.967 | 64.766 | 0.36x |
| wide_arrays.json | strata | 7.937 | 8.059 | 8.280 | 66.117 | 1.00x |
| wide_arrays.json | orjson | 10.128 | 10.553 | 11.291 | 66.117 | 0.76x |
| wide_arrays.json | msgspec | 11.264 | 11.364 | 12.224 | 66.117 | 0.71x |
| wide_arrays.json | ujson | 14.320 | 14.561 | 14.911 | 66.117 | 0.55x |
| wide_arrays.json | json | 18.776 | 19.053 | 20.138 | 66.117 | 0.42x |
| mixed.json | strata | 0.429 | 0.457 | 0.496 | 60.461 | 1.00x |
| mixed.json | orjson | 0.570 | 0.589 | 0.636 | 60.461 | 0.78x |
| mixed.json | msgspec | 0.593 | 0.627 | 0.688 | 60.461 | 0.73x |
| mixed.json | ujson | 0.785 | 0.822 | 0.879 | 60.461 | 0.56x |
| mixed.json | json | 1.041 | 1.073 | 1.289 | 60.461 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 19.871 | 20.121 | 20.685 | 65.734 | 1.00x |
| users.ndjson | orjson | 28.613 | 29.054 | 29.634 | 65.734 | 0.69x |
| users.ndjson | msgspec | 29.240 | 29.501 | 29.989 | 65.734 | 0.68x |
| users.ndjson | ujson | 42.350 | 42.792 | 44.076 | 65.734 | 0.47x |
| users.ndjson | json | 53.811 | 54.211 | 55.142 | 65.734 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.467 | 3.666 | 3.881 | 63.125 | 1.00x |
| users.json | orjson | 4.652 | 4.727 | 5.153 | 63.125 | 0.78x |
| users.json | msgspec | 6.546 | 6.716 | 7.749 | 63.125 | 0.55x |
| users.json | ujson | 28.035 | 28.443 | 29.458 | 63.125 | 0.13x |
| users.json | json | 46.599 | 47.236 | 49.429 | 63.125 | 0.08x |
| flat.json | strata | 0.695 | 0.738 | 0.808 | 64.672 | 1.00x |
| flat.json | orjson | 0.768 | 0.815 | 0.992 | 64.672 | 0.91x |
| flat.json | msgspec | 0.917 | 0.972 | 1.014 | 64.672 | 0.76x |
| flat.json | ujson | 2.792 | 2.917 | 3.257 | 64.672 | 0.25x |
| flat.json | json | 4.350 | 4.403 | 4.585 | 64.672 | 0.17x |
| nested.json | strata | 0.590 | 0.635 | 0.672 | 64.766 | 1.00x |
| nested.json | orjson | 0.778 | 0.849 | 1.344 | 64.766 | 0.75x |
| nested.json | msgspec | 1.026 | 1.065 | 1.394 | 64.766 | 0.60x |
| nested.json | ujson | 3.021 | 3.218 | 3.848 | 64.766 | 0.20x |
| nested.json | json | 5.606 | 5.930 | 7.092 | 64.766 | 0.11x |
| wide_arrays.json | strata | 2.534 | 2.743 | 3.053 | 66.117 | 1.00x |
| wide_arrays.json | orjson | 3.264 | 3.376 | 3.593 | 66.117 | 0.81x |
| wide_arrays.json | msgspec | 4.503 | 4.686 | 5.548 | 66.117 | 0.59x |
| wide_arrays.json | ujson | 12.270 | 12.322 | 13.172 | 66.117 | 0.22x |
| wide_arrays.json | json | 38.138 | 38.525 | 40.169 | 66.117 | 0.07x |
| mixed.json | strata | 0.335 | 0.383 | 0.423 | 60.461 | 1.00x |
| mixed.json | orjson | 0.371 | 0.405 | 1.013 | 60.461 | 0.94x |
| mixed.json | msgspec | 0.405 | 0.459 | 0.529 | 60.461 | 0.83x |
| mixed.json | ujson | 0.801 | 0.867 | 0.900 | 60.461 | 0.44x |
| mixed.json | json | 1.376 | 1.425 | 1.545 | 60.461 | 0.27x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.147 | 0.182 | 0.343 | 63.195 | 1.00x |
| users.json $[*].id | jmespath | 1.086 | 1.102 | 1.293 | 63.195 | 0.17x |
| users.json $[*].id | jsonpath-ng | 5.818 | 5.899 | 6.594 | 63.195 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.906 | 1.079 | 1.400 | 60.418 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 6.548 | 6.847 | 7.480 | 60.418 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 38.519 | 40.380 | 44.360 | 60.418 | 0.03x |
| users.json $..total | strata | 3.453 | 3.558 | 3.971 | 60.508 | 1.00x |
| users.json $..total | jsonpath-ng | 746.953 | 793.822 | 845.967 | 60.508 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.161 | 4.304 | 4.588 | 63.262 | 1.00x |
| users.json $[*].id | orjson+jmespath | 28.394 | 29.504 | 31.880 | 63.262 | 0.15x |
| users.json $[*].id | orjson+jsonpath-ng | 33.748 | 34.632 | 36.623 | 63.262 | 0.12x |
| users.json $[*].orders[*].total | strata | 4.894 | 4.918 | 5.438 | 60.477 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 34.639 | 36.980 | 39.891 | 60.477 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 73.528 | 79.750 | 89.805 | 60.477 | 0.06x |
| users.json $..total | strata | 23.386 | 23.882 | 25.111 | 60.527 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 788.794 | 802.563 | 832.400 | 60.527 | 0.03x |

