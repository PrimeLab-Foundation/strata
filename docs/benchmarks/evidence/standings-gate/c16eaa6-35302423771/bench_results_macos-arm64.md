# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c16eaa65c61f1cf9d4fac46e9f6d8e6b9f3b087f
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
| users.json | strata | 5.991 | 6.419 | 7.251 | 68.203 | 1.00x |
| users.json | orjson | 8.982 | 9.888 | 10.961 | 68.203 | 0.65x |
| users.json | msgspec | 8.669 | 9.369 | 11.496 | 68.203 | 0.69x |
| users.json | ujson | 12.180 | 12.635 | 15.475 | 68.203 | 0.51x |
| users.json | pysimdjson | 120.834 | 126.634 | 139.842 | 68.203 | 0.05x |
| users.json | json | 14.246 | 15.666 | 17.553 | 68.203 | 0.41x |
| flat.json | strata | 0.591 | 0.631 | 0.719 | 100.719 | 1.00x |
| flat.json | orjson | 0.764 | 0.839 | 0.964 | 100.719 | 0.75x |
| flat.json | msgspec | 0.717 | 0.749 | 0.846 | 100.719 | 0.84x |
| flat.json | ujson | 1.154 | 1.197 | 1.742 | 100.719 | 0.53x |
| flat.json | pysimdjson | 12.291 | 12.480 | 14.987 | 100.719 | 0.05x |
| flat.json | json | 1.340 | 1.432 | 1.862 | 100.719 | 0.44x |
| nested.json | strata | 0.517 | 0.528 | 0.594 | 100.734 | 1.00x |
| nested.json | orjson | 0.741 | 0.781 | 0.909 | 100.734 | 0.68x |
| nested.json | msgspec | 0.684 | 0.703 | 0.751 | 100.734 | 0.75x |
| nested.json | ujson | 1.070 | 1.154 | 1.499 | 100.734 | 0.46x |
| nested.json | pysimdjson | 10.448 | 10.737 | 11.528 | 100.734 | 0.05x |
| nested.json | json | 1.453 | 1.513 | 1.813 | 100.734 | 0.35x |
| wide_arrays.json | strata | 3.226 | 3.700 | 4.955 | 103.469 | 1.00x |
| wide_arrays.json | orjson | 3.957 | 4.644 | 8.513 | 103.469 | 0.80x |
| wide_arrays.json | msgspec | 4.365 | 5.132 | 7.029 | 103.469 | 0.72x |
| wide_arrays.json | ujson | 5.592 | 6.541 | 11.677 | 103.469 | 0.57x |
| wide_arrays.json | pysimdjson | 66.070 | 70.662 | 94.676 | 103.469 | 0.05x |
| wide_arrays.json | json | 7.245 | 8.107 | 12.433 | 103.469 | 0.46x |
| mixed.json | strata | 0.127 | 0.129 | 0.150 | 103.484 | 1.00x |
| mixed.json | orjson | 0.159 | 0.168 | 0.176 | 103.484 | 0.77x |
| mixed.json | msgspec | 0.173 | 0.176 | 0.181 | 103.484 | 0.74x |
| mixed.json | ujson | 0.218 | 0.370 | 0.519 | 103.484 | 0.35x |
| mixed.json | pysimdjson | 2.528 | 2.557 | 2.590 | 103.484 | 0.05x |
| mixed.json | json | 0.333 | 0.341 | 0.346 | 103.484 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.457 | 1.558 | 1.644 | 81.375 | 1.00x |
| users.json | orjson | 2.266 | 2.405 | 2.466 | 81.375 | 0.65x |
| users.json | msgspec | 2.913 | 3.168 | 3.347 | 81.375 | 0.49x |
| users.json | ujson | 8.643 | 9.127 | 9.360 | 81.375 | 0.17x |
| users.json | json | 15.423 | 16.249 | 16.426 | 81.375 | 0.10x |
| flat.json | strata | 0.236 | 0.253 | 0.559 | 100.734 | 1.00x |
| flat.json | orjson | 0.261 | 0.283 | 0.459 | 100.734 | 0.89x |
| flat.json | msgspec | 0.327 | 0.361 | 0.531 | 100.734 | 0.70x |
| flat.json | ujson | 0.788 | 0.833 | 0.969 | 100.734 | 0.30x |
| flat.json | json | 1.402 | 1.748 | 2.301 | 100.734 | 0.14x |
| nested.json | strata | 0.132 | 0.141 | 0.182 | 100.734 | 1.00x |
| nested.json | orjson | 0.227 | 0.245 | 0.274 | 100.734 | 0.57x |
| nested.json | msgspec | 0.305 | 0.394 | 0.492 | 100.734 | 0.36x |
| nested.json | ujson | 0.832 | 0.849 | 1.192 | 100.734 | 0.17x |
| nested.json | json | 1.673 | 1.754 | 3.748 | 100.734 | 0.08x |
| wide_arrays.json | strata | 1.150 | 1.341 | 1.448 | 103.469 | 1.00x |
| wide_arrays.json | orjson | 1.512 | 1.677 | 1.841 | 103.469 | 0.80x |
| wide_arrays.json | msgspec | 2.286 | 2.537 | 2.865 | 103.469 | 0.53x |
| wide_arrays.json | ujson | 4.917 | 5.574 | 6.076 | 103.469 | 0.24x |
| wide_arrays.json | json | 12.054 | 12.634 | 13.508 | 103.469 | 0.11x |
| mixed.json | strata | 0.037 | 0.041 | 0.051 | 103.484 | 1.00x |
| mixed.json | orjson | 0.046 | 0.054 | 0.076 | 103.484 | 0.77x |
| mixed.json | msgspec | 0.056 | 0.140 | 0.379 | 103.484 | 0.30x |
| mixed.json | ujson | 0.174 | 0.181 | 0.214 | 103.484 | 0.23x |
| mixed.json | json | 0.354 | 0.370 | 0.422 | 103.484 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.373 | 6.809 | 7.322 | 93.062 | 1.00x |
| users.json | orjson | 9.281 | 10.144 | 10.651 | 93.062 | 0.67x |
| users.json | msgspec | 9.036 | 9.794 | 11.297 | 93.062 | 0.70x |
| users.json | ujson | 12.925 | 13.499 | 15.858 | 93.062 | 0.50x |
| users.json | json | 14.667 | 15.763 | 18.792 | 93.062 | 0.43x |
| flat.json | strata | 0.692 | 0.766 | 1.021 | 100.734 | 1.00x |
| flat.json | orjson | 0.980 | 1.073 | 1.246 | 100.734 | 0.71x |
| flat.json | msgspec | 0.858 | 0.904 | 1.124 | 100.734 | 0.85x |
| flat.json | ujson | 1.210 | 1.318 | 1.567 | 100.734 | 0.58x |
| flat.json | json | 1.447 | 1.499 | 1.716 | 100.734 | 0.51x |
| nested.json | strata | 0.694 | 0.743 | 1.598 | 100.734 | 1.00x |
| nested.json | orjson | 1.059 | 1.185 | 1.732 | 100.734 | 0.63x |
| nested.json | msgspec | 0.926 | 1.077 | 1.382 | 100.734 | 0.69x |
| nested.json | ujson | 1.263 | 1.588 | 2.031 | 100.734 | 0.47x |
| nested.json | json | 1.712 | 2.001 | 2.992 | 100.734 | 0.37x |
| wide_arrays.json | strata | 3.269 | 3.317 | 3.363 | 103.469 | 1.00x |
| wide_arrays.json | orjson | 3.997 | 4.039 | 4.089 | 103.469 | 0.82x |
| wide_arrays.json | msgspec | 4.495 | 4.514 | 4.546 | 103.469 | 0.73x |
| wide_arrays.json | ujson | 5.807 | 5.852 | 6.077 | 103.469 | 0.57x |
| wide_arrays.json | json | 7.211 | 7.334 | 7.660 | 103.469 | 0.45x |
| mixed.json | strata | 0.206 | 0.235 | 0.256 | 103.484 | 1.00x |
| mixed.json | orjson | 0.293 | 0.463 | 0.518 | 103.484 | 0.51x |
| mixed.json | msgspec | 0.290 | 0.320 | 0.368 | 103.484 | 0.73x |
| mixed.json | ujson | 0.350 | 0.394 | 0.565 | 103.484 | 0.60x |
| mixed.json | json | 0.444 | 0.504 | 0.531 | 103.484 | 0.47x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.200 | 7.492 | 9.083 | 100.625 | 1.00x |
| users.ndjson | orjson | 12.181 | 12.408 | 14.308 | 100.625 | 0.60x |
| users.ndjson | msgspec | 11.766 | 12.479 | 13.324 | 100.625 | 0.60x |
| users.ndjson | ujson | 14.900 | 15.383 | 17.196 | 100.625 | 0.49x |
| users.ndjson | json | 18.895 | 20.200 | 21.770 | 100.625 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.829 | 1.945 | 2.248 | 94.750 | 1.00x |
| users.json | orjson | 2.727 | 2.881 | 3.052 | 94.750 | 0.68x |
| users.json | msgspec | 3.503 | 3.730 | 4.720 | 94.750 | 0.52x |
| users.json | ujson | 9.629 | 10.213 | 10.452 | 94.750 | 0.19x |
| users.json | json | 16.460 | 17.336 | 18.892 | 94.750 | 0.11x |
| flat.json | strata | 0.478 | 0.526 | 0.749 | 100.734 | 1.00x |
| flat.json | orjson | 0.490 | 0.561 | 1.097 | 100.734 | 0.94x |
| flat.json | msgspec | 0.525 | 0.646 | 0.987 | 100.734 | 0.82x |
| flat.json | ujson | 1.042 | 1.163 | 1.789 | 100.734 | 0.45x |
| flat.json | json | 1.723 | 1.890 | 3.469 | 100.734 | 0.28x |
| nested.json | strata | 0.450 | 0.506 | 0.930 | 100.734 | 1.00x |
| nested.json | orjson | 0.557 | 0.715 | 1.016 | 100.734 | 0.71x |
| nested.json | msgspec | 0.651 | 1.098 | 3.870 | 100.734 | 0.46x |
| nested.json | ujson | 1.480 | 1.636 | 2.931 | 100.734 | 0.31x |
| nested.json | json | 2.196 | 2.582 | 5.474 | 100.734 | 0.20x |
| wide_arrays.json | strata | 1.335 | 1.693 | 1.808 | 103.469 | 1.00x |
| wide_arrays.json | orjson | 1.727 | 1.877 | 2.056 | 103.469 | 0.90x |
| wide_arrays.json | msgspec | 2.551 | 2.820 | 2.864 | 103.469 | 0.60x |
| wide_arrays.json | ujson | 5.242 | 5.669 | 6.372 | 103.469 | 0.30x |
| wide_arrays.json | json | 12.089 | 12.731 | 13.051 | 103.469 | 0.13x |
| mixed.json | strata | 0.237 | 0.269 | 0.363 | 103.484 | 1.00x |
| mixed.json | orjson | 0.257 | 0.323 | 0.398 | 103.484 | 0.83x |
| mixed.json | msgspec | 0.261 | 0.360 | 0.678 | 103.484 | 0.75x |
| mixed.json | ujson | 0.409 | 0.486 | 0.570 | 103.484 | 0.55x |
| mixed.json | json | 0.627 | 0.694 | 0.767 | 103.484 | 0.39x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.054 | 0.058 | 0.063 | 94.797 | 1.00x |
| users.json $[*].id | jmespath | 0.277 | 0.294 | 0.318 | 94.797 | 0.20x |
| users.json $[*].id | jsonpath-ng | 1.452 | 1.553 | 1.611 | 94.797 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.311 | 0.364 | 0.881 | 95.094 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.658 | 1.755 | 2.600 | 95.094 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 10.858 | 11.288 | 13.853 | 95.094 | 0.03x |
| users.json $..total | strata | 1.449 | 1.617 | 4.237 | 95.172 | 1.00x |
| users.json $..total | jsonpath-ng | 214.588 | 250.963 | 282.932 | 95.172 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.503 | 3.712 | 3.790 | 95.016 | 1.00x |
| users.json $[*].id | orjson+jmespath | 9.713 | 10.446 | 10.578 | 95.016 | 0.36x |
| users.json $[*].id | orjson+jsonpath-ng | 10.918 | 11.752 | 11.938 | 95.016 | 0.32x |
| users.json $[*].orders[*].total | strata | 3.900 | 4.308 | 5.253 | 95.141 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 11.434 | 15.069 | 16.262 | 95.141 | 0.29x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 23.800 | 29.462 | 44.611 | 95.141 | 0.15x |
| users.json $..total | strata | 8.612 | 10.147 | 13.623 | 95.172 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 220.363 | 247.018 | 269.849 | 95.172 | 0.04x |

