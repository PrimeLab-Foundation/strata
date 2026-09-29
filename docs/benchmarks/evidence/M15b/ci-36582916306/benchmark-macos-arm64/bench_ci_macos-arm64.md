# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 82e3fa5f24c38cfa1150440f0cf5da6c286c01cd
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
| users.json | strata | 6.064 | 6.699 | 8.756 | 68.156 | 1.00x |
| users.json | orjson | 9.208 | 10.051 | 11.450 | 68.156 | 0.67x |
| users.json | msgspec | 8.846 | 10.378 | 15.535 | 68.156 | 0.65x |
| users.json | ujson | 11.864 | 14.155 | 15.809 | 68.156 | 0.47x |
| users.json | pysimdjson | 121.981 | 130.169 | 139.834 | 68.156 | 0.05x |
| users.json | json | 14.592 | 16.105 | 17.624 | 68.156 | 0.42x |
| flat.json | strata | 0.562 | 0.568 | 0.609 | 99.828 | 1.00x |
| flat.json | orjson | 0.711 | 0.767 | 0.861 | 99.828 | 0.74x |
| flat.json | msgspec | 0.704 | 0.709 | 0.737 | 99.828 | 0.80x |
| flat.json | ujson | 1.141 | 1.186 | 1.221 | 99.828 | 0.48x |
| flat.json | pysimdjson | 11.654 | 11.758 | 12.191 | 99.828 | 0.05x |
| flat.json | json | 1.324 | 1.344 | 1.421 | 99.828 | 0.42x |
| nested.json | strata | 0.474 | 0.534 | 0.612 | 99.844 | 1.00x |
| nested.json | orjson | 0.647 | 0.783 | 0.839 | 99.844 | 0.68x |
| nested.json | msgspec | 0.634 | 0.712 | 0.820 | 99.844 | 0.75x |
| nested.json | ujson | 0.968 | 1.202 | 1.460 | 99.844 | 0.44x |
| nested.json | pysimdjson | 9.714 | 10.640 | 11.767 | 99.844 | 0.05x |
| nested.json | json | 1.285 | 1.439 | 1.536 | 99.844 | 0.37x |
| wide_arrays.json | strata | 2.785 | 3.006 | 3.210 | 102.641 | 1.00x |
| wide_arrays.json | orjson | 3.343 | 3.693 | 3.824 | 102.641 | 0.81x |
| wide_arrays.json | msgspec | 3.778 | 4.067 | 4.274 | 102.641 | 0.74x |
| wide_arrays.json | ujson | 4.911 | 5.189 | 5.367 | 102.641 | 0.58x |
| wide_arrays.json | pysimdjson | 59.877 | 62.919 | 66.076 | 102.641 | 0.05x |
| wide_arrays.json | json | 6.331 | 6.796 | 7.898 | 102.641 | 0.44x |
| mixed.json | strata | 0.120 | 0.123 | 0.196 | 102.656 | 1.00x |
| mixed.json | orjson | 0.153 | 0.155 | 0.428 | 102.656 | 0.79x |
| mixed.json | msgspec | 0.165 | 0.167 | 0.180 | 102.656 | 0.74x |
| mixed.json | ujson | 0.220 | 0.412 | 0.441 | 102.656 | 0.30x |
| mixed.json | pysimdjson | 2.469 | 2.494 | 2.947 | 102.656 | 0.05x |
| mixed.json | json | 0.314 | 0.324 | 0.475 | 102.656 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.385 | 1.465 | 1.788 | 75.641 | 1.00x |
| users.json | orjson | 2.179 | 2.311 | 2.483 | 75.641 | 0.63x |
| users.json | msgspec | 2.752 | 2.924 | 4.011 | 75.641 | 0.50x |
| users.json | ujson | 8.358 | 8.875 | 9.647 | 75.641 | 0.17x |
| users.json | json | 14.857 | 15.511 | 15.914 | 75.641 | 0.09x |
| flat.json | strata | 0.197 | 0.198 | 0.211 | 99.844 | 1.00x |
| flat.json | orjson | 0.235 | 0.237 | 0.248 | 99.844 | 0.84x |
| flat.json | msgspec | 0.293 | 0.294 | 0.298 | 99.844 | 0.67x |
| flat.json | ujson | 0.709 | 0.710 | 0.732 | 99.844 | 0.28x |
| flat.json | json | 1.312 | 1.322 | 1.391 | 99.844 | 0.15x |
| nested.json | strata | 0.113 | 0.115 | 0.154 | 99.875 | 1.00x |
| nested.json | orjson | 0.200 | 0.208 | 0.273 | 99.875 | 0.55x |
| nested.json | msgspec | 0.267 | 0.300 | 0.507 | 99.875 | 0.38x |
| nested.json | ujson | 0.746 | 0.775 | 1.029 | 99.875 | 0.15x |
| nested.json | json | 1.536 | 1.553 | 1.842 | 99.875 | 0.07x |
| wide_arrays.json | strata | 1.074 | 1.109 | 1.533 | 102.641 | 1.00x |
| wide_arrays.json | orjson | 1.379 | 1.555 | 1.624 | 102.641 | 0.71x |
| wide_arrays.json | msgspec | 2.094 | 2.182 | 2.887 | 102.641 | 0.51x |
| wide_arrays.json | ujson | 4.709 | 5.066 | 6.456 | 102.641 | 0.22x |
| wide_arrays.json | json | 11.613 | 12.170 | 13.134 | 102.641 | 0.09x |
| mixed.json | strata | 0.034 | 0.035 | 0.043 | 102.656 | 1.00x |
| mixed.json | orjson | 0.044 | 0.103 | 0.270 | 102.656 | 0.34x |
| mixed.json | msgspec | 0.049 | 0.053 | 0.064 | 102.656 | 0.67x |
| mixed.json | ujson | 0.165 | 0.167 | 0.174 | 102.656 | 0.21x |
| mixed.json | json | 0.341 | 0.346 | 0.359 | 102.656 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.517 | 6.834 | 8.834 | 92.969 | 1.00x |
| users.json | orjson | 9.635 | 9.761 | 10.238 | 92.969 | 0.70x |
| users.json | msgspec | 9.294 | 9.512 | 13.684 | 92.969 | 0.72x |
| users.json | ujson | 12.541 | 13.092 | 14.932 | 92.969 | 0.52x |
| users.json | json | 15.104 | 15.434 | 20.599 | 92.969 | 0.44x |
| flat.json | strata | 0.569 | 0.571 | 0.580 | 99.844 | 1.00x |
| flat.json | orjson | 0.773 | 0.784 | 0.879 | 99.844 | 0.73x |
| flat.json | msgspec | 0.721 | 0.723 | 0.752 | 99.844 | 0.79x |
| flat.json | ujson | 1.046 | 1.052 | 1.117 | 99.844 | 0.54x |
| flat.json | json | 1.306 | 1.312 | 1.366 | 99.844 | 0.44x |
| nested.json | strata | 0.505 | 0.509 | 0.543 | 99.875 | 1.00x |
| nested.json | orjson | 0.752 | 0.783 | 0.830 | 99.875 | 0.65x |
| nested.json | msgspec | 0.686 | 0.688 | 0.750 | 99.875 | 0.74x |
| nested.json | ujson | 0.950 | 0.959 | 0.987 | 99.875 | 0.53x |
| nested.json | json | 1.341 | 1.353 | 1.406 | 99.875 | 0.38x |
| wide_arrays.json | strata | 3.088 | 3.114 | 3.374 | 102.641 | 1.00x |
| wide_arrays.json | orjson | 3.683 | 3.805 | 4.227 | 102.641 | 0.82x |
| wide_arrays.json | msgspec | 4.297 | 4.368 | 4.428 | 102.641 | 0.71x |
| wide_arrays.json | ujson | 5.523 | 5.728 | 5.908 | 102.641 | 0.54x |
| wide_arrays.json | json | 6.946 | 7.052 | 7.405 | 102.641 | 0.44x |
| mixed.json | strata | 0.142 | 0.154 | 0.196 | 102.656 | 1.00x |
| mixed.json | orjson | 0.201 | 0.336 | 0.419 | 102.656 | 0.46x |
| mixed.json | msgspec | 0.207 | 0.228 | 0.246 | 102.656 | 0.67x |
| mixed.json | ujson | 0.247 | 0.261 | 0.330 | 102.656 | 0.59x |
| mixed.json | json | 0.354 | 0.368 | 0.422 | 102.656 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.457 | 6.533 | 6.980 | 99.812 | 1.00x |
| users.ndjson | orjson | 11.086 | 11.210 | 11.726 | 99.812 | 0.58x |
| users.ndjson | msgspec | 11.030 | 11.072 | 11.673 | 99.812 | 0.59x |
| users.ndjson | ujson | 13.678 | 13.817 | 16.127 | 99.812 | 0.47x |
| users.ndjson | json | 17.777 | 17.989 | 19.121 | 99.812 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.872 | 1.950 | 2.242 | 94.141 | 1.00x |
| users.json | orjson | 2.657 | 2.751 | 2.826 | 94.141 | 0.71x |
| users.json | msgspec | 3.462 | 3.491 | 3.757 | 94.141 | 0.56x |
| users.json | ujson | 9.422 | 9.690 | 10.221 | 94.141 | 0.20x |
| users.json | json | 15.992 | 16.427 | 17.194 | 94.141 | 0.12x |
| flat.json | strata | 0.301 | 0.326 | 0.371 | 99.844 | 1.00x |
| flat.json | orjson | 0.348 | 0.379 | 0.532 | 99.844 | 0.86x |
| flat.json | msgspec | 0.429 | 0.442 | 0.476 | 99.844 | 0.74x |
| flat.json | ujson | 0.861 | 0.882 | 0.969 | 99.844 | 0.37x |
| flat.json | json | 1.415 | 1.520 | 1.574 | 99.844 | 0.21x |
| nested.json | strata | 0.221 | 0.247 | 0.363 | 99.875 | 1.00x |
| nested.json | orjson | 0.322 | 0.348 | 0.539 | 99.875 | 0.71x |
| nested.json | msgspec | 0.423 | 0.549 | 0.687 | 99.875 | 0.45x |
| nested.json | ujson | 0.897 | 1.009 | 1.240 | 99.875 | 0.24x |
| nested.json | json | 1.679 | 1.716 | 2.392 | 99.875 | 0.14x |
| wide_arrays.json | strata | 1.477 | 1.517 | 1.751 | 102.641 | 1.00x |
| wide_arrays.json | orjson | 1.797 | 2.019 | 2.119 | 102.641 | 0.75x |
| wide_arrays.json | msgspec | 2.544 | 2.617 | 3.112 | 102.641 | 0.58x |
| wide_arrays.json | ujson | 5.354 | 5.525 | 5.759 | 102.641 | 0.27x |
| wide_arrays.json | json | 12.303 | 12.467 | 12.884 | 102.641 | 0.12x |
| mixed.json | strata | 0.123 | 0.145 | 0.177 | 102.656 | 1.00x |
| mixed.json | orjson | 0.144 | 0.155 | 0.210 | 102.656 | 0.93x |
| mixed.json | msgspec | 0.150 | 0.160 | 0.179 | 102.656 | 0.91x |
| mixed.json | ujson | 0.273 | 0.292 | 0.311 | 102.656 | 0.50x |
| mixed.json | json | 0.451 | 0.474 | 0.493 | 102.656 | 0.31x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.053 | 0.058 | 0.061 | 94.172 | 1.00x |
| users.json $[*].id | jmespath | 0.276 | 0.287 | 0.309 | 94.172 | 0.20x |
| users.json $[*].id | jsonpath-ng | 1.498 | 1.523 | 1.588 | 94.172 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.285 | 0.297 | 0.480 | 94.281 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.617 | 1.677 | 1.737 | 94.281 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 10.042 | 10.253 | 10.468 | 94.281 | 0.03x |
| users.json $..total | strata | 1.205 | 1.386 | 1.541 | 94.391 | 1.00x |
| users.json $..total | jsonpath-ng | 177.823 | 186.587 | 203.713 | 94.391 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.498 | 3.556 | 3.645 | 94.250 | 1.00x |
| users.json $[*].id | orjson+jmespath | 9.581 | 9.969 | 10.284 | 94.250 | 0.36x |
| users.json $[*].id | orjson+jsonpath-ng | 10.840 | 11.211 | 11.483 | 94.250 | 0.32x |
| users.json $[*].orders[*].total | strata | 3.588 | 3.667 | 4.397 | 94.359 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 11.033 | 11.294 | 12.053 | 94.359 | 0.32x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 21.544 | 22.164 | 23.974 | 94.359 | 0.17x |
| users.json $..total | strata | 8.212 | 9.097 | 9.632 | 94.422 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 205.560 | 211.283 | 230.174 | 94.422 | 0.04x |

