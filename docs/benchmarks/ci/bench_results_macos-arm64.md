# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c20ac86eedff410e10c973bc3b1f19f6e9a5f56e
- python: 3.12.10
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit
- machine: arm64
- processor: Apple M2 Pro (Virtual)
- compiler_flags: -fno-strict-overflow -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -O3 -Wall -arch arm64 -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -march=native -flto=thin -fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata -bundle -undefined (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 5.104 | 5.321 | 6.262 | 71.109 | 1.00x |
| users.json | orjson | 7.679 | 8.255 | 9.417 | 71.109 | 0.64x |
| users.json | msgspec | 7.610 | 7.908 | 9.426 | 71.109 | 0.67x |
| users.json | ujson | 9.889 | 10.277 | 12.098 | 71.109 | 0.52x |
| users.json | pysimdjson | 110.977 | 113.454 | 121.810 | 71.109 | 0.05x |
| users.json | json | 12.654 | 12.987 | 13.873 | 71.109 | 0.41x |
| flat.json | strata | 0.479 | 0.481 | 0.497 | 107.125 | 1.00x |
| flat.json | orjson | 0.577 | 0.579 | 0.608 | 107.125 | 0.83x |
| flat.json | msgspec | 0.591 | 0.594 | 0.608 | 107.125 | 0.81x |
| flat.json | ujson | 0.864 | 0.869 | 0.881 | 107.125 | 0.55x |
| flat.json | pysimdjson | 10.589 | 10.637 | 10.671 | 107.125 | 0.05x |
| flat.json | json | 1.118 | 1.126 | 1.148 | 107.125 | 0.43x |
| nested.json | strata | 0.406 | 0.414 | 0.424 | 107.156 | 1.00x |
| nested.json | orjson | 0.559 | 0.559 | 0.567 | 107.156 | 0.74x |
| nested.json | msgspec | 0.564 | 0.565 | 0.616 | 107.156 | 0.73x |
| nested.json | ujson | 0.810 | 0.812 | 0.829 | 107.156 | 0.51x |
| nested.json | pysimdjson | 9.333 | 9.350 | 9.412 | 107.156 | 0.04x |
| nested.json | json | 1.209 | 1.220 | 1.226 | 107.156 | 0.34x |
| wide_arrays.json | strata | 2.466 | 2.500 | 2.806 | 110.969 | 1.00x |
| wide_arrays.json | orjson | 2.827 | 2.895 | 3.332 | 110.969 | 0.86x |
| wide_arrays.json | msgspec | 3.279 | 3.374 | 4.119 | 110.969 | 0.74x |
| wide_arrays.json | ujson | 4.330 | 4.377 | 4.791 | 110.969 | 0.57x |
| wide_arrays.json | pysimdjson | 58.113 | 58.211 | 60.834 | 110.969 | 0.04x |
| wide_arrays.json | json | 5.485 | 5.504 | 5.833 | 110.969 | 0.45x |
| mixed.json | strata | 0.099 | 0.105 | 0.142 | 110.984 | 1.00x |
| mixed.json | orjson | 0.127 | 0.142 | 0.173 | 110.984 | 0.74x |
| mixed.json | msgspec | 0.141 | 0.150 | 0.173 | 110.984 | 0.70x |
| mixed.json | ujson | 0.175 | 0.215 | 0.300 | 110.984 | 0.49x |
| mixed.json | pysimdjson | 2.252 | 2.329 | 2.752 | 110.984 | 0.05x |
| mixed.json | json | 0.269 | 0.288 | 0.328 | 110.984 | 0.36x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.275 | 1.333 | 1.407 | 77.719 | 1.00x |
| users.json | orjson | 1.909 | 1.935 | 2.357 | 77.719 | 0.69x |
| users.json | msgspec | 2.438 | 2.474 | 2.591 | 77.719 | 0.54x |
| users.json | ujson | 7.506 | 7.626 | 8.164 | 77.719 | 0.17x |
| users.json | json | 14.135 | 14.529 | 15.141 | 77.719 | 0.09x |
| flat.json | strata | 0.167 | 0.169 | 0.181 | 107.156 | 1.00x |
| flat.json | orjson | 0.206 | 0.208 | 0.219 | 107.156 | 0.81x |
| flat.json | msgspec | 0.260 | 0.261 | 0.274 | 107.156 | 0.65x |
| flat.json | ujson | 0.617 | 0.619 | 0.633 | 107.156 | 0.27x |
| flat.json | json | 1.185 | 1.201 | 1.209 | 107.156 | 0.14x |
| nested.json | strata | 0.104 | 0.105 | 0.129 | 107.172 | 1.00x |
| nested.json | orjson | 0.183 | 0.185 | 0.203 | 107.172 | 0.57x |
| nested.json | msgspec | 0.240 | 0.245 | 0.265 | 107.172 | 0.43x |
| nested.json | ujson | 0.710 | 0.737 | 0.778 | 107.172 | 0.14x |
| nested.json | json | 1.474 | 1.502 | 1.596 | 107.172 | 0.07x |
| wide_arrays.json | strata | 0.902 | 0.912 | 0.929 | 110.969 | 1.00x |
| wide_arrays.json | orjson | 1.083 | 1.204 | 1.430 | 110.969 | 0.76x |
| wide_arrays.json | msgspec | 1.785 | 1.801 | 1.815 | 110.969 | 0.51x |
| wide_arrays.json | ujson | 3.807 | 3.811 | 3.840 | 110.969 | 0.24x |
| wide_arrays.json | json | 10.487 | 10.870 | 11.066 | 110.969 | 0.08x |
| mixed.json | strata | 0.029 | 0.030 | 0.048 | 110.984 | 1.00x |
| mixed.json | orjson | 0.036 | 0.038 | 0.057 | 110.984 | 0.79x |
| mixed.json | msgspec | 0.041 | 0.044 | 0.052 | 110.984 | 0.69x |
| mixed.json | ujson | 0.143 | 0.152 | 0.171 | 110.984 | 0.20x |
| mixed.json | json | 0.301 | 0.319 | 0.348 | 110.984 | 0.09x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 5.363 | 5.456 | 6.514 | 91.266 | 1.00x |
| users.json | orjson | 7.825 | 8.086 | 8.930 | 91.266 | 0.67x |
| users.json | msgspec | 7.916 | 8.100 | 9.063 | 91.266 | 0.67x |
| users.json | ujson | 10.695 | 10.917 | 13.536 | 91.266 | 0.50x |
| users.json | json | 12.791 | 13.125 | 15.075 | 91.266 | 0.42x |
| flat.json | strata | 0.503 | 0.504 | 0.526 | 107.156 | 1.00x |
| flat.json | orjson | 0.630 | 0.638 | 0.643 | 107.156 | 0.79x |
| flat.json | msgspec | 0.623 | 0.626 | 0.660 | 107.156 | 0.80x |
| flat.json | ujson | 0.898 | 0.907 | 0.922 | 107.156 | 0.56x |
| flat.json | json | 1.149 | 1.167 | 1.238 | 107.156 | 0.43x |
| nested.json | strata | 0.431 | 0.450 | 0.473 | 107.172 | 1.00x |
| nested.json | orjson | 0.627 | 0.653 | 0.670 | 107.172 | 0.69x |
| nested.json | msgspec | 0.625 | 0.632 | 0.651 | 107.172 | 0.71x |
| nested.json | ujson | 0.835 | 0.872 | 0.894 | 107.172 | 0.52x |
| nested.json | json | 1.306 | 1.346 | 1.368 | 107.172 | 0.33x |
| wide_arrays.json | strata | 2.592 | 2.600 | 2.612 | 110.969 | 1.00x |
| wide_arrays.json | orjson | 2.969 | 2.997 | 3.075 | 110.969 | 0.87x |
| wide_arrays.json | msgspec | 3.473 | 3.499 | 4.008 | 110.969 | 0.74x |
| wide_arrays.json | ujson | 4.576 | 4.615 | 5.011 | 110.969 | 0.56x |
| wide_arrays.json | json | 5.660 | 5.708 | 5.763 | 110.969 | 0.46x |
| mixed.json | strata | 0.122 | 0.134 | 0.155 | 110.984 | 1.00x |
| mixed.json | orjson | 0.170 | 0.198 | 0.290 | 110.984 | 0.68x |
| mixed.json | msgspec | 0.173 | 0.180 | 0.192 | 110.984 | 0.74x |
| mixed.json | ujson | 0.219 | 0.240 | 0.395 | 110.984 | 0.56x |
| mixed.json | json | 0.302 | 0.325 | 0.498 | 110.984 | 0.41x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 5.413 | 5.442 | 5.595 | 107.094 | 1.00x |
| users.ndjson | orjson | 9.314 | 9.430 | 10.204 | 107.094 | 0.58x |
| users.ndjson | msgspec | 9.548 | 9.614 | 10.260 | 107.094 | 0.57x |
| users.ndjson | ujson | 11.886 | 11.953 | 12.817 | 107.094 | 0.46x |
| users.ndjson | json | 15.478 | 15.623 | 16.930 | 107.094 | 0.35x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.488 | 1.512 | 1.659 | 101.328 | 1.00x |
| users.json | orjson | 2.167 | 2.220 | 2.557 | 101.328 | 0.68x |
| users.json | msgspec | 2.724 | 2.886 | 7.989 | 101.328 | 0.52x |
| users.json | ujson | 7.932 | 8.101 | 13.349 | 101.328 | 0.19x |
| users.json | json | 14.309 | 14.933 | 15.840 | 101.328 | 0.10x |
| flat.json | strata | 0.239 | 0.255 | 0.265 | 107.156 | 1.00x |
| flat.json | orjson | 0.279 | 0.281 | 0.344 | 107.156 | 0.91x |
| flat.json | msgspec | 0.332 | 0.339 | 0.373 | 107.156 | 0.75x |
| flat.json | ujson | 0.705 | 0.711 | 0.726 | 107.156 | 0.36x |
| flat.json | json | 1.264 | 1.328 | 1.508 | 107.156 | 0.19x |
| nested.json | strata | 0.175 | 0.188 | 0.212 | 107.172 | 1.00x |
| nested.json | orjson | 0.253 | 0.261 | 0.320 | 107.172 | 0.72x |
| nested.json | msgspec | 0.332 | 0.368 | 0.381 | 107.172 | 0.51x |
| nested.json | ujson | 0.767 | 0.815 | 1.088 | 107.172 | 0.23x |
| nested.json | json | 1.526 | 1.605 | 1.659 | 107.172 | 0.12x |
| wide_arrays.json | strata | 1.121 | 1.334 | 1.600 | 110.969 | 1.00x |
| wide_arrays.json | orjson | 1.303 | 1.499 | 1.846 | 110.969 | 0.89x |
| wide_arrays.json | msgspec | 1.957 | 2.204 | 2.616 | 110.969 | 0.61x |
| wide_arrays.json | ujson | 4.140 | 5.125 | 5.694 | 110.969 | 0.26x |
| wide_arrays.json | json | 10.932 | 12.302 | 14.240 | 110.969 | 0.11x |
| mixed.json | strata | 0.083 | 0.090 | 0.127 | 110.984 | 1.00x |
| mixed.json | orjson | 0.094 | 0.105 | 0.148 | 110.984 | 0.86x |
| mixed.json | msgspec | 0.096 | 0.110 | 0.201 | 110.984 | 0.82x |
| mixed.json | ujson | 0.204 | 0.223 | 0.311 | 110.984 | 0.41x |
| mixed.json | json | 0.361 | 0.377 | 0.469 | 110.984 | 0.24x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.034 | 0.042 | 0.063 | 101.391 | 1.00x |
| users.json $[*].id | jmespath | 0.222 | 0.237 | 0.320 | 101.391 | 0.18x |
| users.json $[*].id | jsonpath-ng | 1.257 | 1.285 | 1.782 | 101.391 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.199 | 0.205 | 0.252 | 101.500 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.376 | 1.403 | 1.442 | 101.500 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 8.737 | 8.812 | 8.981 | 101.500 | 0.02x |
| users.json $..total | strata | 1.066 | 1.213 | 1.351 | 101.625 | 1.00x |
| users.json $..total | jsonpath-ng | 166.745 | 169.854 | 183.828 | 101.625 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 2.973 | 3.000 | 3.137 | 101.438 | 1.00x |
| users.json $[*].id | orjson+jmespath | 8.026 | 8.296 | 9.869 | 101.438 | 0.36x |
| users.json $[*].id | orjson+jsonpath-ng | 9.149 | 9.294 | 11.091 | 101.438 | 0.32x |
| users.json $[*].orders[*].total | strata | 3.027 | 3.126 | 3.451 | 101.578 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 9.101 | 9.485 | 9.881 | 101.578 | 0.33x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 18.344 | 18.612 | 21.242 | 101.578 | 0.17x |
| users.json $..total | strata | 6.499 | 6.654 | 6.929 | 101.688 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 174.274 | 178.137 | 179.101 | 101.688 | 0.04x |

