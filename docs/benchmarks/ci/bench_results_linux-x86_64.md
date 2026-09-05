# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: b7f31bb
- python: 3.12.14
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
| users.json | strata | 9.518 | 9.576 | 14.107 | 59.312 | 1.00x |
| users.json | orjson | 12.988 | 13.047 | 16.519 | 59.312 | 0.73x |
| users.json | msgspec | 12.922 | 13.079 | 15.973 | 59.312 | 0.73x |
| users.json | ujson | 17.475 | 17.668 | 22.412 | 59.312 | 0.54x |
| users.json | pysimdjson | 17.803 | 18.260 | 22.010 | 59.312 | 0.52x |
| users.json | json | 22.464 | 22.803 | 24.102 | 59.312 | 0.42x |
| flat.json | strata | 0.825 | 0.844 | 0.892 | 72.746 | 1.00x |
| flat.json | orjson | 0.974 | 0.986 | 1.269 | 72.746 | 0.86x |
| flat.json | msgspec | 1.004 | 1.022 | 1.062 | 72.746 | 0.83x |
| flat.json | ujson | 1.462 | 1.528 | 1.715 | 72.746 | 0.55x |
| flat.json | pysimdjson | 1.503 | 1.521 | 1.670 | 72.746 | 0.56x |
| flat.json | json | 1.923 | 1.945 | 1.986 | 72.746 | 0.43x |
| nested.json | strata | 0.791 | 0.807 | 0.849 | 72.746 | 1.00x |
| nested.json | orjson | 1.009 | 1.022 | 1.039 | 72.746 | 0.79x |
| nested.json | msgspec | 1.035 | 1.040 | 1.088 | 72.746 | 0.78x |
| nested.json | ujson | 1.456 | 1.545 | 1.609 | 72.746 | 0.52x |
| nested.json | pysimdjson | 1.407 | 1.419 | 1.485 | 72.746 | 0.57x |
| nested.json | json | 2.055 | 2.070 | 2.221 | 72.746 | 0.39x |
| wide_arrays.json | strata | 4.358 | 4.500 | 4.772 | 78.750 | 1.00x |
| wide_arrays.json | orjson | 5.178 | 5.363 | 5.914 | 78.750 | 0.84x |
| wide_arrays.json | msgspec | 5.772 | 5.864 | 6.151 | 78.750 | 0.77x |
| wide_arrays.json | ujson | 6.980 | 7.327 | 7.539 | 78.750 | 0.61x |
| wide_arrays.json | pysimdjson | 6.085 | 6.467 | 6.884 | 78.750 | 0.70x |
| wide_arrays.json | json | 9.815 | 10.172 | 10.777 | 78.750 | 0.44x |
| mixed.json | strata | 0.188 | 0.192 | 0.211 | 78.812 | 1.00x |
| mixed.json | orjson | 0.227 | 0.232 | 0.432 | 78.812 | 0.83x |
| mixed.json | msgspec | 0.236 | 0.242 | 0.265 | 78.812 | 0.80x |
| mixed.json | ujson | 0.296 | 0.317 | 0.338 | 78.812 | 0.61x |
| mixed.json | pysimdjson | 0.292 | 0.298 | 0.310 | 78.812 | 0.65x |
| mixed.json | json | 0.473 | 0.486 | 0.498 | 78.812 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.395 | 2.409 | 2.603 | 57.754 | 1.00x |
| users.json | orjson | 2.895 | 2.919 | 2.967 | 57.754 | 0.83x |
| users.json | msgspec | 3.799 | 3.824 | 3.852 | 57.754 | 0.63x |
| users.json | ujson | 11.175 | 11.360 | 11.551 | 57.754 | 0.21x |
| users.json | json | 21.638 | 21.697 | 22.048 | 57.754 | 0.11x |
| flat.json | strata | 0.265 | 0.268 | 0.299 | 72.746 | 1.00x |
| flat.json | orjson | 0.330 | 0.339 | 0.344 | 72.746 | 0.79x |
| flat.json | msgspec | 0.436 | 0.445 | 0.462 | 72.746 | 0.60x |
| flat.json | ujson | 1.008 | 1.015 | 1.032 | 72.746 | 0.26x |
| flat.json | json | 1.835 | 1.847 | 1.886 | 72.746 | 0.15x |
| nested.json | strata | 0.256 | 0.259 | 0.271 | 72.746 | 1.00x |
| nested.json | orjson | 0.294 | 0.297 | 0.310 | 72.746 | 0.87x |
| nested.json | msgspec | 0.403 | 0.418 | 0.441 | 72.746 | 0.62x |
| nested.json | ujson | 1.071 | 1.080 | 1.305 | 72.746 | 0.24x |
| nested.json | json | 2.409 | 2.436 | 2.538 | 72.746 | 0.11x |
| wide_arrays.json | strata | 1.501 | 1.524 | 1.566 | 78.750 | 1.00x |
| wide_arrays.json | orjson | 1.793 | 1.833 | 1.873 | 78.750 | 0.83x |
| wide_arrays.json | msgspec | 2.677 | 2.680 | 2.734 | 78.750 | 0.57x |
| wide_arrays.json | ujson | 6.324 | 6.372 | 6.454 | 78.750 | 0.24x |
| wide_arrays.json | json | 16.537 | 16.764 | 16.900 | 78.750 | 0.09x |
| mixed.json | strata | 0.061 | 0.062 | 0.067 | 78.844 | 1.00x |
| mixed.json | orjson | 0.064 | 0.065 | 0.068 | 78.844 | 0.96x |
| mixed.json | msgspec | 0.083 | 0.084 | 0.098 | 78.844 | 0.74x |
| mixed.json | ujson | 0.225 | 0.227 | 0.254 | 78.844 | 0.27x |
| mixed.json | json | 0.501 | 0.508 | 0.541 | 78.844 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.169 | 10.920 | 11.903 | 72.203 | 1.00x |
| users.json | orjson | 13.287 | 14.006 | 14.395 | 72.203 | 0.78x |
| users.json | msgspec | 13.544 | 14.005 | 14.298 | 72.203 | 0.78x |
| users.json | ujson | 18.462 | 19.338 | 20.560 | 72.203 | 0.56x |
| users.json | json | 22.980 | 23.921 | 24.171 | 72.203 | 0.46x |
| flat.json | strata | 0.872 | 0.897 | 0.921 | 72.746 | 1.00x |
| flat.json | orjson | 1.043 | 1.066 | 1.089 | 72.746 | 0.84x |
| flat.json | msgspec | 1.069 | 1.100 | 1.192 | 72.746 | 0.82x |
| flat.json | ujson | 1.565 | 1.653 | 1.740 | 72.746 | 0.54x |
| flat.json | json | 1.994 | 2.017 | 2.088 | 72.746 | 0.44x |
| nested.json | strata | 0.835 | 0.856 | 0.883 | 72.746 | 1.00x |
| nested.json | orjson | 1.061 | 1.095 | 1.141 | 72.746 | 0.78x |
| nested.json | msgspec | 1.071 | 1.096 | 1.148 | 72.746 | 0.78x |
| nested.json | ujson | 1.548 | 1.574 | 1.655 | 72.746 | 0.54x |
| nested.json | json | 2.090 | 2.116 | 2.253 | 72.746 | 0.40x |
| wide_arrays.json | strata | 4.572 | 4.729 | 4.860 | 78.812 | 1.00x |
| wide_arrays.json | orjson | 5.268 | 5.555 | 5.753 | 78.812 | 0.85x |
| wide_arrays.json | msgspec | 5.916 | 6.043 | 6.275 | 78.812 | 0.78x |
| wide_arrays.json | ujson | 7.323 | 7.551 | 7.748 | 78.812 | 0.63x |
| wide_arrays.json | json | 10.101 | 10.309 | 10.569 | 78.812 | 0.46x |
| mixed.json | strata | 0.210 | 0.212 | 0.236 | 78.844 | 1.00x |
| mixed.json | orjson | 0.272 | 0.277 | 0.294 | 78.844 | 0.77x |
| mixed.json | msgspec | 0.280 | 0.286 | 0.300 | 78.844 | 0.74x |
| mixed.json | ujson | 0.352 | 0.363 | 0.381 | 78.844 | 0.58x |
| mixed.json | json | 0.514 | 0.527 | 0.542 | 78.844 | 0.40x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.027 | 10.962 | 13.116 | 72.746 | 1.00x |
| users.ndjson | orjson | 16.509 | 16.954 | 19.638 | 72.746 | 0.65x |
| users.ndjson | msgspec | 16.479 | 16.989 | 20.146 | 72.746 | 0.65x |
| users.ndjson | ujson | 21.526 | 22.713 | 25.525 | 72.746 | 0.48x |
| users.ndjson | json | 29.440 | 30.681 | 32.445 | 72.746 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.936 | 3.011 | 3.275 | 69.223 | 1.00x |
| users.json | orjson | 3.500 | 3.631 | 3.887 | 69.223 | 0.83x |
| users.json | msgspec | 4.390 | 4.505 | 4.620 | 69.223 | 0.67x |
| users.json | ujson | 12.080 | 12.229 | 12.931 | 69.223 | 0.25x |
| users.json | json | 22.517 | 22.850 | 23.612 | 69.223 | 0.13x |
| flat.json | strata | 0.407 | 0.434 | 0.506 | 72.746 | 1.00x |
| flat.json | orjson | 0.488 | 0.523 | 0.599 | 72.746 | 0.83x |
| flat.json | msgspec | 0.613 | 0.632 | 0.710 | 72.746 | 0.69x |
| flat.json | ujson | 1.189 | 1.220 | 1.328 | 72.746 | 0.36x |
| flat.json | json | 2.047 | 2.079 | 2.192 | 72.746 | 0.21x |
| nested.json | strata | 0.374 | 0.392 | 0.414 | 72.746 | 1.00x |
| nested.json | orjson | 0.425 | 0.429 | 0.431 | 72.746 | 0.92x |
| nested.json | msgspec | 0.549 | 0.553 | 0.572 | 72.746 | 0.71x |
| nested.json | ujson | 1.219 | 1.227 | 1.276 | 72.746 | 0.32x |
| nested.json | json | 2.546 | 2.557 | 2.619 | 72.746 | 0.15x |
| wide_arrays.json | strata | 1.966 | 1.980 | 2.017 | 78.812 | 1.00x |
| wide_arrays.json | orjson | 2.309 | 2.318 | 2.348 | 78.812 | 0.85x |
| wide_arrays.json | msgspec | 3.119 | 3.181 | 3.309 | 78.812 | 0.62x |
| wide_arrays.json | ujson | 6.914 | 6.995 | 7.128 | 78.812 | 0.28x |
| wide_arrays.json | json | 17.154 | 17.234 | 17.385 | 78.812 | 0.11x |
| mixed.json | strata | 0.151 | 0.163 | 0.180 | 78.844 | 1.00x |
| mixed.json | orjson | 0.169 | 0.176 | 0.186 | 78.844 | 0.92x |
| mixed.json | msgspec | 0.187 | 0.195 | 0.261 | 78.844 | 0.83x |
| mixed.json | ujson | 0.345 | 0.355 | 0.377 | 78.844 | 0.46x |
| mixed.json | json | 0.623 | 0.649 | 0.663 | 78.844 | 0.25x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.060 | 0.062 | 0.066 | 69.223 | 1.00x |
| users.json $[*].id | jmespath | 0.488 | 0.501 | 0.521 | 69.223 | 0.12x |
| users.json $[*].id | jsonpath-ng | 2.769 | 2.817 | 3.116 | 69.223 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.445 | 0.451 | 0.465 | 71.223 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.114 | 3.130 | 3.200 | 71.223 | 0.14x |
| users.json $[*].orders[*].total | jsonpath-ng | 19.259 | 19.769 | 20.562 | 71.223 | 0.02x |
| users.json $..total | strata | 1.647 | 1.667 | 1.994 | 72.316 | 1.00x |
| users.json $..total | jsonpath-ng | 386.994 | 390.868 | 394.516 | 72.316 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.108 | 3.182 | 3.227 | 71.223 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.908 | 14.419 | 14.688 | 71.223 | 0.22x |
| users.json $[*].id | orjson+jsonpath-ng | 16.280 | 16.850 | 17.125 | 71.223 | 0.19x |
| users.json $[*].orders[*].total | strata | 3.375 | 3.414 | 3.467 | 72.316 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 16.884 | 17.196 | 17.963 | 72.316 | 0.20x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 36.431 | 38.353 | 40.705 | 72.316 | 0.09x |
| users.json $..total | strata | 12.907 | 13.532 | 14.546 | 74.488 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 408.550 | 411.002 | 414.847 | 74.488 | 0.03x |

