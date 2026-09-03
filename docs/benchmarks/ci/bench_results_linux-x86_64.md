# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c88786c
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
| users.json | strata | 11.695 | 12.443 | 16.843 | 59.598 | 1.00x |
| users.json | orjson | 14.590 | 15.969 | 26.069 | 59.598 | 0.78x |
| users.json | msgspec | 14.461 | 15.143 | 29.621 | 59.598 | 0.82x |
| users.json | ujson | 21.044 | 22.384 | 46.003 | 59.598 | 0.56x |
| users.json | pysimdjson | 21.350 | 23.760 | 46.761 | 59.598 | 0.52x |
| users.json | json | 22.202 | 23.589 | 25.102 | 59.598 | 0.53x |
| flat.json | strata | 0.923 | 0.937 | 0.959 | 72.035 | 1.00x |
| flat.json | orjson | 1.073 | 1.098 | 1.130 | 72.035 | 0.85x |
| flat.json | msgspec | 1.109 | 1.116 | 1.157 | 72.035 | 0.84x |
| flat.json | ujson | 1.635 | 1.743 | 1.824 | 72.035 | 0.54x |
| flat.json | pysimdjson | 1.698 | 1.731 | 1.786 | 72.035 | 0.54x |
| flat.json | json | 1.743 | 1.762 | 1.823 | 72.035 | 0.53x |
| nested.json | strata | 0.836 | 0.855 | 0.893 | 72.035 | 1.00x |
| nested.json | orjson | 1.039 | 1.083 | 1.617 | 72.035 | 0.79x |
| nested.json | msgspec | 0.995 | 1.032 | 1.061 | 72.035 | 0.83x |
| nested.json | ujson | 1.601 | 1.640 | 1.741 | 72.035 | 0.52x |
| nested.json | pysimdjson | 1.513 | 1.553 | 1.648 | 72.035 | 0.55x |
| nested.json | json | 1.855 | 1.881 | 1.974 | 72.035 | 0.45x |
| wide_arrays.json | strata | 5.019 | 5.166 | 5.502 | 76.035 | 1.00x |
| wide_arrays.json | orjson | 5.686 | 5.851 | 6.040 | 76.035 | 0.88x |
| wide_arrays.json | msgspec | 6.163 | 6.289 | 6.489 | 76.035 | 0.82x |
| wide_arrays.json | ujson | 7.824 | 8.042 | 8.424 | 76.035 | 0.64x |
| wide_arrays.json | pysimdjson | 6.494 | 6.727 | 7.019 | 76.035 | 0.77x |
| wide_arrays.json | json | 9.932 | 10.028 | 10.250 | 76.035 | 0.52x |
| mixed.json | strata | 0.207 | 0.220 | 0.387 | 76.098 | 1.00x |
| mixed.json | orjson | 0.252 | 0.273 | 0.281 | 76.098 | 0.80x |
| mixed.json | msgspec | 0.258 | 0.275 | 0.317 | 76.098 | 0.80x |
| mixed.json | ujson | 0.352 | 0.366 | 0.393 | 76.098 | 0.60x |
| mixed.json | pysimdjson | 0.317 | 0.341 | 0.383 | 76.098 | 0.64x |
| mixed.json | json | 0.460 | 0.489 | 0.512 | 76.098 | 0.45x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.524 | 2.637 | 2.818 | 57.957 | 1.00x |
| users.json | orjson | 3.160 | 3.255 | 3.667 | 57.957 | 0.81x |
| users.json | msgspec | 4.216 | 4.326 | 4.779 | 57.957 | 0.61x |
| users.json | ujson | 14.758 | 15.128 | 19.228 | 57.957 | 0.17x |
| users.json | json | 21.819 | 22.104 | 22.631 | 57.957 | 0.12x |
| flat.json | strata | 0.312 | 0.332 | 0.732 | 72.035 | 1.00x |
| flat.json | orjson | 0.380 | 0.394 | 0.424 | 72.035 | 0.84x |
| flat.json | msgspec | 0.489 | 0.512 | 0.541 | 72.035 | 0.65x |
| flat.json | ujson | 1.257 | 1.276 | 1.334 | 72.035 | 0.26x |
| flat.json | json | 1.870 | 1.908 | 2.220 | 72.035 | 0.17x |
| nested.json | strata | 0.246 | 0.258 | 0.323 | 72.035 | 1.00x |
| nested.json | orjson | 0.299 | 0.315 | 0.577 | 72.035 | 0.82x |
| nested.json | msgspec | 0.428 | 0.440 | 0.864 | 72.035 | 0.59x |
| nested.json | ujson | 1.367 | 1.376 | 2.228 | 72.035 | 0.19x |
| nested.json | json | 2.333 | 2.379 | 3.364 | 72.035 | 0.11x |
| wide_arrays.json | strata | 1.703 | 1.739 | 1.896 | 76.035 | 1.00x |
| wide_arrays.json | orjson | 1.924 | 1.946 | 2.041 | 76.035 | 0.89x |
| wide_arrays.json | msgspec | 2.973 | 2.995 | 3.030 | 76.035 | 0.58x |
| wide_arrays.json | ujson | 8.539 | 8.586 | 8.753 | 76.035 | 0.20x |
| wide_arrays.json | json | 16.509 | 16.608 | 16.829 | 76.035 | 0.10x |
| mixed.json | strata | 0.072 | 0.074 | 0.079 | 76.098 | 1.00x |
| mixed.json | orjson | 0.072 | 0.074 | 0.076 | 76.098 | 1.00x |
| mixed.json | msgspec | 0.093 | 0.095 | 0.102 | 76.098 | 0.78x |
| mixed.json | ujson | 0.304 | 0.313 | 0.323 | 76.098 | 0.23x |
| mixed.json | json | 0.519 | 0.530 | 0.555 | 76.098 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 12.799 | 13.476 | 15.458 | 72.012 | 1.00x |
| users.json | orjson | 15.475 | 16.723 | 18.330 | 72.012 | 0.81x |
| users.json | msgspec | 15.818 | 16.317 | 17.404 | 72.012 | 0.83x |
| users.json | ujson | 22.507 | 23.942 | 28.120 | 72.012 | 0.56x |
| users.json | json | 22.971 | 24.502 | 26.291 | 72.012 | 0.55x |
| flat.json | strata | 0.969 | 1.040 | 1.071 | 72.035 | 1.00x |
| flat.json | orjson | 1.171 | 1.224 | 1.264 | 72.035 | 0.85x |
| flat.json | msgspec | 1.182 | 1.218 | 1.249 | 72.035 | 0.85x |
| flat.json | ujson | 1.790 | 1.940 | 2.005 | 72.035 | 0.54x |
| flat.json | json | 1.843 | 1.874 | 1.935 | 72.035 | 0.56x |
| nested.json | strata | 0.874 | 0.919 | 1.003 | 72.035 | 1.00x |
| nested.json | orjson | 1.129 | 1.166 | 1.208 | 72.035 | 0.79x |
| nested.json | msgspec | 1.077 | 1.110 | 1.167 | 72.035 | 0.83x |
| nested.json | ujson | 1.581 | 1.648 | 1.718 | 72.035 | 0.56x |
| nested.json | json | 1.913 | 1.994 | 2.053 | 72.035 | 0.46x |
| wide_arrays.json | strata | 5.265 | 5.357 | 5.514 | 76.098 | 1.00x |
| wide_arrays.json | orjson | 5.879 | 6.111 | 6.381 | 76.098 | 0.88x |
| wide_arrays.json | msgspec | 6.372 | 6.642 | 7.413 | 76.098 | 0.81x |
| wide_arrays.json | ujson | 8.402 | 8.533 | 9.340 | 76.098 | 0.63x |
| wide_arrays.json | json | 10.239 | 10.476 | 10.825 | 76.098 | 0.51x |
| mixed.json | strata | 0.223 | 0.233 | 0.273 | 76.098 | 1.00x |
| mixed.json | orjson | 0.297 | 0.308 | 0.341 | 76.098 | 0.76x |
| mixed.json | msgspec | 0.294 | 0.312 | 0.335 | 76.098 | 0.75x |
| mixed.json | ujson | 0.393 | 0.408 | 0.439 | 76.098 | 0.57x |
| mixed.json | json | 0.493 | 0.519 | 0.560 | 76.098 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 12.798 | 13.440 | 15.269 | 72.035 | 1.00x |
| users.ndjson | orjson | 19.903 | 20.607 | 22.798 | 72.035 | 0.65x |
| users.ndjson | msgspec | 19.422 | 20.442 | 22.814 | 72.035 | 0.66x |
| users.ndjson | ujson | 24.951 | 26.013 | 27.036 | 72.035 | 0.52x |
| users.ndjson | json | 31.645 | 32.004 | 33.326 | 72.035 | 0.42x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.235 | 3.490 | 4.204 | 68.027 | 1.00x |
| users.json | orjson | 3.919 | 4.258 | 68.288 | 68.027 | 0.82x |
| users.json | msgspec | 4.976 | 5.184 | 25.979 | 68.027 | 0.67x |
| users.json | ujson | 16.019 | 16.643 | 97.116 | 68.027 | 0.21x |
| users.json | json | 22.825 | 23.367 | 134.287 | 68.027 | 0.15x |
| flat.json | strata | 0.489 | 0.499 | 0.580 | 72.035 | 1.00x |
| flat.json | orjson | 0.550 | 0.583 | 0.678 | 72.035 | 0.86x |
| flat.json | msgspec | 0.651 | 0.688 | 0.754 | 72.035 | 0.73x |
| flat.json | ujson | 1.461 | 1.502 | 1.565 | 72.035 | 0.33x |
| flat.json | json | 2.095 | 2.139 | 2.542 | 72.035 | 0.23x |
| nested.json | strata | 0.385 | 0.407 | 0.445 | 72.035 | 1.00x |
| nested.json | orjson | 0.467 | 0.502 | 0.523 | 72.035 | 0.81x |
| nested.json | msgspec | 0.577 | 0.613 | 0.644 | 72.035 | 0.66x |
| nested.json | ujson | 1.530 | 1.589 | 1.617 | 72.035 | 0.26x |
| nested.json | json | 2.540 | 2.577 | 2.632 | 72.035 | 0.16x |
| wide_arrays.json | strata | 2.184 | 2.257 | 2.481 | 76.098 | 1.00x |
| wide_arrays.json | orjson | 2.423 | 2.480 | 2.519 | 76.098 | 0.91x |
| wide_arrays.json | msgspec | 3.470 | 3.517 | 3.592 | 76.098 | 0.64x |
| wide_arrays.json | ujson | 9.164 | 9.273 | 9.431 | 76.098 | 0.24x |
| wide_arrays.json | json | 17.232 | 17.327 | 17.781 | 76.098 | 0.13x |
| mixed.json | strata | 0.168 | 0.178 | 0.201 | 76.098 | 1.00x |
| mixed.json | orjson | 0.183 | 0.192 | 0.239 | 76.098 | 0.93x |
| mixed.json | msgspec | 0.206 | 0.211 | 0.260 | 76.098 | 0.84x |
| mixed.json | ujson | 0.454 | 0.465 | 0.583 | 76.098 | 0.38x |
| mixed.json | json | 0.645 | 0.662 | 0.821 | 76.098 | 0.27x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.080 | 0.083 | 0.104 | 68.027 | 1.00x |
| users.json $[*].id | jmespath | 0.477 | 0.496 | 0.510 | 68.027 | 0.17x |
| users.json $[*].id | jsonpath-ng | 2.947 | 3.127 | 3.294 | 68.027 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.446 | 0.474 | 0.549 | 69.035 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.012 | 3.065 | 3.237 | 69.035 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 20.746 | 21.423 | 22.920 | 69.035 | 0.02x |
| users.json $..total | strata | 1.847 | 1.944 | 3.242 | 74.648 | 1.00x |
| users.json $..total | jsonpath-ng | 391.437 | 396.380 | 412.767 | 74.648 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.278 | 3.290 | 3.319 | 69.035 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.535 | 16.214 | 17.017 | 69.035 | 0.20x |
| users.json $[*].id | orjson+jsonpath-ng | 18.058 | 18.927 | 20.284 | 69.035 | 0.17x |
| users.json $[*].orders[*].total | strata | 3.583 | 3.616 | 3.655 | 71.059 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 19.923 | 20.653 | 21.336 | 71.059 | 0.18x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 44.197 | 45.275 | 47.922 | 71.059 | 0.08x |
| users.json $..total | strata | 15.376 | 17.324 | 18.863 | 74.648 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 418.093 | 423.499 | 447.067 | 74.648 | 0.04x |

