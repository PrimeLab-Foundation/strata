# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: e8ac860
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
| users.json | strata | 22.960 | 24.080 | 41.309 | 52.582 | 1.00x |
| users.json | orjson | 30.215 | 33.540 | 74.995 | 52.582 | 0.72x |
| users.json | msgspec | 31.419 | 34.475 | 52.672 | 52.582 | 0.70x |
| users.json | ujson | 45.396 | 50.584 | 66.814 | 52.582 | 0.48x |
| users.json | pysimdjson | 198.436 | 208.082 | 239.192 | 52.582 | 0.12x |
| users.json | json | 52.111 | 56.089 | 62.418 | 52.582 | 0.43x |
| flat.json | strata | 1.498 | 1.580 | 3.221 | 60.910 | 1.00x |
| flat.json | orjson | 1.589 | 1.757 | 2.407 | 60.910 | 0.90x |
| flat.json | msgspec | 1.837 | 2.167 | 3.324 | 60.910 | 0.73x |
| flat.json | ujson | 3.215 | 3.861 | 4.960 | 60.910 | 0.41x |
| flat.json | pysimdjson | 18.076 | 19.624 | 44.380 | 60.910 | 0.08x |
| flat.json | json | 3.763 | 4.196 | 5.757 | 60.910 | 0.38x |
| nested.json | strata | 1.676 | 2.177 | 3.216 | 50.160 | 1.00x |
| nested.json | orjson | 1.918 | 2.119 | 4.021 | 50.160 | 1.03x |
| nested.json | msgspec | 2.139 | 2.165 | 3.242 | 50.160 | 1.01x |
| nested.json | ujson | 3.488 | 3.600 | 4.767 | 50.160 | 0.60x |
| nested.json | pysimdjson | 15.535 | 17.670 | 65.405 | 50.160 | 0.12x |
| nested.json | json | 4.471 | 5.299 | 7.728 | 50.160 | 0.41x |
| wide_arrays.json | strata | 10.605 | 11.792 | 16.008 | 61.637 | 1.00x |
| wide_arrays.json | orjson | 11.379 | 13.329 | 30.833 | 61.637 | 0.88x |
| wide_arrays.json | msgspec | 12.567 | 14.283 | 33.222 | 61.637 | 0.83x |
| wide_arrays.json | ujson | 16.356 | 20.353 | 102.866 | 61.637 | 0.58x |
| wide_arrays.json | pysimdjson | 99.740 | 126.314 | 196.285 | 61.637 | 0.09x |
| wide_arrays.json | json | 21.082 | 26.785 | 34.554 | 61.637 | 0.44x |
| mixed.json | strata | 0.464 | 0.501 | 1.370 | 59.973 | 1.00x |
| mixed.json | orjson | 0.550 | 0.595 | 1.029 | 59.973 | 0.84x |
| mixed.json | msgspec | 0.569 | 0.611 | 0.919 | 59.973 | 0.82x |
| mixed.json | ujson | 0.798 | 0.962 | 2.042 | 59.973 | 0.52x |
| mixed.json | pysimdjson | 3.992 | 4.565 | 11.409 | 59.973 | 0.11x |
| mixed.json | json | 1.100 | 1.369 | 7.850 | 59.973 | 0.37x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.023 | 3.544 | 10.416 | 46.859 | 1.00x |
| users.json | orjson | 3.901 | 4.446 | 17.104 | 46.859 | 0.80x |
| users.json | msgspec | 6.603 | 7.366 | 8.302 | 46.859 | 0.48x |
| users.json | ujson | 38.027 | 40.345 | 41.948 | 46.859 | 0.09x |
| users.json | json | 52.090 | 56.176 | 72.492 | 46.859 | 0.06x |
| flat.json | strata | 0.352 | 0.375 | 0.704 | 49.758 | 1.00x |
| flat.json | orjson | 0.466 | 0.486 | 0.613 | 49.758 | 0.77x |
| flat.json | msgspec | 0.760 | 2.201 | 20.706 | 49.758 | 0.17x |
| flat.json | ujson | 3.119 | 3.187 | 4.411 | 49.758 | 0.12x |
| flat.json | json | 4.312 | 4.499 | 31.212 | 49.758 | 0.08x |
| nested.json | strata | 0.268 | 0.280 | 0.422 | 50.051 | 1.00x |
| nested.json | orjson | 0.392 | 0.436 | 1.661 | 50.051 | 0.64x |
| nested.json | msgspec | 0.622 | 0.666 | 2.440 | 50.051 | 0.42x |
| nested.json | ujson | 3.149 | 3.457 | 5.102 | 50.051 | 0.08x |
| nested.json | json | 5.186 | 5.803 | 6.937 | 50.051 | 0.05x |
| wide_arrays.json | strata | 2.202 | 2.369 | 13.179 | 60.293 | 1.00x |
| wide_arrays.json | orjson | 2.681 | 2.952 | 3.913 | 60.293 | 0.80x |
| wide_arrays.json | msgspec | 3.901 | 4.467 | 6.471 | 60.293 | 0.53x |
| wide_arrays.json | ujson | 18.756 | 20.258 | 26.312 | 60.293 | 0.12x |
| wide_arrays.json | json | 44.292 | 46.973 | 59.539 | 60.293 | 0.05x |
| mixed.json | strata | 0.098 | 0.124 | 0.261 | 55.719 | 1.00x |
| mixed.json | orjson | 0.091 | 0.154 | 0.331 | 55.719 | 0.81x |
| mixed.json | msgspec | 0.130 | 0.172 | 0.245 | 55.719 | 0.72x |
| mixed.json | ujson | 0.773 | 0.852 | 1.900 | 55.719 | 0.15x |
| mixed.json | json | 1.195 | 1.255 | 2.363 | 55.719 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 23.873 | 25.900 | 36.731 | 59.074 | 1.00x |
| users.json | orjson | 33.946 | 37.609 | 51.198 | 59.074 | 0.69x |
| users.json | msgspec | 32.559 | 38.924 | 66.786 | 59.074 | 0.67x |
| users.json | ujson | 48.070 | 56.433 | 72.255 | 59.074 | 0.46x |
| users.json | json | 60.486 | 64.979 | 71.468 | 59.074 | 0.40x |
| flat.json | strata | 1.561 | 1.676 | 6.373 | 49.922 | 1.00x |
| flat.json | orjson | 1.727 | 1.847 | 10.299 | 49.922 | 0.91x |
| flat.json | msgspec | 2.011 | 2.083 | 2.958 | 49.922 | 0.80x |
| flat.json | ujson | 3.414 | 3.467 | 5.496 | 49.922 | 0.48x |
| flat.json | json | 3.822 | 4.071 | 30.745 | 49.922 | 0.41x |
| nested.json | strata | 1.846 | 2.033 | 3.750 | 50.051 | 1.00x |
| nested.json | orjson | 2.096 | 2.548 | 4.529 | 50.051 | 0.80x |
| nested.json | msgspec | 2.478 | 2.788 | 8.600 | 50.051 | 0.73x |
| nested.json | ujson | 3.925 | 4.513 | 6.523 | 50.051 | 0.45x |
| nested.json | json | 4.854 | 5.287 | 8.830 | 50.051 | 0.38x |
| wide_arrays.json | strata | 10.063 | 11.888 | 23.842 | 60.418 | 1.00x |
| wide_arrays.json | orjson | 11.251 | 13.907 | 35.702 | 60.418 | 0.85x |
| wide_arrays.json | msgspec | 12.181 | 14.216 | 20.445 | 60.418 | 0.84x |
| wide_arrays.json | ujson | 16.026 | 18.562 | 26.089 | 60.418 | 0.64x |
| wide_arrays.json | json | 20.289 | 24.864 | 36.660 | 60.418 | 0.48x |
| mixed.json | strata | 0.556 | 0.681 | 1.083 | 55.719 | 1.00x |
| mixed.json | orjson | 0.680 | 0.817 | 1.563 | 55.719 | 0.83x |
| mixed.json | msgspec | 0.717 | 0.833 | 5.891 | 55.719 | 0.82x |
| mixed.json | ujson | 0.957 | 1.200 | 1.688 | 55.719 | 0.57x |
| mixed.json | json | 1.214 | 1.547 | 2.199 | 55.719 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 24.599 | 27.316 | 32.013 | 59.227 | 1.00x |
| users.ndjson | orjson | 35.042 | 38.234 | 61.882 | 59.227 | 0.71x |
| users.ndjson | msgspec | 35.894 | 37.927 | 71.034 | 59.227 | 0.72x |
| users.ndjson | ujson | 51.046 | 56.928 | 77.912 | 59.227 | 0.48x |
| users.ndjson | json | 67.410 | 79.682 | 92.464 | 59.227 | 0.34x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.246 | 5.223 | 40.341 | 56.227 | 1.00x |
| users.json | orjson | 5.126 | 5.888 | 48.273 | 56.227 | 0.89x |
| users.json | msgspec | 7.096 | 7.848 | 23.032 | 56.227 | 0.67x |
| users.json | ujson | 38.412 | 40.629 | 147.661 | 56.227 | 0.13x |
| users.json | json | 50.294 | 54.382 | 134.885 | 56.227 | 0.10x |
| flat.json | strata | 0.786 | 1.039 | 54.149 | 49.922 | 1.00x |
| flat.json | orjson | 0.893 | 0.987 | 1.382 | 49.922 | 1.05x |
| flat.json | msgspec | 1.062 | 1.289 | 16.177 | 49.922 | 0.81x |
| flat.json | ujson | 3.565 | 4.337 | 12.658 | 49.922 | 0.24x |
| flat.json | json | 4.724 | 5.142 | 30.839 | 49.922 | 0.20x |
| nested.json | strata | 0.679 | 0.733 | 1.630 | 50.051 | 1.00x |
| nested.json | orjson | 0.843 | 1.309 | 3.530 | 50.051 | 0.56x |
| nested.json | msgspec | 1.031 | 1.468 | 2.099 | 50.051 | 0.50x |
| nested.json | ujson | 4.732 | 5.582 | 7.290 | 50.051 | 0.13x |
| nested.json | json | 6.401 | 8.172 | 23.832 | 50.051 | 0.09x |
| wide_arrays.json | strata | 3.119 | 3.739 | 24.889 | 60.418 | 1.00x |
| wide_arrays.json | orjson | 3.462 | 4.003 | 5.938 | 60.418 | 0.93x |
| wide_arrays.json | msgspec | 4.690 | 5.372 | 32.899 | 60.418 | 0.70x |
| wide_arrays.json | ujson | 19.390 | 23.473 | 35.819 | 60.418 | 0.16x |
| wide_arrays.json | json | 42.284 | 51.522 | 58.684 | 60.418 | 0.07x |
| mixed.json | strata | 0.469 | 0.653 | 1.411 | 55.719 | 1.00x |
| mixed.json | orjson | 0.517 | 0.565 | 0.928 | 55.719 | 1.16x |
| mixed.json | msgspec | 0.631 | 0.720 | 1.007 | 55.719 | 0.91x |
| mixed.json | ujson | 1.231 | 1.644 | 2.056 | 55.719 | 0.40x |
| mixed.json | json | 1.667 | 1.915 | 2.608 | 55.719 | 0.34x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.197 | 0.240 | 4.443 | 56.312 | 1.00x |
| users.json $[*].id | jmespath | 1.086 | 1.214 | 2.904 | 56.312 | 0.20x |
| users.json $[*].id | jsonpath-ng | 6.286 | 7.282 | 51.302 | 56.312 | 0.03x |
| users.json $[*].orders[*].total | strata | 1.117 | 1.251 | 11.367 | 54.148 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 7.334 | 7.912 | 8.446 | 54.148 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 42.560 | 43.466 | 45.766 | 54.148 | 0.03x |
| users.json $..total | strata | 3.859 | 4.359 | 10.212 | 56.258 | 1.00x |
| users.json $..total | jsonpath-ng | 930.867 | 1017.028 | 1664.019 | 56.258 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.648 | 5.064 | 7.305 | 56.375 | 1.00x |
| users.json $[*].id | orjson+jmespath | 31.256 | 39.274 | 54.124 | 56.375 | 0.13x |
| users.json $[*].id | orjson+jsonpath-ng | 36.356 | 42.729 | 62.663 | 56.375 | 0.12x |
| users.json $[*].orders[*].total | strata | 5.035 | 5.096 | 5.928 | 56.191 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 37.631 | 38.975 | 40.884 | 56.191 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 77.961 | 81.530 | 103.045 | 56.191 | 0.06x |
| users.json $..total | strata | 27.995 | 31.566 | 36.148 | 56.277 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 1079.332 | 1115.060 | 1427.946 | 56.277 | 0.03x |

