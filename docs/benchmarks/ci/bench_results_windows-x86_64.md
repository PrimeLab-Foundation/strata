# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 07e47e6
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 25 Model 17 Stepping 1, AuthenticAMD
- compiler_flags: clang-cl /std:c++20 /O2 /arch:AVX2 -fprofile-use (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.208 | 11.137 | 23.030 | 45.598 | 1.00x |
| users.json | orjson | 14.235 | 18.924 | 25.815 | 45.598 | 0.59x |
| users.json | msgspec | 13.625 | 20.870 | 70.395 | 45.598 | 0.53x |
| users.json | ujson | 21.708 | 30.208 | 56.712 | 45.598 | 0.37x |
| users.json | json | 22.560 | 40.750 | 54.494 | 45.598 | 0.27x |
| flat.json | strata | 1.331 | 1.408 | 3.168 | 51.219 | 1.00x |
| flat.json | orjson | 2.076 | 2.995 | 3.852 | 51.219 | 0.47x |
| flat.json | msgspec | 1.691 | 1.729 | 3.429 | 51.219 | 0.81x |
| flat.json | ujson | 3.220 | 3.394 | 5.039 | 51.219 | 0.41x |
| flat.json | json | 3.162 | 4.198 | 5.011 | 51.219 | 0.34x |
| nested.json | strata | 1.225 | 1.253 | 2.905 | 50.594 | 1.00x |
| nested.json | orjson | 1.686 | 1.728 | 2.778 | 50.594 | 0.73x |
| nested.json | msgspec | 1.590 | 1.623 | 1.666 | 50.594 | 0.77x |
| nested.json | ujson | 2.564 | 2.595 | 3.216 | 50.594 | 0.48x |
| nested.json | json | 3.489 | 3.543 | 4.006 | 50.594 | 0.35x |
| wide_arrays.json | strata | 7.454 | 7.933 | 11.330 | 52.566 | 1.00x |
| wide_arrays.json | orjson | 9.326 | 11.181 | 14.100 | 52.566 | 0.71x |
| wide_arrays.json | msgspec | 9.684 | 9.878 | 13.875 | 52.566 | 0.80x |
| wide_arrays.json | ujson | 12.943 | 13.222 | 21.408 | 52.566 | 0.60x |
| wide_arrays.json | json | 19.404 | 19.682 | 27.313 | 52.566 | 0.40x |
| mixed.json | strata | 0.294 | 0.299 | 0.329 | 51.164 | 1.00x |
| mixed.json | orjson | 0.343 | 0.346 | 0.377 | 51.164 | 0.86x |
| mixed.json | msgspec | 0.380 | 0.385 | 0.577 | 51.164 | 0.78x |
| mixed.json | ujson | 0.551 | 0.557 | 0.602 | 51.164 | 0.54x |
| mixed.json | json | 0.791 | 0.795 | 0.831 | 51.164 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.228 | 6.474 | 7.169 | 44.215 | 1.00x |
| users.json | orjson | 3.877 | 5.980 | 7.992 | 44.215 | 1.08x |
| users.json | msgspec | 6.214 | 10.030 | 12.124 | 44.215 | 0.65x |
| users.json | ujson | 17.675 | 29.373 | 37.703 | 44.215 | 0.22x |
| users.json | json | 25.329 | 58.098 | 152.824 | 44.215 | 0.11x |
| flat.json | strata | 0.437 | 0.459 | 2.168 | 50.961 | 1.00x |
| flat.json | orjson | 0.551 | 0.632 | 2.401 | 50.961 | 0.73x |
| flat.json | msgspec | 0.769 | 0.818 | 0.918 | 50.961 | 0.56x |
| flat.json | ujson | 2.546 | 2.573 | 4.270 | 50.961 | 0.18x |
| flat.json | json | 3.575 | 4.228 | 5.417 | 50.961 | 0.11x |
| nested.json | strata | 0.420 | 0.436 | 0.501 | 50.980 | 1.00x |
| nested.json | orjson | 0.468 | 0.477 | 2.293 | 50.980 | 0.91x |
| nested.json | msgspec | 0.718 | 0.747 | 2.367 | 50.980 | 0.58x |
| nested.json | ujson | 2.119 | 2.127 | 3.989 | 50.980 | 0.21x |
| nested.json | json | 4.160 | 5.911 | 50.030 | 50.980 | 0.07x |
| wide_arrays.json | strata | 3.047 | 3.177 | 5.674 | 52.152 | 1.00x |
| wide_arrays.json | orjson | 3.828 | 4.024 | 5.770 | 52.152 | 0.79x |
| wide_arrays.json | msgspec | 6.348 | 6.575 | 10.137 | 52.152 | 0.48x |
| wide_arrays.json | ujson | 15.224 | 15.376 | 23.194 | 52.152 | 0.21x |
| wide_arrays.json | json | 29.981 | 31.145 | 110.153 | 52.152 | 0.10x |
| mixed.json | strata | 0.103 | 0.104 | 0.133 | 50.418 | 1.00x |
| mixed.json | orjson | 0.102 | 0.105 | 1.002 | 50.418 | 0.99x |
| mixed.json | msgspec | 0.137 | 0.140 | 0.189 | 50.418 | 0.75x |
| mixed.json | ujson | 0.503 | 0.506 | 0.542 | 50.418 | 0.21x |
| mixed.json | json | 0.870 | 0.896 | 0.940 | 50.418 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 17.119 | 24.688 | 60.749 | 54.250 | 1.00x |
| users.json | orjson | 23.164 | 32.403 | 89.052 | 54.250 | 0.76x |
| users.json | msgspec | 26.567 | 30.716 | 50.428 | 54.250 | 0.80x |
| users.json | ujson | 39.229 | 55.646 | 119.129 | 54.250 | 0.44x |
| users.json | json | 39.310 | 55.453 | 123.552 | 54.250 | 0.45x |
| flat.json | strata | 1.796 | 1.839 | 3.617 | 50.770 | 1.00x |
| flat.json | orjson | 2.361 | 3.388 | 48.564 | 50.770 | 0.54x |
| flat.json | msgspec | 1.982 | 3.062 | 3.989 | 50.770 | 0.60x |
| flat.json | ujson | 3.872 | 4.820 | 5.855 | 50.770 | 0.38x |
| flat.json | json | 3.358 | 3.449 | 5.627 | 50.770 | 0.53x |
| nested.json | strata | 1.370 | 1.395 | 47.720 | 50.613 | 1.00x |
| nested.json | orjson | 1.884 | 1.902 | 3.692 | 50.613 | 0.73x |
| nested.json | msgspec | 1.741 | 1.826 | 5.369 | 50.613 | 0.76x |
| nested.json | ujson | 3.019 | 3.087 | 4.958 | 50.613 | 0.45x |
| nested.json | json | 3.688 | 3.731 | 4.118 | 50.613 | 0.37x |
| wide_arrays.json | strata | 8.084 | 8.307 | 11.929 | 52.152 | 1.00x |
| wide_arrays.json | orjson | 9.870 | 10.042 | 15.460 | 52.152 | 0.83x |
| wide_arrays.json | msgspec | 10.249 | 10.388 | 15.757 | 52.152 | 0.80x |
| wide_arrays.json | ujson | 16.443 | 17.242 | 66.650 | 52.152 | 0.48x |
| wide_arrays.json | json | 19.902 | 20.254 | 30.761 | 52.152 | 0.41x |
| mixed.json | strata | 0.393 | 0.402 | 0.457 | 50.406 | 1.00x |
| mixed.json | orjson | 0.491 | 0.545 | 0.661 | 50.406 | 0.74x |
| mixed.json | msgspec | 0.518 | 0.550 | 0.605 | 50.406 | 0.73x |
| mixed.json | ujson | 0.757 | 0.786 | 0.843 | 50.406 | 0.51x |
| mixed.json | json | 0.934 | 0.955 | 0.989 | 50.406 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 17.244 | 25.887 | 65.065 | 52.035 | 1.00x |
| users.ndjson | orjson | 27.755 | 32.096 | 77.000 | 52.035 | 0.81x |
| users.ndjson | msgspec | 27.772 | 32.143 | 51.539 | 52.035 | 0.81x |
| users.ndjson | ujson | 40.904 | 47.805 | 124.345 | 52.035 | 0.54x |
| users.ndjson | json | 48.989 | 52.884 | 133.761 | 52.035 | 0.49x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 5.841 | 7.776 | 10.193 | 50.496 | 1.00x |
| users.json | orjson | 6.632 | 8.810 | 10.286 | 50.496 | 0.88x |
| users.json | msgspec | 9.352 | 12.947 | 14.869 | 50.496 | 0.60x |
| users.json | ujson | 41.299 | 58.906 | 163.522 | 50.496 | 0.13x |
| users.json | json | 57.804 | 77.515 | 143.064 | 50.496 | 0.10x |
| flat.json | strata | 0.906 | 0.942 | 2.699 | 51.344 | 1.00x |
| flat.json | orjson | 1.004 | 1.071 | 2.841 | 51.344 | 0.88x |
| flat.json | msgspec | 1.206 | 1.266 | 1.292 | 51.344 | 0.74x |
| flat.json | ujson | 4.792 | 5.784 | 7.095 | 51.344 | 0.16x |
| flat.json | json | 5.697 | 7.673 | 12.226 | 51.344 | 0.12x |
| nested.json | strata | 0.867 | 0.911 | 0.967 | 50.965 | 1.00x |
| nested.json | orjson | 0.909 | 0.956 | 1.082 | 50.965 | 0.95x |
| nested.json | msgspec | 1.178 | 1.220 | 1.394 | 50.965 | 0.75x |
| nested.json | ujson | 3.901 | 3.927 | 8.605 | 50.965 | 0.23x |
| nested.json | json | 5.966 | 6.013 | 6.131 | 50.965 | 0.15x |
| wide_arrays.json | strata | 3.988 | 4.053 | 35.018 | 52.152 | 1.00x |
| wide_arrays.json | orjson | 4.592 | 4.684 | 6.523 | 52.152 | 0.87x |
| wide_arrays.json | msgspec | 7.090 | 7.276 | 53.682 | 52.152 | 0.56x |
| wide_arrays.json | ujson | 27.237 | 27.651 | 74.607 | 52.152 | 0.15x |
| wide_arrays.json | json | 41.825 | 42.383 | 114.333 | 52.152 | 0.10x |
| mixed.json | strata | 0.485 | 0.513 | 0.585 | 50.453 | 1.00x |
| mixed.json | orjson | 0.492 | 0.508 | 0.602 | 50.453 | 1.01x |
| mixed.json | msgspec | 0.532 | 0.544 | 0.783 | 50.453 | 0.94x |
| mixed.json | ujson | 1.207 | 1.264 | 1.584 | 50.453 | 0.41x |
| mixed.json | json | 1.579 | 1.629 | 1.690 | 50.453 | 0.31x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.141 | 0.151 | 0.166 | 50.539 | 1.00x |
| users.json $[*].id | jmespath | 0.747 | 0.770 | 0.821 | 50.539 | 0.20x |
| users.json $[*].id | jsonpath-ng | 4.171 | 6.109 | 6.353 | 50.539 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.848 | 0.879 | 2.770 | 51.570 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 4.776 | 4.863 | 8.317 | 51.570 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 28.535 | 40.418 | 78.170 | 51.570 | 0.02x |
| users.json $..total | strata | 3.416 | 3.454 | 6.217 | 53.570 | 1.00x |
| users.json $..total | jsonpath-ng | 607.000 | 740.078 | 812.074 | 53.570 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 5.875 | 6.996 | 9.225 | 51.566 | 1.00x |
| users.json $[*].id | orjson+jmespath | 23.930 | 30.902 | 83.099 | 51.566 | 0.23x |
| users.json $[*].id | orjson+jsonpath-ng | 27.801 | 38.408 | 83.551 | 51.566 | 0.18x |
| users.json $[*].orders[*].total | strata | 6.255 | 6.463 | 10.530 | 53.570 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 28.393 | 30.413 | 92.741 | 53.570 | 0.21x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 57.336 | 67.495 | 143.167 | 53.570 | 0.10x |
| users.json $..total | strata | 22.289 | 31.529 | 40.236 | 53.570 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 740.277 | 904.523 | 1171.499 | 53.570 | 0.03x |

