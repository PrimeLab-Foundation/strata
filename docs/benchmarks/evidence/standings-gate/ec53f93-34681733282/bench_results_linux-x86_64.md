# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: ec53f936b8a9245edf5bfeec36c38539666a1775
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: AMD EPYC 9V74 80-Core Processor
- compiler_flags: -fno-strict-overflow -Wsign-compare -DNDEBUG -g -O3 -Wall -fPIC -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -fomit-frame-pointer -march=native -flto=thin -fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata -fsplit-machine-functions -shared -Wl,--rpath=/opt/hostedtoolcache/Python/3.12.14/x64/lib (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.048 | 10.239 | 14.909 | 66.266 | 1.00x |
| users.json | orjson | 14.053 | 14.185 | 17.881 | 66.266 | 0.72x |
| users.json | msgspec | 13.923 | 14.108 | 17.557 | 66.266 | 0.73x |
| users.json | ujson | 18.141 | 18.572 | 23.524 | 66.266 | 0.55x |
| users.json | pysimdjson | 19.127 | 19.398 | 23.013 | 66.266 | 0.53x |
| users.json | json | 21.429 | 21.579 | 23.059 | 66.266 | 0.47x |
| flat.json | strata | 0.847 | 0.863 | 0.933 | 63.367 | 1.00x |
| flat.json | orjson | 1.024 | 1.034 | 1.040 | 63.367 | 0.84x |
| flat.json | msgspec | 1.028 | 1.051 | 1.059 | 63.367 | 0.82x |
| flat.json | ujson | 1.494 | 1.517 | 1.542 | 63.367 | 0.57x |
| flat.json | pysimdjson | 1.564 | 1.604 | 1.638 | 63.367 | 0.54x |
| flat.json | json | 1.687 | 1.708 | 1.757 | 63.367 | 0.51x |
| nested.json | strata | 0.804 | 0.820 | 0.858 | 63.367 | 1.00x |
| nested.json | orjson | 0.998 | 1.015 | 1.021 | 63.367 | 0.81x |
| nested.json | msgspec | 0.963 | 0.983 | 0.994 | 63.367 | 0.83x |
| nested.json | ujson | 1.417 | 1.430 | 1.460 | 63.367 | 0.57x |
| nested.json | pysimdjson | 1.375 | 1.409 | 1.454 | 63.367 | 0.58x |
| nested.json | json | 1.817 | 1.844 | 1.902 | 63.367 | 0.44x |
| wide_arrays.json | strata | 4.461 | 4.501 | 4.556 | 78.684 | 1.00x |
| wide_arrays.json | orjson | 5.598 | 5.692 | 5.786 | 78.684 | 0.79x |
| wide_arrays.json | msgspec | 6.201 | 6.279 | 6.437 | 78.684 | 0.72x |
| wide_arrays.json | ujson | 7.615 | 7.734 | 7.886 | 78.684 | 0.58x |
| wide_arrays.json | pysimdjson | 6.451 | 6.523 | 6.673 | 78.684 | 0.69x |
| wide_arrays.json | json | 9.859 | 10.007 | 10.078 | 78.684 | 0.45x |
| mixed.json | strata | 0.191 | 0.195 | 0.205 | 78.746 | 1.00x |
| mixed.json | orjson | 0.235 | 0.242 | 0.253 | 78.746 | 0.80x |
| mixed.json | msgspec | 0.243 | 0.249 | 0.266 | 78.746 | 0.78x |
| mixed.json | ujson | 0.309 | 0.318 | 0.331 | 78.746 | 0.61x |
| mixed.json | pysimdjson | 0.306 | 0.309 | 0.327 | 78.746 | 0.63x |
| mixed.json | json | 0.443 | 0.457 | 0.468 | 78.746 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.357 | 2.365 | 2.375 | 47.441 | 1.00x |
| users.json | orjson | 3.089 | 3.092 | 3.156 | 47.441 | 0.76x |
| users.json | msgspec | 4.137 | 4.156 | 4.193 | 47.441 | 0.57x |
| users.json | ujson | 11.320 | 11.451 | 11.538 | 47.441 | 0.21x |
| users.json | json | 22.008 | 22.158 | 22.281 | 47.441 | 0.11x |
| flat.json | strata | 0.298 | 0.318 | 0.322 | 63.367 | 1.00x |
| flat.json | orjson | 0.369 | 0.377 | 0.386 | 63.367 | 0.84x |
| flat.json | msgspec | 0.468 | 0.486 | 0.497 | 63.367 | 0.65x |
| flat.json | ujson | 1.027 | 1.035 | 1.045 | 63.367 | 0.31x |
| flat.json | json | 1.855 | 1.872 | 1.897 | 63.367 | 0.17x |
| nested.json | strata | 0.228 | 0.235 | 0.239 | 63.367 | 1.00x |
| nested.json | orjson | 0.299 | 0.306 | 0.320 | 63.367 | 0.77x |
| nested.json | msgspec | 0.408 | 0.422 | 0.440 | 63.367 | 0.56x |
| nested.json | ujson | 1.070 | 1.085 | 1.095 | 63.367 | 0.22x |
| nested.json | json | 2.349 | 2.373 | 2.432 | 63.367 | 0.10x |
| wide_arrays.json | strata | 1.827 | 1.852 | 1.953 | 78.684 | 1.00x |
| wide_arrays.json | orjson | 1.983 | 2.001 | 2.046 | 78.684 | 0.93x |
| wide_arrays.json | msgspec | 3.076 | 3.115 | 3.146 | 78.684 | 0.59x |
| wide_arrays.json | ujson | 6.452 | 6.475 | 6.536 | 78.684 | 0.29x |
| wide_arrays.json | json | 16.630 | 16.784 | 16.948 | 78.684 | 0.11x |
| mixed.json | strata | 0.063 | 0.065 | 0.079 | 78.746 | 1.00x |
| mixed.json | orjson | 0.068 | 0.071 | 0.081 | 78.746 | 0.91x |
| mixed.json | msgspec | 0.088 | 0.089 | 0.102 | 78.746 | 0.73x |
| mixed.json | ujson | 0.228 | 0.232 | 0.246 | 78.746 | 0.28x |
| mixed.json | json | 0.527 | 0.538 | 0.548 | 78.746 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.679 | 10.925 | 12.728 | 64.965 | 1.00x |
| users.json | orjson | 15.211 | 15.633 | 16.332 | 64.965 | 0.70x |
| users.json | msgspec | 14.771 | 15.260 | 15.503 | 64.965 | 0.72x |
| users.json | ujson | 20.338 | 20.751 | 22.742 | 64.965 | 0.53x |
| users.json | json | 22.062 | 22.354 | 22.926 | 64.965 | 0.49x |
| flat.json | strata | 0.873 | 0.891 | 0.907 | 63.367 | 1.00x |
| flat.json | orjson | 1.082 | 1.102 | 1.157 | 63.367 | 0.81x |
| flat.json | msgspec | 1.094 | 1.114 | 1.130 | 63.367 | 0.80x |
| flat.json | ujson | 1.592 | 1.607 | 1.620 | 63.367 | 0.55x |
| flat.json | json | 1.761 | 1.783 | 1.796 | 63.367 | 0.50x |
| nested.json | strata | 0.857 | 0.875 | 0.892 | 63.367 | 1.00x |
| nested.json | orjson | 1.066 | 1.080 | 1.113 | 63.367 | 0.81x |
| nested.json | msgspec | 1.035 | 1.048 | 1.067 | 63.367 | 0.84x |
| nested.json | ujson | 1.475 | 1.494 | 1.566 | 63.367 | 0.59x |
| nested.json | json | 1.897 | 1.908 | 1.974 | 63.367 | 0.46x |
| wide_arrays.json | strata | 4.454 | 4.528 | 4.651 | 78.746 | 1.00x |
| wide_arrays.json | orjson | 5.698 | 5.720 | 5.888 | 78.746 | 0.79x |
| wide_arrays.json | msgspec | 6.287 | 6.331 | 6.459 | 78.746 | 0.72x |
| wide_arrays.json | ujson | 7.881 | 7.898 | 8.011 | 78.746 | 0.57x |
| wide_arrays.json | json | 9.937 | 9.979 | 10.038 | 78.746 | 0.45x |
| mixed.json | strata | 0.211 | 0.215 | 0.241 | 78.746 | 1.00x |
| mixed.json | orjson | 0.284 | 0.294 | 0.307 | 78.746 | 0.73x |
| mixed.json | msgspec | 0.291 | 0.297 | 0.316 | 78.746 | 0.72x |
| mixed.json | ujson | 0.362 | 0.380 | 0.394 | 78.746 | 0.57x |
| mixed.json | json | 0.493 | 0.499 | 0.516 | 78.746 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.845 | 10.975 | 11.049 | 63.367 | 1.00x |
| users.ndjson | orjson | 18.030 | 18.286 | 18.386 | 63.367 | 0.60x |
| users.ndjson | msgspec | 18.178 | 18.295 | 18.505 | 63.367 | 0.60x |
| users.ndjson | ujson | 23.132 | 23.333 | 23.897 | 63.367 | 0.47x |
| users.ndjson | json | 28.915 | 29.288 | 29.654 | 63.367 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.914 | 2.926 | 2.985 | 64.965 | 1.00x |
| users.json | orjson | 3.697 | 3.718 | 3.781 | 64.965 | 0.79x |
| users.json | msgspec | 4.719 | 4.748 | 4.769 | 64.965 | 0.62x |
| users.json | ujson | 12.008 | 12.096 | 12.182 | 64.965 | 0.24x |
| users.json | json | 23.282 | 23.377 | 23.689 | 64.965 | 0.13x |
| flat.json | strata | 0.452 | 0.462 | 0.474 | 63.367 | 1.00x |
| flat.json | orjson | 0.517 | 0.538 | 0.546 | 63.367 | 0.86x |
| flat.json | msgspec | 0.626 | 0.644 | 0.661 | 63.367 | 0.72x |
| flat.json | ujson | 1.206 | 1.216 | 1.227 | 63.367 | 0.38x |
| flat.json | json | 2.033 | 2.052 | 3.089 | 63.367 | 0.23x |
| nested.json | strata | 0.349 | 0.358 | 0.383 | 63.367 | 1.00x |
| nested.json | orjson | 0.436 | 0.454 | 0.465 | 63.367 | 0.79x |
| nested.json | msgspec | 0.544 | 0.563 | 0.573 | 63.367 | 0.64x |
| nested.json | ujson | 1.222 | 1.239 | 1.260 | 63.367 | 0.29x |
| nested.json | json | 2.524 | 2.549 | 2.638 | 63.367 | 0.14x |
| wide_arrays.json | strata | 2.223 | 2.254 | 2.311 | 78.746 | 1.00x |
| wide_arrays.json | orjson | 2.398 | 2.425 | 2.464 | 78.746 | 0.93x |
| wide_arrays.json | msgspec | 3.512 | 3.541 | 3.756 | 78.746 | 0.64x |
| wide_arrays.json | ujson | 6.927 | 6.963 | 7.051 | 78.746 | 0.32x |
| wide_arrays.json | json | 17.161 | 17.289 | 18.830 | 78.746 | 0.13x |
| mixed.json | strata | 0.152 | 0.168 | 0.175 | 78.746 | 1.00x |
| mixed.json | orjson | 0.171 | 0.177 | 0.190 | 78.746 | 0.95x |
| mixed.json | msgspec | 0.186 | 0.192 | 0.201 | 78.746 | 0.88x |
| mixed.json | ujson | 0.345 | 0.352 | 0.355 | 78.746 | 0.48x |
| mixed.json | json | 0.648 | 0.664 | 0.682 | 78.746 | 0.25x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.072 | 0.074 | 0.080 | 64.965 | 1.00x |
| users.json $[*].id | jmespath | 0.466 | 0.483 | 0.512 | 64.965 | 0.15x |
| users.json $[*].id | jsonpath-ng | 2.719 | 2.786 | 2.828 | 64.965 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.418 | 0.435 | 0.507 | 64.980 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.985 | 3.038 | 3.210 | 64.980 | 0.14x |
| users.json $[*].orders[*].total | jsonpath-ng | 19.360 | 19.782 | 20.401 | 64.980 | 0.02x |
| users.json $..total | strata | 1.824 | 1.864 | 1.885 | 65.000 | 1.00x |
| users.json $..total | jsonpath-ng | 387.846 | 389.329 | 391.644 | 65.000 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.245 | 3.274 | 3.287 | 64.980 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.851 | 16.915 | 17.192 | 64.980 | 0.19x |
| users.json $[*].id | orjson+jsonpath-ng | 18.167 | 18.680 | 19.465 | 64.980 | 0.18x |
| users.json $[*].orders[*].total | strata | 3.490 | 3.513 | 3.538 | 65.000 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.760 | 19.051 | 21.073 | 65.000 | 0.18x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 39.045 | 39.664 | 40.105 | 65.000 | 0.09x |
| users.json $..total | strata | 14.114 | 14.530 | 15.287 | 65.004 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 413.071 | 415.427 | 418.482 | 65.004 | 0.03x |

