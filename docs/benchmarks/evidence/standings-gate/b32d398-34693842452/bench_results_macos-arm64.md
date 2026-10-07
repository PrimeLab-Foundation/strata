# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: b32d39824b8fea15839872cab8834a1b0fef1294
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
| users.json | strata | 6.043 | 6.964 | 8.412 | 67.734 | 1.00x |
| users.json | orjson | 8.890 | 10.631 | 11.992 | 67.734 | 0.66x |
| users.json | msgspec | 8.914 | 10.564 | 12.692 | 67.734 | 0.66x |
| users.json | ujson | 11.470 | 14.161 | 16.723 | 67.734 | 0.49x |
| users.json | pysimdjson | 126.418 | 141.215 | 190.811 | 67.734 | 0.05x |
| users.json | json | 14.201 | 17.268 | 21.742 | 67.734 | 0.40x |
| flat.json | strata | 0.553 | 0.615 | 0.682 | 99.141 | 1.00x |
| flat.json | orjson | 0.741 | 0.814 | 0.868 | 99.141 | 0.76x |
| flat.json | msgspec | 0.741 | 0.782 | 0.843 | 99.141 | 0.79x |
| flat.json | ujson | 1.198 | 1.354 | 1.659 | 99.141 | 0.45x |
| flat.json | pysimdjson | 12.368 | 12.879 | 14.115 | 99.141 | 0.05x |
| flat.json | json | 1.352 | 1.489 | 1.580 | 99.141 | 0.41x |
| nested.json | strata | 0.480 | 0.537 | 0.606 | 99.141 | 1.00x |
| nested.json | orjson | 0.646 | 0.724 | 0.863 | 99.141 | 0.74x |
| nested.json | msgspec | 0.636 | 0.681 | 0.781 | 99.141 | 0.79x |
| nested.json | ujson | 0.997 | 1.094 | 1.516 | 99.141 | 0.49x |
| nested.json | pysimdjson | 9.755 | 10.500 | 11.242 | 99.141 | 0.05x |
| nested.json | json | 1.315 | 1.456 | 1.857 | 99.141 | 0.37x |
| wide_arrays.json | strata | 2.816 | 3.173 | 3.461 | 100.734 | 1.00x |
| wide_arrays.json | orjson | 3.344 | 3.775 | 3.919 | 100.734 | 0.84x |
| wide_arrays.json | msgspec | 3.773 | 4.286 | 4.533 | 100.734 | 0.74x |
| wide_arrays.json | ujson | 4.979 | 5.564 | 5.850 | 100.734 | 0.57x |
| wide_arrays.json | pysimdjson | 60.992 | 67.116 | 67.849 | 100.734 | 0.05x |
| wide_arrays.json | json | 6.354 | 7.110 | 8.091 | 100.734 | 0.45x |
| mixed.json | strata | 0.114 | 0.115 | 0.127 | 100.750 | 1.00x |
| mixed.json | orjson | 0.143 | 0.149 | 0.181 | 100.750 | 0.77x |
| mixed.json | msgspec | 0.156 | 0.158 | 0.161 | 100.750 | 0.73x |
| mixed.json | ujson | 0.194 | 0.233 | 0.429 | 100.750 | 0.49x |
| mixed.json | pysimdjson | 2.353 | 2.375 | 2.607 | 100.750 | 0.05x |
| mixed.json | json | 0.300 | 0.310 | 0.479 | 100.750 | 0.37x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.418 | 1.545 | 1.963 | 82.031 | 1.00x |
| users.json | orjson | 2.219 | 2.429 | 2.658 | 82.031 | 0.64x |
| users.json | msgspec | 2.871 | 3.178 | 4.514 | 82.031 | 0.49x |
| users.json | ujson | 8.637 | 9.203 | 10.084 | 82.031 | 0.17x |
| users.json | json | 15.605 | 16.302 | 18.148 | 82.031 | 0.09x |
| flat.json | strata | 0.202 | 0.234 | 0.261 | 99.141 | 1.00x |
| flat.json | orjson | 0.264 | 0.280 | 0.300 | 99.141 | 0.83x |
| flat.json | msgspec | 0.328 | 0.340 | 0.457 | 99.141 | 0.69x |
| flat.json | ujson | 0.805 | 0.825 | 0.884 | 99.141 | 0.28x |
| flat.json | json | 1.526 | 1.576 | 1.619 | 99.141 | 0.15x |
| nested.json | strata | 0.119 | 0.130 | 0.137 | 99.141 | 1.00x |
| nested.json | orjson | 0.209 | 0.223 | 0.273 | 99.141 | 0.58x |
| nested.json | msgspec | 0.284 | 0.500 | 0.517 | 99.141 | 0.26x |
| nested.json | ujson | 0.790 | 0.826 | 1.015 | 99.141 | 0.16x |
| nested.json | json | 1.621 | 1.672 | 1.736 | 99.141 | 0.08x |
| wide_arrays.json | strata | 1.050 | 1.225 | 1.441 | 100.734 | 1.00x |
| wide_arrays.json | orjson | 1.357 | 1.512 | 1.624 | 100.734 | 0.81x |
| wide_arrays.json | msgspec | 2.094 | 2.304 | 2.452 | 100.734 | 0.53x |
| wide_arrays.json | ujson | 4.534 | 5.048 | 5.304 | 100.734 | 0.24x |
| wide_arrays.json | json | 11.098 | 12.139 | 12.814 | 100.734 | 0.10x |
| mixed.json | strata | 0.035 | 0.037 | 0.046 | 100.750 | 1.00x |
| mixed.json | orjson | 0.043 | 0.048 | 0.056 | 100.750 | 0.77x |
| mixed.json | msgspec | 0.050 | 0.055 | 0.138 | 100.750 | 0.67x |
| mixed.json | ujson | 0.164 | 0.169 | 0.224 | 100.750 | 0.22x |
| mixed.json | json | 0.334 | 0.356 | 0.462 | 100.750 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.760 | 7.203 | 7.776 | 92.672 | 1.00x |
| users.json | orjson | 9.956 | 10.499 | 12.193 | 92.672 | 0.69x |
| users.json | msgspec | 9.624 | 10.650 | 10.877 | 92.672 | 0.68x |
| users.json | ujson | 13.123 | 14.898 | 15.947 | 92.672 | 0.48x |
| users.json | json | 15.983 | 17.672 | 18.925 | 92.672 | 0.41x |
| flat.json | strata | 0.568 | 0.660 | 0.717 | 99.141 | 1.00x |
| flat.json | orjson | 0.767 | 0.901 | 1.116 | 99.141 | 0.73x |
| flat.json | msgspec | 0.714 | 0.777 | 0.901 | 99.141 | 0.85x |
| flat.json | ujson | 1.046 | 1.094 | 1.277 | 99.141 | 0.60x |
| flat.json | json | 1.291 | 1.413 | 2.246 | 99.141 | 0.47x |
| nested.json | strata | 0.528 | 0.552 | 0.697 | 99.141 | 1.00x |
| nested.json | orjson | 0.786 | 0.863 | 1.090 | 99.141 | 0.64x |
| nested.json | msgspec | 0.715 | 0.735 | 0.871 | 99.141 | 0.75x |
| nested.json | ujson | 0.997 | 1.035 | 1.161 | 99.141 | 0.53x |
| nested.json | json | 1.426 | 1.444 | 1.645 | 99.141 | 0.38x |
| wide_arrays.json | strata | 3.174 | 3.361 | 3.463 | 100.734 | 1.00x |
| wide_arrays.json | orjson | 3.860 | 3.947 | 4.135 | 100.734 | 0.85x |
| wide_arrays.json | msgspec | 4.402 | 4.598 | 4.788 | 100.734 | 0.73x |
| wide_arrays.json | ujson | 5.747 | 5.935 | 6.453 | 100.734 | 0.57x |
| wide_arrays.json | json | 7.182 | 7.491 | 7.893 | 100.734 | 0.45x |
| mixed.json | strata | 0.151 | 0.159 | 0.179 | 100.750 | 1.00x |
| mixed.json | orjson | 0.202 | 0.236 | 0.433 | 100.750 | 0.67x |
| mixed.json | msgspec | 0.215 | 0.238 | 0.280 | 100.750 | 0.67x |
| mixed.json | ujson | 0.265 | 0.295 | 0.348 | 100.750 | 0.54x |
| mixed.json | json | 0.365 | 0.386 | 0.477 | 100.750 | 0.41x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.751 | 7.448 | 8.667 | 99.141 | 1.00x |
| users.ndjson | orjson | 11.811 | 13.109 | 13.978 | 99.141 | 0.57x |
| users.ndjson | msgspec | 11.572 | 13.451 | 17.923 | 99.141 | 0.55x |
| users.ndjson | ujson | 14.246 | 15.677 | 17.863 | 99.141 | 0.48x |
| users.ndjson | json | 18.714 | 20.916 | 23.753 | 99.141 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.813 | 1.999 | 6.934 | 93.438 | 1.00x |
| users.json | orjson | 2.714 | 2.977 | 3.851 | 93.438 | 0.67x |
| users.json | msgspec | 3.392 | 3.610 | 4.698 | 93.438 | 0.55x |
| users.json | ujson | 9.373 | 10.053 | 11.506 | 93.438 | 0.20x |
| users.json | json | 16.413 | 16.878 | 24.157 | 93.438 | 0.12x |
| flat.json | strata | 0.321 | 0.331 | 0.414 | 99.141 | 1.00x |
| flat.json | orjson | 0.366 | 0.381 | 0.438 | 99.141 | 0.87x |
| flat.json | msgspec | 0.429 | 0.453 | 0.874 | 99.141 | 0.73x |
| flat.json | ujson | 0.851 | 0.881 | 1.037 | 99.141 | 0.38x |
| flat.json | json | 1.429 | 1.543 | 1.581 | 99.141 | 0.21x |
| nested.json | strata | 0.236 | 0.254 | 0.302 | 99.141 | 1.00x |
| nested.json | orjson | 0.337 | 0.370 | 0.413 | 99.141 | 0.69x |
| nested.json | msgspec | 0.413 | 0.544 | 0.633 | 99.141 | 0.47x |
| nested.json | ujson | 0.948 | 0.995 | 1.195 | 99.141 | 0.25x |
| nested.json | json | 1.685 | 1.729 | 1.825 | 99.141 | 0.15x |
| wide_arrays.json | strata | 1.425 | 1.491 | 1.631 | 100.734 | 1.00x |
| wide_arrays.json | orjson | 1.791 | 1.874 | 2.343 | 100.734 | 0.80x |
| wide_arrays.json | msgspec | 2.487 | 2.630 | 2.790 | 100.734 | 0.57x |
| wide_arrays.json | ujson | 5.107 | 5.196 | 5.328 | 100.734 | 0.29x |
| wide_arrays.json | json | 11.574 | 11.831 | 13.013 | 100.734 | 0.13x |
| mixed.json | strata | 0.135 | 0.157 | 0.188 | 100.750 | 1.00x |
| mixed.json | orjson | 0.145 | 0.182 | 0.258 | 100.750 | 0.86x |
| mixed.json | msgspec | 0.157 | 0.183 | 0.361 | 100.750 | 0.86x |
| mixed.json | ujson | 0.280 | 0.302 | 0.329 | 100.750 | 0.52x |
| mixed.json | json | 0.431 | 0.495 | 0.542 | 100.750 | 0.32x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.073 | 0.096 | 0.207 | 93.484 | 1.00x |
| users.json $[*].id | jmespath | 0.293 | 0.370 | 0.665 | 93.484 | 0.26x |
| users.json $[*].id | jsonpath-ng | 1.644 | 1.748 | 2.119 | 93.484 | 0.06x |
| users.json $[*].orders[*].total | strata | 0.346 | 0.545 | 0.721 | 93.719 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.863 | 2.087 | 2.620 | 93.719 | 0.26x |
| users.json $[*].orders[*].total | jsonpath-ng | 11.168 | 12.717 | 14.063 | 93.719 | 0.04x |
| users.json $..total | strata | 1.353 | 1.433 | 1.579 | 93.719 | 1.00x |
| users.json $..total | jsonpath-ng | 201.143 | 208.663 | 213.551 | 93.719 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.654 | 3.894 | 4.041 | 93.562 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.401 | 11.601 | 12.252 | 93.562 | 0.34x |
| users.json $[*].id | orjson+jsonpath-ng | 11.721 | 13.102 | 13.978 | 93.562 | 0.30x |
| users.json $[*].orders[*].total | strata | 3.883 | 4.151 | 5.922 | 93.719 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.102 | 13.248 | 18.439 | 93.719 | 0.31x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 25.207 | 27.095 | 37.870 | 93.719 | 0.15x |
| users.json $..total | strata | 8.129 | 8.745 | 10.416 | 93.734 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 207.724 | 221.208 | 272.023 | 93.734 | 0.04x |

