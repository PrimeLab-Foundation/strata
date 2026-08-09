# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 62edf69
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
| users.json | strata | 10.022 | 10.185 | 10.523 | 52.688 | 1.00x |
| users.json | orjson | 10.775 | 10.923 | 11.391 | 52.688 | 0.93x |
| users.json | msgspec | 10.888 | 11.024 | 11.594 | 52.688 | 0.92x |
| users.json | ujson | 14.384 | 14.502 | 15.676 | 52.688 | 0.70x |
| users.json | json | 19.092 | 19.354 | 19.992 | 52.688 | 0.53x |
| flat.json | strata | 0.977 | 0.989 | 1.035 | 70.672 | 1.00x |
| flat.json | orjson | 0.860 | 0.866 | 0.899 | 70.672 | 1.14x |
| flat.json | msgspec | 0.891 | 0.895 | 0.902 | 70.672 | 1.11x |
| flat.json | ujson | 1.295 | 1.302 | 1.312 | 70.672 | 0.76x |
| flat.json | json | 1.841 | 1.849 | 1.882 | 70.672 | 0.53x |
| nested.json | strata | 0.796 | 0.804 | 0.815 | 70.734 | 1.00x |
| nested.json | orjson | 1.525 | 1.550 | 1.741 | 70.734 | 0.52x |
| nested.json | msgspec | 1.533 | 1.566 | 1.897 | 70.734 | 0.51x |
| nested.json | ujson | 1.882 | 1.916 | 2.025 | 70.734 | 0.42x |
| nested.json | json | 2.475 | 2.508 | 3.035 | 70.734 | 0.32x |
| wide_arrays.json | strata | 4.704 | 4.735 | 5.155 | 70.750 | 1.00x |
| wide_arrays.json | orjson | 4.859 | 4.919 | 5.350 | 70.750 | 0.96x |
| wide_arrays.json | msgspec | 5.765 | 5.883 | 6.515 | 70.750 | 0.80x |
| wide_arrays.json | ujson | 7.330 | 7.489 | 7.976 | 70.750 | 0.63x |
| wide_arrays.json | json | 10.324 | 10.442 | 11.260 | 70.750 | 0.45x |
| mixed.json | strata | 0.205 | 0.209 | 0.217 | 70.750 | 1.00x |
| mixed.json | orjson | 0.164 | 0.167 | 0.191 | 70.750 | 1.25x |
| mixed.json | msgspec | 0.182 | 0.187 | 0.203 | 70.750 | 1.12x |
| mixed.json | ujson | 0.235 | 0.238 | 0.258 | 70.750 | 0.88x |
| mixed.json | json | 0.389 | 0.396 | 0.400 | 70.750 | 0.53x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.065 | 3.106 | 3.222 | 58.594 | 1.00x |
| users.json | orjson | 2.572 | 2.594 | 2.666 | 58.594 | 1.20x |
| users.json | msgspec | 3.398 | 3.414 | 3.470 | 58.594 | 0.91x |
| users.json | ujson | 13.812 | 13.891 | 13.992 | 58.594 | 0.22x |
| users.json | json | 19.549 | 19.751 | 20.250 | 58.594 | 0.16x |
| flat.json | strata | 0.342 | 0.346 | 0.357 | 70.734 | 1.00x |
| flat.json | orjson | 0.291 | 0.294 | 0.325 | 70.734 | 1.18x |
| flat.json | msgspec | 0.384 | 0.385 | 0.388 | 70.734 | 0.90x |
| flat.json | ujson | 1.128 | 1.137 | 1.270 | 70.734 | 0.30x |
| flat.json | json | 1.630 | 1.654 | 1.739 | 70.734 | 0.21x |
| nested.json | strata | 0.268 | 0.278 | 0.321 | 70.750 | 1.00x |
| nested.json | orjson | 0.261 | 0.270 | 0.274 | 70.750 | 1.03x |
| nested.json | msgspec | 0.350 | 0.359 | 0.390 | 70.750 | 0.77x |
| nested.json | ujson | 1.270 | 1.294 | 1.344 | 70.750 | 0.21x |
| nested.json | json | 2.061 | 2.076 | 2.175 | 70.750 | 0.13x |
| wide_arrays.json | strata | 1.936 | 1.950 | 2.113 | 70.750 | 1.00x |
| wide_arrays.json | orjson | 1.456 | 1.474 | 1.587 | 70.750 | 1.32x |
| wide_arrays.json | msgspec | 2.534 | 2.562 | 2.695 | 70.750 | 0.76x |
| wide_arrays.json | ujson | 7.830 | 7.914 | 8.139 | 70.750 | 0.25x |
| wide_arrays.json | json | 14.103 | 14.225 | 14.444 | 70.750 | 0.14x |
| mixed.json | strata | 0.066 | 0.070 | 0.088 | 70.750 | 1.00x |
| mixed.json | orjson | 0.056 | 0.060 | 0.095 | 70.750 | 1.17x |
| mixed.json | msgspec | 0.064 | 0.069 | 0.081 | 70.750 | 1.02x |
| mixed.json | ujson | 0.278 | 0.284 | 0.291 | 70.750 | 0.25x |
| mixed.json | json | 0.443 | 0.452 | 0.487 | 70.750 | 0.16x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.399 | 10.511 | 10.952 | 72.047 | 1.00x |
| users.json | orjson | 13.152 | 13.352 | 14.076 | 72.047 | 0.79x |
| users.json | msgspec | 13.307 | 13.602 | 14.092 | 72.047 | 0.77x |
| users.json | ujson | 17.014 | 17.872 | 18.568 | 72.047 | 0.59x |
| users.json | json | 21.508 | 21.719 | 22.525 | 72.047 | 0.48x |
| flat.json | strata | 1.062 | 1.083 | 1.190 | 70.734 | 1.00x |
| flat.json | orjson | 0.948 | 0.979 | 1.085 | 70.734 | 1.11x |
| flat.json | msgspec | 0.962 | 1.017 | 1.299 | 70.734 | 1.06x |
| flat.json | ujson | 1.427 | 1.499 | 1.584 | 70.734 | 0.72x |
| flat.json | json | 1.923 | 1.955 | 2.009 | 70.734 | 0.55x |
| nested.json | strata | 0.849 | 0.862 | 0.963 | 70.750 | 1.00x |
| nested.json | orjson | 1.749 | 1.778 | 2.148 | 70.750 | 0.49x |
| nested.json | msgspec | 1.736 | 1.773 | 1.879 | 70.750 | 0.49x |
| nested.json | ujson | 2.130 | 2.177 | 2.435 | 70.750 | 0.40x |
| nested.json | json | 2.700 | 2.767 | 2.937 | 70.750 | 0.31x |
| wide_arrays.json | strata | 4.861 | 4.898 | 5.128 | 70.750 | 1.00x |
| wide_arrays.json | orjson | 5.424 | 5.448 | 5.728 | 70.750 | 0.90x |
| wide_arrays.json | msgspec | 6.416 | 6.485 | 6.604 | 70.750 | 0.76x |
| wide_arrays.json | ujson | 8.089 | 8.168 | 8.298 | 70.750 | 0.60x |
| wide_arrays.json | json | 10.986 | 11.055 | 11.729 | 70.750 | 0.44x |
| mixed.json | strata | 0.266 | 0.278 | 0.334 | 70.797 | 1.00x |
| mixed.json | orjson | 0.227 | 0.241 | 0.292 | 70.797 | 1.15x |
| mixed.json | msgspec | 0.239 | 0.256 | 0.279 | 70.797 | 1.09x |
| mixed.json | ujson | 0.316 | 0.341 | 0.412 | 70.797 | 0.82x |
| mixed.json | json | 0.453 | 0.476 | 0.534 | 70.797 | 0.59x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.937 | 10.009 | 10.200 | 69.812 | 1.00x |
| users.ndjson | orjson | 13.460 | 13.726 | 14.709 | 69.812 | 0.73x |
| users.ndjson | msgspec | 13.176 | 13.387 | 13.684 | 69.812 | 0.75x |
| users.ndjson | ujson | 16.862 | 17.099 | 17.732 | 69.812 | 0.59x |
| users.ndjson | json | 23.358 | 23.591 | 24.355 | 69.812 | 0.42x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.541 | 3.594 | 3.649 | 68.141 | 1.00x |
| users.json | orjson | 2.999 | 3.051 | 3.092 | 68.141 | 1.18x |
| users.json | msgspec | 3.807 | 3.839 | 4.117 | 68.141 | 0.94x |
| users.json | ujson | 14.057 | 14.240 | 14.632 | 68.141 | 0.25x |
| users.json | json | 20.063 | 20.207 | 20.758 | 68.141 | 0.18x |
| flat.json | strata | 0.506 | 0.518 | 0.572 | 70.734 | 1.00x |
| flat.json | orjson | 0.439 | 0.457 | 0.499 | 70.734 | 1.13x |
| flat.json | msgspec | 0.520 | 0.541 | 0.586 | 70.734 | 0.96x |
| flat.json | ujson | 1.282 | 1.298 | 1.344 | 70.734 | 0.40x |
| flat.json | json | 1.785 | 1.817 | 2.001 | 70.734 | 0.29x |
| nested.json | strata | 0.440 | 0.484 | 0.544 | 70.750 | 1.00x |
| nested.json | orjson | 0.408 | 0.431 | 0.459 | 70.750 | 1.12x |
| nested.json | msgspec | 0.495 | 0.545 | 0.669 | 70.750 | 0.89x |
| nested.json | ujson | 1.520 | 1.556 | 1.826 | 70.750 | 0.31x |
| nested.json | json | 2.231 | 2.295 | 2.411 | 70.750 | 0.21x |
| wide_arrays.json | strata | 2.254 | 2.387 | 2.631 | 70.750 | 1.00x |
| wide_arrays.json | orjson | 1.743 | 1.815 | 2.254 | 70.750 | 1.32x |
| wide_arrays.json | msgspec | 2.820 | 2.878 | 2.986 | 70.750 | 0.83x |
| wide_arrays.json | ujson | 8.207 | 8.260 | 8.523 | 70.750 | 0.29x |
| wide_arrays.json | json | 14.479 | 14.595 | 15.214 | 70.750 | 0.16x |
| mixed.json | strata | 0.208 | 0.238 | 0.328 | 70.828 | 1.00x |
| mixed.json | orjson | 0.186 | 0.205 | 0.244 | 70.828 | 1.16x |
| mixed.json | msgspec | 0.177 | 0.204 | 0.258 | 70.828 | 1.16x |
| mixed.json | ujson | 0.414 | 0.444 | 0.461 | 70.828 | 0.53x |
| mixed.json | json | 0.572 | 0.612 | 0.699 | 70.828 | 0.39x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.064 | 0.075 | 0.106 | 68.422 | 1.00x |
| users.json $[*].id | jmespath | 0.436 | 0.446 | 0.529 | 68.422 | 0.17x |
| users.json $[*].id | jsonpath-ng | 4.246 | 4.438 | 4.926 | 68.422 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.403 | 0.427 | 0.494 | 68.828 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.666 | 2.731 | 3.033 | 68.828 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 14.547 | 14.817 | 17.010 | 68.828 | 0.03x |
| users.json $..total | strata | 1.771 | 1.785 | 1.801 | 70.953 | 1.00x |
| users.json $..total | jsonpath-ng | 273.198 | 273.882 | 286.048 | 70.953 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.803 | 4.867 | 5.038 | 68.516 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.746 | 14.525 | 14.930 | 68.516 | 0.34x |
| users.json $[*].id | orjson+jsonpath-ng | 15.197 | 15.603 | 16.751 | 68.516 | 0.31x |
| users.json $[*].orders[*].total | strata | 4.927 | 4.986 | 5.109 | 70.891 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.934 | 16.256 | 17.111 | 70.891 | 0.31x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 25.652 | 26.028 | 26.970 | 70.891 | 0.19x |
| users.json $..total | strata | 14.772 | 14.874 | 15.817 | 70.953 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 283.539 | 284.438 | 289.048 | 70.953 | 0.05x |

