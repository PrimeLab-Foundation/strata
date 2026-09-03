# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 59554e7
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
| users.json | strata | 7.024 | 9.493 | 12.756 | 63.672 | 1.00x |
| users.json | orjson | 10.157 | 13.119 | 14.378 | 63.672 | 0.72x |
| users.json | msgspec | 11.427 | 12.780 | 16.578 | 63.672 | 0.74x |
| users.json | ujson | 13.568 | 17.774 | 22.368 | 63.672 | 0.53x |
| users.json | pysimdjson | 135.615 | 164.505 | 185.892 | 63.672 | 0.06x |
| users.json | json | 15.408 | 20.623 | 25.958 | 63.672 | 0.46x |
| flat.json | strata | 0.597 | 0.640 | 0.733 | 87.312 | 1.00x |
| flat.json | orjson | 0.751 | 0.811 | 1.070 | 87.312 | 0.79x |
| flat.json | msgspec | 0.735 | 0.813 | 1.087 | 87.312 | 0.79x |
| flat.json | ujson | 1.230 | 1.354 | 1.413 | 87.312 | 0.47x |
| flat.json | pysimdjson | 12.142 | 12.982 | 14.236 | 87.312 | 0.05x |
| flat.json | json | 1.405 | 1.481 | 1.716 | 87.312 | 0.43x |
| nested.json | strata | 0.545 | 0.612 | 0.654 | 87.328 | 1.00x |
| nested.json | orjson | 0.772 | 0.824 | 1.165 | 87.328 | 0.74x |
| nested.json | msgspec | 0.702 | 0.772 | 0.905 | 87.328 | 0.79x |
| nested.json | ujson | 1.068 | 1.196 | 1.426 | 87.328 | 0.51x |
| nested.json | pysimdjson | 10.969 | 11.608 | 12.474 | 87.328 | 0.05x |
| nested.json | json | 1.472 | 1.640 | 2.166 | 87.328 | 0.37x |
| wide_arrays.json | strata | 3.611 | 3.901 | 3.988 | 90.078 | 1.00x |
| wide_arrays.json | orjson | 3.765 | 4.208 | 4.349 | 90.078 | 0.93x |
| wide_arrays.json | msgspec | 4.191 | 4.689 | 4.898 | 90.078 | 0.83x |
| wide_arrays.json | ujson | 5.667 | 6.102 | 6.769 | 90.078 | 0.64x |
| wide_arrays.json | pysimdjson | 66.709 | 68.986 | 73.170 | 90.078 | 0.06x |
| wide_arrays.json | json | 6.990 | 7.342 | 7.984 | 90.078 | 0.53x |
| mixed.json | strata | 0.152 | 0.226 | 0.334 | 90.094 | 1.00x |
| mixed.json | orjson | 0.175 | 0.199 | 0.436 | 90.094 | 1.14x |
| mixed.json | msgspec | 0.188 | 0.203 | 0.270 | 90.094 | 1.11x |
| mixed.json | ujson | 0.240 | 0.370 | 0.598 | 90.094 | 0.61x |
| mixed.json | pysimdjson | 2.799 | 3.093 | 3.395 | 90.094 | 0.07x |
| mixed.json | json | 0.372 | 0.493 | 0.660 | 90.094 | 0.46x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.712 | 2.237 | 3.941 | 76.047 | 1.00x |
| users.json | orjson | 2.346 | 2.749 | 5.430 | 76.047 | 0.81x |
| users.json | msgspec | 2.968 | 3.627 | 6.026 | 76.047 | 0.62x |
| users.json | ujson | 11.560 | 13.173 | 19.205 | 76.047 | 0.17x |
| users.json | json | 16.428 | 20.190 | 22.864 | 76.047 | 0.11x |
| flat.json | strata | 0.220 | 0.230 | 0.260 | 87.312 | 1.00x |
| flat.json | orjson | 0.338 | 0.387 | 0.433 | 87.312 | 0.59x |
| flat.json | msgspec | 0.331 | 0.362 | 0.410 | 87.312 | 0.64x |
| flat.json | ujson | 0.947 | 0.973 | 1.123 | 87.312 | 0.24x |
| flat.json | json | 1.499 | 1.582 | 1.727 | 87.312 | 0.15x |
| nested.json | strata | 0.158 | 0.174 | 0.414 | 87.344 | 1.00x |
| nested.json | orjson | 0.250 | 0.273 | 0.465 | 87.344 | 0.64x |
| nested.json | msgspec | 0.317 | 0.350 | 0.612 | 87.344 | 0.50x |
| nested.json | ujson | 1.303 | 1.441 | 1.760 | 87.344 | 0.12x |
| nested.json | json | 1.716 | 1.843 | 2.317 | 87.344 | 0.09x |
| wide_arrays.json | strata | 1.150 | 1.257 | 2.445 | 90.078 | 1.00x |
| wide_arrays.json | orjson | 1.466 | 1.579 | 2.056 | 90.078 | 0.80x |
| wide_arrays.json | msgspec | 2.143 | 2.281 | 2.371 | 90.078 | 0.55x |
| wide_arrays.json | ujson | 6.735 | 7.041 | 7.343 | 90.078 | 0.18x |
| wide_arrays.json | json | 12.236 | 12.629 | 13.464 | 90.078 | 0.10x |
| mixed.json | strata | 0.055 | 0.065 | 0.078 | 90.094 | 1.00x |
| mixed.json | orjson | 0.054 | 0.059 | 0.212 | 90.094 | 1.10x |
| mixed.json | msgspec | 0.069 | 0.073 | 0.177 | 90.094 | 0.90x |
| mixed.json | ujson | 0.241 | 0.259 | 0.367 | 90.094 | 0.25x |
| mixed.json | json | 0.387 | 0.439 | 0.592 | 90.094 | 0.15x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.356 | 8.901 | 10.502 | 86.438 | 1.00x |
| users.json | orjson | 10.147 | 12.535 | 14.442 | 86.438 | 0.71x |
| users.json | msgspec | 9.902 | 11.962 | 13.477 | 86.438 | 0.74x |
| users.json | ujson | 13.820 | 17.961 | 20.151 | 86.438 | 0.50x |
| users.json | json | 16.220 | 20.556 | 28.574 | 86.438 | 0.43x |
| flat.json | strata | 0.651 | 0.794 | 0.864 | 87.312 | 1.00x |
| flat.json | orjson | 0.932 | 1.088 | 1.134 | 87.312 | 0.73x |
| flat.json | msgspec | 0.878 | 0.996 | 1.160 | 87.312 | 0.80x |
| flat.json | ujson | 1.217 | 1.408 | 1.512 | 87.312 | 0.56x |
| flat.json | json | 1.555 | 1.704 | 1.759 | 87.312 | 0.47x |
| nested.json | strata | 0.687 | 0.752 | 0.900 | 87.344 | 1.00x |
| nested.json | orjson | 1.053 | 1.139 | 1.750 | 87.344 | 0.66x |
| nested.json | msgspec | 0.943 | 0.997 | 1.174 | 87.344 | 0.75x |
| nested.json | ujson | 1.254 | 1.367 | 1.546 | 87.344 | 0.55x |
| nested.json | json | 1.694 | 1.788 | 1.991 | 87.344 | 0.42x |
| wide_arrays.json | strata | 3.886 | 4.742 | 5.606 | 90.078 | 1.00x |
| wide_arrays.json | orjson | 3.994 | 4.942 | 5.964 | 90.078 | 0.96x |
| wide_arrays.json | msgspec | 4.711 | 6.046 | 7.549 | 90.078 | 0.78x |
| wide_arrays.json | ujson | 6.663 | 7.549 | 8.075 | 90.078 | 0.63x |
| wide_arrays.json | json | 8.350 | 9.614 | 11.022 | 90.078 | 0.49x |
| mixed.json | strata | 0.202 | 0.269 | 0.306 | 90.094 | 1.00x |
| mixed.json | orjson | 0.310 | 0.405 | 0.548 | 90.094 | 0.66x |
| mixed.json | msgspec | 0.283 | 0.327 | 0.411 | 90.094 | 0.82x |
| mixed.json | ujson | 0.324 | 0.372 | 0.492 | 90.094 | 0.72x |
| mixed.json | json | 0.389 | 0.497 | 0.859 | 90.094 | 0.54x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.636 | 7.975 | 8.654 | 87.312 | 1.00x |
| users.ndjson | orjson | 12.224 | 12.510 | 15.011 | 87.312 | 0.64x |
| users.ndjson | msgspec | 11.963 | 12.280 | 13.885 | 87.312 | 0.65x |
| users.ndjson | ujson | 14.824 | 15.247 | 16.545 | 87.312 | 0.52x |
| users.ndjson | json | 19.421 | 19.769 | 22.870 | 87.312 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.611 | 3.098 | 4.800 | 83.609 | 1.00x |
| users.json | orjson | 3.040 | 3.712 | 4.709 | 83.609 | 0.83x |
| users.json | msgspec | 4.051 | 4.365 | 5.451 | 83.609 | 0.71x |
| users.json | ujson | 13.423 | 15.682 | 16.762 | 83.609 | 0.20x |
| users.json | json | 18.825 | 22.235 | 23.097 | 83.609 | 0.14x |
| flat.json | strata | 0.436 | 0.524 | 0.588 | 87.328 | 1.00x |
| flat.json | orjson | 0.468 | 0.664 | 0.798 | 87.328 | 0.79x |
| flat.json | msgspec | 0.596 | 0.657 | 0.755 | 87.328 | 0.80x |
| flat.json | ujson | 1.199 | 1.339 | 1.443 | 87.328 | 0.39x |
| flat.json | json | 1.633 | 1.961 | 2.295 | 87.328 | 0.27x |
| nested.json | strata | 0.356 | 0.406 | 0.469 | 87.344 | 1.00x |
| nested.json | orjson | 0.473 | 0.507 | 0.645 | 87.344 | 0.80x |
| nested.json | msgspec | 0.603 | 0.776 | 1.082 | 87.344 | 0.52x |
| nested.json | ujson | 1.481 | 1.552 | 1.672 | 87.344 | 0.26x |
| nested.json | json | 2.048 | 2.270 | 2.566 | 87.344 | 0.18x |
| wide_arrays.json | strata | 1.778 | 2.317 | 9.492 | 90.078 | 1.00x |
| wide_arrays.json | orjson | 2.147 | 2.416 | 2.837 | 90.078 | 0.96x |
| wide_arrays.json | msgspec | 3.273 | 3.479 | 3.833 | 90.078 | 0.67x |
| wide_arrays.json | ujson | 7.996 | 9.356 | 10.473 | 90.078 | 0.25x |
| wide_arrays.json | json | 14.124 | 16.177 | 23.300 | 90.078 | 0.14x |
| mixed.json | strata | 0.229 | 0.350 | 0.456 | 90.094 | 1.00x |
| mixed.json | orjson | 0.180 | 0.376 | 0.400 | 90.094 | 0.93x |
| mixed.json | msgspec | 0.320 | 0.383 | 0.735 | 90.094 | 0.91x |
| mixed.json | ujson | 0.451 | 0.622 | 0.772 | 90.094 | 0.56x |
| mixed.json | json | 0.691 | 0.837 | 0.985 | 90.094 | 0.42x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.113 | 0.149 | 0.293 | 83.672 | 1.00x |
| users.json $[*].id | jmespath | 0.357 | 0.443 | 1.589 | 83.672 | 0.34x |
| users.json $[*].id | jsonpath-ng | 1.755 | 2.076 | 5.667 | 83.672 | 0.07x |
| users.json $[*].orders[*].total | strata | 0.472 | 0.644 | 1.024 | 83.844 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.839 | 2.229 | 2.751 | 83.844 | 0.29x |
| users.json $[*].orders[*].total | jsonpath-ng | 12.090 | 12.607 | 15.902 | 83.844 | 0.05x |
| users.json $..total | strata | 1.513 | 1.814 | 3.971 | 84.922 | 1.00x |
| users.json $..total | jsonpath-ng | 225.839 | 248.675 | 299.328 | 84.922 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.196 | 4.735 | 5.660 | 83.750 | 1.00x |
| users.json $[*].id | orjson+jmespath | 11.709 | 12.132 | 14.041 | 83.750 | 0.39x |
| users.json $[*].id | orjson+jsonpath-ng | 12.964 | 15.011 | 16.284 | 83.750 | 0.32x |
| users.json $[*].orders[*].total | strata | 3.980 | 4.368 | 9.544 | 84.875 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.503 | 14.988 | 18.931 | 84.875 | 0.29x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 26.367 | 35.398 | 72.525 | 84.875 | 0.12x |
| users.json $..total | strata | 9.057 | 10.412 | 16.422 | 84.922 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 212.678 | 263.187 | 326.876 | 84.922 | 0.04x |

