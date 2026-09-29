# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 38eaa9f248f509d9a4beb75b267189d011c1109e
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 26 Model 2 Stepping 1, AuthenticAMD
- compiler_flags: /nologo /O2 /W3 /DNDEBUG /MD /EHsc /std:c++20 /Zc:__cplusplus /D_USE_STD_VECTOR_ALGORITHMS=0 /arch:AVX2 /clang:-O3 /clang:-fprofile-use=D:\a\_temp\strata-arm\build\pgo\strata.profdata /DLL /MANIFEST:EMBED,ID=2 /MANIFESTUAC:NO /EXPORT:PyInit__strata (19 recorded commands; see the JSON companion)
- repeats: 60
- warmup: 2
- provenance_schema: 1

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 5.080 | 5.480 | 6.119 | 48.941 | 1.00x |
| users.json | orjson | 8.248 | 9.306 | 10.737 | 48.941 | 0.59x |
| users.json | msgspec | 7.121 | 7.909 | 8.509 | 48.941 | 0.69x |
| users.json | ujson | 11.059 | 13.388 | 15.171 | 48.941 | 0.41x |
| users.json | json | 12.319 | 13.635 | 15.080 | 48.941 | 0.40x |
| flat.json | strata | 0.512 | 0.592 | 0.654 | 57.895 | 1.00x |
| flat.json | orjson | 0.741 | 0.773 | 0.868 | 57.895 | 0.77x |
| flat.json | msgspec | 0.631 | 0.665 | 0.730 | 57.895 | 0.89x |
| flat.json | ujson | 0.931 | 1.023 | 1.294 | 57.895 | 0.58x |
| flat.json | json | 1.048 | 1.086 | 1.156 | 57.895 | 0.55x |
| nested.json | strata | 0.383 | 0.410 | 0.434 | 57.410 | 1.00x |
| nested.json | orjson | 0.587 | 0.641 | 0.692 | 57.410 | 0.64x |
| nested.json | msgspec | 0.485 | 0.509 | 0.556 | 57.410 | 0.80x |
| nested.json | ujson | 0.772 | 0.819 | 0.873 | 57.410 | 0.50x |
| nested.json | json | 1.051 | 1.105 | 1.152 | 57.410 | 0.37x |
| wide_arrays.json | strata | 2.391 | 2.453 | 2.587 | 59.625 | 1.00x |
| wide_arrays.json | orjson | 3.432 | 3.650 | 4.232 | 59.625 | 0.67x |
| wide_arrays.json | msgspec | 3.340 | 3.471 | 3.644 | 59.625 | 0.71x |
| wide_arrays.json | ujson | 4.644 | 4.830 | 5.195 | 59.625 | 0.51x |
| wide_arrays.json | json | 6.215 | 6.405 | 6.731 | 59.625 | 0.38x |
| mixed.json | strata | 0.099 | 0.100 | 0.103 | 58.086 | 1.00x |
| mixed.json | orjson | 0.116 | 0.117 | 0.129 | 58.086 | 0.85x |
| mixed.json | msgspec | 0.124 | 0.127 | 0.141 | 58.086 | 0.79x |
| mixed.json | ujson | 0.173 | 0.178 | 0.198 | 58.086 | 0.56x |
| mixed.json | json | 0.250 | 0.252 | 0.266 | 58.086 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.818 | 1.910 | 2.097 | 48.496 | 1.00x |
| users.json | orjson | 1.959 | 2.173 | 2.338 | 48.496 | 0.88x |
| users.json | msgspec | 3.361 | 3.506 | 3.807 | 48.496 | 0.54x |
| users.json | ujson | 7.138 | 7.520 | 8.631 | 48.496 | 0.25x |
| users.json | json | 12.581 | 13.289 | 14.330 | 48.496 | 0.14x |
| flat.json | strata | 0.202 | 0.209 | 0.217 | 58.273 | 1.00x |
| flat.json | orjson | 0.193 | 0.198 | 0.206 | 58.273 | 1.05x |
| flat.json | msgspec | 0.326 | 0.336 | 0.353 | 58.273 | 0.62x |
| flat.json | ujson | 0.614 | 0.641 | 0.670 | 58.273 | 0.33x |
| flat.json | json | 1.065 | 1.105 | 1.131 | 58.273 | 0.19x |
| nested.json | strata | 0.125 | 0.129 | 0.149 | 58.000 | 1.00x |
| nested.json | orjson | 0.161 | 0.168 | 0.191 | 58.000 | 0.77x |
| nested.json | msgspec | 0.282 | 0.293 | 0.321 | 58.000 | 0.44x |
| nested.json | ujson | 0.536 | 0.560 | 0.596 | 58.000 | 0.23x |
| nested.json | json | 1.327 | 1.383 | 1.444 | 58.000 | 0.09x |
| wide_arrays.json | strata | 1.297 | 1.366 | 1.497 | 59.066 | 1.00x |
| wide_arrays.json | orjson | 1.520 | 1.588 | 1.720 | 59.066 | 0.86x |
| wide_arrays.json | msgspec | 2.639 | 2.794 | 2.984 | 59.066 | 0.49x |
| wide_arrays.json | ujson | 4.554 | 4.725 | 4.909 | 59.066 | 0.29x |
| wide_arrays.json | json | 10.418 | 10.792 | 11.133 | 59.066 | 0.13x |
| mixed.json | strata | 0.039 | 0.041 | 0.045 | 58.258 | 1.00x |
| mixed.json | orjson | 0.035 | 0.037 | 0.041 | 58.258 | 1.12x |
| mixed.json | msgspec | 0.055 | 0.060 | 0.078 | 58.258 | 0.69x |
| mixed.json | ujson | 0.124 | 0.135 | 0.155 | 58.258 | 0.31x |
| mixed.json | json | 0.288 | 0.304 | 0.328 | 58.258 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.737 | 7.882 | 9.750 | 58.531 | 1.00x |
| users.json | orjson | 9.600 | 11.368 | 14.041 | 58.531 | 0.69x |
| users.json | msgspec | 8.527 | 10.197 | 11.623 | 58.531 | 0.77x |
| users.json | ujson | 14.848 | 17.292 | 20.651 | 58.531 | 0.46x |
| users.json | json | 13.593 | 15.249 | 17.036 | 58.531 | 0.52x |
| flat.json | strata | 0.561 | 0.682 | 0.772 | 58.152 | 1.00x |
| flat.json | orjson | 0.812 | 0.876 | 0.935 | 58.152 | 0.78x |
| flat.json | msgspec | 0.716 | 0.763 | 0.829 | 58.152 | 0.89x |
| flat.json | ujson | 1.174 | 1.290 | 1.422 | 58.152 | 0.53x |
| flat.json | json | 1.111 | 1.150 | 1.212 | 58.152 | 0.59x |
| nested.json | strata | 0.435 | 0.464 | 0.501 | 57.816 | 1.00x |
| nested.json | orjson | 0.661 | 0.708 | 0.762 | 57.816 | 0.66x |
| nested.json | msgspec | 0.560 | 0.580 | 0.628 | 57.816 | 0.80x |
| nested.json | ujson | 0.947 | 0.990 | 1.044 | 57.816 | 0.47x |
| nested.json | json | 1.128 | 1.160 | 1.210 | 57.816 | 0.40x |
| wide_arrays.json | strata | 2.851 | 3.093 | 3.648 | 59.066 | 1.00x |
| wide_arrays.json | orjson | 3.982 | 4.442 | 5.238 | 59.066 | 0.70x |
| wide_arrays.json | msgspec | 3.953 | 4.223 | 4.816 | 59.066 | 0.73x |
| wide_arrays.json | ujson | 6.227 | 6.798 | 7.827 | 59.066 | 0.45x |
| wide_arrays.json | json | 6.752 | 7.294 | 8.336 | 59.066 | 0.42x |
| mixed.json | strata | 0.138 | 0.147 | 0.172 | 58.258 | 1.00x |
| mixed.json | orjson | 0.175 | 0.185 | 0.226 | 58.258 | 0.79x |
| mixed.json | msgspec | 0.181 | 0.192 | 0.216 | 58.258 | 0.76x |
| mixed.json | ujson | 0.264 | 0.279 | 0.311 | 58.258 | 0.52x |
| mixed.json | json | 0.308 | 0.326 | 0.353 | 58.258 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.775 | 7.390 | 8.295 | 58.664 | 1.00x |
| users.ndjson | orjson | 10.987 | 11.966 | 13.041 | 58.664 | 0.62x |
| users.ndjson | msgspec | 10.940 | 11.514 | 12.387 | 58.664 | 0.64x |
| users.ndjson | ujson | 15.628 | 17.025 | 18.214 | 58.664 | 0.43x |
| users.ndjson | json | 18.298 | 19.741 | 21.042 | 58.664 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.597 | 2.773 | 3.113 | 57.910 | 1.00x |
| users.json | orjson | 2.794 | 3.059 | 3.297 | 57.910 | 0.91x |
| users.json | msgspec | 3.978 | 4.267 | 4.658 | 57.910 | 0.65x |
| users.json | ujson | 11.967 | 12.556 | 13.278 | 57.910 | 0.22x |
| users.json | json | 17.801 | 18.417 | 19.584 | 57.910 | 0.15x |
| flat.json | strata | 0.418 | 0.432 | 0.494 | 57.887 | 1.00x |
| flat.json | orjson | 0.419 | 0.435 | 0.495 | 57.887 | 0.99x |
| flat.json | msgspec | 0.555 | 0.572 | 0.632 | 57.887 | 0.76x |
| flat.json | ujson | 1.309 | 1.351 | 1.416 | 57.887 | 0.32x |
| flat.json | json | 1.746 | 1.794 | 1.842 | 57.887 | 0.24x |
| nested.json | strata | 0.330 | 0.344 | 0.387 | 58.230 | 1.00x |
| nested.json | orjson | 0.386 | 0.402 | 0.440 | 58.230 | 0.86x |
| nested.json | msgspec | 0.507 | 0.536 | 0.578 | 58.230 | 0.64x |
| nested.json | ujson | 1.072 | 1.123 | 1.213 | 58.230 | 0.31x |
| nested.json | json | 1.883 | 1.930 | 2.050 | 58.230 | 0.18x |
| wide_arrays.json | strata | 1.885 | 2.034 | 2.262 | 59.078 | 1.00x |
| wide_arrays.json | orjson | 2.115 | 2.213 | 2.352 | 59.078 | 0.92x |
| wide_arrays.json | msgspec | 3.297 | 3.393 | 3.554 | 59.078 | 0.60x |
| wide_arrays.json | ujson | 8.096 | 8.258 | 8.558 | 59.078 | 0.25x |
| wide_arrays.json | json | 14.093 | 14.359 | 15.369 | 59.078 | 0.14x |
| mixed.json | strata | 0.206 | 0.275 | 0.428 | 58.285 | 1.00x |
| mixed.json | orjson | 0.211 | 0.288 | 0.472 | 58.285 | 0.95x |
| mixed.json | msgspec | 0.235 | 0.314 | 0.497 | 58.285 | 0.88x |
| mixed.json | ujson | 0.387 | 0.476 | 0.670 | 58.285 | 0.58x |
| mixed.json | json | 0.551 | 0.655 | 0.831 | 58.285 | 0.42x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.055 | 0.060 | 0.071 | 58.242 | 1.00x |
| users.json $[*].id | jmespath | 0.207 | 0.221 | 0.243 | 58.242 | 0.27x |
| users.json $[*].id | jsonpath-ng | 1.269 | 1.438 | 1.641 | 58.242 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.280 | 0.296 | 0.328 | 58.312 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.301 | 1.358 | 1.417 | 58.312 | 0.22x |
| users.json $[*].orders[*].total | jsonpath-ng | 9.647 | 10.470 | 11.784 | 58.312 | 0.03x |
| users.json $..total | strata | 1.080 | 1.149 | 1.206 | 58.312 | 1.00x |
| users.json $..total | jsonpath-ng | 187.488 | 195.105 | 202.320 | 58.312 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 2.692 | 2.769 | 2.934 | 58.254 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.966 | 12.093 | 13.286 | 58.254 | 0.23x |
| users.json $[*].id | orjson+jsonpath-ng | 12.091 | 13.232 | 14.904 | 58.254 | 0.21x |
| users.json $[*].orders[*].total | strata | 2.827 | 3.092 | 3.172 | 58.312 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.673 | 14.089 | 14.672 | 58.312 | 0.22x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 22.607 | 25.764 | 27.153 | 58.312 | 0.12x |
| users.json $..total | strata | 11.596 | 12.984 | 14.085 | 58.312 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 205.181 | 211.021 | 223.358 | 58.312 | 0.06x |

