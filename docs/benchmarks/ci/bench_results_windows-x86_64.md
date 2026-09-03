# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 49acca3
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
| users.json | strata | 10.206 | 11.668 | 17.595 | 45.441 | 1.00x |
| users.json | orjson | 14.356 | 15.587 | 22.144 | 45.441 | 0.75x |
| users.json | msgspec | 13.246 | 13.976 | 21.196 | 45.441 | 0.83x |
| users.json | ujson | 21.177 | 23.328 | 38.672 | 45.441 | 0.50x |
| users.json | json | 22.816 | 23.508 | 35.688 | 45.441 | 0.50x |
| flat.json | strata | 1.010 | 1.171 | 1.844 | 48.613 | 1.00x |
| flat.json | orjson | 1.210 | 1.267 | 1.848 | 48.613 | 0.92x |
| flat.json | msgspec | 1.198 | 1.253 | 1.865 | 48.613 | 0.93x |
| flat.json | ujson | 2.184 | 2.250 | 3.285 | 48.613 | 0.52x |
| flat.json | json | 1.902 | 1.949 | 3.157 | 48.613 | 0.60x |
| nested.json | strata | 0.839 | 0.873 | 0.892 | 48.520 | 1.00x |
| nested.json | orjson | 1.094 | 1.141 | 1.171 | 48.520 | 0.77x |
| nested.json | msgspec | 0.980 | 1.001 | 1.041 | 48.520 | 0.87x |
| nested.json | ujson | 1.573 | 1.672 | 1.730 | 48.520 | 0.52x |
| nested.json | json | 2.081 | 2.100 | 2.201 | 48.520 | 0.42x |
| wide_arrays.json | strata | 4.867 | 4.991 | 15.324 | 50.672 | 1.00x |
| wide_arrays.json | orjson | 5.898 | 6.023 | 6.975 | 50.672 | 0.83x |
| wide_arrays.json | msgspec | 5.927 | 6.046 | 9.771 | 50.672 | 0.83x |
| wide_arrays.json | ujson | 8.151 | 8.287 | 13.014 | 50.672 | 0.60x |
| wide_arrays.json | json | 11.391 | 11.702 | 32.822 | 50.672 | 0.43x |
| mixed.json | strata | 0.204 | 0.209 | 0.230 | 48.551 | 1.00x |
| mixed.json | orjson | 0.217 | 0.224 | 0.245 | 48.551 | 0.93x |
| mixed.json | msgspec | 0.228 | 0.235 | 0.312 | 48.551 | 0.89x |
| mixed.json | ujson | 0.344 | 0.355 | 0.389 | 48.551 | 0.59x |
| mixed.json | json | 0.466 | 0.474 | 0.623 | 48.551 | 0.44x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.347 | 3.421 | 4.943 | 45.773 | 1.00x |
| users.json | orjson | 3.634 | 3.852 | 5.770 | 45.773 | 0.89x |
| users.json | msgspec | 5.326 | 5.576 | 7.893 | 45.773 | 0.61x |
| users.json | ujson | 15.978 | 16.291 | 16.679 | 45.773 | 0.21x |
| users.json | json | 22.855 | 23.267 | 30.798 | 45.773 | 0.15x |
| flat.json | strata | 0.340 | 0.346 | 0.370 | 49.062 | 1.00x |
| flat.json | orjson | 0.377 | 0.395 | 0.488 | 49.062 | 0.88x |
| flat.json | msgspec | 0.547 | 0.587 | 0.609 | 49.062 | 0.59x |
| flat.json | ujson | 1.597 | 1.703 | 2.750 | 49.062 | 0.20x |
| flat.json | json | 1.956 | 2.099 | 2.663 | 49.062 | 0.16x |
| nested.json | strata | 0.278 | 0.285 | 0.343 | 49.098 | 1.00x |
| nested.json | orjson | 0.326 | 0.330 | 0.420 | 49.098 | 0.86x |
| nested.json | msgspec | 0.505 | 0.521 | 0.630 | 49.098 | 0.55x |
| nested.json | ujson | 1.340 | 1.381 | 2.247 | 49.098 | 0.21x |
| nested.json | json | 2.486 | 2.544 | 4.066 | 49.098 | 0.11x |
| wide_arrays.json | strata | 2.326 | 2.368 | 3.414 | 50.367 | 1.00x |
| wide_arrays.json | orjson | 2.618 | 2.696 | 3.286 | 50.367 | 0.88x |
| wide_arrays.json | msgspec | 4.356 | 4.447 | 6.907 | 50.367 | 0.53x |
| wide_arrays.json | ujson | 9.921 | 10.112 | 16.563 | 50.367 | 0.23x |
| wide_arrays.json | json | 18.046 | 18.256 | 33.417 | 50.367 | 0.13x |
| mixed.json | strata | 0.076 | 0.084 | 0.092 | 48.508 | 1.00x |
| mixed.json | orjson | 0.073 | 0.077 | 0.107 | 48.508 | 1.09x |
| mixed.json | msgspec | 0.101 | 0.107 | 0.116 | 48.508 | 0.78x |
| mixed.json | ujson | 0.329 | 0.334 | 0.354 | 48.508 | 0.25x |
| mixed.json | json | 0.519 | 0.537 | 0.788 | 48.508 | 0.16x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.333 | 12.028 | 13.734 | 55.801 | 1.00x |
| users.json | orjson | 15.180 | 15.745 | 23.998 | 55.801 | 0.76x |
| users.json | msgspec | 14.213 | 15.132 | 21.662 | 55.801 | 0.79x |
| users.json | ujson | 25.365 | 26.323 | 37.478 | 55.801 | 0.46x |
| users.json | json | 23.464 | 24.351 | 39.713 | 55.801 | 0.49x |
| flat.json | strata | 1.352 | 1.397 | 1.442 | 48.352 | 1.00x |
| flat.json | orjson | 1.544 | 1.568 | 1.651 | 48.352 | 0.89x |
| flat.json | msgspec | 1.264 | 1.307 | 1.427 | 48.352 | 1.07x |
| flat.json | ujson | 2.699 | 2.740 | 2.800 | 48.352 | 0.51x |
| flat.json | json | 2.214 | 2.231 | 2.387 | 48.352 | 0.63x |
| nested.json | strata | 0.999 | 1.022 | 1.595 | 48.715 | 1.00x |
| nested.json | orjson | 1.221 | 1.275 | 1.895 | 48.715 | 0.80x |
| nested.json | msgspec | 1.106 | 1.144 | 1.367 | 48.715 | 0.89x |
| nested.json | ujson | 1.897 | 1.936 | 2.052 | 48.715 | 0.53x |
| nested.json | json | 2.217 | 2.245 | 2.611 | 48.715 | 0.46x |
| wide_arrays.json | strata | 5.356 | 5.478 | 8.493 | 50.367 | 1.00x |
| wide_arrays.json | orjson | 6.417 | 6.606 | 9.830 | 50.367 | 0.83x |
| wide_arrays.json | msgspec | 6.441 | 6.644 | 7.325 | 50.367 | 0.82x |
| wide_arrays.json | ujson | 10.554 | 10.911 | 11.086 | 50.367 | 0.50x |
| wide_arrays.json | json | 11.949 | 12.147 | 17.119 | 50.367 | 0.45x |
| mixed.json | strata | 0.284 | 0.309 | 0.571 | 48.449 | 1.00x |
| mixed.json | orjson | 0.413 | 0.462 | 0.651 | 48.449 | 0.67x |
| mixed.json | msgspec | 0.368 | 0.398 | 0.638 | 48.449 | 0.78x |
| mixed.json | ujson | 0.540 | 0.582 | 0.850 | 48.449 | 0.53x |
| mixed.json | json | 0.585 | 0.611 | 0.929 | 48.449 | 0.51x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 11.756 | 12.796 | 20.870 | 49.582 | 1.00x |
| users.ndjson | orjson | 18.525 | 19.351 | 20.798 | 49.582 | 0.66x |
| users.ndjson | msgspec | 17.963 | 18.958 | 20.487 | 49.582 | 0.67x |
| users.ndjson | ujson | 26.330 | 27.615 | 36.584 | 49.582 | 0.46x |
| users.ndjson | json | 31.485 | 32.743 | 40.887 | 49.582 | 0.39x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.405 | 4.578 | 6.600 | 53.723 | 1.00x |
| users.json | orjson | 4.767 | 4.904 | 7.273 | 53.723 | 0.93x |
| users.json | msgspec | 6.459 | 6.765 | 10.291 | 53.723 | 0.68x |
| users.json | ujson | 25.510 | 25.954 | 38.687 | 53.723 | 0.18x |
| users.json | json | 32.516 | 33.024 | 40.056 | 53.723 | 0.14x |
| flat.json | strata | 0.707 | 0.749 | 1.007 | 48.988 | 1.00x |
| flat.json | orjson | 0.739 | 0.774 | 1.043 | 48.988 | 0.97x |
| flat.json | msgspec | 0.918 | 1.000 | 1.786 | 48.988 | 0.75x |
| flat.json | ujson | 3.052 | 3.157 | 4.840 | 48.988 | 0.24x |
| flat.json | json | 3.372 | 3.669 | 5.930 | 48.988 | 0.20x |
| nested.json | strata | 0.640 | 0.672 | 0.720 | 49.078 | 1.00x |
| nested.json | orjson | 0.685 | 0.721 | 0.760 | 49.078 | 0.93x |
| nested.json | msgspec | 0.868 | 0.889 | 0.939 | 49.078 | 0.76x |
| nested.json | ujson | 2.407 | 2.447 | 2.500 | 49.078 | 0.27x |
| nested.json | json | 3.525 | 3.595 | 3.717 | 49.078 | 0.19x |
| wide_arrays.json | strata | 3.076 | 3.232 | 4.522 | 50.367 | 1.00x |
| wide_arrays.json | orjson | 3.382 | 3.439 | 3.762 | 50.367 | 0.94x |
| wide_arrays.json | msgspec | 5.177 | 5.304 | 5.741 | 50.367 | 0.61x |
| wide_arrays.json | ujson | 16.642 | 17.083 | 18.075 | 50.367 | 0.19x |
| wide_arrays.json | json | 24.810 | 25.437 | 38.685 | 50.367 | 0.13x |
| mixed.json | strata | 0.383 | 0.412 | 0.519 | 48.566 | 1.00x |
| mixed.json | orjson | 0.384 | 0.409 | 0.451 | 48.566 | 1.01x |
| mixed.json | msgspec | 0.414 | 0.429 | 0.486 | 48.566 | 0.96x |
| mixed.json | ujson | 0.818 | 0.848 | 0.914 | 48.566 | 0.49x |
| mixed.json | json | 1.004 | 1.049 | 1.195 | 48.566 | 0.39x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.095 | 0.099 | 0.105 | 53.738 | 1.00x |
| users.json $[*].id | jmespath | 0.451 | 0.462 | 0.474 | 53.738 | 0.21x |
| users.json $[*].id | jsonpath-ng | 2.355 | 2.511 | 2.776 | 53.738 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.503 | 0.535 | 1.011 | 53.824 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.714 | 2.765 | 2.830 | 53.824 | 0.19x |
| users.json $[*].orders[*].total | jsonpath-ng | 16.171 | 16.963 | 29.892 | 53.824 | 0.03x |
| users.json $..total | strata | 1.835 | 1.859 | 3.299 | 53.824 | 1.00x |
| users.json $..total | jsonpath-ng | 325.546 | 343.853 | 921.800 | 53.824 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.322 | 4.366 | 4.922 | 53.742 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.515 | 16.117 | 23.441 | 53.742 | 0.27x |
| users.json $[*].id | orjson+jsonpath-ng | 17.676 | 18.186 | 26.002 | 53.742 | 0.24x |
| users.json $[*].orders[*].total | strata | 4.649 | 4.703 | 6.835 | 53.824 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.523 | 19.174 | 20.040 | 53.824 | 0.25x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 35.902 | 38.354 | 49.138 | 53.824 | 0.12x |
| users.json $..total | strata | 17.597 | 19.833 | 32.550 | 53.449 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 409.710 | 488.047 | 515.890 | 53.449 | 0.04x |

