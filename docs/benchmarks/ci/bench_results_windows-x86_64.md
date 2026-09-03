# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 68d6e74
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: Intel64 Family 6 Model 207 Stepping 2, GenuineIntel
- compiler_flags: /std:c++20 /O2 /arch:AVX2 /GL /LTCG /USEPROFILE (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.290 | 8.601 | 11.959 | 45.371 | 1.00x |
| users.json | orjson | 12.810 | 13.435 | 16.477 | 45.371 | 0.64x |
| users.json | msgspec | 11.973 | 12.208 | 16.802 | 45.371 | 0.70x |
| users.json | ujson | 17.373 | 18.577 | 26.560 | 45.371 | 0.46x |
| users.json | json | 19.346 | 20.654 | 25.047 | 45.371 | 0.42x |
| flat.json | strata | 1.098 | 1.145 | 1.321 | 48.309 | 1.00x |
| flat.json | orjson | 1.097 | 1.138 | 1.244 | 48.309 | 1.01x |
| flat.json | msgspec | 1.015 | 1.060 | 1.154 | 48.309 | 1.08x |
| flat.json | ujson | 1.463 | 1.529 | 1.715 | 48.309 | 0.75x |
| flat.json | json | 1.668 | 1.703 | 1.917 | 48.309 | 0.67x |
| nested.json | strata | 0.650 | 0.691 | 1.142 | 48.270 | 1.00x |
| nested.json | orjson | 0.855 | 0.898 | 1.612 | 48.270 | 0.77x |
| nested.json | msgspec | 0.762 | 0.809 | 1.284 | 48.270 | 0.85x |
| nested.json | ujson | 1.128 | 1.219 | 2.071 | 48.270 | 0.57x |
| nested.json | json | 1.655 | 1.750 | 3.008 | 48.270 | 0.40x |
| wide_arrays.json | strata | 3.884 | 3.921 | 4.647 | 50.535 | 1.00x |
| wide_arrays.json | orjson | 5.371 | 5.524 | 5.854 | 50.535 | 0.71x |
| wide_arrays.json | msgspec | 5.078 | 5.195 | 7.945 | 50.535 | 0.75x |
| wide_arrays.json | ujson | 6.950 | 7.139 | 11.033 | 50.535 | 0.55x |
| wide_arrays.json | json | 9.820 | 10.237 | 17.118 | 50.535 | 0.38x |
| mixed.json | strata | 0.166 | 0.171 | 0.189 | 48.562 | 1.00x |
| mixed.json | orjson | 0.181 | 0.188 | 0.228 | 48.562 | 0.90x |
| mixed.json | msgspec | 0.206 | 0.216 | 0.262 | 48.562 | 0.79x |
| mixed.json | ujson | 0.269 | 0.286 | 0.309 | 48.562 | 0.60x |
| mixed.json | json | 0.403 | 0.445 | 0.469 | 48.562 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.509 | 2.844 | 4.997 | 44.609 | 1.00x |
| users.json | orjson | 3.089 | 3.144 | 4.677 | 44.609 | 0.90x |
| users.json | msgspec | 5.101 | 5.584 | 9.567 | 44.609 | 0.51x |
| users.json | ujson | 13.515 | 14.996 | 18.127 | 44.609 | 0.19x |
| users.json | json | 20.527 | 22.820 | 34.956 | 44.609 | 0.12x |
| flat.json | strata | 0.298 | 0.377 | 0.440 | 48.500 | 1.00x |
| flat.json | orjson | 0.291 | 0.309 | 0.364 | 48.500 | 1.22x |
| flat.json | msgspec | 0.465 | 0.564 | 0.633 | 48.500 | 0.67x |
| flat.json | ujson | 1.092 | 1.214 | 1.392 | 48.500 | 0.31x |
| flat.json | json | 1.589 | 1.836 | 2.016 | 48.500 | 0.21x |
| nested.json | strata | 0.195 | 0.206 | 0.248 | 48.973 | 1.00x |
| nested.json | orjson | 0.272 | 0.282 | 0.343 | 48.973 | 0.73x |
| nested.json | msgspec | 0.387 | 0.413 | 0.521 | 48.973 | 0.50x |
| nested.json | ujson | 1.040 | 1.123 | 1.464 | 48.973 | 0.18x |
| nested.json | json | 1.936 | 1.993 | 2.653 | 48.973 | 0.10x |
| wide_arrays.json | strata | 1.921 | 1.982 | 3.294 | 50.367 | 1.00x |
| wide_arrays.json | orjson | 2.269 | 2.406 | 3.299 | 50.367 | 0.82x |
| wide_arrays.json | msgspec | 3.920 | 4.068 | 5.461 | 50.367 | 0.49x |
| wide_arrays.json | ujson | 8.269 | 8.503 | 9.339 | 50.367 | 0.23x |
| wide_arrays.json | json | 14.814 | 15.528 | 23.802 | 50.367 | 0.13x |
| mixed.json | strata | 0.065 | 0.068 | 0.104 | 48.699 | 1.00x |
| mixed.json | orjson | 0.058 | 0.065 | 0.126 | 48.699 | 1.06x |
| mixed.json | msgspec | 0.083 | 0.095 | 0.139 | 48.699 | 0.72x |
| mixed.json | ujson | 0.241 | 0.247 | 0.359 | 48.699 | 0.28x |
| mixed.json | json | 0.443 | 0.479 | 1.074 | 48.699 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.567 | 10.156 | 13.190 | 54.684 | 1.00x |
| users.json | orjson | 13.990 | 14.536 | 20.822 | 54.684 | 0.70x |
| users.json | msgspec | 12.936 | 13.697 | 22.189 | 54.684 | 0.74x |
| users.json | ujson | 20.977 | 23.619 | 30.153 | 54.684 | 0.43x |
| users.json | json | 20.467 | 21.693 | 24.301 | 54.684 | 0.47x |
| flat.json | strata | 1.361 | 1.514 | 1.962 | 47.895 | 1.00x |
| flat.json | orjson | 1.498 | 1.632 | 2.263 | 47.895 | 0.93x |
| flat.json | msgspec | 1.187 | 1.314 | 1.693 | 47.895 | 1.15x |
| flat.json | ujson | 2.082 | 2.218 | 3.295 | 47.895 | 0.68x |
| flat.json | json | 2.098 | 2.276 | 3.866 | 47.895 | 0.66x |
| nested.json | strata | 0.708 | 0.773 | 0.917 | 49.070 | 1.00x |
| nested.json | orjson | 0.995 | 1.068 | 1.737 | 49.070 | 0.72x |
| nested.json | msgspec | 0.848 | 0.924 | 1.036 | 49.070 | 0.84x |
| nested.json | ujson | 1.410 | 1.514 | 1.598 | 49.070 | 0.51x |
| nested.json | json | 1.710 | 1.809 | 1.964 | 49.070 | 0.43x |
| wide_arrays.json | strata | 4.554 | 4.812 | 5.035 | 50.367 | 1.00x |
| wide_arrays.json | orjson | 5.997 | 6.312 | 8.384 | 50.367 | 0.76x |
| wide_arrays.json | msgspec | 5.772 | 6.035 | 8.836 | 50.367 | 0.80x |
| wide_arrays.json | ujson | 9.242 | 9.720 | 15.038 | 50.367 | 0.50x |
| wide_arrays.json | json | 10.448 | 10.685 | 11.045 | 50.367 | 0.45x |
| mixed.json | strata | 0.260 | 0.285 | 0.462 | 48.859 | 1.00x |
| mixed.json | orjson | 0.305 | 0.337 | 0.466 | 48.859 | 0.85x |
| mixed.json | msgspec | 0.333 | 0.365 | 0.551 | 48.859 | 0.78x |
| mixed.json | ujson | 0.442 | 0.470 | 0.809 | 48.859 | 0.61x |
| mixed.json | json | 0.522 | 0.573 | 0.973 | 48.859 | 0.50x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.501 | 10.048 | 11.724 | 49.527 | 1.00x |
| users.ndjson | orjson | 15.211 | 15.834 | 16.650 | 49.527 | 0.63x |
| users.ndjson | msgspec | 14.947 | 15.441 | 18.709 | 49.527 | 0.65x |
| users.ndjson | ujson | 20.283 | 21.006 | 32.707 | 49.527 | 0.48x |
| users.ndjson | json | 25.719 | 26.531 | 44.349 | 49.527 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.569 | 3.793 | 5.423 | 51.918 | 1.00x |
| users.json | orjson | 4.055 | 4.671 | 6.496 | 51.918 | 0.81x |
| users.json | msgspec | 6.062 | 7.598 | 10.025 | 51.918 | 0.50x |
| users.json | ujson | 19.753 | 23.343 | 48.368 | 51.918 | 0.16x |
| users.json | json | 27.439 | 33.467 | 67.819 | 51.918 | 0.11x |
| flat.json | strata | 0.636 | 0.708 | 0.818 | 48.637 | 1.00x |
| flat.json | orjson | 0.635 | 0.684 | 0.765 | 48.637 | 1.03x |
| flat.json | msgspec | 0.818 | 0.877 | 1.000 | 48.637 | 0.81x |
| flat.json | ujson | 2.202 | 2.328 | 2.610 | 48.637 | 0.30x |
| flat.json | json | 2.672 | 2.863 | 3.149 | 48.637 | 0.25x |
| nested.json | strata | 0.537 | 0.593 | 0.903 | 49.199 | 1.00x |
| nested.json | orjson | 0.628 | 0.677 | 0.942 | 49.199 | 0.88x |
| nested.json | msgspec | 0.742 | 0.805 | 1.052 | 49.199 | 0.74x |
| nested.json | ujson | 1.912 | 1.978 | 3.188 | 49.199 | 0.30x |
| nested.json | json | 2.794 | 2.995 | 5.198 | 49.199 | 0.20x |
| wide_arrays.json | strata | 2.728 | 2.813 | 3.702 | 50.383 | 1.00x |
| wide_arrays.json | orjson | 3.069 | 3.222 | 4.063 | 50.383 | 0.87x |
| wide_arrays.json | msgspec | 4.689 | 4.801 | 6.982 | 50.383 | 0.59x |
| wide_arrays.json | ujson | 13.426 | 13.872 | 19.931 | 50.383 | 0.20x |
| wide_arrays.json | json | 19.781 | 20.107 | 22.383 | 50.383 | 0.14x |
| mixed.json | strata | 0.351 | 0.393 | 0.469 | 48.891 | 1.00x |
| mixed.json | orjson | 0.347 | 0.394 | 2.325 | 48.891 | 1.00x |
| mixed.json | msgspec | 0.377 | 0.400 | 0.526 | 48.891 | 0.98x |
| mixed.json | ujson | 0.641 | 0.701 | 1.043 | 48.891 | 0.56x |
| mixed.json | json | 0.853 | 0.924 | 1.477 | 48.891 | 0.42x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.073 | 0.081 | 0.096 | 51.938 | 1.00x |
| users.json $[*].id | jmespath | 0.345 | 0.350 | 0.442 | 51.938 | 0.23x |
| users.json $[*].id | jsonpath-ng | 1.885 | 2.009 | 2.185 | 51.938 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.435 | 0.441 | 0.593 | 51.941 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.161 | 2.261 | 4.024 | 51.941 | 0.19x |
| users.json $[*].orders[*].total | jsonpath-ng | 12.584 | 13.098 | 18.963 | 51.941 | 0.03x |
| users.json $..total | strata | 1.592 | 1.626 | 2.568 | 52.941 | 1.00x |
| users.json $..total | jsonpath-ng | 257.483 | 264.451 | 284.085 | 52.941 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.640 | 3.887 | 5.300 | 51.941 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.193 | 14.510 | 21.333 | 51.941 | 0.27x |
| users.json $[*].id | orjson+jsonpath-ng | 15.691 | 15.931 | 17.146 | 51.941 | 0.24x |
| users.json $[*].orders[*].total | strata | 3.952 | 4.141 | 5.095 | 52.941 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 16.080 | 17.343 | 24.862 | 52.941 | 0.24x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 32.022 | 32.829 | 46.802 | 52.941 | 0.13x |
| users.json $..total | strata | 12.210 | 13.756 | 14.768 | 52.941 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 277.005 | 298.931 | 306.453 | 52.941 | 0.05x |

