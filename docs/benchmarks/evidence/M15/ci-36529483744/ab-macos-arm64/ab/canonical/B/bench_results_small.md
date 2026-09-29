# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: afd1550cbabc9433e8292644444a23bb27bbc4e9
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
| users.json | strata | 6.650 | 8.046 | 14.219 | 69.250 | 1.00x |
| users.json | orjson | 10.157 | 13.206 | 21.522 | 69.250 | 0.61x |
| users.json | msgspec | 10.070 | 13.379 | 21.169 | 69.250 | 0.60x |
| users.json | ujson | 13.834 | 18.186 | 25.557 | 69.250 | 0.44x |
| users.json | pysimdjson | 135.039 | 164.500 | 207.074 | 69.250 | 0.05x |
| users.json | json | 16.968 | 20.877 | 33.074 | 69.250 | 0.39x |
| flat.json | strata | 0.583 | 0.605 | 0.656 | 102.516 | 1.00x |
| flat.json | orjson | 0.751 | 0.783 | 0.877 | 102.516 | 0.77x |
| flat.json | msgspec | 0.717 | 0.739 | 0.803 | 102.516 | 0.82x |
| flat.json | ujson | 1.133 | 1.229 | 1.413 | 102.516 | 0.49x |
| flat.json | pysimdjson | 11.900 | 12.057 | 12.369 | 102.516 | 0.05x |
| flat.json | json | 1.339 | 1.377 | 1.489 | 102.516 | 0.44x |
| nested.json | strata | 0.511 | 0.561 | 0.641 | 102.516 | 1.00x |
| nested.json | orjson | 0.730 | 0.797 | 0.914 | 102.516 | 0.70x |
| nested.json | msgspec | 0.688 | 0.743 | 0.824 | 102.516 | 0.75x |
| nested.json | ujson | 1.098 | 1.265 | 1.423 | 102.516 | 0.44x |
| nested.json | pysimdjson | 10.499 | 10.839 | 12.027 | 102.516 | 0.05x |
| nested.json | json | 1.423 | 1.518 | 1.804 | 102.516 | 0.37x |
| wide_arrays.json | strata | 3.442 | 4.224 | 6.958 | 105.281 | 1.00x |
| wide_arrays.json | orjson | 4.010 | 4.878 | 10.098 | 105.281 | 0.87x |
| wide_arrays.json | msgspec | 4.410 | 5.774 | 10.397 | 105.281 | 0.73x |
| wide_arrays.json | ujson | 5.775 | 6.973 | 10.811 | 105.281 | 0.61x |
| wide_arrays.json | pysimdjson | 70.520 | 84.329 | 104.359 | 105.281 | 0.05x |
| wide_arrays.json | json | 7.458 | 8.435 | 13.597 | 105.281 | 0.50x |
| mixed.json | strata | 0.135 | 0.166 | 0.437 | 105.297 | 1.00x |
| mixed.json | orjson | 0.169 | 0.212 | 0.458 | 105.297 | 0.78x |
| mixed.json | msgspec | 0.184 | 0.224 | 0.535 | 105.297 | 0.74x |
| mixed.json | ujson | 0.231 | 0.427 | 1.242 | 105.297 | 0.39x |
| mixed.json | pysimdjson | 2.604 | 3.339 | 6.333 | 105.297 | 0.05x |
| mixed.json | json | 0.343 | 0.435 | 1.001 | 105.297 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.672 | 2.104 | 5.444 | 84.391 | 1.00x |
| users.json | orjson | 2.440 | 2.960 | 5.913 | 84.391 | 0.71x |
| users.json | msgspec | 3.144 | 4.014 | 7.855 | 84.391 | 0.52x |
| users.json | ujson | 9.257 | 11.862 | 20.025 | 84.391 | 0.18x |
| users.json | json | 17.396 | 22.969 | 32.796 | 84.391 | 0.09x |
| flat.json | strata | 0.209 | 0.244 | 0.324 | 102.516 | 1.00x |
| flat.json | orjson | 0.256 | 0.279 | 0.313 | 102.516 | 0.87x |
| flat.json | msgspec | 0.317 | 0.348 | 0.378 | 102.516 | 0.70x |
| flat.json | ujson | 0.791 | 0.826 | 0.881 | 102.516 | 0.30x |
| flat.json | json | 1.420 | 1.585 | 1.812 | 102.516 | 0.15x |
| nested.json | strata | 0.133 | 0.157 | 0.184 | 102.516 | 1.00x |
| nested.json | orjson | 0.240 | 0.276 | 0.357 | 102.516 | 0.57x |
| nested.json | msgspec | 0.300 | 0.352 | 0.518 | 102.516 | 0.45x |
| nested.json | ujson | 0.836 | 1.124 | 1.320 | 102.516 | 0.14x |
| nested.json | json | 1.717 | 1.943 | 2.231 | 102.516 | 0.08x |
| wide_arrays.json | strata | 1.256 | 1.594 | 3.680 | 105.281 | 1.00x |
| wide_arrays.json | orjson | 1.578 | 1.798 | 4.184 | 105.281 | 0.89x |
| wide_arrays.json | msgspec | 2.346 | 3.021 | 5.524 | 105.281 | 0.53x |
| wide_arrays.json | ujson | 5.152 | 7.064 | 11.346 | 105.281 | 0.23x |
| wide_arrays.json | json | 12.426 | 16.690 | 21.670 | 105.281 | 0.10x |
| mixed.json | strata | 0.052 | 0.064 | 0.176 | 105.297 | 1.00x |
| mixed.json | orjson | 0.058 | 0.074 | 0.183 | 105.297 | 0.87x |
| mixed.json | msgspec | 0.067 | 0.100 | 0.385 | 105.297 | 0.64x |
| mixed.json | ujson | 0.186 | 0.228 | 0.581 | 105.297 | 0.28x |
| mixed.json | json | 0.398 | 0.464 | 1.129 | 105.297 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.286 | 9.724 | 14.820 | 96.078 | 1.00x |
| users.json | orjson | 10.576 | 18.396 | 27.331 | 96.078 | 0.53x |
| users.json | msgspec | 10.546 | 18.456 | 27.961 | 96.078 | 0.53x |
| users.json | ujson | 14.101 | 25.336 | 38.052 | 96.078 | 0.38x |
| users.json | json | 17.011 | 28.711 | 39.640 | 96.078 | 0.34x |
| flat.json | strata | 0.664 | 0.710 | 0.765 | 102.516 | 1.00x |
| flat.json | orjson | 0.941 | 1.053 | 1.236 | 102.516 | 0.67x |
| flat.json | msgspec | 0.825 | 0.893 | 0.949 | 102.516 | 0.80x |
| flat.json | ujson | 1.196 | 1.269 | 1.481 | 102.516 | 0.56x |
| flat.json | json | 1.467 | 1.539 | 1.664 | 102.516 | 0.46x |
| nested.json | strata | 0.554 | 0.668 | 0.766 | 102.516 | 1.00x |
| nested.json | orjson | 0.937 | 1.112 | 1.312 | 102.516 | 0.60x |
| nested.json | msgspec | 0.781 | 0.921 | 1.030 | 102.516 | 0.73x |
| nested.json | ujson | 1.065 | 1.240 | 1.384 | 102.516 | 0.54x |
| nested.json | json | 1.478 | 1.672 | 1.918 | 102.516 | 0.40x |
| wide_arrays.json | strata | 3.560 | 4.483 | 8.031 | 105.281 | 1.00x |
| wide_arrays.json | orjson | 4.197 | 5.450 | 9.065 | 105.281 | 0.82x |
| wide_arrays.json | msgspec | 4.697 | 5.727 | 10.455 | 105.281 | 0.78x |
| wide_arrays.json | ujson | 6.118 | 7.747 | 13.071 | 105.281 | 0.58x |
| wide_arrays.json | json | 7.780 | 11.052 | 16.716 | 105.281 | 0.41x |
| mixed.json | strata | 0.206 | 0.278 | 0.553 | 105.297 | 1.00x |
| mixed.json | orjson | 0.270 | 0.420 | 0.865 | 105.297 | 0.66x |
| mixed.json | msgspec | 0.288 | 0.383 | 0.894 | 105.297 | 0.72x |
| mixed.json | ujson | 0.348 | 0.499 | 1.026 | 105.297 | 0.56x |
| mixed.json | json | 0.453 | 0.572 | 0.773 | 105.297 | 0.48x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.176 | 8.581 | 14.738 | 102.516 | 1.00x |
| users.ndjson | orjson | 12.297 | 14.217 | 22.310 | 102.516 | 0.60x |
| users.ndjson | msgspec | 12.051 | 14.298 | 22.891 | 102.516 | 0.60x |
| users.ndjson | ujson | 14.810 | 17.703 | 29.337 | 102.516 | 0.48x |
| users.ndjson | json | 18.890 | 22.912 | 32.106 | 102.516 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.254 | 3.550 | 6.564 | 96.891 | 1.00x |
| users.json | orjson | 3.540 | 5.087 | 7.807 | 96.891 | 0.70x |
| users.json | msgspec | 4.248 | 6.391 | 9.104 | 96.891 | 0.56x |
| users.json | ujson | 10.803 | 16.269 | 22.325 | 96.891 | 0.22x |
| users.json | json | 18.574 | 29.524 | 36.616 | 96.891 | 0.12x |
| flat.json | strata | 0.357 | 0.446 | 0.542 | 102.516 | 1.00x |
| flat.json | orjson | 0.403 | 0.505 | 0.612 | 102.516 | 0.88x |
| flat.json | msgspec | 0.474 | 0.564 | 0.722 | 102.516 | 0.79x |
| flat.json | ujson | 0.987 | 1.067 | 1.243 | 102.516 | 0.42x |
| flat.json | json | 1.603 | 1.726 | 2.024 | 102.516 | 0.26x |
| nested.json | strata | 0.274 | 0.446 | 0.692 | 102.516 | 1.00x |
| nested.json | orjson | 0.392 | 0.585 | 0.776 | 102.516 | 0.76x |
| nested.json | msgspec | 0.492 | 0.724 | 1.277 | 102.516 | 0.62x |
| nested.json | ujson | 1.075 | 1.410 | 1.796 | 102.516 | 0.32x |
| nested.json | json | 1.961 | 2.397 | 3.296 | 102.516 | 0.19x |
| wide_arrays.json | strata | 1.594 | 2.086 | 3.636 | 105.281 | 1.00x |
| wide_arrays.json | orjson | 2.079 | 2.723 | 5.156 | 105.281 | 0.77x |
| wide_arrays.json | msgspec | 2.870 | 3.496 | 8.383 | 105.281 | 0.60x |
| wide_arrays.json | ujson | 6.206 | 7.344 | 9.551 | 105.281 | 0.28x |
| wide_arrays.json | json | 13.565 | 15.861 | 21.860 | 105.281 | 0.13x |
| mixed.json | strata | 0.237 | 0.330 | 0.443 | 105.297 | 1.00x |
| mixed.json | orjson | 0.240 | 0.366 | 0.577 | 105.297 | 0.90x |
| mixed.json | msgspec | 0.294 | 0.387 | 0.696 | 105.297 | 0.85x |
| mixed.json | ujson | 0.385 | 0.533 | 1.049 | 105.297 | 0.62x |
| mixed.json | json | 0.572 | 0.778 | 1.110 | 105.297 | 0.42x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.116 | 0.230 | 0.443 | 96.922 | 1.00x |
| users.json $[*].id | jmespath | 0.350 | 0.595 | 1.380 | 96.922 | 0.39x |
| users.json $[*].id | jsonpath-ng | 1.872 | 2.849 | 4.976 | 96.922 | 0.08x |
| users.json $[*].orders[*].total | strata | 0.489 | 1.160 | 2.529 | 97.094 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.370 | 3.293 | 6.240 | 97.094 | 0.35x |
| users.json $[*].orders[*].total | jsonpath-ng | 14.377 | 20.015 | 31.624 | 97.094 | 0.06x |
| users.json $..total | strata | 1.333 | 1.663 | 2.482 | 97.188 | 1.00x |
| users.json $..total | jsonpath-ng | 210.388 | 254.237 | 292.809 | 97.188 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.261 | 5.578 | 6.816 | 96.969 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.450 | 22.361 | 31.594 | 96.969 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 15.924 | 24.873 | 34.005 | 96.969 | 0.22x |
| users.json $[*].orders[*].total | strata | 4.100 | 4.978 | 6.132 | 97.125 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.281 | 19.219 | 29.509 | 97.125 | 0.26x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 25.014 | 39.746 | 54.603 | 97.125 | 0.13x |
| users.json $..total | strata | 8.402 | 10.808 | 20.520 | 97.188 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 206.484 | 270.476 | 386.001 | 97.188 | 0.04x |

