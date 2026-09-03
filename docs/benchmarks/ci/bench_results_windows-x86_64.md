# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: ab20434
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD
- compiler_flags: clang-cl /std:c++20 /O2 /arch:AVX2 -fprofile-use (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.603 | 10.573 | 16.978 | 45.410 | 1.00x |
| users.json | orjson | 13.816 | 14.459 | 22.628 | 45.410 | 0.73x |
| users.json | msgspec | 12.556 | 13.181 | 15.041 | 45.410 | 0.80x |
| users.json | ujson | 20.893 | 22.555 | 28.809 | 45.410 | 0.47x |
| users.json | json | 23.017 | 23.845 | 42.978 | 45.410 | 0.44x |
| flat.json | strata | 0.834 | 0.857 | 0.965 | 51.336 | 1.00x |
| flat.json | orjson | 1.104 | 1.146 | 1.170 | 51.336 | 0.75x |
| flat.json | msgspec | 1.080 | 1.103 | 1.150 | 51.336 | 0.78x |
| flat.json | ujson | 2.042 | 2.058 | 2.106 | 51.336 | 0.42x |
| flat.json | json | 2.005 | 2.019 | 2.024 | 51.336 | 0.42x |
| nested.json | strata | 0.760 | 0.801 | 1.247 | 50.613 | 1.00x |
| nested.json | orjson | 1.042 | 1.089 | 1.649 | 50.613 | 0.74x |
| nested.json | msgspec | 0.982 | 1.016 | 1.698 | 50.613 | 0.79x |
| nested.json | ujson | 1.554 | 1.592 | 2.815 | 50.613 | 0.50x |
| nested.json | json | 2.121 | 2.127 | 3.953 | 50.613 | 0.38x |
| wide_arrays.json | strata | 4.359 | 4.410 | 6.478 | 52.789 | 1.00x |
| wide_arrays.json | orjson | 5.504 | 5.578 | 5.714 | 52.789 | 0.79x |
| wide_arrays.json | msgspec | 5.751 | 5.799 | 6.047 | 52.789 | 0.76x |
| wide_arrays.json | ujson | 8.259 | 8.316 | 13.623 | 52.789 | 0.53x |
| wide_arrays.json | json | 11.520 | 11.615 | 23.570 | 52.789 | 0.38x |
| mixed.json | strata | 0.186 | 0.208 | 0.309 | 51.848 | 1.00x |
| mixed.json | orjson | 0.210 | 0.226 | 0.378 | 51.848 | 0.92x |
| mixed.json | msgspec | 0.231 | 0.243 | 0.406 | 51.848 | 0.86x |
| mixed.json | ujson | 0.354 | 0.362 | 0.404 | 51.848 | 0.58x |
| mixed.json | json | 0.467 | 0.497 | 0.856 | 51.848 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.302 | 3.365 | 4.535 | 44.145 | 1.00x |
| users.json | orjson | 3.763 | 4.112 | 6.616 | 44.145 | 0.82x |
| users.json | msgspec | 5.368 | 5.866 | 9.025 | 44.145 | 0.57x |
| users.json | ujson | 17.538 | 17.831 | 27.713 | 44.145 | 0.19x |
| users.json | json | 24.114 | 25.715 | 38.040 | 44.145 | 0.13x |
| flat.json | strata | 0.294 | 0.328 | 0.382 | 50.977 | 1.00x |
| flat.json | orjson | 0.352 | 0.394 | 0.507 | 50.977 | 0.83x |
| flat.json | msgspec | 0.495 | 0.519 | 1.009 | 50.977 | 0.63x |
| flat.json | ujson | 1.772 | 1.842 | 2.781 | 50.977 | 0.18x |
| flat.json | json | 1.931 | 2.147 | 3.645 | 50.977 | 0.15x |
| nested.json | strata | 0.283 | 0.285 | 0.317 | 51.234 | 1.00x |
| nested.json | orjson | 0.322 | 0.325 | 0.372 | 51.234 | 0.88x |
| nested.json | msgspec | 0.460 | 0.469 | 0.503 | 51.234 | 0.61x |
| nested.json | ujson | 1.320 | 1.372 | 1.397 | 51.234 | 0.21x |
| nested.json | json | 2.429 | 2.443 | 2.544 | 51.234 | 0.12x |
| wide_arrays.json | strata | 1.924 | 1.941 | 2.786 | 51.609 | 1.00x |
| wide_arrays.json | orjson | 2.272 | 2.417 | 2.689 | 51.609 | 0.80x |
| wide_arrays.json | msgspec | 3.340 | 3.823 | 4.062 | 51.609 | 0.51x |
| wide_arrays.json | ujson | 9.483 | 9.674 | 11.988 | 51.609 | 0.20x |
| wide_arrays.json | json | 17.765 | 18.137 | 29.978 | 51.609 | 0.11x |
| mixed.json | strata | 0.071 | 0.073 | 0.103 | 50.617 | 1.00x |
| mixed.json | orjson | 0.068 | 0.069 | 0.108 | 50.617 | 1.05x |
| mixed.json | msgspec | 0.091 | 0.092 | 0.143 | 50.617 | 0.79x |
| mixed.json | ujson | 0.326 | 0.346 | 0.732 | 50.617 | 0.21x |
| mixed.json | json | 0.501 | 0.528 | 1.271 | 50.617 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.875 | 11.162 | 11.880 | 54.273 | 1.00x |
| users.json | orjson | 14.996 | 15.611 | 17.037 | 54.273 | 0.72x |
| users.json | msgspec | 14.181 | 14.294 | 14.620 | 54.273 | 0.78x |
| users.json | ujson | 26.419 | 27.045 | 29.123 | 54.273 | 0.41x |
| users.json | json | 23.842 | 24.859 | 42.479 | 54.273 | 0.45x |
| flat.json | strata | 1.098 | 1.157 | 1.236 | 50.504 | 1.00x |
| flat.json | orjson | 1.241 | 1.279 | 1.366 | 50.504 | 0.90x |
| flat.json | msgspec | 1.220 | 1.269 | 1.308 | 50.504 | 0.91x |
| flat.json | ujson | 2.686 | 2.713 | 2.895 | 50.504 | 0.43x |
| flat.json | json | 2.238 | 2.250 | 2.287 | 50.504 | 0.51x |
| nested.json | strata | 0.848 | 0.861 | 1.028 | 50.824 | 1.00x |
| nested.json | orjson | 1.161 | 1.198 | 1.223 | 50.824 | 0.72x |
| nested.json | msgspec | 1.108 | 1.130 | 1.180 | 50.824 | 0.76x |
| nested.json | ujson | 1.987 | 2.003 | 2.139 | 50.824 | 0.43x |
| nested.json | json | 2.244 | 2.261 | 2.419 | 50.824 | 0.38x |
| wide_arrays.json | strata | 4.840 | 4.925 | 5.279 | 51.609 | 1.00x |
| wide_arrays.json | orjson | 5.989 | 6.027 | 6.462 | 51.609 | 0.82x |
| wide_arrays.json | msgspec | 6.270 | 6.371 | 10.201 | 51.609 | 0.77x |
| wide_arrays.json | ujson | 11.338 | 11.402 | 16.923 | 51.609 | 0.43x |
| wide_arrays.json | json | 12.039 | 12.127 | 18.883 | 51.609 | 0.41x |
| mixed.json | strata | 0.262 | 0.264 | 0.310 | 50.527 | 1.00x |
| mixed.json | orjson | 0.319 | 0.348 | 0.428 | 50.527 | 0.76x |
| mixed.json | msgspec | 0.341 | 0.358 | 0.378 | 50.527 | 0.74x |
| mixed.json | ujson | 0.535 | 0.558 | 0.641 | 50.527 | 0.47x |
| mixed.json | json | 0.575 | 0.597 | 0.650 | 50.527 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.190 | 10.451 | 16.842 | 51.918 | 1.00x |
| users.ndjson | orjson | 16.492 | 16.850 | 26.188 | 51.918 | 0.62x |
| users.ndjson | msgspec | 16.783 | 16.995 | 27.167 | 51.918 | 0.61x |
| users.ndjson | ujson | 24.673 | 24.907 | 28.993 | 51.918 | 0.42x |
| users.ndjson | json | 28.894 | 29.281 | 29.775 | 51.918 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.157 | 4.311 | 5.037 | 51.453 | 1.00x |
| users.json | orjson | 4.580 | 4.661 | 5.078 | 51.453 | 0.92x |
| users.json | msgspec | 6.123 | 6.540 | 8.482 | 51.453 | 0.66x |
| users.json | ujson | 27.372 | 27.683 | 45.125 | 51.453 | 0.16x |
| users.json | json | 33.024 | 34.222 | 45.557 | 51.453 | 0.13x |
| flat.json | strata | 0.628 | 0.659 | 0.687 | 51.102 | 1.00x |
| flat.json | orjson | 0.743 | 0.756 | 0.809 | 51.102 | 0.87x |
| flat.json | msgspec | 0.871 | 0.913 | 5.793 | 51.102 | 0.72x |
| flat.json | ujson | 3.162 | 3.217 | 3.357 | 51.102 | 0.20x |
| flat.json | json | 3.545 | 3.622 | 3.797 | 51.102 | 0.18x |
| nested.json | strata | 0.615 | 0.641 | 0.908 | 51.219 | 1.00x |
| nested.json | orjson | 0.654 | 0.677 | 0.915 | 51.219 | 0.95x |
| nested.json | msgspec | 0.805 | 0.883 | 1.199 | 51.219 | 0.73x |
| nested.json | ujson | 2.397 | 2.438 | 4.068 | 51.219 | 0.26x |
| nested.json | json | 3.445 | 3.496 | 6.062 | 51.219 | 0.18x |
| wide_arrays.json | strata | 2.552 | 2.601 | 2.637 | 52.965 | 1.00x |
| wide_arrays.json | orjson | 2.884 | 3.047 | 3.211 | 52.965 | 0.85x |
| wide_arrays.json | msgspec | 4.293 | 4.574 | 5.717 | 52.965 | 0.57x |
| wide_arrays.json | ujson | 16.267 | 16.440 | 27.113 | 52.965 | 0.16x |
| wide_arrays.json | json | 24.468 | 24.862 | 34.695 | 52.965 | 0.10x |
| mixed.json | strata | 0.372 | 0.387 | 0.447 | 50.648 | 1.00x |
| mixed.json | orjson | 0.370 | 0.380 | 0.426 | 50.648 | 1.02x |
| mixed.json | msgspec | 0.393 | 0.400 | 0.452 | 50.648 | 0.97x |
| mixed.json | ujson | 0.801 | 0.822 | 0.857 | 50.648 | 0.47x |
| mixed.json | json | 0.975 | 1.000 | 1.045 | 50.648 | 0.39x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.082 | 0.084 | 0.131 | 50.492 | 1.00x |
| users.json $[*].id | jmespath | 0.432 | 0.439 | 0.471 | 50.492 | 0.19x |
| users.json $[*].id | jsonpath-ng | 2.441 | 2.629 | 2.716 | 50.492 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.455 | 0.462 | 0.620 | 50.770 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.748 | 2.912 | 3.153 | 50.770 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.081 | 17.412 | 33.100 | 50.770 | 0.03x |
| users.json $..total | strata | 1.903 | 2.012 | 2.737 | 52.770 | 1.00x |
| users.json $..total | jsonpath-ng | 351.531 | 378.394 | 393.379 | 52.770 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.212 | 4.515 | 5.420 | 50.527 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.710 | 15.943 | 17.507 | 50.527 | 0.28x |
| users.json $[*].id | orjson+jsonpath-ng | 18.012 | 18.177 | 30.442 | 50.527 | 0.25x |
| users.json $[*].orders[*].total | strata | 4.421 | 4.520 | 7.069 | 52.770 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.358 | 18.981 | 27.229 | 52.770 | 0.24x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 36.171 | 36.969 | 45.041 | 52.770 | 0.12x |
| users.json $..total | strata | 13.495 | 13.932 | 21.005 | 52.770 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 345.170 | 371.075 | 449.656 | 52.770 | 0.04x |

