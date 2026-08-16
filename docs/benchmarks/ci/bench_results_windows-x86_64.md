# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 04160cb
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 25 Model 17 Stepping 1, AuthenticAMD
- compiler_flags: /std:c++20 /O2 /arch:AVX2 /GL /LTCG /USEPROFILE (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 12.444 | 18.357 | 33.030 | 45.527 | 1.00x |
| users.json | orjson | 15.807 | 19.206 | 79.808 | 45.527 | 0.96x |
| users.json | msgspec | 15.785 | 20.887 | 32.645 | 45.527 | 0.88x |
| users.json | ujson | 28.118 | 38.822 | 78.962 | 45.527 | 0.47x |
| users.json | json | 25.547 | 30.401 | 53.235 | 45.527 | 0.60x |
| flat.json | strata | 1.182 | 1.283 | 2.006 | 48.801 | 1.00x |
| flat.json | orjson | 1.498 | 1.602 | 2.193 | 48.801 | 0.80x |
| flat.json | msgspec | 1.265 | 1.360 | 1.835 | 48.801 | 0.94x |
| flat.json | ujson | 2.415 | 2.604 | 3.824 | 48.801 | 0.49x |
| flat.json | json | 1.986 | 2.145 | 3.574 | 48.801 | 0.60x |
| nested.json | strata | 0.897 | 1.024 | 2.246 | 48.516 | 1.00x |
| nested.json | orjson | 1.111 | 1.161 | 2.002 | 48.516 | 0.88x |
| nested.json | msgspec | 0.992 | 1.037 | 1.576 | 48.516 | 0.99x |
| nested.json | ujson | 1.734 | 1.808 | 3.552 | 48.516 | 0.57x |
| nested.json | json | 2.144 | 2.291 | 4.585 | 48.516 | 0.45x |
| wide_arrays.json | strata | 5.612 | 6.048 | 6.491 | 50.516 | 1.00x |
| wide_arrays.json | orjson | 6.798 | 7.304 | 8.726 | 50.516 | 0.83x |
| wide_arrays.json | msgspec | 6.398 | 6.844 | 12.199 | 50.516 | 0.88x |
| wide_arrays.json | ujson | 8.902 | 9.399 | 14.679 | 50.516 | 0.64x |
| wide_arrays.json | json | 12.280 | 12.991 | 15.147 | 50.516 | 0.47x |
| mixed.json | strata | 0.214 | 0.222 | 0.362 | 48.402 | 1.00x |
| mixed.json | orjson | 0.223 | 0.238 | 0.317 | 48.402 | 0.94x |
| mixed.json | msgspec | 0.238 | 0.247 | 0.405 | 48.402 | 0.90x |
| mixed.json | ujson | 0.362 | 0.375 | 0.421 | 48.402 | 0.59x |
| mixed.json | json | 0.483 | 0.498 | 0.537 | 48.402 | 0.45x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.130 | 4.858 | 5.588 | 44.117 | 1.00x |
| users.json | orjson | 4.012 | 5.592 | 6.308 | 44.117 | 0.87x |
| users.json | msgspec | 6.830 | 8.237 | 47.674 | 44.117 | 0.59x |
| users.json | ujson | 21.004 | 24.770 | 27.323 | 44.117 | 0.20x |
| users.json | json | 34.505 | 39.438 | 42.506 | 44.117 | 0.12x |
| flat.json | strata | 0.357 | 0.387 | 0.612 | 49.180 | 1.00x |
| flat.json | orjson | 0.381 | 0.391 | 0.529 | 49.180 | 0.99x |
| flat.json | msgspec | 0.562 | 0.588 | 0.727 | 49.180 | 0.66x |
| flat.json | ujson | 1.705 | 1.787 | 1.932 | 49.180 | 0.22x |
| flat.json | json | 2.007 | 2.182 | 3.300 | 49.180 | 0.18x |
| nested.json | strata | 0.285 | 0.297 | 0.471 | 49.031 | 1.00x |
| nested.json | orjson | 0.331 | 0.354 | 0.549 | 49.031 | 0.84x |
| nested.json | msgspec | 0.523 | 0.590 | 0.754 | 49.031 | 0.50x |
| nested.json | ujson | 1.337 | 1.389 | 2.165 | 49.031 | 0.21x |
| nested.json | json | 2.488 | 2.852 | 4.577 | 49.031 | 0.10x |
| wide_arrays.json | strata | 2.497 | 2.580 | 3.059 | 49.340 | 1.00x |
| wide_arrays.json | orjson | 2.460 | 2.832 | 2.978 | 49.340 | 0.91x |
| wide_arrays.json | msgspec | 4.023 | 4.440 | 4.708 | 49.340 | 0.58x |
| wide_arrays.json | ujson | 10.095 | 10.291 | 14.291 | 49.340 | 0.25x |
| wide_arrays.json | json | 18.848 | 19.544 | 42.212 | 49.340 | 0.13x |
| mixed.json | strata | 0.083 | 0.088 | 0.115 | 48.578 | 1.00x |
| mixed.json | orjson | 0.075 | 0.081 | 0.085 | 48.578 | 1.08x |
| mixed.json | msgspec | 0.108 | 0.118 | 0.166 | 48.578 | 0.74x |
| mixed.json | ujson | 0.334 | 0.346 | 0.482 | 48.578 | 0.25x |
| mixed.json | json | 0.531 | 0.544 | 1.690 | 48.578 | 0.16x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 17.135 | 21.436 | 25.622 | 54.141 | 1.00x |
| users.json | orjson | 20.155 | 23.415 | 50.505 | 54.141 | 0.92x |
| users.json | msgspec | 18.319 | 23.157 | 25.847 | 54.141 | 0.93x |
| users.json | ujson | 34.272 | 39.931 | 46.268 | 54.141 | 0.54x |
| users.json | json | 23.965 | 38.619 | 46.852 | 54.141 | 0.56x |
| flat.json | strata | 1.625 | 1.722 | 1.901 | 48.809 | 1.00x |
| flat.json | orjson | 1.753 | 1.921 | 2.017 | 48.809 | 0.90x |
| flat.json | msgspec | 1.408 | 1.514 | 1.653 | 48.809 | 1.14x |
| flat.json | ujson | 2.908 | 3.120 | 3.957 | 48.809 | 0.55x |
| flat.json | json | 2.165 | 2.270 | 2.599 | 48.809 | 0.76x |
| nested.json | strata | 1.045 | 1.116 | 1.273 | 48.543 | 1.00x |
| nested.json | orjson | 1.306 | 1.459 | 2.111 | 48.543 | 0.76x |
| nested.json | msgspec | 1.175 | 1.308 | 1.521 | 48.543 | 0.85x |
| nested.json | ujson | 1.975 | 2.107 | 2.376 | 48.543 | 0.53x |
| nested.json | json | 2.319 | 2.394 | 2.746 | 48.543 | 0.47x |
| wide_arrays.json | strata | 7.355 | 7.474 | 7.722 | 50.312 | 1.00x |
| wide_arrays.json | orjson | 6.947 | 7.401 | 11.075 | 50.312 | 1.01x |
| wide_arrays.json | msgspec | 7.166 | 7.549 | 11.625 | 50.312 | 0.99x |
| wide_arrays.json | ujson | 11.402 | 11.920 | 16.446 | 50.312 | 0.63x |
| wide_arrays.json | json | 12.953 | 13.110 | 13.731 | 50.312 | 0.57x |
| mixed.json | strata | 0.295 | 0.306 | 0.452 | 48.395 | 1.00x |
| mixed.json | orjson | 0.430 | 0.484 | 0.613 | 48.395 | 0.63x |
| mixed.json | msgspec | 0.380 | 0.389 | 0.541 | 48.395 | 0.79x |
| mixed.json | ujson | 0.556 | 0.577 | 0.732 | 48.395 | 0.53x |
| mixed.json | json | 0.600 | 0.658 | 1.390 | 48.395 | 0.47x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 16.128 | 17.777 | 23.387 | 49.723 | 1.00x |
| users.ndjson | orjson | 21.108 | 22.690 | 41.112 | 49.723 | 0.78x |
| users.ndjson | msgspec | 20.044 | 21.920 | 43.727 | 49.723 | 0.81x |
| users.ndjson | ujson | 30.066 | 31.424 | 34.873 | 49.723 | 0.57x |
| users.ndjson | json | 33.881 | 36.059 | 53.212 | 49.723 | 0.49x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 5.122 | 6.397 | 6.868 | 52.207 | 1.00x |
| users.json | orjson | 5.254 | 7.197 | 7.654 | 52.207 | 0.89x |
| users.json | msgspec | 7.935 | 9.077 | 10.520 | 52.207 | 0.70x |
| users.json | ujson | 34.666 | 38.515 | 48.862 | 52.207 | 0.17x |
| users.json | json | 41.369 | 53.115 | 58.294 | 52.207 | 0.12x |
| flat.json | strata | 0.819 | 0.946 | 1.748 | 49.199 | 1.00x |
| flat.json | orjson | 0.778 | 0.890 | 1.547 | 49.199 | 1.06x |
| flat.json | msgspec | 1.004 | 1.138 | 2.059 | 49.199 | 0.83x |
| flat.json | ujson | 3.083 | 3.278 | 5.219 | 49.199 | 0.29x |
| flat.json | json | 3.503 | 3.744 | 6.442 | 49.199 | 0.25x |
| nested.json | strata | 0.719 | 0.831 | 1.109 | 49.016 | 1.00x |
| nested.json | orjson | 0.760 | 0.872 | 14.055 | 49.016 | 0.95x |
| nested.json | msgspec | 0.950 | 1.042 | 1.824 | 49.016 | 0.80x |
| nested.json | ujson | 2.566 | 2.656 | 2.847 | 49.016 | 0.31x |
| nested.json | json | 3.703 | 3.888 | 6.798 | 49.016 | 0.21x |
| wide_arrays.json | strata | 3.451 | 3.496 | 4.170 | 49.355 | 1.00x |
| wide_arrays.json | orjson | 3.258 | 3.658 | 3.993 | 49.355 | 0.96x |
| wide_arrays.json | msgspec | 4.948 | 5.159 | 19.444 | 49.355 | 0.68x |
| wide_arrays.json | ujson | 17.051 | 17.548 | 47.188 | 49.355 | 0.20x |
| wide_arrays.json | json | 25.891 | 26.426 | 29.645 | 49.355 | 0.13x |
| mixed.json | strata | 0.417 | 0.461 | 0.565 | 48.457 | 1.00x |
| mixed.json | orjson | 0.439 | 0.479 | 0.887 | 48.457 | 0.96x |
| mixed.json | msgspec | 0.452 | 0.488 | 0.568 | 48.457 | 0.94x |
| mixed.json | ujson | 0.846 | 0.907 | 1.272 | 48.457 | 0.51x |
| mixed.json | json | 1.045 | 1.137 | 1.691 | 48.457 | 0.41x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.113 | 0.161 | 0.197 | 52.246 | 1.00x |
| users.json $[*].id | jmespath | 0.450 | 0.777 | 0.868 | 52.246 | 0.21x |
| users.json $[*].id | jsonpath-ng | 4.170 | 4.423 | 5.004 | 52.246 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.865 | 0.903 | 0.973 | 51.832 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 4.190 | 4.825 | 6.022 | 51.832 | 0.19x |
| users.json $[*].orders[*].total | jsonpath-ng | 26.610 | 30.059 | 38.803 | 51.832 | 0.03x |
| users.json $..total | strata | 1.880 | 2.025 | 3.376 | 52.828 | 1.00x |
| users.json $..total | jsonpath-ng | 335.725 | 361.244 | 514.341 | 52.828 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 6.574 | 8.034 | 9.119 | 51.684 | 1.00x |
| users.json $[*].id | orjson+jmespath | 19.754 | 24.623 | 27.147 | 51.684 | 0.33x |
| users.json $[*].id | orjson+jsonpath-ng | 26.374 | 28.598 | 31.203 | 51.684 | 0.28x |
| users.json $[*].orders[*].total | strata | 6.058 | 8.447 | 10.000 | 52.789 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 20.496 | 28.581 | 34.749 | 52.789 | 0.30x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 40.603 | 56.128 | 59.703 | 52.789 | 0.15x |
| users.json $..total | strata | 17.006 | 18.915 | 30.376 | 52.453 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 361.790 | 386.960 | 648.199 | 52.453 | 0.05x |

