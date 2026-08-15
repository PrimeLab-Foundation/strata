# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 6520b15
- python: 3.12.10
- implementation: CPython
- platform: macOS-15.7.7-x86_64-i386-64bit
- machine: x86_64
- processor: Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 22.009 | 22.843 | 25.959 | 52.582 | 1.00x |
| users.json | orjson | 27.823 | 29.636 | 36.657 | 52.582 | 0.77x |
| users.json | msgspec | 27.901 | 30.872 | 32.795 | 52.582 | 0.74x |
| users.json | ujson | 42.512 | 44.720 | 48.519 | 52.582 | 0.51x |
| users.json | pysimdjson | 184.702 | 188.768 | 194.837 | 52.582 | 0.12x |
| users.json | json | 48.359 | 50.067 | 58.620 | 52.582 | 0.46x |
| flat.json | strata | 1.491 | 1.562 | 1.702 | 59.188 | 1.00x |
| flat.json | orjson | 1.561 | 1.627 | 1.693 | 59.188 | 0.96x |
| flat.json | msgspec | 1.767 | 1.828 | 2.133 | 59.188 | 0.85x |
| flat.json | ujson | 3.123 | 3.185 | 4.362 | 59.188 | 0.49x |
| flat.json | pysimdjson | 16.847 | 17.340 | 17.888 | 59.188 | 0.09x |
| flat.json | json | 3.537 | 3.597 | 4.281 | 59.188 | 0.43x |
| nested.json | strata | 1.639 | 1.737 | 1.913 | 55.148 | 1.00x |
| nested.json | orjson | 1.803 | 1.909 | 1.934 | 55.148 | 0.91x |
| nested.json | msgspec | 1.997 | 2.121 | 2.579 | 55.148 | 0.82x |
| nested.json | ujson | 3.357 | 3.499 | 3.877 | 55.148 | 0.50x |
| nested.json | pysimdjson | 15.099 | 15.232 | 15.712 | 55.148 | 0.11x |
| nested.json | json | 4.284 | 4.426 | 4.998 | 55.148 | 0.39x |
| wide_arrays.json | strata | 9.550 | 9.758 | 10.940 | 60.566 | 1.00x |
| wide_arrays.json | orjson | 9.864 | 10.408 | 10.967 | 60.566 | 0.94x |
| wide_arrays.json | msgspec | 10.955 | 11.566 | 12.100 | 60.566 | 0.84x |
| wide_arrays.json | ujson | 14.271 | 15.143 | 15.581 | 60.566 | 0.64x |
| wide_arrays.json | pysimdjson | 89.316 | 90.441 | 92.707 | 60.566 | 0.11x |
| wide_arrays.json | json | 18.575 | 19.642 | 20.038 | 60.566 | 0.50x |
| mixed.json | strata | 0.445 | 0.512 | 0.600 | 57.914 | 1.00x |
| mixed.json | orjson | 0.508 | 0.593 | 0.787 | 57.914 | 0.86x |
| mixed.json | msgspec | 0.539 | 0.561 | 0.746 | 57.914 | 0.91x |
| mixed.json | ujson | 0.749 | 0.794 | 1.137 | 57.914 | 0.64x |
| mixed.json | pysimdjson | 3.796 | 3.937 | 4.291 | 57.914 | 0.13x |
| mixed.json | json | 1.048 | 1.185 | 1.327 | 57.914 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.205 | 3.566 | 4.218 | 51.980 | 1.00x |
| users.json | orjson | 3.674 | 4.302 | 4.790 | 51.980 | 0.83x |
| users.json | msgspec | 5.706 | 6.102 | 7.442 | 51.980 | 0.58x |
| users.json | ujson | 35.734 | 37.242 | 40.473 | 51.980 | 0.10x |
| users.json | json | 47.320 | 49.152 | 51.557 | 51.980 | 0.07x |
| flat.json | strata | 0.343 | 0.370 | 0.418 | 54.363 | 1.00x |
| flat.json | orjson | 0.438 | 0.465 | 0.731 | 54.363 | 0.80x |
| flat.json | msgspec | 0.583 | 0.628 | 0.810 | 54.363 | 0.59x |
| flat.json | ujson | 3.047 | 3.093 | 3.510 | 54.363 | 0.12x |
| flat.json | json | 4.152 | 4.205 | 4.753 | 54.363 | 0.09x |
| nested.json | strata | 0.279 | 0.297 | 0.327 | 55.328 | 1.00x |
| nested.json | orjson | 0.377 | 0.402 | 0.408 | 55.328 | 0.74x |
| nested.json | msgspec | 0.602 | 0.642 | 0.887 | 55.328 | 0.46x |
| nested.json | ujson | 3.135 | 3.171 | 3.483 | 55.328 | 0.09x |
| nested.json | json | 5.155 | 5.226 | 5.523 | 55.328 | 0.06x |
| wide_arrays.json | strata | 1.960 | 2.163 | 2.221 | 58.176 | 1.00x |
| wide_arrays.json | orjson | 2.416 | 2.616 | 3.108 | 58.176 | 0.83x |
| wide_arrays.json | msgspec | 3.528 | 3.724 | 4.039 | 58.176 | 0.58x |
| wide_arrays.json | ujson | 17.008 | 17.620 | 18.073 | 58.176 | 0.12x |
| wide_arrays.json | json | 38.122 | 39.288 | 42.878 | 58.176 | 0.06x |
| mixed.json | strata | 0.109 | 0.119 | 0.181 | 53.535 | 1.00x |
| mixed.json | orjson | 0.099 | 0.103 | 0.143 | 53.535 | 1.16x |
| mixed.json | msgspec | 0.139 | 0.143 | 0.208 | 53.535 | 0.83x |
| mixed.json | ujson | 0.704 | 0.744 | 1.014 | 53.535 | 0.16x |
| mixed.json | json | 1.092 | 1.119 | 1.360 | 53.535 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 22.964 | 23.934 | 26.240 | 61.836 | 1.00x |
| users.json | orjson | 28.469 | 32.196 | 36.822 | 61.836 | 0.74x |
| users.json | msgspec | 29.817 | 32.341 | 35.285 | 61.836 | 0.74x |
| users.json | ujson | 44.385 | 47.499 | 49.849 | 61.836 | 0.50x |
| users.json | json | 48.945 | 53.040 | 56.656 | 61.836 | 0.45x |
| flat.json | strata | 1.644 | 1.705 | 1.971 | 55.020 | 1.00x |
| flat.json | orjson | 1.704 | 1.798 | 1.835 | 55.020 | 0.95x |
| flat.json | msgspec | 1.937 | 2.011 | 2.450 | 55.020 | 0.85x |
| flat.json | ujson | 3.276 | 3.407 | 3.773 | 55.020 | 0.50x |
| flat.json | json | 3.662 | 3.720 | 4.121 | 55.020 | 0.46x |
| nested.json | strata | 1.764 | 1.855 | 2.168 | 55.328 | 1.00x |
| nested.json | orjson | 1.991 | 2.082 | 2.428 | 55.328 | 0.89x |
| nested.json | msgspec | 2.204 | 2.315 | 2.732 | 55.328 | 0.80x |
| nested.json | ujson | 3.571 | 3.712 | 4.797 | 55.328 | 0.50x |
| nested.json | json | 4.458 | 4.687 | 5.307 | 55.328 | 0.40x |
| wide_arrays.json | strata | 9.736 | 10.259 | 11.521 | 58.176 | 1.00x |
| wide_arrays.json | orjson | 10.246 | 10.854 | 12.406 | 58.176 | 0.95x |
| wide_arrays.json | msgspec | 11.600 | 12.262 | 13.043 | 58.176 | 0.84x |
| wide_arrays.json | ujson | 15.084 | 15.893 | 16.637 | 58.176 | 0.65x |
| wide_arrays.json | json | 18.981 | 20.339 | 21.669 | 58.176 | 0.50x |
| mixed.json | strata | 0.561 | 0.574 | 0.620 | 53.535 | 1.00x |
| mixed.json | orjson | 0.678 | 0.697 | 0.777 | 53.535 | 0.82x |
| mixed.json | msgspec | 0.711 | 0.761 | 0.953 | 53.535 | 0.75x |
| mixed.json | ujson | 0.941 | 0.959 | 1.366 | 53.535 | 0.60x |
| mixed.json | json | 1.185 | 1.230 | 1.319 | 53.535 | 0.47x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 24.198 | 24.871 | 27.117 | 58.227 | 1.00x |
| users.ndjson | orjson | 32.604 | 33.234 | 35.683 | 58.227 | 0.75x |
| users.ndjson | msgspec | 31.586 | 33.591 | 36.628 | 58.227 | 0.74x |
| users.ndjson | ujson | 46.092 | 47.785 | 52.815 | 58.227 | 0.52x |
| users.ndjson | json | 58.641 | 60.498 | 65.628 | 58.227 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.285 | 4.603 | 5.125 | 59.930 | 1.00x |
| users.json | orjson | 4.801 | 5.061 | 6.238 | 59.930 | 0.91x |
| users.json | msgspec | 6.562 | 6.946 | 7.672 | 59.930 | 0.66x |
| users.json | ujson | 37.058 | 38.089 | 42.893 | 59.930 | 0.12x |
| users.json | json | 48.125 | 49.181 | 50.132 | 59.930 | 0.09x |
| flat.json | strata | 0.672 | 0.760 | 0.909 | 55.020 | 1.00x |
| flat.json | orjson | 0.812 | 0.886 | 0.960 | 55.020 | 0.86x |
| flat.json | msgspec | 0.938 | 1.058 | 1.418 | 55.020 | 0.72x |
| flat.json | ujson | 3.397 | 3.533 | 4.104 | 55.020 | 0.22x |
| flat.json | json | 4.408 | 4.552 | 5.191 | 55.020 | 0.17x |
| nested.json | strata | 0.597 | 0.655 | 0.743 | 55.328 | 1.00x |
| nested.json | orjson | 0.731 | 0.823 | 1.238 | 55.328 | 0.80x |
| nested.json | msgspec | 0.920 | 1.070 | 1.124 | 55.328 | 0.61x |
| nested.json | ujson | 3.489 | 3.795 | 4.216 | 55.328 | 0.17x |
| nested.json | json | 5.564 | 5.715 | 6.416 | 55.328 | 0.11x |
| wide_arrays.json | strata | 2.647 | 2.880 | 3.675 | 58.176 | 1.00x |
| wide_arrays.json | orjson | 3.106 | 3.621 | 4.195 | 58.176 | 0.80x |
| wide_arrays.json | msgspec | 4.249 | 4.655 | 6.052 | 58.176 | 0.62x |
| wide_arrays.json | ujson | 18.329 | 18.762 | 23.593 | 58.176 | 0.15x |
| wide_arrays.json | json | 39.709 | 40.553 | 48.701 | 58.176 | 0.07x |
| mixed.json | strata | 0.393 | 0.477 | 0.568 | 53.535 | 1.00x |
| mixed.json | orjson | 0.397 | 0.515 | 0.641 | 53.535 | 0.93x |
| mixed.json | msgspec | 0.424 | 0.533 | 0.716 | 53.535 | 0.89x |
| mixed.json | ujson | 1.044 | 1.157 | 1.282 | 53.535 | 0.41x |
| mixed.json | json | 1.456 | 1.543 | 2.013 | 53.535 | 0.31x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.146 | 0.186 | 0.221 | 60.020 | 1.00x |
| users.json $[*].id | jmespath | 1.044 | 1.111 | 1.586 | 60.020 | 0.17x |
| users.json $[*].id | jsonpath-ng | 5.813 | 6.096 | 6.472 | 60.020 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.964 | 1.114 | 1.131 | 56.441 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 6.353 | 6.794 | 7.234 | 56.441 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 37.636 | 40.592 | 41.498 | 56.441 | 0.03x |
| users.json $..total | strata | 3.631 | 4.038 | 4.330 | 57.512 | 1.00x |
| users.json $..total | jsonpath-ng | 796.342 | 807.009 | 852.452 | 57.512 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.531 | 4.693 | 5.251 | 60.082 | 1.00x |
| users.json $[*].id | orjson+jmespath | 28.929 | 31.150 | 32.801 | 60.082 | 0.15x |
| users.json $[*].id | orjson+jsonpath-ng | 33.342 | 36.050 | 37.487 | 60.082 | 0.13x |
| users.json $[*].orders[*].total | strata | 4.763 | 4.840 | 5.030 | 56.676 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 33.409 | 36.858 | 47.430 | 56.676 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 71.164 | 76.504 | 84.892 | 56.676 | 0.06x |
| users.json $..total | strata | 26.231 | 27.693 | 29.716 | 56.773 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 822.886 | 840.096 | 876.013 | 56.773 | 0.03x |

