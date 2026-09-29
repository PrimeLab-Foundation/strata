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
| users.json | strata | 6.369 | 7.696 | 11.665 | 69.203 | 1.00x |
| users.json | orjson | 9.701 | 11.908 | 15.602 | 69.203 | 0.65x |
| users.json | msgspec | 9.412 | 11.782 | 14.241 | 69.203 | 0.65x |
| users.json | ujson | 12.690 | 15.934 | 21.510 | 69.203 | 0.48x |
| users.json | pysimdjson | 131.010 | 150.817 | 173.581 | 69.203 | 0.05x |
| users.json | json | 15.405 | 19.106 | 23.544 | 69.203 | 0.40x |
| flat.json | strata | 0.606 | 0.671 | 0.938 | 99.219 | 1.00x |
| flat.json | orjson | 0.754 | 0.882 | 1.158 | 99.219 | 0.76x |
| flat.json | msgspec | 0.744 | 0.802 | 0.976 | 99.219 | 0.84x |
| flat.json | ujson | 1.173 | 1.338 | 1.654 | 99.219 | 0.50x |
| flat.json | pysimdjson | 11.953 | 13.249 | 15.666 | 99.219 | 0.05x |
| flat.json | json | 1.384 | 1.537 | 1.904 | 99.219 | 0.44x |
| nested.json | strata | 0.520 | 0.582 | 0.654 | 99.219 | 1.00x |
| nested.json | orjson | 0.733 | 0.813 | 0.891 | 99.219 | 0.72x |
| nested.json | msgspec | 0.686 | 0.770 | 0.848 | 99.219 | 0.76x |
| nested.json | ujson | 0.958 | 1.219 | 1.718 | 99.219 | 0.48x |
| nested.json | pysimdjson | 10.602 | 11.442 | 13.989 | 99.219 | 0.05x |
| nested.json | json | 1.437 | 1.600 | 1.865 | 99.219 | 0.36x |
| wide_arrays.json | strata | 2.881 | 3.419 | 4.230 | 101.609 | 1.00x |
| wide_arrays.json | orjson | 3.460 | 4.130 | 5.140 | 101.609 | 0.83x |
| wide_arrays.json | msgspec | 3.862 | 4.579 | 5.540 | 101.609 | 0.75x |
| wide_arrays.json | ujson | 5.129 | 5.901 | 7.323 | 101.609 | 0.58x |
| wide_arrays.json | pysimdjson | 62.100 | 67.238 | 77.979 | 101.609 | 0.05x |
| wide_arrays.json | json | 6.535 | 7.450 | 9.246 | 101.609 | 0.46x |
| mixed.json | strata | 0.119 | 0.150 | 0.243 | 101.625 | 1.00x |
| mixed.json | orjson | 0.152 | 0.189 | 0.341 | 101.625 | 0.79x |
| mixed.json | msgspec | 0.164 | 0.203 | 0.270 | 101.625 | 0.74x |
| mixed.json | ujson | 0.201 | 0.284 | 0.621 | 101.625 | 0.53x |
| mixed.json | pysimdjson | 2.437 | 2.832 | 3.488 | 101.625 | 0.05x |
| mixed.json | json | 0.317 | 0.382 | 0.614 | 101.625 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.579 | 1.939 | 2.239 | 83.516 | 1.00x |
| users.json | orjson | 2.276 | 2.907 | 3.645 | 83.516 | 0.67x |
| users.json | msgspec | 3.046 | 3.574 | 4.352 | 83.516 | 0.54x |
| users.json | ujson | 9.046 | 10.181 | 12.684 | 83.516 | 0.19x |
| users.json | json | 16.168 | 18.241 | 20.961 | 83.516 | 0.11x |
| flat.json | strata | 0.214 | 0.256 | 0.291 | 99.219 | 1.00x |
| flat.json | orjson | 0.259 | 0.308 | 0.366 | 99.219 | 0.83x |
| flat.json | msgspec | 0.329 | 0.372 | 0.478 | 99.219 | 0.69x |
| flat.json | ujson | 0.780 | 0.914 | 1.119 | 99.219 | 0.28x |
| flat.json | json | 1.396 | 1.658 | 2.145 | 99.219 | 0.15x |
| nested.json | strata | 0.141 | 0.162 | 0.306 | 99.219 | 1.00x |
| nested.json | orjson | 0.234 | 0.268 | 0.464 | 99.219 | 0.60x |
| nested.json | msgspec | 0.310 | 0.473 | 0.677 | 99.219 | 0.34x |
| nested.json | ujson | 0.846 | 1.041 | 1.294 | 99.219 | 0.16x |
| nested.json | json | 1.682 | 1.887 | 2.518 | 99.219 | 0.09x |
| wide_arrays.json | strata | 1.061 | 1.237 | 1.667 | 101.609 | 1.00x |
| wide_arrays.json | orjson | 1.308 | 1.550 | 2.071 | 101.609 | 0.80x |
| wide_arrays.json | msgspec | 2.120 | 2.430 | 3.149 | 101.609 | 0.51x |
| wide_arrays.json | ujson | 4.679 | 5.285 | 6.731 | 101.609 | 0.23x |
| wide_arrays.json | json | 11.346 | 12.664 | 15.744 | 101.609 | 0.10x |
| mixed.json | strata | 0.035 | 0.048 | 0.074 | 101.625 | 1.00x |
| mixed.json | orjson | 0.044 | 0.062 | 0.186 | 101.625 | 0.78x |
| mixed.json | msgspec | 0.052 | 0.078 | 0.207 | 101.625 | 0.61x |
| mixed.json | ujson | 0.167 | 0.192 | 0.226 | 101.625 | 0.25x |
| mixed.json | json | 0.342 | 0.385 | 0.504 | 101.625 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.401 | 7.423 | 8.947 | 93.562 | 1.00x |
| users.json | orjson | 9.284 | 11.500 | 15.068 | 93.562 | 0.65x |
| users.json | msgspec | 9.184 | 11.239 | 14.465 | 93.562 | 0.66x |
| users.json | ujson | 12.432 | 15.171 | 20.363 | 93.562 | 0.49x |
| users.json | json | 14.635 | 18.044 | 22.270 | 93.562 | 0.41x |
| flat.json | strata | 0.634 | 0.775 | 0.977 | 99.219 | 1.00x |
| flat.json | orjson | 0.901 | 1.065 | 1.337 | 99.219 | 0.73x |
| flat.json | msgspec | 0.793 | 0.944 | 1.179 | 99.219 | 0.82x |
| flat.json | ujson | 1.164 | 1.369 | 1.830 | 99.219 | 0.57x |
| flat.json | json | 1.434 | 1.666 | 2.025 | 99.219 | 0.47x |
| nested.json | strata | 0.599 | 0.717 | 1.097 | 99.219 | 1.00x |
| nested.json | orjson | 0.974 | 1.193 | 1.580 | 99.219 | 0.60x |
| nested.json | msgspec | 0.820 | 0.988 | 1.449 | 99.219 | 0.73x |
| nested.json | ujson | 1.127 | 1.372 | 1.872 | 99.219 | 0.52x |
| nested.json | json | 1.584 | 1.801 | 2.894 | 99.219 | 0.40x |
| wide_arrays.json | strata | 3.110 | 3.657 | 4.509 | 101.609 | 1.00x |
| wide_arrays.json | orjson | 3.782 | 4.553 | 5.487 | 101.609 | 0.80x |
| wide_arrays.json | msgspec | 4.273 | 5.068 | 6.374 | 101.609 | 0.72x |
| wide_arrays.json | ujson | 5.638 | 6.591 | 8.193 | 101.609 | 0.55x |
| wide_arrays.json | json | 6.779 | 8.180 | 9.736 | 101.609 | 0.45x |
| mixed.json | strata | 0.142 | 0.216 | 0.317 | 101.625 | 1.00x |
| mixed.json | orjson | 0.205 | 0.396 | 0.619 | 101.625 | 0.54x |
| mixed.json | msgspec | 0.208 | 0.275 | 0.452 | 101.625 | 0.78x |
| mixed.json | ujson | 0.252 | 0.338 | 0.534 | 101.625 | 0.64x |
| mixed.json | json | 0.348 | 0.478 | 0.613 | 101.625 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.694 | 7.849 | 10.828 | 99.203 | 1.00x |
| users.ndjson | orjson | 11.235 | 13.430 | 18.262 | 99.203 | 0.58x |
| users.ndjson | msgspec | 11.328 | 13.220 | 17.172 | 99.203 | 0.59x |
| users.ndjson | ujson | 14.229 | 16.590 | 23.886 | 99.203 | 0.47x |
| users.ndjson | json | 18.004 | 21.855 | 28.208 | 99.203 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.846 | 2.149 | 2.472 | 93.578 | 1.00x |
| users.json | orjson | 2.666 | 3.169 | 4.584 | 93.578 | 0.68x |
| users.json | msgspec | 3.358 | 3.800 | 5.226 | 93.578 | 0.57x |
| users.json | ujson | 9.384 | 10.487 | 12.752 | 93.578 | 0.20x |
| users.json | json | 16.148 | 18.050 | 23.452 | 93.578 | 0.12x |
| flat.json | strata | 0.353 | 0.456 | 0.855 | 99.219 | 1.00x |
| flat.json | orjson | 0.391 | 0.505 | 0.970 | 99.219 | 0.90x |
| flat.json | msgspec | 0.455 | 0.544 | 1.286 | 99.219 | 0.84x |
| flat.json | ujson | 0.931 | 1.047 | 2.067 | 99.219 | 0.44x |
| flat.json | json | 1.654 | 1.784 | 6.846 | 99.219 | 0.26x |
| nested.json | strata | 0.284 | 0.370 | 0.436 | 99.219 | 1.00x |
| nested.json | orjson | 0.378 | 0.499 | 0.632 | 99.219 | 0.74x |
| nested.json | msgspec | 0.531 | 0.692 | 0.849 | 99.219 | 0.53x |
| nested.json | ujson | 1.015 | 1.262 | 1.447 | 99.219 | 0.29x |
| nested.json | json | 1.850 | 2.019 | 2.222 | 99.219 | 0.18x |
| wide_arrays.json | strata | 1.327 | 1.653 | 2.288 | 101.609 | 1.00x |
| wide_arrays.json | orjson | 1.562 | 2.111 | 3.064 | 101.609 | 0.78x |
| wide_arrays.json | msgspec | 2.457 | 3.009 | 4.038 | 101.609 | 0.55x |
| wide_arrays.json | ujson | 5.051 | 6.006 | 7.882 | 101.609 | 0.28x |
| wide_arrays.json | json | 11.850 | 13.519 | 17.037 | 101.609 | 0.12x |
| mixed.json | strata | 0.139 | 0.255 | 0.432 | 101.625 | 1.00x |
| mixed.json | orjson | 0.154 | 0.308 | 0.456 | 101.625 | 0.83x |
| mixed.json | msgspec | 0.166 | 0.331 | 0.510 | 101.625 | 0.77x |
| mixed.json | ujson | 0.296 | 0.459 | 0.654 | 101.625 | 0.55x |
| mixed.json | json | 0.467 | 0.663 | 0.887 | 101.625 | 0.38x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.069 | 0.140 | 0.374 | 93.625 | 1.00x |
| users.json $[*].id | jmespath | 0.337 | 0.423 | 1.012 | 93.625 | 0.33x |
| users.json $[*].id | jsonpath-ng | 1.602 | 2.087 | 4.768 | 93.625 | 0.07x |
| users.json $[*].orders[*].total | strata | 0.555 | 0.829 | 1.737 | 93.781 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.044 | 2.600 | 3.624 | 93.781 | 0.32x |
| users.json $[*].orders[*].total | jsonpath-ng | 12.618 | 15.791 | 20.014 | 93.781 | 0.05x |
| users.json $..total | strata | 1.346 | 1.751 | 2.740 | 93.781 | 1.00x |
| users.json $..total | jsonpath-ng | 193.682 | 282.243 | 368.711 | 93.781 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.774 | 4.270 | 5.996 | 93.688 | 1.00x |
| users.json $[*].id | orjson+jmespath | 11.207 | 14.014 | 19.662 | 93.688 | 0.30x |
| users.json $[*].id | orjson+jsonpath-ng | 13.303 | 16.162 | 23.234 | 93.688 | 0.26x |
| users.json $[*].orders[*].total | strata | 4.040 | 5.012 | 7.286 | 93.781 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 14.526 | 17.684 | 23.508 | 93.781 | 0.28x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 28.492 | 38.826 | 50.247 | 93.781 | 0.13x |
| users.json $..total | strata | 8.008 | 9.232 | 11.308 | 93.781 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 206.082 | 229.517 | 252.523 | 93.781 | 0.04x |

