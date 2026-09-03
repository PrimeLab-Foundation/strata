# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: e6c5e36
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
| users.json | strata | 17.785 | 18.364 | 21.467 | 52.578 | 1.00x |
| users.json | orjson | 23.694 | 23.956 | 26.125 | 52.578 | 0.77x |
| users.json | msgspec | 23.754 | 24.248 | 27.340 | 52.578 | 0.76x |
| users.json | ujson | 34.431 | 36.443 | 39.843 | 52.578 | 0.50x |
| users.json | pysimdjson | 152.177 | 153.118 | 158.856 | 52.578 | 0.12x |
| users.json | json | 39.064 | 40.265 | 43.903 | 52.578 | 0.46x |
| flat.json | strata | 1.181 | 1.194 | 1.277 | 58.328 | 1.00x |
| flat.json | orjson | 1.257 | 1.285 | 1.333 | 58.328 | 0.93x |
| flat.json | msgspec | 1.455 | 1.472 | 1.548 | 58.328 | 0.81x |
| flat.json | ujson | 2.519 | 2.571 | 2.911 | 58.328 | 0.46x |
| flat.json | pysimdjson | 13.697 | 13.769 | 15.442 | 58.328 | 0.09x |
| flat.json | json | 2.914 | 2.930 | 2.960 | 58.328 | 0.41x |
| nested.json | strata | 1.344 | 1.358 | 1.394 | 53.621 | 1.00x |
| nested.json | orjson | 1.525 | 1.554 | 1.607 | 53.621 | 0.87x |
| nested.json | msgspec | 1.688 | 1.721 | 1.782 | 53.621 | 0.79x |
| nested.json | ujson | 2.711 | 2.877 | 2.944 | 53.621 | 0.47x |
| nested.json | pysimdjson | 12.499 | 12.692 | 13.458 | 53.621 | 0.11x |
| nested.json | json | 3.612 | 3.638 | 4.102 | 53.621 | 0.37x |
| wide_arrays.json | strata | 8.005 | 8.159 | 8.694 | 57.906 | 1.00x |
| wide_arrays.json | orjson | 8.415 | 8.939 | 9.305 | 57.906 | 0.91x |
| wide_arrays.json | msgspec | 9.204 | 9.826 | 10.145 | 57.906 | 0.83x |
| wide_arrays.json | ujson | 12.251 | 12.677 | 13.449 | 57.906 | 0.64x |
| wide_arrays.json | pysimdjson | 74.290 | 74.937 | 75.848 | 57.906 | 0.11x |
| wide_arrays.json | json | 15.806 | 16.224 | 16.496 | 57.906 | 0.50x |
| mixed.json | strata | 0.339 | 0.358 | 0.379 | 55.133 | 1.00x |
| mixed.json | orjson | 0.402 | 0.431 | 0.453 | 55.133 | 0.83x |
| mixed.json | msgspec | 0.424 | 0.449 | 0.455 | 55.133 | 0.80x |
| mixed.json | ujson | 0.604 | 0.621 | 0.640 | 55.133 | 0.58x |
| mixed.json | pysimdjson | 3.043 | 3.087 | 3.523 | 55.133 | 0.12x |
| mixed.json | json | 0.845 | 0.855 | 0.873 | 55.133 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.419 | 2.566 | 2.793 | 51.395 | 1.00x |
| users.json | orjson | 3.162 | 3.317 | 3.410 | 51.395 | 0.77x |
| users.json | msgspec | 4.993 | 5.007 | 5.448 | 51.395 | 0.51x |
| users.json | ujson | 29.764 | 29.973 | 30.205 | 51.395 | 0.09x |
| users.json | json | 39.161 | 39.331 | 40.389 | 51.395 | 0.07x |
| flat.json | strata | 0.282 | 0.288 | 0.294 | 53.488 | 1.00x |
| flat.json | orjson | 0.360 | 0.365 | 0.379 | 53.488 | 0.79x |
| flat.json | msgspec | 0.464 | 0.498 | 0.569 | 53.488 | 0.58x |
| flat.json | ujson | 2.447 | 2.472 | 2.573 | 53.488 | 0.12x |
| flat.json | json | 3.406 | 3.432 | 3.809 | 53.488 | 0.08x |
| nested.json | strata | 0.203 | 0.224 | 0.255 | 53.766 | 1.00x |
| nested.json | orjson | 0.301 | 0.322 | 0.329 | 53.766 | 0.70x |
| nested.json | msgspec | 0.517 | 0.524 | 0.560 | 53.766 | 0.43x |
| nested.json | ujson | 2.608 | 2.626 | 2.677 | 53.766 | 0.09x |
| nested.json | json | 4.318 | 4.361 | 4.580 | 53.766 | 0.05x |
| wide_arrays.json | strata | 1.662 | 1.697 | 1.789 | 56.004 | 1.00x |
| wide_arrays.json | orjson | 2.402 | 2.535 | 2.649 | 56.004 | 0.67x |
| wide_arrays.json | msgspec | 2.966 | 3.089 | 3.160 | 56.004 | 0.55x |
| wide_arrays.json | ujson | 14.281 | 14.687 | 14.882 | 56.004 | 0.12x |
| wide_arrays.json | json | 31.829 | 32.089 | 34.673 | 56.004 | 0.05x |
| mixed.json | strata | 0.076 | 0.081 | 0.091 | 52.996 | 1.00x |
| mixed.json | orjson | 0.069 | 0.077 | 0.090 | 52.996 | 1.05x |
| mixed.json | msgspec | 0.100 | 0.108 | 0.128 | 52.996 | 0.75x |
| mixed.json | ujson | 0.584 | 0.589 | 0.626 | 52.996 | 0.14x |
| mixed.json | json | 0.897 | 0.915 | 1.228 | 52.996 | 0.09x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 18.392 | 18.541 | 19.287 | 61.258 | 1.00x |
| users.json | orjson | 23.680 | 25.542 | 26.626 | 61.258 | 0.73x |
| users.json | msgspec | 24.055 | 25.269 | 27.190 | 61.258 | 0.73x |
| users.json | ujson | 36.052 | 36.920 | 38.194 | 61.258 | 0.50x |
| users.json | json | 40.017 | 40.657 | 43.842 | 61.258 | 0.46x |
| flat.json | strata | 1.274 | 1.291 | 1.332 | 53.488 | 1.00x |
| flat.json | orjson | 1.407 | 1.423 | 1.478 | 53.488 | 0.91x |
| flat.json | msgspec | 1.596 | 1.621 | 1.699 | 53.488 | 0.80x |
| flat.json | ujson | 2.695 | 2.747 | 2.839 | 53.488 | 0.47x |
| flat.json | json | 2.973 | 3.064 | 3.328 | 53.488 | 0.42x |
| nested.json | strata | 1.425 | 1.480 | 1.500 | 53.766 | 1.00x |
| nested.json | orjson | 1.634 | 1.688 | 1.772 | 53.766 | 0.88x |
| nested.json | msgspec | 1.811 | 1.871 | 1.918 | 53.766 | 0.79x |
| nested.json | ujson | 3.014 | 3.059 | 3.211 | 53.766 | 0.48x |
| nested.json | json | 3.718 | 3.781 | 3.897 | 53.766 | 0.39x |
| wide_arrays.json | strata | 8.165 | 8.233 | 8.970 | 57.230 | 1.00x |
| wide_arrays.json | orjson | 8.687 | 8.754 | 8.989 | 57.230 | 0.94x |
| wide_arrays.json | msgspec | 9.763 | 9.800 | 9.904 | 57.230 | 0.84x |
| wide_arrays.json | ujson | 12.562 | 12.667 | 14.487 | 57.230 | 0.65x |
| wide_arrays.json | json | 15.971 | 16.157 | 16.516 | 57.230 | 0.51x |
| mixed.json | strata | 0.403 | 0.430 | 0.449 | 52.996 | 1.00x |
| mixed.json | orjson | 0.498 | 0.539 | 1.027 | 52.996 | 0.80x |
| mixed.json | msgspec | 0.507 | 0.574 | 0.622 | 52.996 | 0.75x |
| mixed.json | ujson | 0.698 | 0.741 | 0.861 | 52.996 | 0.58x |
| mixed.json | json | 0.909 | 0.967 | 1.047 | 52.996 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 18.542 | 18.692 | 19.137 | 57.367 | 1.00x |
| users.ndjson | orjson | 25.023 | 25.220 | 26.055 | 57.367 | 0.74x |
| users.ndjson | msgspec | 25.649 | 25.893 | 26.789 | 57.367 | 0.72x |
| users.ndjson | ujson | 36.582 | 36.891 | 37.846 | 57.367 | 0.51x |
| users.ndjson | json | 45.667 | 45.929 | 46.590 | 57.367 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.297 | 3.341 | 3.453 | 58.332 | 1.00x |
| users.json | orjson | 3.971 | 4.129 | 4.609 | 58.332 | 0.81x |
| users.json | msgspec | 5.766 | 5.790 | 6.037 | 58.332 | 0.58x |
| users.json | ujson | 30.867 | 30.930 | 31.758 | 58.332 | 0.11x |
| users.json | json | 40.047 | 40.436 | 41.143 | 58.332 | 0.08x |
| flat.json | strata | 0.537 | 0.601 | 0.654 | 53.488 | 1.00x |
| flat.json | orjson | 0.661 | 0.710 | 0.743 | 53.488 | 0.85x |
| flat.json | msgspec | 0.721 | 0.824 | 0.878 | 53.488 | 0.73x |
| flat.json | ujson | 2.800 | 2.874 | 4.167 | 53.488 | 0.21x |
| flat.json | json | 3.788 | 3.817 | 3.892 | 53.488 | 0.16x |
| nested.json | strata | 0.504 | 0.520 | 0.563 | 53.766 | 1.00x |
| nested.json | orjson | 0.636 | 0.658 | 0.736 | 53.766 | 0.79x |
| nested.json | msgspec | 0.791 | 0.845 | 0.874 | 53.766 | 0.62x |
| nested.json | ujson | 2.978 | 3.032 | 3.071 | 53.766 | 0.17x |
| nested.json | json | 4.694 | 4.730 | 5.485 | 53.766 | 0.11x |
| wide_arrays.json | strata | 2.264 | 2.288 | 2.419 | 56.070 | 1.00x |
| wide_arrays.json | orjson | 2.754 | 2.806 | 3.230 | 56.070 | 0.82x |
| wide_arrays.json | msgspec | 3.573 | 3.627 | 3.907 | 56.070 | 0.63x |
| wide_arrays.json | ujson | 14.994 | 15.095 | 15.945 | 56.070 | 0.15x |
| wide_arrays.json | json | 32.617 | 32.974 | 33.800 | 56.070 | 0.07x |
| mixed.json | strata | 0.308 | 0.342 | 0.412 | 52.996 | 1.00x |
| mixed.json | orjson | 0.320 | 0.333 | 0.409 | 52.996 | 1.03x |
| mixed.json | msgspec | 0.353 | 0.382 | 0.424 | 52.996 | 0.89x |
| mixed.json | ujson | 0.849 | 0.879 | 0.976 | 52.996 | 0.39x |
| mixed.json | json | 1.199 | 1.238 | 1.488 | 52.996 | 0.28x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.122 | 0.134 | 0.169 | 58.398 | 1.00x |
| users.json $[*].id | jmespath | 0.869 | 0.899 | 0.925 | 58.398 | 0.15x |
| users.json $[*].id | jsonpath-ng | 4.853 | 4.899 | 5.568 | 58.398 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.841 | 0.856 | 1.043 | 55.715 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.586 | 5.676 | 6.571 | 55.715 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 32.578 | 32.776 | 36.001 | 55.715 | 0.03x |
| users.json $..total | strata | 3.001 | 3.392 | 3.897 | 57.773 | 1.00x |
| users.json $..total | jsonpath-ng | 655.772 | 683.983 | 955.333 | 57.773 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.810 | 3.830 | 3.962 | 58.469 | 1.00x |
| users.json $[*].id | orjson+jmespath | 24.908 | 25.270 | 27.022 | 58.469 | 0.15x |
| users.json $[*].id | orjson+jsonpath-ng | 29.171 | 29.415 | 31.301 | 58.469 | 0.13x |
| users.json $[*].orders[*].total | strata | 3.996 | 4.029 | 4.129 | 56.977 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 29.752 | 30.351 | 32.929 | 56.977 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 61.360 | 61.696 | 69.849 | 56.977 | 0.07x |
| users.json $..total | strata | 21.728 | 22.100 | 23.379 | 57.012 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 681.347 | 688.120 | 724.687 | 57.012 | 0.03x |

