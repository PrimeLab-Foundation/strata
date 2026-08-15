# Benchmark results - large

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 26c72e3
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
| users.json | strata | 324.677 | 328.881 | 333.924 | 493.297 | 1.00x |
| users.json | orjson | 380.539 | 384.319 | 386.336 | 493.297 | 0.86x |
| users.json | msgspec | 390.902 | 394.219 | 397.386 | 493.297 | 0.83x |
| users.json | ujson | 531.075 | 535.363 | 553.032 | 493.297 | 0.61x |
| users.json | json | 648.744 | 654.416 | 657.492 | 493.297 | 0.50x |
| flat.json | strata | 6.180 | 6.286 | 6.369 | 349.859 | 1.00x |
| flat.json | orjson | 7.776 | 7.868 | 8.034 | 349.859 | 0.80x |
| flat.json | msgspec | 7.865 | 7.926 | 8.109 | 349.859 | 0.79x |
| flat.json | ujson | 11.362 | 11.451 | 11.602 | 349.859 | 0.55x |
| flat.json | json | 14.883 | 15.128 | 15.275 | 349.859 | 0.42x |
| nested.json | strata | 5.367 | 5.435 | 5.551 | 352.094 | 1.00x |
| nested.json | orjson | 6.744 | 6.833 | 6.960 | 352.094 | 0.80x |
| nested.json | msgspec | 6.718 | 6.853 | 7.032 | 352.094 | 0.79x |
| nested.json | ujson | 9.406 | 9.563 | 9.675 | 352.094 | 0.57x |
| nested.json | json | 13.861 | 14.023 | 14.381 | 352.094 | 0.39x |
| wide_arrays.json | strata | 33.749 | 33.871 | 34.265 | 421.594 | 1.00x |
| wide_arrays.json | orjson | 32.554 | 33.220 | 33.660 | 421.594 | 1.02x |
| wide_arrays.json | msgspec | 40.455 | 40.974 | 41.492 | 421.594 | 0.83x |
| wide_arrays.json | ujson | 52.244 | 52.909 | 53.243 | 421.594 | 0.64x |
| wide_arrays.json | json | 75.867 | 76.775 | 77.386 | 421.594 | 0.44x |
| mixed.json | strata | 1.259 | 1.285 | 1.360 | 775.688 | 1.00x |
| mixed.json | orjson | 2.051 | 2.085 | 2.443 | 775.688 | 0.62x |
| mixed.json | msgspec | 2.150 | 2.199 | 2.334 | 775.688 | 0.58x |
| mixed.json | ujson | 2.532 | 2.600 | 2.712 | 775.688 | 0.49x |
| mixed.json | json | 3.673 | 3.746 | 3.975 | 775.688 | 0.34x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 72.703 | 73.851 | 75.134 | 1464.328 | 1.00x |
| users.json | orjson | 82.003 | 84.683 | 86.664 | 1464.328 | 0.87x |
| users.json | msgspec | 109.281 | 111.463 | 115.395 | 1464.328 | 0.66x |
| users.json | ujson | 442.624 | 450.687 | 460.029 | 1464.328 | 0.16x |
| users.json | json | 598.553 | 603.097 | 616.883 | 1464.328 | 0.12x |
| flat.json | strata | 1.758 | 1.763 | 1.795 | 351.766 | 1.00x |
| flat.json | orjson | 2.111 | 2.152 | 2.173 | 351.766 | 0.82x |
| flat.json | msgspec | 2.786 | 2.838 | 2.878 | 351.766 | 0.62x |
| flat.json | ujson | 8.442 | 8.580 | 8.644 | 351.766 | 0.21x |
| flat.json | json | 12.184 | 12.339 | 12.358 | 351.766 | 0.14x |
| nested.json | strata | 1.318 | 1.329 | 1.350 | 352.109 | 1.00x |
| nested.json | orjson | 1.975 | 2.009 | 2.068 | 352.109 | 0.66x |
| nested.json | msgspec | 2.583 | 2.621 | 2.705 | 352.109 | 0.51x |
| nested.json | ujson | 9.464 | 9.537 | 9.689 | 352.109 | 0.14x |
| nested.json | json | 14.783 | 15.048 | 15.191 | 352.109 | 0.09x |
| wide_arrays.json | strata | 12.470 | 12.614 | 13.591 | 626.266 | 1.00x |
| wide_arrays.json | orjson | 11.576 | 11.823 | 12.496 | 626.266 | 1.07x |
| wide_arrays.json | msgspec | 20.069 | 20.307 | 20.517 | 626.266 | 0.62x |
| wide_arrays.json | ujson | 63.007 | 63.252 | 63.832 | 626.266 | 0.20x |
| wide_arrays.json | json | 108.689 | 109.662 | 111.100 | 626.266 | 0.12x |
| mixed.json | strata | 0.326 | 0.334 | 0.392 | 775.703 | 1.00x |
| mixed.json | orjson | 0.377 | 0.384 | 0.395 | 775.703 | 0.87x |
| mixed.json | msgspec | 0.414 | 0.423 | 0.432 | 775.703 | 0.79x |
| mixed.json | ujson | 2.059 | 2.083 | 2.240 | 775.703 | 0.16x |
| mixed.json | json | 3.194 | 3.254 | 3.426 | 775.703 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 341.253 | 344.900 | 348.706 | 902.016 | 1.00x |
| users.json | orjson | 462.299 | 466.303 | 486.576 | 902.016 | 0.74x |
| users.json | msgspec | 472.673 | 475.396 | 516.833 | 902.016 | 0.73x |
| users.json | ujson | 617.398 | 623.807 | 632.213 | 902.016 | 0.55x |
| users.json | json | 731.245 | 735.305 | 740.065 | 902.016 | 0.47x |
| flat.json | strata | 6.563 | 6.659 | 6.772 | 352.062 | 1.00x |
| flat.json | orjson | 8.631 | 8.734 | 9.250 | 352.062 | 0.76x |
| flat.json | msgspec | 8.765 | 8.926 | 9.259 | 352.062 | 0.75x |
| flat.json | ujson | 12.373 | 12.584 | 13.071 | 352.062 | 0.53x |
| flat.json | json | 15.806 | 15.969 | 16.630 | 352.062 | 0.42x |
| nested.json | strata | 5.617 | 5.660 | 5.729 | 353.578 | 1.00x |
| nested.json | orjson | 8.333 | 8.377 | 8.762 | 353.578 | 0.68x |
| nested.json | msgspec | 8.433 | 8.492 | 8.833 | 353.578 | 0.67x |
| nested.json | ujson | 11.323 | 11.498 | 11.830 | 353.578 | 0.49x |
| nested.json | json | 15.498 | 15.677 | 16.043 | 353.578 | 0.36x |
| wide_arrays.json | strata | 34.579 | 35.065 | 36.456 | 641.266 | 1.00x |
| wide_arrays.json | orjson | 38.144 | 38.622 | 40.040 | 641.266 | 0.91x |
| wide_arrays.json | msgspec | 45.792 | 46.709 | 48.012 | 641.266 | 0.75x |
| wide_arrays.json | ujson | 59.288 | 59.905 | 61.425 | 641.266 | 0.59x |
| wide_arrays.json | json | 80.837 | 81.944 | 82.762 | 641.266 | 0.43x |
| mixed.json | strata | 1.369 | 1.396 | 1.431 | 776.016 | 1.00x |
| mixed.json | orjson | 2.372 | 2.411 | 2.703 | 776.016 | 0.58x |
| mixed.json | msgspec | 2.475 | 2.556 | 2.853 | 776.016 | 0.55x |
| mixed.json | ujson | 2.913 | 2.983 | 3.229 | 776.016 | 0.47x |
| mixed.json | json | 3.952 | 4.079 | 4.369 | 776.016 | 0.34x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 337.189 | 340.997 | 349.145 | 377.203 | 1.00x |
| users.ndjson | orjson | 527.928 | 532.681 | 537.520 | 377.203 | 0.64x |
| users.ndjson | msgspec | 529.952 | 535.579 | 556.829 | 377.203 | 0.64x |
| users.ndjson | ujson | 686.473 | 691.292 | 693.815 | 377.203 | 0.49x |
| users.ndjson | json | 802.510 | 804.609 | 809.533 | 377.203 | 0.42x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 84.296 | 84.685 | 86.775 | 1743.156 | 1.00x |
| users.json | orjson | 92.886 | 95.809 | 104.758 | 1743.156 | 0.88x |
| users.json | msgspec | 117.248 | 119.937 | 127.940 | 1743.156 | 0.71x |
| users.json | ujson | 441.372 | 448.319 | 457.054 | 1743.156 | 0.19x |
| users.json | json | 603.929 | 607.924 | 612.761 | 1743.156 | 0.14x |
| flat.json | strata | 2.101 | 2.160 | 2.281 | 352.062 | 1.00x |
| flat.json | orjson | 2.491 | 2.561 | 2.604 | 352.062 | 0.84x |
| flat.json | msgspec | 3.188 | 3.238 | 3.322 | 352.062 | 0.67x |
| flat.json | ujson | 9.203 | 9.321 | 9.501 | 352.062 | 0.23x |
| flat.json | json | 12.596 | 12.843 | 13.095 | 352.062 | 0.17x |
| nested.json | strata | 1.661 | 1.700 | 1.886 | 353.578 | 1.00x |
| nested.json | orjson | 2.323 | 2.364 | 2.434 | 353.578 | 0.72x |
| nested.json | msgspec | 2.892 | 2.950 | 3.039 | 353.578 | 0.58x |
| nested.json | ujson | 9.879 | 9.961 | 10.212 | 353.578 | 0.17x |
| nested.json | json | 15.131 | 15.276 | 15.709 | 353.578 | 0.11x |
| wide_arrays.json | strata | 13.831 | 14.493 | 14.917 | 774.547 | 1.00x |
| wide_arrays.json | orjson | 13.254 | 13.747 | 14.228 | 774.547 | 1.05x |
| wide_arrays.json | msgspec | 21.773 | 22.517 | 22.744 | 774.547 | 0.64x |
| wide_arrays.json | ujson | 65.576 | 66.094 | 67.027 | 774.547 | 0.22x |
| wide_arrays.json | json | 111.279 | 111.984 | 113.276 | 774.547 | 0.13x |
| mixed.json | strata | 0.490 | 0.520 | 0.562 | 776.016 | 1.00x |
| mixed.json | orjson | 0.550 | 0.567 | 0.624 | 776.016 | 0.92x |
| mixed.json | msgspec | 0.585 | 0.601 | 0.637 | 776.016 | 0.87x |
| mixed.json | ujson | 2.250 | 2.275 | 2.430 | 776.016 | 0.23x |
| mixed.json | json | 3.342 | 3.380 | 3.527 | 776.016 | 0.15x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.664 | 0.681 | 0.696 | 809.125 | 1.00x |
| users.json $[*].id | jmespath | 2.018 | 2.124 | 16.630 | 809.125 | 0.32x |
| users.json $[*].id | jsonpath-ng | 70.989 | 71.398 | 72.557 | 809.125 | 0.01x |
| users.json $[*].orders[*].total | strata | 13.105 | 13.343 | 17.671 | 603.594 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 35.262 | 36.359 | 40.925 | 603.594 | 0.37x |
| users.json $[*].orders[*].total | jsonpath-ng | 217.174 | 221.457 | 223.966 | 603.594 | 0.06x |
| users.json $..total | strata | 53.091 | 53.391 | 53.953 | 620.109 | 1.00x |
| users.json $..total | jsonpath-ng | 8565.397 | 8577.882 | 8586.133 | 620.109 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 133.866 | 134.493 | 139.956 | 823.750 | 1.00x |
| users.json $[*].id | orjson+jmespath | 450.888 | 452.092 | 456.801 | 823.750 | 0.30x |
| users.json $[*].id | orjson+jsonpath-ng | 454.332 | 456.566 | 459.277 | 823.750 | 0.29x |
| users.json $[*].orders[*].total | strata | 134.113 | 135.685 | 137.658 | 847.469 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 481.554 | 483.647 | 490.490 | 847.469 | 0.28x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 608.714 | 612.315 | 616.253 | 847.469 | 0.22x |
| users.json $..total | strata | 444.598 | 448.574 | 450.488 | 858.531 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 8938.623 | 8945.467 | 8958.281 | 858.531 | 0.05x |

