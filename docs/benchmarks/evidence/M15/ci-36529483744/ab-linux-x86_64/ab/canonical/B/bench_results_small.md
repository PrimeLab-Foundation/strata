# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: afd1550cbabc9433e8292644444a23bb27bbc4e9
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: AMD EPYC 9V45 96-Core Processor
- compiler_flags: -fno-strict-overflow -Wsign-compare -DNDEBUG -g -O3 -Wall -fPIC -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -fomit-frame-pointer -march=native -flto=thin -fprofile-use=/home/runner/work/_temp/strata-arm/build/pgo/strata.profdata -fsplit-machine-functions -shared -Wl,--rpath=/opt/hostedtoolcache/Python/3.12.14/x64/lib (24 recorded commands; see the JSON companion)
- repeats: 60
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.209 | 8.101 | 9.346 | 66.203 | 1.00x |
| users.json | orjson | 9.516 | 11.977 | 12.951 | 66.203 | 0.68x |
| users.json | msgspec | 9.054 | 11.078 | 12.317 | 66.203 | 0.73x |
| users.json | ujson | 13.456 | 17.051 | 18.653 | 66.203 | 0.48x |
| users.json | pysimdjson | 14.773 | 17.984 | 19.521 | 66.203 | 0.45x |
| users.json | json | 16.031 | 18.028 | 20.207 | 66.203 | 0.45x |
| flat.json | strata | 0.557 | 0.606 | 0.702 | 63.980 | 1.00x |
| flat.json | orjson | 0.684 | 0.733 | 0.790 | 63.980 | 0.83x |
| flat.json | msgspec | 0.632 | 0.675 | 0.748 | 63.980 | 0.90x |
| flat.json | ujson | 0.971 | 1.149 | 1.303 | 63.980 | 0.53x |
| flat.json | pysimdjson | 1.022 | 1.135 | 1.309 | 63.980 | 0.53x |
| flat.json | json | 1.246 | 1.294 | 1.381 | 63.980 | 0.47x |
| nested.json | strata | 0.440 | 0.472 | 0.504 | 63.980 | 1.00x |
| nested.json | orjson | 0.554 | 0.583 | 0.614 | 63.980 | 0.81x |
| nested.json | msgspec | 0.540 | 0.562 | 0.607 | 63.980 | 0.84x |
| nested.json | ujson | 0.806 | 0.925 | 1.025 | 63.980 | 0.51x |
| nested.json | pysimdjson | 0.784 | 0.834 | 0.893 | 63.980 | 0.57x |
| nested.json | json | 1.318 | 1.361 | 1.407 | 63.980 | 0.35x |
| wide_arrays.json | strata | 2.494 | 2.969 | 4.726 | 79.285 | 1.00x |
| wide_arrays.json | orjson | 3.549 | 4.348 | 6.362 | 79.285 | 0.68x |
| wide_arrays.json | msgspec | 3.730 | 4.388 | 6.024 | 79.285 | 0.68x |
| wide_arrays.json | ujson | 4.530 | 5.283 | 7.043 | 79.285 | 0.56x |
| wide_arrays.json | pysimdjson | 4.074 | 4.999 | 6.898 | 79.285 | 0.59x |
| wide_arrays.json | json | 9.120 | 10.416 | 12.649 | 79.285 | 0.29x |
| mixed.json | strata | 0.107 | 0.114 | 0.128 | 79.285 | 1.00x |
| mixed.json | orjson | 0.136 | 0.143 | 0.158 | 79.285 | 0.80x |
| mixed.json | msgspec | 0.135 | 0.145 | 0.159 | 79.285 | 0.79x |
| mixed.json | ujson | 0.178 | 0.196 | 0.224 | 79.285 | 0.58x |
| mixed.json | pysimdjson | 0.180 | 0.193 | 0.212 | 79.285 | 0.59x |
| mixed.json | json | 0.286 | 0.300 | 0.323 | 79.285 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.376 | 1.550 | 1.771 | 47.438 | 1.00x |
| users.json | orjson | 1.346 | 1.450 | 1.585 | 47.438 | 1.07x |
| users.json | msgspec | 2.460 | 2.595 | 2.775 | 47.438 | 0.60x |
| users.json | ujson | 6.559 | 6.856 | 7.393 | 47.438 | 0.23x |
| users.json | json | 11.658 | 12.287 | 13.460 | 47.438 | 0.13x |
| flat.json | strata | 0.193 | 0.210 | 0.239 | 63.980 | 1.00x |
| flat.json | orjson | 0.182 | 0.191 | 0.210 | 63.980 | 1.10x |
| flat.json | msgspec | 0.286 | 0.297 | 0.323 | 63.980 | 0.71x |
| flat.json | ujson | 0.595 | 0.612 | 0.642 | 63.980 | 0.34x |
| flat.json | json | 1.033 | 1.048 | 1.099 | 63.980 | 0.20x |
| nested.json | strata | 0.120 | 0.128 | 0.139 | 63.980 | 1.00x |
| nested.json | orjson | 0.138 | 0.144 | 0.159 | 63.980 | 0.89x |
| nested.json | msgspec | 0.245 | 0.254 | 0.263 | 63.980 | 0.50x |
| nested.json | ujson | 0.613 | 0.634 | 0.652 | 63.980 | 0.20x |
| nested.json | json | 1.240 | 1.288 | 1.317 | 63.980 | 0.10x |
| wide_arrays.json | strata | 1.106 | 1.261 | 1.654 | 79.285 | 1.00x |
| wide_arrays.json | orjson | 1.047 | 1.121 | 1.469 | 79.285 | 1.12x |
| wide_arrays.json | msgspec | 1.861 | 1.948 | 2.400 | 79.285 | 0.65x |
| wide_arrays.json | ujson | 3.597 | 3.883 | 4.847 | 79.285 | 0.32x |
| wide_arrays.json | json | 9.138 | 9.997 | 11.371 | 79.285 | 0.13x |
| mixed.json | strata | 0.039 | 0.044 | 0.049 | 79.285 | 1.00x |
| mixed.json | orjson | 0.033 | 0.036 | 0.043 | 79.285 | 1.23x |
| mixed.json | msgspec | 0.049 | 0.054 | 0.062 | 79.285 | 0.80x |
| mixed.json | ujson | 0.136 | 0.146 | 0.154 | 79.285 | 0.30x |
| mixed.json | json | 0.291 | 0.312 | 0.327 | 79.285 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.278 | 9.002 | 10.228 | 65.562 | 1.00x |
| users.json | orjson | 9.633 | 12.090 | 13.513 | 65.562 | 0.74x |
| users.json | msgspec | 9.217 | 11.363 | 13.854 | 65.562 | 0.79x |
| users.json | ujson | 13.332 | 17.254 | 21.154 | 65.562 | 0.52x |
| users.json | json | 16.116 | 18.134 | 20.460 | 65.562 | 0.50x |
| flat.json | strata | 0.550 | 0.609 | 0.674 | 63.980 | 1.00x |
| flat.json | orjson | 0.692 | 0.736 | 0.808 | 63.980 | 0.83x |
| flat.json | msgspec | 0.640 | 0.694 | 0.795 | 63.980 | 0.88x |
| flat.json | ujson | 0.950 | 1.228 | 1.449 | 63.980 | 0.50x |
| flat.json | json | 1.249 | 1.290 | 1.326 | 63.980 | 0.47x |
| nested.json | strata | 0.482 | 0.531 | 0.632 | 63.980 | 1.00x |
| nested.json | orjson | 0.621 | 0.671 | 0.764 | 63.980 | 0.79x |
| nested.json | msgspec | 0.582 | 0.635 | 0.723 | 63.980 | 0.84x |
| nested.json | ujson | 0.847 | 0.934 | 1.017 | 63.980 | 0.57x |
| nested.json | json | 1.403 | 1.469 | 1.573 | 63.980 | 0.36x |
| wide_arrays.json | strata | 3.017 | 3.392 | 4.138 | 79.285 | 1.00x |
| wide_arrays.json | orjson | 4.000 | 4.617 | 5.435 | 79.285 | 0.73x |
| wide_arrays.json | msgspec | 4.326 | 4.762 | 5.653 | 79.285 | 0.71x |
| wide_arrays.json | ujson | 5.260 | 5.656 | 6.712 | 79.285 | 0.60x |
| wide_arrays.json | json | 9.753 | 10.334 | 11.380 | 79.285 | 0.33x |
| mixed.json | strata | 0.120 | 0.143 | 0.168 | 79.285 | 1.00x |
| mixed.json | orjson | 0.167 | 0.195 | 0.238 | 79.285 | 0.73x |
| mixed.json | msgspec | 0.163 | 0.192 | 0.240 | 79.285 | 0.74x |
| mixed.json | ujson | 0.213 | 0.263 | 0.308 | 79.285 | 0.54x |
| mixed.json | json | 0.308 | 0.344 | 0.382 | 79.285 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 8.530 | 10.469 | 11.875 | 63.980 | 1.00x |
| users.ndjson | orjson | 12.340 | 14.269 | 16.347 | 63.980 | 0.73x |
| users.ndjson | msgspec | 12.165 | 14.030 | 16.433 | 63.980 | 0.75x |
| users.ndjson | ujson | 16.194 | 19.575 | 21.737 | 63.980 | 0.53x |
| users.ndjson | json | 22.262 | 24.726 | 26.771 | 63.980 | 0.42x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.087 | 2.280 | 2.631 | 65.562 | 1.00x |
| users.json | orjson | 2.068 | 2.305 | 5.025 | 65.562 | 0.99x |
| users.json | msgspec | 3.078 | 3.437 | 4.798 | 65.562 | 0.66x |
| users.json | ujson | 7.226 | 7.813 | 8.431 | 65.562 | 0.29x |
| users.json | json | 12.218 | 13.155 | 13.714 | 65.562 | 0.17x |
| flat.json | strata | 0.411 | 0.491 | 0.600 | 63.980 | 1.00x |
| flat.json | orjson | 0.396 | 0.459 | 0.570 | 63.980 | 1.07x |
| flat.json | msgspec | 0.496 | 0.567 | 0.708 | 63.980 | 0.87x |
| flat.json | ujson | 0.826 | 0.897 | 1.022 | 63.980 | 0.55x |
| flat.json | json | 1.248 | 1.341 | 1.485 | 63.980 | 0.37x |
| nested.json | strata | 0.314 | 0.350 | 0.404 | 63.980 | 1.00x |
| nested.json | orjson | 0.352 | 0.376 | 0.429 | 63.980 | 0.93x |
| nested.json | msgspec | 0.460 | 0.486 | 0.557 | 63.980 | 0.72x |
| nested.json | ujson | 0.827 | 0.859 | 0.903 | 63.980 | 0.41x |
| nested.json | json | 1.480 | 1.528 | 1.605 | 63.980 | 0.23x |
| wide_arrays.json | strata | 1.653 | 1.838 | 2.352 | 79.285 | 1.00x |
| wide_arrays.json | orjson | 1.612 | 1.755 | 2.000 | 79.285 | 1.05x |
| wide_arrays.json | msgspec | 2.413 | 2.561 | 2.884 | 79.285 | 0.72x |
| wide_arrays.json | ujson | 4.216 | 4.463 | 5.059 | 79.285 | 0.41x |
| wide_arrays.json | json | 9.841 | 10.470 | 11.553 | 79.285 | 0.18x |
| mixed.json | strata | 0.197 | 0.215 | 0.247 | 79.285 | 1.00x |
| mixed.json | orjson | 0.204 | 0.217 | 0.265 | 79.285 | 0.99x |
| mixed.json | msgspec | 0.209 | 0.235 | 0.279 | 79.285 | 0.91x |
| mixed.json | ujson | 0.299 | 0.328 | 0.445 | 79.285 | 0.65x |
| mixed.json | json | 0.458 | 0.497 | 0.549 | 79.285 | 0.43x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.046 | 0.050 | 0.059 | 65.562 | 1.00x |
| users.json $[*].id | jmespath | 0.270 | 0.283 | 0.295 | 65.562 | 0.18x |
| users.json $[*].id | jsonpath-ng | 1.731 | 1.889 | 1.987 | 65.562 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.259 | 0.280 | 0.336 | 65.574 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.650 | 1.742 | 1.839 | 65.574 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 12.535 | 13.954 | 15.871 | 65.574 | 0.02x |
| users.json $..total | strata | 1.091 | 1.606 | 2.075 | 65.609 | 1.00x |
| users.json $..total | jsonpath-ng | 221.154 | 229.142 | 236.700 | 65.609 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 2.110 | 2.183 | 2.229 | 65.574 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.911 | 12.224 | 14.317 | 65.574 | 0.18x |
| users.json $[*].id | orjson+jsonpath-ng | 12.422 | 14.034 | 17.072 | 65.574 | 0.16x |
| users.json $[*].orders[*].total | strata | 2.253 | 2.345 | 2.423 | 65.609 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 13.074 | 14.902 | 16.392 | 65.609 | 0.16x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 26.245 | 29.912 | 33.104 | 65.609 | 0.08x |
| users.json $..total | strata | 15.068 | 18.872 | 21.298 | 65.617 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 253.694 | 264.278 | 273.309 | 65.617 | 0.07x |

