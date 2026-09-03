# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: f8c17d0
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
| users.json | strata | 20.734 | 23.607 | 33.522 | 52.809 | 1.00x |
| users.json | orjson | 30.039 | 35.454 | 41.381 | 52.809 | 0.67x |
| users.json | msgspec | 29.985 | 33.663 | 44.344 | 52.809 | 0.70x |
| users.json | ujson | 44.534 | 49.538 | 60.801 | 52.809 | 0.48x |
| users.json | pysimdjson | 179.319 | 196.927 | 227.097 | 52.809 | 0.12x |
| users.json | json | 49.467 | 55.477 | 63.943 | 52.809 | 0.43x |
| flat.json | strata | 1.381 | 1.531 | 2.240 | 61.625 | 1.00x |
| flat.json | orjson | 1.501 | 1.539 | 2.388 | 61.625 | 0.99x |
| flat.json | msgspec | 1.713 | 1.770 | 2.442 | 61.625 | 0.86x |
| flat.json | ujson | 3.043 | 3.155 | 3.939 | 61.625 | 0.49x |
| flat.json | pysimdjson | 15.832 | 16.580 | 19.483 | 61.625 | 0.09x |
| flat.json | json | 3.434 | 3.683 | 5.447 | 61.625 | 0.42x |
| nested.json | strata | 1.641 | 1.913 | 2.337 | 56.926 | 1.00x |
| nested.json | orjson | 1.874 | 1.942 | 2.758 | 56.926 | 0.99x |
| nested.json | msgspec | 2.030 | 2.272 | 3.165 | 56.926 | 0.84x |
| nested.json | ujson | 3.392 | 3.914 | 5.142 | 56.926 | 0.49x |
| nested.json | pysimdjson | 14.541 | 16.067 | 18.262 | 56.926 | 0.12x |
| nested.json | json | 4.305 | 5.054 | 5.871 | 56.926 | 0.38x |
| wide_arrays.json | strata | 8.991 | 9.908 | 12.751 | 61.203 | 1.00x |
| wide_arrays.json | orjson | 10.185 | 10.629 | 12.741 | 61.203 | 0.93x |
| wide_arrays.json | msgspec | 11.034 | 11.755 | 14.260 | 61.203 | 0.84x |
| wide_arrays.json | ujson | 14.167 | 15.709 | 18.598 | 61.203 | 0.63x |
| wide_arrays.json | pysimdjson | 83.402 | 89.976 | 96.132 | 61.203 | 0.11x |
| wide_arrays.json | json | 17.966 | 18.822 | 22.553 | 61.203 | 0.53x |
| mixed.json | strata | 0.408 | 0.421 | 0.543 | 58.426 | 1.00x |
| mixed.json | orjson | 0.476 | 0.504 | 0.633 | 58.426 | 0.84x |
| mixed.json | msgspec | 0.505 | 0.529 | 0.654 | 58.426 | 0.80x |
| mixed.json | ujson | 0.673 | 0.722 | 0.816 | 58.426 | 0.58x |
| mixed.json | pysimdjson | 3.435 | 3.578 | 4.291 | 58.426 | 0.12x |
| mixed.json | json | 0.973 | 0.997 | 1.395 | 58.426 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.046 | 3.151 | 4.870 | 51.586 | 1.00x |
| users.json | orjson | 3.870 | 4.067 | 4.796 | 51.586 | 0.77x |
| users.json | msgspec | 5.846 | 5.917 | 7.541 | 51.586 | 0.53x |
| users.json | ujson | 33.853 | 36.467 | 42.862 | 51.586 | 0.09x |
| users.json | json | 44.120 | 47.458 | 53.584 | 51.586 | 0.07x |
| flat.json | strata | 0.331 | 0.339 | 0.432 | 56.789 | 1.00x |
| flat.json | orjson | 0.466 | 0.474 | 0.616 | 56.789 | 0.72x |
| flat.json | msgspec | 0.603 | 0.614 | 0.776 | 56.789 | 0.55x |
| flat.json | ujson | 2.861 | 2.896 | 3.118 | 56.789 | 0.12x |
| flat.json | json | 3.930 | 3.998 | 4.852 | 56.789 | 0.08x |
| nested.json | strata | 0.281 | 0.298 | 0.350 | 57.062 | 1.00x |
| nested.json | orjson | 0.388 | 0.462 | 0.761 | 57.062 | 0.65x |
| nested.json | msgspec | 0.625 | 0.664 | 1.056 | 57.062 | 0.45x |
| nested.json | ujson | 3.051 | 3.111 | 4.820 | 57.062 | 0.10x |
| nested.json | json | 5.020 | 5.500 | 7.968 | 57.062 | 0.05x |
| wide_arrays.json | strata | 2.061 | 2.132 | 3.182 | 59.297 | 1.00x |
| wide_arrays.json | orjson | 2.893 | 3.012 | 3.420 | 59.297 | 0.71x |
| wide_arrays.json | msgspec | 3.505 | 3.687 | 4.741 | 59.297 | 0.58x |
| wide_arrays.json | ujson | 16.418 | 17.698 | 22.623 | 59.297 | 0.12x |
| wide_arrays.json | json | 35.974 | 39.789 | 42.602 | 59.297 | 0.05x |
| mixed.json | strata | 0.085 | 0.089 | 0.121 | 56.289 | 1.00x |
| mixed.json | orjson | 0.082 | 0.088 | 0.156 | 56.289 | 1.01x |
| mixed.json | msgspec | 0.120 | 0.124 | 0.219 | 56.289 | 0.72x |
| mixed.json | ujson | 0.659 | 0.667 | 0.689 | 56.289 | 0.13x |
| mixed.json | json | 1.014 | 1.025 | 1.153 | 56.289 | 0.09x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 21.559 | 25.469 | 56.608 | 61.602 | 1.00x |
| users.json | orjson | 32.141 | 35.583 | 43.226 | 61.602 | 0.72x |
| users.json | msgspec | 31.376 | 34.805 | 49.031 | 61.602 | 0.73x |
| users.json | ujson | 48.370 | 51.473 | 65.822 | 61.602 | 0.49x |
| users.json | json | 53.844 | 57.220 | 141.486 | 61.602 | 0.45x |
| flat.json | strata | 1.512 | 1.925 | 2.332 | 56.789 | 1.00x |
| flat.json | orjson | 1.681 | 1.987 | 2.126 | 56.789 | 0.97x |
| flat.json | msgspec | 1.949 | 2.293 | 2.981 | 56.789 | 0.84x |
| flat.json | ujson | 3.271 | 3.586 | 4.876 | 56.789 | 0.54x |
| flat.json | json | 3.556 | 4.223 | 5.033 | 56.789 | 0.46x |
| nested.json | strata | 1.764 | 1.873 | 2.820 | 57.062 | 1.00x |
| nested.json | orjson | 2.048 | 2.116 | 3.207 | 57.062 | 0.89x |
| nested.json | msgspec | 2.233 | 2.435 | 3.520 | 57.062 | 0.77x |
| nested.json | ujson | 3.618 | 3.694 | 4.978 | 57.062 | 0.51x |
| nested.json | json | 4.465 | 4.564 | 6.432 | 57.062 | 0.41x |
| wide_arrays.json | strata | 9.198 | 11.386 | 12.984 | 60.523 | 1.00x |
| wide_arrays.json | orjson | 10.356 | 11.768 | 14.836 | 60.523 | 0.97x |
| wide_arrays.json | msgspec | 11.463 | 13.000 | 16.082 | 60.523 | 0.88x |
| wide_arrays.json | ujson | 15.038 | 18.161 | 20.589 | 60.523 | 0.63x |
| wide_arrays.json | json | 18.375 | 21.561 | 25.077 | 60.523 | 0.53x |
| mixed.json | strata | 0.469 | 0.497 | 0.522 | 56.289 | 1.00x |
| mixed.json | orjson | 0.576 | 0.603 | 0.703 | 56.289 | 0.82x |
| mixed.json | msgspec | 0.589 | 0.644 | 0.687 | 56.289 | 0.77x |
| mixed.json | ujson | 0.789 | 0.840 | 0.939 | 56.289 | 0.59x |
| mixed.json | json | 1.026 | 1.072 | 1.148 | 56.289 | 0.46x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 21.399 | 22.480 | 24.404 | 60.664 | 1.00x |
| users.ndjson | orjson | 30.534 | 32.149 | 38.856 | 60.664 | 0.70x |
| users.ndjson | msgspec | 30.380 | 31.402 | 40.823 | 60.664 | 0.72x |
| users.ndjson | ujson | 43.775 | 46.673 | 51.273 | 60.664 | 0.48x |
| users.ndjson | json | 55.058 | 57.482 | 65.829 | 60.664 | 0.39x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.265 | 4.849 | 5.914 | 59.660 | 1.00x |
| users.json | orjson | 5.050 | 6.028 | 7.025 | 59.660 | 0.80x |
| users.json | msgspec | 7.034 | 8.142 | 9.936 | 59.660 | 0.60x |
| users.json | ujson | 36.215 | 44.292 | 51.170 | 59.660 | 0.11x |
| users.json | json | 48.728 | 57.363 | 64.212 | 59.660 | 0.08x |
| flat.json | strata | 0.718 | 0.784 | 1.007 | 56.789 | 1.00x |
| flat.json | orjson | 0.874 | 1.035 | 1.230 | 56.789 | 0.76x |
| flat.json | msgspec | 1.078 | 1.209 | 1.596 | 56.789 | 0.65x |
| flat.json | ujson | 3.337 | 3.690 | 4.511 | 56.789 | 0.21x |
| flat.json | json | 4.495 | 5.404 | 6.071 | 56.789 | 0.15x |
| nested.json | strata | 0.651 | 0.704 | 0.859 | 57.062 | 1.00x |
| nested.json | orjson | 0.794 | 0.869 | 1.110 | 57.062 | 0.81x |
| nested.json | msgspec | 1.046 | 1.200 | 1.505 | 57.062 | 0.59x |
| nested.json | ujson | 3.552 | 4.048 | 5.331 | 57.062 | 0.17x |
| nested.json | json | 5.546 | 6.314 | 7.947 | 57.062 | 0.11x |
| wide_arrays.json | strata | 2.710 | 2.847 | 3.985 | 59.363 | 1.00x |
| wide_arrays.json | orjson | 3.201 | 3.422 | 4.629 | 59.363 | 0.83x |
| wide_arrays.json | msgspec | 4.231 | 4.371 | 4.605 | 59.363 | 0.65x |
| wide_arrays.json | ujson | 17.341 | 17.515 | 19.806 | 59.363 | 0.16x |
| wide_arrays.json | json | 37.404 | 37.697 | 41.728 | 59.363 | 0.08x |
| mixed.json | strata | 0.327 | 0.357 | 0.419 | 56.289 | 1.00x |
| mixed.json | orjson | 0.346 | 0.392 | 0.421 | 56.289 | 0.91x |
| mixed.json | msgspec | 0.348 | 0.414 | 0.447 | 56.289 | 0.86x |
| mixed.json | ujson | 0.913 | 0.970 | 1.056 | 56.289 | 0.37x |
| mixed.json | json | 1.264 | 1.312 | 1.516 | 56.289 | 0.27x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.200 | 0.207 | 0.312 | 59.727 | 1.00x |
| users.json $[*].id | jmespath | 1.113 | 1.158 | 1.618 | 59.727 | 0.18x |
| users.json $[*].id | jsonpath-ng | 5.994 | 6.175 | 7.266 | 59.727 | 0.03x |
| users.json $[*].orders[*].total | strata | 1.285 | 1.466 | 1.715 | 57.004 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 6.985 | 8.167 | 10.023 | 57.004 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 43.143 | 46.920 | 51.005 | 57.004 | 0.03x |
| users.json $..total | strata | 3.955 | 4.168 | 5.606 | 57.023 | 1.00x |
| users.json $..total | jsonpath-ng | 775.730 | 853.225 | 967.003 | 57.023 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.385 | 5.463 | 6.945 | 59.797 | 1.00x |
| users.json $[*].id | orjson+jmespath | 31.913 | 34.940 | 36.947 | 59.797 | 0.16x |
| users.json $[*].id | orjson+jsonpath-ng | 38.027 | 40.493 | 46.153 | 59.797 | 0.13x |
| users.json $[*].orders[*].total | strata | 4.473 | 4.928 | 7.028 | 56.930 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 38.703 | 41.172 | 47.066 | 56.930 | 0.12x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 82.065 | 91.428 | 100.300 | 56.930 | 0.05x |
| users.json $..total | strata | 25.384 | 27.158 | 30.459 | 57.051 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 839.547 | 871.966 | 958.121 | 57.051 | 0.03x |

