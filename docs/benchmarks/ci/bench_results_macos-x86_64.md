# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 1e1e47c
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
| users.json | strata | 18.256 | 19.133 | 21.757 | 52.543 | 1.00x |
| users.json | orjson | 23.070 | 25.134 | 29.205 | 52.543 | 0.76x |
| users.json | msgspec | 23.678 | 25.707 | 33.118 | 52.543 | 0.74x |
| users.json | ujson | 35.154 | 38.296 | 41.096 | 52.543 | 0.50x |
| users.json | pysimdjson | 154.063 | 157.459 | 163.231 | 52.543 | 0.12x |
| users.json | json | 39.853 | 42.416 | 47.356 | 52.543 | 0.45x |
| flat.json | strata | 1.191 | 1.237 | 1.605 | 59.504 | 1.00x |
| flat.json | orjson | 1.264 | 1.315 | 1.347 | 59.504 | 0.94x |
| flat.json | msgspec | 1.457 | 1.502 | 1.564 | 59.504 | 0.82x |
| flat.json | ujson | 2.550 | 2.622 | 2.758 | 59.504 | 0.47x |
| flat.json | pysimdjson | 13.776 | 13.950 | 14.150 | 59.504 | 0.09x |
| flat.json | json | 2.928 | 2.960 | 3.284 | 59.504 | 0.42x |
| nested.json | strata | 1.336 | 1.381 | 1.763 | 57.320 | 1.00x |
| nested.json | orjson | 1.507 | 1.557 | 1.794 | 57.320 | 0.89x |
| nested.json | msgspec | 1.676 | 1.721 | 1.822 | 57.320 | 0.80x |
| nested.json | ujson | 2.839 | 2.867 | 4.345 | 57.320 | 0.48x |
| nested.json | pysimdjson | 12.628 | 12.782 | 14.762 | 57.320 | 0.11x |
| nested.json | json | 3.603 | 3.648 | 3.756 | 57.320 | 0.38x |
| wide_arrays.json | strata | 7.975 | 8.154 | 9.008 | 59.965 | 1.00x |
| wide_arrays.json | orjson | 8.531 | 8.727 | 9.498 | 59.965 | 0.93x |
| wide_arrays.json | msgspec | 9.344 | 9.676 | 10.745 | 59.965 | 0.84x |
| wide_arrays.json | ujson | 12.150 | 12.250 | 13.042 | 59.965 | 0.67x |
| wide_arrays.json | pysimdjson | 74.124 | 75.933 | 77.651 | 59.965 | 0.11x |
| wide_arrays.json | json | 15.581 | 16.372 | 16.761 | 59.965 | 0.50x |
| mixed.json | strata | 0.341 | 0.352 | 0.428 | 58.434 | 1.00x |
| mixed.json | orjson | 0.403 | 0.421 | 0.441 | 58.434 | 0.84x |
| mixed.json | msgspec | 0.425 | 0.438 | 0.835 | 58.434 | 0.80x |
| mixed.json | ujson | 0.584 | 0.615 | 0.818 | 58.434 | 0.57x |
| mixed.json | pysimdjson | 3.015 | 3.075 | 3.153 | 58.434 | 0.11x |
| mixed.json | json | 0.820 | 0.853 | 0.912 | 58.434 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.321 | 2.387 | 2.783 | 48.219 | 1.00x |
| users.json | orjson | 2.944 | 3.181 | 3.463 | 48.219 | 0.75x |
| users.json | msgspec | 4.769 | 5.504 | 6.027 | 48.219 | 0.43x |
| users.json | ujson | 29.683 | 30.127 | 31.028 | 48.219 | 0.08x |
| users.json | json | 39.606 | 40.768 | 41.742 | 48.219 | 0.06x |
| flat.json | strata | 0.241 | 0.260 | 0.289 | 57.184 | 1.00x |
| flat.json | orjson | 0.344 | 0.375 | 0.524 | 57.184 | 0.69x |
| flat.json | msgspec | 0.465 | 0.497 | 0.540 | 57.184 | 0.52x |
| flat.json | ujson | 2.464 | 2.496 | 2.548 | 57.184 | 0.10x |
| flat.json | json | 3.408 | 3.460 | 3.492 | 57.184 | 0.08x |
| nested.json | strata | 0.198 | 0.215 | 0.232 | 52.223 | 1.00x |
| nested.json | orjson | 0.308 | 0.320 | 0.366 | 52.223 | 0.67x |
| nested.json | msgspec | 0.500 | 0.522 | 0.593 | 52.223 | 0.41x |
| nested.json | ujson | 2.468 | 2.603 | 2.914 | 52.223 | 0.08x |
| nested.json | json | 4.338 | 4.360 | 4.711 | 52.223 | 0.05x |
| wide_arrays.json | strata | 1.576 | 1.696 | 2.679 | 57.246 | 1.00x |
| wide_arrays.json | orjson | 2.036 | 2.153 | 2.531 | 57.246 | 0.79x |
| wide_arrays.json | msgspec | 2.977 | 2.989 | 3.449 | 57.246 | 0.57x |
| wide_arrays.json | ujson | 14.269 | 14.480 | 14.974 | 57.246 | 0.12x |
| wide_arrays.json | json | 32.182 | 32.717 | 33.723 | 57.246 | 0.05x |
| mixed.json | strata | 0.068 | 0.077 | 0.096 | 54.004 | 1.00x |
| mixed.json | orjson | 0.067 | 0.074 | 0.090 | 54.004 | 1.04x |
| mixed.json | msgspec | 0.096 | 0.109 | 0.124 | 54.004 | 0.71x |
| mixed.json | ujson | 0.578 | 0.589 | 0.621 | 54.004 | 0.13x |
| mixed.json | json | 0.897 | 0.916 | 0.994 | 54.004 | 0.08x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 18.458 | 18.823 | 19.499 | 60.527 | 1.00x |
| users.json | orjson | 23.795 | 24.825 | 28.333 | 60.527 | 0.76x |
| users.json | msgspec | 23.683 | 24.812 | 27.839 | 60.527 | 0.76x |
| users.json | ujson | 35.421 | 37.080 | 41.120 | 60.527 | 0.51x |
| users.json | json | 41.636 | 42.885 | 44.038 | 60.527 | 0.44x |
| flat.json | strata | 1.288 | 1.320 | 1.420 | 57.184 | 1.00x |
| flat.json | orjson | 1.376 | 1.413 | 2.300 | 57.184 | 0.93x |
| flat.json | msgspec | 1.593 | 1.616 | 1.708 | 57.184 | 0.82x |
| flat.json | ujson | 2.633 | 2.752 | 2.841 | 57.184 | 0.48x |
| flat.json | json | 3.030 | 3.079 | 3.394 | 57.184 | 0.43x |
| nested.json | strata | 1.563 | 1.580 | 1.622 | 52.223 | 1.00x |
| nested.json | orjson | 1.753 | 1.786 | 2.101 | 52.223 | 0.88x |
| nested.json | msgspec | 1.946 | 1.997 | 2.461 | 52.223 | 0.79x |
| nested.json | ujson | 3.151 | 3.197 | 3.299 | 52.223 | 0.49x |
| nested.json | json | 3.905 | 3.953 | 4.042 | 52.223 | 0.40x |
| wide_arrays.json | strata | 8.013 | 8.153 | 8.870 | 58.391 | 1.00x |
| wide_arrays.json | orjson | 8.387 | 8.768 | 9.622 | 58.391 | 0.93x |
| wide_arrays.json | msgspec | 9.595 | 9.804 | 10.508 | 58.391 | 0.83x |
| wide_arrays.json | ujson | 12.589 | 12.738 | 13.653 | 58.391 | 0.64x |
| wide_arrays.json | json | 15.956 | 16.249 | 16.916 | 58.391 | 0.50x |
| mixed.json | strata | 0.399 | 0.458 | 0.508 | 54.004 | 1.00x |
| mixed.json | orjson | 0.492 | 0.564 | 0.595 | 54.004 | 0.81x |
| mixed.json | msgspec | 0.535 | 0.598 | 0.985 | 54.004 | 0.77x |
| mixed.json | ujson | 0.700 | 0.777 | 1.001 | 54.004 | 0.59x |
| mixed.json | json | 0.904 | 0.980 | 1.097 | 54.004 | 0.47x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 18.697 | 19.304 | 19.870 | 59.844 | 1.00x |
| users.ndjson | orjson | 25.018 | 25.938 | 27.012 | 59.844 | 0.74x |
| users.ndjson | msgspec | 25.620 | 26.001 | 27.519 | 59.844 | 0.74x |
| users.ndjson | ujson | 37.343 | 38.432 | 39.109 | 59.844 | 0.50x |
| users.ndjson | json | 46.537 | 48.032 | 49.959 | 59.844 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.048 | 3.137 | 3.695 | 53.969 | 1.00x |
| users.json | orjson | 3.821 | 4.096 | 4.687 | 53.969 | 0.77x |
| users.json | msgspec | 5.645 | 6.227 | 6.588 | 53.969 | 0.50x |
| users.json | ujson | 30.799 | 31.140 | 31.988 | 53.969 | 0.10x |
| users.json | json | 40.218 | 41.483 | 43.526 | 53.969 | 0.08x |
| flat.json | strata | 0.559 | 0.589 | 0.694 | 57.184 | 1.00x |
| flat.json | orjson | 0.679 | 0.724 | 0.812 | 57.184 | 0.81x |
| flat.json | msgspec | 0.807 | 0.884 | 1.153 | 57.184 | 0.67x |
| flat.json | ujson | 2.778 | 2.927 | 3.268 | 57.184 | 0.20x |
| flat.json | json | 3.782 | 3.888 | 3.988 | 57.184 | 0.15x |
| nested.json | strata | 0.551 | 0.621 | 0.681 | 52.223 | 1.00x |
| nested.json | orjson | 0.690 | 0.744 | 0.918 | 52.223 | 0.83x |
| nested.json | msgspec | 0.914 | 0.994 | 1.380 | 52.223 | 0.62x |
| nested.json | ujson | 3.101 | 3.216 | 4.166 | 52.223 | 0.19x |
| nested.json | json | 4.778 | 4.981 | 5.374 | 52.223 | 0.12x |
| wide_arrays.json | strata | 2.141 | 2.363 | 2.851 | 58.391 | 1.00x |
| wide_arrays.json | orjson | 2.894 | 2.992 | 3.345 | 58.391 | 0.79x |
| wide_arrays.json | msgspec | 3.615 | 3.831 | 4.289 | 58.391 | 0.62x |
| wide_arrays.json | ujson | 15.132 | 15.403 | 16.329 | 58.391 | 0.15x |
| wide_arrays.json | json | 33.227 | 33.898 | 34.700 | 58.391 | 0.07x |
| mixed.json | strata | 0.308 | 0.348 | 0.422 | 54.004 | 1.00x |
| mixed.json | orjson | 0.328 | 0.385 | 0.452 | 54.004 | 0.90x |
| mixed.json | msgspec | 0.328 | 0.440 | 0.690 | 54.004 | 0.79x |
| mixed.json | ujson | 0.855 | 0.934 | 1.284 | 54.004 | 0.37x |
| mixed.json | json | 1.180 | 1.223 | 1.306 | 54.004 | 0.28x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.124 | 0.135 | 0.184 | 54.055 | 1.00x |
| users.json $[*].id | jmespath | 0.828 | 0.901 | 0.981 | 54.055 | 0.15x |
| users.json $[*].id | jsonpath-ng | 4.854 | 4.908 | 5.525 | 54.055 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.832 | 0.909 | 1.060 | 54.586 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.377 | 5.836 | 5.995 | 54.586 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 32.601 | 33.528 | 35.831 | 54.586 | 0.03x |
| users.json $..total | strata | 3.135 | 3.270 | 3.882 | 55.980 | 1.00x |
| users.json $..total | jsonpath-ng | 660.784 | 673.486 | 703.131 | 55.980 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.562 | 3.624 | 3.945 | 58.887 | 1.00x |
| users.json $[*].id | orjson+jmespath | 24.507 | 25.196 | 26.106 | 58.887 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 28.713 | 30.368 | 32.360 | 58.887 | 0.12x |
| users.json $[*].orders[*].total | strata | 3.947 | 3.969 | 6.657 | 55.906 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 30.181 | 32.411 | 35.570 | 55.906 | 0.12x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 62.699 | 68.311 | 73.535 | 55.906 | 0.06x |
| users.json $..total | strata | 21.816 | 22.317 | 25.277 | 56.074 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 690.163 | 696.393 | 723.229 | 56.074 | 0.03x |

