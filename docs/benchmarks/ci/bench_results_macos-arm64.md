# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 1cc7049
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
| users.json | strata | 7.066 | 7.310 | 8.290 | 63.766 | 1.00x |
| users.json | orjson | 10.590 | 10.962 | 25.535 | 63.766 | 0.67x |
| users.json | msgspec | 9.664 | 10.539 | 13.521 | 63.766 | 0.69x |
| users.json | ujson | 14.122 | 14.681 | 23.192 | 63.766 | 0.50x |
| users.json | pysimdjson | 132.824 | 156.556 | 278.520 | 63.766 | 0.05x |
| users.json | json | 16.462 | 18.126 | 29.929 | 63.766 | 0.40x |
| flat.json | strata | 0.579 | 0.681 | 1.944 | 98.969 | 1.00x |
| flat.json | orjson | 0.743 | 0.859 | 1.480 | 98.969 | 0.79x |
| flat.json | msgspec | 0.751 | 0.882 | 2.018 | 98.969 | 0.77x |
| flat.json | ujson | 1.301 | 1.517 | 2.124 | 98.969 | 0.45x |
| flat.json | pysimdjson | 12.474 | 15.797 | 19.092 | 98.969 | 0.04x |
| flat.json | json | 1.403 | 1.652 | 2.524 | 98.969 | 0.41x |
| nested.json | strata | 0.542 | 0.610 | 1.278 | 98.984 | 1.00x |
| nested.json | orjson | 0.721 | 0.810 | 1.013 | 98.984 | 0.75x |
| nested.json | msgspec | 0.674 | 0.737 | 0.811 | 98.984 | 0.83x |
| nested.json | ujson | 1.167 | 1.216 | 1.773 | 98.984 | 0.50x |
| nested.json | pysimdjson | 10.598 | 11.080 | 15.073 | 98.984 | 0.06x |
| nested.json | json | 1.419 | 1.580 | 3.700 | 98.984 | 0.39x |
| wide_arrays.json | strata | 3.090 | 3.298 | 5.007 | 100.531 | 1.00x |
| wide_arrays.json | orjson | 3.654 | 4.029 | 4.679 | 100.531 | 0.82x |
| wide_arrays.json | msgspec | 4.215 | 4.752 | 5.979 | 100.531 | 0.69x |
| wide_arrays.json | ujson | 5.463 | 5.813 | 8.355 | 100.531 | 0.57x |
| wide_arrays.json | pysimdjson | 65.088 | 71.549 | 83.332 | 100.531 | 0.05x |
| wide_arrays.json | json | 6.933 | 7.484 | 10.887 | 100.531 | 0.44x |
| mixed.json | strata | 0.141 | 0.158 | 0.193 | 100.562 | 1.00x |
| mixed.json | orjson | 0.187 | 0.210 | 0.351 | 100.562 | 0.75x |
| mixed.json | msgspec | 0.191 | 0.218 | 0.553 | 100.562 | 0.72x |
| mixed.json | ujson | 0.246 | 0.363 | 0.773 | 100.562 | 0.43x |
| mixed.json | pysimdjson | 2.673 | 2.862 | 3.451 | 100.562 | 0.06x |
| mixed.json | json | 0.379 | 0.406 | 0.501 | 100.562 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.841 | 2.027 | 4.278 | 80.969 | 1.00x |
| users.json | orjson | 2.434 | 2.752 | 4.630 | 80.969 | 0.74x |
| users.json | msgspec | 3.019 | 3.373 | 6.721 | 80.969 | 0.60x |
| users.json | ujson | 9.442 | 11.577 | 18.755 | 80.969 | 0.18x |
| users.json | json | 17.337 | 21.153 | 34.543 | 80.969 | 0.10x |
| flat.json | strata | 0.245 | 0.280 | 0.472 | 98.969 | 1.00x |
| flat.json | orjson | 0.287 | 0.557 | 0.888 | 98.969 | 0.50x |
| flat.json | msgspec | 0.344 | 0.404 | 0.959 | 98.969 | 0.69x |
| flat.json | ujson | 0.848 | 0.928 | 1.754 | 98.969 | 0.30x |
| flat.json | json | 1.450 | 1.823 | 3.927 | 98.969 | 0.15x |
| nested.json | strata | 0.140 | 0.165 | 0.315 | 98.984 | 1.00x |
| nested.json | orjson | 0.240 | 0.265 | 0.298 | 98.984 | 0.62x |
| nested.json | msgspec | 0.325 | 0.442 | 0.554 | 98.984 | 0.37x |
| nested.json | ujson | 0.843 | 1.061 | 1.505 | 98.984 | 0.16x |
| nested.json | json | 1.786 | 1.967 | 2.591 | 98.984 | 0.08x |
| wide_arrays.json | strata | 1.199 | 1.367 | 2.489 | 100.547 | 1.00x |
| wide_arrays.json | orjson | 1.413 | 1.644 | 2.651 | 100.547 | 0.83x |
| wide_arrays.json | msgspec | 2.388 | 2.511 | 4.647 | 100.547 | 0.54x |
| wide_arrays.json | ujson | 5.058 | 5.960 | 8.162 | 100.547 | 0.23x |
| wide_arrays.json | json | 12.479 | 13.996 | 22.153 | 100.547 | 0.10x |
| mixed.json | strata | 0.047 | 0.062 | 0.068 | 100.562 | 1.00x |
| mixed.json | orjson | 0.056 | 0.061 | 0.310 | 100.562 | 1.01x |
| mixed.json | msgspec | 0.067 | 0.078 | 0.086 | 100.562 | 0.79x |
| mixed.json | ujson | 0.191 | 0.204 | 0.330 | 100.562 | 0.30x |
| mixed.json | json | 0.400 | 0.448 | 0.538 | 100.562 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.083 | 7.565 | 12.640 | 91.156 | 1.00x |
| users.json | orjson | 10.515 | 11.677 | 24.282 | 91.156 | 0.65x |
| users.json | msgspec | 9.682 | 11.620 | 22.709 | 91.156 | 0.65x |
| users.json | ujson | 14.011 | 16.896 | 29.236 | 91.156 | 0.45x |
| users.json | json | 16.903 | 21.931 | 37.208 | 91.156 | 0.34x |
| flat.json | strata | 0.618 | 0.786 | 1.494 | 98.969 | 1.00x |
| flat.json | orjson | 0.935 | 1.161 | 1.842 | 98.969 | 0.68x |
| flat.json | msgspec | 0.760 | 0.913 | 1.029 | 98.969 | 0.86x |
| flat.json | ujson | 1.140 | 1.309 | 2.633 | 98.969 | 0.60x |
| flat.json | json | 1.405 | 1.599 | 3.822 | 98.969 | 0.49x |
| nested.json | strata | 0.624 | 0.696 | 0.947 | 98.984 | 1.00x |
| nested.json | orjson | 1.028 | 1.086 | 1.192 | 98.984 | 0.64x |
| nested.json | msgspec | 0.808 | 0.909 | 1.248 | 98.984 | 0.77x |
| nested.json | ujson | 1.141 | 1.262 | 1.717 | 98.984 | 0.55x |
| nested.json | json | 1.591 | 1.676 | 2.049 | 98.984 | 0.42x |
| wide_arrays.json | strata | 3.414 | 3.709 | 3.955 | 100.547 | 1.00x |
| wide_arrays.json | orjson | 4.297 | 4.827 | 5.533 | 100.547 | 0.77x |
| wide_arrays.json | msgspec | 4.790 | 5.384 | 7.946 | 100.547 | 0.69x |
| wide_arrays.json | ujson | 6.335 | 7.215 | 16.458 | 100.547 | 0.51x |
| wide_arrays.json | json | 7.984 | 8.379 | 11.211 | 100.547 | 0.44x |
| mixed.json | strata | 0.221 | 0.266 | 0.460 | 100.562 | 1.00x |
| mixed.json | orjson | 0.256 | 0.561 | 1.202 | 100.562 | 0.47x |
| mixed.json | msgspec | 0.262 | 0.353 | 0.671 | 100.562 | 0.75x |
| mixed.json | ujson | 0.306 | 0.436 | 0.756 | 100.562 | 0.61x |
| mixed.json | json | 0.461 | 0.521 | 1.102 | 100.562 | 0.51x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.397 | 7.987 | 9.603 | 98.969 | 1.00x |
| users.ndjson | orjson | 12.475 | 13.099 | 18.126 | 98.969 | 0.61x |
| users.ndjson | msgspec | 12.514 | 13.702 | 20.751 | 98.969 | 0.58x |
| users.ndjson | ujson | 15.378 | 16.575 | 29.058 | 98.969 | 0.48x |
| users.ndjson | json | 20.143 | 21.918 | 33.051 | 98.969 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.970 | 2.143 | 6.344 | 93.250 | 1.00x |
| users.json | orjson | 2.762 | 3.123 | 8.155 | 93.250 | 0.69x |
| users.json | msgspec | 3.319 | 3.818 | 8.565 | 93.250 | 0.56x |
| users.json | ujson | 9.349 | 9.925 | 15.515 | 93.250 | 0.22x |
| users.json | json | 15.837 | 17.045 | 20.596 | 93.250 | 0.13x |
| flat.json | strata | 0.397 | 0.554 | 1.124 | 98.969 | 1.00x |
| flat.json | orjson | 0.420 | 0.632 | 0.990 | 98.969 | 0.88x |
| flat.json | msgspec | 0.499 | 0.657 | 0.755 | 98.969 | 0.84x |
| flat.json | ujson | 0.936 | 1.069 | 1.599 | 98.969 | 0.52x |
| flat.json | json | 1.575 | 1.841 | 2.802 | 98.969 | 0.30x |
| nested.json | strata | 0.273 | 0.338 | 0.519 | 98.984 | 1.00x |
| nested.json | orjson | 0.369 | 0.389 | 0.482 | 98.984 | 0.87x |
| nested.json | msgspec | 0.448 | 0.633 | 0.834 | 98.984 | 0.53x |
| nested.json | ujson | 0.942 | 1.087 | 1.332 | 98.984 | 0.31x |
| nested.json | json | 1.737 | 1.825 | 2.188 | 98.984 | 0.19x |
| wide_arrays.json | strata | 1.535 | 1.752 | 1.921 | 100.547 | 1.00x |
| wide_arrays.json | orjson | 1.739 | 2.167 | 2.531 | 100.547 | 0.81x |
| wide_arrays.json | msgspec | 2.756 | 3.097 | 4.807 | 100.547 | 0.57x |
| wide_arrays.json | ujson | 5.749 | 6.690 | 10.086 | 100.547 | 0.26x |
| wide_arrays.json | json | 12.093 | 13.843 | 20.030 | 100.547 | 0.13x |
| mixed.json | strata | 0.241 | 0.305 | 0.360 | 100.562 | 1.00x |
| mixed.json | orjson | 0.328 | 0.385 | 0.476 | 100.562 | 0.79x |
| mixed.json | msgspec | 0.293 | 0.511 | 1.017 | 100.562 | 0.60x |
| mixed.json | ujson | 0.436 | 0.557 | 1.040 | 100.562 | 0.55x |
| mixed.json | json | 0.579 | 0.741 | 1.672 | 100.562 | 0.41x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.062 | 0.080 | 0.124 | 93.312 | 1.00x |
| users.json $[*].id | jmespath | 0.306 | 0.338 | 0.442 | 93.312 | 0.24x |
| users.json $[*].id | jsonpath-ng | 1.599 | 1.687 | 5.544 | 93.312 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.698 | 0.796 | 1.066 | 93.516 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.018 | 2.539 | 6.459 | 93.516 | 0.31x |
| users.json $[*].orders[*].total | jsonpath-ng | 13.886 | 16.677 | 20.837 | 93.516 | 0.05x |
| users.json $..total | strata | 1.473 | 1.642 | 2.538 | 95.531 | 1.00x |
| users.json $..total | jsonpath-ng | 209.208 | 246.109 | 305.002 | 95.531 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.804 | 3.975 | 5.012 | 93.375 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.448 | 11.726 | 17.142 | 93.375 | 0.34x |
| users.json $[*].id | orjson+jsonpath-ng | 12.248 | 13.016 | 19.591 | 93.375 | 0.31x |
| users.json $[*].orders[*].total | strata | 3.877 | 4.743 | 7.830 | 95.531 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.859 | 16.771 | 27.404 | 95.531 | 0.28x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 27.401 | 35.923 | 54.327 | 95.531 | 0.13x |
| users.json $..total | strata | 8.975 | 10.067 | 23.403 | 95.344 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 216.009 | 274.126 | 357.669 | 95.344 | 0.04x |

