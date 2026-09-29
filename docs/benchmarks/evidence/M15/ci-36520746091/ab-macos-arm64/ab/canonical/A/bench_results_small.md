# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 38eaa9f248f509d9a4beb75b267189d011c1109e
- python: 3.12.10
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit
- machine: arm64
- processor: Apple M1 (Virtual)
- compiler_flags: -fno-strict-overflow -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -O3 -Wall -arch arm64 -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -march=native -flto=thin -fprofile-use=/Users/runner/work/_temp/strata-arm/build/pgo/strata.profdata -bundle -undefined (19 recorded commands; see the JSON companion)
- repeats: 60
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.309 | 7.071 | 8.188 | 69.734 | 1.00x |
| users.json | orjson | 9.705 | 11.259 | 15.142 | 69.734 | 0.63x |
| users.json | msgspec | 9.170 | 10.863 | 14.185 | 69.734 | 0.65x |
| users.json | ujson | 12.933 | 14.832 | 19.336 | 69.734 | 0.48x |
| users.json | pysimdjson | 128.483 | 138.769 | 171.477 | 69.734 | 0.05x |
| users.json | json | 15.446 | 17.587 | 23.816 | 69.734 | 0.40x |
| flat.json | strata | 0.626 | 0.717 | 1.681 | 99.516 | 1.00x |
| flat.json | orjson | 0.835 | 0.958 | 2.129 | 99.516 | 0.75x |
| flat.json | msgspec | 0.778 | 0.880 | 1.900 | 99.516 | 0.81x |
| flat.json | ujson | 1.219 | 1.502 | 3.177 | 99.516 | 0.48x |
| flat.json | pysimdjson | 12.858 | 15.760 | 22.471 | 99.516 | 0.05x |
| flat.json | json | 1.419 | 1.648 | 2.951 | 99.516 | 0.43x |
| nested.json | strata | 0.548 | 0.657 | 0.858 | 99.547 | 1.00x |
| nested.json | orjson | 0.821 | 0.935 | 1.164 | 99.547 | 0.70x |
| nested.json | msgspec | 0.744 | 0.857 | 1.102 | 99.547 | 0.77x |
| nested.json | ujson | 1.178 | 1.432 | 1.734 | 99.547 | 0.46x |
| nested.json | pysimdjson | 11.086 | 12.692 | 15.490 | 99.547 | 0.05x |
| nested.json | json | 1.506 | 1.775 | 2.393 | 99.547 | 0.37x |
| wide_arrays.json | strata | 3.188 | 3.621 | 4.545 | 102.297 | 1.00x |
| wide_arrays.json | orjson | 3.979 | 4.629 | 5.577 | 102.297 | 0.78x |
| wide_arrays.json | msgspec | 4.378 | 4.925 | 6.569 | 102.297 | 0.74x |
| wide_arrays.json | ujson | 5.886 | 6.417 | 8.657 | 102.297 | 0.56x |
| wide_arrays.json | pysimdjson | 67.488 | 69.906 | 87.027 | 102.297 | 0.05x |
| wide_arrays.json | json | 7.275 | 7.949 | 10.426 | 102.297 | 0.46x |
| mixed.json | strata | 0.136 | 0.162 | 0.425 | 103.578 | 1.00x |
| mixed.json | orjson | 0.180 | 0.211 | 0.301 | 103.578 | 0.77x |
| mixed.json | msgspec | 0.194 | 0.220 | 0.387 | 103.578 | 0.73x |
| mixed.json | ujson | 0.256 | 0.391 | 0.879 | 103.578 | 0.41x |
| mixed.json | pysimdjson | 2.726 | 3.083 | 4.343 | 103.578 | 0.05x |
| mixed.json | json | 0.366 | 0.424 | 0.766 | 103.578 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.441 | 1.695 | 1.952 | 81.703 | 1.00x |
| users.json | orjson | 2.302 | 2.576 | 2.886 | 81.703 | 0.66x |
| users.json | msgspec | 2.920 | 3.313 | 3.653 | 81.703 | 0.51x |
| users.json | ujson | 8.950 | 9.732 | 10.370 | 81.703 | 0.17x |
| users.json | json | 15.937 | 17.017 | 19.457 | 81.703 | 0.10x |
| flat.json | strata | 0.251 | 0.291 | 0.380 | 99.547 | 1.00x |
| flat.json | orjson | 0.281 | 0.327 | 0.463 | 99.547 | 0.89x |
| flat.json | msgspec | 0.361 | 0.440 | 0.592 | 99.547 | 0.66x |
| flat.json | ujson | 0.843 | 0.996 | 1.220 | 99.547 | 0.29x |
| flat.json | json | 1.506 | 1.811 | 2.145 | 99.547 | 0.16x |
| nested.json | strata | 0.128 | 0.152 | 0.174 | 99.547 | 1.00x |
| nested.json | orjson | 0.231 | 0.267 | 0.301 | 99.547 | 0.57x |
| nested.json | msgspec | 0.294 | 0.455 | 0.673 | 99.547 | 0.33x |
| nested.json | ujson | 0.871 | 1.115 | 1.272 | 99.547 | 0.14x |
| nested.json | json | 1.672 | 1.816 | 2.101 | 99.547 | 0.08x |
| wide_arrays.json | strata | 1.148 | 1.338 | 1.629 | 102.297 | 1.00x |
| wide_arrays.json | orjson | 1.463 | 1.687 | 2.053 | 102.297 | 0.79x |
| wide_arrays.json | msgspec | 2.292 | 2.590 | 2.942 | 102.297 | 0.52x |
| wide_arrays.json | ujson | 5.141 | 5.524 | 6.099 | 102.297 | 0.24x |
| wide_arrays.json | json | 12.408 | 13.040 | 14.374 | 102.297 | 0.10x |
| mixed.json | strata | 0.039 | 0.049 | 0.076 | 103.578 | 1.00x |
| mixed.json | orjson | 0.048 | 0.062 | 0.353 | 103.578 | 0.78x |
| mixed.json | msgspec | 0.054 | 0.070 | 0.089 | 103.578 | 0.70x |
| mixed.json | ujson | 0.175 | 0.199 | 0.246 | 103.578 | 0.25x |
| mixed.json | json | 0.357 | 0.393 | 0.484 | 103.578 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.138 | 7.857 | 12.131 | 92.062 | 1.00x |
| users.json | orjson | 10.464 | 12.452 | 16.364 | 92.062 | 0.63x |
| users.json | msgspec | 10.341 | 12.258 | 18.899 | 92.062 | 0.64x |
| users.json | ujson | 14.772 | 16.715 | 26.679 | 92.062 | 0.47x |
| users.json | json | 16.700 | 19.572 | 28.210 | 92.062 | 0.40x |
| flat.json | strata | 0.717 | 0.795 | 1.046 | 99.547 | 1.00x |
| flat.json | orjson | 1.007 | 1.252 | 1.561 | 99.547 | 0.63x |
| flat.json | msgspec | 0.884 | 1.005 | 1.359 | 99.547 | 0.79x |
| flat.json | ujson | 1.289 | 1.467 | 1.727 | 99.547 | 0.54x |
| flat.json | json | 1.530 | 1.708 | 2.001 | 99.547 | 0.47x |
| nested.json | strata | 0.624 | 0.688 | 0.792 | 99.547 | 1.00x |
| nested.json | orjson | 0.972 | 1.164 | 1.358 | 99.547 | 0.59x |
| nested.json | msgspec | 0.839 | 0.927 | 1.047 | 99.547 | 0.74x |
| nested.json | ujson | 1.126 | 1.231 | 1.446 | 99.547 | 0.56x |
| nested.json | json | 1.637 | 1.737 | 2.034 | 99.547 | 0.40x |
| wide_arrays.json | strata | 3.349 | 3.647 | 3.974 | 102.922 | 1.00x |
| wide_arrays.json | orjson | 4.097 | 4.511 | 4.968 | 102.922 | 0.81x |
| wide_arrays.json | msgspec | 4.582 | 4.915 | 5.320 | 102.922 | 0.74x |
| wide_arrays.json | ujson | 6.031 | 6.527 | 7.088 | 102.922 | 0.56x |
| wide_arrays.json | json | 7.436 | 7.929 | 8.278 | 102.922 | 0.46x |
| mixed.json | strata | 0.190 | 0.231 | 0.281 | 103.578 | 1.00x |
| mixed.json | orjson | 0.258 | 0.334 | 0.504 | 103.578 | 0.69x |
| mixed.json | msgspec | 0.245 | 0.328 | 0.398 | 103.578 | 0.70x |
| mixed.json | ujson | 0.321 | 0.479 | 0.599 | 103.578 | 0.48x |
| mixed.json | json | 0.421 | 0.508 | 0.595 | 103.578 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.540 | 9.000 | 13.640 | 99.516 | 1.00x |
| users.ndjson | orjson | 13.125 | 16.173 | 26.596 | 99.516 | 0.56x |
| users.ndjson | msgspec | 12.931 | 16.057 | 23.125 | 99.516 | 0.56x |
| users.ndjson | ujson | 16.472 | 20.700 | 27.755 | 99.516 | 0.43x |
| users.ndjson | json | 20.783 | 26.208 | 40.716 | 99.516 | 0.34x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.032 | 2.446 | 5.037 | 93.828 | 1.00x |
| users.json | orjson | 2.952 | 3.638 | 6.713 | 93.828 | 0.67x |
| users.json | msgspec | 3.685 | 4.349 | 7.287 | 93.828 | 0.56x |
| users.json | ujson | 10.172 | 12.013 | 16.347 | 93.828 | 0.20x |
| users.json | json | 16.519 | 20.632 | 26.417 | 93.828 | 0.12x |
| flat.json | strata | 0.471 | 0.594 | 0.840 | 99.547 | 1.00x |
| flat.json | orjson | 0.509 | 0.702 | 1.130 | 99.547 | 0.85x |
| flat.json | msgspec | 0.662 | 0.807 | 1.106 | 99.547 | 0.74x |
| flat.json | ujson | 1.106 | 1.396 | 2.824 | 99.547 | 0.43x |
| flat.json | json | 1.730 | 2.247 | 3.399 | 99.547 | 0.26x |
| nested.json | strata | 0.337 | 0.404 | 0.510 | 99.547 | 1.00x |
| nested.json | orjson | 0.442 | 0.549 | 0.704 | 99.547 | 0.74x |
| nested.json | msgspec | 0.606 | 0.758 | 0.946 | 99.547 | 0.53x |
| nested.json | ujson | 1.114 | 1.362 | 1.608 | 99.547 | 0.30x |
| nested.json | json | 1.971 | 2.167 | 2.449 | 99.547 | 0.19x |
| wide_arrays.json | strata | 1.540 | 1.798 | 2.077 | 103.562 | 1.00x |
| wide_arrays.json | orjson | 1.900 | 2.295 | 2.756 | 103.562 | 0.78x |
| wide_arrays.json | msgspec | 2.783 | 3.123 | 3.459 | 103.562 | 0.58x |
| wide_arrays.json | ujson | 5.670 | 6.231 | 6.919 | 103.562 | 0.29x |
| wide_arrays.json | json | 13.006 | 13.856 | 14.493 | 103.562 | 0.13x |
| mixed.json | strata | 0.156 | 0.253 | 0.398 | 103.578 | 1.00x |
| mixed.json | orjson | 0.171 | 0.294 | 0.525 | 103.578 | 0.86x |
| mixed.json | msgspec | 0.190 | 0.301 | 0.407 | 103.578 | 0.84x |
| mixed.json | ujson | 0.339 | 0.441 | 0.589 | 103.578 | 0.57x |
| mixed.json | json | 0.503 | 0.641 | 0.874 | 103.578 | 0.39x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.084 | 0.151 | 0.263 | 93.875 | 1.00x |
| users.json $[*].id | jmespath | 0.324 | 0.444 | 0.731 | 93.875 | 0.34x |
| users.json $[*].id | jsonpath-ng | 1.607 | 2.249 | 2.748 | 93.875 | 0.07x |
| users.json $[*].orders[*].total | strata | 0.375 | 0.720 | 1.005 | 94.000 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.946 | 2.455 | 6.162 | 94.000 | 0.29x |
| users.json $[*].orders[*].total | jsonpath-ng | 12.140 | 14.617 | 21.065 | 94.000 | 0.05x |
| users.json $..total | strata | 1.340 | 1.705 | 3.090 | 94.062 | 1.00x |
| users.json $..total | jsonpath-ng | 194.286 | 226.650 | 276.398 | 94.062 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.750 | 4.262 | 5.026 | 93.922 | 1.00x |
| users.json $[*].id | orjson+jmespath | 11.544 | 13.400 | 20.072 | 93.922 | 0.32x |
| users.json $[*].id | orjson+jsonpath-ng | 12.028 | 14.977 | 19.085 | 93.922 | 0.28x |
| users.json $[*].orders[*].total | strata | 3.791 | 4.373 | 4.912 | 94.062 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.751 | 15.536 | 18.691 | 94.062 | 0.28x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 26.623 | 33.088 | 41.445 | 94.062 | 0.13x |
| users.json $..total | strata | 8.165 | 9.623 | 11.716 | 94.094 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 206.624 | 238.905 | 260.976 | 94.094 | 0.04x |

