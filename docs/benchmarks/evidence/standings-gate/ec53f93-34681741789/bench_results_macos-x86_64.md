# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: ec53f936b8a9245edf5bfeec36c38539666a1775
- python: 3.12.10
- implementation: CPython
- platform: macOS-15.7.9-x86_64-i386-64bit
- machine: x86_64
- processor: Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz
- compiler_flags: -fno-strict-overflow -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -O3 -Wall -arch x86_64 -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -fomit-frame-pointer -march=native -flto=thin -fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata -bundle -undefined (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 16.817 | 18.123 | 26.151 | 57.129 | 1.00x |
| users.json | orjson | 26.699 | 30.356 | 36.499 | 57.129 | 0.60x |
| users.json | msgspec | 26.416 | 29.213 | 33.786 | 57.129 | 0.62x |
| users.json | ujson | 39.434 | 41.748 | 52.176 | 57.129 | 0.43x |
| users.json | pysimdjson | 157.800 | 165.525 | 171.276 | 57.129 | 0.11x |
| users.json | json | 42.896 | 46.940 | 51.623 | 57.129 | 0.39x |
| flat.json | strata | 1.195 | 1.234 | 1.569 | 66.648 | 1.00x |
| flat.json | orjson | 1.352 | 1.402 | 1.506 | 66.648 | 0.88x |
| flat.json | msgspec | 1.523 | 1.558 | 2.040 | 66.648 | 0.79x |
| flat.json | ujson | 2.662 | 2.833 | 3.138 | 66.648 | 0.44x |
| flat.json | pysimdjson | 13.879 | 14.367 | 14.678 | 66.648 | 0.09x |
| flat.json | json | 3.001 | 3.133 | 4.099 | 66.648 | 0.39x |
| nested.json | strata | 1.339 | 1.390 | 1.557 | 63.184 | 1.00x |
| nested.json | orjson | 1.559 | 1.625 | 1.767 | 63.184 | 0.86x |
| nested.json | msgspec | 1.717 | 1.824 | 2.013 | 63.184 | 0.76x |
| nested.json | ujson | 2.853 | 2.951 | 3.542 | 63.184 | 0.47x |
| nested.json | pysimdjson | 12.642 | 12.832 | 16.087 | 63.184 | 0.11x |
| nested.json | json | 3.667 | 3.792 | 4.466 | 63.184 | 0.37x |
| wide_arrays.json | strata | 7.218 | 7.824 | 8.881 | 67.594 | 1.00x |
| wide_arrays.json | orjson | 9.594 | 10.318 | 11.689 | 67.594 | 0.76x |
| wide_arrays.json | msgspec | 10.394 | 10.948 | 14.099 | 67.594 | 0.71x |
| wide_arrays.json | ujson | 12.886 | 14.544 | 17.278 | 67.594 | 0.54x |
| wide_arrays.json | pysimdjson | 76.030 | 80.414 | 91.728 | 67.594 | 0.10x |
| wide_arrays.json | json | 16.592 | 17.215 | 19.988 | 67.594 | 0.45x |
| mixed.json | strata | 0.350 | 0.357 | 0.490 | 65.496 | 1.00x |
| mixed.json | orjson | 0.433 | 0.457 | 0.571 | 65.496 | 0.78x |
| mixed.json | msgspec | 0.463 | 0.482 | 0.782 | 65.496 | 0.74x |
| mixed.json | ujson | 0.623 | 0.655 | 0.936 | 65.496 | 0.54x |
| mixed.json | pysimdjson | 3.069 | 3.242 | 4.314 | 65.496 | 0.11x |
| mixed.json | json | 0.871 | 0.941 | 1.289 | 65.496 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.680 | 2.923 | 4.957 | 53.020 | 1.00x |
| users.json | orjson | 3.543 | 3.700 | 9.557 | 53.020 | 0.79x |
| users.json | msgspec | 5.299 | 5.925 | 21.265 | 53.020 | 0.49x |
| users.json | ujson | 24.070 | 26.509 | 106.843 | 53.020 | 0.11x |
| users.json | json | 40.593 | 45.412 | 51.200 | 53.020 | 0.06x |
| flat.json | strata | 0.321 | 0.335 | 0.482 | 63.027 | 1.00x |
| flat.json | orjson | 0.398 | 0.416 | 0.784 | 63.027 | 0.80x |
| flat.json | msgspec | 0.515 | 0.544 | 0.604 | 63.027 | 0.61x |
| flat.json | ujson | 2.106 | 2.168 | 12.007 | 63.027 | 0.15x |
| flat.json | json | 3.505 | 3.614 | 6.472 | 63.027 | 0.09x |
| nested.json | strata | 0.229 | 0.237 | 0.362 | 56.852 | 1.00x |
| nested.json | orjson | 0.336 | 0.356 | 0.442 | 56.852 | 0.67x |
| nested.json | msgspec | 0.538 | 0.571 | 0.848 | 56.852 | 0.41x |
| nested.json | ujson | 2.204 | 2.251 | 2.606 | 56.852 | 0.11x |
| nested.json | json | 4.362 | 4.437 | 4.963 | 56.852 | 0.05x |
| wide_arrays.json | strata | 2.087 | 2.221 | 2.572 | 63.617 | 1.00x |
| wide_arrays.json | orjson | 2.619 | 2.817 | 4.234 | 63.617 | 0.79x |
| wide_arrays.json | msgspec | 3.575 | 3.693 | 4.321 | 63.617 | 0.60x |
| wide_arrays.json | ujson | 10.346 | 10.647 | 13.638 | 63.617 | 0.21x |
| wide_arrays.json | json | 33.237 | 34.153 | 38.983 | 63.617 | 0.07x |
| mixed.json | strata | 0.088 | 0.109 | 0.275 | 62.258 | 1.00x |
| mixed.json | orjson | 0.116 | 0.137 | 0.380 | 62.258 | 0.80x |
| mixed.json | msgspec | 0.146 | 0.185 | 0.271 | 62.258 | 0.59x |
| mixed.json | ujson | 0.466 | 0.611 | 0.903 | 62.258 | 0.18x |
| mixed.json | json | 0.971 | 1.385 | 2.936 | 62.258 | 0.08x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 17.599 | 18.685 | 21.739 | 63.805 | 1.00x |
| users.json | orjson | 28.158 | 31.669 | 37.074 | 63.805 | 0.59x |
| users.json | msgspec | 28.042 | 30.760 | 33.618 | 63.805 | 0.61x |
| users.json | ujson | 41.344 | 44.095 | 51.388 | 63.805 | 0.42x |
| users.json | json | 44.972 | 47.795 | 55.662 | 63.805 | 0.39x |
| flat.json | strata | 1.310 | 1.345 | 1.727 | 63.055 | 1.00x |
| flat.json | orjson | 1.468 | 1.507 | 1.578 | 63.055 | 0.89x |
| flat.json | msgspec | 1.681 | 1.707 | 1.893 | 63.055 | 0.79x |
| flat.json | ujson | 2.805 | 2.855 | 3.283 | 63.055 | 0.47x |
| flat.json | json | 3.142 | 3.166 | 3.335 | 63.055 | 0.43x |
| nested.json | strata | 1.546 | 1.585 | 1.895 | 57.133 | 1.00x |
| nested.json | orjson | 1.792 | 1.886 | 2.411 | 57.133 | 0.84x |
| nested.json | msgspec | 1.988 | 2.042 | 2.738 | 57.133 | 0.78x |
| nested.json | ujson | 3.140 | 3.240 | 4.452 | 57.133 | 0.49x |
| nested.json | json | 3.933 | 4.104 | 5.258 | 57.133 | 0.39x |
| wide_arrays.json | strata | 7.088 | 7.476 | 9.140 | 65.770 | 1.00x |
| wide_arrays.json | orjson | 9.089 | 9.526 | 13.395 | 65.770 | 0.78x |
| wide_arrays.json | msgspec | 9.875 | 10.792 | 14.299 | 65.770 | 0.69x |
| wide_arrays.json | ujson | 12.629 | 14.258 | 17.236 | 65.770 | 0.52x |
| wide_arrays.json | json | 16.332 | 17.725 | 20.635 | 65.770 | 0.42x |
| mixed.json | strata | 0.446 | 0.469 | 0.565 | 62.258 | 1.00x |
| mixed.json | orjson | 0.573 | 0.597 | 0.874 | 62.258 | 0.79x |
| mixed.json | msgspec | 0.594 | 0.657 | 0.940 | 62.258 | 0.71x |
| mixed.json | ujson | 0.784 | 0.827 | 1.081 | 62.258 | 0.57x |
| mixed.json | json | 1.023 | 1.053 | 1.432 | 62.258 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 17.960 | 18.768 | 25.513 | 65.148 | 1.00x |
| users.ndjson | orjson | 26.895 | 32.104 | 38.724 | 65.148 | 0.58x |
| users.ndjson | msgspec | 26.283 | 32.199 | 40.912 | 65.148 | 0.58x |
| users.ndjson | ujson | 39.462 | 40.549 | 59.776 | 65.148 | 0.46x |
| users.ndjson | json | 47.949 | 55.116 | 73.326 | 65.148 | 0.34x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.871 | 4.463 | 5.084 | 63.828 | 1.00x |
| users.json | orjson | 4.658 | 5.244 | 7.399 | 63.828 | 0.85x |
| users.json | msgspec | 6.642 | 7.671 | 9.230 | 63.828 | 0.58x |
| users.json | ujson | 27.843 | 31.320 | 32.905 | 63.828 | 0.14x |
| users.json | json | 41.639 | 45.148 | 56.692 | 63.828 | 0.10x |
| flat.json | strata | 0.636 | 0.712 | 0.756 | 63.055 | 1.00x |
| flat.json | orjson | 0.744 | 0.791 | 0.930 | 63.055 | 0.90x |
| flat.json | msgspec | 0.871 | 0.905 | 0.974 | 63.055 | 0.79x |
| flat.json | ujson | 2.477 | 2.534 | 3.242 | 63.055 | 0.28x |
| flat.json | json | 3.776 | 3.927 | 4.084 | 63.055 | 0.18x |
| nested.json | strata | 0.579 | 0.639 | 0.857 | 57.133 | 1.00x |
| nested.json | orjson | 0.786 | 0.842 | 1.375 | 57.133 | 0.76x |
| nested.json | msgspec | 0.962 | 1.105 | 1.523 | 57.133 | 0.58x |
| nested.json | ujson | 2.708 | 3.011 | 3.979 | 57.133 | 0.21x |
| nested.json | json | 4.921 | 5.249 | 7.004 | 57.133 | 0.12x |
| wide_arrays.json | strata | 2.891 | 3.082 | 3.328 | 65.770 | 1.00x |
| wide_arrays.json | orjson | 3.502 | 3.712 | 4.186 | 65.770 | 0.83x |
| wide_arrays.json | msgspec | 4.451 | 5.121 | 5.762 | 65.770 | 0.60x |
| wide_arrays.json | ujson | 11.574 | 11.767 | 12.431 | 65.770 | 0.26x |
| wide_arrays.json | json | 34.153 | 35.684 | 40.036 | 65.770 | 0.09x |
| mixed.json | strata | 0.465 | 0.518 | 0.619 | 62.258 | 1.00x |
| mixed.json | orjson | 0.444 | 0.638 | 0.771 | 62.258 | 0.81x |
| mixed.json | msgspec | 0.571 | 0.640 | 0.790 | 62.258 | 0.81x |
| mixed.json | ujson | 0.877 | 1.275 | 1.369 | 62.258 | 0.41x |
| mixed.json | json | 1.368 | 1.713 | 2.328 | 62.258 | 0.30x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.179 | 0.215 | 0.248 | 63.895 | 1.00x |
| users.json $[*].id | jmespath | 0.983 | 1.016 | 1.411 | 63.895 | 0.21x |
| users.json $[*].id | jsonpath-ng | 5.126 | 5.575 | 7.159 | 63.895 | 0.04x |
| users.json $[*].orders[*].total | strata | 1.242 | 1.414 | 2.092 | 61.320 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 6.482 | 6.882 | 8.011 | 61.320 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 35.169 | 37.813 | 41.682 | 61.320 | 0.04x |
| users.json $..total | strata | 3.522 | 3.753 | 5.137 | 61.430 | 1.00x |
| users.json $..total | jsonpath-ng | 716.765 | 740.260 | 881.732 | 61.430 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.016 | 4.178 | 5.055 | 63.957 | 1.00x |
| users.json $[*].id | orjson+jmespath | 28.869 | 32.023 | 37.102 | 63.957 | 0.13x |
| users.json $[*].id | orjson+jsonpath-ng | 33.644 | 37.070 | 40.531 | 63.957 | 0.11x |
| users.json $[*].orders[*].total | strata | 4.152 | 4.278 | 9.624 | 61.348 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 32.142 | 38.844 | 49.162 | 61.348 | 0.11x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 77.526 | 90.637 | 202.720 | 61.348 | 0.05x |
| users.json $..total | strata | 21.576 | 23.045 | 27.802 | 61.430 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 727.593 | 737.652 | 913.651 | 61.430 | 0.03x |

