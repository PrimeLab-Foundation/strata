# Benchmark results - ci-linux-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 79fa3df
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
| users.json | strata | 8.830 | 8.911 | 10.559 | 51.816 | 1.00x |
| users.json | orjson | 11.582 | 11.785 | 13.277 | 51.816 | 0.76x |
| users.json | msgspec | 12.100 | 12.224 | 13.445 | 51.816 | 0.73x |
| users.json | ujson | 16.565 | 16.945 | 19.549 | 51.816 | 0.53x |
| users.json | pysimdjson | 16.517 | 16.930 | 19.163 | 51.816 | 0.53x |
| users.json | json | 20.485 | 20.832 | 21.586 | 51.816 | 0.43x |
| flat.json | strata | 0.809 | 0.834 | 0.846 | 58.672 | 1.00x |
| flat.json | orjson | 0.866 | 0.882 | 0.922 | 58.672 | 0.95x |
| flat.json | msgspec | 0.917 | 0.927 | 0.940 | 58.672 | 0.90x |
| flat.json | ujson | 1.448 | 1.464 | 1.481 | 58.672 | 0.57x |
| flat.json | pysimdjson | 1.473 | 1.490 | 1.508 | 58.672 | 0.56x |
| flat.json | json | 1.775 | 1.781 | 1.794 | 58.672 | 0.47x |
| nested.json | strata | 0.798 | 0.810 | 0.818 | 58.672 | 1.00x |
| nested.json | orjson | 0.871 | 0.888 | 0.898 | 58.672 | 0.91x |
| nested.json | msgspec | 0.988 | 0.997 | 1.007 | 58.672 | 0.81x |
| nested.json | ujson | 1.388 | 1.405 | 1.425 | 58.672 | 0.58x |
| nested.json | pysimdjson | 1.396 | 1.412 | 1.434 | 58.672 | 0.57x |
| nested.json | json | 1.937 | 1.959 | 1.984 | 58.672 | 0.41x |
| wide_arrays.json | strata | 3.799 | 3.832 | 3.909 | 60.246 | 1.00x |
| wide_arrays.json | orjson | 3.981 | 4.071 | 4.185 | 60.246 | 0.94x |
| wide_arrays.json | msgspec | 5.003 | 5.047 | 5.097 | 60.246 | 0.76x |
| wide_arrays.json | ujson | 6.434 | 6.455 | 6.550 | 60.246 | 0.59x |
| wide_arrays.json | pysimdjson | 5.216 | 5.267 | 5.352 | 60.246 | 0.73x |
| wide_arrays.json | json | 9.450 | 9.524 | 9.635 | 60.246 | 0.40x |
| mixed.json | strata | 0.188 | 0.192 | 0.222 | 60.246 | 1.00x |
| mixed.json | orjson | 0.213 | 0.216 | 0.220 | 60.246 | 0.89x |
| mixed.json | msgspec | 0.233 | 0.234 | 0.277 | 60.246 | 0.82x |
| mixed.json | ujson | 0.301 | 0.305 | 0.330 | 60.246 | 0.63x |
| mixed.json | pysimdjson | 0.295 | 0.298 | 0.320 | 60.246 | 0.65x |
| mixed.json | json | 0.450 | 0.455 | 0.472 | 60.246 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.987 | 2.003 | 2.016 | 50.898 | 1.00x |
| users.json | orjson | 2.579 | 2.592 | 2.604 | 50.898 | 0.77x |
| users.json | msgspec | 3.325 | 3.338 | 3.359 | 50.898 | 0.60x |
| users.json | ujson | 10.521 | 10.556 | 10.669 | 50.898 | 0.19x |
| users.json | json | 19.068 | 19.149 | 19.304 | 50.898 | 0.10x |
| flat.json | strata | 0.240 | 0.242 | 0.260 | 58.672 | 1.00x |
| flat.json | orjson | 0.301 | 0.310 | 0.328 | 58.672 | 0.78x |
| flat.json | msgspec | 0.385 | 0.389 | 0.404 | 58.672 | 0.62x |
| flat.json | ujson | 0.996 | 1.006 | 1.027 | 58.672 | 0.24x |
| flat.json | json | 1.698 | 1.706 | 1.722 | 58.672 | 0.14x |
| nested.json | strata | 0.235 | 0.237 | 0.258 | 58.676 | 1.00x |
| nested.json | orjson | 0.283 | 0.286 | 0.304 | 58.676 | 0.83x |
| nested.json | msgspec | 0.362 | 0.366 | 0.383 | 58.676 | 0.65x |
| nested.json | ujson | 1.075 | 1.084 | 1.094 | 58.676 | 0.22x |
| nested.json | json | 2.147 | 2.181 | 2.204 | 58.676 | 0.11x |
| wide_arrays.json | strata | 1.270 | 1.282 | 1.300 | 60.246 | 1.00x |
| wide_arrays.json | orjson | 1.528 | 1.551 | 1.567 | 60.246 | 0.83x |
| wide_arrays.json | msgspec | 2.298 | 2.307 | 2.328 | 60.246 | 0.56x |
| wide_arrays.json | ujson | 4.694 | 4.719 | 4.732 | 60.246 | 0.27x |
| wide_arrays.json | json | 13.474 | 13.514 | 13.559 | 60.246 | 0.09x |
| mixed.json | strata | 0.065 | 0.068 | 0.069 | 60.246 | 1.00x |
| mixed.json | orjson | 0.065 | 0.067 | 0.081 | 60.246 | 1.01x |
| mixed.json | msgspec | 0.078 | 0.080 | 0.101 | 60.246 | 0.85x |
| mixed.json | ujson | 0.236 | 0.239 | 0.261 | 60.246 | 0.28x |
| mixed.json | json | 0.481 | 0.490 | 0.506 | 60.246 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.275 | 9.433 | 10.262 | 63.121 | 1.00x |
| users.json | orjson | 11.910 | 12.475 | 12.929 | 63.121 | 0.76x |
| users.json | msgspec | 12.441 | 12.879 | 13.161 | 63.121 | 0.73x |
| users.json | ujson | 17.797 | 18.130 | 19.021 | 63.121 | 0.52x |
| users.json | json | 21.165 | 21.481 | 21.694 | 63.121 | 0.44x |
| flat.json | strata | 0.828 | 0.865 | 0.892 | 58.672 | 1.00x |
| flat.json | orjson | 0.937 | 0.949 | 0.983 | 58.672 | 0.91x |
| flat.json | msgspec | 0.965 | 0.997 | 1.010 | 58.672 | 0.87x |
| flat.json | ujson | 1.530 | 1.548 | 1.562 | 58.672 | 0.56x |
| flat.json | json | 1.828 | 1.841 | 1.858 | 58.672 | 0.47x |
| nested.json | strata | 0.840 | 0.856 | 0.863 | 58.676 | 1.00x |
| nested.json | orjson | 0.938 | 0.958 | 0.973 | 58.676 | 0.89x |
| nested.json | msgspec | 1.052 | 1.069 | 1.080 | 58.676 | 0.80x |
| nested.json | ujson | 1.467 | 1.489 | 1.506 | 58.676 | 0.57x |
| nested.json | json | 2.008 | 2.012 | 2.029 | 58.676 | 0.43x |
| wide_arrays.json | strata | 3.875 | 3.911 | 3.983 | 60.246 | 1.00x |
| wide_arrays.json | orjson | 4.071 | 4.150 | 4.339 | 60.246 | 0.94x |
| wide_arrays.json | msgspec | 5.089 | 5.146 | 5.309 | 60.246 | 0.76x |
| wide_arrays.json | ujson | 6.682 | 6.726 | 6.899 | 60.246 | 0.58x |
| wide_arrays.json | json | 9.617 | 9.682 | 9.860 | 60.246 | 0.40x |
| mixed.json | strata | 0.218 | 0.222 | 0.252 | 60.246 | 1.00x |
| mixed.json | orjson | 0.275 | 0.286 | 0.304 | 60.246 | 0.78x |
| mixed.json | msgspec | 0.293 | 0.295 | 0.319 | 60.246 | 0.75x |
| mixed.json | ujson | 0.378 | 0.388 | 0.409 | 60.246 | 0.57x |
| mixed.json | json | 0.507 | 0.521 | 0.535 | 60.246 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.520 | 10.020 | 10.235 | 58.668 | 1.00x |
| users.ndjson | orjson | 14.846 | 15.405 | 15.511 | 58.668 | 0.65x |
| users.ndjson | msgspec | 15.307 | 15.531 | 15.766 | 58.668 | 0.65x |
| users.ndjson | ujson | 19.954 | 20.367 | 20.683 | 58.668 | 0.49x |
| users.ndjson | json | 26.294 | 26.827 | 27.186 | 58.668 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.550 | 2.591 | 13.602 | 60.121 | 1.00x |
| users.json | orjson | 3.198 | 3.243 | 3.288 | 60.121 | 0.80x |
| users.json | msgspec | 3.883 | 3.954 | 4.004 | 60.121 | 0.66x |
| users.json | ujson | 11.307 | 11.443 | 11.766 | 60.121 | 0.23x |
| users.json | json | 19.922 | 20.247 | 20.456 | 60.121 | 0.13x |
| flat.json | strata | 0.399 | 0.404 | 0.429 | 58.672 | 1.00x |
| flat.json | orjson | 0.483 | 0.502 | 0.522 | 58.672 | 0.81x |
| flat.json | msgspec | 0.562 | 0.586 | 0.599 | 58.672 | 0.69x |
| flat.json | ujson | 1.198 | 1.212 | 1.250 | 58.672 | 0.33x |
| flat.json | json | 1.878 | 1.914 | 1.942 | 58.672 | 0.21x |
| nested.json | strata | 0.370 | 0.375 | 0.413 | 58.676 | 1.00x |
| nested.json | orjson | 0.437 | 0.447 | 0.476 | 58.676 | 0.84x |
| nested.json | msgspec | 0.519 | 0.529 | 0.542 | 58.676 | 0.71x |
| nested.json | ujson | 1.266 | 1.272 | 1.305 | 58.676 | 0.29x |
| nested.json | json | 2.314 | 2.345 | 2.403 | 58.676 | 0.16x |
| wide_arrays.json | strata | 1.630 | 1.675 | 1.728 | 60.246 | 1.00x |
| wide_arrays.json | orjson | 1.915 | 1.989 | 2.024 | 60.246 | 0.84x |
| wide_arrays.json | msgspec | 2.673 | 2.721 | 2.770 | 60.246 | 0.62x |
| wide_arrays.json | ujson | 5.167 | 5.198 | 5.263 | 60.246 | 0.32x |
| wide_arrays.json | json | 13.949 | 13.991 | 14.092 | 60.246 | 0.12x |
| mixed.json | strata | 0.166 | 0.187 | 0.208 | 60.246 | 1.00x |
| mixed.json | orjson | 0.184 | 0.193 | 0.207 | 60.246 | 0.97x |
| mixed.json | msgspec | 0.200 | 0.224 | 0.234 | 60.246 | 0.84x |
| mixed.json | ujson | 0.378 | 0.385 | 0.443 | 60.246 | 0.48x |
| mixed.json | json | 0.608 | 0.620 | 0.661 | 60.246 | 0.30x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.105 | 0.109 | 0.123 | 60.125 | 1.00x |
| users.json $[*].id | jmespath | 0.475 | 0.484 | 0.496 | 60.125 | 0.23x |
| users.json $[*].id | jsonpath-ng | 2.492 | 2.547 | 2.615 | 60.125 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.621 | 0.635 | 0.650 | 60.254 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.980 | 3.012 | 3.036 | 60.254 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.898 | 18.270 | 19.419 | 60.254 | 0.03x |
| users.json $..total | strata | 1.714 | 1.722 | 1.747 | 62.121 | 1.00x |
| users.json $..total | jsonpath-ng | 292.919 | 293.298 | 293.571 | 62.121 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.237 | 3.263 | 3.542 | 60.254 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.663 | 12.814 | 12.921 | 60.254 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 14.426 | 14.588 | 14.734 | 60.254 | 0.22x |
| users.json $[*].orders[*].total | strata | 3.453 | 3.467 | 3.505 | 62.121 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.282 | 15.656 | 15.992 | 62.121 | 0.22x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 33.605 | 34.621 | 35.521 | 62.121 | 0.10x |
| users.json $..total | strata | 11.571 | 11.853 | 12.130 | 62.195 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 311.885 | 314.233 | 315.278 | 62.195 | 0.04x |

