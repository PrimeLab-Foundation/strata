# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: bc6d9ba83ad33f6b9e2f3b35d87b07d0b4dcc11e
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
| users.json | strata | 5.775 | 6.108 | 6.691 | 68.219 | 1.00x |
| users.json | orjson | 8.613 | 8.894 | 9.851 | 68.219 | 0.69x |
| users.json | msgspec | 8.364 | 8.780 | 9.555 | 68.219 | 0.70x |
| users.json | ujson | 11.032 | 11.742 | 13.598 | 68.219 | 0.52x |
| users.json | pysimdjson | 115.753 | 120.423 | 129.571 | 68.219 | 0.05x |
| users.json | json | 13.584 | 14.399 | 15.033 | 68.219 | 0.42x |
| flat.json | strata | 0.557 | 0.642 | 0.782 | 95.469 | 1.00x |
| flat.json | orjson | 0.718 | 0.814 | 0.897 | 95.469 | 0.79x |
| flat.json | msgspec | 0.708 | 0.759 | 0.909 | 95.469 | 0.85x |
| flat.json | ujson | 1.117 | 1.380 | 1.437 | 95.469 | 0.46x |
| flat.json | pysimdjson | 11.530 | 12.336 | 13.853 | 95.469 | 0.05x |
| flat.json | json | 1.263 | 1.406 | 1.541 | 95.469 | 0.46x |
| nested.json | strata | 0.503 | 0.551 | 0.618 | 95.469 | 1.00x |
| nested.json | orjson | 0.740 | 0.800 | 0.871 | 95.469 | 0.69x |
| nested.json | msgspec | 0.717 | 0.758 | 0.793 | 95.469 | 0.73x |
| nested.json | ujson | 1.149 | 1.217 | 1.261 | 95.469 | 0.45x |
| nested.json | pysimdjson | 10.454 | 11.124 | 11.284 | 95.469 | 0.05x |
| nested.json | json | 1.421 | 1.540 | 1.592 | 95.469 | 0.36x |
| wide_arrays.json | strata | 2.988 | 3.506 | 4.313 | 98.203 | 1.00x |
| wide_arrays.json | orjson | 3.576 | 4.266 | 5.115 | 98.203 | 0.82x |
| wide_arrays.json | msgspec | 4.204 | 4.865 | 5.485 | 98.203 | 0.72x |
| wide_arrays.json | ujson | 5.654 | 5.943 | 6.753 | 98.203 | 0.59x |
| wide_arrays.json | pysimdjson | 62.717 | 66.832 | 78.819 | 98.203 | 0.05x |
| wide_arrays.json | json | 6.652 | 7.561 | 8.138 | 98.203 | 0.46x |
| mixed.json | strata | 0.122 | 0.131 | 0.172 | 98.641 | 1.00x |
| mixed.json | orjson | 0.155 | 0.174 | 0.271 | 98.641 | 0.76x |
| mixed.json | msgspec | 0.173 | 0.187 | 0.201 | 98.641 | 0.70x |
| mixed.json | ujson | 0.231 | 0.352 | 0.448 | 98.641 | 0.37x |
| mixed.json | pysimdjson | 2.461 | 2.594 | 2.726 | 98.641 | 0.05x |
| mixed.json | json | 0.321 | 0.355 | 0.404 | 98.641 | 0.37x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.489 | 1.699 | 1.954 | 79.281 | 1.00x |
| users.json | orjson | 2.331 | 2.568 | 3.187 | 79.281 | 0.66x |
| users.json | msgspec | 2.875 | 3.190 | 4.620 | 79.281 | 0.53x |
| users.json | ujson | 8.933 | 9.664 | 10.033 | 79.281 | 0.18x |
| users.json | json | 15.546 | 17.096 | 18.106 | 79.281 | 0.10x |
| flat.json | strata | 0.207 | 0.233 | 0.451 | 95.469 | 1.00x |
| flat.json | orjson | 0.243 | 0.273 | 0.300 | 95.469 | 0.85x |
| flat.json | msgspec | 0.307 | 0.334 | 0.606 | 95.469 | 0.70x |
| flat.json | ujson | 0.756 | 0.800 | 1.168 | 95.469 | 0.29x |
| flat.json | json | 1.400 | 1.471 | 1.597 | 95.469 | 0.16x |
| nested.json | strata | 0.137 | 0.147 | 0.156 | 95.469 | 1.00x |
| nested.json | orjson | 0.240 | 0.252 | 0.265 | 95.469 | 0.58x |
| nested.json | msgspec | 0.299 | 0.317 | 0.356 | 95.469 | 0.46x |
| nested.json | ujson | 0.996 | 1.041 | 1.144 | 95.469 | 0.14x |
| nested.json | json | 1.701 | 1.788 | 1.857 | 95.469 | 0.08x |
| wide_arrays.json | strata | 1.070 | 1.118 | 1.347 | 98.203 | 1.00x |
| wide_arrays.json | orjson | 1.307 | 1.584 | 1.786 | 98.203 | 0.71x |
| wide_arrays.json | msgspec | 2.090 | 2.232 | 2.660 | 98.203 | 0.50x |
| wide_arrays.json | ujson | 4.502 | 4.986 | 5.358 | 98.203 | 0.22x |
| wide_arrays.json | json | 11.485 | 12.220 | 13.114 | 98.203 | 0.09x |
| mixed.json | strata | 0.037 | 0.044 | 0.050 | 98.641 | 1.00x |
| mixed.json | orjson | 0.045 | 0.053 | 0.068 | 98.641 | 0.83x |
| mixed.json | msgspec | 0.053 | 0.056 | 0.072 | 98.641 | 0.78x |
| mixed.json | ujson | 0.166 | 0.187 | 0.220 | 98.641 | 0.24x |
| mixed.json | json | 0.342 | 0.389 | 0.620 | 98.641 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.684 | 6.981 | 7.375 | 89.312 | 1.00x |
| users.json | orjson | 9.610 | 10.218 | 10.601 | 89.312 | 0.68x |
| users.json | msgspec | 9.596 | 10.009 | 13.374 | 89.312 | 0.70x |
| users.json | ujson | 13.680 | 14.308 | 15.846 | 89.312 | 0.49x |
| users.json | json | 15.745 | 16.163 | 19.438 | 89.312 | 0.43x |
| flat.json | strata | 0.632 | 0.680 | 0.708 | 95.469 | 1.00x |
| flat.json | orjson | 0.860 | 0.921 | 1.162 | 95.469 | 0.74x |
| flat.json | msgspec | 0.768 | 0.826 | 0.928 | 95.469 | 0.82x |
| flat.json | ujson | 1.094 | 1.160 | 1.288 | 95.469 | 0.59x |
| flat.json | json | 1.332 | 1.423 | 1.555 | 95.469 | 0.48x |
| nested.json | strata | 0.625 | 0.656 | 0.707 | 95.469 | 1.00x |
| nested.json | orjson | 0.983 | 1.048 | 1.155 | 95.469 | 0.63x |
| nested.json | msgspec | 0.853 | 0.890 | 0.917 | 95.469 | 0.74x |
| nested.json | ujson | 1.117 | 1.208 | 1.295 | 95.469 | 0.54x |
| nested.json | json | 1.600 | 1.625 | 1.681 | 95.469 | 0.40x |
| wide_arrays.json | strata | 3.041 | 3.199 | 3.503 | 98.203 | 1.00x |
| wide_arrays.json | orjson | 3.651 | 3.936 | 4.281 | 98.203 | 0.81x |
| wide_arrays.json | msgspec | 4.295 | 4.544 | 4.895 | 98.203 | 0.70x |
| wide_arrays.json | ujson | 5.710 | 5.958 | 6.226 | 98.203 | 0.54x |
| wide_arrays.json | json | 6.946 | 7.252 | 7.745 | 98.203 | 0.44x |
| mixed.json | strata | 0.153 | 0.170 | 0.198 | 98.641 | 1.00x |
| mixed.json | orjson | 0.227 | 0.300 | 0.378 | 98.641 | 0.57x |
| mixed.json | msgspec | 0.223 | 0.252 | 0.325 | 98.641 | 0.67x |
| mixed.json | ujson | 0.268 | 0.287 | 0.314 | 98.641 | 0.59x |
| mixed.json | json | 0.359 | 0.381 | 0.481 | 98.641 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.579 | 6.834 | 7.170 | 94.844 | 1.00x |
| users.ndjson | orjson | 11.208 | 11.668 | 12.810 | 94.844 | 0.59x |
| users.ndjson | msgspec | 11.259 | 11.714 | 12.726 | 94.844 | 0.58x |
| users.ndjson | ujson | 14.103 | 14.507 | 15.289 | 94.844 | 0.47x |
| users.ndjson | json | 17.865 | 18.530 | 19.243 | 94.844 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.239 | 2.533 | 5.736 | 91.062 | 1.00x |
| users.json | orjson | 3.329 | 3.520 | 7.459 | 91.062 | 0.72x |
| users.json | msgspec | 3.972 | 4.355 | 7.237 | 91.062 | 0.58x |
| users.json | ujson | 10.742 | 12.099 | 19.656 | 91.062 | 0.21x |
| users.json | json | 17.689 | 21.475 | 39.908 | 91.062 | 0.12x |
| flat.json | strata | 0.354 | 0.502 | 0.598 | 95.469 | 1.00x |
| flat.json | orjson | 0.471 | 0.587 | 0.857 | 95.469 | 0.86x |
| flat.json | msgspec | 0.505 | 0.620 | 0.724 | 95.469 | 0.81x |
| flat.json | ujson | 0.923 | 1.133 | 1.373 | 95.469 | 0.44x |
| flat.json | json | 1.656 | 1.789 | 2.113 | 95.469 | 0.28x |
| nested.json | strata | 0.364 | 0.449 | 0.633 | 95.469 | 1.00x |
| nested.json | orjson | 0.521 | 0.589 | 0.749 | 95.469 | 0.76x |
| nested.json | msgspec | 0.602 | 0.730 | 0.878 | 95.469 | 0.61x |
| nested.json | ujson | 1.205 | 1.284 | 1.505 | 95.469 | 0.35x |
| nested.json | json | 2.164 | 2.317 | 2.527 | 95.469 | 0.19x |
| wide_arrays.json | strata | 1.395 | 1.587 | 2.284 | 98.625 | 1.00x |
| wide_arrays.json | orjson | 1.838 | 2.021 | 2.680 | 98.625 | 0.79x |
| wide_arrays.json | msgspec | 2.522 | 2.765 | 3.019 | 98.625 | 0.57x |
| wide_arrays.json | ujson | 4.962 | 5.749 | 6.374 | 98.625 | 0.28x |
| wide_arrays.json | json | 11.959 | 12.771 | 14.019 | 98.625 | 0.12x |
| mixed.json | strata | 0.130 | 0.177 | 0.330 | 98.641 | 1.00x |
| mixed.json | orjson | 0.139 | 0.256 | 0.383 | 98.641 | 0.69x |
| mixed.json | msgspec | 0.155 | 0.260 | 0.812 | 98.641 | 0.68x |
| mixed.json | ujson | 0.289 | 0.346 | 0.453 | 98.641 | 0.51x |
| mixed.json | json | 0.433 | 0.501 | 0.883 | 98.641 | 0.35x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.075 | 0.117 | 0.180 | 91.125 | 1.00x |
| users.json $[*].id | jmespath | 0.367 | 0.413 | 0.447 | 91.125 | 0.28x |
| users.json $[*].id | jsonpath-ng | 1.722 | 1.876 | 5.267 | 91.125 | 0.06x |
| users.json $[*].orders[*].total | strata | 0.296 | 0.428 | 0.668 | 91.219 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.882 | 2.041 | 2.261 | 91.219 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 10.892 | 11.609 | 13.162 | 91.219 | 0.04x |
| users.json $..total | strata | 1.269 | 1.324 | 1.437 | 91.250 | 1.00x |
| users.json $..total | jsonpath-ng | 190.221 | 191.833 | 195.611 | 91.250 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.768 | 3.983 | 5.990 | 91.156 | 1.00x |
| users.json $[*].id | orjson+jmespath | 11.525 | 12.386 | 14.935 | 91.156 | 0.32x |
| users.json $[*].id | orjson+jsonpath-ng | 12.970 | 13.271 | 14.121 | 91.156 | 0.30x |
| users.json $[*].orders[*].total | strata | 3.610 | 4.012 | 7.721 | 91.250 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 11.090 | 12.325 | 26.645 | 91.250 | 0.33x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 23.206 | 24.925 | 47.601 | 91.250 | 0.16x |
| users.json $..total | strata | 8.049 | 8.282 | 9.100 | 91.250 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 202.167 | 202.631 | 207.998 | 91.250 | 0.04x |

