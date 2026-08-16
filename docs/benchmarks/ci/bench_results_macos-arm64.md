# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: f35a664
- python: 3.12.10
- implementation: CPython
- platform: macOS-26.5.2-arm64-arm-64bit
- machine: arm64
- processor: Apple M1 (Virtual)
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.215 | 9.041 | 14.502 | 63.734 | 1.00x |
| users.json | orjson | 10.248 | 12.209 | 16.961 | 63.734 | 0.74x |
| users.json | msgspec | 10.473 | 12.661 | 16.319 | 63.734 | 0.71x |
| users.json | ujson | 14.489 | 16.019 | 27.595 | 63.734 | 0.56x |
| users.json | pysimdjson | 136.208 | 157.038 | 179.708 | 63.734 | 0.06x |
| users.json | json | 15.393 | 17.912 | 21.107 | 63.734 | 0.50x |
| flat.json | strata | 0.658 | 0.683 | 0.826 | 85.438 | 1.00x |
| flat.json | orjson | 0.757 | 0.790 | 0.968 | 85.438 | 0.87x |
| flat.json | msgspec | 0.716 | 0.743 | 0.829 | 85.438 | 0.92x |
| flat.json | ujson | 1.254 | 1.350 | 1.445 | 85.438 | 0.51x |
| flat.json | pysimdjson | 11.976 | 12.242 | 13.500 | 85.438 | 0.06x |
| flat.json | json | 1.356 | 1.395 | 1.497 | 85.438 | 0.49x |
| nested.json | strata | 0.594 | 0.691 | 1.750 | 85.453 | 1.00x |
| nested.json | orjson | 0.740 | 0.829 | 0.910 | 85.453 | 0.83x |
| nested.json | msgspec | 0.687 | 0.812 | 0.913 | 85.453 | 0.85x |
| nested.json | ujson | 1.090 | 1.280 | 1.420 | 85.453 | 0.54x |
| nested.json | pysimdjson | 10.665 | 11.568 | 17.671 | 85.453 | 0.06x |
| nested.json | json | 1.484 | 1.552 | 1.708 | 85.453 | 0.45x |
| wide_arrays.json | strata | 3.536 | 3.767 | 3.958 | 88.438 | 1.00x |
| wide_arrays.json | orjson | 3.694 | 3.985 | 4.513 | 88.438 | 0.95x |
| wide_arrays.json | msgspec | 4.263 | 4.700 | 5.667 | 88.438 | 0.80x |
| wide_arrays.json | ujson | 5.692 | 5.885 | 6.656 | 88.438 | 0.64x |
| wide_arrays.json | pysimdjson | 66.340 | 69.632 | 78.049 | 88.438 | 0.05x |
| wide_arrays.json | json | 6.997 | 7.542 | 11.120 | 88.438 | 0.50x |
| mixed.json | strata | 0.150 | 0.170 | 0.177 | 88.469 | 1.00x |
| mixed.json | orjson | 0.173 | 0.191 | 0.266 | 88.469 | 0.89x |
| mixed.json | msgspec | 0.180 | 0.187 | 0.235 | 88.469 | 0.91x |
| mixed.json | ujson | 0.249 | 0.322 | 0.422 | 88.469 | 0.53x |
| mixed.json | pysimdjson | 2.741 | 2.816 | 4.358 | 88.469 | 0.06x |
| mixed.json | json | 0.366 | 0.383 | 0.491 | 88.469 | 0.44x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.662 | 2.040 | 2.282 | 75.406 | 1.00x |
| users.json | orjson | 2.216 | 2.708 | 3.131 | 75.406 | 0.75x |
| users.json | msgspec | 2.728 | 3.157 | 3.777 | 75.406 | 0.65x |
| users.json | ujson | 10.601 | 11.752 | 13.448 | 75.406 | 0.17x |
| users.json | json | 15.899 | 17.587 | 20.467 | 75.406 | 0.12x |
| flat.json | strata | 0.205 | 0.276 | 0.392 | 85.453 | 1.00x |
| flat.json | orjson | 0.268 | 0.330 | 0.396 | 85.453 | 0.84x |
| flat.json | msgspec | 0.324 | 0.334 | 0.370 | 85.453 | 0.83x |
| flat.json | ujson | 0.955 | 1.018 | 1.114 | 85.453 | 0.27x |
| flat.json | json | 1.436 | 1.486 | 1.574 | 85.453 | 0.19x |
| nested.json | strata | 0.143 | 0.163 | 0.175 | 85.469 | 1.00x |
| nested.json | orjson | 0.252 | 0.271 | 0.384 | 85.469 | 0.60x |
| nested.json | msgspec | 0.295 | 0.331 | 0.358 | 85.469 | 0.49x |
| nested.json | ujson | 1.181 | 1.257 | 1.350 | 85.469 | 0.13x |
| nested.json | json | 1.757 | 1.871 | 2.029 | 85.469 | 0.09x |
| wide_arrays.json | strata | 1.295 | 1.341 | 1.725 | 88.453 | 1.00x |
| wide_arrays.json | orjson | 1.382 | 1.565 | 1.689 | 88.453 | 0.86x |
| wide_arrays.json | msgspec | 2.280 | 2.414 | 3.000 | 88.453 | 0.56x |
| wide_arrays.json | ujson | 6.809 | 7.104 | 7.961 | 88.453 | 0.19x |
| wide_arrays.json | json | 12.627 | 12.810 | 13.054 | 88.453 | 0.10x |
| mixed.json | strata | 0.047 | 0.056 | 0.083 | 88.469 | 1.00x |
| mixed.json | orjson | 0.052 | 0.060 | 0.112 | 88.469 | 0.93x |
| mixed.json | msgspec | 0.060 | 0.080 | 0.260 | 88.469 | 0.70x |
| mixed.json | ujson | 0.239 | 0.266 | 0.340 | 88.469 | 0.21x |
| mixed.json | json | 0.407 | 0.421 | 0.481 | 88.469 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.167 | 8.548 | 9.825 | 85.734 | 1.00x |
| users.json | orjson | 10.040 | 10.722 | 12.555 | 85.734 | 0.80x |
| users.json | msgspec | 9.898 | 10.732 | 12.117 | 85.734 | 0.80x |
| users.json | ujson | 13.278 | 14.544 | 17.574 | 85.734 | 0.59x |
| users.json | json | 16.066 | 17.540 | 20.322 | 85.734 | 0.49x |
| flat.json | strata | 0.710 | 0.748 | 0.822 | 85.453 | 1.00x |
| flat.json | orjson | 0.885 | 0.961 | 1.573 | 85.453 | 0.78x |
| flat.json | msgspec | 0.802 | 0.859 | 0.990 | 85.453 | 0.87x |
| flat.json | ujson | 1.145 | 1.218 | 1.450 | 85.453 | 0.61x |
| flat.json | json | 1.402 | 1.435 | 1.873 | 85.453 | 0.52x |
| nested.json | strata | 0.640 | 0.731 | 0.847 | 85.469 | 1.00x |
| nested.json | orjson | 0.939 | 1.043 | 1.299 | 85.469 | 0.70x |
| nested.json | msgspec | 0.776 | 0.878 | 1.109 | 85.469 | 0.83x |
| nested.json | ujson | 1.075 | 1.238 | 1.573 | 85.469 | 0.59x |
| nested.json | json | 1.464 | 1.656 | 2.013 | 85.469 | 0.44x |
| wide_arrays.json | strata | 3.707 | 3.996 | 4.489 | 88.453 | 1.00x |
| wide_arrays.json | orjson | 4.026 | 4.204 | 4.994 | 88.453 | 0.95x |
| wide_arrays.json | msgspec | 4.530 | 4.821 | 5.646 | 88.453 | 0.83x |
| wide_arrays.json | ujson | 6.089 | 6.705 | 7.659 | 88.453 | 0.60x |
| wide_arrays.json | json | 7.281 | 8.068 | 14.241 | 88.453 | 0.50x |
| mixed.json | strata | 0.186 | 0.198 | 0.234 | 88.469 | 1.00x |
| mixed.json | orjson | 0.227 | 0.323 | 0.427 | 88.469 | 0.61x |
| mixed.json | msgspec | 0.233 | 0.257 | 0.275 | 88.469 | 0.77x |
| mixed.json | ujson | 0.294 | 0.320 | 0.482 | 88.469 | 0.62x |
| mixed.json | json | 0.393 | 0.441 | 0.541 | 88.469 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.908 | 8.505 | 9.617 | 84.781 | 1.00x |
| users.ndjson | orjson | 11.884 | 12.575 | 14.960 | 84.781 | 0.68x |
| users.ndjson | msgspec | 12.281 | 12.858 | 13.589 | 84.781 | 0.66x |
| users.ndjson | ujson | 15.223 | 16.058 | 17.067 | 84.781 | 0.53x |
| users.ndjson | json | 19.542 | 20.675 | 23.760 | 84.781 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.337 | 2.552 | 2.951 | 84.875 | 1.00x |
| users.json | orjson | 2.954 | 3.146 | 3.976 | 84.875 | 0.81x |
| users.json | msgspec | 3.526 | 3.722 | 4.071 | 84.875 | 0.69x |
| users.json | ujson | 12.268 | 12.933 | 14.048 | 84.875 | 0.20x |
| users.json | json | 17.498 | 18.544 | 19.783 | 84.875 | 0.14x |
| flat.json | strata | 0.371 | 0.460 | 0.769 | 85.453 | 1.00x |
| flat.json | orjson | 0.445 | 0.485 | 0.816 | 85.453 | 0.95x |
| flat.json | msgspec | 0.494 | 0.623 | 1.293 | 85.453 | 0.74x |
| flat.json | ujson | 1.119 | 1.189 | 1.596 | 85.453 | 0.39x |
| flat.json | json | 1.613 | 1.755 | 2.484 | 85.453 | 0.26x |
| nested.json | strata | 0.329 | 0.428 | 0.732 | 85.469 | 1.00x |
| nested.json | orjson | 0.432 | 0.526 | 0.593 | 85.469 | 0.81x |
| nested.json | msgspec | 0.620 | 0.716 | 0.887 | 85.469 | 0.60x |
| nested.json | ujson | 1.407 | 1.615 | 2.933 | 85.469 | 0.27x |
| nested.json | json | 2.062 | 2.234 | 2.619 | 85.469 | 0.19x |
| wide_arrays.json | strata | 1.800 | 1.872 | 2.161 | 88.453 | 1.00x |
| wide_arrays.json | orjson | 2.033 | 2.470 | 3.209 | 88.453 | 0.76x |
| wide_arrays.json | msgspec | 2.951 | 3.453 | 5.779 | 88.453 | 0.54x |
| wide_arrays.json | ujson | 8.493 | 8.825 | 9.456 | 88.453 | 0.21x |
| wide_arrays.json | json | 14.115 | 14.382 | 14.895 | 88.453 | 0.13x |
| mixed.json | strata | 0.176 | 0.215 | 0.283 | 88.469 | 1.00x |
| mixed.json | orjson | 0.204 | 0.231 | 0.260 | 88.469 | 0.93x |
| mixed.json | msgspec | 0.208 | 0.313 | 0.512 | 88.469 | 0.69x |
| mixed.json | ujson | 0.422 | 0.447 | 0.551 | 88.469 | 0.48x |
| mixed.json | json | 0.553 | 0.605 | 0.701 | 88.469 | 0.35x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.052 | 0.063 | 0.096 | 84.938 | 1.00x |
| users.json $[*].id | jmespath | 0.294 | 0.320 | 0.375 | 84.938 | 0.20x |
| users.json $[*].id | jsonpath-ng | 1.494 | 1.637 | 1.790 | 84.938 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.320 | 0.333 | 0.349 | 85.016 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.774 | 1.901 | 2.152 | 85.016 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 11.009 | 11.402 | 11.907 | 85.016 | 0.03x |
| users.json $..total | strata | 1.261 | 1.438 | 1.556 | 84.938 | 1.00x |
| users.json $..total | jsonpath-ng | 190.543 | 203.169 | 236.522 | 84.938 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.516 | 3.757 | 4.639 | 84.984 | 1.00x |
| users.json $[*].id | orjson+jmespath | 9.977 | 10.736 | 11.366 | 84.984 | 0.35x |
| users.json $[*].id | orjson+jsonpath-ng | 11.301 | 11.990 | 12.911 | 84.984 | 0.31x |
| users.json $[*].orders[*].total | strata | 3.649 | 3.895 | 4.372 | 84.891 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 11.612 | 12.202 | 13.516 | 84.891 | 0.32x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 23.661 | 24.498 | 26.749 | 84.891 | 0.16x |
| users.json $..total | strata | 9.273 | 10.187 | 15.112 | 84.969 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 207.606 | 223.561 | 251.670 | 84.969 | 0.05x |

