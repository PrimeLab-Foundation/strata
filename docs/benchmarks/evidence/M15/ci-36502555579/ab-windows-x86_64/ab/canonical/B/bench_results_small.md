# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 296d2ea02694ce7811592deeaa970aaa11c9432f
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD
- compiler_flags: /nologo /O2 /W3 /DNDEBUG /MD /EHsc /std:c++20 /Zc:__cplusplus /D_USE_STD_VECTOR_ALGORITHMS=0 /arch:AVX2 /clang:-O3 /clang:-fprofile-use=D:\a\_temp\strata-arm\build\pgo\strata.profdata /DLL /MANIFEST:EMBED,ID=2 /MANIFESTUAC:NO /EXPORT:PyInit__strata (24 recorded commands; see the JSON companion)
- repeats: 60
- warmup: 2
- provenance_schema: 1

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.289 | 10.663 | 11.751 | 49.090 | 1.00x |
| users.json | orjson | 13.703 | 15.439 | 16.879 | 49.090 | 0.69x |
| users.json | msgspec | 13.187 | 14.643 | 15.735 | 49.090 | 0.73x |
| users.json | ujson | 22.175 | 25.504 | 27.483 | 49.090 | 0.42x |
| users.json | json | 23.015 | 24.987 | 30.998 | 49.090 | 0.43x |
| flat.json | strata | 0.839 | 0.917 | 0.977 | 57.457 | 1.00x |
| flat.json | orjson | 1.108 | 1.177 | 1.311 | 57.457 | 0.78x |
| flat.json | msgspec | 1.083 | 1.147 | 1.275 | 57.457 | 0.80x |
| flat.json | ujson | 2.154 | 2.304 | 2.471 | 57.457 | 0.40x |
| flat.json | json | 1.957 | 1.994 | 2.133 | 57.457 | 0.46x |
| nested.json | strata | 0.739 | 0.768 | 0.922 | 56.605 | 1.00x |
| nested.json | orjson | 1.050 | 1.131 | 1.282 | 56.605 | 0.68x |
| nested.json | msgspec | 0.996 | 1.056 | 1.151 | 56.605 | 0.73x |
| nested.json | ujson | 1.591 | 1.719 | 1.850 | 56.605 | 0.45x |
| nested.json | json | 2.120 | 2.176 | 2.376 | 56.605 | 0.35x |
| wide_arrays.json | strata | 4.384 | 4.838 | 5.971 | 58.367 | 1.00x |
| wide_arrays.json | orjson | 5.963 | 6.583 | 7.679 | 58.367 | 0.73x |
| wide_arrays.json | msgspec | 5.903 | 6.423 | 7.376 | 58.367 | 0.75x |
| wide_arrays.json | ujson | 8.711 | 9.413 | 10.283 | 58.367 | 0.51x |
| wide_arrays.json | json | 11.850 | 13.037 | 13.790 | 58.367 | 0.37x |
| mixed.json | strata | 0.183 | 0.193 | 0.258 | 56.316 | 1.00x |
| mixed.json | orjson | 0.215 | 0.228 | 0.307 | 56.316 | 0.85x |
| mixed.json | msgspec | 0.237 | 0.250 | 0.364 | 56.316 | 0.77x |
| mixed.json | ujson | 0.353 | 0.374 | 0.430 | 56.316 | 0.52x |
| mixed.json | json | 0.471 | 0.491 | 0.565 | 56.316 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.005 | 3.171 | 3.541 | 48.332 | 1.00x |
| users.json | orjson | 3.594 | 3.930 | 4.203 | 48.332 | 0.81x |
| users.json | msgspec | 4.938 | 5.242 | 5.786 | 48.332 | 0.60x |
| users.json | ujson | 13.853 | 14.693 | 15.273 | 48.332 | 0.22x |
| users.json | json | 22.683 | 23.383 | 24.335 | 48.332 | 0.14x |
| flat.json | strata | 0.297 | 0.324 | 0.382 | 57.789 | 1.00x |
| flat.json | orjson | 0.357 | 0.379 | 0.436 | 57.789 | 0.86x |
| flat.json | msgspec | 0.505 | 0.539 | 0.612 | 57.789 | 0.60x |
| flat.json | ujson | 1.427 | 1.490 | 1.582 | 57.789 | 0.22x |
| flat.json | json | 1.882 | 1.964 | 2.101 | 57.789 | 0.17x |
| nested.json | strata | 0.268 | 0.277 | 0.310 | 57.168 | 1.00x |
| nested.json | orjson | 0.325 | 0.333 | 0.375 | 57.168 | 0.83x |
| nested.json | msgspec | 0.471 | 0.487 | 0.527 | 57.168 | 0.57x |
| nested.json | ujson | 1.273 | 1.341 | 1.372 | 57.168 | 0.21x |
| nested.json | json | 2.448 | 2.495 | 2.544 | 57.168 | 0.11x |
| wide_arrays.json | strata | 1.912 | 2.052 | 2.327 | 57.742 | 1.00x |
| wide_arrays.json | orjson | 2.519 | 2.683 | 3.121 | 57.742 | 0.76x |
| wide_arrays.json | msgspec | 3.873 | 4.175 | 4.760 | 57.742 | 0.49x |
| wide_arrays.json | ujson | 7.588 | 7.967 | 8.622 | 57.742 | 0.26x |
| wide_arrays.json | json | 18.302 | 19.238 | 20.451 | 57.742 | 0.11x |
| mixed.json | strata | 0.070 | 0.074 | 0.092 | 56.488 | 1.00x |
| mixed.json | orjson | 0.070 | 0.074 | 0.087 | 56.488 | 1.00x |
| mixed.json | msgspec | 0.096 | 0.102 | 0.142 | 56.488 | 0.72x |
| mixed.json | ujson | 0.264 | 0.271 | 0.310 | 56.488 | 0.27x |
| mixed.json | json | 0.505 | 0.520 | 0.571 | 56.488 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.000 | 12.120 | 13.145 | 58.359 | 1.00x |
| users.json | orjson | 14.383 | 16.478 | 17.929 | 58.359 | 0.74x |
| users.json | msgspec | 14.320 | 16.366 | 19.088 | 58.359 | 0.74x |
| users.json | ujson | 27.494 | 30.170 | 32.635 | 58.359 | 0.40x |
| users.json | json | 23.783 | 25.927 | 27.047 | 58.359 | 0.47x |
| flat.json | strata | 0.943 | 1.059 | 1.213 | 57.246 | 1.00x |
| flat.json | orjson | 1.255 | 1.326 | 1.577 | 57.246 | 0.80x |
| flat.json | msgspec | 1.211 | 1.339 | 1.492 | 57.246 | 0.79x |
| flat.json | ujson | 2.699 | 2.916 | 3.196 | 57.246 | 0.36x |
| flat.json | json | 2.125 | 2.210 | 2.410 | 57.246 | 0.48x |
| nested.json | strata | 0.831 | 0.892 | 1.035 | 56.582 | 1.00x |
| nested.json | orjson | 1.163 | 1.257 | 1.585 | 56.582 | 0.71x |
| nested.json | msgspec | 1.123 | 1.193 | 1.379 | 56.582 | 0.75x |
| nested.json | ujson | 2.012 | 2.099 | 2.246 | 56.582 | 0.43x |
| nested.json | json | 2.262 | 2.325 | 2.545 | 56.582 | 0.38x |
| wide_arrays.json | strata | 5.173 | 5.682 | 6.073 | 57.367 | 1.00x |
| wide_arrays.json | orjson | 6.568 | 7.123 | 7.467 | 57.367 | 0.80x |
| wide_arrays.json | msgspec | 6.594 | 7.076 | 7.654 | 57.367 | 0.80x |
| wide_arrays.json | ujson | 12.013 | 12.534 | 13.145 | 57.367 | 0.45x |
| wide_arrays.json | json | 12.697 | 13.239 | 13.891 | 57.367 | 0.43x |
| mixed.json | strata | 0.256 | 0.273 | 0.351 | 56.492 | 1.00x |
| mixed.json | orjson | 0.327 | 0.352 | 0.416 | 56.492 | 0.78x |
| mixed.json | msgspec | 0.349 | 0.367 | 0.447 | 56.492 | 0.74x |
| mixed.json | ujson | 0.550 | 0.576 | 0.667 | 56.492 | 0.47x |
| mixed.json | json | 0.584 | 0.610 | 0.709 | 56.492 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 11.962 | 13.783 | 14.634 | 58.309 | 1.00x |
| users.ndjson | orjson | 18.707 | 20.496 | 22.509 | 58.309 | 0.67x |
| users.ndjson | msgspec | 18.807 | 20.710 | 22.562 | 58.309 | 0.67x |
| users.ndjson | ujson | 28.771 | 30.368 | 32.017 | 58.309 | 0.45x |
| users.ndjson | json | 33.057 | 34.420 | 35.637 | 58.309 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.944 | 4.160 | 4.601 | 58.406 | 1.00x |
| users.json | orjson | 4.704 | 5.138 | 6.038 | 58.406 | 0.81x |
| users.json | msgspec | 5.943 | 6.424 | 6.969 | 58.406 | 0.65x |
| users.json | ujson | 23.519 | 24.239 | 25.242 | 58.406 | 0.17x |
| users.json | json | 32.209 | 33.268 | 34.556 | 58.406 | 0.13x |
| flat.json | strata | 0.643 | 0.712 | 0.937 | 57.617 | 1.00x |
| flat.json | orjson | 0.734 | 0.806 | 1.042 | 57.617 | 0.88x |
| flat.json | msgspec | 0.882 | 0.962 | 1.314 | 57.617 | 0.74x |
| flat.json | ujson | 2.844 | 2.910 | 3.149 | 57.617 | 0.24x |
| flat.json | json | 3.305 | 3.426 | 3.693 | 57.617 | 0.21x |
| nested.json | strata | 0.600 | 0.651 | 0.741 | 57.078 | 1.00x |
| nested.json | orjson | 0.687 | 0.749 | 0.804 | 57.078 | 0.87x |
| nested.json | msgspec | 0.828 | 0.884 | 0.969 | 57.078 | 0.74x |
| nested.json | ujson | 2.332 | 2.379 | 2.594 | 57.078 | 0.27x |
| nested.json | json | 3.499 | 3.568 | 3.793 | 57.078 | 0.18x |
| wide_arrays.json | strata | 2.659 | 2.833 | 3.109 | 57.301 | 1.00x |
| wide_arrays.json | orjson | 3.290 | 3.538 | 4.364 | 57.301 | 0.80x |
| wide_arrays.json | msgspec | 4.704 | 4.909 | 5.565 | 57.301 | 0.58x |
| wide_arrays.json | ujson | 14.591 | 15.125 | 15.779 | 57.301 | 0.19x |
| wide_arrays.json | json | 25.599 | 26.382 | 27.244 | 57.301 | 0.11x |
| mixed.json | strata | 0.360 | 0.401 | 0.503 | 56.512 | 1.00x |
| mixed.json | orjson | 0.394 | 0.431 | 0.525 | 56.512 | 0.93x |
| mixed.json | msgspec | 0.418 | 0.460 | 0.553 | 56.512 | 0.87x |
| mixed.json | ujson | 0.760 | 0.798 | 0.939 | 56.512 | 0.50x |
| mixed.json | json | 1.001 | 1.076 | 1.262 | 56.512 | 0.37x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.094 | 0.101 | 0.133 | 58.449 | 1.00x |
| users.json $[*].id | jmespath | 0.453 | 0.468 | 0.539 | 58.449 | 0.22x |
| users.json $[*].id | jsonpath-ng | 2.663 | 2.860 | 3.129 | 58.449 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.468 | 0.503 | 0.601 | 58.465 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.795 | 2.910 | 3.187 | 58.465 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 18.755 | 19.999 | 21.691 | 58.465 | 0.03x |
| users.json $..total | strata | 1.988 | 2.106 | 2.514 | 58.402 | 1.00x |
| users.json $..total | jsonpath-ng | 330.239 | 333.579 | 342.762 | 58.402 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.003 | 4.076 | 4.642 | 58.461 | 1.00x |
| users.json $[*].id | orjson+jmespath | 16.182 | 17.082 | 18.881 | 58.461 | 0.24x |
| users.json $[*].id | orjson+jsonpath-ng | 18.154 | 19.342 | 20.900 | 58.461 | 0.21x |
| users.json $[*].orders[*].total | strata | 4.243 | 4.319 | 4.448 | 58.465 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.957 | 20.596 | 22.428 | 58.465 | 0.21x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 37.807 | 42.697 | 45.092 | 58.465 | 0.10x |
| users.json $..total | strata | 16.430 | 20.695 | 22.474 | 58.402 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 359.143 | 364.991 | 374.943 | 58.402 | 0.06x |

