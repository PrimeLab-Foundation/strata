# Benchmark results - large

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: dc84475
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
| users.json | strata | 325.543 | 330.613 | 338.296 | 279.859 | 1.00x |
| users.json | orjson | 383.667 | 389.521 | 394.125 | 279.859 | 0.85x |
| users.json | msgspec | 393.658 | 397.558 | 406.133 | 279.859 | 0.83x |
| users.json | ujson | 535.610 | 537.528 | 544.600 | 279.859 | 0.62x |
| users.json | json | 647.558 | 651.866 | 660.897 | 279.859 | 0.51x |
| flat.json | strata | 6.281 | 6.354 | 6.538 | 345.531 | 1.00x |
| flat.json | orjson | 7.975 | 8.043 | 8.280 | 345.531 | 0.79x |
| flat.json | msgspec | 8.095 | 8.173 | 8.363 | 345.531 | 0.78x |
| flat.json | ujson | 11.765 | 11.821 | 12.023 | 345.531 | 0.54x |
| flat.json | json | 15.421 | 15.648 | 15.868 | 345.531 | 0.41x |
| nested.json | strata | 5.203 | 5.249 | 5.349 | 347.750 | 1.00x |
| nested.json | orjson | 6.686 | 6.780 | 6.906 | 347.750 | 0.77x |
| nested.json | msgspec | 6.699 | 6.781 | 7.051 | 347.750 | 0.77x |
| nested.json | ujson | 9.397 | 9.465 | 9.927 | 347.750 | 0.55x |
| nested.json | json | 13.764 | 13.960 | 14.192 | 347.750 | 0.38x |
| wide_arrays.json | strata | 33.327 | 33.703 | 34.995 | 417.250 | 1.00x |
| wide_arrays.json | orjson | 32.494 | 32.726 | 34.826 | 417.250 | 1.03x |
| wide_arrays.json | msgspec | 39.888 | 40.701 | 43.870 | 417.250 | 0.83x |
| wide_arrays.json | ujson | 51.541 | 52.266 | 54.212 | 417.250 | 0.64x |
| wide_arrays.json | json | 75.327 | 76.234 | 77.161 | 417.250 | 0.44x |
| mixed.json | strata | 1.259 | 1.274 | 1.297 | 774.328 | 1.00x |
| mixed.json | orjson | 2.057 | 2.088 | 2.210 | 774.328 | 0.61x |
| mixed.json | msgspec | 2.184 | 2.207 | 2.381 | 774.328 | 0.58x |
| mixed.json | ujson | 2.568 | 2.628 | 2.770 | 774.328 | 0.48x |
| mixed.json | json | 3.672 | 3.720 | 3.852 | 774.328 | 0.34x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 74.404 | 74.670 | 75.732 | 2150.094 | 1.00x |
| users.json | orjson | 82.989 | 84.466 | 86.692 | 2150.094 | 0.88x |
| users.json | msgspec | 108.955 | 112.870 | 119.803 | 2150.094 | 0.66x |
| users.json | ujson | 441.850 | 446.450 | 457.929 | 2150.094 | 0.17x |
| users.json | json | 603.366 | 604.908 | 609.591 | 2150.094 | 0.12x |
| flat.json | strata | 1.779 | 1.782 | 1.791 | 347.422 | 1.00x |
| flat.json | orjson | 2.118 | 2.125 | 2.334 | 347.422 | 0.84x |
| flat.json | msgspec | 2.805 | 2.827 | 2.877 | 347.422 | 0.63x |
| flat.json | ujson | 8.499 | 8.538 | 8.784 | 347.422 | 0.21x |
| flat.json | json | 12.104 | 12.175 | 12.389 | 347.422 | 0.15x |
| nested.json | strata | 1.357 | 1.366 | 1.399 | 347.766 | 1.00x |
| nested.json | orjson | 1.965 | 1.983 | 2.014 | 347.766 | 0.69x |
| nested.json | msgspec | 2.565 | 2.599 | 2.610 | 347.766 | 0.53x |
| nested.json | ujson | 9.377 | 9.403 | 9.475 | 347.766 | 0.15x |
| nested.json | json | 14.627 | 14.693 | 14.780 | 347.766 | 0.09x |
| wide_arrays.json | strata | 12.318 | 12.611 | 13.652 | 621.906 | 1.00x |
| wide_arrays.json | orjson | 11.543 | 11.750 | 11.995 | 621.906 | 1.07x |
| wide_arrays.json | msgspec | 19.945 | 20.282 | 20.616 | 621.906 | 0.62x |
| wide_arrays.json | ujson | 63.067 | 63.435 | 64.313 | 621.906 | 0.20x |
| wide_arrays.json | json | 108.134 | 108.832 | 109.860 | 621.906 | 0.12x |
| mixed.json | strata | 0.333 | 0.337 | 0.344 | 774.344 | 1.00x |
| mixed.json | orjson | 0.368 | 0.372 | 0.380 | 774.344 | 0.90x |
| mixed.json | msgspec | 0.405 | 0.408 | 0.418 | 774.344 | 0.82x |
| mixed.json | ujson | 2.061 | 2.075 | 2.149 | 774.344 | 0.16x |
| mixed.json | json | 3.163 | 3.184 | 3.213 | 774.344 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 337.951 | 343.133 | 357.065 | 902.422 | 1.00x |
| users.json | orjson | 460.584 | 467.785 | 489.013 | 902.422 | 0.73x |
| users.json | msgspec | 471.836 | 476.630 | 482.032 | 902.422 | 0.72x |
| users.json | ujson | 622.111 | 623.998 | 628.204 | 902.422 | 0.55x |
| users.json | json | 726.235 | 729.900 | 735.976 | 902.422 | 0.47x |
| flat.json | strata | 6.751 | 6.833 | 6.926 | 347.734 | 1.00x |
| flat.json | orjson | 8.819 | 8.938 | 9.211 | 347.734 | 0.76x |
| flat.json | msgspec | 9.137 | 9.392 | 9.687 | 347.734 | 0.73x |
| flat.json | ujson | 12.868 | 13.131 | 13.548 | 347.734 | 0.52x |
| flat.json | json | 16.354 | 16.562 | 16.973 | 347.734 | 0.41x |
| nested.json | strata | 5.491 | 5.506 | 5.552 | 349.234 | 1.00x |
| nested.json | orjson | 8.332 | 8.389 | 8.858 | 349.234 | 0.66x |
| nested.json | msgspec | 8.465 | 8.500 | 8.948 | 349.234 | 0.65x |
| nested.json | ujson | 11.396 | 11.473 | 11.846 | 349.234 | 0.48x |
| nested.json | json | 15.487 | 15.522 | 16.047 | 349.234 | 0.35x |
| wide_arrays.json | strata | 34.412 | 34.885 | 36.897 | 640.906 | 1.00x |
| wide_arrays.json | orjson | 38.172 | 38.731 | 40.123 | 640.906 | 0.90x |
| wide_arrays.json | msgspec | 46.028 | 46.720 | 47.799 | 640.906 | 0.75x |
| wide_arrays.json | ujson | 59.163 | 59.982 | 61.360 | 640.906 | 0.58x |
| wide_arrays.json | json | 81.285 | 81.655 | 82.752 | 640.906 | 0.43x |
| mixed.json | strata | 1.355 | 1.383 | 1.401 | 774.656 | 1.00x |
| mixed.json | orjson | 2.356 | 2.392 | 2.423 | 774.656 | 0.58x |
| mixed.json | msgspec | 2.483 | 2.530 | 2.715 | 774.656 | 0.55x |
| mixed.json | ujson | 2.907 | 2.958 | 3.212 | 774.656 | 0.47x |
| mixed.json | json | 3.952 | 4.007 | 4.048 | 774.656 | 0.35x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 325.764 | 329.128 | 332.288 | 371.891 | 1.00x |
| users.ndjson | orjson | 519.899 | 524.073 | 526.418 | 371.891 | 0.63x |
| users.ndjson | msgspec | 525.226 | 528.202 | 530.619 | 371.891 | 0.62x |
| users.ndjson | ujson | 678.853 | 683.316 | 686.057 | 371.891 | 0.48x |
| users.ndjson | json | 785.500 | 788.504 | 794.201 | 371.891 | 0.42x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 85.142 | 86.193 | 96.183 | 2181.828 | 1.00x |
| users.json | orjson | 93.707 | 95.196 | 96.853 | 2181.828 | 0.91x |
| users.json | msgspec | 119.727 | 121.084 | 127.161 | 2181.828 | 0.71x |
| users.json | ujson | 439.235 | 448.187 | 458.589 | 2181.828 | 0.19x |
| users.json | json | 606.326 | 614.965 | 621.203 | 2181.828 | 0.14x |
| flat.json | strata | 2.135 | 2.215 | 2.337 | 347.734 | 1.00x |
| flat.json | orjson | 2.500 | 2.542 | 2.614 | 347.734 | 0.87x |
| flat.json | msgspec | 3.174 | 3.264 | 3.390 | 347.734 | 0.68x |
| flat.json | ujson | 9.154 | 9.318 | 9.472 | 347.734 | 0.24x |
| flat.json | json | 12.582 | 12.682 | 12.954 | 347.734 | 0.17x |
| nested.json | strata | 1.675 | 1.745 | 1.802 | 349.234 | 1.00x |
| nested.json | orjson | 2.319 | 2.361 | 2.386 | 349.234 | 0.74x |
| nested.json | msgspec | 2.947 | 2.956 | 3.041 | 349.234 | 0.59x |
| nested.json | ujson | 9.864 | 9.885 | 10.030 | 349.234 | 0.18x |
| nested.json | json | 15.033 | 15.254 | 15.411 | 349.234 | 0.11x |
| wide_arrays.json | strata | 13.887 | 14.121 | 14.541 | 774.188 | 1.00x |
| wide_arrays.json | orjson | 13.194 | 13.317 | 13.569 | 774.188 | 1.06x |
| wide_arrays.json | msgspec | 21.798 | 22.081 | 22.360 | 774.188 | 0.64x |
| wide_arrays.json | ujson | 65.305 | 65.685 | 66.258 | 774.188 | 0.21x |
| wide_arrays.json | json | 110.017 | 110.660 | 111.620 | 774.188 | 0.13x |
| mixed.json | strata | 0.506 | 0.525 | 0.575 | 774.656 | 1.00x |
| mixed.json | orjson | 0.528 | 0.552 | 0.601 | 774.656 | 0.95x |
| mixed.json | msgspec | 0.563 | 0.591 | 0.626 | 774.656 | 0.89x |
| mixed.json | ujson | 2.265 | 2.288 | 2.427 | 774.656 | 0.23x |
| mixed.json | json | 3.335 | 3.388 | 3.600 | 774.656 | 0.16x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.673 | 0.690 | 0.729 | 1066.719 | 1.00x |
| users.json $[*].id | jmespath | 2.029 | 2.063 | 16.185 | 1066.719 | 0.33x |
| users.json $[*].id | jsonpath-ng | 70.955 | 71.358 | 72.255 | 1066.719 | 0.01x |
| users.json $[*].orders[*].total | strata | 13.159 | 13.308 | 13.618 | 815.344 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 35.920 | 36.504 | 41.183 | 815.344 | 0.36x |
| users.json $[*].orders[*].total | jsonpath-ng | 216.082 | 220.833 | 225.490 | 815.344 | 0.06x |
| users.json $..total | strata | 53.480 | 54.023 | 55.133 | 613.531 | 1.00x |
| users.json $..total | jsonpath-ng | 8579.157 | 8586.522 | 8757.470 | 613.531 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 132.869 | 133.025 | 134.087 | 823.578 | 1.00x |
| users.json $[*].id | orjson+jmespath | 451.110 | 454.658 | 462.314 | 823.578 | 0.29x |
| users.json $[*].id | orjson+jsonpath-ng | 457.225 | 459.093 | 462.393 | 823.578 | 0.29x |
| users.json $[*].orders[*].total | strata | 134.049 | 134.635 | 135.376 | 842.906 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 484.595 | 484.929 | 491.216 | 842.906 | 0.28x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 606.752 | 612.928 | 620.834 | 842.906 | 0.22x |
| users.json $..total | strata | 443.987 | 446.331 | 450.687 | 851.938 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 8938.374 | 8949.535 | 8972.094 | 851.938 | 0.05x |

