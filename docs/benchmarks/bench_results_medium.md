# Benchmark results - medium

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: e0e780e
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
| users.json | strata | 42.741 | 45.521 | 48.191 | 139.391 | 1.00x |
| users.json | orjson | 50.636 | 52.978 | 55.560 | 139.391 | 0.86x |
| users.json | msgspec | 51.534 | 54.279 | 57.880 | 139.391 | 0.84x |
| users.json | ujson | 72.965 | 74.660 | 78.570 | 139.391 | 0.61x |
| users.json | json | 88.977 | 91.397 | 94.583 | 139.391 | 0.50x |
| flat.json | strata | 2.637 | 2.724 | 2.855 | 163.953 | 1.00x |
| flat.json | orjson | 3.599 | 3.970 | 4.243 | 163.953 | 0.69x |
| flat.json | msgspec | 3.557 | 3.712 | 4.060 | 163.953 | 0.73x |
| flat.json | ujson | 4.997 | 5.238 | 5.605 | 163.953 | 0.52x |
| flat.json | json | 6.483 | 6.868 | 7.171 | 163.953 | 0.40x |
| nested.json | strata | 2.262 | 2.345 | 2.571 | 166.062 | 1.00x |
| nested.json | orjson | 3.086 | 3.521 | 3.697 | 166.062 | 0.67x |
| nested.json | msgspec | 3.081 | 3.582 | 3.638 | 166.062 | 0.65x |
| nested.json | ujson | 4.165 | 4.672 | 5.018 | 166.062 | 0.50x |
| nested.json | json | 6.085 | 6.660 | 7.137 | 166.062 | 0.35x |
| wide_arrays.json | strata | 13.336 | 14.008 | 14.321 | 193.422 | 1.00x |
| wide_arrays.json | orjson | 13.935 | 14.590 | 15.181 | 193.422 | 0.96x |
| wide_arrays.json | msgspec | 16.488 | 17.706 | 18.116 | 193.422 | 0.79x |
| wide_arrays.json | ujson | 21.309 | 21.725 | 23.283 | 193.422 | 0.64x |
| wide_arrays.json | json | 30.839 | 32.062 | 32.704 | 193.422 | 0.44x |
| mixed.json | strata | 0.529 | 0.552 | 0.581 | 164.672 | 1.00x |
| mixed.json | orjson | 1.172 | 1.406 | 1.522 | 164.672 | 0.39x |
| mixed.json | msgspec | 1.216 | 1.434 | 1.608 | 164.672 | 0.38x |
| mixed.json | ujson | 1.376 | 1.517 | 1.736 | 164.672 | 0.36x |
| mixed.json | json | 1.828 | 2.023 | 2.339 | 164.672 | 0.27x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.529 | 11.645 | 12.048 | 592.844 | 1.00x |
| users.json | orjson | 12.203 | 12.455 | 12.600 | 592.844 | 0.94x |
| users.json | msgspec | 15.866 | 16.134 | 16.475 | 592.844 | 0.72x |
| users.json | ujson | 63.708 | 64.184 | 65.881 | 592.844 | 0.18x |
| users.json | json | 87.960 | 88.420 | 88.971 | 592.844 | 0.13x |
| flat.json | strata | 0.829 | 0.835 | 0.920 | 164.406 | 1.00x |
| flat.json | orjson | 0.862 | 0.878 | 0.980 | 164.406 | 0.95x |
| flat.json | msgspec | 1.146 | 1.188 | 1.250 | 164.406 | 0.70x |
| flat.json | ujson | 3.602 | 3.743 | 3.825 | 164.406 | 0.22x |
| flat.json | json | 4.944 | 5.123 | 5.314 | 164.406 | 0.16x |
| nested.json | strata | 0.606 | 0.634 | 0.746 | 166.062 | 1.00x |
| nested.json | orjson | 0.794 | 0.866 | 0.933 | 166.062 | 0.73x |
| nested.json | msgspec | 1.056 | 1.141 | 1.205 | 166.062 | 0.56x |
| nested.json | ujson | 3.823 | 4.035 | 4.162 | 166.062 | 0.16x |
| nested.json | json | 6.173 | 6.502 | 6.765 | 166.062 | 0.10x |
| wide_arrays.json | strata | 4.715 | 5.028 | 5.536 | 189.297 | 1.00x |
| wide_arrays.json | orjson | 5.188 | 5.220 | 5.598 | 189.297 | 0.96x |
| wide_arrays.json | msgspec | 8.490 | 8.590 | 8.722 | 189.297 | 0.59x |
| wide_arrays.json | ujson | 24.712 | 25.460 | 26.141 | 189.297 | 0.20x |
| wide_arrays.json | json | 44.632 | 45.448 | 47.413 | 189.297 | 0.11x |
| mixed.json | strata | 0.159 | 0.169 | 0.197 | 164.688 | 1.00x |
| mixed.json | orjson | 0.157 | 0.173 | 0.194 | 164.688 | 0.98x |
| mixed.json | msgspec | 0.175 | 0.186 | 0.199 | 164.688 | 0.91x |
| mixed.json | ujson | 0.870 | 0.895 | 0.903 | 164.688 | 0.19x |
| mixed.json | json | 1.320 | 1.380 | 1.444 | 164.688 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 44.034 | 46.269 | 49.841 | 647.969 | 1.00x |
| users.json | orjson | 64.941 | 66.765 | 70.994 | 647.969 | 0.69x |
| users.json | msgspec | 65.915 | 67.708 | 72.377 | 647.969 | 0.68x |
| users.json | ujson | 86.513 | 88.910 | 92.926 | 647.969 | 0.52x |
| users.json | json | 101.651 | 104.647 | 108.783 | 647.969 | 0.44x |
| flat.json | strata | 2.804 | 2.883 | 3.044 | 166.062 | 1.00x |
| flat.json | orjson | 4.010 | 4.346 | 4.572 | 166.062 | 0.66x |
| flat.json | msgspec | 4.017 | 4.407 | 4.682 | 166.062 | 0.65x |
| flat.json | ujson | 5.551 | 5.789 | 6.339 | 166.062 | 0.50x |
| flat.json | json | 6.910 | 7.095 | 7.618 | 166.062 | 0.41x |
| nested.json | strata | 2.405 | 2.489 | 2.644 | 166.062 | 1.00x |
| nested.json | orjson | 3.586 | 4.026 | 4.216 | 166.062 | 0.62x |
| nested.json | msgspec | 3.538 | 3.999 | 4.301 | 166.062 | 0.62x |
| nested.json | ujson | 4.736 | 5.366 | 5.604 | 166.062 | 0.46x |
| nested.json | json | 6.545 | 7.191 | 7.587 | 166.062 | 0.35x |
| wide_arrays.json | strata | 13.875 | 14.416 | 15.233 | 157.625 | 1.00x |
| wide_arrays.json | orjson | 15.737 | 16.375 | 17.022 | 157.625 | 0.88x |
| wide_arrays.json | msgspec | 18.835 | 19.416 | 20.587 | 157.625 | 0.74x |
| wide_arrays.json | ujson | 24.204 | 25.333 | 25.646 | 157.625 | 0.57x |
| wide_arrays.json | json | 33.019 | 34.031 | 35.129 | 157.625 | 0.42x |
| mixed.json | strata | 0.599 | 0.620 | 0.692 | 164.688 | 1.00x |
| mixed.json | orjson | 1.347 | 1.481 | 1.759 | 164.688 | 0.42x |
| mixed.json | msgspec | 1.395 | 1.496 | 1.861 | 164.688 | 0.41x |
| mixed.json | ujson | 1.581 | 1.675 | 2.125 | 164.688 | 0.37x |
| mixed.json | json | 2.007 | 2.147 | 2.551 | 164.688 | 0.29x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 44.309 | 46.015 | 47.385 | 167.547 | 1.00x |
| users.ndjson | orjson | 65.513 | 67.027 | 70.908 | 167.547 | 0.69x |
| users.ndjson | msgspec | 64.846 | 66.338 | 70.387 | 167.547 | 0.69x |
| users.ndjson | ujson | 84.161 | 86.204 | 90.356 | 167.547 | 0.53x |
| users.ndjson | json | 106.350 | 107.840 | 111.546 | 167.547 | 0.43x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 12.920 | 13.357 | 14.753 | 569.359 | 1.00x |
| users.json | orjson | 13.815 | 14.805 | 15.039 | 569.359 | 0.90x |
| users.json | msgspec | 17.762 | 18.921 | 19.233 | 569.359 | 0.71x |
| users.json | ujson | 63.681 | 66.738 | 67.799 | 569.359 | 0.20x |
| users.json | json | 87.838 | 89.476 | 89.734 | 569.359 | 0.15x |
| flat.json | strata | 1.066 | 1.185 | 1.335 | 166.062 | 1.00x |
| flat.json | orjson | 1.112 | 1.235 | 1.426 | 166.062 | 0.96x |
| flat.json | msgspec | 1.377 | 1.499 | 1.719 | 166.062 | 0.79x |
| flat.json | ujson | 3.720 | 4.001 | 4.337 | 166.062 | 0.30x |
| flat.json | json | 5.258 | 5.646 | 6.078 | 166.062 | 0.21x |
| nested.json | strata | 0.819 | 0.877 | 0.989 | 166.172 | 1.00x |
| nested.json | orjson | 1.011 | 1.100 | 1.203 | 166.172 | 0.80x |
| nested.json | msgspec | 1.276 | 1.398 | 1.619 | 166.172 | 0.63x |
| nested.json | ujson | 4.057 | 4.344 | 4.604 | 166.172 | 0.20x |
| nested.json | json | 6.427 | 6.867 | 7.312 | 166.172 | 0.13x |
| wide_arrays.json | strata | 5.953 | 6.597 | 7.044 | 164.672 | 1.00x |
| wide_arrays.json | orjson | 6.158 | 6.783 | 7.009 | 164.672 | 0.97x |
| wide_arrays.json | msgspec | 9.665 | 10.027 | 10.531 | 164.672 | 0.66x |
| wide_arrays.json | ujson | 26.041 | 27.377 | 28.426 | 164.672 | 0.24x |
| wide_arrays.json | json | 46.202 | 48.205 | 49.456 | 164.672 | 0.14x |
| mixed.json | strata | 0.273 | 0.341 | 0.386 | 164.688 | 1.00x |
| mixed.json | orjson | 0.294 | 0.343 | 0.420 | 164.688 | 1.00x |
| mixed.json | msgspec | 0.308 | 0.378 | 0.447 | 164.688 | 0.90x |
| mixed.json | ujson | 1.032 | 1.129 | 1.158 | 164.688 | 0.30x |
| mixed.json | json | 1.462 | 1.599 | 1.844 | 164.688 | 0.21x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.186 | 0.198 | 0.248 | 569.719 | 1.00x |
| users.json $[*].id | jmespath | 0.814 | 0.870 | 0.915 | 569.719 | 0.23x |
| users.json $[*].id | jsonpath-ng | 12.996 | 13.794 | 14.365 | 569.719 | 0.01x |
| users.json $[*].orders[*].total | strata | 2.476 | 2.548 | 2.713 | 575.703 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 8.394 | 8.469 | 8.555 | 575.703 | 0.30x |
| users.json $[*].orders[*].total | jsonpath-ng | 45.013 | 45.482 | 48.733 | 575.703 | 0.06x |
| users.json $..total | strata | 7.797 | 7.890 | 8.884 | 222.531 | 1.00x |
| users.json $..total | jsonpath-ng | 1244.080 | 1249.151 | 1291.273 | 222.531 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 18.552 | 18.807 | 19.047 | 589.406 | 1.00x |
| users.json $[*].id | orjson+jmespath | 66.416 | 67.154 | 67.845 | 589.406 | 0.28x |
| users.json $[*].id | orjson+jsonpath-ng | 68.735 | 69.481 | 70.140 | 589.406 | 0.27x |
| users.json $[*].orders[*].total | strata | 18.883 | 19.065 | 19.688 | 608.844 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 72.404 | 73.571 | 75.554 | 608.844 | 0.26x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 101.055 | 102.537 | 107.601 | 608.844 | 0.19x |
| users.json $..total | strata | 64.274 | 67.275 | 68.585 | 202.750 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 1357.916 | 1363.141 | 1369.846 | 202.750 | 0.05x |

