# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: cd9d20b6ab3ec573716c10b12fe76ae4b70a707c
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
| users.json | strata | 16.757 | 17.231 | 22.974 | 57.133 | 1.00x |
| users.json | orjson | 23.221 | 25.291 | 30.450 | 57.133 | 0.68x |
| users.json | msgspec | 23.701 | 24.843 | 29.414 | 57.133 | 0.69x |
| users.json | ujson | 35.369 | 36.204 | 43.606 | 57.133 | 0.48x |
| users.json | pysimdjson | 152.820 | 154.500 | 167.114 | 57.133 | 0.11x |
| users.json | json | 39.698 | 41.034 | 44.678 | 57.133 | 0.42x |
| flat.json | strata | 1.242 | 1.263 | 1.327 | 68.004 | 1.00x |
| flat.json | orjson | 1.347 | 1.383 | 1.479 | 68.004 | 0.91x |
| flat.json | msgspec | 1.524 | 1.547 | 1.575 | 68.004 | 0.82x |
| flat.json | ujson | 2.660 | 2.723 | 3.144 | 68.004 | 0.46x |
| flat.json | pysimdjson | 14.133 | 14.388 | 15.168 | 68.004 | 0.09x |
| flat.json | json | 3.037 | 3.084 | 3.597 | 68.004 | 0.41x |
| nested.json | strata | 1.335 | 1.364 | 1.413 | 66.488 | 1.00x |
| nested.json | orjson | 1.543 | 1.565 | 1.670 | 66.488 | 0.87x |
| nested.json | msgspec | 1.704 | 1.712 | 1.839 | 66.488 | 0.80x |
| nested.json | ujson | 2.812 | 2.836 | 2.995 | 66.488 | 0.48x |
| nested.json | pysimdjson | 12.601 | 12.711 | 13.390 | 66.488 | 0.11x |
| nested.json | json | 3.608 | 3.638 | 3.775 | 66.488 | 0.38x |
| wide_arrays.json | strata | 7.054 | 7.235 | 8.126 | 72.453 | 1.00x |
| wide_arrays.json | orjson | 9.393 | 9.698 | 10.679 | 72.453 | 0.75x |
| wide_arrays.json | msgspec | 9.641 | 9.882 | 10.392 | 72.453 | 0.73x |
| wide_arrays.json | ujson | 12.055 | 12.570 | 13.419 | 72.453 | 0.58x |
| wide_arrays.json | pysimdjson | 74.485 | 75.984 | 76.694 | 72.453 | 0.10x |
| wide_arrays.json | json | 15.660 | 16.452 | 20.617 | 72.453 | 0.44x |
| mixed.json | strata | 0.328 | 0.329 | 0.338 | 65.250 | 1.00x |
| mixed.json | orjson | 0.406 | 0.410 | 0.440 | 65.250 | 0.80x |
| mixed.json | msgspec | 0.429 | 0.432 | 0.456 | 65.250 | 0.76x |
| mixed.json | ujson | 0.587 | 0.595 | 0.653 | 65.250 | 0.55x |
| mixed.json | pysimdjson | 3.028 | 3.042 | 3.090 | 65.250 | 0.11x |
| mixed.json | json | 0.832 | 0.838 | 0.877 | 65.250 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.214 | 2.481 | 2.850 | 52.422 | 1.00x |
| users.json | orjson | 3.044 | 3.223 | 3.523 | 52.422 | 0.77x |
| users.json | msgspec | 4.819 | 4.983 | 5.316 | 52.422 | 0.50x |
| users.json | ujson | 23.010 | 23.185 | 23.309 | 52.422 | 0.11x |
| users.json | json | 38.366 | 39.219 | 39.913 | 52.422 | 0.06x |
| flat.json | strata | 0.328 | 0.346 | 0.494 | 66.527 | 1.00x |
| flat.json | orjson | 0.406 | 0.417 | 0.701 | 66.527 | 0.83x |
| flat.json | msgspec | 0.533 | 0.555 | 0.850 | 66.527 | 0.62x |
| flat.json | ujson | 2.120 | 2.182 | 3.258 | 66.527 | 0.16x |
| flat.json | json | 3.513 | 3.659 | 5.783 | 66.527 | 0.09x |
| nested.json | strata | 0.209 | 0.229 | 0.291 | 66.617 | 1.00x |
| nested.json | orjson | 0.323 | 0.344 | 0.356 | 66.617 | 0.67x |
| nested.json | msgspec | 0.508 | 0.531 | 0.782 | 66.617 | 0.43x |
| nested.json | ujson | 2.157 | 2.184 | 2.288 | 66.617 | 0.10x |
| nested.json | json | 4.312 | 4.409 | 4.464 | 66.617 | 0.05x |
| wide_arrays.json | strata | 1.636 | 1.841 | 2.012 | 66.180 | 1.00x |
| wide_arrays.json | orjson | 2.183 | 2.370 | 2.621 | 66.180 | 0.78x |
| wide_arrays.json | msgspec | 3.167 | 3.301 | 3.476 | 66.180 | 0.56x |
| wide_arrays.json | ujson | 9.752 | 10.535 | 10.737 | 66.180 | 0.17x |
| wide_arrays.json | json | 32.057 | 32.473 | 32.713 | 66.180 | 0.06x |
| mixed.json | strata | 0.056 | 0.059 | 0.062 | 62.051 | 1.00x |
| mixed.json | orjson | 0.066 | 0.075 | 0.090 | 62.051 | 0.78x |
| mixed.json | msgspec | 0.095 | 0.100 | 0.116 | 62.051 | 0.58x |
| mixed.json | ujson | 0.421 | 0.425 | 0.434 | 62.051 | 0.14x |
| mixed.json | json | 0.884 | 0.895 | 0.905 | 62.051 | 0.07x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 17.179 | 17.465 | 18.706 | 63.195 | 1.00x |
| users.json | orjson | 24.631 | 26.389 | 27.503 | 63.195 | 0.66x |
| users.json | msgspec | 23.960 | 26.106 | 27.913 | 63.195 | 0.67x |
| users.json | ujson | 36.935 | 38.488 | 38.997 | 63.195 | 0.45x |
| users.json | json | 40.674 | 41.914 | 43.584 | 63.195 | 0.42x |
| flat.json | strata | 1.282 | 1.371 | 1.503 | 66.527 | 1.00x |
| flat.json | orjson | 1.455 | 1.588 | 2.181 | 66.527 | 0.86x |
| flat.json | msgspec | 1.593 | 1.742 | 2.662 | 66.527 | 0.79x |
| flat.json | ujson | 2.701 | 2.877 | 3.671 | 66.527 | 0.48x |
| flat.json | json | 3.086 | 3.189 | 4.355 | 66.527 | 0.43x |
| nested.json | strata | 1.413 | 1.497 | 1.836 | 66.617 | 1.00x |
| nested.json | orjson | 1.643 | 1.713 | 1.872 | 66.617 | 0.87x |
| nested.json | msgspec | 1.814 | 1.899 | 1.957 | 66.617 | 0.79x |
| nested.json | ujson | 2.924 | 3.052 | 3.074 | 66.617 | 0.49x |
| nested.json | json | 3.713 | 3.878 | 4.169 | 66.617 | 0.39x |
| wide_arrays.json | strata | 7.155 | 7.208 | 7.466 | 67.410 | 1.00x |
| wide_arrays.json | orjson | 8.790 | 8.863 | 9.044 | 67.410 | 0.81x |
| wide_arrays.json | msgspec | 9.778 | 9.845 | 10.051 | 67.410 | 0.73x |
| wide_arrays.json | ujson | 12.379 | 12.522 | 13.018 | 67.410 | 0.58x |
| wide_arrays.json | json | 15.950 | 16.222 | 17.503 | 67.410 | 0.44x |
| mixed.json | strata | 0.387 | 0.397 | 0.417 | 62.051 | 1.00x |
| mixed.json | orjson | 0.505 | 0.527 | 0.558 | 62.051 | 0.75x |
| mixed.json | msgspec | 0.532 | 0.548 | 0.579 | 62.051 | 0.72x |
| mixed.json | ujson | 0.712 | 0.720 | 0.763 | 62.051 | 0.55x |
| mixed.json | json | 0.926 | 0.938 | 0.974 | 62.051 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 18.472 | 19.017 | 19.856 | 67.582 | 1.00x |
| users.ndjson | orjson | 26.814 | 27.733 | 30.466 | 67.582 | 0.69x |
| users.ndjson | msgspec | 27.734 | 28.509 | 30.629 | 67.582 | 0.67x |
| users.ndjson | ujson | 39.866 | 40.803 | 42.562 | 67.582 | 0.47x |
| users.ndjson | json | 49.096 | 50.419 | 51.892 | 67.582 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.280 | 3.621 | 4.799 | 63.223 | 1.00x |
| users.json | orjson | 4.494 | 4.755 | 6.573 | 63.223 | 0.76x |
| users.json | msgspec | 6.049 | 6.406 | 7.971 | 63.223 | 0.57x |
| users.json | ujson | 24.778 | 25.334 | 28.370 | 63.223 | 0.14x |
| users.json | json | 40.556 | 41.995 | 54.457 | 63.223 | 0.09x |
| flat.json | strata | 0.652 | 0.747 | 6.583 | 66.527 | 1.00x |
| flat.json | orjson | 0.739 | 0.828 | 1.221 | 66.527 | 0.90x |
| flat.json | msgspec | 0.874 | 0.998 | 1.186 | 66.527 | 0.75x |
| flat.json | ujson | 2.578 | 2.696 | 2.924 | 66.527 | 0.28x |
| flat.json | json | 3.897 | 4.131 | 4.226 | 66.527 | 0.18x |
| nested.json | strata | 0.502 | 0.540 | 0.605 | 66.617 | 1.00x |
| nested.json | orjson | 0.646 | 0.708 | 1.099 | 66.617 | 0.76x |
| nested.json | msgspec | 0.842 | 0.888 | 1.169 | 66.617 | 0.61x |
| nested.json | ujson | 2.537 | 2.660 | 2.862 | 66.617 | 0.20x |
| nested.json | json | 4.715 | 5.038 | 5.422 | 66.617 | 0.11x |
| wide_arrays.json | strata | 2.298 | 2.530 | 2.814 | 66.180 | 1.00x |
| wide_arrays.json | orjson | 3.039 | 3.130 | 3.314 | 66.180 | 0.81x |
| wide_arrays.json | msgspec | 3.983 | 4.145 | 4.286 | 66.180 | 0.61x |
| wide_arrays.json | ujson | 11.051 | 11.561 | 11.698 | 66.180 | 0.22x |
| wide_arrays.json | json | 33.066 | 33.227 | 34.029 | 66.180 | 0.08x |
| mixed.json | strata | 0.301 | 0.337 | 0.381 | 62.051 | 1.00x |
| mixed.json | orjson | 0.348 | 0.376 | 0.436 | 62.051 | 0.90x |
| mixed.json | msgspec | 0.388 | 0.411 | 0.441 | 62.051 | 0.82x |
| mixed.json | ujson | 0.716 | 0.765 | 0.868 | 62.051 | 0.44x |
| mixed.json | json | 1.196 | 1.216 | 1.294 | 62.051 | 0.28x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.121 | 0.134 | 0.157 | 63.297 | 1.00x |
| users.json $[*].id | jmespath | 0.865 | 0.894 | 1.009 | 63.297 | 0.15x |
| users.json $[*].id | jsonpath-ng | 4.873 | 4.902 | 5.091 | 63.297 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.815 | 0.902 | 0.997 | 60.449 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.534 | 5.618 | 6.645 | 60.449 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 31.071 | 32.858 | 34.530 | 60.449 | 0.03x |
| users.json $..total | strata | 3.031 | 3.378 | 3.758 | 60.551 | 1.00x |
| users.json $..total | jsonpath-ng | 654.799 | 665.402 | 704.139 | 60.551 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.598 | 3.620 | 3.637 | 63.355 | 1.00x |
| users.json $[*].id | orjson+jmespath | 24.809 | 26.011 | 27.051 | 63.355 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 29.540 | 30.121 | 30.566 | 63.355 | 0.12x |
| users.json $[*].orders[*].total | strata | 4.011 | 4.035 | 4.090 | 60.523 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 30.191 | 30.641 | 32.208 | 60.523 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 61.605 | 62.842 | 64.446 | 60.523 | 0.06x |
| users.json $..total | strata | 20.825 | 21.945 | 24.840 | 60.605 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 679.140 | 749.134 | 784.945 | 60.605 | 0.03x |

