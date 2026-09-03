# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 68d6e74
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
| users.json | strata | 22.722 | 23.701 | 30.533 | 52.637 | 1.00x |
| users.json | orjson | 28.357 | 30.982 | 36.186 | 52.637 | 0.76x |
| users.json | msgspec | 28.958 | 30.855 | 35.453 | 52.637 | 0.77x |
| users.json | ujson | 41.641 | 44.591 | 52.983 | 52.637 | 0.53x |
| users.json | pysimdjson | 187.018 | 198.120 | 221.001 | 52.637 | 0.12x |
| users.json | json | 48.313 | 52.849 | 56.846 | 52.637 | 0.45x |
| flat.json | strata | 1.433 | 1.461 | 1.526 | 63.051 | 1.00x |
| flat.json | orjson | 1.445 | 1.481 | 1.505 | 63.051 | 0.99x |
| flat.json | msgspec | 1.685 | 1.698 | 1.730 | 63.051 | 0.86x |
| flat.json | ujson | 2.946 | 2.973 | 3.005 | 63.051 | 0.49x |
| flat.json | pysimdjson | 15.890 | 15.983 | 16.288 | 63.051 | 0.09x |
| flat.json | json | 3.410 | 3.425 | 3.507 | 63.051 | 0.43x |
| nested.json | strata | 1.597 | 1.716 | 1.793 | 57.266 | 1.00x |
| nested.json | orjson | 1.755 | 1.888 | 2.028 | 57.266 | 0.91x |
| nested.json | msgspec | 1.962 | 2.057 | 2.217 | 57.266 | 0.83x |
| nested.json | ujson | 3.222 | 3.452 | 4.127 | 57.266 | 0.50x |
| nested.json | pysimdjson | 14.709 | 15.573 | 16.372 | 57.266 | 0.11x |
| nested.json | json | 4.267 | 4.441 | 4.979 | 57.266 | 0.39x |
| wide_arrays.json | strata | 9.765 | 9.917 | 10.583 | 61.621 | 1.00x |
| wide_arrays.json | orjson | 9.971 | 10.387 | 11.317 | 61.621 | 0.95x |
| wide_arrays.json | msgspec | 11.332 | 11.649 | 12.094 | 61.621 | 0.85x |
| wide_arrays.json | ujson | 14.696 | 15.162 | 15.360 | 61.621 | 0.65x |
| wide_arrays.json | pysimdjson | 90.843 | 91.577 | 92.452 | 61.621 | 0.11x |
| wide_arrays.json | json | 18.907 | 19.500 | 19.869 | 61.621 | 0.51x |
| mixed.json | strata | 0.391 | 0.395 | 0.397 | 61.570 | 1.00x |
| mixed.json | orjson | 0.446 | 0.455 | 0.464 | 61.570 | 0.87x |
| mixed.json | msgspec | 0.470 | 0.481 | 0.496 | 61.570 | 0.82x |
| mixed.json | ujson | 0.653 | 0.668 | 0.682 | 61.570 | 0.59x |
| mixed.json | pysimdjson | 3.386 | 3.429 | 3.545 | 61.570 | 0.12x |
| mixed.json | json | 0.930 | 0.936 | 1.075 | 61.570 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.848 | 2.899 | 3.420 | 49.207 | 1.00x |
| users.json | orjson | 3.621 | 3.982 | 4.740 | 49.207 | 0.73x |
| users.json | msgspec | 6.228 | 6.497 | 7.119 | 49.207 | 0.45x |
| users.json | ujson | 36.526 | 36.976 | 37.516 | 49.207 | 0.08x |
| users.json | json | 47.925 | 48.519 | 49.220 | 49.207 | 0.06x |
| flat.json | strata | 0.292 | 0.301 | 0.315 | 58.770 | 1.00x |
| flat.json | orjson | 0.407 | 0.415 | 0.443 | 58.770 | 0.73x |
| flat.json | msgspec | 0.544 | 0.577 | 0.660 | 58.770 | 0.52x |
| flat.json | ujson | 2.843 | 2.862 | 2.941 | 58.770 | 0.11x |
| flat.json | json | 3.934 | 3.960 | 4.248 | 58.770 | 0.08x |
| nested.json | strata | 0.281 | 0.314 | 0.359 | 51.680 | 1.00x |
| nested.json | orjson | 0.421 | 0.446 | 0.474 | 51.680 | 0.70x |
| nested.json | msgspec | 0.655 | 0.685 | 0.879 | 51.680 | 0.46x |
| nested.json | ujson | 3.279 | 3.407 | 4.174 | 51.680 | 0.09x |
| nested.json | json | 5.337 | 5.716 | 6.083 | 51.680 | 0.05x |
| wide_arrays.json | strata | 2.002 | 2.134 | 2.406 | 64.652 | 1.00x |
| wide_arrays.json | orjson | 2.611 | 2.715 | 2.921 | 64.652 | 0.79x |
| wide_arrays.json | msgspec | 3.579 | 3.715 | 3.972 | 64.652 | 0.57x |
| wide_arrays.json | ujson | 17.092 | 17.639 | 18.235 | 64.652 | 0.12x |
| wide_arrays.json | json | 37.953 | 39.265 | 41.631 | 64.652 | 0.05x |
| mixed.json | strata | 0.076 | 0.079 | 0.092 | 57.312 | 1.00x |
| mixed.json | orjson | 0.073 | 0.076 | 0.082 | 57.312 | 1.03x |
| mixed.json | msgspec | 0.107 | 0.111 | 0.125 | 57.312 | 0.71x |
| mixed.json | ujson | 0.633 | 0.642 | 0.656 | 57.312 | 0.12x |
| mixed.json | json | 0.982 | 1.006 | 1.053 | 57.312 | 0.08x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 21.825 | 22.587 | 24.083 | 61.562 | 1.00x |
| users.json | orjson | 26.971 | 28.532 | 32.144 | 61.562 | 0.79x |
| users.json | msgspec | 27.418 | 28.976 | 29.912 | 61.562 | 0.78x |
| users.json | ujson | 40.834 | 42.805 | 45.490 | 61.562 | 0.53x |
| users.json | json | 46.408 | 47.807 | 48.371 | 61.562 | 0.47x |
| flat.json | strata | 1.556 | 1.577 | 1.688 | 58.770 | 1.00x |
| flat.json | orjson | 1.598 | 1.629 | 1.715 | 58.770 | 0.97x |
| flat.json | msgspec | 1.798 | 1.858 | 1.904 | 58.770 | 0.85x |
| flat.json | ujson | 3.116 | 3.131 | 3.201 | 58.770 | 0.50x |
| flat.json | json | 3.519 | 3.556 | 3.876 | 58.770 | 0.44x |
| nested.json | strata | 1.718 | 1.786 | 1.897 | 51.172 | 1.00x |
| nested.json | orjson | 1.931 | 2.000 | 2.085 | 51.172 | 0.89x |
| nested.json | msgspec | 2.160 | 2.194 | 2.287 | 51.172 | 0.81x |
| nested.json | ujson | 3.538 | 3.608 | 3.746 | 51.172 | 0.49x |
| nested.json | json | 4.372 | 4.491 | 4.887 | 51.172 | 0.40x |
| wide_arrays.json | strata | 9.476 | 9.612 | 9.704 | 64.652 | 1.00x |
| wide_arrays.json | orjson | 9.861 | 10.155 | 10.585 | 64.652 | 0.95x |
| wide_arrays.json | msgspec | 10.988 | 11.282 | 11.546 | 64.652 | 0.85x |
| wide_arrays.json | ujson | 14.390 | 14.871 | 15.164 | 64.652 | 0.65x |
| wide_arrays.json | json | 18.429 | 18.707 | 18.927 | 64.652 | 0.51x |
| mixed.json | strata | 0.433 | 0.458 | 0.488 | 57.312 | 1.00x |
| mixed.json | orjson | 0.511 | 0.558 | 0.593 | 57.312 | 0.82x |
| mixed.json | msgspec | 0.548 | 0.598 | 0.848 | 57.312 | 0.77x |
| mixed.json | ujson | 0.747 | 0.777 | 0.801 | 57.312 | 0.59x |
| mixed.json | json | 0.992 | 1.003 | 1.009 | 57.312 | 0.46x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 21.342 | 22.491 | 26.122 | 62.094 | 1.00x |
| users.ndjson | orjson | 27.803 | 30.288 | 34.248 | 62.094 | 0.74x |
| users.ndjson | msgspec | 28.467 | 29.955 | 31.299 | 62.094 | 0.75x |
| users.ndjson | ujson | 40.774 | 42.877 | 50.170 | 62.094 | 0.52x |
| users.ndjson | json | 51.104 | 52.900 | 60.906 | 62.094 | 0.43x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.680 | 3.842 | 3.966 | 57.680 | 1.00x |
| users.json | orjson | 4.555 | 4.683 | 4.818 | 57.680 | 0.82x |
| users.json | msgspec | 6.829 | 7.230 | 7.547 | 57.680 | 0.53x |
| users.json | ujson | 35.818 | 36.679 | 41.413 | 57.680 | 0.10x |
| users.json | json | 46.756 | 49.678 | 53.965 | 57.680 | 0.08x |
| flat.json | strata | 0.619 | 0.677 | 0.994 | 58.770 | 1.00x |
| flat.json | orjson | 0.720 | 0.816 | 0.883 | 58.770 | 0.83x |
| flat.json | msgspec | 0.907 | 0.959 | 0.999 | 58.770 | 0.71x |
| flat.json | ujson | 3.134 | 3.289 | 5.484 | 58.770 | 0.21x |
| flat.json | json | 4.317 | 4.411 | 4.500 | 58.770 | 0.15x |
| nested.json | strata | 0.580 | 0.614 | 0.763 | 51.172 | 1.00x |
| nested.json | orjson | 0.713 | 0.768 | 0.978 | 51.172 | 0.80x |
| nested.json | msgspec | 0.900 | 0.996 | 1.108 | 51.172 | 0.62x |
| nested.json | ujson | 3.477 | 3.532 | 3.739 | 51.172 | 0.17x |
| nested.json | json | 5.599 | 5.686 | 5.974 | 51.172 | 0.11x |
| wide_arrays.json | strata | 2.605 | 2.767 | 3.144 | 64.652 | 1.00x |
| wide_arrays.json | orjson | 3.113 | 3.268 | 3.359 | 64.652 | 0.85x |
| wide_arrays.json | msgspec | 4.020 | 4.170 | 4.371 | 64.652 | 0.66x |
| wide_arrays.json | ujson | 16.973 | 17.356 | 19.011 | 64.652 | 0.16x |
| wide_arrays.json | json | 37.182 | 37.979 | 41.326 | 64.652 | 0.07x |
| mixed.json | strata | 0.326 | 0.340 | 0.373 | 57.312 | 1.00x |
| mixed.json | orjson | 0.341 | 0.376 | 0.687 | 57.312 | 0.90x |
| mixed.json | msgspec | 0.375 | 0.390 | 0.476 | 57.312 | 0.87x |
| mixed.json | ujson | 0.910 | 0.943 | 1.094 | 57.312 | 0.36x |
| mixed.json | json | 1.264 | 1.311 | 1.423 | 57.312 | 0.26x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.153 | 0.165 | 0.304 | 56.352 | 1.00x |
| users.json $[*].id | jmespath | 1.047 | 1.070 | 1.227 | 56.352 | 0.15x |
| users.json $[*].id | jsonpath-ng | 5.758 | 5.876 | 6.492 | 56.352 | 0.03x |
| users.json $[*].orders[*].total | strata | 1.005 | 1.046 | 1.195 | 55.914 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 6.760 | 6.859 | 7.326 | 55.914 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 39.750 | 40.488 | 44.146 | 55.914 | 0.03x |
| users.json $..total | strata | 3.572 | 3.752 | 4.057 | 58.008 | 1.00x |
| users.json $..total | jsonpath-ng | 766.881 | 780.373 | 823.407 | 58.008 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.623 | 4.706 | 4.997 | 57.395 | 1.00x |
| users.json $[*].id | orjson+jmespath | 28.953 | 29.421 | 62.805 | 57.395 | 0.16x |
| users.json $[*].id | orjson+jsonpath-ng | 34.121 | 35.245 | 38.657 | 57.395 | 0.13x |
| users.json $[*].orders[*].total | strata | 5.167 | 5.196 | 5.301 | 57.199 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 35.153 | 36.584 | 38.098 | 57.199 | 0.14x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 73.296 | 74.601 | 82.365 | 57.199 | 0.07x |
| users.json $..total | strata | 26.233 | 29.172 | 33.123 | 57.285 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 795.201 | 896.126 | 1107.299 | 57.285 | 0.03x |

