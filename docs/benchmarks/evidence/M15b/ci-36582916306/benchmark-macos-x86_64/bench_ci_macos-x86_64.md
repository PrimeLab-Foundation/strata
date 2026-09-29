# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 82e3fa5f24c38cfa1150440f0cf5da6c286c01cd
- python: 3.12.10
- implementation: CPython
- platform: macOS-15.7.9-x86_64-i386-64bit
- machine: x86_64
- processor: Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz
- compiler_flags: -fno-strict-overflow -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -O3 -Wall -arch x86_64 -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -fomit-frame-pointer -march=native -flto=thin -fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata -bundle -undefined (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 16.622 | 17.362 | 22.441 | 57.254 | 1.00x |
| users.json | orjson | 23.936 | 26.115 | 32.178 | 57.254 | 0.66x |
| users.json | msgspec | 24.041 | 24.881 | 31.089 | 57.254 | 0.70x |
| users.json | ujson | 35.171 | 37.447 | 44.294 | 57.254 | 0.46x |
| users.json | pysimdjson | 153.258 | 156.439 | 174.964 | 57.254 | 0.11x |
| users.json | json | 39.777 | 42.193 | 49.371 | 57.254 | 0.41x |
| flat.json | strata | 1.354 | 1.397 | 1.653 | 66.184 | 1.00x |
| flat.json | orjson | 1.505 | 1.537 | 2.001 | 66.184 | 0.91x |
| flat.json | msgspec | 1.657 | 1.716 | 2.090 | 66.184 | 0.81x |
| flat.json | ujson | 2.888 | 3.003 | 3.454 | 66.184 | 0.47x |
| flat.json | pysimdjson | 15.341 | 15.549 | 17.296 | 66.184 | 0.09x |
| flat.json | json | 3.340 | 3.411 | 3.863 | 66.184 | 0.41x |
| nested.json | strata | 1.616 | 1.665 | 2.185 | 64.656 | 1.00x |
| nested.json | orjson | 1.847 | 1.923 | 2.653 | 64.656 | 0.87x |
| nested.json | msgspec | 2.014 | 2.145 | 2.887 | 64.656 | 0.78x |
| nested.json | ujson | 3.325 | 3.541 | 4.975 | 64.656 | 0.47x |
| nested.json | pysimdjson | 14.314 | 15.897 | 18.653 | 64.656 | 0.10x |
| nested.json | json | 4.215 | 4.376 | 6.583 | 64.656 | 0.38x |
| wide_arrays.json | strata | 7.562 | 7.999 | 9.116 | 70.625 | 1.00x |
| wide_arrays.json | orjson | 9.672 | 10.150 | 13.259 | 70.625 | 0.79x |
| wide_arrays.json | msgspec | 10.237 | 11.025 | 14.795 | 70.625 | 0.73x |
| wide_arrays.json | ujson | 12.857 | 14.433 | 17.847 | 70.625 | 0.55x |
| wide_arrays.json | pysimdjson | 76.706 | 83.805 | 91.100 | 70.625 | 0.10x |
| wide_arrays.json | json | 16.348 | 17.981 | 18.413 | 70.625 | 0.44x |
| mixed.json | strata | 0.325 | 0.344 | 0.687 | 63.422 | 1.00x |
| mixed.json | orjson | 0.403 | 0.432 | 0.785 | 63.422 | 0.80x |
| mixed.json | msgspec | 0.433 | 0.461 | 0.663 | 63.422 | 0.75x |
| mixed.json | ujson | 0.594 | 0.620 | 1.162 | 63.422 | 0.56x |
| mixed.json | pysimdjson | 3.039 | 3.102 | 5.514 | 63.422 | 0.11x |
| mixed.json | json | 0.845 | 0.875 | 1.730 | 63.422 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.397 | 2.547 | 2.705 | 52.504 | 1.00x |
| users.json | orjson | 3.314 | 3.431 | 4.085 | 52.504 | 0.74x |
| users.json | msgspec | 4.836 | 5.190 | 5.702 | 52.504 | 0.49x |
| users.json | ujson | 23.231 | 23.930 | 25.559 | 52.504 | 0.11x |
| users.json | json | 39.661 | 41.830 | 47.149 | 52.504 | 0.06x |
| flat.json | strata | 0.366 | 0.380 | 0.411 | 64.695 | 1.00x |
| flat.json | orjson | 0.434 | 0.457 | 0.628 | 64.695 | 0.83x |
| flat.json | msgspec | 0.592 | 0.605 | 0.627 | 64.695 | 0.63x |
| flat.json | ujson | 2.352 | 2.410 | 2.484 | 64.695 | 0.16x |
| flat.json | json | 3.835 | 3.937 | 4.319 | 64.695 | 0.10x |
| nested.json | strata | 0.238 | 0.274 | 0.369 | 64.789 | 1.00x |
| nested.json | orjson | 0.375 | 0.407 | 0.600 | 64.789 | 0.67x |
| nested.json | msgspec | 0.594 | 0.623 | 0.825 | 64.789 | 0.44x |
| nested.json | ujson | 2.534 | 2.583 | 3.072 | 64.789 | 0.11x |
| nested.json | json | 4.903 | 5.021 | 6.005 | 64.789 | 0.05x |
| wide_arrays.json | strata | 1.682 | 1.889 | 2.311 | 64.352 | 1.00x |
| wide_arrays.json | orjson | 2.267 | 2.356 | 2.869 | 64.352 | 0.80x |
| wide_arrays.json | msgspec | 3.185 | 3.373 | 3.713 | 64.352 | 0.56x |
| wide_arrays.json | ujson | 9.913 | 10.530 | 10.931 | 64.352 | 0.18x |
| wide_arrays.json | json | 32.263 | 32.533 | 33.049 | 64.352 | 0.06x |
| mixed.json | strata | 0.061 | 0.068 | 0.073 | 60.223 | 1.00x |
| mixed.json | orjson | 0.075 | 0.083 | 0.094 | 60.223 | 0.82x |
| mixed.json | msgspec | 0.101 | 0.111 | 0.129 | 60.223 | 0.61x |
| mixed.json | ujson | 0.428 | 0.433 | 0.521 | 60.223 | 0.16x |
| mixed.json | json | 0.907 | 0.931 | 0.987 | 60.223 | 0.07x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 16.883 | 17.116 | 39.754 | 63.215 | 1.00x |
| users.json | orjson | 22.716 | 24.354 | 26.534 | 63.215 | 0.70x |
| users.json | msgspec | 23.951 | 25.440 | 28.890 | 63.215 | 0.67x |
| users.json | ujson | 36.886 | 37.759 | 42.601 | 63.215 | 0.45x |
| users.json | json | 40.003 | 41.855 | 49.440 | 63.215 | 0.41x |
| flat.json | strata | 1.451 | 1.522 | 2.020 | 64.695 | 1.00x |
| flat.json | orjson | 1.679 | 1.749 | 2.300 | 64.695 | 0.87x |
| flat.json | msgspec | 1.852 | 1.995 | 2.469 | 64.695 | 0.76x |
| flat.json | ujson | 3.111 | 3.236 | 4.108 | 64.695 | 0.47x |
| flat.json | json | 3.500 | 3.578 | 4.994 | 64.695 | 0.43x |
| nested.json | strata | 1.692 | 1.827 | 2.293 | 64.789 | 1.00x |
| nested.json | orjson | 1.991 | 2.072 | 2.807 | 64.789 | 0.88x |
| nested.json | msgspec | 2.161 | 2.303 | 3.018 | 64.789 | 0.79x |
| nested.json | ujson | 3.400 | 3.515 | 4.383 | 64.789 | 0.52x |
| nested.json | json | 4.333 | 4.384 | 5.415 | 64.789 | 0.42x |
| wide_arrays.json | strata | 6.945 | 7.037 | 7.392 | 65.582 | 1.00x |
| wide_arrays.json | orjson | 8.542 | 8.911 | 9.768 | 65.582 | 0.79x |
| wide_arrays.json | msgspec | 9.640 | 9.801 | 11.462 | 65.582 | 0.72x |
| wide_arrays.json | ujson | 12.372 | 12.484 | 13.290 | 65.582 | 0.56x |
| wide_arrays.json | json | 16.030 | 16.297 | 17.143 | 65.582 | 0.43x |
| mixed.json | strata | 0.390 | 0.401 | 0.442 | 60.223 | 1.00x |
| mixed.json | orjson | 0.512 | 0.528 | 0.579 | 60.223 | 0.76x |
| mixed.json | msgspec | 0.542 | 0.551 | 0.613 | 60.223 | 0.73x |
| mixed.json | ujson | 0.712 | 0.732 | 0.790 | 60.223 | 0.55x |
| mixed.json | json | 0.928 | 0.941 | 0.985 | 60.223 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 18.997 | 19.578 | 22.013 | 65.762 | 1.00x |
| users.ndjson | orjson | 28.259 | 29.183 | 34.291 | 65.762 | 0.67x |
| users.ndjson | msgspec | 28.656 | 29.380 | 34.386 | 65.762 | 0.67x |
| users.ndjson | ujson | 41.565 | 43.607 | 53.481 | 65.762 | 0.45x |
| users.ndjson | json | 52.972 | 55.881 | 60.843 | 65.762 | 0.35x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.195 | 3.361 | 3.647 | 63.246 | 1.00x |
| users.json | orjson | 4.182 | 4.505 | 5.359 | 63.246 | 0.75x |
| users.json | msgspec | 5.882 | 6.512 | 7.059 | 63.246 | 0.52x |
| users.json | ujson | 24.968 | 25.341 | 26.416 | 63.246 | 0.13x |
| users.json | json | 40.327 | 41.342 | 44.200 | 63.246 | 0.08x |
| flat.json | strata | 0.711 | 0.803 | 1.016 | 64.695 | 1.00x |
| flat.json | orjson | 0.854 | 0.959 | 1.353 | 64.695 | 0.84x |
| flat.json | msgspec | 0.980 | 1.221 | 1.466 | 64.695 | 0.66x |
| flat.json | ujson | 2.823 | 3.019 | 3.387 | 64.695 | 0.27x |
| flat.json | json | 4.560 | 4.924 | 8.110 | 64.695 | 0.16x |
| nested.json | strata | 0.578 | 0.643 | 0.892 | 64.789 | 1.00x |
| nested.json | orjson | 0.766 | 0.887 | 0.987 | 64.789 | 0.72x |
| nested.json | msgspec | 1.005 | 1.090 | 1.358 | 64.789 | 0.59x |
| nested.json | ujson | 2.912 | 3.118 | 3.687 | 64.789 | 0.21x |
| nested.json | json | 5.373 | 5.696 | 6.355 | 64.789 | 0.11x |
| wide_arrays.json | strata | 2.294 | 2.598 | 2.757 | 64.352 | 1.00x |
| wide_arrays.json | orjson | 2.937 | 3.190 | 3.429 | 64.352 | 0.81x |
| wide_arrays.json | msgspec | 3.818 | 4.173 | 4.617 | 64.352 | 0.62x |
| wide_arrays.json | ujson | 11.258 | 11.601 | 55.701 | 64.352 | 0.22x |
| wide_arrays.json | json | 33.131 | 33.914 | 34.207 | 64.352 | 0.08x |
| mixed.json | strata | 0.307 | 0.346 | 0.418 | 60.223 | 1.00x |
| mixed.json | orjson | 0.365 | 0.397 | 0.437 | 60.223 | 0.87x |
| mixed.json | msgspec | 0.375 | 0.462 | 0.516 | 60.223 | 0.75x |
| mixed.json | ujson | 0.737 | 0.763 | 1.640 | 60.223 | 0.45x |
| mixed.json | json | 1.181 | 1.253 | 2.301 | 60.223 | 0.28x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.164 | 0.183 | 0.257 | 63.328 | 1.00x |
| users.json $[*].id | jmespath | 0.931 | 0.988 | 1.067 | 63.328 | 0.19x |
| users.json $[*].id | jsonpath-ng | 4.983 | 5.251 | 7.028 | 63.328 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.904 | 0.942 | 1.224 | 60.469 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.528 | 5.821 | 7.503 | 60.469 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 33.250 | 34.226 | 36.494 | 60.469 | 0.03x |
| users.json $..total | strata | 3.457 | 3.851 | 5.181 | 60.582 | 1.00x |
| users.json $..total | jsonpath-ng | 753.985 | 780.605 | 880.271 | 60.582 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.597 | 3.655 | 3.761 | 63.395 | 1.00x |
| users.json $[*].id | orjson+jmespath | 25.273 | 26.060 | 28.199 | 63.395 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 29.234 | 30.671 | 32.321 | 63.395 | 0.12x |
| users.json $[*].orders[*].total | strata | 4.227 | 4.986 | 6.133 | 60.555 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 32.503 | 35.278 | 42.680 | 60.555 | 0.14x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 66.277 | 76.369 | 88.386 | 60.555 | 0.07x |
| users.json $..total | strata | 23.321 | 23.777 | 29.044 | 60.656 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 775.970 | 806.706 | 916.429 | 60.656 | 0.03x |

