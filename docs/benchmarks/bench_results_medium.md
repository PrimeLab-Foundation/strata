# Benchmark results - medium

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 1698e08
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
| users.json | strata | 61.095 | 63.716 | 66.182 | 139.203 | 1.00x |
| users.json | orjson | 63.960 | 66.964 | 70.909 | 139.203 | 0.95x |
| users.json | msgspec | 65.368 | 68.879 | 73.546 | 139.203 | 0.93x |
| users.json | ujson | 89.841 | 93.419 | 102.685 | 139.203 | 0.68x |
| users.json | json | 114.128 | 118.354 | 123.534 | 139.203 | 0.54x |
| flat.json | strata | 3.934 | 3.943 | 4.032 | 178.094 | 1.00x |
| flat.json | orjson | 4.530 | 4.563 | 4.702 | 178.094 | 0.86x |
| flat.json | msgspec | 4.656 | 4.736 | 4.879 | 178.094 | 0.83x |
| flat.json | ujson | 6.427 | 6.481 | 6.555 | 178.094 | 0.61x |
| flat.json | json | 8.338 | 8.388 | 8.621 | 178.094 | 0.47x |
| nested.json | strata | 3.141 | 3.171 | 3.271 | 179.031 | 1.00x |
| nested.json | orjson | 3.953 | 3.999 | 4.104 | 179.031 | 0.79x |
| nested.json | msgspec | 3.956 | 3.988 | 4.048 | 179.031 | 0.80x |
| nested.json | ujson | 5.333 | 5.448 | 5.799 | 179.031 | 0.58x |
| nested.json | json | 7.699 | 7.786 | 8.528 | 179.031 | 0.41x |
| wide_arrays.json | strata | 18.974 | 19.301 | 20.597 | 206.406 | 1.00x |
| wide_arrays.json | orjson | 17.001 | 17.191 | 18.048 | 206.406 | 1.12x |
| wide_arrays.json | msgspec | 21.162 | 21.459 | 22.174 | 206.406 | 0.90x |
| wide_arrays.json | ujson | 27.284 | 27.666 | 28.327 | 206.406 | 0.70x |
| wide_arrays.json | json | 39.616 | 40.024 | 41.014 | 206.406 | 0.48x |
| mixed.json | strata | 0.831 | 0.844 | 1.012 | 178.781 | 1.00x |
| mixed.json | orjson | 1.535 | 1.616 | 1.922 | 178.781 | 0.52x |
| mixed.json | msgspec | 1.574 | 1.614 | 1.744 | 178.781 | 0.52x |
| mixed.json | ujson | 1.793 | 1.852 | 1.990 | 178.781 | 0.46x |
| mixed.json | json | 2.392 | 2.475 | 2.672 | 178.781 | 0.34x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 18.168 | 18.338 | 18.572 | 425.781 | 1.00x |
| users.json | orjson | 15.956 | 16.144 | 16.504 | 425.781 | 1.14x |
| users.json | msgspec | 20.894 | 21.236 | 21.543 | 425.781 | 0.86x |
| users.json | ujson | 83.410 | 83.993 | 84.077 | 425.781 | 0.22x |
| users.json | json | 115.494 | 116.103 | 116.649 | 425.781 | 0.16x |
| flat.json | strata | 1.338 | 1.358 | 1.597 | 178.188 | 1.00x |
| flat.json | orjson | 1.126 | 1.150 | 1.201 | 178.188 | 1.18x |
| flat.json | msgspec | 1.501 | 1.510 | 1.588 | 178.188 | 0.90x |
| flat.json | ujson | 4.639 | 4.672 | 4.685 | 178.188 | 0.29x |
| flat.json | json | 6.418 | 6.501 | 6.796 | 178.188 | 0.21x |
| nested.json | strata | 1.061 | 1.078 | 1.136 | 179.047 | 1.00x |
| nested.json | orjson | 1.030 | 1.041 | 1.103 | 179.047 | 1.04x |
| nested.json | msgspec | 1.368 | 1.384 | 1.405 | 179.047 | 0.78x |
| nested.json | ujson | 4.989 | 5.054 | 5.147 | 179.047 | 0.21x |
| nested.json | json | 8.166 | 8.241 | 8.623 | 179.047 | 0.13x |
| wide_arrays.json | strata | 8.145 | 8.185 | 8.431 | 201.516 | 1.00x |
| wide_arrays.json | orjson | 6.282 | 6.431 | 6.921 | 201.516 | 1.27x |
| wide_arrays.json | msgspec | 10.671 | 10.889 | 11.244 | 201.516 | 0.75x |
| wide_arrays.json | ujson | 32.587 | 32.898 | 33.877 | 201.516 | 0.25x |
| wide_arrays.json | json | 56.338 | 56.894 | 57.419 | 201.516 | 0.14x |
| mixed.json | strata | 0.251 | 0.257 | 0.268 | 178.781 | 1.00x |
| mixed.json | orjson | 0.200 | 0.203 | 0.211 | 178.781 | 1.26x |
| mixed.json | msgspec | 0.225 | 0.228 | 0.230 | 178.781 | 1.13x |
| mixed.json | ujson | 1.084 | 1.092 | 1.113 | 178.781 | 0.24x |
| mixed.json | json | 1.660 | 1.674 | 1.727 | 178.781 | 0.15x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 62.138 | 64.085 | 67.052 | 483.219 | 1.00x |
| users.json | orjson | 79.462 | 80.975 | 85.779 | 483.219 | 0.79x |
| users.json | msgspec | 81.133 | 84.597 | 87.761 | 483.219 | 0.76x |
| users.json | ujson | 105.749 | 110.363 | 112.455 | 483.219 | 0.58x |
| users.json | json | 129.906 | 132.527 | 135.091 | 483.219 | 0.48x |
| flat.json | strata | 4.127 | 4.163 | 4.389 | 179.016 | 1.00x |
| flat.json | orjson | 5.063 | 5.098 | 5.355 | 179.016 | 0.82x |
| flat.json | msgspec | 5.198 | 5.266 | 5.653 | 179.016 | 0.79x |
| flat.json | ujson | 7.097 | 7.214 | 7.770 | 179.016 | 0.58x |
| flat.json | json | 8.778 | 8.878 | 9.213 | 179.016 | 0.47x |
| nested.json | strata | 3.265 | 3.304 | 3.477 | 179.047 | 1.00x |
| nested.json | orjson | 4.452 | 4.534 | 4.989 | 179.047 | 0.73x |
| nested.json | msgspec | 4.481 | 4.572 | 4.897 | 179.047 | 0.72x |
| nested.json | ujson | 5.980 | 6.099 | 6.376 | 179.047 | 0.54x |
| nested.json | json | 8.215 | 8.300 | 8.639 | 179.047 | 0.40x |
| wide_arrays.json | strata | 19.512 | 19.621 | 20.004 | 175.516 | 1.00x |
| wide_arrays.json | orjson | 19.530 | 19.662 | 20.885 | 175.516 | 1.00x |
| wide_arrays.json | msgspec | 23.867 | 24.112 | 24.553 | 175.516 | 0.81x |
| wide_arrays.json | ujson | 30.549 | 30.949 | 31.311 | 175.516 | 0.63x |
| wide_arrays.json | json | 42.477 | 42.852 | 43.384 | 175.516 | 0.46x |
| mixed.json | strata | 0.894 | 0.916 | 0.954 | 178.781 | 1.00x |
| mixed.json | orjson | 1.729 | 1.774 | 2.098 | 178.781 | 0.52x |
| mixed.json | msgspec | 1.793 | 1.829 | 2.021 | 178.781 | 0.50x |
| mixed.json | ujson | 2.037 | 2.109 | 2.231 | 178.781 | 0.43x |
| mixed.json | json | 2.595 | 2.656 | 2.838 | 178.781 | 0.35x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 58.091 | 58.718 | 59.155 | 180.859 | 1.00x |
| users.ndjson | orjson | 82.414 | 82.750 | 84.273 | 180.859 | 0.71x |
| users.ndjson | msgspec | 81.672 | 82.641 | 83.801 | 180.859 | 0.71x |
| users.ndjson | ujson | 103.496 | 105.268 | 107.597 | 180.859 | 0.56x |
| users.ndjson | json | 134.671 | 137.189 | 139.363 | 180.859 | 0.43x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 20.136 | 21.094 | 21.699 | 608.094 | 1.00x |
| users.json | orjson | 17.920 | 18.594 | 20.417 | 608.094 | 1.13x |
| users.json | msgspec | 22.730 | 23.818 | 24.079 | 608.094 | 0.89x |
| users.json | ujson | 84.446 | 86.703 | 89.604 | 608.094 | 0.24x |
| users.json | json | 117.624 | 118.333 | 119.645 | 608.094 | 0.18x |
| flat.json | strata | 1.628 | 1.653 | 1.782 | 179.031 | 1.00x |
| flat.json | orjson | 1.373 | 1.413 | 1.467 | 179.031 | 1.17x |
| flat.json | msgspec | 1.737 | 1.770 | 1.860 | 179.031 | 0.93x |
| flat.json | ujson | 4.776 | 4.798 | 5.043 | 179.031 | 0.34x |
| flat.json | json | 6.704 | 6.802 | 7.100 | 179.031 | 0.24x |
| nested.json | strata | 1.331 | 1.373 | 1.456 | 179.156 | 1.00x |
| nested.json | orjson | 1.260 | 1.294 | 1.432 | 179.156 | 1.06x |
| nested.json | msgspec | 1.585 | 1.622 | 1.672 | 179.156 | 0.85x |
| nested.json | ujson | 5.278 | 5.497 | 5.772 | 179.156 | 0.25x |
| nested.json | json | 8.428 | 8.495 | 8.687 | 179.156 | 0.16x |
| wide_arrays.json | strata | 9.184 | 9.327 | 9.465 | 178.781 | 1.00x |
| wide_arrays.json | orjson | 7.212 | 7.353 | 7.516 | 178.781 | 1.27x |
| wide_arrays.json | msgspec | 11.727 | 11.809 | 12.257 | 178.781 | 0.79x |
| wide_arrays.json | ujson | 33.768 | 34.077 | 34.978 | 178.781 | 0.27x |
| wide_arrays.json | json | 57.684 | 57.902 | 58.321 | 178.781 | 0.16x |
| mixed.json | strata | 0.418 | 0.455 | 0.513 | 178.781 | 1.00x |
| mixed.json | orjson | 0.339 | 0.365 | 0.463 | 178.781 | 1.25x |
| mixed.json | msgspec | 0.364 | 0.389 | 0.433 | 178.781 | 1.17x |
| mixed.json | ujson | 1.256 | 1.273 | 1.374 | 178.781 | 0.36x |
| mixed.json | json | 1.834 | 1.877 | 2.067 | 178.781 | 0.24x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.202 | 0.214 | 0.297 | 608.578 | 1.00x |
| users.json $[*].id | jmespath | 1.009 | 1.070 | 1.173 | 608.578 | 0.20x |
| users.json $[*].id | jsonpath-ng | 14.882 | 15.317 | 16.815 | 608.578 | 0.01x |
| users.json $[*].orders[*].total | strata | 2.661 | 2.867 | 3.348 | 479.922 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 10.303 | 10.414 | 11.199 | 479.922 | 0.28x |
| users.json $[*].orders[*].total | jsonpath-ng | 56.961 | 58.825 | 61.671 | 479.922 | 0.05x |
| users.json $..total | strata | 10.127 | 10.210 | 10.477 | 164.266 | 1.00x |
| users.json $..total | jsonpath-ng | 1629.882 | 1631.477 | 1635.607 | 164.266 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 28.837 | 28.988 | 29.131 | 675.641 | 1.00x |
| users.json $[*].id | orjson+jmespath | 81.049 | 81.916 | 83.740 | 675.641 | 0.35x |
| users.json $[*].id | orjson+jsonpath-ng | 84.683 | 85.392 | 88.788 | 675.641 | 0.34x |
| users.json $[*].orders[*].total | strata | 29.043 | 29.466 | 29.624 | 513.141 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 89.361 | 90.238 | 92.495 | 513.141 | 0.33x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 125.983 | 128.707 | 131.719 | 513.141 | 0.23x |
| users.json $..total | strata | 84.651 | 85.324 | 85.862 | 215.000 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 1692.320 | 1695.372 | 1698.601 | 215.000 | 0.05x |

