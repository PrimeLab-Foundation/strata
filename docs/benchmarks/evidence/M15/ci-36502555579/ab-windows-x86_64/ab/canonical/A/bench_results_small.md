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
| users.json | strata | 9.851 | 11.111 | 12.983 | 49.074 | 1.00x |
| users.json | orjson | 14.232 | 15.761 | 19.182 | 49.074 | 0.70x |
| users.json | msgspec | 13.429 | 14.878 | 16.459 | 49.074 | 0.75x |
| users.json | ujson | 22.849 | 25.307 | 27.444 | 49.074 | 0.44x |
| users.json | json | 23.573 | 25.209 | 27.033 | 49.074 | 0.44x |
| flat.json | strata | 0.809 | 0.873 | 0.957 | 56.895 | 1.00x |
| flat.json | orjson | 1.083 | 1.170 | 1.270 | 56.895 | 0.75x |
| flat.json | msgspec | 1.092 | 1.151 | 1.223 | 56.895 | 0.76x |
| flat.json | ujson | 2.142 | 2.339 | 2.505 | 56.895 | 0.37x |
| flat.json | json | 1.962 | 1.988 | 2.072 | 56.895 | 0.44x |
| nested.json | strata | 0.748 | 0.777 | 0.880 | 56.652 | 1.00x |
| nested.json | orjson | 1.054 | 1.125 | 1.266 | 56.652 | 0.69x |
| nested.json | msgspec | 1.006 | 1.054 | 1.202 | 56.652 | 0.74x |
| nested.json | ujson | 1.616 | 1.684 | 1.874 | 56.652 | 0.46x |
| nested.json | json | 2.159 | 2.220 | 2.431 | 56.652 | 0.35x |
| wide_arrays.json | strata | 4.416 | 4.868 | 5.915 | 58.309 | 1.00x |
| wide_arrays.json | orjson | 6.024 | 6.610 | 7.501 | 58.309 | 0.74x |
| wide_arrays.json | msgspec | 5.939 | 6.659 | 7.593 | 58.309 | 0.73x |
| wide_arrays.json | ujson | 8.580 | 9.346 | 10.302 | 58.309 | 0.52x |
| wide_arrays.json | json | 11.941 | 12.982 | 13.797 | 58.309 | 0.37x |
| mixed.json | strata | 0.183 | 0.191 | 0.213 | 55.520 | 1.00x |
| mixed.json | orjson | 0.211 | 0.220 | 0.256 | 55.520 | 0.87x |
| mixed.json | msgspec | 0.232 | 0.240 | 0.286 | 55.520 | 0.80x |
| mixed.json | ujson | 0.361 | 0.386 | 0.449 | 55.520 | 0.49x |
| mixed.json | json | 0.469 | 0.500 | 0.546 | 55.520 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.066 | 3.196 | 3.689 | 49.266 | 1.00x |
| users.json | orjson | 3.809 | 4.047 | 4.665 | 49.266 | 0.79x |
| users.json | msgspec | 5.060 | 5.407 | 5.938 | 49.266 | 0.59x |
| users.json | ujson | 14.090 | 14.705 | 15.356 | 49.266 | 0.22x |
| users.json | json | 22.716 | 24.414 | 25.783 | 49.266 | 0.13x |
| flat.json | strata | 0.289 | 0.306 | 0.359 | 57.148 | 1.00x |
| flat.json | orjson | 0.359 | 0.375 | 0.427 | 57.148 | 0.81x |
| flat.json | msgspec | 0.501 | 0.520 | 0.569 | 57.148 | 0.59x |
| flat.json | ujson | 1.424 | 1.470 | 1.549 | 57.148 | 0.21x |
| flat.json | json | 1.901 | 1.932 | 2.040 | 57.148 | 0.16x |
| nested.json | strata | 0.268 | 0.281 | 0.322 | 56.988 | 1.00x |
| nested.json | orjson | 0.327 | 0.336 | 0.372 | 56.988 | 0.84x |
| nested.json | msgspec | 0.471 | 0.492 | 0.547 | 56.988 | 0.57x |
| nested.json | ujson | 1.243 | 1.291 | 1.324 | 56.988 | 0.22x |
| nested.json | json | 2.400 | 2.438 | 2.496 | 56.988 | 0.12x |
| wide_arrays.json | strata | 1.907 | 2.039 | 2.322 | 57.699 | 1.00x |
| wide_arrays.json | orjson | 2.524 | 2.739 | 2.985 | 57.699 | 0.74x |
| wide_arrays.json | msgspec | 3.751 | 3.920 | 4.250 | 57.699 | 0.52x |
| wide_arrays.json | ujson | 7.566 | 7.833 | 8.614 | 57.699 | 0.26x |
| wide_arrays.json | json | 18.100 | 18.813 | 24.086 | 57.699 | 0.11x |
| mixed.json | strata | 0.068 | 0.074 | 0.105 | 55.840 | 1.00x |
| mixed.json | orjson | 0.070 | 0.073 | 0.093 | 55.840 | 1.00x |
| mixed.json | msgspec | 0.094 | 0.101 | 0.145 | 55.840 | 0.73x |
| mixed.json | ujson | 0.261 | 0.269 | 0.310 | 55.840 | 0.27x |
| mixed.json | json | 0.505 | 0.519 | 0.697 | 55.840 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.322 | 12.695 | 13.991 | 59.293 | 1.00x |
| users.json | orjson | 14.619 | 16.659 | 18.551 | 59.293 | 0.76x |
| users.json | msgspec | 14.374 | 16.385 | 17.987 | 59.293 | 0.77x |
| users.json | ujson | 27.461 | 30.329 | 32.591 | 59.293 | 0.42x |
| users.json | json | 24.139 | 26.002 | 27.347 | 59.293 | 0.49x |
| flat.json | strata | 0.929 | 0.988 | 1.136 | 57.094 | 1.00x |
| flat.json | orjson | 1.225 | 1.306 | 1.417 | 57.094 | 0.76x |
| flat.json | msgspec | 1.247 | 1.314 | 1.478 | 57.094 | 0.75x |
| flat.json | ujson | 2.723 | 2.932 | 3.135 | 57.094 | 0.34x |
| flat.json | json | 2.137 | 2.180 | 2.352 | 57.094 | 0.45x |
| nested.json | strata | 0.841 | 0.899 | 1.112 | 56.648 | 1.00x |
| nested.json | orjson | 1.172 | 1.282 | 1.486 | 56.648 | 0.70x |
| nested.json | msgspec | 1.131 | 1.198 | 1.320 | 56.648 | 0.75x |
| nested.json | ujson | 1.995 | 2.063 | 2.298 | 56.648 | 0.44x |
| nested.json | json | 2.295 | 2.340 | 2.490 | 56.648 | 0.38x |
| wide_arrays.json | strata | 5.093 | 5.542 | 6.101 | 57.242 | 1.00x |
| wide_arrays.json | orjson | 6.509 | 7.032 | 7.513 | 57.242 | 0.79x |
| wide_arrays.json | msgspec | 6.597 | 7.118 | 7.622 | 57.242 | 0.78x |
| wide_arrays.json | ujson | 11.733 | 12.422 | 13.122 | 57.242 | 0.45x |
| wide_arrays.json | json | 12.579 | 13.308 | 13.810 | 57.242 | 0.42x |
| mixed.json | strata | 0.257 | 0.282 | 0.331 | 55.836 | 1.00x |
| mixed.json | orjson | 0.327 | 0.360 | 0.429 | 55.836 | 0.78x |
| mixed.json | msgspec | 0.344 | 0.372 | 0.430 | 55.836 | 0.76x |
| mixed.json | ujson | 0.540 | 0.590 | 0.667 | 55.836 | 0.48x |
| mixed.json | json | 0.580 | 0.619 | 0.712 | 55.836 | 0.46x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 12.190 | 13.809 | 14.923 | 57.988 | 1.00x |
| users.ndjson | orjson | 18.775 | 20.165 | 21.757 | 57.988 | 0.68x |
| users.ndjson | msgspec | 19.457 | 20.591 | 21.919 | 57.988 | 0.67x |
| users.ndjson | ujson | 28.666 | 30.404 | 31.231 | 57.988 | 0.45x |
| users.ndjson | json | 32.443 | 34.108 | 36.540 | 57.988 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.961 | 4.253 | 4.641 | 59.230 | 1.00x |
| users.json | orjson | 4.676 | 5.196 | 5.780 | 59.230 | 0.82x |
| users.json | msgspec | 6.122 | 6.716 | 7.598 | 59.230 | 0.63x |
| users.json | ujson | 23.731 | 24.355 | 24.887 | 59.230 | 0.17x |
| users.json | json | 32.398 | 33.537 | 36.207 | 59.230 | 0.13x |
| flat.json | strata | 0.642 | 0.751 | 0.931 | 57.410 | 1.00x |
| flat.json | orjson | 0.726 | 0.812 | 0.977 | 57.410 | 0.92x |
| flat.json | msgspec | 0.899 | 0.985 | 1.153 | 57.410 | 0.76x |
| flat.json | ujson | 2.854 | 2.944 | 3.129 | 57.410 | 0.25x |
| flat.json | json | 3.344 | 3.465 | 3.722 | 57.410 | 0.22x |
| nested.json | strata | 0.606 | 0.684 | 0.763 | 56.898 | 1.00x |
| nested.json | orjson | 0.708 | 0.763 | 0.856 | 56.898 | 0.90x |
| nested.json | msgspec | 0.840 | 0.917 | 1.048 | 56.898 | 0.75x |
| nested.json | ujson | 2.338 | 2.398 | 2.538 | 56.898 | 0.29x |
| nested.json | json | 3.502 | 3.607 | 3.776 | 56.898 | 0.19x |
| wide_arrays.json | strata | 2.631 | 2.863 | 3.153 | 56.758 | 1.00x |
| wide_arrays.json | orjson | 3.304 | 3.564 | 4.087 | 56.758 | 0.80x |
| wide_arrays.json | msgspec | 4.713 | 5.075 | 5.684 | 56.758 | 0.56x |
| wide_arrays.json | ujson | 14.552 | 15.194 | 17.680 | 56.758 | 0.19x |
| wide_arrays.json | json | 25.334 | 26.450 | 27.356 | 56.758 | 0.11x |
| mixed.json | strata | 0.362 | 0.395 | 0.476 | 55.863 | 1.00x |
| mixed.json | orjson | 0.394 | 0.429 | 0.538 | 55.863 | 0.92x |
| mixed.json | msgspec | 0.421 | 0.459 | 0.545 | 55.863 | 0.86x |
| mixed.json | ujson | 0.754 | 0.817 | 0.910 | 55.863 | 0.48x |
| mixed.json | json | 1.004 | 1.073 | 1.216 | 55.863 | 0.37x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.095 | 0.105 | 0.135 | 59.273 | 1.00x |
| users.json $[*].id | jmespath | 0.447 | 0.477 | 0.550 | 59.273 | 0.22x |
| users.json $[*].id | jsonpath-ng | 2.615 | 2.917 | 3.053 | 59.273 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.479 | 0.531 | 0.692 | 59.293 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.851 | 2.985 | 3.373 | 59.293 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 18.676 | 19.960 | 24.188 | 59.293 | 0.03x |
| users.json $..total | strata | 2.021 | 2.164 | 2.598 | 59.230 | 1.00x |
| users.json $..total | jsonpath-ng | 329.386 | 332.680 | 343.833 | 59.230 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.002 | 4.081 | 4.396 | 59.293 | 1.00x |
| users.json $[*].id | orjson+jmespath | 16.269 | 18.048 | 20.687 | 59.293 | 0.23x |
| users.json $[*].id | orjson+jsonpath-ng | 18.508 | 20.203 | 24.270 | 59.293 | 0.20x |
| users.json $[*].orders[*].total | strata | 4.238 | 4.313 | 4.702 | 59.293 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 19.451 | 21.430 | 24.307 | 59.293 | 0.20x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 39.419 | 43.352 | 48.499 | 59.293 | 0.10x |
| users.json $..total | strata | 16.820 | 21.079 | 22.733 | 59.230 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 360.985 | 369.224 | 381.271 | 59.230 | 0.06x |

