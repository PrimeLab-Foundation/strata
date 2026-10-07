# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: ec53f936b8a9245edf5bfeec36c38539666a1775
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
| users.json | strata | 5.865 | 6.682 | 9.137 | 68.359 | 1.00x |
| users.json | orjson | 8.781 | 10.141 | 12.944 | 68.359 | 0.66x |
| users.json | msgspec | 8.344 | 9.700 | 18.359 | 68.359 | 0.69x |
| users.json | ujson | 11.520 | 13.492 | 18.655 | 68.359 | 0.50x |
| users.json | pysimdjson | 129.311 | 134.552 | 153.784 | 68.359 | 0.05x |
| users.json | json | 14.178 | 15.845 | 32.635 | 68.359 | 0.42x |
| flat.json | strata | 0.578 | 0.594 | 0.673 | 99.078 | 1.00x |
| flat.json | orjson | 0.742 | 0.761 | 0.807 | 99.078 | 0.78x |
| flat.json | msgspec | 0.705 | 0.728 | 0.799 | 99.078 | 0.82x |
| flat.json | ujson | 1.168 | 1.222 | 1.280 | 99.078 | 0.49x |
| flat.json | pysimdjson | 11.711 | 11.741 | 12.209 | 99.078 | 0.05x |
| flat.json | json | 1.337 | 1.375 | 1.456 | 99.078 | 0.43x |
| nested.json | strata | 0.513 | 0.534 | 0.790 | 99.078 | 1.00x |
| nested.json | orjson | 0.720 | 0.763 | 0.942 | 99.078 | 0.70x |
| nested.json | msgspec | 0.681 | 0.708 | 0.881 | 99.078 | 0.75x |
| nested.json | ujson | 1.025 | 1.079 | 1.391 | 99.078 | 0.50x |
| nested.json | pysimdjson | 10.260 | 10.604 | 12.176 | 99.078 | 0.05x |
| nested.json | json | 1.436 | 1.473 | 2.038 | 99.078 | 0.36x |
| wide_arrays.json | strata | 2.986 | 3.552 | 3.717 | 101.984 | 1.00x |
| wide_arrays.json | orjson | 3.712 | 4.209 | 4.644 | 101.984 | 0.84x |
| wide_arrays.json | msgspec | 3.978 | 4.375 | 5.752 | 101.984 | 0.81x |
| wide_arrays.json | ujson | 5.268 | 5.585 | 6.833 | 101.984 | 0.64x |
| wide_arrays.json | pysimdjson | 63.433 | 67.193 | 76.620 | 101.984 | 0.05x |
| wide_arrays.json | json | 6.833 | 7.255 | 16.658 | 101.984 | 0.49x |
| mixed.json | strata | 0.119 | 0.122 | 0.134 | 102.141 | 1.00x |
| mixed.json | orjson | 0.153 | 0.162 | 0.168 | 102.141 | 0.75x |
| mixed.json | msgspec | 0.165 | 0.171 | 0.186 | 102.141 | 0.71x |
| mixed.json | ujson | 0.208 | 0.276 | 0.401 | 102.141 | 0.44x |
| mixed.json | pysimdjson | 2.449 | 2.502 | 2.584 | 102.141 | 0.05x |
| mixed.json | json | 0.306 | 0.321 | 0.367 | 102.141 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.652 | 1.873 | 4.256 | 80.531 | 1.00x |
| users.json | orjson | 2.467 | 2.720 | 3.102 | 80.531 | 0.69x |
| users.json | msgspec | 3.401 | 3.792 | 4.959 | 80.531 | 0.49x |
| users.json | ujson | 9.966 | 14.049 | 21.683 | 80.531 | 0.13x |
| users.json | json | 17.398 | 18.323 | 30.658 | 80.531 | 0.10x |
| flat.json | strata | 0.203 | 0.209 | 0.243 | 99.078 | 1.00x |
| flat.json | orjson | 0.433 | 0.462 | 0.496 | 99.078 | 0.45x |
| flat.json | msgspec | 0.324 | 0.328 | 0.339 | 99.078 | 0.64x |
| flat.json | ujson | 0.764 | 0.787 | 0.870 | 99.078 | 0.27x |
| flat.json | json | 1.329 | 1.361 | 1.523 | 99.078 | 0.15x |
| nested.json | strata | 0.127 | 0.138 | 0.169 | 99.078 | 1.00x |
| nested.json | orjson | 0.227 | 0.242 | 0.305 | 99.078 | 0.57x |
| nested.json | msgspec | 0.377 | 0.443 | 0.511 | 99.078 | 0.31x |
| nested.json | ujson | 0.862 | 0.997 | 2.995 | 99.078 | 0.14x |
| nested.json | json | 1.660 | 1.715 | 2.042 | 99.078 | 0.08x |
| wide_arrays.json | strata | 1.085 | 1.103 | 1.206 | 101.984 | 1.00x |
| wide_arrays.json | orjson | 1.433 | 1.486 | 1.590 | 101.984 | 0.74x |
| wide_arrays.json | msgspec | 2.101 | 2.169 | 2.291 | 101.984 | 0.51x |
| wide_arrays.json | ujson | 4.844 | 4.974 | 6.636 | 101.984 | 0.22x |
| wide_arrays.json | json | 11.814 | 11.942 | 13.089 | 101.984 | 0.09x |
| mixed.json | strata | 0.034 | 0.035 | 0.043 | 102.141 | 1.00x |
| mixed.json | orjson | 0.042 | 0.045 | 0.056 | 102.141 | 0.78x |
| mixed.json | msgspec | 0.049 | 0.080 | 0.172 | 102.141 | 0.44x |
| mixed.json | ujson | 0.163 | 0.166 | 0.197 | 102.141 | 0.21x |
| mixed.json | json | 0.340 | 0.350 | 0.412 | 102.141 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.747 | 7.903 | 9.602 | 93.375 | 1.00x |
| users.json | orjson | 11.240 | 11.630 | 12.973 | 93.375 | 0.68x |
| users.json | msgspec | 10.933 | 11.342 | 11.547 | 93.375 | 0.70x |
| users.json | ujson | 15.350 | 15.631 | 17.723 | 93.375 | 0.51x |
| users.json | json | 14.638 | 18.416 | 18.746 | 93.375 | 0.43x |
| flat.json | strata | 0.636 | 0.671 | 0.691 | 99.078 | 1.00x |
| flat.json | orjson | 0.985 | 1.039 | 1.080 | 99.078 | 0.65x |
| flat.json | msgspec | 0.797 | 0.841 | 0.906 | 99.078 | 0.80x |
| flat.json | ujson | 1.126 | 1.178 | 1.369 | 99.078 | 0.57x |
| flat.json | json | 1.419 | 1.508 | 1.551 | 99.078 | 0.44x |
| nested.json | strata | 0.541 | 0.582 | 0.752 | 99.078 | 1.00x |
| nested.json | orjson | 0.830 | 0.951 | 1.097 | 99.078 | 0.61x |
| nested.json | msgspec | 0.732 | 0.887 | 1.023 | 99.078 | 0.66x |
| nested.json | ujson | 1.011 | 1.102 | 1.357 | 99.078 | 0.53x |
| nested.json | json | 1.428 | 1.543 | 3.408 | 99.078 | 0.38x |
| wide_arrays.json | strata | 3.140 | 3.325 | 3.687 | 101.984 | 1.00x |
| wide_arrays.json | orjson | 3.795 | 4.007 | 4.365 | 101.984 | 0.83x |
| wide_arrays.json | msgspec | 4.333 | 4.596 | 4.939 | 101.984 | 0.72x |
| wide_arrays.json | ujson | 5.657 | 5.914 | 6.539 | 101.984 | 0.56x |
| wide_arrays.json | json | 7.047 | 7.304 | 8.519 | 101.984 | 0.46x |
| mixed.json | strata | 0.143 | 0.147 | 0.169 | 102.141 | 1.00x |
| mixed.json | orjson | 0.234 | 0.283 | 0.343 | 102.141 | 0.52x |
| mixed.json | msgspec | 0.201 | 0.208 | 0.237 | 102.141 | 0.71x |
| mixed.json | ujson | 0.248 | 0.258 | 0.282 | 102.141 | 0.57x |
| mixed.json | json | 0.339 | 0.349 | 0.389 | 102.141 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.872 | 7.140 | 8.029 | 99.078 | 1.00x |
| users.ndjson | orjson | 11.440 | 12.014 | 12.409 | 99.078 | 0.59x |
| users.ndjson | msgspec | 11.317 | 11.730 | 12.770 | 99.078 | 0.61x |
| users.ndjson | ujson | 14.003 | 14.540 | 15.068 | 99.078 | 0.49x |
| users.ndjson | json | 18.106 | 18.731 | 20.716 | 99.078 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.911 | 2.014 | 2.436 | 93.406 | 1.00x |
| users.json | orjson | 2.623 | 3.015 | 3.328 | 93.406 | 0.67x |
| users.json | msgspec | 3.251 | 3.690 | 4.329 | 93.406 | 0.55x |
| users.json | ujson | 9.093 | 10.309 | 11.426 | 93.406 | 0.20x |
| users.json | json | 15.904 | 17.119 | 19.686 | 93.406 | 0.12x |
| flat.json | strata | 0.357 | 0.402 | 0.622 | 99.078 | 1.00x |
| flat.json | orjson | 0.390 | 0.474 | 0.567 | 99.078 | 0.85x |
| flat.json | msgspec | 0.445 | 0.517 | 0.822 | 99.078 | 0.78x |
| flat.json | ujson | 0.941 | 0.997 | 1.352 | 99.078 | 0.40x |
| flat.json | json | 1.519 | 1.617 | 1.913 | 99.078 | 0.25x |
| nested.json | strata | 0.314 | 0.398 | 0.449 | 99.078 | 1.00x |
| nested.json | orjson | 0.435 | 0.518 | 0.601 | 99.078 | 0.77x |
| nested.json | msgspec | 0.544 | 0.662 | 0.786 | 99.078 | 0.60x |
| nested.json | ujson | 1.269 | 1.378 | 1.996 | 99.078 | 0.29x |
| nested.json | json | 2.190 | 2.333 | 2.416 | 99.078 | 0.17x |
| wide_arrays.json | strata | 1.444 | 1.542 | 1.811 | 102.125 | 1.00x |
| wide_arrays.json | orjson | 1.748 | 1.914 | 2.456 | 102.125 | 0.81x |
| wide_arrays.json | msgspec | 2.541 | 2.697 | 3.253 | 102.125 | 0.57x |
| wide_arrays.json | ujson | 5.459 | 5.603 | 5.806 | 102.125 | 0.28x |
| wide_arrays.json | json | 12.064 | 12.716 | 13.167 | 102.125 | 0.12x |
| mixed.json | strata | 0.157 | 0.184 | 0.267 | 102.141 | 1.00x |
| mixed.json | orjson | 0.179 | 0.208 | 0.360 | 102.141 | 0.89x |
| mixed.json | msgspec | 0.164 | 0.314 | 0.508 | 102.141 | 0.59x |
| mixed.json | ujson | 0.297 | 0.374 | 0.460 | 102.141 | 0.49x |
| mixed.json | json | 0.498 | 0.540 | 0.632 | 102.141 | 0.34x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.049 | 0.054 | 0.110 | 93.484 | 1.00x |
| users.json $[*].id | jmespath | 0.264 | 0.280 | 0.388 | 93.484 | 0.19x |
| users.json $[*].id | jsonpath-ng | 1.415 | 1.489 | 1.591 | 93.484 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.313 | 0.332 | 0.558 | 93.594 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.630 | 1.684 | 2.043 | 93.594 | 0.20x |
| users.json $[*].orders[*].total | jsonpath-ng | 10.304 | 10.793 | 13.404 | 93.594 | 0.03x |
| users.json $..total | strata | 1.288 | 1.492 | 2.030 | 93.625 | 1.00x |
| users.json $..total | jsonpath-ng | 204.768 | 213.057 | 264.560 | 93.625 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.501 | 3.598 | 4.158 | 93.531 | 1.00x |
| users.json $[*].id | orjson+jmespath | 9.957 | 10.340 | 12.980 | 93.531 | 0.35x |
| users.json $[*].id | orjson+jsonpath-ng | 11.149 | 11.724 | 13.130 | 93.531 | 0.31x |
| users.json $[*].orders[*].total | strata | 3.568 | 3.782 | 5.217 | 93.625 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 11.070 | 11.669 | 13.291 | 93.625 | 0.32x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 22.192 | 23.215 | 29.748 | 93.625 | 0.16x |
| users.json $..total | strata | 7.908 | 8.813 | 12.029 | 93.672 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 202.373 | 226.643 | 245.286 | 93.672 | 0.04x |

