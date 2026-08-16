# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 6309d05
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
| users.json | strata | 7.900 | 9.916 | 17.789 | 63.844 | 1.00x |
| users.json | orjson | 10.751 | 13.361 | 22.032 | 63.844 | 0.74x |
| users.json | msgspec | 10.071 | 12.156 | 16.611 | 63.844 | 0.82x |
| users.json | ujson | 14.441 | 17.675 | 23.065 | 63.844 | 0.56x |
| users.json | pysimdjson | 141.057 | 155.468 | 198.712 | 63.844 | 0.06x |
| users.json | json | 17.389 | 21.373 | 30.271 | 63.844 | 0.46x |
| flat.json | strata | 0.635 | 0.807 | 1.899 | 93.234 | 1.00x |
| flat.json | orjson | 0.721 | 0.930 | 1.688 | 93.234 | 0.87x |
| flat.json | msgspec | 0.706 | 0.777 | 0.845 | 93.234 | 1.04x |
| flat.json | ujson | 1.189 | 1.374 | 2.231 | 93.234 | 0.59x |
| flat.json | pysimdjson | 11.855 | 13.940 | 16.942 | 93.234 | 0.06x |
| flat.json | json | 1.348 | 1.551 | 2.087 | 93.234 | 0.52x |
| nested.json | strata | 0.579 | 0.650 | 0.702 | 93.250 | 1.00x |
| nested.json | orjson | 0.732 | 0.779 | 0.943 | 93.250 | 0.83x |
| nested.json | msgspec | 0.677 | 0.746 | 0.940 | 93.250 | 0.87x |
| nested.json | ujson | 1.042 | 1.166 | 2.982 | 93.250 | 0.56x |
| nested.json | pysimdjson | 10.417 | 11.178 | 13.883 | 93.250 | 0.06x |
| nested.json | json | 1.460 | 1.564 | 2.139 | 93.250 | 0.42x |
| wide_arrays.json | strata | 3.604 | 5.017 | 11.263 | 96.031 | 1.00x |
| wide_arrays.json | orjson | 3.975 | 4.239 | 13.084 | 96.031 | 1.18x |
| wide_arrays.json | msgspec | 4.314 | 5.007 | 8.141 | 96.031 | 1.00x |
| wide_arrays.json | ujson | 6.111 | 8.016 | 19.733 | 96.031 | 0.63x |
| wide_arrays.json | pysimdjson | 68.190 | 91.314 | 142.046 | 96.031 | 0.05x |
| wide_arrays.json | json | 7.276 | 8.630 | 13.232 | 96.031 | 0.58x |
| mixed.json | strata | 0.157 | 0.176 | 0.217 | 96.047 | 1.00x |
| mixed.json | orjson | 0.196 | 0.209 | 0.223 | 96.047 | 0.85x |
| mixed.json | msgspec | 0.190 | 0.210 | 0.223 | 96.047 | 0.84x |
| mixed.json | ujson | 0.242 | 0.337 | 0.565 | 96.047 | 0.52x |
| mixed.json | pysimdjson | 2.952 | 3.181 | 3.343 | 96.047 | 0.06x |
| mixed.json | json | 0.371 | 0.404 | 0.451 | 96.047 | 0.44x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.065 | 2.505 | 5.812 | 82.109 | 1.00x |
| users.json | orjson | 2.692 | 3.007 | 6.086 | 82.109 | 0.83x |
| users.json | msgspec | 3.353 | 3.822 | 4.187 | 82.109 | 0.66x |
| users.json | ujson | 11.909 | 17.235 | 21.760 | 82.109 | 0.15x |
| users.json | json | 17.008 | 24.271 | 32.268 | 82.109 | 0.10x |
| flat.json | strata | 0.201 | 0.271 | 0.500 | 93.250 | 1.00x |
| flat.json | orjson | 0.268 | 0.318 | 0.506 | 93.250 | 0.85x |
| flat.json | msgspec | 0.331 | 0.382 | 0.616 | 93.250 | 0.71x |
| flat.json | ujson | 0.941 | 1.170 | 1.368 | 93.250 | 0.23x |
| flat.json | json | 1.427 | 1.665 | 1.942 | 93.250 | 0.16x |
| nested.json | strata | 0.155 | 0.178 | 0.208 | 93.250 | 1.00x |
| nested.json | orjson | 0.240 | 0.264 | 0.389 | 93.250 | 0.67x |
| nested.json | msgspec | 0.307 | 0.413 | 0.757 | 93.250 | 0.43x |
| nested.json | ujson | 1.105 | 1.230 | 2.951 | 93.250 | 0.14x |
| nested.json | json | 1.708 | 2.047 | 2.826 | 93.250 | 0.09x |
| wide_arrays.json | strata | 1.239 | 1.362 | 2.167 | 96.031 | 1.00x |
| wide_arrays.json | orjson | 1.322 | 1.521 | 1.855 | 96.031 | 0.90x |
| wide_arrays.json | msgspec | 2.210 | 2.365 | 2.618 | 96.031 | 0.58x |
| wide_arrays.json | ujson | 6.746 | 6.963 | 8.822 | 96.031 | 0.20x |
| wide_arrays.json | json | 12.303 | 13.156 | 16.623 | 96.031 | 0.10x |
| mixed.json | strata | 0.046 | 0.054 | 0.070 | 96.047 | 1.00x |
| mixed.json | orjson | 0.046 | 0.061 | 0.075 | 96.047 | 0.88x |
| mixed.json | msgspec | 0.057 | 0.073 | 0.223 | 96.047 | 0.73x |
| mixed.json | ujson | 0.241 | 0.287 | 0.418 | 96.047 | 0.19x |
| mixed.json | json | 0.382 | 0.441 | 0.598 | 96.047 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.483 | 11.164 | 17.114 | 92.250 | 1.00x |
| users.json | orjson | 13.408 | 14.583 | 19.675 | 92.250 | 0.77x |
| users.json | msgspec | 12.949 | 13.884 | 20.450 | 92.250 | 0.80x |
| users.json | ujson | 17.361 | 21.530 | 24.749 | 92.250 | 0.52x |
| users.json | json | 20.289 | 23.523 | 32.770 | 92.250 | 0.47x |
| flat.json | strata | 0.725 | 0.891 | 1.698 | 93.250 | 1.00x |
| flat.json | orjson | 0.960 | 1.105 | 1.285 | 93.250 | 0.81x |
| flat.json | msgspec | 0.810 | 1.076 | 2.297 | 93.250 | 0.83x |
| flat.json | ujson | 1.156 | 1.452 | 1.805 | 93.250 | 0.61x |
| flat.json | json | 1.399 | 1.702 | 2.158 | 93.250 | 0.52x |
| nested.json | strata | 0.690 | 0.785 | 2.340 | 93.250 | 1.00x |
| nested.json | orjson | 1.029 | 1.211 | 2.807 | 93.250 | 0.65x |
| nested.json | msgspec | 0.844 | 1.041 | 2.986 | 93.250 | 0.75x |
| nested.json | ujson | 1.147 | 1.361 | 4.287 | 93.250 | 0.58x |
| nested.json | json | 1.588 | 1.941 | 4.128 | 93.250 | 0.40x |
| wide_arrays.json | strata | 3.691 | 4.124 | 6.880 | 96.031 | 1.00x |
| wide_arrays.json | orjson | 3.819 | 5.036 | 8.344 | 96.031 | 0.82x |
| wide_arrays.json | msgspec | 4.514 | 5.077 | 9.193 | 96.031 | 0.81x |
| wide_arrays.json | ujson | 6.122 | 6.690 | 8.315 | 96.031 | 0.62x |
| wide_arrays.json | json | 7.103 | 8.665 | 14.326 | 96.031 | 0.48x |
| mixed.json | strata | 0.191 | 0.236 | 0.285 | 96.047 | 1.00x |
| mixed.json | orjson | 0.245 | 0.430 | 0.755 | 96.047 | 0.55x |
| mixed.json | msgspec | 0.280 | 0.315 | 0.387 | 96.047 | 0.75x |
| mixed.json | ujson | 0.335 | 0.359 | 0.398 | 96.047 | 0.66x |
| mixed.json | json | 0.456 | 0.473 | 0.526 | 96.047 | 0.50x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 8.227 | 9.394 | 12.682 | 93.234 | 1.00x |
| users.ndjson | orjson | 11.982 | 12.949 | 15.980 | 93.234 | 0.73x |
| users.ndjson | msgspec | 11.663 | 13.376 | 16.569 | 93.234 | 0.70x |
| users.ndjson | ujson | 15.437 | 17.138 | 24.193 | 93.234 | 0.55x |
| users.ndjson | json | 19.535 | 23.262 | 27.948 | 93.234 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.540 | 2.904 | 3.436 | 88.547 | 1.00x |
| users.json | orjson | 2.689 | 3.410 | 3.901 | 88.547 | 0.85x |
| users.json | msgspec | 3.616 | 4.125 | 7.156 | 88.547 | 0.70x |
| users.json | ujson | 11.884 | 13.232 | 21.099 | 88.547 | 0.22x |
| users.json | json | 16.754 | 18.440 | 29.037 | 88.547 | 0.16x |
| flat.json | strata | 0.502 | 0.637 | 0.754 | 93.250 | 1.00x |
| flat.json | orjson | 0.537 | 0.728 | 1.394 | 93.250 | 0.88x |
| flat.json | msgspec | 0.491 | 0.847 | 1.595 | 93.250 | 0.75x |
| flat.json | ujson | 1.051 | 1.731 | 3.800 | 93.250 | 0.37x |
| flat.json | json | 1.472 | 2.186 | 2.649 | 93.250 | 0.29x |
| nested.json | strata | 0.294 | 0.379 | 0.442 | 93.250 | 1.00x |
| nested.json | orjson | 0.371 | 0.449 | 0.505 | 93.250 | 0.84x |
| nested.json | msgspec | 0.418 | 0.507 | 0.683 | 93.250 | 0.75x |
| nested.json | ujson | 1.214 | 1.423 | 1.664 | 93.250 | 0.27x |
| nested.json | json | 1.770 | 1.935 | 2.414 | 93.250 | 0.20x |
| wide_arrays.json | strata | 1.706 | 2.343 | 2.872 | 96.031 | 1.00x |
| wide_arrays.json | orjson | 1.872 | 2.518 | 3.386 | 96.031 | 0.93x |
| wide_arrays.json | msgspec | 2.634 | 3.297 | 4.204 | 96.031 | 0.71x |
| wide_arrays.json | ujson | 7.439 | 8.169 | 11.617 | 96.031 | 0.29x |
| wide_arrays.json | json | 12.470 | 15.507 | 16.458 | 96.031 | 0.15x |
| mixed.json | strata | 0.124 | 0.243 | 0.334 | 96.047 | 1.00x |
| mixed.json | orjson | 0.153 | 0.263 | 0.329 | 96.047 | 0.92x |
| mixed.json | msgspec | 0.134 | 0.241 | 0.673 | 96.047 | 1.01x |
| mixed.json | ujson | 0.338 | 0.500 | 0.806 | 96.047 | 0.49x |
| mixed.json | json | 0.431 | 0.668 | 0.744 | 96.047 | 0.36x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.081 | 0.147 | 0.413 | 88.594 | 1.00x |
| users.json $[*].id | jmespath | 0.328 | 0.465 | 1.354 | 88.594 | 0.32x |
| users.json $[*].id | jsonpath-ng | 1.595 | 2.286 | 7.640 | 88.594 | 0.06x |
| users.json $[*].orders[*].total | strata | 0.328 | 0.499 | 0.781 | 88.734 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.772 | 1.926 | 2.533 | 88.734 | 0.26x |
| users.json $[*].orders[*].total | jsonpath-ng | 11.053 | 12.097 | 13.241 | 88.734 | 0.04x |
| users.json $..total | strata | 1.364 | 1.564 | 4.305 | 90.766 | 1.00x |
| users.json $..total | jsonpath-ng | 212.366 | 236.705 | 266.402 | 90.766 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.763 | 4.114 | 8.270 | 88.641 | 1.00x |
| users.json $[*].id | orjson+jmespath | 11.246 | 13.356 | 25.174 | 88.641 | 0.31x |
| users.json $[*].id | orjson+jsonpath-ng | 12.177 | 13.101 | 21.652 | 88.641 | 0.31x |
| users.json $[*].orders[*].total | strata | 3.787 | 4.028 | 4.483 | 90.766 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.054 | 12.881 | 14.833 | 90.766 | 0.31x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 24.208 | 26.291 | 33.275 | 90.766 | 0.15x |
| users.json $..total | strata | 10.472 | 12.216 | 17.590 | 90.766 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 225.848 | 244.882 | 275.534 | 90.766 | 0.05x |

