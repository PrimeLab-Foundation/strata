# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 6520b15
- python: 3.12.13
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
| users.json | strata | 7.688 | 7.855 | 14.562 | 58.355 | 1.00x |
| users.json | orjson | 10.916 | 11.139 | 18.009 | 58.355 | 0.71x |
| users.json | msgspec | 11.033 | 11.397 | 16.240 | 58.355 | 0.69x |
| users.json | ujson | 15.124 | 15.504 | 18.422 | 58.355 | 0.51x |
| users.json | pysimdjson | 14.612 | 15.053 | 21.541 | 58.355 | 0.52x |
| users.json | json | 16.936 | 17.374 | 19.479 | 58.355 | 0.45x |
| flat.json | strata | 0.733 | 0.751 | 0.791 | 72.105 | 1.00x |
| flat.json | orjson | 0.773 | 0.793 | 0.912 | 72.105 | 0.95x |
| flat.json | msgspec | 0.740 | 0.766 | 0.795 | 72.105 | 0.98x |
| flat.json | ujson | 1.116 | 1.135 | 1.211 | 72.105 | 0.66x |
| flat.json | pysimdjson | 1.258 | 1.284 | 1.311 | 72.105 | 0.59x |
| flat.json | json | 1.363 | 1.386 | 1.424 | 72.105 | 0.54x |
| nested.json | strata | 0.590 | 0.616 | 0.640 | 72.105 | 1.00x |
| nested.json | orjson | 0.680 | 0.705 | 0.783 | 72.105 | 0.87x |
| nested.json | msgspec | 0.682 | 0.696 | 0.709 | 72.105 | 0.88x |
| nested.json | ujson | 1.015 | 1.050 | 1.139 | 72.105 | 0.59x |
| nested.json | pysimdjson | 0.983 | 0.997 | 1.034 | 72.105 | 0.62x |
| nested.json | json | 1.363 | 1.376 | 1.404 | 72.105 | 0.45x |
| wide_arrays.json | strata | 3.493 | 3.549 | 3.634 | 76.109 | 1.00x |
| wide_arrays.json | orjson | 3.868 | 3.911 | 4.063 | 76.109 | 0.91x |
| wide_arrays.json | msgspec | 4.213 | 4.389 | 4.574 | 76.109 | 0.81x |
| wide_arrays.json | ujson | 5.524 | 5.596 | 5.911 | 76.109 | 0.63x |
| wide_arrays.json | pysimdjson | 4.289 | 4.390 | 4.573 | 76.109 | 0.81x |
| wide_arrays.json | json | 7.145 | 7.181 | 7.349 | 76.109 | 0.49x |
| mixed.json | strata | 0.153 | 0.167 | 0.180 | 76.172 | 1.00x |
| mixed.json | orjson | 0.177 | 0.196 | 0.219 | 76.172 | 0.85x |
| mixed.json | msgspec | 0.180 | 0.200 | 0.220 | 76.172 | 0.83x |
| mixed.json | ujson | 0.242 | 0.259 | 0.301 | 76.172 | 0.64x |
| mixed.json | pysimdjson | 0.235 | 0.249 | 0.266 | 76.172 | 0.67x |
| mixed.json | json | 0.348 | 0.366 | 0.441 | 76.172 | 0.46x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.821 | 2.060 | 2.417 | 57.480 | 1.00x |
| users.json | orjson | 1.729 | 2.070 | 2.100 | 57.480 | 1.00x |
| users.json | msgspec | 2.925 | 3.253 | 3.387 | 57.480 | 0.63x |
| users.json | ujson | 10.464 | 10.902 | 12.023 | 57.480 | 0.19x |
| users.json | json | 15.480 | 16.027 | 17.367 | 57.480 | 0.13x |
| flat.json | strata | 0.203 | 0.215 | 0.226 | 72.105 | 1.00x |
| flat.json | orjson | 0.214 | 0.215 | 0.234 | 72.105 | 1.00x |
| flat.json | msgspec | 0.342 | 0.353 | 0.395 | 72.105 | 0.61x |
| flat.json | ujson | 0.916 | 0.942 | 1.054 | 72.105 | 0.23x |
| flat.json | json | 1.364 | 1.389 | 1.618 | 72.105 | 0.15x |
| nested.json | strata | 0.137 | 0.153 | 0.184 | 72.105 | 1.00x |
| nested.json | orjson | 0.192 | 0.210 | 0.243 | 72.105 | 0.73x |
| nested.json | msgspec | 0.294 | 0.314 | 0.351 | 72.105 | 0.49x |
| nested.json | ujson | 0.948 | 0.981 | 1.081 | 72.105 | 0.16x |
| nested.json | json | 1.657 | 1.752 | 1.910 | 72.105 | 0.09x |
| wide_arrays.json | strata | 1.199 | 1.207 | 1.233 | 76.109 | 1.00x |
| wide_arrays.json | orjson | 1.153 | 1.183 | 1.326 | 76.109 | 1.02x |
| wide_arrays.json | msgspec | 1.972 | 1.987 | 2.138 | 76.109 | 0.61x |
| wide_arrays.json | ujson | 5.848 | 5.879 | 6.427 | 76.109 | 0.21x |
| wide_arrays.json | json | 10.928 | 11.013 | 12.872 | 76.109 | 0.11x |
| mixed.json | strata | 0.052 | 0.055 | 0.073 | 76.172 | 1.00x |
| mixed.json | orjson | 0.049 | 0.052 | 0.064 | 76.172 | 1.07x |
| mixed.json | msgspec | 0.068 | 0.076 | 0.093 | 76.172 | 0.73x |
| mixed.json | ujson | 0.225 | 0.257 | 0.288 | 76.172 | 0.21x |
| mixed.json | json | 0.388 | 0.443 | 0.534 | 76.172 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.320 | 9.043 | 10.370 | 71.766 | 1.00x |
| users.json | orjson | 11.588 | 12.394 | 13.248 | 71.766 | 0.73x |
| users.json | msgspec | 11.708 | 12.753 | 13.262 | 71.766 | 0.71x |
| users.json | ujson | 16.268 | 17.681 | 19.024 | 71.766 | 0.51x |
| users.json | json | 17.735 | 18.592 | 19.597 | 71.766 | 0.49x |
| flat.json | strata | 0.786 | 0.796 | 0.841 | 72.105 | 1.00x |
| flat.json | orjson | 0.847 | 0.860 | 0.959 | 72.105 | 0.93x |
| flat.json | msgspec | 0.824 | 0.848 | 0.949 | 72.105 | 0.94x |
| flat.json | ujson | 1.224 | 1.268 | 1.381 | 72.105 | 0.63x |
| flat.json | json | 1.415 | 1.470 | 1.581 | 72.105 | 0.54x |
| nested.json | strata | 0.636 | 0.649 | 0.699 | 72.105 | 1.00x |
| nested.json | orjson | 0.731 | 0.759 | 0.857 | 72.105 | 0.85x |
| nested.json | msgspec | 0.716 | 0.756 | 0.847 | 72.105 | 0.86x |
| nested.json | ujson | 1.073 | 1.109 | 1.213 | 72.105 | 0.59x |
| nested.json | json | 1.395 | 1.453 | 1.590 | 72.105 | 0.45x |
| wide_arrays.json | strata | 3.589 | 3.643 | 3.958 | 76.172 | 1.00x |
| wide_arrays.json | orjson | 3.939 | 4.002 | 4.341 | 76.172 | 0.91x |
| wide_arrays.json | msgspec | 4.360 | 4.489 | 4.685 | 76.172 | 0.81x |
| wide_arrays.json | ujson | 5.811 | 6.084 | 6.248 | 76.172 | 0.60x |
| wide_arrays.json | json | 7.265 | 7.356 | 7.675 | 76.172 | 0.50x |
| mixed.json | strata | 0.195 | 0.200 | 0.259 | 76.172 | 1.00x |
| mixed.json | orjson | 0.249 | 0.253 | 0.308 | 76.172 | 0.79x |
| mixed.json | msgspec | 0.251 | 0.254 | 0.309 | 76.172 | 0.79x |
| mixed.json | ujson | 0.357 | 0.364 | 0.385 | 76.172 | 0.55x |
| mixed.json | json | 0.441 | 0.457 | 0.511 | 76.172 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 8.437 | 8.998 | 10.426 | 72.105 | 1.00x |
| users.ndjson | orjson | 12.665 | 13.371 | 15.466 | 72.105 | 0.67x |
| users.ndjson | msgspec | 12.418 | 13.166 | 15.550 | 72.105 | 0.68x |
| users.ndjson | ujson | 16.371 | 17.422 | 20.551 | 72.105 | 0.52x |
| users.ndjson | json | 20.788 | 22.855 | 24.982 | 72.105 | 0.39x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.279 | 2.563 | 43.907 | 69.797 | 1.00x |
| users.json | orjson | 2.405 | 2.668 | 2.905 | 69.797 | 0.96x |
| users.json | msgspec | 3.679 | 3.908 | 89.755 | 69.797 | 0.66x |
| users.json | ujson | 11.167 | 12.105 | 12.246 | 69.797 | 0.21x |
| users.json | json | 16.434 | 18.398 | 139.169 | 69.797 | 0.14x |
| flat.json | strata | 0.307 | 0.321 | 0.403 | 72.105 | 1.00x |
| flat.json | orjson | 0.328 | 0.341 | 0.418 | 72.105 | 0.94x |
| flat.json | msgspec | 0.456 | 0.483 | 0.563 | 72.105 | 0.67x |
| flat.json | ujson | 1.063 | 1.098 | 154.254 | 72.105 | 0.29x |
| flat.json | json | 1.503 | 1.552 | 1.713 | 72.105 | 0.21x |
| nested.json | strata | 0.231 | 0.260 | 12.523 | 72.105 | 1.00x |
| nested.json | orjson | 0.293 | 0.333 | 69.538 | 72.105 | 0.78x |
| nested.json | msgspec | 0.381 | 0.441 | 0.492 | 72.105 | 0.59x |
| nested.json | ujson | 1.078 | 1.122 | 251.381 | 72.105 | 0.23x |
| nested.json | json | 1.820 | 1.888 | 25.775 | 72.105 | 0.14x |
| wide_arrays.json | strata | 1.612 | 1.790 | 17.075 | 76.172 | 1.00x |
| wide_arrays.json | orjson | 1.629 | 1.751 | 16.617 | 76.172 | 1.02x |
| wide_arrays.json | msgspec | 2.410 | 2.609 | 8.223 | 76.172 | 0.69x |
| wide_arrays.json | ujson | 6.300 | 6.856 | 21.541 | 76.172 | 0.26x |
| wide_arrays.json | json | 11.838 | 15.730 | 40.288 | 76.172 | 0.11x |
| mixed.json | strata | 0.110 | 0.122 | 0.192 | 76.172 | 1.00x |
| mixed.json | orjson | 0.118 | 0.121 | 0.168 | 76.172 | 1.00x |
| mixed.json | msgspec | 0.141 | 0.149 | 0.191 | 76.172 | 0.82x |
| mixed.json | ujson | 0.308 | 0.326 | 0.393 | 76.172 | 0.37x |
| mixed.json | json | 0.475 | 0.523 | 160.461 | 76.172 | 0.23x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.065 | 0.071 | 0.075 | 69.797 | 1.00x |
| users.json $[*].id | jmespath | 0.365 | 0.396 | 0.434 | 69.797 | 0.18x |
| users.json $[*].id | jsonpath-ng | 1.835 | 2.059 | 2.119 | 69.797 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.444 | 0.465 | 0.502 | 69.820 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.214 | 2.356 | 2.503 | 69.820 | 0.20x |
| users.json $[*].orders[*].total | jsonpath-ng | 12.377 | 13.175 | 13.909 | 69.820 | 0.04x |
| users.json $..total | strata | 1.388 | 1.498 | 1.585 | 73.734 | 1.00x |
| users.json $..total | jsonpath-ng | 236.301 | 239.997 | 269.614 | 73.734 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 2.363 | 2.671 | 2.822 | 69.820 | 1.00x |
| users.json $[*].id | orjson+jmespath | 11.825 | 12.177 | 13.283 | 69.820 | 0.22x |
| users.json $[*].id | orjson+jsonpath-ng | 12.878 | 13.779 | 14.434 | 69.820 | 0.19x |
| users.json $[*].orders[*].total | strata | 2.557 | 2.883 | 3.139 | 73.734 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 13.738 | 14.417 | 15.995 | 73.734 | 0.20x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 28.174 | 30.028 | 31.060 | 73.734 | 0.10x |
| users.json $..total | strata | 11.485 | 13.315 | 14.724 | 73.734 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 256.139 | 258.348 | 263.218 | 73.734 | 0.05x |

