# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: ec53f936b8a9245edf5bfeec36c38539666a1775
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
| users.json | strata | 6.406 | 6.890 | 13.527 | 68.594 | 1.00x |
| users.json | orjson | 9.904 | 10.887 | 14.675 | 68.594 | 0.63x |
| users.json | msgspec | 9.509 | 13.095 | 17.286 | 68.594 | 0.53x |
| users.json | ujson | 13.555 | 15.786 | 29.146 | 68.594 | 0.44x |
| users.json | pysimdjson | 139.660 | 144.792 | 188.793 | 68.594 | 0.05x |
| users.json | json | 15.414 | 18.397 | 29.857 | 68.594 | 0.37x |
| flat.json | strata | 0.555 | 0.594 | 0.742 | 95.750 | 1.00x |
| flat.json | orjson | 0.710 | 0.743 | 0.953 | 95.750 | 0.80x |
| flat.json | msgspec | 0.689 | 0.734 | 0.817 | 95.750 | 0.81x |
| flat.json | ujson | 1.087 | 1.203 | 1.623 | 95.750 | 0.49x |
| flat.json | pysimdjson | 11.626 | 12.325 | 16.404 | 95.750 | 0.05x |
| flat.json | json | 1.322 | 1.411 | 1.913 | 95.750 | 0.42x |
| nested.json | strata | 0.558 | 0.639 | 1.091 | 95.750 | 1.00x |
| nested.json | orjson | 0.789 | 0.862 | 1.585 | 95.750 | 0.74x |
| nested.json | msgspec | 0.733 | 0.768 | 1.241 | 95.750 | 0.83x |
| nested.json | ujson | 1.072 | 1.210 | 3.347 | 95.750 | 0.53x |
| nested.json | pysimdjson | 10.799 | 11.493 | 17.259 | 95.750 | 0.06x |
| nested.json | json | 1.502 | 1.542 | 4.082 | 95.750 | 0.41x |
| wide_arrays.json | strata | 3.133 | 3.750 | 4.687 | 98.562 | 1.00x |
| wide_arrays.json | orjson | 3.699 | 4.514 | 6.405 | 98.562 | 0.83x |
| wide_arrays.json | msgspec | 4.097 | 4.849 | 7.790 | 98.562 | 0.77x |
| wide_arrays.json | ujson | 5.535 | 6.364 | 10.586 | 98.562 | 0.59x |
| wide_arrays.json | pysimdjson | 64.251 | 72.061 | 92.884 | 98.562 | 0.05x |
| wide_arrays.json | json | 7.118 | 8.069 | 10.448 | 98.562 | 0.46x |
| mixed.json | strata | 0.151 | 0.174 | 0.621 | 98.578 | 1.00x |
| mixed.json | orjson | 0.171 | 0.198 | 0.702 | 98.578 | 0.88x |
| mixed.json | msgspec | 0.197 | 0.216 | 0.306 | 98.578 | 0.81x |
| mixed.json | ujson | 0.235 | 0.439 | 1.138 | 98.578 | 0.40x |
| mixed.json | pysimdjson | 2.630 | 2.862 | 7.843 | 98.578 | 0.06x |
| mixed.json | json | 0.367 | 0.413 | 1.250 | 98.578 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.435 | 1.775 | 2.211 | 78.375 | 1.00x |
| users.json | orjson | 2.132 | 2.607 | 3.897 | 78.375 | 0.68x |
| users.json | msgspec | 2.739 | 3.211 | 5.142 | 78.375 | 0.55x |
| users.json | ujson | 8.481 | 9.747 | 11.692 | 78.375 | 0.18x |
| users.json | json | 15.585 | 17.561 | 24.276 | 78.375 | 0.10x |
| flat.json | strata | 0.206 | 0.292 | 0.556 | 95.750 | 1.00x |
| flat.json | orjson | 0.236 | 0.290 | 0.454 | 95.750 | 1.01x |
| flat.json | msgspec | 0.295 | 0.380 | 0.751 | 95.750 | 0.77x |
| flat.json | ujson | 0.735 | 0.899 | 1.316 | 95.750 | 0.33x |
| flat.json | json | 1.388 | 1.993 | 3.105 | 95.750 | 0.15x |
| nested.json | strata | 0.145 | 0.159 | 0.221 | 95.750 | 1.00x |
| nested.json | orjson | 0.251 | 0.269 | 0.489 | 95.750 | 0.59x |
| nested.json | msgspec | 0.305 | 0.355 | 0.774 | 95.750 | 0.45x |
| nested.json | ujson | 0.891 | 1.147 | 1.428 | 95.750 | 0.14x |
| nested.json | json | 1.742 | 1.818 | 2.021 | 95.750 | 0.09x |
| wide_arrays.json | strata | 1.107 | 1.274 | 1.855 | 98.562 | 1.00x |
| wide_arrays.json | orjson | 1.279 | 1.606 | 1.978 | 98.562 | 0.79x |
| wide_arrays.json | msgspec | 2.153 | 2.470 | 2.563 | 98.562 | 0.52x |
| wide_arrays.json | ujson | 5.123 | 5.374 | 6.548 | 98.562 | 0.24x |
| wide_arrays.json | json | 11.735 | 12.779 | 16.170 | 98.562 | 0.10x |
| mixed.json | strata | 0.045 | 0.058 | 0.070 | 98.578 | 1.00x |
| mixed.json | orjson | 0.052 | 0.065 | 0.151 | 98.578 | 0.88x |
| mixed.json | msgspec | 0.062 | 0.073 | 0.281 | 98.578 | 0.78x |
| mixed.json | ujson | 0.186 | 0.241 | 0.551 | 98.578 | 0.24x |
| mixed.json | json | 0.394 | 0.478 | 0.906 | 98.578 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.301 | 7.344 | 11.736 | 88.438 | 1.00x |
| users.json | orjson | 10.172 | 11.305 | 13.709 | 88.438 | 0.65x |
| users.json | msgspec | 8.934 | 10.702 | 12.919 | 88.438 | 0.69x |
| users.json | ujson | 11.977 | 15.349 | 29.233 | 88.438 | 0.48x |
| users.json | json | 14.546 | 16.077 | 20.506 | 88.438 | 0.46x |
| flat.json | strata | 0.702 | 0.769 | 0.841 | 95.750 | 1.00x |
| flat.json | orjson | 1.053 | 1.177 | 1.589 | 95.750 | 0.65x |
| flat.json | msgspec | 0.896 | 0.990 | 2.254 | 95.750 | 0.78x |
| flat.json | ujson | 1.208 | 1.348 | 1.541 | 95.750 | 0.57x |
| flat.json | json | 1.563 | 1.633 | 1.982 | 95.750 | 0.47x |
| nested.json | strata | 0.655 | 0.736 | 1.100 | 95.750 | 1.00x |
| nested.json | orjson | 1.058 | 1.114 | 2.622 | 95.750 | 0.66x |
| nested.json | msgspec | 0.873 | 0.957 | 1.080 | 95.750 | 0.77x |
| nested.json | ujson | 1.236 | 1.284 | 1.377 | 95.750 | 0.57x |
| nested.json | json | 1.647 | 1.809 | 4.458 | 95.750 | 0.41x |
| wide_arrays.json | strata | 3.204 | 3.668 | 4.054 | 98.562 | 1.00x |
| wide_arrays.json | orjson | 3.919 | 4.560 | 4.783 | 98.562 | 0.80x |
| wide_arrays.json | msgspec | 4.409 | 5.040 | 6.267 | 98.562 | 0.73x |
| wide_arrays.json | ujson | 5.675 | 6.687 | 7.449 | 98.562 | 0.55x |
| wide_arrays.json | json | 7.334 | 7.961 | 8.419 | 98.562 | 0.46x |
| mixed.json | strata | 0.235 | 0.274 | 0.592 | 98.578 | 1.00x |
| mixed.json | orjson | 0.350 | 0.536 | 1.230 | 98.578 | 0.51x |
| mixed.json | msgspec | 0.311 | 0.412 | 0.878 | 98.578 | 0.67x |
| mixed.json | ujson | 0.385 | 0.466 | 0.584 | 98.578 | 0.59x |
| mixed.json | json | 0.479 | 0.564 | 1.284 | 98.578 | 0.49x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.642 | 7.188 | 7.632 | 95.750 | 1.00x |
| users.ndjson | orjson | 11.496 | 11.847 | 14.254 | 95.750 | 0.61x |
| users.ndjson | msgspec | 11.282 | 12.159 | 16.494 | 95.750 | 0.59x |
| users.ndjson | ujson | 14.187 | 15.213 | 16.303 | 95.750 | 0.47x |
| users.ndjson | json | 18.824 | 19.256 | 23.296 | 95.750 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.024 | 2.557 | 5.857 | 90.062 | 1.00x |
| users.json | orjson | 2.821 | 4.568 | 8.306 | 90.062 | 0.56x |
| users.json | msgspec | 3.819 | 4.371 | 7.549 | 90.062 | 0.58x |
| users.json | ujson | 10.484 | 11.438 | 15.212 | 90.062 | 0.22x |
| users.json | json | 17.948 | 20.071 | 22.216 | 90.062 | 0.13x |
| flat.json | strata | 0.476 | 0.661 | 1.795 | 95.750 | 1.00x |
| flat.json | orjson | 0.485 | 0.722 | 1.461 | 95.750 | 0.92x |
| flat.json | msgspec | 0.626 | 0.719 | 0.830 | 95.750 | 0.92x |
| flat.json | ujson | 1.188 | 1.234 | 2.427 | 95.750 | 0.54x |
| flat.json | json | 1.575 | 1.971 | 3.560 | 95.750 | 0.34x |
| nested.json | strata | 0.501 | 0.563 | 0.639 | 95.750 | 1.00x |
| nested.json | orjson | 0.618 | 0.664 | 0.871 | 95.750 | 0.85x |
| nested.json | msgspec | 0.874 | 0.905 | 0.948 | 95.750 | 0.62x |
| nested.json | ujson | 1.352 | 1.513 | 3.465 | 95.750 | 0.37x |
| nested.json | json | 2.292 | 2.341 | 2.836 | 95.750 | 0.24x |
| wide_arrays.json | strata | 1.489 | 1.736 | 2.392 | 98.562 | 1.00x |
| wide_arrays.json | orjson | 1.726 | 2.169 | 2.850 | 98.562 | 0.80x |
| wide_arrays.json | msgspec | 2.741 | 3.176 | 3.728 | 98.562 | 0.55x |
| wide_arrays.json | ujson | 5.489 | 6.249 | 7.547 | 98.562 | 0.28x |
| wide_arrays.json | json | 12.304 | 14.052 | 16.408 | 98.562 | 0.12x |
| mixed.json | strata | 0.204 | 0.281 | 0.697 | 98.578 | 1.00x |
| mixed.json | orjson | 0.248 | 0.299 | 0.698 | 98.578 | 0.94x |
| mixed.json | msgspec | 0.280 | 0.366 | 0.567 | 98.578 | 0.77x |
| mixed.json | ujson | 0.390 | 0.516 | 0.744 | 98.578 | 0.54x |
| mixed.json | json | 0.598 | 0.699 | 0.816 | 98.578 | 0.40x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.064 | 0.080 | 0.170 | 90.141 | 1.00x |
| users.json $[*].id | jmespath | 0.334 | 0.374 | 0.835 | 90.141 | 0.21x |
| users.json $[*].id | jsonpath-ng | 1.722 | 1.767 | 2.987 | 90.141 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.297 | 0.383 | 0.617 | 90.250 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.628 | 1.857 | 2.005 | 90.250 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 9.977 | 11.558 | 12.369 | 90.250 | 0.03x |
| users.json $..total | strata | 1.292 | 1.882 | 5.937 | 90.281 | 1.00x |
| users.json $..total | jsonpath-ng | 213.116 | 295.620 | 577.393 | 90.281 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.656 | 3.892 | 5.759 | 90.188 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.039 | 11.680 | 14.631 | 90.188 | 0.33x |
| users.json $[*].id | orjson+jsonpath-ng | 11.061 | 12.985 | 18.079 | 90.188 | 0.30x |
| users.json $[*].orders[*].total | strata | 3.686 | 4.059 | 4.715 | 90.250 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 11.454 | 13.561 | 14.876 | 90.250 | 0.30x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 24.283 | 27.971 | 42.200 | 90.250 | 0.15x |
| users.json $..total | strata | 8.096 | 9.817 | 19.119 | 90.297 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 233.862 | 246.838 | 281.890 | 90.297 | 0.04x |

