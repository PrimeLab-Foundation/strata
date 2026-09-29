# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: afd1550cbabc9433e8292644444a23bb27bbc4e9
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD
- compiler_flags: /nologo /O2 /W3 /DNDEBUG /MD /EHsc /std:c++20 /Zc:__cplusplus /D_USE_STD_VECTOR_ALGORITHMS=0 /arch:AVX2 /clang:-O3 /clang:-fprofile-use=D:\a\_temp\strata-arm\build\pgo\strata.profdata /DLL /MANIFEST:EMBED,ID=2 /MANIFESTUAC:NO /EXPORT:PyInit__strata (24 recorded commands; see the JSON companion)
- repeats: 60
- warmup: 2
- provenance_schema: 1

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.846 | 9.954 | 11.613 | 48.902 | 1.00x |
| users.json | orjson | 13.050 | 14.896 | 16.476 | 48.902 | 0.67x |
| users.json | msgspec | 12.663 | 13.938 | 15.421 | 48.902 | 0.71x |
| users.json | ujson | 20.792 | 23.896 | 26.258 | 48.902 | 0.42x |
| users.json | json | 22.470 | 24.318 | 26.358 | 48.902 | 0.41x |
| flat.json | strata | 0.827 | 0.888 | 0.987 | 57.195 | 1.00x |
| flat.json | orjson | 1.102 | 1.175 | 1.342 | 57.195 | 0.76x |
| flat.json | msgspec | 1.107 | 1.180 | 1.271 | 57.195 | 0.75x |
| flat.json | ujson | 2.155 | 2.359 | 2.536 | 57.195 | 0.38x |
| flat.json | json | 1.932 | 1.981 | 2.190 | 57.195 | 0.45x |
| nested.json | strata | 0.770 | 0.856 | 1.019 | 56.547 | 1.00x |
| nested.json | orjson | 1.099 | 1.209 | 1.323 | 56.547 | 0.71x |
| nested.json | msgspec | 1.012 | 1.097 | 1.331 | 56.547 | 0.78x |
| nested.json | ujson | 1.686 | 1.836 | 2.023 | 56.547 | 0.47x |
| nested.json | json | 2.148 | 2.301 | 2.585 | 56.547 | 0.37x |
| wide_arrays.json | strata | 4.175 | 4.369 | 6.041 | 58.281 | 1.00x |
| wide_arrays.json | orjson | 5.741 | 6.137 | 7.610 | 58.281 | 0.71x |
| wide_arrays.json | msgspec | 5.772 | 6.058 | 6.998 | 58.281 | 0.72x |
| wide_arrays.json | ujson | 8.315 | 8.840 | 11.200 | 58.281 | 0.49x |
| wide_arrays.json | json | 11.660 | 12.321 | 13.636 | 58.281 | 0.35x |
| mixed.json | strata | 0.180 | 0.183 | 0.211 | 55.598 | 1.00x |
| mixed.json | orjson | 0.212 | 0.217 | 0.261 | 55.598 | 0.85x |
| mixed.json | msgspec | 0.233 | 0.238 | 0.294 | 55.598 | 0.77x |
| mixed.json | ujson | 0.351 | 0.359 | 0.413 | 55.598 | 0.51x |
| mixed.json | json | 0.466 | 0.478 | 0.537 | 55.598 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.945 | 3.114 | 3.316 | 49.199 | 1.00x |
| users.json | orjson | 3.550 | 3.748 | 3.943 | 49.199 | 0.83x |
| users.json | msgspec | 4.911 | 5.112 | 5.334 | 49.199 | 0.61x |
| users.json | ujson | 13.853 | 14.115 | 14.735 | 49.199 | 0.22x |
| users.json | json | 22.989 | 23.307 | 24.169 | 49.199 | 0.13x |
| flat.json | strata | 0.323 | 0.396 | 0.466 | 57.164 | 1.00x |
| flat.json | orjson | 0.373 | 0.433 | 0.535 | 57.164 | 0.91x |
| flat.json | msgspec | 0.512 | 0.588 | 0.683 | 57.164 | 0.67x |
| flat.json | ujson | 1.453 | 1.575 | 1.656 | 57.164 | 0.25x |
| flat.json | json | 1.976 | 2.119 | 2.222 | 57.164 | 0.19x |
| nested.json | strata | 0.276 | 0.302 | 0.358 | 57.086 | 1.00x |
| nested.json | orjson | 0.327 | 0.350 | 0.393 | 57.086 | 0.86x |
| nested.json | msgspec | 0.477 | 0.507 | 0.599 | 57.086 | 0.59x |
| nested.json | ujson | 1.228 | 1.283 | 1.334 | 57.086 | 0.23x |
| nested.json | json | 2.440 | 2.497 | 2.630 | 57.086 | 0.12x |
| wide_arrays.json | strata | 1.934 | 2.025 | 2.360 | 57.523 | 1.00x |
| wide_arrays.json | orjson | 2.625 | 2.722 | 2.963 | 57.523 | 0.74x |
| wide_arrays.json | msgspec | 4.092 | 4.221 | 4.878 | 57.523 | 0.48x |
| wide_arrays.json | ujson | 7.823 | 8.024 | 8.568 | 57.523 | 0.25x |
| wide_arrays.json | json | 18.747 | 19.355 | 20.059 | 57.523 | 0.10x |
| mixed.json | strata | 0.065 | 0.071 | 0.098 | 55.859 | 1.00x |
| mixed.json | orjson | 0.068 | 0.071 | 0.087 | 55.859 | 1.01x |
| mixed.json | msgspec | 0.092 | 0.098 | 0.136 | 55.859 | 0.73x |
| mixed.json | ujson | 0.261 | 0.269 | 0.370 | 55.859 | 0.26x |
| mixed.json | json | 0.503 | 0.520 | 0.596 | 55.859 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.374 | 11.149 | 13.157 | 59.223 | 1.00x |
| users.json | orjson | 14.101 | 15.620 | 16.647 | 59.223 | 0.71x |
| users.json | msgspec | 13.767 | 14.986 | 16.723 | 59.223 | 0.74x |
| users.json | ujson | 25.927 | 27.858 | 30.696 | 59.223 | 0.40x |
| users.json | json | 23.381 | 24.751 | 26.710 | 59.223 | 0.45x |
| flat.json | strata | 0.980 | 1.177 | 1.319 | 56.879 | 1.00x |
| flat.json | orjson | 1.278 | 1.502 | 1.673 | 56.879 | 0.78x |
| flat.json | msgspec | 1.360 | 1.493 | 1.665 | 56.879 | 0.79x |
| flat.json | ujson | 3.017 | 3.268 | 3.539 | 56.879 | 0.36x |
| flat.json | json | 2.177 | 2.458 | 2.654 | 56.879 | 0.48x |
| nested.json | strata | 0.845 | 0.905 | 1.196 | 57.074 | 1.00x |
| nested.json | orjson | 1.193 | 1.278 | 1.672 | 57.074 | 0.71x |
| nested.json | msgspec | 1.125 | 1.207 | 1.475 | 57.074 | 0.75x |
| nested.json | ujson | 1.977 | 2.057 | 2.311 | 57.074 | 0.44x |
| nested.json | json | 2.243 | 2.324 | 2.673 | 57.074 | 0.39x |
| wide_arrays.json | strata | 4.688 | 4.872 | 5.202 | 57.047 | 1.00x |
| wide_arrays.json | orjson | 6.228 | 6.469 | 7.260 | 57.047 | 0.75x |
| wide_arrays.json | msgspec | 6.265 | 6.516 | 7.037 | 57.047 | 0.75x |
| wide_arrays.json | ujson | 11.415 | 11.795 | 12.563 | 57.047 | 0.41x |
| wide_arrays.json | json | 12.110 | 12.466 | 13.126 | 57.047 | 0.39x |
| mixed.json | strata | 0.255 | 0.274 | 0.322 | 55.852 | 1.00x |
| mixed.json | orjson | 0.325 | 0.351 | 0.399 | 55.852 | 0.78x |
| mixed.json | msgspec | 0.347 | 0.369 | 0.421 | 55.852 | 0.74x |
| mixed.json | ujson | 0.532 | 0.573 | 0.638 | 55.852 | 0.48x |
| mixed.json | json | 0.584 | 0.609 | 0.685 | 55.852 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.968 | 14.115 | 16.486 | 58.516 | 1.00x |
| users.ndjson | orjson | 17.697 | 21.374 | 22.985 | 58.516 | 0.66x |
| users.ndjson | msgspec | 17.728 | 21.866 | 23.402 | 58.516 | 0.65x |
| users.ndjson | ujson | 26.233 | 31.211 | 33.390 | 58.516 | 0.45x |
| users.ndjson | json | 31.087 | 34.773 | 39.413 | 58.516 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.756 | 3.951 | 4.684 | 58.281 | 1.00x |
| users.json | orjson | 4.456 | 4.712 | 5.181 | 58.281 | 0.84x |
| users.json | msgspec | 5.739 | 6.140 | 6.739 | 58.281 | 0.64x |
| users.json | ujson | 23.105 | 23.477 | 27.293 | 58.281 | 0.17x |
| users.json | json | 32.076 | 32.555 | 34.051 | 58.281 | 0.12x |
| flat.json | strata | 0.788 | 1.008 | 1.149 | 57.395 | 1.00x |
| flat.json | orjson | 0.838 | 1.062 | 1.205 | 57.395 | 0.95x |
| flat.json | msgspec | 1.008 | 1.253 | 1.444 | 57.395 | 0.80x |
| flat.json | ujson | 2.954 | 3.250 | 3.513 | 57.395 | 0.31x |
| flat.json | json | 3.581 | 3.887 | 4.119 | 57.395 | 0.26x |
| nested.json | strata | 0.600 | 0.696 | 0.807 | 57.051 | 1.00x |
| nested.json | orjson | 0.684 | 0.765 | 0.885 | 57.051 | 0.91x |
| nested.json | msgspec | 0.824 | 0.922 | 1.000 | 57.051 | 0.75x |
| nested.json | ujson | 2.347 | 2.455 | 2.569 | 57.051 | 0.28x |
| nested.json | json | 3.511 | 3.649 | 3.898 | 57.051 | 0.19x |
| wide_arrays.json | strata | 2.610 | 2.754 | 3.284 | 57.238 | 1.00x |
| wide_arrays.json | orjson | 3.295 | 3.415 | 3.969 | 57.238 | 0.81x |
| wide_arrays.json | msgspec | 4.735 | 4.891 | 6.216 | 57.238 | 0.56x |
| wide_arrays.json | ujson | 14.560 | 14.843 | 16.400 | 57.238 | 0.19x |
| wide_arrays.json | json | 25.346 | 26.022 | 27.277 | 57.238 | 0.11x |
| mixed.json | strata | 0.347 | 0.369 | 0.460 | 55.848 | 1.00x |
| mixed.json | orjson | 0.382 | 0.401 | 0.498 | 55.848 | 0.92x |
| mixed.json | msgspec | 0.410 | 0.427 | 0.555 | 55.848 | 0.86x |
| mixed.json | ujson | 0.748 | 0.789 | 1.196 | 55.848 | 0.47x |
| mixed.json | json | 0.995 | 1.056 | 1.175 | 55.848 | 0.35x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.082 | 0.086 | 0.107 | 58.312 | 1.00x |
| users.json $[*].id | jmespath | 0.436 | 0.446 | 0.486 | 58.312 | 0.19x |
| users.json $[*].id | jsonpath-ng | 2.435 | 2.551 | 2.724 | 58.312 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.475 | 0.513 | 0.597 | 58.328 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.782 | 2.928 | 3.258 | 58.328 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 18.645 | 20.540 | 23.739 | 58.328 | 0.02x |
| users.json $..total | strata | 1.944 | 2.357 | 3.159 | 58.270 | 1.00x |
| users.json $..total | jsonpath-ng | 328.949 | 335.199 | 345.103 | 58.270 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.024 | 4.101 | 4.240 | 58.324 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.343 | 15.908 | 17.942 | 58.324 | 0.26x |
| users.json $[*].id | orjson+jsonpath-ng | 17.281 | 17.805 | 21.233 | 58.324 | 0.23x |
| users.json $[*].orders[*].total | strata | 4.335 | 4.433 | 4.678 | 58.328 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 20.398 | 23.264 | 27.149 | 58.328 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 39.356 | 47.069 | 53.726 | 58.328 | 0.09x |
| users.json $..total | strata | 15.925 | 22.259 | 24.665 | 58.270 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 352.784 | 366.864 | 378.677 | 58.270 | 0.06x |

