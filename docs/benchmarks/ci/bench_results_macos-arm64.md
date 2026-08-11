# Benchmark results - ci-macos-latest

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 16b0a58
- python: 3.12.10
- implementation: CPython
- platform: macOS-26.5.2-arm64-arm-64bit
- machine: arm64
- processor: Apple M1 (Virtual)
- compiler_flags: -std=c++20 -O3 -march=native
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.750 | 14.360 | 24.375 | 64.203 | 1.00x |
| users.json | orjson | 11.876 | 15.241 | 22.038 | 64.203 | 0.94x |
| users.json | msgspec | 12.358 | 16.193 | 22.303 | 64.203 | 0.89x |
| users.json | ujson | 16.735 | 20.079 | 29.415 | 64.203 | 0.72x |
| users.json | pysimdjson | 153.112 | 193.101 | 225.421 | 64.203 | 0.07x |
| users.json | json | 17.568 | 25.803 | 38.575 | 64.203 | 0.56x |
| flat.json | strata | 0.915 | 0.993 | 1.345 | 85.844 | 1.00x |
| flat.json | orjson | 0.835 | 0.900 | 0.973 | 85.844 | 1.10x |
| flat.json | msgspec | 0.750 | 0.833 | 1.370 | 85.844 | 1.19x |
| flat.json | ujson | 1.245 | 1.461 | 1.828 | 85.844 | 0.68x |
| flat.json | pysimdjson | 12.546 | 14.846 | 19.889 | 85.844 | 0.07x |
| flat.json | json | 1.514 | 1.662 | 2.901 | 85.844 | 0.60x |
| nested.json | strata | 0.872 | 1.006 | 1.957 | 85.844 | 1.00x |
| nested.json | orjson | 0.853 | 0.976 | 2.332 | 85.844 | 1.03x |
| nested.json | msgspec | 0.783 | 1.916 | 2.743 | 85.844 | 0.52x |
| nested.json | ujson | 1.314 | 1.904 | 3.861 | 85.844 | 0.53x |
| nested.json | pysimdjson | 11.558 | 14.625 | 23.842 | 85.844 | 0.07x |
| nested.json | json | 1.566 | 1.830 | 4.098 | 85.844 | 0.55x |
| wide_arrays.json | strata | 4.633 | 5.738 | 8.404 | 88.625 | 1.00x |
| wide_arrays.json | orjson | 4.078 | 5.338 | 11.370 | 88.625 | 1.07x |
| wide_arrays.json | msgspec | 4.819 | 6.346 | 11.212 | 88.625 | 0.90x |
| wide_arrays.json | ujson | 6.125 | 8.798 | 12.971 | 88.625 | 0.65x |
| wide_arrays.json | pysimdjson | 83.199 | 100.920 | 137.811 | 88.625 | 0.06x |
| wide_arrays.json | json | 9.014 | 10.152 | 15.028 | 88.625 | 0.57x |
| mixed.json | strata | 0.224 | 0.253 | 0.672 | 88.641 | 1.00x |
| mixed.json | orjson | 0.195 | 0.221 | 0.389 | 88.641 | 1.14x |
| mixed.json | msgspec | 0.210 | 0.261 | 0.551 | 88.641 | 0.97x |
| mixed.json | ujson | 0.269 | 0.381 | 0.483 | 88.641 | 0.66x |
| mixed.json | pysimdjson | 2.854 | 3.193 | 3.966 | 88.641 | 0.08x |
| mixed.json | json | 0.394 | 0.484 | 0.844 | 88.641 | 0.52x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.833 | 3.563 | 8.176 | 74.062 | 1.00x |
| users.json | orjson | 2.495 | 3.268 | 6.426 | 74.062 | 1.09x |
| users.json | msgspec | 3.342 | 3.650 | 4.566 | 74.062 | 0.98x |
| users.json | ujson | 12.900 | 14.535 | 18.585 | 74.062 | 0.25x |
| users.json | json | 19.406 | 22.763 | 25.131 | 74.062 | 0.16x |
| flat.json | strata | 0.319 | 0.340 | 0.833 | 85.844 | 1.00x |
| flat.json | orjson | 0.287 | 0.338 | 0.828 | 85.844 | 1.00x |
| flat.json | msgspec | 0.373 | 0.537 | 0.581 | 85.844 | 0.63x |
| flat.json | ujson | 1.000 | 1.161 | 1.640 | 85.844 | 0.29x |
| flat.json | json | 1.513 | 1.813 | 3.291 | 85.844 | 0.19x |
| nested.json | strata | 0.226 | 0.249 | 0.282 | 85.859 | 1.00x |
| nested.json | orjson | 0.227 | 0.256 | 0.440 | 85.859 | 0.97x |
| nested.json | msgspec | 0.335 | 0.438 | 0.485 | 85.859 | 0.57x |
| nested.json | ujson | 1.118 | 1.256 | 2.314 | 85.859 | 0.20x |
| nested.json | json | 1.693 | 1.870 | 2.208 | 85.859 | 0.13x |
| wide_arrays.json | strata | 1.416 | 1.666 | 4.305 | 88.625 | 1.00x |
| wide_arrays.json | orjson | 1.430 | 1.702 | 2.889 | 88.625 | 0.98x |
| wide_arrays.json | msgspec | 2.209 | 2.642 | 5.608 | 88.625 | 0.63x |
| wide_arrays.json | ujson | 7.038 | 9.143 | 12.821 | 88.625 | 0.18x |
| wide_arrays.json | json | 12.544 | 15.107 | 21.966 | 88.625 | 0.11x |
| mixed.json | strata | 0.066 | 0.081 | 0.100 | 88.641 | 1.00x |
| mixed.json | orjson | 0.050 | 0.059 | 0.070 | 88.641 | 1.36x |
| mixed.json | msgspec | 0.062 | 0.073 | 0.302 | 88.641 | 1.10x |
| mixed.json | ujson | 0.241 | 0.264 | 0.334 | 88.641 | 0.31x |
| mixed.json | json | 0.397 | 0.422 | 0.567 | 88.641 | 0.19x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.533 | 13.824 | 23.474 | 84.625 | 1.00x |
| users.json | orjson | 13.000 | 14.839 | 35.821 | 84.625 | 0.93x |
| users.json | msgspec | 12.904 | 19.233 | 27.323 | 84.625 | 0.72x |
| users.json | ujson | 15.562 | 20.477 | 36.900 | 84.625 | 0.68x |
| users.json | json | 18.177 | 21.513 | 31.480 | 84.625 | 0.64x |
| flat.json | strata | 1.071 | 1.157 | 2.243 | 85.844 | 1.00x |
| flat.json | orjson | 1.205 | 1.298 | 2.804 | 85.844 | 0.89x |
| flat.json | msgspec | 1.049 | 1.151 | 2.325 | 85.844 | 1.00x |
| flat.json | ujson | 1.394 | 1.585 | 1.772 | 85.844 | 0.73x |
| flat.json | json | 1.743 | 1.954 | 2.757 | 85.844 | 0.59x |
| nested.json | strata | 0.984 | 1.192 | 2.449 | 85.859 | 1.00x |
| nested.json | orjson | 1.162 | 1.316 | 2.638 | 85.859 | 0.91x |
| nested.json | msgspec | 0.945 | 1.273 | 2.535 | 85.859 | 0.94x |
| nested.json | ujson | 1.387 | 1.690 | 3.847 | 85.859 | 0.71x |
| nested.json | json | 1.760 | 2.343 | 4.085 | 85.859 | 0.51x |
| wide_arrays.json | strata | 4.803 | 5.291 | 8.376 | 88.625 | 1.00x |
| wide_arrays.json | orjson | 4.196 | 5.016 | 8.338 | 88.625 | 1.05x |
| wide_arrays.json | msgspec | 4.657 | 5.579 | 12.992 | 88.625 | 0.95x |
| wide_arrays.json | ujson | 6.303 | 8.806 | 17.724 | 88.625 | 0.60x |
| wide_arrays.json | json | 8.368 | 10.541 | 15.116 | 88.625 | 0.50x |
| mixed.json | strata | 0.330 | 0.395 | 0.735 | 88.641 | 1.00x |
| mixed.json | orjson | 0.321 | 0.371 | 0.579 | 88.641 | 1.06x |
| mixed.json | msgspec | 0.332 | 0.442 | 0.651 | 88.641 | 0.89x |
| mixed.json | ujson | 0.380 | 0.492 | 1.001 | 88.641 | 0.80x |
| mixed.json | json | 0.509 | 0.601 | 0.860 | 88.641 | 0.66x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.214 | 14.268 | 20.744 | 85.828 | 1.00x |
| users.ndjson | orjson | 12.943 | 16.800 | 28.539 | 85.828 | 0.85x |
| users.ndjson | msgspec | 12.548 | 15.983 | 23.681 | 85.828 | 0.89x |
| users.ndjson | ujson | 16.325 | 19.932 | 27.852 | 85.828 | 0.72x |
| users.ndjson | json | 23.054 | 27.831 | 31.405 | 85.828 | 0.51x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.943 | 7.820 | 52.159 | 81.578 | 1.00x |
| users.json | orjson | 3.631 | 7.088 | 17.760 | 81.578 | 1.10x |
| users.json | msgspec | 4.324 | 6.257 | 17.442 | 81.578 | 1.25x |
| users.json | ujson | 15.508 | 22.537 | 47.464 | 81.578 | 0.35x |
| users.json | json | 23.749 | 29.905 | 129.649 | 81.578 | 0.26x |
| flat.json | strata | 0.558 | 0.634 | 0.811 | 85.844 | 1.00x |
| flat.json | orjson | 0.519 | 0.653 | 6.506 | 85.844 | 0.97x |
| flat.json | msgspec | 0.646 | 0.766 | 6.431 | 85.844 | 0.83x |
| flat.json | ujson | 1.297 | 1.456 | 12.745 | 85.844 | 0.44x |
| flat.json | json | 1.877 | 2.174 | 2.803 | 85.844 | 0.29x |
| nested.json | strata | 0.502 | 0.631 | 1.052 | 85.859 | 1.00x |
| nested.json | orjson | 0.492 | 0.660 | 1.554 | 85.859 | 0.95x |
| nested.json | msgspec | 0.619 | 0.755 | 1.580 | 85.859 | 0.84x |
| nested.json | ujson | 1.566 | 1.903 | 11.544 | 85.859 | 0.33x |
| nested.json | json | 2.103 | 2.898 | 5.186 | 85.859 | 0.22x |
| wide_arrays.json | strata | 1.724 | 2.050 | 3.388 | 88.625 | 1.00x |
| wide_arrays.json | orjson | 1.850 | 2.217 | 4.891 | 88.625 | 0.92x |
| wide_arrays.json | msgspec | 2.755 | 3.095 | 3.491 | 88.625 | 0.66x |
| wide_arrays.json | ujson | 7.903 | 9.103 | 19.160 | 88.625 | 0.23x |
| wide_arrays.json | json | 13.321 | 16.890 | 21.286 | 88.625 | 0.12x |
| mixed.json | strata | 0.323 | 0.443 | 0.866 | 88.641 | 1.00x |
| mixed.json | orjson | 0.373 | 0.481 | 0.876 | 88.641 | 0.92x |
| mixed.json | msgspec | 0.385 | 0.580 | 1.059 | 88.641 | 0.76x |
| mixed.json | ujson | 0.578 | 0.674 | 1.397 | 88.641 | 0.66x |
| mixed.json | json | 0.782 | 0.959 | 1.381 | 88.641 | 0.46x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.119 | 0.198 | 0.356 | 81.594 | 1.00x |
| users.json $[*].id | jmespath | 0.424 | 0.890 | 1.970 | 81.594 | 0.22x |
| users.json $[*].id | jsonpath-ng | 2.057 | 3.243 | 6.513 | 81.594 | 0.06x |
| users.json $[*].orders[*].total | strata | 0.679 | 0.784 | 1.420 | 81.719 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.985 | 2.468 | 3.958 | 81.719 | 0.32x |
| users.json $[*].orders[*].total | jsonpath-ng | 14.217 | 16.263 | 17.351 | 81.719 | 0.05x |
| users.json $..total | strata | 1.738 | 2.263 | 3.576 | 82.625 | 1.00x |
| users.json $..total | jsonpath-ng | 258.005 | 310.476 | 358.344 | 82.625 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.960 | 5.845 | 7.717 | 81.641 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.588 | 14.487 | 17.891 | 81.641 | 0.40x |
| users.json $[*].id | orjson+jsonpath-ng | 14.979 | 16.866 | 26.266 | 81.641 | 0.35x |
| users.json $[*].orders[*].total | strata | 4.744 | 5.806 | 10.250 | 82.578 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.893 | 16.514 | 19.892 | 82.578 | 0.35x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 27.675 | 36.474 | 59.425 | 82.578 | 0.16x |
| users.json $..total | strata | 14.034 | 18.701 | 24.648 | 82.609 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 250.138 | 330.280 | 358.650 | 82.609 | 0.06x |

