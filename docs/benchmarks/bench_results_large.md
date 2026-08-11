# Benchmark results - large

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 67d6d68
- python: 3.14.3
- implementation: CPython
- platform: macOS-26.3-arm64-arm-64bit-Mach-O
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
| users.json | strata | 332.137 | 336.121 | 345.419 | 279.750 | 1.00x |
| users.json | orjson | 379.328 | 386.586 | 393.453 | 279.750 | 0.87x |
| users.json | msgspec | 392.487 | 396.393 | 402.849 | 279.750 | 0.85x |
| users.json | ujson | 528.692 | 533.378 | 540.666 | 279.750 | 0.63x |
| users.json | json | 647.141 | 651.938 | 655.866 | 279.750 | 0.52x |
| flat.json | strata | 6.308 | 6.394 | 6.494 | 353.938 | 1.00x |
| flat.json | orjson | 7.735 | 7.802 | 8.066 | 353.938 | 0.82x |
| flat.json | msgspec | 7.868 | 8.064 | 8.125 | 353.938 | 0.79x |
| flat.json | ujson | 11.447 | 11.620 | 11.813 | 353.938 | 0.55x |
| flat.json | json | 14.809 | 15.100 | 15.252 | 353.938 | 0.42x |
| nested.json | strata | 5.524 | 5.631 | 6.004 | 355.859 | 1.00x |
| nested.json | orjson | 6.719 | 6.775 | 7.023 | 355.859 | 0.83x |
| nested.json | msgspec | 6.712 | 6.792 | 7.391 | 355.859 | 0.83x |
| nested.json | ujson | 9.489 | 9.582 | 9.810 | 355.859 | 0.59x |
| nested.json | json | 13.805 | 14.147 | 14.653 | 355.859 | 0.40x |
| wide_arrays.json | strata | 32.529 | 32.864 | 33.519 | 427.078 | 1.00x |
| wide_arrays.json | orjson | 32.381 | 32.702 | 33.383 | 427.078 | 1.00x |
| wide_arrays.json | msgspec | 39.986 | 40.493 | 41.568 | 427.078 | 0.81x |
| wide_arrays.json | ujson | 51.942 | 52.279 | 52.927 | 427.078 | 0.63x |
| wide_arrays.json | json | 75.237 | 75.966 | 76.876 | 427.078 | 0.43x |
| mixed.json | strata | 1.306 | 1.313 | 1.326 | 780.156 | 1.00x |
| mixed.json | orjson | 2.052 | 2.062 | 2.223 | 780.156 | 0.64x |
| mixed.json | msgspec | 2.157 | 2.180 | 2.557 | 780.156 | 0.60x |
| mixed.json | ujson | 2.555 | 2.584 | 2.866 | 780.156 | 0.51x |
| mixed.json | json | 3.675 | 3.711 | 4.260 | 780.156 | 0.35x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 83.333 | 84.455 | 85.371 | 1956.281 | 1.00x |
| users.json | orjson | 81.922 | 83.519 | 85.647 | 1956.281 | 1.01x |
| users.json | msgspec | 108.500 | 110.836 | 116.978 | 1956.281 | 0.76x |
| users.json | ujson | 428.770 | 432.935 | 442.954 | 1956.281 | 0.20x |
| users.json | json | 595.333 | 599.656 | 606.567 | 1956.281 | 0.14x |
| flat.json | strata | 1.999 | 2.015 | 2.052 | 355.828 | 1.00x |
| flat.json | orjson | 2.110 | 2.118 | 2.196 | 355.828 | 0.95x |
| flat.json | msgspec | 2.787 | 2.808 | 2.852 | 355.828 | 0.72x |
| flat.json | ujson | 8.807 | 8.832 | 9.001 | 355.828 | 0.23x |
| flat.json | json | 12.069 | 12.229 | 12.400 | 355.828 | 0.16x |
| nested.json | strata | 1.527 | 1.536 | 1.634 | 355.859 | 1.00x |
| nested.json | orjson | 1.969 | 1.980 | 1.999 | 355.859 | 0.78x |
| nested.json | msgspec | 2.556 | 2.576 | 2.599 | 355.859 | 0.60x |
| nested.json | ujson | 9.479 | 9.520 | 9.612 | 355.859 | 0.16x |
| nested.json | json | 14.659 | 14.798 | 14.918 | 355.859 | 0.10x |
| wide_arrays.json | strata | 12.157 | 12.381 | 12.896 | 631.734 | 1.00x |
| wide_arrays.json | orjson | 11.484 | 11.533 | 12.676 | 631.734 | 1.07x |
| wide_arrays.json | msgspec | 19.746 | 20.036 | 20.273 | 631.734 | 0.62x |
| wide_arrays.json | ujson | 61.645 | 62.618 | 63.170 | 631.734 | 0.20x |
| wide_arrays.json | json | 107.930 | 108.542 | 109.592 | 631.734 | 0.11x |
| mixed.json | strata | 0.381 | 0.387 | 0.399 | 780.172 | 1.00x |
| mixed.json | orjson | 0.375 | 0.380 | 0.404 | 780.172 | 1.02x |
| mixed.json | msgspec | 0.419 | 0.423 | 0.486 | 780.172 | 0.92x |
| mixed.json | ujson | 2.061 | 2.072 | 2.099 | 780.172 | 0.19x |
| mixed.json | json | 3.174 | 3.227 | 3.503 | 780.172 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 335.680 | 349.023 | 355.024 | 902.453 | 1.00x |
| users.json | orjson | 448.715 | 462.574 | 484.499 | 902.453 | 0.75x |
| users.json | msgspec | 457.478 | 474.052 | 478.543 | 902.453 | 0.74x |
| users.json | ujson | 606.871 | 620.029 | 627.452 | 902.453 | 0.56x |
| users.json | json | 716.077 | 728.690 | 735.040 | 902.453 | 0.48x |
| flat.json | strata | 6.789 | 6.828 | 6.910 | 355.828 | 1.00x |
| flat.json | orjson | 8.585 | 8.625 | 8.761 | 355.828 | 0.79x |
| flat.json | msgspec | 8.926 | 9.041 | 9.502 | 355.828 | 0.76x |
| flat.json | ujson | 12.570 | 12.712 | 13.157 | 355.828 | 0.54x |
| flat.json | json | 15.807 | 15.938 | 16.376 | 355.828 | 0.43x |
| nested.json | strata | 5.820 | 5.918 | 6.026 | 359.062 | 1.00x |
| nested.json | orjson | 8.342 | 8.400 | 8.641 | 359.062 | 0.70x |
| nested.json | msgspec | 8.432 | 8.507 | 8.803 | 359.062 | 0.70x |
| nested.json | ujson | 11.339 | 11.473 | 11.750 | 359.062 | 0.52x |
| nested.json | json | 15.446 | 15.583 | 16.071 | 359.062 | 0.38x |
| wide_arrays.json | strata | 34.018 | 34.247 | 34.904 | 642.734 | 1.00x |
| wide_arrays.json | orjson | 38.082 | 38.368 | 39.288 | 642.734 | 0.89x |
| wide_arrays.json | msgspec | 46.022 | 46.385 | 47.516 | 642.734 | 0.74x |
| wide_arrays.json | ujson | 59.245 | 59.591 | 60.782 | 642.734 | 0.57x |
| wide_arrays.json | json | 81.120 | 81.823 | 83.031 | 642.734 | 0.42x |
| mixed.json | strata | 1.408 | 1.417 | 1.429 | 780.172 | 1.00x |
| mixed.json | orjson | 2.335 | 2.364 | 2.485 | 780.172 | 0.60x |
| mixed.json | msgspec | 2.470 | 2.487 | 2.674 | 780.172 | 0.57x |
| mixed.json | ujson | 2.893 | 2.926 | 3.313 | 780.172 | 0.48x |
| mixed.json | json | 3.956 | 3.981 | 4.338 | 780.172 | 0.36x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 322.898 | 323.991 | 327.349 | 380.266 | 1.00x |
| users.ndjson | orjson | 508.756 | 510.983 | 514.867 | 380.266 | 0.63x |
| users.ndjson | msgspec | 512.779 | 515.111 | 518.744 | 380.266 | 0.63x |
| users.ndjson | ujson | 661.409 | 666.499 | 670.760 | 380.266 | 0.49x |
| users.ndjson | json | 772.865 | 776.208 | 782.819 | 380.266 | 0.42x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 93.879 | 95.363 | 96.342 | 1633.641 | 1.00x |
| users.json | orjson | 92.706 | 94.281 | 97.628 | 1633.641 | 1.01x |
| users.json | msgspec | 118.412 | 120.194 | 126.505 | 1633.641 | 0.79x |
| users.json | ujson | 436.789 | 439.593 | 451.945 | 1633.641 | 0.22x |
| users.json | json | 603.035 | 617.677 | 632.308 | 1633.641 | 0.15x |
| flat.json | strata | 2.385 | 2.437 | 2.837 | 355.844 | 1.00x |
| flat.json | orjson | 2.496 | 2.565 | 2.835 | 355.844 | 0.95x |
| flat.json | msgspec | 3.206 | 3.255 | 3.549 | 355.844 | 0.75x |
| flat.json | ujson | 9.276 | 9.342 | 9.657 | 355.844 | 0.26x |
| flat.json | json | 12.547 | 12.632 | 13.046 | 355.844 | 0.19x |
| nested.json | strata | 1.841 | 1.904 | 1.930 | 359.062 | 1.00x |
| nested.json | orjson | 2.290 | 2.329 | 2.394 | 359.062 | 0.82x |
| nested.json | msgspec | 2.884 | 2.931 | 3.042 | 359.062 | 0.65x |
| nested.json | ujson | 9.869 | 9.952 | 10.074 | 359.062 | 0.19x |
| nested.json | json | 15.097 | 15.338 | 15.457 | 359.062 | 0.12x |
| wide_arrays.json | strata | 13.839 | 14.253 | 18.911 | 779.016 | 1.00x |
| wide_arrays.json | orjson | 13.249 | 13.576 | 14.807 | 779.016 | 1.05x |
| wide_arrays.json | msgspec | 21.751 | 22.270 | 22.826 | 779.016 | 0.64x |
| wide_arrays.json | ujson | 64.715 | 65.636 | 66.280 | 779.016 | 0.22x |
| wide_arrays.json | json | 109.706 | 110.953 | 111.685 | 779.016 | 0.13x |
| mixed.json | strata | 0.539 | 0.552 | 0.598 | 780.172 | 1.00x |
| mixed.json | orjson | 0.528 | 0.544 | 0.602 | 780.172 | 1.01x |
| mixed.json | msgspec | 0.574 | 0.587 | 0.613 | 780.172 | 0.94x |
| mixed.json | ujson | 2.263 | 2.282 | 2.328 | 780.172 | 0.24x |
| mixed.json | json | 3.330 | 3.366 | 3.535 | 780.172 | 0.16x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.666 | 0.683 | 0.717 | 850.875 | 1.00x |
| users.json $[*].id | jmespath | 2.005 | 2.019 | 2.145 | 850.875 | 0.34x |
| users.json $[*].id | jsonpath-ng | 70.606 | 71.491 | 82.417 | 850.875 | 0.01x |
| users.json $[*].orders[*].total | strata | 12.959 | 13.131 | 16.847 | 604.531 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 34.541 | 35.555 | 40.251 | 604.531 | 0.37x |
| users.json $[*].orders[*].total | jsonpath-ng | 213.082 | 218.001 | 224.013 | 604.531 | 0.06x |
| users.json $..total | strata | 52.913 | 53.081 | 53.474 | 623.016 | 1.00x |
| users.json $..total | jsonpath-ng | 8622.304 | 8633.535 | 8660.453 | 623.016 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 131.832 | 132.790 | 134.055 | 825.000 | 1.00x |
| users.json $[*].id | orjson+jmespath | 450.587 | 451.928 | 462.789 | 825.000 | 0.29x |
| users.json $[*].id | orjson+jsonpath-ng | 455.476 | 457.230 | 458.320 | 825.000 | 0.29x |
| users.json $[*].orders[*].total | strata | 133.647 | 134.596 | 135.147 | 848.391 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 480.624 | 484.058 | 487.186 | 848.391 | 0.28x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 608.131 | 612.072 | 622.636 | 848.391 | 0.22x |
| users.json $..total | strata | 447.283 | 451.895 | 454.418 | 861.406 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 8937.208 | 8987.188 | 9008.794 | 861.406 | 0.05x |

