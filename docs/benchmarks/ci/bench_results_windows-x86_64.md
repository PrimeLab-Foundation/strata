# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 1e1e47c
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
| users.json | strata | 9.627 | 9.915 | 12.712 | 45.551 | 1.00x |
| users.json | orjson | 13.064 | 13.310 | 19.933 | 45.551 | 0.74x |
| users.json | msgspec | 12.557 | 12.978 | 21.481 | 45.551 | 0.76x |
| users.json | ujson | 20.977 | 22.072 | 35.349 | 45.551 | 0.45x |
| users.json | json | 22.068 | 22.412 | 24.738 | 45.551 | 0.44x |
| flat.json | strata | 0.940 | 1.010 | 1.739 | 50.270 | 1.00x |
| flat.json | orjson | 1.162 | 1.288 | 1.857 | 50.270 | 0.78x |
| flat.json | msgspec | 1.150 | 1.214 | 2.057 | 50.270 | 0.83x |
| flat.json | ujson | 2.353 | 2.499 | 3.830 | 50.270 | 0.40x |
| flat.json | json | 1.991 | 2.006 | 4.141 | 50.270 | 0.50x |
| nested.json | strata | 0.829 | 0.862 | 0.887 | 49.625 | 1.00x |
| nested.json | orjson | 1.098 | 1.141 | 1.175 | 49.625 | 0.76x |
| nested.json | msgspec | 1.013 | 1.058 | 1.091 | 49.625 | 0.81x |
| nested.json | ujson | 1.745 | 1.848 | 2.289 | 49.625 | 0.47x |
| nested.json | json | 2.153 | 2.170 | 2.242 | 49.625 | 0.40x |
| wide_arrays.json | strata | 4.632 | 4.709 | 4.994 | 51.902 | 1.00x |
| wide_arrays.json | orjson | 5.578 | 5.639 | 5.902 | 51.902 | 0.84x |
| wide_arrays.json | msgspec | 5.838 | 5.889 | 7.234 | 51.902 | 0.80x |
| wide_arrays.json | ujson | 8.172 | 8.345 | 8.764 | 51.902 | 0.56x |
| wide_arrays.json | json | 11.596 | 11.679 | 12.792 | 51.902 | 0.40x |
| mixed.json | strata | 0.199 | 0.204 | 0.334 | 51.875 | 1.00x |
| mixed.json | orjson | 0.219 | 0.225 | 0.264 | 51.875 | 0.91x |
| mixed.json | msgspec | 0.239 | 0.264 | 0.438 | 51.875 | 0.77x |
| mixed.json | ujson | 0.366 | 0.393 | 0.425 | 51.875 | 0.52x |
| mixed.json | json | 0.476 | 0.503 | 0.532 | 51.875 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.329 | 3.417 | 5.115 | 44.695 | 1.00x |
| users.json | orjson | 3.782 | 4.030 | 5.798 | 44.695 | 0.85x |
| users.json | msgspec | 5.504 | 5.876 | 7.747 | 44.695 | 0.58x |
| users.json | ujson | 16.228 | 17.091 | 18.238 | 44.695 | 0.20x |
| users.json | json | 23.374 | 24.973 | 43.343 | 44.695 | 0.14x |
| flat.json | strata | 0.332 | 0.361 | 0.417 | 50.004 | 1.00x |
| flat.json | orjson | 0.370 | 0.380 | 0.425 | 50.004 | 0.95x |
| flat.json | msgspec | 0.525 | 0.546 | 0.603 | 50.004 | 0.66x |
| flat.json | ujson | 1.747 | 1.786 | 1.953 | 50.004 | 0.20x |
| flat.json | json | 1.970 | 2.014 | 2.597 | 50.004 | 0.18x |
| nested.json | strata | 0.298 | 0.302 | 0.338 | 50.023 | 1.00x |
| nested.json | orjson | 0.336 | 0.349 | 0.368 | 50.023 | 0.87x |
| nested.json | msgspec | 0.477 | 0.489 | 0.519 | 50.023 | 0.62x |
| nested.json | ujson | 1.283 | 1.343 | 1.421 | 50.023 | 0.23x |
| nested.json | json | 2.462 | 2.490 | 4.855 | 50.023 | 0.12x |
| wide_arrays.json | strata | 2.248 | 2.312 | 2.485 | 50.711 | 1.00x |
| wide_arrays.json | orjson | 2.270 | 2.458 | 2.712 | 50.711 | 0.94x |
| wide_arrays.json | msgspec | 3.387 | 3.595 | 3.786 | 50.711 | 0.64x |
| wide_arrays.json | ujson | 9.383 | 9.605 | 16.429 | 50.711 | 0.24x |
| wide_arrays.json | json | 18.187 | 18.351 | 28.530 | 50.711 | 0.13x |
| mixed.json | strata | 0.077 | 0.081 | 0.095 | 49.793 | 1.00x |
| mixed.json | orjson | 0.069 | 0.073 | 0.191 | 49.793 | 1.12x |
| mixed.json | msgspec | 0.091 | 0.095 | 0.125 | 49.793 | 0.85x |
| mixed.json | ujson | 0.324 | 0.330 | 0.378 | 49.793 | 0.25x |
| mixed.json | json | 0.506 | 0.547 | 0.652 | 49.793 | 0.15x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.984 | 11.345 | 13.325 | 54.723 | 1.00x |
| users.json | orjson | 14.295 | 14.588 | 16.674 | 54.723 | 0.78x |
| users.json | msgspec | 13.753 | 13.947 | 15.114 | 54.723 | 0.81x |
| users.json | ujson | 25.616 | 28.296 | 33.848 | 54.723 | 0.40x |
| users.json | json | 23.074 | 24.059 | 38.794 | 54.723 | 0.47x |
| flat.json | strata | 1.299 | 1.346 | 1.446 | 49.148 | 1.00x |
| flat.json | orjson | 1.293 | 1.426 | 1.506 | 49.148 | 0.94x |
| flat.json | msgspec | 1.376 | 1.436 | 1.491 | 49.148 | 0.94x |
| flat.json | ujson | 2.944 | 3.153 | 3.270 | 49.148 | 0.43x |
| flat.json | json | 2.278 | 2.377 | 2.479 | 49.148 | 0.57x |
| nested.json | strata | 0.923 | 0.983 | 1.383 | 49.949 | 1.00x |
| nested.json | orjson | 1.250 | 1.369 | 1.707 | 49.949 | 0.72x |
| nested.json | msgspec | 1.161 | 1.214 | 1.346 | 49.949 | 0.81x |
| nested.json | ujson | 2.153 | 2.253 | 2.900 | 49.949 | 0.44x |
| nested.json | json | 2.306 | 2.361 | 2.448 | 49.949 | 0.42x |
| wide_arrays.json | strata | 5.171 | 5.536 | 6.101 | 50.711 | 1.00x |
| wide_arrays.json | orjson | 6.151 | 6.518 | 9.923 | 50.711 | 0.85x |
| wide_arrays.json | msgspec | 6.433 | 6.730 | 11.692 | 50.711 | 0.82x |
| wide_arrays.json | ujson | 11.405 | 11.694 | 14.545 | 50.711 | 0.47x |
| wide_arrays.json | json | 12.222 | 12.506 | 13.427 | 50.711 | 0.44x |
| mixed.json | strata | 0.282 | 0.293 | 0.431 | 49.746 | 1.00x |
| mixed.json | orjson | 0.329 | 0.386 | 0.429 | 49.746 | 0.76x |
| mixed.json | msgspec | 0.349 | 0.384 | 0.641 | 49.746 | 0.76x |
| mixed.json | ujson | 0.544 | 0.594 | 0.714 | 49.746 | 0.49x |
| mixed.json | json | 0.585 | 0.604 | 0.650 | 49.746 | 0.48x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.890 | 11.137 | 20.381 | 50.695 | 1.00x |
| users.ndjson | orjson | 16.737 | 17.471 | 27.532 | 50.695 | 0.64x |
| users.ndjson | msgspec | 16.778 | 17.505 | 18.547 | 50.695 | 0.64x |
| users.ndjson | ujson | 24.938 | 25.904 | 27.997 | 50.695 | 0.43x |
| users.ndjson | json | 29.745 | 30.590 | 48.982 | 50.695 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.119 | 4.180 | 5.990 | 50.172 | 1.00x |
| users.json | orjson | 4.695 | 4.763 | 6.909 | 50.172 | 0.88x |
| users.json | msgspec | 5.982 | 6.192 | 9.496 | 50.172 | 0.68x |
| users.json | ujson | 25.607 | 26.070 | 44.549 | 50.172 | 0.16x |
| users.json | json | 32.062 | 32.705 | 33.590 | 50.172 | 0.13x |
| flat.json | strata | 0.762 | 0.844 | 1.500 | 50.223 | 1.00x |
| flat.json | orjson | 0.772 | 0.843 | 1.151 | 50.223 | 1.00x |
| flat.json | msgspec | 0.922 | 1.040 | 1.738 | 50.223 | 0.81x |
| flat.json | ujson | 3.104 | 3.262 | 5.309 | 50.223 | 0.26x |
| flat.json | json | 3.425 | 3.502 | 6.268 | 50.223 | 0.24x |
| nested.json | strata | 0.631 | 0.662 | 0.780 | 50.168 | 1.00x |
| nested.json | orjson | 0.665 | 0.699 | 0.821 | 50.168 | 0.95x |
| nested.json | msgspec | 0.819 | 0.864 | 0.958 | 50.168 | 0.77x |
| nested.json | ujson | 2.383 | 2.434 | 2.481 | 50.168 | 0.27x |
| nested.json | json | 3.477 | 3.558 | 3.646 | 50.168 | 0.19x |
| wide_arrays.json | strata | 2.985 | 3.071 | 4.862 | 52.984 | 1.00x |
| wide_arrays.json | orjson | 2.975 | 3.182 | 4.389 | 52.984 | 0.97x |
| wide_arrays.json | msgspec | 4.167 | 4.286 | 7.313 | 52.984 | 0.72x |
| wide_arrays.json | ujson | 16.426 | 16.703 | 23.068 | 52.984 | 0.18x |
| wide_arrays.json | json | 25.106 | 25.943 | 38.304 | 52.984 | 0.12x |
| mixed.json | strata | 0.385 | 0.402 | 0.599 | 49.824 | 1.00x |
| mixed.json | orjson | 0.378 | 0.433 | 0.993 | 49.824 | 0.93x |
| mixed.json | msgspec | 0.402 | 0.449 | 0.778 | 49.824 | 0.89x |
| mixed.json | ujson | 0.835 | 0.883 | 2.216 | 49.824 | 0.46x |
| mixed.json | json | 0.989 | 1.043 | 3.468 | 49.824 | 0.39x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.085 | 0.091 | 0.192 | 50.234 | 1.00x |
| users.json $[*].id | jmespath | 0.433 | 0.447 | 1.069 | 50.234 | 0.20x |
| users.json $[*].id | jsonpath-ng | 2.477 | 2.702 | 5.982 | 50.234 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.465 | 0.500 | 0.813 | 50.621 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.733 | 2.769 | 5.275 | 50.621 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 16.680 | 17.397 | 21.217 | 50.621 | 0.03x |
| users.json $..total | strata | 1.825 | 2.002 | 50.100 | 52.625 | 1.00x |
| users.json $..total | jsonpath-ng | 422.069 | 507.987 | 775.932 | 52.625 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.039 | 4.123 | 6.004 | 50.266 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.633 | 15.046 | 17.951 | 50.266 | 0.27x |
| users.json $[*].id | orjson+jsonpath-ng | 16.589 | 17.540 | 22.011 | 50.266 | 0.24x |
| users.json $[*].orders[*].total | strata | 4.293 | 4.380 | 5.823 | 52.621 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.124 | 17.549 | 28.538 | 52.621 | 0.25x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 34.071 | 35.273 | 40.345 | 52.621 | 0.12x |
| users.json $..total | strata | 14.009 | 19.618 | 22.944 | 52.250 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 350.505 | 416.344 | 616.014 | 52.250 | 0.05x |

