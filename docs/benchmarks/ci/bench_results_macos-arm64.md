# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 07e47e6
- python: 3.12.10
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit
- machine: arm64
- processor: Apple M1 (Virtual)
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.344 | 10.645 | 14.432 | 63.625 | 1.00x |
| users.json | orjson | 10.628 | 14.303 | 22.801 | 63.625 | 0.74x |
| users.json | msgspec | 10.869 | 13.603 | 19.842 | 63.625 | 0.78x |
| users.json | ujson | 14.798 | 19.870 | 25.548 | 63.625 | 0.54x |
| users.json | pysimdjson | 175.799 | 184.778 | 207.277 | 63.625 | 0.06x |
| users.json | json | 16.821 | 22.760 | 34.352 | 63.625 | 0.47x |
| flat.json | strata | 0.666 | 0.757 | 1.750 | 85.516 | 1.00x |
| flat.json | orjson | 0.820 | 1.031 | 2.915 | 85.516 | 0.73x |
| flat.json | msgspec | 0.802 | 0.894 | 1.564 | 85.516 | 0.85x |
| flat.json | ujson | 1.301 | 1.514 | 1.834 | 85.516 | 0.50x |
| flat.json | pysimdjson | 13.233 | 14.657 | 19.360 | 85.516 | 0.05x |
| flat.json | json | 1.553 | 1.815 | 3.570 | 85.516 | 0.42x |
| nested.json | strata | 0.640 | 0.703 | 2.017 | 85.547 | 1.00x |
| nested.json | orjson | 0.889 | 0.954 | 2.213 | 85.547 | 0.74x |
| nested.json | msgspec | 0.738 | 0.893 | 1.416 | 85.547 | 0.79x |
| nested.json | ujson | 1.260 | 1.544 | 2.520 | 85.547 | 0.46x |
| nested.json | pysimdjson | 12.174 | 13.505 | 17.701 | 85.547 | 0.05x |
| nested.json | json | 1.536 | 1.876 | 2.456 | 85.547 | 0.37x |
| wide_arrays.json | strata | 4.468 | 5.711 | 9.441 | 88.062 | 1.00x |
| wide_arrays.json | orjson | 4.676 | 5.936 | 10.426 | 88.062 | 0.96x |
| wide_arrays.json | msgspec | 5.324 | 6.143 | 10.240 | 88.062 | 0.93x |
| wide_arrays.json | ujson | 6.959 | 7.519 | 14.052 | 88.062 | 0.76x |
| wide_arrays.json | pysimdjson | 93.546 | 99.736 | 115.567 | 88.062 | 0.06x |
| wide_arrays.json | json | 7.965 | 10.947 | 16.778 | 88.062 | 0.52x |
| mixed.json | strata | 0.154 | 0.172 | 0.512 | 88.078 | 1.00x |
| mixed.json | orjson | 0.211 | 0.226 | 0.443 | 88.078 | 0.76x |
| mixed.json | msgspec | 0.204 | 0.253 | 1.034 | 88.078 | 0.68x |
| mixed.json | ujson | 0.271 | 0.440 | 1.091 | 88.078 | 0.39x |
| mixed.json | pysimdjson | 2.823 | 5.397 | 8.595 | 88.078 | 0.03x |
| mixed.json | json | 0.393 | 0.452 | 1.471 | 88.078 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.818 | 2.235 | 4.841 | 72.172 | 1.00x |
| users.json | orjson | 2.697 | 3.285 | 6.182 | 72.172 | 0.68x |
| users.json | msgspec | 3.514 | 4.202 | 7.485 | 72.172 | 0.53x |
| users.json | ujson | 11.682 | 15.351 | 24.820 | 72.172 | 0.15x |
| users.json | json | 18.013 | 27.894 | 33.496 | 72.172 | 0.08x |
| flat.json | strata | 0.262 | 0.311 | 0.529 | 85.531 | 1.00x |
| flat.json | orjson | 0.308 | 0.362 | 0.978 | 85.531 | 0.86x |
| flat.json | msgspec | 0.374 | 0.440 | 0.655 | 85.531 | 0.71x |
| flat.json | ujson | 1.140 | 1.647 | 2.728 | 85.531 | 0.19x |
| flat.json | json | 1.513 | 1.944 | 3.619 | 85.531 | 0.16x |
| nested.json | strata | 0.158 | 0.190 | 0.710 | 85.562 | 1.00x |
| nested.json | orjson | 0.236 | 0.349 | 0.780 | 85.562 | 0.55x |
| nested.json | msgspec | 0.328 | 0.520 | 0.823 | 85.562 | 0.37x |
| nested.json | ujson | 1.273 | 1.478 | 2.986 | 85.562 | 0.13x |
| nested.json | json | 2.070 | 2.564 | 3.852 | 85.562 | 0.07x |
| wide_arrays.json | strata | 1.251 | 1.736 | 2.440 | 88.062 | 1.00x |
| wide_arrays.json | orjson | 1.619 | 2.719 | 3.819 | 88.062 | 0.64x |
| wide_arrays.json | msgspec | 2.474 | 3.835 | 6.033 | 88.062 | 0.45x |
| wide_arrays.json | ujson | 7.916 | 10.084 | 14.209 | 88.062 | 0.17x |
| wide_arrays.json | json | 15.443 | 16.773 | 27.374 | 88.062 | 0.10x |
| mixed.json | strata | 0.051 | 0.120 | 0.287 | 88.078 | 1.00x |
| mixed.json | orjson | 0.062 | 0.066 | 0.428 | 88.078 | 1.81x |
| mixed.json | msgspec | 0.072 | 0.089 | 0.264 | 88.078 | 1.35x |
| mixed.json | ujson | 0.248 | 0.279 | 1.355 | 88.078 | 0.43x |
| mixed.json | json | 0.433 | 0.748 | 2.046 | 88.078 | 0.16x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.629 | 9.536 | 19.779 | 83.484 | 1.00x |
| users.json | orjson | 11.821 | 15.854 | 30.373 | 83.484 | 0.60x |
| users.json | msgspec | 11.640 | 14.691 | 22.865 | 83.484 | 0.65x |
| users.json | ujson | 14.880 | 18.296 | 41.845 | 83.484 | 0.52x |
| users.json | json | 17.593 | 23.928 | 33.322 | 83.484 | 0.40x |
| flat.json | strata | 0.817 | 1.010 | 1.938 | 85.531 | 1.00x |
| flat.json | orjson | 1.322 | 1.562 | 2.932 | 85.531 | 0.65x |
| flat.json | msgspec | 1.043 | 1.205 | 2.189 | 85.531 | 0.84x |
| flat.json | ujson | 1.410 | 1.853 | 2.478 | 85.531 | 0.54x |
| flat.json | json | 1.721 | 2.128 | 4.286 | 85.531 | 0.47x |
| nested.json | strata | 0.657 | 0.765 | 2.026 | 85.562 | 1.00x |
| nested.json | orjson | 1.034 | 1.251 | 2.892 | 85.562 | 0.61x |
| nested.json | msgspec | 0.900 | 1.010 | 2.495 | 85.562 | 0.76x |
| nested.json | ujson | 1.198 | 1.455 | 1.938 | 85.562 | 0.53x |
| nested.json | json | 1.657 | 1.848 | 2.692 | 85.562 | 0.41x |
| wide_arrays.json | strata | 3.856 | 6.235 | 8.087 | 88.062 | 1.00x |
| wide_arrays.json | orjson | 4.409 | 6.502 | 8.152 | 88.062 | 0.96x |
| wide_arrays.json | msgspec | 4.951 | 6.767 | 10.730 | 88.062 | 0.92x |
| wide_arrays.json | ujson | 6.629 | 9.031 | 12.852 | 88.062 | 0.69x |
| wide_arrays.json | json | 7.985 | 8.996 | 16.777 | 88.062 | 0.69x |
| mixed.json | strata | 0.201 | 0.218 | 4.398 | 88.078 | 1.00x |
| mixed.json | orjson | 0.322 | 0.451 | 1.228 | 88.078 | 0.48x |
| mixed.json | msgspec | 0.274 | 0.366 | 0.567 | 88.078 | 0.60x |
| mixed.json | ujson | 0.366 | 0.424 | 1.163 | 88.078 | 0.51x |
| mixed.json | json | 0.439 | 0.497 | 1.283 | 88.078 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.119 | 10.983 | 30.290 | 85.516 | 1.00x |
| users.ndjson | orjson | 15.243 | 18.994 | 42.064 | 85.516 | 0.58x |
| users.ndjson | msgspec | 13.894 | 16.932 | 31.077 | 85.516 | 0.65x |
| users.ndjson | ujson | 18.327 | 24.295 | 28.526 | 85.516 | 0.45x |
| users.ndjson | json | 24.398 | 25.975 | 40.937 | 85.516 | 0.42x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.230 | 2.635 | 3.755 | 79.781 | 1.00x |
| users.json | orjson | 3.164 | 3.576 | 4.302 | 79.781 | 0.74x |
| users.json | msgspec | 3.674 | 3.957 | 4.946 | 79.781 | 0.67x |
| users.json | ujson | 13.102 | 14.098 | 15.079 | 79.781 | 0.19x |
| users.json | json | 17.392 | 20.812 | 26.318 | 79.781 | 0.13x |
| flat.json | strata | 0.604 | 0.851 | 1.408 | 85.531 | 1.00x |
| flat.json | orjson | 0.719 | 0.875 | 7.190 | 85.531 | 0.97x |
| flat.json | msgspec | 0.753 | 1.039 | 6.130 | 85.531 | 0.82x |
| flat.json | ujson | 1.496 | 1.876 | 3.983 | 85.531 | 0.45x |
| flat.json | json | 2.067 | 2.436 | 5.336 | 85.531 | 0.35x |
| nested.json | strata | 0.483 | 0.561 | 1.913 | 85.562 | 1.00x |
| nested.json | orjson | 0.595 | 0.694 | 1.067 | 85.562 | 0.81x |
| nested.json | msgspec | 0.719 | 0.826 | 1.593 | 85.562 | 0.68x |
| nested.json | ujson | 1.544 | 1.805 | 2.466 | 85.562 | 0.31x |
| nested.json | json | 2.368 | 2.575 | 3.336 | 85.562 | 0.22x |
| wide_arrays.json | strata | 1.800 | 2.427 | 5.251 | 88.062 | 1.00x |
| wide_arrays.json | orjson | 2.347 | 2.801 | 5.595 | 88.062 | 0.87x |
| wide_arrays.json | msgspec | 3.223 | 3.808 | 6.464 | 88.062 | 0.64x |
| wide_arrays.json | ujson | 8.446 | 9.655 | 17.938 | 88.062 | 0.25x |
| wide_arrays.json | json | 13.985 | 18.278 | 28.172 | 88.062 | 0.13x |
| mixed.json | strata | 0.330 | 0.355 | 0.955 | 88.078 | 1.00x |
| mixed.json | orjson | 0.293 | 0.335 | 0.906 | 88.078 | 1.06x |
| mixed.json | msgspec | 0.313 | 0.523 | 5.158 | 88.078 | 0.68x |
| mixed.json | ujson | 0.491 | 0.637 | 1.590 | 88.078 | 0.56x |
| mixed.json | json | 0.618 | 0.862 | 1.913 | 88.078 | 0.41x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.083 | 0.134 | 0.354 | 79.859 | 1.00x |
| users.json $[*].id | jmespath | 0.353 | 0.438 | 1.035 | 79.859 | 0.31x |
| users.json $[*].id | jsonpath-ng | 1.698 | 1.999 | 2.926 | 79.859 | 0.07x |
| users.json $[*].orders[*].total | strata | 0.858 | 1.182 | 2.618 | 80.141 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.460 | 2.551 | 4.795 | 80.141 | 0.46x |
| users.json $[*].orders[*].total | jsonpath-ng | 14.134 | 22.088 | 40.977 | 80.141 | 0.05x |
| users.json $..total | strata | 1.473 | 2.411 | 4.202 | 82.000 | 1.00x |
| users.json $..total | jsonpath-ng | 287.168 | 324.650 | 418.365 | 82.000 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.187 | 4.481 | 5.133 | 79.922 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.212 | 13.602 | 23.581 | 79.922 | 0.33x |
| users.json $[*].id | orjson+jsonpath-ng | 12.545 | 15.471 | 42.453 | 79.922 | 0.29x |
| users.json $[*].orders[*].total | strata | 4.180 | 6.495 | 8.527 | 81.953 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 14.426 | 22.225 | 33.212 | 81.953 | 0.29x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 30.250 | 49.200 | 65.165 | 81.953 | 0.13x |
| users.json $..total | strata | 11.069 | 16.906 | 32.107 | 82.016 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 273.991 | 360.046 | 452.322 | 82.016 | 0.05x |

