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
| users.json | strata | 8.930 | 9.081 | 9.239 | 57.348 | 1.00x |
| users.json | orjson | 11.595 | 11.867 | 12.057 | 57.348 | 0.77x |
| users.json | msgspec | 12.089 | 12.344 | 12.488 | 57.348 | 0.74x |
| users.json | ujson | 16.299 | 16.776 | 17.198 | 57.348 | 0.54x |
| users.json | pysimdjson | 16.306 | 16.709 | 17.286 | 57.348 | 0.54x |
| users.json | json | 20.568 | 20.929 | 21.177 | 57.348 | 0.43x |
| flat.json | strata | 0.829 | 0.862 | 0.882 | 59.367 | 1.00x |
| flat.json | orjson | 0.850 | 0.882 | 0.899 | 59.367 | 0.98x |
| flat.json | msgspec | 0.894 | 0.920 | 0.933 | 59.367 | 0.94x |
| flat.json | ujson | 1.429 | 1.451 | 1.476 | 59.367 | 0.59x |
| flat.json | pysimdjson | 1.465 | 1.486 | 1.508 | 59.367 | 0.58x |
| flat.json | json | 1.763 | 1.783 | 1.804 | 59.367 | 0.48x |
| nested.json | strata | 0.829 | 0.850 | 0.864 | 59.367 | 1.00x |
| nested.json | orjson | 0.872 | 0.894 | 0.906 | 59.367 | 0.95x |
| nested.json | msgspec | 0.977 | 0.997 | 1.011 | 59.367 | 0.85x |
| nested.json | ujson | 1.414 | 1.434 | 1.457 | 59.367 | 0.59x |
| nested.json | pysimdjson | 1.399 | 1.423 | 1.439 | 59.367 | 0.60x |
| nested.json | json | 1.940 | 1.968 | 1.982 | 59.367 | 0.43x |
| wide_arrays.json | strata | 3.935 | 4.001 | 4.046 | 65.957 | 1.00x |
| wide_arrays.json | orjson | 4.043 | 4.127 | 4.172 | 65.957 | 0.97x |
| wide_arrays.json | msgspec | 5.069 | 5.121 | 5.165 | 65.957 | 0.78x |
| wide_arrays.json | ujson | 6.493 | 6.577 | 6.629 | 65.957 | 0.61x |
| wide_arrays.json | pysimdjson | 5.256 | 5.334 | 5.397 | 65.957 | 0.75x |
| wide_arrays.json | json | 9.428 | 9.528 | 9.601 | 65.957 | 0.42x |
| mixed.json | strata | 0.196 | 0.201 | 0.221 | 65.957 | 1.00x |
| mixed.json | orjson | 0.216 | 0.222 | 0.246 | 65.957 | 0.91x |
| mixed.json | msgspec | 0.233 | 0.240 | 0.266 | 65.957 | 0.84x |
| mixed.json | ujson | 0.304 | 0.314 | 0.333 | 65.957 | 0.64x |
| mixed.json | pysimdjson | 0.293 | 0.300 | 0.321 | 65.957 | 0.67x |
| mixed.json | json | 0.458 | 0.467 | 0.489 | 65.957 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.959 | 1.986 | 1.996 | 46.008 | 1.00x |
| users.json | orjson | 2.613 | 2.637 | 2.657 | 46.008 | 0.75x |
| users.json | msgspec | 3.350 | 3.373 | 3.395 | 46.008 | 0.59x |
| users.json | ujson | 10.495 | 10.556 | 10.615 | 46.008 | 0.19x |
| users.json | json | 19.190 | 19.263 | 19.379 | 46.008 | 0.10x |
| flat.json | strata | 0.238 | 0.243 | 0.261 | 59.367 | 1.00x |
| flat.json | orjson | 0.306 | 0.310 | 0.335 | 59.367 | 0.78x |
| flat.json | msgspec | 0.397 | 0.404 | 0.431 | 59.367 | 0.60x |
| flat.json | ujson | 1.000 | 1.012 | 1.020 | 59.367 | 0.24x |
| flat.json | json | 1.718 | 1.745 | 1.763 | 59.367 | 0.14x |
| nested.json | strata | 0.218 | 0.224 | 0.240 | 59.367 | 1.00x |
| nested.json | orjson | 0.284 | 0.288 | 0.312 | 59.367 | 0.78x |
| nested.json | msgspec | 0.371 | 0.379 | 0.400 | 59.367 | 0.59x |
| nested.json | ujson | 1.119 | 1.141 | 1.161 | 59.367 | 0.20x |
| nested.json | json | 2.158 | 2.190 | 2.222 | 59.367 | 0.10x |
| wide_arrays.json | strata | 1.308 | 1.324 | 1.352 | 65.957 | 1.00x |
| wide_arrays.json | orjson | 1.582 | 1.604 | 1.637 | 65.957 | 0.83x |
| wide_arrays.json | msgspec | 2.351 | 2.373 | 2.397 | 65.957 | 0.56x |
| wide_arrays.json | ujson | 4.755 | 4.786 | 4.814 | 65.957 | 0.28x |
| wide_arrays.json | json | 13.535 | 13.590 | 13.662 | 65.957 | 0.10x |
| mixed.json | strata | 0.064 | 0.068 | 0.072 | 65.957 | 1.00x |
| mixed.json | orjson | 0.066 | 0.070 | 0.072 | 65.957 | 0.98x |
| mixed.json | msgspec | 0.081 | 0.085 | 0.088 | 65.957 | 0.80x |
| mixed.json | ujson | 0.237 | 0.245 | 0.265 | 65.957 | 0.28x |
| mixed.json | json | 0.482 | 0.501 | 0.521 | 65.957 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.102 | 9.319 | 9.564 | 60.734 | 1.00x |
| users.json | orjson | 11.723 | 12.141 | 12.475 | 60.734 | 0.77x |
| users.json | msgspec | 12.350 | 12.665 | 12.846 | 60.734 | 0.74x |
| users.json | ujson | 17.019 | 17.455 | 17.795 | 60.734 | 0.53x |
| users.json | json | 20.874 | 21.300 | 21.611 | 60.734 | 0.44x |
| flat.json | strata | 0.872 | 0.907 | 0.923 | 59.367 | 1.00x |
| flat.json | orjson | 0.936 | 0.967 | 0.993 | 59.367 | 0.94x |
| flat.json | msgspec | 0.977 | 1.007 | 1.028 | 59.367 | 0.90x |
| flat.json | ujson | 1.549 | 1.576 | 1.602 | 59.367 | 0.58x |
| flat.json | json | 1.838 | 1.855 | 1.876 | 59.367 | 0.49x |
| nested.json | strata | 0.863 | 0.886 | 0.909 | 59.367 | 1.00x |
| nested.json | orjson | 0.945 | 0.973 | 0.993 | 59.367 | 0.91x |
| nested.json | msgspec | 1.062 | 1.076 | 1.096 | 59.367 | 0.82x |
| nested.json | ujson | 1.493 | 1.523 | 1.559 | 59.367 | 0.58x |
| nested.json | json | 2.009 | 2.028 | 2.054 | 59.367 | 0.44x |
| wide_arrays.json | strata | 3.978 | 4.037 | 4.123 | 65.957 | 1.00x |
| wide_arrays.json | orjson | 4.089 | 4.232 | 4.343 | 65.957 | 0.95x |
| wide_arrays.json | msgspec | 5.177 | 5.242 | 5.316 | 65.957 | 0.77x |
| wide_arrays.json | ujson | 6.721 | 6.844 | 6.950 | 65.957 | 0.59x |
| wide_arrays.json | json | 9.582 | 9.705 | 9.846 | 65.957 | 0.42x |
| mixed.json | strata | 0.222 | 0.229 | 0.253 | 65.957 | 1.00x |
| mixed.json | orjson | 0.287 | 0.295 | 0.322 | 65.957 | 0.78x |
| mixed.json | msgspec | 0.302 | 0.311 | 0.333 | 65.957 | 0.74x |
| mixed.json | ujson | 0.385 | 0.401 | 0.427 | 65.957 | 0.57x |
| mixed.json | json | 0.519 | 0.538 | 0.554 | 65.957 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.530 | 9.838 | 10.175 | 59.359 | 1.00x |
| users.ndjson | orjson | 14.777 | 15.163 | 15.405 | 59.359 | 0.65x |
| users.ndjson | msgspec | 15.116 | 15.409 | 15.579 | 59.359 | 0.64x |
| users.ndjson | ujson | 19.722 | 20.189 | 20.585 | 59.359 | 0.49x |
| users.ndjson | json | 25.721 | 26.701 | 27.141 | 59.359 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.750 | 2.835 | 2.973 | 60.734 | 1.00x |
| users.json | orjson | 3.417 | 3.509 | 3.602 | 60.734 | 0.81x |
| users.json | msgspec | 4.109 | 4.267 | 4.378 | 60.734 | 0.66x |
| users.json | ujson | 11.582 | 11.707 | 12.761 | 60.734 | 0.24x |
| users.json | json | 20.206 | 20.557 | 20.815 | 60.734 | 0.14x |
| flat.json | strata | 0.658 | 0.707 | 0.869 | 59.367 | 1.00x |
| flat.json | orjson | 0.752 | 0.808 | 0.963 | 59.367 | 0.87x |
| flat.json | msgspec | 0.845 | 0.916 | 0.997 | 59.367 | 0.77x |
| flat.json | ujson | 1.495 | 1.539 | 1.839 | 59.367 | 0.46x |
| flat.json | json | 2.203 | 2.270 | 2.349 | 59.367 | 0.31x |
| nested.json | strata | 0.595 | 0.654 | 0.749 | 59.367 | 1.00x |
| nested.json | orjson | 0.715 | 0.774 | 0.854 | 59.367 | 0.84x |
| nested.json | msgspec | 0.819 | 0.864 | 0.918 | 59.367 | 0.76x |
| nested.json | ujson | 1.540 | 1.622 | 1.803 | 59.367 | 0.40x |
| nested.json | json | 2.602 | 2.681 | 2.840 | 59.367 | 0.24x |
| wide_arrays.json | strata | 1.981 | 2.080 | 2.266 | 65.957 | 1.00x |
| wide_arrays.json | orjson | 2.278 | 2.375 | 2.527 | 65.957 | 0.88x |
| wide_arrays.json | msgspec | 3.056 | 3.148 | 3.292 | 65.957 | 0.66x |
| wide_arrays.json | ujson | 5.535 | 5.644 | 5.977 | 65.957 | 0.37x |
| wide_arrays.json | json | 14.353 | 14.536 | 14.780 | 65.957 | 0.14x |
| mixed.json | strata | 0.408 | 0.475 | 0.666 | 65.957 | 1.00x |
| mixed.json | orjson | 0.463 | 0.517 | 0.645 | 65.957 | 0.92x |
| mixed.json | msgspec | 0.474 | 0.549 | 0.613 | 65.957 | 0.86x |
| mixed.json | ujson | 0.635 | 0.736 | 0.822 | 65.957 | 0.64x |
| mixed.json | json | 0.900 | 0.977 | 1.092 | 65.957 | 0.49x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.108 | 0.111 | 0.124 | 60.734 | 1.00x |
| users.json $[*].id | jmespath | 0.479 | 0.493 | 0.507 | 60.734 | 0.23x |
| users.json $[*].id | jsonpath-ng | 2.503 | 2.600 | 2.666 | 60.734 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.637 | 0.657 | 0.677 | 60.852 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.039 | 3.081 | 3.122 | 60.852 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 18.233 | 19.072 | 19.573 | 60.852 | 0.03x |
| users.json $..total | strata | 1.735 | 1.756 | 1.777 | 60.992 | 1.00x |
| users.json $..total | jsonpath-ng | 297.259 | 298.075 | 298.625 | 60.992 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.183 | 3.230 | 3.561 | 60.852 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.604 | 13.121 | 13.588 | 60.852 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 14.632 | 14.950 | 15.512 | 60.852 | 0.22x |
| users.json $[*].orders[*].total | strata | 3.365 | 3.401 | 3.705 | 60.992 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.468 | 16.117 | 16.368 | 60.992 | 0.21x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 34.688 | 35.487 | 36.148 | 60.992 | 0.10x |
| users.json $..total | strata | 11.873 | 12.518 | 12.902 | 60.996 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 316.258 | 318.217 | 319.125 | 60.996 | 0.04x |

