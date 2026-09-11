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
| users.json | strata | 9.101 | 9.610 | 12.842 | 48.996 | 1.00x |
| users.json | orjson | 13.282 | 13.545 | 19.390 | 48.996 | 0.71x |
| users.json | msgspec | 12.784 | 13.194 | 15.552 | 48.996 | 0.73x |
| users.json | ujson | 20.895 | 22.669 | 28.409 | 48.996 | 0.42x |
| users.json | json | 22.683 | 23.495 | 38.470 | 48.996 | 0.41x |
| flat.json | strata | 0.984 | 1.018 | 1.173 | 57.117 | 1.00x |
| flat.json | orjson | 1.120 | 1.206 | 1.263 | 57.117 | 0.84x |
| flat.json | msgspec | 1.119 | 1.165 | 1.216 | 57.117 | 0.87x |
| flat.json | ujson | 2.253 | 2.377 | 2.628 | 57.117 | 0.43x |
| flat.json | json | 1.984 | 2.000 | 2.152 | 57.117 | 0.51x |
| nested.json | strata | 0.771 | 0.804 | 0.941 | 56.875 | 1.00x |
| nested.json | orjson | 1.071 | 1.114 | 1.152 | 56.875 | 0.72x |
| nested.json | msgspec | 1.026 | 1.048 | 1.091 | 56.875 | 0.77x |
| nested.json | ujson | 1.594 | 1.659 | 1.801 | 56.875 | 0.48x |
| nested.json | json | 2.158 | 2.188 | 3.898 | 56.875 | 0.37x |
| wide_arrays.json | strata | 4.229 | 4.405 | 4.505 | 59.012 | 1.00x |
| wide_arrays.json | orjson | 5.796 | 6.237 | 7.750 | 59.012 | 0.71x |
| wide_arrays.json | msgspec | 5.796 | 6.016 | 6.832 | 59.012 | 0.73x |
| wide_arrays.json | ujson | 8.416 | 8.620 | 13.476 | 59.012 | 0.51x |
| wide_arrays.json | json | 11.599 | 12.157 | 12.707 | 59.012 | 0.36x |
| mixed.json | strata | 0.184 | 0.186 | 0.196 | 56.863 | 1.00x |
| mixed.json | orjson | 0.212 | 0.215 | 0.254 | 56.863 | 0.87x |
| mixed.json | msgspec | 0.235 | 0.239 | 0.287 | 56.863 | 0.78x |
| mixed.json | ujson | 0.350 | 0.360 | 0.532 | 56.863 | 0.52x |
| mixed.json | json | 0.473 | 0.477 | 0.516 | 56.863 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.146 | 3.204 | 3.281 | 48.992 | 1.00x |
| users.json | orjson | 3.612 | 3.700 | 3.975 | 48.992 | 0.87x |
| users.json | msgspec | 4.932 | 5.167 | 5.509 | 48.992 | 0.62x |
| users.json | ujson | 14.094 | 14.270 | 23.216 | 48.992 | 0.22x |
| users.json | json | 23.790 | 24.504 | 33.903 | 48.992 | 0.13x |
| flat.json | strata | 0.312 | 0.321 | 0.363 | 57.684 | 1.00x |
| flat.json | orjson | 0.356 | 0.363 | 0.398 | 57.684 | 0.89x |
| flat.json | msgspec | 0.504 | 0.519 | 0.545 | 57.684 | 0.62x |
| flat.json | ujson | 1.424 | 1.461 | 1.514 | 57.684 | 0.22x |
| flat.json | json | 1.987 | 2.007 | 2.035 | 57.684 | 0.16x |
| nested.json | strata | 0.303 | 0.313 | 0.400 | 57.504 | 1.00x |
| nested.json | orjson | 0.325 | 0.334 | 0.372 | 57.504 | 0.94x |
| nested.json | msgspec | 0.471 | 0.493 | 0.510 | 57.504 | 0.64x |
| nested.json | ujson | 1.274 | 1.324 | 1.383 | 57.504 | 0.24x |
| nested.json | json | 2.476 | 2.510 | 2.577 | 57.504 | 0.12x |
| wide_arrays.json | strata | 1.929 | 1.989 | 2.826 | 58.570 | 1.00x |
| wide_arrays.json | orjson | 2.547 | 2.610 | 3.809 | 58.570 | 0.76x |
| wide_arrays.json | msgspec | 4.123 | 4.242 | 4.597 | 58.570 | 0.47x |
| wide_arrays.json | ujson | 7.664 | 7.934 | 8.513 | 58.570 | 0.25x |
| wide_arrays.json | json | 19.199 | 19.673 | 22.943 | 58.570 | 0.10x |
| mixed.json | strata | 0.071 | 0.074 | 0.078 | 56.949 | 1.00x |
| mixed.json | orjson | 0.071 | 0.072 | 0.108 | 56.949 | 1.03x |
| mixed.json | msgspec | 0.095 | 0.097 | 0.134 | 56.949 | 0.77x |
| mixed.json | ujson | 0.266 | 0.286 | 0.452 | 56.949 | 0.26x |
| mixed.json | json | 0.518 | 0.526 | 0.567 | 56.949 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.095 | 11.631 | 17.800 | 59.023 | 1.00x |
| users.json | orjson | 14.574 | 14.947 | 20.246 | 59.023 | 0.78x |
| users.json | msgspec | 14.149 | 14.558 | 19.010 | 59.023 | 0.80x |
| users.json | ujson | 27.073 | 28.094 | 38.295 | 59.023 | 0.41x |
| users.json | json | 23.655 | 24.312 | 30.670 | 59.023 | 0.48x |
| flat.json | strata | 1.162 | 1.219 | 1.616 | 57.051 | 1.00x |
| flat.json | orjson | 1.261 | 1.367 | 1.896 | 57.051 | 0.89x |
| flat.json | msgspec | 1.285 | 1.403 | 1.468 | 57.051 | 0.87x |
| flat.json | ujson | 2.777 | 2.860 | 3.082 | 57.051 | 0.43x |
| flat.json | json | 2.134 | 2.196 | 3.677 | 57.051 | 0.55x |
| nested.json | strata | 0.874 | 0.898 | 0.955 | 57.508 | 1.00x |
| nested.json | orjson | 1.219 | 1.256 | 1.334 | 57.508 | 0.72x |
| nested.json | msgspec | 1.148 | 1.202 | 1.238 | 57.508 | 0.75x |
| nested.json | ujson | 2.030 | 2.106 | 2.223 | 57.508 | 0.43x |
| nested.json | json | 2.293 | 2.342 | 2.637 | 57.508 | 0.38x |
| wide_arrays.json | strata | 4.654 | 4.924 | 5.065 | 58.570 | 1.00x |
| wide_arrays.json | orjson | 6.221 | 6.513 | 7.248 | 58.570 | 0.76x |
| wide_arrays.json | msgspec | 6.349 | 6.535 | 6.848 | 58.570 | 0.75x |
| wide_arrays.json | ujson | 11.391 | 11.577 | 12.821 | 58.570 | 0.43x |
| wide_arrays.json | json | 12.150 | 12.545 | 16.998 | 58.570 | 0.39x |
| mixed.json | strata | 0.267 | 0.274 | 0.308 | 56.949 | 1.00x |
| mixed.json | orjson | 0.331 | 0.338 | 0.528 | 56.949 | 0.81x |
| mixed.json | msgspec | 0.351 | 0.368 | 0.402 | 56.949 | 0.74x |
| mixed.json | ujson | 0.540 | 0.570 | 0.622 | 56.949 | 0.48x |
| mixed.json | json | 0.593 | 0.598 | 0.653 | 56.949 | 0.46x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 11.956 | 12.395 | 13.108 | 58.441 | 1.00x |
| users.ndjson | orjson | 17.952 | 18.699 | 24.758 | 58.441 | 0.66x |
| users.ndjson | msgspec | 18.626 | 19.287 | 23.633 | 58.441 | 0.64x |
| users.ndjson | ujson | 27.823 | 29.374 | 33.917 | 58.441 | 0.42x |
| users.ndjson | json | 31.551 | 32.973 | 39.401 | 58.441 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.152 | 4.253 | 5.668 | 59.254 | 1.00x |
| users.json | orjson | 4.591 | 4.719 | 7.019 | 59.254 | 0.90x |
| users.json | msgspec | 5.953 | 6.142 | 6.564 | 59.254 | 0.69x |
| users.json | ujson | 23.550 | 23.804 | 40.743 | 59.254 | 0.18x |
| users.json | json | 33.369 | 33.718 | 56.767 | 59.254 | 0.13x |
| flat.json | strata | 0.686 | 0.725 | 0.792 | 57.801 | 1.00x |
| flat.json | orjson | 0.728 | 0.791 | 6.762 | 57.801 | 0.92x |
| flat.json | msgspec | 0.867 | 0.920 | 0.959 | 57.801 | 0.79x |
| flat.json | ujson | 2.819 | 2.880 | 3.112 | 57.801 | 0.25x |
| flat.json | json | 3.390 | 3.446 | 3.615 | 57.801 | 0.21x |
| nested.json | strata | 0.674 | 0.734 | 0.744 | 57.508 | 1.00x |
| nested.json | orjson | 0.706 | 0.748 | 0.794 | 57.508 | 0.98x |
| nested.json | msgspec | 0.823 | 0.878 | 1.169 | 57.508 | 0.84x |
| nested.json | ujson | 2.330 | 2.383 | 2.471 | 57.508 | 0.31x |
| nested.json | json | 3.570 | 3.634 | 3.918 | 57.508 | 0.20x |
| wide_arrays.json | strata | 2.751 | 2.808 | 2.952 | 58.570 | 1.00x |
| wide_arrays.json | orjson | 3.264 | 3.379 | 4.998 | 58.570 | 0.83x |
| wide_arrays.json | msgspec | 4.878 | 4.981 | 5.381 | 58.570 | 0.56x |
| wide_arrays.json | ujson | 14.652 | 14.848 | 17.438 | 58.570 | 0.19x |
| wide_arrays.json | json | 26.039 | 26.444 | 32.669 | 58.570 | 0.11x |
| mixed.json | strata | 0.392 | 0.407 | 0.453 | 56.949 | 1.00x |
| mixed.json | orjson | 0.388 | 0.395 | 0.482 | 56.949 | 1.03x |
| mixed.json | msgspec | 0.420 | 0.440 | 0.485 | 56.949 | 0.92x |
| mixed.json | ujson | 0.754 | 0.792 | 0.859 | 56.949 | 0.51x |
| mixed.json | json | 1.013 | 1.051 | 1.110 | 56.949 | 0.39x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.088 | 0.093 | 0.101 | 59.316 | 1.00x |
| users.json $[*].id | jmespath | 0.435 | 0.447 | 0.491 | 59.316 | 0.21x |
| users.json $[*].id | jsonpath-ng | 2.463 | 2.715 | 4.646 | 59.316 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.472 | 0.502 | 0.855 | 59.336 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.775 | 2.819 | 5.351 | 59.336 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 18.065 | 18.735 | 29.128 | 59.336 | 0.03x |
| users.json $..total | strata | 1.903 | 1.981 | 3.469 | 59.336 | 1.00x |
| users.json $..total | jsonpath-ng | 335.594 | 360.926 | 393.327 | 59.336 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.090 | 4.123 | 4.143 | 59.336 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.552 | 16.345 | 19.688 | 59.336 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 17.747 | 18.841 | 20.076 | 59.336 | 0.22x |
| users.json $[*].orders[*].total | strata | 4.317 | 4.347 | 4.411 | 59.336 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.355 | 19.354 | 24.233 | 59.336 | 0.22x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 36.719 | 39.084 | 55.301 | 59.336 | 0.11x |
| users.json $..total | strata | 15.123 | 17.116 | 18.883 | 59.336 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 356.946 | 365.938 | 412.419 | 59.336 | 0.05x |

