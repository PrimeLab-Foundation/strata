# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 2f12155
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: Intel64 Family 6 Model 207 Stepping 2, GenuineIntel
- compiler_flags: /std:c++20 /O2 /arch:AVX2 /GL /LTCG /USEPROFILE (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.027 | 9.306 | 13.293 | 45.492 | 1.00x |
| users.json | orjson | 14.665 | 14.898 | 21.341 | 45.492 | 0.62x |
| users.json | msgspec | 13.571 | 13.723 | 19.855 | 45.492 | 0.68x |
| users.json | ujson | 19.607 | 20.641 | 27.342 | 45.492 | 0.45x |
| users.json | json | 22.638 | 23.149 | 43.013 | 45.492 | 0.40x |
| flat.json | strata | 1.055 | 1.081 | 1.678 | 50.035 | 1.00x |
| flat.json | orjson | 1.283 | 1.338 | 1.947 | 50.035 | 0.81x |
| flat.json | msgspec | 1.121 | 1.155 | 1.747 | 50.035 | 0.94x |
| flat.json | ujson | 1.733 | 1.779 | 2.843 | 50.035 | 0.61x |
| flat.json | json | 1.938 | 1.967 | 3.310 | 50.035 | 0.55x |
| nested.json | strata | 0.704 | 0.763 | 1.256 | 49.875 | 1.00x |
| nested.json | orjson | 0.982 | 1.037 | 1.596 | 49.875 | 0.74x |
| nested.json | msgspec | 0.846 | 0.870 | 1.453 | 49.875 | 0.88x |
| nested.json | ujson | 1.339 | 1.438 | 2.429 | 49.875 | 0.53x |
| nested.json | json | 1.929 | 1.950 | 4.066 | 49.875 | 0.39x |
| wide_arrays.json | strata | 4.236 | 4.388 | 7.356 | 51.848 | 1.00x |
| wide_arrays.json | orjson | 6.168 | 6.329 | 9.103 | 51.848 | 0.69x |
| wide_arrays.json | msgspec | 5.880 | 6.064 | 6.641 | 51.848 | 0.72x |
| wide_arrays.json | ujson | 8.008 | 8.175 | 9.131 | 51.848 | 0.54x |
| wide_arrays.json | json | 11.368 | 11.651 | 12.133 | 51.848 | 0.38x |
| mixed.json | strata | 0.308 | 0.317 | 0.386 | 50.430 | 1.00x |
| mixed.json | orjson | 0.217 | 0.360 | 0.668 | 50.430 | 0.88x |
| mixed.json | msgspec | 0.368 | 0.405 | 0.447 | 50.430 | 0.78x |
| mixed.json | ujson | 0.410 | 0.524 | 1.898 | 50.430 | 0.60x |
| mixed.json | json | 0.742 | 0.792 | 0.954 | 50.430 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.945 | 3.015 | 4.719 | 45.270 | 1.00x |
| users.json | orjson | 3.252 | 3.598 | 4.304 | 45.270 | 0.84x |
| users.json | msgspec | 5.485 | 5.703 | 8.064 | 45.270 | 0.53x |
| users.json | ujson | 15.097 | 15.215 | 24.422 | 45.270 | 0.20x |
| users.json | json | 22.859 | 23.489 | 40.991 | 45.270 | 0.13x |
| flat.json | strata | 0.340 | 0.370 | 0.517 | 50.453 | 1.00x |
| flat.json | orjson | 0.336 | 0.397 | 0.528 | 50.453 | 0.93x |
| flat.json | msgspec | 0.527 | 0.572 | 1.123 | 50.453 | 0.65x |
| flat.json | ujson | 1.362 | 1.408 | 2.337 | 50.453 | 0.26x |
| flat.json | json | 1.909 | 2.108 | 3.626 | 50.453 | 0.18x |
| nested.json | strata | 0.224 | 0.231 | 0.284 | 50.359 | 1.00x |
| nested.json | orjson | 0.320 | 0.328 | 0.341 | 50.359 | 0.71x |
| nested.json | msgspec | 0.449 | 0.465 | 0.505 | 50.359 | 0.50x |
| nested.json | ujson | 1.195 | 1.238 | 1.333 | 50.359 | 0.19x |
| nested.json | json | 2.242 | 2.267 | 2.603 | 50.359 | 0.10x |
| wide_arrays.json | strata | 2.182 | 2.205 | 2.883 | 51.566 | 1.00x |
| wide_arrays.json | orjson | 2.552 | 2.597 | 3.690 | 51.566 | 0.85x |
| wide_arrays.json | msgspec | 4.481 | 4.528 | 6.539 | 51.566 | 0.49x |
| wide_arrays.json | ujson | 9.310 | 9.454 | 13.323 | 51.566 | 0.23x |
| wide_arrays.json | json | 16.729 | 17.007 | 20.928 | 51.566 | 0.13x |
| mixed.json | strata | 0.107 | 0.116 | 0.131 | 49.840 | 1.00x |
| mixed.json | orjson | 0.093 | 0.097 | 0.182 | 49.840 | 1.21x |
| mixed.json | msgspec | 0.132 | 0.141 | 0.148 | 49.840 | 0.83x |
| mixed.json | ujson | 0.409 | 0.449 | 0.573 | 49.840 | 0.26x |
| mixed.json | json | 0.821 | 0.907 | 1.144 | 49.840 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.411 | 12.720 | 30.057 | 55.301 | 1.00x |
| users.json | orjson | 16.045 | 20.260 | 43.282 | 55.301 | 0.63x |
| users.json | msgspec | 14.960 | 17.023 | 24.799 | 55.301 | 0.75x |
| users.json | ujson | 23.873 | 25.402 | 114.747 | 55.301 | 0.50x |
| users.json | json | 24.011 | 26.498 | 57.260 | 55.301 | 0.48x |
| flat.json | strata | 1.175 | 1.239 | 1.330 | 50.062 | 1.00x |
| flat.json | orjson | 1.439 | 1.504 | 1.533 | 50.062 | 0.82x |
| flat.json | msgspec | 1.276 | 1.350 | 1.427 | 50.062 | 0.92x |
| flat.json | ujson | 2.147 | 2.252 | 2.401 | 50.062 | 0.55x |
| flat.json | json | 2.101 | 2.123 | 2.162 | 50.062 | 0.58x |
| nested.json | strata | 0.803 | 0.847 | 1.375 | 50.363 | 1.00x |
| nested.json | orjson | 1.145 | 1.185 | 1.797 | 50.363 | 0.71x |
| nested.json | msgspec | 0.990 | 1.028 | 1.656 | 50.363 | 0.82x |
| nested.json | ujson | 1.613 | 1.700 | 3.097 | 50.363 | 0.50x |
| nested.json | json | 2.059 | 2.101 | 3.557 | 50.363 | 0.40x |
| wide_arrays.json | strata | 5.008 | 5.129 | 5.543 | 51.566 | 1.00x |
| wide_arrays.json | orjson | 6.763 | 6.852 | 7.433 | 51.566 | 0.75x |
| wide_arrays.json | msgspec | 6.640 | 6.766 | 9.857 | 51.566 | 0.76x |
| wide_arrays.json | ujson | 10.623 | 10.749 | 15.623 | 51.566 | 0.48x |
| wide_arrays.json | json | 12.076 | 12.332 | 15.938 | 51.566 | 0.42x |
| mixed.json | strata | 0.429 | 0.435 | 0.503 | 49.844 | 1.00x |
| mixed.json | orjson | 0.503 | 0.571 | 48.323 | 49.844 | 0.76x |
| mixed.json | msgspec | 0.539 | 0.591 | 0.756 | 49.844 | 0.74x |
| mixed.json | ujson | 0.746 | 0.807 | 0.897 | 49.844 | 0.54x |
| mixed.json | json | 0.961 | 1.007 | 1.091 | 49.844 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.897 | 11.338 | 17.537 | 51.156 | 1.00x |
| users.ndjson | orjson | 17.947 | 18.225 | 27.921 | 51.156 | 0.62x |
| users.ndjson | msgspec | 17.622 | 17.972 | 19.720 | 51.156 | 0.63x |
| users.ndjson | ujson | 23.385 | 24.030 | 36.276 | 51.156 | 0.47x |
| users.ndjson | json | 29.367 | 29.955 | 32.407 | 51.156 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.151 | 4.712 | 13.260 | 50.383 | 1.00x |
| users.json | orjson | 4.708 | 5.082 | 6.324 | 50.383 | 0.93x |
| users.json | msgspec | 6.736 | 7.330 | 15.140 | 50.383 | 0.64x |
| users.json | ujson | 23.450 | 36.158 | 342.815 | 50.383 | 0.13x |
| users.json | json | 31.512 | 41.955 | 58.799 | 50.383 | 0.11x |
| flat.json | strata | 0.714 | 0.751 | 0.788 | 50.660 | 1.00x |
| flat.json | orjson | 0.709 | 0.732 | 3.988 | 50.660 | 1.03x |
| flat.json | msgspec | 0.906 | 0.958 | 0.976 | 50.660 | 0.78x |
| flat.json | ujson | 2.509 | 2.554 | 6.925 | 50.660 | 0.29x |
| flat.json | json | 3.091 | 3.164 | 3.587 | 50.660 | 0.24x |
| nested.json | strata | 0.596 | 0.629 | 0.680 | 50.363 | 1.00x |
| nested.json | orjson | 0.704 | 0.725 | 0.798 | 50.363 | 0.87x |
| nested.json | msgspec | 0.839 | 0.898 | 0.915 | 50.363 | 0.70x |
| nested.json | ujson | 2.221 | 2.253 | 2.448 | 50.363 | 0.28x |
| nested.json | json | 3.278 | 3.328 | 3.376 | 50.363 | 0.19x |
| wide_arrays.json | strata | 3.139 | 4.417 | 4.822 | 51.574 | 1.00x |
| wide_arrays.json | orjson | 3.675 | 4.750 | 8.434 | 51.574 | 0.93x |
| wide_arrays.json | msgspec | 5.422 | 7.125 | 10.776 | 51.574 | 0.62x |
| wide_arrays.json | ujson | 23.223 | 23.581 | 25.120 | 51.574 | 0.19x |
| wide_arrays.json | json | 37.682 | 39.600 | 97.138 | 51.574 | 0.11x |
| mixed.json | strata | 0.404 | 0.446 | 0.580 | 50.113 | 1.00x |
| mixed.json | orjson | 0.394 | 0.420 | 0.523 | 50.113 | 1.06x |
| mixed.json | msgspec | 0.418 | 0.461 | 0.641 | 50.113 | 0.97x |
| mixed.json | ujson | 0.744 | 0.793 | 0.995 | 50.113 | 0.56x |
| mixed.json | json | 0.984 | 1.043 | 1.721 | 50.113 | 0.43x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.092 | 0.095 | 0.132 | 50.680 | 1.00x |
| users.json $[*].id | jmespath | 0.402 | 0.408 | 0.809 | 50.680 | 0.23x |
| users.json $[*].id | jsonpath-ng | 2.179 | 2.360 | 3.908 | 50.680 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.526 | 0.538 | 0.561 | 50.918 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.439 | 2.494 | 4.636 | 50.918 | 0.22x |
| users.json $[*].orders[*].total | jsonpath-ng | 15.206 | 15.892 | 27.549 | 50.918 | 0.03x |
| users.json $..total | strata | 1.795 | 1.819 | 1.912 | 52.965 | 1.00x |
| users.json $..total | jsonpath-ng | 293.816 | 318.171 | 344.120 | 52.965 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.165 | 4.329 | 6.195 | 50.691 | 1.00x |
| users.json $[*].id | orjson+jmespath | 16.359 | 17.073 | 25.385 | 50.691 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 17.795 | 18.200 | 21.238 | 50.691 | 0.24x |
| users.json $[*].orders[*].total | strata | 4.282 | 4.531 | 4.824 | 52.961 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.371 | 19.102 | 27.869 | 52.961 | 0.24x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 35.592 | 36.509 | 51.035 | 52.961 | 0.12x |
| users.json $..total | strata | 14.892 | 15.379 | 21.012 | 52.965 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 314.816 | 341.630 | 409.612 | 52.965 | 0.05x |

