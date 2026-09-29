# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 38eaa9f248f509d9a4beb75b267189d011c1109e
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD
- compiler_flags: /nologo /O2 /W3 /DNDEBUG /MD /EHsc /std:c++20 /Zc:__cplusplus /D_USE_STD_VECTOR_ALGORITHMS=0 /arch:AVX2 /clang:-O3 /clang:-fprofile-use=D:\a\_temp\strata-arm\build\pgo\strata.profdata /DLL /MANIFEST:EMBED,ID=2 /MANIFESTUAC:NO /EXPORT:PyInit__strata (19 recorded commands; see the JSON companion)
- repeats: 60
- warmup: 2
- provenance_schema: 1

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.767 | 9.052 | 10.303 | 49.199 | 1.00x |
| users.json | orjson | 13.170 | 13.703 | 16.035 | 49.199 | 0.66x |
| users.json | msgspec | 12.668 | 13.178 | 16.360 | 49.199 | 0.69x |
| users.json | ujson | 20.362 | 22.160 | 26.698 | 49.199 | 0.41x |
| users.json | json | 22.103 | 22.968 | 25.737 | 49.199 | 0.39x |
| flat.json | strata | 0.828 | 0.867 | 0.914 | 58.148 | 1.00x |
| flat.json | orjson | 1.114 | 1.171 | 1.199 | 58.148 | 0.74x |
| flat.json | msgspec | 1.108 | 1.149 | 1.190 | 58.148 | 0.75x |
| flat.json | ujson | 2.164 | 2.248 | 2.352 | 58.148 | 0.39x |
| flat.json | json | 2.023 | 2.040 | 2.098 | 58.148 | 0.42x |
| nested.json | strata | 0.744 | 0.764 | 0.803 | 57.215 | 1.00x |
| nested.json | orjson | 1.068 | 1.120 | 1.157 | 57.215 | 0.68x |
| nested.json | msgspec | 1.004 | 1.041 | 1.070 | 57.215 | 0.73x |
| nested.json | ujson | 1.539 | 1.614 | 1.683 | 57.215 | 0.47x |
| nested.json | json | 2.112 | 2.133 | 2.210 | 57.215 | 0.36x |
| wide_arrays.json | strata | 4.208 | 4.453 | 5.291 | 58.891 | 1.00x |
| wide_arrays.json | orjson | 5.732 | 6.160 | 6.646 | 58.891 | 0.72x |
| wide_arrays.json | msgspec | 5.790 | 6.045 | 6.412 | 58.891 | 0.74x |
| wide_arrays.json | ujson | 8.239 | 8.643 | 9.274 | 58.891 | 0.52x |
| wide_arrays.json | json | 11.640 | 12.202 | 13.152 | 58.891 | 0.36x |
| mixed.json | strata | 0.183 | 0.192 | 0.304 | 55.840 | 1.00x |
| mixed.json | orjson | 0.213 | 0.226 | 0.388 | 55.840 | 0.85x |
| mixed.json | msgspec | 0.232 | 0.247 | 0.326 | 55.840 | 0.78x |
| mixed.json | ujson | 0.356 | 0.395 | 0.593 | 55.840 | 0.49x |
| mixed.json | json | 0.474 | 0.509 | 0.710 | 55.840 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.940 | 3.068 | 3.561 | 48.496 | 1.00x |
| users.json | orjson | 3.691 | 3.844 | 4.008 | 48.496 | 0.80x |
| users.json | msgspec | 4.961 | 5.198 | 5.723 | 48.496 | 0.59x |
| users.json | ujson | 13.768 | 13.984 | 14.537 | 48.496 | 0.22x |
| users.json | json | 22.890 | 23.407 | 24.213 | 48.496 | 0.13x |
| flat.json | strata | 0.290 | 0.295 | 0.321 | 58.152 | 1.00x |
| flat.json | orjson | 0.355 | 0.364 | 0.410 | 58.152 | 0.81x |
| flat.json | msgspec | 0.498 | 0.513 | 0.592 | 58.152 | 0.57x |
| flat.json | ujson | 1.507 | 1.553 | 1.581 | 58.152 | 0.19x |
| flat.json | json | 1.943 | 1.975 | 2.278 | 58.152 | 0.15x |
| nested.json | strata | 0.272 | 0.278 | 0.313 | 57.457 | 1.00x |
| nested.json | orjson | 0.325 | 0.333 | 0.367 | 57.457 | 0.84x |
| nested.json | msgspec | 0.464 | 0.474 | 0.514 | 57.457 | 0.59x |
| nested.json | ujson | 1.150 | 1.192 | 1.241 | 57.457 | 0.23x |
| nested.json | json | 2.417 | 2.449 | 2.495 | 57.457 | 0.11x |
| wide_arrays.json | strata | 1.935 | 2.159 | 2.480 | 59.227 | 1.00x |
| wide_arrays.json | orjson | 2.182 | 2.399 | 2.762 | 59.227 | 0.90x |
| wide_arrays.json | msgspec | 3.449 | 3.965 | 4.176 | 59.227 | 0.54x |
| wide_arrays.json | ujson | 7.446 | 7.671 | 8.047 | 59.227 | 0.28x |
| wide_arrays.json | json | 18.261 | 18.924 | 20.380 | 59.227 | 0.11x |
| mixed.json | strata | 0.066 | 0.071 | 0.079 | 56.168 | 1.00x |
| mixed.json | orjson | 0.067 | 0.074 | 0.107 | 56.168 | 0.96x |
| mixed.json | msgspec | 0.093 | 0.103 | 0.122 | 56.168 | 0.69x |
| mixed.json | ujson | 0.272 | 0.280 | 0.337 | 56.168 | 0.25x |
| mixed.json | json | 0.513 | 0.535 | 0.592 | 56.168 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.945 | 11.448 | 12.690 | 58.523 | 1.00x |
| users.json | orjson | 13.977 | 15.871 | 16.728 | 58.523 | 0.72x |
| users.json | msgspec | 13.718 | 15.265 | 18.078 | 58.523 | 0.75x |
| users.json | ujson | 24.862 | 28.029 | 31.153 | 58.523 | 0.41x |
| users.json | json | 22.912 | 24.837 | 26.199 | 58.523 | 0.46x |
| flat.json | strata | 0.925 | 0.965 | 1.017 | 58.484 | 1.00x |
| flat.json | orjson | 1.252 | 1.312 | 1.364 | 58.484 | 0.74x |
| flat.json | msgspec | 1.245 | 1.308 | 1.365 | 58.484 | 0.74x |
| flat.json | ujson | 2.732 | 2.834 | 2.988 | 58.484 | 0.34x |
| flat.json | json | 2.195 | 2.224 | 2.270 | 58.484 | 0.43x |
| nested.json | strata | 0.820 | 0.856 | 0.904 | 57.016 | 1.00x |
| nested.json | orjson | 1.180 | 1.240 | 1.300 | 57.016 | 0.69x |
| nested.json | msgspec | 1.123 | 1.173 | 1.227 | 57.016 | 0.73x |
| nested.json | ujson | 1.980 | 2.035 | 2.131 | 57.016 | 0.42x |
| nested.json | json | 2.234 | 2.264 | 2.320 | 57.016 | 0.38x |
| wide_arrays.json | strata | 4.764 | 5.371 | 5.981 | 59.086 | 1.00x |
| wide_arrays.json | orjson | 6.177 | 6.770 | 7.794 | 59.086 | 0.79x |
| wide_arrays.json | msgspec | 6.376 | 7.046 | 8.272 | 59.086 | 0.76x |
| wide_arrays.json | ujson | 11.610 | 12.509 | 13.446 | 59.086 | 0.43x |
| wide_arrays.json | json | 12.212 | 13.125 | 16.212 | 59.086 | 0.41x |
| mixed.json | strata | 0.259 | 0.268 | 0.309 | 56.160 | 1.00x |
| mixed.json | orjson | 0.332 | 0.347 | 0.397 | 56.160 | 0.77x |
| mixed.json | msgspec | 0.353 | 0.369 | 0.421 | 56.160 | 0.73x |
| mixed.json | ujson | 0.546 | 0.580 | 0.643 | 56.160 | 0.46x |
| mixed.json | json | 0.590 | 0.614 | 0.677 | 56.160 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.048 | 11.994 | 13.702 | 58.938 | 1.00x |
| users.ndjson | orjson | 16.930 | 18.339 | 21.563 | 58.938 | 0.65x |
| users.ndjson | msgspec | 16.928 | 18.307 | 22.558 | 58.938 | 0.66x |
| users.ndjson | ujson | 25.023 | 27.124 | 30.701 | 58.938 | 0.44x |
| users.ndjson | json | 29.270 | 31.840 | 35.107 | 58.938 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.764 | 3.964 | 4.687 | 59.570 | 1.00x |
| users.json | orjson | 4.412 | 4.505 | 5.069 | 59.570 | 0.88x |
| users.json | msgspec | 5.768 | 5.906 | 6.448 | 59.570 | 0.67x |
| users.json | ujson | 23.206 | 23.571 | 24.390 | 59.570 | 0.17x |
| users.json | json | 32.273 | 32.761 | 34.462 | 59.570 | 0.12x |
| flat.json | strata | 0.627 | 0.668 | 0.732 | 57.906 | 1.00x |
| flat.json | orjson | 0.728 | 0.768 | 0.845 | 57.906 | 0.87x |
| flat.json | msgspec | 0.869 | 0.934 | 0.970 | 57.906 | 0.72x |
| flat.json | ujson | 2.855 | 2.907 | 3.086 | 57.906 | 0.23x |
| flat.json | json | 3.360 | 3.421 | 3.498 | 57.906 | 0.20x |
| nested.json | strata | 0.579 | 0.627 | 0.716 | 56.809 | 1.00x |
| nested.json | orjson | 0.653 | 0.712 | 0.787 | 56.809 | 0.88x |
| nested.json | msgspec | 0.796 | 0.852 | 0.902 | 56.809 | 0.74x |
| nested.json | ujson | 2.283 | 2.335 | 2.611 | 56.809 | 0.27x |
| nested.json | json | 3.472 | 3.561 | 3.691 | 56.809 | 0.18x |
| wide_arrays.json | strata | 2.642 | 3.089 | 3.774 | 56.816 | 1.00x |
| wide_arrays.json | orjson | 3.099 | 3.309 | 4.182 | 56.816 | 0.93x |
| wide_arrays.json | msgspec | 4.341 | 4.829 | 5.524 | 56.816 | 0.64x |
| wide_arrays.json | ujson | 14.378 | 14.921 | 16.011 | 56.816 | 0.21x |
| wide_arrays.json | json | 25.203 | 26.364 | 27.426 | 56.816 | 0.12x |
| mixed.json | strata | 0.360 | 0.386 | 0.466 | 55.902 | 1.00x |
| mixed.json | orjson | 0.387 | 0.409 | 0.464 | 55.902 | 0.94x |
| mixed.json | msgspec | 0.420 | 0.449 | 0.504 | 55.902 | 0.86x |
| mixed.json | ujson | 0.746 | 0.784 | 0.910 | 55.902 | 0.49x |
| mixed.json | json | 1.001 | 1.063 | 1.158 | 55.902 | 0.36x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.088 | 0.104 | 0.118 | 59.613 | 1.00x |
| users.json $[*].id | jmespath | 0.438 | 0.464 | 0.532 | 59.613 | 0.22x |
| users.json $[*].id | jsonpath-ng | 2.581 | 3.020 | 3.311 | 59.613 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.461 | 0.481 | 0.521 | 59.633 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.713 | 2.779 | 2.931 | 59.633 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.610 | 18.733 | 19.766 | 59.633 | 0.03x |
| users.json $..total | strata | 1.904 | 2.012 | 2.140 | 59.574 | 1.00x |
| users.json $..total | jsonpath-ng | 327.835 | 333.304 | 344.783 | 59.574 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.019 | 4.077 | 4.387 | 59.633 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.825 | 17.166 | 19.090 | 59.633 | 0.24x |
| users.json $[*].id | orjson+jsonpath-ng | 18.456 | 19.546 | 21.329 | 59.633 | 0.21x |
| users.json $[*].orders[*].total | strata | 4.224 | 4.286 | 4.379 | 59.633 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.368 | 18.938 | 20.186 | 59.633 | 0.23x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 36.494 | 38.068 | 42.130 | 59.633 | 0.11x |
| users.json $..total | strata | 14.048 | 16.205 | 18.418 | 59.574 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 352.158 | 361.894 | 375.510 | 59.574 | 0.04x |

