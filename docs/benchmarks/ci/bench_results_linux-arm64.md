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
| users.json | strata | 8.760 | 8.917 | 11.275 | 57.203 | 1.00x |
| users.json | orjson | 11.660 | 12.133 | 14.194 | 57.203 | 0.73x |
| users.json | msgspec | 12.186 | 12.479 | 14.238 | 57.203 | 0.71x |
| users.json | ujson | 16.662 | 17.210 | 20.224 | 57.203 | 0.52x |
| users.json | pysimdjson | 16.150 | 17.543 | 19.737 | 57.203 | 0.51x |
| users.json | json | 20.433 | 21.381 | 22.439 | 57.203 | 0.42x |
| flat.json | strata | 0.860 | 0.878 | 0.905 | 67.992 | 1.00x |
| flat.json | orjson | 0.901 | 0.921 | 0.952 | 67.992 | 0.95x |
| flat.json | msgspec | 0.938 | 0.948 | 0.975 | 67.992 | 0.93x |
| flat.json | ujson | 1.497 | 1.537 | 1.639 | 67.992 | 0.57x |
| flat.json | pysimdjson | 1.521 | 1.556 | 1.577 | 67.992 | 0.56x |
| flat.json | json | 1.804 | 1.835 | 1.879 | 67.992 | 0.48x |
| nested.json | strata | 0.826 | 0.838 | 0.845 | 67.992 | 1.00x |
| nested.json | orjson | 0.887 | 0.903 | 0.909 | 67.992 | 0.93x |
| nested.json | msgspec | 1.017 | 1.025 | 1.041 | 67.992 | 0.82x |
| nested.json | ujson | 1.431 | 1.450 | 1.476 | 67.992 | 0.58x |
| nested.json | pysimdjson | 1.421 | 1.429 | 1.452 | 67.992 | 0.59x |
| nested.json | json | 1.974 | 1.987 | 2.012 | 67.992 | 0.42x |
| wide_arrays.json | strata | 3.947 | 3.978 | 4.050 | 69.562 | 1.00x |
| wide_arrays.json | orjson | 4.107 | 4.159 | 4.290 | 69.562 | 0.96x |
| wide_arrays.json | msgspec | 5.043 | 5.094 | 5.201 | 69.562 | 0.78x |
| wide_arrays.json | ujson | 6.497 | 6.621 | 6.806 | 69.562 | 0.60x |
| wide_arrays.json | pysimdjson | 5.275 | 5.465 | 5.672 | 69.562 | 0.73x |
| wide_arrays.json | json | 9.591 | 9.775 | 9.916 | 69.562 | 0.41x |
| mixed.json | strata | 0.197 | 0.204 | 0.237 | 69.562 | 1.00x |
| mixed.json | orjson | 0.223 | 0.227 | 0.254 | 69.562 | 0.90x |
| mixed.json | msgspec | 0.239 | 0.256 | 0.273 | 69.562 | 0.80x |
| mixed.json | ujson | 0.328 | 0.342 | 0.359 | 69.562 | 0.60x |
| mixed.json | pysimdjson | 0.302 | 0.312 | 0.335 | 69.562 | 0.65x |
| mixed.json | json | 0.477 | 0.491 | 0.506 | 69.562 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.922 | 1.927 | 1.939 | 56.312 | 1.00x |
| users.json | orjson | 2.577 | 2.590 | 2.619 | 56.312 | 0.74x |
| users.json | msgspec | 3.307 | 3.318 | 3.326 | 56.312 | 0.58x |
| users.json | ujson | 10.522 | 10.577 | 10.607 | 56.312 | 0.18x |
| users.json | json | 18.945 | 19.061 | 19.205 | 56.312 | 0.10x |
| flat.json | strata | 0.237 | 0.243 | 0.261 | 67.992 | 1.00x |
| flat.json | orjson | 0.304 | 0.308 | 0.325 | 67.992 | 0.79x |
| flat.json | msgspec | 0.393 | 0.406 | 0.419 | 67.992 | 0.60x |
| flat.json | ujson | 0.989 | 1.002 | 1.017 | 67.992 | 0.24x |
| flat.json | json | 1.716 | 1.731 | 1.749 | 67.992 | 0.14x |
| nested.json | strata | 0.215 | 0.218 | 0.242 | 67.992 | 1.00x |
| nested.json | orjson | 0.282 | 0.283 | 0.304 | 67.992 | 0.77x |
| nested.json | msgspec | 0.367 | 0.371 | 0.393 | 67.992 | 0.59x |
| nested.json | ujson | 1.084 | 1.093 | 1.103 | 67.992 | 0.20x |
| nested.json | json | 2.138 | 2.188 | 2.218 | 67.992 | 0.10x |
| wide_arrays.json | strata | 1.398 | 1.468 | 1.497 | 69.562 | 1.00x |
| wide_arrays.json | orjson | 1.641 | 1.673 | 1.697 | 69.562 | 0.88x |
| wide_arrays.json | msgspec | 2.396 | 2.413 | 2.466 | 69.562 | 0.61x |
| wide_arrays.json | ujson | 4.831 | 4.861 | 4.898 | 69.562 | 0.30x |
| wide_arrays.json | json | 13.811 | 13.826 | 13.951 | 69.562 | 0.11x |
| mixed.json | strata | 0.065 | 0.068 | 0.086 | 69.562 | 1.00x |
| mixed.json | orjson | 0.067 | 0.069 | 0.086 | 69.562 | 0.99x |
| mixed.json | msgspec | 0.083 | 0.086 | 0.104 | 69.562 | 0.79x |
| mixed.json | ujson | 0.249 | 0.250 | 0.278 | 69.562 | 0.27x |
| mixed.json | json | 0.505 | 0.515 | 0.528 | 69.562 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.418 | 9.490 | 10.208 | 68.426 | 1.00x |
| users.json | orjson | 12.479 | 12.860 | 13.395 | 68.426 | 0.74x |
| users.json | msgspec | 12.913 | 13.408 | 14.115 | 68.426 | 0.71x |
| users.json | ujson | 17.996 | 18.901 | 19.585 | 68.426 | 0.50x |
| users.json | json | 21.338 | 22.040 | 22.692 | 68.426 | 0.43x |
| flat.json | strata | 0.883 | 0.898 | 0.930 | 67.992 | 1.00x |
| flat.json | orjson | 0.965 | 0.983 | 1.009 | 67.992 | 0.91x |
| flat.json | msgspec | 1.014 | 1.024 | 1.068 | 67.992 | 0.88x |
| flat.json | ujson | 1.581 | 1.600 | 1.634 | 67.992 | 0.56x |
| flat.json | json | 1.866 | 1.883 | 1.892 | 67.992 | 0.48x |
| nested.json | strata | 0.859 | 0.880 | 0.905 | 67.992 | 1.00x |
| nested.json | orjson | 0.980 | 0.989 | 1.008 | 67.992 | 0.89x |
| nested.json | msgspec | 1.082 | 1.100 | 1.112 | 67.992 | 0.80x |
| nested.json | ujson | 1.526 | 1.549 | 1.579 | 67.992 | 0.57x |
| nested.json | json | 2.045 | 2.069 | 2.099 | 67.992 | 0.43x |
| wide_arrays.json | strata | 3.999 | 4.103 | 4.237 | 69.562 | 1.00x |
| wide_arrays.json | orjson | 4.206 | 4.507 | 4.597 | 69.562 | 0.91x |
| wide_arrays.json | msgspec | 5.348 | 5.449 | 5.551 | 69.562 | 0.75x |
| wide_arrays.json | ujson | 6.986 | 7.099 | 7.244 | 69.562 | 0.58x |
| wide_arrays.json | json | 9.837 | 10.037 | 10.185 | 69.562 | 0.41x |
| mixed.json | strata | 0.222 | 0.231 | 0.254 | 69.562 | 1.00x |
| mixed.json | orjson | 0.299 | 0.314 | 0.332 | 69.562 | 0.74x |
| mixed.json | msgspec | 0.319 | 0.327 | 0.381 | 69.562 | 0.71x |
| mixed.json | ujson | 0.409 | 0.428 | 0.447 | 69.562 | 0.54x |
| mixed.json | json | 0.537 | 0.544 | 0.563 | 69.562 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.125 | 10.451 | 10.580 | 67.984 | 1.00x |
| users.ndjson | orjson | 15.607 | 16.220 | 16.461 | 67.984 | 0.64x |
| users.ndjson | msgspec | 15.983 | 16.485 | 16.695 | 67.984 | 0.63x |
| users.ndjson | ujson | 20.953 | 21.550 | 21.920 | 67.984 | 0.48x |
| users.ndjson | json | 27.445 | 28.065 | 28.337 | 67.984 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.585 | 2.624 | 2.704 | 68.426 | 1.00x |
| users.json | orjson | 3.329 | 3.417 | 3.497 | 68.426 | 0.77x |
| users.json | msgspec | 4.045 | 4.118 | 4.270 | 68.426 | 0.64x |
| users.json | ujson | 11.288 | 11.455 | 11.535 | 68.426 | 0.23x |
| users.json | json | 19.939 | 20.026 | 20.191 | 68.426 | 0.13x |
| flat.json | strata | 0.447 | 0.466 | 0.497 | 67.992 | 1.00x |
| flat.json | orjson | 0.535 | 0.577 | 0.647 | 67.992 | 0.81x |
| flat.json | msgspec | 0.623 | 0.658 | 0.733 | 67.992 | 0.71x |
| flat.json | ujson | 1.266 | 1.299 | 1.371 | 67.992 | 0.36x |
| flat.json | json | 1.981 | 2.023 | 2.068 | 67.992 | 0.23x |
| nested.json | strata | 0.403 | 0.417 | 0.448 | 67.992 | 1.00x |
| nested.json | orjson | 0.506 | 0.519 | 0.543 | 67.992 | 0.80x |
| nested.json | msgspec | 0.577 | 0.608 | 0.659 | 67.992 | 0.69x |
| nested.json | ujson | 1.337 | 1.355 | 1.412 | 67.992 | 0.31x |
| nested.json | json | 2.392 | 2.444 | 2.470 | 67.992 | 0.17x |
| wide_arrays.json | strata | 1.874 | 1.978 | 2.098 | 69.562 | 1.00x |
| wide_arrays.json | orjson | 2.189 | 2.304 | 2.403 | 69.562 | 0.86x |
| wide_arrays.json | msgspec | 2.978 | 3.019 | 3.099 | 69.562 | 0.66x |
| wide_arrays.json | ujson | 5.451 | 5.534 | 5.660 | 69.562 | 0.36x |
| wide_arrays.json | json | 14.474 | 14.589 | 14.785 | 69.562 | 0.14x |
| mixed.json | strata | 0.218 | 0.233 | 0.268 | 69.562 | 1.00x |
| mixed.json | orjson | 0.246 | 0.264 | 0.315 | 69.562 | 0.88x |
| mixed.json | msgspec | 0.259 | 0.272 | 0.298 | 69.562 | 0.86x |
| mixed.json | ujson | 0.441 | 0.467 | 0.504 | 69.562 | 0.50x |
| mixed.json | json | 0.696 | 0.720 | 0.758 | 69.562 | 0.32x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.115 | 0.119 | 0.129 | 68.426 | 1.00x |
| users.json $[*].id | jmespath | 0.484 | 0.501 | 0.513 | 68.426 | 0.24x |
| users.json $[*].id | jsonpath-ng | 2.497 | 2.602 | 2.674 | 68.426 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.624 | 0.632 | 0.657 | 68.551 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.951 | 2.980 | 3.052 | 68.551 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.952 | 18.831 | 19.245 | 68.551 | 0.03x |
| users.json $..total | strata | 1.714 | 1.738 | 1.773 | 69.559 | 1.00x |
| users.json $..total | jsonpath-ng | 298.129 | 298.778 | 299.170 | 69.559 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.205 | 3.221 | 3.233 | 68.551 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.109 | 13.232 | 13.755 | 68.551 | 0.24x |
| users.json $[*].id | orjson+jsonpath-ng | 14.751 | 15.216 | 15.538 | 68.551 | 0.21x |
| users.json $[*].orders[*].total | strata | 3.388 | 3.418 | 3.422 | 69.559 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.914 | 16.072 | 16.489 | 69.559 | 0.21x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 35.333 | 35.487 | 36.190 | 69.559 | 0.10x |
| users.json $..total | strata | 12.040 | 12.610 | 13.025 | 69.621 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 315.067 | 316.766 | 317.230 | 69.621 | 0.04x |

