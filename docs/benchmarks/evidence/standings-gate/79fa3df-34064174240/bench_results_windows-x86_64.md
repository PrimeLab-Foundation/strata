# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 79fa3df
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD
- compiler_flags: clang-cl /std:c++20 /O2 /arch:AVX2 -fprofile-use (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.650 | 11.932 | 15.541 | 45.793 | 1.00x |
| users.json | orjson | 14.748 | 18.007 | 19.729 | 45.793 | 0.66x |
| users.json | msgspec | 13.088 | 15.574 | 18.621 | 45.793 | 0.77x |
| users.json | ujson | 24.032 | 29.378 | 33.578 | 45.793 | 0.41x |
| users.json | json | 27.170 | 29.186 | 34.436 | 45.793 | 0.41x |
| flat.json | strata | 0.826 | 0.833 | 0.875 | 51.664 | 1.00x |
| flat.json | orjson | 1.137 | 1.185 | 1.212 | 51.664 | 0.70x |
| flat.json | msgspec | 1.085 | 1.137 | 1.152 | 51.664 | 0.73x |
| flat.json | ujson | 2.114 | 2.183 | 2.266 | 51.664 | 0.38x |
| flat.json | json | 2.013 | 2.037 | 2.070 | 51.664 | 0.41x |
| nested.json | strata | 0.775 | 0.816 | 0.877 | 51.309 | 1.00x |
| nested.json | orjson | 1.091 | 1.125 | 1.197 | 51.309 | 0.73x |
| nested.json | msgspec | 0.999 | 1.014 | 1.070 | 51.309 | 0.80x |
| nested.json | ujson | 1.599 | 1.647 | 1.705 | 51.309 | 0.50x |
| nested.json | json | 2.167 | 2.208 | 2.227 | 51.309 | 0.37x |
| wide_arrays.json | strata | 4.153 | 4.352 | 4.929 | 53.414 | 1.00x |
| wide_arrays.json | orjson | 5.943 | 6.173 | 6.672 | 53.414 | 0.70x |
| wide_arrays.json | msgspec | 5.845 | 5.978 | 6.098 | 53.414 | 0.73x |
| wide_arrays.json | ujson | 8.534 | 8.745 | 9.171 | 53.414 | 0.50x |
| wide_arrays.json | json | 11.921 | 12.356 | 15.425 | 53.414 | 0.35x |
| mixed.json | strata | 0.181 | 0.184 | 0.282 | 51.277 | 1.00x |
| mixed.json | orjson | 0.215 | 0.220 | 0.254 | 51.277 | 0.84x |
| mixed.json | msgspec | 0.234 | 0.236 | 0.240 | 51.277 | 0.78x |
| mixed.json | ujson | 0.348 | 0.361 | 0.563 | 51.277 | 0.51x |
| mixed.json | json | 0.471 | 0.485 | 0.931 | 51.277 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.462 | 3.673 | 5.791 | 44.973 | 1.00x |
| users.json | orjson | 4.004 | 4.806 | 6.150 | 44.973 | 0.76x |
| users.json | msgspec | 5.518 | 6.137 | 7.755 | 44.973 | 0.60x |
| users.json | ujson | 15.860 | 17.772 | 23.270 | 44.973 | 0.21x |
| users.json | json | 24.545 | 28.220 | 33.024 | 44.973 | 0.13x |
| flat.json | strata | 0.316 | 0.320 | 0.435 | 51.793 | 1.00x |
| flat.json | orjson | 0.359 | 0.372 | 0.497 | 51.793 | 0.86x |
| flat.json | msgspec | 0.500 | 0.512 | 0.558 | 51.793 | 0.62x |
| flat.json | ujson | 1.545 | 1.580 | 2.303 | 51.793 | 0.20x |
| flat.json | json | 2.013 | 2.029 | 3.463 | 51.793 | 0.16x |
| nested.json | strata | 0.306 | 0.318 | 0.357 | 51.645 | 1.00x |
| nested.json | orjson | 0.320 | 0.327 | 0.380 | 51.645 | 0.97x |
| nested.json | msgspec | 0.470 | 0.485 | 0.538 | 51.645 | 0.66x |
| nested.json | ujson | 1.153 | 1.208 | 1.277 | 51.645 | 0.26x |
| nested.json | json | 2.468 | 2.500 | 2.639 | 51.645 | 0.13x |
| wide_arrays.json | strata | 1.955 | 2.015 | 2.879 | 52.867 | 1.00x |
| wide_arrays.json | orjson | 2.610 | 2.667 | 3.653 | 52.867 | 0.76x |
| wide_arrays.json | msgspec | 4.026 | 4.108 | 5.811 | 52.867 | 0.49x |
| wide_arrays.json | ujson | 7.788 | 7.944 | 12.671 | 52.867 | 0.25x |
| wide_arrays.json | json | 19.184 | 19.355 | 20.278 | 52.867 | 0.10x |
| mixed.json | strata | 0.072 | 0.079 | 0.103 | 51.242 | 1.00x |
| mixed.json | orjson | 0.070 | 0.073 | 0.104 | 51.242 | 1.09x |
| mixed.json | msgspec | 0.097 | 0.101 | 0.133 | 51.242 | 0.79x |
| mixed.json | ujson | 0.268 | 0.274 | 0.309 | 51.242 | 0.29x |
| mixed.json | json | 0.519 | 0.539 | 0.622 | 51.242 | 0.15x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.918 | 13.099 | 15.776 | 55.039 | 1.00x |
| users.json | orjson | 16.403 | 18.252 | 20.936 | 55.039 | 0.72x |
| users.json | msgspec | 14.421 | 16.704 | 26.711 | 55.039 | 0.78x |
| users.json | ujson | 31.806 | 34.016 | 36.265 | 55.039 | 0.39x |
| users.json | json | 26.173 | 28.286 | 31.311 | 55.039 | 0.46x |
| flat.json | strata | 1.014 | 1.125 | 1.288 | 51.723 | 1.00x |
| flat.json | orjson | 1.422 | 1.489 | 1.681 | 51.723 | 0.76x |
| flat.json | msgspec | 1.216 | 1.298 | 1.365 | 51.723 | 0.87x |
| flat.json | ujson | 2.802 | 2.848 | 3.079 | 51.723 | 0.40x |
| flat.json | json | 2.144 | 2.165 | 2.465 | 51.723 | 0.52x |
| nested.json | strata | 0.878 | 0.896 | 1.013 | 51.461 | 1.00x |
| nested.json | orjson | 1.295 | 1.302 | 1.328 | 51.461 | 0.69x |
| nested.json | msgspec | 1.137 | 1.182 | 1.313 | 51.461 | 0.76x |
| nested.json | ujson | 1.993 | 2.067 | 2.451 | 51.461 | 0.43x |
| nested.json | json | 2.300 | 2.396 | 2.545 | 51.461 | 0.37x |
| wide_arrays.json | strata | 5.028 | 5.208 | 7.639 | 52.867 | 1.00x |
| wide_arrays.json | orjson | 6.780 | 6.946 | 7.235 | 52.867 | 0.75x |
| wide_arrays.json | msgspec | 6.643 | 6.832 | 10.803 | 52.867 | 0.76x |
| wide_arrays.json | ujson | 12.000 | 12.209 | 13.865 | 52.867 | 0.43x |
| wide_arrays.json | json | 12.758 | 13.033 | 22.022 | 52.867 | 0.40x |
| mixed.json | strata | 0.266 | 0.278 | 0.397 | 51.371 | 1.00x |
| mixed.json | orjson | 0.324 | 0.347 | 0.535 | 51.371 | 0.80x |
| mixed.json | msgspec | 0.352 | 0.367 | 0.580 | 51.371 | 0.76x |
| mixed.json | ujson | 0.546 | 0.570 | 0.795 | 51.371 | 0.49x |
| mixed.json | json | 0.588 | 0.601 | 1.010 | 51.371 | 0.46x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 11.751 | 12.002 | 13.675 | 52.207 | 1.00x |
| users.ndjson | orjson | 18.182 | 18.767 | 19.253 | 52.207 | 0.64x |
| users.ndjson | msgspec | 18.071 | 18.738 | 19.342 | 52.207 | 0.64x |
| users.ndjson | ujson | 27.175 | 27.885 | 29.597 | 52.207 | 0.43x |
| users.ndjson | json | 31.602 | 32.194 | 36.439 | 52.207 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.360 | 5.405 | 5.807 | 52.086 | 1.00x |
| users.json | orjson | 4.935 | 5.663 | 7.525 | 52.086 | 0.95x |
| users.json | msgspec | 6.846 | 7.570 | 9.002 | 52.086 | 0.71x |
| users.json | ujson | 24.650 | 27.526 | 32.533 | 52.086 | 0.20x |
| users.json | json | 34.272 | 39.972 | 43.436 | 52.086 | 0.14x |
| flat.json | strata | 0.688 | 0.714 | 0.758 | 52.281 | 1.00x |
| flat.json | orjson | 0.721 | 0.737 | 0.846 | 52.281 | 0.97x |
| flat.json | msgspec | 0.862 | 0.903 | 0.951 | 52.281 | 0.79x |
| flat.json | ujson | 2.817 | 2.841 | 2.866 | 52.281 | 0.25x |
| flat.json | json | 3.360 | 3.383 | 3.616 | 52.281 | 0.21x |
| nested.json | strata | 0.680 | 0.724 | 2.658 | 51.719 | 1.00x |
| nested.json | orjson | 0.687 | 0.771 | 1.302 | 51.719 | 0.94x |
| nested.json | msgspec | 0.843 | 0.920 | 1.422 | 51.719 | 0.79x |
| nested.json | ujson | 2.350 | 2.434 | 3.981 | 51.719 | 0.30x |
| nested.json | json | 3.656 | 3.766 | 5.225 | 51.719 | 0.19x |
| wide_arrays.json | strata | 2.759 | 2.844 | 2.875 | 52.867 | 1.00x |
| wide_arrays.json | orjson | 3.276 | 3.361 | 3.557 | 52.867 | 0.85x |
| wide_arrays.json | msgspec | 4.633 | 4.831 | 5.769 | 52.867 | 0.59x |
| wide_arrays.json | ujson | 14.721 | 14.910 | 23.414 | 52.867 | 0.19x |
| wide_arrays.json | json | 26.051 | 26.494 | 31.135 | 52.867 | 0.11x |
| mixed.json | strata | 0.414 | 0.465 | 0.505 | 51.371 | 1.00x |
| mixed.json | orjson | 0.401 | 0.440 | 0.540 | 51.371 | 1.06x |
| mixed.json | msgspec | 0.427 | 0.468 | 0.538 | 51.371 | 0.99x |
| mixed.json | ujson | 0.771 | 0.815 | 0.858 | 51.371 | 0.57x |
| mixed.json | json | 1.027 | 1.059 | 1.124 | 51.371 | 0.44x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.094 | 0.101 | 0.418 | 51.602 | 1.00x |
| users.json $[*].id | jmespath | 0.461 | 0.507 | 2.263 | 51.602 | 0.20x |
| users.json $[*].id | jsonpath-ng | 2.563 | 2.861 | 4.909 | 51.602 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.467 | 0.530 | 2.154 | 51.809 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.764 | 2.964 | 5.605 | 51.809 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 20.727 | 22.760 | 24.460 | 51.809 | 0.02x |
| users.json $..total | strata | 1.937 | 1.990 | 3.573 | 53.254 | 1.00x |
| users.json $..total | jsonpath-ng | 365.620 | 403.260 | 432.234 | 53.254 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.227 | 5.798 | 6.036 | 51.680 | 1.00x |
| users.json $[*].id | orjson+jmespath | 16.248 | 20.285 | 25.375 | 51.680 | 0.29x |
| users.json $[*].id | orjson+jsonpath-ng | 20.905 | 22.029 | 22.610 | 51.680 | 0.26x |
| users.json $[*].orders[*].total | strata | 4.375 | 4.557 | 7.537 | 52.918 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 20.842 | 21.924 | 25.614 | 52.918 | 0.21x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 39.442 | 48.368 | 52.854 | 52.918 | 0.09x |
| users.json $..total | strata | 15.175 | 16.922 | 21.803 | 53.293 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 355.761 | 424.157 | 456.267 | 53.293 | 0.04x |

