# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c88786c
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
| users.json | strata | 23.116 | 36.622 | 73.652 | 52.613 | 1.00x |
| users.json | orjson | 31.558 | 51.259 | 100.967 | 52.613 | 0.71x |
| users.json | msgspec | 33.400 | 53.580 | 96.006 | 52.613 | 0.68x |
| users.json | ujson | 71.832 | 94.178 | 175.683 | 52.613 | 0.39x |
| users.json | pysimdjson | 224.156 | 345.912 | 562.475 | 52.613 | 0.11x |
| users.json | json | 65.029 | 92.851 | 147.011 | 52.613 | 0.39x |
| flat.json | strata | 1.563 | 1.905 | 2.132 | 62.645 | 1.00x |
| flat.json | orjson | 1.557 | 1.903 | 2.393 | 62.645 | 1.00x |
| flat.json | msgspec | 1.638 | 2.169 | 2.558 | 62.645 | 0.88x |
| flat.json | ujson | 3.277 | 3.703 | 4.277 | 62.645 | 0.51x |
| flat.json | pysimdjson | 16.994 | 18.424 | 18.858 | 62.645 | 0.10x |
| flat.json | json | 3.310 | 4.390 | 4.735 | 62.645 | 0.43x |
| nested.json | strata | 1.554 | 1.599 | 2.218 | 50.988 | 1.00x |
| nested.json | orjson | 1.741 | 1.839 | 2.528 | 50.988 | 0.87x |
| nested.json | msgspec | 1.948 | 2.031 | 2.914 | 50.988 | 0.79x |
| nested.json | ujson | 3.286 | 3.450 | 3.975 | 50.988 | 0.46x |
| nested.json | pysimdjson | 14.159 | 15.693 | 18.680 | 50.988 | 0.10x |
| nested.json | json | 4.161 | 4.284 | 6.271 | 50.988 | 0.37x |
| wide_arrays.json | strata | 8.782 | 9.047 | 10.283 | 62.285 | 1.00x |
| wide_arrays.json | orjson | 9.479 | 10.003 | 11.666 | 62.285 | 0.90x |
| wide_arrays.json | msgspec | 10.083 | 10.592 | 13.093 | 62.285 | 0.85x |
| wide_arrays.json | ujson | 13.147 | 14.155 | 20.785 | 62.285 | 0.64x |
| wide_arrays.json | pysimdjson | 78.310 | 81.059 | 98.564 | 62.285 | 0.11x |
| wide_arrays.json | json | 16.880 | 18.045 | 23.254 | 62.285 | 0.50x |
| mixed.json | strata | 0.390 | 0.408 | 0.597 | 61.047 | 1.00x |
| mixed.json | orjson | 0.462 | 0.525 | 0.585 | 61.047 | 0.78x |
| mixed.json | msgspec | 0.496 | 0.536 | 0.777 | 61.047 | 0.76x |
| mixed.json | ujson | 0.680 | 0.894 | 1.082 | 61.047 | 0.46x |
| mixed.json | pysimdjson | 3.372 | 3.506 | 4.232 | 61.047 | 0.12x |
| mixed.json | json | 0.938 | 1.029 | 1.212 | 61.047 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.588 | 3.860 | 4.766 | 48.305 | 1.00x |
| users.json | orjson | 3.933 | 4.696 | 16.540 | 48.305 | 0.82x |
| users.json | msgspec | 6.706 | 9.652 | 23.690 | 48.305 | 0.40x |
| users.json | ujson | 34.454 | 38.910 | 41.233 | 48.305 | 0.10x |
| users.json | json | 47.326 | 64.161 | 94.694 | 48.305 | 0.06x |
| flat.json | strata | 0.416 | 0.502 | 0.595 | 51.422 | 1.00x |
| flat.json | orjson | 0.508 | 0.668 | 1.206 | 51.422 | 0.75x |
| flat.json | msgspec | 0.732 | 0.864 | 1.009 | 51.422 | 0.58x |
| flat.json | ujson | 3.362 | 3.709 | 4.316 | 51.422 | 0.14x |
| flat.json | json | 5.105 | 5.495 | 6.650 | 51.422 | 0.09x |
| nested.json | strata | 0.241 | 0.255 | 0.318 | 50.727 | 1.00x |
| nested.json | orjson | 0.357 | 0.403 | 0.478 | 50.727 | 0.63x |
| nested.json | msgspec | 0.564 | 0.597 | 1.325 | 50.727 | 0.43x |
| nested.json | ujson | 2.938 | 2.990 | 3.856 | 50.727 | 0.09x |
| nested.json | json | 4.751 | 4.908 | 5.783 | 50.727 | 0.05x |
| wide_arrays.json | strata | 2.096 | 2.312 | 3.222 | 64.152 | 1.00x |
| wide_arrays.json | orjson | 2.549 | 2.744 | 3.958 | 64.152 | 0.84x |
| wide_arrays.json | msgspec | 3.315 | 3.641 | 4.195 | 64.152 | 0.63x |
| wide_arrays.json | ujson | 15.449 | 16.721 | 20.160 | 64.152 | 0.14x |
| wide_arrays.json | json | 35.235 | 39.260 | 44.200 | 64.152 | 0.06x |
| mixed.json | strata | 0.089 | 0.102 | 0.163 | 59.852 | 1.00x |
| mixed.json | orjson | 0.082 | 0.097 | 0.160 | 59.852 | 1.05x |
| mixed.json | msgspec | 0.118 | 0.126 | 0.263 | 59.852 | 0.81x |
| mixed.json | ujson | 0.644 | 0.675 | 0.982 | 59.852 | 0.15x |
| mixed.json | json | 1.001 | 1.042 | 1.223 | 59.852 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 20.105 | 24.831 | 137.006 | 60.621 | 1.00x |
| users.json | orjson | 27.023 | 31.706 | 50.895 | 60.621 | 0.78x |
| users.json | msgspec | 26.719 | 33.032 | 82.646 | 60.621 | 0.75x |
| users.json | ujson | 38.800 | 51.231 | 89.912 | 60.621 | 0.48x |
| users.json | json | 43.385 | 57.297 | 135.303 | 60.621 | 0.43x |
| flat.json | strata | 1.465 | 1.719 | 2.230 | 50.762 | 1.00x |
| flat.json | orjson | 1.605 | 1.918 | 2.594 | 50.762 | 0.90x |
| flat.json | msgspec | 1.853 | 2.179 | 2.793 | 50.762 | 0.79x |
| flat.json | ujson | 3.133 | 3.564 | 4.628 | 50.762 | 0.48x |
| flat.json | json | 3.423 | 3.852 | 5.245 | 50.762 | 0.45x |
| nested.json | strata | 1.651 | 1.689 | 1.965 | 50.727 | 1.00x |
| nested.json | orjson | 1.888 | 1.947 | 2.087 | 50.727 | 0.87x |
| nested.json | msgspec | 2.068 | 2.256 | 3.795 | 50.727 | 0.75x |
| nested.json | ujson | 3.412 | 3.781 | 4.420 | 50.727 | 0.45x |
| nested.json | json | 4.221 | 4.286 | 6.503 | 50.727 | 0.39x |
| wide_arrays.json | strata | 8.701 | 12.902 | 13.510 | 64.152 | 1.00x |
| wide_arrays.json | orjson | 11.552 | 13.704 | 14.855 | 64.152 | 0.94x |
| wide_arrays.json | msgspec | 11.510 | 14.434 | 17.719 | 64.152 | 0.89x |
| wide_arrays.json | ujson | 17.084 | 19.785 | 22.436 | 64.152 | 0.65x |
| wide_arrays.json | json | 21.478 | 26.570 | 28.951 | 64.152 | 0.49x |
| mixed.json | strata | 0.488 | 0.517 | 0.754 | 59.852 | 1.00x |
| mixed.json | orjson | 0.594 | 0.651 | 0.925 | 59.852 | 0.79x |
| mixed.json | msgspec | 0.638 | 0.692 | 0.964 | 59.852 | 0.75x |
| mixed.json | ujson | 0.839 | 1.010 | 1.062 | 59.852 | 0.51x |
| mixed.json | json | 1.091 | 1.220 | 1.568 | 59.852 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 20.646 | 25.524 | 27.567 | 62.984 | 1.00x |
| users.ndjson | orjson | 28.205 | 32.809 | 40.733 | 62.984 | 0.78x |
| users.ndjson | msgspec | 28.710 | 35.053 | 41.866 | 62.984 | 0.73x |
| users.ndjson | ujson | 41.969 | 50.668 | 56.106 | 62.984 | 0.50x |
| users.ndjson | json | 51.283 | 65.963 | 72.857 | 62.984 | 0.39x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 5.983 | 9.842 | 12.655 | 58.691 | 1.00x |
| users.json | orjson | 7.782 | 12.045 | 24.314 | 58.691 | 0.82x |
| users.json | msgspec | 8.823 | 12.559 | 21.739 | 58.691 | 0.78x |
| users.json | ujson | 38.763 | 53.640 | 96.426 | 58.691 | 0.18x |
| users.json | json | 51.861 | 67.162 | 165.907 | 58.691 | 0.15x |
| flat.json | strata | 0.633 | 0.756 | 35.749 | 50.766 | 1.00x |
| flat.json | orjson | 0.768 | 0.886 | 34.211 | 50.766 | 0.85x |
| flat.json | msgspec | 0.913 | 1.033 | 1.236 | 50.766 | 0.73x |
| flat.json | ujson | 3.075 | 3.207 | 4.186 | 50.766 | 0.24x |
| flat.json | json | 4.218 | 4.454 | 49.816 | 50.766 | 0.17x |
| nested.json | strata | 0.618 | 1.057 | 3.299 | 50.727 | 1.00x |
| nested.json | orjson | 0.820 | 0.938 | 1.547 | 50.727 | 1.13x |
| nested.json | msgspec | 0.992 | 1.254 | 2.175 | 50.727 | 0.84x |
| nested.json | ujson | 3.533 | 4.244 | 5.289 | 50.727 | 0.25x |
| nested.json | json | 5.972 | 7.209 | 44.602 | 50.727 | 0.15x |
| wide_arrays.json | strata | 2.961 | 3.458 | 10.766 | 64.152 | 1.00x |
| wide_arrays.json | orjson | 3.575 | 4.445 | 4.953 | 64.152 | 0.78x |
| wide_arrays.json | msgspec | 4.530 | 5.163 | 6.205 | 64.152 | 0.67x |
| wide_arrays.json | ujson | 19.668 | 20.496 | 22.698 | 64.152 | 0.17x |
| wide_arrays.json | json | 40.408 | 44.801 | 47.247 | 64.152 | 0.08x |
| mixed.json | strata | 0.454 | 0.567 | 0.741 | 59.852 | 1.00x |
| mixed.json | orjson | 0.442 | 0.488 | 0.622 | 59.852 | 1.16x |
| mixed.json | msgspec | 0.493 | 0.541 | 0.621 | 59.852 | 1.05x |
| mixed.json | ujson | 1.037 | 1.152 | 1.592 | 59.852 | 0.49x |
| mixed.json | json | 1.392 | 1.474 | 1.644 | 59.852 | 0.38x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.205 | 0.240 | 0.545 | 58.770 | 1.00x |
| users.json $[*].id | jmespath | 1.201 | 1.436 | 1.946 | 58.770 | 0.17x |
| users.json $[*].id | jsonpath-ng | 6.115 | 7.478 | 13.479 | 58.770 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.904 | 1.275 | 2.007 | 55.898 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.830 | 7.372 | 9.780 | 55.898 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 35.915 | 46.086 | 93.637 | 55.898 | 0.03x |
| users.json $..total | strata | 3.604 | 4.249 | 5.815 | 56.961 | 1.00x |
| users.json $..total | jsonpath-ng | 789.927 | 958.649 | 1207.046 | 56.961 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.163 | 4.410 | 26.819 | 58.820 | 1.00x |
| users.json $[*].id | orjson+jmespath | 27.441 | 30.951 | 209.882 | 58.820 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 32.473 | 39.523 | 72.741 | 58.820 | 0.11x |
| users.json $[*].orders[*].total | strata | 4.319 | 5.229 | 6.927 | 56.172 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 31.288 | 36.756 | 51.177 | 56.172 | 0.14x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 64.579 | 88.573 | 174.847 | 56.172 | 0.06x |
| users.json $..total | strata | 26.087 | 28.957 | 35.997 | 56.992 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 854.127 | 880.999 | 1048.859 | 56.992 | 0.03x |

