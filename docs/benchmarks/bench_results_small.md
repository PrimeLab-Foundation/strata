# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: dc84475
- python: 3.14.3
- implementation: CPython
- platform: macOS-26.3-arm64-arm-64bit-Mach-O
- machine: arm64
- processor: Apple M1 Max
- compiler_flags: -std=c++20 -O3 -march=native
- repeats: 30
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.706 | 6.796 | 7.056 | 52.875 | 1.00x |
| users.json | orjson | 8.273 | 8.453 | 8.998 | 52.875 | 0.80x |
| users.json | msgspec | 8.338 | 8.471 | 9.035 | 52.875 | 0.80x |
| users.json | ujson | 11.055 | 11.285 | 11.989 | 52.875 | 0.60x |
| users.json | json | 14.738 | 14.964 | 15.670 | 52.875 | 0.45x |
| flat.json | strata | 0.618 | 0.623 | 0.695 | 67.656 | 1.00x |
| flat.json | orjson | 0.625 | 0.633 | 0.672 | 67.656 | 0.98x |
| flat.json | msgspec | 0.664 | 0.679 | 0.735 | 67.656 | 0.92x |
| flat.json | ujson | 0.980 | 0.994 | 1.097 | 67.656 | 0.63x |
| flat.json | json | 1.358 | 1.370 | 1.496 | 67.656 | 0.45x |
| nested.json | strata | 0.524 | 0.533 | 0.557 | 67.719 | 1.00x |
| nested.json | orjson | 1.194 | 1.213 | 1.534 | 67.719 | 0.44x |
| nested.json | msgspec | 1.188 | 1.222 | 1.517 | 67.719 | 0.44x |
| nested.json | ujson | 1.461 | 1.551 | 1.851 | 67.719 | 0.34x |
| nested.json | json | 1.906 | 1.961 | 2.242 | 67.719 | 0.27x |
| wide_arrays.json | strata | 3.232 | 3.300 | 3.638 | 67.734 | 1.00x |
| wide_arrays.json | orjson | 3.785 | 3.891 | 4.193 | 67.734 | 0.85x |
| wide_arrays.json | msgspec | 4.437 | 4.514 | 4.937 | 67.734 | 0.73x |
| wide_arrays.json | ujson | 5.618 | 5.732 | 6.147 | 67.734 | 0.58x |
| wide_arrays.json | json | 7.906 | 8.061 | 8.588 | 67.734 | 0.41x |
| mixed.json | strata | 0.128 | 0.133 | 0.159 | 68.266 | 1.00x |
| mixed.json | orjson | 0.125 | 0.131 | 0.147 | 68.266 | 1.02x |
| mixed.json | msgspec | 0.137 | 0.142 | 0.170 | 68.266 | 0.94x |
| mixed.json | ujson | 0.181 | 0.187 | 0.199 | 68.266 | 0.71x |
| mixed.json | json | 0.295 | 0.306 | 0.355 | 68.266 | 0.44x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.682 | 1.698 | 1.775 | 56.844 | 1.00x |
| users.json | orjson | 1.970 | 1.988 | 2.048 | 56.844 | 0.85x |
| users.json | msgspec | 2.582 | 2.606 | 2.704 | 56.844 | 0.65x |
| users.json | ujson | 10.649 | 10.749 | 11.172 | 56.844 | 0.16x |
| users.json | json | 14.804 | 14.936 | 15.346 | 56.844 | 0.11x |
| flat.json | strata | 0.182 | 0.185 | 0.210 | 67.719 | 1.00x |
| flat.json | orjson | 0.220 | 0.225 | 0.248 | 67.719 | 0.82x |
| flat.json | msgspec | 0.290 | 0.295 | 0.324 | 67.719 | 0.63x |
| flat.json | ujson | 0.854 | 0.862 | 0.906 | 67.719 | 0.21x |
| flat.json | json | 1.232 | 1.245 | 1.359 | 67.719 | 0.15x |
| nested.json | strata | 0.139 | 0.144 | 0.159 | 67.719 | 1.00x |
| nested.json | orjson | 0.198 | 0.203 | 0.219 | 67.719 | 0.71x |
| nested.json | msgspec | 0.267 | 0.272 | 0.289 | 67.719 | 0.53x |
| nested.json | ujson | 0.956 | 0.968 | 1.007 | 67.719 | 0.15x |
| nested.json | json | 1.566 | 1.587 | 1.643 | 67.719 | 0.09x |
| wide_arrays.json | strata | 1.081 | 1.100 | 1.198 | 68.266 | 1.00x |
| wide_arrays.json | orjson | 1.106 | 1.127 | 1.266 | 68.266 | 0.98x |
| wide_arrays.json | msgspec | 1.925 | 1.949 | 2.063 | 68.266 | 0.56x |
| wide_arrays.json | ujson | 5.940 | 5.991 | 6.327 | 68.266 | 0.18x |
| wide_arrays.json | json | 10.672 | 10.802 | 11.155 | 68.266 | 0.10x |
| mixed.json | strata | 0.040 | 0.043 | 0.051 | 68.266 | 1.00x |
| mixed.json | orjson | 0.042 | 0.043 | 0.050 | 68.266 | 1.00x |
| mixed.json | msgspec | 0.049 | 0.053 | 0.057 | 68.266 | 0.82x |
| mixed.json | ujson | 0.210 | 0.215 | 0.230 | 68.266 | 0.20x |
| mixed.json | json | 0.330 | 0.336 | 0.420 | 68.266 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.889 | 7.151 | 7.560 | 70.484 | 1.00x |
| users.json | orjson | 10.280 | 10.634 | 11.538 | 70.484 | 0.67x |
| users.json | msgspec | 10.394 | 10.654 | 11.687 | 70.484 | 0.67x |
| users.json | ujson | 13.367 | 13.703 | 14.474 | 70.484 | 0.52x |
| users.json | json | 16.810 | 17.297 | 18.277 | 70.484 | 0.41x |
| flat.json | strata | 0.678 | 0.699 | 0.778 | 67.719 | 1.00x |
| flat.json | orjson | 0.703 | 0.731 | 0.799 | 67.719 | 0.96x |
| flat.json | msgspec | 0.739 | 0.776 | 0.830 | 67.719 | 0.90x |
| flat.json | ujson | 1.088 | 1.109 | 1.189 | 67.719 | 0.63x |
| flat.json | json | 1.434 | 1.452 | 1.590 | 67.719 | 0.48x |
| nested.json | strata | 0.592 | 0.607 | 0.666 | 67.719 | 1.00x |
| nested.json | orjson | 1.373 | 1.401 | 1.508 | 67.719 | 0.43x |
| nested.json | msgspec | 1.371 | 1.413 | 1.596 | 67.719 | 0.43x |
| nested.json | ujson | 1.670 | 1.729 | 1.917 | 67.719 | 0.35x |
| nested.json | json | 2.085 | 2.135 | 2.307 | 67.719 | 0.28x |
| wide_arrays.json | strata | 3.384 | 3.438 | 3.683 | 68.266 | 1.00x |
| wide_arrays.json | orjson | 4.244 | 4.355 | 4.626 | 68.266 | 0.79x |
| wide_arrays.json | msgspec | 4.967 | 5.071 | 5.498 | 68.266 | 0.68x |
| wide_arrays.json | ujson | 6.263 | 6.382 | 6.844 | 68.266 | 0.54x |
| wide_arrays.json | json | 8.502 | 8.600 | 9.022 | 68.266 | 0.40x |
| mixed.json | strata | 0.168 | 0.181 | 0.230 | 68.266 | 1.00x |
| mixed.json | orjson | 0.169 | 0.180 | 0.216 | 68.266 | 1.01x |
| mixed.json | msgspec | 0.180 | 0.194 | 0.256 | 68.266 | 0.93x |
| mixed.json | ujson | 0.239 | 0.250 | 0.285 | 68.266 | 0.72x |
| mixed.json | json | 0.339 | 0.351 | 0.413 | 68.266 | 0.52x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.133 | 7.294 | 7.552 | 66.797 | 1.00x |
| users.ndjson | orjson | 10.422 | 10.627 | 11.026 | 66.797 | 0.69x |
| users.ndjson | msgspec | 10.154 | 10.399 | 10.822 | 66.797 | 0.70x |
| users.ndjson | ujson | 13.006 | 13.202 | 14.139 | 66.797 | 0.55x |
| users.ndjson | json | 17.914 | 18.244 | 19.881 | 66.797 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.028 | 2.105 | 2.219 | 67.516 | 1.00x |
| users.json | orjson | 2.354 | 2.413 | 2.512 | 67.516 | 0.87x |
| users.json | msgspec | 2.939 | 3.015 | 3.189 | 67.516 | 0.70x |
| users.json | ujson | 10.848 | 10.958 | 11.369 | 67.516 | 0.19x |
| users.json | json | 15.299 | 15.430 | 15.911 | 67.516 | 0.14x |
| flat.json | strata | 0.308 | 0.327 | 0.441 | 67.719 | 1.00x |
| flat.json | orjson | 0.355 | 0.377 | 0.441 | 67.719 | 0.87x |
| flat.json | msgspec | 0.418 | 0.440 | 0.465 | 67.719 | 0.74x |
| flat.json | ujson | 0.999 | 1.033 | 1.104 | 67.719 | 0.32x |
| flat.json | json | 1.383 | 1.417 | 1.512 | 67.719 | 0.23x |
| nested.json | strata | 0.264 | 0.282 | 0.336 | 67.719 | 1.00x |
| nested.json | orjson | 0.322 | 0.345 | 0.400 | 67.719 | 0.82x |
| nested.json | msgspec | 0.393 | 0.419 | 0.466 | 67.719 | 0.67x |
| nested.json | ujson | 1.138 | 1.182 | 1.389 | 67.719 | 0.24x |
| nested.json | json | 1.705 | 1.738 | 1.897 | 67.719 | 0.16x |
| wide_arrays.json | strata | 1.354 | 1.415 | 1.540 | 68.266 | 1.00x |
| wide_arrays.json | orjson | 1.409 | 1.464 | 1.571 | 68.266 | 0.97x |
| wide_arrays.json | msgspec | 2.200 | 2.273 | 2.451 | 68.266 | 0.62x |
| wide_arrays.json | ujson | 6.302 | 6.383 | 6.672 | 68.266 | 0.22x |
| wide_arrays.json | json | 10.966 | 11.166 | 11.421 | 68.266 | 0.13x |
| mixed.json | strata | 0.137 | 0.158 | 0.239 | 63.047 | 1.00x |
| mixed.json | orjson | 0.142 | 0.179 | 0.228 | 63.047 | 0.89x |
| mixed.json | msgspec | 0.151 | 0.175 | 0.261 | 63.047 | 0.90x |
| mixed.json | ujson | 0.321 | 0.343 | 0.388 | 63.047 | 0.46x |
| mixed.json | json | 0.441 | 0.479 | 0.563 | 63.047 | 0.33x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.050 | 0.056 | 0.074 | 67.812 | 1.00x |
| users.json $[*].id | jmespath | 0.332 | 0.343 | 0.407 | 67.812 | 0.16x |
| users.json $[*].id | jsonpath-ng | 3.369 | 3.443 | 3.989 | 67.812 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.348 | 0.363 | 0.428 | 68.141 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.059 | 2.084 | 2.434 | 68.141 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 11.136 | 11.381 | 12.039 | 68.141 | 0.03x |
| users.json $..total | strata | 1.353 | 1.379 | 1.478 | 62.688 | 1.00x |
| users.json $..total | jsonpath-ng | 206.779 | 208.681 | 215.674 | 62.688 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.170 | 3.237 | 3.399 | 67.875 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.763 | 10.955 | 11.925 | 67.875 | 0.30x |
| users.json $[*].id | orjson+jsonpath-ng | 11.930 | 12.229 | 12.938 | 67.875 | 0.26x |
| users.json $[*].orders[*].total | strata | 3.283 | 3.387 | 3.510 | 69.375 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.640 | 12.959 | 13.705 | 69.375 | 0.26x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 19.851 | 20.356 | 22.422 | 69.375 | 0.17x |
| users.json $..total | strata | 10.563 | 11.038 | 11.852 | 67.922 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 216.020 | 217.060 | 220.538 | 67.922 | 0.05x |

