# Benchmark results - medium

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 8cdb525
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
| users.json | strata | 44.566 | 45.586 | 46.947 | 139.938 | 1.00x |
| users.json | orjson | 50.803 | 52.056 | 53.054 | 139.938 | 0.88x |
| users.json | msgspec | 52.254 | 53.257 | 54.792 | 139.938 | 0.86x |
| users.json | ujson | 71.864 | 73.095 | 74.954 | 139.938 | 0.62x |
| users.json | json | 103.854 | 105.559 | 112.917 | 139.938 | 0.43x |
| flat.json | strata | 2.664 | 2.737 | 3.023 | 163.797 | 1.00x |
| flat.json | orjson | 2.784 | 2.874 | 3.230 | 163.797 | 0.95x |
| flat.json | msgspec | 2.893 | 2.961 | 3.285 | 163.797 | 0.92x |
| flat.json | ujson | 4.272 | 4.652 | 4.859 | 163.797 | 0.59x |
| flat.json | json | 5.940 | 6.210 | 6.413 | 163.797 | 0.44x |
| nested.json | strata | 2.457 | 2.598 | 2.777 | 165.688 | 1.00x |
| nested.json | orjson | 2.422 | 2.509 | 3.093 | 165.688 | 1.04x |
| nested.json | msgspec | 2.443 | 2.805 | 3.131 | 165.688 | 0.93x |
| nested.json | ujson | 3.632 | 4.005 | 4.287 | 165.688 | 0.65x |
| nested.json | json | 5.909 | 6.149 | 6.691 | 165.688 | 0.42x |
| wide_arrays.json | strata | 13.849 | 14.066 | 14.488 | 193.344 | 1.00x |
| wide_arrays.json | orjson | 13.774 | 14.093 | 14.361 | 193.344 | 1.00x |
| wide_arrays.json | msgspec | 16.959 | 17.244 | 17.689 | 193.344 | 0.82x |
| wide_arrays.json | ujson | 21.900 | 22.241 | 22.738 | 193.344 | 0.63x |
| wide_arrays.json | json | 31.857 | 32.296 | 33.301 | 193.344 | 0.44x |
| mixed.json | strata | 0.554 | 0.559 | 0.618 | 166.672 | 1.00x |
| mixed.json | orjson | 0.594 | 0.634 | 0.722 | 166.672 | 0.88x |
| mixed.json | msgspec | 0.651 | 0.680 | 0.704 | 166.672 | 0.82x |
| mixed.json | ujson | 0.805 | 0.852 | 0.951 | 166.672 | 0.66x |
| mixed.json | json | 1.328 | 1.400 | 1.578 | 166.672 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.661 | 9.793 | 10.340 | 601.984 | 1.00x |
| users.json | orjson | 12.329 | 12.402 | 12.467 | 601.984 | 0.79x |
| users.json | msgspec | 16.078 | 16.207 | 16.343 | 601.984 | 0.60x |
| users.json | ujson | 62.726 | 63.177 | 64.325 | 601.984 | 0.16x |
| users.json | json | 91.141 | 91.366 | 92.633 | 601.984 | 0.11x |
| flat.json | strata | 0.830 | 0.843 | 0.905 | 163.906 | 1.00x |
| flat.json | orjson | 0.877 | 0.892 | 1.001 | 163.906 | 0.94x |
| flat.json | msgspec | 1.155 | 1.188 | 1.279 | 163.906 | 0.71x |
| flat.json | ujson | 3.494 | 3.636 | 3.758 | 163.906 | 0.23x |
| flat.json | json | 5.645 | 5.820 | 5.954 | 163.906 | 0.14x |
| nested.json | strata | 0.557 | 0.606 | 0.663 | 166.094 | 1.00x |
| nested.json | orjson | 0.790 | 0.823 | 0.930 | 166.094 | 0.74x |
| nested.json | msgspec | 1.049 | 1.081 | 1.218 | 166.094 | 0.56x |
| nested.json | ujson | 3.908 | 4.177 | 4.405 | 166.094 | 0.14x |
| nested.json | json | 6.781 | 7.242 | 7.471 | 166.094 | 0.08x |
| wide_arrays.json | strata | 4.356 | 4.629 | 5.016 | 191.281 | 1.00x |
| wide_arrays.json | orjson | 5.258 | 5.337 | 5.507 | 191.281 | 0.87x |
| wide_arrays.json | msgspec | 8.620 | 8.760 | 8.930 | 191.281 | 0.53x |
| wide_arrays.json | ujson | 24.758 | 25.174 | 25.364 | 191.281 | 0.18x |
| wide_arrays.json | json | 46.014 | 46.613 | 46.773 | 191.281 | 0.10x |
| mixed.json | strata | 0.151 | 0.154 | 0.202 | 166.750 | 1.00x |
| mixed.json | orjson | 0.152 | 0.155 | 0.168 | 166.750 | 0.99x |
| mixed.json | msgspec | 0.171 | 0.179 | 0.205 | 166.750 | 0.86x |
| mixed.json | ujson | 0.833 | 0.848 | 0.866 | 166.750 | 0.18x |
| mixed.json | json | 1.319 | 1.370 | 1.570 | 166.750 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 45.765 | 47.210 | 48.993 | 657.125 | 1.00x |
| users.json | orjson | 52.263 | 54.151 | 55.648 | 657.125 | 0.87x |
| users.json | msgspec | 53.288 | 55.197 | 56.640 | 657.125 | 0.86x |
| users.json | ujson | 74.024 | 76.622 | 77.750 | 657.125 | 0.62x |
| users.json | json | 104.418 | 107.071 | 110.368 | 657.125 | 0.44x |
| flat.json | strata | 2.857 | 2.947 | 3.033 | 165.562 | 1.00x |
| flat.json | orjson | 2.991 | 3.077 | 3.221 | 165.562 | 0.96x |
| flat.json | msgspec | 3.085 | 3.228 | 3.401 | 165.562 | 0.91x |
| flat.json | ujson | 4.604 | 4.879 | 5.055 | 165.562 | 0.60x |
| flat.json | json | 6.036 | 6.269 | 6.432 | 165.562 | 0.47x |
| nested.json | strata | 2.582 | 2.700 | 2.845 | 166.094 | 1.00x |
| nested.json | orjson | 2.509 | 2.619 | 2.777 | 166.094 | 1.03x |
| nested.json | msgspec | 2.540 | 2.679 | 2.980 | 166.094 | 1.01x |
| nested.json | ujson | 3.660 | 4.047 | 4.133 | 166.094 | 0.67x |
| nested.json | json | 5.621 | 5.988 | 6.218 | 166.094 | 0.45x |
| wide_arrays.json | strata | 14.166 | 14.514 | 14.997 | 159.625 | 1.00x |
| wide_arrays.json | orjson | 14.241 | 14.772 | 15.252 | 159.625 | 0.98x |
| wide_arrays.json | msgspec | 17.284 | 18.205 | 18.466 | 159.625 | 0.80x |
| wide_arrays.json | ujson | 22.855 | 23.470 | 23.848 | 159.625 | 0.62x |
| wide_arrays.json | json | 32.128 | 32.759 | 33.249 | 159.625 | 0.44x |
| mixed.json | strata | 0.624 | 0.652 | 0.749 | 166.750 | 1.00x |
| mixed.json | orjson | 0.668 | 0.711 | 0.831 | 166.750 | 0.92x |
| mixed.json | msgspec | 0.746 | 0.799 | 0.886 | 166.750 | 0.82x |
| mixed.json | ujson | 0.938 | 0.973 | 1.173 | 166.750 | 0.67x |
| mixed.json | json | 1.422 | 1.462 | 1.538 | 166.750 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 46.401 | 46.959 | 47.514 | 166.031 | 1.00x |
| users.ndjson | orjson | 63.464 | 64.891 | 65.301 | 166.031 | 0.72x |
| users.ndjson | msgspec | 64.431 | 64.845 | 65.282 | 166.031 | 0.72x |
| users.ndjson | ujson | 82.400 | 83.417 | 84.358 | 166.031 | 0.56x |
| users.ndjson | json | 108.605 | 109.722 | 112.004 | 166.031 | 0.43x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.150 | 12.833 | 14.715 | 513.109 | 1.00x |
| users.json | orjson | 13.950 | 15.447 | 16.017 | 513.109 | 0.83x |
| users.json | msgspec | 17.820 | 19.154 | 21.241 | 513.109 | 0.67x |
| users.json | ujson | 66.338 | 68.337 | 69.007 | 513.109 | 0.19x |
| users.json | json | 93.579 | 94.985 | 97.144 | 513.109 | 0.14x |
| flat.json | strata | 1.121 | 1.146 | 1.247 | 165.656 | 1.00x |
| flat.json | orjson | 1.072 | 1.149 | 1.225 | 165.656 | 1.00x |
| flat.json | msgspec | 1.396 | 1.500 | 1.565 | 165.656 | 0.76x |
| flat.json | ujson | 3.898 | 3.994 | 4.047 | 165.656 | 0.29x |
| flat.json | json | 5.961 | 6.009 | 6.213 | 165.656 | 0.19x |
| nested.json | strata | 0.754 | 0.829 | 0.858 | 166.094 | 1.00x |
| nested.json | orjson | 1.063 | 1.110 | 1.287 | 166.094 | 0.75x |
| nested.json | msgspec | 1.254 | 1.358 | 1.483 | 166.094 | 0.61x |
| nested.json | ujson | 4.226 | 4.361 | 4.530 | 166.094 | 0.19x |
| nested.json | json | 6.998 | 7.411 | 7.943 | 166.094 | 0.11x |
| wide_arrays.json | strata | 5.304 | 5.735 | 6.103 | 166.672 | 1.00x |
| wide_arrays.json | orjson | 6.249 | 6.439 | 7.177 | 166.672 | 0.89x |
| wide_arrays.json | msgspec | 9.714 | 10.072 | 10.579 | 166.672 | 0.57x |
| wide_arrays.json | ujson | 25.928 | 26.512 | 26.976 | 166.672 | 0.22x |
| wide_arrays.json | json | 47.623 | 48.281 | 51.242 | 166.672 | 0.12x |
| mixed.json | strata | 0.292 | 0.333 | 0.398 | 166.750 | 1.00x |
| mixed.json | orjson | 0.315 | 0.355 | 0.433 | 166.750 | 0.94x |
| mixed.json | msgspec | 0.315 | 0.349 | 0.411 | 166.750 | 0.95x |
| mixed.json | ujson | 1.009 | 1.109 | 1.218 | 166.750 | 0.30x |
| mixed.json | json | 1.603 | 1.703 | 1.825 | 166.750 | 0.20x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.187 | 0.217 | 0.285 | 513.375 | 1.00x |
| users.json $[*].id | jmespath | 0.738 | 0.831 | 0.850 | 513.375 | 0.26x |
| users.json $[*].id | jsonpath-ng | 3.065 | 3.150 | 3.203 | 513.375 | 0.07x |
| users.json $[*].orders[*].total | strata | 2.705 | 2.915 | 3.365 | 563.703 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 7.042 | 7.315 | 7.603 | 563.703 | 0.40x |
| users.json $[*].orders[*].total | jsonpath-ng | 36.825 | 38.502 | 41.258 | 563.703 | 0.08x |
| users.json $..total | strata | 8.060 | 8.316 | 8.542 | 156.266 | 1.00x |
| users.json $..total | jsonpath-ng | 1130.215 | 1133.161 | 1136.714 | 156.266 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 19.401 | 19.654 | 19.788 | 581.531 | 1.00x |
| users.json $[*].id | orjson+jmespath | 52.965 | 53.959 | 54.873 | 581.531 | 0.36x |
| users.json $[*].id | orjson+jsonpath-ng | 56.011 | 56.701 | 57.804 | 581.531 | 0.35x |
| users.json $[*].orders[*].total | strata | 19.915 | 20.120 | 20.886 | 381.719 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 58.812 | 59.709 | 61.992 | 381.719 | 0.34x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 101.331 | 103.182 | 116.238 | 381.719 | 0.19x |
| users.json $..total | strata | 54.700 | 55.122 | 56.107 | 202.406 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 1194.058 | 1198.726 | 1209.800 | 202.406 | 0.05x |

