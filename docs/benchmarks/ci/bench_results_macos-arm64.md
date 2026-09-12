# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: b32d39824b8fea15839872cab8834a1b0fef1294
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
| users.json | strata | 6.064 | 6.351 | 8.256 | 68.125 | 1.00x |
| users.json | orjson | 9.048 | 10.057 | 11.647 | 68.125 | 0.63x |
| users.json | msgspec | 8.769 | 9.743 | 10.891 | 68.125 | 0.65x |
| users.json | ujson | 12.109 | 13.541 | 15.343 | 68.125 | 0.47x |
| users.json | pysimdjson | 121.179 | 124.601 | 148.998 | 68.125 | 0.05x |
| users.json | json | 14.409 | 15.433 | 18.888 | 68.125 | 0.41x |
| flat.json | strata | 0.563 | 0.573 | 0.714 | 90.969 | 1.00x |
| flat.json | orjson | 0.708 | 0.730 | 0.793 | 90.969 | 0.78x |
| flat.json | msgspec | 0.692 | 0.708 | 0.813 | 90.969 | 0.81x |
| flat.json | ujson | 1.061 | 1.082 | 1.212 | 90.969 | 0.53x |
| flat.json | pysimdjson | 11.565 | 11.803 | 12.431 | 90.969 | 0.05x |
| flat.json | json | 1.312 | 1.373 | 1.506 | 90.969 | 0.42x |
| nested.json | strata | 0.494 | 0.517 | 0.570 | 91.016 | 1.00x |
| nested.json | orjson | 0.712 | 0.733 | 0.785 | 91.016 | 0.71x |
| nested.json | msgspec | 0.667 | 0.680 | 0.707 | 91.016 | 0.76x |
| nested.json | ujson | 1.059 | 1.111 | 1.179 | 91.016 | 0.47x |
| nested.json | pysimdjson | 10.121 | 10.191 | 10.520 | 91.016 | 0.05x |
| nested.json | json | 1.382 | 1.414 | 1.641 | 91.016 | 0.37x |
| wide_arrays.json | strata | 2.923 | 3.069 | 3.453 | 94.547 | 1.00x |
| wide_arrays.json | orjson | 3.587 | 3.702 | 4.052 | 94.547 | 0.83x |
| wide_arrays.json | msgspec | 3.999 | 4.103 | 4.178 | 94.547 | 0.75x |
| wide_arrays.json | ujson | 5.182 | 5.282 | 5.612 | 94.547 | 0.58x |
| wide_arrays.json | pysimdjson | 62.408 | 63.836 | 71.311 | 94.547 | 0.05x |
| wide_arrays.json | json | 6.729 | 6.793 | 7.767 | 94.547 | 0.45x |
| mixed.json | strata | 0.121 | 0.124 | 0.144 | 94.562 | 1.00x |
| mixed.json | orjson | 0.155 | 0.158 | 0.180 | 94.562 | 0.78x |
| mixed.json | msgspec | 0.168 | 0.173 | 0.182 | 94.562 | 0.72x |
| mixed.json | ujson | 0.208 | 0.215 | 0.246 | 94.562 | 0.58x |
| mixed.json | pysimdjson | 2.453 | 2.473 | 2.500 | 94.562 | 0.05x |
| mixed.json | json | 0.323 | 0.330 | 0.343 | 94.562 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.552 | 1.653 | 1.903 | 71.219 | 1.00x |
| users.json | orjson | 2.276 | 2.580 | 2.821 | 71.219 | 0.64x |
| users.json | msgspec | 2.958 | 3.216 | 3.390 | 71.219 | 0.51x |
| users.json | ujson | 8.942 | 9.929 | 10.197 | 71.219 | 0.17x |
| users.json | json | 15.591 | 16.928 | 18.226 | 71.219 | 0.10x |
| flat.json | strata | 0.198 | 0.202 | 0.228 | 91.016 | 1.00x |
| flat.json | orjson | 0.244 | 0.247 | 0.315 | 91.016 | 0.82x |
| flat.json | msgspec | 0.303 | 0.314 | 0.492 | 91.016 | 0.64x |
| flat.json | ujson | 0.760 | 0.784 | 0.950 | 91.016 | 0.26x |
| flat.json | json | 1.338 | 1.440 | 1.662 | 91.016 | 0.14x |
| nested.json | strata | 0.117 | 0.122 | 0.295 | 91.016 | 1.00x |
| nested.json | orjson | 0.210 | 0.219 | 0.241 | 91.016 | 0.56x |
| nested.json | msgspec | 0.279 | 0.308 | 0.458 | 91.016 | 0.40x |
| nested.json | ujson | 0.774 | 0.784 | 0.892 | 91.016 | 0.16x |
| nested.json | json | 1.574 | 1.623 | 1.834 | 91.016 | 0.08x |
| wide_arrays.json | strata | 1.085 | 1.102 | 1.260 | 94.547 | 1.00x |
| wide_arrays.json | orjson | 1.313 | 1.415 | 1.571 | 94.547 | 0.78x |
| wide_arrays.json | msgspec | 2.104 | 2.152 | 2.385 | 94.547 | 0.51x |
| wide_arrays.json | ujson | 4.799 | 4.876 | 5.080 | 94.547 | 0.23x |
| wide_arrays.json | json | 11.626 | 11.703 | 12.104 | 94.547 | 0.09x |
| mixed.json | strata | 0.037 | 0.038 | 0.041 | 94.562 | 1.00x |
| mixed.json | orjson | 0.046 | 0.047 | 0.054 | 94.562 | 0.81x |
| mixed.json | msgspec | 0.052 | 0.056 | 0.066 | 94.562 | 0.68x |
| mixed.json | ujson | 0.168 | 0.175 | 0.192 | 94.562 | 0.22x |
| mixed.json | json | 0.351 | 0.356 | 0.374 | 94.562 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.461 | 6.536 | 7.021 | 83.016 | 1.00x |
| users.json | orjson | 9.338 | 9.669 | 10.140 | 83.016 | 0.68x |
| users.json | msgspec | 9.130 | 9.552 | 10.075 | 83.016 | 0.68x |
| users.json | ujson | 12.569 | 13.228 | 14.575 | 83.016 | 0.49x |
| users.json | json | 14.994 | 15.124 | 16.049 | 83.016 | 0.43x |
| flat.json | strata | 0.606 | 0.640 | 0.723 | 91.016 | 1.00x |
| flat.json | orjson | 0.807 | 0.886 | 1.129 | 91.016 | 0.72x |
| flat.json | msgspec | 0.755 | 0.778 | 0.899 | 91.016 | 0.82x |
| flat.json | ujson | 1.110 | 1.165 | 1.279 | 91.016 | 0.55x |
| flat.json | json | 1.375 | 1.393 | 1.469 | 91.016 | 0.46x |
| nested.json | strata | 0.535 | 0.553 | 0.654 | 91.016 | 1.00x |
| nested.json | orjson | 0.836 | 0.865 | 0.986 | 91.016 | 0.64x |
| nested.json | msgspec | 0.713 | 0.757 | 0.904 | 91.016 | 0.73x |
| nested.json | ujson | 0.992 | 1.050 | 1.101 | 91.016 | 0.53x |
| nested.json | json | 1.422 | 1.467 | 1.660 | 91.016 | 0.38x |
| wide_arrays.json | strata | 3.099 | 3.134 | 3.185 | 94.547 | 1.00x |
| wide_arrays.json | orjson | 3.741 | 3.777 | 4.052 | 94.547 | 0.83x |
| wide_arrays.json | msgspec | 4.277 | 4.338 | 4.790 | 94.547 | 0.72x |
| wide_arrays.json | ujson | 5.544 | 5.660 | 5.894 | 94.547 | 0.55x |
| wide_arrays.json | json | 6.937 | 6.998 | 7.413 | 94.547 | 0.45x |
| mixed.json | strata | 0.155 | 0.190 | 0.255 | 94.562 | 1.00x |
| mixed.json | orjson | 0.206 | 0.245 | 0.282 | 94.562 | 0.78x |
| mixed.json | msgspec | 0.207 | 0.242 | 0.328 | 94.562 | 0.79x |
| mixed.json | ujson | 0.251 | 0.308 | 0.401 | 94.562 | 0.62x |
| mixed.json | json | 0.366 | 0.386 | 0.510 | 94.562 | 0.49x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.557 | 7.019 | 7.837 | 90.938 | 1.00x |
| users.ndjson | orjson | 11.123 | 11.715 | 24.114 | 90.938 | 0.60x |
| users.ndjson | msgspec | 10.998 | 11.704 | 20.510 | 90.938 | 0.60x |
| users.ndjson | ujson | 13.820 | 14.922 | 19.306 | 90.938 | 0.47x |
| users.ndjson | json | 17.924 | 18.955 | 23.145 | 90.938 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.960 | 2.095 | 5.112 | 85.891 | 1.00x |
| users.json | orjson | 2.750 | 2.934 | 3.172 | 85.891 | 0.71x |
| users.json | msgspec | 3.358 | 3.575 | 4.154 | 85.891 | 0.59x |
| users.json | ujson | 9.415 | 9.756 | 11.907 | 85.891 | 0.21x |
| users.json | json | 16.044 | 16.645 | 17.564 | 85.891 | 0.13x |
| flat.json | strata | 0.353 | 0.447 | 0.521 | 91.016 | 1.00x |
| flat.json | orjson | 0.415 | 0.493 | 0.688 | 91.016 | 0.91x |
| flat.json | msgspec | 0.486 | 0.530 | 0.824 | 91.016 | 0.84x |
| flat.json | ujson | 0.913 | 1.018 | 1.257 | 91.016 | 0.44x |
| flat.json | json | 1.575 | 1.713 | 1.985 | 91.016 | 0.26x |
| nested.json | strata | 0.278 | 0.296 | 0.439 | 91.016 | 1.00x |
| nested.json | orjson | 0.357 | 0.427 | 0.803 | 91.016 | 0.69x |
| nested.json | msgspec | 0.453 | 0.549 | 0.674 | 91.016 | 0.54x |
| nested.json | ujson | 1.003 | 1.054 | 1.146 | 91.016 | 0.28x |
| nested.json | json | 1.740 | 1.864 | 2.032 | 91.016 | 0.16x |
| wide_arrays.json | strata | 1.520 | 1.683 | 1.870 | 94.547 | 1.00x |
| wide_arrays.json | orjson | 1.835 | 2.075 | 3.427 | 94.547 | 0.81x |
| wide_arrays.json | msgspec | 2.516 | 2.652 | 2.926 | 94.547 | 0.63x |
| wide_arrays.json | ujson | 5.265 | 5.649 | 6.188 | 94.547 | 0.30x |
| wide_arrays.json | json | 12.092 | 12.534 | 12.947 | 94.547 | 0.13x |
| mixed.json | strata | 0.178 | 0.225 | 0.269 | 94.562 | 1.00x |
| mixed.json | orjson | 0.182 | 0.217 | 0.421 | 94.562 | 1.04x |
| mixed.json | msgspec | 0.177 | 0.216 | 0.321 | 94.562 | 1.04x |
| mixed.json | ujson | 0.298 | 0.350 | 0.712 | 94.562 | 0.64x |
| mixed.json | json | 0.507 | 0.600 | 0.760 | 94.562 | 0.38x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.050 | 0.057 | 0.094 | 85.938 | 1.00x |
| users.json $[*].id | jmespath | 0.274 | 0.280 | 0.304 | 85.938 | 0.20x |
| users.json $[*].id | jsonpath-ng | 1.449 | 1.476 | 1.539 | 85.938 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.310 | 0.338 | 0.427 | 86.203 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.672 | 1.745 | 2.028 | 86.203 | 0.19x |
| users.json $[*].orders[*].total | jsonpath-ng | 10.319 | 10.537 | 12.047 | 86.203 | 0.03x |
| users.json $..total | strata | 1.262 | 1.444 | 2.599 | 86.219 | 1.00x |
| users.json $..total | jsonpath-ng | 185.889 | 195.993 | 231.065 | 86.219 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.548 | 3.624 | 4.058 | 86.141 | 1.00x |
| users.json $[*].id | orjson+jmespath | 9.760 | 10.507 | 11.310 | 86.141 | 0.34x |
| users.json $[*].id | orjson+jsonpath-ng | 10.882 | 11.503 | 12.758 | 86.141 | 0.32x |
| users.json $[*].orders[*].total | strata | 3.566 | 3.657 | 4.253 | 86.219 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 11.144 | 11.324 | 11.754 | 86.219 | 0.32x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 21.927 | 22.372 | 24.865 | 86.219 | 0.16x |
| users.json $..total | strata | 7.814 | 8.278 | 9.839 | 86.250 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 196.738 | 204.079 | 231.631 | 86.250 | 0.04x |

