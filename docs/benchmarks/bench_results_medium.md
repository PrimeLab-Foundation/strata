# Benchmark results - medium

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 90dd3b7
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
| users.json | strata | 36.792 | 38.718 | 44.270 | 139.438 | 1.00x |
| users.json | orjson | 52.295 | 53.677 | 57.044 | 139.438 | 0.72x |
| users.json | msgspec | 53.521 | 55.619 | 59.380 | 139.438 | 0.70x |
| users.json | ujson | 74.532 | 76.331 | 84.096 | 139.438 | 0.51x |
| users.json | json | 104.035 | 110.093 | 118.439 | 139.438 | 0.35x |
| flat.json | strata | 2.324 | 2.490 | 2.706 | 169.969 | 1.00x |
| flat.json | orjson | 2.860 | 3.105 | 3.230 | 169.969 | 0.80x |
| flat.json | msgspec | 2.962 | 3.095 | 3.380 | 169.969 | 0.80x |
| flat.json | ujson | 4.320 | 4.579 | 4.645 | 169.969 | 0.54x |
| flat.json | json | 6.066 | 6.392 | 6.635 | 169.969 | 0.39x |
| nested.json | strata | 1.961 | 2.045 | 2.322 | 171.656 | 1.00x |
| nested.json | orjson | 2.415 | 2.586 | 3.004 | 171.656 | 0.79x |
| nested.json | msgspec | 2.457 | 2.606 | 2.760 | 171.656 | 0.78x |
| nested.json | ujson | 3.660 | 3.867 | 4.301 | 171.656 | 0.53x |
| nested.json | json | 5.810 | 5.999 | 6.266 | 171.656 | 0.34x |
| wide_arrays.json | strata | 12.182 | 12.325 | 12.769 | 199.094 | 1.00x |
| wide_arrays.json | orjson | 14.101 | 14.249 | 14.503 | 199.094 | 0.86x |
| wide_arrays.json | msgspec | 17.144 | 17.396 | 17.717 | 199.094 | 0.71x |
| wide_arrays.json | ujson | 22.136 | 22.491 | 22.674 | 199.094 | 0.55x |
| wide_arrays.json | json | 31.943 | 32.323 | 32.670 | 199.094 | 0.38x |
| mixed.json | strata | 0.461 | 0.482 | 0.520 | 166.062 | 1.00x |
| mixed.json | orjson | 0.594 | 0.613 | 0.682 | 166.062 | 0.79x |
| mixed.json | msgspec | 0.655 | 0.684 | 0.707 | 166.062 | 0.70x |
| mixed.json | ujson | 0.802 | 0.868 | 0.903 | 166.062 | 0.56x |
| mixed.json | json | 1.362 | 1.412 | 1.513 | 166.062 | 0.34x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.950 | 8.089 | 8.437 | 604.438 | 1.00x |
| users.json | orjson | 12.452 | 12.564 | 13.758 | 604.438 | 0.64x |
| users.json | msgspec | 16.282 | 16.467 | 16.973 | 604.438 | 0.49x |
| users.json | ujson | 62.682 | 63.598 | 65.555 | 604.438 | 0.13x |
| users.json | json | 90.945 | 91.803 | 93.981 | 604.438 | 0.09x |
| flat.json | strata | 0.758 | 0.766 | 0.923 | 170.000 | 1.00x |
| flat.json | orjson | 0.868 | 0.928 | 1.038 | 170.000 | 0.83x |
| flat.json | msgspec | 1.157 | 1.182 | 1.344 | 170.000 | 0.65x |
| flat.json | ujson | 3.481 | 3.614 | 3.761 | 170.000 | 0.21x |
| flat.json | json | 5.311 | 5.691 | 6.011 | 170.000 | 0.13x |
| nested.json | strata | 0.506 | 0.519 | 0.624 | 171.734 | 1.00x |
| nested.json | orjson | 0.799 | 0.843 | 0.888 | 171.734 | 0.62x |
| nested.json | msgspec | 1.073 | 1.095 | 1.214 | 171.734 | 0.47x |
| nested.json | ujson | 3.917 | 4.039 | 4.132 | 171.734 | 0.13x |
| nested.json | json | 6.520 | 6.816 | 7.230 | 171.734 | 0.08x |
| wide_arrays.json | strata | 4.305 | 4.607 | 4.986 | 157.844 | 1.00x |
| wide_arrays.json | orjson | 5.116 | 5.382 | 5.540 | 157.844 | 0.86x |
| wide_arrays.json | msgspec | 8.590 | 9.011 | 9.828 | 157.844 | 0.51x |
| wide_arrays.json | ujson | 25.392 | 26.172 | 26.873 | 157.844 | 0.18x |
| wide_arrays.json | json | 45.269 | 46.514 | 47.928 | 157.844 | 0.10x |
| mixed.json | strata | 0.129 | 0.132 | 0.141 | 166.109 | 1.00x |
| mixed.json | orjson | 0.157 | 0.176 | 0.208 | 166.109 | 0.75x |
| mixed.json | msgspec | 0.177 | 0.186 | 0.209 | 166.109 | 0.71x |
| mixed.json | ujson | 0.854 | 0.884 | 0.965 | 166.109 | 0.15x |
| mixed.json | json | 1.360 | 1.564 | 1.773 | 166.109 | 0.08x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 37.542 | 39.272 | 41.346 | 657.391 | 1.00x |
| users.json | orjson | 52.674 | 54.507 | 58.093 | 657.391 | 0.72x |
| users.json | msgspec | 54.397 | 56.627 | 58.025 | 657.391 | 0.69x |
| users.json | ujson | 75.615 | 78.063 | 85.428 | 657.391 | 0.50x |
| users.json | json | 104.881 | 109.076 | 117.977 | 657.391 | 0.36x |
| flat.json | strata | 2.552 | 2.660 | 2.803 | 171.625 | 1.00x |
| flat.json | orjson | 3.113 | 3.238 | 3.358 | 171.625 | 0.82x |
| flat.json | msgspec | 3.139 | 3.293 | 3.507 | 171.625 | 0.81x |
| flat.json | ujson | 4.684 | 5.000 | 5.087 | 171.625 | 0.53x |
| flat.json | json | 6.163 | 6.548 | 6.739 | 171.625 | 0.41x |
| nested.json | strata | 2.069 | 2.140 | 2.194 | 171.734 | 1.00x |
| nested.json | orjson | 2.508 | 2.607 | 2.966 | 171.734 | 0.82x |
| nested.json | msgspec | 2.483 | 2.555 | 2.919 | 171.734 | 0.84x |
| nested.json | ujson | 3.693 | 3.787 | 3.964 | 171.734 | 0.57x |
| nested.json | json | 5.683 | 5.759 | 6.001 | 171.734 | 0.37x |
| wide_arrays.json | strata | 12.597 | 12.913 | 13.371 | 162.812 | 1.00x |
| wide_arrays.json | orjson | 14.481 | 14.939 | 15.789 | 162.812 | 0.86x |
| wide_arrays.json | msgspec | 17.821 | 18.136 | 18.292 | 162.812 | 0.71x |
| wide_arrays.json | ujson | 23.294 | 23.692 | 24.105 | 162.812 | 0.55x |
| wide_arrays.json | json | 32.449 | 32.913 | 33.484 | 162.812 | 0.39x |
| mixed.json | strata | 0.534 | 0.583 | 0.620 | 166.109 | 1.00x |
| mixed.json | orjson | 0.689 | 0.723 | 0.831 | 166.109 | 0.81x |
| mixed.json | msgspec | 0.735 | 0.783 | 1.034 | 166.109 | 0.74x |
| mixed.json | ujson | 0.956 | 0.970 | 1.127 | 166.109 | 0.60x |
| mixed.json | json | 1.396 | 1.495 | 1.716 | 166.109 | 0.39x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 37.360 | 38.352 | 38.748 | 170.734 | 1.00x |
| users.ndjson | orjson | 63.096 | 64.932 | 65.721 | 170.734 | 0.59x |
| users.ndjson | msgspec | 63.475 | 64.782 | 65.868 | 170.734 | 0.59x |
| users.ndjson | ujson | 82.616 | 83.415 | 84.792 | 170.734 | 0.46x |
| users.ndjson | json | 106.774 | 109.182 | 110.684 | 170.734 | 0.35x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.637 | 11.695 | 14.236 | 515.906 | 1.00x |
| users.json | orjson | 15.108 | 15.979 | 19.124 | 515.906 | 0.73x |
| users.json | msgspec | 19.293 | 20.233 | 24.058 | 515.906 | 0.58x |
| users.json | ujson | 67.055 | 68.759 | 70.156 | 515.906 | 0.17x |
| users.json | json | 92.357 | 94.068 | 96.452 | 515.906 | 0.12x |
| flat.json | strata | 1.036 | 1.164 | 1.365 | 171.625 | 1.00x |
| flat.json | orjson | 1.137 | 1.261 | 1.525 | 171.625 | 0.92x |
| flat.json | msgspec | 1.403 | 1.523 | 1.692 | 171.625 | 0.76x |
| flat.json | ujson | 3.769 | 3.886 | 4.076 | 171.625 | 0.30x |
| flat.json | json | 5.728 | 6.119 | 6.432 | 171.625 | 0.19x |
| nested.json | strata | 0.741 | 0.793 | 0.840 | 171.844 | 1.00x |
| nested.json | orjson | 1.050 | 1.096 | 1.142 | 171.844 | 0.72x |
| nested.json | msgspec | 1.275 | 1.350 | 1.483 | 171.844 | 0.59x |
| nested.json | ujson | 4.173 | 4.379 | 4.758 | 171.844 | 0.18x |
| nested.json | json | 6.722 | 7.200 | 7.638 | 171.844 | 0.11x |
| wide_arrays.json | strata | 5.631 | 5.748 | 6.008 | 166.062 | 1.00x |
| wide_arrays.json | orjson | 6.339 | 6.498 | 6.757 | 166.062 | 0.88x |
| wide_arrays.json | msgspec | 9.822 | 10.060 | 10.405 | 166.062 | 0.57x |
| wide_arrays.json | ujson | 26.922 | 27.400 | 27.903 | 166.062 | 0.21x |
| wide_arrays.json | json | 47.298 | 48.103 | 48.849 | 166.062 | 0.12x |
| mixed.json | strata | 0.283 | 0.322 | 0.389 | 166.109 | 1.00x |
| mixed.json | orjson | 0.314 | 0.361 | 0.496 | 166.109 | 0.89x |
| mixed.json | msgspec | 0.327 | 0.381 | 0.482 | 166.109 | 0.85x |
| mixed.json | ujson | 1.027 | 1.084 | 1.266 | 166.109 | 0.30x |
| mixed.json | json | 1.540 | 1.779 | 1.940 | 166.109 | 0.18x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.176 | 0.222 | 0.290 | 516.578 | 1.00x |
| users.json $[*].id | jmespath | 0.724 | 0.783 | 0.851 | 516.578 | 0.28x |
| users.json $[*].id | jsonpath-ng | 3.068 | 3.146 | 3.324 | 516.578 | 0.07x |
| users.json $[*].orders[*].total | strata | 2.669 | 3.008 | 3.465 | 305.703 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 7.008 | 7.348 | 7.475 | 305.703 | 0.41x |
| users.json $[*].orders[*].total | jsonpath-ng | 37.072 | 38.751 | 42.706 | 305.703 | 0.08x |
| users.json $..total | strata | 8.252 | 8.391 | 8.959 | 160.953 | 1.00x |
| users.json $..total | jsonpath-ng | 1135.354 | 1141.125 | 1147.342 | 160.953 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 19.401 | 19.689 | 20.250 | 584.766 | 1.00x |
| users.json $[*].id | orjson+jmespath | 53.848 | 55.901 | 59.143 | 584.766 | 0.35x |
| users.json $[*].id | orjson+jsonpath-ng | 56.869 | 58.253 | 59.498 | 584.766 | 0.34x |
| users.json $[*].orders[*].total | strata | 19.561 | 19.678 | 20.092 | 365.875 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 58.680 | 59.965 | 89.113 | 365.875 | 0.33x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 103.068 | 104.265 | 106.759 | 365.875 | 0.19x |
| users.json $..total | strata | 45.203 | 46.751 | 47.178 | 203.109 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 1196.463 | 1202.328 | 1208.360 | 203.109 | 0.04x |

