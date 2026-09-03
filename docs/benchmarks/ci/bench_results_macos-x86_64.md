# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 9b6124a
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
| users.json | strata | 19.075 | 19.461 | 24.069 | 52.816 | 1.00x |
| users.json | orjson | 24.677 | 25.972 | 31.258 | 52.816 | 0.75x |
| users.json | msgspec | 25.016 | 26.170 | 31.295 | 52.816 | 0.74x |
| users.json | ujson | 36.648 | 39.676 | 45.028 | 52.816 | 0.49x |
| users.json | pysimdjson | 161.849 | 165.893 | 188.923 | 52.816 | 0.12x |
| users.json | json | 42.147 | 43.643 | 49.652 | 52.816 | 0.45x |
| flat.json | strata | 1.218 | 1.225 | 1.303 | 59.008 | 1.00x |
| flat.json | orjson | 1.290 | 1.321 | 1.396 | 59.008 | 0.93x |
| flat.json | msgspec | 1.493 | 1.512 | 1.633 | 59.008 | 0.81x |
| flat.json | ujson | 2.615 | 2.637 | 2.844 | 59.008 | 0.46x |
| flat.json | pysimdjson | 14.017 | 14.142 | 14.282 | 59.008 | 0.09x |
| flat.json | json | 3.011 | 3.035 | 3.166 | 59.008 | 0.40x |
| nested.json | strata | 1.377 | 1.422 | 1.536 | 51.613 | 1.00x |
| nested.json | orjson | 1.563 | 1.624 | 1.773 | 51.613 | 0.88x |
| nested.json | msgspec | 1.712 | 1.787 | 1.975 | 51.613 | 0.80x |
| nested.json | ujson | 2.869 | 3.038 | 3.237 | 51.613 | 0.47x |
| nested.json | pysimdjson | 12.832 | 13.156 | 14.665 | 51.613 | 0.11x |
| nested.json | json | 3.683 | 3.830 | 4.193 | 51.613 | 0.37x |
| wide_arrays.json | strata | 8.199 | 8.424 | 11.413 | 58.562 | 1.00x |
| wide_arrays.json | orjson | 8.516 | 8.987 | 10.692 | 58.562 | 0.94x |
| wide_arrays.json | msgspec | 9.607 | 10.231 | 12.466 | 58.562 | 0.82x |
| wide_arrays.json | ujson | 12.374 | 12.699 | 13.364 | 58.562 | 0.66x |
| wide_arrays.json | pysimdjson | 75.916 | 77.586 | 85.463 | 58.562 | 0.11x |
| wide_arrays.json | json | 15.921 | 16.849 | 21.966 | 58.562 | 0.50x |
| mixed.json | strata | 0.355 | 0.364 | 0.408 | 55.902 | 1.00x |
| mixed.json | orjson | 0.419 | 0.434 | 0.461 | 55.902 | 0.84x |
| mixed.json | msgspec | 0.454 | 0.462 | 0.498 | 55.902 | 0.79x |
| mixed.json | ujson | 0.625 | 0.635 | 0.683 | 55.902 | 0.57x |
| mixed.json | pysimdjson | 3.242 | 3.255 | 3.329 | 55.902 | 0.11x |
| mixed.json | json | 0.871 | 0.888 | 0.918 | 55.902 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.333 | 2.376 | 2.864 | 48.148 | 1.00x |
| users.json | orjson | 3.595 | 3.866 | 4.114 | 48.148 | 0.61x |
| users.json | msgspec | 4.877 | 5.077 | 5.368 | 48.148 | 0.47x |
| users.json | ujson | 30.715 | 31.663 | 34.549 | 48.148 | 0.08x |
| users.json | json | 39.899 | 42.108 | 46.418 | 48.148 | 0.06x |
| flat.json | strata | 0.266 | 0.282 | 0.303 | 51.473 | 1.00x |
| flat.json | orjson | 0.359 | 0.372 | 0.657 | 51.473 | 0.76x |
| flat.json | msgspec | 0.485 | 0.509 | 0.551 | 51.473 | 0.56x |
| flat.json | ujson | 2.509 | 2.561 | 2.635 | 51.473 | 0.11x |
| flat.json | json | 3.478 | 3.524 | 3.568 | 51.473 | 0.08x |
| nested.json | strata | 0.226 | 0.231 | 0.248 | 46.727 | 1.00x |
| nested.json | orjson | 0.328 | 0.342 | 0.372 | 46.727 | 0.68x |
| nested.json | msgspec | 0.513 | 0.530 | 0.552 | 46.727 | 0.44x |
| nested.json | ujson | 2.667 | 2.691 | 2.821 | 46.727 | 0.09x |
| nested.json | json | 4.442 | 4.541 | 4.757 | 46.727 | 0.05x |
| wide_arrays.json | strata | 1.997 | 2.283 | 3.637 | 56.969 | 1.00x |
| wide_arrays.json | orjson | 2.640 | 3.296 | 4.453 | 56.969 | 0.69x |
| wide_arrays.json | msgspec | 3.533 | 3.947 | 17.793 | 56.969 | 0.58x |
| wide_arrays.json | ujson | 15.778 | 17.830 | 35.433 | 56.969 | 0.13x |
| wide_arrays.json | json | 37.473 | 38.743 | 88.326 | 56.969 | 0.06x |
| mixed.json | strata | 0.064 | 0.067 | 0.089 | 51.648 | 1.00x |
| mixed.json | orjson | 0.073 | 0.076 | 0.095 | 51.648 | 0.89x |
| mixed.json | msgspec | 0.106 | 0.113 | 0.127 | 51.648 | 0.59x |
| mixed.json | ujson | 0.613 | 0.618 | 0.650 | 51.648 | 0.11x |
| mixed.json | json | 0.957 | 0.969 | 1.075 | 51.648 | 0.07x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 18.618 | 19.086 | 21.086 | 60.863 | 1.00x |
| users.json | orjson | 23.837 | 25.384 | 28.633 | 60.863 | 0.75x |
| users.json | msgspec | 24.735 | 25.725 | 30.703 | 60.863 | 0.74x |
| users.json | ujson | 35.782 | 37.569 | 40.528 | 60.863 | 0.51x |
| users.json | json | 40.692 | 41.335 | 45.681 | 60.863 | 0.46x |
| flat.json | strata | 1.314 | 1.345 | 1.407 | 51.473 | 1.00x |
| flat.json | orjson | 1.435 | 1.485 | 1.553 | 51.473 | 0.91x |
| flat.json | msgspec | 1.619 | 1.658 | 1.747 | 51.473 | 0.81x |
| flat.json | ujson | 2.698 | 2.795 | 3.000 | 51.473 | 0.48x |
| flat.json | json | 3.126 | 3.150 | 3.396 | 51.473 | 0.43x |
| nested.json | strata | 1.473 | 1.517 | 1.906 | 46.879 | 1.00x |
| nested.json | orjson | 1.700 | 1.771 | 1.922 | 46.879 | 0.86x |
| nested.json | msgspec | 1.882 | 1.936 | 2.088 | 46.879 | 0.78x |
| nested.json | ujson | 3.072 | 3.176 | 3.378 | 46.879 | 0.48x |
| nested.json | json | 3.829 | 3.932 | 4.201 | 46.879 | 0.39x |
| wide_arrays.json | strata | 9.161 | 9.411 | 9.867 | 56.969 | 1.00x |
| wide_arrays.json | orjson | 9.551 | 9.970 | 10.612 | 56.969 | 0.94x |
| wide_arrays.json | msgspec | 10.871 | 11.111 | 11.744 | 56.969 | 0.85x |
| wide_arrays.json | ujson | 14.460 | 14.595 | 15.133 | 56.969 | 0.64x |
| wide_arrays.json | json | 17.860 | 18.623 | 19.212 | 56.969 | 0.51x |
| mixed.json | strata | 0.425 | 0.434 | 0.539 | 51.648 | 1.00x |
| mixed.json | orjson | 0.533 | 0.541 | 0.603 | 51.648 | 0.80x |
| mixed.json | msgspec | 0.561 | 0.570 | 0.622 | 51.648 | 0.76x |
| mixed.json | ujson | 0.745 | 0.767 | 0.828 | 51.648 | 0.57x |
| mixed.json | json | 0.972 | 0.991 | 1.045 | 51.648 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 18.947 | 19.945 | 21.974 | 57.980 | 1.00x |
| users.ndjson | orjson | 25.642 | 27.486 | 29.143 | 57.980 | 0.73x |
| users.ndjson | msgspec | 26.210 | 26.941 | 29.400 | 57.980 | 0.74x |
| users.ndjson | ujson | 37.518 | 39.323 | 42.386 | 57.980 | 0.51x |
| users.ndjson | json | 46.887 | 49.480 | 52.723 | 57.980 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.998 | 3.130 | 3.414 | 53.012 | 1.00x |
| users.json | orjson | 4.017 | 4.150 | 4.813 | 53.012 | 0.75x |
| users.json | msgspec | 5.586 | 5.768 | 6.484 | 53.012 | 0.54x |
| users.json | ujson | 31.538 | 31.945 | 36.288 | 53.012 | 0.10x |
| users.json | json | 41.011 | 41.864 | 47.686 | 53.012 | 0.07x |
| flat.json | strata | 0.551 | 0.626 | 0.756 | 51.473 | 1.00x |
| flat.json | orjson | 0.648 | 0.751 | 0.831 | 51.473 | 0.83x |
| flat.json | msgspec | 0.785 | 0.921 | 0.995 | 51.473 | 0.68x |
| flat.json | ujson | 2.770 | 3.053 | 3.497 | 51.473 | 0.21x |
| flat.json | json | 3.851 | 4.137 | 4.632 | 51.473 | 0.15x |
| nested.json | strata | 0.512 | 0.617 | 0.746 | 46.977 | 1.00x |
| nested.json | orjson | 0.627 | 0.685 | 0.750 | 46.977 | 0.90x |
| nested.json | msgspec | 0.803 | 0.878 | 0.929 | 46.977 | 0.70x |
| nested.json | ujson | 3.047 | 3.311 | 3.756 | 46.977 | 0.19x |
| nested.json | json | 4.790 | 5.299 | 5.436 | 46.977 | 0.12x |
| wide_arrays.json | strata | 2.307 | 2.382 | 2.583 | 56.969 | 1.00x |
| wide_arrays.json | orjson | 2.988 | 3.162 | 3.362 | 56.969 | 0.75x |
| wide_arrays.json | msgspec | 3.847 | 3.914 | 4.554 | 56.969 | 0.61x |
| wide_arrays.json | ujson | 16.222 | 16.507 | 17.087 | 56.969 | 0.14x |
| wide_arrays.json | json | 35.246 | 35.935 | 36.731 | 56.969 | 0.07x |
| mixed.json | strata | 0.281 | 0.324 | 0.354 | 51.648 | 1.00x |
| mixed.json | orjson | 0.301 | 0.338 | 0.385 | 51.648 | 0.96x |
| mixed.json | msgspec | 0.320 | 0.389 | 0.588 | 51.648 | 0.83x |
| mixed.json | ujson | 0.844 | 0.909 | 0.995 | 51.648 | 0.36x |
| mixed.json | json | 1.210 | 1.254 | 1.333 | 51.648 | 0.26x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.153 | 0.165 | 0.228 | 53.082 | 1.00x |
| users.json $[*].id | jmespath | 0.957 | 1.004 | 1.084 | 53.082 | 0.16x |
| users.json $[*].id | jsonpath-ng | 5.111 | 5.353 | 5.657 | 53.082 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.854 | 0.901 | 0.940 | 52.777 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 6.021 | 6.191 | 6.637 | 52.777 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 35.516 | 36.468 | 37.442 | 52.777 | 0.02x |
| users.json $..total | strata | 3.161 | 3.380 | 4.032 | 54.867 | 1.00x |
| users.json $..total | jsonpath-ng | 674.283 | 707.219 | 800.442 | 54.867 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.886 | 3.991 | 4.775 | 57.172 | 1.00x |
| users.json $[*].id | orjson+jmespath | 27.487 | 29.349 | 31.046 | 57.172 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 31.875 | 33.869 | 36.398 | 57.172 | 0.12x |
| users.json $[*].orders[*].total | strata | 4.066 | 4.144 | 4.415 | 54.781 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 29.882 | 32.050 | 33.894 | 54.781 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 62.936 | 66.459 | 75.062 | 54.781 | 0.06x |
| users.json $..total | strata | 22.195 | 23.956 | 24.600 | 54.816 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 719.695 | 751.539 | 802.399 | 54.816 | 0.03x |

