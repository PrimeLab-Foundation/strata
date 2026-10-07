# Benchmark results - ci-linux-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c16eaa65c61f1cf9d4fac46e9f6d8e6b9f3b087f
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
| users.json | strata | 9.058 | 9.155 | 11.648 | 57.270 | 1.00x |
| users.json | orjson | 12.400 | 12.769 | 14.919 | 57.270 | 0.72x |
| users.json | msgspec | 12.689 | 12.935 | 15.003 | 57.270 | 0.71x |
| users.json | ujson | 17.776 | 18.001 | 21.259 | 57.270 | 0.51x |
| users.json | pysimdjson | 17.785 | 18.122 | 20.966 | 57.270 | 0.51x |
| users.json | json | 21.442 | 21.851 | 23.402 | 57.270 | 0.42x |
| flat.json | strata | 0.838 | 0.879 | 0.903 | 67.992 | 1.00x |
| flat.json | orjson | 0.880 | 0.912 | 0.951 | 67.992 | 0.96x |
| flat.json | msgspec | 0.924 | 0.936 | 0.962 | 67.992 | 0.94x |
| flat.json | ujson | 1.474 | 1.501 | 1.546 | 67.992 | 0.59x |
| flat.json | pysimdjson | 1.484 | 1.509 | 1.562 | 67.992 | 0.58x |
| flat.json | json | 1.785 | 1.796 | 1.820 | 67.992 | 0.49x |
| nested.json | strata | 0.824 | 0.837 | 0.845 | 67.992 | 1.00x |
| nested.json | orjson | 0.888 | 0.909 | 0.919 | 67.992 | 0.92x |
| nested.json | msgspec | 1.020 | 1.024 | 1.041 | 67.992 | 0.82x |
| nested.json | ujson | 1.446 | 1.465 | 1.653 | 67.992 | 0.57x |
| nested.json | pysimdjson | 1.415 | 1.427 | 1.448 | 67.992 | 0.59x |
| nested.json | json | 1.993 | 2.013 | 2.026 | 67.992 | 0.42x |
| wide_arrays.json | strata | 3.959 | 4.028 | 4.116 | 69.562 | 1.00x |
| wide_arrays.json | orjson | 4.192 | 4.257 | 4.420 | 69.562 | 0.95x |
| wide_arrays.json | msgspec | 5.118 | 5.163 | 5.241 | 69.562 | 0.78x |
| wide_arrays.json | ujson | 6.638 | 6.681 | 6.893 | 69.562 | 0.60x |
| wide_arrays.json | pysimdjson | 5.446 | 5.545 | 5.831 | 69.562 | 0.73x |
| wide_arrays.json | json | 9.654 | 9.795 | 10.084 | 69.562 | 0.41x |
| mixed.json | strata | 0.200 | 0.204 | 0.220 | 69.562 | 1.00x |
| mixed.json | orjson | 0.224 | 0.229 | 0.248 | 69.562 | 0.89x |
| mixed.json | msgspec | 0.243 | 0.249 | 0.267 | 69.562 | 0.82x |
| mixed.json | ujson | 0.323 | 0.328 | 0.345 | 69.562 | 0.62x |
| mixed.json | pysimdjson | 0.303 | 0.312 | 0.340 | 69.562 | 0.66x |
| mixed.json | json | 0.469 | 0.483 | 0.499 | 69.562 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.949 | 1.969 | 2.003 | 56.379 | 1.00x |
| users.json | orjson | 2.606 | 2.618 | 2.640 | 56.379 | 0.75x |
| users.json | msgspec | 3.342 | 3.353 | 3.373 | 56.379 | 0.59x |
| users.json | ujson | 10.675 | 10.722 | 10.800 | 56.379 | 0.18x |
| users.json | json | 19.090 | 19.181 | 19.277 | 56.379 | 0.10x |
| flat.json | strata | 0.238 | 0.244 | 0.265 | 67.992 | 1.00x |
| flat.json | orjson | 0.308 | 0.312 | 0.328 | 67.992 | 0.78x |
| flat.json | msgspec | 0.399 | 0.403 | 0.411 | 67.992 | 0.61x |
| flat.json | ujson | 1.004 | 1.013 | 1.021 | 67.992 | 0.24x |
| flat.json | json | 1.740 | 1.753 | 1.774 | 67.992 | 0.14x |
| nested.json | strata | 0.223 | 0.228 | 0.243 | 67.992 | 1.00x |
| nested.json | orjson | 0.286 | 0.293 | 0.320 | 67.992 | 0.78x |
| nested.json | msgspec | 0.372 | 0.384 | 0.397 | 67.992 | 0.59x |
| nested.json | ujson | 1.088 | 1.091 | 1.103 | 67.992 | 0.21x |
| nested.json | json | 2.158 | 2.188 | 2.210 | 67.992 | 0.10x |
| wide_arrays.json | strata | 1.376 | 1.405 | 1.428 | 69.562 | 1.00x |
| wide_arrays.json | orjson | 1.621 | 1.653 | 1.680 | 69.562 | 0.85x |
| wide_arrays.json | msgspec | 2.383 | 2.400 | 2.410 | 69.562 | 0.59x |
| wide_arrays.json | ujson | 4.786 | 4.807 | 4.865 | 69.562 | 0.29x |
| wide_arrays.json | json | 13.665 | 13.732 | 13.857 | 69.562 | 0.10x |
| mixed.json | strata | 0.067 | 0.071 | 0.072 | 69.562 | 1.00x |
| mixed.json | orjson | 0.071 | 0.072 | 0.073 | 69.562 | 0.99x |
| mixed.json | msgspec | 0.085 | 0.086 | 0.088 | 69.562 | 0.82x |
| mixed.json | ujson | 0.245 | 0.252 | 0.269 | 69.562 | 0.28x |
| mixed.json | json | 0.499 | 0.522 | 0.535 | 69.562 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.471 | 9.586 | 10.462 | 68.426 | 1.00x |
| users.json | orjson | 12.718 | 13.237 | 13.484 | 68.426 | 0.72x |
| users.json | msgspec | 13.349 | 13.856 | 14.461 | 68.426 | 0.69x |
| users.json | ujson | 18.536 | 19.380 | 20.665 | 68.426 | 0.49x |
| users.json | json | 22.190 | 22.668 | 23.129 | 68.426 | 0.42x |
| flat.json | strata | 0.896 | 0.919 | 0.965 | 67.992 | 1.00x |
| flat.json | orjson | 0.985 | 1.000 | 1.044 | 67.992 | 0.92x |
| flat.json | msgspec | 1.018 | 1.036 | 1.074 | 67.992 | 0.89x |
| flat.json | ujson | 1.580 | 1.599 | 1.655 | 67.992 | 0.57x |
| flat.json | json | 1.848 | 1.858 | 1.899 | 67.992 | 0.49x |
| nested.json | strata | 0.879 | 0.889 | 0.906 | 67.992 | 1.00x |
| nested.json | orjson | 0.989 | 1.009 | 1.026 | 67.992 | 0.88x |
| nested.json | msgspec | 1.106 | 1.133 | 1.161 | 67.992 | 0.78x |
| nested.json | ujson | 1.545 | 1.570 | 1.618 | 67.992 | 0.57x |
| nested.json | json | 2.071 | 2.089 | 2.125 | 67.992 | 0.43x |
| wide_arrays.json | strata | 3.939 | 4.026 | 4.120 | 69.562 | 1.00x |
| wide_arrays.json | orjson | 4.267 | 4.400 | 4.514 | 69.562 | 0.91x |
| wide_arrays.json | msgspec | 5.195 | 5.303 | 5.440 | 69.562 | 0.76x |
| wide_arrays.json | ujson | 6.818 | 6.902 | 7.028 | 69.562 | 0.58x |
| wide_arrays.json | json | 9.667 | 9.874 | 10.233 | 69.562 | 0.41x |
| mixed.json | strata | 0.228 | 0.234 | 0.254 | 69.562 | 1.00x |
| mixed.json | orjson | 0.294 | 0.304 | 0.332 | 69.562 | 0.77x |
| mixed.json | msgspec | 0.316 | 0.323 | 0.347 | 69.562 | 0.73x |
| mixed.json | ujson | 0.397 | 0.411 | 0.429 | 69.562 | 0.57x |
| mixed.json | json | 0.527 | 0.543 | 0.558 | 69.562 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.895 | 10.218 | 10.477 | 67.984 | 1.00x |
| users.ndjson | orjson | 15.522 | 15.877 | 16.014 | 67.984 | 0.64x |
| users.ndjson | msgspec | 15.872 | 16.214 | 16.595 | 67.984 | 0.63x |
| users.ndjson | ujson | 21.010 | 21.359 | 22.117 | 67.984 | 0.48x |
| users.ndjson | json | 27.488 | 27.695 | 28.116 | 67.984 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.562 | 2.606 | 2.687 | 68.426 | 1.00x |
| users.json | orjson | 3.256 | 3.356 | 3.461 | 68.426 | 0.78x |
| users.json | msgspec | 4.053 | 4.074 | 4.253 | 68.426 | 0.64x |
| users.json | ujson | 11.291 | 11.470 | 11.562 | 68.426 | 0.23x |
| users.json | json | 20.027 | 20.083 | 20.203 | 68.426 | 0.13x |
| flat.json | strata | 0.414 | 0.426 | 0.434 | 67.992 | 1.00x |
| flat.json | orjson | 0.496 | 0.527 | 0.545 | 67.992 | 0.81x |
| flat.json | msgspec | 0.605 | 0.617 | 0.639 | 67.992 | 0.69x |
| flat.json | ujson | 1.234 | 1.253 | 1.262 | 67.992 | 0.34x |
| flat.json | json | 1.959 | 1.971 | 1.981 | 67.992 | 0.22x |
| nested.json | strata | 0.401 | 0.426 | 0.458 | 67.992 | 1.00x |
| nested.json | orjson | 0.499 | 0.538 | 0.558 | 67.992 | 0.79x |
| nested.json | msgspec | 0.590 | 0.616 | 0.650 | 67.992 | 0.69x |
| nested.json | ujson | 1.357 | 1.386 | 1.396 | 67.992 | 0.31x |
| nested.json | json | 2.404 | 2.449 | 2.500 | 67.992 | 0.17x |
| wide_arrays.json | strata | 1.837 | 1.915 | 1.966 | 69.562 | 1.00x |
| wide_arrays.json | orjson | 2.129 | 2.212 | 2.255 | 69.562 | 0.87x |
| wide_arrays.json | msgspec | 2.861 | 2.915 | 2.994 | 69.562 | 0.66x |
| wide_arrays.json | ujson | 5.319 | 5.381 | 5.411 | 69.562 | 0.36x |
| wide_arrays.json | json | 14.238 | 14.300 | 14.528 | 69.562 | 0.13x |
| mixed.json | strata | 0.180 | 0.194 | 0.216 | 69.562 | 1.00x |
| mixed.json | orjson | 0.200 | 0.217 | 0.256 | 69.562 | 0.89x |
| mixed.json | msgspec | 0.218 | 0.228 | 0.249 | 69.562 | 0.85x |
| mixed.json | ujson | 0.408 | 0.414 | 0.436 | 69.562 | 0.47x |
| mixed.json | json | 0.648 | 0.672 | 0.685 | 69.562 | 0.29x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.119 | 0.123 | 0.125 | 68.426 | 1.00x |
| users.json $[*].id | jmespath | 0.503 | 0.513 | 0.528 | 68.426 | 0.24x |
| users.json $[*].id | jsonpath-ng | 2.540 | 2.619 | 2.734 | 68.426 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.676 | 0.717 | 0.750 | 68.551 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.065 | 3.131 | 3.234 | 68.551 | 0.23x |
| users.json $[*].orders[*].total | jsonpath-ng | 19.117 | 19.625 | 20.393 | 68.551 | 0.04x |
| users.json $..total | strata | 1.736 | 1.764 | 1.812 | 69.559 | 1.00x |
| users.json $..total | jsonpath-ng | 298.122 | 298.580 | 298.840 | 69.559 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.230 | 3.243 | 3.264 | 68.551 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.609 | 13.784 | 14.474 | 68.551 | 0.24x |
| users.json $[*].id | orjson+jsonpath-ng | 15.529 | 15.769 | 16.082 | 68.551 | 0.21x |
| users.json $[*].orders[*].total | strata | 3.422 | 3.442 | 3.480 | 69.559 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 16.641 | 17.008 | 17.555 | 69.559 | 0.20x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 37.406 | 38.019 | 39.681 | 69.559 | 0.09x |
| users.json $..total | strata | 11.852 | 12.496 | 12.730 | 69.621 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 314.617 | 316.685 | 317.977 | 69.621 | 0.04x |

