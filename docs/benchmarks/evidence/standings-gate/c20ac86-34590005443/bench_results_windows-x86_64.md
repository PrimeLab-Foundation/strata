# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c20ac86eedff410e10c973bc3b1f19f6e9a5f56e
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
| users.json | strata | 8.759 | 9.005 | 12.699 | 48.938 | 1.00x |
| users.json | orjson | 13.196 | 13.354 | 17.312 | 48.938 | 0.67x |
| users.json | msgspec | 12.576 | 12.763 | 15.166 | 48.938 | 0.71x |
| users.json | ujson | 20.302 | 21.955 | 28.365 | 48.938 | 0.41x |
| users.json | json | 21.961 | 22.381 | 23.488 | 48.938 | 0.40x |
| flat.json | strata | 0.806 | 0.843 | 0.885 | 58.145 | 1.00x |
| flat.json | orjson | 1.094 | 1.128 | 1.166 | 58.145 | 0.75x |
| flat.json | msgspec | 1.072 | 1.085 | 1.200 | 58.145 | 0.78x |
| flat.json | ujson | 2.075 | 2.137 | 2.197 | 58.145 | 0.39x |
| flat.json | json | 1.976 | 1.988 | 2.021 | 58.145 | 0.42x |
| nested.json | strata | 0.734 | 0.746 | 0.779 | 57.492 | 1.00x |
| nested.json | orjson | 1.110 | 1.145 | 1.222 | 57.492 | 0.65x |
| nested.json | msgspec | 0.987 | 1.022 | 1.043 | 57.492 | 0.73x |
| nested.json | ujson | 1.506 | 1.560 | 1.646 | 57.492 | 0.48x |
| nested.json | json | 2.088 | 2.098 | 2.152 | 57.492 | 0.36x |
| wide_arrays.json | strata | 4.273 | 4.308 | 4.673 | 59.477 | 1.00x |
| wide_arrays.json | orjson | 5.662 | 5.836 | 6.597 | 59.477 | 0.74x |
| wide_arrays.json | msgspec | 5.723 | 5.867 | 6.051 | 59.477 | 0.73x |
| wide_arrays.json | ujson | 8.282 | 8.357 | 8.806 | 59.477 | 0.52x |
| wide_arrays.json | json | 11.546 | 11.635 | 12.339 | 59.477 | 0.37x |
| mixed.json | strata | 0.181 | 0.184 | 0.210 | 57.402 | 1.00x |
| mixed.json | orjson | 0.210 | 0.211 | 0.237 | 57.402 | 0.87x |
| mixed.json | msgspec | 0.233 | 0.236 | 0.260 | 57.402 | 0.78x |
| mixed.json | ujson | 0.346 | 0.352 | 0.391 | 57.402 | 0.52x |
| mixed.json | json | 0.467 | 0.490 | 0.518 | 57.402 | 0.37x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.083 | 3.139 | 3.184 | 47.523 | 1.00x |
| users.json | orjson | 3.714 | 3.806 | 3.844 | 47.523 | 0.82x |
| users.json | msgspec | 5.073 | 5.286 | 7.439 | 47.523 | 0.59x |
| users.json | ujson | 14.309 | 14.468 | 14.611 | 47.523 | 0.22x |
| users.json | json | 23.105 | 23.293 | 23.676 | 47.523 | 0.13x |
| flat.json | strata | 0.307 | 0.312 | 0.328 | 58.035 | 1.00x |
| flat.json | orjson | 0.352 | 0.356 | 0.441 | 58.035 | 0.87x |
| flat.json | msgspec | 0.497 | 0.506 | 0.595 | 58.035 | 0.62x |
| flat.json | ujson | 1.440 | 1.478 | 1.491 | 58.035 | 0.21x |
| flat.json | json | 1.903 | 1.921 | 1.985 | 58.035 | 0.16x |
| nested.json | strata | 0.299 | 0.301 | 0.468 | 57.703 | 1.00x |
| nested.json | orjson | 0.321 | 0.323 | 0.490 | 57.703 | 0.93x |
| nested.json | msgspec | 0.460 | 0.475 | 0.743 | 57.703 | 0.63x |
| nested.json | ujson | 1.205 | 1.288 | 2.151 | 57.703 | 0.23x |
| nested.json | json | 2.373 | 2.409 | 2.439 | 57.703 | 0.12x |
| wide_arrays.json | strata | 1.933 | 1.960 | 2.141 | 59.012 | 1.00x |
| wide_arrays.json | orjson | 2.517 | 2.559 | 2.649 | 59.012 | 0.77x |
| wide_arrays.json | msgspec | 3.925 | 4.016 | 4.067 | 59.012 | 0.49x |
| wide_arrays.json | ujson | 7.606 | 7.784 | 7.931 | 59.012 | 0.25x |
| wide_arrays.json | json | 18.657 | 18.885 | 19.245 | 59.012 | 0.10x |
| mixed.json | strata | 0.071 | 0.072 | 0.092 | 57.676 | 1.00x |
| mixed.json | orjson | 0.069 | 0.070 | 0.089 | 57.676 | 1.03x |
| mixed.json | msgspec | 0.093 | 0.095 | 0.117 | 57.676 | 0.76x |
| mixed.json | ujson | 0.270 | 0.273 | 0.290 | 57.676 | 0.26x |
| mixed.json | json | 0.507 | 0.520 | 0.557 | 57.676 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.075 | 10.437 | 11.491 | 57.746 | 1.00x |
| users.json | orjson | 14.119 | 14.484 | 14.962 | 57.746 | 0.72x |
| users.json | msgspec | 13.684 | 13.936 | 15.141 | 57.746 | 0.75x |
| users.json | ujson | 25.553 | 26.738 | 31.897 | 57.746 | 0.39x |
| users.json | json | 23.059 | 23.232 | 30.284 | 57.746 | 0.45x |
| flat.json | strata | 1.094 | 1.134 | 1.255 | 57.742 | 1.00x |
| flat.json | orjson | 1.233 | 1.291 | 1.457 | 57.742 | 0.88x |
| flat.json | msgspec | 1.279 | 1.329 | 1.449 | 57.742 | 0.85x |
| flat.json | ujson | 2.728 | 2.779 | 3.054 | 57.742 | 0.41x |
| flat.json | json | 2.129 | 2.133 | 2.431 | 57.742 | 0.53x |
| nested.json | strata | 0.825 | 0.849 | 0.880 | 57.516 | 1.00x |
| nested.json | orjson | 1.186 | 1.239 | 1.764 | 57.516 | 0.69x |
| nested.json | msgspec | 1.120 | 1.158 | 1.370 | 57.516 | 0.73x |
| nested.json | ujson | 1.984 | 2.012 | 2.191 | 57.516 | 0.42x |
| nested.json | json | 2.250 | 2.267 | 2.278 | 57.516 | 0.37x |
| wide_arrays.json | strata | 4.749 | 4.867 | 5.060 | 59.012 | 1.00x |
| wide_arrays.json | orjson | 6.184 | 6.297 | 6.561 | 59.012 | 0.77x |
| wide_arrays.json | msgspec | 6.335 | 6.443 | 7.098 | 59.012 | 0.76x |
| wide_arrays.json | ujson | 11.368 | 11.552 | 13.473 | 59.012 | 0.42x |
| wide_arrays.json | json | 12.025 | 12.267 | 12.870 | 59.012 | 0.40x |
| mixed.json | strata | 0.257 | 0.259 | 0.277 | 57.680 | 1.00x |
| mixed.json | orjson | 0.321 | 0.326 | 0.400 | 57.680 | 0.80x |
| mixed.json | msgspec | 0.343 | 0.347 | 0.373 | 57.680 | 0.75x |
| mixed.json | ujson | 0.533 | 0.538 | 0.601 | 57.680 | 0.48x |
| mixed.json | json | 0.584 | 0.601 | 0.643 | 57.680 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.052 | 10.400 | 10.512 | 58.344 | 1.00x |
| users.ndjson | orjson | 16.951 | 17.467 | 17.882 | 58.344 | 0.60x |
| users.ndjson | msgspec | 17.206 | 17.695 | 21.886 | 58.344 | 0.59x |
| users.ndjson | ujson | 25.546 | 26.423 | 27.680 | 58.344 | 0.39x |
| users.ndjson | json | 29.556 | 30.733 | 31.067 | 58.344 | 0.34x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.949 | 4.062 | 4.178 | 58.699 | 1.00x |
| users.json | orjson | 4.604 | 4.748 | 5.180 | 58.699 | 0.86x |
| users.json | msgspec | 5.983 | 6.149 | 10.669 | 58.699 | 0.66x |
| users.json | ujson | 23.357 | 23.557 | 23.961 | 58.699 | 0.17x |
| users.json | json | 31.792 | 32.217 | 32.647 | 58.699 | 0.13x |
| flat.json | strata | 0.655 | 0.668 | 0.705 | 58.035 | 1.00x |
| flat.json | orjson | 0.699 | 0.736 | 0.846 | 58.035 | 0.91x |
| flat.json | msgspec | 0.843 | 0.866 | 0.942 | 58.035 | 0.77x |
| flat.json | ujson | 2.791 | 2.841 | 2.870 | 58.035 | 0.24x |
| flat.json | json | 3.254 | 3.285 | 3.428 | 58.035 | 0.20x |
| nested.json | strata | 0.639 | 0.657 | 0.728 | 57.668 | 1.00x |
| nested.json | orjson | 0.659 | 0.709 | 0.738 | 57.668 | 0.93x |
| nested.json | msgspec | 0.799 | 0.819 | 0.905 | 57.668 | 0.80x |
| nested.json | ujson | 2.292 | 2.315 | 2.435 | 57.668 | 0.28x |
| nested.json | json | 3.437 | 3.475 | 5.113 | 57.668 | 0.19x |
| wide_arrays.json | strata | 2.581 | 2.690 | 2.856 | 58.945 | 1.00x |
| wide_arrays.json | orjson | 3.175 | 3.231 | 3.295 | 58.945 | 0.83x |
| wide_arrays.json | msgspec | 4.526 | 4.681 | 4.795 | 58.945 | 0.57x |
| wide_arrays.json | ujson | 14.353 | 14.505 | 14.688 | 58.945 | 0.19x |
| wide_arrays.json | json | 24.916 | 25.281 | 32.184 | 58.945 | 0.11x |
| mixed.json | strata | 0.377 | 0.399 | 0.506 | 57.680 | 1.00x |
| mixed.json | orjson | 0.376 | 0.386 | 0.433 | 57.680 | 1.03x |
| mixed.json | msgspec | 0.402 | 0.410 | 0.533 | 57.680 | 0.97x |
| mixed.json | ujson | 0.749 | 0.794 | 0.832 | 57.680 | 0.50x |
| mixed.json | json | 0.985 | 1.001 | 1.211 | 57.680 | 0.40x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.084 | 0.090 | 0.130 | 57.941 | 1.00x |
| users.json $[*].id | jmespath | 0.482 | 0.497 | 0.526 | 57.941 | 0.18x |
| users.json $[*].id | jsonpath-ng | 2.464 | 2.575 | 2.926 | 57.941 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.461 | 0.493 | 0.790 | 58.285 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.087 | 3.102 | 3.305 | 58.285 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.297 | 17.739 | 19.207 | 58.285 | 0.03x |
| users.json $..total | strata | 1.893 | 1.905 | 1.950 | 58.195 | 1.00x |
| users.json $..total | jsonpath-ng | 327.567 | 328.946 | 335.986 | 58.195 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.037 | 4.081 | 4.240 | 58.004 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.187 | 15.458 | 17.054 | 58.004 | 0.26x |
| users.json $[*].id | orjson+jsonpath-ng | 17.072 | 17.340 | 24.277 | 58.004 | 0.24x |
| users.json $[*].orders[*].total | strata | 4.278 | 4.308 | 4.428 | 58.184 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.110 | 18.509 | 19.059 | 58.184 | 0.23x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 35.179 | 35.886 | 36.250 | 58.184 | 0.12x |
| users.json $..total | strata | 13.775 | 15.070 | 17.853 | 58.242 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 350.798 | 356.873 | 367.402 | 58.242 | 0.04x |

