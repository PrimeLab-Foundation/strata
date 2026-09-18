# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: baba505678994faef24df367a2dc4b93d5d4bef0
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
| users.json | strata | 27.013 | 30.703 | 34.867 | 57.191 | 1.00x |
| users.json | orjson | 38.082 | 46.218 | 73.609 | 57.191 | 0.66x |
| users.json | msgspec | 41.647 | 46.325 | 52.556 | 57.191 | 0.66x |
| users.json | ujson | 59.210 | 63.905 | 85.513 | 57.191 | 0.48x |
| users.json | pysimdjson | 236.189 | 246.424 | 314.084 | 57.191 | 0.12x |
| users.json | json | 63.238 | 73.103 | 130.915 | 57.191 | 0.42x |
| flat.json | strata | 1.470 | 2.200 | 2.518 | 67.938 | 1.00x |
| flat.json | orjson | 1.623 | 2.225 | 2.587 | 67.938 | 0.99x |
| flat.json | msgspec | 1.859 | 2.551 | 2.855 | 67.938 | 0.86x |
| flat.json | ujson | 3.888 | 4.682 | 5.544 | 67.938 | 0.47x |
| flat.json | pysimdjson | 19.658 | 21.349 | 22.782 | 67.938 | 0.10x |
| flat.json | json | 3.876 | 5.173 | 5.435 | 67.938 | 0.43x |
| nested.json | strata | 1.927 | 2.579 | 2.847 | 66.418 | 1.00x |
| nested.json | orjson | 2.078 | 3.031 | 3.317 | 66.418 | 0.85x |
| nested.json | msgspec | 2.413 | 3.258 | 3.621 | 66.418 | 0.79x |
| nested.json | ujson | 4.132 | 5.180 | 5.382 | 66.418 | 0.50x |
| nested.json | pysimdjson | 19.582 | 20.886 | 21.731 | 66.418 | 0.12x |
| nested.json | json | 5.091 | 6.567 | 7.338 | 66.418 | 0.39x |
| wide_arrays.json | strata | 9.691 | 11.781 | 13.130 | 72.379 | 1.00x |
| wide_arrays.json | orjson | 14.514 | 17.232 | 19.244 | 72.379 | 0.68x |
| wide_arrays.json | msgspec | 14.182 | 17.571 | 20.084 | 72.379 | 0.67x |
| wide_arrays.json | ujson | 19.113 | 22.317 | 26.667 | 72.379 | 0.53x |
| wide_arrays.json | pysimdjson | 113.542 | 119.181 | 184.336 | 72.379 | 0.10x |
| wide_arrays.json | json | 22.960 | 27.893 | 29.558 | 72.379 | 0.42x |
| mixed.json | strata | 0.451 | 0.547 | 0.773 | 65.176 | 1.00x |
| mixed.json | orjson | 0.583 | 0.804 | 0.841 | 65.176 | 0.68x |
| mixed.json | msgspec | 0.601 | 0.862 | 0.903 | 65.176 | 0.63x |
| mixed.json | ujson | 0.802 | 1.160 | 1.328 | 65.176 | 0.47x |
| mixed.json | pysimdjson | 3.861 | 4.788 | 5.542 | 65.176 | 0.11x |
| mixed.json | json | 1.335 | 1.508 | 1.670 | 65.176 | 0.36x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.418 | 4.731 | 5.441 | 52.441 | 1.00x |
| users.json | orjson | 4.813 | 6.520 | 6.962 | 52.441 | 0.73x |
| users.json | msgspec | 6.578 | 9.054 | 9.927 | 52.441 | 0.52x |
| users.json | ujson | 34.911 | 38.169 | 42.395 | 52.441 | 0.12x |
| users.json | json | 58.847 | 68.992 | 73.462 | 52.441 | 0.07x |
| flat.json | strata | 0.412 | 0.563 | 0.590 | 66.453 | 1.00x |
| flat.json | orjson | 0.501 | 0.717 | 0.826 | 66.453 | 0.79x |
| flat.json | msgspec | 0.671 | 0.917 | 0.986 | 66.453 | 0.61x |
| flat.json | ujson | 2.674 | 2.944 | 4.037 | 66.453 | 0.19x |
| flat.json | json | 4.492 | 5.705 | 6.443 | 66.453 | 0.10x |
| nested.json | strata | 0.456 | 0.558 | 0.678 | 66.547 | 1.00x |
| nested.json | orjson | 0.510 | 0.786 | 0.832 | 66.547 | 0.71x |
| nested.json | msgspec | 0.788 | 1.001 | 1.167 | 66.547 | 0.56x |
| nested.json | ujson | 2.919 | 4.028 | 4.349 | 66.547 | 0.14x |
| nested.json | json | 7.299 | 8.113 | 8.535 | 66.547 | 0.07x |
| wide_arrays.json | strata | 2.665 | 3.303 | 3.888 | 66.105 | 1.00x |
| wide_arrays.json | orjson | 3.354 | 4.361 | 4.822 | 66.105 | 0.76x |
| wide_arrays.json | msgspec | 4.425 | 5.607 | 6.913 | 66.105 | 0.59x |
| wide_arrays.json | ujson | 13.110 | 17.319 | 19.676 | 66.105 | 0.19x |
| wide_arrays.json | json | 52.655 | 55.598 | 57.284 | 66.105 | 0.06x |
| mixed.json | strata | 0.139 | 0.158 | 0.232 | 61.977 | 1.00x |
| mixed.json | orjson | 0.118 | 0.181 | 0.236 | 61.977 | 0.87x |
| mixed.json | msgspec | 0.166 | 0.242 | 0.388 | 61.977 | 0.65x |
| mixed.json | ujson | 0.692 | 0.837 | 0.909 | 61.977 | 0.19x |
| mixed.json | json | 1.228 | 1.574 | 1.850 | 61.977 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 25.961 | 29.720 | 58.235 | 63.223 | 1.00x |
| users.json | orjson | 36.478 | 44.670 | 270.638 | 63.223 | 0.67x |
| users.json | msgspec | 37.851 | 42.144 | 58.499 | 63.223 | 0.71x |
| users.json | ujson | 56.398 | 67.387 | 195.065 | 63.223 | 0.44x |
| users.json | json | 64.340 | 69.950 | 147.694 | 63.223 | 0.42x |
| flat.json | strata | 1.672 | 2.324 | 2.557 | 66.453 | 1.00x |
| flat.json | orjson | 2.070 | 2.731 | 3.448 | 66.453 | 0.85x |
| flat.json | msgspec | 2.288 | 3.037 | 3.240 | 66.453 | 0.77x |
| flat.json | ujson | 3.537 | 5.099 | 5.393 | 66.453 | 0.46x |
| flat.json | json | 4.120 | 5.063 | 6.296 | 66.453 | 0.46x |
| nested.json | strata | 1.888 | 2.738 | 2.963 | 66.547 | 1.00x |
| nested.json | orjson | 2.598 | 3.053 | 3.333 | 66.547 | 0.90x |
| nested.json | msgspec | 3.022 | 3.515 | 4.061 | 66.547 | 0.78x |
| nested.json | ujson | 5.000 | 5.625 | 6.281 | 66.547 | 0.49x |
| nested.json | json | 5.551 | 6.793 | 7.339 | 66.547 | 0.40x |
| wide_arrays.json | strata | 11.883 | 12.641 | 13.739 | 67.336 | 1.00x |
| wide_arrays.json | orjson | 11.542 | 16.778 | 25.753 | 67.336 | 0.75x |
| wide_arrays.json | msgspec | 13.775 | 17.317 | 18.212 | 67.336 | 0.73x |
| wide_arrays.json | ujson | 18.463 | 23.201 | 25.329 | 67.336 | 0.54x |
| wide_arrays.json | json | 22.356 | 27.759 | 32.176 | 67.336 | 0.46x |
| mixed.json | strata | 0.647 | 0.735 | 0.822 | 61.977 | 1.00x |
| mixed.json | orjson | 0.729 | 0.949 | 1.053 | 61.977 | 0.77x |
| mixed.json | msgspec | 0.853 | 1.049 | 1.122 | 61.977 | 0.70x |
| mixed.json | ujson | 0.957 | 1.378 | 1.519 | 61.977 | 0.53x |
| mixed.json | json | 1.253 | 1.832 | 2.089 | 61.977 | 0.40x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 21.939 | 28.733 | 31.026 | 67.516 | 1.00x |
| users.ndjson | orjson | 38.253 | 45.250 | 50.823 | 67.516 | 0.63x |
| users.ndjson | msgspec | 37.494 | 44.227 | 50.190 | 67.516 | 0.65x |
| users.ndjson | ujson | 49.113 | 64.178 | 69.215 | 67.516 | 0.45x |
| users.ndjson | json | 68.722 | 76.372 | 89.143 | 67.516 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.313 | 5.612 | 9.862 | 63.250 | 1.00x |
| users.json | orjson | 6.181 | 7.072 | 8.796 | 63.250 | 0.79x |
| users.json | msgspec | 7.906 | 9.331 | 12.535 | 63.250 | 0.60x |
| users.json | ujson | 34.299 | 38.669 | 41.251 | 63.250 | 0.15x |
| users.json | json | 55.314 | 61.649 | 72.244 | 63.250 | 0.09x |
| flat.json | strata | 0.816 | 0.997 | 1.309 | 66.453 | 1.00x |
| flat.json | orjson | 0.994 | 1.034 | 1.577 | 66.453 | 0.96x |
| flat.json | msgspec | 1.128 | 1.199 | 1.663 | 66.453 | 0.83x |
| flat.json | ujson | 3.192 | 3.501 | 4.540 | 66.453 | 0.28x |
| flat.json | json | 5.086 | 5.665 | 7.555 | 66.453 | 0.18x |
| nested.json | strata | 0.781 | 1.067 | 1.190 | 66.547 | 1.00x |
| nested.json | orjson | 1.068 | 1.381 | 1.603 | 66.547 | 0.77x |
| nested.json | msgspec | 1.117 | 1.640 | 1.876 | 66.547 | 0.65x |
| nested.json | ujson | 3.222 | 4.648 | 4.922 | 66.547 | 0.23x |
| nested.json | json | 7.208 | 8.328 | 9.264 | 66.547 | 0.13x |
| wide_arrays.json | strata | 3.383 | 4.027 | 4.697 | 66.105 | 1.00x |
| wide_arrays.json | orjson | 4.333 | 5.228 | 6.832 | 66.105 | 0.77x |
| wide_arrays.json | msgspec | 5.326 | 6.952 | 8.152 | 66.105 | 0.58x |
| wide_arrays.json | ujson | 14.769 | 17.211 | 23.097 | 66.105 | 0.23x |
| wide_arrays.json | json | 42.117 | 45.556 | 62.994 | 66.105 | 0.09x |
| mixed.json | strata | 0.442 | 0.550 | 0.766 | 61.977 | 1.00x |
| mixed.json | orjson | 0.555 | 0.612 | 0.990 | 61.977 | 0.90x |
| mixed.json | msgspec | 0.583 | 0.646 | 0.884 | 61.977 | 0.85x |
| mixed.json | ujson | 1.032 | 1.229 | 1.600 | 61.977 | 0.45x |
| mixed.json | json | 1.584 | 1.774 | 2.447 | 61.977 | 0.31x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.243 | 0.348 | 0.590 | 63.316 | 1.00x |
| users.json $[*].id | jmespath | 1.376 | 1.669 | 2.240 | 63.316 | 0.21x |
| users.json $[*].id | jsonpath-ng | 7.682 | 9.246 | 11.266 | 63.316 | 0.04x |
| users.json $[*].orders[*].total | strata | 1.412 | 1.603 | 2.260 | 60.512 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 7.499 | 8.965 | 10.719 | 60.512 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 43.978 | 50.634 | 60.337 | 60.512 | 0.03x |
| users.json $..total | strata | 4.238 | 6.369 | 7.311 | 60.535 | 1.00x |
| users.json $..total | jsonpath-ng | 1036.336 | 1170.473 | 1641.617 | 60.535 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.365 | 5.202 | 5.890 | 63.395 | 1.00x |
| users.json $[*].id | orjson+jmespath | 36.659 | 41.381 | 47.905 | 63.395 | 0.13x |
| users.json $[*].id | orjson+jsonpath-ng | 40.177 | 47.989 | 53.206 | 63.395 | 0.11x |
| users.json $[*].orders[*].total | strata | 6.426 | 7.028 | 9.088 | 60.531 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 47.475 | 57.257 | 65.714 | 60.531 | 0.12x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 112.729 | 123.415 | 139.182 | 60.531 | 0.06x |
| users.json $..total | strata | 29.833 | 37.409 | 40.271 | 60.551 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 1162.129 | 1196.253 | 1383.852 | 60.551 | 0.03x |

