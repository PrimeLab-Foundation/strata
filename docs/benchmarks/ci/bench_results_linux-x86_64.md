# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 53fa480
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
| users.json | strata | 10.204 | 10.376 | 14.096 | 60.125 | 1.00x |
| users.json | orjson | 13.863 | 14.009 | 17.487 | 60.125 | 0.74x |
| users.json | msgspec | 13.776 | 14.079 | 17.462 | 60.125 | 0.74x |
| users.json | ujson | 18.018 | 18.240 | 22.929 | 60.125 | 0.57x |
| users.json | pysimdjson | 18.623 | 19.010 | 22.887 | 60.125 | 0.55x |
| users.json | json | 21.407 | 21.602 | 23.007 | 60.125 | 0.48x |
| flat.json | strata | 0.902 | 0.907 | 0.924 | 70.582 | 1.00x |
| flat.json | orjson | 1.060 | 1.073 | 1.088 | 70.582 | 0.84x |
| flat.json | msgspec | 1.066 | 1.076 | 1.089 | 70.582 | 0.84x |
| flat.json | ujson | 1.528 | 1.553 | 1.579 | 70.582 | 0.58x |
| flat.json | pysimdjson | 1.639 | 1.693 | 1.727 | 70.582 | 0.54x |
| flat.json | json | 1.728 | 1.749 | 1.757 | 70.582 | 0.52x |
| nested.json | strata | 0.777 | 0.792 | 0.813 | 70.582 | 1.00x |
| nested.json | orjson | 0.991 | 1.000 | 1.021 | 70.582 | 0.79x |
| nested.json | msgspec | 0.962 | 0.977 | 1.003 | 70.582 | 0.81x |
| nested.json | ujson | 1.399 | 1.423 | 1.503 | 70.582 | 0.56x |
| nested.json | pysimdjson | 1.377 | 1.399 | 1.446 | 70.582 | 0.57x |
| nested.json | json | 1.820 | 1.837 | 1.849 | 70.582 | 0.43x |
| wide_arrays.json | strata | 4.764 | 4.799 | 4.869 | 74.586 | 1.00x |
| wide_arrays.json | orjson | 5.445 | 5.499 | 5.579 | 74.586 | 0.87x |
| wide_arrays.json | msgspec | 6.028 | 6.106 | 6.158 | 74.586 | 0.79x |
| wide_arrays.json | ujson | 7.480 | 7.564 | 7.644 | 74.586 | 0.63x |
| wide_arrays.json | pysimdjson | 6.305 | 6.364 | 6.582 | 74.586 | 0.75x |
| wide_arrays.json | json | 9.604 | 9.753 | 9.922 | 74.586 | 0.49x |
| mixed.json | strata | 0.193 | 0.197 | 0.208 | 74.648 | 1.00x |
| mixed.json | orjson | 0.234 | 0.240 | 0.253 | 74.648 | 0.82x |
| mixed.json | msgspec | 0.240 | 0.246 | 0.260 | 74.648 | 0.80x |
| mixed.json | ujson | 0.303 | 0.308 | 0.343 | 74.648 | 0.64x |
| mixed.json | pysimdjson | 0.299 | 0.304 | 0.315 | 74.648 | 0.65x |
| mixed.json | json | 0.443 | 0.457 | 0.465 | 74.648 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.426 | 2.441 | 2.468 | 58.922 | 1.00x |
| users.json | orjson | 3.077 | 3.089 | 3.117 | 58.922 | 0.79x |
| users.json | msgspec | 4.139 | 4.147 | 4.397 | 58.922 | 0.59x |
| users.json | ujson | 11.299 | 11.407 | 11.532 | 58.922 | 0.21x |
| users.json | json | 21.267 | 21.371 | 21.753 | 58.922 | 0.11x |
| flat.json | strata | 0.301 | 0.306 | 0.314 | 70.582 | 1.00x |
| flat.json | orjson | 0.359 | 0.364 | 0.374 | 70.582 | 0.84x |
| flat.json | msgspec | 0.486 | 0.495 | 0.510 | 70.582 | 0.62x |
| flat.json | ujson | 1.039 | 1.049 | 1.053 | 70.582 | 0.29x |
| flat.json | json | 1.848 | 1.888 | 1.967 | 70.582 | 0.16x |
| nested.json | strata | 0.243 | 0.246 | 0.261 | 70.582 | 1.00x |
| nested.json | orjson | 0.304 | 0.311 | 0.319 | 70.582 | 0.79x |
| nested.json | msgspec | 0.420 | 0.425 | 0.435 | 70.582 | 0.58x |
| nested.json | ujson | 1.064 | 1.076 | 1.104 | 70.582 | 0.23x |
| nested.json | json | 2.350 | 2.379 | 2.413 | 70.582 | 0.10x |
| wide_arrays.json | strata | 1.646 | 1.665 | 1.694 | 74.586 | 1.00x |
| wide_arrays.json | orjson | 1.849 | 1.871 | 1.883 | 74.586 | 0.89x |
| wide_arrays.json | msgspec | 2.934 | 2.955 | 3.005 | 74.586 | 0.56x |
| wide_arrays.json | ujson | 6.231 | 6.250 | 6.290 | 74.586 | 0.27x |
| wide_arrays.json | json | 16.540 | 16.564 | 16.818 | 74.586 | 0.10x |
| mixed.json | strata | 0.063 | 0.065 | 0.077 | 74.648 | 1.00x |
| mixed.json | orjson | 0.069 | 0.070 | 0.078 | 74.648 | 0.93x |
| mixed.json | msgspec | 0.085 | 0.086 | 0.090 | 74.648 | 0.75x |
| mixed.json | ujson | 0.224 | 0.227 | 0.239 | 74.648 | 0.29x |
| mixed.json | json | 0.507 | 0.515 | 0.539 | 74.648 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.596 | 10.809 | 12.483 | 75.168 | 1.00x |
| users.json | orjson | 14.314 | 14.670 | 15.559 | 75.168 | 0.74x |
| users.json | msgspec | 14.333 | 14.534 | 14.926 | 75.168 | 0.74x |
| users.json | ujson | 18.876 | 19.232 | 21.874 | 75.168 | 0.56x |
| users.json | json | 21.754 | 21.980 | 22.251 | 75.168 | 0.49x |
| flat.json | strata | 0.926 | 0.934 | 0.951 | 70.582 | 1.00x |
| flat.json | orjson | 1.118 | 1.127 | 1.177 | 70.582 | 0.83x |
| flat.json | msgspec | 1.105 | 1.128 | 1.157 | 70.582 | 0.83x |
| flat.json | ujson | 1.610 | 1.657 | 1.684 | 70.582 | 0.56x |
| flat.json | json | 1.771 | 1.789 | 1.801 | 70.582 | 0.52x |
| nested.json | strata | 0.819 | 0.839 | 0.849 | 70.582 | 1.00x |
| nested.json | orjson | 1.040 | 1.062 | 1.094 | 70.582 | 0.79x |
| nested.json | msgspec | 1.021 | 1.040 | 1.048 | 70.582 | 0.81x |
| nested.json | ujson | 1.453 | 1.478 | 1.494 | 70.582 | 0.57x |
| nested.json | json | 1.882 | 1.900 | 1.997 | 70.582 | 0.44x |
| wide_arrays.json | strata | 4.818 | 4.866 | 4.924 | 74.648 | 1.00x |
| wide_arrays.json | orjson | 5.572 | 5.635 | 7.059 | 74.648 | 0.86x |
| wide_arrays.json | msgspec | 6.169 | 6.233 | 6.297 | 74.648 | 0.78x |
| wide_arrays.json | ujson | 7.749 | 7.791 | 7.807 | 74.648 | 0.62x |
| wide_arrays.json | json | 9.782 | 9.822 | 10.030 | 74.648 | 0.50x |
| mixed.json | strata | 0.212 | 0.218 | 0.224 | 74.648 | 1.00x |
| mixed.json | orjson | 0.293 | 0.300 | 0.313 | 74.648 | 0.73x |
| mixed.json | msgspec | 0.282 | 0.287 | 0.308 | 74.648 | 0.76x |
| mixed.json | ujson | 0.368 | 0.378 | 0.398 | 74.648 | 0.58x |
| mixed.json | json | 0.487 | 0.505 | 0.518 | 74.648 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.627 | 10.886 | 11.037 | 70.582 | 1.00x |
| users.ndjson | orjson | 17.940 | 18.170 | 19.193 | 70.582 | 0.60x |
| users.ndjson | msgspec | 17.770 | 18.002 | 18.253 | 70.582 | 0.60x |
| users.ndjson | ujson | 22.124 | 22.438 | 22.568 | 70.582 | 0.49x |
| users.ndjson | json | 28.897 | 29.334 | 29.779 | 70.582 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.963 | 2.985 | 3.041 | 72.215 | 1.00x |
| users.json | orjson | 3.719 | 3.745 | 3.786 | 72.215 | 0.80x |
| users.json | msgspec | 4.693 | 4.717 | 4.767 | 72.215 | 0.63x |
| users.json | ujson | 12.123 | 12.313 | 12.361 | 72.215 | 0.24x |
| users.json | json | 22.261 | 22.322 | 22.712 | 72.215 | 0.13x |
| flat.json | strata | 0.434 | 0.446 | 0.469 | 70.582 | 1.00x |
| flat.json | orjson | 0.517 | 0.545 | 0.562 | 70.582 | 0.82x |
| flat.json | msgspec | 0.640 | 0.654 | 0.684 | 70.582 | 0.68x |
| flat.json | ujson | 1.213 | 1.223 | 1.236 | 70.582 | 0.36x |
| flat.json | json | 2.024 | 2.054 | 2.199 | 70.582 | 0.22x |
| nested.json | strata | 0.357 | 0.368 | 0.388 | 70.582 | 1.00x |
| nested.json | orjson | 0.436 | 0.464 | 0.492 | 70.582 | 0.79x |
| nested.json | msgspec | 0.558 | 0.573 | 0.586 | 70.582 | 0.64x |
| nested.json | ujson | 1.217 | 1.226 | 1.249 | 70.582 | 0.30x |
| nested.json | json | 2.520 | 2.546 | 2.608 | 70.582 | 0.14x |
| wide_arrays.json | strata | 2.049 | 2.069 | 2.095 | 74.648 | 1.00x |
| wide_arrays.json | orjson | 2.276 | 2.297 | 2.318 | 74.648 | 0.90x |
| wide_arrays.json | msgspec | 3.335 | 3.361 | 3.397 | 74.648 | 0.62x |
| wide_arrays.json | ujson | 6.703 | 6.733 | 6.789 | 74.648 | 0.31x |
| wide_arrays.json | json | 16.915 | 16.986 | 17.187 | 74.648 | 0.12x |
| mixed.json | strata | 0.152 | 0.158 | 0.167 | 74.648 | 1.00x |
| mixed.json | orjson | 0.171 | 0.178 | 0.208 | 74.648 | 0.89x |
| mixed.json | msgspec | 0.187 | 0.195 | 0.218 | 74.648 | 0.81x |
| mixed.json | ujson | 0.342 | 0.355 | 0.376 | 74.648 | 0.44x |
| mixed.json | json | 0.627 | 0.645 | 0.682 | 74.648 | 0.24x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.068 | 0.073 | 0.086 | 72.215 | 1.00x |
| users.json $[*].id | jmespath | 0.463 | 0.477 | 0.493 | 72.215 | 0.15x |
| users.json $[*].id | jsonpath-ng | 2.686 | 2.728 | 2.778 | 72.215 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.419 | 0.429 | 0.439 | 72.215 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.979 | 2.991 | 3.020 | 72.215 | 0.14x |
| users.json $[*].orders[*].total | jsonpath-ng | 19.046 | 19.160 | 19.474 | 72.215 | 0.02x |
| users.json $..total | strata | 1.765 | 1.802 | 1.880 | 74.184 | 1.00x |
| users.json $..total | jsonpath-ng | 385.877 | 387.264 | 389.302 | 74.184 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.298 | 3.312 | 3.345 | 72.215 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.095 | 15.136 | 15.303 | 72.215 | 0.22x |
| users.json $[*].id | orjson+jsonpath-ng | 17.183 | 17.470 | 17.743 | 72.215 | 0.19x |
| users.json $[*].orders[*].total | strata | 3.507 | 3.524 | 3.547 | 74.184 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.740 | 18.049 | 18.383 | 74.184 | 0.20x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 37.906 | 38.389 | 39.652 | 74.184 | 0.09x |
| users.json $..total | strata | 13.019 | 13.143 | 13.588 | 74.184 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 407.178 | 409.611 | 416.625 | 74.184 | 0.03x |

