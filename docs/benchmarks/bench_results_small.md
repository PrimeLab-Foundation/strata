# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 0a6c576
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
| users.json | strata | 6.380 | 6.478 | 6.796 | 52.953 | 1.00x |
| users.json | orjson | 7.891 | 7.972 | 8.302 | 52.953 | 0.81x |
| users.json | msgspec | 8.004 | 8.115 | 8.377 | 52.953 | 0.80x |
| users.json | ujson | 10.844 | 10.979 | 11.413 | 52.953 | 0.59x |
| users.json | json | 15.115 | 15.344 | 15.987 | 52.953 | 0.42x |
| flat.json | strata | 0.595 | 0.599 | 0.616 | 69.797 | 1.00x |
| flat.json | orjson | 0.640 | 0.643 | 0.648 | 69.797 | 0.93x |
| flat.json | msgspec | 0.676 | 0.687 | 0.695 | 69.797 | 0.87x |
| flat.json | ujson | 1.026 | 1.035 | 1.045 | 69.797 | 0.58x |
| flat.json | json | 1.416 | 1.421 | 1.439 | 69.797 | 0.42x |
| nested.json | strata | 0.534 | 0.538 | 0.544 | 69.875 | 1.00x |
| nested.json | orjson | 0.604 | 0.607 | 0.617 | 69.875 | 0.89x |
| nested.json | msgspec | 0.606 | 0.612 | 0.624 | 69.875 | 0.88x |
| nested.json | ujson | 0.873 | 0.879 | 0.891 | 69.875 | 0.61x |
| nested.json | json | 1.388 | 1.395 | 1.413 | 69.875 | 0.39x |
| wide_arrays.json | strata | 2.988 | 3.051 | 3.399 | 70.016 | 1.00x |
| wide_arrays.json | orjson | 3.171 | 3.265 | 3.394 | 70.016 | 0.93x |
| wide_arrays.json | msgspec | 3.960 | 4.014 | 4.514 | 70.016 | 0.76x |
| wide_arrays.json | ujson | 5.146 | 5.198 | 5.243 | 70.016 | 0.59x |
| wide_arrays.json | json | 7.559 | 7.597 | 7.749 | 70.016 | 0.40x |
| mixed.json | strata | 0.127 | 0.129 | 0.141 | 70.016 | 1.00x |
| mixed.json | orjson | 0.128 | 0.129 | 0.136 | 70.016 | 1.00x |
| mixed.json | msgspec | 0.141 | 0.142 | 0.152 | 70.016 | 0.91x |
| mixed.json | ujson | 0.180 | 0.182 | 0.189 | 70.016 | 0.71x |
| mixed.json | json | 0.306 | 0.307 | 0.319 | 70.016 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.431 | 1.436 | 1.458 | 55.531 | 1.00x |
| users.json | orjson | 2.004 | 2.010 | 2.078 | 55.531 | 0.71x |
| users.json | msgspec | 2.630 | 2.637 | 2.683 | 55.531 | 0.54x |
| users.json | ujson | 10.620 | 10.652 | 11.174 | 55.531 | 0.13x |
| users.json | json | 15.479 | 15.534 | 16.302 | 55.531 | 0.09x |
| flat.json | strata | 0.177 | 0.179 | 0.181 | 69.828 | 1.00x |
| flat.json | orjson | 0.224 | 0.225 | 0.225 | 69.828 | 0.80x |
| flat.json | msgspec | 0.294 | 0.295 | 0.297 | 69.828 | 0.61x |
| flat.json | ujson | 0.902 | 0.905 | 0.908 | 69.828 | 0.20x |
| flat.json | json | 1.323 | 1.339 | 1.383 | 69.828 | 0.13x |
| nested.json | strata | 0.134 | 0.136 | 0.140 | 70.016 | 1.00x |
| nested.json | orjson | 0.198 | 0.201 | 0.205 | 70.016 | 0.68x |
| nested.json | msgspec | 0.267 | 0.269 | 0.272 | 70.016 | 0.51x |
| nested.json | ujson | 0.959 | 0.966 | 1.029 | 70.016 | 0.14x |
| nested.json | json | 1.616 | 1.624 | 1.992 | 70.016 | 0.08x |
| wide_arrays.json | strata | 0.913 | 0.931 | 0.955 | 70.016 | 1.00x |
| wide_arrays.json | orjson | 1.113 | 1.116 | 1.155 | 70.016 | 0.83x |
| wide_arrays.json | msgspec | 1.933 | 1.952 | 1.996 | 70.016 | 0.48x |
| wide_arrays.json | ujson | 5.984 | 6.002 | 6.218 | 70.016 | 0.16x |
| wide_arrays.json | json | 11.014 | 11.192 | 12.166 | 70.016 | 0.08x |
| mixed.json | strata | 0.036 | 0.038 | 0.042 | 70.094 | 1.00x |
| mixed.json | orjson | 0.043 | 0.044 | 0.057 | 70.094 | 0.87x |
| mixed.json | msgspec | 0.049 | 0.052 | 0.063 | 70.094 | 0.73x |
| mixed.json | ujson | 0.215 | 0.224 | 0.228 | 70.094 | 0.17x |
| mixed.json | json | 0.356 | 0.369 | 0.449 | 70.094 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.671 | 6.766 | 7.070 | 68.938 | 1.00x |
| users.json | orjson | 8.236 | 8.371 | 8.688 | 68.938 | 0.81x |
| users.json | msgspec | 8.295 | 8.505 | 8.706 | 68.938 | 0.80x |
| users.json | ujson | 11.264 | 11.492 | 12.282 | 68.938 | 0.59x |
| users.json | json | 15.415 | 15.603 | 16.323 | 68.938 | 0.43x |
| flat.json | strata | 0.668 | 0.676 | 0.766 | 69.828 | 1.00x |
| flat.json | orjson | 0.720 | 0.732 | 0.821 | 69.828 | 0.92x |
| flat.json | msgspec | 0.759 | 0.770 | 0.844 | 69.828 | 0.88x |
| flat.json | ujson | 1.136 | 1.151 | 1.282 | 69.828 | 0.59x |
| flat.json | json | 1.486 | 1.507 | 1.753 | 69.828 | 0.45x |
| nested.json | strata | 0.591 | 0.600 | 0.629 | 70.016 | 1.00x |
| nested.json | orjson | 0.671 | 0.676 | 0.695 | 70.016 | 0.89x |
| nested.json | msgspec | 0.672 | 0.685 | 0.698 | 70.016 | 0.88x |
| nested.json | ujson | 0.965 | 0.977 | 1.001 | 70.016 | 0.61x |
| nested.json | json | 1.465 | 1.478 | 1.494 | 70.016 | 0.41x |
| wide_arrays.json | strata | 3.147 | 3.164 | 3.203 | 70.016 | 1.00x |
| wide_arrays.json | orjson | 3.331 | 3.360 | 3.396 | 70.016 | 0.94x |
| wide_arrays.json | msgspec | 4.155 | 4.166 | 4.216 | 70.016 | 0.76x |
| wide_arrays.json | ujson | 5.462 | 5.484 | 5.503 | 70.016 | 0.58x |
| wide_arrays.json | json | 7.740 | 7.773 | 7.829 | 70.016 | 0.41x |
| mixed.json | strata | 0.175 | 0.188 | 0.213 | 70.094 | 1.00x |
| mixed.json | orjson | 0.188 | 0.205 | 0.217 | 70.094 | 0.92x |
| mixed.json | msgspec | 0.195 | 0.207 | 0.226 | 70.094 | 0.91x |
| mixed.json | ujson | 0.248 | 0.258 | 0.287 | 70.094 | 0.73x |
| mixed.json | json | 0.353 | 0.357 | 0.380 | 70.094 | 0.53x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.859 | 6.898 | 7.094 | 68.938 | 1.00x |
| users.ndjson | orjson | 10.042 | 10.156 | 10.422 | 68.938 | 0.68x |
| users.ndjson | msgspec | 9.974 | 10.118 | 10.819 | 68.938 | 0.68x |
| users.ndjson | ujson | 12.791 | 12.980 | 13.503 | 68.938 | 0.53x |
| users.ndjson | json | 17.920 | 18.065 | 18.463 | 68.938 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.790 | 1.837 | 2.064 | 64.938 | 1.00x |
| users.json | orjson | 2.399 | 2.447 | 2.578 | 64.938 | 0.75x |
| users.json | msgspec | 3.009 | 3.044 | 3.298 | 64.938 | 0.60x |
| users.json | ujson | 11.192 | 11.247 | 11.996 | 64.938 | 0.16x |
| users.json | json | 15.947 | 16.006 | 17.093 | 64.938 | 0.11x |
| flat.json | strata | 0.314 | 0.337 | 0.409 | 69.859 | 1.00x |
| flat.json | orjson | 0.376 | 0.388 | 0.431 | 69.859 | 0.87x |
| flat.json | msgspec | 0.428 | 0.448 | 0.549 | 69.859 | 0.75x |
| flat.json | ujson | 1.047 | 1.066 | 1.162 | 69.859 | 0.32x |
| flat.json | json | 1.490 | 1.513 | 1.711 | 69.859 | 0.22x |
| nested.json | strata | 0.286 | 0.293 | 0.314 | 70.016 | 1.00x |
| nested.json | orjson | 0.343 | 0.361 | 0.372 | 70.016 | 0.81x |
| nested.json | msgspec | 0.420 | 0.428 | 0.457 | 70.016 | 0.69x |
| nested.json | ujson | 1.125 | 1.152 | 1.164 | 70.016 | 0.25x |
| nested.json | json | 1.784 | 1.804 | 1.920 | 70.016 | 0.16x |
| wide_arrays.json | strata | 1.264 | 1.327 | 1.641 | 70.016 | 1.00x |
| wide_arrays.json | orjson | 1.444 | 1.528 | 1.969 | 70.016 | 0.87x |
| wide_arrays.json | msgspec | 2.219 | 2.321 | 2.664 | 70.016 | 0.57x |
| wide_arrays.json | ujson | 6.336 | 6.441 | 6.844 | 70.016 | 0.21x |
| wide_arrays.json | json | 11.390 | 11.504 | 12.325 | 70.016 | 0.12x |
| mixed.json | strata | 0.153 | 0.160 | 0.240 | 70.125 | 1.00x |
| mixed.json | orjson | 0.156 | 0.177 | 0.228 | 70.125 | 0.90x |
| mixed.json | msgspec | 0.167 | 0.182 | 0.241 | 70.125 | 0.88x |
| mixed.json | ujson | 0.339 | 0.366 | 0.403 | 70.125 | 0.44x |
| mixed.json | json | 0.479 | 0.491 | 0.511 | 70.125 | 0.33x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.049 | 0.050 | 0.068 | 65.125 | 1.00x |
| users.json $[*].id | jmespath | 0.267 | 0.269 | 0.276 | 65.125 | 0.19x |
| users.json $[*].id | jsonpath-ng | 1.421 | 1.434 | 1.451 | 65.125 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.331 | 0.340 | 0.377 | 67.031 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.644 | 1.654 | 1.689 | 67.031 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 9.488 | 9.519 | 10.268 | 67.031 | 0.04x |
| users.json $..total | strata | 1.375 | 1.395 | 1.527 | 69.125 | 1.00x |
| users.json $..total | jsonpath-ng | 185.487 | 187.235 | 193.735 | 69.125 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.364 | 3.413 | 3.623 | 66.891 | 1.00x |
| users.json $[*].id | orjson+jmespath | 8.587 | 8.697 | 8.950 | 66.891 | 0.39x |
| users.json $[*].id | orjson+jsonpath-ng | 9.736 | 9.881 | 10.016 | 66.891 | 0.35x |
| users.json $[*].orders[*].total | strata | 3.441 | 3.479 | 3.503 | 69.078 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 10.004 | 10.066 | 10.272 | 69.078 | 0.35x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 19.198 | 19.329 | 19.572 | 69.078 | 0.18x |
| users.json $..total | strata | 8.184 | 8.242 | 8.360 | 70.031 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 196.078 | 196.672 | 201.250 | 70.031 | 0.04x |

