# Benchmark results - medium

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
| users.json | strata | 42.311 | 44.456 | 47.356 | 139.578 | 1.00x |
| users.json | orjson | 50.943 | 54.634 | 56.834 | 139.578 | 0.81x |
| users.json | msgspec | 52.779 | 55.984 | 58.863 | 139.578 | 0.79x |
| users.json | ujson | 72.367 | 76.741 | 79.470 | 139.578 | 0.58x |
| users.json | json | 91.580 | 95.696 | 97.658 | 139.578 | 0.46x |
| flat.json | strata | 2.596 | 2.656 | 2.798 | 164.984 | 1.00x |
| flat.json | orjson | 3.620 | 3.730 | 3.780 | 164.984 | 0.71x |
| flat.json | msgspec | 3.686 | 3.799 | 3.930 | 164.984 | 0.70x |
| flat.json | ujson | 5.167 | 5.261 | 5.395 | 164.984 | 0.50x |
| flat.json | json | 6.712 | 6.953 | 7.037 | 164.984 | 0.38x |
| nested.json | strata | 2.182 | 2.215 | 2.340 | 166.609 | 1.00x |
| nested.json | orjson | 3.054 | 3.131 | 3.448 | 166.609 | 0.71x |
| nested.json | msgspec | 3.058 | 3.108 | 3.473 | 166.609 | 0.71x |
| nested.json | ujson | 4.165 | 4.347 | 4.504 | 166.609 | 0.51x |
| nested.json | json | 5.985 | 6.090 | 6.442 | 166.609 | 0.36x |
| wide_arrays.json | strata | 13.278 | 13.423 | 13.779 | 193.984 | 1.00x |
| wide_arrays.json | orjson | 13.336 | 13.610 | 14.078 | 193.984 | 0.99x |
| wide_arrays.json | msgspec | 16.345 | 16.585 | 17.033 | 193.984 | 0.81x |
| wide_arrays.json | ujson | 21.157 | 21.518 | 21.966 | 193.984 | 0.62x |
| wide_arrays.json | json | 30.504 | 30.873 | 31.406 | 193.984 | 0.43x |
| mixed.json | strata | 0.520 | 0.528 | 0.547 | 166.016 | 1.00x |
| mixed.json | orjson | 1.187 | 1.203 | 1.282 | 166.016 | 0.44x |
| mixed.json | msgspec | 1.236 | 1.268 | 1.294 | 166.016 | 0.42x |
| mixed.json | ujson | 1.405 | 1.410 | 1.599 | 166.016 | 0.37x |
| mixed.json | json | 1.866 | 1.881 | 2.039 | 166.016 | 0.28x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.134 | 9.799 | 10.194 | 602.078 | 1.00x |
| users.json | orjson | 12.351 | 12.761 | 13.451 | 602.078 | 0.77x |
| users.json | msgspec | 15.981 | 16.421 | 16.802 | 602.078 | 0.60x |
| users.json | ujson | 64.925 | 65.561 | 66.686 | 602.078 | 0.15x |
| users.json | json | 89.275 | 90.110 | 90.957 | 602.078 | 0.11x |
| flat.json | strata | 0.702 | 0.719 | 0.775 | 164.984 | 1.00x |
| flat.json | orjson | 0.857 | 0.892 | 0.927 | 164.984 | 0.81x |
| flat.json | msgspec | 1.152 | 1.167 | 1.203 | 164.984 | 0.62x |
| flat.json | ujson | 3.444 | 3.502 | 3.651 | 164.984 | 0.21x |
| flat.json | json | 4.970 | 5.067 | 5.248 | 164.984 | 0.14x |
| nested.json | strata | 0.485 | 0.496 | 0.667 | 166.625 | 1.00x |
| nested.json | orjson | 0.785 | 0.802 | 0.836 | 166.625 | 0.62x |
| nested.json | msgspec | 1.041 | 1.061 | 1.336 | 166.625 | 0.47x |
| nested.json | ujson | 3.817 | 3.851 | 3.976 | 166.625 | 0.13x |
| nested.json | json | 6.081 | 6.211 | 6.252 | 166.625 | 0.08x |
| wide_arrays.json | strata | 4.614 | 5.027 | 5.811 | 150.016 | 1.00x |
| wide_arrays.json | orjson | 5.064 | 5.259 | 5.525 | 150.016 | 0.96x |
| wide_arrays.json | msgspec | 8.361 | 8.594 | 8.977 | 150.016 | 0.58x |
| wide_arrays.json | ujson | 24.740 | 24.931 | 26.491 | 150.016 | 0.20x |
| wide_arrays.json | json | 44.054 | 44.272 | 47.085 | 150.016 | 0.11x |
| mixed.json | strata | 0.125 | 0.132 | 0.146 | 166.031 | 1.00x |
| mixed.json | orjson | 0.153 | 0.157 | 0.218 | 166.031 | 0.84x |
| mixed.json | msgspec | 0.170 | 0.174 | 0.182 | 166.031 | 0.76x |
| mixed.json | ujson | 0.827 | 0.838 | 0.859 | 166.031 | 0.16x |
| mixed.json | json | 1.283 | 1.314 | 1.452 | 166.031 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 44.206 | 46.788 | 47.844 | 657.172 | 1.00x |
| users.json | orjson | 66.916 | 68.709 | 73.162 | 657.172 | 0.68x |
| users.json | msgspec | 67.462 | 70.649 | 74.260 | 657.172 | 0.66x |
| users.json | ujson | 87.836 | 91.929 | 93.994 | 657.172 | 0.51x |
| users.json | json | 107.637 | 109.084 | 112.450 | 657.172 | 0.43x |
| flat.json | strata | 2.772 | 2.793 | 2.990 | 166.609 | 1.00x |
| flat.json | orjson | 3.970 | 4.031 | 4.436 | 166.609 | 0.69x |
| flat.json | msgspec | 4.063 | 4.110 | 4.387 | 166.609 | 0.68x |
| flat.json | ujson | 5.587 | 5.688 | 5.910 | 166.609 | 0.49x |
| flat.json | json | 7.079 | 7.127 | 7.546 | 166.609 | 0.39x |
| nested.json | strata | 2.323 | 2.341 | 2.439 | 166.625 | 1.00x |
| nested.json | orjson | 3.552 | 3.588 | 3.693 | 166.625 | 0.65x |
| nested.json | msgspec | 3.555 | 3.608 | 3.744 | 166.625 | 0.65x |
| nested.json | ujson | 4.712 | 4.835 | 6.241 | 166.625 | 0.48x |
| nested.json | json | 6.473 | 6.599 | 6.817 | 166.625 | 0.35x |
| wide_arrays.json | strata | 13.859 | 14.087 | 14.458 | 158.969 | 1.00x |
| wide_arrays.json | orjson | 15.597 | 15.915 | 16.593 | 158.969 | 0.89x |
| wide_arrays.json | msgspec | 18.647 | 19.183 | 19.554 | 158.969 | 0.73x |
| wide_arrays.json | ujson | 24.118 | 24.653 | 25.106 | 158.969 | 0.57x |
| wide_arrays.json | json | 33.314 | 33.552 | 34.523 | 158.969 | 0.42x |
| mixed.json | strata | 0.616 | 0.641 | 0.671 | 166.031 | 1.00x |
| mixed.json | orjson | 1.382 | 1.411 | 1.458 | 166.031 | 0.45x |
| mixed.json | msgspec | 1.431 | 1.474 | 1.527 | 166.031 | 0.44x |
| mixed.json | ujson | 1.652 | 1.725 | 1.924 | 166.031 | 0.37x |
| mixed.json | json | 2.065 | 2.131 | 2.184 | 166.031 | 0.30x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 43.378 | 44.170 | 46.028 | 168.750 | 1.00x |
| users.ndjson | orjson | 65.046 | 66.978 | 70.230 | 168.750 | 0.66x |
| users.ndjson | msgspec | 64.123 | 65.647 | 69.561 | 168.750 | 0.67x |
| users.ndjson | ujson | 81.928 | 84.432 | 88.359 | 168.750 | 0.52x |
| users.ndjson | json | 106.480 | 108.413 | 111.084 | 168.750 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.359 | 12.204 | 13.337 | 633.672 | 1.00x |
| users.json | orjson | 15.048 | 15.236 | 16.746 | 633.672 | 0.80x |
| users.json | msgspec | 18.367 | 18.717 | 19.724 | 633.672 | 0.65x |
| users.json | ujson | 64.403 | 66.742 | 68.563 | 633.672 | 0.18x |
| users.json | json | 89.606 | 92.051 | 96.502 | 633.672 | 0.13x |
| flat.json | strata | 0.950 | 0.971 | 1.070 | 166.609 | 1.00x |
| flat.json | orjson | 1.106 | 1.164 | 1.308 | 166.609 | 0.83x |
| flat.json | msgspec | 1.416 | 1.453 | 1.573 | 166.609 | 0.67x |
| flat.json | ujson | 3.921 | 4.009 | 4.153 | 166.609 | 0.24x |
| flat.json | json | 5.267 | 5.436 | 5.582 | 166.609 | 0.18x |
| nested.json | strata | 0.686 | 0.758 | 0.863 | 166.734 | 1.00x |
| nested.json | orjson | 1.016 | 1.130 | 1.222 | 166.734 | 0.67x |
| nested.json | msgspec | 1.277 | 1.351 | 1.633 | 166.734 | 0.56x |
| nested.json | ujson | 4.097 | 4.199 | 4.500 | 166.734 | 0.18x |
| nested.json | json | 6.405 | 6.561 | 7.032 | 166.734 | 0.12x |
| wide_arrays.json | strata | 5.573 | 5.926 | 6.417 | 166.016 | 1.00x |
| wide_arrays.json | orjson | 5.884 | 6.020 | 6.545 | 166.016 | 0.98x |
| wide_arrays.json | msgspec | 9.269 | 9.515 | 9.955 | 166.016 | 0.62x |
| wide_arrays.json | ujson | 25.686 | 26.140 | 26.610 | 166.016 | 0.23x |
| wide_arrays.json | json | 45.246 | 45.774 | 46.464 | 166.016 | 0.13x |
| mixed.json | strata | 0.245 | 0.289 | 0.380 | 166.031 | 1.00x |
| mixed.json | orjson | 0.307 | 0.325 | 0.441 | 166.031 | 0.89x |
| mixed.json | msgspec | 0.292 | 0.340 | 0.547 | 166.031 | 0.85x |
| mixed.json | ujson | 0.984 | 1.052 | 1.148 | 166.031 | 0.27x |
| mixed.json | json | 1.408 | 1.484 | 1.707 | 166.031 | 0.19x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.178 | 0.185 | 0.274 | 634.125 | 1.00x |
| users.json $[*].id | jmespath | 0.799 | 0.844 | 0.932 | 634.125 | 0.22x |
| users.json $[*].id | jsonpath-ng | 12.679 | 12.938 | 13.942 | 634.125 | 0.01x |
| users.json $[*].orders[*].total | strata | 2.508 | 2.620 | 3.193 | 683.500 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 8.223 | 8.355 | 8.489 | 683.500 | 0.31x |
| users.json $[*].orders[*].total | jsonpath-ng | 44.854 | 46.213 | 50.080 | 683.500 | 0.06x |
| users.json $..total | strata | 7.871 | 8.144 | 8.608 | 151.094 | 1.00x |
| users.json $..total | jsonpath-ng | 1261.889 | 1270.496 | 1278.320 | 151.094 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 18.828 | 19.288 | 20.074 | 702.297 | 1.00x |
| users.json $[*].id | orjson+jmespath | 66.648 | 68.844 | 69.523 | 702.297 | 0.28x |
| users.json $[*].id | orjson+jsonpath-ng | 69.581 | 70.677 | 74.565 | 702.297 | 0.27x |
| users.json $[*].orders[*].total | strata | 19.332 | 19.598 | 23.438 | 484.906 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 73.196 | 75.822 | 79.034 | 484.906 | 0.26x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 102.478 | 106.635 | 110.550 | 484.906 | 0.18x |
| users.json $..total | strata | 62.413 | 62.836 | 66.006 | 202.828 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 1315.290 | 1324.259 | 1339.086 | 202.828 | 0.05x |

