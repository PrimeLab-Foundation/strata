# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c3d92f5
- python: 3.12.10
- implementation: CPython
- platform: macOS-15.7.9-x86_64-i386-64bit
- machine: x86_64
- processor: Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 22.953 | 24.468 | 35.265 | 52.637 | 1.00x |
| users.json | orjson | 30.675 | 32.638 | 40.326 | 52.637 | 0.75x |
| users.json | msgspec | 31.176 | 32.999 | 36.653 | 52.637 | 0.74x |
| users.json | ujson | 43.833 | 48.495 | 52.915 | 52.637 | 0.50x |
| users.json | pysimdjson | 197.660 | 207.081 | 214.697 | 52.637 | 0.12x |
| users.json | json | 52.017 | 54.225 | 61.004 | 52.637 | 0.45x |
| flat.json | strata | 1.470 | 1.607 | 1.706 | 60.164 | 1.00x |
| flat.json | orjson | 1.661 | 1.783 | 2.216 | 60.164 | 0.90x |
| flat.json | msgspec | 1.914 | 1.999 | 2.335 | 60.164 | 0.80x |
| flat.json | ujson | 3.289 | 3.483 | 4.437 | 60.164 | 0.46x |
| flat.json | pysimdjson | 17.876 | 19.147 | 21.523 | 60.164 | 0.08x |
| flat.json | json | 3.740 | 4.149 | 5.169 | 60.164 | 0.39x |
| nested.json | strata | 1.900 | 2.009 | 2.848 | 48.348 | 1.00x |
| nested.json | orjson | 2.180 | 2.330 | 3.139 | 48.348 | 0.86x |
| nested.json | msgspec | 2.402 | 2.545 | 4.665 | 48.348 | 0.79x |
| nested.json | ujson | 3.981 | 4.269 | 5.818 | 48.348 | 0.47x |
| nested.json | pysimdjson | 17.280 | 18.578 | 20.751 | 48.348 | 0.11x |
| nested.json | json | 5.151 | 5.429 | 6.309 | 48.348 | 0.37x |
| wide_arrays.json | strata | 11.554 | 12.358 | 18.151 | 59.812 | 1.00x |
| wide_arrays.json | orjson | 12.253 | 12.925 | 17.001 | 59.812 | 0.96x |
| wide_arrays.json | msgspec | 13.558 | 14.243 | 21.133 | 59.812 | 0.87x |
| wide_arrays.json | ujson | 17.635 | 18.785 | 20.711 | 59.812 | 0.66x |
| wide_arrays.json | pysimdjson | 107.769 | 113.765 | 118.442 | 59.812 | 0.11x |
| wide_arrays.json | json | 22.952 | 23.578 | 37.378 | 59.812 | 0.52x |
| mixed.json | strata | 0.492 | 0.509 | 0.860 | 56.105 | 1.00x |
| mixed.json | orjson | 0.594 | 0.599 | 0.655 | 56.105 | 0.85x |
| mixed.json | msgspec | 0.619 | 0.629 | 0.725 | 56.105 | 0.81x |
| mixed.json | ujson | 0.855 | 0.882 | 1.411 | 56.105 | 0.58x |
| mixed.json | pysimdjson | 4.351 | 4.506 | 5.510 | 56.105 | 0.11x |
| mixed.json | json | 1.193 | 1.283 | 1.904 | 56.105 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.944 | 3.160 | 3.301 | 47.316 | 1.00x |
| users.json | orjson | 3.855 | 4.078 | 4.278 | 47.316 | 0.77x |
| users.json | msgspec | 6.638 | 6.973 | 7.404 | 47.316 | 0.45x |
| users.json | ujson | 36.199 | 36.586 | 39.118 | 47.316 | 0.09x |
| users.json | json | 48.960 | 49.476 | 50.802 | 47.316 | 0.06x |
| flat.json | strata | 0.383 | 0.455 | 0.598 | 47.969 | 1.00x |
| flat.json | orjson | 0.503 | 0.543 | 0.674 | 47.969 | 0.84x |
| flat.json | msgspec | 0.788 | 0.864 | 0.976 | 47.969 | 0.53x |
| flat.json | ujson | 3.167 | 3.677 | 5.363 | 47.969 | 0.12x |
| flat.json | json | 4.582 | 5.260 | 7.242 | 47.969 | 0.09x |
| nested.json | strata | 0.337 | 0.350 | 0.470 | 48.230 | 1.00x |
| nested.json | orjson | 0.492 | 0.511 | 0.695 | 48.230 | 0.68x |
| nested.json | msgspec | 0.737 | 0.770 | 0.873 | 48.230 | 0.45x |
| nested.json | ujson | 3.797 | 3.847 | 4.359 | 48.230 | 0.09x |
| nested.json | json | 6.225 | 6.467 | 7.040 | 48.230 | 0.05x |
| wide_arrays.json | strata | 2.520 | 2.757 | 4.556 | 59.211 | 1.00x |
| wide_arrays.json | orjson | 3.113 | 3.610 | 7.244 | 59.211 | 0.76x |
| wide_arrays.json | msgspec | 4.336 | 4.822 | 9.291 | 59.211 | 0.57x |
| wide_arrays.json | ujson | 20.606 | 24.450 | 32.628 | 59.211 | 0.11x |
| wide_arrays.json | json | 46.677 | 50.861 | 63.257 | 59.211 | 0.05x |
| mixed.json | strata | 0.117 | 0.139 | 0.190 | 53.895 | 1.00x |
| mixed.json | orjson | 0.112 | 0.125 | 0.239 | 53.895 | 1.11x |
| mixed.json | msgspec | 0.158 | 0.171 | 0.330 | 53.895 | 0.81x |
| mixed.json | ujson | 0.845 | 0.903 | 1.299 | 53.895 | 0.15x |
| mixed.json | json | 1.321 | 1.485 | 2.066 | 53.895 | 0.09x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 22.317 | 23.653 | 26.032 | 59.672 | 1.00x |
| users.json | orjson | 30.040 | 34.385 | 35.255 | 59.672 | 0.69x |
| users.json | msgspec | 30.220 | 33.055 | 36.392 | 59.672 | 0.72x |
| users.json | ujson | 45.515 | 48.723 | 61.336 | 59.672 | 0.49x |
| users.json | json | 50.894 | 54.892 | 60.383 | 59.672 | 0.43x |
| flat.json | strata | 1.919 | 2.396 | 3.232 | 48.109 | 1.00x |
| flat.json | orjson | 2.008 | 2.298 | 3.325 | 48.109 | 1.04x |
| flat.json | msgspec | 2.210 | 2.628 | 3.031 | 48.109 | 0.91x |
| flat.json | ujson | 3.816 | 4.717 | 5.241 | 48.109 | 0.51x |
| flat.json | json | 4.711 | 5.469 | 11.357 | 48.109 | 0.44x |
| nested.json | strata | 2.094 | 2.357 | 3.203 | 48.230 | 1.00x |
| nested.json | orjson | 2.441 | 2.486 | 3.409 | 48.230 | 0.95x |
| nested.json | msgspec | 2.745 | 2.948 | 3.808 | 48.230 | 0.80x |
| nested.json | ujson | 4.335 | 4.894 | 5.878 | 48.230 | 0.48x |
| nested.json | json | 5.456 | 6.132 | 6.974 | 48.230 | 0.38x |
| wide_arrays.json | strata | 12.037 | 13.314 | 17.694 | 59.211 | 1.00x |
| wide_arrays.json | orjson | 12.740 | 14.047 | 17.797 | 59.211 | 0.95x |
| wide_arrays.json | msgspec | 14.558 | 16.009 | 18.654 | 59.211 | 0.83x |
| wide_arrays.json | ujson | 19.219 | 21.575 | 25.369 | 59.211 | 0.62x |
| wide_arrays.json | json | 24.174 | 26.446 | 31.266 | 59.211 | 0.50x |
| mixed.json | strata | 0.589 | 0.652 | 0.917 | 53.895 | 1.00x |
| mixed.json | orjson | 0.740 | 0.848 | 1.265 | 53.895 | 0.77x |
| mixed.json | msgspec | 0.802 | 0.998 | 1.328 | 53.895 | 0.65x |
| mixed.json | ujson | 1.088 | 1.192 | 1.508 | 53.895 | 0.55x |
| mixed.json | json | 1.361 | 1.586 | 2.192 | 53.895 | 0.41x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 21.322 | 21.864 | 26.117 | 59.129 | 1.00x |
| users.ndjson | orjson | 29.348 | 30.742 | 34.805 | 59.129 | 0.71x |
| users.ndjson | msgspec | 29.912 | 30.489 | 39.787 | 59.129 | 0.72x |
| users.ndjson | ujson | 41.522 | 43.099 | 50.699 | 59.129 | 0.51x |
| users.ndjson | json | 52.888 | 54.756 | 63.828 | 59.129 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.214 | 4.372 | 4.775 | 53.328 | 1.00x |
| users.json | orjson | 4.947 | 5.193 | 5.476 | 53.328 | 0.84x |
| users.json | msgspec | 7.383 | 7.666 | 8.573 | 53.328 | 0.57x |
| users.json | ujson | 39.077 | 41.107 | 42.650 | 53.328 | 0.11x |
| users.json | json | 52.049 | 54.336 | 56.897 | 53.328 | 0.08x |
| flat.json | strata | 0.805 | 1.049 | 1.182 | 48.109 | 1.00x |
| flat.json | orjson | 0.989 | 1.142 | 1.465 | 48.109 | 0.92x |
| flat.json | msgspec | 1.227 | 1.419 | 2.014 | 48.109 | 0.74x |
| flat.json | ujson | 3.908 | 4.969 | 5.512 | 48.109 | 0.21x |
| flat.json | json | 5.785 | 6.433 | 8.207 | 48.109 | 0.16x |
| nested.json | strata | 0.738 | 0.815 | 1.253 | 48.230 | 1.00x |
| nested.json | orjson | 0.933 | 1.142 | 1.369 | 48.230 | 0.71x |
| nested.json | msgspec | 1.302 | 1.398 | 2.232 | 48.230 | 0.58x |
| nested.json | ujson | 4.374 | 4.702 | 7.125 | 48.230 | 0.17x |
| nested.json | json | 6.873 | 7.536 | 9.446 | 48.230 | 0.11x |
| wide_arrays.json | strata | 3.371 | 3.455 | 4.414 | 59.211 | 1.00x |
| wide_arrays.json | orjson | 4.181 | 4.547 | 5.678 | 59.211 | 0.76x |
| wide_arrays.json | msgspec | 5.297 | 5.928 | 7.010 | 59.211 | 0.58x |
| wide_arrays.json | ujson | 22.320 | 25.702 | 31.056 | 59.211 | 0.13x |
| wide_arrays.json | json | 47.522 | 52.509 | 68.953 | 59.211 | 0.07x |
| mixed.json | strata | 0.461 | 0.557 | 0.625 | 53.895 | 1.00x |
| mixed.json | orjson | 0.519 | 0.647 | 0.787 | 53.895 | 0.86x |
| mixed.json | msgspec | 0.539 | 0.649 | 0.929 | 53.895 | 0.86x |
| mixed.json | ujson | 1.383 | 1.762 | 3.391 | 53.895 | 0.32x |
| mixed.json | json | 1.844 | 2.543 | 112.514 | 53.895 | 0.22x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.159 | 0.179 | 0.213 | 53.406 | 1.00x |
| users.json $[*].id | jmespath | 1.096 | 1.158 | 1.209 | 53.406 | 0.15x |
| users.json $[*].id | jsonpath-ng | 6.127 | 6.338 | 6.644 | 53.406 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.975 | 1.101 | 1.286 | 55.234 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 6.784 | 7.044 | 7.590 | 55.234 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 41.104 | 42.482 | 45.384 | 55.234 | 0.03x |
| users.json $..total | strata | 3.345 | 3.794 | 4.424 | 57.242 | 1.00x |
| users.json $..total | jsonpath-ng | 765.308 | 861.524 | 1236.989 | 57.242 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.519 | 4.635 | 5.905 | 58.242 | 1.00x |
| users.json $[*].id | orjson+jmespath | 31.426 | 33.197 | 47.234 | 58.242 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 35.076 | 38.163 | 44.969 | 58.242 | 0.12x |
| users.json $[*].orders[*].total | strata | 4.888 | 4.934 | 8.040 | 56.426 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 36.181 | 37.500 | 48.845 | 56.426 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 75.254 | 79.725 | 105.473 | 56.426 | 0.06x |
| users.json $..total | strata | 26.474 | 27.255 | 28.695 | 56.465 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 836.257 | 876.881 | 897.877 | 56.465 | 0.03x |

