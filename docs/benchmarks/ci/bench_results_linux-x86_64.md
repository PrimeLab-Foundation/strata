# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 59554e7
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
| users.json | strata | 10.703 | 11.646 | 16.765 | 58.332 | 1.00x |
| users.json | orjson | 13.687 | 14.268 | 20.037 | 58.332 | 0.82x |
| users.json | msgspec | 13.281 | 13.680 | 16.068 | 58.332 | 0.85x |
| users.json | ujson | 18.707 | 20.293 | 25.791 | 58.332 | 0.57x |
| users.json | pysimdjson | 20.291 | 22.293 | 30.296 | 58.332 | 0.52x |
| users.json | json | 22.792 | 23.408 | 32.645 | 58.332 | 0.50x |
| flat.json | strata | 0.880 | 0.890 | 0.903 | 69.191 | 1.00x |
| flat.json | orjson | 0.996 | 1.001 | 1.014 | 69.191 | 0.89x |
| flat.json | msgspec | 1.008 | 1.012 | 1.109 | 69.191 | 0.88x |
| flat.json | ujson | 1.455 | 1.467 | 1.519 | 69.191 | 0.61x |
| flat.json | pysimdjson | 1.547 | 1.565 | 1.587 | 69.191 | 0.57x |
| flat.json | json | 1.910 | 1.920 | 1.952 | 69.191 | 0.46x |
| nested.json | strata | 0.802 | 0.814 | 0.870 | 69.191 | 1.00x |
| nested.json | orjson | 1.011 | 1.015 | 1.029 | 69.191 | 0.80x |
| nested.json | msgspec | 1.028 | 1.042 | 1.095 | 69.191 | 0.78x |
| nested.json | ujson | 1.468 | 1.482 | 1.536 | 69.191 | 0.55x |
| nested.json | pysimdjson | 1.402 | 1.418 | 1.436 | 69.191 | 0.57x |
| nested.json | json | 2.033 | 2.055 | 2.103 | 69.191 | 0.40x |
| wide_arrays.json | strata | 4.578 | 4.651 | 4.723 | 75.191 | 1.00x |
| wide_arrays.json | orjson | 5.092 | 5.150 | 5.391 | 75.191 | 0.90x |
| wide_arrays.json | msgspec | 5.599 | 5.665 | 6.101 | 75.191 | 0.82x |
| wide_arrays.json | ujson | 7.206 | 7.341 | 7.535 | 75.191 | 0.63x |
| wide_arrays.json | pysimdjson | 6.005 | 6.114 | 6.424 | 75.191 | 0.76x |
| wide_arrays.json | json | 9.685 | 9.799 | 10.056 | 75.191 | 0.47x |
| mixed.json | strata | 0.195 | 0.324 | 0.360 | 75.316 | 1.00x |
| mixed.json | orjson | 0.231 | 0.351 | 0.409 | 75.316 | 0.92x |
| mixed.json | msgspec | 0.238 | 0.336 | 0.477 | 75.316 | 0.96x |
| mixed.json | ujson | 0.301 | 0.490 | 0.544 | 75.316 | 0.66x |
| mixed.json | pysimdjson | 0.302 | 0.477 | 0.507 | 75.316 | 0.68x |
| mixed.json | json | 0.476 | 0.791 | 0.813 | 75.316 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.511 | 2.562 | 2.595 | 57.305 | 1.00x |
| users.json | orjson | 2.902 | 2.949 | 2.997 | 57.305 | 0.87x |
| users.json | msgspec | 3.856 | 3.902 | 4.021 | 57.305 | 0.66x |
| users.json | ujson | 14.776 | 14.959 | 15.140 | 57.305 | 0.17x |
| users.json | json | 21.651 | 22.115 | 22.565 | 57.305 | 0.12x |
| flat.json | strata | 0.263 | 0.264 | 0.288 | 69.191 | 1.00x |
| flat.json | orjson | 0.328 | 0.330 | 0.350 | 69.191 | 0.80x |
| flat.json | msgspec | 0.428 | 0.437 | 0.457 | 69.191 | 0.60x |
| flat.json | ujson | 1.264 | 1.284 | 2.134 | 69.191 | 0.21x |
| flat.json | json | 1.829 | 1.851 | 1.875 | 69.191 | 0.14x |
| nested.json | strata | 0.256 | 0.260 | 0.282 | 69.191 | 1.00x |
| nested.json | orjson | 0.291 | 0.304 | 0.317 | 69.191 | 0.86x |
| nested.json | msgspec | 0.405 | 0.408 | 0.428 | 69.191 | 0.64x |
| nested.json | ujson | 1.381 | 1.398 | 1.421 | 69.191 | 0.19x |
| nested.json | json | 2.400 | 2.413 | 2.445 | 69.191 | 0.11x |
| wide_arrays.json | strata | 1.551 | 1.565 | 1.587 | 75.191 | 1.00x |
| wide_arrays.json | orjson | 1.801 | 1.810 | 1.901 | 75.191 | 0.86x |
| wide_arrays.json | msgspec | 2.685 | 2.692 | 2.750 | 75.191 | 0.58x |
| wide_arrays.json | ujson | 8.673 | 8.736 | 8.757 | 75.191 | 0.18x |
| wide_arrays.json | json | 16.390 | 16.442 | 16.950 | 75.191 | 0.10x |
| mixed.json | strata | 0.063 | 0.065 | 0.100 | 75.316 | 1.00x |
| mixed.json | orjson | 0.064 | 0.065 | 0.098 | 75.316 | 0.99x |
| mixed.json | msgspec | 0.083 | 0.085 | 0.097 | 75.316 | 0.76x |
| mixed.json | ujson | 0.299 | 0.306 | 0.318 | 75.316 | 0.21x |
| mixed.json | json | 0.506 | 0.527 | 0.533 | 75.316 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.250 | 11.585 | 13.133 | 69.902 | 1.00x |
| users.json | orjson | 13.871 | 14.358 | 16.157 | 69.902 | 0.81x |
| users.json | msgspec | 13.991 | 14.311 | 15.208 | 69.902 | 0.81x |
| users.json | ujson | 19.734 | 21.198 | 22.867 | 69.902 | 0.55x |
| users.json | json | 23.171 | 23.372 | 24.423 | 69.902 | 0.50x |
| flat.json | strata | 0.913 | 0.932 | 0.953 | 69.191 | 1.00x |
| flat.json | orjson | 1.061 | 1.068 | 1.079 | 69.191 | 0.87x |
| flat.json | msgspec | 1.072 | 1.080 | 1.089 | 69.191 | 0.86x |
| flat.json | ujson | 1.542 | 1.571 | 1.600 | 69.191 | 0.59x |
| flat.json | json | 1.981 | 1.990 | 2.034 | 69.191 | 0.47x |
| nested.json | strata | 0.827 | 0.841 | 1.639 | 69.191 | 1.00x |
| nested.json | orjson | 1.058 | 1.074 | 1.907 | 69.191 | 0.78x |
| nested.json | msgspec | 1.085 | 1.100 | 2.163 | 69.191 | 0.76x |
| nested.json | ujson | 1.553 | 1.578 | 3.165 | 69.191 | 0.53x |
| nested.json | json | 2.101 | 2.123 | 4.289 | 69.191 | 0.40x |
| wide_arrays.json | strata | 4.736 | 4.756 | 4.840 | 75.254 | 1.00x |
| wide_arrays.json | orjson | 5.118 | 5.236 | 5.337 | 75.254 | 0.91x |
| wide_arrays.json | msgspec | 5.712 | 5.824 | 6.207 | 75.254 | 0.82x |
| wide_arrays.json | ujson | 7.504 | 7.619 | 7.818 | 75.254 | 0.62x |
| wide_arrays.json | json | 9.808 | 9.895 | 10.294 | 75.254 | 0.48x |
| mixed.json | strata | 0.214 | 0.214 | 0.249 | 75.316 | 1.00x |
| mixed.json | orjson | 0.273 | 0.276 | 0.288 | 75.316 | 0.78x |
| mixed.json | msgspec | 0.281 | 0.293 | 0.344 | 75.316 | 0.73x |
| mixed.json | ujson | 0.361 | 0.371 | 0.384 | 75.316 | 0.58x |
| mixed.json | json | 0.523 | 0.538 | 0.551 | 75.316 | 0.40x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.478 | 10.776 | 12.595 | 69.191 | 1.00x |
| users.ndjson | orjson | 16.789 | 17.144 | 19.051 | 69.191 | 0.63x |
| users.ndjson | msgspec | 16.738 | 16.859 | 17.509 | 69.191 | 0.64x |
| users.ndjson | ujson | 21.634 | 21.890 | 24.066 | 69.191 | 0.49x |
| users.ndjson | json | 28.911 | 29.504 | 30.764 | 69.191 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.153 | 3.198 | 3.271 | 68.645 | 1.00x |
| users.json | orjson | 3.604 | 3.680 | 3.974 | 68.645 | 0.87x |
| users.json | msgspec | 4.522 | 4.592 | 5.000 | 68.645 | 0.70x |
| users.json | ujson | 15.527 | 15.688 | 17.007 | 68.645 | 0.20x |
| users.json | json | 22.499 | 22.796 | 24.097 | 68.645 | 0.14x |
| flat.json | strata | 0.403 | 0.414 | 0.426 | 69.191 | 1.00x |
| flat.json | orjson | 0.488 | 0.502 | 0.525 | 69.191 | 0.82x |
| flat.json | msgspec | 0.588 | 0.601 | 0.625 | 69.191 | 0.69x |
| flat.json | ujson | 1.453 | 1.478 | 1.501 | 69.191 | 0.28x |
| flat.json | json | 2.029 | 2.058 | 2.090 | 69.191 | 0.20x |
| nested.json | strata | 0.371 | 0.379 | 0.405 | 69.191 | 1.00x |
| nested.json | orjson | 0.423 | 0.428 | 0.459 | 69.191 | 0.89x |
| nested.json | msgspec | 0.540 | 0.552 | 0.581 | 69.191 | 0.69x |
| nested.json | ujson | 1.541 | 1.556 | 1.606 | 69.191 | 0.24x |
| nested.json | json | 2.504 | 2.541 | 2.631 | 69.191 | 0.15x |
| wide_arrays.json | strata | 1.989 | 2.019 | 2.960 | 75.254 | 1.00x |
| wide_arrays.json | orjson | 2.263 | 2.312 | 3.131 | 75.254 | 0.87x |
| wide_arrays.json | msgspec | 3.141 | 3.161 | 3.231 | 75.254 | 0.64x |
| wide_arrays.json | ujson | 9.226 | 9.327 | 13.109 | 75.254 | 0.22x |
| wide_arrays.json | json | 16.970 | 17.054 | 19.997 | 75.254 | 0.12x |
| mixed.json | strata | 0.145 | 0.157 | 0.172 | 75.316 | 1.00x |
| mixed.json | orjson | 0.171 | 0.173 | 0.557 | 75.316 | 0.90x |
| mixed.json | msgspec | 0.179 | 0.192 | 0.206 | 75.316 | 0.82x |
| mixed.json | ujson | 0.414 | 0.421 | 0.438 | 75.316 | 0.37x |
| mixed.json | json | 0.624 | 0.655 | 0.681 | 75.316 | 0.24x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.067 | 0.071 | 0.077 | 68.645 | 1.00x |
| users.json $[*].id | jmespath | 0.497 | 0.507 | 0.524 | 68.645 | 0.14x |
| users.json $[*].id | jsonpath-ng | 2.935 | 3.141 | 3.288 | 68.645 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.455 | 0.480 | 0.527 | 68.656 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.107 | 3.152 | 3.197 | 68.656 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 20.051 | 20.504 | 20.911 | 68.656 | 0.02x |
| users.json $..total | strata | 1.665 | 1.705 | 1.735 | 70.637 | 1.00x |
| users.json $..total | jsonpath-ng | 389.836 | 393.295 | 395.371 | 70.637 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.225 | 3.244 | 3.277 | 68.656 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.206 | 14.603 | 15.125 | 68.656 | 0.22x |
| users.json $[*].id | orjson+jsonpath-ng | 16.592 | 17.167 | 18.541 | 68.656 | 0.19x |
| users.json $[*].orders[*].total | strata | 3.477 | 3.505 | 3.639 | 69.855 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.473 | 17.895 | 19.545 | 69.855 | 0.20x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 38.478 | 40.590 | 42.934 | 69.855 | 0.09x |
| users.json $..total | strata | 13.468 | 14.191 | 17.624 | 73.012 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 412.693 | 416.092 | 419.705 | 73.012 | 0.03x |

