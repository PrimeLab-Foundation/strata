# Benchmark results - large

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 71d959d
- python: 3.14.7
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit-Mach-O
- machine: arm64
- processor: Apple M1 Max
- compiler_flags: -std=c++20 -O3 -march=native
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 307.180 | 317.037 | 335.950 | 493.656 | 1.00x |
| users.json | orjson | 388.478 | 399.860 | 427.396 | 493.656 | 0.79x |
| users.json | msgspec | 399.442 | 409.551 | 420.103 | 493.656 | 0.77x |
| users.json | ujson | 544.989 | 557.265 | 565.409 | 493.656 | 0.57x |
| users.json | json | 801.781 | 805.748 | 823.554 | 493.656 | 0.39x |
| flat.json | strata | 6.322 | 6.371 | 6.423 | 355.062 | 1.00x |
| flat.json | orjson | 7.162 | 7.199 | 7.274 | 355.062 | 0.88x |
| flat.json | msgspec | 7.257 | 7.310 | 7.335 | 355.062 | 0.87x |
| flat.json | ujson | 10.796 | 10.896 | 10.920 | 355.062 | 0.58x |
| flat.json | json | 14.747 | 14.821 | 14.932 | 355.062 | 0.43x |
| nested.json | strata | 5.366 | 5.430 | 5.618 | 356.281 | 1.00x |
| nested.json | orjson | 6.062 | 6.148 | 6.523 | 356.281 | 0.88x |
| nested.json | msgspec | 6.020 | 6.052 | 6.475 | 356.281 | 0.90x |
| nested.json | ujson | 9.042 | 9.097 | 9.311 | 356.281 | 0.60x |
| nested.json | json | 13.841 | 13.910 | 14.684 | 356.281 | 0.39x |
| wide_arrays.json | strata | 31.008 | 31.546 | 32.736 | 427.203 | 1.00x |
| wide_arrays.json | orjson | 35.023 | 36.111 | 37.730 | 427.203 | 0.87x |
| wide_arrays.json | msgspec | 42.751 | 43.594 | 44.751 | 427.203 | 0.72x |
| wide_arrays.json | ujson | 54.943 | 56.041 | 57.202 | 427.203 | 0.56x |
| wide_arrays.json | json | 79.681 | 80.360 | 82.579 | 427.203 | 0.39x |
| mixed.json | strata | 1.274 | 1.281 | 1.328 | 914.125 | 1.00x |
| mixed.json | orjson | 1.479 | 1.516 | 1.564 | 914.125 | 0.85x |
| mixed.json | msgspec | 1.606 | 1.644 | 1.696 | 914.125 | 0.78x |
| mixed.json | ujson | 2.000 | 2.038 | 2.111 | 914.125 | 0.63x |
| mixed.json | json | 3.296 | 3.337 | 3.503 | 914.125 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 60.923 | 63.520 | 66.084 | 3565.812 | 1.00x |
| users.json | orjson | 87.901 | 88.902 | 90.082 | 3565.812 | 0.71x |
| users.json | msgspec | 114.139 | 115.571 | 118.336 | 3565.812 | 0.55x |
| users.json | ujson | 450.630 | 455.190 | 461.953 | 3565.812 | 0.14x |
| users.json | json | 632.899 | 634.497 | 641.530 | 3565.812 | 0.10x |
| flat.json | strata | 1.706 | 1.722 | 1.727 | 356.953 | 1.00x |
| flat.json | orjson | 2.141 | 2.153 | 2.173 | 356.953 | 0.80x |
| flat.json | msgspec | 2.821 | 2.846 | 2.873 | 356.953 | 0.61x |
| flat.json | ujson | 8.900 | 8.941 | 8.954 | 356.953 | 0.19x |
| flat.json | json | 13.231 | 13.319 | 13.406 | 356.953 | 0.13x |
| nested.json | strata | 1.334 | 1.354 | 1.432 | 356.281 | 1.00x |
| nested.json | orjson | 1.990 | 2.007 | 2.030 | 356.281 | 0.67x |
| nested.json | msgspec | 2.607 | 2.634 | 2.675 | 356.281 | 0.51x |
| nested.json | ujson | 9.516 | 9.560 | 9.613 | 356.281 | 0.14x |
| nested.json | json | 15.186 | 15.243 | 15.422 | 356.281 | 0.09x |
| wide_arrays.json | strata | 9.806 | 9.954 | 10.260 | 691.141 | 1.00x |
| wide_arrays.json | orjson | 11.853 | 12.080 | 12.614 | 691.141 | 0.82x |
| wide_arrays.json | msgspec | 19.848 | 20.039 | 20.228 | 691.141 | 0.50x |
| wide_arrays.json | ujson | 61.463 | 61.720 | 62.599 | 691.141 | 0.16x |
| wide_arrays.json | json | 113.832 | 115.118 | 117.861 | 691.141 | 0.09x |
| mixed.json | strata | 0.323 | 0.331 | 0.443 | 914.125 | 1.00x |
| mixed.json | orjson | 0.378 | 0.383 | 0.416 | 914.125 | 0.86x |
| mixed.json | msgspec | 0.412 | 0.423 | 0.481 | 914.125 | 0.78x |
| mixed.json | ujson | 2.129 | 2.174 | 2.303 | 914.125 | 0.15x |
| mixed.json | json | 3.321 | 3.554 | 4.243 | 914.125 | 0.09x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 306.886 | 327.435 | 361.150 | 902.594 | 1.00x |
| users.json | orjson | 394.505 | 403.929 | 438.454 | 902.594 | 0.81x |
| users.json | msgspec | 396.364 | 405.806 | 440.165 | 902.594 | 0.81x |
| users.json | ujson | 550.916 | 567.754 | 602.599 | 902.594 | 0.58x |
| users.json | json | 797.337 | 818.303 | 856.393 | 902.594 | 0.40x |
| flat.json | strata | 6.717 | 6.789 | 6.948 | 357.219 | 1.00x |
| flat.json | orjson | 7.456 | 7.684 | 7.840 | 357.219 | 0.88x |
| flat.json | msgspec | 7.829 | 7.897 | 8.006 | 357.219 | 0.86x |
| flat.json | ujson | 11.420 | 11.627 | 12.063 | 357.219 | 0.58x |
| flat.json | json | 15.235 | 15.514 | 16.176 | 357.219 | 0.44x |
| nested.json | strata | 5.687 | 5.764 | 5.959 | 359.188 | 1.00x |
| nested.json | orjson | 6.281 | 6.406 | 6.570 | 359.188 | 0.90x |
| nested.json | msgspec | 6.284 | 6.333 | 6.548 | 359.188 | 0.91x |
| nested.json | ujson | 9.411 | 9.507 | 9.731 | 359.188 | 0.61x |
| nested.json | json | 13.969 | 14.084 | 14.362 | 359.188 | 0.41x |
| wide_arrays.json | strata | 32.783 | 33.217 | 33.522 | 702.141 | 1.00x |
| wide_arrays.json | orjson | 37.024 | 37.476 | 39.178 | 702.141 | 0.89x |
| wide_arrays.json | msgspec | 44.822 | 45.428 | 46.633 | 702.141 | 0.73x |
| wide_arrays.json | ujson | 58.897 | 59.367 | 60.820 | 702.141 | 0.56x |
| wide_arrays.json | json | 82.348 | 82.751 | 84.055 | 702.141 | 0.40x |
| mixed.json | strata | 1.413 | 1.441 | 1.539 | 914.125 | 1.00x |
| mixed.json | orjson | 1.560 | 1.652 | 1.760 | 914.125 | 0.87x |
| mixed.json | msgspec | 1.724 | 1.804 | 1.946 | 914.125 | 0.80x |
| mixed.json | ujson | 2.140 | 2.279 | 2.444 | 914.125 | 0.63x |
| mixed.json | json | 3.412 | 3.503 | 3.684 | 914.125 | 0.41x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 302.763 | 304.653 | 307.834 | 385.406 | 1.00x |
| users.ndjson | orjson | 677.208 | 687.109 | 697.412 | 385.406 | 0.44x |
| users.ndjson | msgspec | 680.468 | 689.864 | 697.179 | 385.406 | 0.44x |
| users.ndjson | ujson | 849.265 | 858.824 | 879.606 | 385.406 | 0.35x |
| users.ndjson | json | 1019.977 | 1028.786 | 1052.265 | 385.406 | 0.30x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 69.964 | 84.566 | 106.779 | 1106.188 | 1.00x |
| users.json | orjson | 97.037 | 100.040 | 130.434 | 1106.188 | 0.85x |
| users.json | msgspec | 124.302 | 127.170 | 151.227 | 1106.188 | 0.66x |
| users.json | ujson | 451.376 | 458.626 | 480.404 | 1106.188 | 0.18x |
| users.json | json | 642.209 | 656.284 | 676.738 | 1106.188 | 0.13x |
| flat.json | strata | 2.068 | 2.195 | 2.684 | 357.234 | 1.00x |
| flat.json | orjson | 2.518 | 2.619 | 3.003 | 357.234 | 0.84x |
| flat.json | msgspec | 3.217 | 3.334 | 3.509 | 357.234 | 0.66x |
| flat.json | ujson | 8.985 | 9.280 | 9.643 | 357.234 | 0.24x |
| flat.json | json | 13.490 | 13.855 | 14.409 | 357.234 | 0.16x |
| nested.json | strata | 1.693 | 1.771 | 2.199 | 359.188 | 1.00x |
| nested.json | orjson | 2.421 | 2.552 | 2.684 | 359.188 | 0.69x |
| nested.json | msgspec | 3.060 | 3.108 | 3.705 | 359.188 | 0.57x |
| nested.json | ujson | 10.057 | 10.276 | 10.607 | 359.188 | 0.17x |
| nested.json | json | 15.932 | 16.204 | 16.908 | 359.188 | 0.11x |
| wide_arrays.json | strata | 11.973 | 12.288 | 13.316 | 917.078 | 1.00x |
| wide_arrays.json | orjson | 14.022 | 14.591 | 15.120 | 917.078 | 0.84x |
| wide_arrays.json | msgspec | 22.273 | 22.451 | 22.874 | 917.078 | 0.55x |
| wide_arrays.json | ujson | 63.800 | 64.485 | 65.971 | 917.078 | 0.19x |
| wide_arrays.json | json | 116.792 | 118.126 | 121.255 | 917.078 | 0.10x |
| mixed.json | strata | 0.550 | 0.585 | 0.700 | 914.125 | 1.00x |
| mixed.json | orjson | 0.570 | 0.615 | 0.696 | 914.125 | 0.95x |
| mixed.json | msgspec | 0.616 | 0.666 | 0.765 | 914.125 | 0.88x |
| mixed.json | ujson | 2.350 | 2.460 | 2.754 | 914.125 | 0.24x |
| mixed.json | json | 3.546 | 3.656 | 4.095 | 914.125 | 0.16x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.737 | 0.747 | 1.301 | 694.156 | 1.00x |
| users.json $[*].id | jmespath | 1.898 | 1.948 | 2.137 | 694.156 | 0.38x |
| users.json $[*].id | jsonpath-ng | 7.631 | 8.175 | 17.660 | 694.156 | 0.09x |
| users.json $[*].orders[*].total | strata | 14.131 | 14.392 | 17.117 | 605.625 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 31.382 | 33.618 | 35.227 | 605.625 | 0.43x |
| users.json $[*].orders[*].total | jsonpath-ng | 159.480 | 167.835 | 173.402 | 605.625 | 0.09x |
| users.json $..total | strata | 55.311 | 55.932 | 57.353 | 624.656 | 1.00x |
| users.json $..total | jsonpath-ng | 7792.592 | 7805.510 | 7846.381 | 624.656 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 138.078 | 139.168 | 142.671 | 1000.578 | 1.00x |
| users.json $[*].id | orjson+jmespath | 397.651 | 404.273 | 409.745 | 1000.578 | 0.34x |
| users.json $[*].id | orjson+jsonpath-ng | 484.403 | 492.273 | 495.630 | 1000.578 | 0.28x |
| users.json $[*].orders[*].total | strata | 139.030 | 139.709 | 143.333 | 848.531 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 429.885 | 436.095 | 442.634 | 848.531 | 0.32x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 1192.339 | 1211.448 | 1224.345 | 848.531 | 0.12x |
| users.json $..total | strata | 370.890 | 374.154 | 382.824 | 864.219 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 8766.478 | 8794.045 | 8815.287 | 864.219 | 0.04x |

