# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 26c72e3
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
| users.json | strata | 6.780 | 6.832 | 7.277 | 52.438 | 1.00x |
| users.json | orjson | 8.366 | 8.477 | 8.979 | 52.438 | 0.81x |
| users.json | msgspec | 8.435 | 8.786 | 9.388 | 52.438 | 0.78x |
| users.json | ujson | 11.130 | 11.714 | 12.038 | 52.438 | 0.58x |
| users.json | json | 14.762 | 15.374 | 15.792 | 52.438 | 0.44x |
| flat.json | strata | 0.608 | 0.617 | 0.680 | 70.438 | 1.00x |
| flat.json | orjson | 0.613 | 0.624 | 0.647 | 70.438 | 0.99x |
| flat.json | msgspec | 0.662 | 0.668 | 0.745 | 70.438 | 0.92x |
| flat.json | ujson | 0.953 | 0.971 | 1.035 | 70.438 | 0.64x |
| flat.json | json | 1.344 | 1.365 | 1.459 | 70.438 | 0.45x |
| nested.json | strata | 0.546 | 0.555 | 0.608 | 70.516 | 1.00x |
| nested.json | orjson | 1.204 | 1.262 | 1.477 | 70.516 | 0.44x |
| nested.json | msgspec | 1.193 | 1.245 | 1.360 | 70.516 | 0.45x |
| nested.json | ujson | 1.460 | 1.546 | 1.811 | 70.516 | 0.36x |
| nested.json | json | 1.908 | 2.015 | 2.263 | 70.516 | 0.28x |
| wide_arrays.json | strata | 3.256 | 3.290 | 3.535 | 70.547 | 1.00x |
| wide_arrays.json | orjson | 3.767 | 3.813 | 4.253 | 70.547 | 0.86x |
| wide_arrays.json | msgspec | 4.475 | 4.520 | 4.602 | 70.547 | 0.73x |
| wide_arrays.json | ujson | 5.644 | 5.752 | 5.957 | 70.547 | 0.57x |
| wide_arrays.json | json | 7.982 | 8.073 | 8.618 | 70.547 | 0.41x |
| mixed.json | strata | 0.131 | 0.134 | 0.136 | 70.547 | 1.00x |
| mixed.json | orjson | 0.126 | 0.128 | 0.134 | 70.547 | 1.05x |
| mixed.json | msgspec | 0.138 | 0.141 | 0.157 | 70.547 | 0.95x |
| mixed.json | ujson | 0.181 | 0.185 | 0.202 | 70.547 | 0.72x |
| mixed.json | json | 0.300 | 0.303 | 0.323 | 70.547 | 0.44x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.646 | 1.660 | 1.728 | 57.234 | 1.00x |
| users.json | orjson | 1.956 | 1.987 | 2.039 | 57.234 | 0.84x |
| users.json | msgspec | 2.585 | 2.601 | 2.665 | 57.234 | 0.64x |
| users.json | ujson | 10.478 | 10.574 | 11.077 | 57.234 | 0.16x |
| users.json | json | 14.912 | 15.154 | 15.447 | 57.234 | 0.11x |
| flat.json | strata | 0.179 | 0.183 | 0.196 | 70.500 | 1.00x |
| flat.json | orjson | 0.223 | 0.227 | 0.286 | 70.500 | 0.81x |
| flat.json | msgspec | 0.294 | 0.302 | 0.320 | 70.500 | 0.61x |
| flat.json | ujson | 0.863 | 0.876 | 0.930 | 70.500 | 0.21x |
| flat.json | json | 1.237 | 1.267 | 1.293 | 70.500 | 0.14x |
| nested.json | strata | 0.134 | 0.138 | 0.141 | 70.547 | 1.00x |
| nested.json | orjson | 0.198 | 0.201 | 0.210 | 70.547 | 0.69x |
| nested.json | msgspec | 0.266 | 0.269 | 0.316 | 70.547 | 0.51x |
| nested.json | ujson | 0.965 | 0.977 | 1.023 | 70.547 | 0.14x |
| nested.json | json | 1.567 | 1.579 | 1.592 | 70.547 | 0.09x |
| wide_arrays.json | strata | 1.085 | 1.096 | 1.153 | 70.547 | 1.00x |
| wide_arrays.json | orjson | 1.093 | 1.112 | 1.163 | 70.547 | 0.99x |
| wide_arrays.json | msgspec | 1.918 | 1.934 | 2.004 | 70.547 | 0.57x |
| wide_arrays.json | ujson | 5.967 | 6.031 | 6.516 | 70.547 | 0.18x |
| wide_arrays.json | json | 10.627 | 10.763 | 11.163 | 70.547 | 0.10x |
| mixed.json | strata | 0.039 | 0.043 | 0.064 | 70.562 | 1.00x |
| mixed.json | orjson | 0.042 | 0.044 | 0.053 | 70.562 | 0.97x |
| mixed.json | msgspec | 0.049 | 0.051 | 0.056 | 70.562 | 0.83x |
| mixed.json | ujson | 0.211 | 0.214 | 0.221 | 70.562 | 0.20x |
| mixed.json | json | 0.331 | 0.339 | 0.386 | 70.562 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.062 | 7.125 | 7.361 | 72.156 | 1.00x |
| users.json | orjson | 10.413 | 10.535 | 11.737 | 72.156 | 0.68x |
| users.json | msgspec | 10.499 | 10.627 | 11.378 | 72.156 | 0.67x |
| users.json | ujson | 13.441 | 13.749 | 15.255 | 72.156 | 0.52x |
| users.json | json | 16.766 | 17.061 | 17.712 | 72.156 | 0.42x |
| flat.json | strata | 0.672 | 0.682 | 0.755 | 70.500 | 1.00x |
| flat.json | orjson | 0.678 | 0.696 | 0.819 | 70.500 | 0.98x |
| flat.json | msgspec | 0.742 | 0.769 | 0.832 | 70.500 | 0.89x |
| flat.json | ujson | 1.069 | 1.095 | 1.152 | 70.500 | 0.62x |
| flat.json | json | 1.409 | 1.448 | 1.556 | 70.500 | 0.47x |
| nested.json | strata | 0.609 | 0.621 | 0.644 | 70.547 | 1.00x |
| nested.json | orjson | 1.364 | 1.404 | 1.482 | 70.547 | 0.44x |
| nested.json | msgspec | 1.377 | 1.407 | 1.605 | 70.547 | 0.44x |
| nested.json | ujson | 1.672 | 1.700 | 1.752 | 70.547 | 0.37x |
| nested.json | json | 2.087 | 2.112 | 2.212 | 70.547 | 0.29x |
| wide_arrays.json | strata | 3.445 | 3.478 | 3.584 | 70.547 | 1.00x |
| wide_arrays.json | orjson | 4.255 | 4.321 | 4.620 | 70.547 | 0.80x |
| wide_arrays.json | msgspec | 5.018 | 5.077 | 5.230 | 70.547 | 0.69x |
| wide_arrays.json | ujson | 6.316 | 6.448 | 6.834 | 70.547 | 0.54x |
| wide_arrays.json | json | 8.510 | 8.631 | 9.409 | 70.547 | 0.40x |
| mixed.json | strata | 0.169 | 0.182 | 0.191 | 70.562 | 1.00x |
| mixed.json | orjson | 0.167 | 0.182 | 0.257 | 70.562 | 1.00x |
| mixed.json | msgspec | 0.180 | 0.191 | 0.213 | 70.562 | 0.95x |
| mixed.json | ujson | 0.243 | 0.248 | 0.272 | 70.562 | 0.73x |
| mixed.json | json | 0.343 | 0.348 | 0.373 | 70.562 | 0.52x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.287 | 7.354 | 7.692 | 69.578 | 1.00x |
| users.ndjson | orjson | 10.366 | 10.607 | 11.210 | 69.578 | 0.69x |
| users.ndjson | msgspec | 10.140 | 10.260 | 10.763 | 69.578 | 0.72x |
| users.ndjson | ujson | 12.882 | 12.993 | 13.354 | 69.578 | 0.57x |
| users.ndjson | json | 17.756 | 17.974 | 19.768 | 69.578 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.028 | 2.054 | 2.105 | 70.766 | 1.00x |
| users.json | orjson | 2.345 | 2.388 | 2.442 | 70.766 | 0.86x |
| users.json | msgspec | 2.976 | 3.048 | 3.149 | 70.766 | 0.67x |
| users.json | ujson | 10.871 | 10.958 | 11.269 | 70.766 | 0.19x |
| users.json | json | 15.444 | 15.591 | 16.169 | 70.766 | 0.13x |
| flat.json | strata | 0.308 | 0.334 | 0.395 | 70.516 | 1.00x |
| flat.json | orjson | 0.355 | 0.394 | 0.425 | 70.516 | 0.85x |
| flat.json | msgspec | 0.422 | 0.439 | 0.478 | 70.516 | 0.76x |
| flat.json | ujson | 1.002 | 1.036 | 1.106 | 70.516 | 0.32x |
| flat.json | json | 1.386 | 1.460 | 1.510 | 70.516 | 0.23x |
| nested.json | strata | 0.253 | 0.268 | 0.335 | 70.547 | 1.00x |
| nested.json | orjson | 0.319 | 0.331 | 0.370 | 70.547 | 0.81x |
| nested.json | msgspec | 0.384 | 0.401 | 0.437 | 70.547 | 0.67x |
| nested.json | ujson | 1.116 | 1.150 | 1.245 | 70.547 | 0.23x |
| nested.json | json | 1.722 | 1.781 | 1.830 | 70.547 | 0.15x |
| wide_arrays.json | strata | 1.361 | 1.415 | 1.517 | 70.547 | 1.00x |
| wide_arrays.json | orjson | 1.388 | 1.447 | 1.560 | 70.547 | 0.98x |
| wide_arrays.json | msgspec | 2.208 | 2.250 | 2.410 | 70.547 | 0.63x |
| wide_arrays.json | ujson | 6.257 | 6.416 | 6.698 | 70.547 | 0.22x |
| wide_arrays.json | json | 10.929 | 11.142 | 11.352 | 70.547 | 0.13x |
| mixed.json | strata | 0.141 | 0.163 | 0.194 | 70.594 | 1.00x |
| mixed.json | orjson | 0.156 | 0.162 | 0.204 | 70.594 | 1.00x |
| mixed.json | msgspec | 0.155 | 0.166 | 0.233 | 70.594 | 0.98x |
| mixed.json | ujson | 0.326 | 0.348 | 0.367 | 70.594 | 0.47x |
| mixed.json | json | 0.464 | 0.469 | 0.588 | 70.594 | 0.35x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.052 | 0.066 | 0.081 | 71.031 | 1.00x |
| users.json $[*].id | jmespath | 0.344 | 0.355 | 0.466 | 71.031 | 0.19x |
| users.json $[*].id | jsonpath-ng | 3.398 | 3.621 | 3.878 | 71.031 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.343 | 0.366 | 0.396 | 71.328 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.069 | 2.089 | 2.263 | 71.328 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 11.261 | 11.429 | 12.487 | 71.328 | 0.03x |
| users.json $..total | strata | 1.357 | 1.382 | 1.443 | 72.656 | 1.00x |
| users.json $..total | jsonpath-ng | 207.479 | 209.293 | 211.075 | 72.656 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.250 | 3.280 | 3.434 | 71.125 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.817 | 11.122 | 12.375 | 71.125 | 0.29x |
| users.json $[*].id | orjson+jsonpath-ng | 12.086 | 12.234 | 12.768 | 71.125 | 0.27x |
| users.json $[*].orders[*].total | strata | 3.334 | 3.384 | 3.577 | 72.547 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.656 | 12.941 | 13.514 | 72.547 | 0.26x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 19.993 | 20.560 | 21.447 | 72.547 | 0.16x |
| users.json $..total | strata | 10.550 | 10.699 | 11.532 | 72.656 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 216.595 | 217.582 | 223.773 | 72.656 | 0.05x |

