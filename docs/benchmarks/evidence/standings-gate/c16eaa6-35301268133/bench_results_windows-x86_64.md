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
| users.json | strata | 9.400 | 9.698 | 13.824 | 48.945 | 1.00x |
| users.json | orjson | 13.521 | 13.998 | 15.622 | 48.945 | 0.69x |
| users.json | msgspec | 12.956 | 13.095 | 15.103 | 48.945 | 0.74x |
| users.json | ujson | 21.774 | 22.747 | 25.473 | 48.945 | 0.43x |
| users.json | json | 22.382 | 22.903 | 27.182 | 48.945 | 0.42x |
| flat.json | strata | 0.871 | 0.900 | 0.961 | 57.758 | 1.00x |
| flat.json | orjson | 1.154 | 1.203 | 1.231 | 57.758 | 0.75x |
| flat.json | msgspec | 1.120 | 1.165 | 1.205 | 57.758 | 0.77x |
| flat.json | ujson | 2.254 | 2.329 | 2.381 | 57.758 | 0.39x |
| flat.json | json | 1.986 | 2.016 | 2.074 | 57.758 | 0.45x |
| nested.json | strata | 0.749 | 0.786 | 0.854 | 56.797 | 1.00x |
| nested.json | orjson | 1.072 | 1.119 | 1.158 | 56.797 | 0.70x |
| nested.json | msgspec | 1.003 | 1.047 | 1.096 | 56.797 | 0.75x |
| nested.json | ujson | 1.639 | 1.676 | 1.722 | 56.797 | 0.47x |
| nested.json | json | 2.159 | 2.181 | 2.292 | 56.797 | 0.36x |
| wide_arrays.json | strata | 4.223 | 4.415 | 6.154 | 58.758 | 1.00x |
| wide_arrays.json | orjson | 5.589 | 5.930 | 9.953 | 58.758 | 0.74x |
| wide_arrays.json | msgspec | 5.764 | 5.978 | 7.379 | 58.758 | 0.74x |
| wide_arrays.json | ujson | 8.315 | 8.515 | 11.420 | 58.758 | 0.52x |
| wide_arrays.json | json | 11.915 | 12.687 | 19.048 | 58.758 | 0.35x |
| mixed.json | strata | 0.182 | 0.185 | 0.205 | 56.699 | 1.00x |
| mixed.json | orjson | 0.211 | 0.217 | 0.240 | 56.699 | 0.85x |
| mixed.json | msgspec | 0.229 | 0.247 | 0.313 | 56.699 | 0.75x |
| mixed.json | ujson | 0.343 | 0.353 | 0.400 | 56.699 | 0.52x |
| mixed.json | json | 0.469 | 0.496 | 0.533 | 56.699 | 0.37x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.019 | 3.066 | 4.369 | 48.004 | 1.00x |
| users.json | orjson | 3.541 | 3.739 | 3.824 | 48.004 | 0.82x |
| users.json | msgspec | 4.898 | 5.038 | 7.054 | 48.004 | 0.61x |
| users.json | ujson | 14.061 | 14.223 | 19.439 | 48.004 | 0.22x |
| users.json | json | 23.838 | 24.210 | 24.727 | 48.004 | 0.13x |
| flat.json | strata | 0.301 | 0.348 | 0.429 | 57.465 | 1.00x |
| flat.json | orjson | 0.367 | 0.387 | 0.570 | 57.465 | 0.90x |
| flat.json | msgspec | 0.523 | 0.538 | 0.782 | 57.465 | 0.65x |
| flat.json | ujson | 1.444 | 1.485 | 2.203 | 57.465 | 0.23x |
| flat.json | json | 1.992 | 2.044 | 3.337 | 57.465 | 0.17x |
| nested.json | strata | 0.273 | 0.277 | 0.314 | 57.332 | 1.00x |
| nested.json | orjson | 0.329 | 0.333 | 0.371 | 57.332 | 0.83x |
| nested.json | msgspec | 0.479 | 0.482 | 0.520 | 57.332 | 0.58x |
| nested.json | ujson | 1.207 | 1.262 | 1.301 | 57.332 | 0.22x |
| nested.json | json | 2.468 | 2.489 | 2.573 | 57.332 | 0.11x |
| wide_arrays.json | strata | 1.902 | 2.068 | 2.465 | 58.648 | 1.00x |
| wide_arrays.json | orjson | 2.505 | 2.575 | 2.829 | 58.648 | 0.80x |
| wide_arrays.json | msgspec | 3.970 | 4.210 | 4.728 | 58.648 | 0.49x |
| wide_arrays.json | ujson | 7.574 | 8.005 | 8.567 | 58.648 | 0.26x |
| wide_arrays.json | json | 19.095 | 19.918 | 20.291 | 58.648 | 0.10x |
| mixed.json | strata | 0.068 | 0.070 | 0.104 | 56.820 | 1.00x |
| mixed.json | orjson | 0.069 | 0.070 | 0.073 | 56.820 | 0.99x |
| mixed.json | msgspec | 0.091 | 0.093 | 0.129 | 56.820 | 0.75x |
| mixed.json | ujson | 0.263 | 0.268 | 0.298 | 56.820 | 0.26x |
| mixed.json | json | 0.510 | 0.514 | 0.618 | 56.820 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.527 | 11.356 | 12.561 | 58.027 | 1.00x |
| users.json | orjson | 14.523 | 15.038 | 21.658 | 58.027 | 0.76x |
| users.json | msgspec | 14.074 | 14.593 | 19.089 | 58.027 | 0.78x |
| users.json | ujson | 26.903 | 27.795 | 28.393 | 58.027 | 0.41x |
| users.json | json | 23.567 | 24.107 | 35.478 | 58.027 | 0.47x |
| flat.json | strata | 1.183 | 1.310 | 1.693 | 56.977 | 1.00x |
| flat.json | orjson | 1.480 | 1.515 | 2.147 | 56.977 | 0.86x |
| flat.json | msgspec | 1.303 | 1.429 | 2.125 | 56.977 | 0.92x |
| flat.json | ujson | 2.859 | 2.956 | 3.188 | 56.977 | 0.44x |
| flat.json | json | 2.142 | 2.198 | 2.244 | 56.977 | 0.60x |
| nested.json | strata | 0.863 | 0.898 | 1.016 | 56.805 | 1.00x |
| nested.json | orjson | 1.184 | 1.278 | 1.359 | 56.805 | 0.70x |
| nested.json | msgspec | 1.146 | 1.207 | 1.252 | 56.805 | 0.74x |
| nested.json | ujson | 2.082 | 2.193 | 2.767 | 56.805 | 0.41x |
| nested.json | json | 2.314 | 2.336 | 2.411 | 56.805 | 0.38x |
| wide_arrays.json | strata | 4.586 | 4.871 | 5.827 | 58.648 | 1.00x |
| wide_arrays.json | orjson | 5.935 | 6.162 | 6.427 | 58.648 | 0.79x |
| wide_arrays.json | msgspec | 6.177 | 6.342 | 7.153 | 58.648 | 0.77x |
| wide_arrays.json | ujson | 11.222 | 11.555 | 15.011 | 58.648 | 0.42x |
| wide_arrays.json | json | 12.017 | 12.628 | 15.179 | 58.648 | 0.39x |
| mixed.json | strata | 0.249 | 0.254 | 0.302 | 56.848 | 1.00x |
| mixed.json | orjson | 0.322 | 0.328 | 0.476 | 56.848 | 0.78x |
| mixed.json | msgspec | 0.346 | 0.354 | 0.508 | 56.848 | 0.72x |
| mixed.json | ujson | 0.529 | 0.550 | 0.834 | 56.848 | 0.46x |
| mixed.json | json | 0.583 | 0.594 | 1.012 | 56.848 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 12.518 | 12.963 | 20.233 | 58.402 | 1.00x |
| users.ndjson | orjson | 18.501 | 19.470 | 25.737 | 58.402 | 0.67x |
| users.ndjson | msgspec | 18.228 | 19.552 | 21.515 | 58.402 | 0.66x |
| users.ndjson | ujson | 27.454 | 28.921 | 35.165 | 58.402 | 0.45x |
| users.ndjson | json | 32.379 | 33.041 | 36.370 | 58.402 | 0.39x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.891 | 3.969 | 6.432 | 58.203 | 1.00x |
| users.json | orjson | 4.579 | 4.776 | 4.907 | 58.203 | 0.83x |
| users.json | msgspec | 5.985 | 6.155 | 7.357 | 58.203 | 0.64x |
| users.json | ujson | 23.475 | 27.493 | 35.506 | 58.203 | 0.14x |
| users.json | json | 32.645 | 33.697 | 47.434 | 58.203 | 0.12x |
| flat.json | strata | 0.674 | 0.751 | 1.006 | 57.652 | 1.00x |
| flat.json | orjson | 0.773 | 0.850 | 1.299 | 57.652 | 0.88x |
| flat.json | msgspec | 0.915 | 1.025 | 1.369 | 57.652 | 0.73x |
| flat.json | ujson | 2.849 | 2.940 | 4.598 | 57.652 | 0.26x |
| flat.json | json | 3.443 | 3.554 | 5.879 | 57.652 | 0.21x |
| nested.json | strata | 0.587 | 0.639 | 0.884 | 57.332 | 1.00x |
| nested.json | orjson | 0.717 | 0.764 | 0.798 | 57.332 | 0.84x |
| nested.json | msgspec | 0.820 | 0.891 | 1.024 | 57.332 | 0.72x |
| nested.json | ujson | 2.315 | 2.373 | 2.439 | 57.332 | 0.27x |
| nested.json | json | 3.521 | 3.607 | 3.703 | 57.332 | 0.18x |
| wide_arrays.json | strata | 2.571 | 2.741 | 3.672 | 58.402 | 1.00x |
| wide_arrays.json | orjson | 3.173 | 3.398 | 4.729 | 58.402 | 0.81x |
| wide_arrays.json | msgspec | 4.698 | 4.909 | 5.273 | 58.402 | 0.56x |
| wide_arrays.json | ujson | 14.428 | 14.892 | 16.219 | 58.402 | 0.18x |
| wide_arrays.json | json | 25.467 | 26.306 | 28.268 | 58.402 | 0.10x |
| mixed.json | strata | 0.342 | 0.369 | 0.439 | 56.848 | 1.00x |
| mixed.json | orjson | 0.382 | 0.397 | 5.561 | 56.848 | 0.93x |
| mixed.json | msgspec | 0.401 | 0.412 | 0.534 | 56.848 | 0.89x |
| mixed.json | ujson | 0.745 | 0.788 | 1.153 | 56.848 | 0.47x |
| mixed.json | json | 1.007 | 1.069 | 1.759 | 56.848 | 0.34x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.094 | 0.100 | 0.239 | 58.254 | 1.00x |
| users.json $[*].id | jmespath | 0.451 | 0.486 | 0.846 | 58.254 | 0.21x |
| users.json $[*].id | jsonpath-ng | 2.627 | 2.906 | 4.890 | 58.254 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.473 | 0.515 | 0.796 | 58.273 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.759 | 2.837 | 3.902 | 58.273 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 18.036 | 19.671 | 27.566 | 58.273 | 0.03x |
| users.json $..total | strata | 1.973 | 2.011 | 2.341 | 58.273 | 1.00x |
| users.json $..total | jsonpath-ng | 328.885 | 343.261 | 390.802 | 58.273 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.015 | 4.053 | 4.850 | 58.273 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.829 | 16.243 | 17.676 | 58.273 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 18.026 | 18.726 | 28.367 | 58.273 | 0.22x |
| users.json $[*].orders[*].total | strata | 4.255 | 4.289 | 6.165 | 58.273 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.792 | 19.947 | 24.345 | 58.273 | 0.22x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 38.288 | 40.255 | 45.936 | 58.273 | 0.11x |
| users.json $..total | strata | 14.753 | 17.578 | 28.913 | 58.273 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 349.487 | 375.526 | 419.706 | 58.273 | 0.05x |

