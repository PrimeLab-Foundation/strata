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
| users.json | strata | 5.906 | 6.197 | 6.492 | 55.797 | 1.00x |
| users.json | orjson | 7.908 | 8.217 | 8.645 | 55.797 | 0.75x |
| users.json | msgspec | 8.001 | 8.405 | 8.726 | 55.797 | 0.74x |
| users.json | ujson | 10.980 | 11.314 | 12.424 | 55.797 | 0.55x |
| users.json | json | 15.205 | 15.802 | 16.647 | 55.797 | 0.39x |
| flat.json | strata | 0.593 | 0.615 | 0.640 | 76.984 | 1.00x |
| flat.json | orjson | 0.687 | 0.712 | 0.752 | 76.984 | 0.86x |
| flat.json | msgspec | 0.707 | 0.729 | 0.763 | 76.984 | 0.84x |
| flat.json | ujson | 1.058 | 1.093 | 1.233 | 76.984 | 0.56x |
| flat.json | json | 1.478 | 1.498 | 1.570 | 76.984 | 0.41x |
| nested.json | strata | 0.504 | 0.522 | 0.560 | 77.031 | 1.00x |
| nested.json | orjson | 0.631 | 0.644 | 7.914 | 77.031 | 0.81x |
| nested.json | msgspec | 0.635 | 0.654 | 0.706 | 77.031 | 0.80x |
| nested.json | ujson | 0.922 | 0.931 | 1.360 | 77.031 | 0.56x |
| nested.json | json | 1.454 | 1.476 | 1.504 | 77.031 | 0.35x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.345 | 1.440 | 1.705 | 61.234 | 1.00x |
| users.json | orjson | 2.074 | 2.146 | 2.342 | 61.234 | 0.67x |
| users.json | msgspec | 2.654 | 2.738 | 2.953 | 61.234 | 0.53x |
| users.json | ujson | 8.668 | 8.978 | 9.482 | 61.234 | 0.16x |
| users.json | json | 15.179 | 15.762 | 16.463 | 61.234 | 0.09x |
| flat.json | strata | 0.193 | 0.199 | 0.213 | 77.016 | 1.00x |
| flat.json | orjson | 0.238 | 0.245 | 0.253 | 77.016 | 0.81x |
| flat.json | msgspec | 0.297 | 0.307 | 0.330 | 77.016 | 0.65x |
| flat.json | ujson | 0.750 | 0.785 | 0.813 | 77.016 | 0.25x |
| flat.json | json | 1.354 | 1.414 | 1.535 | 77.016 | 0.14x |
| nested.json | strata | 0.118 | 0.122 | 0.132 | 77.203 | 1.00x |
| nested.json | orjson | 0.205 | 0.216 | 15.826 | 77.203 | 0.57x |
| nested.json | msgspec | 0.278 | 0.288 | 0.575 | 77.203 | 0.42x |
| nested.json | ujson | 0.916 | 0.924 | 3.106 | 77.203 | 0.13x |
| nested.json | json | 1.692 | 1.759 | 2.088 | 77.203 | 0.07x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.188 | 6.425 | 7.012 | 73.016 | 1.00x |
| users.json | orjson | 8.277 | 8.608 | 8.865 | 73.016 | 0.75x |
| users.json | msgspec | 8.361 | 9.172 | 9.458 | 73.016 | 0.70x |
| users.json | ujson | 11.680 | 12.166 | 12.614 | 73.016 | 0.53x |
| users.json | json | 15.625 | 16.354 | 17.197 | 73.016 | 0.39x |
| flat.json | strata | 0.684 | 0.721 | 0.972 | 77.016 | 1.00x |
| flat.json | orjson | 0.820 | 0.877 | 0.916 | 77.016 | 0.82x |
| flat.json | msgspec | 0.833 | 0.880 | 0.925 | 77.016 | 0.82x |
| flat.json | ujson | 1.218 | 1.278 | 1.324 | 77.016 | 0.56x |
| flat.json | json | 1.590 | 1.646 | 1.749 | 77.016 | 0.44x |
| nested.json | strata | 0.595 | 0.652 | 0.696 | 77.203 | 1.00x |
| nested.json | orjson | 0.774 | 0.797 | 0.830 | 77.203 | 0.82x |
| nested.json | msgspec | 0.728 | 0.796 | 0.870 | 77.203 | 0.82x |
| nested.json | ujson | 1.062 | 1.087 | 1.296 | 77.203 | 0.60x |
| nested.json | json | 1.609 | 1.652 | 1.951 | 77.203 | 0.39x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.831 | 1.930 | 2.894 | 73.500 | 1.00x |
| users.json | orjson | 2.545 | 2.664 | 2.962 | 73.500 | 0.72x |
| users.json | msgspec | 3.124 | 3.189 | 3.439 | 73.500 | 0.61x |
| users.json | ujson | 9.062 | 9.449 | 9.806 | 73.500 | 0.20x |
| users.json | json | 16.388 | 17.221 | 17.810 | 73.500 | 0.11x |
| flat.json | strata | 0.404 | 0.461 | 0.529 | 77.016 | 1.00x |
| flat.json | orjson | 0.477 | 0.522 | 0.570 | 77.016 | 0.88x |
| flat.json | msgspec | 0.530 | 0.583 | 0.653 | 77.016 | 0.79x |
| flat.json | ujson | 0.991 | 1.069 | 1.407 | 77.016 | 0.43x |
| flat.json | json | 1.637 | 1.716 | 2.039 | 77.016 | 0.27x |
| nested.json | strata | 0.317 | 0.411 | 0.626 | 77.203 | 1.00x |
| nested.json | orjson | 0.430 | 0.486 | 0.705 | 77.203 | 0.85x |
| nested.json | msgspec | 0.511 | 0.558 | 0.763 | 77.203 | 0.74x |
| nested.json | ujson | 1.129 | 1.193 | 1.471 | 77.203 | 0.34x |
| nested.json | json | 1.928 | 2.034 | 2.244 | 77.203 | 0.20x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.051 | 0.061 | 0.087 | 73.922 | 1.00x |
| users.json $[*].id | jmespath | 0.278 | 0.303 | 0.320 | 73.922 | 0.20x |
| users.json $[*].id | jsonpath-ng | 1.433 | 1.475 | 1.561 | 73.922 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.338 | 0.523 | 0.994 | 74.391 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.700 | 1.823 | 2.344 | 74.391 | 0.29x |
| users.json $[*].orders[*].total | jsonpath-ng | 9.643 | 10.384 | 11.155 | 74.391 | 0.05x |
| users.json $..total | strata | 1.374 | 1.422 | 1.808 | 74.438 | 1.00x |
| users.json $..total | jsonpath-ng | 188.685 | 191.779 | 197.335 | 74.438 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.345 | 3.425 | 3.574 | 74.000 | 1.00x |
| users.json $[*].id | orjson+jmespath | 8.595 | 9.040 | 9.717 | 74.000 | 0.38x |
| users.json $[*].id | orjson+jsonpath-ng | 9.808 | 10.494 | 11.045 | 74.000 | 0.33x |
| users.json $[*].orders[*].total | strata | 3.392 | 3.489 | 3.766 | 74.406 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 9.965 | 10.396 | 11.074 | 74.406 | 0.34x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 19.340 | 20.455 | 21.902 | 74.406 | 0.17x |
| users.json $..total | strata | 7.977 | 8.437 | 9.207 | 76.094 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 204.161 | 205.928 | 228.250 | 76.094 | 0.04x |

