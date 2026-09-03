# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 2f12155
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
| users.json | strata | 8.230 | 8.925 | 11.816 | 58.059 | 1.00x |
| users.json | orjson | 10.869 | 11.440 | 13.736 | 58.059 | 0.78x |
| users.json | msgspec | 10.450 | 10.727 | 13.559 | 58.059 | 0.83x |
| users.json | ujson | 14.974 | 16.133 | 19.150 | 58.059 | 0.55x |
| users.json | pysimdjson | 16.744 | 17.514 | 18.906 | 58.059 | 0.51x |
| users.json | json | 17.025 | 17.315 | 18.124 | 58.059 | 0.52x |
| flat.json | strata | 0.708 | 0.723 | 0.745 | 69.465 | 1.00x |
| flat.json | orjson | 0.809 | 0.816 | 0.844 | 69.465 | 0.89x |
| flat.json | msgspec | 0.775 | 0.789 | 0.804 | 69.465 | 0.92x |
| flat.json | ujson | 1.202 | 1.250 | 1.311 | 69.465 | 0.58x |
| flat.json | pysimdjson | 1.263 | 1.289 | 1.377 | 69.465 | 0.56x |
| flat.json | json | 1.336 | 1.352 | 1.549 | 69.465 | 0.53x |
| nested.json | strata | 0.610 | 0.624 | 0.819 | 69.465 | 1.00x |
| nested.json | orjson | 0.781 | 0.786 | 0.793 | 69.465 | 0.79x |
| nested.json | msgspec | 0.721 | 0.739 | 0.756 | 69.465 | 0.84x |
| nested.json | ujson | 1.099 | 1.146 | 1.182 | 69.465 | 0.54x |
| nested.json | pysimdjson | 1.116 | 1.151 | 1.166 | 69.465 | 0.54x |
| nested.json | json | 1.445 | 1.458 | 1.495 | 69.465 | 0.43x |
| wide_arrays.json | strata | 3.846 | 3.916 | 3.994 | 73.523 | 1.00x |
| wide_arrays.json | orjson | 4.209 | 4.296 | 4.402 | 73.523 | 0.91x |
| wide_arrays.json | msgspec | 4.752 | 4.870 | 5.023 | 73.523 | 0.80x |
| wide_arrays.json | ujson | 5.923 | 6.017 | 6.809 | 73.523 | 0.65x |
| wide_arrays.json | pysimdjson | 4.787 | 4.949 | 5.437 | 73.523 | 0.79x |
| wide_arrays.json | json | 7.570 | 7.638 | 7.908 | 73.523 | 0.51x |
| mixed.json | strata | 0.151 | 0.155 | 0.169 | 73.586 | 1.00x |
| mixed.json | orjson | 0.180 | 0.187 | 0.208 | 73.586 | 0.83x |
| mixed.json | msgspec | 0.189 | 0.191 | 0.201 | 73.586 | 0.81x |
| mixed.json | ujson | 0.242 | 0.246 | 0.262 | 73.586 | 0.63x |
| mixed.json | pysimdjson | 0.232 | 0.237 | 0.250 | 73.586 | 0.65x |
| mixed.json | json | 0.363 | 0.370 | 0.384 | 73.586 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.970 | 2.007 | 2.430 | 56.781 | 1.00x |
| users.json | orjson | 1.992 | 2.007 | 2.018 | 56.781 | 1.00x |
| users.json | msgspec | 3.231 | 3.289 | 4.814 | 56.781 | 0.61x |
| users.json | ujson | 11.303 | 11.427 | 18.049 | 56.781 | 0.18x |
| users.json | json | 16.861 | 17.117 | 28.417 | 56.781 | 0.12x |
| flat.json | strata | 0.225 | 0.230 | 0.237 | 69.465 | 1.00x |
| flat.json | orjson | 0.238 | 0.242 | 0.251 | 69.465 | 0.95x |
| flat.json | msgspec | 0.370 | 0.380 | 0.392 | 69.465 | 0.61x |
| flat.json | ujson | 0.985 | 0.996 | 1.010 | 69.465 | 0.23x |
| flat.json | json | 1.460 | 1.510 | 1.554 | 69.465 | 0.15x |
| nested.json | strata | 0.187 | 0.189 | 0.202 | 69.465 | 1.00x |
| nested.json | orjson | 0.218 | 0.225 | 0.235 | 69.465 | 0.84x |
| nested.json | msgspec | 0.326 | 0.337 | 0.362 | 69.465 | 0.56x |
| nested.json | ujson | 1.071 | 1.080 | 1.090 | 69.465 | 0.17x |
| nested.json | json | 1.863 | 1.878 | 1.893 | 69.465 | 0.10x |
| wide_arrays.json | strata | 1.427 | 1.443 | 1.462 | 73.523 | 1.00x |
| wide_arrays.json | orjson | 1.377 | 1.393 | 1.405 | 73.523 | 1.04x |
| wide_arrays.json | msgspec | 2.274 | 2.293 | 2.314 | 73.523 | 0.63x |
| wide_arrays.json | ujson | 6.644 | 6.689 | 6.819 | 73.523 | 0.22x |
| wide_arrays.json | json | 12.921 | 13.004 | 13.099 | 73.523 | 0.11x |
| mixed.json | strata | 0.052 | 0.054 | 0.062 | 73.586 | 1.00x |
| mixed.json | orjson | 0.048 | 0.050 | 0.052 | 73.586 | 1.09x |
| mixed.json | msgspec | 0.067 | 0.069 | 0.081 | 73.586 | 0.78x |
| mixed.json | ujson | 0.234 | 0.244 | 0.248 | 73.586 | 0.22x |
| mixed.json | json | 0.398 | 0.410 | 0.436 | 73.586 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.518 | 9.852 | 11.103 | 75.027 | 1.00x |
| users.json | orjson | 11.585 | 11.989 | 12.665 | 75.027 | 0.82x |
| users.json | msgspec | 11.249 | 11.677 | 12.207 | 75.027 | 0.84x |
| users.json | ujson | 15.521 | 16.765 | 18.455 | 75.027 | 0.59x |
| users.json | json | 17.471 | 17.839 | 19.826 | 75.027 | 0.55x |
| flat.json | strata | 0.736 | 0.743 | 0.755 | 69.465 | 1.00x |
| flat.json | orjson | 0.862 | 0.870 | 0.917 | 69.465 | 0.85x |
| flat.json | msgspec | 0.821 | 0.837 | 0.858 | 69.465 | 0.89x |
| flat.json | ujson | 1.242 | 1.287 | 1.313 | 69.465 | 0.58x |
| flat.json | json | 1.384 | 1.393 | 1.455 | 69.465 | 0.53x |
| nested.json | strata | 0.657 | 0.666 | 0.682 | 69.465 | 1.00x |
| nested.json | orjson | 0.825 | 0.848 | 0.939 | 69.465 | 0.79x |
| nested.json | msgspec | 0.787 | 0.802 | 0.815 | 69.465 | 0.83x |
| nested.json | ujson | 1.165 | 1.193 | 1.311 | 69.465 | 0.56x |
| nested.json | json | 1.500 | 1.520 | 1.541 | 69.465 | 0.44x |
| wide_arrays.json | strata | 3.925 | 3.962 | 4.328 | 73.586 | 1.00x |
| wide_arrays.json | orjson | 4.288 | 4.411 | 4.538 | 73.586 | 0.90x |
| wide_arrays.json | msgspec | 4.885 | 5.004 | 5.113 | 73.586 | 0.79x |
| wide_arrays.json | ujson | 6.170 | 6.277 | 6.422 | 73.586 | 0.63x |
| wide_arrays.json | json | 7.686 | 7.767 | 7.870 | 73.586 | 0.51x |
| mixed.json | strata | 0.168 | 0.171 | 0.184 | 73.586 | 1.00x |
| mixed.json | orjson | 0.221 | 0.225 | 0.240 | 73.586 | 0.76x |
| mixed.json | msgspec | 0.224 | 0.229 | 0.245 | 73.586 | 0.74x |
| mixed.json | ujson | 0.284 | 0.301 | 0.312 | 73.586 | 0.57x |
| mixed.json | json | 0.393 | 0.404 | 0.427 | 73.586 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.240 | 10.171 | 10.633 | 69.465 | 1.00x |
| users.ndjson | orjson | 14.612 | 15.179 | 15.528 | 69.465 | 0.67x |
| users.ndjson | msgspec | 14.220 | 14.675 | 15.394 | 69.465 | 0.69x |
| users.ndjson | ujson | 18.847 | 19.317 | 20.227 | 69.465 | 0.53x |
| users.ndjson | json | 23.410 | 24.262 | 25.199 | 69.465 | 0.42x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.458 | 2.485 | 2.593 | 72.074 | 1.00x |
| users.json | orjson | 2.553 | 2.570 | 2.637 | 72.074 | 0.97x |
| users.json | msgspec | 3.762 | 3.820 | 9.886 | 72.074 | 0.65x |
| users.json | ujson | 11.964 | 12.133 | 17.528 | 72.074 | 0.20x |
| users.json | json | 17.738 | 23.464 | 35.785 | 72.074 | 0.11x |
| flat.json | strata | 0.350 | 0.355 | 1.927 | 69.465 | 1.00x |
| flat.json | orjson | 0.368 | 0.375 | 0.423 | 69.465 | 0.95x |
| flat.json | msgspec | 0.502 | 0.513 | 0.527 | 69.465 | 0.69x |
| flat.json | ujson | 1.121 | 1.150 | 1.198 | 69.465 | 0.31x |
| flat.json | json | 1.609 | 1.653 | 1.672 | 69.465 | 0.21x |
| nested.json | strata | 0.277 | 0.289 | 0.321 | 69.465 | 1.00x |
| nested.json | orjson | 0.330 | 0.340 | 0.386 | 69.465 | 0.85x |
| nested.json | msgspec | 0.433 | 0.447 | 0.474 | 69.465 | 0.65x |
| nested.json | ujson | 1.192 | 1.201 | 1.234 | 69.465 | 0.24x |
| nested.json | json | 1.972 | 2.002 | 57.669 | 69.465 | 0.14x |
| wide_arrays.json | strata | 1.719 | 1.771 | 18.390 | 73.586 | 1.00x |
| wide_arrays.json | orjson | 1.778 | 1.825 | 23.595 | 73.586 | 0.97x |
| wide_arrays.json | msgspec | 2.674 | 2.713 | 2.775 | 73.586 | 0.65x |
| wide_arrays.json | ujson | 7.100 | 7.204 | 7.339 | 73.586 | 0.25x |
| wide_arrays.json | json | 13.549 | 13.771 | 43.263 | 73.586 | 0.13x |
| mixed.json | strata | 0.123 | 0.131 | 1.574 | 73.586 | 1.00x |
| mixed.json | orjson | 0.127 | 0.131 | 0.148 | 73.586 | 1.00x |
| mixed.json | msgspec | 0.144 | 0.148 | 0.164 | 73.586 | 0.89x |
| mixed.json | ujson | 0.326 | 0.341 | 0.364 | 73.586 | 0.39x |
| mixed.json | json | 0.493 | 0.515 | 0.536 | 73.586 | 0.26x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.058 | 0.062 | 0.078 | 72.074 | 1.00x |
| users.json $[*].id | jmespath | 0.365 | 0.372 | 0.383 | 72.074 | 0.17x |
| users.json $[*].id | jsonpath-ng | 2.175 | 2.278 | 2.394 | 72.074 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.340 | 0.349 | 0.399 | 72.078 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.343 | 2.374 | 2.509 | 72.078 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 15.047 | 15.960 | 16.220 | 72.078 | 0.02x |
| users.json $..total | strata | 1.390 | 1.407 | 1.458 | 74.047 | 1.00x |
| users.json $..total | jsonpath-ng | 297.032 | 298.407 | 301.383 | 74.047 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 2.578 | 2.601 | 2.645 | 72.078 | 1.00x |
| users.json $[*].id | orjson+jmespath | 11.765 | 12.373 | 12.794 | 72.078 | 0.21x |
| users.json $[*].id | orjson+jsonpath-ng | 13.880 | 14.191 | 14.546 | 72.078 | 0.18x |
| users.json $[*].orders[*].total | strata | 2.812 | 2.850 | 2.884 | 73.250 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 14.278 | 14.653 | 15.576 | 73.250 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 30.543 | 31.587 | 34.069 | 73.250 | 0.09x |
| users.json $..total | strata | 11.886 | 12.197 | 13.761 | 73.270 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 316.660 | 320.617 | 322.778 | 73.270 | 0.04x |

