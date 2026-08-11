# Benchmark results - ci-ubuntu-latest

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 16b0a58
- python: 3.12.13
- implementation: CPython
- platform: Linux-6.17.0-1020-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: x86_64
- compiler_flags: -std=c++20 -O3 -march=native
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 16.789 | 17.522 | 26.936 | 59.242 | 1.00x |
| users.json | orjson | 16.485 | 16.758 | 18.836 | 59.242 | 1.05x |
| users.json | msgspec | 15.998 | 16.611 | 19.582 | 59.242 | 1.05x |
| users.json | ujson | 22.370 | 22.860 | 25.805 | 59.242 | 0.77x |
| users.json | pysimdjson | 22.826 | 23.375 | 26.236 | 59.242 | 0.75x |
| users.json | json | 24.519 | 24.868 | 28.132 | 59.242 | 0.70x |
| flat.json | strata | 1.435 | 1.469 | 1.510 | 70.645 | 1.00x |
| flat.json | orjson | 1.213 | 1.230 | 1.257 | 70.645 | 1.19x |
| flat.json | msgspec | 1.151 | 1.165 | 1.191 | 70.645 | 1.26x |
| flat.json | ujson | 1.639 | 1.695 | 1.715 | 70.645 | 0.87x |
| flat.json | pysimdjson | 1.856 | 1.904 | 1.927 | 70.645 | 0.77x |
| flat.json | json | 1.933 | 1.982 | 2.050 | 70.645 | 0.74x |
| nested.json | strata | 1.239 | 1.280 | 1.373 | 70.645 | 1.00x |
| nested.json | orjson | 1.010 | 1.033 | 1.054 | 70.645 | 1.24x |
| nested.json | msgspec | 1.024 | 1.046 | 1.097 | 70.645 | 1.22x |
| nested.json | ujson | 1.520 | 1.541 | 1.579 | 70.645 | 0.83x |
| nested.json | pysimdjson | 1.412 | 1.452 | 1.504 | 70.645 | 0.88x |
| nested.json | json | 1.950 | 2.012 | 2.062 | 70.645 | 0.64x |
| wide_arrays.json | strata | 6.878 | 7.046 | 7.145 | 74.707 | 1.00x |
| wide_arrays.json | orjson | 5.608 | 5.875 | 6.022 | 74.707 | 1.20x |
| wide_arrays.json | msgspec | 5.778 | 5.905 | 6.986 | 74.707 | 1.19x |
| wide_arrays.json | ujson | 7.441 | 7.603 | 7.819 | 74.707 | 0.93x |
| wide_arrays.json | pysimdjson | 5.801 | 6.097 | 6.385 | 74.707 | 1.16x |
| wide_arrays.json | json | 9.904 | 10.129 | 10.573 | 74.707 | 0.70x |
| mixed.json | strata | 0.336 | 0.344 | 0.364 | 74.832 | 1.00x |
| mixed.json | orjson | 0.262 | 0.275 | 0.308 | 74.832 | 1.25x |
| mixed.json | msgspec | 0.265 | 0.282 | 0.304 | 74.832 | 1.22x |
| mixed.json | ujson | 0.351 | 0.372 | 0.415 | 74.832 | 0.92x |
| mixed.json | pysimdjson | 0.336 | 0.349 | 0.373 | 74.832 | 0.98x |
| mixed.json | json | 0.512 | 0.519 | 0.527 | 74.832 | 0.66x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.231 | 3.295 | 3.445 | 58.430 | 1.00x |
| users.json | orjson | 2.207 | 2.292 | 2.901 | 58.430 | 1.44x |
| users.json | msgspec | 3.986 | 4.048 | 4.162 | 58.430 | 0.81x |
| users.json | ujson | 14.619 | 14.807 | 15.841 | 58.430 | 0.22x |
| users.json | json | 21.823 | 22.105 | 23.175 | 58.430 | 0.15x |
| flat.json | strata | 0.361 | 0.372 | 0.384 | 70.645 | 1.00x |
| flat.json | orjson | 0.306 | 0.322 | 0.337 | 70.645 | 1.16x |
| flat.json | msgspec | 0.468 | 0.490 | 0.500 | 70.645 | 0.76x |
| flat.json | ujson | 1.282 | 1.318 | 1.391 | 70.645 | 0.28x |
| flat.json | json | 1.903 | 1.951 | 1.994 | 70.645 | 0.19x |
| nested.json | strata | 0.262 | 0.281 | 0.303 | 70.645 | 1.00x |
| nested.json | orjson | 0.246 | 0.253 | 0.266 | 70.645 | 1.11x |
| nested.json | msgspec | 0.407 | 0.415 | 0.457 | 70.645 | 0.68x |
| nested.json | ujson | 1.349 | 1.357 | 1.409 | 70.645 | 0.21x |
| nested.json | json | 2.335 | 2.386 | 2.416 | 70.645 | 0.12x |
| wide_arrays.json | strata | 1.863 | 1.902 | 1.985 | 74.707 | 1.00x |
| wide_arrays.json | orjson | 1.611 | 1.629 | 1.671 | 74.707 | 1.17x |
| wide_arrays.json | msgspec | 2.654 | 2.702 | 2.737 | 74.707 | 0.70x |
| wide_arrays.json | ujson | 7.991 | 8.074 | 8.256 | 74.707 | 0.24x |
| wide_arrays.json | json | 15.490 | 15.692 | 15.890 | 74.707 | 0.12x |
| mixed.json | strata | 0.087 | 0.090 | 0.093 | 74.832 | 1.00x |
| mixed.json | orjson | 0.059 | 0.063 | 0.064 | 74.832 | 1.43x |
| mixed.json | msgspec | 0.089 | 0.092 | 0.104 | 74.832 | 0.98x |
| mixed.json | ujson | 0.317 | 0.318 | 0.334 | 74.832 | 0.28x |
| mixed.json | json | 0.552 | 0.563 | 0.581 | 74.832 | 0.16x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 18.131 | 18.379 | 19.327 | 72.754 | 1.00x |
| users.json | orjson | 17.271 | 17.628 | 18.720 | 72.754 | 1.04x |
| users.json | msgspec | 17.486 | 17.674 | 18.042 | 72.754 | 1.04x |
| users.json | ujson | 24.094 | 24.428 | 25.893 | 72.754 | 0.75x |
| users.json | json | 25.248 | 25.690 | 26.355 | 72.754 | 0.72x |
| flat.json | strata | 1.480 | 1.542 | 1.554 | 70.645 | 1.00x |
| flat.json | orjson | 1.317 | 1.338 | 1.368 | 70.645 | 1.15x |
| flat.json | msgspec | 1.207 | 1.282 | 1.698 | 70.645 | 1.20x |
| flat.json | ujson | 1.774 | 1.803 | 1.848 | 70.645 | 0.86x |
| flat.json | json | 2.022 | 2.061 | 2.083 | 70.645 | 0.75x |
| nested.json | strata | 1.304 | 1.351 | 1.360 | 70.645 | 1.00x |
| nested.json | orjson | 1.095 | 1.156 | 1.174 | 70.645 | 1.17x |
| nested.json | msgspec | 1.129 | 1.158 | 1.173 | 70.645 | 1.17x |
| nested.json | ujson | 1.645 | 1.658 | 1.722 | 70.645 | 0.81x |
| nested.json | json | 2.020 | 2.090 | 2.145 | 70.645 | 0.65x |
| wide_arrays.json | strata | 7.270 | 7.388 | 7.508 | 74.770 | 1.00x |
| wide_arrays.json | orjson | 5.961 | 6.229 | 6.540 | 74.770 | 1.19x |
| wide_arrays.json | msgspec | 6.102 | 6.187 | 6.377 | 74.770 | 1.19x |
| wide_arrays.json | ujson | 7.965 | 8.238 | 8.692 | 74.770 | 0.90x |
| wide_arrays.json | json | 10.450 | 10.641 | 10.825 | 74.770 | 0.69x |
| mixed.json | strata | 0.376 | 0.387 | 0.413 | 74.832 | 1.00x |
| mixed.json | orjson | 0.344 | 0.362 | 0.454 | 74.832 | 1.07x |
| mixed.json | msgspec | 0.339 | 0.371 | 0.400 | 74.832 | 1.04x |
| mixed.json | ujson | 0.440 | 0.462 | 0.529 | 74.832 | 0.84x |
| mixed.json | json | 0.565 | 0.578 | 0.687 | 74.832 | 0.67x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 17.296 | 17.501 | 27.509 | 70.645 | 1.00x |
| users.ndjson | orjson | 19.896 | 20.067 | 26.408 | 70.645 | 0.87x |
| users.ndjson | msgspec | 19.565 | 19.818 | 20.025 | 70.645 | 0.88x |
| users.ndjson | ujson | 25.243 | 25.864 | 27.191 | 70.645 | 0.68x |
| users.ndjson | json | 30.970 | 31.270 | 33.481 | 70.645 | 0.56x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.941 | 4.145 | 8.053 | 68.816 | 1.00x |
| users.json | orjson | 2.957 | 3.099 | 6.117 | 68.816 | 1.34x |
| users.json | msgspec | 4.866 | 5.024 | 5.181 | 68.816 | 0.82x |
| users.json | ujson | 15.808 | 16.054 | 16.323 | 68.816 | 0.26x |
| users.json | json | 23.317 | 23.517 | 24.004 | 68.816 | 0.18x |
| flat.json | strata | 0.514 | 0.578 | 0.632 | 70.645 | 1.00x |
| flat.json | orjson | 0.499 | 0.538 | 0.579 | 70.645 | 1.08x |
| flat.json | msgspec | 0.687 | 0.717 | 12.518 | 70.645 | 0.81x |
| flat.json | ujson | 1.516 | 1.585 | 1.757 | 70.645 | 0.36x |
| flat.json | json | 2.167 | 2.206 | 2.273 | 70.645 | 0.26x |
| nested.json | strata | 0.439 | 0.454 | 0.474 | 70.645 | 1.00x |
| nested.json | orjson | 0.406 | 0.433 | 0.454 | 70.645 | 1.05x |
| nested.json | msgspec | 0.548 | 0.586 | 0.615 | 70.645 | 0.77x |
| nested.json | ujson | 1.512 | 1.564 | 1.634 | 70.645 | 0.29x |
| nested.json | json | 2.579 | 2.612 | 2.666 | 70.645 | 0.17x |
| wide_arrays.json | strata | 2.363 | 2.443 | 2.636 | 74.770 | 1.00x |
| wide_arrays.json | orjson | 2.129 | 2.213 | 22.022 | 74.770 | 1.10x |
| wide_arrays.json | msgspec | 3.261 | 3.293 | 3.410 | 74.770 | 0.74x |
| wide_arrays.json | ujson | 8.727 | 8.780 | 9.164 | 74.770 | 0.28x |
| wide_arrays.json | json | 16.486 | 16.970 | 62.768 | 74.770 | 0.14x |
| mixed.json | strata | 0.190 | 0.244 | 0.407 | 74.832 | 1.00x |
| mixed.json | orjson | 0.215 | 0.265 | 33.401 | 74.832 | 0.92x |
| mixed.json | msgspec | 0.195 | 0.242 | 0.382 | 74.832 | 1.01x |
| mixed.json | ujson | 0.437 | 0.497 | 0.946 | 74.832 | 0.49x |
| mixed.json | json | 0.679 | 0.761 | 1.072 | 74.832 | 0.32x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.107 | 0.113 | 0.128 | 68.820 | 1.00x |
| users.json $[*].id | jmespath | 0.508 | 0.529 | 0.536 | 68.820 | 0.21x |
| users.json $[*].id | jsonpath-ng | 2.531 | 2.648 | 2.778 | 68.820 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.575 | 0.591 | 0.640 | 69.785 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.007 | 3.125 | 3.203 | 69.785 | 0.19x |
| users.json $[*].orders[*].total | jsonpath-ng | 18.248 | 18.916 | 20.378 | 69.785 | 0.03x |
| users.json $..total | strata | 1.925 | 1.940 | 2.085 | 76.215 | 1.00x |
| users.json $..total | jsonpath-ng | 332.430 | 334.024 | 340.271 | 76.215 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 6.165 | 6.240 | 6.280 | 69.785 | 1.00x |
| users.json $[*].id | orjson+jmespath | 16.972 | 17.433 | 17.728 | 69.785 | 0.36x |
| users.json $[*].id | orjson+jsonpath-ng | 19.068 | 19.311 | 20.091 | 69.785 | 0.32x |
| users.json $[*].orders[*].total | strata | 6.365 | 6.490 | 6.610 | 73.969 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 20.274 | 20.445 | 20.967 | 73.969 | 0.32x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 42.285 | 42.447 | 43.173 | 73.969 | 0.15x |
| users.json $..total | strata | 20.439 | 20.646 | 21.014 | 75.500 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 362.158 | 364.461 | 375.567 | 75.500 | 0.06x |

