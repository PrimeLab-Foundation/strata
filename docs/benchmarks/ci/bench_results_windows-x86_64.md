# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 8cdb525
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD
- compiler_flags: /std:c++20 /O2 /arch:AVX2 /GL /LTCG /USEPROFILE (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.177 | 10.494 | 29.117 | 45.375 | 1.00x |
| users.json | orjson | 12.961 | 13.205 | 34.007 | 45.375 | 0.79x |
| users.json | msgspec | 12.470 | 12.851 | 30.154 | 45.375 | 0.82x |
| users.json | ujson | 20.266 | 22.244 | 74.576 | 45.375 | 0.47x |
| users.json | json | 21.907 | 23.169 | 59.528 | 45.375 | 0.45x |
| flat.json | strata | 1.209 | 1.215 | 1.244 | 49.902 | 1.00x |
| flat.json | orjson | 1.102 | 1.109 | 1.793 | 49.902 | 1.10x |
| flat.json | msgspec | 1.139 | 1.160 | 1.854 | 49.902 | 1.05x |
| flat.json | ujson | 2.113 | 2.139 | 3.337 | 49.902 | 0.57x |
| flat.json | json | 1.918 | 1.941 | 3.754 | 49.902 | 0.63x |
| nested.json | strata | 0.851 | 0.862 | 0.921 | 49.914 | 1.00x |
| nested.json | orjson | 1.047 | 1.071 | 1.108 | 49.914 | 0.80x |
| nested.json | msgspec | 0.999 | 1.025 | 1.084 | 49.914 | 0.84x |
| nested.json | ujson | 1.537 | 1.582 | 1.613 | 49.914 | 0.54x |
| nested.json | json | 2.112 | 2.116 | 2.392 | 49.914 | 0.41x |
| wide_arrays.json | strata | 5.002 | 5.127 | 5.350 | 52.070 | 1.00x |
| wide_arrays.json | orjson | 5.574 | 6.041 | 6.389 | 52.070 | 0.85x |
| wide_arrays.json | msgspec | 5.720 | 5.965 | 9.839 | 52.070 | 0.86x |
| wide_arrays.json | ujson | 8.331 | 8.462 | 13.934 | 52.070 | 0.61x |
| wide_arrays.json | json | 11.548 | 11.700 | 20.367 | 52.070 | 0.44x |
| mixed.json | strata | 0.209 | 0.231 | 0.344 | 49.977 | 1.00x |
| mixed.json | orjson | 0.216 | 0.246 | 0.282 | 49.977 | 0.94x |
| mixed.json | msgspec | 0.262 | 0.266 | 0.284 | 49.977 | 0.87x |
| mixed.json | ujson | 0.383 | 0.397 | 0.418 | 49.977 | 0.58x |
| mixed.json | json | 0.490 | 0.516 | 0.737 | 49.977 | 0.45x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.279 | 3.405 | 5.078 | 44.715 | 1.00x |
| users.json | orjson | 3.677 | 3.809 | 5.807 | 44.715 | 0.89x |
| users.json | msgspec | 5.348 | 6.072 | 8.653 | 44.715 | 0.56x |
| users.json | ujson | 16.479 | 19.551 | 29.256 | 44.715 | 0.17x |
| users.json | json | 23.086 | 23.530 | 59.091 | 44.715 | 0.14x |
| flat.json | strata | 0.316 | 0.346 | 0.394 | 50.590 | 1.00x |
| flat.json | orjson | 0.353 | 0.359 | 0.394 | 50.590 | 0.96x |
| flat.json | msgspec | 0.490 | 0.515 | 0.549 | 50.590 | 0.67x |
| flat.json | ujson | 1.696 | 1.731 | 1.788 | 50.590 | 0.20x |
| flat.json | json | 1.908 | 2.065 | 2.354 | 50.590 | 0.17x |
| nested.json | strata | 0.270 | 0.273 | 0.313 | 50.551 | 1.00x |
| nested.json | orjson | 0.322 | 0.323 | 0.361 | 50.551 | 0.85x |
| nested.json | msgspec | 0.458 | 0.462 | 0.534 | 50.551 | 0.59x |
| nested.json | ujson | 1.260 | 1.309 | 1.397 | 50.551 | 0.21x |
| nested.json | json | 2.408 | 2.431 | 2.604 | 50.551 | 0.11x |
| wide_arrays.json | strata | 2.389 | 2.420 | 3.658 | 51.457 | 1.00x |
| wide_arrays.json | orjson | 2.476 | 2.514 | 3.621 | 51.457 | 0.96x |
| wide_arrays.json | msgspec | 3.714 | 3.918 | 5.831 | 51.457 | 0.62x |
| wide_arrays.json | ujson | 9.548 | 9.627 | 10.955 | 51.457 | 0.25x |
| wide_arrays.json | json | 18.640 | 18.937 | 28.885 | 51.457 | 0.13x |
| mixed.json | strata | 0.078 | 0.079 | 0.108 | 50.082 | 1.00x |
| mixed.json | orjson | 0.068 | 0.069 | 0.152 | 50.082 | 1.14x |
| mixed.json | msgspec | 0.091 | 0.092 | 0.138 | 50.082 | 0.85x |
| mixed.json | ujson | 0.319 | 0.330 | 0.542 | 50.082 | 0.24x |
| mixed.json | json | 0.496 | 0.527 | 0.912 | 50.082 | 0.15x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 12.823 | 13.057 | 13.395 | 54.090 | 1.00x |
| users.json | orjson | 13.916 | 14.319 | 14.596 | 54.090 | 0.91x |
| users.json | msgspec | 13.443 | 13.923 | 23.171 | 54.090 | 0.94x |
| users.json | ujson | 25.692 | 26.561 | 31.014 | 54.090 | 0.49x |
| users.json | json | 22.966 | 23.356 | 25.123 | 54.090 | 0.56x |
| flat.json | strata | 1.413 | 1.473 | 1.563 | 50.148 | 1.00x |
| flat.json | orjson | 1.380 | 1.421 | 1.463 | 50.148 | 1.04x |
| flat.json | msgspec | 1.262 | 1.317 | 1.505 | 50.148 | 1.12x |
| flat.json | ujson | 2.712 | 2.749 | 2.802 | 50.148 | 0.54x |
| flat.json | json | 2.072 | 2.081 | 2.118 | 50.148 | 0.71x |
| nested.json | strata | 0.973 | 1.020 | 1.865 | 50.168 | 1.00x |
| nested.json | orjson | 1.171 | 1.199 | 1.812 | 50.168 | 0.85x |
| nested.json | msgspec | 1.122 | 1.164 | 1.860 | 50.168 | 0.88x |
| nested.json | ujson | 1.970 | 2.000 | 3.544 | 50.168 | 0.51x |
| nested.json | json | 2.225 | 2.240 | 4.025 | 50.168 | 0.46x |
| wide_arrays.json | strata | 6.294 | 6.460 | 14.650 | 50.891 | 1.00x |
| wide_arrays.json | orjson | 6.030 | 6.099 | 9.416 | 50.891 | 1.06x |
| wide_arrays.json | msgspec | 6.250 | 6.365 | 10.577 | 50.891 | 1.01x |
| wide_arrays.json | ujson | 11.358 | 11.605 | 16.880 | 50.891 | 0.56x |
| wide_arrays.json | json | 12.043 | 12.179 | 13.202 | 50.891 | 0.53x |
| mixed.json | strata | 0.275 | 0.293 | 0.434 | 50.082 | 1.00x |
| mixed.json | orjson | 0.319 | 0.339 | 0.503 | 50.082 | 0.87x |
| mixed.json | msgspec | 0.339 | 0.355 | 0.531 | 50.082 | 0.83x |
| mixed.json | ujson | 0.536 | 0.559 | 0.972 | 50.082 | 0.53x |
| mixed.json | json | 0.573 | 0.597 | 0.978 | 50.082 | 0.49x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 12.821 | 13.398 | 22.702 | 50.984 | 1.00x |
| users.ndjson | orjson | 16.510 | 17.227 | 19.607 | 50.984 | 0.78x |
| users.ndjson | msgspec | 16.731 | 17.175 | 22.822 | 50.984 | 0.78x |
| users.ndjson | ujson | 24.909 | 25.483 | 37.025 | 50.984 | 0.53x |
| users.ndjson | json | 29.714 | 30.276 | 36.345 | 50.984 | 0.44x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.136 | 4.214 | 6.232 | 50.895 | 1.00x |
| users.json | orjson | 4.486 | 4.627 | 6.796 | 50.895 | 0.91x |
| users.json | msgspec | 6.134 | 6.273 | 9.818 | 50.895 | 0.67x |
| users.json | ujson | 25.389 | 25.829 | 31.888 | 50.895 | 0.16x |
| users.json | json | 32.092 | 32.343 | 32.932 | 50.895 | 0.13x |
| flat.json | strata | 0.666 | 0.709 | 0.938 | 50.422 | 1.00x |
| flat.json | orjson | 0.706 | 0.772 | 0.989 | 50.422 | 0.92x |
| flat.json | msgspec | 0.840 | 0.885 | 1.236 | 50.422 | 0.80x |
| flat.json | ujson | 3.043 | 3.106 | 5.083 | 50.422 | 0.23x |
| flat.json | json | 3.254 | 3.359 | 5.581 | 50.422 | 0.21x |
| nested.json | strata | 0.605 | 0.671 | 0.959 | 50.543 | 1.00x |
| nested.json | orjson | 0.659 | 0.705 | 0.790 | 50.543 | 0.95x |
| nested.json | msgspec | 0.796 | 0.804 | 0.836 | 50.543 | 0.83x |
| nested.json | ujson | 2.350 | 2.405 | 2.468 | 50.543 | 0.28x |
| nested.json | json | 3.449 | 3.514 | 6.669 | 50.543 | 0.19x |
| wide_arrays.json | strata | 3.059 | 3.129 | 9.225 | 50.938 | 1.00x |
| wide_arrays.json | orjson | 2.962 | 3.167 | 3.472 | 50.938 | 0.99x |
| wide_arrays.json | msgspec | 4.073 | 4.437 | 5.960 | 50.938 | 0.71x |
| wide_arrays.json | ujson | 16.248 | 16.415 | 27.473 | 50.938 | 0.19x |
| wide_arrays.json | json | 24.922 | 25.153 | 28.248 | 50.938 | 0.12x |
| mixed.json | strata | 0.384 | 0.388 | 0.515 | 50.113 | 1.00x |
| mixed.json | orjson | 0.374 | 0.387 | 0.518 | 50.113 | 1.00x |
| mixed.json | msgspec | 0.396 | 0.430 | 0.547 | 50.113 | 0.90x |
| mixed.json | ujson | 0.802 | 0.863 | 0.906 | 50.113 | 0.45x |
| mixed.json | json | 0.974 | 0.986 | 1.048 | 50.113 | 0.39x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.083 | 0.086 | 0.139 | 50.922 | 1.00x |
| users.json $[*].id | jmespath | 0.434 | 0.439 | 0.447 | 50.922 | 0.20x |
| users.json $[*].id | jsonpath-ng | 2.471 | 2.582 | 2.830 | 50.922 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.449 | 0.486 | 0.798 | 50.457 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.742 | 2.792 | 3.255 | 50.457 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 16.692 | 17.298 | 30.717 | 50.457 | 0.03x |
| users.json $..total | strata | 1.781 | 1.828 | 1.874 | 52.484 | 1.00x |
| users.json $..total | jsonpath-ng | 333.238 | 357.855 | 371.974 | 52.484 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 5.358 | 5.431 | 8.598 | 50.305 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.575 | 14.720 | 22.503 | 50.305 | 0.37x |
| users.json $[*].id | orjson+jsonpath-ng | 16.562 | 16.994 | 25.330 | 50.305 | 0.32x |
| users.json $[*].orders[*].total | strata | 5.397 | 5.464 | 8.736 | 52.402 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.084 | 17.498 | 18.309 | 52.402 | 0.31x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 34.352 | 34.860 | 54.492 | 52.402 | 0.16x |
| users.json $..total | strata | 15.058 | 15.709 | 15.995 | 52.445 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 351.084 | 375.131 | 378.672 | 52.445 | 0.04x |

