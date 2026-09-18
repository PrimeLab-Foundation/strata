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
| users.json | strata | 5.746 | 5.795 | 6.758 | 68.172 | 1.00x |
| users.json | orjson | 8.615 | 8.712 | 9.625 | 68.172 | 0.67x |
| users.json | msgspec | 8.259 | 8.311 | 9.244 | 68.172 | 0.70x |
| users.json | ujson | 11.004 | 11.057 | 12.771 | 68.172 | 0.52x |
| users.json | pysimdjson | 115.324 | 115.703 | 126.307 | 68.172 | 0.05x |
| users.json | json | 13.974 | 14.110 | 15.119 | 68.172 | 0.41x |
| flat.json | strata | 0.543 | 0.548 | 0.609 | 98.703 | 1.00x |
| flat.json | orjson | 0.663 | 0.687 | 0.724 | 98.703 | 0.80x |
| flat.json | msgspec | 0.650 | 0.652 | 0.665 | 98.703 | 0.84x |
| flat.json | ujson | 0.999 | 1.021 | 1.163 | 98.703 | 0.54x |
| flat.json | pysimdjson | 11.069 | 11.097 | 11.283 | 98.703 | 0.05x |
| flat.json | json | 1.267 | 1.274 | 1.413 | 98.703 | 0.43x |
| nested.json | strata | 0.481 | 0.540 | 0.598 | 98.703 | 1.00x |
| nested.json | orjson | 0.673 | 0.728 | 0.835 | 98.703 | 0.74x |
| nested.json | msgspec | 0.638 | 0.647 | 0.728 | 98.703 | 0.83x |
| nested.json | ujson | 1.064 | 1.128 | 1.281 | 98.703 | 0.48x |
| nested.json | pysimdjson | 9.737 | 10.225 | 11.214 | 98.703 | 0.05x |
| nested.json | json | 1.404 | 1.453 | 1.694 | 98.703 | 0.37x |
| wide_arrays.json | strata | 2.772 | 2.806 | 3.040 | 100.328 | 1.00x |
| wide_arrays.json | orjson | 3.377 | 3.396 | 3.781 | 100.328 | 0.83x |
| wide_arrays.json | msgspec | 3.769 | 3.790 | 4.169 | 100.328 | 0.74x |
| wide_arrays.json | ujson | 4.785 | 4.982 | 5.135 | 100.328 | 0.56x |
| wide_arrays.json | pysimdjson | 59.710 | 59.870 | 60.903 | 100.328 | 0.05x |
| wide_arrays.json | json | 6.339 | 6.403 | 6.919 | 100.328 | 0.44x |
| mixed.json | strata | 0.114 | 0.116 | 0.119 | 100.344 | 1.00x |
| mixed.json | orjson | 0.144 | 0.146 | 0.156 | 100.344 | 0.80x |
| mixed.json | msgspec | 0.157 | 0.158 | 0.184 | 100.344 | 0.73x |
| mixed.json | ujson | 0.235 | 0.240 | 0.335 | 100.344 | 0.48x |
| mixed.json | pysimdjson | 2.355 | 2.366 | 2.585 | 100.344 | 0.05x |
| mixed.json | json | 0.303 | 0.309 | 0.335 | 100.344 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.383 | 1.400 | 1.570 | 79.219 | 1.00x |
| users.json | orjson | 2.090 | 2.171 | 2.319 | 79.219 | 0.64x |
| users.json | msgspec | 2.707 | 2.816 | 2.880 | 79.219 | 0.50x |
| users.json | ujson | 8.283 | 8.326 | 8.428 | 79.219 | 0.17x |
| users.json | json | 14.985 | 15.071 | 15.553 | 79.219 | 0.09x |
| flat.json | strata | 0.189 | 0.201 | 0.247 | 98.703 | 1.00x |
| flat.json | orjson | 0.234 | 0.239 | 0.317 | 98.703 | 0.84x |
| flat.json | msgspec | 0.289 | 0.294 | 0.406 | 98.703 | 0.68x |
| flat.json | ujson | 0.708 | 0.718 | 0.799 | 98.703 | 0.28x |
| flat.json | json | 1.297 | 1.392 | 1.622 | 98.703 | 0.14x |
| nested.json | strata | 0.122 | 0.126 | 0.149 | 98.703 | 1.00x |
| nested.json | orjson | 0.222 | 0.261 | 0.289 | 98.703 | 0.48x |
| nested.json | msgspec | 0.285 | 0.305 | 0.351 | 98.703 | 0.41x |
| nested.json | ujson | 0.912 | 0.967 | 1.096 | 98.703 | 0.13x |
| nested.json | json | 1.663 | 1.749 | 1.840 | 98.703 | 0.07x |
| wide_arrays.json | strata | 1.007 | 1.014 | 1.059 | 100.328 | 1.00x |
| wide_arrays.json | orjson | 1.206 | 1.351 | 1.466 | 100.328 | 0.75x |
| wide_arrays.json | msgspec | 2.095 | 2.124 | 2.183 | 100.328 | 0.48x |
| wide_arrays.json | ujson | 4.484 | 4.514 | 4.553 | 100.328 | 0.22x |
| wide_arrays.json | json | 11.150 | 11.194 | 11.530 | 100.328 | 0.09x |
| mixed.json | strata | 0.032 | 0.032 | 0.033 | 100.344 | 1.00x |
| mixed.json | orjson | 0.040 | 0.040 | 0.041 | 100.344 | 0.80x |
| mixed.json | msgspec | 0.046 | 0.047 | 0.047 | 100.344 | 0.69x |
| mixed.json | ujson | 0.158 | 0.160 | 0.165 | 100.344 | 0.20x |
| mixed.json | json | 0.324 | 0.325 | 0.337 | 100.344 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.114 | 6.160 | 6.581 | 91.891 | 1.00x |
| users.json | orjson | 8.905 | 9.051 | 9.205 | 91.891 | 0.68x |
| users.json | msgspec | 8.651 | 8.700 | 8.913 | 91.891 | 0.71x |
| users.json | ujson | 11.647 | 11.740 | 12.241 | 91.891 | 0.52x |
| users.json | json | 14.328 | 14.420 | 14.610 | 91.891 | 0.43x |
| flat.json | strata | 0.578 | 0.588 | 0.727 | 98.703 | 1.00x |
| flat.json | orjson | 0.757 | 0.779 | 0.876 | 98.703 | 0.75x |
| flat.json | msgspec | 0.701 | 0.712 | 0.739 | 98.703 | 0.83x |
| flat.json | ujson | 1.021 | 1.032 | 1.272 | 98.703 | 0.57x |
| flat.json | json | 1.314 | 1.328 | 1.533 | 98.703 | 0.44x |
| nested.json | strata | 0.555 | 0.587 | 0.666 | 98.703 | 1.00x |
| nested.json | orjson | 0.897 | 0.977 | 1.042 | 98.703 | 0.60x |
| nested.json | msgspec | 0.781 | 0.826 | 0.919 | 98.703 | 0.71x |
| nested.json | ujson | 1.040 | 1.131 | 1.212 | 98.703 | 0.52x |
| nested.json | json | 1.570 | 1.611 | 1.707 | 98.703 | 0.36x |
| wide_arrays.json | strata | 2.964 | 2.990 | 3.173 | 100.328 | 1.00x |
| wide_arrays.json | orjson | 3.592 | 3.613 | 3.703 | 100.328 | 0.83x |
| wide_arrays.json | msgspec | 4.082 | 4.126 | 4.160 | 100.328 | 0.72x |
| wide_arrays.json | ujson | 5.291 | 5.403 | 5.444 | 100.328 | 0.55x |
| wide_arrays.json | json | 6.666 | 6.697 | 7.081 | 100.328 | 0.45x |
| mixed.json | strata | 0.134 | 0.136 | 0.179 | 100.344 | 1.00x |
| mixed.json | orjson | 0.184 | 0.237 | 0.327 | 100.344 | 0.57x |
| mixed.json | msgspec | 0.191 | 0.195 | 0.209 | 100.344 | 0.69x |
| mixed.json | ujson | 0.236 | 0.247 | 0.336 | 100.344 | 0.55x |
| mixed.json | json | 0.338 | 0.341 | 0.418 | 100.344 | 0.40x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.428 | 6.943 | 7.668 | 98.703 | 1.00x |
| users.ndjson | orjson | 10.921 | 11.764 | 13.971 | 98.703 | 0.59x |
| users.ndjson | msgspec | 10.867 | 12.064 | 12.960 | 98.703 | 0.58x |
| users.ndjson | ujson | 13.517 | 15.391 | 16.209 | 98.703 | 0.45x |
| users.ndjson | json | 18.383 | 19.145 | 21.372 | 98.703 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.656 | 1.767 | 1.825 | 93.109 | 1.00x |
| users.json | orjson | 2.487 | 2.613 | 2.750 | 93.109 | 0.68x |
| users.json | msgspec | 3.059 | 3.199 | 3.275 | 93.109 | 0.55x |
| users.json | ujson | 8.895 | 8.956 | 9.138 | 93.109 | 0.20x |
| users.json | json | 15.393 | 15.473 | 16.296 | 93.109 | 0.11x |
| flat.json | strata | 0.303 | 0.338 | 0.600 | 98.703 | 1.00x |
| flat.json | orjson | 0.345 | 0.375 | 0.472 | 98.703 | 0.90x |
| flat.json | msgspec | 0.403 | 0.425 | 0.542 | 98.703 | 0.79x |
| flat.json | ujson | 0.847 | 0.859 | 0.890 | 98.703 | 0.39x |
| flat.json | json | 1.476 | 1.539 | 1.648 | 98.703 | 0.22x |
| nested.json | strata | 0.280 | 0.346 | 0.419 | 98.703 | 1.00x |
| nested.json | orjson | 0.418 | 0.466 | 0.905 | 98.703 | 0.74x |
| nested.json | msgspec | 0.457 | 0.713 | 0.804 | 98.703 | 0.49x |
| nested.json | ujson | 1.165 | 1.228 | 1.419 | 98.703 | 0.28x |
| nested.json | json | 1.844 | 1.937 | 2.054 | 98.703 | 0.18x |
| wide_arrays.json | strata | 1.287 | 1.326 | 1.550 | 100.328 | 1.00x |
| wide_arrays.json | orjson | 1.608 | 1.674 | 2.062 | 100.328 | 0.79x |
| wide_arrays.json | msgspec | 2.430 | 2.515 | 2.776 | 100.328 | 0.53x |
| wide_arrays.json | ujson | 4.873 | 5.004 | 5.731 | 100.328 | 0.26x |
| wide_arrays.json | json | 11.519 | 11.695 | 12.250 | 100.328 | 0.11x |
| mixed.json | strata | 0.112 | 0.119 | 0.151 | 100.344 | 1.00x |
| mixed.json | orjson | 0.126 | 0.142 | 0.401 | 100.344 | 0.84x |
| mixed.json | msgspec | 0.137 | 0.141 | 0.158 | 100.344 | 0.84x |
| mixed.json | ujson | 0.253 | 0.265 | 0.290 | 100.344 | 0.45x |
| mixed.json | json | 0.426 | 0.441 | 0.451 | 100.344 | 0.27x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.045 | 0.046 | 0.047 | 93.156 | 1.00x |
| users.json $[*].id | jmespath | 0.253 | 0.255 | 0.263 | 93.156 | 0.18x |
| users.json $[*].id | jsonpath-ng | 1.396 | 1.418 | 1.443 | 93.156 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.273 | 0.274 | 0.374 | 93.266 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.531 | 1.560 | 1.836 | 93.266 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 9.745 | 9.905 | 9.937 | 93.266 | 0.03x |
| users.json $..total | strata | 1.205 | 1.223 | 1.457 | 93.328 | 1.00x |
| users.json $..total | jsonpath-ng | 180.094 | 182.067 | 198.061 | 93.328 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.352 | 3.396 | 3.764 | 93.188 | 1.00x |
| users.json $[*].id | orjson+jmespath | 9.194 | 9.402 | 10.409 | 93.188 | 0.36x |
| users.json $[*].id | orjson+jsonpath-ng | 10.408 | 10.719 | 11.916 | 93.188 | 0.32x |
| users.json $[*].orders[*].total | strata | 3.417 | 3.491 | 3.705 | 93.328 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 10.425 | 10.532 | 10.808 | 93.328 | 0.33x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 20.438 | 20.611 | 21.111 | 93.328 | 0.17x |
| users.json $..total | strata | 7.472 | 7.592 | 8.820 | 93.328 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 187.650 | 189.533 | 209.296 | 93.328 | 0.04x |

