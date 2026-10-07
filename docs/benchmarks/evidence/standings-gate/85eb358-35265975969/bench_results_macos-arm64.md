# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 85eb3583e98587844415f5d32f85a61cbedfcf49
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
| users.json | strata | 5.778 | 5.832 | 17.605 | 68.250 | 1.00x |
| users.json | orjson | 8.725 | 8.898 | 25.145 | 68.250 | 0.66x |
| users.json | msgspec | 8.348 | 8.651 | 10.937 | 68.250 | 0.67x |
| users.json | ujson | 11.013 | 11.689 | 13.725 | 68.250 | 0.50x |
| users.json | pysimdjson | 115.612 | 120.430 | 128.399 | 68.250 | 0.05x |
| users.json | json | 13.711 | 13.873 | 15.717 | 68.250 | 0.42x |
| flat.json | strata | 0.619 | 0.665 | 1.064 | 100.891 | 1.00x |
| flat.json | orjson | 0.787 | 0.856 | 2.440 | 100.891 | 0.78x |
| flat.json | msgspec | 0.755 | 0.801 | 0.898 | 100.891 | 0.83x |
| flat.json | ujson | 1.200 | 1.342 | 3.161 | 100.891 | 0.50x |
| flat.json | pysimdjson | 12.528 | 13.022 | 16.278 | 100.891 | 0.05x |
| flat.json | json | 1.423 | 1.511 | 1.700 | 100.891 | 0.44x |
| nested.json | strata | 0.525 | 0.561 | 0.641 | 100.891 | 1.00x |
| nested.json | orjson | 0.788 | 0.835 | 1.216 | 100.891 | 0.67x |
| nested.json | msgspec | 0.718 | 0.789 | 1.135 | 100.891 | 0.71x |
| nested.json | ujson | 1.094 | 1.189 | 1.462 | 100.891 | 0.47x |
| nested.json | pysimdjson | 10.345 | 11.595 | 12.064 | 100.891 | 0.05x |
| nested.json | json | 1.462 | 1.534 | 1.920 | 100.891 | 0.37x |
| wide_arrays.json | strata | 3.206 | 3.715 | 6.694 | 104.688 | 1.00x |
| wide_arrays.json | orjson | 4.321 | 4.513 | 5.827 | 104.688 | 0.82x |
| wide_arrays.json | msgspec | 4.341 | 5.025 | 10.820 | 104.688 | 0.74x |
| wide_arrays.json | ujson | 5.958 | 6.335 | 20.112 | 104.688 | 0.59x |
| wide_arrays.json | pysimdjson | 69.697 | 73.482 | 83.317 | 104.688 | 0.05x |
| wide_arrays.json | json | 7.014 | 8.093 | 13.263 | 104.688 | 0.46x |
| mixed.json | strata | 0.138 | 0.147 | 0.159 | 105.672 | 1.00x |
| mixed.json | orjson | 0.177 | 0.193 | 0.229 | 105.672 | 0.76x |
| mixed.json | msgspec | 0.172 | 0.201 | 0.346 | 105.672 | 0.73x |
| mixed.json | ujson | 0.218 | 0.373 | 0.919 | 105.672 | 0.39x |
| mixed.json | pysimdjson | 2.472 | 2.743 | 3.087 | 105.672 | 0.05x |
| mixed.json | json | 0.324 | 0.382 | 0.465 | 105.672 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.298 | 1.460 | 1.616 | 82.422 | 1.00x |
| users.json | orjson | 2.118 | 2.175 | 2.248 | 82.422 | 0.67x |
| users.json | msgspec | 2.706 | 2.809 | 2.955 | 82.422 | 0.52x |
| users.json | ujson | 8.422 | 8.456 | 8.822 | 82.422 | 0.17x |
| users.json | json | 14.940 | 15.220 | 16.119 | 82.422 | 0.10x |
| flat.json | strata | 0.241 | 0.290 | 0.381 | 100.891 | 1.00x |
| flat.json | orjson | 0.287 | 0.318 | 0.452 | 100.891 | 0.91x |
| flat.json | msgspec | 0.378 | 0.400 | 3.763 | 100.891 | 0.73x |
| flat.json | ujson | 0.843 | 0.907 | 2.600 | 100.891 | 0.32x |
| flat.json | json | 1.499 | 1.669 | 2.080 | 100.891 | 0.17x |
| nested.json | strata | 0.133 | 0.154 | 0.185 | 100.891 | 1.00x |
| nested.json | orjson | 0.242 | 0.267 | 0.282 | 100.891 | 0.58x |
| nested.json | msgspec | 0.400 | 0.442 | 0.481 | 100.891 | 0.35x |
| nested.json | ujson | 0.911 | 1.004 | 1.129 | 100.891 | 0.15x |
| nested.json | json | 1.729 | 1.870 | 4.073 | 100.891 | 0.08x |
| wide_arrays.json | strata | 1.270 | 1.422 | 4.173 | 104.688 | 1.00x |
| wide_arrays.json | orjson | 1.516 | 1.808 | 1.871 | 104.688 | 0.79x |
| wide_arrays.json | msgspec | 2.630 | 2.800 | 3.954 | 104.688 | 0.51x |
| wide_arrays.json | ujson | 5.217 | 6.458 | 8.837 | 104.688 | 0.22x |
| wide_arrays.json | json | 12.582 | 13.485 | 22.893 | 104.688 | 0.11x |
| mixed.json | strata | 0.034 | 0.045 | 0.053 | 105.672 | 1.00x |
| mixed.json | orjson | 0.054 | 0.128 | 0.228 | 105.672 | 0.35x |
| mixed.json | msgspec | 0.058 | 0.068 | 0.247 | 105.672 | 0.66x |
| mixed.json | ujson | 0.181 | 0.191 | 0.209 | 105.672 | 0.23x |
| mixed.json | json | 0.379 | 0.402 | 0.637 | 105.672 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.169 | 6.244 | 8.179 | 94.188 | 1.00x |
| users.json | orjson | 8.990 | 9.160 | 11.290 | 94.188 | 0.68x |
| users.json | msgspec | 8.729 | 8.761 | 10.732 | 94.188 | 0.71x |
| users.json | ujson | 11.699 | 12.103 | 15.605 | 94.188 | 0.52x |
| users.json | json | 14.047 | 14.140 | 14.709 | 94.188 | 0.44x |
| flat.json | strata | 0.724 | 0.754 | 2.676 | 100.891 | 1.00x |
| flat.json | orjson | 1.046 | 1.268 | 2.043 | 100.891 | 0.59x |
| flat.json | msgspec | 0.877 | 1.031 | 3.316 | 100.891 | 0.73x |
| flat.json | ujson | 1.338 | 1.421 | 5.632 | 100.891 | 0.53x |
| flat.json | json | 1.563 | 1.625 | 4.453 | 100.891 | 0.46x |
| nested.json | strata | 0.622 | 0.683 | 1.730 | 100.891 | 1.00x |
| nested.json | orjson | 0.933 | 1.013 | 1.098 | 100.891 | 0.67x |
| nested.json | msgspec | 0.818 | 0.902 | 1.134 | 100.891 | 0.76x |
| nested.json | ujson | 1.168 | 1.218 | 1.530 | 100.891 | 0.56x |
| nested.json | json | 1.585 | 1.697 | 4.779 | 100.891 | 0.40x |
| wide_arrays.json | strata | 3.343 | 3.574 | 3.781 | 105.641 | 1.00x |
| wide_arrays.json | orjson | 3.836 | 4.719 | 8.215 | 105.641 | 0.76x |
| wide_arrays.json | msgspec | 4.467 | 5.068 | 6.799 | 105.641 | 0.71x |
| wide_arrays.json | ujson | 5.971 | 6.709 | 11.135 | 105.641 | 0.53x |
| wide_arrays.json | json | 7.438 | 8.045 | 10.139 | 105.641 | 0.44x |
| mixed.json | strata | 0.194 | 0.235 | 0.539 | 105.672 | 1.00x |
| mixed.json | orjson | 0.282 | 0.330 | 1.481 | 105.672 | 0.71x |
| mixed.json | msgspec | 0.276 | 0.306 | 0.612 | 105.672 | 0.77x |
| mixed.json | ujson | 0.353 | 0.533 | 0.642 | 105.672 | 0.44x |
| mixed.json | json | 0.453 | 0.476 | 0.522 | 105.672 | 0.49x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.151 | 8.149 | 19.357 | 100.891 | 1.00x |
| users.ndjson | orjson | 11.591 | 13.631 | 21.586 | 100.891 | 0.60x |
| users.ndjson | msgspec | 11.755 | 13.697 | 21.329 | 100.891 | 0.59x |
| users.ndjson | ujson | 14.530 | 18.219 | 22.906 | 100.891 | 0.45x |
| users.ndjson | json | 20.383 | 22.217 | 32.403 | 100.891 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.754 | 1.830 | 2.416 | 95.297 | 1.00x |
| users.json | orjson | 2.494 | 2.740 | 2.923 | 95.297 | 0.67x |
| users.json | msgspec | 3.125 | 3.279 | 5.549 | 95.297 | 0.56x |
| users.json | ujson | 8.827 | 9.385 | 15.677 | 95.297 | 0.20x |
| users.json | json | 15.645 | 16.411 | 24.053 | 95.297 | 0.11x |
| flat.json | strata | 0.425 | 0.609 | 1.051 | 100.891 | 1.00x |
| flat.json | orjson | 0.520 | 0.587 | 0.767 | 100.891 | 1.04x |
| flat.json | msgspec | 0.566 | 0.676 | 0.815 | 100.891 | 0.90x |
| flat.json | ujson | 0.955 | 1.280 | 3.111 | 100.891 | 0.48x |
| flat.json | json | 1.681 | 2.094 | 3.113 | 100.891 | 0.29x |
| nested.json | strata | 0.369 | 0.441 | 0.522 | 100.891 | 1.00x |
| nested.json | orjson | 0.531 | 0.603 | 1.305 | 100.891 | 0.73x |
| nested.json | msgspec | 0.714 | 0.884 | 0.993 | 100.891 | 0.50x |
| nested.json | ujson | 1.284 | 1.550 | 3.386 | 100.891 | 0.28x |
| nested.json | json | 2.013 | 2.276 | 3.310 | 100.891 | 0.19x |
| wide_arrays.json | strata | 1.538 | 1.911 | 2.472 | 105.656 | 1.00x |
| wide_arrays.json | orjson | 2.004 | 2.487 | 3.186 | 105.656 | 0.77x |
| wide_arrays.json | msgspec | 3.106 | 3.385 | 4.237 | 105.656 | 0.56x |
| wide_arrays.json | ujson | 6.122 | 6.475 | 7.491 | 105.656 | 0.30x |
| wide_arrays.json | json | 13.209 | 14.266 | 15.233 | 105.656 | 0.13x |
| mixed.json | strata | 0.239 | 0.328 | 0.415 | 105.672 | 1.00x |
| mixed.json | orjson | 0.313 | 0.392 | 0.597 | 105.672 | 0.84x |
| mixed.json | msgspec | 0.331 | 0.396 | 0.692 | 105.672 | 0.83x |
| mixed.json | ujson | 0.480 | 0.527 | 0.719 | 105.672 | 0.62x |
| mixed.json | json | 0.611 | 0.722 | 1.022 | 105.672 | 0.45x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.083 | 0.159 | 0.452 | 95.328 | 1.00x |
| users.json $[*].id | jmespath | 0.349 | 0.463 | 0.848 | 95.328 | 0.34x |
| users.json $[*].id | jsonpath-ng | 1.881 | 2.783 | 7.047 | 95.328 | 0.06x |
| users.json $[*].orders[*].total | strata | 0.505 | 1.030 | 3.099 | 95.438 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.027 | 2.765 | 6.974 | 95.438 | 0.37x |
| users.json $[*].orders[*].total | jsonpath-ng | 12.754 | 16.485 | 26.254 | 95.438 | 0.06x |
| users.json $..total | strata | 1.262 | 1.583 | 3.074 | 95.469 | 1.00x |
| users.json $..total | jsonpath-ng | 201.352 | 233.787 | 273.991 | 95.469 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.146 | 5.010 | 8.387 | 95.375 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.762 | 18.753 | 26.473 | 95.375 | 0.27x |
| users.json $[*].id | orjson+jsonpath-ng | 14.648 | 20.083 | 24.351 | 95.375 | 0.25x |
| users.json $[*].orders[*].total | strata | 3.798 | 4.237 | 5.939 | 95.469 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.382 | 13.721 | 18.779 | 95.469 | 0.31x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 26.371 | 28.886 | 30.469 | 95.469 | 0.15x |
| users.json $..total | strata | 7.772 | 8.857 | 19.701 | 95.500 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 214.144 | 219.696 | 255.908 | 95.500 | 0.04x |

