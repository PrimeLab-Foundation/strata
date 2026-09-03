# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c88786c
- python: 3.12.10
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit
- machine: arm64
- processor: Apple M1 (Virtual)
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.694 | 8.728 | 11.090 | 63.625 | 1.00x |
| users.json | orjson | 11.302 | 13.093 | 14.315 | 63.625 | 0.67x |
| users.json | msgspec | 10.982 | 11.835 | 13.057 | 63.625 | 0.74x |
| users.json | ujson | 14.719 | 16.181 | 17.355 | 63.625 | 0.54x |
| users.json | pysimdjson | 152.373 | 157.675 | 172.106 | 63.625 | 0.06x |
| users.json | json | 18.846 | 20.620 | 25.913 | 63.625 | 0.42x |
| flat.json | strata | 0.603 | 0.617 | 0.754 | 90.031 | 1.00x |
| flat.json | orjson | 0.759 | 0.771 | 1.165 | 90.031 | 0.80x |
| flat.json | msgspec | 0.708 | 0.763 | 0.967 | 90.031 | 0.81x |
| flat.json | ujson | 1.170 | 1.224 | 2.036 | 90.031 | 0.50x |
| flat.json | pysimdjson | 12.136 | 12.375 | 15.785 | 90.031 | 0.05x |
| flat.json | json | 1.342 | 1.398 | 1.794 | 90.031 | 0.44x |
| nested.json | strata | 0.551 | 0.626 | 0.834 | 90.047 | 1.00x |
| nested.json | orjson | 0.763 | 0.871 | 1.884 | 90.047 | 0.72x |
| nested.json | msgspec | 0.700 | 0.811 | 1.548 | 90.047 | 0.77x |
| nested.json | ujson | 1.135 | 1.344 | 1.666 | 90.047 | 0.47x |
| nested.json | pysimdjson | 12.308 | 13.630 | 34.836 | 90.047 | 0.05x |
| nested.json | json | 1.473 | 1.738 | 3.064 | 90.047 | 0.36x |
| wide_arrays.json | strata | 3.545 | 5.658 | 10.498 | 92.781 | 1.00x |
| wide_arrays.json | orjson | 4.366 | 5.752 | 9.357 | 92.781 | 0.98x |
| wide_arrays.json | msgspec | 4.338 | 6.497 | 10.676 | 92.781 | 0.87x |
| wide_arrays.json | ujson | 6.275 | 9.605 | 13.086 | 92.781 | 0.59x |
| wide_arrays.json | pysimdjson | 85.725 | 107.760 | 149.323 | 92.781 | 0.05x |
| wide_arrays.json | json | 8.103 | 13.173 | 20.574 | 92.781 | 0.43x |
| mixed.json | strata | 0.144 | 0.174 | 0.341 | 92.844 | 1.00x |
| mixed.json | orjson | 0.183 | 0.211 | 0.518 | 92.844 | 0.82x |
| mixed.json | msgspec | 0.180 | 0.219 | 0.545 | 92.844 | 0.79x |
| mixed.json | ujson | 0.224 | 0.504 | 0.547 | 92.844 | 0.35x |
| mixed.json | pysimdjson | 2.566 | 3.208 | 5.668 | 92.844 | 0.05x |
| mixed.json | json | 0.343 | 0.423 | 1.011 | 92.844 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.039 | 2.217 | 4.540 | 75.969 | 1.00x |
| users.json | orjson | 2.637 | 3.103 | 3.591 | 75.969 | 0.71x |
| users.json | msgspec | 3.438 | 3.631 | 4.147 | 75.969 | 0.61x |
| users.json | ujson | 13.071 | 14.122 | 15.442 | 75.969 | 0.16x |
| users.json | json | 17.797 | 19.726 | 20.759 | 75.969 | 0.11x |
| flat.json | strata | 0.223 | 0.254 | 0.407 | 90.047 | 1.00x |
| flat.json | orjson | 0.293 | 0.334 | 0.625 | 90.047 | 0.76x |
| flat.json | msgspec | 0.341 | 0.383 | 0.547 | 90.047 | 0.66x |
| flat.json | ujson | 0.975 | 1.090 | 2.553 | 90.047 | 0.23x |
| flat.json | json | 1.497 | 1.646 | 3.923 | 90.047 | 0.15x |
| nested.json | strata | 0.147 | 0.155 | 0.188 | 90.047 | 1.00x |
| nested.json | orjson | 0.239 | 0.257 | 0.321 | 90.047 | 0.60x |
| nested.json | msgspec | 0.301 | 0.413 | 0.604 | 90.047 | 0.38x |
| nested.json | ujson | 1.062 | 1.247 | 2.578 | 90.047 | 0.12x |
| nested.json | json | 1.690 | 1.790 | 1.902 | 90.047 | 0.09x |
| wide_arrays.json | strata | 1.272 | 1.511 | 3.669 | 92.781 | 1.00x |
| wide_arrays.json | orjson | 1.559 | 1.971 | 3.124 | 92.781 | 0.77x |
| wide_arrays.json | msgspec | 2.215 | 3.339 | 4.846 | 92.781 | 0.45x |
| wide_arrays.json | ujson | 7.201 | 9.881 | 20.390 | 92.781 | 0.15x |
| wide_arrays.json | json | 12.024 | 20.260 | 26.174 | 92.781 | 0.07x |
| mixed.json | strata | 0.051 | 0.063 | 0.094 | 92.844 | 1.00x |
| mixed.json | orjson | 0.049 | 0.065 | 0.080 | 92.844 | 0.97x |
| mixed.json | msgspec | 0.060 | 0.082 | 0.233 | 92.844 | 0.77x |
| mixed.json | ujson | 0.230 | 0.288 | 0.940 | 92.844 | 0.22x |
| mixed.json | json | 0.365 | 0.422 | 1.501 | 92.844 | 0.15x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.648 | 9.433 | 10.926 | 87.984 | 1.00x |
| users.json | orjson | 10.877 | 12.918 | 14.725 | 87.984 | 0.73x |
| users.json | msgspec | 11.872 | 12.821 | 13.439 | 87.984 | 0.74x |
| users.json | ujson | 16.158 | 17.445 | 20.935 | 87.984 | 0.54x |
| users.json | json | 18.638 | 20.749 | 22.115 | 87.984 | 0.45x |
| flat.json | strata | 0.708 | 0.864 | 1.115 | 90.047 | 1.00x |
| flat.json | orjson | 1.035 | 1.152 | 1.665 | 90.047 | 0.75x |
| flat.json | msgspec | 0.855 | 0.970 | 2.544 | 90.047 | 0.89x |
| flat.json | ujson | 1.248 | 1.440 | 3.949 | 90.047 | 0.60x |
| flat.json | json | 1.440 | 1.652 | 4.176 | 90.047 | 0.52x |
| nested.json | strata | 0.580 | 0.708 | 0.916 | 90.047 | 1.00x |
| nested.json | orjson | 0.964 | 1.166 | 1.538 | 90.047 | 0.61x |
| nested.json | msgspec | 0.840 | 0.903 | 1.393 | 90.047 | 0.78x |
| nested.json | ujson | 1.133 | 1.328 | 1.586 | 90.047 | 0.53x |
| nested.json | json | 1.571 | 1.856 | 2.650 | 90.047 | 0.38x |
| wide_arrays.json | strata | 4.038 | 4.580 | 7.802 | 92.781 | 1.00x |
| wide_arrays.json | orjson | 4.482 | 6.084 | 7.996 | 92.781 | 0.75x |
| wide_arrays.json | msgspec | 4.693 | 6.152 | 10.606 | 92.781 | 0.74x |
| wide_arrays.json | ujson | 7.208 | 8.277 | 11.343 | 92.781 | 0.55x |
| wide_arrays.json | json | 7.774 | 9.618 | 12.693 | 92.781 | 0.48x |
| mixed.json | strata | 0.169 | 0.293 | 0.641 | 92.844 | 1.00x |
| mixed.json | orjson | 0.362 | 0.420 | 0.757 | 92.844 | 0.70x |
| mixed.json | msgspec | 0.234 | 0.413 | 0.865 | 92.844 | 0.71x |
| mixed.json | ujson | 0.272 | 0.497 | 1.492 | 92.844 | 0.59x |
| mixed.json | json | 0.388 | 0.579 | 1.309 | 92.844 | 0.51x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 8.072 | 10.074 | 15.068 | 90.016 | 1.00x |
| users.ndjson | orjson | 12.893 | 15.417 | 21.997 | 90.016 | 0.65x |
| users.ndjson | msgspec | 12.615 | 15.594 | 25.778 | 90.016 | 0.65x |
| users.ndjson | ujson | 14.916 | 18.084 | 34.323 | 90.016 | 0.56x |
| users.ndjson | json | 18.679 | 21.347 | 34.430 | 90.016 | 0.47x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.653 | 3.081 | 4.018 | 83.297 | 1.00x |
| users.json | orjson | 3.319 | 3.775 | 10.808 | 83.297 | 0.82x |
| users.json | msgspec | 4.133 | 4.453 | 8.388 | 83.297 | 0.69x |
| users.json | ujson | 14.198 | 14.914 | 32.275 | 83.297 | 0.21x |
| users.json | json | 20.525 | 23.248 | 35.600 | 83.297 | 0.13x |
| flat.json | strata | 0.481 | 0.716 | 6.314 | 90.047 | 1.00x |
| flat.json | orjson | 0.602 | 0.697 | 5.779 | 90.047 | 1.03x |
| flat.json | msgspec | 0.513 | 0.770 | 7.848 | 90.047 | 0.93x |
| flat.json | ujson | 1.096 | 2.032 | 7.805 | 90.047 | 0.35x |
| flat.json | json | 1.885 | 3.867 | 13.456 | 90.047 | 0.19x |
| nested.json | strata | 0.258 | 0.283 | 0.459 | 90.047 | 1.00x |
| nested.json | orjson | 0.356 | 0.406 | 0.568 | 90.047 | 0.70x |
| nested.json | msgspec | 0.415 | 0.568 | 0.853 | 90.047 | 0.50x |
| nested.json | ujson | 1.208 | 1.329 | 1.978 | 90.047 | 0.21x |
| nested.json | json | 1.796 | 1.852 | 2.448 | 90.047 | 0.15x |
| wide_arrays.json | strata | 1.674 | 2.566 | 4.736 | 92.828 | 1.00x |
| wide_arrays.json | orjson | 2.037 | 2.605 | 5.540 | 92.828 | 0.99x |
| wide_arrays.json | msgspec | 2.792 | 3.692 | 4.842 | 92.828 | 0.70x |
| wide_arrays.json | ujson | 7.663 | 10.691 | 15.585 | 92.828 | 0.24x |
| wide_arrays.json | json | 13.182 | 14.757 | 21.547 | 92.828 | 0.17x |
| mixed.json | strata | 0.303 | 0.358 | 0.671 | 92.844 | 1.00x |
| mixed.json | orjson | 0.199 | 0.355 | 0.564 | 92.844 | 1.01x |
| mixed.json | msgspec | 0.187 | 0.417 | 0.664 | 92.844 | 0.86x |
| mixed.json | ujson | 0.364 | 0.681 | 1.041 | 92.844 | 0.53x |
| mixed.json | json | 0.478 | 0.825 | 1.697 | 92.844 | 0.43x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.156 | 0.218 | 0.361 | 83.359 | 1.00x |
| users.json $[*].id | jmespath | 0.415 | 0.525 | 1.232 | 83.359 | 0.42x |
| users.json $[*].id | jsonpath-ng | 1.903 | 2.579 | 3.615 | 83.359 | 0.08x |
| users.json $[*].orders[*].total | strata | 0.557 | 0.699 | 2.101 | 84.547 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.015 | 2.416 | 4.835 | 84.547 | 0.29x |
| users.json $[*].orders[*].total | jsonpath-ng | 12.144 | 14.708 | 15.662 | 84.547 | 0.05x |
| users.json $..total | strata | 1.259 | 1.626 | 2.421 | 86.391 | 1.00x |
| users.json $..total | jsonpath-ng | 206.889 | 249.082 | 295.376 | 86.391 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.214 | 4.530 | 6.235 | 84.438 | 1.00x |
| users.json $[*].id | orjson+jmespath | 11.565 | 13.234 | 18.352 | 84.438 | 0.34x |
| users.json $[*].id | orjson+jsonpath-ng | 13.419 | 14.825 | 17.816 | 84.438 | 0.31x |
| users.json $[*].orders[*].total | strata | 4.353 | 4.981 | 6.964 | 86.359 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 13.935 | 16.692 | 21.895 | 86.359 | 0.30x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 30.769 | 31.693 | 57.718 | 86.359 | 0.16x |
| users.json $..total | strata | 10.223 | 11.240 | 18.301 | 86.406 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 250.913 | 273.744 | 421.805 | 86.406 | 0.04x |

