# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 79fa3df
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
| users.json | strata | 8.490 | 8.529 | 10.454 | 63.004 | 1.00x |
| users.json | orjson | 12.034 | 12.115 | 13.365 | 63.004 | 0.70x |
| users.json | msgspec | 12.491 | 12.558 | 13.656 | 63.004 | 0.68x |
| users.json | ujson | 15.599 | 15.740 | 17.859 | 63.004 | 0.54x |
| users.json | pysimdjson | 15.858 | 16.012 | 17.608 | 63.004 | 0.53x |
| users.json | json | 18.866 | 18.997 | 19.462 | 63.004 | 0.45x |
| flat.json | strata | 0.756 | 0.773 | 0.785 | 74.531 | 1.00x |
| flat.json | orjson | 0.905 | 0.932 | 0.970 | 74.531 | 0.83x |
| flat.json | msgspec | 0.865 | 0.888 | 0.919 | 74.531 | 0.87x |
| flat.json | ujson | 1.218 | 1.228 | 1.249 | 74.531 | 0.63x |
| flat.json | pysimdjson | 1.354 | 1.362 | 1.383 | 74.531 | 0.57x |
| flat.json | json | 1.555 | 1.653 | 1.693 | 74.531 | 0.47x |
| nested.json | strata | 0.629 | 0.647 | 0.670 | 74.531 | 1.00x |
| nested.json | orjson | 0.798 | 0.810 | 0.819 | 74.531 | 0.80x |
| nested.json | msgspec | 0.817 | 0.832 | 0.858 | 74.531 | 0.78x |
| nested.json | ujson | 1.159 | 1.171 | 1.184 | 74.531 | 0.55x |
| nested.json | pysimdjson | 1.128 | 1.138 | 1.183 | 74.531 | 0.57x |
| nested.json | json | 1.637 | 1.641 | 1.652 | 74.531 | 0.39x |
| wide_arrays.json | strata | 3.288 | 3.317 | 3.339 | 78.535 | 1.00x |
| wide_arrays.json | orjson | 4.328 | 4.485 | 4.610 | 78.535 | 0.74x |
| wide_arrays.json | msgspec | 4.730 | 4.756 | 4.805 | 78.535 | 0.70x |
| wide_arrays.json | ujson | 5.780 | 5.824 | 5.869 | 78.535 | 0.57x |
| wide_arrays.json | pysimdjson | 4.728 | 4.806 | 4.853 | 78.535 | 0.69x |
| wide_arrays.json | json | 8.010 | 8.074 | 8.123 | 78.535 | 0.41x |
| mixed.json | strata | 0.168 | 0.171 | 0.185 | 78.598 | 1.00x |
| mixed.json | orjson | 0.199 | 0.208 | 0.216 | 78.598 | 0.82x |
| mixed.json | msgspec | 0.212 | 0.215 | 0.226 | 78.598 | 0.80x |
| mixed.json | ujson | 0.262 | 0.270 | 0.280 | 78.598 | 0.63x |
| mixed.json | pysimdjson | 0.258 | 0.263 | 0.279 | 78.598 | 0.65x |
| mixed.json | json | 0.428 | 0.531 | 0.574 | 78.598 | 0.32x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.755 | 1.791 | 1.800 | 63.645 | 1.00x |
| users.json | orjson | 2.016 | 2.064 | 2.135 | 63.645 | 0.87x |
| users.json | msgspec | 3.363 | 3.407 | 3.460 | 63.645 | 0.53x |
| users.json | ujson | 9.856 | 9.886 | 9.923 | 63.645 | 0.18x |
| users.json | json | 17.891 | 18.007 | 18.102 | 63.645 | 0.10x |
| flat.json | strata | 0.248 | 0.250 | 0.275 | 74.531 | 1.00x |
| flat.json | orjson | 0.242 | 0.246 | 0.259 | 74.531 | 1.02x |
| flat.json | msgspec | 0.368 | 0.384 | 0.402 | 74.531 | 0.65x |
| flat.json | ujson | 0.904 | 0.911 | 0.928 | 74.531 | 0.27x |
| flat.json | json | 1.599 | 1.617 | 1.635 | 74.531 | 0.15x |
| nested.json | strata | 0.183 | 0.186 | 0.198 | 74.531 | 1.00x |
| nested.json | orjson | 0.225 | 0.232 | 0.247 | 74.531 | 0.80x |
| nested.json | msgspec | 0.320 | 0.331 | 0.339 | 74.531 | 0.56x |
| nested.json | ujson | 0.951 | 0.959 | 0.965 | 74.531 | 0.19x |
| nested.json | json | 1.960 | 1.981 | 2.024 | 74.531 | 0.09x |
| wide_arrays.json | strata | 1.286 | 1.305 | 1.320 | 78.535 | 1.00x |
| wide_arrays.json | orjson | 1.244 | 1.253 | 1.306 | 78.535 | 1.04x |
| wide_arrays.json | msgspec | 2.204 | 2.212 | 2.218 | 78.535 | 0.59x |
| wide_arrays.json | ujson | 4.826 | 4.839 | 4.858 | 78.535 | 0.27x |
| wide_arrays.json | json | 12.950 | 13.017 | 13.310 | 78.535 | 0.10x |
| mixed.json | strata | 0.053 | 0.055 | 0.061 | 78.598 | 1.00x |
| mixed.json | orjson | 0.051 | 0.051 | 0.056 | 78.598 | 1.07x |
| mixed.json | msgspec | 0.066 | 0.068 | 0.080 | 78.598 | 0.80x |
| mixed.json | ujson | 0.204 | 0.206 | 0.216 | 78.598 | 0.27x |
| mixed.json | json | 0.470 | 0.502 | 0.510 | 78.598 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.667 | 8.729 | 9.448 | 79.555 | 1.00x |
| users.json | orjson | 12.151 | 12.233 | 12.622 | 79.555 | 0.71x |
| users.json | msgspec | 12.658 | 12.684 | 12.907 | 79.555 | 0.69x |
| users.json | ujson | 15.945 | 16.192 | 17.310 | 79.555 | 0.54x |
| users.json | json | 19.115 | 19.213 | 19.370 | 79.555 | 0.45x |
| flat.json | strata | 0.783 | 0.795 | 0.808 | 74.531 | 1.00x |
| flat.json | orjson | 0.967 | 0.999 | 1.107 | 74.531 | 0.80x |
| flat.json | msgspec | 0.945 | 0.966 | 0.983 | 74.531 | 0.82x |
| flat.json | ujson | 1.316 | 1.323 | 1.340 | 74.531 | 0.60x |
| flat.json | json | 1.628 | 1.699 | 1.761 | 74.531 | 0.47x |
| nested.json | strata | 0.654 | 0.663 | 0.685 | 74.531 | 1.00x |
| nested.json | orjson | 0.852 | 0.859 | 0.894 | 74.531 | 0.77x |
| nested.json | msgspec | 0.875 | 0.883 | 0.892 | 74.531 | 0.75x |
| nested.json | ujson | 1.243 | 1.251 | 1.315 | 74.531 | 0.53x |
| nested.json | json | 1.665 | 1.688 | 1.747 | 74.531 | 0.39x |
| wide_arrays.json | strata | 3.363 | 3.384 | 3.410 | 78.598 | 1.00x |
| wide_arrays.json | orjson | 4.378 | 4.573 | 4.613 | 78.598 | 0.74x |
| wide_arrays.json | msgspec | 4.827 | 4.883 | 4.890 | 78.598 | 0.69x |
| wide_arrays.json | ujson | 6.018 | 6.038 | 6.137 | 78.598 | 0.56x |
| wide_arrays.json | json | 8.152 | 8.192 | 8.277 | 78.598 | 0.41x |
| mixed.json | strata | 0.183 | 0.185 | 0.195 | 78.598 | 1.00x |
| mixed.json | orjson | 0.249 | 0.253 | 0.267 | 78.598 | 0.73x |
| mixed.json | msgspec | 0.263 | 0.264 | 0.396 | 78.598 | 0.70x |
| mixed.json | ujson | 0.332 | 0.341 | 0.348 | 78.598 | 0.54x |
| mixed.json | json | 0.455 | 0.559 | 0.596 | 78.598 | 0.33x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 8.891 | 8.933 | 9.040 | 74.531 | 1.00x |
| users.ndjson | orjson | 14.090 | 14.169 | 14.323 | 74.531 | 0.63x |
| users.ndjson | msgspec | 14.146 | 14.240 | 14.399 | 74.531 | 0.63x |
| users.ndjson | ujson | 17.593 | 17.745 | 17.864 | 74.531 | 0.50x |
| users.ndjson | json | 23.172 | 23.364 | 23.412 | 74.531 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.145 | 2.172 | 2.227 | 76.602 | 1.00x |
| users.json | orjson | 2.480 | 2.519 | 2.574 | 76.602 | 0.86x |
| users.json | msgspec | 3.755 | 3.815 | 3.868 | 76.602 | 0.57x |
| users.json | ujson | 10.364 | 10.453 | 10.530 | 76.602 | 0.21x |
| users.json | json | 18.479 | 18.541 | 18.637 | 76.602 | 0.12x |
| flat.json | strata | 0.342 | 0.346 | 0.356 | 74.531 | 1.00x |
| flat.json | orjson | 0.344 | 0.353 | 0.376 | 74.531 | 0.98x |
| flat.json | msgspec | 0.473 | 0.494 | 0.499 | 74.531 | 0.70x |
| flat.json | ujson | 1.017 | 1.029 | 1.041 | 74.531 | 0.34x |
| flat.json | json | 1.700 | 1.720 | 1.810 | 74.531 | 0.20x |
| nested.json | strata | 0.258 | 0.265 | 0.276 | 74.531 | 1.00x |
| nested.json | orjson | 0.316 | 0.331 | 0.347 | 74.531 | 0.80x |
| nested.json | msgspec | 0.404 | 0.420 | 0.480 | 74.531 | 0.63x |
| nested.json | ujson | 1.030 | 1.039 | 1.049 | 74.531 | 0.25x |
| nested.json | json | 2.073 | 2.081 | 2.129 | 74.531 | 0.13x |
| wide_arrays.json | strata | 1.555 | 1.571 | 1.601 | 78.598 | 1.00x |
| wide_arrays.json | orjson | 1.528 | 1.540 | 1.568 | 78.598 | 1.02x |
| wide_arrays.json | msgspec | 2.495 | 2.517 | 2.541 | 78.598 | 0.62x |
| wide_arrays.json | ujson | 5.182 | 5.243 | 5.261 | 78.598 | 0.30x |
| wide_arrays.json | json | 13.323 | 13.378 | 81.436 | 78.598 | 0.12x |
| mixed.json | strata | 0.111 | 0.119 | 98.331 | 78.598 | 1.00x |
| mixed.json | orjson | 0.121 | 0.125 | 0.137 | 78.598 | 0.95x |
| mixed.json | msgspec | 0.133 | 0.140 | 0.154 | 78.598 | 0.85x |
| mixed.json | ujson | 0.284 | 0.289 | 55.338 | 78.598 | 0.41x |
| mixed.json | json | 0.538 | 0.567 | 0.657 | 78.598 | 0.21x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.056 | 0.057 | 0.069 | 76.605 | 1.00x |
| users.json $[*].id | jmespath | 0.420 | 0.432 | 0.451 | 76.605 | 0.13x |
| users.json $[*].id | jsonpath-ng | 1.995 | 2.022 | 2.084 | 76.605 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.417 | 0.423 | 0.454 | 76.609 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.583 | 2.610 | 2.647 | 76.609 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 13.760 | 13.823 | 13.954 | 76.609 | 0.03x |
| users.json $..total | strata | 1.558 | 1.565 | 1.601 | 78.574 | 1.00x |
| users.json $..total | jsonpath-ng | 266.041 | 266.604 | 269.189 | 78.574 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 2.547 | 2.613 | 2.726 | 76.609 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.642 | 12.689 | 12.763 | 76.609 | 0.21x |
| users.json $[*].id | orjson+jsonpath-ng | 14.270 | 14.328 | 14.384 | 76.609 | 0.18x |
| users.json $[*].orders[*].total | strata | 2.614 | 2.771 | 2.851 | 77.809 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 14.855 | 14.920 | 14.961 | 77.809 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 30.888 | 31.074 | 31.252 | 77.809 | 0.09x |
| users.json $..total | strata | 10.449 | 10.540 | 10.588 | 78.371 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 285.718 | 287.750 | 289.576 | 78.371 | 0.04x |

