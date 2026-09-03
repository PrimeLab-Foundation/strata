# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: ab20434
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
| users.json | strata | 18.051 | 18.637 | 21.648 | 52.617 | 1.00x |
| users.json | orjson | 23.316 | 25.486 | 30.496 | 52.617 | 0.73x |
| users.json | msgspec | 22.726 | 25.606 | 28.130 | 52.617 | 0.73x |
| users.json | ujson | 35.257 | 37.009 | 40.963 | 52.617 | 0.50x |
| users.json | pysimdjson | 151.550 | 155.070 | 170.581 | 52.617 | 0.12x |
| users.json | json | 39.993 | 41.530 | 45.756 | 52.617 | 0.45x |
| flat.json | strata | 1.159 | 1.180 | 1.254 | 57.805 | 1.00x |
| flat.json | orjson | 1.161 | 1.278 | 1.341 | 57.805 | 0.92x |
| flat.json | msgspec | 1.325 | 1.455 | 1.487 | 57.805 | 0.81x |
| flat.json | ujson | 2.455 | 2.550 | 2.617 | 57.805 | 0.46x |
| flat.json | pysimdjson | 13.676 | 13.777 | 14.666 | 57.805 | 0.09x |
| flat.json | json | 2.681 | 2.937 | 2.967 | 57.805 | 0.40x |
| nested.json | strata | 1.435 | 1.460 | 1.508 | 46.199 | 1.00x |
| nested.json | orjson | 1.623 | 1.673 | 1.796 | 46.199 | 0.87x |
| nested.json | msgspec | 1.753 | 1.866 | 1.945 | 46.199 | 0.78x |
| nested.json | ujson | 2.938 | 3.010 | 3.188 | 46.199 | 0.48x |
| nested.json | pysimdjson | 12.811 | 13.031 | 14.339 | 46.199 | 0.11x |
| nested.json | json | 3.749 | 3.862 | 4.178 | 46.199 | 0.38x |
| wide_arrays.json | strata | 7.781 | 8.153 | 8.938 | 57.648 | 1.00x |
| wide_arrays.json | orjson | 8.248 | 9.259 | 10.335 | 57.648 | 0.88x |
| wide_arrays.json | msgspec | 9.473 | 10.040 | 11.019 | 57.648 | 0.81x |
| wide_arrays.json | ujson | 12.226 | 12.593 | 13.818 | 57.648 | 0.65x |
| wide_arrays.json | pysimdjson | 73.553 | 75.775 | 83.525 | 57.648 | 0.11x |
| wide_arrays.json | json | 15.408 | 16.380 | 20.236 | 57.648 | 0.50x |
| mixed.json | strata | 0.331 | 0.365 | 0.393 | 52.906 | 1.00x |
| mixed.json | orjson | 0.361 | 0.433 | 0.463 | 52.906 | 0.84x |
| mixed.json | msgspec | 0.389 | 0.460 | 0.465 | 52.906 | 0.79x |
| mixed.json | ujson | 0.575 | 0.627 | 0.718 | 52.906 | 0.58x |
| mixed.json | pysimdjson | 2.838 | 3.249 | 3.355 | 52.906 | 0.11x |
| mixed.json | json | 0.756 | 0.888 | 0.911 | 52.906 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.135 | 2.389 | 2.527 | 48.129 | 1.00x |
| users.json | orjson | 2.882 | 3.145 | 3.817 | 48.129 | 0.76x |
| users.json | msgspec | 4.759 | 4.928 | 6.126 | 48.129 | 0.48x |
| users.json | ujson | 29.621 | 30.767 | 32.705 | 48.129 | 0.08x |
| users.json | json | 38.567 | 41.258 | 44.805 | 48.129 | 0.06x |
| flat.json | strata | 0.231 | 0.278 | 0.309 | 45.746 | 1.00x |
| flat.json | orjson | 0.338 | 0.417 | 0.546 | 45.746 | 0.67x |
| flat.json | msgspec | 0.434 | 0.496 | 0.532 | 45.746 | 0.56x |
| flat.json | ujson | 2.464 | 2.555 | 2.831 | 45.746 | 0.11x |
| flat.json | json | 3.420 | 3.459 | 3.908 | 45.746 | 0.08x |
| nested.json | strata | 0.199 | 0.224 | 0.309 | 46.055 | 1.00x |
| nested.json | orjson | 0.291 | 0.324 | 0.400 | 46.055 | 0.69x |
| nested.json | msgspec | 0.470 | 0.538 | 0.571 | 46.055 | 0.42x |
| nested.json | ujson | 2.437 | 2.655 | 2.689 | 46.055 | 0.08x |
| nested.json | json | 4.034 | 4.373 | 4.654 | 46.055 | 0.05x |
| wide_arrays.json | strata | 1.348 | 1.522 | 1.660 | 55.887 | 1.00x |
| wide_arrays.json | orjson | 1.953 | 2.086 | 2.194 | 55.887 | 0.73x |
| wide_arrays.json | msgspec | 2.723 | 3.006 | 3.171 | 55.887 | 0.51x |
| wide_arrays.json | ujson | 13.299 | 14.220 | 14.906 | 55.887 | 0.11x |
| wide_arrays.json | json | 31.073 | 32.016 | 32.812 | 55.887 | 0.05x |
| mixed.json | strata | 0.058 | 0.059 | 0.064 | 50.695 | 1.00x |
| mixed.json | orjson | 0.070 | 0.072 | 0.080 | 50.695 | 0.82x |
| mixed.json | msgspec | 0.097 | 0.099 | 0.111 | 50.695 | 0.60x |
| mixed.json | ujson | 0.577 | 0.580 | 0.587 | 50.695 | 0.10x |
| mixed.json | json | 0.901 | 0.905 | 0.956 | 50.695 | 0.07x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 18.531 | 19.513 | 21.369 | 60.574 | 1.00x |
| users.json | orjson | 25.508 | 26.604 | 30.959 | 60.574 | 0.73x |
| users.json | msgspec | 24.676 | 27.282 | 28.037 | 60.574 | 0.72x |
| users.json | ujson | 37.486 | 39.481 | 41.998 | 60.574 | 0.49x |
| users.json | json | 40.544 | 43.317 | 48.994 | 60.574 | 0.45x |
| flat.json | strata | 1.169 | 1.295 | 1.649 | 45.957 | 1.00x |
| flat.json | orjson | 1.317 | 1.447 | 1.478 | 45.957 | 0.90x |
| flat.json | msgspec | 1.516 | 1.630 | 2.061 | 45.957 | 0.79x |
| flat.json | ujson | 2.530 | 2.775 | 2.851 | 45.957 | 0.47x |
| flat.json | json | 2.856 | 3.128 | 3.315 | 45.957 | 0.41x |
| nested.json | strata | 1.297 | 1.482 | 1.859 | 46.055 | 1.00x |
| nested.json | orjson | 1.500 | 1.698 | 1.808 | 46.055 | 0.87x |
| nested.json | msgspec | 1.754 | 1.862 | 2.149 | 46.055 | 0.80x |
| nested.json | ujson | 2.721 | 3.016 | 3.256 | 46.055 | 0.49x |
| nested.json | json | 3.506 | 3.763 | 3.897 | 46.055 | 0.39x |
| wide_arrays.json | strata | 7.625 | 8.106 | 8.278 | 56.012 | 1.00x |
| wide_arrays.json | orjson | 8.047 | 8.725 | 9.544 | 56.012 | 0.93x |
| wide_arrays.json | msgspec | 9.560 | 9.746 | 10.429 | 56.012 | 0.83x |
| wide_arrays.json | ujson | 11.829 | 12.773 | 13.135 | 56.012 | 0.63x |
| wide_arrays.json | json | 15.470 | 16.173 | 17.026 | 56.012 | 0.50x |
| mixed.json | strata | 0.347 | 0.407 | 0.474 | 50.695 | 1.00x |
| mixed.json | orjson | 0.436 | 0.502 | 0.601 | 50.695 | 0.81x |
| mixed.json | msgspec | 0.460 | 0.536 | 0.640 | 50.695 | 0.76x |
| mixed.json | ujson | 0.610 | 0.717 | 0.746 | 50.695 | 0.57x |
| mixed.json | json | 0.813 | 0.914 | 0.977 | 50.695 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 18.458 | 19.091 | 20.805 | 57.625 | 1.00x |
| users.ndjson | orjson | 25.225 | 26.286 | 27.830 | 57.625 | 0.73x |
| users.ndjson | msgspec | 25.432 | 26.274 | 28.584 | 57.625 | 0.73x |
| users.ndjson | ujson | 36.521 | 37.666 | 41.002 | 57.625 | 0.51x |
| users.ndjson | json | 46.093 | 47.299 | 51.795 | 57.625 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.759 | 3.138 | 3.559 | 57.656 | 1.00x |
| users.json | orjson | 4.097 | 4.235 | 4.434 | 57.656 | 0.74x |
| users.json | msgspec | 5.599 | 5.835 | 6.401 | 57.656 | 0.54x |
| users.json | ujson | 29.801 | 31.145 | 32.465 | 57.656 | 0.10x |
| users.json | json | 39.740 | 40.615 | 43.180 | 57.656 | 0.08x |
| flat.json | strata | 0.578 | 0.687 | 1.188 | 45.957 | 1.00x |
| flat.json | orjson | 0.683 | 0.797 | 1.010 | 45.957 | 0.86x |
| flat.json | msgspec | 0.812 | 0.993 | 1.855 | 45.957 | 0.69x |
| flat.json | ujson | 2.839 | 3.241 | 3.926 | 45.957 | 0.21x |
| flat.json | json | 3.788 | 4.069 | 4.919 | 45.957 | 0.17x |
| nested.json | strata | 0.447 | 0.539 | 0.674 | 46.055 | 1.00x |
| nested.json | orjson | 0.632 | 0.651 | 0.719 | 46.055 | 0.83x |
| nested.json | msgspec | 0.788 | 0.835 | 1.004 | 46.055 | 0.65x |
| nested.json | ujson | 2.871 | 3.000 | 3.198 | 46.055 | 0.18x |
| nested.json | json | 4.587 | 4.701 | 4.847 | 46.055 | 0.11x |
| wide_arrays.json | strata | 2.046 | 2.166 | 2.308 | 56.012 | 1.00x |
| wide_arrays.json | orjson | 2.704 | 2.887 | 3.033 | 56.012 | 0.75x |
| wide_arrays.json | msgspec | 3.431 | 3.702 | 3.765 | 56.012 | 0.59x |
| wide_arrays.json | ujson | 14.777 | 15.052 | 16.648 | 56.012 | 0.14x |
| wide_arrays.json | json | 32.986 | 34.001 | 36.341 | 56.012 | 0.06x |
| mixed.json | strata | 0.303 | 0.357 | 0.461 | 50.695 | 1.00x |
| mixed.json | orjson | 0.325 | 0.372 | 40.878 | 50.695 | 0.96x |
| mixed.json | msgspec | 0.354 | 0.400 | 40.629 | 50.695 | 0.89x |
| mixed.json | ujson | 0.887 | 0.946 | 40.663 | 50.695 | 0.38x |
| mixed.json | json | 1.165 | 1.253 | 1.338 | 50.695 | 0.29x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.114 | 0.135 | 0.156 | 57.727 | 1.00x |
| users.json $[*].id | jmespath | 0.811 | 0.897 | 1.081 | 57.727 | 0.15x |
| users.json $[*].id | jsonpath-ng | 4.818 | 4.893 | 5.432 | 57.727 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.715 | 0.840 | 1.006 | 55.023 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.131 | 5.622 | 5.774 | 55.023 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 32.103 | 34.318 | 36.116 | 55.023 | 0.02x |
| users.json $..total | strata | 2.847 | 3.102 | 3.403 | 57.129 | 1.00x |
| users.json $..total | jsonpath-ng | 654.420 | 677.009 | 694.005 | 57.129 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.448 | 3.725 | 4.310 | 57.926 | 1.00x |
| users.json $[*].id | orjson+jmespath | 25.226 | 25.767 | 27.356 | 57.926 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 27.532 | 29.158 | 30.723 | 57.926 | 0.13x |
| users.json $[*].orders[*].total | strata | 3.704 | 3.853 | 4.013 | 56.309 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 30.166 | 31.600 | 34.510 | 56.309 | 0.12x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 61.084 | 63.259 | 66.113 | 56.309 | 0.06x |
| users.json $..total | strata | 21.009 | 22.533 | 24.184 | 56.383 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 679.231 | 685.720 | 740.030 | 56.383 | 0.03x |

