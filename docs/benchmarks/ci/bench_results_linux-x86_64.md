# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: ddc9cb7
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
| users.json | strata | 7.568 | 8.765 | 11.494 | 58.512 | 1.00x |
| users.json | orjson | 10.056 | 11.839 | 16.163 | 58.512 | 0.74x |
| users.json | msgspec | 8.879 | 10.198 | 15.690 | 58.512 | 0.86x |
| users.json | ujson | 14.804 | 15.872 | 21.926 | 58.512 | 0.55x |
| users.json | pysimdjson | 14.883 | 17.385 | 22.171 | 58.512 | 0.50x |
| users.json | json | 15.271 | 17.666 | 19.261 | 58.512 | 0.50x |
| flat.json | strata | 0.612 | 0.643 | 1.438 | 71.492 | 1.00x |
| flat.json | orjson | 0.694 | 0.746 | 0.866 | 71.492 | 0.86x |
| flat.json | msgspec | 0.668 | 0.702 | 0.960 | 71.492 | 0.92x |
| flat.json | ujson | 1.231 | 1.281 | 1.905 | 71.492 | 0.50x |
| flat.json | pysimdjson | 1.130 | 1.225 | 1.522 | 71.492 | 0.53x |
| flat.json | json | 1.252 | 1.283 | 1.437 | 71.492 | 0.50x |
| nested.json | strata | 0.481 | 0.524 | 0.615 | 71.492 | 1.00x |
| nested.json | orjson | 0.607 | 0.641 | 0.715 | 71.492 | 0.82x |
| nested.json | msgspec | 0.580 | 0.602 | 0.678 | 71.492 | 0.87x |
| nested.json | ujson | 0.996 | 1.055 | 1.574 | 71.492 | 0.50x |
| nested.json | pysimdjson | 0.854 | 0.952 | 1.941 | 71.492 | 0.55x |
| nested.json | json | 1.430 | 1.457 | 1.586 | 71.492 | 0.36x |
| wide_arrays.json | strata | 3.108 | 4.077 | 4.375 | 76.605 | 1.00x |
| wide_arrays.json | orjson | 3.737 | 4.891 | 6.608 | 76.605 | 0.83x |
| wide_arrays.json | msgspec | 4.079 | 4.806 | 5.138 | 76.605 | 0.85x |
| wide_arrays.json | ujson | 5.110 | 5.534 | 8.662 | 76.605 | 0.74x |
| wide_arrays.json | pysimdjson | 4.587 | 5.147 | 6.197 | 76.605 | 0.79x |
| wide_arrays.json | json | 9.693 | 10.668 | 12.000 | 76.605 | 0.38x |
| mixed.json | strata | 0.123 | 0.129 | 0.173 | 76.668 | 1.00x |
| mixed.json | orjson | 0.150 | 0.158 | 0.179 | 76.668 | 0.81x |
| mixed.json | msgspec | 0.151 | 0.156 | 0.181 | 76.668 | 0.82x |
| mixed.json | ujson | 0.227 | 0.238 | 0.352 | 76.668 | 0.54x |
| mixed.json | pysimdjson | 0.202 | 0.216 | 0.323 | 76.668 | 0.60x |
| mixed.json | json | 0.307 | 0.320 | 0.328 | 76.668 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.479 | 1.649 | 1.902 | 57.609 | 1.00x |
| users.json | orjson | 1.359 | 1.504 | 1.725 | 57.609 | 1.10x |
| users.json | msgspec | 2.483 | 2.718 | 3.010 | 57.609 | 0.61x |
| users.json | ujson | 8.450 | 8.884 | 11.086 | 57.609 | 0.19x |
| users.json | json | 12.407 | 13.220 | 21.933 | 57.609 | 0.12x |
| flat.json | strata | 0.185 | 0.203 | 0.249 | 71.492 | 1.00x |
| flat.json | orjson | 0.183 | 0.203 | 0.249 | 71.492 | 1.00x |
| flat.json | msgspec | 0.289 | 0.319 | 0.341 | 71.492 | 0.64x |
| flat.json | ujson | 0.756 | 0.784 | 0.840 | 71.492 | 0.26x |
| flat.json | json | 1.059 | 1.127 | 1.781 | 71.492 | 0.18x |
| nested.json | strata | 0.131 | 0.140 | 0.149 | 71.492 | 1.00x |
| nested.json | orjson | 0.147 | 0.154 | 0.165 | 71.492 | 0.91x |
| nested.json | msgspec | 0.260 | 0.265 | 0.276 | 71.492 | 0.53x |
| nested.json | ujson | 0.749 | 0.773 | 0.962 | 71.492 | 0.18x |
| nested.json | json | 1.334 | 1.385 | 1.521 | 71.492 | 0.10x |
| wide_arrays.json | strata | 1.215 | 1.296 | 1.788 | 76.605 | 1.00x |
| wide_arrays.json | orjson | 1.046 | 1.072 | 1.239 | 76.605 | 1.21x |
| wide_arrays.json | msgspec | 1.861 | 1.906 | 2.335 | 76.605 | 0.68x |
| wide_arrays.json | ujson | 4.972 | 5.077 | 7.383 | 76.605 | 0.26x |
| wide_arrays.json | json | 9.990 | 10.091 | 11.725 | 76.605 | 0.13x |
| mixed.json | strata | 0.040 | 0.043 | 0.069 | 76.668 | 1.00x |
| mixed.json | orjson | 0.037 | 0.038 | 0.040 | 76.668 | 1.15x |
| mixed.json | msgspec | 0.054 | 0.056 | 0.062 | 76.668 | 0.78x |
| mixed.json | ujson | 0.179 | 0.180 | 0.196 | 76.668 | 0.24x |
| mixed.json | json | 0.308 | 0.322 | 0.626 | 76.668 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.071 | 9.167 | 11.906 | 75.102 | 1.00x |
| users.json | orjson | 9.781 | 11.145 | 13.038 | 75.102 | 0.82x |
| users.json | msgspec | 9.529 | 10.162 | 11.246 | 75.102 | 0.90x |
| users.json | ujson | 14.614 | 16.629 | 24.665 | 75.102 | 0.55x |
| users.json | json | 16.736 | 17.534 | 22.762 | 75.102 | 0.52x |
| flat.json | strata | 0.587 | 0.681 | 0.715 | 71.492 | 1.00x |
| flat.json | orjson | 0.705 | 0.800 | 1.369 | 71.492 | 0.85x |
| flat.json | msgspec | 0.660 | 0.782 | 1.225 | 71.492 | 0.87x |
| flat.json | ujson | 1.044 | 1.297 | 1.361 | 71.492 | 0.52x |
| flat.json | json | 1.243 | 1.364 | 1.444 | 71.492 | 0.50x |
| nested.json | strata | 0.546 | 0.600 | 1.192 | 71.492 | 1.00x |
| nested.json | orjson | 0.709 | 0.765 | 1.003 | 71.492 | 0.78x |
| nested.json | msgspec | 0.656 | 0.696 | 0.891 | 71.492 | 0.86x |
| nested.json | ujson | 0.972 | 1.005 | 1.278 | 71.492 | 0.60x |
| nested.json | json | 1.479 | 1.511 | 1.787 | 71.492 | 0.40x |
| wide_arrays.json | strata | 3.354 | 3.693 | 5.705 | 76.668 | 1.00x |
| wide_arrays.json | orjson | 4.244 | 4.913 | 6.438 | 76.668 | 0.75x |
| wide_arrays.json | msgspec | 4.633 | 4.840 | 5.370 | 76.668 | 0.76x |
| wide_arrays.json | ujson | 5.622 | 6.170 | 6.685 | 76.668 | 0.60x |
| wide_arrays.json | json | 10.075 | 10.653 | 11.650 | 76.668 | 0.35x |
| mixed.json | strata | 0.141 | 0.149 | 0.221 | 76.668 | 1.00x |
| mixed.json | orjson | 0.177 | 0.196 | 0.316 | 76.668 | 0.76x |
| mixed.json | msgspec | 0.177 | 0.201 | 0.305 | 76.668 | 0.74x |
| mixed.json | ujson | 0.261 | 0.277 | 0.299 | 76.668 | 0.54x |
| mixed.json | json | 0.321 | 0.339 | 0.358 | 76.668 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.930 | 11.577 | 16.360 | 71.492 | 1.00x |
| users.ndjson | orjson | 14.987 | 16.055 | 18.084 | 71.492 | 0.72x |
| users.ndjson | msgspec | 15.056 | 15.572 | 18.228 | 71.492 | 0.74x |
| users.ndjson | ujson | 18.992 | 21.169 | 29.443 | 71.492 | 0.55x |
| users.ndjson | json | 24.647 | 26.287 | 28.943 | 71.492 | 0.44x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.004 | 2.102 | 2.546 | 70.180 | 1.00x |
| users.json | orjson | 2.010 | 2.185 | 33.735 | 70.180 | 0.96x |
| users.json | msgspec | 3.099 | 3.408 | 7.817 | 70.180 | 0.62x |
| users.json | ujson | 9.141 | 9.398 | 10.132 | 70.180 | 0.22x |
| users.json | json | 12.829 | 13.352 | 34.277 | 70.180 | 0.16x |
| flat.json | strata | 0.372 | 0.405 | 2.458 | 71.492 | 1.00x |
| flat.json | orjson | 0.366 | 0.404 | 0.551 | 71.492 | 1.00x |
| flat.json | msgspec | 0.470 | 0.512 | 0.640 | 71.492 | 0.79x |
| flat.json | ujson | 0.944 | 1.006 | 123.463 | 71.492 | 0.40x |
| flat.json | json | 1.293 | 1.358 | 2.191 | 71.492 | 0.30x |
| nested.json | strata | 0.238 | 0.324 | 35.429 | 71.492 | 1.00x |
| nested.json | orjson | 0.259 | 0.366 | 6.128 | 71.492 | 0.89x |
| nested.json | msgspec | 0.365 | 0.462 | 0.523 | 71.492 | 0.70x |
| nested.json | ujson | 0.872 | 0.960 | 18.571 | 71.492 | 0.34x |
| nested.json | json | 1.431 | 1.526 | 7.580 | 71.492 | 0.21x |
| wide_arrays.json | strata | 1.748 | 1.835 | 134.721 | 76.668 | 1.00x |
| wide_arrays.json | orjson | 1.568 | 1.716 | 2.143 | 76.668 | 1.07x |
| wide_arrays.json | msgspec | 2.400 | 2.459 | 6.352 | 76.668 | 0.75x |
| wide_arrays.json | ujson | 5.575 | 5.790 | 29.653 | 76.668 | 0.32x |
| wide_arrays.json | json | 10.694 | 11.204 | 32.930 | 76.668 | 0.16x |
| mixed.json | strata | 0.103 | 0.112 | 0.135 | 76.668 | 1.00x |
| mixed.json | orjson | 0.098 | 0.108 | 26.487 | 76.668 | 1.04x |
| mixed.json | msgspec | 0.112 | 0.119 | 0.191 | 76.668 | 0.94x |
| mixed.json | ujson | 0.235 | 0.245 | 0.443 | 76.668 | 0.46x |
| mixed.json | json | 0.355 | 0.368 | 0.400 | 76.668 | 0.30x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.052 | 0.054 | 0.057 | 70.180 | 1.00x |
| users.json $[*].id | jmespath | 0.278 | 0.289 | 0.295 | 70.180 | 0.19x |
| users.json $[*].id | jsonpath-ng | 1.749 | 1.965 | 2.062 | 70.180 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.282 | 0.322 | 0.462 | 71.266 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.791 | 1.813 | 1.876 | 71.266 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 14.385 | 15.510 | 19.032 | 71.266 | 0.02x |
| users.json $..total | strata | 1.140 | 1.337 | 1.699 | 74.121 | 1.00x |
| users.json $..total | jsonpath-ng | 232.803 | 238.741 | 244.318 | 74.121 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 2.086 | 2.188 | 2.237 | 71.266 | 1.00x |
| users.json $[*].id | orjson+jmespath | 9.968 | 10.970 | 15.399 | 71.266 | 0.20x |
| users.json $[*].id | orjson+jsonpath-ng | 11.019 | 13.187 | 19.004 | 71.266 | 0.17x |
| users.json $[*].orders[*].total | strata | 2.427 | 2.482 | 2.534 | 74.121 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.669 | 14.024 | 15.158 | 74.121 | 0.18x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 29.163 | 31.658 | 32.768 | 74.121 | 0.08x |
| users.json $..total | strata | 10.967 | 13.805 | 16.251 | 74.121 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 258.659 | 266.380 | 300.810 | 74.121 | 0.05x |

