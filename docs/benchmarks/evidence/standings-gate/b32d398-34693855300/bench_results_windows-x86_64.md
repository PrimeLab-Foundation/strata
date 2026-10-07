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
| users.json | strata | 8.792 | 9.067 | 12.766 | 48.898 | 1.00x |
| users.json | orjson | 13.225 | 13.549 | 16.471 | 48.898 | 0.67x |
| users.json | msgspec | 12.855 | 13.001 | 15.231 | 48.898 | 0.70x |
| users.json | ujson | 20.363 | 21.104 | 24.554 | 48.898 | 0.43x |
| users.json | json | 22.386 | 22.817 | 23.580 | 48.898 | 0.40x |
| flat.json | strata | 0.814 | 0.854 | 0.939 | 57.516 | 1.00x |
| flat.json | orjson | 1.086 | 1.138 | 1.561 | 57.516 | 0.75x |
| flat.json | msgspec | 1.133 | 1.173 | 1.249 | 57.516 | 0.73x |
| flat.json | ujson | 2.076 | 2.141 | 2.401 | 57.516 | 0.40x |
| flat.json | json | 2.011 | 2.084 | 3.293 | 57.516 | 0.41x |
| nested.json | strata | 0.749 | 0.790 | 0.841 | 56.926 | 1.00x |
| nested.json | orjson | 1.065 | 1.126 | 1.170 | 56.926 | 0.70x |
| nested.json | msgspec | 1.013 | 1.080 | 1.125 | 56.926 | 0.73x |
| nested.json | ujson | 1.580 | 1.601 | 1.786 | 56.926 | 0.49x |
| nested.json | json | 2.133 | 2.180 | 2.268 | 56.926 | 0.36x |
| wide_arrays.json | strata | 4.179 | 4.239 | 5.321 | 58.895 | 1.00x |
| wide_arrays.json | orjson | 5.598 | 5.683 | 5.896 | 58.895 | 0.75x |
| wide_arrays.json | msgspec | 5.716 | 5.811 | 5.927 | 58.895 | 0.73x |
| wide_arrays.json | ujson | 8.059 | 8.266 | 8.894 | 58.895 | 0.51x |
| wide_arrays.json | json | 11.534 | 11.833 | 12.399 | 58.895 | 0.36x |
| mixed.json | strata | 0.180 | 0.185 | 0.318 | 56.750 | 1.00x |
| mixed.json | orjson | 0.210 | 0.216 | 0.252 | 56.750 | 0.85x |
| mixed.json | msgspec | 0.235 | 0.241 | 0.323 | 56.750 | 0.77x |
| mixed.json | ujson | 0.345 | 0.377 | 0.599 | 56.750 | 0.49x |
| mixed.json | json | 0.465 | 0.491 | 0.523 | 56.750 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.055 | 3.105 | 3.269 | 48.301 | 1.00x |
| users.json | orjson | 3.589 | 3.866 | 4.009 | 48.301 | 0.80x |
| users.json | msgspec | 4.941 | 5.223 | 5.590 | 48.301 | 0.59x |
| users.json | ujson | 13.966 | 14.254 | 14.535 | 48.301 | 0.22x |
| users.json | json | 23.260 | 23.692 | 25.470 | 48.301 | 0.13x |
| flat.json | strata | 0.308 | 0.329 | 0.347 | 57.613 | 1.00x |
| flat.json | orjson | 0.352 | 0.356 | 0.394 | 57.613 | 0.92x |
| flat.json | msgspec | 0.490 | 0.519 | 0.596 | 57.613 | 0.63x |
| flat.json | ujson | 1.437 | 1.545 | 1.578 | 57.613 | 0.21x |
| flat.json | json | 2.058 | 2.081 | 2.142 | 57.613 | 0.16x |
| nested.json | strata | 0.290 | 0.304 | 0.459 | 57.523 | 1.00x |
| nested.json | orjson | 0.326 | 0.346 | 0.428 | 57.523 | 0.88x |
| nested.json | msgspec | 0.466 | 0.480 | 0.643 | 57.523 | 0.63x |
| nested.json | ujson | 1.201 | 1.264 | 2.223 | 57.523 | 0.24x |
| nested.json | json | 2.434 | 2.531 | 2.833 | 57.523 | 0.12x |
| wide_arrays.json | strata | 1.993 | 2.041 | 2.953 | 58.449 | 1.00x |
| wide_arrays.json | orjson | 2.553 | 2.606 | 3.658 | 58.449 | 0.78x |
| wide_arrays.json | msgspec | 4.053 | 4.120 | 5.798 | 58.449 | 0.50x |
| wide_arrays.json | ujson | 7.765 | 7.846 | 11.377 | 58.449 | 0.26x |
| wide_arrays.json | json | 19.327 | 19.598 | 21.927 | 58.449 | 0.10x |
| mixed.json | strata | 0.072 | 0.073 | 0.081 | 56.844 | 1.00x |
| mixed.json | orjson | 0.068 | 0.070 | 0.076 | 56.844 | 1.05x |
| mixed.json | msgspec | 0.092 | 0.094 | 0.119 | 56.844 | 0.78x |
| mixed.json | ujson | 0.262 | 0.269 | 0.304 | 56.844 | 0.27x |
| mixed.json | json | 0.510 | 0.519 | 0.704 | 56.844 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.238 | 10.803 | 13.213 | 58.328 | 1.00x |
| users.json | orjson | 14.123 | 14.661 | 15.795 | 58.328 | 0.74x |
| users.json | msgspec | 13.815 | 14.312 | 22.819 | 58.328 | 0.75x |
| users.json | ujson | 25.472 | 26.296 | 28.001 | 58.328 | 0.41x |
| users.json | json | 23.082 | 23.999 | 24.641 | 58.328 | 0.45x |
| flat.json | strata | 1.034 | 1.108 | 1.580 | 57.223 | 1.00x |
| flat.json | orjson | 1.190 | 1.321 | 1.942 | 57.223 | 0.84x |
| flat.json | msgspec | 1.284 | 1.359 | 1.979 | 57.223 | 0.82x |
| flat.json | ujson | 2.646 | 2.764 | 3.333 | 57.223 | 0.40x |
| flat.json | json | 2.121 | 2.198 | 3.058 | 57.223 | 0.50x |
| nested.json | strata | 0.831 | 0.848 | 0.915 | 57.523 | 1.00x |
| nested.json | orjson | 1.150 | 1.208 | 1.388 | 57.523 | 0.70x |
| nested.json | msgspec | 1.133 | 1.195 | 1.313 | 57.523 | 0.71x |
| nested.json | ujson | 1.961 | 2.021 | 2.125 | 57.523 | 0.42x |
| nested.json | json | 2.272 | 2.295 | 2.470 | 57.523 | 0.37x |
| wide_arrays.json | strata | 4.614 | 4.675 | 5.000 | 58.449 | 1.00x |
| wide_arrays.json | orjson | 6.085 | 6.167 | 6.307 | 58.449 | 0.76x |
| wide_arrays.json | msgspec | 6.196 | 6.324 | 6.713 | 58.449 | 0.74x |
| wide_arrays.json | ujson | 11.210 | 11.300 | 12.943 | 58.449 | 0.41x |
| wide_arrays.json | json | 11.999 | 12.166 | 18.832 | 58.449 | 0.38x |
| mixed.json | strata | 0.260 | 0.289 | 0.329 | 56.844 | 1.00x |
| mixed.json | orjson | 0.322 | 0.355 | 0.401 | 56.844 | 0.81x |
| mixed.json | msgspec | 0.349 | 0.381 | 0.422 | 56.844 | 0.76x |
| mixed.json | ujson | 0.523 | 0.542 | 0.621 | 56.844 | 0.53x |
| mixed.json | json | 0.582 | 0.625 | 0.707 | 56.844 | 0.46x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.991 | 10.436 | 11.402 | 57.992 | 1.00x |
| users.ndjson | orjson | 16.684 | 17.166 | 22.453 | 57.992 | 0.61x |
| users.ndjson | msgspec | 17.235 | 17.913 | 18.440 | 57.992 | 0.58x |
| users.ndjson | ujson | 24.897 | 25.919 | 34.426 | 57.992 | 0.40x |
| users.ndjson | json | 29.515 | 30.317 | 40.390 | 57.992 | 0.34x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.878 | 3.995 | 4.024 | 59.344 | 1.00x |
| users.json | orjson | 4.441 | 4.592 | 4.701 | 59.344 | 0.87x |
| users.json | msgspec | 5.761 | 5.972 | 6.959 | 59.344 | 0.67x |
| users.json | ujson | 23.297 | 23.721 | 28.975 | 59.344 | 0.17x |
| users.json | json | 31.993 | 32.694 | 35.172 | 59.344 | 0.12x |
| flat.json | strata | 0.655 | 0.696 | 0.786 | 57.723 | 1.00x |
| flat.json | orjson | 0.708 | 0.755 | 0.813 | 57.723 | 0.92x |
| flat.json | msgspec | 0.843 | 0.896 | 2.865 | 57.723 | 0.78x |
| flat.json | ujson | 2.863 | 2.937 | 3.107 | 57.723 | 0.24x |
| flat.json | json | 3.454 | 3.517 | 5.620 | 57.723 | 0.20x |
| nested.json | strata | 0.629 | 0.697 | 0.788 | 57.648 | 1.00x |
| nested.json | orjson | 0.674 | 0.699 | 0.727 | 57.648 | 1.00x |
| nested.json | msgspec | 0.798 | 0.867 | 0.906 | 57.648 | 0.80x |
| nested.json | ujson | 2.289 | 2.324 | 2.493 | 57.648 | 0.30x |
| nested.json | json | 3.475 | 3.539 | 3.879 | 57.648 | 0.20x |
| wide_arrays.json | strata | 2.656 | 2.735 | 3.021 | 58.449 | 1.00x |
| wide_arrays.json | orjson | 3.218 | 3.322 | 3.471 | 58.449 | 0.82x |
| wide_arrays.json | msgspec | 4.688 | 4.745 | 5.002 | 58.449 | 0.58x |
| wide_arrays.json | ujson | 14.499 | 14.795 | 15.794 | 58.449 | 0.18x |
| wide_arrays.json | json | 25.973 | 26.727 | 28.070 | 58.449 | 0.10x |
| mixed.json | strata | 0.398 | 0.451 | 0.505 | 56.871 | 1.00x |
| mixed.json | orjson | 0.381 | 0.386 | 0.452 | 56.871 | 1.17x |
| mixed.json | msgspec | 0.408 | 0.423 | 0.485 | 56.871 | 1.07x |
| mixed.json | ujson | 0.753 | 0.801 | 1.056 | 56.871 | 0.56x |
| mixed.json | json | 0.999 | 1.045 | 1.137 | 56.871 | 0.43x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.086 | 0.090 | 0.093 | 59.395 | 1.00x |
| users.json $[*].id | jmespath | 0.433 | 0.462 | 0.493 | 59.395 | 0.20x |
| users.json $[*].id | jsonpath-ng | 2.485 | 2.629 | 2.857 | 59.395 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.463 | 0.507 | 0.564 | 59.430 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.744 | 2.861 | 3.164 | 59.430 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.511 | 18.108 | 19.665 | 59.430 | 0.03x |
| users.json $..total | strata | 1.865 | 1.921 | 2.004 | 59.430 | 1.00x |
| users.json $..total | jsonpath-ng | 327.758 | 331.927 | 346.129 | 59.430 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.034 | 4.132 | 4.271 | 59.430 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.377 | 15.714 | 16.903 | 59.430 | 0.26x |
| users.json $[*].id | orjson+jsonpath-ng | 17.039 | 17.453 | 18.535 | 59.430 | 0.24x |
| users.json $[*].orders[*].total | strata | 4.267 | 4.354 | 4.536 | 59.430 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.151 | 18.531 | 19.019 | 59.430 | 0.23x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 34.872 | 36.209 | 42.887 | 59.430 | 0.12x |
| users.json $..total | strata | 13.699 | 14.779 | 16.354 | 59.430 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 354.108 | 358.254 | 379.545 | 59.430 | 0.04x |

