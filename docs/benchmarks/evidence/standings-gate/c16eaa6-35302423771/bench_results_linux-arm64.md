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
| users.json | strata | 9.296 | 9.352 | 11.461 | 57.258 | 1.00x |
| users.json | orjson | 12.910 | 13.182 | 14.603 | 57.258 | 0.71x |
| users.json | msgspec | 13.417 | 13.564 | 15.040 | 57.258 | 0.69x |
| users.json | ujson | 18.537 | 18.907 | 21.304 | 57.258 | 0.49x |
| users.json | pysimdjson | 18.746 | 19.040 | 20.912 | 57.258 | 0.49x |
| users.json | json | 22.101 | 22.393 | 22.925 | 57.258 | 0.42x |
| flat.json | strata | 0.901 | 0.907 | 0.925 | 68.000 | 1.00x |
| flat.json | orjson | 0.944 | 0.961 | 0.986 | 68.000 | 0.94x |
| flat.json | msgspec | 0.965 | 0.981 | 0.995 | 68.000 | 0.92x |
| flat.json | ujson | 1.535 | 1.557 | 1.592 | 68.000 | 0.58x |
| flat.json | pysimdjson | 1.589 | 1.600 | 1.632 | 68.000 | 0.57x |
| flat.json | json | 1.830 | 1.862 | 1.875 | 68.000 | 0.49x |
| nested.json | strata | 0.861 | 0.877 | 0.940 | 68.000 | 1.00x |
| nested.json | orjson | 0.918 | 0.948 | 0.964 | 68.000 | 0.93x |
| nested.json | msgspec | 1.038 | 1.043 | 1.092 | 68.000 | 0.84x |
| nested.json | ujson | 1.461 | 1.492 | 1.551 | 68.000 | 0.59x |
| nested.json | pysimdjson | 1.454 | 1.476 | 1.511 | 68.000 | 0.59x |
| nested.json | json | 2.055 | 2.083 | 2.097 | 68.000 | 0.42x |
| wide_arrays.json | strata | 4.355 | 4.405 | 4.476 | 69.570 | 1.00x |
| wide_arrays.json | orjson | 4.652 | 4.739 | 4.860 | 69.570 | 0.93x |
| wide_arrays.json | msgspec | 5.618 | 5.674 | 5.771 | 69.570 | 0.78x |
| wide_arrays.json | ujson | 7.105 | 7.167 | 7.277 | 69.570 | 0.61x |
| wide_arrays.json | pysimdjson | 5.956 | 5.985 | 6.081 | 69.570 | 0.74x |
| wide_arrays.json | json | 10.212 | 10.251 | 10.405 | 69.570 | 0.43x |
| mixed.json | strata | 0.213 | 0.217 | 0.244 | 69.570 | 1.00x |
| mixed.json | orjson | 0.230 | 0.239 | 0.275 | 69.570 | 0.91x |
| mixed.json | msgspec | 0.257 | 0.264 | 0.295 | 69.570 | 0.82x |
| mixed.json | ujson | 0.337 | 0.342 | 0.363 | 69.570 | 0.63x |
| mixed.json | pysimdjson | 0.311 | 0.318 | 0.346 | 69.570 | 0.68x |
| mixed.json | json | 0.493 | 0.511 | 0.546 | 69.570 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.990 | 2.003 | 2.017 | 56.363 | 1.00x |
| users.json | orjson | 2.631 | 2.652 | 2.711 | 56.363 | 0.76x |
| users.json | msgspec | 3.380 | 3.393 | 3.428 | 56.363 | 0.59x |
| users.json | ujson | 10.699 | 10.760 | 10.898 | 56.363 | 0.19x |
| users.json | json | 19.292 | 19.358 | 19.497 | 56.363 | 0.10x |
| flat.json | strata | 0.260 | 0.266 | 0.289 | 68.000 | 1.00x |
| flat.json | orjson | 0.326 | 0.329 | 0.351 | 68.000 | 0.81x |
| flat.json | msgspec | 0.421 | 0.433 | 0.462 | 68.000 | 0.61x |
| flat.json | ujson | 1.039 | 1.052 | 1.062 | 68.000 | 0.25x |
| flat.json | json | 1.779 | 1.793 | 1.817 | 68.000 | 0.15x |
| nested.json | strata | 0.232 | 0.236 | 0.255 | 68.000 | 1.00x |
| nested.json | orjson | 0.292 | 0.300 | 0.323 | 68.000 | 0.79x |
| nested.json | msgspec | 0.385 | 0.391 | 0.417 | 68.000 | 0.60x |
| nested.json | ujson | 1.100 | 1.113 | 1.133 | 68.000 | 0.21x |
| nested.json | json | 2.202 | 2.228 | 2.297 | 68.000 | 0.11x |
| wide_arrays.json | strata | 1.542 | 1.573 | 1.605 | 69.570 | 1.00x |
| wide_arrays.json | orjson | 1.719 | 1.787 | 1.811 | 69.570 | 0.88x |
| wide_arrays.json | msgspec | 2.482 | 2.532 | 2.600 | 69.570 | 0.62x |
| wide_arrays.json | ujson | 4.979 | 5.021 | 5.068 | 69.570 | 0.31x |
| wide_arrays.json | json | 13.920 | 13.958 | 14.048 | 69.570 | 0.11x |
| mixed.json | strata | 0.071 | 0.073 | 0.095 | 69.570 | 1.00x |
| mixed.json | orjson | 0.071 | 0.074 | 0.093 | 69.570 | 0.98x |
| mixed.json | msgspec | 0.089 | 0.091 | 0.092 | 69.570 | 0.80x |
| mixed.json | ujson | 0.254 | 0.263 | 0.283 | 69.570 | 0.28x |
| mixed.json | json | 0.522 | 0.531 | 0.535 | 69.570 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.555 | 9.713 | 10.424 | 68.430 | 1.00x |
| users.json | orjson | 13.178 | 13.516 | 13.974 | 68.430 | 0.72x |
| users.json | msgspec | 13.989 | 14.138 | 14.299 | 68.430 | 0.69x |
| users.json | ujson | 18.977 | 19.671 | 20.289 | 68.430 | 0.49x |
| users.json | json | 22.560 | 22.734 | 23.025 | 68.430 | 0.43x |
| flat.json | strata | 0.975 | 0.987 | 1.005 | 68.000 | 1.00x |
| flat.json | orjson | 1.087 | 1.105 | 1.121 | 68.000 | 0.89x |
| flat.json | msgspec | 1.122 | 1.128 | 1.152 | 68.000 | 0.87x |
| flat.json | ujson | 1.714 | 1.733 | 1.765 | 68.000 | 0.57x |
| flat.json | json | 1.985 | 1.995 | 2.005 | 68.000 | 0.49x |
| nested.json | strata | 0.943 | 0.950 | 0.973 | 68.000 | 1.00x |
| nested.json | orjson | 1.081 | 1.095 | 1.106 | 68.000 | 0.87x |
| nested.json | msgspec | 1.177 | 1.208 | 1.224 | 68.000 | 0.79x |
| nested.json | ujson | 1.662 | 1.687 | 1.703 | 68.000 | 0.56x |
| nested.json | json | 2.173 | 2.207 | 2.258 | 68.000 | 0.43x |
| wide_arrays.json | strata | 4.221 | 4.258 | 4.325 | 69.570 | 1.00x |
| wide_arrays.json | orjson | 4.714 | 4.786 | 4.862 | 69.570 | 0.89x |
| wide_arrays.json | msgspec | 5.693 | 5.781 | 5.823 | 69.570 | 0.74x |
| wide_arrays.json | ujson | 7.396 | 7.472 | 7.518 | 69.570 | 0.57x |
| wide_arrays.json | json | 10.177 | 10.284 | 10.354 | 69.570 | 0.41x |
| mixed.json | strata | 0.251 | 0.266 | 0.277 | 69.570 | 1.00x |
| mixed.json | orjson | 0.343 | 0.363 | 0.381 | 69.570 | 0.73x |
| mixed.json | msgspec | 0.366 | 0.386 | 0.406 | 69.570 | 0.69x |
| mixed.json | ujson | 0.460 | 0.486 | 0.514 | 69.570 | 0.55x |
| mixed.json | json | 0.582 | 0.605 | 0.631 | 69.570 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.453 | 10.554 | 10.643 | 67.992 | 1.00x |
| users.ndjson | orjson | 16.329 | 16.497 | 16.611 | 67.992 | 0.64x |
| users.ndjson | msgspec | 16.617 | 16.719 | 16.818 | 67.992 | 0.63x |
| users.ndjson | ujson | 21.808 | 21.923 | 22.189 | 67.992 | 0.48x |
| users.ndjson | json | 28.110 | 28.244 | 28.333 | 67.992 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.742 | 2.767 | 2.821 | 68.430 | 1.00x |
| users.json | orjson | 3.502 | 3.557 | 3.615 | 68.430 | 0.78x |
| users.json | msgspec | 4.242 | 4.300 | 4.364 | 68.430 | 0.64x |
| users.json | ujson | 11.648 | 11.799 | 11.859 | 68.430 | 0.23x |
| users.json | json | 20.363 | 20.455 | 20.734 | 68.430 | 0.14x |
| flat.json | strata | 0.551 | 0.582 | 0.652 | 68.000 | 1.00x |
| flat.json | orjson | 0.673 | 0.693 | 0.774 | 68.000 | 0.84x |
| flat.json | msgspec | 0.742 | 0.782 | 0.813 | 68.000 | 0.74x |
| flat.json | ujson | 1.398 | 1.424 | 1.520 | 68.000 | 0.41x |
| flat.json | json | 2.166 | 2.202 | 2.272 | 68.000 | 0.26x |
| nested.json | strata | 0.502 | 0.544 | 0.574 | 68.000 | 1.00x |
| nested.json | orjson | 0.621 | 0.653 | 0.665 | 68.000 | 0.83x |
| nested.json | msgspec | 0.726 | 0.746 | 0.812 | 68.000 | 0.73x |
| nested.json | ujson | 1.469 | 1.499 | 1.570 | 68.000 | 0.36x |
| nested.json | json | 2.585 | 2.622 | 2.655 | 68.000 | 0.21x |
| wide_arrays.json | strata | 2.130 | 2.199 | 2.315 | 69.570 | 1.00x |
| wide_arrays.json | orjson | 2.400 | 2.495 | 2.566 | 69.570 | 0.88x |
| wide_arrays.json | msgspec | 3.186 | 3.283 | 3.416 | 69.570 | 0.67x |
| wide_arrays.json | ujson | 5.687 | 5.815 | 6.022 | 69.570 | 0.38x |
| wide_arrays.json | json | 14.724 | 14.778 | 14.826 | 69.570 | 0.15x |
| mixed.json | strata | 0.292 | 0.306 | 0.349 | 69.570 | 1.00x |
| mixed.json | orjson | 0.325 | 0.360 | 0.389 | 69.570 | 0.85x |
| mixed.json | msgspec | 0.342 | 0.374 | 0.417 | 69.570 | 0.82x |
| mixed.json | ujson | 0.522 | 0.563 | 0.599 | 69.570 | 0.54x |
| mixed.json | json | 0.792 | 0.833 | 0.885 | 69.570 | 0.37x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.120 | 0.123 | 0.143 | 68.430 | 1.00x |
| users.json $[*].id | jmespath | 0.503 | 0.509 | 0.525 | 68.430 | 0.24x |
| users.json $[*].id | jsonpath-ng | 2.593 | 2.683 | 2.754 | 68.430 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.683 | 0.732 | 0.786 | 68.559 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.173 | 3.242 | 3.412 | 68.559 | 0.23x |
| users.json $[*].orders[*].total | jsonpath-ng | 20.486 | 20.850 | 21.178 | 68.559 | 0.04x |
| users.json $..total | strata | 1.793 | 1.811 | 1.840 | 69.566 | 1.00x |
| users.json $..total | jsonpath-ng | 294.596 | 295.176 | 295.916 | 69.566 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.243 | 3.256 | 3.280 | 68.559 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.282 | 14.481 | 14.596 | 68.559 | 0.22x |
| users.json $[*].id | orjson+jsonpath-ng | 16.353 | 16.570 | 16.931 | 68.559 | 0.20x |
| users.json $[*].orders[*].total | strata | 3.427 | 3.456 | 3.507 | 69.566 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.584 | 17.838 | 18.265 | 69.566 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 38.981 | 39.664 | 39.852 | 69.566 | 0.09x |
| users.json $..total | strata | 12.579 | 13.202 | 13.662 | 69.629 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 317.691 | 319.236 | 319.911 | 69.629 | 0.04x |

