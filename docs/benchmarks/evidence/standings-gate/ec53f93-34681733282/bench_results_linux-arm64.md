# Benchmark results - ci-linux-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: ec53f936b8a9245edf5bfeec36c38539666a1775
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
| users.json | strata | 8.727 | 8.764 | 10.656 | 57.262 | 1.00x |
| users.json | orjson | 11.424 | 11.523 | 13.089 | 57.262 | 0.76x |
| users.json | msgspec | 11.972 | 12.023 | 13.478 | 57.262 | 0.73x |
| users.json | ujson | 16.125 | 16.301 | 18.592 | 57.262 | 0.54x |
| users.json | pysimdjson | 16.115 | 16.184 | 18.012 | 57.262 | 0.54x |
| users.json | json | 20.321 | 20.432 | 21.091 | 57.262 | 0.43x |
| flat.json | strata | 0.780 | 0.798 | 0.818 | 68.117 | 1.00x |
| flat.json | orjson | 0.837 | 0.856 | 0.862 | 68.117 | 0.93x |
| flat.json | msgspec | 0.887 | 0.900 | 0.908 | 68.117 | 0.89x |
| flat.json | ujson | 1.382 | 1.391 | 1.415 | 68.117 | 0.57x |
| flat.json | pysimdjson | 1.430 | 1.437 | 1.453 | 68.117 | 0.56x |
| flat.json | json | 1.733 | 1.739 | 1.742 | 68.117 | 0.46x |
| nested.json | strata | 0.796 | 0.814 | 0.817 | 68.117 | 1.00x |
| nested.json | orjson | 0.863 | 0.879 | 0.888 | 68.117 | 0.93x |
| nested.json | msgspec | 0.983 | 0.990 | 0.996 | 68.117 | 0.82x |
| nested.json | ujson | 1.375 | 1.386 | 1.414 | 68.117 | 0.59x |
| nested.json | pysimdjson | 1.373 | 1.385 | 1.409 | 68.117 | 0.59x |
| nested.json | json | 1.945 | 1.952 | 1.960 | 68.117 | 0.42x |
| wide_arrays.json | strata | 3.799 | 3.846 | 3.883 | 69.695 | 1.00x |
| wide_arrays.json | orjson | 4.042 | 4.078 | 4.103 | 69.695 | 0.94x |
| wide_arrays.json | msgspec | 5.035 | 5.066 | 5.083 | 69.695 | 0.76x |
| wide_arrays.json | ujson | 6.450 | 6.503 | 6.525 | 69.695 | 0.59x |
| wide_arrays.json | pysimdjson | 5.223 | 5.250 | 5.300 | 69.695 | 0.73x |
| wide_arrays.json | json | 9.455 | 9.491 | 9.550 | 69.695 | 0.41x |
| mixed.json | strata | 0.186 | 0.189 | 0.227 | 69.695 | 1.00x |
| mixed.json | orjson | 0.207 | 0.226 | 0.258 | 69.695 | 0.84x |
| mixed.json | msgspec | 0.230 | 0.236 | 0.288 | 69.695 | 0.80x |
| mixed.json | ujson | 0.299 | 0.306 | 0.384 | 69.695 | 0.62x |
| mixed.json | pysimdjson | 0.290 | 0.296 | 0.410 | 69.695 | 0.64x |
| mixed.json | json | 0.441 | 0.447 | 0.561 | 69.695 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.916 | 1.925 | 1.971 | 56.367 | 1.00x |
| users.json | orjson | 2.554 | 2.569 | 2.588 | 56.367 | 0.75x |
| users.json | msgspec | 3.290 | 3.296 | 3.331 | 56.367 | 0.58x |
| users.json | ujson | 10.468 | 10.481 | 10.516 | 56.367 | 0.18x |
| users.json | json | 18.898 | 18.958 | 19.005 | 56.367 | 0.10x |
| flat.json | strata | 0.235 | 0.236 | 0.251 | 68.117 | 1.00x |
| flat.json | orjson | 0.292 | 0.294 | 0.306 | 68.117 | 0.80x |
| flat.json | msgspec | 0.380 | 0.383 | 0.396 | 68.117 | 0.62x |
| flat.json | ujson | 0.979 | 0.984 | 1.000 | 68.117 | 0.24x |
| flat.json | json | 1.670 | 1.680 | 1.697 | 68.117 | 0.14x |
| nested.json | strata | 0.216 | 0.219 | 0.236 | 68.117 | 1.00x |
| nested.json | orjson | 0.281 | 0.282 | 0.301 | 68.117 | 0.78x |
| nested.json | msgspec | 0.367 | 0.369 | 0.395 | 68.117 | 0.59x |
| nested.json | ujson | 1.069 | 1.077 | 1.099 | 68.117 | 0.20x |
| nested.json | json | 2.120 | 2.161 | 2.185 | 68.117 | 0.10x |
| wide_arrays.json | strata | 1.294 | 1.303 | 1.338 | 69.695 | 1.00x |
| wide_arrays.json | orjson | 1.575 | 1.585 | 1.605 | 69.695 | 0.82x |
| wide_arrays.json | msgspec | 2.356 | 2.372 | 2.380 | 69.695 | 0.55x |
| wide_arrays.json | ujson | 4.722 | 4.731 | 4.760 | 69.695 | 0.28x |
| wide_arrays.json | json | 13.503 | 13.530 | 13.614 | 69.695 | 0.10x |
| mixed.json | strata | 0.078 | 0.084 | 0.151 | 69.695 | 1.00x |
| mixed.json | orjson | 0.077 | 0.089 | 0.163 | 69.695 | 0.94x |
| mixed.json | msgspec | 0.095 | 0.099 | 0.177 | 69.695 | 0.85x |
| mixed.json | ujson | 0.259 | 0.275 | 0.347 | 69.695 | 0.30x |
| mixed.json | json | 0.521 | 0.534 | 0.596 | 69.695 | 0.16x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.880 | 8.985 | 9.810 | 68.559 | 1.00x |
| users.json | orjson | 11.592 | 11.752 | 12.225 | 68.559 | 0.76x |
| users.json | msgspec | 12.185 | 12.294 | 12.651 | 68.559 | 0.73x |
| users.json | ujson | 16.770 | 17.161 | 18.175 | 68.559 | 0.52x |
| users.json | json | 20.595 | 20.725 | 20.988 | 68.559 | 0.43x |
| flat.json | strata | 0.825 | 0.843 | 0.866 | 68.117 | 1.00x |
| flat.json | orjson | 0.913 | 0.931 | 0.940 | 68.117 | 0.91x |
| flat.json | msgspec | 0.968 | 0.972 | 0.990 | 68.117 | 0.87x |
| flat.json | ujson | 1.489 | 1.515 | 1.532 | 68.117 | 0.56x |
| flat.json | json | 1.794 | 1.808 | 1.839 | 68.117 | 0.47x |
| nested.json | strata | 0.830 | 0.842 | 0.849 | 68.117 | 1.00x |
| nested.json | orjson | 0.920 | 0.933 | 0.959 | 68.117 | 0.90x |
| nested.json | msgspec | 1.037 | 1.043 | 1.067 | 68.117 | 0.81x |
| nested.json | ujson | 1.457 | 1.474 | 1.486 | 68.117 | 0.57x |
| nested.json | json | 2.001 | 2.008 | 2.023 | 68.117 | 0.42x |
| wide_arrays.json | strata | 3.782 | 3.818 | 3.852 | 69.695 | 1.00x |
| wide_arrays.json | orjson | 3.990 | 4.026 | 4.060 | 69.695 | 0.95x |
| wide_arrays.json | msgspec | 5.013 | 5.064 | 5.095 | 69.695 | 0.75x |
| wide_arrays.json | ujson | 6.610 | 6.635 | 6.676 | 69.695 | 0.58x |
| wide_arrays.json | json | 9.508 | 9.553 | 9.596 | 69.695 | 0.40x |
| mixed.json | strata | 0.209 | 0.214 | 0.231 | 69.695 | 1.00x |
| mixed.json | orjson | 0.267 | 0.273 | 0.300 | 69.695 | 0.78x |
| mixed.json | msgspec | 0.287 | 0.297 | 0.322 | 69.695 | 0.72x |
| mixed.json | ujson | 0.362 | 0.373 | 0.395 | 69.695 | 0.57x |
| mixed.json | json | 0.496 | 0.516 | 0.522 | 69.695 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.207 | 9.246 | 9.396 | 68.113 | 1.00x |
| users.ndjson | orjson | 14.387 | 14.506 | 14.771 | 68.113 | 0.64x |
| users.ndjson | msgspec | 14.783 | 14.930 | 15.063 | 68.113 | 0.62x |
| users.ndjson | ujson | 19.230 | 19.490 | 19.741 | 68.113 | 0.47x |
| users.ndjson | json | 25.325 | 25.517 | 25.986 | 68.113 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.375 | 2.428 | 2.518 | 68.559 | 1.00x |
| users.json | orjson | 3.073 | 3.104 | 3.208 | 68.559 | 0.78x |
| users.json | msgspec | 3.779 | 3.823 | 3.863 | 68.559 | 0.63x |
| users.json | ujson | 11.051 | 11.105 | 11.159 | 68.559 | 0.22x |
| users.json | json | 19.586 | 19.661 | 19.740 | 68.559 | 0.12x |
| flat.json | strata | 0.395 | 0.414 | 0.438 | 68.117 | 1.00x |
| flat.json | orjson | 0.475 | 0.496 | 0.505 | 68.117 | 0.83x |
| flat.json | msgspec | 0.575 | 0.591 | 0.601 | 68.117 | 0.70x |
| flat.json | ujson | 1.176 | 1.204 | 1.234 | 68.117 | 0.34x |
| flat.json | json | 1.900 | 1.912 | 1.934 | 68.117 | 0.22x |
| nested.json | strata | 0.362 | 0.374 | 0.402 | 68.117 | 1.00x |
| nested.json | orjson | 0.447 | 0.462 | 0.491 | 68.117 | 0.81x |
| nested.json | msgspec | 0.528 | 0.555 | 0.576 | 68.117 | 0.67x |
| nested.json | ujson | 1.259 | 1.288 | 1.324 | 68.117 | 0.29x |
| nested.json | json | 2.351 | 2.363 | 2.376 | 68.117 | 0.16x |
| wide_arrays.json | strata | 1.641 | 1.668 | 1.700 | 69.695 | 1.00x |
| wide_arrays.json | orjson | 1.978 | 1.988 | 2.028 | 69.695 | 0.84x |
| wide_arrays.json | msgspec | 2.735 | 2.756 | 2.768 | 69.695 | 0.61x |
| wide_arrays.json | ujson | 5.165 | 5.195 | 5.229 | 69.695 | 0.32x |
| wide_arrays.json | json | 13.942 | 13.975 | 14.046 | 69.695 | 0.12x |
| mixed.json | strata | 0.156 | 0.170 | 0.172 | 69.695 | 1.00x |
| mixed.json | orjson | 0.174 | 0.189 | 0.222 | 69.695 | 0.90x |
| mixed.json | msgspec | 0.188 | 0.201 | 0.228 | 69.695 | 0.85x |
| mixed.json | ujson | 0.362 | 0.393 | 0.408 | 69.695 | 0.43x |
| mixed.json | json | 0.592 | 0.635 | 0.640 | 69.695 | 0.27x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.100 | 0.102 | 0.116 | 68.559 | 1.00x |
| users.json $[*].id | jmespath | 0.464 | 0.466 | 0.481 | 68.559 | 0.22x |
| users.json $[*].id | jsonpath-ng | 2.406 | 2.455 | 2.504 | 68.559 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.601 | 0.616 | 0.629 | 68.684 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.917 | 2.936 | 2.981 | 68.684 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.274 | 17.567 | 17.836 | 68.684 | 0.04x |
| users.json $..total | strata | 1.687 | 1.697 | 1.703 | 69.691 | 1.00x |
| users.json $..total | jsonpath-ng | 295.610 | 296.516 | 296.862 | 69.691 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.178 | 3.203 | 3.223 | 68.684 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.356 | 12.542 | 12.716 | 68.684 | 0.26x |
| users.json $[*].id | orjson+jsonpath-ng | 14.179 | 14.292 | 14.403 | 68.684 | 0.22x |
| users.json $[*].orders[*].total | strata | 3.370 | 3.390 | 3.414 | 69.691 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.206 | 15.356 | 15.536 | 69.691 | 0.22x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 33.131 | 33.238 | 33.656 | 69.691 | 0.10x |
| users.json $..total | strata | 11.336 | 11.574 | 11.930 | 69.750 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 310.665 | 312.002 | 313.200 | 69.750 | 0.04x |

