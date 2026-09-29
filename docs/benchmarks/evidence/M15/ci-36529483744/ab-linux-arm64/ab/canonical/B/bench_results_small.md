# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: afd1550cbabc9433e8292644444a23bb27bbc4e9
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-aarch64-with-glibc2.39
- machine: aarch64
- processor: aarch64
- compiler_flags: -fno-strict-overflow -Wsign-compare -DNDEBUG -g -O3 -Wall -fPIC -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -march=native -flto=thin -fprofile-use=/home/runner/work/_temp/strata-arm/build/pgo/strata.profdata -shared -Wl,--rpath=/opt/hostedtoolcache/Python/3.12.14/arm64/lib (24 recorded commands; see the JSON companion)
- repeats: 60
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.882 | 9.098 | 9.270 | 57.402 | 1.00x |
| users.json | orjson | 11.453 | 11.804 | 12.138 | 57.402 | 0.77x |
| users.json | msgspec | 11.996 | 12.305 | 12.629 | 57.402 | 0.74x |
| users.json | ujson | 16.130 | 16.687 | 17.190 | 57.402 | 0.55x |
| users.json | pysimdjson | 16.150 | 16.659 | 17.106 | 57.402 | 0.55x |
| users.json | json | 20.263 | 20.678 | 21.112 | 57.402 | 0.44x |
| flat.json | strata | 0.849 | 0.880 | 0.901 | 68.246 | 1.00x |
| flat.json | orjson | 0.861 | 0.891 | 0.906 | 68.246 | 0.99x |
| flat.json | msgspec | 0.915 | 0.946 | 0.959 | 68.246 | 0.93x |
| flat.json | ujson | 1.450 | 1.483 | 1.506 | 68.246 | 0.59x |
| flat.json | pysimdjson | 1.496 | 1.516 | 1.542 | 68.246 | 0.58x |
| flat.json | json | 1.775 | 1.799 | 1.823 | 68.246 | 0.49x |
| nested.json | strata | 0.814 | 0.841 | 0.854 | 68.246 | 1.00x |
| nested.json | orjson | 0.873 | 0.899 | 0.914 | 68.246 | 0.94x |
| nested.json | msgspec | 0.986 | 1.001 | 1.012 | 68.246 | 0.84x |
| nested.json | ujson | 1.416 | 1.445 | 1.467 | 68.246 | 0.58x |
| nested.json | pysimdjson | 1.395 | 1.420 | 1.452 | 68.246 | 0.59x |
| nested.json | json | 1.967 | 1.991 | 2.021 | 68.246 | 0.42x |
| wide_arrays.json | strata | 3.939 | 4.071 | 4.182 | 70.617 | 1.00x |
| wide_arrays.json | orjson | 4.066 | 4.168 | 4.406 | 70.617 | 0.98x |
| wide_arrays.json | msgspec | 5.039 | 5.113 | 5.277 | 70.617 | 0.80x |
| wide_arrays.json | ujson | 6.462 | 6.595 | 6.696 | 70.617 | 0.62x |
| wide_arrays.json | pysimdjson | 5.249 | 5.400 | 5.623 | 70.617 | 0.75x |
| wide_arrays.json | json | 9.521 | 9.689 | 9.863 | 70.617 | 0.42x |
| mixed.json | strata | 0.189 | 0.201 | 0.246 | 70.617 | 1.00x |
| mixed.json | orjson | 0.212 | 0.222 | 0.282 | 70.617 | 0.91x |
| mixed.json | msgspec | 0.229 | 0.242 | 0.302 | 70.617 | 0.83x |
| mixed.json | ujson | 0.301 | 0.318 | 0.381 | 70.617 | 0.63x |
| mixed.json | pysimdjson | 0.290 | 0.300 | 0.358 | 70.617 | 0.67x |
| mixed.json | json | 0.450 | 0.473 | 0.548 | 70.617 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.980 | 1.995 | 2.014 | 56.516 | 1.00x |
| users.json | orjson | 2.615 | 2.634 | 2.652 | 56.516 | 0.76x |
| users.json | msgspec | 3.341 | 3.367 | 3.392 | 56.516 | 0.59x |
| users.json | ujson | 10.491 | 10.586 | 10.652 | 56.516 | 0.19x |
| users.json | json | 18.991 | 19.109 | 19.207 | 56.516 | 0.10x |
| flat.json | strata | 0.233 | 0.243 | 0.259 | 68.246 | 1.00x |
| flat.json | orjson | 0.302 | 0.312 | 0.329 | 68.246 | 0.78x |
| flat.json | msgspec | 0.392 | 0.407 | 0.423 | 68.246 | 0.60x |
| flat.json | ujson | 0.993 | 1.009 | 1.022 | 68.246 | 0.24x |
| flat.json | json | 1.681 | 1.718 | 1.742 | 68.246 | 0.14x |
| nested.json | strata | 0.219 | 0.223 | 0.242 | 68.250 | 1.00x |
| nested.json | orjson | 0.286 | 0.290 | 0.311 | 68.250 | 0.77x |
| nested.json | msgspec | 0.375 | 0.393 | 0.405 | 68.250 | 0.57x |
| nested.json | ujson | 1.078 | 1.087 | 1.107 | 68.250 | 0.20x |
| nested.json | json | 2.159 | 2.188 | 2.218 | 68.250 | 0.10x |
| wide_arrays.json | strata | 1.310 | 1.370 | 1.422 | 70.617 | 1.00x |
| wide_arrays.json | orjson | 1.593 | 1.637 | 1.672 | 70.617 | 0.84x |
| wide_arrays.json | msgspec | 2.350 | 2.379 | 2.412 | 70.617 | 0.58x |
| wide_arrays.json | ujson | 4.769 | 4.809 | 4.861 | 70.617 | 0.28x |
| wide_arrays.json | json | 13.540 | 13.696 | 13.799 | 70.617 | 0.10x |
| mixed.json | strata | 0.062 | 0.067 | 0.082 | 70.617 | 1.00x |
| mixed.json | orjson | 0.066 | 0.068 | 0.084 | 70.617 | 0.98x |
| mixed.json | msgspec | 0.079 | 0.083 | 0.096 | 70.617 | 0.81x |
| mixed.json | ujson | 0.241 | 0.246 | 0.268 | 70.617 | 0.27x |
| mixed.json | json | 0.482 | 0.499 | 0.516 | 70.617 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.181 | 9.486 | 9.714 | 69.469 | 1.00x |
| users.json | orjson | 11.802 | 12.230 | 12.627 | 69.469 | 0.78x |
| users.json | msgspec | 12.375 | 12.751 | 13.188 | 69.469 | 0.74x |
| users.json | ujson | 16.733 | 17.532 | 18.028 | 69.469 | 0.54x |
| users.json | json | 20.654 | 21.282 | 21.683 | 69.469 | 0.45x |
| flat.json | strata | 0.876 | 0.925 | 0.957 | 68.246 | 1.00x |
| flat.json | orjson | 0.947 | 0.978 | 1.016 | 68.246 | 0.95x |
| flat.json | msgspec | 0.996 | 1.030 | 1.060 | 68.246 | 0.90x |
| flat.json | ujson | 1.568 | 1.613 | 1.654 | 68.246 | 0.57x |
| flat.json | json | 1.849 | 1.871 | 1.902 | 68.246 | 0.49x |
| nested.json | strata | 0.855 | 0.878 | 0.892 | 68.250 | 1.00x |
| nested.json | orjson | 0.950 | 0.977 | 0.999 | 68.250 | 0.90x |
| nested.json | msgspec | 1.049 | 1.078 | 1.097 | 68.250 | 0.81x |
| nested.json | ujson | 1.478 | 1.518 | 1.556 | 68.250 | 0.58x |
| nested.json | json | 2.015 | 2.039 | 2.072 | 68.250 | 0.43x |
| wide_arrays.json | strata | 3.940 | 4.091 | 4.230 | 70.617 | 1.00x |
| wide_arrays.json | orjson | 4.137 | 4.293 | 4.473 | 70.617 | 0.95x |
| wide_arrays.json | msgspec | 5.093 | 5.269 | 5.494 | 70.617 | 0.78x |
| wide_arrays.json | ujson | 6.663 | 6.859 | 7.155 | 70.617 | 0.60x |
| wide_arrays.json | json | 9.544 | 9.823 | 10.069 | 70.617 | 0.42x |
| mixed.json | strata | 0.222 | 0.231 | 0.256 | 70.617 | 1.00x |
| mixed.json | orjson | 0.283 | 0.306 | 0.331 | 70.617 | 0.76x |
| mixed.json | msgspec | 0.302 | 0.321 | 0.341 | 70.617 | 0.72x |
| mixed.json | ujson | 0.392 | 0.413 | 0.442 | 70.617 | 0.56x |
| mixed.json | json | 0.514 | 0.538 | 0.562 | 70.617 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.827 | 10.499 | 10.810 | 68.234 | 1.00x |
| users.ndjson | orjson | 14.951 | 15.585 | 16.025 | 68.234 | 0.67x |
| users.ndjson | msgspec | 15.310 | 15.893 | 16.352 | 68.234 | 0.66x |
| users.ndjson | ujson | 19.912 | 20.710 | 21.244 | 68.234 | 0.51x |
| users.ndjson | json | 25.998 | 27.304 | 27.665 | 68.234 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.540 | 2.625 | 2.729 | 69.469 | 1.00x |
| users.json | orjson | 3.236 | 3.304 | 3.383 | 69.469 | 0.79x |
| users.json | msgspec | 3.939 | 4.023 | 4.121 | 69.469 | 0.65x |
| users.json | ujson | 11.279 | 11.388 | 11.522 | 69.469 | 0.23x |
| users.json | json | 19.726 | 19.918 | 20.162 | 69.469 | 0.13x |
| flat.json | strata | 0.471 | 0.509 | 0.546 | 68.246 | 1.00x |
| flat.json | orjson | 0.565 | 0.600 | 0.653 | 68.246 | 0.85x |
| flat.json | msgspec | 0.656 | 0.695 | 0.744 | 68.246 | 0.73x |
| flat.json | ujson | 1.286 | 1.332 | 1.366 | 68.246 | 0.38x |
| flat.json | json | 1.987 | 2.020 | 2.051 | 68.246 | 0.25x |
| nested.json | strata | 0.440 | 0.489 | 0.523 | 68.250 | 1.00x |
| nested.json | orjson | 0.555 | 0.603 | 0.651 | 68.250 | 0.81x |
| nested.json | msgspec | 0.649 | 0.687 | 0.729 | 68.250 | 0.71x |
| nested.json | ujson | 1.398 | 1.443 | 1.480 | 68.250 | 0.34x |
| nested.json | json | 2.457 | 2.502 | 2.568 | 68.250 | 0.20x |
| wide_arrays.json | strata | 1.752 | 1.910 | 2.595 | 70.617 | 1.00x |
| wide_arrays.json | orjson | 2.075 | 2.210 | 3.017 | 70.617 | 0.86x |
| wide_arrays.json | msgspec | 2.828 | 2.946 | 3.750 | 70.617 | 0.65x |
| wide_arrays.json | ujson | 5.274 | 5.427 | 6.329 | 70.617 | 0.35x |
| wide_arrays.json | json | 14.010 | 14.367 | 15.149 | 70.617 | 0.13x |
| mixed.json | strata | 0.250 | 0.294 | 0.320 | 70.617 | 1.00x |
| mixed.json | orjson | 0.281 | 0.327 | 0.358 | 70.617 | 0.90x |
| mixed.json | msgspec | 0.299 | 0.323 | 0.368 | 70.617 | 0.91x |
| mixed.json | ujson | 0.465 | 0.502 | 0.557 | 70.617 | 0.59x |
| mixed.json | json | 0.717 | 0.769 | 0.813 | 70.617 | 0.38x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.101 | 0.106 | 0.118 | 69.469 | 1.00x |
| users.json $[*].id | jmespath | 0.463 | 0.475 | 0.498 | 69.469 | 0.22x |
| users.json $[*].id | jsonpath-ng | 2.462 | 2.544 | 2.597 | 69.469 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.604 | 0.628 | 0.651 | 69.613 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.012 | 3.064 | 3.099 | 69.613 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.652 | 18.265 | 18.909 | 69.613 | 0.03x |
| users.json $..total | strata | 1.742 | 1.771 | 1.801 | 69.754 | 1.00x |
| users.json $..total | jsonpath-ng | 294.426 | 295.755 | 296.392 | 69.754 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.185 | 3.225 | 3.262 | 69.613 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.603 | 13.096 | 13.472 | 69.613 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 14.575 | 14.947 | 15.257 | 69.613 | 0.22x |
| users.json $[*].orders[*].total | strata | 3.357 | 3.390 | 3.423 | 69.754 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.246 | 16.133 | 16.460 | 69.754 | 0.21x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 33.919 | 35.112 | 36.106 | 69.754 | 0.10x |
| users.json $..total | strata | 12.331 | 13.175 | 13.802 | 69.871 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 317.646 | 321.132 | 322.882 | 69.871 | 0.04x |

