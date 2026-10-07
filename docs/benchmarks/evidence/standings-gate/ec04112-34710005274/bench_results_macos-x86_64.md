# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: ec0411225b6d58a1df905844c946a766d3c39f0a
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
| users.json | strata | 25.578 | 28.856 | 41.079 | 57.223 | 1.00x |
| users.json | orjson | 40.552 | 45.361 | 55.505 | 57.223 | 0.64x |
| users.json | msgspec | 40.627 | 45.409 | 57.477 | 57.223 | 0.64x |
| users.json | ujson | 58.076 | 64.289 | 85.742 | 57.223 | 0.45x |
| users.json | pysimdjson | 240.049 | 251.754 | 262.844 | 57.223 | 0.11x |
| users.json | json | 64.554 | 73.841 | 76.632 | 57.223 | 0.39x |
| flat.json | strata | 1.694 | 1.771 | 2.552 | 66.391 | 1.00x |
| flat.json | orjson | 1.928 | 2.060 | 2.436 | 66.391 | 0.86x |
| flat.json | msgspec | 2.223 | 2.339 | 3.197 | 66.391 | 0.76x |
| flat.json | ujson | 3.807 | 4.104 | 5.230 | 66.391 | 0.43x |
| flat.json | pysimdjson | 20.251 | 21.269 | 21.581 | 66.391 | 0.08x |
| flat.json | json | 4.390 | 4.629 | 6.038 | 66.391 | 0.38x |
| nested.json | strata | 2.322 | 2.485 | 3.042 | 56.863 | 1.00x |
| nested.json | orjson | 2.427 | 3.017 | 3.871 | 56.863 | 0.82x |
| nested.json | msgspec | 2.762 | 3.353 | 4.537 | 56.863 | 0.74x |
| nested.json | ujson | 4.785 | 5.870 | 6.400 | 56.863 | 0.42x |
| nested.json | pysimdjson | 21.445 | 23.092 | 24.380 | 56.863 | 0.11x |
| nested.json | json | 6.298 | 7.411 | 8.108 | 56.863 | 0.34x |
| wide_arrays.json | strata | 10.094 | 11.708 | 13.673 | 66.656 | 1.00x |
| wide_arrays.json | orjson | 14.327 | 15.755 | 18.785 | 66.656 | 0.74x |
| wide_arrays.json | msgspec | 14.614 | 15.442 | 18.301 | 66.656 | 0.76x |
| wide_arrays.json | ujson | 19.482 | 21.225 | 22.558 | 66.656 | 0.55x |
| wide_arrays.json | pysimdjson | 112.235 | 121.729 | 131.427 | 66.656 | 0.10x |
| wide_arrays.json | json | 24.036 | 26.426 | 30.959 | 66.656 | 0.44x |
| mixed.json | strata | 0.515 | 0.708 | 0.799 | 66.316 | 1.00x |
| mixed.json | orjson | 0.680 | 0.850 | 1.032 | 66.316 | 0.83x |
| mixed.json | msgspec | 0.670 | 0.985 | 1.028 | 66.316 | 0.72x |
| mixed.json | ujson | 1.013 | 1.289 | 1.694 | 66.316 | 0.55x |
| mixed.json | pysimdjson | 5.189 | 5.906 | 9.320 | 66.316 | 0.12x |
| mixed.json | json | 1.647 | 1.809 | 1.973 | 66.316 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.809 | 4.382 | 5.235 | 53.492 | 1.00x |
| users.json | orjson | 4.970 | 5.351 | 6.579 | 53.492 | 0.82x |
| users.json | msgspec | 7.536 | 7.978 | 9.071 | 53.492 | 0.55x |
| users.json | ujson | 34.576 | 38.082 | 41.185 | 53.492 | 0.12x |
| users.json | json | 59.172 | 63.847 | 72.342 | 53.492 | 0.07x |
| flat.json | strata | 0.479 | 0.514 | 0.598 | 56.469 | 1.00x |
| flat.json | orjson | 0.640 | 0.719 | 0.861 | 56.469 | 0.71x |
| flat.json | msgspec | 0.860 | 1.010 | 1.163 | 56.469 | 0.51x |
| flat.json | ujson | 3.325 | 3.734 | 4.049 | 56.469 | 0.14x |
| flat.json | json | 5.557 | 5.785 | 7.379 | 56.469 | 0.09x |
| nested.json | strata | 0.375 | 0.410 | 0.667 | 55.676 | 1.00x |
| nested.json | orjson | 0.533 | 0.649 | 0.854 | 55.676 | 0.63x |
| nested.json | msgspec | 0.798 | 0.857 | 1.403 | 55.676 | 0.48x |
| nested.json | ujson | 3.288 | 3.330 | 4.661 | 55.676 | 0.12x |
| nested.json | json | 6.575 | 6.710 | 8.304 | 55.676 | 0.06x |
| wide_arrays.json | strata | 2.977 | 3.456 | 3.873 | 66.766 | 1.00x |
| wide_arrays.json | orjson | 3.630 | 3.910 | 4.439 | 66.766 | 0.88x |
| wide_arrays.json | msgspec | 4.942 | 5.690 | 6.602 | 66.766 | 0.61x |
| wide_arrays.json | ujson | 14.872 | 17.649 | 19.102 | 66.766 | 0.20x |
| wide_arrays.json | json | 48.788 | 55.881 | 64.170 | 66.766 | 0.06x |
| mixed.json | strata | 0.127 | 0.150 | 0.186 | 62.062 | 1.00x |
| mixed.json | orjson | 0.132 | 0.162 | 0.211 | 62.062 | 0.93x |
| mixed.json | msgspec | 0.189 | 0.260 | 0.276 | 62.062 | 0.58x |
| mixed.json | ujson | 0.657 | 0.784 | 1.016 | 62.062 | 0.19x |
| mixed.json | json | 1.513 | 1.786 | 2.164 | 62.062 | 0.08x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 26.011 | 28.965 | 35.219 | 63.504 | 1.00x |
| users.json | orjson | 42.169 | 45.813 | 52.313 | 63.504 | 0.63x |
| users.json | msgspec | 40.928 | 46.424 | 54.069 | 63.504 | 0.62x |
| users.json | ujson | 59.973 | 68.897 | 81.973 | 63.504 | 0.42x |
| users.json | json | 67.356 | 74.030 | 96.730 | 63.504 | 0.39x |
| flat.json | strata | 1.873 | 2.042 | 3.511 | 57.004 | 1.00x |
| flat.json | orjson | 2.160 | 2.700 | 3.103 | 57.004 | 0.76x |
| flat.json | msgspec | 2.471 | 3.023 | 4.308 | 57.004 | 0.68x |
| flat.json | ujson | 4.177 | 4.711 | 5.075 | 57.004 | 0.43x |
| flat.json | json | 4.615 | 5.185 | 6.211 | 57.004 | 0.39x |
| nested.json | strata | 2.216 | 2.403 | 2.755 | 56.180 | 1.00x |
| nested.json | orjson | 2.725 | 3.020 | 3.789 | 56.180 | 0.80x |
| nested.json | msgspec | 3.038 | 3.410 | 4.190 | 56.180 | 0.70x |
| nested.json | ujson | 4.773 | 5.559 | 6.346 | 56.180 | 0.43x |
| nested.json | json | 5.883 | 6.522 | 7.327 | 56.180 | 0.37x |
| wide_arrays.json | strata | 10.182 | 11.379 | 12.351 | 66.766 | 1.00x |
| wide_arrays.json | orjson | 13.546 | 14.857 | 16.817 | 66.766 | 0.77x |
| wide_arrays.json | msgspec | 14.722 | 16.569 | 17.146 | 66.766 | 0.69x |
| wide_arrays.json | ujson | 19.094 | 20.945 | 25.574 | 66.766 | 0.54x |
| wide_arrays.json | json | 24.150 | 28.136 | 30.841 | 66.766 | 0.40x |
| mixed.json | strata | 0.613 | 0.800 | 1.023 | 62.062 | 1.00x |
| mixed.json | orjson | 0.799 | 0.897 | 1.165 | 62.062 | 0.89x |
| mixed.json | msgspec | 0.849 | 1.100 | 1.295 | 62.062 | 0.73x |
| mixed.json | ujson | 1.158 | 1.431 | 1.625 | 62.062 | 0.56x |
| mixed.json | json | 1.420 | 1.767 | 2.506 | 62.062 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 25.604 | 26.903 | 35.591 | 64.973 | 1.00x |
| users.ndjson | orjson | 38.763 | 45.646 | 61.718 | 64.973 | 0.59x |
| users.ndjson | msgspec | 39.375 | 46.098 | 52.810 | 64.973 | 0.58x |
| users.ndjson | ujson | 57.194 | 59.113 | 69.086 | 64.973 | 0.46x |
| users.ndjson | json | 69.723 | 74.512 | 89.583 | 64.973 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 5.424 | 6.097 | 8.259 | 63.570 | 1.00x |
| users.json | orjson | 6.364 | 7.352 | 7.643 | 63.570 | 0.83x |
| users.json | msgspec | 9.310 | 9.776 | 10.868 | 63.570 | 0.62x |
| users.json | ujson | 37.185 | 39.839 | 42.687 | 63.570 | 0.15x |
| users.json | json | 65.261 | 68.438 | 73.250 | 63.570 | 0.09x |
| flat.json | strata | 0.940 | 1.045 | 1.351 | 57.004 | 1.00x |
| flat.json | orjson | 1.058 | 1.258 | 1.557 | 57.004 | 0.83x |
| flat.json | msgspec | 1.249 | 1.551 | 1.921 | 57.004 | 0.67x |
| flat.json | ujson | 3.935 | 4.220 | 5.108 | 57.004 | 0.25x |
| flat.json | json | 5.935 | 6.857 | 8.399 | 57.004 | 0.15x |
| nested.json | strata | 0.885 | 0.981 | 1.139 | 56.180 | 1.00x |
| nested.json | orjson | 1.173 | 1.433 | 2.039 | 56.180 | 0.68x |
| nested.json | msgspec | 1.495 | 1.758 | 1.960 | 56.180 | 0.56x |
| nested.json | ujson | 4.057 | 4.939 | 5.336 | 56.180 | 0.20x |
| nested.json | json | 7.441 | 8.669 | 9.251 | 56.180 | 0.11x |
| wide_arrays.json | strata | 4.008 | 4.350 | 5.031 | 66.766 | 1.00x |
| wide_arrays.json | orjson | 4.990 | 5.812 | 6.470 | 66.766 | 0.75x |
| wide_arrays.json | msgspec | 6.081 | 7.053 | 7.698 | 66.766 | 0.62x |
| wide_arrays.json | ujson | 16.051 | 18.986 | 23.149 | 66.766 | 0.23x |
| wide_arrays.json | json | 50.671 | 56.267 | 59.046 | 66.766 | 0.08x |
| mixed.json | strata | 0.525 | 0.616 | 0.762 | 62.062 | 1.00x |
| mixed.json | orjson | 0.609 | 0.724 | 0.773 | 62.062 | 0.85x |
| mixed.json | msgspec | 0.610 | 0.722 | 0.834 | 62.062 | 0.85x |
| mixed.json | ujson | 1.135 | 1.243 | 1.764 | 62.062 | 0.50x |
| mixed.json | json | 1.912 | 2.176 | 2.803 | 62.062 | 0.28x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.256 | 0.275 | 0.379 | 63.660 | 1.00x |
| users.json $[*].id | jmespath | 1.406 | 1.543 | 2.062 | 63.660 | 0.18x |
| users.json $[*].id | jsonpath-ng | 7.997 | 8.468 | 11.232 | 63.660 | 0.03x |
| users.json $[*].orders[*].total | strata | 1.826 | 2.288 | 3.028 | 61.141 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 10.975 | 11.835 | 12.896 | 61.141 | 0.19x |
| users.json $[*].orders[*].total | jsonpath-ng | 58.859 | 68.126 | 77.607 | 61.141 | 0.03x |
| users.json $..total | strata | 5.150 | 5.888 | 7.465 | 61.191 | 1.00x |
| users.json $..total | jsonpath-ng | 1095.699 | 1149.642 | 1246.923 | 61.191 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 5.732 | 6.937 | 7.337 | 63.715 | 1.00x |
| users.json $[*].id | orjson+jmespath | 42.420 | 47.769 | 52.626 | 63.715 | 0.15x |
| users.json $[*].id | orjson+jsonpath-ng | 48.554 | 55.634 | 59.059 | 63.715 | 0.12x |
| users.json $[*].orders[*].total | strata | 5.914 | 6.751 | 8.216 | 61.164 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 48.592 | 54.983 | 60.961 | 61.164 | 0.12x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 112.621 | 122.923 | 127.531 | 61.164 | 0.05x |
| users.json $..total | strata | 31.551 | 36.799 | 43.164 | 61.191 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 1126.112 | 1179.366 | 1327.921 | 61.191 | 0.03x |

