# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: b7f31bb
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
| users.json | strata | 19.833 | 20.501 | 25.404 | 52.793 | 1.00x |
| users.json | orjson | 27.813 | 28.547 | 32.908 | 52.793 | 0.72x |
| users.json | msgspec | 27.537 | 28.681 | 33.577 | 52.793 | 0.71x |
| users.json | ujson | 40.750 | 41.658 | 50.601 | 52.793 | 0.49x |
| users.json | pysimdjson | 180.406 | 185.432 | 193.608 | 52.793 | 0.11x |
| users.json | json | 46.211 | 48.054 | 50.001 | 52.793 | 0.43x |
| flat.json | strata | 1.523 | 1.556 | 1.666 | 58.156 | 1.00x |
| flat.json | orjson | 1.657 | 1.699 | 1.901 | 58.156 | 0.92x |
| flat.json | msgspec | 1.866 | 1.892 | 2.028 | 58.156 | 0.82x |
| flat.json | ujson | 3.211 | 3.296 | 3.400 | 58.156 | 0.47x |
| flat.json | pysimdjson | 17.179 | 17.674 | 18.328 | 58.156 | 0.09x |
| flat.json | json | 3.775 | 3.908 | 4.037 | 58.156 | 0.40x |
| nested.json | strata | 1.523 | 1.553 | 1.696 | 46.523 | 1.00x |
| nested.json | orjson | 1.784 | 1.809 | 1.970 | 46.523 | 0.86x |
| nested.json | msgspec | 1.968 | 2.024 | 2.192 | 46.523 | 0.77x |
| nested.json | ujson | 3.254 | 3.410 | 3.673 | 46.523 | 0.46x |
| nested.json | pysimdjson | 14.537 | 15.160 | 16.240 | 46.523 | 0.10x |
| nested.json | json | 4.141 | 4.237 | 4.728 | 46.523 | 0.37x |
| wide_arrays.json | strata | 8.534 | 8.681 | 9.779 | 57.977 | 1.00x |
| wide_arrays.json | orjson | 10.204 | 10.312 | 13.703 | 57.977 | 0.84x |
| wide_arrays.json | msgspec | 11.182 | 11.287 | 15.815 | 57.977 | 0.77x |
| wide_arrays.json | ujson | 14.207 | 14.359 | 15.447 | 57.977 | 0.60x |
| wide_arrays.json | pysimdjson | 90.875 | 91.102 | 95.808 | 57.977 | 0.10x |
| wide_arrays.json | json | 18.915 | 19.113 | 20.762 | 57.977 | 0.45x |
| mixed.json | strata | 0.385 | 0.388 | 0.424 | 53.234 | 1.00x |
| mixed.json | orjson | 0.476 | 0.479 | 0.515 | 53.234 | 0.81x |
| mixed.json | msgspec | 0.505 | 0.512 | 0.549 | 53.234 | 0.76x |
| mixed.json | ujson | 0.680 | 0.699 | 0.783 | 53.234 | 0.56x |
| mixed.json | pysimdjson | 3.588 | 3.610 | 3.729 | 53.234 | 0.11x |
| mixed.json | json | 0.973 | 0.984 | 0.999 | 53.234 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.566 | 2.694 | 3.416 | 48.246 | 1.00x |
| users.json | orjson | 3.528 | 3.753 | 4.458 | 48.246 | 0.72x |
| users.json | msgspec | 5.450 | 6.048 | 6.749 | 48.246 | 0.45x |
| users.json | ujson | 26.603 | 27.336 | 28.715 | 48.246 | 0.10x |
| users.json | json | 45.333 | 46.404 | 50.186 | 48.246 | 0.06x |
| flat.json | strata | 0.308 | 0.327 | 0.343 | 46.094 | 1.00x |
| flat.json | orjson | 0.410 | 0.477 | 0.610 | 46.094 | 0.69x |
| flat.json | msgspec | 0.539 | 0.562 | 0.597 | 46.094 | 0.58x |
| flat.json | ujson | 2.359 | 2.470 | 2.591 | 46.094 | 0.13x |
| flat.json | json | 3.936 | 3.999 | 4.345 | 46.094 | 0.08x |
| nested.json | strata | 0.290 | 0.296 | 0.568 | 46.379 | 1.00x |
| nested.json | orjson | 0.401 | 0.414 | 0.442 | 46.379 | 0.71x |
| nested.json | msgspec | 0.628 | 0.649 | 0.674 | 46.379 | 0.46x |
| nested.json | ujson | 2.562 | 2.702 | 2.739 | 46.379 | 0.11x |
| nested.json | json | 5.374 | 5.570 | 5.766 | 46.379 | 0.05x |
| wide_arrays.json | strata | 1.878 | 2.029 | 2.846 | 56.215 | 1.00x |
| wide_arrays.json | orjson | 2.534 | 2.658 | 2.856 | 56.215 | 0.76x |
| wide_arrays.json | msgspec | 3.613 | 3.807 | 3.958 | 56.215 | 0.53x |
| wide_arrays.json | ujson | 11.666 | 12.170 | 13.145 | 56.215 | 0.17x |
| wide_arrays.json | json | 39.189 | 39.725 | 44.404 | 56.215 | 0.05x |
| mixed.json | strata | 0.066 | 0.069 | 0.077 | 51.023 | 1.00x |
| mixed.json | orjson | 0.077 | 0.084 | 0.089 | 51.023 | 0.82x |
| mixed.json | msgspec | 0.112 | 0.118 | 0.213 | 51.023 | 0.59x |
| mixed.json | ujson | 0.501 | 0.512 | 0.538 | 51.023 | 0.14x |
| mixed.json | json | 1.059 | 1.071 | 1.086 | 51.023 | 0.06x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 20.108 | 20.640 | 23.113 | 60.852 | 1.00x |
| users.json | orjson | 27.339 | 28.738 | 29.957 | 60.852 | 0.72x |
| users.json | msgspec | 28.532 | 29.908 | 30.661 | 60.852 | 0.69x |
| users.json | ujson | 41.704 | 44.229 | 47.717 | 60.852 | 0.47x |
| users.json | json | 47.056 | 47.850 | 51.567 | 60.852 | 0.43x |
| flat.json | strata | 1.428 | 1.472 | 1.552 | 46.281 | 1.00x |
| flat.json | orjson | 1.634 | 1.680 | 1.869 | 46.281 | 0.88x |
| flat.json | msgspec | 1.842 | 1.887 | 1.967 | 46.281 | 0.78x |
| flat.json | ujson | 3.094 | 3.162 | 3.467 | 46.281 | 0.47x |
| flat.json | json | 3.565 | 3.591 | 3.665 | 46.281 | 0.41x |
| nested.json | strata | 1.670 | 1.720 | 1.747 | 46.379 | 1.00x |
| nested.json | orjson | 1.988 | 2.022 | 2.068 | 46.379 | 0.85x |
| nested.json | msgspec | 2.166 | 2.231 | 2.301 | 46.379 | 0.77x |
| nested.json | ujson | 3.479 | 3.590 | 3.760 | 46.379 | 0.48x |
| nested.json | json | 4.415 | 4.468 | 4.963 | 46.379 | 0.38x |
| wide_arrays.json | strata | 8.751 | 8.803 | 9.225 | 56.340 | 1.00x |
| wide_arrays.json | orjson | 10.352 | 10.407 | 10.901 | 56.340 | 0.85x |
| wide_arrays.json | msgspec | 11.504 | 11.665 | 11.786 | 56.340 | 0.75x |
| wide_arrays.json | ujson | 14.863 | 15.022 | 15.430 | 56.340 | 0.59x |
| wide_arrays.json | json | 19.340 | 19.431 | 20.315 | 56.340 | 0.45x |
| mixed.json | strata | 0.445 | 0.462 | 0.569 | 51.023 | 1.00x |
| mixed.json | orjson | 0.575 | 0.595 | 0.662 | 51.023 | 0.78x |
| mixed.json | msgspec | 0.607 | 0.633 | 0.684 | 51.023 | 0.73x |
| mixed.json | ujson | 0.788 | 0.828 | 0.894 | 51.023 | 0.56x |
| mixed.json | json | 1.051 | 1.077 | 1.130 | 51.023 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 20.624 | 20.757 | 21.965 | 58.629 | 1.00x |
| users.ndjson | orjson | 29.302 | 29.490 | 31.052 | 58.629 | 0.70x |
| users.ndjson | msgspec | 29.832 | 30.002 | 37.270 | 58.629 | 0.69x |
| users.ndjson | ujson | 42.853 | 43.659 | 44.246 | 58.629 | 0.48x |
| users.ndjson | json | 53.615 | 54.053 | 60.825 | 58.629 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.424 | 3.491 | 3.748 | 57.918 | 1.00x |
| users.json | orjson | 4.621 | 4.682 | 5.087 | 57.918 | 0.75x |
| users.json | msgspec | 6.655 | 6.719 | 7.301 | 57.918 | 0.52x |
| users.json | ujson | 28.450 | 28.624 | 30.707 | 57.918 | 0.12x |
| users.json | json | 47.704 | 48.308 | 50.838 | 57.918 | 0.07x |
| flat.json | strata | 0.653 | 0.687 | 0.716 | 46.281 | 1.00x |
| flat.json | orjson | 0.746 | 0.809 | 1.190 | 46.281 | 0.85x |
| flat.json | msgspec | 0.898 | 0.923 | 0.978 | 46.281 | 0.74x |
| flat.json | ujson | 2.748 | 2.814 | 3.103 | 46.281 | 0.24x |
| flat.json | json | 4.335 | 4.386 | 4.800 | 46.281 | 0.16x |
| nested.json | strata | 0.612 | 0.670 | 0.744 | 46.379 | 1.00x |
| nested.json | orjson | 0.764 | 0.855 | 1.095 | 46.379 | 0.78x |
| nested.json | msgspec | 0.998 | 1.060 | 1.170 | 46.379 | 0.63x |
| nested.json | ujson | 2.927 | 3.283 | 3.895 | 46.379 | 0.20x |
| nested.json | json | 5.499 | 5.864 | 7.079 | 46.379 | 0.11x |
| wide_arrays.json | strata | 2.406 | 2.509 | 2.804 | 56.340 | 1.00x |
| wide_arrays.json | orjson | 3.220 | 3.337 | 3.400 | 56.340 | 0.75x |
| wide_arrays.json | msgspec | 4.209 | 4.385 | 4.811 | 56.340 | 0.57x |
| wide_arrays.json | ujson | 12.195 | 12.418 | 13.181 | 56.340 | 0.20x |
| wide_arrays.json | json | 38.943 | 39.366 | 40.307 | 56.340 | 0.06x |
| mixed.json | strata | 0.319 | 0.335 | 0.376 | 51.027 | 1.00x |
| mixed.json | orjson | 0.304 | 0.402 | 0.445 | 51.027 | 0.83x |
| mixed.json | msgspec | 0.372 | 0.414 | 0.487 | 51.027 | 0.81x |
| mixed.json | ujson | 0.769 | 0.818 | 1.052 | 51.027 | 0.41x |
| mixed.json | json | 1.270 | 1.337 | 1.517 | 51.027 | 0.25x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.146 | 0.152 | 0.199 | 58.004 | 1.00x |
| users.json $[*].id | jmespath | 1.034 | 1.052 | 1.156 | 58.004 | 0.14x |
| users.json $[*].id | jsonpath-ng | 5.659 | 5.841 | 6.114 | 58.004 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.859 | 0.959 | 1.009 | 55.371 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 6.350 | 6.426 | 6.988 | 55.371 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 37.621 | 37.915 | 41.963 | 55.371 | 0.03x |
| users.json $..total | strata | 3.495 | 3.909 | 4.750 | 56.422 | 1.00x |
| users.json $..total | jsonpath-ng | 772.689 | 817.989 | 849.729 | 56.422 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.289 | 4.403 | 4.775 | 58.215 | 1.00x |
| users.json $[*].id | orjson+jmespath | 29.374 | 30.942 | 33.972 | 58.215 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 34.956 | 36.889 | 39.947 | 58.215 | 0.12x |
| users.json $[*].orders[*].total | strata | 4.561 | 4.623 | 4.967 | 56.422 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 33.516 | 34.554 | 38.228 | 56.422 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 70.324 | 72.738 | 81.143 | 56.422 | 0.06x |
| users.json $..total | strata | 23.664 | 24.158 | 26.338 | 56.398 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 791.539 | 833.659 | 884.072 | 56.398 | 0.03x |

