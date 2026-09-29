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
| users.json | strata | 5.907 | 6.431 | 7.052 | 57.156 | 1.00x |
| users.json | orjson | 8.108 | 8.811 | 9.197 | 57.156 | 0.73x |
| users.json | msgspec | 8.165 | 8.648 | 9.624 | 57.156 | 0.74x |
| users.json | ujson | 10.843 | 12.019 | 12.617 | 57.156 | 0.54x |
| users.json | json | 15.683 | 16.386 | 17.479 | 57.156 | 0.39x |
| flat.json | strata | 0.581 | 0.604 | 0.623 | 75.688 | 1.00x |
| flat.json | orjson | 0.660 | 0.669 | 0.759 | 75.688 | 0.90x |
| flat.json | msgspec | 0.697 | 0.725 | 0.775 | 75.688 | 0.83x |
| flat.json | ujson | 1.045 | 1.139 | 1.285 | 75.688 | 0.53x |
| flat.json | json | 1.447 | 1.538 | 1.592 | 75.688 | 0.39x |
| nested.json | strata | 0.492 | 0.515 | 0.630 | 75.766 | 1.00x |
| nested.json | orjson | 0.605 | 0.628 | 0.758 | 75.766 | 0.82x |
| nested.json | msgspec | 0.625 | 0.649 | 0.701 | 75.766 | 0.79x |
| nested.json | ujson | 0.878 | 0.905 | 1.179 | 75.766 | 0.57x |
| nested.json | json | 1.412 | 1.475 | 1.696 | 75.766 | 0.35x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.323 | 1.417 | 1.545 | 61.703 | 1.00x |
| users.json | orjson | 2.059 | 2.204 | 2.392 | 61.703 | 0.64x |
| users.json | msgspec | 2.640 | 2.710 | 2.775 | 61.703 | 0.52x |
| users.json | ujson | 8.620 | 8.834 | 9.281 | 61.703 | 0.16x |
| users.json | json | 15.693 | 16.355 | 16.843 | 61.703 | 0.09x |
| flat.json | strata | 0.193 | 0.201 | 0.220 | 75.766 | 1.00x |
| flat.json | orjson | 0.238 | 0.254 | 0.280 | 75.766 | 0.79x |
| flat.json | msgspec | 0.304 | 0.324 | 0.359 | 75.766 | 0.62x |
| flat.json | ujson | 0.753 | 0.786 | 0.931 | 75.766 | 0.26x |
| flat.json | json | 1.413 | 1.434 | 1.627 | 75.766 | 0.14x |
| nested.json | strata | 0.123 | 0.131 | 0.153 | 75.781 | 1.00x |
| nested.json | orjson | 0.207 | 0.219 | 0.292 | 75.781 | 0.59x |
| nested.json | msgspec | 0.277 | 0.297 | 0.332 | 75.781 | 0.44x |
| nested.json | ujson | 0.848 | 0.888 | 0.940 | 75.781 | 0.15x |
| nested.json | json | 1.694 | 1.802 | 2.154 | 75.781 | 0.07x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.445 | 6.658 | 7.236 | 75.328 | 1.00x |
| users.json | orjson | 8.450 | 8.920 | 9.637 | 75.328 | 0.75x |
| users.json | msgspec | 8.665 | 8.953 | 10.439 | 75.328 | 0.74x |
| users.json | ujson | 11.866 | 12.677 | 13.118 | 75.328 | 0.53x |
| users.json | json | 16.043 | 16.617 | 17.203 | 75.328 | 0.40x |
| flat.json | strata | 0.653 | 0.706 | 0.776 | 75.766 | 1.00x |
| flat.json | orjson | 0.743 | 0.844 | 1.093 | 75.766 | 0.84x |
| flat.json | msgspec | 0.794 | 0.860 | 0.965 | 75.766 | 0.82x |
| flat.json | ujson | 1.155 | 1.235 | 1.430 | 75.766 | 0.57x |
| flat.json | json | 1.535 | 1.614 | 1.782 | 75.766 | 0.44x |
| nested.json | strata | 0.576 | 0.633 | 0.721 | 75.781 | 1.00x |
| nested.json | orjson | 0.709 | 0.763 | 0.837 | 75.781 | 0.83x |
| nested.json | msgspec | 0.693 | 0.752 | 0.853 | 75.781 | 0.84x |
| nested.json | ujson | 0.993 | 1.080 | 1.280 | 75.781 | 0.59x |
| nested.json | json | 1.513 | 1.652 | 1.751 | 75.781 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.840 | 1.866 | 1.931 | 75.344 | 1.00x |
| users.json | orjson | 2.493 | 2.714 | 2.774 | 75.344 | 0.69x |
| users.json | msgspec | 3.048 | 3.338 | 3.763 | 75.344 | 0.56x |
| users.json | ujson | 9.093 | 9.460 | 9.747 | 75.344 | 0.20x |
| users.json | json | 16.262 | 16.815 | 17.469 | 75.344 | 0.11x |
| flat.json | strata | 0.360 | 0.466 | 0.528 | 75.766 | 1.00x |
| flat.json | orjson | 0.441 | 0.502 | 0.658 | 75.766 | 0.93x |
| flat.json | msgspec | 0.497 | 0.567 | 0.674 | 75.766 | 0.82x |
| flat.json | ujson | 0.999 | 1.081 | 1.185 | 75.766 | 0.43x |
| flat.json | json | 1.562 | 1.719 | 2.056 | 75.766 | 0.27x |
| nested.json | strata | 0.311 | 0.381 | 0.535 | 75.781 | 1.00x |
| nested.json | orjson | 0.386 | 0.453 | 0.653 | 75.781 | 0.84x |
| nested.json | msgspec | 0.508 | 0.593 | 0.689 | 75.781 | 0.64x |
| nested.json | ujson | 1.077 | 1.132 | 1.284 | 75.781 | 0.34x |
| nested.json | json | 1.918 | 2.064 | 2.336 | 75.781 | 0.18x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.053 | 0.080 | 0.164 | 75.750 | 1.00x |
| users.json $[*].id | jmespath | 0.298 | 0.351 | 0.529 | 75.750 | 0.23x |
| users.json $[*].id | jsonpath-ng | 1.459 | 1.557 | 1.741 | 75.750 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.342 | 0.490 | 0.885 | 76.219 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.673 | 1.815 | 1.983 | 76.219 | 0.27x |
| users.json $[*].orders[*].total | jsonpath-ng | 9.628 | 10.363 | 12.031 | 76.219 | 0.05x |
| users.json $..total | strata | 1.378 | 1.415 | 1.755 | 76.281 | 1.00x |
| users.json $..total | jsonpath-ng | 187.881 | 189.899 | 193.283 | 76.281 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.411 | 3.461 | 3.630 | 75.844 | 1.00x |
| users.json $[*].id | orjson+jmespath | 8.760 | 9.077 | 9.903 | 75.844 | 0.38x |
| users.json $[*].id | orjson+jsonpath-ng | 9.937 | 10.594 | 10.988 | 75.844 | 0.33x |
| users.json $[*].orders[*].total | strata | 3.446 | 3.539 | 3.784 | 76.219 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 10.242 | 10.768 | 11.861 | 76.219 | 0.33x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 20.105 | 21.744 | 24.252 | 76.219 | 0.16x |
| users.json $..total | strata | 7.804 | 8.399 | 9.242 | 74.797 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 201.810 | 202.935 | 208.097 | 74.797 | 0.04x |

