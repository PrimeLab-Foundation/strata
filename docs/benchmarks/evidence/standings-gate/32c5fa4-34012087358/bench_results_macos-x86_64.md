# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 32c5fa4
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
| users.json | strata | 17.442 | 18.022 | 22.789 | 52.824 | 1.00x |
| users.json | orjson | 25.051 | 25.849 | 30.276 | 52.824 | 0.70x |
| users.json | msgspec | 24.988 | 25.946 | 32.032 | 52.824 | 0.69x |
| users.json | ujson | 36.607 | 38.045 | 44.807 | 52.824 | 0.47x |
| users.json | pysimdjson | 159.973 | 162.487 | 180.758 | 52.824 | 0.11x |
| users.json | json | 41.109 | 43.169 | 47.150 | 52.824 | 0.42x |
| flat.json | strata | 1.231 | 1.276 | 1.352 | 62.477 | 1.00x |
| flat.json | orjson | 1.385 | 1.420 | 1.797 | 62.477 | 0.90x |
| flat.json | msgspec | 1.568 | 1.581 | 1.656 | 62.477 | 0.81x |
| flat.json | ujson | 2.729 | 2.772 | 3.107 | 62.477 | 0.46x |
| flat.json | pysimdjson | 14.774 | 14.883 | 15.396 | 62.477 | 0.09x |
| flat.json | json | 3.167 | 3.187 | 3.250 | 62.477 | 0.40x |
| nested.json | strata | 1.434 | 1.523 | 1.720 | 59.734 | 1.00x |
| nested.json | orjson | 1.662 | 1.771 | 5.761 | 59.734 | 0.86x |
| nested.json | msgspec | 1.833 | 1.927 | 2.003 | 59.734 | 0.79x |
| nested.json | ujson | 3.049 | 3.159 | 3.541 | 59.734 | 0.48x |
| nested.json | pysimdjson | 13.572 | 13.878 | 14.473 | 59.734 | 0.11x |
| nested.json | json | 3.877 | 4.119 | 4.582 | 59.734 | 0.37x |
| wide_arrays.json | strata | 7.451 | 7.647 | 7.999 | 64.027 | 1.00x |
| wide_arrays.json | orjson | 9.492 | 9.722 | 10.417 | 64.027 | 0.79x |
| wide_arrays.json | msgspec | 10.378 | 10.633 | 11.221 | 64.027 | 0.72x |
| wide_arrays.json | ujson | 13.146 | 13.471 | 14.279 | 64.027 | 0.57x |
| wide_arrays.json | pysimdjson | 82.071 | 84.068 | 86.059 | 64.027 | 0.09x |
| wide_arrays.json | json | 17.503 | 17.984 | 18.657 | 64.027 | 0.43x |
| mixed.json | strata | 0.360 | 0.396 | 0.411 | 61.047 | 1.00x |
| mixed.json | orjson | 0.439 | 0.474 | 0.603 | 61.047 | 0.83x |
| mixed.json | msgspec | 0.463 | 0.510 | 0.563 | 61.047 | 0.78x |
| mixed.json | ujson | 0.639 | 0.692 | 0.720 | 61.047 | 0.57x |
| mixed.json | pysimdjson | 3.212 | 3.505 | 3.799 | 61.047 | 0.11x |
| mixed.json | json | 0.886 | 1.002 | 1.152 | 61.047 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.377 | 2.440 | 2.998 | 48.629 | 1.00x |
| users.json | orjson | 3.151 | 3.254 | 3.460 | 48.629 | 0.75x |
| users.json | msgspec | 5.669 | 5.803 | 6.355 | 48.629 | 0.42x |
| users.json | ujson | 23.475 | 24.341 | 28.079 | 48.629 | 0.10x |
| users.json | json | 40.909 | 41.522 | 48.124 | 48.629 | 0.06x |
| flat.json | strata | 0.303 | 0.313 | 0.328 | 59.598 | 1.00x |
| flat.json | orjson | 0.392 | 0.406 | 0.429 | 59.598 | 0.77x |
| flat.json | msgspec | 0.529 | 0.536 | 0.616 | 59.598 | 0.58x |
| flat.json | ujson | 2.205 | 2.215 | 2.279 | 59.598 | 0.14x |
| flat.json | json | 3.647 | 3.687 | 3.951 | 59.598 | 0.08x |
| nested.json | strata | 0.257 | 0.274 | 0.335 | 54.645 | 1.00x |
| nested.json | orjson | 0.361 | 0.369 | 0.399 | 54.645 | 0.74x |
| nested.json | msgspec | 0.581 | 0.588 | 0.647 | 54.645 | 0.47x |
| nested.json | ujson | 2.400 | 2.484 | 2.539 | 54.645 | 0.11x |
| nested.json | json | 4.791 | 4.969 | 5.518 | 54.645 | 0.06x |
| wide_arrays.json | strata | 1.696 | 1.731 | 1.922 | 62.547 | 1.00x |
| wide_arrays.json | orjson | 2.237 | 2.305 | 2.503 | 62.547 | 0.75x |
| wide_arrays.json | msgspec | 3.146 | 3.241 | 3.654 | 62.547 | 0.53x |
| wide_arrays.json | ujson | 10.281 | 10.497 | 11.575 | 62.547 | 0.16x |
| wide_arrays.json | json | 34.466 | 34.900 | 35.775 | 62.547 | 0.05x |
| mixed.json | strata | 0.065 | 0.076 | 0.092 | 57.727 | 1.00x |
| mixed.json | orjson | 0.077 | 0.088 | 0.107 | 57.727 | 0.86x |
| mixed.json | msgspec | 0.106 | 0.121 | 0.168 | 57.727 | 0.63x |
| mixed.json | ujson | 0.445 | 0.491 | 0.558 | 57.727 | 0.16x |
| mixed.json | json | 0.952 | 1.074 | 1.222 | 57.727 | 0.07x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 17.733 | 18.412 | 20.774 | 61.324 | 1.00x |
| users.json | orjson | 25.061 | 25.777 | 31.286 | 61.324 | 0.71x |
| users.json | msgspec | 25.445 | 27.036 | 33.658 | 61.324 | 0.68x |
| users.json | ujson | 36.901 | 38.956 | 44.599 | 61.324 | 0.47x |
| users.json | json | 41.319 | 43.950 | 51.871 | 61.324 | 0.42x |
| flat.json | strata | 1.349 | 1.474 | 1.531 | 59.598 | 1.00x |
| flat.json | orjson | 1.590 | 1.688 | 1.765 | 59.598 | 0.87x |
| flat.json | msgspec | 1.812 | 1.867 | 1.972 | 59.598 | 0.79x |
| flat.json | ujson | 3.049 | 3.245 | 3.317 | 59.598 | 0.45x |
| flat.json | json | 3.427 | 3.660 | 3.980 | 59.598 | 0.40x |
| nested.json | strata | 1.624 | 1.640 | 1.694 | 54.645 | 1.00x |
| nested.json | orjson | 1.906 | 1.930 | 2.094 | 54.645 | 0.85x |
| nested.json | msgspec | 2.076 | 2.117 | 2.167 | 54.645 | 0.78x |
| nested.json | ujson | 3.359 | 3.405 | 3.495 | 54.645 | 0.48x |
| nested.json | json | 4.232 | 4.310 | 4.488 | 54.645 | 0.38x |
| wide_arrays.json | strata | 7.461 | 7.593 | 8.089 | 62.547 | 1.00x |
| wide_arrays.json | orjson | 9.334 | 9.598 | 10.189 | 62.547 | 0.79x |
| wide_arrays.json | msgspec | 10.268 | 10.526 | 11.073 | 62.547 | 0.72x |
| wide_arrays.json | ujson | 13.348 | 13.750 | 14.506 | 62.547 | 0.55x |
| wide_arrays.json | json | 17.263 | 17.533 | 18.034 | 62.547 | 0.43x |
| mixed.json | strata | 0.456 | 0.477 | 0.675 | 57.727 | 1.00x |
| mixed.json | orjson | 0.593 | 0.615 | 0.650 | 57.727 | 0.78x |
| mixed.json | msgspec | 0.625 | 0.643 | 0.682 | 57.727 | 0.74x |
| mixed.json | ujson | 0.816 | 0.851 | 0.883 | 57.727 | 0.56x |
| mixed.json | json | 1.089 | 1.099 | 1.344 | 57.727 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 18.776 | 19.098 | 21.053 | 62.816 | 1.00x |
| users.ndjson | orjson | 27.370 | 28.107 | 30.411 | 62.816 | 0.68x |
| users.ndjson | msgspec | 27.505 | 28.070 | 30.636 | 62.816 | 0.68x |
| users.ndjson | ujson | 39.536 | 40.429 | 43.576 | 62.816 | 0.47x |
| users.ndjson | json | 49.262 | 50.536 | 54.881 | 62.816 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.512 | 3.688 | 4.233 | 54.758 | 1.00x |
| users.json | orjson | 4.498 | 4.840 | 5.577 | 54.758 | 0.76x |
| users.json | msgspec | 6.970 | 7.419 | 8.230 | 54.758 | 0.50x |
| users.json | ujson | 27.818 | 28.140 | 35.252 | 54.758 | 0.13x |
| users.json | json | 47.852 | 48.128 | 49.346 | 54.758 | 0.08x |
| flat.json | strata | 0.707 | 0.727 | 0.815 | 59.598 | 1.00x |
| flat.json | orjson | 0.817 | 0.841 | 0.944 | 59.598 | 0.86x |
| flat.json | msgspec | 0.962 | 1.008 | 1.117 | 59.598 | 0.72x |
| flat.json | ujson | 2.706 | 2.734 | 3.127 | 59.598 | 0.27x |
| flat.json | json | 4.166 | 4.252 | 4.377 | 59.598 | 0.17x |
| nested.json | strata | 0.546 | 0.594 | 0.712 | 54.645 | 1.00x |
| nested.json | orjson | 0.653 | 0.731 | 0.768 | 54.645 | 0.81x |
| nested.json | msgspec | 0.894 | 0.984 | 1.052 | 54.645 | 0.60x |
| nested.json | ujson | 2.807 | 2.920 | 2.982 | 54.645 | 0.20x |
| nested.json | json | 5.255 | 5.467 | 5.724 | 54.645 | 0.11x |
| wide_arrays.json | strata | 2.617 | 2.689 | 2.916 | 62.547 | 1.00x |
| wide_arrays.json | orjson | 3.394 | 3.467 | 3.610 | 62.547 | 0.78x |
| wide_arrays.json | msgspec | 4.079 | 4.372 | 4.471 | 62.547 | 0.62x |
| wide_arrays.json | ujson | 11.311 | 12.498 | 13.066 | 62.547 | 0.22x |
| wide_arrays.json | json | 39.380 | 40.341 | 41.220 | 62.547 | 0.07x |
| mixed.json | strata | 0.319 | 0.345 | 0.453 | 57.727 | 1.00x |
| mixed.json | orjson | 0.341 | 0.394 | 0.475 | 57.727 | 0.88x |
| mixed.json | msgspec | 0.349 | 0.413 | 0.533 | 57.727 | 0.84x |
| mixed.json | ujson | 0.740 | 0.788 | 0.943 | 57.727 | 0.44x |
| mixed.json | json | 1.232 | 1.292 | 1.485 | 57.727 | 0.27x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.146 | 0.157 | 0.197 | 54.824 | 1.00x |
| users.json $[*].id | jmespath | 0.984 | 1.021 | 1.089 | 54.824 | 0.15x |
| users.json $[*].id | jsonpath-ng | 5.494 | 5.539 | 5.686 | 54.824 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.849 | 0.887 | 0.923 | 54.656 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.630 | 5.984 | 6.625 | 54.656 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 34.066 | 34.968 | 37.388 | 54.656 | 0.03x |
| users.json $..total | strata | 3.209 | 3.504 | 4.228 | 56.648 | 1.00x |
| users.json $..total | jsonpath-ng | 708.520 | 751.324 | 789.313 | 56.648 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.847 | 3.868 | 3.940 | 58.855 | 1.00x |
| users.json $[*].id | orjson+jmespath | 26.929 | 27.521 | 28.893 | 58.855 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 31.136 | 32.017 | 33.909 | 58.855 | 0.12x |
| users.json $[*].orders[*].total | strata | 4.052 | 4.153 | 4.277 | 56.578 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 31.010 | 33.170 | 37.993 | 56.578 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 64.076 | 70.069 | 74.504 | 56.578 | 0.06x |
| users.json $..total | strata | 22.510 | 23.422 | 24.537 | 56.691 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 756.295 | 783.858 | 810.177 | 56.691 | 0.03x |

