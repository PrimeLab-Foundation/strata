# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 9c9d784
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
| users.json | strata | 10.617 | 10.801 | 18.035 | 59.438 | 1.00x |
| users.json | orjson | 13.759 | 13.911 | 19.124 | 59.438 | 0.78x |
| users.json | msgspec | 13.763 | 13.995 | 20.880 | 59.438 | 0.77x |
| users.json | ujson | 17.974 | 18.762 | 39.404 | 59.438 | 0.58x |
| users.json | pysimdjson | 18.470 | 18.869 | 37.158 | 59.438 | 0.57x |
| users.json | json | 20.884 | 21.126 | 40.735 | 59.438 | 0.51x |
| flat.json | strata | 0.933 | 0.944 | 0.954 | 69.137 | 1.00x |
| flat.json | orjson | 1.035 | 1.043 | 1.056 | 69.137 | 0.91x |
| flat.json | msgspec | 1.015 | 1.037 | 1.080 | 69.137 | 0.91x |
| flat.json | ujson | 1.444 | 1.480 | 1.569 | 69.137 | 0.64x |
| flat.json | pysimdjson | 1.556 | 1.591 | 1.629 | 69.137 | 0.59x |
| flat.json | json | 1.674 | 1.698 | 1.718 | 69.137 | 0.56x |
| nested.json | strata | 0.852 | 0.865 | 0.874 | 69.137 | 1.00x |
| nested.json | orjson | 0.997 | 1.005 | 1.017 | 69.137 | 0.86x |
| nested.json | msgspec | 0.974 | 0.984 | 1.014 | 69.137 | 0.88x |
| nested.json | ujson | 1.397 | 1.410 | 1.453 | 69.137 | 0.61x |
| nested.json | pysimdjson | 1.367 | 1.385 | 1.438 | 69.137 | 0.62x |
| nested.json | json | 1.820 | 1.847 | 1.859 | 69.137 | 0.47x |
| wide_arrays.json | strata | 5.140 | 5.172 | 5.262 | 71.941 | 1.00x |
| wide_arrays.json | orjson | 5.276 | 5.353 | 5.432 | 71.941 | 0.97x |
| wide_arrays.json | msgspec | 5.920 | 5.983 | 6.084 | 71.941 | 0.86x |
| wide_arrays.json | ujson | 7.559 | 7.586 | 7.767 | 71.941 | 0.68x |
| wide_arrays.json | pysimdjson | 6.202 | 6.262 | 6.331 | 71.941 | 0.83x |
| wide_arrays.json | json | 9.667 | 9.733 | 9.806 | 71.941 | 0.53x |
| mixed.json | strata | 0.199 | 0.202 | 0.208 | 72.004 | 1.00x |
| mixed.json | orjson | 0.230 | 0.244 | 0.262 | 72.004 | 0.83x |
| mixed.json | msgspec | 0.243 | 0.247 | 0.258 | 72.004 | 0.82x |
| mixed.json | ujson | 0.304 | 0.307 | 0.312 | 72.004 | 0.66x |
| mixed.json | pysimdjson | 0.291 | 0.295 | 0.300 | 72.004 | 0.68x |
| mixed.json | json | 0.439 | 0.453 | 0.464 | 72.004 | 0.45x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.714 | 2.747 | 2.785 | 58.535 | 1.00x |
| users.json | orjson | 3.076 | 3.096 | 3.160 | 58.535 | 0.89x |
| users.json | msgspec | 4.151 | 4.170 | 4.197 | 58.535 | 0.66x |
| users.json | ujson | 14.527 | 14.640 | 15.043 | 58.535 | 0.19x |
| users.json | json | 22.181 | 22.409 | 22.476 | 58.535 | 0.12x |
| flat.json | strata | 0.287 | 0.292 | 0.297 | 69.137 | 1.00x |
| flat.json | orjson | 0.360 | 0.367 | 0.383 | 69.137 | 0.79x |
| flat.json | msgspec | 0.472 | 0.484 | 0.507 | 69.137 | 0.60x |
| flat.json | ujson | 1.324 | 1.365 | 1.401 | 69.137 | 0.21x |
| flat.json | json | 1.893 | 1.903 | 1.917 | 69.137 | 0.15x |
| nested.json | strata | 0.239 | 0.244 | 0.253 | 69.137 | 1.00x |
| nested.json | orjson | 0.295 | 0.298 | 0.314 | 69.137 | 0.82x |
| nested.json | msgspec | 0.412 | 0.424 | 0.446 | 69.137 | 0.58x |
| nested.json | ujson | 1.338 | 1.351 | 1.363 | 69.137 | 0.18x |
| nested.json | json | 2.308 | 2.321 | 2.367 | 69.137 | 0.11x |
| wide_arrays.json | strata | 1.741 | 1.755 | 1.761 | 71.941 | 1.00x |
| wide_arrays.json | orjson | 1.853 | 1.865 | 2.059 | 71.941 | 0.94x |
| wide_arrays.json | msgspec | 2.934 | 2.955 | 3.001 | 71.941 | 0.59x |
| wide_arrays.json | ujson | 8.596 | 8.619 | 8.805 | 71.941 | 0.20x |
| wide_arrays.json | json | 16.472 | 16.534 | 16.609 | 71.941 | 0.11x |
| mixed.json | strata | 0.069 | 0.071 | 0.075 | 72.004 | 1.00x |
| mixed.json | orjson | 0.068 | 0.069 | 0.073 | 72.004 | 1.02x |
| mixed.json | msgspec | 0.086 | 0.087 | 0.098 | 72.004 | 0.81x |
| mixed.json | ujson | 0.298 | 0.301 | 0.314 | 72.004 | 0.24x |
| mixed.json | json | 0.505 | 0.516 | 0.526 | 72.004 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.228 | 11.352 | 13.120 | 75.684 | 1.00x |
| users.json | orjson | 14.367 | 14.495 | 15.557 | 75.684 | 0.78x |
| users.json | msgspec | 14.319 | 14.691 | 15.157 | 75.684 | 0.77x |
| users.json | ujson | 19.125 | 19.341 | 21.945 | 75.684 | 0.59x |
| users.json | json | 21.399 | 21.635 | 21.947 | 75.684 | 0.52x |
| flat.json | strata | 0.976 | 0.978 | 0.987 | 69.137 | 1.00x |
| flat.json | orjson | 1.098 | 1.106 | 1.192 | 69.137 | 0.88x |
| flat.json | msgspec | 1.078 | 1.091 | 1.108 | 69.137 | 0.90x |
| flat.json | ujson | 1.531 | 1.546 | 1.606 | 69.137 | 0.63x |
| flat.json | json | 1.734 | 1.748 | 1.784 | 69.137 | 0.56x |
| nested.json | strata | 0.888 | 0.893 | 0.922 | 69.137 | 1.00x |
| nested.json | orjson | 1.048 | 1.065 | 1.092 | 69.137 | 0.84x |
| nested.json | msgspec | 1.027 | 1.048 | 1.070 | 69.137 | 0.85x |
| nested.json | ujson | 1.446 | 1.474 | 1.494 | 69.137 | 0.61x |
| nested.json | json | 1.892 | 1.911 | 1.951 | 69.137 | 0.47x |
| wide_arrays.json | strata | 5.245 | 5.288 | 5.383 | 72.004 | 1.00x |
| wide_arrays.json | orjson | 5.438 | 5.481 | 5.549 | 72.004 | 0.96x |
| wide_arrays.json | msgspec | 6.072 | 6.117 | 6.175 | 72.004 | 0.86x |
| wide_arrays.json | ujson | 7.787 | 7.868 | 8.088 | 72.004 | 0.67x |
| wide_arrays.json | json | 9.756 | 9.820 | 9.907 | 72.004 | 0.54x |
| mixed.json | strata | 0.229 | 0.231 | 0.265 | 72.004 | 1.00x |
| mixed.json | orjson | 0.281 | 0.288 | 0.318 | 72.004 | 0.80x |
| mixed.json | msgspec | 0.287 | 0.291 | 0.354 | 72.004 | 0.80x |
| mixed.json | ujson | 0.356 | 0.362 | 0.376 | 72.004 | 0.64x |
| mixed.json | json | 0.484 | 0.502 | 0.513 | 72.004 | 0.46x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 11.059 | 11.252 | 12.870 | 69.137 | 1.00x |
| users.ndjson | orjson | 17.277 | 17.540 | 17.720 | 69.137 | 0.64x |
| users.ndjson | msgspec | 17.040 | 17.357 | 17.712 | 69.137 | 0.65x |
| users.ndjson | ujson | 21.809 | 22.061 | 22.619 | 69.137 | 0.51x |
| users.ndjson | json | 27.942 | 28.195 | 28.545 | 69.137 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.270 | 3.310 | 3.414 | 71.746 | 1.00x |
| users.json | orjson | 3.689 | 3.726 | 3.992 | 71.746 | 0.89x |
| users.json | msgspec | 4.697 | 4.730 | 4.767 | 71.746 | 0.70x |
| users.json | ujson | 15.255 | 15.415 | 15.725 | 71.746 | 0.21x |
| users.json | json | 22.700 | 23.330 | 23.983 | 71.746 | 0.14x |
| flat.json | strata | 0.437 | 0.454 | 0.467 | 69.137 | 1.00x |
| flat.json | orjson | 0.523 | 0.537 | 0.568 | 69.137 | 0.85x |
| flat.json | msgspec | 0.641 | 0.643 | 0.683 | 69.137 | 0.71x |
| flat.json | ujson | 1.439 | 1.474 | 1.552 | 69.137 | 0.31x |
| flat.json | json | 2.054 | 2.070 | 2.088 | 69.137 | 0.22x |
| nested.json | strata | 0.361 | 0.368 | 0.377 | 69.137 | 1.00x |
| nested.json | orjson | 0.442 | 0.447 | 0.474 | 69.137 | 0.82x |
| nested.json | msgspec | 0.561 | 0.567 | 0.599 | 69.137 | 0.65x |
| nested.json | ujson | 1.490 | 1.512 | 1.552 | 69.137 | 0.24x |
| nested.json | json | 2.476 | 2.519 | 2.574 | 69.137 | 0.15x |
| wide_arrays.json | strata | 2.126 | 2.145 | 3.081 | 72.004 | 1.00x |
| wide_arrays.json | orjson | 2.283 | 2.301 | 3.291 | 72.004 | 0.93x |
| wide_arrays.json | msgspec | 3.364 | 3.385 | 3.397 | 72.004 | 0.63x |
| wide_arrays.json | ujson | 9.001 | 9.110 | 9.263 | 72.004 | 0.24x |
| wide_arrays.json | json | 16.957 | 17.027 | 17.581 | 72.004 | 0.13x |
| mixed.json | strata | 0.161 | 0.166 | 0.183 | 72.004 | 1.00x |
| mixed.json | orjson | 0.180 | 0.184 | 0.206 | 72.004 | 0.90x |
| mixed.json | msgspec | 0.193 | 0.203 | 0.235 | 72.004 | 0.82x |
| mixed.json | ujson | 0.426 | 0.435 | 0.460 | 72.004 | 0.38x |
| mixed.json | json | 0.629 | 0.648 | 0.757 | 72.004 | 0.26x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.070 | 0.071 | 0.074 | 71.746 | 1.00x |
| users.json $[*].id | jmespath | 0.457 | 0.463 | 0.470 | 71.746 | 0.15x |
| users.json $[*].id | jsonpath-ng | 2.684 | 2.734 | 2.767 | 71.746 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.420 | 0.445 | 0.487 | 72.734 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.913 | 2.982 | 3.003 | 72.734 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 18.764 | 19.139 | 19.670 | 72.734 | 0.02x |
| users.json $..total | strata | 1.782 | 1.811 | 1.849 | 74.703 | 1.00x |
| users.json $..total | jsonpath-ng | 380.313 | 382.375 | 389.415 | 74.703 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.352 | 3.375 | 3.505 | 72.734 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.708 | 14.896 | 15.038 | 72.734 | 0.23x |
| users.json $[*].id | orjson+jsonpath-ng | 16.954 | 17.125 | 17.291 | 72.734 | 0.20x |
| users.json $[*].orders[*].total | strata | 3.551 | 3.581 | 3.614 | 73.906 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.449 | 17.666 | 17.928 | 73.906 | 0.20x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 37.685 | 38.605 | 38.821 | 73.906 | 0.09x |
| users.json $..total | strata | 13.585 | 13.957 | 14.416 | 73.926 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 401.796 | 405.612 | 419.833 | 73.926 | 0.03x |

