# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 4d4a4be
- python: 3.12.10
- implementation: CPython
- platform: macOS-15.7.7-x86_64-i386-64bit
- machine: x86_64
- processor: Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 21.616 | 23.185 | 92.614 | 52.594 | 1.00x |
| users.json | orjson | 28.663 | 35.496 | 51.393 | 52.594 | 0.65x |
| users.json | msgspec | 29.523 | 33.291 | 43.095 | 52.594 | 0.70x |
| users.json | ujson | 41.366 | 48.877 | 99.376 | 52.594 | 0.47x |
| users.json | pysimdjson | 182.573 | 212.410 | 477.722 | 52.594 | 0.11x |
| users.json | json | 46.044 | 62.465 | 164.512 | 52.594 | 0.37x |
| flat.json | strata | 1.457 | 1.507 | 2.269 | 59.172 | 1.00x |
| flat.json | orjson | 1.538 | 1.566 | 2.109 | 59.172 | 0.96x |
| flat.json | msgspec | 1.761 | 1.800 | 2.179 | 59.172 | 0.84x |
| flat.json | ujson | 3.067 | 3.199 | 4.414 | 59.172 | 0.47x |
| flat.json | pysimdjson | 16.143 | 17.202 | 18.768 | 59.172 | 0.09x |
| flat.json | json | 3.463 | 3.567 | 5.131 | 59.172 | 0.42x |
| nested.json | strata | 1.643 | 1.681 | 1.870 | 47.352 | 1.00x |
| nested.json | orjson | 1.843 | 1.878 | 2.472 | 47.352 | 0.90x |
| nested.json | msgspec | 2.053 | 2.108 | 2.868 | 47.352 | 0.80x |
| nested.json | ujson | 3.372 | 3.466 | 4.236 | 47.352 | 0.49x |
| nested.json | pysimdjson | 14.739 | 14.903 | 15.464 | 47.352 | 0.11x |
| nested.json | json | 4.331 | 4.426 | 4.712 | 47.352 | 0.38x |
| wide_arrays.json | strata | 9.389 | 11.002 | 12.038 | 58.770 | 1.00x |
| wide_arrays.json | orjson | 10.439 | 11.449 | 14.324 | 58.770 | 0.96x |
| wide_arrays.json | msgspec | 11.408 | 12.631 | 16.874 | 58.770 | 0.87x |
| wide_arrays.json | ujson | 14.637 | 16.098 | 18.728 | 58.770 | 0.68x |
| wide_arrays.json | pysimdjson | 88.872 | 99.408 | 109.797 | 58.770 | 0.11x |
| wide_arrays.json | json | 18.804 | 21.024 | 34.954 | 58.770 | 0.52x |
| mixed.json | strata | 0.419 | 0.427 | 0.491 | 54.230 | 1.00x |
| mixed.json | orjson | 0.484 | 0.490 | 0.500 | 54.230 | 0.87x |
| mixed.json | msgspec | 0.514 | 0.525 | 0.808 | 54.230 | 0.82x |
| mixed.json | ujson | 0.709 | 0.714 | 0.729 | 54.230 | 0.60x |
| mixed.json | pysimdjson | 3.555 | 3.585 | 3.971 | 54.230 | 0.12x |
| mixed.json | json | 0.985 | 0.990 | 1.037 | 54.230 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.152 | 3.541 | 14.173 | 47.324 | 1.00x |
| users.json | orjson | 3.518 | 3.956 | 4.127 | 47.324 | 0.89x |
| users.json | msgspec | 6.162 | 7.047 | 12.850 | 47.324 | 0.50x |
| users.json | ujson | 33.654 | 35.818 | 63.068 | 47.324 | 0.10x |
| users.json | json | 45.003 | 47.540 | 59.977 | 47.324 | 0.07x |
| flat.json | strata | 0.358 | 0.374 | 0.383 | 46.973 | 1.00x |
| flat.json | orjson | 0.443 | 0.466 | 0.536 | 46.973 | 0.80x |
| flat.json | msgspec | 0.630 | 0.805 | 0.855 | 46.973 | 0.46x |
| flat.json | ujson | 2.916 | 2.966 | 3.120 | 46.973 | 0.13x |
| flat.json | json | 3.999 | 4.042 | 4.886 | 46.973 | 0.09x |
| nested.json | strata | 0.293 | 0.308 | 0.417 | 47.184 | 1.00x |
| nested.json | orjson | 0.398 | 0.430 | 0.624 | 47.184 | 0.72x |
| nested.json | msgspec | 0.604 | 0.648 | 0.870 | 47.184 | 0.47x |
| nested.json | ujson | 3.050 | 3.310 | 4.067 | 47.184 | 0.09x |
| nested.json | json | 5.027 | 5.275 | 7.027 | 47.184 | 0.06x |
| wide_arrays.json | strata | 2.071 | 2.169 | 3.535 | 55.148 | 1.00x |
| wide_arrays.json | orjson | 2.485 | 2.600 | 3.546 | 55.148 | 0.83x |
| wide_arrays.json | msgspec | 3.591 | 3.733 | 6.202 | 55.148 | 0.58x |
| wide_arrays.json | ujson | 16.711 | 16.919 | 21.614 | 55.148 | 0.13x |
| wide_arrays.json | json | 37.114 | 38.704 | 46.977 | 55.148 | 0.06x |
| mixed.json | strata | 0.104 | 0.107 | 0.114 | 53.719 | 1.00x |
| mixed.json | orjson | 0.092 | 0.097 | 0.106 | 53.719 | 1.10x |
| mixed.json | msgspec | 0.130 | 0.136 | 0.183 | 53.719 | 0.79x |
| mixed.json | ujson | 0.673 | 0.681 | 0.738 | 53.719 | 0.16x |
| mixed.json | json | 1.047 | 1.063 | 1.095 | 53.719 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 21.331 | 21.998 | 25.762 | 59.672 | 1.00x |
| users.json | orjson | 27.052 | 31.541 | 43.497 | 59.672 | 0.70x |
| users.json | msgspec | 28.609 | 29.914 | 39.125 | 59.672 | 0.74x |
| users.json | ujson | 41.787 | 43.108 | 53.793 | 59.672 | 0.51x |
| users.json | json | 46.278 | 48.865 | 56.372 | 59.672 | 0.45x |
| flat.json | strata | 1.428 | 1.519 | 1.622 | 47.113 | 1.00x |
| flat.json | orjson | 1.558 | 1.629 | 1.694 | 47.113 | 0.93x |
| flat.json | msgspec | 1.807 | 1.889 | 1.971 | 47.113 | 0.80x |
| flat.json | ujson | 3.115 | 3.189 | 3.349 | 47.113 | 0.48x |
| flat.json | json | 3.531 | 3.602 | 3.903 | 47.113 | 0.42x |
| nested.json | strata | 1.789 | 1.857 | 2.576 | 47.184 | 1.00x |
| nested.json | orjson | 2.010 | 2.059 | 3.102 | 47.184 | 0.90x |
| nested.json | msgspec | 2.223 | 2.252 | 2.680 | 47.184 | 0.82x |
| nested.json | ujson | 3.583 | 3.716 | 4.214 | 47.184 | 0.50x |
| nested.json | json | 4.483 | 4.848 | 6.433 | 47.184 | 0.38x |
| wide_arrays.json | strata | 9.097 | 9.214 | 12.393 | 56.934 | 1.00x |
| wide_arrays.json | orjson | 10.119 | 10.352 | 16.333 | 56.934 | 0.89x |
| wide_arrays.json | msgspec | 11.310 | 12.015 | 18.569 | 56.934 | 0.77x |
| wide_arrays.json | ujson | 14.667 | 15.979 | 17.496 | 56.934 | 0.58x |
| wide_arrays.json | json | 18.655 | 19.262 | 24.312 | 56.934 | 0.48x |
| mixed.json | strata | 0.514 | 0.653 | 1.273 | 53.719 | 1.00x |
| mixed.json | orjson | 0.629 | 0.736 | 0.964 | 53.719 | 0.89x |
| mixed.json | msgspec | 0.667 | 0.956 | 1.219 | 53.719 | 0.68x |
| mixed.json | ujson | 0.891 | 1.031 | 1.464 | 53.719 | 0.63x |
| mixed.json | json | 1.129 | 1.295 | 1.757 | 53.719 | 0.50x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 22.072 | 22.954 | 25.228 | 58.137 | 1.00x |
| users.ndjson | orjson | 29.525 | 31.183 | 37.368 | 58.137 | 0.74x |
| users.ndjson | msgspec | 29.987 | 32.337 | 37.097 | 58.137 | 0.71x |
| users.ndjson | ujson | 42.990 | 44.708 | 54.593 | 58.137 | 0.51x |
| users.ndjson | json | 53.523 | 55.627 | 69.072 | 58.137 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.229 | 4.605 | 4.980 | 53.273 | 1.00x |
| users.json | orjson | 4.637 | 4.885 | 5.369 | 53.273 | 0.94x |
| users.json | msgspec | 6.591 | 6.886 | 59.044 | 53.273 | 0.67x |
| users.json | ujson | 35.836 | 36.697 | 40.282 | 53.273 | 0.13x |
| users.json | json | 47.295 | 48.927 | 96.008 | 53.273 | 0.09x |
| flat.json | strata | 0.741 | 0.779 | 0.925 | 47.113 | 1.00x |
| flat.json | orjson | 0.851 | 0.931 | 1.281 | 47.113 | 0.84x |
| flat.json | msgspec | 1.034 | 1.119 | 1.440 | 47.113 | 0.70x |
| flat.json | ujson | 3.340 | 3.427 | 4.016 | 47.113 | 0.23x |
| flat.json | json | 4.489 | 4.599 | 5.575 | 47.113 | 0.17x |
| nested.json | strata | 0.641 | 0.707 | 1.081 | 47.184 | 1.00x |
| nested.json | orjson | 0.811 | 0.909 | 4.349 | 47.184 | 0.78x |
| nested.json | msgspec | 0.994 | 1.095 | 1.818 | 47.184 | 0.65x |
| nested.json | ujson | 3.526 | 3.787 | 5.524 | 47.184 | 0.19x |
| nested.json | json | 5.544 | 6.025 | 7.607 | 47.184 | 0.12x |
| wide_arrays.json | strata | 2.913 | 3.101 | 3.924 | 57.336 | 1.00x |
| wide_arrays.json | orjson | 3.207 | 3.429 | 5.386 | 57.336 | 0.90x |
| wide_arrays.json | msgspec | 4.590 | 4.947 | 5.817 | 57.336 | 0.63x |
| wide_arrays.json | ujson | 17.441 | 18.208 | 23.269 | 57.336 | 0.17x |
| wide_arrays.json | json | 38.359 | 38.783 | 96.364 | 57.336 | 0.08x |
| mixed.json | strata | 0.377 | 0.509 | 0.689 | 53.719 | 1.00x |
| mixed.json | orjson | 0.435 | 0.510 | 0.705 | 53.719 | 1.00x |
| mixed.json | msgspec | 0.433 | 0.536 | 0.781 | 53.719 | 0.95x |
| mixed.json | ujson | 0.996 | 1.089 | 1.510 | 53.719 | 0.47x |
| mixed.json | json | 1.372 | 1.504 | 3.161 | 53.719 | 0.34x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.169 | 0.242 | 1.159 | 53.336 | 1.00x |
| users.json $[*].id | jmespath | 1.058 | 1.155 | 8.116 | 53.336 | 0.21x |
| users.json $[*].id | jsonpath-ng | 5.929 | 6.408 | 43.338 | 53.336 | 0.04x |
| users.json $[*].orders[*].total | strata | 1.163 | 1.260 | 2.220 | 54.293 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 6.955 | 8.777 | 53.366 | 54.293 | 0.14x |
| users.json $[*].orders[*].total | jsonpath-ng | 41.235 | 52.243 | 149.688 | 54.293 | 0.02x |
| users.json $..total | strata | 3.485 | 3.939 | 4.780 | 57.250 | 1.00x |
| users.json $..total | jsonpath-ng | 746.867 | 824.123 | 900.295 | 57.250 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.111 | 4.860 | 16.783 | 57.242 | 1.00x |
| users.json $[*].id | orjson+jmespath | 27.709 | 35.467 | 42.105 | 57.242 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 32.892 | 41.453 | 140.733 | 57.242 | 0.12x |
| users.json $[*].orders[*].total | strata | 4.742 | 5.517 | 7.337 | 56.445 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 38.612 | 46.300 | 54.596 | 56.445 | 0.12x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 78.501 | 100.835 | 110.266 | 56.445 | 0.05x |
| users.json $..total | strata | 26.020 | 27.287 | 39.678 | 56.500 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 803.497 | 904.698 | 1071.263 | 56.500 | 0.03x |

