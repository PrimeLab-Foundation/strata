# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c16eaa65c61f1cf9d4fac46e9f6d8e6b9f3b087f
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD
- compiler_flags: /nologo /O2 /W3 /DNDEBUG /MD /EHsc /std:c++20 /Zc:__cplusplus /D_USE_STD_VECTOR_ALGORITHMS=0 /arch:AVX2 /clang:-O3 /clang:-fprofile-use=D:\a\strata\strata\build\pgo\strata.profdata /DLL /MANIFEST:EMBED,ID=2 /MANIFESTUAC:NO /EXPORT:PyInit__strata (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.079 | 10.638 | 15.138 | 48.883 | 1.00x |
| users.json | orjson | 13.299 | 15.055 | 20.995 | 48.883 | 0.71x |
| users.json | msgspec | 12.648 | 14.230 | 21.285 | 48.883 | 0.75x |
| users.json | ujson | 21.328 | 23.619 | 32.128 | 48.883 | 0.45x |
| users.json | json | 22.543 | 23.621 | 29.960 | 48.883 | 0.45x |
| flat.json | strata | 0.958 | 1.047 | 1.071 | 57.441 | 1.00x |
| flat.json | orjson | 1.091 | 1.134 | 1.182 | 57.441 | 0.92x |
| flat.json | msgspec | 1.061 | 1.087 | 1.107 | 57.441 | 0.96x |
| flat.json | ujson | 2.102 | 2.136 | 3.146 | 57.441 | 0.49x |
| flat.json | json | 1.937 | 1.996 | 3.524 | 57.441 | 0.52x |
| nested.json | strata | 0.739 | 0.755 | 0.793 | 57.160 | 1.00x |
| nested.json | orjson | 1.042 | 1.069 | 1.102 | 57.160 | 0.71x |
| nested.json | msgspec | 0.984 | 1.022 | 1.043 | 57.160 | 0.74x |
| nested.json | ujson | 1.515 | 1.563 | 1.638 | 57.160 | 0.48x |
| nested.json | json | 2.119 | 2.139 | 2.174 | 57.160 | 0.35x |
| wide_arrays.json | strata | 4.156 | 4.182 | 4.331 | 59.129 | 1.00x |
| wide_arrays.json | orjson | 5.547 | 5.584 | 5.641 | 59.129 | 0.75x |
| wide_arrays.json | msgspec | 5.707 | 5.740 | 6.854 | 59.129 | 0.73x |
| wide_arrays.json | ujson | 8.180 | 8.240 | 8.387 | 59.129 | 0.51x |
| wide_arrays.json | json | 11.533 | 11.604 | 11.834 | 59.129 | 0.36x |
| mixed.json | strata | 0.188 | 0.193 | 0.221 | 57.098 | 1.00x |
| mixed.json | orjson | 0.214 | 0.217 | 0.225 | 57.098 | 0.89x |
| mixed.json | msgspec | 0.234 | 0.238 | 0.259 | 57.098 | 0.81x |
| mixed.json | ujson | 0.347 | 0.355 | 0.419 | 57.098 | 0.54x |
| mixed.json | json | 0.475 | 0.486 | 0.533 | 57.098 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.914 | 2.970 | 4.206 | 49.055 | 1.00x |
| users.json | orjson | 3.557 | 3.750 | 4.944 | 49.055 | 0.79x |
| users.json | msgspec | 4.849 | 5.238 | 8.235 | 49.055 | 0.57x |
| users.json | ujson | 14.249 | 16.165 | 24.609 | 49.055 | 0.18x |
| users.json | json | 22.737 | 23.388 | 36.630 | 49.055 | 0.13x |
| flat.json | strata | 0.294 | 0.297 | 0.397 | 58.031 | 1.00x |
| flat.json | orjson | 0.353 | 0.356 | 0.546 | 58.031 | 0.84x |
| flat.json | msgspec | 0.493 | 0.495 | 0.560 | 58.031 | 0.60x |
| flat.json | ujson | 1.531 | 1.553 | 1.649 | 58.031 | 0.19x |
| flat.json | json | 1.919 | 1.923 | 1.972 | 58.031 | 0.15x |
| nested.json | strata | 0.271 | 0.273 | 0.277 | 57.652 | 1.00x |
| nested.json | orjson | 0.321 | 0.322 | 0.362 | 57.652 | 0.85x |
| nested.json | msgspec | 0.464 | 0.486 | 0.505 | 57.652 | 0.56x |
| nested.json | ujson | 1.159 | 1.178 | 1.213 | 57.652 | 0.23x |
| nested.json | json | 2.407 | 2.412 | 2.446 | 57.652 | 0.11x |
| wide_arrays.json | strata | 1.904 | 1.955 | 2.927 | 58.449 | 1.00x |
| wide_arrays.json | orjson | 2.515 | 2.559 | 3.677 | 58.449 | 0.76x |
| wide_arrays.json | msgspec | 3.974 | 4.082 | 5.777 | 58.449 | 0.48x |
| wide_arrays.json | ujson | 7.614 | 7.687 | 8.986 | 58.449 | 0.25x |
| wide_arrays.json | json | 18.698 | 18.830 | 22.690 | 58.449 | 0.10x |
| mixed.json | strata | 0.070 | 0.074 | 0.195 | 57.328 | 1.00x |
| mixed.json | orjson | 0.070 | 0.089 | 0.179 | 57.328 | 0.84x |
| mixed.json | msgspec | 0.096 | 0.116 | 0.178 | 57.328 | 0.64x |
| mixed.json | ujson | 0.268 | 0.288 | 0.481 | 57.328 | 0.26x |
| mixed.json | json | 0.518 | 0.558 | 1.036 | 57.328 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.401 | 10.694 | 17.490 | 59.078 | 1.00x |
| users.json | orjson | 14.205 | 14.461 | 23.475 | 59.078 | 0.74x |
| users.json | msgspec | 13.770 | 14.102 | 23.108 | 59.078 | 0.76x |
| users.json | ujson | 25.374 | 25.926 | 31.158 | 59.078 | 0.41x |
| users.json | json | 23.185 | 23.695 | 35.451 | 59.078 | 0.45x |
| flat.json | strata | 1.055 | 1.091 | 1.129 | 57.469 | 1.00x |
| flat.json | orjson | 1.220 | 1.243 | 1.538 | 57.469 | 0.88x |
| flat.json | msgspec | 1.251 | 1.290 | 1.336 | 57.469 | 0.85x |
| flat.json | ujson | 2.671 | 2.725 | 2.755 | 57.469 | 0.40x |
| flat.json | json | 2.100 | 2.112 | 2.142 | 57.469 | 0.52x |
| nested.json | strata | 0.829 | 0.863 | 1.215 | 57.168 | 1.00x |
| nested.json | orjson | 1.146 | 1.208 | 1.771 | 57.168 | 0.71x |
| nested.json | msgspec | 1.131 | 1.162 | 1.867 | 57.168 | 0.74x |
| nested.json | ujson | 1.971 | 2.010 | 3.281 | 57.168 | 0.43x |
| nested.json | json | 2.292 | 2.327 | 2.399 | 57.168 | 0.37x |
| wide_arrays.json | strata | 4.596 | 4.649 | 5.565 | 58.449 | 1.00x |
| wide_arrays.json | orjson | 5.841 | 6.058 | 6.176 | 58.449 | 0.77x |
| wide_arrays.json | msgspec | 6.203 | 6.261 | 10.064 | 58.449 | 0.74x |
| wide_arrays.json | ujson | 11.063 | 11.330 | 14.555 | 58.449 | 0.41x |
| wide_arrays.json | json | 11.920 | 12.143 | 15.636 | 58.449 | 0.38x |
| mixed.json | strata | 0.255 | 0.270 | 0.302 | 57.484 | 1.00x |
| mixed.json | orjson | 0.322 | 0.333 | 0.357 | 57.484 | 0.81x |
| mixed.json | msgspec | 0.340 | 0.354 | 0.412 | 57.484 | 0.76x |
| mixed.json | ujson | 0.533 | 0.553 | 0.619 | 57.484 | 0.49x |
| mixed.json | json | 0.584 | 0.603 | 0.633 | 57.484 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.393 | 10.724 | 17.075 | 58.586 | 1.00x |
| users.ndjson | orjson | 16.825 | 17.333 | 19.856 | 58.586 | 0.62x |
| users.ndjson | msgspec | 17.232 | 17.651 | 28.050 | 58.586 | 0.61x |
| users.ndjson | ujson | 25.068 | 25.497 | 26.843 | 58.586 | 0.42x |
| users.ndjson | json | 29.463 | 29.769 | 30.959 | 58.586 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.692 | 3.803 | 5.235 | 59.273 | 1.00x |
| users.json | orjson | 4.373 | 4.521 | 6.551 | 59.273 | 0.84x |
| users.json | msgspec | 5.690 | 6.071 | 9.254 | 59.273 | 0.63x |
| users.json | ujson | 23.030 | 23.465 | 48.632 | 59.273 | 0.16x |
| users.json | json | 31.614 | 32.004 | 32.926 | 59.273 | 0.12x |
| flat.json | strata | 0.614 | 0.639 | 0.871 | 57.727 | 1.00x |
| flat.json | orjson | 0.702 | 0.713 | 0.972 | 57.727 | 0.90x |
| flat.json | msgspec | 0.847 | 0.900 | 1.217 | 57.727 | 0.71x |
| flat.json | ujson | 2.812 | 2.862 | 3.028 | 57.727 | 0.22x |
| flat.json | json | 3.267 | 3.316 | 5.708 | 57.727 | 0.19x |
| nested.json | strata | 0.579 | 0.599 | 0.661 | 57.449 | 1.00x |
| nested.json | orjson | 0.669 | 0.729 | 0.772 | 57.449 | 0.82x |
| nested.json | msgspec | 0.808 | 0.828 | 0.876 | 57.449 | 0.72x |
| nested.json | ujson | 2.311 | 2.339 | 14.808 | 57.449 | 0.26x |
| nested.json | json | 3.459 | 3.500 | 3.816 | 57.449 | 0.17x |
| wide_arrays.json | strata | 2.575 | 2.632 | 2.758 | 58.449 | 1.00x |
| wide_arrays.json | orjson | 3.235 | 3.302 | 6.379 | 58.449 | 0.80x |
| wide_arrays.json | msgspec | 4.630 | 4.756 | 4.913 | 58.449 | 0.55x |
| wide_arrays.json | ujson | 14.401 | 14.586 | 15.716 | 58.449 | 0.18x |
| wide_arrays.json | json | 25.454 | 25.631 | 26.503 | 58.449 | 0.10x |
| mixed.json | strata | 0.345 | 0.355 | 0.410 | 57.516 | 1.00x |
| mixed.json | orjson | 0.381 | 0.388 | 0.458 | 57.516 | 0.91x |
| mixed.json | msgspec | 0.408 | 0.413 | 0.455 | 57.516 | 0.86x |
| mixed.json | ujson | 0.751 | 0.808 | 0.823 | 57.516 | 0.44x |
| mixed.json | json | 0.991 | 1.020 | 1.052 | 57.516 | 0.35x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.080 | 0.086 | 0.089 | 59.320 | 1.00x |
| users.json $[*].id | jmespath | 0.438 | 0.449 | 0.592 | 59.320 | 0.19x |
| users.json $[*].id | jsonpath-ng | 2.443 | 2.548 | 2.639 | 59.320 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.443 | 0.487 | 0.879 | 59.340 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.756 | 2.847 | 5.806 | 59.340 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 16.944 | 18.242 | 25.758 | 59.340 | 0.03x |
| users.json $..total | strata | 1.894 | 1.903 | 2.217 | 59.340 | 1.00x |
| users.json $..total | jsonpath-ng | 334.475 | 337.484 | 352.711 | 59.340 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.011 | 4.049 | 4.091 | 59.340 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.241 | 15.409 | 20.569 | 59.340 | 0.26x |
| users.json $[*].id | orjson+jsonpath-ng | 17.182 | 17.684 | 22.434 | 59.340 | 0.23x |
| users.json $[*].orders[*].total | strata | 4.172 | 4.245 | 5.591 | 59.340 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.935 | 18.240 | 19.694 | 59.340 | 0.23x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 34.680 | 35.769 | 39.354 | 59.340 | 0.12x |
| users.json $..total | strata | 13.970 | 15.004 | 15.560 | 59.340 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 349.924 | 354.442 | 369.000 | 59.340 | 0.04x |

