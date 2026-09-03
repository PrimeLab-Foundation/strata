# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 571381e
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 25 Model 17 Stepping 1, AuthenticAMD
- compiler_flags: /std:c++20 /O2 /arch:AVX2 /GL /LTCG /USEPROFILE (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.007 | 8.738 | 31.610 | 45.465 | 1.00x |
| users.json | orjson | 11.454 | 11.846 | 18.572 | 45.465 | 0.74x |
| users.json | msgspec | 10.313 | 10.465 | 17.101 | 45.465 | 0.83x |
| users.json | ujson | 17.286 | 18.413 | 24.466 | 45.465 | 0.47x |
| users.json | json | 17.644 | 18.119 | 29.486 | 45.465 | 0.48x |
| flat.json | strata | 0.895 | 0.947 | 1.543 | 49.691 | 1.00x |
| flat.json | orjson | 0.948 | 1.032 | 1.476 | 49.691 | 0.92x |
| flat.json | msgspec | 0.960 | 0.996 | 1.466 | 49.691 | 0.95x |
| flat.json | ujson | 1.822 | 1.935 | 2.702 | 49.691 | 0.49x |
| flat.json | json | 1.501 | 1.528 | 2.414 | 49.691 | 0.62x |
| nested.json | strata | 0.636 | 0.680 | 1.288 | 49.570 | 1.00x |
| nested.json | orjson | 0.872 | 0.898 | 1.395 | 49.570 | 0.76x |
| nested.json | msgspec | 0.761 | 0.780 | 1.252 | 49.570 | 0.87x |
| nested.json | ujson | 1.263 | 1.346 | 2.531 | 49.570 | 0.51x |
| nested.json | json | 1.609 | 1.633 | 4.402 | 49.570 | 0.42x |
| wide_arrays.json | strata | 3.875 | 4.067 | 4.665 | 51.504 | 1.00x |
| wide_arrays.json | orjson | 4.906 | 5.166 | 5.913 | 51.504 | 0.79x |
| wide_arrays.json | msgspec | 4.668 | 4.910 | 5.580 | 51.504 | 0.83x |
| wide_arrays.json | ujson | 6.637 | 7.015 | 10.498 | 51.504 | 0.58x |
| wide_arrays.json | json | 9.125 | 9.517 | 15.753 | 51.504 | 0.43x |
| mixed.json | strata | 0.152 | 0.160 | 0.191 | 50.094 | 1.00x |
| mixed.json | orjson | 0.173 | 0.180 | 0.200 | 50.094 | 0.89x |
| mixed.json | msgspec | 0.178 | 0.189 | 0.209 | 50.094 | 0.85x |
| mixed.json | ujson | 0.272 | 0.292 | 0.333 | 50.094 | 0.55x |
| mixed.json | json | 0.362 | 0.369 | 0.463 | 50.094 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.711 | 2.822 | 3.913 | 44.602 | 1.00x |
| users.json | orjson | 2.761 | 2.806 | 3.371 | 44.602 | 1.01x |
| users.json | msgspec | 4.587 | 4.708 | 5.277 | 44.602 | 0.60x |
| users.json | ujson | 12.707 | 12.960 | 14.109 | 44.602 | 0.22x |
| users.json | json | 18.241 | 18.532 | 21.854 | 44.602 | 0.15x |
| flat.json | strata | 0.270 | 0.281 | 0.302 | 50.324 | 1.00x |
| flat.json | orjson | 0.266 | 0.283 | 0.342 | 50.324 | 1.00x |
| flat.json | msgspec | 0.439 | 0.511 | 0.556 | 50.324 | 0.55x |
| flat.json | ujson | 1.285 | 1.344 | 1.463 | 50.324 | 0.21x |
| flat.json | json | 1.543 | 1.606 | 1.809 | 50.324 | 0.18x |
| nested.json | strata | 0.221 | 0.230 | 0.273 | 50.367 | 1.00x |
| nested.json | orjson | 0.257 | 0.262 | 0.278 | 50.367 | 0.88x |
| nested.json | msgspec | 0.408 | 0.420 | 0.474 | 50.367 | 0.55x |
| nested.json | ujson | 1.018 | 1.054 | 1.079 | 50.367 | 0.22x |
| nested.json | json | 1.939 | 1.960 | 2.084 | 50.367 | 0.12x |
| wide_arrays.json | strata | 1.875 | 1.980 | 2.696 | 51.234 | 1.00x |
| wide_arrays.json | orjson | 2.116 | 2.133 | 3.318 | 51.234 | 0.93x |
| wide_arrays.json | msgspec | 3.544 | 3.593 | 4.780 | 51.234 | 0.55x |
| wide_arrays.json | ujson | 7.917 | 8.009 | 9.696 | 51.234 | 0.25x |
| wide_arrays.json | json | 14.458 | 14.772 | 16.632 | 51.234 | 0.13x |
| mixed.json | strata | 0.064 | 0.075 | 0.098 | 50.164 | 1.00x |
| mixed.json | orjson | 0.054 | 0.060 | 0.087 | 50.164 | 1.26x |
| mixed.json | msgspec | 0.081 | 0.092 | 0.197 | 50.164 | 0.82x |
| mixed.json | ujson | 0.254 | 0.264 | 0.540 | 50.164 | 0.29x |
| mixed.json | json | 0.413 | 0.463 | 0.748 | 50.164 | 0.16x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.334 | 10.620 | 15.199 | 54.672 | 1.00x |
| users.json | orjson | 12.730 | 13.295 | 18.638 | 54.672 | 0.80x |
| users.json | msgspec | 11.617 | 12.311 | 18.722 | 54.672 | 0.86x |
| users.json | ujson | 21.043 | 22.423 | 58.609 | 54.672 | 0.47x |
| users.json | json | 18.588 | 19.451 | 30.281 | 54.672 | 0.55x |
| flat.json | strata | 0.993 | 1.076 | 1.210 | 49.863 | 1.00x |
| flat.json | orjson | 1.098 | 1.162 | 1.206 | 49.863 | 0.93x |
| flat.json | msgspec | 1.054 | 1.107 | 1.345 | 49.863 | 0.97x |
| flat.json | ujson | 2.210 | 2.326 | 2.509 | 49.863 | 0.46x |
| flat.json | json | 1.647 | 1.734 | 1.802 | 49.863 | 0.62x |
| nested.json | strata | 0.742 | 0.789 | 0.839 | 50.371 | 1.00x |
| nested.json | orjson | 1.039 | 1.080 | 1.191 | 50.371 | 0.73x |
| nested.json | msgspec | 0.889 | 0.945 | 0.986 | 50.371 | 0.84x |
| nested.json | ujson | 1.531 | 1.584 | 1.691 | 50.371 | 0.50x |
| nested.json | json | 1.755 | 1.796 | 1.874 | 50.371 | 0.44x |
| wide_arrays.json | strata | 4.632 | 4.725 | 5.068 | 51.234 | 1.00x |
| wide_arrays.json | orjson | 5.456 | 5.616 | 6.111 | 51.234 | 0.84x |
| wide_arrays.json | msgspec | 5.401 | 5.568 | 8.302 | 51.234 | 0.85x |
| wide_arrays.json | ujson | 8.754 | 8.976 | 13.717 | 51.234 | 0.53x |
| wide_arrays.json | json | 9.626 | 9.793 | 9.967 | 51.234 | 0.48x |
| mixed.json | strata | 0.224 | 0.252 | 0.338 | 49.254 | 1.00x |
| mixed.json | orjson | 0.336 | 0.365 | 0.391 | 49.254 | 0.69x |
| mixed.json | msgspec | 0.282 | 0.291 | 0.347 | 49.254 | 0.87x |
| mixed.json | ujson | 0.440 | 0.474 | 0.607 | 49.254 | 0.53x |
| mixed.json | json | 0.462 | 0.472 | 0.513 | 49.254 | 0.53x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 11.395 | 12.552 | 18.052 | 51.145 | 1.00x |
| users.ndjson | orjson | 16.396 | 17.461 | 21.895 | 51.145 | 0.72x |
| users.ndjson | msgspec | 15.782 | 17.010 | 18.525 | 51.145 | 0.74x |
| users.ndjson | ujson | 23.823 | 24.788 | 32.452 | 51.145 | 0.51x |
| users.ndjson | json | 26.574 | 27.643 | 32.200 | 51.145 | 0.45x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.622 | 3.731 | 3.889 | 51.848 | 1.00x |
| users.json | orjson | 3.623 | 3.757 | 5.130 | 51.848 | 0.99x |
| users.json | msgspec | 5.347 | 5.595 | 8.927 | 51.848 | 0.67x |
| users.json | ujson | 19.789 | 20.057 | 21.088 | 51.848 | 0.19x |
| users.json | json | 25.494 | 25.870 | 39.501 | 51.848 | 0.14x |
| flat.json | strata | 0.587 | 0.642 | 0.689 | 50.301 | 1.00x |
| flat.json | orjson | 0.576 | 0.613 | 0.703 | 50.301 | 1.05x |
| flat.json | msgspec | 0.738 | 0.780 | 0.979 | 50.301 | 0.82x |
| flat.json | ujson | 2.399 | 2.435 | 2.943 | 50.301 | 0.26x |
| flat.json | json | 2.650 | 2.843 | 3.137 | 50.301 | 0.23x |
| nested.json | strata | 0.588 | 0.663 | 0.913 | 50.371 | 1.00x |
| nested.json | orjson | 0.600 | 0.641 | 0.886 | 50.371 | 1.03x |
| nested.json | msgspec | 0.762 | 0.802 | 1.407 | 50.371 | 0.83x |
| nested.json | ujson | 1.922 | 2.012 | 3.138 | 50.371 | 0.33x |
| nested.json | json | 2.925 | 3.007 | 4.950 | 50.371 | 0.22x |
| wide_arrays.json | strata | 2.539 | 2.665 | 3.072 | 51.234 | 1.00x |
| wide_arrays.json | orjson | 2.691 | 2.760 | 3.016 | 51.234 | 0.97x |
| wide_arrays.json | msgspec | 4.135 | 4.208 | 4.381 | 51.234 | 0.63x |
| wide_arrays.json | ujson | 13.206 | 13.416 | 13.704 | 51.234 | 0.20x |
| wide_arrays.json | json | 19.826 | 20.095 | 33.341 | 51.234 | 0.13x |
| mixed.json | strata | 0.312 | 0.317 | 0.372 | 49.371 | 1.00x |
| mixed.json | orjson | 0.306 | 0.336 | 0.469 | 49.371 | 0.94x |
| mixed.json | msgspec | 0.330 | 0.358 | 0.391 | 49.371 | 0.89x |
| mixed.json | ujson | 0.634 | 0.668 | 0.814 | 49.371 | 0.47x |
| mixed.json | json | 0.819 | 0.835 | 1.201 | 49.371 | 0.38x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.098 | 0.103 | 0.114 | 51.863 | 1.00x |
| users.json $[*].id | jmespath | 0.338 | 0.344 | 0.489 | 51.863 | 0.30x |
| users.json $[*].id | jsonpath-ng | 1.883 | 2.216 | 2.471 | 51.863 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.409 | 0.452 | 0.578 | 51.871 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.081 | 2.149 | 2.275 | 51.871 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 12.761 | 14.421 | 26.830 | 51.871 | 0.03x |
| users.json $..total | strata | 1.453 | 1.480 | 1.698 | 52.871 | 1.00x |
| users.json $..total | jsonpath-ng | 252.108 | 264.428 | 273.445 | 52.871 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.444 | 3.550 | 5.316 | 51.867 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.038 | 13.134 | 19.818 | 51.867 | 0.27x |
| users.json $[*].id | orjson+jsonpath-ng | 14.461 | 14.910 | 15.428 | 51.867 | 0.24x |
| users.json $[*].orders[*].total | strata | 3.713 | 3.765 | 3.805 | 52.871 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 14.969 | 15.591 | 16.210 | 52.871 | 0.24x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 29.090 | 30.064 | 41.059 | 52.871 | 0.13x |
| users.json $..total | strata | 12.250 | 14.369 | 15.044 | 52.871 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 274.633 | 283.827 | 295.412 | 52.871 | 0.05x |

