# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: a4f3b0e
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: x86_64
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.701 | 11.403 | 18.568 | 59.020 | 1.00x |
| users.json | orjson | 14.337 | 14.883 | 21.972 | 59.020 | 0.77x |
| users.json | msgspec | 13.998 | 14.527 | 29.360 | 59.020 | 0.78x |
| users.json | ujson | 19.046 | 20.901 | 43.481 | 59.020 | 0.55x |
| users.json | pysimdjson | 20.432 | 22.785 | 45.379 | 59.020 | 0.50x |
| users.json | json | 21.910 | 22.463 | 33.141 | 59.020 | 0.51x |
| flat.json | strata | 0.918 | 0.927 | 0.948 | 70.953 | 1.00x |
| flat.json | orjson | 1.078 | 1.092 | 1.126 | 70.953 | 0.85x |
| flat.json | msgspec | 1.053 | 1.073 | 1.099 | 70.953 | 0.86x |
| flat.json | ujson | 1.631 | 1.701 | 1.781 | 70.953 | 0.55x |
| flat.json | pysimdjson | 1.648 | 1.678 | 1.793 | 70.953 | 0.55x |
| flat.json | json | 1.725 | 1.750 | 1.783 | 70.953 | 0.53x |
| nested.json | strata | 0.810 | 0.827 | 0.891 | 70.953 | 1.00x |
| nested.json | orjson | 1.000 | 1.019 | 1.061 | 70.953 | 0.81x |
| nested.json | msgspec | 0.961 | 0.992 | 1.743 | 70.953 | 0.83x |
| nested.json | ujson | 1.496 | 1.641 | 2.188 | 70.953 | 0.50x |
| nested.json | pysimdjson | 1.395 | 1.494 | 2.091 | 70.953 | 0.55x |
| nested.json | json | 1.826 | 1.867 | 2.205 | 70.953 | 0.44x |
| wide_arrays.json | strata | 5.030 | 5.104 | 6.081 | 74.559 | 1.00x |
| wide_arrays.json | orjson | 5.597 | 5.819 | 6.232 | 74.559 | 0.88x |
| wide_arrays.json | msgspec | 6.093 | 6.242 | 6.355 | 74.559 | 0.82x |
| wide_arrays.json | ujson | 7.781 | 7.957 | 8.231 | 74.559 | 0.64x |
| wide_arrays.json | pysimdjson | 6.490 | 6.695 | 7.294 | 74.559 | 0.76x |
| wide_arrays.json | json | 9.825 | 10.043 | 10.980 | 74.559 | 0.51x |
| mixed.json | strata | 0.199 | 0.204 | 0.385 | 74.621 | 1.00x |
| mixed.json | orjson | 0.239 | 0.259 | 0.274 | 74.621 | 0.79x |
| mixed.json | msgspec | 0.246 | 0.253 | 0.297 | 74.621 | 0.81x |
| mixed.json | ujson | 0.325 | 0.345 | 0.460 | 74.621 | 0.59x |
| mixed.json | pysimdjson | 0.309 | 0.317 | 0.339 | 74.621 | 0.65x |
| mixed.json | json | 0.450 | 0.471 | 0.773 | 74.621 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.521 | 2.560 | 2.631 | 58.133 | 1.00x |
| users.json | orjson | 3.090 | 3.136 | 3.297 | 58.133 | 0.82x |
| users.json | msgspec | 4.162 | 4.219 | 4.324 | 58.133 | 0.61x |
| users.json | ujson | 14.579 | 14.953 | 15.202 | 58.133 | 0.17x |
| users.json | json | 21.365 | 21.690 | 23.444 | 58.133 | 0.12x |
| flat.json | strata | 0.302 | 0.310 | 0.325 | 70.953 | 1.00x |
| flat.json | orjson | 0.359 | 0.372 | 0.378 | 70.953 | 0.83x |
| flat.json | msgspec | 0.480 | 0.490 | 0.610 | 70.953 | 0.63x |
| flat.json | ujson | 1.281 | 1.291 | 1.305 | 70.953 | 0.24x |
| flat.json | json | 1.861 | 1.876 | 1.889 | 70.953 | 0.17x |
| nested.json | strata | 0.240 | 0.247 | 0.267 | 70.953 | 1.00x |
| nested.json | orjson | 0.300 | 0.310 | 0.327 | 70.953 | 0.80x |
| nested.json | msgspec | 0.419 | 0.427 | 0.442 | 70.953 | 0.58x |
| nested.json | ujson | 1.356 | 1.365 | 1.387 | 70.953 | 0.18x |
| nested.json | json | 2.304 | 2.334 | 2.408 | 70.953 | 0.11x |
| wide_arrays.json | strata | 1.781 | 1.862 | 2.638 | 74.559 | 1.00x |
| wide_arrays.json | orjson | 1.909 | 1.932 | 2.556 | 74.559 | 0.96x |
| wide_arrays.json | msgspec | 2.979 | 2.994 | 3.682 | 74.559 | 0.62x |
| wide_arrays.json | ujson | 8.592 | 8.646 | 8.973 | 74.559 | 0.22x |
| wide_arrays.json | json | 16.590 | 16.804 | 17.811 | 74.559 | 0.11x |
| mixed.json | strata | 0.065 | 0.068 | 0.070 | 74.621 | 1.00x |
| mixed.json | orjson | 0.069 | 0.072 | 0.083 | 74.621 | 0.94x |
| mixed.json | msgspec | 0.087 | 0.089 | 0.103 | 74.621 | 0.76x |
| mixed.json | ujson | 0.307 | 0.311 | 0.330 | 74.621 | 0.22x |
| mixed.json | json | 0.514 | 0.530 | 0.546 | 74.621 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.911 | 13.397 | 14.753 | 74.406 | 1.00x |
| users.json | orjson | 15.047 | 16.094 | 16.798 | 74.406 | 0.83x |
| users.json | msgspec | 15.317 | 15.806 | 17.470 | 74.406 | 0.85x |
| users.json | ujson | 21.218 | 23.487 | 24.543 | 74.406 | 0.57x |
| users.json | json | 22.876 | 23.547 | 25.488 | 74.406 | 0.57x |
| flat.json | strata | 0.956 | 0.971 | 1.209 | 70.953 | 1.00x |
| flat.json | orjson | 1.140 | 1.150 | 1.258 | 70.953 | 0.84x |
| flat.json | msgspec | 1.108 | 1.123 | 1.184 | 70.953 | 0.86x |
| flat.json | ujson | 1.706 | 1.759 | 1.785 | 70.953 | 0.55x |
| flat.json | json | 1.776 | 1.792 | 1.817 | 70.953 | 0.54x |
| nested.json | strata | 0.843 | 0.861 | 0.939 | 70.953 | 1.00x |
| nested.json | orjson | 1.078 | 1.085 | 1.283 | 70.953 | 0.79x |
| nested.json | msgspec | 1.025 | 1.059 | 1.077 | 70.953 | 0.81x |
| nested.json | ujson | 1.484 | 1.529 | 1.600 | 70.953 | 0.56x |
| nested.json | json | 1.891 | 1.928 | 2.186 | 70.953 | 0.45x |
| wide_arrays.json | strata | 5.229 | 5.347 | 6.144 | 74.621 | 1.00x |
| wide_arrays.json | orjson | 5.912 | 6.122 | 7.475 | 74.621 | 0.87x |
| wide_arrays.json | msgspec | 6.433 | 6.724 | 7.459 | 74.621 | 0.80x |
| wide_arrays.json | ujson | 8.221 | 8.427 | 9.101 | 74.621 | 0.63x |
| wide_arrays.json | json | 10.145 | 10.428 | 10.769 | 74.621 | 0.51x |
| mixed.json | strata | 0.227 | 0.240 | 0.257 | 74.621 | 1.00x |
| mixed.json | orjson | 0.297 | 0.314 | 0.372 | 74.621 | 0.77x |
| mixed.json | msgspec | 0.296 | 0.314 | 0.334 | 74.621 | 0.76x |
| mixed.json | ujson | 0.377 | 0.401 | 0.422 | 74.621 | 0.60x |
| mixed.json | json | 0.508 | 0.521 | 0.546 | 74.621 | 0.46x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 13.292 | 14.654 | 15.795 | 70.953 | 1.00x |
| users.ndjson | orjson | 19.587 | 21.086 | 21.602 | 70.953 | 0.69x |
| users.ndjson | msgspec | 20.377 | 20.776 | 21.475 | 70.953 | 0.71x |
| users.ndjson | ujson | 26.572 | 27.435 | 29.619 | 70.953 | 0.53x |
| users.ndjson | json | 32.437 | 33.300 | 35.954 | 70.953 | 0.44x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.175 | 3.300 | 3.472 | 72.438 | 1.00x |
| users.json | orjson | 3.882 | 3.922 | 4.382 | 72.438 | 0.84x |
| users.json | msgspec | 4.883 | 4.967 | 7.222 | 72.438 | 0.66x |
| users.json | ujson | 15.647 | 16.040 | 16.542 | 72.438 | 0.21x |
| users.json | json | 22.805 | 23.380 | 23.857 | 72.438 | 0.14x |
| flat.json | strata | 0.472 | 0.492 | 0.540 | 70.953 | 1.00x |
| flat.json | orjson | 0.550 | 0.569 | 0.632 | 70.953 | 0.86x |
| flat.json | msgspec | 0.665 | 0.676 | 0.752 | 70.953 | 0.73x |
| flat.json | ujson | 1.473 | 1.490 | 1.517 | 70.953 | 0.33x |
| flat.json | json | 2.095 | 2.121 | 2.163 | 70.953 | 0.23x |
| nested.json | strata | 0.372 | 0.384 | 0.415 | 70.953 | 1.00x |
| nested.json | orjson | 0.454 | 0.463 | 0.496 | 70.953 | 0.83x |
| nested.json | msgspec | 0.566 | 0.591 | 0.686 | 70.953 | 0.65x |
| nested.json | ujson | 1.511 | 1.545 | 1.652 | 70.953 | 0.25x |
| nested.json | json | 2.498 | 2.525 | 2.577 | 70.953 | 0.15x |
| wide_arrays.json | strata | 2.243 | 2.281 | 2.418 | 74.621 | 1.00x |
| wide_arrays.json | orjson | 2.385 | 2.431 | 2.496 | 74.621 | 0.94x |
| wide_arrays.json | msgspec | 3.445 | 3.472 | 3.532 | 74.621 | 0.66x |
| wide_arrays.json | ujson | 9.144 | 9.199 | 9.351 | 74.621 | 0.25x |
| wide_arrays.json | json | 17.261 | 17.444 | 17.829 | 74.621 | 0.13x |
| mixed.json | strata | 0.168 | 0.180 | 0.247 | 74.621 | 1.00x |
| mixed.json | orjson | 0.192 | 0.203 | 0.236 | 74.621 | 0.89x |
| mixed.json | msgspec | 0.204 | 0.214 | 0.241 | 74.621 | 0.84x |
| mixed.json | ujson | 0.438 | 0.463 | 0.497 | 74.621 | 0.39x |
| mixed.json | json | 0.649 | 0.672 | 0.727 | 74.621 | 0.27x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.078 | 0.084 | 0.091 | 72.438 | 1.00x |
| users.json $[*].id | jmespath | 0.474 | 0.489 | 0.622 | 72.438 | 0.17x |
| users.json $[*].id | jsonpath-ng | 3.101 | 3.204 | 3.281 | 72.438 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.451 | 0.514 | 0.632 | 72.449 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.141 | 3.299 | 3.997 | 72.449 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 21.330 | 22.895 | 23.665 | 72.449 | 0.02x |
| users.json $..total | strata | 1.836 | 1.997 | 2.297 | 73.449 | 1.00x |
| users.json $..total | jsonpath-ng | 388.982 | 391.876 | 395.747 | 73.449 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.271 | 3.294 | 3.328 | 72.449 | 1.00x |
| users.json $[*].id | orjson+jmespath | 16.269 | 16.519 | 17.746 | 72.449 | 0.20x |
| users.json $[*].id | orjson+jsonpath-ng | 18.962 | 20.568 | 22.196 | 72.449 | 0.16x |
| users.json $[*].orders[*].total | strata | 3.551 | 3.592 | 3.664 | 73.449 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.597 | 19.247 | 21.283 | 73.449 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 41.677 | 43.613 | 50.437 | 73.449 | 0.08x |
| users.json $..total | strata | 14.930 | 18.277 | 19.075 | 73.566 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 419.864 | 425.726 | 440.131 | 73.566 | 0.04x |

