# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: cd9d20b6ab3ec573716c10b12fe76ae4b70a707c
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
| users.json | strata | 7.371 | 7.591 | 10.627 | 48.914 | 1.00x |
| users.json | orjson | 11.088 | 11.506 | 13.495 | 48.914 | 0.66x |
| users.json | msgspec | 10.179 | 10.396 | 12.529 | 48.914 | 0.73x |
| users.json | ujson | 15.451 | 15.725 | 18.936 | 48.914 | 0.48x |
| users.json | json | 17.115 | 17.383 | 19.118 | 48.914 | 0.44x |
| flat.json | strata | 0.849 | 0.879 | 0.903 | 57.168 | 1.00x |
| flat.json | orjson | 0.940 | 0.959 | 1.012 | 57.168 | 0.92x |
| flat.json | msgspec | 0.908 | 0.943 | 1.432 | 57.168 | 0.93x |
| flat.json | ujson | 1.408 | 1.482 | 1.938 | 57.168 | 0.59x |
| flat.json | json | 1.468 | 1.512 | 1.534 | 57.168 | 0.58x |
| nested.json | strata | 0.590 | 0.599 | 0.657 | 56.949 | 1.00x |
| nested.json | orjson | 0.890 | 0.898 | 0.972 | 56.949 | 0.67x |
| nested.json | msgspec | 0.745 | 0.757 | 0.791 | 56.949 | 0.79x |
| nested.json | ujson | 1.161 | 1.207 | 1.425 | 56.949 | 0.50x |
| nested.json | json | 1.604 | 1.617 | 1.634 | 56.949 | 0.37x |
| wide_arrays.json | strata | 3.506 | 3.622 | 4.797 | 59.160 | 1.00x |
| wide_arrays.json | orjson | 4.710 | 5.053 | 6.990 | 59.160 | 0.72x |
| wide_arrays.json | msgspec | 4.608 | 4.749 | 5.047 | 59.160 | 0.76x |
| wide_arrays.json | ujson | 6.307 | 6.494 | 7.004 | 59.160 | 0.56x |
| wide_arrays.json | json | 8.846 | 9.418 | 10.623 | 59.160 | 0.38x |
| mixed.json | strata | 0.145 | 0.149 | 0.164 | 57.020 | 1.00x |
| mixed.json | orjson | 0.167 | 0.171 | 0.200 | 57.020 | 0.87x |
| mixed.json | msgspec | 0.183 | 0.185 | 0.201 | 57.020 | 0.80x |
| mixed.json | ujson | 0.258 | 0.268 | 0.290 | 57.020 | 0.55x |
| mixed.json | json | 0.358 | 0.364 | 0.397 | 57.020 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.428 | 2.460 | 2.669 | 47.992 | 1.00x |
| users.json | orjson | 2.563 | 2.753 | 3.920 | 47.992 | 0.89x |
| users.json | msgspec | 4.161 | 4.479 | 4.685 | 47.992 | 0.55x |
| users.json | ujson | 9.974 | 10.109 | 12.425 | 47.992 | 0.24x |
| users.json | json | 17.851 | 18.109 | 20.697 | 47.992 | 0.14x |
| flat.json | strata | 0.250 | 0.266 | 0.430 | 57.941 | 1.00x |
| flat.json | orjson | 0.264 | 0.270 | 0.373 | 57.941 | 0.99x |
| flat.json | msgspec | 0.423 | 0.433 | 0.584 | 57.941 | 0.61x |
| flat.json | ujson | 1.116 | 1.160 | 1.582 | 57.941 | 0.23x |
| flat.json | json | 1.544 | 1.617 | 2.827 | 57.941 | 0.16x |
| nested.json | strata | 0.192 | 0.204 | 0.234 | 57.512 | 1.00x |
| nested.json | orjson | 0.236 | 0.241 | 0.278 | 57.512 | 0.85x |
| nested.json | msgspec | 0.402 | 0.415 | 0.459 | 57.512 | 0.49x |
| nested.json | ujson | 0.781 | 0.788 | 0.813 | 57.512 | 0.26x |
| nested.json | json | 2.035 | 2.075 | 2.141 | 57.512 | 0.10x |
| wide_arrays.json | strata | 1.700 | 1.719 | 1.874 | 58.719 | 1.00x |
| wide_arrays.json | orjson | 2.099 | 2.123 | 2.192 | 58.719 | 0.81x |
| wide_arrays.json | msgspec | 3.402 | 3.622 | 3.886 | 58.719 | 0.47x |
| wide_arrays.json | ujson | 6.160 | 6.244 | 6.421 | 58.719 | 0.28x |
| wide_arrays.json | json | 14.283 | 14.518 | 15.243 | 58.719 | 0.12x |
| mixed.json | strata | 0.055 | 0.057 | 0.089 | 57.176 | 1.00x |
| mixed.json | orjson | 0.052 | 0.054 | 0.076 | 57.176 | 1.07x |
| mixed.json | msgspec | 0.079 | 0.081 | 0.117 | 57.176 | 0.71x |
| mixed.json | ujson | 0.204 | 0.205 | 0.332 | 57.176 | 0.28x |
| mixed.json | json | 0.416 | 0.420 | 0.714 | 57.176 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.195 | 9.650 | 15.136 | 58.023 | 1.00x |
| users.json | orjson | 12.741 | 13.446 | 17.874 | 58.023 | 0.72x |
| users.json | msgspec | 11.540 | 12.918 | 17.690 | 58.023 | 0.75x |
| users.json | ujson | 19.743 | 20.911 | 27.607 | 58.023 | 0.46x |
| users.json | json | 18.553 | 20.310 | 29.564 | 58.023 | 0.48x |
| flat.json | strata | 0.935 | 1.020 | 1.052 | 57.117 | 1.00x |
| flat.json | orjson | 1.096 | 1.120 | 1.189 | 57.117 | 0.91x |
| flat.json | msgspec | 0.999 | 1.067 | 1.188 | 57.117 | 0.96x |
| flat.json | ujson | 1.778 | 1.855 | 1.962 | 57.117 | 0.55x |
| flat.json | json | 1.615 | 1.647 | 1.701 | 57.117 | 0.62x |
| nested.json | strata | 0.677 | 0.724 | 1.120 | 57.262 | 1.00x |
| nested.json | orjson | 0.999 | 1.058 | 1.339 | 57.262 | 0.68x |
| nested.json | msgspec | 0.888 | 0.935 | 1.042 | 57.262 | 0.77x |
| nested.json | ujson | 1.522 | 1.601 | 2.550 | 57.262 | 0.45x |
| nested.json | json | 1.723 | 1.782 | 2.840 | 57.262 | 0.41x |
| wide_arrays.json | strata | 3.887 | 4.034 | 5.724 | 58.719 | 1.00x |
| wide_arrays.json | orjson | 5.037 | 5.102 | 7.542 | 58.719 | 0.79x |
| wide_arrays.json | msgspec | 4.967 | 5.084 | 6.597 | 58.719 | 0.79x |
| wide_arrays.json | ujson | 8.129 | 8.215 | 10.178 | 58.719 | 0.49x |
| wide_arrays.json | json | 9.078 | 9.322 | 17.549 | 58.719 | 0.43x |
| mixed.json | strata | 0.213 | 0.225 | 0.251 | 57.176 | 1.00x |
| mixed.json | orjson | 0.271 | 0.277 | 0.305 | 57.176 | 0.81x |
| mixed.json | msgspec | 0.278 | 0.292 | 0.355 | 57.176 | 0.77x |
| mixed.json | ujson | 0.404 | 0.409 | 0.430 | 57.176 | 0.55x |
| mixed.json | json | 0.460 | 0.473 | 0.500 | 57.176 | 0.48x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.148 | 10.522 | 13.622 | 58.363 | 1.00x |
| users.ndjson | orjson | 14.534 | 15.848 | 18.330 | 58.363 | 0.66x |
| users.ndjson | msgspec | 14.959 | 15.313 | 17.308 | 58.363 | 0.69x |
| users.ndjson | ujson | 20.700 | 21.970 | 30.664 | 58.363 | 0.48x |
| users.ndjson | json | 24.373 | 25.589 | 29.215 | 58.363 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.303 | 3.441 | 4.513 | 59.164 | 1.00x |
| users.json | orjson | 3.442 | 3.821 | 4.805 | 59.164 | 0.90x |
| users.json | msgspec | 5.046 | 5.517 | 6.399 | 59.164 | 0.62x |
| users.json | ujson | 17.631 | 18.229 | 18.532 | 59.164 | 0.19x |
| users.json | json | 25.459 | 26.057 | 34.060 | 59.164 | 0.13x |
| flat.json | strata | 0.507 | 0.533 | 0.655 | 57.805 | 1.00x |
| flat.json | orjson | 0.552 | 0.570 | 0.765 | 57.805 | 0.94x |
| flat.json | msgspec | 0.697 | 0.730 | 0.920 | 57.805 | 0.73x |
| flat.json | ujson | 2.185 | 2.225 | 2.320 | 57.805 | 0.24x |
| flat.json | json | 2.613 | 2.649 | 4.333 | 57.805 | 0.20x |
| nested.json | strata | 0.467 | 0.505 | 0.634 | 57.395 | 1.00x |
| nested.json | orjson | 0.532 | 0.570 | 0.590 | 57.395 | 0.89x |
| nested.json | msgspec | 0.696 | 0.732 | 0.780 | 57.395 | 0.69x |
| nested.json | ujson | 1.595 | 1.677 | 1.795 | 57.395 | 0.30x |
| nested.json | json | 2.854 | 2.915 | 6.715 | 57.395 | 0.17x |
| wide_arrays.json | strata | 2.274 | 2.348 | 2.547 | 58.719 | 1.00x |
| wide_arrays.json | orjson | 2.669 | 2.759 | 2.992 | 58.719 | 0.85x |
| wide_arrays.json | msgspec | 4.114 | 4.253 | 6.535 | 58.719 | 0.55x |
| wide_arrays.json | ujson | 11.425 | 11.658 | 12.732 | 58.719 | 0.20x |
| wide_arrays.json | json | 19.618 | 19.961 | 28.426 | 58.719 | 0.12x |
| mixed.json | strata | 0.277 | 0.298 | 0.370 | 57.203 | 1.00x |
| mixed.json | orjson | 0.301 | 0.333 | 0.368 | 57.203 | 0.89x |
| mixed.json | msgspec | 0.324 | 0.336 | 0.385 | 57.203 | 0.89x |
| mixed.json | ujson | 0.583 | 0.605 | 0.693 | 57.203 | 0.49x |
| mixed.json | json | 0.799 | 0.814 | 0.878 | 57.203 | 0.37x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.076 | 0.089 | 0.098 | 59.219 | 1.00x |
| users.json $[*].id | jmespath | 0.335 | 0.357 | 0.375 | 59.219 | 0.25x |
| users.json $[*].id | jsonpath-ng | 2.038 | 2.208 | 2.343 | 59.219 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.419 | 0.460 | 0.723 | 59.234 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.123 | 2.171 | 4.351 | 59.234 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 14.799 | 15.368 | 19.253 | 59.234 | 0.03x |
| users.json $..total | strata | 1.489 | 1.536 | 2.064 | 59.234 | 1.00x |
| users.json $..total | jsonpath-ng | 250.487 | 253.778 | 259.137 | 59.234 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.486 | 3.530 | 3.592 | 59.234 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.547 | 14.073 | 17.283 | 59.234 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 15.176 | 15.934 | 17.106 | 59.234 | 0.22x |
| users.json $[*].orders[*].total | strata | 3.668 | 3.719 | 3.870 | 59.234 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 16.162 | 16.627 | 17.443 | 59.234 | 0.22x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 30.651 | 31.644 | 36.413 | 59.234 | 0.12x |
| users.json $..total | strata | 11.754 | 15.137 | 17.149 | 59.234 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 275.473 | 281.218 | 283.312 | 59.234 | 0.05x |

