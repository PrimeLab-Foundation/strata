# Benchmark results - pgo

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 10521a972a612b42f87c1a42b0774904b9cb4699
- python: 3.14.7
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit-Mach-O
- machine: arm64
- processor: Apple M1 Max
- compiler_flags: -fno-strict-overflow -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -O3 -Wall -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -march=native -flto=thin -fprofile-use=/Users/borysbardysh/worktrees/strata/m15-pgo/build/pgo/strata.profdata -bundle -undefined (22 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 5.986 | 6.247 | 6.530 | 56.984 | 1.00x |
| users.json | orjson | 7.937 | 8.384 | 8.705 | 56.984 | 0.75x |
| users.json | msgspec | 8.125 | 8.467 | 8.984 | 56.984 | 0.74x |
| users.json | ujson | 11.020 | 11.508 | 11.839 | 56.984 | 0.54x |
| users.json | json | 15.418 | 16.076 | 17.036 | 56.984 | 0.39x |
| flat.json | strata | 0.554 | 0.568 | 0.676 | 76.594 | 1.00x |
| flat.json | orjson | 0.639 | 0.648 | 0.673 | 76.594 | 0.88x |
| flat.json | msgspec | 0.697 | 0.708 | 0.760 | 76.594 | 0.80x |
| flat.json | ujson | 1.030 | 1.059 | 1.178 | 76.594 | 0.54x |
| flat.json | json | 1.422 | 1.447 | 1.539 | 76.594 | 0.39x |
| nested.json | strata | 0.516 | 0.520 | 0.536 | 76.609 | 1.00x |
| nested.json | orjson | 0.621 | 0.633 | 0.679 | 76.609 | 0.82x |
| nested.json | msgspec | 0.624 | 0.638 | 0.659 | 76.609 | 0.82x |
| nested.json | ujson | 0.905 | 0.918 | 0.996 | 76.609 | 0.57x |
| nested.json | json | 1.430 | 1.447 | 1.679 | 76.609 | 0.36x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.331 | 1.359 | 1.451 | 60.062 | 1.00x |
| users.json | orjson | 2.057 | 2.083 | 2.252 | 60.062 | 0.65x |
| users.json | msgspec | 2.609 | 2.681 | 2.775 | 60.062 | 0.51x |
| users.json | ujson | 8.751 | 8.852 | 9.190 | 60.062 | 0.15x |
| users.json | json | 15.724 | 16.274 | 16.570 | 60.062 | 0.08x |
| flat.json | strata | 0.191 | 0.196 | 0.221 | 76.609 | 1.00x |
| flat.json | orjson | 0.234 | 0.239 | 0.248 | 76.609 | 0.82x |
| flat.json | msgspec | 0.296 | 0.298 | 0.313 | 76.609 | 0.66x |
| flat.json | ujson | 0.748 | 0.756 | 0.770 | 76.609 | 0.26x |
| flat.json | json | 1.336 | 1.345 | 1.374 | 76.609 | 0.15x |
| nested.json | strata | 0.117 | 0.120 | 0.136 | 76.703 | 1.00x |
| nested.json | orjson | 0.202 | 0.204 | 0.221 | 76.703 | 0.59x |
| nested.json | msgspec | 0.271 | 0.278 | 0.289 | 76.703 | 0.43x |
| nested.json | ujson | 0.819 | 0.833 | 0.862 | 76.703 | 0.14x |
| nested.json | json | 1.647 | 1.669 | 2.073 | 76.703 | 0.07x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.227 | 6.383 | 6.694 | 75.109 | 1.00x |
| users.json | orjson | 8.319 | 8.445 | 8.611 | 75.109 | 0.76x |
| users.json | msgspec | 8.424 | 8.522 | 8.935 | 75.109 | 0.75x |
| users.json | ujson | 11.477 | 11.962 | 12.174 | 75.109 | 0.53x |
| users.json | json | 15.681 | 16.055 | 16.433 | 75.109 | 0.40x |
| flat.json | strata | 0.636 | 0.653 | 0.700 | 76.609 | 1.00x |
| flat.json | orjson | 0.730 | 0.749 | 0.806 | 76.609 | 0.87x |
| flat.json | msgspec | 0.789 | 0.799 | 0.879 | 76.609 | 0.82x |
| flat.json | ujson | 1.153 | 1.187 | 1.396 | 76.609 | 0.55x |
| flat.json | json | 1.533 | 1.556 | 1.676 | 76.609 | 0.42x |
| nested.json | strata | 0.592 | 0.622 | 0.680 | 76.703 | 1.00x |
| nested.json | orjson | 0.720 | 0.764 | 0.810 | 76.703 | 0.81x |
| nested.json | msgspec | 0.729 | 0.751 | 0.803 | 76.703 | 0.83x |
| nested.json | ujson | 1.020 | 1.088 | 1.260 | 76.703 | 0.57x |
| nested.json | json | 1.506 | 1.612 | 1.678 | 76.703 | 0.39x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.679 | 1.728 | 1.844 | 75.109 | 1.00x |
| users.json | orjson | 2.428 | 2.483 | 2.640 | 75.109 | 0.70x |
| users.json | msgspec | 3.069 | 3.102 | 3.182 | 75.109 | 0.56x |
| users.json | ujson | 9.119 | 9.204 | 9.361 | 75.109 | 0.19x |
| users.json | json | 15.926 | 16.471 | 17.099 | 75.109 | 0.10x |
| flat.json | strata | 0.390 | 0.409 | 0.532 | 76.609 | 1.00x |
| flat.json | orjson | 0.433 | 0.471 | 0.614 | 76.609 | 0.87x |
| flat.json | msgspec | 0.495 | 0.548 | 0.667 | 76.609 | 0.75x |
| flat.json | ujson | 0.954 | 1.017 | 1.076 | 76.609 | 0.40x |
| flat.json | json | 1.569 | 1.605 | 1.812 | 76.609 | 0.25x |
| nested.json | strata | 0.300 | 0.320 | 0.387 | 76.703 | 1.00x |
| nested.json | orjson | 0.393 | 0.412 | 0.472 | 76.703 | 0.78x |
| nested.json | msgspec | 0.463 | 0.492 | 0.551 | 76.703 | 0.65x |
| nested.json | ujson | 1.020 | 1.077 | 1.117 | 76.703 | 0.30x |
| nested.json | json | 1.864 | 1.938 | 2.190 | 76.703 | 0.17x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.052 | 0.058 | 0.074 | 75.391 | 1.00x |
| users.json $[*].id | jmespath | 0.281 | 0.298 | 0.355 | 75.391 | 0.19x |
| users.json $[*].id | jsonpath-ng | 1.447 | 1.465 | 1.642 | 75.391 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.333 | 0.362 | 0.517 | 75.688 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.683 | 1.720 | 1.911 | 75.688 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 9.671 | 9.868 | 10.820 | 75.688 | 0.04x |
| users.json $..total | strata | 1.385 | 1.403 | 1.416 | 75.703 | 1.00x |
| users.json $..total | jsonpath-ng | 188.793 | 189.456 | 191.483 | 75.703 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.344 | 3.400 | 3.577 | 75.453 | 1.00x |
| users.json $[*].id | orjson+jmespath | 8.619 | 9.071 | 9.180 | 75.453 | 0.37x |
| users.json $[*].id | orjson+jsonpath-ng | 9.869 | 10.491 | 10.783 | 75.453 | 0.32x |
| users.json $[*].orders[*].total | strata | 3.391 | 3.420 | 3.554 | 75.688 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 10.027 | 10.260 | 10.743 | 75.688 | 0.33x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 19.563 | 19.860 | 21.045 | 75.688 | 0.17x |
| users.json $..total | strata | 7.627 | 7.822 | 8.089 | 75.703 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 198.843 | 201.241 | 203.540 | 75.703 | 0.04x |

