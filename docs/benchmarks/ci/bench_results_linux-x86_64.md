# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: e6c5e36
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
| users.json | strata | 10.327 | 10.712 | 14.723 | 58.598 | 1.00x |
| users.json | orjson | 14.187 | 14.607 | 17.473 | 58.598 | 0.73x |
| users.json | msgspec | 13.990 | 14.545 | 17.121 | 58.598 | 0.74x |
| users.json | ujson | 18.728 | 19.199 | 24.375 | 58.598 | 0.56x |
| users.json | pysimdjson | 20.000 | 21.153 | 25.034 | 58.598 | 0.51x |
| users.json | json | 21.611 | 21.848 | 30.128 | 58.598 | 0.49x |
| flat.json | strata | 0.881 | 0.886 | 0.923 | 70.453 | 1.00x |
| flat.json | orjson | 1.046 | 1.052 | 1.093 | 70.453 | 0.84x |
| flat.json | msgspec | 1.048 | 1.055 | 1.269 | 70.453 | 0.84x |
| flat.json | ujson | 1.509 | 1.529 | 1.554 | 70.453 | 0.58x |
| flat.json | pysimdjson | 1.647 | 1.665 | 1.686 | 70.453 | 0.53x |
| flat.json | json | 1.726 | 1.739 | 1.765 | 70.453 | 0.51x |
| nested.json | strata | 0.785 | 0.803 | 0.854 | 70.453 | 1.00x |
| nested.json | orjson | 0.997 | 1.014 | 1.039 | 70.453 | 0.79x |
| nested.json | msgspec | 0.985 | 0.998 | 1.006 | 70.453 | 0.81x |
| nested.json | ujson | 1.391 | 1.417 | 1.502 | 70.453 | 0.57x |
| nested.json | pysimdjson | 1.383 | 1.409 | 1.469 | 70.453 | 0.57x |
| nested.json | json | 1.814 | 1.837 | 1.883 | 70.453 | 0.44x |
| wide_arrays.json | strata | 4.940 | 5.032 | 5.115 | 75.801 | 1.00x |
| wide_arrays.json | orjson | 5.561 | 5.607 | 5.701 | 75.801 | 0.90x |
| wide_arrays.json | msgspec | 6.102 | 6.179 | 6.550 | 75.801 | 0.81x |
| wide_arrays.json | ujson | 7.724 | 7.838 | 8.156 | 75.801 | 0.64x |
| wide_arrays.json | pysimdjson | 6.421 | 6.550 | 6.842 | 75.801 | 0.77x |
| wide_arrays.json | json | 10.014 | 10.159 | 10.241 | 75.801 | 0.50x |
| mixed.json | strata | 0.191 | 0.196 | 0.200 | 75.863 | 1.00x |
| mixed.json | orjson | 0.236 | 0.241 | 0.252 | 75.863 | 0.81x |
| mixed.json | msgspec | 0.243 | 0.248 | 0.283 | 75.863 | 0.79x |
| mixed.json | ujson | 0.302 | 0.308 | 0.321 | 75.863 | 0.64x |
| mixed.json | pysimdjson | 0.294 | 0.301 | 0.313 | 75.863 | 0.65x |
| mixed.json | json | 0.448 | 0.458 | 0.468 | 75.863 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.503 | 2.519 | 2.595 | 56.988 | 1.00x |
| users.json | orjson | 3.075 | 3.102 | 3.147 | 56.988 | 0.81x |
| users.json | msgspec | 4.140 | 4.168 | 4.245 | 56.988 | 0.60x |
| users.json | ujson | 14.454 | 14.579 | 16.606 | 56.988 | 0.17x |
| users.json | json | 21.218 | 21.492 | 21.842 | 56.988 | 0.12x |
| flat.json | strata | 0.289 | 0.296 | 0.314 | 70.453 | 1.00x |
| flat.json | orjson | 0.358 | 0.361 | 0.400 | 70.453 | 0.82x |
| flat.json | msgspec | 0.473 | 0.482 | 0.495 | 70.453 | 0.61x |
| flat.json | ujson | 1.265 | 1.317 | 1.375 | 70.453 | 0.22x |
| flat.json | json | 1.826 | 1.856 | 1.892 | 70.453 | 0.16x |
| nested.json | strata | 0.240 | 0.250 | 0.260 | 70.453 | 1.00x |
| nested.json | orjson | 0.297 | 0.303 | 0.411 | 70.453 | 0.83x |
| nested.json | msgspec | 0.413 | 0.420 | 0.529 | 70.453 | 0.60x |
| nested.json | ujson | 1.334 | 1.347 | 1.395 | 70.453 | 0.19x |
| nested.json | json | 2.302 | 2.329 | 2.517 | 70.453 | 0.11x |
| wide_arrays.json | strata | 1.697 | 1.711 | 1.736 | 75.801 | 1.00x |
| wide_arrays.json | orjson | 1.901 | 1.912 | 1.944 | 75.801 | 0.89x |
| wide_arrays.json | msgspec | 2.959 | 2.971 | 3.130 | 75.801 | 0.58x |
| wide_arrays.json | ujson | 8.503 | 8.562 | 8.770 | 75.801 | 0.20x |
| wide_arrays.json | json | 16.584 | 16.648 | 16.804 | 75.801 | 0.10x |
| mixed.json | strata | 0.066 | 0.070 | 0.090 | 75.863 | 1.00x |
| mixed.json | orjson | 0.068 | 0.071 | 0.087 | 75.863 | 0.98x |
| mixed.json | msgspec | 0.086 | 0.090 | 0.103 | 75.863 | 0.78x |
| mixed.json | ujson | 0.300 | 0.305 | 0.314 | 75.863 | 0.23x |
| mixed.json | json | 0.502 | 0.516 | 0.552 | 75.863 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.503 | 11.932 | 13.193 | 73.055 | 1.00x |
| users.json | orjson | 15.028 | 15.500 | 15.975 | 73.055 | 0.77x |
| users.json | msgspec | 15.197 | 15.449 | 15.808 | 73.055 | 0.77x |
| users.json | ujson | 19.941 | 21.197 | 22.731 | 73.055 | 0.56x |
| users.json | json | 21.889 | 22.586 | 23.764 | 73.055 | 0.53x |
| flat.json | strata | 0.894 | 0.914 | 0.928 | 70.453 | 1.00x |
| flat.json | orjson | 1.104 | 1.114 | 1.131 | 70.453 | 0.82x |
| flat.json | msgspec | 1.104 | 1.112 | 1.151 | 70.453 | 0.82x |
| flat.json | ujson | 1.602 | 1.618 | 1.659 | 70.453 | 0.57x |
| flat.json | json | 1.781 | 1.791 | 1.854 | 70.453 | 0.51x |
| nested.json | strata | 0.824 | 0.844 | 0.894 | 70.453 | 1.00x |
| nested.json | orjson | 1.063 | 1.074 | 1.107 | 70.453 | 0.79x |
| nested.json | msgspec | 1.059 | 1.069 | 1.086 | 70.453 | 0.79x |
| nested.json | ujson | 1.469 | 1.490 | 1.529 | 70.453 | 0.57x |
| nested.json | json | 1.886 | 1.911 | 2.287 | 70.453 | 0.44x |
| wide_arrays.json | strata | 5.016 | 5.067 | 5.269 | 75.863 | 1.00x |
| wide_arrays.json | orjson | 5.577 | 5.635 | 5.815 | 75.863 | 0.90x |
| wide_arrays.json | msgspec | 6.193 | 6.253 | 6.319 | 75.863 | 0.81x |
| wide_arrays.json | ujson | 7.907 | 8.013 | 8.117 | 75.863 | 0.63x |
| wide_arrays.json | json | 10.059 | 10.165 | 10.342 | 75.863 | 0.50x |
| mixed.json | strata | 0.212 | 0.220 | 0.232 | 75.863 | 1.00x |
| mixed.json | orjson | 0.281 | 0.290 | 0.304 | 75.863 | 0.76x |
| mixed.json | msgspec | 0.285 | 0.297 | 0.314 | 75.863 | 0.74x |
| mixed.json | ujson | 0.353 | 0.365 | 0.399 | 75.863 | 0.60x |
| mixed.json | json | 0.502 | 0.512 | 0.540 | 75.863 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.783 | 11.474 | 12.237 | 70.453 | 1.00x |
| users.ndjson | orjson | 17.965 | 18.729 | 19.729 | 70.453 | 0.61x |
| users.ndjson | msgspec | 18.128 | 18.458 | 19.554 | 70.453 | 0.62x |
| users.ndjson | ujson | 22.428 | 23.359 | 25.638 | 70.453 | 0.49x |
| users.ndjson | json | 29.080 | 29.931 | 30.549 | 70.453 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.109 | 3.144 | 3.191 | 71.070 | 1.00x |
| users.json | orjson | 3.703 | 3.750 | 3.889 | 71.070 | 0.84x |
| users.json | msgspec | 4.742 | 4.826 | 4.922 | 71.070 | 0.65x |
| users.json | ujson | 15.264 | 15.400 | 17.674 | 71.070 | 0.20x |
| users.json | json | 22.475 | 22.723 | 23.003 | 71.070 | 0.14x |
| flat.json | strata | 0.442 | 0.457 | 0.495 | 70.453 | 1.00x |
| flat.json | orjson | 0.526 | 0.541 | 0.557 | 70.453 | 0.84x |
| flat.json | msgspec | 0.628 | 0.647 | 0.661 | 70.453 | 0.71x |
| flat.json | ujson | 1.435 | 1.450 | 1.487 | 70.453 | 0.32x |
| flat.json | json | 2.035 | 2.048 | 2.213 | 70.453 | 0.22x |
| nested.json | strata | 0.355 | 0.367 | 0.393 | 70.453 | 1.00x |
| nested.json | orjson | 0.441 | 0.450 | 0.469 | 70.453 | 0.82x |
| nested.json | msgspec | 0.554 | 0.569 | 0.668 | 70.453 | 0.65x |
| nested.json | ujson | 1.487 | 1.514 | 1.611 | 70.453 | 0.24x |
| nested.json | json | 2.472 | 2.500 | 2.532 | 70.453 | 0.15x |
| wide_arrays.json | strata | 2.109 | 2.151 | 2.193 | 75.863 | 1.00x |
| wide_arrays.json | orjson | 2.347 | 2.376 | 2.443 | 75.863 | 0.91x |
| wide_arrays.json | msgspec | 3.366 | 3.422 | 3.478 | 75.863 | 0.63x |
| wide_arrays.json | ujson | 9.065 | 9.160 | 9.273 | 75.863 | 0.23x |
| wide_arrays.json | json | 17.119 | 17.272 | 18.011 | 75.863 | 0.12x |
| mixed.json | strata | 0.159 | 0.165 | 0.260 | 75.863 | 1.00x |
| mixed.json | orjson | 0.175 | 0.188 | 0.294 | 75.863 | 0.87x |
| mixed.json | msgspec | 0.191 | 0.200 | 0.342 | 75.863 | 0.82x |
| mixed.json | ujson | 0.425 | 0.432 | 0.585 | 75.863 | 0.38x |
| mixed.json | json | 0.642 | 0.656 | 0.684 | 75.863 | 0.25x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.073 | 0.077 | 0.079 | 71.070 | 1.00x |
| users.json $[*].id | jmespath | 0.465 | 0.477 | 0.488 | 71.070 | 0.16x |
| users.json $[*].id | jsonpath-ng | 2.809 | 2.913 | 3.046 | 71.070 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.427 | 0.457 | 0.520 | 71.082 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.993 | 3.049 | 3.080 | 71.082 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 19.105 | 19.523 | 19.976 | 71.082 | 0.02x |
| users.json $..total | strata | 1.770 | 1.837 | 2.037 | 72.078 | 1.00x |
| users.json $..total | jsonpath-ng | 389.893 | 391.871 | 401.319 | 72.078 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.220 | 3.239 | 3.549 | 71.082 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.063 | 15.257 | 15.821 | 71.082 | 0.21x |
| users.json $[*].id | orjson+jsonpath-ng | 17.374 | 17.615 | 17.886 | 71.082 | 0.18x |
| users.json $[*].orders[*].total | strata | 3.489 | 3.505 | 3.541 | 72.078 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.236 | 18.397 | 18.948 | 72.078 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 38.217 | 39.102 | 39.518 | 72.078 | 0.09x |
| users.json $..total | strata | 13.196 | 13.789 | 14.277 | 72.082 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 412.840 | 414.945 | 418.331 | 72.082 | 0.03x |

