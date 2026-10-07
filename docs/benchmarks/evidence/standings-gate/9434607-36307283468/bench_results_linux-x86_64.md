# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 943460734d2b79bd16c949d8d97f4d22cc202e84
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
| users.json | strata | 9.758 | 10.925 | 15.525 | 68.645 | 1.00x |
| users.json | orjson | 13.273 | 14.388 | 17.885 | 68.645 | 0.76x |
| users.json | msgspec | 13.664 | 14.189 | 16.804 | 68.645 | 0.77x |
| users.json | ujson | 19.217 | 20.571 | 23.160 | 68.645 | 0.53x |
| users.json | pysimdjson | 18.918 | 21.790 | 23.767 | 68.645 | 0.50x |
| users.json | json | 22.233 | 23.112 | 24.424 | 68.645 | 0.47x |
| flat.json | strata | 0.835 | 0.864 | 0.983 | 66.910 | 1.00x |
| flat.json | orjson | 0.974 | 0.988 | 1.047 | 66.910 | 0.87x |
| flat.json | msgspec | 1.030 | 1.050 | 1.108 | 66.910 | 0.82x |
| flat.json | ujson | 1.451 | 1.595 | 1.733 | 66.910 | 0.54x |
| flat.json | pysimdjson | 1.512 | 1.557 | 1.687 | 66.910 | 0.55x |
| flat.json | json | 1.836 | 1.860 | 1.904 | 66.910 | 0.46x |
| nested.json | strata | 0.810 | 0.825 | 0.852 | 66.941 | 1.00x |
| nested.json | orjson | 1.016 | 1.023 | 1.034 | 66.941 | 0.81x |
| nested.json | msgspec | 1.053 | 1.059 | 1.069 | 66.941 | 0.78x |
| nested.json | ujson | 1.468 | 1.500 | 2.891 | 66.941 | 0.55x |
| nested.json | pysimdjson | 1.419 | 1.429 | 1.478 | 66.941 | 0.58x |
| nested.json | json | 2.061 | 2.074 | 2.141 | 66.941 | 0.40x |
| wide_arrays.json | strata | 4.108 | 4.187 | 4.614 | 78.941 | 1.00x |
| wide_arrays.json | orjson | 5.193 | 5.317 | 5.806 | 78.941 | 0.79x |
| wide_arrays.json | msgspec | 5.722 | 5.801 | 5.947 | 78.941 | 0.72x |
| wide_arrays.json | ujson | 7.176 | 7.257 | 7.450 | 78.941 | 0.58x |
| wide_arrays.json | pysimdjson | 6.183 | 6.307 | 6.907 | 78.941 | 0.66x |
| wide_arrays.json | json | 9.831 | 10.083 | 10.769 | 78.941 | 0.42x |
| mixed.json | strata | 0.190 | 0.192 | 0.197 | 78.941 | 1.00x |
| mixed.json | orjson | 0.227 | 0.230 | 0.247 | 78.941 | 0.84x |
| mixed.json | msgspec | 0.238 | 0.241 | 0.251 | 78.941 | 0.80x |
| mixed.json | ujson | 0.299 | 0.302 | 0.328 | 78.941 | 0.64x |
| mixed.json | pysimdjson | 0.298 | 0.300 | 0.324 | 78.941 | 0.64x |
| mixed.json | json | 0.468 | 0.484 | 0.498 | 78.941 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.339 | 2.371 | 2.494 | 47.211 | 1.00x |
| users.json | orjson | 2.887 | 2.924 | 3.013 | 47.211 | 0.81x |
| users.json | msgspec | 3.846 | 3.865 | 3.952 | 47.211 | 0.61x |
| users.json | ujson | 11.324 | 11.389 | 11.633 | 47.211 | 0.21x |
| users.json | json | 22.231 | 22.425 | 22.851 | 47.211 | 0.11x |
| flat.json | strata | 0.274 | 0.278 | 0.293 | 66.941 | 1.00x |
| flat.json | orjson | 0.329 | 0.333 | 0.380 | 66.941 | 0.83x |
| flat.json | msgspec | 0.430 | 0.436 | 0.457 | 66.941 | 0.64x |
| flat.json | ujson | 1.009 | 1.015 | 1.037 | 66.941 | 0.27x |
| flat.json | json | 1.926 | 1.943 | 1.968 | 66.941 | 0.14x |
| nested.json | strata | 0.230 | 0.232 | 0.250 | 66.941 | 1.00x |
| nested.json | orjson | 0.294 | 0.295 | 0.313 | 66.941 | 0.79x |
| nested.json | msgspec | 0.409 | 0.412 | 0.444 | 66.941 | 0.56x |
| nested.json | ujson | 1.071 | 1.082 | 1.124 | 66.941 | 0.21x |
| nested.json | json | 2.427 | 2.436 | 2.505 | 66.941 | 0.10x |
| wide_arrays.json | strata | 1.673 | 1.679 | 1.706 | 78.941 | 1.00x |
| wide_arrays.json | orjson | 1.815 | 1.828 | 1.846 | 78.941 | 0.92x |
| wide_arrays.json | msgspec | 2.764 | 2.785 | 2.818 | 78.941 | 0.60x |
| wide_arrays.json | ujson | 6.370 | 6.402 | 6.564 | 78.941 | 0.26x |
| wide_arrays.json | json | 16.690 | 16.820 | 17.290 | 78.941 | 0.10x |
| mixed.json | strata | 0.060 | 0.062 | 0.074 | 78.941 | 1.00x |
| mixed.json | orjson | 0.064 | 0.066 | 0.073 | 78.941 | 0.93x |
| mixed.json | msgspec | 0.084 | 0.087 | 0.092 | 78.941 | 0.71x |
| mixed.json | ujson | 0.230 | 0.237 | 0.250 | 78.941 | 0.26x |
| mixed.json | json | 0.518 | 0.524 | 0.545 | 78.941 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.471 | 11.190 | 12.438 | 65.902 | 1.00x |
| users.json | orjson | 13.589 | 14.122 | 20.516 | 65.902 | 0.79x |
| users.json | msgspec | 13.564 | 14.247 | 14.569 | 65.902 | 0.79x |
| users.json | ujson | 18.997 | 20.400 | 22.372 | 65.902 | 0.55x |
| users.json | json | 22.654 | 23.005 | 23.758 | 65.902 | 0.49x |
| flat.json | strata | 0.854 | 0.865 | 0.916 | 66.941 | 1.00x |
| flat.json | orjson | 1.032 | 1.044 | 1.068 | 66.941 | 0.83x |
| flat.json | msgspec | 1.086 | 1.103 | 1.142 | 66.941 | 0.78x |
| flat.json | ujson | 1.549 | 1.564 | 1.719 | 66.941 | 0.55x |
| flat.json | json | 1.897 | 1.965 | 2.110 | 66.941 | 0.44x |
| nested.json | strata | 0.834 | 0.860 | 0.957 | 66.941 | 1.00x |
| nested.json | orjson | 1.071 | 1.089 | 1.177 | 66.941 | 0.79x |
| nested.json | msgspec | 1.092 | 1.120 | 1.167 | 66.941 | 0.77x |
| nested.json | ujson | 1.521 | 1.560 | 1.661 | 66.941 | 0.55x |
| nested.json | json | 2.108 | 2.124 | 2.373 | 66.941 | 0.40x |
| wide_arrays.json | strata | 4.198 | 4.251 | 4.400 | 78.941 | 1.00x |
| wide_arrays.json | orjson | 5.144 | 5.344 | 5.486 | 78.941 | 0.80x |
| wide_arrays.json | msgspec | 5.819 | 5.962 | 6.182 | 78.941 | 0.71x |
| wide_arrays.json | ujson | 7.267 | 7.455 | 7.818 | 78.941 | 0.57x |
| wide_arrays.json | json | 9.853 | 9.973 | 10.170 | 78.941 | 0.43x |
| mixed.json | strata | 0.208 | 0.210 | 0.213 | 78.941 | 1.00x |
| mixed.json | orjson | 0.279 | 0.294 | 0.302 | 78.941 | 0.71x |
| mixed.json | msgspec | 0.284 | 0.286 | 0.299 | 78.941 | 0.73x |
| mixed.json | ujson | 0.358 | 0.374 | 0.382 | 78.941 | 0.56x |
| mixed.json | json | 0.527 | 0.534 | 0.574 | 78.941 | 0.39x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.462 | 11.861 | 15.402 | 66.910 | 1.00x |
| users.ndjson | orjson | 17.193 | 18.256 | 20.094 | 66.910 | 0.65x |
| users.ndjson | msgspec | 18.064 | 19.043 | 19.825 | 66.910 | 0.62x |
| users.ndjson | ujson | 22.900 | 24.450 | 26.619 | 66.910 | 0.49x |
| users.ndjson | json | 30.381 | 31.208 | 33.175 | 66.910 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.059 | 3.187 | 3.318 | 65.902 | 1.00x |
| users.json | orjson | 3.749 | 3.898 | 4.117 | 65.902 | 0.82x |
| users.json | msgspec | 4.570 | 4.724 | 4.990 | 65.902 | 0.67x |
| users.json | ujson | 12.409 | 12.499 | 12.875 | 65.902 | 0.25x |
| users.json | json | 22.887 | 23.190 | 24.517 | 65.902 | 0.14x |
| flat.json | strata | 0.488 | 0.550 | 0.790 | 66.941 | 1.00x |
| flat.json | orjson | 0.540 | 0.610 | 1.050 | 66.941 | 0.90x |
| flat.json | msgspec | 0.657 | 0.778 | 2.273 | 66.941 | 0.71x |
| flat.json | ujson | 1.256 | 1.344 | 1.448 | 66.941 | 0.41x |
| flat.json | json | 2.194 | 2.234 | 4.728 | 66.941 | 0.25x |
| nested.json | strata | 0.438 | 0.490 | 0.651 | 66.941 | 1.00x |
| nested.json | orjson | 0.508 | 0.656 | 0.891 | 66.941 | 0.75x |
| nested.json | msgspec | 0.620 | 0.681 | 0.979 | 66.941 | 0.72x |
| nested.json | ujson | 1.336 | 1.452 | 7.568 | 66.941 | 0.34x |
| nested.json | json | 2.686 | 2.865 | 3.029 | 66.941 | 0.17x |
| wide_arrays.json | strata | 2.210 | 2.290 | 4.214 | 78.941 | 1.00x |
| wide_arrays.json | orjson | 2.414 | 2.519 | 2.867 | 78.941 | 0.91x |
| wide_arrays.json | msgspec | 3.353 | 3.457 | 3.880 | 78.941 | 0.66x |
| wide_arrays.json | ujson | 7.044 | 7.157 | 7.514 | 78.941 | 0.32x |
| wide_arrays.json | json | 17.462 | 17.957 | 18.360 | 78.941 | 0.13x |
| mixed.json | strata | 0.205 | 0.283 | 0.529 | 78.941 | 1.00x |
| mixed.json | orjson | 0.235 | 0.269 | 0.832 | 78.941 | 1.05x |
| mixed.json | msgspec | 0.260 | 0.354 | 0.395 | 78.941 | 0.80x |
| mixed.json | ujson | 0.405 | 0.458 | 2.464 | 78.941 | 0.62x |
| mixed.json | json | 0.730 | 0.781 | 0.911 | 78.941 | 0.36x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.068 | 0.071 | 0.081 | 65.902 | 1.00x |
| users.json $[*].id | jmespath | 0.495 | 0.509 | 0.539 | 65.902 | 0.14x |
| users.json $[*].id | jsonpath-ng | 2.955 | 3.066 | 3.160 | 65.902 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.451 | 0.458 | 0.488 | 65.906 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.082 | 3.108 | 3.283 | 65.906 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 19.467 | 20.013 | 20.656 | 65.906 | 0.02x |
| users.json $..total | strata | 1.666 | 1.694 | 1.830 | 68.328 | 1.00x |
| users.json $..total | jsonpath-ng | 383.055 | 384.597 | 386.940 | 68.328 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.272 | 3.315 | 3.554 | 65.906 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.672 | 14.994 | 15.773 | 65.906 | 0.22x |
| users.json $[*].id | orjson+jsonpath-ng | 17.087 | 17.457 | 17.767 | 65.906 | 0.19x |
| users.json $[*].orders[*].total | strata | 3.560 | 3.589 | 3.615 | 65.906 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.925 | 18.117 | 19.056 | 65.906 | 0.20x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 37.404 | 39.375 | 40.405 | 65.906 | 0.09x |
| users.json $..total | strata | 14.098 | 15.279 | 19.050 | 68.328 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 406.079 | 410.113 | 412.729 | 68.328 | 0.04x |

