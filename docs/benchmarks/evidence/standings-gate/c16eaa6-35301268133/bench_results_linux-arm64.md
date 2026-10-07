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
| users.json | strata | 8.708 | 8.760 | 10.533 | 57.281 | 1.00x |
| users.json | orjson | 11.391 | 11.456 | 12.953 | 57.281 | 0.76x |
| users.json | msgspec | 11.947 | 12.019 | 13.405 | 57.281 | 0.73x |
| users.json | ujson | 16.020 | 16.091 | 18.376 | 57.281 | 0.54x |
| users.json | pysimdjson | 16.048 | 16.121 | 17.833 | 57.281 | 0.54x |
| users.json | json | 20.136 | 20.259 | 20.909 | 57.281 | 0.43x |
| flat.json | strata | 0.786 | 0.799 | 0.805 | 67.984 | 1.00x |
| flat.json | orjson | 0.850 | 0.860 | 0.868 | 67.984 | 0.93x |
| flat.json | msgspec | 0.897 | 0.910 | 0.920 | 67.984 | 0.88x |
| flat.json | ujson | 1.412 | 1.423 | 1.428 | 67.984 | 0.56x |
| flat.json | pysimdjson | 1.461 | 1.468 | 1.503 | 67.984 | 0.54x |
| flat.json | json | 1.765 | 1.775 | 1.782 | 67.984 | 0.45x |
| nested.json | strata | 0.791 | 0.810 | 0.823 | 67.984 | 1.00x |
| nested.json | orjson | 0.857 | 0.872 | 0.879 | 67.984 | 0.93x |
| nested.json | msgspec | 0.974 | 0.982 | 0.989 | 67.984 | 0.82x |
| nested.json | ujson | 1.364 | 1.382 | 1.391 | 67.984 | 0.59x |
| nested.json | pysimdjson | 1.372 | 1.389 | 1.412 | 67.984 | 0.58x |
| nested.json | json | 1.938 | 1.949 | 1.955 | 67.984 | 0.42x |
| wide_arrays.json | strata | 3.801 | 3.808 | 3.853 | 69.566 | 1.00x |
| wide_arrays.json | orjson | 3.968 | 4.005 | 4.026 | 69.566 | 0.95x |
| wide_arrays.json | msgspec | 4.992 | 5.022 | 5.051 | 69.566 | 0.76x |
| wide_arrays.json | ujson | 6.398 | 6.436 | 6.484 | 69.566 | 0.59x |
| wide_arrays.json | pysimdjson | 5.128 | 5.179 | 5.227 | 69.566 | 0.74x |
| wide_arrays.json | json | 9.394 | 9.431 | 9.451 | 69.566 | 0.40x |
| mixed.json | strata | 0.183 | 0.184 | 0.210 | 69.566 | 1.00x |
| mixed.json | orjson | 0.208 | 0.210 | 0.223 | 69.566 | 0.88x |
| mixed.json | msgspec | 0.231 | 0.245 | 0.265 | 69.566 | 0.75x |
| mixed.json | ujson | 0.293 | 0.296 | 0.320 | 69.566 | 0.62x |
| mixed.json | pysimdjson | 0.286 | 0.288 | 0.302 | 69.566 | 0.64x |
| mixed.json | json | 0.441 | 0.443 | 0.459 | 69.566 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.902 | 1.905 | 1.914 | 56.391 | 1.00x |
| users.json | orjson | 2.556 | 2.561 | 2.574 | 56.391 | 0.74x |
| users.json | msgspec | 3.291 | 3.299 | 3.312 | 56.391 | 0.58x |
| users.json | ujson | 10.443 | 10.475 | 10.503 | 56.391 | 0.18x |
| users.json | json | 18.781 | 18.832 | 18.917 | 56.391 | 0.10x |
| flat.json | strata | 0.230 | 0.232 | 0.245 | 67.984 | 1.00x |
| flat.json | orjson | 0.296 | 0.297 | 0.314 | 67.984 | 0.78x |
| flat.json | msgspec | 0.376 | 0.386 | 0.399 | 67.984 | 0.60x |
| flat.json | ujson | 0.974 | 0.976 | 0.985 | 67.984 | 0.24x |
| flat.json | json | 1.682 | 1.690 | 1.707 | 67.984 | 0.14x |
| nested.json | strata | 0.208 | 0.209 | 0.226 | 67.988 | 1.00x |
| nested.json | orjson | 0.281 | 0.282 | 0.299 | 67.988 | 0.74x |
| nested.json | msgspec | 0.364 | 0.368 | 0.381 | 67.988 | 0.57x |
| nested.json | ujson | 1.067 | 1.076 | 1.091 | 67.988 | 0.19x |
| nested.json | json | 2.145 | 2.164 | 2.187 | 67.988 | 0.10x |
| wide_arrays.json | strata | 1.319 | 1.329 | 1.341 | 69.566 | 1.00x |
| wide_arrays.json | orjson | 1.571 | 1.578 | 1.588 | 69.566 | 0.84x |
| wide_arrays.json | msgspec | 2.342 | 2.352 | 2.367 | 69.566 | 0.57x |
| wide_arrays.json | ujson | 4.705 | 4.733 | 4.755 | 69.566 | 0.28x |
| wide_arrays.json | json | 13.480 | 13.495 | 13.522 | 69.566 | 0.10x |
| mixed.json | strata | 0.058 | 0.058 | 0.065 | 69.566 | 1.00x |
| mixed.json | orjson | 0.061 | 0.062 | 0.078 | 69.566 | 0.94x |
| mixed.json | msgspec | 0.074 | 0.075 | 0.093 | 69.566 | 0.77x |
| mixed.json | ujson | 0.230 | 0.233 | 0.246 | 69.566 | 0.25x |
| mixed.json | json | 0.464 | 0.473 | 0.495 | 69.566 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.841 | 8.894 | 9.634 | 68.426 | 1.00x |
| users.json | orjson | 11.432 | 11.592 | 11.992 | 68.426 | 0.77x |
| users.json | msgspec | 12.029 | 12.163 | 12.414 | 68.426 | 0.73x |
| users.json | ujson | 16.385 | 16.511 | 17.636 | 68.426 | 0.54x |
| users.json | json | 20.413 | 20.448 | 20.556 | 68.426 | 0.43x |
| flat.json | strata | 0.820 | 0.835 | 0.843 | 67.984 | 1.00x |
| flat.json | orjson | 0.926 | 0.932 | 0.944 | 67.984 | 0.90x |
| flat.json | msgspec | 0.974 | 0.978 | 0.985 | 67.984 | 0.85x |
| flat.json | ujson | 1.507 | 1.517 | 1.530 | 67.984 | 0.55x |
| flat.json | json | 1.829 | 1.836 | 1.844 | 67.984 | 0.45x |
| nested.json | strata | 0.824 | 0.844 | 0.849 | 67.988 | 1.00x |
| nested.json | orjson | 0.916 | 0.936 | 0.955 | 67.988 | 0.90x |
| nested.json | msgspec | 1.036 | 1.045 | 1.059 | 67.988 | 0.81x |
| nested.json | ujson | 1.440 | 1.459 | 1.478 | 67.988 | 0.58x |
| nested.json | json | 1.995 | 2.010 | 2.028 | 67.988 | 0.42x |
| wide_arrays.json | strata | 3.799 | 3.820 | 3.852 | 69.566 | 1.00x |
| wide_arrays.json | orjson | 3.976 | 4.002 | 4.022 | 69.566 | 0.95x |
| wide_arrays.json | msgspec | 5.026 | 5.044 | 5.092 | 69.566 | 0.76x |
| wide_arrays.json | ujson | 6.575 | 6.601 | 6.647 | 69.566 | 0.58x |
| wide_arrays.json | json | 9.467 | 9.510 | 9.570 | 69.566 | 0.40x |
| mixed.json | strata | 0.204 | 0.206 | 0.229 | 69.566 | 1.00x |
| mixed.json | orjson | 0.264 | 0.270 | 0.290 | 69.566 | 0.76x |
| mixed.json | msgspec | 0.283 | 0.286 | 0.310 | 69.566 | 0.72x |
| mixed.json | ujson | 0.363 | 0.373 | 0.397 | 69.566 | 0.55x |
| mixed.json | json | 0.499 | 0.517 | 0.523 | 69.566 | 0.40x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.174 | 9.196 | 9.241 | 67.980 | 1.00x |
| users.ndjson | orjson | 14.297 | 14.339 | 14.465 | 67.980 | 0.64x |
| users.ndjson | msgspec | 14.679 | 14.713 | 14.843 | 67.980 | 0.63x |
| users.ndjson | ujson | 19.125 | 19.221 | 19.325 | 67.980 | 0.48x |
| users.ndjson | json | 25.209 | 25.239 | 25.374 | 67.980 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.289 | 2.341 | 2.381 | 68.426 | 1.00x |
| users.json | orjson | 3.008 | 3.056 | 3.073 | 68.426 | 0.77x |
| users.json | msgspec | 3.718 | 3.763 | 3.816 | 68.426 | 0.62x |
| users.json | ujson | 11.109 | 11.345 | 11.433 | 68.426 | 0.21x |
| users.json | json | 19.482 | 19.559 | 19.814 | 68.426 | 0.12x |
| flat.json | strata | 0.361 | 0.378 | 0.402 | 67.984 | 1.00x |
| flat.json | orjson | 0.460 | 0.474 | 0.503 | 67.984 | 0.80x |
| flat.json | msgspec | 0.533 | 0.558 | 0.588 | 67.984 | 0.68x |
| flat.json | ujson | 1.156 | 1.170 | 1.185 | 67.984 | 0.32x |
| flat.json | json | 1.857 | 1.894 | 1.904 | 67.984 | 0.20x |
| nested.json | strata | 0.328 | 0.353 | 0.379 | 67.988 | 1.00x |
| nested.json | orjson | 0.431 | 0.457 | 0.491 | 67.988 | 0.77x |
| nested.json | msgspec | 0.514 | 0.546 | 0.570 | 67.988 | 0.65x |
| nested.json | ujson | 1.257 | 1.271 | 1.302 | 67.988 | 0.28x |
| nested.json | json | 2.326 | 2.351 | 2.381 | 67.988 | 0.15x |
| wide_arrays.json | strata | 1.630 | 1.673 | 1.679 | 69.566 | 1.00x |
| wide_arrays.json | orjson | 1.936 | 1.960 | 1.990 | 69.566 | 0.85x |
| wide_arrays.json | msgspec | 2.699 | 2.741 | 2.781 | 69.566 | 0.61x |
| wide_arrays.json | ujson | 5.146 | 5.169 | 5.177 | 69.566 | 0.32x |
| wide_arrays.json | json | 13.877 | 13.916 | 13.943 | 69.566 | 0.12x |
| mixed.json | strata | 0.150 | 0.159 | 0.163 | 69.566 | 1.00x |
| mixed.json | orjson | 0.172 | 0.181 | 0.195 | 69.566 | 0.88x |
| mixed.json | msgspec | 0.187 | 0.197 | 0.223 | 69.566 | 0.80x |
| mixed.json | ujson | 0.363 | 0.371 | 0.394 | 69.566 | 0.43x |
| mixed.json | json | 0.600 | 0.614 | 0.631 | 69.566 | 0.26x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.096 | 0.098 | 0.109 | 68.426 | 1.00x |
| users.json $[*].id | jmespath | 0.460 | 0.468 | 0.479 | 68.426 | 0.21x |
| users.json $[*].id | jsonpath-ng | 2.394 | 2.438 | 2.477 | 68.426 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.577 | 0.592 | 0.603 | 68.535 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.891 | 2.927 | 2.959 | 68.535 | 0.20x |
| users.json $[*].orders[*].total | jsonpath-ng | 16.969 | 17.084 | 17.337 | 68.535 | 0.03x |
| users.json $..total | strata | 1.679 | 1.692 | 1.727 | 69.562 | 1.00x |
| users.json $..total | jsonpath-ng | 295.010 | 295.582 | 296.162 | 69.562 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.100 | 3.111 | 3.192 | 68.535 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.103 | 12.191 | 12.360 | 68.535 | 0.26x |
| users.json $[*].id | orjson+jsonpath-ng | 14.054 | 14.116 | 14.191 | 68.535 | 0.22x |
| users.json $[*].orders[*].total | strata | 3.267 | 3.287 | 3.334 | 69.562 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 14.685 | 14.797 | 14.867 | 69.562 | 0.22x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 32.494 | 32.645 | 32.992 | 69.562 | 0.10x |
| users.json $..total | strata | 10.970 | 11.233 | 11.505 | 69.617 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 308.537 | 309.383 | 310.616 | 69.617 | 0.04x |

