# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c16eaa65c61f1cf9d4fac46e9f6d8e6b9f3b087f
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: AMD EPYC 9V74 80-Core Processor
- compiler_flags: -fno-strict-overflow -Wsign-compare -DNDEBUG -g -O3 -Wall -fPIC -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -fomit-frame-pointer -march=native -flto=thin -fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata -fsplit-machine-functions -shared -Wl,--rpath=/opt/hostedtoolcache/Python/3.12.14/x64/lib (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.000 | 10.247 | 15.350 | 64.355 | 1.00x |
| users.json | orjson | 13.983 | 14.300 | 18.157 | 64.355 | 0.72x |
| users.json | msgspec | 13.933 | 14.262 | 18.298 | 64.355 | 0.72x |
| users.json | ujson | 18.321 | 18.587 | 25.457 | 64.355 | 0.55x |
| users.json | pysimdjson | 19.088 | 19.593 | 23.497 | 64.355 | 0.52x |
| users.json | json | 21.266 | 21.557 | 23.063 | 64.355 | 0.48x |
| flat.json | strata | 0.861 | 0.897 | 0.904 | 78.012 | 1.00x |
| flat.json | orjson | 1.056 | 1.066 | 1.075 | 78.012 | 0.84x |
| flat.json | msgspec | 1.015 | 1.037 | 1.052 | 78.012 | 0.87x |
| flat.json | ujson | 1.486 | 1.510 | 1.546 | 78.012 | 0.59x |
| flat.json | pysimdjson | 1.617 | 1.637 | 1.666 | 78.012 | 0.55x |
| flat.json | json | 1.719 | 1.737 | 1.751 | 78.012 | 0.52x |
| nested.json | strata | 0.793 | 0.816 | 0.826 | 78.043 | 1.00x |
| nested.json | orjson | 1.012 | 1.022 | 1.132 | 78.043 | 0.80x |
| nested.json | msgspec | 0.972 | 0.994 | 1.070 | 78.043 | 0.82x |
| nested.json | ujson | 1.414 | 1.431 | 1.444 | 78.043 | 0.57x |
| nested.json | pysimdjson | 1.395 | 1.414 | 1.430 | 78.043 | 0.58x |
| nested.json | json | 1.829 | 1.854 | 1.876 | 78.043 | 0.44x |
| wide_arrays.json | strata | 4.391 | 4.501 | 4.549 | 82.742 | 1.00x |
| wide_arrays.json | orjson | 5.556 | 5.648 | 5.733 | 82.742 | 0.80x |
| wide_arrays.json | msgspec | 6.114 | 6.239 | 6.277 | 82.742 | 0.72x |
| wide_arrays.json | ujson | 7.593 | 7.703 | 7.787 | 82.742 | 0.58x |
| wide_arrays.json | pysimdjson | 6.430 | 6.490 | 6.651 | 82.742 | 0.69x |
| wide_arrays.json | json | 9.905 | 10.027 | 10.073 | 82.742 | 0.45x |
| mixed.json | strata | 0.190 | 0.198 | 0.213 | 82.742 | 1.00x |
| mixed.json | orjson | 0.236 | 0.240 | 0.259 | 82.742 | 0.82x |
| mixed.json | msgspec | 0.244 | 0.253 | 0.338 | 82.742 | 0.78x |
| mixed.json | ujson | 0.310 | 0.315 | 0.325 | 82.742 | 0.63x |
| mixed.json | pysimdjson | 0.305 | 0.316 | 0.322 | 82.742 | 0.63x |
| mixed.json | json | 0.457 | 0.463 | 0.475 | 82.742 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.337 | 2.350 | 2.363 | 62.746 | 1.00x |
| users.json | orjson | 3.078 | 3.097 | 3.162 | 62.746 | 0.76x |
| users.json | msgspec | 4.156 | 4.176 | 4.244 | 62.746 | 0.56x |
| users.json | ujson | 11.277 | 11.351 | 11.517 | 62.746 | 0.21x |
| users.json | json | 21.107 | 21.293 | 21.448 | 62.746 | 0.11x |
| flat.json | strata | 0.302 | 0.308 | 0.325 | 78.043 | 1.00x |
| flat.json | orjson | 0.352 | 0.362 | 0.433 | 78.043 | 0.85x |
| flat.json | msgspec | 0.461 | 0.470 | 0.505 | 78.043 | 0.66x |
| flat.json | ujson | 1.013 | 1.025 | 1.113 | 78.043 | 0.30x |
| flat.json | json | 1.830 | 1.847 | 1.872 | 78.043 | 0.17x |
| nested.json | strata | 0.226 | 0.232 | 0.242 | 78.043 | 1.00x |
| nested.json | orjson | 0.295 | 0.306 | 0.351 | 78.043 | 0.76x |
| nested.json | msgspec | 0.410 | 0.424 | 0.437 | 78.043 | 0.55x |
| nested.json | ujson | 1.063 | 1.074 | 1.159 | 78.043 | 0.22x |
| nested.json | json | 2.296 | 2.316 | 2.407 | 78.043 | 0.10x |
| wide_arrays.json | strata | 1.761 | 1.776 | 1.800 | 82.742 | 1.00x |
| wide_arrays.json | orjson | 1.929 | 1.940 | 1.952 | 82.742 | 0.92x |
| wide_arrays.json | msgspec | 3.044 | 3.082 | 3.138 | 82.742 | 0.58x |
| wide_arrays.json | ujson | 6.366 | 6.428 | 6.503 | 82.742 | 0.28x |
| wide_arrays.json | json | 16.657 | 16.717 | 16.848 | 82.742 | 0.11x |
| mixed.json | strata | 0.063 | 0.065 | 0.075 | 82.742 | 1.00x |
| mixed.json | orjson | 0.068 | 0.070 | 0.084 | 82.742 | 0.93x |
| mixed.json | msgspec | 0.085 | 0.087 | 0.096 | 82.742 | 0.75x |
| mixed.json | ujson | 0.227 | 0.231 | 0.252 | 82.742 | 0.28x |
| mixed.json | json | 0.503 | 0.513 | 0.527 | 82.742 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.712 | 10.891 | 12.593 | 79.645 | 1.00x |
| users.json | orjson | 14.494 | 14.842 | 16.098 | 79.645 | 0.73x |
| users.json | msgspec | 14.503 | 14.727 | 15.340 | 79.645 | 0.74x |
| users.json | ujson | 19.502 | 20.161 | 22.675 | 79.645 | 0.54x |
| users.json | json | 21.897 | 22.127 | 22.868 | 79.645 | 0.49x |
| flat.json | strata | 0.909 | 0.924 | 0.945 | 78.043 | 1.00x |
| flat.json | orjson | 1.112 | 1.132 | 1.164 | 78.043 | 0.82x |
| flat.json | msgspec | 1.093 | 1.098 | 1.118 | 78.043 | 0.84x |
| flat.json | ujson | 1.609 | 1.626 | 1.663 | 78.043 | 0.57x |
| flat.json | json | 1.772 | 1.788 | 1.854 | 78.043 | 0.52x |
| nested.json | strata | 0.822 | 0.842 | 0.879 | 78.043 | 1.00x |
| nested.json | orjson | 1.065 | 1.072 | 1.095 | 78.043 | 0.79x |
| nested.json | msgspec | 1.029 | 1.037 | 1.046 | 78.043 | 0.81x |
| nested.json | ujson | 1.471 | 1.491 | 1.582 | 78.043 | 0.56x |
| nested.json | json | 1.890 | 1.899 | 1.940 | 78.043 | 0.44x |
| wide_arrays.json | strata | 4.445 | 4.508 | 4.590 | 82.742 | 1.00x |
| wide_arrays.json | orjson | 5.613 | 5.706 | 5.756 | 82.742 | 0.79x |
| wide_arrays.json | msgspec | 6.252 | 6.313 | 6.363 | 82.742 | 0.71x |
| wide_arrays.json | ujson | 7.870 | 7.923 | 7.989 | 82.742 | 0.57x |
| wide_arrays.json | json | 9.904 | 10.009 | 10.099 | 82.742 | 0.45x |
| mixed.json | strata | 0.207 | 0.210 | 0.214 | 82.742 | 1.00x |
| mixed.json | orjson | 0.284 | 0.287 | 0.305 | 82.742 | 0.73x |
| mixed.json | msgspec | 0.287 | 0.293 | 0.308 | 82.742 | 0.72x |
| mixed.json | ujson | 0.359 | 0.368 | 0.380 | 82.742 | 0.57x |
| mixed.json | json | 0.502 | 0.509 | 0.527 | 82.742 | 0.41x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.615 | 10.918 | 11.127 | 78.012 | 1.00x |
| users.ndjson | orjson | 17.934 | 18.424 | 18.945 | 78.012 | 0.59x |
| users.ndjson | msgspec | 18.173 | 18.420 | 19.193 | 78.012 | 0.59x |
| users.ndjson | ujson | 23.070 | 23.617 | 26.932 | 78.012 | 0.46x |
| users.ndjson | json | 29.380 | 29.683 | 30.090 | 78.012 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.902 | 2.924 | 2.951 | 79.645 | 1.00x |
| users.json | orjson | 3.704 | 3.755 | 3.850 | 79.645 | 0.78x |
| users.json | msgspec | 4.747 | 4.783 | 4.885 | 79.645 | 0.61x |
| users.json | ujson | 12.027 | 12.087 | 12.226 | 79.645 | 0.24x |
| users.json | json | 21.970 | 22.137 | 22.840 | 79.645 | 0.13x |
| flat.json | strata | 0.445 | 0.469 | 0.481 | 78.043 | 1.00x |
| flat.json | orjson | 0.523 | 0.538 | 0.591 | 78.043 | 0.87x |
| flat.json | msgspec | 0.632 | 0.646 | 0.657 | 78.043 | 0.73x |
| flat.json | ujson | 1.213 | 1.224 | 1.284 | 78.043 | 0.38x |
| flat.json | json | 2.022 | 2.039 | 2.099 | 78.043 | 0.23x |
| nested.json | strata | 0.341 | 0.346 | 0.362 | 78.043 | 1.00x |
| nested.json | orjson | 0.442 | 0.453 | 0.475 | 78.043 | 0.76x |
| nested.json | msgspec | 0.557 | 0.566 | 0.577 | 78.043 | 0.61x |
| nested.json | ujson | 1.217 | 1.243 | 1.312 | 78.043 | 0.28x |
| nested.json | json | 2.488 | 2.506 | 2.590 | 78.043 | 0.14x |
| wide_arrays.json | strata | 2.146 | 2.180 | 2.201 | 82.742 | 1.00x |
| wide_arrays.json | orjson | 2.348 | 2.375 | 2.393 | 82.742 | 0.92x |
| wide_arrays.json | msgspec | 3.462 | 3.499 | 3.545 | 82.742 | 0.62x |
| wide_arrays.json | ujson | 6.865 | 6.913 | 6.950 | 82.742 | 0.32x |
| wide_arrays.json | json | 17.183 | 17.258 | 17.397 | 82.742 | 0.13x |
| mixed.json | strata | 0.147 | 0.155 | 0.159 | 82.742 | 1.00x |
| mixed.json | orjson | 0.173 | 0.180 | 0.187 | 82.742 | 0.86x |
| mixed.json | msgspec | 0.191 | 0.196 | 0.201 | 82.742 | 0.79x |
| mixed.json | ujson | 0.343 | 0.355 | 0.359 | 82.742 | 0.44x |
| mixed.json | json | 0.624 | 0.630 | 0.669 | 82.742 | 0.25x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.071 | 0.073 | 0.076 | 79.645 | 1.00x |
| users.json $[*].id | jmespath | 0.460 | 0.470 | 0.477 | 79.645 | 0.15x |
| users.json $[*].id | jsonpath-ng | 2.703 | 2.776 | 2.883 | 79.645 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.424 | 0.447 | 0.468 | 79.648 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.982 | 2.994 | 3.924 | 79.648 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 19.442 | 19.830 | 20.252 | 79.648 | 0.02x |
| users.json $..total | strata | 1.819 | 1.888 | 1.945 | 79.648 | 1.00x |
| users.json $..total | jsonpath-ng | 386.185 | 391.156 | 396.120 | 79.648 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.219 | 3.236 | 3.267 | 79.648 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.773 | 16.054 | 16.267 | 79.648 | 0.20x |
| users.json $[*].id | orjson+jsonpath-ng | 17.920 | 18.422 | 18.767 | 79.648 | 0.18x |
| users.json $[*].orders[*].total | strata | 3.467 | 3.476 | 3.491 | 79.648 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 19.004 | 19.297 | 19.526 | 79.648 | 0.18x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 39.249 | 39.916 | 43.073 | 79.648 | 0.09x |
| users.json $..total | strata | 14.040 | 16.026 | 16.918 | 79.648 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 410.429 | 414.581 | 416.114 | 79.648 | 0.04x |

