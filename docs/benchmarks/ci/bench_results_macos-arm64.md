# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: a4d78c1
- python: 3.12.10
- implementation: CPython
- platform: macOS-26.5.2-arm64-arm-64bit
- machine: arm64
- processor: Apple M1 (Virtual)
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.161 | 9.924 | 15.326 | 63.453 | 1.00x |
| users.json | orjson | 11.758 | 14.459 | 22.964 | 63.453 | 0.69x |
| users.json | msgspec | 12.906 | 15.538 | 18.353 | 63.453 | 0.64x |
| users.json | ujson | 17.168 | 19.339 | 26.545 | 63.453 | 0.51x |
| users.json | pysimdjson | 165.597 | 192.288 | 201.595 | 63.453 | 0.05x |
| users.json | json | 18.823 | 24.844 | 34.004 | 63.453 | 0.40x |
| flat.json | strata | 0.666 | 0.789 | 1.114 | 94.766 | 1.00x |
| flat.json | orjson | 0.855 | 0.921 | 1.985 | 94.766 | 0.86x |
| flat.json | msgspec | 0.773 | 0.848 | 1.954 | 94.766 | 0.93x |
| flat.json | ujson | 1.372 | 1.536 | 2.674 | 94.766 | 0.51x |
| flat.json | pysimdjson | 16.834 | 20.872 | 25.650 | 94.766 | 0.04x |
| flat.json | json | 1.528 | 1.624 | 3.792 | 94.766 | 0.49x |
| nested.json | strata | 0.616 | 0.686 | 1.299 | 94.781 | 1.00x |
| nested.json | orjson | 0.820 | 1.021 | 2.139 | 94.781 | 0.67x |
| nested.json | msgspec | 0.742 | 0.853 | 1.746 | 94.781 | 0.80x |
| nested.json | ujson | 1.150 | 1.310 | 2.898 | 94.781 | 0.52x |
| nested.json | pysimdjson | 11.158 | 18.114 | 21.384 | 94.781 | 0.04x |
| nested.json | json | 1.563 | 2.122 | 16.253 | 94.781 | 0.32x |
| wide_arrays.json | strata | 4.211 | 5.591 | 10.912 | 97.422 | 1.00x |
| wide_arrays.json | orjson | 4.530 | 4.774 | 12.729 | 97.422 | 1.17x |
| wide_arrays.json | msgspec | 4.532 | 5.301 | 7.328 | 97.422 | 1.05x |
| wide_arrays.json | ujson | 6.233 | 8.892 | 38.551 | 97.422 | 0.63x |
| wide_arrays.json | pysimdjson | 83.971 | 92.502 | 103.854 | 97.422 | 0.06x |
| wide_arrays.json | json | 8.022 | 9.525 | 12.266 | 97.422 | 0.59x |
| mixed.json | strata | 0.163 | 0.173 | 0.438 | 97.453 | 1.00x |
| mixed.json | orjson | 0.187 | 0.221 | 0.531 | 97.453 | 0.79x |
| mixed.json | msgspec | 0.203 | 0.222 | 0.265 | 97.453 | 0.78x |
| mixed.json | ujson | 0.266 | 0.524 | 1.010 | 97.453 | 0.33x |
| mixed.json | pysimdjson | 2.701 | 2.921 | 3.574 | 97.453 | 0.06x |
| mixed.json | json | 0.381 | 0.396 | 0.565 | 97.453 | 0.44x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.968 | 2.437 | 5.534 | 82.422 | 1.00x |
| users.json | orjson | 2.575 | 2.870 | 5.869 | 82.422 | 0.85x |
| users.json | msgspec | 3.157 | 4.027 | 8.163 | 82.422 | 0.61x |
| users.json | ujson | 12.585 | 18.442 | 25.887 | 82.422 | 0.13x |
| users.json | json | 17.826 | 21.399 | 28.631 | 82.422 | 0.11x |
| flat.json | strata | 0.224 | 0.268 | 0.611 | 94.781 | 1.00x |
| flat.json | orjson | 0.267 | 0.313 | 0.882 | 94.781 | 0.85x |
| flat.json | msgspec | 0.338 | 0.370 | 0.876 | 94.781 | 0.72x |
| flat.json | ujson | 0.948 | 1.087 | 2.157 | 94.781 | 0.25x |
| flat.json | json | 1.465 | 1.702 | 3.331 | 94.781 | 0.16x |
| nested.json | strata | 0.170 | 0.206 | 0.578 | 94.781 | 1.00x |
| nested.json | orjson | 0.279 | 0.363 | 0.769 | 94.781 | 0.57x |
| nested.json | msgspec | 0.394 | 0.518 | 0.848 | 94.781 | 0.40x |
| nested.json | ujson | 1.225 | 1.527 | 2.890 | 94.781 | 0.13x |
| nested.json | json | 1.884 | 2.108 | 4.948 | 94.781 | 0.10x |
| wide_arrays.json | strata | 1.327 | 1.834 | 3.528 | 97.438 | 1.00x |
| wide_arrays.json | orjson | 1.729 | 2.365 | 3.670 | 97.438 | 0.78x |
| wide_arrays.json | msgspec | 2.404 | 3.427 | 6.421 | 97.438 | 0.54x |
| wide_arrays.json | ujson | 7.760 | 11.632 | 15.500 | 97.438 | 0.16x |
| wide_arrays.json | json | 13.161 | 17.512 | 23.402 | 97.438 | 0.10x |
| mixed.json | strata | 0.061 | 0.071 | 0.197 | 97.453 | 1.00x |
| mixed.json | orjson | 0.067 | 0.071 | 0.440 | 97.453 | 1.00x |
| mixed.json | msgspec | 0.068 | 0.076 | 0.089 | 97.453 | 0.93x |
| mixed.json | ujson | 0.242 | 0.284 | 0.637 | 97.453 | 0.25x |
| mixed.json | json | 0.403 | 0.484 | 0.900 | 97.453 | 0.15x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.228 | 12.063 | 88.699 | 92.688 | 1.00x |
| users.json | orjson | 11.489 | 14.275 | 43.082 | 92.688 | 0.85x |
| users.json | msgspec | 11.768 | 18.182 | 28.517 | 92.688 | 0.66x |
| users.json | ujson | 19.231 | 24.481 | 33.839 | 92.688 | 0.49x |
| users.json | json | 18.187 | 22.971 | 42.759 | 92.688 | 0.53x |
| flat.json | strata | 0.737 | 0.873 | 1.806 | 94.781 | 1.00x |
| flat.json | orjson | 1.060 | 1.196 | 3.007 | 94.781 | 0.73x |
| flat.json | msgspec | 0.925 | 0.980 | 1.612 | 94.781 | 0.89x |
| flat.json | ujson | 1.339 | 1.437 | 3.439 | 94.781 | 0.61x |
| flat.json | json | 1.577 | 1.765 | 4.289 | 94.781 | 0.49x |
| nested.json | strata | 0.739 | 0.941 | 1.513 | 94.781 | 1.00x |
| nested.json | orjson | 1.102 | 1.354 | 2.755 | 94.781 | 0.70x |
| nested.json | msgspec | 0.926 | 1.204 | 2.046 | 94.781 | 0.78x |
| nested.json | ujson | 1.186 | 1.591 | 3.211 | 94.781 | 0.59x |
| nested.json | json | 1.832 | 2.292 | 4.756 | 94.781 | 0.41x |
| wide_arrays.json | strata | 3.991 | 6.617 | 10.889 | 97.438 | 1.00x |
| wide_arrays.json | orjson | 4.225 | 5.451 | 7.552 | 97.438 | 1.21x |
| wide_arrays.json | msgspec | 4.914 | 5.778 | 10.166 | 97.438 | 1.15x |
| wide_arrays.json | ujson | 6.628 | 9.192 | 24.591 | 97.438 | 0.72x |
| wide_arrays.json | json | 8.155 | 11.046 | 18.334 | 97.438 | 0.60x |
| mixed.json | strata | 0.248 | 0.285 | 0.621 | 97.453 | 1.00x |
| mixed.json | orjson | 0.340 | 0.675 | 1.377 | 97.453 | 0.42x |
| mixed.json | msgspec | 0.344 | 0.381 | 0.581 | 97.453 | 0.75x |
| mixed.json | ujson | 0.383 | 0.482 | 1.065 | 97.453 | 0.59x |
| mixed.json | json | 0.504 | 0.565 | 1.324 | 97.453 | 0.50x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 8.898 | 10.463 | 14.471 | 94.766 | 1.00x |
| users.ndjson | orjson | 14.275 | 18.130 | 24.938 | 94.766 | 0.58x |
| users.ndjson | msgspec | 14.393 | 17.911 | 22.943 | 94.766 | 0.58x |
| users.ndjson | ujson | 15.449 | 22.318 | 34.737 | 94.766 | 0.47x |
| users.ndjson | json | 25.536 | 27.886 | 38.120 | 94.766 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.581 | 3.184 | 7.341 | 90.031 | 1.00x |
| users.json | orjson | 3.124 | 4.448 | 6.451 | 90.031 | 0.72x |
| users.json | msgspec | 4.108 | 5.120 | 6.855 | 90.031 | 0.62x |
| users.json | ujson | 12.283 | 16.328 | 22.776 | 90.031 | 0.20x |
| users.json | json | 19.448 | 23.399 | 31.285 | 90.031 | 0.14x |
| flat.json | strata | 0.568 | 0.768 | 1.381 | 94.781 | 1.00x |
| flat.json | orjson | 0.622 | 0.721 | 1.497 | 94.781 | 1.07x |
| flat.json | msgspec | 0.661 | 0.704 | 2.111 | 94.781 | 1.09x |
| flat.json | ujson | 1.329 | 1.545 | 3.311 | 94.781 | 0.50x |
| flat.json | json | 1.834 | 2.156 | 4.478 | 94.781 | 0.36x |
| nested.json | strata | 0.399 | 0.465 | 1.148 | 94.781 | 1.00x |
| nested.json | orjson | 0.518 | 0.721 | 1.256 | 94.781 | 0.65x |
| nested.json | msgspec | 0.649 | 0.968 | 1.949 | 94.781 | 0.48x |
| nested.json | ujson | 1.453 | 1.656 | 3.740 | 94.781 | 0.28x |
| nested.json | json | 2.083 | 2.295 | 7.270 | 94.781 | 0.20x |
| wide_arrays.json | strata | 1.686 | 2.036 | 8.025 | 97.438 | 1.00x |
| wide_arrays.json | orjson | 2.199 | 2.558 | 10.103 | 97.438 | 0.80x |
| wide_arrays.json | msgspec | 3.142 | 3.483 | 15.221 | 97.438 | 0.58x |
| wide_arrays.json | ujson | 8.584 | 8.704 | 9.872 | 97.438 | 0.23x |
| wide_arrays.json | json | 13.976 | 15.401 | 21.396 | 97.438 | 0.13x |
| mixed.json | strata | 0.259 | 0.313 | 0.396 | 97.453 | 1.00x |
| mixed.json | orjson | 0.315 | 0.374 | 0.480 | 97.453 | 0.84x |
| mixed.json | msgspec | 0.304 | 0.373 | 0.652 | 97.453 | 0.84x |
| mixed.json | ujson | 0.540 | 0.593 | 0.692 | 97.453 | 0.53x |
| mixed.json | json | 0.707 | 0.786 | 0.850 | 97.453 | 0.40x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.137 | 0.180 | 0.321 | 90.094 | 1.00x |
| users.json $[*].id | jmespath | 0.388 | 0.441 | 0.535 | 90.094 | 0.41x |
| users.json $[*].id | jsonpath-ng | 1.661 | 1.980 | 3.309 | 90.094 | 0.09x |
| users.json $[*].orders[*].total | strata | 0.687 | 0.900 | 2.083 | 90.266 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.885 | 2.820 | 6.579 | 90.266 | 0.32x |
| users.json $[*].orders[*].total | jsonpath-ng | 13.172 | 16.360 | 22.189 | 90.266 | 0.06x |
| users.json $..total | strata | 1.464 | 1.678 | 4.317 | 91.109 | 1.00x |
| users.json $..total | jsonpath-ng | 269.764 | 317.968 | 341.108 | 91.109 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.973 | 4.799 | 7.012 | 90.141 | 1.00x |
| users.json $[*].id | orjson+jmespath | 11.832 | 16.778 | 25.173 | 90.141 | 0.29x |
| users.json $[*].id | orjson+jsonpath-ng | 12.907 | 15.244 | 28.392 | 90.141 | 0.31x |
| users.json $[*].orders[*].total | strata | 4.364 | 4.988 | 7.200 | 91.109 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 14.582 | 18.110 | 24.949 | 91.109 | 0.28x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 36.608 | 43.686 | 55.766 | 91.109 | 0.11x |
| users.json $..total | strata | 10.286 | 14.187 | 19.398 | 91.109 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 286.907 | 313.287 | 339.959 | 91.109 | 0.05x |

