# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c16eaa65c61f1cf9d4fac46e9f6d8e6b9f3b087f
- python: 3.12.10
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit
- machine: arm64
- processor: Apple M1 (Virtual)
- compiler_flags: -fno-strict-overflow -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -O3 -Wall -arch arm64 -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -march=native -flto=thin -fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata -bundle -undefined (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.397 | 6.667 | 9.592 | 67.859 | 1.00x |
| users.json | orjson | 9.901 | 10.633 | 11.318 | 67.859 | 0.63x |
| users.json | msgspec | 9.430 | 10.052 | 13.444 | 67.859 | 0.66x |
| users.json | ujson | 12.906 | 13.485 | 20.603 | 67.859 | 0.49x |
| users.json | pysimdjson | 129.258 | 133.578 | 147.015 | 67.859 | 0.05x |
| users.json | json | 15.750 | 16.227 | 23.723 | 67.859 | 0.41x |
| flat.json | strata | 0.671 | 0.995 | 1.527 | 90.203 | 1.00x |
| flat.json | orjson | 0.883 | 1.565 | 2.970 | 90.203 | 0.64x |
| flat.json | msgspec | 0.804 | 1.351 | 4.359 | 90.203 | 0.74x |
| flat.json | ujson | 1.524 | 2.050 | 3.968 | 90.203 | 0.49x |
| flat.json | pysimdjson | 13.745 | 25.762 | 35.194 | 90.203 | 0.04x |
| flat.json | json | 1.728 | 2.374 | 3.367 | 90.203 | 0.42x |
| nested.json | strata | 0.567 | 0.604 | 1.645 | 90.203 | 1.00x |
| nested.json | orjson | 0.799 | 0.859 | 2.255 | 90.203 | 0.70x |
| nested.json | msgspec | 0.742 | 1.120 | 3.042 | 90.203 | 0.54x |
| nested.json | ujson | 1.041 | 1.887 | 4.450 | 90.203 | 0.32x |
| nested.json | pysimdjson | 11.410 | 14.760 | 21.622 | 90.203 | 0.04x |
| nested.json | json | 1.530 | 2.558 | 4.862 | 90.203 | 0.24x |
| wide_arrays.json | strata | 3.421 | 3.618 | 4.499 | 92.453 | 1.00x |
| wide_arrays.json | orjson | 4.112 | 4.793 | 11.248 | 92.453 | 0.75x |
| wide_arrays.json | msgspec | 4.365 | 4.818 | 8.822 | 92.453 | 0.75x |
| wide_arrays.json | ujson | 5.736 | 6.297 | 11.004 | 92.453 | 0.57x |
| wide_arrays.json | pysimdjson | 68.473 | 74.193 | 100.630 | 92.453 | 0.05x |
| wide_arrays.json | json | 7.276 | 8.735 | 15.307 | 92.453 | 0.41x |
| mixed.json | strata | 0.129 | 0.153 | 0.198 | 92.609 | 1.00x |
| mixed.json | orjson | 0.168 | 0.193 | 0.236 | 92.609 | 0.79x |
| mixed.json | msgspec | 0.183 | 0.192 | 0.246 | 92.609 | 0.80x |
| mixed.json | ujson | 0.251 | 0.334 | 0.429 | 92.609 | 0.46x |
| mixed.json | pysimdjson | 2.586 | 2.896 | 3.822 | 92.609 | 0.05x |
| mixed.json | json | 0.336 | 0.364 | 0.450 | 92.609 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.600 | 1.641 | 2.941 | 70.969 | 1.00x |
| users.json | orjson | 2.440 | 2.532 | 6.149 | 70.969 | 0.65x |
| users.json | msgspec | 3.052 | 3.117 | 3.486 | 70.969 | 0.53x |
| users.json | ujson | 9.158 | 9.331 | 10.117 | 70.969 | 0.18x |
| users.json | json | 16.117 | 17.044 | 17.434 | 70.969 | 0.10x |
| flat.json | strata | 0.243 | 0.411 | 0.993 | 90.203 | 1.00x |
| flat.json | orjson | 0.291 | 0.374 | 1.307 | 90.203 | 1.10x |
| flat.json | msgspec | 0.383 | 0.579 | 1.221 | 90.203 | 0.71x |
| flat.json | ujson | 0.872 | 1.082 | 1.667 | 90.203 | 0.38x |
| flat.json | json | 1.541 | 3.853 | 7.201 | 90.203 | 0.11x |
| nested.json | strata | 0.144 | 0.188 | 0.414 | 90.203 | 1.00x |
| nested.json | orjson | 0.248 | 0.262 | 0.783 | 90.203 | 0.72x |
| nested.json | msgspec | 0.311 | 0.509 | 2.136 | 90.203 | 0.37x |
| nested.json | ujson | 0.910 | 1.099 | 2.446 | 90.203 | 0.17x |
| nested.json | json | 1.773 | 2.338 | 7.005 | 90.203 | 0.08x |
| wide_arrays.json | strata | 1.303 | 1.694 | 3.637 | 92.594 | 1.00x |
| wide_arrays.json | orjson | 1.573 | 1.815 | 4.593 | 92.594 | 0.93x |
| wide_arrays.json | msgspec | 2.320 | 2.476 | 5.813 | 92.594 | 0.68x |
| wide_arrays.json | ujson | 5.016 | 5.498 | 7.790 | 92.594 | 0.31x |
| wide_arrays.json | json | 12.264 | 13.130 | 17.970 | 92.594 | 0.13x |
| mixed.json | strata | 0.044 | 0.057 | 0.077 | 92.609 | 1.00x |
| mixed.json | orjson | 0.050 | 0.058 | 0.070 | 92.609 | 0.98x |
| mixed.json | msgspec | 0.059 | 0.075 | 0.199 | 92.609 | 0.76x |
| mixed.json | ujson | 0.182 | 0.198 | 0.285 | 92.609 | 0.29x |
| mixed.json | json | 0.368 | 0.390 | 0.845 | 92.609 | 0.15x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.864 | 7.264 | 7.653 | 82.766 | 1.00x |
| users.json | orjson | 10.410 | 10.744 | 17.572 | 82.766 | 0.68x |
| users.json | msgspec | 10.091 | 10.649 | 11.212 | 82.766 | 0.68x |
| users.json | ujson | 13.932 | 15.045 | 18.694 | 82.766 | 0.48x |
| users.json | json | 16.437 | 17.079 | 17.816 | 82.766 | 0.43x |
| flat.json | strata | 0.794 | 0.907 | 2.417 | 90.203 | 1.00x |
| flat.json | orjson | 1.076 | 1.655 | 3.226 | 90.203 | 0.55x |
| flat.json | msgspec | 0.980 | 1.114 | 2.522 | 90.203 | 0.81x |
| flat.json | ujson | 1.424 | 2.276 | 4.560 | 90.203 | 0.40x |
| flat.json | json | 1.765 | 2.971 | 4.658 | 90.203 | 0.31x |
| nested.json | strata | 0.668 | 0.738 | 1.994 | 90.203 | 1.00x |
| nested.json | orjson | 1.014 | 1.183 | 3.283 | 90.203 | 0.62x |
| nested.json | msgspec | 0.879 | 0.976 | 1.805 | 90.203 | 0.76x |
| nested.json | ujson | 1.247 | 1.595 | 3.434 | 90.203 | 0.46x |
| nested.json | json | 1.711 | 1.881 | 2.888 | 90.203 | 0.39x |
| wide_arrays.json | strata | 3.283 | 3.437 | 7.753 | 92.594 | 1.00x |
| wide_arrays.json | orjson | 4.135 | 4.203 | 5.330 | 92.594 | 0.82x |
| wide_arrays.json | msgspec | 4.549 | 4.718 | 8.194 | 92.594 | 0.73x |
| wide_arrays.json | ujson | 5.902 | 6.187 | 12.891 | 92.594 | 0.56x |
| wide_arrays.json | json | 7.443 | 7.670 | 16.466 | 92.594 | 0.45x |
| mixed.json | strata | 0.178 | 0.217 | 0.378 | 92.609 | 1.00x |
| mixed.json | orjson | 0.325 | 0.381 | 0.527 | 92.609 | 0.57x |
| mixed.json | msgspec | 0.258 | 0.294 | 0.412 | 92.609 | 0.74x |
| mixed.json | ujson | 0.333 | 0.359 | 0.634 | 92.609 | 0.60x |
| mixed.json | json | 0.427 | 0.445 | 0.919 | 92.609 | 0.49x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 8.103 | 12.403 | 27.533 | 90.203 | 1.00x |
| users.ndjson | orjson | 13.882 | 20.318 | 37.109 | 90.203 | 0.61x |
| users.ndjson | msgspec | 13.876 | 18.994 | 30.615 | 90.203 | 0.65x |
| users.ndjson | ujson | 15.533 | 21.085 | 64.748 | 90.203 | 0.59x |
| users.ndjson | json | 22.181 | 33.410 | 40.308 | 90.203 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.906 | 2.232 | 3.591 | 84.422 | 1.00x |
| users.json | orjson | 2.984 | 3.198 | 4.540 | 84.422 | 0.70x |
| users.json | msgspec | 3.539 | 3.866 | 4.025 | 84.422 | 0.58x |
| users.json | ujson | 10.168 | 10.610 | 10.833 | 84.422 | 0.21x |
| users.json | json | 16.817 | 17.796 | 19.196 | 84.422 | 0.13x |
| flat.json | strata | 0.500 | 0.670 | 0.896 | 90.203 | 1.00x |
| flat.json | orjson | 0.539 | 0.646 | 0.972 | 90.203 | 1.04x |
| flat.json | msgspec | 0.662 | 0.754 | 2.097 | 90.203 | 0.89x |
| flat.json | ujson | 1.138 | 1.502 | 2.987 | 90.203 | 0.45x |
| flat.json | json | 1.892 | 2.119 | 3.909 | 90.203 | 0.32x |
| nested.json | strata | 0.387 | 0.439 | 0.646 | 90.203 | 1.00x |
| nested.json | orjson | 0.494 | 0.566 | 1.050 | 90.203 | 0.78x |
| nested.json | msgspec | 0.608 | 0.773 | 1.000 | 90.203 | 0.57x |
| nested.json | ujson | 1.165 | 1.363 | 1.698 | 90.203 | 0.32x |
| nested.json | json | 2.058 | 2.469 | 3.344 | 90.203 | 0.18x |
| wide_arrays.json | strata | 1.665 | 1.730 | 1.912 | 92.594 | 1.00x |
| wide_arrays.json | orjson | 2.152 | 2.335 | 2.467 | 92.594 | 0.74x |
| wide_arrays.json | msgspec | 2.918 | 3.066 | 3.554 | 92.594 | 0.56x |
| wide_arrays.json | ujson | 5.987 | 6.292 | 7.461 | 92.594 | 0.28x |
| wide_arrays.json | json | 13.343 | 14.069 | 17.702 | 92.594 | 0.12x |
| mixed.json | strata | 0.226 | 0.287 | 0.536 | 92.609 | 1.00x |
| mixed.json | orjson | 0.242 | 0.300 | 0.387 | 92.609 | 0.96x |
| mixed.json | msgspec | 0.271 | 0.346 | 0.439 | 92.609 | 0.83x |
| mixed.json | ujson | 0.412 | 0.478 | 0.643 | 92.609 | 0.60x |
| mixed.json | json | 0.601 | 0.682 | 0.944 | 92.609 | 0.42x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.065 | 0.086 | 0.106 | 84.469 | 1.00x |
| users.json $[*].id | jmespath | 0.308 | 0.356 | 0.405 | 84.469 | 0.24x |
| users.json $[*].id | jsonpath-ng | 1.593 | 1.671 | 1.773 | 84.469 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.721 | 1.158 | 3.495 | 84.812 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.187 | 3.041 | 6.741 | 84.812 | 0.38x |
| users.json $[*].orders[*].total | jsonpath-ng | 13.272 | 18.841 | 29.619 | 84.812 | 0.06x |
| users.json $..total | strata | 1.524 | 2.361 | 3.808 | 84.812 | 1.00x |
| users.json $..total | jsonpath-ng | 251.307 | 296.873 | 391.309 | 84.812 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.722 | 3.966 | 7.262 | 84.719 | 1.00x |
| users.json $[*].id | orjson+jmespath | 11.145 | 14.033 | 17.853 | 84.719 | 0.28x |
| users.json $[*].id | orjson+jsonpath-ng | 12.313 | 13.911 | 20.345 | 84.719 | 0.29x |
| users.json $[*].orders[*].total | strata | 3.965 | 4.621 | 23.098 | 84.812 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 13.367 | 14.374 | 36.635 | 84.812 | 0.32x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 28.855 | 31.064 | 99.934 | 84.812 | 0.15x |
| users.json $..total | strata | 9.438 | 12.687 | 16.177 | 84.828 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 257.017 | 315.573 | 363.614 | 84.828 | 0.04x |

