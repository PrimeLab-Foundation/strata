# Benchmark results - ci-linux-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: cd9d20b6ab3ec573716c10b12fe76ae4b70a707c
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-aarch64-with-glibc2.39
- machine: aarch64
- processor: aarch64
- compiler_flags: -fno-strict-overflow -Wsign-compare -DNDEBUG -g -O3 -Wall -fPIC -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -march=native -flto=thin -fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata -shared -Wl,--rpath=/opt/hostedtoolcache/Python/3.12.14/arm64/lib (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.308 | 9.479 | 11.876 | 57.340 | 1.00x |
| users.json | orjson | 12.373 | 12.664 | 14.519 | 57.340 | 0.75x |
| users.json | msgspec | 12.659 | 13.025 | 14.831 | 57.340 | 0.73x |
| users.json | ujson | 17.702 | 18.233 | 20.867 | 57.340 | 0.52x |
| users.json | pysimdjson | 17.717 | 18.424 | 20.631 | 57.340 | 0.51x |
| users.json | json | 21.458 | 21.886 | 22.621 | 57.340 | 0.43x |
| flat.json | strata | 0.824 | 0.832 | 0.873 | 58.402 | 1.00x |
| flat.json | orjson | 0.874 | 0.879 | 0.892 | 58.402 | 0.95x |
| flat.json | msgspec | 0.893 | 0.913 | 0.922 | 58.402 | 0.91x |
| flat.json | ujson | 1.417 | 1.429 | 1.471 | 58.402 | 0.58x |
| flat.json | pysimdjson | 1.447 | 1.472 | 1.484 | 58.402 | 0.57x |
| flat.json | json | 1.762 | 1.774 | 1.800 | 58.402 | 0.47x |
| nested.json | strata | 0.838 | 0.850 | 0.866 | 58.402 | 1.00x |
| nested.json | orjson | 0.892 | 0.909 | 0.921 | 58.402 | 0.94x |
| nested.json | msgspec | 1.006 | 1.020 | 1.032 | 58.402 | 0.83x |
| nested.json | ujson | 1.434 | 1.454 | 1.463 | 58.402 | 0.59x |
| nested.json | pysimdjson | 1.418 | 1.441 | 1.470 | 58.402 | 0.59x |
| nested.json | json | 2.000 | 2.019 | 2.058 | 58.402 | 0.42x |
| wide_arrays.json | strata | 4.126 | 4.182 | 4.453 | 64.957 | 1.00x |
| wide_arrays.json | orjson | 4.191 | 4.399 | 4.606 | 64.957 | 0.95x |
| wide_arrays.json | msgspec | 5.152 | 5.259 | 5.518 | 64.957 | 0.80x |
| wide_arrays.json | ujson | 6.681 | 6.857 | 7.019 | 64.957 | 0.61x |
| wide_arrays.json | pysimdjson | 5.483 | 5.631 | 5.846 | 64.957 | 0.74x |
| wide_arrays.json | json | 9.765 | 9.933 | 10.373 | 64.957 | 0.42x |
| mixed.json | strata | 0.199 | 0.205 | 0.224 | 64.957 | 1.00x |
| mixed.json | orjson | 0.221 | 0.230 | 0.248 | 64.957 | 0.89x |
| mixed.json | msgspec | 0.240 | 0.251 | 0.276 | 64.957 | 0.82x |
| mixed.json | ujson | 0.312 | 0.325 | 0.365 | 64.957 | 0.63x |
| mixed.json | pysimdjson | 0.305 | 0.308 | 0.325 | 64.957 | 0.67x |
| mixed.json | json | 0.475 | 0.493 | 0.504 | 64.957 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.961 | 1.970 | 1.998 | 45.980 | 1.00x |
| users.json | orjson | 2.597 | 2.621 | 2.632 | 45.980 | 0.75x |
| users.json | msgspec | 3.329 | 3.346 | 3.386 | 45.980 | 0.59x |
| users.json | ujson | 10.626 | 10.672 | 10.714 | 45.980 | 0.18x |
| users.json | json | 19.156 | 19.226 | 19.291 | 45.980 | 0.10x |
| flat.json | strata | 0.237 | 0.241 | 0.261 | 58.402 | 1.00x |
| flat.json | orjson | 0.298 | 0.300 | 0.319 | 58.402 | 0.80x |
| flat.json | msgspec | 0.389 | 0.393 | 0.414 | 58.402 | 0.61x |
| flat.json | ujson | 0.989 | 0.996 | 0.998 | 58.402 | 0.24x |
| flat.json | json | 1.699 | 1.716 | 1.721 | 58.402 | 0.14x |
| nested.json | strata | 0.218 | 0.221 | 0.244 | 58.402 | 1.00x |
| nested.json | orjson | 0.283 | 0.285 | 0.309 | 58.402 | 0.78x |
| nested.json | msgspec | 0.371 | 0.392 | 0.421 | 58.402 | 0.56x |
| nested.json | ujson | 1.101 | 1.119 | 1.122 | 58.402 | 0.20x |
| nested.json | json | 2.146 | 2.180 | 2.207 | 58.402 | 0.10x |
| wide_arrays.json | strata | 1.333 | 1.390 | 1.482 | 64.957 | 1.00x |
| wide_arrays.json | orjson | 1.608 | 1.650 | 1.694 | 64.957 | 0.84x |
| wide_arrays.json | msgspec | 2.391 | 2.414 | 2.435 | 64.957 | 0.58x |
| wide_arrays.json | ujson | 4.800 | 4.823 | 4.855 | 64.957 | 0.29x |
| wide_arrays.json | json | 13.669 | 13.764 | 13.886 | 64.957 | 0.10x |
| mixed.json | strata | 0.064 | 0.068 | 0.090 | 64.957 | 1.00x |
| mixed.json | orjson | 0.068 | 0.069 | 0.076 | 64.957 | 0.99x |
| mixed.json | msgspec | 0.083 | 0.086 | 0.092 | 64.957 | 0.79x |
| mixed.json | ujson | 0.249 | 0.251 | 0.258 | 64.957 | 0.27x |
| mixed.json | json | 0.500 | 0.525 | 0.541 | 64.957 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.675 | 9.843 | 10.613 | 59.789 | 1.00x |
| users.json | orjson | 12.811 | 13.114 | 13.392 | 59.789 | 0.75x |
| users.json | msgspec | 13.358 | 13.790 | 14.104 | 59.789 | 0.71x |
| users.json | ujson | 18.410 | 19.624 | 20.537 | 59.789 | 0.50x |
| users.json | json | 22.103 | 22.296 | 22.699 | 59.789 | 0.44x |
| flat.json | strata | 0.854 | 0.923 | 0.937 | 58.402 | 1.00x |
| flat.json | orjson | 0.966 | 0.982 | 1.003 | 58.402 | 0.94x |
| flat.json | msgspec | 1.001 | 1.012 | 1.022 | 58.402 | 0.91x |
| flat.json | ujson | 1.554 | 1.575 | 1.598 | 58.402 | 0.59x |
| flat.json | json | 1.845 | 1.856 | 1.878 | 58.402 | 0.50x |
| nested.json | strata | 0.878 | 0.908 | 0.941 | 58.402 | 1.00x |
| nested.json | orjson | 0.980 | 1.033 | 1.077 | 58.402 | 0.88x |
| nested.json | msgspec | 1.086 | 1.153 | 1.193 | 58.402 | 0.79x |
| nested.json | ujson | 1.539 | 1.603 | 1.642 | 58.402 | 0.57x |
| nested.json | json | 2.061 | 2.111 | 2.171 | 58.402 | 0.43x |
| wide_arrays.json | strata | 4.013 | 4.063 | 4.172 | 64.957 | 1.00x |
| wide_arrays.json | orjson | 4.264 | 4.314 | 4.605 | 64.957 | 0.94x |
| wide_arrays.json | msgspec | 5.190 | 5.316 | 5.513 | 64.957 | 0.76x |
| wide_arrays.json | ujson | 6.800 | 6.908 | 7.154 | 64.957 | 0.59x |
| wide_arrays.json | json | 9.627 | 9.825 | 10.083 | 64.957 | 0.41x |
| mixed.json | strata | 0.235 | 0.241 | 0.263 | 64.957 | 1.00x |
| mixed.json | orjson | 0.313 | 0.323 | 0.397 | 64.957 | 0.75x |
| mixed.json | msgspec | 0.320 | 0.341 | 0.414 | 64.957 | 0.71x |
| mixed.json | ujson | 0.406 | 0.436 | 0.469 | 64.957 | 0.55x |
| mixed.json | json | 0.545 | 0.564 | 0.587 | 64.957 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.574 | 9.796 | 10.124 | 58.398 | 1.00x |
| users.ndjson | orjson | 14.775 | 15.081 | 15.642 | 58.398 | 0.65x |
| users.ndjson | msgspec | 15.156 | 15.326 | 15.750 | 58.398 | 0.64x |
| users.ndjson | ujson | 19.663 | 20.312 | 20.520 | 58.398 | 0.48x |
| users.ndjson | json | 25.737 | 26.823 | 27.433 | 58.398 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.713 | 2.806 | 2.907 | 59.789 | 1.00x |
| users.json | orjson | 3.455 | 3.582 | 3.755 | 59.789 | 0.78x |
| users.json | msgspec | 4.292 | 4.318 | 4.390 | 59.789 | 0.65x |
| users.json | ujson | 11.655 | 11.794 | 11.879 | 59.789 | 0.24x |
| users.json | json | 20.422 | 20.467 | 20.622 | 59.789 | 0.14x |
| flat.json | strata | 0.478 | 0.492 | 0.533 | 58.402 | 1.00x |
| flat.json | orjson | 0.569 | 0.579 | 0.687 | 58.402 | 0.85x |
| flat.json | msgspec | 0.669 | 0.681 | 0.708 | 58.402 | 0.72x |
| flat.json | ujson | 1.275 | 1.303 | 1.329 | 58.402 | 0.38x |
| flat.json | json | 1.990 | 2.018 | 2.068 | 58.402 | 0.24x |
| nested.json | strata | 0.421 | 0.478 | 0.533 | 58.402 | 1.00x |
| nested.json | orjson | 0.544 | 0.596 | 0.627 | 58.402 | 0.80x |
| nested.json | msgspec | 0.656 | 0.682 | 0.709 | 58.402 | 0.70x |
| nested.json | ujson | 1.400 | 1.416 | 1.477 | 58.402 | 0.34x |
| nested.json | json | 2.489 | 2.531 | 2.568 | 58.402 | 0.19x |
| wide_arrays.json | strata | 1.945 | 2.068 | 2.118 | 64.957 | 1.00x |
| wide_arrays.json | orjson | 2.256 | 2.332 | 2.445 | 64.957 | 0.89x |
| wide_arrays.json | msgspec | 3.026 | 3.071 | 3.134 | 64.957 | 0.67x |
| wide_arrays.json | ujson | 5.498 | 5.564 | 5.640 | 64.957 | 0.37x |
| wide_arrays.json | json | 14.532 | 14.598 | 14.739 | 64.957 | 0.14x |
| mixed.json | strata | 0.272 | 0.308 | 1.271 | 64.957 | 1.00x |
| mixed.json | orjson | 0.319 | 0.358 | 0.382 | 64.957 | 0.86x |
| mixed.json | msgspec | 0.350 | 0.379 | 0.415 | 64.957 | 0.81x |
| mixed.json | ujson | 0.524 | 0.558 | 0.606 | 64.957 | 0.55x |
| mixed.json | json | 0.790 | 0.805 | 0.859 | 64.957 | 0.38x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.119 | 0.124 | 0.139 | 59.789 | 1.00x |
| users.json $[*].id | jmespath | 0.502 | 0.514 | 0.526 | 59.789 | 0.24x |
| users.json $[*].id | jsonpath-ng | 2.516 | 2.660 | 2.734 | 59.789 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.710 | 0.742 | 1.536 | 59.906 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.125 | 3.216 | 3.311 | 59.906 | 0.23x |
| users.json $[*].orders[*].total | jsonpath-ng | 19.323 | 20.388 | 21.138 | 59.906 | 0.04x |
| users.json $..total | strata | 1.727 | 1.733 | 1.771 | 60.016 | 1.00x |
| users.json $..total | jsonpath-ng | 294.373 | 295.221 | 296.111 | 60.016 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.257 | 3.280 | 3.929 | 59.906 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.818 | 14.374 | 14.589 | 59.906 | 0.23x |
| users.json $[*].id | orjson+jsonpath-ng | 16.092 | 16.320 | 16.533 | 59.906 | 0.20x |
| users.json $[*].orders[*].total | strata | 3.444 | 3.454 | 4.085 | 60.016 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 16.880 | 17.457 | 17.877 | 60.016 | 0.20x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 36.577 | 38.993 | 40.202 | 60.016 | 0.09x |
| users.json $..total | strata | 11.546 | 12.497 | 12.998 | 60.035 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 312.079 | 315.121 | 316.804 | 60.035 | 0.04x |

