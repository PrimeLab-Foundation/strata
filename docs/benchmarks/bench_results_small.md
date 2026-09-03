# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 81e4c89
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
| users.json | strata | 6.820 | 6.910 | 7.401 | 52.984 | 1.00x |
| users.json | orjson | 7.947 | 8.067 | 8.510 | 52.984 | 0.86x |
| users.json | msgspec | 8.072 | 8.193 | 8.581 | 52.984 | 0.84x |
| users.json | ujson | 10.985 | 11.127 | 11.840 | 52.984 | 0.62x |
| users.json | json | 15.445 | 15.641 | 16.279 | 52.984 | 0.44x |
| flat.json | strata | 0.582 | 0.590 | 0.765 | 70.500 | 1.00x |
| flat.json | orjson | 0.634 | 0.642 | 0.731 | 70.500 | 0.92x |
| flat.json | msgspec | 0.671 | 0.683 | 0.723 | 70.500 | 0.86x |
| flat.json | ujson | 0.999 | 1.003 | 1.094 | 70.500 | 0.59x |
| flat.json | json | 1.407 | 1.416 | 1.480 | 70.500 | 0.42x |
| nested.json | strata | 0.527 | 0.532 | 0.593 | 70.516 | 1.00x |
| nested.json | orjson | 0.603 | 0.608 | 0.654 | 70.516 | 0.88x |
| nested.json | msgspec | 0.605 | 0.613 | 0.687 | 70.516 | 0.87x |
| nested.json | ujson | 0.882 | 0.893 | 1.022 | 70.516 | 0.60x |
| nested.json | json | 1.395 | 1.405 | 1.550 | 70.516 | 0.38x |
| wide_arrays.json | strata | 3.095 | 3.116 | 3.175 | 70.781 | 1.00x |
| wide_arrays.json | orjson | 3.156 | 3.178 | 3.300 | 70.781 | 0.98x |
| wide_arrays.json | msgspec | 3.879 | 3.923 | 3.975 | 70.781 | 0.79x |
| wide_arrays.json | ujson | 5.086 | 5.126 | 5.285 | 70.781 | 0.61x |
| wide_arrays.json | json | 7.477 | 7.530 | 7.678 | 70.781 | 0.41x |
| mixed.json | strata | 0.131 | 0.134 | 0.147 | 70.781 | 1.00x |
| mixed.json | orjson | 0.126 | 0.130 | 0.138 | 70.781 | 1.03x |
| mixed.json | msgspec | 0.140 | 0.143 | 0.149 | 70.781 | 0.94x |
| mixed.json | ujson | 0.181 | 0.184 | 0.189 | 70.781 | 0.73x |
| mixed.json | json | 0.303 | 0.307 | 0.312 | 70.781 | 0.44x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.446 | 1.455 | 1.627 | 55.625 | 1.00x |
| users.json | orjson | 2.018 | 2.023 | 2.077 | 55.625 | 0.72x |
| users.json | msgspec | 2.656 | 2.667 | 2.695 | 55.625 | 0.55x |
| users.json | ujson | 10.904 | 10.959 | 10.989 | 55.625 | 0.13x |
| users.json | json | 15.640 | 15.677 | 16.218 | 55.625 | 0.09x |
| flat.json | strata | 0.177 | 0.178 | 0.184 | 70.516 | 1.00x |
| flat.json | orjson | 0.223 | 0.225 | 0.231 | 70.516 | 0.79x |
| flat.json | msgspec | 0.293 | 0.294 | 0.296 | 70.516 | 0.60x |
| flat.json | ujson | 0.887 | 0.889 | 0.917 | 70.516 | 0.20x |
| flat.json | json | 1.325 | 1.343 | 1.354 | 70.516 | 0.13x |
| nested.json | strata | 0.134 | 0.138 | 0.147 | 70.609 | 1.00x |
| nested.json | orjson | 0.197 | 0.202 | 0.215 | 70.609 | 0.68x |
| nested.json | msgspec | 0.268 | 0.274 | 0.286 | 70.609 | 0.50x |
| nested.json | ujson | 0.955 | 0.964 | 1.017 | 70.609 | 0.14x |
| nested.json | json | 1.638 | 1.653 | 1.868 | 70.609 | 0.08x |
| wide_arrays.json | strata | 0.913 | 0.917 | 0.944 | 70.781 | 1.00x |
| wide_arrays.json | orjson | 1.107 | 1.110 | 1.114 | 70.781 | 0.83x |
| wide_arrays.json | msgspec | 1.936 | 1.947 | 2.037 | 70.781 | 0.47x |
| wide_arrays.json | ujson | 5.943 | 5.976 | 6.235 | 70.781 | 0.15x |
| wide_arrays.json | json | 11.023 | 11.156 | 11.305 | 70.781 | 0.08x |
| mixed.json | strata | 0.035 | 0.035 | 0.054 | 70.859 | 1.00x |
| mixed.json | orjson | 0.042 | 0.043 | 0.046 | 70.859 | 0.82x |
| mixed.json | msgspec | 0.048 | 0.049 | 0.058 | 70.859 | 0.72x |
| mixed.json | ujson | 0.213 | 0.215 | 0.217 | 70.859 | 0.17x |
| mixed.json | json | 0.346 | 0.351 | 0.357 | 70.859 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.130 | 7.228 | 7.506 | 69.016 | 1.00x |
| users.json | orjson | 8.426 | 8.528 | 9.138 | 69.016 | 0.85x |
| users.json | msgspec | 8.541 | 8.681 | 8.986 | 69.016 | 0.83x |
| users.json | ujson | 11.716 | 11.870 | 12.179 | 69.016 | 0.61x |
| users.json | json | 15.929 | 16.213 | 16.548 | 69.016 | 0.45x |
| flat.json | strata | 0.656 | 0.661 | 0.706 | 70.516 | 1.00x |
| flat.json | orjson | 0.717 | 0.725 | 0.746 | 70.516 | 0.91x |
| flat.json | msgspec | 0.750 | 0.760 | 0.769 | 70.516 | 0.87x |
| flat.json | ujson | 1.092 | 1.126 | 1.135 | 70.516 | 0.59x |
| flat.json | json | 1.484 | 1.493 | 1.501 | 70.516 | 0.44x |
| nested.json | strata | 0.594 | 0.604 | 0.705 | 70.781 | 1.00x |
| nested.json | orjson | 0.675 | 0.692 | 0.725 | 70.781 | 0.87x |
| nested.json | msgspec | 0.676 | 0.692 | 0.711 | 70.781 | 0.87x |
| nested.json | ujson | 0.985 | 1.007 | 1.071 | 70.781 | 0.60x |
| nested.json | json | 1.475 | 1.489 | 1.615 | 70.781 | 0.41x |
| wide_arrays.json | strata | 3.276 | 3.303 | 3.396 | 70.781 | 1.00x |
| wide_arrays.json | orjson | 3.362 | 3.418 | 3.436 | 70.781 | 0.97x |
| wide_arrays.json | msgspec | 4.108 | 4.201 | 4.250 | 70.781 | 0.79x |
| wide_arrays.json | ujson | 5.425 | 5.481 | 5.816 | 70.781 | 0.60x |
| wide_arrays.json | json | 7.706 | 7.745 | 8.401 | 70.781 | 0.43x |
| mixed.json | strata | 0.177 | 0.182 | 0.236 | 70.859 | 1.00x |
| mixed.json | orjson | 0.175 | 0.185 | 0.251 | 70.859 | 0.99x |
| mixed.json | msgspec | 0.189 | 0.204 | 0.220 | 70.859 | 0.89x |
| mixed.json | ujson | 0.242 | 0.251 | 0.279 | 70.859 | 0.73x |
| mixed.json | json | 0.354 | 0.362 | 0.379 | 70.859 | 0.50x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.211 | 7.300 | 7.416 | 69.641 | 1.00x |
| users.ndjson | orjson | 10.090 | 10.165 | 10.498 | 69.641 | 0.72x |
| users.ndjson | msgspec | 9.924 | 10.132 | 10.449 | 69.641 | 0.72x |
| users.ndjson | ujson | 12.777 | 13.075 | 13.193 | 69.641 | 0.56x |
| users.ndjson | json | 18.010 | 18.264 | 18.444 | 69.641 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.829 | 1.900 | 2.124 | 67.016 | 1.00x |
| users.json | orjson | 2.448 | 2.537 | 2.782 | 67.016 | 0.75x |
| users.json | msgspec | 3.075 | 3.174 | 3.448 | 67.016 | 0.60x |
| users.json | ujson | 11.080 | 11.168 | 11.895 | 67.016 | 0.17x |
| users.json | json | 16.136 | 16.358 | 17.201 | 67.016 | 0.12x |
| flat.json | strata | 0.325 | 0.341 | 0.359 | 70.516 | 1.00x |
| flat.json | orjson | 0.385 | 0.404 | 0.422 | 70.516 | 0.84x |
| flat.json | msgspec | 0.446 | 0.459 | 0.488 | 70.516 | 0.74x |
| flat.json | ujson | 1.072 | 1.080 | 1.138 | 70.516 | 0.32x |
| flat.json | json | 1.498 | 1.518 | 1.585 | 70.516 | 0.22x |
| nested.json | strata | 0.300 | 0.319 | 0.340 | 70.781 | 1.00x |
| nested.json | orjson | 0.361 | 0.382 | 0.402 | 70.781 | 0.83x |
| nested.json | msgspec | 0.412 | 0.460 | 0.705 | 70.781 | 0.69x |
| nested.json | ujson | 1.137 | 1.188 | 1.268 | 70.781 | 0.27x |
| nested.json | json | 1.861 | 1.896 | 1.946 | 70.781 | 0.17x |
| wide_arrays.json | strata | 1.255 | 1.313 | 1.352 | 70.781 | 1.00x |
| wide_arrays.json | orjson | 1.438 | 1.505 | 1.569 | 70.781 | 0.87x |
| wide_arrays.json | msgspec | 2.332 | 2.374 | 2.431 | 70.781 | 0.55x |
| wide_arrays.json | ujson | 6.379 | 6.434 | 6.522 | 70.781 | 0.20x |
| wide_arrays.json | json | 11.483 | 11.543 | 11.641 | 70.781 | 0.11x |
| mixed.json | strata | 0.160 | 0.178 | 0.188 | 70.891 | 1.00x |
| mixed.json | orjson | 0.175 | 0.188 | 0.210 | 70.891 | 0.95x |
| mixed.json | msgspec | 0.181 | 0.199 | 0.223 | 70.891 | 0.89x |
| mixed.json | ujson | 0.357 | 0.363 | 0.387 | 70.891 | 0.49x |
| mixed.json | json | 0.486 | 0.507 | 0.558 | 70.891 | 0.35x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.052 | 0.060 | 0.151 | 67.344 | 1.00x |
| users.json $[*].id | jmespath | 0.283 | 0.294 | 0.366 | 67.344 | 0.20x |
| users.json $[*].id | jsonpath-ng | 1.440 | 1.476 | 1.702 | 67.344 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.326 | 0.330 | 0.355 | 67.625 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.639 | 1.644 | 1.655 | 67.625 | 0.20x |
| users.json $[*].orders[*].total | jsonpath-ng | 9.471 | 9.499 | 9.556 | 67.625 | 0.03x |
| users.json $..total | strata | 1.391 | 1.400 | 1.441 | 68.703 | 1.00x |
| users.json $..total | jsonpath-ng | 184.405 | 186.446 | 188.272 | 68.703 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.395 | 3.430 | 3.491 | 67.375 | 1.00x |
| users.json $[*].id | orjson+jmespath | 8.497 | 8.563 | 8.841 | 67.375 | 0.40x |
| users.json $[*].id | orjson+jsonpath-ng | 9.701 | 9.773 | 10.015 | 67.375 | 0.35x |
| users.json $[*].orders[*].total | strata | 3.441 | 3.507 | 3.719 | 68.641 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 9.889 | 9.991 | 10.069 | 68.641 | 0.35x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 18.983 | 19.160 | 19.378 | 68.641 | 0.18x |
| users.json $..total | strata | 8.424 | 8.533 | 8.653 | 67.844 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 194.987 | 196.170 | 197.910 | 67.844 | 0.04x |

