# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 85eb3583e98587844415f5d32f85a61cbedfcf49
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: Intel64 Family 6 Model 173 Stepping 1, GenuineIntel
- compiler_flags: /nologo /O2 /W3 /DNDEBUG /MD /EHsc /std:c++20 /Zc:__cplusplus /D_USE_STD_VECTOR_ALGORITHMS=0 /arch:AVX2 /clang:-O3 /clang:-fprofile-use=D:\a\strata\strata\build\pgo\strata.profdata /DLL /MANIFEST:EMBED,ID=2 /MANIFESTUAC:NO /EXPORT:PyInit__strata (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 5.708 | 6.125 | 9.696 | 48.809 | 1.00x |
| users.json | orjson | 11.706 | 12.240 | 14.544 | 48.809 | 0.50x |
| users.json | msgspec | 10.239 | 10.765 | 14.345 | 48.809 | 0.57x |
| users.json | ujson | 13.931 | 14.779 | 19.261 | 48.809 | 0.41x |
| users.json | json | 16.599 | 17.079 | 19.037 | 48.809 | 0.36x |
| flat.json | strata | 0.638 | 0.801 | 1.056 | 57.211 | 1.00x |
| flat.json | orjson | 0.965 | 1.019 | 1.088 | 57.211 | 0.79x |
| flat.json | msgspec | 0.784 | 0.848 | 1.021 | 57.211 | 0.94x |
| flat.json | ujson | 1.115 | 1.264 | 1.421 | 57.211 | 0.63x |
| flat.json | json | 1.363 | 1.420 | 1.570 | 57.211 | 0.56x |
| nested.json | strata | 0.460 | 0.499 | 0.594 | 56.875 | 1.00x |
| nested.json | orjson | 0.743 | 0.791 | 1.114 | 56.875 | 0.63x |
| nested.json | msgspec | 0.589 | 0.619 | 0.955 | 56.875 | 0.81x |
| nested.json | ujson | 0.898 | 0.948 | 1.557 | 56.875 | 0.53x |
| nested.json | json | 1.315 | 1.384 | 1.628 | 56.875 | 0.36x |
| wide_arrays.json | strata | 2.802 | 2.882 | 4.256 | 59.152 | 1.00x |
| wide_arrays.json | orjson | 4.988 | 5.223 | 5.346 | 59.152 | 0.55x |
| wide_arrays.json | msgspec | 4.359 | 4.514 | 4.726 | 59.152 | 0.64x |
| wide_arrays.json | ujson | 5.895 | 6.083 | 6.272 | 59.152 | 0.47x |
| wide_arrays.json | json | 8.394 | 8.597 | 8.804 | 59.152 | 0.34x |
| mixed.json | strata | 0.127 | 0.131 | 0.180 | 59.145 | 1.00x |
| mixed.json | orjson | 0.145 | 0.153 | 0.224 | 59.145 | 0.86x |
| mixed.json | msgspec | 0.165 | 0.178 | 0.241 | 59.145 | 0.73x |
| mixed.json | ujson | 0.220 | 0.225 | 0.319 | 59.145 | 0.58x |
| mixed.json | json | 0.322 | 0.330 | 0.441 | 59.145 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.000 | 2.217 | 2.289 | 48.191 | 1.00x |
| users.json | orjson | 2.558 | 2.842 | 3.061 | 48.191 | 0.78x |
| users.json | msgspec | 3.957 | 4.560 | 4.875 | 48.191 | 0.49x |
| users.json | ujson | 9.318 | 9.829 | 10.492 | 48.191 | 0.23x |
| users.json | json | 16.315 | 17.252 | 18.044 | 48.191 | 0.13x |
| flat.json | strata | 0.229 | 0.233 | 0.279 | 57.297 | 1.00x |
| flat.json | orjson | 0.241 | 0.251 | 0.292 | 57.297 | 0.93x |
| flat.json | msgspec | 0.368 | 0.396 | 0.444 | 57.297 | 0.59x |
| flat.json | ujson | 0.781 | 0.827 | 1.165 | 57.297 | 0.28x |
| flat.json | json | 1.271 | 1.387 | 2.081 | 57.297 | 0.17x |
| nested.json | strata | 0.148 | 0.169 | 0.233 | 57.344 | 1.00x |
| nested.json | orjson | 0.236 | 0.272 | 0.337 | 57.344 | 0.62x |
| nested.json | msgspec | 0.312 | 0.352 | 0.421 | 57.344 | 0.48x |
| nested.json | ujson | 0.756 | 0.825 | 0.884 | 57.344 | 0.21x |
| nested.json | json | 1.615 | 1.769 | 1.898 | 57.344 | 0.10x |
| wide_arrays.json | strata | 1.400 | 1.452 | 1.598 | 58.148 | 1.00x |
| wide_arrays.json | orjson | 1.805 | 1.990 | 2.291 | 58.148 | 0.73x |
| wide_arrays.json | msgspec | 3.236 | 3.457 | 3.781 | 58.148 | 0.42x |
| wide_arrays.json | ujson | 5.363 | 5.874 | 6.513 | 58.148 | 0.25x |
| wide_arrays.json | json | 11.734 | 12.210 | 12.852 | 58.148 | 0.12x |
| mixed.json | strata | 0.053 | 0.055 | 0.073 | 59.230 | 1.00x |
| mixed.json | orjson | 0.057 | 0.059 | 0.065 | 59.230 | 0.94x |
| mixed.json | msgspec | 0.072 | 0.082 | 0.139 | 59.230 | 0.67x |
| mixed.json | ujson | 0.161 | 0.182 | 0.302 | 59.230 | 0.30x |
| mixed.json | json | 0.376 | 0.409 | 0.578 | 59.230 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.886 | 7.780 | 10.041 | 58.219 | 1.00x |
| users.json | orjson | 12.466 | 13.373 | 14.105 | 58.219 | 0.58x |
| users.json | msgspec | 11.662 | 12.007 | 12.806 | 58.219 | 0.65x |
| users.json | ujson | 17.373 | 18.594 | 19.754 | 58.219 | 0.42x |
| users.json | json | 17.652 | 18.503 | 18.993 | 58.219 | 0.42x |
| flat.json | strata | 0.969 | 1.085 | 1.167 | 56.820 | 1.00x |
| flat.json | orjson | 1.077 | 1.205 | 1.342 | 56.820 | 0.90x |
| flat.json | msgspec | 0.917 | 0.981 | 1.111 | 56.820 | 1.11x |
| flat.json | ujson | 1.531 | 1.628 | 1.744 | 56.820 | 0.67x |
| flat.json | json | 1.506 | 1.576 | 1.775 | 56.820 | 0.69x |
| nested.json | strata | 0.531 | 0.627 | 0.701 | 57.379 | 1.00x |
| nested.json | orjson | 0.930 | 0.993 | 1.527 | 57.379 | 0.63x |
| nested.json | msgspec | 0.739 | 0.890 | 1.184 | 57.379 | 0.70x |
| nested.json | ujson | 1.183 | 1.363 | 1.586 | 57.379 | 0.46x |
| nested.json | json | 1.531 | 1.662 | 1.925 | 57.379 | 0.38x |
| wide_arrays.json | strata | 3.327 | 3.458 | 3.875 | 58.148 | 1.00x |
| wide_arrays.json | orjson | 5.491 | 5.601 | 6.156 | 58.148 | 0.62x |
| wide_arrays.json | msgspec | 5.060 | 5.106 | 5.462 | 58.148 | 0.68x |
| wide_arrays.json | ujson | 7.927 | 8.168 | 9.577 | 58.148 | 0.42x |
| wide_arrays.json | json | 8.991 | 9.071 | 11.336 | 58.148 | 0.38x |
| mixed.json | strata | 0.208 | 0.221 | 0.256 | 59.230 | 1.00x |
| mixed.json | orjson | 0.270 | 0.279 | 0.327 | 59.230 | 0.79x |
| mixed.json | msgspec | 0.283 | 0.323 | 0.362 | 59.230 | 0.68x |
| mixed.json | ujson | 0.368 | 0.402 | 0.468 | 59.230 | 0.55x |
| mixed.json | json | 0.432 | 0.470 | 0.500 | 59.230 | 0.47x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.043 | 7.250 | 8.430 | 58.164 | 1.00x |
| users.ndjson | orjson | 12.791 | 13.109 | 14.049 | 58.164 | 0.55x |
| users.ndjson | msgspec | 12.859 | 13.071 | 13.447 | 58.164 | 0.55x |
| users.ndjson | ujson | 16.271 | 16.591 | 19.204 | 58.164 | 0.44x |
| users.ndjson | json | 20.560 | 21.274 | 21.456 | 58.164 | 0.34x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.724 | 3.078 | 3.254 | 57.820 | 1.00x |
| users.json | orjson | 3.836 | 4.001 | 4.312 | 57.820 | 0.77x |
| users.json | msgspec | 4.971 | 5.594 | 6.972 | 57.820 | 0.55x |
| users.json | ujson | 15.003 | 15.934 | 18.122 | 57.820 | 0.19x |
| users.json | json | 22.761 | 23.498 | 26.881 | 57.820 | 0.13x |
| flat.json | strata | 0.475 | 0.493 | 0.558 | 57.375 | 1.00x |
| flat.json | orjson | 0.504 | 0.532 | 0.629 | 57.375 | 0.93x |
| flat.json | msgspec | 0.661 | 0.674 | 0.768 | 57.375 | 0.73x |
| flat.json | ujson | 1.640 | 1.700 | 1.949 | 57.375 | 0.29x |
| flat.json | json | 2.224 | 2.273 | 2.577 | 57.375 | 0.22x |
| nested.json | strata | 0.408 | 0.478 | 0.590 | 57.379 | 1.00x |
| nested.json | orjson | 0.602 | 0.669 | 0.727 | 57.379 | 0.71x |
| nested.json | msgspec | 0.653 | 0.762 | 0.841 | 57.379 | 0.63x |
| nested.json | ujson | 1.626 | 1.654 | 1.814 | 57.379 | 0.29x |
| nested.json | json | 2.463 | 2.721 | 2.848 | 57.379 | 0.18x |
| wide_arrays.json | strata | 1.973 | 2.046 | 2.178 | 60.250 | 1.00x |
| wide_arrays.json | orjson | 2.400 | 2.585 | 3.210 | 60.250 | 0.79x |
| wide_arrays.json | msgspec | 3.457 | 4.025 | 4.209 | 60.250 | 0.51x |
| wide_arrays.json | ujson | 9.704 | 10.187 | 10.964 | 60.250 | 0.20x |
| wide_arrays.json | json | 15.947 | 16.447 | 17.233 | 60.250 | 0.12x |
| mixed.json | strata | 0.272 | 0.290 | 0.356 | 59.258 | 1.00x |
| mixed.json | orjson | 0.311 | 0.348 | 123.519 | 59.258 | 0.83x |
| mixed.json | msgspec | 0.329 | 0.367 | 0.414 | 59.258 | 0.79x |
| mixed.json | ujson | 0.500 | 0.572 | 75.073 | 59.258 | 0.51x |
| mixed.json | json | 0.725 | 0.807 | 1.260 | 59.258 | 0.36x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.059 | 0.062 | 0.092 | 58.094 | 1.00x |
| users.json $[*].id | jmespath | 0.285 | 0.319 | 0.447 | 58.094 | 0.19x |
| users.json $[*].id | jsonpath-ng | 1.485 | 1.731 | 2.295 | 58.094 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.407 | 0.437 | 0.481 | 58.242 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.767 | 1.944 | 2.051 | 58.242 | 0.22x |
| users.json $[*].orders[*].total | jsonpath-ng | 11.006 | 11.619 | 16.633 | 58.242 | 0.04x |
| users.json $..total | strata | 1.368 | 1.633 | 1.674 | 58.242 | 1.00x |
| users.json $..total | jsonpath-ng | 199.714 | 202.410 | 211.861 | 58.242 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.248 | 3.578 | 4.278 | 58.113 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.696 | 13.242 | 13.741 | 58.113 | 0.27x |
| users.json $[*].id | orjson+jsonpath-ng | 14.029 | 14.893 | 15.882 | 58.113 | 0.24x |
| users.json $[*].orders[*].total | strata | 3.271 | 3.715 | 3.981 | 58.242 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 14.446 | 15.319 | 15.560 | 58.242 | 0.24x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 28.928 | 29.782 | 31.112 | 58.242 | 0.12x |
| users.json $..total | strata | 9.064 | 9.310 | 11.436 | 58.242 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 215.626 | 218.345 | 223.428 | 58.242 | 0.04x |

