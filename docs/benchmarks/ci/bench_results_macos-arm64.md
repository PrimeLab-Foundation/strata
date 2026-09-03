# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: f8c17d0
- python: 3.12.10
- implementation: CPython
- platform: macOS-26.5.2-arm64-arm-64bit
- machine: arm64
- processor: Apple M1 (Virtual)
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.475 | 8.251 | 8.874 | 63.625 | 1.00x |
| users.json | orjson | 8.868 | 11.400 | 12.360 | 63.625 | 0.72x |
| users.json | msgspec | 8.511 | 11.521 | 12.516 | 63.625 | 0.72x |
| users.json | ujson | 11.285 | 14.878 | 15.815 | 63.625 | 0.55x |
| users.json | pysimdjson | 120.552 | 138.197 | 166.402 | 63.625 | 0.06x |
| users.json | json | 14.056 | 18.519 | 21.736 | 63.625 | 0.45x |
| flat.json | strata | 0.665 | 0.703 | 0.914 | 86.031 | 1.00x |
| flat.json | orjson | 0.810 | 0.896 | 1.329 | 86.031 | 0.78x |
| flat.json | msgspec | 0.783 | 0.816 | 2.201 | 86.031 | 0.86x |
| flat.json | ujson | 1.282 | 1.455 | 3.622 | 86.031 | 0.48x |
| flat.json | pysimdjson | 12.735 | 14.746 | 19.107 | 86.031 | 0.05x |
| flat.json | json | 1.440 | 1.553 | 3.979 | 86.031 | 0.45x |
| nested.json | strata | 0.569 | 0.588 | 0.620 | 86.078 | 1.00x |
| nested.json | orjson | 0.773 | 0.803 | 0.936 | 86.078 | 0.73x |
| nested.json | msgspec | 0.721 | 0.770 | 0.807 | 86.078 | 0.76x |
| nested.json | ujson | 1.180 | 1.234 | 1.277 | 86.078 | 0.48x |
| nested.json | pysimdjson | 10.609 | 10.762 | 12.047 | 86.078 | 0.05x |
| nested.json | json | 1.471 | 1.521 | 1.686 | 86.078 | 0.39x |
| wide_arrays.json | strata | 3.766 | 4.035 | 9.661 | 88.969 | 1.00x |
| wide_arrays.json | orjson | 3.861 | 4.433 | 10.495 | 88.969 | 0.91x |
| wide_arrays.json | msgspec | 4.515 | 4.814 | 5.123 | 88.969 | 0.84x |
| wide_arrays.json | ujson | 5.701 | 6.547 | 12.924 | 88.969 | 0.62x |
| wide_arrays.json | pysimdjson | 66.333 | 76.799 | 102.943 | 88.969 | 0.05x |
| wide_arrays.json | json | 7.307 | 7.679 | 9.952 | 88.969 | 0.53x |
| mixed.json | strata | 0.137 | 0.148 | 0.163 | 88.984 | 1.00x |
| mixed.json | orjson | 0.165 | 0.174 | 0.202 | 88.984 | 0.85x |
| mixed.json | msgspec | 0.180 | 0.190 | 0.235 | 88.984 | 0.78x |
| mixed.json | ujson | 0.226 | 0.330 | 0.456 | 88.984 | 0.45x |
| mixed.json | pysimdjson | 2.548 | 2.616 | 2.663 | 88.984 | 0.06x |
| mixed.json | json | 0.346 | 0.361 | 0.376 | 88.984 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.884 | 1.983 | 2.030 | 72.125 | 1.00x |
| users.json | orjson | 2.654 | 2.746 | 2.812 | 72.125 | 0.72x |
| users.json | msgspec | 3.271 | 3.464 | 3.626 | 72.125 | 0.57x |
| users.json | ujson | 12.477 | 12.944 | 14.311 | 72.125 | 0.15x |
| users.json | json | 18.343 | 19.007 | 19.719 | 72.125 | 0.10x |
| flat.json | strata | 0.196 | 0.252 | 0.509 | 86.047 | 1.00x |
| flat.json | orjson | 0.437 | 0.511 | 0.638 | 86.047 | 0.49x |
| flat.json | msgspec | 0.345 | 0.414 | 0.916 | 86.047 | 0.61x |
| flat.json | ujson | 1.016 | 1.096 | 2.587 | 86.047 | 0.23x |
| flat.json | json | 1.421 | 1.602 | 3.386 | 86.047 | 0.16x |
| nested.json | strata | 0.132 | 0.140 | 0.157 | 86.078 | 1.00x |
| nested.json | orjson | 0.242 | 0.257 | 0.282 | 86.078 | 0.55x |
| nested.json | msgspec | 0.302 | 0.336 | 0.447 | 86.078 | 0.42x |
| nested.json | ujson | 1.212 | 1.273 | 1.354 | 86.078 | 0.11x |
| nested.json | json | 1.728 | 1.942 | 2.002 | 86.078 | 0.07x |
| wide_arrays.json | strata | 1.188 | 1.241 | 1.779 | 88.969 | 1.00x |
| wide_arrays.json | orjson | 1.529 | 1.630 | 2.183 | 88.969 | 0.76x |
| wide_arrays.json | msgspec | 2.240 | 2.273 | 2.491 | 88.969 | 0.55x |
| wide_arrays.json | ujson | 6.926 | 7.085 | 7.305 | 88.969 | 0.18x |
| wide_arrays.json | json | 12.424 | 12.666 | 13.466 | 88.969 | 0.10x |
| mixed.json | strata | 0.044 | 0.046 | 0.055 | 88.984 | 1.00x |
| mixed.json | orjson | 0.046 | 0.049 | 0.052 | 88.984 | 0.95x |
| mixed.json | msgspec | 0.056 | 0.059 | 0.063 | 88.984 | 0.78x |
| mixed.json | ujson | 0.229 | 0.246 | 0.271 | 88.984 | 0.19x |
| mixed.json | json | 0.365 | 0.376 | 0.393 | 88.984 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.244 | 8.475 | 8.824 | 82.141 | 1.00x |
| users.json | orjson | 11.289 | 11.601 | 12.233 | 82.141 | 0.73x |
| users.json | msgspec | 10.956 | 11.310 | 11.644 | 82.141 | 0.75x |
| users.json | ujson | 14.646 | 15.311 | 15.533 | 82.141 | 0.55x |
| users.json | json | 17.864 | 18.146 | 18.576 | 82.141 | 0.47x |
| flat.json | strata | 0.717 | 0.742 | 1.247 | 86.047 | 1.00x |
| flat.json | orjson | 1.070 | 1.104 | 2.715 | 86.047 | 0.67x |
| flat.json | msgspec | 0.872 | 0.949 | 2.716 | 86.047 | 0.78x |
| flat.json | ujson | 1.269 | 1.320 | 2.340 | 86.047 | 0.56x |
| flat.json | json | 1.537 | 1.559 | 1.645 | 86.047 | 0.48x |
| nested.json | strata | 0.629 | 0.663 | 0.692 | 86.078 | 1.00x |
| nested.json | orjson | 0.967 | 0.996 | 1.090 | 86.078 | 0.67x |
| nested.json | msgspec | 0.826 | 0.870 | 0.981 | 86.078 | 0.76x |
| nested.json | ujson | 1.080 | 1.186 | 1.243 | 86.078 | 0.56x |
| nested.json | json | 1.612 | 1.645 | 1.743 | 86.078 | 0.40x |
| wide_arrays.json | strata | 3.761 | 3.832 | 3.967 | 88.969 | 1.00x |
| wide_arrays.json | orjson | 4.001 | 4.079 | 4.144 | 88.969 | 0.94x |
| wide_arrays.json | msgspec | 4.570 | 4.660 | 4.806 | 88.969 | 0.82x |
| wide_arrays.json | ujson | 6.098 | 6.250 | 6.360 | 88.969 | 0.61x |
| wide_arrays.json | json | 7.393 | 7.559 | 7.897 | 88.969 | 0.51x |
| mixed.json | strata | 0.200 | 0.206 | 0.220 | 88.984 | 1.00x |
| mixed.json | orjson | 0.242 | 0.356 | 0.500 | 88.984 | 0.58x |
| mixed.json | msgspec | 0.258 | 0.289 | 0.322 | 88.984 | 0.71x |
| mixed.json | ujson | 0.306 | 0.333 | 0.422 | 88.984 | 0.62x |
| mixed.json | json | 0.430 | 0.449 | 0.505 | 88.984 | 0.46x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 8.146 | 8.265 | 9.024 | 86.031 | 1.00x |
| users.ndjson | orjson | 12.636 | 12.840 | 14.972 | 86.031 | 0.64x |
| users.ndjson | msgspec | 12.478 | 12.816 | 14.645 | 86.031 | 0.64x |
| users.ndjson | ujson | 15.353 | 15.687 | 19.139 | 86.031 | 0.53x |
| users.ndjson | json | 20.137 | 20.953 | 24.038 | 86.031 | 0.39x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.124 | 2.401 | 8.984 | 81.328 | 1.00x |
| users.json | orjson | 2.853 | 3.322 | 8.277 | 81.328 | 0.72x |
| users.json | msgspec | 3.593 | 4.210 | 10.928 | 81.328 | 0.57x |
| users.json | ujson | 13.070 | 17.041 | 24.229 | 81.328 | 0.14x |
| users.json | json | 18.135 | 22.003 | 30.991 | 81.328 | 0.11x |
| flat.json | strata | 0.493 | 0.559 | 1.223 | 86.047 | 1.00x |
| flat.json | orjson | 0.535 | 0.799 | 2.200 | 86.047 | 0.70x |
| flat.json | msgspec | 0.553 | 0.702 | 2.392 | 86.047 | 0.80x |
| flat.json | ujson | 1.265 | 1.372 | 3.726 | 86.047 | 0.41x |
| flat.json | json | 1.805 | 2.057 | 4.994 | 86.047 | 0.27x |
| nested.json | strata | 0.465 | 0.508 | 2.481 | 86.078 | 1.00x |
| nested.json | orjson | 0.542 | 0.631 | 1.363 | 86.078 | 0.81x |
| nested.json | msgspec | 0.619 | 0.952 | 2.594 | 86.078 | 0.53x |
| nested.json | ujson | 1.475 | 1.688 | 2.314 | 86.078 | 0.30x |
| nested.json | json | 2.116 | 2.730 | 3.876 | 86.078 | 0.19x |
| wide_arrays.json | strata | 1.712 | 1.775 | 1.903 | 88.969 | 1.00x |
| wide_arrays.json | orjson | 2.150 | 2.253 | 4.944 | 88.969 | 0.79x |
| wide_arrays.json | msgspec | 2.798 | 2.976 | 3.581 | 88.969 | 0.60x |
| wide_arrays.json | ujson | 7.716 | 8.102 | 8.240 | 88.969 | 0.22x |
| wide_arrays.json | json | 13.191 | 13.617 | 14.124 | 88.969 | 0.13x |
| mixed.json | strata | 0.263 | 0.314 | 0.348 | 88.984 | 1.00x |
| mixed.json | orjson | 0.278 | 0.321 | 0.455 | 88.984 | 0.98x |
| mixed.json | msgspec | 0.287 | 0.336 | 0.574 | 88.984 | 0.93x |
| mixed.json | ujson | 0.468 | 0.544 | 1.068 | 88.984 | 0.58x |
| mixed.json | json | 0.593 | 0.686 | 0.776 | 88.984 | 0.46x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.066 | 0.091 | 0.135 | 81.406 | 1.00x |
| users.json $[*].id | jmespath | 0.302 | 0.344 | 0.396 | 81.406 | 0.27x |
| users.json $[*].id | jsonpath-ng | 1.582 | 1.740 | 1.949 | 81.406 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.458 | 0.639 | 1.106 | 81.578 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.941 | 2.248 | 2.662 | 81.578 | 0.28x |
| users.json $[*].orders[*].total | jsonpath-ng | 12.072 | 13.452 | 14.278 | 81.578 | 0.05x |
| users.json $..total | strata | 1.320 | 1.426 | 1.601 | 83.422 | 1.00x |
| users.json $..total | jsonpath-ng | 197.288 | 204.796 | 221.265 | 83.422 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.492 | 4.040 | 4.673 | 81.516 | 1.00x |
| users.json $[*].id | orjson+jmespath | 9.293 | 12.006 | 13.032 | 81.516 | 0.34x |
| users.json $[*].id | orjson+jsonpath-ng | 10.605 | 13.728 | 15.434 | 81.516 | 0.29x |
| users.json $[*].orders[*].total | strata | 4.015 | 4.423 | 5.370 | 83.406 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.965 | 13.970 | 16.750 | 83.406 | 0.32x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 25.341 | 28.382 | 38.403 | 83.406 | 0.16x |
| users.json $..total | strata | 9.448 | 9.846 | 13.493 | 83.453 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 221.493 | 232.854 | 294.188 | 83.453 | 0.04x |

