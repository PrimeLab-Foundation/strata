# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 1cc7049
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
| users.json | strata | 9.005 | 9.433 | 12.286 | 58.773 | 1.00x |
| users.json | orjson | 11.985 | 12.331 | 15.295 | 58.773 | 0.76x |
| users.json | msgspec | 12.263 | 12.597 | 14.778 | 58.773 | 0.75x |
| users.json | ujson | 15.994 | 17.247 | 20.880 | 58.773 | 0.55x |
| users.json | pysimdjson | 16.525 | 17.455 | 20.385 | 58.773 | 0.54x |
| users.json | json | 20.775 | 21.099 | 28.487 | 58.773 | 0.45x |
| flat.json | strata | 0.788 | 0.794 | 0.809 | 71.809 | 1.00x |
| flat.json | orjson | 0.918 | 0.923 | 0.972 | 71.809 | 0.86x |
| flat.json | msgspec | 0.916 | 0.926 | 0.941 | 71.809 | 0.86x |
| flat.json | ujson | 1.320 | 1.328 | 1.346 | 71.809 | 0.60x |
| flat.json | pysimdjson | 1.394 | 1.409 | 1.426 | 71.809 | 0.56x |
| flat.json | json | 1.765 | 1.774 | 1.785 | 71.809 | 0.45x |
| nested.json | strata | 0.733 | 0.745 | 0.754 | 71.809 | 1.00x |
| nested.json | orjson | 0.921 | 0.924 | 0.937 | 71.809 | 0.81x |
| nested.json | msgspec | 0.933 | 0.942 | 0.956 | 71.809 | 0.79x |
| nested.json | ujson | 1.316 | 1.330 | 1.340 | 71.809 | 0.56x |
| nested.json | pysimdjson | 1.282 | 1.301 | 1.416 | 71.809 | 0.57x |
| nested.json | json | 1.869 | 1.881 | 1.931 | 71.809 | 0.40x |
| wide_arrays.json | strata | 4.063 | 4.233 | 4.495 | 74.879 | 1.00x |
| wide_arrays.json | orjson | 4.683 | 4.806 | 5.153 | 74.879 | 0.88x |
| wide_arrays.json | msgspec | 5.240 | 5.378 | 5.661 | 74.879 | 0.79x |
| wide_arrays.json | ujson | 6.436 | 6.617 | 7.019 | 74.879 | 0.64x |
| wide_arrays.json | pysimdjson | 5.544 | 5.669 | 6.136 | 74.879 | 0.75x |
| wide_arrays.json | json | 9.042 | 9.220 | 9.786 | 74.879 | 0.46x |
| mixed.json | strata | 0.175 | 0.188 | 0.200 | 74.941 | 1.00x |
| mixed.json | orjson | 0.209 | 0.211 | 0.242 | 74.941 | 0.89x |
| mixed.json | msgspec | 0.217 | 0.221 | 0.238 | 74.941 | 0.85x |
| mixed.json | ujson | 0.275 | 0.279 | 0.315 | 74.941 | 0.68x |
| mixed.json | pysimdjson | 0.275 | 0.278 | 0.300 | 74.941 | 0.68x |
| mixed.json | json | 0.435 | 0.438 | 0.478 | 74.941 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.235 | 2.299 | 2.439 | 57.871 | 1.00x |
| users.json | orjson | 2.658 | 2.700 | 2.863 | 57.871 | 0.85x |
| users.json | msgspec | 3.537 | 3.604 | 3.845 | 57.871 | 0.64x |
| users.json | ujson | 10.571 | 10.994 | 11.362 | 57.871 | 0.21x |
| users.json | json | 19.763 | 20.221 | 21.429 | 57.871 | 0.11x |
| flat.json | strata | 0.241 | 0.249 | 0.292 | 71.809 | 1.00x |
| flat.json | orjson | 0.307 | 0.318 | 0.348 | 71.809 | 0.78x |
| flat.json | msgspec | 0.395 | 0.407 | 0.443 | 71.809 | 0.61x |
| flat.json | ujson | 0.936 | 0.940 | 1.059 | 71.809 | 0.26x |
| flat.json | json | 1.693 | 1.706 | 1.851 | 71.809 | 0.15x |
| nested.json | strata | 0.234 | 0.235 | 0.236 | 71.809 | 1.00x |
| nested.json | orjson | 0.273 | 0.274 | 0.285 | 71.809 | 0.86x |
| nested.json | msgspec | 0.372 | 0.383 | 0.407 | 71.809 | 0.61x |
| nested.json | ujson | 1.026 | 1.032 | 1.073 | 71.809 | 0.23x |
| nested.json | json | 2.173 | 2.193 | 2.322 | 71.809 | 0.11x |
| wide_arrays.json | strata | 1.343 | 1.411 | 1.466 | 74.879 | 1.00x |
| wide_arrays.json | orjson | 1.613 | 1.629 | 1.791 | 74.879 | 0.87x |
| wide_arrays.json | msgspec | 2.444 | 2.473 | 2.674 | 74.879 | 0.57x |
| wide_arrays.json | ujson | 5.762 | 5.965 | 6.258 | 74.879 | 0.24x |
| wide_arrays.json | json | 15.148 | 15.632 | 16.215 | 74.879 | 0.09x |
| mixed.json | strata | 0.056 | 0.057 | 0.062 | 74.941 | 1.00x |
| mixed.json | orjson | 0.059 | 0.059 | 0.066 | 74.941 | 0.97x |
| mixed.json | msgspec | 0.076 | 0.076 | 0.101 | 74.941 | 0.75x |
| mixed.json | ujson | 0.216 | 0.219 | 0.239 | 74.941 | 0.26x |
| mixed.json | json | 0.467 | 0.482 | 0.512 | 74.941 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.395 | 9.686 | 11.825 | 77.441 | 1.00x |
| users.json | orjson | 12.383 | 12.700 | 13.861 | 77.441 | 0.76x |
| users.json | msgspec | 12.704 | 13.104 | 13.456 | 77.441 | 0.74x |
| users.json | ujson | 17.476 | 18.699 | 19.429 | 77.441 | 0.52x |
| users.json | json | 21.059 | 21.375 | 23.487 | 77.441 | 0.45x |
| flat.json | strata | 0.818 | 0.820 | 0.856 | 71.809 | 1.00x |
| flat.json | orjson | 0.969 | 0.977 | 1.056 | 71.809 | 0.84x |
| flat.json | msgspec | 0.979 | 0.988 | 1.053 | 71.809 | 0.83x |
| flat.json | ujson | 1.410 | 1.419 | 1.437 | 71.809 | 0.58x |
| flat.json | json | 1.816 | 1.819 | 1.864 | 71.809 | 0.45x |
| nested.json | strata | 0.752 | 0.766 | 0.780 | 71.809 | 1.00x |
| nested.json | orjson | 0.964 | 0.969 | 1.024 | 71.809 | 0.79x |
| nested.json | msgspec | 0.982 | 0.992 | 1.055 | 71.809 | 0.77x |
| nested.json | ujson | 1.387 | 1.398 | 1.438 | 71.809 | 0.55x |
| nested.json | json | 1.906 | 1.921 | 1.933 | 71.809 | 0.40x |
| wide_arrays.json | strata | 4.119 | 4.323 | 4.525 | 74.941 | 1.00x |
| wide_arrays.json | orjson | 4.668 | 4.878 | 5.376 | 74.941 | 0.89x |
| wide_arrays.json | msgspec | 5.245 | 5.449 | 5.989 | 74.941 | 0.79x |
| wide_arrays.json | ujson | 6.607 | 6.777 | 7.065 | 74.941 | 0.64x |
| wide_arrays.json | json | 9.038 | 9.130 | 9.362 | 74.941 | 0.47x |
| mixed.json | strata | 0.191 | 0.200 | 0.229 | 74.941 | 1.00x |
| mixed.json | orjson | 0.249 | 0.259 | 0.273 | 74.941 | 0.77x |
| mixed.json | msgspec | 0.258 | 0.261 | 0.278 | 74.941 | 0.76x |
| mixed.json | ujson | 0.326 | 0.342 | 0.353 | 74.941 | 0.58x |
| mixed.json | json | 0.474 | 0.483 | 0.533 | 74.941 | 0.41x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.262 | 9.454 | 9.761 | 71.809 | 1.00x |
| users.ndjson | orjson | 15.273 | 15.568 | 16.560 | 71.809 | 0.61x |
| users.ndjson | msgspec | 15.354 | 15.611 | 16.549 | 71.809 | 0.61x |
| users.ndjson | ujson | 19.345 | 20.009 | 22.586 | 71.809 | 0.47x |
| users.ndjson | json | 26.804 | 27.400 | 28.979 | 71.809 | 0.35x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.727 | 2.752 | 3.001 | 73.441 | 1.00x |
| users.json | orjson | 3.246 | 3.318 | 3.417 | 73.441 | 0.83x |
| users.json | msgspec | 4.497 | 4.652 | 4.805 | 73.441 | 0.59x |
| users.json | ujson | 11.500 | 11.786 | 12.761 | 73.441 | 0.23x |
| users.json | json | 20.211 | 20.523 | 21.793 | 73.441 | 0.13x |
| flat.json | strata | 0.371 | 0.374 | 0.420 | 71.809 | 1.00x |
| flat.json | orjson | 0.449 | 0.487 | 0.555 | 71.809 | 0.77x |
| flat.json | msgspec | 0.539 | 0.576 | 0.620 | 71.809 | 0.65x |
| flat.json | ujson | 1.112 | 1.191 | 1.234 | 71.809 | 0.31x |
| flat.json | json | 1.861 | 1.931 | 2.075 | 71.809 | 0.19x |
| nested.json | strata | 0.340 | 0.345 | 0.397 | 71.809 | 1.00x |
| nested.json | orjson | 0.396 | 0.412 | 0.471 | 71.809 | 0.84x |
| nested.json | msgspec | 0.493 | 0.508 | 0.549 | 71.809 | 0.68x |
| nested.json | ujson | 1.174 | 1.232 | 1.296 | 71.809 | 0.28x |
| nested.json | json | 2.315 | 2.437 | 2.539 | 71.809 | 0.14x |
| wide_arrays.json | strata | 1.699 | 1.759 | 1.862 | 74.941 | 1.00x |
| wide_arrays.json | orjson | 2.001 | 2.028 | 2.193 | 74.941 | 0.87x |
| wide_arrays.json | msgspec | 2.845 | 2.906 | 3.107 | 74.941 | 0.61x |
| wide_arrays.json | ujson | 6.233 | 6.352 | 6.811 | 74.941 | 0.28x |
| wide_arrays.json | json | 15.660 | 16.018 | 16.998 | 74.941 | 0.11x |
| mixed.json | strata | 0.146 | 0.148 | 0.157 | 74.941 | 1.00x |
| mixed.json | orjson | 0.165 | 0.168 | 0.186 | 74.941 | 0.88x |
| mixed.json | msgspec | 0.168 | 0.184 | 0.204 | 74.941 | 0.80x |
| mixed.json | ujson | 0.342 | 0.345 | 0.365 | 74.941 | 0.43x |
| mixed.json | json | 0.619 | 0.634 | 0.645 | 74.941 | 0.23x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.057 | 0.058 | 0.061 | 73.441 | 1.00x |
| users.json $[*].id | jmespath | 0.457 | 0.470 | 0.517 | 73.441 | 0.12x |
| users.json $[*].id | jsonpath-ng | 2.570 | 2.583 | 2.628 | 73.441 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.411 | 0.412 | 0.425 | 73.441 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.871 | 2.885 | 3.011 | 73.441 | 0.14x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.460 | 17.723 | 19.309 | 73.441 | 0.02x |
| users.json $..total | strata | 1.535 | 1.548 | 1.677 | 75.285 | 1.00x |
| users.json $..total | jsonpath-ng | 353.018 | 363.871 | 373.998 | 75.285 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 2.854 | 2.878 | 3.021 | 73.441 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.846 | 12.967 | 14.038 | 73.441 | 0.22x |
| users.json $[*].id | orjson+jsonpath-ng | 14.942 | 15.110 | 15.450 | 73.441 | 0.19x |
| users.json $[*].orders[*].total | strata | 3.072 | 3.091 | 3.326 | 75.285 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.392 | 15.636 | 16.995 | 75.285 | 0.20x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 33.494 | 33.902 | 36.158 | 75.285 | 0.09x |
| users.json $..total | strata | 11.751 | 11.908 | 12.660 | 75.305 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 376.654 | 380.235 | 389.919 | 75.305 | 0.03x |

