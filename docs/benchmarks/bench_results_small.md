# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 3b61188
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
| users.json | strata | 10.002 | 10.119 | 10.571 | 52.625 | 1.00x |
| users.json | orjson | 10.638 | 10.924 | 11.252 | 52.625 | 0.93x |
| users.json | msgspec | 10.842 | 11.121 | 11.553 | 52.625 | 0.91x |
| users.json | ujson | 14.443 | 14.762 | 15.204 | 52.625 | 0.69x |
| users.json | json | 18.993 | 19.370 | 19.711 | 52.625 | 0.52x |
| flat.json | strata | 0.985 | 0.992 | 1.058 | 69.844 | 1.00x |
| flat.json | orjson | 0.824 | 0.832 | 0.850 | 69.844 | 1.19x |
| flat.json | msgspec | 0.900 | 0.907 | 0.952 | 69.844 | 1.09x |
| flat.json | ujson | 1.314 | 1.323 | 1.441 | 69.844 | 0.75x |
| flat.json | json | 1.819 | 1.833 | 1.872 | 69.844 | 0.54x |
| nested.json | strata | 0.786 | 0.799 | 0.906 | 69.875 | 1.00x |
| nested.json | orjson | 1.561 | 1.603 | 1.897 | 69.875 | 0.50x |
| nested.json | msgspec | 1.560 | 1.620 | 1.817 | 69.875 | 0.49x |
| nested.json | ujson | 1.917 | 1.978 | 2.262 | 69.875 | 0.40x |
| nested.json | json | 2.506 | 2.538 | 2.876 | 69.875 | 0.31x |
| wide_arrays.json | strata | 4.701 | 4.734 | 4.788 | 69.891 | 1.00x |
| wide_arrays.json | orjson | 4.843 | 4.919 | 4.976 | 69.891 | 0.96x |
| wide_arrays.json | msgspec | 5.767 | 5.808 | 6.384 | 69.891 | 0.81x |
| wide_arrays.json | ujson | 7.310 | 7.558 | 7.981 | 69.891 | 0.63x |
| wide_arrays.json | json | 10.363 | 10.543 | 11.118 | 69.891 | 0.45x |
| mixed.json | strata | 0.207 | 0.211 | 0.218 | 69.906 | 1.00x |
| mixed.json | orjson | 0.163 | 0.166 | 0.170 | 69.906 | 1.27x |
| mixed.json | msgspec | 0.178 | 0.183 | 0.194 | 69.906 | 1.15x |
| mixed.json | ujson | 0.233 | 0.242 | 0.259 | 69.906 | 0.87x |
| mixed.json | json | 0.388 | 0.396 | 0.427 | 69.906 | 0.53x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.680 | 2.701 | 2.870 | 55.094 | 1.00x |
| users.json | orjson | 2.585 | 2.601 | 2.681 | 55.094 | 1.04x |
| users.json | msgspec | 3.395 | 3.406 | 3.542 | 55.094 | 0.79x |
| users.json | ujson | 13.764 | 13.868 | 14.206 | 55.094 | 0.19x |
| users.json | json | 19.545 | 19.701 | 20.157 | 55.094 | 0.14x |
| flat.json | strata | 0.278 | 0.290 | 0.307 | 69.859 | 1.00x |
| flat.json | orjson | 0.291 | 0.298 | 0.308 | 69.859 | 0.97x |
| flat.json | msgspec | 0.385 | 0.391 | 0.424 | 69.859 | 0.74x |
| flat.json | ujson | 1.135 | 1.149 | 1.193 | 69.859 | 0.25x |
| flat.json | json | 1.632 | 1.653 | 1.754 | 69.859 | 0.18x |
| nested.json | strata | 0.206 | 0.208 | 0.213 | 69.891 | 1.00x |
| nested.json | orjson | 0.258 | 0.262 | 0.272 | 69.891 | 0.79x |
| nested.json | msgspec | 0.353 | 0.357 | 0.372 | 69.891 | 0.58x |
| nested.json | ujson | 1.255 | 1.267 | 1.286 | 69.891 | 0.16x |
| nested.json | json | 2.058 | 2.080 | 2.186 | 69.891 | 0.10x |
| wide_arrays.json | strata | 1.457 | 1.471 | 1.609 | 69.906 | 1.00x |
| wide_arrays.json | orjson | 1.432 | 1.453 | 1.697 | 69.906 | 1.01x |
| wide_arrays.json | msgspec | 2.519 | 2.547 | 2.817 | 69.906 | 0.58x |
| wide_arrays.json | ujson | 7.795 | 7.865 | 7.999 | 69.906 | 0.19x |
| wide_arrays.json | json | 13.994 | 14.137 | 14.332 | 69.906 | 0.10x |
| mixed.json | strata | 0.059 | 0.066 | 0.073 | 69.906 | 1.00x |
| mixed.json | orjson | 0.055 | 0.058 | 0.078 | 69.906 | 1.14x |
| mixed.json | msgspec | 0.065 | 0.073 | 0.092 | 69.906 | 0.90x |
| mixed.json | ujson | 0.277 | 0.286 | 0.295 | 69.906 | 0.23x |
| mixed.json | json | 0.441 | 0.460 | 0.491 | 69.906 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.310 | 10.394 | 10.590 | 70.141 | 1.00x |
| users.json | orjson | 12.927 | 13.282 | 14.416 | 70.141 | 0.78x |
| users.json | msgspec | 13.201 | 13.282 | 13.867 | 70.141 | 0.78x |
| users.json | ujson | 16.930 | 17.103 | 17.912 | 70.141 | 0.61x |
| users.json | json | 21.347 | 21.476 | 22.028 | 70.141 | 0.48x |
| flat.json | strata | 1.061 | 1.082 | 1.129 | 69.859 | 1.00x |
| flat.json | orjson | 0.910 | 0.928 | 0.983 | 69.859 | 1.16x |
| flat.json | msgspec | 0.981 | 1.006 | 1.058 | 69.859 | 1.07x |
| flat.json | ujson | 1.437 | 1.448 | 1.518 | 69.859 | 0.75x |
| flat.json | json | 1.904 | 1.934 | 2.032 | 69.859 | 0.56x |
| nested.json | strata | 0.862 | 0.870 | 0.918 | 69.891 | 1.00x |
| nested.json | orjson | 1.744 | 1.770 | 1.837 | 69.891 | 0.49x |
| nested.json | msgspec | 1.755 | 1.768 | 1.989 | 69.891 | 0.49x |
| nested.json | ujson | 2.129 | 2.195 | 2.287 | 69.891 | 0.40x |
| nested.json | json | 2.685 | 2.715 | 2.788 | 69.891 | 0.32x |
| wide_arrays.json | strata | 4.878 | 4.935 | 5.022 | 69.906 | 1.00x |
| wide_arrays.json | orjson | 5.429 | 5.653 | 6.034 | 69.906 | 0.87x |
| wide_arrays.json | msgspec | 6.445 | 6.543 | 6.882 | 69.906 | 0.75x |
| wide_arrays.json | ujson | 8.199 | 8.263 | 8.781 | 69.906 | 0.60x |
| wide_arrays.json | json | 11.016 | 11.095 | 11.417 | 69.906 | 0.44x |
| mixed.json | strata | 0.257 | 0.273 | 0.311 | 69.906 | 1.00x |
| mixed.json | orjson | 0.223 | 0.230 | 0.251 | 69.906 | 1.19x |
| mixed.json | msgspec | 0.246 | 0.261 | 0.322 | 69.906 | 1.04x |
| mixed.json | ujson | 0.313 | 0.336 | 0.403 | 69.906 | 0.81x |
| mixed.json | json | 0.453 | 0.463 | 0.532 | 69.906 | 0.59x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.778 | 9.913 | 10.554 | 68.984 | 1.00x |
| users.ndjson | orjson | 13.325 | 13.625 | 15.096 | 68.984 | 0.73x |
| users.ndjson | msgspec | 13.177 | 13.383 | 13.921 | 68.984 | 0.74x |
| users.ndjson | ujson | 16.780 | 16.938 | 17.525 | 68.984 | 0.59x |
| users.ndjson | json | 23.117 | 23.213 | 23.982 | 68.984 | 0.43x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.108 | 3.180 | 3.339 | 65.219 | 1.00x |
| users.json | orjson | 3.004 | 3.072 | 3.136 | 65.219 | 1.04x |
| users.json | msgspec | 3.802 | 3.897 | 4.052 | 65.219 | 0.82x |
| users.json | ujson | 14.066 | 14.398 | 14.610 | 65.219 | 0.22x |
| users.json | json | 20.007 | 20.286 | 20.807 | 65.219 | 0.16x |
| flat.json | strata | 0.432 | 0.468 | 0.514 | 69.875 | 1.00x |
| flat.json | orjson | 0.433 | 0.453 | 0.474 | 69.875 | 1.03x |
| flat.json | msgspec | 0.522 | 0.534 | 0.568 | 69.875 | 0.88x |
| flat.json | ujson | 1.283 | 1.310 | 1.356 | 69.875 | 0.36x |
| flat.json | json | 1.797 | 1.817 | 1.874 | 69.875 | 0.26x |
| nested.json | strata | 0.381 | 0.405 | 0.526 | 69.891 | 1.00x |
| nested.json | orjson | 0.394 | 0.413 | 0.468 | 69.891 | 0.98x |
| nested.json | msgspec | 0.485 | 0.506 | 0.535 | 69.891 | 0.80x |
| nested.json | ujson | 1.434 | 1.490 | 1.675 | 69.891 | 0.27x |
| nested.json | json | 2.220 | 2.238 | 2.347 | 69.891 | 0.18x |
| wide_arrays.json | strata | 1.786 | 1.856 | 2.003 | 69.906 | 1.00x |
| wide_arrays.json | orjson | 1.763 | 1.811 | 1.986 | 69.906 | 1.02x |
| wide_arrays.json | msgspec | 2.811 | 2.870 | 2.960 | 69.906 | 0.65x |
| wide_arrays.json | ujson | 8.178 | 8.259 | 8.695 | 69.906 | 0.22x |
| wide_arrays.json | json | 14.299 | 14.469 | 14.972 | 69.906 | 0.13x |
| mixed.json | strata | 0.211 | 0.232 | 0.266 | 69.938 | 1.00x |
| mixed.json | orjson | 0.172 | 0.183 | 0.194 | 69.938 | 1.26x |
| mixed.json | msgspec | 0.182 | 0.193 | 0.278 | 69.938 | 1.20x |
| mixed.json | ujson | 0.403 | 0.423 | 0.439 | 69.938 | 0.55x |
| mixed.json | json | 0.574 | 0.582 | 0.664 | 69.938 | 0.40x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.064 | 0.071 | 0.086 | 65.656 | 1.00x |
| users.json $[*].id | jmespath | 0.431 | 0.442 | 0.451 | 65.656 | 0.16x |
| users.json $[*].id | jsonpath-ng | 4.203 | 4.245 | 4.539 | 65.656 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.418 | 0.425 | 0.444 | 67.078 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.687 | 2.732 | 3.058 | 67.078 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 14.465 | 14.527 | 15.995 | 67.078 | 0.03x |
| users.json $..total | strata | 1.767 | 1.785 | 1.823 | 69.219 | 1.00x |
| users.json $..total | jsonpath-ng | 271.615 | 273.443 | 280.842 | 69.219 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.889 | 4.954 | 5.036 | 66.781 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.430 | 13.833 | 14.799 | 66.781 | 0.36x |
| users.json $[*].id | orjson+jsonpath-ng | 14.985 | 15.203 | 15.950 | 66.781 | 0.33x |
| users.json $[*].orders[*].total | strata | 4.976 | 5.051 | 5.223 | 69.141 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.737 | 15.962 | 17.204 | 69.141 | 0.32x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 25.444 | 25.801 | 26.841 | 69.141 | 0.20x |
| users.json $..total | strata | 14.625 | 14.812 | 15.771 | 70.141 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 283.112 | 283.874 | 288.554 | 70.141 | 0.05x |

