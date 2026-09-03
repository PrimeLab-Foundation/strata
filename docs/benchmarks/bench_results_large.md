# Benchmark results - large

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 68d6e74
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
| users.json | strata | 325.245 | 336.220 | 343.974 | 493.328 | 1.00x |
| users.json | orjson | 386.129 | 391.159 | 394.589 | 493.328 | 0.86x |
| users.json | msgspec | 399.173 | 404.151 | 407.059 | 493.328 | 0.83x |
| users.json | ujson | 526.448 | 545.417 | 555.988 | 493.328 | 0.62x |
| users.json | json | 797.724 | 812.974 | 834.964 | 493.328 | 0.41x |
| flat.json | strata | 6.456 | 6.476 | 6.585 | 351.203 | 1.00x |
| flat.json | orjson | 7.015 | 7.052 | 7.154 | 351.203 | 0.92x |
| flat.json | msgspec | 7.098 | 7.159 | 7.444 | 351.203 | 0.90x |
| flat.json | ujson | 10.589 | 10.713 | 11.313 | 351.203 | 0.60x |
| flat.json | json | 14.594 | 14.618 | 14.741 | 351.203 | 0.44x |
| nested.json | strata | 5.625 | 5.647 | 5.772 | 353.406 | 1.00x |
| nested.json | orjson | 6.006 | 6.044 | 6.132 | 353.406 | 0.93x |
| nested.json | msgspec | 6.018 | 6.047 | 6.087 | 353.406 | 0.93x |
| nested.json | ujson | 9.012 | 9.067 | 9.317 | 353.406 | 0.62x |
| nested.json | json | 13.820 | 13.858 | 14.911 | 353.406 | 0.41x |
| wide_arrays.json | strata | 31.793 | 31.994 | 33.241 | 422.938 | 1.00x |
| wide_arrays.json | orjson | 35.178 | 35.368 | 36.828 | 422.938 | 0.90x |
| wide_arrays.json | msgspec | 42.570 | 42.900 | 43.913 | 422.938 | 0.75x |
| wide_arrays.json | ujson | 54.366 | 54.705 | 56.298 | 422.938 | 0.58x |
| wide_arrays.json | json | 78.993 | 79.260 | 80.021 | 422.938 | 0.40x |
| mixed.json | strata | 1.318 | 1.334 | 1.361 | 775.016 | 1.00x |
| mixed.json | orjson | 1.462 | 1.473 | 1.577 | 775.016 | 0.91x |
| mixed.json | msgspec | 1.587 | 1.616 | 1.647 | 775.016 | 0.83x |
| mixed.json | ujson | 1.977 | 2.012 | 2.234 | 775.016 | 0.66x |
| mixed.json | json | 3.254 | 3.288 | 3.333 | 775.016 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 71.207 | 72.517 | 74.343 | 2835.891 | 1.00x |
| users.json | orjson | 83.874 | 85.983 | 88.714 | 2835.891 | 0.84x |
| users.json | msgspec | 110.504 | 113.449 | 115.431 | 2835.891 | 0.64x |
| users.json | ujson | 444.408 | 453.383 | 457.761 | 2835.891 | 0.16x |
| users.json | json | 623.084 | 624.604 | 633.893 | 2835.891 | 0.12x |
| flat.json | strata | 2.021 | 2.027 | 2.050 | 353.109 | 1.00x |
| flat.json | orjson | 2.150 | 2.161 | 2.179 | 353.109 | 0.94x |
| flat.json | msgspec | 2.835 | 2.842 | 2.911 | 353.109 | 0.71x |
| flat.json | ujson | 8.806 | 8.829 | 9.247 | 353.109 | 0.23x |
| flat.json | json | 13.136 | 13.257 | 14.256 | 353.109 | 0.15x |
| nested.json | strata | 1.401 | 1.409 | 1.417 | 353.484 | 1.00x |
| nested.json | orjson | 2.023 | 2.026 | 2.034 | 353.484 | 0.70x |
| nested.json | msgspec | 2.604 | 2.621 | 2.642 | 353.484 | 0.54x |
| nested.json | ujson | 9.512 | 9.579 | 9.610 | 353.484 | 0.15x |
| nested.json | json | 15.203 | 15.388 | 15.536 | 353.484 | 0.09x |
| wide_arrays.json | strata | 11.318 | 11.527 | 11.796 | 627.594 | 1.00x |
| wide_arrays.json | orjson | 11.543 | 11.598 | 11.899 | 627.594 | 0.99x |
| wide_arrays.json | msgspec | 20.168 | 20.304 | 21.124 | 627.594 | 0.57x |
| wide_arrays.json | ujson | 62.294 | 62.632 | 63.467 | 627.594 | 0.18x |
| wide_arrays.json | json | 111.599 | 112.591 | 114.728 | 627.594 | 0.10x |
| mixed.json | strata | 0.363 | 0.366 | 0.377 | 775.078 | 1.00x |
| mixed.json | orjson | 0.376 | 0.378 | 0.412 | 775.078 | 0.97x |
| mixed.json | msgspec | 0.413 | 0.415 | 0.425 | 775.078 | 0.88x |
| mixed.json | ujson | 2.109 | 2.115 | 2.361 | 775.078 | 0.17x |
| mixed.json | json | 3.282 | 3.318 | 3.664 | 775.078 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 333.206 | 350.902 | 369.957 | 902.672 | 1.00x |
| users.json | orjson | 400.542 | 411.597 | 451.026 | 902.672 | 0.85x |
| users.json | msgspec | 400.446 | 407.562 | 449.465 | 902.672 | 0.86x |
| users.json | ujson | 549.931 | 563.125 | 577.459 | 902.672 | 0.62x |
| users.json | json | 806.208 | 818.157 | 847.957 | 902.672 | 0.43x |
| flat.json | strata | 6.783 | 6.844 | 6.889 | 353.375 | 1.00x |
| flat.json | orjson | 7.294 | 7.350 | 7.500 | 353.375 | 0.93x |
| flat.json | msgspec | 7.527 | 7.614 | 7.725 | 353.375 | 0.90x |
| flat.json | ujson | 11.205 | 11.238 | 11.425 | 353.375 | 0.61x |
| flat.json | json | 14.849 | 14.968 | 15.062 | 353.375 | 0.46x |
| nested.json | strata | 5.999 | 6.048 | 6.109 | 354.922 | 1.00x |
| nested.json | orjson | 6.268 | 6.342 | 6.623 | 354.922 | 0.95x |
| nested.json | msgspec | 6.321 | 6.403 | 6.466 | 354.922 | 0.94x |
| nested.json | ujson | 9.331 | 9.425 | 9.683 | 354.922 | 0.64x |
| nested.json | json | 13.984 | 14.119 | 14.559 | 354.922 | 0.43x |
| wide_arrays.json | strata | 32.958 | 33.104 | 36.455 | 641.594 | 1.00x |
| wide_arrays.json | orjson | 36.366 | 37.000 | 37.302 | 641.594 | 0.89x |
| wide_arrays.json | msgspec | 44.129 | 44.505 | 45.228 | 641.594 | 0.74x |
| wide_arrays.json | ujson | 57.663 | 58.192 | 58.606 | 641.594 | 0.57x |
| wide_arrays.json | json | 80.492 | 81.291 | 83.729 | 641.594 | 0.41x |
| mixed.json | strata | 1.414 | 1.424 | 1.435 | 775.078 | 1.00x |
| mixed.json | orjson | 1.539 | 1.549 | 1.554 | 775.078 | 0.92x |
| mixed.json | msgspec | 1.662 | 1.677 | 1.776 | 775.078 | 0.85x |
| mixed.json | ujson | 2.085 | 2.094 | 2.265 | 775.078 | 0.68x |
| mixed.json | json | 3.297 | 3.311 | 3.450 | 775.078 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 324.545 | 337.610 | 345.704 | 381.547 | 1.00x |
| users.ndjson | orjson | 666.449 | 678.233 | 689.484 | 381.547 | 0.50x |
| users.ndjson | msgspec | 674.824 | 684.115 | 695.025 | 381.547 | 0.49x |
| users.ndjson | ujson | 848.910 | 856.982 | 864.412 | 381.547 | 0.39x |
| users.ndjson | json | 1041.347 | 1048.008 | 1061.987 | 381.547 | 0.32x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 81.607 | 85.159 | 105.225 | 1253.781 | 1.00x |
| users.json | orjson | 91.859 | 95.450 | 126.048 | 1253.781 | 0.89x |
| users.json | msgspec | 121.010 | 123.760 | 142.438 | 1253.781 | 0.69x |
| users.json | ujson | 454.159 | 459.160 | 473.838 | 1253.781 | 0.19x |
| users.json | json | 616.586 | 632.940 | 648.282 | 1253.781 | 0.13x |
| flat.json | strata | 2.384 | 2.425 | 2.502 | 353.391 | 1.00x |
| flat.json | orjson | 2.537 | 2.572 | 2.679 | 353.391 | 0.94x |
| flat.json | msgspec | 3.254 | 3.297 | 3.357 | 353.391 | 0.74x |
| flat.json | ujson | 9.267 | 9.333 | 9.443 | 353.391 | 0.26x |
| flat.json | json | 13.514 | 13.622 | 14.007 | 353.391 | 0.18x |
| nested.json | strata | 1.713 | 1.770 | 1.822 | 354.922 | 1.00x |
| nested.json | orjson | 2.377 | 2.418 | 2.437 | 354.922 | 0.73x |
| nested.json | msgspec | 2.960 | 3.010 | 3.062 | 354.922 | 0.59x |
| nested.json | ujson | 9.941 | 10.000 | 10.134 | 354.922 | 0.18x |
| nested.json | json | 15.657 | 15.854 | 16.167 | 354.922 | 0.11x |
| wide_arrays.json | strata | 12.978 | 13.248 | 13.968 | 773.875 | 1.00x |
| wide_arrays.json | orjson | 13.420 | 13.547 | 14.035 | 773.875 | 0.98x |
| wide_arrays.json | msgspec | 22.116 | 22.509 | 27.054 | 773.875 | 0.59x |
| wide_arrays.json | ujson | 64.842 | 65.580 | 69.208 | 773.875 | 0.20x |
| wide_arrays.json | json | 114.019 | 114.470 | 115.676 | 773.875 | 0.12x |
| mixed.json | strata | 0.533 | 0.552 | 0.619 | 775.078 | 1.00x |
| mixed.json | orjson | 0.555 | 0.575 | 0.586 | 775.078 | 0.96x |
| mixed.json | msgspec | 0.580 | 0.610 | 0.992 | 775.078 | 0.90x |
| mixed.json | ujson | 2.305 | 2.341 | 2.436 | 775.078 | 0.24x |
| mixed.json | json | 3.465 | 3.541 | 3.759 | 775.078 | 0.16x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.721 | 0.750 | 0.772 | 575.328 | 1.00x |
| users.json $[*].id | jmespath | 1.889 | 1.933 | 13.289 | 575.328 | 0.39x |
| users.json $[*].id | jsonpath-ng | 7.121 | 7.251 | 7.629 | 575.328 | 0.10x |
| users.json $[*].orders[*].total | strata | 13.852 | 13.972 | 14.236 | 602.375 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 31.868 | 32.585 | 32.818 | 602.375 | 0.43x |
| users.json $[*].orders[*].total | jsonpath-ng | 160.587 | 164.888 | 169.299 | 602.375 | 0.08x |
| users.json $..total | strata | 54.113 | 54.440 | 54.880 | 618.938 | 1.00x |
| users.json $..total | jsonpath-ng | 7623.667 | 7639.008 | 7686.338 | 618.938 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 135.498 | 139.055 | 162.366 | 881.781 | 1.00x |
| users.json $[*].id | orjson+jmespath | 386.944 | 391.972 | 402.737 | 881.781 | 0.35x |
| users.json $[*].id | orjson+jsonpath-ng | 472.314 | 477.832 | 485.751 | 881.781 | 0.29x |
| users.json $[*].orders[*].total | strata | 136.036 | 139.781 | 141.372 | 846.203 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 421.955 | 424.078 | 430.565 | 846.203 | 0.33x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 1151.427 | 1166.829 | 1187.549 | 846.203 | 0.12x |
| users.json $..total | strata | 386.109 | 389.364 | 391.402 | 858.406 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 8629.302 | 8641.670 | 8667.710 | 858.406 | 0.05x |

