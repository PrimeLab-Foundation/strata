# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 9108337
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
| users.json | strata | 18.791 | 19.226 | 22.239 | 52.824 | 1.00x |
| users.json | orjson | 25.216 | 28.302 | 32.410 | 52.824 | 0.68x |
| users.json | msgspec | 25.398 | 28.426 | 30.160 | 52.824 | 0.68x |
| users.json | ujson | 38.633 | 40.792 | 44.667 | 52.824 | 0.47x |
| users.json | pysimdjson | 158.108 | 166.346 | 171.754 | 52.824 | 0.12x |
| users.json | json | 41.567 | 44.407 | 54.213 | 52.824 | 0.43x |
| flat.json | strata | 1.222 | 1.232 | 1.256 | 60.379 | 1.00x |
| flat.json | orjson | 1.312 | 1.333 | 1.392 | 60.379 | 0.92x |
| flat.json | msgspec | 1.487 | 1.518 | 1.585 | 60.379 | 0.81x |
| flat.json | ujson | 2.616 | 2.652 | 3.060 | 60.379 | 0.46x |
| flat.json | pysimdjson | 13.800 | 13.909 | 14.040 | 60.379 | 0.09x |
| flat.json | json | 2.950 | 2.997 | 3.071 | 60.379 | 0.41x |
| nested.json | strata | 1.396 | 1.423 | 1.480 | 57.637 | 1.00x |
| nested.json | orjson | 1.598 | 1.618 | 1.661 | 57.637 | 0.88x |
| nested.json | msgspec | 1.761 | 1.783 | 1.909 | 57.637 | 0.80x |
| nested.json | ujson | 2.867 | 2.949 | 3.029 | 57.637 | 0.48x |
| nested.json | pysimdjson | 12.733 | 12.921 | 13.257 | 57.637 | 0.11x |
| nested.json | json | 3.625 | 3.741 | 4.013 | 57.637 | 0.38x |
| wide_arrays.json | strata | 8.060 | 8.271 | 8.661 | 61.918 | 1.00x |
| wide_arrays.json | orjson | 8.434 | 8.788 | 9.160 | 61.918 | 0.94x |
| wide_arrays.json | msgspec | 9.461 | 9.666 | 9.814 | 61.918 | 0.86x |
| wide_arrays.json | ujson | 12.140 | 12.443 | 12.731 | 61.918 | 0.66x |
| wide_arrays.json | pysimdjson | 74.207 | 75.843 | 77.105 | 61.918 | 0.11x |
| wide_arrays.json | json | 15.807 | 16.265 | 16.603 | 61.918 | 0.51x |
| mixed.json | strata | 0.361 | 0.366 | 0.414 | 58.934 | 1.00x |
| mixed.json | orjson | 0.423 | 0.429 | 0.478 | 58.934 | 0.85x |
| mixed.json | msgspec | 0.449 | 0.450 | 0.460 | 58.934 | 0.81x |
| mixed.json | ujson | 0.613 | 0.620 | 0.641 | 58.934 | 0.59x |
| mixed.json | pysimdjson | 3.078 | 3.109 | 3.415 | 58.934 | 0.12x |
| mixed.json | json | 0.846 | 0.855 | 0.871 | 58.934 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.620 | 3.177 | 41.809 | 48.664 | 1.00x |
| users.json | orjson | 3.329 | 4.296 | 26.330 | 48.664 | 0.74x |
| users.json | msgspec | 6.035 | 7.983 | 17.524 | 48.664 | 0.40x |
| users.json | ujson | 31.705 | 37.161 | 80.309 | 48.664 | 0.09x |
| users.json | json | 42.352 | 48.936 | 158.995 | 48.664 | 0.06x |
| flat.json | strata | 0.292 | 0.302 | 0.313 | 57.500 | 1.00x |
| flat.json | orjson | 0.385 | 0.393 | 0.409 | 57.500 | 0.77x |
| flat.json | msgspec | 0.515 | 0.519 | 0.557 | 57.500 | 0.58x |
| flat.json | ujson | 2.508 | 2.522 | 2.643 | 57.500 | 0.12x |
| flat.json | json | 3.432 | 3.443 | 3.476 | 57.500 | 0.09x |
| nested.json | strata | 0.233 | 0.237 | 0.245 | 52.539 | 1.00x |
| nested.json | orjson | 0.328 | 0.335 | 0.341 | 52.539 | 0.71x |
| nested.json | msgspec | 0.525 | 0.533 | 0.543 | 52.539 | 0.44x |
| nested.json | ujson | 2.610 | 2.620 | 2.666 | 52.539 | 0.09x |
| nested.json | json | 4.296 | 4.353 | 4.504 | 52.539 | 0.05x |
| wide_arrays.json | strata | 1.511 | 1.588 | 1.705 | 60.438 | 1.00x |
| wide_arrays.json | orjson | 2.053 | 2.164 | 2.327 | 60.438 | 0.73x |
| wide_arrays.json | msgspec | 2.898 | 3.038 | 3.354 | 60.438 | 0.52x |
| wide_arrays.json | ujson | 14.197 | 14.346 | 14.662 | 60.438 | 0.11x |
| wide_arrays.json | json | 32.165 | 32.547 | 32.989 | 60.438 | 0.05x |
| mixed.json | strata | 0.068 | 0.071 | 0.079 | 55.613 | 1.00x |
| mixed.json | orjson | 0.080 | 0.083 | 0.099 | 55.613 | 0.85x |
| mixed.json | msgspec | 0.108 | 0.116 | 0.128 | 55.613 | 0.61x |
| mixed.json | ujson | 0.578 | 0.585 | 0.595 | 55.613 | 0.12x |
| mixed.json | json | 0.899 | 0.912 | 0.956 | 55.613 | 0.08x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 18.658 | 19.029 | 21.374 | 61.355 | 1.00x |
| users.json | orjson | 25.033 | 26.066 | 29.903 | 61.355 | 0.73x |
| users.json | msgspec | 25.588 | 26.183 | 29.475 | 61.355 | 0.73x |
| users.json | ujson | 36.624 | 40.115 | 44.922 | 61.355 | 0.47x |
| users.json | json | 41.876 | 43.183 | 46.744 | 61.355 | 0.44x |
| flat.json | strata | 1.334 | 1.353 | 1.406 | 57.500 | 1.00x |
| flat.json | orjson | 1.401 | 1.484 | 1.532 | 57.500 | 0.91x |
| flat.json | msgspec | 1.640 | 1.684 | 1.707 | 57.500 | 0.80x |
| flat.json | ujson | 2.707 | 2.804 | 2.956 | 57.500 | 0.48x |
| flat.json | json | 3.053 | 3.088 | 3.169 | 57.500 | 0.44x |
| nested.json | strata | 1.515 | 1.535 | 1.639 | 52.539 | 1.00x |
| nested.json | orjson | 1.727 | 1.777 | 2.085 | 52.539 | 0.86x |
| nested.json | msgspec | 1.925 | 1.987 | 2.286 | 52.539 | 0.77x |
| nested.json | ujson | 3.131 | 3.169 | 3.289 | 52.539 | 0.48x |
| nested.json | json | 3.850 | 3.991 | 4.309 | 52.539 | 0.38x |
| wide_arrays.json | strata | 8.250 | 8.278 | 8.487 | 60.438 | 1.00x |
| wide_arrays.json | orjson | 8.786 | 8.887 | 9.212 | 60.438 | 0.93x |
| wide_arrays.json | msgspec | 9.642 | 9.848 | 10.027 | 60.438 | 0.84x |
| wide_arrays.json | ujson | 12.913 | 13.207 | 15.253 | 60.438 | 0.63x |
| wide_arrays.json | json | 16.184 | 16.332 | 16.792 | 60.438 | 0.51x |
| mixed.json | strata | 0.448 | 0.453 | 0.490 | 55.613 | 1.00x |
| mixed.json | orjson | 0.558 | 0.565 | 0.584 | 55.613 | 0.80x |
| mixed.json | msgspec | 0.588 | 0.602 | 0.631 | 55.613 | 0.75x |
| mixed.json | ujson | 0.765 | 0.774 | 0.870 | 55.613 | 0.58x |
| mixed.json | json | 0.981 | 0.987 | 1.109 | 55.613 | 0.46x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 19.241 | 19.726 | 78.444 | 60.719 | 1.00x |
| users.ndjson | orjson | 26.428 | 28.160 | 44.875 | 60.719 | 0.70x |
| users.ndjson | msgspec | 26.323 | 28.442 | 29.618 | 60.719 | 0.69x |
| users.ndjson | ujson | 38.741 | 41.093 | 52.421 | 60.719 | 0.48x |
| users.ndjson | json | 47.569 | 48.555 | 60.292 | 60.719 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.291 | 3.504 | 9.641 | 55.742 | 1.00x |
| users.json | orjson | 4.041 | 4.498 | 12.767 | 55.742 | 0.78x |
| users.json | msgspec | 6.492 | 6.790 | 19.329 | 55.742 | 0.52x |
| users.json | ujson | 30.642 | 31.119 | 59.616 | 55.742 | 0.11x |
| users.json | json | 41.096 | 42.160 | 135.770 | 55.742 | 0.08x |
| flat.json | strata | 0.615 | 0.652 | 0.726 | 57.500 | 1.00x |
| flat.json | orjson | 0.714 | 0.812 | 1.863 | 57.500 | 0.80x |
| flat.json | msgspec | 0.852 | 0.897 | 1.894 | 57.500 | 0.73x |
| flat.json | ujson | 2.826 | 2.958 | 4.607 | 57.500 | 0.22x |
| flat.json | json | 3.815 | 3.861 | 4.938 | 57.500 | 0.17x |
| nested.json | strata | 0.503 | 0.568 | 0.628 | 52.539 | 1.00x |
| nested.json | orjson | 0.624 | 0.674 | 1.601 | 52.539 | 0.84x |
| nested.json | msgspec | 0.848 | 0.887 | 1.811 | 52.539 | 0.64x |
| nested.json | ujson | 3.006 | 3.103 | 3.818 | 52.539 | 0.18x |
| nested.json | json | 4.679 | 4.729 | 4.892 | 52.539 | 0.12x |
| wide_arrays.json | strata | 2.061 | 2.167 | 3.940 | 60.438 | 1.00x |
| wide_arrays.json | orjson | 2.920 | 3.039 | 3.484 | 60.438 | 0.71x |
| wide_arrays.json | msgspec | 3.589 | 3.774 | 4.422 | 60.438 | 0.57x |
| wide_arrays.json | ujson | 14.981 | 15.411 | 15.636 | 60.438 | 0.14x |
| wide_arrays.json | json | 32.842 | 33.113 | 33.795 | 60.438 | 0.07x |
| mixed.json | strata | 0.316 | 0.375 | 0.438 | 55.613 | 1.00x |
| mixed.json | orjson | 0.330 | 0.426 | 0.484 | 55.613 | 0.88x |
| mixed.json | msgspec | 0.390 | 0.433 | 0.505 | 55.613 | 0.87x |
| mixed.json | ujson | 0.846 | 0.942 | 1.042 | 55.613 | 0.40x |
| mixed.json | json | 1.215 | 1.242 | 1.316 | 55.613 | 0.30x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.145 | 0.172 | 1.323 | 55.816 | 1.00x |
| users.json $[*].id | jmespath | 0.906 | 0.967 | 2.268 | 55.816 | 0.18x |
| users.json $[*].id | jsonpath-ng | 5.014 | 5.155 | 17.904 | 55.816 | 0.03x |
| users.json $[*].orders[*].total | strata | 1.053 | 1.208 | 1.275 | 55.652 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.648 | 6.452 | 7.791 | 55.652 | 0.19x |
| users.json $[*].orders[*].total | jsonpath-ng | 35.551 | 37.954 | 49.645 | 55.652 | 0.03x |
| users.json $..total | strata | 3.270 | 3.375 | 3.862 | 56.660 | 1.00x |
| users.json $..total | jsonpath-ng | 670.191 | 693.868 | 914.976 | 56.660 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.454 | 3.561 | 10.239 | 59.836 | 1.00x |
| users.json $[*].id | orjson+jmespath | 26.310 | 28.943 | 72.581 | 59.836 | 0.12x |
| users.json $[*].id | orjson+jsonpath-ng | 30.630 | 33.156 | 75.377 | 59.836 | 0.11x |
| users.json $[*].orders[*].total | strata | 3.866 | 3.920 | 4.233 | 56.656 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 30.819 | 32.652 | 35.927 | 56.656 | 0.12x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 63.068 | 71.570 | 73.928 | 56.656 | 0.05x |
| users.json $..total | strata | 21.997 | 22.172 | 26.584 | 56.652 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 690.791 | 707.690 | 835.726 | 56.652 | 0.03x |

