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
| users.json | strata | 8.989 | 9.148 | 9.328 | 57.406 | 1.00x |
| users.json | orjson | 11.779 | 12.080 | 12.364 | 57.406 | 0.76x |
| users.json | msgspec | 12.329 | 12.625 | 12.770 | 57.406 | 0.72x |
| users.json | ujson | 16.585 | 17.056 | 17.719 | 57.406 | 0.54x |
| users.json | pysimdjson | 16.733 | 17.194 | 17.797 | 57.406 | 0.53x |
| users.json | json | 20.778 | 21.130 | 21.539 | 57.406 | 0.43x |
| flat.json | strata | 0.840 | 0.876 | 0.894 | 68.312 | 1.00x |
| flat.json | orjson | 0.865 | 0.892 | 0.908 | 68.312 | 0.98x |
| flat.json | msgspec | 0.901 | 0.926 | 0.934 | 68.312 | 0.95x |
| flat.json | ujson | 1.432 | 1.470 | 1.501 | 68.312 | 0.60x |
| flat.json | pysimdjson | 1.472 | 1.496 | 1.517 | 68.312 | 0.59x |
| flat.json | json | 1.762 | 1.777 | 1.794 | 68.312 | 0.49x |
| nested.json | strata | 0.818 | 0.840 | 0.852 | 68.312 | 1.00x |
| nested.json | orjson | 0.889 | 0.911 | 0.928 | 68.312 | 0.92x |
| nested.json | msgspec | 1.012 | 1.025 | 1.037 | 68.312 | 0.82x |
| nested.json | ujson | 1.419 | 1.454 | 1.486 | 68.312 | 0.58x |
| nested.json | pysimdjson | 1.419 | 1.444 | 1.463 | 68.312 | 0.58x |
| nested.json | json | 1.989 | 2.003 | 2.024 | 68.312 | 0.42x |
| wide_arrays.json | strata | 3.939 | 4.015 | 4.069 | 70.680 | 1.00x |
| wide_arrays.json | orjson | 4.145 | 4.209 | 4.272 | 70.680 | 0.95x |
| wide_arrays.json | msgspec | 5.120 | 5.196 | 5.244 | 70.680 | 0.77x |
| wide_arrays.json | ujson | 6.577 | 6.650 | 6.700 | 70.680 | 0.60x |
| wide_arrays.json | pysimdjson | 5.343 | 5.416 | 5.466 | 70.680 | 0.74x |
| wide_arrays.json | json | 9.580 | 9.648 | 9.699 | 70.680 | 0.42x |
| mixed.json | strata | 0.193 | 0.201 | 0.225 | 70.680 | 1.00x |
| mixed.json | orjson | 0.216 | 0.223 | 0.248 | 70.680 | 0.90x |
| mixed.json | msgspec | 0.235 | 0.239 | 0.265 | 70.680 | 0.84x |
| mixed.json | ujson | 0.309 | 0.319 | 0.347 | 70.680 | 0.63x |
| mixed.json | pysimdjson | 0.301 | 0.308 | 0.333 | 70.680 | 0.65x |
| mixed.json | json | 0.454 | 0.465 | 0.492 | 70.680 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.006 | 2.020 | 2.039 | 56.516 | 1.00x |
| users.json | orjson | 2.641 | 2.657 | 2.697 | 56.516 | 0.76x |
| users.json | msgspec | 3.370 | 3.394 | 3.422 | 56.516 | 0.60x |
| users.json | ujson | 10.584 | 10.648 | 10.704 | 56.516 | 0.19x |
| users.json | json | 19.079 | 19.172 | 19.286 | 56.516 | 0.11x |
| flat.json | strata | 0.238 | 0.244 | 0.263 | 68.312 | 1.00x |
| flat.json | orjson | 0.309 | 0.313 | 0.335 | 68.312 | 0.78x |
| flat.json | msgspec | 0.397 | 0.404 | 0.426 | 68.312 | 0.60x |
| flat.json | ujson | 1.004 | 1.014 | 1.025 | 68.312 | 0.24x |
| flat.json | json | 1.710 | 1.729 | 1.740 | 68.312 | 0.14x |
| nested.json | strata | 0.224 | 0.228 | 0.270 | 68.312 | 1.00x |
| nested.json | orjson | 0.291 | 0.295 | 0.344 | 68.312 | 0.77x |
| nested.json | msgspec | 0.378 | 0.387 | 0.449 | 68.312 | 0.59x |
| nested.json | ujson | 1.086 | 1.100 | 1.161 | 68.312 | 0.21x |
| nested.json | json | 2.177 | 2.225 | 2.347 | 68.312 | 0.10x |
| wide_arrays.json | strata | 1.325 | 1.339 | 1.361 | 70.680 | 1.00x |
| wide_arrays.json | orjson | 1.600 | 1.625 | 1.649 | 70.680 | 0.82x |
| wide_arrays.json | msgspec | 2.362 | 2.390 | 2.409 | 70.680 | 0.56x |
| wide_arrays.json | ujson | 4.769 | 4.804 | 4.837 | 70.680 | 0.28x |
| wide_arrays.json | json | 13.572 | 13.619 | 13.681 | 70.680 | 0.10x |
| mixed.json | strata | 0.065 | 0.069 | 0.083 | 70.680 | 1.00x |
| mixed.json | orjson | 0.067 | 0.071 | 0.073 | 70.680 | 0.97x |
| mixed.json | msgspec | 0.081 | 0.084 | 0.099 | 70.680 | 0.82x |
| mixed.json | ujson | 0.241 | 0.251 | 0.274 | 70.680 | 0.27x |
| mixed.json | json | 0.480 | 0.496 | 0.518 | 70.680 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.199 | 9.424 | 9.703 | 69.535 | 1.00x |
| users.json | orjson | 11.951 | 12.413 | 12.857 | 69.535 | 0.76x |
| users.json | msgspec | 12.580 | 12.913 | 13.297 | 69.535 | 0.73x |
| users.json | ujson | 17.181 | 17.731 | 18.487 | 69.535 | 0.53x |
| users.json | json | 21.050 | 21.475 | 21.859 | 69.535 | 0.44x |
| flat.json | strata | 0.870 | 0.909 | 0.932 | 68.312 | 1.00x |
| flat.json | orjson | 0.959 | 0.975 | 0.995 | 68.312 | 0.93x |
| flat.json | msgspec | 0.993 | 1.012 | 1.033 | 68.312 | 0.90x |
| flat.json | ujson | 1.544 | 1.592 | 1.629 | 68.312 | 0.57x |
| flat.json | json | 1.831 | 1.851 | 1.866 | 68.312 | 0.49x |
| nested.json | strata | 0.848 | 0.873 | 0.885 | 68.312 | 1.00x |
| nested.json | orjson | 0.967 | 0.980 | 0.995 | 68.312 | 0.89x |
| nested.json | msgspec | 1.070 | 1.086 | 1.112 | 68.312 | 0.80x |
| nested.json | ujson | 1.501 | 1.533 | 1.563 | 68.312 | 0.57x |
| nested.json | json | 2.044 | 2.061 | 2.079 | 68.312 | 0.42x |
| wide_arrays.json | strata | 3.974 | 4.006 | 4.052 | 70.680 | 1.00x |
| wide_arrays.json | orjson | 4.132 | 4.223 | 4.275 | 70.680 | 0.95x |
| wide_arrays.json | msgspec | 5.158 | 5.222 | 5.286 | 70.680 | 0.77x |
| wide_arrays.json | ujson | 6.717 | 6.803 | 6.872 | 70.680 | 0.59x |
| wide_arrays.json | json | 9.578 | 9.672 | 9.746 | 70.680 | 0.41x |
| mixed.json | strata | 0.217 | 0.224 | 0.253 | 70.680 | 1.00x |
| mixed.json | orjson | 0.282 | 0.292 | 0.313 | 70.680 | 0.77x |
| mixed.json | msgspec | 0.299 | 0.308 | 0.332 | 70.680 | 0.73x |
| mixed.json | ujson | 0.384 | 0.404 | 0.427 | 70.680 | 0.55x |
| mixed.json | json | 0.511 | 0.531 | 0.551 | 70.680 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.762 | 10.042 | 10.516 | 68.301 | 1.00x |
| users.ndjson | orjson | 15.198 | 15.555 | 15.847 | 68.301 | 0.65x |
| users.ndjson | msgspec | 15.525 | 15.848 | 16.137 | 68.301 | 0.63x |
| users.ndjson | ujson | 20.286 | 20.831 | 21.362 | 68.301 | 0.48x |
| users.ndjson | json | 26.408 | 27.244 | 27.703 | 68.301 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.805 | 2.943 | 3.038 | 69.535 | 1.00x |
| users.json | orjson | 3.494 | 3.616 | 3.712 | 69.535 | 0.81x |
| users.json | msgspec | 4.209 | 4.341 | 4.458 | 69.535 | 0.68x |
| users.json | ujson | 11.590 | 11.742 | 11.896 | 69.535 | 0.25x |
| users.json | json | 20.081 | 20.363 | 20.654 | 69.535 | 0.14x |
| flat.json | strata | 0.657 | 0.722 | 2.804 | 68.312 | 1.00x |
| flat.json | orjson | 0.766 | 0.839 | 0.920 | 68.312 | 0.86x |
| flat.json | msgspec | 0.854 | 0.936 | 1.001 | 68.312 | 0.77x |
| flat.json | ujson | 1.503 | 1.557 | 1.659 | 68.312 | 0.46x |
| flat.json | json | 2.195 | 2.272 | 2.347 | 68.312 | 0.32x |
| nested.json | strata | 0.621 | 0.689 | 0.852 | 68.312 | 1.00x |
| nested.json | orjson | 0.766 | 0.820 | 0.878 | 68.312 | 0.84x |
| nested.json | msgspec | 0.852 | 0.906 | 1.033 | 68.312 | 0.76x |
| nested.json | ujson | 1.589 | 1.656 | 1.744 | 68.312 | 0.42x |
| nested.json | json | 2.691 | 2.752 | 2.881 | 68.312 | 0.25x |
| wide_arrays.json | strata | 2.011 | 2.070 | 2.253 | 70.680 | 1.00x |
| wide_arrays.json | orjson | 2.306 | 2.408 | 2.504 | 70.680 | 0.86x |
| wide_arrays.json | msgspec | 3.103 | 3.165 | 3.832 | 70.680 | 0.65x |
| wide_arrays.json | ujson | 5.541 | 5.639 | 5.745 | 70.680 | 0.37x |
| wide_arrays.json | json | 14.357 | 14.507 | 15.957 | 70.680 | 0.14x |
| mixed.json | strata | 0.443 | 0.475 | 0.526 | 70.680 | 1.00x |
| mixed.json | orjson | 0.474 | 0.526 | 0.583 | 70.680 | 0.90x |
| mixed.json | msgspec | 0.504 | 0.543 | 0.696 | 70.680 | 0.88x |
| mixed.json | ujson | 0.695 | 0.739 | 0.807 | 70.680 | 0.64x |
| mixed.json | json | 0.917 | 0.979 | 1.046 | 70.680 | 0.49x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.113 | 0.115 | 0.130 | 69.535 | 1.00x |
| users.json $[*].id | jmespath | 0.488 | 0.499 | 0.513 | 69.535 | 0.23x |
| users.json $[*].id | jsonpath-ng | 2.536 | 2.627 | 2.682 | 69.535 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.673 | 0.698 | 0.716 | 69.680 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.103 | 3.148 | 3.184 | 69.680 | 0.22x |
| users.json $[*].orders[*].total | jsonpath-ng | 18.263 | 18.932 | 19.369 | 69.680 | 0.04x |
| users.json $..total | strata | 1.772 | 1.804 | 1.837 | 69.820 | 1.00x |
| users.json $..total | jsonpath-ng | 298.770 | 299.584 | 300.285 | 69.820 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.208 | 3.241 | 3.272 | 69.680 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.901 | 13.379 | 13.598 | 69.680 | 0.24x |
| users.json $[*].id | orjson+jsonpath-ng | 14.758 | 15.178 | 15.601 | 69.680 | 0.21x |
| users.json $[*].orders[*].total | strata | 3.395 | 3.433 | 3.482 | 69.820 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.871 | 16.661 | 17.133 | 69.820 | 0.21x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 35.156 | 36.840 | 38.358 | 69.820 | 0.09x |
| users.json $..total | strata | 12.181 | 13.201 | 13.662 | 69.938 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 317.766 | 321.339 | 322.455 | 69.938 | 0.04x |

