# Benchmark results - medium

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
| users.json | strata | 41.707 | 44.756 | 47.746 | 139.562 | 1.00x |
| users.json | orjson | 50.591 | 53.680 | 57.246 | 139.562 | 0.83x |
| users.json | msgspec | 52.127 | 54.590 | 56.613 | 139.562 | 0.82x |
| users.json | ujson | 71.928 | 75.702 | 77.517 | 139.562 | 0.59x |
| users.json | json | 89.668 | 94.212 | 95.932 | 139.562 | 0.48x |
| flat.json | strata | 2.521 | 2.537 | 2.611 | 163.453 | 1.00x |
| flat.json | orjson | 3.518 | 3.532 | 3.634 | 163.453 | 0.72x |
| flat.json | msgspec | 3.536 | 3.557 | 3.698 | 163.453 | 0.71x |
| flat.json | ujson | 4.864 | 4.899 | 4.938 | 163.453 | 0.52x |
| flat.json | json | 6.334 | 6.353 | 6.420 | 163.453 | 0.40x |
| nested.json | strata | 2.164 | 2.171 | 2.187 | 164.516 | 1.00x |
| nested.json | orjson | 3.056 | 3.068 | 3.102 | 164.516 | 0.71x |
| nested.json | msgspec | 3.036 | 3.045 | 3.102 | 164.516 | 0.71x |
| nested.json | ujson | 4.099 | 4.143 | 4.198 | 164.516 | 0.52x |
| nested.json | json | 5.960 | 5.992 | 6.130 | 164.516 | 0.36x |
| wide_arrays.json | strata | 13.140 | 13.170 | 13.283 | 192.391 | 1.00x |
| wide_arrays.json | orjson | 13.109 | 13.162 | 13.485 | 192.391 | 1.00x |
| wide_arrays.json | msgspec | 15.986 | 16.064 | 16.474 | 192.391 | 0.82x |
| wide_arrays.json | ujson | 20.633 | 20.734 | 21.195 | 192.391 | 0.64x |
| wide_arrays.json | json | 29.982 | 30.110 | 30.522 | 192.391 | 0.44x |
| mixed.json | strata | 0.503 | 0.508 | 0.514 | 169.016 | 1.00x |
| mixed.json | orjson | 1.159 | 1.170 | 1.999 | 169.016 | 0.43x |
| mixed.json | msgspec | 1.208 | 1.212 | 1.236 | 169.016 | 0.42x |
| mixed.json | ujson | 1.360 | 1.367 | 1.439 | 169.016 | 0.37x |
| mixed.json | json | 1.812 | 1.820 | 1.855 | 169.016 | 0.28x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.971 | 10.032 | 10.553 | 602.109 | 1.00x |
| users.json | orjson | 12.263 | 12.470 | 12.745 | 602.109 | 0.80x |
| users.json | msgspec | 15.890 | 16.109 | 16.227 | 602.109 | 0.62x |
| users.json | ujson | 62.382 | 62.925 | 63.383 | 602.109 | 0.16x |
| users.json | json | 88.325 | 88.889 | 89.290 | 602.109 | 0.11x |
| flat.json | strata | 0.697 | 0.698 | 0.716 | 163.547 | 1.00x |
| flat.json | orjson | 0.853 | 0.858 | 0.867 | 163.547 | 0.81x |
| flat.json | msgspec | 1.125 | 1.128 | 1.160 | 163.547 | 0.62x |
| flat.json | ujson | 3.553 | 3.562 | 3.599 | 163.547 | 0.20x |
| flat.json | json | 4.837 | 4.888 | 4.978 | 163.547 | 0.14x |
| nested.json | strata | 0.512 | 0.513 | 0.525 | 164.547 | 1.00x |
| nested.json | orjson | 0.768 | 0.773 | 0.786 | 164.547 | 0.66x |
| nested.json | msgspec | 1.025 | 1.032 | 1.079 | 164.547 | 0.50x |
| nested.json | ujson | 3.784 | 3.789 | 3.897 | 164.547 | 0.14x |
| nested.json | json | 6.063 | 6.092 | 6.145 | 164.547 | 0.08x |
| wide_arrays.json | strata | 4.533 | 4.771 | 5.203 | 188.281 | 1.00x |
| wide_arrays.json | orjson | 4.957 | 5.034 | 5.379 | 188.281 | 0.95x |
| wide_arrays.json | msgspec | 8.200 | 8.287 | 8.427 | 188.281 | 0.58x |
| wide_arrays.json | ujson | 24.049 | 24.326 | 24.698 | 188.281 | 0.20x |
| wide_arrays.json | json | 42.916 | 43.117 | 43.506 | 188.281 | 0.11x |
| mixed.json | strata | 0.135 | 0.137 | 0.149 | 169.156 | 1.00x |
| mixed.json | orjson | 0.151 | 0.153 | 0.158 | 169.156 | 0.89x |
| mixed.json | msgspec | 0.168 | 0.169 | 0.173 | 169.156 | 0.81x |
| mixed.json | ujson | 0.821 | 0.824 | 0.832 | 169.156 | 0.17x |
| mixed.json | json | 1.291 | 1.295 | 1.308 | 169.156 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 43.500 | 45.548 | 48.619 | 657.203 | 1.00x |
| users.json | orjson | 65.580 | 67.501 | 70.551 | 657.203 | 0.67x |
| users.json | msgspec | 66.740 | 69.448 | 72.205 | 657.203 | 0.66x |
| users.json | ujson | 87.852 | 89.804 | 93.073 | 657.203 | 0.51x |
| users.json | json | 103.599 | 106.746 | 109.841 | 657.203 | 0.43x |
| flat.json | strata | 2.691 | 2.711 | 2.748 | 164.406 | 1.00x |
| flat.json | orjson | 3.906 | 3.918 | 4.055 | 164.406 | 0.69x |
| flat.json | msgspec | 3.979 | 4.008 | 4.129 | 164.406 | 0.68x |
| flat.json | ujson | 5.410 | 5.453 | 5.679 | 164.406 | 0.50x |
| flat.json | json | 6.769 | 6.794 | 6.835 | 164.406 | 0.40x |
| nested.json | strata | 2.274 | 2.304 | 2.478 | 165.109 | 1.00x |
| nested.json | orjson | 3.489 | 3.540 | 3.631 | 165.109 | 0.65x |
| nested.json | msgspec | 3.510 | 3.574 | 3.673 | 165.109 | 0.64x |
| nested.json | ujson | 4.657 | 4.727 | 4.847 | 165.109 | 0.49x |
| nested.json | json | 6.457 | 6.488 | 7.068 | 165.109 | 0.36x |
| wide_arrays.json | strata | 13.687 | 13.728 | 13.889 | 197.234 | 1.00x |
| wide_arrays.json | orjson | 15.221 | 15.389 | 15.771 | 197.234 | 0.89x |
| wide_arrays.json | msgspec | 18.265 | 18.447 | 18.727 | 197.234 | 0.74x |
| wide_arrays.json | ujson | 23.440 | 23.725 | 24.103 | 197.234 | 0.58x |
| wide_arrays.json | json | 32.385 | 32.498 | 32.795 | 197.234 | 0.42x |
| mixed.json | strata | 0.565 | 0.577 | 0.582 | 169.156 | 1.00x |
| mixed.json | orjson | 1.321 | 1.332 | 1.358 | 169.156 | 0.43x |
| mixed.json | msgspec | 1.375 | 1.394 | 1.430 | 169.156 | 0.41x |
| mixed.json | ujson | 1.561 | 1.586 | 1.631 | 169.156 | 0.36x |
| mixed.json | json | 1.981 | 1.993 | 2.007 | 169.156 | 0.29x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 42.477 | 42.610 | 43.203 | 165.578 | 1.00x |
| users.ndjson | orjson | 63.357 | 63.846 | 66.416 | 165.578 | 0.67x |
| users.ndjson | msgspec | 62.541 | 63.181 | 64.009 | 165.578 | 0.67x |
| users.ndjson | ujson | 81.071 | 81.640 | 82.539 | 165.578 | 0.52x |
| users.ndjson | json | 103.326 | 104.404 | 105.203 | 165.578 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.344 | 12.077 | 12.998 | 674.141 | 1.00x |
| users.json | orjson | 13.917 | 14.511 | 15.798 | 674.141 | 0.83x |
| users.json | msgspec | 17.759 | 18.337 | 19.224 | 674.141 | 0.66x |
| users.json | ujson | 64.997 | 65.708 | 66.243 | 674.141 | 0.18x |
| users.json | json | 90.432 | 91.258 | 92.907 | 674.141 | 0.13x |
| flat.json | strata | 0.914 | 0.943 | 0.961 | 164.516 | 1.00x |
| flat.json | orjson | 1.077 | 1.097 | 1.129 | 164.516 | 0.86x |
| flat.json | msgspec | 1.356 | 1.375 | 1.398 | 164.516 | 0.69x |
| flat.json | ujson | 3.779 | 3.819 | 3.878 | 164.516 | 0.25x |
| flat.json | json | 5.102 | 5.160 | 5.236 | 164.516 | 0.18x |
| nested.json | strata | 0.705 | 0.745 | 0.813 | 165.141 | 1.00x |
| nested.json | orjson | 0.973 | 1.002 | 1.036 | 165.141 | 0.74x |
| nested.json | msgspec | 1.239 | 1.260 | 1.303 | 165.141 | 0.59x |
| nested.json | ujson | 4.050 | 4.071 | 4.169 | 165.141 | 0.18x |
| nested.json | json | 6.326 | 6.352 | 6.411 | 165.141 | 0.12x |
| wide_arrays.json | strata | 5.291 | 5.627 | 6.088 | 204.281 | 1.00x |
| wide_arrays.json | orjson | 5.745 | 5.835 | 6.037 | 204.281 | 0.96x |
| wide_arrays.json | msgspec | 9.139 | 9.208 | 9.833 | 204.281 | 0.61x |
| wide_arrays.json | ujson | 25.170 | 25.550 | 25.983 | 204.281 | 0.22x |
| wide_arrays.json | json | 44.005 | 44.114 | 44.389 | 204.281 | 0.13x |
| mixed.json | strata | 0.255 | 0.271 | 0.290 | 169.156 | 1.00x |
| mixed.json | orjson | 0.283 | 0.296 | 0.314 | 169.156 | 0.92x |
| mixed.json | msgspec | 0.300 | 0.313 | 0.337 | 169.156 | 0.87x |
| mixed.json | ujson | 0.975 | 0.984 | 0.997 | 169.156 | 0.28x |
| mixed.json | json | 1.423 | 1.444 | 1.479 | 169.156 | 0.19x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.178 | 0.184 | 0.206 | 674.594 | 1.00x |
| users.json $[*].id | jmespath | 0.805 | 0.819 | 0.948 | 674.594 | 0.23x |
| users.json $[*].id | jsonpath-ng | 12.649 | 12.891 | 13.611 | 674.594 | 0.01x |
| users.json $[*].orders[*].total | strata | 2.484 | 2.550 | 2.940 | 685.438 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 8.200 | 8.376 | 9.028 | 685.438 | 0.30x |
| users.json $[*].orders[*].total | jsonpath-ng | 44.759 | 45.889 | 47.147 | 685.438 | 0.06x |
| users.json $..total | strata | 7.821 | 7.890 | 7.975 | 258.812 | 1.00x |
| users.json $..total | jsonpath-ng | 1240.707 | 1243.920 | 1251.518 | 258.812 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 18.747 | 19.181 | 19.314 | 694.234 | 1.00x |
| users.json $[*].id | orjson+jmespath | 66.462 | 67.805 | 70.654 | 694.234 | 0.28x |
| users.json $[*].id | orjson+jsonpath-ng | 69.064 | 69.972 | 72.159 | 694.234 | 0.27x |
| users.json $[*].orders[*].total | strata | 19.188 | 19.585 | 19.856 | 716.703 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 72.504 | 73.762 | 74.608 | 716.703 | 0.27x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 100.953 | 103.038 | 105.287 | 716.703 | 0.19x |
| users.json $..total | strata | 61.336 | 62.344 | 63.477 | 202.797 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 1293.074 | 1295.822 | 1299.453 | 202.797 | 0.05x |

