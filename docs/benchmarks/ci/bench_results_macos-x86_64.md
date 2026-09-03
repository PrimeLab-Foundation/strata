# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 571381e
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
| users.json | strata | 17.963 | 18.896 | 22.865 | 52.652 | 1.00x |
| users.json | orjson | 24.085 | 25.371 | 31.868 | 52.652 | 0.74x |
| users.json | msgspec | 23.261 | 25.796 | 30.868 | 52.652 | 0.73x |
| users.json | ujson | 35.078 | 37.346 | 45.348 | 52.652 | 0.51x |
| users.json | pysimdjson | 152.974 | 162.678 | 173.087 | 52.652 | 0.12x |
| users.json | json | 39.954 | 43.011 | 49.597 | 52.652 | 0.44x |
| flat.json | strata | 1.163 | 1.243 | 1.293 | 61.117 | 1.00x |
| flat.json | orjson | 1.301 | 1.340 | 1.393 | 61.117 | 0.93x |
| flat.json | msgspec | 1.454 | 1.531 | 1.588 | 61.117 | 0.81x |
| flat.json | ujson | 2.542 | 2.699 | 2.801 | 61.117 | 0.46x |
| flat.json | pysimdjson | 13.992 | 14.733 | 14.908 | 61.117 | 0.08x |
| flat.json | json | 2.947 | 3.121 | 3.183 | 61.117 | 0.40x |
| nested.json | strata | 1.451 | 1.478 | 1.824 | 58.383 | 1.00x |
| nested.json | orjson | 1.602 | 1.663 | 1.720 | 58.383 | 0.89x |
| nested.json | msgspec | 1.780 | 1.825 | 2.046 | 58.383 | 0.81x |
| nested.json | ujson | 2.946 | 3.080 | 3.198 | 58.383 | 0.48x |
| nested.json | pysimdjson | 13.037 | 13.835 | 14.157 | 58.383 | 0.11x |
| nested.json | json | 3.855 | 3.912 | 3.970 | 58.383 | 0.38x |
| wide_arrays.json | strata | 7.930 | 8.318 | 8.598 | 62.672 | 1.00x |
| wide_arrays.json | orjson | 8.704 | 9.084 | 15.596 | 62.672 | 0.92x |
| wide_arrays.json | msgspec | 9.442 | 9.792 | 10.168 | 62.672 | 0.85x |
| wide_arrays.json | ujson | 12.200 | 12.487 | 13.379 | 62.672 | 0.67x |
| wide_arrays.json | pysimdjson | 74.666 | 75.223 | 79.945 | 62.672 | 0.11x |
| wide_arrays.json | json | 15.728 | 15.935 | 17.518 | 62.672 | 0.52x |
| mixed.json | strata | 0.335 | 0.346 | 0.366 | 59.684 | 1.00x |
| mixed.json | orjson | 0.404 | 0.415 | 0.461 | 59.684 | 0.83x |
| mixed.json | msgspec | 0.425 | 0.439 | 0.457 | 59.684 | 0.79x |
| mixed.json | ujson | 0.585 | 0.614 | 0.675 | 59.684 | 0.56x |
| mixed.json | pysimdjson | 3.015 | 3.141 | 3.298 | 59.684 | 0.11x |
| mixed.json | json | 0.824 | 0.860 | 0.979 | 59.684 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.476 | 2.559 | 2.671 | 48.477 | 1.00x |
| users.json | orjson | 3.062 | 3.146 | 3.803 | 48.477 | 0.81x |
| users.json | msgspec | 5.503 | 5.623 | 6.260 | 48.477 | 0.46x |
| users.json | ujson | 29.630 | 30.126 | 33.142 | 48.477 | 0.08x |
| users.json | json | 40.075 | 41.253 | 44.348 | 48.477 | 0.06x |
| flat.json | strata | 0.295 | 0.304 | 0.318 | 58.246 | 1.00x |
| flat.json | orjson | 0.373 | 0.387 | 0.421 | 58.246 | 0.79x |
| flat.json | msgspec | 0.493 | 0.517 | 0.562 | 58.246 | 0.59x |
| flat.json | ujson | 2.547 | 2.598 | 2.708 | 58.246 | 0.12x |
| flat.json | json | 3.496 | 3.669 | 3.952 | 58.246 | 0.08x |
| nested.json | strata | 0.241 | 0.264 | 0.285 | 53.293 | 1.00x |
| nested.json | orjson | 0.336 | 0.349 | 0.412 | 53.293 | 0.76x |
| nested.json | msgspec | 0.538 | 0.565 | 0.627 | 53.293 | 0.47x |
| nested.json | ujson | 2.717 | 2.875 | 2.972 | 53.293 | 0.09x |
| nested.json | json | 4.382 | 4.721 | 5.769 | 53.293 | 0.06x |
| wide_arrays.json | strata | 1.649 | 1.751 | 1.882 | 61.195 | 1.00x |
| wide_arrays.json | orjson | 2.109 | 2.178 | 2.462 | 61.195 | 0.80x |
| wide_arrays.json | msgspec | 2.944 | 3.007 | 3.119 | 61.195 | 0.58x |
| wide_arrays.json | ujson | 14.210 | 14.767 | 15.344 | 61.195 | 0.12x |
| wide_arrays.json | json | 32.050 | 34.466 | 35.154 | 61.195 | 0.05x |
| mixed.json | strata | 0.064 | 0.070 | 0.093 | 56.363 | 1.00x |
| mixed.json | orjson | 0.071 | 0.078 | 0.086 | 56.363 | 0.90x |
| mixed.json | msgspec | 0.099 | 0.107 | 0.116 | 56.363 | 0.65x |
| mixed.json | ujson | 0.591 | 0.621 | 0.625 | 56.363 | 0.11x |
| mixed.json | json | 0.903 | 0.953 | 0.977 | 56.363 | 0.07x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 19.062 | 19.768 | 20.330 | 61.020 | 1.00x |
| users.json | orjson | 24.897 | 26.098 | 28.028 | 61.020 | 0.76x |
| users.json | msgspec | 24.711 | 26.656 | 28.096 | 61.020 | 0.74x |
| users.json | ujson | 36.758 | 40.046 | 43.898 | 61.020 | 0.49x |
| users.json | json | 40.149 | 44.575 | 46.487 | 61.020 | 0.44x |
| flat.json | strata | 1.243 | 1.314 | 1.361 | 58.246 | 1.00x |
| flat.json | orjson | 1.395 | 1.424 | 1.494 | 58.246 | 0.92x |
| flat.json | msgspec | 1.572 | 1.636 | 1.701 | 58.246 | 0.80x |
| flat.json | ujson | 2.704 | 2.809 | 3.024 | 58.246 | 0.47x |
| flat.json | json | 3.036 | 3.119 | 3.643 | 58.246 | 0.42x |
| nested.json | strata | 1.531 | 1.598 | 1.886 | 53.293 | 1.00x |
| nested.json | orjson | 1.752 | 1.847 | 1.966 | 53.293 | 0.86x |
| nested.json | msgspec | 1.996 | 2.047 | 2.191 | 53.293 | 0.78x |
| nested.json | ujson | 3.166 | 3.260 | 3.476 | 53.293 | 0.49x |
| nested.json | json | 4.007 | 4.092 | 4.493 | 53.293 | 0.39x |
| wide_arrays.json | strata | 8.625 | 8.935 | 9.391 | 61.195 | 1.00x |
| wide_arrays.json | orjson | 9.136 | 9.565 | 9.832 | 61.195 | 0.93x |
| wide_arrays.json | msgspec | 10.396 | 10.602 | 10.848 | 61.195 | 0.84x |
| wide_arrays.json | ujson | 13.134 | 14.254 | 14.805 | 61.195 | 0.63x |
| wide_arrays.json | json | 17.638 | 17.810 | 18.068 | 61.195 | 0.50x |
| mixed.json | strata | 0.414 | 0.428 | 0.524 | 56.363 | 1.00x |
| mixed.json | orjson | 0.513 | 0.530 | 0.587 | 56.363 | 0.81x |
| mixed.json | msgspec | 0.527 | 0.559 | 0.607 | 56.363 | 0.77x |
| mixed.json | ujson | 0.689 | 0.745 | 0.857 | 56.363 | 0.58x |
| mixed.json | json | 0.884 | 0.975 | 1.023 | 56.363 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 18.730 | 19.709 | 20.911 | 61.453 | 1.00x |
| users.ndjson | orjson | 25.123 | 26.824 | 28.543 | 61.453 | 0.73x |
| users.ndjson | msgspec | 25.713 | 27.554 | 29.276 | 61.453 | 0.72x |
| users.ndjson | ujson | 36.630 | 38.022 | 40.641 | 61.453 | 0.52x |
| users.ndjson | json | 45.936 | 47.378 | 51.323 | 61.453 | 0.42x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.073 | 3.380 | 3.496 | 55.453 | 1.00x |
| users.json | orjson | 3.978 | 4.065 | 4.324 | 55.453 | 0.83x |
| users.json | msgspec | 5.881 | 6.659 | 7.197 | 55.453 | 0.51x |
| users.json | ujson | 30.682 | 30.943 | 32.821 | 55.453 | 0.11x |
| users.json | json | 41.062 | 42.148 | 45.177 | 55.453 | 0.08x |
| flat.json | strata | 0.603 | 0.650 | 0.711 | 58.246 | 1.00x |
| flat.json | orjson | 0.645 | 0.719 | 0.787 | 58.246 | 0.90x |
| flat.json | msgspec | 0.818 | 0.868 | 0.910 | 58.246 | 0.75x |
| flat.json | ujson | 2.879 | 2.983 | 3.331 | 58.246 | 0.22x |
| flat.json | json | 3.785 | 4.026 | 4.104 | 58.246 | 0.16x |
| nested.json | strata | 0.580 | 0.650 | 0.682 | 53.293 | 1.00x |
| nested.json | orjson | 0.706 | 0.790 | 1.020 | 53.293 | 0.82x |
| nested.json | msgspec | 0.931 | 1.014 | 1.169 | 53.293 | 0.64x |
| nested.json | ujson | 3.169 | 3.259 | 3.407 | 53.293 | 0.20x |
| nested.json | json | 4.904 | 5.132 | 5.252 | 53.293 | 0.13x |
| wide_arrays.json | strata | 2.391 | 2.505 | 2.954 | 61.195 | 1.00x |
| wide_arrays.json | orjson | 3.009 | 3.096 | 3.655 | 61.195 | 0.81x |
| wide_arrays.json | msgspec | 3.785 | 3.953 | 4.899 | 61.195 | 0.63x |
| wide_arrays.json | ujson | 15.415 | 15.768 | 17.524 | 61.195 | 0.16x |
| wide_arrays.json | json | 33.103 | 34.687 | 40.049 | 61.195 | 0.07x |
| mixed.json | strata | 0.297 | 0.341 | 0.382 | 56.363 | 1.00x |
| mixed.json | orjson | 0.318 | 0.362 | 0.529 | 56.363 | 0.94x |
| mixed.json | msgspec | 0.343 | 0.355 | 0.450 | 56.363 | 0.96x |
| mixed.json | ujson | 0.842 | 0.863 | 1.005 | 56.363 | 0.39x |
| mixed.json | json | 1.154 | 1.210 | 1.275 | 56.363 | 0.28x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.127 | 0.137 | 0.177 | 55.520 | 1.00x |
| users.json $[*].id | jmespath | 0.901 | 0.907 | 0.996 | 55.520 | 0.15x |
| users.json $[*].id | jsonpath-ng | 4.860 | 4.898 | 4.943 | 55.520 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.812 | 0.890 | 1.593 | 55.414 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.568 | 5.988 | 6.589 | 55.414 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 32.809 | 34.513 | 38.521 | 55.414 | 0.03x |
| users.json $..total | strata | 3.047 | 3.225 | 3.646 | 57.410 | 1.00x |
| users.json $..total | jsonpath-ng | 676.049 | 701.912 | 760.592 | 57.410 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.624 | 3.712 | 4.007 | 59.551 | 1.00x |
| users.json $[*].id | orjson+jmespath | 25.570 | 27.741 | 31.288 | 59.551 | 0.13x |
| users.json $[*].id | orjson+jsonpath-ng | 29.689 | 32.390 | 33.915 | 59.551 | 0.11x |
| users.json $[*].orders[*].total | strata | 4.008 | 4.058 | 4.252 | 56.621 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 30.600 | 31.075 | 32.207 | 56.621 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 62.494 | 64.594 | 70.051 | 56.621 | 0.06x |
| users.json $..total | strata | 22.590 | 23.322 | 24.106 | 56.730 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 681.360 | 725.129 | 744.598 | 56.730 | 0.03x |

