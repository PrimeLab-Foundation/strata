# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 9b6124a
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
| users.json | strata | 10.538 | 11.635 | 16.947 | 58.668 | 1.00x |
| users.json | orjson | 13.989 | 14.671 | 18.146 | 58.668 | 0.79x |
| users.json | msgspec | 13.742 | 14.225 | 17.280 | 58.668 | 0.82x |
| users.json | ujson | 20.012 | 21.270 | 26.643 | 58.668 | 0.55x |
| users.json | pysimdjson | 21.896 | 22.765 | 25.277 | 58.668 | 0.51x |
| users.json | json | 23.768 | 24.404 | 40.343 | 58.668 | 0.48x |
| flat.json | strata | 0.880 | 0.904 | 1.049 | 72.594 | 1.00x |
| flat.json | orjson | 0.993 | 1.022 | 1.090 | 72.594 | 0.88x |
| flat.json | msgspec | 1.043 | 1.063 | 1.204 | 72.594 | 0.85x |
| flat.json | ujson | 1.475 | 1.659 | 1.856 | 72.594 | 0.54x |
| flat.json | pysimdjson | 1.592 | 1.668 | 1.862 | 72.594 | 0.54x |
| flat.json | json | 1.941 | 1.970 | 2.024 | 72.594 | 0.46x |
| nested.json | strata | 0.822 | 0.864 | 1.153 | 72.594 | 1.00x |
| nested.json | orjson | 1.023 | 1.033 | 1.095 | 72.594 | 0.84x |
| nested.json | msgspec | 1.049 | 1.075 | 1.119 | 72.594 | 0.80x |
| nested.json | ujson | 1.590 | 1.682 | 1.822 | 72.594 | 0.51x |
| nested.json | pysimdjson | 1.459 | 1.524 | 1.625 | 72.594 | 0.57x |
| nested.json | json | 2.053 | 2.105 | 2.124 | 72.594 | 0.41x |
| wide_arrays.json | strata | 4.729 | 4.784 | 5.708 | 75.492 | 1.00x |
| wide_arrays.json | orjson | 5.198 | 5.404 | 6.317 | 75.492 | 0.89x |
| wide_arrays.json | msgspec | 5.662 | 5.946 | 7.363 | 75.492 | 0.80x |
| wide_arrays.json | ujson | 7.348 | 7.527 | 9.612 | 75.492 | 0.64x |
| wide_arrays.json | pysimdjson | 6.268 | 6.457 | 8.107 | 75.492 | 0.74x |
| wide_arrays.json | json | 9.829 | 9.999 | 10.774 | 75.492 | 0.48x |
| mixed.json | strata | 0.196 | 0.217 | 0.255 | 75.555 | 1.00x |
| mixed.json | orjson | 0.235 | 0.249 | 0.480 | 75.555 | 0.87x |
| mixed.json | msgspec | 0.250 | 0.266 | 0.543 | 75.555 | 0.82x |
| mixed.json | ujson | 0.336 | 0.352 | 0.676 | 75.555 | 0.62x |
| mixed.json | pysimdjson | 0.318 | 0.334 | 0.611 | 75.555 | 0.65x |
| mixed.json | json | 0.485 | 0.516 | 1.032 | 75.555 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.553 | 2.653 | 2.776 | 57.754 | 1.00x |
| users.json | orjson | 2.969 | 3.031 | 3.221 | 57.754 | 0.88x |
| users.json | msgspec | 3.981 | 4.075 | 4.304 | 57.754 | 0.65x |
| users.json | ujson | 15.219 | 15.869 | 17.412 | 57.754 | 0.17x |
| users.json | json | 22.915 | 23.115 | 31.422 | 57.754 | 0.11x |
| flat.json | strata | 0.269 | 0.281 | 0.299 | 72.594 | 1.00x |
| flat.json | orjson | 0.330 | 0.347 | 0.373 | 72.594 | 0.81x |
| flat.json | msgspec | 0.432 | 0.439 | 0.458 | 72.594 | 0.64x |
| flat.json | ujson | 1.282 | 1.305 | 1.372 | 72.594 | 0.22x |
| flat.json | json | 1.855 | 1.872 | 2.186 | 72.594 | 0.15x |
| nested.json | strata | 0.260 | 0.263 | 0.291 | 72.594 | 1.00x |
| nested.json | orjson | 0.295 | 0.301 | 0.319 | 72.594 | 0.87x |
| nested.json | msgspec | 0.404 | 0.409 | 0.422 | 72.594 | 0.64x |
| nested.json | ujson | 1.392 | 1.411 | 1.429 | 72.594 | 0.19x |
| nested.json | json | 2.401 | 2.418 | 2.453 | 72.594 | 0.11x |
| wide_arrays.json | strata | 1.588 | 1.608 | 1.892 | 75.492 | 1.00x |
| wide_arrays.json | orjson | 1.828 | 1.857 | 1.870 | 75.492 | 0.87x |
| wide_arrays.json | msgspec | 2.707 | 2.732 | 2.881 | 75.492 | 0.59x |
| wide_arrays.json | ujson | 8.562 | 8.655 | 8.896 | 75.492 | 0.19x |
| wide_arrays.json | json | 16.514 | 16.682 | 17.222 | 75.492 | 0.10x |
| mixed.json | strata | 0.065 | 0.069 | 0.104 | 75.555 | 1.00x |
| mixed.json | orjson | 0.065 | 0.067 | 0.131 | 75.555 | 1.02x |
| mixed.json | msgspec | 0.086 | 0.090 | 0.143 | 75.555 | 0.76x |
| mixed.json | ujson | 0.308 | 0.323 | 0.627 | 75.555 | 0.21x |
| mixed.json | json | 0.520 | 0.527 | 1.127 | 75.555 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.868 | 12.835 | 13.671 | 74.023 | 1.00x |
| users.json | orjson | 14.042 | 15.175 | 17.244 | 74.023 | 0.85x |
| users.json | msgspec | 14.437 | 15.091 | 15.911 | 74.023 | 0.85x |
| users.json | ujson | 20.597 | 22.503 | 24.943 | 74.023 | 0.57x |
| users.json | json | 23.964 | 25.100 | 27.339 | 74.023 | 0.51x |
| flat.json | strata | 0.946 | 0.972 | 1.044 | 72.594 | 1.00x |
| flat.json | orjson | 1.085 | 1.121 | 1.225 | 72.594 | 0.87x |
| flat.json | msgspec | 1.140 | 1.183 | 1.248 | 72.594 | 0.82x |
| flat.json | ujson | 1.811 | 1.887 | 2.336 | 72.594 | 0.52x |
| flat.json | json | 2.018 | 2.050 | 2.120 | 72.594 | 0.47x |
| nested.json | strata | 0.870 | 0.883 | 1.491 | 72.594 | 1.00x |
| nested.json | orjson | 1.079 | 1.117 | 1.300 | 72.594 | 0.79x |
| nested.json | msgspec | 1.110 | 1.150 | 1.302 | 72.594 | 0.77x |
| nested.json | ujson | 1.603 | 1.698 | 1.782 | 72.594 | 0.52x |
| nested.json | json | 2.128 | 2.156 | 2.246 | 72.594 | 0.41x |
| wide_arrays.json | strata | 4.847 | 5.050 | 5.075 | 75.555 | 1.00x |
| wide_arrays.json | orjson | 5.409 | 5.556 | 5.826 | 75.555 | 0.91x |
| wide_arrays.json | msgspec | 6.034 | 6.134 | 6.402 | 75.555 | 0.82x |
| wide_arrays.json | ujson | 7.790 | 7.966 | 8.141 | 75.555 | 0.63x |
| wide_arrays.json | json | 10.106 | 10.256 | 10.479 | 75.555 | 0.49x |
| mixed.json | strata | 0.217 | 0.224 | 0.243 | 75.555 | 1.00x |
| mixed.json | orjson | 0.277 | 0.294 | 0.311 | 75.555 | 0.76x |
| mixed.json | msgspec | 0.288 | 0.293 | 0.323 | 75.555 | 0.77x |
| mixed.json | ujson | 0.380 | 0.391 | 0.405 | 75.555 | 0.57x |
| mixed.json | json | 0.521 | 0.530 | 0.576 | 75.555 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 11.659 | 12.862 | 14.383 | 72.594 | 1.00x |
| users.ndjson | orjson | 17.047 | 19.695 | 22.678 | 72.594 | 0.65x |
| users.ndjson | msgspec | 16.780 | 19.114 | 20.194 | 72.594 | 0.67x |
| users.ndjson | ujson | 22.153 | 25.409 | 27.416 | 72.594 | 0.51x |
| users.ndjson | json | 31.258 | 33.223 | 36.247 | 72.594 | 0.39x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.142 | 3.316 | 3.641 | 71.070 | 1.00x |
| users.json | orjson | 3.650 | 4.039 | 4.241 | 71.070 | 0.82x |
| users.json | msgspec | 4.672 | 5.036 | 5.450 | 71.070 | 0.66x |
| users.json | ujson | 16.435 | 16.907 | 21.538 | 71.070 | 0.20x |
| users.json | json | 23.559 | 24.142 | 38.126 | 71.070 | 0.14x |
| flat.json | strata | 0.457 | 0.493 | 0.561 | 72.594 | 1.00x |
| flat.json | orjson | 0.512 | 0.543 | 0.640 | 72.594 | 0.91x |
| flat.json | msgspec | 0.619 | 0.662 | 0.764 | 72.594 | 0.74x |
| flat.json | ujson | 1.523 | 1.573 | 1.894 | 72.594 | 0.31x |
| flat.json | json | 2.112 | 2.161 | 2.240 | 72.594 | 0.23x |
| nested.json | strata | 0.384 | 0.426 | 0.467 | 72.594 | 1.00x |
| nested.json | orjson | 0.449 | 0.475 | 0.512 | 72.594 | 0.90x |
| nested.json | msgspec | 0.563 | 0.582 | 0.622 | 72.594 | 0.73x |
| nested.json | ujson | 1.558 | 1.615 | 1.696 | 72.594 | 0.26x |
| nested.json | json | 2.545 | 2.624 | 2.673 | 72.594 | 0.16x |
| wide_arrays.json | strata | 2.100 | 2.123 | 2.236 | 75.555 | 1.00x |
| wide_arrays.json | orjson | 2.378 | 2.413 | 2.484 | 75.555 | 0.88x |
| wide_arrays.json | msgspec | 3.237 | 3.271 | 3.338 | 75.555 | 0.65x |
| wide_arrays.json | ujson | 9.402 | 9.498 | 9.770 | 75.555 | 0.22x |
| wide_arrays.json | json | 17.421 | 17.568 | 17.868 | 75.555 | 0.12x |
| mixed.json | strata | 0.158 | 0.162 | 0.182 | 75.555 | 1.00x |
| mixed.json | orjson | 0.177 | 0.193 | 0.218 | 75.555 | 0.84x |
| mixed.json | msgspec | 0.196 | 0.207 | 0.224 | 75.555 | 0.78x |
| mixed.json | ujson | 0.429 | 0.433 | 0.484 | 75.555 | 0.37x |
| mixed.json | json | 0.634 | 0.654 | 0.678 | 75.555 | 0.25x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.076 | 0.084 | 0.095 | 71.070 | 1.00x |
| users.json $[*].id | jmespath | 0.521 | 0.533 | 0.549 | 71.070 | 0.16x |
| users.json $[*].id | jsonpath-ng | 3.007 | 3.340 | 3.565 | 71.070 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.470 | 0.502 | 0.688 | 71.094 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.212 | 3.288 | 3.571 | 71.094 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 21.722 | 22.134 | 23.398 | 71.094 | 0.02x |
| users.json $..total | strata | 1.716 | 1.865 | 2.060 | 73.086 | 1.00x |
| users.json $..total | jsonpath-ng | 396.030 | 398.973 | 412.378 | 73.086 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.306 | 3.321 | 3.368 | 71.094 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.793 | 15.872 | 18.895 | 71.094 | 0.21x |
| users.json $[*].id | orjson+jsonpath-ng | 17.871 | 19.156 | 19.670 | 71.094 | 0.17x |
| users.json $[*].orders[*].total | strata | 3.551 | 3.573 | 3.594 | 72.305 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.376 | 19.508 | 20.246 | 72.305 | 0.18x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 42.402 | 43.414 | 45.074 | 72.305 | 0.08x |
| users.json $..total | strata | 13.725 | 15.356 | 20.127 | 74.445 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 420.671 | 423.196 | 428.809 | 74.445 | 0.04x |

