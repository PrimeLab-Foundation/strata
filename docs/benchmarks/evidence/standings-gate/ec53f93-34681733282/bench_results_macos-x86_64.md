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
| users.json | strata | 16.452 | 16.809 | 22.994 | 57.090 | 1.00x |
| users.json | orjson | 23.722 | 25.031 | 31.748 | 57.090 | 0.67x |
| users.json | msgspec | 23.493 | 24.694 | 29.286 | 57.090 | 0.68x |
| users.json | ujson | 34.611 | 35.890 | 40.601 | 57.090 | 0.47x |
| users.json | pysimdjson | 152.520 | 155.440 | 165.926 | 57.090 | 0.11x |
| users.json | json | 39.676 | 41.846 | 46.117 | 57.090 | 0.40x |
| flat.json | strata | 1.209 | 1.258 | 1.998 | 66.176 | 1.00x |
| flat.json | orjson | 1.355 | 1.438 | 2.548 | 66.176 | 0.87x |
| flat.json | msgspec | 1.536 | 1.670 | 1.921 | 66.176 | 0.75x |
| flat.json | ujson | 2.684 | 2.818 | 3.141 | 66.176 | 0.45x |
| flat.json | pysimdjson | 13.932 | 14.948 | 21.841 | 66.176 | 0.08x |
| flat.json | json | 3.053 | 3.233 | 3.707 | 66.176 | 0.39x |
| nested.json | strata | 1.339 | 1.421 | 1.470 | 65.293 | 1.00x |
| nested.json | orjson | 1.558 | 1.645 | 1.813 | 65.293 | 0.86x |
| nested.json | msgspec | 1.716 | 1.872 | 2.069 | 65.293 | 0.76x |
| nested.json | ujson | 2.828 | 2.975 | 3.378 | 65.293 | 0.48x |
| nested.json | pysimdjson | 12.616 | 12.872 | 15.542 | 65.293 | 0.11x |
| nested.json | json | 3.671 | 3.801 | 8.137 | 65.293 | 0.37x |
| wide_arrays.json | strata | 6.881 | 7.689 | 8.688 | 71.254 | 1.00x |
| wide_arrays.json | orjson | 9.049 | 9.722 | 20.494 | 71.254 | 0.79x |
| wide_arrays.json | msgspec | 9.725 | 10.488 | 14.703 | 71.254 | 0.73x |
| wide_arrays.json | ujson | 12.481 | 13.596 | 24.116 | 71.254 | 0.57x |
| wide_arrays.json | pysimdjson | 75.427 | 77.118 | 99.629 | 71.254 | 0.10x |
| wide_arrays.json | json | 16.031 | 17.087 | 32.690 | 71.254 | 0.45x |
| mixed.json | strata | 0.345 | 0.464 | 0.681 | 64.051 | 1.00x |
| mixed.json | orjson | 0.420 | 0.743 | 1.551 | 64.051 | 0.62x |
| mixed.json | msgspec | 0.442 | 0.691 | 0.791 | 64.051 | 0.67x |
| mixed.json | ujson | 0.625 | 0.716 | 1.160 | 64.051 | 0.65x |
| mixed.json | pysimdjson | 3.152 | 4.207 | 4.695 | 64.051 | 0.11x |
| mixed.json | json | 0.878 | 1.205 | 1.979 | 64.051 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.328 | 2.632 | 3.341 | 52.410 | 1.00x |
| users.json | orjson | 3.036 | 3.433 | 3.777 | 52.410 | 0.77x |
| users.json | msgspec | 4.789 | 5.286 | 5.926 | 52.410 | 0.50x |
| users.json | ujson | 23.326 | 25.299 | 26.455 | 52.410 | 0.10x |
| users.json | json | 39.071 | 41.531 | 45.139 | 52.410 | 0.06x |
| flat.json | strata | 0.324 | 0.335 | 0.372 | 64.688 | 1.00x |
| flat.json | orjson | 0.400 | 0.420 | 0.462 | 64.688 | 0.80x |
| flat.json | msgspec | 0.537 | 0.558 | 0.620 | 64.688 | 0.60x |
| flat.json | ujson | 2.138 | 2.163 | 2.381 | 64.688 | 0.15x |
| flat.json | json | 3.483 | 3.571 | 3.875 | 64.688 | 0.09x |
| nested.json | strata | 0.279 | 0.292 | 0.941 | 65.422 | 1.00x |
| nested.json | orjson | 0.384 | 0.420 | 0.647 | 65.422 | 0.70x |
| nested.json | msgspec | 0.613 | 0.662 | 1.856 | 65.422 | 0.44x |
| nested.json | ujson | 2.300 | 2.548 | 3.784 | 65.422 | 0.11x |
| nested.json | json | 4.705 | 4.878 | 6.111 | 65.422 | 0.06x |
| wide_arrays.json | strata | 1.914 | 2.380 | 4.416 | 64.980 | 1.00x |
| wide_arrays.json | orjson | 2.551 | 3.552 | 5.782 | 64.980 | 0.67x |
| wide_arrays.json | msgspec | 3.403 | 3.807 | 8.486 | 64.980 | 0.63x |
| wide_arrays.json | ujson | 9.885 | 11.632 | 16.647 | 64.980 | 0.20x |
| wide_arrays.json | json | 32.428 | 36.648 | 63.347 | 64.980 | 0.06x |
| mixed.json | strata | 0.074 | 0.082 | 0.146 | 60.852 | 1.00x |
| mixed.json | orjson | 0.089 | 0.100 | 0.178 | 60.852 | 0.82x |
| mixed.json | msgspec | 0.117 | 0.131 | 0.291 | 60.852 | 0.63x |
| mixed.json | ujson | 0.440 | 0.480 | 0.755 | 60.852 | 0.17x |
| mixed.json | json | 0.923 | 0.994 | 6.312 | 60.852 | 0.08x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 16.892 | 16.950 | 17.938 | 63.211 | 1.00x |
| users.json | orjson | 23.610 | 24.275 | 26.264 | 63.211 | 0.70x |
| users.json | msgspec | 24.070 | 24.281 | 28.057 | 63.211 | 0.70x |
| users.json | ujson | 35.670 | 36.644 | 39.410 | 63.211 | 0.46x |
| users.json | json | 40.267 | 40.493 | 40.925 | 63.211 | 0.42x |
| flat.json | strata | 1.299 | 1.351 | 2.380 | 65.332 | 1.00x |
| flat.json | orjson | 1.488 | 1.507 | 2.677 | 65.332 | 0.90x |
| flat.json | msgspec | 1.692 | 1.725 | 3.523 | 65.332 | 0.78x |
| flat.json | ujson | 2.829 | 2.856 | 4.744 | 65.332 | 0.47x |
| flat.json | json | 3.150 | 3.241 | 5.863 | 65.332 | 0.42x |
| nested.json | strata | 1.483 | 1.659 | 2.705 | 65.422 | 1.00x |
| nested.json | orjson | 1.733 | 1.941 | 17.219 | 65.422 | 0.85x |
| nested.json | msgspec | 1.936 | 2.036 | 2.400 | 65.422 | 0.81x |
| nested.json | ujson | 3.036 | 3.293 | 5.003 | 65.422 | 0.50x |
| nested.json | json | 3.793 | 4.117 | 6.447 | 65.422 | 0.40x |
| wide_arrays.json | strata | 7.143 | 8.728 | 14.447 | 66.211 | 1.00x |
| wide_arrays.json | orjson | 9.203 | 10.423 | 14.585 | 66.211 | 0.84x |
| wide_arrays.json | msgspec | 10.136 | 11.379 | 16.022 | 66.211 | 0.77x |
| wide_arrays.json | ujson | 12.948 | 15.230 | 20.658 | 66.211 | 0.57x |
| wide_arrays.json | json | 16.413 | 18.883 | 34.327 | 66.211 | 0.46x |
| mixed.json | strata | 0.393 | 0.440 | 0.499 | 60.852 | 1.00x |
| mixed.json | orjson | 0.512 | 0.556 | 0.596 | 60.852 | 0.79x |
| mixed.json | msgspec | 0.527 | 0.591 | 0.629 | 60.852 | 0.74x |
| mixed.json | ujson | 0.732 | 0.776 | 0.801 | 60.852 | 0.57x |
| mixed.json | json | 0.944 | 0.992 | 1.008 | 60.852 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 17.066 | 17.855 | 29.118 | 65.758 | 1.00x |
| users.ndjson | orjson | 25.120 | 26.556 | 32.831 | 65.758 | 0.67x |
| users.ndjson | msgspec | 25.457 | 26.929 | 39.929 | 65.758 | 0.66x |
| users.ndjson | ujson | 36.677 | 40.093 | 49.958 | 65.758 | 0.45x |
| users.ndjson | json | 45.750 | 49.005 | 64.293 | 65.758 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.084 | 3.249 | 3.560 | 63.254 | 1.00x |
| users.json | orjson | 4.241 | 4.338 | 4.618 | 63.254 | 0.75x |
| users.json | msgspec | 5.793 | 5.855 | 6.193 | 63.254 | 0.55x |
| users.json | ujson | 24.446 | 24.564 | 25.177 | 63.254 | 0.13x |
| users.json | json | 40.098 | 40.363 | 41.097 | 63.254 | 0.08x |
| flat.json | strata | 0.671 | 0.725 | 1.218 | 65.332 | 1.00x |
| flat.json | orjson | 0.762 | 0.840 | 0.903 | 65.332 | 0.86x |
| flat.json | msgspec | 0.896 | 0.978 | 1.192 | 65.332 | 0.74x |
| flat.json | ujson | 2.489 | 2.664 | 3.188 | 65.332 | 0.27x |
| flat.json | json | 3.910 | 4.208 | 6.423 | 65.332 | 0.17x |
| nested.json | strata | 0.527 | 0.607 | 0.737 | 65.422 | 1.00x |
| nested.json | orjson | 0.642 | 0.809 | 1.101 | 65.422 | 0.75x |
| nested.json | msgspec | 0.833 | 0.997 | 1.535 | 65.422 | 0.61x |
| nested.json | ujson | 2.564 | 2.698 | 4.664 | 65.422 | 0.22x |
| nested.json | json | 4.692 | 5.105 | 8.824 | 65.422 | 0.12x |
| wide_arrays.json | strata | 2.691 | 3.155 | 4.153 | 64.980 | 1.00x |
| wide_arrays.json | orjson | 3.236 | 3.711 | 5.055 | 64.980 | 0.85x |
| wide_arrays.json | msgspec | 4.080 | 4.888 | 9.289 | 64.980 | 0.65x |
| wide_arrays.json | ujson | 12.444 | 13.912 | 24.996 | 64.980 | 0.23x |
| wide_arrays.json | json | 33.842 | 42.436 | 59.195 | 64.980 | 0.07x |
| mixed.json | strata | 0.356 | 0.402 | 0.715 | 60.852 | 1.00x |
| mixed.json | orjson | 0.404 | 0.445 | 0.574 | 60.852 | 0.90x |
| mixed.json | msgspec | 0.454 | 0.527 | 0.874 | 60.852 | 0.76x |
| mixed.json | ujson | 0.824 | 0.839 | 1.662 | 60.852 | 0.48x |
| mixed.json | json | 1.328 | 1.385 | 1.606 | 60.852 | 0.29x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.130 | 0.140 | 0.174 | 63.332 | 1.00x |
| users.json $[*].id | jmespath | 0.895 | 0.924 | 0.959 | 63.332 | 0.15x |
| users.json $[*].id | jsonpath-ng | 4.750 | 4.906 | 5.025 | 63.332 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.829 | 0.871 | 1.135 | 60.516 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.583 | 5.737 | 6.070 | 60.516 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 32.793 | 33.231 | 35.329 | 60.516 | 0.03x |
| users.json $..total | strata | 3.115 | 3.489 | 3.728 | 60.633 | 1.00x |
| users.json $..total | jsonpath-ng | 654.928 | 674.780 | 744.113 | 60.633 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.622 | 3.685 | 3.890 | 63.398 | 1.00x |
| users.json $[*].id | orjson+jmespath | 25.067 | 27.132 | 29.511 | 63.398 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 29.249 | 31.412 | 33.553 | 63.398 | 0.12x |
| users.json $[*].orders[*].total | strata | 3.897 | 4.057 | 4.339 | 60.547 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 29.585 | 31.788 | 34.025 | 60.547 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 61.576 | 64.065 | 73.123 | 60.547 | 0.06x |
| users.json $..total | strata | 20.663 | 21.438 | 23.663 | 60.633 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 678.669 | 703.468 | 736.770 | 60.633 | 0.03x |

