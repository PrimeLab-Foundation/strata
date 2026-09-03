# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 68d6e74
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
| users.json | strata | 10.665 | 10.993 | 15.332 | 62.887 | 1.00x |
| users.json | orjson | 13.854 | 14.342 | 17.568 | 62.887 | 0.77x |
| users.json | msgspec | 13.958 | 14.291 | 20.420 | 62.887 | 0.77x |
| users.json | ujson | 18.117 | 18.898 | 24.289 | 62.887 | 0.58x |
| users.json | pysimdjson | 20.004 | 21.285 | 28.117 | 62.887 | 0.52x |
| users.json | json | 21.288 | 23.104 | 34.361 | 62.887 | 0.48x |
| flat.json | strata | 0.956 | 0.973 | 1.078 | 73.449 | 1.00x |
| flat.json | orjson | 1.038 | 1.045 | 1.361 | 73.449 | 0.93x |
| flat.json | msgspec | 1.054 | 1.072 | 1.332 | 73.449 | 0.91x |
| flat.json | ujson | 1.507 | 1.594 | 2.037 | 73.449 | 0.61x |
| flat.json | pysimdjson | 1.622 | 1.674 | 1.985 | 73.449 | 0.58x |
| flat.json | json | 1.695 | 1.713 | 1.751 | 73.449 | 0.57x |
| nested.json | strata | 0.851 | 0.863 | 0.888 | 73.449 | 1.00x |
| nested.json | orjson | 0.993 | 1.009 | 1.033 | 73.449 | 0.86x |
| nested.json | msgspec | 0.966 | 0.986 | 1.006 | 73.449 | 0.87x |
| nested.json | ujson | 1.422 | 1.437 | 1.484 | 73.449 | 0.60x |
| nested.json | pysimdjson | 1.386 | 1.421 | 1.617 | 73.449 | 0.61x |
| nested.json | json | 1.827 | 1.844 | 2.067 | 73.449 | 0.47x |
| wide_arrays.json | strata | 5.116 | 5.160 | 5.255 | 78.012 | 1.00x |
| wide_arrays.json | orjson | 5.547 | 5.663 | 5.816 | 78.012 | 0.91x |
| wide_arrays.json | msgspec | 6.078 | 6.146 | 6.276 | 78.012 | 0.84x |
| wide_arrays.json | ujson | 7.790 | 7.847 | 8.213 | 78.012 | 0.66x |
| wide_arrays.json | pysimdjson | 6.467 | 6.497 | 6.631 | 78.012 | 0.79x |
| wide_arrays.json | json | 9.733 | 9.892 | 9.976 | 78.012 | 0.52x |
| mixed.json | strata | 0.209 | 0.212 | 0.229 | 78.074 | 1.00x |
| mixed.json | orjson | 0.236 | 0.243 | 0.274 | 78.074 | 0.87x |
| mixed.json | msgspec | 0.242 | 0.252 | 0.263 | 78.074 | 0.84x |
| mixed.json | ujson | 0.315 | 0.324 | 0.345 | 78.074 | 0.66x |
| mixed.json | pysimdjson | 0.297 | 0.312 | 0.330 | 78.074 | 0.68x |
| mixed.json | json | 0.448 | 0.462 | 0.483 | 78.074 | 0.46x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.468 | 2.504 | 2.643 | 61.246 | 1.00x |
| users.json | orjson | 3.064 | 3.103 | 4.098 | 61.246 | 0.81x |
| users.json | msgspec | 4.160 | 4.190 | 4.339 | 61.246 | 0.60x |
| users.json | ujson | 14.700 | 14.891 | 15.351 | 61.246 | 0.17x |
| users.json | json | 21.592 | 21.693 | 21.939 | 61.246 | 0.12x |
| flat.json | strata | 0.279 | 0.287 | 0.305 | 73.449 | 1.00x |
| flat.json | orjson | 0.355 | 0.367 | 0.418 | 73.449 | 0.78x |
| flat.json | msgspec | 0.465 | 0.474 | 0.490 | 73.449 | 0.60x |
| flat.json | ujson | 1.278 | 1.298 | 1.338 | 73.449 | 0.22x |
| flat.json | json | 1.828 | 1.836 | 1.924 | 73.449 | 0.16x |
| nested.json | strata | 0.249 | 0.253 | 0.269 | 73.449 | 1.00x |
| nested.json | orjson | 0.297 | 0.312 | 0.323 | 73.449 | 0.81x |
| nested.json | msgspec | 0.413 | 0.422 | 0.432 | 73.449 | 0.60x |
| nested.json | ujson | 1.342 | 1.356 | 1.608 | 73.449 | 0.19x |
| nested.json | json | 2.305 | 2.342 | 2.400 | 73.449 | 0.11x |
| wide_arrays.json | strata | 1.723 | 1.746 | 1.793 | 78.012 | 1.00x |
| wide_arrays.json | orjson | 1.905 | 1.926 | 1.960 | 78.012 | 0.91x |
| wide_arrays.json | msgspec | 2.946 | 2.956 | 3.001 | 78.012 | 0.59x |
| wide_arrays.json | ujson | 8.705 | 8.743 | 8.832 | 78.012 | 0.20x |
| wide_arrays.json | json | 16.462 | 16.609 | 16.700 | 78.012 | 0.11x |
| mixed.json | strata | 0.066 | 0.067 | 0.078 | 78.074 | 1.00x |
| mixed.json | orjson | 0.070 | 0.071 | 0.075 | 78.074 | 0.95x |
| mixed.json | msgspec | 0.089 | 0.090 | 0.108 | 78.074 | 0.75x |
| mixed.json | ujson | 0.302 | 0.311 | 0.335 | 78.074 | 0.22x |
| mixed.json | json | 0.517 | 0.525 | 0.546 | 78.074 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.825 | 12.316 | 13.832 | 78.027 | 1.00x |
| users.json | orjson | 14.743 | 15.225 | 16.121 | 78.027 | 0.81x |
| users.json | msgspec | 14.518 | 15.091 | 15.672 | 78.027 | 0.82x |
| users.json | ujson | 19.995 | 21.066 | 23.262 | 78.027 | 0.58x |
| users.json | json | 21.899 | 22.772 | 23.450 | 78.027 | 0.54x |
| flat.json | strata | 0.981 | 0.991 | 1.008 | 73.449 | 1.00x |
| flat.json | orjson | 1.096 | 1.105 | 1.134 | 73.449 | 0.90x |
| flat.json | msgspec | 1.104 | 1.117 | 1.172 | 73.449 | 0.89x |
| flat.json | ujson | 1.592 | 1.650 | 1.687 | 73.449 | 0.60x |
| flat.json | json | 1.763 | 1.771 | 1.802 | 73.449 | 0.56x |
| nested.json | strata | 0.882 | 0.894 | 0.905 | 73.449 | 1.00x |
| nested.json | orjson | 1.060 | 1.074 | 1.083 | 73.449 | 0.83x |
| nested.json | msgspec | 1.030 | 1.051 | 1.072 | 73.449 | 0.85x |
| nested.json | ujson | 1.482 | 1.497 | 1.539 | 73.449 | 0.60x |
| nested.json | json | 1.876 | 1.912 | 1.977 | 73.449 | 0.47x |
| wide_arrays.json | strata | 5.190 | 5.321 | 5.475 | 78.074 | 1.00x |
| wide_arrays.json | orjson | 5.653 | 5.912 | 6.084 | 78.074 | 0.90x |
| wide_arrays.json | msgspec | 6.429 | 6.467 | 6.602 | 78.074 | 0.82x |
| wide_arrays.json | ujson | 8.154 | 8.266 | 8.477 | 78.074 | 0.64x |
| wide_arrays.json | json | 9.929 | 10.227 | 10.404 | 78.074 | 0.52x |
| mixed.json | strata | 0.221 | 0.230 | 0.236 | 78.074 | 1.00x |
| mixed.json | orjson | 0.279 | 0.290 | 0.326 | 78.074 | 0.79x |
| mixed.json | msgspec | 0.283 | 0.296 | 0.326 | 78.074 | 0.78x |
| mixed.json | ujson | 0.356 | 0.371 | 0.383 | 78.074 | 0.62x |
| mixed.json | json | 0.483 | 0.494 | 0.515 | 78.074 | 0.47x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 11.706 | 11.879 | 13.667 | 73.449 | 1.00x |
| users.ndjson | orjson | 18.139 | 18.530 | 20.430 | 73.449 | 0.64x |
| users.ndjson | msgspec | 17.689 | 18.621 | 19.887 | 73.449 | 0.64x |
| users.ndjson | ujson | 22.571 | 23.470 | 26.296 | 73.449 | 0.51x |
| users.ndjson | json | 29.528 | 30.359 | 31.608 | 73.449 | 0.39x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.059 | 3.119 | 4.362 | 75.074 | 1.00x |
| users.json | orjson | 3.743 | 3.784 | 3.822 | 75.074 | 0.82x |
| users.json | msgspec | 4.764 | 4.833 | 4.878 | 75.074 | 0.65x |
| users.json | ujson | 15.638 | 15.694 | 16.058 | 75.074 | 0.20x |
| users.json | json | 22.397 | 22.825 | 26.492 | 75.074 | 0.14x |
| flat.json | strata | 0.435 | 0.450 | 0.463 | 73.449 | 1.00x |
| flat.json | orjson | 0.517 | 0.532 | 0.559 | 73.449 | 0.85x |
| flat.json | msgspec | 0.635 | 0.658 | 0.667 | 73.449 | 0.68x |
| flat.json | ujson | 1.470 | 1.484 | 1.517 | 73.449 | 0.30x |
| flat.json | json | 2.016 | 2.041 | 2.142 | 73.449 | 0.22x |
| nested.json | strata | 0.365 | 0.379 | 0.403 | 73.449 | 1.00x |
| nested.json | orjson | 0.446 | 0.455 | 0.475 | 73.449 | 0.83x |
| nested.json | msgspec | 0.561 | 0.576 | 0.587 | 73.449 | 0.66x |
| nested.json | ujson | 1.510 | 1.530 | 1.572 | 73.449 | 0.25x |
| nested.json | json | 2.481 | 2.520 | 2.605 | 73.449 | 0.15x |
| wide_arrays.json | strata | 2.178 | 2.209 | 2.477 | 78.074 | 1.00x |
| wide_arrays.json | orjson | 2.344 | 2.411 | 2.692 | 78.074 | 0.92x |
| wide_arrays.json | msgspec | 3.392 | 3.448 | 5.844 | 78.074 | 0.64x |
| wide_arrays.json | ujson | 9.270 | 9.309 | 11.762 | 78.074 | 0.24x |
| wide_arrays.json | json | 17.069 | 17.136 | 20.020 | 78.074 | 0.13x |
| mixed.json | strata | 0.157 | 0.166 | 0.193 | 78.074 | 1.00x |
| mixed.json | orjson | 0.176 | 0.183 | 0.199 | 78.074 | 0.91x |
| mixed.json | msgspec | 0.197 | 0.201 | 0.239 | 78.074 | 0.82x |
| mixed.json | ujson | 0.430 | 0.441 | 0.461 | 78.074 | 0.38x |
| mixed.json | json | 0.643 | 0.657 | 0.692 | 78.074 | 0.25x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.073 | 0.076 | 0.080 | 75.074 | 1.00x |
| users.json $[*].id | jmespath | 0.463 | 0.472 | 0.507 | 75.074 | 0.16x |
| users.json $[*].id | jsonpath-ng | 2.705 | 2.898 | 3.052 | 75.074 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.423 | 0.434 | 0.454 | 75.078 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.009 | 3.064 | 3.244 | 75.078 | 0.14x |
| users.json $[*].orders[*].total | jsonpath-ng | 19.391 | 19.573 | 20.279 | 75.078 | 0.02x |
| users.json $..total | strata | 1.779 | 1.823 | 1.860 | 77.047 | 1.00x |
| users.json $..total | jsonpath-ng | 387.561 | 391.177 | 393.499 | 77.047 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.355 | 3.362 | 3.379 | 75.078 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.054 | 15.343 | 16.887 | 75.078 | 0.22x |
| users.json $[*].id | orjson+jsonpath-ng | 17.392 | 17.637 | 19.416 | 75.078 | 0.19x |
| users.json $[*].orders[*].total | strata | 3.594 | 3.626 | 3.701 | 77.047 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.126 | 18.506 | 18.945 | 77.047 | 0.20x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 37.971 | 39.607 | 41.118 | 77.047 | 0.09x |
| users.json $..total | strata | 14.261 | 15.031 | 15.682 | 76.266 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 410.090 | 412.434 | 418.395 | 76.266 | 0.04x |

