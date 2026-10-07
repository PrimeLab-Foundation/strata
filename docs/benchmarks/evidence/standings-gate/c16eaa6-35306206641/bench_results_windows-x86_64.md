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
| users.json | strata | 7.266 | 7.364 | 11.309 | 48.828 | 1.00x |
| users.json | orjson | 10.770 | 11.058 | 15.150 | 48.828 | 0.67x |
| users.json | msgspec | 10.008 | 10.105 | 12.118 | 48.828 | 0.73x |
| users.json | ujson | 15.087 | 15.554 | 19.656 | 48.828 | 0.47x |
| users.json | json | 16.864 | 17.122 | 24.182 | 48.828 | 0.43x |
| flat.json | strata | 0.687 | 0.859 | 0.895 | 56.926 | 1.00x |
| flat.json | orjson | 0.917 | 0.952 | 0.989 | 56.926 | 0.90x |
| flat.json | msgspec | 0.864 | 0.908 | 0.962 | 56.926 | 0.95x |
| flat.json | ujson | 1.338 | 1.434 | 1.489 | 56.926 | 0.60x |
| flat.json | json | 1.494 | 1.524 | 1.550 | 56.926 | 0.56x |
| nested.json | strata | 0.593 | 0.598 | 0.641 | 56.934 | 1.00x |
| nested.json | orjson | 0.849 | 0.856 | 0.898 | 56.934 | 0.70x |
| nested.json | msgspec | 0.751 | 0.774 | 0.798 | 56.934 | 0.77x |
| nested.json | ujson | 1.181 | 1.208 | 1.226 | 56.934 | 0.49x |
| nested.json | json | 1.559 | 1.591 | 1.609 | 56.934 | 0.38x |
| wide_arrays.json | strata | 3.457 | 3.517 | 3.610 | 59.094 | 1.00x |
| wide_arrays.json | orjson | 4.623 | 4.692 | 4.813 | 59.094 | 0.75x |
| wide_arrays.json | msgspec | 4.553 | 4.636 | 5.032 | 59.094 | 0.76x |
| wide_arrays.json | ujson | 6.274 | 6.321 | 6.986 | 59.094 | 0.56x |
| wide_arrays.json | json | 8.827 | 8.915 | 9.250 | 59.094 | 0.39x |
| mixed.json | strata | 0.144 | 0.148 | 0.153 | 59.406 | 1.00x |
| mixed.json | orjson | 0.166 | 0.169 | 0.172 | 59.406 | 0.87x |
| mixed.json | msgspec | 0.175 | 0.178 | 0.188 | 59.406 | 0.83x |
| mixed.json | ujson | 0.256 | 0.259 | 0.266 | 59.406 | 0.57x |
| mixed.json | json | 0.352 | 0.355 | 0.362 | 59.406 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.388 | 2.393 | 2.466 | 48.062 | 1.00x |
| users.json | orjson | 2.545 | 2.661 | 3.682 | 48.062 | 0.90x |
| users.json | msgspec | 4.145 | 4.353 | 4.555 | 48.062 | 0.55x |
| users.json | ujson | 10.140 | 10.358 | 10.437 | 48.062 | 0.23x |
| users.json | json | 17.586 | 17.686 | 18.098 | 48.062 | 0.14x |
| flat.json | strata | 0.252 | 0.257 | 0.281 | 57.414 | 1.00x |
| flat.json | orjson | 0.257 | 0.263 | 0.278 | 57.414 | 0.98x |
| flat.json | msgspec | 0.422 | 0.426 | 0.495 | 57.414 | 0.60x |
| flat.json | ujson | 1.096 | 1.132 | 1.161 | 57.414 | 0.23x |
| flat.json | json | 1.499 | 1.525 | 1.538 | 57.414 | 0.17x |
| nested.json | strata | 0.186 | 0.189 | 0.216 | 57.547 | 1.00x |
| nested.json | orjson | 0.233 | 0.238 | 0.262 | 57.547 | 0.80x |
| nested.json | msgspec | 0.391 | 0.399 | 0.430 | 57.547 | 0.47x |
| nested.json | ujson | 0.781 | 0.794 | 0.831 | 57.547 | 0.24x |
| nested.json | json | 1.907 | 1.922 | 1.941 | 57.547 | 0.10x |
| wide_arrays.json | strata | 1.683 | 1.723 | 1.759 | 60.570 | 1.00x |
| wide_arrays.json | orjson | 1.686 | 1.747 | 2.097 | 60.570 | 0.99x |
| wide_arrays.json | msgspec | 2.953 | 3.012 | 3.371 | 60.570 | 0.57x |
| wide_arrays.json | ujson | 5.992 | 6.019 | 6.066 | 60.570 | 0.29x |
| wide_arrays.json | json | 13.658 | 13.702 | 13.958 | 60.570 | 0.13x |
| mixed.json | strata | 0.055 | 0.056 | 0.060 | 59.445 | 1.00x |
| mixed.json | orjson | 0.052 | 0.053 | 0.055 | 59.445 | 1.05x |
| mixed.json | msgspec | 0.078 | 0.083 | 0.092 | 59.445 | 0.67x |
| mixed.json | ujson | 0.197 | 0.201 | 0.208 | 59.445 | 0.28x |
| mixed.json | json | 0.391 | 0.397 | 0.405 | 59.445 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.422 | 8.907 | 9.572 | 58.098 | 1.00x |
| users.json | orjson | 11.860 | 12.412 | 12.804 | 58.098 | 0.72x |
| users.json | msgspec | 10.870 | 11.404 | 11.766 | 58.098 | 0.78x |
| users.json | ujson | 18.911 | 19.719 | 24.984 | 58.098 | 0.45x |
| users.json | json | 17.868 | 18.384 | 18.632 | 58.098 | 0.48x |
| flat.json | strata | 0.812 | 0.857 | 0.875 | 57.824 | 1.00x |
| flat.json | orjson | 1.210 | 1.244 | 1.357 | 57.824 | 0.69x |
| flat.json | msgspec | 0.988 | 1.030 | 1.449 | 57.824 | 0.83x |
| flat.json | ujson | 1.728 | 1.782 | 2.642 | 57.824 | 0.48x |
| flat.json | json | 1.613 | 1.642 | 1.688 | 57.824 | 0.52x |
| nested.json | strata | 0.674 | 0.700 | 0.829 | 57.137 | 1.00x |
| nested.json | orjson | 0.963 | 1.019 | 1.082 | 57.137 | 0.69x |
| nested.json | msgspec | 0.864 | 0.884 | 0.917 | 57.137 | 0.79x |
| nested.json | ujson | 1.453 | 1.502 | 1.610 | 57.137 | 0.47x |
| nested.json | json | 1.683 | 1.720 | 1.819 | 57.137 | 0.41x |
| wide_arrays.json | strata | 3.899 | 3.949 | 6.145 | 60.570 | 1.00x |
| wide_arrays.json | orjson | 4.941 | 5.037 | 6.167 | 60.570 | 0.78x |
| wide_arrays.json | msgspec | 4.955 | 5.033 | 5.111 | 60.570 | 0.78x |
| wide_arrays.json | ujson | 8.121 | 8.203 | 12.547 | 60.570 | 0.48x |
| wide_arrays.json | json | 9.083 | 9.167 | 9.333 | 60.570 | 0.43x |
| mixed.json | strata | 0.211 | 0.218 | 0.313 | 59.445 | 1.00x |
| mixed.json | orjson | 0.266 | 0.270 | 0.412 | 59.445 | 0.81x |
| mixed.json | msgspec | 0.282 | 0.291 | 0.320 | 59.445 | 0.75x |
| mixed.json | ujson | 0.400 | 0.419 | 0.489 | 59.445 | 0.52x |
| mixed.json | json | 0.458 | 0.466 | 0.501 | 59.445 | 0.47x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 8.579 | 9.367 | 9.829 | 57.879 | 1.00x |
| users.ndjson | orjson | 14.494 | 15.230 | 15.600 | 57.879 | 0.62x |
| users.ndjson | msgspec | 14.365 | 14.899 | 15.528 | 57.879 | 0.63x |
| users.ndjson | ujson | 20.122 | 20.932 | 21.870 | 57.879 | 0.45x |
| users.ndjson | json | 23.717 | 24.796 | 25.314 | 57.879 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.096 | 3.141 | 3.233 | 58.941 | 1.00x |
| users.json | orjson | 3.232 | 3.320 | 3.447 | 58.941 | 0.95x |
| users.json | msgspec | 5.170 | 5.242 | 5.335 | 58.941 | 0.60x |
| users.json | ujson | 17.017 | 17.152 | 17.631 | 58.941 | 0.18x |
| users.json | json | 25.032 | 25.194 | 25.530 | 58.941 | 0.12x |
| flat.json | strata | 0.520 | 0.530 | 0.548 | 57.445 | 1.00x |
| flat.json | orjson | 0.539 | 0.547 | 0.575 | 57.445 | 0.97x |
| flat.json | msgspec | 0.708 | 0.744 | 0.783 | 57.445 | 0.71x |
| flat.json | ujson | 2.216 | 2.246 | 2.324 | 57.445 | 0.24x |
| flat.json | json | 2.582 | 2.597 | 2.618 | 57.445 | 0.20x |
| nested.json | strata | 0.437 | 0.470 | 0.701 | 57.527 | 1.00x |
| nested.json | orjson | 0.517 | 0.557 | 0.741 | 57.527 | 0.84x |
| nested.json | msgspec | 0.674 | 0.717 | 1.070 | 57.527 | 0.66x |
| nested.json | ujson | 1.596 | 1.661 | 2.436 | 57.527 | 0.28x |
| nested.json | json | 2.763 | 2.808 | 4.223 | 57.527 | 0.17x |
| wide_arrays.json | strata | 2.257 | 2.315 | 2.418 | 60.582 | 1.00x |
| wide_arrays.json | orjson | 2.332 | 2.455 | 2.522 | 60.582 | 0.94x |
| wide_arrays.json | msgspec | 3.574 | 3.656 | 3.948 | 60.582 | 0.63x |
| wide_arrays.json | ujson | 11.296 | 11.344 | 11.597 | 60.582 | 0.20x |
| wide_arrays.json | json | 19.057 | 19.113 | 19.301 | 60.582 | 0.12x |
| mixed.json | strata | 0.274 | 0.281 | 0.309 | 59.445 | 1.00x |
| mixed.json | orjson | 0.296 | 0.309 | 11.295 | 59.445 | 0.91x |
| mixed.json | msgspec | 0.322 | 0.339 | 0.391 | 59.445 | 0.83x |
| mixed.json | ujson | 0.579 | 0.591 | 0.653 | 59.445 | 0.48x |
| mixed.json | json | 0.780 | 0.788 | 0.861 | 59.445 | 0.36x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.070 | 0.074 | 0.094 | 57.918 | 1.00x |
| users.json $[*].id | jmespath | 0.321 | 0.327 | 0.343 | 57.918 | 0.23x |
| users.json $[*].id | jsonpath-ng | 1.870 | 1.951 | 2.190 | 57.918 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.412 | 0.427 | 0.686 | 58.145 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.077 | 2.147 | 3.845 | 58.145 | 0.20x |
| users.json $[*].orders[*].total | jsonpath-ng | 13.236 | 14.116 | 14.405 | 58.145 | 0.03x |
| users.json $..total | strata | 1.498 | 1.534 | 1.573 | 58.320 | 1.00x |
| users.json $..total | jsonpath-ng | 250.073 | 253.904 | 258.235 | 58.320 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.428 | 3.528 | 3.667 | 57.938 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.171 | 13.937 | 14.420 | 57.938 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 14.643 | 15.534 | 15.997 | 57.938 | 0.23x |
| users.json $[*].orders[*].total | strata | 3.599 | 3.676 | 3.776 | 58.320 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.579 | 15.824 | 16.279 | 58.320 | 0.23x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 28.835 | 29.563 | 31.341 | 58.320 | 0.12x |
| users.json $..total | strata | 11.587 | 16.225 | 19.625 | 58.320 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 277.059 | 286.198 | 300.782 | 58.320 | 0.06x |

