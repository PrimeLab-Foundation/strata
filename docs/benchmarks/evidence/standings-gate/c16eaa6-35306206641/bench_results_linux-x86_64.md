# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c16eaa65c61f1cf9d4fac46e9f6d8e6b9f3b087f
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
| users.json | strata | 9.614 | 9.712 | 13.955 | 66.586 | 1.00x |
| users.json | orjson | 13.007 | 13.192 | 16.675 | 66.586 | 0.74x |
| users.json | msgspec | 13.084 | 13.268 | 16.385 | 66.586 | 0.73x |
| users.json | ujson | 17.474 | 18.067 | 23.431 | 66.586 | 0.54x |
| users.json | pysimdjson | 17.751 | 19.303 | 23.918 | 66.586 | 0.50x |
| users.json | json | 22.469 | 22.718 | 26.019 | 66.586 | 0.43x |
| flat.json | strata | 0.835 | 0.855 | 0.915 | 82.355 | 1.00x |
| flat.json | orjson | 0.980 | 0.986 | 1.001 | 82.355 | 0.87x |
| flat.json | msgspec | 1.006 | 1.017 | 1.048 | 82.355 | 0.84x |
| flat.json | ujson | 1.458 | 1.483 | 1.621 | 82.355 | 0.58x |
| flat.json | pysimdjson | 1.530 | 1.574 | 1.766 | 82.355 | 0.54x |
| flat.json | json | 1.922 | 1.932 | 1.977 | 82.355 | 0.44x |
| nested.json | strata | 0.793 | 0.807 | 0.820 | 82.355 | 1.00x |
| nested.json | orjson | 1.004 | 1.014 | 1.026 | 82.355 | 0.80x |
| nested.json | msgspec | 1.018 | 1.025 | 1.056 | 82.355 | 0.79x |
| nested.json | ujson | 1.449 | 1.482 | 1.611 | 82.355 | 0.54x |
| nested.json | pysimdjson | 1.397 | 1.414 | 1.449 | 82.355 | 0.57x |
| nested.json | json | 2.044 | 2.063 | 2.083 | 82.355 | 0.39x |
| wide_arrays.json | strata | 4.214 | 4.341 | 4.902 | 84.355 | 1.00x |
| wide_arrays.json | orjson | 5.311 | 5.737 | 6.253 | 84.355 | 0.76x |
| wide_arrays.json | msgspec | 5.851 | 6.174 | 6.507 | 84.355 | 0.70x |
| wide_arrays.json | ujson | 7.203 | 7.720 | 7.921 | 84.355 | 0.56x |
| wide_arrays.json | pysimdjson | 6.336 | 6.945 | 7.494 | 84.355 | 0.63x |
| wide_arrays.json | json | 10.043 | 10.563 | 11.240 | 84.355 | 0.41x |
| mixed.json | strata | 0.189 | 0.191 | 0.215 | 84.355 | 1.00x |
| mixed.json | orjson | 0.226 | 0.228 | 0.251 | 84.355 | 0.84x |
| mixed.json | msgspec | 0.237 | 0.240 | 0.258 | 84.355 | 0.80x |
| mixed.json | ujson | 0.298 | 0.309 | 0.325 | 84.355 | 0.62x |
| mixed.json | pysimdjson | 0.297 | 0.300 | 0.314 | 84.355 | 0.63x |
| mixed.json | json | 0.475 | 0.490 | 0.635 | 84.355 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.264 | 2.300 | 2.321 | 64.945 | 1.00x |
| users.json | orjson | 2.881 | 2.901 | 2.936 | 64.945 | 0.79x |
| users.json | msgspec | 3.808 | 3.838 | 3.892 | 64.945 | 0.60x |
| users.json | ujson | 11.703 | 11.840 | 11.987 | 64.945 | 0.19x |
| users.json | json | 21.505 | 21.660 | 21.934 | 64.945 | 0.11x |
| flat.json | strata | 0.277 | 0.283 | 0.297 | 82.355 | 1.00x |
| flat.json | orjson | 0.325 | 0.334 | 0.344 | 82.355 | 0.85x |
| flat.json | msgspec | 0.420 | 0.426 | 0.440 | 82.355 | 0.66x |
| flat.json | ujson | 1.012 | 1.030 | 1.080 | 82.355 | 0.27x |
| flat.json | json | 1.830 | 1.855 | 1.866 | 82.355 | 0.15x |
| nested.json | strata | 0.226 | 0.229 | 0.241 | 82.355 | 1.00x |
| nested.json | orjson | 0.288 | 0.291 | 0.304 | 82.355 | 0.79x |
| nested.json | msgspec | 0.399 | 0.413 | 0.444 | 82.355 | 0.55x |
| nested.json | ujson | 1.083 | 1.093 | 1.103 | 82.355 | 0.21x |
| nested.json | json | 2.332 | 2.358 | 2.398 | 82.355 | 0.10x |
| wide_arrays.json | strata | 1.659 | 1.726 | 1.884 | 84.355 | 1.00x |
| wide_arrays.json | orjson | 1.856 | 1.871 | 1.931 | 84.355 | 0.92x |
| wide_arrays.json | msgspec | 2.781 | 2.815 | 2.937 | 84.355 | 0.61x |
| wide_arrays.json | ujson | 6.455 | 6.592 | 6.946 | 84.355 | 0.26x |
| wide_arrays.json | json | 16.707 | 17.032 | 17.516 | 84.355 | 0.10x |
| mixed.json | strata | 0.060 | 0.060 | 0.092 | 84.355 | 1.00x |
| mixed.json | orjson | 0.063 | 0.065 | 0.066 | 84.355 | 0.94x |
| mixed.json | msgspec | 0.082 | 0.083 | 0.096 | 84.355 | 0.73x |
| mixed.json | ujson | 0.234 | 0.237 | 0.259 | 84.355 | 0.26x |
| mixed.json | json | 0.501 | 0.512 | 0.523 | 84.355 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.129 | 10.923 | 11.772 | 81.840 | 1.00x |
| users.json | orjson | 13.204 | 13.887 | 14.616 | 81.840 | 0.79x |
| users.json | msgspec | 13.441 | 14.059 | 14.312 | 81.840 | 0.78x |
| users.json | ujson | 19.352 | 20.265 | 21.216 | 81.840 | 0.54x |
| users.json | json | 22.700 | 23.350 | 23.770 | 81.840 | 0.47x |
| flat.json | strata | 0.875 | 0.892 | 0.907 | 82.355 | 1.00x |
| flat.json | orjson | 1.035 | 1.043 | 1.056 | 82.355 | 0.86x |
| flat.json | msgspec | 1.057 | 1.071 | 1.092 | 82.355 | 0.83x |
| flat.json | ujson | 1.550 | 1.580 | 1.624 | 82.355 | 0.56x |
| flat.json | json | 1.975 | 1.989 | 2.014 | 82.355 | 0.45x |
| nested.json | strata | 0.807 | 0.825 | 0.834 | 82.355 | 1.00x |
| nested.json | orjson | 1.042 | 1.056 | 1.101 | 82.355 | 0.78x |
| nested.json | msgspec | 1.068 | 1.075 | 1.115 | 82.355 | 0.77x |
| nested.json | ujson | 1.516 | 1.534 | 1.563 | 82.355 | 0.54x |
| nested.json | json | 2.090 | 2.113 | 2.141 | 82.355 | 0.39x |
| wide_arrays.json | strata | 4.194 | 4.265 | 4.317 | 84.355 | 1.00x |
| wide_arrays.json | orjson | 5.289 | 5.369 | 5.523 | 84.355 | 0.79x |
| wide_arrays.json | msgspec | 5.781 | 5.927 | 6.333 | 84.355 | 0.72x |
| wide_arrays.json | ujson | 7.262 | 7.369 | 7.547 | 84.355 | 0.58x |
| wide_arrays.json | json | 9.790 | 9.894 | 10.533 | 84.355 | 0.43x |
| mixed.json | strata | 0.204 | 0.205 | 0.329 | 84.355 | 1.00x |
| mixed.json | orjson | 0.274 | 0.278 | 0.300 | 84.355 | 0.74x |
| mixed.json | msgspec | 0.280 | 0.284 | 0.306 | 84.355 | 0.72x |
| mixed.json | ujson | 0.356 | 0.369 | 0.382 | 84.355 | 0.56x |
| mixed.json | json | 0.519 | 0.536 | 0.540 | 84.355 | 0.38x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.398 | 11.862 | 13.119 | 82.355 | 1.00x |
| users.ndjson | orjson | 16.846 | 17.848 | 19.808 | 82.355 | 0.66x |
| users.ndjson | msgspec | 17.244 | 18.703 | 20.604 | 82.355 | 0.63x |
| users.ndjson | ujson | 22.620 | 24.021 | 26.443 | 82.355 | 0.49x |
| users.ndjson | json | 30.250 | 31.526 | 33.473 | 82.355 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.860 | 2.942 | 3.071 | 81.840 | 1.00x |
| users.json | orjson | 3.552 | 3.665 | 3.781 | 81.840 | 0.80x |
| users.json | msgspec | 4.389 | 4.467 | 4.578 | 81.840 | 0.66x |
| users.json | ujson | 12.607 | 12.766 | 13.179 | 81.840 | 0.23x |
| users.json | json | 22.207 | 22.463 | 22.650 | 81.840 | 0.13x |
| flat.json | strata | 0.415 | 0.430 | 0.445 | 82.355 | 1.00x |
| flat.json | orjson | 0.477 | 0.499 | 0.526 | 82.355 | 0.86x |
| flat.json | msgspec | 0.595 | 0.603 | 0.634 | 82.355 | 0.71x |
| flat.json | ujson | 1.189 | 1.198 | 1.214 | 82.355 | 0.36x |
| flat.json | json | 2.019 | 2.034 | 2.116 | 82.355 | 0.21x |
| nested.json | strata | 0.326 | 0.333 | 0.360 | 82.355 | 1.00x |
| nested.json | orjson | 0.415 | 0.430 | 0.450 | 82.355 | 0.77x |
| nested.json | msgspec | 0.522 | 0.541 | 0.565 | 82.355 | 0.62x |
| nested.json | ujson | 1.210 | 1.218 | 1.233 | 82.355 | 0.27x |
| nested.json | json | 2.488 | 2.507 | 2.545 | 82.355 | 0.13x |
| wide_arrays.json | strata | 2.060 | 2.087 | 2.121 | 84.355 | 1.00x |
| wide_arrays.json | orjson | 2.283 | 2.296 | 2.321 | 84.355 | 0.91x |
| wide_arrays.json | msgspec | 3.197 | 3.224 | 3.246 | 84.355 | 0.65x |
| wide_arrays.json | ujson | 6.942 | 6.974 | 7.011 | 84.355 | 0.30x |
| wide_arrays.json | json | 17.059 | 17.134 | 17.382 | 84.355 | 0.12x |
| mixed.json | strata | 0.139 | 0.140 | 0.151 | 84.355 | 1.00x |
| mixed.json | orjson | 0.162 | 0.165 | 0.183 | 84.355 | 0.85x |
| mixed.json | msgspec | 0.181 | 0.188 | 0.218 | 84.355 | 0.75x |
| mixed.json | ujson | 0.343 | 0.346 | 0.369 | 84.355 | 0.41x |
| mixed.json | json | 0.615 | 0.635 | 0.687 | 84.355 | 0.22x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.061 | 0.063 | 0.067 | 81.840 | 1.00x |
| users.json $[*].id | jmespath | 0.511 | 0.529 | 0.537 | 81.840 | 0.12x |
| users.json $[*].id | jsonpath-ng | 2.844 | 2.895 | 3.009 | 81.840 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.421 | 0.456 | 0.500 | 81.844 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.211 | 3.238 | 3.297 | 81.844 | 0.14x |
| users.json $[*].orders[*].total | jsonpath-ng | 19.345 | 20.718 | 22.287 | 81.844 | 0.02x |
| users.json $..total | strata | 1.705 | 1.741 | 2.081 | 83.023 | 1.00x |
| users.json $..total | jsonpath-ng | 389.812 | 393.860 | 399.848 | 83.023 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.237 | 3.300 | 3.345 | 81.844 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.406 | 14.914 | 15.362 | 81.844 | 0.22x |
| users.json $[*].id | orjson+jsonpath-ng | 16.832 | 17.575 | 19.053 | 81.844 | 0.19x |
| users.json $[*].orders[*].total | strata | 3.498 | 3.510 | 3.542 | 81.844 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.405 | 17.732 | 17.867 | 81.844 | 0.20x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 37.076 | 37.778 | 39.523 | 81.844 | 0.09x |
| users.json $..total | strata | 13.015 | 13.679 | 15.536 | 83.992 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 404.965 | 408.728 | 420.176 | 83.992 | 0.03x |

