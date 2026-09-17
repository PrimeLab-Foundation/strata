# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 38cf28416465e176915ffd24d827a95015eacdbd
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: AMD EPYC 7763 64-Core Processor
- compiler_flags: -fno-strict-overflow -Wsign-compare -DNDEBUG -g -O3 -Wall -fPIC -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -fomit-frame-pointer -march=native -flto=thin -fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata -fsplit-machine-functions -shared -Wl,--rpath=/opt/hostedtoolcache/Python/3.12.14/x64/lib (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.627 | 9.752 | 14.489 | 63.508 | 1.00x |
| users.json | orjson | 13.125 | 13.241 | 17.240 | 63.508 | 0.74x |
| users.json | msgspec | 13.245 | 13.447 | 16.700 | 63.508 | 0.73x |
| users.json | ujson | 17.630 | 17.932 | 23.965 | 63.508 | 0.54x |
| users.json | pysimdjson | 18.121 | 18.402 | 22.020 | 63.508 | 0.53x |
| users.json | json | 22.325 | 22.450 | 24.553 | 63.508 | 0.43x |
| flat.json | strata | 0.845 | 0.854 | 0.861 | 77.988 | 1.00x |
| flat.json | orjson | 0.978 | 0.990 | 0.992 | 77.988 | 0.86x |
| flat.json | msgspec | 0.983 | 0.994 | 1.011 | 77.988 | 0.86x |
| flat.json | ujson | 1.432 | 1.447 | 1.497 | 77.988 | 0.59x |
| flat.json | pysimdjson | 1.513 | 1.534 | 1.565 | 77.988 | 0.56x |
| flat.json | json | 1.886 | 1.901 | 1.947 | 77.988 | 0.45x |
| nested.json | strata | 0.807 | 0.817 | 0.830 | 78.023 | 1.00x |
| nested.json | orjson | 0.998 | 1.002 | 1.014 | 78.023 | 0.82x |
| nested.json | msgspec | 1.016 | 1.022 | 1.073 | 78.023 | 0.80x |
| nested.json | ujson | 1.439 | 1.454 | 1.485 | 78.023 | 0.56x |
| nested.json | pysimdjson | 1.405 | 1.414 | 1.430 | 78.023 | 0.58x |
| nested.json | json | 2.037 | 2.048 | 2.092 | 78.023 | 0.40x |
| wide_arrays.json | strata | 4.084 | 4.126 | 4.195 | 82.281 | 1.00x |
| wide_arrays.json | orjson | 5.067 | 5.158 | 6.063 | 82.281 | 0.80x |
| wide_arrays.json | msgspec | 5.618 | 5.714 | 5.768 | 82.281 | 0.72x |
| wide_arrays.json | ujson | 7.048 | 7.119 | 7.204 | 82.281 | 0.58x |
| wide_arrays.json | pysimdjson | 6.021 | 6.093 | 6.542 | 82.281 | 0.68x |
| wide_arrays.json | json | 9.687 | 9.797 | 9.921 | 82.281 | 0.42x |
| mixed.json | strata | 0.188 | 0.191 | 0.203 | 82.281 | 1.00x |
| mixed.json | orjson | 0.228 | 0.233 | 0.244 | 82.281 | 0.82x |
| mixed.json | msgspec | 0.240 | 0.242 | 0.261 | 82.281 | 0.79x |
| mixed.json | ujson | 0.300 | 0.303 | 0.328 | 82.281 | 0.63x |
| mixed.json | pysimdjson | 0.311 | 0.314 | 0.327 | 82.281 | 0.61x |
| mixed.json | json | 0.469 | 0.484 | 0.663 | 82.281 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.267 | 2.276 | 2.313 | 62.062 | 1.00x |
| users.json | orjson | 2.896 | 2.911 | 2.980 | 62.062 | 0.78x |
| users.json | msgspec | 3.820 | 3.833 | 3.862 | 62.062 | 0.59x |
| users.json | ujson | 11.671 | 11.814 | 11.932 | 62.062 | 0.19x |
| users.json | json | 21.633 | 21.690 | 21.929 | 62.062 | 0.10x |
| flat.json | strata | 0.277 | 0.283 | 0.388 | 78.023 | 1.00x |
| flat.json | orjson | 0.325 | 0.345 | 0.558 | 78.023 | 0.82x |
| flat.json | msgspec | 0.425 | 0.434 | 0.654 | 78.023 | 0.65x |
| flat.json | ujson | 1.004 | 1.093 | 1.878 | 78.023 | 0.26x |
| flat.json | json | 1.825 | 1.907 | 2.251 | 78.023 | 0.15x |
| nested.json | strata | 0.225 | 0.227 | 0.239 | 78.023 | 1.00x |
| nested.json | orjson | 0.288 | 0.289 | 0.305 | 78.023 | 0.78x |
| nested.json | msgspec | 0.398 | 0.403 | 0.414 | 78.023 | 0.56x |
| nested.json | ujson | 1.110 | 1.119 | 1.142 | 78.023 | 0.20x |
| nested.json | json | 2.370 | 2.382 | 2.416 | 78.023 | 0.10x |
| wide_arrays.json | strata | 1.620 | 1.635 | 1.648 | 82.281 | 1.00x |
| wide_arrays.json | orjson | 1.797 | 1.809 | 1.824 | 82.281 | 0.90x |
| wide_arrays.json | msgspec | 2.738 | 2.755 | 2.824 | 82.281 | 0.59x |
| wide_arrays.json | ujson | 6.385 | 6.416 | 6.464 | 82.281 | 0.25x |
| wide_arrays.json | json | 16.640 | 16.673 | 16.860 | 82.281 | 0.10x |
| mixed.json | strata | 0.061 | 0.061 | 0.074 | 82.281 | 1.00x |
| mixed.json | orjson | 0.064 | 0.066 | 0.077 | 82.281 | 0.93x |
| mixed.json | msgspec | 0.084 | 0.085 | 0.100 | 82.281 | 0.72x |
| mixed.json | ujson | 0.230 | 0.238 | 0.317 | 82.281 | 0.26x |
| mixed.json | json | 0.504 | 0.517 | 0.530 | 82.281 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.345 | 10.593 | 12.291 | 79.621 | 1.00x |
| users.json | orjson | 13.539 | 13.937 | 14.644 | 79.621 | 0.76x |
| users.json | msgspec | 13.581 | 13.726 | 14.111 | 79.621 | 0.77x |
| users.json | ujson | 18.736 | 19.645 | 21.052 | 79.621 | 0.54x |
| users.json | json | 22.854 | 23.083 | 23.271 | 79.621 | 0.46x |
| flat.json | strata | 0.869 | 0.880 | 0.929 | 78.023 | 1.00x |
| flat.json | orjson | 1.041 | 1.048 | 1.070 | 78.023 | 0.84x |
| flat.json | msgspec | 1.052 | 1.066 | 1.083 | 78.023 | 0.83x |
| flat.json | ujson | 1.529 | 1.548 | 1.624 | 78.023 | 0.57x |
| flat.json | json | 1.925 | 1.943 | 1.967 | 78.023 | 0.45x |
| nested.json | strata | 0.840 | 0.849 | 0.861 | 78.023 | 1.00x |
| nested.json | orjson | 1.053 | 1.060 | 1.117 | 78.023 | 0.80x |
| nested.json | msgspec | 1.071 | 1.086 | 1.105 | 78.023 | 0.78x |
| nested.json | ujson | 1.512 | 1.539 | 1.633 | 78.023 | 0.55x |
| nested.json | json | 2.113 | 2.126 | 2.185 | 78.023 | 0.40x |
| wide_arrays.json | strata | 4.170 | 4.201 | 4.288 | 82.281 | 1.00x |
| wide_arrays.json | orjson | 5.152 | 5.212 | 5.410 | 82.281 | 0.81x |
| wide_arrays.json | msgspec | 5.707 | 5.784 | 5.986 | 82.281 | 0.73x |
| wide_arrays.json | ujson | 7.240 | 7.318 | 7.472 | 82.281 | 0.57x |
| wide_arrays.json | json | 9.774 | 9.842 | 10.250 | 82.281 | 0.43x |
| mixed.json | strata | 0.202 | 0.205 | 0.220 | 82.281 | 1.00x |
| mixed.json | orjson | 0.271 | 0.274 | 0.341 | 82.281 | 0.75x |
| mixed.json | msgspec | 0.280 | 0.290 | 0.302 | 82.281 | 0.71x |
| mixed.json | ujson | 0.355 | 0.364 | 0.385 | 82.281 | 0.56x |
| mixed.json | json | 0.513 | 0.532 | 0.577 | 82.281 | 0.39x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.110 | 10.229 | 11.278 | 77.988 | 1.00x |
| users.ndjson | orjson | 16.794 | 16.933 | 17.654 | 77.988 | 0.60x |
| users.ndjson | msgspec | 16.716 | 17.071 | 17.356 | 77.988 | 0.60x |
| users.ndjson | ujson | 22.009 | 22.180 | 22.547 | 77.988 | 0.46x |
| users.ndjson | json | 29.375 | 29.772 | 31.362 | 77.988 | 0.34x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.813 | 2.860 | 2.905 | 79.621 | 1.00x |
| users.json | orjson | 3.539 | 3.557 | 3.597 | 79.621 | 0.80x |
| users.json | msgspec | 4.441 | 4.464 | 4.526 | 79.621 | 0.64x |
| users.json | ujson | 12.560 | 12.799 | 14.192 | 79.621 | 0.22x |
| users.json | json | 22.422 | 22.498 | 22.563 | 79.621 | 0.13x |
| flat.json | strata | 0.422 | 0.435 | 0.462 | 78.023 | 1.00x |
| flat.json | orjson | 0.483 | 0.494 | 0.505 | 78.023 | 0.88x |
| flat.json | msgspec | 0.579 | 0.595 | 0.600 | 78.023 | 0.73x |
| flat.json | ujson | 1.185 | 1.193 | 1.221 | 78.023 | 0.36x |
| flat.json | json | 2.015 | 2.028 | 2.047 | 78.023 | 0.21x |
| nested.json | strata | 0.331 | 0.339 | 0.362 | 78.023 | 1.00x |
| nested.json | orjson | 0.418 | 0.426 | 0.441 | 78.023 | 0.80x |
| nested.json | msgspec | 0.527 | 0.542 | 0.549 | 78.023 | 0.63x |
| nested.json | ujson | 1.255 | 1.268 | 1.285 | 78.023 | 0.27x |
| nested.json | json | 2.518 | 2.530 | 2.605 | 78.023 | 0.13x |
| wide_arrays.json | strata | 2.022 | 2.055 | 2.092 | 82.281 | 1.00x |
| wide_arrays.json | orjson | 2.248 | 2.277 | 2.319 | 82.281 | 0.90x |
| wide_arrays.json | msgspec | 3.183 | 3.212 | 3.274 | 82.281 | 0.64x |
| wide_arrays.json | ujson | 6.898 | 6.925 | 7.032 | 82.281 | 0.30x |
| wide_arrays.json | json | 17.215 | 17.274 | 17.783 | 82.281 | 0.12x |
| mixed.json | strata | 0.144 | 0.146 | 0.164 | 82.281 | 1.00x |
| mixed.json | orjson | 0.170 | 0.172 | 0.192 | 82.281 | 0.85x |
| mixed.json | msgspec | 0.189 | 0.191 | 0.210 | 82.281 | 0.76x |
| mixed.json | ujson | 0.349 | 0.356 | 0.378 | 82.281 | 0.41x |
| mixed.json | json | 0.623 | 0.645 | 0.654 | 82.281 | 0.23x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.061 | 0.065 | 0.078 | 79.621 | 1.00x |
| users.json $[*].id | jmespath | 0.498 | 0.507 | 0.520 | 79.621 | 0.13x |
| users.json $[*].id | jsonpath-ng | 2.861 | 2.913 | 3.216 | 79.621 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.457 | 0.470 | 0.482 | 79.625 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.051 | 3.089 | 3.141 | 79.625 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 19.849 | 20.174 | 20.530 | 79.625 | 0.02x |
| users.json $..total | strata | 1.655 | 1.719 | 1.725 | 79.625 | 1.00x |
| users.json $..total | jsonpath-ng | 392.507 | 395.155 | 397.115 | 79.625 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.265 | 3.285 | 3.312 | 79.625 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.596 | 14.978 | 15.019 | 79.625 | 0.22x |
| users.json $[*].id | orjson+jsonpath-ng | 17.078 | 17.315 | 17.737 | 79.625 | 0.19x |
| users.json $[*].orders[*].total | strata | 3.510 | 3.530 | 3.574 | 79.625 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.767 | 17.876 | 18.450 | 79.625 | 0.20x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 37.653 | 38.238 | 40.182 | 79.625 | 0.09x |
| users.json $..total | strata | 13.387 | 14.180 | 15.016 | 79.625 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 416.854 | 418.891 | 421.645 | 79.625 | 0.03x |

