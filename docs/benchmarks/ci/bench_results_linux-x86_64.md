# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: cd9d20b6ab3ec573716c10b12fe76ae4b70a707c
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: AMD EPYC 7763 64-Core Processor
- compiler_flags: -fno-strict-overflow -Wsign-compare -DNDEBUG -g -O3 -Wall -fPIC -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -fomit-frame-pointer -march=native -flto=thin -fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata -fsplit-machine-functions -shared -Wl,--rpath=/opt/hostedtoolcache/Python/3.12.14/x64/lib (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.519 | 10.287 | 13.968 | 66.930 | 1.00x |
| users.json | orjson | 13.074 | 13.827 | 16.398 | 66.930 | 0.74x |
| users.json | msgspec | 13.101 | 14.031 | 16.133 | 66.930 | 0.73x |
| users.json | ujson | 17.217 | 19.141 | 22.454 | 66.930 | 0.54x |
| users.json | pysimdjson | 18.328 | 19.220 | 23.091 | 66.930 | 0.54x |
| users.json | json | 22.089 | 22.886 | 24.797 | 66.930 | 0.45x |
| flat.json | strata | 0.852 | 0.861 | 0.892 | 66.625 | 1.00x |
| flat.json | orjson | 0.987 | 0.994 | 1.008 | 66.625 | 0.87x |
| flat.json | msgspec | 1.009 | 1.027 | 1.038 | 66.625 | 0.84x |
| flat.json | ujson | 1.481 | 1.523 | 1.603 | 66.625 | 0.57x |
| flat.json | pysimdjson | 1.550 | 1.570 | 1.656 | 66.625 | 0.55x |
| flat.json | json | 1.861 | 1.891 | 1.971 | 66.625 | 0.46x |
| nested.json | strata | 0.792 | 0.830 | 0.849 | 66.625 | 1.00x |
| nested.json | orjson | 1.003 | 1.027 | 1.040 | 66.625 | 0.81x |
| nested.json | msgspec | 1.022 | 1.064 | 1.099 | 66.625 | 0.78x |
| nested.json | ujson | 1.449 | 1.505 | 1.565 | 66.625 | 0.55x |
| nested.json | pysimdjson | 1.395 | 1.444 | 1.477 | 66.625 | 0.58x |
| nested.json | json | 2.031 | 2.106 | 2.127 | 66.625 | 0.39x |
| wide_arrays.json | strata | 4.234 | 4.300 | 4.719 | 78.625 | 1.00x |
| wide_arrays.json | orjson | 5.369 | 5.483 | 6.118 | 78.625 | 0.78x |
| wide_arrays.json | msgspec | 5.992 | 6.060 | 6.383 | 78.625 | 0.71x |
| wide_arrays.json | ujson | 7.278 | 7.427 | 7.582 | 78.625 | 0.58x |
| wide_arrays.json | pysimdjson | 6.347 | 6.524 | 6.836 | 78.625 | 0.66x |
| wide_arrays.json | json | 10.023 | 10.264 | 10.731 | 78.625 | 0.42x |
| mixed.json | strata | 0.201 | 0.202 | 0.218 | 78.625 | 1.00x |
| mixed.json | orjson | 0.239 | 0.240 | 0.255 | 78.625 | 0.84x |
| mixed.json | msgspec | 0.253 | 0.257 | 0.272 | 78.625 | 0.79x |
| mixed.json | ujson | 0.316 | 0.320 | 0.337 | 78.625 | 0.63x |
| mixed.json | pysimdjson | 0.316 | 0.324 | 0.330 | 78.625 | 0.62x |
| mixed.json | json | 0.497 | 0.510 | 0.517 | 78.625 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.359 | 2.373 | 2.410 | 47.277 | 1.00x |
| users.json | orjson | 2.901 | 2.910 | 2.923 | 47.277 | 0.82x |
| users.json | msgspec | 3.858 | 3.875 | 3.921 | 47.277 | 0.61x |
| users.json | ujson | 11.214 | 11.309 | 11.770 | 47.277 | 0.21x |
| users.json | json | 21.953 | 22.051 | 22.341 | 47.277 | 0.11x |
| flat.json | strata | 0.275 | 0.277 | 0.297 | 66.625 | 1.00x |
| flat.json | orjson | 0.331 | 0.335 | 0.412 | 66.625 | 0.83x |
| flat.json | msgspec | 0.431 | 0.439 | 0.451 | 66.625 | 0.63x |
| flat.json | ujson | 1.010 | 1.012 | 1.040 | 66.625 | 0.27x |
| flat.json | json | 1.896 | 1.911 | 1.929 | 66.625 | 0.15x |
| nested.json | strata | 0.227 | 0.229 | 0.241 | 66.625 | 1.00x |
| nested.json | orjson | 0.292 | 0.296 | 0.305 | 66.625 | 0.77x |
| nested.json | msgspec | 0.402 | 0.415 | 0.427 | 66.625 | 0.55x |
| nested.json | ujson | 1.106 | 1.111 | 1.128 | 66.625 | 0.21x |
| nested.json | json | 2.434 | 2.450 | 2.466 | 66.625 | 0.09x |
| wide_arrays.json | strata | 1.677 | 1.697 | 1.865 | 78.625 | 1.00x |
| wide_arrays.json | orjson | 1.813 | 1.825 | 1.934 | 78.625 | 0.93x |
| wide_arrays.json | msgspec | 2.755 | 2.779 | 2.841 | 78.625 | 0.61x |
| wide_arrays.json | ujson | 6.338 | 6.360 | 6.455 | 78.625 | 0.27x |
| wide_arrays.json | json | 16.456 | 16.525 | 16.882 | 78.625 | 0.10x |
| mixed.json | strata | 0.059 | 0.061 | 0.071 | 78.625 | 1.00x |
| mixed.json | orjson | 0.063 | 0.065 | 0.067 | 78.625 | 0.94x |
| mixed.json | msgspec | 0.082 | 0.084 | 0.088 | 78.625 | 0.72x |
| mixed.json | ujson | 0.240 | 0.242 | 0.262 | 78.625 | 0.25x |
| mixed.json | json | 0.526 | 0.537 | 0.554 | 78.625 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.159 | 10.697 | 11.742 | 66.094 | 1.00x |
| users.json | orjson | 13.612 | 13.936 | 14.607 | 66.094 | 0.77x |
| users.json | msgspec | 13.594 | 13.843 | 16.096 | 66.094 | 0.77x |
| users.json | ujson | 18.601 | 20.089 | 22.103 | 66.094 | 0.53x |
| users.json | json | 22.658 | 22.948 | 23.373 | 66.094 | 0.47x |
| flat.json | strata | 0.869 | 0.889 | 0.926 | 66.625 | 1.00x |
| flat.json | orjson | 1.050 | 1.066 | 1.116 | 66.625 | 0.83x |
| flat.json | msgspec | 1.071 | 1.099 | 1.519 | 66.625 | 0.81x |
| flat.json | ujson | 1.583 | 1.649 | 1.727 | 66.625 | 0.54x |
| flat.json | json | 1.900 | 1.928 | 1.958 | 66.625 | 0.46x |
| nested.json | strata | 0.842 | 0.862 | 0.886 | 66.625 | 1.00x |
| nested.json | orjson | 1.081 | 1.086 | 1.115 | 66.625 | 0.79x |
| nested.json | msgspec | 1.134 | 1.144 | 1.154 | 66.625 | 0.75x |
| nested.json | ujson | 1.576 | 1.610 | 1.655 | 66.625 | 0.54x |
| nested.json | json | 2.168 | 2.184 | 2.201 | 66.625 | 0.39x |
| wide_arrays.json | strata | 4.190 | 4.225 | 4.347 | 78.625 | 1.00x |
| wide_arrays.json | orjson | 5.258 | 5.302 | 5.603 | 78.625 | 0.80x |
| wide_arrays.json | msgspec | 5.793 | 5.876 | 6.113 | 78.625 | 0.72x |
| wide_arrays.json | ujson | 7.246 | 7.356 | 7.642 | 78.625 | 0.57x |
| wide_arrays.json | json | 9.927 | 10.017 | 10.491 | 78.625 | 0.42x |
| mixed.json | strata | 0.214 | 0.216 | 0.237 | 78.625 | 1.00x |
| mixed.json | orjson | 0.283 | 0.295 | 0.307 | 78.625 | 0.73x |
| mixed.json | msgspec | 0.297 | 0.300 | 0.329 | 78.625 | 0.72x |
| mixed.json | ujson | 0.373 | 0.382 | 0.395 | 78.625 | 0.56x |
| mixed.json | json | 0.532 | 0.547 | 0.614 | 78.625 | 0.39x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.303 | 10.809 | 13.121 | 66.625 | 1.00x |
| users.ndjson | orjson | 16.869 | 17.314 | 18.334 | 66.625 | 0.62x |
| users.ndjson | msgspec | 16.788 | 17.561 | 19.077 | 66.625 | 0.62x |
| users.ndjson | ujson | 22.175 | 23.233 | 25.381 | 66.625 | 0.47x |
| users.ndjson | json | 29.344 | 30.573 | 31.445 | 66.625 | 0.35x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.997 | 3.082 | 3.217 | 66.094 | 1.00x |
| users.json | orjson | 3.641 | 3.714 | 4.567 | 66.094 | 0.83x |
| users.json | msgspec | 4.536 | 4.598 | 5.337 | 66.094 | 0.67x |
| users.json | ujson | 12.100 | 12.370 | 12.576 | 66.094 | 0.25x |
| users.json | json | 22.821 | 23.012 | 23.418 | 66.094 | 0.13x |
| flat.json | strata | 0.476 | 0.498 | 0.511 | 66.625 | 1.00x |
| flat.json | orjson | 0.550 | 0.575 | 0.594 | 66.625 | 0.87x |
| flat.json | msgspec | 0.651 | 0.671 | 0.725 | 66.625 | 0.74x |
| flat.json | ujson | 1.248 | 1.276 | 1.345 | 66.625 | 0.39x |
| flat.json | json | 2.135 | 2.160 | 3.154 | 66.625 | 0.23x |
| nested.json | strata | 0.403 | 0.429 | 0.459 | 66.625 | 1.00x |
| nested.json | orjson | 0.490 | 0.527 | 1.381 | 66.625 | 0.81x |
| nested.json | msgspec | 0.609 | 0.635 | 0.708 | 66.625 | 0.68x |
| nested.json | ujson | 1.341 | 1.364 | 1.442 | 66.625 | 0.31x |
| nested.json | json | 2.710 | 2.757 | 2.790 | 66.625 | 0.16x |
| wide_arrays.json | strata | 2.166 | 2.240 | 2.374 | 78.625 | 1.00x |
| wide_arrays.json | orjson | 2.348 | 2.373 | 2.499 | 78.625 | 0.94x |
| wide_arrays.json | msgspec | 3.295 | 3.310 | 3.398 | 78.625 | 0.68x |
| wide_arrays.json | ujson | 6.973 | 7.016 | 9.020 | 78.625 | 0.32x |
| wide_arrays.json | json | 17.158 | 17.290 | 19.673 | 78.625 | 0.13x |
| mixed.json | strata | 0.200 | 0.223 | 0.241 | 78.625 | 1.00x |
| mixed.json | orjson | 0.228 | 0.246 | 0.256 | 78.625 | 0.91x |
| mixed.json | msgspec | 0.244 | 0.252 | 0.326 | 78.625 | 0.88x |
| mixed.json | ujson | 0.408 | 0.421 | 0.465 | 78.625 | 0.53x |
| mixed.json | json | 0.710 | 0.734 | 3.302 | 78.625 | 0.30x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.065 | 0.067 | 0.088 | 66.094 | 1.00x |
| users.json $[*].id | jmespath | 0.500 | 0.516 | 0.560 | 66.094 | 0.13x |
| users.json $[*].id | jsonpath-ng | 2.866 | 2.956 | 3.551 | 66.094 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.428 | 0.452 | 0.476 | 66.094 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.099 | 3.139 | 3.259 | 66.094 | 0.14x |
| users.json $[*].orders[*].total | jsonpath-ng | 20.024 | 20.749 | 23.328 | 66.094 | 0.02x |
| users.json $..total | strata | 1.657 | 1.708 | 1.731 | 66.152 | 1.00x |
| users.json $..total | jsonpath-ng | 385.762 | 389.076 | 390.416 | 66.152 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.290 | 3.321 | 3.358 | 66.094 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.751 | 15.197 | 16.165 | 66.094 | 0.22x |
| users.json $[*].id | orjson+jsonpath-ng | 17.215 | 17.602 | 18.754 | 66.094 | 0.19x |
| users.json $[*].orders[*].total | strata | 3.543 | 3.598 | 3.660 | 66.152 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.888 | 18.556 | 19.141 | 66.152 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 37.848 | 40.175 | 45.758 | 66.152 | 0.09x |
| users.json $..total | strata | 14.865 | 15.154 | 16.329 | 68.262 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 410.037 | 419.569 | 429.236 | 68.262 | 0.04x |

