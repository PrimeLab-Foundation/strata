# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 9b6124a
- python: 3.14.7
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit-Mach-O
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
| users.json | strata | 6.917 | 7.101 | 7.661 | 52.547 | 1.00x |
| users.json | orjson | 8.179 | 8.424 | 8.938 | 52.547 | 0.84x |
| users.json | msgspec | 8.277 | 8.526 | 8.964 | 52.547 | 0.83x |
| users.json | ujson | 11.366 | 11.628 | 12.162 | 52.547 | 0.61x |
| users.json | json | 15.758 | 16.012 | 16.424 | 52.547 | 0.44x |
| flat.json | strata | 0.571 | 0.588 | 0.632 | 69.797 | 1.00x |
| flat.json | orjson | 0.616 | 0.646 | 0.678 | 69.797 | 0.91x |
| flat.json | msgspec | 0.687 | 0.704 | 0.756 | 69.797 | 0.84x |
| flat.json | ujson | 1.006 | 1.098 | 1.175 | 69.797 | 0.54x |
| flat.json | json | 1.384 | 1.423 | 1.510 | 69.797 | 0.41x |
| nested.json | strata | 0.533 | 0.572 | 0.601 | 69.812 | 1.00x |
| nested.json | orjson | 0.610 | 0.614 | 0.657 | 69.812 | 0.93x |
| nested.json | msgspec | 0.619 | 0.649 | 0.689 | 69.812 | 0.88x |
| nested.json | ujson | 0.894 | 0.946 | 1.194 | 69.812 | 0.60x |
| nested.json | json | 1.418 | 1.453 | 1.698 | 69.812 | 0.39x |
| wide_arrays.json | strata | 3.252 | 3.303 | 3.544 | 69.891 | 1.00x |
| wide_arrays.json | orjson | 3.212 | 3.294 | 3.827 | 69.891 | 1.00x |
| wide_arrays.json | msgspec | 3.978 | 4.177 | 4.727 | 69.891 | 0.79x |
| wide_arrays.json | ujson | 5.189 | 5.585 | 5.721 | 69.891 | 0.59x |
| wide_arrays.json | json | 7.697 | 7.913 | 8.553 | 69.891 | 0.42x |
| mixed.json | strata | 0.135 | 0.140 | 0.185 | 69.922 | 1.00x |
| mixed.json | orjson | 0.128 | 0.135 | 0.218 | 69.922 | 1.04x |
| mixed.json | msgspec | 0.145 | 0.159 | 0.205 | 69.922 | 0.88x |
| mixed.json | ujson | 0.187 | 0.203 | 0.231 | 69.922 | 0.69x |
| mixed.json | json | 0.311 | 0.322 | 0.395 | 69.922 | 0.44x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.439 | 1.458 | 1.548 | 56.078 | 1.00x |
| users.json | orjson | 1.996 | 2.026 | 2.158 | 56.078 | 0.72x |
| users.json | msgspec | 2.618 | 2.671 | 2.950 | 56.078 | 0.55x |
| users.json | ujson | 10.648 | 10.729 | 11.000 | 56.078 | 0.14x |
| users.json | json | 15.632 | 16.176 | 16.839 | 56.078 | 0.09x |
| flat.json | strata | 0.179 | 0.183 | 0.263 | 69.812 | 1.00x |
| flat.json | orjson | 0.228 | 0.239 | 0.322 | 69.812 | 0.76x |
| flat.json | msgspec | 0.296 | 0.306 | 0.358 | 69.812 | 0.60x |
| flat.json | ujson | 0.874 | 0.885 | 0.969 | 69.812 | 0.21x |
| flat.json | json | 1.459 | 1.539 | 1.589 | 69.812 | 0.12x |
| nested.json | strata | 0.135 | 0.140 | 0.143 | 69.891 | 1.00x |
| nested.json | orjson | 0.199 | 0.209 | 0.287 | 69.891 | 0.67x |
| nested.json | msgspec | 0.269 | 0.281 | 0.303 | 69.891 | 0.50x |
| nested.json | ujson | 0.967 | 1.000 | 1.113 | 69.891 | 0.14x |
| nested.json | json | 1.673 | 1.846 | 1.977 | 69.891 | 0.08x |
| wide_arrays.json | strata | 0.933 | 0.969 | 1.084 | 69.922 | 1.00x |
| wide_arrays.json | orjson | 1.110 | 1.163 | 1.277 | 69.922 | 0.83x |
| wide_arrays.json | msgspec | 1.934 | 2.005 | 2.168 | 69.922 | 0.48x |
| wide_arrays.json | ujson | 6.008 | 6.225 | 6.644 | 69.922 | 0.16x |
| wide_arrays.json | json | 11.345 | 11.657 | 12.106 | 69.922 | 0.08x |
| mixed.json | strata | 0.038 | 0.042 | 0.057 | 69.969 | 1.00x |
| mixed.json | orjson | 0.044 | 0.048 | 0.065 | 69.969 | 0.89x |
| mixed.json | msgspec | 0.048 | 0.052 | 0.065 | 69.969 | 0.81x |
| mixed.json | ujson | 0.218 | 0.223 | 0.269 | 69.969 | 0.19x |
| mixed.json | json | 0.365 | 0.417 | 0.448 | 69.969 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.958 | 7.396 | 7.526 | 71.297 | 1.00x |
| users.json | orjson | 8.430 | 8.630 | 8.831 | 71.297 | 0.86x |
| users.json | msgspec | 8.571 | 8.956 | 9.484 | 71.297 | 0.83x |
| users.json | ujson | 11.723 | 11.948 | 12.378 | 71.297 | 0.62x |
| users.json | json | 15.932 | 16.399 | 16.931 | 71.297 | 0.45x |
| flat.json | strata | 0.630 | 0.671 | 0.738 | 69.812 | 1.00x |
| flat.json | orjson | 0.687 | 0.741 | 0.844 | 69.812 | 0.91x |
| flat.json | msgspec | 0.770 | 0.810 | 0.904 | 69.812 | 0.83x |
| flat.json | ujson | 1.099 | 1.174 | 1.426 | 69.812 | 0.57x |
| flat.json | json | 1.449 | 1.611 | 1.678 | 69.812 | 0.42x |
| nested.json | strata | 0.601 | 0.633 | 0.686 | 69.891 | 1.00x |
| nested.json | orjson | 0.685 | 0.731 | 0.955 | 69.891 | 0.87x |
| nested.json | msgspec | 0.687 | 0.720 | 0.765 | 69.891 | 0.88x |
| nested.json | ujson | 0.986 | 1.034 | 1.188 | 69.891 | 0.61x |
| nested.json | json | 1.483 | 1.529 | 1.654 | 69.891 | 0.41x |
| wide_arrays.json | strata | 3.422 | 3.548 | 3.690 | 69.922 | 1.00x |
| wide_arrays.json | orjson | 3.555 | 3.650 | 3.874 | 69.922 | 0.97x |
| wide_arrays.json | msgspec | 4.192 | 4.371 | 4.674 | 69.922 | 0.81x |
| wide_arrays.json | ujson | 5.525 | 5.937 | 6.257 | 69.922 | 0.60x |
| wide_arrays.json | json | 7.820 | 8.273 | 8.615 | 69.922 | 0.43x |
| mixed.json | strata | 0.184 | 0.189 | 0.216 | 69.969 | 1.00x |
| mixed.json | orjson | 0.178 | 0.185 | 0.210 | 69.969 | 1.02x |
| mixed.json | msgspec | 0.192 | 0.201 | 0.269 | 69.969 | 0.94x |
| mixed.json | ujson | 0.285 | 0.326 | 0.347 | 69.969 | 0.58x |
| mixed.json | json | 0.367 | 0.382 | 0.440 | 69.969 | 0.50x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.567 | 7.787 | 7.880 | 68.922 | 1.00x |
| users.ndjson | orjson | 10.664 | 11.125 | 11.408 | 68.922 | 0.70x |
| users.ndjson | msgspec | 10.582 | 10.898 | 11.242 | 68.922 | 0.71x |
| users.ndjson | ujson | 13.845 | 14.041 | 14.553 | 68.922 | 0.55x |
| users.ndjson | json | 19.330 | 19.663 | 19.898 | 68.922 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.829 | 2.005 | 2.406 | 68.500 | 1.00x |
| users.json | orjson | 2.478 | 2.656 | 3.087 | 68.500 | 0.75x |
| users.json | msgspec | 3.005 | 3.263 | 3.615 | 68.500 | 0.61x |
| users.json | ujson | 11.381 | 11.672 | 12.365 | 68.500 | 0.17x |
| users.json | json | 16.344 | 16.993 | 17.665 | 68.500 | 0.12x |
| flat.json | strata | 0.357 | 0.387 | 0.454 | 69.812 | 1.00x |
| flat.json | orjson | 0.402 | 0.458 | 0.533 | 69.812 | 0.85x |
| flat.json | msgspec | 0.462 | 0.496 | 0.534 | 69.812 | 0.78x |
| flat.json | ujson | 1.080 | 1.133 | 1.169 | 69.812 | 0.34x |
| flat.json | json | 1.559 | 1.763 | 1.832 | 69.812 | 0.22x |
| nested.json | strata | 0.287 | 0.331 | 0.396 | 69.891 | 1.00x |
| nested.json | orjson | 0.368 | 0.383 | 0.413 | 69.891 | 0.87x |
| nested.json | msgspec | 0.429 | 0.460 | 0.547 | 69.891 | 0.72x |
| nested.json | ujson | 1.149 | 1.192 | 1.436 | 69.891 | 0.28x |
| nested.json | json | 1.813 | 1.912 | 2.247 | 69.891 | 0.17x |
| wide_arrays.json | strata | 1.264 | 1.360 | 1.646 | 69.922 | 1.00x |
| wide_arrays.json | orjson | 1.444 | 1.536 | 1.816 | 69.922 | 0.89x |
| wide_arrays.json | msgspec | 2.323 | 2.404 | 2.670 | 69.922 | 0.57x |
| wide_arrays.json | ujson | 6.619 | 6.764 | 7.408 | 69.922 | 0.20x |
| wide_arrays.json | json | 12.061 | 12.747 | 13.305 | 69.922 | 0.11x |
| mixed.json | strata | 0.159 | 0.181 | 0.277 | 70.000 | 1.00x |
| mixed.json | orjson | 0.173 | 0.209 | 0.328 | 70.000 | 0.87x |
| mixed.json | msgspec | 0.177 | 0.235 | 0.343 | 70.000 | 0.77x |
| mixed.json | ujson | 0.352 | 0.375 | 0.485 | 70.000 | 0.48x |
| mixed.json | json | 0.508 | 0.594 | 0.668 | 70.000 | 0.30x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.055 | 0.063 | 0.085 | 68.656 | 1.00x |
| users.json $[*].id | jmespath | 0.284 | 0.348 | 0.368 | 68.656 | 0.18x |
| users.json $[*].id | jsonpath-ng | 1.434 | 1.526 | 1.696 | 68.656 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.334 | 0.377 | 0.552 | 68.891 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.670 | 1.744 | 2.118 | 68.891 | 0.22x |
| users.json $[*].orders[*].total | jsonpath-ng | 9.597 | 10.079 | 11.131 | 68.891 | 0.04x |
| users.json $..total | strata | 1.402 | 1.464 | 1.589 | 71.109 | 1.00x |
| users.json $..total | jsonpath-ng | 187.855 | 191.202 | 193.259 | 71.109 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.499 | 3.670 | 3.816 | 68.781 | 1.00x |
| users.json $[*].id | orjson+jmespath | 8.792 | 8.999 | 9.487 | 68.781 | 0.41x |
| users.json $[*].id | orjson+jsonpath-ng | 10.070 | 10.364 | 10.696 | 68.781 | 0.35x |
| users.json $[*].orders[*].total | strata | 3.549 | 3.628 | 3.749 | 70.922 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 10.141 | 10.432 | 10.843 | 70.922 | 0.35x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 19.532 | 20.910 | 21.470 | 70.922 | 0.17x |
| users.json $..total | strata | 8.689 | 9.037 | 9.276 | 71.109 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 198.864 | 202.677 | 204.088 | 71.109 | 0.04x |

