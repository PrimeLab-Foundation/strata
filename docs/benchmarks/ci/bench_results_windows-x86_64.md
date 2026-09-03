# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: e6c5e36
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
| users.json | strata | 18.035 | 19.530 | 68.188 | 45.586 | 1.00x |
| users.json | orjson | 23.223 | 25.164 | 132.017 | 45.586 | 0.78x |
| users.json | msgspec | 21.392 | 24.342 | 31.940 | 45.586 | 0.80x |
| users.json | ujson | 36.535 | 40.006 | 53.910 | 45.586 | 0.49x |
| users.json | json | 38.426 | 41.755 | 70.240 | 45.586 | 0.47x |
| flat.json | strata | 1.160 | 1.270 | 2.990 | 50.480 | 1.00x |
| flat.json | orjson | 1.241 | 1.285 | 2.139 | 50.480 | 0.99x |
| flat.json | msgspec | 1.219 | 1.305 | 2.455 | 50.480 | 0.97x |
| flat.json | ujson | 2.234 | 2.377 | 5.387 | 50.480 | 0.53x |
| flat.json | json | 2.024 | 2.134 | 3.671 | 50.480 | 0.60x |
| nested.json | strata | 0.798 | 0.845 | 0.957 | 50.348 | 1.00x |
| nested.json | orjson | 1.127 | 1.139 | 1.535 | 50.348 | 0.74x |
| nested.json | msgspec | 0.988 | 1.045 | 1.254 | 50.348 | 0.81x |
| nested.json | ujson | 1.558 | 1.609 | 1.629 | 50.348 | 0.53x |
| nested.json | json | 2.164 | 2.182 | 2.889 | 50.348 | 0.39x |
| wide_arrays.json | strata | 4.930 | 5.065 | 7.407 | 52.320 | 1.00x |
| wide_arrays.json | orjson | 6.335 | 6.508 | 9.742 | 52.320 | 0.78x |
| wide_arrays.json | msgspec | 6.146 | 6.278 | 10.014 | 52.320 | 0.81x |
| wide_arrays.json | ujson | 8.435 | 8.514 | 15.020 | 52.320 | 0.59x |
| wide_arrays.json | json | 11.880 | 12.132 | 25.167 | 52.320 | 0.42x |
| mixed.json | strata | 0.205 | 0.322 | 0.497 | 50.180 | 1.00x |
| mixed.json | orjson | 0.225 | 0.269 | 0.410 | 50.180 | 1.20x |
| mixed.json | msgspec | 0.233 | 0.240 | 0.408 | 50.180 | 1.34x |
| mixed.json | ujson | 0.351 | 0.385 | 0.580 | 50.180 | 0.84x |
| mixed.json | json | 0.479 | 0.497 | 0.658 | 50.180 | 0.65x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.923 | 5.228 | 13.498 | 44.859 | 1.00x |
| users.json | orjson | 5.693 | 6.395 | 8.776 | 44.859 | 0.82x |
| users.json | msgspec | 8.099 | 8.819 | 11.472 | 44.859 | 0.59x |
| users.json | ujson | 25.842 | 26.939 | 33.318 | 44.859 | 0.19x |
| users.json | json | 40.514 | 44.331 | 91.442 | 44.859 | 0.12x |
| flat.json | strata | 0.333 | 0.351 | 0.465 | 50.648 | 1.00x |
| flat.json | orjson | 0.377 | 0.385 | 0.416 | 50.648 | 0.91x |
| flat.json | msgspec | 0.551 | 0.564 | 0.642 | 50.648 | 0.62x |
| flat.json | ujson | 1.656 | 1.747 | 1.942 | 50.648 | 0.20x |
| flat.json | json | 1.984 | 2.149 | 2.231 | 50.648 | 0.16x |
| nested.json | strata | 0.284 | 0.287 | 0.314 | 50.949 | 1.00x |
| nested.json | orjson | 0.319 | 0.326 | 0.373 | 50.949 | 0.88x |
| nested.json | msgspec | 0.505 | 0.532 | 0.591 | 50.949 | 0.54x |
| nested.json | ujson | 1.384 | 1.473 | 1.551 | 50.949 | 0.19x |
| nested.json | json | 2.590 | 2.706 | 4.260 | 50.949 | 0.11x |
| wide_arrays.json | strata | 2.401 | 2.533 | 3.478 | 53.730 | 1.00x |
| wide_arrays.json | orjson | 2.581 | 2.803 | 3.516 | 53.730 | 0.90x |
| wide_arrays.json | msgspec | 4.070 | 4.429 | 4.622 | 53.730 | 0.57x |
| wide_arrays.json | ujson | 10.036 | 10.190 | 10.515 | 53.730 | 0.25x |
| wide_arrays.json | json | 18.163 | 18.447 | 30.399 | 53.730 | 0.14x |
| mixed.json | strata | 0.079 | 0.081 | 0.100 | 50.266 | 1.00x |
| mixed.json | orjson | 0.072 | 0.074 | 0.116 | 50.266 | 1.09x |
| mixed.json | msgspec | 0.097 | 0.102 | 0.122 | 50.266 | 0.79x |
| mixed.json | ujson | 0.330 | 0.333 | 0.504 | 50.266 | 0.24x |
| mixed.json | json | 0.511 | 0.545 | 0.881 | 50.266 | 0.15x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 20.023 | 21.096 | 43.433 | 54.887 | 1.00x |
| users.json | orjson | 24.169 | 26.481 | 80.815 | 54.887 | 0.80x |
| users.json | msgspec | 24.255 | 25.090 | 27.065 | 54.887 | 0.84x |
| users.json | ujson | 42.322 | 44.648 | 65.207 | 54.887 | 0.47x |
| users.json | json | 31.014 | 41.363 | 44.095 | 54.887 | 0.51x |
| flat.json | strata | 1.431 | 1.549 | 2.127 | 50.234 | 1.00x |
| flat.json | orjson | 1.558 | 1.646 | 1.984 | 50.234 | 0.94x |
| flat.json | msgspec | 1.283 | 1.396 | 1.675 | 50.234 | 1.11x |
| flat.json | ujson | 2.771 | 3.041 | 3.853 | 50.234 | 0.51x |
| flat.json | json | 2.327 | 2.371 | 2.706 | 50.234 | 0.65x |
| nested.json | strata | 0.917 | 0.997 | 1.159 | 50.953 | 1.00x |
| nested.json | orjson | 1.262 | 1.320 | 1.468 | 50.953 | 0.76x |
| nested.json | msgspec | 1.091 | 1.125 | 1.217 | 50.953 | 0.89x |
| nested.json | ujson | 1.862 | 1.917 | 2.007 | 50.953 | 0.52x |
| nested.json | json | 2.223 | 2.352 | 2.654 | 50.953 | 0.42x |
| wide_arrays.json | strata | 5.548 | 5.695 | 5.934 | 53.730 | 1.00x |
| wide_arrays.json | orjson | 6.765 | 6.916 | 7.368 | 53.730 | 0.82x |
| wide_arrays.json | msgspec | 6.544 | 6.917 | 7.113 | 53.730 | 0.82x |
| wide_arrays.json | ujson | 11.039 | 11.237 | 17.060 | 53.730 | 0.51x |
| wide_arrays.json | json | 12.306 | 12.652 | 21.871 | 53.730 | 0.45x |
| mixed.json | strata | 0.289 | 0.327 | 0.449 | 50.184 | 1.00x |
| mixed.json | orjson | 0.343 | 0.431 | 0.893 | 50.184 | 0.76x |
| mixed.json | msgspec | 0.342 | 0.364 | 0.662 | 50.184 | 0.90x |
| mixed.json | ujson | 0.533 | 0.563 | 1.164 | 50.184 | 0.58x |
| mixed.json | json | 0.578 | 0.660 | 0.901 | 50.184 | 0.50x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 12.574 | 12.967 | 13.802 | 51.375 | 1.00x |
| users.ndjson | orjson | 19.565 | 20.110 | 23.318 | 51.375 | 0.64x |
| users.ndjson | msgspec | 18.761 | 19.622 | 30.345 | 51.375 | 0.66x |
| users.ndjson | ujson | 27.303 | 27.856 | 40.200 | 51.375 | 0.47x |
| users.ndjson | json | 31.977 | 32.377 | 35.012 | 51.375 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.592 | 4.731 | 6.524 | 50.930 | 1.00x |
| users.json | orjson | 4.825 | 5.133 | 7.712 | 50.930 | 0.92x |
| users.json | msgspec | 6.504 | 6.860 | 10.360 | 50.930 | 0.69x |
| users.json | ujson | 25.879 | 26.774 | 63.325 | 50.930 | 0.18x |
| users.json | json | 33.267 | 39.603 | 70.246 | 50.930 | 0.12x |
| flat.json | strata | 0.695 | 0.737 | 0.994 | 51.078 | 1.00x |
| flat.json | orjson | 0.728 | 0.787 | 1.179 | 51.078 | 0.94x |
| flat.json | msgspec | 0.908 | 1.003 | 1.252 | 51.078 | 0.73x |
| flat.json | ujson | 3.214 | 3.382 | 9.181 | 51.078 | 0.22x |
| flat.json | json | 3.411 | 4.313 | 7.403 | 51.078 | 0.17x |
| nested.json | strata | 0.638 | 0.680 | 0.761 | 50.953 | 1.00x |
| nested.json | orjson | 0.704 | 0.765 | 3.715 | 50.953 | 0.89x |
| nested.json | msgspec | 0.884 | 0.915 | 1.018 | 50.953 | 0.74x |
| nested.json | ujson | 2.428 | 2.503 | 3.378 | 50.953 | 0.27x |
| nested.json | json | 3.609 | 3.732 | 3.809 | 50.953 | 0.18x |
| wide_arrays.json | strata | 3.172 | 3.330 | 5.868 | 53.730 | 1.00x |
| wide_arrays.json | orjson | 3.334 | 3.557 | 5.920 | 53.730 | 0.94x |
| wide_arrays.json | msgspec | 4.811 | 5.441 | 7.954 | 53.730 | 0.61x |
| wide_arrays.json | ujson | 17.154 | 17.488 | 23.094 | 53.730 | 0.19x |
| wide_arrays.json | json | 25.186 | 25.939 | 43.415 | 53.730 | 0.13x |
| mixed.json | strata | 0.381 | 0.411 | 0.552 | 50.266 | 1.00x |
| mixed.json | orjson | 0.383 | 0.403 | 0.462 | 50.266 | 1.02x |
| mixed.json | msgspec | 0.414 | 0.446 | 0.518 | 50.266 | 0.92x |
| mixed.json | ujson | 0.813 | 0.870 | 0.964 | 50.266 | 0.47x |
| mixed.json | json | 1.009 | 1.054 | 1.201 | 50.266 | 0.39x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.097 | 0.101 | 0.149 | 50.949 | 1.00x |
| users.json $[*].id | jmespath | 0.413 | 0.438 | 0.631 | 50.949 | 0.23x |
| users.json $[*].id | jsonpath-ng | 2.530 | 2.763 | 2.903 | 50.949 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.531 | 0.608 | 0.650 | 51.957 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.716 | 2.900 | 2.967 | 51.957 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 18.015 | 18.734 | 35.730 | 51.957 | 0.03x |
| users.json $..total | strata | 1.856 | 1.889 | 3.611 | 53.961 | 1.00x |
| users.json $..total | jsonpath-ng | 337.658 | 358.475 | 383.983 | 53.961 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.423 | 4.697 | 7.818 | 51.957 | 1.00x |
| users.json $[*].id | orjson+jmespath | 16.990 | 18.202 | 113.964 | 51.957 | 0.26x |
| users.json $[*].id | orjson+jsonpath-ng | 19.419 | 21.367 | 38.017 | 51.957 | 0.22x |
| users.json $[*].orders[*].total | strata | 4.728 | 4.857 | 4.915 | 53.582 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 19.578 | 20.535 | 34.473 | 53.582 | 0.24x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 38.860 | 40.235 | 57.127 | 53.582 | 0.12x |
| users.json $..total | strata | 14.922 | 15.893 | 16.370 | 53.586 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 360.544 | 380.120 | 401.147 | 53.586 | 0.04x |

