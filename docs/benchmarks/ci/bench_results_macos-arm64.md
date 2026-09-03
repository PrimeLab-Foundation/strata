# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 74164cd
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
| users.json | strata | 8.360 | 9.837 | 16.615 | 63.547 | 1.00x |
| users.json | orjson | 11.209 | 13.053 | 15.270 | 63.547 | 0.75x |
| users.json | msgspec | 10.330 | 13.342 | 18.304 | 63.547 | 0.74x |
| users.json | ujson | 13.573 | 17.563 | 20.401 | 63.547 | 0.56x |
| users.json | pysimdjson | 162.286 | 183.078 | 191.709 | 63.547 | 0.05x |
| users.json | json | 18.500 | 23.639 | 45.444 | 63.547 | 0.42x |
| flat.json | strata | 0.641 | 0.694 | 0.774 | 91.781 | 1.00x |
| flat.json | orjson | 0.793 | 0.840 | 0.956 | 91.781 | 0.83x |
| flat.json | msgspec | 0.759 | 0.868 | 0.980 | 91.781 | 0.80x |
| flat.json | ujson | 1.136 | 1.279 | 1.474 | 91.781 | 0.54x |
| flat.json | pysimdjson | 12.712 | 13.173 | 14.071 | 91.781 | 0.05x |
| flat.json | json | 1.465 | 1.554 | 1.712 | 91.781 | 0.45x |
| nested.json | strata | 0.580 | 0.625 | 0.675 | 91.797 | 1.00x |
| nested.json | orjson | 0.764 | 0.837 | 1.297 | 91.797 | 0.75x |
| nested.json | msgspec | 0.711 | 0.783 | 0.853 | 91.797 | 0.80x |
| nested.json | ujson | 1.163 | 1.240 | 1.420 | 91.797 | 0.50x |
| nested.json | pysimdjson | 10.666 | 11.235 | 12.826 | 91.797 | 0.06x |
| nested.json | json | 1.505 | 1.598 | 1.893 | 91.797 | 0.39x |
| wide_arrays.json | strata | 3.670 | 3.944 | 4.381 | 94.641 | 1.00x |
| wide_arrays.json | orjson | 3.915 | 4.362 | 4.544 | 94.641 | 0.90x |
| wide_arrays.json | msgspec | 4.332 | 4.754 | 5.011 | 94.641 | 0.83x |
| wide_arrays.json | ujson | 5.737 | 6.300 | 7.151 | 94.641 | 0.63x |
| wide_arrays.json | pysimdjson | 67.313 | 70.657 | 96.018 | 94.641 | 0.06x |
| wide_arrays.json | json | 7.336 | 7.699 | 8.495 | 94.641 | 0.51x |
| mixed.json | strata | 0.138 | 0.150 | 0.182 | 94.656 | 1.00x |
| mixed.json | orjson | 0.168 | 0.193 | 0.215 | 94.656 | 0.78x |
| mixed.json | msgspec | 0.176 | 0.188 | 0.225 | 94.656 | 0.80x |
| mixed.json | ujson | 0.225 | 0.308 | 0.450 | 94.656 | 0.49x |
| mixed.json | pysimdjson | 2.579 | 2.811 | 3.028 | 94.656 | 0.05x |
| mixed.json | json | 0.342 | 0.375 | 0.561 | 94.656 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.803 | 2.957 | 6.086 | 77.078 | 1.00x |
| users.json | orjson | 2.616 | 3.673 | 8.338 | 77.078 | 0.81x |
| users.json | msgspec | 3.393 | 4.391 | 8.337 | 77.078 | 0.67x |
| users.json | ujson | 12.415 | 17.378 | 21.353 | 77.078 | 0.17x |
| users.json | json | 18.608 | 22.504 | 30.071 | 77.078 | 0.13x |
| flat.json | strata | 0.227 | 0.248 | 0.436 | 91.797 | 1.00x |
| flat.json | orjson | 0.275 | 0.373 | 0.480 | 91.797 | 0.66x |
| flat.json | msgspec | 0.333 | 0.376 | 0.439 | 91.797 | 0.66x |
| flat.json | ujson | 0.963 | 1.041 | 1.237 | 91.797 | 0.24x |
| flat.json | json | 1.470 | 1.524 | 1.709 | 91.797 | 0.16x |
| nested.json | strata | 0.149 | 0.157 | 0.316 | 91.797 | 1.00x |
| nested.json | orjson | 0.242 | 0.258 | 0.816 | 91.797 | 0.61x |
| nested.json | msgspec | 0.302 | 0.327 | 1.019 | 91.797 | 0.48x |
| nested.json | ujson | 1.083 | 1.217 | 12.412 | 91.797 | 0.13x |
| nested.json | json | 1.690 | 1.847 | 5.136 | 91.797 | 0.08x |
| wide_arrays.json | strata | 1.178 | 1.345 | 1.820 | 94.641 | 1.00x |
| wide_arrays.json | orjson | 1.387 | 1.636 | 1.755 | 94.641 | 0.82x |
| wide_arrays.json | msgspec | 2.154 | 2.353 | 2.552 | 94.641 | 0.57x |
| wide_arrays.json | ujson | 6.756 | 7.254 | 8.644 | 94.641 | 0.19x |
| wide_arrays.json | json | 12.418 | 12.891 | 17.226 | 94.641 | 0.10x |
| mixed.json | strata | 0.054 | 0.063 | 0.069 | 94.656 | 1.00x |
| mixed.json | orjson | 0.056 | 0.065 | 0.077 | 94.656 | 0.98x |
| mixed.json | msgspec | 0.067 | 0.072 | 0.253 | 94.656 | 0.88x |
| mixed.json | ujson | 0.241 | 0.386 | 0.682 | 94.656 | 0.16x |
| mixed.json | json | 0.404 | 0.441 | 0.538 | 94.656 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.241 | 9.881 | 22.048 | 87.375 | 1.00x |
| users.json | orjson | 11.730 | 17.085 | 47.686 | 87.375 | 0.58x |
| users.json | msgspec | 11.633 | 14.252 | 55.646 | 87.375 | 0.69x |
| users.json | ujson | 17.326 | 21.270 | 27.128 | 87.375 | 0.46x |
| users.json | json | 17.794 | 26.910 | 32.428 | 87.375 | 0.37x |
| flat.json | strata | 0.708 | 0.773 | 0.925 | 91.797 | 1.00x |
| flat.json | orjson | 1.010 | 1.105 | 2.056 | 91.797 | 0.70x |
| flat.json | msgspec | 0.891 | 1.003 | 1.432 | 91.797 | 0.77x |
| flat.json | ujson | 1.254 | 1.376 | 1.581 | 91.797 | 0.56x |
| flat.json | json | 1.566 | 1.625 | 1.711 | 91.797 | 0.48x |
| nested.json | strata | 0.676 | 0.710 | 0.798 | 91.797 | 1.00x |
| nested.json | orjson | 1.031 | 1.115 | 1.319 | 91.797 | 0.64x |
| nested.json | msgspec | 0.865 | 0.914 | 0.962 | 91.797 | 0.78x |
| nested.json | ujson | 1.217 | 1.254 | 1.627 | 91.797 | 0.57x |
| nested.json | json | 1.690 | 1.802 | 1.975 | 91.797 | 0.39x |
| wide_arrays.json | strata | 3.868 | 4.051 | 4.416 | 94.641 | 1.00x |
| wide_arrays.json | orjson | 4.107 | 4.251 | 4.729 | 94.641 | 0.95x |
| wide_arrays.json | msgspec | 4.799 | 4.886 | 5.218 | 94.641 | 0.83x |
| wide_arrays.json | ujson | 6.357 | 6.560 | 7.014 | 94.641 | 0.62x |
| wide_arrays.json | json | 7.831 | 7.901 | 8.078 | 94.641 | 0.51x |
| mixed.json | strata | 0.218 | 0.257 | 0.426 | 94.656 | 1.00x |
| mixed.json | orjson | 0.307 | 0.476 | 1.356 | 94.656 | 0.54x |
| mixed.json | msgspec | 0.291 | 0.370 | 0.793 | 94.656 | 0.69x |
| mixed.json | ujson | 0.357 | 0.492 | 1.055 | 94.656 | 0.52x |
| mixed.json | json | 0.464 | 0.540 | 1.380 | 94.656 | 0.48x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 8.045 | 9.936 | 14.422 | 91.781 | 1.00x |
| users.ndjson | orjson | 13.292 | 16.560 | 19.207 | 91.781 | 0.60x |
| users.ndjson | msgspec | 12.928 | 15.577 | 18.605 | 91.781 | 0.64x |
| users.ndjson | ujson | 16.260 | 18.745 | 27.971 | 91.781 | 0.53x |
| users.ndjson | json | 19.984 | 24.772 | 50.295 | 91.781 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.575 | 2.745 | 3.062 | 84.109 | 1.00x |
| users.json | orjson | 3.184 | 3.494 | 6.526 | 84.109 | 0.79x |
| users.json | msgspec | 3.728 | 4.254 | 8.207 | 84.109 | 0.65x |
| users.json | ujson | 12.468 | 14.268 | 19.084 | 84.109 | 0.19x |
| users.json | json | 17.724 | 20.058 | 26.493 | 84.109 | 0.14x |
| flat.json | strata | 0.489 | 0.614 | 0.829 | 91.797 | 1.00x |
| flat.json | orjson | 0.595 | 0.755 | 2.847 | 91.797 | 0.81x |
| flat.json | msgspec | 0.639 | 0.767 | 1.399 | 91.797 | 0.80x |
| flat.json | ujson | 1.410 | 1.660 | 4.119 | 91.797 | 0.37x |
| flat.json | json | 1.880 | 2.240 | 2.824 | 91.797 | 0.27x |
| nested.json | strata | 0.322 | 0.348 | 0.413 | 91.797 | 1.00x |
| nested.json | orjson | 0.414 | 0.442 | 0.628 | 91.797 | 0.79x |
| nested.json | msgspec | 0.530 | 0.657 | 0.730 | 91.797 | 0.53x |
| nested.json | ujson | 1.388 | 1.414 | 1.991 | 91.797 | 0.25x |
| nested.json | json | 1.901 | 1.953 | 2.995 | 91.797 | 0.18x |
| wide_arrays.json | strata | 1.462 | 1.595 | 1.863 | 94.641 | 1.00x |
| wide_arrays.json | orjson | 1.836 | 1.984 | 2.175 | 94.641 | 0.80x |
| wide_arrays.json | msgspec | 2.690 | 2.793 | 3.570 | 94.641 | 0.57x |
| wide_arrays.json | ujson | 7.428 | 7.531 | 11.670 | 94.641 | 0.21x |
| wide_arrays.json | json | 12.599 | 12.651 | 17.064 | 94.641 | 0.13x |
| mixed.json | strata | 0.190 | 0.274 | 0.434 | 94.656 | 1.00x |
| mixed.json | orjson | 0.203 | 0.322 | 0.593 | 94.656 | 0.85x |
| mixed.json | msgspec | 0.195 | 0.368 | 0.712 | 94.656 | 0.74x |
| mixed.json | ujson | 0.455 | 0.534 | 1.512 | 94.656 | 0.51x |
| mixed.json | json | 0.538 | 0.719 | 0.998 | 94.656 | 0.38x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.139 | 0.167 | 0.329 | 84.172 | 1.00x |
| users.json $[*].id | jmespath | 0.394 | 0.464 | 1.018 | 84.172 | 0.36x |
| users.json $[*].id | jsonpath-ng | 1.753 | 2.283 | 3.955 | 84.172 | 0.07x |
| users.json $[*].orders[*].total | strata | 0.560 | 1.000 | 2.869 | 84.359 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.027 | 2.769 | 4.992 | 84.359 | 0.36x |
| users.json $[*].orders[*].total | jsonpath-ng | 14.408 | 18.810 | 33.593 | 84.359 | 0.05x |
| users.json $..total | strata | 1.456 | 1.706 | 1.811 | 86.422 | 1.00x |
| users.json $..total | jsonpath-ng | 245.137 | 263.638 | 324.548 | 86.422 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.031 | 5.139 | 6.599 | 84.234 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.137 | 18.626 | 25.459 | 84.234 | 0.28x |
| users.json $[*].id | orjson+jsonpath-ng | 15.220 | 19.222 | 23.892 | 84.234 | 0.27x |
| users.json $[*].orders[*].total | strata | 4.177 | 5.046 | 8.312 | 86.391 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.717 | 17.236 | 30.812 | 86.391 | 0.29x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 36.932 | 40.220 | 60.490 | 86.391 | 0.13x |
| users.json $..total | strata | 9.540 | 10.095 | 17.049 | 86.438 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 222.401 | 224.411 | 335.808 | 86.438 | 0.04x |

