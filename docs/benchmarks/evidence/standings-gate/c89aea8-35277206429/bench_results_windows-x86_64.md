# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c89aea890fd23de6c299c86e4099ab38c8418300
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
| users.json | strata | 9.073 | 9.752 | 12.959 | 48.875 | 1.00x |
| users.json | orjson | 13.360 | 13.660 | 15.922 | 48.875 | 0.71x |
| users.json | msgspec | 12.657 | 13.101 | 14.967 | 48.875 | 0.74x |
| users.json | ujson | 20.227 | 22.038 | 24.669 | 48.875 | 0.44x |
| users.json | json | 22.380 | 23.312 | 25.002 | 48.875 | 0.42x |
| flat.json | strata | 0.822 | 0.855 | 0.908 | 57.656 | 1.00x |
| flat.json | orjson | 1.082 | 1.114 | 1.144 | 57.656 | 0.77x |
| flat.json | msgspec | 1.078 | 1.119 | 1.183 | 57.656 | 0.76x |
| flat.json | ujson | 2.091 | 2.124 | 2.267 | 57.656 | 0.40x |
| flat.json | json | 1.962 | 1.970 | 1.981 | 57.656 | 0.43x |
| nested.json | strata | 0.736 | 0.751 | 0.778 | 57.105 | 1.00x |
| nested.json | orjson | 1.058 | 1.092 | 1.105 | 57.105 | 0.69x |
| nested.json | msgspec | 0.991 | 1.017 | 1.044 | 57.105 | 0.74x |
| nested.json | ujson | 1.510 | 1.554 | 1.628 | 57.105 | 0.48x |
| nested.json | json | 2.125 | 2.133 | 2.525 | 57.105 | 0.35x |
| wide_arrays.json | strata | 4.159 | 4.296 | 4.495 | 59.121 | 1.00x |
| wide_arrays.json | orjson | 5.650 | 5.730 | 6.276 | 59.121 | 0.75x |
| wide_arrays.json | msgspec | 5.755 | 5.815 | 8.347 | 59.121 | 0.74x |
| wide_arrays.json | ujson | 8.168 | 8.313 | 10.452 | 59.121 | 0.52x |
| wide_arrays.json | json | 11.606 | 11.667 | 13.258 | 59.121 | 0.37x |
| mixed.json | strata | 0.180 | 0.183 | 0.221 | 57.777 | 1.00x |
| mixed.json | orjson | 0.212 | 0.213 | 0.250 | 57.777 | 0.86x |
| mixed.json | msgspec | 0.232 | 0.234 | 0.329 | 57.777 | 0.78x |
| mixed.json | ujson | 0.344 | 0.351 | 0.387 | 57.777 | 0.52x |
| mixed.json | json | 0.478 | 0.486 | 0.528 | 57.777 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.948 | 3.021 | 3.228 | 48.129 | 1.00x |
| users.json | orjson | 3.580 | 3.704 | 3.771 | 48.129 | 0.82x |
| users.json | msgspec | 4.842 | 5.165 | 5.464 | 48.129 | 0.58x |
| users.json | ujson | 14.029 | 14.264 | 14.494 | 48.129 | 0.21x |
| users.json | json | 23.189 | 23.741 | 24.992 | 48.129 | 0.13x |
| flat.json | strata | 0.291 | 0.297 | 0.341 | 57.617 | 1.00x |
| flat.json | orjson | 0.357 | 0.359 | 0.403 | 57.617 | 0.83x |
| flat.json | msgspec | 0.489 | 0.492 | 0.529 | 57.617 | 0.60x |
| flat.json | ujson | 1.443 | 1.459 | 1.554 | 57.617 | 0.20x |
| flat.json | json | 1.949 | 1.963 | 1.994 | 57.617 | 0.15x |
| nested.json | strata | 0.269 | 0.273 | 0.310 | 57.559 | 1.00x |
| nested.json | orjson | 0.324 | 0.329 | 0.423 | 57.559 | 0.83x |
| nested.json | msgspec | 0.476 | 0.483 | 0.572 | 57.559 | 0.56x |
| nested.json | ujson | 1.205 | 1.237 | 1.261 | 57.559 | 0.22x |
| nested.json | json | 2.430 | 2.436 | 2.472 | 57.559 | 0.11x |
| wide_arrays.json | strata | 2.011 | 2.043 | 2.072 | 58.805 | 1.00x |
| wide_arrays.json | orjson | 2.499 | 2.541 | 2.603 | 58.805 | 0.80x |
| wide_arrays.json | msgspec | 3.806 | 3.922 | 4.144 | 58.805 | 0.52x |
| wide_arrays.json | ujson | 7.633 | 7.722 | 7.889 | 58.805 | 0.26x |
| wide_arrays.json | json | 20.041 | 20.609 | 21.067 | 58.805 | 0.10x |
| mixed.json | strata | 0.069 | 0.070 | 0.071 | 57.793 | 1.00x |
| mixed.json | orjson | 0.068 | 0.069 | 0.083 | 57.793 | 1.01x |
| mixed.json | msgspec | 0.093 | 0.095 | 0.114 | 57.793 | 0.74x |
| mixed.json | ujson | 0.263 | 0.265 | 0.286 | 57.793 | 0.26x |
| mixed.json | json | 0.499 | 0.515 | 0.548 | 57.793 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.622 | 10.989 | 16.630 | 58.156 | 1.00x |
| users.json | orjson | 14.190 | 14.881 | 15.203 | 58.156 | 0.74x |
| users.json | msgspec | 13.709 | 14.325 | 14.934 | 58.156 | 0.77x |
| users.json | ujson | 26.114 | 27.199 | 28.169 | 58.156 | 0.40x |
| users.json | json | 23.595 | 24.118 | 30.244 | 58.156 | 0.46x |
| flat.json | strata | 0.988 | 1.138 | 1.292 | 57.781 | 1.00x |
| flat.json | orjson | 1.414 | 1.482 | 1.666 | 57.781 | 0.77x |
| flat.json | msgspec | 1.267 | 1.305 | 1.419 | 57.781 | 0.87x |
| flat.json | ujson | 2.724 | 2.822 | 3.169 | 57.781 | 0.40x |
| flat.json | json | 2.082 | 2.114 | 2.460 | 57.781 | 0.54x |
| nested.json | strata | 0.824 | 0.833 | 0.878 | 57.168 | 1.00x |
| nested.json | orjson | 1.177 | 1.240 | 1.290 | 57.168 | 0.67x |
| nested.json | msgspec | 1.119 | 1.179 | 1.248 | 57.168 | 0.71x |
| nested.json | ujson | 1.963 | 1.983 | 2.028 | 57.168 | 0.42x |
| nested.json | json | 2.256 | 2.270 | 2.943 | 57.168 | 0.37x |
| wide_arrays.json | strata | 4.627 | 4.714 | 5.248 | 58.805 | 1.00x |
| wide_arrays.json | orjson | 6.055 | 6.248 | 6.667 | 58.805 | 0.75x |
| wide_arrays.json | msgspec | 6.298 | 6.406 | 6.772 | 58.805 | 0.74x |
| wide_arrays.json | ujson | 11.298 | 11.500 | 11.776 | 58.805 | 0.41x |
| wide_arrays.json | json | 12.091 | 12.269 | 12.776 | 58.805 | 0.38x |
| mixed.json | strata | 0.252 | 0.255 | 0.293 | 57.793 | 1.00x |
| mixed.json | orjson | 0.328 | 0.345 | 0.435 | 57.793 | 0.74x |
| mixed.json | msgspec | 0.347 | 0.353 | 0.574 | 57.793 | 0.72x |
| mixed.json | ujson | 0.536 | 0.541 | 0.610 | 57.793 | 0.47x |
| mixed.json | json | 0.595 | 0.614 | 0.709 | 57.793 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.276 | 10.848 | 11.800 | 58.363 | 1.00x |
| users.ndjson | orjson | 17.269 | 17.656 | 19.842 | 58.363 | 0.61x |
| users.ndjson | msgspec | 17.519 | 18.197 | 19.304 | 58.363 | 0.60x |
| users.ndjson | ujson | 25.671 | 26.403 | 27.859 | 58.363 | 0.41x |
| users.ndjson | json | 29.891 | 30.367 | 32.207 | 58.363 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.855 | 4.022 | 4.211 | 58.809 | 1.00x |
| users.json | orjson | 4.577 | 4.794 | 5.159 | 58.809 | 0.84x |
| users.json | msgspec | 6.121 | 6.299 | 6.498 | 58.809 | 0.64x |
| users.json | ujson | 23.288 | 23.820 | 24.364 | 58.809 | 0.17x |
| users.json | json | 32.463 | 33.315 | 33.949 | 58.809 | 0.12x |
| flat.json | strata | 0.609 | 0.622 | 0.660 | 58.332 | 1.00x |
| flat.json | orjson | 0.702 | 0.741 | 0.775 | 58.332 | 0.84x |
| flat.json | msgspec | 0.846 | 0.866 | 0.954 | 58.332 | 0.72x |
| flat.json | ujson | 2.795 | 2.822 | 2.904 | 58.332 | 0.22x |
| flat.json | json | 3.335 | 3.354 | 3.935 | 58.332 | 0.19x |
| nested.json | strata | 0.572 | 0.593 | 0.699 | 57.547 | 1.00x |
| nested.json | orjson | 0.658 | 0.687 | 0.833 | 57.547 | 0.86x |
| nested.json | msgspec | 0.817 | 0.847 | 0.891 | 57.547 | 0.70x |
| nested.json | ujson | 2.287 | 2.296 | 2.433 | 57.547 | 0.26x |
| nested.json | json | 3.465 | 3.528 | 4.019 | 57.547 | 0.17x |
| wide_arrays.json | strata | 2.598 | 2.668 | 4.234 | 58.953 | 1.00x |
| wide_arrays.json | orjson | 3.139 | 3.193 | 3.273 | 58.953 | 0.84x |
| wide_arrays.json | msgspec | 4.749 | 4.848 | 5.968 | 58.953 | 0.55x |
| wide_arrays.json | ujson | 14.401 | 14.525 | 17.900 | 58.953 | 0.18x |
| wide_arrays.json | json | 26.449 | 26.964 | 36.272 | 58.953 | 0.10x |
| mixed.json | strata | 0.348 | 0.355 | 0.406 | 57.793 | 1.00x |
| mixed.json | orjson | 0.383 | 0.418 | 0.451 | 57.793 | 0.85x |
| mixed.json | msgspec | 0.411 | 0.435 | 0.462 | 57.793 | 0.82x |
| mixed.json | ujson | 0.743 | 0.754 | 0.832 | 57.793 | 0.47x |
| mixed.json | json | 0.992 | 1.023 | 1.302 | 57.793 | 0.35x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.093 | 0.103 | 0.135 | 58.859 | 1.00x |
| users.json $[*].id | jmespath | 0.446 | 0.459 | 0.490 | 58.859 | 0.23x |
| users.json $[*].id | jsonpath-ng | 2.633 | 2.779 | 2.869 | 58.859 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.474 | 0.513 | 0.617 | 58.879 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.773 | 2.833 | 2.895 | 58.879 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.807 | 18.518 | 22.798 | 58.879 | 0.03x |
| users.json $..total | strata | 1.897 | 1.952 | 2.169 | 58.883 | 1.00x |
| users.json $..total | jsonpath-ng | 334.277 | 340.900 | 354.435 | 58.883 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.066 | 4.106 | 4.187 | 58.879 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.687 | 16.605 | 18.493 | 58.879 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 17.611 | 18.762 | 21.019 | 58.879 | 0.22x |
| users.json $[*].orders[*].total | strata | 4.241 | 4.298 | 4.853 | 58.883 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.263 | 18.660 | 19.054 | 58.883 | 0.23x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 35.610 | 36.288 | 42.593 | 58.883 | 0.12x |
| users.json $..total | strata | 13.901 | 14.574 | 15.188 | 58.883 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 347.966 | 351.483 | 375.028 | 58.883 | 0.04x |

