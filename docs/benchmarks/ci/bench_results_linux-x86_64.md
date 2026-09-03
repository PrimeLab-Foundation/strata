# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: e44489e
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
| users.json | strata | 10.067 | 10.219 | 14.766 | 58.430 | 1.00x |
| users.json | orjson | 13.846 | 14.042 | 25.943 | 58.430 | 0.73x |
| users.json | msgspec | 14.515 | 14.756 | 26.985 | 58.430 | 0.69x |
| users.json | ujson | 17.948 | 18.288 | 39.711 | 58.430 | 0.56x |
| users.json | pysimdjson | 19.133 | 19.481 | 32.457 | 58.430 | 0.52x |
| users.json | json | 21.256 | 21.492 | 22.711 | 58.430 | 0.48x |
| flat.json | strata | 0.886 | 0.898 | 0.929 | 67.379 | 1.00x |
| flat.json | orjson | 1.053 | 1.061 | 1.070 | 67.379 | 0.85x |
| flat.json | msgspec | 1.030 | 1.049 | 1.062 | 67.379 | 0.86x |
| flat.json | ujson | 1.503 | 1.520 | 1.569 | 67.379 | 0.59x |
| flat.json | pysimdjson | 1.644 | 1.671 | 1.773 | 67.379 | 0.54x |
| flat.json | json | 1.707 | 1.743 | 1.768 | 67.379 | 0.52x |
| nested.json | strata | 0.816 | 0.821 | 0.838 | 67.379 | 1.00x |
| nested.json | orjson | 1.014 | 1.036 | 1.043 | 67.379 | 0.79x |
| nested.json | msgspec | 1.000 | 1.021 | 1.071 | 67.379 | 0.80x |
| nested.json | ujson | 1.416 | 1.447 | 1.468 | 67.379 | 0.57x |
| nested.json | pysimdjson | 1.406 | 1.427 | 1.457 | 67.379 | 0.58x |
| nested.json | json | 1.845 | 1.865 | 1.898 | 67.379 | 0.44x |
| wide_arrays.json | strata | 4.948 | 5.004 | 6.462 | 73.262 | 1.00x |
| wide_arrays.json | orjson | 5.455 | 5.561 | 5.762 | 73.262 | 0.90x |
| wide_arrays.json | msgspec | 6.066 | 6.119 | 6.186 | 73.262 | 0.82x |
| wide_arrays.json | ujson | 7.660 | 7.767 | 8.336 | 73.262 | 0.64x |
| wide_arrays.json | pysimdjson | 6.348 | 6.423 | 6.523 | 73.262 | 0.78x |
| wide_arrays.json | json | 9.612 | 9.692 | 10.035 | 73.262 | 0.52x |
| mixed.json | strata | 0.191 | 0.196 | 0.213 | 73.324 | 1.00x |
| mixed.json | orjson | 0.233 | 0.237 | 0.249 | 73.324 | 0.83x |
| mixed.json | msgspec | 0.242 | 0.245 | 0.258 | 73.324 | 0.80x |
| mixed.json | ujson | 0.298 | 0.304 | 0.327 | 73.324 | 0.65x |
| mixed.json | pysimdjson | 0.296 | 0.302 | 0.312 | 73.324 | 0.65x |
| mixed.json | json | 0.443 | 0.454 | 0.470 | 73.324 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.484 | 2.523 | 2.641 | 57.520 | 1.00x |
| users.json | orjson | 3.058 | 3.093 | 3.340 | 57.520 | 0.82x |
| users.json | msgspec | 4.135 | 4.168 | 4.271 | 57.520 | 0.61x |
| users.json | ujson | 14.594 | 14.676 | 14.827 | 57.520 | 0.17x |
| users.json | json | 21.428 | 21.706 | 22.171 | 57.520 | 0.12x |
| flat.json | strata | 0.282 | 0.290 | 0.369 | 67.379 | 1.00x |
| flat.json | orjson | 0.360 | 0.369 | 0.523 | 67.379 | 0.79x |
| flat.json | msgspec | 0.472 | 0.484 | 0.661 | 67.379 | 0.60x |
| flat.json | ujson | 1.265 | 1.280 | 1.312 | 67.379 | 0.23x |
| flat.json | json | 1.841 | 1.856 | 2.046 | 67.379 | 0.16x |
| nested.json | strata | 0.236 | 0.241 | 0.252 | 67.379 | 1.00x |
| nested.json | orjson | 0.299 | 0.311 | 0.317 | 67.379 | 0.77x |
| nested.json | msgspec | 0.416 | 0.419 | 0.432 | 67.379 | 0.57x |
| nested.json | ujson | 1.327 | 1.350 | 1.383 | 67.379 | 0.18x |
| nested.json | json | 2.310 | 2.350 | 2.501 | 67.379 | 0.10x |
| wide_arrays.json | strata | 1.696 | 1.707 | 1.729 | 73.262 | 1.00x |
| wide_arrays.json | orjson | 1.901 | 1.912 | 2.149 | 73.262 | 0.89x |
| wide_arrays.json | msgspec | 2.967 | 2.982 | 3.413 | 73.262 | 0.57x |
| wide_arrays.json | ujson | 8.538 | 8.574 | 8.791 | 73.262 | 0.20x |
| wide_arrays.json | json | 16.523 | 16.583 | 17.075 | 73.262 | 0.10x |
| mixed.json | strata | 0.065 | 0.068 | 0.073 | 73.324 | 1.00x |
| mixed.json | orjson | 0.069 | 0.071 | 0.082 | 73.324 | 0.97x |
| mixed.json | msgspec | 0.085 | 0.088 | 0.100 | 73.324 | 0.78x |
| mixed.json | ujson | 0.299 | 0.305 | 0.332 | 73.324 | 0.22x |
| mixed.json | json | 0.508 | 0.520 | 0.528 | 73.324 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.654 | 10.839 | 12.737 | 71.789 | 1.00x |
| users.json | orjson | 14.360 | 14.664 | 15.662 | 71.789 | 0.74x |
| users.json | msgspec | 14.997 | 15.254 | 15.762 | 71.789 | 0.71x |
| users.json | ujson | 18.902 | 19.869 | 22.155 | 71.789 | 0.55x |
| users.json | json | 21.627 | 21.955 | 22.440 | 71.789 | 0.49x |
| flat.json | strata | 0.906 | 0.939 | 0.973 | 67.379 | 1.00x |
| flat.json | orjson | 1.111 | 1.124 | 1.157 | 67.379 | 0.84x |
| flat.json | msgspec | 1.080 | 1.108 | 1.129 | 67.379 | 0.85x |
| flat.json | ujson | 1.569 | 1.610 | 1.642 | 67.379 | 0.58x |
| flat.json | json | 1.774 | 1.805 | 1.853 | 67.379 | 0.52x |
| nested.json | strata | 0.846 | 0.859 | 0.871 | 67.379 | 1.00x |
| nested.json | orjson | 1.071 | 1.084 | 1.102 | 67.379 | 0.79x |
| nested.json | msgspec | 1.044 | 1.058 | 1.081 | 67.379 | 0.81x |
| nested.json | ujson | 1.486 | 1.504 | 1.518 | 67.379 | 0.57x |
| nested.json | json | 1.887 | 1.902 | 1.988 | 67.379 | 0.45x |
| wide_arrays.json | strata | 5.001 | 5.128 | 5.221 | 73.324 | 1.00x |
| wide_arrays.json | orjson | 5.580 | 5.728 | 5.811 | 73.324 | 0.90x |
| wide_arrays.json | msgspec | 6.263 | 6.333 | 6.453 | 73.324 | 0.81x |
| wide_arrays.json | ujson | 8.018 | 8.099 | 8.380 | 73.324 | 0.63x |
| wide_arrays.json | json | 9.735 | 9.938 | 10.151 | 73.324 | 0.52x |
| mixed.json | strata | 0.215 | 0.224 | 0.238 | 73.324 | 1.00x |
| mixed.json | orjson | 0.282 | 0.289 | 0.307 | 73.324 | 0.78x |
| mixed.json | msgspec | 0.288 | 0.299 | 0.319 | 73.324 | 0.75x |
| mixed.json | ujson | 0.358 | 0.366 | 0.385 | 73.324 | 0.61x |
| mixed.json | json | 0.488 | 0.505 | 0.517 | 73.324 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.735 | 10.841 | 11.291 | 67.379 | 1.00x |
| users.ndjson | orjson | 17.707 | 18.054 | 18.379 | 67.379 | 0.60x |
| users.ndjson | msgspec | 17.524 | 18.175 | 22.200 | 67.379 | 0.60x |
| users.ndjson | ujson | 22.047 | 22.605 | 22.934 | 67.379 | 0.48x |
| users.ndjson | json | 28.689 | 29.066 | 29.919 | 67.379 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.075 | 3.110 | 3.160 | 69.820 | 1.00x |
| users.json | orjson | 3.685 | 3.742 | 3.862 | 69.820 | 0.83x |
| users.json | msgspec | 4.744 | 4.784 | 4.860 | 69.820 | 0.65x |
| users.json | ujson | 15.196 | 15.328 | 15.537 | 69.820 | 0.20x |
| users.json | json | 22.310 | 22.434 | 22.740 | 69.820 | 0.14x |
| flat.json | strata | 0.444 | 0.452 | 0.532 | 67.379 | 1.00x |
| flat.json | orjson | 0.526 | 0.537 | 0.550 | 67.379 | 0.84x |
| flat.json | msgspec | 0.642 | 0.652 | 0.675 | 67.379 | 0.69x |
| flat.json | ujson | 1.436 | 1.467 | 1.478 | 67.379 | 0.31x |
| flat.json | json | 2.057 | 2.066 | 2.118 | 67.379 | 0.22x |
| nested.json | strata | 0.358 | 0.367 | 0.390 | 67.379 | 1.00x |
| nested.json | orjson | 0.447 | 0.456 | 0.505 | 67.379 | 0.81x |
| nested.json | msgspec | 0.562 | 0.574 | 0.585 | 67.379 | 0.64x |
| nested.json | ujson | 1.493 | 1.511 | 1.549 | 67.379 | 0.24x |
| nested.json | json | 2.479 | 2.503 | 2.551 | 67.379 | 0.15x |
| wide_arrays.json | strata | 2.118 | 2.128 | 2.160 | 73.324 | 1.00x |
| wide_arrays.json | orjson | 2.333 | 2.360 | 2.419 | 73.324 | 0.90x |
| wide_arrays.json | msgspec | 3.380 | 3.421 | 3.554 | 73.324 | 0.62x |
| wide_arrays.json | ujson | 9.078 | 9.133 | 9.279 | 73.324 | 0.23x |
| wide_arrays.json | json | 17.037 | 17.086 | 17.533 | 73.324 | 0.12x |
| mixed.json | strata | 0.158 | 0.166 | 0.172 | 73.324 | 1.00x |
| mixed.json | orjson | 0.177 | 0.181 | 0.203 | 73.324 | 0.91x |
| mixed.json | msgspec | 0.192 | 0.199 | 0.243 | 73.324 | 0.83x |
| mixed.json | ujson | 0.420 | 0.427 | 0.451 | 73.324 | 0.39x |
| mixed.json | json | 0.639 | 0.649 | 0.666 | 73.324 | 0.26x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.071 | 0.074 | 0.082 | 69.820 | 1.00x |
| users.json $[*].id | jmespath | 0.482 | 0.487 | 0.502 | 69.820 | 0.15x |
| users.json $[*].id | jsonpath-ng | 2.756 | 2.820 | 2.986 | 69.820 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.408 | 0.429 | 0.457 | 69.844 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.079 | 3.118 | 3.180 | 69.844 | 0.14x |
| users.json $[*].orders[*].total | jsonpath-ng | 19.443 | 19.602 | 19.883 | 69.844 | 0.02x |
| users.json $..total | strata | 1.718 | 1.803 | 1.904 | 70.852 | 1.00x |
| users.json $..total | jsonpath-ng | 383.588 | 387.041 | 388.615 | 70.852 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.225 | 3.247 | 3.365 | 69.844 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.829 | 15.071 | 15.667 | 69.844 | 0.22x |
| users.json $[*].id | orjson+jsonpath-ng | 17.230 | 17.444 | 18.049 | 69.844 | 0.19x |
| users.json $[*].orders[*].total | strata | 3.469 | 3.491 | 3.567 | 70.852 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.755 | 18.246 | 18.977 | 70.852 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 38.134 | 38.618 | 39.426 | 70.852 | 0.09x |
| users.json $..total | strata | 13.172 | 13.460 | 13.984 | 70.977 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 408.459 | 410.708 | 411.839 | 70.977 | 0.03x |

