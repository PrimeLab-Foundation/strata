# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: baba505678994faef24df367a2dc4b93d5d4bef0
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 26 Model 2 Stepping 1, AuthenticAMD
- compiler_flags: /nologo /O2 /W3 /DNDEBUG /MD /EHsc /std:c++20 /Zc:__cplusplus /D_USE_STD_VECTOR_ALGORITHMS=0 /arch:AVX2 /clang:-O3 /clang:-fprofile-use=D:\a\strata\strata\build\pgo\strata.profdata /DLL /MANIFEST:EMBED,ID=2 /MANIFESTUAC:NO /EXPORT:PyInit__strata (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 5.300 | 5.864 | 8.775 | 49.000 | 1.00x |
| users.json | orjson | 8.615 | 9.092 | 11.997 | 49.000 | 0.64x |
| users.json | msgspec | 7.234 | 7.457 | 9.433 | 49.000 | 0.79x |
| users.json | ujson | 11.154 | 12.739 | 14.428 | 49.000 | 0.46x |
| users.json | json | 12.604 | 13.418 | 13.882 | 49.000 | 0.44x |
| flat.json | strata | 0.544 | 0.835 | 1.643 | 57.672 | 1.00x |
| flat.json | orjson | 0.801 | 1.737 | 2.403 | 57.672 | 0.48x |
| flat.json | msgspec | 0.666 | 0.963 | 1.413 | 57.672 | 0.87x |
| flat.json | ujson | 0.968 | 1.362 | 2.615 | 57.672 | 0.61x |
| flat.json | json | 1.110 | 1.403 | 2.635 | 57.672 | 0.60x |
| nested.json | strata | 0.407 | 0.409 | 0.430 | 57.145 | 1.00x |
| nested.json | orjson | 0.633 | 0.646 | 0.682 | 57.145 | 0.63x |
| nested.json | msgspec | 0.507 | 0.514 | 0.562 | 57.145 | 0.80x |
| nested.json | ujson | 0.811 | 0.840 | 0.949 | 57.145 | 0.49x |
| nested.json | json | 1.106 | 1.124 | 1.143 | 57.145 | 0.36x |
| wide_arrays.json | strata | 2.416 | 2.529 | 3.164 | 59.656 | 1.00x |
| wide_arrays.json | orjson | 3.442 | 3.609 | 5.363 | 59.656 | 0.70x |
| wide_arrays.json | msgspec | 3.496 | 3.581 | 4.060 | 59.656 | 0.71x |
| wide_arrays.json | ujson | 4.724 | 4.930 | 5.750 | 59.656 | 0.51x |
| wide_arrays.json | json | 6.366 | 6.620 | 7.878 | 59.656 | 0.38x |
| mixed.json | strata | 0.104 | 0.106 | 0.127 | 57.039 | 1.00x |
| mixed.json | orjson | 0.120 | 0.125 | 0.136 | 57.039 | 0.85x |
| mixed.json | msgspec | 0.126 | 0.133 | 0.140 | 57.039 | 0.80x |
| mixed.json | ujson | 0.185 | 0.204 | 0.221 | 57.039 | 0.52x |
| mixed.json | json | 0.259 | 0.265 | 0.296 | 57.039 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.832 | 1.939 | 2.220 | 47.277 | 1.00x |
| users.json | orjson | 2.046 | 2.180 | 2.260 | 47.277 | 0.89x |
| users.json | msgspec | 3.417 | 3.532 | 3.897 | 47.277 | 0.55x |
| users.json | ujson | 7.284 | 7.459 | 8.204 | 47.277 | 0.26x |
| users.json | json | 13.274 | 13.761 | 16.801 | 47.277 | 0.14x |
| flat.json | strata | 0.206 | 0.211 | 0.221 | 57.523 | 1.00x |
| flat.json | orjson | 0.197 | 0.199 | 0.204 | 57.523 | 1.06x |
| flat.json | msgspec | 0.335 | 0.341 | 0.358 | 57.523 | 0.62x |
| flat.json | ujson | 0.634 | 0.660 | 0.674 | 57.523 | 0.32x |
| flat.json | json | 1.093 | 1.114 | 1.141 | 57.523 | 0.19x |
| nested.json | strata | 0.133 | 0.135 | 0.251 | 57.281 | 1.00x |
| nested.json | orjson | 0.170 | 0.171 | 0.182 | 57.281 | 0.78x |
| nested.json | msgspec | 0.287 | 0.291 | 0.325 | 57.281 | 0.46x |
| nested.json | ujson | 0.555 | 0.565 | 0.588 | 57.281 | 0.24x |
| nested.json | json | 1.356 | 1.390 | 1.419 | 57.281 | 0.10x |
| wide_arrays.json | strata | 1.318 | 1.349 | 1.477 | 58.684 | 1.00x |
| wide_arrays.json | orjson | 1.605 | 1.689 | 1.808 | 58.684 | 0.80x |
| wide_arrays.json | msgspec | 2.878 | 2.964 | 3.655 | 58.684 | 0.46x |
| wide_arrays.json | ujson | 4.703 | 4.770 | 5.436 | 58.684 | 0.28x |
| wide_arrays.json | json | 10.810 | 10.938 | 13.400 | 58.684 | 0.12x |
| mixed.json | strata | 0.039 | 0.042 | 0.063 | 57.191 | 1.00x |
| mixed.json | orjson | 0.035 | 0.037 | 0.042 | 57.191 | 1.13x |
| mixed.json | msgspec | 0.055 | 0.065 | 0.097 | 57.191 | 0.65x |
| mixed.json | ujson | 0.125 | 0.131 | 0.147 | 57.191 | 0.32x |
| mixed.json | json | 0.282 | 0.298 | 0.318 | 57.191 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.776 | 7.110 | 8.359 | 57.500 | 1.00x |
| users.json | orjson | 9.894 | 10.269 | 10.833 | 57.500 | 0.69x |
| users.json | msgspec | 8.350 | 9.008 | 11.153 | 57.500 | 0.79x |
| users.json | ujson | 14.678 | 15.303 | 17.579 | 57.500 | 0.46x |
| users.json | json | 13.681 | 14.258 | 14.879 | 57.500 | 0.50x |
| flat.json | strata | 0.729 | 0.764 | 0.822 | 57.309 | 1.00x |
| flat.json | orjson | 0.882 | 0.935 | 1.047 | 57.309 | 0.82x |
| flat.json | msgspec | 0.740 | 0.770 | 0.877 | 57.309 | 0.99x |
| flat.json | ujson | 1.218 | 1.323 | 1.588 | 57.309 | 0.58x |
| flat.json | json | 1.167 | 1.200 | 1.303 | 57.309 | 0.64x |
| nested.json | strata | 0.441 | 0.462 | 0.648 | 57.156 | 1.00x |
| nested.json | orjson | 0.678 | 0.735 | 1.026 | 57.156 | 0.63x |
| nested.json | msgspec | 0.566 | 0.605 | 0.820 | 57.156 | 0.76x |
| nested.json | ujson | 0.954 | 1.075 | 1.443 | 57.156 | 0.43x |
| nested.json | json | 1.155 | 1.195 | 1.675 | 57.156 | 0.39x |
| wide_arrays.json | strata | 2.849 | 3.078 | 3.364 | 58.684 | 1.00x |
| wide_arrays.json | orjson | 3.924 | 4.399 | 4.638 | 58.684 | 0.70x |
| wide_arrays.json | msgspec | 3.992 | 4.315 | 4.481 | 58.684 | 0.71x |
| wide_arrays.json | ujson | 6.299 | 6.753 | 7.396 | 58.684 | 0.46x |
| wide_arrays.json | json | 6.813 | 7.257 | 7.823 | 58.684 | 0.42x |
| mixed.json | strata | 0.135 | 0.140 | 0.165 | 57.191 | 1.00x |
| mixed.json | orjson | 0.172 | 0.180 | 0.216 | 57.191 | 0.78x |
| mixed.json | msgspec | 0.171 | 0.181 | 0.190 | 57.191 | 0.77x |
| mixed.json | ujson | 0.257 | 0.287 | 0.308 | 57.191 | 0.49x |
| mixed.json | json | 0.306 | 0.329 | 0.344 | 57.191 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 5.840 | 6.318 | 9.841 | 58.031 | 1.00x |
| users.ndjson | orjson | 10.739 | 11.127 | 13.595 | 58.031 | 0.57x |
| users.ndjson | msgspec | 10.372 | 11.606 | 14.466 | 58.031 | 0.54x |
| users.ndjson | ujson | 15.590 | 17.230 | 19.198 | 58.031 | 0.37x |
| users.ndjson | json | 17.357 | 19.285 | 22.974 | 58.031 | 0.33x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.587 | 2.699 | 2.828 | 58.777 | 1.00x |
| users.json | orjson | 2.833 | 2.913 | 3.300 | 58.777 | 0.93x |
| users.json | msgspec | 4.158 | 4.264 | 4.580 | 58.777 | 0.63x |
| users.json | ujson | 11.861 | 12.286 | 13.065 | 58.777 | 0.22x |
| users.json | json | 17.845 | 18.201 | 22.466 | 58.777 | 0.15x |
| flat.json | strata | 0.424 | 0.450 | 0.518 | 57.641 | 1.00x |
| flat.json | orjson | 0.427 | 0.451 | 0.513 | 57.641 | 1.00x |
| flat.json | msgspec | 0.562 | 0.600 | 0.614 | 57.641 | 0.75x |
| flat.json | ujson | 1.368 | 1.391 | 1.407 | 57.641 | 0.32x |
| flat.json | json | 1.808 | 1.864 | 1.911 | 57.641 | 0.24x |
| nested.json | strata | 0.351 | 0.380 | 0.499 | 57.266 | 1.00x |
| nested.json | orjson | 0.404 | 0.432 | 0.491 | 57.266 | 0.88x |
| nested.json | msgspec | 0.517 | 0.551 | 0.589 | 57.266 | 0.69x |
| nested.json | ujson | 1.113 | 1.161 | 1.243 | 57.266 | 0.33x |
| nested.json | json | 1.952 | 1.985 | 2.082 | 57.266 | 0.19x |
| wide_arrays.json | strata | 1.781 | 1.991 | 2.195 | 58.684 | 1.00x |
| wide_arrays.json | orjson | 2.050 | 2.224 | 2.372 | 58.684 | 0.90x |
| wide_arrays.json | msgspec | 3.317 | 3.469 | 3.649 | 58.684 | 0.57x |
| wide_arrays.json | ujson | 8.013 | 8.254 | 8.828 | 58.684 | 0.24x |
| wide_arrays.json | json | 13.965 | 14.389 | 14.859 | 58.684 | 0.14x |
| mixed.json | strata | 0.208 | 0.223 | 0.446 | 57.191 | 1.00x |
| mixed.json | orjson | 0.216 | 0.221 | 0.380 | 57.191 | 1.01x |
| mixed.json | msgspec | 0.229 | 0.260 | 0.991 | 57.191 | 0.86x |
| mixed.json | ujson | 0.384 | 0.395 | 0.567 | 57.191 | 0.56x |
| mixed.json | json | 0.554 | 0.598 | 1.604 | 57.191 | 0.37x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.051 | 0.057 | 0.074 | 58.836 | 1.00x |
| users.json $[*].id | jmespath | 0.218 | 0.226 | 0.246 | 58.836 | 0.25x |
| users.json $[*].id | jsonpath-ng | 1.263 | 1.378 | 1.556 | 58.836 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.285 | 0.292 | 0.302 | 58.855 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.377 | 1.399 | 1.458 | 58.855 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 8.821 | 9.391 | 10.826 | 58.855 | 0.03x |
| users.json $..total | strata | 1.062 | 1.082 | 1.106 | 58.855 | 1.00x |
| users.json $..total | jsonpath-ng | 186.603 | 191.990 | 195.094 | 58.855 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 2.815 | 2.837 | 2.882 | 58.855 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.664 | 11.044 | 11.624 | 58.855 | 0.26x |
| users.json $[*].id | orjson+jsonpath-ng | 11.449 | 11.960 | 12.952 | 58.855 | 0.24x |
| users.json $[*].orders[*].total | strata | 2.913 | 2.963 | 3.101 | 58.855 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.484 | 13.047 | 13.590 | 58.855 | 0.23x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 21.950 | 22.665 | 23.802 | 58.855 | 0.13x |
| users.json $..total | strata | 9.887 | 11.470 | 12.187 | 58.855 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 203.280 | 209.306 | 215.650 | 58.855 | 0.05x |

