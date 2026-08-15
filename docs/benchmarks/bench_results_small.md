# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 4d4a4be
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
| users.json | strata | 6.635 | 6.705 | 7.003 | 51.234 | 1.00x |
| users.json | orjson | 8.219 | 8.300 | 8.753 | 51.234 | 0.81x |
| users.json | msgspec | 8.311 | 8.515 | 8.736 | 51.234 | 0.79x |
| users.json | ujson | 11.049 | 11.348 | 11.985 | 51.234 | 0.59x |
| users.json | json | 14.485 | 14.672 | 15.294 | 51.234 | 0.46x |
| flat.json | strata | 0.639 | 0.645 | 0.719 | 69.234 | 1.00x |
| flat.json | orjson | 0.638 | 0.643 | 0.690 | 69.234 | 1.00x |
| flat.json | msgspec | 0.679 | 0.686 | 0.763 | 69.234 | 0.94x |
| flat.json | ujson | 1.014 | 1.027 | 1.105 | 69.234 | 0.63x |
| flat.json | json | 1.424 | 1.434 | 1.475 | 69.234 | 0.45x |
| nested.json | strata | 0.545 | 0.553 | 0.640 | 69.234 | 1.00x |
| nested.json | orjson | 1.204 | 1.227 | 1.521 | 69.234 | 0.45x |
| nested.json | msgspec | 1.206 | 1.233 | 1.341 | 69.234 | 0.45x |
| nested.json | ujson | 1.463 | 1.525 | 1.762 | 69.234 | 0.36x |
| nested.json | json | 1.922 | 1.958 | 2.400 | 69.234 | 0.28x |
| wide_arrays.json | strata | 3.232 | 3.275 | 3.610 | 69.250 | 1.00x |
| wide_arrays.json | orjson | 3.776 | 3.852 | 4.199 | 69.250 | 0.85x |
| wide_arrays.json | msgspec | 4.468 | 4.571 | 4.729 | 69.250 | 0.72x |
| wide_arrays.json | ujson | 5.640 | 5.736 | 6.266 | 69.250 | 0.57x |
| wide_arrays.json | json | 7.926 | 8.059 | 8.671 | 69.250 | 0.41x |
| mixed.json | strata | 0.129 | 0.131 | 0.135 | 69.250 | 1.00x |
| mixed.json | orjson | 0.124 | 0.127 | 0.150 | 69.250 | 1.03x |
| mixed.json | msgspec | 0.136 | 0.141 | 0.226 | 69.250 | 0.93x |
| mixed.json | ujson | 0.180 | 0.190 | 0.228 | 69.250 | 0.69x |
| mixed.json | json | 0.297 | 0.301 | 0.315 | 69.250 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.719 | 1.744 | 1.895 | 57.156 | 1.00x |
| users.json | orjson | 1.985 | 2.011 | 2.130 | 57.156 | 0.87x |
| users.json | msgspec | 2.594 | 2.619 | 2.691 | 57.156 | 0.67x |
| users.json | ujson | 10.669 | 10.768 | 11.042 | 57.156 | 0.16x |
| users.json | json | 14.877 | 15.044 | 15.497 | 57.156 | 0.12x |
| flat.json | strata | 0.182 | 0.185 | 0.223 | 69.234 | 1.00x |
| flat.json | orjson | 0.219 | 0.225 | 0.240 | 69.234 | 0.82x |
| flat.json | msgspec | 0.288 | 0.292 | 0.332 | 69.234 | 0.63x |
| flat.json | ujson | 0.858 | 0.864 | 0.928 | 69.234 | 0.21x |
| flat.json | json | 1.229 | 1.242 | 1.288 | 69.234 | 0.15x |
| nested.json | strata | 0.149 | 0.153 | 0.170 | 69.250 | 1.00x |
| nested.json | orjson | 0.199 | 0.202 | 0.213 | 69.250 | 0.76x |
| nested.json | msgspec | 0.267 | 0.274 | 0.334 | 69.250 | 0.56x |
| nested.json | ujson | 0.951 | 0.965 | 1.015 | 69.250 | 0.16x |
| nested.json | json | 1.552 | 1.598 | 1.678 | 69.250 | 0.10x |
| wide_arrays.json | strata | 1.086 | 1.106 | 1.228 | 69.250 | 1.00x |
| wide_arrays.json | orjson | 1.119 | 1.131 | 1.202 | 69.250 | 0.98x |
| wide_arrays.json | msgspec | 1.913 | 1.955 | 1.992 | 69.250 | 0.57x |
| wide_arrays.json | ujson | 5.965 | 6.001 | 6.250 | 69.250 | 0.18x |
| wide_arrays.json | json | 10.647 | 10.762 | 10.951 | 69.250 | 0.10x |
| mixed.json | strata | 0.040 | 0.042 | 0.046 | 69.250 | 1.00x |
| mixed.json | orjson | 0.042 | 0.044 | 0.047 | 69.250 | 0.96x |
| mixed.json | msgspec | 0.048 | 0.051 | 0.075 | 69.250 | 0.83x |
| mixed.json | ujson | 0.209 | 0.211 | 0.233 | 69.250 | 0.20x |
| mixed.json | json | 0.332 | 0.335 | 0.354 | 69.250 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.019 | 7.102 | 7.368 | 70.938 | 1.00x |
| users.json | orjson | 10.383 | 10.627 | 11.217 | 70.938 | 0.67x |
| users.json | msgspec | 10.486 | 10.738 | 11.615 | 70.938 | 0.66x |
| users.json | ujson | 13.332 | 13.716 | 14.322 | 70.938 | 0.52x |
| users.json | json | 16.690 | 16.808 | 17.563 | 70.938 | 0.42x |
| flat.json | strata | 0.715 | 0.736 | 0.793 | 69.234 | 1.00x |
| flat.json | orjson | 0.720 | 0.739 | 0.808 | 69.234 | 1.00x |
| flat.json | msgspec | 0.756 | 0.769 | 0.823 | 69.234 | 0.96x |
| flat.json | ujson | 1.130 | 1.143 | 1.184 | 69.234 | 0.64x |
| flat.json | json | 1.503 | 1.518 | 1.625 | 69.234 | 0.49x |
| nested.json | strata | 0.608 | 0.621 | 0.681 | 69.250 | 1.00x |
| nested.json | orjson | 1.371 | 1.425 | 1.670 | 69.250 | 0.44x |
| nested.json | msgspec | 1.368 | 1.434 | 1.572 | 69.250 | 0.43x |
| nested.json | ujson | 1.666 | 1.706 | 1.975 | 69.250 | 0.36x |
| nested.json | json | 2.083 | 2.116 | 2.302 | 69.250 | 0.29x |
| wide_arrays.json | strata | 3.405 | 3.468 | 3.575 | 69.250 | 1.00x |
| wide_arrays.json | orjson | 4.272 | 4.413 | 4.624 | 69.250 | 0.79x |
| wide_arrays.json | msgspec | 5.015 | 5.158 | 5.444 | 69.250 | 0.67x |
| wide_arrays.json | ujson | 6.354 | 6.386 | 6.746 | 69.250 | 0.54x |
| wide_arrays.json | json | 8.468 | 8.785 | 9.308 | 69.250 | 0.39x |
| mixed.json | strata | 0.167 | 0.177 | 0.223 | 69.250 | 1.00x |
| mixed.json | orjson | 0.171 | 0.178 | 0.224 | 69.250 | 0.99x |
| mixed.json | msgspec | 0.181 | 0.189 | 0.230 | 69.250 | 0.94x |
| mixed.json | ujson | 0.236 | 0.250 | 0.284 | 69.250 | 0.71x |
| mixed.json | json | 0.341 | 0.354 | 0.362 | 69.250 | 0.50x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.114 | 7.189 | 7.721 | 68.375 | 1.00x |
| users.ndjson | orjson | 10.192 | 10.469 | 11.635 | 68.375 | 0.69x |
| users.ndjson | msgspec | 10.058 | 10.253 | 10.816 | 68.375 | 0.70x |
| users.ndjson | ujson | 12.843 | 12.977 | 14.039 | 68.375 | 0.55x |
| users.ndjson | json | 17.669 | 17.872 | 18.859 | 68.375 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.092 | 2.127 | 2.298 | 67.953 | 1.00x |
| users.json | orjson | 2.339 | 2.436 | 2.506 | 67.953 | 0.87x |
| users.json | msgspec | 2.956 | 3.045 | 3.196 | 67.953 | 0.70x |
| users.json | ujson | 10.838 | 10.914 | 11.238 | 67.953 | 0.19x |
| users.json | json | 15.360 | 15.503 | 15.728 | 67.953 | 0.14x |
| flat.json | strata | 0.308 | 0.328 | 0.398 | 69.234 | 1.00x |
| flat.json | orjson | 0.358 | 0.379 | 0.415 | 69.234 | 0.87x |
| flat.json | msgspec | 0.418 | 0.436 | 0.519 | 69.234 | 0.75x |
| flat.json | ujson | 1.018 | 1.044 | 1.154 | 69.234 | 0.31x |
| flat.json | json | 1.385 | 1.421 | 1.660 | 69.234 | 0.23x |
| nested.json | strata | 0.277 | 0.296 | 0.316 | 69.250 | 1.00x |
| nested.json | orjson | 0.328 | 0.336 | 0.465 | 69.250 | 0.88x |
| nested.json | msgspec | 0.395 | 0.413 | 0.468 | 69.250 | 0.72x |
| nested.json | ujson | 1.109 | 1.130 | 1.162 | 69.250 | 0.26x |
| nested.json | json | 1.702 | 1.738 | 1.815 | 69.250 | 0.17x |
| wide_arrays.json | strata | 1.363 | 1.375 | 1.505 | 69.250 | 1.00x |
| wide_arrays.json | orjson | 1.386 | 1.417 | 1.533 | 69.250 | 0.97x |
| wide_arrays.json | msgspec | 2.174 | 2.242 | 2.398 | 69.250 | 0.61x |
| wide_arrays.json | ujson | 6.274 | 6.348 | 6.568 | 69.250 | 0.22x |
| wide_arrays.json | json | 10.940 | 11.137 | 11.332 | 69.250 | 0.12x |
| mixed.json | strata | 0.142 | 0.152 | 0.193 | 69.281 | 1.00x |
| mixed.json | orjson | 0.139 | 0.154 | 0.216 | 69.281 | 0.99x |
| mixed.json | msgspec | 0.157 | 0.174 | 0.224 | 69.281 | 0.87x |
| mixed.json | ujson | 0.336 | 0.354 | 0.383 | 69.281 | 0.43x |
| mixed.json | json | 0.449 | 0.473 | 0.490 | 69.281 | 0.32x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.055 | 0.065 | 0.081 | 68.156 | 1.00x |
| users.json $[*].id | jmespath | 0.333 | 0.347 | 0.389 | 68.156 | 0.19x |
| users.json $[*].id | jsonpath-ng | 3.378 | 3.530 | 3.684 | 68.156 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.348 | 0.369 | 0.466 | 68.469 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.053 | 2.093 | 2.309 | 68.469 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 11.169 | 11.526 | 12.937 | 68.469 | 0.03x |
| users.json $..total | strata | 1.359 | 1.375 | 1.461 | 69.594 | 1.00x |
| users.json $..total | jsonpath-ng | 207.720 | 208.271 | 214.983 | 69.594 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.224 | 3.264 | 3.384 | 68.219 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.739 | 10.832 | 11.776 | 68.219 | 0.30x |
| users.json $[*].id | orjson+jsonpath-ng | 11.880 | 12.042 | 12.601 | 68.219 | 0.27x |
| users.json $[*].orders[*].total | strata | 3.317 | 3.394 | 3.536 | 69.547 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.486 | 12.658 | 13.359 | 69.547 | 0.27x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 19.974 | 20.215 | 21.538 | 69.547 | 0.17x |
| users.json $..total | strata | 10.531 | 10.675 | 11.056 | 69.594 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 215.647 | 216.163 | 217.136 | 69.594 | 0.05x |

