# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 6e4d93e74610755e4c46406e91c8aaf4ab6817a1
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
| users.json | strata | 16.677 | 16.960 | 19.564 | 57.047 | 1.00x |
| users.json | orjson | 22.983 | 24.119 | 26.535 | 57.047 | 0.70x |
| users.json | msgspec | 22.697 | 24.611 | 26.441 | 57.047 | 0.69x |
| users.json | ujson | 34.898 | 35.498 | 39.570 | 57.047 | 0.48x |
| users.json | pysimdjson | 151.841 | 152.872 | 155.847 | 57.047 | 0.11x |
| users.json | json | 39.386 | 40.895 | 43.866 | 57.047 | 0.41x |
| flat.json | strata | 1.174 | 1.186 | 1.199 | 67.949 | 1.00x |
| flat.json | orjson | 1.294 | 1.321 | 1.356 | 67.949 | 0.90x |
| flat.json | msgspec | 1.458 | 1.485 | 1.521 | 67.949 | 0.80x |
| flat.json | ujson | 2.535 | 2.610 | 3.004 | 67.949 | 0.45x |
| flat.json | pysimdjson | 13.765 | 13.938 | 16.615 | 67.949 | 0.09x |
| flat.json | json | 2.962 | 2.980 | 3.148 | 67.949 | 0.40x |
| nested.json | strata | 1.260 | 1.367 | 1.473 | 66.422 | 1.00x |
| nested.json | orjson | 1.444 | 1.562 | 1.585 | 66.422 | 0.87x |
| nested.json | msgspec | 1.625 | 1.723 | 1.958 | 66.422 | 0.79x |
| nested.json | ujson | 2.760 | 2.865 | 2.926 | 66.422 | 0.48x |
| nested.json | pysimdjson | 12.159 | 12.648 | 12.792 | 66.422 | 0.11x |
| nested.json | json | 3.521 | 3.654 | 3.824 | 66.422 | 0.37x |
| wide_arrays.json | strata | 7.003 | 7.334 | 7.704 | 72.387 | 1.00x |
| wide_arrays.json | orjson | 8.870 | 8.973 | 9.808 | 72.387 | 0.82x |
| wide_arrays.json | msgspec | 9.508 | 9.726 | 10.371 | 72.387 | 0.75x |
| wide_arrays.json | ujson | 12.187 | 12.269 | 13.304 | 72.387 | 0.60x |
| wide_arrays.json | pysimdjson | 74.426 | 75.051 | 75.866 | 72.387 | 0.10x |
| wide_arrays.json | json | 15.981 | 16.074 | 16.126 | 72.387 | 0.46x |
| mixed.json | strata | 0.328 | 0.333 | 1.079 | 65.184 | 1.00x |
| mixed.json | orjson | 0.407 | 0.413 | 0.450 | 65.184 | 0.81x |
| mixed.json | msgspec | 0.428 | 0.435 | 0.451 | 65.184 | 0.76x |
| mixed.json | ujson | 0.590 | 0.594 | 0.629 | 65.184 | 0.56x |
| mixed.json | pysimdjson | 3.026 | 3.045 | 3.086 | 65.184 | 0.11x |
| mixed.json | json | 0.829 | 0.837 | 0.892 | 65.184 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.136 | 2.289 | 2.605 | 52.363 | 1.00x |
| users.json | orjson | 3.044 | 3.211 | 4.032 | 52.363 | 0.71x |
| users.json | msgspec | 4.757 | 4.897 | 5.191 | 52.363 | 0.47x |
| users.json | ujson | 22.868 | 23.109 | 23.537 | 52.363 | 0.10x |
| users.json | json | 38.520 | 39.078 | 39.217 | 52.363 | 0.06x |
| flat.json | strata | 0.305 | 0.316 | 0.331 | 66.461 | 1.00x |
| flat.json | orjson | 0.373 | 0.385 | 0.518 | 66.461 | 0.82x |
| flat.json | msgspec | 0.497 | 0.518 | 0.542 | 66.461 | 0.61x |
| flat.json | ujson | 2.076 | 2.095 | 2.151 | 66.461 | 0.15x |
| flat.json | json | 3.419 | 3.458 | 3.633 | 66.461 | 0.09x |
| nested.json | strata | 0.218 | 0.220 | 0.281 | 66.555 | 1.00x |
| nested.json | orjson | 0.324 | 0.330 | 0.381 | 66.555 | 0.67x |
| nested.json | msgspec | 0.513 | 0.517 | 0.536 | 66.555 | 0.42x |
| nested.json | ujson | 2.166 | 2.178 | 2.223 | 66.555 | 0.10x |
| nested.json | json | 4.310 | 4.325 | 4.403 | 66.555 | 0.05x |
| wide_arrays.json | strata | 1.597 | 1.917 | 2.011 | 66.113 | 1.00x |
| wide_arrays.json | orjson | 2.194 | 2.340 | 2.716 | 66.113 | 0.82x |
| wide_arrays.json | msgspec | 3.123 | 3.344 | 7.876 | 66.113 | 0.57x |
| wide_arrays.json | ujson | 9.257 | 10.491 | 10.732 | 66.113 | 0.18x |
| wide_arrays.json | json | 32.137 | 32.299 | 33.710 | 66.113 | 0.06x |
| mixed.json | strata | 0.057 | 0.061 | 0.069 | 61.984 | 1.00x |
| mixed.json | orjson | 0.073 | 0.078 | 0.087 | 61.984 | 0.77x |
| mixed.json | msgspec | 0.100 | 0.109 | 0.164 | 61.984 | 0.56x |
| mixed.json | ujson | 0.424 | 0.426 | 0.442 | 61.984 | 0.14x |
| mixed.json | json | 0.885 | 0.898 | 0.945 | 61.984 | 0.07x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 16.811 | 17.087 | 18.695 | 63.156 | 1.00x |
| users.json | orjson | 23.575 | 24.067 | 24.875 | 63.156 | 0.71x |
| users.json | msgspec | 23.768 | 24.149 | 25.330 | 63.156 | 0.71x |
| users.json | ujson | 35.507 | 36.011 | 37.640 | 63.156 | 0.47x |
| users.json | json | 38.571 | 40.199 | 42.284 | 63.156 | 0.43x |
| flat.json | strata | 1.204 | 1.328 | 1.500 | 66.461 | 1.00x |
| flat.json | orjson | 1.359 | 1.472 | 1.784 | 66.461 | 0.90x |
| flat.json | msgspec | 1.598 | 1.649 | 1.707 | 66.461 | 0.81x |
| flat.json | ujson | 2.632 | 2.856 | 2.951 | 66.461 | 0.46x |
| flat.json | json | 2.918 | 3.135 | 3.278 | 66.461 | 0.42x |
| nested.json | strata | 1.356 | 1.500 | 1.619 | 66.555 | 1.00x |
| nested.json | orjson | 1.562 | 1.745 | 1.825 | 66.555 | 0.86x |
| nested.json | msgspec | 1.784 | 1.893 | 2.076 | 66.555 | 0.79x |
| nested.json | ujson | 2.799 | 3.009 | 3.171 | 66.555 | 0.50x |
| nested.json | json | 3.674 | 3.871 | 4.278 | 66.555 | 0.39x |
| wide_arrays.json | strata | 7.260 | 7.332 | 7.904 | 67.344 | 1.00x |
| wide_arrays.json | orjson | 8.755 | 8.969 | 9.206 | 67.344 | 0.82x |
| wide_arrays.json | msgspec | 9.780 | 9.921 | 10.472 | 67.344 | 0.74x |
| wide_arrays.json | ujson | 12.299 | 12.337 | 12.490 | 67.344 | 0.59x |
| wide_arrays.json | json | 16.016 | 16.088 | 17.239 | 67.344 | 0.46x |
| mixed.json | strata | 0.428 | 0.437 | 0.460 | 61.984 | 1.00x |
| mixed.json | orjson | 0.546 | 0.558 | 0.568 | 61.984 | 0.78x |
| mixed.json | msgspec | 0.576 | 0.596 | 0.603 | 61.984 | 0.73x |
| mixed.json | ujson | 0.756 | 0.774 | 0.847 | 61.984 | 0.56x |
| mixed.json | json | 0.986 | 1.001 | 1.210 | 61.984 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 17.509 | 17.695 | 18.863 | 67.527 | 1.00x |
| users.ndjson | orjson | 25.220 | 25.868 | 26.045 | 67.527 | 0.68x |
| users.ndjson | msgspec | 25.472 | 25.884 | 27.814 | 67.527 | 0.68x |
| users.ndjson | ujson | 36.604 | 36.952 | 37.632 | 67.527 | 0.48x |
| users.ndjson | json | 45.750 | 46.523 | 49.177 | 67.527 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.028 | 3.136 | 3.733 | 63.195 | 1.00x |
| users.json | orjson | 4.081 | 4.239 | 4.395 | 63.195 | 0.74x |
| users.json | msgspec | 5.569 | 5.795 | 6.089 | 63.195 | 0.54x |
| users.json | ujson | 24.313 | 24.358 | 24.633 | 63.195 | 0.13x |
| users.json | json | 40.235 | 40.540 | 40.756 | 63.195 | 0.08x |
| flat.json | strata | 0.617 | 0.642 | 0.881 | 66.461 | 1.00x |
| flat.json | orjson | 0.712 | 0.743 | 0.814 | 66.461 | 0.86x |
| flat.json | msgspec | 0.804 | 0.880 | 0.926 | 66.461 | 0.73x |
| flat.json | ujson | 2.472 | 2.496 | 2.644 | 66.461 | 0.26x |
| flat.json | json | 3.766 | 3.847 | 4.134 | 66.461 | 0.17x |
| nested.json | strata | 0.469 | 0.535 | 0.599 | 66.555 | 1.00x |
| nested.json | orjson | 0.592 | 0.665 | 0.827 | 66.555 | 0.80x |
| nested.json | msgspec | 0.794 | 0.869 | 0.998 | 66.555 | 0.62x |
| nested.json | ujson | 2.469 | 2.607 | 3.003 | 66.555 | 0.21x |
| nested.json | json | 4.694 | 4.711 | 4.867 | 66.555 | 0.11x |
| wide_arrays.json | strata | 2.619 | 2.710 | 3.124 | 66.113 | 1.00x |
| wide_arrays.json | orjson | 3.127 | 3.276 | 3.558 | 66.113 | 0.83x |
| wide_arrays.json | msgspec | 4.131 | 4.346 | 5.410 | 66.113 | 0.62x |
| wide_arrays.json | ujson | 11.421 | 11.719 | 13.703 | 66.113 | 0.23x |
| wide_arrays.json | json | 33.410 | 33.611 | 34.694 | 66.113 | 0.08x |
| mixed.json | strata | 0.353 | 0.372 | 0.410 | 61.984 | 1.00x |
| mixed.json | orjson | 0.403 | 0.434 | 0.513 | 61.984 | 0.86x |
| mixed.json | msgspec | 0.439 | 0.456 | 0.494 | 61.984 | 0.82x |
| mixed.json | ujson | 0.781 | 0.798 | 0.947 | 61.984 | 0.47x |
| mixed.json | json | 1.251 | 1.286 | 1.513 | 61.984 | 0.29x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.120 | 0.136 | 0.175 | 63.258 | 1.00x |
| users.json $[*].id | jmespath | 0.879 | 0.898 | 0.942 | 63.258 | 0.15x |
| users.json $[*].id | jsonpath-ng | 4.870 | 4.919 | 5.132 | 63.258 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.794 | 0.896 | 1.127 | 60.445 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.398 | 5.718 | 6.318 | 60.445 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 32.470 | 33.594 | 36.191 | 60.445 | 0.03x |
| users.json $..total | strata | 2.952 | 3.277 | 3.477 | 60.543 | 1.00x |
| users.json $..total | jsonpath-ng | 648.667 | 655.722 | 669.747 | 60.543 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.587 | 3.613 | 3.933 | 63.320 | 1.00x |
| users.json $[*].id | orjson+jmespath | 24.207 | 26.654 | 29.367 | 63.320 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 29.440 | 31.137 | 33.362 | 63.320 | 0.12x |
| users.json $[*].orders[*].total | strata | 3.961 | 4.037 | 4.359 | 60.469 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 29.722 | 30.485 | 32.529 | 60.469 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 61.489 | 61.987 | 64.142 | 60.469 | 0.07x |
| users.json $..total | strata | 20.675 | 21.177 | 21.760 | 60.543 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 676.052 | 680.772 | 690.002 | 60.543 | 0.03x |

