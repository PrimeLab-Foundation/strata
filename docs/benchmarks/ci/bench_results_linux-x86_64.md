# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c3d92f5
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
| users.json | strata | 10.896 | 11.445 | 26.150 | 60.293 | 1.00x |
| users.json | orjson | 14.318 | 15.344 | 28.548 | 60.293 | 0.75x |
| users.json | msgspec | 13.951 | 14.757 | 26.206 | 60.293 | 0.78x |
| users.json | ujson | 19.623 | 22.135 | 29.985 | 60.293 | 0.52x |
| users.json | pysimdjson | 21.024 | 22.684 | 26.474 | 60.293 | 0.50x |
| users.json | json | 21.820 | 22.645 | 23.746 | 60.293 | 0.51x |
| flat.json | strata | 0.888 | 0.902 | 0.921 | 70.492 | 1.00x |
| flat.json | orjson | 1.058 | 1.065 | 1.095 | 70.492 | 0.85x |
| flat.json | msgspec | 1.041 | 1.056 | 1.102 | 70.492 | 0.85x |
| flat.json | ujson | 1.517 | 1.554 | 1.584 | 70.492 | 0.58x |
| flat.json | pysimdjson | 1.630 | 1.680 | 1.687 | 70.492 | 0.54x |
| flat.json | json | 1.718 | 1.746 | 1.762 | 70.492 | 0.52x |
| nested.json | strata | 0.802 | 0.812 | 1.099 | 70.492 | 1.00x |
| nested.json | orjson | 0.995 | 1.002 | 1.016 | 70.492 | 0.81x |
| nested.json | msgspec | 0.984 | 0.997 | 1.032 | 70.492 | 0.81x |
| nested.json | ujson | 1.432 | 1.448 | 1.459 | 70.492 | 0.56x |
| nested.json | pysimdjson | 1.382 | 1.416 | 1.431 | 70.492 | 0.57x |
| nested.json | json | 1.821 | 1.832 | 1.884 | 70.492 | 0.44x |
| wide_arrays.json | strata | 5.005 | 5.081 | 5.102 | 75.711 | 1.00x |
| wide_arrays.json | orjson | 5.531 | 5.670 | 5.756 | 75.711 | 0.90x |
| wide_arrays.json | msgspec | 6.069 | 6.154 | 6.226 | 75.711 | 0.83x |
| wide_arrays.json | ujson | 7.771 | 7.847 | 7.918 | 75.711 | 0.65x |
| wide_arrays.json | pysimdjson | 6.549 | 6.621 | 6.783 | 75.711 | 0.77x |
| wide_arrays.json | json | 9.808 | 9.911 | 10.051 | 75.711 | 0.51x |
| mixed.json | strata | 0.198 | 0.207 | 0.219 | 75.773 | 1.00x |
| mixed.json | orjson | 0.246 | 0.259 | 0.282 | 75.773 | 0.80x |
| mixed.json | msgspec | 0.251 | 0.262 | 0.421 | 75.773 | 0.79x |
| mixed.json | ujson | 0.321 | 0.343 | 0.357 | 75.773 | 0.60x |
| mixed.json | pysimdjson | 0.308 | 0.330 | 0.340 | 75.773 | 0.63x |
| mixed.json | json | 0.450 | 0.475 | 0.506 | 75.773 | 0.44x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.523 | 2.588 | 2.730 | 59.387 | 1.00x |
| users.json | orjson | 3.101 | 3.153 | 3.231 | 59.387 | 0.82x |
| users.json | msgspec | 4.190 | 4.237 | 4.614 | 59.387 | 0.61x |
| users.json | ujson | 14.707 | 14.926 | 15.211 | 59.387 | 0.17x |
| users.json | json | 21.368 | 21.637 | 22.456 | 59.387 | 0.12x |
| flat.json | strata | 0.290 | 0.293 | 0.306 | 70.492 | 1.00x |
| flat.json | orjson | 0.357 | 0.358 | 0.370 | 70.492 | 0.82x |
| flat.json | msgspec | 0.473 | 0.478 | 0.498 | 70.492 | 0.61x |
| flat.json | ujson | 1.252 | 1.261 | 1.302 | 70.492 | 0.23x |
| flat.json | json | 1.829 | 1.852 | 1.868 | 70.492 | 0.16x |
| nested.json | strata | 0.240 | 0.243 | 0.303 | 70.492 | 1.00x |
| nested.json | orjson | 0.301 | 0.318 | 0.359 | 70.492 | 0.77x |
| nested.json | msgspec | 0.416 | 0.425 | 0.435 | 70.492 | 0.57x |
| nested.json | ujson | 1.342 | 1.349 | 1.390 | 70.492 | 0.18x |
| nested.json | json | 2.314 | 2.328 | 2.575 | 70.492 | 0.10x |
| wide_arrays.json | strata | 1.723 | 1.754 | 1.923 | 75.711 | 1.00x |
| wide_arrays.json | orjson | 1.882 | 1.894 | 2.104 | 75.711 | 0.93x |
| wide_arrays.json | msgspec | 2.988 | 3.000 | 3.055 | 75.711 | 0.58x |
| wide_arrays.json | ujson | 8.499 | 8.561 | 8.696 | 75.711 | 0.20x |
| wide_arrays.json | json | 16.460 | 16.620 | 17.003 | 75.711 | 0.11x |
| mixed.json | strata | 0.066 | 0.070 | 0.086 | 75.773 | 1.00x |
| mixed.json | orjson | 0.069 | 0.071 | 0.074 | 75.773 | 0.98x |
| mixed.json | msgspec | 0.087 | 0.089 | 0.102 | 75.773 | 0.78x |
| mixed.json | ujson | 0.301 | 0.304 | 0.339 | 75.773 | 0.23x |
| mixed.json | json | 0.501 | 0.519 | 0.530 | 75.773 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.859 | 12.442 | 14.492 | 75.121 | 1.00x |
| users.json | orjson | 14.852 | 15.695 | 16.490 | 75.121 | 0.79x |
| users.json | msgspec | 15.133 | 15.664 | 16.369 | 75.121 | 0.79x |
| users.json | ujson | 20.868 | 21.602 | 23.365 | 75.121 | 0.58x |
| users.json | json | 22.389 | 23.049 | 23.555 | 75.121 | 0.54x |
| flat.json | strata | 0.926 | 0.945 | 0.977 | 70.492 | 1.00x |
| flat.json | orjson | 1.122 | 1.130 | 1.154 | 70.492 | 0.84x |
| flat.json | msgspec | 1.111 | 1.126 | 1.169 | 70.492 | 0.84x |
| flat.json | ujson | 1.621 | 1.647 | 1.727 | 70.492 | 0.57x |
| flat.json | json | 1.804 | 1.812 | 1.823 | 70.492 | 0.52x |
| nested.json | strata | 0.829 | 0.841 | 0.849 | 70.492 | 1.00x |
| nested.json | orjson | 1.063 | 1.067 | 1.095 | 70.492 | 0.79x |
| nested.json | msgspec | 1.032 | 1.040 | 1.058 | 70.492 | 0.81x |
| nested.json | ujson | 1.493 | 1.514 | 1.534 | 70.492 | 0.56x |
| nested.json | json | 1.870 | 1.890 | 1.914 | 70.492 | 0.44x |
| wide_arrays.json | strata | 5.106 | 5.222 | 5.252 | 75.773 | 1.00x |
| wide_arrays.json | orjson | 5.825 | 6.001 | 6.156 | 75.773 | 0.87x |
| wide_arrays.json | msgspec | 6.433 | 6.590 | 6.708 | 75.773 | 0.79x |
| wide_arrays.json | ujson | 8.067 | 8.356 | 8.595 | 75.773 | 0.62x |
| wide_arrays.json | json | 10.034 | 10.265 | 10.663 | 75.773 | 0.51x |
| mixed.json | strata | 0.223 | 0.228 | 0.237 | 75.773 | 1.00x |
| mixed.json | orjson | 0.296 | 0.304 | 0.323 | 75.773 | 0.75x |
| mixed.json | msgspec | 0.293 | 0.301 | 0.320 | 75.773 | 0.76x |
| mixed.json | ujson | 0.382 | 0.393 | 0.418 | 75.773 | 0.58x |
| mixed.json | json | 0.497 | 0.519 | 0.537 | 75.773 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 11.647 | 12.450 | 13.162 | 70.492 | 1.00x |
| users.ndjson | orjson | 18.627 | 19.135 | 19.968 | 70.492 | 0.65x |
| users.ndjson | msgspec | 18.421 | 18.978 | 19.786 | 70.492 | 0.66x |
| users.ndjson | ujson | 23.146 | 24.333 | 25.071 | 70.492 | 0.51x |
| users.ndjson | json | 29.558 | 30.649 | 31.287 | 70.492 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.242 | 3.323 | 174.209 | 72.121 | 1.00x |
| users.json | orjson | 3.845 | 4.033 | 55.992 | 72.121 | 0.82x |
| users.json | msgspec | 4.887 | 5.001 | 5.116 | 72.121 | 0.66x |
| users.json | ujson | 15.625 | 15.790 | 17.197 | 72.121 | 0.21x |
| users.json | json | 22.886 | 23.069 | 23.617 | 72.121 | 0.14x |
| flat.json | strata | 0.448 | 0.471 | 0.485 | 70.492 | 1.00x |
| flat.json | orjson | 0.525 | 0.539 | 0.559 | 70.492 | 0.87x |
| flat.json | msgspec | 0.649 | 0.656 | 0.674 | 70.492 | 0.72x |
| flat.json | ujson | 1.451 | 1.467 | 1.493 | 70.492 | 0.32x |
| flat.json | json | 2.030 | 2.061 | 2.103 | 70.492 | 0.23x |
| nested.json | strata | 0.362 | 0.380 | 0.413 | 70.492 | 1.00x |
| nested.json | orjson | 0.465 | 0.477 | 0.499 | 70.492 | 0.80x |
| nested.json | msgspec | 0.578 | 0.588 | 0.646 | 70.492 | 0.65x |
| nested.json | ujson | 1.518 | 1.541 | 1.551 | 70.492 | 0.25x |
| nested.json | json | 2.474 | 2.516 | 2.546 | 70.492 | 0.15x |
| wide_arrays.json | strata | 2.189 | 2.282 | 2.572 | 75.773 | 1.00x |
| wide_arrays.json | orjson | 2.362 | 2.414 | 2.517 | 75.773 | 0.95x |
| wide_arrays.json | msgspec | 3.454 | 3.496 | 3.541 | 75.773 | 0.65x |
| wide_arrays.json | ujson | 9.125 | 9.250 | 9.575 | 75.773 | 0.25x |
| wide_arrays.json | json | 17.239 | 17.322 | 17.748 | 75.773 | 0.13x |
| mixed.json | strata | 0.166 | 0.176 | 0.192 | 75.773 | 1.00x |
| mixed.json | orjson | 0.179 | 0.189 | 0.200 | 75.773 | 0.93x |
| mixed.json | msgspec | 0.198 | 0.211 | 0.231 | 75.773 | 0.83x |
| mixed.json | ujson | 0.433 | 0.447 | 0.487 | 75.773 | 0.39x |
| mixed.json | json | 0.645 | 0.659 | 0.801 | 75.773 | 0.27x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.078 | 0.084 | 0.090 | 72.121 | 1.00x |
| users.json $[*].id | jmespath | 0.481 | 0.498 | 0.543 | 72.121 | 0.17x |
| users.json $[*].id | jsonpath-ng | 2.830 | 3.134 | 3.603 | 72.121 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.452 | 0.480 | 0.574 | 72.121 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.044 | 3.127 | 3.215 | 72.121 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 20.746 | 21.510 | 23.301 | 72.121 | 0.02x |
| users.json $..total | strata | 1.813 | 1.922 | 2.023 | 72.965 | 1.00x |
| users.json $..total | jsonpath-ng | 387.910 | 393.797 | 406.593 | 72.965 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.249 | 3.276 | 3.340 | 72.121 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.637 | 16.049 | 16.572 | 72.121 | 0.20x |
| users.json $[*].id | orjson+jsonpath-ng | 17.973 | 18.438 | 19.639 | 72.121 | 0.18x |
| users.json $[*].orders[*].total | strata | 3.571 | 3.588 | 3.629 | 72.965 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.845 | 19.191 | 20.524 | 72.965 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 40.984 | 41.914 | 45.613 | 72.965 | 0.09x |
| users.json $..total | strata | 14.766 | 15.568 | 18.070 | 72.984 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 414.470 | 416.242 | 424.729 | 72.984 | 0.04x |

