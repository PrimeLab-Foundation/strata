# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 38eaa9f248f509d9a4beb75b267189d011c1109e
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: AMD EPYC 9V74 80-Core Processor
- compiler_flags: -fno-strict-overflow -Wsign-compare -DNDEBUG -g -O3 -Wall -fPIC -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -fomit-frame-pointer -march=native -flto=thin -fprofile-use=/home/runner/work/_temp/strata-arm/build/pgo/strata.profdata -fsplit-machine-functions -shared -Wl,--rpath=/opt/hostedtoolcache/Python/3.12.14/x64/lib (19 recorded commands; see the JSON companion)
- repeats: 60
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.620 | 7.784 | 7.918 | 65.715 | 1.00x |
| users.json | orjson | 10.695 | 10.896 | 11.441 | 65.715 | 0.71x |
| users.json | msgspec | 10.461 | 10.876 | 11.112 | 65.715 | 0.72x |
| users.json | ujson | 13.741 | 14.508 | 14.803 | 65.715 | 0.54x |
| users.json | pysimdjson | 14.478 | 15.192 | 15.392 | 65.715 | 0.51x |
| users.json | json | 15.984 | 16.537 | 16.801 | 65.715 | 0.47x |
| flat.json | strata | 0.695 | 0.710 | 0.730 | 79.332 | 1.00x |
| flat.json | orjson | 0.826 | 0.843 | 0.854 | 79.332 | 0.84x |
| flat.json | msgspec | 0.765 | 0.796 | 0.810 | 79.332 | 0.89x |
| flat.json | ujson | 1.195 | 1.215 | 1.238 | 79.332 | 0.58x |
| flat.json | pysimdjson | 1.237 | 1.276 | 1.303 | 79.332 | 0.56x |
| flat.json | json | 1.336 | 1.359 | 1.393 | 79.332 | 0.52x |
| nested.json | strata | 0.617 | 0.634 | 0.647 | 79.332 | 1.00x |
| nested.json | orjson | 0.773 | 0.790 | 0.806 | 79.332 | 0.80x |
| nested.json | msgspec | 0.730 | 0.756 | 0.775 | 79.332 | 0.84x |
| nested.json | ujson | 1.085 | 1.121 | 1.143 | 79.332 | 0.57x |
| nested.json | pysimdjson | 1.090 | 1.116 | 1.136 | 79.332 | 0.57x |
| nested.json | json | 1.412 | 1.434 | 1.461 | 79.332 | 0.44x |
| wide_arrays.json | strata | 3.385 | 3.461 | 3.514 | 83.336 | 1.00x |
| wide_arrays.json | orjson | 4.293 | 4.365 | 4.426 | 83.336 | 0.79x |
| wide_arrays.json | msgspec | 4.705 | 4.770 | 4.860 | 83.336 | 0.73x |
| wide_arrays.json | ujson | 5.849 | 5.947 | 6.042 | 83.336 | 0.58x |
| wide_arrays.json | pysimdjson | 4.923 | 5.024 | 5.095 | 83.336 | 0.69x |
| wide_arrays.json | json | 7.612 | 7.731 | 7.832 | 83.336 | 0.45x |
| mixed.json | strata | 0.147 | 0.152 | 0.164 | 83.336 | 1.00x |
| mixed.json | orjson | 0.180 | 0.186 | 0.199 | 83.336 | 0.81x |
| mixed.json | msgspec | 0.183 | 0.191 | 0.204 | 83.336 | 0.79x |
| mixed.json | ujson | 0.237 | 0.246 | 0.262 | 83.336 | 0.62x |
| mixed.json | pysimdjson | 0.228 | 0.236 | 0.247 | 83.336 | 0.64x |
| mixed.json | json | 0.342 | 0.352 | 0.365 | 83.336 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.882 | 1.902 | 1.921 | 64.805 | 1.00x |
| users.json | orjson | 2.041 | 2.070 | 2.087 | 64.805 | 0.92x |
| users.json | msgspec | 3.301 | 3.329 | 3.360 | 64.805 | 0.57x |
| users.json | ujson | 8.899 | 9.057 | 9.187 | 64.805 | 0.21x |
| users.json | json | 17.119 | 17.361 | 17.723 | 64.805 | 0.11x |
| flat.json | strata | 0.237 | 0.247 | 0.261 | 79.332 | 1.00x |
| flat.json | orjson | 0.243 | 0.250 | 0.261 | 79.332 | 0.99x |
| flat.json | msgspec | 0.370 | 0.381 | 0.389 | 79.332 | 0.65x |
| flat.json | ujson | 0.786 | 0.803 | 0.815 | 79.332 | 0.31x |
| flat.json | json | 1.442 | 1.461 | 1.483 | 79.332 | 0.17x |
| nested.json | strata | 0.172 | 0.176 | 0.188 | 79.332 | 1.00x |
| nested.json | orjson | 0.218 | 0.222 | 0.232 | 79.332 | 0.79x |
| nested.json | msgspec | 0.322 | 0.329 | 0.340 | 79.332 | 0.53x |
| nested.json | ujson | 0.819 | 0.840 | 0.857 | 79.332 | 0.21x |
| nested.json | json | 1.820 | 1.852 | 1.885 | 79.332 | 0.10x |
| wide_arrays.json | strata | 1.357 | 1.379 | 1.397 | 83.336 | 1.00x |
| wide_arrays.json | orjson | 1.463 | 1.484 | 1.498 | 83.336 | 0.93x |
| wide_arrays.json | msgspec | 2.368 | 2.399 | 2.424 | 83.336 | 0.57x |
| wide_arrays.json | ujson | 4.957 | 5.001 | 5.045 | 83.336 | 0.28x |
| wide_arrays.json | json | 12.966 | 13.056 | 13.151 | 83.336 | 0.11x |
| mixed.json | strata | 0.048 | 0.049 | 0.052 | 83.336 | 1.00x |
| mixed.json | orjson | 0.046 | 0.048 | 0.050 | 83.336 | 1.03x |
| mixed.json | msgspec | 0.066 | 0.068 | 0.075 | 83.336 | 0.73x |
| mixed.json | ujson | 0.177 | 0.187 | 0.194 | 83.336 | 0.26x |
| mixed.json | json | 0.398 | 0.405 | 0.416 | 83.336 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.083 | 8.534 | 8.759 | 78.000 | 1.00x |
| users.json | orjson | 11.015 | 11.901 | 12.072 | 78.000 | 0.72x |
| users.json | msgspec | 10.841 | 11.738 | 11.954 | 78.000 | 0.73x |
| users.json | ujson | 15.117 | 16.553 | 17.122 | 78.000 | 0.52x |
| users.json | json | 16.615 | 17.531 | 17.779 | 78.000 | 0.49x |
| flat.json | strata | 0.717 | 0.736 | 0.756 | 79.332 | 1.00x |
| flat.json | orjson | 0.871 | 0.893 | 0.908 | 79.332 | 0.82x |
| flat.json | msgspec | 0.815 | 0.837 | 0.852 | 79.332 | 0.88x |
| flat.json | ujson | 1.254 | 1.281 | 1.309 | 79.332 | 0.57x |
| flat.json | json | 1.365 | 1.391 | 1.429 | 79.332 | 0.53x |
| nested.json | strata | 0.643 | 0.658 | 0.671 | 79.332 | 1.00x |
| nested.json | orjson | 0.818 | 0.836 | 0.849 | 79.332 | 0.79x |
| nested.json | msgspec | 0.768 | 0.790 | 0.804 | 79.332 | 0.83x |
| nested.json | ujson | 1.137 | 1.157 | 1.173 | 79.332 | 0.57x |
| nested.json | json | 1.451 | 1.469 | 1.510 | 79.332 | 0.45x |
| wide_arrays.json | strata | 3.444 | 3.523 | 3.576 | 83.336 | 1.00x |
| wide_arrays.json | orjson | 4.383 | 4.475 | 4.551 | 83.336 | 0.79x |
| wide_arrays.json | msgspec | 4.803 | 4.922 | 5.031 | 83.336 | 0.72x |
| wide_arrays.json | ujson | 6.038 | 6.142 | 6.205 | 83.336 | 0.57x |
| wide_arrays.json | json | 7.621 | 7.717 | 7.831 | 83.336 | 0.46x |
| mixed.json | strata | 0.159 | 0.165 | 0.177 | 83.336 | 1.00x |
| mixed.json | orjson | 0.220 | 0.226 | 0.240 | 83.336 | 0.73x |
| mixed.json | msgspec | 0.219 | 0.227 | 0.237 | 83.336 | 0.73x |
| mixed.json | ujson | 0.280 | 0.291 | 0.310 | 83.336 | 0.57x |
| mixed.json | json | 0.379 | 0.386 | 0.399 | 83.336 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 8.249 | 8.370 | 8.451 | 79.332 | 1.00x |
| users.ndjson | orjson | 13.842 | 14.055 | 14.189 | 79.332 | 0.60x |
| users.ndjson | msgspec | 13.882 | 14.035 | 14.175 | 79.332 | 0.60x |
| users.ndjson | ujson | 18.013 | 18.569 | 18.828 | 79.332 | 0.45x |
| users.ndjson | json | 22.479 | 22.703 | 22.879 | 79.332 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.483 | 2.519 | 2.559 | 78.000 | 1.00x |
| users.json | orjson | 2.682 | 2.713 | 2.760 | 78.000 | 0.93x |
| users.json | msgspec | 3.934 | 3.967 | 4.010 | 78.000 | 0.63x |
| users.json | ujson | 9.683 | 9.821 | 9.932 | 78.000 | 0.26x |
| users.json | json | 17.756 | 18.207 | 18.585 | 78.000 | 0.14x |
| flat.json | strata | 0.468 | 0.492 | 0.516 | 79.332 | 1.00x |
| flat.json | orjson | 0.486 | 0.511 | 0.535 | 79.332 | 0.96x |
| flat.json | msgspec | 0.617 | 0.643 | 0.665 | 79.332 | 0.77x |
| flat.json | ujson | 1.054 | 1.080 | 1.105 | 79.332 | 0.46x |
| flat.json | json | 1.708 | 1.749 | 1.779 | 79.332 | 0.28x |
| nested.json | strata | 0.384 | 0.406 | 0.429 | 79.332 | 1.00x |
| nested.json | orjson | 0.446 | 0.474 | 0.495 | 79.332 | 0.86x |
| nested.json | msgspec | 0.546 | 0.582 | 0.604 | 79.332 | 0.70x |
| nested.json | ujson | 1.072 | 1.096 | 1.135 | 79.332 | 0.37x |
| nested.json | json | 2.070 | 2.118 | 2.166 | 79.332 | 0.19x |
| wide_arrays.json | strata | 1.820 | 1.844 | 1.880 | 83.336 | 1.00x |
| wide_arrays.json | orjson | 1.931 | 1.955 | 1.997 | 83.336 | 0.94x |
| wide_arrays.json | msgspec | 2.845 | 2.878 | 2.918 | 83.336 | 0.64x |
| wide_arrays.json | ujson | 5.480 | 5.533 | 5.584 | 83.336 | 0.33x |
| wide_arrays.json | json | 13.503 | 13.631 | 13.783 | 83.336 | 0.14x |
| mixed.json | strata | 0.230 | 0.244 | 0.276 | 83.336 | 1.00x |
| mixed.json | orjson | 0.244 | 0.261 | 0.288 | 83.336 | 0.94x |
| mixed.json | msgspec | 0.258 | 0.282 | 0.311 | 83.336 | 0.87x |
| mixed.json | ujson | 0.383 | 0.405 | 0.427 | 83.336 | 0.60x |
| mixed.json | json | 0.610 | 0.638 | 0.672 | 83.336 | 0.38x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.054 | 0.056 | 0.059 | 78.000 | 1.00x |
| users.json $[*].id | jmespath | 0.368 | 0.376 | 0.386 | 78.000 | 0.15x |
| users.json $[*].id | jsonpath-ng | 2.105 | 2.150 | 2.190 | 78.000 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.327 | 0.346 | 0.358 | 78.023 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.319 | 2.361 | 2.402 | 78.023 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 14.856 | 15.429 | 15.605 | 78.023 | 0.02x |
| users.json $..total | strata | 1.398 | 1.449 | 1.507 | 80.969 | 1.00x |
| users.json $..total | jsonpath-ng | 297.436 | 299.946 | 301.647 | 80.969 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 2.562 | 2.580 | 2.599 | 78.023 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.303 | 12.558 | 12.883 | 78.023 | 0.21x |
| users.json $[*].id | orjson+jsonpath-ng | 14.009 | 14.388 | 14.646 | 78.023 | 0.18x |
| users.json $[*].orders[*].total | strata | 2.720 | 2.742 | 2.770 | 80.969 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 14.618 | 15.216 | 15.392 | 80.969 | 0.18x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 30.428 | 31.745 | 32.138 | 80.969 | 0.09x |
| users.json $..total | strata | 10.433 | 12.050 | 12.299 | 80.969 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 321.231 | 323.917 | 325.753 | 80.969 | 0.04x |

