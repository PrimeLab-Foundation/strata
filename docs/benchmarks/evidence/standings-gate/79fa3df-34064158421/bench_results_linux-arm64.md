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
| users.json | strata | 8.975 | 9.308 | 11.036 | 51.812 | 1.00x |
| users.json | orjson | 12.159 | 12.510 | 13.815 | 51.812 | 0.74x |
| users.json | msgspec | 12.449 | 12.809 | 14.033 | 51.812 | 0.73x |
| users.json | ujson | 17.178 | 18.166 | 19.975 | 51.812 | 0.51x |
| users.json | pysimdjson | 17.629 | 18.022 | 19.807 | 51.812 | 0.52x |
| users.json | json | 21.463 | 21.625 | 22.691 | 51.812 | 0.43x |
| flat.json | strata | 0.810 | 0.831 | 0.853 | 59.672 | 1.00x |
| flat.json | orjson | 0.861 | 0.885 | 0.896 | 59.672 | 0.94x |
| flat.json | msgspec | 0.890 | 0.915 | 0.927 | 59.672 | 0.91x |
| flat.json | ujson | 1.425 | 1.437 | 1.456 | 59.672 | 0.58x |
| flat.json | pysimdjson | 1.469 | 1.476 | 1.515 | 59.672 | 0.56x |
| flat.json | json | 1.770 | 1.778 | 1.793 | 59.672 | 0.47x |
| nested.json | strata | 0.805 | 0.827 | 0.841 | 59.672 | 1.00x |
| nested.json | orjson | 0.887 | 0.905 | 0.916 | 59.672 | 0.91x |
| nested.json | msgspec | 1.003 | 1.009 | 1.025 | 59.672 | 0.82x |
| nested.json | ujson | 1.406 | 1.434 | 1.464 | 59.672 | 0.58x |
| nested.json | pysimdjson | 1.420 | 1.435 | 1.461 | 59.672 | 0.58x |
| nested.json | json | 1.994 | 2.011 | 2.024 | 59.672 | 0.41x |
| wide_arrays.json | strata | 3.820 | 3.860 | 3.892 | 61.242 | 1.00x |
| wide_arrays.json | orjson | 3.997 | 4.103 | 4.245 | 61.242 | 0.94x |
| wide_arrays.json | msgspec | 5.050 | 5.103 | 5.182 | 61.242 | 0.76x |
| wide_arrays.json | ujson | 6.469 | 6.521 | 6.657 | 61.242 | 0.59x |
| wide_arrays.json | pysimdjson | 5.251 | 5.309 | 5.514 | 61.242 | 0.73x |
| wide_arrays.json | json | 9.457 | 9.522 | 9.670 | 61.242 | 0.41x |
| mixed.json | strata | 0.199 | 0.201 | 0.225 | 61.242 | 1.00x |
| mixed.json | orjson | 0.223 | 0.225 | 0.249 | 61.242 | 0.89x |
| mixed.json | msgspec | 0.246 | 0.249 | 0.269 | 61.242 | 0.81x |
| mixed.json | ujson | 0.314 | 0.320 | 0.345 | 61.242 | 0.63x |
| mixed.json | pysimdjson | 0.302 | 0.307 | 0.334 | 61.242 | 0.65x |
| mixed.json | json | 0.479 | 0.488 | 0.512 | 61.242 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.040 | 2.066 | 2.115 | 50.898 | 1.00x |
| users.json | orjson | 2.613 | 2.632 | 2.662 | 50.898 | 0.78x |
| users.json | msgspec | 3.363 | 3.387 | 3.446 | 50.898 | 0.61x |
| users.json | ujson | 10.621 | 10.732 | 10.822 | 50.898 | 0.19x |
| users.json | json | 19.202 | 19.284 | 19.398 | 50.898 | 0.11x |
| flat.json | strata | 0.240 | 0.245 | 0.262 | 59.672 | 1.00x |
| flat.json | orjson | 0.302 | 0.308 | 0.331 | 59.672 | 0.79x |
| flat.json | msgspec | 0.382 | 0.386 | 0.408 | 59.672 | 0.63x |
| flat.json | ujson | 1.001 | 1.006 | 1.021 | 59.672 | 0.24x |
| flat.json | json | 1.684 | 1.699 | 1.724 | 59.672 | 0.14x |
| nested.json | strata | 0.241 | 0.244 | 0.256 | 59.672 | 1.00x |
| nested.json | orjson | 0.286 | 0.291 | 0.306 | 59.672 | 0.84x |
| nested.json | msgspec | 0.368 | 0.371 | 0.384 | 59.672 | 0.66x |
| nested.json | ujson | 1.112 | 1.121 | 1.137 | 59.672 | 0.22x |
| nested.json | json | 2.149 | 2.169 | 2.193 | 59.672 | 0.11x |
| wide_arrays.json | strata | 1.322 | 1.338 | 1.474 | 61.242 | 1.00x |
| wide_arrays.json | orjson | 1.582 | 1.590 | 1.649 | 61.242 | 0.84x |
| wide_arrays.json | msgspec | 2.319 | 2.337 | 2.386 | 61.242 | 0.57x |
| wide_arrays.json | ujson | 4.734 | 4.754 | 4.889 | 61.242 | 0.28x |
| wide_arrays.json | json | 13.553 | 13.602 | 13.808 | 61.242 | 0.10x |
| mixed.json | strata | 0.072 | 0.075 | 0.077 | 61.242 | 1.00x |
| mixed.json | orjson | 0.070 | 0.072 | 0.074 | 61.242 | 1.04x |
| mixed.json | msgspec | 0.088 | 0.089 | 0.117 | 61.242 | 0.84x |
| mixed.json | ujson | 0.251 | 0.261 | 0.281 | 61.242 | 0.29x |
| mixed.json | json | 0.506 | 0.517 | 0.540 | 61.242 | 0.15x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.353 | 9.562 | 10.158 | 63.121 | 1.00x |
| users.json | orjson | 12.612 | 12.682 | 13.002 | 63.121 | 0.75x |
| users.json | msgspec | 12.970 | 13.279 | 13.432 | 63.121 | 0.72x |
| users.json | ujson | 17.737 | 18.453 | 19.288 | 63.121 | 0.52x |
| users.json | json | 21.582 | 21.999 | 22.479 | 63.121 | 0.43x |
| flat.json | strata | 0.936 | 0.954 | 0.990 | 59.672 | 1.00x |
| flat.json | orjson | 1.027 | 1.055 | 1.102 | 59.672 | 0.90x |
| flat.json | msgspec | 1.037 | 1.058 | 1.092 | 59.672 | 0.90x |
| flat.json | ujson | 1.590 | 1.627 | 1.681 | 59.672 | 0.59x |
| flat.json | json | 1.879 | 1.905 | 1.929 | 59.672 | 0.50x |
| nested.json | strata | 0.834 | 0.858 | 0.878 | 59.672 | 1.00x |
| nested.json | orjson | 0.967 | 0.980 | 1.018 | 59.672 | 0.88x |
| nested.json | msgspec | 1.077 | 1.094 | 1.154 | 59.672 | 0.78x |
| nested.json | ujson | 1.509 | 1.523 | 1.621 | 59.672 | 0.56x |
| nested.json | json | 2.050 | 2.089 | 2.187 | 59.672 | 0.41x |
| wide_arrays.json | strata | 3.997 | 4.034 | 4.090 | 61.242 | 1.00x |
| wide_arrays.json | orjson | 4.254 | 4.316 | 4.398 | 61.242 | 0.93x |
| wide_arrays.json | msgspec | 5.213 | 5.317 | 5.422 | 61.242 | 0.76x |
| wide_arrays.json | ujson | 6.869 | 7.019 | 7.175 | 61.242 | 0.57x |
| wide_arrays.json | json | 9.800 | 9.849 | 10.066 | 61.242 | 0.41x |
| mixed.json | strata | 0.233 | 0.240 | 0.260 | 61.242 | 1.00x |
| mixed.json | orjson | 0.295 | 0.311 | 0.339 | 61.242 | 0.77x |
| mixed.json | msgspec | 0.315 | 0.325 | 0.340 | 61.242 | 0.74x |
| mixed.json | ujson | 0.405 | 0.417 | 0.444 | 61.242 | 0.57x |
| mixed.json | json | 0.532 | 0.553 | 0.580 | 61.242 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.363 | 9.621 | 9.776 | 59.668 | 1.00x |
| users.ndjson | orjson | 14.931 | 15.175 | 15.709 | 59.668 | 0.63x |
| users.ndjson | msgspec | 15.203 | 15.477 | 15.814 | 59.668 | 0.62x |
| users.ndjson | ujson | 19.732 | 20.049 | 21.020 | 59.668 | 0.48x |
| users.ndjson | json | 26.293 | 26.607 | 27.066 | 59.668 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.809 | 3.146 | 22.554 | 60.121 | 1.00x |
| users.json | orjson | 3.542 | 3.647 | 4.060 | 60.121 | 0.86x |
| users.json | msgspec | 4.275 | 4.387 | 4.783 | 60.121 | 0.72x |
| users.json | ujson | 11.604 | 11.925 | 12.757 | 60.121 | 0.26x |
| users.json | json | 20.069 | 20.515 | 21.094 | 60.121 | 0.15x |
| flat.json | strata | 0.520 | 0.584 | 0.621 | 59.672 | 1.00x |
| flat.json | orjson | 0.611 | 0.657 | 0.747 | 59.672 | 0.89x |
| flat.json | msgspec | 0.687 | 0.736 | 0.877 | 59.672 | 0.79x |
| flat.json | ujson | 1.356 | 1.390 | 1.467 | 59.672 | 0.42x |
| flat.json | json | 2.044 | 2.148 | 2.173 | 59.672 | 0.27x |
| nested.json | strata | 0.483 | 0.537 | 0.599 | 59.672 | 1.00x |
| nested.json | orjson | 0.568 | 0.666 | 0.721 | 59.672 | 0.81x |
| nested.json | msgspec | 0.658 | 0.737 | 0.786 | 59.672 | 0.73x |
| nested.json | ujson | 1.399 | 1.492 | 1.622 | 59.672 | 0.36x |
| nested.json | json | 2.510 | 2.579 | 2.689 | 59.672 | 0.21x |
| wide_arrays.json | strata | 1.828 | 1.934 | 1.999 | 61.242 | 1.00x |
| wide_arrays.json | orjson | 2.144 | 2.214 | 2.320 | 61.242 | 0.87x |
| wide_arrays.json | msgspec | 2.874 | 2.948 | 3.060 | 61.242 | 0.66x |
| wide_arrays.json | ujson | 5.370 | 5.456 | 5.549 | 61.242 | 0.35x |
| wide_arrays.json | json | 14.261 | 14.336 | 14.410 | 61.242 | 0.13x |
| mixed.json | strata | 0.275 | 0.309 | 0.336 | 61.242 | 1.00x |
| mixed.json | orjson | 0.303 | 0.330 | 0.405 | 61.242 | 0.94x |
| mixed.json | msgspec | 0.318 | 0.363 | 0.409 | 61.242 | 0.85x |
| mixed.json | ujson | 0.497 | 0.559 | 0.585 | 61.242 | 0.55x |
| mixed.json | json | 0.763 | 0.797 | 0.930 | 61.242 | 0.39x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.112 | 0.119 | 0.131 | 60.125 | 1.00x |
| users.json $[*].id | jmespath | 0.494 | 0.511 | 0.518 | 60.125 | 0.23x |
| users.json $[*].id | jsonpath-ng | 2.490 | 2.630 | 2.752 | 60.125 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.717 | 0.753 | 0.914 | 60.254 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.163 | 3.263 | 3.578 | 60.254 | 0.23x |
| users.json $[*].orders[*].total | jsonpath-ng | 19.346 | 20.372 | 21.331 | 60.254 | 0.04x |
| users.json $..total | strata | 1.748 | 1.770 | 1.786 | 62.137 | 1.00x |
| users.json $..total | jsonpath-ng | 294.587 | 294.948 | 295.679 | 62.137 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.294 | 3.312 | 3.377 | 60.254 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.349 | 13.481 | 13.729 | 60.254 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 15.189 | 15.396 | 15.727 | 60.254 | 0.22x |
| users.json $[*].orders[*].total | strata | 3.525 | 3.561 | 3.602 | 62.137 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 16.500 | 17.344 | 17.614 | 62.137 | 0.21x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 37.395 | 39.457 | 39.861 | 62.137 | 0.09x |
| users.json $..total | strata | 11.521 | 11.747 | 11.942 | 62.211 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 313.009 | 314.297 | 316.768 | 62.211 | 0.04x |

