# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 8135519
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
| users.json | strata | 6.346 | 6.437 | 7.049 | 51.469 | 1.00x |
| users.json | orjson | 7.832 | 7.876 | 8.312 | 51.469 | 0.82x |
| users.json | msgspec | 7.908 | 7.958 | 8.495 | 51.469 | 0.81x |
| users.json | ujson | 10.764 | 10.880 | 11.779 | 51.469 | 0.59x |
| users.json | json | 14.903 | 15.098 | 16.080 | 51.469 | 0.43x |
| flat.json | strata | 0.627 | 0.630 | 0.661 | 67.672 | 1.00x |
| flat.json | orjson | 0.665 | 0.666 | 0.680 | 67.672 | 0.95x |
| flat.json | msgspec | 0.686 | 0.690 | 0.710 | 67.672 | 0.91x |
| flat.json | ujson | 1.027 | 1.031 | 1.042 | 67.672 | 0.61x |
| flat.json | json | 1.429 | 1.436 | 1.443 | 67.672 | 0.44x |
| nested.json | strata | 0.529 | 0.532 | 0.537 | 67.672 | 1.00x |
| nested.json | orjson | 0.600 | 0.602 | 0.668 | 67.672 | 0.88x |
| nested.json | msgspec | 0.602 | 0.605 | 0.620 | 67.672 | 0.88x |
| nested.json | ujson | 0.865 | 0.868 | 0.882 | 67.672 | 0.61x |
| nested.json | json | 1.397 | 1.405 | 1.413 | 67.672 | 0.38x |
| wide_arrays.json | strata | 2.965 | 2.972 | 2.992 | 67.797 | 1.00x |
| wide_arrays.json | orjson | 3.159 | 3.162 | 3.184 | 67.797 | 0.94x |
| wide_arrays.json | msgspec | 3.884 | 3.901 | 4.010 | 67.797 | 0.76x |
| wide_arrays.json | ujson | 5.067 | 5.086 | 5.102 | 67.797 | 0.58x |
| wide_arrays.json | json | 7.478 | 7.496 | 7.535 | 67.797 | 0.40x |
| mixed.json | strata | 0.126 | 0.127 | 0.136 | 68.797 | 1.00x |
| mixed.json | orjson | 0.125 | 0.126 | 0.128 | 68.797 | 1.00x |
| mixed.json | msgspec | 0.138 | 0.139 | 0.146 | 68.797 | 0.91x |
| mixed.json | ujson | 0.176 | 0.179 | 0.185 | 68.797 | 0.71x |
| mixed.json | json | 0.302 | 0.304 | 0.309 | 68.797 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.426 | 1.434 | 1.438 | 54.000 | 1.00x |
| users.json | orjson | 1.997 | 2.007 | 2.014 | 54.000 | 0.71x |
| users.json | msgspec | 2.613 | 2.627 | 2.712 | 54.000 | 0.55x |
| users.json | ujson | 10.778 | 10.807 | 10.939 | 54.000 | 0.13x |
| users.json | json | 15.401 | 15.450 | 15.546 | 54.000 | 0.09x |
| flat.json | strata | 0.176 | 0.179 | 0.185 | 67.672 | 1.00x |
| flat.json | orjson | 0.222 | 0.224 | 0.229 | 67.672 | 0.80x |
| flat.json | msgspec | 0.292 | 0.293 | 0.304 | 67.672 | 0.61x |
| flat.json | ujson | 0.896 | 0.898 | 0.923 | 67.672 | 0.20x |
| flat.json | json | 1.316 | 1.322 | 1.351 | 67.672 | 0.14x |
| nested.json | strata | 0.134 | 0.138 | 0.144 | 67.781 | 1.00x |
| nested.json | orjson | 0.198 | 0.200 | 0.202 | 67.781 | 0.69x |
| nested.json | msgspec | 0.268 | 0.268 | 0.272 | 67.781 | 0.51x |
| nested.json | ujson | 0.952 | 0.956 | 0.966 | 67.781 | 0.14x |
| nested.json | json | 1.608 | 1.618 | 1.642 | 67.781 | 0.09x |
| wide_arrays.json | strata | 0.913 | 0.921 | 0.953 | 67.797 | 1.00x |
| wide_arrays.json | orjson | 1.103 | 1.116 | 1.146 | 67.797 | 0.82x |
| wide_arrays.json | msgspec | 1.934 | 1.945 | 1.961 | 67.797 | 0.47x |
| wide_arrays.json | ujson | 5.955 | 6.005 | 6.114 | 67.797 | 0.15x |
| wide_arrays.json | json | 10.995 | 11.079 | 11.650 | 67.797 | 0.08x |
| mixed.json | strata | 0.035 | 0.036 | 0.036 | 68.875 | 1.00x |
| mixed.json | orjson | 0.041 | 0.042 | 0.044 | 68.875 | 0.84x |
| mixed.json | msgspec | 0.048 | 0.050 | 0.057 | 68.875 | 0.72x |
| mixed.json | ujson | 0.212 | 0.214 | 0.221 | 68.875 | 0.17x |
| mixed.json | json | 0.348 | 0.350 | 0.355 | 68.875 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.568 | 6.653 | 6.958 | 65.609 | 1.00x |
| users.json | orjson | 8.143 | 8.223 | 8.387 | 65.609 | 0.81x |
| users.json | msgspec | 8.182 | 8.308 | 8.747 | 65.609 | 0.80x |
| users.json | ujson | 11.167 | 11.357 | 11.771 | 65.609 | 0.59x |
| users.json | json | 15.219 | 15.380 | 15.899 | 65.609 | 0.43x |
| flat.json | strata | 0.711 | 0.718 | 0.750 | 67.672 | 1.00x |
| flat.json | orjson | 0.749 | 0.763 | 0.768 | 67.672 | 0.94x |
| flat.json | msgspec | 0.758 | 0.779 | 0.828 | 67.672 | 0.92x |
| flat.json | ujson | 1.150 | 1.167 | 1.207 | 67.672 | 0.62x |
| flat.json | json | 1.516 | 1.537 | 1.581 | 67.672 | 0.47x |
| nested.json | strata | 0.590 | 0.595 | 0.647 | 67.781 | 1.00x |
| nested.json | orjson | 0.664 | 0.669 | 0.769 | 67.781 | 0.89x |
| nested.json | msgspec | 0.667 | 0.670 | 0.738 | 67.781 | 0.89x |
| nested.json | ujson | 0.954 | 0.964 | 0.997 | 67.781 | 0.62x |
| nested.json | json | 1.467 | 1.473 | 1.500 | 67.781 | 0.40x |
| wide_arrays.json | strata | 3.134 | 3.155 | 3.469 | 68.797 | 1.00x |
| wide_arrays.json | orjson | 3.315 | 3.388 | 3.647 | 68.797 | 0.93x |
| wide_arrays.json | msgspec | 4.075 | 4.140 | 4.383 | 68.797 | 0.76x |
| wide_arrays.json | ujson | 5.383 | 5.447 | 5.662 | 68.797 | 0.58x |
| wide_arrays.json | json | 7.690 | 7.795 | 8.228 | 68.797 | 0.40x |
| mixed.json | strata | 0.165 | 0.168 | 0.174 | 68.875 | 1.00x |
| mixed.json | orjson | 0.170 | 0.172 | 0.177 | 68.875 | 0.97x |
| mixed.json | msgspec | 0.185 | 0.186 | 0.190 | 68.875 | 0.90x |
| mixed.json | ujson | 0.235 | 0.238 | 0.247 | 68.875 | 0.71x |
| mixed.json | json | 0.350 | 0.354 | 0.365 | 68.875 | 0.47x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.642 | 6.693 | 6.885 | 66.812 | 1.00x |
| users.ndjson | orjson | 9.871 | 9.928 | 10.065 | 66.812 | 0.67x |
| users.ndjson | msgspec | 9.756 | 9.791 | 10.169 | 66.812 | 0.68x |
| users.ndjson | ujson | 12.647 | 12.703 | 13.453 | 66.812 | 0.53x |
| users.ndjson | json | 17.509 | 17.781 | 18.657 | 66.812 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.747 | 1.790 | 1.919 | 64.109 | 1.00x |
| users.json | orjson | 2.349 | 2.436 | 2.504 | 64.109 | 0.73x |
| users.json | msgspec | 2.962 | 3.033 | 3.164 | 64.109 | 0.59x |
| users.json | ujson | 10.904 | 10.987 | 11.102 | 64.109 | 0.16x |
| users.json | json | 15.893 | 16.158 | 16.339 | 64.109 | 0.11x |
| flat.json | strata | 0.328 | 0.354 | 0.470 | 67.672 | 1.00x |
| flat.json | orjson | 0.384 | 0.402 | 0.472 | 67.672 | 0.88x |
| flat.json | msgspec | 0.440 | 0.471 | 0.492 | 67.672 | 0.75x |
| flat.json | ujson | 1.062 | 1.085 | 1.260 | 67.672 | 0.33x |
| flat.json | json | 1.498 | 1.527 | 1.692 | 67.672 | 0.23x |
| nested.json | strata | 0.267 | 0.277 | 0.318 | 67.797 | 1.00x |
| nested.json | orjson | 0.344 | 0.353 | 0.379 | 67.797 | 0.79x |
| nested.json | msgspec | 0.396 | 0.411 | 0.431 | 67.797 | 0.67x |
| nested.json | ujson | 1.095 | 1.100 | 1.114 | 67.797 | 0.25x |
| nested.json | json | 1.759 | 1.804 | 1.915 | 67.797 | 0.15x |
| wide_arrays.json | strata | 1.187 | 1.201 | 1.247 | 68.797 | 1.00x |
| wide_arrays.json | orjson | 1.396 | 1.417 | 1.431 | 68.797 | 0.85x |
| wide_arrays.json | msgspec | 2.208 | 2.234 | 2.343 | 68.797 | 0.54x |
| wide_arrays.json | ujson | 6.291 | 6.317 | 6.409 | 68.797 | 0.19x |
| wide_arrays.json | json | 11.320 | 11.349 | 11.395 | 68.797 | 0.11x |
| mixed.json | strata | 0.148 | 0.157 | 0.174 | 68.906 | 1.00x |
| mixed.json | orjson | 0.155 | 0.158 | 0.183 | 68.906 | 1.00x |
| mixed.json | msgspec | 0.161 | 0.165 | 0.193 | 68.906 | 0.96x |
| mixed.json | ujson | 0.332 | 0.338 | 0.374 | 68.906 | 0.47x |
| mixed.json | json | 0.474 | 0.480 | 0.505 | 68.906 | 0.33x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.048 | 0.049 | 0.066 | 64.281 | 1.00x |
| users.json $[*].id | jmespath | 0.270 | 0.274 | 0.286 | 64.281 | 0.18x |
| users.json $[*].id | jsonpath-ng | 1.427 | 1.437 | 1.571 | 64.281 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.321 | 0.326 | 0.372 | 64.797 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.645 | 1.660 | 1.797 | 64.797 | 0.20x |
| users.json $[*].orders[*].total | jsonpath-ng | 9.379 | 9.443 | 10.417 | 64.797 | 0.03x |
| users.json $..total | strata | 1.386 | 1.404 | 1.464 | 66.875 | 1.00x |
| users.json $..total | jsonpath-ng | 183.556 | 184.240 | 187.320 | 66.875 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.355 | 3.362 | 3.566 | 64.500 | 1.00x |
| users.json $[*].id | orjson+jmespath | 8.291 | 8.353 | 8.629 | 64.500 | 0.40x |
| users.json $[*].id | orjson+jsonpath-ng | 9.498 | 9.555 | 9.757 | 64.500 | 0.35x |
| users.json $[*].orders[*].total | strata | 3.411 | 3.433 | 3.523 | 66.812 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 9.715 | 9.745 | 9.939 | 66.812 | 0.35x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 18.759 | 18.824 | 19.378 | 66.812 | 0.18x |
| users.json $..total | strata | 7.988 | 8.044 | 8.669 | 66.000 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 193.122 | 194.970 | 196.680 | 66.000 | 0.04x |

