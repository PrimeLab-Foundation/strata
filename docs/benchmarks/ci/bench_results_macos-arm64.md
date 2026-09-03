# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 2b2f55a
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
| users.json | strata | 6.805 | 8.052 | 10.006 | 63.734 | 1.00x |
| users.json | orjson | 9.184 | 11.255 | 12.555 | 63.734 | 0.72x |
| users.json | msgspec | 8.731 | 11.081 | 14.240 | 63.734 | 0.73x |
| users.json | ujson | 11.644 | 15.500 | 22.668 | 63.734 | 0.52x |
| users.json | pysimdjson | 126.188 | 150.886 | 158.659 | 63.734 | 0.05x |
| users.json | json | 15.142 | 17.285 | 23.306 | 63.734 | 0.47x |
| flat.json | strata | 0.648 | 0.663 | 0.843 | 91.125 | 1.00x |
| flat.json | orjson | 0.809 | 0.883 | 1.159 | 91.125 | 0.75x |
| flat.json | msgspec | 0.730 | 0.820 | 0.924 | 91.125 | 0.81x |
| flat.json | ujson | 1.245 | 1.417 | 1.586 | 91.125 | 0.47x |
| flat.json | pysimdjson | 12.303 | 13.171 | 14.188 | 91.125 | 0.05x |
| flat.json | json | 1.418 | 1.537 | 1.653 | 91.125 | 0.43x |
| nested.json | strata | 0.603 | 0.653 | 1.025 | 91.141 | 1.00x |
| nested.json | orjson | 0.842 | 0.893 | 1.213 | 91.141 | 0.73x |
| nested.json | msgspec | 0.749 | 0.828 | 1.087 | 91.141 | 0.79x |
| nested.json | ujson | 1.255 | 1.436 | 1.631 | 91.141 | 0.45x |
| nested.json | pysimdjson | 11.059 | 12.522 | 15.154 | 91.141 | 0.05x |
| nested.json | json | 1.598 | 1.767 | 2.450 | 91.141 | 0.37x |
| wide_arrays.json | strata | 3.822 | 4.386 | 4.730 | 93.781 | 1.00x |
| wide_arrays.json | orjson | 4.398 | 4.671 | 8.339 | 93.781 | 0.94x |
| wide_arrays.json | msgspec | 4.704 | 5.289 | 12.654 | 93.781 | 0.83x |
| wide_arrays.json | ujson | 6.256 | 7.130 | 13.679 | 93.781 | 0.62x |
| wide_arrays.json | pysimdjson | 66.484 | 74.626 | 86.026 | 93.781 | 0.06x |
| wide_arrays.json | json | 7.532 | 8.382 | 11.140 | 93.781 | 0.52x |
| mixed.json | strata | 0.141 | 0.164 | 0.339 | 93.797 | 1.00x |
| mixed.json | orjson | 0.177 | 0.210 | 0.451 | 93.797 | 0.78x |
| mixed.json | msgspec | 0.185 | 0.220 | 0.414 | 93.797 | 0.74x |
| mixed.json | ujson | 0.245 | 0.406 | 0.630 | 93.797 | 0.40x |
| mixed.json | pysimdjson | 2.729 | 3.034 | 3.365 | 93.797 | 0.05x |
| mixed.json | json | 0.370 | 0.401 | 0.718 | 93.797 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.803 | 2.148 | 3.414 | 76.219 | 1.00x |
| users.json | orjson | 2.520 | 2.896 | 3.745 | 76.219 | 0.74x |
| users.json | msgspec | 3.358 | 3.796 | 5.779 | 76.219 | 0.57x |
| users.json | ujson | 11.485 | 13.280 | 22.875 | 76.219 | 0.16x |
| users.json | json | 16.198 | 18.343 | 20.918 | 76.219 | 0.12x |
| flat.json | strata | 0.246 | 0.272 | 0.374 | 91.141 | 1.00x |
| flat.json | orjson | 0.301 | 0.346 | 0.378 | 91.141 | 0.79x |
| flat.json | msgspec | 0.356 | 0.405 | 0.430 | 91.141 | 0.67x |
| flat.json | ujson | 0.992 | 1.124 | 1.292 | 91.141 | 0.24x |
| flat.json | json | 1.628 | 1.670 | 1.887 | 91.141 | 0.16x |
| nested.json | strata | 0.160 | 0.171 | 0.229 | 91.141 | 1.00x |
| nested.json | orjson | 0.264 | 0.286 | 0.402 | 91.141 | 0.60x |
| nested.json | msgspec | 0.492 | 0.539 | 0.715 | 91.141 | 0.32x |
| nested.json | ujson | 1.263 | 1.329 | 1.688 | 91.141 | 0.13x |
| nested.json | json | 1.892 | 1.996 | 4.186 | 91.141 | 0.09x |
| wide_arrays.json | strata | 1.206 | 1.345 | 1.437 | 93.781 | 1.00x |
| wide_arrays.json | orjson | 1.520 | 1.727 | 2.039 | 93.781 | 0.78x |
| wide_arrays.json | msgspec | 2.333 | 2.549 | 2.796 | 93.781 | 0.53x |
| wide_arrays.json | ujson | 6.782 | 7.836 | 12.797 | 93.781 | 0.17x |
| wide_arrays.json | json | 12.252 | 13.893 | 27.766 | 93.781 | 0.10x |
| mixed.json | strata | 0.049 | 0.063 | 0.098 | 93.797 | 1.00x |
| mixed.json | orjson | 0.054 | 0.069 | 0.159 | 93.797 | 0.91x |
| mixed.json | msgspec | 0.059 | 0.159 | 0.474 | 93.797 | 0.40x |
| mixed.json | ujson | 0.235 | 0.295 | 0.413 | 93.797 | 0.21x |
| mixed.json | json | 0.404 | 0.450 | 0.519 | 93.797 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.700 | 8.245 | 17.303 | 89.594 | 1.00x |
| users.json | orjson | 11.014 | 11.243 | 11.997 | 89.594 | 0.73x |
| users.json | msgspec | 10.235 | 10.679 | 11.847 | 89.594 | 0.77x |
| users.json | ujson | 13.436 | 15.249 | 19.072 | 89.594 | 0.54x |
| users.json | json | 16.072 | 18.475 | 27.497 | 89.594 | 0.45x |
| flat.json | strata | 0.804 | 0.853 | 0.940 | 91.141 | 1.00x |
| flat.json | orjson | 1.257 | 1.362 | 1.498 | 91.141 | 0.63x |
| flat.json | msgspec | 0.941 | 0.997 | 1.077 | 91.141 | 0.86x |
| flat.json | ujson | 1.450 | 1.475 | 1.556 | 91.141 | 0.58x |
| flat.json | json | 1.651 | 1.801 | 1.995 | 91.141 | 0.47x |
| nested.json | strata | 0.746 | 0.800 | 2.748 | 91.141 | 1.00x |
| nested.json | orjson | 1.122 | 1.262 | 1.483 | 91.141 | 0.63x |
| nested.json | msgspec | 0.984 | 1.022 | 2.744 | 91.141 | 0.78x |
| nested.json | ujson | 1.250 | 1.391 | 1.539 | 91.141 | 0.58x |
| nested.json | json | 1.725 | 1.924 | 2.110 | 91.141 | 0.42x |
| wide_arrays.json | strata | 3.880 | 4.423 | 11.869 | 93.781 | 1.00x |
| wide_arrays.json | orjson | 4.193 | 5.037 | 6.101 | 93.781 | 0.88x |
| wide_arrays.json | msgspec | 4.641 | 5.554 | 7.070 | 93.781 | 0.80x |
| wide_arrays.json | ujson | 6.769 | 7.419 | 10.650 | 93.781 | 0.60x |
| wide_arrays.json | json | 7.646 | 8.536 | 10.057 | 93.781 | 0.52x |
| mixed.json | strata | 0.221 | 0.319 | 0.513 | 93.797 | 1.00x |
| mixed.json | orjson | 0.504 | 0.543 | 0.809 | 93.797 | 0.59x |
| mixed.json | msgspec | 0.320 | 0.394 | 0.678 | 93.797 | 0.81x |
| mixed.json | ujson | 0.450 | 0.512 | 0.799 | 93.797 | 0.62x |
| mixed.json | json | 0.510 | 0.626 | 0.855 | 93.797 | 0.51x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 8.356 | 9.297 | 9.514 | 91.125 | 1.00x |
| users.ndjson | orjson | 12.960 | 14.805 | 14.997 | 91.125 | 0.63x |
| users.ndjson | msgspec | 12.833 | 14.376 | 15.369 | 91.125 | 0.65x |
| users.ndjson | ujson | 16.645 | 18.061 | 19.575 | 91.125 | 0.51x |
| users.ndjson | json | 21.446 | 23.463 | 25.429 | 91.125 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.096 | 2.938 | 5.880 | 86.453 | 1.00x |
| users.json | orjson | 2.956 | 3.546 | 5.858 | 86.453 | 0.83x |
| users.json | msgspec | 3.699 | 4.278 | 6.309 | 86.453 | 0.69x |
| users.json | ujson | 12.829 | 15.864 | 23.868 | 86.453 | 0.19x |
| users.json | json | 17.442 | 26.162 | 30.248 | 86.453 | 0.11x |
| flat.json | strata | 0.489 | 0.613 | 0.733 | 91.141 | 1.00x |
| flat.json | orjson | 0.583 | 0.710 | 0.911 | 91.141 | 0.86x |
| flat.json | msgspec | 0.631 | 0.733 | 1.518 | 91.141 | 0.84x |
| flat.json | ujson | 1.306 | 1.537 | 1.845 | 91.141 | 0.40x |
| flat.json | json | 1.880 | 2.202 | 3.188 | 91.141 | 0.28x |
| nested.json | strata | 0.396 | 0.448 | 0.577 | 91.141 | 1.00x |
| nested.json | orjson | 0.514 | 0.572 | 0.668 | 91.141 | 0.78x |
| nested.json | msgspec | 0.678 | 0.900 | 1.152 | 91.141 | 0.50x |
| nested.json | ujson | 1.480 | 1.734 | 2.001 | 91.141 | 0.26x |
| nested.json | json | 2.024 | 2.214 | 2.316 | 91.141 | 0.20x |
| wide_arrays.json | strata | 1.473 | 1.758 | 2.326 | 93.781 | 1.00x |
| wide_arrays.json | orjson | 2.045 | 2.362 | 2.473 | 93.781 | 0.74x |
| wide_arrays.json | msgspec | 2.688 | 3.149 | 3.818 | 93.781 | 0.56x |
| wide_arrays.json | ujson | 7.893 | 9.112 | 11.519 | 93.781 | 0.19x |
| wide_arrays.json | json | 12.885 | 15.047 | 15.926 | 93.781 | 0.12x |
| mixed.json | strata | 0.217 | 0.316 | 0.486 | 93.797 | 1.00x |
| mixed.json | orjson | 0.236 | 0.324 | 0.462 | 93.797 | 0.98x |
| mixed.json | msgspec | 0.278 | 0.490 | 0.711 | 93.797 | 0.64x |
| mixed.json | ujson | 0.479 | 0.603 | 0.728 | 93.797 | 0.52x |
| mixed.json | json | 0.599 | 0.703 | 0.853 | 93.797 | 0.45x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.064 | 0.091 | 0.240 | 86.500 | 1.00x |
| users.json $[*].id | jmespath | 0.292 | 0.364 | 1.015 | 86.500 | 0.25x |
| users.json $[*].id | jsonpath-ng | 1.561 | 1.682 | 2.469 | 86.500 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.330 | 0.702 | 1.334 | 86.656 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.757 | 2.359 | 2.988 | 86.656 | 0.30x |
| users.json $[*].orders[*].total | jsonpath-ng | 10.596 | 12.125 | 16.120 | 86.656 | 0.06x |
| users.json $..total | strata | 1.316 | 1.795 | 2.640 | 88.500 | 1.00x |
| users.json $..total | jsonpath-ng | 199.379 | 236.358 | 252.787 | 88.500 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.647 | 4.273 | 5.639 | 86.531 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.340 | 12.910 | 16.001 | 86.531 | 0.33x |
| users.json $[*].id | orjson+jsonpath-ng | 11.358 | 13.159 | 17.961 | 86.531 | 0.32x |
| users.json $[*].orders[*].total | strata | 4.204 | 4.597 | 5.510 | 88.469 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 14.890 | 15.764 | 18.890 | 88.469 | 0.29x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 30.513 | 32.343 | 37.089 | 88.469 | 0.14x |
| users.json $..total | strata | 10.983 | 13.976 | 21.866 | 88.594 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 255.152 | 282.201 | 332.008 | 88.594 | 0.05x |

