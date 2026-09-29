# Benchmark results - ci-linux-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 82e3fa5f24c38cfa1150440f0cf5da6c286c01cd
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
| users.json | strata | 8.981 | 9.127 | 11.147 | 57.230 | 1.00x |
| users.json | orjson | 11.547 | 11.880 | 13.793 | 57.230 | 0.77x |
| users.json | msgspec | 12.096 | 12.290 | 14.233 | 57.230 | 0.74x |
| users.json | ujson | 16.268 | 16.918 | 19.995 | 57.230 | 0.54x |
| users.json | pysimdjson | 16.422 | 16.894 | 19.233 | 57.230 | 0.54x |
| users.json | json | 20.567 | 20.934 | 21.750 | 57.230 | 0.44x |
| flat.json | strata | 0.824 | 0.872 | 0.878 | 68.516 | 1.00x |
| flat.json | orjson | 0.860 | 0.889 | 0.894 | 68.516 | 0.98x |
| flat.json | msgspec | 0.915 | 0.921 | 0.928 | 68.516 | 0.95x |
| flat.json | ujson | 1.448 | 1.465 | 1.499 | 68.516 | 0.60x |
| flat.json | pysimdjson | 1.478 | 1.501 | 1.516 | 68.516 | 0.58x |
| flat.json | json | 1.774 | 1.791 | 1.836 | 68.516 | 0.49x |
| nested.json | strata | 0.814 | 0.827 | 0.832 | 68.516 | 1.00x |
| nested.json | orjson | 0.864 | 0.885 | 0.893 | 68.516 | 0.93x |
| nested.json | msgspec | 0.998 | 1.010 | 1.022 | 68.516 | 0.82x |
| nested.json | ujson | 1.396 | 1.411 | 1.431 | 68.516 | 0.59x |
| nested.json | pysimdjson | 1.388 | 1.405 | 1.428 | 68.516 | 0.59x |
| nested.json | json | 1.963 | 1.984 | 2.000 | 68.516 | 0.42x |
| wide_arrays.json | strata | 3.928 | 3.958 | 4.022 | 70.172 | 1.00x |
| wide_arrays.json | orjson | 4.040 | 4.074 | 4.112 | 70.172 | 0.97x |
| wide_arrays.json | msgspec | 5.029 | 5.062 | 5.121 | 70.172 | 0.78x |
| wide_arrays.json | ujson | 6.457 | 6.476 | 6.564 | 70.172 | 0.61x |
| wide_arrays.json | pysimdjson | 5.252 | 5.281 | 5.381 | 70.172 | 0.75x |
| wide_arrays.json | json | 9.409 | 9.430 | 9.563 | 70.172 | 0.42x |
| mixed.json | strata | 0.193 | 0.195 | 0.236 | 70.172 | 1.00x |
| mixed.json | orjson | 0.209 | 0.215 | 0.243 | 70.172 | 0.91x |
| mixed.json | msgspec | 0.230 | 0.233 | 0.264 | 70.172 | 0.84x |
| mixed.json | ujson | 0.300 | 0.316 | 0.326 | 70.172 | 0.62x |
| mixed.json | pysimdjson | 0.291 | 0.303 | 0.341 | 70.172 | 0.64x |
| mixed.json | json | 0.449 | 0.468 | 0.475 | 70.172 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.908 | 1.919 | 1.933 | 56.328 | 1.00x |
| users.json | orjson | 2.580 | 2.588 | 2.619 | 56.328 | 0.74x |
| users.json | msgspec | 3.264 | 3.282 | 3.297 | 56.328 | 0.58x |
| users.json | ujson | 10.537 | 10.559 | 10.599 | 56.328 | 0.18x |
| users.json | json | 19.185 | 19.281 | 19.335 | 56.328 | 0.10x |
| flat.json | strata | 0.233 | 0.234 | 0.251 | 68.516 | 1.00x |
| flat.json | orjson | 0.300 | 0.315 | 0.341 | 68.516 | 0.74x |
| flat.json | msgspec | 0.385 | 0.400 | 0.419 | 68.516 | 0.58x |
| flat.json | ujson | 0.981 | 0.987 | 0.993 | 68.516 | 0.24x |
| flat.json | json | 1.696 | 1.700 | 1.744 | 68.516 | 0.14x |
| nested.json | strata | 0.211 | 0.215 | 0.224 | 68.516 | 1.00x |
| nested.json | orjson | 0.286 | 0.288 | 0.306 | 68.516 | 0.75x |
| nested.json | msgspec | 0.368 | 0.383 | 0.390 | 68.516 | 0.56x |
| nested.json | ujson | 1.073 | 1.076 | 1.091 | 68.516 | 0.20x |
| nested.json | json | 2.134 | 2.157 | 2.192 | 68.516 | 0.10x |
| wide_arrays.json | strata | 1.302 | 1.329 | 1.341 | 70.172 | 1.00x |
| wide_arrays.json | orjson | 1.554 | 1.570 | 1.608 | 70.172 | 0.85x |
| wide_arrays.json | msgspec | 2.354 | 2.381 | 2.413 | 70.172 | 0.56x |
| wide_arrays.json | ujson | 4.723 | 4.744 | 4.886 | 70.172 | 0.28x |
| wide_arrays.json | json | 13.542 | 13.618 | 13.705 | 70.172 | 0.10x |
| mixed.json | strata | 0.060 | 0.061 | 0.062 | 70.172 | 1.00x |
| mixed.json | orjson | 0.063 | 0.064 | 0.086 | 70.172 | 0.95x |
| mixed.json | msgspec | 0.076 | 0.078 | 0.080 | 70.172 | 0.78x |
| mixed.json | ujson | 0.238 | 0.242 | 0.257 | 70.172 | 0.25x |
| mixed.json | json | 0.474 | 0.484 | 0.503 | 70.172 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.406 | 9.579 | 10.212 | 70.148 | 1.00x |
| users.json | orjson | 12.064 | 12.462 | 12.938 | 70.148 | 0.77x |
| users.json | msgspec | 12.729 | 13.010 | 13.224 | 70.148 | 0.74x |
| users.json | ujson | 17.725 | 18.374 | 18.930 | 70.148 | 0.52x |
| users.json | json | 21.249 | 21.535 | 21.714 | 70.148 | 0.44x |
| flat.json | strata | 0.893 | 0.906 | 0.922 | 68.516 | 1.00x |
| flat.json | orjson | 0.944 | 0.970 | 0.981 | 68.516 | 0.93x |
| flat.json | msgspec | 0.992 | 1.005 | 1.016 | 68.516 | 0.90x |
| flat.json | ujson | 1.557 | 1.572 | 1.583 | 68.516 | 0.58x |
| flat.json | json | 1.840 | 1.858 | 1.872 | 68.516 | 0.49x |
| nested.json | strata | 0.853 | 0.877 | 0.940 | 68.516 | 1.00x |
| nested.json | orjson | 0.949 | 0.982 | 1.039 | 68.516 | 0.89x |
| nested.json | msgspec | 1.064 | 1.088 | 1.154 | 68.516 | 0.81x |
| nested.json | ujson | 1.490 | 1.518 | 1.636 | 68.516 | 0.58x |
| nested.json | json | 2.030 | 2.058 | 2.119 | 68.516 | 0.43x |
| wide_arrays.json | strata | 3.915 | 3.937 | 3.971 | 70.172 | 1.00x |
| wide_arrays.json | orjson | 4.071 | 4.104 | 4.187 | 70.172 | 0.96x |
| wide_arrays.json | msgspec | 5.109 | 5.132 | 5.196 | 70.172 | 0.77x |
| wide_arrays.json | ujson | 6.619 | 6.651 | 6.698 | 70.172 | 0.59x |
| wide_arrays.json | json | 9.481 | 9.533 | 9.562 | 70.172 | 0.41x |
| mixed.json | strata | 0.213 | 0.219 | 0.222 | 70.172 | 1.00x |
| mixed.json | orjson | 0.276 | 0.279 | 0.297 | 70.172 | 0.79x |
| mixed.json | msgspec | 0.293 | 0.301 | 0.311 | 70.172 | 0.73x |
| mixed.json | ujson | 0.373 | 0.382 | 0.415 | 70.172 | 0.57x |
| mixed.json | json | 0.499 | 0.506 | 0.535 | 70.172 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.493 | 9.732 | 10.038 | 68.516 | 1.00x |
| users.ndjson | orjson | 14.615 | 14.972 | 15.219 | 68.516 | 0.65x |
| users.ndjson | msgspec | 15.099 | 15.344 | 15.545 | 68.516 | 0.63x |
| users.ndjson | ujson | 19.575 | 20.213 | 20.575 | 68.516 | 0.48x |
| users.ndjson | json | 25.876 | 26.719 | 27.075 | 68.516 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.780 | 2.860 | 7.448 | 70.148 | 1.00x |
| users.json | orjson | 3.497 | 3.645 | 3.695 | 70.148 | 0.78x |
| users.json | msgspec | 4.195 | 4.312 | 4.383 | 70.148 | 0.66x |
| users.json | ujson | 11.579 | 11.724 | 13.585 | 70.148 | 0.24x |
| users.json | json | 20.130 | 20.367 | 22.630 | 70.148 | 0.14x |
| flat.json | strata | 0.679 | 0.726 | 0.754 | 68.516 | 1.00x |
| flat.json | orjson | 0.773 | 0.821 | 0.908 | 68.516 | 0.88x |
| flat.json | msgspec | 0.895 | 0.922 | 3.228 | 68.516 | 0.79x |
| flat.json | ujson | 1.515 | 1.541 | 1.583 | 68.516 | 0.47x |
| flat.json | json | 2.233 | 2.272 | 2.398 | 68.516 | 0.32x |
| nested.json | strata | 0.603 | 0.632 | 0.667 | 68.516 | 1.00x |
| nested.json | orjson | 0.744 | 0.761 | 0.996 | 68.516 | 0.83x |
| nested.json | msgspec | 0.826 | 0.849 | 5.153 | 68.516 | 0.74x |
| nested.json | ujson | 1.553 | 1.603 | 1.659 | 68.516 | 0.39x |
| nested.json | json | 2.656 | 2.682 | 2.717 | 68.516 | 0.24x |
| wide_arrays.json | strata | 1.951 | 1.994 | 4.339 | 70.172 | 1.00x |
| wide_arrays.json | orjson | 2.242 | 2.271 | 2.349 | 70.172 | 0.88x |
| wide_arrays.json | msgspec | 3.039 | 3.091 | 3.203 | 70.172 | 0.65x |
| wide_arrays.json | ujson | 5.426 | 5.502 | 5.656 | 70.172 | 0.36x |
| wide_arrays.json | json | 14.341 | 14.437 | 14.511 | 70.172 | 0.14x |
| mixed.json | strata | 0.374 | 0.415 | 0.438 | 70.172 | 1.00x |
| mixed.json | orjson | 0.438 | 0.461 | 3.716 | 70.172 | 0.90x |
| mixed.json | msgspec | 0.450 | 0.479 | 0.527 | 70.172 | 0.87x |
| mixed.json | ujson | 0.637 | 0.675 | 0.707 | 70.172 | 0.61x |
| mixed.json | json | 0.903 | 0.922 | 1.007 | 70.172 | 0.45x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.110 | 0.112 | 0.121 | 70.148 | 1.00x |
| users.json $[*].id | jmespath | 0.478 | 0.493 | 0.508 | 70.148 | 0.23x |
| users.json $[*].id | jsonpath-ng | 2.506 | 2.563 | 2.598 | 70.148 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.628 | 0.648 | 0.656 | 70.152 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.945 | 2.986 | 3.045 | 70.152 | 0.22x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.930 | 18.361 | 19.236 | 70.152 | 0.04x |
| users.json $..total | strata | 1.700 | 1.719 | 1.739 | 70.152 | 1.00x |
| users.json $..total | jsonpath-ng | 294.274 | 294.553 | 295.629 | 70.152 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.208 | 3.219 | 3.247 | 70.152 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.822 | 13.014 | 13.506 | 70.152 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 14.495 | 14.858 | 15.500 | 70.152 | 0.22x |
| users.json $[*].orders[*].total | strata | 3.373 | 3.394 | 3.416 | 70.152 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.743 | 15.965 | 16.189 | 70.152 | 0.21x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 34.162 | 34.744 | 35.195 | 70.152 | 0.10x |
| users.json $..total | strata | 11.900 | 12.183 | 12.842 | 70.152 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 315.553 | 316.393 | 318.117 | 70.152 | 0.04x |

