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
| users.json | strata | 9.167 | 9.405 | 12.533 | 49.039 | 1.00x |
| users.json | orjson | 12.964 | 13.420 | 15.875 | 49.039 | 0.70x |
| users.json | msgspec | 12.580 | 12.804 | 18.418 | 49.039 | 0.73x |
| users.json | ujson | 20.300 | 20.993 | 29.807 | 49.039 | 0.45x |
| users.json | json | 22.339 | 22.794 | 24.228 | 49.039 | 0.41x |
| flat.json | strata | 0.786 | 0.814 | 0.830 | 57.297 | 1.00x |
| flat.json | orjson | 1.091 | 1.111 | 1.124 | 57.297 | 0.73x |
| flat.json | msgspec | 1.065 | 1.082 | 1.103 | 57.297 | 0.75x |
| flat.json | ujson | 2.042 | 2.061 | 2.095 | 57.297 | 0.39x |
| flat.json | json | 1.936 | 1.940 | 1.966 | 57.297 | 0.42x |
| nested.json | strata | 0.762 | 0.781 | 0.808 | 56.688 | 1.00x |
| nested.json | orjson | 1.082 | 1.102 | 1.151 | 56.688 | 0.71x |
| nested.json | msgspec | 1.029 | 1.050 | 1.098 | 56.688 | 0.74x |
| nested.json | ujson | 1.548 | 1.591 | 1.645 | 56.688 | 0.49x |
| nested.json | json | 2.167 | 2.186 | 2.205 | 56.688 | 0.36x |
| wide_arrays.json | strata | 4.140 | 4.183 | 4.289 | 58.852 | 1.00x |
| wide_arrays.json | orjson | 5.583 | 5.655 | 5.775 | 58.852 | 0.74x |
| wide_arrays.json | msgspec | 5.663 | 5.739 | 5.835 | 58.852 | 0.73x |
| wide_arrays.json | ujson | 8.081 | 8.203 | 9.862 | 58.852 | 0.51x |
| wide_arrays.json | json | 11.523 | 11.646 | 11.971 | 58.852 | 0.36x |
| mixed.json | strata | 0.185 | 0.202 | 0.297 | 57.594 | 1.00x |
| mixed.json | orjson | 0.215 | 0.219 | 0.350 | 57.594 | 0.92x |
| mixed.json | msgspec | 0.235 | 0.239 | 0.300 | 57.594 | 0.85x |
| mixed.json | ujson | 0.351 | 0.375 | 0.619 | 57.594 | 0.54x |
| mixed.json | json | 0.477 | 0.515 | 0.832 | 57.594 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.926 | 2.984 | 3.054 | 47.242 | 1.00x |
| users.json | orjson | 3.845 | 3.923 | 5.787 | 47.242 | 0.76x |
| users.json | msgspec | 5.176 | 5.367 | 5.507 | 47.242 | 0.56x |
| users.json | ujson | 14.076 | 14.481 | 14.907 | 47.242 | 0.21x |
| users.json | json | 23.640 | 24.019 | 24.294 | 47.242 | 0.12x |
| flat.json | strata | 0.292 | 0.296 | 0.321 | 57.340 | 1.00x |
| flat.json | orjson | 0.356 | 0.362 | 0.381 | 57.340 | 0.82x |
| flat.json | msgspec | 0.493 | 0.507 | 0.524 | 57.340 | 0.58x |
| flat.json | ujson | 1.438 | 1.458 | 1.479 | 57.340 | 0.20x |
| flat.json | json | 1.957 | 1.990 | 2.028 | 57.340 | 0.15x |
| nested.json | strata | 0.269 | 0.281 | 0.390 | 56.977 | 1.00x |
| nested.json | orjson | 0.321 | 0.326 | 0.355 | 56.977 | 0.86x |
| nested.json | msgspec | 0.466 | 0.484 | 0.499 | 56.977 | 0.58x |
| nested.json | ujson | 1.261 | 1.280 | 1.404 | 56.977 | 0.22x |
| nested.json | json | 2.466 | 2.515 | 2.689 | 56.977 | 0.11x |
| wide_arrays.json | strata | 1.949 | 1.966 | 2.072 | 58.734 | 1.00x |
| wide_arrays.json | orjson | 2.517 | 2.579 | 2.696 | 58.734 | 0.76x |
| wide_arrays.json | msgspec | 3.988 | 4.109 | 4.294 | 58.734 | 0.48x |
| wide_arrays.json | ujson | 7.832 | 7.887 | 8.084 | 58.734 | 0.25x |
| wide_arrays.json | json | 18.625 | 18.681 | 25.239 | 58.734 | 0.11x |
| mixed.json | strata | 0.068 | 0.070 | 0.100 | 57.727 | 1.00x |
| mixed.json | orjson | 0.070 | 0.071 | 0.132 | 57.727 | 0.99x |
| mixed.json | msgspec | 0.093 | 0.095 | 0.126 | 57.727 | 0.74x |
| mixed.json | ujson | 0.263 | 0.264 | 0.293 | 57.727 | 0.27x |
| mixed.json | json | 0.511 | 0.543 | 0.570 | 57.727 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.255 | 10.723 | 12.372 | 57.461 | 1.00x |
| users.json | orjson | 13.977 | 14.464 | 16.076 | 57.461 | 0.74x |
| users.json | msgspec | 13.788 | 14.608 | 15.376 | 57.461 | 0.73x |
| users.json | ujson | 25.427 | 26.313 | 30.766 | 57.461 | 0.41x |
| users.json | json | 23.513 | 24.024 | 41.223 | 57.461 | 0.45x |
| flat.json | strata | 1.025 | 1.067 | 1.548 | 57.113 | 1.00x |
| flat.json | orjson | 1.250 | 1.298 | 1.815 | 57.113 | 0.82x |
| flat.json | msgspec | 1.221 | 1.258 | 2.198 | 57.113 | 0.85x |
| flat.json | ujson | 2.622 | 2.719 | 3.683 | 57.113 | 0.39x |
| flat.json | json | 2.071 | 2.087 | 2.147 | 57.113 | 0.51x |
| nested.json | strata | 0.884 | 0.886 | 0.931 | 56.699 | 1.00x |
| nested.json | orjson | 1.211 | 1.219 | 1.345 | 56.699 | 0.73x |
| nested.json | msgspec | 1.157 | 1.162 | 1.192 | 56.699 | 0.76x |
| nested.json | ujson | 1.974 | 1.983 | 2.020 | 56.699 | 0.45x |
| nested.json | json | 2.288 | 2.310 | 2.389 | 56.699 | 0.38x |
| wide_arrays.json | strata | 4.580 | 4.737 | 5.251 | 58.734 | 1.00x |
| wide_arrays.json | orjson | 6.060 | 6.197 | 7.559 | 58.734 | 0.76x |
| wide_arrays.json | msgspec | 6.144 | 6.281 | 8.454 | 58.734 | 0.75x |
| wide_arrays.json | ujson | 11.169 | 11.270 | 12.201 | 58.734 | 0.42x |
| wide_arrays.json | json | 12.000 | 12.288 | 15.741 | 58.734 | 0.39x |
| mixed.json | strata | 0.252 | 0.270 | 0.296 | 57.730 | 1.00x |
| mixed.json | orjson | 0.325 | 0.346 | 0.381 | 57.730 | 0.78x |
| mixed.json | msgspec | 0.352 | 0.362 | 0.396 | 57.730 | 0.75x |
| mixed.json | ujson | 0.536 | 0.556 | 0.619 | 57.730 | 0.49x |
| mixed.json | json | 0.597 | 0.633 | 0.647 | 57.730 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.389 | 10.525 | 10.704 | 57.914 | 1.00x |
| users.ndjson | orjson | 16.893 | 17.234 | 18.401 | 57.914 | 0.61x |
| users.ndjson | msgspec | 16.987 | 17.085 | 19.177 | 57.914 | 0.62x |
| users.ndjson | ujson | 24.933 | 25.216 | 28.026 | 57.914 | 0.42x |
| users.ndjson | json | 29.685 | 30.280 | 31.506 | 57.914 | 0.35x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.779 | 3.832 | 4.242 | 59.715 | 1.00x |
| users.json | orjson | 4.539 | 4.790 | 4.860 | 59.715 | 0.80x |
| users.json | msgspec | 5.881 | 6.021 | 6.276 | 59.715 | 0.64x |
| users.json | ujson | 23.320 | 23.517 | 23.554 | 59.715 | 0.16x |
| users.json | json | 32.781 | 33.058 | 34.002 | 59.715 | 0.12x |
| flat.json | strata | 0.602 | 0.631 | 0.647 | 57.203 | 1.00x |
| flat.json | orjson | 0.698 | 0.737 | 0.764 | 57.203 | 0.86x |
| flat.json | msgspec | 0.861 | 0.875 | 0.978 | 57.203 | 0.72x |
| flat.json | ujson | 2.804 | 2.839 | 4.508 | 57.203 | 0.22x |
| flat.json | json | 3.348 | 3.365 | 3.673 | 57.203 | 0.19x |
| nested.json | strata | 0.569 | 0.601 | 0.629 | 57.270 | 1.00x |
| nested.json | orjson | 0.669 | 0.693 | 0.730 | 57.270 | 0.87x |
| nested.json | msgspec | 0.839 | 0.862 | 0.893 | 57.270 | 0.70x |
| nested.json | ujson | 2.328 | 2.357 | 2.400 | 57.270 | 0.25x |
| nested.json | json | 3.557 | 3.576 | 3.612 | 57.270 | 0.17x |
| wide_arrays.json | strata | 2.570 | 2.625 | 2.685 | 58.734 | 1.00x |
| wide_arrays.json | orjson | 3.193 | 3.301 | 3.459 | 58.734 | 0.80x |
| wide_arrays.json | msgspec | 4.781 | 4.827 | 4.920 | 58.734 | 0.54x |
| wide_arrays.json | ujson | 14.443 | 14.730 | 15.803 | 58.734 | 0.18x |
| wide_arrays.json | json | 25.201 | 25.432 | 26.546 | 58.734 | 0.10x |
| mixed.json | strata | 0.346 | 0.365 | 0.398 | 57.734 | 1.00x |
| mixed.json | orjson | 0.376 | 0.404 | 0.434 | 57.734 | 0.90x |
| mixed.json | msgspec | 0.403 | 0.425 | 0.460 | 57.734 | 0.86x |
| mixed.json | ujson | 0.777 | 0.785 | 0.838 | 57.734 | 0.46x |
| mixed.json | json | 1.025 | 1.042 | 1.064 | 57.734 | 0.35x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.081 | 0.087 | 0.165 | 57.688 | 1.00x |
| users.json $[*].id | jmespath | 0.433 | 0.437 | 0.820 | 57.688 | 0.20x |
| users.json $[*].id | jsonpath-ng | 2.485 | 2.541 | 3.698 | 57.688 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.460 | 0.483 | 0.671 | 57.980 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.774 | 2.825 | 4.633 | 57.980 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 16.774 | 17.264 | 17.714 | 57.980 | 0.03x |
| users.json $..total | strata | 1.879 | 1.936 | 1.993 | 57.891 | 1.00x |
| users.json $..total | jsonpath-ng | 328.261 | 333.993 | 343.232 | 57.891 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.952 | 4.012 | 4.113 | 57.707 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.913 | 15.293 | 15.504 | 57.707 | 0.26x |
| users.json $[*].id | orjson+jsonpath-ng | 16.877 | 17.144 | 27.580 | 57.707 | 0.23x |
| users.json $[*].orders[*].total | strata | 4.186 | 4.246 | 4.324 | 57.895 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.741 | 18.133 | 18.375 | 57.895 | 0.23x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 34.409 | 35.625 | 36.481 | 57.895 | 0.12x |
| users.json $..total | strata | 13.070 | 15.093 | 24.180 | 57.965 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 346.321 | 368.243 | 588.104 | 57.965 | 0.04x |

