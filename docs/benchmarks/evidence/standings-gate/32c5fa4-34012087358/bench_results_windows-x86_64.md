# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 32c5fa4
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: Intel64 Family 6 Model 207 Stepping 2, GenuineIntel
- compiler_flags: clang-cl /std:c++20 /O2 /arch:AVX2 -fprofile-use (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.878 | 8.474 | 14.230 | 45.648 | 1.00x |
| users.json | orjson | 14.117 | 14.479 | 20.758 | 45.648 | 0.59x |
| users.json | msgspec | 13.169 | 13.415 | 21.243 | 45.648 | 0.63x |
| users.json | ujson | 18.319 | 19.210 | 32.580 | 45.648 | 0.44x |
| users.json | json | 21.456 | 22.399 | 26.887 | 45.648 | 0.38x |
| flat.json | strata | 1.044 | 1.089 | 1.513 | 48.930 | 1.00x |
| flat.json | orjson | 1.276 | 1.321 | 1.932 | 48.930 | 0.82x |
| flat.json | msgspec | 1.067 | 1.143 | 1.776 | 48.930 | 0.95x |
| flat.json | ujson | 1.593 | 1.683 | 2.569 | 48.930 | 0.65x |
| flat.json | json | 1.869 | 1.964 | 3.282 | 48.930 | 0.55x |
| nested.json | strata | 0.606 | 0.660 | 1.086 | 48.734 | 1.00x |
| nested.json | orjson | 1.016 | 1.096 | 1.588 | 48.734 | 0.60x |
| nested.json | msgspec | 0.805 | 0.854 | 1.384 | 48.734 | 0.77x |
| nested.json | ujson | 1.223 | 1.286 | 2.384 | 48.734 | 0.51x |
| nested.json | json | 1.833 | 1.860 | 3.382 | 48.734 | 0.35x |
| wide_arrays.json | strata | 3.581 | 3.691 | 5.941 | 50.879 | 1.00x |
| wide_arrays.json | orjson | 6.184 | 6.323 | 9.478 | 50.879 | 0.58x |
| wide_arrays.json | msgspec | 5.747 | 5.804 | 8.957 | 50.879 | 0.64x |
| wide_arrays.json | ujson | 7.696 | 7.895 | 10.749 | 50.879 | 0.47x |
| wide_arrays.json | json | 11.155 | 11.401 | 11.617 | 50.879 | 0.32x |
| mixed.json | strata | 0.166 | 0.170 | 0.176 | 48.773 | 1.00x |
| mixed.json | orjson | 0.204 | 0.207 | 0.232 | 48.773 | 0.82x |
| mixed.json | msgspec | 0.223 | 0.231 | 0.261 | 48.773 | 0.74x |
| mixed.json | ujson | 0.289 | 0.294 | 0.324 | 48.773 | 0.58x |
| mixed.json | json | 0.443 | 0.450 | 0.515 | 48.773 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.625 | 2.680 | 4.009 | 44.875 | 1.00x |
| users.json | orjson | 3.429 | 3.490 | 4.759 | 44.875 | 0.77x |
| users.json | msgspec | 5.882 | 5.987 | 8.202 | 44.875 | 0.45x |
| users.json | ujson | 12.807 | 12.960 | 18.881 | 44.875 | 0.21x |
| users.json | json | 22.540 | 23.097 | 31.155 | 44.875 | 0.12x |
| flat.json | strata | 0.285 | 0.292 | 0.317 | 49.391 | 1.00x |
| flat.json | orjson | 0.321 | 0.366 | 0.480 | 49.391 | 0.80x |
| flat.json | msgspec | 0.498 | 0.533 | 0.594 | 49.391 | 0.55x |
| flat.json | ujson | 1.078 | 1.150 | 1.256 | 49.391 | 0.25x |
| flat.json | json | 1.798 | 1.839 | 2.151 | 49.391 | 0.16x |
| nested.json | strata | 0.214 | 0.219 | 0.261 | 49.195 | 1.00x |
| nested.json | orjson | 0.290 | 0.319 | 0.418 | 49.195 | 0.69x |
| nested.json | msgspec | 0.428 | 0.447 | 0.529 | 49.195 | 0.49x |
| nested.json | ujson | 0.934 | 0.992 | 1.021 | 49.195 | 0.22x |
| nested.json | json | 2.226 | 2.255 | 4.201 | 49.195 | 0.10x |
| wide_arrays.json | strata | 1.979 | 2.054 | 3.016 | 52.258 | 1.00x |
| wide_arrays.json | orjson | 2.358 | 2.821 | 3.530 | 52.258 | 0.73x |
| wide_arrays.json | msgspec | 3.827 | 4.241 | 5.879 | 52.258 | 0.48x |
| wide_arrays.json | ujson | 7.572 | 7.891 | 8.144 | 52.258 | 0.26x |
| wide_arrays.json | json | 15.919 | 16.826 | 25.973 | 52.258 | 0.12x |
| mixed.json | strata | 0.063 | 0.069 | 0.100 | 48.715 | 1.00x |
| mixed.json | orjson | 0.067 | 0.070 | 0.122 | 48.715 | 0.98x |
| mixed.json | msgspec | 0.088 | 0.096 | 0.137 | 48.715 | 0.72x |
| mixed.json | ujson | 0.224 | 0.273 | 0.373 | 48.715 | 0.25x |
| mixed.json | json | 0.507 | 0.561 | 0.931 | 48.715 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.309 | 9.614 | 16.185 | 54.906 | 1.00x |
| users.json | orjson | 15.406 | 15.573 | 21.430 | 54.906 | 0.62x |
| users.json | msgspec | 14.355 | 14.525 | 22.374 | 54.906 | 0.66x |
| users.json | ujson | 22.221 | 24.842 | 31.923 | 54.906 | 0.39x |
| users.json | json | 22.906 | 23.341 | 27.244 | 54.906 | 0.41x |
| flat.json | strata | 1.260 | 1.378 | 1.461 | 48.914 | 1.00x |
| flat.json | orjson | 1.518 | 1.854 | 2.048 | 48.914 | 0.74x |
| flat.json | msgspec | 1.338 | 1.380 | 1.460 | 48.914 | 1.00x |
| flat.json | ujson | 2.195 | 2.235 | 2.313 | 48.914 | 0.62x |
| flat.json | json | 2.252 | 2.282 | 2.309 | 48.914 | 0.60x |
| nested.json | strata | 0.719 | 0.765 | 1.228 | 48.914 | 1.00x |
| nested.json | orjson | 1.194 | 1.265 | 1.755 | 48.914 | 0.60x |
| nested.json | msgspec | 0.957 | 0.996 | 1.544 | 48.914 | 0.77x |
| nested.json | ujson | 1.555 | 1.606 | 2.503 | 48.914 | 0.48x |
| nested.json | json | 1.982 | 1.998 | 2.150 | 48.914 | 0.38x |
| wide_arrays.json | strata | 4.410 | 4.549 | 7.195 | 52.258 | 1.00x |
| wide_arrays.json | orjson | 6.995 | 7.074 | 7.661 | 52.258 | 0.64x |
| wide_arrays.json | msgspec | 6.548 | 6.653 | 6.788 | 52.258 | 0.68x |
| wide_arrays.json | ujson | 10.563 | 10.694 | 11.897 | 52.258 | 0.43x |
| wide_arrays.json | json | 11.956 | 12.124 | 19.639 | 52.258 | 0.38x |
| mixed.json | strata | 0.255 | 0.266 | 0.340 | 48.777 | 1.00x |
| mixed.json | orjson | 0.344 | 0.443 | 0.563 | 48.777 | 0.60x |
| mixed.json | msgspec | 0.348 | 0.389 | 0.538 | 48.777 | 0.68x |
| mixed.json | ujson | 0.461 | 0.502 | 0.765 | 48.777 | 0.53x |
| mixed.json | json | 0.566 | 0.602 | 0.961 | 48.777 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.458 | 10.389 | 16.582 | 49.961 | 1.00x |
| users.ndjson | orjson | 17.312 | 17.947 | 25.119 | 49.961 | 0.58x |
| users.ndjson | msgspec | 17.086 | 17.444 | 23.450 | 49.961 | 0.60x |
| users.ndjson | ujson | 21.888 | 22.539 | 24.878 | 49.961 | 0.46x |
| users.ndjson | json | 27.797 | 28.703 | 34.977 | 49.961 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.730 | 3.900 | 4.153 | 51.020 | 1.00x |
| users.json | orjson | 4.504 | 4.775 | 6.085 | 51.020 | 0.82x |
| users.json | msgspec | 6.933 | 7.332 | 18.512 | 51.020 | 0.53x |
| users.json | ujson | 20.506 | 21.543 | 31.320 | 51.020 | 0.18x |
| users.json | json | 30.145 | 31.140 | 39.168 | 51.020 | 0.13x |
| flat.json | strata | 0.660 | 0.718 | 0.884 | 49.188 | 1.00x |
| flat.json | orjson | 0.701 | 0.747 | 1.026 | 49.188 | 0.96x |
| flat.json | msgspec | 0.879 | 0.940 | 1.137 | 49.188 | 0.76x |
| flat.json | ujson | 2.264 | 2.360 | 3.695 | 49.188 | 0.30x |
| flat.json | json | 3.042 | 3.204 | 4.473 | 49.188 | 0.22x |
| nested.json | strata | 0.579 | 0.646 | 0.873 | 49.289 | 1.00x |
| nested.json | orjson | 0.679 | 0.753 | 0.933 | 49.289 | 0.86x |
| nested.json | msgspec | 0.822 | 0.887 | 1.277 | 49.289 | 0.73x |
| nested.json | ujson | 1.943 | 1.962 | 3.237 | 49.289 | 0.33x |
| nested.json | json | 3.139 | 3.198 | 5.946 | 49.289 | 0.20x |
| wide_arrays.json | strata | 2.906 | 2.949 | 3.229 | 49.734 | 1.00x |
| wide_arrays.json | orjson | 3.117 | 3.583 | 4.619 | 49.734 | 0.82x |
| wide_arrays.json | msgspec | 4.782 | 5.225 | 6.220 | 49.734 | 0.56x |
| wide_arrays.json | ujson | 13.574 | 13.896 | 17.184 | 49.734 | 0.21x |
| wide_arrays.json | json | 22.095 | 23.190 | 37.482 | 49.734 | 0.13x |
| mixed.json | strata | 0.364 | 0.383 | 1.539 | 48.840 | 1.00x |
| mixed.json | orjson | 0.369 | 0.387 | 0.463 | 48.840 | 0.99x |
| mixed.json | msgspec | 0.396 | 0.424 | 0.510 | 48.840 | 0.90x |
| mixed.json | ujson | 0.667 | 0.711 | 1.003 | 48.840 | 0.54x |
| mixed.json | json | 0.950 | 0.996 | 3.995 | 48.840 | 0.38x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.086 | 0.090 | 0.110 | 51.055 | 1.00x |
| users.json $[*].id | jmespath | 0.382 | 0.411 | 0.458 | 51.055 | 0.22x |
| users.json $[*].id | jsonpath-ng | 2.076 | 2.213 | 2.266 | 51.055 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.484 | 0.496 | 0.654 | 51.070 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.364 | 2.412 | 4.470 | 51.070 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 14.221 | 14.612 | 26.161 | 51.070 | 0.03x |
| users.json $..total | strata | 1.859 | 1.924 | 2.894 | 53.070 | 1.00x |
| users.json $..total | jsonpath-ng | 288.468 | 298.554 | 321.836 | 53.070 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.065 | 4.204 | 5.790 | 51.070 | 1.00x |
| users.json $[*].id | orjson+jmespath | 16.291 | 16.769 | 23.824 | 51.070 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 17.765 | 18.260 | 19.519 | 51.070 | 0.23x |
| users.json $[*].orders[*].total | strata | 4.230 | 4.387 | 5.918 | 53.070 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.689 | 18.891 | 19.245 | 53.070 | 0.23x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 35.873 | 36.673 | 50.397 | 53.070 | 0.12x |
| users.json $..total | strata | 12.589 | 13.091 | 19.488 | 53.070 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 305.891 | 327.489 | 342.890 | 53.070 | 0.04x |

