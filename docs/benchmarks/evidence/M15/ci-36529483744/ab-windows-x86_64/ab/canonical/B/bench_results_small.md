# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: afd1550cbabc9433e8292644444a23bb27bbc4e9
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 26 Model 2 Stepping 1, AuthenticAMD
- compiler_flags: /nologo /O2 /W3 /DNDEBUG /MD /EHsc /std:c++20 /Zc:__cplusplus /D_USE_STD_VECTOR_ALGORITHMS=0 /arch:AVX2 /clang:-O3 /clang:-fprofile-use=D:\a\_temp\strata-arm\build\pgo\strata.profdata /DLL /MANIFEST:EMBED,ID=2 /MANIFESTUAC:NO /EXPORT:PyInit__strata (24 recorded commands; see the JSON companion)
- repeats: 60
- warmup: 2
- provenance_schema: 1

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 5.144 | 8.569 | 10.150 | 48.914 | 1.00x |
| users.json | orjson | 8.378 | 12.718 | 15.617 | 48.914 | 0.67x |
| users.json | msgspec | 7.218 | 11.105 | 14.926 | 48.914 | 0.77x |
| users.json | ujson | 11.419 | 19.452 | 23.226 | 48.914 | 0.44x |
| users.json | json | 12.281 | 17.017 | 19.459 | 48.914 | 0.50x |
| flat.json | strata | 0.548 | 0.631 | 0.888 | 58.016 | 1.00x |
| flat.json | orjson | 0.771 | 0.887 | 1.100 | 58.016 | 0.71x |
| flat.json | msgspec | 0.699 | 0.803 | 0.946 | 58.016 | 0.79x |
| flat.json | ujson | 1.190 | 1.596 | 1.986 | 58.016 | 0.40x |
| flat.json | json | 1.138 | 1.245 | 1.538 | 58.016 | 0.51x |
| nested.json | strata | 0.417 | 0.457 | 0.655 | 57.281 | 1.00x |
| nested.json | orjson | 0.654 | 0.724 | 1.350 | 57.281 | 0.63x |
| nested.json | msgspec | 0.510 | 0.557 | 0.880 | 57.281 | 0.82x |
| nested.json | ujson | 0.885 | 1.054 | 1.504 | 57.281 | 0.43x |
| nested.json | json | 1.123 | 1.192 | 1.496 | 57.281 | 0.38x |
| wide_arrays.json | strata | 2.454 | 3.420 | 5.927 | 59.020 | 1.00x |
| wide_arrays.json | orjson | 3.611 | 5.111 | 7.583 | 59.020 | 0.67x |
| wide_arrays.json | msgspec | 3.469 | 4.407 | 7.012 | 59.020 | 0.78x |
| wide_arrays.json | ujson | 4.660 | 5.813 | 8.865 | 59.020 | 0.59x |
| wide_arrays.json | json | 6.252 | 7.954 | 10.480 | 59.020 | 0.43x |
| mixed.json | strata | 0.098 | 0.113 | 0.343 | 56.828 | 1.00x |
| mixed.json | orjson | 0.116 | 0.150 | 0.351 | 56.828 | 0.75x |
| mixed.json | msgspec | 0.123 | 0.139 | 0.364 | 56.828 | 0.81x |
| mixed.json | ujson | 0.182 | 0.205 | 0.527 | 56.828 | 0.55x |
| mixed.json | json | 0.253 | 0.283 | 0.557 | 56.828 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.144 | 2.661 | 3.610 | 49.223 | 1.00x |
| users.json | orjson | 2.234 | 2.718 | 3.791 | 49.223 | 0.98x |
| users.json | msgspec | 3.503 | 4.318 | 5.042 | 49.223 | 0.62x |
| users.json | ujson | 8.023 | 9.219 | 10.069 | 49.223 | 0.29x |
| users.json | json | 14.180 | 15.415 | 16.069 | 49.223 | 0.17x |
| flat.json | strata | 0.214 | 0.244 | 0.363 | 57.676 | 1.00x |
| flat.json | orjson | 0.205 | 0.226 | 0.362 | 57.676 | 1.08x |
| flat.json | msgspec | 0.345 | 0.375 | 0.510 | 57.676 | 0.65x |
| flat.json | ujson | 0.661 | 0.706 | 0.825 | 57.676 | 0.35x |
| flat.json | json | 1.122 | 1.182 | 1.337 | 57.676 | 0.21x |
| nested.json | strata | 0.136 | 0.151 | 0.218 | 57.730 | 1.00x |
| nested.json | orjson | 0.177 | 0.194 | 0.286 | 57.730 | 0.78x |
| nested.json | msgspec | 0.304 | 0.331 | 0.404 | 57.730 | 0.46x |
| nested.json | ujson | 0.570 | 0.608 | 0.999 | 57.730 | 0.25x |
| nested.json | json | 1.410 | 1.475 | 1.667 | 57.730 | 0.10x |
| wide_arrays.json | strata | 1.299 | 1.716 | 2.811 | 58.781 | 1.00x |
| wide_arrays.json | orjson | 1.532 | 1.751 | 2.256 | 58.781 | 0.98x |
| wide_arrays.json | msgspec | 2.723 | 2.992 | 3.467 | 58.781 | 0.57x |
| wide_arrays.json | ujson | 4.569 | 4.888 | 5.738 | 58.781 | 0.35x |
| wide_arrays.json | json | 10.449 | 11.674 | 13.838 | 58.781 | 0.15x |
| mixed.json | strata | 0.038 | 0.040 | 0.051 | 56.902 | 1.00x |
| mixed.json | orjson | 0.035 | 0.036 | 0.053 | 56.902 | 1.11x |
| mixed.json | msgspec | 0.054 | 0.057 | 0.084 | 56.902 | 0.70x |
| mixed.json | ujson | 0.124 | 0.131 | 0.213 | 56.902 | 0.31x |
| mixed.json | json | 0.284 | 0.302 | 0.390 | 56.902 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.448 | 10.330 | 11.655 | 59.246 | 1.00x |
| users.json | orjson | 11.131 | 13.860 | 16.300 | 59.246 | 0.75x |
| users.json | msgspec | 9.582 | 12.733 | 15.795 | 59.246 | 0.81x |
| users.json | ujson | 18.334 | 22.450 | 26.535 | 59.246 | 0.46x |
| users.json | json | 15.202 | 17.945 | 21.441 | 59.246 | 0.58x |
| flat.json | strata | 0.658 | 0.805 | 1.263 | 57.582 | 1.00x |
| flat.json | orjson | 0.835 | 1.017 | 1.657 | 57.582 | 0.79x |
| flat.json | msgspec | 0.775 | 0.924 | 1.426 | 57.582 | 0.87x |
| flat.json | ujson | 1.449 | 1.922 | 2.541 | 57.582 | 0.42x |
| flat.json | json | 1.200 | 1.402 | 2.005 | 57.582 | 0.57x |
| nested.json | strata | 0.492 | 0.625 | 1.242 | 57.051 | 1.00x |
| nested.json | orjson | 0.765 | 0.895 | 1.430 | 57.051 | 0.70x |
| nested.json | msgspec | 0.634 | 0.735 | 1.174 | 57.051 | 0.85x |
| nested.json | ujson | 1.111 | 1.353 | 1.871 | 57.051 | 0.46x |
| nested.json | json | 1.218 | 1.420 | 2.300 | 57.051 | 0.44x |
| wide_arrays.json | strata | 2.755 | 4.227 | 6.581 | 58.781 | 1.00x |
| wide_arrays.json | orjson | 3.914 | 5.711 | 8.724 | 58.781 | 0.74x |
| wide_arrays.json | msgspec | 3.746 | 5.159 | 8.357 | 58.781 | 0.82x |
| wide_arrays.json | ujson | 6.079 | 7.963 | 11.831 | 58.781 | 0.53x |
| wide_arrays.json | json | 6.508 | 8.230 | 11.639 | 58.781 | 0.51x |
| mixed.json | strata | 0.135 | 0.155 | 0.525 | 56.902 | 1.00x |
| mixed.json | orjson | 0.174 | 0.191 | 0.629 | 56.902 | 0.81x |
| mixed.json | msgspec | 0.176 | 0.196 | 0.637 | 56.902 | 0.79x |
| mixed.json | ujson | 0.262 | 0.297 | 0.864 | 56.902 | 0.52x |
| mixed.json | json | 0.309 | 0.334 | 0.853 | 56.902 | 0.46x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.540 | 8.230 | 11.509 | 58.492 | 1.00x |
| users.ndjson | orjson | 11.494 | 13.749 | 17.251 | 58.492 | 0.60x |
| users.ndjson | msgspec | 11.110 | 12.887 | 17.176 | 58.492 | 0.64x |
| users.ndjson | ujson | 15.487 | 18.282 | 23.571 | 58.492 | 0.45x |
| users.ndjson | json | 17.914 | 21.092 | 25.156 | 58.492 | 0.39x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.178 | 3.766 | 4.885 | 59.301 | 1.00x |
| users.json | orjson | 3.348 | 4.040 | 5.326 | 59.301 | 0.93x |
| users.json | msgspec | 4.994 | 5.846 | 6.959 | 59.301 | 0.64x |
| users.json | ujson | 13.750 | 14.822 | 18.259 | 59.301 | 0.25x |
| users.json | json | 20.203 | 21.370 | 23.430 | 59.301 | 0.18x |
| flat.json | strata | 0.468 | 0.521 | 0.817 | 58.055 | 1.00x |
| flat.json | orjson | 0.461 | 0.515 | 0.754 | 58.055 | 1.01x |
| flat.json | msgspec | 0.606 | 0.662 | 0.910 | 58.055 | 0.79x |
| flat.json | ujson | 1.415 | 1.494 | 1.845 | 58.055 | 0.35x |
| flat.json | json | 1.902 | 1.978 | 2.384 | 58.055 | 0.26x |
| nested.json | strata | 0.361 | 0.437 | 0.705 | 57.594 | 1.00x |
| nested.json | orjson | 0.420 | 0.501 | 0.758 | 57.594 | 0.87x |
| nested.json | msgspec | 0.541 | 0.637 | 0.945 | 57.594 | 0.69x |
| nested.json | ujson | 1.177 | 1.292 | 1.662 | 57.594 | 0.34x |
| nested.json | json | 1.963 | 2.141 | 3.136 | 57.594 | 0.20x |
| wide_arrays.json | strata | 1.850 | 2.183 | 5.410 | 58.738 | 1.00x |
| wide_arrays.json | orjson | 2.151 | 2.289 | 5.528 | 58.738 | 0.95x |
| wide_arrays.json | msgspec | 3.159 | 3.461 | 7.390 | 58.738 | 0.63x |
| wide_arrays.json | ujson | 7.972 | 8.431 | 13.484 | 58.738 | 0.26x |
| wide_arrays.json | json | 13.847 | 14.783 | 19.574 | 58.738 | 0.15x |
| mixed.json | strata | 0.211 | 0.225 | 0.287 | 56.930 | 1.00x |
| mixed.json | orjson | 0.214 | 0.234 | 0.357 | 56.930 | 0.96x |
| mixed.json | msgspec | 0.238 | 0.255 | 0.359 | 56.930 | 0.88x |
| mixed.json | ujson | 0.387 | 0.416 | 0.511 | 56.930 | 0.54x |
| mixed.json | json | 0.557 | 0.600 | 0.696 | 56.930 | 0.38x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.074 | 0.088 | 0.108 | 59.336 | 1.00x |
| users.json $[*].id | jmespath | 0.251 | 0.285 | 0.370 | 59.336 | 0.31x |
| users.json $[*].id | jsonpath-ng | 1.645 | 2.043 | 2.232 | 59.336 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.327 | 0.391 | 1.085 | 59.352 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.478 | 1.641 | 2.563 | 59.352 | 0.24x |
| users.json $[*].orders[*].total | jsonpath-ng | 12.170 | 14.479 | 16.985 | 59.352 | 0.03x |
| users.json $..total | strata | 1.067 | 1.123 | 1.443 | 59.352 | 1.00x |
| users.json $..total | jsonpath-ng | 183.811 | 189.814 | 199.009 | 59.352 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 2.994 | 3.100 | 3.303 | 59.348 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.135 | 16.032 | 20.315 | 59.348 | 0.19x |
| users.json $[*].id | orjson+jsonpath-ng | 14.661 | 17.873 | 21.202 | 59.348 | 0.17x |
| users.json $[*].orders[*].total | strata | 3.227 | 3.354 | 3.564 | 59.352 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.865 | 20.159 | 25.130 | 59.352 | 0.17x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 31.081 | 36.930 | 44.527 | 59.352 | 0.09x |
| users.json $..total | strata | 11.976 | 15.959 | 18.121 | 59.289 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 209.043 | 219.218 | 229.296 | 59.289 | 0.07x |

