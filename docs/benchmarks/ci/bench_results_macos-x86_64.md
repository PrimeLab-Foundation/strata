# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 8cdb525
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
| users.json | strata | 23.923 | 24.297 | 28.628 | 52.605 | 1.00x |
| users.json | orjson | 29.394 | 31.801 | 35.903 | 52.605 | 0.76x |
| users.json | msgspec | 29.167 | 31.085 | 34.865 | 52.605 | 0.78x |
| users.json | ujson | 43.821 | 46.310 | 49.525 | 52.605 | 0.52x |
| users.json | pysimdjson | 197.416 | 201.474 | 210.649 | 52.605 | 0.12x |
| users.json | json | 51.296 | 52.157 | 63.020 | 52.605 | 0.47x |
| flat.json | strata | 1.447 | 1.454 | 1.821 | 60.090 | 1.00x |
| flat.json | orjson | 1.483 | 1.514 | 1.562 | 60.090 | 0.96x |
| flat.json | msgspec | 1.755 | 1.770 | 1.971 | 60.090 | 0.82x |
| flat.json | ujson | 3.023 | 3.067 | 3.254 | 60.090 | 0.47x |
| flat.json | pysimdjson | 16.353 | 16.462 | 16.808 | 60.090 | 0.09x |
| flat.json | json | 3.500 | 3.527 | 3.937 | 60.090 | 0.41x |
| nested.json | strata | 1.633 | 1.690 | 1.874 | 55.387 | 1.00x |
| nested.json | orjson | 1.785 | 1.872 | 2.026 | 55.387 | 0.90x |
| nested.json | msgspec | 1.957 | 2.060 | 2.248 | 55.387 | 0.82x |
| nested.json | ujson | 3.251 | 3.408 | 3.736 | 55.387 | 0.50x |
| nested.json | pysimdjson | 14.682 | 15.614 | 16.885 | 55.387 | 0.11x |
| nested.json | json | 4.188 | 4.393 | 5.117 | 55.387 | 0.38x |
| wide_arrays.json | strata | 9.260 | 10.092 | 10.547 | 59.668 | 1.00x |
| wide_arrays.json | orjson | 10.059 | 10.543 | 11.208 | 59.668 | 0.96x |
| wide_arrays.json | msgspec | 10.843 | 11.497 | 12.389 | 59.668 | 0.88x |
| wide_arrays.json | ujson | 14.267 | 15.415 | 16.053 | 59.668 | 0.65x |
| wide_arrays.json | pysimdjson | 85.379 | 94.532 | 97.574 | 59.668 | 0.11x |
| wide_arrays.json | json | 18.095 | 19.950 | 20.662 | 59.668 | 0.51x |
| mixed.json | strata | 0.424 | 0.443 | 0.474 | 56.891 | 1.00x |
| mixed.json | orjson | 0.486 | 0.519 | 0.908 | 56.891 | 0.85x |
| mixed.json | msgspec | 0.517 | 0.537 | 0.680 | 56.891 | 0.82x |
| mixed.json | ujson | 0.713 | 0.746 | 0.828 | 56.891 | 0.59x |
| mixed.json | pysimdjson | 3.703 | 3.829 | 3.914 | 56.891 | 0.12x |
| mixed.json | json | 1.000 | 1.050 | 1.083 | 56.891 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.330 | 3.465 | 3.670 | 51.410 | 1.00x |
| users.json | orjson | 4.173 | 4.353 | 5.177 | 51.410 | 0.80x |
| users.json | msgspec | 6.264 | 6.585 | 7.227 | 51.410 | 0.53x |
| users.json | ujson | 38.039 | 38.722 | 61.432 | 51.410 | 0.09x |
| users.json | json | 49.830 | 51.366 | 55.259 | 51.410 | 0.07x |
| flat.json | strata | 0.302 | 0.312 | 0.333 | 55.254 | 1.00x |
| flat.json | orjson | 0.407 | 0.421 | 0.476 | 55.254 | 0.74x |
| flat.json | msgspec | 0.547 | 0.563 | 0.623 | 55.254 | 0.55x |
| flat.json | ujson | 2.828 | 2.860 | 3.330 | 55.254 | 0.11x |
| flat.json | json | 3.913 | 3.941 | 4.042 | 55.254 | 0.08x |
| nested.json | strata | 0.254 | 0.265 | 0.294 | 55.523 | 1.00x |
| nested.json | orjson | 0.365 | 0.380 | 0.427 | 55.523 | 0.70x |
| nested.json | msgspec | 0.593 | 0.604 | 0.657 | 55.523 | 0.44x |
| nested.json | ujson | 3.003 | 3.109 | 3.504 | 55.523 | 0.09x |
| nested.json | json | 5.007 | 5.049 | 5.524 | 55.523 | 0.05x |
| wide_arrays.json | strata | 2.091 | 2.165 | 2.341 | 57.762 | 1.00x |
| wide_arrays.json | orjson | 2.909 | 3.026 | 3.335 | 57.762 | 0.72x |
| wide_arrays.json | msgspec | 3.498 | 3.649 | 3.786 | 57.762 | 0.59x |
| wide_arrays.json | ujson | 16.905 | 17.473 | 19.278 | 57.762 | 0.12x |
| wide_arrays.json | json | 38.024 | 38.542 | 42.098 | 57.762 | 0.06x |
| mixed.json | strata | 0.080 | 0.088 | 0.108 | 54.754 | 1.00x |
| mixed.json | orjson | 0.078 | 0.084 | 0.099 | 54.754 | 1.04x |
| mixed.json | msgspec | 0.116 | 0.122 | 0.137 | 54.754 | 0.72x |
| mixed.json | ujson | 0.698 | 0.705 | 0.721 | 54.754 | 0.12x |
| mixed.json | json | 1.090 | 1.099 | 1.150 | 54.754 | 0.08x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 23.072 | 24.370 | 26.445 | 61.266 | 1.00x |
| users.json | orjson | 28.825 | 31.328 | 34.151 | 61.266 | 0.78x |
| users.json | msgspec | 30.214 | 31.487 | 33.815 | 61.266 | 0.77x |
| users.json | ujson | 44.286 | 46.613 | 54.448 | 61.266 | 0.52x |
| users.json | json | 49.167 | 51.415 | 57.313 | 61.266 | 0.47x |
| flat.json | strata | 1.505 | 1.527 | 1.584 | 55.254 | 1.00x |
| flat.json | orjson | 1.555 | 1.621 | 1.701 | 55.254 | 0.94x |
| flat.json | msgspec | 1.860 | 1.882 | 1.952 | 55.254 | 0.81x |
| flat.json | ujson | 3.145 | 3.190 | 3.306 | 55.254 | 0.48x |
| flat.json | json | 3.532 | 3.564 | 3.771 | 55.254 | 0.43x |
| nested.json | strata | 1.735 | 1.864 | 2.093 | 55.523 | 1.00x |
| nested.json | orjson | 2.002 | 2.092 | 2.282 | 55.523 | 0.89x |
| nested.json | msgspec | 2.151 | 2.303 | 2.417 | 55.523 | 0.81x |
| nested.json | ujson | 3.473 | 3.751 | 3.836 | 55.523 | 0.50x |
| nested.json | json | 4.367 | 4.713 | 4.809 | 55.523 | 0.40x |
| wide_arrays.json | strata | 9.804 | 9.870 | 10.245 | 58.988 | 1.00x |
| wide_arrays.json | orjson | 10.285 | 10.383 | 10.638 | 58.988 | 0.95x |
| wide_arrays.json | msgspec | 11.519 | 11.663 | 12.070 | 58.988 | 0.85x |
| wide_arrays.json | ujson | 14.977 | 15.086 | 15.360 | 58.988 | 0.65x |
| wide_arrays.json | json | 18.966 | 19.204 | 19.772 | 58.988 | 0.51x |
| mixed.json | strata | 0.495 | 0.505 | 0.557 | 54.754 | 1.00x |
| mixed.json | orjson | 0.585 | 0.616 | 0.659 | 54.754 | 0.82x |
| mixed.json | msgspec | 0.629 | 0.645 | 0.731 | 54.754 | 0.78x |
| mixed.json | ujson | 0.825 | 0.855 | 0.987 | 54.754 | 0.59x |
| mixed.json | json | 1.080 | 1.112 | 1.136 | 54.754 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 23.944 | 24.296 | 24.710 | 58.488 | 1.00x |
| users.ndjson | orjson | 30.652 | 32.010 | 33.199 | 58.488 | 0.76x |
| users.ndjson | msgspec | 31.216 | 32.173 | 32.792 | 58.488 | 0.76x |
| users.ndjson | ujson | 45.103 | 46.225 | 47.042 | 58.488 | 0.53x |
| users.ndjson | json | 55.546 | 57.930 | 59.248 | 58.488 | 0.42x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.143 | 4.235 | 4.520 | 59.367 | 1.00x |
| users.json | orjson | 4.822 | 5.023 | 5.681 | 59.367 | 0.84x |
| users.json | msgspec | 6.850 | 7.336 | 8.740 | 59.367 | 0.58x |
| users.json | ujson | 37.039 | 37.636 | 38.245 | 59.367 | 0.11x |
| users.json | json | 47.760 | 49.303 | 51.382 | 59.367 | 0.09x |
| flat.json | strata | 0.629 | 0.690 | 0.742 | 55.254 | 1.00x |
| flat.json | orjson | 0.765 | 0.789 | 0.891 | 55.254 | 0.87x |
| flat.json | msgspec | 0.912 | 0.972 | 1.059 | 55.254 | 0.71x |
| flat.json | ujson | 3.273 | 3.314 | 3.369 | 55.254 | 0.21x |
| flat.json | json | 4.344 | 4.475 | 5.058 | 55.254 | 0.15x |
| nested.json | strata | 0.616 | 0.649 | 0.783 | 55.523 | 1.00x |
| nested.json | orjson | 0.754 | 0.805 | 1.173 | 55.523 | 0.81x |
| nested.json | msgspec | 0.960 | 1.049 | 1.132 | 55.523 | 0.62x |
| nested.json | ujson | 3.497 | 3.727 | 3.844 | 55.523 | 0.17x |
| nested.json | json | 5.671 | 6.005 | 6.252 | 55.523 | 0.11x |
| wide_arrays.json | strata | 2.980 | 3.126 | 3.282 | 57.828 | 1.00x |
| wide_arrays.json | orjson | 3.435 | 3.532 | 5.695 | 57.828 | 0.89x |
| wide_arrays.json | msgspec | 4.286 | 4.747 | 13.134 | 57.828 | 0.66x |
| wide_arrays.json | ujson | 18.244 | 21.331 | 49.959 | 57.828 | 0.15x |
| wide_arrays.json | json | 39.545 | 42.125 | 49.466 | 57.828 | 0.07x |
| mixed.json | strata | 0.349 | 0.380 | 0.448 | 54.754 | 1.00x |
| mixed.json | orjson | 0.364 | 0.404 | 0.477 | 54.754 | 0.94x |
| mixed.json | msgspec | 0.408 | 0.441 | 0.497 | 54.754 | 0.86x |
| mixed.json | ujson | 0.981 | 1.061 | 1.221 | 54.754 | 0.36x |
| mixed.json | json | 1.392 | 1.450 | 1.722 | 54.754 | 0.26x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.149 | 0.156 | 0.170 | 59.426 | 1.00x |
| users.json $[*].id | jmespath | 1.042 | 1.068 | 1.152 | 59.426 | 0.15x |
| users.json $[*].id | jsonpath-ng | 5.675 | 5.838 | 6.019 | 59.426 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.969 | 1.044 | 1.244 | 56.711 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 6.440 | 6.647 | 7.804 | 56.711 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 37.785 | 39.637 | 44.053 | 56.711 | 0.03x |
| users.json $..total | strata | 3.551 | 3.820 | 4.483 | 57.750 | 1.00x |
| users.json $..total | jsonpath-ng | 773.803 | 825.906 | 863.005 | 57.750 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.706 | 4.737 | 4.763 | 59.480 | 1.00x |
| users.json $[*].id | orjson+jmespath | 28.969 | 29.967 | 32.871 | 59.480 | 0.16x |
| users.json $[*].id | orjson+jsonpath-ng | 33.588 | 34.312 | 35.299 | 59.480 | 0.14x |
| users.json $[*].orders[*].total | strata | 4.873 | 4.922 | 5.130 | 56.934 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 34.052 | 36.610 | 39.710 | 56.934 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 71.393 | 75.027 | 84.270 | 56.934 | 0.07x |
| users.json $..total | strata | 25.981 | 27.058 | 28.924 | 56.973 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 792.419 | 828.328 | 881.274 | 56.973 | 0.03x |

