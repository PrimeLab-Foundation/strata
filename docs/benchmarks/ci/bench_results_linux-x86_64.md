# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 9108337
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
| users.json | strata | 10.573 | 11.489 | 28.953 | 57.500 | 1.00x |
| users.json | orjson | 14.094 | 14.358 | 30.215 | 57.500 | 0.80x |
| users.json | msgspec | 13.728 | 14.133 | 30.910 | 57.500 | 0.81x |
| users.json | ujson | 19.695 | 20.918 | 50.644 | 57.500 | 0.55x |
| users.json | pysimdjson | 20.572 | 21.550 | 36.086 | 57.500 | 0.53x |
| users.json | json | 23.223 | 24.058 | 26.687 | 57.500 | 0.48x |
| flat.json | strata | 0.872 | 0.936 | 1.100 | 65.656 | 1.00x |
| flat.json | orjson | 1.009 | 1.043 | 1.258 | 65.656 | 0.90x |
| flat.json | msgspec | 1.040 | 1.089 | 1.254 | 65.656 | 0.86x |
| flat.json | ujson | 1.652 | 1.749 | 1.943 | 65.656 | 0.54x |
| flat.json | pysimdjson | 1.539 | 1.734 | 1.877 | 65.656 | 0.54x |
| flat.json | json | 1.908 | 1.955 | 2.095 | 65.656 | 0.48x |
| nested.json | strata | 0.820 | 0.853 | 1.049 | 65.656 | 1.00x |
| nested.json | orjson | 1.008 | 1.042 | 1.141 | 65.656 | 0.82x |
| nested.json | msgspec | 1.046 | 1.076 | 1.225 | 65.656 | 0.79x |
| nested.json | ujson | 1.574 | 1.672 | 1.877 | 65.656 | 0.51x |
| nested.json | pysimdjson | 1.406 | 1.453 | 1.757 | 65.656 | 0.59x |
| nested.json | json | 2.050 | 2.073 | 2.273 | 65.656 | 0.41x |
| wide_arrays.json | strata | 4.726 | 5.505 | 6.060 | 72.141 | 1.00x |
| wide_arrays.json | orjson | 5.261 | 6.257 | 6.780 | 72.141 | 0.88x |
| wide_arrays.json | msgspec | 5.871 | 6.397 | 8.288 | 72.141 | 0.86x |
| wide_arrays.json | ujson | 7.381 | 7.974 | 9.305 | 72.141 | 0.69x |
| wide_arrays.json | pysimdjson | 6.175 | 7.122 | 7.645 | 72.141 | 0.77x |
| wide_arrays.json | json | 9.870 | 11.210 | 11.746 | 72.141 | 0.49x |
| mixed.json | strata | 0.197 | 0.209 | 0.235 | 72.203 | 1.00x |
| mixed.json | orjson | 0.233 | 0.250 | 0.273 | 72.203 | 0.84x |
| mixed.json | msgspec | 0.244 | 0.260 | 0.315 | 72.203 | 0.80x |
| mixed.json | ujson | 0.336 | 0.359 | 0.415 | 72.203 | 0.58x |
| mixed.json | pysimdjson | 0.304 | 0.323 | 0.396 | 72.203 | 0.65x |
| mixed.json | json | 0.479 | 0.507 | 0.596 | 72.203 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.472 | 2.513 | 2.593 | 55.969 | 1.00x |
| users.json | orjson | 2.933 | 3.005 | 3.221 | 55.969 | 0.84x |
| users.json | msgspec | 3.881 | 3.961 | 4.182 | 55.969 | 0.63x |
| users.json | ujson | 15.021 | 15.187 | 15.608 | 55.969 | 0.17x |
| users.json | json | 21.730 | 21.931 | 23.494 | 55.969 | 0.11x |
| flat.json | strata | 0.292 | 0.352 | 0.421 | 65.656 | 1.00x |
| flat.json | orjson | 0.372 | 0.406 | 0.463 | 65.656 | 0.87x |
| flat.json | msgspec | 0.465 | 0.541 | 0.629 | 65.656 | 0.65x |
| flat.json | ujson | 1.297 | 1.374 | 1.539 | 65.656 | 0.26x |
| flat.json | json | 1.856 | 1.992 | 3.191 | 65.656 | 0.18x |
| nested.json | strata | 0.268 | 0.279 | 0.301 | 65.656 | 1.00x |
| nested.json | orjson | 0.292 | 0.308 | 0.335 | 65.656 | 0.91x |
| nested.json | msgspec | 0.404 | 0.430 | 0.465 | 65.656 | 0.65x |
| nested.json | ujson | 1.431 | 1.458 | 1.644 | 65.656 | 0.19x |
| nested.json | json | 2.415 | 2.458 | 2.667 | 65.656 | 0.11x |
| wide_arrays.json | strata | 1.432 | 1.474 | 1.584 | 72.141 | 1.00x |
| wide_arrays.json | orjson | 1.848 | 1.914 | 1.960 | 72.141 | 0.77x |
| wide_arrays.json | msgspec | 2.706 | 2.753 | 2.832 | 72.141 | 0.54x |
| wide_arrays.json | ujson | 8.660 | 8.791 | 10.651 | 72.141 | 0.17x |
| wide_arrays.json | json | 16.668 | 16.861 | 27.868 | 72.141 | 0.09x |
| mixed.json | strata | 0.066 | 0.069 | 0.087 | 72.203 | 1.00x |
| mixed.json | orjson | 0.066 | 0.068 | 0.070 | 72.203 | 1.01x |
| mixed.json | msgspec | 0.088 | 0.091 | 0.106 | 72.203 | 0.76x |
| mixed.json | ujson | 0.308 | 0.313 | 0.339 | 72.203 | 0.22x |
| mixed.json | json | 0.515 | 0.528 | 0.630 | 72.203 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 12.426 | 13.043 | 14.775 | 70.238 | 1.00x |
| users.json | orjson | 14.354 | 14.903 | 16.290 | 70.238 | 0.88x |
| users.json | msgspec | 14.434 | 15.074 | 17.281 | 70.238 | 0.87x |
| users.json | ujson | 20.924 | 22.154 | 24.519 | 70.238 | 0.59x |
| users.json | json | 23.523 | 24.539 | 26.959 | 70.238 | 0.53x |
| flat.json | strata | 0.896 | 0.958 | 1.032 | 65.656 | 1.00x |
| flat.json | orjson | 1.065 | 1.107 | 1.209 | 65.656 | 0.87x |
| flat.json | msgspec | 1.131 | 1.149 | 1.427 | 65.656 | 0.83x |
| flat.json | ujson | 1.657 | 1.749 | 1.880 | 65.656 | 0.55x |
| flat.json | json | 1.938 | 1.978 | 2.133 | 65.656 | 0.48x |
| nested.json | strata | 0.874 | 0.927 | 1.077 | 65.656 | 1.00x |
| nested.json | orjson | 1.138 | 1.214 | 1.366 | 65.656 | 0.76x |
| nested.json | msgspec | 1.118 | 1.283 | 1.563 | 65.656 | 0.72x |
| nested.json | ujson | 1.630 | 1.780 | 2.150 | 65.656 | 0.52x |
| nested.json | json | 2.092 | 2.221 | 2.288 | 65.656 | 0.42x |
| wide_arrays.json | strata | 5.201 | 5.327 | 5.516 | 72.203 | 1.00x |
| wide_arrays.json | orjson | 5.792 | 5.928 | 6.185 | 72.203 | 0.90x |
| wide_arrays.json | msgspec | 6.415 | 6.683 | 6.867 | 72.203 | 0.80x |
| wide_arrays.json | ujson | 8.099 | 8.425 | 8.785 | 72.203 | 0.63x |
| wide_arrays.json | json | 10.546 | 10.806 | 11.071 | 72.203 | 0.49x |
| mixed.json | strata | 0.218 | 0.239 | 0.285 | 72.203 | 1.00x |
| mixed.json | orjson | 0.277 | 0.308 | 0.342 | 72.203 | 0.78x |
| mixed.json | msgspec | 0.283 | 0.304 | 0.334 | 72.203 | 0.79x |
| mixed.json | ujson | 0.365 | 0.407 | 0.432 | 72.203 | 0.59x |
| mixed.json | json | 0.524 | 0.535 | 0.570 | 72.203 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 14.166 | 14.568 | 21.308 | 65.656 | 1.00x |
| users.ndjson | orjson | 20.186 | 21.174 | 22.038 | 65.656 | 0.69x |
| users.ndjson | msgspec | 19.166 | 20.875 | 21.796 | 65.656 | 0.70x |
| users.ndjson | ujson | 26.181 | 27.538 | 29.470 | 65.656 | 0.53x |
| users.ndjson | json | 33.305 | 34.048 | 35.197 | 65.656 | 0.43x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.241 | 3.373 | 3.571 | 65.316 | 1.00x |
| users.json | orjson | 3.734 | 4.034 | 4.306 | 65.316 | 0.84x |
| users.json | msgspec | 4.716 | 4.922 | 5.169 | 65.316 | 0.69x |
| users.json | ujson | 16.092 | 16.454 | 16.992 | 65.316 | 0.20x |
| users.json | json | 23.011 | 23.292 | 23.788 | 65.316 | 0.14x |
| flat.json | strata | 0.459 | 0.503 | 0.588 | 65.656 | 1.00x |
| flat.json | orjson | 0.550 | 0.569 | 0.656 | 65.656 | 0.88x |
| flat.json | msgspec | 0.638 | 0.685 | 0.822 | 65.656 | 0.73x |
| flat.json | ujson | 1.505 | 1.586 | 1.668 | 65.656 | 0.32x |
| flat.json | json | 2.086 | 2.164 | 2.371 | 65.656 | 0.23x |
| nested.json | strata | 0.405 | 0.441 | 0.506 | 65.656 | 1.00x |
| nested.json | orjson | 0.451 | 0.477 | 0.572 | 65.656 | 0.93x |
| nested.json | msgspec | 0.571 | 0.599 | 0.658 | 65.656 | 0.74x |
| nested.json | ujson | 1.573 | 1.601 | 1.867 | 65.656 | 0.28x |
| nested.json | json | 2.550 | 2.593 | 2.760 | 65.656 | 0.17x |
| wide_arrays.json | strata | 2.000 | 2.131 | 2.212 | 72.203 | 1.00x |
| wide_arrays.json | orjson | 2.377 | 2.500 | 2.647 | 72.203 | 0.85x |
| wide_arrays.json | msgspec | 3.221 | 3.353 | 3.486 | 72.203 | 0.64x |
| wide_arrays.json | ujson | 9.320 | 9.619 | 9.952 | 72.203 | 0.22x |
| wide_arrays.json | json | 17.469 | 17.717 | 18.272 | 72.203 | 0.12x |
| mixed.json | strata | 0.160 | 0.173 | 0.190 | 72.203 | 1.00x |
| mixed.json | orjson | 0.181 | 0.193 | 0.214 | 72.203 | 0.90x |
| mixed.json | msgspec | 0.194 | 0.218 | 0.251 | 72.203 | 0.80x |
| mixed.json | ujson | 0.432 | 0.458 | 0.480 | 72.203 | 0.38x |
| mixed.json | json | 0.628 | 0.670 | 0.725 | 72.203 | 0.26x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.071 | 0.076 | 0.084 | 65.316 | 1.00x |
| users.json $[*].id | jmespath | 0.511 | 0.517 | 0.589 | 65.316 | 0.15x |
| users.json $[*].id | jsonpath-ng | 2.953 | 3.189 | 3.331 | 65.316 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.469 | 0.493 | 0.571 | 67.285 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.168 | 3.242 | 3.286 | 67.285 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 21.407 | 22.492 | 26.725 | 67.285 | 0.02x |
| users.json $..total | strata | 1.679 | 1.954 | 2.415 | 69.254 | 1.00x |
| users.json $..total | jsonpath-ng | 389.030 | 391.765 | 396.998 | 69.254 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.225 | 3.238 | 3.341 | 67.285 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.593 | 15.042 | 15.866 | 67.285 | 0.22x |
| users.json $[*].id | orjson+jsonpath-ng | 17.157 | 17.415 | 18.028 | 67.285 | 0.19x |
| users.json $[*].orders[*].total | strata | 3.448 | 3.520 | 3.853 | 68.473 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.698 | 18.493 | 20.573 | 68.473 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 39.025 | 41.379 | 44.024 | 68.473 | 0.09x |
| users.json $..total | strata | 15.862 | 17.670 | 19.007 | 68.492 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 418.807 | 423.622 | 428.922 | 68.492 | 0.04x |

