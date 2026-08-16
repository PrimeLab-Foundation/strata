# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 208e6f9
- python: 3.12.10
- implementation: CPython
- platform: macOS-26.5.2-arm64-arm-64bit
- machine: arm64
- processor: Apple M1 (Virtual)
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.419 | 7.742 | 9.282 | 63.406 | 1.00x |
| users.json | orjson | 9.444 | 9.910 | 10.807 | 63.406 | 0.78x |
| users.json | msgspec | 9.030 | 9.605 | 10.538 | 63.406 | 0.81x |
| users.json | ujson | 11.890 | 13.878 | 14.451 | 63.406 | 0.56x |
| users.json | pysimdjson | 123.479 | 132.422 | 142.555 | 63.406 | 0.06x |
| users.json | json | 14.747 | 15.779 | 17.209 | 63.406 | 0.49x |
| flat.json | strata | 0.667 | 0.670 | 0.714 | 87.109 | 1.00x |
| flat.json | orjson | 0.753 | 0.781 | 0.804 | 87.109 | 0.86x |
| flat.json | msgspec | 0.725 | 0.744 | 0.774 | 87.109 | 0.90x |
| flat.json | ujson | 1.144 | 1.171 | 1.200 | 87.109 | 0.57x |
| flat.json | pysimdjson | 12.035 | 12.101 | 12.342 | 87.109 | 0.06x |
| flat.json | json | 1.350 | 1.387 | 1.491 | 87.109 | 0.48x |
| nested.json | strata | 0.602 | 0.611 | 0.639 | 87.109 | 1.00x |
| nested.json | orjson | 0.747 | 0.776 | 0.805 | 87.109 | 0.79x |
| nested.json | msgspec | 0.700 | 0.706 | 0.719 | 87.109 | 0.87x |
| nested.json | ujson | 1.084 | 1.119 | 1.150 | 87.109 | 0.55x |
| nested.json | pysimdjson | 10.557 | 10.604 | 10.683 | 87.109 | 0.06x |
| nested.json | json | 1.471 | 1.512 | 1.550 | 87.109 | 0.40x |
| wide_arrays.json | strata | 3.481 | 3.644 | 3.885 | 89.688 | 1.00x |
| wide_arrays.json | orjson | 3.659 | 3.944 | 4.187 | 89.688 | 0.92x |
| wide_arrays.json | msgspec | 4.152 | 4.492 | 6.959 | 89.688 | 0.81x |
| wide_arrays.json | ujson | 5.662 | 6.304 | 6.559 | 89.688 | 0.58x |
| wide_arrays.json | pysimdjson | 64.972 | 67.454 | 74.140 | 89.688 | 0.05x |
| wide_arrays.json | json | 6.781 | 7.261 | 7.922 | 89.688 | 0.50x |
| mixed.json | strata | 0.134 | 0.136 | 0.148 | 93.312 | 1.00x |
| mixed.json | orjson | 0.149 | 0.155 | 0.196 | 93.312 | 0.88x |
| mixed.json | msgspec | 0.164 | 0.168 | 0.191 | 93.312 | 0.81x |
| mixed.json | ujson | 0.201 | 0.241 | 0.416 | 93.312 | 0.57x |
| mixed.json | pysimdjson | 2.435 | 2.443 | 2.550 | 93.312 | 0.06x |
| mixed.json | json | 0.312 | 0.318 | 0.333 | 93.312 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.702 | 1.843 | 1.985 | 74.156 | 1.00x |
| users.json | orjson | 2.137 | 2.465 | 2.972 | 74.156 | 0.75x |
| users.json | msgspec | 2.815 | 3.063 | 3.640 | 74.156 | 0.60x |
| users.json | ujson | 10.639 | 11.235 | 11.747 | 74.156 | 0.16x |
| users.json | json | 15.652 | 16.301 | 19.868 | 74.156 | 0.11x |
| flat.json | strata | 0.198 | 0.204 | 0.312 | 87.109 | 1.00x |
| flat.json | orjson | 0.255 | 0.315 | 0.346 | 87.109 | 0.65x |
| flat.json | msgspec | 0.324 | 0.334 | 0.368 | 87.109 | 0.61x |
| flat.json | ujson | 0.950 | 0.969 | 1.184 | 87.109 | 0.21x |
| flat.json | json | 1.387 | 1.445 | 1.600 | 87.109 | 0.14x |
| nested.json | strata | 0.134 | 0.138 | 0.146 | 87.109 | 1.00x |
| nested.json | orjson | 0.230 | 0.238 | 0.320 | 87.109 | 0.58x |
| nested.json | msgspec | 0.290 | 0.295 | 0.322 | 87.109 | 0.47x |
| nested.json | ujson | 1.038 | 1.123 | 1.172 | 87.109 | 0.12x |
| nested.json | json | 1.673 | 1.698 | 1.840 | 87.109 | 0.08x |
| wide_arrays.json | strata | 1.197 | 1.286 | 1.325 | 89.688 | 1.00x |
| wide_arrays.json | orjson | 1.313 | 1.432 | 1.539 | 89.688 | 0.90x |
| wide_arrays.json | msgspec | 2.115 | 2.321 | 2.415 | 89.688 | 0.55x |
| wide_arrays.json | ujson | 6.477 | 6.703 | 6.795 | 89.688 | 0.19x |
| wide_arrays.json | json | 11.499 | 12.104 | 12.242 | 89.688 | 0.11x |
| mixed.json | strata | 0.039 | 0.041 | 0.048 | 93.312 | 1.00x |
| mixed.json | orjson | 0.044 | 0.045 | 0.048 | 93.312 | 0.91x |
| mixed.json | msgspec | 0.052 | 0.054 | 0.058 | 93.312 | 0.75x |
| mixed.json | ujson | 0.220 | 0.223 | 0.241 | 93.312 | 0.18x |
| mixed.json | json | 0.352 | 0.357 | 0.371 | 93.312 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.564 | 8.027 | 8.223 | 84.484 | 1.00x |
| users.json | orjson | 9.433 | 10.154 | 10.323 | 84.484 | 0.79x |
| users.json | msgspec | 9.053 | 9.730 | 9.897 | 84.484 | 0.82x |
| users.json | ujson | 12.337 | 13.447 | 13.736 | 84.484 | 0.60x |
| users.json | json | 14.796 | 15.827 | 16.062 | 84.484 | 0.51x |
| flat.json | strata | 0.712 | 0.719 | 0.815 | 87.109 | 1.00x |
| flat.json | orjson | 0.858 | 0.964 | 1.010 | 87.109 | 0.75x |
| flat.json | msgspec | 0.782 | 0.802 | 0.896 | 87.109 | 0.90x |
| flat.json | ujson | 1.134 | 1.176 | 1.319 | 87.109 | 0.61x |
| flat.json | json | 1.400 | 1.441 | 1.584 | 87.109 | 0.50x |
| nested.json | strata | 0.652 | 0.666 | 0.684 | 87.109 | 1.00x |
| nested.json | orjson | 0.857 | 0.887 | 0.939 | 87.109 | 0.75x |
| nested.json | msgspec | 0.752 | 0.789 | 0.835 | 87.109 | 0.84x |
| nested.json | ujson | 1.065 | 1.080 | 1.132 | 87.109 | 0.62x |
| nested.json | json | 1.497 | 1.533 | 1.635 | 87.109 | 0.43x |
| wide_arrays.json | strata | 3.656 | 3.748 | 3.980 | 93.297 | 1.00x |
| wide_arrays.json | orjson | 3.723 | 3.934 | 4.035 | 93.297 | 0.95x |
| wide_arrays.json | msgspec | 4.286 | 4.519 | 4.596 | 93.297 | 0.83x |
| wide_arrays.json | ujson | 5.678 | 6.079 | 6.250 | 93.297 | 0.62x |
| wide_arrays.json | json | 6.966 | 7.329 | 7.675 | 93.297 | 0.51x |
| mixed.json | strata | 0.172 | 0.177 | 0.197 | 93.312 | 1.00x |
| mixed.json | orjson | 0.212 | 0.288 | 0.384 | 93.312 | 0.61x |
| mixed.json | msgspec | 0.214 | 0.227 | 0.281 | 93.312 | 0.78x |
| mixed.json | ujson | 0.266 | 0.276 | 0.304 | 93.312 | 0.64x |
| mixed.json | json | 0.367 | 0.375 | 0.421 | 93.312 | 0.47x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.647 | 8.137 | 8.761 | 87.109 | 1.00x |
| users.ndjson | orjson | 11.140 | 11.907 | 12.558 | 87.109 | 0.68x |
| users.ndjson | msgspec | 11.020 | 11.845 | 12.151 | 87.109 | 0.69x |
| users.ndjson | ujson | 13.908 | 14.633 | 14.763 | 87.109 | 0.56x |
| users.ndjson | json | 17.793 | 18.950 | 19.446 | 87.109 | 0.43x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.152 | 2.235 | 2.673 | 83.344 | 1.00x |
| users.json | orjson | 2.526 | 2.830 | 3.000 | 83.344 | 0.79x |
| users.json | msgspec | 3.414 | 3.519 | 4.091 | 83.344 | 0.63x |
| users.json | ujson | 11.440 | 12.166 | 12.917 | 83.344 | 0.18x |
| users.json | json | 15.762 | 17.008 | 17.231 | 83.344 | 0.13x |
| flat.json | strata | 0.293 | 0.320 | 0.410 | 87.109 | 1.00x |
| flat.json | orjson | 0.352 | 0.452 | 0.648 | 87.109 | 0.71x |
| flat.json | msgspec | 0.410 | 0.433 | 0.508 | 87.109 | 0.74x |
| flat.json | ujson | 1.011 | 1.038 | 1.084 | 87.109 | 0.31x |
| flat.json | json | 1.434 | 1.519 | 1.733 | 87.109 | 0.21x |
| nested.json | strata | 0.233 | 0.246 | 0.355 | 87.109 | 1.00x |
| nested.json | orjson | 0.326 | 0.332 | 0.401 | 87.109 | 0.74x |
| nested.json | msgspec | 0.387 | 0.399 | 0.492 | 87.109 | 0.62x |
| nested.json | ujson | 1.138 | 1.221 | 1.355 | 87.109 | 0.20x |
| nested.json | json | 1.711 | 1.765 | 1.826 | 87.109 | 0.14x |
| wide_arrays.json | strata | 1.651 | 1.689 | 1.770 | 93.297 | 1.00x |
| wide_arrays.json | orjson | 1.772 | 1.867 | 2.058 | 93.297 | 0.90x |
| wide_arrays.json | msgspec | 2.582 | 2.652 | 2.845 | 93.297 | 0.64x |
| wide_arrays.json | ujson | 7.158 | 7.357 | 7.688 | 93.297 | 0.23x |
| wide_arrays.json | json | 12.077 | 12.569 | 12.857 | 93.297 | 0.13x |
| mixed.json | strata | 0.139 | 0.146 | 0.285 | 93.312 | 1.00x |
| mixed.json | orjson | 0.148 | 0.158 | 0.245 | 93.312 | 0.93x |
| mixed.json | msgspec | 0.153 | 0.203 | 0.379 | 93.312 | 0.72x |
| mixed.json | ujson | 0.334 | 0.357 | 0.485 | 93.312 | 0.41x |
| mixed.json | json | 0.459 | 0.474 | 0.615 | 93.312 | 0.31x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.052 | 0.057 | 0.100 | 83.391 | 1.00x |
| users.json $[*].id | jmespath | 0.287 | 0.299 | 0.326 | 83.391 | 0.19x |
| users.json $[*].id | jsonpath-ng | 1.542 | 1.577 | 1.626 | 83.391 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.272 | 0.301 | 0.354 | 83.531 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.606 | 1.714 | 1.965 | 83.531 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 9.954 | 10.491 | 11.074 | 83.531 | 0.03x |
| users.json $..total | strata | 1.298 | 1.327 | 1.548 | 84.438 | 1.00x |
| users.json $..total | jsonpath-ng | 190.688 | 195.996 | 197.184 | 84.438 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.724 | 3.949 | 4.122 | 83.469 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.436 | 11.559 | 11.864 | 83.469 | 0.34x |
| users.json $[*].id | orjson+jsonpath-ng | 11.564 | 13.044 | 13.446 | 83.469 | 0.30x |
| users.json $[*].orders[*].total | strata | 3.597 | 3.803 | 3.928 | 84.375 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 10.966 | 11.951 | 12.130 | 84.375 | 0.32x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 21.350 | 23.434 | 23.760 | 84.375 | 0.16x |
| users.json $..total | strata | 9.328 | 9.543 | 10.306 | 84.469 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 201.705 | 207.169 | 222.428 | 84.469 | 0.05x |

