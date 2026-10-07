# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c16eaa65c61f1cf9d4fac46e9f6d8e6b9f3b087f
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
| users.json | strata | 8.383 | 9.014 | 17.085 | 48.988 | 1.00x |
| users.json | orjson | 11.516 | 12.677 | 17.055 | 48.988 | 0.71x |
| users.json | msgspec | 10.570 | 11.236 | 18.878 | 48.988 | 0.80x |
| users.json | ujson | 17.765 | 19.036 | 25.944 | 48.988 | 0.47x |
| users.json | json | 18.357 | 19.155 | 26.224 | 48.988 | 0.47x |
| flat.json | strata | 0.841 | 0.898 | 1.283 | 57.145 | 1.00x |
| flat.json | orjson | 0.960 | 1.003 | 1.437 | 57.145 | 0.90x |
| flat.json | msgspec | 0.880 | 0.966 | 1.463 | 57.145 | 0.93x |
| flat.json | ujson | 1.473 | 1.575 | 2.499 | 57.145 | 0.57x |
| flat.json | json | 1.485 | 1.570 | 2.548 | 57.145 | 0.57x |
| nested.json | strata | 0.590 | 0.600 | 0.650 | 56.930 | 1.00x |
| nested.json | orjson | 0.858 | 0.887 | 0.926 | 56.930 | 0.68x |
| nested.json | msgspec | 0.760 | 0.783 | 0.808 | 56.930 | 0.77x |
| nested.json | ujson | 1.169 | 1.222 | 1.313 | 56.930 | 0.49x |
| nested.json | json | 1.599 | 1.629 | 1.757 | 56.930 | 0.37x |
| wide_arrays.json | strata | 3.455 | 3.904 | 6.559 | 59.773 | 1.00x |
| wide_arrays.json | orjson | 4.712 | 5.104 | 7.637 | 59.773 | 0.76x |
| wide_arrays.json | msgspec | 4.651 | 4.927 | 7.490 | 59.773 | 0.79x |
| wide_arrays.json | ujson | 6.379 | 6.666 | 10.220 | 59.773 | 0.59x |
| wide_arrays.json | json | 8.992 | 9.531 | 13.137 | 59.773 | 0.41x |
| mixed.json | strata | 0.146 | 0.153 | 0.248 | 56.754 | 1.00x |
| mixed.json | orjson | 0.175 | 0.181 | 0.327 | 56.754 | 0.84x |
| mixed.json | msgspec | 0.185 | 0.202 | 0.347 | 56.754 | 0.76x |
| mixed.json | ujson | 0.262 | 0.289 | 0.467 | 56.754 | 0.53x |
| mixed.json | json | 0.364 | 0.383 | 0.796 | 56.754 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.384 | 2.473 | 3.517 | 48.184 | 1.00x |
| users.json | orjson | 2.594 | 2.794 | 3.597 | 48.184 | 0.89x |
| users.json | msgspec | 4.162 | 4.433 | 4.961 | 48.184 | 0.56x |
| users.json | ujson | 10.307 | 10.443 | 15.992 | 48.184 | 0.24x |
| users.json | json | 17.611 | 18.085 | 24.976 | 48.184 | 0.14x |
| flat.json | strata | 0.254 | 0.262 | 0.278 | 57.719 | 1.00x |
| flat.json | orjson | 0.262 | 0.270 | 0.290 | 57.719 | 0.97x |
| flat.json | msgspec | 0.423 | 0.429 | 0.485 | 57.719 | 0.61x |
| flat.json | ujson | 1.124 | 1.157 | 1.183 | 57.719 | 0.23x |
| flat.json | json | 1.544 | 1.560 | 1.607 | 57.719 | 0.17x |
| nested.json | strata | 0.189 | 0.200 | 0.226 | 57.410 | 1.00x |
| nested.json | orjson | 0.236 | 0.246 | 0.285 | 57.410 | 0.81x |
| nested.json | msgspec | 0.391 | 0.408 | 0.436 | 57.410 | 0.49x |
| nested.json | ujson | 0.808 | 0.831 | 0.853 | 57.410 | 0.24x |
| nested.json | json | 1.940 | 1.964 | 2.119 | 57.410 | 0.10x |
| wide_arrays.json | strata | 1.670 | 1.704 | 2.427 | 58.383 | 1.00x |
| wide_arrays.json | orjson | 2.060 | 2.144 | 2.826 | 58.383 | 0.80x |
| wide_arrays.json | msgspec | 3.486 | 3.624 | 4.864 | 58.383 | 0.47x |
| wide_arrays.json | ujson | 6.148 | 6.265 | 7.142 | 58.383 | 0.27x |
| wide_arrays.json | json | 14.349 | 14.451 | 14.826 | 58.383 | 0.12x |
| mixed.json | strata | 0.054 | 0.058 | 0.083 | 56.840 | 1.00x |
| mixed.json | orjson | 0.053 | 0.055 | 0.078 | 56.840 | 1.06x |
| mixed.json | msgspec | 0.078 | 0.081 | 0.115 | 56.840 | 0.71x |
| mixed.json | ujson | 0.203 | 0.208 | 0.319 | 56.840 | 0.28x |
| mixed.json | json | 0.399 | 0.410 | 0.689 | 56.840 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.899 | 9.284 | 13.625 | 58.207 | 1.00x |
| users.json | orjson | 12.240 | 12.618 | 14.667 | 58.207 | 0.74x |
| users.json | msgspec | 11.269 | 11.560 | 17.788 | 58.207 | 0.80x |
| users.json | ujson | 19.387 | 20.230 | 29.976 | 58.207 | 0.46x |
| users.json | json | 18.459 | 18.909 | 28.985 | 58.207 | 0.49x |
| flat.json | strata | 0.979 | 0.991 | 1.038 | 57.105 | 1.00x |
| flat.json | orjson | 1.083 | 1.111 | 1.212 | 57.105 | 0.89x |
| flat.json | msgspec | 1.006 | 1.111 | 1.494 | 57.105 | 0.89x |
| flat.json | ujson | 1.768 | 1.834 | 1.933 | 57.105 | 0.54x |
| flat.json | json | 1.593 | 1.642 | 1.715 | 57.105 | 0.60x |
| nested.json | strata | 0.696 | 0.719 | 1.102 | 56.945 | 1.00x |
| nested.json | orjson | 0.988 | 1.065 | 1.585 | 56.945 | 0.68x |
| nested.json | msgspec | 0.899 | 0.939 | 1.521 | 56.945 | 0.77x |
| nested.json | ujson | 1.557 | 1.623 | 2.584 | 56.945 | 0.44x |
| nested.json | json | 1.732 | 1.777 | 2.915 | 56.945 | 0.40x |
| wide_arrays.json | strata | 3.893 | 4.173 | 5.998 | 58.383 | 1.00x |
| wide_arrays.json | orjson | 5.083 | 5.336 | 7.840 | 58.383 | 0.78x |
| wide_arrays.json | msgspec | 5.080 | 5.328 | 5.769 | 58.383 | 0.78x |
| wide_arrays.json | ujson | 8.342 | 8.464 | 13.123 | 58.383 | 0.49x |
| wide_arrays.json | json | 9.208 | 9.826 | 13.215 | 58.383 | 0.42x |
| mixed.json | strata | 0.217 | 0.227 | 0.261 | 56.840 | 1.00x |
| mixed.json | orjson | 0.271 | 0.284 | 0.315 | 56.840 | 0.80x |
| mixed.json | msgspec | 0.276 | 0.285 | 0.336 | 56.840 | 0.79x |
| mixed.json | ujson | 0.402 | 0.422 | 0.458 | 56.840 | 0.54x |
| mixed.json | json | 0.465 | 0.487 | 0.513 | 56.840 | 0.47x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 8.785 | 9.341 | 13.774 | 58.352 | 1.00x |
| users.ndjson | orjson | 14.484 | 15.230 | 20.169 | 58.352 | 0.61x |
| users.ndjson | msgspec | 14.581 | 14.967 | 17.026 | 58.352 | 0.62x |
| users.ndjson | ujson | 20.432 | 21.402 | 28.658 | 58.352 | 0.44x |
| users.ndjson | json | 24.456 | 24.951 | 27.774 | 58.352 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.184 | 3.225 | 3.326 | 59.305 | 1.00x |
| users.json | orjson | 3.333 | 3.453 | 4.937 | 59.305 | 0.93x |
| users.json | msgspec | 4.958 | 5.244 | 7.085 | 59.305 | 0.61x |
| users.json | ujson | 16.990 | 17.156 | 35.487 | 59.305 | 0.19x |
| users.json | json | 24.909 | 25.143 | 44.821 | 59.305 | 0.13x |
| flat.json | strata | 0.517 | 0.555 | 0.706 | 57.742 | 1.00x |
| flat.json | orjson | 0.548 | 0.587 | 0.816 | 57.742 | 0.95x |
| flat.json | msgspec | 0.721 | 0.740 | 1.000 | 57.742 | 0.75x |
| flat.json | ujson | 2.238 | 2.276 | 3.482 | 57.742 | 0.24x |
| flat.json | json | 2.609 | 2.658 | 4.327 | 57.742 | 0.21x |
| nested.json | strata | 0.492 | 0.513 | 0.570 | 57.383 | 1.00x |
| nested.json | orjson | 0.562 | 0.613 | 0.651 | 57.383 | 0.84x |
| nested.json | msgspec | 0.703 | 0.759 | 0.791 | 57.383 | 0.68x |
| nested.json | ujson | 1.658 | 1.703 | 2.835 | 57.383 | 0.30x |
| nested.json | json | 2.816 | 2.879 | 4.503 | 57.383 | 0.18x |
| wide_arrays.json | strata | 2.293 | 2.364 | 3.288 | 58.383 | 1.00x |
| wide_arrays.json | orjson | 2.667 | 2.734 | 3.980 | 58.383 | 0.86x |
| wide_arrays.json | msgspec | 4.108 | 4.277 | 5.946 | 58.383 | 0.55x |
| wide_arrays.json | ujson | 11.486 | 11.644 | 15.068 | 58.383 | 0.20x |
| wide_arrays.json | json | 19.788 | 20.205 | 22.708 | 58.383 | 0.12x |
| mixed.json | strata | 0.284 | 0.293 | 0.311 | 56.840 | 1.00x |
| mixed.json | orjson | 0.301 | 0.316 | 0.353 | 56.840 | 0.93x |
| mixed.json | msgspec | 0.328 | 0.373 | 0.411 | 56.840 | 0.79x |
| mixed.json | ujson | 0.578 | 0.605 | 0.641 | 56.840 | 0.48x |
| mixed.json | json | 0.793 | 0.801 | 0.897 | 56.840 | 0.37x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.074 | 0.076 | 0.107 | 59.367 | 1.00x |
| users.json $[*].id | jmespath | 0.330 | 0.334 | 0.365 | 59.367 | 0.23x |
| users.json $[*].id | jsonpath-ng | 1.958 | 2.070 | 2.180 | 59.367 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.404 | 0.446 | 0.717 | 59.387 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.116 | 2.163 | 2.212 | 59.387 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 13.631 | 14.180 | 15.130 | 59.387 | 0.03x |
| users.json $..total | strata | 1.489 | 1.529 | 2.672 | 59.387 | 1.00x |
| users.json $..total | jsonpath-ng | 255.318 | 267.578 | 283.036 | 59.387 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.478 | 3.498 | 3.704 | 59.387 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.210 | 13.829 | 14.610 | 59.387 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 14.974 | 15.277 | 22.996 | 59.387 | 0.23x |
| users.json $[*].orders[*].total | strata | 3.628 | 3.685 | 4.954 | 59.387 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.901 | 16.069 | 23.199 | 59.387 | 0.23x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 30.370 | 31.028 | 45.574 | 59.387 | 0.12x |
| users.json $..total | strata | 12.007 | 14.767 | 21.301 | 59.387 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 278.265 | 296.549 | 318.591 | 59.387 | 0.05x |

