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
| users.json | strata | 18.655 | 21.060 | 24.364 | 57.082 | 1.00x |
| users.json | orjson | 27.500 | 31.799 | 33.209 | 57.082 | 0.66x |
| users.json | msgspec | 27.622 | 29.625 | 36.225 | 57.082 | 0.71x |
| users.json | ujson | 39.817 | 43.505 | 51.527 | 57.082 | 0.48x |
| users.json | pysimdjson | 172.228 | 176.465 | 191.263 | 57.082 | 0.12x |
| users.json | json | 45.535 | 49.035 | 61.865 | 57.082 | 0.43x |
| flat.json | strata | 1.371 | 1.436 | 2.202 | 66.109 | 1.00x |
| flat.json | orjson | 1.515 | 1.600 | 1.738 | 66.109 | 0.90x |
| flat.json | msgspec | 1.708 | 1.774 | 2.273 | 66.109 | 0.81x |
| flat.json | ujson | 2.992 | 3.078 | 4.267 | 66.109 | 0.47x |
| flat.json | pysimdjson | 15.796 | 16.420 | 19.061 | 66.109 | 0.09x |
| flat.json | json | 3.366 | 3.549 | 4.202 | 66.109 | 0.40x |
| nested.json | strata | 1.618 | 1.722 | 1.854 | 65.211 | 1.00x |
| nested.json | orjson | 1.837 | 1.891 | 2.734 | 65.211 | 0.91x |
| nested.json | msgspec | 2.039 | 2.179 | 2.438 | 65.211 | 0.79x |
| nested.json | ujson | 3.355 | 3.501 | 3.996 | 65.211 | 0.49x |
| nested.json | pysimdjson | 14.595 | 15.098 | 18.284 | 65.211 | 0.11x |
| nested.json | json | 4.287 | 4.481 | 6.063 | 65.211 | 0.38x |
| wide_arrays.json | strata | 8.356 | 8.837 | 10.437 | 71.172 | 1.00x |
| wide_arrays.json | orjson | 10.856 | 11.095 | 13.266 | 71.172 | 0.80x |
| wide_arrays.json | msgspec | 11.378 | 11.645 | 13.026 | 71.172 | 0.76x |
| wide_arrays.json | ujson | 14.119 | 14.962 | 15.741 | 71.172 | 0.59x |
| wide_arrays.json | pysimdjson | 85.365 | 89.027 | 91.573 | 71.172 | 0.10x |
| wide_arrays.json | json | 18.346 | 18.973 | 20.795 | 71.172 | 0.47x |
| mixed.json | strata | 0.376 | 0.382 | 0.399 | 63.969 | 1.00x |
| mixed.json | orjson | 0.459 | 0.470 | 0.827 | 63.969 | 0.81x |
| mixed.json | msgspec | 0.486 | 0.493 | 0.580 | 63.969 | 0.78x |
| mixed.json | ujson | 0.659 | 0.675 | 0.830 | 63.969 | 0.57x |
| mixed.json | pysimdjson | 3.358 | 3.383 | 3.546 | 63.969 | 0.11x |
| mixed.json | json | 0.928 | 0.938 | 0.965 | 63.969 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.617 | 2.825 | 3.079 | 52.422 | 1.00x |
| users.json | orjson | 3.581 | 3.823 | 4.023 | 52.422 | 0.74x |
| users.json | msgspec | 5.675 | 5.845 | 6.822 | 52.422 | 0.48x |
| users.json | ujson | 25.770 | 26.813 | 27.967 | 52.422 | 0.11x |
| users.json | json | 43.495 | 44.802 | 46.443 | 52.422 | 0.06x |
| flat.json | strata | 0.362 | 0.445 | 0.698 | 64.625 | 1.00x |
| flat.json | orjson | 0.442 | 0.465 | 0.729 | 64.625 | 0.96x |
| flat.json | msgspec | 0.609 | 0.649 | 1.033 | 64.625 | 0.69x |
| flat.json | ujson | 2.421 | 2.653 | 3.386 | 64.625 | 0.17x |
| flat.json | json | 3.988 | 4.596 | 5.900 | 64.625 | 0.10x |
| nested.json | strata | 0.262 | 0.286 | 0.317 | 65.340 | 1.00x |
| nested.json | orjson | 0.396 | 0.425 | 0.720 | 65.340 | 0.67x |
| nested.json | msgspec | 0.632 | 0.638 | 1.032 | 65.340 | 0.45x |
| nested.json | ujson | 2.526 | 2.577 | 2.777 | 65.340 | 0.11x |
| nested.json | json | 4.973 | 5.082 | 5.465 | 65.340 | 0.06x |
| wide_arrays.json | strata | 2.204 | 2.330 | 2.428 | 64.898 | 1.00x |
| wide_arrays.json | orjson | 2.687 | 2.841 | 3.049 | 64.898 | 0.82x |
| wide_arrays.json | msgspec | 3.847 | 4.048 | 4.739 | 64.898 | 0.58x |
| wide_arrays.json | ujson | 11.787 | 12.207 | 13.594 | 64.898 | 0.19x |
| wide_arrays.json | json | 36.945 | 37.492 | 38.997 | 64.898 | 0.06x |
| mixed.json | strata | 0.073 | 0.089 | 0.134 | 60.770 | 1.00x |
| mixed.json | orjson | 0.089 | 0.103 | 0.127 | 60.770 | 0.86x |
| mixed.json | msgspec | 0.124 | 0.138 | 0.212 | 60.770 | 0.64x |
| mixed.json | ujson | 0.477 | 0.497 | 0.655 | 60.770 | 0.18x |
| mixed.json | json | 0.996 | 1.065 | 1.361 | 60.770 | 0.08x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 18.945 | 19.703 | 21.835 | 63.203 | 1.00x |
| users.json | orjson | 27.750 | 29.949 | 31.404 | 63.203 | 0.66x |
| users.json | msgspec | 27.536 | 29.818 | 34.291 | 63.203 | 0.66x |
| users.json | ujson | 41.380 | 43.835 | 49.798 | 63.203 | 0.45x |
| users.json | json | 46.015 | 48.813 | 52.529 | 63.203 | 0.40x |
| flat.json | strata | 1.568 | 1.971 | 2.247 | 65.246 | 1.00x |
| flat.json | orjson | 1.842 | 2.181 | 2.624 | 65.246 | 0.90x |
| flat.json | msgspec | 1.922 | 2.307 | 2.658 | 65.246 | 0.85x |
| flat.json | ujson | 3.302 | 3.789 | 4.547 | 65.246 | 0.52x |
| flat.json | json | 3.738 | 4.375 | 5.208 | 65.246 | 0.45x |
| nested.json | strata | 1.694 | 1.774 | 2.042 | 65.340 | 1.00x |
| nested.json | orjson | 1.969 | 2.138 | 2.835 | 65.340 | 0.83x |
| nested.json | msgspec | 2.226 | 2.310 | 3.256 | 65.340 | 0.77x |
| nested.json | ujson | 3.493 | 3.736 | 4.775 | 65.340 | 0.47x |
| nested.json | json | 4.366 | 4.730 | 5.629 | 65.340 | 0.38x |
| wide_arrays.json | strata | 8.007 | 8.107 | 10.224 | 66.129 | 1.00x |
| wide_arrays.json | orjson | 10.009 | 10.454 | 11.779 | 66.129 | 0.78x |
| wide_arrays.json | msgspec | 10.827 | 11.335 | 12.268 | 66.129 | 0.72x |
| wide_arrays.json | ujson | 13.752 | 13.932 | 15.529 | 66.129 | 0.58x |
| wide_arrays.json | json | 17.825 | 18.267 | 21.959 | 66.129 | 0.44x |
| mixed.json | strata | 0.444 | 0.476 | 1.732 | 60.770 | 1.00x |
| mixed.json | orjson | 0.595 | 0.621 | 2.398 | 60.770 | 0.77x |
| mixed.json | msgspec | 0.638 | 0.660 | 1.819 | 60.770 | 0.72x |
| mixed.json | ujson | 0.825 | 0.839 | 2.454 | 60.770 | 0.57x |
| mixed.json | json | 1.056 | 1.148 | 4.593 | 60.770 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 20.717 | 21.273 | 32.447 | 65.688 | 1.00x |
| users.ndjson | orjson | 30.402 | 32.896 | 50.563 | 65.688 | 0.65x |
| users.ndjson | msgspec | 31.358 | 32.781 | 51.397 | 65.688 | 0.65x |
| users.ndjson | ujson | 44.635 | 46.940 | 64.299 | 65.688 | 0.45x |
| users.ndjson | json | 55.331 | 57.189 | 79.575 | 65.688 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.624 | 3.805 | 4.004 | 63.215 | 1.00x |
| users.json | orjson | 4.884 | 5.036 | 5.869 | 63.215 | 0.76x |
| users.json | msgspec | 6.648 | 6.916 | 28.973 | 63.215 | 0.55x |
| users.json | ujson | 27.139 | 28.269 | 30.457 | 63.215 | 0.13x |
| users.json | json | 44.763 | 45.765 | 67.039 | 63.215 | 0.08x |
| flat.json | strata | 0.730 | 0.978 | 1.248 | 65.246 | 1.00x |
| flat.json | orjson | 0.827 | 0.933 | 1.195 | 65.246 | 1.05x |
| flat.json | msgspec | 0.994 | 1.121 | 1.462 | 65.246 | 0.87x |
| flat.json | ujson | 3.164 | 3.440 | 4.176 | 65.246 | 0.28x |
| flat.json | json | 4.473 | 5.713 | 6.203 | 65.246 | 0.17x |
| nested.json | strata | 0.630 | 0.680 | 0.934 | 65.340 | 1.00x |
| nested.json | orjson | 0.765 | 0.854 | 0.957 | 65.340 | 0.80x |
| nested.json | msgspec | 1.067 | 1.135 | 1.203 | 65.340 | 0.60x |
| nested.json | ujson | 2.923 | 3.152 | 3.388 | 65.340 | 0.22x |
| nested.json | json | 5.587 | 5.684 | 6.757 | 65.340 | 0.12x |
| wide_arrays.json | strata | 2.981 | 3.338 | 4.207 | 64.898 | 1.00x |
| wide_arrays.json | orjson | 3.513 | 3.830 | 4.928 | 64.898 | 0.87x |
| wide_arrays.json | msgspec | 4.688 | 5.174 | 6.128 | 64.898 | 0.65x |
| wide_arrays.json | ujson | 12.785 | 13.369 | 15.426 | 64.898 | 0.25x |
| wide_arrays.json | json | 36.944 | 37.810 | 40.944 | 64.898 | 0.09x |
| mixed.json | strata | 0.351 | 0.403 | 0.578 | 60.770 | 1.00x |
| mixed.json | orjson | 0.434 | 0.452 | 0.523 | 60.770 | 0.89x |
| mixed.json | msgspec | 0.457 | 0.484 | 0.539 | 60.770 | 0.83x |
| mixed.json | ujson | 0.842 | 0.883 | 1.029 | 60.770 | 0.46x |
| mixed.json | json | 1.379 | 1.394 | 1.478 | 60.770 | 0.29x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.154 | 0.194 | 0.255 | 63.285 | 1.00x |
| users.json $[*].id | jmespath | 1.017 | 1.056 | 1.316 | 63.285 | 0.18x |
| users.json $[*].id | jsonpath-ng | 5.511 | 5.902 | 7.172 | 63.285 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.983 | 1.300 | 2.146 | 60.414 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 6.344 | 6.977 | 8.691 | 60.414 | 0.19x |
| users.json $[*].orders[*].total | jsonpath-ng | 37.268 | 41.088 | 50.883 | 60.414 | 0.03x |
| users.json $..total | strata | 3.650 | 3.961 | 5.086 | 60.504 | 1.00x |
| users.json $..total | jsonpath-ng | 735.172 | 752.485 | 881.635 | 60.504 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.993 | 4.111 | 4.724 | 63.336 | 1.00x |
| users.json $[*].id | orjson+jmespath | 29.524 | 31.629 | 35.605 | 63.336 | 0.13x |
| users.json $[*].id | orjson+jsonpath-ng | 32.904 | 36.304 | 42.516 | 63.336 | 0.11x |
| users.json $[*].orders[*].total | strata | 4.452 | 4.557 | 4.856 | 60.445 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 35.193 | 36.985 | 40.386 | 60.445 | 0.12x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 74.250 | 83.563 | 91.759 | 60.445 | 0.05x |
| users.json $..total | strata | 24.369 | 25.945 | 33.795 | 60.527 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 822.001 | 837.728 | 934.792 | 60.527 | 0.03x |

