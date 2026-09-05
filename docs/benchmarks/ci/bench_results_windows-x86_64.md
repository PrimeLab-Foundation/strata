# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: b7f31bb
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD
- compiler_flags: clang-cl /std:c++20 /O2 /arch:AVX2 -fprofile-use (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.740 | 8.803 | 11.789 | 45.609 | 1.00x |
| users.json | orjson | 12.949 | 13.101 | 15.443 | 45.609 | 0.67x |
| users.json | msgspec | 12.530 | 12.652 | 14.422 | 45.609 | 0.70x |
| users.json | ujson | 20.003 | 20.581 | 24.068 | 45.609 | 0.43x |
| users.json | json | 22.035 | 22.293 | 23.590 | 45.609 | 0.39x |
| flat.json | strata | 0.931 | 0.970 | 0.980 | 50.234 | 1.00x |
| flat.json | orjson | 1.098 | 1.127 | 1.196 | 50.234 | 0.86x |
| flat.json | msgspec | 1.101 | 1.136 | 1.151 | 50.234 | 0.85x |
| flat.json | ujson | 2.105 | 2.119 | 2.134 | 50.234 | 0.46x |
| flat.json | json | 1.986 | 2.014 | 2.025 | 50.234 | 0.48x |
| nested.json | strata | 0.773 | 0.801 | 0.822 | 50.277 | 1.00x |
| nested.json | orjson | 1.070 | 1.090 | 1.124 | 50.277 | 0.73x |
| nested.json | msgspec | 0.994 | 1.030 | 1.428 | 50.277 | 0.78x |
| nested.json | ujson | 1.526 | 1.571 | 1.622 | 50.277 | 0.51x |
| nested.json | json | 2.105 | 2.116 | 2.143 | 50.277 | 0.38x |
| wide_arrays.json | strata | 4.102 | 4.131 | 4.376 | 52.551 | 1.00x |
| wide_arrays.json | orjson | 5.534 | 5.556 | 5.641 | 52.551 | 0.74x |
| wide_arrays.json | msgspec | 5.684 | 5.740 | 6.100 | 52.551 | 0.72x |
| wide_arrays.json | ujson | 8.180 | 8.324 | 8.665 | 52.551 | 0.50x |
| wide_arrays.json | json | 11.766 | 11.802 | 18.612 | 52.551 | 0.35x |
| mixed.json | strata | 0.183 | 0.186 | 0.323 | 51.648 | 1.00x |
| mixed.json | orjson | 0.212 | 0.216 | 0.353 | 51.648 | 0.86x |
| mixed.json | msgspec | 0.232 | 0.235 | 0.410 | 51.648 | 0.79x |
| mixed.json | ujson | 0.345 | 0.360 | 0.570 | 51.648 | 0.52x |
| mixed.json | json | 0.473 | 0.486 | 0.864 | 51.648 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.072 | 3.099 | 3.311 | 44.848 | 1.00x |
| users.json | orjson | 3.647 | 3.713 | 3.740 | 44.848 | 0.83x |
| users.json | msgspec | 5.397 | 5.424 | 5.576 | 44.848 | 0.57x |
| users.json | ujson | 13.945 | 13.989 | 14.331 | 44.848 | 0.22x |
| users.json | json | 22.789 | 22.962 | 23.450 | 44.848 | 0.13x |
| flat.json | strata | 0.270 | 0.272 | 0.404 | 50.605 | 1.00x |
| flat.json | orjson | 0.351 | 0.368 | 0.528 | 50.605 | 0.74x |
| flat.json | msgspec | 0.513 | 0.518 | 0.758 | 50.605 | 0.52x |
| flat.json | ujson | 1.522 | 1.620 | 1.640 | 50.605 | 0.17x |
| flat.json | json | 1.868 | 2.102 | 3.536 | 50.605 | 0.13x |
| nested.json | strata | 0.272 | 0.275 | 0.306 | 50.926 | 1.00x |
| nested.json | orjson | 0.315 | 0.329 | 0.359 | 50.926 | 0.84x |
| nested.json | msgspec | 0.462 | 0.467 | 0.508 | 50.926 | 0.59x |
| nested.json | ujson | 1.135 | 1.180 | 1.218 | 50.926 | 0.23x |
| nested.json | json | 2.384 | 2.402 | 2.552 | 50.926 | 0.11x |
| wide_arrays.json | strata | 1.853 | 1.882 | 1.894 | 53.172 | 1.00x |
| wide_arrays.json | orjson | 2.273 | 2.337 | 2.390 | 53.172 | 0.81x |
| wide_arrays.json | msgspec | 3.408 | 3.654 | 3.958 | 53.172 | 0.52x |
| wide_arrays.json | ujson | 7.336 | 7.487 | 7.644 | 53.172 | 0.25x |
| wide_arrays.json | json | 18.078 | 18.255 | 18.738 | 53.172 | 0.10x |
| mixed.json | strata | 0.069 | 0.070 | 0.072 | 51.648 | 1.00x |
| mixed.json | orjson | 0.069 | 0.070 | 0.104 | 51.648 | 1.00x |
| mixed.json | msgspec | 0.093 | 0.093 | 0.096 | 51.648 | 0.75x |
| mixed.json | ujson | 0.268 | 0.273 | 0.276 | 51.648 | 0.26x |
| mixed.json | json | 0.515 | 0.525 | 0.574 | 51.648 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.796 | 10.012 | 10.748 | 54.871 | 1.00x |
| users.json | orjson | 13.855 | 14.086 | 15.128 | 54.871 | 0.71x |
| users.json | msgspec | 13.435 | 13.872 | 16.456 | 54.871 | 0.72x |
| users.json | ujson | 24.904 | 26.155 | 28.084 | 54.871 | 0.38x |
| users.json | json | 23.088 | 23.480 | 24.494 | 54.871 | 0.43x |
| flat.json | strata | 1.023 | 1.036 | 1.093 | 50.488 | 1.00x |
| flat.json | orjson | 1.233 | 1.272 | 1.284 | 50.488 | 0.81x |
| flat.json | msgspec | 1.254 | 1.312 | 1.490 | 50.488 | 0.79x |
| flat.json | ujson | 2.731 | 2.749 | 2.802 | 50.488 | 0.38x |
| flat.json | json | 2.166 | 2.177 | 2.201 | 50.488 | 0.48x |
| nested.json | strata | 0.831 | 0.881 | 1.043 | 50.594 | 1.00x |
| nested.json | orjson | 1.177 | 1.228 | 1.258 | 50.594 | 0.72x |
| nested.json | msgspec | 1.111 | 1.163 | 1.181 | 50.594 | 0.76x |
| nested.json | ujson | 1.942 | 1.960 | 1.972 | 50.594 | 0.45x |
| nested.json | json | 2.245 | 2.252 | 2.310 | 50.594 | 0.39x |
| wide_arrays.json | strata | 4.573 | 4.616 | 5.273 | 53.172 | 1.00x |
| wide_arrays.json | orjson | 5.920 | 5.949 | 6.157 | 53.172 | 0.78x |
| wide_arrays.json | msgspec | 6.175 | 6.211 | 6.336 | 53.172 | 0.74x |
| wide_arrays.json | ujson | 11.088 | 11.221 | 11.607 | 53.172 | 0.41x |
| wide_arrays.json | json | 12.142 | 12.214 | 12.495 | 53.172 | 0.38x |
| mixed.json | strata | 0.257 | 0.260 | 0.285 | 51.648 | 1.00x |
| mixed.json | orjson | 0.321 | 0.323 | 0.360 | 51.648 | 0.80x |
| mixed.json | msgspec | 0.340 | 0.345 | 0.390 | 51.648 | 0.75x |
| mixed.json | ujson | 0.522 | 0.537 | 0.636 | 51.648 | 0.48x |
| mixed.json | json | 0.582 | 0.592 | 0.644 | 51.648 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.779 | 9.914 | 10.246 | 51.250 | 1.00x |
| users.ndjson | orjson | 16.444 | 16.738 | 21.028 | 51.250 | 0.59x |
| users.ndjson | msgspec | 16.673 | 16.825 | 22.476 | 51.250 | 0.59x |
| users.ndjson | ujson | 24.284 | 24.468 | 27.446 | 51.250 | 0.41x |
| users.ndjson | json | 28.833 | 29.044 | 29.711 | 51.250 | 0.34x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.941 | 4.074 | 4.411 | 51.934 | 1.00x |
| users.json | orjson | 4.521 | 4.671 | 6.163 | 51.934 | 0.87x |
| users.json | msgspec | 6.246 | 6.410 | 27.961 | 51.934 | 0.64x |
| users.json | ujson | 23.404 | 23.516 | 33.336 | 51.934 | 0.17x |
| users.json | json | 31.647 | 32.070 | 52.139 | 51.934 | 0.13x |
| flat.json | strata | 0.609 | 0.618 | 0.677 | 50.797 | 1.00x |
| flat.json | orjson | 0.692 | 0.704 | 0.772 | 50.797 | 0.88x |
| flat.json | msgspec | 0.835 | 0.846 | 1.004 | 50.797 | 0.73x |
| flat.json | ujson | 2.804 | 2.840 | 2.892 | 50.797 | 0.22x |
| flat.json | json | 3.261 | 3.302 | 4.379 | 50.797 | 0.19x |
| nested.json | strata | 0.602 | 0.637 | 0.870 | 50.973 | 1.00x |
| nested.json | orjson | 0.652 | 0.702 | 0.910 | 50.973 | 0.91x |
| nested.json | msgspec | 0.798 | 0.816 | 0.879 | 50.973 | 0.78x |
| nested.json | ujson | 2.277 | 2.289 | 2.400 | 50.973 | 0.28x |
| nested.json | json | 3.453 | 3.482 | 3.500 | 50.973 | 0.18x |
| wide_arrays.json | strata | 2.477 | 2.504 | 2.684 | 52.762 | 1.00x |
| wide_arrays.json | orjson | 2.919 | 2.978 | 4.073 | 52.762 | 0.84x |
| wide_arrays.json | msgspec | 4.016 | 4.288 | 6.307 | 52.762 | 0.58x |
| wide_arrays.json | ujson | 14.039 | 14.223 | 19.145 | 52.762 | 0.18x |
| wide_arrays.json | json | 24.537 | 24.676 | 25.025 | 52.762 | 0.10x |
| mixed.json | strata | 0.374 | 0.379 | 0.434 | 51.672 | 1.00x |
| mixed.json | orjson | 0.378 | 0.395 | 0.430 | 51.672 | 0.96x |
| mixed.json | msgspec | 0.402 | 0.434 | 0.463 | 51.672 | 0.87x |
| mixed.json | ujson | 0.736 | 0.743 | 0.808 | 51.672 | 0.51x |
| mixed.json | json | 0.980 | 1.029 | 1.060 | 51.672 | 0.37x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.082 | 0.088 | 0.130 | 52.020 | 1.00x |
| users.json $[*].id | jmespath | 0.434 | 0.443 | 0.823 | 52.020 | 0.20x |
| users.json $[*].id | jsonpath-ng | 2.457 | 2.567 | 4.584 | 52.020 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.457 | 0.468 | 0.497 | 52.043 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.733 | 2.763 | 2.857 | 52.043 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 16.458 | 16.979 | 24.964 | 52.043 | 0.03x |
| users.json $..total | strata | 1.923 | 1.934 | 3.450 | 52.668 | 1.00x |
| users.json $..total | jsonpath-ng | 334.304 | 341.767 | 349.891 | 52.668 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.697 | 3.762 | 3.931 | 52.043 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.366 | 14.787 | 15.120 | 52.043 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 16.403 | 16.732 | 17.307 | 52.043 | 0.22x |
| users.json $[*].orders[*].total | strata | 3.892 | 3.929 | 4.069 | 52.668 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.029 | 17.238 | 18.015 | 52.668 | 0.23x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 34.067 | 34.592 | 35.419 | 52.668 | 0.11x |
| users.json $..total | strata | 12.332 | 12.782 | 13.387 | 52.668 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 345.356 | 349.580 | 362.725 | 52.668 | 0.04x |

