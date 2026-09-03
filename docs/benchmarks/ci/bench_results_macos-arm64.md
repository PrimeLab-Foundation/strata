# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 6ef7acf
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
| users.json | strata | 7.450 | 7.917 | 8.540 | 63.797 | 1.00x |
| users.json | orjson | 9.884 | 10.918 | 11.878 | 63.797 | 0.73x |
| users.json | msgspec | 9.934 | 10.771 | 13.256 | 63.797 | 0.74x |
| users.json | ujson | 14.343 | 14.865 | 17.212 | 63.797 | 0.53x |
| users.json | pysimdjson | 132.851 | 138.220 | 147.064 | 63.797 | 0.06x |
| users.json | json | 16.813 | 17.616 | 19.131 | 63.797 | 0.45x |
| flat.json | strata | 0.574 | 0.683 | 0.777 | 85.297 | 1.00x |
| flat.json | orjson | 0.687 | 0.820 | 0.908 | 85.297 | 0.83x |
| flat.json | msgspec | 0.685 | 0.826 | 0.906 | 85.297 | 0.83x |
| flat.json | ujson | 1.103 | 1.348 | 1.409 | 85.297 | 0.51x |
| flat.json | pysimdjson | 12.171 | 13.026 | 13.801 | 85.297 | 0.05x |
| flat.json | json | 1.301 | 1.453 | 1.707 | 85.297 | 0.47x |
| nested.json | strata | 0.563 | 0.620 | 1.837 | 85.312 | 1.00x |
| nested.json | orjson | 0.784 | 0.831 | 0.898 | 85.312 | 0.75x |
| nested.json | msgspec | 0.727 | 0.790 | 0.880 | 85.312 | 0.78x |
| nested.json | ujson | 1.110 | 1.237 | 1.435 | 85.312 | 0.50x |
| nested.json | pysimdjson | 11.313 | 11.654 | 12.238 | 85.312 | 0.05x |
| nested.json | json | 1.540 | 1.717 | 2.056 | 85.312 | 0.36x |
| wide_arrays.json | strata | 3.708 | 4.089 | 4.369 | 88.078 | 1.00x |
| wide_arrays.json | orjson | 3.884 | 4.223 | 4.575 | 88.078 | 0.97x |
| wide_arrays.json | msgspec | 4.350 | 4.805 | 5.788 | 88.078 | 0.85x |
| wide_arrays.json | ujson | 5.800 | 6.101 | 6.680 | 88.078 | 0.67x |
| wide_arrays.json | pysimdjson | 67.052 | 68.557 | 76.275 | 88.078 | 0.06x |
| wide_arrays.json | json | 7.442 | 7.754 | 8.505 | 88.078 | 0.53x |
| mixed.json | strata | 0.145 | 0.152 | 0.175 | 88.141 | 1.00x |
| mixed.json | orjson | 0.159 | 0.186 | 0.210 | 88.141 | 0.82x |
| mixed.json | msgspec | 0.176 | 0.189 | 0.203 | 88.141 | 0.80x |
| mixed.json | ujson | 0.231 | 0.367 | 0.459 | 88.141 | 0.41x |
| mixed.json | pysimdjson | 2.566 | 2.606 | 2.721 | 88.141 | 0.06x |
| mixed.json | json | 0.335 | 0.389 | 0.545 | 88.141 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.881 | 2.036 | 2.253 | 71.000 | 1.00x |
| users.json | orjson | 2.475 | 2.629 | 2.872 | 71.000 | 0.77x |
| users.json | msgspec | 3.088 | 3.312 | 3.957 | 71.000 | 0.61x |
| users.json | ujson | 11.691 | 12.055 | 14.465 | 71.000 | 0.17x |
| users.json | json | 16.825 | 17.378 | 19.017 | 71.000 | 0.12x |
| flat.json | strata | 0.223 | 0.252 | 0.305 | 85.312 | 1.00x |
| flat.json | orjson | 0.271 | 0.330 | 0.529 | 85.312 | 0.76x |
| flat.json | msgspec | 0.330 | 0.361 | 0.536 | 85.312 | 0.70x |
| flat.json | ujson | 0.978 | 1.069 | 1.197 | 85.312 | 0.24x |
| flat.json | json | 1.421 | 1.643 | 1.826 | 85.312 | 0.15x |
| nested.json | strata | 0.142 | 0.163 | 0.213 | 85.312 | 1.00x |
| nested.json | orjson | 0.249 | 0.268 | 0.303 | 85.312 | 0.61x |
| nested.json | msgspec | 0.349 | 0.437 | 0.525 | 85.312 | 0.37x |
| nested.json | ujson | 1.090 | 1.126 | 1.186 | 85.312 | 0.14x |
| nested.json | json | 1.804 | 1.866 | 1.954 | 85.312 | 0.09x |
| wide_arrays.json | strata | 1.215 | 1.301 | 1.514 | 88.078 | 1.00x |
| wide_arrays.json | orjson | 1.600 | 1.695 | 2.999 | 88.078 | 0.77x |
| wide_arrays.json | msgspec | 2.199 | 2.327 | 3.568 | 88.078 | 0.56x |
| wide_arrays.json | ujson | 6.868 | 7.224 | 7.827 | 88.078 | 0.18x |
| wide_arrays.json | json | 12.091 | 12.873 | 17.125 | 88.078 | 0.10x |
| mixed.json | strata | 0.045 | 0.047 | 0.055 | 88.141 | 1.00x |
| mixed.json | orjson | 0.047 | 0.053 | 0.069 | 88.141 | 0.88x |
| mixed.json | msgspec | 0.054 | 0.134 | 0.226 | 88.141 | 0.35x |
| mixed.json | ujson | 0.224 | 0.240 | 0.258 | 88.141 | 0.19x |
| mixed.json | json | 0.358 | 0.372 | 0.394 | 88.141 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.936 | 8.502 | 9.548 | 83.000 | 1.00x |
| users.json | orjson | 11.067 | 11.870 | 17.891 | 83.000 | 0.72x |
| users.json | msgspec | 10.878 | 11.274 | 12.005 | 83.000 | 0.75x |
| users.json | ujson | 14.952 | 15.713 | 19.485 | 83.000 | 0.54x |
| users.json | json | 17.276 | 18.701 | 24.644 | 83.000 | 0.45x |
| flat.json | strata | 0.684 | 0.760 | 0.832 | 85.312 | 1.00x |
| flat.json | orjson | 0.954 | 1.061 | 1.133 | 85.312 | 0.72x |
| flat.json | msgspec | 0.839 | 0.970 | 1.101 | 85.312 | 0.78x |
| flat.json | ujson | 1.222 | 1.334 | 1.445 | 85.312 | 0.57x |
| flat.json | json | 1.503 | 1.577 | 1.735 | 85.312 | 0.48x |
| nested.json | strata | 0.654 | 0.688 | 0.779 | 85.312 | 1.00x |
| nested.json | orjson | 0.980 | 1.048 | 1.169 | 85.312 | 0.66x |
| nested.json | msgspec | 0.833 | 0.885 | 0.994 | 85.312 | 0.78x |
| nested.json | ujson | 1.079 | 1.225 | 1.598 | 85.312 | 0.56x |
| nested.json | json | 1.628 | 1.676 | 1.776 | 85.312 | 0.41x |
| wide_arrays.json | strata | 3.906 | 4.083 | 9.414 | 88.078 | 1.00x |
| wide_arrays.json | orjson | 4.054 | 4.429 | 5.659 | 88.078 | 0.92x |
| wide_arrays.json | msgspec | 4.852 | 5.396 | 8.341 | 88.078 | 0.76x |
| wide_arrays.json | ujson | 6.386 | 6.580 | 9.791 | 88.078 | 0.62x |
| wide_arrays.json | json | 7.699 | 7.962 | 10.643 | 88.078 | 0.51x |
| mixed.json | strata | 0.191 | 0.209 | 0.246 | 88.141 | 1.00x |
| mixed.json | orjson | 0.273 | 0.415 | 0.481 | 88.141 | 0.50x |
| mixed.json | msgspec | 0.246 | 0.291 | 0.350 | 88.141 | 0.72x |
| mixed.json | ujson | 0.300 | 0.369 | 0.504 | 88.141 | 0.57x |
| mixed.json | json | 0.415 | 0.454 | 0.525 | 88.141 | 0.46x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.808 | 8.359 | 9.851 | 85.281 | 1.00x |
| users.ndjson | orjson | 12.481 | 13.335 | 16.487 | 85.281 | 0.63x |
| users.ndjson | msgspec | 12.357 | 12.749 | 21.589 | 85.281 | 0.66x |
| users.ndjson | ujson | 15.513 | 16.072 | 20.424 | 85.281 | 0.52x |
| users.ndjson | json | 19.856 | 21.162 | 21.818 | 85.281 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.410 | 2.715 | 3.409 | 78.625 | 1.00x |
| users.json | orjson | 3.092 | 3.416 | 4.031 | 78.625 | 0.79x |
| users.json | msgspec | 3.768 | 4.062 | 4.918 | 78.625 | 0.67x |
| users.json | ujson | 12.321 | 13.009 | 14.506 | 78.625 | 0.21x |
| users.json | json | 17.452 | 18.000 | 22.000 | 78.625 | 0.15x |
| flat.json | strata | 0.454 | 0.514 | 0.655 | 85.312 | 1.00x |
| flat.json | orjson | 0.533 | 0.644 | 0.732 | 85.312 | 0.80x |
| flat.json | msgspec | 0.512 | 0.640 | 0.693 | 85.312 | 0.80x |
| flat.json | ujson | 1.208 | 1.308 | 1.491 | 85.312 | 0.39x |
| flat.json | json | 1.711 | 1.913 | 2.114 | 85.312 | 0.27x |
| nested.json | strata | 0.359 | 0.391 | 0.497 | 85.312 | 1.00x |
| nested.json | orjson | 0.406 | 0.529 | 0.825 | 85.312 | 0.74x |
| nested.json | msgspec | 0.593 | 0.716 | 0.872 | 85.312 | 0.55x |
| nested.json | ujson | 1.303 | 1.411 | 1.777 | 85.312 | 0.28x |
| nested.json | json | 1.940 | 2.095 | 2.757 | 85.312 | 0.19x |
| wide_arrays.json | strata | 1.536 | 1.702 | 1.941 | 88.125 | 1.00x |
| wide_arrays.json | orjson | 1.867 | 2.096 | 2.896 | 88.125 | 0.81x |
| wide_arrays.json | msgspec | 2.537 | 2.896 | 4.417 | 88.125 | 0.59x |
| wide_arrays.json | ujson | 7.375 | 7.794 | 12.401 | 88.125 | 0.22x |
| wide_arrays.json | json | 12.057 | 13.926 | 21.298 | 88.125 | 0.12x |
| mixed.json | strata | 0.231 | 0.306 | 0.373 | 88.141 | 1.00x |
| mixed.json | orjson | 0.299 | 0.337 | 0.396 | 88.141 | 0.91x |
| mixed.json | msgspec | 0.303 | 0.396 | 0.501 | 88.141 | 0.77x |
| mixed.json | ujson | 0.446 | 0.529 | 0.613 | 88.141 | 0.58x |
| mixed.json | json | 0.577 | 0.689 | 1.097 | 88.141 | 0.44x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.068 | 0.102 | 0.344 | 78.656 | 1.00x |
| users.json $[*].id | jmespath | 0.330 | 0.352 | 0.381 | 78.656 | 0.29x |
| users.json $[*].id | jsonpath-ng | 1.596 | 1.659 | 2.907 | 78.656 | 0.06x |
| users.json $[*].orders[*].total | strata | 0.443 | 0.575 | 0.680 | 79.781 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.853 | 2.036 | 2.287 | 79.781 | 0.28x |
| users.json $[*].orders[*].total | jsonpath-ng | 11.680 | 12.271 | 12.748 | 79.781 | 0.05x |
| users.json $..total | strata | 1.328 | 1.442 | 1.747 | 81.688 | 1.00x |
| users.json $..total | jsonpath-ng | 196.730 | 206.469 | 250.571 | 81.688 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.874 | 4.005 | 4.593 | 79.734 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.751 | 11.686 | 12.730 | 79.734 | 0.34x |
| users.json $[*].id | orjson+jsonpath-ng | 12.536 | 12.990 | 16.108 | 79.734 | 0.31x |
| users.json $[*].orders[*].total | strata | 4.248 | 4.524 | 4.760 | 81.594 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 13.407 | 13.894 | 17.116 | 81.594 | 0.33x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 27.302 | 28.860 | 34.047 | 81.594 | 0.16x |
| users.json $..total | strata | 9.455 | 9.874 | 10.778 | 81.688 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 217.996 | 226.677 | 235.248 | 81.688 | 0.04x |

