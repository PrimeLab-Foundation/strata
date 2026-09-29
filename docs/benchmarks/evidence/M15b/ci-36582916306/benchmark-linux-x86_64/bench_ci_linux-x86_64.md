# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 82e3fa5f24c38cfa1150440f0cf5da6c286c01cd
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: AMD EPYC 9V74 80-Core Processor
- compiler_flags: -fno-strict-overflow -Wsign-compare -DNDEBUG -g -O3 -Wall -fPIC -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -fomit-frame-pointer -march=native -flto=thin -fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata -fsplit-machine-functions -shared -Wl,--rpath=/opt/hostedtoolcache/Python/3.12.14/x64/lib (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.979 | 12.868 | 16.513 | 68.895 | 1.00x |
| users.json | orjson | 14.575 | 16.378 | 19.303 | 68.895 | 0.79x |
| users.json | msgspec | 14.244 | 15.636 | 20.515 | 68.895 | 0.82x |
| users.json | ujson | 20.774 | 22.329 | 29.878 | 68.895 | 0.58x |
| users.json | pysimdjson | 21.726 | 23.408 | 27.664 | 68.895 | 0.55x |
| users.json | json | 21.944 | 23.065 | 24.984 | 68.895 | 0.56x |
| flat.json | strata | 0.959 | 1.069 | 1.149 | 65.254 | 1.00x |
| flat.json | orjson | 1.161 | 1.229 | 1.300 | 65.254 | 0.87x |
| flat.json | msgspec | 1.146 | 1.231 | 1.293 | 65.254 | 0.87x |
| flat.json | ujson | 1.835 | 2.011 | 2.077 | 65.254 | 0.53x |
| flat.json | pysimdjson | 1.761 | 2.042 | 2.204 | 65.254 | 0.52x |
| flat.json | json | 1.745 | 1.908 | 2.002 | 65.254 | 0.56x |
| nested.json | strata | 0.799 | 0.825 | 0.947 | 65.254 | 1.00x |
| nested.json | orjson | 1.004 | 1.019 | 1.204 | 65.254 | 0.81x |
| nested.json | msgspec | 0.978 | 0.995 | 1.056 | 65.254 | 0.83x |
| nested.json | ujson | 1.467 | 1.505 | 1.880 | 65.254 | 0.55x |
| nested.json | pysimdjson | 1.410 | 1.431 | 1.734 | 65.254 | 0.58x |
| nested.json | json | 1.832 | 1.865 | 2.000 | 65.254 | 0.44x |
| wide_arrays.json | strata | 4.556 | 4.822 | 5.490 | 78.328 | 1.00x |
| wide_arrays.json | orjson | 5.873 | 6.175 | 6.706 | 78.328 | 0.78x |
| wide_arrays.json | msgspec | 6.201 | 6.524 | 6.912 | 78.328 | 0.74x |
| wide_arrays.json | ujson | 7.830 | 8.316 | 9.437 | 78.328 | 0.58x |
| wide_arrays.json | pysimdjson | 6.717 | 7.504 | 8.659 | 78.328 | 0.64x |
| wide_arrays.json | json | 10.242 | 10.781 | 11.956 | 78.328 | 0.45x |
| mixed.json | strata | 0.197 | 0.211 | 0.256 | 78.328 | 1.00x |
| mixed.json | orjson | 0.241 | 0.278 | 0.320 | 78.328 | 0.76x |
| mixed.json | msgspec | 0.247 | 0.277 | 0.316 | 78.328 | 0.76x |
| mixed.json | ujson | 0.311 | 0.367 | 0.410 | 78.328 | 0.57x |
| mixed.json | pysimdjson | 0.315 | 0.362 | 0.401 | 78.328 | 0.58x |
| mixed.json | json | 0.462 | 0.505 | 0.553 | 78.328 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.391 | 2.520 | 2.711 | 47.285 | 1.00x |
| users.json | orjson | 3.257 | 3.321 | 3.533 | 47.285 | 0.76x |
| users.json | msgspec | 4.191 | 4.257 | 4.387 | 47.285 | 0.59x |
| users.json | ujson | 11.389 | 11.617 | 11.730 | 47.285 | 0.22x |
| users.json | json | 22.258 | 22.502 | 23.062 | 47.285 | 0.11x |
| flat.json | strata | 0.317 | 0.327 | 0.341 | 65.254 | 1.00x |
| flat.json | orjson | 0.369 | 0.380 | 0.419 | 65.254 | 0.86x |
| flat.json | msgspec | 0.491 | 0.493 | 0.566 | 65.254 | 0.66x |
| flat.json | ujson | 1.023 | 1.041 | 1.067 | 65.254 | 0.31x |
| flat.json | json | 1.871 | 1.883 | 1.916 | 65.254 | 0.17x |
| nested.json | strata | 0.232 | 0.236 | 0.250 | 65.254 | 1.00x |
| nested.json | orjson | 0.309 | 0.314 | 0.321 | 65.254 | 0.75x |
| nested.json | msgspec | 0.435 | 0.444 | 0.461 | 65.254 | 0.53x |
| nested.json | ujson | 1.074 | 1.086 | 1.106 | 65.254 | 0.22x |
| nested.json | json | 2.389 | 2.399 | 2.449 | 65.254 | 0.10x |
| wide_arrays.json | strata | 1.830 | 1.895 | 2.293 | 78.328 | 1.00x |
| wide_arrays.json | orjson | 1.972 | 1.997 | 2.795 | 78.328 | 0.95x |
| wide_arrays.json | msgspec | 3.176 | 3.217 | 3.846 | 78.328 | 0.59x |
| wide_arrays.json | ujson | 6.454 | 6.610 | 6.922 | 78.328 | 0.29x |
| wide_arrays.json | json | 17.361 | 17.959 | 19.056 | 78.328 | 0.11x |
| mixed.json | strata | 0.066 | 0.071 | 0.074 | 78.328 | 1.00x |
| mixed.json | orjson | 0.073 | 0.076 | 0.084 | 78.328 | 0.93x |
| mixed.json | msgspec | 0.095 | 0.098 | 0.104 | 78.328 | 0.72x |
| mixed.json | ujson | 0.233 | 0.238 | 0.252 | 78.328 | 0.30x |
| mixed.json | json | 0.526 | 0.538 | 0.551 | 78.328 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.652 | 12.328 | 13.797 | 66.887 | 1.00x |
| users.json | orjson | 15.218 | 15.629 | 16.444 | 66.887 | 0.79x |
| users.json | msgspec | 15.389 | 15.676 | 17.216 | 66.887 | 0.79x |
| users.json | ujson | 21.268 | 22.342 | 25.097 | 66.887 | 0.55x |
| users.json | json | 22.454 | 23.240 | 24.234 | 66.887 | 0.53x |
| flat.json | strata | 0.942 | 0.981 | 1.014 | 65.254 | 1.00x |
| flat.json | orjson | 1.139 | 1.169 | 1.207 | 65.254 | 0.84x |
| flat.json | msgspec | 1.156 | 1.181 | 1.203 | 65.254 | 0.83x |
| flat.json | ujson | 1.779 | 1.827 | 1.884 | 65.254 | 0.54x |
| flat.json | json | 1.801 | 1.828 | 1.887 | 65.254 | 0.54x |
| nested.json | strata | 0.833 | 0.862 | 0.885 | 65.254 | 1.00x |
| nested.json | orjson | 1.080 | 1.090 | 1.143 | 65.254 | 0.79x |
| nested.json | msgspec | 1.046 | 1.069 | 1.084 | 65.254 | 0.81x |
| nested.json | ujson | 1.544 | 1.593 | 1.646 | 65.254 | 0.54x |
| nested.json | json | 1.957 | 1.969 | 2.007 | 65.254 | 0.44x |
| wide_arrays.json | strata | 4.570 | 4.779 | 5.259 | 78.328 | 1.00x |
| wide_arrays.json | orjson | 5.825 | 6.040 | 6.422 | 78.328 | 0.79x |
| wide_arrays.json | msgspec | 6.544 | 6.708 | 7.135 | 78.328 | 0.71x |
| wide_arrays.json | ujson | 8.046 | 8.376 | 8.830 | 78.328 | 0.57x |
| wide_arrays.json | json | 10.141 | 10.333 | 11.038 | 78.328 | 0.46x |
| mixed.json | strata | 0.220 | 0.239 | 0.263 | 78.328 | 1.00x |
| mixed.json | orjson | 0.300 | 0.333 | 0.369 | 78.328 | 0.72x |
| mixed.json | msgspec | 0.308 | 0.333 | 0.361 | 78.328 | 0.72x |
| mixed.json | ujson | 0.374 | 0.422 | 0.453 | 78.328 | 0.57x |
| mixed.json | json | 0.505 | 0.551 | 0.567 | 78.328 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 13.909 | 15.079 | 15.810 | 65.254 | 1.00x |
| users.ndjson | orjson | 20.243 | 21.698 | 24.107 | 65.254 | 0.69x |
| users.ndjson | msgspec | 20.429 | 22.220 | 23.857 | 65.254 | 0.68x |
| users.ndjson | ujson | 26.627 | 28.551 | 30.630 | 65.254 | 0.53x |
| users.ndjson | json | 32.877 | 33.977 | 34.696 | 65.254 | 0.44x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.135 | 3.232 | 3.342 | 66.887 | 1.00x |
| users.json | orjson | 4.109 | 4.199 | 4.360 | 66.887 | 0.77x |
| users.json | msgspec | 4.961 | 5.097 | 5.491 | 66.887 | 0.63x |
| users.json | ujson | 12.333 | 12.635 | 12.804 | 66.887 | 0.26x |
| users.json | json | 23.060 | 23.405 | 24.025 | 66.887 | 0.14x |
| flat.json | strata | 0.531 | 0.579 | 45.191 | 65.254 | 1.00x |
| flat.json | orjson | 0.631 | 0.650 | 9.071 | 65.254 | 0.89x |
| flat.json | msgspec | 0.730 | 0.765 | 2.013 | 65.254 | 0.76x |
| flat.json | ujson | 1.322 | 1.375 | 2.718 | 65.254 | 0.42x |
| flat.json | json | 2.163 | 2.252 | 2.334 | 65.254 | 0.26x |
| nested.json | strata | 0.415 | 0.430 | 0.459 | 65.254 | 1.00x |
| nested.json | orjson | 0.512 | 0.542 | 0.565 | 65.254 | 0.79x |
| nested.json | msgspec | 0.643 | 0.676 | 0.871 | 65.254 | 0.64x |
| nested.json | ujson | 1.307 | 1.324 | 1.335 | 65.254 | 0.32x |
| nested.json | json | 2.594 | 2.617 | 2.671 | 65.254 | 0.16x |
| wide_arrays.json | strata | 2.317 | 2.440 | 2.869 | 78.328 | 1.00x |
| wide_arrays.json | orjson | 2.509 | 2.654 | 2.804 | 78.328 | 0.92x |
| wide_arrays.json | msgspec | 3.671 | 3.824 | 3.928 | 78.328 | 0.64x |
| wide_arrays.json | ujson | 7.039 | 7.159 | 7.401 | 78.328 | 0.34x |
| wide_arrays.json | json | 17.778 | 18.216 | 19.012 | 78.328 | 0.13x |
| mixed.json | strata | 0.216 | 0.229 | 0.500 | 78.328 | 1.00x |
| mixed.json | orjson | 0.241 | 0.253 | 0.268 | 78.328 | 0.90x |
| mixed.json | msgspec | 0.258 | 0.278 | 0.297 | 78.328 | 0.82x |
| mixed.json | ujson | 0.408 | 0.439 | 0.515 | 78.328 | 0.52x |
| mixed.json | json | 0.716 | 0.736 | 0.756 | 78.328 | 0.31x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.082 | 0.086 | 0.106 | 66.887 | 1.00x |
| users.json $[*].id | jmespath | 0.478 | 0.485 | 0.575 | 66.887 | 0.18x |
| users.json $[*].id | jsonpath-ng | 2.874 | 3.055 | 3.105 | 66.887 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.447 | 0.477 | 0.576 | 66.891 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.016 | 3.086 | 3.202 | 66.891 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 21.117 | 22.148 | 22.956 | 66.891 | 0.02x |
| users.json $..total | strata | 1.863 | 1.917 | 2.028 | 66.891 | 1.00x |
| users.json $..total | jsonpath-ng | 386.012 | 387.171 | 400.056 | 66.891 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.289 | 3.296 | 3.308 | 66.891 | 1.00x |
| users.json $[*].id | orjson+jmespath | 16.782 | 17.533 | 18.393 | 66.891 | 0.19x |
| users.json $[*].id | orjson+jsonpath-ng | 19.280 | 20.975 | 22.075 | 66.891 | 0.16x |
| users.json $[*].orders[*].total | strata | 3.523 | 3.593 | 3.648 | 66.891 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 20.076 | 20.789 | 23.925 | 66.891 | 0.17x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 43.691 | 45.936 | 48.279 | 66.891 | 0.08x |
| users.json $..total | strata | 16.431 | 19.631 | 22.973 | 66.891 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 415.024 | 419.686 | 423.481 | 66.891 | 0.05x |

