# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 060bd7b
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
| users.json | strata | 7.603 | 9.248 | 13.603 | 63.562 | 1.00x |
| users.json | orjson | 11.442 | 14.026 | 21.968 | 63.562 | 0.66x |
| users.json | msgspec | 10.701 | 14.428 | 23.252 | 63.562 | 0.64x |
| users.json | ujson | 14.198 | 18.253 | 30.124 | 63.562 | 0.51x |
| users.json | pysimdjson | 140.508 | 171.301 | 198.360 | 63.562 | 0.05x |
| users.json | json | 19.417 | 20.527 | 37.759 | 63.562 | 0.45x |
| flat.json | strata | 0.657 | 0.731 | 0.979 | 93.125 | 1.00x |
| flat.json | orjson | 0.843 | 0.946 | 1.644 | 93.125 | 0.77x |
| flat.json | msgspec | 0.737 | 0.879 | 1.071 | 93.125 | 0.83x |
| flat.json | ujson | 1.215 | 1.440 | 3.080 | 93.125 | 0.51x |
| flat.json | pysimdjson | 13.098 | 15.121 | 17.002 | 93.125 | 0.05x |
| flat.json | json | 1.425 | 1.638 | 1.791 | 93.125 | 0.45x |
| nested.json | strata | 0.545 | 0.614 | 0.764 | 93.125 | 1.00x |
| nested.json | orjson | 0.762 | 0.827 | 1.099 | 93.125 | 0.74x |
| nested.json | msgspec | 0.710 | 0.757 | 0.965 | 93.125 | 0.81x |
| nested.json | ujson | 1.091 | 1.168 | 1.524 | 93.125 | 0.53x |
| nested.json | pysimdjson | 10.702 | 10.988 | 12.601 | 93.125 | 0.06x |
| nested.json | json | 1.433 | 1.533 | 1.830 | 93.125 | 0.40x |
| wide_arrays.json | strata | 3.593 | 4.603 | 6.433 | 96.922 | 1.00x |
| wide_arrays.json | orjson | 3.973 | 4.658 | 7.087 | 96.922 | 0.99x |
| wide_arrays.json | msgspec | 4.261 | 5.737 | 8.820 | 96.922 | 0.80x |
| wide_arrays.json | ujson | 6.147 | 6.494 | 9.738 | 96.922 | 0.71x |
| wide_arrays.json | pysimdjson | 69.050 | 78.271 | 112.658 | 96.922 | 0.06x |
| wide_arrays.json | json | 7.403 | 8.820 | 15.373 | 96.922 | 0.52x |
| mixed.json | strata | 0.136 | 0.168 | 0.264 | 97.344 | 1.00x |
| mixed.json | orjson | 0.163 | 0.190 | 0.398 | 97.344 | 0.88x |
| mixed.json | msgspec | 0.174 | 0.207 | 0.248 | 97.344 | 0.81x |
| mixed.json | ujson | 0.229 | 0.269 | 0.523 | 97.344 | 0.63x |
| mixed.json | pysimdjson | 2.536 | 3.177 | 3.720 | 97.344 | 0.05x |
| mixed.json | json | 0.340 | 0.394 | 0.435 | 97.344 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.975 | 2.111 | 2.994 | 77.875 | 1.00x |
| users.json | orjson | 2.644 | 2.926 | 3.606 | 77.875 | 0.72x |
| users.json | msgspec | 3.511 | 3.727 | 4.278 | 77.875 | 0.57x |
| users.json | ujson | 12.935 | 13.931 | 18.522 | 77.875 | 0.15x |
| users.json | json | 19.170 | 20.063 | 27.726 | 77.875 | 0.11x |
| flat.json | strata | 0.221 | 0.275 | 0.310 | 93.125 | 1.00x |
| flat.json | orjson | 0.261 | 0.306 | 0.476 | 93.125 | 0.90x |
| flat.json | msgspec | 0.346 | 0.375 | 0.406 | 93.125 | 0.73x |
| flat.json | ujson | 0.920 | 1.049 | 1.788 | 93.125 | 0.26x |
| flat.json | json | 1.377 | 1.490 | 1.787 | 93.125 | 0.18x |
| nested.json | strata | 0.139 | 0.145 | 0.185 | 93.125 | 1.00x |
| nested.json | orjson | 0.233 | 0.251 | 0.324 | 93.125 | 0.58x |
| nested.json | msgspec | 0.427 | 0.447 | 0.518 | 93.125 | 0.32x |
| nested.json | ujson | 1.076 | 1.128 | 1.258 | 93.125 | 0.13x |
| nested.json | json | 1.687 | 1.741 | 1.855 | 93.125 | 0.08x |
| wide_arrays.json | strata | 1.118 | 1.283 | 1.804 | 96.938 | 1.00x |
| wide_arrays.json | orjson | 1.352 | 1.621 | 2.090 | 96.938 | 0.79x |
| wide_arrays.json | msgspec | 2.355 | 2.679 | 2.873 | 96.938 | 0.48x |
| wide_arrays.json | ujson | 6.815 | 7.287 | 8.358 | 96.938 | 0.18x |
| wide_arrays.json | json | 12.544 | 13.110 | 14.808 | 96.938 | 0.10x |
| mixed.json | strata | 0.049 | 0.057 | 0.095 | 97.344 | 1.00x |
| mixed.json | orjson | 0.055 | 0.063 | 0.072 | 97.344 | 0.91x |
| mixed.json | msgspec | 0.068 | 0.077 | 0.136 | 97.344 | 0.74x |
| mixed.json | ujson | 0.260 | 0.293 | 0.719 | 97.344 | 0.20x |
| mixed.json | json | 0.409 | 0.466 | 0.543 | 97.344 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.142 | 9.412 | 12.520 | 88.078 | 1.00x |
| users.json | orjson | 12.610 | 13.066 | 13.682 | 88.078 | 0.72x |
| users.json | msgspec | 11.925 | 12.865 | 16.377 | 88.078 | 0.73x |
| users.json | ujson | 16.761 | 18.982 | 22.360 | 88.078 | 0.50x |
| users.json | json | 19.264 | 20.707 | 24.820 | 88.078 | 0.45x |
| flat.json | strata | 0.755 | 0.837 | 0.932 | 93.125 | 1.00x |
| flat.json | orjson | 1.103 | 1.209 | 1.522 | 93.125 | 0.69x |
| flat.json | msgspec | 0.937 | 1.027 | 1.358 | 93.125 | 0.81x |
| flat.json | ujson | 1.358 | 1.485 | 1.718 | 93.125 | 0.56x |
| flat.json | json | 1.604 | 1.806 | 1.978 | 93.125 | 0.46x |
| nested.json | strata | 0.588 | 0.628 | 0.740 | 93.125 | 1.00x |
| nested.json | orjson | 0.918 | 0.973 | 1.071 | 93.125 | 0.65x |
| nested.json | msgspec | 0.761 | 0.820 | 0.849 | 93.125 | 0.77x |
| nested.json | ujson | 1.047 | 1.155 | 1.253 | 93.125 | 0.54x |
| nested.json | json | 1.462 | 1.520 | 1.620 | 93.125 | 0.41x |
| wide_arrays.json | strata | 3.991 | 4.618 | 4.984 | 96.938 | 1.00x |
| wide_arrays.json | orjson | 4.254 | 5.132 | 5.377 | 96.938 | 0.90x |
| wide_arrays.json | msgspec | 5.023 | 5.756 | 6.545 | 96.938 | 0.80x |
| wide_arrays.json | ujson | 6.465 | 7.515 | 8.206 | 96.938 | 0.61x |
| wide_arrays.json | json | 7.733 | 8.854 | 9.659 | 96.938 | 0.52x |
| mixed.json | strata | 0.203 | 0.266 | 0.359 | 97.344 | 1.00x |
| mixed.json | orjson | 0.263 | 0.338 | 0.451 | 97.344 | 0.79x |
| mixed.json | msgspec | 0.268 | 0.370 | 0.543 | 97.344 | 0.72x |
| mixed.json | ujson | 0.331 | 0.488 | 0.643 | 97.344 | 0.54x |
| mixed.json | json | 0.445 | 0.546 | 0.633 | 97.344 | 0.49x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.756 | 8.265 | 9.688 | 93.125 | 1.00x |
| users.ndjson | orjson | 11.993 | 14.698 | 16.596 | 93.125 | 0.56x |
| users.ndjson | msgspec | 14.089 | 15.530 | 18.584 | 93.125 | 0.53x |
| users.ndjson | ujson | 17.674 | 19.135 | 28.565 | 93.125 | 0.43x |
| users.ndjson | json | 20.896 | 23.831 | 26.214 | 93.125 | 0.35x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.182 | 2.586 | 3.086 | 85.375 | 1.00x |
| users.json | orjson | 2.758 | 3.517 | 4.842 | 85.375 | 0.74x |
| users.json | msgspec | 3.835 | 4.308 | 5.036 | 85.375 | 0.60x |
| users.json | ujson | 13.102 | 15.035 | 18.291 | 85.375 | 0.17x |
| users.json | json | 19.126 | 21.687 | 23.722 | 85.375 | 0.12x |
| flat.json | strata | 0.416 | 0.626 | 1.290 | 93.125 | 1.00x |
| flat.json | orjson | 0.426 | 0.634 | 1.242 | 93.125 | 0.99x |
| flat.json | msgspec | 0.505 | 0.803 | 1.135 | 93.125 | 0.78x |
| flat.json | ujson | 1.167 | 1.643 | 3.620 | 93.125 | 0.38x |
| flat.json | json | 1.596 | 2.024 | 3.478 | 93.125 | 0.31x |
| nested.json | strata | 0.311 | 0.463 | 0.816 | 93.125 | 1.00x |
| nested.json | orjson | 0.447 | 0.552 | 0.914 | 93.125 | 0.84x |
| nested.json | msgspec | 0.637 | 1.000 | 2.997 | 93.125 | 0.46x |
| nested.json | ujson | 1.404 | 1.797 | 2.827 | 93.125 | 0.26x |
| nested.json | json | 1.940 | 2.122 | 3.227 | 93.125 | 0.22x |
| wide_arrays.json | strata | 1.619 | 1.910 | 2.385 | 97.328 | 1.00x |
| wide_arrays.json | orjson | 2.089 | 2.542 | 3.732 | 97.328 | 0.75x |
| wide_arrays.json | msgspec | 2.864 | 3.315 | 4.215 | 97.328 | 0.58x |
| wide_arrays.json | ujson | 8.058 | 9.504 | 11.395 | 97.328 | 0.20x |
| wide_arrays.json | json | 14.438 | 15.547 | 17.453 | 97.328 | 0.12x |
| mixed.json | strata | 0.302 | 0.342 | 0.388 | 97.344 | 1.00x |
| mixed.json | orjson | 0.331 | 0.378 | 0.851 | 97.344 | 0.90x |
| mixed.json | msgspec | 0.327 | 0.522 | 1.637 | 97.344 | 0.65x |
| mixed.json | ujson | 0.523 | 0.628 | 1.562 | 97.344 | 0.54x |
| mixed.json | json | 0.657 | 0.807 | 1.386 | 97.344 | 0.42x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.053 | 0.084 | 0.154 | 85.453 | 1.00x |
| users.json $[*].id | jmespath | 0.287 | 0.399 | 0.522 | 85.453 | 0.21x |
| users.json $[*].id | jsonpath-ng | 1.583 | 1.885 | 2.090 | 85.453 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.401 | 0.669 | 0.857 | 86.609 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.006 | 2.597 | 2.967 | 86.609 | 0.26x |
| users.json $[*].orders[*].total | jsonpath-ng | 11.197 | 15.198 | 17.151 | 86.609 | 0.04x |
| users.json $..total | strata | 1.494 | 1.723 | 2.019 | 88.453 | 1.00x |
| users.json $..total | jsonpath-ng | 227.234 | 261.882 | 300.667 | 88.453 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.956 | 4.685 | 5.957 | 86.516 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.604 | 13.163 | 14.968 | 86.516 | 0.36x |
| users.json $[*].id | orjson+jsonpath-ng | 12.487 | 14.267 | 15.212 | 86.516 | 0.33x |
| users.json $[*].orders[*].total | strata | 4.156 | 4.915 | 5.202 | 88.453 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 11.816 | 14.494 | 29.710 | 88.453 | 0.34x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 23.117 | 37.072 | 42.114 | 88.453 | 0.13x |
| users.json $..total | strata | 9.881 | 10.669 | 12.367 | 88.484 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 233.585 | 254.962 | 292.214 | 88.484 | 0.04x |

