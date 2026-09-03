# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: fb7997e
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
| users.json | strata | 19.322 | 21.832 | 29.432 | 52.781 | 1.00x |
| users.json | orjson | 26.280 | 30.312 | 39.021 | 52.781 | 0.72x |
| users.json | msgspec | 26.634 | 29.064 | 40.563 | 52.781 | 0.75x |
| users.json | ujson | 38.805 | 45.093 | 113.426 | 52.781 | 0.48x |
| users.json | pysimdjson | 167.673 | 192.534 | 412.877 | 52.781 | 0.11x |
| users.json | json | 46.473 | 53.743 | 73.339 | 52.781 | 0.41x |
| flat.json | strata | 1.231 | 1.396 | 3.487 | 59.527 | 1.00x |
| flat.json | orjson | 1.342 | 1.558 | 3.528 | 59.527 | 0.90x |
| flat.json | msgspec | 1.576 | 1.628 | 4.948 | 59.527 | 0.86x |
| flat.json | ujson | 2.701 | 2.957 | 4.731 | 59.527 | 0.47x |
| flat.json | pysimdjson | 14.202 | 15.478 | 25.199 | 59.527 | 0.09x |
| flat.json | json | 3.123 | 3.546 | 5.079 | 59.527 | 0.39x |
| nested.json | strata | 1.425 | 1.494 | 1.701 | 57.488 | 1.00x |
| nested.json | orjson | 1.656 | 1.747 | 1.861 | 57.488 | 0.86x |
| nested.json | msgspec | 1.833 | 1.933 | 2.083 | 57.488 | 0.77x |
| nested.json | ujson | 2.991 | 3.069 | 3.356 | 57.488 | 0.49x |
| nested.json | pysimdjson | 12.878 | 13.315 | 14.490 | 57.488 | 0.11x |
| nested.json | json | 3.803 | 3.911 | 4.403 | 57.488 | 0.38x |
| wide_arrays.json | strata | 7.920 | 8.315 | 9.395 | 61.773 | 1.00x |
| wide_arrays.json | orjson | 8.474 | 9.312 | 10.480 | 61.773 | 0.89x |
| wide_arrays.json | msgspec | 9.558 | 10.109 | 11.323 | 61.773 | 0.82x |
| wide_arrays.json | ujson | 12.151 | 13.880 | 15.412 | 61.773 | 0.60x |
| wide_arrays.json | pysimdjson | 75.430 | 79.433 | 84.703 | 61.773 | 0.10x |
| wide_arrays.json | json | 15.784 | 17.173 | 19.800 | 61.773 | 0.48x |
| mixed.json | strata | 0.383 | 0.401 | 0.499 | 58.781 | 1.00x |
| mixed.json | orjson | 0.454 | 0.475 | 0.507 | 58.781 | 0.85x |
| mixed.json | msgspec | 0.489 | 0.499 | 0.565 | 58.781 | 0.80x |
| mixed.json | ujson | 0.672 | 0.720 | 0.836 | 58.781 | 0.56x |
| mixed.json | pysimdjson | 3.268 | 3.393 | 3.767 | 58.781 | 0.12x |
| mixed.json | json | 0.915 | 0.936 | 1.086 | 58.781 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.664 | 3.001 | 7.813 | 48.688 | 1.00x |
| users.json | orjson | 3.448 | 3.837 | 7.440 | 48.688 | 0.78x |
| users.json | msgspec | 6.289 | 8.774 | 40.223 | 48.688 | 0.34x |
| users.json | ujson | 32.211 | 36.079 | 43.822 | 48.688 | 0.08x |
| users.json | json | 41.982 | 58.003 | 187.016 | 48.688 | 0.05x |
| flat.json | strata | 0.301 | 0.335 | 0.562 | 56.652 | 1.00x |
| flat.json | orjson | 0.419 | 0.467 | 0.684 | 56.652 | 0.72x |
| flat.json | msgspec | 0.534 | 0.620 | 0.958 | 56.652 | 0.54x |
| flat.json | ujson | 2.579 | 2.804 | 4.691 | 56.652 | 0.12x |
| flat.json | json | 3.583 | 3.985 | 6.074 | 56.652 | 0.08x |
| nested.json | strata | 0.265 | 0.309 | 0.466 | 52.395 | 1.00x |
| nested.json | orjson | 0.368 | 0.445 | 0.579 | 52.395 | 0.69x |
| nested.json | msgspec | 0.576 | 0.659 | 0.844 | 52.395 | 0.47x |
| nested.json | ujson | 2.724 | 3.092 | 3.767 | 52.395 | 0.10x |
| nested.json | json | 4.795 | 5.394 | 6.064 | 52.395 | 0.06x |
| wide_arrays.json | strata | 1.669 | 1.719 | 2.007 | 60.293 | 1.00x |
| wide_arrays.json | orjson | 2.273 | 2.358 | 2.685 | 60.293 | 0.73x |
| wide_arrays.json | msgspec | 3.135 | 3.211 | 3.602 | 60.293 | 0.54x |
| wide_arrays.json | ujson | 14.859 | 15.148 | 18.333 | 60.293 | 0.11x |
| wide_arrays.json | json | 32.861 | 34.170 | 36.941 | 60.293 | 0.05x |
| mixed.json | strata | 0.074 | 0.080 | 0.093 | 55.461 | 1.00x |
| mixed.json | orjson | 0.088 | 0.093 | 0.104 | 55.461 | 0.86x |
| mixed.json | msgspec | 0.120 | 0.126 | 0.197 | 55.461 | 0.63x |
| mixed.json | ujson | 0.625 | 0.634 | 0.707 | 55.461 | 0.13x |
| mixed.json | json | 0.974 | 0.993 | 1.043 | 55.461 | 0.08x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 19.111 | 20.845 | 23.243 | 61.387 | 1.00x |
| users.json | orjson | 26.491 | 28.557 | 31.597 | 61.387 | 0.73x |
| users.json | msgspec | 27.542 | 29.235 | 37.256 | 61.387 | 0.71x |
| users.json | ujson | 38.584 | 41.681 | 114.534 | 61.387 | 0.50x |
| users.json | json | 42.558 | 45.949 | 56.983 | 61.387 | 0.45x |
| flat.json | strata | 1.326 | 1.362 | 1.609 | 57.352 | 1.00x |
| flat.json | orjson | 1.444 | 1.466 | 1.547 | 57.352 | 0.93x |
| flat.json | msgspec | 1.689 | 1.730 | 1.946 | 57.352 | 0.79x |
| flat.json | ujson | 2.802 | 2.871 | 3.439 | 57.352 | 0.47x |
| flat.json | json | 3.154 | 3.185 | 3.775 | 57.352 | 0.43x |
| nested.json | strata | 1.584 | 1.642 | 1.870 | 52.395 | 1.00x |
| nested.json | orjson | 1.808 | 1.933 | 2.406 | 52.395 | 0.85x |
| nested.json | msgspec | 2.031 | 2.178 | 2.675 | 52.395 | 0.75x |
| nested.json | ujson | 3.227 | 3.564 | 5.193 | 52.395 | 0.46x |
| nested.json | json | 4.017 | 4.206 | 5.447 | 52.395 | 0.39x |
| wide_arrays.json | strata | 8.621 | 8.745 | 8.974 | 60.293 | 1.00x |
| wide_arrays.json | orjson | 9.479 | 9.770 | 11.416 | 60.293 | 0.90x |
| wide_arrays.json | msgspec | 10.365 | 10.715 | 15.177 | 60.293 | 0.82x |
| wide_arrays.json | ujson | 13.818 | 14.026 | 16.152 | 60.293 | 0.62x |
| wide_arrays.json | json | 17.218 | 17.688 | 19.220 | 60.293 | 0.49x |
| mixed.json | strata | 0.484 | 0.491 | 0.499 | 55.461 | 1.00x |
| mixed.json | orjson | 0.600 | 0.612 | 0.708 | 55.461 | 0.80x |
| mixed.json | msgspec | 0.639 | 0.652 | 0.926 | 55.461 | 0.75x |
| mixed.json | ujson | 0.839 | 0.847 | 0.914 | 55.461 | 0.58x |
| mixed.json | json | 1.045 | 1.066 | 1.097 | 55.461 | 0.46x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 19.654 | 20.688 | 44.038 | 59.238 | 1.00x |
| users.ndjson | orjson | 27.143 | 29.875 | 82.407 | 59.238 | 0.69x |
| users.ndjson | msgspec | 27.266 | 29.105 | 30.740 | 59.238 | 0.71x |
| users.ndjson | ujson | 39.342 | 40.373 | 42.676 | 59.238 | 0.51x |
| users.ndjson | json | 47.700 | 54.292 | 120.261 | 59.238 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.507 | 4.073 | 6.717 | 54.770 | 1.00x |
| users.json | orjson | 4.263 | 4.873 | 7.486 | 54.770 | 0.84x |
| users.json | msgspec | 6.307 | 7.990 | 11.481 | 54.770 | 0.51x |
| users.json | ujson | 32.136 | 35.586 | 41.572 | 54.770 | 0.11x |
| users.json | json | 43.421 | 48.257 | 53.699 | 54.770 | 0.08x |
| flat.json | strata | 0.620 | 0.668 | 0.772 | 57.352 | 1.00x |
| flat.json | orjson | 0.727 | 0.778 | 0.944 | 57.352 | 0.86x |
| flat.json | msgspec | 0.841 | 0.936 | 0.976 | 57.352 | 0.71x |
| flat.json | ujson | 2.857 | 2.925 | 3.320 | 57.352 | 0.23x |
| flat.json | json | 3.839 | 3.912 | 4.015 | 57.352 | 0.17x |
| nested.json | strata | 0.518 | 0.554 | 0.614 | 52.395 | 1.00x |
| nested.json | orjson | 0.647 | 0.678 | 0.868 | 52.395 | 0.82x |
| nested.json | msgspec | 0.863 | 0.900 | 1.046 | 52.395 | 0.62x |
| nested.json | ujson | 3.113 | 3.237 | 3.704 | 52.395 | 0.17x |
| nested.json | json | 4.975 | 5.049 | 5.370 | 52.395 | 0.11x |
| wide_arrays.json | strata | 2.497 | 2.605 | 2.766 | 60.293 | 1.00x |
| wide_arrays.json | orjson | 3.344 | 3.514 | 4.046 | 60.293 | 0.74x |
| wide_arrays.json | msgspec | 4.123 | 4.192 | 4.866 | 60.293 | 0.62x |
| wide_arrays.json | ujson | 16.232 | 16.496 | 17.707 | 60.293 | 0.16x |
| wide_arrays.json | json | 35.083 | 36.234 | 38.635 | 60.293 | 0.07x |
| mixed.json | strata | 0.395 | 0.449 | 0.511 | 55.461 | 1.00x |
| mixed.json | orjson | 0.436 | 0.473 | 0.501 | 55.461 | 0.95x |
| mixed.json | msgspec | 0.473 | 0.517 | 0.645 | 55.461 | 0.87x |
| mixed.json | ujson | 1.003 | 1.110 | 1.542 | 55.461 | 0.40x |
| mixed.json | json | 1.373 | 1.429 | 1.767 | 55.461 | 0.31x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.179 | 0.208 | 0.249 | 54.824 | 1.00x |
| users.json $[*].id | jmespath | 1.008 | 1.029 | 1.179 | 54.824 | 0.20x |
| users.json $[*].id | jsonpath-ng | 5.373 | 5.503 | 5.975 | 54.824 | 0.04x |
| users.json $[*].orders[*].total | strata | 1.161 | 1.286 | 2.041 | 54.742 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 6.527 | 7.159 | 9.241 | 54.742 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 37.558 | 41.145 | 51.109 | 54.742 | 0.03x |
| users.json $..total | strata | 3.476 | 3.676 | 8.221 | 56.758 | 1.00x |
| users.json $..total | jsonpath-ng | 722.422 | 762.471 | 1278.642 | 56.758 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.722 | 3.995 | 4.469 | 58.922 | 1.00x |
| users.json $[*].id | orjson+jmespath | 30.685 | 33.139 | 35.426 | 58.922 | 0.12x |
| users.json $[*].id | orjson+jsonpath-ng | 33.356 | 38.585 | 41.604 | 58.922 | 0.10x |
| users.json $[*].orders[*].total | strata | 4.050 | 4.301 | 5.534 | 56.750 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 34.146 | 39.239 | 46.351 | 56.750 | 0.11x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 71.382 | 82.557 | 100.205 | 56.750 | 0.05x |
| users.json $..total | strata | 22.457 | 25.559 | 28.901 | 56.816 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 696.188 | 784.636 | 1111.473 | 56.816 | 0.03x |

