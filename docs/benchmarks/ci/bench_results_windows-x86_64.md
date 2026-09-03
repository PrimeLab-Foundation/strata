# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 74164cd
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD
- compiler_flags: /std:c++20 /O2 /arch:AVX2 /GL /LTCG /USEPROFILE (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.674 | 9.977 | 13.580 | 45.492 | 1.00x |
| users.json | orjson | 13.093 | 13.742 | 25.096 | 45.492 | 0.73x |
| users.json | msgspec | 12.541 | 12.910 | 22.621 | 45.492 | 0.77x |
| users.json | ujson | 20.414 | 22.004 | 29.052 | 45.492 | 0.45x |
| users.json | json | 21.723 | 22.303 | 39.560 | 45.492 | 0.45x |
| flat.json | strata | 1.035 | 1.086 | 1.222 | 49.730 | 1.00x |
| flat.json | orjson | 1.109 | 1.160 | 1.264 | 49.730 | 0.94x |
| flat.json | msgspec | 1.091 | 1.127 | 1.168 | 49.730 | 0.96x |
| flat.json | ujson | 2.095 | 2.121 | 2.175 | 49.730 | 0.51x |
| flat.json | json | 2.027 | 2.048 | 2.063 | 49.730 | 0.53x |
| nested.json | strata | 0.793 | 0.797 | 0.830 | 49.539 | 1.00x |
| nested.json | orjson | 1.048 | 1.093 | 1.125 | 49.539 | 0.73x |
| nested.json | msgspec | 0.994 | 1.020 | 1.046 | 49.539 | 0.78x |
| nested.json | ujson | 1.542 | 1.607 | 1.650 | 49.539 | 0.50x |
| nested.json | json | 2.104 | 2.127 | 2.188 | 49.539 | 0.37x |
| wide_arrays.json | strata | 4.645 | 4.713 | 4.775 | 52.613 | 1.00x |
| wide_arrays.json | orjson | 5.528 | 5.622 | 5.704 | 52.613 | 0.84x |
| wide_arrays.json | msgspec | 5.669 | 5.773 | 8.094 | 52.613 | 0.82x |
| wide_arrays.json | ujson | 8.230 | 8.297 | 13.165 | 52.613 | 0.57x |
| wide_arrays.json | json | 11.509 | 11.637 | 21.213 | 52.613 | 0.41x |
| mixed.json | strata | 0.194 | 0.196 | 0.211 | 49.605 | 1.00x |
| mixed.json | orjson | 0.211 | 0.213 | 0.260 | 49.605 | 0.92x |
| mixed.json | msgspec | 0.230 | 0.247 | 0.384 | 49.605 | 0.79x |
| mixed.json | ujson | 0.359 | 0.386 | 0.561 | 49.605 | 0.51x |
| mixed.json | json | 0.460 | 0.483 | 0.517 | 49.605 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.219 | 3.274 | 3.567 | 44.805 | 1.00x |
| users.json | orjson | 3.609 | 3.844 | 5.930 | 44.805 | 0.85x |
| users.json | msgspec | 4.803 | 5.169 | 8.076 | 44.805 | 0.63x |
| users.json | ujson | 16.145 | 16.375 | 27.072 | 44.805 | 0.20x |
| users.json | json | 23.196 | 23.582 | 39.531 | 44.805 | 0.14x |
| flat.json | strata | 0.309 | 0.312 | 0.320 | 49.914 | 1.00x |
| flat.json | orjson | 0.354 | 0.363 | 0.400 | 49.914 | 0.86x |
| flat.json | msgspec | 0.514 | 0.614 | 0.632 | 49.914 | 0.51x |
| flat.json | ujson | 1.741 | 1.819 | 1.867 | 49.914 | 0.17x |
| flat.json | json | 1.924 | 2.117 | 2.143 | 49.914 | 0.15x |
| nested.json | strata | 0.285 | 0.290 | 0.468 | 50.570 | 1.00x |
| nested.json | orjson | 0.324 | 0.331 | 0.496 | 50.570 | 0.88x |
| nested.json | msgspec | 0.466 | 0.478 | 0.751 | 50.570 | 0.61x |
| nested.json | ujson | 1.261 | 1.304 | 2.534 | 50.570 | 0.22x |
| nested.json | json | 2.514 | 2.578 | 4.445 | 50.570 | 0.11x |
| wide_arrays.json | strata | 2.231 | 2.259 | 3.329 | 51.070 | 1.00x |
| wide_arrays.json | orjson | 2.559 | 2.593 | 3.705 | 51.070 | 0.87x |
| wide_arrays.json | msgspec | 4.044 | 4.080 | 5.904 | 51.070 | 0.55x |
| wide_arrays.json | ujson | 9.715 | 9.809 | 9.983 | 51.070 | 0.23x |
| wide_arrays.json | json | 18.361 | 18.522 | 27.759 | 51.070 | 0.12x |
| mixed.json | strata | 0.076 | 0.078 | 0.126 | 49.766 | 1.00x |
| mixed.json | orjson | 0.068 | 0.069 | 0.089 | 49.766 | 1.13x |
| mixed.json | msgspec | 0.093 | 0.094 | 0.098 | 49.766 | 0.83x |
| mixed.json | ujson | 0.326 | 0.332 | 0.367 | 49.766 | 0.24x |
| mixed.json | json | 0.515 | 0.559 | 0.603 | 49.766 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.835 | 11.213 | 12.049 | 54.777 | 1.00x |
| users.json | orjson | 14.105 | 14.212 | 24.006 | 54.777 | 0.79x |
| users.json | msgspec | 13.524 | 13.749 | 22.660 | 54.777 | 0.82x |
| users.json | ujson | 25.654 | 26.271 | 40.957 | 54.777 | 0.43x |
| users.json | json | 22.700 | 23.087 | 23.671 | 54.777 | 0.49x |
| flat.json | strata | 1.068 | 1.326 | 1.933 | 49.719 | 1.00x |
| flat.json | orjson | 1.378 | 1.420 | 2.057 | 49.719 | 0.93x |
| flat.json | msgspec | 1.228 | 1.342 | 2.071 | 49.719 | 0.99x |
| flat.json | ujson | 2.735 | 2.807 | 4.297 | 49.719 | 0.47x |
| flat.json | json | 2.152 | 2.175 | 3.754 | 49.719 | 0.61x |
| nested.json | strata | 0.884 | 0.913 | 0.976 | 50.570 | 1.00x |
| nested.json | orjson | 1.183 | 1.236 | 1.410 | 50.570 | 0.74x |
| nested.json | msgspec | 1.118 | 1.154 | 1.245 | 50.570 | 0.79x |
| nested.json | ujson | 1.992 | 2.021 | 2.094 | 50.570 | 0.45x |
| nested.json | json | 2.234 | 2.267 | 2.462 | 50.570 | 0.40x |
| wide_arrays.json | strata | 5.132 | 5.289 | 5.925 | 51.070 | 1.00x |
| wide_arrays.json | orjson | 6.039 | 6.120 | 9.187 | 51.070 | 0.86x |
| wide_arrays.json | msgspec | 6.245 | 6.306 | 10.315 | 51.070 | 0.84x |
| wide_arrays.json | ujson | 11.258 | 11.410 | 16.168 | 51.070 | 0.46x |
| wide_arrays.json | json | 11.931 | 12.184 | 12.718 | 51.070 | 0.43x |
| mixed.json | strata | 0.273 | 0.285 | 0.318 | 49.469 | 1.00x |
| mixed.json | orjson | 0.324 | 0.379 | 0.453 | 49.469 | 0.75x |
| mixed.json | msgspec | 0.348 | 0.365 | 0.423 | 49.469 | 0.78x |
| mixed.json | ujson | 0.549 | 0.583 | 0.607 | 49.469 | 0.49x |
| mixed.json | json | 0.578 | 0.601 | 0.643 | 49.469 | 0.47x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.878 | 11.312 | 12.035 | 50.668 | 1.00x |
| users.ndjson | orjson | 16.937 | 17.262 | 26.092 | 50.668 | 0.66x |
| users.ndjson | msgspec | 16.925 | 17.412 | 23.310 | 50.668 | 0.65x |
| users.ndjson | ujson | 25.050 | 25.924 | 41.520 | 50.668 | 0.44x |
| users.ndjson | json | 29.400 | 30.033 | 47.939 | 50.668 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.032 | 4.232 | 5.970 | 50.238 | 1.00x |
| users.json | orjson | 4.611 | 4.774 | 5.449 | 50.238 | 0.89x |
| users.json | msgspec | 5.755 | 5.918 | 6.068 | 50.238 | 0.72x |
| users.json | ujson | 25.762 | 25.974 | 26.783 | 50.238 | 0.16x |
| users.json | json | 31.978 | 32.597 | 47.996 | 50.238 | 0.13x |
| flat.json | strata | 0.655 | 0.673 | 0.762 | 50.484 | 1.00x |
| flat.json | orjson | 0.699 | 0.716 | 0.764 | 50.484 | 0.94x |
| flat.json | msgspec | 0.855 | 0.967 | 1.030 | 50.484 | 0.70x |
| flat.json | ujson | 3.106 | 3.160 | 3.353 | 50.484 | 0.21x |
| flat.json | json | 3.321 | 3.554 | 4.009 | 50.484 | 0.19x |
| nested.json | strata | 0.612 | 0.649 | 0.721 | 50.227 | 1.00x |
| nested.json | orjson | 0.654 | 0.682 | 0.807 | 50.227 | 0.95x |
| nested.json | msgspec | 0.795 | 0.846 | 8.216 | 50.227 | 0.77x |
| nested.json | ujson | 2.359 | 2.395 | 2.482 | 50.227 | 0.27x |
| nested.json | json | 3.606 | 3.653 | 3.891 | 50.227 | 0.18x |
| wide_arrays.json | strata | 2.878 | 2.956 | 4.273 | 51.066 | 1.00x |
| wide_arrays.json | orjson | 3.173 | 3.235 | 4.572 | 51.066 | 0.91x |
| wide_arrays.json | msgspec | 4.641 | 4.690 | 6.846 | 51.066 | 0.63x |
| wide_arrays.json | ujson | 16.515 | 16.707 | 24.183 | 51.066 | 0.18x |
| wide_arrays.json | json | 24.962 | 25.220 | 47.221 | 51.066 | 0.12x |
| mixed.json | strata | 0.379 | 0.393 | 0.467 | 49.621 | 1.00x |
| mixed.json | orjson | 0.374 | 0.396 | 0.436 | 49.621 | 0.99x |
| mixed.json | msgspec | 0.398 | 0.425 | 0.693 | 49.621 | 0.92x |
| mixed.json | ujson | 0.799 | 0.812 | 0.854 | 49.621 | 0.48x |
| mixed.json | json | 0.981 | 1.050 | 1.073 | 49.621 | 0.37x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.087 | 0.089 | 0.147 | 50.371 | 1.00x |
| users.json $[*].id | jmespath | 0.447 | 0.453 | 0.828 | 50.371 | 0.20x |
| users.json $[*].id | jsonpath-ng | 2.491 | 2.592 | 4.415 | 50.371 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.468 | 0.488 | 0.520 | 50.750 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.799 | 2.845 | 2.976 | 50.750 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 16.643 | 17.026 | 17.940 | 50.750 | 0.03x |
| users.json $..total | strata | 1.841 | 1.870 | 1.904 | 52.379 | 1.00x |
| users.json $..total | jsonpath-ng | 326.951 | 350.992 | 379.145 | 52.379 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.013 | 4.099 | 4.352 | 50.379 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.489 | 14.868 | 15.493 | 50.379 | 0.28x |
| users.json $[*].id | orjson+jsonpath-ng | 16.512 | 17.258 | 26.551 | 50.379 | 0.24x |
| users.json $[*].orders[*].total | strata | 4.228 | 4.300 | 4.461 | 52.379 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.300 | 17.744 | 25.534 | 52.379 | 0.24x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 34.857 | 35.290 | 41.811 | 52.379 | 0.12x |
| users.json $..total | strata | 13.577 | 14.409 | 22.378 | 52.418 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 346.672 | 369.748 | 395.503 | 52.418 | 0.04x |

