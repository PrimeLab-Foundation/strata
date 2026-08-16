# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 208e6f9
- python: 3.12.13
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
| users.json | strata | 12.187 | 13.006 | 18.038 | 59.336 | 1.00x |
| users.json | orjson | 15.182 | 15.833 | 19.040 | 59.336 | 0.82x |
| users.json | msgspec | 14.597 | 15.839 | 18.286 | 59.336 | 0.82x |
| users.json | ujson | 21.740 | 23.062 | 28.164 | 59.336 | 0.56x |
| users.json | pysimdjson | 23.296 | 24.516 | 28.074 | 59.336 | 0.53x |
| users.json | json | 22.877 | 23.713 | 30.490 | 59.336 | 0.55x |
| flat.json | strata | 1.144 | 1.200 | 1.467 | 70.066 | 1.00x |
| flat.json | orjson | 1.206 | 1.302 | 1.548 | 70.066 | 0.92x |
| flat.json | msgspec | 1.205 | 1.253 | 1.300 | 70.066 | 0.96x |
| flat.json | ujson | 1.994 | 2.090 | 2.143 | 70.066 | 0.57x |
| flat.json | pysimdjson | 2.079 | 2.144 | 2.276 | 70.066 | 0.56x |
| flat.json | json | 1.907 | 1.994 | 2.062 | 70.066 | 0.60x |
| nested.json | strata | 0.942 | 1.038 | 1.128 | 70.066 | 1.00x |
| nested.json | orjson | 1.094 | 1.158 | 1.196 | 70.066 | 0.90x |
| nested.json | msgspec | 1.061 | 1.137 | 1.586 | 70.066 | 0.91x |
| nested.json | ujson | 1.645 | 1.756 | 1.863 | 70.066 | 0.59x |
| nested.json | pysimdjson | 1.551 | 1.722 | 1.842 | 70.066 | 0.60x |
| nested.json | json | 1.973 | 2.056 | 2.143 | 70.066 | 0.51x |
| wide_arrays.json | strata | 6.068 | 6.432 | 7.138 | 72.934 | 1.00x |
| wide_arrays.json | orjson | 6.650 | 7.047 | 7.315 | 72.934 | 0.91x |
| wide_arrays.json | msgspec | 7.164 | 7.439 | 7.960 | 72.934 | 0.86x |
| wide_arrays.json | ujson | 8.978 | 9.513 | 10.746 | 72.934 | 0.68x |
| wide_arrays.json | pysimdjson | 7.573 | 7.928 | 8.212 | 72.934 | 0.81x |
| wide_arrays.json | json | 11.495 | 11.693 | 12.004 | 72.934 | 0.55x |
| mixed.json | strata | 0.221 | 0.245 | 0.268 | 72.996 | 1.00x |
| mixed.json | orjson | 0.257 | 0.276 | 0.296 | 72.996 | 0.89x |
| mixed.json | msgspec | 0.264 | 0.297 | 0.316 | 72.996 | 0.83x |
| mixed.json | ujson | 0.354 | 0.400 | 0.434 | 72.996 | 0.61x |
| mixed.json | pysimdjson | 0.325 | 0.369 | 0.419 | 72.996 | 0.66x |
| mixed.json | json | 0.476 | 0.522 | 0.557 | 72.996 | 0.47x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.609 | 2.699 | 2.977 | 58.438 | 1.00x |
| users.json | orjson | 3.194 | 3.299 | 3.496 | 58.438 | 0.82x |
| users.json | msgspec | 4.228 | 4.437 | 4.794 | 58.438 | 0.61x |
| users.json | ujson | 14.729 | 15.039 | 15.362 | 58.438 | 0.18x |
| users.json | json | 22.035 | 22.453 | 23.055 | 58.438 | 0.12x |
| flat.json | strata | 0.372 | 0.425 | 0.488 | 70.066 | 1.00x |
| flat.json | orjson | 0.454 | 0.474 | 0.526 | 70.066 | 0.90x |
| flat.json | msgspec | 0.590 | 0.616 | 0.702 | 70.066 | 0.69x |
| flat.json | ujson | 1.371 | 1.386 | 1.571 | 70.066 | 0.31x |
| flat.json | json | 2.051 | 2.076 | 2.215 | 70.066 | 0.20x |
| nested.json | strata | 0.250 | 0.262 | 0.345 | 70.066 | 1.00x |
| nested.json | orjson | 0.314 | 0.329 | 0.345 | 70.066 | 0.80x |
| nested.json | msgspec | 0.436 | 0.452 | 0.560 | 70.066 | 0.58x |
| nested.json | ujson | 1.364 | 1.382 | 1.415 | 70.066 | 0.19x |
| nested.json | json | 2.384 | 2.417 | 2.434 | 70.066 | 0.11x |
| wide_arrays.json | strata | 1.930 | 2.072 | 2.253 | 72.934 | 1.00x |
| wide_arrays.json | orjson | 1.943 | 2.094 | 2.160 | 72.934 | 0.99x |
| wide_arrays.json | msgspec | 3.136 | 3.235 | 3.586 | 72.934 | 0.64x |
| wide_arrays.json | ujson | 8.864 | 9.066 | 9.443 | 72.934 | 0.23x |
| wide_arrays.json | json | 17.347 | 17.617 | 17.850 | 72.934 | 0.12x |
| mixed.json | strata | 0.070 | 0.075 | 0.079 | 72.996 | 1.00x |
| mixed.json | orjson | 0.073 | 0.076 | 0.083 | 72.996 | 0.98x |
| mixed.json | msgspec | 0.095 | 0.101 | 0.106 | 72.996 | 0.74x |
| mixed.json | ujson | 0.309 | 0.318 | 0.346 | 72.996 | 0.24x |
| mixed.json | json | 0.539 | 0.552 | 0.561 | 72.996 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 12.916 | 14.281 | 16.870 | 75.664 | 1.00x |
| users.json | orjson | 15.358 | 16.704 | 17.987 | 75.664 | 0.85x |
| users.json | msgspec | 15.608 | 16.565 | 18.059 | 75.664 | 0.86x |
| users.json | ujson | 23.852 | 24.654 | 26.736 | 75.664 | 0.58x |
| users.json | json | 23.207 | 24.425 | 26.810 | 75.664 | 0.58x |
| flat.json | strata | 1.316 | 1.335 | 1.398 | 70.066 | 1.00x |
| flat.json | orjson | 1.419 | 1.491 | 1.547 | 70.066 | 0.90x |
| flat.json | msgspec | 1.352 | 1.429 | 1.489 | 70.066 | 0.93x |
| flat.json | ujson | 2.068 | 2.154 | 2.243 | 70.066 | 0.62x |
| flat.json | json | 2.113 | 2.145 | 2.296 | 70.066 | 0.62x |
| nested.json | strata | 1.010 | 1.169 | 1.644 | 70.066 | 1.00x |
| nested.json | orjson | 1.242 | 1.461 | 3.020 | 70.066 | 0.80x |
| nested.json | msgspec | 1.149 | 1.256 | 2.428 | 70.066 | 0.93x |
| nested.json | ujson | 1.719 | 1.919 | 3.740 | 70.066 | 0.61x |
| nested.json | json | 2.105 | 2.257 | 3.768 | 70.066 | 0.52x |
| wide_arrays.json | strata | 5.756 | 5.916 | 6.177 | 72.996 | 1.00x |
| wide_arrays.json | orjson | 6.400 | 6.537 | 6.843 | 72.996 | 0.90x |
| wide_arrays.json | msgspec | 6.798 | 7.088 | 7.293 | 72.996 | 0.83x |
| wide_arrays.json | ujson | 8.624 | 8.918 | 9.397 | 72.996 | 0.66x |
| wide_arrays.json | json | 10.820 | 10.929 | 11.369 | 72.996 | 0.54x |
| mixed.json | strata | 0.265 | 0.293 | 0.318 | 72.996 | 1.00x |
| mixed.json | orjson | 0.327 | 0.367 | 0.402 | 72.996 | 0.80x |
| mixed.json | msgspec | 0.335 | 0.353 | 0.398 | 72.996 | 0.83x |
| mixed.json | ujson | 0.431 | 0.466 | 0.506 | 72.996 | 0.63x |
| mixed.json | json | 0.530 | 0.576 | 0.598 | 72.996 | 0.51x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 15.336 | 16.677 | 17.239 | 70.066 | 1.00x |
| users.ndjson | orjson | 21.843 | 23.678 | 24.618 | 70.066 | 0.70x |
| users.ndjson | msgspec | 21.289 | 22.584 | 24.296 | 70.066 | 0.74x |
| users.ndjson | ujson | 27.680 | 29.898 | 31.123 | 70.066 | 0.56x |
| users.ndjson | json | 33.388 | 35.004 | 36.435 | 70.066 | 0.48x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.324 | 3.358 | 3.427 | 73.695 | 1.00x |
| users.json | orjson | 3.969 | 4.070 | 4.307 | 73.695 | 0.82x |
| users.json | msgspec | 4.958 | 5.113 | 6.064 | 73.695 | 0.66x |
| users.json | ujson | 15.724 | 15.866 | 17.805 | 73.695 | 0.21x |
| users.json | json | 23.022 | 23.421 | 24.540 | 73.695 | 0.14x |
| flat.json | strata | 0.632 | 0.710 | 0.777 | 70.066 | 1.00x |
| flat.json | orjson | 0.744 | 0.830 | 0.859 | 70.066 | 0.86x |
| flat.json | msgspec | 0.867 | 0.959 | 1.034 | 70.066 | 0.74x |
| flat.json | ujson | 1.702 | 1.767 | 1.859 | 70.066 | 0.40x |
| flat.json | json | 2.416 | 2.458 | 2.588 | 70.066 | 0.29x |
| nested.json | strata | 0.425 | 0.466 | 0.581 | 70.066 | 1.00x |
| nested.json | orjson | 0.522 | 0.555 | 0.625 | 70.066 | 0.84x |
| nested.json | msgspec | 0.623 | 0.656 | 0.773 | 70.066 | 0.71x |
| nested.json | ujson | 1.587 | 1.625 | 1.782 | 70.066 | 0.29x |
| nested.json | json | 2.579 | 2.628 | 2.804 | 70.066 | 0.18x |
| wide_arrays.json | strata | 2.372 | 2.482 | 2.668 | 72.996 | 1.00x |
| wide_arrays.json | orjson | 2.492 | 2.545 | 2.829 | 72.996 | 0.98x |
| wide_arrays.json | msgspec | 3.645 | 3.722 | 3.910 | 72.996 | 0.67x |
| wide_arrays.json | ujson | 9.354 | 9.559 | 9.833 | 72.996 | 0.26x |
| wide_arrays.json | json | 18.088 | 18.491 | 20.129 | 72.996 | 0.13x |
| mixed.json | strata | 0.181 | 0.190 | 0.203 | 72.996 | 1.00x |
| mixed.json | orjson | 0.188 | 0.222 | 0.265 | 72.996 | 0.86x |
| mixed.json | msgspec | 0.216 | 0.240 | 0.349 | 72.996 | 0.79x |
| mixed.json | ujson | 0.444 | 0.463 | 0.510 | 72.996 | 0.41x |
| mixed.json | json | 0.674 | 0.712 | 0.857 | 72.996 | 0.27x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.086 | 0.090 | 0.103 | 73.695 | 1.00x |
| users.json $[*].id | jmespath | 0.474 | 0.497 | 0.521 | 73.695 | 0.18x |
| users.json $[*].id | jsonpath-ng | 2.917 | 3.240 | 3.358 | 73.695 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.451 | 0.497 | 0.654 | 73.695 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.053 | 3.129 | 3.479 | 73.695 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 21.602 | 23.271 | 25.348 | 73.695 | 0.02x |
| users.json $..total | strata | 1.926 | 2.163 | 2.551 | 73.695 | 1.00x |
| users.json $..total | jsonpath-ng | 395.423 | 396.725 | 399.537 | 73.695 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.422 | 3.439 | 3.474 | 73.695 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.591 | 15.984 | 16.260 | 73.695 | 0.22x |
| users.json $[*].id | orjson+jsonpath-ng | 18.102 | 18.372 | 20.522 | 73.695 | 0.19x |
| users.json $[*].orders[*].total | strata | 3.749 | 3.799 | 3.984 | 73.695 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 19.454 | 20.852 | 23.330 | 73.695 | 0.18x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 45.342 | 47.053 | 50.960 | 73.695 | 0.08x |
| users.json $..total | strata | 16.987 | 19.420 | 21.076 | 73.695 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 426.064 | 429.642 | 435.652 | 73.695 | 0.05x |

