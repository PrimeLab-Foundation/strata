# Benchmark results - medium

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 71d959d
- python: 3.14.7
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit-Mach-O
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
| users.json | strata | 40.725 | 41.918 | 46.430 | 139.734 | 1.00x |
| users.json | orjson | 52.109 | 54.211 | 61.795 | 139.734 | 0.77x |
| users.json | msgspec | 52.295 | 56.502 | 60.422 | 139.734 | 0.74x |
| users.json | ujson | 74.606 | 78.949 | 83.587 | 139.734 | 0.53x |
| users.json | json | 105.703 | 108.980 | 115.596 | 139.734 | 0.38x |
| flat.json | strata | 2.408 | 2.456 | 2.588 | 164.375 | 1.00x |
| flat.json | orjson | 2.779 | 2.944 | 3.126 | 164.375 | 0.83x |
| flat.json | msgspec | 2.848 | 3.023 | 3.174 | 164.375 | 0.81x |
| flat.json | ujson | 4.309 | 4.547 | 4.799 | 164.375 | 0.54x |
| flat.json | json | 5.912 | 6.205 | 6.412 | 164.375 | 0.40x |
| nested.json | strata | 2.172 | 2.278 | 2.481 | 167.281 | 1.00x |
| nested.json | orjson | 2.469 | 2.573 | 2.938 | 167.281 | 0.89x |
| nested.json | msgspec | 2.552 | 2.877 | 3.175 | 167.281 | 0.79x |
| nested.json | ujson | 3.530 | 3.785 | 4.162 | 167.281 | 0.60x |
| nested.json | json | 5.699 | 6.246 | 6.681 | 167.281 | 0.36x |
| wide_arrays.json | strata | 12.617 | 12.786 | 12.951 | 194.625 | 1.00x |
| wide_arrays.json | orjson | 13.812 | 14.114 | 14.653 | 194.625 | 0.91x |
| wide_arrays.json | msgspec | 16.713 | 17.305 | 18.251 | 194.625 | 0.74x |
| wide_arrays.json | ujson | 21.822 | 22.102 | 22.933 | 194.625 | 0.58x |
| wide_arrays.json | json | 31.546 | 31.938 | 32.698 | 194.625 | 0.40x |
| mixed.json | strata | 0.495 | 0.510 | 0.559 | 151.906 | 1.00x |
| mixed.json | orjson | 0.591 | 0.629 | 0.711 | 151.906 | 0.81x |
| mixed.json | msgspec | 0.641 | 0.659 | 0.697 | 151.906 | 0.77x |
| mixed.json | ujson | 0.800 | 0.846 | 1.008 | 151.906 | 0.60x |
| mixed.json | json | 1.328 | 1.387 | 1.572 | 151.906 | 0.37x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.545 | 8.842 | 9.524 | 604.141 | 1.00x |
| users.json | orjson | 12.623 | 12.906 | 13.644 | 604.141 | 0.69x |
| users.json | msgspec | 16.723 | 16.895 | 17.290 | 604.141 | 0.52x |
| users.json | ujson | 66.616 | 67.081 | 67.790 | 604.141 | 0.13x |
| users.json | json | 92.496 | 93.218 | 94.332 | 604.141 | 0.09x |
| flat.json | strata | 0.702 | 0.730 | 0.807 | 165.141 | 1.00x |
| flat.json | orjson | 0.873 | 0.913 | 1.012 | 165.141 | 0.80x |
| flat.json | msgspec | 1.151 | 1.186 | 1.269 | 165.141 | 0.62x |
| flat.json | ujson | 3.484 | 3.597 | 3.744 | 165.141 | 0.20x |
| flat.json | json | 5.392 | 5.784 | 6.003 | 165.141 | 0.13x |
| nested.json | strata | 0.539 | 0.567 | 0.625 | 167.375 | 1.00x |
| nested.json | orjson | 0.790 | 0.836 | 0.919 | 167.375 | 0.68x |
| nested.json | msgspec | 1.052 | 1.091 | 1.269 | 167.375 | 0.52x |
| nested.json | ujson | 3.933 | 4.067 | 4.436 | 167.375 | 0.14x |
| nested.json | json | 6.725 | 7.300 | 7.443 | 167.375 | 0.08x |
| wide_arrays.json | strata | 4.303 | 4.384 | 4.642 | 143.688 | 1.00x |
| wide_arrays.json | orjson | 5.099 | 5.379 | 5.668 | 143.688 | 0.81x |
| wide_arrays.json | msgspec | 8.684 | 8.878 | 9.063 | 143.688 | 0.49x |
| wide_arrays.json | ujson | 26.422 | 26.758 | 27.155 | 143.688 | 0.16x |
| wide_arrays.json | json | 46.161 | 46.774 | 47.593 | 143.688 | 0.09x |
| mixed.json | strata | 0.136 | 0.139 | 0.168 | 151.984 | 1.00x |
| mixed.json | orjson | 0.154 | 0.163 | 0.170 | 151.984 | 0.85x |
| mixed.json | msgspec | 0.170 | 0.188 | 0.216 | 151.984 | 0.74x |
| mixed.json | ujson | 0.850 | 0.870 | 0.959 | 151.984 | 0.16x |
| mixed.json | json | 1.486 | 1.600 | 1.732 | 151.984 | 0.09x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 41.656 | 43.991 | 75.736 | 657.109 | 1.00x |
| users.json | orjson | 53.277 | 56.507 | 62.823 | 657.109 | 0.78x |
| users.json | msgspec | 55.584 | 58.144 | 63.245 | 657.109 | 0.76x |
| users.json | ujson | 77.686 | 80.980 | 86.922 | 657.109 | 0.54x |
| users.json | json | 108.096 | 110.486 | 120.238 | 657.109 | 0.40x |
| flat.json | strata | 2.663 | 2.719 | 2.915 | 167.250 | 1.00x |
| flat.json | orjson | 2.973 | 3.100 | 3.273 | 167.250 | 0.88x |
| flat.json | msgspec | 3.078 | 3.154 | 3.383 | 167.250 | 0.86x |
| flat.json | ujson | 4.458 | 4.761 | 5.069 | 167.250 | 0.57x |
| flat.json | json | 6.032 | 6.192 | 6.541 | 167.250 | 0.44x |
| nested.json | strata | 2.326 | 2.445 | 2.567 | 167.375 | 1.00x |
| nested.json | orjson | 2.537 | 2.683 | 2.851 | 167.375 | 0.91x |
| nested.json | msgspec | 2.569 | 2.806 | 2.936 | 167.375 | 0.87x |
| nested.json | ujson | 3.749 | 3.959 | 4.345 | 167.375 | 0.62x |
| nested.json | json | 5.891 | 6.079 | 6.213 | 167.375 | 0.40x |
| wide_arrays.json | strata | 12.994 | 13.246 | 13.354 | 148.656 | 1.00x |
| wide_arrays.json | orjson | 14.498 | 14.844 | 15.212 | 148.656 | 0.89x |
| wide_arrays.json | msgspec | 17.662 | 17.998 | 18.492 | 148.656 | 0.74x |
| wide_arrays.json | ujson | 22.972 | 23.481 | 24.290 | 148.656 | 0.56x |
| wide_arrays.json | json | 32.339 | 32.511 | 33.059 | 148.656 | 0.41x |
| mixed.json | strata | 0.571 | 0.615 | 0.710 | 151.984 | 1.00x |
| mixed.json | orjson | 0.676 | 0.736 | 0.802 | 151.984 | 0.84x |
| mixed.json | msgspec | 0.733 | 0.777 | 0.854 | 151.984 | 0.79x |
| mixed.json | ujson | 0.917 | 1.003 | 1.167 | 151.984 | 0.61x |
| mixed.json | json | 1.416 | 1.518 | 1.630 | 151.984 | 0.41x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 40.455 | 41.054 | 41.905 | 167.969 | 1.00x |
| users.ndjson | orjson | 63.465 | 64.337 | 65.030 | 167.969 | 0.64x |
| users.ndjson | msgspec | 64.094 | 64.792 | 65.896 | 167.969 | 0.63x |
| users.ndjson | ujson | 82.771 | 83.329 | 84.294 | 167.969 | 0.49x |
| users.ndjson | json | 107.500 | 108.536 | 109.376 | 167.969 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.637 | 11.399 | 12.088 | 509.781 | 1.00x |
| users.json | orjson | 14.846 | 15.429 | 16.210 | 509.781 | 0.74x |
| users.json | msgspec | 18.511 | 19.384 | 19.925 | 509.781 | 0.59x |
| users.json | ujson | 67.484 | 68.512 | 69.614 | 509.781 | 0.17x |
| users.json | json | 96.009 | 96.670 | 98.651 | 509.781 | 0.12x |
| flat.json | strata | 0.961 | 1.007 | 1.094 | 167.250 | 1.00x |
| flat.json | orjson | 1.148 | 1.238 | 1.483 | 167.250 | 0.81x |
| flat.json | msgspec | 1.367 | 1.535 | 1.622 | 167.250 | 0.66x |
| flat.json | ujson | 3.751 | 3.902 | 4.412 | 167.250 | 0.26x |
| flat.json | json | 5.705 | 6.179 | 6.811 | 167.250 | 0.16x |
| nested.json | strata | 0.766 | 0.836 | 1.015 | 167.375 | 1.00x |
| nested.json | orjson | 1.030 | 1.086 | 1.396 | 167.375 | 0.77x |
| nested.json | msgspec | 1.323 | 1.373 | 1.523 | 167.375 | 0.61x |
| nested.json | ujson | 4.192 | 4.449 | 4.655 | 167.375 | 0.19x |
| nested.json | json | 6.687 | 7.377 | 7.621 | 167.375 | 0.11x |
| wide_arrays.json | strata | 5.333 | 5.769 | 5.933 | 151.906 | 1.00x |
| wide_arrays.json | orjson | 6.193 | 6.518 | 7.160 | 151.906 | 0.89x |
| wide_arrays.json | msgspec | 9.680 | 9.949 | 10.768 | 151.906 | 0.58x |
| wide_arrays.json | ujson | 27.758 | 28.168 | 29.288 | 151.906 | 0.20x |
| wide_arrays.json | json | 47.388 | 47.927 | 51.318 | 151.906 | 0.12x |
| mixed.json | strata | 0.312 | 0.348 | 0.415 | 151.984 | 1.00x |
| mixed.json | orjson | 0.327 | 0.359 | 0.711 | 151.984 | 0.97x |
| mixed.json | msgspec | 0.339 | 0.370 | 0.486 | 151.984 | 0.94x |
| mixed.json | ujson | 1.035 | 1.135 | 1.263 | 151.984 | 0.31x |
| mixed.json | json | 1.536 | 1.783 | 1.926 | 151.984 | 0.20x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.199 | 0.219 | 0.299 | 509.984 | 1.00x |
| users.json $[*].id | jmespath | 0.788 | 0.847 | 1.216 | 509.984 | 0.26x |
| users.json $[*].id | jsonpath-ng | 3.106 | 3.219 | 3.538 | 509.984 | 0.07x |
| users.json $[*].orders[*].total | strata | 2.715 | 2.933 | 6.639 | 297.922 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 7.442 | 7.873 | 9.561 | 297.922 | 0.37x |
| users.json $[*].orders[*].total | jsonpath-ng | 39.883 | 40.873 | 43.204 | 297.922 | 0.07x |
| users.json $..total | strata | 8.324 | 8.410 | 9.003 | 157.047 | 1.00x |
| users.json $..total | jsonpath-ng | 1135.767 | 1138.450 | 1140.384 | 157.047 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 20.058 | 20.241 | 20.347 | 577.203 | 1.00x |
| users.json $[*].id | orjson+jmespath | 54.529 | 55.854 | 60.448 | 577.203 | 0.36x |
| users.json $[*].id | orjson+jsonpath-ng | 57.840 | 59.102 | 60.365 | 577.203 | 0.34x |
| users.json $[*].orders[*].total | strata | 20.358 | 20.575 | 21.009 | 360.016 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 59.503 | 61.713 | 63.449 | 360.016 | 0.33x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 105.494 | 108.075 | 112.158 | 360.016 | 0.19x |
| users.json $..total | strata | 48.762 | 49.020 | 50.781 | 202.234 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 1201.177 | 1203.603 | 1211.839 | 202.234 | 0.04x |

