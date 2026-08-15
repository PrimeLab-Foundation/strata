# Benchmark results - medium

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: dc84475
- python: 3.14.3
- implementation: CPython
- platform: macOS-26.3-arm64-arm-64bit-Mach-O
- machine: arm64
- processor: Apple M1 Max
- compiler_flags: -std=c++20 -O3 -march=native
- repeats: 20
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 40.248 | 41.268 | 46.523 | 154.875 | 1.00x |
| users.json | orjson | 49.606 | 51.719 | 54.492 | 154.875 | 0.80x |
| users.json | msgspec | 50.573 | 52.588 | 56.669 | 154.875 | 0.78x |
| users.json | ujson | 69.132 | 72.514 | 76.886 | 154.875 | 0.57x |
| users.json | json | 88.107 | 89.784 | 94.199 | 154.875 | 0.46x |
| flat.json | strata | 2.411 | 2.423 | 2.524 | 165.781 | 1.00x |
| flat.json | orjson | 3.467 | 3.513 | 3.712 | 165.781 | 0.69x |
| flat.json | msgspec | 3.552 | 3.591 | 3.791 | 165.781 | 0.67x |
| flat.json | ujson | 4.898 | 4.967 | 5.053 | 165.781 | 0.49x |
| flat.json | json | 6.323 | 6.356 | 6.431 | 165.781 | 0.38x |
| nested.json | strata | 2.111 | 2.128 | 2.239 | 135.609 | 1.00x |
| nested.json | orjson | 3.070 | 3.109 | 3.177 | 135.609 | 0.68x |
| nested.json | msgspec | 3.047 | 3.092 | 3.201 | 135.609 | 0.69x |
| nested.json | ujson | 4.142 | 4.207 | 4.342 | 135.609 | 0.51x |
| nested.json | json | 5.887 | 5.960 | 6.187 | 135.609 | 0.36x |
| wide_arrays.json | strata | 13.189 | 13.407 | 13.649 | 162.969 | 1.00x |
| wide_arrays.json | orjson | 13.219 | 13.454 | 13.648 | 162.969 | 1.00x |
| wide_arrays.json | msgspec | 16.107 | 16.526 | 17.029 | 162.969 | 0.81x |
| wide_arrays.json | ujson | 20.919 | 21.330 | 21.685 | 162.969 | 0.63x |
| wide_arrays.json | json | 30.232 | 30.769 | 31.394 | 162.969 | 0.44x |
| mixed.json | strata | 0.514 | 0.519 | 0.537 | 174.875 | 1.00x |
| mixed.json | orjson | 1.168 | 1.192 | 1.273 | 174.875 | 0.44x |
| mixed.json | msgspec | 1.210 | 1.227 | 1.304 | 174.875 | 0.42x |
| mixed.json | ujson | 1.373 | 1.390 | 1.410 | 174.875 | 0.37x |
| mixed.json | json | 1.817 | 1.839 | 1.884 | 174.875 | 0.28x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.071 | 10.321 | 11.173 | 554.812 | 1.00x |
| users.json | orjson | 12.187 | 12.521 | 13.259 | 554.812 | 0.82x |
| users.json | msgspec | 15.938 | 16.209 | 17.173 | 554.812 | 0.64x |
| users.json | ujson | 64.000 | 64.745 | 66.749 | 554.812 | 0.16x |
| users.json | json | 87.772 | 88.472 | 89.305 | 554.812 | 0.12x |
| flat.json | strata | 0.709 | 0.719 | 0.742 | 165.781 | 1.00x |
| flat.json | orjson | 0.866 | 0.873 | 0.892 | 165.781 | 0.82x |
| flat.json | msgspec | 1.134 | 1.140 | 1.207 | 165.781 | 0.63x |
| flat.json | ujson | 3.503 | 3.531 | 3.587 | 165.781 | 0.20x |
| flat.json | json | 4.876 | 4.933 | 5.047 | 165.781 | 0.15x |
| nested.json | strata | 0.539 | 0.547 | 0.566 | 135.609 | 1.00x |
| nested.json | orjson | 0.782 | 0.789 | 0.800 | 135.609 | 0.69x |
| nested.json | msgspec | 1.034 | 1.047 | 1.077 | 135.609 | 0.52x |
| nested.json | ujson | 3.803 | 3.846 | 3.910 | 135.609 | 0.14x |
| nested.json | json | 6.127 | 6.212 | 6.283 | 135.609 | 0.09x |
| wide_arrays.json | strata | 4.517 | 4.794 | 5.024 | 158.875 | 1.00x |
| wide_arrays.json | orjson | 4.935 | 5.103 | 5.207 | 158.875 | 0.94x |
| wide_arrays.json | msgspec | 8.259 | 8.397 | 8.472 | 158.875 | 0.57x |
| wide_arrays.json | ujson | 23.974 | 24.313 | 24.754 | 158.875 | 0.20x |
| wide_arrays.json | json | 43.406 | 43.826 | 44.282 | 158.875 | 0.11x |
| mixed.json | strata | 0.137 | 0.141 | 0.150 | 174.891 | 1.00x |
| mixed.json | orjson | 0.151 | 0.155 | 0.161 | 174.891 | 0.91x |
| mixed.json | msgspec | 0.170 | 0.173 | 0.176 | 174.891 | 0.81x |
| mixed.json | ujson | 0.824 | 0.840 | 0.856 | 174.891 | 0.17x |
| mixed.json | json | 1.281 | 1.307 | 1.345 | 174.891 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 41.677 | 42.817 | 46.375 | 575.938 | 1.00x |
| users.json | orjson | 64.531 | 65.962 | 68.888 | 575.938 | 0.65x |
| users.json | msgspec | 65.348 | 66.141 | 70.858 | 575.938 | 0.65x |
| users.json | ujson | 84.682 | 86.543 | 93.865 | 575.938 | 0.49x |
| users.json | json | 102.451 | 104.589 | 108.935 | 575.938 | 0.41x |
| flat.json | strata | 2.575 | 2.622 | 2.650 | 135.609 | 1.00x |
| flat.json | orjson | 3.884 | 3.930 | 4.052 | 135.609 | 0.67x |
| flat.json | msgspec | 4.014 | 4.090 | 4.274 | 135.609 | 0.64x |
| flat.json | ujson | 5.404 | 5.516 | 5.607 | 135.609 | 0.48x |
| flat.json | json | 6.775 | 6.847 | 7.220 | 135.609 | 0.38x |
| nested.json | strata | 2.238 | 2.279 | 2.337 | 135.609 | 1.00x |
| nested.json | orjson | 3.526 | 3.587 | 3.784 | 135.609 | 0.64x |
| nested.json | msgspec | 3.554 | 3.599 | 3.708 | 135.609 | 0.63x |
| nested.json | ujson | 4.694 | 4.786 | 5.005 | 135.609 | 0.48x |
| nested.json | json | 6.363 | 6.500 | 6.738 | 135.609 | 0.35x |
| wide_arrays.json | strata | 13.744 | 13.974 | 14.240 | 167.828 | 1.00x |
| wide_arrays.json | orjson | 15.417 | 15.654 | 16.030 | 167.828 | 0.89x |
| wide_arrays.json | msgspec | 18.483 | 18.728 | 19.490 | 167.828 | 0.75x |
| wide_arrays.json | ujson | 23.657 | 24.251 | 24.814 | 167.828 | 0.58x |
| wide_arrays.json | json | 32.831 | 33.283 | 33.720 | 167.828 | 0.42x |
| mixed.json | strata | 0.575 | 0.588 | 0.617 | 174.891 | 1.00x |
| mixed.json | orjson | 1.343 | 1.367 | 1.432 | 174.891 | 0.43x |
| mixed.json | msgspec | 1.387 | 1.409 | 1.476 | 174.891 | 0.42x |
| mixed.json | ujson | 1.576 | 1.608 | 1.665 | 174.891 | 0.37x |
| mixed.json | json | 1.988 | 2.048 | 2.165 | 174.891 | 0.29x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 42.805 | 43.130 | 43.570 | 175.953 | 1.00x |
| users.ndjson | orjson | 63.916 | 65.111 | 66.071 | 175.953 | 0.66x |
| users.ndjson | msgspec | 63.337 | 64.784 | 65.649 | 175.953 | 0.67x |
| users.ndjson | ujson | 80.204 | 82.259 | 84.200 | 175.953 | 0.52x |
| users.ndjson | json | 104.052 | 105.912 | 107.252 | 175.953 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.783 | 12.175 | 13.306 | 479.375 | 1.00x |
| users.json | orjson | 14.353 | 14.894 | 15.788 | 479.375 | 0.82x |
| users.json | msgspec | 17.689 | 17.931 | 19.057 | 479.375 | 0.68x |
| users.json | ujson | 65.176 | 66.394 | 67.514 | 479.375 | 0.18x |
| users.json | json | 89.631 | 90.763 | 91.624 | 479.375 | 0.13x |
| flat.json | strata | 0.915 | 0.958 | 0.994 | 135.609 | 1.00x |
| flat.json | orjson | 1.082 | 1.120 | 1.188 | 135.609 | 0.86x |
| flat.json | msgspec | 1.351 | 1.389 | 1.432 | 135.609 | 0.69x |
| flat.json | ujson | 3.686 | 3.713 | 3.841 | 135.609 | 0.26x |
| flat.json | json | 5.096 | 5.150 | 5.291 | 135.609 | 0.19x |
| nested.json | strata | 0.740 | 0.767 | 0.805 | 135.719 | 1.00x |
| nested.json | orjson | 0.986 | 1.011 | 1.047 | 135.719 | 0.76x |
| nested.json | msgspec | 1.237 | 1.269 | 1.310 | 135.719 | 0.60x |
| nested.json | ujson | 4.007 | 4.066 | 4.104 | 135.719 | 0.19x |
| nested.json | json | 6.350 | 6.426 | 6.512 | 135.719 | 0.12x |
| wide_arrays.json | strata | 5.358 | 5.671 | 5.947 | 174.875 | 1.00x |
| wide_arrays.json | orjson | 5.762 | 5.904 | 6.118 | 174.875 | 0.96x |
| wide_arrays.json | msgspec | 9.062 | 9.229 | 9.560 | 174.875 | 0.61x |
| wide_arrays.json | ujson | 25.079 | 25.606 | 25.908 | 174.875 | 0.22x |
| wide_arrays.json | json | 44.073 | 44.785 | 45.133 | 174.875 | 0.13x |
| mixed.json | strata | 0.254 | 0.274 | 0.311 | 174.891 | 1.00x |
| mixed.json | orjson | 0.278 | 0.292 | 0.356 | 174.891 | 0.94x |
| mixed.json | msgspec | 0.289 | 0.313 | 0.326 | 174.891 | 0.88x |
| mixed.json | ujson | 0.961 | 0.989 | 1.065 | 174.891 | 0.28x |
| mixed.json | json | 1.416 | 1.439 | 1.504 | 174.891 | 0.19x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.173 | 0.188 | 0.207 | 479.766 | 1.00x |
| users.json $[*].id | jmespath | 0.796 | 0.836 | 0.883 | 479.766 | 0.22x |
| users.json $[*].id | jsonpath-ng | 12.531 | 13.130 | 14.300 | 479.766 | 0.01x |
| users.json $[*].orders[*].total | strata | 2.448 | 2.520 | 3.004 | 425.250 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 8.171 | 8.327 | 8.488 | 425.250 | 0.30x |
| users.json $[*].orders[*].total | jsonpath-ng | 44.361 | 44.903 | 46.311 | 425.250 | 0.06x |
| users.json $..total | strata | 7.828 | 7.894 | 8.176 | 157.531 | 1.00x |
| users.json $..total | jsonpath-ng | 1242.060 | 1245.048 | 1248.055 | 157.531 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 18.756 | 18.884 | 19.029 | 455.000 | 1.00x |
| users.json $[*].id | orjson+jmespath | 65.614 | 66.980 | 68.093 | 455.000 | 0.28x |
| users.json $[*].id | orjson+jsonpath-ng | 67.917 | 69.206 | 70.761 | 455.000 | 0.27x |
| users.json $[*].orders[*].total | strata | 19.148 | 19.233 | 19.504 | 267.609 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 71.972 | 73.120 | 75.133 | 267.609 | 0.26x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 100.481 | 101.744 | 103.436 | 267.609 | 0.19x |
| users.json $..total | strata | 61.431 | 62.498 | 63.194 | 202.625 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 1296.235 | 1301.292 | 1305.874 | 202.625 | 0.05x |

