# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 2b2f55a
- python: 3.12.10
- implementation: CPython
- platform: macOS-15.7.9-x86_64-i386-64bit
- machine: x86_64
- processor: Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 17.249 | 18.375 | 21.259 | 52.582 | 1.00x |
| users.json | orjson | 23.946 | 24.352 | 27.911 | 52.582 | 0.75x |
| users.json | msgspec | 24.528 | 25.028 | 27.228 | 52.582 | 0.73x |
| users.json | ujson | 34.119 | 36.193 | 41.200 | 52.582 | 0.51x |
| users.json | pysimdjson | 152.419 | 153.287 | 157.114 | 52.582 | 0.12x |
| users.json | json | 39.272 | 40.781 | 42.614 | 52.582 | 0.45x |
| flat.json | strata | 1.268 | 1.299 | 1.582 | 60.332 | 1.00x |
| flat.json | orjson | 1.293 | 1.386 | 1.476 | 60.332 | 0.94x |
| flat.json | msgspec | 1.502 | 1.578 | 1.749 | 60.332 | 0.82x |
| flat.json | ujson | 2.658 | 2.736 | 2.889 | 60.332 | 0.47x |
| flat.json | pysimdjson | 13.924 | 14.133 | 14.891 | 60.332 | 0.09x |
| flat.json | json | 2.962 | 3.043 | 3.226 | 60.332 | 0.43x |
| nested.json | strata | 1.374 | 1.385 | 1.461 | 52.598 | 1.00x |
| nested.json | orjson | 1.532 | 1.560 | 1.605 | 52.598 | 0.89x |
| nested.json | msgspec | 1.693 | 1.717 | 1.775 | 52.598 | 0.81x |
| nested.json | ujson | 2.863 | 2.878 | 3.180 | 52.598 | 0.48x |
| nested.json | pysimdjson | 12.678 | 12.809 | 12.975 | 52.598 | 0.11x |
| nested.json | json | 3.625 | 3.655 | 3.894 | 52.598 | 0.38x |
| wide_arrays.json | strata | 7.905 | 8.042 | 8.419 | 60.355 | 1.00x |
| wide_arrays.json | orjson | 8.749 | 8.872 | 9.045 | 60.355 | 0.91x |
| wide_arrays.json | msgspec | 9.263 | 9.689 | 10.009 | 60.355 | 0.83x |
| wide_arrays.json | ujson | 12.025 | 12.439 | 13.313 | 60.355 | 0.65x |
| wide_arrays.json | pysimdjson | 73.891 | 74.640 | 75.308 | 60.355 | 0.11x |
| wide_arrays.json | json | 15.011 | 15.987 | 16.631 | 60.355 | 0.50x |
| mixed.json | strata | 0.312 | 0.346 | 0.371 | 54.871 | 1.00x |
| mixed.json | orjson | 0.366 | 0.405 | 0.422 | 54.871 | 0.85x |
| mixed.json | msgspec | 0.432 | 0.434 | 0.497 | 54.871 | 0.80x |
| mixed.json | ujson | 0.541 | 0.599 | 0.818 | 54.871 | 0.58x |
| mixed.json | pysimdjson | 2.810 | 3.032 | 3.054 | 54.871 | 0.11x |
| mixed.json | json | 0.784 | 0.833 | 1.109 | 54.871 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.311 | 2.555 | 2.635 | 47.707 | 1.00x |
| users.json | orjson | 3.853 | 4.089 | 4.683 | 47.707 | 0.62x |
| users.json | msgspec | 4.879 | 4.930 | 5.583 | 47.707 | 0.52x |
| users.json | ujson | 30.404 | 30.691 | 31.231 | 47.707 | 0.08x |
| users.json | json | 38.794 | 39.595 | 40.215 | 47.707 | 0.06x |
| flat.json | strata | 0.264 | 0.277 | 0.308 | 52.621 | 1.00x |
| flat.json | orjson | 0.337 | 0.364 | 0.400 | 52.621 | 0.76x |
| flat.json | msgspec | 0.446 | 0.500 | 0.533 | 52.621 | 0.55x |
| flat.json | ujson | 2.486 | 2.508 | 2.537 | 52.621 | 0.11x |
| flat.json | json | 3.422 | 3.470 | 3.495 | 52.621 | 0.08x |
| nested.json | strata | 0.218 | 0.228 | 0.234 | 48.645 | 1.00x |
| nested.json | orjson | 0.326 | 0.339 | 0.574 | 48.645 | 0.67x |
| nested.json | msgspec | 0.482 | 0.517 | 0.545 | 48.645 | 0.44x |
| nested.json | ujson | 2.609 | 2.655 | 3.099 | 48.645 | 0.09x |
| nested.json | json | 4.332 | 4.399 | 4.569 | 48.645 | 0.05x |
| wide_arrays.json | strata | 1.427 | 1.595 | 2.052 | 56.746 | 1.00x |
| wide_arrays.json | orjson | 2.073 | 2.404 | 2.703 | 56.746 | 0.66x |
| wide_arrays.json | msgspec | 2.805 | 3.035 | 3.263 | 56.746 | 0.53x |
| wide_arrays.json | ujson | 14.251 | 14.698 | 14.926 | 56.746 | 0.11x |
| wide_arrays.json | json | 31.254 | 32.254 | 32.654 | 56.746 | 0.05x |
| mixed.json | strata | 0.054 | 0.060 | 0.066 | 54.766 | 1.00x |
| mixed.json | orjson | 0.061 | 0.072 | 0.081 | 54.766 | 0.82x |
| mixed.json | msgspec | 0.086 | 0.101 | 0.113 | 54.766 | 0.59x |
| mixed.json | ujson | 0.531 | 0.575 | 0.582 | 54.766 | 0.10x |
| mixed.json | json | 0.825 | 0.893 | 0.905 | 54.766 | 0.07x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 18.327 | 18.382 | 19.415 | 60.074 | 1.00x |
| users.json | orjson | 23.394 | 25.002 | 28.054 | 60.074 | 0.74x |
| users.json | msgspec | 24.529 | 25.222 | 29.015 | 60.074 | 0.73x |
| users.json | ujson | 35.992 | 36.568 | 37.811 | 60.074 | 0.50x |
| users.json | json | 40.432 | 40.712 | 42.020 | 60.074 | 0.45x |
| flat.json | strata | 1.283 | 1.312 | 1.520 | 52.621 | 1.00x |
| flat.json | orjson | 1.411 | 1.422 | 1.508 | 52.621 | 0.92x |
| flat.json | msgspec | 1.649 | 1.681 | 1.734 | 52.621 | 0.78x |
| flat.json | ujson | 2.746 | 2.771 | 2.888 | 52.621 | 0.47x |
| flat.json | json | 3.050 | 3.096 | 3.130 | 52.621 | 0.42x |
| nested.json | strata | 1.464 | 1.484 | 1.491 | 48.719 | 1.00x |
| nested.json | orjson | 1.620 | 1.695 | 1.708 | 48.719 | 0.88x |
| nested.json | msgspec | 1.821 | 1.923 | 2.051 | 48.719 | 0.77x |
| nested.json | ujson | 3.028 | 3.049 | 3.402 | 48.719 | 0.49x |
| nested.json | json | 3.784 | 3.798 | 3.836 | 48.719 | 0.39x |
| wide_arrays.json | strata | 8.128 | 8.169 | 8.440 | 57.973 | 1.00x |
| wide_arrays.json | orjson | 8.586 | 8.849 | 9.428 | 57.973 | 0.92x |
| wide_arrays.json | msgspec | 9.290 | 9.943 | 10.570 | 57.973 | 0.82x |
| wide_arrays.json | ujson | 12.688 | 12.856 | 13.730 | 57.973 | 0.64x |
| wide_arrays.json | json | 15.990 | 16.201 | 17.755 | 57.973 | 0.50x |
| mixed.json | strata | 0.408 | 0.413 | 0.417 | 54.766 | 1.00x |
| mixed.json | orjson | 0.500 | 0.508 | 0.576 | 54.766 | 0.81x |
| mixed.json | msgspec | 0.535 | 0.541 | 0.583 | 54.766 | 0.76x |
| mixed.json | ujson | 0.708 | 0.721 | 1.062 | 54.766 | 0.57x |
| mixed.json | json | 0.919 | 0.924 | 0.963 | 54.766 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 18.636 | 19.108 | 31.855 | 59.305 | 1.00x |
| users.ndjson | orjson | 25.175 | 26.691 | 42.368 | 59.305 | 0.72x |
| users.ndjson | msgspec | 25.919 | 27.169 | 35.363 | 59.305 | 0.70x |
| users.ndjson | ujson | 36.874 | 39.540 | 110.347 | 59.305 | 0.48x |
| users.ndjson | json | 46.172 | 48.148 | 85.949 | 59.305 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.060 | 3.388 | 4.023 | 54.652 | 1.00x |
| users.json | orjson | 4.814 | 5.032 | 5.542 | 54.652 | 0.67x |
| users.json | msgspec | 5.827 | 6.125 | 6.507 | 54.652 | 0.55x |
| users.json | ujson | 31.374 | 31.845 | 32.973 | 54.652 | 0.11x |
| users.json | json | 40.593 | 40.968 | 86.674 | 54.652 | 0.08x |
| flat.json | strata | 0.568 | 0.618 | 0.659 | 52.621 | 1.00x |
| flat.json | orjson | 0.686 | 0.705 | 0.751 | 52.621 | 0.88x |
| flat.json | msgspec | 0.791 | 0.845 | 1.118 | 52.621 | 0.73x |
| flat.json | ujson | 2.832 | 2.887 | 3.033 | 52.621 | 0.21x |
| flat.json | json | 3.787 | 3.848 | 3.878 | 52.621 | 0.16x |
| nested.json | strata | 0.441 | 0.515 | 0.536 | 48.719 | 1.00x |
| nested.json | orjson | 0.617 | 0.639 | 0.685 | 48.719 | 0.81x |
| nested.json | msgspec | 0.821 | 0.853 | 0.988 | 48.719 | 0.60x |
| nested.json | ujson | 2.941 | 3.015 | 3.107 | 48.719 | 0.17x |
| nested.json | json | 4.612 | 4.718 | 4.748 | 48.719 | 0.11x |
| wide_arrays.json | strata | 2.069 | 2.124 | 2.539 | 57.977 | 1.00x |
| wide_arrays.json | orjson | 2.673 | 2.800 | 2.918 | 57.977 | 0.76x |
| wide_arrays.json | msgspec | 3.678 | 3.751 | 4.188 | 57.977 | 0.57x |
| wide_arrays.json | ujson | 15.048 | 15.311 | 16.075 | 57.977 | 0.14x |
| wide_arrays.json | json | 32.880 | 33.102 | 33.815 | 57.977 | 0.06x |
| mixed.json | strata | 0.299 | 0.348 | 0.395 | 54.766 | 1.00x |
| mixed.json | orjson | 0.318 | 0.383 | 0.410 | 54.766 | 0.91x |
| mixed.json | msgspec | 0.309 | 0.390 | 0.475 | 54.766 | 0.89x |
| mixed.json | ujson | 0.783 | 0.884 | 0.925 | 54.766 | 0.39x |
| mixed.json | json | 1.051 | 1.228 | 1.321 | 54.766 | 0.28x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.160 | 0.176 | 0.334 | 54.715 | 1.00x |
| users.json $[*].id | jmespath | 0.942 | 0.957 | 1.012 | 54.715 | 0.18x |
| users.json $[*].id | jsonpath-ng | 4.961 | 5.026 | 5.207 | 54.715 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.869 | 1.133 | 1.328 | 52.988 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.675 | 6.008 | 6.433 | 52.988 | 0.19x |
| users.json $[*].orders[*].total | jsonpath-ng | 33.263 | 35.712 | 37.037 | 52.988 | 0.03x |
| users.json $..total | strata | 3.155 | 3.287 | 3.603 | 55.086 | 1.00x |
| users.json $..total | jsonpath-ng | 655.146 | 659.656 | 668.614 | 55.086 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.442 | 3.559 | 3.616 | 58.559 | 1.00x |
| users.json $[*].id | orjson+jmespath | 25.394 | 25.584 | 26.038 | 58.559 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 29.603 | 31.317 | 33.174 | 58.559 | 0.11x |
| users.json $[*].orders[*].total | strata | 3.882 | 3.905 | 4.241 | 54.227 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 30.644 | 31.869 | 35.143 | 54.227 | 0.12x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 63.071 | 67.695 | 73.000 | 54.227 | 0.06x |
| users.json $..total | strata | 21.799 | 22.092 | 22.521 | 54.348 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 680.175 | 683.641 | 689.774 | 54.348 | 0.03x |

