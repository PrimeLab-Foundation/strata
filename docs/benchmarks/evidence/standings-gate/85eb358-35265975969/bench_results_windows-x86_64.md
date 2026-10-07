# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 85eb3583e98587844415f5d32f85a61cbedfcf49
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
| users.json | strata | 10.808 | 11.592 | 15.899 | 48.992 | 1.00x |
| users.json | orjson | 15.341 | 16.078 | 17.849 | 48.992 | 0.72x |
| users.json | msgspec | 13.552 | 14.298 | 16.364 | 48.992 | 0.81x |
| users.json | ujson | 22.688 | 23.406 | 26.897 | 48.992 | 0.50x |
| users.json | json | 23.481 | 24.467 | 24.941 | 48.992 | 0.47x |
| flat.json | strata | 1.169 | 1.210 | 1.808 | 57.168 | 1.00x |
| flat.json | orjson | 1.279 | 1.350 | 1.989 | 57.168 | 0.90x |
| flat.json | msgspec | 1.223 | 1.282 | 1.874 | 57.168 | 0.94x |
| flat.json | ujson | 2.289 | 2.347 | 3.511 | 57.168 | 0.52x |
| flat.json | json | 2.054 | 2.113 | 3.249 | 57.168 | 0.57x |
| nested.json | strata | 0.781 | 0.843 | 0.979 | 57.055 | 1.00x |
| nested.json | orjson | 1.150 | 1.197 | 1.218 | 57.055 | 0.70x |
| nested.json | msgspec | 0.992 | 1.049 | 1.102 | 57.055 | 0.80x |
| nested.json | ujson | 1.627 | 1.654 | 1.690 | 57.055 | 0.51x |
| nested.json | json | 2.121 | 2.144 | 2.261 | 57.055 | 0.39x |
| wide_arrays.json | strata | 5.799 | 6.449 | 7.125 | 59.070 | 1.00x |
| wide_arrays.json | orjson | 7.569 | 8.370 | 10.879 | 59.070 | 0.77x |
| wide_arrays.json | msgspec | 6.947 | 7.819 | 12.961 | 59.070 | 0.82x |
| wide_arrays.json | ujson | 10.054 | 10.670 | 15.845 | 59.070 | 0.60x |
| wide_arrays.json | json | 13.361 | 15.940 | 21.852 | 59.070 | 0.40x |
| mixed.json | strata | 0.203 | 0.230 | 0.326 | 56.957 | 1.00x |
| mixed.json | orjson | 0.238 | 0.302 | 0.431 | 56.957 | 0.76x |
| mixed.json | msgspec | 0.249 | 0.266 | 0.480 | 56.957 | 0.86x |
| mixed.json | ujson | 0.374 | 0.416 | 0.695 | 56.957 | 0.55x |
| mixed.json | json | 0.487 | 0.523 | 0.899 | 56.957 | 0.44x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.139 | 3.269 | 3.481 | 49.059 | 1.00x |
| users.json | orjson | 3.729 | 3.978 | 4.166 | 49.059 | 0.82x |
| users.json | msgspec | 5.501 | 5.759 | 6.718 | 49.059 | 0.57x |
| users.json | ujson | 13.109 | 13.559 | 15.372 | 49.059 | 0.24x |
| users.json | json | 23.342 | 23.703 | 24.323 | 49.059 | 0.14x |
| flat.json | strata | 0.365 | 0.377 | 0.422 | 57.996 | 1.00x |
| flat.json | orjson | 0.397 | 0.416 | 0.481 | 57.996 | 0.91x |
| flat.json | msgspec | 0.575 | 0.607 | 0.630 | 57.996 | 0.62x |
| flat.json | ujson | 1.447 | 1.485 | 1.560 | 57.996 | 0.25x |
| flat.json | json | 2.010 | 2.072 | 2.178 | 57.996 | 0.18x |
| nested.json | strata | 0.255 | 0.361 | 0.481 | 57.703 | 1.00x |
| nested.json | orjson | 0.340 | 0.347 | 0.561 | 57.703 | 1.04x |
| nested.json | msgspec | 0.536 | 0.557 | 0.821 | 57.703 | 0.65x |
| nested.json | ujson | 1.037 | 1.402 | 1.801 | 57.703 | 0.26x |
| nested.json | json | 2.537 | 3.813 | 4.326 | 57.703 | 0.09x |
| wide_arrays.json | strata | 2.413 | 2.590 | 3.029 | 58.684 | 1.00x |
| wide_arrays.json | orjson | 2.889 | 3.023 | 3.601 | 58.684 | 0.86x |
| wide_arrays.json | msgspec | 4.676 | 5.106 | 8.315 | 58.684 | 0.51x |
| wide_arrays.json | ujson | 8.625 | 9.265 | 14.624 | 58.684 | 0.28x |
| wide_arrays.json | json | 19.777 | 20.800 | 25.403 | 58.684 | 0.12x |
| mixed.json | strata | 0.072 | 0.078 | 0.083 | 57.137 | 1.00x |
| mixed.json | orjson | 0.076 | 0.080 | 0.096 | 57.137 | 0.97x |
| mixed.json | msgspec | 0.110 | 0.117 | 0.168 | 57.137 | 0.67x |
| mixed.json | ujson | 0.260 | 0.267 | 0.303 | 57.137 | 0.29x |
| mixed.json | json | 0.539 | 0.543 | 0.624 | 57.137 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.738 | 13.128 | 18.601 | 58.078 | 1.00x |
| users.json | orjson | 16.012 | 16.982 | 21.812 | 58.078 | 0.77x |
| users.json | msgspec | 14.943 | 15.630 | 16.451 | 58.078 | 0.84x |
| users.json | ujson | 26.308 | 27.316 | 31.050 | 58.078 | 0.48x |
| users.json | json | 24.139 | 25.478 | 26.373 | 58.078 | 0.52x |
| flat.json | strata | 1.226 | 1.387 | 2.087 | 57.793 | 1.00x |
| flat.json | orjson | 1.652 | 1.715 | 2.472 | 57.793 | 0.81x |
| flat.json | msgspec | 1.397 | 1.536 | 2.319 | 57.793 | 0.90x |
| flat.json | ujson | 2.570 | 2.657 | 3.946 | 57.793 | 0.52x |
| flat.json | json | 2.181 | 2.231 | 2.415 | 57.793 | 0.62x |
| nested.json | strata | 0.947 | 1.068 | 1.183 | 57.105 | 1.00x |
| nested.json | orjson | 1.403 | 1.485 | 2.193 | 57.105 | 0.72x |
| nested.json | msgspec | 1.233 | 1.367 | 1.628 | 57.105 | 0.78x |
| nested.json | ujson | 2.072 | 2.205 | 3.293 | 57.105 | 0.48x |
| nested.json | json | 2.366 | 2.483 | 4.390 | 57.105 | 0.43x |
| wide_arrays.json | strata | 5.480 | 5.724 | 8.495 | 58.684 | 1.00x |
| wide_arrays.json | orjson | 7.058 | 7.215 | 9.647 | 58.684 | 0.79x |
| wide_arrays.json | msgspec | 6.925 | 7.234 | 10.302 | 58.684 | 0.79x |
| wide_arrays.json | ujson | 10.947 | 11.856 | 15.406 | 58.684 | 0.48x |
| wide_arrays.json | json | 12.524 | 12.893 | 20.532 | 58.684 | 0.44x |
| mixed.json | strata | 0.282 | 0.301 | 0.471 | 57.203 | 1.00x |
| mixed.json | orjson | 0.369 | 0.405 | 0.547 | 57.203 | 0.74x |
| mixed.json | msgspec | 0.388 | 0.403 | 0.594 | 57.203 | 0.75x |
| mixed.json | ujson | 0.551 | 0.582 | 0.847 | 57.203 | 0.52x |
| mixed.json | json | 0.592 | 0.637 | 0.962 | 57.203 | 0.47x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 14.659 | 15.043 | 21.206 | 58.277 | 1.00x |
| users.ndjson | orjson | 21.908 | 22.393 | 24.492 | 58.277 | 0.67x |
| users.ndjson | msgspec | 21.342 | 22.396 | 29.278 | 58.277 | 0.67x |
| users.ndjson | ujson | 29.795 | 31.287 | 32.518 | 58.277 | 0.48x |
| users.ndjson | json | 34.936 | 38.733 | 46.066 | 58.277 | 0.39x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.131 | 4.328 | 4.534 | 59.215 | 1.00x |
| users.json | orjson | 5.015 | 5.220 | 5.534 | 59.215 | 0.83x |
| users.json | msgspec | 6.735 | 7.013 | 7.306 | 59.215 | 0.62x |
| users.json | ujson | 22.371 | 22.707 | 23.481 | 59.215 | 0.19x |
| users.json | json | 32.746 | 33.128 | 36.636 | 59.215 | 0.13x |
| flat.json | strata | 0.757 | 0.833 | 1.139 | 57.918 | 1.00x |
| flat.json | orjson | 0.821 | 0.958 | 1.337 | 57.918 | 0.87x |
| flat.json | msgspec | 1.011 | 1.106 | 1.513 | 57.918 | 0.75x |
| flat.json | ujson | 2.944 | 2.981 | 3.162 | 57.918 | 0.28x |
| flat.json | json | 3.506 | 3.600 | 4.582 | 57.918 | 0.23x |
| nested.json | strata | 0.686 | 0.729 | 0.977 | 57.668 | 1.00x |
| nested.json | orjson | 0.778 | 0.899 | 1.134 | 57.668 | 0.81x |
| nested.json | msgspec | 0.960 | 1.082 | 1.442 | 57.668 | 0.67x |
| nested.json | ujson | 2.248 | 2.329 | 10.141 | 57.668 | 0.31x |
| nested.json | json | 3.733 | 3.835 | 6.310 | 57.668 | 0.19x |
| wide_arrays.json | strata | 2.996 | 3.371 | 4.788 | 58.684 | 1.00x |
| wide_arrays.json | orjson | 3.590 | 4.037 | 6.613 | 58.684 | 0.84x |
| wide_arrays.json | msgspec | 5.506 | 6.035 | 9.584 | 58.684 | 0.56x |
| wide_arrays.json | ujson | 15.368 | 15.948 | 17.400 | 58.684 | 0.21x |
| wide_arrays.json | json | 26.668 | 29.567 | 43.522 | 58.684 | 0.11x |
| mixed.json | strata | 0.385 | 0.406 | 0.532 | 57.207 | 1.00x |
| mixed.json | orjson | 0.417 | 0.448 | 0.577 | 57.207 | 0.91x |
| mixed.json | msgspec | 0.448 | 0.487 | 0.658 | 57.207 | 0.83x |
| mixed.json | ujson | 0.767 | 0.794 | 1.218 | 57.207 | 0.51x |
| mixed.json | json | 1.058 | 1.109 | 1.627 | 57.207 | 0.37x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.110 | 0.116 | 0.131 | 59.266 | 1.00x |
| users.json $[*].id | jmespath | 0.449 | 0.460 | 0.509 | 59.266 | 0.25x |
| users.json $[*].id | jsonpath-ng | 2.580 | 2.885 | 2.940 | 59.266 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.547 | 0.563 | 1.136 | 59.293 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.779 | 2.803 | 3.247 | 59.293 | 0.20x |
| users.json $[*].orders[*].total | jsonpath-ng | 18.070 | 19.533 | 21.002 | 59.293 | 0.03x |
| users.json $..total | strata | 2.016 | 2.081 | 2.479 | 59.293 | 1.00x |
| users.json $..total | jsonpath-ng | 322.280 | 324.776 | 335.704 | 59.293 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.432 | 4.481 | 5.864 | 59.293 | 1.00x |
| users.json $[*].id | orjson+jmespath | 17.572 | 18.276 | 19.144 | 59.293 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 19.535 | 20.287 | 20.905 | 59.293 | 0.22x |
| users.json $[*].orders[*].total | strata | 4.671 | 4.703 | 4.847 | 59.293 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 21.769 | 22.795 | 26.728 | 59.293 | 0.21x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 43.779 | 44.169 | 52.547 | 59.293 | 0.11x |
| users.json $..total | strata | 19.766 | 21.850 | 24.374 | 59.293 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 355.100 | 374.963 | 425.302 | 59.293 | 0.06x |

