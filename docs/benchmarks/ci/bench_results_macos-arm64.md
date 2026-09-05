# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 53fa480
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
| users.json | strata | 6.498 | 7.694 | 15.988 | 63.641 | 1.00x |
| users.json | orjson | 10.069 | 11.708 | 22.250 | 63.641 | 0.66x |
| users.json | msgspec | 9.478 | 11.330 | 21.172 | 63.641 | 0.68x |
| users.json | ujson | 12.878 | 15.633 | 29.280 | 63.641 | 0.49x |
| users.json | pysimdjson | 142.155 | 149.452 | 184.513 | 63.641 | 0.05x |
| users.json | json | 15.853 | 17.365 | 45.068 | 63.641 | 0.44x |
| flat.json | strata | 0.646 | 0.762 | 1.018 | 95.656 | 1.00x |
| flat.json | orjson | 0.873 | 1.104 | 2.523 | 95.656 | 0.69x |
| flat.json | msgspec | 0.839 | 0.859 | 2.247 | 95.656 | 0.89x |
| flat.json | ujson | 1.287 | 1.561 | 2.775 | 95.656 | 0.49x |
| flat.json | pysimdjson | 13.897 | 16.681 | 23.379 | 95.656 | 0.05x |
| flat.json | json | 1.514 | 1.761 | 2.317 | 95.656 | 0.43x |
| nested.json | strata | 0.589 | 0.640 | 0.696 | 95.672 | 1.00x |
| nested.json | orjson | 0.825 | 0.890 | 0.995 | 95.672 | 0.72x |
| nested.json | msgspec | 0.737 | 0.816 | 1.109 | 95.672 | 0.78x |
| nested.json | ujson | 1.328 | 1.388 | 1.557 | 95.672 | 0.46x |
| nested.json | pysimdjson | 11.817 | 12.050 | 12.195 | 95.672 | 0.05x |
| nested.json | json | 1.635 | 1.686 | 1.868 | 95.672 | 0.38x |
| wide_arrays.json | strata | 3.285 | 3.711 | 4.751 | 98.406 | 1.00x |
| wide_arrays.json | orjson | 3.878 | 4.217 | 4.853 | 98.406 | 0.88x |
| wide_arrays.json | msgspec | 4.241 | 4.842 | 5.163 | 98.406 | 0.77x |
| wide_arrays.json | ujson | 5.656 | 6.258 | 6.732 | 98.406 | 0.59x |
| wide_arrays.json | pysimdjson | 68.870 | 73.188 | 74.073 | 98.406 | 0.05x |
| wide_arrays.json | json | 7.051 | 7.913 | 9.141 | 98.406 | 0.47x |
| mixed.json | strata | 0.131 | 0.145 | 0.183 | 98.438 | 1.00x |
| mixed.json | orjson | 0.159 | 0.179 | 0.443 | 98.438 | 0.81x |
| mixed.json | msgspec | 0.175 | 0.189 | 0.217 | 98.438 | 0.77x |
| mixed.json | ujson | 0.234 | 0.336 | 0.516 | 98.438 | 0.43x |
| mixed.json | pysimdjson | 2.613 | 2.859 | 3.443 | 98.438 | 0.05x |
| mixed.json | json | 0.341 | 0.397 | 0.501 | 98.438 | 0.37x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.770 | 1.927 | 4.649 | 79.000 | 1.00x |
| users.json | orjson | 2.392 | 2.823 | 6.503 | 79.000 | 0.68x |
| users.json | msgspec | 3.228 | 3.997 | 7.591 | 79.000 | 0.48x |
| users.json | ujson | 9.356 | 11.302 | 17.686 | 79.000 | 0.17x |
| users.json | json | 17.113 | 19.916 | 36.514 | 79.000 | 0.10x |
| flat.json | strata | 0.386 | 0.430 | 0.660 | 95.672 | 1.00x |
| flat.json | orjson | 0.286 | 0.302 | 0.782 | 95.672 | 1.42x |
| flat.json | msgspec | 0.330 | 0.372 | 0.782 | 95.672 | 1.15x |
| flat.json | ujson | 0.854 | 1.006 | 1.958 | 95.672 | 0.43x |
| flat.json | json | 1.452 | 1.586 | 2.196 | 95.672 | 0.27x |
| nested.json | strata | 0.146 | 0.150 | 0.498 | 95.672 | 1.00x |
| nested.json | orjson | 0.240 | 0.251 | 0.533 | 95.672 | 0.60x |
| nested.json | msgspec | 0.335 | 0.574 | 0.627 | 95.672 | 0.26x |
| nested.json | ujson | 1.114 | 1.175 | 1.382 | 95.672 | 0.13x |
| nested.json | json | 1.714 | 1.760 | 1.932 | 95.672 | 0.09x |
| wide_arrays.json | strata | 1.155 | 1.244 | 1.525 | 98.422 | 1.00x |
| wide_arrays.json | orjson | 1.615 | 1.763 | 1.829 | 98.422 | 0.71x |
| wide_arrays.json | msgspec | 2.296 | 2.675 | 2.852 | 98.422 | 0.46x |
| wide_arrays.json | ujson | 5.122 | 5.716 | 8.795 | 98.422 | 0.22x |
| wide_arrays.json | json | 13.033 | 13.580 | 20.378 | 98.422 | 0.09x |
| mixed.json | strata | 0.043 | 0.048 | 0.067 | 98.438 | 1.00x |
| mixed.json | orjson | 0.050 | 0.055 | 0.099 | 98.438 | 0.87x |
| mixed.json | msgspec | 0.058 | 0.064 | 0.097 | 98.438 | 0.74x |
| mixed.json | ujson | 0.178 | 0.187 | 0.228 | 98.438 | 0.26x |
| mixed.json | json | 0.358 | 0.417 | 0.456 | 98.438 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.312 | 8.468 | 17.013 | 92.828 | 1.00x |
| users.json | orjson | 10.660 | 13.036 | 26.472 | 92.828 | 0.65x |
| users.json | msgspec | 11.100 | 13.609 | 24.680 | 92.828 | 0.62x |
| users.json | ujson | 15.161 | 21.163 | 36.070 | 92.828 | 0.40x |
| users.json | json | 18.321 | 20.638 | 33.863 | 92.828 | 0.41x |
| flat.json | strata | 0.700 | 0.821 | 1.159 | 95.672 | 1.00x |
| flat.json | orjson | 1.016 | 1.296 | 2.230 | 95.672 | 0.63x |
| flat.json | msgspec | 0.938 | 1.106 | 1.846 | 95.672 | 0.74x |
| flat.json | ujson | 1.324 | 1.521 | 3.532 | 95.672 | 0.54x |
| flat.json | json | 1.617 | 1.783 | 3.397 | 95.672 | 0.46x |
| nested.json | strata | 0.704 | 0.771 | 0.844 | 95.672 | 1.00x |
| nested.json | orjson | 1.150 | 1.278 | 1.323 | 95.672 | 0.60x |
| nested.json | msgspec | 0.928 | 0.978 | 1.074 | 95.672 | 0.79x |
| nested.json | ujson | 1.256 | 1.347 | 1.532 | 95.672 | 0.57x |
| nested.json | json | 1.769 | 1.833 | 2.054 | 95.672 | 0.42x |
| wide_arrays.json | strata | 3.224 | 3.656 | 3.956 | 98.422 | 1.00x |
| wide_arrays.json | orjson | 3.881 | 4.364 | 4.667 | 98.422 | 0.84x |
| wide_arrays.json | msgspec | 4.481 | 5.103 | 5.381 | 98.422 | 0.72x |
| wide_arrays.json | ujson | 5.705 | 6.389 | 7.157 | 98.422 | 0.57x |
| wide_arrays.json | json | 7.452 | 8.264 | 13.612 | 98.422 | 0.44x |
| mixed.json | strata | 0.183 | 0.221 | 0.260 | 98.438 | 1.00x |
| mixed.json | orjson | 0.249 | 0.265 | 0.460 | 98.438 | 0.83x |
| mixed.json | msgspec | 0.237 | 0.267 | 0.331 | 98.438 | 0.83x |
| mixed.json | ujson | 0.443 | 0.517 | 0.652 | 98.438 | 0.43x |
| mixed.json | json | 0.409 | 0.473 | 0.560 | 98.438 | 0.47x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.709 | 8.327 | 10.999 | 95.656 | 1.00x |
| users.ndjson | orjson | 12.378 | 13.906 | 17.268 | 95.656 | 0.60x |
| users.ndjson | msgspec | 12.416 | 13.833 | 21.787 | 95.656 | 0.60x |
| users.ndjson | ujson | 15.447 | 17.146 | 30.325 | 95.656 | 0.49x |
| users.ndjson | json | 20.940 | 22.147 | 28.005 | 95.656 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.644 | 3.847 | 6.487 | 91.000 | 1.00x |
| users.json | orjson | 3.952 | 4.689 | 8.530 | 91.000 | 0.82x |
| users.json | msgspec | 4.241 | 5.330 | 14.346 | 91.000 | 0.72x |
| users.json | ujson | 11.177 | 14.294 | 20.069 | 91.000 | 0.27x |
| users.json | json | 18.246 | 24.862 | 31.769 | 91.000 | 0.15x |
| flat.json | strata | 0.489 | 0.650 | 1.058 | 95.672 | 1.00x |
| flat.json | orjson | 0.614 | 0.904 | 1.924 | 95.672 | 0.72x |
| flat.json | msgspec | 0.638 | 0.813 | 1.781 | 95.672 | 0.80x |
| flat.json | ujson | 1.203 | 1.575 | 3.411 | 95.672 | 0.41x |
| flat.json | json | 1.920 | 2.163 | 3.423 | 95.672 | 0.30x |
| nested.json | strata | 0.403 | 0.489 | 1.271 | 95.672 | 1.00x |
| nested.json | orjson | 0.486 | 0.615 | 1.043 | 95.672 | 0.79x |
| nested.json | msgspec | 0.584 | 0.870 | 1.216 | 95.672 | 0.56x |
| nested.json | ujson | 1.227 | 1.514 | 3.595 | 95.672 | 0.32x |
| nested.json | json | 2.058 | 2.356 | 2.852 | 95.672 | 0.21x |
| wide_arrays.json | strata | 1.411 | 1.800 | 3.603 | 98.422 | 1.00x |
| wide_arrays.json | orjson | 1.854 | 2.087 | 2.485 | 98.422 | 0.86x |
| wide_arrays.json | msgspec | 2.787 | 3.013 | 4.559 | 98.422 | 0.60x |
| wide_arrays.json | ujson | 5.290 | 6.228 | 6.844 | 98.422 | 0.29x |
| wide_arrays.json | json | 12.555 | 14.350 | 19.080 | 98.422 | 0.13x |
| mixed.json | strata | 0.249 | 0.293 | 0.464 | 98.438 | 1.00x |
| mixed.json | orjson | 0.230 | 0.318 | 0.599 | 98.438 | 0.92x |
| mixed.json | msgspec | 0.218 | 0.300 | 0.409 | 98.438 | 0.98x |
| mixed.json | ujson | 0.398 | 0.455 | 0.599 | 98.438 | 0.64x |
| mixed.json | json | 0.540 | 0.674 | 0.858 | 98.438 | 0.43x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.118 | 0.147 | 0.256 | 91.031 | 1.00x |
| users.json $[*].id | jmespath | 0.437 | 0.527 | 1.076 | 91.031 | 0.28x |
| users.json $[*].id | jsonpath-ng | 1.831 | 2.142 | 4.650 | 91.031 | 0.07x |
| users.json $[*].orders[*].total | strata | 0.590 | 0.884 | 2.299 | 91.250 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.112 | 2.500 | 7.558 | 91.250 | 0.35x |
| users.json $[*].orders[*].total | jsonpath-ng | 13.869 | 17.010 | 29.202 | 91.250 | 0.05x |
| users.json $..total | strata | 1.474 | 1.558 | 2.876 | 92.109 | 1.00x |
| users.json $..total | jsonpath-ng | 212.442 | 231.136 | 244.391 | 92.109 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.870 | 4.393 | 6.590 | 91.125 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.477 | 16.467 | 28.711 | 91.125 | 0.27x |
| users.json $[*].id | orjson+jsonpath-ng | 12.004 | 16.889 | 21.080 | 91.125 | 0.26x |
| users.json $[*].orders[*].total | strata | 4.399 | 4.545 | 8.680 | 92.094 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 13.094 | 15.055 | 33.437 | 92.094 | 0.30x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 30.097 | 32.821 | 46.978 | 92.094 | 0.14x |
| users.json $..total | strata | 8.831 | 9.944 | 12.551 | 92.109 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 231.224 | 239.559 | 250.039 | 92.109 | 0.04x |

