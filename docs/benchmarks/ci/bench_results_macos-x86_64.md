# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: a4f3b0e
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
| users.json | strata | 27.822 | 42.118 | 48.212 | 52.633 | 1.00x |
| users.json | orjson | 39.381 | 55.776 | 66.535 | 52.633 | 0.76x |
| users.json | msgspec | 40.127 | 57.943 | 72.572 | 52.633 | 0.73x |
| users.json | ujson | 60.772 | 87.148 | 107.207 | 52.633 | 0.48x |
| users.json | pysimdjson | 251.527 | 333.415 | 440.698 | 52.633 | 0.13x |
| users.json | json | 65.065 | 89.935 | 107.235 | 52.633 | 0.47x |
| flat.json | strata | 1.748 | 2.604 | 3.341 | 61.082 | 1.00x |
| flat.json | orjson | 2.034 | 2.397 | 2.848 | 61.082 | 1.09x |
| flat.json | msgspec | 2.283 | 3.091 | 4.235 | 61.082 | 0.84x |
| flat.json | ujson | 3.771 | 5.372 | 5.960 | 61.082 | 0.48x |
| flat.json | pysimdjson | 22.313 | 24.667 | 30.504 | 61.082 | 0.11x |
| flat.json | json | 4.555 | 6.377 | 6.873 | 61.082 | 0.41x |
| nested.json | strata | 2.100 | 3.164 | 3.472 | 58.367 | 1.00x |
| nested.json | orjson | 2.561 | 3.081 | 3.726 | 58.367 | 1.03x |
| nested.json | msgspec | 2.716 | 3.201 | 4.996 | 58.367 | 0.99x |
| nested.json | ujson | 4.445 | 5.661 | 6.812 | 58.367 | 0.56x |
| nested.json | pysimdjson | 19.343 | 22.029 | 25.304 | 58.367 | 0.14x |
| nested.json | json | 6.085 | 6.845 | 8.595 | 58.367 | 0.46x |
| wide_arrays.json | strata | 12.099 | 15.054 | 18.928 | 62.656 | 1.00x |
| wide_arrays.json | orjson | 12.708 | 16.408 | 20.456 | 62.656 | 0.92x |
| wide_arrays.json | msgspec | 14.354 | 17.491 | 33.742 | 62.656 | 0.86x |
| wide_arrays.json | ujson | 18.469 | 21.557 | 26.473 | 62.656 | 0.70x |
| wide_arrays.json | pysimdjson | 114.886 | 138.979 | 150.119 | 62.656 | 0.11x |
| wide_arrays.json | json | 23.748 | 29.747 | 36.500 | 62.656 | 0.51x |
| mixed.json | strata | 0.509 | 0.567 | 0.767 | 59.664 | 1.00x |
| mixed.json | orjson | 0.606 | 0.621 | 0.877 | 59.664 | 0.91x |
| mixed.json | msgspec | 0.635 | 0.732 | 0.951 | 59.664 | 0.77x |
| mixed.json | ujson | 0.873 | 1.013 | 1.538 | 59.664 | 0.56x |
| mixed.json | pysimdjson | 4.393 | 5.024 | 5.765 | 59.664 | 0.11x |
| mixed.json | json | 1.213 | 1.315 | 1.817 | 59.664 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.818 | 4.286 | 5.776 | 48.500 | 1.00x |
| users.json | orjson | 4.724 | 5.169 | 7.691 | 48.500 | 0.83x |
| users.json | msgspec | 8.429 | 9.440 | 13.610 | 48.500 | 0.45x |
| users.json | ujson | 44.785 | 49.282 | 66.033 | 48.500 | 0.09x |
| users.json | json | 58.501 | 66.254 | 100.598 | 48.500 | 0.06x |
| flat.json | strata | 0.466 | 0.534 | 0.616 | 58.207 | 1.00x |
| flat.json | orjson | 0.588 | 0.709 | 0.874 | 58.207 | 0.75x |
| flat.json | msgspec | 0.759 | 0.818 | 1.040 | 58.207 | 0.65x |
| flat.json | ujson | 3.820 | 4.625 | 4.873 | 58.207 | 0.12x |
| flat.json | json | 5.221 | 6.189 | 8.231 | 58.207 | 0.09x |
| nested.json | strata | 0.357 | 0.384 | 0.536 | 53.277 | 1.00x |
| nested.json | orjson | 0.512 | 0.527 | 0.707 | 53.277 | 0.73x |
| nested.json | msgspec | 0.814 | 0.870 | 1.133 | 53.277 | 0.44x |
| nested.json | ujson | 3.949 | 4.118 | 5.364 | 53.277 | 0.09x |
| nested.json | json | 6.543 | 6.785 | 10.122 | 53.277 | 0.06x |
| wide_arrays.json | strata | 2.553 | 2.745 | 3.475 | 61.176 | 1.00x |
| wide_arrays.json | orjson | 3.241 | 3.482 | 4.291 | 61.176 | 0.79x |
| wide_arrays.json | msgspec | 4.454 | 4.730 | 6.798 | 61.176 | 0.58x |
| wide_arrays.json | ujson | 21.538 | 24.757 | 25.718 | 61.176 | 0.11x |
| wide_arrays.json | json | 48.519 | 50.074 | 58.718 | 61.176 | 0.05x |
| mixed.json | strata | 0.107 | 0.129 | 0.183 | 56.344 | 1.00x |
| mixed.json | orjson | 0.114 | 0.155 | 0.257 | 56.344 | 0.83x |
| mixed.json | msgspec | 0.162 | 0.175 | 0.231 | 56.344 | 0.74x |
| mixed.json | ujson | 0.839 | 0.925 | 1.312 | 56.344 | 0.14x |
| mixed.json | json | 1.305 | 1.772 | 2.342 | 56.344 | 0.07x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 27.594 | 32.215 | 38.108 | 61.039 | 1.00x |
| users.json | orjson | 37.653 | 43.290 | 54.519 | 61.039 | 0.74x |
| users.json | msgspec | 37.808 | 41.351 | 59.263 | 61.039 | 0.78x |
| users.json | ujson | 56.044 | 62.817 | 78.934 | 61.039 | 0.51x |
| users.json | json | 64.214 | 72.307 | 87.698 | 61.039 | 0.45x |
| flat.json | strata | 1.957 | 2.121 | 3.440 | 58.230 | 1.00x |
| flat.json | orjson | 2.186 | 2.311 | 3.275 | 58.230 | 0.92x |
| flat.json | msgspec | 2.460 | 2.663 | 3.899 | 58.230 | 0.80x |
| flat.json | ujson | 4.149 | 4.708 | 6.460 | 58.230 | 0.45x |
| flat.json | json | 4.732 | 5.047 | 6.690 | 58.230 | 0.42x |
| nested.json | strata | 2.215 | 2.279 | 2.950 | 53.277 | 1.00x |
| nested.json | orjson | 2.623 | 2.801 | 3.263 | 53.277 | 0.81x |
| nested.json | msgspec | 2.931 | 3.304 | 4.281 | 53.277 | 0.69x |
| nested.json | ujson | 4.719 | 5.237 | 5.601 | 53.277 | 0.44x |
| nested.json | json | 5.752 | 6.198 | 6.722 | 53.277 | 0.37x |
| wide_arrays.json | strata | 11.819 | 12.910 | 16.610 | 61.176 | 1.00x |
| wide_arrays.json | orjson | 12.626 | 13.271 | 17.766 | 61.176 | 0.97x |
| wide_arrays.json | msgspec | 14.203 | 14.577 | 18.218 | 61.176 | 0.89x |
| wide_arrays.json | ujson | 18.382 | 19.799 | 22.662 | 61.176 | 0.65x |
| wide_arrays.json | json | 23.392 | 24.415 | 28.669 | 61.176 | 0.53x |
| mixed.json | strata | 0.632 | 0.683 | 1.286 | 56.344 | 1.00x |
| mixed.json | orjson | 0.767 | 0.960 | 1.260 | 56.344 | 0.71x |
| mixed.json | msgspec | 0.815 | 1.006 | 1.547 | 56.344 | 0.68x |
| mixed.json | ujson | 1.055 | 1.343 | 1.926 | 56.344 | 0.51x |
| mixed.json | json | 1.376 | 1.721 | 2.155 | 56.344 | 0.40x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 26.388 | 29.790 | 35.591 | 60.762 | 1.00x |
| users.ndjson | orjson | 37.207 | 39.314 | 48.152 | 60.762 | 0.76x |
| users.ndjson | msgspec | 37.445 | 39.989 | 48.995 | 60.762 | 0.74x |
| users.ndjson | ujson | 53.598 | 57.548 | 68.798 | 60.762 | 0.52x |
| users.ndjson | json | 66.772 | 74.789 | 84.057 | 60.762 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 5.511 | 5.991 | 19.105 | 55.477 | 1.00x |
| users.json | orjson | 5.989 | 6.872 | 22.177 | 55.477 | 0.87x |
| users.json | msgspec | 9.508 | 10.847 | 67.616 | 55.477 | 0.55x |
| users.json | ujson | 46.242 | 51.536 | 87.417 | 55.477 | 0.12x |
| users.json | json | 61.609 | 70.506 | 81.651 | 55.477 | 0.08x |
| flat.json | strata | 0.921 | 1.118 | 1.350 | 58.230 | 1.00x |
| flat.json | orjson | 1.078 | 1.486 | 2.048 | 58.230 | 0.75x |
| flat.json | msgspec | 1.233 | 1.526 | 2.262 | 58.230 | 0.73x |
| flat.json | ujson | 4.370 | 4.959 | 5.772 | 58.230 | 0.23x |
| flat.json | json | 5.823 | 6.854 | 7.966 | 58.230 | 0.16x |
| nested.json | strata | 0.798 | 0.906 | 1.286 | 53.277 | 1.00x |
| nested.json | orjson | 0.991 | 1.183 | 1.604 | 53.277 | 0.77x |
| nested.json | msgspec | 1.325 | 1.566 | 2.158 | 53.277 | 0.58x |
| nested.json | ujson | 4.576 | 5.074 | 6.936 | 53.277 | 0.18x |
| nested.json | json | 7.312 | 8.097 | 8.940 | 53.277 | 0.11x |
| wide_arrays.json | strata | 3.363 | 3.632 | 4.759 | 61.176 | 1.00x |
| wide_arrays.json | orjson | 4.328 | 4.674 | 5.998 | 61.176 | 0.78x |
| wide_arrays.json | msgspec | 5.288 | 6.144 | 7.095 | 61.176 | 0.59x |
| wide_arrays.json | ujson | 22.365 | 23.921 | 27.291 | 61.176 | 0.15x |
| wide_arrays.json | json | 48.019 | 50.877 | 57.274 | 61.176 | 0.07x |
| mixed.json | strata | 0.496 | 0.530 | 0.758 | 56.344 | 1.00x |
| mixed.json | orjson | 0.546 | 0.569 | 0.634 | 56.344 | 0.93x |
| mixed.json | msgspec | 0.569 | 0.600 | 0.887 | 56.344 | 0.88x |
| mixed.json | ujson | 1.272 | 1.388 | 1.925 | 56.344 | 0.38x |
| mixed.json | json | 1.740 | 1.849 | 2.503 | 56.344 | 0.29x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.225 | 0.271 | 0.394 | 55.555 | 1.00x |
| users.json $[*].id | jmespath | 1.339 | 1.367 | 1.882 | 55.555 | 0.20x |
| users.json $[*].id | jsonpath-ng | 7.568 | 8.357 | 9.331 | 55.555 | 0.03x |
| users.json $[*].orders[*].total | strata | 1.421 | 1.662 | 2.140 | 55.414 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 7.946 | 8.876 | 9.654 | 55.414 | 0.19x |
| users.json $[*].orders[*].total | jsonpath-ng | 48.000 | 51.243 | 63.561 | 55.414 | 0.03x |
| users.json $..total | strata | 4.576 | 5.004 | 6.853 | 56.492 | 1.00x |
| users.json $..total | jsonpath-ng | 1050.703 | 1374.807 | 1707.181 | 56.492 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.846 | 5.051 | 5.432 | 59.582 | 1.00x |
| users.json $[*].id | orjson+jmespath | 33.880 | 36.759 | 39.957 | 59.582 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 39.211 | 44.764 | 51.396 | 59.582 | 0.11x |
| users.json $[*].orders[*].total | strata | 5.590 | 7.029 | 55.843 | 56.652 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 47.281 | 84.084 | 233.916 | 56.652 | 0.08x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 106.582 | 153.243 | 322.890 | 56.652 | 0.05x |
| users.json $..total | strata | 32.304 | 33.334 | 47.925 | 56.684 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 1053.173 | 1146.693 | 1419.528 | 56.684 | 0.03x |

