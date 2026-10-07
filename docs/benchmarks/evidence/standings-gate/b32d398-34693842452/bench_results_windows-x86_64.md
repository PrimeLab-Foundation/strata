# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: b32d39824b8fea15839872cab8834a1b0fef1294
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD
- compiler_flags: /nologo /O2 /W3 /DNDEBUG /MD /EHsc /std:c++20 /Zc:__cplusplus /arch:AVX2 /clang:-O3 /clang:-fprofile-use=D:\a\strata\strata\build\pgo\strata.profdata /DLL /MANIFEST:EMBED,ID=2 /MANIFESTUAC:NO /EXPORT:PyInit__strata (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.861 | 9.045 | 13.436 | 48.824 | 1.00x |
| users.json | orjson | 13.128 | 13.389 | 16.348 | 48.824 | 0.68x |
| users.json | msgspec | 12.488 | 12.840 | 17.363 | 48.824 | 0.70x |
| users.json | ujson | 20.075 | 20.562 | 26.714 | 48.824 | 0.44x |
| users.json | json | 22.176 | 22.536 | 23.797 | 48.824 | 0.40x |
| flat.json | strata | 0.864 | 0.908 | 0.932 | 58.090 | 1.00x |
| flat.json | orjson | 1.159 | 1.184 | 1.667 | 58.090 | 0.77x |
| flat.json | msgspec | 1.131 | 1.162 | 1.778 | 58.090 | 0.78x |
| flat.json | ujson | 2.164 | 2.236 | 3.306 | 58.090 | 0.41x |
| flat.json | json | 1.975 | 1.990 | 2.054 | 58.090 | 0.46x |
| nested.json | strata | 0.764 | 0.795 | 1.120 | 57.059 | 1.00x |
| nested.json | orjson | 1.065 | 1.089 | 1.116 | 57.059 | 0.73x |
| nested.json | msgspec | 0.995 | 1.030 | 1.050 | 57.059 | 0.77x |
| nested.json | ujson | 1.538 | 1.580 | 1.631 | 57.059 | 0.50x |
| nested.json | json | 2.161 | 2.181 | 2.210 | 57.059 | 0.36x |
| wide_arrays.json | strata | 4.206 | 4.351 | 5.132 | 58.785 | 1.00x |
| wide_arrays.json | orjson | 5.637 | 5.851 | 7.028 | 58.785 | 0.74x |
| wide_arrays.json | msgspec | 5.747 | 5.799 | 7.453 | 58.785 | 0.75x |
| wide_arrays.json | ujson | 8.379 | 8.685 | 9.732 | 58.785 | 0.50x |
| wide_arrays.json | json | 11.674 | 12.065 | 13.670 | 58.785 | 0.36x |
| mixed.json | strata | 0.188 | 0.190 | 0.246 | 56.660 | 1.00x |
| mixed.json | orjson | 0.215 | 0.222 | 0.267 | 56.660 | 0.86x |
| mixed.json | msgspec | 0.238 | 0.248 | 0.300 | 56.660 | 0.77x |
| mixed.json | ujson | 0.352 | 0.369 | 0.396 | 56.660 | 0.52x |
| mixed.json | json | 0.486 | 0.491 | 0.570 | 56.660 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.017 | 3.068 | 3.212 | 48.184 | 1.00x |
| users.json | orjson | 3.458 | 3.566 | 3.751 | 48.184 | 0.86x |
| users.json | msgspec | 4.899 | 5.109 | 7.832 | 48.184 | 0.60x |
| users.json | ujson | 13.857 | 14.048 | 15.150 | 48.184 | 0.22x |
| users.json | json | 24.454 | 24.689 | 25.793 | 48.184 | 0.12x |
| flat.json | strata | 0.314 | 0.318 | 0.360 | 57.578 | 1.00x |
| flat.json | orjson | 0.366 | 0.368 | 0.391 | 57.578 | 0.86x |
| flat.json | msgspec | 0.499 | 0.515 | 0.556 | 57.578 | 0.62x |
| flat.json | ujson | 1.470 | 1.521 | 1.587 | 57.578 | 0.21x |
| flat.json | json | 2.119 | 2.151 | 2.193 | 57.578 | 0.15x |
| nested.json | strata | 0.292 | 0.296 | 0.325 | 57.289 | 1.00x |
| nested.json | orjson | 0.326 | 0.330 | 0.506 | 57.289 | 0.90x |
| nested.json | msgspec | 0.463 | 0.475 | 0.498 | 57.289 | 0.62x |
| nested.json | ujson | 1.143 | 1.223 | 1.281 | 57.289 | 0.24x |
| nested.json | json | 2.515 | 2.573 | 2.635 | 57.289 | 0.12x |
| wide_arrays.json | strata | 2.008 | 2.026 | 2.131 | 58.371 | 1.00x |
| wide_arrays.json | orjson | 2.494 | 2.649 | 2.807 | 58.371 | 0.76x |
| wide_arrays.json | msgspec | 4.015 | 4.180 | 4.458 | 58.371 | 0.48x |
| wide_arrays.json | ujson | 7.667 | 7.730 | 8.150 | 58.371 | 0.26x |
| wide_arrays.json | json | 20.828 | 21.035 | 22.091 | 58.371 | 0.10x |
| mixed.json | strata | 0.074 | 0.079 | 0.110 | 56.781 | 1.00x |
| mixed.json | orjson | 0.071 | 0.073 | 0.113 | 56.781 | 1.07x |
| mixed.json | msgspec | 0.098 | 0.100 | 0.125 | 56.781 | 0.79x |
| mixed.json | ujson | 0.261 | 0.278 | 0.300 | 56.781 | 0.28x |
| mixed.json | json | 0.530 | 0.566 | 0.620 | 56.781 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.594 | 10.862 | 12.201 | 58.223 | 1.00x |
| users.json | orjson | 14.484 | 14.813 | 16.564 | 58.223 | 0.73x |
| users.json | msgspec | 13.611 | 13.987 | 14.958 | 58.223 | 0.78x |
| users.json | ujson | 25.671 | 26.184 | 30.965 | 58.223 | 0.41x |
| users.json | json | 23.498 | 24.042 | 32.208 | 58.223 | 0.45x |
| flat.json | strata | 1.055 | 1.136 | 1.539 | 57.242 | 1.00x |
| flat.json | orjson | 1.317 | 1.506 | 1.629 | 57.242 | 0.75x |
| flat.json | msgspec | 1.283 | 1.364 | 2.356 | 57.242 | 0.83x |
| flat.json | ujson | 2.820 | 2.916 | 4.311 | 57.242 | 0.39x |
| flat.json | json | 2.125 | 2.191 | 3.886 | 57.242 | 0.52x |
| nested.json | strata | 0.865 | 0.896 | 0.923 | 56.820 | 1.00x |
| nested.json | orjson | 1.210 | 1.262 | 1.293 | 56.820 | 0.71x |
| nested.json | msgspec | 1.136 | 1.183 | 1.234 | 56.820 | 0.76x |
| nested.json | ujson | 2.007 | 2.086 | 2.188 | 56.820 | 0.43x |
| nested.json | json | 2.294 | 2.340 | 2.361 | 56.820 | 0.38x |
| wide_arrays.json | strata | 4.804 | 4.954 | 5.230 | 58.371 | 1.00x |
| wide_arrays.json | orjson | 6.312 | 6.413 | 6.615 | 58.371 | 0.77x |
| wide_arrays.json | msgspec | 6.433 | 6.607 | 6.760 | 58.371 | 0.75x |
| wide_arrays.json | ujson | 11.406 | 11.581 | 12.209 | 58.371 | 0.43x |
| wide_arrays.json | json | 12.383 | 12.714 | 13.638 | 58.371 | 0.39x |
| mixed.json | strata | 0.266 | 0.279 | 0.420 | 56.781 | 1.00x |
| mixed.json | orjson | 0.323 | 0.347 | 0.536 | 56.781 | 0.81x |
| mixed.json | msgspec | 0.348 | 0.372 | 0.426 | 56.781 | 0.75x |
| mixed.json | ujson | 0.546 | 0.578 | 0.772 | 56.781 | 0.48x |
| mixed.json | json | 0.599 | 0.629 | 0.992 | 56.781 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 11.399 | 12.982 | 13.972 | 58.199 | 1.00x |
| users.ndjson | orjson | 18.659 | 19.910 | 21.131 | 58.199 | 0.65x |
| users.ndjson | msgspec | 18.679 | 19.936 | 21.857 | 58.199 | 0.65x |
| users.ndjson | ujson | 28.027 | 29.943 | 31.789 | 58.199 | 0.43x |
| users.ndjson | json | 31.737 | 33.807 | 35.006 | 58.199 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.954 | 4.042 | 4.371 | 59.270 | 1.00x |
| users.json | orjson | 4.487 | 4.638 | 6.232 | 59.270 | 0.87x |
| users.json | msgspec | 5.816 | 6.032 | 6.341 | 59.270 | 0.67x |
| users.json | ujson | 23.252 | 23.413 | 23.929 | 59.270 | 0.17x |
| users.json | json | 33.427 | 33.649 | 35.039 | 59.270 | 0.12x |
| flat.json | strata | 0.661 | 0.713 | 1.088 | 57.488 | 1.00x |
| flat.json | orjson | 0.702 | 0.758 | 1.065 | 57.488 | 0.94x |
| flat.json | msgspec | 0.841 | 0.893 | 1.257 | 57.488 | 0.80x |
| flat.json | ujson | 2.800 | 2.845 | 3.183 | 57.488 | 0.25x |
| flat.json | json | 3.419 | 3.466 | 3.858 | 57.488 | 0.21x |
| nested.json | strata | 0.649 | 0.727 | 1.098 | 57.129 | 1.00x |
| nested.json | orjson | 0.710 | 0.730 | 1.323 | 57.129 | 1.00x |
| nested.json | msgspec | 0.827 | 0.916 | 8.471 | 57.129 | 0.79x |
| nested.json | ujson | 2.306 | 2.411 | 2.821 | 57.129 | 0.30x |
| nested.json | json | 3.597 | 3.833 | 5.447 | 57.129 | 0.19x |
| wide_arrays.json | strata | 2.765 | 2.853 | 3.077 | 58.371 | 1.00x |
| wide_arrays.json | orjson | 3.273 | 3.384 | 4.119 | 58.371 | 0.84x |
| wide_arrays.json | msgspec | 4.784 | 4.915 | 5.524 | 58.371 | 0.58x |
| wide_arrays.json | ujson | 14.572 | 14.690 | 21.771 | 58.371 | 0.19x |
| wide_arrays.json | json | 27.681 | 28.056 | 30.369 | 58.371 | 0.10x |
| mixed.json | strata | 0.389 | 0.407 | 0.459 | 56.809 | 1.00x |
| mixed.json | orjson | 0.386 | 0.414 | 0.531 | 56.809 | 0.98x |
| mixed.json | msgspec | 0.410 | 0.418 | 0.518 | 56.809 | 0.97x |
| mixed.json | ujson | 0.749 | 0.788 | 0.815 | 56.809 | 0.52x |
| mixed.json | json | 1.003 | 1.026 | 1.104 | 56.809 | 0.40x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.084 | 0.093 | 0.109 | 59.320 | 1.00x |
| users.json $[*].id | jmespath | 0.446 | 0.453 | 0.839 | 59.320 | 0.20x |
| users.json $[*].id | jsonpath-ng | 2.469 | 2.682 | 4.481 | 59.320 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.481 | 0.503 | 0.548 | 59.344 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.806 | 2.865 | 5.382 | 59.344 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.354 | 17.944 | 19.752 | 59.344 | 0.03x |
| users.json $..total | strata | 1.920 | 1.936 | 2.111 | 59.344 | 1.00x |
| users.json $..total | jsonpath-ng | 324.467 | 332.304 | 349.154 | 59.344 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.077 | 4.153 | 4.447 | 59.344 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.484 | 15.578 | 16.093 | 59.344 | 0.27x |
| users.json $[*].id | orjson+jsonpath-ng | 17.281 | 17.677 | 19.625 | 59.344 | 0.23x |
| users.json $[*].orders[*].total | strata | 4.334 | 4.391 | 4.987 | 59.344 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.242 | 18.503 | 18.693 | 59.344 | 0.24x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 35.536 | 36.417 | 42.319 | 59.344 | 0.12x |
| users.json $..total | strata | 14.736 | 16.763 | 24.024 | 59.344 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 352.443 | 359.138 | 366.794 | 59.344 | 0.05x |

