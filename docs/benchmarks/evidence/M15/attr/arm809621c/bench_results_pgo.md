# Benchmark results - pgo

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 809621c8cf8396034c0f74ac20827b62f8e518a0
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
| users.json | strata | 5.949 | 5.998 | 6.728 | 57.453 | 1.00x |
| users.json | orjson | 8.124 | 8.240 | 8.896 | 57.453 | 0.73x |
| users.json | msgspec | 8.245 | 8.347 | 8.702 | 57.453 | 0.72x |
| users.json | ujson | 10.995 | 11.226 | 12.374 | 57.453 | 0.53x |
| users.json | json | 15.677 | 15.932 | 16.334 | 57.453 | 0.38x |
| flat.json | strata | 0.580 | 0.599 | 0.627 | 76.719 | 1.00x |
| flat.json | orjson | 0.658 | 0.682 | 0.794 | 76.719 | 0.88x |
| flat.json | msgspec | 0.697 | 0.725 | 0.835 | 76.719 | 0.83x |
| flat.json | ujson | 1.050 | 1.127 | 1.263 | 76.719 | 0.53x |
| flat.json | json | 1.468 | 1.543 | 1.585 | 76.719 | 0.39x |
| nested.json | strata | 0.502 | 0.515 | 0.561 | 76.719 | 1.00x |
| nested.json | orjson | 0.611 | 0.627 | 0.801 | 76.719 | 0.82x |
| nested.json | msgspec | 0.621 | 0.639 | 0.700 | 76.719 | 0.81x |
| nested.json | ujson | 0.890 | 0.949 | 1.199 | 76.719 | 0.54x |
| nested.json | json | 1.435 | 1.469 | 1.728 | 76.719 | 0.35x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.332 | 1.386 | 1.488 | 61.516 | 1.00x |
| users.json | orjson | 2.084 | 2.134 | 2.221 | 61.516 | 0.65x |
| users.json | msgspec | 2.641 | 2.720 | 2.851 | 61.516 | 0.51x |
| users.json | ujson | 8.805 | 8.931 | 9.329 | 61.516 | 0.16x |
| users.json | json | 15.857 | 16.325 | 16.687 | 61.516 | 0.08x |
| flat.json | strata | 0.192 | 0.195 | 0.219 | 76.719 | 1.00x |
| flat.json | orjson | 0.236 | 0.243 | 0.256 | 76.719 | 0.80x |
| flat.json | msgspec | 0.295 | 0.311 | 0.334 | 76.719 | 0.63x |
| flat.json | ujson | 0.752 | 0.764 | 0.859 | 76.719 | 0.26x |
| flat.json | json | 1.329 | 1.425 | 1.578 | 76.719 | 0.14x |
| nested.json | strata | 0.117 | 0.124 | 0.135 | 76.797 | 1.00x |
| nested.json | orjson | 0.204 | 0.209 | 0.242 | 76.797 | 0.59x |
| nested.json | msgspec | 0.272 | 0.283 | 0.299 | 76.797 | 0.44x |
| nested.json | ujson | 0.859 | 0.878 | 0.920 | 76.797 | 0.14x |
| nested.json | json | 1.674 | 1.707 | 1.862 | 76.797 | 0.07x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.111 | 6.535 | 6.809 | 73.484 | 1.00x |
| users.json | orjson | 8.400 | 8.646 | 8.998 | 73.484 | 0.76x |
| users.json | msgspec | 8.422 | 8.879 | 9.417 | 73.484 | 0.74x |
| users.json | ujson | 11.845 | 12.049 | 12.360 | 73.484 | 0.54x |
| users.json | json | 15.753 | 16.148 | 16.801 | 73.484 | 0.40x |
| flat.json | strata | 0.658 | 0.678 | 0.715 | 76.719 | 1.00x |
| flat.json | orjson | 0.762 | 0.800 | 0.976 | 76.719 | 0.85x |
| flat.json | msgspec | 0.804 | 0.824 | 0.972 | 76.719 | 0.82x |
| flat.json | ujson | 1.190 | 1.240 | 1.373 | 76.719 | 0.55x |
| flat.json | json | 1.545 | 1.617 | 1.714 | 76.719 | 0.42x |
| nested.json | strata | 0.573 | 0.588 | 0.629 | 76.797 | 1.00x |
| nested.json | orjson | 0.704 | 0.729 | 0.797 | 76.797 | 0.81x |
| nested.json | msgspec | 0.697 | 0.740 | 0.798 | 76.797 | 0.79x |
| nested.json | ujson | 1.003 | 1.029 | 1.082 | 76.797 | 0.57x |
| nested.json | json | 1.515 | 1.559 | 1.649 | 76.797 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.727 | 1.802 | 1.905 | 73.516 | 1.00x |
| users.json | orjson | 2.488 | 2.593 | 2.648 | 73.516 | 0.69x |
| users.json | msgspec | 3.097 | 3.225 | 3.399 | 73.516 | 0.56x |
| users.json | ujson | 9.175 | 9.278 | 9.869 | 73.516 | 0.19x |
| users.json | json | 15.849 | 16.766 | 17.008 | 73.516 | 0.11x |
| flat.json | strata | 0.380 | 0.415 | 0.501 | 76.719 | 1.00x |
| flat.json | orjson | 0.417 | 0.480 | 0.647 | 76.719 | 0.87x |
| flat.json | msgspec | 0.479 | 0.513 | 0.578 | 76.719 | 0.81x |
| flat.json | ujson | 0.912 | 0.973 | 1.137 | 76.719 | 0.43x |
| flat.json | json | 1.520 | 1.607 | 1.897 | 76.719 | 0.26x |
| nested.json | strata | 0.292 | 0.331 | 0.408 | 76.812 | 1.00x |
| nested.json | orjson | 0.392 | 0.423 | 0.462 | 76.812 | 0.78x |
| nested.json | msgspec | 0.471 | 0.517 | 0.714 | 76.812 | 0.64x |
| nested.json | ujson | 1.050 | 1.094 | 1.180 | 76.812 | 0.30x |
| nested.json | json | 1.909 | 1.933 | 2.185 | 76.812 | 0.17x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.057 | 0.065 | 0.075 | 73.844 | 1.00x |
| users.json $[*].id | jmespath | 0.288 | 0.305 | 0.325 | 73.844 | 0.21x |
| users.json $[*].id | jsonpath-ng | 1.453 | 1.522 | 1.660 | 73.844 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.361 | 0.501 | 0.633 | 75.797 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.737 | 1.950 | 2.034 | 75.797 | 0.26x |
| users.json $[*].orders[*].total | jsonpath-ng | 10.045 | 10.785 | 11.206 | 75.797 | 0.05x |
| users.json $..total | strata | 1.394 | 1.478 | 1.529 | 75.828 | 1.00x |
| users.json $..total | jsonpath-ng | 188.061 | 190.692 | 194.083 | 75.828 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.389 | 3.524 | 3.696 | 75.562 | 1.00x |
| users.json $[*].id | orjson+jmespath | 8.676 | 9.119 | 9.290 | 75.562 | 0.39x |
| users.json $[*].id | orjson+jsonpath-ng | 10.043 | 10.383 | 11.004 | 75.562 | 0.34x |
| users.json $[*].orders[*].total | strata | 3.399 | 3.501 | 3.645 | 75.812 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 10.220 | 10.438 | 10.906 | 75.812 | 0.34x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 20.063 | 20.709 | 23.059 | 75.812 | 0.17x |
| users.json $..total | strata | 7.692 | 8.027 | 8.157 | 75.828 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 202.545 | 204.937 | 206.741 | 75.828 | 0.04x |

