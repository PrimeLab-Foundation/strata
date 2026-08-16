# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 208e6f9
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 25 Model 17 Stepping 1, AuthenticAMD
- compiler_flags: /std:c++20 /O2 /arch:AVX2 /GL /LTCG /USEPROFILE (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.957 | 11.965 | 22.683 | 45.469 | 1.00x |
| users.json | orjson | 14.620 | 15.045 | 17.129 | 45.469 | 0.80x |
| users.json | msgspec | 13.318 | 13.702 | 17.659 | 45.469 | 0.87x |
| users.json | ujson | 21.207 | 22.231 | 34.890 | 45.469 | 0.54x |
| users.json | json | 22.432 | 23.310 | 34.396 | 45.469 | 0.51x |
| flat.json | strata | 1.501 | 1.519 | 1.612 | 49.832 | 1.00x |
| flat.json | orjson | 1.748 | 1.775 | 2.507 | 49.832 | 0.86x |
| flat.json | msgspec | 1.693 | 1.742 | 1.891 | 49.832 | 0.87x |
| flat.json | ujson | 3.102 | 3.241 | 3.524 | 49.832 | 0.47x |
| flat.json | json | 3.146 | 3.171 | 3.542 | 49.832 | 0.48x |
| nested.json | strata | 1.403 | 1.438 | 1.485 | 49.605 | 1.00x |
| nested.json | orjson | 1.654 | 1.736 | 2.486 | 49.605 | 0.83x |
| nested.json | msgspec | 1.594 | 1.645 | 1.657 | 49.605 | 0.87x |
| nested.json | ujson | 2.598 | 2.678 | 2.748 | 49.605 | 0.54x |
| nested.json | json | 3.421 | 3.459 | 4.518 | 49.605 | 0.42x |
| wide_arrays.json | strata | 7.965 | 8.734 | 54.265 | 51.578 | 1.00x |
| wide_arrays.json | orjson | 8.953 | 9.213 | 9.830 | 51.578 | 0.95x |
| wide_arrays.json | msgspec | 9.310 | 9.679 | 11.013 | 51.578 | 0.90x |
| wide_arrays.json | ujson | 12.835 | 13.086 | 13.565 | 51.578 | 0.67x |
| wide_arrays.json | json | 18.893 | 19.145 | 20.797 | 51.578 | 0.46x |
| mixed.json | strata | 0.209 | 0.309 | 0.377 | 49.539 | 1.00x |
| mixed.json | orjson | 0.220 | 0.246 | 0.511 | 49.539 | 1.26x |
| mixed.json | msgspec | 0.238 | 0.259 | 0.396 | 49.539 | 1.20x |
| mixed.json | ujson | 0.360 | 0.399 | 0.680 | 49.539 | 0.78x |
| mixed.json | json | 0.464 | 0.622 | 1.144 | 49.539 | 0.50x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.725 | 4.444 | 5.728 | 44.672 | 1.00x |
| users.json | orjson | 4.014 | 4.126 | 5.987 | 44.672 | 1.08x |
| users.json | msgspec | 5.994 | 6.228 | 8.212 | 44.672 | 0.71x |
| users.json | ujson | 16.511 | 17.866 | 23.730 | 44.672 | 0.25x |
| users.json | json | 24.092 | 25.825 | 40.360 | 44.672 | 0.17x |
| flat.json | strata | 0.457 | 0.526 | 0.619 | 49.594 | 1.00x |
| flat.json | orjson | 0.534 | 0.564 | 0.822 | 49.594 | 0.93x |
| flat.json | msgspec | 0.740 | 0.851 | 1.360 | 49.594 | 0.62x |
| flat.json | ujson | 2.432 | 2.528 | 3.256 | 49.594 | 0.21x |
| flat.json | json | 3.240 | 3.564 | 3.693 | 49.594 | 0.15x |
| nested.json | strata | 0.420 | 0.445 | 0.469 | 49.926 | 1.00x |
| nested.json | orjson | 0.464 | 0.484 | 0.520 | 49.926 | 0.92x |
| nested.json | msgspec | 0.718 | 0.738 | 0.766 | 49.926 | 0.60x |
| nested.json | ujson | 2.037 | 2.096 | 2.152 | 49.926 | 0.21x |
| nested.json | json | 4.109 | 4.148 | 49.904 | 49.926 | 0.11x |
| wide_arrays.json | strata | 2.572 | 2.785 | 4.359 | 51.316 | 1.00x |
| wide_arrays.json | orjson | 2.739 | 3.106 | 7.932 | 51.316 | 0.90x |
| wide_arrays.json | msgspec | 4.619 | 5.245 | 10.380 | 51.316 | 0.53x |
| wide_arrays.json | ujson | 10.905 | 11.985 | 85.377 | 51.316 | 0.23x |
| wide_arrays.json | json | 19.496 | 21.094 | 52.456 | 51.316 | 0.13x |
| mixed.json | strata | 0.082 | 0.093 | 0.122 | 49.867 | 1.00x |
| mixed.json | orjson | 0.074 | 0.101 | 0.141 | 49.867 | 0.92x |
| mixed.json | msgspec | 0.103 | 0.117 | 0.174 | 49.867 | 0.80x |
| mixed.json | ujson | 0.334 | 0.471 | 0.526 | 49.867 | 0.20x |
| mixed.json | json | 0.532 | 0.622 | 0.938 | 49.867 | 0.15x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 13.895 | 16.241 | 22.537 | 53.953 | 1.00x |
| users.json | orjson | 16.312 | 18.896 | 31.502 | 53.953 | 0.86x |
| users.json | msgspec | 14.860 | 19.107 | 36.544 | 53.953 | 0.85x |
| users.json | ujson | 26.689 | 30.858 | 74.335 | 53.953 | 0.53x |
| users.json | json | 24.350 | 27.815 | 40.230 | 53.953 | 0.58x |
| flat.json | strata | 2.132 | 2.296 | 2.525 | 49.176 | 1.00x |
| flat.json | orjson | 2.113 | 2.184 | 2.474 | 49.176 | 1.05x |
| flat.json | msgspec | 1.901 | 2.000 | 2.448 | 49.176 | 1.15x |
| flat.json | ujson | 3.974 | 4.139 | 5.741 | 49.176 | 0.55x |
| flat.json | json | 3.527 | 3.582 | 4.268 | 49.176 | 0.64x |
| nested.json | strata | 1.574 | 1.623 | 1.664 | 49.617 | 1.00x |
| nested.json | orjson | 1.860 | 1.907 | 1.964 | 49.617 | 0.85x |
| nested.json | msgspec | 1.782 | 1.831 | 2.047 | 49.617 | 0.89x |
| nested.json | ujson | 3.037 | 3.104 | 3.172 | 49.617 | 0.52x |
| nested.json | json | 3.553 | 3.608 | 3.709 | 49.617 | 0.45x |
| wide_arrays.json | strata | 9.330 | 15.302 | 36.960 | 50.453 | 1.00x |
| wide_arrays.json | orjson | 7.304 | 12.061 | 24.022 | 50.453 | 1.27x |
| wide_arrays.json | msgspec | 7.050 | 13.242 | 77.918 | 50.453 | 1.16x |
| wide_arrays.json | ujson | 11.251 | 25.645 | 141.752 | 50.453 | 0.60x |
| wide_arrays.json | json | 16.999 | 31.981 | 68.413 | 50.453 | 0.48x |
| mixed.json | strata | 0.280 | 0.325 | 0.474 | 49.793 | 1.00x |
| mixed.json | orjson | 0.344 | 0.424 | 0.647 | 49.793 | 0.76x |
| mixed.json | msgspec | 0.365 | 0.402 | 0.569 | 49.793 | 0.81x |
| mixed.json | ujson | 0.534 | 0.656 | 0.930 | 49.793 | 0.49x |
| mixed.json | json | 0.595 | 0.681 | 2.008 | 49.793 | 0.48x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 20.484 | 21.140 | 25.319 | 50.594 | 1.00x |
| users.ndjson | orjson | 27.286 | 28.453 | 79.815 | 50.594 | 0.74x |
| users.ndjson | msgspec | 27.322 | 29.099 | 76.469 | 50.594 | 0.73x |
| users.ndjson | ujson | 40.082 | 41.438 | 44.184 | 50.594 | 0.51x |
| users.ndjson | json | 49.479 | 51.807 | 103.074 | 50.594 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.012 | 6.192 | 11.719 | 49.699 | 1.00x |
| users.json | orjson | 6.773 | 6.887 | 7.173 | 49.699 | 0.90x |
| users.json | msgspec | 9.321 | 9.481 | 9.824 | 49.699 | 0.65x |
| users.json | ujson | 40.198 | 40.942 | 88.072 | 49.699 | 0.15x |
| users.json | json | 55.437 | 56.014 | 102.355 | 49.699 | 0.11x |
| flat.json | strata | 0.999 | 1.040 | 1.213 | 50.039 | 1.00x |
| flat.json | orjson | 1.028 | 1.069 | 47.597 | 50.039 | 0.97x |
| flat.json | msgspec | 1.250 | 1.313 | 1.704 | 50.039 | 0.79x |
| flat.json | ujson | 4.750 | 4.878 | 8.448 | 50.039 | 0.21x |
| flat.json | json | 5.547 | 6.011 | 10.761 | 50.039 | 0.17x |
| nested.json | strata | 0.879 | 0.962 | 1.257 | 49.902 | 1.00x |
| nested.json | orjson | 0.931 | 1.007 | 1.237 | 49.902 | 0.96x |
| nested.json | msgspec | 1.177 | 1.278 | 1.459 | 49.902 | 0.75x |
| nested.json | ujson | 3.813 | 4.005 | 4.963 | 49.902 | 0.24x |
| nested.json | json | 5.847 | 5.959 | 6.361 | 49.902 | 0.16x |
| wide_arrays.json | strata | 3.404 | 3.678 | 9.169 | 50.500 | 1.00x |
| wide_arrays.json | orjson | 3.316 | 3.740 | 11.346 | 50.500 | 0.98x |
| wide_arrays.json | msgspec | 5.037 | 5.521 | 10.990 | 50.500 | 0.67x |
| wide_arrays.json | ujson | 16.830 | 19.478 | 28.611 | 50.500 | 0.19x |
| wide_arrays.json | json | 26.784 | 32.359 | 64.543 | 50.500 | 0.11x |
| mixed.json | strata | 0.401 | 0.427 | 0.517 | 49.973 | 1.00x |
| mixed.json | orjson | 0.400 | 0.459 | 0.570 | 49.973 | 0.93x |
| mixed.json | msgspec | 0.432 | 0.504 | 0.588 | 49.973 | 0.85x |
| mixed.json | ujson | 0.853 | 1.010 | 1.301 | 49.973 | 0.42x |
| mixed.json | json | 1.012 | 1.205 | 1.852 | 49.973 | 0.35x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.144 | 0.151 | 0.249 | 49.070 | 1.00x |
| users.json $[*].id | jmespath | 0.737 | 0.747 | 0.909 | 49.070 | 0.20x |
| users.json $[*].id | jsonpath-ng | 4.194 | 4.274 | 4.530 | 49.070 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.868 | 0.894 | 1.022 | 50.363 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 4.628 | 4.753 | 5.252 | 50.363 | 0.19x |
| users.json $[*].orders[*].total | jsonpath-ng | 28.317 | 28.822 | 75.887 | 50.363 | 0.03x |
| users.json $..total | strata | 3.230 | 3.265 | 4.305 | 52.328 | 1.00x |
| users.json $..total | jsonpath-ng | 579.340 | 584.998 | 737.002 | 52.328 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 7.885 | 7.955 | 8.395 | 50.133 | 1.00x |
| users.json $[*].id | orjson+jmespath | 22.924 | 23.702 | 25.450 | 50.133 | 0.34x |
| users.json $[*].id | orjson+jsonpath-ng | 26.403 | 27.306 | 28.569 | 50.133 | 0.29x |
| users.json $[*].orders[*].total | strata | 8.406 | 8.498 | 8.758 | 51.867 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 28.120 | 28.991 | 79.194 | 51.867 | 0.29x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 56.308 | 58.049 | 107.689 | 51.867 | 0.15x |
| users.json $..total | strata | 24.550 | 26.591 | 28.593 | 51.934 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 612.525 | 622.776 | 681.887 | 51.934 | 0.04x |

