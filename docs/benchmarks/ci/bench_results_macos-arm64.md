# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: af229e1
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
| users.json | strata | 7.998 | 8.788 | 12.915 | 63.734 | 1.00x |
| users.json | orjson | 10.797 | 12.312 | 20.259 | 63.734 | 0.71x |
| users.json | msgspec | 11.017 | 12.355 | 17.893 | 63.734 | 0.71x |
| users.json | ujson | 14.029 | 17.733 | 51.063 | 63.734 | 0.50x |
| users.json | pysimdjson | 141.823 | 157.246 | 244.883 | 63.734 | 0.06x |
| users.json | json | 17.892 | 22.016 | 34.600 | 63.734 | 0.40x |
| flat.json | strata | 0.644 | 0.714 | 0.834 | 84.344 | 1.00x |
| flat.json | orjson | 0.826 | 0.864 | 1.148 | 84.344 | 0.83x |
| flat.json | msgspec | 0.746 | 0.833 | 1.042 | 84.344 | 0.86x |
| flat.json | ujson | 1.189 | 1.409 | 1.524 | 84.344 | 0.51x |
| flat.json | pysimdjson | 12.667 | 12.942 | 13.260 | 84.344 | 0.06x |
| flat.json | json | 1.445 | 1.551 | 1.671 | 84.344 | 0.46x |
| nested.json | strata | 0.546 | 0.589 | 0.663 | 84.344 | 1.00x |
| nested.json | orjson | 0.748 | 0.869 | 1.334 | 84.344 | 0.68x |
| nested.json | msgspec | 0.705 | 0.756 | 0.868 | 84.344 | 0.78x |
| nested.json | ujson | 1.156 | 1.222 | 1.311 | 84.344 | 0.48x |
| nested.json | pysimdjson | 11.084 | 11.194 | 11.561 | 84.344 | 0.05x |
| nested.json | json | 1.443 | 1.546 | 1.644 | 84.344 | 0.38x |
| wide_arrays.json | strata | 3.913 | 4.828 | 7.798 | 87.234 | 1.00x |
| wide_arrays.json | orjson | 4.361 | 5.249 | 7.511 | 87.234 | 0.92x |
| wide_arrays.json | msgspec | 4.760 | 5.780 | 8.857 | 87.234 | 0.84x |
| wide_arrays.json | ujson | 6.110 | 8.215 | 13.875 | 87.234 | 0.59x |
| wide_arrays.json | pysimdjson | 69.722 | 99.383 | 153.264 | 87.234 | 0.05x |
| wide_arrays.json | json | 7.579 | 10.703 | 18.409 | 87.234 | 0.45x |
| mixed.json | strata | 0.136 | 0.148 | 0.167 | 87.344 | 1.00x |
| mixed.json | orjson | 0.163 | 0.188 | 0.249 | 87.344 | 0.79x |
| mixed.json | msgspec | 0.183 | 0.196 | 0.225 | 87.344 | 0.76x |
| mixed.json | ujson | 0.219 | 0.372 | 0.460 | 87.344 | 0.40x |
| mixed.json | pysimdjson | 2.615 | 2.723 | 2.870 | 87.344 | 0.05x |
| mixed.json | json | 0.352 | 0.367 | 0.467 | 87.344 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.841 | 1.986 | 2.454 | 67.141 | 1.00x |
| users.json | orjson | 2.510 | 2.783 | 3.941 | 67.141 | 0.71x |
| users.json | msgspec | 3.013 | 3.316 | 4.416 | 67.141 | 0.60x |
| users.json | ujson | 11.961 | 13.033 | 16.516 | 67.141 | 0.15x |
| users.json | json | 17.141 | 19.142 | 24.093 | 67.141 | 0.10x |
| flat.json | strata | 0.243 | 0.271 | 0.504 | 84.344 | 1.00x |
| flat.json | orjson | 0.266 | 0.289 | 0.463 | 84.344 | 0.94x |
| flat.json | msgspec | 0.320 | 0.354 | 0.600 | 84.344 | 0.77x |
| flat.json | ujson | 1.004 | 1.077 | 1.116 | 84.344 | 0.25x |
| flat.json | json | 1.463 | 1.629 | 1.791 | 84.344 | 0.17x |
| nested.json | strata | 0.152 | 0.167 | 0.217 | 84.344 | 1.00x |
| nested.json | orjson | 0.238 | 0.253 | 0.336 | 84.344 | 0.66x |
| nested.json | msgspec | 0.309 | 0.404 | 0.515 | 84.344 | 0.41x |
| nested.json | ujson | 1.171 | 1.235 | 1.309 | 84.344 | 0.14x |
| nested.json | json | 1.748 | 1.834 | 1.909 | 84.344 | 0.09x |
| wide_arrays.json | strata | 1.161 | 1.229 | 1.681 | 87.312 | 1.00x |
| wide_arrays.json | orjson | 1.544 | 1.595 | 2.027 | 87.312 | 0.77x |
| wide_arrays.json | msgspec | 2.363 | 2.491 | 2.563 | 87.312 | 0.49x |
| wide_arrays.json | ujson | 7.164 | 7.336 | 7.872 | 87.312 | 0.17x |
| wide_arrays.json | json | 12.889 | 12.963 | 13.383 | 87.312 | 0.09x |
| mixed.json | strata | 0.046 | 0.054 | 0.066 | 87.344 | 1.00x |
| mixed.json | orjson | 0.057 | 0.068 | 0.084 | 87.344 | 0.78x |
| mixed.json | msgspec | 0.069 | 0.164 | 0.511 | 87.344 | 0.33x |
| mixed.json | ujson | 0.244 | 0.267 | 0.335 | 87.344 | 0.20x |
| mixed.json | json | 0.375 | 0.433 | 0.507 | 87.344 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.936 | 8.790 | 11.990 | 80.344 | 1.00x |
| users.json | orjson | 11.063 | 12.640 | 15.321 | 80.344 | 0.70x |
| users.json | msgspec | 10.265 | 12.138 | 17.152 | 80.344 | 0.72x |
| users.json | ujson | 15.245 | 16.662 | 21.736 | 80.344 | 0.53x |
| users.json | json | 18.196 | 20.932 | 25.764 | 80.344 | 0.42x |
| flat.json | strata | 0.742 | 0.783 | 0.809 | 84.344 | 1.00x |
| flat.json | orjson | 1.051 | 1.123 | 1.185 | 84.344 | 0.70x |
| flat.json | msgspec | 0.878 | 0.921 | 1.008 | 84.344 | 0.85x |
| flat.json | ujson | 1.282 | 1.365 | 1.552 | 84.344 | 0.57x |
| flat.json | json | 1.549 | 1.630 | 1.677 | 84.344 | 0.48x |
| nested.json | strata | 0.663 | 0.712 | 0.894 | 84.344 | 1.00x |
| nested.json | orjson | 0.964 | 1.061 | 1.123 | 84.344 | 0.67x |
| nested.json | msgspec | 0.800 | 0.878 | 0.989 | 84.344 | 0.81x |
| nested.json | ujson | 1.159 | 1.260 | 1.357 | 84.344 | 0.56x |
| nested.json | json | 1.610 | 1.713 | 1.804 | 84.344 | 0.42x |
| wide_arrays.json | strata | 3.971 | 4.100 | 6.959 | 87.312 | 1.00x |
| wide_arrays.json | orjson | 4.132 | 4.488 | 6.721 | 87.312 | 0.91x |
| wide_arrays.json | msgspec | 4.810 | 4.983 | 7.065 | 87.312 | 0.82x |
| wide_arrays.json | ujson | 6.141 | 6.714 | 12.487 | 87.312 | 0.61x |
| wide_arrays.json | json | 7.889 | 8.048 | 15.798 | 87.312 | 0.51x |
| mixed.json | strata | 0.208 | 0.235 | 0.281 | 87.344 | 1.00x |
| mixed.json | orjson | 0.419 | 0.469 | 0.545 | 87.344 | 0.50x |
| mixed.json | msgspec | 0.271 | 0.314 | 0.344 | 87.344 | 0.75x |
| mixed.json | ujson | 0.297 | 0.376 | 0.453 | 87.344 | 0.62x |
| mixed.json | json | 0.392 | 0.476 | 0.597 | 87.344 | 0.49x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 8.056 | 8.355 | 8.687 | 83.781 | 1.00x |
| users.ndjson | orjson | 12.921 | 13.098 | 13.749 | 83.781 | 0.64x |
| users.ndjson | msgspec | 12.833 | 13.055 | 13.777 | 83.781 | 0.64x |
| users.ndjson | ujson | 15.413 | 16.167 | 16.530 | 83.781 | 0.52x |
| users.ndjson | json | 20.098 | 20.737 | 24.619 | 83.781 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.352 | 2.820 | 3.481 | 81.766 | 1.00x |
| users.json | orjson | 3.030 | 3.279 | 5.549 | 81.766 | 0.86x |
| users.json | msgspec | 3.713 | 3.849 | 5.479 | 81.766 | 0.73x |
| users.json | ujson | 12.831 | 13.443 | 29.977 | 81.766 | 0.21x |
| users.json | json | 18.155 | 18.394 | 28.100 | 81.766 | 0.15x |
| flat.json | strata | 0.544 | 0.591 | 0.722 | 84.344 | 1.00x |
| flat.json | orjson | 0.595 | 0.673 | 0.827 | 84.344 | 0.88x |
| flat.json | msgspec | 0.681 | 0.726 | 0.814 | 84.344 | 0.81x |
| flat.json | ujson | 1.322 | 1.424 | 1.692 | 84.344 | 0.42x |
| flat.json | json | 1.801 | 1.930 | 2.294 | 84.344 | 0.31x |
| nested.json | strata | 0.445 | 0.455 | 0.737 | 84.344 | 1.00x |
| nested.json | orjson | 0.504 | 0.571 | 0.767 | 84.344 | 0.80x |
| nested.json | msgspec | 0.596 | 0.795 | 1.143 | 84.344 | 0.57x |
| nested.json | ujson | 1.316 | 1.551 | 1.989 | 84.344 | 0.29x |
| nested.json | json | 2.029 | 2.102 | 2.320 | 84.344 | 0.22x |
| wide_arrays.json | strata | 1.547 | 1.636 | 1.733 | 87.328 | 1.00x |
| wide_arrays.json | orjson | 1.955 | 2.178 | 2.276 | 87.328 | 0.75x |
| wide_arrays.json | msgspec | 2.721 | 2.913 | 3.198 | 87.328 | 0.56x |
| wide_arrays.json | ujson | 7.776 | 8.018 | 9.192 | 87.328 | 0.20x |
| wide_arrays.json | json | 13.277 | 13.613 | 14.717 | 87.328 | 0.12x |
| mixed.json | strata | 0.239 | 0.304 | 0.610 | 87.344 | 1.00x |
| mixed.json | orjson | 0.287 | 0.336 | 1.108 | 87.344 | 0.91x |
| mixed.json | msgspec | 0.270 | 0.390 | 0.711 | 87.344 | 0.78x |
| mixed.json | ujson | 0.442 | 0.610 | 1.198 | 87.344 | 0.50x |
| mixed.json | json | 0.596 | 0.665 | 0.980 | 87.344 | 0.46x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.068 | 0.095 | 0.146 | 81.812 | 1.00x |
| users.json $[*].id | jmespath | 0.315 | 0.388 | 0.458 | 81.812 | 0.24x |
| users.json $[*].id | jsonpath-ng | 1.630 | 1.850 | 2.083 | 81.812 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.475 | 0.599 | 0.724 | 82.000 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.107 | 2.290 | 3.099 | 82.000 | 0.26x |
| users.json $[*].orders[*].total | jsonpath-ng | 12.227 | 13.470 | 15.008 | 82.000 | 0.04x |
| users.json $..total | strata | 1.471 | 1.613 | 2.013 | 80.422 | 1.00x |
| users.json $..total | jsonpath-ng | 210.773 | 227.442 | 279.565 | 80.422 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.003 | 4.272 | 4.819 | 81.844 | 1.00x |
| users.json $[*].id | orjson+jmespath | 11.725 | 12.217 | 13.623 | 81.844 | 0.35x |
| users.json $[*].id | orjson+jsonpath-ng | 13.102 | 13.724 | 14.991 | 81.844 | 0.31x |
| users.json $[*].orders[*].total | strata | 4.210 | 4.321 | 5.822 | 82.000 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 13.229 | 16.727 | 18.990 | 82.000 | 0.26x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 27.953 | 32.993 | 47.885 | 82.000 | 0.13x |
| users.json $..total | strata | 9.593 | 9.856 | 11.401 | 80.422 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 221.963 | 225.598 | 287.177 | 80.422 | 0.04x |

