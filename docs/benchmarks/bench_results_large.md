# Benchmark results - large

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 3758c97
- python: 3.14.7
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit-Mach-O
- machine: arm64
- processor: Apple M1 Max
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 288.891 | 302.024 | 306.280 | 493.453 | 1.00x |
| users.json | orjson | 399.563 | 412.448 | 435.158 | 493.453 | 0.73x |
| users.json | msgspec | 388.289 | 404.816 | 427.766 | 493.453 | 0.75x |
| users.json | ujson | 552.142 | 567.720 | 584.219 | 493.453 | 0.53x |
| users.json | json | 806.916 | 815.200 | 834.050 | 493.453 | 0.37x |
| flat.json | strata | 5.570 | 5.654 | 6.102 | 352.281 | 1.00x |
| flat.json | orjson | 6.944 | 7.088 | 7.408 | 352.281 | 0.80x |
| flat.json | msgspec | 7.237 | 7.393 | 7.745 | 352.281 | 0.76x |
| flat.json | ujson | 10.727 | 10.962 | 11.229 | 352.281 | 0.52x |
| flat.json | json | 14.411 | 14.817 | 15.089 | 352.281 | 0.38x |
| nested.json | strata | 4.896 | 4.950 | 5.615 | 354.453 | 1.00x |
| nested.json | orjson | 6.125 | 6.161 | 6.937 | 354.453 | 0.80x |
| nested.json | msgspec | 6.083 | 6.160 | 6.917 | 354.453 | 0.80x |
| nested.json | ujson | 8.961 | 9.085 | 10.412 | 354.453 | 0.54x |
| nested.json | json | 14.179 | 14.333 | 15.904 | 354.453 | 0.35x |
| wide_arrays.json | strata | 30.051 | 30.154 | 30.521 | 423.938 | 1.00x |
| wide_arrays.json | orjson | 35.391 | 35.643 | 36.507 | 423.938 | 0.85x |
| wide_arrays.json | msgspec | 42.713 | 43.921 | 46.069 | 423.938 | 0.69x |
| wide_arrays.json | ujson | 55.115 | 55.592 | 59.104 | 423.938 | 0.54x |
| wide_arrays.json | json | 79.305 | 80.427 | 81.609 | 423.938 | 0.37x |
| mixed.json | strata | 1.146 | 1.161 | 1.200 | 805.812 | 1.00x |
| mixed.json | orjson | 1.479 | 1.490 | 1.584 | 805.812 | 0.78x |
| mixed.json | msgspec | 1.583 | 1.592 | 1.759 | 805.812 | 0.73x |
| mixed.json | ujson | 1.963 | 1.976 | 2.004 | 805.812 | 0.59x |
| mixed.json | json | 3.262 | 3.283 | 3.435 | 805.812 | 0.35x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 62.350 | 64.092 | 65.697 | 3565.141 | 1.00x |
| users.json | orjson | 86.884 | 87.634 | 89.246 | 3565.141 | 0.73x |
| users.json | msgspec | 112.728 | 115.682 | 117.336 | 3565.141 | 0.55x |
| users.json | ujson | 439.955 | 445.807 | 449.271 | 3565.141 | 0.14x |
| users.json | json | 631.377 | 636.021 | 643.201 | 3565.141 | 0.10x |
| flat.json | strata | 2.011 | 2.025 | 2.038 | 354.156 | 1.00x |
| flat.json | orjson | 2.135 | 2.147 | 2.187 | 354.156 | 0.94x |
| flat.json | msgspec | 2.821 | 2.851 | 2.893 | 354.156 | 0.71x |
| flat.json | ujson | 8.785 | 8.832 | 8.886 | 354.156 | 0.23x |
| flat.json | json | 13.095 | 13.301 | 13.370 | 354.156 | 0.15x |
| nested.json | strata | 1.238 | 1.247 | 1.294 | 354.484 | 1.00x |
| nested.json | orjson | 1.987 | 2.004 | 2.049 | 354.484 | 0.62x |
| nested.json | msgspec | 2.595 | 2.616 | 2.659 | 354.484 | 0.48x |
| nested.json | ujson | 9.585 | 9.638 | 9.681 | 354.484 | 0.13x |
| nested.json | json | 15.284 | 15.341 | 15.454 | 354.484 | 0.08x |
| wide_arrays.json | strata | 10.349 | 10.493 | 10.823 | 590.031 | 1.00x |
| wide_arrays.json | orjson | 12.049 | 12.432 | 12.696 | 590.031 | 0.84x |
| wide_arrays.json | msgspec | 20.240 | 20.362 | 21.469 | 590.031 | 0.52x |
| wide_arrays.json | ujson | 62.592 | 63.030 | 65.082 | 590.031 | 0.17x |
| wide_arrays.json | json | 114.174 | 115.391 | 116.902 | 590.031 | 0.09x |
| mixed.json | strata | 0.308 | 0.310 | 0.313 | 805.859 | 1.00x |
| mixed.json | orjson | 0.378 | 0.386 | 0.413 | 805.859 | 0.80x |
| mixed.json | msgspec | 0.416 | 0.419 | 0.448 | 805.859 | 0.74x |
| mixed.json | ujson | 2.124 | 2.138 | 2.173 | 805.859 | 0.15x |
| mixed.json | json | 3.331 | 3.360 | 3.396 | 805.859 | 0.09x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 304.425 | 321.920 | 339.770 | 902.125 | 1.00x |
| users.json | orjson | 404.637 | 422.363 | 441.925 | 902.125 | 0.76x |
| users.json | msgspec | 395.258 | 422.199 | 433.056 | 902.125 | 0.76x |
| users.json | ujson | 567.574 | 589.462 | 599.114 | 902.125 | 0.55x |
| users.json | json | 826.948 | 838.962 | 856.734 | 902.125 | 0.38x |
| flat.json | strata | 5.942 | 6.030 | 7.603 | 354.406 | 1.00x |
| flat.json | orjson | 7.280 | 7.388 | 7.554 | 354.406 | 0.82x |
| flat.json | msgspec | 7.703 | 7.838 | 8.088 | 354.406 | 0.77x |
| flat.json | ujson | 11.315 | 11.462 | 11.724 | 354.406 | 0.53x |
| flat.json | json | 14.837 | 14.986 | 15.200 | 354.406 | 0.40x |
| nested.json | strata | 5.089 | 5.144 | 5.156 | 355.922 | 1.00x |
| nested.json | orjson | 6.226 | 6.309 | 6.379 | 355.922 | 0.82x |
| nested.json | msgspec | 6.281 | 6.300 | 6.478 | 355.922 | 0.82x |
| nested.json | ujson | 9.254 | 9.325 | 9.454 | 355.922 | 0.55x |
| nested.json | json | 14.249 | 14.356 | 14.439 | 355.922 | 0.36x |
| wide_arrays.json | strata | 32.222 | 32.686 | 33.434 | 603.031 | 1.00x |
| wide_arrays.json | orjson | 37.303 | 37.794 | 38.921 | 603.031 | 0.86x |
| wide_arrays.json | msgspec | 45.596 | 46.457 | 47.041 | 603.031 | 0.70x |
| wide_arrays.json | ujson | 58.827 | 61.021 | 62.104 | 603.031 | 0.54x |
| wide_arrays.json | json | 82.191 | 82.892 | 84.511 | 603.031 | 0.39x |
| mixed.json | strata | 1.236 | 1.249 | 1.303 | 805.859 | 1.00x |
| mixed.json | orjson | 1.527 | 1.551 | 1.586 | 805.859 | 0.81x |
| mixed.json | msgspec | 1.661 | 1.693 | 1.702 | 805.859 | 0.74x |
| mixed.json | ujson | 2.096 | 2.119 | 2.131 | 805.859 | 0.59x |
| mixed.json | json | 3.308 | 3.355 | 3.389 | 805.859 | 0.37x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 293.221 | 305.515 | 309.501 | 382.641 | 1.00x |
| users.ndjson | orjson | 697.587 | 713.026 | 722.050 | 382.641 | 0.43x |
| users.ndjson | msgspec | 700.886 | 717.102 | 745.608 | 382.641 | 0.43x |
| users.ndjson | ujson | 879.998 | 890.414 | 923.770 | 382.641 | 0.34x |
| users.ndjson | json | 1051.035 | 1055.751 | 1067.169 | 382.641 | 0.29x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 69.140 | 75.896 | 107.671 | 1573.094 | 1.00x |
| users.json | orjson | 96.888 | 102.621 | 125.655 | 1573.094 | 0.74x |
| users.json | msgspec | 123.761 | 125.840 | 154.757 | 1573.094 | 0.60x |
| users.json | ujson | 449.759 | 452.823 | 487.969 | 1573.094 | 0.17x |
| users.json | json | 645.800 | 652.210 | 692.371 | 1573.094 | 0.12x |
| flat.json | strata | 2.423 | 2.464 | 2.904 | 354.406 | 1.00x |
| flat.json | orjson | 2.550 | 2.636 | 3.704 | 354.406 | 0.93x |
| flat.json | msgspec | 3.265 | 3.295 | 3.759 | 354.406 | 0.75x |
| flat.json | ujson | 9.280 | 9.418 | 9.697 | 354.406 | 0.26x |
| flat.json | json | 13.563 | 13.761 | 14.305 | 354.406 | 0.18x |
| nested.json | strata | 1.622 | 1.667 | 1.739 | 355.922 | 1.00x |
| nested.json | orjson | 2.399 | 2.443 | 2.732 | 355.922 | 0.68x |
| nested.json | msgspec | 2.983 | 3.034 | 3.260 | 355.922 | 0.55x |
| nested.json | ujson | 10.046 | 10.096 | 10.258 | 355.922 | 0.17x |
| nested.json | json | 15.774 | 15.844 | 16.023 | 355.922 | 0.11x |
| wide_arrays.json | strata | 12.346 | 12.897 | 18.997 | 809.766 | 1.00x |
| wide_arrays.json | orjson | 14.058 | 14.573 | 19.355 | 809.766 | 0.88x |
| wide_arrays.json | msgspec | 22.217 | 23.187 | 27.399 | 809.766 | 0.56x |
| wide_arrays.json | ujson | 64.816 | 65.869 | 69.133 | 809.766 | 0.20x |
| wide_arrays.json | json | 115.019 | 117.407 | 121.452 | 809.766 | 0.11x |
| mixed.json | strata | 0.506 | 0.524 | 0.615 | 805.859 | 1.00x |
| mixed.json | orjson | 0.572 | 0.591 | 0.649 | 805.859 | 0.89x |
| mixed.json | msgspec | 0.603 | 0.620 | 0.697 | 805.859 | 0.84x |
| mixed.json | ujson | 2.315 | 2.335 | 2.392 | 805.859 | 0.22x |
| mixed.json | json | 3.465 | 3.497 | 3.525 | 805.859 | 0.15x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.749 | 0.771 | 0.835 | 973.734 | 1.00x |
| users.json $[*].id | jmespath | 1.892 | 1.936 | 2.391 | 973.734 | 0.40x |
| users.json $[*].id | jsonpath-ng | 7.176 | 7.631 | 32.826 | 973.734 | 0.10x |
| users.json $[*].orders[*].total | strata | 14.233 | 14.726 | 16.528 | 538.531 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 33.002 | 35.392 | 75.521 | 538.531 | 0.42x |
| users.json $[*].orders[*].total | jsonpath-ng | 163.708 | 166.138 | 171.820 | 538.531 | 0.09x |
| users.json $..total | strata | 55.229 | 55.755 | 58.076 | 621.484 | 1.00x |
| users.json $..total | jsonpath-ng | 7807.707 | 7813.960 | 7853.133 | 621.484 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 136.412 | 137.706 | 140.224 | 1162.391 | 1.00x |
| users.json $[*].id | orjson+jmespath | 397.448 | 406.168 | 410.954 | 1162.391 | 0.34x |
| users.json $[*].id | orjson+jsonpath-ng | 492.273 | 503.252 | 511.254 | 1162.391 | 0.27x |
| users.json $[*].orders[*].total | strata | 136.150 | 138.155 | 140.817 | 846.953 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 436.279 | 443.361 | 463.771 | 846.953 | 0.31x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 1211.448 | 1224.112 | 1245.855 | 846.953 | 0.11x |
| users.json $..total | strata | 343.388 | 354.103 | 358.777 | 860.969 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 8825.571 | 8838.853 | 8923.516 | 860.969 | 0.04x |

