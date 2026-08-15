# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 6520b15
- python: 3.12.10
- implementation: CPython
- platform: macOS-26.5.2-arm64-arm-64bit
- machine: arm64
- processor: Apple M1 (Virtual)
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.386 | 7.600 | 9.265 | 63.500 | 1.00x |
| users.json | orjson | 9.410 | 9.843 | 12.516 | 63.500 | 0.77x |
| users.json | msgspec | 9.107 | 9.586 | 12.031 | 63.500 | 0.79x |
| users.json | ujson | 12.232 | 13.022 | 16.122 | 63.500 | 0.58x |
| users.json | pysimdjson | 123.291 | 128.166 | 147.369 | 63.500 | 0.06x |
| users.json | json | 14.989 | 15.867 | 17.202 | 63.500 | 0.48x |
| flat.json | strata | 0.664 | 0.701 | 0.753 | 87.016 | 1.00x |
| flat.json | orjson | 0.743 | 0.774 | 0.828 | 87.016 | 0.91x |
| flat.json | msgspec | 0.701 | 0.724 | 0.746 | 87.016 | 0.97x |
| flat.json | ujson | 1.141 | 1.190 | 1.290 | 87.016 | 0.59x |
| flat.json | pysimdjson | 11.614 | 11.691 | 12.132 | 87.016 | 0.06x |
| flat.json | json | 1.355 | 1.395 | 1.573 | 87.016 | 0.50x |
| nested.json | strata | 0.573 | 0.576 | 0.604 | 87.047 | 1.00x |
| nested.json | orjson | 0.701 | 0.714 | 0.752 | 87.047 | 0.81x |
| nested.json | msgspec | 0.658 | 0.664 | 0.696 | 87.047 | 0.87x |
| nested.json | ujson | 1.024 | 1.084 | 1.169 | 87.047 | 0.53x |
| nested.json | pysimdjson | 10.111 | 10.242 | 13.278 | 87.047 | 0.06x |
| nested.json | json | 1.388 | 1.418 | 1.442 | 87.047 | 0.41x |
| wide_arrays.json | strata | 3.359 | 3.539 | 3.843 | 89.547 | 1.00x |
| wide_arrays.json | orjson | 3.490 | 3.590 | 3.780 | 89.547 | 0.99x |
| wide_arrays.json | msgspec | 3.985 | 4.110 | 4.179 | 89.547 | 0.86x |
| wide_arrays.json | ujson | 5.346 | 5.454 | 5.794 | 89.547 | 0.65x |
| wide_arrays.json | pysimdjson | 62.509 | 62.920 | 63.466 | 89.547 | 0.06x |
| wide_arrays.json | json | 6.675 | 6.927 | 7.007 | 89.547 | 0.51x |
| mixed.json | strata | 0.135 | 0.140 | 0.173 | 89.578 | 1.00x |
| mixed.json | orjson | 0.151 | 0.159 | 0.300 | 89.578 | 0.88x |
| mixed.json | msgspec | 0.164 | 0.167 | 0.177 | 89.578 | 0.84x |
| mixed.json | ujson | 0.212 | 0.348 | 0.394 | 89.578 | 0.40x |
| mixed.json | pysimdjson | 2.432 | 2.466 | 2.530 | 89.578 | 0.06x |
| mixed.json | json | 0.316 | 0.332 | 0.361 | 89.578 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.977 | 2.076 | 2.309 | 71.656 | 1.00x |
| users.json | orjson | 2.286 | 2.373 | 2.803 | 71.656 | 0.87x |
| users.json | msgspec | 2.856 | 3.012 | 3.237 | 71.656 | 0.69x |
| users.json | ujson | 10.883 | 11.078 | 12.446 | 71.656 | 0.19x |
| users.json | json | 15.777 | 16.265 | 17.098 | 71.656 | 0.13x |
| flat.json | strata | 0.192 | 0.208 | 0.223 | 87.047 | 1.00x |
| flat.json | orjson | 0.256 | 0.358 | 0.404 | 87.047 | 0.58x |
| flat.json | msgspec | 0.313 | 0.321 | 0.451 | 87.047 | 0.65x |
| flat.json | ujson | 0.935 | 0.997 | 1.061 | 87.047 | 0.21x |
| flat.json | json | 1.324 | 1.370 | 1.505 | 87.047 | 0.15x |
| nested.json | strata | 0.152 | 0.155 | 0.170 | 87.047 | 1.00x |
| nested.json | orjson | 0.216 | 0.223 | 0.246 | 87.047 | 0.70x |
| nested.json | msgspec | 0.277 | 0.280 | 0.291 | 87.047 | 0.55x |
| nested.json | ujson | 1.092 | 1.122 | 1.223 | 87.047 | 0.14x |
| nested.json | json | 1.591 | 1.628 | 1.662 | 87.047 | 0.10x |
| wide_arrays.json | strata | 1.120 | 1.194 | 1.266 | 89.562 | 1.00x |
| wide_arrays.json | orjson | 1.267 | 1.428 | 1.563 | 89.562 | 0.84x |
| wide_arrays.json | msgspec | 2.044 | 2.124 | 2.239 | 89.562 | 0.56x |
| wide_arrays.json | ujson | 6.410 | 6.618 | 7.114 | 89.562 | 0.18x |
| wide_arrays.json | json | 11.477 | 11.647 | 11.996 | 89.562 | 0.10x |
| mixed.json | strata | 0.040 | 0.044 | 0.053 | 89.578 | 1.00x |
| mixed.json | orjson | 0.042 | 0.104 | 0.252 | 89.578 | 0.42x |
| mixed.json | msgspec | 0.049 | 0.055 | 0.062 | 89.578 | 0.80x |
| mixed.json | ujson | 0.217 | 0.223 | 0.249 | 89.578 | 0.20x |
| mixed.json | json | 0.345 | 0.355 | 0.380 | 89.578 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.873 | 8.018 | 8.592 | 83.797 | 1.00x |
| users.json | orjson | 9.898 | 10.143 | 13.492 | 83.797 | 0.79x |
| users.json | msgspec | 9.612 | 9.891 | 12.012 | 83.797 | 0.81x |
| users.json | ujson | 13.194 | 13.530 | 14.159 | 83.797 | 0.59x |
| users.json | json | 15.562 | 15.863 | 16.453 | 83.797 | 0.51x |
| flat.json | strata | 0.702 | 0.727 | 0.786 | 87.047 | 1.00x |
| flat.json | orjson | 0.898 | 0.939 | 1.053 | 87.047 | 0.77x |
| flat.json | msgspec | 0.770 | 0.841 | 0.905 | 87.047 | 0.86x |
| flat.json | ujson | 1.120 | 1.166 | 1.349 | 87.047 | 0.62x |
| flat.json | json | 1.402 | 1.433 | 1.554 | 87.047 | 0.51x |
| nested.json | strata | 0.618 | 0.631 | 0.723 | 87.047 | 1.00x |
| nested.json | orjson | 0.855 | 0.878 | 1.026 | 87.047 | 0.72x |
| nested.json | msgspec | 0.720 | 0.739 | 0.823 | 87.047 | 0.85x |
| nested.json | ujson | 1.006 | 1.017 | 1.171 | 87.047 | 0.62x |
| nested.json | json | 1.416 | 1.479 | 1.610 | 87.047 | 0.43x |
| wide_arrays.json | strata | 3.573 | 3.653 | 3.984 | 89.562 | 1.00x |
| wide_arrays.json | orjson | 3.706 | 3.838 | 4.111 | 89.562 | 0.95x |
| wide_arrays.json | msgspec | 4.307 | 4.347 | 4.541 | 89.562 | 0.84x |
| wide_arrays.json | ujson | 5.696 | 5.808 | 6.118 | 89.562 | 0.63x |
| wide_arrays.json | json | 7.033 | 7.173 | 7.525 | 89.562 | 0.51x |
| mixed.json | strata | 0.161 | 0.175 | 0.217 | 89.578 | 1.00x |
| mixed.json | orjson | 0.271 | 0.332 | 0.441 | 89.578 | 0.53x |
| mixed.json | msgspec | 0.203 | 0.224 | 0.259 | 89.578 | 0.78x |
| mixed.json | ujson | 0.250 | 0.265 | 0.353 | 89.578 | 0.66x |
| mixed.json | json | 0.344 | 0.360 | 0.395 | 89.578 | 0.49x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.742 | 7.789 | 8.100 | 87.016 | 1.00x |
| users.ndjson | orjson | 11.412 | 11.654 | 11.858 | 87.016 | 0.67x |
| users.ndjson | msgspec | 11.278 | 11.540 | 11.683 | 87.016 | 0.68x |
| users.ndjson | ujson | 14.014 | 14.228 | 14.592 | 87.016 | 0.55x |
| users.ndjson | json | 18.120 | 18.335 | 18.738 | 87.016 | 0.42x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.488 | 2.600 | 3.404 | 81.312 | 1.00x |
| users.json | orjson | 2.785 | 2.926 | 3.323 | 81.312 | 0.89x |
| users.json | msgspec | 3.409 | 3.613 | 4.026 | 81.312 | 0.72x |
| users.json | ujson | 11.585 | 11.886 | 12.493 | 81.312 | 0.22x |
| users.json | json | 16.369 | 16.573 | 17.655 | 81.312 | 0.16x |
| flat.json | strata | 0.385 | 0.458 | 1.178 | 87.047 | 1.00x |
| flat.json | orjson | 0.393 | 0.463 | 0.702 | 87.047 | 0.99x |
| flat.json | msgspec | 0.447 | 0.485 | 1.105 | 87.047 | 0.94x |
| flat.json | ujson | 1.075 | 1.230 | 1.404 | 87.047 | 0.37x |
| flat.json | json | 1.680 | 1.767 | 2.649 | 87.047 | 0.26x |
| nested.json | strata | 0.277 | 0.330 | 0.501 | 87.047 | 1.00x |
| nested.json | orjson | 0.339 | 0.433 | 1.099 | 87.047 | 0.76x |
| nested.json | msgspec | 0.396 | 0.552 | 0.944 | 87.047 | 0.60x |
| nested.json | ujson | 1.148 | 1.289 | 1.515 | 87.047 | 0.26x |
| nested.json | json | 1.739 | 1.875 | 2.373 | 87.047 | 0.18x |
| wide_arrays.json | strata | 1.480 | 1.616 | 2.246 | 89.562 | 1.00x |
| wide_arrays.json | orjson | 1.728 | 1.810 | 2.054 | 89.562 | 0.89x |
| wide_arrays.json | msgspec | 2.390 | 2.546 | 3.004 | 89.562 | 0.63x |
| wide_arrays.json | ujson | 6.946 | 7.046 | 14.358 | 89.562 | 0.23x |
| wide_arrays.json | json | 11.917 | 12.110 | 12.355 | 89.562 | 0.13x |
| mixed.json | strata | 0.139 | 0.171 | 0.296 | 89.578 | 1.00x |
| mixed.json | orjson | 0.137 | 0.164 | 0.253 | 89.578 | 1.05x |
| mixed.json | msgspec | 0.144 | 0.234 | 0.314 | 89.578 | 0.73x |
| mixed.json | ujson | 0.322 | 0.371 | 0.429 | 89.578 | 0.46x |
| mixed.json | json | 0.450 | 0.504 | 0.661 | 89.578 | 0.34x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.052 | 0.060 | 0.071 | 81.375 | 1.00x |
| users.json $[*].id | jmespath | 0.281 | 0.307 | 0.324 | 81.375 | 0.19x |
| users.json $[*].id | jsonpath-ng | 1.523 | 1.529 | 1.642 | 81.375 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.326 | 0.415 | 0.783 | 81.562 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.723 | 1.887 | 2.081 | 81.562 | 0.22x |
| users.json $[*].orders[*].total | jsonpath-ng | 10.747 | 10.934 | 11.321 | 81.562 | 0.04x |
| users.json $..total | strata | 1.246 | 1.422 | 1.585 | 82.609 | 1.00x |
| users.json $..total | jsonpath-ng | 188.284 | 198.654 | 218.548 | 82.609 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.603 | 3.680 | 3.889 | 81.438 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.445 | 10.583 | 10.732 | 81.438 | 0.35x |
| users.json $[*].id | orjson+jsonpath-ng | 11.310 | 11.468 | 11.962 | 81.438 | 0.32x |
| users.json $[*].orders[*].total | strata | 3.593 | 3.746 | 3.855 | 82.594 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 11.339 | 11.851 | 12.334 | 82.594 | 0.32x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 22.205 | 24.518 | 25.361 | 82.594 | 0.15x |
| users.json $..total | strata | 8.962 | 9.314 | 11.184 | 82.609 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 198.257 | 205.715 | 236.867 | 82.609 | 0.05x |

