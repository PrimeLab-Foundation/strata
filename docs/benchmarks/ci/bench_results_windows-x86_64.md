# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 9b6124a
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
| users.json | strata | 20.544 | 29.911 | 76.297 | 45.566 | 1.00x |
| users.json | orjson | 25.863 | 39.019 | 95.678 | 45.566 | 0.77x |
| users.json | msgspec | 22.360 | 36.553 | 46.405 | 45.566 | 0.82x |
| users.json | ujson | 42.164 | 62.023 | 78.283 | 45.566 | 0.48x |
| users.json | json | 48.920 | 67.510 | 166.392 | 45.566 | 0.44x |
| flat.json | strata | 1.034 | 1.116 | 2.084 | 49.215 | 1.00x |
| flat.json | orjson | 1.177 | 1.288 | 4.036 | 49.215 | 0.87x |
| flat.json | msgspec | 1.140 | 1.232 | 2.001 | 49.215 | 0.91x |
| flat.json | ujson | 2.325 | 2.391 | 2.659 | 49.215 | 0.47x |
| flat.json | json | 1.987 | 2.064 | 3.242 | 49.215 | 0.54x |
| nested.json | strata | 0.809 | 0.862 | 1.202 | 49.508 | 1.00x |
| nested.json | orjson | 1.078 | 1.118 | 1.148 | 49.508 | 0.77x |
| nested.json | msgspec | 1.005 | 1.039 | 1.069 | 49.508 | 0.83x |
| nested.json | ujson | 1.594 | 1.650 | 1.682 | 49.508 | 0.52x |
| nested.json | json | 2.120 | 2.134 | 2.158 | 49.508 | 0.40x |
| wide_arrays.json | strata | 4.800 | 4.869 | 7.875 | 51.000 | 1.00x |
| wide_arrays.json | orjson | 5.801 | 5.885 | 7.818 | 51.000 | 0.83x |
| wide_arrays.json | msgspec | 5.740 | 5.799 | 6.054 | 51.000 | 0.84x |
| wide_arrays.json | ujson | 8.167 | 8.366 | 12.684 | 51.000 | 0.58x |
| wide_arrays.json | json | 11.568 | 11.692 | 21.462 | 51.000 | 0.42x |
| mixed.json | strata | 0.187 | 0.198 | 0.231 | 50.355 | 1.00x |
| mixed.json | orjson | 0.199 | 0.214 | 0.218 | 50.355 | 0.92x |
| mixed.json | msgspec | 0.219 | 0.236 | 0.239 | 50.355 | 0.84x |
| mixed.json | ujson | 0.332 | 0.358 | 0.372 | 50.355 | 0.55x |
| mixed.json | json | 0.467 | 0.479 | 0.498 | 50.355 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.660 | 6.971 | 9.175 | 45.746 | 1.00x |
| users.json | orjson | 5.624 | 7.789 | 8.475 | 45.746 | 0.90x |
| users.json | msgspec | 7.769 | 11.475 | 13.390 | 45.746 | 0.61x |
| users.json | ujson | 29.618 | 41.876 | 138.005 | 45.746 | 0.17x |
| users.json | json | 44.269 | 63.630 | 69.001 | 45.746 | 0.11x |
| flat.json | strata | 0.339 | 0.394 | 0.651 | 49.902 | 1.00x |
| flat.json | orjson | 0.363 | 0.381 | 0.627 | 49.902 | 1.03x |
| flat.json | msgspec | 0.506 | 0.530 | 0.851 | 49.902 | 0.74x |
| flat.json | ujson | 1.819 | 2.026 | 2.838 | 49.902 | 0.19x |
| flat.json | json | 1.977 | 2.019 | 3.408 | 49.902 | 0.20x |
| nested.json | strata | 0.286 | 0.288 | 0.315 | 49.250 | 1.00x |
| nested.json | orjson | 0.323 | 0.325 | 0.392 | 49.250 | 0.89x |
| nested.json | msgspec | 0.462 | 0.469 | 0.510 | 49.250 | 0.62x |
| nested.json | ujson | 1.349 | 1.384 | 1.902 | 49.250 | 0.21x |
| nested.json | json | 2.440 | 2.471 | 2.520 | 49.250 | 0.12x |
| wide_arrays.json | strata | 2.197 | 2.210 | 3.280 | 51.617 | 1.00x |
| wide_arrays.json | orjson | 2.059 | 2.109 | 3.116 | 51.617 | 1.05x |
| wide_arrays.json | msgspec | 3.331 | 3.375 | 4.013 | 51.617 | 0.65x |
| wide_arrays.json | ujson | 9.580 | 9.663 | 9.949 | 51.617 | 0.23x |
| wide_arrays.json | json | 17.889 | 17.950 | 31.183 | 51.617 | 0.12x |
| mixed.json | strata | 0.071 | 0.080 | 0.126 | 50.449 | 1.00x |
| mixed.json | orjson | 0.068 | 0.072 | 0.104 | 50.449 | 1.11x |
| mixed.json | msgspec | 0.093 | 0.097 | 0.140 | 50.449 | 0.83x |
| mixed.json | ujson | 0.338 | 0.363 | 0.779 | 50.449 | 0.22x |
| mixed.json | json | 0.507 | 0.526 | 0.929 | 50.449 | 0.15x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 20.207 | 21.745 | 77.275 | 54.766 | 1.00x |
| users.json | orjson | 24.121 | 26.016 | 83.403 | 54.766 | 0.84x |
| users.json | msgspec | 23.476 | 36.056 | 92.836 | 54.766 | 0.60x |
| users.json | ujson | 43.978 | 53.142 | 119.372 | 54.766 | 0.41x |
| users.json | json | 42.820 | 53.980 | 111.510 | 54.766 | 0.40x |
| flat.json | strata | 1.085 | 1.843 | 2.536 | 49.402 | 1.00x |
| flat.json | orjson | 1.258 | 2.009 | 2.966 | 49.402 | 0.92x |
| flat.json | msgspec | 1.284 | 2.220 | 5.685 | 49.402 | 0.83x |
| flat.json | ujson | 2.858 | 4.036 | 5.943 | 49.402 | 0.46x |
| flat.json | json | 2.131 | 3.121 | 5.279 | 49.402 | 0.59x |
| nested.json | strata | 0.913 | 0.948 | 1.438 | 49.383 | 1.00x |
| nested.json | orjson | 1.175 | 1.228 | 1.955 | 49.383 | 0.77x |
| nested.json | msgspec | 1.120 | 1.170 | 1.902 | 49.383 | 0.81x |
| nested.json | ujson | 1.985 | 2.041 | 3.294 | 49.383 | 0.46x |
| nested.json | json | 2.226 | 2.254 | 4.259 | 49.383 | 0.42x |
| wide_arrays.json | strata | 5.510 | 5.565 | 6.707 | 51.617 | 1.00x |
| wide_arrays.json | orjson | 6.164 | 6.267 | 9.194 | 51.617 | 0.89x |
| wide_arrays.json | msgspec | 6.237 | 6.317 | 10.228 | 51.617 | 0.88x |
| wide_arrays.json | ujson | 11.257 | 11.371 | 16.256 | 51.617 | 0.49x |
| wide_arrays.json | json | 11.974 | 12.022 | 12.305 | 51.617 | 0.46x |
| mixed.json | strata | 0.284 | 0.295 | 0.426 | 50.449 | 1.00x |
| mixed.json | orjson | 0.325 | 0.338 | 0.494 | 50.449 | 0.88x |
| mixed.json | msgspec | 0.346 | 0.366 | 0.409 | 50.449 | 0.81x |
| mixed.json | ujson | 0.544 | 0.552 | 0.604 | 50.449 | 0.53x |
| mixed.json | json | 0.583 | 0.599 | 0.699 | 50.449 | 0.49x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 19.605 | 21.667 | 30.956 | 49.887 | 1.00x |
| users.ndjson | orjson | 26.364 | 31.910 | 50.809 | 49.887 | 0.68x |
| users.ndjson | msgspec | 30.967 | 34.377 | 57.624 | 49.887 | 0.63x |
| users.ndjson | ujson | 45.164 | 48.142 | 118.197 | 49.887 | 0.45x |
| users.ndjson | json | 50.519 | 59.370 | 105.461 | 49.887 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 5.750 | 6.315 | 37.967 | 50.832 | 1.00x |
| users.json | orjson | 6.770 | 7.268 | 12.035 | 50.832 | 0.87x |
| users.json | msgspec | 8.888 | 11.780 | 56.460 | 50.832 | 0.54x |
| users.json | ujson | 44.434 | 46.327 | 107.804 | 50.832 | 0.14x |
| users.json | json | 59.902 | 61.129 | 136.000 | 50.832 | 0.10x |
| flat.json | strata | 0.679 | 0.782 | 1.015 | 49.520 | 1.00x |
| flat.json | orjson | 0.720 | 0.880 | 1.327 | 49.520 | 0.89x |
| flat.json | msgspec | 0.866 | 1.070 | 2.082 | 49.520 | 0.73x |
| flat.json | ujson | 3.168 | 3.266 | 5.453 | 49.520 | 0.24x |
| flat.json | json | 3.355 | 3.465 | 5.931 | 49.520 | 0.23x |
| nested.json | strata | 0.620 | 0.679 | 1.088 | 49.383 | 1.00x |
| nested.json | orjson | 0.667 | 0.789 | 6.222 | 49.383 | 0.86x |
| nested.json | msgspec | 0.811 | 0.873 | 1.243 | 49.383 | 0.78x |
| nested.json | ujson | 2.428 | 2.471 | 4.100 | 49.383 | 0.27x |
| nested.json | json | 3.488 | 3.649 | 6.913 | 49.383 | 0.19x |
| wide_arrays.json | strata | 2.816 | 2.917 | 4.182 | 51.535 | 1.00x |
| wide_arrays.json | orjson | 2.785 | 3.110 | 4.145 | 51.535 | 0.94x |
| wide_arrays.json | msgspec | 4.066 | 4.448 | 4.637 | 51.535 | 0.66x |
| wide_arrays.json | ujson | 16.329 | 16.524 | 16.933 | 51.535 | 0.18x |
| wide_arrays.json | json | 24.342 | 24.923 | 39.824 | 51.535 | 0.12x |
| mixed.json | strata | 0.382 | 0.404 | 0.449 | 50.449 | 1.00x |
| mixed.json | orjson | 0.379 | 0.382 | 0.461 | 50.449 | 1.06x |
| mixed.json | msgspec | 0.402 | 0.406 | 0.485 | 50.449 | 1.00x |
| mixed.json | ujson | 0.814 | 0.876 | 0.893 | 50.449 | 0.46x |
| mixed.json | json | 0.980 | 0.993 | 1.051 | 50.449 | 0.41x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.142 | 0.155 | 0.164 | 50.844 | 1.00x |
| users.json $[*].id | jmespath | 0.836 | 0.870 | 2.737 | 50.844 | 0.18x |
| users.json $[*].id | jsonpath-ng | 4.767 | 6.050 | 7.314 | 50.844 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.820 | 0.847 | 0.953 | 50.848 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.167 | 5.425 | 65.106 | 50.848 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 32.327 | 35.417 | 73.447 | 50.848 | 0.02x |
| users.json $..total | strata | 3.377 | 3.704 | 5.820 | 52.473 | 1.00x |
| users.json $..total | jsonpath-ng | 873.270 | 977.935 | 1065.759 | 52.473 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 6.412 | 6.526 | 10.492 | 50.848 | 1.00x |
| users.json $[*].id | orjson+jmespath | 25.266 | 27.601 | 39.292 | 50.848 | 0.24x |
| users.json $[*].id | orjson+jsonpath-ng | 28.833 | 31.718 | 155.663 | 50.848 | 0.21x |
| users.json $[*].orders[*].total | strata | 6.785 | 7.224 | 11.647 | 52.473 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 29.764 | 37.943 | 105.795 | 52.473 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 62.843 | 77.699 | 138.772 | 52.473 | 0.09x |
| users.json $..total | strata | 25.400 | 32.139 | 78.930 | 52.797 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 726.685 | 841.485 | 1018.116 | 52.797 | 0.04x |

