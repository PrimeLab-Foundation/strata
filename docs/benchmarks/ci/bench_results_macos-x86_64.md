# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 060bd7b
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
| users.json | strata | 20.036 | 23.004 | 26.588 | 52.656 | 1.00x |
| users.json | orjson | 29.519 | 33.402 | 39.211 | 52.656 | 0.69x |
| users.json | msgspec | 29.443 | 31.709 | 42.707 | 52.656 | 0.73x |
| users.json | ujson | 41.776 | 52.540 | 56.643 | 52.656 | 0.44x |
| users.json | pysimdjson | 174.549 | 186.722 | 213.859 | 52.656 | 0.12x |
| users.json | json | 48.777 | 52.852 | 61.938 | 52.656 | 0.44x |
| flat.json | strata | 1.205 | 1.231 | 1.636 | 60.234 | 1.00x |
| flat.json | orjson | 1.315 | 1.336 | 1.386 | 60.234 | 0.92x |
| flat.json | msgspec | 1.495 | 1.534 | 1.577 | 60.234 | 0.80x |
| flat.json | ujson | 2.606 | 2.664 | 2.875 | 60.234 | 0.46x |
| flat.json | pysimdjson | 13.799 | 13.980 | 14.170 | 60.234 | 0.09x |
| flat.json | json | 2.994 | 3.012 | 3.087 | 60.234 | 0.41x |
| nested.json | strata | 1.439 | 1.502 | 1.680 | 57.492 | 1.00x |
| nested.json | orjson | 1.661 | 1.762 | 2.369 | 57.492 | 0.85x |
| nested.json | msgspec | 1.758 | 1.902 | 2.207 | 57.492 | 0.79x |
| nested.json | ujson | 2.871 | 3.129 | 4.730 | 57.492 | 0.48x |
| nested.json | pysimdjson | 13.096 | 13.742 | 16.076 | 57.492 | 0.11x |
| nested.json | json | 3.767 | 4.022 | 4.959 | 57.492 | 0.37x |
| wide_arrays.json | strata | 8.330 | 8.670 | 11.368 | 61.773 | 1.00x |
| wide_arrays.json | orjson | 9.052 | 9.591 | 9.943 | 61.773 | 0.90x |
| wide_arrays.json | msgspec | 9.902 | 10.502 | 11.040 | 61.773 | 0.83x |
| wide_arrays.json | ujson | 12.946 | 13.268 | 14.217 | 61.773 | 0.65x |
| wide_arrays.json | pysimdjson | 78.549 | 79.598 | 80.892 | 61.773 | 0.11x |
| wide_arrays.json | json | 16.617 | 17.463 | 20.357 | 61.773 | 0.50x |
| mixed.json | strata | 0.349 | 0.368 | 0.384 | 58.781 | 1.00x |
| mixed.json | orjson | 0.409 | 0.425 | 0.472 | 58.781 | 0.87x |
| mixed.json | msgspec | 0.430 | 0.450 | 0.461 | 58.781 | 0.82x |
| mixed.json | ujson | 0.603 | 0.619 | 0.747 | 58.781 | 0.59x |
| mixed.json | pysimdjson | 3.042 | 3.083 | 3.287 | 58.781 | 0.12x |
| mixed.json | json | 0.838 | 0.854 | 0.897 | 58.781 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.823 | 3.313 | 4.435 | 48.441 | 1.00x |
| users.json | orjson | 3.834 | 3.976 | 5.067 | 48.441 | 0.83x |
| users.json | msgspec | 6.346 | 6.566 | 8.250 | 48.441 | 0.50x |
| users.json | ujson | 33.007 | 34.917 | 38.774 | 48.441 | 0.09x |
| users.json | json | 44.882 | 49.128 | 55.036 | 48.441 | 0.07x |
| flat.json | strata | 0.278 | 0.301 | 0.350 | 57.355 | 1.00x |
| flat.json | orjson | 0.390 | 0.402 | 0.427 | 57.355 | 0.75x |
| flat.json | msgspec | 0.481 | 0.518 | 0.570 | 57.355 | 0.58x |
| flat.json | ujson | 2.481 | 2.520 | 2.838 | 57.355 | 0.12x |
| flat.json | json | 3.425 | 3.457 | 3.557 | 57.355 | 0.09x |
| nested.json | strata | 0.236 | 0.328 | 0.536 | 52.395 | 1.00x |
| nested.json | orjson | 0.355 | 0.450 | 0.994 | 52.395 | 0.73x |
| nested.json | msgspec | 0.557 | 0.632 | 0.964 | 52.395 | 0.52x |
| nested.json | ujson | 2.740 | 2.998 | 4.051 | 52.395 | 0.11x |
| nested.json | json | 4.495 | 5.108 | 7.735 | 52.395 | 0.06x |
| wide_arrays.json | strata | 1.567 | 1.733 | 2.065 | 60.293 | 1.00x |
| wide_arrays.json | orjson | 2.154 | 2.335 | 2.689 | 60.293 | 0.74x |
| wide_arrays.json | msgspec | 3.008 | 3.192 | 3.329 | 60.293 | 0.54x |
| wide_arrays.json | ujson | 14.884 | 15.031 | 16.092 | 60.293 | 0.12x |
| wide_arrays.json | json | 32.851 | 33.211 | 34.390 | 60.293 | 0.05x |
| mixed.json | strata | 0.062 | 0.079 | 0.152 | 55.461 | 1.00x |
| mixed.json | orjson | 0.076 | 0.091 | 0.166 | 55.461 | 0.87x |
| mixed.json | msgspec | 0.110 | 0.119 | 0.212 | 55.461 | 0.67x |
| mixed.json | ujson | 0.588 | 0.613 | 0.671 | 55.461 | 0.13x |
| mixed.json | json | 0.905 | 0.951 | 1.160 | 55.461 | 0.08x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 20.144 | 20.998 | 23.097 | 60.977 | 1.00x |
| users.json | orjson | 29.802 | 33.261 | 36.898 | 60.977 | 0.63x |
| users.json | msgspec | 30.407 | 31.712 | 37.101 | 60.977 | 0.66x |
| users.json | ujson | 42.299 | 46.468 | 51.884 | 60.977 | 0.45x |
| users.json | json | 46.333 | 52.351 | 56.344 | 60.977 | 0.40x |
| flat.json | strata | 1.331 | 1.355 | 1.426 | 57.355 | 1.00x |
| flat.json | orjson | 1.461 | 1.492 | 1.586 | 57.355 | 0.91x |
| flat.json | msgspec | 1.659 | 1.712 | 1.864 | 57.355 | 0.79x |
| flat.json | ujson | 2.760 | 2.827 | 3.468 | 57.355 | 0.48x |
| flat.json | json | 3.123 | 3.185 | 5.299 | 57.355 | 0.43x |
| nested.json | strata | 1.544 | 1.730 | 2.443 | 52.395 | 1.00x |
| nested.json | orjson | 1.799 | 2.000 | 2.971 | 52.395 | 0.86x |
| nested.json | msgspec | 2.013 | 2.223 | 3.723 | 52.395 | 0.78x |
| nested.json | ujson | 3.205 | 3.388 | 4.230 | 52.395 | 0.51x |
| nested.json | json | 3.948 | 4.146 | 5.154 | 52.395 | 0.42x |
| wide_arrays.json | strata | 8.344 | 8.402 | 8.515 | 60.293 | 1.00x |
| wide_arrays.json | orjson | 9.090 | 9.231 | 10.064 | 60.293 | 0.91x |
| wide_arrays.json | msgspec | 10.079 | 10.290 | 11.167 | 60.293 | 0.82x |
| wide_arrays.json | ujson | 12.974 | 13.401 | 14.260 | 60.293 | 0.63x |
| wide_arrays.json | json | 16.544 | 16.930 | 17.271 | 60.293 | 0.50x |
| mixed.json | strata | 0.424 | 0.472 | 0.684 | 55.461 | 1.00x |
| mixed.json | orjson | 0.526 | 0.597 | 0.772 | 55.461 | 0.79x |
| mixed.json | msgspec | 0.548 | 0.631 | 0.841 | 55.461 | 0.75x |
| mixed.json | ujson | 0.725 | 0.803 | 1.159 | 55.461 | 0.59x |
| mixed.json | json | 0.961 | 1.047 | 1.327 | 55.461 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 19.266 | 19.843 | 20.781 | 60.574 | 1.00x |
| users.ndjson | orjson | 26.149 | 26.893 | 28.644 | 60.574 | 0.74x |
| users.ndjson | msgspec | 26.729 | 28.151 | 34.031 | 60.574 | 0.70x |
| users.ndjson | ujson | 38.517 | 39.620 | 45.990 | 60.574 | 0.50x |
| users.ndjson | json | 47.148 | 48.967 | 51.933 | 60.574 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.703 | 4.152 | 4.975 | 52.461 | 1.00x |
| users.json | orjson | 4.575 | 5.273 | 6.244 | 52.461 | 0.79x |
| users.json | msgspec | 7.251 | 7.832 | 8.386 | 52.461 | 0.53x |
| users.json | ujson | 33.799 | 35.243 | 40.700 | 52.461 | 0.12x |
| users.json | json | 45.245 | 51.971 | 123.957 | 52.461 | 0.08x |
| flat.json | strata | 0.672 | 0.743 | 1.741 | 57.355 | 1.00x |
| flat.json | orjson | 0.792 | 0.845 | 0.993 | 57.355 | 0.88x |
| flat.json | msgspec | 0.892 | 0.986 | 1.178 | 57.355 | 0.75x |
| flat.json | ujson | 2.984 | 3.059 | 3.476 | 57.355 | 0.24x |
| flat.json | json | 3.954 | 4.130 | 5.390 | 57.355 | 0.18x |
| nested.json | strata | 0.548 | 0.583 | 0.640 | 52.395 | 1.00x |
| nested.json | orjson | 0.684 | 0.735 | 0.827 | 52.395 | 0.79x |
| nested.json | msgspec | 0.862 | 0.921 | 0.989 | 52.395 | 0.63x |
| nested.json | ujson | 3.062 | 3.184 | 3.307 | 52.395 | 0.18x |
| nested.json | json | 4.894 | 5.064 | 5.472 | 52.395 | 0.12x |
| wide_arrays.json | strata | 2.261 | 2.347 | 2.663 | 60.293 | 1.00x |
| wide_arrays.json | orjson | 3.042 | 3.204 | 3.346 | 60.293 | 0.73x |
| wide_arrays.json | msgspec | 3.750 | 3.877 | 4.206 | 60.293 | 0.61x |
| wide_arrays.json | ujson | 15.265 | 15.554 | 16.322 | 60.293 | 0.15x |
| wide_arrays.json | json | 33.091 | 33.980 | 34.508 | 60.293 | 0.07x |
| mixed.json | strata | 0.383 | 0.407 | 0.474 | 55.461 | 1.00x |
| mixed.json | orjson | 0.409 | 0.456 | 0.519 | 55.461 | 0.89x |
| mixed.json | msgspec | 0.452 | 0.489 | 0.523 | 55.461 | 0.83x |
| mixed.json | ujson | 0.955 | 1.028 | 2.124 | 55.461 | 0.40x |
| mixed.json | json | 1.301 | 1.355 | 2.485 | 55.461 | 0.30x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.196 | 0.219 | 0.366 | 52.547 | 1.00x |
| users.json $[*].id | jmespath | 1.060 | 1.124 | 2.301 | 52.547 | 0.20x |
| users.json $[*].id | jsonpath-ng | 5.554 | 5.783 | 11.106 | 52.547 | 0.04x |
| users.json $[*].orders[*].total | strata | 1.029 | 1.189 | 1.616 | 55.363 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 6.095 | 6.437 | 7.036 | 55.363 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 35.491 | 37.822 | 101.861 | 55.363 | 0.03x |
| users.json $..total | strata | 3.270 | 3.675 | 4.120 | 57.414 | 1.00x |
| users.json $..total | jsonpath-ng | 670.983 | 745.765 | 922.742 | 57.414 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.615 | 3.764 | 4.370 | 59.516 | 1.00x |
| users.json $[*].id | orjson+jmespath | 28.108 | 30.618 | 40.786 | 59.516 | 0.12x |
| users.json $[*].id | orjson+jsonpath-ng | 32.041 | 34.297 | 39.106 | 59.516 | 0.11x |
| users.json $[*].orders[*].total | strata | 3.856 | 4.118 | 4.806 | 56.586 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 32.490 | 34.259 | 37.236 | 56.586 | 0.12x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 67.147 | 73.020 | 80.000 | 56.586 | 0.06x |
| users.json $..total | strata | 22.815 | 23.556 | 25.487 | 56.738 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 719.685 | 742.134 | 931.729 | 56.738 | 0.03x |

