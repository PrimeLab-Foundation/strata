# Benchmark results - ci-linux-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 565fab210bb851772fcefe65082041084aeafc14
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
| users.json | strata | 9.073 | 9.173 | 11.242 | 57.262 | 1.00x |
| users.json | orjson | 11.861 | 12.095 | 13.708 | 57.262 | 0.76x |
| users.json | msgspec | 12.341 | 12.557 | 14.160 | 57.262 | 0.73x |
| users.json | ujson | 16.606 | 17.299 | 19.605 | 57.262 | 0.53x |
| users.json | pysimdjson | 16.689 | 17.355 | 19.157 | 57.262 | 0.53x |
| users.json | json | 20.818 | 21.179 | 21.953 | 57.262 | 0.43x |
| flat.json | strata | 0.847 | 0.866 | 0.879 | 68.504 | 1.00x |
| flat.json | orjson | 0.875 | 0.890 | 0.895 | 68.504 | 0.97x |
| flat.json | msgspec | 0.926 | 0.938 | 0.954 | 68.504 | 0.92x |
| flat.json | ujson | 1.457 | 1.472 | 1.488 | 68.504 | 0.59x |
| flat.json | pysimdjson | 1.488 | 1.496 | 1.515 | 68.504 | 0.58x |
| flat.json | json | 1.788 | 1.801 | 1.837 | 68.504 | 0.48x |
| nested.json | strata | 0.814 | 0.830 | 0.836 | 68.504 | 1.00x |
| nested.json | orjson | 0.883 | 0.888 | 0.894 | 68.504 | 0.94x |
| nested.json | msgspec | 0.999 | 1.008 | 1.025 | 68.504 | 0.82x |
| nested.json | ujson | 1.399 | 1.411 | 1.462 | 68.504 | 0.59x |
| nested.json | pysimdjson | 1.381 | 1.395 | 1.428 | 68.504 | 0.60x |
| nested.json | json | 1.962 | 1.977 | 1.989 | 68.504 | 0.42x |
| wide_arrays.json | strata | 3.935 | 3.975 | 4.000 | 70.160 | 1.00x |
| wide_arrays.json | orjson | 4.077 | 4.124 | 4.149 | 70.160 | 0.96x |
| wide_arrays.json | msgspec | 5.089 | 5.124 | 5.176 | 70.160 | 0.78x |
| wide_arrays.json | ujson | 6.458 | 6.515 | 6.577 | 70.160 | 0.61x |
| wide_arrays.json | pysimdjson | 5.290 | 5.335 | 5.359 | 70.160 | 0.75x |
| wide_arrays.json | json | 9.487 | 9.514 | 9.575 | 70.160 | 0.42x |
| mixed.json | strata | 0.195 | 0.198 | 0.213 | 70.160 | 1.00x |
| mixed.json | orjson | 0.215 | 0.217 | 0.234 | 70.160 | 0.91x |
| mixed.json | msgspec | 0.236 | 0.239 | 0.256 | 70.160 | 0.83x |
| mixed.json | ujson | 0.305 | 0.311 | 0.334 | 70.160 | 0.64x |
| mixed.json | pysimdjson | 0.295 | 0.304 | 0.325 | 70.160 | 0.65x |
| mixed.json | json | 0.455 | 0.466 | 0.483 | 70.160 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.922 | 1.937 | 1.967 | 56.359 | 1.00x |
| users.json | orjson | 2.596 | 2.608 | 2.620 | 56.359 | 0.74x |
| users.json | msgspec | 3.285 | 3.298 | 3.318 | 56.359 | 0.59x |
| users.json | ujson | 10.593 | 10.620 | 10.699 | 56.359 | 0.18x |
| users.json | json | 19.225 | 19.334 | 19.491 | 56.359 | 0.10x |
| flat.json | strata | 0.238 | 0.241 | 0.266 | 68.504 | 1.00x |
| flat.json | orjson | 0.304 | 0.308 | 0.336 | 68.504 | 0.78x |
| flat.json | msgspec | 0.392 | 0.396 | 0.417 | 68.504 | 0.61x |
| flat.json | ujson | 0.989 | 0.995 | 1.006 | 68.504 | 0.24x |
| flat.json | json | 1.713 | 1.746 | 1.767 | 68.504 | 0.14x |
| nested.json | strata | 0.218 | 0.220 | 0.232 | 68.504 | 1.00x |
| nested.json | orjson | 0.286 | 0.294 | 0.306 | 68.504 | 0.75x |
| nested.json | msgspec | 0.374 | 0.378 | 0.395 | 68.504 | 0.58x |
| nested.json | ujson | 1.076 | 1.082 | 1.105 | 68.504 | 0.20x |
| nested.json | json | 2.163 | 2.180 | 2.212 | 68.504 | 0.10x |
| wide_arrays.json | strata | 1.315 | 1.327 | 1.337 | 70.160 | 1.00x |
| wide_arrays.json | orjson | 1.556 | 1.578 | 1.603 | 70.160 | 0.84x |
| wide_arrays.json | msgspec | 2.379 | 2.386 | 2.435 | 70.160 | 0.56x |
| wide_arrays.json | ujson | 4.732 | 4.753 | 4.791 | 70.160 | 0.28x |
| wide_arrays.json | json | 13.567 | 13.603 | 13.653 | 70.160 | 0.10x |
| mixed.json | strata | 0.061 | 0.065 | 0.068 | 70.160 | 1.00x |
| mixed.json | orjson | 0.065 | 0.067 | 0.069 | 70.160 | 0.97x |
| mixed.json | msgspec | 0.079 | 0.082 | 0.085 | 70.160 | 0.79x |
| mixed.json | ujson | 0.235 | 0.242 | 0.263 | 70.160 | 0.27x |
| mixed.json | json | 0.486 | 0.495 | 0.505 | 70.160 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.484 | 9.629 | 10.430 | 70.137 | 1.00x |
| users.json | orjson | 12.385 | 12.825 | 13.091 | 70.137 | 0.75x |
| users.json | msgspec | 12.947 | 13.361 | 13.505 | 70.137 | 0.72x |
| users.json | ujson | 18.202 | 18.538 | 19.055 | 70.137 | 0.52x |
| users.json | json | 21.445 | 21.661 | 22.082 | 70.137 | 0.44x |
| flat.json | strata | 0.871 | 0.894 | 0.924 | 68.504 | 1.00x |
| flat.json | orjson | 0.933 | 0.957 | 0.981 | 68.504 | 0.93x |
| flat.json | msgspec | 1.002 | 1.010 | 1.036 | 68.504 | 0.89x |
| flat.json | ujson | 1.544 | 1.561 | 1.652 | 68.504 | 0.57x |
| flat.json | json | 1.853 | 1.865 | 1.884 | 68.504 | 0.48x |
| nested.json | strata | 0.865 | 0.877 | 0.885 | 68.504 | 1.00x |
| nested.json | orjson | 0.960 | 0.972 | 0.981 | 68.504 | 0.90x |
| nested.json | msgspec | 1.077 | 1.093 | 1.105 | 68.504 | 0.80x |
| nested.json | ujson | 1.510 | 1.525 | 1.560 | 68.504 | 0.58x |
| nested.json | json | 2.040 | 2.057 | 2.073 | 68.504 | 0.43x |
| wide_arrays.json | strata | 3.929 | 3.971 | 4.039 | 70.160 | 1.00x |
| wide_arrays.json | orjson | 4.083 | 4.130 | 4.213 | 70.160 | 0.96x |
| wide_arrays.json | msgspec | 5.136 | 5.195 | 5.263 | 70.160 | 0.76x |
| wide_arrays.json | ujson | 6.629 | 6.700 | 6.809 | 70.160 | 0.59x |
| wide_arrays.json | json | 9.512 | 9.608 | 9.671 | 70.160 | 0.41x |
| mixed.json | strata | 0.219 | 0.225 | 0.238 | 70.160 | 1.00x |
| mixed.json | orjson | 0.280 | 0.287 | 0.308 | 70.160 | 0.78x |
| mixed.json | msgspec | 0.303 | 0.310 | 0.323 | 70.160 | 0.72x |
| mixed.json | ujson | 0.381 | 0.402 | 0.425 | 70.160 | 0.56x |
| mixed.json | json | 0.508 | 0.534 | 0.549 | 70.160 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.609 | 9.706 | 10.189 | 68.504 | 1.00x |
| users.ndjson | orjson | 14.834 | 15.045 | 15.663 | 68.504 | 0.65x |
| users.ndjson | msgspec | 15.387 | 15.512 | 15.871 | 68.504 | 0.63x |
| users.ndjson | ujson | 19.958 | 20.118 | 20.833 | 68.504 | 0.48x |
| users.ndjson | json | 26.091 | 26.399 | 27.344 | 68.504 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.630 | 2.663 | 2.753 | 70.137 | 1.00x |
| users.json | orjson | 3.409 | 3.436 | 3.608 | 70.137 | 0.78x |
| users.json | msgspec | 3.985 | 4.124 | 4.180 | 70.137 | 0.65x |
| users.json | ujson | 11.377 | 11.528 | 11.637 | 70.137 | 0.23x |
| users.json | json | 20.026 | 20.095 | 20.268 | 70.137 | 0.13x |
| flat.json | strata | 0.480 | 0.496 | 0.527 | 68.504 | 1.00x |
| flat.json | orjson | 0.573 | 0.586 | 0.610 | 68.504 | 0.85x |
| flat.json | msgspec | 0.657 | 0.678 | 0.697 | 68.504 | 0.73x |
| flat.json | ujson | 1.275 | 1.307 | 1.353 | 68.504 | 0.38x |
| flat.json | json | 2.009 | 2.052 | 2.138 | 68.504 | 0.24x |
| nested.json | strata | 0.430 | 0.443 | 0.478 | 68.504 | 1.00x |
| nested.json | orjson | 0.543 | 0.559 | 0.597 | 68.504 | 0.79x |
| nested.json | msgspec | 0.628 | 0.653 | 0.708 | 68.504 | 0.68x |
| nested.json | ujson | 1.389 | 1.412 | 1.418 | 68.504 | 0.31x |
| nested.json | json | 2.443 | 2.467 | 2.502 | 68.504 | 0.18x |
| wide_arrays.json | strata | 1.808 | 1.845 | 1.942 | 70.160 | 1.00x |
| wide_arrays.json | orjson | 2.110 | 2.130 | 2.184 | 70.160 | 0.87x |
| wide_arrays.json | msgspec | 2.874 | 2.921 | 2.974 | 70.160 | 0.63x |
| wide_arrays.json | ujson | 5.315 | 5.367 | 5.467 | 70.160 | 0.34x |
| wide_arrays.json | json | 14.160 | 14.223 | 14.336 | 70.160 | 0.13x |
| mixed.json | strata | 0.234 | 0.240 | 0.278 | 70.160 | 1.00x |
| mixed.json | orjson | 0.257 | 0.276 | 0.313 | 70.160 | 0.87x |
| mixed.json | msgspec | 0.282 | 0.289 | 0.342 | 70.160 | 0.83x |
| mixed.json | ujson | 0.469 | 0.488 | 0.520 | 70.160 | 0.49x |
| mixed.json | json | 0.699 | 0.732 | 0.782 | 70.160 | 0.33x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.107 | 0.116 | 0.128 | 70.137 | 1.00x |
| users.json $[*].id | jmespath | 0.479 | 0.495 | 0.507 | 70.137 | 0.23x |
| users.json $[*].id | jsonpath-ng | 2.478 | 2.565 | 2.669 | 70.137 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.645 | 0.656 | 0.675 | 70.141 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.025 | 3.050 | 3.088 | 70.141 | 0.22x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.799 | 18.107 | 18.415 | 70.141 | 0.04x |
| users.json $..total | strata | 1.722 | 1.739 | 1.758 | 70.141 | 1.00x |
| users.json $..total | jsonpath-ng | 294.216 | 294.624 | 295.050 | 70.141 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.195 | 3.209 | 3.231 | 70.141 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.766 | 13.086 | 13.321 | 70.141 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 14.698 | 14.845 | 15.148 | 70.141 | 0.22x |
| users.json $[*].orders[*].total | strata | 3.391 | 3.419 | 3.451 | 70.141 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.832 | 16.278 | 16.545 | 70.141 | 0.21x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 34.458 | 35.292 | 35.838 | 70.141 | 0.10x |
| users.json $..total | strata | 11.804 | 11.965 | 12.108 | 70.141 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 312.890 | 314.400 | 314.988 | 70.141 | 0.04x |

