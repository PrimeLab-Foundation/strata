# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: e8ac860
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
| users.json | strata | 7.472 | 8.427 | 13.581 | 63.750 | 1.00x |
| users.json | orjson | 9.958 | 13.080 | 21.076 | 63.750 | 0.64x |
| users.json | msgspec | 10.247 | 12.761 | 18.741 | 63.750 | 0.66x |
| users.json | ujson | 14.071 | 17.348 | 23.538 | 63.750 | 0.49x |
| users.json | pysimdjson | 149.591 | 158.606 | 176.699 | 63.750 | 0.05x |
| users.json | json | 17.524 | 20.880 | 35.194 | 63.750 | 0.40x |
| flat.json | strata | 0.618 | 0.691 | 0.835 | 86.812 | 1.00x |
| flat.json | orjson | 0.752 | 0.902 | 1.151 | 86.812 | 0.77x |
| flat.json | msgspec | 0.709 | 0.881 | 1.422 | 86.812 | 0.78x |
| flat.json | ujson | 1.243 | 1.477 | 2.167 | 86.812 | 0.47x |
| flat.json | pysimdjson | 12.176 | 13.659 | 17.608 | 86.812 | 0.05x |
| flat.json | json | 1.363 | 1.490 | 1.941 | 86.812 | 0.46x |
| nested.json | strata | 0.618 | 0.672 | 1.463 | 86.812 | 1.00x |
| nested.json | orjson | 0.889 | 1.058 | 1.605 | 86.812 | 0.63x |
| nested.json | msgspec | 0.732 | 0.960 | 1.788 | 86.812 | 0.70x |
| nested.json | ujson | 1.276 | 1.559 | 1.811 | 86.812 | 0.43x |
| nested.json | pysimdjson | 13.574 | 14.875 | 19.337 | 86.812 | 0.05x |
| nested.json | json | 1.524 | 1.751 | 2.694 | 86.812 | 0.38x |
| wide_arrays.json | strata | 3.747 | 4.220 | 7.164 | 89.578 | 1.00x |
| wide_arrays.json | orjson | 3.746 | 5.318 | 8.763 | 89.578 | 0.79x |
| wide_arrays.json | msgspec | 4.295 | 5.287 | 7.951 | 89.578 | 0.80x |
| wide_arrays.json | ujson | 5.655 | 6.395 | 11.513 | 89.578 | 0.66x |
| wide_arrays.json | pysimdjson | 68.026 | 79.854 | 115.427 | 89.578 | 0.05x |
| wide_arrays.json | json | 7.658 | 8.507 | 14.978 | 89.578 | 0.50x |
| mixed.json | strata | 0.126 | 0.140 | 0.209 | 90.516 | 1.00x |
| mixed.json | orjson | 0.148 | 0.177 | 0.344 | 90.516 | 0.79x |
| mixed.json | msgspec | 0.163 | 0.180 | 0.244 | 90.516 | 0.78x |
| mixed.json | ujson | 0.203 | 0.305 | 0.447 | 90.516 | 0.46x |
| mixed.json | pysimdjson | 2.424 | 2.504 | 2.827 | 90.516 | 0.06x |
| mixed.json | json | 0.317 | 0.356 | 0.450 | 90.516 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.979 | 2.365 | 3.336 | 73.531 | 1.00x |
| users.json | orjson | 2.392 | 2.947 | 3.762 | 73.531 | 0.80x |
| users.json | msgspec | 3.187 | 3.764 | 5.732 | 73.531 | 0.63x |
| users.json | ujson | 11.442 | 14.017 | 18.457 | 73.531 | 0.17x |
| users.json | json | 19.126 | 20.357 | 28.228 | 73.531 | 0.12x |
| flat.json | strata | 0.207 | 0.233 | 0.303 | 86.812 | 1.00x |
| flat.json | orjson | 0.259 | 0.286 | 0.465 | 86.812 | 0.82x |
| flat.json | msgspec | 0.321 | 0.361 | 0.446 | 86.812 | 0.64x |
| flat.json | ujson | 0.944 | 1.022 | 1.287 | 86.812 | 0.23x |
| flat.json | json | 1.407 | 1.504 | 1.893 | 86.812 | 0.15x |
| nested.json | strata | 0.150 | 0.183 | 0.548 | 86.812 | 1.00x |
| nested.json | orjson | 0.234 | 0.284 | 0.315 | 86.812 | 0.65x |
| nested.json | msgspec | 0.312 | 0.368 | 0.672 | 86.812 | 0.50x |
| nested.json | ujson | 1.068 | 1.231 | 1.466 | 86.812 | 0.15x |
| nested.json | json | 1.721 | 1.922 | 4.329 | 86.812 | 0.10x |
| wide_arrays.json | strata | 1.441 | 1.646 | 2.372 | 90.375 | 1.00x |
| wide_arrays.json | orjson | 1.477 | 1.916 | 2.615 | 90.375 | 0.86x |
| wide_arrays.json | msgspec | 2.385 | 2.809 | 4.210 | 90.375 | 0.59x |
| wide_arrays.json | ujson | 7.557 | 9.156 | 12.128 | 90.375 | 0.18x |
| wide_arrays.json | json | 14.648 | 17.577 | 20.347 | 90.375 | 0.09x |
| mixed.json | strata | 0.042 | 0.045 | 0.055 | 90.516 | 1.00x |
| mixed.json | orjson | 0.044 | 0.046 | 0.054 | 90.516 | 0.97x |
| mixed.json | msgspec | 0.053 | 0.058 | 0.069 | 90.516 | 0.77x |
| mixed.json | ujson | 0.225 | 0.232 | 0.255 | 90.516 | 0.19x |
| mixed.json | json | 0.361 | 0.377 | 0.465 | 90.516 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.015 | 11.342 | 18.965 | 83.938 | 1.00x |
| users.json | orjson | 11.425 | 13.778 | 24.787 | 83.938 | 0.82x |
| users.json | msgspec | 13.312 | 17.110 | 21.159 | 83.938 | 0.66x |
| users.json | ujson | 16.377 | 23.709 | 37.616 | 83.938 | 0.48x |
| users.json | json | 22.222 | 26.011 | 39.433 | 83.938 | 0.44x |
| flat.json | strata | 0.688 | 0.783 | 0.999 | 86.812 | 1.00x |
| flat.json | orjson | 0.932 | 1.169 | 2.061 | 86.812 | 0.67x |
| flat.json | msgspec | 0.848 | 1.001 | 1.313 | 86.812 | 0.78x |
| flat.json | ujson | 1.180 | 1.345 | 1.783 | 86.812 | 0.58x |
| flat.json | json | 1.467 | 1.729 | 2.102 | 86.812 | 0.45x |
| nested.json | strata | 0.622 | 0.718 | 1.063 | 86.812 | 1.00x |
| nested.json | orjson | 0.925 | 1.108 | 1.513 | 86.812 | 0.65x |
| nested.json | msgspec | 0.787 | 0.998 | 1.255 | 86.812 | 0.72x |
| nested.json | ujson | 1.092 | 1.377 | 1.644 | 86.812 | 0.52x |
| nested.json | json | 1.590 | 1.865 | 3.023 | 86.812 | 0.38x |
| wide_arrays.json | strata | 3.901 | 4.587 | 6.061 | 90.500 | 1.00x |
| wide_arrays.json | orjson | 4.129 | 5.007 | 7.125 | 90.500 | 0.92x |
| wide_arrays.json | msgspec | 4.660 | 5.524 | 11.135 | 90.500 | 0.83x |
| wide_arrays.json | ujson | 6.199 | 7.059 | 9.048 | 90.500 | 0.65x |
| wide_arrays.json | json | 7.800 | 8.773 | 10.511 | 90.500 | 0.52x |
| mixed.json | strata | 0.162 | 0.177 | 0.199 | 90.516 | 1.00x |
| mixed.json | orjson | 0.208 | 0.288 | 0.429 | 90.516 | 0.61x |
| mixed.json | msgspec | 0.215 | 0.235 | 0.338 | 90.516 | 0.75x |
| mixed.json | ujson | 0.260 | 0.284 | 0.840 | 90.516 | 0.62x |
| mixed.json | json | 0.371 | 0.398 | 0.509 | 90.516 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.471 | 11.612 | 17.868 | 86.797 | 1.00x |
| users.ndjson | orjson | 14.743 | 16.511 | 22.102 | 86.797 | 0.70x |
| users.ndjson | msgspec | 14.477 | 16.942 | 19.921 | 86.797 | 0.69x |
| users.ndjson | ujson | 16.173 | 20.409 | 27.532 | 86.797 | 0.57x |
| users.ndjson | json | 22.473 | 29.896 | 33.662 | 86.797 | 0.39x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.801 | 3.186 | 3.934 | 80.047 | 1.00x |
| users.json | orjson | 3.715 | 4.190 | 6.669 | 80.047 | 0.76x |
| users.json | msgspec | 4.248 | 4.660 | 5.481 | 80.047 | 0.68x |
| users.json | ujson | 15.270 | 16.519 | 21.180 | 80.047 | 0.19x |
| users.json | json | 22.118 | 24.820 | 30.641 | 80.047 | 0.13x |
| flat.json | strata | 0.605 | 0.759 | 0.818 | 86.812 | 1.00x |
| flat.json | orjson | 0.595 | 0.804 | 1.429 | 86.812 | 0.94x |
| flat.json | msgspec | 0.725 | 0.879 | 1.896 | 86.812 | 0.86x |
| flat.json | ujson | 1.319 | 1.778 | 3.061 | 86.812 | 0.43x |
| flat.json | json | 1.899 | 2.434 | 3.958 | 86.812 | 0.31x |
| nested.json | strata | 0.388 | 0.478 | 0.840 | 86.812 | 1.00x |
| nested.json | orjson | 0.526 | 0.576 | 0.972 | 86.812 | 0.83x |
| nested.json | msgspec | 0.560 | 0.766 | 0.880 | 86.812 | 0.62x |
| nested.json | ujson | 1.387 | 1.551 | 2.221 | 86.812 | 0.31x |
| nested.json | json | 1.920 | 2.321 | 3.151 | 86.812 | 0.21x |
| wide_arrays.json | strata | 1.515 | 1.678 | 2.013 | 90.500 | 1.00x |
| wide_arrays.json | orjson | 1.918 | 1.975 | 2.581 | 90.500 | 0.85x |
| wide_arrays.json | msgspec | 2.504 | 2.865 | 4.176 | 90.500 | 0.59x |
| wide_arrays.json | ujson | 7.171 | 7.688 | 9.684 | 90.500 | 0.22x |
| wide_arrays.json | json | 12.479 | 12.949 | 15.872 | 90.500 | 0.13x |
| mixed.json | strata | 0.145 | 0.204 | 0.315 | 90.516 | 1.00x |
| mixed.json | orjson | 0.156 | 0.212 | 0.338 | 90.516 | 0.96x |
| mixed.json | msgspec | 0.156 | 0.219 | 0.368 | 90.516 | 0.93x |
| mixed.json | ujson | 0.346 | 0.403 | 0.705 | 90.516 | 0.51x |
| mixed.json | json | 0.456 | 0.615 | 0.933 | 90.516 | 0.33x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.125 | 0.143 | 0.306 | 80.109 | 1.00x |
| users.json $[*].id | jmespath | 0.394 | 0.494 | 1.433 | 80.109 | 0.29x |
| users.json $[*].id | jsonpath-ng | 1.715 | 2.607 | 4.050 | 80.109 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.562 | 0.867 | 2.269 | 80.328 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.523 | 2.859 | 6.339 | 80.328 | 0.30x |
| users.json $[*].orders[*].total | jsonpath-ng | 14.854 | 17.780 | 20.452 | 80.328 | 0.05x |
| users.json $..total | strata | 1.706 | 2.165 | 3.538 | 82.359 | 1.00x |
| users.json $..total | jsonpath-ng | 290.458 | 355.590 | 410.300 | 82.359 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.273 | 4.749 | 5.094 | 80.234 | 1.00x |
| users.json $[*].id | orjson+jmespath | 11.794 | 14.004 | 20.149 | 80.234 | 0.34x |
| users.json $[*].id | orjson+jsonpath-ng | 13.115 | 15.297 | 21.929 | 80.234 | 0.31x |
| users.json $[*].orders[*].total | strata | 4.451 | 4.848 | 7.429 | 82.344 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 14.439 | 19.649 | 30.593 | 82.344 | 0.25x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 35.845 | 53.431 | 72.794 | 82.344 | 0.09x |
| users.json $..total | strata | 10.500 | 11.991 | 16.021 | 82.359 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 250.215 | 282.742 | 328.964 | 82.359 | 0.04x |

