# Benchmark results - ci-linux-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: b32d39824b8fea15839872cab8834a1b0fef1294
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
| users.json | strata | 8.997 | 9.211 | 11.520 | 57.223 | 1.00x |
| users.json | orjson | 12.318 | 12.588 | 14.849 | 57.223 | 0.73x |
| users.json | msgspec | 12.712 | 12.888 | 14.877 | 57.223 | 0.71x |
| users.json | ujson | 17.366 | 17.829 | 20.901 | 57.223 | 0.52x |
| users.json | pysimdjson | 17.521 | 18.553 | 20.863 | 57.223 | 0.50x |
| users.json | json | 21.340 | 21.776 | 22.699 | 57.223 | 0.42x |
| flat.json | strata | 0.868 | 0.930 | 0.950 | 68.109 | 1.00x |
| flat.json | orjson | 0.933 | 0.972 | 1.039 | 68.109 | 0.96x |
| flat.json | msgspec | 0.945 | 0.971 | 1.014 | 68.109 | 0.96x |
| flat.json | ujson | 1.507 | 1.530 | 1.641 | 68.109 | 0.61x |
| flat.json | pysimdjson | 1.525 | 1.591 | 1.677 | 68.109 | 0.58x |
| flat.json | json | 1.816 | 1.853 | 1.896 | 68.109 | 0.50x |
| nested.json | strata | 0.828 | 0.853 | 0.874 | 68.109 | 1.00x |
| nested.json | orjson | 0.919 | 0.934 | 0.942 | 68.109 | 0.91x |
| nested.json | msgspec | 1.029 | 1.040 | 1.073 | 68.109 | 0.82x |
| nested.json | ujson | 1.462 | 1.513 | 1.556 | 68.109 | 0.56x |
| nested.json | pysimdjson | 1.439 | 1.477 | 1.524 | 68.109 | 0.58x |
| nested.json | json | 1.998 | 2.024 | 2.043 | 68.109 | 0.42x |
| wide_arrays.json | strata | 4.262 | 4.534 | 4.747 | 69.691 | 1.00x |
| wide_arrays.json | orjson | 4.573 | 4.965 | 5.222 | 69.691 | 0.91x |
| wide_arrays.json | msgspec | 5.646 | 5.878 | 6.116 | 69.691 | 0.77x |
| wide_arrays.json | ujson | 7.071 | 7.300 | 7.978 | 69.691 | 0.62x |
| wide_arrays.json | pysimdjson | 6.041 | 6.221 | 6.530 | 69.691 | 0.73x |
| wide_arrays.json | json | 10.189 | 10.420 | 10.923 | 69.691 | 0.44x |
| mixed.json | strata | 0.203 | 0.209 | 0.229 | 69.691 | 1.00x |
| mixed.json | orjson | 0.229 | 0.239 | 0.272 | 69.691 | 0.87x |
| mixed.json | msgspec | 0.251 | 0.260 | 0.290 | 69.691 | 0.80x |
| mixed.json | ujson | 0.335 | 0.347 | 0.374 | 69.691 | 0.60x |
| mixed.json | pysimdjson | 0.314 | 0.324 | 0.350 | 69.691 | 0.64x |
| mixed.json | json | 0.483 | 0.505 | 0.527 | 69.691 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.950 | 1.966 | 1.985 | 56.332 | 1.00x |
| users.json | orjson | 2.609 | 2.631 | 2.649 | 56.332 | 0.75x |
| users.json | msgspec | 3.340 | 3.365 | 3.389 | 56.332 | 0.58x |
| users.json | ujson | 10.595 | 10.698 | 10.748 | 56.332 | 0.18x |
| users.json | json | 19.146 | 19.203 | 19.272 | 56.332 | 0.10x |
| flat.json | strata | 0.249 | 0.273 | 0.298 | 68.109 | 1.00x |
| flat.json | orjson | 0.321 | 0.329 | 0.342 | 68.109 | 0.83x |
| flat.json | msgspec | 0.410 | 0.430 | 0.461 | 68.109 | 0.64x |
| flat.json | ujson | 1.025 | 1.066 | 1.083 | 68.109 | 0.26x |
| flat.json | json | 1.768 | 1.801 | 1.812 | 68.109 | 0.15x |
| nested.json | strata | 0.227 | 0.241 | 0.261 | 68.113 | 1.00x |
| nested.json | orjson | 0.289 | 0.293 | 0.310 | 68.113 | 0.82x |
| nested.json | msgspec | 0.384 | 0.406 | 0.430 | 68.113 | 0.59x |
| nested.json | ujson | 1.095 | 1.106 | 1.122 | 68.113 | 0.22x |
| nested.json | json | 2.189 | 2.225 | 2.243 | 68.113 | 0.11x |
| wide_arrays.json | strata | 1.439 | 1.496 | 1.517 | 69.691 | 1.00x |
| wide_arrays.json | orjson | 1.678 | 1.701 | 1.730 | 69.691 | 0.88x |
| wide_arrays.json | msgspec | 2.425 | 2.460 | 2.492 | 69.691 | 0.61x |
| wide_arrays.json | ujson | 4.930 | 4.959 | 4.997 | 69.691 | 0.30x |
| wide_arrays.json | json | 13.800 | 13.843 | 13.944 | 69.691 | 0.11x |
| mixed.json | strata | 0.071 | 0.073 | 0.096 | 69.691 | 1.00x |
| mixed.json | orjson | 0.070 | 0.074 | 0.094 | 69.691 | 0.99x |
| mixed.json | msgspec | 0.088 | 0.090 | 0.091 | 69.691 | 0.81x |
| mixed.json | ujson | 0.249 | 0.252 | 0.256 | 69.691 | 0.29x |
| mixed.json | json | 0.502 | 0.528 | 0.541 | 69.691 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.459 | 9.595 | 10.386 | 68.551 | 1.00x |
| users.json | orjson | 12.833 | 12.982 | 13.292 | 68.551 | 0.74x |
| users.json | msgspec | 13.394 | 13.575 | 13.861 | 68.551 | 0.71x |
| users.json | ujson | 18.649 | 18.959 | 19.741 | 68.551 | 0.51x |
| users.json | json | 21.956 | 22.210 | 22.662 | 68.551 | 0.43x |
| flat.json | strata | 1.000 | 1.040 | 1.045 | 68.109 | 1.00x |
| flat.json | orjson | 1.096 | 1.139 | 1.169 | 68.109 | 0.91x |
| flat.json | msgspec | 1.113 | 1.139 | 1.158 | 68.109 | 0.91x |
| flat.json | ujson | 1.721 | 1.777 | 1.802 | 68.109 | 0.59x |
| flat.json | json | 1.957 | 1.998 | 2.031 | 68.109 | 0.52x |
| nested.json | strata | 0.921 | 0.934 | 0.967 | 68.113 | 1.00x |
| nested.json | orjson | 1.053 | 1.062 | 1.081 | 68.113 | 0.88x |
| nested.json | msgspec | 1.165 | 1.192 | 1.245 | 68.113 | 0.78x |
| nested.json | ujson | 1.626 | 1.667 | 1.709 | 68.113 | 0.56x |
| nested.json | json | 2.092 | 2.131 | 2.190 | 68.113 | 0.44x |
| wide_arrays.json | strata | 4.139 | 4.270 | 4.379 | 69.691 | 1.00x |
| wide_arrays.json | orjson | 4.579 | 4.761 | 4.906 | 69.691 | 0.90x |
| wide_arrays.json | msgspec | 5.588 | 5.720 | 5.885 | 69.691 | 0.75x |
| wide_arrays.json | ujson | 7.223 | 7.314 | 7.494 | 69.691 | 0.58x |
| wide_arrays.json | json | 10.069 | 10.215 | 10.399 | 69.691 | 0.42x |
| mixed.json | strata | 0.246 | 0.258 | 0.271 | 69.691 | 1.00x |
| mixed.json | orjson | 0.316 | 0.346 | 0.363 | 69.691 | 0.74x |
| mixed.json | msgspec | 0.335 | 0.369 | 0.402 | 69.691 | 0.70x |
| mixed.json | ujson | 0.443 | 0.468 | 0.488 | 69.691 | 0.55x |
| mixed.json | json | 0.544 | 0.580 | 0.607 | 69.691 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.356 | 10.505 | 10.608 | 68.105 | 1.00x |
| users.ndjson | orjson | 16.028 | 16.368 | 16.584 | 68.105 | 0.64x |
| users.ndjson | msgspec | 16.500 | 16.633 | 17.168 | 68.105 | 0.63x |
| users.ndjson | ujson | 21.508 | 21.805 | 22.269 | 68.105 | 0.48x |
| users.ndjson | json | 27.879 | 28.033 | 28.424 | 68.105 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.584 | 2.618 | 2.679 | 68.551 | 1.00x |
| users.json | orjson | 3.269 | 3.338 | 3.421 | 68.551 | 0.78x |
| users.json | msgspec | 4.060 | 4.084 | 4.197 | 68.551 | 0.64x |
| users.json | ujson | 11.300 | 11.499 | 11.546 | 68.551 | 0.23x |
| users.json | json | 19.983 | 20.065 | 20.146 | 68.551 | 0.13x |
| flat.json | strata | 0.516 | 0.552 | 0.594 | 68.109 | 1.00x |
| flat.json | orjson | 0.609 | 0.647 | 0.707 | 68.109 | 0.85x |
| flat.json | msgspec | 0.714 | 0.756 | 0.832 | 68.109 | 0.73x |
| flat.json | ujson | 1.346 | 1.408 | 1.441 | 68.109 | 0.39x |
| flat.json | json | 2.113 | 2.146 | 2.222 | 68.109 | 0.26x |
| nested.json | strata | 0.444 | 0.476 | 0.496 | 68.113 | 1.00x |
| nested.json | orjson | 0.559 | 0.585 | 0.619 | 68.113 | 0.81x |
| nested.json | msgspec | 0.658 | 0.681 | 0.715 | 68.113 | 0.70x |
| nested.json | ujson | 1.400 | 1.433 | 1.459 | 68.113 | 0.33x |
| nested.json | json | 2.461 | 2.513 | 2.549 | 68.113 | 0.19x |
| wide_arrays.json | strata | 1.897 | 2.004 | 2.105 | 69.691 | 1.00x |
| wide_arrays.json | orjson | 2.185 | 2.281 | 2.387 | 69.691 | 0.88x |
| wide_arrays.json | msgspec | 2.957 | 3.020 | 3.192 | 69.691 | 0.66x |
| wide_arrays.json | ujson | 5.455 | 5.533 | 5.695 | 69.691 | 0.36x |
| wide_arrays.json | json | 14.429 | 14.490 | 14.624 | 69.691 | 0.14x |
| mixed.json | strata | 0.205 | 0.236 | 0.279 | 69.691 | 1.00x |
| mixed.json | orjson | 0.257 | 0.278 | 0.302 | 69.691 | 0.85x |
| mixed.json | msgspec | 0.274 | 0.293 | 0.346 | 69.691 | 0.81x |
| mixed.json | ujson | 0.462 | 0.485 | 0.527 | 69.691 | 0.49x |
| mixed.json | json | 0.712 | 0.737 | 0.776 | 69.691 | 0.32x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.118 | 0.126 | 0.137 | 68.551 | 1.00x |
| users.json $[*].id | jmespath | 0.504 | 0.520 | 0.538 | 68.551 | 0.24x |
| users.json $[*].id | jsonpath-ng | 2.546 | 2.669 | 2.721 | 68.551 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.685 | 0.710 | 0.732 | 68.676 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.097 | 3.168 | 3.283 | 68.676 | 0.22x |
| users.json $[*].orders[*].total | jsonpath-ng | 18.971 | 20.003 | 20.904 | 68.676 | 0.04x |
| users.json $..total | strata | 1.741 | 1.784 | 1.850 | 69.684 | 1.00x |
| users.json $..total | jsonpath-ng | 298.643 | 299.338 | 299.686 | 69.684 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.277 | 3.308 | 3.326 | 68.676 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.405 | 13.764 | 14.081 | 68.676 | 0.24x |
| users.json $[*].id | orjson+jsonpath-ng | 15.435 | 15.791 | 16.167 | 68.676 | 0.21x |
| users.json $[*].orders[*].total | strata | 3.475 | 3.513 | 3.525 | 69.684 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 16.427 | 16.940 | 17.335 | 69.684 | 0.21x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 37.345 | 38.044 | 38.369 | 69.684 | 0.09x |
| users.json $..total | strata | 12.695 | 13.101 | 13.601 | 69.742 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 319.102 | 320.808 | 321.398 | 69.742 | 0.04x |

