# Benchmark results - ci-linux-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 38cf28416465e176915ffd24d827a95015eacdbd
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-aarch64-with-glibc2.39
- machine: aarch64
- processor: aarch64
- compiler_flags: -fno-strict-overflow -Wsign-compare -DNDEBUG -g -O3 -Wall -fPIC -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -march=native -flto=thin -fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata -shared -Wl,--rpath=/opt/hostedtoolcache/Python/3.12.14/arm64/lib (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.730 | 8.816 | 10.674 | 57.207 | 1.00x |
| users.json | orjson | 11.570 | 11.696 | 13.172 | 57.207 | 0.75x |
| users.json | msgspec | 12.088 | 12.147 | 13.566 | 57.207 | 0.73x |
| users.json | ujson | 16.409 | 16.489 | 18.750 | 57.207 | 0.53x |
| users.json | pysimdjson | 16.319 | 16.539 | 18.193 | 57.207 | 0.53x |
| users.json | json | 20.556 | 20.705 | 21.306 | 57.207 | 0.43x |
| flat.json | strata | 0.827 | 0.856 | 0.876 | 67.992 | 1.00x |
| flat.json | orjson | 0.886 | 0.897 | 0.901 | 67.992 | 0.95x |
| flat.json | msgspec | 0.920 | 0.928 | 0.938 | 67.992 | 0.92x |
| flat.json | ujson | 1.448 | 1.478 | 1.505 | 67.992 | 0.58x |
| flat.json | pysimdjson | 1.497 | 1.507 | 1.576 | 67.992 | 0.57x |
| flat.json | json | 1.785 | 1.793 | 1.803 | 67.992 | 0.48x |
| nested.json | strata | 0.795 | 0.813 | 0.828 | 67.992 | 1.00x |
| nested.json | orjson | 0.857 | 0.877 | 0.889 | 67.992 | 0.93x |
| nested.json | msgspec | 0.982 | 0.986 | 0.998 | 67.992 | 0.82x |
| nested.json | ujson | 1.375 | 1.388 | 1.426 | 67.992 | 0.59x |
| nested.json | pysimdjson | 1.379 | 1.387 | 1.411 | 67.992 | 0.59x |
| nested.json | json | 1.953 | 1.974 | 1.989 | 67.992 | 0.41x |
| wide_arrays.json | strata | 3.839 | 3.855 | 3.896 | 69.562 | 1.00x |
| wide_arrays.json | orjson | 4.034 | 4.068 | 4.100 | 69.562 | 0.95x |
| wide_arrays.json | msgspec | 4.992 | 5.029 | 5.057 | 69.562 | 0.77x |
| wide_arrays.json | ujson | 6.413 | 6.472 | 6.546 | 69.562 | 0.60x |
| wide_arrays.json | pysimdjson | 5.238 | 5.277 | 5.295 | 69.562 | 0.73x |
| wide_arrays.json | json | 9.428 | 9.479 | 9.534 | 69.562 | 0.41x |
| mixed.json | strata | 0.187 | 0.192 | 0.216 | 69.562 | 1.00x |
| mixed.json | orjson | 0.214 | 0.217 | 0.237 | 69.562 | 0.88x |
| mixed.json | msgspec | 0.228 | 0.233 | 0.254 | 69.562 | 0.82x |
| mixed.json | ujson | 0.305 | 0.309 | 0.338 | 69.562 | 0.62x |
| mixed.json | pysimdjson | 0.292 | 0.296 | 0.315 | 69.562 | 0.65x |
| mixed.json | json | 0.456 | 0.470 | 0.496 | 69.562 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.924 | 1.935 | 1.941 | 56.312 | 1.00x |
| users.json | orjson | 2.566 | 2.579 | 2.624 | 56.312 | 0.75x |
| users.json | msgspec | 3.316 | 3.327 | 3.337 | 56.312 | 0.58x |
| users.json | ujson | 10.483 | 10.535 | 10.585 | 56.312 | 0.18x |
| users.json | json | 18.928 | 18.993 | 19.077 | 56.312 | 0.10x |
| flat.json | strata | 0.234 | 0.236 | 0.258 | 67.992 | 1.00x |
| flat.json | orjson | 0.299 | 0.302 | 0.322 | 67.992 | 0.78x |
| flat.json | msgspec | 0.387 | 0.398 | 0.411 | 67.992 | 0.59x |
| flat.json | ujson | 0.981 | 0.995 | 1.014 | 67.992 | 0.24x |
| flat.json | json | 1.701 | 1.708 | 1.727 | 67.992 | 0.14x |
| nested.json | strata | 0.216 | 0.221 | 0.224 | 67.992 | 1.00x |
| nested.json | orjson | 0.278 | 0.281 | 0.288 | 67.992 | 0.79x |
| nested.json | msgspec | 0.365 | 0.385 | 0.394 | 67.992 | 0.57x |
| nested.json | ujson | 1.072 | 1.084 | 1.098 | 67.992 | 0.20x |
| nested.json | json | 2.148 | 2.164 | 2.201 | 67.992 | 0.10x |
| wide_arrays.json | strata | 1.338 | 1.348 | 1.374 | 69.562 | 1.00x |
| wide_arrays.json | orjson | 1.582 | 1.606 | 1.623 | 69.562 | 0.84x |
| wide_arrays.json | msgspec | 2.355 | 2.372 | 2.385 | 69.562 | 0.57x |
| wide_arrays.json | ujson | 4.758 | 4.778 | 4.805 | 69.562 | 0.28x |
| wide_arrays.json | json | 13.569 | 13.580 | 13.660 | 69.562 | 0.10x |
| mixed.json | strata | 0.061 | 0.062 | 0.063 | 69.562 | 1.00x |
| mixed.json | orjson | 0.062 | 0.065 | 0.079 | 69.562 | 0.96x |
| mixed.json | msgspec | 0.076 | 0.077 | 0.093 | 69.562 | 0.80x |
| mixed.json | ujson | 0.238 | 0.242 | 0.274 | 69.562 | 0.26x |
| mixed.json | json | 0.482 | 0.494 | 0.528 | 69.562 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.964 | 9.006 | 9.932 | 68.426 | 1.00x |
| users.json | orjson | 11.823 | 11.959 | 12.408 | 68.426 | 0.75x |
| users.json | msgspec | 12.313 | 12.491 | 12.921 | 68.426 | 0.72x |
| users.json | ujson | 17.017 | 17.483 | 18.534 | 68.426 | 0.52x |
| users.json | json | 20.965 | 21.029 | 21.486 | 68.426 | 0.43x |
| flat.json | strata | 0.866 | 0.892 | 0.924 | 67.992 | 1.00x |
| flat.json | orjson | 0.961 | 0.973 | 1.000 | 67.992 | 0.92x |
| flat.json | msgspec | 0.989 | 1.005 | 1.016 | 67.992 | 0.89x |
| flat.json | ujson | 1.545 | 1.581 | 1.598 | 67.992 | 0.56x |
| flat.json | json | 1.848 | 1.865 | 1.889 | 67.992 | 0.48x |
| nested.json | strata | 0.830 | 0.846 | 0.858 | 67.992 | 1.00x |
| nested.json | orjson | 0.939 | 0.948 | 0.974 | 67.992 | 0.89x |
| nested.json | msgspec | 1.045 | 1.061 | 1.072 | 67.992 | 0.80x |
| nested.json | ujson | 1.468 | 1.495 | 1.533 | 67.992 | 0.57x |
| nested.json | json | 2.032 | 2.038 | 2.069 | 67.992 | 0.42x |
| wide_arrays.json | strata | 3.838 | 3.875 | 3.926 | 69.562 | 1.00x |
| wide_arrays.json | orjson | 4.025 | 4.096 | 4.161 | 69.562 | 0.95x |
| wide_arrays.json | msgspec | 5.073 | 5.122 | 5.194 | 69.562 | 0.76x |
| wide_arrays.json | ujson | 6.612 | 6.671 | 6.815 | 69.562 | 0.58x |
| wide_arrays.json | json | 9.515 | 9.612 | 9.706 | 69.562 | 0.40x |
| mixed.json | strata | 0.209 | 0.219 | 0.240 | 69.562 | 1.00x |
| mixed.json | orjson | 0.273 | 0.282 | 0.304 | 69.562 | 0.78x |
| mixed.json | msgspec | 0.290 | 0.300 | 0.324 | 69.562 | 0.73x |
| mixed.json | ujson | 0.380 | 0.388 | 0.408 | 69.562 | 0.56x |
| mixed.json | json | 0.502 | 0.528 | 0.561 | 69.562 | 0.41x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.301 | 9.459 | 9.645 | 67.984 | 1.00x |
| users.ndjson | orjson | 14.725 | 14.878 | 14.985 | 67.984 | 0.64x |
| users.ndjson | msgspec | 15.084 | 15.168 | 15.350 | 67.984 | 0.62x |
| users.ndjson | ujson | 19.610 | 19.824 | 20.075 | 67.984 | 0.48x |
| users.ndjson | json | 26.057 | 26.292 | 26.872 | 67.984 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.384 | 2.463 | 2.482 | 68.426 | 1.00x |
| users.json | orjson | 3.104 | 3.175 | 3.218 | 68.426 | 0.78x |
| users.json | msgspec | 3.820 | 3.917 | 3.957 | 68.426 | 0.63x |
| users.json | ujson | 11.099 | 11.183 | 11.296 | 68.426 | 0.22x |
| users.json | json | 19.588 | 19.740 | 19.804 | 68.426 | 0.12x |
| flat.json | strata | 0.387 | 0.401 | 0.408 | 67.992 | 1.00x |
| flat.json | orjson | 0.470 | 0.485 | 0.493 | 67.992 | 0.83x |
| flat.json | msgspec | 0.581 | 0.605 | 0.617 | 67.992 | 0.66x |
| flat.json | ujson | 1.204 | 1.217 | 1.242 | 67.992 | 0.33x |
| flat.json | json | 1.918 | 1.926 | 1.995 | 67.992 | 0.21x |
| nested.json | strata | 0.361 | 0.390 | 0.401 | 67.992 | 1.00x |
| nested.json | orjson | 0.458 | 0.473 | 0.505 | 67.992 | 0.82x |
| nested.json | msgspec | 0.546 | 0.587 | 0.601 | 67.992 | 0.66x |
| nested.json | ujson | 1.292 | 1.313 | 1.335 | 67.992 | 0.30x |
| nested.json | json | 2.369 | 2.395 | 2.420 | 67.992 | 0.16x |
| wide_arrays.json | strata | 1.684 | 1.759 | 1.805 | 69.562 | 1.00x |
| wide_arrays.json | orjson | 2.012 | 2.079 | 2.104 | 69.562 | 0.85x |
| wide_arrays.json | msgspec | 2.746 | 2.810 | 2.870 | 69.562 | 0.63x |
| wide_arrays.json | ujson | 5.193 | 5.261 | 5.395 | 69.562 | 0.33x |
| wide_arrays.json | json | 13.989 | 14.092 | 14.298 | 69.562 | 0.12x |
| mixed.json | strata | 0.170 | 0.182 | 0.204 | 69.562 | 1.00x |
| mixed.json | orjson | 0.189 | 0.202 | 0.217 | 69.562 | 0.90x |
| mixed.json | msgspec | 0.211 | 0.215 | 0.245 | 69.562 | 0.85x |
| mixed.json | ujson | 0.387 | 0.394 | 0.412 | 69.562 | 0.46x |
| mixed.json | json | 0.624 | 0.643 | 0.673 | 69.562 | 0.28x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.103 | 0.107 | 0.127 | 68.426 | 1.00x |
| users.json $[*].id | jmespath | 0.476 | 0.483 | 0.495 | 68.426 | 0.22x |
| users.json $[*].id | jsonpath-ng | 2.454 | 2.536 | 2.570 | 68.426 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.615 | 0.624 | 0.654 | 68.551 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.986 | 2.994 | 3.015 | 68.551 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.542 | 17.926 | 18.105 | 68.551 | 0.03x |
| users.json $..total | strata | 1.704 | 1.710 | 1.725 | 69.559 | 1.00x |
| users.json $..total | jsonpath-ng | 296.187 | 296.586 | 296.917 | 69.559 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.183 | 3.201 | 3.214 | 68.551 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.629 | 12.873 | 13.014 | 68.551 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 14.418 | 14.627 | 14.801 | 68.551 | 0.22x |
| users.json $[*].orders[*].total | strata | 3.368 | 3.389 | 3.434 | 69.559 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.334 | 15.638 | 15.824 | 69.559 | 0.22x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 33.326 | 33.677 | 34.122 | 69.559 | 0.10x |
| users.json $..total | strata | 11.506 | 11.773 | 11.983 | 69.621 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 312.349 | 312.758 | 313.998 | 69.621 | 0.04x |

