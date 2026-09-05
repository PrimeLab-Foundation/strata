# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: b6e2a27
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
| users.json | strata | 11.426 | 12.238 | 14.839 | 45.566 | 1.00x |
| users.json | orjson | 16.193 | 18.402 | 19.913 | 45.566 | 0.67x |
| users.json | msgspec | 15.142 | 16.677 | 19.257 | 45.566 | 0.73x |
| users.json | ujson | 25.992 | 27.133 | 32.469 | 45.566 | 0.45x |
| users.json | json | 24.642 | 26.468 | 27.510 | 45.566 | 0.46x |
| flat.json | strata | 1.074 | 1.245 | 1.344 | 49.586 | 1.00x |
| flat.json | orjson | 1.392 | 1.501 | 1.618 | 49.586 | 0.83x |
| flat.json | msgspec | 1.343 | 1.452 | 1.617 | 49.586 | 0.86x |
| flat.json | ujson | 2.283 | 2.429 | 2.525 | 49.586 | 0.51x |
| flat.json | json | 2.073 | 2.180 | 2.836 | 49.586 | 0.57x |
| nested.json | strata | 0.774 | 0.822 | 1.285 | 50.188 | 1.00x |
| nested.json | orjson | 1.124 | 1.177 | 1.464 | 50.188 | 0.70x |
| nested.json | msgspec | 0.984 | 1.068 | 1.737 | 50.188 | 0.77x |
| nested.json | ujson | 1.660 | 1.763 | 2.302 | 50.188 | 0.47x |
| nested.json | json | 2.123 | 2.164 | 2.335 | 50.188 | 0.38x |
| wide_arrays.json | strata | 5.032 | 5.590 | 6.249 | 51.777 | 1.00x |
| wide_arrays.json | orjson | 7.036 | 7.332 | 7.694 | 51.777 | 0.76x |
| wide_arrays.json | msgspec | 6.896 | 7.120 | 7.686 | 51.777 | 0.79x |
| wide_arrays.json | ujson | 9.080 | 9.444 | 14.377 | 51.777 | 0.59x |
| wide_arrays.json | json | 12.844 | 13.153 | 15.197 | 51.777 | 0.42x |
| mixed.json | strata | 0.238 | 0.266 | 0.313 | 49.859 | 1.00x |
| mixed.json | orjson | 0.277 | 0.321 | 0.374 | 49.859 | 0.83x |
| mixed.json | msgspec | 0.303 | 0.348 | 0.427 | 49.859 | 0.76x |
| mixed.json | ujson | 0.421 | 0.460 | 0.557 | 49.859 | 0.58x |
| mixed.json | json | 0.526 | 0.586 | 0.658 | 49.859 | 0.45x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.476 | 3.611 | 3.928 | 45.801 | 1.00x |
| users.json | orjson | 3.956 | 4.222 | 4.487 | 45.801 | 0.86x |
| users.json | msgspec | 5.616 | 5.954 | 6.459 | 45.801 | 0.61x |
| users.json | ujson | 12.938 | 13.482 | 17.564 | 45.801 | 0.27x |
| users.json | json | 23.807 | 24.595 | 25.335 | 45.801 | 0.15x |
| flat.json | strata | 0.406 | 0.440 | 0.506 | 49.770 | 1.00x |
| flat.json | orjson | 0.448 | 0.486 | 0.533 | 49.770 | 0.91x |
| flat.json | msgspec | 0.623 | 0.644 | 0.808 | 49.770 | 0.68x |
| flat.json | ujson | 1.608 | 1.659 | 1.747 | 49.770 | 0.27x |
| flat.json | json | 2.265 | 2.364 | 5.799 | 49.770 | 0.19x |
| nested.json | strata | 0.259 | 0.264 | 0.447 | 50.258 | 1.00x |
| nested.json | orjson | 0.318 | 0.323 | 0.540 | 50.258 | 0.82x |
| nested.json | msgspec | 0.516 | 0.529 | 0.759 | 50.258 | 0.50x |
| nested.json | ujson | 1.037 | 1.093 | 1.784 | 50.258 | 0.24x |
| nested.json | json | 2.483 | 2.530 | 4.330 | 50.258 | 0.10x |
| wide_arrays.json | strata | 2.220 | 2.345 | 2.972 | 50.816 | 1.00x |
| wide_arrays.json | orjson | 2.727 | 2.853 | 2.987 | 50.816 | 0.82x |
| wide_arrays.json | msgspec | 4.024 | 4.505 | 4.742 | 50.816 | 0.52x |
| wide_arrays.json | ujson | 8.031 | 8.243 | 11.938 | 50.816 | 0.28x |
| wide_arrays.json | json | 18.860 | 19.188 | 22.675 | 50.816 | 0.12x |
| mixed.json | strata | 0.082 | 0.099 | 0.239 | 50.000 | 1.00x |
| mixed.json | orjson | 0.079 | 0.097 | 0.177 | 50.000 | 1.02x |
| mixed.json | msgspec | 0.101 | 0.129 | 0.183 | 50.000 | 0.77x |
| mixed.json | ujson | 0.284 | 0.315 | 0.629 | 50.000 | 0.31x |
| mixed.json | json | 0.542 | 0.600 | 1.079 | 50.000 | 0.17x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 12.168 | 12.660 | 13.525 | 55.824 | 1.00x |
| users.json | orjson | 16.618 | 17.215 | 17.787 | 55.824 | 0.74x |
| users.json | msgspec | 15.545 | 16.020 | 18.985 | 55.824 | 0.79x |
| users.json | ujson | 26.672 | 28.087 | 30.998 | 55.824 | 0.45x |
| users.json | json | 25.027 | 26.144 | 27.890 | 55.824 | 0.48x |
| flat.json | strata | 1.242 | 1.506 | 1.856 | 50.113 | 1.00x |
| flat.json | orjson | 1.432 | 1.895 | 3.010 | 50.113 | 0.79x |
| flat.json | msgspec | 1.426 | 1.699 | 1.866 | 50.113 | 0.89x |
| flat.json | ujson | 2.796 | 2.961 | 3.347 | 50.113 | 0.51x |
| flat.json | json | 2.369 | 2.495 | 2.769 | 50.113 | 0.60x |
| nested.json | strata | 0.961 | 1.026 | 1.058 | 50.258 | 1.00x |
| nested.json | orjson | 1.364 | 1.421 | 1.671 | 50.258 | 0.72x |
| nested.json | msgspec | 1.187 | 1.301 | 1.367 | 50.258 | 0.79x |
| nested.json | ujson | 2.026 | 2.089 | 2.236 | 50.258 | 0.49x |
| nested.json | json | 2.381 | 2.456 | 3.137 | 50.258 | 0.42x |
| wide_arrays.json | strata | 5.684 | 5.722 | 5.871 | 50.816 | 1.00x |
| wide_arrays.json | orjson | 7.158 | 7.442 | 7.819 | 50.816 | 0.77x |
| wide_arrays.json | msgspec | 7.403 | 7.573 | 8.711 | 50.816 | 0.76x |
| wide_arrays.json | ujson | 11.474 | 12.005 | 13.245 | 50.816 | 0.48x |
| wide_arrays.json | json | 13.054 | 13.369 | 13.712 | 50.816 | 0.43x |
| mixed.json | strata | 0.269 | 0.472 | 0.510 | 50.008 | 1.00x |
| mixed.json | orjson | 0.329 | 0.599 | 0.974 | 50.008 | 0.79x |
| mixed.json | msgspec | 0.360 | 0.660 | 0.927 | 50.008 | 0.72x |
| mixed.json | ujson | 0.533 | 0.824 | 1.084 | 50.008 | 0.57x |
| mixed.json | json | 0.639 | 0.874 | 1.416 | 50.008 | 0.54x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 11.455 | 15.239 | 22.587 | 50.434 | 1.00x |
| users.ndjson | orjson | 19.009 | 23.128 | 25.799 | 50.434 | 0.66x |
| users.ndjson | msgspec | 18.343 | 22.302 | 27.406 | 50.434 | 0.68x |
| users.ndjson | ujson | 25.726 | 31.514 | 33.877 | 50.434 | 0.48x |
| users.ndjson | json | 31.485 | 36.907 | 103.952 | 50.434 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.364 | 4.529 | 5.074 | 50.883 | 1.00x |
| users.json | orjson | 5.048 | 5.180 | 5.755 | 50.883 | 0.87x |
| users.json | msgspec | 6.784 | 7.124 | 8.713 | 50.883 | 0.64x |
| users.json | ujson | 22.564 | 22.844 | 23.124 | 50.883 | 0.20x |
| users.json | json | 32.869 | 33.134 | 33.471 | 50.883 | 0.14x |
| flat.json | strata | 0.669 | 0.793 | 1.021 | 50.172 | 1.00x |
| flat.json | orjson | 0.776 | 0.826 | 1.172 | 50.172 | 0.96x |
| flat.json | msgspec | 0.913 | 0.993 | 1.393 | 50.172 | 0.80x |
| flat.json | ujson | 2.896 | 2.962 | 4.335 | 50.172 | 0.27x |
| flat.json | json | 3.463 | 3.541 | 4.921 | 50.172 | 0.22x |
| nested.json | strata | 0.743 | 0.855 | 1.135 | 50.328 | 1.00x |
| nested.json | orjson | 0.838 | 0.874 | 1.167 | 50.328 | 0.98x |
| nested.json | msgspec | 1.011 | 1.070 | 1.407 | 50.328 | 0.80x |
| nested.json | ujson | 2.277 | 2.326 | 3.835 | 50.328 | 0.37x |
| nested.json | json | 3.712 | 3.816 | 7.351 | 50.328 | 0.22x |
| wide_arrays.json | strata | 2.747 | 3.426 | 6.176 | 50.816 | 1.00x |
| wide_arrays.json | orjson | 3.177 | 4.125 | 4.907 | 50.816 | 0.83x |
| wide_arrays.json | msgspec | 4.482 | 5.355 | 6.464 | 50.816 | 0.64x |
| wide_arrays.json | ujson | 14.174 | 15.354 | 15.876 | 50.816 | 0.22x |
| wide_arrays.json | json | 24.497 | 26.155 | 32.150 | 50.816 | 0.13x |
| mixed.json | strata | 0.388 | 0.398 | 0.455 | 50.012 | 1.00x |
| mixed.json | orjson | 0.384 | 0.396 | 0.424 | 50.012 | 1.00x |
| mixed.json | msgspec | 0.418 | 0.445 | 0.532 | 50.012 | 0.89x |
| mixed.json | ujson | 0.745 | 0.780 | 0.826 | 50.012 | 0.51x |
| mixed.json | json | 1.000 | 1.036 | 1.063 | 50.012 | 0.38x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.109 | 0.112 | 0.125 | 50.336 | 1.00x |
| users.json $[*].id | jmespath | 0.447 | 0.455 | 0.503 | 50.336 | 0.25x |
| users.json $[*].id | jsonpath-ng | 2.511 | 2.881 | 2.962 | 50.336 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.566 | 0.612 | 0.748 | 50.750 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.902 | 3.038 | 5.132 | 50.750 | 0.20x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.767 | 19.639 | 21.315 | 50.750 | 0.03x |
| users.json $..total | strata | 2.207 | 2.337 | 2.948 | 52.750 | 1.00x |
| users.json $..total | jsonpath-ng | 320.267 | 323.195 | 329.278 | 52.750 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.136 | 4.171 | 4.323 | 50.398 | 1.00x |
| users.json $[*].id | orjson+jmespath | 16.716 | 17.304 | 19.808 | 50.398 | 0.24x |
| users.json $[*].id | orjson+jsonpath-ng | 18.840 | 19.666 | 21.892 | 50.398 | 0.21x |
| users.json $[*].orders[*].total | strata | 4.420 | 4.466 | 6.220 | 52.750 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 20.082 | 21.566 | 23.166 | 52.750 | 0.21x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 40.228 | 44.522 | 47.794 | 52.750 | 0.10x |
| users.json $..total | strata | 17.007 | 17.832 | 19.360 | 52.750 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 352.952 | 354.624 | 394.317 | 52.750 | 0.05x |

