# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 6e4d93e74610755e4c46406e91c8aaf4ab6817a1
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
| users.json | strata | 9.359 | 9.574 | 12.853 | 48.746 | 1.00x |
| users.json | orjson | 13.699 | 13.990 | 17.752 | 48.746 | 0.68x |
| users.json | msgspec | 12.884 | 13.071 | 15.900 | 48.746 | 0.73x |
| users.json | ujson | 19.441 | 20.033 | 23.232 | 48.746 | 0.48x |
| users.json | json | 22.164 | 22.444 | 23.260 | 48.746 | 0.43x |
| flat.json | strata | 1.080 | 1.112 | 1.561 | 57.039 | 1.00x |
| flat.json | orjson | 1.196 | 1.239 | 1.292 | 57.039 | 0.90x |
| flat.json | msgspec | 1.082 | 1.106 | 1.140 | 57.039 | 1.00x |
| flat.json | ujson | 1.786 | 1.813 | 1.851 | 57.039 | 0.61x |
| flat.json | json | 1.911 | 1.924 | 2.235 | 57.039 | 0.58x |
| nested.json | strata | 0.760 | 0.773 | 0.838 | 56.816 | 1.00x |
| nested.json | orjson | 1.076 | 1.106 | 1.167 | 56.816 | 0.70x |
| nested.json | msgspec | 0.969 | 0.993 | 1.022 | 56.816 | 0.78x |
| nested.json | ujson | 1.492 | 1.528 | 1.552 | 56.816 | 0.51x |
| nested.json | json | 2.077 | 2.090 | 2.133 | 56.816 | 0.37x |
| wide_arrays.json | strata | 4.422 | 4.473 | 5.291 | 58.777 | 1.00x |
| wide_arrays.json | orjson | 5.830 | 5.884 | 6.055 | 58.777 | 0.76x |
| wide_arrays.json | msgspec | 5.866 | 5.935 | 6.303 | 58.777 | 0.75x |
| wide_arrays.json | ujson | 8.012 | 8.043 | 8.180 | 58.777 | 0.56x |
| wide_arrays.json | json | 11.341 | 11.417 | 11.487 | 58.777 | 0.39x |
| mixed.json | strata | 0.183 | 0.192 | 0.296 | 56.648 | 1.00x |
| mixed.json | orjson | 0.215 | 0.222 | 0.351 | 56.648 | 0.87x |
| mixed.json | msgspec | 0.230 | 0.233 | 0.392 | 56.648 | 0.82x |
| mixed.json | ujson | 0.329 | 0.340 | 0.556 | 56.648 | 0.56x |
| mixed.json | json | 0.464 | 0.480 | 0.747 | 56.648 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.975 | 3.023 | 3.047 | 48.219 | 1.00x |
| users.json | orjson | 3.809 | 3.878 | 3.959 | 48.219 | 0.78x |
| users.json | msgspec | 5.705 | 5.793 | 6.633 | 48.219 | 0.52x |
| users.json | ujson | 12.830 | 12.972 | 13.532 | 48.219 | 0.23x |
| users.json | json | 23.078 | 23.284 | 23.511 | 48.219 | 0.13x |
| flat.json | strata | 0.323 | 0.329 | 0.359 | 57.652 | 1.00x |
| flat.json | orjson | 0.376 | 0.382 | 0.413 | 57.652 | 0.86x |
| flat.json | msgspec | 0.544 | 0.548 | 0.579 | 57.652 | 0.60x |
| flat.json | ujson | 1.423 | 1.468 | 1.516 | 57.652 | 0.22x |
| flat.json | json | 2.023 | 2.066 | 2.078 | 57.652 | 0.16x |
| nested.json | strata | 0.241 | 0.246 | 0.264 | 57.152 | 1.00x |
| nested.json | orjson | 0.319 | 0.324 | 0.401 | 57.152 | 0.76x |
| nested.json | msgspec | 0.502 | 0.508 | 0.550 | 57.152 | 0.48x |
| nested.json | ujson | 1.002 | 1.037 | 1.046 | 57.152 | 0.24x |
| nested.json | json | 2.567 | 2.587 | 2.674 | 57.152 | 0.09x |
| wide_arrays.json | strata | 2.119 | 2.158 | 2.384 | 58.102 | 1.00x |
| wide_arrays.json | orjson | 2.647 | 2.726 | 2.792 | 58.102 | 0.79x |
| wide_arrays.json | msgspec | 4.475 | 4.532 | 4.600 | 58.102 | 0.48x |
| wide_arrays.json | ujson | 7.829 | 8.003 | 11.340 | 58.102 | 0.27x |
| wide_arrays.json | json | 18.737 | 18.930 | 20.676 | 58.102 | 0.11x |
| mixed.json | strata | 0.068 | 0.070 | 0.075 | 56.859 | 1.00x |
| mixed.json | orjson | 0.073 | 0.075 | 0.094 | 56.859 | 0.94x |
| mixed.json | msgspec | 0.099 | 0.101 | 0.135 | 56.859 | 0.70x |
| mixed.json | ujson | 0.254 | 0.258 | 0.295 | 56.859 | 0.27x |
| mixed.json | json | 0.540 | 0.553 | 0.579 | 56.859 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.642 | 10.951 | 12.513 | 58.250 | 1.00x |
| users.json | orjson | 14.644 | 15.142 | 18.895 | 58.250 | 0.72x |
| users.json | msgspec | 13.838 | 14.100 | 14.461 | 58.250 | 0.78x |
| users.json | ujson | 23.398 | 23.911 | 25.423 | 58.250 | 0.46x |
| users.json | json | 22.894 | 23.267 | 23.838 | 58.250 | 0.47x |
| flat.json | strata | 1.044 | 1.121 | 1.342 | 56.988 | 1.00x |
| flat.json | orjson | 1.515 | 1.548 | 1.634 | 56.988 | 0.72x |
| flat.json | msgspec | 1.230 | 1.285 | 1.450 | 56.988 | 0.87x |
| flat.json | ujson | 2.224 | 2.267 | 2.377 | 56.988 | 0.49x |
| flat.json | json | 2.069 | 2.081 | 2.110 | 56.988 | 0.54x |
| nested.json | strata | 0.855 | 0.880 | 1.338 | 56.824 | 1.00x |
| nested.json | orjson | 1.210 | 1.233 | 1.283 | 56.824 | 0.71x |
| nested.json | msgspec | 1.094 | 1.104 | 1.143 | 56.824 | 0.80x |
| nested.json | ujson | 1.809 | 1.869 | 2.115 | 56.824 | 0.47x |
| nested.json | json | 2.195 | 2.216 | 2.386 | 56.824 | 0.40x |
| wide_arrays.json | strata | 4.851 | 4.895 | 5.044 | 58.102 | 1.00x |
| wide_arrays.json | orjson | 6.203 | 6.229 | 6.352 | 58.102 | 0.79x |
| wide_arrays.json | msgspec | 6.328 | 6.359 | 6.487 | 58.102 | 0.77x |
| wide_arrays.json | ujson | 10.235 | 10.340 | 10.535 | 58.102 | 0.47x |
| wide_arrays.json | json | 11.674 | 11.758 | 12.067 | 58.102 | 0.42x |
| mixed.json | strata | 0.255 | 0.261 | 0.287 | 56.891 | 1.00x |
| mixed.json | orjson | 0.328 | 0.332 | 0.338 | 56.891 | 0.79x |
| mixed.json | msgspec | 0.340 | 0.348 | 0.385 | 56.891 | 0.75x |
| mixed.json | ujson | 0.501 | 0.504 | 0.533 | 56.891 | 0.52x |
| mixed.json | json | 0.580 | 0.624 | 0.642 | 56.891 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.828 | 10.945 | 13.249 | 57.969 | 1.00x |
| users.ndjson | orjson | 18.104 | 18.242 | 18.804 | 57.969 | 0.60x |
| users.ndjson | msgspec | 17.728 | 17.918 | 18.066 | 57.969 | 0.61x |
| users.ndjson | ujson | 24.814 | 25.082 | 25.546 | 57.969 | 0.44x |
| users.ndjson | json | 30.069 | 30.273 | 31.844 | 57.969 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.816 | 3.856 | 5.447 | 57.914 | 1.00x |
| users.json | orjson | 4.646 | 4.761 | 6.543 | 57.914 | 0.81x |
| users.json | msgspec | 6.266 | 6.464 | 7.434 | 57.914 | 0.60x |
| users.json | ujson | 21.586 | 21.882 | 22.301 | 57.914 | 0.18x |
| users.json | json | 32.085 | 32.262 | 33.018 | 57.914 | 0.12x |
| flat.json | strata | 0.647 | 0.671 | 0.723 | 57.402 | 1.00x |
| flat.json | orjson | 0.722 | 0.744 | 0.807 | 57.402 | 0.90x |
| flat.json | msgspec | 0.888 | 0.917 | 1.263 | 57.402 | 0.73x |
| flat.json | ujson | 2.786 | 2.861 | 4.389 | 57.402 | 0.23x |
| flat.json | json | 3.412 | 3.447 | 5.663 | 57.402 | 0.19x |
| nested.json | strata | 0.544 | 0.582 | 0.608 | 57.121 | 1.00x |
| nested.json | orjson | 0.666 | 0.679 | 0.770 | 57.121 | 0.86x |
| nested.json | msgspec | 0.853 | 0.900 | 0.918 | 57.121 | 0.65x |
| nested.json | ujson | 2.070 | 2.093 | 2.124 | 57.121 | 0.28x |
| nested.json | json | 3.639 | 3.674 | 3.718 | 57.121 | 0.16x |
| wide_arrays.json | strata | 2.782 | 2.802 | 13.381 | 58.102 | 1.00x |
| wide_arrays.json | orjson | 3.304 | 3.446 | 3.622 | 58.102 | 0.81x |
| wide_arrays.json | msgspec | 5.129 | 5.242 | 5.304 | 58.102 | 0.53x |
| wide_arrays.json | ujson | 14.460 | 14.703 | 14.820 | 58.102 | 0.19x |
| wide_arrays.json | json | 25.534 | 25.856 | 26.250 | 58.102 | 0.11x |
| mixed.json | strata | 0.343 | 0.352 | 0.413 | 56.922 | 1.00x |
| mixed.json | orjson | 0.388 | 0.405 | 0.441 | 56.922 | 0.87x |
| mixed.json | msgspec | 0.405 | 0.420 | 0.467 | 56.922 | 0.84x |
| mixed.json | ujson | 0.733 | 0.766 | 0.820 | 56.922 | 0.46x |
| mixed.json | json | 1.019 | 1.081 | 1.087 | 56.922 | 0.33x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.093 | 0.094 | 0.098 | 57.926 | 1.00x |
| users.json $[*].id | jmespath | 0.411 | 0.427 | 0.458 | 57.926 | 0.22x |
| users.json $[*].id | jsonpath-ng | 2.324 | 2.488 | 2.527 | 57.926 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.515 | 0.535 | 0.549 | 58.164 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.670 | 2.718 | 2.906 | 58.164 | 0.20x |
| users.json $[*].orders[*].total | jsonpath-ng | 16.621 | 16.852 | 18.554 | 58.164 | 0.03x |
| users.json $..total | strata | 1.933 | 1.955 | 2.004 | 58.184 | 1.00x |
| users.json $..total | jsonpath-ng | 316.923 | 319.420 | 322.126 | 58.184 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.196 | 4.277 | 4.358 | 57.996 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.995 | 16.349 | 18.730 | 57.996 | 0.26x |
| users.json $[*].id | orjson+jsonpath-ng | 17.864 | 17.967 | 18.545 | 57.996 | 0.24x |
| users.json $[*].orders[*].total | strata | 4.463 | 4.512 | 4.662 | 58.180 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.775 | 19.282 | 20.026 | 58.180 | 0.23x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 36.635 | 37.060 | 39.076 | 58.180 | 0.12x |
| users.json $..total | strata | 13.763 | 14.336 | 15.292 | 58.203 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 340.591 | 345.325 | 351.234 | 58.203 | 0.04x |

