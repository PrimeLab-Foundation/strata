# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 38cf28416465e176915ffd24d827a95015eacdbd
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 25 Model 17 Stepping 1, AuthenticAMD
- compiler_flags: /nologo /O2 /W3 /DNDEBUG /MD /EHsc /std:c++20 /Zc:__cplusplus /D_USE_STD_VECTOR_ALGORITHMS=0 /arch:AVX2 /clang:-O3 /clang:-fprofile-use=D:\a\strata\strata\build\pgo\strata.profdata /DLL /MANIFEST:EMBED,ID=2 /MANIFESTUAC:NO /EXPORT:PyInit__strata (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.885 | 10.190 | 14.582 | 48.949 | 1.00x |
| users.json | orjson | 14.363 | 14.712 | 17.240 | 48.949 | 0.69x |
| users.json | msgspec | 13.171 | 13.428 | 16.540 | 48.949 | 0.76x |
| users.json | ujson | 20.754 | 21.272 | 26.229 | 48.949 | 0.48x |
| users.json | json | 22.457 | 23.141 | 26.240 | 48.949 | 0.44x |
| flat.json | strata | 1.121 | 1.165 | 1.231 | 56.930 | 1.00x |
| flat.json | orjson | 1.223 | 1.280 | 1.377 | 56.930 | 0.91x |
| flat.json | msgspec | 1.121 | 1.167 | 1.190 | 56.930 | 1.00x |
| flat.json | ujson | 1.918 | 2.008 | 2.112 | 56.930 | 0.58x |
| flat.json | json | 1.969 | 1.991 | 2.023 | 56.930 | 0.58x |
| nested.json | strata | 0.783 | 0.820 | 1.263 | 56.859 | 1.00x |
| nested.json | orjson | 1.092 | 1.134 | 1.745 | 56.859 | 0.72x |
| nested.json | msgspec | 0.985 | 1.006 | 1.647 | 56.859 | 0.82x |
| nested.json | ujson | 1.526 | 1.579 | 1.968 | 56.859 | 0.52x |
| nested.json | json | 2.110 | 2.134 | 2.339 | 56.859 | 0.38x |
| wide_arrays.json | strata | 4.433 | 4.552 | 6.931 | 59.809 | 1.00x |
| wide_arrays.json | orjson | 5.950 | 6.018 | 6.897 | 59.809 | 0.76x |
| wide_arrays.json | msgspec | 5.902 | 5.990 | 8.177 | 59.809 | 0.76x |
| wide_arrays.json | ujson | 8.176 | 8.373 | 9.424 | 59.809 | 0.54x |
| wide_arrays.json | json | 11.369 | 11.641 | 15.282 | 59.809 | 0.39x |
| mixed.json | strata | 0.190 | 0.193 | 0.233 | 56.863 | 1.00x |
| mixed.json | orjson | 0.215 | 0.222 | 0.237 | 56.863 | 0.87x |
| mixed.json | msgspec | 0.235 | 0.243 | 0.284 | 56.863 | 0.79x |
| mixed.json | ujson | 0.327 | 0.340 | 0.354 | 56.863 | 0.57x |
| mixed.json | json | 0.472 | 0.476 | 0.518 | 56.863 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.006 | 3.067 | 3.190 | 48.164 | 1.00x |
| users.json | orjson | 3.807 | 4.026 | 4.221 | 48.164 | 0.76x |
| users.json | msgspec | 5.523 | 5.702 | 5.876 | 48.164 | 0.54x |
| users.json | ujson | 13.075 | 13.159 | 13.424 | 48.164 | 0.23x |
| users.json | json | 23.136 | 23.382 | 23.943 | 48.164 | 0.13x |
| flat.json | strata | 0.331 | 0.353 | 0.401 | 57.547 | 1.00x |
| flat.json | orjson | 0.387 | 0.403 | 0.432 | 57.547 | 0.88x |
| flat.json | msgspec | 0.566 | 0.582 | 0.634 | 57.547 | 0.61x |
| flat.json | ujson | 1.459 | 1.490 | 1.537 | 57.547 | 0.24x |
| flat.json | json | 1.993 | 2.030 | 2.047 | 57.547 | 0.17x |
| nested.json | strata | 0.243 | 0.249 | 0.304 | 57.461 | 1.00x |
| nested.json | orjson | 0.322 | 0.326 | 0.382 | 57.461 | 0.76x |
| nested.json | msgspec | 0.503 | 0.521 | 0.548 | 57.461 | 0.48x |
| nested.json | ujson | 1.006 | 1.045 | 1.079 | 57.461 | 0.24x |
| nested.json | json | 2.452 | 2.486 | 2.554 | 57.461 | 0.10x |
| wide_arrays.json | strata | 2.152 | 2.192 | 3.148 | 58.312 | 1.00x |
| wide_arrays.json | orjson | 2.774 | 2.848 | 3.798 | 58.312 | 0.77x |
| wide_arrays.json | msgspec | 4.495 | 4.579 | 4.732 | 58.312 | 0.48x |
| wide_arrays.json | ujson | 7.862 | 8.002 | 8.442 | 58.312 | 0.27x |
| wide_arrays.json | json | 18.729 | 18.877 | 19.309 | 58.312 | 0.12x |
| mixed.json | strata | 0.068 | 0.073 | 0.109 | 57.055 | 1.00x |
| mixed.json | orjson | 0.073 | 0.076 | 0.086 | 57.055 | 0.97x |
| mixed.json | msgspec | 0.102 | 0.112 | 0.137 | 57.055 | 0.65x |
| mixed.json | ujson | 0.257 | 0.264 | 0.292 | 57.055 | 0.28x |
| mixed.json | json | 0.531 | 0.580 | 0.612 | 57.055 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.366 | 11.852 | 15.953 | 58.191 | 1.00x |
| users.json | orjson | 15.335 | 16.074 | 16.616 | 58.191 | 0.74x |
| users.json | msgspec | 14.396 | 14.891 | 19.898 | 58.191 | 0.80x |
| users.json | ujson | 25.135 | 25.668 | 26.496 | 58.191 | 0.46x |
| users.json | json | 23.314 | 23.853 | 24.935 | 58.191 | 0.50x |
| flat.json | strata | 1.209 | 1.266 | 1.353 | 57.215 | 1.00x |
| flat.json | orjson | 1.424 | 1.463 | 1.801 | 57.215 | 0.87x |
| flat.json | msgspec | 1.255 | 1.306 | 1.371 | 57.215 | 0.97x |
| flat.json | ujson | 2.337 | 2.462 | 2.566 | 57.215 | 0.51x |
| flat.json | json | 2.129 | 2.153 | 2.185 | 57.215 | 0.59x |
| nested.json | strata | 0.875 | 0.913 | 0.976 | 57.039 | 1.00x |
| nested.json | orjson | 1.219 | 1.309 | 1.422 | 57.039 | 0.70x |
| nested.json | msgspec | 1.120 | 1.190 | 1.207 | 57.039 | 0.77x |
| nested.json | ujson | 1.912 | 2.019 | 2.129 | 57.039 | 0.45x |
| nested.json | json | 2.262 | 2.314 | 2.401 | 57.039 | 0.39x |
| wide_arrays.json | strata | 4.977 | 5.086 | 5.374 | 58.312 | 1.00x |
| wide_arrays.json | orjson | 6.331 | 6.495 | 6.678 | 58.312 | 0.78x |
| wide_arrays.json | msgspec | 6.416 | 6.523 | 6.723 | 58.312 | 0.78x |
| wide_arrays.json | ujson | 10.496 | 10.643 | 10.918 | 58.312 | 0.48x |
| wide_arrays.json | json | 11.918 | 12.060 | 12.590 | 58.312 | 0.42x |
| mixed.json | strata | 0.260 | 0.272 | 0.302 | 57.055 | 1.00x |
| mixed.json | orjson | 0.330 | 0.339 | 0.483 | 57.055 | 0.80x |
| mixed.json | msgspec | 0.344 | 0.385 | 0.438 | 57.055 | 0.71x |
| mixed.json | ujson | 0.496 | 0.507 | 0.621 | 57.055 | 0.54x |
| mixed.json | json | 0.584 | 0.614 | 0.631 | 57.055 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 11.341 | 12.218 | 13.659 | 57.922 | 1.00x |
| users.ndjson | orjson | 18.614 | 19.609 | 21.001 | 57.922 | 0.62x |
| users.ndjson | msgspec | 18.576 | 19.261 | 20.805 | 57.922 | 0.63x |
| users.ndjson | ujson | 25.737 | 27.014 | 28.407 | 57.922 | 0.45x |
| users.ndjson | json | 31.159 | 32.480 | 33.952 | 57.922 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.138 | 4.200 | 4.587 | 58.336 | 1.00x |
| users.json | orjson | 5.011 | 5.194 | 6.740 | 58.336 | 0.81x |
| users.json | msgspec | 6.555 | 6.959 | 7.360 | 58.336 | 0.60x |
| users.json | ujson | 22.189 | 22.854 | 23.814 | 58.336 | 0.18x |
| users.json | json | 32.686 | 33.566 | 38.467 | 58.336 | 0.13x |
| flat.json | strata | 0.653 | 0.749 | 0.866 | 57.504 | 1.00x |
| flat.json | orjson | 0.747 | 0.778 | 0.916 | 57.504 | 0.96x |
| flat.json | msgspec | 0.926 | 0.980 | 1.280 | 57.504 | 0.76x |
| flat.json | ujson | 2.826 | 2.919 | 3.267 | 57.504 | 0.26x |
| flat.json | json | 3.325 | 3.435 | 4.192 | 57.504 | 0.22x |
| nested.json | strata | 0.568 | 0.606 | 0.820 | 57.418 | 1.00x |
| nested.json | orjson | 0.688 | 0.750 | 0.777 | 57.418 | 0.81x |
| nested.json | msgspec | 0.878 | 0.919 | 0.990 | 57.418 | 0.66x |
| nested.json | ujson | 2.101 | 2.142 | 2.192 | 57.418 | 0.28x |
| nested.json | json | 3.549 | 3.573 | 3.731 | 57.418 | 0.17x |
| wide_arrays.json | strata | 2.807 | 2.913 | 3.115 | 58.559 | 1.00x |
| wide_arrays.json | orjson | 3.501 | 3.545 | 3.965 | 58.559 | 0.82x |
| wide_arrays.json | msgspec | 5.217 | 5.278 | 5.822 | 58.559 | 0.55x |
| wide_arrays.json | ujson | 14.597 | 14.761 | 15.102 | 58.559 | 0.20x |
| wide_arrays.json | json | 25.309 | 25.590 | 26.185 | 58.559 | 0.11x |
| mixed.json | strata | 0.340 | 0.364 | 0.417 | 57.082 | 1.00x |
| mixed.json | orjson | 0.390 | 0.406 | 0.441 | 57.082 | 0.90x |
| mixed.json | msgspec | 0.418 | 0.437 | 0.488 | 57.082 | 0.83x |
| mixed.json | ujson | 0.735 | 0.775 | 0.828 | 57.082 | 0.47x |
| mixed.json | json | 0.987 | 1.041 | 1.086 | 57.082 | 0.35x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.110 | 0.117 | 0.139 | 58.406 | 1.00x |
| users.json $[*].id | jmespath | 0.439 | 0.454 | 0.478 | 58.406 | 0.26x |
| users.json $[*].id | jsonpath-ng | 2.514 | 2.888 | 3.087 | 58.406 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.538 | 0.577 | 0.740 | 58.430 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.732 | 2.836 | 4.935 | 58.430 | 0.20x |
| users.json $[*].orders[*].total | jsonpath-ng | 18.477 | 19.226 | 20.638 | 58.430 | 0.03x |
| users.json $..total | strata | 2.027 | 2.110 | 3.509 | 58.430 | 1.00x |
| users.json $..total | jsonpath-ng | 320.815 | 323.633 | 332.729 | 58.430 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.362 | 4.397 | 4.715 | 58.430 | 1.00x |
| users.json $[*].id | orjson+jmespath | 17.031 | 18.003 | 22.517 | 58.430 | 0.24x |
| users.json $[*].id | orjson+jsonpath-ng | 18.891 | 20.079 | 21.102 | 58.430 | 0.22x |
| users.json $[*].orders[*].total | strata | 4.603 | 4.651 | 4.854 | 58.430 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 20.440 | 22.119 | 30.190 | 58.430 | 0.21x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 41.120 | 41.794 | 43.508 | 58.430 | 0.11x |
| users.json $..total | strata | 15.904 | 18.684 | 19.986 | 58.430 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 346.693 | 352.170 | 361.573 | 58.430 | 0.05x |

