# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 04160cb
- python: 3.12.13
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: x86_64
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.290 | 10.798 | 20.134 | 57.953 | 1.00x |
| users.json | orjson | 13.016 | 13.191 | 19.351 | 57.953 | 0.82x |
| users.json | msgspec | 13.036 | 13.151 | 16.342 | 57.953 | 0.82x |
| users.json | ujson | 18.240 | 20.309 | 27.153 | 57.953 | 0.53x |
| users.json | pysimdjson | 18.337 | 19.349 | 34.422 | 57.953 | 0.56x |
| users.json | json | 22.365 | 23.366 | 44.859 | 57.953 | 0.46x |
| flat.json | strata | 0.916 | 0.925 | 0.973 | 68.188 | 1.00x |
| flat.json | orjson | 1.000 | 1.014 | 1.050 | 68.188 | 0.91x |
| flat.json | msgspec | 1.006 | 1.014 | 1.474 | 68.188 | 0.91x |
| flat.json | ujson | 1.472 | 1.486 | 1.530 | 68.188 | 0.62x |
| flat.json | pysimdjson | 1.562 | 1.578 | 1.607 | 68.188 | 0.59x |
| flat.json | json | 1.902 | 1.924 | 1.937 | 68.188 | 0.48x |
| nested.json | strata | 0.828 | 0.851 | 0.890 | 68.188 | 1.00x |
| nested.json | orjson | 0.997 | 1.000 | 1.027 | 68.188 | 0.85x |
| nested.json | msgspec | 1.019 | 1.029 | 1.064 | 68.188 | 0.83x |
| nested.json | ujson | 1.465 | 1.499 | 1.707 | 68.188 | 0.57x |
| nested.json | pysimdjson | 1.397 | 1.408 | 1.602 | 68.188 | 0.60x |
| nested.json | json | 2.087 | 2.109 | 2.133 | 68.188 | 0.40x |
| wide_arrays.json | strata | 4.717 | 4.749 | 6.758 | 72.562 | 1.00x |
| wide_arrays.json | orjson | 4.997 | 5.067 | 5.236 | 72.562 | 0.94x |
| wide_arrays.json | msgspec | 5.556 | 5.597 | 5.795 | 72.562 | 0.85x |
| wide_arrays.json | ujson | 7.105 | 7.166 | 7.247 | 72.562 | 0.66x |
| wide_arrays.json | pysimdjson | 6.018 | 6.057 | 6.146 | 72.562 | 0.78x |
| wide_arrays.json | json | 9.764 | 9.853 | 10.699 | 72.562 | 0.48x |
| mixed.json | strata | 0.199 | 0.200 | 0.238 | 72.625 | 1.00x |
| mixed.json | orjson | 0.230 | 0.230 | 0.246 | 72.625 | 0.87x |
| mixed.json | msgspec | 0.241 | 0.252 | 0.259 | 72.625 | 0.79x |
| mixed.json | ujson | 0.309 | 0.321 | 0.458 | 72.625 | 0.62x |
| mixed.json | pysimdjson | 0.303 | 0.307 | 0.321 | 72.625 | 0.65x |
| mixed.json | json | 0.472 | 0.490 | 0.535 | 72.625 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.571 | 2.604 | 2.647 | 56.680 | 1.00x |
| users.json | orjson | 2.891 | 2.908 | 2.945 | 56.680 | 0.90x |
| users.json | msgspec | 3.827 | 3.856 | 3.873 | 56.680 | 0.68x |
| users.json | ujson | 14.168 | 14.362 | 15.650 | 56.680 | 0.18x |
| users.json | json | 21.694 | 21.864 | 22.370 | 56.680 | 0.12x |
| flat.json | strata | 0.253 | 0.255 | 0.280 | 68.188 | 1.00x |
| flat.json | orjson | 0.328 | 0.329 | 0.358 | 68.188 | 0.77x |
| flat.json | msgspec | 0.430 | 0.437 | 0.450 | 68.188 | 0.58x |
| flat.json | ujson | 1.223 | 1.246 | 1.267 | 68.188 | 0.20x |
| flat.json | json | 1.848 | 1.861 | 1.879 | 68.188 | 0.14x |
| nested.json | strata | 0.244 | 0.257 | 0.289 | 68.188 | 1.00x |
| nested.json | orjson | 0.291 | 0.296 | 0.305 | 68.188 | 0.87x |
| nested.json | msgspec | 0.403 | 0.409 | 0.421 | 68.188 | 0.63x |
| nested.json | ujson | 1.300 | 1.313 | 1.364 | 68.188 | 0.20x |
| nested.json | json | 2.422 | 2.451 | 2.503 | 68.188 | 0.10x |
| wide_arrays.json | strata | 1.634 | 1.646 | 1.796 | 72.562 | 1.00x |
| wide_arrays.json | orjson | 1.784 | 1.793 | 1.836 | 72.562 | 0.92x |
| wide_arrays.json | msgspec | 2.657 | 2.670 | 2.723 | 72.562 | 0.62x |
| wide_arrays.json | ujson | 8.516 | 8.546 | 8.601 | 72.562 | 0.19x |
| wide_arrays.json | json | 16.759 | 16.826 | 17.296 | 72.562 | 0.10x |
| mixed.json | strata | 0.064 | 0.065 | 0.078 | 72.625 | 1.00x |
| mixed.json | orjson | 0.064 | 0.065 | 0.066 | 72.625 | 1.00x |
| mixed.json | msgspec | 0.084 | 0.084 | 0.097 | 72.625 | 0.78x |
| mixed.json | ujson | 0.296 | 0.301 | 0.313 | 72.625 | 0.22x |
| mixed.json | json | 0.514 | 0.526 | 0.545 | 72.625 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.893 | 11.173 | 12.524 | 71.492 | 1.00x |
| users.json | orjson | 13.494 | 13.716 | 14.539 | 71.492 | 0.81x |
| users.json | msgspec | 13.508 | 13.818 | 14.104 | 71.492 | 0.81x |
| users.json | ujson | 18.931 | 19.443 | 21.199 | 71.492 | 0.57x |
| users.json | json | 22.540 | 22.863 | 23.255 | 71.492 | 0.49x |
| flat.json | strata | 0.943 | 0.953 | 1.027 | 68.188 | 1.00x |
| flat.json | orjson | 1.056 | 1.070 | 1.093 | 68.188 | 0.89x |
| flat.json | msgspec | 1.059 | 1.069 | 1.102 | 68.188 | 0.89x |
| flat.json | ujson | 1.579 | 1.601 | 1.777 | 68.188 | 0.59x |
| flat.json | json | 1.943 | 1.957 | 2.080 | 68.188 | 0.49x |
| nested.json | strata | 0.873 | 0.887 | 0.913 | 68.188 | 1.00x |
| nested.json | orjson | 1.053 | 1.063 | 1.466 | 68.188 | 0.83x |
| nested.json | msgspec | 1.081 | 1.089 | 1.249 | 68.188 | 0.81x |
| nested.json | ujson | 1.535 | 1.558 | 1.606 | 68.188 | 0.57x |
| nested.json | json | 2.140 | 2.164 | 2.217 | 68.188 | 0.41x |
| wide_arrays.json | strata | 4.862 | 4.913 | 4.985 | 72.625 | 1.00x |
| wide_arrays.json | orjson | 5.033 | 5.072 | 5.095 | 72.625 | 0.97x |
| wide_arrays.json | msgspec | 5.697 | 5.752 | 5.834 | 72.625 | 0.85x |
| wide_arrays.json | ujson | 7.352 | 7.429 | 7.477 | 72.625 | 0.66x |
| wide_arrays.json | json | 9.869 | 9.956 | 10.168 | 72.625 | 0.49x |
| mixed.json | strata | 0.222 | 0.223 | 0.241 | 72.625 | 1.00x |
| mixed.json | orjson | 0.271 | 0.272 | 0.301 | 72.625 | 0.82x |
| mixed.json | msgspec | 0.286 | 0.292 | 0.311 | 72.625 | 0.76x |
| mixed.json | ujson | 0.363 | 0.369 | 0.393 | 72.625 | 0.60x |
| mixed.json | json | 0.517 | 0.526 | 0.548 | 72.625 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.683 | 10.889 | 12.200 | 68.188 | 1.00x |
| users.ndjson | orjson | 16.455 | 16.717 | 19.000 | 68.188 | 0.65x |
| users.ndjson | msgspec | 16.304 | 16.579 | 17.686 | 68.188 | 0.66x |
| users.ndjson | ujson | 21.731 | 22.381 | 24.414 | 68.188 | 0.49x |
| users.ndjson | json | 28.781 | 29.079 | 30.838 | 68.188 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.204 | 3.246 | 3.463 | 67.555 | 1.00x |
| users.json | orjson | 3.488 | 3.542 | 3.598 | 67.555 | 0.92x |
| users.json | msgspec | 4.430 | 4.459 | 4.562 | 67.555 | 0.73x |
| users.json | ujson | 14.991 | 15.114 | 15.443 | 67.555 | 0.21x |
| users.json | json | 22.630 | 22.694 | 23.164 | 67.555 | 0.14x |
| flat.json | strata | 0.393 | 0.397 | 0.439 | 68.188 | 1.00x |
| flat.json | orjson | 0.483 | 0.499 | 0.525 | 68.188 | 0.80x |
| flat.json | msgspec | 0.587 | 0.605 | 0.632 | 68.188 | 0.66x |
| flat.json | ujson | 1.412 | 1.433 | 1.449 | 68.188 | 0.28x |
| flat.json | json | 2.030 | 2.063 | 2.105 | 68.188 | 0.19x |
| nested.json | strata | 0.357 | 0.371 | 0.427 | 68.188 | 1.00x |
| nested.json | orjson | 0.423 | 0.426 | 0.486 | 68.188 | 0.87x |
| nested.json | msgspec | 0.535 | 0.556 | 0.581 | 68.188 | 0.67x |
| nested.json | ujson | 1.447 | 1.472 | 1.495 | 68.188 | 0.25x |
| nested.json | json | 2.574 | 2.615 | 2.764 | 68.188 | 0.14x |
| wide_arrays.json | strata | 2.032 | 2.057 | 2.091 | 72.625 | 1.00x |
| wide_arrays.json | orjson | 2.233 | 2.243 | 2.278 | 72.625 | 0.92x |
| wide_arrays.json | msgspec | 3.123 | 3.157 | 3.301 | 72.625 | 0.65x |
| wide_arrays.json | ujson | 9.119 | 9.157 | 9.282 | 72.625 | 0.22x |
| wide_arrays.json | json | 17.313 | 17.368 | 17.854 | 72.625 | 0.12x |
| mixed.json | strata | 0.152 | 0.154 | 0.188 | 72.625 | 1.00x |
| mixed.json | orjson | 0.169 | 0.171 | 0.192 | 72.625 | 0.90x |
| mixed.json | msgspec | 0.183 | 0.190 | 0.209 | 72.625 | 0.81x |
| mixed.json | ujson | 0.399 | 0.423 | 0.453 | 72.625 | 0.36x |
| mixed.json | json | 0.622 | 0.639 | 0.691 | 72.625 | 0.24x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.062 | 0.063 | 0.065 | 67.555 | 1.00x |
| users.json $[*].id | jmespath | 0.471 | 0.482 | 0.487 | 67.555 | 0.13x |
| users.json $[*].id | jsonpath-ng | 2.742 | 2.756 | 2.936 | 67.555 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.433 | 0.454 | 0.460 | 68.660 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.997 | 3.020 | 3.314 | 68.660 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 18.783 | 19.126 | 19.415 | 68.660 | 0.02x |
| users.json $..total | strata | 1.652 | 1.691 | 1.768 | 72.770 | 1.00x |
| users.json $..total | jsonpath-ng | 381.671 | 383.602 | 388.938 | 72.770 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.247 | 3.271 | 3.335 | 68.660 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.110 | 14.201 | 14.973 | 68.660 | 0.23x |
| users.json $[*].id | orjson+jsonpath-ng | 16.474 | 16.674 | 17.652 | 68.660 | 0.20x |
| users.json $[*].orders[*].total | strata | 3.481 | 3.527 | 3.561 | 71.988 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 16.860 | 17.086 | 17.516 | 71.988 | 0.21x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 36.063 | 36.790 | 38.309 | 71.988 | 0.10x |
| users.json $..total | strata | 13.544 | 13.697 | 14.406 | 72.008 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 404.831 | 405.474 | 456.971 | 72.008 | 0.03x |

