# Benchmark results - medium

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c88786c
- python: 3.14.7
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit-Mach-O
- machine: arm64
- processor: Apple M1 Max
- compiler_flags: -std=c++20 -O3 -march=native
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 41.654 | 43.540 | 46.698 | 139.578 | 1.00x |
| users.json | orjson | 51.391 | 53.137 | 55.426 | 139.578 | 0.82x |
| users.json | msgspec | 51.642 | 54.242 | 55.624 | 139.578 | 0.80x |
| users.json | ujson | 72.888 | 75.523 | 81.691 | 139.578 | 0.58x |
| users.json | json | 103.885 | 107.619 | 110.217 | 139.578 | 0.40x |
| flat.json | strata | 2.394 | 2.399 | 2.479 | 164.359 | 1.00x |
| flat.json | orjson | 2.801 | 2.814 | 2.839 | 164.359 | 0.85x |
| flat.json | msgspec | 2.791 | 2.803 | 2.854 | 164.359 | 0.86x |
| flat.json | ujson | 4.186 | 4.207 | 4.222 | 164.359 | 0.57x |
| flat.json | json | 5.809 | 5.840 | 5.849 | 164.359 | 0.41x |
| nested.json | strata | 2.129 | 2.138 | 2.149 | 165.219 | 1.00x |
| nested.json | orjson | 2.403 | 2.409 | 2.583 | 165.219 | 0.89x |
| nested.json | msgspec | 2.407 | 2.415 | 2.445 | 165.219 | 0.89x |
| nested.json | ujson | 3.518 | 3.528 | 3.543 | 165.219 | 0.61x |
| nested.json | json | 5.615 | 5.641 | 5.795 | 165.219 | 0.38x |
| wide_arrays.json | strata | 12.799 | 12.987 | 13.487 | 192.672 | 1.00x |
| wide_arrays.json | orjson | 13.445 | 13.600 | 15.330 | 192.672 | 0.95x |
| wide_arrays.json | msgspec | 16.599 | 16.740 | 17.480 | 192.672 | 0.78x |
| wide_arrays.json | ujson | 21.190 | 21.434 | 21.991 | 192.672 | 0.61x |
| wide_arrays.json | json | 30.847 | 30.907 | 31.762 | 192.672 | 0.42x |
| mixed.json | strata | 0.520 | 0.524 | 0.613 | 160.656 | 1.00x |
| mixed.json | orjson | 0.590 | 0.593 | 0.688 | 160.656 | 0.88x |
| mixed.json | msgspec | 0.641 | 0.645 | 0.679 | 160.656 | 0.81x |
| mixed.json | ujson | 0.806 | 0.812 | 0.844 | 160.656 | 0.65x |
| mixed.json | json | 1.319 | 1.324 | 1.347 | 160.656 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.032 | 10.266 | 10.420 | 601.219 | 1.00x |
| users.json | orjson | 12.495 | 12.716 | 13.119 | 601.219 | 0.81x |
| users.json | msgspec | 16.297 | 16.506 | 16.871 | 601.219 | 0.62x |
| users.json | ujson | 64.788 | 65.225 | 65.587 | 601.219 | 0.16x |
| users.json | json | 91.135 | 91.683 | 93.666 | 601.219 | 0.11x |
| flat.json | strata | 0.824 | 0.830 | 0.882 | 164.406 | 1.00x |
| flat.json | orjson | 0.880 | 0.883 | 0.887 | 164.406 | 0.94x |
| flat.json | msgspec | 1.157 | 1.161 | 1.171 | 164.406 | 0.71x |
| flat.json | ujson | 3.456 | 3.469 | 3.581 | 164.406 | 0.24x |
| flat.json | json | 5.210 | 5.229 | 5.305 | 164.406 | 0.16x |
| nested.json | strata | 0.556 | 0.561 | 0.626 | 165.312 | 1.00x |
| nested.json | orjson | 0.789 | 0.793 | 0.799 | 165.312 | 0.71x |
| nested.json | msgspec | 1.050 | 1.056 | 1.073 | 165.312 | 0.53x |
| nested.json | ujson | 3.901 | 3.946 | 3.965 | 165.312 | 0.14x |
| nested.json | json | 6.274 | 6.291 | 6.316 | 165.312 | 0.09x |
| wide_arrays.json | strata | 4.155 | 4.512 | 4.698 | 188.547 | 1.00x |
| wide_arrays.json | orjson | 5.040 | 5.160 | 5.254 | 188.547 | 0.87x |
| wide_arrays.json | msgspec | 8.420 | 8.453 | 8.550 | 188.547 | 0.53x |
| wide_arrays.json | ujson | 24.083 | 24.356 | 24.731 | 188.547 | 0.19x |
| wide_arrays.json | json | 44.850 | 45.048 | 45.833 | 188.547 | 0.10x |
| mixed.json | strata | 0.149 | 0.151 | 0.154 | 160.750 | 1.00x |
| mixed.json | orjson | 0.152 | 0.153 | 0.157 | 160.750 | 0.98x |
| mixed.json | msgspec | 0.170 | 0.174 | 0.186 | 160.750 | 0.86x |
| mixed.json | ujson | 0.838 | 0.842 | 0.850 | 160.750 | 0.18x |
| mixed.json | json | 1.328 | 1.338 | 1.357 | 160.750 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 42.736 | 45.143 | 49.340 | 656.359 | 1.00x |
| users.json | orjson | 52.824 | 54.984 | 58.702 | 656.359 | 0.82x |
| users.json | msgspec | 54.260 | 56.313 | 60.766 | 656.359 | 0.80x |
| users.json | ujson | 75.161 | 79.077 | 82.720 | 656.359 | 0.57x |
| users.json | json | 104.398 | 108.756 | 114.739 | 656.359 | 0.42x |
| flat.json | strata | 2.559 | 2.595 | 2.634 | 165.203 | 1.00x |
| flat.json | orjson | 2.970 | 3.004 | 3.026 | 165.203 | 0.86x |
| flat.json | msgspec | 2.975 | 3.034 | 3.066 | 165.203 | 0.86x |
| flat.json | ujson | 4.483 | 4.531 | 4.641 | 165.203 | 0.57x |
| flat.json | json | 5.973 | 6.039 | 6.133 | 165.203 | 0.43x |
| nested.json | strata | 2.277 | 2.303 | 2.380 | 165.312 | 1.00x |
| nested.json | orjson | 2.474 | 2.533 | 2.584 | 165.312 | 0.91x |
| nested.json | msgspec | 2.491 | 2.517 | 2.540 | 165.312 | 0.92x |
| nested.json | ujson | 3.693 | 3.706 | 3.764 | 165.312 | 0.62x |
| nested.json | json | 5.650 | 5.667 | 5.690 | 165.312 | 0.41x |
| wide_arrays.json | strata | 13.471 | 13.538 | 13.895 | 197.500 | 1.00x |
| wide_arrays.json | orjson | 13.908 | 14.210 | 14.321 | 197.500 | 0.95x |
| wide_arrays.json | msgspec | 17.258 | 17.472 | 17.630 | 197.500 | 0.77x |
| wide_arrays.json | ujson | 22.512 | 22.759 | 24.135 | 197.500 | 0.59x |
| wide_arrays.json | json | 31.754 | 32.035 | 32.794 | 197.500 | 0.42x |
| mixed.json | strata | 0.581 | 0.592 | 0.606 | 160.750 | 1.00x |
| mixed.json | orjson | 0.659 | 0.672 | 0.688 | 160.750 | 0.88x |
| mixed.json | msgspec | 0.710 | 0.721 | 0.733 | 160.750 | 0.82x |
| mixed.json | ujson | 0.894 | 0.909 | 0.972 | 160.750 | 0.65x |
| mixed.json | json | 1.386 | 1.408 | 1.466 | 160.750 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 42.824 | 42.967 | 43.766 | 166.125 | 1.00x |
| users.ndjson | orjson | 62.282 | 62.387 | 63.279 | 166.125 | 0.69x |
| users.ndjson | msgspec | 62.137 | 62.978 | 63.666 | 166.125 | 0.68x |
| users.ndjson | ujson | 79.967 | 80.825 | 82.055 | 166.125 | 0.53x |
| users.ndjson | json | 105.452 | 105.793 | 108.146 | 166.125 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.546 | 12.622 | 13.290 | 601.547 | 1.00x |
| users.json | orjson | 14.723 | 15.543 | 16.896 | 601.547 | 0.81x |
| users.json | msgspec | 18.150 | 19.168 | 20.898 | 601.547 | 0.66x |
| users.json | ujson | 66.383 | 67.327 | 69.892 | 601.547 | 0.19x |
| users.json | json | 93.617 | 94.554 | 97.375 | 601.547 | 0.13x |
| flat.json | strata | 1.065 | 1.136 | 1.461 | 165.203 | 1.00x |
| flat.json | orjson | 1.118 | 1.195 | 1.387 | 165.203 | 0.95x |
| flat.json | msgspec | 1.384 | 1.482 | 1.914 | 165.203 | 0.77x |
| flat.json | ujson | 3.771 | 3.878 | 4.014 | 165.203 | 0.29x |
| flat.json | json | 5.487 | 5.592 | 5.709 | 165.203 | 0.20x |
| nested.json | strata | 0.814 | 0.850 | 0.897 | 165.422 | 1.00x |
| nested.json | orjson | 1.042 | 1.067 | 1.089 | 165.422 | 0.80x |
| nested.json | msgspec | 1.319 | 1.383 | 1.499 | 165.422 | 0.61x |
| nested.json | ujson | 4.203 | 4.257 | 4.435 | 165.422 | 0.20x |
| nested.json | json | 6.577 | 6.736 | 6.824 | 165.422 | 0.13x |
| wide_arrays.json | strata | 5.168 | 5.700 | 5.918 | 160.641 | 1.00x |
| wide_arrays.json | orjson | 6.129 | 6.496 | 7.483 | 160.641 | 0.88x |
| wide_arrays.json | msgspec | 9.588 | 9.866 | 10.329 | 160.641 | 0.58x |
| wide_arrays.json | ujson | 25.293 | 25.904 | 26.349 | 160.641 | 0.22x |
| wide_arrays.json | json | 46.195 | 46.833 | 47.553 | 160.641 | 0.12x |
| mixed.json | strata | 0.314 | 0.344 | 0.370 | 160.750 | 1.00x |
| mixed.json | orjson | 0.318 | 0.354 | 0.388 | 160.750 | 0.97x |
| mixed.json | msgspec | 0.338 | 0.364 | 0.404 | 160.750 | 0.95x |
| mixed.json | ujson | 1.039 | 1.055 | 1.098 | 160.750 | 0.33x |
| mixed.json | json | 1.503 | 1.546 | 1.579 | 160.750 | 0.22x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.185 | 0.222 | 0.298 | 601.781 | 1.00x |
| users.json $[*].id | jmespath | 0.787 | 0.818 | 0.837 | 601.781 | 0.27x |
| users.json $[*].id | jsonpath-ng | 3.062 | 3.141 | 3.288 | 601.781 | 0.07x |
| users.json $[*].orders[*].total | strata | 2.600 | 2.902 | 3.385 | 644.453 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 7.020 | 7.228 | 7.480 | 644.453 | 0.40x |
| users.json $[*].orders[*].total | jsonpath-ng | 37.464 | 38.175 | 39.441 | 644.453 | 0.08x |
| users.json $..total | strata | 7.993 | 8.096 | 8.142 | 328.047 | 1.00x |
| users.json $..total | jsonpath-ng | 1104.164 | 1107.026 | 1116.487 | 328.047 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 19.948 | 20.049 | 21.091 | 661.391 | 1.00x |
| users.json $[*].id | orjson+jmespath | 53.015 | 55.044 | 56.917 | 661.391 | 0.36x |
| users.json $[*].id | orjson+jsonpath-ng | 55.647 | 57.060 | 59.743 | 661.391 | 0.35x |
| users.json $[*].orders[*].total | strata | 20.061 | 20.181 | 20.314 | 542.562 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 58.231 | 59.402 | 63.797 | 542.562 | 0.34x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 101.619 | 104.150 | 111.560 | 542.562 | 0.19x |
| users.json $..total | strata | 50.747 | 51.113 | 51.803 | 201.469 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 1167.240 | 1168.905 | 1175.990 | 201.469 | 0.04x |

