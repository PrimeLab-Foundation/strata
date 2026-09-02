# Benchmark results - large

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 8cdb525
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
| users.json | strata | 328.806 | 334.107 | 366.689 | 493.453 | 1.00x |
| users.json | orjson | 375.072 | 384.510 | 418.078 | 493.453 | 0.87x |
| users.json | msgspec | 383.327 | 390.240 | 408.611 | 493.453 | 0.86x |
| users.json | ujson | 520.746 | 531.538 | 554.796 | 493.453 | 0.63x |
| users.json | json | 777.031 | 786.120 | 876.586 | 493.453 | 0.43x |
| flat.json | strata | 6.815 | 6.894 | 7.043 | 352.062 | 1.00x |
| flat.json | orjson | 7.099 | 7.239 | 7.518 | 352.062 | 0.95x |
| flat.json | msgspec | 7.294 | 7.581 | 7.681 | 352.062 | 0.91x |
| flat.json | ujson | 10.839 | 11.220 | 11.557 | 352.062 | 0.61x |
| flat.json | json | 14.766 | 15.217 | 15.368 | 352.062 | 0.45x |
| nested.json | strata | 6.206 | 6.396 | 6.502 | 353.984 | 1.00x |
| nested.json | orjson | 6.173 | 6.318 | 6.565 | 353.984 | 1.01x |
| nested.json | msgspec | 6.105 | 6.357 | 6.489 | 353.984 | 1.01x |
| nested.json | ujson | 9.127 | 9.357 | 9.442 | 353.984 | 0.68x |
| nested.json | json | 14.052 | 14.486 | 14.952 | 353.984 | 0.44x |
| wide_arrays.json | strata | 34.187 | 34.376 | 34.839 | 425.172 | 1.00x |
| wide_arrays.json | orjson | 35.022 | 35.364 | 35.894 | 425.172 | 0.97x |
| wide_arrays.json | msgspec | 42.640 | 42.840 | 43.678 | 425.172 | 0.80x |
| wide_arrays.json | ujson | 54.891 | 55.218 | 56.261 | 425.172 | 0.62x |
| wide_arrays.json | json | 79.204 | 79.551 | 81.536 | 425.172 | 0.43x |
| mixed.json | strata | 1.386 | 1.411 | 1.485 | 778.250 | 1.00x |
| mixed.json | orjson | 1.477 | 1.493 | 1.594 | 778.250 | 0.95x |
| mixed.json | msgspec | 1.605 | 1.641 | 1.735 | 778.250 | 0.86x |
| mixed.json | ujson | 1.998 | 2.035 | 2.141 | 778.250 | 0.69x |
| mixed.json | json | 3.281 | 3.362 | 3.575 | 778.250 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 72.723 | 74.673 | 78.205 | 2140.125 | 1.00x |
| users.json | orjson | 85.090 | 88.052 | 90.804 | 2140.125 | 0.85x |
| users.json | msgspec | 112.056 | 117.810 | 120.939 | 2140.125 | 0.63x |
| users.json | ujson | 445.392 | 453.875 | 478.746 | 2140.125 | 0.16x |
| users.json | json | 627.713 | 634.670 | 660.220 | 2140.125 | 0.12x |
| flat.json | strata | 2.017 | 2.039 | 2.069 | 353.969 | 1.00x |
| flat.json | orjson | 2.136 | 2.173 | 2.305 | 353.969 | 0.94x |
| flat.json | msgspec | 2.821 | 2.853 | 2.951 | 353.969 | 0.71x |
| flat.json | ujson | 8.758 | 8.825 | 9.125 | 353.969 | 0.23x |
| flat.json | json | 13.152 | 13.309 | 13.770 | 353.969 | 0.15x |
| nested.json | strata | 1.405 | 1.444 | 1.569 | 353.984 | 1.00x |
| nested.json | orjson | 2.012 | 2.076 | 2.207 | 353.984 | 0.70x |
| nested.json | msgspec | 2.661 | 2.699 | 2.854 | 353.984 | 0.54x |
| nested.json | ujson | 9.717 | 9.796 | 10.128 | 353.984 | 0.15x |
| nested.json | json | 15.737 | 15.841 | 16.010 | 353.984 | 0.09x |
| wide_arrays.json | strata | 11.726 | 11.947 | 12.294 | 629.828 | 1.00x |
| wide_arrays.json | orjson | 11.807 | 11.978 | 12.533 | 629.828 | 1.00x |
| wide_arrays.json | msgspec | 20.507 | 20.637 | 21.206 | 629.828 | 0.58x |
| wide_arrays.json | ujson | 63.197 | 63.591 | 63.952 | 629.828 | 0.19x |
| wide_arrays.json | json | 112.970 | 114.687 | 116.013 | 629.828 | 0.10x |
| mixed.json | strata | 0.368 | 0.373 | 0.391 | 778.266 | 1.00x |
| mixed.json | orjson | 0.384 | 0.397 | 0.467 | 778.266 | 0.94x |
| mixed.json | msgspec | 0.426 | 0.434 | 0.444 | 778.266 | 0.86x |
| mixed.json | ujson | 2.110 | 2.147 | 2.184 | 778.266 | 0.17x |
| mixed.json | json | 3.344 | 3.413 | 3.560 | 778.266 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 338.915 | 345.500 | 352.869 | 902.453 | 1.00x |
| users.json | orjson | 384.147 | 400.796 | 418.267 | 902.453 | 0.86x |
| users.json | msgspec | 390.812 | 402.752 | 412.711 | 902.453 | 0.86x |
| users.json | ujson | 545.255 | 557.136 | 577.185 | 902.453 | 0.62x |
| users.json | json | 787.512 | 799.438 | 823.021 | 902.453 | 0.43x |
| flat.json | strata | 7.154 | 7.304 | 7.415 | 353.969 | 1.00x |
| flat.json | orjson | 7.350 | 7.470 | 7.613 | 353.969 | 0.98x |
| flat.json | msgspec | 7.885 | 7.959 | 8.352 | 353.969 | 0.92x |
| flat.json | ujson | 11.643 | 11.832 | 12.081 | 353.969 | 0.62x |
| flat.json | json | 15.321 | 15.492 | 15.805 | 353.969 | 0.47x |
| nested.json | strata | 6.545 | 6.730 | 6.786 | 357.156 | 1.00x |
| nested.json | orjson | 6.293 | 6.536 | 6.622 | 357.156 | 1.03x |
| nested.json | msgspec | 6.407 | 6.560 | 6.664 | 357.156 | 1.03x |
| nested.json | ujson | 9.651 | 9.747 | 9.946 | 357.156 | 0.69x |
| nested.json | json | 14.405 | 14.675 | 14.947 | 357.156 | 0.46x |
| wide_arrays.json | strata | 35.980 | 36.584 | 37.659 | 642.828 | 1.00x |
| wide_arrays.json | orjson | 37.175 | 37.532 | 38.801 | 642.828 | 0.97x |
| wide_arrays.json | msgspec | 45.089 | 45.357 | 46.687 | 642.828 | 0.81x |
| wide_arrays.json | ujson | 58.295 | 59.380 | 61.101 | 642.828 | 0.62x |
| wide_arrays.json | json | 81.879 | 82.559 | 83.322 | 642.828 | 0.44x |
| mixed.json | strata | 1.486 | 1.509 | 1.563 | 778.266 | 1.00x |
| mixed.json | orjson | 1.544 | 1.583 | 1.805 | 778.266 | 0.95x |
| mixed.json | msgspec | 1.669 | 1.721 | 1.798 | 778.266 | 0.88x |
| mixed.json | ujson | 2.123 | 2.167 | 2.232 | 778.266 | 0.70x |
| mixed.json | json | 3.345 | 3.362 | 3.542 | 778.266 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 337.102 | 339.964 | 342.701 | 383.391 | 1.00x |
| users.ndjson | orjson | 676.283 | 681.786 | 687.966 | 383.391 | 0.50x |
| users.ndjson | msgspec | 672.643 | 675.830 | 684.732 | 383.391 | 0.50x |
| users.ndjson | ujson | 839.350 | 842.646 | 848.242 | 383.391 | 0.40x |
| users.ndjson | json | 1012.488 | 1019.909 | 1047.621 | 383.391 | 0.33x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 80.735 | 83.137 | 114.485 | 1212.547 | 1.00x |
| users.json | orjson | 93.649 | 99.314 | 124.478 | 1212.547 | 0.84x |
| users.json | msgspec | 120.610 | 126.121 | 149.530 | 1212.547 | 0.66x |
| users.json | ujson | 454.618 | 469.960 | 489.748 | 1212.547 | 0.18x |
| users.json | json | 637.318 | 652.959 | 675.639 | 1212.547 | 0.13x |
| flat.json | strata | 2.430 | 2.505 | 4.137 | 353.969 | 1.00x |
| flat.json | orjson | 2.584 | 2.723 | 3.235 | 353.969 | 0.92x |
| flat.json | msgspec | 3.243 | 3.396 | 3.826 | 353.969 | 0.74x |
| flat.json | ujson | 9.309 | 9.576 | 10.198 | 353.969 | 0.26x |
| flat.json | json | 13.527 | 14.062 | 14.591 | 353.969 | 0.18x |
| nested.json | strata | 1.755 | 1.814 | 1.881 | 357.156 | 1.00x |
| nested.json | orjson | 2.358 | 2.500 | 2.551 | 357.156 | 0.73x |
| nested.json | msgspec | 2.930 | 3.067 | 3.208 | 357.156 | 0.59x |
| nested.json | ujson | 9.911 | 10.161 | 10.590 | 357.156 | 0.18x |
| nested.json | json | 15.920 | 16.196 | 16.753 | 357.156 | 0.11x |
| wide_arrays.json | strata | 13.609 | 13.678 | 13.735 | 777.109 | 1.00x |
| wide_arrays.json | orjson | 13.725 | 13.840 | 14.226 | 777.109 | 0.99x |
| wide_arrays.json | msgspec | 22.363 | 22.740 | 23.183 | 777.109 | 0.60x |
| wide_arrays.json | ujson | 65.288 | 66.224 | 67.468 | 777.109 | 0.21x |
| wide_arrays.json | json | 114.530 | 116.205 | 117.569 | 777.109 | 0.12x |
| mixed.json | strata | 0.532 | 0.551 | 0.585 | 778.266 | 1.00x |
| mixed.json | orjson | 0.537 | 0.570 | 0.619 | 778.266 | 0.97x |
| mixed.json | msgspec | 0.581 | 0.603 | 0.629 | 778.266 | 0.91x |
| mixed.json | ujson | 2.283 | 2.340 | 2.363 | 778.266 | 0.24x |
| mixed.json | json | 3.512 | 3.546 | 3.597 | 778.266 | 0.16x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.728 | 0.751 | 0.876 | 634.141 | 1.00x |
| users.json $[*].id | jmespath | 1.906 | 1.942 | 8.564 | 634.141 | 0.39x |
| users.json $[*].id | jsonpath-ng | 7.118 | 7.190 | 7.382 | 634.141 | 0.10x |
| users.json $[*].orders[*].total | strata | 13.737 | 14.003 | 14.260 | 936.172 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 31.289 | 31.792 | 33.358 | 936.172 | 0.44x |
| users.json $[*].orders[*].total | jsonpath-ng | 161.494 | 165.941 | 180.957 | 936.172 | 0.08x |
| users.json $..total | strata | 55.052 | 55.371 | 56.696 | 557.016 | 1.00x |
| users.json $..total | jsonpath-ng | 7773.314 | 7833.523 | 7880.851 | 557.016 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 137.845 | 140.847 | 144.005 | 940.547 | 1.00x |
| users.json $[*].id | orjson+jmespath | 384.063 | 391.687 | 420.982 | 940.547 | 0.36x |
| users.json $[*].id | orjson+jsonpath-ng | 475.493 | 478.839 | 488.802 | 940.547 | 0.29x |
| users.json $[*].orders[*].total | strata | 139.774 | 140.465 | 142.412 | 964.734 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 418.490 | 421.825 | 425.716 | 964.734 | 0.33x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 1168.072 | 1176.987 | 1210.396 | 964.734 | 0.12x |
| users.json $..total | strata | 398.163 | 402.797 | 404.929 | 862.000 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 8781.955 | 8807.913 | 8909.275 | 862.000 | 0.05x |

