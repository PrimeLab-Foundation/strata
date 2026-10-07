# Benchmark results - ci-linux-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: ec0411225b6d58a1df905844c946a766d3c39f0a
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
| users.json | strata | 9.057 | 9.200 | 11.148 | 57.227 | 1.00x |
| users.json | orjson | 11.947 | 12.418 | 13.900 | 57.227 | 0.74x |
| users.json | msgspec | 12.219 | 12.639 | 14.269 | 57.227 | 0.73x |
| users.json | ujson | 17.258 | 17.722 | 19.990 | 57.227 | 0.52x |
| users.json | pysimdjson | 17.330 | 17.860 | 20.063 | 57.227 | 0.52x |
| users.json | json | 21.247 | 21.450 | 22.083 | 57.227 | 0.43x |
| flat.json | strata | 0.843 | 0.865 | 0.869 | 68.117 | 1.00x |
| flat.json | orjson | 0.877 | 0.898 | 0.916 | 68.117 | 0.96x |
| flat.json | msgspec | 0.931 | 0.945 | 0.954 | 68.117 | 0.91x |
| flat.json | ujson | 1.484 | 1.509 | 1.540 | 68.117 | 0.57x |
| flat.json | pysimdjson | 1.502 | 1.531 | 1.562 | 68.117 | 0.56x |
| flat.json | json | 1.813 | 1.818 | 1.848 | 68.117 | 0.48x |
| nested.json | strata | 0.808 | 0.828 | 0.838 | 68.117 | 1.00x |
| nested.json | orjson | 0.886 | 0.894 | 0.903 | 68.117 | 0.93x |
| nested.json | msgspec | 0.992 | 0.996 | 1.014 | 68.117 | 0.83x |
| nested.json | ujson | 1.415 | 1.440 | 1.462 | 68.117 | 0.58x |
| nested.json | pysimdjson | 1.403 | 1.417 | 1.437 | 68.117 | 0.58x |
| nested.json | json | 1.980 | 1.989 | 2.018 | 68.117 | 0.42x |
| wide_arrays.json | strata | 4.075 | 4.168 | 4.253 | 69.688 | 1.00x |
| wide_arrays.json | orjson | 4.403 | 4.506 | 4.569 | 69.688 | 0.93x |
| wide_arrays.json | msgspec | 5.320 | 5.384 | 5.506 | 69.688 | 0.77x |
| wide_arrays.json | ujson | 6.772 | 6.886 | 7.103 | 69.688 | 0.61x |
| wide_arrays.json | pysimdjson | 5.635 | 5.742 | 5.855 | 69.688 | 0.73x |
| wide_arrays.json | json | 9.983 | 10.044 | 10.127 | 69.688 | 0.41x |
| mixed.json | strata | 0.202 | 0.209 | 0.237 | 69.688 | 1.00x |
| mixed.json | orjson | 0.227 | 0.232 | 0.255 | 69.688 | 0.90x |
| mixed.json | msgspec | 0.246 | 0.255 | 0.280 | 69.688 | 0.82x |
| mixed.json | ujson | 0.326 | 0.343 | 0.367 | 69.688 | 0.61x |
| mixed.json | pysimdjson | 0.308 | 0.319 | 0.336 | 69.688 | 0.65x |
| mixed.json | json | 0.476 | 0.492 | 0.514 | 69.688 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.945 | 1.957 | 1.990 | 56.332 | 1.00x |
| users.json | orjson | 2.588 | 2.605 | 2.641 | 56.332 | 0.75x |
| users.json | msgspec | 3.320 | 3.347 | 3.381 | 56.332 | 0.58x |
| users.json | ujson | 10.631 | 10.679 | 10.783 | 56.332 | 0.18x |
| users.json | json | 19.136 | 19.157 | 19.273 | 56.332 | 0.10x |
| flat.json | strata | 0.244 | 0.248 | 0.267 | 68.117 | 1.00x |
| flat.json | orjson | 0.311 | 0.314 | 0.332 | 68.117 | 0.79x |
| flat.json | msgspec | 0.405 | 0.409 | 0.430 | 68.117 | 0.61x |
| flat.json | ujson | 1.012 | 1.020 | 1.029 | 68.117 | 0.24x |
| flat.json | json | 1.748 | 1.760 | 1.811 | 68.117 | 0.14x |
| nested.json | strata | 0.221 | 0.226 | 0.247 | 68.117 | 1.00x |
| nested.json | orjson | 0.284 | 0.286 | 0.371 | 68.117 | 0.79x |
| nested.json | msgspec | 0.372 | 0.375 | 0.401 | 68.117 | 0.60x |
| nested.json | ujson | 1.092 | 1.099 | 1.116 | 68.117 | 0.21x |
| nested.json | json | 2.168 | 2.179 | 2.231 | 68.117 | 0.10x |
| wide_arrays.json | strata | 1.439 | 1.459 | 1.504 | 69.688 | 1.00x |
| wide_arrays.json | orjson | 1.674 | 1.685 | 1.728 | 69.688 | 0.87x |
| wide_arrays.json | msgspec | 2.399 | 2.425 | 2.447 | 69.688 | 0.60x |
| wide_arrays.json | ujson | 4.859 | 4.894 | 4.937 | 69.688 | 0.30x |
| wide_arrays.json | json | 13.763 | 13.845 | 13.872 | 69.688 | 0.11x |
| mixed.json | strata | 0.067 | 0.071 | 0.087 | 69.688 | 1.00x |
| mixed.json | orjson | 0.070 | 0.072 | 0.087 | 69.688 | 0.99x |
| mixed.json | msgspec | 0.086 | 0.089 | 0.112 | 69.688 | 0.80x |
| mixed.json | ujson | 0.252 | 0.254 | 0.273 | 69.688 | 0.28x |
| mixed.json | json | 0.507 | 0.517 | 0.534 | 69.688 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.469 | 9.575 | 10.394 | 68.551 | 1.00x |
| users.json | orjson | 12.441 | 12.695 | 13.278 | 68.551 | 0.75x |
| users.json | msgspec | 12.996 | 13.313 | 13.995 | 68.551 | 0.72x |
| users.json | ujson | 18.483 | 18.696 | 19.940 | 68.551 | 0.51x |
| users.json | json | 21.782 | 21.922 | 22.387 | 68.551 | 0.44x |
| flat.json | strata | 0.921 | 0.948 | 0.959 | 68.117 | 1.00x |
| flat.json | orjson | 0.985 | 1.028 | 1.060 | 68.117 | 0.92x |
| flat.json | msgspec | 1.031 | 1.070 | 1.097 | 68.117 | 0.89x |
| flat.json | ujson | 1.617 | 1.655 | 1.698 | 68.117 | 0.57x |
| flat.json | json | 1.893 | 1.926 | 1.984 | 68.117 | 0.49x |
| nested.json | strata | 0.889 | 0.898 | 0.927 | 68.117 | 1.00x |
| nested.json | orjson | 1.000 | 1.020 | 1.043 | 68.117 | 0.88x |
| nested.json | msgspec | 1.116 | 1.126 | 1.159 | 68.117 | 0.80x |
| nested.json | ujson | 1.547 | 1.583 | 1.612 | 68.117 | 0.57x |
| nested.json | json | 2.086 | 2.098 | 2.135 | 68.117 | 0.43x |
| wide_arrays.json | strata | 4.068 | 4.175 | 4.251 | 69.688 | 1.00x |
| wide_arrays.json | orjson | 4.432 | 4.578 | 4.701 | 69.688 | 0.91x |
| wide_arrays.json | msgspec | 5.316 | 5.590 | 5.711 | 69.688 | 0.75x |
| wide_arrays.json | ujson | 7.108 | 7.195 | 7.353 | 69.688 | 0.58x |
| wide_arrays.json | json | 10.006 | 10.107 | 10.225 | 69.688 | 0.41x |
| mixed.json | strata | 0.251 | 0.271 | 0.296 | 69.688 | 1.00x |
| mixed.json | orjson | 0.331 | 0.350 | 0.385 | 69.688 | 0.78x |
| mixed.json | msgspec | 0.348 | 0.363 | 0.395 | 69.688 | 0.75x |
| mixed.json | ujson | 0.436 | 0.463 | 0.503 | 69.688 | 0.59x |
| mixed.json | json | 0.561 | 0.590 | 0.614 | 69.688 | 0.46x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.249 | 10.372 | 10.484 | 68.109 | 1.00x |
| users.ndjson | orjson | 15.798 | 15.904 | 16.017 | 68.109 | 0.65x |
| users.ndjson | msgspec | 16.069 | 16.217 | 16.575 | 68.109 | 0.64x |
| users.ndjson | ujson | 20.887 | 21.305 | 21.390 | 68.109 | 0.49x |
| users.ndjson | json | 27.456 | 27.604 | 27.927 | 68.109 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.648 | 2.685 | 2.730 | 68.551 | 1.00x |
| users.json | orjson | 3.392 | 3.430 | 3.535 | 68.551 | 0.78x |
| users.json | msgspec | 4.127 | 4.180 | 4.250 | 68.551 | 0.64x |
| users.json | ujson | 11.524 | 11.565 | 11.594 | 68.551 | 0.23x |
| users.json | json | 20.067 | 20.132 | 20.182 | 68.551 | 0.13x |
| flat.json | strata | 0.500 | 0.518 | 0.612 | 68.117 | 1.00x |
| flat.json | orjson | 0.576 | 0.603 | 0.677 | 68.117 | 0.86x |
| flat.json | msgspec | 0.684 | 0.714 | 0.754 | 68.117 | 0.73x |
| flat.json | ujson | 1.299 | 1.341 | 1.437 | 68.117 | 0.39x |
| flat.json | json | 2.007 | 2.068 | 2.138 | 68.117 | 0.25x |
| nested.json | strata | 0.452 | 0.475 | 0.509 | 68.117 | 1.00x |
| nested.json | orjson | 0.554 | 0.580 | 0.623 | 68.117 | 0.82x |
| nested.json | msgspec | 0.636 | 0.670 | 0.735 | 68.117 | 0.71x |
| nested.json | ujson | 1.382 | 1.416 | 1.471 | 68.117 | 0.34x |
| nested.json | json | 2.456 | 2.523 | 2.579 | 68.117 | 0.19x |
| wide_arrays.json | strata | 2.032 | 2.083 | 2.193 | 69.688 | 1.00x |
| wide_arrays.json | orjson | 2.214 | 2.307 | 2.394 | 69.688 | 0.90x |
| wide_arrays.json | msgspec | 2.962 | 3.088 | 3.193 | 69.688 | 0.67x |
| wide_arrays.json | ujson | 5.578 | 5.652 | 5.736 | 69.688 | 0.37x |
| wide_arrays.json | json | 14.559 | 14.611 | 14.718 | 69.688 | 0.14x |
| mixed.json | strata | 0.251 | 0.274 | 0.303 | 69.688 | 1.00x |
| mixed.json | orjson | 0.292 | 0.308 | 0.357 | 69.688 | 0.89x |
| mixed.json | msgspec | 0.298 | 0.344 | 0.377 | 69.688 | 0.80x |
| mixed.json | ujson | 0.489 | 0.513 | 0.545 | 69.688 | 0.53x |
| mixed.json | json | 0.733 | 0.777 | 0.818 | 69.688 | 0.35x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.113 | 0.117 | 0.147 | 68.551 | 1.00x |
| users.json $[*].id | jmespath | 0.492 | 0.508 | 0.514 | 68.551 | 0.23x |
| users.json $[*].id | jsonpath-ng | 2.520 | 2.621 | 2.724 | 68.551 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.633 | 0.662 | 0.679 | 68.676 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.056 | 3.081 | 3.177 | 68.676 | 0.22x |
| users.json $[*].orders[*].total | jsonpath-ng | 19.486 | 20.178 | 20.612 | 68.676 | 0.03x |
| users.json $..total | strata | 1.712 | 1.733 | 1.762 | 69.684 | 1.00x |
| users.json $..total | jsonpath-ng | 294.680 | 295.118 | 295.757 | 69.684 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.257 | 3.285 | 3.302 | 68.676 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.242 | 13.570 | 13.820 | 68.676 | 0.24x |
| users.json $[*].id | orjson+jsonpath-ng | 15.193 | 15.604 | 15.742 | 68.676 | 0.21x |
| users.json $[*].orders[*].total | strata | 3.473 | 3.502 | 3.579 | 69.684 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 16.320 | 16.781 | 17.137 | 69.684 | 0.21x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 36.959 | 37.658 | 38.503 | 69.684 | 0.09x |
| users.json $..total | strata | 12.204 | 12.663 | 13.114 | 69.746 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 314.979 | 317.068 | 318.665 | 69.746 | 0.04x |

