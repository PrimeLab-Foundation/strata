# Benchmark results - large

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 5292dd0
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
| users.json | strata | 363.104 | 366.890 | 369.348 | 279.594 | 1.00x |
| users.json | orjson | 387.772 | 390.790 | 393.462 | 279.594 | 0.94x |
| users.json | msgspec | 400.443 | 402.560 | 406.096 | 279.594 | 0.91x |
| users.json | ujson | 537.007 | 542.831 | 548.026 | 279.594 | 0.68x |
| users.json | json | 657.495 | 661.734 | 665.441 | 279.594 | 0.55x |
| flat.json | strata | 7.163 | 7.320 | 7.504 | 351.781 | 1.00x |
| flat.json | orjson | 7.739 | 7.900 | 8.073 | 351.781 | 0.93x |
| flat.json | msgspec | 7.855 | 8.011 | 8.238 | 351.781 | 0.91x |
| flat.json | ujson | 11.034 | 11.319 | 11.873 | 351.781 | 0.65x |
| flat.json | json | 14.668 | 14.884 | 15.047 | 351.781 | 0.49x |
| nested.json | strata | 6.041 | 6.151 | 6.204 | 354.062 | 1.00x |
| nested.json | orjson | 6.706 | 6.807 | 6.857 | 354.062 | 0.90x |
| nested.json | msgspec | 6.753 | 6.850 | 6.954 | 354.062 | 0.90x |
| nested.json | ujson | 9.428 | 9.565 | 9.650 | 354.062 | 0.64x |
| nested.json | json | 13.878 | 14.046 | 14.224 | 354.062 | 0.44x |
| wide_arrays.json | strata | 36.510 | 37.104 | 37.329 | 423.594 | 1.00x |
| wide_arrays.json | orjson | 32.793 | 33.073 | 33.333 | 423.594 | 1.12x |
| wide_arrays.json | msgspec | 40.844 | 41.016 | 41.498 | 423.594 | 0.90x |
| wide_arrays.json | ujson | 52.826 | 53.282 | 54.058 | 423.594 | 0.70x |
| wide_arrays.json | json | 75.955 | 76.672 | 77.157 | 423.594 | 0.48x |
| mixed.json | strata | 1.560 | 1.568 | 1.591 | 775.672 | 1.00x |
| mixed.json | orjson | 2.043 | 2.069 | 2.115 | 775.672 | 0.76x |
| mixed.json | msgspec | 2.151 | 2.167 | 2.229 | 775.672 | 0.72x |
| mixed.json | ujson | 2.549 | 2.565 | 2.604 | 775.672 | 0.61x |
| mixed.json | json | 3.654 | 3.720 | 3.829 | 775.672 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 83.678 | 84.547 | 86.202 | 1540.156 | 1.00x |
| users.json | orjson | 82.506 | 84.127 | 90.044 | 1540.156 | 1.00x |
| users.json | msgspec | 109.953 | 116.274 | 121.382 | 1540.156 | 0.73x |
| users.json | ujson | 444.411 | 447.205 | 452.711 | 1540.156 | 0.19x |
| users.json | json | 601.101 | 605.439 | 617.036 | 1540.156 | 0.14x |
| flat.json | strata | 2.022 | 2.063 | 2.116 | 353.734 | 1.00x |
| flat.json | orjson | 2.117 | 2.141 | 2.176 | 353.734 | 0.96x |
| flat.json | msgspec | 2.801 | 2.868 | 2.993 | 353.734 | 0.72x |
| flat.json | ujson | 8.755 | 8.916 | 9.106 | 353.734 | 0.23x |
| flat.json | json | 12.146 | 12.388 | 12.549 | 353.734 | 0.17x |
| nested.json | strata | 1.539 | 1.566 | 1.624 | 354.109 | 1.00x |
| nested.json | orjson | 1.972 | 2.011 | 2.041 | 354.109 | 0.78x |
| nested.json | msgspec | 2.591 | 2.629 | 2.718 | 354.109 | 0.60x |
| nested.json | ujson | 9.394 | 9.464 | 9.603 | 354.109 | 0.17x |
| nested.json | json | 14.799 | 14.958 | 15.539 | 354.109 | 0.10x |
| wide_arrays.json | strata | 12.509 | 12.647 | 12.860 | 628.250 | 1.00x |
| wide_arrays.json | orjson | 11.504 | 11.708 | 12.171 | 628.250 | 1.08x |
| wide_arrays.json | msgspec | 19.938 | 20.102 | 20.501 | 628.250 | 0.63x |
| wide_arrays.json | ujson | 62.736 | 63.341 | 63.975 | 628.250 | 0.20x |
| wide_arrays.json | json | 108.539 | 109.743 | 110.274 | 628.250 | 0.12x |
| mixed.json | strata | 0.386 | 0.389 | 0.404 | 775.688 | 1.00x |
| mixed.json | orjson | 0.380 | 0.383 | 0.388 | 775.688 | 1.01x |
| mixed.json | msgspec | 0.418 | 0.419 | 0.421 | 775.688 | 0.93x |
| mixed.json | ujson | 2.084 | 2.096 | 2.102 | 775.688 | 0.19x |
| mixed.json | json | 3.199 | 3.211 | 3.248 | 775.688 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 372.224 | 380.234 | 384.974 | 902.125 | 1.00x |
| users.json | orjson | 464.696 | 471.406 | 495.494 | 902.125 | 0.81x |
| users.json | msgspec | 476.614 | 480.520 | 485.751 | 902.125 | 0.79x |
| users.json | ujson | 624.883 | 630.102 | 640.681 | 902.125 | 0.60x |
| users.json | json | 734.792 | 741.507 | 761.527 | 902.125 | 0.51x |
| flat.json | strata | 7.693 | 7.780 | 7.926 | 354.047 | 1.00x |
| flat.json | orjson | 8.679 | 8.825 | 9.300 | 354.047 | 0.88x |
| flat.json | msgspec | 8.958 | 9.027 | 9.405 | 354.047 | 0.86x |
| flat.json | ujson | 12.355 | 12.493 | 12.636 | 354.047 | 0.62x |
| flat.json | json | 15.869 | 15.978 | 16.471 | 354.047 | 0.49x |
| nested.json | strata | 6.357 | 6.438 | 6.486 | 355.578 | 1.00x |
| nested.json | orjson | 8.327 | 8.497 | 8.718 | 355.578 | 0.76x |
| nested.json | msgspec | 8.421 | 8.575 | 9.108 | 355.578 | 0.75x |
| nested.json | ujson | 11.396 | 11.512 | 11.932 | 355.578 | 0.56x |
| nested.json | json | 15.591 | 15.695 | 16.013 | 355.578 | 0.41x |
| wide_arrays.json | strata | 38.215 | 38.597 | 39.193 | 641.250 | 1.00x |
| wide_arrays.json | orjson | 38.352 | 38.824 | 39.660 | 641.250 | 0.99x |
| wide_arrays.json | msgspec | 46.253 | 46.961 | 48.012 | 641.250 | 0.82x |
| wide_arrays.json | ujson | 59.854 | 60.437 | 60.988 | 641.250 | 0.64x |
| wide_arrays.json | json | 81.348 | 82.824 | 83.312 | 641.250 | 0.47x |
| mixed.json | strata | 1.704 | 1.724 | 1.747 | 775.688 | 1.00x |
| mixed.json | orjson | 2.388 | 2.404 | 2.480 | 775.688 | 0.72x |
| mixed.json | msgspec | 2.500 | 2.520 | 2.563 | 775.688 | 0.68x |
| mixed.json | ujson | 2.956 | 2.983 | 3.124 | 775.688 | 0.58x |
| mixed.json | json | 4.044 | 4.076 | 4.247 | 775.688 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 339.932 | 342.340 | 345.624 | 379.156 | 1.00x |
| users.ndjson | orjson | 517.366 | 522.948 | 528.673 | 379.156 | 0.65x |
| users.ndjson | msgspec | 524.316 | 529.761 | 546.824 | 379.156 | 0.65x |
| users.ndjson | ujson | 679.719 | 683.470 | 692.463 | 379.156 | 0.50x |
| users.ndjson | json | 793.480 | 796.407 | 799.571 | 379.156 | 0.43x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 95.434 | 96.093 | 105.877 | 1514.062 | 1.00x |
| users.json | orjson | 93.629 | 96.629 | 112.745 | 1514.062 | 0.99x |
| users.json | msgspec | 120.380 | 123.926 | 128.926 | 1514.062 | 0.78x |
| users.json | ujson | 449.738 | 455.710 | 459.736 | 1514.062 | 0.21x |
| users.json | json | 610.804 | 615.410 | 622.523 | 1514.062 | 0.16x |
| flat.json | strata | 2.436 | 2.474 | 2.532 | 354.047 | 1.00x |
| flat.json | orjson | 2.531 | 2.559 | 2.660 | 354.047 | 0.97x |
| flat.json | msgspec | 3.237 | 3.297 | 3.346 | 354.047 | 0.75x |
| flat.json | ujson | 9.387 | 9.447 | 9.474 | 354.047 | 0.26x |
| flat.json | json | 12.820 | 12.885 | 12.978 | 354.047 | 0.19x |
| nested.json | strata | 1.938 | 1.959 | 2.012 | 355.578 | 1.00x |
| nested.json | orjson | 2.370 | 2.407 | 2.438 | 355.578 | 0.81x |
| nested.json | msgspec | 2.998 | 3.021 | 3.115 | 355.578 | 0.65x |
| nested.json | ujson | 10.015 | 10.071 | 10.122 | 355.578 | 0.19x |
| nested.json | json | 15.335 | 15.470 | 15.599 | 355.578 | 0.13x |
| wide_arrays.json | strata | 14.189 | 14.351 | 14.584 | 774.531 | 1.00x |
| wide_arrays.json | orjson | 13.225 | 13.395 | 13.522 | 774.531 | 1.07x |
| wide_arrays.json | msgspec | 21.822 | 22.101 | 22.266 | 774.531 | 0.65x |
| wide_arrays.json | ujson | 65.281 | 65.496 | 66.166 | 774.531 | 0.22x |
| wide_arrays.json | json | 110.376 | 111.093 | 111.810 | 774.531 | 0.13x |
| mixed.json | strata | 0.566 | 0.594 | 0.622 | 775.688 | 1.00x |
| mixed.json | orjson | 0.542 | 0.553 | 0.563 | 775.688 | 1.07x |
| mixed.json | msgspec | 0.578 | 0.597 | 0.612 | 775.688 | 0.99x |
| mixed.json | ujson | 2.269 | 2.312 | 2.363 | 775.688 | 0.26x |
| mixed.json | json | 3.396 | 3.429 | 3.495 | 775.688 | 0.17x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.675 | 0.687 | 0.709 | 963.703 | 1.00x |
| users.json $[*].id | jmespath | 2.016 | 2.035 | 2.102 | 963.703 | 0.34x |
| users.json $[*].id | jsonpath-ng | 70.966 | 71.379 | 80.525 | 963.703 | 0.01x |
| users.json $[*].orders[*].total | strata | 13.020 | 13.156 | 13.905 | 817.969 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 35.014 | 35.484 | 37.516 | 817.969 | 0.37x |
| users.json $[*].orders[*].total | jsonpath-ng | 217.824 | 220.825 | 224.939 | 817.969 | 0.06x |
| users.json $..total | strata | 53.105 | 53.335 | 53.709 | 621.203 | 1.00x |
| users.json $..total | jsonpath-ng | 8596.585 | 8607.158 | 8620.592 | 621.203 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 154.369 | 156.355 | 157.149 | 823.734 | 1.00x |
| users.json $[*].id | orjson+jmespath | 453.228 | 455.630 | 457.460 | 823.734 | 0.34x |
| users.json $[*].id | orjson+jsonpath-ng | 458.405 | 460.049 | 463.758 | 823.734 | 0.34x |
| users.json $[*].orders[*].total | strata | 157.312 | 157.892 | 158.413 | 846.547 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 484.829 | 486.440 | 488.573 | 846.547 | 0.32x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 607.935 | 614.207 | 622.855 | 846.547 | 0.26x |
| users.json $..total | strata | 480.548 | 482.148 | 491.664 | 859.641 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 8999.230 | 9023.328 | 9119.397 | 859.641 | 0.05x |

