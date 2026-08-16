# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 9c9d784
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
| users.json | strata | 7.179 | 7.858 | 8.699 | 63.531 | 1.00x |
| users.json | orjson | 9.047 | 10.206 | 11.144 | 63.531 | 0.77x |
| users.json | msgspec | 9.249 | 9.912 | 11.838 | 63.531 | 0.79x |
| users.json | ujson | 12.283 | 13.618 | 17.534 | 63.531 | 0.58x |
| users.json | pysimdjson | 121.688 | 130.511 | 151.624 | 63.531 | 0.06x |
| users.json | json | 14.467 | 15.467 | 17.772 | 63.531 | 0.51x |
| flat.json | strata | 0.674 | 0.683 | 0.762 | 91.703 | 1.00x |
| flat.json | orjson | 0.750 | 0.773 | 0.814 | 91.703 | 0.88x |
| flat.json | msgspec | 0.708 | 0.726 | 0.756 | 91.703 | 0.94x |
| flat.json | ujson | 1.158 | 1.188 | 1.245 | 91.703 | 0.57x |
| flat.json | pysimdjson | 11.954 | 12.012 | 12.123 | 91.703 | 0.06x |
| flat.json | json | 1.386 | 1.399 | 1.427 | 91.703 | 0.49x |
| nested.json | strata | 0.598 | 0.608 | 0.671 | 91.719 | 1.00x |
| nested.json | orjson | 0.735 | 0.754 | 0.774 | 91.719 | 0.81x |
| nested.json | msgspec | 0.691 | 0.694 | 0.705 | 91.719 | 0.88x |
| nested.json | ujson | 1.054 | 1.079 | 1.160 | 91.719 | 0.56x |
| nested.json | pysimdjson | 10.462 | 10.555 | 11.037 | 91.719 | 0.06x |
| nested.json | json | 1.447 | 1.468 | 1.777 | 91.719 | 0.41x |
| wide_arrays.json | strata | 3.294 | 3.488 | 3.935 | 94.344 | 1.00x |
| wide_arrays.json | orjson | 3.507 | 3.664 | 4.132 | 94.344 | 0.95x |
| wide_arrays.json | msgspec | 3.999 | 4.183 | 4.250 | 94.344 | 0.83x |
| wide_arrays.json | ujson | 5.391 | 5.562 | 5.631 | 94.344 | 0.63x |
| wide_arrays.json | pysimdjson | 61.828 | 64.681 | 66.166 | 94.344 | 0.05x |
| wide_arrays.json | json | 6.581 | 6.959 | 7.126 | 94.344 | 0.50x |
| mixed.json | strata | 0.138 | 0.143 | 0.148 | 94.359 | 1.00x |
| mixed.json | orjson | 0.158 | 0.165 | 0.180 | 94.359 | 0.87x |
| mixed.json | msgspec | 0.171 | 0.174 | 0.192 | 94.359 | 0.82x |
| mixed.json | ujson | 0.215 | 0.328 | 0.468 | 94.359 | 0.44x |
| mixed.json | pysimdjson | 2.525 | 2.542 | 2.567 | 94.359 | 0.06x |
| mixed.json | json | 0.309 | 0.337 | 0.363 | 94.359 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.854 | 1.984 | 2.162 | 76.562 | 1.00x |
| users.json | orjson | 2.239 | 2.377 | 2.518 | 76.562 | 0.83x |
| users.json | msgspec | 2.842 | 3.057 | 3.172 | 76.562 | 0.65x |
| users.json | ujson | 11.121 | 11.335 | 11.671 | 76.562 | 0.17x |
| users.json | json | 15.307 | 16.163 | 16.904 | 76.562 | 0.12x |
| flat.json | strata | 0.197 | 0.202 | 0.212 | 91.719 | 1.00x |
| flat.json | orjson | 0.318 | 0.324 | 0.385 | 91.719 | 0.62x |
| flat.json | msgspec | 0.332 | 0.336 | 0.369 | 91.719 | 0.60x |
| flat.json | ujson | 0.952 | 0.962 | 1.011 | 91.719 | 0.21x |
| flat.json | json | 1.382 | 1.408 | 1.443 | 91.719 | 0.14x |
| nested.json | strata | 0.151 | 0.154 | 0.160 | 91.734 | 1.00x |
| nested.json | orjson | 0.218 | 0.235 | 0.281 | 91.734 | 0.66x |
| nested.json | msgspec | 0.290 | 0.346 | 0.422 | 91.734 | 0.44x |
| nested.json | ujson | 0.986 | 1.029 | 1.103 | 91.734 | 0.15x |
| nested.json | json | 1.662 | 1.697 | 1.882 | 91.734 | 0.09x |
| wide_arrays.json | strata | 1.169 | 1.255 | 1.449 | 94.344 | 1.00x |
| wide_arrays.json | orjson | 1.319 | 1.465 | 1.532 | 94.344 | 0.86x |
| wide_arrays.json | msgspec | 2.031 | 2.139 | 2.218 | 94.344 | 0.59x |
| wide_arrays.json | ujson | 6.529 | 6.697 | 6.865 | 94.344 | 0.19x |
| wide_arrays.json | json | 11.548 | 11.981 | 12.282 | 94.344 | 0.10x |
| mixed.json | strata | 0.042 | 0.045 | 0.046 | 94.359 | 1.00x |
| mixed.json | orjson | 0.045 | 0.047 | 0.052 | 94.359 | 0.96x |
| mixed.json | msgspec | 0.054 | 0.133 | 0.300 | 94.359 | 0.34x |
| mixed.json | ujson | 0.220 | 0.225 | 0.234 | 94.359 | 0.20x |
| mixed.json | json | 0.356 | 0.364 | 0.396 | 94.359 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.653 | 7.996 | 8.308 | 86.609 | 1.00x |
| users.json | orjson | 9.473 | 10.054 | 10.911 | 86.609 | 0.80x |
| users.json | msgspec | 9.080 | 9.799 | 11.120 | 86.609 | 0.82x |
| users.json | ujson | 12.143 | 13.429 | 14.545 | 86.609 | 0.60x |
| users.json | json | 15.350 | 15.852 | 16.334 | 86.609 | 0.50x |
| flat.json | strata | 0.718 | 0.734 | 0.742 | 91.719 | 1.00x |
| flat.json | orjson | 0.871 | 0.890 | 0.970 | 91.719 | 0.82x |
| flat.json | msgspec | 0.771 | 0.792 | 0.850 | 91.719 | 0.93x |
| flat.json | ujson | 1.135 | 1.173 | 1.280 | 91.719 | 0.63x |
| flat.json | json | 1.431 | 1.442 | 1.480 | 91.719 | 0.51x |
| nested.json | strata | 0.643 | 0.654 | 0.718 | 91.734 | 1.00x |
| nested.json | orjson | 0.865 | 0.886 | 0.933 | 91.734 | 0.74x |
| nested.json | msgspec | 0.746 | 0.759 | 0.833 | 91.734 | 0.86x |
| nested.json | ujson | 1.058 | 1.073 | 1.108 | 91.734 | 0.61x |
| nested.json | json | 1.485 | 1.519 | 1.589 | 91.734 | 0.43x |
| wide_arrays.json | strata | 3.576 | 3.753 | 3.859 | 94.344 | 1.00x |
| wide_arrays.json | orjson | 3.758 | 3.929 | 4.066 | 94.344 | 0.96x |
| wide_arrays.json | msgspec | 4.288 | 4.524 | 4.657 | 94.344 | 0.83x |
| wide_arrays.json | ujson | 5.776 | 5.989 | 6.166 | 94.344 | 0.63x |
| wide_arrays.json | json | 6.941 | 7.339 | 7.526 | 94.344 | 0.51x |
| mixed.json | strata | 0.154 | 0.170 | 0.195 | 94.359 | 1.00x |
| mixed.json | orjson | 0.186 | 0.285 | 0.449 | 94.359 | 0.60x |
| mixed.json | msgspec | 0.191 | 0.210 | 0.264 | 94.359 | 0.81x |
| mixed.json | ujson | 0.240 | 0.256 | 0.330 | 94.359 | 0.66x |
| mixed.json | json | 0.340 | 0.358 | 0.531 | 94.359 | 0.48x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.698 | 8.046 | 8.525 | 91.688 | 1.00x |
| users.ndjson | orjson | 11.353 | 11.907 | 12.113 | 91.688 | 0.68x |
| users.ndjson | msgspec | 11.120 | 11.796 | 12.320 | 91.688 | 0.68x |
| users.ndjson | ujson | 13.720 | 14.655 | 14.915 | 91.688 | 0.55x |
| users.ndjson | json | 18.002 | 18.980 | 19.730 | 91.688 | 0.42x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.265 | 2.440 | 3.273 | 84.953 | 1.00x |
| users.json | orjson | 2.787 | 2.859 | 2.980 | 84.953 | 0.85x |
| users.json | msgspec | 3.362 | 3.588 | 4.007 | 84.953 | 0.68x |
| users.json | ujson | 11.362 | 12.101 | 12.263 | 84.953 | 0.20x |
| users.json | json | 16.062 | 16.930 | 17.961 | 84.953 | 0.14x |
| flat.json | strata | 0.290 | 0.322 | 0.487 | 91.719 | 1.00x |
| flat.json | orjson | 0.351 | 0.461 | 0.556 | 91.719 | 0.70x |
| flat.json | msgspec | 0.410 | 0.423 | 0.473 | 91.719 | 0.76x |
| flat.json | ujson | 1.011 | 1.023 | 1.111 | 91.719 | 0.31x |
| flat.json | json | 1.427 | 1.503 | 1.623 | 91.719 | 0.21x |
| nested.json | strata | 0.256 | 0.296 | 0.406 | 91.734 | 1.00x |
| nested.json | orjson | 0.327 | 0.371 | 0.422 | 91.734 | 0.80x |
| nested.json | msgspec | 0.527 | 0.624 | 0.679 | 91.734 | 0.47x |
| nested.json | ujson | 1.194 | 1.262 | 1.398 | 91.734 | 0.23x |
| nested.json | json | 1.707 | 1.777 | 1.926 | 91.734 | 0.17x |
| wide_arrays.json | strata | 1.554 | 1.607 | 1.793 | 94.344 | 1.00x |
| wide_arrays.json | orjson | 1.741 | 1.830 | 1.972 | 94.344 | 0.88x |
| wide_arrays.json | msgspec | 2.531 | 2.623 | 2.702 | 94.344 | 0.61x |
| wide_arrays.json | ujson | 7.107 | 7.221 | 7.580 | 94.344 | 0.22x |
| wide_arrays.json | json | 12.323 | 12.516 | 12.980 | 94.344 | 0.13x |
| mixed.json | strata | 0.122 | 0.268 | 0.328 | 94.359 | 1.00x |
| mixed.json | orjson | 0.135 | 0.290 | 0.381 | 94.359 | 0.92x |
| mixed.json | msgspec | 0.132 | 0.289 | 0.627 | 94.359 | 0.93x |
| mixed.json | ujson | 0.303 | 0.481 | 0.602 | 94.359 | 0.56x |
| mixed.json | json | 0.430 | 0.619 | 0.761 | 94.359 | 0.43x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.051 | 0.057 | 0.065 | 85.016 | 1.00x |
| users.json $[*].id | jmespath | 0.268 | 0.291 | 0.308 | 85.016 | 0.20x |
| users.json $[*].id | jsonpath-ng | 1.453 | 1.544 | 1.601 | 85.016 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.279 | 0.334 | 0.454 | 85.125 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.605 | 1.783 | 1.973 | 85.125 | 0.19x |
| users.json $[*].orders[*].total | jsonpath-ng | 9.966 | 10.513 | 10.856 | 85.125 | 0.03x |
| users.json $..total | strata | 1.243 | 1.323 | 1.543 | 87.031 | 1.00x |
| users.json $..total | jsonpath-ng | 186.727 | 193.595 | 209.070 | 87.031 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.605 | 3.651 | 3.733 | 85.062 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.194 | 10.299 | 11.984 | 85.062 | 0.35x |
| users.json $[*].id | orjson+jsonpath-ng | 11.440 | 11.529 | 11.823 | 85.062 | 0.32x |
| users.json $[*].orders[*].total | strata | 3.713 | 3.890 | 4.103 | 87.016 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 11.076 | 11.917 | 12.438 | 87.016 | 0.33x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 21.389 | 23.595 | 24.130 | 87.016 | 0.16x |
| users.json $..total | strata | 9.312 | 9.550 | 10.383 | 87.062 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 199.988 | 207.018 | 224.852 | 87.062 | 0.05x |

