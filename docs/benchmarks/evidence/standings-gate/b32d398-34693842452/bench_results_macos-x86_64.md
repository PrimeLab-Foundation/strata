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
| users.json | strata | 16.702 | 16.860 | 20.022 | 57.051 | 1.00x |
| users.json | orjson | 24.243 | 25.043 | 28.090 | 57.051 | 0.67x |
| users.json | msgspec | 23.688 | 24.555 | 27.596 | 57.051 | 0.69x |
| users.json | ujson | 34.941 | 36.868 | 39.587 | 57.051 | 0.46x |
| users.json | pysimdjson | 153.094 | 154.422 | 169.823 | 57.051 | 0.11x |
| users.json | json | 39.720 | 40.241 | 43.789 | 57.051 | 0.42x |
| flat.json | strata | 1.103 | 1.170 | 1.197 | 66.137 | 1.00x |
| flat.json | orjson | 1.293 | 1.317 | 1.633 | 66.137 | 0.89x |
| flat.json | msgspec | 1.485 | 1.493 | 1.622 | 66.137 | 0.78x |
| flat.json | ujson | 2.556 | 2.602 | 2.706 | 66.137 | 0.45x |
| flat.json | pysimdjson | 13.379 | 13.817 | 14.115 | 66.137 | 0.08x |
| flat.json | json | 2.950 | 3.010 | 3.102 | 66.137 | 0.39x |
| nested.json | strata | 1.322 | 1.369 | 1.435 | 65.273 | 1.00x |
| nested.json | orjson | 1.541 | 1.572 | 1.623 | 65.273 | 0.87x |
| nested.json | msgspec | 1.692 | 1.721 | 1.760 | 65.273 | 0.80x |
| nested.json | ujson | 2.826 | 2.855 | 3.196 | 65.273 | 0.48x |
| nested.json | pysimdjson | 12.050 | 12.654 | 13.078 | 65.273 | 0.11x |
| nested.json | json | 3.606 | 3.654 | 4.313 | 65.273 | 0.37x |
| wide_arrays.json | strata | 6.938 | 7.096 | 7.177 | 71.234 | 1.00x |
| wide_arrays.json | orjson | 9.036 | 9.205 | 9.369 | 71.234 | 0.77x |
| wide_arrays.json | msgspec | 9.651 | 9.803 | 10.170 | 71.234 | 0.72x |
| wide_arrays.json | ujson | 11.956 | 12.483 | 13.011 | 71.234 | 0.57x |
| wide_arrays.json | pysimdjson | 74.749 | 75.511 | 84.379 | 71.234 | 0.09x |
| wide_arrays.json | json | 16.114 | 16.250 | 16.863 | 71.234 | 0.44x |
| mixed.json | strata | 0.313 | 0.329 | 0.351 | 65.355 | 1.00x |
| mixed.json | orjson | 0.409 | 0.413 | 0.429 | 65.355 | 0.80x |
| mixed.json | msgspec | 0.416 | 0.444 | 0.470 | 65.355 | 0.74x |
| mixed.json | ujson | 0.589 | 0.598 | 0.625 | 65.355 | 0.55x |
| mixed.json | pysimdjson | 3.028 | 3.051 | 3.506 | 65.355 | 0.11x |
| mixed.json | json | 0.824 | 0.838 | 0.866 | 65.355 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.323 | 2.410 | 2.590 | 52.391 | 1.00x |
| users.json | orjson | 3.148 | 3.209 | 3.443 | 52.391 | 0.75x |
| users.json | msgspec | 4.868 | 5.006 | 5.258 | 52.391 | 0.48x |
| users.json | ujson | 23.228 | 23.472 | 24.178 | 52.391 | 0.10x |
| users.json | json | 38.998 | 39.592 | 40.319 | 52.391 | 0.06x |
| flat.json | strata | 0.295 | 0.310 | 0.354 | 64.656 | 1.00x |
| flat.json | orjson | 0.367 | 0.387 | 0.423 | 64.656 | 0.80x |
| flat.json | msgspec | 0.462 | 0.502 | 0.543 | 64.656 | 0.62x |
| flat.json | ujson | 2.005 | 2.081 | 2.286 | 64.656 | 0.15x |
| flat.json | json | 3.260 | 3.454 | 3.561 | 64.656 | 0.09x |
| nested.json | strata | 0.203 | 0.224 | 0.250 | 65.402 | 1.00x |
| nested.json | orjson | 0.300 | 0.327 | 0.363 | 65.402 | 0.68x |
| nested.json | msgspec | 0.508 | 0.523 | 0.673 | 65.402 | 0.43x |
| nested.json | ujson | 2.170 | 2.181 | 2.623 | 65.402 | 0.10x |
| nested.json | json | 4.158 | 4.357 | 4.605 | 65.402 | 0.05x |
| wide_arrays.json | strata | 2.024 | 2.079 | 2.519 | 64.586 | 1.00x |
| wide_arrays.json | orjson | 2.482 | 2.577 | 2.828 | 64.586 | 0.81x |
| wide_arrays.json | msgspec | 3.502 | 3.561 | 4.053 | 64.586 | 0.58x |
| wide_arrays.json | ujson | 10.052 | 10.379 | 15.336 | 64.586 | 0.20x |
| wide_arrays.json | json | 32.505 | 32.876 | 34.691 | 64.586 | 0.06x |
| mixed.json | strata | 0.058 | 0.061 | 0.068 | 61.102 | 1.00x |
| mixed.json | orjson | 0.071 | 0.075 | 0.085 | 61.102 | 0.82x |
| mixed.json | msgspec | 0.103 | 0.106 | 0.124 | 61.102 | 0.58x |
| mixed.json | ujson | 0.424 | 0.427 | 0.486 | 61.102 | 0.14x |
| mixed.json | json | 0.900 | 0.904 | 0.973 | 61.102 | 0.07x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 16.807 | 17.252 | 18.361 | 63.176 | 1.00x |
| users.json | orjson | 24.125 | 24.992 | 26.698 | 63.176 | 0.69x |
| users.json | msgspec | 24.323 | 24.702 | 26.997 | 63.176 | 0.70x |
| users.json | ujson | 36.129 | 36.628 | 41.399 | 63.176 | 0.47x |
| users.json | json | 40.040 | 41.122 | 42.162 | 63.176 | 0.42x |
| flat.json | strata | 1.295 | 1.308 | 1.323 | 65.312 | 1.00x |
| flat.json | orjson | 1.455 | 1.476 | 1.513 | 65.312 | 0.89x |
| flat.json | msgspec | 1.628 | 1.668 | 1.735 | 65.312 | 0.78x |
| flat.json | ujson | 2.735 | 2.783 | 2.869 | 65.312 | 0.47x |
| flat.json | json | 3.128 | 3.169 | 3.244 | 65.312 | 0.41x |
| nested.json | strata | 1.329 | 1.465 | 1.525 | 65.402 | 1.00x |
| nested.json | orjson | 1.700 | 1.733 | 1.834 | 65.402 | 0.85x |
| nested.json | msgspec | 1.761 | 1.889 | 1.911 | 65.402 | 0.78x |
| nested.json | ujson | 2.954 | 3.034 | 3.239 | 65.402 | 0.48x |
| nested.json | json | 3.549 | 3.802 | 3.909 | 65.402 | 0.39x |
| wide_arrays.json | strata | 6.803 | 6.865 | 7.296 | 66.754 | 1.00x |
| wide_arrays.json | orjson | 8.838 | 9.033 | 9.407 | 66.754 | 0.76x |
| wide_arrays.json | msgspec | 9.685 | 9.774 | 10.235 | 66.754 | 0.70x |
| wide_arrays.json | ujson | 12.271 | 12.393 | 13.162 | 66.754 | 0.55x |
| wide_arrays.json | json | 16.025 | 16.155 | 17.457 | 66.754 | 0.42x |
| mixed.json | strata | 0.378 | 0.405 | 0.430 | 61.102 | 1.00x |
| mixed.json | orjson | 0.499 | 0.528 | 0.550 | 61.102 | 0.77x |
| mixed.json | msgspec | 0.516 | 0.558 | 0.607 | 61.102 | 0.73x |
| mixed.json | ujson | 0.651 | 0.716 | 0.733 | 61.102 | 0.57x |
| mixed.json | json | 0.883 | 0.938 | 0.970 | 61.102 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 17.215 | 17.739 | 18.077 | 65.719 | 1.00x |
| users.ndjson | orjson | 25.578 | 25.813 | 27.149 | 65.719 | 0.69x |
| users.ndjson | msgspec | 25.768 | 26.093 | 28.412 | 65.719 | 0.68x |
| users.ndjson | ujson | 36.987 | 37.325 | 38.777 | 65.719 | 0.48x |
| users.ndjson | json | 46.153 | 46.399 | 48.347 | 65.719 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.175 | 3.329 | 3.753 | 63.223 | 1.00x |
| users.json | orjson | 4.204 | 4.515 | 4.917 | 63.223 | 0.74x |
| users.json | msgspec | 5.700 | 5.988 | 6.507 | 63.223 | 0.56x |
| users.json | ujson | 24.505 | 24.991 | 25.283 | 63.223 | 0.13x |
| users.json | json | 40.335 | 41.347 | 42.349 | 63.223 | 0.08x |
| flat.json | strata | 0.622 | 0.656 | 0.742 | 65.312 | 1.00x |
| flat.json | orjson | 0.722 | 0.764 | 0.819 | 65.312 | 0.86x |
| flat.json | msgspec | 0.843 | 0.883 | 1.165 | 65.312 | 0.74x |
| flat.json | ujson | 2.439 | 2.513 | 2.554 | 65.312 | 0.26x |
| flat.json | json | 3.794 | 3.845 | 3.973 | 65.312 | 0.17x |
| nested.json | strata | 0.532 | 0.567 | 0.631 | 65.402 | 1.00x |
| nested.json | orjson | 0.629 | 0.687 | 0.805 | 65.402 | 0.82x |
| nested.json | msgspec | 0.795 | 0.866 | 1.143 | 65.402 | 0.65x |
| nested.json | ujson | 2.385 | 2.571 | 2.821 | 65.402 | 0.22x |
| nested.json | json | 4.722 | 4.742 | 4.887 | 65.402 | 0.12x |
| wide_arrays.json | strata | 2.386 | 2.527 | 2.792 | 66.754 | 1.00x |
| wide_arrays.json | orjson | 3.069 | 3.145 | 3.281 | 66.754 | 0.80x |
| wide_arrays.json | msgspec | 4.028 | 4.170 | 4.363 | 66.754 | 0.61x |
| wide_arrays.json | ujson | 10.787 | 10.892 | 11.441 | 66.754 | 0.23x |
| wide_arrays.json | json | 33.045 | 33.318 | 33.949 | 66.754 | 0.08x |
| mixed.json | strata | 0.320 | 0.348 | 0.422 | 61.102 | 1.00x |
| mixed.json | orjson | 0.346 | 0.364 | 0.498 | 61.102 | 0.96x |
| mixed.json | msgspec | 0.364 | 0.399 | 0.446 | 61.102 | 0.87x |
| mixed.json | ujson | 0.707 | 0.731 | 0.873 | 61.102 | 0.48x |
| mixed.json | json | 1.189 | 1.230 | 1.316 | 61.102 | 0.28x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.127 | 0.146 | 0.213 | 63.285 | 1.00x |
| users.json $[*].id | jmespath | 0.898 | 0.912 | 0.970 | 63.285 | 0.16x |
| users.json $[*].id | jsonpath-ng | 4.857 | 4.990 | 5.784 | 63.285 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.870 | 0.963 | 1.136 | 60.449 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.626 | 6.021 | 6.227 | 60.449 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 32.824 | 34.677 | 35.410 | 60.449 | 0.03x |
| users.json $..total | strata | 3.011 | 3.096 | 3.453 | 60.551 | 1.00x |
| users.json $..total | jsonpath-ng | 651.865 | 656.503 | 669.416 | 60.551 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.677 | 3.698 | 4.292 | 63.355 | 1.00x |
| users.json $[*].id | orjson+jmespath | 25.568 | 26.014 | 28.076 | 63.355 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 28.592 | 30.872 | 34.321 | 63.355 | 0.12x |
| users.json $[*].orders[*].total | strata | 4.096 | 4.144 | 4.368 | 60.512 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 30.236 | 32.306 | 33.151 | 60.512 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 62.774 | 63.855 | 73.760 | 60.512 | 0.06x |
| users.json $..total | strata | 20.879 | 21.298 | 23.076 | 60.574 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 679.360 | 685.149 | 724.677 | 60.574 | 0.03x |

