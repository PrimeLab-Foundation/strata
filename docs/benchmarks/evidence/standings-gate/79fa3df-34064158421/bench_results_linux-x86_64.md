# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 79fa3df
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
| users.json | strata | 9.517 | 9.790 | 13.260 | 59.219 | 1.00x |
| users.json | orjson | 13.100 | 13.355 | 15.834 | 59.219 | 0.73x |
| users.json | msgspec | 13.060 | 13.406 | 15.584 | 59.219 | 0.73x |
| users.json | ujson | 17.782 | 18.569 | 22.421 | 59.219 | 0.53x |
| users.json | pysimdjson | 18.244 | 19.018 | 23.596 | 59.219 | 0.51x |
| users.json | json | 22.428 | 22.802 | 24.195 | 59.219 | 0.43x |
| flat.json | strata | 0.829 | 0.844 | 0.865 | 67.465 | 1.00x |
| flat.json | orjson | 0.979 | 0.984 | 0.996 | 67.465 | 0.86x |
| flat.json | msgspec | 1.016 | 1.036 | 1.049 | 67.465 | 0.81x |
| flat.json | ujson | 1.453 | 1.476 | 1.520 | 67.465 | 0.57x |
| flat.json | pysimdjson | 1.544 | 1.576 | 1.672 | 67.465 | 0.54x |
| flat.json | json | 1.904 | 1.915 | 2.247 | 67.465 | 0.44x |
| nested.json | strata | 0.787 | 0.804 | 0.840 | 67.465 | 1.00x |
| nested.json | orjson | 0.984 | 0.990 | 1.003 | 67.465 | 0.81x |
| nested.json | msgspec | 1.008 | 1.015 | 1.049 | 67.465 | 0.79x |
| nested.json | ujson | 1.420 | 1.433 | 1.477 | 67.465 | 0.56x |
| nested.json | pysimdjson | 1.379 | 1.393 | 1.457 | 67.465 | 0.58x |
| nested.json | json | 2.033 | 2.048 | 2.060 | 67.465 | 0.39x |
| wide_arrays.json | strata | 4.030 | 4.073 | 4.165 | 73.801 | 1.00x |
| wide_arrays.json | orjson | 5.035 | 5.119 | 5.251 | 73.801 | 0.80x |
| wide_arrays.json | msgspec | 5.637 | 5.703 | 5.756 | 73.801 | 0.71x |
| wide_arrays.json | ujson | 6.975 | 7.094 | 7.215 | 73.801 | 0.57x |
| wide_arrays.json | pysimdjson | 6.082 | 6.126 | 6.166 | 73.801 | 0.66x |
| wide_arrays.json | json | 9.657 | 9.723 | 9.784 | 73.801 | 0.42x |
| mixed.json | strata | 0.190 | 0.192 | 0.206 | 73.863 | 1.00x |
| mixed.json | orjson | 0.228 | 0.231 | 0.252 | 73.863 | 0.83x |
| mixed.json | msgspec | 0.239 | 0.242 | 0.276 | 73.863 | 0.80x |
| mixed.json | ujson | 0.299 | 0.301 | 0.317 | 73.863 | 0.64x |
| mixed.json | pysimdjson | 0.293 | 0.296 | 0.335 | 73.863 | 0.65x |
| mixed.json | json | 0.474 | 0.488 | 0.556 | 73.863 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.473 | 2.499 | 2.686 | 58.289 | 1.00x |
| users.json | orjson | 2.897 | 2.904 | 2.935 | 58.289 | 0.86x |
| users.json | msgspec | 3.837 | 3.865 | 3.920 | 58.289 | 0.65x |
| users.json | ujson | 11.286 | 11.500 | 11.697 | 58.289 | 0.22x |
| users.json | json | 21.650 | 21.802 | 22.226 | 58.289 | 0.11x |
| flat.json | strata | 0.275 | 0.279 | 0.472 | 67.465 | 1.00x |
| flat.json | orjson | 0.323 | 0.336 | 0.600 | 67.465 | 0.83x |
| flat.json | msgspec | 0.423 | 0.436 | 0.991 | 67.465 | 0.64x |
| flat.json | ujson | 1.021 | 1.026 | 1.667 | 67.465 | 0.27x |
| flat.json | json | 1.822 | 1.849 | 2.611 | 67.465 | 0.15x |
| nested.json | strata | 0.261 | 0.265 | 0.282 | 67.465 | 1.00x |
| nested.json | orjson | 0.292 | 0.294 | 0.315 | 67.465 | 0.90x |
| nested.json | msgspec | 0.402 | 0.414 | 0.430 | 67.465 | 0.64x |
| nested.json | ujson | 1.069 | 1.090 | 1.121 | 67.465 | 0.24x |
| nested.json | json | 2.363 | 2.387 | 2.404 | 67.465 | 0.11x |
| wide_arrays.json | strata | 1.556 | 1.578 | 1.598 | 73.801 | 1.00x |
| wide_arrays.json | orjson | 1.793 | 1.804 | 1.820 | 73.801 | 0.88x |
| wide_arrays.json | msgspec | 2.661 | 2.676 | 2.692 | 73.801 | 0.59x |
| wide_arrays.json | ujson | 6.299 | 6.333 | 6.370 | 73.801 | 0.25x |
| wide_arrays.json | json | 16.394 | 16.486 | 17.069 | 73.801 | 0.10x |
| mixed.json | strata | 0.062 | 0.063 | 0.064 | 73.863 | 1.00x |
| mixed.json | orjson | 0.064 | 0.065 | 0.098 | 73.863 | 0.97x |
| mixed.json | msgspec | 0.082 | 0.083 | 0.085 | 73.863 | 0.76x |
| mixed.json | ujson | 0.229 | 0.231 | 0.252 | 73.863 | 0.27x |
| mixed.json | json | 0.507 | 0.523 | 0.638 | 73.863 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.556 | 10.794 | 12.121 | 70.672 | 1.00x |
| users.json | orjson | 13.635 | 13.925 | 14.462 | 70.672 | 0.78x |
| users.json | msgspec | 13.842 | 13.987 | 15.088 | 70.672 | 0.77x |
| users.json | ujson | 18.667 | 19.031 | 22.743 | 70.672 | 0.57x |
| users.json | json | 23.130 | 23.375 | 24.017 | 70.672 | 0.46x |
| flat.json | strata | 0.861 | 0.873 | 0.893 | 67.465 | 1.00x |
| flat.json | orjson | 1.033 | 1.045 | 1.069 | 67.465 | 0.84x |
| flat.json | msgspec | 1.070 | 1.087 | 1.121 | 67.465 | 0.80x |
| flat.json | ujson | 1.544 | 1.562 | 1.588 | 67.465 | 0.56x |
| flat.json | json | 1.958 | 1.976 | 2.016 | 67.465 | 0.44x |
| nested.json | strata | 0.820 | 0.835 | 0.845 | 67.465 | 1.00x |
| nested.json | orjson | 1.037 | 1.051 | 1.074 | 67.465 | 0.80x |
| nested.json | msgspec | 1.067 | 1.086 | 1.102 | 67.465 | 0.77x |
| nested.json | ujson | 1.504 | 1.513 | 1.533 | 67.465 | 0.55x |
| nested.json | json | 2.090 | 2.105 | 2.135 | 67.465 | 0.40x |
| wide_arrays.json | strata | 4.143 | 4.211 | 4.350 | 73.863 | 1.00x |
| wide_arrays.json | orjson | 5.143 | 5.270 | 5.432 | 73.863 | 0.80x |
| wide_arrays.json | msgspec | 5.884 | 6.037 | 6.097 | 73.863 | 0.70x |
| wide_arrays.json | ujson | 7.222 | 7.328 | 7.394 | 73.863 | 0.57x |
| wide_arrays.json | json | 9.749 | 9.791 | 9.953 | 73.863 | 0.43x |
| mixed.json | strata | 0.207 | 0.210 | 0.227 | 73.863 | 1.00x |
| mixed.json | orjson | 0.272 | 0.282 | 0.292 | 73.863 | 0.74x |
| mixed.json | msgspec | 0.281 | 0.285 | 0.304 | 73.863 | 0.74x |
| mixed.json | ujson | 0.353 | 0.364 | 0.388 | 73.863 | 0.58x |
| mixed.json | json | 0.510 | 0.517 | 0.536 | 73.863 | 0.41x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.526 | 11.024 | 14.371 | 67.461 | 1.00x |
| users.ndjson | orjson | 16.935 | 17.398 | 21.839 | 67.461 | 0.63x |
| users.ndjson | msgspec | 16.511 | 17.253 | 17.724 | 67.461 | 0.64x |
| users.ndjson | ujson | 22.132 | 22.698 | 24.209 | 67.461 | 0.49x |
| users.ndjson | json | 30.015 | 30.392 | 37.435 | 67.461 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.086 | 3.123 | 3.190 | 67.719 | 1.00x |
| users.json | orjson | 3.536 | 3.576 | 3.604 | 67.719 | 0.87x |
| users.json | msgspec | 4.456 | 4.485 | 4.609 | 67.719 | 0.70x |
| users.json | ujson | 12.350 | 12.432 | 12.543 | 67.719 | 0.25x |
| users.json | json | 22.677 | 22.755 | 23.253 | 67.719 | 0.14x |
| flat.json | strata | 0.421 | 0.440 | 0.454 | 67.465 | 1.00x |
| flat.json | orjson | 0.482 | 0.507 | 0.523 | 67.465 | 0.87x |
| flat.json | msgspec | 0.584 | 0.604 | 0.617 | 67.465 | 0.73x |
| flat.json | ujson | 1.203 | 1.216 | 1.245 | 67.465 | 0.36x |
| flat.json | json | 2.013 | 2.042 | 2.084 | 67.465 | 0.22x |
| nested.json | strata | 0.376 | 0.380 | 0.414 | 67.465 | 1.00x |
| nested.json | orjson | 0.425 | 0.439 | 0.450 | 67.465 | 0.87x |
| nested.json | msgspec | 0.536 | 0.545 | 0.574 | 67.465 | 0.70x |
| nested.json | ujson | 1.233 | 1.261 | 1.290 | 67.465 | 0.30x |
| nested.json | json | 2.564 | 2.576 | 2.595 | 67.465 | 0.15x |
| wide_arrays.json | strata | 1.994 | 2.007 | 2.039 | 73.863 | 1.00x |
| wide_arrays.json | orjson | 2.224 | 2.259 | 2.275 | 73.863 | 0.89x |
| wide_arrays.json | msgspec | 3.111 | 3.142 | 3.194 | 73.863 | 0.64x |
| wide_arrays.json | ujson | 6.845 | 6.898 | 6.955 | 73.863 | 0.29x |
| wide_arrays.json | json | 17.087 | 17.120 | 17.201 | 73.863 | 0.12x |
| mixed.json | strata | 0.149 | 0.153 | 0.184 | 73.863 | 1.00x |
| mixed.json | orjson | 0.166 | 0.167 | 0.200 | 73.863 | 0.92x |
| mixed.json | msgspec | 0.182 | 0.185 | 0.202 | 73.863 | 0.83x |
| mixed.json | ujson | 0.339 | 0.349 | 0.371 | 73.863 | 0.44x |
| mixed.json | json | 0.629 | 0.645 | 0.654 | 73.863 | 0.24x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.062 | 0.064 | 0.072 | 67.723 | 1.00x |
| users.json $[*].id | jmespath | 0.495 | 0.507 | 0.516 | 67.723 | 0.13x |
| users.json $[*].id | jsonpath-ng | 2.845 | 2.924 | 3.121 | 67.723 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.423 | 0.447 | 0.466 | 67.871 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.080 | 3.104 | 3.332 | 67.871 | 0.14x |
| users.json $[*].orders[*].total | jsonpath-ng | 19.471 | 19.679 | 20.433 | 67.871 | 0.02x |
| users.json $..total | strata | 1.692 | 1.721 | 1.817 | 68.980 | 1.00x |
| users.json $..total | jsonpath-ng | 394.687 | 396.574 | 404.635 | 68.980 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.265 | 3.300 | 3.351 | 67.871 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.275 | 14.411 | 14.806 | 67.871 | 0.23x |
| users.json $[*].id | orjson+jsonpath-ng | 16.610 | 16.825 | 17.774 | 67.871 | 0.20x |
| users.json $[*].orders[*].total | strata | 3.517 | 3.530 | 3.578 | 68.980 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 16.812 | 17.014 | 17.347 | 68.980 | 0.21x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 36.082 | 36.850 | 37.192 | 68.980 | 0.10x |
| users.json $..total | strata | 12.697 | 13.226 | 14.913 | 70.082 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 414.119 | 417.589 | 424.809 | 70.082 | 0.03x |

