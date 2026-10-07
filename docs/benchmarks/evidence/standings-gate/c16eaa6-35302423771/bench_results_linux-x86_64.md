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
| users.json | strata | 7.754 | 8.217 | 11.419 | 65.320 | 1.00x |
| users.json | orjson | 10.823 | 11.509 | 13.750 | 65.320 | 0.71x |
| users.json | msgspec | 10.565 | 10.937 | 13.724 | 65.320 | 0.75x |
| users.json | ujson | 14.309 | 15.556 | 19.006 | 65.320 | 0.53x |
| users.json | pysimdjson | 15.721 | 16.384 | 20.110 | 65.320 | 0.50x |
| users.json | json | 16.279 | 16.765 | 18.360 | 65.320 | 0.49x |
| flat.json | strata | 0.715 | 0.732 | 0.787 | 65.066 | 1.00x |
| flat.json | orjson | 0.846 | 0.863 | 0.914 | 65.066 | 0.85x |
| flat.json | msgspec | 0.798 | 0.829 | 0.868 | 65.066 | 0.88x |
| flat.json | ujson | 1.283 | 1.451 | 1.510 | 65.066 | 0.50x |
| flat.json | pysimdjson | 1.319 | 1.395 | 1.463 | 65.066 | 0.52x |
| flat.json | json | 1.377 | 1.387 | 1.400 | 65.066 | 0.53x |
| nested.json | strata | 0.636 | 0.650 | 0.669 | 65.066 | 1.00x |
| nested.json | orjson | 0.799 | 0.811 | 0.848 | 65.066 | 0.80x |
| nested.json | msgspec | 0.738 | 0.763 | 0.830 | 65.066 | 0.85x |
| nested.json | ujson | 1.170 | 1.223 | 1.370 | 65.066 | 0.53x |
| nested.json | pysimdjson | 1.130 | 1.141 | 1.198 | 65.066 | 0.57x |
| nested.json | json | 1.438 | 1.458 | 1.566 | 65.066 | 0.45x |
| wide_arrays.json | strata | 3.594 | 3.672 | 4.110 | 77.879 | 1.00x |
| wide_arrays.json | orjson | 4.539 | 4.713 | 5.075 | 77.879 | 0.78x |
| wide_arrays.json | msgspec | 4.929 | 5.071 | 5.286 | 77.879 | 0.72x |
| wide_arrays.json | ujson | 6.096 | 6.218 | 7.468 | 77.879 | 0.59x |
| wide_arrays.json | pysimdjson | 5.355 | 5.549 | 5.961 | 77.879 | 0.66x |
| wide_arrays.json | json | 7.959 | 8.136 | 8.869 | 77.879 | 0.45x |
| mixed.json | strata | 0.149 | 0.156 | 0.169 | 77.879 | 1.00x |
| mixed.json | orjson | 0.185 | 0.193 | 0.207 | 77.879 | 0.81x |
| mixed.json | msgspec | 0.190 | 0.196 | 0.217 | 77.879 | 0.79x |
| mixed.json | ujson | 0.243 | 0.263 | 0.302 | 77.879 | 0.59x |
| mixed.json | pysimdjson | 0.239 | 0.253 | 0.270 | 77.879 | 0.62x |
| mixed.json | json | 0.349 | 0.361 | 0.376 | 77.879 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.815 | 1.827 | 1.869 | 47.363 | 1.00x |
| users.json | orjson | 1.980 | 2.020 | 2.061 | 47.363 | 0.90x |
| users.json | msgspec | 3.214 | 3.244 | 3.388 | 47.363 | 0.56x |
| users.json | ujson | 8.783 | 8.918 | 9.268 | 47.363 | 0.20x |
| users.json | json | 17.028 | 17.178 | 17.564 | 47.363 | 0.11x |
| flat.json | strata | 0.237 | 0.249 | 0.272 | 65.066 | 1.00x |
| flat.json | orjson | 0.246 | 0.255 | 0.305 | 65.066 | 0.98x |
| flat.json | msgspec | 0.372 | 0.381 | 0.406 | 65.066 | 0.65x |
| flat.json | ujson | 0.815 | 0.829 | 0.857 | 65.066 | 0.30x |
| flat.json | json | 1.480 | 1.497 | 1.546 | 65.066 | 0.17x |
| nested.json | strata | 0.179 | 0.181 | 0.194 | 65.066 | 1.00x |
| nested.json | orjson | 0.215 | 0.223 | 0.236 | 65.066 | 0.81x |
| nested.json | msgspec | 0.334 | 0.346 | 0.355 | 65.066 | 0.52x |
| nested.json | ujson | 0.838 | 0.847 | 0.859 | 65.066 | 0.21x |
| nested.json | json | 1.854 | 1.888 | 1.944 | 65.066 | 0.10x |
| wide_arrays.json | strata | 1.390 | 1.431 | 1.541 | 77.879 | 1.00x |
| wide_arrays.json | orjson | 1.461 | 1.493 | 1.542 | 77.879 | 0.96x |
| wide_arrays.json | msgspec | 2.439 | 2.475 | 2.565 | 77.879 | 0.58x |
| wide_arrays.json | ujson | 4.986 | 5.043 | 5.309 | 77.879 | 0.28x |
| wide_arrays.json | json | 13.139 | 13.290 | 13.864 | 77.879 | 0.11x |
| mixed.json | strata | 0.048 | 0.051 | 0.056 | 77.879 | 1.00x |
| mixed.json | orjson | 0.048 | 0.050 | 0.055 | 77.879 | 1.02x |
| mixed.json | msgspec | 0.067 | 0.070 | 0.099 | 77.879 | 0.73x |
| mixed.json | ujson | 0.178 | 0.186 | 0.198 | 77.879 | 0.27x |
| mixed.json | json | 0.396 | 0.403 | 0.422 | 77.879 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.809 | 9.280 | 11.267 | 66.703 | 1.00x |
| users.json | orjson | 11.815 | 12.032 | 12.508 | 66.703 | 0.77x |
| users.json | msgspec | 11.605 | 11.800 | 12.742 | 66.703 | 0.79x |
| users.json | ujson | 15.832 | 17.033 | 20.491 | 66.703 | 0.54x |
| users.json | json | 17.013 | 17.491 | 19.123 | 66.703 | 0.53x |
| flat.json | strata | 0.735 | 0.791 | 0.804 | 65.066 | 1.00x |
| flat.json | orjson | 0.916 | 0.938 | 1.031 | 65.066 | 0.84x |
| flat.json | msgspec | 0.863 | 0.901 | 0.926 | 65.066 | 0.88x |
| flat.json | ujson | 1.397 | 1.475 | 1.542 | 65.066 | 0.54x |
| flat.json | json | 1.435 | 1.460 | 1.514 | 65.066 | 0.54x |
| nested.json | strata | 0.674 | 0.713 | 0.764 | 65.066 | 1.00x |
| nested.json | orjson | 0.870 | 0.938 | 1.025 | 65.066 | 0.76x |
| nested.json | msgspec | 0.839 | 0.869 | 0.999 | 65.066 | 0.82x |
| nested.json | ujson | 1.265 | 1.297 | 1.483 | 65.066 | 0.55x |
| nested.json | json | 1.505 | 1.564 | 1.599 | 65.066 | 0.46x |
| wide_arrays.json | strata | 3.522 | 3.674 | 3.927 | 77.879 | 1.00x |
| wide_arrays.json | orjson | 4.420 | 4.641 | 5.001 | 77.879 | 0.79x |
| wide_arrays.json | msgspec | 4.863 | 4.988 | 5.089 | 77.879 | 0.74x |
| wide_arrays.json | ujson | 6.080 | 6.242 | 6.564 | 77.879 | 0.59x |
| wide_arrays.json | json | 7.669 | 7.893 | 8.213 | 77.879 | 0.47x |
| mixed.json | strata | 0.162 | 0.172 | 0.191 | 77.879 | 1.00x |
| mixed.json | orjson | 0.226 | 0.239 | 0.282 | 77.879 | 0.72x |
| mixed.json | msgspec | 0.219 | 0.237 | 0.264 | 77.879 | 0.73x |
| mixed.json | ujson | 0.286 | 0.306 | 0.336 | 77.879 | 0.56x |
| mixed.json | json | 0.386 | 0.393 | 0.407 | 77.879 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.352 | 11.068 | 12.578 | 65.066 | 1.00x |
| users.ndjson | orjson | 15.089 | 16.120 | 18.494 | 65.066 | 0.69x |
| users.ndjson | msgspec | 15.550 | 16.403 | 17.609 | 65.066 | 0.67x |
| users.ndjson | ujson | 19.734 | 21.266 | 23.332 | 65.066 | 0.52x |
| users.ndjson | json | 23.328 | 25.913 | 26.824 | 65.066 | 0.43x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.289 | 2.334 | 12.963 | 66.703 | 1.00x |
| users.json | orjson | 2.537 | 2.592 | 8.317 | 66.703 | 0.90x |
| users.json | msgspec | 3.697 | 3.782 | 3.886 | 66.703 | 0.62x |
| users.json | ujson | 9.394 | 9.660 | 46.969 | 66.703 | 0.24x |
| users.json | json | 17.626 | 17.792 | 56.332 | 66.703 | 0.13x |
| flat.json | strata | 0.385 | 0.423 | 15.552 | 65.066 | 1.00x |
| flat.json | orjson | 0.407 | 0.443 | 0.571 | 65.066 | 0.96x |
| flat.json | msgspec | 0.542 | 0.581 | 0.607 | 65.066 | 0.73x |
| flat.json | ujson | 0.992 | 1.016 | 1.082 | 65.066 | 0.42x |
| flat.json | json | 1.690 | 1.720 | 1.772 | 65.066 | 0.25x |
| nested.json | strata | 0.283 | 0.304 | 0.322 | 65.066 | 1.00x |
| nested.json | orjson | 0.343 | 0.357 | 0.386 | 65.066 | 0.85x |
| nested.json | msgspec | 0.454 | 0.477 | 0.497 | 65.066 | 0.64x |
| nested.json | ujson | 0.982 | 1.002 | 1.011 | 65.066 | 0.30x |
| nested.json | json | 2.026 | 2.052 | 2.133 | 65.066 | 0.15x |
| wide_arrays.json | strata | 1.738 | 1.806 | 1.945 | 77.879 | 1.00x |
| wide_arrays.json | orjson | 1.855 | 1.954 | 30.565 | 77.879 | 0.92x |
| wide_arrays.json | msgspec | 2.829 | 2.870 | 3.049 | 77.879 | 0.63x |
| wide_arrays.json | ujson | 5.442 | 5.494 | 5.555 | 77.879 | 0.33x |
| wide_arrays.json | json | 13.581 | 13.694 | 18.964 | 77.879 | 0.13x |
| mixed.json | strata | 0.117 | 0.123 | 0.144 | 77.879 | 1.00x |
| mixed.json | orjson | 0.132 | 0.142 | 0.165 | 77.879 | 0.86x |
| mixed.json | msgspec | 0.148 | 0.156 | 0.171 | 77.879 | 0.78x |
| mixed.json | ujson | 0.271 | 0.292 | 0.343 | 77.879 | 0.42x |
| mixed.json | json | 0.516 | 0.522 | 0.540 | 77.879 | 0.23x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.060 | 0.070 | 0.096 | 66.703 | 1.00x |
| users.json $[*].id | jmespath | 0.376 | 0.394 | 0.448 | 66.703 | 0.18x |
| users.json $[*].id | jsonpath-ng | 2.217 | 2.432 | 2.569 | 66.703 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.337 | 0.362 | 0.398 | 66.703 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.415 | 2.453 | 2.502 | 66.703 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 15.405 | 16.495 | 17.917 | 66.703 | 0.02x |
| users.json $..total | strata | 1.429 | 1.476 | 1.790 | 66.703 | 1.00x |
| users.json $..total | jsonpath-ng | 299.896 | 301.554 | 302.728 | 66.703 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 2.608 | 2.624 | 2.724 | 66.703 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.877 | 13.276 | 15.085 | 66.703 | 0.20x |
| users.json $[*].id | orjson+jsonpath-ng | 14.780 | 15.598 | 18.054 | 66.703 | 0.17x |
| users.json $[*].orders[*].total | strata | 2.819 | 2.869 | 2.938 | 66.703 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.996 | 16.666 | 18.481 | 66.703 | 0.17x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 32.673 | 35.998 | 38.165 | 66.703 | 0.08x |
| users.json $..total | strata | 12.361 | 15.646 | 18.649 | 66.703 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 327.374 | 332.603 | 335.028 | 66.703 | 0.05x |

