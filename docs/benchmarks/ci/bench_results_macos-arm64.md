# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 75b7e23
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
| users.json | strata | 8.301 | 9.797 | 15.560 | 63.828 | 1.00x |
| users.json | orjson | 11.488 | 13.464 | 20.312 | 63.828 | 0.73x |
| users.json | msgspec | 11.038 | 12.709 | 18.501 | 63.828 | 0.77x |
| users.json | ujson | 14.367 | 18.895 | 29.964 | 63.828 | 0.52x |
| users.json | pysimdjson | 147.274 | 178.868 | 210.429 | 63.828 | 0.05x |
| users.json | json | 18.140 | 27.218 | 31.595 | 63.828 | 0.36x |
| flat.json | strata | 0.711 | 1.055 | 1.834 | 91.234 | 1.00x |
| flat.json | orjson | 0.800 | 0.941 | 3.451 | 91.234 | 1.12x |
| flat.json | msgspec | 0.724 | 0.890 | 2.166 | 91.234 | 1.19x |
| flat.json | ujson | 1.185 | 1.546 | 9.292 | 91.234 | 0.68x |
| flat.json | pysimdjson | 12.309 | 15.447 | 26.118 | 91.234 | 0.07x |
| flat.json | json | 1.457 | 1.618 | 4.706 | 91.234 | 0.65x |
| nested.json | strata | 0.617 | 0.825 | 1.786 | 91.250 | 1.00x |
| nested.json | orjson | 0.787 | 0.998 | 1.319 | 91.250 | 0.83x |
| nested.json | msgspec | 0.767 | 0.962 | 2.097 | 91.250 | 0.86x |
| nested.json | ujson | 1.305 | 1.690 | 4.017 | 91.250 | 0.49x |
| nested.json | pysimdjson | 11.140 | 15.303 | 19.645 | 91.250 | 0.05x |
| nested.json | json | 1.515 | 1.929 | 3.756 | 91.250 | 0.43x |
| wide_arrays.json | strata | 4.214 | 5.424 | 10.142 | 94.016 | 1.00x |
| wide_arrays.json | orjson | 4.083 | 5.562 | 11.459 | 94.016 | 0.98x |
| wide_arrays.json | msgspec | 4.844 | 5.783 | 14.129 | 94.016 | 0.94x |
| wide_arrays.json | ujson | 7.456 | 8.273 | 10.743 | 94.016 | 0.66x |
| wide_arrays.json | pysimdjson | 86.964 | 102.187 | 109.951 | 94.016 | 0.05x |
| wide_arrays.json | json | 8.206 | 9.925 | 13.583 | 94.016 | 0.55x |
| mixed.json | strata | 0.148 | 0.176 | 0.227 | 94.234 | 1.00x |
| mixed.json | orjson | 0.165 | 0.211 | 0.316 | 94.234 | 0.83x |
| mixed.json | msgspec | 0.180 | 0.199 | 0.564 | 94.234 | 0.88x |
| mixed.json | ujson | 0.227 | 0.332 | 0.496 | 94.234 | 0.53x |
| mixed.json | pysimdjson | 2.635 | 3.057 | 3.405 | 94.234 | 0.06x |
| mixed.json | json | 0.350 | 0.375 | 0.600 | 94.234 | 0.47x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.185 | 3.013 | 4.712 | 77.938 | 1.00x |
| users.json | orjson | 2.972 | 4.858 | 6.166 | 77.938 | 0.62x |
| users.json | msgspec | 3.626 | 3.925 | 5.517 | 77.938 | 0.77x |
| users.json | ujson | 13.643 | 16.463 | 24.876 | 77.938 | 0.18x |
| users.json | json | 17.975 | 26.615 | 37.378 | 77.938 | 0.11x |
| flat.json | strata | 0.222 | 0.229 | 0.309 | 91.250 | 1.00x |
| flat.json | orjson | 0.298 | 0.380 | 0.544 | 91.250 | 0.60x |
| flat.json | msgspec | 0.336 | 0.364 | 0.401 | 91.250 | 0.63x |
| flat.json | ujson | 1.003 | 1.178 | 1.796 | 91.250 | 0.19x |
| flat.json | json | 1.423 | 1.614 | 2.847 | 91.250 | 0.14x |
| nested.json | strata | 0.176 | 0.190 | 0.565 | 91.250 | 1.00x |
| nested.json | orjson | 0.255 | 0.335 | 0.480 | 91.250 | 0.57x |
| nested.json | msgspec | 0.314 | 0.388 | 0.950 | 91.250 | 0.49x |
| nested.json | ujson | 1.094 | 1.520 | 1.686 | 91.250 | 0.13x |
| nested.json | json | 1.748 | 2.093 | 4.481 | 91.250 | 0.09x |
| wide_arrays.json | strata | 1.289 | 1.748 | 2.842 | 94.016 | 1.00x |
| wide_arrays.json | orjson | 1.610 | 1.957 | 2.709 | 94.016 | 0.89x |
| wide_arrays.json | msgspec | 2.409 | 2.792 | 3.065 | 94.016 | 0.63x |
| wide_arrays.json | ujson | 7.561 | 9.453 | 14.542 | 94.016 | 0.18x |
| wide_arrays.json | json | 12.952 | 17.245 | 23.119 | 94.016 | 0.10x |
| mixed.json | strata | 0.047 | 0.063 | 0.082 | 94.234 | 1.00x |
| mixed.json | orjson | 0.046 | 0.059 | 0.174 | 94.234 | 1.06x |
| mixed.json | msgspec | 0.056 | 0.067 | 0.263 | 94.234 | 0.94x |
| mixed.json | ujson | 0.229 | 0.277 | 0.648 | 94.234 | 0.23x |
| mixed.json | json | 0.381 | 0.440 | 0.573 | 94.234 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.368 | 10.565 | 18.228 | 88.734 | 1.00x |
| users.json | orjson | 11.522 | 15.909 | 23.865 | 88.734 | 0.66x |
| users.json | msgspec | 10.891 | 13.419 | 24.302 | 88.734 | 0.79x |
| users.json | ujson | 15.995 | 19.745 | 26.150 | 88.734 | 0.54x |
| users.json | json | 17.350 | 25.055 | 33.718 | 88.734 | 0.42x |
| flat.json | strata | 0.796 | 0.844 | 0.946 | 91.250 | 1.00x |
| flat.json | orjson | 0.982 | 1.073 | 1.267 | 91.250 | 0.79x |
| flat.json | msgspec | 0.882 | 0.965 | 1.056 | 91.250 | 0.87x |
| flat.json | ujson | 1.307 | 1.375 | 2.286 | 91.250 | 0.61x |
| flat.json | json | 1.568 | 1.702 | 2.084 | 91.250 | 0.50x |
| nested.json | strata | 0.879 | 1.012 | 1.309 | 91.250 | 1.00x |
| nested.json | orjson | 1.168 | 1.629 | 2.113 | 91.250 | 0.62x |
| nested.json | msgspec | 0.825 | 1.254 | 1.921 | 91.250 | 0.81x |
| nested.json | ujson | 1.154 | 1.572 | 2.256 | 91.250 | 0.64x |
| nested.json | json | 1.728 | 2.223 | 3.027 | 91.250 | 0.46x |
| wide_arrays.json | strata | 4.438 | 6.026 | 8.020 | 94.219 | 1.00x |
| wide_arrays.json | orjson | 4.432 | 5.610 | 7.488 | 94.219 | 1.07x |
| wide_arrays.json | msgspec | 5.116 | 8.297 | 12.906 | 94.219 | 0.73x |
| wide_arrays.json | ujson | 6.707 | 9.973 | 16.297 | 94.219 | 0.60x |
| wide_arrays.json | json | 8.558 | 10.236 | 14.997 | 94.219 | 0.59x |
| mixed.json | strata | 0.192 | 0.232 | 0.480 | 94.234 | 1.00x |
| mixed.json | orjson | 0.220 | 0.429 | 0.545 | 94.234 | 0.54x |
| mixed.json | msgspec | 0.234 | 0.289 | 0.382 | 94.234 | 0.80x |
| mixed.json | ujson | 0.296 | 0.384 | 0.642 | 94.234 | 0.60x |
| mixed.json | json | 0.411 | 0.503 | 0.592 | 94.234 | 0.46x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 8.349 | 11.280 | 13.828 | 91.234 | 1.00x |
| users.ndjson | orjson | 12.742 | 16.986 | 22.393 | 91.234 | 0.66x |
| users.ndjson | msgspec | 12.460 | 15.355 | 20.118 | 91.234 | 0.73x |
| users.ndjson | ujson | 15.612 | 19.784 | 29.163 | 91.234 | 0.57x |
| users.ndjson | json | 20.111 | 28.231 | 31.730 | 91.234 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.447 | 3.270 | 6.023 | 85.484 | 1.00x |
| users.json | orjson | 2.947 | 3.328 | 5.709 | 85.484 | 0.98x |
| users.json | msgspec | 3.588 | 3.953 | 6.058 | 85.484 | 0.83x |
| users.json | ujson | 11.908 | 13.143 | 16.147 | 85.484 | 0.25x |
| users.json | json | 16.578 | 20.365 | 33.809 | 85.484 | 0.16x |
| flat.json | strata | 0.548 | 0.703 | 3.329 | 91.250 | 1.00x |
| flat.json | orjson | 0.610 | 0.826 | 3.460 | 91.250 | 0.85x |
| flat.json | msgspec | 0.616 | 1.097 | 1.787 | 91.250 | 0.64x |
| flat.json | ujson | 1.338 | 1.529 | 3.532 | 91.250 | 0.46x |
| flat.json | json | 1.880 | 2.297 | 2.950 | 91.250 | 0.31x |
| nested.json | strata | 0.401 | 0.579 | 1.142 | 91.250 | 1.00x |
| nested.json | orjson | 0.461 | 0.691 | 0.972 | 91.250 | 0.84x |
| nested.json | msgspec | 0.478 | 0.756 | 1.699 | 91.250 | 0.76x |
| nested.json | ujson | 1.423 | 1.727 | 4.523 | 91.250 | 0.34x |
| nested.json | json | 1.946 | 2.467 | 2.719 | 91.250 | 0.23x |
| wide_arrays.json | strata | 2.116 | 2.191 | 3.198 | 94.219 | 1.00x |
| wide_arrays.json | orjson | 1.949 | 2.379 | 3.484 | 94.219 | 0.92x |
| wide_arrays.json | msgspec | 2.699 | 3.427 | 6.612 | 94.219 | 0.64x |
| wide_arrays.json | ujson | 8.080 | 9.909 | 14.472 | 94.219 | 0.22x |
| wide_arrays.json | json | 13.936 | 18.224 | 21.596 | 94.219 | 0.12x |
| mixed.json | strata | 0.178 | 0.272 | 0.628 | 94.234 | 1.00x |
| mixed.json | orjson | 0.205 | 0.289 | 0.535 | 94.234 | 0.94x |
| mixed.json | msgspec | 0.232 | 0.282 | 0.437 | 94.234 | 0.96x |
| mixed.json | ujson | 0.406 | 0.513 | 1.170 | 94.234 | 0.53x |
| mixed.json | json | 0.546 | 0.596 | 1.029 | 94.234 | 0.46x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.065 | 0.126 | 0.308 | 85.531 | 1.00x |
| users.json $[*].id | jmespath | 0.328 | 0.456 | 0.909 | 85.531 | 0.28x |
| users.json $[*].id | jsonpath-ng | 1.624 | 2.068 | 3.450 | 85.531 | 0.06x |
| users.json $[*].orders[*].total | strata | 0.364 | 0.511 | 0.786 | 85.672 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.751 | 2.104 | 4.211 | 85.672 | 0.24x |
| users.json $[*].orders[*].total | jsonpath-ng | 10.828 | 12.022 | 17.106 | 85.672 | 0.04x |
| users.json $..total | strata | 1.338 | 1.641 | 2.354 | 87.609 | 1.00x |
| users.json $..total | jsonpath-ng | 205.478 | 216.279 | 231.519 | 87.609 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.758 | 4.435 | 5.986 | 85.609 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.294 | 12.965 | 18.392 | 85.609 | 0.34x |
| users.json $[*].id | orjson+jsonpath-ng | 11.668 | 15.433 | 22.667 | 85.609 | 0.29x |
| users.json $[*].orders[*].total | strata | 3.951 | 4.216 | 4.854 | 87.609 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.464 | 13.293 | 19.008 | 87.609 | 0.32x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 25.463 | 29.089 | 34.938 | 87.609 | 0.14x |
| users.json $..total | strata | 10.028 | 11.339 | 21.017 | 87.609 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 253.259 | 292.847 | 367.012 | 87.609 | 0.04x |

