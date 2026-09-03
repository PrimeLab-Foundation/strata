# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 49acca3
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
| users.json | strata | 8.029 | 8.408 | 14.395 | 59.617 | 1.00x |
| users.json | orjson | 10.810 | 11.185 | 13.320 | 59.617 | 0.75x |
| users.json | msgspec | 10.621 | 10.987 | 12.819 | 59.617 | 0.77x |
| users.json | ujson | 14.609 | 15.714 | 26.065 | 59.617 | 0.54x |
| users.json | pysimdjson | 15.281 | 16.678 | 26.273 | 59.617 | 0.50x |
| users.json | json | 16.502 | 16.931 | 18.802 | 59.617 | 0.50x |
| flat.json | strata | 0.704 | 0.728 | 0.758 | 70.195 | 1.00x |
| flat.json | orjson | 0.822 | 0.848 | 0.909 | 70.195 | 0.86x |
| flat.json | msgspec | 0.792 | 0.805 | 1.025 | 70.195 | 0.90x |
| flat.json | ujson | 1.212 | 1.322 | 1.415 | 70.195 | 0.55x |
| flat.json | pysimdjson | 1.278 | 1.365 | 1.459 | 70.195 | 0.53x |
| flat.json | json | 1.352 | 1.363 | 1.387 | 70.195 | 0.53x |
| nested.json | strata | 0.639 | 0.646 | 0.674 | 70.195 | 1.00x |
| nested.json | orjson | 0.781 | 0.789 | 0.797 | 70.195 | 0.82x |
| nested.json | msgspec | 0.732 | 0.747 | 0.795 | 70.195 | 0.86x |
| nested.json | ujson | 1.126 | 1.139 | 1.167 | 70.195 | 0.57x |
| nested.json | pysimdjson | 1.115 | 1.128 | 1.144 | 70.195 | 0.57x |
| nested.json | json | 1.418 | 1.432 | 1.487 | 70.195 | 0.45x |
| wide_arrays.json | strata | 3.903 | 3.949 | 5.944 | 74.566 | 1.00x |
| wide_arrays.json | orjson | 4.304 | 4.526 | 6.097 | 74.566 | 0.87x |
| wide_arrays.json | msgspec | 4.749 | 4.879 | 7.039 | 74.566 | 0.81x |
| wide_arrays.json | ujson | 6.026 | 6.183 | 9.159 | 74.566 | 0.64x |
| wide_arrays.json | pysimdjson | 4.938 | 5.022 | 6.380 | 74.566 | 0.79x |
| wide_arrays.json | json | 7.639 | 7.879 | 8.912 | 74.566 | 0.50x |
| mixed.json | strata | 0.155 | 0.158 | 0.174 | 74.629 | 1.00x |
| mixed.json | orjson | 0.183 | 0.193 | 0.209 | 74.629 | 0.82x |
| mixed.json | msgspec | 0.192 | 0.199 | 0.227 | 74.629 | 0.79x |
| mixed.json | ujson | 0.246 | 0.258 | 0.416 | 74.629 | 0.61x |
| mixed.json | pysimdjson | 0.234 | 0.248 | 0.380 | 74.629 | 0.64x |
| mixed.json | json | 0.352 | 0.360 | 0.388 | 74.629 | 0.44x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.990 | 2.015 | 2.075 | 58.031 | 1.00x |
| users.json | orjson | 1.991 | 2.013 | 2.047 | 58.031 | 1.00x |
| users.json | msgspec | 3.228 | 3.262 | 3.331 | 58.031 | 0.62x |
| users.json | ujson | 11.150 | 11.322 | 12.766 | 58.031 | 0.18x |
| users.json | json | 17.107 | 17.273 | 25.564 | 58.031 | 0.12x |
| flat.json | strata | 0.231 | 0.247 | 0.275 | 70.195 | 1.00x |
| flat.json | orjson | 0.248 | 0.255 | 0.272 | 70.195 | 0.97x |
| flat.json | msgspec | 0.386 | 0.399 | 0.409 | 70.195 | 0.62x |
| flat.json | ujson | 0.992 | 1.003 | 1.019 | 70.195 | 0.25x |
| flat.json | json | 1.480 | 1.495 | 1.534 | 70.195 | 0.16x |
| nested.json | strata | 0.184 | 0.188 | 0.203 | 70.195 | 1.00x |
| nested.json | orjson | 0.221 | 0.229 | 0.245 | 70.195 | 0.82x |
| nested.json | msgspec | 0.323 | 0.340 | 0.352 | 70.195 | 0.55x |
| nested.json | ujson | 1.072 | 1.082 | 1.220 | 70.195 | 0.17x |
| nested.json | json | 1.843 | 1.857 | 1.920 | 70.195 | 0.10x |
| wide_arrays.json | strata | 1.350 | 1.367 | 1.491 | 74.566 | 1.00x |
| wide_arrays.json | orjson | 1.389 | 1.412 | 1.843 | 74.566 | 0.97x |
| wide_arrays.json | msgspec | 2.300 | 2.332 | 3.035 | 74.566 | 0.59x |
| wide_arrays.json | ujson | 6.596 | 6.644 | 6.730 | 74.566 | 0.21x |
| wide_arrays.json | json | 12.943 | 13.194 | 15.325 | 74.566 | 0.10x |
| mixed.json | strata | 0.055 | 0.056 | 0.077 | 74.629 | 1.00x |
| mixed.json | orjson | 0.049 | 0.051 | 0.088 | 74.629 | 1.10x |
| mixed.json | msgspec | 0.069 | 0.071 | 0.074 | 74.629 | 0.79x |
| mixed.json | ujson | 0.234 | 0.242 | 0.279 | 74.629 | 0.23x |
| mixed.json | json | 0.401 | 0.415 | 0.426 | 74.629 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.715 | 9.643 | 10.884 | 74.793 | 1.00x |
| users.json | orjson | 11.275 | 12.122 | 12.647 | 74.793 | 0.80x |
| users.json | msgspec | 11.180 | 11.741 | 12.655 | 74.793 | 0.82x |
| users.json | ujson | 15.885 | 17.069 | 19.284 | 74.793 | 0.56x |
| users.json | json | 16.924 | 17.535 | 18.003 | 74.793 | 0.55x |
| flat.json | strata | 0.763 | 0.804 | 0.838 | 70.195 | 1.00x |
| flat.json | orjson | 0.898 | 0.938 | 0.969 | 70.195 | 0.86x |
| flat.json | msgspec | 0.846 | 0.887 | 0.925 | 70.195 | 0.91x |
| flat.json | ujson | 1.328 | 1.461 | 1.523 | 70.195 | 0.55x |
| flat.json | json | 1.403 | 1.432 | 1.500 | 70.195 | 0.56x |
| nested.json | strata | 0.672 | 0.681 | 0.912 | 70.195 | 1.00x |
| nested.json | orjson | 0.834 | 0.860 | 1.107 | 70.195 | 0.79x |
| nested.json | msgspec | 0.799 | 0.814 | 1.108 | 70.195 | 0.84x |
| nested.json | ujson | 1.168 | 1.221 | 1.417 | 70.195 | 0.56x |
| nested.json | json | 1.463 | 1.513 | 1.696 | 70.195 | 0.45x |
| wide_arrays.json | strata | 3.984 | 4.159 | 4.462 | 74.629 | 1.00x |
| wide_arrays.json | orjson | 4.467 | 4.606 | 4.978 | 74.629 | 0.90x |
| wide_arrays.json | msgspec | 4.956 | 5.187 | 5.775 | 74.629 | 0.80x |
| wide_arrays.json | ujson | 6.301 | 6.576 | 6.931 | 74.629 | 0.63x |
| wide_arrays.json | json | 7.774 | 9.095 | 10.742 | 74.629 | 0.46x |
| mixed.json | strata | 0.169 | 0.172 | 0.186 | 74.629 | 1.00x |
| mixed.json | orjson | 0.220 | 0.225 | 0.265 | 74.629 | 0.77x |
| mixed.json | msgspec | 0.220 | 0.225 | 0.372 | 74.629 | 0.76x |
| mixed.json | ujson | 0.281 | 0.295 | 0.474 | 74.629 | 0.58x |
| mixed.json | json | 0.385 | 0.395 | 0.402 | 74.629 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.274 | 9.789 | 10.997 | 70.195 | 1.00x |
| users.ndjson | orjson | 14.508 | 15.030 | 16.767 | 70.195 | 0.65x |
| users.ndjson | msgspec | 14.098 | 14.751 | 20.622 | 70.195 | 0.66x |
| users.ndjson | ujson | 18.026 | 19.777 | 20.804 | 70.195 | 0.49x |
| users.ndjson | json | 22.755 | 24.379 | 26.044 | 70.195 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.437 | 2.524 | 2.639 | 71.840 | 1.00x |
| users.json | orjson | 2.507 | 2.597 | 2.724 | 71.840 | 0.97x |
| users.json | msgspec | 3.696 | 3.834 | 4.084 | 71.840 | 0.66x |
| users.json | ujson | 11.954 | 12.229 | 44.579 | 71.840 | 0.21x |
| users.json | json | 17.650 | 18.196 | 29.261 | 71.840 | 0.14x |
| flat.json | strata | 0.345 | 0.364 | 0.399 | 70.195 | 1.00x |
| flat.json | orjson | 0.369 | 0.381 | 0.404 | 70.195 | 0.95x |
| flat.json | msgspec | 0.501 | 0.520 | 0.541 | 70.195 | 0.70x |
| flat.json | ujson | 1.133 | 1.150 | 1.302 | 70.195 | 0.32x |
| flat.json | json | 1.620 | 1.654 | 1.670 | 70.195 | 0.22x |
| nested.json | strata | 0.289 | 0.309 | 0.324 | 70.195 | 1.00x |
| nested.json | orjson | 0.340 | 0.361 | 0.402 | 70.195 | 0.86x |
| nested.json | msgspec | 0.445 | 0.460 | 36.904 | 70.195 | 0.67x |
| nested.json | ujson | 1.185 | 1.208 | 1.565 | 70.195 | 0.26x |
| nested.json | json | 1.963 | 2.032 | 4.329 | 70.195 | 0.15x |
| wide_arrays.json | strata | 1.732 | 1.805 | 72.008 | 74.629 | 1.00x |
| wide_arrays.json | orjson | 1.796 | 1.873 | 2.590 | 74.629 | 0.96x |
| wide_arrays.json | msgspec | 2.712 | 2.787 | 3.136 | 74.629 | 0.65x |
| wide_arrays.json | ujson | 7.066 | 7.209 | 8.179 | 74.629 | 0.25x |
| wide_arrays.json | json | 13.524 | 19.068 | 46.637 | 74.629 | 0.09x |
| mixed.json | strata | 0.123 | 0.131 | 74.683 | 74.629 | 1.00x |
| mixed.json | orjson | 0.129 | 0.136 | 0.189 | 74.629 | 0.96x |
| mixed.json | msgspec | 0.148 | 0.153 | 173.196 | 74.629 | 0.86x |
| mixed.json | ujson | 0.322 | 0.341 | 0.398 | 74.629 | 0.38x |
| mixed.json | json | 0.502 | 0.510 | 0.557 | 74.629 | 0.26x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.058 | 0.068 | 0.076 | 71.840 | 1.00x |
| users.json $[*].id | jmespath | 0.368 | 0.382 | 0.407 | 71.840 | 0.18x |
| users.json $[*].id | jsonpath-ng | 2.262 | 2.401 | 2.525 | 71.840 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.357 | 0.375 | 0.403 | 71.840 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.315 | 2.359 | 2.388 | 71.840 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 15.034 | 15.511 | 17.335 | 71.840 | 0.02x |
| users.json $..total | strata | 1.415 | 1.438 | 1.477 | 72.824 | 1.00x |
| users.json $..total | jsonpath-ng | 297.614 | 299.519 | 302.124 | 72.824 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 2.569 | 2.594 | 2.611 | 71.840 | 1.00x |
| users.json $[*].id | orjson+jmespath | 11.909 | 12.096 | 12.239 | 71.840 | 0.21x |
| users.json $[*].id | orjson+jsonpath-ng | 13.665 | 13.937 | 16.391 | 71.840 | 0.19x |
| users.json $[*].orders[*].total | strata | 2.766 | 2.796 | 3.043 | 72.824 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 14.170 | 14.385 | 15.284 | 72.824 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 29.939 | 30.548 | 31.060 | 72.824 | 0.09x |
| users.json $..total | strata | 11.513 | 12.038 | 14.297 | 72.809 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 318.339 | 322.066 | 323.670 | 72.809 | 0.04x |

