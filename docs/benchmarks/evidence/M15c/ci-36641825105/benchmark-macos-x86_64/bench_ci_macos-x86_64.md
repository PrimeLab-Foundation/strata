# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 271a2a0864aa430c4690ca9b609e4201e13cfb51
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
| users.json | strata | 20.997 | 24.975 | 28.554 | 57.230 | 1.00x |
| users.json | orjson | 34.995 | 37.899 | 40.570 | 57.230 | 0.66x |
| users.json | msgspec | 32.725 | 36.355 | 41.646 | 57.230 | 0.69x |
| users.json | ujson | 48.025 | 55.500 | 57.105 | 57.230 | 0.45x |
| users.json | pysimdjson | 196.924 | 214.891 | 221.436 | 57.230 | 0.12x |
| users.json | json | 57.138 | 61.753 | 64.536 | 57.230 | 0.40x |
| flat.json | strata | 1.601 | 1.914 | 2.937 | 67.879 | 1.00x |
| flat.json | orjson | 1.725 | 2.018 | 2.831 | 67.879 | 0.95x |
| flat.json | msgspec | 1.951 | 2.354 | 2.977 | 67.879 | 0.81x |
| flat.json | ujson | 3.505 | 4.025 | 5.036 | 67.879 | 0.48x |
| flat.json | pysimdjson | 17.713 | 19.539 | 22.549 | 67.879 | 0.10x |
| flat.json | json | 3.956 | 4.233 | 6.124 | 67.879 | 0.45x |
| nested.json | strata | 1.875 | 2.152 | 2.664 | 66.348 | 1.00x |
| nested.json | orjson | 2.082 | 2.591 | 3.463 | 66.348 | 0.83x |
| nested.json | msgspec | 2.372 | 3.066 | 3.854 | 66.348 | 0.70x |
| nested.json | ujson | 3.801 | 4.471 | 5.301 | 66.348 | 0.48x |
| nested.json | pysimdjson | 16.203 | 18.456 | 19.574 | 66.348 | 0.12x |
| nested.json | json | 4.816 | 5.450 | 6.943 | 66.348 | 0.39x |
| wide_arrays.json | strata | 9.249 | 11.188 | 15.561 | 72.312 | 1.00x |
| wide_arrays.json | orjson | 12.130 | 14.055 | 20.346 | 72.312 | 0.80x |
| wide_arrays.json | msgspec | 12.629 | 15.982 | 20.182 | 72.312 | 0.70x |
| wide_arrays.json | ujson | 17.347 | 20.550 | 30.576 | 72.312 | 0.54x |
| wide_arrays.json | pysimdjson | 102.077 | 107.829 | 132.847 | 72.312 | 0.10x |
| wide_arrays.json | json | 20.586 | 24.652 | 31.420 | 72.312 | 0.45x |
| mixed.json | strata | 0.477 | 0.572 | 0.715 | 65.109 | 1.00x |
| mixed.json | orjson | 0.562 | 0.646 | 0.795 | 65.109 | 0.89x |
| mixed.json | msgspec | 0.610 | 0.650 | 0.949 | 65.109 | 0.88x |
| mixed.json | ujson | 0.825 | 0.877 | 4.197 | 65.109 | 0.65x |
| mixed.json | pysimdjson | 4.100 | 4.659 | 5.035 | 65.109 | 0.12x |
| mixed.json | json | 1.191 | 1.382 | 1.557 | 65.109 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.279 | 4.005 | 6.254 | 52.527 | 1.00x |
| users.json | orjson | 4.174 | 5.147 | 9.081 | 52.527 | 0.78x |
| users.json | msgspec | 6.320 | 7.660 | 9.630 | 52.527 | 0.52x |
| users.json | ujson | 31.645 | 35.120 | 138.950 | 52.527 | 0.11x |
| users.json | json | 54.365 | 60.362 | 79.359 | 52.527 | 0.07x |
| flat.json | strata | 0.402 | 0.427 | 0.594 | 66.387 | 1.00x |
| flat.json | orjson | 0.484 | 0.507 | 0.830 | 66.387 | 0.84x |
| flat.json | msgspec | 0.662 | 0.740 | 1.002 | 66.387 | 0.58x |
| flat.json | ujson | 2.645 | 2.735 | 3.574 | 66.387 | 0.16x |
| flat.json | json | 4.324 | 4.491 | 5.907 | 66.387 | 0.10x |
| nested.json | strata | 0.372 | 0.430 | 0.575 | 66.477 | 1.00x |
| nested.json | orjson | 0.501 | 0.571 | 0.925 | 66.477 | 0.75x |
| nested.json | msgspec | 0.742 | 0.879 | 1.119 | 66.477 | 0.49x |
| nested.json | ujson | 2.997 | 3.538 | 4.107 | 66.477 | 0.12x |
| nested.json | json | 5.852 | 6.942 | 7.315 | 66.477 | 0.06x |
| wide_arrays.json | strata | 2.443 | 2.722 | 3.023 | 66.039 | 1.00x |
| wide_arrays.json | orjson | 3.095 | 3.379 | 4.360 | 66.039 | 0.81x |
| wide_arrays.json | msgspec | 4.267 | 4.503 | 5.759 | 66.039 | 0.60x |
| wide_arrays.json | ujson | 13.100 | 14.145 | 15.976 | 66.039 | 0.19x |
| wide_arrays.json | json | 41.097 | 42.654 | 49.583 | 66.039 | 0.06x |
| mixed.json | strata | 0.112 | 0.122 | 0.170 | 61.910 | 1.00x |
| mixed.json | orjson | 0.131 | 0.159 | 0.212 | 61.910 | 0.77x |
| mixed.json | msgspec | 0.165 | 0.179 | 0.242 | 61.910 | 0.68x |
| mixed.json | ujson | 0.575 | 0.644 | 0.845 | 61.910 | 0.19x |
| mixed.json | json | 1.226 | 1.417 | 1.759 | 61.910 | 0.09x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 21.867 | 26.075 | 29.551 | 63.223 | 1.00x |
| users.json | orjson | 34.019 | 40.020 | 41.881 | 63.223 | 0.65x |
| users.json | msgspec | 33.135 | 38.594 | 43.560 | 63.223 | 0.68x |
| users.json | ujson | 50.870 | 58.173 | 62.066 | 63.223 | 0.45x |
| users.json | json | 57.872 | 65.986 | 71.392 | 63.223 | 0.40x |
| flat.json | strata | 1.668 | 1.704 | 1.926 | 66.387 | 1.00x |
| flat.json | orjson | 1.910 | 1.952 | 2.202 | 66.387 | 0.87x |
| flat.json | msgspec | 2.057 | 2.163 | 3.070 | 66.387 | 0.79x |
| flat.json | ujson | 3.607 | 3.792 | 4.914 | 66.387 | 0.45x |
| flat.json | json | 3.953 | 3.990 | 4.114 | 66.387 | 0.43x |
| nested.json | strata | 1.936 | 2.095 | 2.859 | 66.477 | 1.00x |
| nested.json | orjson | 2.207 | 2.969 | 3.529 | 66.477 | 0.71x |
| nested.json | msgspec | 2.480 | 2.854 | 4.057 | 66.477 | 0.73x |
| nested.json | ujson | 3.874 | 4.144 | 5.958 | 66.477 | 0.51x |
| nested.json | json | 5.010 | 6.009 | 6.785 | 66.477 | 0.35x |
| wide_arrays.json | strata | 9.397 | 10.969 | 12.673 | 67.270 | 1.00x |
| wide_arrays.json | orjson | 11.253 | 14.237 | 16.462 | 67.270 | 0.77x |
| wide_arrays.json | msgspec | 12.267 | 14.798 | 18.513 | 67.270 | 0.74x |
| wide_arrays.json | ujson | 16.342 | 19.820 | 24.008 | 67.270 | 0.55x |
| wide_arrays.json | json | 21.769 | 25.210 | 30.537 | 67.270 | 0.44x |
| mixed.json | strata | 0.556 | 0.604 | 0.820 | 61.910 | 1.00x |
| mixed.json | orjson | 0.730 | 0.788 | 1.100 | 61.910 | 0.77x |
| mixed.json | msgspec | 0.777 | 0.808 | 1.085 | 61.910 | 0.75x |
| mixed.json | ujson | 1.017 | 1.066 | 1.464 | 61.910 | 0.57x |
| mixed.json | json | 1.312 | 1.434 | 1.810 | 61.910 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 23.634 | 25.560 | 29.436 | 67.457 | 1.00x |
| users.ndjson | orjson | 34.108 | 39.778 | 49.044 | 67.457 | 0.64x |
| users.ndjson | msgspec | 34.385 | 40.557 | 47.954 | 67.457 | 0.63x |
| users.ndjson | ujson | 50.719 | 58.688 | 68.608 | 67.457 | 0.44x |
| users.ndjson | json | 62.435 | 70.577 | 84.694 | 67.457 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.072 | 4.429 | 6.463 | 63.234 | 1.00x |
| users.json | orjson | 5.775 | 6.473 | 7.995 | 63.234 | 0.68x |
| users.json | msgspec | 7.397 | 8.689 | 10.208 | 63.234 | 0.51x |
| users.json | ujson | 33.086 | 34.522 | 41.663 | 63.234 | 0.13x |
| users.json | json | 50.671 | 57.326 | 61.020 | 63.234 | 0.08x |
| flat.json | strata | 0.835 | 0.987 | 1.362 | 66.387 | 1.00x |
| flat.json | orjson | 0.985 | 1.189 | 1.553 | 66.387 | 0.83x |
| flat.json | msgspec | 1.160 | 1.392 | 1.720 | 66.387 | 0.71x |
| flat.json | ujson | 3.267 | 3.880 | 4.517 | 66.387 | 0.25x |
| flat.json | json | 5.027 | 5.646 | 6.544 | 66.387 | 0.17x |
| nested.json | strata | 0.732 | 1.028 | 1.209 | 66.477 | 1.00x |
| nested.json | orjson | 1.003 | 1.097 | 1.491 | 66.477 | 0.94x |
| nested.json | msgspec | 1.271 | 1.336 | 1.833 | 66.477 | 0.77x |
| nested.json | ujson | 3.410 | 4.233 | 7.260 | 66.477 | 0.24x |
| nested.json | json | 6.089 | 7.084 | 9.365 | 66.477 | 0.15x |
| wide_arrays.json | strata | 3.975 | 4.270 | 21.703 | 66.039 | 1.00x |
| wide_arrays.json | orjson | 4.349 | 5.183 | 6.515 | 66.039 | 0.82x |
| wide_arrays.json | msgspec | 5.500 | 6.757 | 7.850 | 66.039 | 0.63x |
| wide_arrays.json | ujson | 15.563 | 18.760 | 21.787 | 66.039 | 0.23x |
| wide_arrays.json | json | 46.733 | 49.546 | 60.299 | 66.039 | 0.09x |
| mixed.json | strata | 0.465 | 0.553 | 0.625 | 61.910 | 1.00x |
| mixed.json | orjson | 0.507 | 0.660 | 0.810 | 61.910 | 0.84x |
| mixed.json | msgspec | 0.621 | 0.687 | 0.857 | 61.910 | 0.80x |
| mixed.json | ujson | 0.998 | 1.085 | 1.538 | 61.910 | 0.51x |
| mixed.json | json | 1.673 | 1.837 | 2.266 | 61.910 | 0.30x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.221 | 0.305 | 0.362 | 63.301 | 1.00x |
| users.json $[*].id | jmespath | 1.189 | 1.357 | 1.658 | 63.301 | 0.22x |
| users.json $[*].id | jsonpath-ng | 6.567 | 7.684 | 8.548 | 63.301 | 0.04x |
| users.json $[*].orders[*].total | strata | 1.402 | 1.770 | 2.141 | 60.516 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 7.537 | 8.604 | 10.029 | 60.516 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 46.848 | 54.181 | 56.145 | 60.516 | 0.03x |
| users.json $..total | strata | 5.084 | 5.761 | 6.715 | 60.598 | 1.00x |
| users.json $..total | jsonpath-ng | 939.260 | 960.246 | 1155.481 | 60.598 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.607 | 5.415 | 6.750 | 63.359 | 1.00x |
| users.json $[*].id | orjson+jmespath | 35.765 | 41.142 | 48.409 | 63.359 | 0.13x |
| users.json $[*].id | orjson+jsonpath-ng | 42.216 | 47.228 | 56.785 | 63.359 | 0.11x |
| users.json $[*].orders[*].total | strata | 5.049 | 6.184 | 7.503 | 60.570 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 41.196 | 49.269 | 57.871 | 60.570 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 100.159 | 112.788 | 156.304 | 60.570 | 0.05x |
| users.json $..total | strata | 26.620 | 31.313 | 35.586 | 60.621 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 937.416 | 996.820 | 1146.648 | 60.621 | 0.03x |

