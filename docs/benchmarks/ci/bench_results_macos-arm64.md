# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: a4f3b0e
- python: 3.12.10
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit
- machine: arm64
- processor: Apple M1 (Virtual)
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.811 | 7.302 | 11.944 | 63.953 | 1.00x |
| users.json | orjson | 9.283 | 10.065 | 14.024 | 63.953 | 0.73x |
| users.json | msgspec | 9.032 | 10.264 | 13.353 | 63.953 | 0.71x |
| users.json | ujson | 11.840 | 14.052 | 20.739 | 63.953 | 0.52x |
| users.json | pysimdjson | 125.967 | 137.894 | 234.226 | 63.953 | 0.05x |
| users.json | json | 14.993 | 16.024 | 19.784 | 63.953 | 0.46x |
| flat.json | strata | 0.625 | 0.676 | 1.630 | 87.172 | 1.00x |
| flat.json | orjson | 0.826 | 0.881 | 1.995 | 87.172 | 0.77x |
| flat.json | msgspec | 0.780 | 0.845 | 0.937 | 87.172 | 0.80x |
| flat.json | ujson | 1.274 | 1.332 | 1.550 | 87.172 | 0.51x |
| flat.json | pysimdjson | 12.846 | 13.227 | 20.819 | 87.172 | 0.05x |
| flat.json | json | 1.489 | 1.557 | 4.141 | 87.172 | 0.43x |
| nested.json | strata | 0.582 | 0.610 | 0.690 | 87.188 | 1.00x |
| nested.json | orjson | 0.798 | 0.888 | 1.001 | 87.188 | 0.69x |
| nested.json | msgspec | 0.750 | 0.832 | 1.128 | 87.188 | 0.73x |
| nested.json | ujson | 1.187 | 1.237 | 1.426 | 87.188 | 0.49x |
| nested.json | pysimdjson | 11.105 | 11.603 | 12.745 | 87.188 | 0.05x |
| nested.json | json | 1.580 | 1.679 | 1.839 | 87.188 | 0.36x |
| wide_arrays.json | strata | 3.307 | 3.937 | 4.199 | 89.953 | 1.00x |
| wide_arrays.json | orjson | 3.340 | 3.938 | 4.515 | 89.953 | 1.00x |
| wide_arrays.json | msgspec | 3.790 | 4.542 | 5.188 | 89.953 | 0.87x |
| wide_arrays.json | ujson | 5.017 | 5.860 | 6.818 | 89.953 | 0.67x |
| wide_arrays.json | pysimdjson | 60.059 | 67.917 | 71.821 | 89.953 | 0.06x |
| wide_arrays.json | json | 6.379 | 7.824 | 8.297 | 89.953 | 0.50x |
| mixed.json | strata | 0.122 | 0.128 | 0.152 | 90.609 | 1.00x |
| mixed.json | orjson | 0.143 | 0.152 | 0.184 | 90.609 | 0.84x |
| mixed.json | msgspec | 0.157 | 0.170 | 0.198 | 90.609 | 0.76x |
| mixed.json | ujson | 0.196 | 0.229 | 0.537 | 90.609 | 0.56x |
| mixed.json | pysimdjson | 2.361 | 2.396 | 2.739 | 90.609 | 0.05x |
| mixed.json | json | 0.303 | 0.307 | 0.346 | 90.609 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.709 | 1.821 | 2.095 | 73.188 | 1.00x |
| users.json | orjson | 2.233 | 2.614 | 4.127 | 73.188 | 0.70x |
| users.json | msgspec | 2.924 | 3.207 | 3.507 | 73.188 | 0.57x |
| users.json | ujson | 11.026 | 12.098 | 17.789 | 73.188 | 0.15x |
| users.json | json | 15.594 | 17.244 | 19.722 | 73.188 | 0.11x |
| flat.json | strata | 0.199 | 0.227 | 0.309 | 87.172 | 1.00x |
| flat.json | orjson | 0.252 | 0.280 | 0.313 | 87.172 | 0.81x |
| flat.json | msgspec | 0.306 | 0.334 | 0.477 | 87.172 | 0.68x |
| flat.json | ujson | 0.891 | 0.951 | 1.085 | 87.172 | 0.24x |
| flat.json | json | 1.383 | 1.519 | 1.666 | 87.172 | 0.15x |
| nested.json | strata | 0.150 | 0.160 | 0.173 | 87.188 | 1.00x |
| nested.json | orjson | 0.255 | 0.275 | 0.554 | 87.188 | 0.58x |
| nested.json | msgspec | 0.350 | 0.480 | 0.511 | 87.188 | 0.33x |
| nested.json | ujson | 1.122 | 1.202 | 1.253 | 87.188 | 0.13x |
| nested.json | json | 1.750 | 1.862 | 2.365 | 87.188 | 0.09x |
| wide_arrays.json | strata | 1.071 | 1.206 | 1.379 | 89.953 | 1.00x |
| wide_arrays.json | orjson | 1.336 | 1.525 | 1.850 | 89.953 | 0.79x |
| wide_arrays.json | msgspec | 1.960 | 2.272 | 2.519 | 89.953 | 0.53x |
| wide_arrays.json | ujson | 6.112 | 6.589 | 7.507 | 89.953 | 0.18x |
| wide_arrays.json | json | 10.901 | 11.385 | 13.467 | 89.953 | 0.11x |
| mixed.json | strata | 0.036 | 0.037 | 0.052 | 90.609 | 1.00x |
| mixed.json | orjson | 0.040 | 0.041 | 0.041 | 90.609 | 0.91x |
| mixed.json | msgspec | 0.046 | 0.047 | 0.049 | 90.609 | 0.78x |
| mixed.json | ujson | 0.203 | 0.208 | 0.219 | 90.609 | 0.18x |
| mixed.json | json | 0.322 | 0.331 | 0.342 | 90.609 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.253 | 7.970 | 8.830 | 84.531 | 1.00x |
| users.json | orjson | 9.655 | 10.981 | 11.764 | 84.531 | 0.73x |
| users.json | msgspec | 9.994 | 10.723 | 16.295 | 84.531 | 0.74x |
| users.json | ujson | 13.469 | 14.512 | 18.353 | 84.531 | 0.55x |
| users.json | json | 15.368 | 17.575 | 20.726 | 84.531 | 0.45x |
| flat.json | strata | 0.634 | 0.766 | 0.834 | 87.172 | 1.00x |
| flat.json | orjson | 0.787 | 1.134 | 1.233 | 87.172 | 0.68x |
| flat.json | msgspec | 1.013 | 1.086 | 1.283 | 87.172 | 0.71x |
| flat.json | ujson | 1.301 | 1.398 | 1.661 | 87.172 | 0.55x |
| flat.json | json | 1.624 | 1.719 | 1.915 | 87.172 | 0.45x |
| nested.json | strata | 0.552 | 0.608 | 0.767 | 87.188 | 1.00x |
| nested.json | orjson | 0.828 | 0.917 | 1.221 | 87.188 | 0.66x |
| nested.json | msgspec | 0.724 | 0.800 | 0.985 | 87.188 | 0.76x |
| nested.json | ujson | 1.017 | 1.059 | 1.364 | 87.188 | 0.57x |
| nested.json | json | 1.449 | 1.481 | 1.786 | 87.188 | 0.41x |
| wide_arrays.json | strata | 3.431 | 3.616 | 4.324 | 89.953 | 1.00x |
| wide_arrays.json | orjson | 3.536 | 3.794 | 4.382 | 89.953 | 0.95x |
| wide_arrays.json | msgspec | 4.204 | 4.463 | 4.737 | 89.953 | 0.81x |
| wide_arrays.json | ujson | 5.464 | 5.802 | 6.617 | 89.953 | 0.62x |
| wide_arrays.json | json | 6.755 | 7.286 | 8.005 | 89.953 | 0.50x |
| mixed.json | strata | 0.139 | 0.142 | 0.180 | 90.609 | 1.00x |
| mixed.json | orjson | 0.175 | 0.219 | 0.300 | 90.609 | 0.65x |
| mixed.json | msgspec | 0.187 | 0.190 | 0.199 | 90.609 | 0.75x |
| mixed.json | ujson | 0.232 | 0.234 | 0.253 | 90.609 | 0.61x |
| mixed.json | json | 0.327 | 0.333 | 0.343 | 90.609 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 8.296 | 8.621 | 10.820 | 87.172 | 1.00x |
| users.ndjson | orjson | 13.181 | 13.817 | 16.521 | 87.172 | 0.62x |
| users.ndjson | msgspec | 12.995 | 13.511 | 17.260 | 87.172 | 0.64x |
| users.ndjson | ujson | 16.482 | 16.780 | 20.016 | 87.172 | 0.51x |
| users.ndjson | json | 21.305 | 21.833 | 30.281 | 87.172 | 0.39x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.224 | 2.354 | 3.654 | 82.484 | 1.00x |
| users.json | orjson | 2.900 | 3.020 | 3.305 | 82.484 | 0.78x |
| users.json | msgspec | 3.605 | 3.716 | 5.672 | 82.484 | 0.63x |
| users.json | ujson | 12.310 | 12.760 | 15.721 | 82.484 | 0.18x |
| users.json | json | 17.187 | 17.841 | 20.936 | 82.484 | 0.13x |
| flat.json | strata | 0.482 | 0.556 | 0.620 | 87.172 | 1.00x |
| flat.json | orjson | 0.537 | 0.607 | 0.685 | 87.172 | 0.92x |
| flat.json | msgspec | 0.600 | 0.657 | 0.747 | 87.172 | 0.85x |
| flat.json | ujson | 1.315 | 1.427 | 1.607 | 87.172 | 0.39x |
| flat.json | json | 1.940 | 2.136 | 2.322 | 87.172 | 0.26x |
| nested.json | strata | 0.407 | 0.471 | 0.525 | 87.188 | 1.00x |
| nested.json | orjson | 0.520 | 0.580 | 0.730 | 87.188 | 0.81x |
| nested.json | msgspec | 0.587 | 0.791 | 0.888 | 87.188 | 0.60x |
| nested.json | ujson | 1.513 | 1.654 | 1.778 | 87.188 | 0.29x |
| nested.json | json | 2.051 | 2.247 | 2.404 | 87.188 | 0.21x |
| wide_arrays.json | strata | 1.330 | 1.593 | 1.753 | 90.594 | 1.00x |
| wide_arrays.json | orjson | 1.665 | 1.863 | 2.211 | 90.594 | 0.85x |
| wide_arrays.json | msgspec | 2.325 | 2.658 | 3.036 | 90.594 | 0.60x |
| wide_arrays.json | ujson | 6.582 | 7.080 | 8.466 | 90.594 | 0.22x |
| wide_arrays.json | json | 11.445 | 12.022 | 15.001 | 90.594 | 0.13x |
| mixed.json | strata | 0.111 | 0.125 | 0.373 | 90.609 | 1.00x |
| mixed.json | orjson | 0.123 | 0.134 | 0.262 | 90.609 | 0.94x |
| mixed.json | msgspec | 0.123 | 0.146 | 0.167 | 90.609 | 0.86x |
| mixed.json | ujson | 0.290 | 0.300 | 1.002 | 90.609 | 0.42x |
| mixed.json | json | 0.402 | 0.420 | 1.186 | 90.609 | 0.30x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.066 | 0.084 | 0.219 | 82.531 | 1.00x |
| users.json $[*].id | jmespath | 0.321 | 0.341 | 0.444 | 82.531 | 0.25x |
| users.json $[*].id | jsonpath-ng | 1.585 | 1.679 | 4.780 | 82.531 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.433 | 0.678 | 2.437 | 82.703 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.174 | 2.773 | 6.518 | 82.703 | 0.24x |
| users.json $[*].orders[*].total | jsonpath-ng | 12.263 | 15.851 | 18.391 | 82.703 | 0.04x |
| users.json $..total | strata | 1.369 | 1.562 | 1.688 | 84.531 | 1.00x |
| users.json $..total | jsonpath-ng | 208.382 | 256.698 | 344.862 | 84.531 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.865 | 4.277 | 5.121 | 82.578 | 1.00x |
| users.json $[*].id | orjson+jmespath | 11.161 | 13.170 | 20.359 | 82.578 | 0.32x |
| users.json $[*].id | orjson+jsonpath-ng | 12.603 | 14.565 | 21.814 | 82.578 | 0.29x |
| users.json $[*].orders[*].total | strata | 3.783 | 3.990 | 5.826 | 84.516 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 11.308 | 13.309 | 20.383 | 84.516 | 0.30x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 24.022 | 28.884 | 48.244 | 84.516 | 0.14x |
| users.json $..total | strata | 8.642 | 10.227 | 12.610 | 84.531 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 214.871 | 223.588 | 230.386 | 84.531 | 0.05x |

