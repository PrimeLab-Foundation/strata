# Benchmark results - ci-linux-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 33465c22eee0fda8e1b52898c16aac6982901a21
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
| users.json | strata | 8.932 | 8.990 | 10.813 | 57.254 | 1.00x |
| users.json | orjson | 11.576 | 11.726 | 13.286 | 57.254 | 0.77x |
| users.json | msgspec | 12.184 | 12.282 | 13.631 | 57.254 | 0.73x |
| users.json | ujson | 16.403 | 16.494 | 18.601 | 57.254 | 0.55x |
| users.json | pysimdjson | 16.257 | 16.552 | 18.281 | 57.254 | 0.54x |
| users.json | json | 20.585 | 20.725 | 21.683 | 57.254 | 0.43x |
| flat.json | strata | 0.840 | 0.885 | 0.892 | 68.527 | 1.00x |
| flat.json | orjson | 0.870 | 0.888 | 0.911 | 68.527 | 1.00x |
| flat.json | msgspec | 0.922 | 0.932 | 0.949 | 68.527 | 0.95x |
| flat.json | ujson | 1.452 | 1.468 | 1.488 | 68.527 | 0.60x |
| flat.json | pysimdjson | 1.480 | 1.503 | 1.538 | 68.527 | 0.59x |
| flat.json | json | 1.777 | 1.794 | 1.808 | 68.527 | 0.49x |
| nested.json | strata | 0.815 | 0.831 | 0.836 | 68.527 | 1.00x |
| nested.json | orjson | 0.868 | 0.885 | 0.901 | 68.527 | 0.94x |
| nested.json | msgspec | 1.000 | 1.008 | 1.022 | 68.527 | 0.82x |
| nested.json | ujson | 1.393 | 1.412 | 1.425 | 68.527 | 0.59x |
| nested.json | pysimdjson | 1.396 | 1.404 | 1.417 | 68.527 | 0.59x |
| nested.json | json | 1.956 | 1.972 | 1.990 | 68.527 | 0.42x |
| wide_arrays.json | strata | 3.940 | 3.965 | 4.026 | 70.156 | 1.00x |
| wide_arrays.json | orjson | 4.078 | 4.100 | 4.144 | 70.156 | 0.97x |
| wide_arrays.json | msgspec | 5.089 | 5.120 | 5.171 | 70.156 | 0.77x |
| wide_arrays.json | ujson | 6.474 | 6.512 | 6.557 | 70.156 | 0.61x |
| wide_arrays.json | pysimdjson | 5.291 | 5.322 | 5.351 | 70.156 | 0.75x |
| wide_arrays.json | json | 9.487 | 9.537 | 9.622 | 70.156 | 0.42x |
| mixed.json | strata | 0.191 | 0.199 | 0.222 | 70.156 | 1.00x |
| mixed.json | orjson | 0.211 | 0.217 | 0.245 | 70.156 | 0.91x |
| mixed.json | msgspec | 0.231 | 0.241 | 0.278 | 70.156 | 0.82x |
| mixed.json | ujson | 0.298 | 0.316 | 0.338 | 70.156 | 0.63x |
| mixed.json | pysimdjson | 0.293 | 0.301 | 0.322 | 70.156 | 0.66x |
| mixed.json | json | 0.462 | 0.478 | 0.493 | 70.156 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.920 | 1.926 | 1.935 | 56.352 | 1.00x |
| users.json | orjson | 2.582 | 2.597 | 2.653 | 56.352 | 0.74x |
| users.json | msgspec | 3.278 | 3.293 | 3.472 | 56.352 | 0.58x |
| users.json | ujson | 10.550 | 10.606 | 10.646 | 56.352 | 0.18x |
| users.json | json | 19.145 | 19.277 | 19.466 | 56.352 | 0.10x |
| flat.json | strata | 0.234 | 0.238 | 0.259 | 68.527 | 1.00x |
| flat.json | orjson | 0.300 | 0.303 | 0.321 | 68.527 | 0.79x |
| flat.json | msgspec | 0.390 | 0.392 | 0.413 | 68.527 | 0.61x |
| flat.json | ujson | 0.987 | 0.992 | 1.011 | 68.527 | 0.24x |
| flat.json | json | 1.702 | 1.716 | 1.743 | 68.527 | 0.14x |
| nested.json | strata | 0.215 | 0.223 | 0.244 | 68.527 | 1.00x |
| nested.json | orjson | 0.285 | 0.288 | 0.305 | 68.527 | 0.77x |
| nested.json | msgspec | 0.369 | 0.372 | 0.397 | 68.527 | 0.60x |
| nested.json | ujson | 1.101 | 1.114 | 1.137 | 68.527 | 0.20x |
| nested.json | json | 2.176 | 2.200 | 2.237 | 68.527 | 0.10x |
| wide_arrays.json | strata | 1.313 | 1.322 | 1.334 | 70.156 | 1.00x |
| wide_arrays.json | orjson | 1.556 | 1.567 | 1.579 | 70.156 | 0.84x |
| wide_arrays.json | msgspec | 2.372 | 2.394 | 2.405 | 70.156 | 0.55x |
| wide_arrays.json | ujson | 4.735 | 4.756 | 4.784 | 70.156 | 0.28x |
| wide_arrays.json | json | 13.581 | 13.604 | 13.690 | 70.156 | 0.10x |
| mixed.json | strata | 0.061 | 0.066 | 0.068 | 70.156 | 1.00x |
| mixed.json | orjson | 0.065 | 0.066 | 0.068 | 70.156 | 0.99x |
| mixed.json | msgspec | 0.080 | 0.082 | 0.096 | 70.156 | 0.80x |
| mixed.json | ujson | 0.244 | 0.245 | 0.248 | 70.156 | 0.27x |
| mixed.json | json | 0.488 | 0.492 | 0.496 | 70.156 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.116 | 9.272 | 10.004 | 70.160 | 1.00x |
| users.json | orjson | 11.897 | 12.099 | 12.717 | 70.160 | 0.77x |
| users.json | msgspec | 12.541 | 12.656 | 12.895 | 70.160 | 0.73x |
| users.json | ujson | 17.088 | 17.475 | 18.871 | 70.160 | 0.53x |
| users.json | json | 20.887 | 21.181 | 21.502 | 70.160 | 0.44x |
| flat.json | strata | 0.870 | 0.894 | 0.914 | 68.527 | 1.00x |
| flat.json | orjson | 0.945 | 0.961 | 0.975 | 68.527 | 0.93x |
| flat.json | msgspec | 0.994 | 1.002 | 1.006 | 68.527 | 0.89x |
| flat.json | ujson | 1.546 | 1.560 | 1.573 | 68.527 | 0.57x |
| flat.json | json | 1.838 | 1.853 | 1.868 | 68.527 | 0.48x |
| nested.json | strata | 0.853 | 0.867 | 0.876 | 68.527 | 1.00x |
| nested.json | orjson | 0.932 | 0.965 | 0.991 | 68.527 | 0.90x |
| nested.json | msgspec | 1.073 | 1.086 | 1.102 | 68.527 | 0.80x |
| nested.json | ujson | 1.495 | 1.517 | 1.535 | 68.527 | 0.57x |
| nested.json | json | 2.028 | 2.037 | 2.053 | 68.527 | 0.43x |
| wide_arrays.json | strata | 3.923 | 3.950 | 3.967 | 70.156 | 1.00x |
| wide_arrays.json | orjson | 4.065 | 4.121 | 4.166 | 70.156 | 0.96x |
| wide_arrays.json | msgspec | 5.122 | 5.141 | 5.193 | 70.156 | 0.77x |
| wide_arrays.json | ujson | 6.636 | 6.681 | 6.743 | 70.156 | 0.59x |
| wide_arrays.json | json | 9.583 | 9.615 | 9.678 | 70.156 | 0.41x |
| mixed.json | strata | 0.218 | 0.224 | 0.241 | 70.156 | 1.00x |
| mixed.json | orjson | 0.278 | 0.289 | 0.313 | 70.156 | 0.78x |
| mixed.json | msgspec | 0.301 | 0.309 | 0.326 | 70.156 | 0.73x |
| mixed.json | ujson | 0.389 | 0.399 | 0.413 | 70.156 | 0.56x |
| mixed.json | json | 0.512 | 0.531 | 0.539 | 70.156 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.527 | 9.637 | 9.971 | 68.527 | 1.00x |
| users.ndjson | orjson | 14.773 | 14.900 | 15.054 | 68.527 | 0.65x |
| users.ndjson | msgspec | 15.118 | 15.297 | 15.505 | 68.527 | 0.63x |
| users.ndjson | ujson | 19.546 | 19.838 | 20.369 | 68.527 | 0.49x |
| users.ndjson | json | 26.076 | 26.349 | 26.880 | 68.527 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.580 | 2.628 | 2.773 | 70.160 | 1.00x |
| users.json | orjson | 3.299 | 3.366 | 3.455 | 70.160 | 0.78x |
| users.json | msgspec | 4.009 | 4.036 | 4.133 | 70.160 | 0.65x |
| users.json | ujson | 11.439 | 11.500 | 11.630 | 70.160 | 0.23x |
| users.json | json | 19.865 | 20.051 | 20.186 | 70.160 | 0.13x |
| flat.json | strata | 0.479 | 0.511 | 0.534 | 68.527 | 1.00x |
| flat.json | orjson | 0.585 | 0.601 | 0.638 | 68.527 | 0.85x |
| flat.json | msgspec | 0.667 | 0.697 | 0.748 | 68.527 | 0.73x |
| flat.json | ujson | 1.289 | 1.309 | 1.333 | 68.527 | 0.39x |
| flat.json | json | 2.004 | 2.042 | 2.084 | 68.527 | 0.25x |
| nested.json | strata | 0.466 | 0.475 | 0.508 | 68.527 | 1.00x |
| nested.json | orjson | 0.558 | 0.584 | 0.599 | 68.527 | 0.81x |
| nested.json | msgspec | 0.649 | 0.686 | 0.697 | 68.527 | 0.69x |
| nested.json | ujson | 1.395 | 1.416 | 1.482 | 68.527 | 0.34x |
| nested.json | json | 2.464 | 2.488 | 2.510 | 68.527 | 0.19x |
| wide_arrays.json | strata | 1.843 | 1.876 | 2.010 | 70.156 | 1.00x |
| wide_arrays.json | orjson | 2.109 | 2.170 | 2.224 | 70.156 | 0.86x |
| wide_arrays.json | msgspec | 2.930 | 2.978 | 2.999 | 70.156 | 0.63x |
| wide_arrays.json | ujson | 5.348 | 5.390 | 5.497 | 70.156 | 0.35x |
| wide_arrays.json | json | 14.205 | 14.260 | 14.404 | 70.156 | 0.13x |
| mixed.json | strata | 0.231 | 0.248 | 0.299 | 70.156 | 1.00x |
| mixed.json | orjson | 0.263 | 0.284 | 0.351 | 70.156 | 0.87x |
| mixed.json | msgspec | 0.295 | 0.323 | 0.358 | 70.156 | 0.77x |
| mixed.json | ujson | 0.481 | 0.499 | 0.539 | 70.156 | 0.50x |
| mixed.json | json | 0.724 | 0.732 | 0.756 | 70.156 | 0.34x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.110 | 0.115 | 0.120 | 70.160 | 1.00x |
| users.json $[*].id | jmespath | 0.488 | 0.502 | 0.519 | 70.160 | 0.23x |
| users.json $[*].id | jsonpath-ng | 2.503 | 2.540 | 2.577 | 70.160 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.639 | 0.655 | 0.666 | 70.164 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.986 | 3.045 | 3.068 | 70.164 | 0.22x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.934 | 18.125 | 18.284 | 70.164 | 0.04x |
| users.json $..total | strata | 1.710 | 1.730 | 1.746 | 70.164 | 1.00x |
| users.json $..total | jsonpath-ng | 293.180 | 293.657 | 294.011 | 70.164 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.186 | 3.214 | 3.247 | 70.164 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.752 | 13.235 | 13.449 | 70.164 | 0.24x |
| users.json $[*].id | orjson+jsonpath-ng | 14.508 | 15.065 | 15.334 | 70.164 | 0.21x |
| users.json $[*].orders[*].total | strata | 3.377 | 3.398 | 3.425 | 70.164 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.748 | 15.877 | 16.311 | 70.164 | 0.21x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 34.282 | 34.732 | 35.605 | 70.164 | 0.10x |
| users.json $..total | strata | 11.699 | 12.022 | 12.351 | 70.164 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 314.133 | 315.064 | 316.791 | 70.164 | 0.04x |

