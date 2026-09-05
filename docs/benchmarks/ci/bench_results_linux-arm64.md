# Benchmark results - ci-linux-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: b7f31bb
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-aarch64-with-glibc2.39
- machine: aarch64
- processor: aarch64
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.830 | 8.885 | 10.643 | 51.836 | 1.00x |
| users.json | orjson | 11.497 | 11.585 | 12.931 | 51.836 | 0.77x |
| users.json | msgspec | 11.999 | 12.072 | 13.343 | 51.836 | 0.74x |
| users.json | ujson | 16.246 | 16.369 | 18.539 | 51.836 | 0.54x |
| users.json | pysimdjson | 16.274 | 16.439 | 18.230 | 51.836 | 0.54x |
| users.json | json | 20.336 | 20.450 | 21.100 | 51.836 | 0.43x |
| flat.json | strata | 0.808 | 0.821 | 0.837 | 58.703 | 1.00x |
| flat.json | orjson | 0.859 | 0.878 | 0.886 | 58.703 | 0.93x |
| flat.json | msgspec | 0.906 | 0.922 | 0.942 | 58.703 | 0.89x |
| flat.json | ujson | 1.417 | 1.443 | 1.463 | 58.703 | 0.57x |
| flat.json | pysimdjson | 1.465 | 1.470 | 1.483 | 58.703 | 0.56x |
| flat.json | json | 1.776 | 1.789 | 1.795 | 58.703 | 0.46x |
| nested.json | strata | 0.793 | 0.815 | 0.820 | 58.703 | 1.00x |
| nested.json | orjson | 0.867 | 0.888 | 0.900 | 58.703 | 0.92x |
| nested.json | msgspec | 0.997 | 1.007 | 1.016 | 58.703 | 0.81x |
| nested.json | ujson | 1.388 | 1.423 | 1.450 | 58.703 | 0.57x |
| nested.json | pysimdjson | 1.395 | 1.403 | 1.409 | 58.703 | 0.58x |
| nested.json | json | 1.965 | 1.987 | 2.006 | 58.703 | 0.41x |
| wide_arrays.json | strata | 4.367 | 4.384 | 4.452 | 60.273 | 1.00x |
| wide_arrays.json | orjson | 4.018 | 4.069 | 4.129 | 60.273 | 1.08x |
| wide_arrays.json | msgspec | 4.993 | 5.026 | 5.096 | 60.273 | 0.87x |
| wide_arrays.json | ujson | 6.465 | 6.497 | 6.533 | 60.273 | 0.67x |
| wide_arrays.json | pysimdjson | 5.226 | 5.272 | 5.314 | 60.273 | 0.83x |
| wide_arrays.json | json | 9.478 | 9.544 | 9.658 | 60.273 | 0.46x |
| mixed.json | strata | 0.198 | 0.199 | 0.234 | 60.273 | 1.00x |
| mixed.json | orjson | 0.213 | 0.223 | 0.237 | 60.273 | 0.89x |
| mixed.json | msgspec | 0.241 | 0.242 | 0.266 | 60.273 | 0.82x |
| mixed.json | ujson | 0.311 | 0.318 | 0.367 | 60.273 | 0.62x |
| mixed.json | pysimdjson | 0.293 | 0.298 | 0.319 | 60.273 | 0.67x |
| mixed.json | json | 0.456 | 0.465 | 0.482 | 60.273 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.057 | 2.065 | 2.097 | 50.934 | 1.00x |
| users.json | orjson | 2.575 | 2.592 | 2.618 | 50.934 | 0.80x |
| users.json | msgspec | 3.317 | 3.326 | 3.348 | 50.934 | 0.62x |
| users.json | ujson | 10.543 | 10.578 | 10.651 | 50.934 | 0.20x |
| users.json | json | 18.982 | 19.033 | 19.122 | 50.934 | 0.11x |
| flat.json | strata | 0.229 | 0.232 | 0.248 | 58.703 | 1.00x |
| flat.json | orjson | 0.300 | 0.302 | 0.325 | 58.703 | 0.77x |
| flat.json | msgspec | 0.384 | 0.397 | 0.408 | 58.703 | 0.58x |
| flat.json | ujson | 0.995 | 1.001 | 1.006 | 58.703 | 0.23x |
| flat.json | json | 1.692 | 1.714 | 1.740 | 58.703 | 0.14x |
| nested.json | strata | 0.230 | 0.236 | 0.262 | 58.707 | 1.00x |
| nested.json | orjson | 0.284 | 0.288 | 0.310 | 58.707 | 0.82x |
| nested.json | msgspec | 0.368 | 0.372 | 0.398 | 58.707 | 0.63x |
| nested.json | ujson | 1.081 | 1.088 | 1.105 | 58.707 | 0.22x |
| nested.json | json | 2.163 | 2.190 | 2.217 | 58.707 | 0.11x |
| wide_arrays.json | strata | 1.260 | 1.265 | 1.297 | 60.273 | 1.00x |
| wide_arrays.json | orjson | 1.540 | 1.554 | 1.579 | 60.273 | 0.81x |
| wide_arrays.json | msgspec | 2.296 | 2.315 | 2.338 | 60.273 | 0.55x |
| wide_arrays.json | ujson | 4.718 | 4.732 | 4.757 | 60.273 | 0.27x |
| wide_arrays.json | json | 13.474 | 13.524 | 13.544 | 60.273 | 0.09x |
| mixed.json | strata | 0.068 | 0.070 | 0.072 | 60.273 | 1.00x |
| mixed.json | orjson | 0.066 | 0.068 | 0.071 | 60.273 | 1.03x |
| mixed.json | msgspec | 0.079 | 0.081 | 0.105 | 60.273 | 0.87x |
| mixed.json | ujson | 0.243 | 0.248 | 0.260 | 60.273 | 0.28x |
| mixed.json | json | 0.482 | 0.498 | 0.511 | 60.273 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.029 | 9.226 | 9.887 | 63.102 | 1.00x |
| users.json | orjson | 11.600 | 11.957 | 12.264 | 63.102 | 0.77x |
| users.json | msgspec | 12.256 | 12.487 | 12.582 | 63.102 | 0.74x |
| users.json | ujson | 16.878 | 17.168 | 17.868 | 63.102 | 0.54x |
| users.json | json | 20.711 | 20.920 | 21.137 | 63.102 | 0.44x |
| flat.json | strata | 0.833 | 0.846 | 0.859 | 58.703 | 1.00x |
| flat.json | orjson | 0.938 | 0.943 | 0.952 | 58.703 | 0.90x |
| flat.json | msgspec | 0.983 | 0.991 | 1.001 | 58.703 | 0.85x |
| flat.json | ujson | 1.513 | 1.526 | 1.544 | 58.703 | 0.55x |
| flat.json | json | 1.837 | 1.845 | 1.860 | 58.703 | 0.46x |
| nested.json | strata | 0.839 | 0.852 | 0.867 | 58.707 | 1.00x |
| nested.json | orjson | 0.951 | 0.961 | 0.976 | 58.707 | 0.89x |
| nested.json | msgspec | 1.063 | 1.075 | 1.093 | 58.707 | 0.79x |
| nested.json | ujson | 1.486 | 1.502 | 1.516 | 58.707 | 0.57x |
| nested.json | json | 2.021 | 2.030 | 2.085 | 58.707 | 0.42x |
| wide_arrays.json | strata | 4.429 | 4.456 | 4.484 | 60.273 | 1.00x |
| wide_arrays.json | orjson | 4.105 | 4.167 | 4.215 | 60.273 | 1.07x |
| wide_arrays.json | msgspec | 5.085 | 5.155 | 5.236 | 60.273 | 0.86x |
| wide_arrays.json | ujson | 6.703 | 6.766 | 6.828 | 60.273 | 0.66x |
| wide_arrays.json | json | 9.641 | 9.671 | 9.730 | 60.273 | 0.46x |
| mixed.json | strata | 0.225 | 0.227 | 0.255 | 60.273 | 1.00x |
| mixed.json | orjson | 0.280 | 0.288 | 0.311 | 60.273 | 0.79x |
| mixed.json | msgspec | 0.297 | 0.303 | 0.316 | 60.273 | 0.75x |
| mixed.json | ujson | 0.385 | 0.399 | 0.416 | 60.273 | 0.57x |
| mixed.json | json | 0.512 | 0.528 | 0.539 | 60.273 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.431 | 9.663 | 9.949 | 58.695 | 1.00x |
| users.ndjson | orjson | 14.526 | 14.851 | 15.146 | 58.695 | 0.65x |
| users.ndjson | msgspec | 14.882 | 15.055 | 15.312 | 58.695 | 0.64x |
| users.ndjson | ujson | 19.389 | 19.616 | 19.824 | 58.695 | 0.49x |
| users.ndjson | json | 25.483 | 25.933 | 26.415 | 58.695 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.505 | 2.526 | 2.661 | 59.102 | 1.00x |
| users.json | orjson | 3.041 | 3.091 | 3.181 | 59.102 | 0.82x |
| users.json | msgspec | 3.766 | 3.820 | 3.885 | 59.102 | 0.66x |
| users.json | ujson | 11.128 | 11.179 | 11.334 | 59.102 | 0.23x |
| users.json | json | 19.499 | 19.957 | 20.225 | 59.102 | 0.13x |
| flat.json | strata | 0.376 | 0.385 | 0.420 | 58.703 | 1.00x |
| flat.json | orjson | 0.475 | 0.493 | 0.510 | 58.703 | 0.78x |
| flat.json | msgspec | 0.562 | 0.583 | 0.598 | 58.703 | 0.66x |
| flat.json | ujson | 1.187 | 1.197 | 1.228 | 58.703 | 0.32x |
| flat.json | json | 1.866 | 1.907 | 1.927 | 58.703 | 0.20x |
| nested.json | strata | 0.381 | 0.389 | 0.408 | 58.707 | 1.00x |
| nested.json | orjson | 0.456 | 0.467 | 0.479 | 58.707 | 0.83x |
| nested.json | msgspec | 0.536 | 0.553 | 0.583 | 58.707 | 0.70x |
| nested.json | ujson | 1.280 | 1.302 | 1.326 | 58.707 | 0.30x |
| nested.json | json | 2.330 | 2.358 | 2.422 | 58.707 | 0.17x |
| wide_arrays.json | strata | 1.632 | 1.661 | 1.726 | 60.273 | 1.00x |
| wide_arrays.json | orjson | 1.952 | 1.972 | 2.027 | 60.273 | 0.84x |
| wide_arrays.json | msgspec | 2.705 | 2.736 | 2.780 | 60.273 | 0.61x |
| wide_arrays.json | ujson | 5.167 | 5.212 | 5.236 | 60.273 | 0.32x |
| wide_arrays.json | json | 14.022 | 14.078 | 14.166 | 60.273 | 0.12x |
| mixed.json | strata | 0.178 | 0.182 | 0.196 | 60.273 | 1.00x |
| mixed.json | orjson | 0.188 | 0.195 | 0.209 | 60.273 | 0.94x |
| mixed.json | msgspec | 0.201 | 0.208 | 0.222 | 60.273 | 0.88x |
| mixed.json | ujson | 0.383 | 0.396 | 0.432 | 60.273 | 0.46x |
| mixed.json | json | 0.619 | 0.635 | 0.661 | 60.273 | 0.29x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.099 | 0.103 | 0.104 | 59.102 | 1.00x |
| users.json $[*].id | jmespath | 0.468 | 0.474 | 0.495 | 59.102 | 0.22x |
| users.json $[*].id | jsonpath-ng | 2.444 | 2.490 | 2.534 | 59.102 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.614 | 0.625 | 0.633 | 59.230 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.980 | 2.994 | 3.010 | 59.230 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.353 | 17.603 | 17.854 | 59.230 | 0.04x |
| users.json $..total | strata | 1.707 | 1.724 | 1.780 | 62.113 | 1.00x |
| users.json $..total | jsonpath-ng | 292.722 | 293.082 | 294.758 | 62.113 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.305 | 3.327 | 3.350 | 59.230 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.209 | 12.262 | 12.364 | 59.230 | 0.27x |
| users.json $[*].id | orjson+jsonpath-ng | 14.033 | 14.151 | 14.336 | 59.230 | 0.24x |
| users.json $[*].orders[*].total | strata | 3.480 | 3.521 | 3.543 | 62.113 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 14.690 | 15.007 | 15.379 | 62.113 | 0.23x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 32.990 | 33.230 | 33.951 | 62.113 | 0.11x |
| users.json $..total | strata | 11.766 | 11.915 | 12.311 | 62.188 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 314.190 | 315.089 | 316.655 | 62.188 | 0.04x |

