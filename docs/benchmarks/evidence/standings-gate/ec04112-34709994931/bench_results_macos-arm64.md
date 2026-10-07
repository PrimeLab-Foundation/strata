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
| users.json | strata | 5.853 | 6.743 | 8.397 | 68.469 | 1.00x |
| users.json | orjson | 9.150 | 10.200 | 10.946 | 68.469 | 0.66x |
| users.json | msgspec | 8.613 | 9.464 | 10.763 | 68.469 | 0.71x |
| users.json | ujson | 11.157 | 12.839 | 15.079 | 68.469 | 0.53x |
| users.json | pysimdjson | 122.142 | 127.275 | 141.442 | 68.469 | 0.05x |
| users.json | json | 14.092 | 15.650 | 18.448 | 68.469 | 0.43x |
| flat.json | strata | 0.531 | 0.535 | 0.559 | 99.594 | 1.00x |
| flat.json | orjson | 0.655 | 0.667 | 0.725 | 99.594 | 0.80x |
| flat.json | msgspec | 0.648 | 0.651 | 0.824 | 99.594 | 0.82x |
| flat.json | ujson | 1.041 | 1.093 | 1.191 | 99.594 | 0.49x |
| flat.json | pysimdjson | 11.091 | 11.107 | 11.353 | 99.594 | 0.05x |
| flat.json | json | 1.248 | 1.263 | 1.479 | 99.594 | 0.42x |
| nested.json | strata | 0.472 | 0.476 | 0.491 | 99.625 | 1.00x |
| nested.json | orjson | 0.645 | 0.663 | 0.694 | 99.625 | 0.72x |
| nested.json | msgspec | 0.630 | 0.636 | 0.694 | 99.625 | 0.75x |
| nested.json | ujson | 0.928 | 0.966 | 1.009 | 99.625 | 0.49x |
| nested.json | pysimdjson | 9.697 | 9.720 | 9.779 | 99.625 | 0.05x |
| nested.json | json | 1.302 | 1.313 | 1.354 | 99.625 | 0.36x |
| wide_arrays.json | strata | 2.774 | 2.793 | 3.067 | 102.562 | 1.00x |
| wide_arrays.json | orjson | 3.340 | 3.363 | 3.957 | 102.562 | 0.83x |
| wide_arrays.json | msgspec | 3.755 | 3.787 | 3.797 | 102.562 | 0.74x |
| wide_arrays.json | ujson | 4.803 | 4.883 | 5.078 | 102.562 | 0.57x |
| wide_arrays.json | pysimdjson | 59.801 | 59.946 | 64.742 | 102.562 | 0.05x |
| wide_arrays.json | json | 6.323 | 6.412 | 6.656 | 102.562 | 0.44x |
| mixed.json | strata | 0.113 | 0.115 | 0.123 | 102.578 | 1.00x |
| mixed.json | orjson | 0.144 | 0.148 | 0.153 | 102.578 | 0.78x |
| mixed.json | msgspec | 0.157 | 0.160 | 0.173 | 102.578 | 0.72x |
| mixed.json | ujson | 0.194 | 0.269 | 0.432 | 102.578 | 0.43x |
| mixed.json | pysimdjson | 2.338 | 2.348 | 2.379 | 102.578 | 0.05x |
| mixed.json | json | 0.300 | 0.301 | 0.319 | 102.578 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.343 | 1.433 | 2.031 | 82.156 | 1.00x |
| users.json | orjson | 2.195 | 2.405 | 3.113 | 82.156 | 0.60x |
| users.json | msgspec | 2.731 | 3.097 | 3.433 | 82.156 | 0.46x |
| users.json | ujson | 8.306 | 8.936 | 9.774 | 82.156 | 0.16x |
| users.json | json | 15.223 | 16.192 | 17.378 | 82.156 | 0.09x |
| flat.json | strata | 0.189 | 0.190 | 0.191 | 99.609 | 1.00x |
| flat.json | orjson | 0.232 | 0.234 | 0.247 | 99.609 | 0.81x |
| flat.json | msgspec | 0.289 | 0.290 | 0.315 | 99.609 | 0.66x |
| flat.json | ujson | 0.721 | 0.723 | 0.734 | 99.609 | 0.26x |
| flat.json | json | 1.269 | 1.274 | 1.287 | 99.609 | 0.15x |
| nested.json | strata | 0.112 | 0.113 | 0.116 | 99.625 | 1.00x |
| nested.json | orjson | 0.199 | 0.203 | 0.209 | 99.625 | 0.56x |
| nested.json | msgspec | 0.268 | 0.293 | 0.375 | 99.625 | 0.39x |
| nested.json | ujson | 0.745 | 0.747 | 0.765 | 99.625 | 0.15x |
| nested.json | json | 1.525 | 1.540 | 1.797 | 99.625 | 0.07x |
| wide_arrays.json | strata | 1.013 | 1.017 | 1.035 | 102.562 | 1.00x |
| wide_arrays.json | orjson | 1.262 | 1.334 | 1.453 | 102.562 | 0.76x |
| wide_arrays.json | msgspec | 1.979 | 2.030 | 2.178 | 102.562 | 0.50x |
| wide_arrays.json | ujson | 4.503 | 4.559 | 4.613 | 102.562 | 0.22x |
| wide_arrays.json | json | 10.944 | 11.088 | 11.200 | 102.562 | 0.09x |
| mixed.json | strata | 0.032 | 0.032 | 0.043 | 102.578 | 1.00x |
| mixed.json | orjson | 0.040 | 0.091 | 0.177 | 102.578 | 0.36x |
| mixed.json | msgspec | 0.047 | 0.049 | 0.050 | 102.578 | 0.67x |
| mixed.json | ujson | 0.157 | 0.159 | 0.167 | 102.578 | 0.20x |
| mixed.json | json | 0.322 | 0.324 | 0.337 | 102.578 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.298 | 6.418 | 6.915 | 93.453 | 1.00x |
| users.json | orjson | 8.968 | 9.399 | 9.910 | 93.453 | 0.68x |
| users.json | msgspec | 8.734 | 9.184 | 9.647 | 93.453 | 0.70x |
| users.json | ujson | 11.859 | 12.917 | 13.427 | 93.453 | 0.50x |
| users.json | json | 14.164 | 14.696 | 15.309 | 93.453 | 0.44x |
| flat.json | strata | 0.564 | 0.568 | 0.581 | 99.609 | 1.00x |
| flat.json | orjson | 0.796 | 0.814 | 0.917 | 99.609 | 0.70x |
| flat.json | msgspec | 0.702 | 0.707 | 0.743 | 99.609 | 0.80x |
| flat.json | ujson | 1.040 | 1.058 | 1.107 | 99.609 | 0.54x |
| flat.json | json | 1.297 | 1.311 | 1.328 | 99.609 | 0.43x |
| nested.json | strata | 0.509 | 0.513 | 0.521 | 99.625 | 1.00x |
| nested.json | orjson | 0.803 | 0.821 | 0.919 | 99.625 | 0.63x |
| nested.json | msgspec | 0.686 | 0.692 | 0.701 | 99.625 | 0.74x |
| nested.json | ujson | 0.970 | 0.974 | 0.984 | 99.625 | 0.53x |
| nested.json | json | 1.348 | 1.350 | 1.388 | 99.625 | 0.38x |
| wide_arrays.json | strata | 2.960 | 2.994 | 3.088 | 102.562 | 1.00x |
| wide_arrays.json | orjson | 3.555 | 3.575 | 3.607 | 102.562 | 0.84x |
| wide_arrays.json | msgspec | 4.102 | 4.131 | 4.335 | 102.562 | 0.72x |
| wide_arrays.json | ujson | 5.291 | 5.416 | 5.456 | 102.562 | 0.55x |
| wide_arrays.json | json | 6.650 | 6.698 | 6.776 | 102.562 | 0.45x |
| mixed.json | strata | 0.135 | 0.139 | 0.155 | 102.578 | 1.00x |
| mixed.json | orjson | 0.289 | 0.292 | 0.441 | 102.578 | 0.48x |
| mixed.json | msgspec | 0.198 | 0.201 | 0.300 | 102.578 | 0.69x |
| mixed.json | ujson | 0.237 | 0.239 | 0.261 | 102.578 | 0.58x |
| mixed.json | json | 0.333 | 0.334 | 0.342 | 102.578 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.327 | 6.348 | 6.558 | 99.594 | 1.00x |
| users.ndjson | orjson | 10.819 | 11.010 | 11.136 | 99.594 | 0.58x |
| users.ndjson | msgspec | 10.711 | 10.782 | 10.929 | 99.594 | 0.59x |
| users.ndjson | ujson | 13.302 | 13.364 | 13.537 | 99.594 | 0.47x |
| users.ndjson | json | 17.185 | 17.212 | 18.198 | 99.594 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.809 | 1.894 | 2.631 | 93.922 | 1.00x |
| users.json | orjson | 2.693 | 2.808 | 3.170 | 93.922 | 0.67x |
| users.json | msgspec | 3.302 | 3.505 | 3.917 | 93.922 | 0.54x |
| users.json | ujson | 9.020 | 9.122 | 9.989 | 93.922 | 0.21x |
| users.json | json | 15.421 | 15.550 | 16.652 | 93.922 | 0.12x |
| flat.json | strata | 0.294 | 0.319 | 0.418 | 99.625 | 1.00x |
| flat.json | orjson | 0.348 | 0.372 | 0.459 | 99.625 | 0.86x |
| flat.json | msgspec | 0.401 | 0.425 | 0.519 | 99.625 | 0.75x |
| flat.json | ujson | 0.851 | 0.873 | 1.166 | 99.625 | 0.37x |
| flat.json | json | 1.393 | 1.509 | 1.568 | 99.625 | 0.21x |
| nested.json | strata | 0.222 | 0.235 | 0.294 | 99.625 | 1.00x |
| nested.json | orjson | 0.321 | 0.326 | 0.383 | 99.625 | 0.72x |
| nested.json | msgspec | 0.379 | 0.427 | 0.501 | 99.625 | 0.55x |
| nested.json | ujson | 0.949 | 0.989 | 1.147 | 99.625 | 0.24x |
| nested.json | json | 1.666 | 1.701 | 2.420 | 99.625 | 0.14x |
| wide_arrays.json | strata | 1.308 | 1.346 | 1.666 | 102.562 | 1.00x |
| wide_arrays.json | orjson | 1.661 | 1.709 | 1.824 | 102.562 | 0.79x |
| wide_arrays.json | msgspec | 2.408 | 2.474 | 2.709 | 102.562 | 0.54x |
| wide_arrays.json | ujson | 4.930 | 5.059 | 5.435 | 102.562 | 0.27x |
| wide_arrays.json | json | 11.518 | 11.781 | 12.083 | 102.562 | 0.11x |
| mixed.json | strata | 0.117 | 0.129 | 0.395 | 102.578 | 1.00x |
| mixed.json | orjson | 0.132 | 0.164 | 0.282 | 102.578 | 0.79x |
| mixed.json | msgspec | 0.138 | 0.154 | 0.390 | 102.578 | 0.84x |
| mixed.json | ujson | 0.259 | 0.266 | 0.295 | 102.578 | 0.48x |
| mixed.json | json | 0.419 | 0.431 | 0.465 | 102.578 | 0.30x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.048 | 0.052 | 0.067 | 93.969 | 1.00x |
| users.json $[*].id | jmespath | 0.258 | 0.269 | 0.329 | 93.969 | 0.19x |
| users.json $[*].id | jsonpath-ng | 1.396 | 1.434 | 1.530 | 93.969 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.267 | 0.281 | 0.452 | 94.094 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.532 | 1.552 | 1.685 | 94.094 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 9.703 | 9.867 | 10.723 | 94.094 | 0.03x |
| users.json $..total | strata | 1.205 | 1.272 | 1.699 | 94.125 | 1.00x |
| users.json $..total | jsonpath-ng | 182.104 | 192.220 | 213.812 | 94.125 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.383 | 3.409 | 3.573 | 94.000 | 1.00x |
| users.json $[*].id | orjson+jmespath | 9.297 | 9.379 | 10.000 | 94.000 | 0.36x |
| users.json $[*].id | orjson+jsonpath-ng | 10.534 | 10.678 | 11.527 | 94.000 | 0.32x |
| users.json $[*].orders[*].total | strata | 3.446 | 3.549 | 4.020 | 94.125 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 10.503 | 10.673 | 12.040 | 94.125 | 0.33x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 20.489 | 20.936 | 27.401 | 94.125 | 0.17x |
| users.json $..total | strata | 7.427 | 7.594 | 9.271 | 94.125 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 187.804 | 190.851 | 212.587 | 94.125 | 0.04x |

