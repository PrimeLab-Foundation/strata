# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 565fab210bb851772fcefe65082041084aeafc14
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
| users.json | strata | 19.352 | 20.116 | 24.275 | 57.133 | 1.00x |
| users.json | orjson | 27.653 | 29.358 | 35.552 | 57.133 | 0.69x |
| users.json | msgspec | 27.238 | 29.698 | 34.089 | 57.133 | 0.68x |
| users.json | ujson | 40.027 | 42.867 | 48.068 | 57.133 | 0.47x |
| users.json | pysimdjson | 177.779 | 184.846 | 196.652 | 57.133 | 0.11x |
| users.json | json | 46.308 | 48.044 | 51.097 | 57.133 | 0.42x |
| flat.json | strata | 1.332 | 1.353 | 1.402 | 67.934 | 1.00x |
| flat.json | orjson | 1.487 | 1.507 | 1.545 | 67.934 | 0.90x |
| flat.json | msgspec | 1.686 | 1.707 | 1.821 | 67.934 | 0.79x |
| flat.json | ujson | 2.977 | 3.003 | 3.075 | 67.934 | 0.45x |
| flat.json | pysimdjson | 15.983 | 16.060 | 17.125 | 67.934 | 0.08x |
| flat.json | json | 3.409 | 3.464 | 3.787 | 67.934 | 0.39x |
| nested.json | strata | 1.577 | 1.673 | 1.818 | 64.574 | 1.00x |
| nested.json | orjson | 1.814 | 1.983 | 2.268 | 64.574 | 0.84x |
| nested.json | msgspec | 1.991 | 2.153 | 2.545 | 64.574 | 0.78x |
| nested.json | ujson | 3.396 | 3.520 | 4.057 | 64.574 | 0.48x |
| nested.json | pysimdjson | 15.111 | 15.434 | 17.661 | 64.574 | 0.11x |
| nested.json | json | 4.289 | 4.394 | 4.550 | 64.574 | 0.38x |
| wide_arrays.json | strata | 8.253 | 8.502 | 9.100 | 70.254 | 1.00x |
| wide_arrays.json | orjson | 10.522 | 10.945 | 12.247 | 70.254 | 0.78x |
| wide_arrays.json | msgspec | 11.349 | 11.661 | 12.243 | 70.254 | 0.73x |
| wide_arrays.json | ujson | 14.274 | 14.466 | 15.995 | 70.254 | 0.59x |
| wide_arrays.json | pysimdjson | 86.992 | 88.049 | 91.677 | 70.254 | 0.10x |
| wide_arrays.json | json | 18.638 | 19.494 | 20.936 | 70.254 | 0.44x |
| mixed.json | strata | 0.444 | 0.448 | 0.531 | 65.172 | 1.00x |
| mixed.json | orjson | 0.548 | 0.553 | 0.691 | 65.172 | 0.81x |
| mixed.json | msgspec | 0.577 | 0.594 | 0.676 | 65.172 | 0.75x |
| mixed.json | ujson | 0.790 | 0.802 | 0.929 | 65.172 | 0.56x |
| mixed.json | pysimdjson | 3.976 | 4.049 | 4.227 | 65.172 | 0.11x |
| mixed.json | json | 1.098 | 1.119 | 1.630 | 65.172 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.745 | 2.812 | 3.280 | 54.371 | 1.00x |
| users.json | orjson | 3.729 | 3.909 | 4.352 | 54.371 | 0.72x |
| users.json | msgspec | 5.692 | 5.878 | 6.588 | 54.371 | 0.48x |
| users.json | ujson | 26.825 | 27.483 | 32.437 | 54.371 | 0.10x |
| users.json | json | 45.530 | 48.679 | 56.523 | 54.371 | 0.06x |
| flat.json | strata | 0.374 | 0.380 | 0.430 | 64.301 | 1.00x |
| flat.json | orjson | 0.461 | 0.470 | 0.496 | 64.301 | 0.81x |
| flat.json | msgspec | 0.597 | 0.612 | 0.673 | 64.301 | 0.62x |
| flat.json | ujson | 2.436 | 2.491 | 2.969 | 64.301 | 0.15x |
| flat.json | json | 4.022 | 4.155 | 4.405 | 64.301 | 0.09x |
| nested.json | strata | 0.260 | 0.269 | 0.305 | 64.766 | 1.00x |
| nested.json | orjson | 0.394 | 0.402 | 0.648 | 64.766 | 0.67x |
| nested.json | msgspec | 0.625 | 0.639 | 0.685 | 64.766 | 0.42x |
| nested.json | ujson | 2.612 | 2.668 | 2.890 | 64.766 | 0.10x |
| nested.json | json | 5.209 | 5.230 | 6.001 | 64.766 | 0.05x |
| wide_arrays.json | strata | 2.236 | 2.332 | 2.499 | 65.074 | 1.00x |
| wide_arrays.json | orjson | 2.793 | 2.818 | 3.504 | 65.074 | 0.83x |
| wide_arrays.json | msgspec | 3.759 | 3.803 | 3.927 | 65.074 | 0.61x |
| wide_arrays.json | ujson | 12.216 | 12.352 | 13.623 | 65.074 | 0.19x |
| wide_arrays.json | json | 38.210 | 38.453 | 40.547 | 65.074 | 0.06x |
| mixed.json | strata | 0.091 | 0.093 | 0.112 | 63.035 | 1.00x |
| mixed.json | orjson | 0.103 | 0.109 | 0.118 | 63.035 | 0.85x |
| mixed.json | msgspec | 0.146 | 0.152 | 0.163 | 63.035 | 0.62x |
| mixed.json | ujson | 0.561 | 0.565 | 0.632 | 63.035 | 0.17x |
| mixed.json | json | 1.182 | 1.206 | 1.350 | 63.035 | 0.08x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 21.349 | 22.007 | 27.363 | 65.117 | 1.00x |
| users.json | orjson | 30.938 | 34.659 | 38.490 | 65.117 | 0.63x |
| users.json | msgspec | 31.014 | 34.991 | 46.800 | 65.117 | 0.63x |
| users.json | ujson | 44.805 | 51.727 | 59.454 | 65.117 | 0.43x |
| users.json | json | 50.173 | 54.148 | 70.025 | 65.117 | 0.41x |
| flat.json | strata | 1.465 | 1.477 | 1.738 | 64.301 | 1.00x |
| flat.json | orjson | 1.677 | 1.717 | 1.874 | 64.301 | 0.86x |
| flat.json | msgspec | 1.894 | 1.924 | 2.239 | 64.301 | 0.77x |
| flat.json | ujson | 3.220 | 3.254 | 3.969 | 64.301 | 0.45x |
| flat.json | json | 3.591 | 3.728 | 4.858 | 64.301 | 0.40x |
| nested.json | strata | 1.740 | 1.805 | 1.963 | 64.766 | 1.00x |
| nested.json | orjson | 1.998 | 2.077 | 2.136 | 64.766 | 0.87x |
| nested.json | msgspec | 2.212 | 2.301 | 2.343 | 64.766 | 0.78x |
| nested.json | ujson | 3.610 | 3.713 | 4.635 | 64.766 | 0.49x |
| nested.json | json | 4.499 | 4.678 | 5.269 | 64.766 | 0.39x |
| wide_arrays.json | strata | 8.039 | 8.266 | 9.480 | 66.426 | 1.00x |
| wide_arrays.json | orjson | 10.310 | 10.676 | 11.125 | 66.426 | 0.77x |
| wide_arrays.json | msgspec | 11.341 | 11.462 | 12.020 | 66.426 | 0.72x |
| wide_arrays.json | ujson | 14.548 | 15.120 | 16.358 | 66.426 | 0.55x |
| wide_arrays.json | json | 18.773 | 19.445 | 20.606 | 66.426 | 0.43x |
| mixed.json | strata | 0.551 | 0.570 | 0.743 | 63.035 | 1.00x |
| mixed.json | orjson | 0.724 | 0.757 | 0.905 | 63.035 | 0.75x |
| mixed.json | msgspec | 0.758 | 0.791 | 1.131 | 63.035 | 0.72x |
| mixed.json | ujson | 0.991 | 1.017 | 1.719 | 63.035 | 0.56x |
| mixed.json | json | 1.270 | 1.396 | 2.025 | 63.035 | 0.41x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 20.190 | 20.596 | 24.389 | 68.617 | 1.00x |
| users.ndjson | orjson | 29.161 | 29.868 | 33.273 | 68.617 | 0.69x |
| users.ndjson | msgspec | 30.238 | 31.503 | 34.337 | 68.617 | 0.65x |
| users.ndjson | ujson | 42.580 | 44.441 | 48.620 | 68.617 | 0.46x |
| users.ndjson | json | 53.408 | 55.501 | 59.527 | 68.617 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.108 | 4.608 | 7.064 | 65.133 | 1.00x |
| users.json | orjson | 4.955 | 5.478 | 8.567 | 65.133 | 0.84x |
| users.json | msgspec | 7.081 | 8.289 | 12.870 | 65.133 | 0.56x |
| users.json | ujson | 30.255 | 33.905 | 43.483 | 65.133 | 0.14x |
| users.json | json | 50.892 | 57.386 | 79.441 | 65.133 | 0.08x |
| flat.json | strata | 0.677 | 0.759 | 0.817 | 64.301 | 1.00x |
| flat.json | orjson | 0.841 | 0.901 | 1.028 | 64.301 | 0.84x |
| flat.json | msgspec | 0.966 | 1.062 | 1.370 | 64.301 | 0.71x |
| flat.json | ujson | 2.930 | 3.009 | 3.183 | 64.301 | 0.25x |
| flat.json | json | 4.504 | 4.552 | 4.751 | 64.301 | 0.17x |
| nested.json | strata | 0.580 | 0.664 | 0.863 | 64.766 | 1.00x |
| nested.json | orjson | 0.791 | 0.821 | 1.428 | 64.766 | 0.81x |
| nested.json | msgspec | 1.011 | 1.106 | 1.351 | 64.766 | 0.60x |
| nested.json | ujson | 3.030 | 3.249 | 3.670 | 64.766 | 0.20x |
| nested.json | json | 5.714 | 6.082 | 7.046 | 64.766 | 0.11x |
| wide_arrays.json | strata | 2.904 | 3.163 | 4.075 | 66.109 | 1.00x |
| wide_arrays.json | orjson | 3.681 | 3.845 | 4.550 | 66.109 | 0.82x |
| wide_arrays.json | msgspec | 4.786 | 5.151 | 6.477 | 66.109 | 0.61x |
| wide_arrays.json | ujson | 13.969 | 15.028 | 19.151 | 66.109 | 0.21x |
| wide_arrays.json | json | 42.355 | 44.573 | 49.172 | 66.109 | 0.07x |
| mixed.json | strata | 0.452 | 0.532 | 0.589 | 63.035 | 1.00x |
| mixed.json | orjson | 0.530 | 0.587 | 0.702 | 63.035 | 0.91x |
| mixed.json | msgspec | 0.519 | 0.596 | 0.833 | 63.035 | 0.89x |
| mixed.json | ujson | 1.028 | 1.075 | 1.248 | 63.035 | 0.50x |
| mixed.json | json | 1.632 | 1.721 | 2.218 | 63.035 | 0.31x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.181 | 0.193 | 0.232 | 65.203 | 1.00x |
| users.json $[*].id | jmespath | 1.111 | 1.138 | 1.217 | 65.203 | 0.17x |
| users.json $[*].id | jsonpath-ng | 6.030 | 6.240 | 7.017 | 65.203 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.884 | 0.931 | 1.122 | 61.602 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 6.006 | 6.135 | 6.541 | 61.602 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 35.909 | 36.461 | 41.123 | 61.602 | 0.03x |
| users.json $..total | strata | 3.747 | 4.167 | 4.693 | 61.645 | 1.00x |
| users.json $..total | jsonpath-ng | 773.459 | 842.385 | 918.076 | 61.645 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.171 | 4.276 | 4.637 | 65.266 | 1.00x |
| users.json $[*].id | orjson+jmespath | 28.239 | 29.456 | 37.675 | 65.266 | 0.15x |
| users.json $[*].id | orjson+jsonpath-ng | 32.778 | 34.194 | 38.553 | 65.266 | 0.13x |
| users.json $[*].orders[*].total | strata | 4.283 | 4.589 | 5.867 | 61.609 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 33.763 | 37.311 | 46.008 | 61.609 | 0.12x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 74.806 | 84.410 | 100.293 | 61.609 | 0.05x |
| users.json $..total | strata | 24.208 | 25.846 | 31.525 | 61.660 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 825.449 | 850.617 | 950.967 | 61.660 | 0.03x |

