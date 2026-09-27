# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 943460734d2b79bd16c949d8d97f4d22cc202e84
- python: 3.12.10
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit
- machine: arm64
- processor: Apple M1 (Virtual)
- compiler_flags: -fno-strict-overflow -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -O3 -Wall -arch arm64 -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -march=native -flto=thin -fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata -bundle -undefined (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 5.789 | 7.070 | 12.150 | 68.141 | 1.00x |
| users.json | orjson | 10.912 | 11.842 | 16.216 | 68.141 | 0.60x |
| users.json | msgspec | 10.057 | 11.066 | 12.412 | 68.141 | 0.64x |
| users.json | ujson | 13.262 | 14.788 | 18.724 | 68.141 | 0.48x |
| users.json | pysimdjson | 125.734 | 137.482 | 150.965 | 68.141 | 0.05x |
| users.json | json | 14.124 | 17.458 | 18.961 | 68.141 | 0.40x |
| flat.json | strata | 0.605 | 0.637 | 0.712 | 99.234 | 1.00x |
| flat.json | orjson | 0.782 | 0.821 | 0.935 | 99.234 | 0.78x |
| flat.json | msgspec | 0.745 | 0.772 | 0.874 | 99.234 | 0.83x |
| flat.json | ujson | 1.268 | 1.338 | 1.477 | 99.234 | 0.48x |
| flat.json | pysimdjson | 12.229 | 12.825 | 13.778 | 99.234 | 0.05x |
| flat.json | json | 1.371 | 1.470 | 1.623 | 99.234 | 0.43x |
| nested.json | strata | 0.535 | 0.569 | 0.620 | 99.234 | 1.00x |
| nested.json | orjson | 0.765 | 0.824 | 0.889 | 99.234 | 0.69x |
| nested.json | msgspec | 0.666 | 0.746 | 0.843 | 99.234 | 0.76x |
| nested.json | ujson | 1.074 | 1.243 | 1.638 | 99.234 | 0.46x |
| nested.json | pysimdjson | 10.288 | 10.869 | 16.813 | 99.234 | 0.05x |
| nested.json | json | 1.457 | 1.495 | 1.857 | 99.234 | 0.38x |
| wide_arrays.json | strata | 2.988 | 3.333 | 3.696 | 101.625 | 1.00x |
| wide_arrays.json | orjson | 3.975 | 4.154 | 5.237 | 101.625 | 0.80x |
| wide_arrays.json | msgspec | 4.099 | 4.424 | 6.169 | 101.625 | 0.75x |
| wide_arrays.json | ujson | 5.290 | 5.801 | 6.648 | 101.625 | 0.57x |
| wide_arrays.json | pysimdjson | 64.624 | 66.348 | 75.363 | 101.625 | 0.05x |
| wide_arrays.json | json | 6.839 | 7.350 | 7.634 | 101.625 | 0.45x |
| mixed.json | strata | 0.123 | 0.136 | 0.178 | 101.641 | 1.00x |
| mixed.json | orjson | 0.155 | 0.174 | 0.212 | 101.641 | 0.78x |
| mixed.json | msgspec | 0.165 | 0.188 | 0.225 | 101.641 | 0.72x |
| mixed.json | ujson | 0.343 | 0.389 | 0.443 | 101.641 | 0.35x |
| mixed.json | pysimdjson | 2.485 | 2.638 | 3.052 | 101.641 | 0.05x |
| mixed.json | json | 0.330 | 0.386 | 0.462 | 101.641 | 0.35x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.510 | 1.727 | 4.638 | 83.312 | 1.00x |
| users.json | orjson | 2.480 | 2.661 | 2.919 | 83.312 | 0.65x |
| users.json | msgspec | 2.932 | 3.241 | 3.531 | 83.312 | 0.53x |
| users.json | ujson | 8.512 | 9.453 | 11.868 | 83.312 | 0.18x |
| users.json | json | 16.073 | 16.458 | 18.612 | 83.312 | 0.10x |
| flat.json | strata | 0.243 | 0.268 | 0.346 | 99.234 | 1.00x |
| flat.json | orjson | 0.274 | 0.315 | 0.633 | 99.234 | 0.85x |
| flat.json | msgspec | 0.351 | 0.381 | 0.404 | 99.234 | 0.70x |
| flat.json | ujson | 0.795 | 0.858 | 0.918 | 99.234 | 0.31x |
| flat.json | json | 1.449 | 1.617 | 2.082 | 99.234 | 0.17x |
| nested.json | strata | 0.134 | 0.140 | 0.153 | 99.234 | 1.00x |
| nested.json | orjson | 0.232 | 0.242 | 0.265 | 99.234 | 0.58x |
| nested.json | msgspec | 0.431 | 0.462 | 0.520 | 99.234 | 0.30x |
| nested.json | ujson | 0.939 | 0.979 | 1.031 | 99.234 | 0.14x |
| nested.json | json | 1.692 | 1.728 | 1.910 | 99.234 | 0.08x |
| wide_arrays.json | strata | 1.024 | 1.108 | 2.577 | 101.625 | 1.00x |
| wide_arrays.json | orjson | 1.238 | 1.604 | 2.230 | 101.625 | 0.69x |
| wide_arrays.json | msgspec | 2.112 | 2.390 | 2.953 | 101.625 | 0.46x |
| wide_arrays.json | ujson | 4.560 | 5.146 | 6.423 | 101.625 | 0.22x |
| wide_arrays.json | json | 11.071 | 11.837 | 15.427 | 101.625 | 0.09x |
| mixed.json | strata | 0.041 | 0.055 | 0.102 | 101.641 | 1.00x |
| mixed.json | orjson | 0.048 | 0.150 | 0.311 | 101.641 | 0.37x |
| mixed.json | msgspec | 0.064 | 0.076 | 0.127 | 101.641 | 0.72x |
| mixed.json | ujson | 0.196 | 0.210 | 0.234 | 101.641 | 0.26x |
| mixed.json | json | 0.380 | 0.425 | 0.512 | 101.641 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.936 | 7.898 | 13.565 | 93.469 | 1.00x |
| users.json | orjson | 9.222 | 12.083 | 17.292 | 93.469 | 0.65x |
| users.json | msgspec | 9.006 | 11.313 | 18.963 | 93.469 | 0.70x |
| users.json | ujson | 14.214 | 16.829 | 25.804 | 93.469 | 0.47x |
| users.json | json | 15.714 | 18.990 | 21.126 | 93.469 | 0.42x |
| flat.json | strata | 0.663 | 0.708 | 0.772 | 99.234 | 1.00x |
| flat.json | orjson | 1.115 | 1.163 | 1.243 | 99.234 | 0.61x |
| flat.json | msgspec | 0.865 | 0.916 | 1.177 | 99.234 | 0.77x |
| flat.json | ujson | 1.247 | 1.316 | 1.544 | 99.234 | 0.54x |
| flat.json | json | 1.464 | 1.541 | 1.643 | 99.234 | 0.46x |
| nested.json | strata | 0.647 | 0.673 | 0.900 | 99.234 | 1.00x |
| nested.json | orjson | 1.003 | 1.090 | 1.313 | 99.234 | 0.62x |
| nested.json | msgspec | 0.849 | 0.927 | 1.407 | 99.234 | 0.73x |
| nested.json | ujson | 1.147 | 1.273 | 1.578 | 99.234 | 0.53x |
| nested.json | json | 1.603 | 1.724 | 2.727 | 99.234 | 0.39x |
| wide_arrays.json | strata | 2.987 | 3.228 | 4.270 | 101.625 | 1.00x |
| wide_arrays.json | orjson | 3.613 | 3.696 | 5.412 | 101.625 | 0.87x |
| wide_arrays.json | msgspec | 4.197 | 4.593 | 6.199 | 101.625 | 0.70x |
| wide_arrays.json | ujson | 5.485 | 5.733 | 7.824 | 101.625 | 0.56x |
| wide_arrays.json | json | 6.714 | 6.918 | 10.059 | 101.625 | 0.47x |
| mixed.json | strata | 0.177 | 0.316 | 0.417 | 101.641 | 1.00x |
| mixed.json | orjson | 0.248 | 0.544 | 0.679 | 101.641 | 0.58x |
| mixed.json | msgspec | 0.263 | 0.352 | 0.587 | 101.641 | 0.90x |
| mixed.json | ujson | 0.293 | 0.430 | 0.609 | 101.641 | 0.74x |
| mixed.json | json | 0.407 | 0.551 | 0.831 | 101.641 | 0.57x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.520 | 9.312 | 14.887 | 99.234 | 1.00x |
| users.ndjson | orjson | 12.655 | 14.910 | 18.502 | 99.234 | 0.62x |
| users.ndjson | msgspec | 12.462 | 13.759 | 15.753 | 99.234 | 0.68x |
| users.ndjson | ujson | 15.417 | 16.527 | 24.986 | 99.234 | 0.56x |
| users.ndjson | json | 20.103 | 24.499 | 28.470 | 99.234 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.822 | 2.093 | 2.275 | 93.484 | 1.00x |
| users.json | orjson | 2.534 | 3.303 | 4.117 | 93.484 | 0.63x |
| users.json | msgspec | 3.216 | 3.984 | 4.242 | 93.484 | 0.53x |
| users.json | ujson | 9.540 | 10.379 | 11.361 | 93.484 | 0.20x |
| users.json | json | 16.103 | 18.171 | 22.206 | 93.484 | 0.12x |
| flat.json | strata | 0.470 | 0.567 | 0.956 | 99.234 | 1.00x |
| flat.json | orjson | 0.530 | 0.594 | 0.803 | 99.234 | 0.95x |
| flat.json | msgspec | 0.587 | 0.666 | 0.891 | 99.234 | 0.85x |
| flat.json | ujson | 1.115 | 1.232 | 1.453 | 99.234 | 0.46x |
| flat.json | json | 1.733 | 1.860 | 2.079 | 99.234 | 0.30x |
| nested.json | strata | 0.362 | 0.394 | 0.557 | 99.234 | 1.00x |
| nested.json | orjson | 0.461 | 0.526 | 0.565 | 99.234 | 0.75x |
| nested.json | msgspec | 0.531 | 0.646 | 0.795 | 99.234 | 0.61x |
| nested.json | ujson | 1.087 | 1.252 | 1.435 | 99.234 | 0.31x |
| nested.json | json | 1.974 | 2.066 | 2.268 | 99.234 | 0.19x |
| wide_arrays.json | strata | 1.333 | 1.518 | 1.905 | 101.625 | 1.00x |
| wide_arrays.json | orjson | 1.574 | 1.949 | 2.478 | 101.625 | 0.78x |
| wide_arrays.json | msgspec | 2.468 | 2.762 | 3.214 | 101.625 | 0.55x |
| wide_arrays.json | ujson | 5.215 | 5.540 | 6.026 | 101.625 | 0.27x |
| wide_arrays.json | json | 11.726 | 12.205 | 15.184 | 101.625 | 0.12x |
| mixed.json | strata | 0.156 | 0.213 | 0.542 | 101.641 | 1.00x |
| mixed.json | orjson | 0.204 | 0.235 | 0.343 | 101.641 | 0.91x |
| mixed.json | msgspec | 0.211 | 0.289 | 0.763 | 101.641 | 0.74x |
| mixed.json | ujson | 0.328 | 0.414 | 0.785 | 101.641 | 0.51x |
| mixed.json | json | 0.525 | 0.581 | 0.674 | 101.641 | 0.37x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.058 | 0.074 | 0.097 | 93.531 | 1.00x |
| users.json $[*].id | jmespath | 0.280 | 0.333 | 0.796 | 93.531 | 0.22x |
| users.json $[*].id | jsonpath-ng | 1.474 | 1.645 | 1.916 | 93.531 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.324 | 0.391 | 0.549 | 93.703 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.740 | 1.909 | 2.083 | 93.703 | 0.20x |
| users.json $[*].orders[*].total | jsonpath-ng | 10.737 | 11.614 | 12.642 | 93.703 | 0.03x |
| users.json $..total | strata | 1.238 | 1.489 | 1.990 | 93.812 | 1.00x |
| users.json $..total | jsonpath-ng | 183.550 | 206.322 | 225.259 | 93.812 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.528 | 3.800 | 4.079 | 93.594 | 1.00x |
| users.json $[*].id | orjson+jmespath | 9.762 | 10.908 | 13.885 | 93.594 | 0.35x |
| users.json $[*].id | orjson+jsonpath-ng | 12.053 | 12.394 | 14.299 | 93.594 | 0.31x |
| users.json $[*].orders[*].total | strata | 3.520 | 4.552 | 5.178 | 93.750 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.434 | 14.504 | 17.155 | 93.750 | 0.31x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 26.278 | 29.653 | 41.682 | 93.750 | 0.15x |
| users.json $..total | strata | 8.737 | 9.067 | 9.248 | 93.812 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 213.477 | 219.958 | 236.900 | 93.812 | 0.04x |

