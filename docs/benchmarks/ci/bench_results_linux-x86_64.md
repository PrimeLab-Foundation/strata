# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 85e2353
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
| users.json | strata | 9.622 | 10.208 | 13.477 | 61.008 | 1.00x |
| users.json | orjson | 13.662 | 14.029 | 21.102 | 61.008 | 0.73x |
| users.json | msgspec | 13.978 | 14.620 | 22.010 | 61.008 | 0.70x |
| users.json | ujson | 18.648 | 19.324 | 25.180 | 61.008 | 0.53x |
| users.json | pysimdjson | 18.449 | 18.866 | 20.832 | 61.008 | 0.54x |
| users.json | json | 21.326 | 21.797 | 32.446 | 61.008 | 0.47x |
| flat.json | strata | 0.842 | 0.876 | 0.912 | 71.539 | 1.00x |
| flat.json | orjson | 1.001 | 1.067 | 1.500 | 71.539 | 0.82x |
| flat.json | msgspec | 1.007 | 1.023 | 1.495 | 71.539 | 0.86x |
| flat.json | ujson | 1.413 | 1.445 | 1.487 | 71.539 | 0.61x |
| flat.json | pysimdjson | 1.538 | 1.610 | 1.660 | 71.539 | 0.54x |
| flat.json | json | 1.739 | 1.806 | 3.257 | 71.539 | 0.49x |
| nested.json | strata | 0.739 | 0.765 | 1.276 | 71.539 | 1.00x |
| nested.json | orjson | 0.881 | 0.916 | 0.936 | 71.539 | 0.83x |
| nested.json | msgspec | 0.929 | 0.944 | 0.984 | 71.539 | 0.81x |
| nested.json | ujson | 1.323 | 1.368 | 1.485 | 71.539 | 0.56x |
| nested.json | pysimdjson | 1.263 | 1.302 | 2.041 | 71.539 | 0.59x |
| nested.json | json | 1.807 | 1.876 | 1.918 | 71.539 | 0.41x |
| wide_arrays.json | strata | 4.188 | 4.323 | 4.443 | 76.129 | 1.00x |
| wide_arrays.json | orjson | 4.882 | 5.043 | 5.099 | 76.129 | 0.86x |
| wide_arrays.json | msgspec | 5.354 | 5.439 | 5.681 | 76.129 | 0.79x |
| wide_arrays.json | ujson | 6.950 | 7.168 | 9.080 | 76.129 | 0.60x |
| wide_arrays.json | pysimdjson | 5.434 | 5.599 | 6.881 | 76.129 | 0.77x |
| wide_arrays.json | json | 8.836 | 9.297 | 11.465 | 76.129 | 0.46x |
| mixed.json | strata | 0.188 | 0.192 | 0.328 | 76.191 | 1.00x |
| mixed.json | orjson | 0.228 | 0.233 | 0.334 | 76.191 | 0.83x |
| mixed.json | msgspec | 0.240 | 0.257 | 0.383 | 76.191 | 0.75x |
| mixed.json | ujson | 0.302 | 0.319 | 0.330 | 76.191 | 0.60x |
| mixed.json | pysimdjson | 0.296 | 0.306 | 0.314 | 76.191 | 0.63x |
| mixed.json | json | 0.462 | 0.482 | 0.618 | 76.191 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.120 | 2.157 | 2.236 | 59.367 | 1.00x |
| users.json | orjson | 2.194 | 2.355 | 2.546 | 59.367 | 0.92x |
| users.json | msgspec | 3.733 | 3.835 | 4.048 | 59.367 | 0.56x |
| users.json | ujson | 13.511 | 13.754 | 14.098 | 59.367 | 0.16x |
| users.json | json | 19.937 | 20.759 | 21.252 | 59.367 | 0.10x |
| flat.json | strata | 0.266 | 0.279 | 0.297 | 71.539 | 1.00x |
| flat.json | orjson | 0.263 | 0.277 | 0.359 | 71.539 | 1.01x |
| flat.json | msgspec | 0.413 | 0.437 | 0.676 | 71.539 | 0.64x |
| flat.json | ujson | 1.171 | 1.208 | 1.262 | 71.539 | 0.23x |
| flat.json | json | 1.648 | 1.781 | 1.832 | 71.539 | 0.16x |
| nested.json | strata | 0.184 | 0.194 | 0.323 | 71.539 | 1.00x |
| nested.json | orjson | 0.236 | 0.253 | 0.283 | 71.539 | 0.77x |
| nested.json | msgspec | 0.368 | 0.380 | 0.424 | 71.539 | 0.51x |
| nested.json | ujson | 1.188 | 1.227 | 1.270 | 71.539 | 0.16x |
| nested.json | json | 2.182 | 2.215 | 3.118 | 71.539 | 0.09x |
| wide_arrays.json | strata | 1.454 | 1.525 | 2.254 | 76.129 | 1.00x |
| wide_arrays.json | orjson | 1.369 | 1.424 | 1.477 | 76.129 | 1.07x |
| wide_arrays.json | msgspec | 2.515 | 2.606 | 3.193 | 76.129 | 0.59x |
| wide_arrays.json | ujson | 7.534 | 7.745 | 8.957 | 76.129 | 0.20x |
| wide_arrays.json | json | 14.683 | 15.043 | 17.432 | 76.129 | 0.10x |
| mixed.json | strata | 0.058 | 0.062 | 0.078 | 76.191 | 1.00x |
| mixed.json | orjson | 0.056 | 0.057 | 0.061 | 76.191 | 1.08x |
| mixed.json | msgspec | 0.075 | 0.079 | 0.086 | 76.191 | 0.79x |
| mixed.json | ujson | 0.282 | 0.294 | 0.405 | 76.191 | 0.21x |
| mixed.json | json | 0.495 | 0.517 | 0.949 | 76.191 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.079 | 10.493 | 11.247 | 76.152 | 1.00x |
| users.json | orjson | 14.042 | 14.537 | 15.066 | 76.152 | 0.72x |
| users.json | msgspec | 14.409 | 14.584 | 15.367 | 76.152 | 0.72x |
| users.json | ujson | 19.120 | 20.371 | 21.107 | 76.152 | 0.52x |
| users.json | json | 21.487 | 22.337 | 22.909 | 76.152 | 0.47x |
| flat.json | strata | 0.860 | 0.884 | 0.919 | 71.539 | 1.00x |
| flat.json | orjson | 1.066 | 1.115 | 1.221 | 71.539 | 0.79x |
| flat.json | msgspec | 1.056 | 1.097 | 1.620 | 71.539 | 0.81x |
| flat.json | ujson | 1.381 | 1.553 | 1.613 | 71.539 | 0.57x |
| flat.json | json | 1.829 | 1.864 | 1.936 | 71.539 | 0.47x |
| nested.json | strata | 0.751 | 0.768 | 1.274 | 71.539 | 1.00x |
| nested.json | orjson | 0.939 | 0.966 | 1.460 | 71.539 | 0.79x |
| nested.json | msgspec | 0.962 | 1.009 | 1.083 | 71.539 | 0.76x |
| nested.json | ujson | 1.391 | 1.424 | 2.341 | 71.539 | 0.54x |
| nested.json | json | 1.871 | 1.898 | 1.989 | 71.539 | 0.40x |
| wide_arrays.json | strata | 4.294 | 4.452 | 4.652 | 76.191 | 1.00x |
| wide_arrays.json | orjson | 4.958 | 5.134 | 5.492 | 76.191 | 0.87x |
| wide_arrays.json | msgspec | 5.645 | 5.721 | 6.880 | 76.191 | 0.78x |
| wide_arrays.json | ujson | 7.312 | 7.492 | 7.798 | 76.191 | 0.59x |
| wide_arrays.json | json | 9.181 | 9.450 | 9.844 | 76.191 | 0.47x |
| mixed.json | strata | 0.207 | 0.215 | 0.343 | 76.191 | 1.00x |
| mixed.json | orjson | 0.275 | 0.296 | 0.331 | 76.191 | 0.73x |
| mixed.json | msgspec | 0.285 | 0.308 | 0.460 | 76.191 | 0.70x |
| mixed.json | ujson | 0.370 | 0.393 | 0.429 | 76.191 | 0.55x |
| mixed.json | json | 0.499 | 0.526 | 0.561 | 76.191 | 0.41x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.995 | 10.309 | 15.193 | 71.539 | 1.00x |
| users.ndjson | orjson | 16.055 | 16.522 | 19.381 | 71.539 | 0.62x |
| users.ndjson | msgspec | 16.146 | 16.651 | 18.958 | 71.539 | 0.62x |
| users.ndjson | ujson | 20.788 | 21.185 | 23.808 | 71.539 | 0.49x |
| users.ndjson | json | 26.185 | 26.805 | 29.436 | 71.539 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.574 | 2.675 | 2.760 | 73.199 | 1.00x |
| users.json | orjson | 2.839 | 2.860 | 3.071 | 73.199 | 0.94x |
| users.json | msgspec | 4.249 | 4.424 | 4.637 | 73.199 | 0.60x |
| users.json | ujson | 13.754 | 14.490 | 15.004 | 73.199 | 0.18x |
| users.json | json | 20.864 | 21.345 | 21.879 | 73.199 | 0.13x |
| flat.json | strata | 0.377 | 0.405 | 2.338 | 71.539 | 1.00x |
| flat.json | orjson | 0.394 | 0.421 | 0.591 | 71.539 | 0.96x |
| flat.json | msgspec | 0.559 | 0.581 | 0.679 | 71.539 | 0.70x |
| flat.json | ujson | 1.332 | 1.359 | 1.860 | 71.539 | 0.30x |
| flat.json | json | 1.898 | 1.930 | 2.529 | 71.539 | 0.21x |
| nested.json | strata | 0.278 | 0.289 | 0.318 | 71.539 | 1.00x |
| nested.json | orjson | 0.335 | 0.367 | 0.388 | 71.539 | 0.79x |
| nested.json | msgspec | 0.479 | 0.496 | 0.705 | 71.539 | 0.58x |
| nested.json | ujson | 1.345 | 1.403 | 2.285 | 71.539 | 0.21x |
| nested.json | json | 2.322 | 2.398 | 2.480 | 71.539 | 0.12x |
| wide_arrays.json | strata | 1.839 | 1.900 | 1.994 | 76.191 | 1.00x |
| wide_arrays.json | orjson | 1.788 | 1.903 | 2.724 | 76.191 | 1.00x |
| wide_arrays.json | msgspec | 2.937 | 3.021 | 43.070 | 76.191 | 0.63x |
| wide_arrays.json | ujson | 7.805 | 8.190 | 38.391 | 76.191 | 0.23x |
| wide_arrays.json | json | 15.323 | 34.090 | 65.443 | 76.191 | 0.06x |
| mixed.json | strata | 0.120 | 0.128 | 0.197 | 76.191 | 1.00x |
| mixed.json | orjson | 0.136 | 0.141 | 0.173 | 76.191 | 0.90x |
| mixed.json | msgspec | 0.154 | 0.159 | 0.187 | 76.191 | 0.80x |
| mixed.json | ujson | 0.373 | 0.388 | 0.418 | 76.191 | 0.33x |
| mixed.json | json | 0.584 | 0.603 | 0.639 | 76.191 | 0.21x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.064 | 0.065 | 0.070 | 73.199 | 1.00x |
| users.json $[*].id | jmespath | 0.459 | 0.475 | 0.504 | 73.199 | 0.14x |
| users.json $[*].id | jsonpath-ng | 2.338 | 2.395 | 2.549 | 73.199 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.470 | 0.487 | 0.505 | 73.199 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.868 | 2.925 | 3.063 | 73.199 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 15.714 | 16.219 | 16.815 | 73.199 | 0.03x |
| users.json $..total | strata | 1.699 | 1.771 | 1.930 | 74.184 | 1.00x |
| users.json $..total | jsonpath-ng | 314.809 | 318.140 | 322.334 | 74.184 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 2.944 | 3.036 | 3.138 | 73.199 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.987 | 14.358 | 14.847 | 73.199 | 0.21x |
| users.json $[*].id | orjson+jsonpath-ng | 16.109 | 16.427 | 16.810 | 73.199 | 0.18x |
| users.json $[*].orders[*].total | strata | 3.109 | 3.224 | 3.283 | 74.184 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 16.838 | 16.957 | 17.054 | 74.184 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 35.243 | 35.533 | 37.942 | 74.184 | 0.09x |
| users.json $..total | strata | 13.015 | 13.830 | 15.161 | 75.168 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 336.871 | 341.160 | 386.138 | 75.168 | 0.04x |

