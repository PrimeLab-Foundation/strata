# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 38eaa9f248f509d9a4beb75b267189d011c1109e
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-aarch64-with-glibc2.39
- machine: aarch64
- processor: aarch64
- compiler_flags: -fno-strict-overflow -Wsign-compare -DNDEBUG -g -O3 -Wall -fPIC -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -march=native -flto=thin -fprofile-use=/home/runner/work/_temp/strata-arm/build/pgo/strata.profdata -shared -Wl,--rpath=/opt/hostedtoolcache/Python/3.12.14/arm64/lib (19 recorded commands; see the JSON companion)
- repeats: 60
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.869 | 9.026 | 9.199 | 57.348 | 1.00x |
| users.json | orjson | 11.456 | 11.791 | 12.028 | 57.348 | 0.77x |
| users.json | msgspec | 11.966 | 12.236 | 12.460 | 57.348 | 0.74x |
| users.json | ujson | 16.210 | 16.664 | 17.015 | 57.348 | 0.54x |
| users.json | pysimdjson | 16.332 | 16.745 | 17.291 | 57.348 | 0.54x |
| users.json | json | 20.364 | 20.585 | 21.106 | 57.348 | 0.44x |
| flat.json | strata | 0.823 | 0.862 | 0.879 | 59.402 | 1.00x |
| flat.json | orjson | 0.862 | 0.884 | 0.901 | 59.402 | 0.97x |
| flat.json | msgspec | 0.903 | 0.925 | 0.940 | 59.402 | 0.93x |
| flat.json | ujson | 1.433 | 1.464 | 1.494 | 59.402 | 0.59x |
| flat.json | pysimdjson | 1.471 | 1.491 | 1.510 | 59.402 | 0.58x |
| flat.json | json | 1.763 | 1.785 | 1.798 | 59.402 | 0.48x |
| nested.json | strata | 0.818 | 0.837 | 0.847 | 59.402 | 1.00x |
| nested.json | orjson | 0.872 | 0.894 | 0.909 | 59.402 | 0.94x |
| nested.json | msgspec | 0.998 | 1.010 | 1.027 | 59.402 | 0.83x |
| nested.json | ujson | 1.405 | 1.428 | 1.455 | 59.402 | 0.59x |
| nested.json | pysimdjson | 1.407 | 1.422 | 1.450 | 59.402 | 0.59x |
| nested.json | json | 1.965 | 1.991 | 2.007 | 59.402 | 0.42x |
| wide_arrays.json | strata | 3.898 | 3.950 | 3.986 | 65.965 | 1.00x |
| wide_arrays.json | orjson | 4.023 | 4.078 | 4.136 | 65.965 | 0.97x |
| wide_arrays.json | msgspec | 5.032 | 5.086 | 5.132 | 65.965 | 0.78x |
| wide_arrays.json | ujson | 6.475 | 6.530 | 6.582 | 65.965 | 0.60x |
| wide_arrays.json | pysimdjson | 5.234 | 5.286 | 5.340 | 65.965 | 0.75x |
| wide_arrays.json | json | 9.433 | 9.504 | 9.561 | 65.965 | 0.42x |
| mixed.json | strata | 0.192 | 0.198 | 0.219 | 65.965 | 1.00x |
| mixed.json | orjson | 0.213 | 0.219 | 0.241 | 65.965 | 0.90x |
| mixed.json | msgspec | 0.230 | 0.238 | 0.256 | 65.965 | 0.83x |
| mixed.json | ujson | 0.299 | 0.308 | 0.330 | 65.965 | 0.64x |
| mixed.json | pysimdjson | 0.287 | 0.295 | 0.321 | 65.965 | 0.67x |
| mixed.json | json | 0.445 | 0.460 | 0.479 | 65.965 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.969 | 1.989 | 2.006 | 46.008 | 1.00x |
| users.json | orjson | 2.634 | 2.657 | 2.689 | 46.008 | 0.75x |
| users.json | msgspec | 3.355 | 3.378 | 3.407 | 46.008 | 0.59x |
| users.json | ujson | 10.516 | 10.593 | 10.674 | 46.008 | 0.19x |
| users.json | json | 19.240 | 19.341 | 19.434 | 46.008 | 0.10x |
| flat.json | strata | 0.237 | 0.241 | 0.259 | 59.402 | 1.00x |
| flat.json | orjson | 0.303 | 0.308 | 0.329 | 59.402 | 0.78x |
| flat.json | msgspec | 0.390 | 0.400 | 0.417 | 59.402 | 0.60x |
| flat.json | ujson | 0.988 | 1.003 | 1.014 | 59.402 | 0.24x |
| flat.json | json | 1.700 | 1.721 | 1.739 | 59.402 | 0.14x |
| nested.json | strata | 0.214 | 0.219 | 0.241 | 59.402 | 1.00x |
| nested.json | orjson | 0.289 | 0.293 | 0.317 | 59.402 | 0.75x |
| nested.json | msgspec | 0.368 | 0.373 | 0.393 | 59.402 | 0.59x |
| nested.json | ujson | 1.074 | 1.087 | 1.103 | 59.402 | 0.20x |
| nested.json | json | 2.140 | 2.170 | 2.212 | 59.402 | 0.10x |
| wide_arrays.json | strata | 1.301 | 1.322 | 1.354 | 65.965 | 1.00x |
| wide_arrays.json | orjson | 1.568 | 1.591 | 1.618 | 65.965 | 0.83x |
| wide_arrays.json | msgspec | 2.347 | 2.371 | 2.402 | 65.965 | 0.56x |
| wide_arrays.json | ujson | 4.742 | 4.772 | 4.809 | 65.965 | 0.28x |
| wide_arrays.json | json | 13.532 | 13.583 | 13.650 | 65.965 | 0.10x |
| mixed.json | strata | 0.060 | 0.063 | 0.066 | 65.965 | 1.00x |
| mixed.json | orjson | 0.063 | 0.066 | 0.083 | 65.965 | 0.95x |
| mixed.json | msgspec | 0.075 | 0.079 | 0.098 | 65.965 | 0.80x |
| mixed.json | ujson | 0.237 | 0.242 | 0.268 | 65.965 | 0.26x |
| mixed.json | json | 0.474 | 0.491 | 0.504 | 65.965 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.145 | 9.405 | 9.658 | 60.734 | 1.00x |
| users.json | orjson | 11.810 | 12.193 | 12.529 | 60.734 | 0.77x |
| users.json | msgspec | 12.393 | 12.669 | 12.984 | 60.734 | 0.74x |
| users.json | ujson | 17.030 | 17.535 | 18.246 | 60.734 | 0.54x |
| users.json | json | 20.645 | 21.099 | 21.516 | 60.734 | 0.45x |
| flat.json | strata | 0.846 | 0.884 | 0.913 | 59.402 | 1.00x |
| flat.json | orjson | 0.935 | 0.960 | 0.976 | 59.402 | 0.92x |
| flat.json | msgspec | 0.970 | 1.002 | 1.017 | 59.402 | 0.88x |
| flat.json | ujson | 1.533 | 1.563 | 1.601 | 59.402 | 0.57x |
| flat.json | json | 1.821 | 1.847 | 1.868 | 59.402 | 0.48x |
| nested.json | strata | 0.844 | 0.871 | 0.884 | 59.402 | 1.00x |
| nested.json | orjson | 0.932 | 0.955 | 0.969 | 59.402 | 0.91x |
| nested.json | msgspec | 1.052 | 1.069 | 1.080 | 59.402 | 0.82x |
| nested.json | ujson | 1.464 | 1.495 | 1.523 | 59.402 | 0.58x |
| nested.json | json | 2.014 | 2.034 | 2.054 | 59.402 | 0.43x |
| wide_arrays.json | strata | 3.927 | 3.974 | 4.018 | 65.965 | 1.00x |
| wide_arrays.json | orjson | 4.021 | 4.115 | 4.184 | 65.965 | 0.97x |
| wide_arrays.json | msgspec | 5.068 | 5.146 | 5.209 | 65.965 | 0.77x |
| wide_arrays.json | ujson | 6.635 | 6.722 | 6.782 | 65.965 | 0.59x |
| wide_arrays.json | json | 9.446 | 9.555 | 9.636 | 65.965 | 0.42x |
| mixed.json | strata | 0.216 | 0.224 | 0.247 | 65.965 | 1.00x |
| mixed.json | orjson | 0.277 | 0.286 | 0.306 | 65.965 | 0.78x |
| mixed.json | msgspec | 0.295 | 0.303 | 0.328 | 65.965 | 0.74x |
| mixed.json | ujson | 0.378 | 0.392 | 0.417 | 65.965 | 0.57x |
| mixed.json | json | 0.504 | 0.523 | 0.545 | 65.965 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.506 | 9.764 | 10.007 | 59.395 | 1.00x |
| users.ndjson | orjson | 14.492 | 14.947 | 15.238 | 59.395 | 0.65x |
| users.ndjson | msgspec | 14.907 | 15.217 | 15.608 | 59.395 | 0.64x |
| users.ndjson | ujson | 19.545 | 19.971 | 20.435 | 59.395 | 0.49x |
| users.ndjson | json | 25.382 | 26.163 | 26.662 | 59.395 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.526 | 2.634 | 2.746 | 60.734 | 1.00x |
| users.json | orjson | 3.229 | 3.317 | 3.424 | 60.734 | 0.79x |
| users.json | msgspec | 3.942 | 4.043 | 4.181 | 60.734 | 0.65x |
| users.json | ujson | 11.249 | 11.388 | 11.573 | 60.734 | 0.23x |
| users.json | json | 19.865 | 20.141 | 20.388 | 60.734 | 0.13x |
| flat.json | strata | 0.459 | 0.507 | 0.544 | 59.402 | 1.00x |
| flat.json | orjson | 0.556 | 0.600 | 0.642 | 59.402 | 0.85x |
| flat.json | msgspec | 0.645 | 0.687 | 0.731 | 59.402 | 0.74x |
| flat.json | ujson | 1.272 | 1.325 | 1.384 | 59.402 | 0.38x |
| flat.json | json | 1.997 | 2.035 | 2.088 | 59.402 | 0.25x |
| nested.json | strata | 0.411 | 0.452 | 0.493 | 59.402 | 1.00x |
| nested.json | orjson | 0.514 | 0.556 | 0.611 | 59.402 | 0.81x |
| nested.json | msgspec | 0.590 | 0.636 | 0.686 | 59.402 | 0.71x |
| nested.json | ujson | 1.354 | 1.388 | 1.434 | 59.402 | 0.33x |
| nested.json | json | 2.380 | 2.446 | 2.490 | 59.402 | 0.18x |
| wide_arrays.json | strata | 1.745 | 1.802 | 1.879 | 65.965 | 1.00x |
| wide_arrays.json | orjson | 2.031 | 2.101 | 2.171 | 65.965 | 0.86x |
| wide_arrays.json | msgspec | 2.796 | 2.870 | 2.920 | 65.965 | 0.63x |
| wide_arrays.json | ujson | 5.239 | 5.336 | 5.408 | 65.965 | 0.34x |
| wide_arrays.json | json | 14.078 | 14.182 | 14.270 | 65.965 | 0.13x |
| mixed.json | strata | 0.243 | 0.278 | 0.319 | 65.965 | 1.00x |
| mixed.json | orjson | 0.273 | 0.321 | 0.378 | 65.965 | 0.87x |
| mixed.json | msgspec | 0.297 | 0.336 | 0.369 | 65.965 | 0.83x |
| mixed.json | ujson | 0.470 | 0.513 | 0.547 | 65.965 | 0.54x |
| mixed.json | json | 0.723 | 0.768 | 0.803 | 65.965 | 0.36x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.102 | 0.105 | 0.114 | 60.734 | 1.00x |
| users.json $[*].id | jmespath | 0.466 | 0.482 | 0.501 | 60.734 | 0.22x |
| users.json $[*].id | jsonpath-ng | 2.427 | 2.510 | 2.619 | 60.734 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.621 | 0.637 | 0.660 | 60.852 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.011 | 3.037 | 3.076 | 60.852 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.383 | 18.079 | 18.637 | 60.852 | 0.04x |
| users.json $..total | strata | 1.758 | 1.780 | 1.809 | 61.016 | 1.00x |
| users.json $..total | jsonpath-ng | 293.302 | 294.411 | 295.074 | 61.016 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.194 | 3.227 | 3.425 | 60.852 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.488 | 13.024 | 13.328 | 60.852 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 14.465 | 14.749 | 15.243 | 60.852 | 0.22x |
| users.json $[*].orders[*].total | strata | 3.381 | 3.407 | 3.759 | 61.016 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.216 | 15.745 | 16.072 | 61.016 | 0.22x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 33.206 | 34.269 | 35.339 | 61.016 | 0.10x |
| users.json $..total | strata | 11.740 | 12.489 | 12.957 | 61.031 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 314.726 | 316.933 | 318.476 | 61.031 | 0.04x |

