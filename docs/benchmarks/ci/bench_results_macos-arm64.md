# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: b294ccd
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
| users.json | strata | 6.390 | 6.955 | 8.221 | 63.359 | 1.00x |
| users.json | orjson | 8.682 | 9.657 | 11.527 | 63.359 | 0.72x |
| users.json | msgspec | 8.366 | 9.024 | 11.305 | 63.359 | 0.77x |
| users.json | ujson | 10.975 | 13.049 | 14.709 | 63.359 | 0.53x |
| users.json | pysimdjson | 116.342 | 125.285 | 142.584 | 63.359 | 0.06x |
| users.json | json | 13.688 | 15.941 | 17.943 | 63.359 | 0.44x |
| flat.json | strata | 0.572 | 0.578 | 0.726 | 90.031 | 1.00x |
| flat.json | orjson | 0.676 | 0.710 | 0.786 | 90.031 | 0.81x |
| flat.json | msgspec | 0.674 | 0.681 | 0.864 | 90.031 | 0.85x |
| flat.json | ujson | 1.072 | 1.111 | 1.432 | 90.031 | 0.52x |
| flat.json | pysimdjson | 11.185 | 11.229 | 13.909 | 90.031 | 0.05x |
| flat.json | json | 1.280 | 1.296 | 1.568 | 90.031 | 0.45x |
| nested.json | strata | 0.495 | 0.519 | 0.537 | 90.062 | 1.00x |
| nested.json | orjson | 0.656 | 0.723 | 0.791 | 90.062 | 0.72x |
| nested.json | msgspec | 0.635 | 0.657 | 0.770 | 90.062 | 0.79x |
| nested.json | ujson | 0.998 | 1.134 | 1.319 | 90.062 | 0.46x |
| nested.json | pysimdjson | 9.728 | 10.116 | 10.793 | 90.062 | 0.05x |
| nested.json | json | 1.336 | 1.370 | 1.445 | 90.062 | 0.38x |
| wide_arrays.json | strata | 3.246 | 3.281 | 4.193 | 91.766 | 1.00x |
| wide_arrays.json | orjson | 3.290 | 3.332 | 5.553 | 91.766 | 0.98x |
| wide_arrays.json | msgspec | 3.807 | 3.875 | 4.807 | 91.766 | 0.85x |
| wide_arrays.json | ujson | 5.069 | 5.197 | 6.493 | 91.766 | 0.63x |
| wide_arrays.json | pysimdjson | 60.123 | 60.843 | 67.777 | 91.766 | 0.05x |
| wide_arrays.json | json | 6.373 | 6.493 | 7.881 | 91.766 | 0.51x |
| mixed.json | strata | 0.129 | 0.143 | 0.163 | 92.234 | 1.00x |
| mixed.json | orjson | 0.151 | 0.170 | 0.216 | 92.234 | 0.84x |
| mixed.json | msgspec | 0.166 | 0.183 | 0.202 | 92.234 | 0.78x |
| mixed.json | ujson | 0.244 | 0.355 | 0.384 | 92.234 | 0.40x |
| mixed.json | pysimdjson | 2.476 | 2.685 | 2.730 | 92.234 | 0.05x |
| mixed.json | json | 0.313 | 0.361 | 0.389 | 92.234 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.616 | 1.778 | 2.248 | 76.312 | 1.00x |
| users.json | orjson | 2.154 | 2.340 | 2.658 | 76.312 | 0.76x |
| users.json | msgspec | 2.710 | 2.923 | 3.357 | 76.312 | 0.61x |
| users.json | ujson | 10.332 | 10.911 | 12.150 | 76.312 | 0.16x |
| users.json | json | 14.815 | 15.302 | 17.676 | 76.312 | 0.12x |
| flat.json | strata | 0.183 | 0.187 | 0.203 | 90.031 | 1.00x |
| flat.json | orjson | 0.232 | 0.273 | 0.343 | 90.031 | 0.69x |
| flat.json | msgspec | 0.290 | 0.296 | 0.300 | 90.031 | 0.63x |
| flat.json | ujson | 0.874 | 0.877 | 0.889 | 90.031 | 0.21x |
| flat.json | json | 1.263 | 1.320 | 1.351 | 90.031 | 0.14x |
| nested.json | strata | 0.124 | 0.132 | 0.140 | 90.062 | 1.00x |
| nested.json | orjson | 0.205 | 0.219 | 0.267 | 90.062 | 0.60x |
| nested.json | msgspec | 0.267 | 0.275 | 0.330 | 90.062 | 0.48x |
| nested.json | ujson | 1.005 | 1.045 | 1.229 | 90.062 | 0.13x |
| nested.json | json | 1.523 | 1.556 | 1.670 | 90.062 | 0.09x |
| wide_arrays.json | strata | 1.105 | 1.310 | 1.579 | 91.766 | 1.00x |
| wide_arrays.json | orjson | 1.393 | 1.618 | 1.800 | 91.766 | 0.81x |
| wide_arrays.json | msgspec | 1.985 | 2.297 | 2.426 | 91.766 | 0.57x |
| wide_arrays.json | ujson | 6.508 | 6.958 | 7.798 | 91.766 | 0.19x |
| wide_arrays.json | json | 11.945 | 12.446 | 13.217 | 91.766 | 0.11x |
| mixed.json | strata | 0.038 | 0.040 | 0.047 | 92.234 | 1.00x |
| mixed.json | orjson | 0.042 | 0.042 | 0.046 | 92.234 | 0.95x |
| mixed.json | msgspec | 0.049 | 0.050 | 0.054 | 92.234 | 0.80x |
| mixed.json | ujson | 0.215 | 0.217 | 0.225 | 92.234 | 0.18x |
| mixed.json | json | 0.342 | 0.344 | 0.362 | 92.234 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.887 | 6.969 | 8.769 | 86.828 | 1.00x |
| users.json | orjson | 9.078 | 9.213 | 11.737 | 86.828 | 0.76x |
| users.json | msgspec | 8.835 | 9.311 | 11.421 | 86.828 | 0.75x |
| users.json | ujson | 11.860 | 12.791 | 15.780 | 86.828 | 0.54x |
| users.json | json | 14.470 | 14.635 | 18.191 | 86.828 | 0.48x |
| flat.json | strata | 0.601 | 0.609 | 1.171 | 90.031 | 1.00x |
| flat.json | orjson | 0.745 | 0.800 | 0.932 | 90.031 | 0.76x |
| flat.json | msgspec | 0.723 | 0.735 | 0.789 | 90.031 | 0.83x |
| flat.json | ujson | 1.066 | 1.092 | 1.104 | 90.031 | 0.56x |
| flat.json | json | 1.319 | 1.332 | 1.470 | 90.031 | 0.46x |
| nested.json | strata | 0.535 | 0.593 | 0.678 | 90.062 | 1.00x |
| nested.json | orjson | 0.777 | 1.002 | 1.086 | 90.062 | 0.59x |
| nested.json | msgspec | 0.700 | 0.797 | 0.867 | 90.062 | 0.74x |
| nested.json | ujson | 0.967 | 1.069 | 1.222 | 90.062 | 0.55x |
| nested.json | json | 1.363 | 1.497 | 1.723 | 90.062 | 0.40x |
| wide_arrays.json | strata | 3.437 | 3.923 | 6.805 | 91.766 | 1.00x |
| wide_arrays.json | orjson | 3.552 | 4.084 | 4.821 | 91.766 | 0.96x |
| wide_arrays.json | msgspec | 4.084 | 4.484 | 5.322 | 91.766 | 0.88x |
| wide_arrays.json | ujson | 5.426 | 6.035 | 8.793 | 91.766 | 0.65x |
| wide_arrays.json | json | 6.786 | 7.459 | 10.448 | 91.766 | 0.53x |
| mixed.json | strata | 0.149 | 0.151 | 0.174 | 92.234 | 1.00x |
| mixed.json | orjson | 0.189 | 0.225 | 0.423 | 92.234 | 0.67x |
| mixed.json | msgspec | 0.196 | 0.205 | 0.223 | 92.234 | 0.74x |
| mixed.json | ujson | 0.244 | 0.247 | 0.308 | 92.234 | 0.61x |
| mixed.json | json | 0.345 | 0.350 | 0.426 | 92.234 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.831 | 6.966 | 6.992 | 90.000 | 1.00x |
| users.ndjson | orjson | 10.730 | 10.918 | 10.977 | 90.000 | 0.64x |
| users.ndjson | msgspec | 10.608 | 10.808 | 10.958 | 90.000 | 0.64x |
| users.ndjson | ujson | 13.149 | 13.379 | 13.667 | 90.000 | 0.52x |
| users.ndjson | json | 17.035 | 17.357 | 18.440 | 90.000 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.335 | 2.600 | 2.816 | 85.203 | 1.00x |
| users.json | orjson | 2.872 | 3.372 | 4.538 | 85.203 | 0.77x |
| users.json | msgspec | 3.567 | 3.794 | 4.331 | 85.203 | 0.69x |
| users.json | ujson | 12.022 | 12.881 | 13.743 | 85.203 | 0.20x |
| users.json | json | 16.940 | 17.880 | 20.728 | 85.203 | 0.15x |
| flat.json | strata | 0.297 | 0.383 | 6.759 | 90.031 | 1.00x |
| flat.json | orjson | 0.368 | 0.502 | 7.293 | 90.031 | 0.76x |
| flat.json | msgspec | 0.444 | 0.560 | 0.861 | 90.031 | 0.68x |
| flat.json | ujson | 1.074 | 1.186 | 6.219 | 90.031 | 0.32x |
| flat.json | json | 1.461 | 1.619 | 6.975 | 90.031 | 0.24x |
| nested.json | strata | 0.241 | 0.263 | 0.686 | 90.062 | 1.00x |
| nested.json | orjson | 0.309 | 0.349 | 0.436 | 90.062 | 0.75x |
| nested.json | msgspec | 0.480 | 0.520 | 0.747 | 90.062 | 0.51x |
| nested.json | ujson | 1.050 | 1.095 | 1.525 | 90.062 | 0.24x |
| nested.json | json | 1.640 | 1.713 | 1.904 | 90.062 | 0.15x |
| wide_arrays.json | strata | 1.728 | 1.794 | 2.001 | 92.219 | 1.00x |
| wide_arrays.json | orjson | 2.040 | 2.213 | 2.581 | 92.219 | 0.81x |
| wide_arrays.json | msgspec | 2.657 | 2.967 | 3.208 | 92.219 | 0.60x |
| wide_arrays.json | ujson | 7.863 | 8.634 | 9.161 | 92.219 | 0.21x |
| wide_arrays.json | json | 13.663 | 14.529 | 15.599 | 92.219 | 0.12x |
| mixed.json | strata | 0.128 | 0.134 | 0.238 | 92.234 | 1.00x |
| mixed.json | orjson | 0.138 | 0.195 | 0.291 | 92.234 | 0.69x |
| mixed.json | msgspec | 0.140 | 0.155 | 0.301 | 92.234 | 0.87x |
| mixed.json | ujson | 0.319 | 0.339 | 0.368 | 92.234 | 0.40x |
| mixed.json | json | 0.430 | 0.443 | 0.493 | 92.234 | 0.30x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.045 | 0.049 | 0.102 | 85.234 | 1.00x |
| users.json $[*].id | jmespath | 0.259 | 0.269 | 0.312 | 85.234 | 0.18x |
| users.json $[*].id | jsonpath-ng | 1.412 | 1.462 | 1.893 | 85.234 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.354 | 0.370 | 0.416 | 85.438 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.759 | 1.855 | 3.734 | 85.438 | 0.20x |
| users.json $[*].orders[*].total | jsonpath-ng | 10.894 | 11.223 | 13.805 | 85.438 | 0.03x |
| users.json $..total | strata | 1.188 | 1.353 | 1.817 | 86.516 | 1.00x |
| users.json $..total | jsonpath-ng | 180.754 | 189.145 | 213.012 | 86.516 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.441 | 3.519 | 3.800 | 85.312 | 1.00x |
| users.json $[*].id | orjson+jmespath | 9.152 | 9.321 | 10.049 | 85.312 | 0.38x |
| users.json $[*].id | orjson+jsonpath-ng | 10.359 | 10.458 | 11.914 | 85.312 | 0.34x |
| users.json $[*].orders[*].total | strata | 3.764 | 3.997 | 6.041 | 86.500 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 11.860 | 12.497 | 14.036 | 86.500 | 0.32x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 23.023 | 24.611 | 28.406 | 86.500 | 0.16x |
| users.json $..total | strata | 8.048 | 8.276 | 9.375 | 86.359 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 190.510 | 194.544 | 211.905 | 86.359 | 0.04x |

