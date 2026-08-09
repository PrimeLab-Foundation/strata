# Benchmark results - medium

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
| users.json | strata | 61.359 | 63.978 | 67.352 | 139.391 | 1.00x |
| users.json | orjson | 65.155 | 67.110 | 83.039 | 139.391 | 0.95x |
| users.json | msgspec | 65.286 | 69.348 | 71.876 | 139.391 | 0.92x |
| users.json | ujson | 90.290 | 93.875 | 99.324 | 139.391 | 0.68x |
| users.json | json | 113.398 | 117.472 | 120.088 | 139.391 | 0.54x |
| flat.json | strata | 3.829 | 3.849 | 4.090 | 164.359 | 1.00x |
| flat.json | orjson | 4.537 | 4.579 | 4.871 | 164.359 | 0.84x |
| flat.json | msgspec | 4.594 | 4.708 | 5.131 | 164.359 | 0.82x |
| flat.json | ujson | 6.299 | 6.400 | 6.839 | 164.359 | 0.60x |
| flat.json | json | 8.246 | 8.335 | 8.965 | 164.359 | 0.46x |
| nested.json | strata | 3.169 | 3.211 | 3.442 | 165.250 | 1.00x |
| nested.json | orjson | 3.934 | 3.980 | 4.069 | 165.250 | 0.81x |
| nested.json | msgspec | 3.956 | 4.065 | 4.630 | 165.250 | 0.79x |
| nested.json | ujson | 5.362 | 5.437 | 5.699 | 165.250 | 0.59x |
| nested.json | json | 7.690 | 7.791 | 8.226 | 165.250 | 0.41x |
| wide_arrays.json | strata | 19.051 | 19.319 | 19.705 | 193.391 | 1.00x |
| wide_arrays.json | orjson | 17.050 | 17.240 | 18.082 | 193.391 | 1.12x |
| wide_arrays.json | msgspec | 20.803 | 20.915 | 21.694 | 193.391 | 0.92x |
| wide_arrays.json | ujson | 26.969 | 27.171 | 27.996 | 193.391 | 0.71x |
| wide_arrays.json | json | 39.329 | 39.695 | 40.506 | 193.391 | 0.49x |
| mixed.json | strata | 0.823 | 0.830 | 0.881 | 173.297 | 1.00x |
| mixed.json | orjson | 1.535 | 1.570 | 1.652 | 173.297 | 0.53x |
| mixed.json | msgspec | 1.578 | 1.706 | 2.092 | 173.297 | 0.49x |
| mixed.json | ujson | 1.785 | 1.828 | 1.991 | 173.297 | 0.45x |
| mixed.json | json | 2.385 | 2.445 | 2.522 | 173.297 | 0.34x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 15.957 | 16.144 | 16.492 | 601.734 | 1.00x |
| users.json | orjson | 15.758 | 15.917 | 16.063 | 601.734 | 1.01x |
| users.json | msgspec | 20.790 | 20.887 | 21.451 | 601.734 | 0.77x |
| users.json | ujson | 84.037 | 84.575 | 85.520 | 601.734 | 0.19x |
| users.json | json | 115.092 | 115.610 | 116.190 | 601.734 | 0.14x |
| flat.json | strata | 1.085 | 1.099 | 1.200 | 164.422 | 1.00x |
| flat.json | orjson | 1.136 | 1.152 | 1.219 | 164.422 | 0.95x |
| flat.json | msgspec | 1.506 | 1.520 | 1.728 | 164.422 | 0.72x |
| flat.json | ujson | 4.686 | 4.718 | 5.070 | 164.422 | 0.23x |
| flat.json | json | 6.450 | 6.492 | 6.749 | 164.422 | 0.17x |
| nested.json | strata | 0.803 | 0.811 | 0.880 | 165.359 | 1.00x |
| nested.json | orjson | 1.031 | 1.036 | 1.055 | 165.359 | 0.78x |
| nested.json | msgspec | 1.362 | 1.379 | 1.455 | 165.359 | 0.59x |
| nested.json | ujson | 5.038 | 5.060 | 5.107 | 165.359 | 0.16x |
| nested.json | json | 8.015 | 8.107 | 8.284 | 165.359 | 0.10x |
| wide_arrays.json | strata | 6.160 | 6.408 | 6.755 | 189.266 | 1.00x |
| wide_arrays.json | orjson | 6.350 | 6.579 | 6.811 | 189.266 | 0.97x |
| wide_arrays.json | msgspec | 10.688 | 10.947 | 11.120 | 189.266 | 0.59x |
| wide_arrays.json | ujson | 31.844 | 31.968 | 32.591 | 189.266 | 0.20x |
| wide_arrays.json | json | 56.666 | 57.254 | 57.848 | 189.266 | 0.11x |
| mixed.json | strata | 0.210 | 0.213 | 0.232 | 173.312 | 1.00x |
| mixed.json | orjson | 0.200 | 0.201 | 0.208 | 173.312 | 1.06x |
| mixed.json | msgspec | 0.223 | 0.231 | 0.239 | 173.312 | 0.92x |
| mixed.json | ujson | 1.089 | 1.098 | 1.116 | 173.312 | 0.19x |
| mixed.json | json | 1.693 | 1.710 | 1.734 | 173.312 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 62.238 | 64.558 | 68.143 | 656.891 | 1.00x |
| users.json | orjson | 79.569 | 82.468 | 85.229 | 656.891 | 0.78x |
| users.json | msgspec | 80.595 | 84.053 | 87.233 | 656.891 | 0.77x |
| users.json | ujson | 107.230 | 110.128 | 114.234 | 656.891 | 0.59x |
| users.json | json | 129.445 | 132.761 | 134.895 | 656.891 | 0.49x |
| flat.json | strata | 4.006 | 4.049 | 4.117 | 165.219 | 1.00x |
| flat.json | orjson | 5.000 | 5.084 | 5.348 | 165.219 | 0.80x |
| flat.json | msgspec | 5.146 | 5.191 | 5.687 | 165.219 | 0.78x |
| flat.json | ujson | 6.900 | 7.077 | 7.654 | 165.219 | 0.57x |
| flat.json | json | 8.765 | 8.831 | 9.154 | 165.219 | 0.46x |
| nested.json | strata | 3.306 | 3.337 | 3.462 | 165.953 | 1.00x |
| nested.json | orjson | 4.467 | 4.535 | 4.933 | 165.953 | 0.74x |
| nested.json | msgspec | 4.506 | 4.584 | 4.775 | 165.953 | 0.73x |
| nested.json | ujson | 5.992 | 6.077 | 6.517 | 165.953 | 0.55x |
| nested.json | json | 8.260 | 8.354 | 9.006 | 165.953 | 0.40x |
| wide_arrays.json | strata | 19.534 | 19.721 | 20.349 | 167.250 | 1.00x |
| wide_arrays.json | orjson | 19.496 | 19.746 | 20.381 | 167.250 | 1.00x |
| wide_arrays.json | msgspec | 23.562 | 23.921 | 24.242 | 167.250 | 0.82x |
| wide_arrays.json | ujson | 30.506 | 31.075 | 32.479 | 167.250 | 0.63x |
| wide_arrays.json | json | 42.072 | 42.450 | 43.267 | 167.250 | 0.46x |
| mixed.json | strata | 0.895 | 0.913 | 0.937 | 173.312 | 1.00x |
| mixed.json | orjson | 1.702 | 1.739 | 1.883 | 173.312 | 0.52x |
| mixed.json | msgspec | 1.756 | 1.808 | 1.943 | 173.312 | 0.50x |
| mixed.json | ujson | 2.002 | 2.056 | 2.249 | 173.312 | 0.44x |
| mixed.json | json | 2.583 | 2.613 | 2.767 | 173.312 | 0.35x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 58.038 | 58.353 | 58.812 | 167.125 | 1.00x |
| users.ndjson | orjson | 81.648 | 82.660 | 83.734 | 167.125 | 0.71x |
| users.ndjson | msgspec | 80.821 | 81.580 | 83.443 | 167.125 | 0.72x |
| users.ndjson | ujson | 104.336 | 105.812 | 108.274 | 167.125 | 0.55x |
| users.ndjson | json | 133.666 | 134.899 | 136.318 | 167.125 | 0.43x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 18.314 | 19.101 | 21.582 | 561.172 | 1.00x |
| users.json | orjson | 18.971 | 19.581 | 21.009 | 561.172 | 0.98x |
| users.json | msgspec | 23.759 | 24.430 | 25.830 | 561.172 | 0.78x |
| users.json | ujson | 84.619 | 86.240 | 88.351 | 561.172 | 0.22x |
| users.json | json | 118.308 | 119.165 | 120.936 | 561.172 | 0.16x |
| flat.json | strata | 1.369 | 1.422 | 1.611 | 165.250 | 1.00x |
| flat.json | orjson | 1.394 | 1.431 | 1.572 | 165.250 | 0.99x |
| flat.json | msgspec | 1.752 | 1.785 | 1.990 | 165.250 | 0.80x |
| flat.json | ujson | 4.798 | 4.866 | 5.260 | 165.250 | 0.29x |
| flat.json | json | 6.740 | 6.853 | 7.128 | 165.250 | 0.21x |
| nested.json | strata | 1.068 | 1.090 | 1.186 | 166.109 | 1.00x |
| nested.json | orjson | 1.252 | 1.308 | 1.384 | 166.109 | 0.83x |
| nested.json | msgspec | 1.597 | 1.639 | 1.683 | 166.109 | 0.66x |
| nested.json | ujson | 5.265 | 5.369 | 5.461 | 166.109 | 0.20x |
| nested.json | json | 8.437 | 8.488 | 8.874 | 166.109 | 0.13x |
| wide_arrays.json | strata | 7.238 | 7.545 | 8.191 | 174.297 | 1.00x |
| wide_arrays.json | orjson | 7.353 | 7.478 | 7.899 | 174.297 | 1.01x |
| wide_arrays.json | msgspec | 11.684 | 11.769 | 12.175 | 174.297 | 0.64x |
| wide_arrays.json | ujson | 32.942 | 33.248 | 33.573 | 174.297 | 0.23x |
| wide_arrays.json | json | 57.820 | 58.128 | 59.399 | 174.297 | 0.13x |
| mixed.json | strata | 0.385 | 0.406 | 0.486 | 173.312 | 1.00x |
| mixed.json | orjson | 0.354 | 0.375 | 0.498 | 173.312 | 1.08x |
| mixed.json | msgspec | 0.368 | 0.384 | 0.485 | 173.312 | 1.06x |
| mixed.json | ujson | 1.245 | 1.283 | 1.322 | 173.312 | 0.32x |
| mixed.json | json | 1.840 | 1.904 | 2.009 | 173.312 | 0.21x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.196 | 0.217 | 0.249 | 561.625 | 1.00x |
| users.json $[*].id | jmespath | 0.992 | 1.029 | 1.207 | 561.625 | 0.21x |
| users.json $[*].id | jsonpath-ng | 14.887 | 15.773 | 16.648 | 561.625 | 0.01x |
| users.json $[*].orders[*].total | strata | 2.660 | 2.737 | 2.994 | 439.969 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 10.337 | 10.464 | 11.367 | 439.969 | 0.26x |
| users.json $[*].orders[*].total | jsonpath-ng | 56.774 | 57.730 | 60.193 | 439.969 | 0.05x |
| users.json $..total | strata | 10.146 | 10.242 | 10.507 | 149.516 | 1.00x |
| users.json $..total | jsonpath-ng | 1626.329 | 1629.290 | 1639.177 | 149.516 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 28.778 | 29.122 | 29.189 | 621.219 | 1.00x |
| users.json $[*].id | orjson+jmespath | 80.889 | 82.401 | 84.178 | 621.219 | 0.35x |
| users.json $[*].id | orjson+jsonpath-ng | 84.439 | 85.304 | 86.006 | 621.219 | 0.34x |
| users.json $[*].orders[*].total | strata | 28.998 | 29.557 | 29.686 | 503.078 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 88.829 | 90.311 | 92.540 | 503.078 | 0.33x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 126.787 | 129.334 | 132.361 | 503.078 | 0.23x |
| users.json $..total | strata | 84.458 | 85.071 | 86.190 | 202.281 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 1693.656 | 1695.774 | 1698.772 | 202.281 | 0.05x |

