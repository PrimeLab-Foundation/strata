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
| users.json | strata | 8.837 | 8.951 | 10.885 | 57.258 | 1.00x |
| users.json | orjson | 11.683 | 11.829 | 13.764 | 57.258 | 0.76x |
| users.json | msgspec | 12.192 | 12.289 | 13.756 | 57.258 | 0.73x |
| users.json | ujson | 16.719 | 16.927 | 19.196 | 57.258 | 0.53x |
| users.json | pysimdjson | 16.769 | 17.116 | 18.980 | 57.258 | 0.52x |
| users.json | json | 20.754 | 20.883 | 21.652 | 57.258 | 0.43x |
| flat.json | strata | 0.820 | 0.833 | 0.845 | 68.000 | 1.00x |
| flat.json | orjson | 0.848 | 0.870 | 0.879 | 68.000 | 0.96x |
| flat.json | msgspec | 0.910 | 0.921 | 0.942 | 68.000 | 0.91x |
| flat.json | ujson | 1.457 | 1.476 | 1.482 | 68.000 | 0.56x |
| flat.json | pysimdjson | 1.489 | 1.495 | 1.523 | 68.000 | 0.56x |
| flat.json | json | 1.801 | 1.805 | 1.831 | 68.000 | 0.46x |
| nested.json | strata | 0.797 | 0.815 | 0.828 | 68.000 | 1.00x |
| nested.json | orjson | 0.864 | 0.890 | 0.902 | 68.000 | 0.92x |
| nested.json | msgspec | 0.988 | 0.995 | 1.011 | 68.000 | 0.82x |
| nested.json | ujson | 1.395 | 1.404 | 1.443 | 68.000 | 0.58x |
| nested.json | pysimdjson | 1.386 | 1.392 | 1.410 | 68.000 | 0.59x |
| nested.json | json | 1.948 | 1.974 | 1.989 | 68.000 | 0.41x |
| wide_arrays.json | strata | 3.907 | 3.939 | 3.974 | 69.570 | 1.00x |
| wide_arrays.json | orjson | 4.085 | 4.107 | 4.214 | 69.570 | 0.96x |
| wide_arrays.json | msgspec | 5.082 | 5.105 | 5.154 | 69.570 | 0.77x |
| wide_arrays.json | ujson | 6.514 | 6.592 | 6.616 | 69.570 | 0.60x |
| wide_arrays.json | pysimdjson | 5.338 | 5.366 | 5.506 | 69.570 | 0.73x |
| wide_arrays.json | json | 9.536 | 9.652 | 9.783 | 69.570 | 0.41x |
| mixed.json | strata | 0.191 | 0.194 | 0.223 | 69.570 | 1.00x |
| mixed.json | orjson | 0.212 | 0.216 | 0.232 | 69.570 | 0.90x |
| mixed.json | msgspec | 0.235 | 0.238 | 0.260 | 69.570 | 0.82x |
| mixed.json | ujson | 0.307 | 0.316 | 0.339 | 69.570 | 0.62x |
| mixed.json | pysimdjson | 0.295 | 0.299 | 0.318 | 69.570 | 0.65x |
| mixed.json | json | 0.455 | 0.473 | 0.483 | 69.570 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.922 | 1.934 | 1.949 | 56.367 | 1.00x |
| users.json | orjson | 2.585 | 2.601 | 2.607 | 56.367 | 0.74x |
| users.json | msgspec | 3.324 | 3.333 | 3.365 | 56.367 | 0.58x |
| users.json | ujson | 10.549 | 10.600 | 10.646 | 56.367 | 0.18x |
| users.json | json | 19.078 | 19.130 | 19.174 | 56.367 | 0.10x |
| flat.json | strata | 0.234 | 0.238 | 0.261 | 68.000 | 1.00x |
| flat.json | orjson | 0.301 | 0.307 | 0.325 | 68.000 | 0.78x |
| flat.json | msgspec | 0.391 | 0.403 | 0.415 | 68.000 | 0.59x |
| flat.json | ujson | 1.000 | 1.004 | 1.014 | 68.000 | 0.24x |
| flat.json | json | 1.719 | 1.733 | 1.741 | 68.000 | 0.14x |
| nested.json | strata | 0.218 | 0.223 | 0.242 | 68.000 | 1.00x |
| nested.json | orjson | 0.283 | 0.285 | 0.306 | 68.000 | 0.78x |
| nested.json | msgspec | 0.368 | 0.372 | 0.395 | 68.000 | 0.60x |
| nested.json | ujson | 1.075 | 1.084 | 1.113 | 68.000 | 0.21x |
| nested.json | json | 2.158 | 2.203 | 2.220 | 68.000 | 0.10x |
| wide_arrays.json | strata | 1.364 | 1.385 | 1.414 | 69.570 | 1.00x |
| wide_arrays.json | orjson | 1.602 | 1.627 | 1.641 | 69.570 | 0.85x |
| wide_arrays.json | msgspec | 2.367 | 2.379 | 2.403 | 69.570 | 0.58x |
| wide_arrays.json | ujson | 4.773 | 4.802 | 4.840 | 69.570 | 0.29x |
| wide_arrays.json | json | 13.643 | 13.720 | 13.809 | 69.570 | 0.10x |
| mixed.json | strata | 0.062 | 0.065 | 0.083 | 69.570 | 1.00x |
| mixed.json | orjson | 0.064 | 0.066 | 0.083 | 69.570 | 0.98x |
| mixed.json | msgspec | 0.079 | 0.082 | 0.091 | 69.570 | 0.79x |
| mixed.json | ujson | 0.238 | 0.243 | 0.263 | 69.570 | 0.27x |
| mixed.json | json | 0.486 | 0.498 | 0.516 | 69.570 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.284 | 9.390 | 10.190 | 68.434 | 1.00x |
| users.json | orjson | 12.140 | 12.318 | 14.646 | 68.434 | 0.76x |
| users.json | msgspec | 12.781 | 12.921 | 14.789 | 68.434 | 0.73x |
| users.json | ujson | 17.510 | 17.822 | 22.701 | 68.434 | 0.53x |
| users.json | json | 21.224 | 21.437 | 23.165 | 68.434 | 0.44x |
| flat.json | strata | 0.868 | 0.881 | 0.895 | 68.000 | 1.00x |
| flat.json | orjson | 0.948 | 0.957 | 0.978 | 68.000 | 0.92x |
| flat.json | msgspec | 0.994 | 1.010 | 1.031 | 68.000 | 0.87x |
| flat.json | ujson | 1.571 | 1.581 | 1.597 | 68.000 | 0.56x |
| flat.json | json | 1.847 | 1.865 | 1.875 | 68.000 | 0.47x |
| nested.json | strata | 0.843 | 0.850 | 0.862 | 68.000 | 1.00x |
| nested.json | orjson | 0.951 | 0.959 | 0.968 | 68.000 | 0.89x |
| nested.json | msgspec | 1.052 | 1.069 | 1.079 | 68.000 | 0.80x |
| nested.json | ujson | 1.496 | 1.512 | 1.538 | 68.000 | 0.56x |
| nested.json | json | 2.005 | 2.024 | 2.065 | 68.000 | 0.42x |
| wide_arrays.json | strata | 3.893 | 3.938 | 4.050 | 69.570 | 1.00x |
| wide_arrays.json | orjson | 4.132 | 4.187 | 4.380 | 69.570 | 0.94x |
| wide_arrays.json | msgspec | 5.143 | 5.251 | 5.325 | 69.570 | 0.75x |
| wide_arrays.json | ujson | 6.695 | 6.811 | 6.983 | 69.570 | 0.58x |
| wide_arrays.json | json | 9.631 | 9.741 | 9.903 | 69.570 | 0.40x |
| mixed.json | strata | 0.216 | 0.220 | 0.238 | 69.570 | 1.00x |
| mixed.json | orjson | 0.278 | 0.282 | 0.298 | 69.570 | 0.78x |
| mixed.json | msgspec | 0.297 | 0.304 | 0.320 | 69.570 | 0.73x |
| mixed.json | ujson | 0.391 | 0.397 | 0.408 | 69.570 | 0.56x |
| mixed.json | json | 0.515 | 0.532 | 0.551 | 69.570 | 0.41x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.488 | 9.812 | 9.938 | 67.992 | 1.00x |
| users.ndjson | orjson | 14.971 | 15.259 | 15.410 | 67.992 | 0.64x |
| users.ndjson | msgspec | 15.313 | 15.486 | 15.626 | 67.992 | 0.63x |
| users.ndjson | ujson | 19.861 | 20.267 | 20.556 | 67.992 | 0.48x |
| users.ndjson | json | 26.454 | 26.620 | 26.932 | 67.992 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.430 | 2.492 | 2.521 | 68.434 | 1.00x |
| users.json | orjson | 3.162 | 3.222 | 3.260 | 68.434 | 0.77x |
| users.json | msgspec | 3.892 | 3.927 | 3.989 | 68.434 | 0.63x |
| users.json | ujson | 11.281 | 11.336 | 11.394 | 68.434 | 0.22x |
| users.json | json | 19.837 | 19.943 | 20.224 | 68.434 | 0.12x |
| flat.json | strata | 0.397 | 0.428 | 0.460 | 68.000 | 1.00x |
| flat.json | orjson | 0.486 | 0.526 | 0.559 | 68.000 | 0.81x |
| flat.json | msgspec | 0.611 | 0.635 | 0.649 | 68.000 | 0.67x |
| flat.json | ujson | 1.225 | 1.249 | 1.282 | 68.000 | 0.34x |
| flat.json | json | 1.949 | 1.977 | 1.988 | 68.000 | 0.22x |
| nested.json | strata | 0.360 | 0.379 | 0.422 | 68.000 | 1.00x |
| nested.json | orjson | 0.452 | 0.490 | 0.536 | 68.000 | 0.77x |
| nested.json | msgspec | 0.546 | 0.573 | 0.610 | 68.000 | 0.66x |
| nested.json | ujson | 1.289 | 1.322 | 1.369 | 68.000 | 0.29x |
| nested.json | json | 2.350 | 2.374 | 2.434 | 68.000 | 0.16x |
| wide_arrays.json | strata | 1.735 | 1.789 | 1.840 | 69.570 | 1.00x |
| wide_arrays.json | orjson | 2.027 | 2.097 | 2.155 | 69.570 | 0.85x |
| wide_arrays.json | msgspec | 2.810 | 2.839 | 2.857 | 69.570 | 0.63x |
| wide_arrays.json | ujson | 5.251 | 5.278 | 5.302 | 69.570 | 0.34x |
| wide_arrays.json | json | 14.109 | 14.188 | 14.243 | 69.570 | 0.13x |
| mixed.json | strata | 0.176 | 0.182 | 0.198 | 69.570 | 1.00x |
| mixed.json | orjson | 0.196 | 0.203 | 0.227 | 69.570 | 0.90x |
| mixed.json | msgspec | 0.205 | 0.213 | 0.246 | 69.570 | 0.86x |
| mixed.json | ujson | 0.387 | 0.397 | 0.430 | 69.570 | 0.46x |
| mixed.json | json | 0.634 | 0.650 | 0.660 | 69.570 | 0.28x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.108 | 0.110 | 0.128 | 68.434 | 1.00x |
| users.json $[*].id | jmespath | 0.479 | 0.485 | 0.507 | 68.434 | 0.23x |
| users.json $[*].id | jsonpath-ng | 2.465 | 2.558 | 2.607 | 68.434 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.613 | 0.632 | 0.648 | 68.559 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.981 | 3.002 | 3.027 | 68.559 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.771 | 18.419 | 18.913 | 68.559 | 0.03x |
| users.json $..total | strata | 1.705 | 1.722 | 1.725 | 69.566 | 1.00x |
| users.json $..total | jsonpath-ng | 294.601 | 294.953 | 295.656 | 69.566 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.190 | 3.205 | 3.216 | 68.559 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.784 | 12.946 | 13.189 | 68.559 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 14.623 | 14.828 | 14.960 | 68.559 | 0.22x |
| users.json $[*].orders[*].total | strata | 3.370 | 3.392 | 3.420 | 69.566 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.665 | 15.871 | 16.209 | 69.566 | 0.21x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 34.406 | 34.938 | 35.282 | 69.566 | 0.10x |
| users.json $..total | strata | 11.800 | 12.152 | 12.604 | 69.629 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 314.493 | 315.901 | 317.061 | 69.629 | 0.04x |

