# Benchmark results - ci-linux-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 943460734d2b79bd16c949d8d97f4d22cc202e84
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
| users.json | strata | 8.911 | 8.971 | 10.680 | 57.258 | 1.00x |
| users.json | orjson | 11.349 | 11.466 | 12.893 | 57.258 | 0.78x |
| users.json | msgspec | 11.899 | 12.027 | 13.366 | 57.258 | 0.75x |
| users.json | ujson | 15.935 | 16.042 | 18.254 | 57.258 | 0.56x |
| users.json | pysimdjson | 15.974 | 16.121 | 17.710 | 57.258 | 0.56x |
| users.json | json | 20.162 | 20.251 | 20.906 | 57.258 | 0.44x |
| flat.json | strata | 0.818 | 0.829 | 0.835 | 58.391 | 1.00x |
| flat.json | orjson | 0.858 | 0.866 | 0.877 | 58.391 | 0.96x |
| flat.json | msgspec | 0.884 | 0.899 | 0.915 | 58.391 | 0.92x |
| flat.json | ujson | 1.390 | 1.399 | 1.410 | 58.391 | 0.59x |
| flat.json | pysimdjson | 1.445 | 1.457 | 1.468 | 58.391 | 0.57x |
| flat.json | json | 1.744 | 1.757 | 1.771 | 58.391 | 0.47x |
| nested.json | strata | 0.809 | 0.828 | 0.853 | 58.391 | 1.00x |
| nested.json | orjson | 0.857 | 0.878 | 0.886 | 58.391 | 0.94x |
| nested.json | msgspec | 0.959 | 0.986 | 1.009 | 58.391 | 0.84x |
| nested.json | ujson | 1.369 | 1.386 | 1.393 | 58.391 | 0.60x |
| nested.json | pysimdjson | 1.373 | 1.387 | 1.407 | 58.391 | 0.60x |
| nested.json | json | 1.938 | 1.952 | 1.973 | 58.391 | 0.42x |
| wide_arrays.json | strata | 3.873 | 3.883 | 3.927 | 64.949 | 1.00x |
| wide_arrays.json | orjson | 3.970 | 3.999 | 4.043 | 64.949 | 0.97x |
| wide_arrays.json | msgspec | 4.918 | 4.969 | 4.996 | 64.949 | 0.78x |
| wide_arrays.json | ujson | 6.353 | 6.400 | 6.416 | 64.949 | 0.61x |
| wide_arrays.json | pysimdjson | 5.146 | 5.187 | 5.214 | 64.949 | 0.75x |
| wide_arrays.json | json | 9.333 | 9.352 | 9.381 | 64.949 | 0.42x |
| mixed.json | strata | 0.193 | 0.194 | 0.215 | 64.949 | 1.00x |
| mixed.json | orjson | 0.205 | 0.209 | 0.211 | 64.949 | 0.93x |
| mixed.json | msgspec | 0.230 | 0.234 | 0.237 | 64.949 | 0.83x |
| mixed.json | ujson | 0.299 | 0.300 | 0.321 | 64.949 | 0.65x |
| mixed.json | pysimdjson | 0.287 | 0.291 | 0.339 | 64.949 | 0.67x |
| mixed.json | json | 0.446 | 0.461 | 0.475 | 64.949 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.909 | 1.916 | 1.926 | 45.902 | 1.00x |
| users.json | orjson | 2.561 | 2.578 | 2.602 | 45.902 | 0.74x |
| users.json | msgspec | 3.283 | 3.293 | 3.325 | 45.902 | 0.58x |
| users.json | ujson | 10.430 | 10.455 | 10.485 | 45.902 | 0.18x |
| users.json | json | 18.772 | 18.815 | 18.853 | 45.902 | 0.10x |
| flat.json | strata | 0.228 | 0.229 | 0.241 | 58.391 | 1.00x |
| flat.json | orjson | 0.293 | 0.295 | 0.314 | 58.391 | 0.78x |
| flat.json | msgspec | 0.377 | 0.387 | 0.438 | 58.391 | 0.59x |
| flat.json | ujson | 0.979 | 0.984 | 0.989 | 58.391 | 0.23x |
| flat.json | json | 1.673 | 1.690 | 1.710 | 58.391 | 0.14x |
| nested.json | strata | 0.212 | 0.213 | 0.231 | 58.395 | 1.00x |
| nested.json | orjson | 0.277 | 0.280 | 0.297 | 58.395 | 0.76x |
| nested.json | msgspec | 0.363 | 0.364 | 0.383 | 58.395 | 0.59x |
| nested.json | ujson | 1.089 | 1.100 | 1.111 | 58.395 | 0.19x |
| nested.json | json | 2.123 | 2.134 | 2.165 | 58.395 | 0.10x |
| wide_arrays.json | strata | 1.284 | 1.295 | 1.308 | 64.949 | 1.00x |
| wide_arrays.json | orjson | 1.593 | 1.611 | 1.635 | 64.949 | 0.80x |
| wide_arrays.json | msgspec | 2.349 | 2.360 | 2.399 | 64.949 | 0.55x |
| wide_arrays.json | ujson | 4.715 | 4.733 | 4.776 | 64.949 | 0.27x |
| wide_arrays.json | json | 13.497 | 13.508 | 13.571 | 64.949 | 0.10x |
| mixed.json | strata | 0.058 | 0.060 | 0.072 | 64.949 | 1.00x |
| mixed.json | orjson | 0.062 | 0.064 | 0.065 | 64.949 | 0.93x |
| mixed.json | msgspec | 0.076 | 0.077 | 0.090 | 64.949 | 0.78x |
| mixed.json | ujson | 0.232 | 0.235 | 0.250 | 64.949 | 0.25x |
| mixed.json | json | 0.464 | 0.477 | 0.485 | 64.949 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.029 | 9.054 | 9.676 | 59.781 | 1.00x |
| users.json | orjson | 11.484 | 11.520 | 11.903 | 59.781 | 0.79x |
| users.json | msgspec | 12.053 | 12.106 | 12.424 | 59.781 | 0.75x |
| users.json | ujson | 16.335 | 16.409 | 17.530 | 59.781 | 0.55x |
| users.json | json | 20.348 | 20.406 | 20.467 | 59.781 | 0.44x |
| flat.json | strata | 0.850 | 0.864 | 0.871 | 58.391 | 1.00x |
| flat.json | orjson | 0.935 | 0.940 | 0.944 | 58.391 | 0.92x |
| flat.json | msgspec | 0.963 | 0.968 | 0.974 | 58.391 | 0.89x |
| flat.json | ujson | 1.495 | 1.507 | 1.526 | 58.391 | 0.57x |
| flat.json | json | 1.804 | 1.819 | 1.826 | 58.391 | 0.47x |
| nested.json | strata | 0.838 | 0.854 | 0.860 | 58.395 | 1.00x |
| nested.json | orjson | 0.923 | 0.925 | 0.932 | 58.395 | 0.92x |
| nested.json | msgspec | 1.029 | 1.036 | 1.049 | 58.395 | 0.82x |
| nested.json | ujson | 1.446 | 1.464 | 1.490 | 58.395 | 0.58x |
| nested.json | json | 1.981 | 1.997 | 2.015 | 58.395 | 0.43x |
| wide_arrays.json | strata | 3.872 | 3.889 | 3.908 | 64.949 | 1.00x |
| wide_arrays.json | orjson | 3.949 | 3.987 | 4.015 | 64.949 | 0.98x |
| wide_arrays.json | msgspec | 4.958 | 4.991 | 5.030 | 64.949 | 0.78x |
| wide_arrays.json | ujson | 6.532 | 6.573 | 6.601 | 64.949 | 0.59x |
| wide_arrays.json | json | 9.401 | 9.456 | 9.561 | 64.949 | 0.41x |
| mixed.json | strata | 0.210 | 0.211 | 0.229 | 64.949 | 1.00x |
| mixed.json | orjson | 0.262 | 0.277 | 0.284 | 64.949 | 0.76x |
| mixed.json | msgspec | 0.287 | 0.293 | 0.310 | 64.949 | 0.72x |
| mixed.json | ujson | 0.367 | 0.377 | 0.405 | 64.949 | 0.56x |
| mixed.json | json | 0.496 | 0.507 | 0.520 | 64.949 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.374 | 9.396 | 9.428 | 58.387 | 1.00x |
| users.ndjson | orjson | 14.284 | 14.337 | 14.372 | 58.387 | 0.66x |
| users.ndjson | msgspec | 14.751 | 14.763 | 14.832 | 58.387 | 0.64x |
| users.ndjson | ujson | 19.058 | 19.105 | 19.177 | 58.387 | 0.49x |
| users.ndjson | json | 25.179 | 25.219 | 25.339 | 58.387 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.385 | 2.470 | 2.511 | 59.781 | 1.00x |
| users.json | orjson | 3.075 | 3.173 | 3.220 | 59.781 | 0.78x |
| users.json | msgspec | 3.792 | 3.862 | 3.934 | 59.781 | 0.64x |
| users.json | ujson | 11.061 | 11.136 | 11.269 | 59.781 | 0.22x |
| users.json | json | 19.525 | 19.583 | 19.629 | 59.781 | 0.13x |
| flat.json | strata | 0.425 | 0.472 | 0.483 | 58.391 | 1.00x |
| flat.json | orjson | 0.519 | 0.551 | 0.583 | 58.391 | 0.86x |
| flat.json | msgspec | 0.615 | 0.643 | 0.690 | 58.391 | 0.73x |
| flat.json | ujson | 1.221 | 1.253 | 1.294 | 58.391 | 0.38x |
| flat.json | json | 1.927 | 1.961 | 2.079 | 58.391 | 0.24x |
| nested.json | strata | 0.380 | 0.423 | 0.468 | 58.395 | 1.00x |
| nested.json | orjson | 0.487 | 0.530 | 0.570 | 58.395 | 0.80x |
| nested.json | msgspec | 0.602 | 0.629 | 0.667 | 58.395 | 0.67x |
| nested.json | ujson | 1.326 | 1.354 | 1.392 | 58.395 | 0.31x |
| nested.json | json | 2.383 | 2.398 | 2.450 | 58.395 | 0.18x |
| wide_arrays.json | strata | 1.670 | 1.713 | 1.735 | 64.949 | 1.00x |
| wide_arrays.json | orjson | 1.997 | 2.063 | 2.069 | 64.949 | 0.83x |
| wide_arrays.json | msgspec | 2.779 | 2.818 | 2.886 | 64.949 | 0.61x |
| wide_arrays.json | ujson | 5.196 | 5.244 | 5.281 | 64.949 | 0.33x |
| wide_arrays.json | json | 13.940 | 13.978 | 14.044 | 64.949 | 0.12x |
| mixed.json | strata | 0.202 | 0.213 | 0.215 | 64.949 | 1.00x |
| mixed.json | orjson | 0.232 | 0.246 | 0.273 | 64.949 | 0.86x |
| mixed.json | msgspec | 0.252 | 0.262 | 0.295 | 64.949 | 0.81x |
| mixed.json | ujson | 0.431 | 0.442 | 0.479 | 64.949 | 0.48x |
| mixed.json | json | 0.653 | 0.676 | 0.727 | 64.949 | 0.31x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.095 | 0.096 | 0.097 | 59.781 | 1.00x |
| users.json $[*].id | jmespath | 0.459 | 0.466 | 0.477 | 59.781 | 0.21x |
| users.json $[*].id | jsonpath-ng | 2.389 | 2.437 | 2.459 | 59.781 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.586 | 0.603 | 0.613 | 59.895 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.922 | 2.938 | 2.976 | 59.895 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 16.928 | 17.099 | 17.167 | 59.895 | 0.04x |
| users.json $..total | strata | 1.678 | 1.687 | 1.699 | 60.004 | 1.00x |
| users.json $..total | jsonpath-ng | 289.362 | 290.263 | 290.923 | 60.004 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.105 | 3.118 | 3.687 | 59.895 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.082 | 12.156 | 12.243 | 59.895 | 0.26x |
| users.json $[*].id | orjson+jsonpath-ng | 14.026 | 14.084 | 14.293 | 59.895 | 0.22x |
| users.json $[*].orders[*].total | strata | 3.262 | 3.272 | 3.855 | 60.004 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 14.640 | 14.735 | 14.817 | 60.004 | 0.22x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 32.494 | 32.644 | 33.073 | 60.004 | 0.10x |
| users.json $..total | strata | 11.048 | 11.218 | 11.329 | 60.023 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 306.401 | 307.467 | 308.394 | 60.023 | 0.04x |

