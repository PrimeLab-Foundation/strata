# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 38eaa9f248f509d9a4beb75b267189d011c1109e
- python: 3.12.10
- implementation: CPython
- platform: macOS-15.7.9-x86_64-i386-64bit
- machine: x86_64
- processor: Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz
- compiler_flags: -fno-strict-overflow -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -O3 -Wall -arch x86_64 -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -fomit-frame-pointer -march=native -flto=thin -fprofile-use=/Users/runner/work/_temp/strata-arm/build/pgo/strata.profdata -bundle -undefined (19 recorded commands; see the JSON companion)
- repeats: 60
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 17.121 | 18.638 | 19.441 | 56.746 | 1.00x |
| users.json | orjson | 23.302 | 26.171 | 29.601 | 56.746 | 0.71x |
| users.json | msgspec | 23.151 | 27.025 | 30.986 | 56.746 | 0.69x |
| users.json | ujson | 34.458 | 38.635 | 43.475 | 56.746 | 0.48x |
| users.json | pysimdjson | 156.204 | 164.688 | 173.091 | 56.746 | 0.11x |
| users.json | json | 39.110 | 43.960 | 46.951 | 56.746 | 0.42x |
| flat.json | strata | 1.171 | 1.253 | 1.493 | 71.516 | 1.00x |
| flat.json | orjson | 1.299 | 1.382 | 1.552 | 71.516 | 0.91x |
| flat.json | msgspec | 1.479 | 1.549 | 1.651 | 71.516 | 0.81x |
| flat.json | ujson | 2.596 | 2.727 | 2.863 | 71.516 | 0.46x |
| flat.json | pysimdjson | 13.928 | 14.405 | 15.302 | 71.516 | 0.09x |
| flat.json | json | 2.999 | 3.102 | 3.615 | 71.516 | 0.40x |
| nested.json | strata | 1.316 | 1.520 | 1.822 | 67.273 | 1.00x |
| nested.json | orjson | 1.521 | 1.695 | 1.995 | 67.273 | 0.90x |
| nested.json | msgspec | 1.720 | 1.829 | 2.219 | 67.273 | 0.83x |
| nested.json | ujson | 2.798 | 3.046 | 3.428 | 67.273 | 0.50x |
| nested.json | pysimdjson | 12.488 | 13.646 | 14.314 | 67.273 | 0.11x |
| nested.json | json | 3.565 | 3.954 | 4.355 | 67.273 | 0.38x |
| wide_arrays.json | strata | 7.170 | 7.921 | 8.881 | 69.398 | 1.00x |
| wide_arrays.json | orjson | 8.683 | 10.059 | 10.805 | 69.398 | 0.79x |
| wide_arrays.json | msgspec | 9.436 | 10.713 | 12.938 | 69.398 | 0.74x |
| wide_arrays.json | ujson | 11.869 | 13.662 | 16.087 | 69.398 | 0.58x |
| wide_arrays.json | pysimdjson | 75.686 | 81.703 | 85.601 | 69.398 | 0.10x |
| wide_arrays.json | json | 15.964 | 17.814 | 20.148 | 69.398 | 0.44x |
| mixed.json | strata | 0.374 | 0.394 | 0.447 | 66.551 | 1.00x |
| mixed.json | orjson | 0.452 | 0.476 | 0.564 | 66.551 | 0.83x |
| mixed.json | msgspec | 0.478 | 0.502 | 0.564 | 66.551 | 0.78x |
| mixed.json | ujson | 0.666 | 0.689 | 0.966 | 66.551 | 0.57x |
| mixed.json | pysimdjson | 3.346 | 3.422 | 3.773 | 66.551 | 0.12x |
| mixed.json | json | 0.920 | 0.951 | 1.059 | 66.551 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.289 | 2.756 | 3.311 | 52.031 | 1.00x |
| users.json | orjson | 3.123 | 3.596 | 4.183 | 52.031 | 0.77x |
| users.json | msgspec | 4.863 | 5.346 | 5.806 | 52.031 | 0.52x |
| users.json | ujson | 23.809 | 25.347 | 27.174 | 52.031 | 0.11x |
| users.json | json | 39.805 | 42.921 | 46.527 | 52.031 | 0.06x |
| flat.json | strata | 0.314 | 0.350 | 0.414 | 67.340 | 1.00x |
| flat.json | orjson | 0.372 | 0.415 | 0.443 | 67.340 | 0.84x |
| flat.json | msgspec | 0.499 | 0.560 | 0.645 | 67.340 | 0.63x |
| flat.json | ujson | 2.135 | 2.231 | 2.454 | 67.340 | 0.16x |
| flat.json | json | 3.491 | 3.622 | 4.312 | 67.340 | 0.10x |
| nested.json | strata | 0.213 | 0.230 | 0.268 | 59.012 | 1.00x |
| nested.json | orjson | 0.334 | 0.357 | 0.384 | 59.012 | 0.64x |
| nested.json | msgspec | 0.525 | 0.549 | 0.618 | 59.012 | 0.42x |
| nested.json | ujson | 2.260 | 2.310 | 2.404 | 59.012 | 0.10x |
| nested.json | json | 4.490 | 4.568 | 4.886 | 59.012 | 0.05x |
| wide_arrays.json | strata | 1.748 | 2.091 | 2.502 | 64.539 | 1.00x |
| wide_arrays.json | orjson | 2.196 | 2.587 | 2.892 | 64.539 | 0.81x |
| wide_arrays.json | msgspec | 3.099 | 3.552 | 3.853 | 64.539 | 0.59x |
| wide_arrays.json | ujson | 9.967 | 10.731 | 11.475 | 64.539 | 0.19x |
| wide_arrays.json | json | 33.043 | 34.679 | 36.495 | 64.539 | 0.06x |
| mixed.json | strata | 0.059 | 0.070 | 0.081 | 63.281 | 1.00x |
| mixed.json | orjson | 0.071 | 0.084 | 0.106 | 63.281 | 0.84x |
| mixed.json | msgspec | 0.102 | 0.119 | 0.140 | 63.281 | 0.59x |
| mixed.json | ujson | 0.454 | 0.473 | 0.512 | 63.281 | 0.15x |
| mixed.json | json | 0.958 | 0.989 | 1.064 | 63.281 | 0.07x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 17.422 | 18.680 | 19.686 | 66.582 | 1.00x |
| users.json | orjson | 23.590 | 26.834 | 29.986 | 66.582 | 0.70x |
| users.json | msgspec | 23.974 | 26.622 | 29.725 | 66.582 | 0.70x |
| users.json | ujson | 35.828 | 39.338 | 43.360 | 66.582 | 0.47x |
| users.json | json | 40.421 | 43.995 | 48.527 | 66.582 | 0.42x |
| flat.json | strata | 1.253 | 1.395 | 1.485 | 67.340 | 1.00x |
| flat.json | orjson | 1.417 | 1.570 | 1.700 | 67.340 | 0.89x |
| flat.json | msgspec | 1.602 | 1.760 | 1.864 | 67.340 | 0.79x |
| flat.json | ujson | 2.725 | 2.991 | 3.255 | 67.340 | 0.47x |
| flat.json | json | 3.103 | 3.355 | 3.706 | 67.340 | 0.42x |
| nested.json | strata | 1.430 | 1.560 | 1.861 | 59.312 | 1.00x |
| nested.json | orjson | 1.638 | 1.808 | 2.648 | 59.312 | 0.86x |
| nested.json | msgspec | 1.802 | 1.976 | 2.575 | 59.312 | 0.79x |
| nested.json | ujson | 2.930 | 3.187 | 3.598 | 59.312 | 0.49x |
| nested.json | json | 3.704 | 3.975 | 4.875 | 59.312 | 0.39x |
| wide_arrays.json | strata | 7.293 | 7.663 | 8.406 | 66.496 | 1.00x |
| wide_arrays.json | orjson | 8.814 | 9.569 | 10.706 | 66.496 | 0.80x |
| wide_arrays.json | msgspec | 9.869 | 10.524 | 11.377 | 66.496 | 0.73x |
| wide_arrays.json | ujson | 12.750 | 13.688 | 14.805 | 66.496 | 0.56x |
| wide_arrays.json | json | 16.263 | 17.715 | 18.799 | 66.496 | 0.43x |
| mixed.json | strata | 0.385 | 0.427 | 0.527 | 63.281 | 1.00x |
| mixed.json | orjson | 0.491 | 0.560 | 0.607 | 63.281 | 0.76x |
| mixed.json | msgspec | 0.522 | 0.596 | 0.652 | 63.281 | 0.72x |
| mixed.json | ujson | 0.684 | 0.772 | 0.834 | 63.281 | 0.55x |
| mixed.json | json | 0.906 | 0.981 | 1.074 | 63.281 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 18.779 | 20.173 | 21.775 | 70.555 | 1.00x |
| users.ndjson | orjson | 26.803 | 29.076 | 31.855 | 70.555 | 0.69x |
| users.ndjson | msgspec | 27.188 | 29.587 | 32.663 | 70.555 | 0.68x |
| users.ndjson | ujson | 38.654 | 42.972 | 48.473 | 70.555 | 0.47x |
| users.ndjson | json | 46.203 | 53.834 | 58.186 | 70.555 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.152 | 3.511 | 4.139 | 63.273 | 1.00x |
| users.json | orjson | 4.076 | 4.453 | 5.032 | 63.273 | 0.79x |
| users.json | msgspec | 5.813 | 6.251 | 6.817 | 63.273 | 0.56x |
| users.json | ujson | 24.585 | 26.113 | 27.675 | 63.273 | 0.13x |
| users.json | json | 40.021 | 42.751 | 45.001 | 63.273 | 0.08x |
| flat.json | strata | 0.557 | 0.682 | 0.764 | 67.340 | 1.00x |
| flat.json | orjson | 0.644 | 0.763 | 0.852 | 67.340 | 0.89x |
| flat.json | msgspec | 0.775 | 0.904 | 1.008 | 67.340 | 0.75x |
| flat.json | ujson | 2.388 | 2.578 | 2.978 | 67.340 | 0.26x |
| flat.json | json | 3.673 | 3.960 | 4.254 | 67.340 | 0.17x |
| nested.json | strata | 0.494 | 0.623 | 0.899 | 59.312 | 1.00x |
| nested.json | orjson | 0.639 | 0.825 | 1.171 | 59.312 | 0.75x |
| nested.json | msgspec | 0.831 | 1.060 | 1.482 | 59.312 | 0.59x |
| nested.json | ujson | 2.526 | 3.064 | 4.206 | 59.312 | 0.20x |
| nested.json | json | 4.755 | 5.652 | 7.797 | 59.312 | 0.11x |
| wide_arrays.json | strata | 2.314 | 2.760 | 3.216 | 66.629 | 1.00x |
| wide_arrays.json | orjson | 2.992 | 3.344 | 3.795 | 66.629 | 0.83x |
| wide_arrays.json | msgspec | 3.743 | 4.318 | 4.817 | 66.629 | 0.64x |
| wide_arrays.json | ujson | 10.760 | 11.610 | 12.398 | 66.629 | 0.24x |
| wide_arrays.json | json | 33.666 | 35.642 | 37.566 | 66.629 | 0.08x |
| mixed.json | strata | 0.302 | 0.331 | 0.384 | 63.281 | 1.00x |
| mixed.json | orjson | 0.341 | 0.377 | 0.466 | 63.281 | 0.88x |
| mixed.json | msgspec | 0.357 | 0.424 | 0.520 | 63.281 | 0.78x |
| mixed.json | ujson | 0.716 | 0.778 | 0.869 | 63.281 | 0.43x |
| mixed.json | json | 1.193 | 1.276 | 1.525 | 63.281 | 0.26x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.126 | 0.153 | 0.201 | 61.949 | 1.00x |
| users.json $[*].id | jmespath | 0.887 | 0.933 | 1.039 | 61.949 | 0.16x |
| users.json $[*].id | jsonpath-ng | 4.938 | 5.125 | 5.755 | 61.949 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.783 | 1.004 | 1.383 | 64.684 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.553 | 6.124 | 7.071 | 64.684 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 32.968 | 36.535 | 40.403 | 64.684 | 0.03x |
| users.json $..total | strata | 2.977 | 3.453 | 3.918 | 64.711 | 1.00x |
| users.json $..total | jsonpath-ng | 682.914 | 710.473 | 745.432 | 64.711 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.585 | 3.763 | 4.158 | 65.961 | 1.00x |
| users.json $[*].id | orjson+jmespath | 24.339 | 28.115 | 32.474 | 65.961 | 0.13x |
| users.json $[*].id | orjson+jsonpath-ng | 28.421 | 32.388 | 39.950 | 65.961 | 0.12x |
| users.json $[*].orders[*].total | strata | 4.001 | 4.152 | 4.586 | 64.707 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 29.317 | 33.011 | 36.276 | 64.707 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 63.483 | 67.947 | 84.920 | 64.707 | 0.06x |
| users.json $..total | strata | 21.740 | 23.632 | 24.708 | 64.715 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 718.884 | 743.844 | 775.890 | 64.715 | 0.03x |

