# Benchmark results - small

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
| users.json | strata | 7.451 | 7.609 | 7.906 | 52.547 | 1.00x |
| users.json | orjson | 7.919 | 8.371 | 8.698 | 52.547 | 0.91x |
| users.json | msgspec | 8.059 | 8.615 | 8.892 | 52.547 | 0.88x |
| users.json | ujson | 11.201 | 11.593 | 12.047 | 52.547 | 0.66x |
| users.json | json | 15.276 | 15.958 | 16.316 | 52.547 | 0.48x |
| flat.json | strata | 0.678 | 0.696 | 0.746 | 73.203 | 1.00x |
| flat.json | orjson | 0.656 | 0.672 | 0.819 | 73.203 | 1.04x |
| flat.json | msgspec | 0.666 | 0.694 | 0.766 | 73.203 | 1.00x |
| flat.json | ujson | 1.014 | 1.062 | 1.186 | 73.203 | 0.66x |
| flat.json | json | 1.416 | 1.485 | 1.547 | 73.203 | 0.47x |
| nested.json | strata | 0.606 | 0.625 | 0.682 | 73.219 | 1.00x |
| nested.json | orjson | 0.606 | 0.615 | 0.660 | 73.219 | 1.02x |
| nested.json | msgspec | 0.616 | 0.637 | 0.666 | 73.219 | 0.98x |
| nested.json | ujson | 0.890 | 0.941 | 1.157 | 73.219 | 0.66x |
| nested.json | json | 1.447 | 1.502 | 1.746 | 73.219 | 0.42x |
| wide_arrays.json | strata | 3.362 | 3.478 | 3.803 | 74.312 | 1.00x |
| wide_arrays.json | orjson | 3.269 | 3.399 | 3.812 | 74.312 | 1.02x |
| wide_arrays.json | msgspec | 3.963 | 4.233 | 4.608 | 74.312 | 0.82x |
| wide_arrays.json | ujson | 5.305 | 5.588 | 5.990 | 74.312 | 0.62x |
| wide_arrays.json | json | 7.813 | 8.131 | 8.387 | 74.312 | 0.43x |
| mixed.json | strata | 0.144 | 0.153 | 0.188 | 74.328 | 1.00x |
| mixed.json | orjson | 0.129 | 0.139 | 0.150 | 74.328 | 1.10x |
| mixed.json | msgspec | 0.143 | 0.156 | 0.188 | 74.328 | 0.98x |
| mixed.json | ujson | 0.183 | 0.191 | 0.219 | 74.328 | 0.80x |
| mixed.json | json | 0.308 | 0.323 | 0.388 | 74.328 | 0.48x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.638 | 1.742 | 1.853 | 55.984 | 1.00x |
| users.json | orjson | 1.997 | 2.068 | 2.198 | 55.984 | 0.84x |
| users.json | msgspec | 2.635 | 2.760 | 2.920 | 55.984 | 0.63x |
| users.json | ujson | 11.010 | 11.325 | 11.667 | 55.984 | 0.15x |
| users.json | json | 16.040 | 16.384 | 16.809 | 55.984 | 0.11x |
| flat.json | strata | 0.211 | 0.216 | 0.232 | 73.219 | 1.00x |
| flat.json | orjson | 0.223 | 0.232 | 0.252 | 73.219 | 0.93x |
| flat.json | msgspec | 0.296 | 0.301 | 0.330 | 73.219 | 0.72x |
| flat.json | ujson | 0.910 | 0.921 | 1.007 | 73.219 | 0.23x |
| flat.json | json | 1.363 | 1.485 | 1.712 | 73.219 | 0.15x |
| nested.json | strata | 0.143 | 0.154 | 0.196 | 73.281 | 1.00x |
| nested.json | orjson | 0.201 | 0.208 | 0.253 | 73.281 | 0.74x |
| nested.json | msgspec | 0.269 | 0.289 | 0.340 | 73.281 | 0.53x |
| nested.json | ujson | 0.971 | 0.989 | 1.064 | 73.281 | 0.16x |
| nested.json | json | 1.630 | 1.809 | 2.011 | 73.281 | 0.09x |
| wide_arrays.json | strata | 0.987 | 1.014 | 1.155 | 74.328 | 1.00x |
| wide_arrays.json | orjson | 1.108 | 1.182 | 1.283 | 74.328 | 0.86x |
| wide_arrays.json | msgspec | 1.928 | 1.999 | 2.119 | 74.328 | 0.51x |
| wide_arrays.json | ujson | 6.050 | 6.194 | 6.504 | 74.328 | 0.16x |
| wide_arrays.json | json | 11.385 | 11.665 | 12.027 | 74.328 | 0.09x |
| mixed.json | strata | 0.040 | 0.042 | 0.054 | 74.375 | 1.00x |
| mixed.json | orjson | 0.041 | 0.045 | 0.062 | 74.375 | 0.94x |
| mixed.json | msgspec | 0.048 | 0.051 | 0.058 | 74.375 | 0.82x |
| mixed.json | ujson | 0.216 | 0.221 | 0.247 | 74.375 | 0.19x |
| mixed.json | json | 0.365 | 0.423 | 0.479 | 74.375 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.720 | 7.875 | 8.024 | 71.188 | 1.00x |
| users.json | orjson | 8.366 | 8.697 | 9.064 | 71.188 | 0.91x |
| users.json | msgspec | 8.486 | 8.804 | 9.011 | 71.188 | 0.89x |
| users.json | ujson | 11.686 | 12.128 | 12.325 | 71.188 | 0.65x |
| users.json | json | 16.116 | 16.240 | 17.003 | 71.188 | 0.48x |
| flat.json | strata | 0.754 | 0.819 | 0.881 | 73.219 | 1.00x |
| flat.json | orjson | 0.731 | 0.796 | 0.869 | 73.219 | 1.03x |
| flat.json | msgspec | 0.745 | 0.789 | 0.820 | 73.219 | 1.04x |
| flat.json | ujson | 1.145 | 1.279 | 1.372 | 73.219 | 0.64x |
| flat.json | json | 1.525 | 1.590 | 1.649 | 73.219 | 0.52x |
| nested.json | strata | 0.676 | 0.755 | 0.802 | 73.281 | 1.00x |
| nested.json | orjson | 0.684 | 0.734 | 0.849 | 73.281 | 1.03x |
| nested.json | msgspec | 0.690 | 0.781 | 0.903 | 73.281 | 0.97x |
| nested.json | ujson | 0.988 | 1.087 | 1.231 | 73.281 | 0.69x |
| nested.json | json | 1.530 | 1.695 | 1.772 | 73.281 | 0.45x |
| wide_arrays.json | strata | 3.672 | 3.737 | 3.878 | 74.328 | 1.00x |
| wide_arrays.json | orjson | 3.398 | 3.618 | 3.795 | 74.328 | 1.03x |
| wide_arrays.json | msgspec | 4.240 | 4.490 | 4.935 | 74.328 | 0.83x |
| wide_arrays.json | ujson | 5.478 | 5.885 | 6.091 | 74.328 | 0.63x |
| wide_arrays.json | json | 8.033 | 8.248 | 8.442 | 74.328 | 0.45x |
| mixed.json | strata | 0.199 | 0.235 | 0.271 | 74.375 | 1.00x |
| mixed.json | orjson | 0.185 | 0.202 | 0.228 | 74.375 | 1.17x |
| mixed.json | msgspec | 0.199 | 0.216 | 0.245 | 74.375 | 1.09x |
| mixed.json | ujson | 0.252 | 0.261 | 0.284 | 74.375 | 0.90x |
| mixed.json | json | 0.402 | 0.441 | 0.474 | 74.375 | 0.53x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 8.083 | 8.326 | 8.439 | 72.344 | 1.00x |
| users.ndjson | orjson | 10.570 | 11.038 | 11.455 | 72.344 | 0.75x |
| users.ndjson | msgspec | 10.246 | 10.967 | 11.346 | 72.344 | 0.76x |
| users.ndjson | ujson | 13.347 | 13.966 | 14.090 | 72.344 | 0.60x |
| users.ndjson | json | 18.785 | 19.397 | 19.571 | 72.344 | 0.43x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.075 | 2.141 | 8.781 | 70.188 | 1.00x |
| users.json | orjson | 2.432 | 2.538 | 4.405 | 70.188 | 0.84x |
| users.json | msgspec | 3.074 | 3.159 | 3.643 | 70.188 | 0.68x |
| users.json | ujson | 11.198 | 11.320 | 11.789 | 70.188 | 0.19x |
| users.json | json | 16.014 | 16.526 | 23.199 | 70.188 | 0.13x |
| flat.json | strata | 0.351 | 0.399 | 0.466 | 73.219 | 1.00x |
| flat.json | orjson | 0.366 | 0.413 | 0.528 | 73.219 | 0.97x |
| flat.json | msgspec | 0.405 | 0.502 | 0.569 | 73.219 | 0.79x |
| flat.json | ujson | 1.046 | 1.143 | 1.252 | 73.219 | 0.35x |
| flat.json | json | 1.504 | 1.603 | 1.759 | 73.219 | 0.25x |
| nested.json | strata | 0.305 | 0.345 | 0.525 | 73.281 | 1.00x |
| nested.json | orjson | 0.318 | 0.401 | 0.482 | 73.281 | 0.86x |
| nested.json | msgspec | 0.410 | 0.449 | 0.525 | 73.281 | 0.77x |
| nested.json | ujson | 1.146 | 1.222 | 1.341 | 73.281 | 0.28x |
| nested.json | json | 1.826 | 1.992 | 2.229 | 73.281 | 0.17x |
| wide_arrays.json | strata | 1.311 | 1.423 | 1.857 | 74.328 | 1.00x |
| wide_arrays.json | orjson | 1.454 | 1.553 | 1.949 | 74.328 | 0.92x |
| wide_arrays.json | msgspec | 2.271 | 2.408 | 2.663 | 74.328 | 0.59x |
| wide_arrays.json | ujson | 6.459 | 6.590 | 7.132 | 74.328 | 0.22x |
| wide_arrays.json | json | 11.758 | 12.301 | 12.684 | 74.328 | 0.12x |
| mixed.json | strata | 0.158 | 0.180 | 0.203 | 74.406 | 1.00x |
| mixed.json | orjson | 0.161 | 0.181 | 0.246 | 74.406 | 1.00x |
| mixed.json | msgspec | 0.158 | 0.238 | 0.278 | 74.406 | 0.76x |
| mixed.json | ujson | 0.350 | 0.389 | 0.402 | 74.406 | 0.46x |
| mixed.json | json | 0.531 | 0.562 | 0.607 | 74.406 | 0.32x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.050 | 0.060 | 0.140 | 70.375 | 1.00x |
| users.json $[*].id | jmespath | 0.278 | 0.321 | 0.367 | 70.375 | 0.19x |
| users.json $[*].id | jsonpath-ng | 1.476 | 1.563 | 1.719 | 70.375 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.344 | 0.431 | 0.649 | 70.594 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.662 | 1.850 | 2.119 | 70.594 | 0.23x |
| users.json $[*].orders[*].total | jsonpath-ng | 9.972 | 10.697 | 11.354 | 70.594 | 0.04x |
| users.json $..total | strata | 1.413 | 1.484 | 1.543 | 72.438 | 1.00x |
| users.json $..total | jsonpath-ng | 190.344 | 192.560 | 195.470 | 72.438 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.515 | 3.565 | 3.682 | 70.453 | 1.00x |
| users.json $[*].id | orjson+jmespath | 8.927 | 9.163 | 9.527 | 70.453 | 0.39x |
| users.json $[*].id | orjson+jsonpath-ng | 10.158 | 10.496 | 10.788 | 70.453 | 0.34x |
| users.json $[*].orders[*].total | strata | 3.510 | 3.638 | 3.751 | 72.266 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 10.272 | 10.838 | 11.256 | 72.266 | 0.34x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 20.593 | 21.233 | 21.567 | 72.266 | 0.17x |
| users.json $..total | strata | 9.327 | 9.544 | 9.650 | 72.438 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 200.121 | 201.741 | 203.928 | 72.438 | 0.05x |

