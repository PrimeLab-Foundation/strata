# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 68d6e74
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
| users.json | strata | 6.915 | 6.953 | 7.393 | 50.844 | 1.00x |
| users.json | orjson | 7.880 | 7.928 | 8.277 | 50.844 | 0.88x |
| users.json | msgspec | 7.970 | 8.023 | 8.342 | 50.844 | 0.87x |
| users.json | ujson | 10.765 | 10.879 | 11.472 | 50.844 | 0.64x |
| users.json | json | 15.005 | 15.057 | 15.243 | 50.844 | 0.46x |
| flat.json | strata | 0.628 | 0.635 | 0.659 | 70.609 | 1.00x |
| flat.json | orjson | 0.638 | 0.644 | 0.734 | 70.609 | 0.99x |
| flat.json | msgspec | 0.674 | 0.682 | 0.798 | 70.609 | 0.93x |
| flat.json | ujson | 0.992 | 1.019 | 1.110 | 70.609 | 0.62x |
| flat.json | json | 1.410 | 1.434 | 1.610 | 70.609 | 0.44x |
| nested.json | strata | 0.563 | 0.567 | 0.583 | 70.656 | 1.00x |
| nested.json | orjson | 0.609 | 0.611 | 0.616 | 70.656 | 0.93x |
| nested.json | msgspec | 0.607 | 0.610 | 0.620 | 70.656 | 0.93x |
| nested.json | ujson | 0.884 | 0.889 | 0.900 | 70.656 | 0.64x |
| nested.json | json | 1.394 | 1.404 | 1.428 | 70.656 | 0.40x |
| wide_arrays.json | strata | 3.055 | 3.073 | 3.356 | 70.766 | 1.00x |
| wide_arrays.json | orjson | 3.148 | 3.168 | 3.711 | 70.766 | 0.97x |
| wide_arrays.json | msgspec | 3.871 | 3.899 | 3.922 | 70.766 | 0.79x |
| wide_arrays.json | ujson | 5.079 | 5.100 | 5.179 | 70.766 | 0.60x |
| wide_arrays.json | json | 7.432 | 7.515 | 7.642 | 70.766 | 0.41x |
| mixed.json | strata | 0.133 | 0.134 | 0.141 | 70.906 | 1.00x |
| mixed.json | orjson | 0.125 | 0.129 | 0.152 | 70.906 | 1.04x |
| mixed.json | msgspec | 0.138 | 0.140 | 0.143 | 70.906 | 0.96x |
| mixed.json | ujson | 0.179 | 0.182 | 0.185 | 70.906 | 0.74x |
| mixed.json | json | 0.303 | 0.304 | 0.308 | 70.906 | 0.44x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.635 | 1.650 | 1.715 | 55.562 | 1.00x |
| users.json | orjson | 1.980 | 1.995 | 2.229 | 55.562 | 0.83x |
| users.json | msgspec | 2.634 | 2.651 | 2.778 | 55.562 | 0.62x |
| users.json | ujson | 10.863 | 10.922 | 11.241 | 55.562 | 0.15x |
| users.json | json | 15.444 | 15.525 | 17.187 | 55.562 | 0.11x |
| flat.json | strata | 0.208 | 0.210 | 0.226 | 70.641 | 1.00x |
| flat.json | orjson | 0.222 | 0.224 | 0.227 | 70.641 | 0.94x |
| flat.json | msgspec | 0.294 | 0.296 | 0.298 | 70.641 | 0.71x |
| flat.json | ujson | 0.900 | 0.903 | 0.922 | 70.641 | 0.23x |
| flat.json | json | 1.335 | 1.347 | 1.365 | 70.641 | 0.16x |
| nested.json | strata | 0.140 | 0.141 | 0.143 | 70.766 | 1.00x |
| nested.json | orjson | 0.197 | 0.199 | 0.201 | 70.766 | 0.71x |
| nested.json | msgspec | 0.268 | 0.270 | 0.279 | 70.766 | 0.52x |
| nested.json | ujson | 0.949 | 0.954 | 0.963 | 70.766 | 0.15x |
| nested.json | json | 1.605 | 1.615 | 1.646 | 70.766 | 0.09x |
| wide_arrays.json | strata | 0.984 | 0.986 | 1.018 | 70.766 | 1.00x |
| wide_arrays.json | orjson | 1.090 | 1.100 | 1.126 | 70.766 | 0.90x |
| wide_arrays.json | msgspec | 1.905 | 1.913 | 1.935 | 70.766 | 0.52x |
| wide_arrays.json | ujson | 5.944 | 5.972 | 6.102 | 70.766 | 0.17x |
| wide_arrays.json | json | 11.002 | 11.126 | 11.254 | 70.766 | 0.09x |
| mixed.json | strata | 0.040 | 0.040 | 0.043 | 70.984 | 1.00x |
| mixed.json | orjson | 0.042 | 0.043 | 0.045 | 70.984 | 0.95x |
| mixed.json | msgspec | 0.048 | 0.051 | 0.057 | 70.984 | 0.79x |
| mixed.json | ujson | 0.217 | 0.218 | 0.222 | 70.984 | 0.19x |
| mixed.json | json | 0.351 | 0.356 | 0.373 | 70.984 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.146 | 7.171 | 7.549 | 67.188 | 1.00x |
| users.json | orjson | 8.147 | 8.206 | 8.257 | 67.188 | 0.87x |
| users.json | msgspec | 8.252 | 8.348 | 8.668 | 67.188 | 0.86x |
| users.json | ujson | 11.190 | 11.408 | 12.197 | 67.188 | 0.63x |
| users.json | json | 15.247 | 15.335 | 15.754 | 67.188 | 0.47x |
| flat.json | strata | 0.690 | 0.706 | 0.829 | 70.641 | 1.00x |
| flat.json | orjson | 0.705 | 0.721 | 0.799 | 70.641 | 0.98x |
| flat.json | msgspec | 0.749 | 0.763 | 0.845 | 70.641 | 0.92x |
| flat.json | ujson | 1.108 | 1.123 | 1.196 | 70.641 | 0.63x |
| flat.json | json | 1.489 | 1.508 | 1.582 | 70.641 | 0.47x |
| nested.json | strata | 0.618 | 0.624 | 0.648 | 70.766 | 1.00x |
| nested.json | orjson | 0.669 | 0.679 | 0.718 | 70.766 | 0.92x |
| nested.json | msgspec | 0.669 | 0.676 | 0.687 | 70.766 | 0.92x |
| nested.json | ujson | 0.972 | 0.984 | 0.992 | 70.766 | 0.63x |
| nested.json | json | 1.477 | 1.482 | 1.495 | 70.766 | 0.42x |
| wide_arrays.json | strata | 3.235 | 3.247 | 3.271 | 70.766 | 1.00x |
| wide_arrays.json | orjson | 3.373 | 3.388 | 3.477 | 70.766 | 0.96x |
| wide_arrays.json | msgspec | 4.108 | 4.152 | 4.302 | 70.766 | 0.78x |
| wide_arrays.json | ujson | 5.422 | 5.458 | 5.878 | 70.766 | 0.59x |
| wide_arrays.json | json | 7.639 | 7.694 | 7.852 | 70.766 | 0.42x |
| mixed.json | strata | 0.177 | 0.193 | 0.205 | 70.984 | 1.00x |
| mixed.json | orjson | 0.176 | 0.184 | 0.222 | 70.984 | 1.05x |
| mixed.json | msgspec | 0.188 | 0.210 | 0.243 | 70.984 | 0.92x |
| mixed.json | ujson | 0.242 | 0.262 | 0.275 | 70.984 | 0.74x |
| mixed.json | json | 0.355 | 0.366 | 0.407 | 70.984 | 0.53x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.231 | 7.314 | 7.518 | 69.750 | 1.00x |
| users.ndjson | orjson | 9.887 | 10.046 | 10.329 | 69.750 | 0.73x |
| users.ndjson | msgspec | 9.792 | 9.905 | 10.079 | 69.750 | 0.74x |
| users.ndjson | ujson | 12.652 | 12.823 | 13.056 | 69.750 | 0.57x |
| users.ndjson | json | 17.612 | 17.780 | 18.336 | 69.750 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.981 | 2.097 | 2.726 | 66.766 | 1.00x |
| users.json | orjson | 2.439 | 2.495 | 2.808 | 66.766 | 0.84x |
| users.json | msgspec | 3.023 | 3.132 | 3.545 | 66.766 | 0.67x |
| users.json | ujson | 10.990 | 11.295 | 13.361 | 66.766 | 0.19x |
| users.json | json | 15.962 | 16.039 | 17.130 | 66.766 | 0.13x |
| flat.json | strata | 0.361 | 0.394 | 0.420 | 70.656 | 1.00x |
| flat.json | orjson | 0.381 | 0.407 | 0.472 | 70.656 | 0.97x |
| flat.json | msgspec | 0.460 | 0.473 | 1.501 | 70.656 | 0.83x |
| flat.json | ujson | 1.070 | 1.107 | 1.122 | 70.656 | 0.36x |
| flat.json | json | 1.524 | 1.544 | 1.633 | 70.656 | 0.26x |
| nested.json | strata | 0.298 | 0.344 | 0.374 | 70.766 | 1.00x |
| nested.json | orjson | 0.373 | 0.386 | 0.424 | 70.766 | 0.89x |
| nested.json | msgspec | 0.426 | 0.457 | 0.496 | 70.766 | 0.75x |
| nested.json | ujson | 1.140 | 1.171 | 1.562 | 70.766 | 0.29x |
| nested.json | json | 1.810 | 1.847 | 1.893 | 70.766 | 0.19x |
| wide_arrays.json | strata | 1.288 | 1.336 | 1.432 | 70.906 | 1.00x |
| wide_arrays.json | orjson | 1.416 | 1.475 | 3.173 | 70.906 | 0.91x |
| wide_arrays.json | msgspec | 2.254 | 2.296 | 2.472 | 70.906 | 0.58x |
| wide_arrays.json | ujson | 6.336 | 6.415 | 6.742 | 70.906 | 0.21x |
| wide_arrays.json | json | 11.381 | 11.512 | 11.958 | 70.906 | 0.12x |
| mixed.json | strata | 0.170 | 0.201 | 0.431 | 71.016 | 1.00x |
| mixed.json | orjson | 0.181 | 0.206 | 0.286 | 71.016 | 0.98x |
| mixed.json | msgspec | 0.182 | 0.212 | 0.327 | 71.016 | 0.95x |
| mixed.json | ujson | 0.353 | 0.391 | 0.501 | 71.016 | 0.52x |
| mixed.json | json | 0.486 | 0.543 | 0.930 | 71.016 | 0.37x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.050 | 0.054 | 0.071 | 66.875 | 1.00x |
| users.json $[*].id | jmespath | 0.272 | 0.288 | 0.299 | 66.875 | 0.19x |
| users.json $[*].id | jsonpath-ng | 1.425 | 1.451 | 1.495 | 66.875 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.321 | 0.335 | 0.377 | 68.781 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.643 | 1.658 | 1.722 | 68.781 | 0.20x |
| users.json $[*].orders[*].total | jsonpath-ng | 9.517 | 9.601 | 10.017 | 68.781 | 0.03x |
| users.json $..total | strata | 1.386 | 1.402 | 1.414 | 69.844 | 1.00x |
| users.json $..total | jsonpath-ng | 183.847 | 184.975 | 188.882 | 69.844 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.375 | 3.453 | 3.679 | 68.609 | 1.00x |
| users.json $[*].id | orjson+jmespath | 8.368 | 8.423 | 8.567 | 68.609 | 0.41x |
| users.json $[*].id | orjson+jsonpath-ng | 9.574 | 9.615 | 9.803 | 68.609 | 0.36x |
| users.json $[*].orders[*].total | strata | 3.430 | 3.479 | 3.538 | 69.797 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 9.627 | 9.778 | 10.101 | 69.797 | 0.36x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 18.791 | 19.076 | 19.776 | 69.797 | 0.18x |
| users.json $..total | strata | 8.498 | 8.599 | 8.721 | 68.953 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 194.265 | 195.208 | 197.444 | 68.953 | 0.04x |

