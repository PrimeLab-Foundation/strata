# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: a4d78c1
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD
- compiler_flags: /std:c++20 /O2 /arch:AVX2 /GL /LTCG /USEPROFILE (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.777 | 11.447 | 14.601 | 45.480 | 1.00x |
| users.json | orjson | 14.466 | 14.836 | 16.723 | 45.480 | 0.77x |
| users.json | msgspec | 13.416 | 13.721 | 16.493 | 45.480 | 0.83x |
| users.json | ujson | 22.199 | 23.900 | 49.010 | 45.480 | 0.48x |
| users.json | json | 24.388 | 25.071 | 45.667 | 45.480 | 0.46x |
| flat.json | strata | 1.149 | 1.202 | 1.418 | 50.004 | 1.00x |
| flat.json | orjson | 1.153 | 1.181 | 1.257 | 50.004 | 1.02x |
| flat.json | msgspec | 1.142 | 1.236 | 1.398 | 50.004 | 0.97x |
| flat.json | ujson | 2.249 | 2.352 | 2.573 | 50.004 | 0.51x |
| flat.json | json | 2.021 | 2.038 | 3.290 | 50.004 | 0.59x |
| nested.json | strata | 0.884 | 0.909 | 1.497 | 50.152 | 1.00x |
| nested.json | orjson | 1.155 | 1.199 | 2.127 | 50.152 | 0.76x |
| nested.json | msgspec | 1.085 | 1.125 | 1.753 | 50.152 | 0.81x |
| nested.json | ujson | 1.671 | 1.689 | 3.137 | 50.152 | 0.54x |
| nested.json | json | 2.229 | 2.270 | 4.000 | 50.152 | 0.40x |
| wide_arrays.json | strata | 4.873 | 5.058 | 9.063 | 52.340 | 1.00x |
| wide_arrays.json | orjson | 5.814 | 6.194 | 11.528 | 52.340 | 0.82x |
| wide_arrays.json | msgspec | 6.078 | 6.282 | 13.444 | 52.340 | 0.81x |
| wide_arrays.json | ujson | 8.350 | 8.962 | 13.386 | 52.340 | 0.56x |
| wide_arrays.json | json | 12.114 | 12.541 | 24.251 | 52.340 | 0.40x |
| mixed.json | strata | 0.212 | 0.217 | 0.370 | 50.125 | 1.00x |
| mixed.json | orjson | 0.223 | 0.225 | 0.399 | 50.125 | 0.96x |
| mixed.json | msgspec | 0.246 | 0.254 | 0.419 | 50.125 | 0.85x |
| mixed.json | ujson | 0.375 | 0.390 | 0.619 | 50.125 | 0.56x |
| mixed.json | json | 0.488 | 0.518 | 0.862 | 50.125 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.645 | 3.695 | 5.657 | 44.773 | 1.00x |
| users.json | orjson | 3.609 | 3.848 | 5.591 | 44.773 | 0.96x |
| users.json | msgspec | 4.990 | 5.353 | 5.588 | 44.773 | 0.69x |
| users.json | ujson | 16.343 | 16.450 | 23.277 | 44.773 | 0.22x |
| users.json | json | 24.846 | 25.340 | 34.763 | 44.773 | 0.15x |
| flat.json | strata | 0.325 | 0.333 | 0.372 | 50.711 | 1.00x |
| flat.json | orjson | 0.355 | 0.364 | 0.441 | 50.711 | 0.91x |
| flat.json | msgspec | 0.498 | 0.503 | 0.662 | 50.711 | 0.66x |
| flat.json | ujson | 1.709 | 1.744 | 1.942 | 50.711 | 0.19x |
| flat.json | json | 1.976 | 2.006 | 2.175 | 50.711 | 0.17x |
| nested.json | strata | 0.299 | 0.306 | 0.349 | 50.551 | 1.00x |
| nested.json | orjson | 0.322 | 0.329 | 0.372 | 50.551 | 0.93x |
| nested.json | msgspec | 0.472 | 0.487 | 0.550 | 50.551 | 0.63x |
| nested.json | ujson | 1.270 | 1.323 | 1.466 | 50.551 | 0.23x |
| nested.json | json | 2.466 | 2.480 | 2.556 | 50.551 | 0.12x |
| wide_arrays.json | strata | 2.562 | 2.579 | 2.691 | 51.727 | 1.00x |
| wide_arrays.json | orjson | 2.482 | 2.534 | 3.790 | 51.727 | 1.02x |
| wide_arrays.json | msgspec | 4.051 | 4.150 | 6.410 | 51.727 | 0.62x |
| wide_arrays.json | ujson | 9.723 | 9.816 | 16.004 | 51.727 | 0.26x |
| wide_arrays.json | json | 19.573 | 19.780 | 20.753 | 51.727 | 0.13x |
| mixed.json | strata | 0.080 | 0.084 | 0.092 | 50.238 | 1.00x |
| mixed.json | orjson | 0.069 | 0.070 | 0.074 | 50.238 | 1.20x |
| mixed.json | msgspec | 0.097 | 0.097 | 0.104 | 50.238 | 0.86x |
| mixed.json | ujson | 0.329 | 0.335 | 0.361 | 50.238 | 0.25x |
| mixed.json | json | 0.519 | 0.535 | 0.641 | 50.238 | 0.16x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 12.292 | 12.630 | 20.123 | 54.801 | 1.00x |
| users.json | orjson | 15.235 | 15.558 | 16.889 | 54.801 | 0.81x |
| users.json | msgspec | 14.621 | 15.092 | 19.771 | 54.801 | 0.84x |
| users.json | ujson | 26.938 | 29.103 | 35.875 | 54.801 | 0.43x |
| users.json | json | 24.709 | 25.471 | 32.645 | 54.801 | 0.50x |
| flat.json | strata | 1.157 | 1.267 | 1.855 | 50.359 | 1.00x |
| flat.json | orjson | 1.309 | 1.413 | 1.969 | 50.359 | 0.90x |
| flat.json | msgspec | 1.273 | 1.380 | 2.127 | 50.359 | 0.92x |
| flat.json | ujson | 2.757 | 2.879 | 4.474 | 50.359 | 0.44x |
| flat.json | json | 2.153 | 2.185 | 3.701 | 50.359 | 0.58x |
| nested.json | strata | 0.975 | 1.004 | 1.123 | 50.379 | 1.00x |
| nested.json | orjson | 1.316 | 1.348 | 1.626 | 50.379 | 0.74x |
| nested.json | msgspec | 1.214 | 1.284 | 1.364 | 50.379 | 0.78x |
| nested.json | ujson | 2.048 | 2.149 | 2.195 | 50.379 | 0.47x |
| nested.json | json | 2.379 | 2.424 | 2.465 | 50.379 | 0.41x |
| wide_arrays.json | strata | 5.370 | 5.667 | 9.020 | 51.738 | 1.00x |
| wide_arrays.json | orjson | 6.321 | 6.685 | 7.164 | 51.738 | 0.85x |
| wide_arrays.json | msgspec | 6.871 | 6.963 | 7.468 | 51.738 | 0.81x |
| wide_arrays.json | ujson | 11.848 | 12.015 | 12.450 | 51.738 | 0.47x |
| wide_arrays.json | json | 12.640 | 12.907 | 22.771 | 51.738 | 0.44x |
| mixed.json | strata | 0.289 | 0.293 | 0.335 | 50.238 | 1.00x |
| mixed.json | orjson | 0.335 | 0.340 | 0.373 | 50.238 | 0.86x |
| mixed.json | msgspec | 0.361 | 0.365 | 0.417 | 50.238 | 0.80x |
| mixed.json | ujson | 0.561 | 0.590 | 0.749 | 50.238 | 0.50x |
| mixed.json | json | 0.606 | 0.627 | 0.674 | 50.238 | 0.47x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 13.418 | 14.890 | 17.531 | 51.238 | 1.00x |
| users.ndjson | orjson | 18.974 | 20.575 | 32.132 | 51.238 | 0.72x |
| users.ndjson | msgspec | 19.983 | 20.382 | 33.111 | 51.238 | 0.73x |
| users.ndjson | ujson | 29.378 | 31.167 | 39.630 | 51.238 | 0.48x |
| users.ndjson | json | 33.120 | 34.719 | 35.908 | 51.238 | 0.43x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.613 | 4.756 | 7.568 | 51.453 | 1.00x |
| users.json | orjson | 4.577 | 4.740 | 6.935 | 51.453 | 1.00x |
| users.json | msgspec | 6.328 | 6.608 | 10.067 | 51.453 | 0.72x |
| users.json | ujson | 25.919 | 26.172 | 44.624 | 51.453 | 0.18x |
| users.json | json | 33.121 | 33.928 | 37.805 | 51.453 | 0.14x |
| flat.json | strata | 0.686 | 0.754 | 1.017 | 50.637 | 1.00x |
| flat.json | orjson | 0.710 | 0.756 | 1.004 | 50.637 | 1.00x |
| flat.json | msgspec | 0.854 | 0.925 | 1.086 | 50.637 | 0.81x |
| flat.json | ujson | 3.097 | 3.152 | 3.312 | 50.637 | 0.24x |
| flat.json | json | 3.321 | 3.384 | 3.566 | 50.637 | 0.22x |
| nested.json | strata | 0.698 | 0.751 | 1.111 | 50.492 | 1.00x |
| nested.json | orjson | 0.698 | 0.790 | 1.064 | 50.492 | 0.95x |
| nested.json | msgspec | 0.862 | 0.902 | 1.294 | 50.492 | 0.83x |
| nested.json | ujson | 2.485 | 2.522 | 4.222 | 50.492 | 0.30x |
| nested.json | json | 3.627 | 3.733 | 6.598 | 50.492 | 0.20x |
| wide_arrays.json | strata | 3.368 | 3.424 | 4.781 | 51.656 | 1.00x |
| wide_arrays.json | orjson | 3.244 | 3.287 | 4.591 | 51.656 | 1.04x |
| wide_arrays.json | msgspec | 4.644 | 4.774 | 5.004 | 51.656 | 0.72x |
| wide_arrays.json | ujson | 16.485 | 16.706 | 17.216 | 51.656 | 0.20x |
| wide_arrays.json | json | 26.316 | 26.713 | 37.312 | 51.656 | 0.13x |
| mixed.json | strata | 0.396 | 0.417 | 0.472 | 50.281 | 1.00x |
| mixed.json | orjson | 0.383 | 0.395 | 0.422 | 50.281 | 1.05x |
| mixed.json | msgspec | 0.414 | 0.429 | 0.467 | 50.281 | 0.97x |
| mixed.json | ujson | 0.811 | 0.857 | 0.972 | 50.281 | 0.49x |
| mixed.json | json | 1.021 | 1.070 | 1.106 | 50.281 | 0.39x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.096 | 0.099 | 0.107 | 51.469 | 1.00x |
| users.json $[*].id | jmespath | 0.442 | 0.459 | 0.728 | 51.469 | 0.22x |
| users.json $[*].id | jsonpath-ng | 2.540 | 2.698 | 2.792 | 51.469 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.502 | 0.561 | 0.667 | 51.480 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.796 | 2.920 | 5.145 | 51.480 | 0.19x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.573 | 18.769 | 33.921 | 51.480 | 0.03x |
| users.json $..total | strata | 1.886 | 1.936 | 2.177 | 53.504 | 1.00x |
| users.json $..total | jsonpath-ng | 336.751 | 360.985 | 403.995 | 53.504 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.065 | 4.098 | 6.443 | 51.473 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.516 | 15.620 | 25.580 | 51.473 | 0.26x |
| users.json $[*].id | orjson+jsonpath-ng | 17.739 | 18.337 | 21.102 | 51.473 | 0.22x |
| users.json $[*].orders[*].total | strata | 4.397 | 4.590 | 6.832 | 53.504 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 19.078 | 19.750 | 20.388 | 53.504 | 0.23x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 37.569 | 40.992 | 54.388 | 53.504 | 0.11x |
| users.json $..total | strata | 15.131 | 16.449 | 18.071 | 53.504 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 357.558 | 381.499 | 403.929 | 53.504 | 0.04x |

