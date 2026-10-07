# Benchmark results - ci-linux-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c89aea890fd23de6c299c86e4099ab38c8418300
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
| users.json | strata | 8.876 | 8.965 | 10.965 | 57.246 | 1.00x |
| users.json | orjson | 11.939 | 12.091 | 13.910 | 57.246 | 0.74x |
| users.json | msgspec | 12.390 | 12.534 | 14.192 | 57.246 | 0.72x |
| users.json | ujson | 16.803 | 17.014 | 20.050 | 57.246 | 0.53x |
| users.json | pysimdjson | 16.878 | 17.254 | 19.398 | 57.246 | 0.52x |
| users.json | json | 20.892 | 21.097 | 21.961 | 57.246 | 0.42x |
| flat.json | strata | 0.820 | 0.841 | 0.856 | 68.000 | 1.00x |
| flat.json | orjson | 0.863 | 0.884 | 0.913 | 68.000 | 0.95x |
| flat.json | msgspec | 0.911 | 0.928 | 0.951 | 68.000 | 0.91x |
| flat.json | ujson | 1.452 | 1.471 | 1.505 | 68.000 | 0.57x |
| flat.json | pysimdjson | 1.498 | 1.517 | 1.528 | 68.000 | 0.55x |
| flat.json | json | 1.804 | 1.809 | 1.814 | 68.000 | 0.46x |
| nested.json | strata | 0.804 | 0.820 | 0.829 | 68.000 | 1.00x |
| nested.json | orjson | 0.874 | 0.887 | 0.891 | 68.000 | 0.92x |
| nested.json | msgspec | 0.983 | 0.993 | 1.006 | 68.000 | 0.83x |
| nested.json | ujson | 1.404 | 1.427 | 1.466 | 68.000 | 0.57x |
| nested.json | pysimdjson | 1.394 | 1.409 | 1.422 | 68.000 | 0.58x |
| nested.json | json | 1.955 | 1.967 | 1.984 | 68.000 | 0.42x |
| wide_arrays.json | strata | 3.879 | 3.952 | 4.001 | 69.570 | 1.00x |
| wide_arrays.json | orjson | 4.042 | 4.100 | 4.135 | 69.570 | 0.96x |
| wide_arrays.json | msgspec | 5.008 | 5.047 | 5.134 | 69.570 | 0.78x |
| wide_arrays.json | ujson | 6.462 | 6.530 | 6.575 | 69.570 | 0.61x |
| wide_arrays.json | pysimdjson | 5.273 | 5.326 | 5.366 | 69.570 | 0.74x |
| wide_arrays.json | json | 9.482 | 9.556 | 9.651 | 69.570 | 0.41x |
| mixed.json | strata | 0.194 | 0.195 | 0.211 | 69.570 | 1.00x |
| mixed.json | orjson | 0.216 | 0.221 | 0.243 | 69.570 | 0.88x |
| mixed.json | msgspec | 0.236 | 0.240 | 0.261 | 69.570 | 0.81x |
| mixed.json | ujson | 0.314 | 0.319 | 0.340 | 69.570 | 0.61x |
| mixed.json | pysimdjson | 0.297 | 0.301 | 0.319 | 69.570 | 0.65x |
| mixed.json | json | 0.460 | 0.475 | 0.492 | 69.570 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.948 | 1.958 | 1.976 | 56.352 | 1.00x |
| users.json | orjson | 2.603 | 2.615 | 2.640 | 56.352 | 0.75x |
| users.json | msgspec | 3.340 | 3.354 | 3.437 | 56.352 | 0.58x |
| users.json | ujson | 10.551 | 10.592 | 10.615 | 56.352 | 0.18x |
| users.json | json | 19.008 | 19.095 | 19.237 | 56.352 | 0.10x |
| flat.json | strata | 0.236 | 0.239 | 0.261 | 68.000 | 1.00x |
| flat.json | orjson | 0.300 | 0.303 | 0.321 | 68.000 | 0.79x |
| flat.json | msgspec | 0.387 | 0.401 | 0.415 | 68.000 | 0.60x |
| flat.json | ujson | 1.000 | 1.004 | 1.016 | 68.000 | 0.24x |
| flat.json | json | 1.714 | 1.728 | 1.742 | 68.000 | 0.14x |
| nested.json | strata | 0.214 | 0.217 | 0.239 | 68.000 | 1.00x |
| nested.json | orjson | 0.279 | 0.282 | 0.301 | 68.000 | 0.77x |
| nested.json | msgspec | 0.365 | 0.373 | 0.390 | 68.000 | 0.58x |
| nested.json | ujson | 1.086 | 1.103 | 1.118 | 68.000 | 0.20x |
| nested.json | json | 2.154 | 2.173 | 2.206 | 68.000 | 0.10x |
| wide_arrays.json | strata | 1.336 | 1.368 | 1.379 | 69.570 | 1.00x |
| wide_arrays.json | orjson | 1.595 | 1.604 | 1.621 | 69.570 | 0.85x |
| wide_arrays.json | msgspec | 2.350 | 2.363 | 2.378 | 69.570 | 0.58x |
| wide_arrays.json | ujson | 4.748 | 4.764 | 4.787 | 69.570 | 0.29x |
| wide_arrays.json | json | 13.585 | 13.633 | 13.683 | 69.570 | 0.10x |
| mixed.json | strata | 0.062 | 0.064 | 0.066 | 69.570 | 1.00x |
| mixed.json | orjson | 0.065 | 0.066 | 0.068 | 69.570 | 0.97x |
| mixed.json | msgspec | 0.080 | 0.082 | 0.094 | 69.570 | 0.78x |
| mixed.json | ujson | 0.241 | 0.245 | 0.269 | 69.570 | 0.26x |
| mixed.json | json | 0.487 | 0.501 | 0.522 | 69.570 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.196 | 9.352 | 10.280 | 68.434 | 1.00x |
| users.json | orjson | 12.307 | 12.514 | 13.126 | 68.434 | 0.75x |
| users.json | msgspec | 12.772 | 13.026 | 13.118 | 68.434 | 0.72x |
| users.json | ujson | 17.995 | 18.310 | 19.274 | 68.434 | 0.51x |
| users.json | json | 21.463 | 21.755 | 21.934 | 68.434 | 0.43x |
| flat.json | strata | 0.845 | 0.878 | 0.886 | 68.000 | 1.00x |
| flat.json | orjson | 0.939 | 0.953 | 0.968 | 68.000 | 0.92x |
| flat.json | msgspec | 0.996 | 1.002 | 1.018 | 68.000 | 0.88x |
| flat.json | ujson | 1.564 | 1.572 | 1.593 | 68.000 | 0.56x |
| flat.json | json | 1.858 | 1.863 | 1.878 | 68.000 | 0.47x |
| nested.json | strata | 0.836 | 0.851 | 0.885 | 68.000 | 1.00x |
| nested.json | orjson | 0.927 | 0.959 | 1.019 | 68.000 | 0.89x |
| nested.json | msgspec | 1.044 | 1.074 | 1.138 | 68.000 | 0.79x |
| nested.json | ujson | 1.481 | 1.526 | 1.605 | 68.000 | 0.56x |
| nested.json | json | 2.004 | 2.048 | 2.110 | 68.000 | 0.42x |
| wide_arrays.json | strata | 3.878 | 3.949 | 4.036 | 69.570 | 1.00x |
| wide_arrays.json | orjson | 4.076 | 4.178 | 4.291 | 69.570 | 0.95x |
| wide_arrays.json | msgspec | 5.125 | 5.178 | 5.304 | 69.570 | 0.76x |
| wide_arrays.json | ujson | 6.694 | 6.754 | 6.934 | 69.570 | 0.58x |
| wide_arrays.json | json | 9.630 | 9.737 | 9.924 | 69.570 | 0.41x |
| mixed.json | strata | 0.213 | 0.220 | 0.240 | 69.570 | 1.00x |
| mixed.json | orjson | 0.276 | 0.285 | 0.307 | 69.570 | 0.77x |
| mixed.json | msgspec | 0.297 | 0.303 | 0.321 | 69.570 | 0.73x |
| mixed.json | ujson | 0.384 | 0.390 | 0.409 | 69.570 | 0.57x |
| mixed.json | json | 0.511 | 0.536 | 0.543 | 69.570 | 0.41x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.649 | 9.790 | 9.958 | 67.992 | 1.00x |
| users.ndjson | orjson | 14.965 | 15.173 | 15.331 | 67.992 | 0.65x |
| users.ndjson | msgspec | 15.245 | 15.456 | 15.692 | 67.992 | 0.63x |
| users.ndjson | ujson | 19.902 | 20.186 | 20.419 | 67.992 | 0.48x |
| users.ndjson | json | 26.431 | 26.668 | 27.141 | 67.992 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.509 | 2.544 | 2.584 | 68.434 | 1.00x |
| users.json | orjson | 3.227 | 3.255 | 3.323 | 68.434 | 0.78x |
| users.json | msgspec | 3.969 | 4.002 | 4.034 | 68.434 | 0.64x |
| users.json | ujson | 11.304 | 11.380 | 11.487 | 68.434 | 0.22x |
| users.json | json | 19.846 | 19.959 | 20.257 | 68.434 | 0.13x |
| flat.json | strata | 0.400 | 0.429 | 3.148 | 68.000 | 1.00x |
| flat.json | orjson | 0.491 | 0.525 | 0.545 | 68.000 | 0.82x |
| flat.json | msgspec | 0.582 | 0.598 | 0.654 | 68.000 | 0.72x |
| flat.json | ujson | 1.209 | 1.244 | 1.280 | 68.000 | 0.35x |
| flat.json | json | 1.933 | 1.957 | 1.989 | 68.000 | 0.22x |
| nested.json | strata | 0.363 | 0.375 | 0.397 | 68.000 | 1.00x |
| nested.json | orjson | 0.462 | 0.476 | 0.499 | 68.000 | 0.79x |
| nested.json | msgspec | 0.548 | 0.568 | 0.600 | 68.000 | 0.66x |
| nested.json | ujson | 1.279 | 1.301 | 1.344 | 68.000 | 0.29x |
| nested.json | json | 2.356 | 2.385 | 2.414 | 68.000 | 0.16x |
| wide_arrays.json | strata | 1.827 | 1.865 | 1.897 | 69.570 | 1.00x |
| wide_arrays.json | orjson | 2.122 | 2.139 | 2.166 | 69.570 | 0.87x |
| wide_arrays.json | msgspec | 2.822 | 2.861 | 2.901 | 69.570 | 0.65x |
| wide_arrays.json | ujson | 5.274 | 5.319 | 5.386 | 69.570 | 0.35x |
| wide_arrays.json | json | 14.292 | 14.313 | 14.368 | 69.570 | 0.13x |
| mixed.json | strata | 0.171 | 0.187 | 0.202 | 69.570 | 1.00x |
| mixed.json | orjson | 0.193 | 0.208 | 0.234 | 69.570 | 0.90x |
| mixed.json | msgspec | 0.213 | 0.221 | 0.255 | 69.570 | 0.85x |
| mixed.json | ujson | 0.398 | 0.409 | 0.446 | 69.570 | 0.46x |
| mixed.json | json | 0.632 | 0.664 | 0.685 | 69.570 | 0.28x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.115 | 0.119 | 0.132 | 68.434 | 1.00x |
| users.json $[*].id | jmespath | 0.492 | 0.501 | 0.515 | 68.434 | 0.24x |
| users.json $[*].id | jsonpath-ng | 2.521 | 2.623 | 2.655 | 68.434 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.619 | 0.626 | 0.661 | 68.559 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.968 | 2.983 | 2.993 | 68.559 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 18.077 | 18.502 | 18.631 | 68.559 | 0.03x |
| users.json $..total | strata | 1.697 | 1.714 | 1.771 | 69.566 | 1.00x |
| users.json $..total | jsonpath-ng | 294.201 | 295.252 | 295.602 | 69.566 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.205 | 3.225 | 3.252 | 68.559 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.120 | 13.414 | 13.687 | 68.559 | 0.24x |
| users.json $[*].id | orjson+jsonpath-ng | 15.088 | 15.232 | 15.736 | 68.559 | 0.21x |
| users.json $[*].orders[*].total | strata | 3.342 | 3.392 | 3.413 | 69.566 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.402 | 15.972 | 16.482 | 69.566 | 0.21x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 33.227 | 34.648 | 37.729 | 69.566 | 0.10x |
| users.json $..total | strata | 11.351 | 11.961 | 12.679 | 69.629 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 312.056 | 315.212 | 315.930 | 69.629 | 0.04x |

