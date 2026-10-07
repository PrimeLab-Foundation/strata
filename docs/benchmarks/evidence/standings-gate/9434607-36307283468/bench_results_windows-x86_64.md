# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 943460734d2b79bd16c949d8d97f4d22cc202e84
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
| users.json | strata | 8.818 | 9.214 | 14.176 | 48.898 | 1.00x |
| users.json | orjson | 13.145 | 14.062 | 18.020 | 48.898 | 0.66x |
| users.json | msgspec | 12.827 | 13.041 | 15.562 | 48.898 | 0.71x |
| users.json | ujson | 20.709 | 21.847 | 25.721 | 48.898 | 0.42x |
| users.json | json | 22.474 | 23.696 | 25.695 | 48.898 | 0.39x |
| flat.json | strata | 0.818 | 0.855 | 0.892 | 58.070 | 1.00x |
| flat.json | orjson | 1.095 | 1.128 | 1.199 | 58.070 | 0.76x |
| flat.json | msgspec | 1.108 | 1.146 | 1.171 | 58.070 | 0.75x |
| flat.json | ujson | 2.076 | 2.115 | 2.246 | 58.070 | 0.40x |
| flat.json | json | 2.008 | 2.022 | 2.056 | 58.070 | 0.42x |
| nested.json | strata | 0.746 | 0.768 | 1.005 | 57.316 | 1.00x |
| nested.json | orjson | 1.069 | 1.093 | 1.632 | 57.316 | 0.70x |
| nested.json | msgspec | 1.001 | 1.044 | 1.734 | 57.316 | 0.74x |
| nested.json | ujson | 1.586 | 1.651 | 2.737 | 57.316 | 0.46x |
| nested.json | json | 2.130 | 2.166 | 2.420 | 57.316 | 0.35x |
| wide_arrays.json | strata | 4.201 | 4.265 | 6.155 | 59.312 | 1.00x |
| wide_arrays.json | orjson | 5.644 | 5.800 | 10.093 | 59.312 | 0.74x |
| wide_arrays.json | msgspec | 5.696 | 5.778 | 22.505 | 59.312 | 0.74x |
| wide_arrays.json | ujson | 8.180 | 8.416 | 9.689 | 59.312 | 0.51x |
| wide_arrays.json | json | 11.805 | 11.992 | 14.229 | 59.312 | 0.36x |
| mixed.json | strata | 0.182 | 0.184 | 0.209 | 57.391 | 1.00x |
| mixed.json | orjson | 0.212 | 0.216 | 0.285 | 57.391 | 0.85x |
| mixed.json | msgspec | 0.232 | 0.236 | 0.262 | 57.391 | 0.78x |
| mixed.json | ujson | 0.342 | 0.348 | 0.381 | 57.391 | 0.53x |
| mixed.json | json | 0.474 | 0.477 | 0.534 | 57.391 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.928 | 2.993 | 4.149 | 49.125 | 1.00x |
| users.json | orjson | 3.439 | 3.498 | 3.680 | 49.125 | 0.86x |
| users.json | msgspec | 4.834 | 4.892 | 7.606 | 49.125 | 0.61x |
| users.json | ujson | 14.072 | 14.515 | 15.016 | 49.125 | 0.21x |
| users.json | json | 23.457 | 24.152 | 36.634 | 49.125 | 0.12x |
| flat.json | strata | 0.287 | 0.290 | 0.449 | 57.508 | 1.00x |
| flat.json | orjson | 0.355 | 0.365 | 0.412 | 57.508 | 0.79x |
| flat.json | msgspec | 0.489 | 0.494 | 0.531 | 57.508 | 0.59x |
| flat.json | ujson | 1.415 | 1.457 | 1.729 | 57.508 | 0.20x |
| flat.json | json | 1.957 | 1.965 | 3.357 | 57.508 | 0.15x |
| nested.json | strata | 0.266 | 0.269 | 0.285 | 57.590 | 1.00x |
| nested.json | orjson | 0.324 | 0.332 | 0.359 | 57.590 | 0.81x |
| nested.json | msgspec | 0.463 | 0.477 | 0.501 | 57.590 | 0.56x |
| nested.json | ujson | 1.247 | 1.273 | 1.289 | 57.590 | 0.21x |
| nested.json | json | 2.484 | 2.524 | 3.494 | 57.590 | 0.11x |
| wide_arrays.json | strata | 1.917 | 1.940 | 2.306 | 59.031 | 1.00x |
| wide_arrays.json | orjson | 2.504 | 2.559 | 3.352 | 59.031 | 0.76x |
| wide_arrays.json | msgspec | 3.804 | 3.973 | 4.448 | 59.031 | 0.49x |
| wide_arrays.json | ujson | 7.594 | 7.712 | 8.568 | 59.031 | 0.25x |
| wide_arrays.json | json | 18.457 | 18.586 | 20.804 | 59.031 | 0.10x |
| mixed.json | strata | 0.068 | 0.069 | 0.091 | 57.473 | 1.00x |
| mixed.json | orjson | 0.068 | 0.069 | 0.089 | 57.473 | 1.00x |
| mixed.json | msgspec | 0.092 | 0.093 | 0.118 | 57.473 | 0.75x |
| mixed.json | ujson | 0.259 | 0.264 | 0.388 | 57.473 | 0.26x |
| mixed.json | json | 0.510 | 0.518 | 0.578 | 57.473 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.567 | 11.096 | 12.434 | 59.156 | 1.00x |
| users.json | orjson | 14.663 | 15.102 | 15.585 | 59.156 | 0.73x |
| users.json | msgspec | 14.045 | 14.671 | 14.994 | 59.156 | 0.76x |
| users.json | ujson | 26.137 | 27.202 | 30.318 | 59.156 | 0.41x |
| users.json | json | 23.934 | 24.400 | 26.356 | 59.156 | 0.45x |
| flat.json | strata | 0.956 | 1.021 | 1.085 | 57.434 | 1.00x |
| flat.json | orjson | 1.258 | 1.318 | 1.548 | 57.434 | 0.78x |
| flat.json | msgspec | 1.293 | 1.341 | 1.403 | 57.434 | 0.76x |
| flat.json | ujson | 2.661 | 2.749 | 2.851 | 57.434 | 0.37x |
| flat.json | json | 2.152 | 2.161 | 2.180 | 57.434 | 0.47x |
| nested.json | strata | 0.823 | 0.861 | 1.299 | 57.336 | 1.00x |
| nested.json | orjson | 1.161 | 1.221 | 1.814 | 57.336 | 0.71x |
| nested.json | msgspec | 1.129 | 1.192 | 1.736 | 57.336 | 0.72x |
| nested.json | ujson | 2.002 | 2.064 | 2.479 | 57.336 | 0.42x |
| nested.json | json | 2.259 | 2.318 | 2.896 | 57.336 | 0.37x |
| wide_arrays.json | strata | 4.618 | 4.725 | 5.308 | 59.031 | 1.00x |
| wide_arrays.json | orjson | 5.990 | 6.104 | 6.549 | 59.031 | 0.77x |
| wide_arrays.json | msgspec | 6.179 | 6.270 | 7.263 | 59.031 | 0.75x |
| wide_arrays.json | ujson | 11.135 | 11.411 | 14.713 | 59.031 | 0.41x |
| wide_arrays.json | json | 12.195 | 12.500 | 13.072 | 59.031 | 0.38x |
| mixed.json | strata | 0.248 | 0.268 | 0.367 | 57.473 | 1.00x |
| mixed.json | orjson | 0.328 | 0.342 | 0.485 | 57.473 | 0.78x |
| mixed.json | msgspec | 0.346 | 0.370 | 0.569 | 57.473 | 0.72x |
| mixed.json | ujson | 0.534 | 0.569 | 0.858 | 57.473 | 0.47x |
| mixed.json | json | 0.591 | 0.631 | 1.156 | 57.473 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.288 | 10.939 | 15.676 | 58.430 | 1.00x |
| users.ndjson | orjson | 16.874 | 17.443 | 18.240 | 58.430 | 0.63x |
| users.ndjson | msgspec | 17.300 | 18.181 | 24.783 | 58.430 | 0.60x |
| users.ndjson | ujson | 25.846 | 26.510 | 29.143 | 58.430 | 0.41x |
| users.ndjson | json | 30.347 | 31.416 | 33.113 | 58.430 | 0.35x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.770 | 3.856 | 12.180 | 59.211 | 1.00x |
| users.json | orjson | 4.446 | 4.638 | 5.005 | 59.211 | 0.83x |
| users.json | msgspec | 5.781 | 6.060 | 6.486 | 59.211 | 0.64x |
| users.json | ujson | 22.974 | 23.285 | 29.490 | 59.211 | 0.17x |
| users.json | json | 32.396 | 32.777 | 33.709 | 59.211 | 0.12x |
| flat.json | strata | 0.609 | 0.647 | 0.699 | 57.801 | 1.00x |
| flat.json | orjson | 0.716 | 0.764 | 0.851 | 57.801 | 0.85x |
| flat.json | msgspec | 0.849 | 0.876 | 0.982 | 57.801 | 0.74x |
| flat.json | ujson | 2.812 | 2.856 | 3.181 | 57.801 | 0.23x |
| flat.json | json | 3.309 | 3.359 | 3.453 | 57.801 | 0.19x |
| nested.json | strata | 0.580 | 0.610 | 0.739 | 57.562 | 1.00x |
| nested.json | orjson | 0.678 | 0.726 | 0.855 | 57.562 | 0.84x |
| nested.json | msgspec | 0.815 | 0.846 | 0.951 | 57.562 | 0.72x |
| nested.json | ujson | 2.313 | 2.357 | 2.423 | 57.562 | 0.26x |
| nested.json | json | 3.520 | 3.594 | 4.383 | 57.562 | 0.17x |
| wide_arrays.json | strata | 2.598 | 2.690 | 3.382 | 59.023 | 1.00x |
| wide_arrays.json | orjson | 3.207 | 3.315 | 4.197 | 59.023 | 0.81x |
| wide_arrays.json | msgspec | 4.612 | 4.680 | 4.942 | 59.023 | 0.57x |
| wide_arrays.json | ujson | 14.440 | 14.547 | 15.062 | 59.023 | 0.18x |
| wide_arrays.json | json | 25.181 | 25.392 | 26.195 | 59.023 | 0.11x |
| mixed.json | strata | 0.346 | 0.361 | 0.420 | 57.473 | 1.00x |
| mixed.json | orjson | 0.378 | 0.395 | 0.456 | 57.473 | 0.91x |
| mixed.json | msgspec | 0.409 | 0.430 | 0.469 | 57.473 | 0.84x |
| mixed.json | ujson | 0.736 | 0.758 | 0.808 | 57.473 | 0.48x |
| mixed.json | json | 0.986 | 1.022 | 1.096 | 57.473 | 0.35x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.086 | 0.089 | 0.095 | 59.266 | 1.00x |
| users.json $[*].id | jmespath | 0.436 | 0.446 | 0.499 | 59.266 | 0.20x |
| users.json $[*].id | jsonpath-ng | 2.491 | 2.609 | 2.779 | 59.266 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.461 | 0.501 | 0.530 | 59.285 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.740 | 2.770 | 2.881 | 59.285 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.458 | 17.963 | 20.562 | 59.285 | 0.03x |
| users.json $..total | strata | 1.924 | 1.994 | 2.940 | 59.285 | 1.00x |
| users.json $..total | jsonpath-ng | 334.468 | 336.454 | 343.597 | 59.285 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.000 | 4.042 | 4.158 | 59.285 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.204 | 15.603 | 16.607 | 59.285 | 0.26x |
| users.json $[*].id | orjson+jsonpath-ng | 16.977 | 17.651 | 24.594 | 59.285 | 0.23x |
| users.json $[*].orders[*].total | strata | 4.222 | 4.287 | 4.663 | 59.285 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.190 | 18.407 | 20.177 | 59.285 | 0.23x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 35.467 | 36.606 | 38.603 | 59.285 | 0.12x |
| users.json $..total | strata | 13.483 | 14.760 | 18.756 | 59.285 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 357.316 | 362.763 | 370.985 | 59.285 | 0.04x |

