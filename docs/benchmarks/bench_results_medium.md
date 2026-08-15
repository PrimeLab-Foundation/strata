# Benchmark results - medium

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c36c406
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
| users.json | strata | 41.236 | 42.744 | 44.774 | 139.547 | 1.00x |
| users.json | orjson | 50.015 | 51.933 | 52.942 | 139.547 | 0.82x |
| users.json | msgspec | 51.273 | 52.654 | 55.487 | 139.547 | 0.81x |
| users.json | ujson | 69.776 | 72.678 | 74.386 | 139.547 | 0.59x |
| users.json | json | 88.157 | 90.063 | 92.837 | 139.547 | 0.47x |
| flat.json | strata | 2.435 | 2.455 | 2.487 | 162.875 | 1.00x |
| flat.json | orjson | 3.487 | 3.506 | 3.768 | 162.875 | 0.70x |
| flat.json | msgspec | 3.532 | 3.556 | 3.687 | 162.875 | 0.69x |
| flat.json | ujson | 4.847 | 4.905 | 4.989 | 162.875 | 0.50x |
| flat.json | json | 6.288 | 6.316 | 6.801 | 162.875 | 0.39x |
| nested.json | strata | 2.092 | 2.100 | 2.119 | 163.734 | 1.00x |
| nested.json | orjson | 3.040 | 3.048 | 3.101 | 163.734 | 0.69x |
| nested.json | msgspec | 3.037 | 3.068 | 3.115 | 163.734 | 0.68x |
| nested.json | ujson | 4.102 | 4.122 | 4.233 | 163.734 | 0.51x |
| nested.json | json | 5.853 | 5.895 | 5.932 | 163.734 | 0.36x |
| wide_arrays.json | strata | 13.235 | 13.395 | 13.563 | 191.578 | 1.00x |
| wide_arrays.json | orjson | 13.249 | 13.453 | 13.705 | 191.578 | 1.00x |
| wide_arrays.json | msgspec | 16.121 | 16.375 | 16.795 | 191.578 | 0.82x |
| wide_arrays.json | ujson | 20.812 | 21.151 | 21.390 | 191.578 | 0.63x |
| wide_arrays.json | json | 30.149 | 30.306 | 30.755 | 191.578 | 0.44x |
| mixed.json | strata | 0.505 | 0.512 | 0.567 | 203.484 | 1.00x |
| mixed.json | orjson | 1.167 | 1.177 | 1.220 | 203.484 | 0.43x |
| mixed.json | msgspec | 1.205 | 1.218 | 1.361 | 203.484 | 0.42x |
| mixed.json | ujson | 1.373 | 1.396 | 1.632 | 203.484 | 0.37x |
| mixed.json | json | 1.828 | 1.852 | 1.942 | 203.484 | 0.28x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.216 | 10.414 | 10.973 | 602.031 | 1.00x |
| users.json | orjson | 12.135 | 12.295 | 12.665 | 602.031 | 0.85x |
| users.json | msgspec | 15.849 | 16.027 | 16.355 | 602.031 | 0.65x |
| users.json | ujson | 63.933 | 64.269 | 66.058 | 602.031 | 0.16x |
| users.json | json | 88.075 | 88.467 | 89.275 | 602.031 | 0.12x |
| flat.json | strata | 0.711 | 0.716 | 0.778 | 162.875 | 1.00x |
| flat.json | orjson | 0.850 | 0.864 | 0.944 | 162.875 | 0.83x |
| flat.json | msgspec | 1.117 | 1.123 | 1.253 | 162.875 | 0.64x |
| flat.json | ujson | 3.502 | 3.521 | 3.664 | 162.875 | 0.20x |
| flat.json | json | 4.867 | 4.881 | 5.072 | 162.875 | 0.15x |
| nested.json | strata | 0.567 | 0.571 | 0.579 | 163.734 | 1.00x |
| nested.json | orjson | 0.767 | 0.771 | 0.787 | 163.734 | 0.74x |
| nested.json | msgspec | 1.025 | 1.032 | 1.052 | 163.734 | 0.55x |
| nested.json | ujson | 3.752 | 3.763 | 3.779 | 163.734 | 0.15x |
| nested.json | json | 6.027 | 6.078 | 6.127 | 163.734 | 0.09x |
| wide_arrays.json | strata | 4.516 | 4.740 | 4.930 | 187.484 | 1.00x |
| wide_arrays.json | orjson | 4.907 | 5.017 | 5.363 | 187.484 | 0.94x |
| wide_arrays.json | msgspec | 8.125 | 8.303 | 8.660 | 187.484 | 0.57x |
| wide_arrays.json | ujson | 24.199 | 24.435 | 24.788 | 187.484 | 0.19x |
| wide_arrays.json | json | 42.994 | 43.384 | 43.752 | 187.484 | 0.11x |
| mixed.json | strata | 0.143 | 0.146 | 0.149 | 203.500 | 1.00x |
| mixed.json | orjson | 0.152 | 0.156 | 0.162 | 203.500 | 0.93x |
| mixed.json | msgspec | 0.171 | 0.172 | 0.177 | 203.500 | 0.85x |
| mixed.json | ujson | 0.827 | 0.830 | 0.852 | 203.500 | 0.18x |
| mixed.json | json | 1.275 | 1.296 | 1.332 | 203.500 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 42.210 | 44.687 | 47.513 | 657.125 | 1.00x |
| users.json | orjson | 64.721 | 67.246 | 69.909 | 657.125 | 0.66x |
| users.json | msgspec | 65.956 | 68.830 | 70.589 | 657.125 | 0.65x |
| users.json | ujson | 85.994 | 88.920 | 93.404 | 657.125 | 0.50x |
| users.json | json | 101.691 | 104.762 | 110.060 | 657.125 | 0.43x |
| flat.json | strata | 2.590 | 2.608 | 2.696 | 163.719 | 1.00x |
| flat.json | orjson | 3.848 | 3.892 | 4.053 | 163.719 | 0.67x |
| flat.json | msgspec | 3.949 | 3.983 | 4.042 | 163.719 | 0.65x |
| flat.json | ujson | 5.333 | 5.381 | 5.569 | 163.719 | 0.48x |
| flat.json | json | 6.667 | 6.709 | 6.944 | 163.719 | 0.39x |
| nested.json | strata | 2.225 | 2.244 | 2.387 | 164.297 | 1.00x |
| nested.json | orjson | 3.503 | 3.536 | 3.613 | 164.297 | 0.63x |
| nested.json | msgspec | 3.548 | 3.579 | 3.696 | 164.297 | 0.63x |
| nested.json | ujson | 4.679 | 4.747 | 5.119 | 164.297 | 0.47x |
| nested.json | json | 6.364 | 6.396 | 6.586 | 164.297 | 0.35x |
| wide_arrays.json | strata | 13.741 | 13.838 | 14.062 | 196.438 | 1.00x |
| wide_arrays.json | orjson | 15.284 | 15.410 | 15.732 | 196.438 | 0.90x |
| wide_arrays.json | msgspec | 18.363 | 18.577 | 19.133 | 196.438 | 0.74x |
| wide_arrays.json | ujson | 23.688 | 24.008 | 24.453 | 196.438 | 0.58x |
| wide_arrays.json | json | 32.378 | 32.814 | 33.502 | 196.438 | 0.42x |
| mixed.json | strata | 0.569 | 0.579 | 1.260 | 162.875 | 1.00x |
| mixed.json | orjson | 1.327 | 1.355 | 1.376 | 162.875 | 0.43x |
| mixed.json | msgspec | 1.369 | 1.398 | 1.526 | 162.875 | 0.41x |
| mixed.json | ujson | 1.579 | 1.613 | 1.831 | 162.875 | 0.36x |
| mixed.json | json | 1.981 | 2.018 | 2.171 | 162.875 | 0.29x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 42.059 | 42.438 | 43.145 | 165.641 | 1.00x |
| users.ndjson | orjson | 63.370 | 64.516 | 66.690 | 165.641 | 0.66x |
| users.ndjson | msgspec | 62.792 | 63.707 | 64.974 | 165.641 | 0.67x |
| users.ndjson | ujson | 80.599 | 82.777 | 83.953 | 165.641 | 0.51x |
| users.ndjson | json | 103.455 | 103.982 | 106.674 | 165.641 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.642 | 12.335 | 13.544 | 673.281 | 1.00x |
| users.json | orjson | 13.874 | 14.334 | 15.567 | 673.281 | 0.86x |
| users.json | msgspec | 17.551 | 18.279 | 19.358 | 673.281 | 0.67x |
| users.json | ujson | 64.875 | 65.509 | 68.103 | 673.281 | 0.19x |
| users.json | json | 87.751 | 89.024 | 89.588 | 673.281 | 0.14x |
| flat.json | strata | 0.922 | 0.964 | 0.980 | 163.734 | 1.00x |
| flat.json | orjson | 1.073 | 1.091 | 1.121 | 163.734 | 0.88x |
| flat.json | msgspec | 1.348 | 1.365 | 1.385 | 163.734 | 0.71x |
| flat.json | ujson | 3.761 | 3.786 | 3.840 | 163.734 | 0.25x |
| flat.json | json | 5.114 | 5.155 | 5.228 | 163.734 | 0.19x |
| nested.json | strata | 0.777 | 0.806 | 0.883 | 164.328 | 1.00x |
| nested.json | orjson | 0.976 | 1.028 | 1.070 | 164.328 | 0.78x |
| nested.json | msgspec | 1.248 | 1.284 | 1.330 | 164.328 | 0.63x |
| nested.json | ujson | 4.017 | 4.091 | 4.215 | 164.328 | 0.20x |
| nested.json | json | 6.334 | 6.403 | 6.634 | 164.328 | 0.13x |
| wide_arrays.json | strata | 5.466 | 5.628 | 5.993 | 203.484 | 1.00x |
| wide_arrays.json | orjson | 5.758 | 5.865 | 5.967 | 203.484 | 0.96x |
| wide_arrays.json | msgspec | 9.059 | 9.161 | 9.305 | 203.484 | 0.61x |
| wide_arrays.json | ujson | 25.237 | 25.470 | 25.868 | 203.484 | 0.22x |
| wide_arrays.json | json | 44.233 | 44.525 | 44.622 | 203.484 | 0.13x |
| mixed.json | strata | 0.273 | 0.283 | 0.341 | 162.875 | 1.00x |
| mixed.json | orjson | 0.287 | 0.296 | 0.362 | 162.875 | 0.96x |
| mixed.json | msgspec | 0.303 | 0.317 | 0.358 | 162.875 | 0.89x |
| mixed.json | ujson | 0.976 | 0.997 | 1.106 | 162.875 | 0.28x |
| mixed.json | json | 1.416 | 1.441 | 1.512 | 162.875 | 0.20x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.172 | 0.183 | 0.197 | 673.719 | 1.00x |
| users.json $[*].id | jmespath | 0.789 | 0.812 | 0.870 | 673.719 | 0.23x |
| users.json $[*].id | jsonpath-ng | 12.568 | 12.692 | 13.440 | 673.719 | 0.01x |
| users.json $[*].orders[*].total | strata | 2.436 | 2.486 | 2.623 | 676.953 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 8.066 | 8.234 | 8.456 | 676.953 | 0.30x |
| users.json $[*].orders[*].total | jsonpath-ng | 44.467 | 45.646 | 46.475 | 676.953 | 0.05x |
| users.json $..total | strata | 7.837 | 7.907 | 8.049 | 257.891 | 1.00x |
| users.json $..total | jsonpath-ng | 1240.832 | 1242.056 | 1244.044 | 257.891 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 18.491 | 18.579 | 19.073 | 693.797 | 1.00x |
| users.json $[*].id | orjson+jmespath | 65.827 | 67.171 | 68.349 | 693.797 | 0.28x |
| users.json $[*].id | orjson+jsonpath-ng | 67.863 | 69.289 | 70.609 | 693.797 | 0.27x |
| users.json $[*].orders[*].total | strata | 18.886 | 19.063 | 19.361 | 708.250 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 71.747 | 72.803 | 75.375 | 708.250 | 0.26x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 100.220 | 101.702 | 103.491 | 708.250 | 0.19x |
| users.json $..total | strata | 61.125 | 62.143 | 62.865 | 202.891 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 1292.120 | 1295.730 | 1301.873 | 202.891 | 0.05x |

