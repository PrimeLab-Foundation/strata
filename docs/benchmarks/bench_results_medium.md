# Benchmark results - medium

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 68d6e74
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
| users.json | strata | 42.413 | 46.032 | 49.059 | 139.688 | 1.00x |
| users.json | orjson | 50.491 | 52.282 | 59.752 | 139.688 | 0.88x |
| users.json | msgspec | 52.126 | 54.191 | 62.076 | 139.688 | 0.85x |
| users.json | ujson | 71.411 | 75.748 | 81.615 | 139.688 | 0.61x |
| users.json | json | 103.399 | 106.967 | 116.463 | 139.688 | 0.43x |
| flat.json | strata | 2.616 | 2.646 | 2.712 | 163.609 | 1.00x |
| flat.json | orjson | 2.844 | 2.878 | 2.911 | 163.609 | 0.92x |
| flat.json | msgspec | 2.834 | 2.864 | 2.904 | 163.609 | 0.92x |
| flat.json | ujson | 4.136 | 4.199 | 4.597 | 163.609 | 0.63x |
| flat.json | json | 5.798 | 5.896 | 5.964 | 163.609 | 0.45x |
| nested.json | strata | 2.254 | 2.281 | 2.415 | 165.406 | 1.00x |
| nested.json | orjson | 2.402 | 2.421 | 2.637 | 165.406 | 0.94x |
| nested.json | msgspec | 2.415 | 2.429 | 2.538 | 165.406 | 0.94x |
| nested.json | ujson | 3.463 | 3.499 | 3.662 | 165.406 | 0.65x |
| nested.json | json | 5.546 | 5.563 | 5.607 | 165.406 | 0.41x |
| wide_arrays.json | strata | 12.408 | 12.575 | 12.841 | 192.812 | 1.00x |
| wide_arrays.json | orjson | 13.295 | 13.400 | 13.850 | 192.812 | 0.94x |
| wide_arrays.json | msgspec | 16.355 | 16.456 | 17.085 | 192.812 | 0.76x |
| wide_arrays.json | ujson | 21.039 | 21.378 | 21.930 | 192.812 | 0.59x |
| wide_arrays.json | json | 30.506 | 30.589 | 31.221 | 192.812 | 0.41x |
| mixed.json | strata | 0.528 | 0.531 | 0.566 | 173.734 | 1.00x |
| mixed.json | orjson | 0.590 | 0.592 | 0.600 | 173.734 | 0.90x |
| mixed.json | msgspec | 0.638 | 0.640 | 0.652 | 173.734 | 0.83x |
| mixed.json | ujson | 0.785 | 0.791 | 0.800 | 173.734 | 0.67x |
| mixed.json | json | 1.318 | 1.322 | 1.344 | 173.734 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.668 | 10.220 | 10.941 | 600.312 | 1.00x |
| users.json | orjson | 12.232 | 12.378 | 13.144 | 600.312 | 0.83x |
| users.json | msgspec | 16.059 | 16.180 | 16.542 | 600.312 | 0.63x |
| users.json | ujson | 62.841 | 63.117 | 64.793 | 600.312 | 0.16x |
| users.json | json | 90.684 | 91.425 | 93.440 | 600.312 | 0.11x |
| flat.json | strata | 0.825 | 0.830 | 0.843 | 164.422 | 1.00x |
| flat.json | orjson | 0.879 | 0.891 | 0.941 | 164.422 | 0.93x |
| flat.json | msgspec | 1.153 | 1.169 | 1.283 | 164.422 | 0.71x |
| flat.json | ujson | 3.431 | 3.481 | 3.558 | 164.422 | 0.24x |
| flat.json | json | 5.218 | 5.307 | 6.111 | 164.422 | 0.16x |
| nested.json | strata | 0.554 | 0.560 | 0.567 | 165.453 | 1.00x |
| nested.json | orjson | 0.788 | 0.791 | 0.804 | 165.453 | 0.71x |
| nested.json | msgspec | 1.046 | 1.057 | 1.148 | 165.453 | 0.53x |
| nested.json | ujson | 3.853 | 3.882 | 3.953 | 165.453 | 0.14x |
| nested.json | json | 6.239 | 6.254 | 6.313 | 165.453 | 0.09x |
| wide_arrays.json | strata | 4.194 | 4.394 | 4.583 | 188.688 | 1.00x |
| wide_arrays.json | orjson | 5.057 | 5.213 | 5.381 | 188.688 | 0.84x |
| wide_arrays.json | msgspec | 8.379 | 8.509 | 9.008 | 188.688 | 0.52x |
| wide_arrays.json | ujson | 24.182 | 24.555 | 26.889 | 188.688 | 0.18x |
| wide_arrays.json | json | 45.267 | 45.921 | 46.805 | 188.688 | 0.10x |
| mixed.json | strata | 0.149 | 0.152 | 0.163 | 173.797 | 1.00x |
| mixed.json | orjson | 0.153 | 0.158 | 0.192 | 173.797 | 0.96x |
| mixed.json | msgspec | 0.170 | 0.182 | 0.190 | 173.797 | 0.84x |
| mixed.json | ujson | 0.848 | 0.860 | 0.892 | 173.797 | 0.18x |
| mixed.json | json | 1.352 | 1.370 | 1.608 | 173.797 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 43.898 | 45.881 | 47.806 | 655.453 | 1.00x |
| users.json | orjson | 52.060 | 53.898 | 57.392 | 655.453 | 0.85x |
| users.json | msgspec | 53.995 | 55.972 | 58.070 | 655.453 | 0.82x |
| users.json | ujson | 75.483 | 77.636 | 83.363 | 655.453 | 0.59x |
| users.json | json | 103.155 | 106.878 | 111.549 | 655.453 | 0.43x |
| flat.json | strata | 2.755 | 2.788 | 2.946 | 165.391 | 1.00x |
| flat.json | orjson | 2.960 | 3.002 | 3.110 | 165.391 | 0.93x |
| flat.json | msgspec | 2.991 | 3.014 | 3.044 | 165.391 | 0.93x |
| flat.json | ujson | 4.375 | 4.428 | 4.518 | 165.391 | 0.63x |
| flat.json | json | 5.927 | 6.014 | 6.264 | 165.391 | 0.46x |
| nested.json | strata | 2.387 | 2.411 | 2.475 | 165.453 | 1.00x |
| nested.json | orjson | 2.491 | 2.518 | 2.559 | 165.453 | 0.96x |
| nested.json | msgspec | 2.475 | 2.514 | 2.534 | 165.453 | 0.96x |
| nested.json | ujson | 3.619 | 3.673 | 3.891 | 165.453 | 0.66x |
| nested.json | json | 5.540 | 5.598 | 5.700 | 165.453 | 0.43x |
| wide_arrays.json | strata | 12.982 | 13.198 | 13.832 | 166.688 | 1.00x |
| wide_arrays.json | orjson | 13.913 | 14.030 | 14.723 | 166.688 | 0.94x |
| wide_arrays.json | msgspec | 17.022 | 17.187 | 17.637 | 166.688 | 0.77x |
| wide_arrays.json | ujson | 22.342 | 22.461 | 23.201 | 166.688 | 0.59x |
| wide_arrays.json | json | 31.247 | 31.651 | 32.027 | 166.688 | 0.42x |
| mixed.json | strata | 0.595 | 0.617 | 0.660 | 173.797 | 1.00x |
| mixed.json | orjson | 0.650 | 0.673 | 0.711 | 173.797 | 0.92x |
| mixed.json | msgspec | 0.708 | 0.753 | 0.769 | 173.797 | 0.82x |
| mixed.json | ujson | 0.881 | 0.924 | 1.019 | 173.797 | 0.67x |
| mixed.json | json | 1.381 | 1.412 | 1.486 | 173.797 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 43.120 | 43.442 | 45.291 | 163.688 | 1.00x |
| users.ndjson | orjson | 61.425 | 61.760 | 63.443 | 163.688 | 0.70x |
| users.ndjson | msgspec | 61.870 | 62.248 | 63.004 | 163.688 | 0.70x |
| users.ndjson | ujson | 79.017 | 80.460 | 84.528 | 163.688 | 0.54x |
| users.ndjson | json | 103.425 | 103.833 | 105.505 | 163.688 | 0.42x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.385 | 12.687 | 13.588 | 502.391 | 1.00x |
| users.json | orjson | 13.948 | 15.536 | 16.284 | 502.391 | 0.82x |
| users.json | msgspec | 17.803 | 19.143 | 20.022 | 502.391 | 0.66x |
| users.json | ujson | 65.695 | 67.788 | 94.580 | 502.391 | 0.19x |
| users.json | json | 92.977 | 94.538 | 96.635 | 502.391 | 0.13x |
| flat.json | strata | 1.049 | 1.070 | 1.123 | 165.406 | 1.00x |
| flat.json | orjson | 1.113 | 1.138 | 1.503 | 165.406 | 0.94x |
| flat.json | msgspec | 1.376 | 1.400 | 1.476 | 165.406 | 0.76x |
| flat.json | ujson | 3.685 | 3.709 | 3.743 | 165.406 | 0.29x |
| flat.json | json | 5.472 | 5.531 | 5.797 | 165.406 | 0.19x |
| nested.json | strata | 0.770 | 0.839 | 1.693 | 165.562 | 1.00x |
| nested.json | orjson | 1.022 | 1.063 | 1.144 | 165.562 | 0.79x |
| nested.json | msgspec | 1.309 | 1.332 | 1.514 | 165.562 | 0.63x |
| nested.json | ujson | 4.115 | 4.179 | 4.421 | 165.562 | 0.20x |
| nested.json | json | 6.505 | 6.553 | 7.303 | 165.562 | 0.13x |
| wide_arrays.json | strata | 5.469 | 5.911 | 6.350 | 173.734 | 1.00x |
| wide_arrays.json | orjson | 6.194 | 6.454 | 7.037 | 173.734 | 0.92x |
| wide_arrays.json | msgspec | 9.721 | 9.913 | 10.357 | 173.734 | 0.60x |
| wide_arrays.json | ujson | 25.667 | 26.072 | 26.826 | 173.734 | 0.23x |
| wide_arrays.json | json | 46.277 | 46.719 | 47.414 | 173.734 | 0.13x |
| mixed.json | strata | 0.288 | 0.325 | 0.732 | 173.797 | 1.00x |
| mixed.json | orjson | 0.293 | 0.319 | 0.389 | 173.797 | 1.02x |
| mixed.json | msgspec | 0.318 | 0.341 | 0.380 | 173.797 | 0.95x |
| mixed.json | ujson | 1.005 | 1.037 | 1.082 | 173.797 | 0.31x |
| mixed.json | json | 1.486 | 1.556 | 1.812 | 173.797 | 0.21x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.192 | 0.246 | 0.295 | 502.688 | 1.00x |
| users.json $[*].id | jmespath | 0.767 | 0.812 | 0.858 | 502.688 | 0.30x |
| users.json $[*].id | jsonpath-ng | 3.084 | 3.154 | 3.353 | 502.688 | 0.08x |
| users.json $[*].orders[*].total | strata | 2.703 | 2.959 | 3.059 | 275.281 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 7.362 | 7.486 | 7.764 | 275.281 | 0.40x |
| users.json $[*].orders[*].total | jsonpath-ng | 38.096 | 38.571 | 42.996 | 275.281 | 0.08x |
| users.json $..total | strata | 8.000 | 8.054 | 8.475 | 154.828 | 1.00x |
| users.json $..total | jsonpath-ng | 1108.484 | 1113.338 | 1116.873 | 154.828 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 19.584 | 19.717 | 20.205 | 569.875 | 1.00x |
| users.json $[*].id | orjson+jmespath | 52.892 | 54.038 | 57.970 | 569.875 | 0.36x |
| users.json $[*].id | orjson+jsonpath-ng | 54.815 | 56.720 | 57.886 | 569.875 | 0.35x |
| users.json $[*].orders[*].total | strata | 19.814 | 19.950 | 20.331 | 307.438 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 58.791 | 59.859 | 61.215 | 307.438 | 0.33x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 101.412 | 105.130 | 110.198 | 307.438 | 0.19x |
| users.json $..total | strata | 51.321 | 51.828 | 52.553 | 200.969 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 1177.078 | 1180.925 | 1190.401 | 200.969 | 0.04x |

