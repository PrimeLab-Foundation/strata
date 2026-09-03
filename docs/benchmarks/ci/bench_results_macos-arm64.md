# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c3d92f5
- python: 3.12.10
- implementation: CPython
- platform: macOS-26.5.2-arm64-arm-64bit
- machine: arm64
- processor: Apple M1 (Virtual)
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.549 | 7.887 | 8.062 | 63.828 | 1.00x |
| users.json | orjson | 10.710 | 11.697 | 46.415 | 63.828 | 0.67x |
| users.json | msgspec | 9.728 | 10.697 | 12.608 | 63.828 | 0.74x |
| users.json | ujson | 13.885 | 18.264 | 23.062 | 63.828 | 0.43x |
| users.json | pysimdjson | 131.410 | 144.675 | 156.270 | 63.828 | 0.05x |
| users.json | json | 15.194 | 18.580 | 21.995 | 63.828 | 0.42x |
| flat.json | strata | 0.669 | 0.694 | 0.760 | 93.188 | 1.00x |
| flat.json | orjson | 0.827 | 0.914 | 0.984 | 93.188 | 0.76x |
| flat.json | msgspec | 0.791 | 0.836 | 0.935 | 93.188 | 0.83x |
| flat.json | ujson | 1.319 | 1.393 | 1.581 | 93.188 | 0.50x |
| flat.json | pysimdjson | 13.050 | 13.285 | 13.578 | 93.188 | 0.05x |
| flat.json | json | 1.477 | 1.504 | 1.585 | 93.188 | 0.46x |
| nested.json | strata | 0.543 | 0.601 | 0.748 | 93.219 | 1.00x |
| nested.json | orjson | 0.802 | 0.855 | 0.985 | 93.219 | 0.70x |
| nested.json | msgspec | 0.737 | 0.818 | 0.942 | 93.219 | 0.73x |
| nested.json | ujson | 1.229 | 1.372 | 3.672 | 93.219 | 0.44x |
| nested.json | pysimdjson | 11.036 | 11.660 | 15.531 | 93.219 | 0.05x |
| nested.json | json | 1.491 | 1.570 | 1.778 | 93.219 | 0.38x |
| wide_arrays.json | strata | 3.264 | 3.762 | 4.638 | 95.984 | 1.00x |
| wide_arrays.json | orjson | 3.332 | 4.020 | 5.072 | 95.984 | 0.94x |
| wide_arrays.json | msgspec | 3.821 | 4.619 | 5.446 | 95.984 | 0.81x |
| wide_arrays.json | ujson | 4.982 | 5.685 | 6.765 | 95.984 | 0.66x |
| wide_arrays.json | pysimdjson | 61.366 | 65.599 | 74.380 | 95.984 | 0.06x |
| wide_arrays.json | json | 6.467 | 7.531 | 8.188 | 95.984 | 0.50x |
| mixed.json | strata | 0.157 | 0.162 | 0.217 | 96.000 | 1.00x |
| mixed.json | orjson | 0.185 | 0.197 | 0.266 | 96.000 | 0.82x |
| mixed.json | msgspec | 0.196 | 0.204 | 0.275 | 96.000 | 0.79x |
| mixed.json | ujson | 0.249 | 0.365 | 0.509 | 96.000 | 0.44x |
| mixed.json | pysimdjson | 2.761 | 2.854 | 3.563 | 96.000 | 0.06x |
| mixed.json | json | 0.358 | 0.395 | 1.058 | 96.000 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.715 | 1.891 | 2.337 | 82.422 | 1.00x |
| users.json | orjson | 2.362 | 2.598 | 4.019 | 82.422 | 0.73x |
| users.json | msgspec | 2.868 | 2.954 | 4.464 | 82.422 | 0.64x |
| users.json | ujson | 10.848 | 11.200 | 14.429 | 82.422 | 0.17x |
| users.json | json | 15.636 | 16.488 | 23.973 | 82.422 | 0.11x |
| flat.json | strata | 0.236 | 0.277 | 0.837 | 93.203 | 1.00x |
| flat.json | orjson | 0.263 | 0.357 | 0.458 | 93.203 | 0.78x |
| flat.json | msgspec | 0.335 | 0.374 | 0.501 | 93.203 | 0.74x |
| flat.json | ujson | 0.940 | 1.081 | 1.174 | 93.203 | 0.26x |
| flat.json | json | 1.442 | 1.481 | 1.620 | 93.203 | 0.19x |
| nested.json | strata | 0.150 | 0.166 | 0.196 | 93.219 | 1.00x |
| nested.json | orjson | 0.250 | 0.274 | 0.292 | 93.219 | 0.61x |
| nested.json | msgspec | 0.308 | 0.340 | 0.377 | 93.219 | 0.49x |
| nested.json | ujson | 1.235 | 1.274 | 1.355 | 93.219 | 0.13x |
| nested.json | json | 1.781 | 1.844 | 1.966 | 93.219 | 0.09x |
| wide_arrays.json | strata | 1.048 | 1.070 | 1.196 | 95.984 | 1.00x |
| wide_arrays.json | orjson | 1.290 | 1.358 | 1.440 | 95.984 | 0.79x |
| wide_arrays.json | msgspec | 1.941 | 1.960 | 2.036 | 95.984 | 0.55x |
| wide_arrays.json | ujson | 6.105 | 6.185 | 6.291 | 95.984 | 0.17x |
| wide_arrays.json | json | 10.965 | 11.007 | 11.232 | 95.984 | 0.10x |
| mixed.json | strata | 0.051 | 0.058 | 0.073 | 96.000 | 1.00x |
| mixed.json | orjson | 0.047 | 0.059 | 0.317 | 96.000 | 0.98x |
| mixed.json | msgspec | 0.060 | 0.064 | 0.279 | 96.000 | 0.90x |
| mixed.json | ujson | 0.235 | 0.253 | 0.349 | 96.000 | 0.23x |
| mixed.json | json | 0.407 | 0.427 | 0.472 | 96.000 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.725 | 7.395 | 8.508 | 92.453 | 1.00x |
| users.json | orjson | 8.946 | 10.420 | 12.051 | 92.453 | 0.71x |
| users.json | msgspec | 8.673 | 9.937 | 12.024 | 92.453 | 0.74x |
| users.json | ujson | 11.833 | 13.985 | 16.894 | 92.453 | 0.53x |
| users.json | json | 14.144 | 15.701 | 18.567 | 92.453 | 0.47x |
| flat.json | strata | 0.692 | 0.741 | 0.855 | 93.203 | 1.00x |
| flat.json | orjson | 0.971 | 1.055 | 1.165 | 93.203 | 0.70x |
| flat.json | msgspec | 0.818 | 0.922 | 1.057 | 93.203 | 0.80x |
| flat.json | ujson | 1.192 | 1.315 | 1.443 | 93.203 | 0.56x |
| flat.json | json | 1.525 | 1.557 | 1.814 | 93.203 | 0.48x |
| nested.json | strata | 0.601 | 0.669 | 0.740 | 93.219 | 1.00x |
| nested.json | orjson | 0.976 | 1.050 | 1.100 | 93.219 | 0.64x |
| nested.json | msgspec | 0.813 | 0.885 | 0.960 | 93.219 | 0.76x |
| nested.json | ujson | 1.074 | 1.164 | 1.325 | 93.219 | 0.58x |
| nested.json | json | 1.522 | 1.607 | 1.785 | 93.219 | 0.42x |
| wide_arrays.json | strata | 3.390 | 3.425 | 3.495 | 95.984 | 1.00x |
| wide_arrays.json | orjson | 3.520 | 3.563 | 3.623 | 95.984 | 0.96x |
| wide_arrays.json | msgspec | 4.102 | 4.163 | 4.692 | 95.984 | 0.82x |
| wide_arrays.json | ujson | 5.443 | 5.482 | 5.555 | 95.984 | 0.62x |
| wide_arrays.json | json | 6.734 | 6.786 | 6.851 | 95.984 | 0.50x |
| mixed.json | strata | 0.206 | 0.239 | 0.572 | 96.000 | 1.00x |
| mixed.json | orjson | 0.310 | 0.516 | 1.172 | 96.000 | 0.46x |
| mixed.json | msgspec | 0.295 | 0.321 | 0.383 | 96.000 | 0.74x |
| mixed.json | ujson | 0.364 | 0.478 | 1.029 | 96.000 | 0.50x |
| mixed.json | json | 0.484 | 0.537 | 1.256 | 96.000 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.149 | 7.990 | 14.636 | 93.188 | 1.00x |
| users.ndjson | orjson | 11.317 | 12.020 | 21.096 | 93.188 | 0.66x |
| users.ndjson | msgspec | 11.113 | 12.293 | 19.306 | 93.188 | 0.65x |
| users.ndjson | ujson | 13.896 | 15.326 | 22.971 | 93.188 | 0.52x |
| users.ndjson | json | 17.877 | 19.668 | 27.354 | 93.188 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.207 | 2.546 | 2.813 | 88.469 | 1.00x |
| users.json | orjson | 2.818 | 3.401 | 3.680 | 88.469 | 0.75x |
| users.json | msgspec | 3.385 | 4.005 | 4.649 | 88.469 | 0.64x |
| users.json | ujson | 11.301 | 12.425 | 14.235 | 88.469 | 0.20x |
| users.json | json | 15.613 | 16.879 | 18.691 | 88.469 | 0.15x |
| flat.json | strata | 0.433 | 0.586 | 1.304 | 93.203 | 1.00x |
| flat.json | orjson | 0.505 | 0.602 | 0.906 | 93.203 | 0.97x |
| flat.json | msgspec | 0.577 | 0.647 | 0.680 | 93.203 | 0.91x |
| flat.json | ujson | 1.252 | 1.387 | 1.547 | 93.203 | 0.42x |
| flat.json | json | 1.737 | 1.856 | 3.285 | 93.203 | 0.32x |
| nested.json | strata | 0.221 | 0.250 | 0.374 | 93.219 | 1.00x |
| nested.json | orjson | 0.301 | 0.348 | 0.655 | 93.219 | 0.72x |
| nested.json | msgspec | 0.430 | 0.532 | 0.660 | 93.219 | 0.47x |
| nested.json | ujson | 1.036 | 1.127 | 1.405 | 93.219 | 0.22x |
| nested.json | json | 1.630 | 1.700 | 1.955 | 93.219 | 0.15x |
| wide_arrays.json | strata | 1.327 | 1.390 | 1.904 | 95.984 | 1.00x |
| wide_arrays.json | orjson | 1.591 | 1.694 | 2.539 | 95.984 | 0.82x |
| wide_arrays.json | msgspec | 2.248 | 2.317 | 3.544 | 95.984 | 0.60x |
| wide_arrays.json | ujson | 6.610 | 6.762 | 10.280 | 95.984 | 0.21x |
| wide_arrays.json | json | 11.340 | 11.581 | 16.293 | 95.984 | 0.12x |
| mixed.json | strata | 0.218 | 0.269 | 0.297 | 96.000 | 1.00x |
| mixed.json | orjson | 0.261 | 0.289 | 0.660 | 96.000 | 0.93x |
| mixed.json | msgspec | 0.241 | 0.521 | 0.812 | 96.000 | 0.52x |
| mixed.json | ujson | 0.442 | 0.502 | 1.276 | 96.000 | 0.54x |
| mixed.json | json | 0.609 | 0.649 | 0.915 | 96.000 | 0.41x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.049 | 0.090 | 0.175 | 88.547 | 1.00x |
| users.json $[*].id | jmespath | 0.257 | 0.382 | 0.417 | 88.547 | 0.24x |
| users.json $[*].id | jsonpath-ng | 1.554 | 1.718 | 1.875 | 88.547 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.334 | 0.504 | 1.658 | 88.719 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.778 | 1.866 | 3.720 | 88.719 | 0.27x |
| users.json $[*].orders[*].total | jsonpath-ng | 11.230 | 13.637 | 22.947 | 88.719 | 0.04x |
| users.json $..total | strata | 1.187 | 1.528 | 2.151 | 90.562 | 1.00x |
| users.json $..total | jsonpath-ng | 180.836 | 211.044 | 580.847 | 90.562 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.955 | 4.144 | 4.261 | 88.594 | 1.00x |
| users.json $[*].id | orjson+jmespath | 11.641 | 11.768 | 12.398 | 88.594 | 0.35x |
| users.json $[*].id | orjson+jsonpath-ng | 12.737 | 13.118 | 13.709 | 88.594 | 0.32x |
| users.json $[*].orders[*].total | strata | 3.833 | 4.122 | 4.405 | 90.547 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 11.655 | 13.251 | 17.416 | 90.547 | 0.31x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 23.528 | 27.568 | 33.150 | 90.547 | 0.15x |
| users.json $..total | strata | 8.045 | 9.075 | 16.388 | 90.562 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 196.341 | 218.698 | 271.309 | 90.562 | 0.04x |

