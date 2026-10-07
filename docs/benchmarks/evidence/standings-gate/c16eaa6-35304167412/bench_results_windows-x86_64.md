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
| users.json | strata | 8.761 | 9.370 | 12.440 | 48.891 | 1.00x |
| users.json | orjson | 13.436 | 13.777 | 15.932 | 48.891 | 0.68x |
| users.json | msgspec | 12.578 | 12.878 | 21.389 | 48.891 | 0.73x |
| users.json | ujson | 21.362 | 22.891 | 25.184 | 48.891 | 0.41x |
| users.json | json | 22.076 | 22.561 | 23.765 | 48.891 | 0.42x |
| flat.json | strata | 0.957 | 1.006 | 1.051 | 56.672 | 1.00x |
| flat.json | orjson | 1.116 | 1.138 | 1.186 | 56.672 | 0.88x |
| flat.json | msgspec | 1.095 | 1.150 | 1.190 | 56.672 | 0.87x |
| flat.json | ujson | 2.119 | 2.174 | 2.333 | 56.672 | 0.46x |
| flat.json | json | 1.969 | 1.984 | 2.032 | 56.672 | 0.51x |
| nested.json | strata | 0.762 | 0.797 | 0.817 | 56.562 | 1.00x |
| nested.json | orjson | 1.068 | 1.123 | 1.193 | 56.562 | 0.71x |
| nested.json | msgspec | 0.984 | 1.026 | 1.038 | 56.562 | 0.78x |
| nested.json | ujson | 1.537 | 1.585 | 1.609 | 56.562 | 0.50x |
| nested.json | json | 2.114 | 2.123 | 2.166 | 56.562 | 0.38x |
| wide_arrays.json | strata | 4.166 | 4.254 | 6.871 | 58.793 | 1.00x |
| wide_arrays.json | orjson | 5.686 | 5.915 | 6.291 | 58.793 | 0.72x |
| wide_arrays.json | msgspec | 5.837 | 5.964 | 6.141 | 58.793 | 0.71x |
| wide_arrays.json | ujson | 8.337 | 8.443 | 8.736 | 58.793 | 0.50x |
| wide_arrays.json | json | 11.474 | 11.791 | 18.746 | 58.793 | 0.36x |
| mixed.json | strata | 0.182 | 0.185 | 0.211 | 56.656 | 1.00x |
| mixed.json | orjson | 0.214 | 0.215 | 0.218 | 56.656 | 0.86x |
| mixed.json | msgspec | 0.233 | 0.236 | 0.265 | 56.656 | 0.78x |
| mixed.json | ujson | 0.348 | 0.357 | 0.417 | 56.656 | 0.52x |
| mixed.json | json | 0.462 | 0.473 | 0.531 | 56.656 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.936 | 2.970 | 3.016 | 49.031 | 1.00x |
| users.json | orjson | 3.675 | 3.760 | 3.862 | 49.031 | 0.79x |
| users.json | msgspec | 4.884 | 5.111 | 8.233 | 49.031 | 0.58x |
| users.json | ujson | 14.033 | 14.168 | 14.350 | 49.031 | 0.21x |
| users.json | json | 23.483 | 23.558 | 23.862 | 49.031 | 0.13x |
| flat.json | strata | 0.296 | 0.323 | 0.352 | 56.742 | 1.00x |
| flat.json | orjson | 0.359 | 0.402 | 0.480 | 56.742 | 0.80x |
| flat.json | msgspec | 0.493 | 0.508 | 0.540 | 56.742 | 0.64x |
| flat.json | ujson | 1.429 | 1.526 | 1.597 | 56.742 | 0.21x |
| flat.json | json | 1.916 | 2.126 | 2.326 | 56.742 | 0.15x |
| nested.json | strata | 0.269 | 0.277 | 0.294 | 57.215 | 1.00x |
| nested.json | orjson | 0.323 | 0.329 | 0.370 | 57.215 | 0.84x |
| nested.json | msgspec | 0.470 | 0.479 | 0.532 | 57.215 | 0.58x |
| nested.json | ujson | 1.257 | 1.297 | 1.310 | 57.215 | 0.21x |
| nested.json | json | 2.446 | 2.475 | 2.509 | 57.215 | 0.11x |
| wide_arrays.json | strata | 1.926 | 1.952 | 1.987 | 57.598 | 1.00x |
| wide_arrays.json | orjson | 2.285 | 2.549 | 2.735 | 57.598 | 0.77x |
| wide_arrays.json | msgspec | 3.706 | 3.745 | 4.119 | 57.598 | 0.52x |
| wide_arrays.json | ujson | 7.697 | 7.856 | 8.204 | 57.598 | 0.25x |
| wide_arrays.json | json | 18.644 | 18.875 | 26.502 | 57.598 | 0.10x |
| mixed.json | strata | 0.069 | 0.071 | 0.073 | 56.746 | 1.00x |
| mixed.json | orjson | 0.069 | 0.071 | 0.131 | 56.746 | 1.01x |
| mixed.json | msgspec | 0.092 | 0.096 | 0.135 | 56.746 | 0.75x |
| mixed.json | ujson | 0.269 | 0.271 | 0.322 | 56.746 | 0.26x |
| mixed.json | json | 0.507 | 0.523 | 0.554 | 56.746 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.255 | 10.805 | 11.567 | 59.055 | 1.00x |
| users.json | orjson | 14.281 | 14.629 | 16.819 | 59.055 | 0.74x |
| users.json | msgspec | 13.551 | 13.959 | 20.825 | 59.055 | 0.77x |
| users.json | ujson | 25.763 | 26.660 | 27.578 | 59.055 | 0.41x |
| users.json | json | 23.119 | 23.292 | 24.742 | 59.055 | 0.46x |
| flat.json | strata | 1.060 | 1.131 | 1.177 | 56.734 | 1.00x |
| flat.json | orjson | 1.268 | 1.316 | 1.364 | 56.734 | 0.86x |
| flat.json | msgspec | 1.298 | 1.337 | 1.399 | 56.734 | 0.85x |
| flat.json | ujson | 2.650 | 2.738 | 2.875 | 56.734 | 0.41x |
| flat.json | json | 2.105 | 2.121 | 2.157 | 56.734 | 0.53x |
| nested.json | strata | 0.834 | 0.857 | 1.319 | 57.219 | 1.00x |
| nested.json | orjson | 1.208 | 1.274 | 1.877 | 57.219 | 0.67x |
| nested.json | msgspec | 1.116 | 1.184 | 1.892 | 57.219 | 0.72x |
| nested.json | ujson | 1.981 | 2.038 | 2.156 | 57.219 | 0.42x |
| nested.json | json | 2.262 | 2.293 | 3.363 | 57.219 | 0.37x |
| wide_arrays.json | strata | 4.618 | 4.719 | 4.984 | 57.602 | 1.00x |
| wide_arrays.json | orjson | 6.023 | 6.144 | 6.363 | 57.602 | 0.77x |
| wide_arrays.json | msgspec | 6.286 | 6.435 | 8.742 | 57.602 | 0.73x |
| wide_arrays.json | ujson | 11.175 | 11.421 | 11.885 | 57.602 | 0.41x |
| wide_arrays.json | json | 12.025 | 12.177 | 12.624 | 57.602 | 0.39x |
| mixed.json | strata | 0.261 | 0.269 | 0.305 | 56.570 | 1.00x |
| mixed.json | orjson | 0.367 | 0.380 | 0.427 | 56.570 | 0.71x |
| mixed.json | msgspec | 0.383 | 0.386 | 0.460 | 56.570 | 0.70x |
| mixed.json | ujson | 0.585 | 0.597 | 0.647 | 56.570 | 0.45x |
| mixed.json | json | 0.581 | 0.633 | 0.689 | 56.570 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.657 | 12.012 | 13.905 | 57.855 | 1.00x |
| users.ndjson | orjson | 17.438 | 18.720 | 19.905 | 57.855 | 0.64x |
| users.ndjson | msgspec | 17.318 | 19.181 | 21.396 | 57.855 | 0.63x |
| users.ndjson | ujson | 25.894 | 28.704 | 29.054 | 57.855 | 0.42x |
| users.ndjson | json | 30.047 | 32.071 | 33.253 | 57.855 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.812 | 3.930 | 3.988 | 59.285 | 1.00x |
| users.json | orjson | 4.579 | 4.653 | 5.080 | 59.285 | 0.84x |
| users.json | msgspec | 5.691 | 5.986 | 6.154 | 59.285 | 0.66x |
| users.json | ujson | 23.354 | 23.690 | 27.995 | 59.285 | 0.17x |
| users.json | json | 32.573 | 32.885 | 33.542 | 59.285 | 0.12x |
| flat.json | strata | 0.612 | 0.630 | 0.701 | 57.016 | 1.00x |
| flat.json | orjson | 0.719 | 0.764 | 0.845 | 57.016 | 0.83x |
| flat.json | msgspec | 0.909 | 0.942 | 0.990 | 57.016 | 0.67x |
| flat.json | ujson | 2.866 | 2.935 | 3.062 | 57.016 | 0.21x |
| flat.json | json | 3.625 | 3.653 | 3.728 | 57.016 | 0.17x |
| nested.json | strata | 0.571 | 0.589 | 0.661 | 57.219 | 1.00x |
| nested.json | orjson | 0.661 | 0.680 | 0.711 | 57.219 | 0.87x |
| nested.json | msgspec | 0.801 | 0.831 | 0.879 | 57.219 | 0.71x |
| nested.json | ujson | 2.303 | 2.318 | 2.371 | 57.219 | 0.25x |
| nested.json | json | 3.525 | 3.573 | 3.603 | 57.219 | 0.16x |
| wide_arrays.json | strata | 2.603 | 2.662 | 2.828 | 57.598 | 1.00x |
| wide_arrays.json | orjson | 3.063 | 3.285 | 3.403 | 57.598 | 0.81x |
| wide_arrays.json | msgspec | 4.305 | 4.718 | 4.793 | 57.598 | 0.56x |
| wide_arrays.json | ujson | 14.337 | 14.684 | 15.334 | 57.598 | 0.18x |
| wide_arrays.json | json | 25.122 | 25.599 | 26.680 | 57.598 | 0.10x |
| mixed.json | strata | 0.341 | 0.347 | 0.410 | 56.688 | 1.00x |
| mixed.json | orjson | 0.376 | 0.395 | 0.461 | 56.688 | 0.88x |
| mixed.json | msgspec | 0.399 | 0.409 | 0.477 | 56.688 | 0.85x |
| mixed.json | ujson | 0.737 | 0.785 | 0.821 | 56.688 | 0.44x |
| mixed.json | json | 0.986 | 1.013 | 1.096 | 56.688 | 0.34x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.102 | 0.106 | 0.139 | 59.344 | 1.00x |
| users.json $[*].id | jmespath | 0.453 | 0.459 | 0.504 | 59.344 | 0.23x |
| users.json $[*].id | jsonpath-ng | 2.656 | 2.805 | 3.369 | 59.344 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.474 | 0.521 | 0.800 | 59.363 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.777 | 2.832 | 2.953 | 59.363 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.551 | 18.567 | 19.553 | 59.363 | 0.03x |
| users.json $..total | strata | 1.914 | 1.926 | 2.034 | 59.363 | 1.00x |
| users.json $..total | jsonpath-ng | 323.429 | 328.642 | 334.106 | 59.363 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.078 | 4.117 | 4.347 | 59.363 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.755 | 16.449 | 17.096 | 59.363 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 18.065 | 18.914 | 19.678 | 59.363 | 0.22x |
| users.json $[*].orders[*].total | strata | 4.258 | 4.291 | 4.488 | 59.363 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.980 | 18.510 | 18.945 | 59.363 | 0.23x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 35.681 | 36.939 | 41.631 | 59.363 | 0.12x |
| users.json $..total | strata | 13.342 | 14.903 | 17.479 | 59.363 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 348.203 | 351.292 | 358.455 | 59.363 | 0.04x |

