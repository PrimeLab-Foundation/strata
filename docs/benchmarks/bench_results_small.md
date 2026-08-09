# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 1698e08
- python: 3.14.3
- implementation: CPython
- platform: macOS-26.3-arm64-arm-64bit-Mach-O
- machine: arm64
- processor: Apple M1 Max
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.977 | 10.093 | 10.604 | 52.844 | 1.00x |
| users.json | orjson | 10.705 | 10.816 | 11.353 | 52.844 | 0.93x |
| users.json | msgspec | 10.865 | 10.921 | 11.621 | 52.844 | 0.92x |
| users.json | ujson | 14.348 | 14.522 | 16.043 | 52.844 | 0.70x |
| users.json | json | 19.290 | 19.590 | 20.079 | 52.844 | 0.52x |
| flat.json | strata | 0.983 | 0.992 | 1.044 | 72.594 | 1.00x |
| flat.json | orjson | 0.845 | 0.853 | 0.924 | 72.594 | 1.16x |
| flat.json | msgspec | 0.885 | 0.891 | 0.935 | 72.594 | 1.11x |
| flat.json | ujson | 1.314 | 1.332 | 1.378 | 72.594 | 0.74x |
| flat.json | json | 1.822 | 1.839 | 1.871 | 72.594 | 0.54x |
| nested.json | strata | 0.786 | 0.799 | 0.867 | 72.609 | 1.00x |
| nested.json | orjson | 1.546 | 1.580 | 1.789 | 72.609 | 0.51x |
| nested.json | msgspec | 1.547 | 1.582 | 1.678 | 72.609 | 0.51x |
| nested.json | ujson | 1.909 | 1.958 | 2.168 | 72.609 | 0.41x |
| nested.json | json | 2.504 | 2.517 | 2.589 | 72.609 | 0.32x |
| wide_arrays.json | strata | 4.694 | 4.770 | 5.154 | 72.609 | 1.00x |
| wide_arrays.json | orjson | 4.837 | 4.921 | 5.314 | 72.609 | 0.97x |
| wide_arrays.json | msgspec | 5.776 | 5.916 | 6.394 | 72.609 | 0.81x |
| wide_arrays.json | ujson | 7.329 | 7.450 | 7.958 | 72.609 | 0.64x |
| wide_arrays.json | json | 10.385 | 10.519 | 11.247 | 72.609 | 0.45x |
| mixed.json | strata | 0.207 | 0.211 | 0.216 | 72.625 | 1.00x |
| mixed.json | orjson | 0.164 | 0.167 | 0.174 | 72.625 | 1.27x |
| mixed.json | msgspec | 0.179 | 0.184 | 0.188 | 72.625 | 1.15x |
| mixed.json | ujson | 0.236 | 0.241 | 0.260 | 72.625 | 0.88x |
| mixed.json | json | 0.389 | 0.395 | 0.432 | 72.625 | 0.53x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.024 | 3.045 | 3.051 | 58.453 | 1.00x |
| users.json | orjson | 2.569 | 2.584 | 2.626 | 58.453 | 1.18x |
| users.json | msgspec | 3.387 | 3.413 | 3.533 | 58.453 | 0.89x |
| users.json | ujson | 14.071 | 14.147 | 14.279 | 58.453 | 0.22x |
| users.json | json | 19.498 | 19.642 | 20.402 | 58.453 | 0.16x |
| flat.json | strata | 0.344 | 0.351 | 0.374 | 72.609 | 1.00x |
| flat.json | orjson | 0.291 | 0.296 | 0.305 | 72.609 | 1.19x |
| flat.json | msgspec | 0.380 | 0.386 | 0.416 | 72.609 | 0.91x |
| flat.json | ujson | 1.131 | 1.134 | 1.149 | 72.609 | 0.31x |
| flat.json | json | 1.631 | 1.648 | 1.708 | 72.609 | 0.21x |
| nested.json | strata | 0.271 | 0.285 | 0.315 | 72.609 | 1.00x |
| nested.json | orjson | 0.264 | 0.269 | 0.310 | 72.609 | 1.06x |
| nested.json | msgspec | 0.355 | 0.362 | 0.377 | 72.609 | 0.79x |
| nested.json | ujson | 1.254 | 1.264 | 1.300 | 72.609 | 0.23x |
| nested.json | json | 2.125 | 2.169 | 2.317 | 72.609 | 0.13x |
| wide_arrays.json | strata | 1.927 | 1.957 | 2.010 | 72.609 | 1.00x |
| wide_arrays.json | orjson | 1.453 | 1.463 | 1.623 | 72.609 | 1.34x |
| wide_arrays.json | msgspec | 2.524 | 2.551 | 2.719 | 72.609 | 0.77x |
| wide_arrays.json | ujson | 7.837 | 7.900 | 8.227 | 72.609 | 0.25x |
| wide_arrays.json | json | 14.016 | 14.102 | 14.495 | 72.609 | 0.14x |
| mixed.json | strata | 0.067 | 0.071 | 0.089 | 72.625 | 1.00x |
| mixed.json | orjson | 0.058 | 0.061 | 0.074 | 72.625 | 1.17x |
| mixed.json | msgspec | 0.064 | 0.068 | 0.075 | 72.625 | 1.05x |
| mixed.json | ujson | 0.280 | 0.282 | 0.361 | 72.625 | 0.25x |
| mixed.json | json | 0.436 | 0.452 | 0.500 | 72.625 | 0.16x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.355 | 10.444 | 10.651 | 70.125 | 1.00x |
| users.json | orjson | 13.089 | 13.446 | 13.961 | 70.125 | 0.78x |
| users.json | msgspec | 13.164 | 13.318 | 13.777 | 70.125 | 0.78x |
| users.json | ujson | 16.925 | 17.103 | 18.329 | 70.125 | 0.61x |
| users.json | json | 21.636 | 21.759 | 22.429 | 70.125 | 0.48x |
| flat.json | strata | 1.064 | 1.080 | 1.180 | 72.609 | 1.00x |
| flat.json | orjson | 0.921 | 0.939 | 0.998 | 72.609 | 1.15x |
| flat.json | msgspec | 0.970 | 0.996 | 1.045 | 72.609 | 1.08x |
| flat.json | ujson | 1.433 | 1.453 | 1.588 | 72.609 | 0.74x |
| flat.json | json | 1.901 | 1.919 | 1.994 | 72.609 | 0.56x |
| nested.json | strata | 0.855 | 0.873 | 0.947 | 72.609 | 1.00x |
| nested.json | orjson | 1.746 | 1.828 | 2.060 | 72.609 | 0.48x |
| nested.json | msgspec | 1.747 | 1.814 | 2.250 | 72.609 | 0.48x |
| nested.json | ujson | 2.151 | 2.240 | 2.569 | 72.609 | 0.39x |
| nested.json | json | 2.693 | 2.767 | 2.926 | 72.609 | 0.32x |
| wide_arrays.json | strata | 4.893 | 4.919 | 4.961 | 72.625 | 1.00x |
| wide_arrays.json | orjson | 5.446 | 5.521 | 5.828 | 72.625 | 0.89x |
| wide_arrays.json | msgspec | 6.416 | 6.508 | 6.725 | 72.625 | 0.76x |
| wide_arrays.json | ujson | 8.118 | 8.166 | 8.416 | 72.625 | 0.60x |
| wide_arrays.json | json | 11.021 | 11.078 | 11.360 | 72.625 | 0.44x |
| mixed.json | strata | 0.253 | 0.259 | 0.264 | 72.672 | 1.00x |
| mixed.json | orjson | 0.212 | 0.221 | 0.266 | 72.672 | 1.17x |
| mixed.json | msgspec | 0.232 | 0.242 | 0.258 | 72.672 | 1.07x |
| mixed.json | ujson | 0.302 | 0.324 | 0.341 | 72.672 | 0.80x |
| mixed.json | json | 0.436 | 0.444 | 0.462 | 72.672 | 0.58x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.915 | 10.003 | 10.392 | 71.719 | 1.00x |
| users.ndjson | orjson | 13.417 | 13.658 | 14.909 | 71.719 | 0.73x |
| users.ndjson | msgspec | 13.096 | 13.187 | 13.562 | 71.719 | 0.76x |
| users.ndjson | ujson | 16.704 | 16.859 | 17.774 | 71.719 | 0.59x |
| users.ndjson | json | 23.382 | 24.636 | 25.542 | 71.719 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.502 | 3.534 | 3.622 | 66.141 | 1.00x |
| users.json | orjson | 3.010 | 3.064 | 3.253 | 66.141 | 1.15x |
| users.json | msgspec | 3.779 | 3.865 | 3.992 | 66.141 | 0.91x |
| users.json | ujson | 14.184 | 14.387 | 14.565 | 66.141 | 0.25x |
| users.json | json | 20.039 | 20.204 | 20.477 | 66.141 | 0.17x |
| flat.json | strata | 0.520 | 0.552 | 0.685 | 72.609 | 1.00x |
| flat.json | orjson | 0.442 | 0.462 | 0.547 | 72.609 | 1.19x |
| flat.json | msgspec | 0.530 | 0.539 | 0.570 | 72.609 | 1.02x |
| flat.json | ujson | 1.287 | 1.316 | 1.446 | 72.609 | 0.42x |
| flat.json | json | 1.798 | 1.831 | 1.957 | 72.609 | 0.30x |
| nested.json | strata | 0.448 | 0.463 | 0.502 | 72.609 | 1.00x |
| nested.json | orjson | 0.394 | 0.416 | 0.450 | 72.609 | 1.11x |
| nested.json | msgspec | 0.490 | 0.500 | 0.521 | 72.609 | 0.93x |
| nested.json | ujson | 1.518 | 1.528 | 1.596 | 72.609 | 0.30x |
| nested.json | json | 2.220 | 2.248 | 2.347 | 72.609 | 0.21x |
| wide_arrays.json | strata | 2.283 | 2.355 | 2.460 | 72.625 | 1.00x |
| wide_arrays.json | orjson | 1.750 | 1.819 | 1.941 | 72.625 | 1.29x |
| wide_arrays.json | msgspec | 2.819 | 2.905 | 3.006 | 72.625 | 0.81x |
| wide_arrays.json | ujson | 8.203 | 8.269 | 8.631 | 72.625 | 0.28x |
| wide_arrays.json | json | 14.386 | 14.499 | 14.630 | 72.625 | 0.16x |
| mixed.json | strata | 0.208 | 0.237 | 0.303 | 72.703 | 1.00x |
| mixed.json | orjson | 0.177 | 0.208 | 0.354 | 72.703 | 1.14x |
| mixed.json | msgspec | 0.185 | 0.198 | 0.250 | 72.703 | 1.20x |
| mixed.json | ujson | 0.408 | 0.438 | 0.487 | 72.703 | 0.54x |
| mixed.json | json | 0.564 | 0.591 | 0.623 | 72.703 | 0.40x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.066 | 0.073 | 0.078 | 66.516 | 1.00x |
| users.json $[*].id | jmespath | 0.428 | 0.437 | 0.508 | 66.516 | 0.17x |
| users.json $[*].id | jsonpath-ng | 4.193 | 4.274 | 4.927 | 66.516 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.423 | 0.432 | 0.458 | 68.812 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.633 | 2.664 | 2.931 | 68.812 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 14.551 | 15.046 | 16.964 | 68.812 | 0.03x |
| users.json $..total | strata | 1.772 | 1.789 | 1.822 | 70.922 | 1.00x |
| users.json $..total | jsonpath-ng | 271.325 | 273.650 | 281.221 | 70.922 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.857 | 4.953 | 5.171 | 68.344 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.525 | 13.723 | 14.330 | 68.344 | 0.36x |
| users.json $[*].id | orjson+jsonpath-ng | 15.125 | 15.255 | 15.874 | 68.344 | 0.32x |
| users.json $[*].orders[*].total | strata | 4.977 | 5.110 | 5.209 | 70.875 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.852 | 16.022 | 16.424 | 70.875 | 0.32x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 25.736 | 25.833 | 27.865 | 70.875 | 0.20x |
| users.json $..total | strata | 14.704 | 15.029 | 15.464 | 71.859 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 282.359 | 283.205 | 288.310 | 71.859 | 0.05x |

