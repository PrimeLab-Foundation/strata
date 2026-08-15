# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 75b7e23
- python: 3.12.13
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
| users.json | strata | 10.511 | 10.717 | 14.500 | 57.926 | 1.00x |
| users.json | orjson | 13.352 | 13.705 | 23.940 | 57.926 | 0.78x |
| users.json | msgspec | 12.842 | 13.162 | 25.490 | 57.926 | 0.81x |
| users.json | ujson | 18.090 | 19.042 | 36.313 | 57.926 | 0.56x |
| users.json | pysimdjson | 17.933 | 19.806 | 34.013 | 57.926 | 0.54x |
| users.json | json | 22.224 | 22.705 | 26.134 | 57.926 | 0.47x |
| flat.json | strata | 0.924 | 0.935 | 0.969 | 67.297 | 1.00x |
| flat.json | orjson | 0.990 | 0.997 | 1.026 | 67.297 | 0.94x |
| flat.json | msgspec | 0.994 | 1.005 | 1.044 | 67.297 | 0.93x |
| flat.json | ujson | 1.477 | 1.489 | 1.602 | 67.297 | 0.63x |
| flat.json | pysimdjson | 1.540 | 1.570 | 1.622 | 67.297 | 0.60x |
| flat.json | json | 1.872 | 1.886 | 1.907 | 67.297 | 0.50x |
| nested.json | strata | 0.856 | 0.874 | 0.894 | 67.297 | 1.00x |
| nested.json | orjson | 1.004 | 1.016 | 1.175 | 67.297 | 0.86x |
| nested.json | msgspec | 1.012 | 1.029 | 1.058 | 67.297 | 0.85x |
| nested.json | ujson | 1.477 | 1.521 | 1.986 | 67.297 | 0.57x |
| nested.json | pysimdjson | 1.412 | 1.441 | 1.468 | 67.297 | 0.61x |
| nested.json | json | 2.101 | 2.118 | 2.175 | 67.297 | 0.41x |
| wide_arrays.json | strata | 4.738 | 4.766 | 5.244 | 71.668 | 1.00x |
| wide_arrays.json | orjson | 4.977 | 5.065 | 5.082 | 71.668 | 0.94x |
| wide_arrays.json | msgspec | 5.549 | 5.664 | 5.706 | 71.668 | 0.84x |
| wide_arrays.json | ujson | 7.095 | 7.164 | 7.496 | 71.668 | 0.67x |
| wide_arrays.json | pysimdjson | 5.945 | 6.027 | 6.225 | 71.668 | 0.79x |
| wide_arrays.json | json | 9.703 | 9.831 | 10.196 | 71.668 | 0.48x |
| mixed.json | strata | 0.202 | 0.207 | 0.267 | 71.730 | 1.00x |
| mixed.json | orjson | 0.226 | 0.230 | 0.245 | 71.730 | 0.90x |
| mixed.json | msgspec | 0.239 | 0.241 | 0.273 | 71.730 | 0.86x |
| mixed.json | ujson | 0.304 | 0.316 | 0.376 | 71.730 | 0.65x |
| mixed.json | pysimdjson | 0.300 | 0.306 | 0.329 | 71.730 | 0.68x |
| mixed.json | json | 0.472 | 0.483 | 0.510 | 71.730 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.633 | 2.657 | 2.800 | 56.656 | 1.00x |
| users.json | orjson | 2.867 | 2.900 | 2.940 | 56.656 | 0.92x |
| users.json | msgspec | 3.846 | 3.865 | 4.038 | 56.656 | 0.69x |
| users.json | ujson | 14.514 | 14.643 | 14.980 | 56.656 | 0.18x |
| users.json | json | 21.753 | 21.963 | 22.666 | 56.656 | 0.12x |
| flat.json | strata | 0.275 | 0.276 | 0.288 | 67.297 | 1.00x |
| flat.json | orjson | 0.329 | 0.338 | 0.502 | 67.297 | 0.82x |
| flat.json | msgspec | 0.438 | 0.445 | 0.482 | 67.297 | 0.62x |
| flat.json | ujson | 1.210 | 1.224 | 1.738 | 67.297 | 0.23x |
| flat.json | json | 1.884 | 1.898 | 1.987 | 67.297 | 0.15x |
| nested.json | strata | 0.270 | 0.326 | 0.418 | 67.297 | 1.00x |
| nested.json | orjson | 0.291 | 0.309 | 0.449 | 67.297 | 1.05x |
| nested.json | msgspec | 0.415 | 0.522 | 0.739 | 67.297 | 0.62x |
| nested.json | ujson | 1.302 | 1.815 | 2.230 | 67.297 | 0.18x |
| nested.json | json | 2.427 | 3.674 | 3.914 | 67.297 | 0.09x |
| wide_arrays.json | strata | 1.598 | 1.618 | 1.656 | 71.668 | 1.00x |
| wide_arrays.json | orjson | 1.783 | 1.792 | 1.827 | 71.668 | 0.90x |
| wide_arrays.json | msgspec | 2.669 | 2.677 | 2.696 | 71.668 | 0.60x |
| wide_arrays.json | ujson | 8.491 | 8.523 | 8.579 | 71.668 | 0.19x |
| wide_arrays.json | json | 16.338 | 16.421 | 16.547 | 71.668 | 0.10x |
| mixed.json | strata | 0.068 | 0.070 | 0.072 | 71.730 | 1.00x |
| mixed.json | orjson | 0.063 | 0.064 | 0.079 | 71.730 | 1.09x |
| mixed.json | msgspec | 0.084 | 0.086 | 0.088 | 71.730 | 0.81x |
| mixed.json | ujson | 0.295 | 0.305 | 0.316 | 71.730 | 0.23x |
| mixed.json | json | 0.518 | 0.526 | 0.540 | 71.730 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.262 | 11.611 | 13.001 | 71.605 | 1.00x |
| users.json | orjson | 13.812 | 14.061 | 14.718 | 71.605 | 0.83x |
| users.json | msgspec | 13.466 | 13.916 | 14.105 | 71.605 | 0.83x |
| users.json | ujson | 19.129 | 20.227 | 23.778 | 71.605 | 0.57x |
| users.json | json | 23.017 | 23.220 | 23.614 | 71.605 | 0.50x |
| flat.json | strata | 0.965 | 0.974 | 0.997 | 67.297 | 1.00x |
| flat.json | orjson | 1.052 | 1.067 | 1.090 | 67.297 | 0.91x |
| flat.json | msgspec | 1.053 | 1.064 | 1.073 | 67.297 | 0.92x |
| flat.json | ujson | 1.580 | 1.625 | 1.708 | 67.297 | 0.60x |
| flat.json | json | 1.917 | 1.962 | 1.991 | 67.297 | 0.50x |
| nested.json | strata | 0.884 | 0.896 | 0.964 | 67.297 | 1.00x |
| nested.json | orjson | 1.057 | 1.061 | 1.072 | 67.297 | 0.84x |
| nested.json | msgspec | 1.073 | 1.081 | 1.112 | 67.297 | 0.83x |
| nested.json | ujson | 1.544 | 1.574 | 1.615 | 67.297 | 0.57x |
| nested.json | json | 2.145 | 2.158 | 2.179 | 67.297 | 0.42x |
| wide_arrays.json | strata | 4.887 | 4.941 | 5.016 | 71.730 | 1.00x |
| wide_arrays.json | orjson | 5.076 | 5.135 | 5.364 | 71.730 | 0.96x |
| wide_arrays.json | msgspec | 5.700 | 5.750 | 5.860 | 71.730 | 0.86x |
| wide_arrays.json | ujson | 7.301 | 7.373 | 7.544 | 71.730 | 0.67x |
| wide_arrays.json | json | 9.869 | 9.945 | 10.082 | 71.730 | 0.50x |
| mixed.json | strata | 0.225 | 0.230 | 0.245 | 71.730 | 1.00x |
| mixed.json | orjson | 0.271 | 0.273 | 0.298 | 71.730 | 0.84x |
| mixed.json | msgspec | 0.280 | 0.282 | 0.301 | 71.730 | 0.81x |
| mixed.json | ujson | 0.360 | 0.370 | 0.382 | 71.730 | 0.62x |
| mixed.json | json | 0.511 | 0.525 | 0.554 | 71.730 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.877 | 11.073 | 12.442 | 67.297 | 1.00x |
| users.ndjson | orjson | 16.662 | 17.058 | 17.927 | 67.297 | 0.65x |
| users.ndjson | msgspec | 16.328 | 16.794 | 17.122 | 67.297 | 0.66x |
| users.ndjson | ujson | 22.008 | 22.477 | 23.240 | 67.297 | 0.49x |
| users.ndjson | json | 29.304 | 29.420 | 29.833 | 67.297 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.184 | 3.250 | 3.354 | 69.605 | 1.00x |
| users.json | orjson | 3.504 | 3.555 | 3.715 | 69.605 | 0.91x |
| users.json | msgspec | 4.439 | 4.513 | 4.710 | 69.605 | 0.72x |
| users.json | ujson | 15.203 | 15.273 | 15.675 | 69.605 | 0.21x |
| users.json | json | 22.715 | 22.896 | 23.328 | 69.605 | 0.14x |
| flat.json | strata | 0.427 | 0.439 | 0.481 | 67.297 | 1.00x |
| flat.json | orjson | 0.496 | 0.519 | 0.569 | 67.297 | 0.85x |
| flat.json | msgspec | 0.593 | 0.610 | 0.659 | 67.297 | 0.72x |
| flat.json | ujson | 1.404 | 1.439 | 1.513 | 67.297 | 0.31x |
| flat.json | json | 2.068 | 2.116 | 2.155 | 67.297 | 0.21x |
| nested.json | strata | 0.383 | 0.396 | 0.406 | 67.297 | 1.00x |
| nested.json | orjson | 0.425 | 0.439 | 0.475 | 67.297 | 0.90x |
| nested.json | msgspec | 0.536 | 0.563 | 0.601 | 67.297 | 0.70x |
| nested.json | ujson | 1.458 | 1.482 | 1.517 | 67.297 | 0.27x |
| nested.json | json | 2.587 | 2.610 | 2.633 | 67.297 | 0.15x |
| wide_arrays.json | strata | 2.029 | 2.068 | 2.122 | 71.730 | 1.00x |
| wide_arrays.json | orjson | 2.214 | 2.245 | 2.346 | 71.730 | 0.92x |
| wide_arrays.json | msgspec | 3.115 | 3.155 | 3.183 | 71.730 | 0.66x |
| wide_arrays.json | ujson | 9.088 | 9.172 | 9.685 | 71.730 | 0.23x |
| wide_arrays.json | json | 16.833 | 16.974 | 17.360 | 71.730 | 0.12x |
| mixed.json | strata | 0.157 | 0.158 | 0.187 | 71.730 | 1.00x |
| mixed.json | orjson | 0.171 | 0.172 | 0.200 | 71.730 | 0.92x |
| mixed.json | msgspec | 0.182 | 0.192 | 0.216 | 71.730 | 0.82x |
| mixed.json | ujson | 0.406 | 0.419 | 0.452 | 71.730 | 0.38x |
| mixed.json | json | 0.628 | 0.645 | 0.676 | 71.730 | 0.25x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.063 | 0.065 | 0.085 | 69.605 | 1.00x |
| users.json $[*].id | jmespath | 0.488 | 0.492 | 0.510 | 69.605 | 0.13x |
| users.json $[*].id | jsonpath-ng | 2.782 | 2.873 | 3.250 | 69.605 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.430 | 0.454 | 0.501 | 69.727 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.006 | 3.043 | 3.183 | 69.727 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 19.324 | 19.665 | 20.442 | 69.727 | 0.02x |
| users.json $..total | strata | 1.679 | 1.707 | 1.959 | 72.863 | 1.00x |
| users.json $..total | jsonpath-ng | 390.001 | 393.296 | 396.989 | 72.863 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.106 | 3.130 | 3.320 | 69.727 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.301 | 14.546 | 14.754 | 69.727 | 0.22x |
| users.json $[*].id | orjson+jsonpath-ng | 16.668 | 16.786 | 17.259 | 69.727 | 0.19x |
| users.json $[*].orders[*].total | strata | 3.313 | 3.332 | 3.508 | 72.863 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.071 | 17.444 | 18.364 | 72.863 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 36.392 | 37.341 | 38.437 | 72.863 | 0.09x |
| users.json $..total | strata | 14.141 | 14.493 | 15.281 | 72.863 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 415.873 | 418.763 | 425.072 | 72.863 | 0.03x |

