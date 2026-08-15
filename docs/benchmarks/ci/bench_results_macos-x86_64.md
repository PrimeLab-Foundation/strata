# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 75b7e23
- python: 3.12.10
- implementation: CPython
- platform: macOS-15.7.7-x86_64-i386-64bit
- machine: x86_64
- processor: Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 21.807 | 22.444 | 25.251 | 52.801 | 1.00x |
| users.json | orjson | 27.197 | 28.202 | 30.184 | 52.801 | 0.80x |
| users.json | msgspec | 27.419 | 28.322 | 30.639 | 52.801 | 0.79x |
| users.json | ujson | 39.968 | 41.282 | 44.653 | 52.801 | 0.54x |
| users.json | pysimdjson | 176.298 | 180.811 | 184.939 | 52.801 | 0.12x |
| users.json | json | 45.577 | 47.675 | 53.533 | 52.801 | 0.47x |
| flat.json | strata | 1.465 | 1.503 | 1.524 | 57.816 | 1.00x |
| flat.json | orjson | 1.499 | 1.527 | 1.582 | 57.816 | 0.98x |
| flat.json | msgspec | 1.722 | 1.763 | 2.203 | 57.816 | 0.85x |
| flat.json | ujson | 3.023 | 3.038 | 3.174 | 57.816 | 0.49x |
| flat.json | pysimdjson | 16.000 | 16.085 | 16.740 | 57.816 | 0.09x |
| flat.json | json | 3.441 | 3.513 | 3.557 | 57.816 | 0.43x |
| nested.json | strata | 1.690 | 1.758 | 1.806 | 55.379 | 1.00x |
| nested.json | orjson | 1.844 | 1.895 | 1.971 | 55.379 | 0.93x |
| nested.json | msgspec | 2.040 | 2.168 | 2.310 | 55.379 | 0.81x |
| nested.json | ujson | 3.388 | 3.496 | 3.720 | 55.379 | 0.50x |
| nested.json | pysimdjson | 15.055 | 15.498 | 16.476 | 55.379 | 0.11x |
| nested.json | json | 4.351 | 4.497 | 4.833 | 55.379 | 0.39x |
| wide_arrays.json | strata | 9.544 | 9.668 | 9.951 | 58.973 | 1.00x |
| wide_arrays.json | orjson | 9.983 | 10.265 | 11.269 | 58.973 | 0.94x |
| wide_arrays.json | msgspec | 10.938 | 11.161 | 11.938 | 58.973 | 0.87x |
| wide_arrays.json | ujson | 14.280 | 14.598 | 15.157 | 58.973 | 0.66x |
| wide_arrays.json | pysimdjson | 88.335 | 89.031 | 90.427 | 58.973 | 0.11x |
| wide_arrays.json | json | 18.650 | 18.799 | 21.742 | 58.973 | 0.51x |
| mixed.json | strata | 0.413 | 0.428 | 0.457 | 58.297 | 1.00x |
| mixed.json | orjson | 0.472 | 0.485 | 0.499 | 58.297 | 0.88x |
| mixed.json | msgspec | 0.500 | 0.516 | 0.773 | 58.297 | 0.83x |
| mixed.json | ujson | 0.687 | 0.712 | 0.727 | 58.297 | 0.60x |
| mixed.json | pysimdjson | 3.512 | 3.557 | 3.597 | 58.297 | 0.12x |
| mixed.json | json | 0.961 | 0.981 | 1.024 | 58.297 | 0.44x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.137 | 3.345 | 3.675 | 47.406 | 1.00x |
| users.json | orjson | 3.561 | 3.732 | 4.704 | 47.406 | 0.90x |
| users.json | msgspec | 6.081 | 6.474 | 6.836 | 47.406 | 0.52x |
| users.json | ujson | 34.188 | 34.397 | 35.871 | 47.406 | 0.10x |
| users.json | json | 45.684 | 46.397 | 48.351 | 47.406 | 0.07x |
| flat.json | strata | 0.342 | 0.359 | 0.378 | 54.762 | 1.00x |
| flat.json | orjson | 0.431 | 0.455 | 0.499 | 54.762 | 0.79x |
| flat.json | msgspec | 0.574 | 0.606 | 0.672 | 54.762 | 0.59x |
| flat.json | ujson | 2.967 | 3.011 | 3.414 | 54.762 | 0.12x |
| flat.json | json | 4.068 | 4.097 | 4.602 | 54.762 | 0.09x |
| nested.json | strata | 0.286 | 0.308 | 0.428 | 55.523 | 1.00x |
| nested.json | orjson | 0.412 | 0.470 | 1.573 | 55.523 | 0.65x |
| nested.json | msgspec | 0.632 | 0.754 | 1.209 | 55.523 | 0.41x |
| nested.json | ujson | 3.142 | 3.452 | 7.075 | 55.523 | 0.09x |
| nested.json | json | 5.260 | 5.887 | 22.705 | 55.523 | 0.05x |
| wide_arrays.json | strata | 1.947 | 2.116 | 2.274 | 61.402 | 1.00x |
| wide_arrays.json | orjson | 2.451 | 2.616 | 2.704 | 61.402 | 0.81x |
| wide_arrays.json | msgspec | 3.408 | 3.561 | 3.792 | 61.402 | 0.59x |
| wide_arrays.json | ujson | 16.623 | 17.022 | 19.459 | 61.402 | 0.12x |
| wide_arrays.json | json | 37.202 | 37.794 | 40.377 | 61.402 | 0.06x |
| mixed.json | strata | 0.091 | 0.098 | 0.102 | 56.652 | 1.00x |
| mixed.json | orjson | 0.080 | 0.087 | 0.099 | 56.652 | 1.13x |
| mixed.json | msgspec | 0.118 | 0.126 | 0.137 | 56.652 | 0.78x |
| mixed.json | ujson | 0.666 | 0.670 | 0.721 | 56.652 | 0.15x |
| mixed.json | json | 1.036 | 1.046 | 1.066 | 56.652 | 0.09x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 21.379 | 21.549 | 23.082 | 60.883 | 1.00x |
| users.json | orjson | 26.340 | 27.069 | 27.884 | 60.883 | 0.80x |
| users.json | msgspec | 26.795 | 27.309 | 28.438 | 60.883 | 0.79x |
| users.json | ujson | 39.465 | 40.436 | 44.134 | 60.883 | 0.53x |
| users.json | json | 44.923 | 45.513 | 46.989 | 60.883 | 0.47x |
| flat.json | strata | 1.612 | 1.659 | 1.742 | 55.414 | 1.00x |
| flat.json | orjson | 1.679 | 1.720 | 1.781 | 55.414 | 0.96x |
| flat.json | msgspec | 1.909 | 1.957 | 2.110 | 55.414 | 0.85x |
| flat.json | ujson | 3.215 | 3.330 | 3.386 | 55.414 | 0.50x |
| flat.json | json | 3.674 | 3.731 | 3.889 | 55.414 | 0.44x |
| nested.json | strata | 1.863 | 2.063 | 2.457 | 55.523 | 1.00x |
| nested.json | orjson | 2.041 | 2.252 | 2.698 | 55.523 | 0.92x |
| nested.json | msgspec | 2.310 | 2.600 | 2.994 | 55.523 | 0.79x |
| nested.json | ujson | 3.730 | 3.975 | 4.641 | 55.523 | 0.52x |
| nested.json | json | 4.647 | 5.407 | 6.635 | 55.523 | 0.38x |
| wide_arrays.json | strata | 9.537 | 9.692 | 9.930 | 61.402 | 1.00x |
| wide_arrays.json | orjson | 9.864 | 10.609 | 11.514 | 61.402 | 0.91x |
| wide_arrays.json | msgspec | 11.051 | 11.379 | 11.711 | 61.402 | 0.85x |
| wide_arrays.json | ujson | 14.520 | 14.991 | 15.413 | 61.402 | 0.65x |
| wide_arrays.json | json | 18.339 | 18.763 | 21.646 | 61.402 | 0.52x |
| mixed.json | strata | 0.488 | 0.519 | 0.710 | 56.652 | 1.00x |
| mixed.json | orjson | 0.585 | 0.620 | 0.681 | 56.652 | 0.84x |
| mixed.json | msgspec | 0.636 | 0.658 | 0.692 | 56.652 | 0.79x |
| mixed.json | ujson | 0.825 | 0.842 | 0.899 | 56.652 | 0.62x |
| mixed.json | json | 1.056 | 1.103 | 1.152 | 56.652 | 0.47x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 22.637 | 22.949 | 23.841 | 56.859 | 1.00x |
| users.ndjson | orjson | 29.372 | 30.211 | 31.233 | 56.859 | 0.76x |
| users.ndjson | msgspec | 29.824 | 30.672 | 32.690 | 56.859 | 0.75x |
| users.ndjson | ujson | 42.291 | 43.315 | 51.633 | 56.859 | 0.53x |
| users.ndjson | json | 53.273 | 54.957 | 62.115 | 56.859 | 0.42x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.713 | 4.145 | 5.105 | 53.059 | 1.00x |
| users.json | orjson | 4.450 | 5.080 | 5.507 | 53.059 | 0.82x |
| users.json | msgspec | 6.717 | 7.001 | 8.408 | 53.059 | 0.59x |
| users.json | ujson | 33.997 | 35.946 | 38.638 | 53.059 | 0.12x |
| users.json | json | 44.463 | 47.424 | 54.242 | 53.059 | 0.09x |
| flat.json | strata | 0.750 | 0.788 | 0.836 | 55.414 | 1.00x |
| flat.json | orjson | 0.857 | 0.903 | 0.958 | 55.414 | 0.87x |
| flat.json | msgspec | 1.020 | 1.067 | 1.131 | 55.414 | 0.74x |
| flat.json | ujson | 3.430 | 3.484 | 3.567 | 55.414 | 0.23x |
| flat.json | json | 4.492 | 4.697 | 5.191 | 55.414 | 0.17x |
| nested.json | strata | 0.601 | 0.669 | 1.291 | 55.523 | 1.00x |
| nested.json | orjson | 0.765 | 0.836 | 1.061 | 55.523 | 0.80x |
| nested.json | msgspec | 0.994 | 1.071 | 1.180 | 55.523 | 0.62x |
| nested.json | ujson | 3.525 | 3.771 | 7.957 | 55.523 | 0.18x |
| nested.json | json | 5.697 | 6.073 | 9.345 | 55.523 | 0.11x |
| wide_arrays.json | strata | 2.620 | 2.857 | 3.475 | 61.402 | 1.00x |
| wide_arrays.json | orjson | 3.153 | 3.388 | 3.592 | 61.402 | 0.84x |
| wide_arrays.json | msgspec | 4.290 | 4.506 | 5.732 | 61.402 | 0.63x |
| wide_arrays.json | ujson | 17.791 | 18.034 | 19.237 | 61.402 | 0.16x |
| wide_arrays.json | json | 38.202 | 38.898 | 41.396 | 61.402 | 0.07x |
| mixed.json | strata | 0.404 | 0.455 | 0.512 | 56.652 | 1.00x |
| mixed.json | orjson | 0.443 | 0.499 | 0.621 | 56.652 | 0.91x |
| mixed.json | msgspec | 0.485 | 0.511 | 0.700 | 56.652 | 0.89x |
| mixed.json | ujson | 1.071 | 1.105 | 1.295 | 56.652 | 0.41x |
| mixed.json | json | 1.454 | 1.498 | 1.613 | 56.652 | 0.30x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.153 | 0.188 | 0.217 | 53.125 | 1.00x |
| users.json $[*].id | jmespath | 1.019 | 1.091 | 1.117 | 53.125 | 0.17x |
| users.json $[*].id | jsonpath-ng | 5.652 | 5.780 | 6.179 | 53.125 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.911 | 1.001 | 1.057 | 53.586 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 6.439 | 6.669 | 7.136 | 53.586 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 38.124 | 38.330 | 39.304 | 53.586 | 0.03x |
| users.json $..total | strata | 3.514 | 3.785 | 4.026 | 55.637 | 1.00x |
| users.json $..total | jsonpath-ng | 748.968 | 774.442 | 831.489 | 55.637 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.317 | 4.364 | 4.387 | 57.914 | 1.00x |
| users.json $[*].id | orjson+jmespath | 27.790 | 28.787 | 31.191 | 57.914 | 0.15x |
| users.json $[*].id | orjson+jsonpath-ng | 33.150 | 33.841 | 35.886 | 57.914 | 0.13x |
| users.json $[*].orders[*].total | strata | 4.689 | 4.793 | 5.038 | 55.605 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 33.343 | 34.793 | 39.921 | 55.605 | 0.14x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 70.679 | 72.858 | 83.448 | 55.605 | 0.07x |
| users.json $..total | strata | 26.386 | 27.059 | 28.565 | 55.586 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 802.528 | 825.560 | 845.613 | 55.586 | 0.03x |

