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
| users.json | strata | 8.758 | 8.868 | 10.829 | 57.242 | 1.00x |
| users.json | orjson | 11.694 | 11.805 | 13.576 | 57.242 | 0.75x |
| users.json | msgspec | 12.177 | 12.313 | 14.017 | 57.242 | 0.72x |
| users.json | ujson | 16.637 | 16.949 | 19.347 | 57.242 | 0.52x |
| users.json | pysimdjson | 16.493 | 16.974 | 18.623 | 57.242 | 0.52x |
| users.json | json | 20.641 | 20.861 | 21.740 | 57.242 | 0.43x |
| flat.json | strata | 0.813 | 0.835 | 0.862 | 67.996 | 1.00x |
| flat.json | orjson | 0.869 | 0.886 | 0.909 | 67.996 | 0.94x |
| flat.json | msgspec | 0.891 | 0.909 | 0.925 | 67.996 | 0.92x |
| flat.json | ujson | 1.416 | 1.447 | 1.502 | 67.996 | 0.58x |
| flat.json | pysimdjson | 1.458 | 1.473 | 1.513 | 67.996 | 0.57x |
| flat.json | json | 1.764 | 1.773 | 1.791 | 67.996 | 0.47x |
| nested.json | strata | 0.797 | 0.812 | 0.821 | 67.996 | 1.00x |
| nested.json | orjson | 0.874 | 0.884 | 0.956 | 67.996 | 0.92x |
| nested.json | msgspec | 0.983 | 0.988 | 0.995 | 67.996 | 0.82x |
| nested.json | ujson | 1.380 | 1.401 | 1.423 | 67.996 | 0.58x |
| nested.json | pysimdjson | 1.383 | 1.404 | 1.432 | 67.996 | 0.58x |
| nested.json | json | 1.948 | 1.960 | 1.994 | 67.996 | 0.41x |
| wide_arrays.json | strata | 3.856 | 3.867 | 3.967 | 69.566 | 1.00x |
| wide_arrays.json | orjson | 4.062 | 4.084 | 4.132 | 69.566 | 0.95x |
| wide_arrays.json | msgspec | 5.022 | 5.067 | 5.093 | 69.566 | 0.76x |
| wide_arrays.json | ujson | 6.469 | 6.518 | 6.571 | 69.566 | 0.59x |
| wide_arrays.json | pysimdjson | 5.264 | 5.284 | 5.339 | 69.566 | 0.73x |
| wide_arrays.json | json | 9.482 | 9.496 | 9.542 | 69.566 | 0.41x |
| mixed.json | strata | 0.188 | 0.190 | 0.218 | 69.566 | 1.00x |
| mixed.json | orjson | 0.212 | 0.214 | 0.235 | 69.566 | 0.89x |
| mixed.json | msgspec | 0.231 | 0.235 | 0.255 | 69.566 | 0.81x |
| mixed.json | ujson | 0.301 | 0.305 | 0.326 | 69.566 | 0.62x |
| mixed.json | pysimdjson | 0.292 | 0.295 | 0.315 | 69.566 | 0.64x |
| mixed.json | json | 0.453 | 0.457 | 0.480 | 69.566 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.932 | 1.939 | 1.949 | 56.348 | 1.00x |
| users.json | orjson | 2.591 | 2.614 | 2.631 | 56.348 | 0.74x |
| users.json | msgspec | 3.314 | 3.327 | 3.351 | 56.348 | 0.58x |
| users.json | ujson | 10.519 | 10.558 | 10.639 | 56.348 | 0.18x |
| users.json | json | 19.005 | 19.090 | 19.214 | 56.348 | 0.10x |
| flat.json | strata | 0.235 | 0.241 | 0.259 | 67.996 | 1.00x |
| flat.json | orjson | 0.302 | 0.304 | 0.321 | 67.996 | 0.79x |
| flat.json | msgspec | 0.391 | 0.404 | 0.411 | 67.996 | 0.60x |
| flat.json | ujson | 0.987 | 0.995 | 1.014 | 67.996 | 0.24x |
| flat.json | json | 1.711 | 1.723 | 1.736 | 67.996 | 0.14x |
| nested.json | strata | 0.212 | 0.216 | 0.227 | 67.996 | 1.00x |
| nested.json | orjson | 0.280 | 0.293 | 0.305 | 67.996 | 0.74x |
| nested.json | msgspec | 0.363 | 0.369 | 0.391 | 67.996 | 0.59x |
| nested.json | ujson | 1.074 | 1.079 | 1.084 | 67.996 | 0.20x |
| nested.json | json | 2.134 | 2.166 | 2.200 | 67.996 | 0.10x |
| wide_arrays.json | strata | 1.332 | 1.345 | 1.355 | 69.566 | 1.00x |
| wide_arrays.json | orjson | 1.585 | 1.601 | 1.614 | 69.566 | 0.84x |
| wide_arrays.json | msgspec | 2.355 | 2.364 | 2.381 | 69.566 | 0.57x |
| wide_arrays.json | ujson | 4.746 | 4.765 | 4.782 | 69.566 | 0.28x |
| wide_arrays.json | json | 13.525 | 13.546 | 13.574 | 69.566 | 0.10x |
| mixed.json | strata | 0.060 | 0.062 | 0.075 | 69.566 | 1.00x |
| mixed.json | orjson | 0.062 | 0.064 | 0.067 | 69.566 | 0.96x |
| mixed.json | msgspec | 0.077 | 0.078 | 0.090 | 69.566 | 0.79x |
| mixed.json | ujson | 0.239 | 0.241 | 0.256 | 69.566 | 0.26x |
| mixed.json | json | 0.478 | 0.496 | 0.510 | 69.566 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.969 | 9.161 | 9.952 | 68.426 | 1.00x |
| users.json | orjson | 11.804 | 12.133 | 12.635 | 68.426 | 0.76x |
| users.json | msgspec | 12.508 | 12.673 | 13.199 | 68.426 | 0.72x |
| users.json | ujson | 17.086 | 17.432 | 18.695 | 68.426 | 0.53x |
| users.json | json | 20.820 | 21.151 | 21.693 | 68.426 | 0.43x |
| flat.json | strata | 0.843 | 0.860 | 0.913 | 67.996 | 1.00x |
| flat.json | orjson | 0.944 | 0.951 | 0.994 | 67.996 | 0.90x |
| flat.json | msgspec | 0.972 | 0.983 | 1.017 | 67.996 | 0.88x |
| flat.json | ujson | 1.523 | 1.540 | 1.599 | 67.996 | 0.56x |
| flat.json | json | 1.821 | 1.833 | 1.860 | 67.996 | 0.47x |
| nested.json | strata | 0.830 | 0.853 | 0.863 | 67.996 | 1.00x |
| nested.json | orjson | 0.943 | 0.951 | 0.961 | 67.996 | 0.90x |
| nested.json | msgspec | 1.043 | 1.054 | 1.115 | 67.996 | 0.81x |
| nested.json | ujson | 1.472 | 1.482 | 1.539 | 67.996 | 0.58x |
| nested.json | json | 2.019 | 2.030 | 2.073 | 67.996 | 0.42x |
| wide_arrays.json | strata | 3.831 | 3.865 | 3.907 | 69.566 | 1.00x |
| wide_arrays.json | orjson | 4.031 | 4.090 | 4.169 | 69.566 | 0.95x |
| wide_arrays.json | msgspec | 5.041 | 5.078 | 5.148 | 69.566 | 0.76x |
| wide_arrays.json | ujson | 6.601 | 6.657 | 6.700 | 69.566 | 0.58x |
| wide_arrays.json | json | 9.444 | 9.520 | 9.607 | 69.566 | 0.41x |
| mixed.json | strata | 0.212 | 0.215 | 0.246 | 69.566 | 1.00x |
| mixed.json | orjson | 0.274 | 0.279 | 0.294 | 69.566 | 0.77x |
| mixed.json | msgspec | 0.291 | 0.296 | 0.312 | 69.566 | 0.73x |
| mixed.json | ujson | 0.374 | 0.378 | 0.401 | 69.566 | 0.57x |
| mixed.json | json | 0.508 | 0.520 | 0.530 | 69.566 | 0.41x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.216 | 9.361 | 9.542 | 67.988 | 1.00x |
| users.ndjson | orjson | 14.546 | 14.671 | 14.837 | 67.988 | 0.64x |
| users.ndjson | msgspec | 14.926 | 15.065 | 15.498 | 67.988 | 0.62x |
| users.ndjson | ujson | 19.386 | 19.598 | 20.142 | 67.988 | 0.48x |
| users.ndjson | json | 25.453 | 26.005 | 26.451 | 67.988 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.444 | 2.485 | 2.553 | 68.426 | 1.00x |
| users.json | orjson | 3.149 | 3.196 | 3.285 | 68.426 | 0.78x |
| users.json | msgspec | 3.867 | 3.940 | 4.059 | 68.426 | 0.63x |
| users.json | ujson | 11.202 | 11.259 | 11.394 | 68.426 | 0.22x |
| users.json | json | 19.712 | 19.819 | 19.948 | 68.426 | 0.13x |
| flat.json | strata | 0.413 | 0.451 | 0.500 | 67.996 | 1.00x |
| flat.json | orjson | 0.522 | 0.533 | 0.555 | 67.996 | 0.85x |
| flat.json | msgspec | 0.612 | 0.632 | 0.703 | 67.996 | 0.71x |
| flat.json | ujson | 1.233 | 1.257 | 1.313 | 67.996 | 0.36x |
| flat.json | json | 1.962 | 1.976 | 2.028 | 67.996 | 0.23x |
| nested.json | strata | 0.384 | 0.402 | 0.420 | 67.996 | 1.00x |
| nested.json | orjson | 0.493 | 0.511 | 0.526 | 67.996 | 0.79x |
| nested.json | msgspec | 0.574 | 0.600 | 0.664 | 67.996 | 0.67x |
| nested.json | ujson | 1.303 | 1.326 | 1.359 | 67.996 | 0.30x |
| nested.json | json | 2.368 | 2.410 | 2.502 | 67.996 | 0.17x |
| wide_arrays.json | strata | 1.724 | 1.770 | 1.818 | 69.566 | 1.00x |
| wide_arrays.json | orjson | 2.028 | 2.060 | 2.217 | 69.566 | 0.86x |
| wide_arrays.json | msgspec | 2.771 | 2.834 | 2.887 | 69.566 | 0.62x |
| wide_arrays.json | ujson | 5.239 | 5.333 | 5.376 | 69.566 | 0.33x |
| wide_arrays.json | json | 14.032 | 14.110 | 14.209 | 69.566 | 0.13x |
| mixed.json | strata | 0.188 | 0.220 | 0.252 | 69.566 | 1.00x |
| mixed.json | orjson | 0.225 | 0.256 | 0.277 | 69.566 | 0.86x |
| mixed.json | msgspec | 0.227 | 0.258 | 0.290 | 69.566 | 0.85x |
| mixed.json | ujson | 0.405 | 0.425 | 0.501 | 69.566 | 0.52x |
| mixed.json | json | 0.656 | 0.685 | 0.696 | 69.566 | 0.32x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.100 | 0.103 | 0.117 | 68.426 | 1.00x |
| users.json $[*].id | jmespath | 0.469 | 0.488 | 0.497 | 68.426 | 0.21x |
| users.json $[*].id | jsonpath-ng | 2.430 | 2.482 | 2.550 | 68.426 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.619 | 0.641 | 0.657 | 68.555 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.965 | 2.993 | 3.045 | 68.555 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.473 | 17.879 | 18.186 | 68.555 | 0.04x |
| users.json $..total | strata | 1.694 | 1.717 | 1.730 | 69.562 | 1.00x |
| users.json $..total | jsonpath-ng | 297.460 | 298.086 | 298.486 | 69.562 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.165 | 3.196 | 3.219 | 68.555 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.338 | 12.662 | 12.984 | 68.555 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 14.269 | 14.425 | 14.748 | 68.555 | 0.22x |
| users.json $[*].orders[*].total | strata | 3.345 | 3.355 | 3.385 | 69.562 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.124 | 15.338 | 15.659 | 69.562 | 0.22x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 33.418 | 33.540 | 33.711 | 69.562 | 0.10x |
| users.json $..total | strata | 11.224 | 11.456 | 11.770 | 69.625 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 311.150 | 311.783 | 313.672 | 69.625 | 0.04x |

