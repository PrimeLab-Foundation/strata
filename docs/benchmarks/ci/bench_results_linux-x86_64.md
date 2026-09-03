# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: f8c17d0
- python: 3.12.14
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
| users.json | strata | 10.070 | 10.435 | 15.223 | 60.219 | 1.00x |
| users.json | orjson | 13.842 | 14.452 | 16.436 | 60.219 | 0.72x |
| users.json | msgspec | 14.153 | 14.581 | 16.132 | 60.219 | 0.72x |
| users.json | ujson | 19.093 | 19.869 | 24.212 | 60.219 | 0.53x |
| users.json | pysimdjson | 18.276 | 19.623 | 30.171 | 60.219 | 0.53x |
| users.json | json | 21.847 | 22.760 | 39.999 | 60.219 | 0.46x |
| flat.json | strata | 0.841 | 0.904 | 0.966 | 69.781 | 1.00x |
| flat.json | orjson | 1.004 | 1.085 | 1.122 | 69.781 | 0.83x |
| flat.json | msgspec | 1.034 | 1.073 | 1.084 | 69.781 | 0.84x |
| flat.json | ujson | 1.485 | 1.501 | 1.514 | 69.781 | 0.60x |
| flat.json | pysimdjson | 1.606 | 1.663 | 1.726 | 69.781 | 0.54x |
| flat.json | json | 1.802 | 1.855 | 1.880 | 69.781 | 0.49x |
| nested.json | strata | 0.721 | 0.747 | 0.782 | 69.781 | 1.00x |
| nested.json | orjson | 0.879 | 0.909 | 0.954 | 69.781 | 0.82x |
| nested.json | msgspec | 0.915 | 0.943 | 0.961 | 69.781 | 0.79x |
| nested.json | ujson | 1.312 | 1.349 | 1.464 | 69.781 | 0.55x |
| nested.json | pysimdjson | 1.239 | 1.289 | 1.308 | 69.781 | 0.58x |
| nested.json | json | 1.792 | 1.830 | 1.873 | 69.781 | 0.41x |
| wide_arrays.json | strata | 4.187 | 4.398 | 4.470 | 73.781 | 1.00x |
| wide_arrays.json | orjson | 4.893 | 5.051 | 5.196 | 73.781 | 0.87x |
| wide_arrays.json | msgspec | 5.375 | 5.559 | 5.747 | 73.781 | 0.79x |
| wide_arrays.json | ujson | 6.902 | 7.169 | 7.245 | 73.781 | 0.61x |
| wide_arrays.json | pysimdjson | 5.416 | 5.618 | 5.776 | 73.781 | 0.78x |
| wide_arrays.json | json | 9.000 | 9.323 | 9.525 | 73.781 | 0.47x |
| mixed.json | strata | 0.193 | 0.201 | 0.236 | 73.844 | 1.00x |
| mixed.json | orjson | 0.230 | 0.245 | 0.280 | 73.844 | 0.82x |
| mixed.json | msgspec | 0.244 | 0.263 | 0.287 | 73.844 | 0.77x |
| mixed.json | ujson | 0.309 | 0.326 | 0.354 | 73.844 | 0.62x |
| mixed.json | pysimdjson | 0.302 | 0.313 | 0.351 | 73.844 | 0.64x |
| mixed.json | json | 0.470 | 0.478 | 0.514 | 73.844 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.173 | 2.191 | 2.233 | 59.125 | 1.00x |
| users.json | orjson | 2.284 | 2.327 | 3.148 | 59.125 | 0.94x |
| users.json | msgspec | 3.915 | 3.951 | 4.485 | 59.125 | 0.55x |
| users.json | ujson | 14.314 | 14.439 | 16.896 | 59.125 | 0.15x |
| users.json | json | 21.468 | 21.618 | 23.514 | 59.125 | 0.10x |
| flat.json | strata | 0.269 | 0.288 | 0.307 | 69.781 | 1.00x |
| flat.json | orjson | 0.280 | 0.287 | 0.303 | 69.781 | 1.00x |
| flat.json | msgspec | 0.436 | 0.452 | 0.497 | 69.781 | 0.64x |
| flat.json | ujson | 1.223 | 1.269 | 1.321 | 69.781 | 0.23x |
| flat.json | json | 1.798 | 1.898 | 1.947 | 69.781 | 0.15x |
| nested.json | strata | 0.179 | 0.185 | 0.198 | 69.781 | 1.00x |
| nested.json | orjson | 0.242 | 0.261 | 0.290 | 69.781 | 0.71x |
| nested.json | msgspec | 0.362 | 0.383 | 0.407 | 69.781 | 0.48x |
| nested.json | ujson | 1.237 | 1.264 | 1.323 | 69.781 | 0.15x |
| nested.json | json | 2.197 | 2.248 | 2.311 | 69.781 | 0.08x |
| wide_arrays.json | strata | 1.468 | 1.508 | 1.566 | 73.781 | 1.00x |
| wide_arrays.json | orjson | 1.367 | 1.406 | 1.458 | 73.781 | 1.07x |
| wide_arrays.json | msgspec | 2.519 | 2.598 | 2.720 | 73.781 | 0.58x |
| wide_arrays.json | ujson | 7.623 | 7.785 | 7.992 | 73.781 | 0.19x |
| wide_arrays.json | json | 14.631 | 15.087 | 15.442 | 73.781 | 0.10x |
| mixed.json | strata | 0.061 | 0.064 | 0.072 | 73.844 | 1.00x |
| mixed.json | orjson | 0.056 | 0.059 | 0.063 | 73.844 | 1.08x |
| mixed.json | msgspec | 0.079 | 0.083 | 0.092 | 73.844 | 0.77x |
| mixed.json | ujson | 0.296 | 0.307 | 0.352 | 73.844 | 0.21x |
| mixed.json | json | 0.524 | 0.534 | 0.551 | 73.844 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.442 | 10.811 | 12.364 | 75.344 | 1.00x |
| users.json | orjson | 14.547 | 14.774 | 16.861 | 75.344 | 0.73x |
| users.json | msgspec | 15.057 | 15.232 | 17.389 | 75.344 | 0.71x |
| users.json | ujson | 20.397 | 21.114 | 22.815 | 75.344 | 0.51x |
| users.json | json | 22.521 | 23.088 | 26.110 | 75.344 | 0.47x |
| flat.json | strata | 0.906 | 0.935 | 1.172 | 69.781 | 1.00x |
| flat.json | orjson | 1.112 | 1.162 | 1.989 | 69.781 | 0.81x |
| flat.json | msgspec | 1.102 | 1.143 | 1.932 | 69.781 | 0.82x |
| flat.json | ujson | 1.559 | 1.613 | 1.981 | 69.781 | 0.58x |
| flat.json | json | 1.854 | 1.906 | 2.195 | 69.781 | 0.49x |
| nested.json | strata | 0.751 | 0.782 | 0.832 | 69.781 | 1.00x |
| nested.json | orjson | 0.939 | 0.965 | 1.011 | 69.781 | 0.81x |
| nested.json | msgspec | 0.970 | 1.000 | 1.036 | 69.781 | 0.78x |
| nested.json | ujson | 1.393 | 1.433 | 1.443 | 69.781 | 0.55x |
| nested.json | json | 1.825 | 1.893 | 1.918 | 69.781 | 0.41x |
| wide_arrays.json | strata | 4.375 | 4.438 | 4.611 | 73.844 | 1.00x |
| wide_arrays.json | orjson | 4.914 | 5.032 | 5.140 | 73.844 | 0.88x |
| wide_arrays.json | msgspec | 5.577 | 5.718 | 5.858 | 73.844 | 0.78x |
| wide_arrays.json | ujson | 7.258 | 7.412 | 7.560 | 73.844 | 0.60x |
| wide_arrays.json | json | 9.235 | 9.605 | 10.170 | 73.844 | 0.46x |
| mixed.json | strata | 0.218 | 0.234 | 0.378 | 73.844 | 1.00x |
| mixed.json | orjson | 0.291 | 0.320 | 0.412 | 73.844 | 0.73x |
| mixed.json | msgspec | 0.306 | 0.330 | 0.537 | 73.844 | 0.71x |
| mixed.json | ujson | 0.389 | 0.429 | 0.602 | 73.844 | 0.55x |
| mixed.json | json | 0.521 | 0.555 | 0.684 | 73.844 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.354 | 10.834 | 11.146 | 69.781 | 1.00x |
| users.ndjson | orjson | 16.613 | 16.900 | 17.032 | 69.781 | 0.64x |
| users.ndjson | msgspec | 16.522 | 16.855 | 17.174 | 69.781 | 0.64x |
| users.ndjson | ujson | 21.151 | 21.679 | 22.002 | 69.781 | 0.50x |
| users.ndjson | json | 27.251 | 27.636 | 28.031 | 69.781 | 0.39x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.706 | 2.777 | 73.282 | 72.391 | 1.00x |
| users.json | orjson | 2.838 | 3.031 | 4.425 | 72.391 | 0.92x |
| users.json | msgspec | 4.441 | 4.563 | 82.664 | 72.391 | 0.61x |
| users.json | ujson | 15.008 | 15.182 | 48.302 | 72.391 | 0.18x |
| users.json | json | 21.694 | 23.879 | 25.467 | 72.391 | 0.12x |
| flat.json | strata | 0.393 | 0.432 | 44.396 | 69.781 | 1.00x |
| flat.json | orjson | 0.392 | 0.449 | 84.492 | 69.781 | 0.96x |
| flat.json | msgspec | 0.576 | 0.585 | 0.657 | 69.781 | 0.74x |
| flat.json | ujson | 1.363 | 1.425 | 9.683 | 69.781 | 0.30x |
| flat.json | json | 1.953 | 2.023 | 2.327 | 69.781 | 0.21x |
| nested.json | strata | 0.280 | 0.297 | 27.416 | 69.781 | 1.00x |
| nested.json | orjson | 0.364 | 0.387 | 8.304 | 69.781 | 0.77x |
| nested.json | msgspec | 0.476 | 0.500 | 25.523 | 69.781 | 0.59x |
| nested.json | ujson | 1.377 | 1.438 | 50.846 | 69.781 | 0.21x |
| nested.json | json | 2.387 | 2.421 | 2.487 | 69.781 | 0.12x |
| wide_arrays.json | strata | 1.863 | 1.963 | 23.311 | 73.844 | 1.00x |
| wide_arrays.json | orjson | 1.861 | 2.081 | 123.398 | 73.844 | 0.94x |
| wide_arrays.json | msgspec | 3.048 | 3.143 | 90.307 | 73.844 | 0.62x |
| wide_arrays.json | ujson | 8.398 | 8.499 | 50.021 | 73.844 | 0.23x |
| wide_arrays.json | json | 15.993 | 16.272 | 79.457 | 73.844 | 0.12x |
| mixed.json | strata | 0.124 | 0.131 | 0.146 | 73.844 | 1.00x |
| mixed.json | orjson | 0.135 | 0.140 | 0.147 | 73.844 | 0.93x |
| mixed.json | msgspec | 0.153 | 0.162 | 0.165 | 73.844 | 0.81x |
| mixed.json | ujson | 0.379 | 0.393 | 0.405 | 73.844 | 0.33x |
| mixed.json | json | 0.599 | 0.622 | 1.798 | 73.844 | 0.21x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.071 | 0.074 | 0.087 | 72.391 | 1.00x |
| users.json $[*].id | jmespath | 0.472 | 0.491 | 0.504 | 72.391 | 0.15x |
| users.json $[*].id | jsonpath-ng | 2.420 | 2.549 | 2.660 | 72.391 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.464 | 0.494 | 0.502 | 72.395 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.837 | 2.991 | 3.862 | 72.395 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 16.276 | 16.636 | 17.290 | 72.395 | 0.03x |
| users.json $..total | strata | 1.782 | 1.846 | 1.907 | 74.363 | 1.00x |
| users.json $..total | jsonpath-ng | 318.515 | 322.742 | 331.109 | 74.363 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.013 | 3.184 | 3.484 | 72.395 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.573 | 14.933 | 15.691 | 72.395 | 0.21x |
| users.json $[*].id | orjson+jsonpath-ng | 16.439 | 16.834 | 17.682 | 72.395 | 0.19x |
| users.json $[*].orders[*].total | strata | 3.322 | 3.466 | 3.545 | 73.566 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.777 | 17.852 | 18.251 | 73.566 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 36.875 | 37.202 | 42.457 | 73.566 | 0.09x |
| users.json $..total | strata | 13.966 | 14.649 | 15.011 | 73.582 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 338.578 | 342.264 | 346.006 | 73.582 | 0.04x |

