# Benchmark results - medium

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 67d6d68
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
| users.json | strata | 41.540 | 42.807 | 47.160 | 154.953 | 1.00x |
| users.json | orjson | 49.253 | 50.830 | 56.709 | 154.953 | 0.84x |
| users.json | msgspec | 50.062 | 51.865 | 56.624 | 154.953 | 0.83x |
| users.json | ujson | 68.483 | 70.158 | 76.819 | 154.953 | 0.61x |
| users.json | json | 87.219 | 88.960 | 93.459 | 154.953 | 0.48x |
| flat.json | strata | 2.614 | 2.631 | 2.709 | 163.578 | 1.00x |
| flat.json | orjson | 3.580 | 3.596 | 3.658 | 163.578 | 0.73x |
| flat.json | msgspec | 3.540 | 3.559 | 3.709 | 163.578 | 0.74x |
| flat.json | ujson | 4.951 | 4.986 | 5.013 | 163.578 | 0.53x |
| flat.json | json | 6.464 | 6.507 | 6.552 | 163.578 | 0.40x |
| nested.json | strata | 2.223 | 2.254 | 2.267 | 165.438 | 1.00x |
| nested.json | orjson | 3.062 | 3.112 | 3.161 | 165.438 | 0.72x |
| nested.json | msgspec | 3.064 | 3.109 | 3.141 | 165.438 | 0.72x |
| nested.json | ujson | 4.138 | 4.196 | 4.282 | 165.438 | 0.54x |
| nested.json | json | 5.881 | 5.994 | 6.066 | 165.438 | 0.38x |
| wide_arrays.json | strata | 12.897 | 13.101 | 13.271 | 192.797 | 1.00x |
| wide_arrays.json | orjson | 13.198 | 13.410 | 13.639 | 192.797 | 0.98x |
| wide_arrays.json | msgspec | 16.139 | 16.411 | 16.887 | 192.797 | 0.80x |
| wide_arrays.json | ujson | 20.829 | 21.137 | 21.699 | 192.797 | 0.62x |
| wide_arrays.json | json | 30.184 | 30.590 | 31.244 | 192.797 | 0.43x |
| mixed.json | strata | 0.525 | 0.529 | 0.540 | 204.719 | 1.00x |
| mixed.json | orjson | 1.167 | 1.176 | 1.192 | 204.719 | 0.45x |
| mixed.json | msgspec | 1.209 | 1.218 | 1.255 | 204.719 | 0.43x |
| mixed.json | ujson | 1.364 | 1.372 | 1.421 | 204.719 | 0.39x |
| mixed.json | json | 1.817 | 1.830 | 1.866 | 204.719 | 0.29x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.620 | 11.791 | 12.174 | 552.984 | 1.00x |
| users.json | orjson | 12.199 | 12.444 | 13.125 | 552.984 | 0.95x |
| users.json | msgspec | 15.878 | 16.052 | 17.007 | 552.984 | 0.73x |
| users.json | ujson | 63.529 | 64.411 | 66.647 | 552.984 | 0.18x |
| users.json | json | 87.231 | 88.468 | 89.396 | 552.984 | 0.13x |
| flat.json | strata | 0.809 | 0.814 | 0.830 | 163.672 | 1.00x |
| flat.json | orjson | 0.846 | 0.856 | 0.889 | 163.672 | 0.95x |
| flat.json | msgspec | 1.124 | 1.130 | 1.151 | 163.672 | 0.72x |
| flat.json | ujson | 3.399 | 3.407 | 3.439 | 163.672 | 0.24x |
| flat.json | json | 4.831 | 4.850 | 4.904 | 163.672 | 0.17x |
| nested.json | strata | 0.606 | 0.609 | 0.626 | 165.438 | 1.00x |
| nested.json | orjson | 0.770 | 0.775 | 0.785 | 165.438 | 0.79x |
| nested.json | msgspec | 1.031 | 1.039 | 1.060 | 165.438 | 0.59x |
| nested.json | ujson | 3.765 | 3.784 | 3.799 | 165.438 | 0.16x |
| nested.json | json | 6.074 | 6.093 | 6.298 | 165.438 | 0.10x |
| wide_arrays.json | strata | 4.452 | 4.692 | 4.969 | 188.703 | 1.00x |
| wide_arrays.json | orjson | 4.925 | 5.049 | 5.145 | 188.703 | 0.93x |
| wide_arrays.json | msgspec | 8.174 | 8.255 | 8.392 | 188.703 | 0.57x |
| wide_arrays.json | ujson | 24.130 | 24.359 | 24.728 | 188.703 | 0.19x |
| wide_arrays.json | json | 43.161 | 43.746 | 44.252 | 188.703 | 0.11x |
| mixed.json | strata | 0.154 | 0.155 | 0.162 | 204.734 | 1.00x |
| mixed.json | orjson | 0.151 | 0.152 | 0.159 | 204.734 | 1.02x |
| mixed.json | msgspec | 0.169 | 0.170 | 0.179 | 204.734 | 0.91x |
| mixed.json | ujson | 0.820 | 0.825 | 0.850 | 204.734 | 0.19x |
| mixed.json | json | 1.256 | 1.270 | 1.312 | 204.734 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 43.273 | 44.208 | 47.614 | 601.141 | 1.00x |
| users.json | orjson | 63.591 | 65.087 | 69.452 | 601.141 | 0.68x |
| users.json | msgspec | 64.632 | 66.646 | 71.572 | 601.141 | 0.66x |
| users.json | ujson | 83.914 | 86.268 | 94.062 | 601.141 | 0.51x |
| users.json | json | 101.684 | 103.633 | 107.287 | 601.141 | 0.43x |
| flat.json | strata | 2.763 | 2.792 | 2.863 | 165.328 | 1.00x |
| flat.json | orjson | 3.972 | 4.003 | 4.073 | 165.328 | 0.70x |
| flat.json | msgspec | 3.983 | 4.013 | 4.096 | 165.328 | 0.70x |
| flat.json | ujson | 5.508 | 5.560 | 5.699 | 165.328 | 0.50x |
| flat.json | json | 6.887 | 6.930 | 7.064 | 165.328 | 0.40x |
| nested.json | strata | 2.345 | 2.365 | 2.394 | 165.438 | 1.00x |
| nested.json | orjson | 3.489 | 3.519 | 3.656 | 165.438 | 0.67x |
| nested.json | msgspec | 3.526 | 3.555 | 3.716 | 165.438 | 0.67x |
| nested.json | ujson | 4.659 | 4.692 | 4.748 | 165.438 | 0.50x |
| nested.json | json | 6.316 | 6.349 | 6.437 | 165.438 | 0.37x |
| wide_arrays.json | strata | 13.501 | 13.664 | 13.868 | 197.672 | 1.00x |
| wide_arrays.json | orjson | 15.365 | 15.638 | 15.746 | 197.672 | 0.87x |
| wide_arrays.json | msgspec | 18.492 | 18.766 | 19.001 | 197.672 | 0.73x |
| wide_arrays.json | ujson | 23.667 | 24.016 | 24.442 | 197.672 | 0.57x |
| wide_arrays.json | json | 32.609 | 33.143 | 33.441 | 197.672 | 0.41x |
| mixed.json | strata | 0.577 | 0.591 | 0.600 | 204.734 | 1.00x |
| mixed.json | orjson | 1.329 | 1.344 | 1.477 | 204.734 | 0.44x |
| mixed.json | msgspec | 1.370 | 1.385 | 1.453 | 204.734 | 0.43x |
| mixed.json | ujson | 1.559 | 1.576 | 1.622 | 204.734 | 0.37x |
| mixed.json | json | 1.984 | 2.015 | 2.057 | 204.734 | 0.29x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 43.893 | 44.304 | 44.828 | 176.812 | 1.00x |
| users.ndjson | orjson | 63.256 | 64.457 | 65.531 | 176.812 | 0.69x |
| users.ndjson | msgspec | 62.739 | 63.554 | 65.149 | 176.812 | 0.70x |
| users.ndjson | ujson | 79.768 | 81.331 | 83.842 | 176.812 | 0.54x |
| users.ndjson | json | 102.992 | 104.295 | 106.697 | 176.812 | 0.42x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 13.392 | 14.175 | 15.318 | 474.734 | 1.00x |
| users.json | orjson | 14.037 | 14.815 | 16.352 | 474.734 | 0.96x |
| users.json | msgspec | 17.686 | 18.868 | 19.746 | 474.734 | 0.75x |
| users.json | ujson | 64.648 | 65.725 | 68.072 | 474.734 | 0.22x |
| users.json | json | 88.000 | 90.693 | 92.773 | 474.734 | 0.16x |
| flat.json | strata | 1.002 | 1.057 | 1.583 | 165.438 | 1.00x |
| flat.json | orjson | 1.072 | 1.108 | 1.659 | 165.438 | 0.95x |
| flat.json | msgspec | 1.331 | 1.366 | 1.790 | 165.438 | 0.77x |
| flat.json | ujson | 3.747 | 3.774 | 4.254 | 165.438 | 0.28x |
| flat.json | json | 5.098 | 5.146 | 5.681 | 165.438 | 0.21x |
| nested.json | strata | 0.790 | 0.820 | 0.859 | 165.547 | 1.00x |
| nested.json | orjson | 0.965 | 0.989 | 1.054 | 165.547 | 0.83x |
| nested.json | msgspec | 1.222 | 1.244 | 1.280 | 165.547 | 0.66x |
| nested.json | ujson | 3.979 | 4.014 | 4.048 | 165.547 | 0.20x |
| nested.json | json | 6.328 | 6.381 | 6.425 | 165.547 | 0.13x |
| wide_arrays.json | strata | 5.260 | 5.730 | 6.174 | 204.719 | 1.00x |
| wide_arrays.json | orjson | 5.695 | 5.840 | 6.373 | 204.719 | 0.98x |
| wide_arrays.json | msgspec | 9.009 | 9.153 | 9.787 | 204.719 | 0.63x |
| wide_arrays.json | ujson | 25.059 | 25.391 | 26.171 | 204.719 | 0.23x |
| wide_arrays.json | json | 44.063 | 44.267 | 45.051 | 204.719 | 0.13x |
| mixed.json | strata | 0.268 | 0.278 | 0.299 | 204.734 | 1.00x |
| mixed.json | orjson | 0.268 | 0.277 | 0.290 | 204.734 | 1.00x |
| mixed.json | msgspec | 0.287 | 0.299 | 0.314 | 204.734 | 0.93x |
| mixed.json | ujson | 0.954 | 0.971 | 0.992 | 204.734 | 0.29x |
| mixed.json | json | 1.398 | 1.420 | 1.455 | 204.734 | 0.20x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.174 | 0.188 | 0.207 | 475.172 | 1.00x |
| users.json $[*].id | jmespath | 0.786 | 0.838 | 0.933 | 475.172 | 0.22x |
| users.json $[*].id | jsonpath-ng | 12.633 | 13.182 | 13.586 | 475.172 | 0.01x |
| users.json $[*].orders[*].total | strata | 2.433 | 2.559 | 2.974 | 267.781 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 8.089 | 8.470 | 9.075 | 267.781 | 0.30x |
| users.json $[*].orders[*].total | jsonpath-ng | 44.836 | 46.645 | 49.141 | 267.781 | 0.05x |
| users.json $..total | strata | 7.825 | 7.935 | 8.260 | 161.531 | 1.00x |
| users.json $..total | jsonpath-ng | 1234.870 | 1238.410 | 1243.137 | 161.531 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 18.606 | 18.823 | 19.229 | 295.656 | 1.00x |
| users.json $[*].id | orjson+jmespath | 64.984 | 66.272 | 67.371 | 295.656 | 0.28x |
| users.json $[*].id | orjson+jsonpath-ng | 67.377 | 68.718 | 70.288 | 295.656 | 0.27x |
| users.json $[*].orders[*].total | strata | 19.094 | 19.271 | 19.502 | 274.219 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 71.792 | 72.975 | 75.172 | 274.219 | 0.26x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 100.101 | 101.141 | 106.101 | 274.219 | 0.19x |
| users.json $..total | strata | 62.616 | 63.559 | 64.570 | 202.641 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 1289.209 | 1294.619 | 1299.215 | 202.641 | 0.05x |

