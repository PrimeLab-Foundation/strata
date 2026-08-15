# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 6520b15
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
| users.json | strata | 10.601 | 11.799 | 21.022 | 45.551 | 1.00x |
| users.json | orjson | 13.569 | 14.196 | 23.606 | 45.551 | 0.83x |
| users.json | msgspec | 12.870 | 13.573 | 18.469 | 45.551 | 0.87x |
| users.json | ujson | 21.789 | 24.716 | 39.809 | 45.551 | 0.48x |
| users.json | json | 22.987 | 23.865 | 49.123 | 45.551 | 0.49x |
| flat.json | strata | 1.226 | 1.969 | 2.825 | 47.312 | 1.00x |
| flat.json | orjson | 1.323 | 1.736 | 2.107 | 47.312 | 1.13x |
| flat.json | msgspec | 1.251 | 1.955 | 2.213 | 47.312 | 1.01x |
| flat.json | ujson | 2.464 | 2.917 | 4.177 | 47.312 | 0.68x |
| flat.json | json | 2.018 | 2.601 | 6.033 | 47.312 | 0.76x |
| nested.json | strata | 0.885 | 1.666 | 2.343 | 47.414 | 1.00x |
| nested.json | orjson | 1.153 | 1.852 | 6.182 | 47.414 | 0.90x |
| nested.json | msgspec | 1.093 | 1.985 | 3.628 | 47.414 | 0.84x |
| nested.json | ujson | 1.659 | 3.264 | 4.317 | 47.414 | 0.51x |
| nested.json | json | 2.147 | 4.173 | 5.295 | 47.414 | 0.40x |
| wide_arrays.json | strata | 5.467 | 6.382 | 11.565 | 49.188 | 1.00x |
| wide_arrays.json | orjson | 6.087 | 7.338 | 11.022 | 49.188 | 0.87x |
| wide_arrays.json | msgspec | 6.181 | 8.743 | 17.657 | 49.188 | 0.73x |
| wide_arrays.json | ujson | 8.779 | 13.375 | 46.013 | 49.188 | 0.48x |
| wide_arrays.json | json | 12.836 | 14.878 | 39.495 | 49.188 | 0.43x |
| mixed.json | strata | 0.236 | 0.366 | 0.415 | 47.348 | 1.00x |
| mixed.json | orjson | 0.306 | 0.427 | 0.504 | 47.348 | 0.86x |
| mixed.json | msgspec | 0.277 | 0.438 | 0.515 | 47.348 | 0.84x |
| mixed.json | ujson | 0.425 | 0.666 | 0.754 | 47.348 | 0.55x |
| mixed.json | json | 0.505 | 0.900 | 0.942 | 47.348 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.740 | 3.772 | 6.139 | 44.840 | 1.00x |
| users.json | orjson | 3.702 | 3.837 | 5.688 | 44.840 | 0.98x |
| users.json | msgspec | 5.224 | 5.348 | 7.891 | 44.840 | 0.71x |
| users.json | ujson | 17.247 | 17.576 | 27.567 | 44.840 | 0.21x |
| users.json | json | 23.486 | 23.890 | 24.615 | 44.840 | 0.16x |
| flat.json | strata | 0.432 | 0.569 | 0.657 | 47.625 | 1.00x |
| flat.json | orjson | 0.390 | 0.688 | 1.578 | 47.625 | 0.83x |
| flat.json | msgspec | 0.540 | 0.786 | 1.824 | 47.625 | 0.72x |
| flat.json | ujson | 1.946 | 2.159 | 4.037 | 47.625 | 0.26x |
| flat.json | json | 2.210 | 3.054 | 4.456 | 47.625 | 0.19x |
| nested.json | strata | 0.301 | 0.336 | 0.510 | 47.719 | 1.00x |
| nested.json | orjson | 0.333 | 0.499 | 0.622 | 47.719 | 0.67x |
| nested.json | msgspec | 0.493 | 0.749 | 1.284 | 47.719 | 0.45x |
| nested.json | ujson | 1.542 | 2.381 | 2.891 | 47.719 | 0.14x |
| nested.json | json | 2.482 | 3.817 | 6.560 | 47.719 | 0.09x |
| wide_arrays.json | strata | 2.402 | 3.948 | 6.162 | 48.297 | 1.00x |
| wide_arrays.json | orjson | 2.543 | 3.203 | 4.233 | 48.297 | 1.23x |
| wide_arrays.json | msgspec | 3.856 | 5.405 | 6.791 | 48.297 | 0.73x |
| wide_arrays.json | ujson | 9.877 | 12.565 | 17.362 | 48.297 | 0.31x |
| wide_arrays.json | json | 18.933 | 27.050 | 85.891 | 48.297 | 0.15x |
| mixed.json | strata | 0.125 | 0.134 | 0.161 | 47.441 | 1.00x |
| mixed.json | orjson | 0.079 | 0.109 | 0.119 | 47.441 | 1.23x |
| mixed.json | msgspec | 0.107 | 0.149 | 0.163 | 47.441 | 0.90x |
| mixed.json | ujson | 0.350 | 0.545 | 0.652 | 47.441 | 0.25x |
| mixed.json | json | 0.841 | 0.914 | 0.990 | 47.441 | 0.15x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 13.702 | 14.502 | 22.324 | 54.160 | 1.00x |
| users.json | orjson | 14.756 | 15.302 | 16.779 | 54.160 | 0.95x |
| users.json | msgspec | 14.533 | 15.564 | 24.286 | 54.160 | 0.93x |
| users.json | ujson | 28.052 | 28.997 | 36.401 | 54.160 | 0.50x |
| users.json | json | 24.303 | 25.238 | 29.068 | 54.160 | 0.57x |
| flat.json | strata | 1.697 | 2.363 | 2.510 | 47.180 | 1.00x |
| flat.json | orjson | 1.751 | 2.371 | 6.284 | 47.180 | 1.00x |
| flat.json | msgspec | 1.632 | 2.137 | 2.900 | 47.180 | 1.11x |
| flat.json | ujson | 3.073 | 4.855 | 6.866 | 47.180 | 0.49x |
| flat.json | json | 2.568 | 4.002 | 10.548 | 47.180 | 0.59x |
| nested.json | strata | 1.086 | 1.284 | 1.620 | 47.422 | 1.00x |
| nested.json | orjson | 1.300 | 1.578 | 2.101 | 47.422 | 0.81x |
| nested.json | msgspec | 1.232 | 1.397 | 1.895 | 47.422 | 0.92x |
| nested.json | ujson | 2.238 | 2.502 | 4.224 | 47.422 | 0.51x |
| nested.json | json | 2.437 | 2.597 | 2.868 | 47.422 | 0.49x |
| wide_arrays.json | strata | 7.247 | 10.562 | 10.987 | 48.297 | 1.00x |
| wide_arrays.json | orjson | 9.645 | 10.294 | 10.715 | 48.297 | 1.03x |
| wide_arrays.json | msgspec | 9.391 | 10.952 | 11.292 | 48.297 | 0.96x |
| wide_arrays.json | ujson | 16.821 | 17.655 | 18.763 | 48.297 | 0.60x |
| wide_arrays.json | json | 21.208 | 22.390 | 26.549 | 48.297 | 0.47x |
| mixed.json | strata | 0.337 | 0.485 | 0.577 | 47.238 | 1.00x |
| mixed.json | orjson | 0.433 | 0.587 | 0.693 | 47.238 | 0.83x |
| mixed.json | msgspec | 0.559 | 0.669 | 0.723 | 47.238 | 0.72x |
| mixed.json | ujson | 0.789 | 1.010 | 1.365 | 47.238 | 0.48x |
| mixed.json | json | 0.647 | 1.045 | 1.486 | 47.238 | 0.46x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 15.089 | 17.518 | 27.598 | 48.293 | 1.00x |
| users.ndjson | orjson | 20.725 | 22.759 | 40.409 | 48.293 | 0.77x |
| users.ndjson | msgspec | 20.751 | 25.221 | 35.721 | 48.293 | 0.69x |
| users.ndjson | ujson | 31.690 | 35.536 | 79.778 | 48.293 | 0.49x |
| users.ndjson | json | 34.908 | 42.802 | 55.092 | 48.293 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.707 | 4.948 | 7.735 | 51.277 | 1.00x |
| users.json | orjson | 4.905 | 5.172 | 7.068 | 51.277 | 0.96x |
| users.json | msgspec | 6.242 | 6.876 | 9.997 | 51.277 | 0.72x |
| users.json | ujson | 27.250 | 27.889 | 36.548 | 51.277 | 0.18x |
| users.json | json | 33.149 | 33.325 | 39.079 | 51.277 | 0.15x |
| flat.json | strata | 0.844 | 1.301 | 1.946 | 47.824 | 1.00x |
| flat.json | orjson | 0.841 | 1.372 | 2.205 | 47.824 | 0.95x |
| flat.json | msgspec | 1.003 | 1.465 | 2.701 | 47.824 | 0.89x |
| flat.json | ujson | 3.459 | 4.881 | 15.293 | 47.824 | 0.27x |
| flat.json | json | 3.774 | 6.208 | 14.597 | 47.824 | 0.21x |
| nested.json | strata | 0.828 | 1.040 | 1.603 | 47.645 | 1.00x |
| nested.json | orjson | 0.727 | 0.906 | 1.344 | 47.645 | 1.15x |
| nested.json | msgspec | 0.966 | 1.305 | 5.242 | 47.645 | 0.80x |
| nested.json | ujson | 2.618 | 3.677 | 5.645 | 47.645 | 0.28x |
| nested.json | json | 3.724 | 5.492 | 8.018 | 47.645 | 0.19x |
| wide_arrays.json | strata | 4.507 | 4.713 | 5.263 | 48.305 | 1.00x |
| wide_arrays.json | orjson | 4.236 | 4.915 | 5.373 | 48.305 | 0.96x |
| wide_arrays.json | msgspec | 5.457 | 6.904 | 7.260 | 48.305 | 0.68x |
| wide_arrays.json | ujson | 17.422 | 25.452 | 40.870 | 48.305 | 0.19x |
| wide_arrays.json | json | 33.952 | 40.353 | 44.307 | 48.305 | 0.12x |
| mixed.json | strata | 0.534 | 0.689 | 0.770 | 47.410 | 1.00x |
| mixed.json | orjson | 0.475 | 0.634 | 0.737 | 47.410 | 1.09x |
| mixed.json | msgspec | 0.572 | 0.633 | 1.667 | 47.410 | 1.09x |
| mixed.json | ujson | 1.312 | 1.403 | 1.702 | 47.410 | 0.49x |
| mixed.json | json | 1.658 | 1.801 | 2.254 | 47.410 | 0.38x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.102 | 0.106 | 0.153 | 51.586 | 1.00x |
| users.json $[*].id | jmespath | 0.462 | 0.473 | 0.881 | 51.586 | 0.22x |
| users.json $[*].id | jsonpath-ng | 2.704 | 2.942 | 4.781 | 51.586 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.483 | 0.513 | 0.653 | 51.617 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.853 | 2.971 | 3.349 | 51.617 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 18.512 | 19.482 | 33.053 | 51.617 | 0.03x |
| users.json $..total | strata | 1.872 | 2.003 | 3.435 | 52.199 | 1.00x |
| users.json $..total | jsonpath-ng | 338.446 | 362.783 | 366.596 | 52.199 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 5.249 | 5.355 | 5.464 | 51.387 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.226 | 16.051 | 24.520 | 51.387 | 0.33x |
| users.json $[*].id | orjson+jsonpath-ng | 17.785 | 18.573 | 30.184 | 51.387 | 0.29x |
| users.json $[*].orders[*].total | strata | 5.622 | 5.688 | 8.959 | 52.125 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.640 | 19.217 | 28.150 | 52.125 | 0.30x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 38.672 | 40.095 | 50.505 | 52.125 | 0.14x |
| users.json $..total | strata | 17.401 | 18.562 | 27.914 | 52.156 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 359.365 | 381.626 | 612.306 | 52.156 | 0.05x |

