# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: b6e2a27
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: x86_64
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.376 | 9.735 | 12.926 | 60.500 | 1.00x |
| users.json | orjson | 12.349 | 12.946 | 15.689 | 60.500 | 0.75x |
| users.json | msgspec | 13.182 | 13.442 | 15.471 | 60.500 | 0.72x |
| users.json | ujson | 18.253 | 18.724 | 22.879 | 60.500 | 0.52x |
| users.json | pysimdjson | 17.645 | 17.900 | 23.944 | 60.500 | 0.54x |
| users.json | json | 22.132 | 22.453 | 23.886 | 60.500 | 0.43x |
| flat.json | strata | 0.825 | 0.833 | 0.837 | 71.047 | 1.00x |
| flat.json | orjson | 0.946 | 0.991 | 1.004 | 71.047 | 0.84x |
| flat.json | msgspec | 1.015 | 1.024 | 1.055 | 71.047 | 0.81x |
| flat.json | ujson | 1.545 | 1.582 | 1.601 | 71.047 | 0.53x |
| flat.json | pysimdjson | 1.432 | 1.528 | 1.557 | 71.047 | 0.55x |
| flat.json | json | 1.771 | 1.914 | 1.929 | 71.047 | 0.44x |
| nested.json | strata | 0.745 | 0.799 | 0.807 | 71.047 | 1.00x |
| nested.json | orjson | 0.936 | 1.004 | 1.018 | 71.047 | 0.80x |
| nested.json | msgspec | 0.974 | 1.023 | 1.323 | 71.047 | 0.78x |
| nested.json | ujson | 1.503 | 1.528 | 1.546 | 71.047 | 0.52x |
| nested.json | pysimdjson | 1.369 | 1.401 | 1.418 | 71.047 | 0.57x |
| nested.json | json | 1.877 | 2.047 | 2.175 | 71.047 | 0.39x |
| wide_arrays.json | strata | 4.297 | 4.336 | 4.375 | 73.824 | 1.00x |
| wide_arrays.json | orjson | 4.963 | 5.031 | 5.326 | 73.824 | 0.86x |
| wide_arrays.json | msgspec | 5.515 | 5.636 | 5.780 | 73.824 | 0.77x |
| wide_arrays.json | ujson | 6.593 | 6.975 | 7.058 | 73.824 | 0.62x |
| wide_arrays.json | pysimdjson | 5.925 | 5.962 | 6.050 | 73.824 | 0.73x |
| wide_arrays.json | json | 9.584 | 9.629 | 9.704 | 73.824 | 0.45x |
| mixed.json | strata | 0.185 | 0.187 | 0.220 | 73.887 | 1.00x |
| mixed.json | orjson | 0.226 | 0.227 | 0.241 | 73.887 | 0.83x |
| mixed.json | msgspec | 0.238 | 0.239 | 0.255 | 73.887 | 0.78x |
| mixed.json | ujson | 0.312 | 0.316 | 0.334 | 73.887 | 0.59x |
| mixed.json | pysimdjson | 0.300 | 0.301 | 0.314 | 73.887 | 0.62x |
| mixed.json | json | 0.482 | 0.493 | 0.522 | 73.887 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.240 | 2.413 | 2.427 | 58.859 | 1.00x |
| users.json | orjson | 2.836 | 2.902 | 2.919 | 58.859 | 0.83x |
| users.json | msgspec | 3.803 | 3.847 | 3.885 | 58.859 | 0.63x |
| users.json | ujson | 11.183 | 11.369 | 11.638 | 58.859 | 0.21x |
| users.json | json | 20.939 | 21.774 | 22.005 | 58.859 | 0.11x |
| flat.json | strata | 0.265 | 0.266 | 0.278 | 71.047 | 1.00x |
| flat.json | orjson | 0.306 | 0.340 | 0.342 | 71.047 | 0.78x |
| flat.json | msgspec | 0.431 | 0.438 | 0.449 | 71.047 | 0.61x |
| flat.json | ujson | 1.013 | 1.017 | 1.024 | 71.047 | 0.26x |
| flat.json | json | 1.719 | 1.860 | 2.093 | 71.047 | 0.14x |
| nested.json | strata | 0.238 | 0.259 | 0.275 | 71.051 | 1.00x |
| nested.json | orjson | 0.291 | 0.294 | 0.308 | 71.051 | 0.88x |
| nested.json | msgspec | 0.396 | 0.424 | 0.437 | 71.051 | 0.61x |
| nested.json | ujson | 1.070 | 1.077 | 1.085 | 71.051 | 0.24x |
| nested.json | json | 2.172 | 2.355 | 2.388 | 71.051 | 0.11x |
| wide_arrays.json | strata | 1.472 | 1.505 | 1.517 | 73.824 | 1.00x |
| wide_arrays.json | orjson | 1.696 | 1.766 | 1.872 | 73.824 | 0.85x |
| wide_arrays.json | msgspec | 2.590 | 2.671 | 2.706 | 73.824 | 0.56x |
| wide_arrays.json | ujson | 5.945 | 6.253 | 6.280 | 73.824 | 0.24x |
| wide_arrays.json | json | 16.009 | 16.481 | 16.744 | 73.824 | 0.09x |
| mixed.json | strata | 0.061 | 0.062 | 0.074 | 73.887 | 1.00x |
| mixed.json | orjson | 0.063 | 0.065 | 0.066 | 73.887 | 0.96x |
| mixed.json | msgspec | 0.083 | 0.085 | 0.096 | 73.887 | 0.73x |
| mixed.json | ujson | 0.232 | 0.234 | 0.237 | 73.887 | 0.27x |
| mixed.json | json | 0.498 | 0.515 | 0.520 | 73.887 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.833 | 10.157 | 11.720 | 75.633 | 1.00x |
| users.json | orjson | 12.890 | 13.513 | 14.103 | 75.633 | 0.75x |
| users.json | msgspec | 13.173 | 13.562 | 13.710 | 75.633 | 0.75x |
| users.json | ujson | 18.720 | 19.466 | 21.145 | 75.633 | 0.52x |
| users.json | json | 22.109 | 22.911 | 23.050 | 75.633 | 0.44x |
| flat.json | strata | 0.866 | 0.871 | 0.874 | 71.047 | 1.00x |
| flat.json | orjson | 1.042 | 1.051 | 1.056 | 71.047 | 0.83x |
| flat.json | msgspec | 1.065 | 1.068 | 1.074 | 71.047 | 0.82x |
| flat.json | ujson | 1.663 | 1.673 | 1.699 | 71.047 | 0.52x |
| flat.json | json | 1.958 | 1.974 | 1.994 | 71.047 | 0.44x |
| nested.json | strata | 0.760 | 0.828 | 0.841 | 71.051 | 1.00x |
| nested.json | orjson | 1.042 | 1.065 | 1.078 | 71.051 | 0.78x |
| nested.json | msgspec | 1.037 | 1.073 | 1.084 | 71.051 | 0.77x |
| nested.json | ujson | 1.466 | 1.605 | 1.645 | 71.051 | 0.52x |
| nested.json | json | 2.030 | 2.104 | 2.210 | 71.051 | 0.39x |
| wide_arrays.json | strata | 4.265 | 4.435 | 4.484 | 73.887 | 1.00x |
| wide_arrays.json | orjson | 4.915 | 5.078 | 5.176 | 73.887 | 0.87x |
| wide_arrays.json | msgspec | 5.617 | 5.660 | 5.694 | 73.887 | 0.78x |
| wide_arrays.json | ujson | 7.087 | 7.180 | 7.255 | 73.887 | 0.62x |
| wide_arrays.json | json | 9.445 | 9.696 | 9.733 | 73.887 | 0.46x |
| mixed.json | strata | 0.208 | 0.217 | 0.224 | 73.887 | 1.00x |
| mixed.json | orjson | 0.275 | 0.277 | 0.278 | 73.887 | 0.79x |
| mixed.json | msgspec | 0.285 | 0.301 | 0.305 | 73.887 | 0.72x |
| mixed.json | ujson | 0.369 | 0.372 | 0.381 | 73.887 | 0.58x |
| mixed.json | json | 0.526 | 0.535 | 0.574 | 73.887 | 0.41x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.709 | 9.994 | 10.545 | 71.047 | 1.00x |
| users.ndjson | orjson | 15.761 | 16.548 | 16.760 | 71.047 | 0.60x |
| users.ndjson | msgspec | 16.108 | 16.538 | 16.687 | 71.047 | 0.60x |
| users.ndjson | ujson | 21.515 | 22.345 | 22.662 | 71.047 | 0.45x |
| users.ndjson | json | 28.876 | 29.137 | 29.716 | 71.047 | 0.34x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.931 | 2.954 | 3.016 | 70.711 | 1.00x |
| users.json | orjson | 3.417 | 3.530 | 3.584 | 70.711 | 0.84x |
| users.json | msgspec | 4.686 | 4.872 | 4.953 | 70.711 | 0.61x |
| users.json | ujson | 12.180 | 12.283 | 12.490 | 70.711 | 0.24x |
| users.json | json | 22.087 | 22.558 | 22.831 | 70.711 | 0.13x |
| flat.json | strata | 0.400 | 0.402 | 0.424 | 71.047 | 1.00x |
| flat.json | orjson | 0.483 | 0.487 | 0.504 | 71.047 | 0.82x |
| flat.json | msgspec | 0.589 | 0.605 | 0.615 | 71.047 | 0.66x |
| flat.json | ujson | 1.193 | 1.200 | 1.229 | 71.047 | 0.33x |
| flat.json | json | 2.039 | 2.046 | 2.100 | 71.047 | 0.20x |
| nested.json | strata | 0.368 | 0.378 | 0.403 | 71.051 | 1.00x |
| nested.json | orjson | 0.422 | 0.431 | 0.596 | 71.051 | 0.88x |
| nested.json | msgspec | 0.530 | 0.546 | 0.560 | 71.051 | 0.69x |
| nested.json | ujson | 1.213 | 1.218 | 1.236 | 71.051 | 0.31x |
| nested.json | json | 2.499 | 2.520 | 2.557 | 71.051 | 0.15x |
| wide_arrays.json | strata | 1.875 | 1.890 | 1.927 | 73.887 | 1.00x |
| wide_arrays.json | orjson | 2.159 | 2.170 | 2.186 | 73.887 | 0.87x |
| wide_arrays.json | msgspec | 3.056 | 3.073 | 3.110 | 73.887 | 0.61x |
| wide_arrays.json | ujson | 6.737 | 6.757 | 7.039 | 73.887 | 0.28x |
| wide_arrays.json | json | 16.906 | 16.956 | 16.989 | 73.887 | 0.11x |
| mixed.json | strata | 0.136 | 0.148 | 0.169 | 73.887 | 1.00x |
| mixed.json | orjson | 0.155 | 0.167 | 0.191 | 73.887 | 0.88x |
| mixed.json | msgspec | 0.173 | 0.186 | 0.208 | 73.887 | 0.79x |
| mixed.json | ujson | 0.341 | 0.344 | 0.362 | 73.887 | 0.43x |
| mixed.json | json | 0.584 | 0.632 | 0.664 | 73.887 | 0.23x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.060 | 0.061 | 0.063 | 70.711 | 1.00x |
| users.json $[*].id | jmespath | 0.492 | 0.500 | 0.556 | 70.711 | 0.12x |
| users.json $[*].id | jsonpath-ng | 2.638 | 2.804 | 2.905 | 70.711 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.387 | 0.445 | 0.458 | 72.680 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.040 | 3.129 | 3.173 | 72.680 | 0.14x |
| users.json $[*].orders[*].total | jsonpath-ng | 19.104 | 19.197 | 19.448 | 72.680 | 0.02x |
| users.json $..total | strata | 1.652 | 1.688 | 1.745 | 74.648 | 1.00x |
| users.json $..total | jsonpath-ng | 382.396 | 385.298 | 390.468 | 74.648 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.081 | 3.125 | 3.167 | 72.680 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.844 | 13.947 | 15.353 | 72.680 | 0.22x |
| users.json $[*].id | orjson+jsonpath-ng | 16.205 | 16.308 | 16.635 | 72.680 | 0.19x |
| users.json $[*].orders[*].total | strata | 3.207 | 3.326 | 3.361 | 73.852 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.926 | 16.728 | 16.970 | 73.852 | 0.20x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 35.398 | 36.448 | 36.873 | 73.852 | 0.09x |
| users.json $..total | strata | 12.244 | 12.533 | 13.363 | 73.871 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 398.699 | 405.477 | 412.627 | 73.871 | 0.03x |

