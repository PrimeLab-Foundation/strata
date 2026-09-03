# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: a4d78c1
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
| users.json | strata | 9.867 | 10.022 | 12.470 | 60.086 | 1.00x |
| users.json | orjson | 13.987 | 14.163 | 20.601 | 60.086 | 0.71x |
| users.json | msgspec | 14.194 | 14.426 | 19.835 | 60.086 | 0.69x |
| users.json | ujson | 18.974 | 19.183 | 28.182 | 60.086 | 0.52x |
| users.json | pysimdjson | 18.328 | 19.449 | 28.249 | 60.086 | 0.52x |
| users.json | json | 21.671 | 22.259 | 30.735 | 60.086 | 0.45x |
| flat.json | strata | 0.919 | 0.941 | 0.978 | 71.191 | 1.00x |
| flat.json | orjson | 1.094 | 1.113 | 1.148 | 71.191 | 0.85x |
| flat.json | msgspec | 1.025 | 1.050 | 1.183 | 71.191 | 0.90x |
| flat.json | ujson | 1.498 | 1.521 | 1.588 | 71.191 | 0.62x |
| flat.json | pysimdjson | 1.672 | 1.686 | 1.792 | 71.191 | 0.56x |
| flat.json | json | 1.821 | 1.854 | 1.967 | 71.191 | 0.51x |
| nested.json | strata | 0.747 | 0.761 | 0.777 | 71.191 | 1.00x |
| nested.json | orjson | 0.906 | 0.927 | 0.941 | 71.191 | 0.82x |
| nested.json | msgspec | 0.942 | 0.954 | 0.978 | 71.191 | 0.80x |
| nested.json | ujson | 1.352 | 1.371 | 1.383 | 71.191 | 0.56x |
| nested.json | pysimdjson | 1.299 | 1.310 | 1.409 | 71.191 | 0.58x |
| nested.json | json | 1.846 | 1.853 | 1.930 | 71.191 | 0.41x |
| wide_arrays.json | strata | 4.301 | 4.322 | 4.522 | 76.441 | 1.00x |
| wide_arrays.json | orjson | 5.018 | 5.179 | 5.823 | 76.441 | 0.83x |
| wide_arrays.json | msgspec | 5.472 | 5.548 | 5.711 | 76.441 | 0.78x |
| wide_arrays.json | ujson | 7.095 | 7.224 | 7.350 | 76.441 | 0.60x |
| wide_arrays.json | pysimdjson | 5.543 | 5.602 | 5.684 | 76.441 | 0.77x |
| wide_arrays.json | json | 9.307 | 9.402 | 9.608 | 76.441 | 0.46x |
| mixed.json | strata | 0.192 | 0.197 | 0.228 | 76.504 | 1.00x |
| mixed.json | orjson | 0.236 | 0.243 | 0.280 | 76.504 | 0.81x |
| mixed.json | msgspec | 0.247 | 0.250 | 0.281 | 76.504 | 0.79x |
| mixed.json | ujson | 0.314 | 0.318 | 0.343 | 76.504 | 0.62x |
| mixed.json | pysimdjson | 0.301 | 0.310 | 0.318 | 76.504 | 0.64x |
| mixed.json | json | 0.457 | 0.475 | 0.483 | 76.504 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.130 | 2.191 | 2.348 | 59.184 | 1.00x |
| users.json | orjson | 2.217 | 2.255 | 2.419 | 59.184 | 0.97x |
| users.json | msgspec | 3.806 | 3.860 | 3.945 | 59.184 | 0.57x |
| users.json | ujson | 13.914 | 14.082 | 16.126 | 59.184 | 0.16x |
| users.json | json | 20.876 | 21.115 | 28.610 | 59.184 | 0.10x |
| flat.json | strata | 0.279 | 0.286 | 0.297 | 71.191 | 1.00x |
| flat.json | orjson | 0.271 | 0.274 | 0.300 | 71.191 | 1.05x |
| flat.json | msgspec | 0.428 | 0.440 | 0.466 | 71.191 | 0.65x |
| flat.json | ujson | 1.208 | 1.219 | 1.261 | 71.191 | 0.23x |
| flat.json | json | 1.790 | 1.820 | 1.857 | 71.191 | 0.16x |
| nested.json | strata | 0.192 | 0.198 | 0.213 | 71.191 | 1.00x |
| nested.json | orjson | 0.239 | 0.256 | 0.264 | 71.191 | 0.78x |
| nested.json | msgspec | 0.376 | 0.387 | 0.416 | 71.191 | 0.51x |
| nested.json | ujson | 1.249 | 1.282 | 1.720 | 71.191 | 0.15x |
| nested.json | json | 2.270 | 2.294 | 2.406 | 71.191 | 0.09x |
| wide_arrays.json | strata | 1.512 | 1.525 | 1.592 | 76.441 | 1.00x |
| wide_arrays.json | orjson | 1.393 | 1.403 | 1.463 | 76.441 | 1.09x |
| wide_arrays.json | msgspec | 2.585 | 2.621 | 2.671 | 76.441 | 0.58x |
| wide_arrays.json | ujson | 7.691 | 7.764 | 7.994 | 76.441 | 0.20x |
| wide_arrays.json | json | 15.006 | 15.169 | 15.543 | 76.441 | 0.10x |
| mixed.json | strata | 0.061 | 0.064 | 0.070 | 76.504 | 1.00x |
| mixed.json | orjson | 0.056 | 0.058 | 0.059 | 76.504 | 1.11x |
| mixed.json | msgspec | 0.078 | 0.082 | 0.092 | 76.504 | 0.79x |
| mixed.json | ujson | 0.293 | 0.309 | 0.335 | 76.504 | 0.21x |
| mixed.json | json | 0.522 | 0.531 | 0.545 | 76.504 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.438 | 10.778 | 11.954 | 74.789 | 1.00x |
| users.json | orjson | 14.588 | 15.170 | 16.762 | 74.789 | 0.71x |
| users.json | msgspec | 14.645 | 14.868 | 16.490 | 74.789 | 0.72x |
| users.json | ujson | 19.930 | 20.867 | 23.816 | 74.789 | 0.52x |
| users.json | json | 22.160 | 22.660 | 25.438 | 74.789 | 0.48x |
| flat.json | strata | 0.970 | 1.011 | 1.269 | 71.191 | 1.00x |
| flat.json | orjson | 1.167 | 1.225 | 1.441 | 71.191 | 0.83x |
| flat.json | msgspec | 1.121 | 1.163 | 1.396 | 71.191 | 0.87x |
| flat.json | ujson | 1.611 | 1.716 | 3.158 | 71.191 | 0.59x |
| flat.json | json | 1.910 | 1.962 | 2.662 | 71.191 | 0.52x |
| nested.json | strata | 0.785 | 0.808 | 0.826 | 71.191 | 1.00x |
| nested.json | orjson | 0.984 | 0.993 | 1.062 | 71.191 | 0.81x |
| nested.json | msgspec | 1.007 | 1.028 | 1.045 | 71.191 | 0.79x |
| nested.json | ujson | 1.449 | 1.478 | 1.525 | 71.191 | 0.55x |
| nested.json | json | 1.900 | 1.947 | 2.060 | 71.191 | 0.41x |
| wide_arrays.json | strata | 4.421 | 4.453 | 4.599 | 76.504 | 1.00x |
| wide_arrays.json | orjson | 5.061 | 5.253 | 5.448 | 76.504 | 0.85x |
| wide_arrays.json | msgspec | 5.678 | 5.731 | 5.938 | 76.504 | 0.78x |
| wide_arrays.json | ujson | 7.377 | 7.474 | 7.614 | 76.504 | 0.60x |
| wide_arrays.json | json | 9.501 | 9.546 | 9.792 | 76.504 | 0.47x |
| mixed.json | strata | 0.209 | 0.214 | 0.254 | 76.504 | 1.00x |
| mixed.json | orjson | 0.289 | 0.302 | 0.334 | 76.504 | 0.71x |
| mixed.json | msgspec | 0.303 | 0.311 | 0.336 | 76.504 | 0.69x |
| mixed.json | ujson | 0.385 | 0.394 | 0.445 | 76.504 | 0.54x |
| mixed.json | json | 0.512 | 0.527 | 0.554 | 76.504 | 0.41x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.253 | 10.467 | 11.191 | 71.191 | 1.00x |
| users.ndjson | orjson | 16.544 | 16.920 | 17.593 | 71.191 | 0.62x |
| users.ndjson | msgspec | 16.361 | 16.509 | 16.894 | 71.191 | 0.63x |
| users.ndjson | ujson | 21.227 | 21.522 | 21.963 | 71.191 | 0.49x |
| users.ndjson | json | 26.727 | 27.495 | 28.559 | 71.191 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.802 | 2.848 | 3.101 | 69.867 | 1.00x |
| users.json | orjson | 2.884 | 2.977 | 4.015 | 69.867 | 0.96x |
| users.json | msgspec | 4.445 | 4.574 | 41.967 | 69.867 | 0.62x |
| users.json | ujson | 14.670 | 14.995 | 15.567 | 69.867 | 0.19x |
| users.json | json | 21.578 | 21.952 | 22.285 | 69.867 | 0.13x |
| flat.json | strata | 0.425 | 0.442 | 0.468 | 71.191 | 1.00x |
| flat.json | orjson | 0.438 | 0.453 | 0.540 | 71.191 | 0.97x |
| flat.json | msgspec | 0.593 | 0.627 | 0.690 | 71.191 | 0.70x |
| flat.json | ujson | 1.385 | 1.417 | 1.462 | 71.191 | 0.31x |
| flat.json | json | 1.970 | 2.028 | 2.079 | 71.191 | 0.22x |
| nested.json | strata | 0.310 | 0.329 | 0.351 | 71.191 | 1.00x |
| nested.json | orjson | 0.394 | 0.421 | 0.441 | 71.191 | 0.78x |
| nested.json | msgspec | 0.533 | 0.542 | 0.568 | 71.191 | 0.61x |
| nested.json | ujson | 1.427 | 1.443 | 1.506 | 71.191 | 0.23x |
| nested.json | json | 2.428 | 2.455 | 2.567 | 71.191 | 0.13x |
| wide_arrays.json | strata | 1.899 | 2.021 | 18.976 | 76.504 | 1.00x |
| wide_arrays.json | orjson | 1.810 | 1.847 | 15.764 | 76.504 | 1.09x |
| wide_arrays.json | msgspec | 2.991 | 3.073 | 28.031 | 76.504 | 0.66x |
| wide_arrays.json | ujson | 8.205 | 8.312 | 31.777 | 76.504 | 0.24x |
| wide_arrays.json | json | 15.681 | 15.860 | 23.254 | 76.504 | 0.13x |
| mixed.json | strata | 0.169 | 0.180 | 0.354 | 76.504 | 1.00x |
| mixed.json | orjson | 0.173 | 0.186 | 0.214 | 76.504 | 0.97x |
| mixed.json | msgspec | 0.193 | 0.207 | 0.240 | 76.504 | 0.87x |
| mixed.json | ujson | 0.434 | 0.443 | 0.519 | 76.504 | 0.41x |
| mixed.json | json | 0.649 | 0.667 | 0.733 | 76.504 | 0.27x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.068 | 0.072 | 0.093 | 69.867 | 1.00x |
| users.json $[*].id | jmespath | 0.475 | 0.488 | 0.501 | 69.867 | 0.15x |
| users.json $[*].id | jsonpath-ng | 2.406 | 2.493 | 2.586 | 69.867 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.470 | 0.487 | 0.684 | 71.836 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.884 | 2.950 | 2.996 | 71.836 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 16.193 | 16.331 | 16.616 | 71.836 | 0.03x |
| users.json $..total | strata | 1.749 | 1.766 | 1.818 | 73.805 | 1.00x |
| users.json $..total | jsonpath-ng | 322.766 | 325.222 | 329.443 | 73.805 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.023 | 3.152 | 3.292 | 71.836 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.722 | 15.115 | 15.402 | 71.836 | 0.21x |
| users.json $[*].id | orjson+jsonpath-ng | 16.467 | 16.811 | 18.970 | 71.836 | 0.19x |
| users.json $[*].orders[*].total | strata | 3.244 | 3.372 | 4.196 | 73.008 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.150 | 17.630 | 23.342 | 73.008 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 35.762 | 36.652 | 38.920 | 73.008 | 0.09x |
| users.json $..total | strata | 13.389 | 13.903 | 14.909 | 73.023 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 342.570 | 346.276 | 365.753 | 73.023 | 0.04x |

