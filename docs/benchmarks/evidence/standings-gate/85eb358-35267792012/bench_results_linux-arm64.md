# Benchmark results - ci-linux-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 85eb3583e98587844415f5d32f85a61cbedfcf49
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
| users.json | strata | 9.191 | 9.419 | 11.369 | 57.211 | 1.00x |
| users.json | orjson | 12.743 | 12.877 | 14.225 | 57.211 | 0.73x |
| users.json | msgspec | 13.070 | 13.261 | 14.452 | 57.211 | 0.71x |
| users.json | ujson | 18.021 | 18.505 | 20.681 | 57.211 | 0.51x |
| users.json | pysimdjson | 18.200 | 18.670 | 19.969 | 57.211 | 0.50x |
| users.json | json | 21.827 | 22.083 | 22.721 | 57.211 | 0.43x |
| flat.json | strata | 0.859 | 0.879 | 0.908 | 68.000 | 1.00x |
| flat.json | orjson | 0.901 | 0.941 | 0.962 | 68.000 | 0.93x |
| flat.json | msgspec | 0.942 | 0.968 | 0.978 | 68.000 | 0.91x |
| flat.json | ujson | 1.509 | 1.542 | 1.576 | 68.000 | 0.57x |
| flat.json | pysimdjson | 1.531 | 1.558 | 1.599 | 68.000 | 0.56x |
| flat.json | json | 1.805 | 1.825 | 1.832 | 68.000 | 0.48x |
| nested.json | strata | 0.839 | 0.861 | 0.896 | 68.000 | 1.00x |
| nested.json | orjson | 0.912 | 0.927 | 0.951 | 68.000 | 0.93x |
| nested.json | msgspec | 1.012 | 1.042 | 1.056 | 68.000 | 0.83x |
| nested.json | ujson | 1.458 | 1.476 | 1.512 | 68.000 | 0.58x |
| nested.json | pysimdjson | 1.426 | 1.458 | 1.493 | 68.000 | 0.59x |
| nested.json | json | 2.029 | 2.040 | 2.099 | 68.000 | 0.42x |
| wide_arrays.json | strata | 4.283 | 4.398 | 4.439 | 69.570 | 1.00x |
| wide_arrays.json | orjson | 4.612 | 4.812 | 4.924 | 69.570 | 0.91x |
| wide_arrays.json | msgspec | 5.552 | 5.760 | 5.923 | 69.570 | 0.76x |
| wide_arrays.json | ujson | 7.124 | 7.212 | 7.285 | 69.570 | 0.61x |
| wide_arrays.json | pysimdjson | 5.848 | 6.056 | 6.240 | 69.570 | 0.73x |
| wide_arrays.json | json | 10.127 | 10.297 | 10.360 | 69.570 | 0.43x |
| mixed.json | strata | 0.208 | 0.214 | 0.232 | 69.570 | 1.00x |
| mixed.json | orjson | 0.234 | 0.240 | 0.260 | 69.570 | 0.89x |
| mixed.json | msgspec | 0.253 | 0.257 | 0.284 | 69.570 | 0.83x |
| mixed.json | ujson | 0.334 | 0.341 | 0.356 | 69.570 | 0.63x |
| mixed.json | pysimdjson | 0.317 | 0.339 | 0.351 | 69.570 | 0.63x |
| mixed.json | json | 0.490 | 0.506 | 0.521 | 69.570 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.945 | 1.961 | 1.984 | 56.320 | 1.00x |
| users.json | orjson | 2.592 | 2.612 | 2.635 | 56.320 | 0.75x |
| users.json | msgspec | 3.342 | 3.356 | 3.388 | 56.320 | 0.58x |
| users.json | ujson | 10.691 | 10.722 | 10.779 | 56.320 | 0.18x |
| users.json | json | 19.145 | 19.205 | 19.540 | 56.320 | 0.10x |
| flat.json | strata | 0.248 | 0.270 | 0.291 | 68.000 | 1.00x |
| flat.json | orjson | 0.313 | 0.339 | 0.350 | 68.000 | 0.80x |
| flat.json | msgspec | 0.415 | 0.433 | 0.444 | 68.000 | 0.62x |
| flat.json | ujson | 1.027 | 1.033 | 1.057 | 68.000 | 0.26x |
| flat.json | json | 1.757 | 1.777 | 1.796 | 68.000 | 0.15x |
| nested.json | strata | 0.226 | 0.234 | 0.261 | 68.000 | 1.00x |
| nested.json | orjson | 0.287 | 0.295 | 0.323 | 68.000 | 0.79x |
| nested.json | msgspec | 0.381 | 0.393 | 0.409 | 68.000 | 0.59x |
| nested.json | ujson | 1.105 | 1.121 | 1.149 | 68.000 | 0.21x |
| nested.json | json | 2.190 | 2.226 | 2.257 | 68.000 | 0.10x |
| wide_arrays.json | strata | 1.548 | 1.561 | 1.639 | 69.570 | 1.00x |
| wide_arrays.json | orjson | 1.716 | 1.759 | 1.824 | 69.570 | 0.89x |
| wide_arrays.json | msgspec | 2.457 | 2.518 | 2.591 | 69.570 | 0.62x |
| wide_arrays.json | ujson | 4.929 | 5.014 | 5.069 | 69.570 | 0.31x |
| wide_arrays.json | json | 13.908 | 13.950 | 14.025 | 69.570 | 0.11x |
| mixed.json | strata | 0.071 | 0.074 | 0.091 | 69.570 | 1.00x |
| mixed.json | orjson | 0.071 | 0.074 | 0.098 | 69.570 | 1.00x |
| mixed.json | msgspec | 0.089 | 0.092 | 0.118 | 69.570 | 0.80x |
| mixed.json | ujson | 0.256 | 0.265 | 0.286 | 69.570 | 0.28x |
| mixed.json | json | 0.514 | 0.520 | 0.546 | 69.570 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.552 | 9.665 | 10.417 | 68.434 | 1.00x |
| users.json | orjson | 13.016 | 13.247 | 13.745 | 68.434 | 0.73x |
| users.json | msgspec | 13.602 | 14.027 | 14.230 | 68.434 | 0.69x |
| users.json | ujson | 18.549 | 19.519 | 20.481 | 68.434 | 0.50x |
| users.json | json | 22.254 | 22.545 | 22.823 | 68.434 | 0.43x |
| flat.json | strata | 0.957 | 0.970 | 1.000 | 68.000 | 1.00x |
| flat.json | orjson | 1.083 | 1.108 | 1.148 | 68.000 | 0.88x |
| flat.json | msgspec | 1.101 | 1.140 | 1.198 | 68.000 | 0.85x |
| flat.json | ujson | 1.711 | 1.744 | 1.788 | 68.000 | 0.56x |
| flat.json | json | 1.951 | 1.977 | 2.032 | 68.000 | 0.49x |
| nested.json | strata | 0.914 | 0.950 | 1.001 | 68.000 | 1.00x |
| nested.json | orjson | 1.054 | 1.096 | 1.149 | 68.000 | 0.87x |
| nested.json | msgspec | 1.176 | 1.218 | 1.237 | 68.000 | 0.78x |
| nested.json | ujson | 1.625 | 1.670 | 1.693 | 68.000 | 0.57x |
| nested.json | json | 2.140 | 2.200 | 2.253 | 68.000 | 0.43x |
| wide_arrays.json | strata | 4.218 | 4.269 | 4.366 | 69.570 | 1.00x |
| wide_arrays.json | orjson | 4.679 | 4.824 | 5.012 | 69.570 | 0.89x |
| wide_arrays.json | msgspec | 5.754 | 5.827 | 5.986 | 69.570 | 0.73x |
| wide_arrays.json | ujson | 7.373 | 7.440 | 7.519 | 69.570 | 0.57x |
| wide_arrays.json | json | 10.243 | 10.318 | 10.427 | 69.570 | 0.41x |
| mixed.json | strata | 0.242 | 0.261 | 0.291 | 69.570 | 1.00x |
| mixed.json | orjson | 0.337 | 0.358 | 0.390 | 69.570 | 0.73x |
| mixed.json | msgspec | 0.349 | 0.368 | 0.393 | 69.570 | 0.71x |
| mixed.json | ujson | 0.437 | 0.476 | 0.513 | 69.570 | 0.55x |
| mixed.json | json | 0.583 | 0.597 | 0.629 | 69.570 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.399 | 10.458 | 10.562 | 67.992 | 1.00x |
| users.ndjson | orjson | 16.039 | 16.229 | 16.469 | 67.992 | 0.64x |
| users.ndjson | msgspec | 16.565 | 16.645 | 16.754 | 67.992 | 0.63x |
| users.ndjson | ujson | 21.407 | 21.632 | 21.871 | 67.992 | 0.48x |
| users.ndjson | json | 27.698 | 27.998 | 28.153 | 67.992 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.535 | 2.596 | 2.656 | 68.434 | 1.00x |
| users.json | orjson | 3.336 | 3.399 | 3.457 | 68.434 | 0.76x |
| users.json | msgspec | 4.089 | 4.145 | 4.231 | 68.434 | 0.63x |
| users.json | ujson | 11.500 | 11.593 | 11.878 | 68.434 | 0.22x |
| users.json | json | 20.075 | 20.127 | 20.248 | 68.434 | 0.13x |
| flat.json | strata | 0.496 | 0.521 | 0.549 | 68.000 | 1.00x |
| flat.json | orjson | 0.601 | 0.631 | 0.658 | 68.000 | 0.82x |
| flat.json | msgspec | 0.686 | 0.721 | 0.743 | 68.000 | 0.72x |
| flat.json | ujson | 1.335 | 1.364 | 1.388 | 68.000 | 0.38x |
| flat.json | json | 2.084 | 2.103 | 2.174 | 68.000 | 0.25x |
| nested.json | strata | 0.421 | 0.453 | 0.474 | 68.000 | 1.00x |
| nested.json | orjson | 0.540 | 0.561 | 0.595 | 68.000 | 0.81x |
| nested.json | msgspec | 0.625 | 0.661 | 0.696 | 68.000 | 0.69x |
| nested.json | ujson | 1.370 | 1.402 | 1.436 | 68.000 | 0.32x |
| nested.json | json | 2.426 | 2.473 | 2.514 | 68.000 | 0.18x |
| wide_arrays.json | strata | 2.011 | 2.089 | 2.161 | 69.570 | 1.00x |
| wide_arrays.json | orjson | 2.291 | 2.362 | 2.421 | 69.570 | 0.88x |
| wide_arrays.json | msgspec | 3.034 | 3.118 | 3.258 | 69.570 | 0.67x |
| wide_arrays.json | ujson | 5.565 | 5.641 | 5.753 | 69.570 | 0.37x |
| wide_arrays.json | json | 14.538 | 14.590 | 14.675 | 69.570 | 0.14x |
| mixed.json | strata | 0.216 | 0.245 | 0.292 | 69.570 | 1.00x |
| mixed.json | orjson | 0.261 | 0.290 | 0.343 | 69.570 | 0.85x |
| mixed.json | msgspec | 0.272 | 0.296 | 0.341 | 69.570 | 0.83x |
| mixed.json | ujson | 0.469 | 0.497 | 0.522 | 69.570 | 0.49x |
| mixed.json | json | 0.695 | 0.738 | 0.775 | 69.570 | 0.33x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.119 | 0.123 | 0.132 | 68.434 | 1.00x |
| users.json $[*].id | jmespath | 0.492 | 0.505 | 0.534 | 68.434 | 0.24x |
| users.json $[*].id | jsonpath-ng | 2.581 | 2.627 | 2.709 | 68.434 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.683 | 0.718 | 0.772 | 68.559 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.068 | 3.121 | 3.273 | 68.559 | 0.23x |
| users.json $[*].orders[*].total | jsonpath-ng | 20.096 | 20.828 | 21.023 | 68.559 | 0.03x |
| users.json $..total | strata | 1.747 | 1.774 | 1.836 | 69.566 | 1.00x |
| users.json $..total | jsonpath-ng | 298.730 | 299.350 | 299.876 | 69.566 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.221 | 3.251 | 3.265 | 68.559 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.594 | 14.165 | 14.445 | 68.559 | 0.23x |
| users.json $[*].id | orjson+jsonpath-ng | 15.721 | 16.186 | 16.609 | 68.559 | 0.20x |
| users.json $[*].orders[*].total | strata | 3.435 | 3.459 | 3.659 | 69.566 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.169 | 17.377 | 18.105 | 69.566 | 0.20x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 38.275 | 39.245 | 40.251 | 69.566 | 0.09x |
| users.json $..total | strata | 12.709 | 12.994 | 13.303 | 69.629 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 316.557 | 318.201 | 318.671 | 69.629 | 0.04x |

