# Benchmark results - pgo

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: ad04f61a26512aa5425916df3c2bfa78450ec478
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
| users.json | strata | 5.800 | 5.952 | 6.529 | 57.062 | 1.00x |
| users.json | orjson | 7.851 | 8.231 | 8.841 | 57.062 | 0.72x |
| users.json | msgspec | 7.945 | 8.425 | 9.018 | 57.062 | 0.71x |
| users.json | ujson | 10.736 | 11.344 | 11.870 | 57.062 | 0.52x |
| users.json | json | 15.247 | 15.877 | 16.461 | 57.062 | 0.37x |
| flat.json | strata | 0.547 | 0.560 | 0.618 | 76.672 | 1.00x |
| flat.json | orjson | 0.624 | 0.638 | 0.664 | 76.672 | 0.88x |
| flat.json | msgspec | 0.682 | 0.710 | 0.853 | 76.672 | 0.79x |
| flat.json | ujson | 0.989 | 1.021 | 1.076 | 76.672 | 0.55x |
| flat.json | json | 1.397 | 1.438 | 1.531 | 76.672 | 0.39x |
| nested.json | strata | 0.491 | 0.507 | 0.561 | 76.688 | 1.00x |
| nested.json | orjson | 0.605 | 0.620 | 0.750 | 76.688 | 0.82x |
| nested.json | msgspec | 0.611 | 0.639 | 0.662 | 76.688 | 0.79x |
| nested.json | ujson | 0.883 | 0.900 | 0.960 | 76.688 | 0.56x |
| nested.json | json | 1.406 | 1.444 | 1.687 | 76.688 | 0.35x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.336 | 1.369 | 1.531 | 61.531 | 1.00x |
| users.json | orjson | 2.044 | 2.120 | 2.335 | 61.531 | 0.65x |
| users.json | msgspec | 2.647 | 2.727 | 2.854 | 61.531 | 0.50x |
| users.json | ujson | 8.714 | 8.906 | 9.268 | 61.531 | 0.15x |
| users.json | json | 15.679 | 16.415 | 17.003 | 61.531 | 0.08x |
| flat.json | strata | 0.193 | 0.197 | 0.225 | 76.688 | 1.00x |
| flat.json | orjson | 0.237 | 0.242 | 0.272 | 76.688 | 0.82x |
| flat.json | msgspec | 0.293 | 0.303 | 0.326 | 76.688 | 0.65x |
| flat.json | ujson | 0.746 | 0.751 | 0.794 | 76.688 | 0.26x |
| flat.json | json | 1.329 | 1.351 | 1.537 | 76.688 | 0.15x |
| nested.json | strata | 0.116 | 0.120 | 0.130 | 76.781 | 1.00x |
| nested.json | orjson | 0.202 | 0.223 | 0.292 | 76.781 | 0.54x |
| nested.json | msgspec | 0.273 | 0.295 | 0.326 | 76.781 | 0.41x |
| nested.json | ujson | 0.858 | 0.882 | 0.930 | 76.781 | 0.14x |
| nested.json | json | 1.658 | 1.806 | 2.129 | 76.781 | 0.07x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.107 | 6.277 | 7.073 | 75.156 | 1.00x |
| users.json | orjson | 8.201 | 8.492 | 8.793 | 75.156 | 0.74x |
| users.json | msgspec | 8.445 | 8.750 | 9.179 | 75.156 | 0.72x |
| users.json | ujson | 11.381 | 12.064 | 12.765 | 75.156 | 0.52x |
| users.json | json | 15.613 | 16.052 | 16.954 | 75.156 | 0.39x |
| flat.json | strata | 0.629 | 0.681 | 0.714 | 76.688 | 1.00x |
| flat.json | orjson | 0.739 | 0.772 | 0.868 | 76.688 | 0.88x |
| flat.json | msgspec | 0.789 | 0.827 | 0.981 | 76.688 | 0.82x |
| flat.json | ujson | 1.129 | 1.179 | 1.363 | 76.688 | 0.58x |
| flat.json | json | 1.512 | 1.553 | 1.656 | 76.688 | 0.44x |
| nested.json | strata | 0.580 | 0.601 | 0.634 | 76.781 | 1.00x |
| nested.json | orjson | 0.688 | 0.735 | 0.815 | 76.781 | 0.82x |
| nested.json | msgspec | 0.708 | 0.726 | 0.863 | 76.781 | 0.83x |
| nested.json | ujson | 0.996 | 1.038 | 1.199 | 76.781 | 0.58x |
| nested.json | json | 1.504 | 1.549 | 1.656 | 76.781 | 0.39x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.731 | 1.884 | 1.981 | 75.156 | 1.00x |
| users.json | orjson | 2.533 | 2.623 | 2.902 | 75.156 | 0.72x |
| users.json | msgspec | 3.117 | 3.212 | 3.544 | 75.156 | 0.59x |
| users.json | ujson | 9.178 | 9.447 | 9.907 | 75.156 | 0.20x |
| users.json | json | 16.273 | 16.943 | 17.120 | 75.156 | 0.11x |
| flat.json | strata | 0.373 | 0.403 | 0.454 | 76.688 | 1.00x |
| flat.json | orjson | 0.436 | 0.465 | 0.565 | 76.688 | 0.87x |
| flat.json | msgspec | 0.486 | 0.527 | 0.612 | 76.688 | 0.76x |
| flat.json | ujson | 0.948 | 0.985 | 1.037 | 76.688 | 0.41x |
| flat.json | json | 1.507 | 1.646 | 1.831 | 76.688 | 0.24x |
| nested.json | strata | 0.298 | 0.349 | 0.395 | 76.781 | 1.00x |
| nested.json | orjson | 0.389 | 0.451 | 0.503 | 76.781 | 0.77x |
| nested.json | msgspec | 0.497 | 0.528 | 0.731 | 76.781 | 0.66x |
| nested.json | ujson | 1.038 | 1.099 | 1.189 | 76.781 | 0.32x |
| nested.json | json | 1.905 | 2.140 | 2.287 | 76.781 | 0.16x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.058 | 0.067 | 0.105 | 75.438 | 1.00x |
| users.json $[*].id | jmespath | 0.279 | 0.302 | 0.351 | 75.438 | 0.22x |
| users.json $[*].id | jsonpath-ng | 1.442 | 1.521 | 1.630 | 75.438 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.358 | 0.381 | 0.582 | 75.734 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.679 | 1.724 | 2.012 | 75.734 | 0.22x |
| users.json $[*].orders[*].total | jsonpath-ng | 9.553 | 9.884 | 11.378 | 75.734 | 0.04x |
| users.json $..total | strata | 1.384 | 1.437 | 1.632 | 75.766 | 1.00x |
| users.json $..total | jsonpath-ng | 189.200 | 190.157 | 192.458 | 75.766 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.401 | 3.491 | 3.639 | 75.500 | 1.00x |
| users.json $[*].id | orjson+jmespath | 8.469 | 8.805 | 8.978 | 75.500 | 0.40x |
| users.json $[*].id | orjson+jsonpath-ng | 9.740 | 10.225 | 10.875 | 75.500 | 0.34x |
| users.json $[*].orders[*].total | strata | 3.448 | 3.583 | 3.753 | 75.734 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 9.759 | 10.252 | 10.733 | 75.734 | 0.35x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 19.467 | 20.385 | 21.138 | 75.734 | 0.18x |
| users.json $..total | strata | 7.518 | 7.781 | 8.412 | 75.766 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 199.427 | 201.162 | 206.262 | 75.766 | 0.04x |

