# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: ec0411225b6d58a1df905844c946a766d3c39f0a
- python: 3.12.10
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit
- machine: arm64
- processor: Apple M1 (Virtual)
- compiler_flags: -fno-strict-overflow -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -O3 -Wall -arch arm64 -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -march=native -flto=thin -fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata -bundle -undefined (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.174 | 7.142 | 8.545 | 68.328 | 1.00x |
| users.json | orjson | 9.714 | 10.924 | 13.828 | 68.328 | 0.65x |
| users.json | msgspec | 9.568 | 10.353 | 13.256 | 68.328 | 0.69x |
| users.json | ujson | 12.349 | 14.672 | 19.724 | 68.328 | 0.49x |
| users.json | pysimdjson | 125.065 | 134.882 | 167.566 | 68.328 | 0.05x |
| users.json | json | 14.987 | 16.646 | 19.876 | 68.328 | 0.43x |
| flat.json | strata | 0.572 | 0.609 | 0.800 | 90.688 | 1.00x |
| flat.json | orjson | 0.748 | 0.791 | 0.995 | 90.688 | 0.77x |
| flat.json | msgspec | 0.691 | 0.743 | 0.954 | 90.688 | 0.82x |
| flat.json | ujson | 1.075 | 1.132 | 1.547 | 90.688 | 0.54x |
| flat.json | pysimdjson | 11.913 | 12.419 | 14.514 | 90.688 | 0.05x |
| flat.json | json | 1.334 | 1.425 | 1.691 | 90.688 | 0.43x |
| nested.json | strata | 0.559 | 0.577 | 0.623 | 90.688 | 1.00x |
| nested.json | orjson | 0.780 | 0.830 | 0.864 | 90.688 | 0.70x |
| nested.json | msgspec | 0.748 | 0.768 | 0.921 | 90.688 | 0.75x |
| nested.json | ujson | 1.202 | 1.252 | 1.317 | 90.688 | 0.46x |
| nested.json | pysimdjson | 11.676 | 11.900 | 12.306 | 90.688 | 0.05x |
| nested.json | json | 1.541 | 1.603 | 1.705 | 90.688 | 0.36x |
| wide_arrays.json | strata | 3.479 | 3.766 | 4.408 | 94.625 | 1.00x |
| wide_arrays.json | orjson | 4.050 | 4.733 | 5.567 | 94.625 | 0.80x |
| wide_arrays.json | msgspec | 4.497 | 5.060 | 5.827 | 94.625 | 0.74x |
| wide_arrays.json | ujson | 5.824 | 6.537 | 8.651 | 94.625 | 0.58x |
| wide_arrays.json | pysimdjson | 71.916 | 74.281 | 98.041 | 94.625 | 0.05x |
| wide_arrays.json | json | 8.060 | 8.403 | 13.621 | 94.625 | 0.45x |
| mixed.json | strata | 0.132 | 0.141 | 0.221 | 94.641 | 1.00x |
| mixed.json | orjson | 0.171 | 0.200 | 0.230 | 94.641 | 0.70x |
| mixed.json | msgspec | 0.184 | 0.192 | 0.257 | 94.641 | 0.73x |
| mixed.json | ujson | 0.238 | 0.338 | 0.503 | 94.641 | 0.42x |
| mixed.json | pysimdjson | 2.632 | 2.788 | 2.954 | 94.641 | 0.05x |
| mixed.json | json | 0.353 | 0.370 | 0.414 | 94.641 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.470 | 1.555 | 2.086 | 70.094 | 1.00x |
| users.json | orjson | 2.207 | 2.375 | 2.830 | 70.094 | 0.65x |
| users.json | msgspec | 2.861 | 3.015 | 3.507 | 70.094 | 0.52x |
| users.json | ujson | 8.853 | 9.338 | 10.422 | 70.094 | 0.17x |
| users.json | json | 15.900 | 16.181 | 17.690 | 70.094 | 0.10x |
| flat.json | strata | 0.224 | 0.258 | 0.288 | 90.688 | 1.00x |
| flat.json | orjson | 0.250 | 0.290 | 0.315 | 90.688 | 0.89x |
| flat.json | msgspec | 0.324 | 0.365 | 0.385 | 90.688 | 0.71x |
| flat.json | ujson | 0.809 | 0.878 | 0.948 | 90.688 | 0.29x |
| flat.json | json | 1.584 | 1.715 | 1.791 | 90.688 | 0.15x |
| nested.json | strata | 0.133 | 0.147 | 0.166 | 90.688 | 1.00x |
| nested.json | orjson | 0.239 | 0.268 | 0.434 | 90.688 | 0.55x |
| nested.json | msgspec | 0.301 | 0.356 | 0.552 | 90.688 | 0.41x |
| nested.json | ujson | 0.817 | 1.070 | 1.169 | 90.688 | 0.14x |
| nested.json | json | 1.703 | 1.895 | 2.047 | 90.688 | 0.08x |
| wide_arrays.json | strata | 1.250 | 1.418 | 1.626 | 94.625 | 1.00x |
| wide_arrays.json | orjson | 1.730 | 1.825 | 1.968 | 94.625 | 0.78x |
| wide_arrays.json | msgspec | 2.357 | 2.647 | 3.845 | 94.625 | 0.54x |
| wide_arrays.json | ujson | 5.222 | 5.742 | 6.006 | 94.625 | 0.25x |
| wide_arrays.json | json | 13.004 | 13.556 | 14.278 | 94.625 | 0.10x |
| mixed.json | strata | 0.042 | 0.048 | 0.053 | 94.641 | 1.00x |
| mixed.json | orjson | 0.049 | 0.057 | 0.072 | 94.641 | 0.84x |
| mixed.json | msgspec | 0.058 | 0.066 | 0.260 | 94.641 | 0.72x |
| mixed.json | ujson | 0.178 | 0.187 | 0.229 | 94.641 | 0.26x |
| mixed.json | json | 0.371 | 0.388 | 0.506 | 94.641 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.385 | 6.833 | 8.854 | 83.312 | 1.00x |
| users.json | orjson | 9.439 | 10.450 | 11.169 | 83.312 | 0.65x |
| users.json | msgspec | 9.190 | 9.688 | 10.299 | 83.312 | 0.71x |
| users.json | ujson | 12.588 | 13.528 | 15.005 | 83.312 | 0.51x |
| users.json | json | 15.265 | 15.780 | 16.644 | 83.312 | 0.43x |
| flat.json | strata | 0.689 | 0.720 | 0.792 | 90.688 | 1.00x |
| flat.json | orjson | 0.980 | 1.030 | 1.143 | 90.688 | 0.70x |
| flat.json | msgspec | 0.831 | 0.880 | 0.924 | 90.688 | 0.82x |
| flat.json | ujson | 1.127 | 1.236 | 1.522 | 90.688 | 0.58x |
| flat.json | json | 1.513 | 1.562 | 1.632 | 90.688 | 0.46x |
| nested.json | strata | 0.541 | 0.656 | 0.721 | 90.688 | 1.00x |
| nested.json | orjson | 0.907 | 1.074 | 1.444 | 90.688 | 0.61x |
| nested.json | msgspec | 0.745 | 0.888 | 1.029 | 90.688 | 0.74x |
| nested.json | ujson | 1.025 | 1.201 | 1.285 | 90.688 | 0.55x |
| nested.json | json | 1.435 | 1.744 | 2.361 | 90.688 | 0.38x |
| wide_arrays.json | strata | 3.266 | 3.604 | 4.414 | 94.625 | 1.00x |
| wide_arrays.json | orjson | 4.075 | 4.481 | 5.032 | 94.625 | 0.80x |
| wide_arrays.json | msgspec | 4.669 | 5.068 | 7.861 | 94.625 | 0.71x |
| wide_arrays.json | ujson | 6.041 | 6.465 | 7.179 | 94.625 | 0.56x |
| wide_arrays.json | json | 7.582 | 8.390 | 11.023 | 94.625 | 0.43x |
| mixed.json | strata | 0.222 | 0.254 | 0.322 | 94.641 | 1.00x |
| mixed.json | orjson | 0.278 | 0.489 | 0.596 | 94.641 | 0.52x |
| mixed.json | msgspec | 0.301 | 0.316 | 0.354 | 94.641 | 0.81x |
| mixed.json | ujson | 0.362 | 0.436 | 0.523 | 94.641 | 0.58x |
| mixed.json | json | 0.514 | 0.533 | 0.691 | 94.641 | 0.48x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.532 | 6.969 | 7.437 | 90.688 | 1.00x |
| users.ndjson | orjson | 11.236 | 12.108 | 12.712 | 90.688 | 0.58x |
| users.ndjson | msgspec | 11.239 | 12.129 | 13.000 | 90.688 | 0.57x |
| users.ndjson | ujson | 14.035 | 15.136 | 15.858 | 90.688 | 0.46x |
| users.ndjson | json | 17.973 | 19.072 | 20.130 | 90.688 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.980 | 2.136 | 3.245 | 86.859 | 1.00x |
| users.json | orjson | 2.777 | 2.983 | 3.194 | 86.859 | 0.72x |
| users.json | msgspec | 3.493 | 3.901 | 4.213 | 86.859 | 0.55x |
| users.json | ujson | 10.036 | 10.410 | 11.094 | 86.859 | 0.21x |
| users.json | json | 16.702 | 17.497 | 20.551 | 86.859 | 0.12x |
| flat.json | strata | 0.406 | 0.565 | 0.675 | 90.688 | 1.00x |
| flat.json | orjson | 0.397 | 0.520 | 0.579 | 90.688 | 1.09x |
| flat.json | msgspec | 0.462 | 0.604 | 0.818 | 90.688 | 0.93x |
| flat.json | ujson | 0.978 | 1.194 | 1.479 | 90.688 | 0.47x |
| flat.json | json | 1.576 | 1.783 | 1.986 | 90.688 | 0.32x |
| nested.json | strata | 0.253 | 0.303 | 0.614 | 90.688 | 1.00x |
| nested.json | orjson | 0.349 | 0.412 | 0.765 | 90.688 | 0.74x |
| nested.json | msgspec | 0.529 | 0.628 | 0.714 | 90.688 | 0.48x |
| nested.json | ujson | 0.968 | 1.073 | 1.456 | 90.688 | 0.28x |
| nested.json | json | 1.848 | 1.961 | 2.040 | 90.688 | 0.15x |
| wide_arrays.json | strata | 1.679 | 1.727 | 2.012 | 94.625 | 1.00x |
| wide_arrays.json | orjson | 1.973 | 2.314 | 2.466 | 94.625 | 0.75x |
| wide_arrays.json | msgspec | 2.890 | 3.124 | 3.306 | 94.625 | 0.55x |
| wide_arrays.json | ujson | 5.957 | 6.414 | 6.835 | 94.625 | 0.27x |
| wide_arrays.json | json | 13.342 | 14.091 | 14.527 | 94.625 | 0.12x |
| mixed.json | strata | 0.208 | 0.256 | 0.403 | 94.641 | 1.00x |
| mixed.json | orjson | 0.181 | 0.270 | 0.505 | 94.641 | 0.95x |
| mixed.json | msgspec | 0.222 | 0.293 | 0.422 | 94.641 | 0.87x |
| mixed.json | ujson | 0.409 | 0.454 | 0.561 | 94.641 | 0.56x |
| mixed.json | json | 0.572 | 0.662 | 1.575 | 94.641 | 0.39x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.051 | 0.084 | 0.135 | 86.938 | 1.00x |
| users.json $[*].id | jmespath | 0.271 | 0.311 | 0.390 | 86.938 | 0.27x |
| users.json $[*].id | jsonpath-ng | 1.568 | 1.645 | 1.979 | 86.938 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.263 | 0.276 | 0.458 | 87.078 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.551 | 1.699 | 2.023 | 87.078 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 9.652 | 9.737 | 10.997 | 87.078 | 0.03x |
| users.json $..total | strata | 1.204 | 1.265 | 1.454 | 87.078 | 1.00x |
| users.json $..total | jsonpath-ng | 178.105 | 181.984 | 205.489 | 87.078 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.382 | 3.553 | 3.883 | 86.984 | 1.00x |
| users.json $[*].id | orjson+jmespath | 9.181 | 9.968 | 11.355 | 86.984 | 0.36x |
| users.json $[*].id | orjson+jsonpath-ng | 10.412 | 11.256 | 12.182 | 86.984 | 0.32x |
| users.json $[*].orders[*].total | strata | 3.451 | 3.695 | 3.954 | 87.078 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 10.434 | 10.797 | 12.838 | 87.078 | 0.34x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 20.506 | 22.832 | 25.887 | 87.078 | 0.16x |
| users.json $..total | strata | 7.692 | 8.482 | 9.165 | 87.094 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 194.809 | 203.519 | 211.537 | 87.094 | 0.04x |

