# Benchmark results - medium

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 67f0cbd
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
| users.json | strata | 60.964 | 62.528 | 65.206 | 139.156 | 1.00x |
| users.json | orjson | 63.582 | 65.837 | 69.065 | 139.156 | 0.95x |
| users.json | msgspec | 65.609 | 68.255 | 70.302 | 139.156 | 0.92x |
| users.json | ujson | 89.751 | 93.488 | 95.134 | 139.156 | 0.67x |
| users.json | json | 113.219 | 116.266 | 118.073 | 139.156 | 0.54x |
| flat.json | strata | 3.941 | 3.960 | 4.219 | 167.672 | 1.00x |
| flat.json | orjson | 4.608 | 4.824 | 5.125 | 167.672 | 0.82x |
| flat.json | msgspec | 4.662 | 4.697 | 5.043 | 167.672 | 0.84x |
| flat.json | ujson | 6.423 | 6.687 | 7.075 | 167.672 | 0.59x |
| flat.json | json | 8.579 | 8.709 | 9.389 | 167.672 | 0.45x |
| nested.json | strata | 3.142 | 3.164 | 3.319 | 169.359 | 1.00x |
| nested.json | orjson | 3.910 | 3.954 | 4.249 | 169.359 | 0.80x |
| nested.json | msgspec | 3.926 | 3.970 | 4.048 | 169.359 | 0.80x |
| nested.json | ujson | 5.295 | 5.452 | 6.206 | 169.359 | 0.58x |
| nested.json | json | 7.651 | 7.774 | 8.499 | 169.359 | 0.41x |
| wide_arrays.json | strata | 18.919 | 19.135 | 19.438 | 196.641 | 1.00x |
| wide_arrays.json | orjson | 17.033 | 17.583 | 18.178 | 196.641 | 1.09x |
| wide_arrays.json | msgspec | 20.799 | 21.458 | 22.581 | 196.641 | 0.89x |
| wide_arrays.json | ujson | 27.421 | 27.822 | 28.404 | 196.641 | 0.69x |
| wide_arrays.json | json | 39.180 | 39.703 | 40.419 | 196.641 | 0.48x |
| mixed.json | strata | 0.819 | 0.823 | 0.862 | 177.578 | 1.00x |
| mixed.json | orjson | 1.504 | 1.517 | 1.554 | 177.578 | 0.54x |
| mixed.json | msgspec | 1.564 | 1.582 | 1.635 | 177.578 | 0.52x |
| mixed.json | ujson | 1.781 | 1.829 | 1.884 | 177.578 | 0.45x |
| mixed.json | json | 2.387 | 2.419 | 2.525 | 177.578 | 0.34x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 16.158 | 16.324 | 17.101 | 601.719 | 1.00x |
| users.json | orjson | 15.748 | 15.955 | 18.137 | 601.719 | 1.02x |
| users.json | msgspec | 20.768 | 20.932 | 21.481 | 601.719 | 0.78x |
| users.json | ujson | 84.008 | 84.484 | 85.669 | 601.719 | 0.19x |
| users.json | json | 115.707 | 116.225 | 116.472 | 601.719 | 0.14x |
| flat.json | strata | 1.172 | 1.183 | 1.208 | 167.734 | 1.00x |
| flat.json | orjson | 1.134 | 1.139 | 1.152 | 167.734 | 1.04x |
| flat.json | msgspec | 1.497 | 1.517 | 1.539 | 167.734 | 0.78x |
| flat.json | ujson | 4.639 | 4.658 | 4.714 | 167.734 | 0.25x |
| flat.json | json | 6.413 | 6.427 | 6.557 | 167.734 | 0.18x |
| nested.json | strata | 0.902 | 0.914 | 1.051 | 169.359 | 1.00x |
| nested.json | orjson | 1.035 | 1.039 | 1.132 | 169.359 | 0.88x |
| nested.json | msgspec | 1.376 | 1.406 | 1.517 | 169.359 | 0.65x |
| nested.json | ujson | 4.995 | 5.046 | 5.557 | 169.359 | 0.18x |
| nested.json | json | 8.190 | 8.252 | 8.961 | 169.359 | 0.11x |
| wide_arrays.json | strata | 7.747 | 7.933 | 8.459 | 192.547 | 1.00x |
| wide_arrays.json | orjson | 6.428 | 6.550 | 7.041 | 192.547 | 1.21x |
| wide_arrays.json | msgspec | 10.704 | 10.787 | 11.497 | 192.547 | 0.74x |
| wide_arrays.json | ujson | 31.883 | 32.182 | 32.604 | 192.547 | 0.25x |
| wide_arrays.json | json | 56.987 | 57.229 | 57.737 | 192.547 | 0.14x |
| mixed.json | strata | 0.226 | 0.233 | 0.244 | 177.594 | 1.00x |
| mixed.json | orjson | 0.200 | 0.205 | 0.227 | 177.594 | 1.13x |
| mixed.json | msgspec | 0.225 | 0.229 | 0.247 | 177.594 | 1.02x |
| mixed.json | ujson | 1.091 | 1.099 | 1.176 | 177.594 | 0.21x |
| mixed.json | json | 1.695 | 1.715 | 1.781 | 177.594 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 61.731 | 63.107 | 65.663 | 656.844 | 1.00x |
| users.json | orjson | 78.993 | 80.997 | 83.930 | 656.844 | 0.78x |
| users.json | msgspec | 80.075 | 82.878 | 85.816 | 656.844 | 0.76x |
| users.json | ujson | 106.102 | 108.929 | 113.411 | 656.844 | 0.58x |
| users.json | json | 128.414 | 131.042 | 133.147 | 656.844 | 0.48x |
| flat.json | strata | 4.111 | 4.157 | 4.318 | 169.359 | 1.00x |
| flat.json | orjson | 5.126 | 5.215 | 5.633 | 169.359 | 0.80x |
| flat.json | msgspec | 5.200 | 5.267 | 5.394 | 169.359 | 0.79x |
| flat.json | ujson | 7.025 | 7.139 | 7.516 | 169.359 | 0.58x |
| flat.json | json | 9.041 | 9.120 | 9.728 | 169.359 | 0.46x |
| nested.json | strata | 3.279 | 3.308 | 3.385 | 169.359 | 1.00x |
| nested.json | orjson | 4.415 | 4.478 | 5.175 | 169.359 | 0.74x |
| nested.json | msgspec | 4.468 | 4.508 | 4.853 | 169.359 | 0.73x |
| nested.json | ujson | 5.940 | 6.009 | 6.564 | 169.359 | 0.55x |
| nested.json | json | 8.165 | 8.227 | 8.480 | 169.359 | 0.40x |
| wide_arrays.json | strata | 19.454 | 19.640 | 19.995 | 170.531 | 1.00x |
| wide_arrays.json | orjson | 19.443 | 19.712 | 20.202 | 170.531 | 1.00x |
| wide_arrays.json | msgspec | 23.352 | 23.674 | 24.825 | 170.531 | 0.83x |
| wide_arrays.json | ujson | 30.591 | 31.159 | 31.734 | 170.531 | 0.63x |
| wide_arrays.json | json | 41.862 | 42.239 | 42.427 | 170.531 | 0.46x |
| mixed.json | strata | 0.897 | 0.923 | 0.980 | 177.594 | 1.00x |
| mixed.json | orjson | 1.727 | 1.815 | 1.962 | 177.594 | 0.51x |
| mixed.json | msgspec | 1.784 | 1.859 | 2.106 | 177.594 | 0.50x |
| mixed.json | ujson | 2.028 | 2.087 | 2.170 | 177.594 | 0.44x |
| mixed.json | json | 2.600 | 2.664 | 3.062 | 177.594 | 0.35x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 57.058 | 57.433 | 58.006 | 171.438 | 1.00x |
| users.ndjson | orjson | 80.172 | 81.646 | 84.127 | 171.438 | 0.70x |
| users.ndjson | msgspec | 79.723 | 81.450 | 82.581 | 171.438 | 0.71x |
| users.ndjson | ujson | 102.341 | 104.488 | 106.035 | 171.438 | 0.55x |
| users.ndjson | json | 132.891 | 133.997 | 136.922 | 171.438 | 0.43x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 18.091 | 18.924 | 21.331 | 574.406 | 1.00x |
| users.json | orjson | 17.786 | 19.185 | 20.800 | 574.406 | 0.99x |
| users.json | msgspec | 22.747 | 24.273 | 25.928 | 574.406 | 0.78x |
| users.json | ujson | 84.298 | 85.701 | 87.462 | 574.406 | 0.22x |
| users.json | json | 115.572 | 116.899 | 119.433 | 574.406 | 0.16x |
| flat.json | strata | 1.441 | 1.497 | 1.628 | 169.359 | 1.00x |
| flat.json | orjson | 1.392 | 1.442 | 1.557 | 169.359 | 1.04x |
| flat.json | msgspec | 1.754 | 1.801 | 1.953 | 169.359 | 0.83x |
| flat.json | ujson | 4.763 | 4.825 | 5.027 | 169.359 | 0.31x |
| flat.json | json | 6.698 | 6.819 | 7.074 | 169.359 | 0.22x |
| nested.json | strata | 1.165 | 1.192 | 1.352 | 169.391 | 1.00x |
| nested.json | orjson | 1.253 | 1.288 | 1.349 | 169.391 | 0.93x |
| nested.json | msgspec | 1.592 | 1.640 | 1.769 | 169.391 | 0.73x |
| nested.json | ujson | 5.235 | 5.311 | 5.394 | 169.391 | 0.22x |
| nested.json | json | 8.245 | 8.475 | 8.620 | 169.391 | 0.14x |
| wide_arrays.json | strata | 8.760 | 9.109 | 9.704 | 177.578 | 1.00x |
| wide_arrays.json | orjson | 7.355 | 7.483 | 7.690 | 177.578 | 1.22x |
| wide_arrays.json | msgspec | 11.681 | 11.818 | 11.921 | 177.578 | 0.77x |
| wide_arrays.json | ujson | 32.983 | 33.510 | 33.935 | 177.578 | 0.27x |
| wide_arrays.json | json | 58.107 | 58.852 | 59.010 | 177.578 | 0.15x |
| mixed.json | strata | 0.405 | 0.431 | 0.440 | 177.594 | 1.00x |
| mixed.json | orjson | 0.334 | 0.352 | 0.407 | 177.594 | 1.22x |
| mixed.json | msgspec | 0.358 | 0.369 | 0.400 | 177.594 | 1.17x |
| mixed.json | ujson | 1.243 | 1.281 | 1.340 | 177.594 | 0.34x |
| mixed.json | json | 1.834 | 1.866 | 1.953 | 177.594 | 0.23x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.205 | 0.222 | 0.288 | 574.922 | 1.00x |
| users.json $[*].id | jmespath | 0.996 | 1.015 | 1.163 | 574.922 | 0.22x |
| users.json $[*].id | jsonpath-ng | 14.937 | 15.161 | 15.731 | 574.922 | 0.01x |
| users.json $[*].orders[*].total | strata | 2.658 | 2.738 | 3.008 | 452.234 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 10.318 | 10.394 | 13.198 | 452.234 | 0.26x |
| users.json $[*].orders[*].total | jsonpath-ng | 56.375 | 58.625 | 61.712 | 452.234 | 0.05x |
| users.json $..total | strata | 10.157 | 10.228 | 10.624 | 151.766 | 1.00x |
| users.json $..total | jsonpath-ng | 1622.380 | 1623.915 | 1628.776 | 151.766 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 28.307 | 28.763 | 29.247 | 634.516 | 1.00x |
| users.json $[*].id | orjson+jmespath | 79.949 | 81.543 | 82.895 | 634.516 | 0.35x |
| users.json $[*].id | orjson+jsonpath-ng | 82.531 | 84.303 | 86.638 | 634.516 | 0.34x |
| users.json $[*].orders[*].total | strata | 29.042 | 29.373 | 29.545 | 516.391 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 87.184 | 89.032 | 90.265 | 516.391 | 0.33x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 125.468 | 127.071 | 130.374 | 516.391 | 0.23x |
| users.json $..total | strata | 82.967 | 83.779 | 84.372 | 202.547 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 1690.887 | 1692.182 | 1693.803 | 202.547 | 0.05x |

