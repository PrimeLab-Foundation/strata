# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 565fab210bb851772fcefe65082041084aeafc14
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: Intel64 Family 6 Model 207 Stepping 2, GenuineIntel
- compiler_flags: /nologo /O2 /W3 /DNDEBUG /MD /EHsc /std:c++20 /Zc:__cplusplus /D_USE_STD_VECTOR_ALGORITHMS=0 /arch:AVX2 /clang:-O3 /clang:-fprofile-use=D:\a\strata\strata\build\pgo\strata.profdata /DLL /MANIFEST:EMBED,ID=2 /MANIFESTUAC:NO /EXPORT:PyInit__strata (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.372 | 8.563 | 13.352 | 48.871 | 1.00x |
| users.json | orjson | 14.744 | 14.977 | 18.957 | 48.871 | 0.57x |
| users.json | msgspec | 13.731 | 13.900 | 16.977 | 48.871 | 0.62x |
| users.json | ujson | 19.700 | 20.076 | 24.665 | 48.871 | 0.43x |
| users.json | json | 22.651 | 23.119 | 30.013 | 48.871 | 0.37x |
| flat.json | strata | 1.127 | 1.169 | 1.336 | 57.074 | 1.00x |
| flat.json | orjson | 1.325 | 1.365 | 1.423 | 57.074 | 0.86x |
| flat.json | msgspec | 1.096 | 1.155 | 1.201 | 57.074 | 1.01x |
| flat.json | ujson | 1.743 | 1.795 | 1.865 | 57.074 | 0.65x |
| flat.json | json | 1.954 | 1.965 | 1.992 | 57.074 | 0.59x |
| nested.json | strata | 0.663 | 0.709 | 0.749 | 56.883 | 1.00x |
| nested.json | orjson | 0.987 | 1.058 | 1.236 | 56.883 | 0.67x |
| nested.json | msgspec | 0.862 | 0.896 | 1.450 | 56.883 | 0.79x |
| nested.json | ujson | 1.298 | 1.364 | 1.391 | 56.883 | 0.52x |
| nested.json | json | 1.932 | 1.949 | 2.339 | 56.883 | 0.36x |
| wide_arrays.json | strata | 4.007 | 4.073 | 4.123 | 58.848 | 1.00x |
| wide_arrays.json | orjson | 6.248 | 6.302 | 7.874 | 58.848 | 0.65x |
| wide_arrays.json | msgspec | 6.032 | 6.074 | 6.217 | 58.848 | 0.67x |
| wide_arrays.json | ujson | 8.054 | 8.101 | 8.285 | 58.848 | 0.50x |
| wide_arrays.json | json | 11.460 | 11.693 | 13.557 | 58.848 | 0.35x |
| mixed.json | strata | 0.183 | 0.192 | 0.226 | 57.391 | 1.00x |
| mixed.json | orjson | 0.215 | 0.231 | 0.278 | 57.391 | 0.83x |
| mixed.json | msgspec | 0.235 | 0.246 | 0.319 | 57.391 | 0.78x |
| mixed.json | ujson | 0.314 | 0.327 | 0.479 | 57.391 | 0.59x |
| mixed.json | json | 0.478 | 0.508 | 0.676 | 57.391 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.605 | 2.667 | 3.102 | 48.156 | 1.00x |
| users.json | orjson | 3.309 | 3.456 | 3.595 | 48.156 | 0.77x |
| users.json | msgspec | 5.321 | 5.603 | 6.185 | 48.156 | 0.48x |
| users.json | ujson | 12.586 | 12.688 | 13.044 | 48.156 | 0.21x |
| users.json | json | 22.668 | 22.883 | 23.670 | 48.156 | 0.12x |
| flat.json | strata | 0.321 | 0.331 | 0.381 | 57.473 | 1.00x |
| flat.json | orjson | 0.336 | 0.350 | 0.437 | 57.473 | 0.95x |
| flat.json | msgspec | 0.497 | 0.527 | 0.563 | 57.473 | 0.63x |
| flat.json | ujson | 1.102 | 1.146 | 1.216 | 57.473 | 0.29x |
| flat.json | json | 1.889 | 1.917 | 1.958 | 57.473 | 0.17x |
| nested.json | strata | 0.207 | 0.220 | 0.271 | 57.336 | 1.00x |
| nested.json | orjson | 0.323 | 0.339 | 0.377 | 57.336 | 0.65x |
| nested.json | msgspec | 0.461 | 0.469 | 0.515 | 57.336 | 0.47x |
| nested.json | ujson | 0.989 | 1.043 | 1.093 | 57.336 | 0.21x |
| nested.json | json | 2.251 | 2.278 | 2.321 | 57.336 | 0.10x |
| wide_arrays.json | strata | 2.010 | 2.032 | 2.062 | 58.406 | 1.00x |
| wide_arrays.json | orjson | 2.678 | 2.826 | 2.901 | 58.406 | 0.72x |
| wide_arrays.json | msgspec | 4.436 | 4.524 | 4.580 | 58.406 | 0.45x |
| wide_arrays.json | ujson | 7.806 | 7.878 | 8.149 | 58.406 | 0.26x |
| wide_arrays.json | json | 17.747 | 17.998 | 18.524 | 58.406 | 0.11x |
| mixed.json | strata | 0.069 | 0.071 | 0.078 | 57.426 | 1.00x |
| mixed.json | orjson | 0.073 | 0.077 | 0.078 | 57.426 | 0.93x |
| mixed.json | msgspec | 0.103 | 0.108 | 0.135 | 57.426 | 0.66x |
| mixed.json | ujson | 0.235 | 0.240 | 0.284 | 57.426 | 0.30x |
| mixed.json | json | 0.542 | 0.563 | 0.615 | 57.426 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.747 | 10.431 | 12.111 | 58.184 | 1.00x |
| users.json | orjson | 16.182 | 16.526 | 17.139 | 58.184 | 0.63x |
| users.json | msgspec | 14.939 | 15.407 | 21.186 | 58.184 | 0.68x |
| users.json | ujson | 23.505 | 24.368 | 26.514 | 58.184 | 0.43x |
| users.json | json | 24.105 | 24.749 | 39.425 | 58.184 | 0.42x |
| flat.json | strata | 1.349 | 1.383 | 1.418 | 57.062 | 1.00x |
| flat.json | orjson | 1.502 | 1.597 | 1.650 | 57.062 | 0.87x |
| flat.json | msgspec | 1.419 | 1.480 | 1.598 | 57.062 | 0.93x |
| flat.json | ujson | 2.349 | 2.422 | 2.649 | 57.062 | 0.57x |
| flat.json | json | 2.156 | 2.180 | 2.261 | 57.062 | 0.63x |
| nested.json | strata | 0.765 | 0.796 | 1.309 | 57.340 | 1.00x |
| nested.json | orjson | 1.174 | 1.241 | 1.812 | 57.340 | 0.64x |
| nested.json | msgspec | 1.026 | 1.070 | 1.595 | 57.340 | 0.74x |
| nested.json | ujson | 1.626 | 1.724 | 1.880 | 57.340 | 0.46x |
| nested.json | json | 2.110 | 2.123 | 2.138 | 57.340 | 0.37x |
| wide_arrays.json | strata | 4.693 | 4.793 | 4.970 | 58.406 | 1.00x |
| wide_arrays.json | orjson | 6.950 | 7.065 | 7.561 | 58.406 | 0.68x |
| wide_arrays.json | msgspec | 6.842 | 6.917 | 7.117 | 58.406 | 0.69x |
| wide_arrays.json | ujson | 10.706 | 10.877 | 11.988 | 58.406 | 0.44x |
| wide_arrays.json | json | 12.218 | 12.406 | 12.667 | 58.406 | 0.39x |
| mixed.json | strata | 0.286 | 0.308 | 0.360 | 57.426 | 1.00x |
| mixed.json | orjson | 0.363 | 0.415 | 0.440 | 57.426 | 0.74x |
| mixed.json | msgspec | 0.381 | 0.396 | 0.614 | 57.426 | 0.78x |
| mixed.json | ujson | 0.522 | 0.539 | 0.725 | 57.426 | 0.57x |
| mixed.json | json | 0.624 | 0.647 | 0.722 | 57.426 | 0.48x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.384 | 10.726 | 13.578 | 58.352 | 1.00x |
| users.ndjson | orjson | 18.355 | 18.832 | 20.522 | 58.352 | 0.57x |
| users.ndjson | msgspec | 17.781 | 18.248 | 18.965 | 58.352 | 0.59x |
| users.ndjson | ujson | 24.101 | 24.465 | 25.945 | 58.352 | 0.44x |
| users.ndjson | json | 29.987 | 30.261 | 31.167 | 58.352 | 0.35x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.762 | 3.823 | 3.874 | 58.262 | 1.00x |
| users.json | orjson | 4.534 | 4.656 | 6.005 | 58.262 | 0.82x |
| users.json | msgspec | 6.522 | 6.747 | 6.843 | 58.262 | 0.57x |
| users.json | ujson | 21.026 | 21.483 | 44.260 | 58.262 | 0.18x |
| users.json | json | 31.014 | 31.485 | 44.296 | 58.262 | 0.12x |
| flat.json | strata | 0.692 | 0.750 | 0.962 | 57.430 | 1.00x |
| flat.json | orjson | 0.716 | 0.794 | 0.832 | 57.430 | 0.95x |
| flat.json | msgspec | 0.917 | 0.972 | 1.294 | 57.430 | 0.77x |
| flat.json | ujson | 2.489 | 2.880 | 3.311 | 57.430 | 0.26x |
| flat.json | json | 3.199 | 3.446 | 3.736 | 57.430 | 0.22x |
| nested.json | strata | 0.549 | 0.574 | 0.609 | 57.340 | 1.00x |
| nested.json | orjson | 0.714 | 0.740 | 0.797 | 57.340 | 0.78x |
| nested.json | msgspec | 0.852 | 0.875 | 0.953 | 57.340 | 0.66x |
| nested.json | ujson | 2.010 | 2.380 | 2.490 | 57.340 | 0.24x |
| nested.json | json | 3.364 | 3.556 | 12.541 | 57.340 | 0.16x |
| wide_arrays.json | strata | 2.894 | 2.987 | 3.078 | 58.406 | 1.00x |
| wide_arrays.json | orjson | 3.607 | 3.708 | 5.055 | 58.406 | 0.81x |
| wide_arrays.json | msgspec | 5.303 | 5.476 | 7.286 | 58.406 | 0.55x |
| wide_arrays.json | ujson | 13.881 | 14.248 | 15.099 | 58.406 | 0.21x |
| wide_arrays.json | json | 24.206 | 24.760 | 31.353 | 58.406 | 0.12x |
| mixed.json | strata | 0.368 | 0.379 | 0.417 | 57.453 | 1.00x |
| mixed.json | orjson | 0.406 | 0.436 | 0.473 | 57.453 | 0.87x |
| mixed.json | msgspec | 0.431 | 0.462 | 0.497 | 57.453 | 0.82x |
| mixed.json | ujson | 0.706 | 0.776 | 0.829 | 57.453 | 0.49x |
| mixed.json | json | 1.022 | 1.136 | 1.203 | 57.453 | 0.33x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.097 | 0.103 | 0.109 | 58.316 | 1.00x |
| users.json $[*].id | jmespath | 0.411 | 0.421 | 0.466 | 58.316 | 0.24x |
| users.json $[*].id | jsonpath-ng | 2.230 | 2.307 | 2.388 | 58.316 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.507 | 0.517 | 0.583 | 58.332 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.472 | 2.553 | 2.623 | 58.332 | 0.20x |
| users.json $[*].orders[*].total | jsonpath-ng | 15.298 | 15.812 | 17.063 | 58.332 | 0.03x |
| users.json $..total | strata | 1.895 | 1.925 | 2.272 | 58.332 | 1.00x |
| users.json $..total | jsonpath-ng | 288.017 | 289.706 | 299.154 | 58.332 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.203 | 4.283 | 4.330 | 58.332 | 1.00x |
| users.json $[*].id | orjson+jmespath | 16.750 | 17.304 | 18.486 | 58.332 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 18.129 | 18.410 | 19.050 | 58.332 | 0.23x |
| users.json $[*].orders[*].total | strata | 4.367 | 4.460 | 4.658 | 58.332 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 20.068 | 20.534 | 21.313 | 58.332 | 0.22x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 36.734 | 37.380 | 46.285 | 58.332 | 0.12x |
| users.json $..total | strata | 14.051 | 15.826 | 16.942 | 58.332 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 316.253 | 322.831 | 331.188 | 58.332 | 0.05x |

