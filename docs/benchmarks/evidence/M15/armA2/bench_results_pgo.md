# Benchmark results - pgo

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 38eaa9f248f509d9a4beb75b267189d011c1109e
- python: 3.14.7
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit-Mach-O
- machine: arm64
- processor: Apple M1 Max
- compiler_flags: -fno-strict-overflow -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -O3 -Wall -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -march=native -flto=thin -fprofile-use=/Users/borysbardysh/worktrees/strata/main-pgo/build/pgo/strata.profdata -bundle -undefined (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 5.819 | 6.070 | 6.677 | 57.156 | 1.00x |
| users.json | orjson | 8.027 | 8.147 | 8.460 | 57.156 | 0.75x |
| users.json | msgspec | 7.881 | 8.221 | 8.800 | 57.156 | 0.74x |
| users.json | ujson | 10.775 | 11.109 | 11.594 | 57.156 | 0.55x |
| users.json | json | 15.398 | 15.829 | 16.810 | 57.156 | 0.38x |
| flat.json | strata | 0.564 | 0.590 | 0.609 | 80.016 | 1.00x |
| flat.json | orjson | 0.653 | 0.667 | 0.755 | 80.016 | 0.88x |
| flat.json | msgspec | 0.687 | 0.714 | 0.793 | 80.016 | 0.83x |
| flat.json | ujson | 1.002 | 1.062 | 1.091 | 80.016 | 0.56x |
| flat.json | json | 1.437 | 1.488 | 1.552 | 80.016 | 0.40x |
| nested.json | strata | 0.478 | 0.498 | 0.513 | 80.062 | 1.00x |
| nested.json | orjson | 0.602 | 0.625 | 0.671 | 80.062 | 0.80x |
| nested.json | msgspec | 0.620 | 0.631 | 0.813 | 80.062 | 0.79x |
| nested.json | ujson | 0.891 | 0.898 | 0.926 | 80.062 | 0.55x |
| nested.json | json | 1.431 | 1.445 | 1.507 | 80.062 | 0.34x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.294 | 1.356 | 1.549 | 61.672 | 1.00x |
| users.json | orjson | 2.007 | 2.107 | 2.289 | 61.672 | 0.64x |
| users.json | msgspec | 2.575 | 2.694 | 2.751 | 61.672 | 0.50x |
| users.json | ujson | 8.716 | 8.835 | 9.109 | 61.672 | 0.15x |
| users.json | json | 15.904 | 16.164 | 16.661 | 61.672 | 0.08x |
| flat.json | strata | 0.189 | 0.201 | 0.218 | 80.062 | 1.00x |
| flat.json | orjson | 0.235 | 0.245 | 0.267 | 80.062 | 0.82x |
| flat.json | msgspec | 0.298 | 0.314 | 0.345 | 80.062 | 0.64x |
| flat.json | ujson | 0.744 | 0.767 | 0.830 | 80.062 | 0.26x |
| flat.json | json | 1.326 | 1.411 | 1.538 | 80.062 | 0.14x |
| nested.json | strata | 0.118 | 0.125 | 0.141 | 80.062 | 1.00x |
| nested.json | orjson | 0.200 | 0.209 | 0.235 | 80.062 | 0.60x |
| nested.json | msgspec | 0.267 | 0.284 | 0.361 | 80.062 | 0.44x |
| nested.json | ujson | 0.821 | 0.854 | 0.950 | 80.062 | 0.15x |
| nested.json | json | 1.692 | 1.813 | 1.930 | 80.062 | 0.07x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.128 | 6.404 | 6.473 | 76.578 | 1.00x |
| users.json | orjson | 8.243 | 8.448 | 8.522 | 76.578 | 0.76x |
| users.json | msgspec | 8.448 | 8.695 | 8.896 | 76.578 | 0.74x |
| users.json | ujson | 11.386 | 11.791 | 12.167 | 76.578 | 0.54x |
| users.json | json | 15.798 | 16.332 | 16.772 | 76.578 | 0.39x |
| flat.json | strata | 0.640 | 0.673 | 0.695 | 80.062 | 1.00x |
| flat.json | orjson | 0.734 | 0.761 | 0.805 | 80.062 | 0.88x |
| flat.json | msgspec | 0.773 | 0.815 | 0.879 | 80.062 | 0.83x |
| flat.json | ujson | 1.140 | 1.182 | 1.277 | 80.062 | 0.57x |
| flat.json | json | 1.524 | 1.574 | 1.689 | 80.062 | 0.43x |
| nested.json | strata | 0.557 | 0.589 | 0.663 | 80.062 | 1.00x |
| nested.json | orjson | 0.715 | 0.733 | 0.780 | 80.062 | 0.80x |
| nested.json | msgspec | 0.702 | 0.737 | 0.794 | 80.062 | 0.80x |
| nested.json | ujson | 1.000 | 1.030 | 1.261 | 80.062 | 0.57x |
| nested.json | json | 1.510 | 1.540 | 1.627 | 80.062 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.758 | 1.791 | 1.908 | 78.188 | 1.00x |
| users.json | orjson | 2.446 | 2.572 | 2.711 | 78.188 | 0.70x |
| users.json | msgspec | 3.009 | 3.135 | 3.391 | 78.188 | 0.57x |
| users.json | ujson | 9.089 | 9.252 | 9.335 | 78.188 | 0.19x |
| users.json | json | 16.237 | 16.460 | 16.968 | 78.188 | 0.11x |
| flat.json | strata | 0.394 | 0.409 | 0.448 | 80.062 | 1.00x |
| flat.json | orjson | 0.433 | 0.482 | 0.553 | 80.062 | 0.85x |
| flat.json | msgspec | 0.505 | 0.535 | 0.572 | 80.062 | 0.76x |
| flat.json | ujson | 0.936 | 1.025 | 1.078 | 80.062 | 0.40x |
| flat.json | json | 1.527 | 1.624 | 1.776 | 80.062 | 0.25x |
| nested.json | strata | 0.300 | 0.336 | 0.374 | 80.062 | 1.00x |
| nested.json | orjson | 0.384 | 0.439 | 0.509 | 80.062 | 0.77x |
| nested.json | msgspec | 0.467 | 0.505 | 0.582 | 80.062 | 0.67x |
| nested.json | ujson | 1.006 | 1.097 | 1.196 | 80.062 | 0.31x |
| nested.json | json | 1.843 | 1.969 | 2.371 | 80.062 | 0.17x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.062 | 0.071 | 0.094 | 78.609 | 1.00x |
| users.json $[*].id | jmespath | 0.279 | 0.314 | 0.341 | 78.609 | 0.23x |
| users.json $[*].id | jsonpath-ng | 1.418 | 1.548 | 1.648 | 78.609 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.349 | 0.423 | 0.494 | 79.094 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.641 | 1.790 | 1.916 | 79.094 | 0.24x |
| users.json $[*].orders[*].total | jsonpath-ng | 9.984 | 10.156 | 10.947 | 79.094 | 0.04x |
| users.json $..total | strata | 1.402 | 1.454 | 1.796 | 79.125 | 1.00x |
| users.json $..total | jsonpath-ng | 191.639 | 193.581 | 195.242 | 79.125 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.334 | 3.399 | 3.504 | 78.656 | 1.00x |
| users.json $[*].id | orjson+jmespath | 8.771 | 8.945 | 9.350 | 78.656 | 0.38x |
| users.json $[*].id | orjson+jsonpath-ng | 10.031 | 10.236 | 10.779 | 78.656 | 0.33x |
| users.json $[*].orders[*].total | strata | 3.385 | 3.443 | 3.590 | 79.094 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 10.037 | 10.516 | 11.170 | 79.094 | 0.33x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 19.612 | 20.712 | 21.303 | 79.094 | 0.17x |
| users.json $..total | strata | 7.781 | 7.983 | 8.365 | 79.125 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 198.986 | 204.272 | 213.243 | 79.125 | 0.04x |

