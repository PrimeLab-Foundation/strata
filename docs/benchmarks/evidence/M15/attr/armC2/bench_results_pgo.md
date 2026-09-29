# Benchmark results - pgo

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 5b876d1a77ab7cbc4f5cfa8783bce584663659a1
- python: 3.14.7
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit-Mach-O
- machine: arm64
- processor: Apple M1 Max
- compiler_flags: -fno-strict-overflow -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -O3 -Wall -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -march=native -flto=thin -fprofile-use=/Users/borysbardysh/worktrees/strata/m15-pgo/build/pgo/strata.profdata -bundle -undefined (23 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 5.815 | 6.004 | 6.373 | 55.734 | 1.00x |
| users.json | orjson | 7.805 | 8.144 | 8.545 | 55.734 | 0.74x |
| users.json | msgspec | 7.953 | 8.219 | 8.719 | 55.734 | 0.73x |
| users.json | ujson | 10.920 | 11.216 | 11.629 | 55.734 | 0.54x |
| users.json | json | 15.410 | 15.602 | 16.158 | 55.734 | 0.38x |
| flat.json | strata | 0.574 | 0.595 | 0.614 | 80.250 | 1.00x |
| flat.json | orjson | 0.656 | 0.669 | 0.682 | 80.250 | 0.89x |
| flat.json | msgspec | 0.696 | 0.714 | 0.746 | 80.250 | 0.83x |
| flat.json | ujson | 1.033 | 1.045 | 1.159 | 80.250 | 0.57x |
| flat.json | json | 1.449 | 1.484 | 1.585 | 80.250 | 0.40x |
| nested.json | strata | 0.508 | 0.515 | 0.588 | 80.297 | 1.00x |
| nested.json | orjson | 0.616 | 0.632 | 0.661 | 80.297 | 0.81x |
| nested.json | msgspec | 0.621 | 0.631 | 0.680 | 80.297 | 0.82x |
| nested.json | ujson | 0.879 | 0.912 | 1.003 | 80.297 | 0.56x |
| nested.json | json | 1.427 | 1.448 | 1.607 | 80.297 | 0.36x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.326 | 1.354 | 1.384 | 61.641 | 1.00x |
| users.json | orjson | 2.075 | 2.101 | 2.169 | 61.641 | 0.64x |
| users.json | msgspec | 2.626 | 2.711 | 2.810 | 61.641 | 0.50x |
| users.json | ujson | 8.662 | 8.833 | 9.130 | 61.641 | 0.15x |
| users.json | json | 15.823 | 16.010 | 16.876 | 61.641 | 0.08x |
| flat.json | strata | 0.191 | 0.198 | 0.216 | 80.281 | 1.00x |
| flat.json | orjson | 0.238 | 0.247 | 0.255 | 80.281 | 0.81x |
| flat.json | msgspec | 0.299 | 0.308 | 0.322 | 80.281 | 0.65x |
| flat.json | ujson | 0.758 | 0.778 | 0.802 | 80.281 | 0.26x |
| flat.json | json | 1.339 | 1.357 | 1.564 | 80.281 | 0.15x |
| nested.json | strata | 0.117 | 0.128 | 0.150 | 80.312 | 1.00x |
| nested.json | orjson | 0.204 | 0.220 | 0.284 | 80.312 | 0.58x |
| nested.json | msgspec | 0.275 | 0.295 | 0.306 | 80.312 | 0.43x |
| nested.json | ujson | 0.850 | 0.893 | 0.901 | 80.312 | 0.14x |
| nested.json | json | 1.652 | 1.735 | 2.001 | 80.312 | 0.07x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.156 | 6.272 | 6.533 | 76.688 | 1.00x |
| users.json | orjson | 8.295 | 8.537 | 8.974 | 76.688 | 0.73x |
| users.json | msgspec | 8.469 | 8.732 | 9.092 | 76.688 | 0.72x |
| users.json | ujson | 11.386 | 11.721 | 12.114 | 76.688 | 0.54x |
| users.json | json | 15.725 | 16.026 | 17.804 | 76.688 | 0.39x |
| flat.json | strata | 0.665 | 0.682 | 0.785 | 80.281 | 1.00x |
| flat.json | orjson | 0.763 | 0.793 | 0.854 | 80.281 | 0.86x |
| flat.json | msgspec | 0.811 | 0.838 | 0.870 | 80.281 | 0.81x |
| flat.json | ujson | 1.168 | 1.214 | 1.308 | 80.281 | 0.56x |
| flat.json | json | 1.556 | 1.603 | 1.645 | 80.281 | 0.43x |
| nested.json | strata | 0.577 | 0.597 | 0.747 | 80.312 | 1.00x |
| nested.json | orjson | 0.711 | 0.735 | 0.918 | 80.312 | 0.81x |
| nested.json | msgspec | 0.707 | 0.734 | 0.784 | 80.312 | 0.81x |
| nested.json | ujson | 0.995 | 1.055 | 1.169 | 80.312 | 0.57x |
| nested.json | json | 1.513 | 1.572 | 1.640 | 80.312 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.708 | 1.756 | 1.874 | 76.812 | 1.00x |
| users.json | orjson | 2.453 | 2.553 | 2.775 | 76.812 | 0.69x |
| users.json | msgspec | 3.079 | 3.124 | 3.252 | 76.812 | 0.56x |
| users.json | ujson | 9.154 | 9.259 | 9.519 | 76.812 | 0.19x |
| users.json | json | 15.776 | 16.513 | 16.960 | 76.812 | 0.11x |
| flat.json | strata | 0.364 | 0.398 | 0.468 | 80.281 | 1.00x |
| flat.json | orjson | 0.414 | 0.452 | 0.540 | 80.281 | 0.88x |
| flat.json | msgspec | 0.467 | 0.533 | 0.585 | 80.281 | 0.75x |
| flat.json | ujson | 0.929 | 0.982 | 1.119 | 80.281 | 0.41x |
| flat.json | json | 1.542 | 1.591 | 1.745 | 80.281 | 0.25x |
| nested.json | strata | 0.325 | 0.360 | 0.434 | 80.328 | 1.00x |
| nested.json | orjson | 0.428 | 0.494 | 4.721 | 80.328 | 0.73x |
| nested.json | msgspec | 0.488 | 0.548 | 0.576 | 80.328 | 0.66x |
| nested.json | ujson | 1.096 | 1.161 | 1.257 | 80.328 | 0.31x |
| nested.json | json | 1.942 | 1.974 | 2.293 | 80.328 | 0.18x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.055 | 0.064 | 0.091 | 77.219 | 1.00x |
| users.json $[*].id | jmespath | 0.279 | 0.301 | 0.376 | 77.219 | 0.21x |
| users.json $[*].id | jsonpath-ng | 1.445 | 1.485 | 1.629 | 77.219 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.336 | 0.370 | 0.419 | 77.703 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.676 | 1.776 | 1.915 | 77.703 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 9.690 | 9.960 | 10.618 | 77.703 | 0.04x |
| users.json $..total | strata | 1.379 | 1.420 | 1.509 | 77.703 | 1.00x |
| users.json $..total | jsonpath-ng | 188.335 | 190.321 | 192.468 | 77.703 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.367 | 3.383 | 3.492 | 77.281 | 1.00x |
| users.json $[*].id | orjson+jmespath | 8.616 | 8.879 | 9.347 | 77.281 | 0.38x |
| users.json $[*].id | orjson+jsonpath-ng | 9.923 | 10.270 | 10.488 | 77.281 | 0.33x |
| users.json $[*].orders[*].total | strata | 3.423 | 3.437 | 3.607 | 77.703 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 9.827 | 10.137 | 11.117 | 77.703 | 0.34x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 19.457 | 19.647 | 20.389 | 77.703 | 0.17x |
| users.json $..total | strata | 7.720 | 7.902 | 8.139 | 79.359 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 201.767 | 203.597 | 205.768 | 79.359 | 0.04x |

