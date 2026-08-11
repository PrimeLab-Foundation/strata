# Benchmark results - ci-macos-15-intel

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 16b0a58
- python: 3.12.10
- implementation: CPython
- platform: macOS-15.7.7-x86_64-i386-64bit
- machine: x86_64
- processor: Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz
- compiler_flags: -std=c++20 -O3 -march=native
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 25.901 | 27.899 | 32.657 | 53.121 | 1.00x |
| users.json | orjson | 25.981 | 28.743 | 33.347 | 53.121 | 0.97x |
| users.json | msgspec | 26.185 | 28.724 | 32.183 | 53.121 | 0.97x |
| users.json | ujson | 39.058 | 41.825 | 48.004 | 53.121 | 0.67x |
| users.json | pysimdjson | 179.606 | 186.526 | 198.203 | 53.121 | 0.15x |
| users.json | json | 43.268 | 48.150 | 52.769 | 53.121 | 0.58x |
| flat.json | strata | 2.073 | 2.101 | 2.143 | 63.211 | 1.00x |
| flat.json | orjson | 1.606 | 1.636 | 1.695 | 63.211 | 1.28x |
| flat.json | msgspec | 1.793 | 1.825 | 1.868 | 63.211 | 1.15x |
| flat.json | ujson | 3.109 | 3.145 | 3.682 | 63.211 | 0.67x |
| flat.json | pysimdjson | 16.828 | 16.982 | 17.234 | 63.211 | 0.12x |
| flat.json | json | 3.656 | 3.680 | 4.034 | 63.211 | 0.57x |
| nested.json | strata | 2.196 | 2.219 | 2.266 | 58.680 | 1.00x |
| nested.json | orjson | 1.805 | 1.830 | 2.055 | 58.680 | 1.21x |
| nested.json | msgspec | 1.998 | 2.030 | 2.049 | 58.680 | 1.09x |
| nested.json | ujson | 3.350 | 3.397 | 3.435 | 58.680 | 0.65x |
| nested.json | pysimdjson | 14.939 | 15.067 | 15.303 | 58.680 | 0.15x |
| nested.json | json | 4.246 | 4.306 | 4.825 | 58.680 | 0.52x |
| wide_arrays.json | strata | 9.620 | 10.175 | 11.310 | 64.746 | 1.00x |
| wide_arrays.json | orjson | 9.834 | 10.396 | 10.924 | 64.746 | 0.98x |
| wide_arrays.json | msgspec | 10.758 | 11.304 | 12.584 | 64.746 | 0.90x |
| wide_arrays.json | ujson | 13.926 | 15.086 | 16.574 | 64.746 | 0.67x |
| wide_arrays.json | pysimdjson | 85.969 | 89.287 | 98.070 | 64.746 | 0.11x |
| wide_arrays.json | json | 18.107 | 19.464 | 20.497 | 64.746 | 0.52x |
| mixed.json | strata | 0.586 | 0.597 | 0.666 | 62.992 | 1.00x |
| mixed.json | orjson | 0.494 | 0.508 | 0.518 | 62.992 | 1.18x |
| mixed.json | msgspec | 0.528 | 0.540 | 0.575 | 62.992 | 1.11x |
| mixed.json | ujson | 0.732 | 0.748 | 0.947 | 62.992 | 0.80x |
| mixed.json | pysimdjson | 3.813 | 3.860 | 3.910 | 62.992 | 0.15x |
| mixed.json | json | 1.031 | 1.037 | 1.068 | 62.992 | 0.58x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.877 | 5.402 | 5.837 | 48.395 | 1.00x |
| users.json | orjson | 3.872 | 4.155 | 4.468 | 48.395 | 1.30x |
| users.json | msgspec | 5.791 | 6.333 | 7.163 | 48.395 | 0.85x |
| users.json | ujson | 34.315 | 38.273 | 43.986 | 48.395 | 0.14x |
| users.json | json | 43.527 | 52.045 | 54.959 | 48.395 | 0.10x |
| flat.json | strata | 0.524 | 0.545 | 0.570 | 60.145 | 1.00x |
| flat.json | orjson | 0.444 | 0.455 | 0.477 | 60.145 | 1.20x |
| flat.json | msgspec | 0.571 | 0.579 | 0.591 | 60.145 | 0.94x |
| flat.json | ujson | 3.092 | 3.108 | 3.286 | 60.145 | 0.18x |
| flat.json | json | 4.199 | 4.239 | 4.555 | 60.145 | 0.13x |
| nested.json | strata | 0.476 | 0.482 | 0.497 | 58.680 | 1.00x |
| nested.json | orjson | 0.408 | 0.415 | 0.436 | 58.680 | 1.16x |
| nested.json | msgspec | 0.593 | 0.604 | 0.694 | 58.680 | 0.80x |
| nested.json | ujson | 3.163 | 3.174 | 3.429 | 58.680 | 0.15x |
| nested.json | json | 5.168 | 5.202 | 5.321 | 58.680 | 0.09x |
| wide_arrays.json | strata | 2.447 | 2.530 | 2.724 | 62.871 | 1.00x |
| wide_arrays.json | orjson | 2.500 | 2.581 | 2.858 | 62.871 | 0.98x |
| wide_arrays.json | msgspec | 3.608 | 3.682 | 3.862 | 62.871 | 0.69x |
| wide_arrays.json | ujson | 16.892 | 17.142 | 19.336 | 62.871 | 0.15x |
| wide_arrays.json | json | 37.805 | 38.326 | 44.407 | 62.871 | 0.07x |
| mixed.json | strata | 0.136 | 0.139 | 0.149 | 60.926 | 1.00x |
| mixed.json | orjson | 0.083 | 0.088 | 0.147 | 60.926 | 1.57x |
| mixed.json | msgspec | 0.116 | 0.121 | 0.133 | 60.926 | 1.14x |
| mixed.json | ujson | 0.738 | 0.743 | 0.755 | 60.926 | 0.19x |
| mixed.json | json | 1.131 | 1.143 | 1.240 | 60.926 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 28.216 | 28.795 | 31.293 | 60.766 | 1.00x |
| users.json | orjson | 28.539 | 31.236 | 33.559 | 60.766 | 0.92x |
| users.json | msgspec | 29.463 | 31.758 | 33.328 | 60.766 | 0.91x |
| users.json | ujson | 43.701 | 45.658 | 49.269 | 60.766 | 0.63x |
| users.json | json | 49.505 | 50.951 | 55.418 | 60.766 | 0.57x |
| flat.json | strata | 2.137 | 2.200 | 2.304 | 60.145 | 1.00x |
| flat.json | orjson | 1.715 | 1.775 | 1.844 | 60.145 | 1.24x |
| flat.json | msgspec | 1.912 | 1.946 | 2.038 | 60.145 | 1.13x |
| flat.json | ujson | 3.198 | 3.260 | 3.385 | 60.145 | 0.67x |
| flat.json | json | 3.687 | 3.812 | 4.022 | 60.145 | 0.58x |
| nested.json | strata | 2.304 | 2.379 | 2.858 | 58.680 | 1.00x |
| nested.json | orjson | 1.959 | 1.990 | 2.554 | 58.680 | 1.20x |
| nested.json | msgspec | 2.187 | 2.208 | 2.345 | 58.680 | 1.08x |
| nested.json | ujson | 3.551 | 3.608 | 3.746 | 58.680 | 0.66x |
| nested.json | json | 4.423 | 4.461 | 4.976 | 58.680 | 0.53x |
| wide_arrays.json | strata | 10.158 | 10.195 | 10.273 | 62.871 | 1.00x |
| wide_arrays.json | orjson | 10.390 | 10.494 | 10.651 | 62.871 | 0.97x |
| wide_arrays.json | msgspec | 11.465 | 11.534 | 12.237 | 62.871 | 0.88x |
| wide_arrays.json | ujson | 15.154 | 15.303 | 15.585 | 62.871 | 0.67x |
| wide_arrays.json | json | 19.208 | 19.545 | 20.017 | 62.871 | 0.52x |
| mixed.json | strata | 0.667 | 0.673 | 0.732 | 60.926 | 1.00x |
| mixed.json | orjson | 0.607 | 0.616 | 0.658 | 60.926 | 1.09x |
| mixed.json | msgspec | 0.638 | 0.655 | 0.696 | 60.926 | 1.03x |
| mixed.json | ujson | 0.847 | 0.876 | 0.942 | 60.926 | 0.77x |
| mixed.json | json | 1.126 | 1.143 | 1.231 | 60.926 | 0.59x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 27.238 | 27.785 | 28.740 | 62.250 | 1.00x |
| users.ndjson | orjson | 29.800 | 30.170 | 32.195 | 62.250 | 0.92x |
| users.ndjson | msgspec | 30.661 | 31.374 | 32.281 | 62.250 | 0.89x |
| users.ndjson | ujson | 44.042 | 44.780 | 46.933 | 62.250 | 0.62x |
| users.ndjson | json | 55.321 | 56.045 | 60.219 | 62.250 | 0.50x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.275 | 6.427 | 7.190 | 57.879 | 1.00x |
| users.json | orjson | 4.737 | 4.907 | 5.633 | 57.879 | 1.31x |
| users.json | msgspec | 6.710 | 7.027 | 7.654 | 57.879 | 0.91x |
| users.json | ujson | 37.835 | 38.381 | 39.443 | 57.879 | 0.17x |
| users.json | json | 49.059 | 50.409 | 53.827 | 57.879 | 0.13x |
| flat.json | strata | 0.870 | 0.945 | 1.035 | 60.145 | 1.00x |
| flat.json | orjson | 0.778 | 0.816 | 0.868 | 60.145 | 1.16x |
| flat.json | msgspec | 0.851 | 0.926 | 1.430 | 60.145 | 1.02x |
| flat.json | ujson | 3.302 | 3.404 | 3.471 | 60.145 | 0.28x |
| flat.json | json | 4.473 | 4.616 | 5.472 | 60.145 | 0.20x |
| nested.json | strata | 0.841 | 0.873 | 0.943 | 58.680 | 1.00x |
| nested.json | orjson | 0.741 | 0.770 | 0.839 | 58.680 | 1.14x |
| nested.json | msgspec | 0.886 | 0.981 | 1.210 | 58.680 | 0.89x |
| nested.json | ujson | 3.526 | 3.599 | 3.928 | 58.680 | 0.24x |
| nested.json | json | 5.504 | 5.596 | 6.046 | 58.680 | 0.16x |
| wide_arrays.json | strata | 3.401 | 3.725 | 6.168 | 62.871 | 1.00x |
| wide_arrays.json | orjson | 3.536 | 3.783 | 4.111 | 62.871 | 0.98x |
| wide_arrays.json | msgspec | 4.493 | 4.748 | 5.658 | 62.871 | 0.78x |
| wide_arrays.json | ujson | 18.233 | 19.179 | 23.176 | 62.871 | 0.19x |
| wide_arrays.json | json | 40.240 | 42.050 | 47.021 | 62.871 | 0.09x |
| mixed.json | strata | 0.439 | 0.493 | 0.640 | 60.926 | 1.00x |
| mixed.json | orjson | 0.336 | 0.378 | 0.524 | 60.926 | 1.30x |
| mixed.json | msgspec | 0.366 | 0.422 | 0.498 | 60.926 | 1.17x |
| mixed.json | ujson | 1.031 | 1.072 | 1.110 | 60.926 | 0.46x |
| mixed.json | json | 1.416 | 1.465 | 1.541 | 60.926 | 0.34x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.152 | 0.158 | 0.184 | 56.488 | 1.00x |
| users.json $[*].id | jmespath | 1.065 | 1.074 | 1.153 | 56.488 | 0.15x |
| users.json $[*].id | jsonpath-ng | 5.999 | 6.030 | 6.160 | 56.488 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.896 | 0.961 | 1.023 | 56.648 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 6.549 | 6.667 | 7.171 | 56.648 | 0.14x |
| users.json $[*].orders[*].total | jsonpath-ng | 38.582 | 39.099 | 40.848 | 56.648 | 0.02x |
| users.json $..total | strata | 3.724 | 3.921 | 4.121 | 57.715 | 1.00x |
| users.json $..total | jsonpath-ng | 806.623 | 845.901 | 883.678 | 57.715 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 6.345 | 6.366 | 6.784 | 56.543 | 1.00x |
| users.json $[*].id | orjson+jmespath | 28.252 | 28.685 | 29.980 | 56.543 | 0.22x |
| users.json $[*].id | orjson+jsonpath-ng | 33.197 | 33.836 | 35.403 | 56.543 | 0.19x |
| users.json $[*].orders[*].total | strata | 6.728 | 7.164 | 7.947 | 57.863 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 35.226 | 38.785 | 42.993 | 57.863 | 0.18x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 74.363 | 79.323 | 92.476 | 57.863 | 0.09x |
| users.json $..total | strata | 30.500 | 31.988 | 34.284 | 57.906 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 833.223 | 852.805 | 890.559 | 57.906 | 0.04x |

