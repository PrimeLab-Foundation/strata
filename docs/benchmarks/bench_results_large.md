# Benchmark results - large

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 9c9d784
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
| users.json | strata | 328.072 | 340.420 | 354.942 | 279.906 | 1.00x |
| users.json | orjson | 383.435 | 401.482 | 413.031 | 279.906 | 0.85x |
| users.json | msgspec | 393.454 | 408.489 | 417.270 | 279.906 | 0.83x |
| users.json | ujson | 528.052 | 546.833 | 554.003 | 279.906 | 0.62x |
| users.json | json | 650.122 | 660.707 | 683.571 | 279.906 | 0.52x |
| flat.json | strata | 6.504 | 6.562 | 6.656 | 348.656 | 1.00x |
| flat.json | orjson | 7.995 | 8.107 | 8.127 | 348.656 | 0.81x |
| flat.json | msgspec | 8.069 | 8.216 | 8.633 | 348.656 | 0.80x |
| flat.json | ujson | 11.704 | 11.850 | 12.098 | 348.656 | 0.55x |
| flat.json | json | 15.422 | 15.835 | 15.980 | 348.656 | 0.41x |
| nested.json | strata | 5.404 | 5.524 | 5.597 | 350.859 | 1.00x |
| nested.json | orjson | 6.813 | 6.881 | 6.946 | 350.859 | 0.80x |
| nested.json | msgspec | 6.862 | 6.946 | 7.013 | 350.859 | 0.80x |
| nested.json | ujson | 9.508 | 9.631 | 9.773 | 350.859 | 0.57x |
| nested.json | json | 14.030 | 14.214 | 14.655 | 350.859 | 0.39x |
| wide_arrays.json | strata | 33.659 | 33.739 | 34.523 | 421.797 | 1.00x |
| wide_arrays.json | orjson | 32.894 | 32.970 | 34.578 | 421.797 | 1.02x |
| wide_arrays.json | msgspec | 40.683 | 40.951 | 44.094 | 421.797 | 0.82x |
| wide_arrays.json | ujson | 52.747 | 53.083 | 54.722 | 421.797 | 0.64x |
| wide_arrays.json | json | 76.547 | 76.874 | 78.256 | 421.797 | 0.44x |
| mixed.json | strata | 1.292 | 1.321 | 1.427 | 775.875 | 1.00x |
| mixed.json | orjson | 2.084 | 2.099 | 2.969 | 775.875 | 0.63x |
| mixed.json | msgspec | 2.199 | 2.216 | 2.845 | 775.875 | 0.60x |
| mixed.json | ujson | 2.604 | 2.686 | 2.974 | 775.875 | 0.49x |
| mixed.json | json | 3.729 | 3.781 | 4.356 | 775.875 | 0.35x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 67.998 | 69.113 | 71.183 | 2150.141 | 1.00x |
| users.json | orjson | 83.676 | 84.745 | 88.721 | 2150.141 | 0.82x |
| users.json | msgspec | 110.353 | 112.419 | 118.014 | 2150.141 | 0.61x |
| users.json | ujson | 434.469 | 437.064 | 456.030 | 2150.141 | 0.16x |
| users.json | json | 598.769 | 601.874 | 614.356 | 2150.141 | 0.11x |
| flat.json | strata | 1.738 | 1.742 | 1.760 | 350.547 | 1.00x |
| flat.json | orjson | 2.141 | 2.150 | 2.162 | 350.547 | 0.81x |
| flat.json | msgspec | 2.833 | 2.844 | 2.872 | 350.547 | 0.61x |
| flat.json | ujson | 8.582 | 8.615 | 8.698 | 350.547 | 0.20x |
| flat.json | json | 12.305 | 12.334 | 12.398 | 350.547 | 0.14x |
| nested.json | strata | 1.226 | 1.232 | 1.265 | 350.875 | 1.00x |
| nested.json | orjson | 2.012 | 2.024 | 2.067 | 350.875 | 0.61x |
| nested.json | msgspec | 2.600 | 2.625 | 2.651 | 350.875 | 0.47x |
| nested.json | ujson | 9.574 | 9.653 | 9.811 | 350.875 | 0.13x |
| nested.json | json | 14.926 | 15.085 | 15.588 | 350.875 | 0.08x |
| wide_arrays.json | strata | 12.507 | 12.797 | 13.386 | 626.453 | 1.00x |
| wide_arrays.json | orjson | 11.680 | 12.012 | 15.863 | 626.453 | 1.07x |
| wide_arrays.json | msgspec | 19.954 | 20.702 | 21.996 | 626.453 | 0.62x |
| wide_arrays.json | ujson | 63.081 | 64.708 | 68.974 | 626.453 | 0.20x |
| wide_arrays.json | json | 109.512 | 111.355 | 114.361 | 626.453 | 0.11x |
| mixed.json | strata | 0.308 | 0.314 | 0.329 | 775.891 | 1.00x |
| mixed.json | orjson | 0.381 | 0.386 | 0.390 | 775.891 | 0.81x |
| mixed.json | msgspec | 0.421 | 0.425 | 0.444 | 775.891 | 0.74x |
| mixed.json | ujson | 2.096 | 2.118 | 2.215 | 775.891 | 0.15x |
| mixed.json | json | 3.217 | 3.224 | 3.254 | 775.891 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 345.326 | 349.465 | 380.504 | 902.500 | 1.00x |
| users.json | orjson | 461.208 | 474.937 | 503.108 | 902.500 | 0.74x |
| users.json | msgspec | 479.547 | 482.189 | 502.137 | 902.500 | 0.72x |
| users.json | ujson | 626.833 | 631.034 | 659.079 | 902.500 | 0.55x |
| users.json | json | 733.179 | 739.172 | 782.195 | 902.500 | 0.47x |
| flat.json | strata | 6.948 | 7.005 | 7.087 | 350.844 | 1.00x |
| flat.json | orjson | 8.923 | 8.969 | 9.271 | 350.844 | 0.78x |
| flat.json | msgspec | 9.182 | 9.241 | 9.401 | 350.844 | 0.76x |
| flat.json | ujson | 12.975 | 13.041 | 13.194 | 350.844 | 0.54x |
| flat.json | json | 16.787 | 16.832 | 17.247 | 350.844 | 0.42x |
| nested.json | strata | 5.692 | 5.734 | 5.863 | 353.781 | 1.00x |
| nested.json | orjson | 8.544 | 8.598 | 8.654 | 353.781 | 0.67x |
| nested.json | msgspec | 8.570 | 8.715 | 8.961 | 353.781 | 0.66x |
| nested.json | ujson | 11.533 | 11.632 | 12.212 | 353.781 | 0.49x |
| nested.json | json | 15.802 | 15.902 | 16.255 | 353.781 | 0.36x |
| wide_arrays.json | strata | 35.436 | 35.994 | 41.355 | 642.453 | 1.00x |
| wide_arrays.json | orjson | 38.994 | 39.513 | 44.527 | 642.453 | 0.91x |
| wide_arrays.json | msgspec | 47.553 | 48.209 | 50.990 | 642.453 | 0.75x |
| wide_arrays.json | ujson | 60.901 | 61.509 | 62.474 | 642.453 | 0.59x |
| wide_arrays.json | json | 82.632 | 83.962 | 84.949 | 642.453 | 0.43x |
| mixed.json | strata | 1.392 | 1.407 | 1.440 | 775.891 | 1.00x |
| mixed.json | orjson | 2.397 | 2.430 | 2.691 | 775.891 | 0.58x |
| mixed.json | msgspec | 2.514 | 2.549 | 2.765 | 775.891 | 0.55x |
| mixed.json | ujson | 2.983 | 3.012 | 3.249 | 775.891 | 0.47x |
| mixed.json | json | 4.031 | 4.062 | 4.293 | 775.891 | 0.35x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 333.191 | 346.579 | 357.992 | 376.000 | 1.00x |
| users.ndjson | orjson | 526.608 | 538.782 | 561.657 | 376.000 | 0.64x |
| users.ndjson | msgspec | 530.691 | 548.864 | 557.214 | 376.000 | 0.63x |
| users.ndjson | ujson | 687.466 | 708.204 | 753.257 | 376.000 | 0.49x |
| users.ndjson | json | 796.321 | 810.896 | 828.515 | 376.000 | 0.43x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 77.681 | 79.266 | 80.168 | 1523.984 | 1.00x |
| users.json | orjson | 91.472 | 96.423 | 109.072 | 1523.984 | 0.82x |
| users.json | msgspec | 119.268 | 120.557 | 143.814 | 1523.984 | 0.66x |
| users.json | ujson | 439.344 | 443.628 | 455.122 | 1523.984 | 0.18x |
| users.json | json | 602.684 | 609.139 | 626.431 | 1523.984 | 0.13x |
| flat.json | strata | 2.149 | 2.182 | 2.795 | 350.844 | 1.00x |
| flat.json | orjson | 2.536 | 2.757 | 3.629 | 350.844 | 0.79x |
| flat.json | msgspec | 3.266 | 3.352 | 4.508 | 350.844 | 0.65x |
| flat.json | ujson | 9.428 | 9.571 | 9.978 | 350.844 | 0.23x |
| flat.json | json | 12.742 | 12.879 | 13.537 | 350.844 | 0.17x |
| nested.json | strata | 1.583 | 1.643 | 3.457 | 353.781 | 1.00x |
| nested.json | orjson | 2.402 | 2.540 | 2.949 | 353.781 | 0.65x |
| nested.json | msgspec | 2.984 | 3.092 | 3.941 | 353.781 | 0.53x |
| nested.json | ujson | 9.988 | 10.272 | 10.719 | 353.781 | 0.16x |
| nested.json | json | 15.482 | 15.810 | 16.777 | 353.781 | 0.10x |
| wide_arrays.json | strata | 14.072 | 14.870 | 15.587 | 774.734 | 1.00x |
| wide_arrays.json | orjson | 13.650 | 14.132 | 14.404 | 774.734 | 1.05x |
| wide_arrays.json | msgspec | 22.057 | 22.930 | 23.724 | 774.734 | 0.65x |
| wide_arrays.json | ujson | 66.293 | 67.415 | 69.115 | 774.734 | 0.22x |
| wide_arrays.json | json | 112.378 | 114.230 | 133.402 | 774.734 | 0.13x |
| mixed.json | strata | 0.501 | 0.551 | 0.906 | 775.891 | 1.00x |
| mixed.json | orjson | 0.566 | 0.619 | 0.665 | 775.891 | 0.89x |
| mixed.json | msgspec | 0.609 | 0.639 | 0.706 | 775.891 | 0.86x |
| mixed.json | ujson | 2.268 | 2.391 | 2.621 | 775.891 | 0.23x |
| mixed.json | json | 3.414 | 3.498 | 3.647 | 775.891 | 0.16x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.673 | 0.687 | 0.726 | 892.703 | 1.00x |
| users.json $[*].id | jmespath | 1.993 | 2.008 | 11.953 | 892.703 | 0.34x |
| users.json $[*].id | jsonpath-ng | 70.464 | 70.835 | 76.472 | 892.703 | 0.01x |
| users.json $[*].orders[*].total | strata | 12.921 | 13.021 | 13.279 | 815.047 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 34.558 | 34.941 | 36.639 | 815.047 | 0.37x |
| users.json $[*].orders[*].total | jsonpath-ng | 216.172 | 218.674 | 240.191 | 815.047 | 0.06x |
| users.json $..total | strata | 53.172 | 53.564 | 54.210 | 619.391 | 1.00x |
| users.json $..total | jsonpath-ng | 8615.870 | 8662.544 | 8696.760 | 619.391 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 132.492 | 133.393 | 134.826 | 823.766 | 1.00x |
| users.json $[*].id | orjson+jmespath | 453.789 | 457.497 | 463.607 | 823.766 | 0.29x |
| users.json $[*].id | orjson+jsonpath-ng | 459.346 | 462.377 | 468.674 | 823.766 | 0.29x |
| users.json $[*].orders[*].total | strata | 133.095 | 134.095 | 136.173 | 845.625 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 487.503 | 490.018 | 494.447 | 845.625 | 0.27x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 611.345 | 616.038 | 622.849 | 845.625 | 0.22x |
| users.json $..total | strata | 451.005 | 454.262 | 468.500 | 856.781 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 9040.398 | 9061.690 | 9136.846 | 856.781 | 0.05x |

