# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: a4d78c1
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
| users.json | strata | 18.110 | 18.728 | 23.461 | 52.656 | 1.00x |
| users.json | orjson | 23.964 | 24.474 | 29.431 | 52.656 | 0.77x |
| users.json | msgspec | 23.254 | 24.228 | 28.577 | 52.656 | 0.77x |
| users.json | ujson | 34.675 | 35.998 | 42.408 | 52.656 | 0.52x |
| users.json | pysimdjson | 152.111 | 153.012 | 169.216 | 52.656 | 0.12x |
| users.json | json | 39.396 | 40.210 | 44.384 | 52.656 | 0.47x |
| flat.json | strata | 1.214 | 1.229 | 1.252 | 57.367 | 1.00x |
| flat.json | orjson | 1.281 | 1.301 | 1.998 | 57.367 | 0.95x |
| flat.json | msgspec | 1.407 | 1.485 | 1.532 | 57.367 | 0.83x |
| flat.json | ujson | 2.420 | 2.573 | 2.675 | 57.367 | 0.48x |
| flat.json | pysimdjson | 13.727 | 13.798 | 14.283 | 57.367 | 0.09x |
| flat.json | json | 2.946 | 2.982 | 3.260 | 57.367 | 0.41x |
| nested.json | strata | 1.290 | 1.369 | 1.416 | 49.641 | 1.00x |
| nested.json | orjson | 1.523 | 1.546 | 1.573 | 49.641 | 0.89x |
| nested.json | msgspec | 1.675 | 1.705 | 1.751 | 49.641 | 0.80x |
| nested.json | ujson | 2.820 | 2.860 | 3.103 | 49.641 | 0.48x |
| nested.json | pysimdjson | 12.596 | 12.626 | 13.349 | 49.641 | 0.11x |
| nested.json | json | 3.603 | 3.659 | 4.041 | 49.641 | 0.37x |
| wide_arrays.json | strata | 7.899 | 8.160 | 8.857 | 57.098 | 1.00x |
| wide_arrays.json | orjson | 8.622 | 8.901 | 9.745 | 57.098 | 0.92x |
| wide_arrays.json | msgspec | 9.480 | 9.559 | 10.712 | 57.098 | 0.85x |
| wide_arrays.json | ujson | 12.186 | 12.321 | 14.414 | 57.098 | 0.66x |
| wide_arrays.json | pysimdjson | 74.116 | 76.049 | 80.829 | 57.098 | 0.11x |
| wide_arrays.json | json | 15.753 | 15.904 | 17.795 | 57.098 | 0.51x |
| mixed.json | strata | 0.339 | 0.364 | 0.383 | 54.152 | 1.00x |
| mixed.json | orjson | 0.400 | 0.432 | 0.480 | 54.152 | 0.84x |
| mixed.json | msgspec | 0.416 | 0.456 | 0.483 | 54.152 | 0.80x |
| mixed.json | ujson | 0.578 | 0.622 | 0.699 | 54.152 | 0.59x |
| mixed.json | pysimdjson | 3.040 | 3.191 | 3.243 | 54.152 | 0.11x |
| mixed.json | json | 0.855 | 0.904 | 0.920 | 54.152 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.512 | 2.890 | 3.152 | 49.148 | 1.00x |
| users.json | orjson | 3.133 | 3.242 | 3.645 | 49.148 | 0.89x |
| users.json | msgspec | 5.467 | 5.732 | 6.083 | 49.148 | 0.50x |
| users.json | ujson | 29.663 | 29.845 | 30.437 | 49.148 | 0.10x |
| users.json | json | 39.205 | 40.472 | 44.574 | 49.148 | 0.07x |
| flat.json | strata | 0.275 | 0.306 | 0.321 | 54.703 | 1.00x |
| flat.json | orjson | 0.325 | 0.367 | 0.395 | 54.703 | 0.84x |
| flat.json | msgspec | 0.478 | 0.509 | 0.539 | 54.703 | 0.60x |
| flat.json | ujson | 2.459 | 2.491 | 2.629 | 54.703 | 0.12x |
| flat.json | json | 3.342 | 3.423 | 3.553 | 54.703 | 0.09x |
| nested.json | strata | 0.239 | 0.255 | 0.294 | 46.156 | 1.00x |
| nested.json | orjson | 0.331 | 0.343 | 0.362 | 46.156 | 0.74x |
| nested.json | msgspec | 0.516 | 0.531 | 0.676 | 46.156 | 0.48x |
| nested.json | ujson | 2.594 | 2.651 | 2.878 | 46.156 | 0.10x |
| nested.json | json | 4.314 | 4.402 | 4.458 | 46.156 | 0.06x |
| wide_arrays.json | strata | 2.260 | 2.464 | 6.380 | 54.977 | 1.00x |
| wide_arrays.json | orjson | 2.094 | 2.205 | 2.569 | 54.977 | 1.12x |
| wide_arrays.json | msgspec | 2.930 | 3.015 | 3.081 | 54.977 | 0.82x |
| wide_arrays.json | ujson | 14.243 | 14.899 | 15.511 | 54.977 | 0.17x |
| wide_arrays.json | json | 31.761 | 33.563 | 34.485 | 54.977 | 0.07x |
| mixed.json | strata | 0.076 | 0.082 | 0.118 | 50.941 | 1.00x |
| mixed.json | orjson | 0.069 | 0.072 | 0.084 | 50.941 | 1.13x |
| mixed.json | msgspec | 0.103 | 0.105 | 0.123 | 50.941 | 0.78x |
| mixed.json | ujson | 0.590 | 0.600 | 0.621 | 50.941 | 0.14x |
| mixed.json | json | 0.921 | 0.965 | 0.982 | 50.941 | 0.08x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 18.676 | 18.824 | 20.745 | 61.516 | 1.00x |
| users.json | orjson | 24.330 | 24.947 | 27.997 | 61.516 | 0.75x |
| users.json | msgspec | 24.009 | 25.115 | 28.713 | 61.516 | 0.75x |
| users.json | ujson | 35.606 | 36.801 | 41.501 | 61.516 | 0.51x |
| users.json | json | 40.361 | 41.587 | 46.593 | 61.516 | 0.45x |
| flat.json | strata | 1.213 | 1.339 | 1.382 | 54.703 | 1.00x |
| flat.json | orjson | 1.300 | 1.441 | 1.474 | 54.703 | 0.93x |
| flat.json | msgspec | 1.589 | 1.630 | 1.894 | 54.703 | 0.82x |
| flat.json | ujson | 2.714 | 2.736 | 2.770 | 54.703 | 0.49x |
| flat.json | json | 2.940 | 3.087 | 3.168 | 54.703 | 0.43x |
| nested.json | strata | 1.510 | 1.520 | 1.651 | 46.645 | 1.00x |
| nested.json | orjson | 1.716 | 1.762 | 1.890 | 46.645 | 0.86x |
| nested.json | msgspec | 1.904 | 1.960 | 2.064 | 46.645 | 0.78x |
| nested.json | ujson | 3.073 | 3.147 | 3.262 | 46.645 | 0.48x |
| nested.json | json | 3.833 | 3.894 | 4.313 | 46.645 | 0.39x |
| wide_arrays.json | strata | 8.017 | 8.332 | 8.848 | 56.199 | 1.00x |
| wide_arrays.json | orjson | 8.629 | 8.774 | 9.462 | 56.199 | 0.95x |
| wide_arrays.json | msgspec | 9.655 | 10.022 | 10.514 | 56.199 | 0.83x |
| wide_arrays.json | ujson | 12.587 | 12.785 | 13.925 | 56.199 | 0.65x |
| wide_arrays.json | json | 16.166 | 17.073 | 17.811 | 56.199 | 0.49x |
| mixed.json | strata | 0.377 | 0.428 | 0.464 | 50.941 | 1.00x |
| mixed.json | orjson | 0.478 | 0.532 | 0.559 | 50.941 | 0.80x |
| mixed.json | msgspec | 0.499 | 0.552 | 0.585 | 50.941 | 0.78x |
| mixed.json | ujson | 0.674 | 0.739 | 0.812 | 50.941 | 0.58x |
| mixed.json | json | 0.877 | 0.964 | 1.041 | 50.941 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 18.767 | 19.294 | 20.980 | 56.410 | 1.00x |
| users.ndjson | orjson | 25.269 | 26.259 | 28.714 | 56.410 | 0.73x |
| users.ndjson | msgspec | 24.997 | 25.917 | 28.428 | 56.410 | 0.74x |
| users.ndjson | ujson | 36.550 | 37.355 | 40.822 | 56.410 | 0.52x |
| users.ndjson | json | 45.824 | 47.109 | 51.604 | 56.410 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.607 | 3.653 | 3.920 | 55.812 | 1.00x |
| users.json | orjson | 3.902 | 4.056 | 4.662 | 55.812 | 0.90x |
| users.json | msgspec | 5.626 | 5.934 | 6.579 | 55.812 | 0.62x |
| users.json | ujson | 30.603 | 31.055 | 32.707 | 55.812 | 0.12x |
| users.json | json | 40.257 | 40.884 | 45.174 | 55.812 | 0.09x |
| flat.json | strata | 0.573 | 0.621 | 0.712 | 54.703 | 1.00x |
| flat.json | orjson | 0.681 | 0.710 | 0.877 | 54.703 | 0.87x |
| flat.json | msgspec | 0.755 | 0.854 | 1.010 | 54.703 | 0.73x |
| flat.json | ujson | 2.783 | 2.867 | 2.959 | 54.703 | 0.22x |
| flat.json | json | 3.686 | 3.797 | 3.876 | 54.703 | 0.16x |
| nested.json | strata | 0.530 | 0.550 | 0.586 | 46.645 | 1.00x |
| nested.json | orjson | 0.640 | 0.659 | 0.687 | 46.645 | 0.83x |
| nested.json | msgspec | 0.840 | 0.868 | 0.918 | 46.645 | 0.63x |
| nested.json | ujson | 2.989 | 3.046 | 3.339 | 46.645 | 0.18x |
| nested.json | json | 4.747 | 4.828 | 5.140 | 46.645 | 0.11x |
| wide_arrays.json | strata | 2.891 | 3.232 | 3.668 | 54.977 | 1.00x |
| wide_arrays.json | orjson | 2.764 | 2.819 | 3.154 | 54.977 | 1.15x |
| wide_arrays.json | msgspec | 3.483 | 3.796 | 3.934 | 54.977 | 0.85x |
| wide_arrays.json | ujson | 15.127 | 15.604 | 16.832 | 54.977 | 0.21x |
| wide_arrays.json | json | 32.956 | 34.259 | 36.406 | 54.977 | 0.09x |
| mixed.json | strata | 0.271 | 0.327 | 0.355 | 50.973 | 1.00x |
| mixed.json | orjson | 0.282 | 0.338 | 0.379 | 50.973 | 0.97x |
| mixed.json | msgspec | 0.309 | 0.375 | 0.429 | 50.973 | 0.87x |
| mixed.json | ujson | 0.835 | 0.871 | 0.968 | 50.973 | 0.38x |
| mixed.json | json | 1.154 | 1.234 | 1.294 | 50.973 | 0.27x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.129 | 0.138 | 0.148 | 55.871 | 1.00x |
| users.json $[*].id | jmespath | 0.880 | 0.900 | 0.935 | 55.871 | 0.15x |
| users.json $[*].id | jsonpath-ng | 4.762 | 4.880 | 5.045 | 55.871 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.831 | 0.917 | 1.381 | 55.250 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.393 | 5.633 | 6.194 | 55.250 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 33.498 | 34.743 | 36.593 | 55.250 | 0.03x |
| users.json $..total | strata | 3.138 | 3.211 | 3.501 | 57.422 | 1.00x |
| users.json $..total | jsonpath-ng | 658.807 | 680.159 | 712.755 | 57.422 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.549 | 3.600 | 3.662 | 59.738 | 1.00x |
| users.json $[*].id | orjson+jmespath | 25.751 | 27.095 | 28.233 | 59.738 | 0.13x |
| users.json $[*].id | orjson+jsonpath-ng | 29.757 | 31.129 | 32.865 | 59.738 | 0.12x |
| users.json $[*].orders[*].total | strata | 3.958 | 3.993 | 4.698 | 56.504 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 30.053 | 30.765 | 32.235 | 56.504 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 61.710 | 62.345 | 66.523 | 56.504 | 0.06x |
| users.json $..total | strata | 22.121 | 22.364 | 24.099 | 56.707 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 677.249 | 686.075 | 749.087 | 56.707 | 0.03x |

