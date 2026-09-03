# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: af229e1
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
| users.json | strata | 18.124 | 18.948 | 22.338 | 52.641 | 1.00x |
| users.json | orjson | 24.127 | 25.532 | 29.293 | 52.641 | 0.74x |
| users.json | msgspec | 23.410 | 26.432 | 30.774 | 52.641 | 0.72x |
| users.json | ujson | 36.451 | 38.137 | 45.366 | 52.641 | 0.50x |
| users.json | pysimdjson | 153.250 | 157.337 | 164.479 | 52.641 | 0.12x |
| users.json | json | 40.819 | 42.122 | 44.981 | 52.641 | 0.45x |
| flat.json | strata | 1.156 | 1.261 | 1.296 | 58.438 | 1.00x |
| flat.json | orjson | 1.315 | 1.338 | 1.850 | 58.438 | 0.94x |
| flat.json | msgspec | 1.507 | 1.530 | 1.650 | 58.438 | 0.82x |
| flat.json | ujson | 2.600 | 2.636 | 2.673 | 58.438 | 0.48x |
| flat.json | pysimdjson | 13.559 | 13.853 | 14.138 | 58.438 | 0.09x |
| flat.json | json | 2.899 | 3.061 | 3.602 | 58.438 | 0.41x |
| nested.json | strata | 1.396 | 1.419 | 1.499 | 55.703 | 1.00x |
| nested.json | orjson | 1.594 | 1.611 | 2.008 | 55.703 | 0.88x |
| nested.json | msgspec | 1.749 | 1.772 | 1.884 | 55.703 | 0.80x |
| nested.json | ujson | 2.915 | 2.943 | 3.087 | 55.703 | 0.48x |
| nested.json | pysimdjson | 12.657 | 12.718 | 12.771 | 55.703 | 0.11x |
| nested.json | json | 3.730 | 3.767 | 4.064 | 55.703 | 0.38x |
| wide_arrays.json | strata | 8.007 | 8.189 | 8.728 | 59.988 | 1.00x |
| wide_arrays.json | orjson | 8.832 | 9.160 | 9.986 | 59.988 | 0.89x |
| wide_arrays.json | msgspec | 9.635 | 9.714 | 9.921 | 59.988 | 0.84x |
| wide_arrays.json | ujson | 12.422 | 12.582 | 13.217 | 59.988 | 0.65x |
| wide_arrays.json | pysimdjson | 75.020 | 76.320 | 83.465 | 59.988 | 0.11x |
| wide_arrays.json | json | 16.035 | 16.438 | 17.238 | 59.988 | 0.50x |
| mixed.json | strata | 0.364 | 0.369 | 0.395 | 56.996 | 1.00x |
| mixed.json | orjson | 0.422 | 0.429 | 0.674 | 56.996 | 0.86x |
| mixed.json | msgspec | 0.452 | 0.460 | 0.499 | 56.996 | 0.80x |
| mixed.json | ujson | 0.611 | 0.637 | 0.756 | 56.996 | 0.58x |
| mixed.json | pysimdjson | 3.098 | 3.137 | 3.568 | 56.996 | 0.12x |
| mixed.json | json | 0.861 | 0.882 | 1.202 | 56.996 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.369 | 2.761 | 3.488 | 48.535 | 1.00x |
| users.json | orjson | 3.121 | 3.410 | 4.865 | 48.535 | 0.81x |
| users.json | msgspec | 5.686 | 5.960 | 6.378 | 48.535 | 0.46x |
| users.json | ujson | 29.800 | 30.650 | 33.072 | 48.535 | 0.09x |
| users.json | json | 40.317 | 42.415 | 44.222 | 48.535 | 0.07x |
| flat.json | strata | 0.243 | 0.281 | 0.304 | 55.559 | 1.00x |
| flat.json | orjson | 0.358 | 0.372 | 0.438 | 55.559 | 0.75x |
| flat.json | msgspec | 0.485 | 0.497 | 0.544 | 55.559 | 0.56x |
| flat.json | ujson | 2.500 | 2.531 | 2.593 | 55.559 | 0.11x |
| flat.json | json | 3.349 | 3.475 | 3.770 | 55.559 | 0.08x |
| nested.json | strata | 0.223 | 0.227 | 0.264 | 50.609 | 1.00x |
| nested.json | orjson | 0.318 | 0.329 | 0.365 | 50.609 | 0.69x |
| nested.json | msgspec | 0.525 | 0.543 | 0.654 | 50.609 | 0.42x |
| nested.json | ujson | 2.627 | 2.680 | 2.852 | 50.609 | 0.08x |
| nested.json | json | 4.413 | 4.442 | 5.159 | 50.609 | 0.05x |
| wide_arrays.json | strata | 1.547 | 1.762 | 1.801 | 58.508 | 1.00x |
| wide_arrays.json | orjson | 2.127 | 2.333 | 2.385 | 58.508 | 0.76x |
| wide_arrays.json | msgspec | 2.969 | 3.207 | 3.619 | 58.508 | 0.55x |
| wide_arrays.json | ujson | 14.394 | 14.969 | 15.485 | 58.508 | 0.12x |
| wide_arrays.json | json | 32.318 | 33.278 | 36.566 | 58.508 | 0.05x |
| mixed.json | strata | 0.068 | 0.072 | 0.090 | 53.676 | 1.00x |
| mixed.json | orjson | 0.076 | 0.079 | 0.088 | 53.676 | 0.91x |
| mixed.json | msgspec | 0.109 | 0.113 | 0.156 | 53.676 | 0.64x |
| mixed.json | ujson | 0.592 | 0.610 | 0.666 | 53.676 | 0.12x |
| mixed.json | json | 0.942 | 0.974 | 1.349 | 53.676 | 0.07x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 18.529 | 19.072 | 20.585 | 61.066 | 1.00x |
| users.json | orjson | 24.598 | 25.629 | 28.671 | 61.066 | 0.74x |
| users.json | msgspec | 25.121 | 25.711 | 27.256 | 61.066 | 0.74x |
| users.json | ujson | 37.255 | 37.871 | 40.613 | 61.066 | 0.50x |
| users.json | json | 41.901 | 42.838 | 45.051 | 61.066 | 0.45x |
| flat.json | strata | 1.313 | 1.360 | 1.713 | 55.559 | 1.00x |
| flat.json | orjson | 1.414 | 1.491 | 1.561 | 55.559 | 0.91x |
| flat.json | msgspec | 1.664 | 1.696 | 1.754 | 55.559 | 0.80x |
| flat.json | ujson | 2.767 | 2.787 | 2.891 | 55.559 | 0.49x |
| flat.json | json | 3.122 | 3.164 | 3.376 | 55.559 | 0.43x |
| nested.json | strata | 1.396 | 1.571 | 1.967 | 50.609 | 1.00x |
| nested.json | orjson | 1.719 | 1.806 | 2.599 | 50.609 | 0.87x |
| nested.json | msgspec | 1.899 | 1.974 | 2.095 | 50.609 | 0.80x |
| nested.json | ujson | 3.065 | 3.101 | 3.289 | 50.609 | 0.51x |
| nested.json | json | 3.691 | 3.943 | 4.192 | 50.609 | 0.40x |
| wide_arrays.json | strata | 8.345 | 8.667 | 9.052 | 58.508 | 1.00x |
| wide_arrays.json | orjson | 9.075 | 9.359 | 9.662 | 58.508 | 0.93x |
| wide_arrays.json | msgspec | 10.112 | 10.537 | 10.867 | 58.508 | 0.82x |
| wide_arrays.json | ujson | 13.377 | 13.864 | 15.350 | 58.508 | 0.63x |
| wide_arrays.json | json | 16.278 | 17.352 | 17.960 | 58.508 | 0.50x |
| mixed.json | strata | 0.416 | 0.450 | 0.528 | 53.676 | 1.00x |
| mixed.json | orjson | 0.520 | 0.570 | 0.677 | 53.676 | 0.79x |
| mixed.json | msgspec | 0.549 | 0.605 | 0.660 | 53.676 | 0.74x |
| mixed.json | ujson | 0.746 | 0.784 | 0.946 | 53.676 | 0.57x |
| mixed.json | json | 0.960 | 1.013 | 1.298 | 53.676 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 18.881 | 19.571 | 20.191 | 58.777 | 1.00x |
| users.ndjson | orjson | 25.529 | 26.342 | 28.521 | 58.777 | 0.74x |
| users.ndjson | msgspec | 26.206 | 26.950 | 28.469 | 58.777 | 0.73x |
| users.ndjson | ujson | 37.282 | 38.658 | 39.845 | 58.777 | 0.51x |
| users.ndjson | json | 46.820 | 48.449 | 49.462 | 58.777 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.128 | 3.361 | 3.853 | 55.500 | 1.00x |
| users.json | orjson | 4.059 | 4.222 | 5.080 | 55.500 | 0.80x |
| users.json | msgspec | 6.098 | 6.812 | 7.180 | 55.500 | 0.49x |
| users.json | ujson | 31.240 | 31.960 | 32.817 | 55.500 | 0.11x |
| users.json | json | 42.026 | 42.750 | 45.650 | 55.500 | 0.08x |
| flat.json | strata | 0.598 | 0.621 | 0.744 | 55.559 | 1.00x |
| flat.json | orjson | 0.706 | 0.745 | 0.824 | 55.559 | 0.83x |
| flat.json | msgspec | 0.818 | 0.854 | 0.901 | 55.559 | 0.73x |
| flat.json | ujson | 2.877 | 2.938 | 3.073 | 55.559 | 0.21x |
| flat.json | json | 3.853 | 3.945 | 4.497 | 55.559 | 0.16x |
| nested.json | strata | 0.497 | 0.543 | 0.604 | 50.609 | 1.00x |
| nested.json | orjson | 0.588 | 0.658 | 0.734 | 50.609 | 0.83x |
| nested.json | msgspec | 0.843 | 0.868 | 0.949 | 50.609 | 0.63x |
| nested.json | ujson | 2.945 | 3.048 | 3.569 | 50.609 | 0.18x |
| nested.json | json | 4.678 | 4.752 | 5.138 | 50.609 | 0.11x |
| wide_arrays.json | strata | 2.123 | 2.251 | 3.063 | 58.508 | 1.00x |
| wide_arrays.json | orjson | 2.976 | 3.156 | 4.010 | 58.508 | 0.71x |
| wide_arrays.json | msgspec | 3.709 | 3.864 | 4.720 | 58.508 | 0.58x |
| wide_arrays.json | ujson | 15.128 | 15.618 | 16.188 | 58.508 | 0.14x |
| wide_arrays.json | json | 33.150 | 33.586 | 37.419 | 58.508 | 0.07x |
| mixed.json | strata | 0.295 | 0.351 | 0.515 | 53.676 | 1.00x |
| mixed.json | orjson | 0.333 | 0.400 | 0.582 | 53.676 | 0.88x |
| mixed.json | msgspec | 0.373 | 0.418 | 0.544 | 53.676 | 0.84x |
| mixed.json | ujson | 0.878 | 0.899 | 1.267 | 53.676 | 0.39x |
| mixed.json | json | 1.200 | 1.290 | 1.454 | 53.676 | 0.27x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.180 | 0.185 | 0.252 | 55.578 | 1.00x |
| users.json $[*].id | jmespath | 0.947 | 0.988 | 1.066 | 55.578 | 0.19x |
| users.json $[*].id | jsonpath-ng | 5.030 | 5.145 | 5.737 | 55.578 | 0.04x |
| users.json $[*].orders[*].total | strata | 1.018 | 1.140 | 1.313 | 55.434 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 6.004 | 6.107 | 6.576 | 55.434 | 0.19x |
| users.json $[*].orders[*].total | jsonpath-ng | 32.997 | 35.611 | 37.813 | 55.434 | 0.03x |
| users.json $..total | strata | 3.095 | 3.251 | 3.808 | 57.426 | 1.00x |
| users.json $..total | jsonpath-ng | 661.381 | 679.358 | 696.657 | 57.426 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.507 | 3.524 | 3.599 | 59.602 | 1.00x |
| users.json $[*].id | orjson+jmespath | 25.220 | 25.671 | 26.302 | 59.602 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 29.490 | 30.549 | 31.593 | 59.602 | 0.12x |
| users.json $[*].orders[*].total | strata | 3.856 | 3.918 | 4.578 | 56.609 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 30.418 | 30.943 | 33.335 | 56.609 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 63.050 | 63.846 | 71.812 | 56.609 | 0.06x |
| users.json $..total | strata | 22.418 | 23.138 | 23.517 | 56.715 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 690.015 | 714.731 | 726.881 | 56.715 | 0.03x |

