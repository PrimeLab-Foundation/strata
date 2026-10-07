# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c16eaa65c61f1cf9d4fac46e9f6d8e6b9f3b087f
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
| users.json | strata | 18.736 | 21.906 | 27.519 | 56.980 | 1.00x |
| users.json | orjson | 26.597 | 29.434 | 40.937 | 56.980 | 0.74x |
| users.json | msgspec | 27.064 | 30.551 | 40.632 | 56.980 | 0.72x |
| users.json | ujson | 41.532 | 48.201 | 57.081 | 56.980 | 0.45x |
| users.json | pysimdjson | 169.570 | 193.092 | 237.226 | 56.980 | 0.11x |
| users.json | json | 43.861 | 54.175 | 70.228 | 56.980 | 0.40x |
| flat.json | strata | 1.352 | 1.399 | 1.974 | 67.871 | 1.00x |
| flat.json | orjson | 1.520 | 1.555 | 2.515 | 67.871 | 0.90x |
| flat.json | msgspec | 1.738 | 1.767 | 3.369 | 67.871 | 0.79x |
| flat.json | ujson | 3.031 | 3.538 | 5.249 | 67.871 | 0.40x |
| flat.json | pysimdjson | 16.086 | 17.018 | 18.641 | 67.871 | 0.08x |
| flat.json | json | 3.446 | 3.637 | 5.469 | 67.871 | 0.38x |
| nested.json | strata | 1.629 | 1.660 | 2.393 | 64.391 | 1.00x |
| nested.json | orjson | 1.831 | 1.940 | 2.283 | 64.391 | 0.86x |
| nested.json | msgspec | 1.992 | 2.131 | 2.558 | 64.391 | 0.78x |
| nested.json | ujson | 3.302 | 3.499 | 4.184 | 64.391 | 0.47x |
| nested.json | pysimdjson | 14.754 | 15.190 | 20.331 | 64.391 | 0.11x |
| nested.json | json | 4.283 | 4.451 | 5.628 | 64.391 | 0.37x |
| wide_arrays.json | strata | 8.368 | 8.562 | 9.319 | 68.809 | 1.00x |
| wide_arrays.json | orjson | 10.003 | 10.961 | 12.477 | 68.809 | 0.78x |
| wide_arrays.json | msgspec | 11.266 | 11.895 | 14.630 | 68.809 | 0.72x |
| wide_arrays.json | ujson | 14.082 | 14.770 | 16.660 | 68.809 | 0.58x |
| wide_arrays.json | pysimdjson | 86.973 | 88.742 | 94.099 | 68.809 | 0.10x |
| wide_arrays.json | json | 18.345 | 19.708 | 23.838 | 68.809 | 0.43x |
| mixed.json | strata | 0.396 | 0.416 | 0.441 | 66.711 | 1.00x |
| mixed.json | orjson | 0.489 | 0.513 | 0.677 | 66.711 | 0.81x |
| mixed.json | msgspec | 0.516 | 0.545 | 0.592 | 66.711 | 0.76x |
| mixed.json | ujson | 0.700 | 0.737 | 0.760 | 66.711 | 0.56x |
| mixed.json | pysimdjson | 3.616 | 3.755 | 4.087 | 66.711 | 0.11x |
| mixed.json | json | 0.992 | 1.035 | 1.102 | 66.711 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.974 | 3.529 | 4.632 | 52.887 | 1.00x |
| users.json | orjson | 3.840 | 4.192 | 6.336 | 52.887 | 0.84x |
| users.json | msgspec | 6.007 | 6.563 | 7.713 | 52.887 | 0.54x |
| users.json | ujson | 30.055 | 31.800 | 33.751 | 52.887 | 0.11x |
| users.json | json | 48.402 | 56.792 | 61.686 | 52.887 | 0.06x |
| flat.json | strata | 0.381 | 0.401 | 0.527 | 64.254 | 1.00x |
| flat.json | orjson | 0.465 | 0.485 | 0.710 | 64.254 | 0.83x |
| flat.json | msgspec | 0.609 | 0.660 | 0.943 | 64.254 | 0.61x |
| flat.json | ujson | 2.439 | 2.473 | 4.018 | 64.254 | 0.16x |
| flat.json | json | 4.021 | 4.100 | 5.247 | 64.254 | 0.10x |
| nested.json | strata | 0.252 | 0.281 | 0.295 | 58.066 | 1.00x |
| nested.json | orjson | 0.382 | 0.412 | 0.425 | 58.066 | 0.68x |
| nested.json | msgspec | 0.597 | 0.634 | 0.805 | 58.066 | 0.44x |
| nested.json | ujson | 2.530 | 2.608 | 2.730 | 58.066 | 0.11x |
| nested.json | json | 4.998 | 5.159 | 5.524 | 58.066 | 0.05x |
| wide_arrays.json | strata | 2.215 | 2.351 | 3.109 | 64.832 | 1.00x |
| wide_arrays.json | orjson | 2.799 | 2.958 | 3.716 | 64.832 | 0.79x |
| wide_arrays.json | msgspec | 3.823 | 4.725 | 5.543 | 64.832 | 0.50x |
| wide_arrays.json | ujson | 11.473 | 12.566 | 13.589 | 64.832 | 0.19x |
| wide_arrays.json | json | 37.642 | 41.349 | 46.371 | 64.832 | 0.06x |
| mixed.json | strata | 0.070 | 0.072 | 0.099 | 63.473 | 1.00x |
| mixed.json | orjson | 0.085 | 0.093 | 0.109 | 63.473 | 0.78x |
| mixed.json | msgspec | 0.123 | 0.133 | 0.174 | 63.473 | 0.54x |
| mixed.json | ujson | 0.509 | 0.539 | 0.629 | 63.473 | 0.13x |
| mixed.json | json | 1.071 | 1.124 | 1.258 | 63.473 | 0.06x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 20.924 | 22.075 | 27.778 | 63.680 | 1.00x |
| users.json | orjson | 30.601 | 35.026 | 42.540 | 63.680 | 0.63x |
| users.json | msgspec | 31.544 | 36.292 | 41.236 | 63.680 | 0.61x |
| users.json | ujson | 46.234 | 51.672 | 57.373 | 63.680 | 0.43x |
| users.json | json | 49.495 | 55.885 | 61.935 | 63.680 | 0.40x |
| flat.json | strata | 1.476 | 1.514 | 2.594 | 64.254 | 1.00x |
| flat.json | orjson | 1.693 | 1.700 | 1.818 | 64.254 | 0.89x |
| flat.json | msgspec | 1.919 | 1.949 | 2.653 | 64.254 | 0.78x |
| flat.json | ujson | 3.236 | 3.253 | 3.336 | 64.254 | 0.47x |
| flat.json | json | 3.609 | 3.625 | 5.159 | 64.254 | 0.42x |
| nested.json | strata | 1.676 | 1.766 | 1.811 | 58.348 | 1.00x |
| nested.json | orjson | 1.968 | 2.053 | 2.404 | 58.348 | 0.86x |
| nested.json | msgspec | 2.152 | 2.258 | 2.397 | 58.348 | 0.78x |
| nested.json | ujson | 3.542 | 3.602 | 3.765 | 58.348 | 0.49x |
| nested.json | json | 4.379 | 4.586 | 5.150 | 58.348 | 0.39x |
| wide_arrays.json | strata | 8.372 | 9.929 | 11.155 | 66.984 | 1.00x |
| wide_arrays.json | orjson | 10.751 | 12.203 | 15.592 | 66.984 | 0.81x |
| wide_arrays.json | msgspec | 11.523 | 13.552 | 16.770 | 66.984 | 0.73x |
| wide_arrays.json | ujson | 14.620 | 15.330 | 22.215 | 66.984 | 0.65x |
| wide_arrays.json | json | 19.085 | 20.386 | 27.629 | 66.984 | 0.49x |
| mixed.json | strata | 0.479 | 0.503 | 0.664 | 63.473 | 1.00x |
| mixed.json | orjson | 0.620 | 0.649 | 1.024 | 63.473 | 0.77x |
| mixed.json | msgspec | 0.656 | 0.693 | 1.070 | 63.473 | 0.73x |
| mixed.json | ujson | 0.861 | 0.916 | 1.141 | 63.473 | 0.55x |
| mixed.json | json | 1.119 | 1.183 | 1.332 | 63.473 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 20.871 | 24.183 | 30.206 | 66.992 | 1.00x |
| users.ndjson | orjson | 30.989 | 34.629 | 41.246 | 66.992 | 0.70x |
| users.ndjson | msgspec | 31.272 | 34.525 | 42.103 | 66.992 | 0.70x |
| users.ndjson | ujson | 45.674 | 50.843 | 58.138 | 66.992 | 0.48x |
| users.ndjson | json | 55.480 | 67.737 | 73.370 | 66.992 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.919 | 4.292 | 4.757 | 63.691 | 1.00x |
| users.json | orjson | 4.804 | 5.174 | 6.433 | 63.691 | 0.83x |
| users.json | msgspec | 6.913 | 7.436 | 12.476 | 63.691 | 0.58x |
| users.json | ujson | 29.083 | 31.821 | 34.476 | 63.691 | 0.13x |
| users.json | json | 47.508 | 52.002 | 61.795 | 63.691 | 0.08x |
| flat.json | strata | 0.733 | 0.769 | 1.086 | 64.254 | 1.00x |
| flat.json | orjson | 0.865 | 0.896 | 1.189 | 64.254 | 0.86x |
| flat.json | msgspec | 0.999 | 1.122 | 1.458 | 64.254 | 0.69x |
| flat.json | ujson | 2.910 | 3.020 | 3.764 | 64.254 | 0.25x |
| flat.json | json | 4.496 | 4.681 | 5.940 | 64.254 | 0.16x |
| nested.json | strata | 0.582 | 0.632 | 0.762 | 58.348 | 1.00x |
| nested.json | orjson | 0.733 | 0.758 | 1.009 | 58.348 | 0.83x |
| nested.json | msgspec | 0.944 | 0.982 | 1.200 | 58.348 | 0.64x |
| nested.json | ujson | 2.960 | 3.026 | 3.670 | 58.348 | 0.21x |
| nested.json | json | 5.434 | 5.648 | 6.054 | 58.348 | 0.11x |
| wide_arrays.json | strata | 3.132 | 3.897 | 4.304 | 66.984 | 1.00x |
| wide_arrays.json | orjson | 3.894 | 5.061 | 5.975 | 66.984 | 0.77x |
| wide_arrays.json | msgspec | 5.104 | 6.324 | 6.853 | 66.984 | 0.62x |
| wide_arrays.json | ujson | 14.265 | 16.913 | 20.924 | 66.984 | 0.23x |
| wide_arrays.json | json | 44.776 | 52.606 | 59.027 | 66.984 | 0.07x |
| mixed.json | strata | 0.416 | 0.447 | 0.582 | 63.473 | 1.00x |
| mixed.json | orjson | 0.405 | 0.502 | 0.600 | 63.473 | 0.89x |
| mixed.json | msgspec | 0.460 | 0.560 | 0.591 | 63.473 | 0.80x |
| mixed.json | ujson | 0.892 | 0.952 | 1.268 | 63.473 | 0.47x |
| mixed.json | json | 1.470 | 1.553 | 2.161 | 63.473 | 0.29x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.174 | 0.211 | 0.436 | 63.754 | 1.00x |
| users.json $[*].id | jmespath | 1.103 | 1.489 | 1.734 | 63.754 | 0.14x |
| users.json $[*].id | jsonpath-ng | 5.782 | 6.613 | 8.293 | 63.754 | 0.03x |
| users.json $[*].orders[*].total | strata | 1.448 | 2.030 | 2.145 | 61.246 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 7.714 | 10.308 | 12.262 | 61.246 | 0.20x |
| users.json $[*].orders[*].total | jsonpath-ng | 52.297 | 57.376 | 60.517 | 61.246 | 0.04x |
| users.json $..total | strata | 3.975 | 4.726 | 6.029 | 61.301 | 1.00x |
| users.json $..total | jsonpath-ng | 875.178 | 946.637 | 1068.702 | 61.301 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.419 | 6.317 | 8.252 | 63.824 | 1.00x |
| users.json $[*].id | orjson+jmespath | 31.489 | 45.596 | 78.350 | 63.824 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 39.785 | 52.284 | 59.989 | 63.824 | 0.12x |
| users.json $[*].orders[*].total | strata | 4.849 | 5.433 | 7.899 | 61.270 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 40.296 | 45.414 | 50.363 | 61.270 | 0.12x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 85.858 | 94.818 | 133.005 | 61.270 | 0.06x |
| users.json $..total | strata | 24.746 | 26.625 | 32.253 | 61.324 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 892.899 | 930.798 | 1108.909 | 61.324 | 0.03x |

