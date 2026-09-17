# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 38cf28416465e176915ffd24d827a95015eacdbd
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
| users.json | strata | 5.760 | 5.922 | 6.691 | 69.156 | 1.00x |
| users.json | orjson | 8.607 | 8.721 | 9.877 | 69.156 | 0.68x |
| users.json | msgspec | 8.273 | 8.405 | 9.546 | 69.156 | 0.70x |
| users.json | ujson | 10.964 | 11.310 | 13.051 | 69.156 | 0.52x |
| users.json | pysimdjson | 115.631 | 118.457 | 127.762 | 69.156 | 0.05x |
| users.json | json | 13.703 | 13.846 | 15.869 | 69.156 | 0.43x |
| flat.json | strata | 0.565 | 0.635 | 0.770 | 94.672 | 1.00x |
| flat.json | orjson | 0.694 | 0.852 | 1.035 | 94.672 | 0.75x |
| flat.json | msgspec | 0.696 | 0.759 | 0.947 | 94.672 | 0.84x |
| flat.json | ujson | 1.151 | 1.305 | 1.428 | 94.672 | 0.49x |
| flat.json | pysimdjson | 11.110 | 12.234 | 13.250 | 94.672 | 0.05x |
| flat.json | json | 1.248 | 1.378 | 1.688 | 94.672 | 0.46x |
| nested.json | strata | 0.479 | 0.482 | 0.492 | 95.328 | 1.00x |
| nested.json | orjson | 0.654 | 0.665 | 0.700 | 95.328 | 0.73x |
| nested.json | msgspec | 0.642 | 0.646 | 0.656 | 95.328 | 0.75x |
| nested.json | ujson | 0.927 | 0.940 | 1.096 | 95.328 | 0.51x |
| nested.json | pysimdjson | 9.694 | 9.775 | 9.846 | 95.328 | 0.05x |
| nested.json | json | 1.319 | 1.327 | 1.340 | 95.328 | 0.36x |
| wide_arrays.json | strata | 2.772 | 2.808 | 2.969 | 97.562 | 1.00x |
| wide_arrays.json | orjson | 3.344 | 3.388 | 3.732 | 97.562 | 0.83x |
| wide_arrays.json | msgspec | 3.759 | 3.803 | 4.221 | 97.562 | 0.74x |
| wide_arrays.json | ujson | 4.861 | 4.895 | 5.278 | 97.562 | 0.57x |
| wide_arrays.json | pysimdjson | 59.735 | 59.958 | 62.389 | 97.562 | 0.05x |
| wide_arrays.json | json | 6.337 | 6.358 | 7.050 | 97.562 | 0.44x |
| mixed.json | strata | 0.113 | 0.115 | 0.132 | 97.578 | 1.00x |
| mixed.json | orjson | 0.144 | 0.145 | 0.159 | 97.578 | 0.79x |
| mixed.json | msgspec | 0.157 | 0.158 | 0.165 | 97.578 | 0.73x |
| mixed.json | ujson | 0.244 | 0.250 | 0.317 | 97.578 | 0.46x |
| mixed.json | pysimdjson | 2.342 | 2.357 | 2.378 | 97.578 | 0.05x |
| mixed.json | json | 0.294 | 0.297 | 0.317 | 97.578 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.395 | 1.575 | 1.646 | 79.766 | 1.00x |
| users.json | orjson | 2.159 | 2.397 | 2.548 | 79.766 | 0.66x |
| users.json | msgspec | 2.709 | 3.016 | 3.085 | 79.766 | 0.52x |
| users.json | ujson | 8.323 | 9.123 | 9.588 | 79.766 | 0.17x |
| users.json | json | 14.760 | 16.278 | 16.485 | 79.766 | 0.10x |
| flat.json | strata | 0.267 | 0.269 | 0.443 | 94.672 | 1.00x |
| flat.json | orjson | 0.246 | 0.248 | 0.322 | 94.672 | 1.08x |
| flat.json | msgspec | 0.293 | 0.299 | 0.329 | 94.672 | 0.90x |
| flat.json | ujson | 0.781 | 0.811 | 0.910 | 94.672 | 0.33x |
| flat.json | json | 1.277 | 1.290 | 1.708 | 94.672 | 0.21x |
| nested.json | strata | 0.113 | 0.113 | 0.118 | 95.328 | 1.00x |
| nested.json | orjson | 0.205 | 0.206 | 0.209 | 95.328 | 0.55x |
| nested.json | msgspec | 0.266 | 0.268 | 0.356 | 95.328 | 0.42x |
| nested.json | ujson | 0.833 | 0.843 | 0.875 | 95.328 | 0.13x |
| nested.json | json | 1.525 | 1.535 | 1.570 | 95.328 | 0.07x |
| wide_arrays.json | strata | 1.005 | 1.025 | 1.046 | 97.562 | 1.00x |
| wide_arrays.json | orjson | 1.217 | 1.324 | 1.377 | 97.562 | 0.77x |
| wide_arrays.json | msgspec | 2.010 | 2.019 | 2.041 | 97.562 | 0.51x |
| wide_arrays.json | ujson | 4.494 | 4.504 | 4.595 | 97.562 | 0.23x |
| wide_arrays.json | json | 10.980 | 10.999 | 11.086 | 97.562 | 0.09x |
| mixed.json | strata | 0.031 | 0.032 | 0.034 | 97.578 | 1.00x |
| mixed.json | orjson | 0.041 | 0.066 | 0.143 | 97.578 | 0.49x |
| mixed.json | msgspec | 0.046 | 0.048 | 0.051 | 97.578 | 0.66x |
| mixed.json | ujson | 0.157 | 0.160 | 0.161 | 97.578 | 0.20x |
| mixed.json | json | 0.324 | 0.326 | 0.329 | 97.578 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.191 | 6.561 | 7.672 | 90.750 | 1.00x |
| users.json | orjson | 9.006 | 9.650 | 12.491 | 90.750 | 0.68x |
| users.json | msgspec | 8.666 | 9.309 | 12.887 | 90.750 | 0.70x |
| users.json | ujson | 11.860 | 12.506 | 15.623 | 90.750 | 0.52x |
| users.json | json | 14.187 | 14.811 | 17.977 | 90.750 | 0.44x |
| flat.json | strata | 0.591 | 0.605 | 0.717 | 95.328 | 1.00x |
| flat.json | orjson | 0.768 | 0.818 | 0.969 | 95.328 | 0.74x |
| flat.json | msgspec | 0.704 | 0.716 | 0.747 | 95.328 | 0.84x |
| flat.json | ujson | 1.018 | 1.029 | 1.154 | 95.328 | 0.59x |
| flat.json | json | 1.272 | 1.287 | 1.559 | 95.328 | 0.47x |
| nested.json | strata | 0.510 | 0.513 | 0.761 | 95.328 | 1.00x |
| nested.json | orjson | 0.792 | 0.825 | 0.900 | 95.328 | 0.62x |
| nested.json | msgspec | 0.696 | 0.707 | 0.771 | 95.328 | 0.73x |
| nested.json | ujson | 0.958 | 0.970 | 0.992 | 95.328 | 0.53x |
| nested.json | json | 1.366 | 1.371 | 1.386 | 95.328 | 0.37x |
| wide_arrays.json | strata | 2.979 | 3.033 | 3.176 | 97.562 | 1.00x |
| wide_arrays.json | orjson | 3.567 | 3.619 | 4.165 | 97.562 | 0.84x |
| wide_arrays.json | msgspec | 4.096 | 4.127 | 4.440 | 97.562 | 0.73x |
| wide_arrays.json | ujson | 5.261 | 5.353 | 5.729 | 97.562 | 0.57x |
| wide_arrays.json | json | 6.640 | 6.701 | 7.140 | 97.562 | 0.45x |
| mixed.json | strata | 0.132 | 0.135 | 0.156 | 97.578 | 1.00x |
| mixed.json | orjson | 0.237 | 0.247 | 0.298 | 97.578 | 0.55x |
| mixed.json | msgspec | 0.194 | 0.199 | 0.215 | 97.578 | 0.68x |
| mixed.json | ujson | 0.234 | 0.239 | 0.245 | 97.578 | 0.56x |
| mixed.json | json | 0.330 | 0.332 | 0.336 | 97.578 | 0.41x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.283 | 7.101 | 8.210 | 94.672 | 1.00x |
| users.ndjson | orjson | 10.828 | 12.214 | 18.087 | 94.672 | 0.58x |
| users.ndjson | msgspec | 10.683 | 12.006 | 17.408 | 94.672 | 0.59x |
| users.ndjson | ujson | 13.287 | 14.897 | 19.710 | 94.672 | 0.48x |
| users.ndjson | json | 17.233 | 19.328 | 24.956 | 94.672 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.628 | 1.809 | 1.914 | 90.781 | 1.00x |
| users.json | orjson | 2.427 | 2.571 | 3.014 | 90.781 | 0.70x |
| users.json | msgspec | 3.102 | 3.218 | 3.777 | 90.781 | 0.56x |
| users.json | ujson | 8.873 | 9.078 | 9.295 | 90.781 | 0.20x |
| users.json | json | 15.196 | 15.441 | 16.209 | 90.781 | 0.12x |
| flat.json | strata | 0.309 | 0.338 | 0.462 | 95.328 | 1.00x |
| flat.json | orjson | 0.340 | 0.363 | 0.529 | 95.328 | 0.93x |
| flat.json | msgspec | 0.409 | 0.424 | 0.552 | 95.328 | 0.80x |
| flat.json | ujson | 0.834 | 0.862 | 1.099 | 95.328 | 0.39x |
| flat.json | json | 1.423 | 1.524 | 1.590 | 95.328 | 0.22x |
| nested.json | strata | 0.208 | 0.236 | 0.290 | 95.328 | 1.00x |
| nested.json | orjson | 0.311 | 0.319 | 0.427 | 95.328 | 0.74x |
| nested.json | msgspec | 0.373 | 0.503 | 0.657 | 95.328 | 0.47x |
| nested.json | ujson | 0.875 | 0.920 | 1.031 | 95.328 | 0.26x |
| nested.json | json | 1.646 | 1.686 | 1.891 | 95.328 | 0.14x |
| wide_arrays.json | strata | 1.323 | 1.386 | 1.597 | 97.562 | 1.00x |
| wide_arrays.json | orjson | 1.587 | 1.720 | 1.916 | 97.562 | 0.81x |
| wide_arrays.json | msgspec | 2.419 | 2.638 | 3.133 | 97.562 | 0.53x |
| wide_arrays.json | ujson | 4.920 | 5.167 | 6.225 | 97.562 | 0.27x |
| wide_arrays.json | json | 11.372 | 11.672 | 12.458 | 97.562 | 0.12x |
| mixed.json | strata | 0.113 | 0.131 | 0.148 | 97.578 | 1.00x |
| mixed.json | orjson | 0.133 | 0.190 | 0.568 | 97.578 | 0.69x |
| mixed.json | msgspec | 0.137 | 0.154 | 0.463 | 97.578 | 0.85x |
| mixed.json | ujson | 0.257 | 0.285 | 0.543 | 97.578 | 0.46x |
| mixed.json | json | 0.426 | 0.442 | 0.454 | 97.578 | 0.30x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.046 | 0.069 | 0.116 | 90.828 | 1.00x |
| users.json $[*].id | jmespath | 0.253 | 0.311 | 0.454 | 90.828 | 0.22x |
| users.json $[*].id | jsonpath-ng | 1.387 | 1.742 | 2.087 | 90.828 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.277 | 0.294 | 0.587 | 91.016 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.552 | 1.592 | 1.897 | 91.016 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 9.616 | 9.962 | 11.479 | 91.016 | 0.03x |
| users.json $..total | strata | 1.209 | 1.269 | 1.367 | 91.016 | 1.00x |
| users.json $..total | jsonpath-ng | 179.436 | 179.775 | 183.312 | 91.016 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.362 | 3.441 | 4.057 | 90.859 | 1.00x |
| users.json $[*].id | orjson+jmespath | 9.210 | 9.308 | 9.861 | 90.859 | 0.37x |
| users.json $[*].id | orjson+jsonpath-ng | 10.369 | 10.548 | 11.923 | 90.859 | 0.33x |
| users.json $[*].orders[*].total | strata | 3.395 | 3.624 | 4.094 | 91.016 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 10.420 | 10.897 | 12.326 | 91.016 | 0.33x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 20.500 | 21.111 | 25.896 | 91.016 | 0.17x |
| users.json $..total | strata | 7.400 | 7.617 | 8.305 | 91.047 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 187.965 | 190.322 | 215.150 | 91.047 | 0.04x |

