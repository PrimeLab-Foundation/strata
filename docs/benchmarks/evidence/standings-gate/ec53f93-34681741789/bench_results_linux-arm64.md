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
| users.json | strata | 8.767 | 8.854 | 10.436 | 57.215 | 1.00x |
| users.json | orjson | 11.737 | 11.877 | 13.079 | 57.215 | 0.75x |
| users.json | msgspec | 12.203 | 12.330 | 13.647 | 57.215 | 0.72x |
| users.json | ujson | 16.685 | 16.881 | 18.716 | 57.215 | 0.52x |
| users.json | pysimdjson | 16.746 | 16.932 | 18.367 | 57.215 | 0.52x |
| users.json | json | 20.781 | 20.974 | 21.476 | 57.215 | 0.42x |
| flat.json | strata | 0.817 | 0.837 | 0.849 | 68.098 | 1.00x |
| flat.json | orjson | 0.867 | 0.878 | 0.902 | 68.098 | 0.95x |
| flat.json | msgspec | 0.895 | 0.902 | 0.907 | 68.098 | 0.93x |
| flat.json | ujson | 1.444 | 1.458 | 1.474 | 68.098 | 0.57x |
| flat.json | pysimdjson | 1.459 | 1.473 | 1.486 | 68.098 | 0.57x |
| flat.json | json | 1.754 | 1.767 | 1.776 | 68.098 | 0.47x |
| nested.json | strata | 0.802 | 0.823 | 0.830 | 68.098 | 1.00x |
| nested.json | orjson | 0.883 | 0.890 | 0.908 | 68.098 | 0.92x |
| nested.json | msgspec | 0.982 | 0.988 | 1.000 | 68.098 | 0.83x |
| nested.json | ujson | 1.406 | 1.423 | 1.448 | 68.098 | 0.58x |
| nested.json | pysimdjson | 1.391 | 1.410 | 1.425 | 68.098 | 0.58x |
| nested.json | json | 1.955 | 1.973 | 2.021 | 68.098 | 0.42x |
| wide_arrays.json | strata | 3.867 | 3.933 | 3.978 | 69.676 | 1.00x |
| wide_arrays.json | orjson | 4.109 | 4.150 | 4.201 | 69.676 | 0.95x |
| wide_arrays.json | msgspec | 5.067 | 5.111 | 5.148 | 69.676 | 0.77x |
| wide_arrays.json | ujson | 6.545 | 6.574 | 6.614 | 69.676 | 0.60x |
| wide_arrays.json | pysimdjson | 5.326 | 5.356 | 5.438 | 69.676 | 0.73x |
| wide_arrays.json | json | 9.589 | 9.637 | 9.740 | 69.676 | 0.41x |
| mixed.json | strata | 0.190 | 0.194 | 0.217 | 69.676 | 1.00x |
| mixed.json | orjson | 0.213 | 0.217 | 0.219 | 69.676 | 0.89x |
| mixed.json | msgspec | 0.234 | 0.238 | 0.241 | 69.676 | 0.81x |
| mixed.json | ujson | 0.306 | 0.314 | 0.317 | 69.676 | 0.62x |
| mixed.json | pysimdjson | 0.295 | 0.298 | 0.301 | 69.676 | 0.65x |
| mixed.json | json | 0.458 | 0.469 | 0.485 | 69.676 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.943 | 1.949 | 1.969 | 56.320 | 1.00x |
| users.json | orjson | 2.577 | 2.597 | 2.623 | 56.320 | 0.75x |
| users.json | msgspec | 3.321 | 3.329 | 3.369 | 56.320 | 0.59x |
| users.json | ujson | 10.551 | 10.576 | 10.650 | 56.320 | 0.18x |
| users.json | json | 19.032 | 19.138 | 19.215 | 56.320 | 0.10x |
| flat.json | strata | 0.238 | 0.241 | 0.243 | 68.098 | 1.00x |
| flat.json | orjson | 0.296 | 0.298 | 0.300 | 68.098 | 0.81x |
| flat.json | msgspec | 0.384 | 0.389 | 0.406 | 68.098 | 0.62x |
| flat.json | ujson | 0.978 | 0.982 | 0.994 | 68.098 | 0.24x |
| flat.json | json | 1.684 | 1.695 | 1.712 | 68.098 | 0.14x |
| nested.json | strata | 0.220 | 0.226 | 0.253 | 68.098 | 1.00x |
| nested.json | orjson | 0.281 | 0.285 | 0.312 | 68.098 | 0.79x |
| nested.json | msgspec | 0.373 | 0.377 | 0.407 | 68.098 | 0.60x |
| nested.json | ujson | 1.070 | 1.088 | 1.103 | 68.098 | 0.21x |
| nested.json | json | 2.164 | 2.184 | 2.223 | 68.098 | 0.10x |
| wide_arrays.json | strata | 1.335 | 1.346 | 1.366 | 69.676 | 1.00x |
| wide_arrays.json | orjson | 1.586 | 1.622 | 1.637 | 69.676 | 0.83x |
| wide_arrays.json | msgspec | 2.384 | 2.399 | 2.421 | 69.676 | 0.56x |
| wide_arrays.json | ujson | 4.759 | 4.791 | 4.813 | 69.676 | 0.28x |
| wide_arrays.json | json | 13.574 | 13.641 | 13.774 | 69.676 | 0.10x |
| mixed.json | strata | 0.063 | 0.066 | 0.082 | 69.676 | 1.00x |
| mixed.json | orjson | 0.065 | 0.067 | 0.068 | 69.676 | 0.99x |
| mixed.json | msgspec | 0.080 | 0.081 | 0.083 | 69.676 | 0.82x |
| mixed.json | ujson | 0.241 | 0.244 | 0.262 | 69.676 | 0.27x |
| mixed.json | json | 0.490 | 0.499 | 0.517 | 69.676 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.186 | 9.246 | 9.841 | 68.539 | 1.00x |
| users.json | orjson | 12.134 | 12.374 | 12.702 | 68.539 | 0.75x |
| users.json | msgspec | 12.636 | 12.832 | 13.231 | 68.539 | 0.72x |
| users.json | ujson | 17.455 | 18.065 | 19.248 | 68.539 | 0.51x |
| users.json | json | 21.338 | 21.550 | 21.762 | 68.539 | 0.43x |
| flat.json | strata | 0.863 | 0.885 | 0.902 | 68.098 | 1.00x |
| flat.json | orjson | 0.941 | 0.960 | 0.990 | 68.098 | 0.92x |
| flat.json | msgspec | 0.977 | 0.991 | 1.009 | 68.098 | 0.89x |
| flat.json | ujson | 1.557 | 1.574 | 1.597 | 68.098 | 0.56x |
| flat.json | json | 1.819 | 1.837 | 1.869 | 68.098 | 0.48x |
| nested.json | strata | 0.842 | 0.862 | 0.877 | 68.098 | 1.00x |
| nested.json | orjson | 0.925 | 0.954 | 0.976 | 68.098 | 0.90x |
| nested.json | msgspec | 1.046 | 1.053 | 1.081 | 68.098 | 0.82x |
| nested.json | ujson | 1.487 | 1.498 | 1.508 | 68.098 | 0.58x |
| nested.json | json | 2.027 | 2.033 | 2.043 | 68.098 | 0.42x |
| wide_arrays.json | strata | 3.869 | 3.889 | 3.923 | 69.676 | 1.00x |
| wide_arrays.json | orjson | 4.099 | 4.176 | 4.206 | 69.676 | 0.93x |
| wide_arrays.json | msgspec | 5.109 | 5.149 | 5.188 | 69.676 | 0.76x |
| wide_arrays.json | ujson | 6.701 | 6.742 | 6.809 | 69.676 | 0.58x |
| wide_arrays.json | json | 9.609 | 9.665 | 9.736 | 69.676 | 0.40x |
| mixed.json | strata | 0.226 | 0.230 | 0.252 | 69.676 | 1.00x |
| mixed.json | orjson | 0.286 | 0.294 | 0.314 | 69.676 | 0.78x |
| mixed.json | msgspec | 0.302 | 0.311 | 0.338 | 69.676 | 0.74x |
| mixed.json | ujson | 0.391 | 0.405 | 0.429 | 69.676 | 0.57x |
| mixed.json | json | 0.516 | 0.528 | 0.545 | 69.676 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.577 | 9.873 | 9.977 | 68.094 | 1.00x |
| users.ndjson | orjson | 15.047 | 15.155 | 15.414 | 68.094 | 0.65x |
| users.ndjson | msgspec | 15.210 | 15.387 | 15.679 | 68.094 | 0.64x |
| users.ndjson | ujson | 20.067 | 20.306 | 20.496 | 68.094 | 0.49x |
| users.ndjson | json | 26.506 | 26.907 | 27.171 | 68.094 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.475 | 2.513 | 2.552 | 68.539 | 1.00x |
| users.json | orjson | 3.158 | 3.196 | 3.254 | 68.539 | 0.79x |
| users.json | msgspec | 3.851 | 3.940 | 3.961 | 68.539 | 0.64x |
| users.json | ujson | 11.202 | 11.282 | 11.322 | 68.539 | 0.22x |
| users.json | json | 19.734 | 19.876 | 19.940 | 68.539 | 0.13x |
| flat.json | strata | 0.393 | 0.437 | 0.447 | 68.098 | 1.00x |
| flat.json | orjson | 0.476 | 0.505 | 0.529 | 68.098 | 0.87x |
| flat.json | msgspec | 0.589 | 0.596 | 0.627 | 68.098 | 0.73x |
| flat.json | ujson | 1.208 | 1.227 | 1.248 | 68.098 | 0.36x |
| flat.json | json | 1.910 | 1.934 | 1.959 | 68.098 | 0.23x |
| nested.json | strata | 0.361 | 0.380 | 0.395 | 68.098 | 1.00x |
| nested.json | orjson | 0.459 | 0.469 | 0.481 | 68.098 | 0.81x |
| nested.json | msgspec | 0.549 | 0.575 | 0.583 | 68.098 | 0.66x |
| nested.json | ujson | 1.299 | 1.311 | 1.465 | 68.098 | 0.29x |
| nested.json | json | 2.353 | 2.371 | 2.410 | 68.098 | 0.16x |
| wide_arrays.json | strata | 1.737 | 1.792 | 1.808 | 69.676 | 1.00x |
| wide_arrays.json | orjson | 2.018 | 2.080 | 2.157 | 69.676 | 0.86x |
| wide_arrays.json | msgspec | 2.825 | 2.838 | 2.936 | 69.676 | 0.63x |
| wide_arrays.json | ujson | 5.258 | 5.297 | 5.353 | 69.676 | 0.34x |
| wide_arrays.json | json | 14.130 | 14.152 | 14.253 | 69.676 | 0.13x |
| mixed.json | strata | 0.184 | 0.193 | 0.225 | 69.676 | 1.00x |
| mixed.json | orjson | 0.205 | 0.214 | 0.259 | 69.676 | 0.90x |
| mixed.json | msgspec | 0.223 | 0.232 | 0.245 | 69.676 | 0.83x |
| mixed.json | ujson | 0.397 | 0.413 | 0.435 | 69.676 | 0.47x |
| mixed.json | json | 0.646 | 0.659 | 0.676 | 69.676 | 0.29x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.108 | 0.109 | 0.113 | 68.539 | 1.00x |
| users.json $[*].id | jmespath | 0.478 | 0.498 | 0.505 | 68.539 | 0.22x |
| users.json $[*].id | jsonpath-ng | 2.492 | 2.566 | 2.583 | 68.539 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.625 | 0.648 | 0.657 | 68.664 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.995 | 3.038 | 3.068 | 68.664 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.927 | 18.163 | 18.301 | 68.664 | 0.04x |
| users.json $..total | strata | 1.720 | 1.733 | 1.764 | 69.672 | 1.00x |
| users.json $..total | jsonpath-ng | 292.838 | 293.585 | 294.150 | 69.672 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.253 | 3.260 | 3.302 | 68.664 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.051 | 13.222 | 13.432 | 68.664 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 14.867 | 15.008 | 15.225 | 68.664 | 0.22x |
| users.json $[*].orders[*].total | strata | 3.422 | 3.438 | 3.468 | 69.672 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.868 | 16.036 | 16.262 | 69.672 | 0.21x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 34.498 | 34.975 | 35.587 | 69.672 | 0.10x |
| users.json $..total | strata | 11.784 | 12.096 | 12.455 | 69.730 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 314.622 | 315.864 | 316.902 | 69.730 | 0.04x |

