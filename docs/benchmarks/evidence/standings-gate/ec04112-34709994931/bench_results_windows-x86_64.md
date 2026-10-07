# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: ec0411225b6d58a1df905844c946a766d3c39f0a
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
| users.json | strata | 9.519 | 11.664 | 16.237 | 48.973 | 1.00x |
| users.json | orjson | 14.809 | 15.880 | 27.276 | 48.973 | 0.73x |
| users.json | msgspec | 13.324 | 17.611 | 25.145 | 48.973 | 0.66x |
| users.json | ujson | 21.924 | 24.318 | 34.986 | 48.973 | 0.48x |
| users.json | json | 23.461 | 27.615 | 35.985 | 48.973 | 0.42x |
| flat.json | strata | 0.979 | 1.047 | 1.117 | 56.887 | 1.00x |
| flat.json | orjson | 1.081 | 1.124 | 1.161 | 56.887 | 0.93x |
| flat.json | msgspec | 1.132 | 1.162 | 1.274 | 56.887 | 0.90x |
| flat.json | ujson | 2.176 | 2.321 | 2.398 | 56.887 | 0.45x |
| flat.json | json | 1.959 | 1.992 | 2.017 | 56.887 | 0.53x |
| nested.json | strata | 0.765 | 0.798 | 1.160 | 56.918 | 1.00x |
| nested.json | orjson | 1.045 | 1.101 | 1.478 | 56.918 | 0.72x |
| nested.json | msgspec | 0.993 | 1.056 | 1.823 | 56.918 | 0.76x |
| nested.json | ujson | 1.550 | 1.699 | 2.879 | 56.918 | 0.47x |
| nested.json | json | 2.158 | 2.253 | 3.571 | 56.918 | 0.35x |
| wide_arrays.json | strata | 4.185 | 4.498 | 5.298 | 58.910 | 1.00x |
| wide_arrays.json | orjson | 5.667 | 6.079 | 9.090 | 58.910 | 0.74x |
| wide_arrays.json | msgspec | 6.010 | 6.339 | 9.992 | 58.910 | 0.71x |
| wide_arrays.json | ujson | 8.268 | 8.836 | 14.445 | 58.910 | 0.51x |
| wide_arrays.json | json | 11.913 | 12.388 | 17.434 | 58.910 | 0.36x |
| mixed.json | strata | 0.185 | 0.190 | 0.298 | 56.840 | 1.00x |
| mixed.json | orjson | 0.216 | 0.218 | 0.380 | 56.840 | 0.87x |
| mixed.json | msgspec | 0.243 | 0.248 | 0.474 | 56.840 | 0.76x |
| mixed.json | ujson | 0.348 | 0.377 | 0.633 | 56.840 | 0.50x |
| mixed.json | json | 0.479 | 0.486 | 0.855 | 56.840 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.058 | 3.340 | 4.379 | 47.965 | 1.00x |
| users.json | orjson | 3.548 | 4.548 | 5.531 | 47.965 | 0.73x |
| users.json | msgspec | 4.961 | 5.145 | 8.405 | 47.965 | 0.65x |
| users.json | ujson | 14.449 | 16.606 | 23.709 | 47.965 | 0.20x |
| users.json | json | 23.240 | 26.520 | 32.121 | 47.965 | 0.13x |
| flat.json | strata | 0.312 | 0.317 | 0.358 | 57.270 | 1.00x |
| flat.json | orjson | 0.360 | 0.368 | 0.405 | 57.270 | 0.86x |
| flat.json | msgspec | 0.498 | 0.547 | 0.720 | 57.270 | 0.58x |
| flat.json | ujson | 1.442 | 1.499 | 1.641 | 57.270 | 0.21x |
| flat.json | json | 2.036 | 2.089 | 2.381 | 57.270 | 0.15x |
| nested.json | strata | 0.292 | 0.298 | 0.419 | 57.328 | 1.00x |
| nested.json | orjson | 0.326 | 0.361 | 0.502 | 57.328 | 0.82x |
| nested.json | msgspec | 0.480 | 0.493 | 0.756 | 57.328 | 0.61x |
| nested.json | ujson | 1.255 | 1.308 | 2.221 | 57.328 | 0.23x |
| nested.json | json | 2.442 | 2.541 | 3.450 | 57.328 | 0.12x |
| wide_arrays.json | strata | 2.032 | 2.106 | 2.942 | 58.594 | 1.00x |
| wide_arrays.json | orjson | 2.443 | 2.510 | 3.630 | 58.594 | 0.84x |
| wide_arrays.json | msgspec | 3.887 | 4.162 | 6.091 | 58.594 | 0.51x |
| wide_arrays.json | ujson | 7.657 | 7.963 | 10.415 | 58.594 | 0.26x |
| wide_arrays.json | json | 18.730 | 19.327 | 31.113 | 58.594 | 0.11x |
| mixed.json | strata | 0.073 | 0.076 | 0.094 | 57.102 | 1.00x |
| mixed.json | orjson | 0.069 | 0.071 | 0.137 | 57.102 | 1.07x |
| mixed.json | msgspec | 0.095 | 0.114 | 0.148 | 57.102 | 0.67x |
| mixed.json | ujson | 0.267 | 0.269 | 0.323 | 57.102 | 0.28x |
| mixed.json | json | 0.514 | 0.544 | 0.567 | 57.102 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.469 | 10.769 | 14.006 | 58.035 | 1.00x |
| users.json | orjson | 13.971 | 14.592 | 20.450 | 58.035 | 0.74x |
| users.json | msgspec | 13.811 | 14.287 | 17.205 | 58.035 | 0.75x |
| users.json | ujson | 26.055 | 27.000 | 28.047 | 58.035 | 0.40x |
| users.json | json | 23.252 | 25.229 | 33.129 | 58.035 | 0.43x |
| flat.json | strata | 0.942 | 1.085 | 1.492 | 57.594 | 1.00x |
| flat.json | orjson | 1.254 | 1.322 | 1.991 | 57.594 | 0.82x |
| flat.json | msgspec | 1.260 | 1.343 | 1.496 | 57.594 | 0.81x |
| flat.json | ujson | 2.745 | 2.832 | 3.113 | 57.594 | 0.38x |
| flat.json | json | 2.096 | 2.122 | 3.779 | 57.594 | 0.51x |
| nested.json | strata | 0.867 | 0.898 | 1.236 | 56.953 | 1.00x |
| nested.json | orjson | 1.241 | 1.260 | 1.884 | 56.953 | 0.71x |
| nested.json | msgspec | 1.126 | 1.199 | 1.956 | 56.953 | 0.75x |
| nested.json | ujson | 1.988 | 2.077 | 3.345 | 56.953 | 0.43x |
| nested.json | json | 2.289 | 2.367 | 4.106 | 56.953 | 0.38x |
| wide_arrays.json | strata | 4.681 | 5.365 | 7.792 | 58.594 | 1.00x |
| wide_arrays.json | orjson | 6.146 | 7.665 | 10.039 | 58.594 | 0.70x |
| wide_arrays.json | msgspec | 6.657 | 7.240 | 10.914 | 58.594 | 0.74x |
| wide_arrays.json | ujson | 11.444 | 12.367 | 15.100 | 58.594 | 0.43x |
| wide_arrays.json | json | 12.340 | 12.686 | 19.871 | 58.594 | 0.42x |
| mixed.json | strata | 0.259 | 0.266 | 0.409 | 57.105 | 1.00x |
| mixed.json | orjson | 0.336 | 0.372 | 0.536 | 57.105 | 0.71x |
| mixed.json | msgspec | 0.346 | 0.352 | 0.597 | 57.105 | 0.75x |
| mixed.json | ujson | 0.529 | 0.575 | 0.839 | 57.105 | 0.46x |
| mixed.json | json | 0.580 | 0.619 | 1.234 | 57.105 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 11.823 | 13.571 | 14.174 | 57.941 | 1.00x |
| users.ndjson | orjson | 18.460 | 20.004 | 22.988 | 57.941 | 0.68x |
| users.ndjson | msgspec | 18.043 | 19.806 | 24.340 | 57.941 | 0.69x |
| users.ndjson | ujson | 27.561 | 30.258 | 37.963 | 57.941 | 0.45x |
| users.ndjson | json | 31.164 | 34.211 | 44.132 | 57.941 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.953 | 4.021 | 12.068 | 58.262 | 1.00x |
| users.json | orjson | 4.416 | 4.608 | 5.804 | 58.262 | 0.87x |
| users.json | msgspec | 5.791 | 6.039 | 8.159 | 58.262 | 0.67x |
| users.json | ujson | 23.393 | 23.741 | 33.350 | 58.262 | 0.17x |
| users.json | json | 32.627 | 33.310 | 44.073 | 58.262 | 0.12x |
| flat.json | strata | 0.710 | 0.822 | 1.036 | 57.715 | 1.00x |
| flat.json | orjson | 0.749 | 0.797 | 0.850 | 57.715 | 1.03x |
| flat.json | msgspec | 0.906 | 0.971 | 1.232 | 57.715 | 0.85x |
| flat.json | ujson | 2.858 | 2.949 | 3.155 | 57.715 | 0.28x |
| flat.json | json | 3.414 | 3.522 | 5.723 | 57.715 | 0.23x |
| nested.json | strata | 0.665 | 0.705 | 1.092 | 57.312 | 1.00x |
| nested.json | orjson | 0.702 | 0.759 | 0.958 | 57.312 | 0.93x |
| nested.json | msgspec | 0.838 | 0.874 | 1.352 | 57.312 | 0.81x |
| nested.json | ujson | 2.331 | 2.400 | 4.064 | 57.312 | 0.29x |
| nested.json | json | 3.660 | 4.248 | 6.323 | 57.312 | 0.17x |
| wide_arrays.json | strata | 2.705 | 2.813 | 4.253 | 58.594 | 1.00x |
| wide_arrays.json | orjson | 3.137 | 3.710 | 5.128 | 58.594 | 0.76x |
| wide_arrays.json | msgspec | 4.691 | 5.393 | 7.020 | 58.594 | 0.52x |
| wide_arrays.json | ujson | 14.403 | 16.730 | 26.219 | 58.594 | 0.17x |
| wide_arrays.json | json | 25.163 | 27.839 | 36.551 | 58.594 | 0.10x |
| mixed.json | strata | 0.383 | 0.397 | 0.567 | 57.133 | 1.00x |
| mixed.json | orjson | 0.378 | 0.397 | 0.550 | 57.133 | 1.00x |
| mixed.json | msgspec | 0.410 | 0.437 | 0.573 | 57.133 | 0.91x |
| mixed.json | ujson | 0.740 | 0.822 | 1.249 | 57.133 | 0.48x |
| mixed.json | json | 0.993 | 1.069 | 1.712 | 57.133 | 0.37x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.084 | 0.094 | 0.142 | 58.328 | 1.00x |
| users.json $[*].id | jmespath | 0.439 | 0.467 | 0.673 | 58.328 | 0.20x |
| users.json $[*].id | jsonpath-ng | 2.536 | 2.678 | 2.969 | 58.328 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.464 | 0.478 | 0.510 | 58.355 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.756 | 2.845 | 2.951 | 58.355 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.054 | 17.768 | 18.980 | 58.355 | 0.03x |
| users.json $..total | strata | 1.906 | 1.922 | 3.388 | 58.355 | 1.00x |
| users.json $..total | jsonpath-ng | 323.449 | 331.233 | 341.049 | 58.355 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.047 | 4.144 | 5.821 | 58.348 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.058 | 15.636 | 15.983 | 58.348 | 0.27x |
| users.json $[*].id | orjson+jsonpath-ng | 17.203 | 17.776 | 23.580 | 58.348 | 0.23x |
| users.json $[*].orders[*].total | strata | 4.271 | 4.327 | 4.536 | 58.355 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.921 | 18.700 | 26.961 | 58.355 | 0.23x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 35.600 | 36.426 | 47.280 | 58.355 | 0.12x |
| users.json $..total | strata | 13.621 | 14.387 | 17.019 | 58.355 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 352.912 | 368.621 | 400.393 | 58.355 | 0.04x |

