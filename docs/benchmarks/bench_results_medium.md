# Benchmark results - medium

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 62edf69
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
| users.json | strata | 60.301 | 62.901 | 66.506 | 139.281 | 1.00x |
| users.json | orjson | 64.612 | 66.774 | 69.852 | 139.281 | 0.94x |
| users.json | msgspec | 65.663 | 67.693 | 71.369 | 139.281 | 0.93x |
| users.json | ujson | 90.238 | 93.305 | 98.476 | 139.281 | 0.67x |
| users.json | json | 114.251 | 117.274 | 120.686 | 139.281 | 0.54x |
| flat.json | strata | 3.933 | 3.996 | 4.203 | 179.922 | 1.00x |
| flat.json | orjson | 4.533 | 4.621 | 4.915 | 179.922 | 0.86x |
| flat.json | msgspec | 4.734 | 4.860 | 5.220 | 179.922 | 0.82x |
| flat.json | ujson | 6.426 | 6.590 | 7.064 | 179.922 | 0.61x |
| flat.json | json | 8.419 | 8.814 | 9.021 | 179.922 | 0.45x |
| nested.json | strata | 3.140 | 3.182 | 3.251 | 179.922 | 1.00x |
| nested.json | orjson | 3.925 | 3.965 | 4.163 | 179.922 | 0.80x |
| nested.json | msgspec | 3.927 | 3.952 | 4.217 | 179.922 | 0.81x |
| nested.json | ujson | 5.321 | 5.353 | 5.836 | 179.922 | 0.59x |
| nested.json | json | 7.703 | 7.807 | 8.037 | 179.922 | 0.41x |
| wide_arrays.json | strata | 18.997 | 19.157 | 19.406 | 207.875 | 1.00x |
| wide_arrays.json | orjson | 17.115 | 17.403 | 17.794 | 207.875 | 1.10x |
| wide_arrays.json | msgspec | 21.046 | 21.203 | 21.562 | 207.875 | 0.90x |
| wide_arrays.json | ujson | 27.268 | 27.484 | 27.891 | 207.875 | 0.70x |
| wide_arrays.json | json | 39.783 | 39.928 | 40.408 | 207.875 | 0.48x |
| mixed.json | strata | 0.819 | 0.829 | 0.855 | 180.516 | 1.00x |
| mixed.json | orjson | 1.531 | 1.566 | 1.906 | 180.516 | 0.53x |
| mixed.json | msgspec | 1.589 | 1.608 | 2.182 | 180.516 | 0.52x |
| mixed.json | ujson | 1.788 | 1.818 | 1.992 | 180.516 | 0.46x |
| mixed.json | json | 2.408 | 2.520 | 2.814 | 180.516 | 0.33x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 18.519 | 18.548 | 18.886 | 425.828 | 1.00x |
| users.json | orjson | 15.877 | 16.161 | 16.392 | 425.828 | 1.15x |
| users.json | msgspec | 20.877 | 21.156 | 21.350 | 425.828 | 0.88x |
| users.json | ujson | 83.933 | 84.158 | 84.756 | 425.828 | 0.22x |
| users.json | json | 115.337 | 116.103 | 117.036 | 425.828 | 0.16x |
| flat.json | strata | 1.347 | 1.364 | 1.387 | 179.922 | 1.00x |
| flat.json | orjson | 1.144 | 1.167 | 1.286 | 179.922 | 1.17x |
| flat.json | msgspec | 1.502 | 1.532 | 1.600 | 179.922 | 0.89x |
| flat.json | ujson | 4.510 | 4.542 | 4.697 | 179.922 | 0.30x |
| flat.json | json | 6.396 | 6.426 | 6.605 | 179.922 | 0.21x |
| nested.json | strata | 1.038 | 1.060 | 1.163 | 179.922 | 1.00x |
| nested.json | orjson | 1.032 | 1.051 | 1.156 | 179.922 | 1.01x |
| nested.json | msgspec | 1.373 | 1.378 | 1.400 | 179.922 | 0.77x |
| nested.json | ujson | 4.973 | 5.065 | 5.254 | 179.922 | 0.21x |
| nested.json | json | 8.109 | 8.210 | 8.514 | 179.922 | 0.13x |
| wide_arrays.json | strata | 8.117 | 8.249 | 8.400 | 203.109 | 1.00x |
| wide_arrays.json | orjson | 6.370 | 6.531 | 6.724 | 203.109 | 1.26x |
| wide_arrays.json | msgspec | 10.780 | 10.887 | 11.198 | 203.109 | 0.76x |
| wide_arrays.json | ujson | 32.803 | 33.065 | 33.630 | 203.109 | 0.25x |
| wide_arrays.json | json | 56.515 | 57.195 | 57.637 | 203.109 | 0.14x |
| mixed.json | strata | 0.252 | 0.254 | 0.280 | 180.516 | 1.00x |
| mixed.json | orjson | 0.201 | 0.204 | 0.267 | 180.516 | 1.24x |
| mixed.json | msgspec | 0.224 | 0.227 | 0.252 | 180.516 | 1.12x |
| mixed.json | ujson | 1.081 | 1.090 | 1.122 | 180.516 | 0.23x |
| mixed.json | json | 1.686 | 1.702 | 1.753 | 180.516 | 0.15x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 62.131 | 64.692 | 68.038 | 483.250 | 1.00x |
| users.json | orjson | 79.095 | 81.977 | 85.078 | 483.250 | 0.79x |
| users.json | msgspec | 80.478 | 84.988 | 89.490 | 483.250 | 0.76x |
| users.json | ujson | 105.870 | 110.711 | 113.496 | 483.250 | 0.58x |
| users.json | json | 129.977 | 133.032 | 136.479 | 483.250 | 0.49x |
| flat.json | strata | 4.111 | 4.154 | 4.205 | 179.922 | 1.00x |
| flat.json | orjson | 5.020 | 5.091 | 5.357 | 179.922 | 0.82x |
| flat.json | msgspec | 5.209 | 5.264 | 5.730 | 179.922 | 0.79x |
| flat.json | ujson | 7.052 | 7.165 | 7.487 | 179.922 | 0.58x |
| flat.json | json | 8.898 | 8.966 | 9.283 | 179.922 | 0.46x |
| nested.json | strata | 3.279 | 3.332 | 3.525 | 180.516 | 1.00x |
| nested.json | orjson | 4.469 | 4.567 | 4.924 | 180.516 | 0.73x |
| nested.json | msgspec | 4.494 | 4.541 | 5.147 | 180.516 | 0.73x |
| nested.json | ujson | 5.992 | 6.048 | 6.595 | 180.516 | 0.55x |
| nested.json | json | 8.249 | 8.364 | 8.799 | 180.516 | 0.40x |
| wide_arrays.json | strata | 19.486 | 19.635 | 20.068 | 177.125 | 1.00x |
| wide_arrays.json | orjson | 19.538 | 19.739 | 21.849 | 177.125 | 0.99x |
| wide_arrays.json | msgspec | 23.664 | 23.936 | 24.263 | 177.125 | 0.82x |
| wide_arrays.json | ujson | 30.516 | 30.993 | 32.241 | 177.125 | 0.63x |
| wide_arrays.json | json | 42.398 | 42.813 | 43.495 | 177.125 | 0.46x |
| mixed.json | strata | 0.893 | 0.923 | 0.970 | 180.516 | 1.00x |
| mixed.json | orjson | 1.738 | 1.770 | 1.863 | 180.516 | 0.52x |
| mixed.json | msgspec | 1.784 | 1.847 | 2.309 | 180.516 | 0.50x |
| mixed.json | ujson | 2.032 | 2.064 | 2.190 | 180.516 | 0.45x |
| mixed.json | json | 2.576 | 2.646 | 2.925 | 180.516 | 0.35x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 58.024 | 58.586 | 58.914 | 182.688 | 1.00x |
| users.ndjson | orjson | 81.502 | 83.823 | 84.642 | 182.688 | 0.70x |
| users.ndjson | msgspec | 81.083 | 82.295 | 82.914 | 182.688 | 0.71x |
| users.ndjson | ujson | 104.136 | 105.444 | 105.891 | 182.688 | 0.56x |
| users.ndjson | json | 134.221 | 136.065 | 138.407 | 182.688 | 0.43x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 20.544 | 21.704 | 23.143 | 608.047 | 1.00x |
| users.json | orjson | 18.236 | 18.992 | 19.540 | 608.047 | 1.14x |
| users.json | msgspec | 23.001 | 24.093 | 24.578 | 608.047 | 0.90x |
| users.json | ujson | 85.000 | 86.792 | 88.239 | 608.047 | 0.25x |
| users.json | json | 116.613 | 117.076 | 117.439 | 608.047 | 0.19x |
| flat.json | strata | 1.629 | 1.720 | 1.813 | 179.922 | 1.00x |
| flat.json | orjson | 1.390 | 1.450 | 1.589 | 179.922 | 1.19x |
| flat.json | msgspec | 1.765 | 1.804 | 2.074 | 179.922 | 0.95x |
| flat.json | ujson | 4.767 | 4.843 | 4.892 | 179.922 | 0.36x |
| flat.json | json | 6.713 | 6.775 | 6.886 | 179.922 | 0.25x |
| nested.json | strata | 1.319 | 1.393 | 1.466 | 180.625 | 1.00x |
| nested.json | orjson | 1.260 | 1.296 | 1.383 | 180.625 | 1.08x |
| nested.json | msgspec | 1.593 | 1.630 | 1.698 | 180.625 | 0.85x |
| nested.json | ujson | 5.217 | 5.305 | 5.416 | 180.625 | 0.26x |
| nested.json | json | 8.371 | 8.487 | 12.267 | 180.625 | 0.16x |
| wide_arrays.json | strata | 9.232 | 9.319 | 9.713 | 180.516 | 1.00x |
| wide_arrays.json | orjson | 7.227 | 7.377 | 8.069 | 180.516 | 1.26x |
| wide_arrays.json | msgspec | 11.645 | 11.861 | 12.107 | 180.516 | 0.79x |
| wide_arrays.json | ujson | 33.824 | 34.178 | 34.568 | 180.516 | 0.27x |
| wide_arrays.json | json | 57.609 | 58.243 | 58.717 | 180.516 | 0.16x |
| mixed.json | strata | 0.413 | 0.432 | 0.515 | 180.516 | 1.00x |
| mixed.json | orjson | 0.332 | 0.360 | 0.388 | 180.516 | 1.20x |
| mixed.json | msgspec | 0.364 | 0.387 | 0.508 | 180.516 | 1.11x |
| mixed.json | ujson | 1.251 | 1.295 | 1.373 | 180.516 | 0.33x |
| mixed.json | json | 1.837 | 1.863 | 2.056 | 180.516 | 0.23x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.197 | 0.213 | 0.244 | 608.531 | 1.00x |
| users.json $[*].id | jmespath | 0.976 | 1.001 | 1.117 | 608.531 | 0.21x |
| users.json $[*].id | jsonpath-ng | 14.864 | 15.028 | 15.893 | 608.531 | 0.01x |
| users.json $[*].orders[*].total | strata | 2.633 | 2.743 | 2.831 | 480.891 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 10.191 | 10.412 | 10.966 | 480.891 | 0.26x |
| users.json $[*].orders[*].total | jsonpath-ng | 56.021 | 57.504 | 59.159 | 480.891 | 0.05x |
| users.json $..total | strata | 10.126 | 10.181 | 10.350 | 163.141 | 1.00x |
| users.json $..total | jsonpath-ng | 1625.565 | 1627.828 | 1631.995 | 163.141 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 28.205 | 28.670 | 28.871 | 675.578 | 1.00x |
| users.json $[*].id | orjson+jmespath | 80.256 | 81.902 | 82.890 | 675.578 | 0.35x |
| users.json $[*].id | orjson+jsonpath-ng | 83.857 | 85.622 | 86.715 | 675.578 | 0.33x |
| users.json $[*].orders[*].total | strata | 28.798 | 29.082 | 29.329 | 513.094 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 88.615 | 90.519 | 92.660 | 513.094 | 0.32x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 125.448 | 128.121 | 132.142 | 513.094 | 0.23x |
| users.json $..total | strata | 84.746 | 85.189 | 85.906 | 214.922 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 1690.733 | 1692.235 | 1696.188 | 214.922 | 0.05x |

