# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 53fa480
- python: 3.12.10
- implementation: CPython
- platform: macOS-15.7.9-x86_64-i386-64bit
- machine: x86_64
- processor: Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 23.147 | 24.315 | 30.920 | 52.590 | 1.00x |
| users.json | orjson | 32.670 | 35.852 | 39.971 | 52.590 | 0.68x |
| users.json | msgspec | 33.890 | 35.967 | 38.256 | 52.590 | 0.68x |
| users.json | ujson | 48.160 | 50.847 | 64.864 | 52.590 | 0.48x |
| users.json | pysimdjson | 209.068 | 216.164 | 231.534 | 52.590 | 0.11x |
| users.json | json | 54.738 | 58.420 | 63.019 | 52.590 | 0.42x |
| flat.json | strata | 1.322 | 1.337 | 1.489 | 60.758 | 1.00x |
| flat.json | orjson | 1.445 | 1.474 | 1.517 | 60.758 | 0.91x |
| flat.json | msgspec | 1.661 | 1.671 | 1.775 | 60.758 | 0.80x |
| flat.json | ujson | 2.909 | 2.954 | 3.321 | 60.758 | 0.45x |
| flat.json | pysimdjson | 15.896 | 15.982 | 16.646 | 60.758 | 0.08x |
| flat.json | json | 3.366 | 3.426 | 3.735 | 60.758 | 0.39x |
| nested.json | strata | 1.685 | 1.787 | 1.967 | 49.133 | 1.00x |
| nested.json | orjson | 1.925 | 2.013 | 2.576 | 49.133 | 0.89x |
| nested.json | msgspec | 2.123 | 2.215 | 2.379 | 49.133 | 0.81x |
| nested.json | ujson | 3.588 | 3.768 | 4.107 | 49.133 | 0.47x |
| nested.json | pysimdjson | 16.027 | 16.303 | 17.142 | 49.133 | 0.11x |
| nested.json | json | 4.546 | 4.705 | 5.177 | 49.133 | 0.38x |
| wide_arrays.json | strata | 8.903 | 9.359 | 10.015 | 60.582 | 1.00x |
| wide_arrays.json | orjson | 10.286 | 10.997 | 11.587 | 60.582 | 0.85x |
| wide_arrays.json | msgspec | 11.427 | 12.114 | 12.566 | 60.582 | 0.77x |
| wide_arrays.json | ujson | 14.291 | 15.184 | 15.419 | 60.582 | 0.62x |
| wide_arrays.json | pysimdjson | 91.415 | 93.991 | 96.433 | 60.582 | 0.10x |
| wide_arrays.json | json | 19.125 | 20.142 | 21.259 | 60.582 | 0.46x |
| mixed.json | strata | 0.373 | 0.387 | 0.424 | 55.840 | 1.00x |
| mixed.json | orjson | 0.458 | 0.474 | 0.498 | 55.840 | 0.82x |
| mixed.json | msgspec | 0.488 | 0.497 | 0.573 | 55.840 | 0.78x |
| mixed.json | ujson | 0.666 | 0.687 | 0.740 | 55.840 | 0.56x |
| mixed.json | pysimdjson | 3.501 | 3.526 | 3.877 | 55.840 | 0.11x |
| mixed.json | json | 0.949 | 0.976 | 1.124 | 55.840 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.174 | 3.349 | 4.925 | 48.070 | 1.00x |
| users.json | orjson | 4.357 | 4.583 | 6.863 | 48.070 | 0.73x |
| users.json | msgspec | 6.537 | 7.690 | 9.645 | 48.070 | 0.44x |
| users.json | ujson | 31.430 | 33.261 | 40.943 | 48.070 | 0.10x |
| users.json | json | 54.881 | 56.748 | 61.621 | 48.070 | 0.06x |
| flat.json | strata | 0.316 | 0.350 | 0.384 | 48.703 | 1.00x |
| flat.json | orjson | 0.419 | 0.501 | 0.991 | 48.703 | 0.70x |
| flat.json | msgspec | 0.560 | 0.581 | 0.649 | 48.703 | 0.60x |
| flat.json | ujson | 2.370 | 2.481 | 3.058 | 48.703 | 0.14x |
| flat.json | json | 3.946 | 3.985 | 4.048 | 48.703 | 0.09x |
| nested.json | strata | 0.278 | 0.290 | 0.353 | 48.988 | 1.00x |
| nested.json | orjson | 0.411 | 0.423 | 0.485 | 48.988 | 0.69x |
| nested.json | msgspec | 0.644 | 0.675 | 0.805 | 48.988 | 0.43x |
| nested.json | ujson | 2.731 | 2.769 | 3.024 | 48.988 | 0.10x |
| nested.json | json | 5.461 | 5.516 | 6.170 | 48.988 | 0.05x |
| wide_arrays.json | strata | 1.789 | 1.874 | 2.190 | 58.820 | 1.00x |
| wide_arrays.json | orjson | 2.425 | 2.488 | 2.738 | 58.820 | 0.75x |
| wide_arrays.json | msgspec | 3.539 | 3.604 | 4.101 | 58.820 | 0.52x |
| wide_arrays.json | ujson | 11.408 | 11.618 | 11.874 | 58.820 | 0.16x |
| wide_arrays.json | json | 38.190 | 38.478 | 39.953 | 58.820 | 0.05x |
| mixed.json | strata | 0.066 | 0.069 | 0.084 | 53.629 | 1.00x |
| mixed.json | orjson | 0.080 | 0.084 | 0.099 | 53.629 | 0.81x |
| mixed.json | msgspec | 0.116 | 0.118 | 0.131 | 53.629 | 0.58x |
| mixed.json | ujson | 0.493 | 0.496 | 0.755 | 53.629 | 0.14x |
| mixed.json | json | 1.038 | 1.048 | 1.098 | 53.629 | 0.07x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 23.783 | 24.087 | 34.403 | 60.535 | 1.00x |
| users.json | orjson | 34.368 | 36.501 | 42.763 | 60.535 | 0.66x |
| users.json | msgspec | 34.032 | 35.666 | 39.462 | 60.535 | 0.68x |
| users.json | ujson | 50.380 | 54.369 | 62.807 | 60.535 | 0.44x |
| users.json | json | 57.010 | 62.379 | 108.977 | 60.535 | 0.39x |
| flat.json | strata | 1.466 | 1.503 | 1.693 | 48.891 | 1.00x |
| flat.json | orjson | 1.647 | 1.681 | 2.913 | 48.891 | 0.89x |
| flat.json | msgspec | 1.863 | 1.902 | 2.338 | 48.891 | 0.79x |
| flat.json | ujson | 3.189 | 3.268 | 4.107 | 48.891 | 0.46x |
| flat.json | json | 3.557 | 3.595 | 5.619 | 48.891 | 0.42x |
| nested.json | strata | 1.774 | 1.855 | 1.916 | 48.988 | 1.00x |
| nested.json | orjson | 2.061 | 2.164 | 2.356 | 48.988 | 0.86x |
| nested.json | msgspec | 2.325 | 2.365 | 2.576 | 48.988 | 0.78x |
| nested.json | ujson | 3.718 | 3.781 | 4.327 | 48.988 | 0.49x |
| nested.json | json | 4.676 | 4.777 | 4.916 | 48.988 | 0.39x |
| wide_arrays.json | strata | 8.509 | 8.658 | 9.703 | 58.945 | 1.00x |
| wide_arrays.json | orjson | 9.988 | 10.720 | 11.462 | 58.945 | 0.81x |
| wide_arrays.json | msgspec | 11.221 | 11.647 | 12.383 | 58.945 | 0.74x |
| wide_arrays.json | ujson | 14.319 | 14.786 | 15.942 | 58.945 | 0.59x |
| wide_arrays.json | json | 18.625 | 19.140 | 21.285 | 58.945 | 0.45x |
| mixed.json | strata | 0.424 | 0.458 | 0.498 | 53.629 | 1.00x |
| mixed.json | orjson | 0.569 | 0.593 | 0.645 | 53.629 | 0.77x |
| mixed.json | msgspec | 0.591 | 0.664 | 0.711 | 53.629 | 0.69x |
| mixed.json | ujson | 0.774 | 0.849 | 0.873 | 53.629 | 0.54x |
| mixed.json | json | 1.026 | 1.081 | 1.128 | 53.629 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 20.812 | 22.424 | 25.500 | 61.230 | 1.00x |
| users.ndjson | orjson | 30.293 | 32.666 | 36.088 | 61.230 | 0.69x |
| users.ndjson | msgspec | 30.639 | 32.265 | 34.363 | 61.230 | 0.70x |
| users.ndjson | ujson | 43.519 | 44.998 | 47.620 | 61.230 | 0.50x |
| users.ndjson | json | 55.428 | 57.979 | 63.821 | 61.230 | 0.39x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.403 | 4.537 | 5.585 | 58.598 | 1.00x |
| users.json | orjson | 5.809 | 6.026 | 6.942 | 58.598 | 0.75x |
| users.json | msgspec | 8.196 | 8.331 | 8.734 | 58.598 | 0.54x |
| users.json | ujson | 33.617 | 33.838 | 41.319 | 58.598 | 0.13x |
| users.json | json | 56.614 | 60.716 | 79.465 | 58.598 | 0.07x |
| flat.json | strata | 0.751 | 0.823 | 0.905 | 48.891 | 1.00x |
| flat.json | orjson | 0.900 | 0.926 | 1.241 | 48.891 | 0.89x |
| flat.json | msgspec | 1.057 | 1.114 | 1.162 | 48.891 | 0.74x |
| flat.json | ujson | 2.986 | 3.258 | 3.500 | 48.891 | 0.25x |
| flat.json | json | 4.729 | 5.166 | 7.035 | 48.891 | 0.16x |
| nested.json | strata | 0.610 | 0.649 | 0.723 | 48.988 | 1.00x |
| nested.json | orjson | 0.784 | 0.802 | 0.867 | 48.988 | 0.81x |
| nested.json | msgspec | 0.980 | 1.053 | 1.114 | 48.988 | 0.62x |
| nested.json | ujson | 3.111 | 3.181 | 3.250 | 48.988 | 0.20x |
| nested.json | json | 5.903 | 6.000 | 6.575 | 48.988 | 0.11x |
| wide_arrays.json | strata | 2.462 | 2.661 | 2.833 | 58.945 | 1.00x |
| wide_arrays.json | orjson | 3.311 | 3.527 | 3.885 | 58.945 | 0.75x |
| wide_arrays.json | msgspec | 4.380 | 4.519 | 6.575 | 58.945 | 0.59x |
| wide_arrays.json | ujson | 12.109 | 12.598 | 13.827 | 58.945 | 0.21x |
| wide_arrays.json | json | 38.159 | 38.578 | 43.201 | 58.945 | 0.07x |
| mixed.json | strata | 0.314 | 0.346 | 0.456 | 53.629 | 1.00x |
| mixed.json | orjson | 0.357 | 0.376 | 0.624 | 53.629 | 0.92x |
| mixed.json | msgspec | 0.398 | 0.449 | 0.671 | 53.629 | 0.77x |
| mixed.json | ujson | 0.791 | 0.847 | 0.952 | 53.629 | 0.41x |
| mixed.json | json | 1.300 | 1.364 | 1.484 | 53.629 | 0.25x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.198 | 0.220 | 0.310 | 58.695 | 1.00x |
| users.json $[*].id | jmespath | 1.251 | 1.307 | 1.417 | 58.695 | 0.17x |
| users.json $[*].id | jsonpath-ng | 6.771 | 6.947 | 9.834 | 58.695 | 0.03x |
| users.json $[*].orders[*].total | strata | 1.313 | 1.387 | 2.570 | 56.074 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 7.659 | 8.250 | 19.424 | 56.074 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 45.374 | 50.318 | 56.005 | 56.074 | 0.03x |
| users.json $..total | strata | 4.350 | 4.717 | 13.908 | 55.352 | 1.00x |
| users.json $..total | jsonpath-ng | 916.737 | 978.624 | 1257.295 | 55.352 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 5.051 | 5.263 | 6.208 | 58.902 | 1.00x |
| users.json $[*].id | orjson+jmespath | 34.372 | 37.188 | 40.645 | 58.902 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 41.105 | 42.297 | 49.950 | 58.902 | 0.12x |
| users.json $[*].orders[*].total | strata | 5.357 | 5.982 | 7.418 | 56.328 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 43.276 | 50.501 | 55.227 | 56.328 | 0.12x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 92.150 | 97.615 | 131.232 | 56.328 | 0.06x |
| users.json $..total | strata | 25.427 | 28.593 | 37.859 | 56.359 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 819.417 | 995.493 | 1118.729 | 56.359 | 0.03x |

