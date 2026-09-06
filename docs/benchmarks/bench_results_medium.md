# Benchmark results - medium

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 3758c97
- python: 3.14.7
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit-Mach-O
- machine: arm64
- processor: Apple M1 Max
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 36.968 | 38.960 | 44.994 | 139.906 | 1.00x |
| users.json | orjson | 52.944 | 53.740 | 61.206 | 139.906 | 0.72x |
| users.json | msgspec | 54.070 | 56.057 | 60.808 | 139.906 | 0.70x |
| users.json | ujson | 74.155 | 77.023 | 85.508 | 139.906 | 0.51x |
| users.json | json | 108.875 | 112.282 | 131.081 | 139.906 | 0.35x |
| flat.json | strata | 2.261 | 2.412 | 2.524 | 165.375 | 1.00x |
| flat.json | orjson | 2.825 | 3.017 | 3.180 | 165.375 | 0.80x |
| flat.json | msgspec | 2.837 | 3.003 | 3.228 | 165.375 | 0.80x |
| flat.json | ujson | 4.226 | 4.560 | 4.680 | 165.375 | 0.53x |
| flat.json | json | 5.873 | 6.134 | 6.272 | 165.375 | 0.39x |
| nested.json | strata | 1.926 | 2.106 | 2.198 | 167.266 | 1.00x |
| nested.json | orjson | 2.407 | 2.684 | 3.046 | 167.266 | 0.78x |
| nested.json | msgspec | 2.455 | 2.670 | 2.847 | 167.266 | 0.79x |
| nested.json | ujson | 3.540 | 3.703 | 4.174 | 167.266 | 0.57x |
| nested.json | json | 5.645 | 6.038 | 6.277 | 167.266 | 0.35x |
| wide_arrays.json | strata | 11.962 | 12.340 | 12.980 | 194.703 | 1.00x |
| wide_arrays.json | orjson | 13.965 | 14.287 | 14.679 | 194.703 | 0.86x |
| wide_arrays.json | msgspec | 16.928 | 17.298 | 17.726 | 194.703 | 0.71x |
| wide_arrays.json | ujson | 21.776 | 22.442 | 22.952 | 194.703 | 0.55x |
| wide_arrays.json | json | 31.745 | 32.130 | 32.525 | 194.703 | 0.38x |
| mixed.json | strata | 0.464 | 0.475 | 0.532 | 192.609 | 1.00x |
| mixed.json | orjson | 0.600 | 0.609 | 0.730 | 192.609 | 0.78x |
| mixed.json | msgspec | 0.645 | 0.678 | 0.883 | 192.609 | 0.70x |
| mixed.json | ujson | 0.800 | 0.836 | 1.093 | 192.609 | 0.57x |
| mixed.json | json | 1.333 | 1.371 | 1.561 | 192.609 | 0.35x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.633 | 8.933 | 9.564 | 604.484 | 1.00x |
| users.json | orjson | 12.631 | 13.039 | 13.562 | 604.484 | 0.69x |
| users.json | msgspec | 16.271 | 16.818 | 17.594 | 604.484 | 0.53x |
| users.json | ujson | 65.714 | 66.453 | 67.046 | 604.484 | 0.13x |
| users.json | json | 92.520 | 93.011 | 93.458 | 604.484 | 0.10x |
| flat.json | strata | 0.826 | 0.877 | 0.925 | 165.484 | 1.00x |
| flat.json | orjson | 0.877 | 0.913 | 1.035 | 165.484 | 0.96x |
| flat.json | msgspec | 1.173 | 1.190 | 1.274 | 165.484 | 0.74x |
| flat.json | ujson | 3.496 | 3.587 | 3.707 | 165.484 | 0.24x |
| flat.json | json | 5.483 | 5.701 | 6.004 | 165.484 | 0.15x |
| nested.json | strata | 0.497 | 0.516 | 0.559 | 167.453 | 1.00x |
| nested.json | orjson | 0.792 | 0.803 | 0.873 | 167.453 | 0.64x |
| nested.json | msgspec | 1.049 | 1.068 | 1.185 | 167.453 | 0.48x |
| nested.json | ujson | 3.906 | 4.017 | 4.225 | 167.453 | 0.13x |
| nested.json | json | 6.356 | 6.834 | 7.255 | 167.453 | 0.08x |
| wide_arrays.json | strata | 4.436 | 4.532 | 5.284 | 184.391 | 1.00x |
| wide_arrays.json | orjson | 5.173 | 5.418 | 5.551 | 184.391 | 0.84x |
| wide_arrays.json | msgspec | 8.659 | 8.833 | 9.243 | 184.391 | 0.51x |
| wide_arrays.json | ujson | 25.914 | 26.333 | 26.567 | 184.391 | 0.17x |
| wide_arrays.json | json | 46.314 | 47.044 | 47.280 | 184.391 | 0.10x |
| mixed.json | strata | 0.130 | 0.133 | 0.180 | 192.656 | 1.00x |
| mixed.json | orjson | 0.153 | 0.160 | 0.176 | 192.656 | 0.83x |
| mixed.json | msgspec | 0.171 | 0.179 | 0.212 | 192.656 | 0.74x |
| mixed.json | ujson | 0.846 | 0.867 | 0.995 | 192.656 | 0.15x |
| mixed.json | json | 1.402 | 1.514 | 1.668 | 192.656 | 0.09x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 38.531 | 40.742 | 45.538 | 657.438 | 1.00x |
| users.json | orjson | 54.364 | 56.824 | 60.626 | 657.438 | 0.72x |
| users.json | msgspec | 55.738 | 58.241 | 65.338 | 657.438 | 0.70x |
| users.json | ujson | 78.319 | 81.503 | 89.531 | 657.438 | 0.50x |
| users.json | json | 110.914 | 115.298 | 122.215 | 657.438 | 0.35x |
| flat.json | strata | 2.436 | 2.530 | 2.769 | 167.141 | 1.00x |
| flat.json | orjson | 2.956 | 3.105 | 3.356 | 167.141 | 0.81x |
| flat.json | msgspec | 3.021 | 3.185 | 3.347 | 167.141 | 0.79x |
| flat.json | ujson | 4.527 | 4.796 | 4.978 | 167.141 | 0.53x |
| flat.json | json | 5.985 | 6.170 | 6.452 | 167.141 | 0.41x |
| nested.json | strata | 2.068 | 2.174 | 2.318 | 167.453 | 1.00x |
| nested.json | orjson | 2.515 | 2.631 | 2.848 | 167.453 | 0.83x |
| nested.json | msgspec | 2.568 | 2.708 | 2.809 | 167.453 | 0.80x |
| nested.json | ujson | 3.652 | 3.893 | 4.255 | 167.453 | 0.56x |
| nested.json | json | 5.668 | 5.850 | 6.120 | 167.453 | 0.37x |
| wide_arrays.json | strata | 12.678 | 13.083 | 13.563 | 189.359 | 1.00x |
| wide_arrays.json | orjson | 14.395 | 14.949 | 15.590 | 189.359 | 0.88x |
| wide_arrays.json | msgspec | 17.987 | 18.201 | 18.820 | 189.359 | 0.72x |
| wide_arrays.json | ujson | 23.075 | 23.734 | 24.396 | 189.359 | 0.55x |
| wide_arrays.json | json | 32.467 | 32.820 | 32.996 | 189.359 | 0.40x |
| mixed.json | strata | 0.538 | 0.579 | 0.683 | 192.656 | 1.00x |
| mixed.json | orjson | 0.667 | 0.689 | 0.762 | 192.656 | 0.84x |
| mixed.json | msgspec | 0.729 | 0.789 | 0.998 | 192.656 | 0.73x |
| mixed.json | ujson | 0.919 | 0.960 | 1.031 | 192.656 | 0.60x |
| mixed.json | json | 1.397 | 1.433 | 1.545 | 192.656 | 0.40x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 38.063 | 38.516 | 38.871 | 167.594 | 1.00x |
| users.ndjson | orjson | 64.085 | 65.148 | 65.662 | 167.594 | 0.59x |
| users.ndjson | msgspec | 64.810 | 65.088 | 66.552 | 167.594 | 0.59x |
| users.ndjson | ujson | 83.054 | 83.929 | 85.457 | 167.594 | 0.46x |
| users.ndjson | json | 110.654 | 111.480 | 113.480 | 167.594 | 0.35x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.424 | 11.281 | 12.156 | 548.250 | 1.00x |
| users.json | orjson | 14.507 | 15.139 | 16.115 | 548.250 | 0.75x |
| users.json | msgspec | 18.384 | 18.995 | 21.884 | 548.250 | 0.59x |
| users.json | ujson | 66.657 | 68.107 | 70.723 | 548.250 | 0.17x |
| users.json | json | 94.833 | 95.690 | 96.609 | 548.250 | 0.12x |
| flat.json | strata | 1.116 | 1.150 | 1.285 | 167.266 | 1.00x |
| flat.json | orjson | 1.112 | 1.196 | 1.411 | 167.266 | 0.96x |
| flat.json | msgspec | 1.445 | 1.541 | 1.671 | 167.266 | 0.75x |
| flat.json | ujson | 3.748 | 3.939 | 4.063 | 167.266 | 0.29x |
| flat.json | json | 5.703 | 6.017 | 6.193 | 167.266 | 0.19x |
| nested.json | strata | 0.722 | 0.818 | 1.037 | 167.453 | 1.00x |
| nested.json | orjson | 1.020 | 1.099 | 1.247 | 167.453 | 0.74x |
| nested.json | msgspec | 1.291 | 1.422 | 1.498 | 167.453 | 0.57x |
| nested.json | ujson | 4.184 | 4.346 | 4.641 | 167.453 | 0.19x |
| nested.json | json | 6.672 | 7.111 | 7.543 | 167.453 | 0.11x |
| wide_arrays.json | strata | 5.895 | 6.027 | 6.325 | 192.609 | 1.00x |
| wide_arrays.json | orjson | 6.374 | 6.598 | 6.707 | 192.609 | 0.91x |
| wide_arrays.json | msgspec | 9.995 | 10.230 | 10.300 | 192.609 | 0.59x |
| wide_arrays.json | ujson | 27.444 | 27.873 | 28.311 | 192.609 | 0.22x |
| wide_arrays.json | json | 47.679 | 49.839 | 51.712 | 192.609 | 0.12x |
| mixed.json | strata | 0.267 | 0.343 | 0.445 | 192.656 | 1.00x |
| mixed.json | orjson | 0.306 | 0.334 | 0.446 | 192.656 | 1.03x |
| mixed.json | msgspec | 0.308 | 0.331 | 2.618 | 192.656 | 1.04x |
| mixed.json | ujson | 1.021 | 1.079 | 1.165 | 192.656 | 0.32x |
| mixed.json | json | 1.526 | 1.741 | 7.625 | 192.656 | 0.20x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.197 | 0.256 | 0.310 | 548.703 | 1.00x |
| users.json $[*].id | jmespath | 0.801 | 0.914 | 1.062 | 548.703 | 0.28x |
| users.json $[*].id | jsonpath-ng | 3.188 | 3.315 | 3.706 | 548.703 | 0.08x |
| users.json $[*].orders[*].total | strata | 2.745 | 2.918 | 3.444 | 333.766 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 7.040 | 7.294 | 7.570 | 333.766 | 0.40x |
| users.json $[*].orders[*].total | jsonpath-ng | 37.472 | 37.977 | 41.224 | 333.766 | 0.08x |
| users.json $..total | strata | 8.197 | 8.366 | 8.588 | 154.688 | 1.00x |
| users.json $..total | jsonpath-ng | 1131.702 | 1133.339 | 1138.568 | 154.688 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 19.404 | 19.690 | 20.293 | 613.891 | 1.00x |
| users.json $[*].id | orjson+jmespath | 54.384 | 56.489 | 58.106 | 613.891 | 0.35x |
| users.json $[*].id | orjson+jsonpath-ng | 57.092 | 58.603 | 60.387 | 613.891 | 0.34x |
| users.json $[*].orders[*].total | strata | 19.589 | 19.878 | 20.696 | 399.859 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 59.411 | 61.173 | 63.063 | 399.859 | 0.32x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 104.173 | 107.368 | 115.349 | 399.859 | 0.19x |
| users.json $..total | strata | 46.591 | 47.004 | 49.141 | 202.859 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 1200.518 | 1203.160 | 1225.621 | 202.859 | 0.04x |

