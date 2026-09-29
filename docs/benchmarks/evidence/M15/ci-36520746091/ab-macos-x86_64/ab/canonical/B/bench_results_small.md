# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: afd1550cbabc9433e8292644444a23bb27bbc4e9
- python: 3.12.10
- implementation: CPython
- platform: macOS-15.7.9-x86_64-i386-64bit
- machine: x86_64
- processor: Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz
- compiler_flags: -fno-strict-overflow -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -O3 -Wall -arch x86_64 -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -fomit-frame-pointer -march=native -flto=thin -fprofile-use=/Users/runner/work/_temp/strata-arm/build/pgo/strata.profdata -bundle -undefined (24 recorded commands; see the JSON companion)
- repeats: 60
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 17.143 | 18.820 | 31.481 | 56.777 | 1.00x |
| users.json | orjson | 24.686 | 29.040 | 41.276 | 56.777 | 0.65x |
| users.json | msgspec | 24.648 | 29.766 | 40.919 | 56.777 | 0.63x |
| users.json | ujson | 35.605 | 43.048 | 58.632 | 56.777 | 0.44x |
| users.json | pysimdjson | 158.028 | 171.066 | 231.390 | 56.777 | 0.11x |
| users.json | json | 41.304 | 48.821 | 65.021 | 56.777 | 0.39x |
| flat.json | strata | 1.247 | 1.369 | 1.957 | 69.629 | 1.00x |
| flat.json | orjson | 1.390 | 1.524 | 2.000 | 69.629 | 0.90x |
| flat.json | msgspec | 1.571 | 1.717 | 2.364 | 69.629 | 0.80x |
| flat.json | ujson | 2.785 | 3.051 | 3.808 | 69.629 | 0.45x |
| flat.json | pysimdjson | 14.834 | 15.818 | 18.121 | 69.629 | 0.09x |
| flat.json | json | 3.193 | 3.432 | 4.310 | 69.629 | 0.40x |
| nested.json | strata | 1.537 | 1.745 | 2.251 | 58.691 | 1.00x |
| nested.json | orjson | 1.747 | 1.927 | 2.550 | 58.691 | 0.91x |
| nested.json | msgspec | 1.927 | 2.198 | 3.100 | 58.691 | 0.79x |
| nested.json | ujson | 3.158 | 3.509 | 5.033 | 58.691 | 0.50x |
| nested.json | pysimdjson | 14.003 | 15.465 | 17.375 | 58.691 | 0.11x |
| nested.json | json | 4.027 | 4.713 | 5.988 | 58.691 | 0.37x |
| wide_arrays.json | strata | 7.944 | 9.204 | 10.982 | 69.379 | 1.00x |
| wide_arrays.json | orjson | 10.334 | 12.341 | 13.871 | 69.379 | 0.75x |
| wide_arrays.json | msgspec | 11.084 | 12.809 | 15.848 | 69.379 | 0.72x |
| wide_arrays.json | ujson | 13.743 | 15.758 | 19.141 | 69.379 | 0.58x |
| wide_arrays.json | pysimdjson | 82.403 | 89.026 | 100.965 | 69.379 | 0.10x |
| wide_arrays.json | json | 17.933 | 22.349 | 25.213 | 69.379 | 0.41x |
| mixed.json | strata | 0.382 | 0.418 | 0.613 | 66.945 | 1.00x |
| mixed.json | orjson | 0.467 | 0.507 | 0.743 | 66.945 | 0.82x |
| mixed.json | msgspec | 0.498 | 0.560 | 0.774 | 66.945 | 0.75x |
| mixed.json | ujson | 0.684 | 0.771 | 1.094 | 66.945 | 0.54x |
| mixed.json | pysimdjson | 3.337 | 3.636 | 4.212 | 66.945 | 0.11x |
| mixed.json | json | 0.934 | 1.063 | 1.436 | 66.945 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.358 | 2.864 | 3.922 | 48.703 | 1.00x |
| users.json | orjson | 3.306 | 3.852 | 4.673 | 48.703 | 0.74x |
| users.json | msgspec | 5.111 | 5.766 | 6.983 | 48.703 | 0.50x |
| users.json | ujson | 23.510 | 25.300 | 28.416 | 48.703 | 0.11x |
| users.json | json | 40.382 | 42.956 | 60.216 | 48.703 | 0.07x |
| flat.json | strata | 0.356 | 0.382 | 0.529 | 58.992 | 1.00x |
| flat.json | orjson | 0.439 | 0.484 | 0.656 | 58.992 | 0.79x |
| flat.json | msgspec | 0.604 | 0.661 | 0.942 | 58.992 | 0.58x |
| flat.json | ujson | 2.347 | 2.576 | 3.316 | 58.992 | 0.15x |
| flat.json | json | 3.838 | 4.200 | 5.452 | 58.992 | 0.09x |
| nested.json | strata | 0.268 | 0.368 | 0.509 | 58.434 | 1.00x |
| nested.json | orjson | 0.393 | 0.507 | 0.718 | 58.434 | 0.73x |
| nested.json | msgspec | 0.598 | 0.717 | 1.035 | 58.434 | 0.51x |
| nested.json | ujson | 2.459 | 3.051 | 3.803 | 58.434 | 0.12x |
| nested.json | json | 4.967 | 6.052 | 7.501 | 58.434 | 0.06x |
| wide_arrays.json | strata | 2.214 | 2.404 | 3.135 | 64.879 | 1.00x |
| wide_arrays.json | orjson | 2.752 | 2.969 | 3.921 | 64.879 | 0.81x |
| wide_arrays.json | msgspec | 3.733 | 4.025 | 4.832 | 64.879 | 0.60x |
| wide_arrays.json | ujson | 11.046 | 12.153 | 14.521 | 64.879 | 0.20x |
| wide_arrays.json | json | 35.872 | 38.441 | 46.177 | 64.879 | 0.06x |
| mixed.json | strata | 0.077 | 0.098 | 0.158 | 63.707 | 1.00x |
| mixed.json | orjson | 0.093 | 0.116 | 0.174 | 63.707 | 0.84x |
| mixed.json | msgspec | 0.128 | 0.154 | 0.213 | 63.707 | 0.63x |
| mixed.json | ujson | 0.476 | 0.519 | 0.635 | 63.707 | 0.19x |
| mixed.json | json | 0.979 | 1.066 | 1.435 | 63.707 | 0.09x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 17.305 | 18.341 | 27.095 | 61.148 | 1.00x |
| users.json | orjson | 24.674 | 28.153 | 43.588 | 61.148 | 0.65x |
| users.json | msgspec | 24.498 | 28.742 | 48.198 | 61.148 | 0.64x |
| users.json | ujson | 36.074 | 41.095 | 65.312 | 61.148 | 0.45x |
| users.json | json | 40.852 | 46.601 | 140.359 | 61.148 | 0.39x |
| flat.json | strata | 1.437 | 1.573 | 1.972 | 59.000 | 1.00x |
| flat.json | orjson | 1.649 | 1.807 | 2.262 | 59.000 | 0.87x |
| flat.json | msgspec | 1.862 | 2.011 | 2.578 | 59.000 | 0.78x |
| flat.json | ujson | 3.121 | 3.400 | 4.265 | 59.000 | 0.46x |
| flat.json | json | 3.455 | 3.826 | 4.850 | 59.000 | 0.41x |
| nested.json | strata | 1.663 | 1.809 | 2.461 | 58.922 | 1.00x |
| nested.json | orjson | 1.894 | 2.121 | 2.872 | 58.922 | 0.85x |
| nested.json | msgspec | 2.102 | 2.370 | 2.991 | 58.922 | 0.76x |
| nested.json | ujson | 3.390 | 3.710 | 4.700 | 58.922 | 0.49x |
| nested.json | json | 4.227 | 4.570 | 5.393 | 58.922 | 0.40x |
| wide_arrays.json | strata | 7.866 | 8.790 | 11.345 | 67.016 | 1.00x |
| wide_arrays.json | orjson | 10.250 | 11.917 | 14.470 | 67.016 | 0.74x |
| wide_arrays.json | msgspec | 11.069 | 13.143 | 15.671 | 67.016 | 0.67x |
| wide_arrays.json | ujson | 14.214 | 16.186 | 20.096 | 67.016 | 0.54x |
| wide_arrays.json | json | 18.208 | 20.854 | 26.140 | 67.016 | 0.42x |
| mixed.json | strata | 0.481 | 0.521 | 0.648 | 63.707 | 1.00x |
| mixed.json | orjson | 0.619 | 0.681 | 0.900 | 63.707 | 0.77x |
| mixed.json | msgspec | 0.657 | 0.728 | 0.920 | 63.707 | 0.72x |
| mixed.json | ujson | 0.841 | 0.945 | 1.240 | 63.707 | 0.55x |
| mixed.json | json | 1.077 | 1.182 | 1.611 | 63.707 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 18.310 | 19.195 | 20.743 | 68.516 | 1.00x |
| users.ndjson | orjson | 26.188 | 28.323 | 32.155 | 68.516 | 0.68x |
| users.ndjson | msgspec | 26.772 | 29.210 | 32.461 | 68.516 | 0.66x |
| users.ndjson | ujson | 37.785 | 41.788 | 48.229 | 68.516 | 0.46x |
| users.ndjson | json | 47.450 | 51.724 | 63.428 | 68.516 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.131 | 3.875 | 6.365 | 61.164 | 1.00x |
| users.json | orjson | 4.088 | 4.924 | 7.135 | 61.164 | 0.79x |
| users.json | msgspec | 5.938 | 6.879 | 9.893 | 61.164 | 0.56x |
| users.json | ujson | 24.670 | 27.034 | 34.993 | 61.164 | 0.14x |
| users.json | json | 40.540 | 45.598 | 57.656 | 61.164 | 0.08x |
| flat.json | strata | 0.713 | 0.861 | 1.065 | 59.000 | 1.00x |
| flat.json | orjson | 0.826 | 1.014 | 1.336 | 59.000 | 0.85x |
| flat.json | msgspec | 1.032 | 1.222 | 1.590 | 59.000 | 0.70x |
| flat.json | ujson | 2.884 | 3.318 | 4.201 | 59.000 | 0.26x |
| flat.json | json | 4.344 | 5.262 | 6.443 | 59.000 | 0.16x |
| nested.json | strata | 0.602 | 0.737 | 0.990 | 58.922 | 1.00x |
| nested.json | orjson | 0.736 | 1.003 | 1.284 | 58.922 | 0.73x |
| nested.json | msgspec | 0.947 | 1.215 | 1.588 | 58.922 | 0.61x |
| nested.json | ujson | 2.790 | 3.395 | 4.345 | 58.922 | 0.22x |
| nested.json | json | 5.224 | 6.425 | 7.918 | 58.922 | 0.11x |
| wide_arrays.json | strata | 2.909 | 3.638 | 4.138 | 67.148 | 1.00x |
| wide_arrays.json | orjson | 3.490 | 4.192 | 5.076 | 67.148 | 0.87x |
| wide_arrays.json | msgspec | 4.723 | 5.357 | 6.365 | 67.148 | 0.68x |
| wide_arrays.json | ujson | 12.261 | 13.770 | 16.588 | 67.148 | 0.26x |
| wide_arrays.json | json | 37.136 | 40.436 | 50.004 | 67.148 | 0.09x |
| mixed.json | strata | 0.394 | 0.454 | 0.594 | 63.707 | 1.00x |
| mixed.json | orjson | 0.444 | 0.523 | 0.668 | 63.707 | 0.87x |
| mixed.json | msgspec | 0.486 | 0.583 | 0.818 | 63.707 | 0.78x |
| mixed.json | ujson | 0.857 | 0.978 | 1.379 | 63.707 | 0.46x |
| mixed.json | json | 1.378 | 1.529 | 1.988 | 63.707 | 0.30x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.122 | 0.197 | 0.290 | 61.246 | 1.00x |
| users.json $[*].id | jmespath | 0.927 | 1.048 | 1.549 | 61.246 | 0.19x |
| users.json $[*].id | jsonpath-ng | 5.087 | 5.662 | 7.118 | 61.246 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.918 | 1.233 | 1.819 | 61.461 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.957 | 6.764 | 8.641 | 61.461 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 35.392 | 39.335 | 49.603 | 61.461 | 0.03x |
| users.json $..total | strata | 3.380 | 3.956 | 5.113 | 61.500 | 1.00x |
| users.json $..total | jsonpath-ng | 706.638 | 793.761 | 879.619 | 61.500 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.695 | 3.994 | 6.023 | 61.297 | 1.00x |
| users.json $[*].id | orjson+jmespath | 26.097 | 31.049 | 46.712 | 61.297 | 0.13x |
| users.json $[*].id | orjson+jsonpath-ng | 29.448 | 35.328 | 51.045 | 61.297 | 0.11x |
| users.json $[*].orders[*].total | strata | 4.317 | 4.699 | 6.133 | 61.461 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 33.434 | 39.440 | 46.897 | 61.461 | 0.12x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 72.256 | 86.123 | 103.068 | 61.461 | 0.05x |
| users.json $..total | strata | 21.701 | 25.788 | 31.548 | 61.520 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 747.887 | 877.016 | 1020.898 | 61.520 | 0.03x |

