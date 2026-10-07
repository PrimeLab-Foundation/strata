# Benchmark results - ci-linux-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: baba505678994faef24df367a2dc4b93d5d4bef0
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-aarch64-with-glibc2.39
- machine: aarch64
- processor: aarch64
- compiler_flags: -fno-strict-overflow -Wsign-compare -DNDEBUG -g -O3 -Wall -fPIC -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -march=native -flto=thin -fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata -shared -Wl,--rpath=/opt/hostedtoolcache/Python/3.12.14/arm64/lib (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.740 | 8.815 | 10.630 | 57.277 | 1.00x |
| users.json | orjson | 11.667 | 11.746 | 13.322 | 57.277 | 0.75x |
| users.json | msgspec | 12.153 | 12.239 | 13.703 | 57.277 | 0.72x |
| users.json | ujson | 16.359 | 16.481 | 18.732 | 57.277 | 0.53x |
| users.json | pysimdjson | 16.300 | 16.484 | 18.261 | 57.277 | 0.53x |
| users.json | json | 20.557 | 20.590 | 21.301 | 57.277 | 0.43x |
| flat.json | strata | 0.808 | 0.816 | 0.824 | 67.980 | 1.00x |
| flat.json | orjson | 0.849 | 0.869 | 0.878 | 67.980 | 0.94x |
| flat.json | msgspec | 0.916 | 0.919 | 0.936 | 67.980 | 0.89x |
| flat.json | ujson | 1.416 | 1.425 | 1.444 | 67.980 | 0.57x |
| flat.json | pysimdjson | 1.483 | 1.499 | 1.508 | 67.980 | 0.54x |
| flat.json | json | 1.758 | 1.775 | 1.787 | 67.980 | 0.46x |
| nested.json | strata | 0.815 | 0.831 | 0.836 | 67.980 | 1.00x |
| nested.json | orjson | 0.882 | 0.894 | 0.920 | 67.980 | 0.93x |
| nested.json | msgspec | 1.005 | 1.009 | 1.020 | 67.980 | 0.82x |
| nested.json | ujson | 1.403 | 1.418 | 1.462 | 67.980 | 0.59x |
| nested.json | pysimdjson | 1.412 | 1.425 | 1.441 | 67.980 | 0.58x |
| nested.json | json | 1.968 | 1.980 | 2.011 | 67.980 | 0.42x |
| wide_arrays.json | strata | 3.855 | 3.876 | 3.887 | 69.559 | 1.00x |
| wide_arrays.json | orjson | 4.084 | 4.114 | 4.141 | 69.559 | 0.94x |
| wide_arrays.json | msgspec | 5.085 | 5.105 | 5.127 | 69.559 | 0.76x |
| wide_arrays.json | ujson | 6.488 | 6.524 | 6.563 | 69.559 | 0.59x |
| wide_arrays.json | pysimdjson | 5.287 | 5.307 | 5.338 | 69.559 | 0.73x |
| wide_arrays.json | json | 9.469 | 9.500 | 9.516 | 69.559 | 0.41x |
| mixed.json | strata | 0.188 | 0.189 | 0.214 | 69.559 | 1.00x |
| mixed.json | orjson | 0.209 | 0.214 | 0.240 | 69.559 | 0.88x |
| mixed.json | msgspec | 0.232 | 0.241 | 0.260 | 69.559 | 0.78x |
| mixed.json | ujson | 0.300 | 0.305 | 0.328 | 69.559 | 0.62x |
| mixed.json | pysimdjson | 0.291 | 0.293 | 0.322 | 69.559 | 0.64x |
| mixed.json | json | 0.449 | 0.459 | 0.481 | 69.559 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.915 | 1.925 | 1.934 | 56.387 | 1.00x |
| users.json | orjson | 2.574 | 2.586 | 2.597 | 56.387 | 0.74x |
| users.json | msgspec | 3.306 | 3.311 | 3.325 | 56.387 | 0.58x |
| users.json | ujson | 10.441 | 10.479 | 10.502 | 56.387 | 0.18x |
| users.json | json | 18.850 | 18.873 | 18.896 | 56.387 | 0.10x |
| flat.json | strata | 0.232 | 0.235 | 0.255 | 67.980 | 1.00x |
| flat.json | orjson | 0.302 | 0.303 | 0.321 | 67.980 | 0.78x |
| flat.json | msgspec | 0.384 | 0.390 | 0.401 | 67.980 | 0.60x |
| flat.json | ujson | 0.981 | 0.990 | 0.998 | 67.980 | 0.24x |
| flat.json | json | 1.694 | 1.705 | 1.715 | 67.980 | 0.14x |
| nested.json | strata | 0.217 | 0.219 | 0.236 | 67.980 | 1.00x |
| nested.json | orjson | 0.287 | 0.290 | 0.306 | 67.980 | 0.75x |
| nested.json | msgspec | 0.372 | 0.378 | 0.394 | 67.980 | 0.58x |
| nested.json | ujson | 1.071 | 1.076 | 1.091 | 67.980 | 0.20x |
| nested.json | json | 2.137 | 2.154 | 2.187 | 67.980 | 0.10x |
| wide_arrays.json | strata | 1.344 | 1.362 | 1.383 | 69.559 | 1.00x |
| wide_arrays.json | orjson | 1.607 | 1.617 | 1.624 | 69.559 | 0.84x |
| wide_arrays.json | msgspec | 2.369 | 2.387 | 2.416 | 69.559 | 0.57x |
| wide_arrays.json | ujson | 4.770 | 4.785 | 4.804 | 69.559 | 0.28x |
| wide_arrays.json | json | 13.530 | 13.558 | 13.580 | 69.559 | 0.10x |
| mixed.json | strata | 0.060 | 0.061 | 0.081 | 69.559 | 1.00x |
| mixed.json | orjson | 0.063 | 0.065 | 0.066 | 69.559 | 0.95x |
| mixed.json | msgspec | 0.078 | 0.079 | 0.080 | 69.559 | 0.78x |
| mixed.json | ujson | 0.232 | 0.234 | 0.256 | 69.559 | 0.26x |
| mixed.json | json | 0.472 | 0.475 | 0.495 | 69.559 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.928 | 8.969 | 9.707 | 68.422 | 1.00x |
| users.json | orjson | 11.715 | 11.886 | 12.214 | 68.422 | 0.75x |
| users.json | msgspec | 12.288 | 12.382 | 12.602 | 68.422 | 0.72x |
| users.json | ujson | 16.740 | 16.982 | 17.969 | 68.422 | 0.53x |
| users.json | json | 20.761 | 20.833 | 20.921 | 68.422 | 0.43x |
| flat.json | strata | 0.836 | 0.853 | 0.866 | 67.980 | 1.00x |
| flat.json | orjson | 0.938 | 0.954 | 0.959 | 67.980 | 0.89x |
| flat.json | msgspec | 0.983 | 0.991 | 1.004 | 67.980 | 0.86x |
| flat.json | ujson | 1.525 | 1.534 | 1.548 | 67.980 | 0.56x |
| flat.json | json | 1.839 | 1.850 | 1.864 | 67.980 | 0.46x |
| nested.json | strata | 0.850 | 0.868 | 0.877 | 67.980 | 1.00x |
| nested.json | orjson | 0.962 | 0.969 | 1.021 | 67.980 | 0.90x |
| nested.json | msgspec | 1.079 | 1.086 | 1.102 | 67.980 | 0.80x |
| nested.json | ujson | 1.496 | 1.509 | 1.519 | 67.980 | 0.58x |
| nested.json | json | 2.045 | 2.054 | 2.075 | 67.980 | 0.42x |
| wide_arrays.json | strata | 3.854 | 3.864 | 3.875 | 69.559 | 1.00x |
| wide_arrays.json | orjson | 4.061 | 4.094 | 4.114 | 69.559 | 0.94x |
| wide_arrays.json | msgspec | 5.102 | 5.112 | 5.138 | 69.559 | 0.76x |
| wide_arrays.json | ujson | 6.638 | 6.671 | 6.721 | 69.559 | 0.58x |
| wide_arrays.json | json | 9.515 | 9.575 | 9.622 | 69.559 | 0.40x |
| mixed.json | strata | 0.211 | 0.213 | 0.215 | 69.559 | 1.00x |
| mixed.json | orjson | 0.277 | 0.279 | 0.295 | 69.559 | 0.76x |
| mixed.json | msgspec | 0.314 | 0.317 | 0.322 | 69.559 | 0.67x |
| mixed.json | ujson | 0.377 | 0.387 | 0.402 | 69.559 | 0.55x |
| mixed.json | json | 0.512 | 0.516 | 0.527 | 69.559 | 0.41x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.263 | 9.322 | 9.382 | 67.977 | 1.00x |
| users.ndjson | orjson | 14.638 | 14.690 | 14.765 | 67.977 | 0.63x |
| users.ndjson | msgspec | 14.998 | 15.073 | 15.124 | 67.977 | 0.62x |
| users.ndjson | ujson | 19.423 | 19.613 | 19.757 | 67.977 | 0.48x |
| users.ndjson | json | 25.578 | 25.665 | 25.712 | 67.977 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.329 | 2.365 | 2.409 | 68.422 | 1.00x |
| users.json | orjson | 3.061 | 3.083 | 3.103 | 68.422 | 0.77x |
| users.json | msgspec | 3.782 | 3.819 | 3.866 | 68.422 | 0.62x |
| users.json | ujson | 11.038 | 11.117 | 11.448 | 68.422 | 0.21x |
| users.json | json | 19.526 | 19.616 | 19.848 | 68.422 | 0.12x |
| flat.json | strata | 0.385 | 0.412 | 0.428 | 67.980 | 1.00x |
| flat.json | orjson | 0.476 | 0.506 | 0.522 | 67.980 | 0.81x |
| flat.json | msgspec | 0.564 | 0.607 | 0.629 | 67.980 | 0.68x |
| flat.json | ujson | 1.196 | 1.218 | 1.242 | 67.980 | 0.34x |
| flat.json | json | 1.921 | 1.941 | 1.955 | 67.980 | 0.21x |
| nested.json | strata | 0.349 | 0.358 | 0.398 | 67.980 | 1.00x |
| nested.json | orjson | 0.455 | 0.472 | 0.500 | 67.980 | 0.76x |
| nested.json | msgspec | 0.541 | 0.557 | 0.584 | 67.980 | 0.64x |
| nested.json | ujson | 1.273 | 1.293 | 1.308 | 67.980 | 0.28x |
| nested.json | json | 2.335 | 2.357 | 2.401 | 67.980 | 0.15x |
| wide_arrays.json | strata | 1.673 | 1.716 | 1.755 | 69.559 | 1.00x |
| wide_arrays.json | orjson | 1.996 | 2.024 | 2.053 | 69.559 | 0.85x |
| wide_arrays.json | msgspec | 2.748 | 2.782 | 2.825 | 69.559 | 0.62x |
| wide_arrays.json | ujson | 5.203 | 5.236 | 5.309 | 69.559 | 0.33x |
| wide_arrays.json | json | 13.973 | 13.993 | 14.033 | 69.559 | 0.12x |
| mixed.json | strata | 0.167 | 0.174 | 0.208 | 69.559 | 1.00x |
| mixed.json | orjson | 0.194 | 0.201 | 0.226 | 69.559 | 0.87x |
| mixed.json | msgspec | 0.207 | 0.219 | 0.234 | 69.559 | 0.80x |
| mixed.json | ujson | 0.385 | 0.402 | 0.418 | 69.559 | 0.43x |
| mixed.json | json | 0.617 | 0.626 | 0.653 | 69.559 | 0.28x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.105 | 0.105 | 0.106 | 68.422 | 1.00x |
| users.json $[*].id | jmespath | 0.474 | 0.491 | 0.498 | 68.422 | 0.21x |
| users.json $[*].id | jsonpath-ng | 2.466 | 2.501 | 2.534 | 68.422 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.645 | 0.659 | 0.692 | 68.531 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.039 | 3.070 | 3.129 | 68.531 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.531 | 17.732 | 17.860 | 68.531 | 0.04x |
| users.json $..total | strata | 1.702 | 1.717 | 1.726 | 69.559 | 1.00x |
| users.json $..total | jsonpath-ng | 295.015 | 296.165 | 297.618 | 69.559 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.118 | 3.139 | 3.180 | 68.531 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.410 | 12.518 | 12.599 | 68.531 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 14.390 | 14.434 | 14.523 | 68.531 | 0.22x |
| users.json $[*].orders[*].total | strata | 3.290 | 3.312 | 3.344 | 69.559 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.181 | 15.251 | 15.352 | 69.559 | 0.22x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 33.897 | 34.127 | 34.239 | 69.559 | 0.10x |
| users.json $..total | strata | 11.055 | 11.252 | 11.437 | 69.613 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 308.342 | 310.563 | 312.767 | 69.613 | 0.04x |

