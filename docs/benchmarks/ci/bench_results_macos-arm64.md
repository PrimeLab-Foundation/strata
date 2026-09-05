# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: b7f31bb
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
| users.json | strata | 6.195 | 6.330 | 7.935 | 63.906 | 1.00x |
| users.json | orjson | 9.269 | 9.898 | 11.368 | 63.906 | 0.64x |
| users.json | msgspec | 8.866 | 9.441 | 10.386 | 63.906 | 0.67x |
| users.json | ujson | 11.924 | 12.907 | 16.831 | 63.906 | 0.49x |
| users.json | pysimdjson | 117.391 | 123.320 | 164.129 | 63.906 | 0.05x |
| users.json | json | 14.596 | 15.357 | 18.282 | 63.906 | 0.41x |
| flat.json | strata | 0.530 | 0.531 | 0.642 | 86.625 | 1.00x |
| flat.json | orjson | 0.656 | 0.681 | 0.829 | 86.625 | 0.78x |
| flat.json | msgspec | 0.672 | 0.674 | 0.712 | 86.625 | 0.79x |
| flat.json | ujson | 1.039 | 1.069 | 1.269 | 86.625 | 0.50x |
| flat.json | pysimdjson | 11.091 | 11.136 | 12.302 | 86.625 | 0.05x |
| flat.json | json | 1.272 | 1.280 | 1.448 | 86.625 | 0.42x |
| nested.json | strata | 0.515 | 0.528 | 0.648 | 86.625 | 1.00x |
| nested.json | orjson | 0.706 | 0.740 | 0.952 | 86.625 | 0.71x |
| nested.json | msgspec | 0.660 | 0.667 | 0.840 | 86.625 | 0.79x |
| nested.json | ujson | 1.036 | 1.111 | 1.189 | 86.625 | 0.48x |
| nested.json | pysimdjson | 10.137 | 10.659 | 10.951 | 86.625 | 0.05x |
| nested.json | json | 1.393 | 1.480 | 1.572 | 86.625 | 0.36x |
| wide_arrays.json | strata | 2.771 | 2.991 | 5.204 | 89.391 | 1.00x |
| wide_arrays.json | orjson | 3.293 | 3.726 | 4.790 | 89.391 | 0.80x |
| wide_arrays.json | msgspec | 3.798 | 4.129 | 4.969 | 89.391 | 0.72x |
| wide_arrays.json | ujson | 4.882 | 5.431 | 6.245 | 89.391 | 0.55x |
| wide_arrays.json | pysimdjson | 60.420 | 64.576 | 69.122 | 89.391 | 0.05x |
| wide_arrays.json | json | 6.313 | 6.896 | 7.806 | 89.391 | 0.43x |
| mixed.json | strata | 0.121 | 0.128 | 0.145 | 89.406 | 1.00x |
| mixed.json | orjson | 0.155 | 0.165 | 0.191 | 89.406 | 0.78x |
| mixed.json | msgspec | 0.167 | 0.178 | 0.225 | 89.406 | 0.72x |
| mixed.json | ujson | 0.207 | 0.283 | 0.441 | 89.406 | 0.45x |
| mixed.json | pysimdjson | 2.444 | 2.511 | 2.589 | 89.406 | 0.05x |
| mixed.json | json | 0.319 | 0.330 | 0.351 | 89.406 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.566 | 1.790 | 2.196 | 73.688 | 1.00x |
| users.json | orjson | 2.229 | 2.436 | 3.769 | 73.688 | 0.73x |
| users.json | msgspec | 2.873 | 3.166 | 3.299 | 73.688 | 0.57x |
| users.json | ujson | 8.974 | 9.407 | 10.733 | 73.688 | 0.19x |
| users.json | json | 15.525 | 16.339 | 19.667 | 73.688 | 0.11x |
| flat.json | strata | 0.206 | 0.209 | 0.219 | 86.625 | 1.00x |
| flat.json | orjson | 0.231 | 0.233 | 0.238 | 86.625 | 0.90x |
| flat.json | msgspec | 0.287 | 0.290 | 0.293 | 86.625 | 0.72x |
| flat.json | ujson | 0.752 | 0.759 | 0.811 | 86.625 | 0.28x |
| flat.json | json | 1.332 | 1.353 | 1.460 | 86.625 | 0.15x |
| nested.json | strata | 0.123 | 0.132 | 0.147 | 86.625 | 1.00x |
| nested.json | orjson | 0.207 | 0.220 | 0.232 | 86.625 | 0.60x |
| nested.json | msgspec | 0.336 | 0.408 | 0.429 | 86.625 | 0.32x |
| nested.json | ujson | 0.834 | 0.908 | 1.032 | 86.625 | 0.15x |
| nested.json | json | 1.534 | 1.641 | 1.765 | 86.625 | 0.08x |
| wide_arrays.json | strata | 0.988 | 1.131 | 1.417 | 89.391 | 1.00x |
| wide_arrays.json | orjson | 1.388 | 1.527 | 1.683 | 89.391 | 0.74x |
| wide_arrays.json | msgspec | 1.978 | 2.204 | 2.499 | 89.391 | 0.51x |
| wide_arrays.json | ujson | 4.726 | 5.125 | 5.360 | 89.391 | 0.22x |
| wide_arrays.json | json | 11.468 | 12.164 | 13.286 | 89.391 | 0.09x |
| mixed.json | strata | 0.038 | 0.041 | 0.045 | 89.406 | 1.00x |
| mixed.json | orjson | 0.043 | 0.046 | 0.052 | 89.406 | 0.89x |
| mixed.json | msgspec | 0.051 | 0.055 | 0.260 | 89.406 | 0.75x |
| mixed.json | ujson | 0.172 | 0.177 | 0.201 | 89.406 | 0.23x |
| mixed.json | json | 0.350 | 0.368 | 0.385 | 89.406 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.317 | 6.994 | 7.585 | 85.234 | 1.00x |
| users.json | orjson | 9.697 | 10.453 | 11.650 | 85.234 | 0.67x |
| users.json | msgspec | 9.258 | 10.125 | 11.112 | 85.234 | 0.69x |
| users.json | ujson | 11.659 | 13.878 | 15.133 | 85.234 | 0.50x |
| users.json | json | 14.241 | 16.283 | 17.776 | 85.234 | 0.43x |
| flat.json | strata | 0.557 | 0.593 | 0.795 | 86.625 | 1.00x |
| flat.json | orjson | 0.766 | 0.905 | 1.116 | 86.625 | 0.65x |
| flat.json | msgspec | 0.720 | 0.773 | 0.977 | 86.625 | 0.77x |
| flat.json | ujson | 1.031 | 1.091 | 1.253 | 86.625 | 0.54x |
| flat.json | json | 1.314 | 1.383 | 1.533 | 86.625 | 0.43x |
| nested.json | strata | 0.515 | 0.532 | 0.617 | 86.625 | 1.00x |
| nested.json | orjson | 0.752 | 0.861 | 0.975 | 86.625 | 0.62x |
| nested.json | msgspec | 0.681 | 0.719 | 0.799 | 86.625 | 0.74x |
| nested.json | ujson | 0.954 | 1.011 | 1.074 | 86.625 | 0.53x |
| nested.json | json | 1.350 | 1.396 | 1.763 | 86.625 | 0.38x |
| wide_arrays.json | strata | 3.406 | 3.527 | 4.182 | 89.391 | 1.00x |
| wide_arrays.json | orjson | 4.138 | 4.298 | 4.818 | 89.391 | 0.82x |
| wide_arrays.json | msgspec | 4.705 | 4.859 | 5.134 | 89.391 | 0.73x |
| wide_arrays.json | ujson | 6.181 | 6.486 | 7.576 | 89.391 | 0.54x |
| wide_arrays.json | json | 7.652 | 7.978 | 9.304 | 89.391 | 0.44x |
| mixed.json | strata | 0.148 | 0.169 | 0.190 | 89.406 | 1.00x |
| mixed.json | orjson | 0.277 | 0.305 | 0.324 | 89.406 | 0.55x |
| mixed.json | msgspec | 0.214 | 0.230 | 0.275 | 89.406 | 0.73x |
| mixed.json | ujson | 0.266 | 0.282 | 0.296 | 89.406 | 0.60x |
| mixed.json | json | 0.353 | 0.383 | 0.420 | 89.406 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.416 | 7.150 | 7.595 | 86.625 | 1.00x |
| users.ndjson | orjson | 10.818 | 11.580 | 12.863 | 86.625 | 0.62x |
| users.ndjson | msgspec | 10.939 | 11.481 | 12.738 | 86.625 | 0.62x |
| users.ndjson | ujson | 13.981 | 14.736 | 16.837 | 86.625 | 0.49x |
| users.ndjson | json | 17.518 | 18.615 | 21.119 | 86.625 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.825 | 2.016 | 2.133 | 80.922 | 1.00x |
| users.json | orjson | 2.653 | 2.855 | 3.260 | 80.922 | 0.71x |
| users.json | msgspec | 3.170 | 3.523 | 3.847 | 80.922 | 0.57x |
| users.json | ujson | 9.189 | 9.811 | 13.449 | 80.922 | 0.21x |
| users.json | json | 15.433 | 16.611 | 17.281 | 80.922 | 0.12x |
| flat.json | strata | 0.408 | 0.511 | 1.253 | 86.625 | 1.00x |
| flat.json | orjson | 0.423 | 0.474 | 0.695 | 86.625 | 1.08x |
| flat.json | msgspec | 0.460 | 0.596 | 0.924 | 86.625 | 0.86x |
| flat.json | ujson | 0.978 | 1.060 | 1.351 | 86.625 | 0.48x |
| flat.json | json | 1.661 | 1.813 | 2.003 | 86.625 | 0.28x |
| nested.json | strata | 0.337 | 0.459 | 0.572 | 86.625 | 1.00x |
| nested.json | orjson | 0.410 | 0.598 | 1.545 | 86.625 | 0.77x |
| nested.json | msgspec | 0.636 | 0.773 | 1.530 | 86.625 | 0.59x |
| nested.json | ujson | 1.323 | 1.505 | 2.590 | 86.625 | 0.31x |
| nested.json | json | 1.921 | 2.182 | 3.437 | 86.625 | 0.21x |
| wide_arrays.json | strata | 1.544 | 1.697 | 1.957 | 89.391 | 1.00x |
| wide_arrays.json | orjson | 1.982 | 2.171 | 2.620 | 89.391 | 0.78x |
| wide_arrays.json | msgspec | 2.507 | 3.019 | 3.384 | 89.391 | 0.56x |
| wide_arrays.json | ujson | 5.741 | 6.004 | 13.365 | 89.391 | 0.28x |
| wide_arrays.json | json | 12.567 | 13.225 | 19.889 | 89.391 | 0.13x |
| mixed.json | strata | 0.161 | 0.187 | 0.273 | 89.406 | 1.00x |
| mixed.json | orjson | 0.165 | 0.194 | 0.401 | 89.406 | 0.97x |
| mixed.json | msgspec | 0.161 | 0.194 | 0.269 | 89.406 | 0.96x |
| mixed.json | ujson | 0.305 | 0.368 | 0.943 | 89.406 | 0.51x |
| mixed.json | json | 0.502 | 0.553 | 0.630 | 89.406 | 0.34x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.046 | 0.048 | 0.063 | 80.984 | 1.00x |
| users.json $[*].id | jmespath | 0.254 | 0.261 | 0.324 | 80.984 | 0.18x |
| users.json $[*].id | jsonpath-ng | 1.404 | 1.444 | 2.112 | 80.984 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.302 | 0.369 | 0.577 | 82.125 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.619 | 1.667 | 2.153 | 82.125 | 0.22x |
| users.json $[*].orders[*].total | jsonpath-ng | 10.188 | 10.742 | 11.130 | 82.125 | 0.03x |
| users.json $..total | strata | 1.199 | 1.379 | 3.027 | 83.938 | 1.00x |
| users.json $..total | jsonpath-ng | 183.951 | 191.238 | 240.460 | 83.938 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.435 | 3.526 | 3.862 | 82.016 | 1.00x |
| users.json $[*].id | orjson+jmespath | 9.158 | 9.902 | 10.542 | 82.016 | 0.36x |
| users.json $[*].id | orjson+jsonpath-ng | 10.432 | 11.262 | 12.081 | 82.016 | 0.31x |
| users.json $[*].orders[*].total | strata | 3.555 | 3.781 | 4.016 | 83.938 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 11.221 | 11.568 | 13.126 | 83.938 | 0.33x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 22.291 | 23.701 | 28.070 | 83.938 | 0.16x |
| users.json $..total | strata | 7.773 | 8.703 | 11.676 | 83.938 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 196.990 | 210.642 | 310.887 | 83.938 | 0.04x |

