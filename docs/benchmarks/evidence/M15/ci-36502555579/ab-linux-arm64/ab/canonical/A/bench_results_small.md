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
| users.json | strata | 8.933 | 9.060 | 9.334 | 57.348 | 1.00x |
| users.json | orjson | 11.544 | 11.721 | 11.868 | 57.348 | 0.77x |
| users.json | msgspec | 12.064 | 12.266 | 12.507 | 57.348 | 0.74x |
| users.json | ujson | 16.209 | 16.444 | 17.206 | 57.348 | 0.55x |
| users.json | pysimdjson | 16.199 | 16.478 | 16.914 | 57.348 | 0.55x |
| users.json | json | 20.332 | 20.544 | 21.051 | 57.348 | 0.44x |
| flat.json | strata | 0.871 | 0.915 | 0.958 | 59.348 | 1.00x |
| flat.json | orjson | 0.889 | 0.944 | 0.991 | 59.348 | 0.97x |
| flat.json | msgspec | 0.923 | 0.969 | 1.018 | 59.348 | 0.94x |
| flat.json | ujson | 1.473 | 1.537 | 1.597 | 59.348 | 0.59x |
| flat.json | pysimdjson | 1.502 | 1.583 | 1.659 | 59.348 | 0.58x |
| flat.json | json | 1.788 | 1.846 | 1.900 | 59.348 | 0.50x |
| nested.json | strata | 0.814 | 0.832 | 0.846 | 59.348 | 1.00x |
| nested.json | orjson | 0.872 | 0.887 | 0.898 | 59.348 | 0.94x |
| nested.json | msgspec | 0.989 | 1.000 | 1.017 | 59.348 | 0.83x |
| nested.json | ujson | 1.385 | 1.414 | 1.437 | 59.348 | 0.59x |
| nested.json | pysimdjson | 1.393 | 1.412 | 1.433 | 59.348 | 0.59x |
| nested.json | json | 1.962 | 1.979 | 1.993 | 59.348 | 0.42x |
| wide_arrays.json | strata | 3.879 | 3.937 | 3.994 | 65.941 | 1.00x |
| wide_arrays.json | orjson | 4.024 | 4.092 | 4.155 | 65.941 | 0.96x |
| wide_arrays.json | msgspec | 5.057 | 5.093 | 5.144 | 65.941 | 0.77x |
| wide_arrays.json | ujson | 6.474 | 6.531 | 6.581 | 65.941 | 0.60x |
| wide_arrays.json | pysimdjson | 5.218 | 5.285 | 5.341 | 65.941 | 0.74x |
| wide_arrays.json | json | 9.445 | 9.508 | 9.573 | 65.941 | 0.41x |
| mixed.json | strata | 0.192 | 0.198 | 0.217 | 65.941 | 1.00x |
| mixed.json | orjson | 0.212 | 0.219 | 0.236 | 65.941 | 0.90x |
| mixed.json | msgspec | 0.229 | 0.238 | 0.253 | 65.941 | 0.83x |
| mixed.json | ujson | 0.299 | 0.319 | 0.333 | 65.941 | 0.62x |
| mixed.json | pysimdjson | 0.289 | 0.301 | 0.317 | 65.941 | 0.66x |
| mixed.json | json | 0.447 | 0.470 | 0.485 | 65.941 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.958 | 1.983 | 2.011 | 46.004 | 1.00x |
| users.json | orjson | 2.611 | 2.629 | 2.650 | 46.004 | 0.75x |
| users.json | msgspec | 3.335 | 3.368 | 3.389 | 46.004 | 0.59x |
| users.json | ujson | 10.560 | 10.666 | 10.820 | 46.004 | 0.19x |
| users.json | json | 19.204 | 19.359 | 19.531 | 46.004 | 0.10x |
| flat.json | strata | 0.237 | 0.242 | 0.284 | 59.348 | 1.00x |
| flat.json | orjson | 0.303 | 0.309 | 0.359 | 59.348 | 0.78x |
| flat.json | msgspec | 0.394 | 0.404 | 0.455 | 59.348 | 0.60x |
| flat.json | ujson | 0.995 | 1.010 | 1.069 | 59.348 | 0.24x |
| flat.json | json | 1.701 | 1.736 | 1.834 | 59.348 | 0.14x |
| nested.json | strata | 0.216 | 0.222 | 0.237 | 59.352 | 1.00x |
| nested.json | orjson | 0.280 | 0.287 | 0.304 | 59.352 | 0.77x |
| nested.json | msgspec | 0.366 | 0.372 | 0.394 | 59.352 | 0.60x |
| nested.json | ujson | 1.069 | 1.076 | 1.097 | 59.352 | 0.21x |
| nested.json | json | 2.117 | 2.158 | 2.184 | 59.352 | 0.10x |
| wide_arrays.json | strata | 1.298 | 1.313 | 1.340 | 65.941 | 1.00x |
| wide_arrays.json | orjson | 1.571 | 1.596 | 1.615 | 65.941 | 0.82x |
| wide_arrays.json | msgspec | 2.349 | 2.370 | 2.391 | 65.941 | 0.55x |
| wide_arrays.json | ujson | 4.741 | 4.782 | 4.812 | 65.941 | 0.27x |
| wide_arrays.json | json | 13.516 | 13.563 | 13.626 | 65.941 | 0.10x |
| mixed.json | strata | 0.059 | 0.063 | 0.076 | 65.941 | 1.00x |
| mixed.json | orjson | 0.062 | 0.064 | 0.076 | 65.941 | 0.98x |
| mixed.json | msgspec | 0.076 | 0.079 | 0.097 | 65.941 | 0.80x |
| mixed.json | ujson | 0.234 | 0.238 | 0.262 | 65.941 | 0.26x |
| mixed.json | json | 0.471 | 0.491 | 0.512 | 65.941 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.104 | 9.241 | 9.398 | 60.715 | 1.00x |
| users.json | orjson | 11.724 | 11.942 | 12.186 | 60.715 | 0.77x |
| users.json | msgspec | 12.242 | 12.465 | 12.710 | 60.715 | 0.74x |
| users.json | ujson | 16.662 | 16.900 | 17.378 | 60.715 | 0.55x |
| users.json | json | 20.545 | 20.766 | 21.075 | 60.715 | 0.44x |
| flat.json | strata | 0.831 | 0.864 | 0.892 | 59.348 | 1.00x |
| flat.json | orjson | 0.921 | 0.938 | 0.958 | 59.348 | 0.92x |
| flat.json | msgspec | 0.959 | 0.987 | 1.001 | 59.348 | 0.87x |
| flat.json | ujson | 1.508 | 1.540 | 1.568 | 59.348 | 0.56x |
| flat.json | json | 1.817 | 1.839 | 1.864 | 59.348 | 0.47x |
| nested.json | strata | 0.850 | 0.880 | 0.898 | 59.352 | 1.00x |
| nested.json | orjson | 0.932 | 0.971 | 0.998 | 59.352 | 0.91x |
| nested.json | msgspec | 1.051 | 1.084 | 1.111 | 59.352 | 0.81x |
| nested.json | ujson | 1.470 | 1.520 | 1.569 | 59.352 | 0.58x |
| nested.json | json | 2.011 | 2.055 | 2.089 | 59.352 | 0.43x |
| wide_arrays.json | strata | 3.932 | 3.967 | 4.033 | 65.941 | 1.00x |
| wide_arrays.json | orjson | 4.057 | 4.126 | 4.238 | 65.941 | 0.96x |
| wide_arrays.json | msgspec | 5.077 | 5.132 | 5.250 | 65.941 | 0.77x |
| wide_arrays.json | ujson | 6.618 | 6.685 | 6.832 | 65.941 | 0.59x |
| wide_arrays.json | json | 9.481 | 9.554 | 9.698 | 65.941 | 0.42x |
| mixed.json | strata | 0.216 | 0.223 | 0.248 | 65.941 | 1.00x |
| mixed.json | orjson | 0.276 | 0.289 | 0.317 | 65.941 | 0.77x |
| mixed.json | msgspec | 0.294 | 0.307 | 0.331 | 65.941 | 0.72x |
| mixed.json | ujson | 0.375 | 0.397 | 0.427 | 65.941 | 0.56x |
| mixed.json | json | 0.502 | 0.520 | 0.547 | 65.941 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.012 | 10.717 | 11.128 | 59.340 | 1.00x |
| users.ndjson | orjson | 15.210 | 16.168 | 16.996 | 59.340 | 0.66x |
| users.ndjson | msgspec | 15.497 | 16.367 | 17.326 | 59.340 | 0.65x |
| users.ndjson | ujson | 20.437 | 21.657 | 22.798 | 59.340 | 0.49x |
| users.ndjson | json | 26.204 | 27.933 | 28.873 | 59.340 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.467 | 2.637 | 2.856 | 60.715 | 1.00x |
| users.json | orjson | 3.154 | 3.296 | 3.538 | 60.715 | 0.80x |
| users.json | msgspec | 3.892 | 4.007 | 4.231 | 60.715 | 0.66x |
| users.json | ujson | 11.278 | 11.551 | 11.845 | 60.715 | 0.23x |
| users.json | json | 19.621 | 20.249 | 20.668 | 60.715 | 0.13x |
| flat.json | strata | 0.458 | 0.507 | 0.549 | 59.348 | 1.00x |
| flat.json | orjson | 0.550 | 0.595 | 0.631 | 59.348 | 0.85x |
| flat.json | msgspec | 0.645 | 0.694 | 0.733 | 59.348 | 0.73x |
| flat.json | ujson | 1.274 | 1.322 | 1.350 | 59.348 | 0.38x |
| flat.json | json | 1.997 | 2.046 | 2.092 | 59.348 | 0.25x |
| nested.json | strata | 0.416 | 0.454 | 0.495 | 59.352 | 1.00x |
| nested.json | orjson | 0.525 | 0.566 | 0.619 | 59.352 | 0.80x |
| nested.json | msgspec | 0.609 | 0.652 | 0.697 | 59.352 | 0.70x |
| nested.json | ujson | 1.365 | 1.409 | 1.456 | 59.352 | 0.32x |
| nested.json | json | 2.399 | 2.451 | 2.502 | 59.352 | 0.19x |
| wide_arrays.json | strata | 1.751 | 1.835 | 1.958 | 65.941 | 1.00x |
| wide_arrays.json | orjson | 2.068 | 2.130 | 2.239 | 65.941 | 0.86x |
| wide_arrays.json | msgspec | 2.820 | 2.884 | 2.953 | 65.941 | 0.64x |
| wide_arrays.json | ujson | 5.260 | 5.345 | 5.440 | 65.941 | 0.34x |
| wide_arrays.json | json | 14.041 | 14.139 | 14.401 | 65.941 | 0.13x |
| mixed.json | strata | 0.225 | 0.275 | 0.335 | 65.941 | 1.00x |
| mixed.json | orjson | 0.263 | 0.327 | 0.385 | 65.941 | 0.84x |
| mixed.json | msgspec | 0.292 | 0.332 | 0.384 | 65.941 | 0.83x |
| mixed.json | ujson | 0.463 | 0.517 | 0.579 | 65.941 | 0.53x |
| mixed.json | json | 0.708 | 0.753 | 0.852 | 65.941 | 0.37x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.110 | 0.115 | 0.127 | 60.715 | 1.00x |
| users.json $[*].id | jmespath | 0.487 | 0.501 | 0.518 | 60.715 | 0.23x |
| users.json $[*].id | jsonpath-ng | 2.518 | 2.641 | 2.674 | 60.715 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.604 | 0.628 | 0.643 | 60.832 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.980 | 3.003 | 3.045 | 60.832 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.285 | 17.751 | 18.021 | 60.832 | 0.04x |
| users.json $..total | strata | 1.738 | 1.767 | 1.785 | 60.973 | 1.00x |
| users.json $..total | jsonpath-ng | 293.983 | 298.225 | 298.758 | 60.973 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.182 | 3.253 | 3.289 | 60.832 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.531 | 13.686 | 14.088 | 60.832 | 0.24x |
| users.json $[*].id | orjson+jsonpath-ng | 14.582 | 15.806 | 16.198 | 60.832 | 0.21x |
| users.json $[*].orders[*].total | strata | 3.301 | 3.356 | 3.748 | 60.973 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 14.910 | 15.247 | 16.134 | 60.973 | 0.22x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 33.387 | 34.061 | 35.238 | 60.973 | 0.10x |
| users.json $..total | strata | 11.474 | 13.233 | 14.349 | 60.977 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 312.066 | 319.440 | 323.069 | 60.977 | 0.04x |

