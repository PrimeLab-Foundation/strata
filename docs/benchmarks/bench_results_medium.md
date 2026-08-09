# Benchmark results - medium

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: a1b3831
- python: 3.14.3
- implementation: CPython
- platform: macOS-26.3-arm64-arm-64bit-Mach-O
- machine: arm64
- processor: Apple M1 Max
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 60.563 | 62.520 | 66.405 | 139.266 | 1.00x |
| users.json | orjson | 63.885 | 66.793 | 69.949 | 139.266 | 0.94x |
| users.json | msgspec | 64.702 | 68.042 | 73.073 | 139.266 | 0.92x |
| users.json | ujson | 88.886 | 94.270 | 105.078 | 139.266 | 0.66x |
| users.json | json | 114.690 | 118.762 | 120.361 | 139.266 | 0.53x |
| flat.json | strata | 3.975 | 4.010 | 4.227 | 180.141 | 1.00x |
| flat.json | orjson | 4.708 | 4.770 | 5.198 | 180.141 | 0.84x |
| flat.json | msgspec | 4.647 | 4.679 | 5.250 | 180.141 | 0.86x |
| flat.json | ujson | 6.541 | 6.596 | 6.925 | 180.141 | 0.61x |
| flat.json | json | 8.616 | 8.769 | 9.343 | 180.141 | 0.46x |
| nested.json | strata | 3.126 | 3.137 | 3.298 | 180.969 | 1.00x |
| nested.json | orjson | 3.912 | 3.986 | 4.193 | 180.969 | 0.79x |
| nested.json | msgspec | 3.930 | 3.985 | 4.469 | 180.969 | 0.79x |
| nested.json | ujson | 5.300 | 5.350 | 5.556 | 180.969 | 0.59x |
| nested.json | json | 7.638 | 7.701 | 7.916 | 180.969 | 0.41x |
| wide_arrays.json | strata | 18.840 | 18.986 | 19.901 | 208.344 | 1.00x |
| wide_arrays.json | orjson | 16.974 | 17.064 | 18.175 | 208.344 | 1.11x |
| wide_arrays.json | msgspec | 20.747 | 20.905 | 21.915 | 208.344 | 0.91x |
| wide_arrays.json | ujson | 26.991 | 27.281 | 27.825 | 208.344 | 0.70x |
| wide_arrays.json | json | 39.263 | 39.742 | 40.421 | 208.344 | 0.48x |
| mixed.json | strata | 0.816 | 0.820 | 0.842 | 180.719 | 1.00x |
| mixed.json | orjson | 1.504 | 1.518 | 1.590 | 180.719 | 0.54x |
| mixed.json | msgspec | 1.573 | 1.609 | 1.690 | 180.719 | 0.51x |
| mixed.json | ujson | 1.777 | 1.789 | 1.817 | 180.719 | 0.46x |
| mixed.json | json | 2.368 | 2.389 | 2.629 | 180.719 | 0.34x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 20.932 | 21.056 | 21.202 | 425.938 | 1.00x |
| users.json | orjson | 15.935 | 16.060 | 16.304 | 425.938 | 1.31x |
| users.json | msgspec | 20.865 | 21.120 | 21.558 | 425.938 | 1.00x |
| users.json | ujson | 82.046 | 82.360 | 82.698 | 425.938 | 0.26x |
| users.json | json | 116.039 | 116.486 | 117.276 | 425.938 | 0.18x |
| flat.json | strata | 1.326 | 1.350 | 1.356 | 180.141 | 1.00x |
| flat.json | orjson | 1.128 | 1.137 | 1.243 | 180.141 | 1.19x |
| flat.json | msgspec | 1.499 | 1.503 | 1.535 | 180.141 | 0.90x |
| flat.json | ujson | 4.627 | 4.650 | 4.708 | 180.141 | 0.29x |
| flat.json | json | 6.404 | 6.424 | 6.488 | 180.141 | 0.21x |
| nested.json | strata | 1.072 | 1.105 | 1.199 | 180.984 | 1.00x |
| nested.json | orjson | 1.032 | 1.051 | 1.130 | 180.984 | 1.05x |
| nested.json | msgspec | 1.375 | 1.396 | 1.471 | 180.984 | 0.79x |
| nested.json | ujson | 4.971 | 4.984 | 5.447 | 180.984 | 0.22x |
| nested.json | json | 8.179 | 8.250 | 8.658 | 180.984 | 0.13x |
| wide_arrays.json | strata | 8.260 | 8.319 | 8.484 | 203.453 | 1.00x |
| wide_arrays.json | orjson | 6.264 | 6.319 | 6.810 | 203.453 | 1.32x |
| wide_arrays.json | msgspec | 10.719 | 10.853 | 10.953 | 203.453 | 0.77x |
| wide_arrays.json | ujson | 32.648 | 32.960 | 33.269 | 203.453 | 0.25x |
| wide_arrays.json | json | 56.603 | 57.280 | 57.769 | 203.453 | 0.15x |
| mixed.json | strata | 0.311 | 0.322 | 0.332 | 180.719 | 1.00x |
| mixed.json | orjson | 0.201 | 0.207 | 0.220 | 180.719 | 1.55x |
| mixed.json | msgspec | 0.223 | 0.230 | 0.279 | 180.719 | 1.40x |
| mixed.json | ujson | 1.084 | 1.095 | 1.172 | 180.719 | 0.29x |
| mixed.json | json | 1.689 | 1.721 | 1.817 | 180.719 | 0.19x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 62.024 | 63.912 | 68.521 | 483.359 | 1.00x |
| users.json | orjson | 78.794 | 81.238 | 83.843 | 483.359 | 0.79x |
| users.json | msgspec | 81.351 | 83.537 | 87.039 | 483.359 | 0.77x |
| users.json | ujson | 106.181 | 109.058 | 112.972 | 483.359 | 0.59x |
| users.json | json | 129.979 | 131.449 | 135.854 | 483.359 | 0.49x |
| flat.json | strata | 4.164 | 4.198 | 4.475 | 180.969 | 1.00x |
| flat.json | orjson | 5.230 | 5.387 | 5.540 | 180.969 | 0.78x |
| flat.json | msgspec | 5.157 | 5.340 | 5.795 | 180.969 | 0.79x |
| flat.json | ujson | 7.150 | 7.244 | 7.550 | 180.969 | 0.58x |
| flat.json | json | 9.049 | 9.195 | 9.764 | 180.969 | 0.46x |
| nested.json | strata | 3.306 | 3.322 | 3.476 | 180.984 | 1.00x |
| nested.json | orjson | 4.419 | 4.518 | 4.815 | 180.984 | 0.74x |
| nested.json | msgspec | 4.434 | 4.580 | 4.852 | 180.984 | 0.73x |
| nested.json | ujson | 5.986 | 6.087 | 6.330 | 180.984 | 0.55x |
| nested.json | json | 8.207 | 8.293 | 9.027 | 180.984 | 0.40x |
| wide_arrays.json | strata | 19.356 | 19.468 | 19.814 | 177.453 | 1.00x |
| wide_arrays.json | orjson | 19.568 | 19.825 | 20.955 | 177.453 | 0.98x |
| wide_arrays.json | msgspec | 23.316 | 23.821 | 24.188 | 177.453 | 0.82x |
| wide_arrays.json | ujson | 30.258 | 30.801 | 31.619 | 177.453 | 0.63x |
| wide_arrays.json | json | 41.864 | 42.323 | 42.706 | 177.453 | 0.46x |
| mixed.json | strata | 0.894 | 0.916 | 0.984 | 180.719 | 1.00x |
| mixed.json | orjson | 1.725 | 1.751 | 2.146 | 180.719 | 0.52x |
| mixed.json | msgspec | 1.782 | 1.870 | 2.156 | 180.719 | 0.49x |
| mixed.json | ujson | 2.016 | 2.113 | 2.303 | 180.719 | 0.43x |
| mixed.json | json | 2.587 | 2.745 | 3.026 | 180.719 | 0.33x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 57.293 | 57.666 | 58.425 | 181.906 | 1.00x |
| users.ndjson | orjson | 80.567 | 82.342 | 83.900 | 181.906 | 0.70x |
| users.ndjson | msgspec | 80.373 | 81.474 | 82.834 | 181.906 | 0.71x |
| users.ndjson | ujson | 102.833 | 103.997 | 106.003 | 181.906 | 0.55x |
| users.ndjson | json | 133.489 | 134.203 | 136.636 | 181.906 | 0.43x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 22.726 | 22.926 | 23.159 | 609.156 | 1.00x |
| users.json | orjson | 17.668 | 18.033 | 19.767 | 609.156 | 1.27x |
| users.json | msgspec | 22.664 | 22.982 | 24.413 | 609.156 | 1.00x |
| users.json | ujson | 84.510 | 85.528 | 86.666 | 609.156 | 0.27x |
| users.json | json | 115.261 | 116.309 | 120.348 | 609.156 | 0.20x |
| flat.json | strata | 1.614 | 1.651 | 1.737 | 180.969 | 1.00x |
| flat.json | orjson | 1.373 | 1.403 | 1.423 | 180.969 | 1.18x |
| flat.json | msgspec | 1.745 | 1.778 | 1.938 | 180.969 | 0.93x |
| flat.json | ujson | 4.767 | 4.811 | 5.035 | 180.969 | 0.34x |
| flat.json | json | 6.680 | 6.739 | 6.820 | 180.969 | 0.24x |
| nested.json | strata | 1.316 | 1.380 | 1.544 | 181.094 | 1.00x |
| nested.json | orjson | 1.240 | 1.280 | 1.392 | 181.094 | 1.08x |
| nested.json | msgspec | 1.610 | 1.636 | 1.823 | 181.094 | 0.84x |
| nested.json | ujson | 5.201 | 5.284 | 5.403 | 181.094 | 0.26x |
| nested.json | json | 8.388 | 8.477 | 8.719 | 181.094 | 0.16x |
| wide_arrays.json | strata | 9.306 | 9.433 | 9.708 | 180.719 | 1.00x |
| wide_arrays.json | orjson | 7.234 | 7.413 | 7.887 | 180.719 | 1.27x |
| wide_arrays.json | msgspec | 11.610 | 11.732 | 12.566 | 180.719 | 0.80x |
| wide_arrays.json | ujson | 33.663 | 34.065 | 34.325 | 180.719 | 0.28x |
| wide_arrays.json | json | 57.277 | 57.679 | 58.793 | 180.719 | 0.16x |
| mixed.json | strata | 0.467 | 0.495 | 0.560 | 180.719 | 1.00x |
| mixed.json | orjson | 0.345 | 0.356 | 0.430 | 180.719 | 1.39x |
| mixed.json | msgspec | 0.367 | 0.387 | 0.448 | 180.719 | 1.28x |
| mixed.json | ujson | 1.241 | 1.265 | 1.324 | 180.719 | 0.39x |
| mixed.json | json | 1.835 | 1.867 | 1.943 | 180.719 | 0.27x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.198 | 0.224 | 0.247 | 609.688 | 1.00x |
| users.json $[*].id | jmespath | 0.997 | 1.038 | 1.147 | 609.688 | 0.22x |
| users.json $[*].id | jsonpath-ng | 14.987 | 15.473 | 16.835 | 609.688 | 0.01x |
| users.json $[*].orders[*].total | strata | 2.706 | 2.797 | 3.070 | 508.172 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 10.358 | 10.450 | 11.315 | 508.172 | 0.27x |
| users.json $[*].orders[*].total | jsonpath-ng | 56.424 | 57.274 | 61.856 | 508.172 | 0.05x |
| users.json $..total | strata | 10.128 | 10.227 | 10.533 | 165.391 | 1.00x |
| users.json $..total | jsonpath-ng | 1627.251 | 1628.582 | 1644.466 | 165.391 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 28.183 | 28.637 | 28.978 | 673.766 | 1.00x |
| users.json $[*].id | orjson+jmespath | 80.057 | 81.755 | 82.732 | 673.766 | 0.35x |
| users.json $[*].id | orjson+jsonpath-ng | 84.262 | 85.209 | 86.749 | 673.766 | 0.34x |
| users.json $[*].orders[*].total | strata | 28.518 | 29.126 | 29.333 | 569.266 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 87.883 | 88.704 | 90.770 | 569.266 | 0.33x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 125.586 | 126.616 | 132.443 | 569.266 | 0.23x |
| users.json $..total | strata | 83.578 | 84.061 | 84.666 | 215.141 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 1690.903 | 1692.379 | 1696.124 | 215.141 | 0.05x |

