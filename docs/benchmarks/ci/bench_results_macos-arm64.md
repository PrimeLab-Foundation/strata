# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 8cdb525
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
| users.json | strata | 8.025 | 9.357 | 12.221 | 63.719 | 1.00x |
| users.json | orjson | 9.640 | 11.535 | 13.954 | 63.719 | 0.81x |
| users.json | msgspec | 9.265 | 11.149 | 20.105 | 63.719 | 0.84x |
| users.json | ujson | 12.243 | 15.190 | 17.600 | 63.719 | 0.62x |
| users.json | pysimdjson | 137.853 | 146.617 | 155.284 | 63.719 | 0.06x |
| users.json | json | 15.724 | 18.350 | 25.528 | 63.719 | 0.51x |
| flat.json | strata | 0.697 | 0.762 | 1.265 | 87.719 | 1.00x |
| flat.json | orjson | 0.774 | 1.089 | 2.840 | 87.719 | 0.70x |
| flat.json | msgspec | 0.771 | 0.914 | 1.343 | 87.719 | 0.83x |
| flat.json | ujson | 1.199 | 1.413 | 3.357 | 87.719 | 0.54x |
| flat.json | pysimdjson | 12.466 | 15.098 | 21.141 | 87.719 | 0.05x |
| flat.json | json | 1.472 | 1.716 | 2.399 | 87.719 | 0.44x |
| nested.json | strata | 0.676 | 0.754 | 1.509 | 87.750 | 1.00x |
| nested.json | orjson | 0.836 | 0.916 | 1.249 | 87.750 | 0.82x |
| nested.json | msgspec | 0.795 | 0.882 | 1.108 | 87.750 | 0.85x |
| nested.json | ujson | 1.164 | 1.463 | 2.311 | 87.750 | 0.51x |
| nested.json | pysimdjson | 11.980 | 12.556 | 15.967 | 87.750 | 0.06x |
| nested.json | json | 1.758 | 1.864 | 2.942 | 87.750 | 0.40x |
| wide_arrays.json | strata | 3.852 | 4.446 | 7.214 | 90.500 | 1.00x |
| wide_arrays.json | orjson | 3.873 | 4.887 | 9.111 | 90.500 | 0.91x |
| wide_arrays.json | msgspec | 4.632 | 5.394 | 6.387 | 90.500 | 0.82x |
| wide_arrays.json | ujson | 5.920 | 7.287 | 67.925 | 90.500 | 0.61x |
| wide_arrays.json | pysimdjson | 74.778 | 86.381 | 164.828 | 90.500 | 0.05x |
| wide_arrays.json | json | 8.077 | 8.991 | 11.897 | 90.500 | 0.49x |
| mixed.json | strata | 0.156 | 0.186 | 0.490 | 90.516 | 1.00x |
| mixed.json | orjson | 0.188 | 0.202 | 0.376 | 90.516 | 0.92x |
| mixed.json | msgspec | 0.191 | 0.214 | 0.239 | 90.516 | 0.87x |
| mixed.json | ujson | 0.251 | 0.355 | 0.728 | 90.516 | 0.52x |
| mixed.json | pysimdjson | 2.735 | 3.289 | 5.203 | 90.516 | 0.06x |
| mixed.json | json | 0.372 | 0.421 | 1.069 | 90.516 | 0.44x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.870 | 2.052 | 3.508 | 73.844 | 1.00x |
| users.json | orjson | 2.334 | 2.700 | 7.353 | 73.844 | 0.76x |
| users.json | msgspec | 3.111 | 3.362 | 6.749 | 73.844 | 0.61x |
| users.json | ujson | 11.862 | 13.079 | 15.399 | 73.844 | 0.16x |
| users.json | json | 17.585 | 19.345 | 28.804 | 73.844 | 0.11x |
| flat.json | strata | 0.235 | 0.273 | 0.410 | 87.734 | 1.00x |
| flat.json | orjson | 0.292 | 0.347 | 7.158 | 87.734 | 0.79x |
| flat.json | msgspec | 0.379 | 0.434 | 0.584 | 87.734 | 0.63x |
| flat.json | ujson | 1.021 | 1.239 | 2.622 | 87.734 | 0.22x |
| flat.json | json | 1.600 | 1.785 | 2.021 | 87.734 | 0.15x |
| nested.json | strata | 0.165 | 0.183 | 0.416 | 87.750 | 1.00x |
| nested.json | orjson | 0.256 | 0.312 | 0.485 | 87.750 | 0.59x |
| nested.json | msgspec | 0.354 | 0.405 | 0.587 | 87.750 | 0.45x |
| nested.json | ujson | 1.256 | 1.467 | 1.612 | 87.750 | 0.13x |
| nested.json | json | 1.907 | 2.022 | 2.374 | 87.750 | 0.09x |
| wide_arrays.json | strata | 1.381 | 1.648 | 4.516 | 90.500 | 1.00x |
| wide_arrays.json | orjson | 1.587 | 1.778 | 2.663 | 90.500 | 0.93x |
| wide_arrays.json | msgspec | 2.338 | 2.681 | 5.344 | 90.500 | 0.61x |
| wide_arrays.json | ujson | 7.496 | 8.299 | 12.680 | 90.500 | 0.20x |
| wide_arrays.json | json | 13.060 | 14.624 | 23.037 | 90.500 | 0.11x |
| mixed.json | strata | 0.060 | 0.066 | 0.086 | 90.516 | 1.00x |
| mixed.json | orjson | 0.054 | 0.072 | 0.303 | 90.516 | 0.91x |
| mixed.json | msgspec | 0.069 | 0.085 | 0.246 | 90.516 | 0.77x |
| mixed.json | ujson | 0.246 | 0.287 | 0.552 | 90.516 | 0.23x |
| mixed.json | json | 0.405 | 0.449 | 0.514 | 90.516 | 0.15x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.137 | 9.840 | 16.060 | 85.031 | 1.00x |
| users.json | orjson | 10.298 | 13.044 | 21.021 | 85.031 | 0.75x |
| users.json | msgspec | 9.917 | 12.660 | 15.295 | 85.031 | 0.78x |
| users.json | ujson | 14.270 | 17.531 | 24.804 | 85.031 | 0.56x |
| users.json | json | 15.716 | 21.314 | 31.133 | 85.031 | 0.46x |
| flat.json | strata | 0.865 | 0.900 | 1.125 | 87.734 | 1.00x |
| flat.json | orjson | 1.029 | 1.152 | 1.493 | 87.734 | 0.78x |
| flat.json | msgspec | 0.900 | 0.986 | 1.229 | 87.734 | 0.91x |
| flat.json | ujson | 1.397 | 1.468 | 1.531 | 87.734 | 0.61x |
| flat.json | json | 1.596 | 1.772 | 2.709 | 87.734 | 0.51x |
| nested.json | strata | 0.693 | 0.751 | 1.051 | 87.750 | 1.00x |
| nested.json | orjson | 1.013 | 1.171 | 1.383 | 87.750 | 0.64x |
| nested.json | msgspec | 0.830 | 1.023 | 1.506 | 87.750 | 0.73x |
| nested.json | ujson | 1.167 | 1.401 | 1.653 | 87.750 | 0.54x |
| nested.json | json | 1.592 | 1.877 | 2.975 | 87.750 | 0.40x |
| wide_arrays.json | strata | 3.936 | 4.389 | 5.033 | 90.500 | 1.00x |
| wide_arrays.json | orjson | 4.162 | 4.799 | 8.577 | 90.500 | 0.91x |
| wide_arrays.json | msgspec | 4.911 | 5.405 | 7.417 | 90.500 | 0.81x |
| wide_arrays.json | ujson | 6.955 | 7.447 | 8.082 | 90.500 | 0.59x |
| wide_arrays.json | json | 8.023 | 9.182 | 20.750 | 90.500 | 0.48x |
| mixed.json | strata | 0.220 | 0.247 | 0.392 | 90.516 | 1.00x |
| mixed.json | orjson | 0.289 | 0.477 | 0.700 | 90.516 | 0.52x |
| mixed.json | msgspec | 0.298 | 0.330 | 0.412 | 90.516 | 0.75x |
| mixed.json | ujson | 0.355 | 0.392 | 0.697 | 90.516 | 0.63x |
| mixed.json | json | 0.476 | 0.510 | 0.755 | 90.516 | 0.48x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 8.727 | 10.543 | 16.324 | 87.719 | 1.00x |
| users.ndjson | orjson | 13.001 | 15.654 | 19.917 | 87.719 | 0.67x |
| users.ndjson | msgspec | 13.490 | 14.616 | 22.836 | 87.719 | 0.72x |
| users.ndjson | ujson | 16.849 | 19.439 | 26.784 | 87.719 | 0.54x |
| users.ndjson | json | 20.581 | 26.158 | 35.618 | 87.719 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.255 | 2.406 | 2.711 | 81.984 | 1.00x |
| users.json | orjson | 2.841 | 2.931 | 3.381 | 81.984 | 0.82x |
| users.json | msgspec | 3.410 | 3.793 | 4.058 | 81.984 | 0.63x |
| users.json | ujson | 12.063 | 12.518 | 14.349 | 81.984 | 0.19x |
| users.json | json | 16.973 | 17.796 | 21.401 | 81.984 | 0.14x |
| flat.json | strata | 0.412 | 0.482 | 0.898 | 87.734 | 1.00x |
| flat.json | orjson | 0.472 | 0.647 | 1.435 | 87.734 | 0.75x |
| flat.json | msgspec | 0.487 | 0.619 | 1.306 | 87.734 | 0.78x |
| flat.json | ujson | 1.153 | 1.313 | 2.090 | 87.734 | 0.37x |
| flat.json | json | 1.655 | 1.969 | 2.161 | 87.734 | 0.24x |
| nested.json | strata | 0.367 | 0.518 | 1.260 | 87.750 | 1.00x |
| nested.json | orjson | 0.495 | 0.557 | 1.133 | 87.750 | 0.93x |
| nested.json | msgspec | 0.630 | 0.797 | 2.349 | 87.750 | 0.65x |
| nested.json | ujson | 1.519 | 1.600 | 2.881 | 87.750 | 0.32x |
| nested.json | json | 2.061 | 2.305 | 2.544 | 87.750 | 0.22x |
| wide_arrays.json | strata | 1.832 | 2.008 | 8.037 | 90.500 | 1.00x |
| wide_arrays.json | orjson | 2.043 | 2.593 | 4.442 | 90.500 | 0.77x |
| wide_arrays.json | msgspec | 2.830 | 3.486 | 5.076 | 90.500 | 0.58x |
| wide_arrays.json | ujson | 7.866 | 8.946 | 16.092 | 90.500 | 0.22x |
| wide_arrays.json | json | 12.917 | 15.256 | 21.874 | 90.500 | 0.13x |
| mixed.json | strata | 0.228 | 0.256 | 5.570 | 90.516 | 1.00x |
| mixed.json | orjson | 0.268 | 0.307 | 11.529 | 90.516 | 0.83x |
| mixed.json | msgspec | 0.240 | 0.332 | 5.379 | 90.516 | 0.77x |
| mixed.json | ujson | 0.522 | 0.567 | 0.717 | 90.516 | 0.45x |
| mixed.json | json | 0.660 | 0.809 | 1.261 | 90.516 | 0.32x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.090 | 0.121 | 0.332 | 82.047 | 1.00x |
| users.json $[*].id | jmespath | 0.322 | 0.395 | 1.318 | 82.047 | 0.31x |
| users.json $[*].id | jsonpath-ng | 1.666 | 2.221 | 5.130 | 82.047 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.444 | 0.760 | 2.217 | 83.250 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.870 | 2.812 | 15.590 | 83.250 | 0.27x |
| users.json $[*].orders[*].total | jsonpath-ng | 13.617 | 18.434 | 54.156 | 83.250 | 0.04x |
| users.json $..total | strata | 1.370 | 1.738 | 4.179 | 85.109 | 1.00x |
| users.json $..total | jsonpath-ng | 245.428 | 271.908 | 351.950 | 85.109 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.684 | 4.445 | 5.537 | 83.109 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.813 | 12.755 | 17.425 | 83.109 | 0.35x |
| users.json $[*].id | orjson+jsonpath-ng | 12.066 | 13.895 | 17.949 | 83.109 | 0.32x |
| users.json $[*].orders[*].total | strata | 4.025 | 4.695 | 6.499 | 85.062 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.602 | 19.682 | 75.695 | 85.062 | 0.24x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 26.363 | 35.286 | 60.299 | 85.062 | 0.13x |
| users.json $..total | strata | 11.488 | 11.874 | 13.980 | 85.109 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 240.993 | 260.461 | 335.273 | 85.109 | 0.05x |

