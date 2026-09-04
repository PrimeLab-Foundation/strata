# Benchmark results - large

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: d61f035
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
| users.json | strata | 326.803 | 332.997 | 341.226 | 493.547 | 1.00x |
| users.json | orjson | 396.385 | 414.056 | 422.612 | 493.547 | 0.80x |
| users.json | msgspec | 401.375 | 420.632 | 430.028 | 493.547 | 0.79x |
| users.json | ujson | 568.369 | 575.625 | 581.674 | 493.547 | 0.58x |
| users.json | json | 808.358 | 820.716 | 834.905 | 493.547 | 0.41x |
| flat.json | strata | 6.076 | 6.196 | 6.374 | 349.688 | 1.00x |
| flat.json | orjson | 7.081 | 7.178 | 7.442 | 349.688 | 0.86x |
| flat.json | msgspec | 7.090 | 7.331 | 7.513 | 349.688 | 0.85x |
| flat.json | ujson | 10.746 | 11.146 | 11.450 | 349.688 | 0.56x |
| flat.json | json | 14.877 | 15.147 | 15.424 | 349.688 | 0.41x |
| nested.json | strata | 5.452 | 5.600 | 5.833 | 351.953 | 1.00x |
| nested.json | orjson | 6.117 | 6.279 | 6.421 | 351.953 | 0.89x |
| nested.json | msgspec | 6.115 | 6.297 | 6.371 | 351.953 | 0.89x |
| nested.json | ujson | 9.243 | 9.376 | 9.549 | 351.953 | 0.60x |
| nested.json | json | 14.144 | 14.317 | 14.473 | 351.953 | 0.39x |
| wide_arrays.json | strata | 30.910 | 31.634 | 32.246 | 421.578 | 1.00x |
| wide_arrays.json | orjson | 35.113 | 35.576 | 36.889 | 421.578 | 0.89x |
| wide_arrays.json | msgspec | 42.547 | 43.810 | 45.018 | 421.578 | 0.72x |
| wide_arrays.json | ujson | 55.379 | 56.047 | 56.926 | 421.578 | 0.56x |
| wide_arrays.json | json | 79.086 | 81.047 | 83.053 | 421.578 | 0.39x |
| mixed.json | strata | 1.266 | 1.276 | 1.419 | 803.406 | 1.00x |
| mixed.json | orjson | 1.476 | 1.505 | 1.665 | 803.406 | 0.85x |
| mixed.json | msgspec | 1.605 | 1.626 | 1.797 | 803.406 | 0.79x |
| mixed.json | ujson | 1.987 | 2.035 | 2.164 | 803.406 | 0.63x |
| mixed.json | json | 3.286 | 3.397 | 3.442 | 803.406 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 61.849 | 63.662 | 64.789 | 3565.781 | 1.00x |
| users.json | orjson | 87.201 | 88.967 | 90.562 | 3565.781 | 0.72x |
| users.json | msgspec | 114.897 | 116.148 | 120.756 | 3565.781 | 0.55x |
| users.json | ujson | 452.011 | 454.598 | 455.763 | 3565.781 | 0.14x |
| users.json | json | 632.250 | 634.530 | 641.349 | 3565.781 | 0.10x |
| flat.json | strata | 1.704 | 1.707 | 1.723 | 351.656 | 1.00x |
| flat.json | orjson | 2.138 | 2.153 | 2.166 | 351.656 | 0.79x |
| flat.json | msgspec | 2.817 | 2.825 | 2.852 | 351.656 | 0.60x |
| flat.json | ujson | 8.803 | 8.842 | 8.874 | 351.656 | 0.19x |
| flat.json | json | 13.213 | 13.280 | 13.369 | 351.656 | 0.13x |
| nested.json | strata | 1.338 | 1.353 | 1.418 | 352.078 | 1.00x |
| nested.json | orjson | 1.993 | 2.018 | 2.154 | 352.078 | 0.67x |
| nested.json | msgspec | 2.593 | 2.634 | 2.777 | 352.078 | 0.51x |
| nested.json | ujson | 9.571 | 9.618 | 9.809 | 352.078 | 0.14x |
| nested.json | json | 15.166 | 15.373 | 15.690 | 352.078 | 0.09x |
| wide_arrays.json | strata | 9.899 | 10.081 | 10.247 | 587.688 | 1.00x |
| wide_arrays.json | orjson | 12.058 | 12.396 | 12.829 | 587.688 | 0.81x |
| wide_arrays.json | msgspec | 20.069 | 20.429 | 20.938 | 587.688 | 0.49x |
| wide_arrays.json | ujson | 62.270 | 62.624 | 62.951 | 587.688 | 0.16x |
| wide_arrays.json | json | 114.239 | 115.482 | 116.033 | 587.688 | 0.09x |
| mixed.json | strata | 0.320 | 0.325 | 0.346 | 803.500 | 1.00x |
| mixed.json | orjson | 0.372 | 0.375 | 0.411 | 803.500 | 0.87x |
| mixed.json | msgspec | 0.407 | 0.411 | 0.429 | 803.500 | 0.79x |
| mixed.json | ujson | 2.110 | 2.130 | 2.160 | 803.500 | 0.15x |
| mixed.json | json | 3.278 | 3.329 | 3.604 | 803.500 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 345.029 | 348.441 | 396.321 | 851.078 | 1.00x |
| users.json | orjson | 405.644 | 427.335 | 471.423 | 851.078 | 0.82x |
| users.json | msgspec | 413.790 | 426.835 | 438.010 | 851.078 | 0.82x |
| users.json | ujson | 564.308 | 580.751 | 601.684 | 851.078 | 0.60x |
| users.json | json | 819.935 | 825.839 | 853.629 | 851.078 | 0.42x |
| flat.json | strata | 6.437 | 6.477 | 6.540 | 351.922 | 1.00x |
| flat.json | orjson | 7.332 | 7.400 | 7.526 | 351.922 | 0.88x |
| flat.json | msgspec | 7.500 | 7.540 | 7.637 | 351.922 | 0.86x |
| flat.json | ujson | 11.278 | 11.417 | 11.478 | 351.922 | 0.57x |
| flat.json | json | 15.301 | 15.399 | 15.484 | 351.922 | 0.42x |
| nested.json | strata | 5.704 | 5.751 | 5.789 | 353.562 | 1.00x |
| nested.json | orjson | 6.260 | 6.317 | 6.351 | 353.562 | 0.91x |
| nested.json | msgspec | 6.258 | 6.331 | 6.372 | 353.562 | 0.91x |
| nested.json | ujson | 9.367 | 9.433 | 9.536 | 353.562 | 0.61x |
| nested.json | json | 14.052 | 14.122 | 14.255 | 353.562 | 0.41x |
| wide_arrays.json | strata | 32.448 | 33.085 | 34.202 | 601.703 | 1.00x |
| wide_arrays.json | orjson | 36.738 | 37.248 | 38.478 | 601.703 | 0.89x |
| wide_arrays.json | msgspec | 44.902 | 45.617 | 46.953 | 601.703 | 0.73x |
| wide_arrays.json | ujson | 58.059 | 59.264 | 60.935 | 601.703 | 0.56x |
| wide_arrays.json | json | 81.623 | 82.643 | 84.229 | 601.703 | 0.40x |
| mixed.json | strata | 1.367 | 1.390 | 1.465 | 803.844 | 1.00x |
| mixed.json | orjson | 1.559 | 1.608 | 1.673 | 803.844 | 0.86x |
| mixed.json | msgspec | 1.692 | 1.719 | 1.775 | 803.844 | 0.81x |
| mixed.json | ujson | 2.129 | 2.183 | 2.367 | 803.844 | 0.64x |
| mixed.json | json | 3.335 | 3.382 | 3.480 | 803.844 | 0.41x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 304.927 | 307.629 | 316.020 | 381.031 | 1.00x |
| users.ndjson | orjson | 681.374 | 687.620 | 696.774 | 381.031 | 0.45x |
| users.ndjson | msgspec | 673.667 | 688.312 | 697.320 | 381.031 | 0.45x |
| users.ndjson | ujson | 849.937 | 856.520 | 874.992 | 381.031 | 0.36x |
| users.ndjson | json | 1030.134 | 1048.256 | 1054.356 | 381.031 | 0.29x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 68.810 | 71.999 | 102.769 | 1226.031 | 1.00x |
| users.json | orjson | 97.344 | 100.322 | 135.657 | 1226.031 | 0.72x |
| users.json | msgspec | 122.529 | 127.455 | 154.855 | 1226.031 | 0.56x |
| users.json | ujson | 452.956 | 459.456 | 485.777 | 1226.031 | 0.16x |
| users.json | json | 645.830 | 649.738 | 679.606 | 1226.031 | 0.11x |
| flat.json | strata | 2.106 | 2.144 | 2.208 | 351.922 | 1.00x |
| flat.json | orjson | 2.557 | 2.608 | 2.720 | 351.922 | 0.82x |
| flat.json | msgspec | 3.267 | 3.303 | 3.675 | 351.922 | 0.65x |
| flat.json | ujson | 9.250 | 9.373 | 9.574 | 351.922 | 0.23x |
| flat.json | json | 13.511 | 13.582 | 13.830 | 351.922 | 0.16x |
| nested.json | strata | 1.687 | 1.716 | 1.766 | 353.562 | 1.00x |
| nested.json | orjson | 2.353 | 2.375 | 2.432 | 353.562 | 0.72x |
| nested.json | msgspec | 2.951 | 2.997 | 3.037 | 353.562 | 0.57x |
| nested.json | ujson | 9.956 | 9.988 | 10.230 | 353.562 | 0.17x |
| nested.json | json | 15.979 | 16.049 | 16.106 | 353.562 | 0.11x |
| wide_arrays.json | strata | 11.793 | 12.164 | 13.721 | 806.359 | 1.00x |
| wide_arrays.json | orjson | 14.006 | 14.231 | 14.887 | 806.359 | 0.85x |
| wide_arrays.json | msgspec | 22.212 | 22.487 | 30.627 | 806.359 | 0.54x |
| wide_arrays.json | ujson | 64.517 | 64.726 | 66.145 | 806.359 | 0.19x |
| wide_arrays.json | json | 115.593 | 117.153 | 117.928 | 806.359 | 0.10x |
| mixed.json | strata | 0.529 | 0.546 | 0.573 | 803.844 | 1.00x |
| mixed.json | orjson | 0.568 | 0.625 | 0.729 | 803.844 | 0.87x |
| mixed.json | msgspec | 0.604 | 0.636 | 0.759 | 803.844 | 0.86x |
| mixed.json | ujson | 2.308 | 2.382 | 2.447 | 803.844 | 0.23x |
| mixed.json | json | 3.531 | 3.612 | 3.638 | 803.844 | 0.15x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.747 | 0.784 | 0.821 | 702.031 | 1.00x |
| users.json $[*].id | jmespath | 1.926 | 2.047 | 11.598 | 702.031 | 0.38x |
| users.json $[*].id | jsonpath-ng | 7.166 | 7.931 | 8.342 | 702.031 | 0.10x |
| users.json $[*].orders[*].total | strata | 14.292 | 14.687 | 17.242 | 554.500 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 32.977 | 34.507 | 45.538 | 554.500 | 0.43x |
| users.json $[*].orders[*].total | jsonpath-ng | 169.634 | 170.877 | 175.962 | 554.500 | 0.09x |
| users.json $..total | strata | 55.574 | 56.396 | 56.700 | 571.094 | 1.00x |
| users.json $..total | jsonpath-ng | 7775.953 | 7785.044 | 7788.624 | 571.094 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 138.298 | 139.872 | 142.218 | 1008.500 | 1.00x |
| users.json $[*].id | orjson+jmespath | 405.259 | 408.626 | 413.894 | 1008.500 | 0.34x |
| users.json $[*].id | orjson+jsonpath-ng | 490.816 | 497.840 | 502.334 | 1008.500 | 0.28x |
| users.json $[*].orders[*].total | strata | 139.327 | 139.830 | 142.175 | 795.406 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 432.828 | 442.864 | 449.976 | 795.406 | 0.32x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 1192.582 | 1209.919 | 1227.222 | 795.406 | 0.12x |
| users.json $..total | strata | 369.027 | 374.520 | 382.971 | 810.562 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 8768.202 | 8787.272 | 8797.468 | 810.562 | 0.04x |

