# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c16eaa65c61f1cf9d4fac46e9f6d8e6b9f3b087f
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
| users.json | strata | 9.383 | 9.605 | 14.085 | 48.969 | 1.00x |
| users.json | orjson | 13.881 | 14.315 | 16.587 | 48.969 | 0.67x |
| users.json | msgspec | 12.816 | 13.195 | 21.697 | 48.969 | 0.73x |
| users.json | ujson | 19.774 | 20.688 | 30.063 | 48.969 | 0.46x |
| users.json | json | 22.010 | 22.501 | 27.829 | 48.969 | 0.43x |
| flat.json | strata | 0.859 | 0.894 | 1.279 | 57.617 | 1.00x |
| flat.json | orjson | 1.172 | 1.226 | 1.787 | 57.617 | 0.73x |
| flat.json | msgspec | 1.086 | 1.124 | 1.293 | 57.617 | 0.80x |
| flat.json | ujson | 1.730 | 1.772 | 1.821 | 57.617 | 0.50x |
| flat.json | json | 1.856 | 1.923 | 2.690 | 57.617 | 0.46x |
| nested.json | strata | 0.771 | 0.779 | 0.810 | 56.953 | 1.00x |
| nested.json | orjson | 1.077 | 1.125 | 1.294 | 56.953 | 0.69x |
| nested.json | msgspec | 0.970 | 1.014 | 1.053 | 56.953 | 0.77x |
| nested.json | ujson | 1.517 | 1.565 | 1.584 | 56.953 | 0.50x |
| nested.json | json | 2.076 | 2.087 | 2.108 | 56.953 | 0.37x |
| wide_arrays.json | strata | 4.441 | 4.576 | 4.774 | 59.098 | 1.00x |
| wide_arrays.json | orjson | 5.923 | 6.010 | 6.345 | 59.098 | 0.76x |
| wide_arrays.json | msgspec | 5.942 | 6.007 | 6.145 | 59.098 | 0.76x |
| wide_arrays.json | ujson | 8.067 | 8.154 | 8.745 | 59.098 | 0.56x |
| wide_arrays.json | json | 11.307 | 11.435 | 13.083 | 59.098 | 0.40x |
| mixed.json | strata | 0.185 | 0.194 | 0.213 | 56.961 | 1.00x |
| mixed.json | orjson | 0.214 | 0.218 | 0.250 | 56.961 | 0.89x |
| mixed.json | msgspec | 0.231 | 0.233 | 0.262 | 56.961 | 0.83x |
| mixed.json | ujson | 0.331 | 0.355 | 0.378 | 56.961 | 0.55x |
| mixed.json | json | 0.461 | 0.467 | 0.508 | 56.961 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.980 | 3.002 | 4.471 | 49.188 | 1.00x |
| users.json | orjson | 3.582 | 3.716 | 5.459 | 49.188 | 0.81x |
| users.json | msgspec | 5.276 | 5.323 | 5.637 | 49.188 | 0.56x |
| users.json | ujson | 12.529 | 12.735 | 17.380 | 49.188 | 0.24x |
| users.json | json | 22.763 | 22.994 | 38.188 | 49.188 | 0.13x |
| flat.json | strata | 0.323 | 0.331 | 0.361 | 57.699 | 1.00x |
| flat.json | orjson | 0.375 | 0.379 | 0.387 | 57.699 | 0.87x |
| flat.json | msgspec | 0.548 | 0.553 | 0.577 | 57.699 | 0.60x |
| flat.json | ujson | 1.429 | 1.455 | 1.529 | 57.699 | 0.23x |
| flat.json | json | 1.966 | 1.987 | 2.000 | 57.699 | 0.17x |
| nested.json | strata | 0.242 | 0.246 | 0.258 | 57.465 | 1.00x |
| nested.json | orjson | 0.322 | 0.351 | 0.366 | 57.465 | 0.70x |
| nested.json | msgspec | 0.500 | 0.509 | 0.554 | 57.465 | 0.48x |
| nested.json | ujson | 1.056 | 1.103 | 1.313 | 57.465 | 0.22x |
| nested.json | json | 2.486 | 2.533 | 3.211 | 57.465 | 0.10x |
| wide_arrays.json | strata | 2.118 | 2.164 | 2.250 | 58.672 | 1.00x |
| wide_arrays.json | orjson | 2.712 | 2.798 | 2.849 | 58.672 | 0.77x |
| wide_arrays.json | msgspec | 4.526 | 4.554 | 4.612 | 58.672 | 0.48x |
| wide_arrays.json | ujson | 7.821 | 7.926 | 8.437 | 58.672 | 0.27x |
| wide_arrays.json | json | 18.635 | 18.883 | 19.274 | 58.672 | 0.11x |
| mixed.json | strata | 0.068 | 0.070 | 0.086 | 57.051 | 1.00x |
| mixed.json | orjson | 0.073 | 0.074 | 0.089 | 57.051 | 0.95x |
| mixed.json | msgspec | 0.098 | 0.100 | 0.105 | 57.051 | 0.70x |
| mixed.json | ujson | 0.255 | 0.260 | 0.289 | 57.051 | 0.27x |
| mixed.json | json | 0.517 | 0.534 | 0.597 | 57.051 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.501 | 10.823 | 15.466 | 59.242 | 1.00x |
| users.json | orjson | 14.807 | 15.229 | 21.577 | 59.242 | 0.71x |
| users.json | msgspec | 13.791 | 14.259 | 17.382 | 59.242 | 0.76x |
| users.json | ujson | 23.757 | 24.294 | 25.455 | 59.242 | 0.45x |
| users.json | json | 23.002 | 23.194 | 27.548 | 59.242 | 0.47x |
| flat.json | strata | 1.106 | 1.156 | 1.230 | 57.246 | 1.00x |
| flat.json | orjson | 1.358 | 1.390 | 1.493 | 57.246 | 0.83x |
| flat.json | msgspec | 1.246 | 1.270 | 1.352 | 57.246 | 0.91x |
| flat.json | ujson | 2.192 | 2.286 | 4.325 | 57.246 | 0.51x |
| flat.json | json | 2.016 | 2.030 | 3.150 | 57.246 | 0.57x |
| nested.json | strata | 0.848 | 0.893 | 1.078 | 57.133 | 1.00x |
| nested.json | orjson | 1.215 | 1.247 | 1.363 | 57.133 | 0.72x |
| nested.json | msgspec | 1.092 | 1.147 | 1.701 | 57.133 | 0.78x |
| nested.json | ujson | 1.883 | 1.910 | 2.022 | 57.133 | 0.47x |
| nested.json | json | 2.197 | 2.213 | 2.346 | 57.133 | 0.40x |
| wide_arrays.json | strata | 4.890 | 4.977 | 5.046 | 58.672 | 1.00x |
| wide_arrays.json | orjson | 6.264 | 6.326 | 6.472 | 58.672 | 0.79x |
| wide_arrays.json | msgspec | 6.377 | 6.455 | 6.551 | 58.672 | 0.77x |
| wide_arrays.json | ujson | 10.244 | 10.382 | 10.499 | 58.672 | 0.48x |
| wide_arrays.json | json | 11.632 | 11.713 | 11.832 | 58.672 | 0.42x |
| mixed.json | strata | 0.258 | 0.288 | 0.331 | 57.051 | 1.00x |
| mixed.json | orjson | 0.329 | 0.334 | 0.347 | 57.051 | 0.86x |
| mixed.json | msgspec | 0.337 | 0.345 | 0.380 | 57.051 | 0.84x |
| mixed.json | ujson | 0.495 | 0.510 | 0.530 | 57.051 | 0.57x |
| mixed.json | json | 0.574 | 0.580 | 0.610 | 57.051 | 0.50x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 11.011 | 11.573 | 13.006 | 57.938 | 1.00x |
| users.ndjson | orjson | 18.279 | 19.200 | 22.604 | 57.938 | 0.60x |
| users.ndjson | msgspec | 18.111 | 19.427 | 21.835 | 57.938 | 0.60x |
| users.ndjson | ujson | 25.749 | 26.550 | 41.279 | 57.938 | 0.44x |
| users.ndjson | json | 30.704 | 31.400 | 33.469 | 57.938 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.865 | 3.942 | 5.731 | 59.352 | 1.00x |
| users.json | orjson | 4.620 | 4.730 | 11.908 | 59.352 | 0.83x |
| users.json | msgspec | 6.354 | 6.503 | 8.940 | 59.352 | 0.61x |
| users.json | ujson | 21.667 | 22.385 | 36.273 | 59.352 | 0.18x |
| users.json | json | 31.869 | 32.057 | 38.740 | 59.352 | 0.12x |
| flat.json | strata | 0.646 | 0.667 | 0.711 | 57.742 | 1.00x |
| flat.json | orjson | 0.730 | 0.770 | 0.801 | 57.742 | 0.87x |
| flat.json | msgspec | 0.902 | 0.916 | 0.987 | 57.742 | 0.73x |
| flat.json | ujson | 2.840 | 2.881 | 2.969 | 57.742 | 0.23x |
| flat.json | json | 3.316 | 3.365 | 3.450 | 57.742 | 0.20x |
| nested.json | strata | 0.557 | 0.583 | 0.696 | 57.395 | 1.00x |
| nested.json | orjson | 0.680 | 0.722 | 6.236 | 57.395 | 0.81x |
| nested.json | msgspec | 0.865 | 0.905 | 2.725 | 57.395 | 0.64x |
| nested.json | ujson | 2.067 | 2.118 | 2.196 | 57.395 | 0.28x |
| nested.json | json | 3.527 | 3.573 | 3.673 | 57.395 | 0.16x |
| wide_arrays.json | strata | 2.734 | 2.826 | 2.946 | 58.672 | 1.00x |
| wide_arrays.json | orjson | 3.340 | 3.459 | 4.215 | 58.672 | 0.82x |
| wide_arrays.json | msgspec | 5.162 | 5.231 | 7.029 | 58.672 | 0.54x |
| wide_arrays.json | ujson | 14.493 | 14.640 | 14.820 | 58.672 | 0.19x |
| wide_arrays.json | json | 25.536 | 25.630 | 26.267 | 58.672 | 0.11x |
| mixed.json | strata | 0.355 | 0.373 | 0.451 | 57.070 | 1.00x |
| mixed.json | orjson | 0.383 | 0.396 | 0.500 | 57.070 | 0.94x |
| mixed.json | msgspec | 0.413 | 0.433 | 0.644 | 57.070 | 0.86x |
| mixed.json | ujson | 0.741 | 0.776 | 1.140 | 57.070 | 0.48x |
| mixed.json | json | 1.001 | 1.067 | 1.626 | 57.070 | 0.35x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.092 | 0.093 | 0.102 | 59.422 | 1.00x |
| users.json $[*].id | jmespath | 0.407 | 0.416 | 0.741 | 59.422 | 0.22x |
| users.json $[*].id | jsonpath-ng | 2.397 | 2.451 | 3.922 | 59.422 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.515 | 0.530 | 0.572 | 59.441 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.658 | 2.683 | 3.415 | 59.441 | 0.20x |
| users.json $[*].orders[*].total | jsonpath-ng | 16.593 | 16.967 | 28.488 | 59.441 | 0.03x |
| users.json $..total | strata | 1.952 | 1.957 | 1.970 | 59.445 | 1.00x |
| users.json $..total | jsonpath-ng | 318.796 | 331.266 | 352.768 | 59.445 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.290 | 4.329 | 5.917 | 59.441 | 1.00x |
| users.json $[*].id | orjson+jmespath | 16.095 | 16.321 | 17.123 | 59.441 | 0.27x |
| users.json $[*].id | orjson+jsonpath-ng | 17.894 | 18.213 | 23.467 | 59.441 | 0.24x |
| users.json $[*].orders[*].total | strata | 4.482 | 4.555 | 6.184 | 59.445 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 19.089 | 19.315 | 22.082 | 59.445 | 0.24x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 36.732 | 37.104 | 51.453 | 59.445 | 0.12x |
| users.json $..total | strata | 14.109 | 16.931 | 29.392 | 59.445 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 343.934 | 399.197 | 874.219 | 59.445 | 0.04x |

