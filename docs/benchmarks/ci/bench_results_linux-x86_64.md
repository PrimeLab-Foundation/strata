# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: fb7997e
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: x86_64
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.192 | 10.727 | 14.823 | 60.863 | 1.00x |
| users.json | orjson | 13.360 | 13.902 | 15.968 | 60.863 | 0.77x |
| users.json | msgspec | 13.083 | 14.104 | 31.941 | 60.863 | 0.76x |
| users.json | ujson | 18.510 | 21.750 | 39.841 | 60.863 | 0.49x |
| users.json | pysimdjson | 19.238 | 20.841 | 27.162 | 60.863 | 0.51x |
| users.json | json | 22.816 | 23.219 | 28.494 | 60.863 | 0.46x |
| flat.json | strata | 0.828 | 0.845 | 0.863 | 68.828 | 1.00x |
| flat.json | orjson | 0.957 | 0.960 | 0.983 | 68.828 | 0.88x |
| flat.json | msgspec | 0.994 | 1.013 | 1.066 | 68.828 | 0.83x |
| flat.json | ujson | 1.413 | 1.454 | 1.673 | 68.828 | 0.58x |
| flat.json | pysimdjson | 1.496 | 1.522 | 1.719 | 68.828 | 0.56x |
| flat.json | json | 1.891 | 1.900 | 1.972 | 68.828 | 0.44x |
| nested.json | strata | 0.798 | 0.811 | 0.824 | 68.828 | 1.00x |
| nested.json | orjson | 0.994 | 1.002 | 1.286 | 68.828 | 0.81x |
| nested.json | msgspec | 1.012 | 1.018 | 1.055 | 68.828 | 0.80x |
| nested.json | ujson | 1.475 | 1.498 | 1.899 | 68.828 | 0.54x |
| nested.json | pysimdjson | 1.391 | 1.410 | 1.747 | 68.828 | 0.58x |
| nested.json | json | 2.027 | 2.040 | 2.060 | 68.828 | 0.40x |
| wide_arrays.json | strata | 4.662 | 4.705 | 4.858 | 75.773 | 1.00x |
| wide_arrays.json | orjson | 5.056 | 5.173 | 5.459 | 75.773 | 0.91x |
| wide_arrays.json | msgspec | 5.580 | 5.655 | 5.785 | 75.773 | 0.83x |
| wide_arrays.json | ujson | 7.188 | 7.274 | 7.408 | 75.773 | 0.65x |
| wide_arrays.json | pysimdjson | 5.969 | 6.142 | 6.274 | 75.773 | 0.77x |
| wide_arrays.json | json | 9.650 | 9.807 | 10.255 | 75.773 | 0.48x |
| mixed.json | strata | 0.193 | 0.195 | 0.226 | 75.836 | 1.00x |
| mixed.json | orjson | 0.226 | 0.230 | 0.255 | 75.836 | 0.85x |
| mixed.json | msgspec | 0.242 | 0.244 | 0.292 | 75.836 | 0.80x |
| mixed.json | ujson | 0.313 | 0.328 | 0.371 | 75.836 | 0.59x |
| mixed.json | pysimdjson | 0.296 | 0.300 | 0.315 | 75.836 | 0.65x |
| mixed.json | json | 0.474 | 0.484 | 0.506 | 75.836 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.432 | 2.468 | 2.549 | 59.223 | 1.00x |
| users.json | orjson | 2.906 | 2.928 | 2.972 | 59.223 | 0.84x |
| users.json | msgspec | 3.845 | 3.869 | 3.972 | 59.223 | 0.64x |
| users.json | ujson | 15.036 | 15.376 | 15.682 | 59.223 | 0.16x |
| users.json | json | 21.583 | 21.884 | 22.222 | 59.223 | 0.11x |
| flat.json | strata | 0.260 | 0.263 | 0.274 | 68.828 | 1.00x |
| flat.json | orjson | 0.328 | 0.342 | 0.346 | 68.828 | 0.77x |
| flat.json | msgspec | 0.428 | 0.434 | 0.454 | 68.828 | 0.61x |
| flat.json | ujson | 1.302 | 1.314 | 1.415 | 68.828 | 0.20x |
| flat.json | json | 1.830 | 1.850 | 1.872 | 68.828 | 0.14x |
| nested.json | strata | 0.251 | 0.254 | 0.269 | 68.828 | 1.00x |
| nested.json | orjson | 0.287 | 0.299 | 0.389 | 68.828 | 0.85x |
| nested.json | msgspec | 0.402 | 0.408 | 0.420 | 68.828 | 0.62x |
| nested.json | ujson | 1.461 | 1.494 | 1.543 | 68.828 | 0.17x |
| nested.json | json | 2.344 | 2.370 | 2.464 | 68.828 | 0.11x |
| wide_arrays.json | strata | 1.457 | 1.477 | 1.489 | 75.773 | 1.00x |
| wide_arrays.json | orjson | 1.792 | 1.808 | 1.833 | 75.773 | 0.82x |
| wide_arrays.json | msgspec | 2.676 | 2.686 | 2.707 | 75.773 | 0.55x |
| wide_arrays.json | ujson | 8.602 | 8.643 | 8.785 | 75.773 | 0.17x |
| wide_arrays.json | json | 16.373 | 16.493 | 16.709 | 75.773 | 0.09x |
| mixed.json | strata | 0.061 | 0.074 | 0.103 | 75.836 | 1.00x |
| mixed.json | orjson | 0.064 | 0.068 | 0.078 | 75.836 | 1.08x |
| mixed.json | msgspec | 0.085 | 0.086 | 0.087 | 75.836 | 0.86x |
| mixed.json | ujson | 0.309 | 0.312 | 0.324 | 75.836 | 0.24x |
| mixed.json | json | 0.509 | 0.518 | 0.531 | 75.836 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.240 | 11.422 | 13.409 | 73.371 | 1.00x |
| users.json | orjson | 13.554 | 13.787 | 14.431 | 73.371 | 0.83x |
| users.json | msgspec | 13.433 | 13.984 | 14.265 | 73.371 | 0.82x |
| users.json | ujson | 18.687 | 20.296 | 24.125 | 73.371 | 0.56x |
| users.json | json | 22.977 | 23.373 | 24.541 | 73.371 | 0.49x |
| flat.json | strata | 0.877 | 0.884 | 0.889 | 68.828 | 1.00x |
| flat.json | orjson | 1.019 | 1.024 | 1.040 | 68.828 | 0.86x |
| flat.json | msgspec | 1.050 | 1.060 | 1.094 | 68.828 | 0.83x |
| flat.json | ujson | 1.522 | 1.577 | 1.598 | 68.828 | 0.56x |
| flat.json | json | 1.964 | 1.982 | 2.016 | 68.828 | 0.45x |
| nested.json | strata | 0.833 | 0.839 | 0.846 | 68.828 | 1.00x |
| nested.json | orjson | 1.039 | 1.048 | 1.067 | 68.828 | 0.80x |
| nested.json | msgspec | 1.058 | 1.072 | 1.123 | 68.828 | 0.78x |
| nested.json | ujson | 1.512 | 1.549 | 1.577 | 68.828 | 0.54x |
| nested.json | json | 2.098 | 2.119 | 2.256 | 68.828 | 0.40x |
| wide_arrays.json | strata | 4.925 | 4.979 | 5.496 | 75.836 | 1.00x |
| wide_arrays.json | orjson | 5.365 | 5.788 | 6.081 | 75.836 | 0.86x |
| wide_arrays.json | msgspec | 5.991 | 6.204 | 6.460 | 75.836 | 0.80x |
| wide_arrays.json | ujson | 7.813 | 8.127 | 8.348 | 75.836 | 0.61x |
| wide_arrays.json | json | 10.266 | 10.450 | 10.640 | 75.836 | 0.48x |
| mixed.json | strata | 0.215 | 0.219 | 0.233 | 75.836 | 1.00x |
| mixed.json | orjson | 0.273 | 0.278 | 0.297 | 75.836 | 0.79x |
| mixed.json | msgspec | 0.284 | 0.287 | 0.305 | 75.836 | 0.76x |
| mixed.json | ujson | 0.360 | 0.374 | 0.429 | 75.836 | 0.59x |
| mixed.json | json | 0.520 | 0.536 | 0.562 | 75.836 | 0.41x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.737 | 11.559 | 11.961 | 68.828 | 1.00x |
| users.ndjson | orjson | 16.962 | 17.369 | 17.769 | 68.828 | 0.67x |
| users.ndjson | msgspec | 16.761 | 17.306 | 17.667 | 68.828 | 0.67x |
| users.ndjson | ujson | 21.926 | 22.851 | 23.320 | 68.828 | 0.51x |
| users.ndjson | json | 30.208 | 30.804 | 32.917 | 68.828 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.005 | 3.069 | 3.140 | 70.418 | 1.00x |
| users.json | orjson | 3.538 | 3.607 | 3.801 | 70.418 | 0.85x |
| users.json | msgspec | 4.470 | 4.529 | 4.689 | 70.418 | 0.68x |
| users.json | ujson | 16.341 | 16.527 | 17.011 | 70.418 | 0.19x |
| users.json | json | 22.475 | 22.593 | 23.356 | 70.418 | 0.14x |
| flat.json | strata | 0.401 | 0.407 | 0.760 | 68.828 | 1.00x |
| flat.json | orjson | 0.489 | 0.502 | 0.989 | 68.828 | 0.81x |
| flat.json | msgspec | 0.588 | 0.611 | 1.056 | 68.828 | 0.67x |
| flat.json | ujson | 1.524 | 1.538 | 1.574 | 68.828 | 0.26x |
| flat.json | json | 2.020 | 2.058 | 2.090 | 68.828 | 0.20x |
| nested.json | strata | 0.358 | 0.382 | 0.405 | 68.828 | 1.00x |
| nested.json | orjson | 0.421 | 0.433 | 0.465 | 68.828 | 0.88x |
| nested.json | msgspec | 0.539 | 0.551 | 0.577 | 68.828 | 0.69x |
| nested.json | ujson | 1.577 | 1.615 | 1.635 | 68.828 | 0.24x |
| nested.json | json | 2.533 | 2.568 | 2.613 | 68.828 | 0.15x |
| wide_arrays.json | strata | 1.922 | 2.174 | 2.615 | 75.836 | 1.00x |
| wide_arrays.json | orjson | 2.279 | 2.449 | 2.895 | 75.836 | 0.89x |
| wide_arrays.json | msgspec | 3.157 | 3.332 | 3.649 | 75.836 | 0.65x |
| wide_arrays.json | ujson | 9.259 | 9.497 | 10.050 | 75.836 | 0.23x |
| wide_arrays.json | json | 16.997 | 17.184 | 18.189 | 75.836 | 0.13x |
| mixed.json | strata | 0.151 | 0.153 | 0.192 | 75.836 | 1.00x |
| mixed.json | orjson | 0.171 | 0.174 | 0.193 | 75.836 | 0.88x |
| mixed.json | msgspec | 0.191 | 0.194 | 0.222 | 75.836 | 0.79x |
| mixed.json | ujson | 0.427 | 0.443 | 0.469 | 75.836 | 0.35x |
| mixed.json | json | 0.624 | 0.642 | 0.668 | 75.836 | 0.24x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.063 | 0.067 | 0.080 | 70.418 | 1.00x |
| users.json $[*].id | jmespath | 0.494 | 0.510 | 0.633 | 70.418 | 0.13x |
| users.json $[*].id | jsonpath-ng | 2.794 | 2.922 | 3.072 | 70.418 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.458 | 0.651 | 0.692 | 70.418 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.121 | 3.176 | 3.433 | 70.418 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 20.029 | 22.548 | 24.077 | 70.418 | 0.03x |
| users.json $..total | strata | 1.676 | 1.699 | 1.755 | 72.387 | 1.00x |
| users.json $..total | jsonpath-ng | 397.424 | 403.198 | 404.485 | 72.387 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.171 | 3.183 | 3.208 | 70.418 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.092 | 14.266 | 15.786 | 70.418 | 0.22x |
| users.json $[*].id | orjson+jsonpath-ng | 16.552 | 16.722 | 19.223 | 70.418 | 0.19x |
| users.json $[*].orders[*].total | strata | 3.407 | 3.456 | 3.536 | 71.605 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 16.945 | 17.443 | 21.204 | 71.605 | 0.20x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 37.839 | 39.696 | 44.554 | 71.605 | 0.09x |
| users.json $..total | strata | 13.604 | 13.813 | 14.999 | 71.664 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 420.179 | 424.974 | 467.971 | 71.664 | 0.03x |

