# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: ec53f936b8a9245edf5bfeec36c38539666a1775
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD
- compiler_flags: /nologo /O2 /W3 /DNDEBUG /MD /EHsc /std:c++20 /Zc:__cplusplus /arch:AVX2 /clang:-O3 /clang:-fprofile-use=D:\a\strata\strata\build\pgo\strata.profdata /DLL /MANIFEST:EMBED,ID=2 /MANIFESTUAC:NO /EXPORT:PyInit__strata (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.969 | 9.548 | 12.449 | 48.730 | 1.00x |
| users.json | orjson | 12.844 | 13.697 | 17.585 | 48.730 | 0.70x |
| users.json | msgspec | 12.807 | 13.345 | 20.010 | 48.730 | 0.72x |
| users.json | ujson | 20.206 | 22.635 | 34.303 | 48.730 | 0.42x |
| users.json | json | 22.294 | 24.506 | 34.829 | 48.730 | 0.39x |
| flat.json | strata | 0.990 | 1.010 | 1.049 | 56.902 | 1.00x |
| flat.json | orjson | 1.100 | 1.141 | 1.564 | 56.902 | 0.89x |
| flat.json | msgspec | 1.153 | 1.177 | 1.208 | 56.902 | 0.86x |
| flat.json | ujson | 2.134 | 2.154 | 2.293 | 56.902 | 0.47x |
| flat.json | json | 1.917 | 1.932 | 1.966 | 56.902 | 0.52x |
| nested.json | strata | 0.734 | 0.745 | 0.837 | 56.969 | 1.00x |
| nested.json | orjson | 1.043 | 1.079 | 1.134 | 56.969 | 0.69x |
| nested.json | msgspec | 0.981 | 1.016 | 1.166 | 56.969 | 0.73x |
| nested.json | ujson | 1.513 | 1.554 | 1.596 | 56.969 | 0.48x |
| nested.json | json | 2.117 | 2.125 | 2.160 | 56.969 | 0.35x |
| wide_arrays.json | strata | 4.127 | 4.196 | 4.310 | 58.617 | 1.00x |
| wide_arrays.json | orjson | 5.645 | 5.692 | 6.114 | 58.617 | 0.74x |
| wide_arrays.json | msgspec | 5.758 | 5.943 | 6.602 | 58.617 | 0.71x |
| wide_arrays.json | ujson | 8.172 | 8.311 | 9.030 | 58.617 | 0.50x |
| wide_arrays.json | json | 11.702 | 11.773 | 12.595 | 58.617 | 0.36x |
| mixed.json | strata | 0.184 | 0.185 | 0.191 | 56.484 | 1.00x |
| mixed.json | orjson | 0.212 | 0.216 | 0.255 | 56.484 | 0.86x |
| mixed.json | msgspec | 0.235 | 0.240 | 0.291 | 56.484 | 0.77x |
| mixed.json | ujson | 0.346 | 0.354 | 0.438 | 56.484 | 0.52x |
| mixed.json | json | 0.470 | 0.481 | 0.574 | 56.484 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.215 | 3.230 | 4.352 | 47.902 | 1.00x |
| users.json | orjson | 3.696 | 3.778 | 5.255 | 47.902 | 0.85x |
| users.json | msgspec | 5.285 | 5.461 | 6.022 | 47.902 | 0.59x |
| users.json | ujson | 14.300 | 14.446 | 20.297 | 47.902 | 0.22x |
| users.json | json | 23.793 | 26.134 | 34.108 | 47.902 | 0.12x |
| flat.json | strata | 0.310 | 0.312 | 0.354 | 57.176 | 1.00x |
| flat.json | orjson | 0.355 | 0.357 | 0.389 | 57.176 | 0.87x |
| flat.json | msgspec | 0.488 | 0.491 | 0.552 | 57.176 | 0.64x |
| flat.json | ujson | 1.510 | 1.562 | 1.578 | 57.176 | 0.20x |
| flat.json | json | 1.964 | 1.979 | 2.079 | 57.176 | 0.16x |
| nested.json | strata | 0.300 | 0.300 | 0.338 | 56.906 | 1.00x |
| nested.json | orjson | 0.323 | 0.325 | 0.335 | 56.906 | 0.92x |
| nested.json | msgspec | 0.463 | 0.469 | 0.499 | 56.906 | 0.64x |
| nested.json | ujson | 1.141 | 1.145 | 1.182 | 56.906 | 0.26x |
| nested.json | json | 2.439 | 2.455 | 2.490 | 56.906 | 0.12x |
| wide_arrays.json | strata | 2.005 | 2.034 | 2.254 | 59.695 | 1.00x |
| wide_arrays.json | orjson | 2.271 | 2.374 | 2.570 | 59.695 | 0.86x |
| wide_arrays.json | msgspec | 3.653 | 3.948 | 4.047 | 59.695 | 0.52x |
| wide_arrays.json | ujson | 7.484 | 7.678 | 7.824 | 59.695 | 0.26x |
| wide_arrays.json | json | 18.373 | 18.637 | 19.112 | 59.695 | 0.11x |
| mixed.json | strata | 0.073 | 0.075 | 0.131 | 56.543 | 1.00x |
| mixed.json | orjson | 0.068 | 0.069 | 0.125 | 56.543 | 1.08x |
| mixed.json | msgspec | 0.090 | 0.091 | 0.137 | 56.543 | 0.82x |
| mixed.json | ujson | 0.263 | 0.269 | 0.480 | 56.543 | 0.28x |
| mixed.json | json | 0.508 | 0.546 | 0.933 | 56.543 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.010 | 11.365 | 15.766 | 57.930 | 1.00x |
| users.json | orjson | 14.264 | 15.192 | 16.615 | 57.930 | 0.75x |
| users.json | msgspec | 14.038 | 15.466 | 19.436 | 57.930 | 0.73x |
| users.json | ujson | 27.019 | 27.675 | 43.285 | 57.930 | 0.41x |
| users.json | json | 24.000 | 24.706 | 35.339 | 57.930 | 0.46x |
| flat.json | strata | 0.958 | 1.024 | 1.153 | 57.547 | 1.00x |
| flat.json | orjson | 1.381 | 1.452 | 1.492 | 57.547 | 0.71x |
| flat.json | msgspec | 1.216 | 1.282 | 1.312 | 57.547 | 0.80x |
| flat.json | ujson | 2.637 | 2.695 | 2.777 | 57.547 | 0.38x |
| flat.json | json | 2.081 | 2.115 | 2.146 | 57.547 | 0.48x |
| nested.json | strata | 0.819 | 0.860 | 0.931 | 56.809 | 1.00x |
| nested.json | orjson | 1.173 | 1.203 | 1.245 | 56.809 | 0.72x |
| nested.json | msgspec | 1.108 | 1.153 | 1.275 | 56.809 | 0.75x |
| nested.json | ujson | 1.942 | 1.976 | 2.091 | 56.809 | 0.44x |
| nested.json | json | 2.257 | 2.316 | 2.534 | 56.809 | 0.37x |
| wide_arrays.json | strata | 4.556 | 4.638 | 5.004 | 59.695 | 1.00x |
| wide_arrays.json | orjson | 6.050 | 6.116 | 9.222 | 59.695 | 0.76x |
| wide_arrays.json | msgspec | 6.204 | 6.317 | 6.362 | 59.695 | 0.73x |
| wide_arrays.json | ujson | 11.230 | 11.467 | 12.390 | 59.695 | 0.40x |
| wide_arrays.json | json | 12.075 | 12.230 | 12.624 | 59.695 | 0.38x |
| mixed.json | strata | 0.259 | 0.267 | 0.306 | 56.543 | 1.00x |
| mixed.json | orjson | 0.323 | 0.335 | 0.397 | 56.543 | 0.80x |
| mixed.json | msgspec | 0.346 | 0.352 | 0.402 | 56.543 | 0.76x |
| mixed.json | ujson | 0.526 | 0.544 | 0.605 | 56.543 | 0.49x |
| mixed.json | json | 0.580 | 0.587 | 0.639 | 56.543 | 0.46x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.497 | 10.893 | 11.827 | 58.055 | 1.00x |
| users.ndjson | orjson | 17.152 | 17.476 | 19.099 | 58.055 | 0.62x |
| users.ndjson | msgspec | 17.518 | 17.870 | 18.483 | 58.055 | 0.61x |
| users.ndjson | ujson | 25.686 | 26.126 | 27.646 | 58.055 | 0.42x |
| users.ndjson | json | 30.253 | 30.927 | 31.335 | 58.055 | 0.35x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.031 | 4.300 | 5.594 | 58.059 | 1.00x |
| users.json | orjson | 4.357 | 4.516 | 4.883 | 58.059 | 0.95x |
| users.json | msgspec | 5.860 | 6.081 | 6.242 | 58.059 | 0.71x |
| users.json | ujson | 23.169 | 23.684 | 34.935 | 58.059 | 0.18x |
| users.json | json | 32.546 | 32.989 | 41.363 | 58.059 | 0.13x |
| flat.json | strata | 0.645 | 0.695 | 0.826 | 57.160 | 1.00x |
| flat.json | orjson | 0.695 | 0.704 | 0.784 | 57.160 | 0.99x |
| flat.json | msgspec | 0.839 | 0.874 | 1.140 | 57.160 | 0.79x |
| flat.json | ujson | 2.808 | 2.844 | 3.325 | 57.160 | 0.24x |
| flat.json | json | 3.324 | 3.374 | 3.604 | 57.160 | 0.21x |
| nested.json | strata | 0.638 | 0.702 | 0.786 | 57.008 | 1.00x |
| nested.json | orjson | 0.661 | 0.699 | 0.809 | 57.008 | 1.00x |
| nested.json | msgspec | 0.803 | 0.854 | 0.891 | 57.008 | 0.82x |
| nested.json | ujson | 2.299 | 2.339 | 2.465 | 57.008 | 0.30x |
| nested.json | json | 3.479 | 3.550 | 3.701 | 57.008 | 0.20x |
| wide_arrays.json | strata | 2.660 | 2.727 | 13.525 | 57.430 | 1.00x |
| wide_arrays.json | orjson | 2.936 | 3.011 | 3.213 | 57.430 | 0.91x |
| wide_arrays.json | msgspec | 4.334 | 4.661 | 5.802 | 57.430 | 0.59x |
| wide_arrays.json | ujson | 14.355 | 14.479 | 14.805 | 57.430 | 0.19x |
| wide_arrays.json | json | 25.045 | 25.260 | 26.693 | 57.430 | 0.11x |
| mixed.json | strata | 0.383 | 0.386 | 0.475 | 56.594 | 1.00x |
| mixed.json | orjson | 0.378 | 0.380 | 0.442 | 56.594 | 1.01x |
| mixed.json | msgspec | 0.398 | 0.408 | 0.465 | 56.594 | 0.95x |
| mixed.json | ujson | 0.742 | 0.758 | 0.803 | 56.594 | 0.51x |
| mixed.json | json | 0.991 | 1.053 | 1.082 | 56.594 | 0.37x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.086 | 0.088 | 0.110 | 57.715 | 1.00x |
| users.json $[*].id | jmespath | 0.441 | 0.466 | 0.506 | 57.715 | 0.19x |
| users.json $[*].id | jsonpath-ng | 2.519 | 2.561 | 3.321 | 57.715 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.458 | 0.461 | 0.518 | 58.074 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.743 | 2.833 | 2.897 | 58.074 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 16.551 | 17.218 | 20.451 | 58.074 | 0.03x |
| users.json $..total | strata | 1.888 | 1.918 | 3.350 | 58.078 | 1.00x |
| users.json $..total | jsonpath-ng | 334.815 | 337.573 | 350.847 | 58.078 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.999 | 4.049 | 4.109 | 57.754 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.181 | 15.471 | 16.138 | 57.754 | 0.26x |
| users.json $[*].id | orjson+jsonpath-ng | 16.904 | 17.175 | 17.677 | 57.754 | 0.24x |
| users.json $[*].orders[*].total | strata | 4.199 | 4.249 | 4.325 | 58.078 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.741 | 18.187 | 20.378 | 58.078 | 0.23x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 34.691 | 35.773 | 42.324 | 58.078 | 0.12x |
| users.json $..total | strata | 13.467 | 15.221 | 17.842 | 58.062 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 351.060 | 356.704 | 365.104 | 58.062 | 0.04x |

