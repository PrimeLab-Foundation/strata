# Benchmark results - large

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c36c406
- python: 3.14.3
- implementation: CPython
- platform: macOS-26.3-arm64-arm-64bit-Mach-O
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
| users.json | strata | 316.178 | 320.247 | 335.126 | 280.094 | 1.00x |
| users.json | orjson | 375.356 | 380.024 | 387.864 | 280.094 | 0.84x |
| users.json | msgspec | 384.501 | 388.474 | 402.909 | 280.094 | 0.82x |
| users.json | ujson | 526.539 | 530.561 | 537.728 | 280.094 | 0.60x |
| users.json | json | 655.059 | 660.139 | 664.185 | 280.094 | 0.49x |
| flat.json | strata | 6.458 | 6.532 | 6.610 | 348.609 | 1.00x |
| flat.json | orjson | 8.063 | 8.158 | 8.380 | 348.609 | 0.80x |
| flat.json | msgspec | 7.888 | 7.966 | 8.039 | 348.609 | 0.82x |
| flat.json | ujson | 11.464 | 11.520 | 11.806 | 348.609 | 0.57x |
| flat.json | json | 15.155 | 15.376 | 15.593 | 348.609 | 0.42x |
| nested.json | strata | 5.258 | 5.345 | 5.404 | 350.844 | 1.00x |
| nested.json | orjson | 6.782 | 6.862 | 7.103 | 350.844 | 0.78x |
| nested.json | msgspec | 6.773 | 6.905 | 7.026 | 350.844 | 0.77x |
| nested.json | ujson | 9.555 | 9.709 | 10.041 | 350.844 | 0.55x |
| nested.json | json | 13.940 | 14.155 | 14.283 | 350.844 | 0.38x |
| wide_arrays.json | strata | 33.006 | 33.286 | 33.807 | 420.328 | 1.00x |
| wide_arrays.json | orjson | 32.452 | 32.582 | 33.003 | 420.328 | 1.02x |
| wide_arrays.json | msgspec | 39.850 | 40.465 | 41.010 | 420.328 | 0.82x |
| wide_arrays.json | ujson | 51.723 | 52.357 | 53.037 | 420.328 | 0.64x |
| wide_arrays.json | json | 74.808 | 75.324 | 76.894 | 420.328 | 0.44x |
| mixed.json | strata | 1.257 | 1.264 | 1.272 | 773.406 | 1.00x |
| mixed.json | orjson | 2.050 | 2.101 | 2.365 | 773.406 | 0.60x |
| mixed.json | msgspec | 2.155 | 2.176 | 2.547 | 773.406 | 0.58x |
| mixed.json | ujson | 2.547 | 2.589 | 2.820 | 773.406 | 0.49x |
| mixed.json | json | 3.643 | 3.728 | 4.103 | 773.406 | 0.34x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 75.365 | 75.918 | 77.712 | 1904.969 | 1.00x |
| users.json | orjson | 82.069 | 83.898 | 87.966 | 1904.969 | 0.90x |
| users.json | msgspec | 108.827 | 110.572 | 116.437 | 1904.969 | 0.69x |
| users.json | ujson | 431.697 | 435.816 | 447.121 | 1904.969 | 0.17x |
| users.json | json | 599.767 | 601.337 | 608.909 | 1904.969 | 0.13x |
| flat.json | strata | 1.771 | 1.804 | 1.840 | 350.516 | 1.00x |
| flat.json | orjson | 2.109 | 2.149 | 2.175 | 350.516 | 0.84x |
| flat.json | msgspec | 2.778 | 2.804 | 2.847 | 350.516 | 0.64x |
| flat.json | ujson | 8.479 | 8.504 | 8.690 | 350.516 | 0.21x |
| flat.json | json | 12.104 | 12.303 | 12.660 | 350.516 | 0.15x |
| nested.json | strata | 1.439 | 1.456 | 1.488 | 350.844 | 1.00x |
| nested.json | orjson | 1.986 | 2.009 | 2.057 | 350.844 | 0.73x |
| nested.json | msgspec | 2.547 | 2.562 | 2.592 | 350.844 | 0.57x |
| nested.json | ujson | 9.393 | 9.436 | 9.595 | 350.844 | 0.15x |
| nested.json | json | 14.629 | 14.882 | 15.091 | 350.844 | 0.10x |
| wide_arrays.json | strata | 12.205 | 12.374 | 12.998 | 624.984 | 1.00x |
| wide_arrays.json | orjson | 11.459 | 11.545 | 11.996 | 624.984 | 1.07x |
| wide_arrays.json | msgspec | 19.676 | 19.928 | 20.242 | 624.984 | 0.62x |
| wide_arrays.json | ujson | 62.559 | 62.926 | 63.531 | 624.984 | 0.20x |
| wide_arrays.json | json | 106.994 | 108.366 | 108.637 | 624.984 | 0.11x |
| mixed.json | strata | 0.347 | 0.356 | 0.405 | 773.422 | 1.00x |
| mixed.json | orjson | 0.367 | 0.372 | 0.392 | 773.422 | 0.96x |
| mixed.json | msgspec | 0.402 | 0.413 | 0.468 | 773.422 | 0.86x |
| mixed.json | ujson | 2.035 | 2.055 | 2.153 | 773.422 | 0.17x |
| mixed.json | json | 3.122 | 3.148 | 3.522 | 773.422 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 326.018 | 334.298 | 345.392 | 902.047 | 1.00x |
| users.json | orjson | 454.701 | 464.469 | 482.851 | 902.047 | 0.72x |
| users.json | msgspec | 458.084 | 469.590 | 491.837 | 902.047 | 0.71x |
| users.json | ujson | 605.317 | 619.327 | 623.908 | 902.047 | 0.54x |
| users.json | json | 731.951 | 738.521 | 746.049 | 902.047 | 0.45x |
| flat.json | strata | 6.950 | 6.988 | 7.042 | 350.828 | 1.00x |
| flat.json | orjson | 8.910 | 9.079 | 9.409 | 350.828 | 0.77x |
| flat.json | msgspec | 8.921 | 9.085 | 9.261 | 350.828 | 0.77x |
| flat.json | ujson | 12.738 | 12.776 | 13.211 | 350.828 | 0.55x |
| flat.json | json | 16.323 | 16.404 | 16.753 | 350.828 | 0.43x |
| nested.json | strata | 5.529 | 5.605 | 5.805 | 352.312 | 1.00x |
| nested.json | orjson | 8.412 | 8.515 | 9.147 | 352.312 | 0.66x |
| nested.json | msgspec | 8.530 | 8.597 | 8.842 | 352.312 | 0.65x |
| nested.json | ujson | 11.546 | 11.659 | 12.277 | 352.312 | 0.48x |
| nested.json | json | 15.686 | 15.835 | 16.273 | 352.312 | 0.35x |
| wide_arrays.json | strata | 34.524 | 34.944 | 35.907 | 640.984 | 1.00x |
| wide_arrays.json | orjson | 38.078 | 38.667 | 39.607 | 640.984 | 0.90x |
| wide_arrays.json | msgspec | 45.839 | 46.652 | 48.415 | 640.984 | 0.75x |
| wide_arrays.json | ujson | 59.071 | 59.950 | 60.913 | 640.984 | 0.58x |
| wide_arrays.json | json | 81.114 | 81.526 | 82.192 | 640.984 | 0.43x |
| mixed.json | strata | 1.374 | 1.389 | 1.424 | 773.734 | 1.00x |
| mixed.json | orjson | 2.365 | 2.420 | 2.820 | 773.734 | 0.57x |
| mixed.json | msgspec | 2.480 | 2.516 | 2.667 | 773.734 | 0.55x |
| mixed.json | ujson | 2.930 | 3.016 | 3.342 | 773.734 | 0.46x |
| mixed.json | json | 3.974 | 4.037 | 4.349 | 773.734 | 0.34x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 312.410 | 316.662 | 318.094 | 374.953 | 1.00x |
| users.ndjson | orjson | 510.163 | 511.715 | 516.609 | 374.953 | 0.62x |
| users.ndjson | msgspec | 512.553 | 514.719 | 517.884 | 374.953 | 0.62x |
| users.ndjson | ujson | 653.791 | 660.861 | 663.974 | 374.953 | 0.48x |
| users.ndjson | json | 787.945 | 793.307 | 813.570 | 374.953 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 85.086 | 86.119 | 88.694 | 1360.266 | 1.00x |
| users.json | orjson | 92.041 | 93.887 | 96.475 | 1360.266 | 0.92x |
| users.json | msgspec | 119.382 | 123.051 | 124.379 | 1360.266 | 0.70x |
| users.json | ujson | 444.002 | 458.852 | 464.664 | 1360.266 | 0.19x |
| users.json | json | 608.901 | 617.263 | 631.167 | 1360.266 | 0.14x |
| flat.json | strata | 2.135 | 2.190 | 2.356 | 350.828 | 1.00x |
| flat.json | orjson | 2.492 | 2.574 | 2.607 | 350.828 | 0.85x |
| flat.json | msgspec | 3.236 | 3.261 | 3.432 | 350.828 | 0.67x |
| flat.json | ujson | 9.314 | 9.385 | 9.428 | 350.828 | 0.23x |
| flat.json | json | 12.616 | 12.835 | 12.905 | 350.828 | 0.17x |
| nested.json | strata | 1.784 | 1.808 | 1.898 | 352.312 | 1.00x |
| nested.json | orjson | 2.326 | 2.359 | 2.401 | 352.312 | 0.77x |
| nested.json | msgspec | 2.880 | 2.912 | 2.968 | 352.312 | 0.62x |
| nested.json | ujson | 9.834 | 9.905 | 9.986 | 352.312 | 0.18x |
| nested.json | json | 15.107 | 15.304 | 15.453 | 352.312 | 0.12x |
| wide_arrays.json | strata | 13.958 | 14.104 | 14.683 | 772.266 | 1.00x |
| wide_arrays.json | orjson | 13.162 | 13.455 | 13.802 | 772.266 | 1.05x |
| wide_arrays.json | msgspec | 21.805 | 21.866 | 22.526 | 772.266 | 0.65x |
| wide_arrays.json | ujson | 64.637 | 65.031 | 65.589 | 772.266 | 0.22x |
| wide_arrays.json | json | 109.596 | 110.478 | 115.310 | 772.266 | 0.13x |
| mixed.json | strata | 0.504 | 0.539 | 0.625 | 773.734 | 1.00x |
| mixed.json | orjson | 0.531 | 0.554 | 0.596 | 773.734 | 0.97x |
| mixed.json | msgspec | 0.572 | 0.584 | 0.621 | 773.734 | 0.92x |
| mixed.json | ujson | 2.269 | 2.327 | 2.508 | 773.734 | 0.23x |
| mixed.json | json | 3.319 | 3.421 | 3.565 | 773.734 | 0.16x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.673 | 0.690 | 0.730 | 831.859 | 1.00x |
| users.json $[*].id | jmespath | 2.009 | 2.056 | 2.160 | 831.859 | 0.34x |
| users.json $[*].id | jsonpath-ng | 71.263 | 72.805 | 81.713 | 831.859 | 0.01x |
| users.json $[*].orders[*].total | strata | 12.973 | 13.112 | 14.892 | 603.375 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 34.889 | 35.670 | 39.415 | 603.375 | 0.37x |
| users.json $[*].orders[*].total | jsonpath-ng | 214.203 | 219.246 | 223.436 | 603.375 | 0.06x |
| users.json $..total | strata | 53.214 | 53.607 | 53.950 | 618.094 | 1.00x |
| users.json $..total | jsonpath-ng | 8598.769 | 8614.421 | 8624.024 | 618.094 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 133.026 | 133.479 | 139.051 | 823.516 | 1.00x |
| users.json $[*].id | orjson+jmespath | 450.051 | 452.327 | 464.050 | 823.516 | 0.30x |
| users.json $[*].id | orjson+jsonpath-ng | 453.484 | 455.823 | 463.195 | 823.516 | 0.29x |
| users.json $[*].orders[*].total | strata | 133.229 | 134.785 | 135.784 | 845.328 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 481.178 | 484.785 | 487.553 | 845.328 | 0.28x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 602.787 | 610.291 | 615.992 | 845.328 | 0.22x |
| users.json $..total | strata | 435.188 | 438.845 | 443.205 | 856.500 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 8935.566 | 8959.062 | 8982.880 | 856.500 | 0.05x |

