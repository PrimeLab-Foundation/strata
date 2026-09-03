# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: b294ccd
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
| users.json | strata | 10.950 | 11.845 | 17.801 | 59.273 | 1.00x |
| users.json | orjson | 14.855 | 16.291 | 19.529 | 59.273 | 0.73x |
| users.json | msgspec | 14.186 | 14.771 | 19.377 | 59.273 | 0.80x |
| users.json | ujson | 19.316 | 21.093 | 26.696 | 59.273 | 0.56x |
| users.json | pysimdjson | 21.689 | 22.531 | 30.318 | 59.273 | 0.53x |
| users.json | json | 21.939 | 22.981 | 39.292 | 59.273 | 0.52x |
| flat.json | strata | 0.870 | 0.879 | 1.042 | 69.070 | 1.00x |
| flat.json | orjson | 1.029 | 1.039 | 1.065 | 69.070 | 0.85x |
| flat.json | msgspec | 1.054 | 1.071 | 1.082 | 69.070 | 0.82x |
| flat.json | ujson | 1.535 | 1.554 | 1.572 | 69.070 | 0.57x |
| flat.json | pysimdjson | 1.667 | 1.688 | 1.718 | 69.070 | 0.52x |
| flat.json | json | 1.736 | 1.757 | 1.783 | 69.070 | 0.50x |
| nested.json | strata | 0.802 | 0.814 | 0.824 | 69.070 | 1.00x |
| nested.json | orjson | 1.014 | 1.020 | 1.055 | 69.070 | 0.80x |
| nested.json | msgspec | 0.972 | 0.991 | 1.010 | 69.070 | 0.82x |
| nested.json | ujson | 1.412 | 1.439 | 1.453 | 69.070 | 0.57x |
| nested.json | pysimdjson | 1.391 | 1.408 | 1.434 | 69.070 | 0.58x |
| nested.json | json | 1.846 | 1.874 | 1.925 | 69.070 | 0.43x |
| wide_arrays.json | strata | 4.959 | 5.001 | 5.099 | 71.965 | 1.00x |
| wide_arrays.json | orjson | 5.469 | 5.533 | 5.600 | 71.965 | 0.90x |
| wide_arrays.json | msgspec | 6.062 | 6.128 | 6.170 | 71.965 | 0.82x |
| wide_arrays.json | ujson | 7.696 | 7.778 | 7.966 | 71.965 | 0.64x |
| wide_arrays.json | pysimdjson | 6.317 | 6.382 | 6.495 | 71.965 | 0.78x |
| wide_arrays.json | json | 9.707 | 9.774 | 9.848 | 71.965 | 0.51x |
| mixed.json | strata | 0.197 | 0.201 | 0.217 | 72.027 | 1.00x |
| mixed.json | orjson | 0.236 | 0.238 | 0.254 | 72.027 | 0.84x |
| mixed.json | msgspec | 0.246 | 0.252 | 0.267 | 72.027 | 0.80x |
| mixed.json | ujson | 0.302 | 0.309 | 0.491 | 72.027 | 0.65x |
| mixed.json | pysimdjson | 0.294 | 0.305 | 0.340 | 72.027 | 0.66x |
| mixed.json | json | 0.445 | 0.455 | 0.464 | 72.027 | 0.44x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.575 | 2.589 | 2.831 | 58.395 | 1.00x |
| users.json | orjson | 3.104 | 3.160 | 3.194 | 58.395 | 0.82x |
| users.json | msgspec | 4.185 | 4.226 | 4.320 | 58.395 | 0.61x |
| users.json | ujson | 14.951 | 15.224 | 16.436 | 58.395 | 0.17x |
| users.json | json | 21.803 | 22.218 | 22.489 | 58.395 | 0.12x |
| flat.json | strata | 0.289 | 0.297 | 0.315 | 69.070 | 1.00x |
| flat.json | orjson | 0.357 | 0.361 | 0.382 | 69.070 | 0.82x |
| flat.json | msgspec | 0.472 | 0.484 | 0.503 | 69.070 | 0.61x |
| flat.json | ujson | 1.259 | 1.268 | 1.297 | 69.070 | 0.23x |
| flat.json | json | 1.832 | 1.846 | 1.874 | 69.070 | 0.16x |
| nested.json | strata | 0.239 | 0.241 | 0.254 | 69.070 | 1.00x |
| nested.json | orjson | 0.300 | 0.304 | 0.321 | 69.070 | 0.79x |
| nested.json | msgspec | 0.414 | 0.421 | 0.436 | 69.070 | 0.57x |
| nested.json | ujson | 1.399 | 1.410 | 1.454 | 69.070 | 0.17x |
| nested.json | json | 2.340 | 2.376 | 2.416 | 69.070 | 0.10x |
| wide_arrays.json | strata | 1.695 | 1.712 | 1.743 | 71.965 | 1.00x |
| wide_arrays.json | orjson | 1.847 | 1.867 | 1.877 | 71.965 | 0.92x |
| wide_arrays.json | msgspec | 2.949 | 2.967 | 3.023 | 71.965 | 0.58x |
| wide_arrays.json | ujson | 8.851 | 8.930 | 10.571 | 71.965 | 0.19x |
| wide_arrays.json | json | 16.496 | 16.599 | 17.182 | 71.965 | 0.10x |
| mixed.json | strata | 0.066 | 0.068 | 0.079 | 72.027 | 1.00x |
| mixed.json | orjson | 0.068 | 0.070 | 0.072 | 72.027 | 0.97x |
| mixed.json | msgspec | 0.086 | 0.089 | 0.104 | 72.027 | 0.76x |
| mixed.json | ujson | 0.303 | 0.307 | 0.318 | 72.027 | 0.22x |
| mixed.json | json | 0.511 | 0.520 | 0.536 | 72.027 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 12.072 | 13.110 | 14.159 | 75.648 | 1.00x |
| users.json | orjson | 15.164 | 16.071 | 17.914 | 75.648 | 0.82x |
| users.json | msgspec | 15.140 | 15.564 | 16.483 | 75.648 | 0.84x |
| users.json | ujson | 21.534 | 22.217 | 23.463 | 75.648 | 0.59x |
| users.json | json | 22.673 | 23.188 | 24.385 | 75.648 | 0.57x |
| flat.json | strata | 0.894 | 0.905 | 1.187 | 69.070 | 1.00x |
| flat.json | orjson | 1.082 | 1.089 | 1.128 | 69.070 | 0.83x |
| flat.json | msgspec | 1.102 | 1.130 | 1.141 | 69.070 | 0.80x |
| flat.json | ujson | 1.612 | 1.632 | 1.666 | 69.070 | 0.55x |
| flat.json | json | 1.790 | 1.809 | 1.871 | 69.070 | 0.50x |
| nested.json | strata | 0.834 | 0.847 | 0.897 | 69.070 | 1.00x |
| nested.json | orjson | 1.070 | 1.083 | 1.164 | 69.070 | 0.78x |
| nested.json | msgspec | 1.035 | 1.046 | 1.082 | 69.070 | 0.81x |
| nested.json | ujson | 1.488 | 1.519 | 1.553 | 69.070 | 0.56x |
| nested.json | json | 1.897 | 1.906 | 1.990 | 69.070 | 0.44x |
| wide_arrays.json | strata | 5.030 | 5.112 | 5.309 | 72.027 | 1.00x |
| wide_arrays.json | orjson | 5.619 | 5.688 | 5.892 | 72.027 | 0.90x |
| wide_arrays.json | msgspec | 6.190 | 6.290 | 6.589 | 72.027 | 0.81x |
| wide_arrays.json | ujson | 7.989 | 8.103 | 9.839 | 72.027 | 0.63x |
| wide_arrays.json | json | 9.841 | 9.930 | 10.527 | 72.027 | 0.51x |
| mixed.json | strata | 0.212 | 0.221 | 0.239 | 72.027 | 1.00x |
| mixed.json | orjson | 0.282 | 0.291 | 0.298 | 72.027 | 0.76x |
| mixed.json | msgspec | 0.286 | 0.295 | 0.324 | 72.027 | 0.75x |
| mixed.json | ujson | 0.356 | 0.369 | 0.420 | 72.027 | 0.60x |
| mixed.json | json | 0.488 | 0.497 | 0.594 | 72.027 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.586 | 10.900 | 16.111 | 69.070 | 1.00x |
| users.ndjson | orjson | 17.632 | 18.107 | 23.690 | 69.070 | 0.60x |
| users.ndjson | msgspec | 17.440 | 18.442 | 25.263 | 69.070 | 0.59x |
| users.ndjson | ujson | 22.138 | 23.096 | 34.519 | 69.070 | 0.47x |
| users.ndjson | json | 28.550 | 29.517 | 36.779 | 69.070 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.233 | 3.265 | 3.370 | 71.711 | 1.00x |
| users.json | orjson | 3.888 | 3.928 | 4.134 | 71.711 | 0.83x |
| users.json | msgspec | 4.935 | 4.989 | 5.047 | 71.711 | 0.65x |
| users.json | ujson | 16.010 | 16.189 | 16.640 | 71.711 | 0.20x |
| users.json | json | 23.304 | 23.665 | 410.053 | 71.711 | 0.14x |
| flat.json | strata | 0.444 | 0.458 | 0.481 | 69.070 | 1.00x |
| flat.json | orjson | 0.520 | 0.538 | 0.544 | 69.070 | 0.85x |
| flat.json | msgspec | 0.632 | 0.654 | 0.684 | 69.070 | 0.70x |
| flat.json | ujson | 1.426 | 1.436 | 1.453 | 69.070 | 0.32x |
| flat.json | json | 2.027 | 2.045 | 2.103 | 69.070 | 0.22x |
| nested.json | strata | 0.359 | 0.369 | 0.753 | 69.070 | 1.00x |
| nested.json | orjson | 0.449 | 0.459 | 0.585 | 69.070 | 0.80x |
| nested.json | msgspec | 0.557 | 0.575 | 0.627 | 69.070 | 0.64x |
| nested.json | ujson | 1.515 | 1.536 | 1.583 | 69.070 | 0.24x |
| nested.json | json | 2.531 | 2.559 | 2.743 | 69.070 | 0.14x |
| wide_arrays.json | strata | 2.114 | 2.150 | 2.457 | 72.027 | 1.00x |
| wide_arrays.json | orjson | 2.315 | 2.342 | 2.468 | 72.027 | 0.92x |
| wide_arrays.json | msgspec | 3.406 | 3.425 | 3.588 | 72.027 | 0.63x |
| wide_arrays.json | ujson | 9.080 | 9.176 | 9.396 | 72.027 | 0.23x |
| wide_arrays.json | json | 17.027 | 17.356 | 18.743 | 72.027 | 0.12x |
| mixed.json | strata | 0.158 | 0.166 | 0.170 | 72.027 | 1.00x |
| mixed.json | orjson | 0.179 | 0.181 | 0.194 | 72.027 | 0.92x |
| mixed.json | msgspec | 0.194 | 0.197 | 0.204 | 72.027 | 0.84x |
| mixed.json | ujson | 0.440 | 0.451 | 0.460 | 72.027 | 0.37x |
| mixed.json | json | 0.633 | 0.641 | 0.663 | 72.027 | 0.26x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.082 | 0.085 | 0.112 | 71.711 | 1.00x |
| users.json $[*].id | jmespath | 0.498 | 0.512 | 0.520 | 71.711 | 0.17x |
| users.json $[*].id | jsonpath-ng | 2.865 | 3.103 | 3.119 | 71.711 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.449 | 0.458 | 0.564 | 72.699 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.138 | 3.166 | 3.476 | 72.699 | 0.14x |
| users.json $[*].orders[*].total | jsonpath-ng | 20.989 | 21.776 | 22.817 | 72.699 | 0.02x |
| users.json $..total | strata | 1.792 | 1.820 | 2.023 | 74.668 | 1.00x |
| users.json $..total | jsonpath-ng | 385.038 | 387.187 | 393.096 | 74.668 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.271 | 3.303 | 3.418 | 72.699 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.692 | 16.139 | 16.504 | 72.699 | 0.20x |
| users.json $[*].id | orjson+jsonpath-ng | 18.031 | 18.788 | 19.564 | 72.699 | 0.18x |
| users.json $[*].orders[*].total | strata | 3.563 | 3.584 | 3.623 | 74.668 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.881 | 19.630 | 21.561 | 74.668 | 0.18x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 41.097 | 43.983 | 44.894 | 74.668 | 0.08x |
| users.json $..total | strata | 13.254 | 13.724 | 18.492 | 73.684 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 410.008 | 412.795 | 422.604 | 73.684 | 0.03x |

