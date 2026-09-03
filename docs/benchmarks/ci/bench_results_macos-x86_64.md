# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 85e2353
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
| users.json | strata | 21.829 | 23.912 | 32.138 | 52.738 | 1.00x |
| users.json | orjson | 29.638 | 33.452 | 39.616 | 52.738 | 0.71x |
| users.json | msgspec | 33.559 | 35.194 | 39.974 | 52.738 | 0.68x |
| users.json | ujson | 43.817 | 49.891 | 61.359 | 52.738 | 0.48x |
| users.json | pysimdjson | 188.806 | 202.038 | 228.233 | 52.738 | 0.12x |
| users.json | json | 48.735 | 53.438 | 60.121 | 52.738 | 0.45x |
| flat.json | strata | 1.518 | 1.537 | 1.896 | 60.785 | 1.00x |
| flat.json | orjson | 1.645 | 1.687 | 2.009 | 60.785 | 0.91x |
| flat.json | msgspec | 1.936 | 2.054 | 2.224 | 60.785 | 0.75x |
| flat.json | ujson | 3.298 | 3.421 | 4.119 | 60.785 | 0.45x |
| flat.json | pysimdjson | 17.098 | 17.376 | 20.584 | 60.785 | 0.09x |
| flat.json | json | 3.728 | 3.774 | 4.726 | 60.785 | 0.41x |
| nested.json | strata | 1.740 | 1.921 | 2.331 | 53.223 | 1.00x |
| nested.json | orjson | 2.013 | 2.140 | 2.522 | 53.223 | 0.90x |
| nested.json | msgspec | 2.310 | 2.492 | 2.856 | 53.223 | 0.77x |
| nested.json | ujson | 3.656 | 3.996 | 5.845 | 53.223 | 0.48x |
| nested.json | pysimdjson | 15.893 | 16.530 | 21.982 | 53.223 | 0.12x |
| nested.json | json | 4.671 | 4.969 | 5.526 | 53.223 | 0.39x |
| wide_arrays.json | strata | 10.104 | 10.756 | 14.899 | 60.391 | 1.00x |
| wide_arrays.json | orjson | 11.425 | 12.234 | 14.901 | 60.391 | 0.88x |
| wide_arrays.json | msgspec | 12.252 | 12.785 | 17.093 | 60.391 | 0.84x |
| wide_arrays.json | ujson | 15.758 | 16.697 | 22.432 | 60.391 | 0.64x |
| wide_arrays.json | pysimdjson | 98.427 | 102.107 | 104.837 | 60.391 | 0.11x |
| wide_arrays.json | json | 20.101 | 21.905 | 25.954 | 60.391 | 0.49x |
| mixed.json | strata | 0.461 | 0.554 | 0.740 | 54.141 | 1.00x |
| mixed.json | orjson | 0.546 | 0.649 | 0.785 | 54.141 | 0.85x |
| mixed.json | msgspec | 0.586 | 0.759 | 0.886 | 54.141 | 0.73x |
| mixed.json | ujson | 0.812 | 0.906 | 1.152 | 54.141 | 0.61x |
| mixed.json | pysimdjson | 4.023 | 4.653 | 5.014 | 54.141 | 0.12x |
| mixed.json | json | 1.134 | 1.250 | 1.635 | 54.141 | 0.44x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.941 | 3.147 | 3.665 | 48.191 | 1.00x |
| users.json | orjson | 4.595 | 4.989 | 6.054 | 48.191 | 0.63x |
| users.json | msgspec | 5.820 | 6.159 | 8.267 | 48.191 | 0.51x |
| users.json | ujson | 35.814 | 37.485 | 46.651 | 48.191 | 0.08x |
| users.json | json | 46.199 | 49.059 | 56.731 | 48.191 | 0.06x |
| flat.json | strata | 0.371 | 0.390 | 0.528 | 53.082 | 1.00x |
| flat.json | orjson | 0.487 | 0.512 | 0.731 | 53.082 | 0.76x |
| flat.json | msgspec | 0.651 | 0.708 | 0.835 | 53.082 | 0.55x |
| flat.json | ujson | 3.106 | 3.202 | 3.935 | 53.082 | 0.12x |
| flat.json | json | 4.235 | 4.508 | 5.990 | 53.082 | 0.09x |
| nested.json | strata | 0.299 | 0.319 | 0.409 | 49.672 | 1.00x |
| nested.json | orjson | 0.432 | 0.500 | 0.613 | 49.672 | 0.64x |
| nested.json | msgspec | 0.683 | 0.710 | 0.941 | 49.672 | 0.45x |
| nested.json | ujson | 3.305 | 3.457 | 4.183 | 49.672 | 0.09x |
| nested.json | json | 5.456 | 5.733 | 7.990 | 49.672 | 0.06x |
| wide_arrays.json | strata | 2.219 | 2.288 | 2.742 | 54.234 | 1.00x |
| wide_arrays.json | orjson | 2.754 | 2.823 | 3.713 | 54.234 | 0.81x |
| wide_arrays.json | msgspec | 3.823 | 3.950 | 4.387 | 54.234 | 0.58x |
| wide_arrays.json | ujson | 17.753 | 18.096 | 24.363 | 54.234 | 0.13x |
| wide_arrays.json | json | 40.484 | 45.288 | 52.263 | 54.234 | 0.05x |
| mixed.json | strata | 0.113 | 0.124 | 0.183 | 53.809 | 1.00x |
| mixed.json | orjson | 0.106 | 0.140 | 0.213 | 53.809 | 0.88x |
| mixed.json | msgspec | 0.156 | 0.182 | 0.322 | 53.809 | 0.68x |
| mixed.json | ujson | 0.757 | 0.856 | 1.043 | 53.809 | 0.14x |
| mixed.json | json | 1.156 | 1.324 | 1.670 | 53.809 | 0.09x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 24.972 | 26.481 | 31.369 | 60.555 | 1.00x |
| users.json | orjson | 32.723 | 34.620 | 56.612 | 60.555 | 0.76x |
| users.json | msgspec | 31.939 | 38.783 | 59.523 | 60.555 | 0.68x |
| users.json | ujson | 47.867 | 55.791 | 92.081 | 60.555 | 0.47x |
| users.json | json | 51.314 | 62.120 | 90.110 | 60.555 | 0.43x |
| flat.json | strata | 1.652 | 1.716 | 2.408 | 53.082 | 1.00x |
| flat.json | orjson | 1.817 | 1.850 | 2.091 | 53.082 | 0.93x |
| flat.json | msgspec | 2.095 | 2.169 | 2.906 | 53.082 | 0.79x |
| flat.json | ujson | 3.480 | 3.531 | 4.416 | 53.082 | 0.49x |
| flat.json | json | 3.897 | 4.031 | 4.733 | 53.082 | 0.43x |
| nested.json | strata | 1.856 | 1.991 | 2.273 | 48.961 | 1.00x |
| nested.json | orjson | 2.190 | 2.350 | 2.939 | 48.961 | 0.85x |
| nested.json | msgspec | 2.472 | 2.572 | 3.518 | 48.961 | 0.77x |
| nested.json | ujson | 3.831 | 3.998 | 6.085 | 48.961 | 0.50x |
| nested.json | json | 4.857 | 5.947 | 6.646 | 48.961 | 0.33x |
| wide_arrays.json | strata | 10.888 | 13.465 | 15.731 | 57.246 | 1.00x |
| wide_arrays.json | orjson | 12.367 | 15.344 | 17.980 | 57.246 | 0.88x |
| wide_arrays.json | msgspec | 12.907 | 16.224 | 21.129 | 57.246 | 0.83x |
| wide_arrays.json | ujson | 17.458 | 21.749 | 24.967 | 57.246 | 0.62x |
| wide_arrays.json | json | 23.252 | 26.718 | 32.543 | 57.246 | 0.50x |
| mixed.json | strata | 0.552 | 0.621 | 0.903 | 53.809 | 1.00x |
| mixed.json | orjson | 0.704 | 0.751 | 0.915 | 53.809 | 0.83x |
| mixed.json | msgspec | 0.762 | 0.847 | 1.340 | 53.809 | 0.73x |
| mixed.json | ujson | 0.965 | 1.065 | 1.447 | 53.809 | 0.58x |
| mixed.json | json | 1.233 | 1.484 | 1.826 | 53.809 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 23.174 | 24.573 | 28.757 | 59.758 | 1.00x |
| users.ndjson | orjson | 32.227 | 35.280 | 41.783 | 59.758 | 0.70x |
| users.ndjson | msgspec | 33.993 | 35.508 | 41.585 | 59.758 | 0.69x |
| users.ndjson | ujson | 48.372 | 50.453 | 56.545 | 59.758 | 0.49x |
| users.ndjson | json | 60.251 | 63.399 | 68.359 | 59.758 | 0.39x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.999 | 4.771 | 5.767 | 56.102 | 1.00x |
| users.json | orjson | 6.299 | 6.832 | 9.180 | 56.102 | 0.70x |
| users.json | msgspec | 7.419 | 8.191 | 54.791 | 56.102 | 0.58x |
| users.json | ujson | 38.225 | 40.255 | 45.146 | 56.102 | 0.12x |
| users.json | json | 49.332 | 50.670 | 54.287 | 56.102 | 0.09x |
| flat.json | strata | 0.757 | 0.858 | 1.052 | 53.082 | 1.00x |
| flat.json | orjson | 0.938 | 0.992 | 1.242 | 53.082 | 0.87x |
| flat.json | msgspec | 1.099 | 1.173 | 1.648 | 53.082 | 0.73x |
| flat.json | ujson | 3.694 | 4.063 | 4.974 | 53.082 | 0.21x |
| flat.json | json | 4.856 | 5.312 | 6.113 | 53.082 | 0.16x |
| nested.json | strata | 0.707 | 0.769 | 1.095 | 48.961 | 1.00x |
| nested.json | orjson | 0.867 | 1.109 | 1.242 | 48.961 | 0.69x |
| nested.json | msgspec | 1.078 | 1.293 | 1.805 | 48.961 | 0.59x |
| nested.json | ujson | 3.841 | 4.110 | 5.246 | 48.961 | 0.19x |
| nested.json | json | 6.083 | 6.850 | 8.548 | 48.961 | 0.11x |
| wide_arrays.json | strata | 3.096 | 3.351 | 4.384 | 57.246 | 1.00x |
| wide_arrays.json | orjson | 3.842 | 4.374 | 5.591 | 57.246 | 0.77x |
| wide_arrays.json | msgspec | 4.949 | 5.352 | 5.874 | 57.246 | 0.63x |
| wide_arrays.json | ujson | 20.248 | 22.519 | 27.593 | 57.246 | 0.15x |
| wide_arrays.json | json | 44.676 | 48.974 | 56.860 | 57.246 | 0.07x |
| mixed.json | strata | 0.457 | 0.523 | 0.643 | 53.809 | 1.00x |
| mixed.json | orjson | 0.467 | 0.513 | 0.692 | 53.809 | 1.02x |
| mixed.json | msgspec | 0.555 | 0.610 | 0.782 | 53.809 | 0.86x |
| mixed.json | ujson | 1.220 | 1.332 | 1.801 | 53.809 | 0.39x |
| mixed.json | json | 1.607 | 1.869 | 2.607 | 53.809 | 0.28x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.167 | 0.185 | 0.214 | 56.160 | 1.00x |
| users.json $[*].id | jmespath | 1.097 | 1.143 | 1.852 | 56.160 | 0.16x |
| users.json $[*].id | jsonpath-ng | 5.853 | 5.902 | 8.767 | 56.160 | 0.03x |
| users.json $[*].orders[*].total | strata | 1.043 | 1.148 | 1.245 | 54.391 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 6.904 | 7.060 | 7.284 | 54.391 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 40.299 | 41.335 | 42.135 | 54.391 | 0.03x |
| users.json $..total | strata | 3.437 | 3.697 | 4.240 | 55.465 | 1.00x |
| users.json $..total | jsonpath-ng | 782.787 | 810.930 | 896.283 | 55.465 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.333 | 4.448 | 6.455 | 60.016 | 1.00x |
| users.json $[*].id | orjson+jmespath | 30.085 | 31.873 | 41.340 | 60.016 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 35.599 | 36.306 | 42.519 | 60.016 | 0.12x |
| users.json $[*].orders[*].total | strata | 4.539 | 4.658 | 4.887 | 54.660 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 34.495 | 35.516 | 37.998 | 54.660 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 72.555 | 73.124 | 74.771 | 54.660 | 0.06x |
| users.json $..total | strata | 27.530 | 28.219 | 31.630 | 54.703 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 900.684 | 945.824 | 1072.533 | 54.703 | 0.03x |

