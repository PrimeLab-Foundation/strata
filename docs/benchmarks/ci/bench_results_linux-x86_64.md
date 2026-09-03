# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 571381e
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
| users.json | strata | 10.541 | 11.587 | 20.621 | 60.965 | 1.00x |
| users.json | orjson | 13.676 | 14.368 | 24.939 | 60.965 | 0.81x |
| users.json | msgspec | 13.402 | 14.370 | 27.338 | 60.965 | 0.81x |
| users.json | ujson | 19.529 | 21.878 | 33.326 | 60.965 | 0.53x |
| users.json | pysimdjson | 19.730 | 21.645 | 22.832 | 60.965 | 0.54x |
| users.json | json | 22.938 | 23.879 | 27.690 | 60.965 | 0.49x |
| flat.json | strata | 0.869 | 0.882 | 0.906 | 68.043 | 1.00x |
| flat.json | orjson | 0.967 | 0.987 | 1.001 | 68.043 | 0.89x |
| flat.json | msgspec | 1.018 | 1.065 | 1.082 | 68.043 | 0.83x |
| flat.json | ujson | 1.493 | 1.652 | 1.717 | 68.043 | 0.53x |
| flat.json | pysimdjson | 1.543 | 1.601 | 1.658 | 68.043 | 0.55x |
| flat.json | json | 1.928 | 1.947 | 1.978 | 68.043 | 0.45x |
| nested.json | strata | 0.811 | 0.827 | 0.862 | 68.043 | 1.00x |
| nested.json | orjson | 1.018 | 1.031 | 1.078 | 68.043 | 0.80x |
| nested.json | msgspec | 1.027 | 1.047 | 1.056 | 68.043 | 0.79x |
| nested.json | ujson | 1.480 | 1.557 | 1.622 | 68.043 | 0.53x |
| nested.json | pysimdjson | 1.407 | 1.435 | 1.498 | 68.043 | 0.58x |
| nested.json | json | 2.041 | 2.065 | 2.134 | 68.043 | 0.40x |
| wide_arrays.json | strata | 4.662 | 4.900 | 5.119 | 74.832 | 1.00x |
| wide_arrays.json | orjson | 5.076 | 5.438 | 5.769 | 74.832 | 0.90x |
| wide_arrays.json | msgspec | 5.638 | 5.808 | 6.120 | 74.832 | 0.84x |
| wide_arrays.json | ujson | 7.249 | 7.488 | 7.819 | 74.832 | 0.65x |
| wide_arrays.json | pysimdjson | 6.091 | 6.362 | 6.822 | 74.832 | 0.77x |
| wide_arrays.json | json | 9.691 | 10.171 | 10.490 | 74.832 | 0.48x |
| mixed.json | strata | 0.197 | 0.201 | 0.280 | 76.586 | 1.00x |
| mixed.json | orjson | 0.228 | 0.236 | 0.287 | 76.586 | 0.85x |
| mixed.json | msgspec | 0.237 | 0.244 | 0.258 | 76.586 | 0.82x |
| mixed.json | ujson | 0.299 | 0.323 | 0.545 | 76.586 | 0.62x |
| mixed.json | pysimdjson | 0.300 | 0.307 | 0.429 | 76.586 | 0.65x |
| mixed.json | json | 0.475 | 0.487 | 0.806 | 76.586 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.492 | 2.557 | 2.897 | 59.383 | 1.00x |
| users.json | orjson | 2.891 | 2.936 | 3.182 | 59.383 | 0.87x |
| users.json | msgspec | 3.896 | 4.004 | 4.404 | 59.383 | 0.64x |
| users.json | ujson | 14.912 | 15.086 | 15.449 | 59.383 | 0.17x |
| users.json | json | 21.547 | 22.093 | 22.498 | 59.383 | 0.12x |
| flat.json | strata | 0.268 | 0.277 | 0.295 | 68.043 | 1.00x |
| flat.json | orjson | 0.331 | 0.333 | 0.358 | 68.043 | 0.83x |
| flat.json | msgspec | 0.436 | 0.448 | 0.499 | 68.043 | 0.62x |
| flat.json | ujson | 1.298 | 1.321 | 1.347 | 68.043 | 0.21x |
| flat.json | json | 1.842 | 1.875 | 1.982 | 68.043 | 0.15x |
| nested.json | strata | 0.252 | 0.261 | 0.265 | 68.043 | 1.00x |
| nested.json | orjson | 0.289 | 0.293 | 0.323 | 68.043 | 0.89x |
| nested.json | msgspec | 0.409 | 0.423 | 0.437 | 68.043 | 0.62x |
| nested.json | ujson | 1.445 | 1.464 | 1.482 | 68.043 | 0.18x |
| nested.json | json | 2.369 | 2.401 | 2.454 | 68.043 | 0.11x |
| wide_arrays.json | strata | 1.593 | 1.604 | 1.643 | 76.523 | 1.00x |
| wide_arrays.json | orjson | 1.812 | 1.826 | 1.964 | 76.523 | 0.88x |
| wide_arrays.json | msgspec | 2.688 | 2.714 | 2.737 | 76.523 | 0.59x |
| wide_arrays.json | ujson | 8.554 | 8.706 | 9.749 | 76.523 | 0.18x |
| wide_arrays.json | json | 16.565 | 16.700 | 17.179 | 76.523 | 0.10x |
| mixed.json | strata | 0.066 | 0.069 | 0.086 | 76.586 | 1.00x |
| mixed.json | orjson | 0.064 | 0.067 | 0.095 | 76.586 | 1.03x |
| mixed.json | msgspec | 0.086 | 0.089 | 0.127 | 76.586 | 0.77x |
| mixed.json | ujson | 0.300 | 0.307 | 0.319 | 76.586 | 0.22x |
| mixed.json | json | 0.508 | 0.527 | 0.572 | 76.586 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.299 | 11.749 | 13.710 | 73.609 | 1.00x |
| users.json | orjson | 13.747 | 14.855 | 15.649 | 73.609 | 0.79x |
| users.json | msgspec | 13.606 | 14.653 | 22.635 | 73.609 | 0.80x |
| users.json | ujson | 20.549 | 22.472 | 29.458 | 73.609 | 0.52x |
| users.json | json | 23.382 | 24.033 | 31.193 | 73.609 | 0.49x |
| flat.json | strata | 0.896 | 0.918 | 0.946 | 68.043 | 1.00x |
| flat.json | orjson | 1.037 | 1.073 | 1.108 | 68.043 | 0.86x |
| flat.json | msgspec | 1.096 | 1.117 | 1.228 | 68.043 | 0.82x |
| flat.json | ujson | 1.615 | 1.677 | 1.748 | 68.043 | 0.55x |
| flat.json | json | 1.976 | 1.995 | 2.092 | 68.043 | 0.46x |
| nested.json | strata | 0.836 | 0.852 | 0.974 | 68.043 | 1.00x |
| nested.json | orjson | 1.074 | 1.091 | 1.105 | 68.043 | 0.78x |
| nested.json | msgspec | 1.089 | 1.101 | 1.177 | 68.043 | 0.77x |
| nested.json | ujson | 1.550 | 1.571 | 1.833 | 68.043 | 0.54x |
| nested.json | json | 2.099 | 2.118 | 2.210 | 68.043 | 0.40x |
| wide_arrays.json | strata | 4.842 | 4.951 | 5.165 | 76.586 | 1.00x |
| wide_arrays.json | orjson | 5.288 | 5.466 | 5.683 | 76.586 | 0.91x |
| wide_arrays.json | msgspec | 5.908 | 6.102 | 6.351 | 76.586 | 0.81x |
| wide_arrays.json | ujson | 7.537 | 7.824 | 8.034 | 76.586 | 0.63x |
| wide_arrays.json | json | 9.919 | 10.136 | 10.565 | 76.586 | 0.49x |
| mixed.json | strata | 0.213 | 0.225 | 0.240 | 76.586 | 1.00x |
| mixed.json | orjson | 0.276 | 0.285 | 0.321 | 76.586 | 0.79x |
| mixed.json | msgspec | 0.278 | 0.293 | 0.303 | 76.586 | 0.77x |
| mixed.json | ujson | 0.362 | 0.384 | 0.407 | 76.586 | 0.59x |
| mixed.json | json | 0.511 | 0.529 | 0.548 | 76.586 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 11.907 | 12.948 | 13.680 | 68.043 | 1.00x |
| users.ndjson | orjson | 17.500 | 19.194 | 19.597 | 68.043 | 0.67x |
| users.ndjson | msgspec | 17.924 | 18.403 | 21.819 | 68.043 | 0.70x |
| users.ndjson | ujson | 23.225 | 25.234 | 26.384 | 68.043 | 0.51x |
| users.ndjson | json | 31.086 | 32.277 | 32.937 | 68.043 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.148 | 3.210 | 3.374 | 69.672 | 1.00x |
| users.json | orjson | 3.607 | 3.700 | 3.839 | 69.672 | 0.87x |
| users.json | msgspec | 4.664 | 4.763 | 4.933 | 69.672 | 0.67x |
| users.json | ujson | 15.711 | 16.213 | 29.097 | 69.672 | 0.20x |
| users.json | json | 22.764 | 23.137 | 24.127 | 69.672 | 0.14x |
| flat.json | strata | 0.426 | 0.451 | 0.464 | 68.043 | 1.00x |
| flat.json | orjson | 0.498 | 0.531 | 0.568 | 68.043 | 0.85x |
| flat.json | msgspec | 0.621 | 0.648 | 0.682 | 68.043 | 0.70x |
| flat.json | ujson | 1.475 | 1.509 | 2.629 | 68.043 | 0.30x |
| flat.json | json | 2.082 | 2.116 | 2.157 | 68.043 | 0.21x |
| nested.json | strata | 0.373 | 0.404 | 0.484 | 68.043 | 1.00x |
| nested.json | orjson | 0.437 | 0.455 | 0.498 | 68.043 | 0.89x |
| nested.json | msgspec | 0.538 | 0.567 | 0.585 | 68.043 | 0.71x |
| nested.json | ujson | 1.583 | 1.615 | 1.661 | 68.043 | 0.25x |
| nested.json | json | 2.511 | 2.577 | 2.644 | 68.043 | 0.16x |
| wide_arrays.json | strata | 2.041 | 2.096 | 2.217 | 76.586 | 1.00x |
| wide_arrays.json | orjson | 2.312 | 2.385 | 2.761 | 76.586 | 0.88x |
| wide_arrays.json | msgspec | 3.163 | 3.224 | 3.258 | 76.586 | 0.65x |
| wide_arrays.json | ujson | 9.277 | 9.342 | 9.708 | 76.586 | 0.22x |
| wide_arrays.json | json | 17.035 | 17.335 | 18.850 | 76.586 | 0.12x |
| mixed.json | strata | 0.154 | 0.168 | 0.189 | 76.586 | 1.00x |
| mixed.json | orjson | 0.170 | 0.177 | 0.218 | 76.586 | 0.95x |
| mixed.json | msgspec | 0.188 | 0.200 | 0.225 | 76.586 | 0.84x |
| mixed.json | ujson | 0.419 | 0.434 | 0.475 | 76.586 | 0.39x |
| mixed.json | json | 0.623 | 0.645 | 0.671 | 76.586 | 0.26x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.071 | 0.077 | 0.091 | 69.672 | 1.00x |
| users.json $[*].id | jmespath | 0.499 | 0.519 | 0.580 | 69.672 | 0.15x |
| users.json $[*].id | jsonpath-ng | 2.951 | 3.070 | 3.251 | 69.672 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.460 | 0.468 | 0.505 | 70.656 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.075 | 3.183 | 3.229 | 70.656 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 20.064 | 21.276 | 22.764 | 70.656 | 0.02x |
| users.json $..total | strata | 1.660 | 1.706 | 1.992 | 72.625 | 1.00x |
| users.json $..total | jsonpath-ng | 378.584 | 380.752 | 389.098 | 72.625 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.284 | 3.301 | 3.319 | 70.656 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.234 | 14.860 | 16.364 | 70.656 | 0.22x |
| users.json $[*].id | orjson+jsonpath-ng | 16.736 | 18.317 | 21.328 | 70.656 | 0.18x |
| users.json $[*].orders[*].total | strata | 3.527 | 3.549 | 3.620 | 71.844 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.885 | 18.530 | 22.953 | 71.844 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 38.526 | 40.886 | 43.619 | 71.844 | 0.09x |
| users.json $..total | strata | 13.068 | 14.924 | 16.732 | 71.863 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 403.653 | 411.980 | 416.755 | 71.863 | 0.04x |

