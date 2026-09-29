# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 296d2ea02694ce7811592deeaa970aaa11c9432f
- python: 3.12.10
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit
- machine: arm64
- processor: Apple M1 (Virtual)
- compiler_flags: -fno-strict-overflow -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -O3 -Wall -arch arm64 -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -march=native -flto=thin -fprofile-use=/Users/runner/work/_temp/strata-arm/build/pgo/strata.profdata -bundle -undefined (24 recorded commands; see the JSON companion)
- repeats: 60
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 5.797 | 6.873 | 8.094 | 69.469 | 1.00x |
| users.json | orjson | 8.738 | 10.483 | 12.299 | 69.469 | 0.66x |
| users.json | msgspec | 8.535 | 10.284 | 12.415 | 69.469 | 0.67x |
| users.json | ujson | 11.155 | 13.902 | 16.100 | 69.469 | 0.49x |
| users.json | pysimdjson | 117.232 | 134.870 | 151.125 | 69.469 | 0.05x |
| users.json | json | 13.918 | 16.284 | 18.656 | 69.469 | 0.42x |
| flat.json | strata | 0.544 | 0.640 | 0.746 | 97.734 | 1.00x |
| flat.json | orjson | 0.685 | 0.846 | 0.951 | 97.734 | 0.76x |
| flat.json | msgspec | 0.674 | 0.782 | 0.886 | 97.734 | 0.82x |
| flat.json | ujson | 1.049 | 1.308 | 1.566 | 97.734 | 0.49x |
| flat.json | pysimdjson | 11.151 | 12.827 | 13.797 | 97.734 | 0.05x |
| flat.json | json | 1.268 | 1.428 | 1.585 | 97.734 | 0.45x |
| nested.json | strata | 0.477 | 0.553 | 0.625 | 97.734 | 1.00x |
| nested.json | orjson | 0.663 | 0.769 | 0.868 | 97.734 | 0.72x |
| nested.json | msgspec | 0.634 | 0.734 | 0.822 | 97.734 | 0.75x |
| nested.json | ujson | 0.957 | 1.160 | 1.328 | 97.734 | 0.48x |
| nested.json | pysimdjson | 9.698 | 10.827 | 12.542 | 97.734 | 0.05x |
| nested.json | json | 1.310 | 1.499 | 1.747 | 97.734 | 0.37x |
| wide_arrays.json | strata | 2.838 | 3.713 | 4.873 | 100.500 | 1.00x |
| wide_arrays.json | orjson | 3.443 | 4.436 | 6.025 | 100.500 | 0.84x |
| wide_arrays.json | msgspec | 3.805 | 4.987 | 6.440 | 100.500 | 0.74x |
| wide_arrays.json | ujson | 4.950 | 6.221 | 8.030 | 100.500 | 0.60x |
| wide_arrays.json | pysimdjson | 60.162 | 70.076 | 90.385 | 100.500 | 0.05x |
| wide_arrays.json | json | 6.424 | 7.933 | 10.326 | 100.500 | 0.47x |
| mixed.json | strata | 0.119 | 0.142 | 0.247 | 100.516 | 1.00x |
| mixed.json | orjson | 0.151 | 0.183 | 0.234 | 100.516 | 0.78x |
| mixed.json | msgspec | 0.164 | 0.195 | 0.237 | 100.516 | 0.73x |
| mixed.json | ujson | 0.209 | 0.344 | 0.523 | 100.516 | 0.41x |
| mixed.json | pysimdjson | 2.436 | 2.715 | 3.815 | 100.516 | 0.05x |
| mixed.json | json | 0.314 | 0.379 | 0.623 | 100.516 | 0.37x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.486 | 1.677 | 2.051 | 82.031 | 1.00x |
| users.json | orjson | 2.228 | 2.617 | 3.814 | 82.031 | 0.64x |
| users.json | msgspec | 2.802 | 3.248 | 4.095 | 82.031 | 0.52x |
| users.json | ujson | 8.648 | 9.542 | 11.584 | 82.031 | 0.18x |
| users.json | json | 15.494 | 17.099 | 20.002 | 82.031 | 0.10x |
| flat.json | strata | 0.192 | 0.210 | 0.256 | 97.734 | 1.00x |
| flat.json | orjson | 0.237 | 0.318 | 0.546 | 97.734 | 0.66x |
| flat.json | msgspec | 0.293 | 0.314 | 0.372 | 97.734 | 0.67x |
| flat.json | ujson | 0.702 | 0.747 | 0.871 | 97.734 | 0.28x |
| flat.json | json | 1.266 | 1.405 | 1.857 | 97.734 | 0.15x |
| nested.json | strata | 0.114 | 0.145 | 0.179 | 97.734 | 1.00x |
| nested.json | orjson | 0.202 | 0.246 | 0.323 | 97.734 | 0.59x |
| nested.json | msgspec | 0.300 | 0.464 | 0.561 | 97.734 | 0.31x |
| nested.json | ujson | 0.754 | 0.984 | 1.141 | 97.734 | 0.15x |
| nested.json | json | 1.530 | 1.750 | 1.994 | 97.734 | 0.08x |
| wide_arrays.json | strata | 1.029 | 1.260 | 1.651 | 100.500 | 1.00x |
| wide_arrays.json | orjson | 1.407 | 1.662 | 2.077 | 100.500 | 0.76x |
| wide_arrays.json | msgspec | 2.052 | 2.299 | 2.830 | 100.500 | 0.55x |
| wide_arrays.json | ujson | 4.686 | 5.162 | 6.491 | 100.500 | 0.24x |
| wide_arrays.json | json | 11.229 | 12.268 | 14.511 | 100.500 | 0.10x |
| mixed.json | strata | 0.040 | 0.052 | 0.062 | 100.516 | 1.00x |
| mixed.json | orjson | 0.048 | 0.063 | 0.076 | 100.516 | 0.83x |
| mixed.json | msgspec | 0.055 | 0.077 | 0.265 | 100.516 | 0.68x |
| mixed.json | ujson | 0.171 | 0.201 | 0.228 | 100.516 | 0.26x |
| mixed.json | json | 0.355 | 0.406 | 0.487 | 100.516 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.171 | 6.642 | 7.882 | 92.078 | 1.00x |
| users.json | orjson | 9.066 | 9.952 | 11.944 | 92.078 | 0.67x |
| users.json | msgspec | 8.727 | 9.677 | 11.851 | 92.078 | 0.69x |
| users.json | ujson | 11.895 | 13.400 | 16.118 | 92.078 | 0.50x |
| users.json | json | 14.059 | 15.395 | 18.342 | 92.078 | 0.43x |
| flat.json | strata | 0.577 | 0.673 | 0.838 | 97.734 | 1.00x |
| flat.json | orjson | 0.756 | 1.048 | 1.257 | 97.734 | 0.64x |
| flat.json | msgspec | 0.720 | 0.890 | 1.045 | 97.734 | 0.76x |
| flat.json | ujson | 1.038 | 1.240 | 1.480 | 97.734 | 0.54x |
| flat.json | json | 1.285 | 1.488 | 1.732 | 97.734 | 0.45x |
| nested.json | strata | 0.511 | 0.658 | 0.819 | 97.734 | 1.00x |
| nested.json | orjson | 0.820 | 1.090 | 1.348 | 97.734 | 0.60x |
| nested.json | msgspec | 0.695 | 0.907 | 1.047 | 97.734 | 0.73x |
| nested.json | ujson | 0.971 | 1.226 | 1.498 | 97.734 | 0.54x |
| nested.json | json | 1.367 | 1.657 | 2.413 | 97.734 | 0.40x |
| wide_arrays.json | strata | 3.036 | 3.777 | 7.588 | 100.500 | 1.00x |
| wide_arrays.json | orjson | 3.680 | 4.490 | 9.367 | 100.500 | 0.84x |
| wide_arrays.json | msgspec | 4.153 | 5.113 | 9.083 | 100.500 | 0.74x |
| wide_arrays.json | ujson | 5.450 | 6.655 | 12.532 | 100.500 | 0.57x |
| wide_arrays.json | json | 6.814 | 8.108 | 14.312 | 100.500 | 0.47x |
| mixed.json | strata | 0.133 | 0.159 | 0.275 | 100.516 | 1.00x |
| mixed.json | orjson | 0.245 | 0.365 | 0.575 | 100.516 | 0.44x |
| mixed.json | msgspec | 0.196 | 0.228 | 0.404 | 100.516 | 0.70x |
| mixed.json | ujson | 0.235 | 0.274 | 0.482 | 100.516 | 0.58x |
| mixed.json | json | 0.332 | 0.379 | 0.553 | 100.516 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.698 | 7.831 | 8.342 | 97.734 | 1.00x |
| users.ndjson | orjson | 11.496 | 13.371 | 14.896 | 97.734 | 0.59x |
| users.ndjson | msgspec | 10.914 | 13.225 | 15.571 | 97.734 | 0.59x |
| users.ndjson | ujson | 13.579 | 16.461 | 20.459 | 97.734 | 0.48x |
| users.ndjson | json | 17.617 | 21.105 | 23.020 | 97.734 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.793 | 2.014 | 2.197 | 92.125 | 1.00x |
| users.json | orjson | 2.593 | 2.915 | 3.302 | 92.125 | 0.69x |
| users.json | msgspec | 3.304 | 3.649 | 4.015 | 92.125 | 0.55x |
| users.json | ujson | 9.316 | 9.798 | 10.482 | 92.125 | 0.21x |
| users.json | json | 15.804 | 16.968 | 18.503 | 92.125 | 0.12x |
| flat.json | strata | 0.468 | 0.559 | 1.067 | 97.734 | 1.00x |
| flat.json | orjson | 0.530 | 0.612 | 0.835 | 97.734 | 0.91x |
| flat.json | msgspec | 0.592 | 0.687 | 0.962 | 97.734 | 0.81x |
| flat.json | ujson | 1.113 | 1.218 | 1.387 | 97.734 | 0.46x |
| flat.json | json | 1.763 | 2.084 | 2.325 | 97.734 | 0.27x |
| nested.json | strata | 0.225 | 0.403 | 0.479 | 97.734 | 1.00x |
| nested.json | orjson | 0.339 | 0.530 | 0.643 | 97.734 | 0.76x |
| nested.json | msgspec | 0.435 | 0.775 | 0.953 | 97.734 | 0.52x |
| nested.json | ujson | 0.912 | 1.375 | 1.681 | 97.734 | 0.29x |
| nested.json | json | 1.657 | 2.217 | 2.510 | 97.734 | 0.18x |
| wide_arrays.json | strata | 1.533 | 1.969 | 4.415 | 100.500 | 1.00x |
| wide_arrays.json | orjson | 1.934 | 2.536 | 5.094 | 100.500 | 0.78x |
| wide_arrays.json | msgspec | 2.711 | 3.243 | 5.670 | 100.500 | 0.61x |
| wide_arrays.json | ujson | 5.537 | 6.602 | 9.045 | 100.500 | 0.30x |
| wide_arrays.json | json | 12.231 | 14.310 | 24.692 | 100.500 | 0.14x |
| mixed.json | strata | 0.108 | 0.174 | 0.381 | 100.516 | 1.00x |
| mixed.json | orjson | 0.127 | 0.175 | 0.432 | 100.516 | 0.99x |
| mixed.json | msgspec | 0.128 | 0.248 | 0.532 | 100.516 | 0.70x |
| mixed.json | ujson | 0.248 | 0.339 | 0.556 | 100.516 | 0.51x |
| mixed.json | json | 0.407 | 0.498 | 0.781 | 100.516 | 0.35x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.046 | 0.059 | 0.093 | 92.203 | 1.00x |
| users.json $[*].id | jmespath | 0.257 | 0.289 | 0.401 | 92.203 | 0.20x |
| users.json $[*].id | jsonpath-ng | 1.406 | 1.543 | 1.796 | 92.203 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.270 | 0.285 | 0.566 | 92.312 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.558 | 1.720 | 2.420 | 92.312 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 9.722 | 9.972 | 13.787 | 92.312 | 0.03x |
| users.json $..total | strata | 1.220 | 1.621 | 2.674 | 92.375 | 1.00x |
| users.json $..total | jsonpath-ng | 184.821 | 217.716 | 280.021 | 92.375 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.361 | 3.541 | 3.925 | 92.250 | 1.00x |
| users.json $[*].id | orjson+jmespath | 9.175 | 10.072 | 12.027 | 92.250 | 0.35x |
| users.json $[*].id | orjson+jsonpath-ng | 10.454 | 11.419 | 13.920 | 92.250 | 0.31x |
| users.json $[*].orders[*].total | strata | 3.420 | 3.602 | 4.576 | 92.328 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 10.461 | 11.071 | 15.545 | 92.328 | 0.33x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 20.559 | 22.149 | 30.523 | 92.328 | 0.16x |
| users.json $..total | strata | 7.522 | 9.231 | 14.852 | 92.406 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 200.370 | 230.505 | 302.265 | 92.406 | 0.04x |

