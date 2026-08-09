# Benchmark results - medium

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 5292dd0
- python: 3.14.3
- implementation: CPython
- platform: macOS-26.3-arm64-arm-64bit-Mach-O
- machine: arm64
- processor: Apple M1 Max
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 20
- warmup: 3

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 46.126 | 47.504 | 51.137 | 154.797 | 1.00x |
| users.json | orjson | 50.492 | 52.818 | 55.912 | 154.797 | 0.90x |
| users.json | msgspec | 51.481 | 53.834 | 58.438 | 154.797 | 0.88x |
| users.json | ujson | 70.624 | 73.673 | 78.054 | 154.797 | 0.64x |
| users.json | json | 88.353 | 90.740 | 96.414 | 154.797 | 0.52x |
| flat.json | strata | 2.914 | 2.924 | 3.090 | 160.312 | 1.00x |
| flat.json | orjson | 3.515 | 3.564 | 3.752 | 160.312 | 0.82x |
| flat.json | msgspec | 3.502 | 3.522 | 3.762 | 160.312 | 0.83x |
| flat.json | ujson | 4.824 | 4.866 | 4.947 | 160.312 | 0.60x |
| flat.json | json | 6.318 | 6.345 | 6.398 | 160.312 | 0.46x |
| nested.json | strata | 2.388 | 2.402 | 2.441 | 161.359 | 1.00x |
| nested.json | orjson | 3.032 | 3.048 | 3.100 | 161.359 | 0.79x |
| nested.json | msgspec | 3.021 | 3.047 | 3.096 | 161.359 | 0.79x |
| nested.json | ujson | 4.110 | 4.133 | 4.228 | 161.359 | 0.58x |
| nested.json | json | 5.831 | 5.886 | 5.980 | 161.359 | 0.41x |
| wide_arrays.json | strata | 14.314 | 14.580 | 14.695 | 189.219 | 1.00x |
| wide_arrays.json | orjson | 13.195 | 13.454 | 13.693 | 189.219 | 1.08x |
| wide_arrays.json | msgspec | 16.010 | 16.472 | 16.649 | 189.219 | 0.89x |
| wide_arrays.json | ujson | 20.767 | 21.266 | 21.648 | 189.219 | 0.69x |
| wide_arrays.json | json | 30.101 | 30.900 | 31.436 | 189.219 | 0.47x |
| mixed.json | strata | 0.622 | 0.627 | 0.636 | 170.141 | 1.00x |
| mixed.json | orjson | 1.161 | 1.168 | 1.224 | 170.141 | 0.54x |
| mixed.json | msgspec | 1.198 | 1.211 | 1.249 | 170.141 | 0.52x |
| mixed.json | ujson | 1.367 | 1.373 | 1.407 | 170.141 | 0.46x |
| mixed.json | json | 1.797 | 1.817 | 1.884 | 170.141 | 0.34x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.755 | 12.181 | 12.447 | 551.625 | 1.00x |
| users.json | orjson | 12.233 | 12.650 | 13.256 | 551.625 | 0.96x |
| users.json | msgspec | 15.979 | 16.407 | 17.064 | 551.625 | 0.74x |
| users.json | ujson | 63.721 | 64.814 | 66.951 | 551.625 | 0.19x |
| users.json | json | 88.413 | 88.873 | 89.617 | 551.625 | 0.14x |
| flat.json | strata | 0.810 | 0.816 | 0.819 | 160.406 | 1.00x |
| flat.json | orjson | 0.849 | 0.854 | 0.871 | 160.406 | 0.96x |
| flat.json | msgspec | 1.119 | 1.122 | 1.138 | 160.406 | 0.73x |
| flat.json | ujson | 3.384 | 3.394 | 3.414 | 160.406 | 0.24x |
| flat.json | json | 4.818 | 4.829 | 4.907 | 160.406 | 0.17x |
| nested.json | strata | 0.610 | 0.614 | 0.621 | 161.375 | 1.00x |
| nested.json | orjson | 0.767 | 0.773 | 0.780 | 161.375 | 0.79x |
| nested.json | msgspec | 1.022 | 1.034 | 1.041 | 161.375 | 0.59x |
| nested.json | ujson | 3.738 | 3.759 | 3.783 | 161.375 | 0.16x |
| nested.json | json | 6.028 | 6.063 | 6.141 | 161.375 | 0.10x |
| wide_arrays.json | strata | 4.535 | 4.929 | 5.041 | 185.109 | 1.00x |
| wide_arrays.json | orjson | 4.962 | 5.044 | 5.230 | 185.109 | 0.98x |
| wide_arrays.json | msgspec | 8.215 | 8.321 | 8.475 | 185.109 | 0.59x |
| wide_arrays.json | ujson | 24.007 | 24.424 | 24.838 | 185.109 | 0.20x |
| wide_arrays.json | json | 43.225 | 43.778 | 44.041 | 185.109 | 0.11x |
| mixed.json | strata | 0.156 | 0.158 | 0.161 | 170.156 | 1.00x |
| mixed.json | orjson | 0.150 | 0.151 | 0.156 | 170.156 | 1.04x |
| mixed.json | msgspec | 0.168 | 0.169 | 0.174 | 170.156 | 0.93x |
| mixed.json | ujson | 0.815 | 0.821 | 0.831 | 170.156 | 0.19x |
| mixed.json | json | 1.253 | 1.264 | 1.281 | 170.156 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 47.491 | 48.391 | 52.411 | 600.797 | 1.00x |
| users.json | orjson | 64.566 | 66.254 | 68.928 | 600.797 | 0.73x |
| users.json | msgspec | 65.530 | 66.793 | 71.470 | 600.797 | 0.72x |
| users.json | ujson | 85.119 | 87.600 | 93.326 | 600.797 | 0.55x |
| users.json | json | 102.081 | 103.659 | 107.642 | 600.797 | 0.47x |
| flat.json | strata | 3.089 | 3.153 | 3.178 | 161.266 | 1.00x |
| flat.json | orjson | 3.940 | 4.032 | 4.081 | 161.266 | 0.78x |
| flat.json | msgspec | 3.961 | 4.010 | 4.051 | 161.266 | 0.79x |
| flat.json | ujson | 5.382 | 5.475 | 5.580 | 161.266 | 0.58x |
| flat.json | json | 6.736 | 6.864 | 6.954 | 161.266 | 0.46x |
| nested.json | strata | 2.488 | 2.518 | 2.596 | 161.938 | 1.00x |
| nested.json | orjson | 3.462 | 3.506 | 3.621 | 161.938 | 0.72x |
| nested.json | msgspec | 3.476 | 3.521 | 3.845 | 161.938 | 0.72x |
| nested.json | ujson | 4.659 | 4.709 | 4.881 | 161.938 | 0.53x |
| nested.json | json | 6.326 | 6.395 | 6.587 | 161.938 | 0.39x |
| wide_arrays.json | strata | 15.116 | 15.399 | 15.519 | 194.062 | 1.00x |
| wide_arrays.json | orjson | 15.395 | 15.634 | 15.922 | 194.062 | 0.98x |
| wide_arrays.json | msgspec | 18.392 | 18.857 | 19.135 | 194.062 | 0.82x |
| wide_arrays.json | ujson | 23.667 | 24.237 | 24.564 | 194.062 | 0.64x |
| wide_arrays.json | json | 32.713 | 33.303 | 33.722 | 194.062 | 0.46x |
| mixed.json | strata | 0.683 | 0.698 | 0.729 | 170.156 | 1.00x |
| mixed.json | orjson | 1.323 | 1.334 | 1.379 | 170.156 | 0.52x |
| mixed.json | msgspec | 1.368 | 1.383 | 1.411 | 170.156 | 0.50x |
| mixed.json | ujson | 1.564 | 1.583 | 1.603 | 170.156 | 0.44x |
| mixed.json | json | 1.967 | 1.987 | 2.025 | 170.156 | 0.35x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 45.101 | 45.648 | 46.233 | 174.953 | 1.00x |
| users.ndjson | orjson | 63.870 | 64.774 | 66.335 | 174.953 | 0.70x |
| users.ndjson | msgspec | 63.196 | 64.552 | 66.175 | 174.953 | 0.71x |
| users.ndjson | ujson | 80.522 | 81.960 | 84.878 | 174.953 | 0.56x |
| users.ndjson | json | 102.927 | 105.475 | 107.024 | 174.953 | 0.43x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 13.759 | 14.312 | 15.240 | 524.047 | 1.00x |
| users.json | orjson | 14.392 | 14.733 | 15.496 | 524.047 | 0.97x |
| users.json | msgspec | 17.910 | 18.642 | 19.728 | 524.047 | 0.77x |
| users.json | ujson | 65.648 | 66.923 | 68.279 | 524.047 | 0.21x |
| users.json | json | 89.431 | 91.202 | 92.720 | 524.047 | 0.16x |
| flat.json | strata | 1.036 | 1.069 | 1.100 | 161.359 | 1.00x |
| flat.json | orjson | 1.062 | 1.078 | 1.120 | 161.359 | 0.99x |
| flat.json | msgspec | 1.340 | 1.364 | 1.379 | 161.359 | 0.78x |
| flat.json | ujson | 3.658 | 3.684 | 3.720 | 161.359 | 0.29x |
| flat.json | json | 5.078 | 5.120 | 5.159 | 161.359 | 0.21x |
| nested.json | strata | 0.825 | 0.867 | 0.914 | 161.969 | 1.00x |
| nested.json | orjson | 0.973 | 1.005 | 1.033 | 161.969 | 0.86x |
| nested.json | msgspec | 1.237 | 1.282 | 1.320 | 161.969 | 0.68x |
| nested.json | ujson | 3.993 | 4.077 | 4.123 | 161.969 | 0.21x |
| nested.json | json | 6.316 | 6.437 | 6.482 | 161.969 | 0.13x |
| wide_arrays.json | strata | 5.572 | 5.872 | 6.102 | 170.141 | 1.00x |
| wide_arrays.json | orjson | 5.733 | 5.857 | 5.971 | 170.141 | 1.00x |
| wide_arrays.json | msgspec | 9.150 | 9.285 | 9.445 | 170.141 | 0.63x |
| wide_arrays.json | ujson | 25.314 | 25.684 | 25.913 | 170.141 | 0.23x |
| wide_arrays.json | json | 44.785 | 45.110 | 45.290 | 170.141 | 0.13x |
| mixed.json | strata | 0.313 | 0.343 | 0.428 | 170.156 | 1.00x |
| mixed.json | orjson | 0.270 | 0.292 | 0.327 | 170.156 | 1.18x |
| mixed.json | msgspec | 0.297 | 0.316 | 0.353 | 170.156 | 1.08x |
| mixed.json | ujson | 0.955 | 0.982 | 1.043 | 170.156 | 0.35x |
| mixed.json | json | 1.416 | 1.456 | 1.525 | 170.156 | 0.24x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.172 | 0.186 | 0.220 | 524.406 | 1.00x |
| users.json $[*].id | jmespath | 0.779 | 0.818 | 0.893 | 524.406 | 0.23x |
| users.json $[*].id | jsonpath-ng | 12.643 | 12.944 | 13.971 | 524.406 | 0.01x |
| users.json $[*].orders[*].total | strata | 2.429 | 2.492 | 2.720 | 221.578 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 8.061 | 8.170 | 8.541 | 221.578 | 0.31x |
| users.json $[*].orders[*].total | jsonpath-ng | 44.418 | 45.083 | 46.928 | 221.578 | 0.06x |
| users.json $..total | strata | 7.893 | 8.131 | 8.403 | 147.844 | 1.00x |
| users.json $..total | jsonpath-ng | 1243.739 | 1261.516 | 1273.722 | 147.844 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 21.810 | 22.142 | 22.690 | 443.562 | 1.00x |
| users.json $[*].id | orjson+jmespath | 64.816 | 67.094 | 69.753 | 443.562 | 0.33x |
| users.json $[*].id | orjson+jsonpath-ng | 67.747 | 69.279 | 72.687 | 443.562 | 0.32x |
| users.json $[*].orders[*].total | strata | 22.046 | 22.206 | 22.539 | 284.672 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 71.698 | 72.360 | 73.999 | 284.672 | 0.31x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 100.256 | 101.178 | 102.901 | 284.672 | 0.22x |
| users.json $..total | strata | 66.927 | 67.426 | 68.100 | 202.641 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 1295.930 | 1297.700 | 1302.631 | 202.641 | 0.05x |

