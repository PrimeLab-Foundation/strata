# Benchmark results - ci-linux-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: bc6d9ba83ad33f6b9e2f3b35d87b07d0b4dcc11e
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
| users.json | strata | 8.985 | 9.068 | 10.932 | 57.305 | 1.00x |
| users.json | orjson | 11.805 | 11.907 | 13.340 | 57.305 | 0.76x |
| users.json | msgspec | 12.347 | 12.458 | 13.810 | 57.305 | 0.73x |
| users.json | ujson | 16.580 | 16.695 | 18.957 | 57.305 | 0.54x |
| users.json | pysimdjson | 16.550 | 16.690 | 18.604 | 57.305 | 0.54x |
| users.json | json | 20.775 | 20.815 | 21.540 | 57.305 | 0.44x |
| flat.json | strata | 0.833 | 0.865 | 0.900 | 68.523 | 1.00x |
| flat.json | orjson | 0.872 | 0.881 | 0.910 | 68.523 | 0.98x |
| flat.json | msgspec | 0.926 | 0.937 | 0.958 | 68.523 | 0.92x |
| flat.json | ujson | 1.433 | 1.447 | 1.489 | 68.523 | 0.60x |
| flat.json | pysimdjson | 1.473 | 1.493 | 1.509 | 68.523 | 0.58x |
| flat.json | json | 1.770 | 1.783 | 1.796 | 68.523 | 0.49x |
| nested.json | strata | 0.812 | 0.833 | 0.855 | 68.523 | 1.00x |
| nested.json | orjson | 0.875 | 0.900 | 0.909 | 68.523 | 0.93x |
| nested.json | msgspec | 1.015 | 1.018 | 1.042 | 68.523 | 0.82x |
| nested.json | ujson | 1.397 | 1.424 | 1.443 | 68.523 | 0.58x |
| nested.json | pysimdjson | 1.408 | 1.427 | 1.447 | 68.523 | 0.58x |
| nested.json | json | 1.980 | 1.995 | 2.005 | 68.523 | 0.42x |
| wide_arrays.json | strata | 3.928 | 3.959 | 3.989 | 70.152 | 1.00x |
| wide_arrays.json | orjson | 4.117 | 4.154 | 4.195 | 70.152 | 0.95x |
| wide_arrays.json | msgspec | 5.150 | 5.159 | 5.203 | 70.152 | 0.77x |
| wide_arrays.json | ujson | 6.582 | 6.602 | 6.620 | 70.152 | 0.60x |
| wide_arrays.json | pysimdjson | 5.311 | 5.359 | 5.398 | 70.152 | 0.74x |
| wide_arrays.json | json | 9.548 | 9.586 | 9.614 | 70.152 | 0.41x |
| mixed.json | strata | 0.199 | 0.216 | 0.234 | 70.152 | 1.00x |
| mixed.json | orjson | 0.212 | 0.244 | 0.268 | 70.152 | 0.89x |
| mixed.json | msgspec | 0.244 | 0.262 | 0.273 | 70.152 | 0.82x |
| mixed.json | ujson | 0.305 | 0.316 | 0.337 | 70.152 | 0.68x |
| mixed.json | pysimdjson | 0.295 | 0.302 | 0.310 | 70.152 | 0.72x |
| mixed.json | json | 0.453 | 0.465 | 0.504 | 70.152 | 0.46x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.925 | 1.932 | 1.944 | 56.398 | 1.00x |
| users.json | orjson | 2.587 | 2.601 | 2.617 | 56.398 | 0.74x |
| users.json | msgspec | 3.281 | 3.297 | 3.310 | 56.398 | 0.59x |
| users.json | ujson | 10.494 | 10.505 | 10.527 | 56.398 | 0.18x |
| users.json | json | 19.094 | 19.158 | 19.227 | 56.398 | 0.10x |
| flat.json | strata | 0.237 | 0.239 | 0.242 | 68.523 | 1.00x |
| flat.json | orjson | 0.306 | 0.317 | 0.330 | 68.523 | 0.75x |
| flat.json | msgspec | 0.395 | 0.419 | 0.425 | 68.523 | 0.57x |
| flat.json | ujson | 0.998 | 1.005 | 1.017 | 68.523 | 0.24x |
| flat.json | json | 1.706 | 1.723 | 1.743 | 68.523 | 0.14x |
| nested.json | strata | 0.220 | 0.224 | 0.258 | 68.523 | 1.00x |
| nested.json | orjson | 0.287 | 0.290 | 0.307 | 68.523 | 0.77x |
| nested.json | msgspec | 0.374 | 0.379 | 0.404 | 68.523 | 0.59x |
| nested.json | ujson | 1.110 | 1.114 | 1.147 | 68.523 | 0.20x |
| nested.json | json | 2.158 | 2.176 | 2.209 | 68.523 | 0.10x |
| wide_arrays.json | strata | 1.323 | 1.329 | 1.353 | 70.152 | 1.00x |
| wide_arrays.json | orjson | 1.566 | 1.581 | 1.608 | 70.152 | 0.84x |
| wide_arrays.json | msgspec | 2.395 | 2.413 | 2.435 | 70.152 | 0.55x |
| wide_arrays.json | ujson | 4.746 | 4.771 | 4.806 | 70.152 | 0.28x |
| wide_arrays.json | json | 13.609 | 13.640 | 13.690 | 70.152 | 0.10x |
| mixed.json | strata | 0.062 | 0.065 | 0.106 | 70.152 | 1.00x |
| mixed.json | orjson | 0.066 | 0.070 | 0.084 | 70.152 | 0.93x |
| mixed.json | msgspec | 0.079 | 0.082 | 0.100 | 70.152 | 0.79x |
| mixed.json | ujson | 0.244 | 0.248 | 0.270 | 70.152 | 0.26x |
| mixed.json | json | 0.486 | 0.497 | 0.518 | 70.152 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.208 | 9.302 | 10.296 | 70.156 | 1.00x |
| users.json | orjson | 12.058 | 12.163 | 12.707 | 70.156 | 0.76x |
| users.json | msgspec | 12.654 | 12.767 | 13.049 | 70.156 | 0.73x |
| users.json | ujson | 17.145 | 17.366 | 18.390 | 70.156 | 0.54x |
| users.json | json | 21.009 | 21.141 | 21.363 | 70.156 | 0.44x |
| flat.json | strata | 0.879 | 0.911 | 0.921 | 68.523 | 1.00x |
| flat.json | orjson | 0.961 | 0.970 | 0.988 | 68.523 | 0.94x |
| flat.json | msgspec | 1.009 | 1.018 | 1.030 | 68.523 | 0.90x |
| flat.json | ujson | 1.558 | 1.573 | 1.599 | 68.523 | 0.58x |
| flat.json | json | 1.846 | 1.861 | 1.885 | 68.523 | 0.49x |
| nested.json | strata | 0.857 | 0.871 | 0.882 | 68.523 | 1.00x |
| nested.json | orjson | 0.951 | 0.977 | 0.989 | 68.523 | 0.89x |
| nested.json | msgspec | 1.089 | 1.108 | 1.124 | 68.523 | 0.79x |
| nested.json | ujson | 1.512 | 1.531 | 1.561 | 68.523 | 0.57x |
| nested.json | json | 2.052 | 2.063 | 2.085 | 68.523 | 0.42x |
| wide_arrays.json | strata | 3.937 | 3.966 | 4.019 | 70.152 | 1.00x |
| wide_arrays.json | orjson | 4.100 | 4.188 | 4.232 | 70.152 | 0.95x |
| wide_arrays.json | msgspec | 5.186 | 5.234 | 5.285 | 70.152 | 0.76x |
| wide_arrays.json | ujson | 6.712 | 6.753 | 6.834 | 70.152 | 0.59x |
| wide_arrays.json | json | 9.607 | 9.707 | 9.790 | 70.152 | 0.41x |
| mixed.json | strata | 0.224 | 0.229 | 0.254 | 70.152 | 1.00x |
| mixed.json | orjson | 0.284 | 0.287 | 0.318 | 70.152 | 0.80x |
| mixed.json | msgspec | 0.303 | 0.322 | 0.339 | 70.152 | 0.71x |
| mixed.json | ujson | 0.384 | 0.394 | 0.427 | 70.152 | 0.58x |
| mixed.json | json | 0.517 | 0.537 | 0.558 | 70.152 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.597 | 9.685 | 10.127 | 68.523 | 1.00x |
| users.ndjson | orjson | 14.919 | 15.128 | 15.593 | 68.523 | 0.64x |
| users.ndjson | msgspec | 15.415 | 15.555 | 15.909 | 68.523 | 0.62x |
| users.ndjson | ujson | 19.937 | 20.221 | 20.560 | 68.523 | 0.48x |
| users.ndjson | json | 26.075 | 26.551 | 27.072 | 68.523 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.547 | 2.607 | 2.838 | 70.156 | 1.00x |
| users.json | orjson | 3.280 | 3.367 | 3.428 | 70.156 | 0.77x |
| users.json | msgspec | 3.922 | 3.997 | 4.086 | 70.156 | 0.65x |
| users.json | ujson | 11.336 | 11.404 | 11.487 | 70.156 | 0.23x |
| users.json | json | 19.788 | 19.870 | 19.979 | 70.156 | 0.13x |
| flat.json | strata | 0.514 | 0.532 | 0.557 | 68.523 | 1.00x |
| flat.json | orjson | 0.595 | 0.622 | 0.668 | 68.523 | 0.86x |
| flat.json | msgspec | 0.685 | 0.734 | 0.795 | 68.523 | 0.72x |
| flat.json | ujson | 1.318 | 1.349 | 1.375 | 68.523 | 0.39x |
| flat.json | json | 2.034 | 2.073 | 2.136 | 68.523 | 0.26x |
| nested.json | strata | 0.453 | 0.482 | 0.504 | 68.523 | 1.00x |
| nested.json | orjson | 0.544 | 0.579 | 0.623 | 68.523 | 0.83x |
| nested.json | msgspec | 0.631 | 0.668 | 0.763 | 68.523 | 0.72x |
| nested.json | ujson | 1.404 | 1.424 | 1.504 | 68.523 | 0.34x |
| nested.json | json | 2.459 | 2.487 | 2.528 | 68.523 | 0.19x |
| wide_arrays.json | strata | 1.803 | 1.864 | 1.938 | 70.152 | 1.00x |
| wide_arrays.json | orjson | 2.110 | 2.168 | 2.259 | 70.152 | 0.86x |
| wide_arrays.json | msgspec | 2.933 | 2.976 | 3.065 | 70.152 | 0.63x |
| wide_arrays.json | ujson | 5.360 | 5.408 | 5.482 | 70.152 | 0.34x |
| wide_arrays.json | json | 14.217 | 14.289 | 14.419 | 70.152 | 0.13x |
| mixed.json | strata | 0.251 | 0.258 | 0.283 | 70.152 | 1.00x |
| mixed.json | orjson | 0.291 | 0.306 | 0.341 | 70.152 | 0.85x |
| mixed.json | msgspec | 0.305 | 0.317 | 0.338 | 70.152 | 0.81x |
| mixed.json | ujson | 0.484 | 0.507 | 0.524 | 70.152 | 0.51x |
| mixed.json | json | 0.720 | 0.743 | 0.758 | 70.152 | 0.35x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.110 | 0.111 | 0.122 | 70.156 | 1.00x |
| users.json $[*].id | jmespath | 0.488 | 0.495 | 0.502 | 70.156 | 0.22x |
| users.json $[*].id | jsonpath-ng | 2.484 | 2.517 | 2.562 | 70.156 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.658 | 0.690 | 0.709 | 70.160 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.060 | 3.097 | 3.126 | 70.160 | 0.22x |
| users.json $[*].orders[*].total | jsonpath-ng | 18.161 | 18.382 | 18.679 | 70.160 | 0.04x |
| users.json $..total | strata | 1.724 | 1.739 | 1.775 | 70.160 | 1.00x |
| users.json $..total | jsonpath-ng | 295.806 | 296.204 | 297.144 | 70.160 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.169 | 3.203 | 3.229 | 70.160 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.669 | 12.944 | 13.422 | 70.160 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 14.568 | 14.782 | 15.269 | 70.160 | 0.22x |
| users.json $[*].orders[*].total | strata | 3.389 | 3.414 | 3.449 | 70.160 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.933 | 16.176 | 16.487 | 70.160 | 0.21x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 35.077 | 35.288 | 35.649 | 70.160 | 0.10x |
| users.json $..total | strata | 11.812 | 12.349 | 14.589 | 70.160 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 314.791 | 315.843 | 322.016 | 70.160 | 0.04x |

