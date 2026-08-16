# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 6309d05
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
| users.json | strata | 10.266 | 10.486 | 14.890 | 58.109 | 1.00x |
| users.json | orjson | 12.907 | 13.119 | 16.874 | 58.109 | 0.80x |
| users.json | msgspec | 12.891 | 13.091 | 15.953 | 58.109 | 0.80x |
| users.json | ujson | 17.856 | 18.387 | 27.130 | 58.109 | 0.57x |
| users.json | pysimdjson | 18.209 | 18.708 | 21.621 | 58.109 | 0.56x |
| users.json | json | 22.093 | 22.404 | 25.700 | 58.109 | 0.47x |
| flat.json | strata | 0.934 | 0.947 | 0.981 | 69.988 | 1.00x |
| flat.json | orjson | 1.003 | 1.009 | 1.020 | 69.988 | 0.94x |
| flat.json | msgspec | 1.033 | 1.052 | 1.107 | 69.988 | 0.90x |
| flat.json | ujson | 1.497 | 1.584 | 1.653 | 69.988 | 0.60x |
| flat.json | pysimdjson | 1.553 | 1.557 | 1.589 | 69.988 | 0.61x |
| flat.json | json | 1.882 | 1.899 | 2.008 | 69.988 | 0.50x |
| nested.json | strata | 0.865 | 0.870 | 0.891 | 69.988 | 1.00x |
| nested.json | orjson | 1.022 | 1.036 | 1.093 | 69.988 | 0.84x |
| nested.json | msgspec | 1.034 | 1.057 | 1.172 | 69.988 | 0.82x |
| nested.json | ujson | 1.506 | 1.578 | 1.788 | 69.988 | 0.55x |
| nested.json | pysimdjson | 1.411 | 1.458 | 1.510 | 69.988 | 0.60x |
| nested.json | json | 2.097 | 2.119 | 2.140 | 69.988 | 0.41x |
| wide_arrays.json | strata | 4.738 | 4.771 | 4.802 | 73.992 | 1.00x |
| wide_arrays.json | orjson | 4.953 | 5.020 | 5.266 | 73.992 | 0.95x |
| wide_arrays.json | msgspec | 5.470 | 5.536 | 5.563 | 73.992 | 0.86x |
| wide_arrays.json | ujson | 7.089 | 7.141 | 7.374 | 73.992 | 0.67x |
| wide_arrays.json | pysimdjson | 5.935 | 5.998 | 6.549 | 73.992 | 0.80x |
| wide_arrays.json | json | 9.711 | 9.788 | 10.016 | 73.992 | 0.49x |
| mixed.json | strata | 0.202 | 0.217 | 0.247 | 74.055 | 1.00x |
| mixed.json | orjson | 0.233 | 0.250 | 0.306 | 74.055 | 0.87x |
| mixed.json | msgspec | 0.242 | 0.260 | 0.311 | 74.055 | 0.84x |
| mixed.json | ujson | 0.308 | 0.334 | 0.432 | 74.055 | 0.65x |
| mixed.json | pysimdjson | 0.298 | 0.316 | 0.355 | 74.055 | 0.69x |
| mixed.json | json | 0.484 | 0.503 | 0.563 | 74.055 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.640 | 2.681 | 2.834 | 57.234 | 1.00x |
| users.json | orjson | 2.883 | 2.936 | 3.044 | 57.234 | 0.91x |
| users.json | msgspec | 3.932 | 3.951 | 4.170 | 57.234 | 0.68x |
| users.json | ujson | 14.182 | 14.317 | 15.801 | 57.234 | 0.19x |
| users.json | json | 22.029 | 22.182 | 37.594 | 57.234 | 0.12x |
| flat.json | strata | 0.264 | 0.272 | 0.288 | 69.988 | 1.00x |
| flat.json | orjson | 0.332 | 0.337 | 0.367 | 69.988 | 0.81x |
| flat.json | msgspec | 0.440 | 0.454 | 0.479 | 69.988 | 0.60x |
| flat.json | ujson | 1.290 | 1.328 | 1.496 | 69.988 | 0.20x |
| flat.json | json | 1.875 | 1.902 | 1.965 | 69.988 | 0.14x |
| nested.json | strata | 0.247 | 0.249 | 0.258 | 69.988 | 1.00x |
| nested.json | orjson | 0.292 | 0.300 | 0.309 | 69.988 | 0.83x |
| nested.json | msgspec | 0.412 | 0.416 | 0.427 | 69.988 | 0.60x |
| nested.json | ujson | 1.326 | 1.375 | 1.430 | 69.988 | 0.18x |
| nested.json | json | 2.449 | 2.462 | 2.501 | 69.988 | 0.10x |
| wide_arrays.json | strata | 1.614 | 1.626 | 1.645 | 73.992 | 1.00x |
| wide_arrays.json | orjson | 1.805 | 1.843 | 1.896 | 73.992 | 0.88x |
| wide_arrays.json | msgspec | 2.685 | 2.701 | 2.762 | 73.992 | 0.60x |
| wide_arrays.json | ujson | 8.556 | 8.689 | 8.832 | 73.992 | 0.19x |
| wide_arrays.json | json | 16.346 | 16.580 | 17.082 | 73.992 | 0.10x |
| mixed.json | strata | 0.067 | 0.070 | 0.072 | 74.055 | 1.00x |
| mixed.json | orjson | 0.063 | 0.064 | 0.066 | 74.055 | 1.09x |
| mixed.json | msgspec | 0.082 | 0.085 | 0.089 | 74.055 | 0.82x |
| mixed.json | ujson | 0.307 | 0.309 | 0.326 | 74.055 | 0.23x |
| mixed.json | json | 0.521 | 0.538 | 0.552 | 74.055 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.873 | 11.316 | 12.851 | 71.645 | 1.00x |
| users.json | orjson | 13.510 | 13.838 | 14.464 | 71.645 | 0.82x |
| users.json | msgspec | 13.539 | 13.937 | 17.438 | 71.645 | 0.81x |
| users.json | ujson | 19.046 | 19.967 | 21.981 | 71.645 | 0.57x |
| users.json | json | 22.533 | 23.033 | 23.134 | 71.645 | 0.49x |
| flat.json | strata | 0.961 | 0.971 | 1.007 | 69.988 | 1.00x |
| flat.json | orjson | 1.055 | 1.070 | 1.100 | 69.988 | 0.91x |
| flat.json | msgspec | 1.067 | 1.082 | 1.113 | 69.988 | 0.90x |
| flat.json | ujson | 1.586 | 1.615 | 1.694 | 69.988 | 0.60x |
| flat.json | json | 1.941 | 1.951 | 2.091 | 69.988 | 0.50x |
| nested.json | strata | 0.883 | 0.899 | 0.931 | 69.988 | 1.00x |
| nested.json | orjson | 1.061 | 1.068 | 1.237 | 69.988 | 0.84x |
| nested.json | msgspec | 1.076 | 1.103 | 1.262 | 69.988 | 0.82x |
| nested.json | ujson | 1.558 | 1.601 | 1.663 | 69.988 | 0.56x |
| nested.json | json | 2.154 | 2.163 | 2.217 | 69.988 | 0.42x |
| wide_arrays.json | strata | 4.920 | 4.994 | 5.224 | 74.055 | 1.00x |
| wide_arrays.json | orjson | 5.154 | 5.250 | 5.450 | 74.055 | 0.95x |
| wide_arrays.json | msgspec | 5.681 | 5.816 | 5.926 | 74.055 | 0.86x |
| wide_arrays.json | ujson | 7.492 | 7.659 | 7.844 | 74.055 | 0.65x |
| wide_arrays.json | json | 9.979 | 10.129 | 10.309 | 74.055 | 0.49x |
| mixed.json | strata | 0.229 | 0.235 | 0.266 | 74.055 | 1.00x |
| mixed.json | orjson | 0.281 | 0.298 | 0.343 | 74.055 | 0.79x |
| mixed.json | msgspec | 0.286 | 0.293 | 0.388 | 74.055 | 0.80x |
| mixed.json | ujson | 0.367 | 0.394 | 0.435 | 74.055 | 0.60x |
| mixed.json | json | 0.529 | 0.540 | 0.557 | 74.055 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.643 | 11.004 | 11.824 | 69.988 | 1.00x |
| users.ndjson | orjson | 16.361 | 16.909 | 18.109 | 69.988 | 0.65x |
| users.ndjson | msgspec | 16.192 | 16.799 | 17.645 | 69.988 | 0.66x |
| users.ndjson | ujson | 22.368 | 23.239 | 24.432 | 69.988 | 0.47x |
| users.ndjson | json | 28.897 | 29.657 | 30.988 | 69.988 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.228 | 3.313 | 3.754 | 68.645 | 1.00x |
| users.json | orjson | 3.524 | 3.611 | 4.064 | 68.645 | 0.92x |
| users.json | msgspec | 4.524 | 4.649 | 5.053 | 68.645 | 0.71x |
| users.json | ujson | 15.202 | 15.326 | 16.070 | 68.645 | 0.22x |
| users.json | json | 22.840 | 23.380 | 26.990 | 68.645 | 0.14x |
| flat.json | strata | 0.402 | 0.420 | 0.455 | 69.988 | 1.00x |
| flat.json | orjson | 0.483 | 0.503 | 0.520 | 69.988 | 0.83x |
| flat.json | msgspec | 0.596 | 0.617 | 0.813 | 69.988 | 0.68x |
| flat.json | ujson | 1.402 | 1.450 | 1.513 | 69.988 | 0.29x |
| flat.json | json | 2.024 | 2.085 | 2.157 | 69.988 | 0.20x |
| nested.json | strata | 0.352 | 0.355 | 0.413 | 69.988 | 1.00x |
| nested.json | orjson | 0.419 | 0.425 | 0.440 | 69.988 | 0.84x |
| nested.json | msgspec | 0.531 | 0.543 | 0.557 | 69.988 | 0.65x |
| nested.json | ujson | 1.476 | 1.521 | 1.548 | 69.988 | 0.23x |
| nested.json | json | 2.558 | 2.603 | 2.648 | 69.988 | 0.14x |
| wide_arrays.json | strata | 2.042 | 2.087 | 2.116 | 74.055 | 1.00x |
| wide_arrays.json | orjson | 2.315 | 2.353 | 2.393 | 74.055 | 0.89x |
| wide_arrays.json | msgspec | 3.148 | 3.191 | 3.425 | 74.055 | 0.65x |
| wide_arrays.json | ujson | 9.184 | 9.311 | 10.502 | 74.055 | 0.22x |
| wide_arrays.json | json | 16.999 | 17.083 | 17.691 | 74.055 | 0.12x |
| mixed.json | strata | 0.158 | 0.168 | 0.187 | 74.055 | 1.00x |
| mixed.json | orjson | 0.166 | 0.174 | 0.191 | 74.055 | 0.96x |
| mixed.json | msgspec | 0.187 | 0.198 | 0.220 | 74.055 | 0.85x |
| mixed.json | ujson | 0.412 | 0.440 | 0.466 | 74.055 | 0.38x |
| mixed.json | json | 0.630 | 0.659 | 0.775 | 74.055 | 0.26x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.060 | 0.064 | 0.075 | 68.645 | 1.00x |
| users.json $[*].id | jmespath | 0.492 | 0.500 | 0.531 | 68.645 | 0.13x |
| users.json $[*].id | jsonpath-ng | 2.698 | 2.792 | 3.024 | 68.645 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.427 | 0.453 | 0.481 | 68.672 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.986 | 3.017 | 3.209 | 68.672 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 19.365 | 20.026 | 21.007 | 68.672 | 0.02x |
| users.json $..total | strata | 1.679 | 1.760 | 1.902 | 73.602 | 1.00x |
| users.json $..total | jsonpath-ng | 380.524 | 383.155 | 385.714 | 73.602 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.099 | 3.131 | 3.200 | 68.672 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.004 | 14.317 | 14.575 | 68.672 | 0.22x |
| users.json $[*].id | orjson+jsonpath-ng | 16.274 | 16.524 | 16.653 | 68.672 | 0.19x |
| users.json $[*].orders[*].total | strata | 3.322 | 3.373 | 3.414 | 73.602 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 16.749 | 16.976 | 17.850 | 73.602 | 0.20x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 35.917 | 36.374 | 37.061 | 73.602 | 0.09x |
| users.json $..total | strata | 13.289 | 13.975 | 15.386 | 72.715 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 421.021 | 424.745 | 427.475 | 72.715 | 0.03x |

