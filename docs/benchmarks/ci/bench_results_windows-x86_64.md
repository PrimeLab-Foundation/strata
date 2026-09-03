# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: af229e1
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 25 Model 17 Stepping 1, AuthenticAMD
- compiler_flags: clang-cl /std:c++20 /O2 /arch:AVX2 -fprofile-use (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.461 | 9.751 | 15.512 | 45.480 | 1.00x |
| users.json | orjson | 12.115 | 13.045 | 17.818 | 45.480 | 0.75x |
| users.json | msgspec | 10.760 | 11.773 | 19.377 | 45.480 | 0.83x |
| users.json | ujson | 18.423 | 20.353 | 30.886 | 45.480 | 0.48x |
| users.json | json | 18.914 | 20.169 | 28.582 | 45.480 | 0.48x |
| flat.json | strata | 0.739 | 0.766 | 0.833 | 48.602 | 1.00x |
| flat.json | orjson | 0.939 | 0.985 | 1.031 | 48.602 | 0.78x |
| flat.json | msgspec | 0.888 | 0.949 | 1.062 | 48.602 | 0.81x |
| flat.json | ujson | 1.839 | 1.917 | 2.030 | 48.602 | 0.40x |
| flat.json | json | 1.559 | 1.593 | 1.674 | 48.602 | 0.48x |
| nested.json | strata | 0.609 | 0.638 | 1.004 | 48.367 | 1.00x |
| nested.json | orjson | 0.877 | 0.893 | 1.682 | 48.367 | 0.71x |
| nested.json | msgspec | 0.760 | 0.780 | 1.509 | 48.367 | 0.82x |
| nested.json | ujson | 1.215 | 1.320 | 2.057 | 48.367 | 0.48x |
| nested.json | json | 1.608 | 1.640 | 2.730 | 48.367 | 0.39x |
| wide_arrays.json | strata | 4.130 | 4.303 | 5.990 | 50.051 | 1.00x |
| wide_arrays.json | orjson | 4.842 | 5.244 | 7.607 | 50.051 | 0.82x |
| wide_arrays.json | msgspec | 4.668 | 4.823 | 8.300 | 50.051 | 0.89x |
| wide_arrays.json | ujson | 6.500 | 6.626 | 7.043 | 50.051 | 0.65x |
| wide_arrays.json | json | 9.072 | 9.400 | 11.298 | 50.051 | 0.46x |
| mixed.json | strata | 0.153 | 0.167 | 0.201 | 47.918 | 1.00x |
| mixed.json | orjson | 0.182 | 0.193 | 0.319 | 47.918 | 0.87x |
| mixed.json | msgspec | 0.191 | 0.203 | 0.231 | 47.918 | 0.82x |
| mixed.json | ujson | 0.301 | 0.320 | 0.438 | 47.918 | 0.52x |
| mixed.json | json | 0.378 | 0.408 | 0.566 | 47.918 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.751 | 2.842 | 2.972 | 44.719 | 1.00x |
| users.json | orjson | 2.816 | 2.909 | 3.041 | 44.719 | 0.98x |
| users.json | msgspec | 4.754 | 4.992 | 8.374 | 44.719 | 0.57x |
| users.json | ujson | 13.019 | 13.471 | 18.188 | 44.719 | 0.21x |
| users.json | json | 18.575 | 19.182 | 19.964 | 44.719 | 0.15x |
| flat.json | strata | 0.246 | 0.265 | 0.350 | 48.047 | 1.00x |
| flat.json | orjson | 0.269 | 0.321 | 0.432 | 48.047 | 0.82x |
| flat.json | msgspec | 0.429 | 0.467 | 1.312 | 48.047 | 0.57x |
| flat.json | ujson | 1.291 | 1.411 | 1.954 | 48.047 | 0.19x |
| flat.json | json | 1.549 | 1.728 | 3.212 | 48.047 | 0.15x |
| nested.json | strata | 0.216 | 0.230 | 0.253 | 48.371 | 1.00x |
| nested.json | orjson | 0.237 | 0.248 | 0.271 | 48.371 | 0.93x |
| nested.json | msgspec | 0.413 | 0.421 | 0.455 | 48.371 | 0.55x |
| nested.json | ujson | 1.020 | 1.050 | 1.191 | 48.371 | 0.22x |
| nested.json | json | 1.957 | 2.017 | 2.109 | 48.371 | 0.11x |
| wide_arrays.json | strata | 1.679 | 1.708 | 2.397 | 49.473 | 1.00x |
| wide_arrays.json | orjson | 2.064 | 2.091 | 3.158 | 49.473 | 0.82x |
| wide_arrays.json | msgspec | 3.446 | 3.561 | 4.835 | 49.473 | 0.48x |
| wide_arrays.json | ujson | 7.974 | 8.206 | 9.290 | 49.473 | 0.21x |
| wide_arrays.json | json | 14.498 | 14.813 | 18.300 | 49.473 | 0.12x |
| mixed.json | strata | 0.065 | 0.074 | 0.140 | 48.020 | 1.00x |
| mixed.json | orjson | 0.054 | 0.062 | 0.099 | 48.020 | 1.19x |
| mixed.json | msgspec | 0.090 | 0.100 | 0.127 | 48.020 | 0.74x |
| mixed.json | ujson | 0.264 | 0.279 | 0.388 | 48.020 | 0.27x |
| mixed.json | json | 0.407 | 0.438 | 0.644 | 48.020 | 0.17x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.968 | 11.119 | 15.324 | 54.750 | 1.00x |
| users.json | orjson | 12.999 | 13.426 | 15.556 | 54.750 | 0.83x |
| users.json | msgspec | 11.719 | 12.359 | 13.766 | 54.750 | 0.90x |
| users.json | ujson | 22.722 | 23.553 | 32.378 | 54.750 | 0.47x |
| users.json | json | 19.340 | 20.557 | 28.408 | 54.750 | 0.54x |
| flat.json | strata | 0.894 | 0.954 | 1.032 | 48.512 | 1.00x |
| flat.json | orjson | 1.209 | 1.246 | 1.342 | 48.512 | 0.77x |
| flat.json | msgspec | 1.021 | 1.072 | 1.200 | 48.512 | 0.89x |
| flat.json | ujson | 2.238 | 2.315 | 2.380 | 48.512 | 0.41x |
| flat.json | json | 1.666 | 1.751 | 1.820 | 48.512 | 0.54x |
| nested.json | strata | 0.702 | 0.779 | 0.971 | 48.090 | 1.00x |
| nested.json | orjson | 0.966 | 1.017 | 1.039 | 48.090 | 0.77x |
| nested.json | msgspec | 0.863 | 0.919 | 0.967 | 48.090 | 0.85x |
| nested.json | ujson | 1.479 | 1.617 | 1.733 | 48.090 | 0.48x |
| nested.json | json | 1.741 | 1.767 | 2.367 | 48.090 | 0.44x |
| wide_arrays.json | strata | 4.718 | 4.980 | 7.079 | 49.473 | 1.00x |
| wide_arrays.json | orjson | 5.403 | 5.579 | 9.213 | 49.473 | 0.89x |
| wide_arrays.json | msgspec | 5.318 | 5.468 | 7.457 | 49.473 | 0.91x |
| wide_arrays.json | ujson | 8.764 | 8.939 | 9.868 | 49.473 | 0.56x |
| wide_arrays.json | json | 9.727 | 10.053 | 10.583 | 49.473 | 0.50x |
| mixed.json | strata | 0.231 | 0.250 | 0.337 | 48.176 | 1.00x |
| mixed.json | orjson | 0.276 | 0.301 | 0.354 | 48.176 | 0.83x |
| mixed.json | msgspec | 0.283 | 0.320 | 0.432 | 48.176 | 0.78x |
| mixed.json | ujson | 0.439 | 0.527 | 0.565 | 48.176 | 0.47x |
| mixed.json | json | 0.475 | 0.499 | 0.575 | 48.176 | 0.50x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.946 | 11.493 | 17.992 | 49.086 | 1.00x |
| users.ndjson | orjson | 15.992 | 16.842 | 19.531 | 49.086 | 0.68x |
| users.ndjson | msgspec | 14.572 | 16.780 | 17.958 | 49.086 | 0.68x |
| users.ndjson | ujson | 22.706 | 25.261 | 26.659 | 49.086 | 0.45x |
| users.ndjson | json | 26.464 | 28.138 | 39.393 | 49.086 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.630 | 3.875 | 11.126 | 51.801 | 1.00x |
| users.json | orjson | 3.753 | 4.020 | 7.429 | 51.801 | 0.96x |
| users.json | msgspec | 5.851 | 6.154 | 8.588 | 51.801 | 0.63x |
| users.json | ujson | 20.457 | 21.250 | 21.651 | 51.801 | 0.18x |
| users.json | json | 26.354 | 26.896 | 28.157 | 51.801 | 0.14x |
| flat.json | strata | 0.568 | 0.585 | 0.642 | 48.285 | 1.00x |
| flat.json | orjson | 0.562 | 0.609 | 0.694 | 48.285 | 0.96x |
| flat.json | msgspec | 0.771 | 0.824 | 36.504 | 48.285 | 0.71x |
| flat.json | ujson | 2.465 | 2.547 | 2.719 | 48.285 | 0.23x |
| flat.json | json | 2.867 | 3.012 | 3.162 | 48.285 | 0.19x |
| nested.json | strata | 0.509 | 0.555 | 0.611 | 48.598 | 1.00x |
| nested.json | orjson | 0.541 | 0.580 | 0.630 | 48.598 | 0.96x |
| nested.json | msgspec | 0.742 | 0.807 | 0.873 | 48.598 | 0.69x |
| nested.json | ujson | 1.865 | 1.917 | 2.099 | 48.598 | 0.29x |
| nested.json | json | 2.812 | 2.866 | 3.039 | 48.598 | 0.19x |
| wide_arrays.json | strata | 2.433 | 2.573 | 2.931 | 49.473 | 1.00x |
| wide_arrays.json | orjson | 2.626 | 2.815 | 3.044 | 49.473 | 0.91x |
| wide_arrays.json | msgspec | 4.137 | 4.270 | 4.756 | 49.473 | 0.60x |
| wide_arrays.json | ujson | 13.295 | 13.749 | 22.668 | 49.473 | 0.19x |
| wide_arrays.json | json | 20.334 | 21.252 | 30.749 | 49.473 | 0.12x |
| mixed.json | strata | 0.321 | 0.364 | 0.475 | 48.203 | 1.00x |
| mixed.json | orjson | 0.315 | 0.344 | 0.484 | 48.203 | 1.06x |
| mixed.json | msgspec | 0.345 | 0.389 | 0.735 | 48.203 | 0.93x |
| mixed.json | ujson | 0.651 | 0.675 | 1.013 | 48.203 | 0.54x |
| mixed.json | json | 0.813 | 0.911 | 1.540 | 48.203 | 0.40x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.087 | 0.104 | 0.155 | 51.832 | 1.00x |
| users.json $[*].id | jmespath | 0.354 | 0.394 | 0.748 | 51.832 | 0.26x |
| users.json $[*].id | jsonpath-ng | 2.242 | 2.640 | 5.418 | 51.832 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.443 | 0.527 | 0.726 | 51.902 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.172 | 2.536 | 6.158 | 51.902 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 14.189 | 19.257 | 48.718 | 51.902 | 0.03x |
| users.json $..total | strata | 1.512 | 1.801 | 3.197 | 53.062 | 1.00x |
| users.json $..total | jsonpath-ng | 254.682 | 294.881 | 530.526 | 53.062 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.455 | 3.630 | 8.913 | 51.855 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.443 | 14.644 | 289.331 | 51.855 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 15.127 | 16.109 | 206.203 | 51.855 | 0.23x |
| users.json $[*].orders[*].total | strata | 3.698 | 3.761 | 10.912 | 52.984 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.755 | 16.444 | 33.176 | 52.984 | 0.23x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 33.149 | 37.986 | 105.493 | 52.984 | 0.10x |
| users.json $..total | strata | 13.167 | 16.026 | 25.462 | 52.688 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 279.281 | 299.428 | 431.020 | 52.688 | 0.05x |

