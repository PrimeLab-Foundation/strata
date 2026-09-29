# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 565fab210bb851772fcefe65082041084aeafc14
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
| users.json | strata | 5.107 | 5.157 | 5.761 | 70.219 | 1.00x |
| users.json | orjson | 7.660 | 7.753 | 8.414 | 70.219 | 0.67x |
| users.json | msgspec | 7.604 | 7.637 | 11.032 | 70.219 | 0.68x |
| users.json | ujson | 9.946 | 10.256 | 15.027 | 70.219 | 0.50x |
| users.json | pysimdjson | 111.161 | 112.032 | 118.017 | 70.219 | 0.05x |
| users.json | json | 12.512 | 12.698 | 13.178 | 70.219 | 0.41x |
| flat.json | strata | 0.482 | 0.556 | 0.623 | 123.891 | 1.00x |
| flat.json | orjson | 0.606 | 0.703 | 0.826 | 123.891 | 0.79x |
| flat.json | msgspec | 0.607 | 0.652 | 0.730 | 123.891 | 0.85x |
| flat.json | ujson | 0.957 | 1.164 | 1.417 | 123.891 | 0.48x |
| flat.json | pysimdjson | 10.646 | 11.615 | 12.118 | 123.891 | 0.05x |
| flat.json | json | 1.137 | 1.300 | 1.352 | 123.891 | 0.43x |
| nested.json | strata | 0.410 | 0.413 | 0.416 | 123.953 | 1.00x |
| nested.json | orjson | 0.563 | 0.567 | 0.581 | 123.953 | 0.73x |
| nested.json | msgspec | 0.575 | 0.581 | 0.593 | 123.953 | 0.71x |
| nested.json | ujson | 0.819 | 0.822 | 0.841 | 123.953 | 0.50x |
| nested.json | pysimdjson | 9.335 | 9.407 | 9.488 | 123.953 | 0.04x |
| nested.json | json | 1.180 | 1.186 | 1.314 | 123.953 | 0.35x |
| wide_arrays.json | strata | 2.468 | 2.490 | 2.637 | 126.547 | 1.00x |
| wide_arrays.json | orjson | 2.856 | 2.953 | 3.101 | 126.547 | 0.84x |
| wide_arrays.json | msgspec | 3.316 | 3.326 | 3.449 | 126.547 | 0.75x |
| wide_arrays.json | ujson | 4.355 | 4.414 | 4.530 | 126.547 | 0.56x |
| wide_arrays.json | pysimdjson | 58.146 | 58.445 | 60.244 | 126.547 | 0.04x |
| wide_arrays.json | json | 5.477 | 5.506 | 5.873 | 126.547 | 0.45x |
| mixed.json | strata | 0.099 | 0.100 | 0.101 | 126.562 | 1.00x |
| mixed.json | orjson | 0.128 | 0.134 | 0.147 | 126.562 | 0.74x |
| mixed.json | msgspec | 0.142 | 0.143 | 0.156 | 126.562 | 0.70x |
| mixed.json | ujson | 0.176 | 0.244 | 0.315 | 126.562 | 0.41x |
| mixed.json | pysimdjson | 2.237 | 2.265 | 2.299 | 126.562 | 0.04x |
| mixed.json | json | 0.267 | 0.269 | 0.281 | 126.562 | 0.37x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.200 | 1.229 | 1.538 | 93.719 | 1.00x |
| users.json | orjson | 1.911 | 1.945 | 2.102 | 93.719 | 0.63x |
| users.json | msgspec | 2.446 | 2.473 | 2.663 | 93.719 | 0.50x |
| users.json | ujson | 7.546 | 7.618 | 9.156 | 93.719 | 0.16x |
| users.json | json | 13.365 | 13.582 | 14.650 | 93.719 | 0.09x |
| flat.json | strata | 0.166 | 0.168 | 0.175 | 123.922 | 1.00x |
| flat.json | orjson | 0.207 | 0.210 | 0.230 | 123.922 | 0.80x |
| flat.json | msgspec | 0.258 | 0.260 | 0.278 | 123.922 | 0.65x |
| flat.json | ujson | 0.619 | 0.621 | 0.629 | 123.922 | 0.27x |
| flat.json | json | 1.147 | 1.169 | 1.328 | 123.922 | 0.14x |
| nested.json | strata | 0.101 | 0.102 | 0.122 | 123.969 | 1.00x |
| nested.json | orjson | 0.183 | 0.184 | 0.198 | 123.969 | 0.55x |
| nested.json | msgspec | 0.239 | 0.239 | 0.260 | 123.969 | 0.43x |
| nested.json | ujson | 0.695 | 0.712 | 0.847 | 123.969 | 0.14x |
| nested.json | json | 1.376 | 1.395 | 1.483 | 123.969 | 0.07x |
| wide_arrays.json | strata | 0.903 | 0.921 | 0.977 | 126.547 | 1.00x |
| wide_arrays.json | orjson | 1.241 | 1.345 | 1.452 | 126.547 | 0.68x |
| wide_arrays.json | msgspec | 1.765 | 1.794 | 2.093 | 126.547 | 0.51x |
| wide_arrays.json | ujson | 3.837 | 3.860 | 4.925 | 126.547 | 0.24x |
| wide_arrays.json | json | 10.120 | 10.251 | 10.704 | 126.547 | 0.09x |
| mixed.json | strata | 0.028 | 0.028 | 0.029 | 126.562 | 1.00x |
| mixed.json | orjson | 0.036 | 0.038 | 0.042 | 126.562 | 0.74x |
| mixed.json | msgspec | 0.041 | 0.076 | 0.118 | 126.562 | 0.37x |
| mixed.json | ujson | 0.143 | 0.144 | 0.145 | 126.562 | 0.20x |
| mixed.json | json | 0.290 | 0.291 | 0.294 | 126.562 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 5.384 | 5.480 | 6.149 | 103.969 | 1.00x |
| users.json | orjson | 8.134 | 8.410 | 8.857 | 103.969 | 0.65x |
| users.json | msgspec | 8.076 | 8.161 | 8.782 | 103.969 | 0.67x |
| users.json | ujson | 11.012 | 11.185 | 12.598 | 103.969 | 0.49x |
| users.json | json | 12.766 | 12.944 | 14.779 | 103.969 | 0.42x |
| flat.json | strata | 0.498 | 0.499 | 0.582 | 123.953 | 1.00x |
| flat.json | orjson | 0.640 | 0.652 | 0.705 | 123.953 | 0.77x |
| flat.json | msgspec | 0.634 | 0.636 | 0.685 | 123.953 | 0.78x |
| flat.json | ujson | 0.929 | 0.932 | 0.996 | 123.953 | 0.54x |
| flat.json | json | 1.172 | 1.177 | 1.210 | 123.953 | 0.42x |
| nested.json | strata | 0.427 | 0.433 | 0.531 | 123.969 | 1.00x |
| nested.json | orjson | 0.629 | 0.659 | 0.782 | 123.969 | 0.66x |
| nested.json | msgspec | 0.605 | 0.616 | 0.676 | 123.969 | 0.70x |
| nested.json | ujson | 0.833 | 0.853 | 0.964 | 123.969 | 0.51x |
| nested.json | json | 1.212 | 1.237 | 1.364 | 123.969 | 0.35x |
| wide_arrays.json | strata | 2.591 | 2.602 | 2.670 | 126.547 | 1.00x |
| wide_arrays.json | orjson | 3.020 | 3.043 | 3.211 | 126.547 | 0.86x |
| wide_arrays.json | msgspec | 3.492 | 3.533 | 3.630 | 126.547 | 0.74x |
| wide_arrays.json | ujson | 4.612 | 4.636 | 4.819 | 126.547 | 0.56x |
| wide_arrays.json | json | 5.660 | 5.719 | 5.864 | 126.547 | 0.45x |
| mixed.json | strata | 0.110 | 0.112 | 0.139 | 126.562 | 1.00x |
| mixed.json | orjson | 0.227 | 0.234 | 0.342 | 126.562 | 0.48x |
| mixed.json | msgspec | 0.164 | 0.173 | 0.206 | 126.562 | 0.65x |
| mixed.json | ujson | 0.204 | 0.206 | 0.256 | 126.562 | 0.54x |
| mixed.json | json | 0.288 | 0.298 | 0.363 | 126.562 | 0.38x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 5.415 | 6.179 | 6.864 | 123.266 | 1.00x |
| users.ndjson | orjson | 9.384 | 10.830 | 11.930 | 123.266 | 0.57x |
| users.ndjson | msgspec | 9.619 | 10.930 | 11.588 | 123.266 | 0.57x |
| users.ndjson | ujson | 11.915 | 13.853 | 14.714 | 123.266 | 0.45x |
| users.ndjson | json | 15.551 | 17.743 | 20.547 | 123.266 | 0.35x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.467 | 1.527 | 7.602 | 115.562 | 1.00x |
| users.json | orjson | 2.190 | 2.349 | 12.667 | 115.562 | 0.65x |
| users.json | msgspec | 2.788 | 2.885 | 13.530 | 115.562 | 0.53x |
| users.json | ujson | 7.940 | 8.320 | 14.883 | 115.562 | 0.18x |
| users.json | json | 13.898 | 14.361 | 25.984 | 115.562 | 0.11x |
| flat.json | strata | 0.230 | 0.247 | 0.360 | 123.953 | 1.00x |
| flat.json | orjson | 0.286 | 0.305 | 0.390 | 123.953 | 0.81x |
| flat.json | msgspec | 0.334 | 0.343 | 0.648 | 123.953 | 0.72x |
| flat.json | ujson | 0.701 | 0.735 | 0.782 | 123.953 | 0.34x |
| flat.json | json | 1.232 | 1.251 | 1.515 | 123.953 | 0.20x |
| nested.json | strata | 0.168 | 0.174 | 0.198 | 123.969 | 1.00x |
| nested.json | orjson | 0.252 | 0.269 | 0.780 | 123.969 | 0.65x |
| nested.json | msgspec | 0.311 | 0.411 | 0.528 | 123.969 | 0.42x |
| nested.json | ujson | 0.765 | 0.863 | 1.065 | 123.969 | 0.20x |
| nested.json | json | 1.469 | 1.514 | 1.602 | 123.969 | 0.12x |
| wide_arrays.json | strata | 1.087 | 1.119 | 1.289 | 126.547 | 1.00x |
| wide_arrays.json | orjson | 1.326 | 1.505 | 1.823 | 126.547 | 0.74x |
| wide_arrays.json | msgspec | 1.978 | 1.993 | 2.124 | 126.547 | 0.56x |
| wide_arrays.json | ujson | 4.043 | 4.120 | 4.539 | 126.547 | 0.27x |
| wide_arrays.json | json | 10.432 | 10.517 | 10.904 | 126.547 | 0.11x |
| mixed.json | strata | 0.076 | 0.085 | 0.113 | 126.562 | 1.00x |
| mixed.json | orjson | 0.090 | 0.101 | 0.129 | 126.562 | 0.84x |
| mixed.json | msgspec | 0.097 | 0.157 | 0.249 | 126.562 | 0.54x |
| mixed.json | ujson | 0.207 | 0.215 | 0.231 | 126.562 | 0.39x |
| mixed.json | json | 0.346 | 0.357 | 0.391 | 126.562 | 0.24x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.035 | 0.036 | 0.040 | 115.625 | 1.00x |
| users.json $[*].id | jmespath | 0.220 | 0.225 | 0.278 | 115.625 | 0.16x |
| users.json $[*].id | jsonpath-ng | 1.287 | 1.296 | 1.373 | 115.625 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.202 | 0.212 | 0.278 | 115.766 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.366 | 1.376 | 1.395 | 115.766 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 8.712 | 9.000 | 9.301 | 115.766 | 0.02x |
| users.json $..total | strata | 1.056 | 1.157 | 1.303 | 115.875 | 1.00x |
| users.json $..total | jsonpath-ng | 164.186 | 164.917 | 166.633 | 115.875 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 2.985 | 3.019 | 3.205 | 115.672 | 1.00x |
| users.json $[*].id | orjson+jmespath | 8.154 | 8.478 | 8.559 | 115.672 | 0.36x |
| users.json $[*].id | orjson+jsonpath-ng | 9.196 | 9.296 | 9.444 | 115.672 | 0.32x |
| users.json $[*].orders[*].total | strata | 3.035 | 3.548 | 3.758 | 115.812 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 9.333 | 11.297 | 11.800 | 115.812 | 0.31x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 19.173 | 22.352 | 22.749 | 115.812 | 0.16x |
| users.json $..total | strata | 6.469 | 6.607 | 7.951 | 116.000 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 172.858 | 173.698 | 204.487 | 116.000 | 0.04x |

