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
| users.json | strata | 5.873 | 6.848 | 8.147 | 70.734 | 1.00x |
| users.json | orjson | 8.675 | 10.396 | 12.834 | 70.734 | 0.66x |
| users.json | msgspec | 8.358 | 10.273 | 12.044 | 70.734 | 0.67x |
| users.json | ujson | 11.288 | 13.933 | 17.047 | 70.734 | 0.49x |
| users.json | pysimdjson | 118.136 | 130.049 | 146.359 | 70.734 | 0.05x |
| users.json | json | 13.862 | 16.577 | 19.364 | 70.734 | 0.41x |
| flat.json | strata | 0.585 | 0.624 | 0.739 | 95.953 | 1.00x |
| flat.json | orjson | 0.776 | 0.832 | 1.126 | 95.953 | 0.75x |
| flat.json | msgspec | 0.717 | 0.758 | 0.859 | 95.953 | 0.82x |
| flat.json | ujson | 1.218 | 1.313 | 1.535 | 95.953 | 0.47x |
| flat.json | pysimdjson | 12.097 | 12.700 | 14.840 | 95.953 | 0.05x |
| flat.json | json | 1.349 | 1.435 | 1.730 | 95.953 | 0.43x |
| nested.json | strata | 0.578 | 0.685 | 1.163 | 95.953 | 1.00x |
| nested.json | orjson | 0.788 | 0.963 | 1.949 | 95.953 | 0.71x |
| nested.json | msgspec | 0.727 | 0.871 | 1.338 | 95.953 | 0.79x |
| nested.json | ujson | 1.112 | 1.424 | 2.238 | 95.953 | 0.48x |
| nested.json | pysimdjson | 11.037 | 12.976 | 19.264 | 95.953 | 0.05x |
| nested.json | json | 1.488 | 1.760 | 2.840 | 95.953 | 0.39x |
| wide_arrays.json | strata | 3.257 | 4.560 | 9.731 | 98.719 | 1.00x |
| wide_arrays.json | orjson | 4.120 | 5.464 | 9.285 | 98.719 | 0.83x |
| wide_arrays.json | msgspec | 4.695 | 6.078 | 12.881 | 98.719 | 0.75x |
| wide_arrays.json | ujson | 5.805 | 9.015 | 18.262 | 98.719 | 0.51x |
| wide_arrays.json | pysimdjson | 78.495 | 105.759 | 141.495 | 98.719 | 0.04x |
| wide_arrays.json | json | 7.175 | 10.428 | 18.373 | 98.719 | 0.44x |
| mixed.json | strata | 0.117 | 0.131 | 0.164 | 98.734 | 1.00x |
| mixed.json | orjson | 0.149 | 0.173 | 0.210 | 98.734 | 0.76x |
| mixed.json | msgspec | 0.163 | 0.188 | 0.228 | 98.734 | 0.70x |
| mixed.json | ujson | 0.206 | 0.260 | 0.403 | 98.734 | 0.50x |
| mixed.json | pysimdjson | 2.364 | 2.589 | 2.899 | 98.734 | 0.05x |
| mixed.json | json | 0.311 | 0.346 | 0.413 | 98.734 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.415 | 1.610 | 1.877 | 74.984 | 1.00x |
| users.json | orjson | 2.221 | 2.546 | 2.855 | 74.984 | 0.63x |
| users.json | msgspec | 2.745 | 2.992 | 3.542 | 74.984 | 0.54x |
| users.json | ujson | 8.285 | 8.994 | 10.210 | 74.984 | 0.18x |
| users.json | json | 14.773 | 16.076 | 17.914 | 74.984 | 0.10x |
| flat.json | strata | 0.250 | 0.284 | 0.664 | 95.953 | 1.00x |
| flat.json | orjson | 0.276 | 0.308 | 0.440 | 95.953 | 0.92x |
| flat.json | msgspec | 0.340 | 0.369 | 0.461 | 95.953 | 0.77x |
| flat.json | ujson | 0.826 | 0.931 | 1.436 | 95.953 | 0.30x |
| flat.json | json | 1.460 | 1.655 | 2.363 | 95.953 | 0.17x |
| nested.json | strata | 0.145 | 0.170 | 0.539 | 95.969 | 1.00x |
| nested.json | orjson | 0.248 | 0.295 | 0.686 | 95.969 | 0.58x |
| nested.json | msgspec | 0.321 | 0.501 | 0.959 | 95.969 | 0.34x |
| nested.json | ujson | 0.943 | 1.157 | 2.011 | 95.969 | 0.15x |
| nested.json | json | 1.727 | 2.087 | 3.465 | 95.969 | 0.08x |
| wide_arrays.json | strata | 1.256 | 1.522 | 2.826 | 98.719 | 1.00x |
| wide_arrays.json | orjson | 1.614 | 1.900 | 3.695 | 98.719 | 0.80x |
| wide_arrays.json | msgspec | 2.437 | 2.765 | 4.311 | 98.719 | 0.55x |
| wide_arrays.json | ujson | 5.291 | 6.143 | 9.546 | 98.719 | 0.25x |
| wide_arrays.json | json | 12.707 | 14.379 | 21.597 | 98.719 | 0.11x |
| mixed.json | strata | 0.035 | 0.042 | 0.066 | 98.734 | 1.00x |
| mixed.json | orjson | 0.045 | 0.052 | 0.104 | 98.734 | 0.82x |
| mixed.json | msgspec | 0.050 | 0.061 | 0.084 | 98.734 | 0.70x |
| mixed.json | ujson | 0.164 | 0.187 | 0.224 | 98.734 | 0.23x |
| mixed.json | json | 0.340 | 0.381 | 0.481 | 98.734 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.447 | 7.352 | 9.919 | 86.672 | 1.00x |
| users.json | orjson | 9.463 | 11.416 | 16.405 | 86.672 | 0.64x |
| users.json | msgspec | 9.160 | 10.994 | 20.523 | 86.672 | 0.67x |
| users.json | ujson | 12.482 | 15.490 | 22.484 | 86.672 | 0.47x |
| users.json | json | 14.974 | 17.475 | 25.156 | 86.672 | 0.42x |
| flat.json | strata | 0.689 | 0.840 | 1.622 | 95.953 | 1.00x |
| flat.json | orjson | 1.043 | 1.207 | 1.804 | 95.953 | 0.70x |
| flat.json | msgspec | 0.875 | 1.024 | 1.450 | 95.953 | 0.82x |
| flat.json | ujson | 1.262 | 1.465 | 2.340 | 95.953 | 0.57x |
| flat.json | json | 1.507 | 1.746 | 2.997 | 95.953 | 0.48x |
| nested.json | strata | 0.633 | 0.768 | 1.479 | 95.969 | 1.00x |
| nested.json | orjson | 1.028 | 1.183 | 1.394 | 95.969 | 0.65x |
| nested.json | msgspec | 0.881 | 1.014 | 1.160 | 95.969 | 0.76x |
| nested.json | ujson | 1.199 | 1.384 | 2.422 | 95.969 | 0.55x |
| nested.json | json | 1.699 | 1.862 | 2.555 | 95.969 | 0.41x |
| wide_arrays.json | strata | 3.221 | 3.836 | 5.331 | 98.719 | 1.00x |
| wide_arrays.json | orjson | 3.970 | 4.661 | 6.276 | 98.719 | 0.82x |
| wide_arrays.json | msgspec | 4.415 | 5.160 | 6.965 | 98.719 | 0.74x |
| wide_arrays.json | ujson | 5.710 | 6.731 | 8.553 | 98.719 | 0.57x |
| wide_arrays.json | json | 7.145 | 8.376 | 10.241 | 98.719 | 0.46x |
| mixed.json | strata | 0.134 | 0.206 | 0.289 | 98.734 | 1.00x |
| mixed.json | orjson | 0.194 | 0.388 | 0.584 | 98.734 | 0.53x |
| mixed.json | msgspec | 0.196 | 0.306 | 0.439 | 98.734 | 0.67x |
| mixed.json | ujson | 0.243 | 0.371 | 0.638 | 98.734 | 0.56x |
| mixed.json | json | 0.337 | 0.452 | 0.611 | 98.734 | 0.46x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.687 | 7.956 | 9.523 | 95.953 | 1.00x |
| users.ndjson | orjson | 11.064 | 13.800 | 19.122 | 95.953 | 0.58x |
| users.ndjson | msgspec | 11.154 | 13.520 | 17.685 | 95.953 | 0.59x |
| users.ndjson | ujson | 14.459 | 16.728 | 22.208 | 95.953 | 0.48x |
| users.ndjson | json | 18.001 | 22.021 | 29.993 | 95.953 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.766 | 2.182 | 2.752 | 90.297 | 1.00x |
| users.json | orjson | 2.782 | 3.346 | 4.328 | 90.297 | 0.65x |
| users.json | msgspec | 3.342 | 3.972 | 4.640 | 90.297 | 0.55x |
| users.json | ujson | 9.259 | 10.579 | 12.480 | 90.297 | 0.21x |
| users.json | json | 16.115 | 18.508 | 22.401 | 90.297 | 0.12x |
| flat.json | strata | 0.502 | 0.640 | 1.437 | 95.953 | 1.00x |
| flat.json | orjson | 0.546 | 0.692 | 1.413 | 95.953 | 0.93x |
| flat.json | msgspec | 0.589 | 0.731 | 1.539 | 95.953 | 0.88x |
| flat.json | ujson | 1.050 | 1.339 | 2.736 | 95.953 | 0.48x |
| flat.json | json | 1.773 | 2.295 | 4.111 | 95.953 | 0.28x |
| nested.json | strata | 0.331 | 0.446 | 0.537 | 95.969 | 1.00x |
| nested.json | orjson | 0.461 | 0.605 | 0.985 | 95.969 | 0.74x |
| nested.json | msgspec | 0.621 | 0.799 | 1.105 | 95.969 | 0.56x |
| nested.json | ujson | 1.156 | 1.438 | 2.082 | 95.969 | 0.31x |
| nested.json | json | 2.047 | 2.323 | 2.874 | 95.969 | 0.19x |
| wide_arrays.json | strata | 1.432 | 1.816 | 2.645 | 98.719 | 1.00x |
| wide_arrays.json | orjson | 1.863 | 2.263 | 2.625 | 98.719 | 0.80x |
| wide_arrays.json | msgspec | 2.734 | 3.061 | 4.058 | 98.719 | 0.59x |
| wide_arrays.json | ujson | 5.499 | 6.403 | 8.159 | 98.719 | 0.28x |
| wide_arrays.json | json | 12.752 | 14.209 | 17.160 | 98.719 | 0.13x |
| mixed.json | strata | 0.107 | 0.139 | 0.320 | 98.734 | 1.00x |
| mixed.json | orjson | 0.124 | 0.148 | 0.289 | 98.734 | 0.93x |
| mixed.json | msgspec | 0.134 | 0.159 | 0.356 | 98.734 | 0.87x |
| mixed.json | ujson | 0.249 | 0.289 | 0.508 | 98.734 | 0.48x |
| mixed.json | json | 0.418 | 0.472 | 0.758 | 98.734 | 0.29x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.051 | 0.079 | 0.158 | 90.328 | 1.00x |
| users.json $[*].id | jmespath | 0.266 | 0.339 | 0.423 | 90.328 | 0.23x |
| users.json $[*].id | jsonpath-ng | 1.465 | 1.683 | 1.940 | 90.328 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.312 | 0.495 | 0.860 | 90.453 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.660 | 2.039 | 2.553 | 90.453 | 0.24x |
| users.json $[*].orders[*].total | jsonpath-ng | 10.484 | 11.789 | 13.680 | 90.453 | 0.04x |
| users.json $..total | strata | 1.216 | 1.530 | 1.834 | 90.562 | 1.00x |
| users.json $..total | jsonpath-ng | 186.723 | 200.982 | 238.912 | 90.562 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.644 | 3.952 | 5.012 | 90.375 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.269 | 12.021 | 17.459 | 90.375 | 0.33x |
| users.json $[*].id | orjson+jsonpath-ng | 11.101 | 13.508 | 16.892 | 90.375 | 0.29x |
| users.json $[*].orders[*].total | strata | 3.425 | 4.017 | 4.456 | 90.531 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 10.546 | 12.934 | 14.814 | 90.531 | 0.31x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 20.683 | 27.225 | 30.474 | 90.531 | 0.15x |
| users.json $..total | strata | 7.488 | 8.724 | 10.388 | 90.562 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 192.133 | 213.116 | 248.769 | 90.562 | 0.04x |

