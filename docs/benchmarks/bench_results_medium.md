# Benchmark results - medium

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 4d4a4be
- python: 3.14.3
- implementation: CPython
- platform: macOS-26.3-arm64-arm-64bit-Mach-O
- machine: arm64
- processor: Apple M1 Max
- compiler_flags: -std=c++20 -O3 -march=native
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 41.247 | 42.863 | 45.850 | 139.516 | 1.00x |
| users.json | orjson | 49.995 | 51.952 | 54.339 | 139.516 | 0.83x |
| users.json | msgspec | 51.091 | 53.779 | 58.128 | 139.516 | 0.80x |
| users.json | ujson | 70.296 | 72.848 | 77.368 | 139.516 | 0.59x |
| users.json | json | 87.194 | 88.785 | 91.460 | 139.516 | 0.48x |
| flat.json | strata | 2.497 | 2.520 | 2.780 | 165.188 | 1.00x |
| flat.json | orjson | 3.523 | 3.570 | 3.679 | 165.188 | 0.71x |
| flat.json | msgspec | 3.535 | 3.577 | 3.845 | 165.188 | 0.70x |
| flat.json | ujson | 4.920 | 5.019 | 5.274 | 165.188 | 0.50x |
| flat.json | json | 6.381 | 6.471 | 7.126 | 165.188 | 0.39x |
| nested.json | strata | 2.146 | 2.159 | 2.200 | 167.031 | 1.00x |
| nested.json | orjson | 3.060 | 3.112 | 3.384 | 167.031 | 0.69x |
| nested.json | msgspec | 3.060 | 3.151 | 3.406 | 167.031 | 0.69x |
| nested.json | ujson | 4.223 | 4.448 | 4.790 | 167.031 | 0.49x |
| nested.json | json | 5.910 | 6.105 | 6.638 | 167.031 | 0.35x |
| wide_arrays.json | strata | 13.114 | 13.253 | 13.762 | 194.391 | 1.00x |
| wide_arrays.json | orjson | 13.188 | 13.378 | 14.166 | 194.391 | 0.99x |
| wide_arrays.json | msgspec | 16.178 | 16.251 | 17.284 | 194.391 | 0.82x |
| wide_arrays.json | ujson | 20.855 | 21.405 | 21.865 | 194.391 | 0.62x |
| wide_arrays.json | json | 30.089 | 30.629 | 31.648 | 194.391 | 0.43x |
| mixed.json | strata | 0.505 | 0.513 | 0.546 | 175.328 | 1.00x |
| mixed.json | orjson | 1.166 | 1.204 | 1.610 | 175.328 | 0.43x |
| mixed.json | msgspec | 1.214 | 1.227 | 1.295 | 175.328 | 0.42x |
| mixed.json | ujson | 1.373 | 1.390 | 1.437 | 175.328 | 0.37x |
| mixed.json | json | 1.819 | 1.846 | 2.091 | 175.328 | 0.28x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.268 | 10.455 | 10.757 | 601.828 | 1.00x |
| users.json | orjson | 12.117 | 12.348 | 12.805 | 601.828 | 0.85x |
| users.json | msgspec | 15.850 | 16.104 | 16.506 | 601.828 | 0.65x |
| users.json | ujson | 63.669 | 64.212 | 65.033 | 601.828 | 0.16x |
| users.json | json | 87.695 | 88.395 | 88.858 | 601.828 | 0.12x |
| flat.json | strata | 0.713 | 0.726 | 0.785 | 165.188 | 1.00x |
| flat.json | orjson | 0.864 | 0.873 | 0.915 | 165.188 | 0.83x |
| flat.json | msgspec | 1.136 | 1.143 | 1.169 | 165.188 | 0.64x |
| flat.json | ujson | 3.552 | 3.603 | 3.681 | 165.188 | 0.20x |
| flat.json | json | 4.883 | 4.975 | 5.142 | 165.188 | 0.15x |
| nested.json | strata | 0.574 | 0.579 | 0.606 | 167.031 | 1.00x |
| nested.json | orjson | 0.783 | 0.792 | 0.839 | 167.031 | 0.73x |
| nested.json | msgspec | 1.040 | 1.053 | 1.112 | 167.031 | 0.55x |
| nested.json | ujson | 3.795 | 3.834 | 4.165 | 167.031 | 0.15x |
| nested.json | json | 6.076 | 6.142 | 6.281 | 167.031 | 0.09x |
| wide_arrays.json | strata | 4.546 | 4.892 | 5.064 | 190.297 | 1.00x |
| wide_arrays.json | orjson | 4.964 | 5.054 | 5.444 | 190.297 | 0.97x |
| wide_arrays.json | msgspec | 8.219 | 8.293 | 8.398 | 190.297 | 0.59x |
| wide_arrays.json | ujson | 24.087 | 24.419 | 24.875 | 190.297 | 0.20x |
| wide_arrays.json | json | 43.102 | 43.646 | 44.092 | 190.297 | 0.11x |
| mixed.json | strata | 0.141 | 0.144 | 0.152 | 175.344 | 1.00x |
| mixed.json | orjson | 0.153 | 0.156 | 0.171 | 175.344 | 0.92x |
| mixed.json | msgspec | 0.170 | 0.173 | 0.183 | 175.344 | 0.83x |
| mixed.json | ujson | 0.829 | 0.833 | 0.868 | 175.344 | 0.17x |
| mixed.json | json | 1.280 | 1.301 | 1.430 | 175.344 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 42.994 | 45.152 | 47.177 | 656.922 | 1.00x |
| users.json | orjson | 64.308 | 66.657 | 68.992 | 656.922 | 0.68x |
| users.json | msgspec | 65.536 | 67.706 | 71.969 | 656.922 | 0.67x |
| users.json | ujson | 85.635 | 88.845 | 93.403 | 656.922 | 0.51x |
| users.json | json | 101.551 | 104.175 | 109.600 | 656.922 | 0.43x |
| flat.json | strata | 2.680 | 2.706 | 2.890 | 166.938 | 1.00x |
| flat.json | orjson | 3.888 | 3.968 | 4.148 | 166.938 | 0.68x |
| flat.json | msgspec | 3.932 | 4.033 | 4.322 | 166.938 | 0.67x |
| flat.json | ujson | 5.395 | 5.572 | 5.917 | 166.938 | 0.49x |
| flat.json | json | 6.767 | 6.902 | 7.441 | 166.938 | 0.39x |
| nested.json | strata | 2.280 | 2.298 | 2.433 | 167.031 | 1.00x |
| nested.json | orjson | 3.502 | 3.567 | 3.857 | 167.031 | 0.64x |
| nested.json | msgspec | 3.527 | 3.755 | 3.926 | 167.031 | 0.61x |
| nested.json | ujson | 4.718 | 4.894 | 5.350 | 167.031 | 0.47x |
| nested.json | json | 6.429 | 6.541 | 6.940 | 167.031 | 0.35x |
| wide_arrays.json | strata | 13.760 | 13.881 | 14.206 | 199.250 | 1.00x |
| wide_arrays.json | orjson | 15.346 | 15.529 | 15.770 | 199.250 | 0.89x |
| wide_arrays.json | msgspec | 18.555 | 18.715 | 19.106 | 199.250 | 0.74x |
| wide_arrays.json | ujson | 23.696 | 24.026 | 24.234 | 199.250 | 0.58x |
| wide_arrays.json | json | 32.431 | 32.948 | 33.723 | 199.250 | 0.42x |
| mixed.json | strata | 0.569 | 0.593 | 0.661 | 175.344 | 1.00x |
| mixed.json | orjson | 1.342 | 1.373 | 1.439 | 175.344 | 0.43x |
| mixed.json | msgspec | 1.383 | 1.407 | 1.472 | 175.344 | 0.42x |
| mixed.json | ujson | 1.578 | 1.622 | 1.793 | 175.344 | 0.37x |
| mixed.json | json | 1.995 | 2.043 | 2.150 | 175.344 | 0.29x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 41.710 | 42.433 | 43.139 | 167.422 | 1.00x |
| users.ndjson | orjson | 62.870 | 64.135 | 65.673 | 167.422 | 0.66x |
| users.ndjson | msgspec | 62.310 | 63.187 | 64.571 | 167.422 | 0.67x |
| users.ndjson | ujson | 80.256 | 81.888 | 83.273 | 167.422 | 0.52x |
| users.ndjson | json | 102.434 | 103.054 | 107.030 | 167.422 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.630 | 11.772 | 12.018 | 673.078 | 1.00x |
| users.json | orjson | 13.708 | 13.849 | 14.224 | 673.078 | 0.85x |
| users.json | msgspec | 17.511 | 17.647 | 17.829 | 673.078 | 0.67x |
| users.json | ujson | 64.136 | 64.268 | 65.664 | 673.078 | 0.18x |
| users.json | json | 88.032 | 88.858 | 89.870 | 673.078 | 0.13x |
| flat.json | strata | 0.942 | 0.997 | 1.081 | 167.031 | 1.00x |
| flat.json | orjson | 1.103 | 1.166 | 1.301 | 167.031 | 0.86x |
| flat.json | msgspec | 1.377 | 1.408 | 1.543 | 167.031 | 0.71x |
| flat.json | ujson | 3.669 | 3.812 | 3.985 | 167.031 | 0.26x |
| flat.json | json | 5.125 | 5.243 | 5.477 | 167.031 | 0.19x |
| nested.json | strata | 0.789 | 0.807 | 0.832 | 167.141 | 1.00x |
| nested.json | orjson | 0.984 | 1.017 | 1.100 | 167.141 | 0.79x |
| nested.json | msgspec | 1.253 | 1.286 | 1.363 | 167.141 | 0.63x |
| nested.json | ujson | 4.021 | 4.134 | 9.030 | 167.141 | 0.20x |
| nested.json | json | 6.364 | 6.449 | 6.658 | 167.141 | 0.13x |
| wide_arrays.json | strata | 5.397 | 5.756 | 6.057 | 206.297 | 1.00x |
| wide_arrays.json | orjson | 5.780 | 5.904 | 6.195 | 206.297 | 0.98x |
| wide_arrays.json | msgspec | 9.120 | 9.286 | 9.698 | 206.297 | 0.62x |
| wide_arrays.json | ujson | 25.298 | 25.546 | 26.231 | 206.297 | 0.23x |
| wide_arrays.json | json | 44.244 | 44.808 | 45.518 | 206.297 | 0.13x |
| mixed.json | strata | 0.259 | 0.272 | 0.316 | 175.344 | 1.00x |
| mixed.json | orjson | 0.276 | 0.290 | 0.309 | 175.344 | 0.94x |
| mixed.json | msgspec | 0.292 | 0.308 | 0.332 | 175.344 | 0.88x |
| mixed.json | ujson | 0.959 | 1.003 | 1.136 | 175.344 | 0.27x |
| mixed.json | json | 1.411 | 1.429 | 1.479 | 175.344 | 0.19x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.170 | 0.176 | 0.221 | 673.406 | 1.00x |
| users.json $[*].id | jmespath | 0.787 | 0.814 | 0.862 | 673.406 | 0.22x |
| users.json $[*].id | jsonpath-ng | 12.552 | 12.691 | 13.823 | 673.406 | 0.01x |
| users.json $[*].orders[*].total | strata | 2.454 | 2.521 | 2.609 | 683.219 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 8.190 | 8.299 | 8.515 | 683.219 | 0.30x |
| users.json $[*].orders[*].total | jsonpath-ng | 44.705 | 45.470 | 46.065 | 683.219 | 0.06x |
| users.json $..total | strata | 7.802 | 7.871 | 8.104 | 259.594 | 1.00x |
| users.json $..total | jsonpath-ng | 1243.751 | 1244.589 | 1249.018 | 259.594 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 18.681 | 18.777 | 19.128 | 696.000 | 1.00x |
| users.json $[*].id | orjson+jmespath | 65.809 | 66.655 | 67.841 | 696.000 | 0.28x |
| users.json $[*].id | orjson+jsonpath-ng | 68.000 | 68.928 | 70.616 | 696.000 | 0.27x |
| users.json $[*].orders[*].total | strata | 19.075 | 19.228 | 19.354 | 716.359 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 71.613 | 72.658 | 73.219 | 716.359 | 0.26x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 99.920 | 100.682 | 102.276 | 716.359 | 0.19x |
| users.json $..total | strata | 60.308 | 61.333 | 62.330 | 202.547 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 1297.027 | 1299.929 | 1322.031 | 202.547 | 0.05x |

