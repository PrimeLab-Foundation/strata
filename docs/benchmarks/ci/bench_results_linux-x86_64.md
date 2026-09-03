# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 2b2f55a
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
| users.json | strata | 11.550 | 11.902 | 17.170 | 58.188 | 1.00x |
| users.json | orjson | 14.036 | 14.627 | 18.520 | 58.188 | 0.81x |
| users.json | msgspec | 13.467 | 14.100 | 18.536 | 58.188 | 0.84x |
| users.json | ujson | 18.618 | 21.561 | 27.358 | 58.188 | 0.55x |
| users.json | pysimdjson | 20.283 | 22.187 | 26.632 | 58.188 | 0.54x |
| users.json | json | 23.024 | 23.745 | 25.258 | 58.188 | 0.50x |
| flat.json | strata | 0.865 | 0.884 | 0.992 | 69.746 | 1.00x |
| flat.json | orjson | 0.964 | 1.006 | 1.115 | 69.746 | 0.88x |
| flat.json | msgspec | 1.019 | 1.041 | 1.097 | 69.746 | 0.85x |
| flat.json | ujson | 1.630 | 1.744 | 1.858 | 69.746 | 0.51x |
| flat.json | pysimdjson | 1.556 | 1.637 | 1.777 | 69.746 | 0.54x |
| flat.json | json | 1.934 | 1.973 | 2.030 | 69.746 | 0.45x |
| nested.json | strata | 0.833 | 0.861 | 1.080 | 69.746 | 1.00x |
| nested.json | orjson | 1.027 | 1.070 | 1.122 | 69.746 | 0.80x |
| nested.json | msgspec | 1.039 | 1.053 | 1.170 | 69.746 | 0.82x |
| nested.json | ujson | 1.610 | 1.671 | 1.860 | 69.746 | 0.52x |
| nested.json | pysimdjson | 1.422 | 1.482 | 1.939 | 69.746 | 0.58x |
| nested.json | json | 2.104 | 2.150 | 2.222 | 69.746 | 0.40x |
| wide_arrays.json | strata | 4.767 | 4.948 | 5.091 | 73.785 | 1.00x |
| wide_arrays.json | orjson | 5.087 | 5.333 | 5.854 | 73.785 | 0.93x |
| wide_arrays.json | msgspec | 5.640 | 5.751 | 6.276 | 73.785 | 0.86x |
| wide_arrays.json | ujson | 7.221 | 7.294 | 7.711 | 73.785 | 0.68x |
| wide_arrays.json | pysimdjson | 6.012 | 6.221 | 6.677 | 73.785 | 0.80x |
| wide_arrays.json | json | 9.744 | 9.893 | 10.258 | 73.785 | 0.50x |
| mixed.json | strata | 0.204 | 0.215 | 0.237 | 73.848 | 1.00x |
| mixed.json | orjson | 0.233 | 0.250 | 0.270 | 73.848 | 0.86x |
| mixed.json | msgspec | 0.248 | 0.265 | 0.270 | 73.848 | 0.81x |
| mixed.json | ujson | 0.328 | 0.364 | 0.385 | 73.848 | 0.59x |
| mixed.json | pysimdjson | 0.307 | 0.328 | 0.359 | 73.848 | 0.65x |
| mixed.json | json | 0.494 | 0.511 | 0.532 | 73.848 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.409 | 2.453 | 2.495 | 56.930 | 1.00x |
| users.json | orjson | 2.970 | 3.010 | 3.175 | 56.930 | 0.82x |
| users.json | msgspec | 3.896 | 3.957 | 3.987 | 56.930 | 0.62x |
| users.json | ujson | 14.809 | 15.249 | 15.362 | 56.930 | 0.16x |
| users.json | json | 22.322 | 22.660 | 23.395 | 56.930 | 0.11x |
| flat.json | strata | 0.268 | 0.279 | 0.305 | 69.746 | 1.00x |
| flat.json | orjson | 0.336 | 0.349 | 0.410 | 69.746 | 0.80x |
| flat.json | msgspec | 0.445 | 0.461 | 0.489 | 69.746 | 0.61x |
| flat.json | ujson | 1.309 | 1.326 | 1.365 | 69.746 | 0.21x |
| flat.json | json | 1.890 | 1.935 | 1.974 | 69.746 | 0.14x |
| nested.json | strata | 0.253 | 0.271 | 0.280 | 69.746 | 1.00x |
| nested.json | orjson | 0.300 | 0.303 | 0.334 | 69.746 | 0.89x |
| nested.json | msgspec | 0.410 | 0.423 | 0.431 | 69.746 | 0.64x |
| nested.json | ujson | 1.409 | 1.424 | 1.480 | 69.746 | 0.19x |
| nested.json | json | 2.439 | 2.452 | 2.495 | 69.746 | 0.11x |
| wide_arrays.json | strata | 1.490 | 1.498 | 1.906 | 73.785 | 1.00x |
| wide_arrays.json | orjson | 1.773 | 1.828 | 1.870 | 73.785 | 0.82x |
| wide_arrays.json | msgspec | 2.686 | 2.733 | 2.850 | 73.785 | 0.55x |
| wide_arrays.json | ujson | 8.611 | 8.675 | 10.146 | 73.785 | 0.17x |
| wide_arrays.json | json | 16.405 | 16.895 | 17.314 | 73.785 | 0.09x |
| mixed.json | strata | 0.064 | 0.068 | 0.084 | 73.848 | 1.00x |
| mixed.json | orjson | 0.068 | 0.070 | 0.082 | 73.848 | 0.97x |
| mixed.json | msgspec | 0.089 | 0.092 | 0.105 | 73.848 | 0.73x |
| mixed.json | ujson | 0.305 | 0.312 | 0.327 | 73.848 | 0.22x |
| mixed.json | json | 0.525 | 0.537 | 0.770 | 73.848 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 12.069 | 12.908 | 13.567 | 75.332 | 1.00x |
| users.json | orjson | 14.527 | 15.012 | 16.007 | 75.332 | 0.86x |
| users.json | msgspec | 14.582 | 15.082 | 16.157 | 75.332 | 0.86x |
| users.json | ujson | 20.613 | 21.895 | 23.716 | 75.332 | 0.59x |
| users.json | json | 24.005 | 24.923 | 26.094 | 75.332 | 0.52x |
| flat.json | strata | 0.910 | 0.939 | 0.996 | 69.746 | 1.00x |
| flat.json | orjson | 1.136 | 1.150 | 1.191 | 69.746 | 0.82x |
| flat.json | msgspec | 1.111 | 1.129 | 1.187 | 69.746 | 0.83x |
| flat.json | ujson | 1.661 | 1.752 | 1.891 | 69.746 | 0.54x |
| flat.json | json | 1.983 | 2.024 | 2.279 | 69.746 | 0.46x |
| nested.json | strata | 0.856 | 0.918 | 1.067 | 69.746 | 1.00x |
| nested.json | orjson | 1.098 | 1.144 | 1.203 | 69.746 | 0.80x |
| nested.json | msgspec | 1.107 | 1.151 | 1.351 | 69.746 | 0.80x |
| nested.json | ujson | 1.613 | 1.660 | 1.818 | 69.746 | 0.55x |
| nested.json | json | 2.154 | 2.234 | 2.342 | 69.746 | 0.41x |
| wide_arrays.json | strata | 4.954 | 5.323 | 5.529 | 73.848 | 1.00x |
| wide_arrays.json | orjson | 5.182 | 5.682 | 5.932 | 73.848 | 0.94x |
| wide_arrays.json | msgspec | 5.916 | 6.280 | 6.571 | 73.848 | 0.85x |
| wide_arrays.json | ujson | 7.449 | 7.999 | 8.413 | 73.848 | 0.67x |
| wide_arrays.json | json | 9.873 | 10.459 | 10.767 | 73.848 | 0.51x |
| mixed.json | strata | 0.227 | 0.237 | 0.252 | 73.848 | 1.00x |
| mixed.json | orjson | 0.287 | 0.312 | 0.331 | 73.848 | 0.76x |
| mixed.json | msgspec | 0.290 | 0.306 | 0.357 | 73.848 | 0.77x |
| mixed.json | ujson | 0.390 | 0.426 | 0.484 | 73.848 | 0.56x |
| mixed.json | json | 0.532 | 0.547 | 0.574 | 73.848 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 13.711 | 14.770 | 16.032 | 69.746 | 1.00x |
| users.ndjson | orjson | 19.220 | 20.357 | 23.279 | 69.746 | 0.73x |
| users.ndjson | msgspec | 19.233 | 20.805 | 23.220 | 69.746 | 0.71x |
| users.ndjson | ujson | 23.343 | 27.930 | 29.783 | 69.746 | 0.53x |
| users.ndjson | json | 33.224 | 35.349 | 38.955 | 69.746 | 0.42x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.046 | 3.150 | 3.703 | 71.332 | 1.00x |
| users.json | orjson | 3.771 | 3.878 | 4.094 | 71.332 | 0.81x |
| users.json | msgspec | 4.604 | 4.807 | 4.953 | 71.332 | 0.66x |
| users.json | ujson | 15.830 | 16.352 | 17.470 | 71.332 | 0.19x |
| users.json | json | 23.211 | 23.836 | 24.307 | 71.332 | 0.13x |
| flat.json | strata | 0.411 | 0.464 | 0.546 | 69.746 | 1.00x |
| flat.json | orjson | 0.511 | 0.560 | 0.629 | 69.746 | 0.83x |
| flat.json | msgspec | 0.608 | 0.660 | 0.802 | 69.746 | 0.70x |
| flat.json | ujson | 1.508 | 1.551 | 1.637 | 69.746 | 0.30x |
| flat.json | json | 2.092 | 2.147 | 2.263 | 69.746 | 0.22x |
| nested.json | strata | 0.371 | 0.403 | 0.428 | 69.746 | 1.00x |
| nested.json | orjson | 0.435 | 0.455 | 0.646 | 69.746 | 0.89x |
| nested.json | msgspec | 0.547 | 0.562 | 0.599 | 69.746 | 0.72x |
| nested.json | ujson | 1.565 | 1.587 | 1.667 | 69.746 | 0.25x |
| nested.json | json | 2.589 | 2.627 | 2.675 | 69.746 | 0.15x |
| wide_arrays.json | strata | 1.986 | 2.044 | 2.199 | 73.848 | 1.00x |
| wide_arrays.json | orjson | 2.327 | 2.362 | 2.568 | 73.848 | 0.87x |
| wide_arrays.json | msgspec | 3.222 | 3.272 | 3.447 | 73.848 | 0.62x |
| wide_arrays.json | ujson | 9.299 | 9.450 | 9.775 | 73.848 | 0.22x |
| wide_arrays.json | json | 17.271 | 17.625 | 18.050 | 73.848 | 0.12x |
| mixed.json | strata | 0.172 | 0.183 | 0.215 | 73.848 | 1.00x |
| mixed.json | orjson | 0.191 | 0.199 | 0.225 | 73.848 | 0.92x |
| mixed.json | msgspec | 0.208 | 0.216 | 0.264 | 73.848 | 0.85x |
| mixed.json | ujson | 0.443 | 0.469 | 0.500 | 73.848 | 0.39x |
| mixed.json | json | 0.661 | 0.693 | 0.714 | 73.848 | 0.26x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.073 | 0.078 | 0.094 | 71.332 | 1.00x |
| users.json $[*].id | jmespath | 0.507 | 0.524 | 0.560 | 71.332 | 0.15x |
| users.json $[*].id | jsonpath-ng | 2.955 | 3.227 | 3.376 | 71.332 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.459 | 0.503 | 0.630 | 71.336 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.241 | 3.403 | 4.057 | 71.336 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 22.046 | 23.367 | 25.142 | 71.336 | 0.02x |
| users.json $..total | strata | 1.689 | 1.979 | 2.397 | 73.164 | 1.00x |
| users.json $..total | jsonpath-ng | 395.420 | 399.838 | 402.762 | 73.164 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.175 | 3.214 | 3.324 | 71.336 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.861 | 15.709 | 17.626 | 71.336 | 0.20x |
| users.json $[*].id | orjson+jsonpath-ng | 17.391 | 18.661 | 20.520 | 71.336 | 0.17x |
| users.json $[*].orders[*].total | strata | 3.411 | 3.482 | 3.547 | 73.164 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.103 | 20.126 | 22.118 | 73.164 | 0.17x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 40.405 | 42.893 | 47.407 | 73.164 | 0.08x |
| users.json $..total | strata | 14.968 | 16.221 | 18.528 | 73.180 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 420.494 | 425.831 | 431.503 | 73.180 | 0.04x |

