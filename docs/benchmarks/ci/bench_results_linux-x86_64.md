# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: af229e1
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
| users.json | strata | 9.654 | 9.800 | 16.062 | 59.559 | 1.00x |
| users.json | orjson | 13.724 | 14.086 | 18.862 | 59.559 | 0.70x |
| users.json | msgspec | 13.861 | 14.129 | 15.730 | 59.559 | 0.69x |
| users.json | ujson | 18.527 | 18.751 | 23.015 | 59.559 | 0.52x |
| users.json | pysimdjson | 18.043 | 18.427 | 28.083 | 59.559 | 0.53x |
| users.json | json | 21.002 | 21.150 | 37.264 | 59.559 | 0.46x |
| flat.json | strata | 0.872 | 0.900 | 0.907 | 65.512 | 1.00x |
| flat.json | orjson | 1.017 | 1.052 | 1.081 | 65.512 | 0.86x |
| flat.json | msgspec | 0.993 | 1.020 | 1.281 | 65.512 | 0.88x |
| flat.json | ujson | 1.438 | 1.461 | 1.619 | 65.512 | 0.62x |
| flat.json | pysimdjson | 1.630 | 1.650 | 1.685 | 65.512 | 0.55x |
| flat.json | json | 1.774 | 1.810 | 1.869 | 65.512 | 0.50x |
| nested.json | strata | 0.721 | 0.745 | 0.761 | 65.512 | 1.00x |
| nested.json | orjson | 0.883 | 0.893 | 0.947 | 65.512 | 0.83x |
| nested.json | msgspec | 0.916 | 0.926 | 1.042 | 65.512 | 0.80x |
| nested.json | ujson | 1.314 | 1.326 | 1.351 | 65.512 | 0.56x |
| nested.json | pysimdjson | 1.263 | 1.275 | 1.344 | 65.512 | 0.58x |
| nested.json | json | 1.801 | 1.821 | 1.882 | 65.512 | 0.41x |
| wide_arrays.json | strata | 4.149 | 4.221 | 4.314 | 72.027 | 1.00x |
| wide_arrays.json | orjson | 4.951 | 5.076 | 5.220 | 72.027 | 0.83x |
| wide_arrays.json | msgspec | 5.401 | 5.479 | 5.592 | 72.027 | 0.77x |
| wide_arrays.json | ujson | 6.993 | 7.098 | 7.323 | 72.027 | 0.59x |
| wide_arrays.json | pysimdjson | 5.500 | 5.572 | 5.735 | 72.027 | 0.76x |
| wide_arrays.json | json | 9.127 | 9.234 | 9.371 | 72.027 | 0.46x |
| mixed.json | strata | 0.190 | 0.192 | 0.205 | 72.090 | 1.00x |
| mixed.json | orjson | 0.225 | 0.236 | 0.270 | 72.090 | 0.81x |
| mixed.json | msgspec | 0.242 | 0.252 | 0.269 | 72.090 | 0.76x |
| mixed.json | ujson | 0.302 | 0.313 | 0.329 | 72.090 | 0.61x |
| mixed.json | pysimdjson | 0.294 | 0.302 | 0.315 | 72.090 | 0.64x |
| mixed.json | json | 0.452 | 0.472 | 0.506 | 72.090 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.950 | 1.967 | 2.050 | 58.645 | 1.00x |
| users.json | orjson | 2.187 | 2.226 | 2.305 | 58.645 | 0.88x |
| users.json | msgspec | 3.717 | 3.759 | 3.891 | 58.645 | 0.52x |
| users.json | ujson | 13.488 | 13.603 | 14.381 | 58.645 | 0.14x |
| users.json | json | 20.426 | 20.611 | 21.215 | 58.645 | 0.10x |
| flat.json | strata | 0.269 | 0.277 | 0.297 | 65.512 | 1.00x |
| flat.json | orjson | 0.264 | 0.270 | 0.303 | 65.512 | 1.03x |
| flat.json | msgspec | 0.407 | 0.423 | 0.480 | 65.512 | 0.65x |
| flat.json | ujson | 1.180 | 1.194 | 1.254 | 65.512 | 0.23x |
| flat.json | json | 1.719 | 1.758 | 2.077 | 65.512 | 0.16x |
| nested.json | strata | 0.182 | 0.183 | 0.199 | 65.512 | 1.00x |
| nested.json | orjson | 0.236 | 0.248 | 0.262 | 65.512 | 0.74x |
| nested.json | msgspec | 0.357 | 0.380 | 0.386 | 65.512 | 0.48x |
| nested.json | ujson | 1.232 | 1.240 | 1.259 | 65.512 | 0.15x |
| nested.json | json | 2.189 | 2.220 | 2.414 | 65.512 | 0.08x |
| wide_arrays.json | strata | 1.384 | 1.407 | 1.438 | 72.027 | 1.00x |
| wide_arrays.json | orjson | 1.459 | 1.482 | 1.545 | 72.027 | 0.95x |
| wide_arrays.json | msgspec | 2.520 | 2.533 | 2.697 | 72.027 | 0.56x |
| wide_arrays.json | ujson | 7.581 | 7.720 | 7.857 | 72.027 | 0.18x |
| wide_arrays.json | json | 14.599 | 14.854 | 14.957 | 72.027 | 0.09x |
| mixed.json | strata | 0.056 | 0.057 | 0.070 | 72.090 | 1.00x |
| mixed.json | orjson | 0.056 | 0.059 | 0.067 | 72.090 | 0.97x |
| mixed.json | msgspec | 0.078 | 0.083 | 0.109 | 72.090 | 0.69x |
| mixed.json | ujson | 0.286 | 0.289 | 0.318 | 72.090 | 0.20x |
| mixed.json | json | 0.500 | 0.511 | 0.576 | 72.090 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.033 | 10.265 | 11.083 | 70.922 | 1.00x |
| users.json | orjson | 14.074 | 14.233 | 14.547 | 70.922 | 0.72x |
| users.json | msgspec | 14.329 | 14.465 | 14.812 | 70.922 | 0.71x |
| users.json | ujson | 19.391 | 19.984 | 21.163 | 70.922 | 0.51x |
| users.json | json | 21.517 | 21.683 | 22.059 | 70.922 | 0.47x |
| flat.json | strata | 0.891 | 0.911 | 0.960 | 65.512 | 1.00x |
| flat.json | orjson | 1.069 | 1.117 | 1.172 | 65.512 | 0.82x |
| flat.json | msgspec | 1.075 | 1.103 | 1.180 | 65.512 | 0.83x |
| flat.json | ujson | 1.555 | 1.575 | 1.823 | 65.512 | 0.58x |
| flat.json | json | 1.838 | 1.862 | 2.020 | 65.512 | 0.49x |
| nested.json | strata | 0.760 | 0.779 | 0.951 | 65.512 | 1.00x |
| nested.json | orjson | 0.949 | 0.958 | 1.000 | 65.512 | 0.81x |
| nested.json | msgspec | 0.956 | 0.991 | 1.000 | 65.512 | 0.79x |
| nested.json | ujson | 1.394 | 1.420 | 1.443 | 65.512 | 0.55x |
| nested.json | json | 1.881 | 1.898 | 1.941 | 65.512 | 0.41x |
| wide_arrays.json | strata | 4.221 | 4.266 | 4.394 | 72.090 | 1.00x |
| wide_arrays.json | orjson | 5.006 | 5.341 | 5.603 | 72.090 | 0.80x |
| wide_arrays.json | msgspec | 5.542 | 5.597 | 5.852 | 72.090 | 0.76x |
| wide_arrays.json | ujson | 7.328 | 7.398 | 7.507 | 72.090 | 0.58x |
| wide_arrays.json | json | 9.282 | 9.379 | 9.535 | 72.090 | 0.45x |
| mixed.json | strata | 0.219 | 0.222 | 0.300 | 72.090 | 1.00x |
| mixed.json | orjson | 0.278 | 0.287 | 0.385 | 72.090 | 0.77x |
| mixed.json | msgspec | 0.287 | 0.294 | 0.421 | 72.090 | 0.76x |
| mixed.json | ujson | 0.369 | 0.394 | 0.546 | 72.090 | 0.56x |
| mixed.json | json | 0.521 | 0.525 | 0.769 | 72.090 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.147 | 10.285 | 10.526 | 65.508 | 1.00x |
| users.ndjson | orjson | 15.930 | 16.201 | 16.309 | 65.508 | 0.63x |
| users.ndjson | msgspec | 15.855 | 16.233 | 16.458 | 65.508 | 0.63x |
| users.ndjson | ujson | 20.584 | 20.834 | 21.417 | 65.508 | 0.49x |
| users.ndjson | json | 25.881 | 26.342 | 27.411 | 65.508 | 0.39x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.457 | 2.555 | 46.946 | 67.969 | 1.00x |
| users.json | orjson | 2.798 | 2.827 | 24.900 | 67.969 | 0.90x |
| users.json | msgspec | 4.338 | 4.374 | 63.419 | 67.969 | 0.58x |
| users.json | ujson | 14.309 | 14.542 | 90.486 | 67.969 | 0.18x |
| users.json | json | 21.023 | 21.772 | 110.620 | 67.969 | 0.12x |
| flat.json | strata | 0.381 | 0.400 | 0.446 | 65.512 | 1.00x |
| flat.json | orjson | 0.388 | 0.406 | 0.442 | 65.512 | 0.99x |
| flat.json | msgspec | 0.536 | 0.547 | 0.584 | 65.512 | 0.73x |
| flat.json | ujson | 1.314 | 1.333 | 1.377 | 65.512 | 0.30x |
| flat.json | json | 1.870 | 1.902 | 1.922 | 65.512 | 0.21x |
| nested.json | strata | 0.276 | 0.282 | 0.318 | 65.512 | 1.00x |
| nested.json | orjson | 0.352 | 0.367 | 0.396 | 65.512 | 0.77x |
| nested.json | msgspec | 0.456 | 0.488 | 0.516 | 65.512 | 0.58x |
| nested.json | ujson | 1.357 | 1.392 | 1.414 | 65.512 | 0.20x |
| nested.json | json | 2.320 | 2.364 | 2.437 | 65.512 | 0.12x |
| wide_arrays.json | strata | 1.788 | 1.840 | 16.479 | 72.090 | 1.00x |
| wide_arrays.json | orjson | 1.885 | 2.017 | 21.323 | 72.090 | 0.91x |
| wide_arrays.json | msgspec | 2.968 | 3.035 | 44.622 | 72.090 | 0.61x |
| wide_arrays.json | ujson | 8.058 | 8.249 | 8.862 | 72.090 | 0.22x |
| wide_arrays.json | json | 15.164 | 15.591 | 66.942 | 72.090 | 0.12x |
| mixed.json | strata | 0.121 | 0.124 | 0.160 | 72.090 | 1.00x |
| mixed.json | orjson | 0.135 | 0.140 | 0.153 | 72.090 | 0.89x |
| mixed.json | msgspec | 0.155 | 0.161 | 78.034 | 72.090 | 0.77x |
| mixed.json | ujson | 0.375 | 0.386 | 0.464 | 72.090 | 0.32x |
| mixed.json | json | 0.593 | 0.605 | 0.656 | 72.090 | 0.21x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.064 | 0.068 | 0.071 | 67.969 | 1.00x |
| users.json $[*].id | jmespath | 0.455 | 0.472 | 0.492 | 67.969 | 0.14x |
| users.json $[*].id | jsonpath-ng | 2.323 | 2.366 | 2.522 | 67.969 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.472 | 0.492 | 0.524 | 67.992 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.857 | 2.874 | 3.026 | 67.992 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 15.696 | 15.897 | 16.132 | 67.992 | 0.03x |
| users.json $..total | strata | 1.755 | 1.767 | 1.819 | 70.090 | 1.00x |
| users.json $..total | jsonpath-ng | 308.947 | 310.085 | 325.662 | 70.090 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 2.716 | 2.972 | 3.002 | 67.992 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.442 | 14.569 | 14.756 | 67.992 | 0.20x |
| users.json $[*].id | orjson+jsonpath-ng | 16.245 | 16.354 | 16.689 | 67.992 | 0.18x |
| users.json $[*].orders[*].total | strata | 3.159 | 3.226 | 3.263 | 69.309 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.072 | 17.265 | 17.546 | 69.309 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 35.730 | 36.064 | 37.581 | 69.309 | 0.09x |
| users.json $..total | strata | 13.399 | 13.963 | 14.373 | 69.328 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 328.710 | 330.432 | 332.580 | 69.328 | 0.04x |

