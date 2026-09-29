# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 296d2ea02694ce7811592deeaa970aaa11c9432f
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: AMD EPYC 7763 64-Core Processor
- compiler_flags: -fno-strict-overflow -Wsign-compare -DNDEBUG -g -O3 -Wall -fPIC -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -fomit-frame-pointer -march=native -flto=thin -fprofile-use=/home/runner/work/_temp/strata-arm/build/pgo/strata.profdata -fsplit-machine-functions -shared -Wl,--rpath=/opt/hostedtoolcache/Python/3.12.14/x64/lib (24 recorded commands; see the JSON companion)
- repeats: 60
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.711 | 11.643 | 13.508 | 63.758 | 1.00x |
| users.json | orjson | 12.897 | 15.431 | 18.462 | 63.758 | 0.75x |
| users.json | msgspec | 13.244 | 15.160 | 17.551 | 63.758 | 0.77x |
| users.json | ujson | 17.454 | 21.898 | 24.874 | 63.758 | 0.53x |
| users.json | pysimdjson | 18.141 | 22.019 | 25.063 | 63.758 | 0.53x |
| users.json | json | 22.228 | 24.470 | 26.902 | 63.758 | 0.48x |
| flat.json | strata | 0.866 | 0.912 | 1.009 | 77.426 | 1.00x |
| flat.json | orjson | 1.006 | 1.051 | 1.168 | 77.426 | 0.87x |
| flat.json | msgspec | 1.018 | 1.075 | 1.142 | 77.426 | 0.85x |
| flat.json | ujson | 1.519 | 1.728 | 1.863 | 77.426 | 0.53x |
| flat.json | pysimdjson | 1.548 | 1.712 | 1.913 | 77.426 | 0.53x |
| flat.json | json | 1.854 | 1.890 | 2.011 | 77.426 | 0.48x |
| nested.json | strata | 0.794 | 0.824 | 0.957 | 77.426 | 1.00x |
| nested.json | orjson | 0.992 | 1.024 | 1.207 | 77.426 | 0.80x |
| nested.json | msgspec | 1.022 | 1.048 | 1.146 | 77.426 | 0.79x |
| nested.json | ujson | 1.435 | 1.590 | 1.825 | 77.426 | 0.52x |
| nested.json | pysimdjson | 1.385 | 1.468 | 1.737 | 77.426 | 0.56x |
| nested.json | json | 2.037 | 2.099 | 2.239 | 77.426 | 0.39x |
| wide_arrays.json | strata | 4.083 | 4.845 | 6.027 | 81.750 | 1.00x |
| wide_arrays.json | orjson | 5.111 | 6.277 | 7.439 | 81.750 | 0.77x |
| wide_arrays.json | msgspec | 5.674 | 6.383 | 7.723 | 81.750 | 0.76x |
| wide_arrays.json | ujson | 7.097 | 8.038 | 9.510 | 81.750 | 0.60x |
| wide_arrays.json | pysimdjson | 6.020 | 7.167 | 8.237 | 81.750 | 0.68x |
| wide_arrays.json | json | 9.711 | 11.024 | 12.326 | 81.750 | 0.44x |
| mixed.json | strata | 0.187 | 0.190 | 0.206 | 81.750 | 1.00x |
| mixed.json | orjson | 0.225 | 0.230 | 0.249 | 81.750 | 0.83x |
| mixed.json | msgspec | 0.235 | 0.240 | 0.256 | 81.750 | 0.79x |
| mixed.json | ujson | 0.295 | 0.306 | 0.329 | 81.750 | 0.62x |
| mixed.json | pysimdjson | 0.295 | 0.301 | 0.319 | 81.750 | 0.63x |
| mixed.json | json | 0.467 | 0.484 | 0.497 | 81.750 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.409 | 2.522 | 2.901 | 62.270 | 1.00x |
| users.json | orjson | 2.953 | 3.105 | 3.521 | 62.270 | 0.81x |
| users.json | msgspec | 3.919 | 4.085 | 4.735 | 62.270 | 0.62x |
| users.json | ujson | 11.168 | 11.713 | 12.474 | 62.270 | 0.22x |
| users.json | json | 22.502 | 23.253 | 23.868 | 62.270 | 0.11x |
| flat.json | strata | 0.277 | 0.293 | 0.332 | 77.426 | 1.00x |
| flat.json | orjson | 0.331 | 0.347 | 0.400 | 77.426 | 0.84x |
| flat.json | msgspec | 0.428 | 0.453 | 0.512 | 77.426 | 0.65x |
| flat.json | ujson | 0.995 | 1.009 | 1.062 | 77.426 | 0.29x |
| flat.json | json | 1.904 | 1.936 | 2.012 | 77.426 | 0.15x |
| nested.json | strata | 0.232 | 0.244 | 0.265 | 77.430 | 1.00x |
| nested.json | orjson | 0.294 | 0.308 | 0.332 | 77.430 | 0.79x |
| nested.json | msgspec | 0.412 | 0.425 | 0.445 | 77.430 | 0.57x |
| nested.json | ujson | 1.076 | 1.087 | 1.126 | 77.430 | 0.22x |
| nested.json | json | 2.451 | 2.478 | 2.516 | 77.430 | 0.10x |
| wide_arrays.json | strata | 1.691 | 1.803 | 2.176 | 81.750 | 1.00x |
| wide_arrays.json | orjson | 1.862 | 1.931 | 2.200 | 81.750 | 0.93x |
| wide_arrays.json | msgspec | 2.755 | 2.823 | 3.116 | 81.750 | 0.64x |
| wide_arrays.json | ujson | 6.434 | 6.614 | 7.177 | 81.750 | 0.27x |
| wide_arrays.json | json | 16.595 | 17.290 | 18.012 | 81.750 | 0.10x |
| mixed.json | strata | 0.063 | 0.072 | 0.087 | 81.750 | 1.00x |
| mixed.json | orjson | 0.065 | 0.072 | 0.084 | 81.750 | 1.00x |
| mixed.json | msgspec | 0.087 | 0.097 | 0.114 | 81.750 | 0.74x |
| mixed.json | ujson | 0.234 | 0.251 | 0.276 | 81.750 | 0.29x |
| mixed.json | json | 0.523 | 0.553 | 0.583 | 81.750 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.462 | 12.369 | 13.607 | 79.062 | 1.00x |
| users.json | orjson | 13.290 | 15.428 | 16.797 | 79.062 | 0.80x |
| users.json | msgspec | 13.754 | 15.477 | 17.599 | 79.062 | 0.80x |
| users.json | ujson | 19.783 | 22.024 | 24.567 | 79.062 | 0.56x |
| users.json | json | 22.872 | 24.664 | 26.495 | 79.062 | 0.50x |
| flat.json | strata | 0.861 | 0.942 | 1.075 | 77.426 | 1.00x |
| flat.json | orjson | 1.036 | 1.110 | 1.291 | 77.426 | 0.85x |
| flat.json | msgspec | 1.054 | 1.152 | 1.285 | 77.426 | 0.82x |
| flat.json | ujson | 1.510 | 1.798 | 2.095 | 77.426 | 0.52x |
| flat.json | json | 1.871 | 1.943 | 2.120 | 77.426 | 0.48x |
| nested.json | strata | 0.831 | 0.873 | 1.207 | 77.430 | 1.00x |
| nested.json | orjson | 1.045 | 1.113 | 1.347 | 77.430 | 0.78x |
| nested.json | msgspec | 1.071 | 1.122 | 1.348 | 77.430 | 0.78x |
| nested.json | ujson | 1.510 | 1.647 | 2.080 | 77.430 | 0.53x |
| nested.json | json | 2.084 | 2.158 | 2.489 | 77.430 | 0.40x |
| wide_arrays.json | strata | 4.255 | 4.965 | 5.525 | 81.750 | 1.00x |
| wide_arrays.json | orjson | 5.371 | 6.119 | 6.829 | 81.750 | 0.81x |
| wide_arrays.json | msgspec | 5.985 | 6.669 | 7.455 | 81.750 | 0.74x |
| wide_arrays.json | ujson | 7.419 | 8.396 | 8.918 | 81.750 | 0.59x |
| wide_arrays.json | json | 9.912 | 10.999 | 11.433 | 81.750 | 0.45x |
| mixed.json | strata | 0.210 | 0.231 | 0.259 | 81.750 | 1.00x |
| mixed.json | orjson | 0.278 | 0.313 | 0.393 | 81.750 | 0.74x |
| mixed.json | msgspec | 0.287 | 0.321 | 0.357 | 81.750 | 0.72x |
| mixed.json | ujson | 0.371 | 0.412 | 0.471 | 81.750 | 0.56x |
| mixed.json | json | 0.521 | 0.550 | 0.611 | 81.750 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.254 | 11.124 | 13.070 | 77.426 | 1.00x |
| users.ndjson | orjson | 16.765 | 17.609 | 19.383 | 77.426 | 0.63x |
| users.ndjson | msgspec | 16.926 | 17.674 | 19.315 | 77.426 | 0.63x |
| users.ndjson | ujson | 21.694 | 23.504 | 26.362 | 77.426 | 0.47x |
| users.ndjson | json | 29.258 | 30.556 | 33.248 | 77.426 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.087 | 3.275 | 3.854 | 79.062 | 1.00x |
| users.json | orjson | 3.705 | 4.001 | 4.858 | 79.062 | 0.82x |
| users.json | msgspec | 4.634 | 4.877 | 5.397 | 79.062 | 0.67x |
| users.json | ujson | 12.212 | 12.876 | 13.761 | 79.062 | 0.25x |
| users.json | json | 23.403 | 24.285 | 25.424 | 79.062 | 0.13x |
| flat.json | strata | 0.488 | 0.526 | 0.595 | 77.426 | 1.00x |
| flat.json | orjson | 0.554 | 0.605 | 0.724 | 77.426 | 0.87x |
| flat.json | msgspec | 0.658 | 0.712 | 0.995 | 77.426 | 0.74x |
| flat.json | ujson | 1.253 | 1.305 | 1.409 | 77.426 | 0.40x |
| flat.json | json | 2.190 | 2.253 | 2.433 | 77.426 | 0.23x |
| nested.json | strata | 0.403 | 0.462 | 0.549 | 77.430 | 1.00x |
| nested.json | orjson | 0.494 | 0.554 | 0.623 | 77.430 | 0.83x |
| nested.json | msgspec | 0.613 | 0.663 | 0.737 | 77.430 | 0.70x |
| nested.json | ujson | 1.283 | 1.343 | 1.430 | 77.430 | 0.34x |
| nested.json | json | 2.701 | 2.765 | 2.967 | 77.430 | 0.17x |
| wide_arrays.json | strata | 2.174 | 2.243 | 2.305 | 81.750 | 1.00x |
| wide_arrays.json | orjson | 2.371 | 2.429 | 2.512 | 81.750 | 0.92x |
| wide_arrays.json | msgspec | 3.253 | 3.298 | 3.368 | 81.750 | 0.68x |
| wide_arrays.json | ujson | 6.990 | 7.076 | 7.253 | 81.750 | 0.32x |
| wide_arrays.json | json | 17.115 | 17.279 | 17.738 | 81.750 | 0.13x |
| mixed.json | strata | 0.208 | 0.234 | 0.251 | 81.750 | 1.00x |
| mixed.json | orjson | 0.233 | 0.257 | 0.327 | 81.750 | 0.91x |
| mixed.json | msgspec | 0.257 | 0.280 | 0.319 | 81.750 | 0.83x |
| mixed.json | ujson | 0.422 | 0.457 | 0.529 | 81.750 | 0.51x |
| mixed.json | json | 0.723 | 0.758 | 0.824 | 81.750 | 0.31x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.061 | 0.064 | 0.070 | 79.062 | 1.00x |
| users.json $[*].id | jmespath | 0.487 | 0.499 | 0.517 | 79.062 | 0.13x |
| users.json $[*].id | jsonpath-ng | 2.815 | 2.902 | 3.070 | 79.062 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.435 | 0.470 | 0.521 | 79.062 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.070 | 3.142 | 3.302 | 79.062 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 20.167 | 21.550 | 24.484 | 79.062 | 0.02x |
| users.json $..total | strata | 1.680 | 1.806 | 2.336 | 79.062 | 1.00x |
| users.json $..total | jsonpath-ng | 390.279 | 393.616 | 396.173 | 79.062 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.247 | 3.303 | 3.357 | 79.062 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.778 | 15.446 | 17.612 | 79.062 | 0.21x |
| users.json $[*].id | orjson+jsonpath-ng | 17.043 | 17.827 | 21.339 | 79.062 | 0.19x |
| users.json $[*].orders[*].total | strata | 3.511 | 3.602 | 3.653 | 79.062 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.314 | 19.587 | 22.449 | 79.062 | 0.18x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 38.970 | 43.232 | 47.744 | 79.062 | 0.08x |
| users.json $..total | strata | 14.479 | 16.864 | 21.949 | 79.062 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 414.516 | 419.967 | 428.182 | 79.062 | 0.04x |

