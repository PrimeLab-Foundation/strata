# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 296d2ea02694ce7811592deeaa970aaa11c9432f
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-aarch64-with-glibc2.39
- machine: aarch64
- processor: aarch64
- compiler_flags: -fno-strict-overflow -Wsign-compare -DNDEBUG -g -O3 -Wall -fPIC -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -march=native -flto=thin -fprofile-use=/home/runner/work/_temp/strata-arm/build/pgo/strata.profdata -shared -Wl,--rpath=/opt/hostedtoolcache/Python/3.12.14/arm64/lib (24 recorded commands; see the JSON companion)
- repeats: 60
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.907 | 9.185 | 9.573 | 57.402 | 1.00x |
| users.json | orjson | 11.542 | 12.026 | 12.556 | 57.402 | 0.76x |
| users.json | msgspec | 12.107 | 12.548 | 13.009 | 57.402 | 0.73x |
| users.json | ujson | 16.292 | 17.099 | 18.682 | 57.402 | 0.54x |
| users.json | pysimdjson | 16.248 | 17.199 | 18.412 | 57.402 | 0.53x |
| users.json | json | 20.484 | 21.255 | 21.833 | 57.402 | 0.43x |
| flat.json | strata | 0.819 | 0.837 | 0.846 | 68.309 | 1.00x |
| flat.json | orjson | 0.859 | 0.878 | 0.890 | 68.309 | 0.95x |
| flat.json | msgspec | 0.897 | 0.917 | 0.928 | 68.309 | 0.91x |
| flat.json | ujson | 1.411 | 1.425 | 1.447 | 68.309 | 0.59x |
| flat.json | pysimdjson | 1.469 | 1.483 | 1.502 | 68.309 | 0.56x |
| flat.json | json | 1.761 | 1.791 | 1.809 | 68.309 | 0.47x |
| nested.json | strata | 0.843 | 0.865 | 0.875 | 68.309 | 1.00x |
| nested.json | orjson | 0.891 | 0.915 | 0.935 | 68.309 | 0.95x |
| nested.json | msgspec | 1.016 | 1.028 | 1.052 | 68.309 | 0.84x |
| nested.json | ujson | 1.427 | 1.460 | 1.498 | 68.309 | 0.59x |
| nested.json | pysimdjson | 1.424 | 1.446 | 1.495 | 68.309 | 0.60x |
| nested.json | json | 1.976 | 2.005 | 2.066 | 68.309 | 0.43x |
| wide_arrays.json | strata | 3.962 | 4.070 | 4.223 | 70.676 | 1.00x |
| wide_arrays.json | orjson | 4.148 | 4.236 | 4.426 | 70.676 | 0.96x |
| wide_arrays.json | msgspec | 5.128 | 5.209 | 5.342 | 70.676 | 0.78x |
| wide_arrays.json | ujson | 6.563 | 6.644 | 6.832 | 70.676 | 0.61x |
| wide_arrays.json | pysimdjson | 5.364 | 5.487 | 5.685 | 70.676 | 0.74x |
| wide_arrays.json | json | 9.546 | 9.697 | 9.934 | 70.676 | 0.42x |
| mixed.json | strata | 0.195 | 0.204 | 0.227 | 70.676 | 1.00x |
| mixed.json | orjson | 0.215 | 0.224 | 0.243 | 70.676 | 0.91x |
| mixed.json | msgspec | 0.232 | 0.241 | 0.262 | 70.676 | 0.85x |
| mixed.json | ujson | 0.302 | 0.323 | 0.346 | 70.676 | 0.63x |
| mixed.json | pysimdjson | 0.292 | 0.304 | 0.328 | 70.676 | 0.67x |
| mixed.json | json | 0.451 | 0.474 | 0.494 | 70.676 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.983 | 1.998 | 2.018 | 56.512 | 1.00x |
| users.json | orjson | 2.624 | 2.643 | 2.660 | 56.512 | 0.76x |
| users.json | msgspec | 3.361 | 3.386 | 3.407 | 56.512 | 0.59x |
| users.json | ujson | 10.618 | 10.687 | 10.772 | 56.512 | 0.19x |
| users.json | json | 19.127 | 19.276 | 19.359 | 56.512 | 0.10x |
| flat.json | strata | 0.235 | 0.239 | 0.257 | 68.309 | 1.00x |
| flat.json | orjson | 0.301 | 0.308 | 0.325 | 68.309 | 0.78x |
| flat.json | msgspec | 0.391 | 0.403 | 0.418 | 68.309 | 0.59x |
| flat.json | ujson | 0.992 | 1.007 | 1.019 | 68.309 | 0.24x |
| flat.json | json | 1.682 | 1.702 | 1.733 | 68.309 | 0.14x |
| nested.json | strata | 0.220 | 0.230 | 0.251 | 68.309 | 1.00x |
| nested.json | orjson | 0.291 | 0.303 | 0.319 | 68.309 | 0.76x |
| nested.json | msgspec | 0.375 | 0.391 | 0.415 | 68.309 | 0.59x |
| nested.json | ujson | 1.102 | 1.121 | 1.147 | 68.309 | 0.21x |
| nested.json | json | 2.165 | 2.216 | 2.258 | 68.309 | 0.10x |
| wide_arrays.json | strata | 1.354 | 1.407 | 1.499 | 70.676 | 1.00x |
| wide_arrays.json | orjson | 1.620 | 1.655 | 1.708 | 70.676 | 0.85x |
| wide_arrays.json | msgspec | 2.377 | 2.409 | 2.450 | 70.676 | 0.58x |
| wide_arrays.json | ujson | 4.786 | 4.838 | 4.917 | 70.676 | 0.29x |
| wide_arrays.json | json | 13.616 | 13.744 | 13.882 | 70.676 | 0.10x |
| mixed.json | strata | 0.063 | 0.066 | 0.084 | 70.676 | 1.00x |
| mixed.json | orjson | 0.067 | 0.069 | 0.083 | 70.676 | 0.95x |
| mixed.json | msgspec | 0.080 | 0.083 | 0.087 | 70.676 | 0.79x |
| mixed.json | ujson | 0.239 | 0.244 | 0.262 | 70.676 | 0.27x |
| mixed.json | json | 0.473 | 0.491 | 0.514 | 70.676 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.428 | 9.918 | 10.050 | 69.531 | 1.00x |
| users.json | orjson | 12.259 | 13.010 | 13.436 | 69.531 | 0.76x |
| users.json | msgspec | 12.728 | 13.581 | 13.986 | 69.531 | 0.73x |
| users.json | ujson | 17.730 | 18.815 | 19.361 | 69.531 | 0.53x |
| users.json | json | 21.422 | 22.333 | 22.831 | 69.531 | 0.44x |
| flat.json | strata | 0.858 | 0.877 | 0.893 | 68.309 | 1.00x |
| flat.json | orjson | 0.939 | 0.957 | 0.967 | 68.309 | 0.92x |
| flat.json | msgspec | 0.974 | 0.991 | 1.000 | 68.309 | 0.88x |
| flat.json | ujson | 1.528 | 1.544 | 1.583 | 68.309 | 0.57x |
| flat.json | json | 1.834 | 1.848 | 1.868 | 68.309 | 0.47x |
| nested.json | strata | 0.871 | 0.900 | 0.920 | 68.309 | 1.00x |
| nested.json | orjson | 0.976 | 0.987 | 1.016 | 68.309 | 0.91x |
| nested.json | msgspec | 1.081 | 1.101 | 1.124 | 68.309 | 0.82x |
| nested.json | ujson | 1.508 | 1.537 | 1.582 | 68.309 | 0.59x |
| nested.json | json | 2.032 | 2.056 | 2.086 | 68.309 | 0.44x |
| wide_arrays.json | strata | 4.036 | 4.158 | 4.288 | 70.676 | 1.00x |
| wide_arrays.json | orjson | 4.214 | 4.446 | 4.645 | 70.676 | 0.94x |
| wide_arrays.json | msgspec | 5.190 | 5.387 | 5.555 | 70.676 | 0.77x |
| wide_arrays.json | ujson | 6.796 | 6.983 | 7.209 | 70.676 | 0.60x |
| wide_arrays.json | json | 9.718 | 9.973 | 10.133 | 70.676 | 0.42x |
| mixed.json | strata | 0.220 | 0.224 | 0.244 | 70.676 | 1.00x |
| mixed.json | orjson | 0.282 | 0.291 | 0.309 | 70.676 | 0.77x |
| mixed.json | msgspec | 0.298 | 0.309 | 0.328 | 70.676 | 0.72x |
| mixed.json | ujson | 0.383 | 0.393 | 0.423 | 70.676 | 0.57x |
| mixed.json | json | 0.513 | 0.532 | 0.548 | 70.676 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.544 | 9.899 | 10.866 | 68.297 | 1.00x |
| users.ndjson | orjson | 14.552 | 15.076 | 16.011 | 68.297 | 0.66x |
| users.ndjson | msgspec | 14.988 | 15.425 | 16.425 | 68.297 | 0.64x |
| users.ndjson | ujson | 19.454 | 20.314 | 21.582 | 68.297 | 0.49x |
| users.ndjson | json | 25.511 | 26.615 | 27.979 | 68.297 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.491 | 2.757 | 2.816 | 69.531 | 1.00x |
| users.json | orjson | 3.188 | 3.467 | 3.537 | 69.531 | 0.80x |
| users.json | msgspec | 3.910 | 4.217 | 4.279 | 69.531 | 0.65x |
| users.json | ujson | 11.171 | 11.644 | 11.773 | 69.531 | 0.24x |
| users.json | json | 19.653 | 20.300 | 20.388 | 69.531 | 0.14x |
| flat.json | strata | 0.439 | 0.510 | 0.578 | 68.309 | 1.00x |
| flat.json | orjson | 0.549 | 0.621 | 0.676 | 68.309 | 0.82x |
| flat.json | msgspec | 0.640 | 0.714 | 0.773 | 68.309 | 0.71x |
| flat.json | ujson | 1.251 | 1.346 | 1.411 | 68.309 | 0.38x |
| flat.json | json | 1.956 | 2.043 | 2.101 | 68.309 | 0.25x |
| nested.json | strata | 0.422 | 0.477 | 0.535 | 68.309 | 1.00x |
| nested.json | orjson | 0.553 | 0.603 | 0.655 | 68.309 | 0.79x |
| nested.json | msgspec | 0.637 | 0.693 | 0.743 | 68.309 | 0.69x |
| nested.json | ujson | 1.368 | 1.429 | 1.477 | 68.309 | 0.33x |
| nested.json | json | 2.445 | 2.515 | 2.576 | 68.309 | 0.19x |
| wide_arrays.json | strata | 1.808 | 1.963 | 2.124 | 70.676 | 1.00x |
| wide_arrays.json | orjson | 2.114 | 2.253 | 2.359 | 70.676 | 0.87x |
| wide_arrays.json | msgspec | 2.887 | 2.998 | 3.113 | 70.676 | 0.65x |
| wide_arrays.json | ujson | 5.359 | 5.465 | 5.615 | 70.676 | 0.36x |
| wide_arrays.json | json | 14.144 | 14.398 | 14.563 | 70.676 | 0.14x |
| mixed.json | strata | 0.244 | 0.269 | 0.294 | 70.676 | 1.00x |
| mixed.json | orjson | 0.276 | 0.311 | 0.343 | 70.676 | 0.87x |
| mixed.json | msgspec | 0.291 | 0.325 | 0.360 | 70.676 | 0.83x |
| mixed.json | ujson | 0.475 | 0.510 | 0.552 | 70.676 | 0.53x |
| mixed.json | json | 0.707 | 0.751 | 0.778 | 70.676 | 0.36x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.106 | 0.110 | 0.118 | 69.531 | 1.00x |
| users.json $[*].id | jmespath | 0.470 | 0.485 | 0.497 | 69.531 | 0.23x |
| users.json $[*].id | jsonpath-ng | 2.504 | 2.573 | 2.633 | 69.531 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.643 | 0.660 | 0.686 | 69.676 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.026 | 3.066 | 3.122 | 69.676 | 0.22x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.764 | 18.668 | 19.413 | 69.676 | 0.04x |
| users.json $..total | strata | 1.748 | 1.771 | 1.790 | 69.816 | 1.00x |
| users.json $..total | jsonpath-ng | 291.208 | 293.805 | 294.779 | 69.816 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.154 | 3.221 | 3.251 | 69.676 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.386 | 13.131 | 13.559 | 69.676 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 14.390 | 14.982 | 15.617 | 69.676 | 0.22x |
| users.json $[*].orders[*].total | strata | 3.344 | 3.397 | 3.425 | 69.816 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.473 | 16.216 | 16.851 | 69.816 | 0.21x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 34.366 | 35.518 | 37.615 | 69.816 | 0.10x |
| users.json $..total | strata | 11.259 | 11.758 | 12.539 | 69.934 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 311.530 | 314.596 | 318.409 | 69.934 | 0.04x |

