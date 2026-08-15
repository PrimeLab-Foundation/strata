# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 4d4a4be
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
| users.json | strata | 11.923 | 12.668 | 27.045 | 45.324 | 1.00x |
| users.json | orjson | 14.588 | 15.718 | 28.666 | 45.324 | 0.81x |
| users.json | msgspec | 13.703 | 14.492 | 30.474 | 45.324 | 0.87x |
| users.json | ujson | 23.726 | 26.344 | 64.252 | 45.324 | 0.48x |
| users.json | json | 24.758 | 25.972 | 30.391 | 45.324 | 0.49x |
| flat.json | strata | 1.875 | 2.363 | 4.035 | 50.277 | 1.00x |
| flat.json | orjson | 1.879 | 2.099 | 3.720 | 50.277 | 1.13x |
| flat.json | msgspec | 1.974 | 2.305 | 3.897 | 50.277 | 1.03x |
| flat.json | ujson | 3.985 | 5.489 | 6.085 | 50.277 | 0.43x |
| flat.json | json | 3.590 | 4.101 | 51.251 | 50.277 | 0.58x |
| nested.json | strata | 1.393 | 1.438 | 3.300 | 50.148 | 1.00x |
| nested.json | orjson | 1.758 | 1.841 | 3.837 | 50.148 | 0.78x |
| nested.json | msgspec | 1.705 | 2.993 | 3.682 | 50.148 | 0.48x |
| nested.json | ujson | 2.848 | 4.002 | 8.635 | 50.148 | 0.36x |
| nested.json | json | 3.892 | 5.039 | 11.773 | 50.148 | 0.29x |
| wide_arrays.json | strata | 8.875 | 11.548 | 34.723 | 52.102 | 1.00x |
| wide_arrays.json | orjson | 10.042 | 12.232 | 58.855 | 52.102 | 0.94x |
| wide_arrays.json | msgspec | 10.722 | 12.944 | 69.547 | 52.102 | 0.89x |
| wide_arrays.json | ujson | 14.654 | 20.005 | 42.048 | 52.102 | 0.58x |
| wide_arrays.json | json | 22.925 | 28.790 | 111.403 | 52.102 | 0.40x |
| mixed.json | strata | 0.351 | 0.361 | 2.294 | 49.902 | 1.00x |
| mixed.json | orjson | 0.380 | 0.432 | 0.595 | 49.902 | 0.83x |
| mixed.json | msgspec | 0.426 | 0.439 | 2.354 | 49.902 | 0.82x |
| mixed.json | ujson | 0.617 | 0.666 | 2.712 | 49.902 | 0.54x |
| mixed.json | json | 0.871 | 0.890 | 3.240 | 49.902 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.703 | 3.844 | 6.609 | 44.645 | 1.00x |
| users.json | orjson | 3.945 | 3.990 | 6.174 | 44.645 | 0.96x |
| users.json | msgspec | 5.641 | 5.880 | 9.691 | 44.645 | 0.65x |
| users.json | ujson | 18.066 | 18.257 | 27.132 | 44.645 | 0.21x |
| users.json | json | 25.314 | 26.052 | 29.318 | 44.645 | 0.15x |
| flat.json | strata | 0.551 | 0.591 | 0.677 | 50.980 | 1.00x |
| flat.json | orjson | 0.539 | 0.560 | 0.642 | 50.980 | 1.05x |
| flat.json | msgspec | 0.723 | 0.844 | 1.170 | 50.980 | 0.70x |
| flat.json | ujson | 2.697 | 3.000 | 3.406 | 50.980 | 0.20x |
| flat.json | json | 3.548 | 3.792 | 4.205 | 50.980 | 0.16x |
| nested.json | strata | 0.473 | 0.488 | 1.946 | 50.551 | 1.00x |
| nested.json | orjson | 0.492 | 0.511 | 2.082 | 50.551 | 0.96x |
| nested.json | msgspec | 0.735 | 0.778 | 2.793 | 50.551 | 0.63x |
| nested.json | ujson | 2.310 | 4.109 | 6.350 | 50.551 | 0.12x |
| nested.json | json | 4.382 | 6.309 | 8.603 | 50.551 | 0.08x |
| wide_arrays.json | strata | 3.601 | 3.951 | 5.516 | 50.973 | 1.00x |
| wide_arrays.json | orjson | 3.989 | 4.414 | 6.417 | 50.973 | 0.90x |
| wide_arrays.json | msgspec | 6.029 | 6.446 | 8.276 | 50.973 | 0.61x |
| wide_arrays.json | ujson | 16.512 | 17.087 | 24.726 | 50.973 | 0.23x |
| wide_arrays.json | json | 32.210 | 33.714 | 98.580 | 50.973 | 0.12x |
| mixed.json | strata | 0.129 | 0.136 | 0.160 | 50.270 | 1.00x |
| mixed.json | orjson | 0.105 | 0.111 | 1.907 | 50.270 | 1.23x |
| mixed.json | msgspec | 0.141 | 0.151 | 0.217 | 50.270 | 0.90x |
| mixed.json | ujson | 0.526 | 0.542 | 0.577 | 50.270 | 0.25x |
| mixed.json | json | 0.911 | 0.952 | 48.558 | 50.270 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 14.394 | 14.750 | 15.156 | 54.027 | 1.00x |
| users.json | orjson | 15.580 | 15.998 | 26.421 | 54.027 | 0.92x |
| users.json | msgspec | 15.151 | 16.166 | 28.592 | 54.027 | 0.91x |
| users.json | ujson | 28.993 | 30.083 | 75.127 | 54.027 | 0.49x |
| users.json | json | 25.452 | 25.769 | 26.973 | 54.027 | 0.57x |
| flat.json | strata | 2.211 | 2.442 | 4.455 | 49.918 | 1.00x |
| flat.json | orjson | 2.127 | 3.880 | 21.906 | 49.918 | 0.63x |
| flat.json | msgspec | 2.148 | 2.457 | 4.273 | 49.918 | 0.99x |
| flat.json | ujson | 4.633 | 6.067 | 6.688 | 49.918 | 0.40x |
| flat.json | json | 3.908 | 5.088 | 53.942 | 49.918 | 0.48x |
| nested.json | strata | 1.591 | 1.901 | 1.947 | 50.148 | 1.00x |
| nested.json | orjson | 1.964 | 2.215 | 2.318 | 50.148 | 0.86x |
| nested.json | msgspec | 1.900 | 2.103 | 2.333 | 50.148 | 0.90x |
| nested.json | ujson | 3.302 | 3.680 | 4.014 | 50.148 | 0.52x |
| nested.json | json | 4.048 | 4.370 | 52.209 | 50.148 | 0.44x |
| wide_arrays.json | strata | 14.720 | 16.326 | 17.145 | 50.973 | 1.00x |
| wide_arrays.json | orjson | 12.260 | 14.922 | 16.634 | 50.973 | 1.09x |
| wide_arrays.json | msgspec | 12.823 | 15.459 | 21.230 | 50.973 | 1.06x |
| wide_arrays.json | ujson | 22.028 | 25.950 | 77.453 | 50.973 | 0.63x |
| wide_arrays.json | json | 24.158 | 31.858 | 76.599 | 50.973 | 0.51x |
| mixed.json | strata | 0.442 | 0.460 | 0.625 | 50.383 | 1.00x |
| mixed.json | orjson | 0.558 | 0.594 | 2.566 | 50.383 | 0.77x |
| mixed.json | msgspec | 0.582 | 0.626 | 2.561 | 50.383 | 0.73x |
| mixed.json | ujson | 0.842 | 0.911 | 2.879 | 50.383 | 0.50x |
| mixed.json | json | 1.009 | 1.048 | 3.014 | 50.383 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 25.598 | 27.518 | 41.354 | 51.555 | 1.00x |
| users.ndjson | orjson | 33.014 | 40.389 | 108.135 | 51.555 | 0.68x |
| users.ndjson | msgspec | 33.209 | 37.403 | 105.650 | 51.555 | 0.74x |
| users.ndjson | ujson | 49.553 | 61.609 | 142.732 | 51.555 | 0.45x |
| users.ndjson | json | 62.911 | 75.870 | 134.225 | 51.555 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.920 | 5.019 | 7.843 | 50.762 | 1.00x |
| users.json | orjson | 4.894 | 5.269 | 8.387 | 50.762 | 0.95x |
| users.json | msgspec | 6.644 | 7.000 | 11.326 | 50.762 | 0.72x |
| users.json | ujson | 27.677 | 28.419 | 29.741 | 50.762 | 0.18x |
| users.json | json | 34.418 | 34.997 | 35.963 | 50.762 | 0.14x |
| flat.json | strata | 1.134 | 1.198 | 1.547 | 50.797 | 1.00x |
| flat.json | orjson | 1.034 | 1.122 | 1.577 | 50.797 | 1.07x |
| flat.json | msgspec | 1.222 | 1.300 | 1.696 | 50.797 | 0.92x |
| flat.json | ujson | 5.024 | 5.373 | 6.492 | 50.797 | 0.22x |
| flat.json | json | 6.160 | 6.540 | 18.408 | 50.797 | 0.18x |
| nested.json | strata | 0.976 | 1.067 | 3.110 | 50.543 | 1.00x |
| nested.json | orjson | 0.970 | 1.085 | 3.398 | 50.543 | 0.98x |
| nested.json | msgspec | 1.199 | 1.306 | 3.517 | 50.543 | 0.82x |
| nested.json | ujson | 4.489 | 5.893 | 44.034 | 50.543 | 0.18x |
| nested.json | json | 6.273 | 8.215 | 58.841 | 50.543 | 0.13x |
| wide_arrays.json | strata | 4.750 | 6.861 | 70.740 | 53.367 | 1.00x |
| wide_arrays.json | orjson | 5.173 | 7.129 | 12.806 | 53.367 | 0.96x |
| wide_arrays.json | msgspec | 8.082 | 8.882 | 13.643 | 53.367 | 0.77x |
| wide_arrays.json | ujson | 29.907 | 39.389 | 287.742 | 53.367 | 0.17x |
| wide_arrays.json | json | 49.550 | 61.156 | 111.742 | 53.367 | 0.11x |
| mixed.json | strata | 0.612 | 0.636 | 2.580 | 50.449 | 1.00x |
| mixed.json | orjson | 0.539 | 0.583 | 2.420 | 50.449 | 1.09x |
| mixed.json | msgspec | 0.569 | 0.591 | 2.850 | 50.449 | 1.08x |
| mixed.json | ujson | 1.300 | 1.337 | 3.313 | 50.449 | 0.48x |
| mixed.json | json | 1.699 | 1.768 | 3.637 | 50.449 | 0.36x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.101 | 0.106 | 0.207 | 50.180 | 1.00x |
| users.json $[*].id | jmespath | 0.474 | 0.508 | 0.861 | 50.180 | 0.21x |
| users.json $[*].id | jsonpath-ng | 2.677 | 2.958 | 4.988 | 50.180 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.494 | 0.583 | 0.882 | 50.570 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.985 | 3.171 | 3.403 | 50.570 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 20.018 | 21.479 | 38.721 | 50.570 | 0.03x |
| users.json $..total | strata | 1.929 | 2.209 | 5.517 | 52.121 | 1.00x |
| users.json $..total | jsonpath-ng | 388.809 | 537.112 | 775.531 | 52.121 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 5.622 | 5.734 | 10.092 | 50.242 | 1.00x |
| users.json $[*].id | orjson+jmespath | 16.089 | 16.816 | 23.761 | 50.242 | 0.34x |
| users.json $[*].id | orjson+jsonpath-ng | 18.970 | 20.446 | 25.697 | 50.242 | 0.28x |
| users.json $[*].orders[*].total | strata | 6.081 | 6.617 | 12.170 | 52.059 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 22.385 | 35.670 | 117.212 | 52.059 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 42.470 | 71.449 | 169.412 | 52.059 | 0.09x |
| users.json $..total | strata | 30.717 | 42.975 | 93.223 | 52.082 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 858.961 | 1029.765 | 1246.999 | 52.082 | 0.04x |

