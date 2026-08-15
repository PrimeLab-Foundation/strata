# Benchmark results - large

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 4d4a4be
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
| users.json | strata | 335.801 | 339.953 | 345.800 | 280.062 | 1.00x |
| users.json | orjson | 382.922 | 386.458 | 394.286 | 280.062 | 0.88x |
| users.json | msgspec | 395.209 | 398.157 | 416.296 | 280.062 | 0.85x |
| users.json | ujson | 531.024 | 537.230 | 544.979 | 280.062 | 0.63x |
| users.json | json | 645.739 | 658.937 | 662.770 | 280.062 | 0.52x |
| flat.json | strata | 6.291 | 6.372 | 6.482 | 352.766 | 1.00x |
| flat.json | orjson | 7.871 | 8.015 | 8.102 | 352.766 | 0.80x |
| flat.json | msgspec | 8.100 | 8.198 | 8.303 | 352.766 | 0.78x |
| flat.json | ujson | 11.503 | 11.744 | 12.122 | 352.766 | 0.54x |
| flat.json | json | 15.158 | 15.407 | 15.751 | 352.766 | 0.41x |
| nested.json | strata | 5.395 | 5.444 | 5.625 | 355.031 | 1.00x |
| nested.json | orjson | 6.770 | 6.827 | 6.977 | 355.031 | 0.80x |
| nested.json | msgspec | 6.754 | 6.893 | 7.575 | 355.031 | 0.79x |
| nested.json | ujson | 9.488 | 9.547 | 10.188 | 355.031 | 0.57x |
| nested.json | json | 13.952 | 14.205 | 15.077 | 355.031 | 0.38x |
| wide_arrays.json | strata | 33.364 | 33.558 | 34.014 | 425.953 | 1.00x |
| wide_arrays.json | orjson | 32.583 | 32.803 | 33.541 | 425.953 | 1.02x |
| wide_arrays.json | msgspec | 40.103 | 40.712 | 41.899 | 425.953 | 0.82x |
| wide_arrays.json | ujson | 51.998 | 52.623 | 53.929 | 425.953 | 0.64x |
| wide_arrays.json | json | 75.793 | 76.223 | 77.330 | 425.953 | 0.44x |
| mixed.json | strata | 1.277 | 1.286 | 1.319 | 779.031 | 1.00x |
| mixed.json | orjson | 2.051 | 2.072 | 2.119 | 779.031 | 0.62x |
| mixed.json | msgspec | 2.158 | 2.178 | 2.275 | 779.031 | 0.59x |
| mixed.json | ujson | 2.561 | 2.600 | 2.624 | 779.031 | 0.49x |
| mixed.json | json | 3.676 | 3.762 | 3.916 | 779.031 | 0.34x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 75.272 | 75.521 | 76.630 | 1956.125 | 1.00x |
| users.json | orjson | 81.991 | 84.178 | 86.539 | 1956.125 | 0.90x |
| users.json | msgspec | 108.344 | 110.828 | 116.073 | 1956.125 | 0.68x |
| users.json | ujson | 435.756 | 439.527 | 449.460 | 1956.125 | 0.17x |
| users.json | json | 601.323 | 604.642 | 609.381 | 1956.125 | 0.12x |
| flat.json | strata | 1.782 | 1.808 | 1.947 | 354.719 | 1.00x |
| flat.json | orjson | 2.121 | 2.151 | 2.283 | 354.719 | 0.84x |
| flat.json | msgspec | 2.795 | 2.873 | 3.008 | 354.719 | 0.63x |
| flat.json | ujson | 8.459 | 8.531 | 8.619 | 354.719 | 0.21x |
| flat.json | json | 12.156 | 12.414 | 12.693 | 354.719 | 0.15x |
| nested.json | strata | 1.446 | 1.457 | 1.482 | 355.031 | 1.00x |
| nested.json | orjson | 1.978 | 1.995 | 2.151 | 355.031 | 0.73x |
| nested.json | msgspec | 2.579 | 2.600 | 2.736 | 355.031 | 0.56x |
| nested.json | ujson | 9.537 | 9.684 | 10.153 | 355.031 | 0.15x |
| nested.json | json | 14.762 | 14.904 | 15.382 | 355.031 | 0.10x |
| wide_arrays.json | strata | 12.420 | 12.552 | 13.037 | 630.609 | 1.00x |
| wide_arrays.json | orjson | 11.586 | 11.672 | 12.199 | 630.609 | 1.08x |
| wide_arrays.json | msgspec | 19.699 | 20.189 | 20.377 | 630.609 | 0.62x |
| wide_arrays.json | ujson | 62.380 | 62.972 | 64.315 | 630.609 | 0.20x |
| wide_arrays.json | json | 108.378 | 109.470 | 110.288 | 630.609 | 0.11x |
| mixed.json | strata | 0.345 | 0.349 | 0.352 | 779.047 | 1.00x |
| mixed.json | orjson | 0.374 | 0.378 | 0.397 | 779.047 | 0.92x |
| mixed.json | msgspec | 0.412 | 0.414 | 0.457 | 779.047 | 0.84x |
| mixed.json | ujson | 2.056 | 2.075 | 2.208 | 779.047 | 0.17x |
| mixed.json | json | 3.182 | 3.217 | 3.241 | 779.047 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 334.776 | 354.038 | 355.576 | 902.281 | 1.00x |
| users.json | orjson | 445.482 | 467.073 | 489.948 | 902.281 | 0.76x |
| users.json | msgspec | 455.884 | 475.151 | 481.350 | 902.281 | 0.75x |
| users.json | ujson | 602.988 | 625.405 | 631.242 | 902.281 | 0.57x |
| users.json | json | 718.168 | 737.718 | 741.403 | 902.281 | 0.48x |
| flat.json | strata | 6.808 | 6.885 | 6.946 | 355.016 | 1.00x |
| flat.json | orjson | 8.757 | 8.969 | 9.576 | 355.016 | 0.77x |
| flat.json | msgspec | 9.120 | 9.350 | 9.708 | 355.016 | 0.74x |
| flat.json | ujson | 12.729 | 12.810 | 13.649 | 355.016 | 0.54x |
| flat.json | json | 16.123 | 16.245 | 16.843 | 355.016 | 0.42x |
| nested.json | strata | 5.642 | 5.793 | 5.953 | 357.938 | 1.00x |
| nested.json | orjson | 8.372 | 8.495 | 9.018 | 357.938 | 0.68x |
| nested.json | msgspec | 8.537 | 8.721 | 9.504 | 357.938 | 0.66x |
| nested.json | ujson | 11.474 | 11.559 | 12.343 | 357.938 | 0.50x |
| nested.json | json | 15.638 | 15.875 | 16.455 | 357.938 | 0.36x |
| wide_arrays.json | strata | 34.903 | 35.143 | 36.535 | 642.609 | 1.00x |
| wide_arrays.json | orjson | 38.273 | 38.553 | 39.223 | 642.609 | 0.91x |
| wide_arrays.json | msgspec | 46.185 | 46.496 | 47.703 | 642.609 | 0.76x |
| wide_arrays.json | ujson | 59.276 | 60.008 | 62.924 | 642.609 | 0.59x |
| wide_arrays.json | json | 81.557 | 82.156 | 83.546 | 642.609 | 0.43x |
| mixed.json | strata | 1.377 | 1.409 | 1.440 | 779.047 | 1.00x |
| mixed.json | orjson | 2.370 | 2.460 | 2.742 | 779.047 | 0.57x |
| mixed.json | msgspec | 2.488 | 2.556 | 2.662 | 779.047 | 0.55x |
| mixed.json | ujson | 2.935 | 2.977 | 3.075 | 779.047 | 0.47x |
| mixed.json | json | 4.013 | 4.046 | 4.499 | 779.047 | 0.35x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 324.602 | 326.731 | 329.638 | 379.109 | 1.00x |
| users.ndjson | orjson | 515.670 | 517.374 | 531.579 | 379.109 | 0.63x |
| users.ndjson | msgspec | 517.494 | 521.751 | 529.076 | 379.109 | 0.63x |
| users.ndjson | ujson | 671.989 | 678.081 | 679.664 | 379.109 | 0.48x |
| users.ndjson | json | 785.755 | 789.892 | 797.362 | 379.109 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 85.188 | 87.108 | 91.794 | 1600.438 | 1.00x |
| users.json | orjson | 90.112 | 94.780 | 101.115 | 1600.438 | 0.92x |
| users.json | msgspec | 117.581 | 121.587 | 128.281 | 1600.438 | 0.72x |
| users.json | ujson | 444.060 | 455.691 | 477.982 | 1600.438 | 0.19x |
| users.json | json | 609.940 | 618.090 | 626.595 | 1600.438 | 0.14x |
| flat.json | strata | 2.163 | 2.200 | 2.302 | 355.016 | 1.00x |
| flat.json | orjson | 2.516 | 2.562 | 2.743 | 355.016 | 0.86x |
| flat.json | msgspec | 3.241 | 3.284 | 3.407 | 355.016 | 0.67x |
| flat.json | ujson | 9.167 | 9.322 | 9.518 | 355.016 | 0.24x |
| flat.json | json | 12.621 | 12.700 | 13.000 | 355.016 | 0.17x |
| nested.json | strata | 1.796 | 1.860 | 1.966 | 357.938 | 1.00x |
| nested.json | orjson | 2.403 | 2.435 | 2.544 | 357.938 | 0.76x |
| nested.json | msgspec | 2.948 | 3.019 | 3.249 | 357.938 | 0.62x |
| nested.json | ujson | 9.949 | 10.068 | 10.207 | 357.938 | 0.18x |
| nested.json | json | 15.359 | 15.606 | 15.679 | 357.938 | 0.12x |
| wide_arrays.json | strata | 13.992 | 14.176 | 14.716 | 778.891 | 1.00x |
| wide_arrays.json | orjson | 13.215 | 13.416 | 13.654 | 778.891 | 1.06x |
| wide_arrays.json | msgspec | 21.880 | 22.164 | 23.016 | 778.891 | 0.64x |
| wide_arrays.json | ujson | 64.991 | 65.507 | 65.849 | 778.891 | 0.22x |
| wide_arrays.json | json | 110.528 | 111.676 | 113.232 | 778.891 | 0.13x |
| mixed.json | strata | 0.511 | 0.548 | 0.666 | 779.047 | 1.00x |
| mixed.json | orjson | 0.554 | 0.576 | 0.605 | 779.047 | 0.95x |
| mixed.json | msgspec | 0.586 | 0.617 | 0.695 | 779.047 | 0.89x |
| mixed.json | ujson | 2.280 | 2.308 | 2.530 | 779.047 | 0.24x |
| mixed.json | json | 3.337 | 3.410 | 3.623 | 779.047 | 0.16x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.678 | 0.693 | 0.740 | 870.703 | 1.00x |
| users.json $[*].id | jmespath | 2.015 | 2.058 | 14.107 | 870.703 | 0.34x |
| users.json $[*].id | jsonpath-ng | 71.298 | 71.881 | 75.021 | 870.703 | 0.01x |
| users.json $[*].orders[*].total | strata | 12.974 | 13.170 | 13.577 | 816.578 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 34.721 | 35.304 | 36.406 | 816.578 | 0.37x |
| users.json $[*].orders[*].total | jsonpath-ng | 216.628 | 221.414 | 226.513 | 816.578 | 0.06x |
| users.json $..total | strata | 53.059 | 53.367 | 53.590 | 622.547 | 1.00x |
| users.json $..total | jsonpath-ng | 8588.371 | 8593.505 | 8605.778 | 622.547 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 133.620 | 134.478 | 135.160 | 823.703 | 1.00x |
| users.json $[*].id | orjson+jmespath | 452.452 | 454.593 | 466.883 | 823.703 | 0.30x |
| users.json $[*].id | orjson+jsonpath-ng | 455.496 | 459.144 | 461.514 | 823.703 | 0.29x |
| users.json $[*].orders[*].total | strata | 134.726 | 135.427 | 136.422 | 846.219 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 482.936 | 484.860 | 489.573 | 846.219 | 0.28x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 606.021 | 611.339 | 620.789 | 846.219 | 0.22x |
| users.json $..total | strata | 452.358 | 454.909 | 456.209 | 860.938 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 8932.931 | 8942.561 | 8950.695 | 860.938 | 0.05x |

