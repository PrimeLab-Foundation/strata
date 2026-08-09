# Benchmark results - medium

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: a4cb8c0
- python: 3.14.3
- implementation: CPython
- platform: macOS-26.3-arm64-arm-64bit-Mach-O
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
| users.json | strata | 60.044 | 63.021 | 66.023 | 139.375 | 1.00x |
| users.json | orjson | 64.086 | 66.676 | 68.745 | 139.375 | 0.95x |
| users.json | msgspec | 64.624 | 67.760 | 71.989 | 139.375 | 0.93x |
| users.json | ujson | 89.016 | 93.174 | 98.253 | 139.375 | 0.68x |
| users.json | json | 112.350 | 116.347 | 121.371 | 139.375 | 0.54x |
| flat.json | strata | 3.801 | 3.833 | 4.066 | 165.812 | 1.00x |
| flat.json | orjson | 4.551 | 4.653 | 4.948 | 165.812 | 0.82x |
| flat.json | msgspec | 4.554 | 4.644 | 5.146 | 165.812 | 0.83x |
| flat.json | ujson | 6.284 | 6.363 | 6.939 | 165.812 | 0.60x |
| flat.json | json | 8.251 | 8.326 | 8.980 | 165.812 | 0.46x |
| nested.json | strata | 3.153 | 3.175 | 3.380 | 167.719 | 1.00x |
| nested.json | orjson | 3.916 | 4.021 | 4.115 | 167.719 | 0.79x |
| nested.json | msgspec | 3.920 | 3.964 | 4.006 | 167.719 | 0.80x |
| nested.json | ujson | 5.278 | 5.341 | 5.677 | 167.719 | 0.59x |
| nested.json | json | 7.665 | 7.695 | 8.314 | 167.719 | 0.41x |
| wide_arrays.json | strata | 18.882 | 18.983 | 19.726 | 195.078 | 1.00x |
| wide_arrays.json | orjson | 16.979 | 17.263 | 17.713 | 195.078 | 1.10x |
| wide_arrays.json | msgspec | 20.753 | 21.048 | 21.749 | 195.078 | 0.90x |
| wide_arrays.json | ujson | 26.919 | 27.372 | 27.864 | 195.078 | 0.69x |
| wide_arrays.json | json | 38.929 | 39.407 | 40.467 | 195.078 | 0.48x |
| mixed.json | strata | 0.821 | 0.828 | 0.834 | 175.984 | 1.00x |
| mixed.json | orjson | 1.505 | 1.530 | 1.802 | 175.984 | 0.54x |
| mixed.json | msgspec | 1.560 | 1.581 | 1.701 | 175.984 | 0.52x |
| mixed.json | ujson | 1.765 | 1.835 | 1.947 | 175.984 | 0.45x |
| mixed.json | json | 2.346 | 2.402 | 2.565 | 175.984 | 0.34x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 15.209 | 15.350 | 15.781 | 601.516 | 1.00x |
| users.json | orjson | 15.798 | 16.052 | 16.348 | 601.516 | 0.96x |
| users.json | msgspec | 20.665 | 20.783 | 21.392 | 601.516 | 0.74x |
| users.json | ujson | 83.832 | 84.306 | 84.937 | 601.516 | 0.18x |
| users.json | json | 115.175 | 116.367 | 118.469 | 601.516 | 0.13x |
| flat.json | strata | 1.080 | 1.083 | 1.109 | 165.969 | 1.00x |
| flat.json | orjson | 1.124 | 1.150 | 1.364 | 165.969 | 0.94x |
| flat.json | msgspec | 1.495 | 1.529 | 1.546 | 165.969 | 0.71x |
| flat.json | ujson | 4.630 | 4.676 | 4.938 | 165.969 | 0.23x |
| flat.json | json | 6.426 | 6.494 | 6.895 | 165.969 | 0.17x |
| nested.json | strata | 0.809 | 0.820 | 0.864 | 167.750 | 1.00x |
| nested.json | orjson | 1.025 | 1.033 | 1.082 | 167.750 | 0.79x |
| nested.json | msgspec | 1.359 | 1.395 | 1.414 | 167.750 | 0.59x |
| nested.json | ujson | 4.965 | 5.027 | 5.281 | 167.750 | 0.16x |
| nested.json | json | 7.979 | 8.035 | 8.423 | 167.750 | 0.10x |
| wide_arrays.json | strata | 5.937 | 6.226 | 6.542 | 190.953 | 1.00x |
| wide_arrays.json | orjson | 6.346 | 6.549 | 6.892 | 190.953 | 0.95x |
| wide_arrays.json | msgspec | 10.669 | 10.722 | 10.845 | 190.953 | 0.58x |
| wide_arrays.json | ujson | 31.874 | 31.984 | 32.291 | 190.953 | 0.19x |
| wide_arrays.json | json | 56.507 | 56.879 | 57.159 | 190.953 | 0.11x |
| mixed.json | strata | 0.207 | 0.213 | 0.268 | 176.000 | 1.00x |
| mixed.json | orjson | 0.202 | 0.207 | 0.221 | 176.000 | 1.03x |
| mixed.json | msgspec | 0.225 | 0.232 | 0.251 | 176.000 | 0.92x |
| mixed.json | ujson | 1.083 | 1.093 | 1.105 | 176.000 | 0.19x |
| mixed.json | json | 1.682 | 1.724 | 1.814 | 176.000 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 61.143 | 64.555 | 67.065 | 656.656 | 1.00x |
| users.json | orjson | 79.154 | 81.425 | 85.007 | 656.656 | 0.79x |
| users.json | msgspec | 80.571 | 83.688 | 87.171 | 656.656 | 0.77x |
| users.json | ujson | 105.610 | 108.832 | 114.898 | 656.656 | 0.59x |
| users.json | json | 127.314 | 132.072 | 135.267 | 656.656 | 0.49x |
| flat.json | strata | 3.997 | 4.053 | 4.262 | 167.625 | 1.00x |
| flat.json | orjson | 5.096 | 5.189 | 5.513 | 167.625 | 0.78x |
| flat.json | msgspec | 5.087 | 5.161 | 5.499 | 167.625 | 0.79x |
| flat.json | ujson | 6.959 | 7.119 | 7.421 | 167.625 | 0.57x |
| flat.json | json | 8.779 | 8.892 | 9.424 | 167.625 | 0.46x |
| nested.json | strata | 3.275 | 3.357 | 3.495 | 167.750 | 1.00x |
| nested.json | orjson | 4.425 | 4.576 | 4.826 | 167.750 | 0.73x |
| nested.json | msgspec | 4.496 | 4.597 | 4.940 | 167.750 | 0.73x |
| nested.json | ujson | 5.967 | 6.073 | 6.375 | 167.750 | 0.55x |
| nested.json | json | 8.157 | 8.392 | 8.754 | 167.750 | 0.40x |
| wide_arrays.json | strata | 19.481 | 19.677 | 20.001 | 168.938 | 1.00x |
| wide_arrays.json | orjson | 19.425 | 19.658 | 19.968 | 168.938 | 1.00x |
| wide_arrays.json | msgspec | 23.302 | 23.607 | 24.151 | 168.938 | 0.83x |
| wide_arrays.json | ujson | 30.397 | 30.794 | 31.163 | 168.938 | 0.64x |
| wide_arrays.json | json | 42.098 | 42.394 | 42.602 | 168.938 | 0.46x |
| mixed.json | strata | 0.892 | 0.912 | 0.991 | 176.000 | 1.00x |
| mixed.json | orjson | 1.711 | 1.742 | 1.915 | 176.000 | 0.52x |
| mixed.json | msgspec | 1.762 | 1.794 | 2.001 | 176.000 | 0.51x |
| mixed.json | ujson | 2.008 | 2.034 | 2.208 | 176.000 | 0.45x |
| mixed.json | json | 2.573 | 2.592 | 2.962 | 176.000 | 0.35x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 56.884 | 57.366 | 57.895 | 169.047 | 1.00x |
| users.ndjson | orjson | 80.446 | 81.299 | 82.866 | 169.047 | 0.71x |
| users.ndjson | msgspec | 79.605 | 80.911 | 82.177 | 169.047 | 0.71x |
| users.ndjson | ujson | 102.390 | 103.614 | 104.608 | 169.047 | 0.55x |
| users.ndjson | json | 132.233 | 133.526 | 138.366 | 169.047 | 0.43x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 17.010 | 17.537 | 20.088 | 574.203 | 1.00x |
| users.json | orjson | 17.668 | 18.431 | 21.067 | 574.203 | 0.95x |
| users.json | msgspec | 22.632 | 24.109 | 25.119 | 574.203 | 0.73x |
| users.json | ujson | 84.041 | 86.153 | 87.749 | 574.203 | 0.20x |
| users.json | json | 115.601 | 116.725 | 119.672 | 574.203 | 0.15x |
| flat.json | strata | 1.376 | 1.408 | 1.483 | 167.719 | 1.00x |
| flat.json | orjson | 1.394 | 1.409 | 1.468 | 167.719 | 1.00x |
| flat.json | msgspec | 1.735 | 1.766 | 1.916 | 167.719 | 0.80x |
| flat.json | ujson | 4.769 | 4.833 | 5.098 | 167.719 | 0.29x |
| flat.json | json | 6.729 | 6.834 | 6.904 | 167.719 | 0.21x |
| nested.json | strata | 1.058 | 1.102 | 1.182 | 167.797 | 1.00x |
| nested.json | orjson | 1.236 | 1.261 | 1.354 | 167.797 | 0.87x |
| nested.json | msgspec | 1.587 | 1.635 | 1.732 | 167.797 | 0.67x |
| nested.json | ujson | 5.222 | 5.328 | 5.795 | 167.797 | 0.21x |
| nested.json | json | 8.208 | 8.345 | 8.556 | 167.797 | 0.13x |
| wide_arrays.json | strata | 6.941 | 7.255 | 7.396 | 175.984 | 1.00x |
| wide_arrays.json | orjson | 7.262 | 7.392 | 7.518 | 175.984 | 0.98x |
| wide_arrays.json | msgspec | 11.652 | 11.796 | 12.043 | 175.984 | 0.62x |
| wide_arrays.json | ujson | 33.039 | 33.362 | 33.724 | 175.984 | 0.22x |
| wide_arrays.json | json | 57.774 | 58.129 | 58.745 | 175.984 | 0.12x |
| mixed.json | strata | 0.379 | 0.397 | 0.471 | 176.000 | 1.00x |
| mixed.json | orjson | 0.340 | 0.352 | 0.426 | 176.000 | 1.13x |
| mixed.json | msgspec | 0.370 | 0.391 | 0.420 | 176.000 | 1.02x |
| mixed.json | ujson | 1.248 | 1.272 | 1.363 | 176.000 | 0.31x |
| mixed.json | json | 1.839 | 1.868 | 1.979 | 176.000 | 0.21x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.207 | 0.228 | 0.264 | 574.641 | 1.00x |
| users.json $[*].id | jmespath | 0.988 | 1.044 | 1.220 | 574.641 | 0.22x |
| users.json $[*].id | jsonpath-ng | 14.976 | 15.310 | 16.249 | 574.641 | 0.01x |
| users.json $[*].orders[*].total | strata | 2.674 | 2.752 | 2.947 | 453.969 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 10.390 | 10.744 | 11.572 | 453.969 | 0.26x |
| users.json $[*].orders[*].total | jsonpath-ng | 56.690 | 58.048 | 59.997 | 453.969 | 0.05x |
| users.json $..total | strata | 10.138 | 10.297 | 10.574 | 150.391 | 1.00x |
| users.json $..total | jsonpath-ng | 1623.855 | 1626.326 | 1631.425 | 150.391 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 28.458 | 29.033 | 29.127 | 634.219 | 1.00x |
| users.json $[*].id | orjson+jmespath | 79.693 | 81.170 | 83.259 | 634.219 | 0.36x |
| users.json $[*].id | orjson+jsonpath-ng | 82.783 | 84.987 | 86.046 | 634.219 | 0.34x |
| users.json $[*].orders[*].total | strata | 29.223 | 29.586 | 29.736 | 516.094 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 87.478 | 88.635 | 89.635 | 516.094 | 0.33x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 123.887 | 126.992 | 130.291 | 516.094 | 0.23x |
| users.json $..total | strata | 83.355 | 83.845 | 84.932 | 202.172 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 1687.962 | 1690.565 | 1695.127 | 202.172 | 0.05x |

