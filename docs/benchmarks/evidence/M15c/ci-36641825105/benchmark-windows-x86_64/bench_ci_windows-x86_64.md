# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 271a2a0864aa430c4690ca9b609e4201e13cfb51
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
| users.json | strata | 7.474 | 7.584 | 10.360 | 49.031 | 1.00x |
| users.json | orjson | 10.914 | 11.085 | 13.223 | 49.031 | 0.68x |
| users.json | msgspec | 10.062 | 10.176 | 13.781 | 49.031 | 0.75x |
| users.json | ujson | 15.338 | 16.084 | 23.639 | 49.031 | 0.47x |
| users.json | json | 16.828 | 17.111 | 20.298 | 49.031 | 0.44x |
| flat.json | strata | 0.805 | 0.830 | 0.843 | 57.398 | 1.00x |
| flat.json | orjson | 0.928 | 0.950 | 0.995 | 57.398 | 0.87x |
| flat.json | msgspec | 0.884 | 0.909 | 0.931 | 57.398 | 0.91x |
| flat.json | ujson | 1.366 | 1.395 | 1.429 | 57.398 | 0.60x |
| flat.json | json | 1.479 | 1.507 | 1.525 | 57.398 | 0.55x |
| nested.json | strata | 0.585 | 0.597 | 0.633 | 57.312 | 1.00x |
| nested.json | orjson | 0.840 | 0.858 | 0.996 | 57.312 | 0.70x |
| nested.json | msgspec | 0.723 | 0.759 | 0.768 | 57.312 | 0.79x |
| nested.json | ujson | 1.164 | 1.177 | 1.227 | 57.312 | 0.51x |
| nested.json | json | 1.585 | 1.612 | 1.748 | 57.312 | 0.37x |
| wide_arrays.json | strata | 3.489 | 3.557 | 3.665 | 59.277 | 1.00x |
| wide_arrays.json | orjson | 4.642 | 4.703 | 4.857 | 59.277 | 0.76x |
| wide_arrays.json | msgspec | 4.539 | 4.601 | 4.666 | 59.277 | 0.77x |
| wide_arrays.json | ujson | 6.195 | 6.312 | 6.616 | 59.277 | 0.56x |
| wide_arrays.json | json | 8.801 | 8.896 | 9.002 | 59.277 | 0.40x |
| mixed.json | strata | 0.144 | 0.149 | 0.262 | 57.125 | 1.00x |
| mixed.json | orjson | 0.167 | 0.172 | 0.270 | 57.125 | 0.87x |
| mixed.json | msgspec | 0.179 | 0.185 | 0.322 | 57.125 | 0.81x |
| mixed.json | ujson | 0.255 | 0.264 | 0.413 | 57.125 | 0.56x |
| mixed.json | json | 0.355 | 0.365 | 0.610 | 57.125 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.379 | 2.403 | 2.456 | 48.621 | 1.00x |
| users.json | orjson | 2.568 | 2.600 | 3.734 | 48.621 | 0.92x |
| users.json | msgspec | 4.043 | 4.378 | 6.911 | 48.621 | 0.55x |
| users.json | ujson | 10.173 | 10.348 | 10.548 | 48.621 | 0.23x |
| users.json | json | 18.149 | 18.347 | 18.712 | 48.621 | 0.13x |
| flat.json | strata | 0.253 | 0.260 | 0.376 | 58.086 | 1.00x |
| flat.json | orjson | 0.255 | 0.263 | 0.366 | 58.086 | 0.99x |
| flat.json | msgspec | 0.412 | 0.422 | 0.448 | 58.086 | 0.62x |
| flat.json | ujson | 1.085 | 1.155 | 1.163 | 58.086 | 0.23x |
| flat.json | json | 1.524 | 1.535 | 1.584 | 58.086 | 0.17x |
| nested.json | strata | 0.189 | 0.194 | 0.215 | 57.918 | 1.00x |
| nested.json | orjson | 0.234 | 0.238 | 0.277 | 57.918 | 0.81x |
| nested.json | msgspec | 0.379 | 0.383 | 0.409 | 57.918 | 0.51x |
| nested.json | ujson | 0.781 | 0.792 | 0.814 | 57.918 | 0.24x |
| nested.json | json | 1.893 | 1.911 | 1.962 | 57.918 | 0.10x |
| wide_arrays.json | strata | 1.606 | 1.650 | 1.858 | 58.988 | 1.00x |
| wide_arrays.json | orjson | 1.963 | 1.998 | 2.026 | 58.988 | 0.83x |
| wide_arrays.json | msgspec | 3.320 | 3.391 | 3.440 | 58.988 | 0.49x |
| wide_arrays.json | ujson | 5.940 | 6.046 | 6.117 | 58.988 | 0.27x |
| wide_arrays.json | json | 14.111 | 14.204 | 14.276 | 58.988 | 0.12x |
| mixed.json | strata | 0.055 | 0.055 | 0.059 | 57.301 | 1.00x |
| mixed.json | orjson | 0.052 | 0.052 | 0.055 | 57.301 | 1.05x |
| mixed.json | msgspec | 0.075 | 0.077 | 0.094 | 57.301 | 0.71x |
| mixed.json | ujson | 0.201 | 0.204 | 0.235 | 57.301 | 0.27x |
| mixed.json | json | 0.398 | 0.410 | 0.443 | 57.301 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.540 | 8.893 | 9.847 | 58.645 | 1.00x |
| users.json | orjson | 11.926 | 12.403 | 12.815 | 58.645 | 0.72x |
| users.json | msgspec | 10.969 | 11.295 | 11.737 | 58.645 | 0.79x |
| users.json | ujson | 18.831 | 19.415 | 21.961 | 58.645 | 0.46x |
| users.json | json | 17.787 | 18.409 | 18.854 | 58.645 | 0.48x |
| flat.json | strata | 0.885 | 0.927 | 1.060 | 57.488 | 1.00x |
| flat.json | orjson | 1.039 | 1.075 | 1.260 | 57.488 | 0.86x |
| flat.json | msgspec | 0.998 | 1.022 | 1.148 | 57.488 | 0.91x |
| flat.json | ujson | 1.681 | 1.751 | 2.012 | 57.488 | 0.53x |
| flat.json | json | 1.595 | 1.644 | 1.683 | 57.488 | 0.56x |
| nested.json | strata | 0.661 | 0.683 | 0.750 | 57.320 | 1.00x |
| nested.json | orjson | 0.959 | 0.995 | 1.059 | 57.320 | 0.69x |
| nested.json | msgspec | 0.832 | 0.842 | 0.870 | 57.320 | 0.81x |
| nested.json | ujson | 1.461 | 1.491 | 1.606 | 57.320 | 0.46x |
| nested.json | json | 1.712 | 1.732 | 1.809 | 57.320 | 0.39x |
| wide_arrays.json | strata | 3.802 | 3.890 | 3.973 | 58.988 | 1.00x |
| wide_arrays.json | orjson | 4.920 | 4.981 | 5.194 | 58.988 | 0.78x |
| wide_arrays.json | msgspec | 4.896 | 4.965 | 5.242 | 58.988 | 0.78x |
| wide_arrays.json | ujson | 8.032 | 8.133 | 8.352 | 58.988 | 0.48x |
| wide_arrays.json | json | 9.116 | 9.161 | 9.345 | 58.988 | 0.42x |
| mixed.json | strata | 0.214 | 0.215 | 0.238 | 57.301 | 1.00x |
| mixed.json | orjson | 0.266 | 0.268 | 0.281 | 57.301 | 0.80x |
| mixed.json | msgspec | 0.279 | 0.282 | 0.313 | 57.301 | 0.76x |
| mixed.json | ujson | 0.395 | 0.398 | 0.442 | 57.301 | 0.54x |
| mixed.json | json | 0.455 | 0.458 | 0.496 | 57.301 | 0.47x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 8.619 | 8.753 | 9.015 | 58.422 | 1.00x |
| users.ndjson | orjson | 14.181 | 14.441 | 19.751 | 58.422 | 0.61x |
| users.ndjson | msgspec | 14.037 | 14.390 | 14.807 | 58.422 | 0.61x |
| users.ndjson | ujson | 19.788 | 20.360 | 23.407 | 58.422 | 0.43x |
| users.ndjson | json | 23.651 | 24.000 | 24.847 | 58.422 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.115 | 3.141 | 3.234 | 59.090 | 1.00x |
| users.json | orjson | 3.152 | 3.221 | 3.290 | 59.090 | 0.98x |
| users.json | msgspec | 4.821 | 4.916 | 16.213 | 59.090 | 0.64x |
| users.json | ujson | 17.007 | 17.181 | 17.362 | 59.090 | 0.18x |
| users.json | json | 25.090 | 25.302 | 25.501 | 59.090 | 0.12x |
| flat.json | strata | 0.513 | 0.525 | 0.567 | 58.098 | 1.00x |
| flat.json | orjson | 0.540 | 0.560 | 0.606 | 58.098 | 0.94x |
| flat.json | msgspec | 0.694 | 0.726 | 0.773 | 58.098 | 0.72x |
| flat.json | ujson | 2.182 | 2.212 | 2.244 | 58.098 | 0.24x |
| flat.json | json | 2.589 | 2.608 | 2.642 | 58.098 | 0.20x |
| nested.json | strata | 0.431 | 0.457 | 0.509 | 57.727 | 1.00x |
| nested.json | orjson | 0.515 | 0.555 | 0.593 | 57.727 | 0.82x |
| nested.json | msgspec | 0.654 | 0.688 | 0.744 | 57.727 | 0.66x |
| nested.json | ujson | 1.609 | 1.659 | 1.695 | 57.727 | 0.28x |
| nested.json | json | 2.744 | 2.786 | 2.912 | 57.727 | 0.16x |
| wide_arrays.json | strata | 2.177 | 2.198 | 2.261 | 58.988 | 1.00x |
| wide_arrays.json | orjson | 2.490 | 2.551 | 2.650 | 58.988 | 0.86x |
| wide_arrays.json | msgspec | 3.877 | 3.927 | 3.975 | 58.988 | 0.56x |
| wide_arrays.json | ujson | 11.246 | 11.357 | 11.531 | 58.988 | 0.19x |
| wide_arrays.json | json | 19.333 | 19.454 | 22.264 | 58.988 | 0.11x |
| mixed.json | strata | 0.272 | 0.281 | 0.339 | 57.301 | 1.00x |
| mixed.json | orjson | 0.290 | 0.301 | 0.322 | 57.301 | 0.93x |
| mixed.json | msgspec | 0.317 | 0.329 | 0.384 | 57.301 | 0.85x |
| mixed.json | ujson | 0.572 | 0.586 | 0.622 | 57.301 | 0.48x |
| mixed.json | json | 0.782 | 0.819 | 6.566 | 57.301 | 0.34x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.070 | 0.074 | 0.103 | 59.191 | 1.00x |
| users.json $[*].id | jmespath | 0.322 | 0.327 | 0.377 | 59.191 | 0.23x |
| users.json $[*].id | jsonpath-ng | 1.831 | 1.909 | 2.025 | 59.191 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.399 | 0.414 | 0.438 | 59.219 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.061 | 2.113 | 2.328 | 59.219 | 0.20x |
| users.json $[*].orders[*].total | jsonpath-ng | 12.874 | 13.449 | 17.549 | 59.219 | 0.03x |
| users.json $..total | strata | 1.468 | 1.519 | 1.561 | 59.219 | 1.00x |
| users.json $..total | jsonpath-ng | 247.686 | 250.358 | 256.208 | 59.219 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.405 | 3.448 | 3.551 | 59.215 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.937 | 13.210 | 17.029 | 59.215 | 0.26x |
| users.json $[*].id | orjson+jsonpath-ng | 14.364 | 14.594 | 15.061 | 59.215 | 0.24x |
| users.json $[*].orders[*].total | strata | 3.603 | 3.666 | 4.132 | 59.219 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.486 | 15.726 | 18.125 | 59.219 | 0.23x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 29.144 | 29.752 | 30.431 | 59.219 | 0.12x |
| users.json $..total | strata | 11.762 | 13.327 | 15.838 | 59.219 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 269.972 | 272.064 | 279.374 | 59.219 | 0.05x |

