# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: b294ccd
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
| users.json | strata | 19.871 | 20.884 | 23.863 | 52.586 | 1.00x |
| users.json | orjson | 28.467 | 29.380 | 34.599 | 52.586 | 0.71x |
| users.json | msgspec | 27.362 | 29.251 | 31.790 | 52.586 | 0.71x |
| users.json | ujson | 40.043 | 42.085 | 52.991 | 52.586 | 0.50x |
| users.json | pysimdjson | 167.795 | 174.337 | 185.544 | 52.586 | 0.12x |
| users.json | json | 45.654 | 46.932 | 57.704 | 52.586 | 0.44x |
| flat.json | strata | 1.309 | 1.355 | 1.435 | 60.504 | 1.00x |
| flat.json | orjson | 1.402 | 1.455 | 1.553 | 60.504 | 0.93x |
| flat.json | msgspec | 1.616 | 1.656 | 1.731 | 60.504 | 0.82x |
| flat.json | ujson | 2.826 | 2.918 | 3.074 | 60.504 | 0.46x |
| flat.json | pysimdjson | 15.036 | 15.118 | 15.423 | 60.504 | 0.09x |
| flat.json | json | 3.251 | 3.282 | 3.399 | 60.504 | 0.41x |
| nested.json | strata | 1.402 | 1.470 | 1.538 | 58.324 | 1.00x |
| nested.json | orjson | 1.588 | 1.676 | 1.767 | 58.324 | 0.88x |
| nested.json | msgspec | 1.777 | 1.877 | 2.168 | 58.324 | 0.78x |
| nested.json | ujson | 3.027 | 3.077 | 3.340 | 58.324 | 0.48x |
| nested.json | pysimdjson | 13.113 | 13.210 | 13.597 | 58.324 | 0.11x |
| nested.json | json | 3.822 | 3.845 | 3.986 | 58.324 | 0.38x |
| wide_arrays.json | strata | 8.356 | 8.568 | 10.052 | 60.961 | 1.00x |
| wide_arrays.json | orjson | 8.813 | 9.678 | 10.405 | 60.961 | 0.89x |
| wide_arrays.json | msgspec | 9.901 | 10.234 | 11.953 | 60.961 | 0.84x |
| wide_arrays.json | ujson | 12.851 | 13.252 | 14.927 | 60.961 | 0.65x |
| wide_arrays.json | pysimdjson | 76.467 | 77.999 | 82.741 | 60.961 | 0.11x |
| wide_arrays.json | json | 16.534 | 17.131 | 19.363 | 60.961 | 0.50x |
| mixed.json | strata | 0.382 | 0.429 | 0.555 | 58.562 | 1.00x |
| mixed.json | orjson | 0.458 | 0.470 | 0.522 | 58.562 | 0.91x |
| mixed.json | msgspec | 0.473 | 0.514 | 0.563 | 58.562 | 0.83x |
| mixed.json | ujson | 0.661 | 0.701 | 0.904 | 58.562 | 0.61x |
| mixed.json | pysimdjson | 3.300 | 3.383 | 3.984 | 58.562 | 0.13x |
| mixed.json | json | 0.933 | 0.988 | 1.197 | 58.562 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.968 | 3.075 | 3.756 | 48.172 | 1.00x |
| users.json | orjson | 3.809 | 4.360 | 5.603 | 48.172 | 0.71x |
| users.json | msgspec | 5.444 | 6.235 | 7.536 | 48.172 | 0.49x |
| users.json | ujson | 32.642 | 34.266 | 42.998 | 48.172 | 0.09x |
| users.json | json | 44.424 | 47.224 | 58.556 | 48.172 | 0.07x |
| flat.json | strata | 0.289 | 0.321 | 0.330 | 58.188 | 1.00x |
| flat.json | orjson | 0.381 | 0.418 | 0.442 | 58.188 | 0.77x |
| flat.json | msgspec | 0.490 | 0.543 | 0.568 | 58.188 | 0.59x |
| flat.json | ujson | 2.550 | 2.670 | 2.901 | 58.188 | 0.12x |
| flat.json | json | 3.598 | 3.691 | 3.743 | 58.188 | 0.09x |
| nested.json | strata | 0.231 | 0.243 | 0.281 | 53.223 | 1.00x |
| nested.json | orjson | 0.332 | 0.348 | 0.379 | 53.223 | 0.70x |
| nested.json | msgspec | 0.530 | 0.554 | 0.562 | 53.223 | 0.44x |
| nested.json | ujson | 2.680 | 2.706 | 2.778 | 53.223 | 0.09x |
| nested.json | json | 4.468 | 4.507 | 4.773 | 53.223 | 0.05x |
| wide_arrays.json | strata | 1.843 | 1.997 | 2.473 | 59.730 | 1.00x |
| wide_arrays.json | orjson | 2.337 | 2.554 | 2.779 | 59.730 | 0.78x |
| wide_arrays.json | msgspec | 3.264 | 3.397 | 4.159 | 59.730 | 0.59x |
| wide_arrays.json | ujson | 15.577 | 15.863 | 18.406 | 59.730 | 0.13x |
| wide_arrays.json | json | 34.663 | 36.067 | 38.454 | 59.730 | 0.06x |
| mixed.json | strata | 0.090 | 0.093 | 0.117 | 54.223 | 1.00x |
| mixed.json | orjson | 0.088 | 0.091 | 0.119 | 54.223 | 1.02x |
| mixed.json | msgspec | 0.119 | 0.125 | 0.147 | 54.223 | 0.74x |
| mixed.json | ujson | 0.627 | 0.634 | 0.726 | 54.223 | 0.15x |
| mixed.json | json | 0.979 | 0.988 | 1.139 | 54.223 | 0.09x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 20.295 | 22.915 | 26.227 | 60.383 | 1.00x |
| users.json | orjson | 29.422 | 34.068 | 39.560 | 60.383 | 0.67x |
| users.json | msgspec | 29.659 | 32.546 | 40.548 | 60.383 | 0.70x |
| users.json | ujson | 45.487 | 48.168 | 54.289 | 60.383 | 0.48x |
| users.json | json | 51.491 | 57.494 | 64.260 | 60.383 | 0.40x |
| flat.json | strata | 1.400 | 1.427 | 1.472 | 58.188 | 1.00x |
| flat.json | orjson | 1.548 | 1.569 | 1.668 | 58.188 | 0.91x |
| flat.json | msgspec | 1.768 | 1.781 | 1.809 | 58.188 | 0.80x |
| flat.json | ujson | 2.980 | 2.992 | 3.050 | 58.188 | 0.48x |
| flat.json | json | 3.309 | 3.346 | 3.499 | 58.188 | 0.43x |
| nested.json | strata | 1.533 | 1.586 | 1.632 | 53.223 | 1.00x |
| nested.json | orjson | 1.797 | 1.849 | 1.935 | 53.223 | 0.86x |
| nested.json | msgspec | 1.946 | 2.031 | 2.223 | 53.223 | 0.78x |
| nested.json | ujson | 3.114 | 3.273 | 3.560 | 53.223 | 0.48x |
| nested.json | json | 3.870 | 4.040 | 4.493 | 53.223 | 0.39x |
| wide_arrays.json | strata | 8.914 | 9.211 | 13.908 | 59.777 | 1.00x |
| wide_arrays.json | orjson | 9.400 | 10.072 | 14.792 | 59.777 | 0.91x |
| wide_arrays.json | msgspec | 10.637 | 11.359 | 13.770 | 59.777 | 0.81x |
| wide_arrays.json | ujson | 13.676 | 14.498 | 16.229 | 59.777 | 0.64x |
| wide_arrays.json | json | 17.515 | 18.286 | 21.181 | 59.777 | 0.50x |
| mixed.json | strata | 0.477 | 0.509 | 0.905 | 54.223 | 1.00x |
| mixed.json | orjson | 0.594 | 0.637 | 0.925 | 54.223 | 0.80x |
| mixed.json | msgspec | 0.640 | 0.722 | 1.041 | 54.223 | 0.71x |
| mixed.json | ujson | 0.830 | 0.917 | 1.258 | 54.223 | 0.56x |
| mixed.json | json | 1.067 | 1.309 | 1.542 | 54.223 | 0.39x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 19.932 | 20.783 | 22.738 | 60.902 | 1.00x |
| users.ndjson | orjson | 27.647 | 29.612 | 36.989 | 60.902 | 0.70x |
| users.ndjson | msgspec | 28.232 | 28.981 | 36.859 | 60.902 | 0.72x |
| users.ndjson | ujson | 40.252 | 41.188 | 48.678 | 60.902 | 0.50x |
| users.ndjson | json | 50.281 | 51.464 | 63.388 | 60.902 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.877 | 4.461 | 5.112 | 58.469 | 1.00x |
| users.json | orjson | 5.008 | 5.613 | 6.474 | 58.469 | 0.79x |
| users.json | msgspec | 6.629 | 7.001 | 7.938 | 58.469 | 0.64x |
| users.json | ujson | 34.066 | 34.375 | 39.354 | 58.469 | 0.13x |
| users.json | json | 44.874 | 46.073 | 55.640 | 58.469 | 0.10x |
| flat.json | strata | 0.631 | 0.674 | 0.854 | 58.188 | 1.00x |
| flat.json | orjson | 0.795 | 0.819 | 1.004 | 58.188 | 0.82x |
| flat.json | msgspec | 0.889 | 0.948 | 1.506 | 58.188 | 0.71x |
| flat.json | ujson | 2.941 | 2.994 | 4.080 | 58.188 | 0.22x |
| flat.json | json | 3.978 | 4.083 | 4.764 | 58.188 | 0.16x |
| nested.json | strata | 0.559 | 0.615 | 0.769 | 53.223 | 1.00x |
| nested.json | orjson | 0.676 | 0.732 | 0.958 | 53.223 | 0.84x |
| nested.json | msgspec | 0.820 | 0.930 | 0.987 | 53.223 | 0.66x |
| nested.json | ujson | 3.048 | 3.142 | 3.242 | 53.223 | 0.20x |
| nested.json | json | 4.803 | 4.937 | 5.348 | 53.223 | 0.12x |
| wide_arrays.json | strata | 2.609 | 2.695 | 2.766 | 59.777 | 1.00x |
| wide_arrays.json | orjson | 3.286 | 3.402 | 3.675 | 59.777 | 0.79x |
| wide_arrays.json | msgspec | 4.099 | 4.255 | 4.853 | 59.777 | 0.63x |
| wide_arrays.json | ujson | 16.445 | 17.014 | 19.107 | 59.777 | 0.16x |
| wide_arrays.json | json | 35.508 | 36.039 | 38.626 | 59.777 | 0.07x |
| mixed.json | strata | 0.381 | 0.416 | 0.624 | 54.223 | 1.00x |
| mixed.json | orjson | 0.421 | 0.452 | 0.631 | 54.223 | 0.92x |
| mixed.json | msgspec | 0.413 | 0.456 | 0.820 | 54.223 | 0.91x |
| mixed.json | ujson | 1.009 | 1.118 | 1.204 | 54.223 | 0.37x |
| mixed.json | json | 1.321 | 1.488 | 2.763 | 54.223 | 0.28x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.199 | 0.207 | 0.257 | 58.539 | 1.00x |
| users.json $[*].id | jmespath | 1.051 | 1.103 | 1.563 | 58.539 | 0.19x |
| users.json $[*].id | jsonpath-ng | 5.640 | 5.876 | 6.626 | 58.539 | 0.04x |
| users.json $[*].orders[*].total | strata | 1.209 | 1.353 | 1.509 | 55.699 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 6.608 | 7.062 | 7.898 | 55.699 | 0.19x |
| users.json $[*].orders[*].total | jsonpath-ng | 38.634 | 39.551 | 41.602 | 55.699 | 0.03x |
| users.json $..total | strata | 3.321 | 3.625 | 4.834 | 56.051 | 1.00x |
| users.json $..total | jsonpath-ng | 712.540 | 755.497 | 794.268 | 56.051 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.159 | 4.243 | 4.633 | 58.594 | 1.00x |
| users.json $[*].id | orjson+jmespath | 28.296 | 30.682 | 33.956 | 58.594 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 33.653 | 35.416 | 43.114 | 58.594 | 0.12x |
| users.json $[*].orders[*].total | strata | 4.344 | 4.433 | 5.092 | 56.016 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 33.975 | 36.407 | 40.999 | 56.016 | 0.12x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 69.683 | 76.037 | 83.013 | 56.016 | 0.06x |
| users.json $..total | strata | 23.205 | 24.159 | 25.579 | 56.082 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 731.445 | 762.144 | 886.139 | 56.082 | 0.03x |

