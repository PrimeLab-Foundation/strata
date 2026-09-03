# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 6ef7acf
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
| users.json | strata | 20.383 | 21.595 | 26.978 | 52.645 | 1.00x |
| users.json | orjson | 27.839 | 29.780 | 36.739 | 52.645 | 0.73x |
| users.json | msgspec | 27.708 | 29.644 | 36.456 | 52.645 | 0.73x |
| users.json | ujson | 39.284 | 42.427 | 47.819 | 52.645 | 0.51x |
| users.json | pysimdjson | 173.156 | 180.837 | 189.040 | 52.645 | 0.12x |
| users.json | json | 45.172 | 49.494 | 55.267 | 52.645 | 0.44x |
| flat.json | strata | 1.563 | 1.738 | 2.363 | 61.750 | 1.00x |
| flat.json | orjson | 1.667 | 1.922 | 2.286 | 61.750 | 0.90x |
| flat.json | msgspec | 1.938 | 2.562 | 3.346 | 61.750 | 0.68x |
| flat.json | ujson | 3.224 | 4.046 | 4.964 | 61.750 | 0.43x |
| flat.json | pysimdjson | 16.923 | 20.609 | 25.178 | 61.750 | 0.08x |
| flat.json | json | 3.684 | 4.164 | 5.612 | 61.750 | 0.42x |
| nested.json | strata | 1.607 | 1.695 | 1.850 | 50.094 | 1.00x |
| nested.json | orjson | 1.833 | 1.911 | 2.033 | 50.094 | 0.89x |
| nested.json | msgspec | 2.030 | 2.103 | 3.508 | 50.094 | 0.81x |
| nested.json | ujson | 3.351 | 3.486 | 3.644 | 50.094 | 0.49x |
| nested.json | pysimdjson | 14.824 | 15.142 | 15.742 | 50.094 | 0.11x |
| nested.json | json | 4.282 | 4.390 | 5.046 | 50.094 | 0.39x |
| wide_arrays.json | strata | 9.111 | 9.542 | 21.677 | 61.398 | 1.00x |
| wide_arrays.json | orjson | 9.893 | 10.365 | 34.482 | 61.398 | 0.92x |
| wide_arrays.json | msgspec | 10.838 | 11.497 | 14.309 | 61.398 | 0.83x |
| wide_arrays.json | ujson | 14.068 | 14.481 | 16.981 | 61.398 | 0.66x |
| wide_arrays.json | pysimdjson | 84.521 | 88.088 | 147.388 | 61.398 | 0.11x |
| wide_arrays.json | json | 18.009 | 18.997 | 40.276 | 61.398 | 0.50x |
| mixed.json | strata | 0.425 | 0.441 | 0.477 | 60.160 | 1.00x |
| mixed.json | orjson | 0.506 | 0.526 | 0.641 | 60.160 | 0.84x |
| mixed.json | msgspec | 0.529 | 0.567 | 0.637 | 60.160 | 0.78x |
| mixed.json | ujson | 0.730 | 0.783 | 0.895 | 60.160 | 0.56x |
| mixed.json | pysimdjson | 3.669 | 3.865 | 4.386 | 60.160 | 0.11x |
| mixed.json | json | 1.018 | 1.062 | 1.101 | 60.160 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.632 | 2.946 | 3.195 | 48.266 | 1.00x |
| users.json | orjson | 3.551 | 3.843 | 3.935 | 48.266 | 0.77x |
| users.json | msgspec | 5.584 | 6.219 | 6.752 | 48.266 | 0.47x |
| users.json | ujson | 32.766 | 33.971 | 36.620 | 48.266 | 0.09x |
| users.json | json | 44.124 | 46.091 | 47.933 | 48.266 | 0.06x |
| flat.json | strata | 0.346 | 0.362 | 0.374 | 50.527 | 1.00x |
| flat.json | orjson | 0.454 | 0.473 | 0.749 | 50.527 | 0.76x |
| flat.json | msgspec | 0.606 | 0.635 | 0.649 | 50.527 | 0.57x |
| flat.json | ujson | 3.036 | 3.068 | 3.339 | 50.527 | 0.12x |
| flat.json | json | 4.188 | 4.209 | 4.236 | 50.527 | 0.09x |
| nested.json | strata | 0.260 | 0.269 | 0.400 | 49.840 | 1.00x |
| nested.json | orjson | 0.376 | 0.388 | 0.424 | 49.840 | 0.69x |
| nested.json | msgspec | 0.598 | 0.609 | 0.665 | 49.840 | 0.44x |
| nested.json | ujson | 3.045 | 3.097 | 3.438 | 49.840 | 0.09x |
| nested.json | json | 5.041 | 5.099 | 5.538 | 49.840 | 0.05x |
| wide_arrays.json | strata | 2.156 | 2.389 | 2.622 | 63.266 | 1.00x |
| wide_arrays.json | orjson | 2.649 | 2.904 | 3.227 | 63.266 | 0.82x |
| wide_arrays.json | msgspec | 3.477 | 3.723 | 4.072 | 63.266 | 0.64x |
| wide_arrays.json | ujson | 15.971 | 17.290 | 22.127 | 63.266 | 0.14x |
| wide_arrays.json | json | 35.948 | 39.550 | 42.860 | 63.266 | 0.06x |
| mixed.json | strata | 0.090 | 0.100 | 0.102 | 58.965 | 1.00x |
| mixed.json | orjson | 0.084 | 0.095 | 0.156 | 58.965 | 1.05x |
| mixed.json | msgspec | 0.122 | 0.137 | 0.145 | 58.965 | 0.73x |
| mixed.json | ujson | 0.694 | 0.702 | 0.743 | 58.965 | 0.14x |
| mixed.json | json | 1.069 | 1.093 | 1.304 | 58.965 | 0.09x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 22.709 | 23.359 | 24.888 | 60.578 | 1.00x |
| users.json | orjson | 32.463 | 33.120 | 36.886 | 60.578 | 0.71x |
| users.json | msgspec | 32.319 | 34.273 | 35.957 | 60.578 | 0.68x |
| users.json | ujson | 46.281 | 48.444 | 51.935 | 60.578 | 0.48x |
| users.json | json | 52.053 | 53.764 | 65.607 | 60.578 | 0.43x |
| flat.json | strata | 1.600 | 1.647 | 2.085 | 49.867 | 1.00x |
| flat.json | orjson | 1.750 | 1.827 | 2.495 | 49.867 | 0.90x |
| flat.json | msgspec | 2.024 | 2.108 | 3.448 | 49.867 | 0.78x |
| flat.json | ujson | 3.453 | 3.494 | 4.512 | 49.867 | 0.47x |
| flat.json | json | 3.838 | 3.905 | 4.311 | 49.867 | 0.42x |
| nested.json | strata | 1.725 | 1.775 | 1.874 | 49.840 | 1.00x |
| nested.json | orjson | 1.977 | 2.050 | 2.099 | 49.840 | 0.87x |
| nested.json | msgspec | 2.224 | 2.290 | 2.671 | 49.840 | 0.78x |
| nested.json | ujson | 3.470 | 3.589 | 3.773 | 49.840 | 0.49x |
| nested.json | json | 4.323 | 4.504 | 4.908 | 49.840 | 0.39x |
| wide_arrays.json | strata | 10.046 | 10.346 | 12.790 | 63.266 | 1.00x |
| wide_arrays.json | orjson | 10.715 | 11.363 | 13.081 | 63.266 | 0.91x |
| wide_arrays.json | msgspec | 11.960 | 13.452 | 14.851 | 63.266 | 0.77x |
| wide_arrays.json | ujson | 15.606 | 16.002 | 19.589 | 63.266 | 0.65x |
| wide_arrays.json | json | 19.645 | 21.600 | 24.739 | 63.266 | 0.48x |
| mixed.json | strata | 0.484 | 0.521 | 0.530 | 58.965 | 1.00x |
| mixed.json | orjson | 0.613 | 0.643 | 0.654 | 58.965 | 0.81x |
| mixed.json | msgspec | 0.630 | 0.675 | 0.707 | 58.965 | 0.77x |
| mixed.json | ujson | 0.809 | 0.887 | 0.908 | 58.965 | 0.59x |
| mixed.json | json | 1.092 | 1.146 | 1.261 | 58.965 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 22.107 | 26.277 | 32.044 | 62.090 | 1.00x |
| users.ndjson | orjson | 30.851 | 34.454 | 45.529 | 62.090 | 0.76x |
| users.ndjson | msgspec | 31.230 | 36.236 | 49.271 | 62.090 | 0.73x |
| users.ndjson | ujson | 47.073 | 53.108 | 68.168 | 62.090 | 0.49x |
| users.ndjson | json | 56.481 | 66.252 | 83.880 | 62.090 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.975 | 4.319 | 5.644 | 56.691 | 1.00x |
| users.json | orjson | 5.061 | 5.400 | 7.403 | 56.691 | 0.80x |
| users.json | msgspec | 6.673 | 7.080 | 7.747 | 56.691 | 0.61x |
| users.json | ujson | 36.629 | 38.333 | 42.371 | 56.691 | 0.11x |
| users.json | json | 48.570 | 49.571 | 60.475 | 56.691 | 0.09x |
| flat.json | strata | 0.724 | 0.793 | 1.575 | 49.867 | 1.00x |
| flat.json | orjson | 0.818 | 0.887 | 0.944 | 49.867 | 0.89x |
| flat.json | msgspec | 0.987 | 1.052 | 4.115 | 49.867 | 0.75x |
| flat.json | ujson | 3.385 | 3.516 | 26.459 | 49.867 | 0.23x |
| flat.json | json | 4.543 | 4.766 | 8.681 | 49.867 | 0.17x |
| nested.json | strata | 0.605 | 0.641 | 0.745 | 49.840 | 1.00x |
| nested.json | orjson | 0.738 | 0.803 | 0.897 | 49.840 | 0.80x |
| nested.json | msgspec | 0.937 | 1.019 | 1.156 | 49.840 | 0.63x |
| nested.json | ujson | 3.426 | 3.487 | 3.587 | 49.840 | 0.18x |
| nested.json | json | 5.303 | 5.533 | 5.855 | 49.840 | 0.12x |
| wide_arrays.json | strata | 3.115 | 3.235 | 3.969 | 63.266 | 1.00x |
| wide_arrays.json | orjson | 3.668 | 3.822 | 3.975 | 63.266 | 0.85x |
| wide_arrays.json | msgspec | 4.609 | 4.791 | 5.102 | 63.266 | 0.68x |
| wide_arrays.json | ujson | 18.963 | 19.268 | 22.644 | 63.266 | 0.17x |
| wide_arrays.json | json | 41.340 | 42.023 | 44.479 | 63.266 | 0.08x |
| mixed.json | strata | 0.388 | 0.443 | 0.661 | 58.965 | 1.00x |
| mixed.json | orjson | 0.397 | 0.477 | 0.567 | 58.965 | 0.93x |
| mixed.json | msgspec | 0.436 | 0.529 | 0.800 | 58.965 | 0.84x |
| mixed.json | ujson | 1.078 | 1.196 | 1.454 | 58.965 | 0.37x |
| mixed.json | json | 1.457 | 1.523 | 1.869 | 58.965 | 0.29x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.219 | 0.249 | 0.399 | 56.773 | 1.00x |
| users.json $[*].id | jmespath | 1.160 | 1.359 | 1.912 | 56.773 | 0.18x |
| users.json $[*].id | jsonpath-ng | 6.382 | 7.476 | 8.909 | 56.773 | 0.03x |
| users.json $[*].orders[*].total | strata | 1.273 | 1.392 | 1.719 | 54.938 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 7.113 | 7.612 | 8.948 | 54.938 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 41.853 | 44.144 | 47.758 | 54.938 | 0.03x |
| users.json $..total | strata | 3.867 | 4.151 | 4.551 | 57.004 | 1.00x |
| users.json $..total | jsonpath-ng | 804.139 | 861.345 | 1033.615 | 57.004 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.584 | 4.656 | 5.531 | 57.844 | 1.00x |
| users.json $[*].id | orjson+jmespath | 32.296 | 33.385 | 36.379 | 57.844 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 36.806 | 39.270 | 40.215 | 57.844 | 0.12x |
| users.json $[*].orders[*].total | strata | 4.650 | 4.853 | 5.282 | 56.203 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 35.696 | 37.619 | 39.025 | 56.203 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 76.318 | 79.160 | 84.896 | 56.203 | 0.06x |
| users.json $..total | strata | 24.028 | 26.171 | 28.224 | 56.309 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 777.553 | 849.818 | 1190.182 | 56.309 | 0.03x |

