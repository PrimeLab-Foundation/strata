# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 37a96fb
- python: 3.12.10
- implementation: CPython
- platform: macOS-15.7.9-x86_64-i386-64bit
- machine: x86_64
- processor: Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 19.452 | 21.474 | 38.635 | 52.641 | 1.00x |
| users.json | orjson | 29.043 | 30.460 | 40.954 | 52.641 | 0.70x |
| users.json | msgspec | 28.166 | 29.710 | 38.981 | 52.641 | 0.72x |
| users.json | ujson | 40.154 | 42.058 | 65.664 | 52.641 | 0.51x |
| users.json | pysimdjson | 171.507 | 186.932 | 213.291 | 52.641 | 0.11x |
| users.json | json | 44.728 | 47.188 | 68.466 | 52.641 | 0.46x |
| flat.json | strata | 1.348 | 1.384 | 1.650 | 59.492 | 1.00x |
| flat.json | orjson | 1.427 | 1.442 | 1.697 | 59.492 | 0.96x |
| flat.json | msgspec | 1.631 | 1.662 | 2.101 | 59.492 | 0.83x |
| flat.json | ujson | 2.858 | 2.907 | 3.077 | 59.492 | 0.48x |
| flat.json | pysimdjson | 15.216 | 15.285 | 16.282 | 59.492 | 0.09x |
| flat.json | json | 3.247 | 3.297 | 3.467 | 59.492 | 0.42x |
| nested.json | strata | 1.585 | 2.007 | 12.126 | 54.789 | 1.00x |
| nested.json | orjson | 1.799 | 2.136 | 4.352 | 54.789 | 0.94x |
| nested.json | msgspec | 1.971 | 2.541 | 4.274 | 54.789 | 0.79x |
| nested.json | ujson | 3.206 | 4.238 | 6.214 | 54.789 | 0.47x |
| nested.json | pysimdjson | 14.361 | 15.907 | 21.225 | 54.789 | 0.13x |
| nested.json | json | 4.101 | 5.348 | 27.751 | 54.789 | 0.38x |
| wide_arrays.json | strata | 9.305 | 10.617 | 12.502 | 59.074 | 1.00x |
| wide_arrays.json | orjson | 10.501 | 11.876 | 14.178 | 59.074 | 0.89x |
| wide_arrays.json | msgspec | 11.311 | 12.375 | 16.391 | 59.074 | 0.86x |
| wide_arrays.json | ujson | 13.832 | 15.670 | 17.800 | 59.074 | 0.68x |
| wide_arrays.json | pysimdjson | 85.559 | 89.193 | 107.744 | 59.074 | 0.12x |
| wide_arrays.json | json | 18.230 | 20.104 | 23.456 | 59.074 | 0.53x |
| mixed.json | strata | 0.409 | 0.448 | 0.605 | 56.297 | 1.00x |
| mixed.json | orjson | 0.489 | 0.536 | 0.615 | 56.297 | 0.84x |
| mixed.json | msgspec | 0.518 | 0.537 | 0.810 | 56.297 | 0.84x |
| mixed.json | ujson | 0.708 | 0.753 | 0.849 | 56.297 | 0.60x |
| mixed.json | pysimdjson | 3.489 | 3.714 | 4.237 | 56.297 | 0.12x |
| mixed.json | json | 0.983 | 1.032 | 1.470 | 56.297 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.809 | 3.590 | 5.523 | 51.441 | 1.00x |
| users.json | orjson | 3.951 | 4.162 | 5.607 | 51.441 | 0.86x |
| users.json | msgspec | 5.909 | 6.384 | 8.202 | 51.441 | 0.56x |
| users.json | ujson | 33.843 | 37.928 | 43.808 | 51.441 | 0.09x |
| users.json | json | 44.318 | 50.822 | 79.354 | 51.441 | 0.07x |
| flat.json | strata | 0.293 | 0.336 | 0.407 | 54.656 | 1.00x |
| flat.json | orjson | 0.425 | 0.456 | 0.660 | 54.656 | 0.74x |
| flat.json | msgspec | 0.561 | 0.586 | 0.735 | 54.656 | 0.57x |
| flat.json | ujson | 2.714 | 2.846 | 3.292 | 54.656 | 0.12x |
| flat.json | json | 3.773 | 3.903 | 4.389 | 54.656 | 0.09x |
| nested.json | strata | 0.256 | 0.268 | 0.470 | 54.934 | 1.00x |
| nested.json | orjson | 0.386 | 0.406 | 0.714 | 54.934 | 0.66x |
| nested.json | msgspec | 0.593 | 0.613 | 0.990 | 54.934 | 0.44x |
| nested.json | ujson | 2.928 | 2.979 | 3.272 | 54.934 | 0.09x |
| nested.json | json | 4.838 | 4.928 | 5.440 | 54.934 | 0.05x |
| wide_arrays.json | strata | 2.089 | 2.230 | 2.702 | 57.168 | 1.00x |
| wide_arrays.json | orjson | 2.988 | 3.201 | 3.463 | 57.168 | 0.70x |
| wide_arrays.json | msgspec | 3.615 | 3.725 | 4.535 | 57.168 | 0.60x |
| wide_arrays.json | ujson | 16.860 | 17.200 | 19.584 | 57.168 | 0.13x |
| wide_arrays.json | json | 36.706 | 38.669 | 40.398 | 57.168 | 0.06x |
| mixed.json | strata | 0.096 | 0.102 | 0.115 | 54.160 | 1.00x |
| mixed.json | orjson | 0.090 | 0.103 | 0.119 | 54.160 | 1.00x |
| mixed.json | msgspec | 0.128 | 0.137 | 0.165 | 54.160 | 0.75x |
| mixed.json | ujson | 0.683 | 0.692 | 0.806 | 54.160 | 0.15x |
| mixed.json | json | 1.038 | 1.065 | 1.386 | 54.160 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 20.669 | 21.408 | 23.424 | 61.301 | 1.00x |
| users.json | orjson | 27.067 | 30.225 | 34.338 | 61.301 | 0.71x |
| users.json | msgspec | 27.768 | 30.666 | 34.903 | 61.301 | 0.70x |
| users.json | ujson | 42.770 | 44.416 | 51.498 | 61.301 | 0.48x |
| users.json | json | 45.225 | 49.474 | 81.830 | 61.301 | 0.43x |
| flat.json | strata | 1.432 | 1.506 | 1.573 | 54.656 | 1.00x |
| flat.json | orjson | 1.546 | 1.635 | 1.924 | 54.656 | 0.92x |
| flat.json | msgspec | 1.792 | 1.861 | 1.944 | 54.656 | 0.81x |
| flat.json | ujson | 3.078 | 3.130 | 3.224 | 54.656 | 0.48x |
| flat.json | json | 3.388 | 3.473 | 3.610 | 54.656 | 0.43x |
| nested.json | strata | 1.730 | 1.857 | 2.123 | 54.934 | 1.00x |
| nested.json | orjson | 1.953 | 2.192 | 2.925 | 54.934 | 0.85x |
| nested.json | msgspec | 2.212 | 2.436 | 2.858 | 54.934 | 0.76x |
| nested.json | ujson | 3.516 | 3.796 | 4.480 | 54.934 | 0.49x |
| nested.json | json | 4.419 | 4.640 | 5.411 | 54.934 | 0.40x |
| wide_arrays.json | strata | 9.393 | 9.647 | 11.323 | 58.395 | 1.00x |
| wide_arrays.json | orjson | 10.307 | 10.797 | 11.546 | 58.395 | 0.89x |
| wide_arrays.json | msgspec | 11.164 | 11.475 | 14.988 | 58.395 | 0.84x |
| wide_arrays.json | ujson | 14.567 | 15.455 | 19.917 | 58.395 | 0.62x |
| wide_arrays.json | json | 18.595 | 20.305 | 27.181 | 58.395 | 0.48x |
| mixed.json | strata | 0.503 | 0.523 | 0.644 | 54.160 | 1.00x |
| mixed.json | orjson | 0.634 | 0.645 | 0.809 | 54.160 | 0.81x |
| mixed.json | msgspec | 0.668 | 0.685 | 0.741 | 54.160 | 0.76x |
| mixed.json | ujson | 0.863 | 0.887 | 0.934 | 54.160 | 0.59x |
| mixed.json | json | 1.081 | 1.137 | 1.156 | 54.160 | 0.46x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 20.916 | 21.078 | 22.598 | 58.531 | 1.00x |
| users.ndjson | orjson | 29.056 | 29.787 | 37.546 | 58.531 | 0.71x |
| users.ndjson | msgspec | 29.056 | 29.931 | 38.644 | 58.531 | 0.70x |
| users.ndjson | ujson | 41.394 | 43.642 | 53.260 | 58.531 | 0.48x |
| users.ndjson | json | 51.480 | 52.474 | 58.257 | 58.531 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.093 | 4.361 | 5.889 | 57.391 | 1.00x |
| users.json | orjson | 5.006 | 5.331 | 7.690 | 57.391 | 0.82x |
| users.json | msgspec | 6.968 | 7.151 | 8.740 | 57.391 | 0.61x |
| users.json | ujson | 34.908 | 35.457 | 40.132 | 57.391 | 0.12x |
| users.json | json | 45.211 | 46.140 | 55.387 | 57.391 | 0.09x |
| flat.json | strata | 0.645 | 0.685 | 0.747 | 54.656 | 1.00x |
| flat.json | orjson | 0.804 | 0.835 | 0.941 | 54.656 | 0.82x |
| flat.json | msgspec | 0.940 | 0.985 | 1.128 | 54.656 | 0.70x |
| flat.json | ujson | 3.186 | 3.371 | 47.638 | 54.656 | 0.20x |
| flat.json | json | 4.148 | 4.299 | 4.774 | 54.656 | 0.16x |
| nested.json | strata | 0.593 | 0.703 | 0.835 | 54.934 | 1.00x |
| nested.json | orjson | 0.810 | 0.877 | 1.393 | 54.934 | 0.80x |
| nested.json | msgspec | 1.005 | 1.139 | 1.343 | 54.934 | 0.62x |
| nested.json | ujson | 3.464 | 3.731 | 4.332 | 54.934 | 0.19x |
| nested.json | json | 5.416 | 5.879 | 6.808 | 54.934 | 0.12x |
| wide_arrays.json | strata | 2.891 | 3.097 | 3.571 | 57.234 | 1.00x |
| wide_arrays.json | orjson | 3.515 | 3.718 | 4.288 | 57.234 | 0.83x |
| wide_arrays.json | msgspec | 4.531 | 4.980 | 6.259 | 57.234 | 0.62x |
| wide_arrays.json | ujson | 17.604 | 18.518 | 22.610 | 57.234 | 0.17x |
| wide_arrays.json | json | 38.332 | 40.019 | 42.732 | 57.234 | 0.08x |
| mixed.json | strata | 0.427 | 0.458 | 0.481 | 54.160 | 1.00x |
| mixed.json | orjson | 0.459 | 0.478 | 0.501 | 54.160 | 0.96x |
| mixed.json | msgspec | 0.476 | 0.531 | 0.562 | 54.160 | 0.86x |
| mixed.json | ujson | 1.049 | 1.105 | 1.197 | 54.160 | 0.41x |
| mixed.json | json | 1.408 | 1.463 | 1.568 | 54.160 | 0.31x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.159 | 0.206 | 0.243 | 57.453 | 1.00x |
| users.json $[*].id | jmespath | 1.052 | 1.093 | 1.332 | 57.453 | 0.19x |
| users.json $[*].id | jsonpath-ng | 5.650 | 5.869 | 6.796 | 57.453 | 0.04x |
| users.json $[*].orders[*].total | strata | 1.049 | 1.255 | 1.843 | 55.699 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 6.532 | 6.835 | 7.700 | 55.699 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 39.050 | 40.190 | 45.752 | 55.699 | 0.03x |
| users.json $..total | strata | 3.781 | 3.879 | 6.728 | 57.742 | 1.00x |
| users.json $..total | jsonpath-ng | 739.644 | 784.038 | 959.460 | 57.742 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.136 | 4.418 | 5.700 | 58.492 | 1.00x |
| users.json $[*].id | orjson+jmespath | 29.881 | 33.410 | 39.243 | 58.492 | 0.13x |
| users.json $[*].id | orjson+jsonpath-ng | 34.351 | 38.781 | 43.389 | 58.492 | 0.11x |
| users.json $[*].orders[*].total | strata | 4.553 | 4.800 | 5.753 | 56.930 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 36.230 | 39.243 | 45.572 | 56.930 | 0.12x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 75.589 | 89.445 | 107.452 | 56.930 | 0.05x |
| users.json $..total | strata | 24.223 | 26.209 | 32.025 | 56.980 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 795.285 | 837.847 | 923.192 | 56.980 | 0.03x |

