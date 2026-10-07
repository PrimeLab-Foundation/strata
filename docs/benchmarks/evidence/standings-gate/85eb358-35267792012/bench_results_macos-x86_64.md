# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 85eb3583e98587844415f5d32f85a61cbedfcf49
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
| users.json | strata | 16.587 | 16.960 | 20.136 | 57.012 | 1.00x |
| users.json | orjson | 22.968 | 25.612 | 28.367 | 57.012 | 0.66x |
| users.json | msgspec | 23.446 | 25.336 | 27.679 | 57.012 | 0.67x |
| users.json | ujson | 34.836 | 37.059 | 40.052 | 57.012 | 0.46x |
| users.json | pysimdjson | 151.920 | 155.311 | 157.450 | 57.012 | 0.11x |
| users.json | json | 39.766 | 41.295 | 43.945 | 57.012 | 0.41x |
| flat.json | strata | 1.161 | 1.210 | 1.335 | 66.137 | 1.00x |
| flat.json | orjson | 1.306 | 1.328 | 1.353 | 66.137 | 0.91x |
| flat.json | msgspec | 1.459 | 1.489 | 1.638 | 66.137 | 0.81x |
| flat.json | ujson | 2.535 | 2.581 | 2.681 | 66.137 | 0.47x |
| flat.json | pysimdjson | 13.669 | 13.792 | 14.357 | 66.137 | 0.09x |
| flat.json | json | 2.959 | 2.999 | 3.092 | 66.137 | 0.40x |
| nested.json | strata | 1.341 | 1.376 | 1.409 | 65.266 | 1.00x |
| nested.json | orjson | 1.509 | 1.559 | 1.603 | 65.266 | 0.88x |
| nested.json | msgspec | 1.662 | 1.721 | 1.753 | 65.266 | 0.80x |
| nested.json | ujson | 2.775 | 2.830 | 2.899 | 65.266 | 0.49x |
| nested.json | pysimdjson | 12.562 | 12.757 | 13.002 | 65.266 | 0.11x |
| nested.json | json | 3.618 | 3.659 | 3.792 | 65.266 | 0.38x |
| wide_arrays.json | strata | 6.941 | 7.286 | 7.549 | 71.227 | 1.00x |
| wide_arrays.json | orjson | 8.488 | 8.819 | 9.283 | 71.227 | 0.83x |
| wide_arrays.json | msgspec | 9.575 | 9.880 | 10.208 | 71.227 | 0.74x |
| wide_arrays.json | ujson | 11.970 | 12.235 | 12.369 | 71.227 | 0.60x |
| wide_arrays.json | pysimdjson | 74.016 | 74.870 | 77.286 | 71.227 | 0.10x |
| wide_arrays.json | json | 15.749 | 16.084 | 16.631 | 71.227 | 0.45x |
| mixed.json | strata | 0.304 | 0.332 | 0.346 | 64.023 | 1.00x |
| mixed.json | orjson | 0.375 | 0.411 | 0.477 | 64.023 | 0.81x |
| mixed.json | msgspec | 0.396 | 0.430 | 0.447 | 64.023 | 0.77x |
| mixed.json | ujson | 0.543 | 0.608 | 0.628 | 64.023 | 0.55x |
| mixed.json | pysimdjson | 3.014 | 3.037 | 3.316 | 64.023 | 0.11x |
| mixed.json | json | 0.793 | 0.829 | 0.909 | 64.023 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.266 | 2.313 | 2.547 | 52.285 | 1.00x |
| users.json | orjson | 3.092 | 3.128 | 3.319 | 52.285 | 0.74x |
| users.json | msgspec | 4.806 | 4.890 | 5.222 | 52.285 | 0.47x |
| users.json | ujson | 22.844 | 23.269 | 23.902 | 52.285 | 0.10x |
| users.json | json | 38.884 | 39.224 | 40.761 | 52.285 | 0.06x |
| flat.json | strata | 0.300 | 0.306 | 0.314 | 64.648 | 1.00x |
| flat.json | orjson | 0.365 | 0.378 | 0.390 | 64.648 | 0.81x |
| flat.json | msgspec | 0.494 | 0.504 | 0.528 | 64.648 | 0.61x |
| flat.json | ujson | 2.083 | 2.092 | 2.107 | 64.648 | 0.15x |
| flat.json | json | 3.414 | 3.432 | 3.498 | 64.648 | 0.09x |
| nested.json | strata | 0.217 | 0.221 | 0.237 | 65.395 | 1.00x |
| nested.json | orjson | 0.320 | 0.326 | 0.361 | 65.395 | 0.68x |
| nested.json | msgspec | 0.507 | 0.513 | 0.535 | 65.395 | 0.43x |
| nested.json | ujson | 2.158 | 2.171 | 2.207 | 65.395 | 0.10x |
| nested.json | json | 4.310 | 4.336 | 4.639 | 65.395 | 0.05x |
| wide_arrays.json | strata | 1.703 | 1.812 | 2.071 | 64.953 | 1.00x |
| wide_arrays.json | orjson | 2.135 | 2.334 | 2.519 | 64.953 | 0.78x |
| wide_arrays.json | msgspec | 3.215 | 3.338 | 3.663 | 64.953 | 0.54x |
| wide_arrays.json | ujson | 9.849 | 10.355 | 10.637 | 64.953 | 0.17x |
| wide_arrays.json | json | 32.088 | 32.310 | 32.482 | 64.953 | 0.06x |
| mixed.json | strata | 0.058 | 0.060 | 0.075 | 60.824 | 1.00x |
| mixed.json | orjson | 0.071 | 0.075 | 0.087 | 60.824 | 0.80x |
| mixed.json | msgspec | 0.103 | 0.110 | 0.144 | 60.824 | 0.55x |
| mixed.json | ujson | 0.427 | 0.430 | 0.454 | 60.824 | 0.14x |
| mixed.json | json | 0.896 | 0.904 | 0.924 | 60.824 | 0.07x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 16.994 | 17.066 | 17.872 | 63.066 | 1.00x |
| users.json | orjson | 23.050 | 24.116 | 25.621 | 63.066 | 0.71x |
| users.json | msgspec | 23.812 | 24.163 | 24.644 | 63.066 | 0.71x |
| users.json | ujson | 35.730 | 36.896 | 37.832 | 63.066 | 0.46x |
| users.json | json | 40.204 | 40.456 | 40.818 | 63.066 | 0.42x |
| flat.json | strata | 1.169 | 1.293 | 1.531 | 65.305 | 1.00x |
| flat.json | orjson | 1.366 | 1.449 | 1.513 | 65.305 | 0.89x |
| flat.json | msgspec | 1.508 | 1.643 | 2.543 | 65.305 | 0.79x |
| flat.json | ujson | 2.646 | 2.761 | 2.855 | 65.305 | 0.47x |
| flat.json | json | 2.890 | 3.081 | 3.314 | 65.305 | 0.42x |
| nested.json | strata | 1.358 | 1.484 | 1.538 | 65.395 | 1.00x |
| nested.json | orjson | 1.578 | 1.720 | 1.776 | 65.395 | 0.86x |
| nested.json | msgspec | 1.794 | 1.880 | 1.929 | 65.395 | 0.79x |
| nested.json | ujson | 2.807 | 3.000 | 3.120 | 65.395 | 0.49x |
| nested.json | json | 3.658 | 3.793 | 3.964 | 65.395 | 0.39x |
| wide_arrays.json | strata | 7.173 | 7.241 | 8.138 | 66.184 | 1.00x |
| wide_arrays.json | orjson | 8.607 | 8.714 | 9.463 | 66.184 | 0.83x |
| wide_arrays.json | msgspec | 9.649 | 9.696 | 10.139 | 66.184 | 0.75x |
| wide_arrays.json | ujson | 12.264 | 12.389 | 12.902 | 66.184 | 0.58x |
| wide_arrays.json | json | 15.527 | 16.046 | 16.599 | 66.184 | 0.45x |
| mixed.json | strata | 0.358 | 0.403 | 0.463 | 60.824 | 1.00x |
| mixed.json | orjson | 0.470 | 0.530 | 0.586 | 60.824 | 0.76x |
| mixed.json | msgspec | 0.492 | 0.544 | 0.581 | 60.824 | 0.74x |
| mixed.json | ujson | 0.663 | 0.722 | 0.813 | 60.824 | 0.56x |
| mixed.json | json | 0.869 | 0.944 | 1.038 | 60.824 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 17.353 | 17.637 | 17.931 | 65.715 | 1.00x |
| users.ndjson | orjson | 25.193 | 25.994 | 26.685 | 65.715 | 0.68x |
| users.ndjson | msgspec | 25.521 | 26.581 | 27.613 | 65.715 | 0.66x |
| users.ndjson | ujson | 36.649 | 38.077 | 42.292 | 65.715 | 0.46x |
| users.ndjson | json | 45.845 | 47.506 | 48.395 | 65.715 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.949 | 3.052 | 3.154 | 63.117 | 1.00x |
| users.json | orjson | 4.059 | 4.115 | 4.668 | 63.117 | 0.74x |
| users.json | msgspec | 5.659 | 5.819 | 6.305 | 63.117 | 0.52x |
| users.json | ujson | 23.439 | 24.376 | 24.692 | 63.117 | 0.13x |
| users.json | json | 40.007 | 40.356 | 40.582 | 63.117 | 0.08x |
| flat.json | strata | 0.596 | 0.631 | 0.682 | 65.305 | 1.00x |
| flat.json | orjson | 0.667 | 0.732 | 0.819 | 65.305 | 0.86x |
| flat.json | msgspec | 0.820 | 0.871 | 0.944 | 65.305 | 0.72x |
| flat.json | ujson | 2.373 | 2.461 | 2.522 | 65.305 | 0.26x |
| flat.json | json | 3.766 | 3.819 | 3.977 | 65.305 | 0.17x |
| nested.json | strata | 0.500 | 0.521 | 0.549 | 65.395 | 1.00x |
| nested.json | orjson | 0.584 | 0.655 | 0.881 | 65.395 | 0.80x |
| nested.json | msgspec | 0.814 | 0.863 | 0.938 | 65.395 | 0.60x |
| nested.json | ujson | 2.389 | 2.528 | 2.616 | 65.395 | 0.21x |
| nested.json | json | 4.527 | 4.724 | 4.961 | 65.395 | 0.11x |
| wide_arrays.json | strata | 2.280 | 2.709 | 3.079 | 64.953 | 1.00x |
| wide_arrays.json | orjson | 2.987 | 3.286 | 3.499 | 64.953 | 0.82x |
| wide_arrays.json | msgspec | 4.187 | 4.365 | 4.876 | 64.953 | 0.62x |
| wide_arrays.json | ujson | 11.320 | 11.733 | 12.322 | 64.953 | 0.23x |
| wide_arrays.json | json | 33.140 | 33.908 | 77.968 | 64.953 | 0.08x |
| mixed.json | strata | 0.304 | 0.323 | 0.364 | 60.824 | 1.00x |
| mixed.json | orjson | 0.346 | 0.376 | 0.406 | 60.824 | 0.86x |
| mixed.json | msgspec | 0.378 | 0.402 | 0.452 | 60.824 | 0.80x |
| mixed.json | ujson | 0.718 | 0.749 | 0.813 | 60.824 | 0.43x |
| mixed.json | json | 1.175 | 1.215 | 1.539 | 60.824 | 0.27x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.128 | 0.134 | 0.147 | 63.176 | 1.00x |
| users.json $[*].id | jmespath | 0.884 | 0.895 | 0.921 | 63.176 | 0.15x |
| users.json $[*].id | jsonpath-ng | 4.863 | 4.888 | 5.174 | 63.176 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.847 | 0.997 | 1.121 | 60.402 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.557 | 5.677 | 6.434 | 60.402 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 32.728 | 34.139 | 35.076 | 60.402 | 0.03x |
| users.json $..total | strata | 2.872 | 3.078 | 4.120 | 60.480 | 1.00x |
| users.json $..total | jsonpath-ng | 651.921 | 654.794 | 664.786 | 60.480 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.596 | 3.640 | 3.731 | 63.242 | 1.00x |
| users.json $[*].id | orjson+jmespath | 25.017 | 27.053 | 77.081 | 63.242 | 0.13x |
| users.json $[*].id | orjson+jsonpath-ng | 27.328 | 29.792 | 33.709 | 63.242 | 0.12x |
| users.json $[*].orders[*].total | strata | 3.984 | 4.031 | 5.025 | 60.445 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 29.555 | 31.471 | 34.011 | 60.445 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 59.811 | 63.228 | 69.159 | 60.445 | 0.06x |
| users.json $..total | strata | 20.577 | 20.937 | 22.159 | 60.500 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 677.887 | 685.011 | 722.963 | 60.500 | 0.03x |

