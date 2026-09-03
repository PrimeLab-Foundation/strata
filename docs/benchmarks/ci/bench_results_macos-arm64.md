# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 1e1e47c
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
| users.json | strata | 7.294 | 8.795 | 14.140 | 63.594 | 1.00x |
| users.json | orjson | 10.187 | 14.026 | 18.138 | 63.594 | 0.63x |
| users.json | msgspec | 9.401 | 13.806 | 20.863 | 63.594 | 0.64x |
| users.json | ujson | 14.608 | 18.296 | 22.457 | 63.594 | 0.48x |
| users.json | pysimdjson | 142.831 | 172.415 | 212.720 | 63.594 | 0.05x |
| users.json | json | 16.662 | 21.663 | 32.599 | 63.594 | 0.41x |
| flat.json | strata | 0.635 | 0.938 | 1.501 | 85.219 | 1.00x |
| flat.json | orjson | 0.818 | 1.162 | 2.423 | 85.219 | 0.81x |
| flat.json | msgspec | 0.767 | 0.864 | 3.491 | 85.219 | 1.09x |
| flat.json | ujson | 1.307 | 1.652 | 5.551 | 85.219 | 0.57x |
| flat.json | pysimdjson | 14.226 | 19.308 | 29.241 | 85.219 | 0.05x |
| flat.json | json | 1.417 | 1.836 | 3.519 | 85.219 | 0.51x |
| nested.json | strata | 0.607 | 0.692 | 1.388 | 85.250 | 1.00x |
| nested.json | orjson | 0.875 | 1.139 | 2.316 | 85.250 | 0.61x |
| nested.json | msgspec | 0.758 | 0.830 | 1.267 | 85.250 | 0.83x |
| nested.json | ujson | 1.112 | 1.483 | 5.275 | 85.250 | 0.47x |
| nested.json | pysimdjson | 12.387 | 14.525 | 20.090 | 85.250 | 0.05x |
| nested.json | json | 1.616 | 1.879 | 2.955 | 85.250 | 0.37x |
| wide_arrays.json | strata | 4.096 | 4.951 | 11.413 | 69.859 | 1.00x |
| wide_arrays.json | orjson | 5.144 | 6.395 | 8.829 | 69.859 | 0.77x |
| wide_arrays.json | msgspec | 5.577 | 6.493 | 16.321 | 69.859 | 0.76x |
| wide_arrays.json | ujson | 7.352 | 8.325 | 18.120 | 69.859 | 0.59x |
| wide_arrays.json | pysimdjson | 77.308 | 89.266 | 128.220 | 69.859 | 0.06x |
| wide_arrays.json | json | 8.466 | 10.920 | 16.243 | 69.859 | 0.45x |
| mixed.json | strata | 0.155 | 0.165 | 0.176 | 71.469 | 1.00x |
| mixed.json | orjson | 0.183 | 0.214 | 1.621 | 71.469 | 0.77x |
| mixed.json | msgspec | 0.203 | 0.237 | 0.509 | 71.469 | 0.70x |
| mixed.json | ujson | 0.264 | 0.322 | 0.585 | 71.469 | 0.51x |
| mixed.json | pysimdjson | 2.681 | 2.936 | 7.517 | 71.469 | 0.06x |
| mixed.json | json | 0.374 | 0.415 | 0.562 | 71.469 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.064 | 2.306 | 3.156 | 73.688 | 1.00x |
| users.json | orjson | 2.918 | 3.817 | 5.942 | 73.688 | 0.60x |
| users.json | msgspec | 3.293 | 4.033 | 4.814 | 73.688 | 0.57x |
| users.json | ujson | 12.661 | 15.182 | 16.899 | 73.688 | 0.15x |
| users.json | json | 19.189 | 23.453 | 26.945 | 73.688 | 0.10x |
| flat.json | strata | 0.238 | 0.280 | 1.277 | 85.250 | 1.00x |
| flat.json | orjson | 0.297 | 0.459 | 2.856 | 85.250 | 0.61x |
| flat.json | msgspec | 0.396 | 0.526 | 0.929 | 85.250 | 0.53x |
| flat.json | ujson | 1.053 | 1.268 | 3.762 | 85.250 | 0.22x |
| flat.json | json | 1.509 | 2.402 | 4.061 | 85.250 | 0.12x |
| nested.json | strata | 0.147 | 0.166 | 0.599 | 83.719 | 1.00x |
| nested.json | orjson | 0.239 | 0.297 | 1.283 | 83.719 | 0.56x |
| nested.json | msgspec | 0.317 | 0.472 | 0.892 | 83.719 | 0.35x |
| nested.json | ujson | 1.169 | 2.084 | 2.991 | 83.719 | 0.08x |
| nested.json | json | 1.748 | 2.049 | 4.793 | 83.719 | 0.08x |
| wide_arrays.json | strata | 1.283 | 1.604 | 2.152 | 68.719 | 1.00x |
| wide_arrays.json | orjson | 1.720 | 1.958 | 5.568 | 68.719 | 0.82x |
| wide_arrays.json | msgspec | 2.506 | 2.735 | 3.527 | 68.719 | 0.59x |
| wide_arrays.json | ujson | 7.202 | 8.564 | 11.845 | 68.719 | 0.19x |
| wide_arrays.json | json | 12.858 | 14.766 | 22.328 | 68.719 | 0.11x |
| mixed.json | strata | 0.063 | 0.068 | 0.274 | 71.469 | 1.00x |
| mixed.json | orjson | 0.065 | 0.074 | 0.109 | 71.469 | 0.91x |
| mixed.json | msgspec | 0.077 | 0.093 | 0.471 | 71.469 | 0.73x |
| mixed.json | ujson | 0.249 | 0.344 | 0.758 | 71.469 | 0.20x |
| mixed.json | json | 0.450 | 0.711 | 1.203 | 71.469 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.314 | 12.493 | 39.746 | 85.062 | 1.00x |
| users.json | orjson | 12.059 | 20.146 | 24.825 | 85.062 | 0.62x |
| users.json | msgspec | 12.389 | 20.502 | 47.246 | 85.062 | 0.61x |
| users.json | ujson | 16.490 | 24.937 | 35.461 | 85.062 | 0.50x |
| users.json | json | 22.147 | 31.472 | 67.421 | 85.062 | 0.40x |
| flat.json | strata | 0.764 | 0.847 | 1.921 | 85.250 | 1.00x |
| flat.json | orjson | 1.066 | 1.488 | 2.437 | 85.250 | 0.57x |
| flat.json | msgspec | 0.981 | 1.168 | 2.928 | 85.250 | 0.72x |
| flat.json | ujson | 1.521 | 2.067 | 2.927 | 85.250 | 0.41x |
| flat.json | json | 1.589 | 1.748 | 4.017 | 85.250 | 0.48x |
| nested.json | strata | 0.713 | 1.155 | 3.518 | 74.625 | 1.00x |
| nested.json | orjson | 1.225 | 1.697 | 4.506 | 74.625 | 0.68x |
| nested.json | msgspec | 0.967 | 1.336 | 7.415 | 74.625 | 0.86x |
| nested.json | ujson | 1.300 | 2.147 | 4.145 | 74.625 | 0.54x |
| nested.json | json | 1.700 | 2.013 | 6.004 | 74.625 | 0.57x |
| wide_arrays.json | strata | 4.049 | 4.584 | 7.792 | 68.719 | 1.00x |
| wide_arrays.json | orjson | 4.670 | 5.300 | 8.297 | 68.719 | 0.87x |
| wide_arrays.json | msgspec | 5.112 | 6.163 | 10.397 | 68.719 | 0.74x |
| wide_arrays.json | ujson | 6.606 | 7.616 | 10.051 | 68.719 | 0.60x |
| wide_arrays.json | json | 7.875 | 9.084 | 12.918 | 68.719 | 0.50x |
| mixed.json | strata | 0.224 | 0.261 | 0.405 | 71.469 | 1.00x |
| mixed.json | orjson | 0.309 | 0.485 | 0.774 | 71.469 | 0.54x |
| mixed.json | msgspec | 0.330 | 0.398 | 0.877 | 71.469 | 0.65x |
| mixed.json | ujson | 0.395 | 0.546 | 1.114 | 71.469 | 0.48x |
| mixed.json | json | 0.531 | 0.699 | 1.581 | 71.469 | 0.37x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 8.235 | 12.008 | 15.289 | 85.203 | 1.00x |
| users.ndjson | orjson | 13.202 | 16.117 | 28.110 | 85.203 | 0.75x |
| users.ndjson | msgspec | 13.382 | 17.686 | 26.065 | 85.203 | 0.68x |
| users.ndjson | ujson | 17.927 | 24.387 | 35.973 | 85.203 | 0.49x |
| users.ndjson | json | 25.654 | 34.697 | 51.145 | 85.203 | 0.35x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.321 | 3.009 | 7.020 | 82.500 | 1.00x |
| users.json | orjson | 3.556 | 3.795 | 6.799 | 82.500 | 0.79x |
| users.json | msgspec | 3.935 | 4.375 | 7.408 | 82.500 | 0.69x |
| users.json | ujson | 13.155 | 15.301 | 21.756 | 82.500 | 0.20x |
| users.json | json | 18.896 | 21.724 | 28.968 | 82.500 | 0.14x |
| flat.json | strata | 0.573 | 0.719 | 2.049 | 85.250 | 1.00x |
| flat.json | orjson | 0.676 | 0.784 | 1.725 | 85.250 | 0.92x |
| flat.json | msgspec | 0.837 | 0.901 | 1.803 | 85.250 | 0.80x |
| flat.json | ujson | 1.530 | 1.666 | 2.141 | 85.250 | 0.43x |
| flat.json | json | 2.034 | 2.441 | 3.749 | 85.250 | 0.29x |
| nested.json | strata | 0.444 | 0.663 | 7.016 | 73.562 | 1.00x |
| nested.json | orjson | 0.624 | 1.644 | 14.293 | 73.562 | 0.40x |
| nested.json | msgspec | 0.548 | 1.064 | 7.164 | 73.562 | 0.62x |
| nested.json | ujson | 1.464 | 4.028 | 7.260 | 73.562 | 0.16x |
| nested.json | json | 2.198 | 4.542 | 20.097 | 73.562 | 0.15x |
| wide_arrays.json | strata | 2.086 | 2.601 | 4.138 | 71.453 | 1.00x |
| wide_arrays.json | orjson | 2.426 | 2.828 | 3.110 | 71.453 | 0.92x |
| wide_arrays.json | msgspec | 3.152 | 3.711 | 5.402 | 71.453 | 0.70x |
| wide_arrays.json | ujson | 9.372 | 10.198 | 17.137 | 71.453 | 0.26x |
| wide_arrays.json | json | 14.927 | 16.397 | 21.700 | 71.453 | 0.16x |
| mixed.json | strata | 0.281 | 0.328 | 0.576 | 71.469 | 1.00x |
| mixed.json | orjson | 0.283 | 0.381 | 0.593 | 71.469 | 0.86x |
| mixed.json | msgspec | 0.337 | 0.431 | 0.848 | 71.469 | 0.76x |
| mixed.json | ujson | 0.516 | 0.628 | 0.802 | 71.469 | 0.52x |
| mixed.json | json | 0.642 | 0.755 | 0.982 | 71.469 | 0.43x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.109 | 0.165 | 0.342 | 82.531 | 1.00x |
| users.json $[*].id | jmespath | 0.380 | 0.468 | 0.707 | 82.531 | 0.35x |
| users.json $[*].id | jsonpath-ng | 1.703 | 1.970 | 4.484 | 82.531 | 0.08x |
| users.json $[*].orders[*].total | strata | 0.634 | 0.863 | 1.859 | 82.719 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.163 | 2.605 | 3.179 | 82.719 | 0.33x |
| users.json $[*].orders[*].total | jsonpath-ng | 12.846 | 15.256 | 26.904 | 82.719 | 0.06x |
| users.json $..total | strata | 1.423 | 1.778 | 2.726 | 83.578 | 1.00x |
| users.json $..total | jsonpath-ng | 251.580 | 286.957 | 315.392 | 83.578 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.998 | 4.699 | 7.177 | 82.609 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.662 | 14.160 | 21.198 | 82.609 | 0.33x |
| users.json $[*].id | orjson+jsonpath-ng | 13.877 | 17.213 | 20.880 | 82.609 | 0.27x |
| users.json $[*].orders[*].total | strata | 4.422 | 4.865 | 6.298 | 83.516 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 14.579 | 18.357 | 21.442 | 83.516 | 0.27x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 31.782 | 40.708 | 47.953 | 83.516 | 0.12x |
| users.json $..total | strata | 10.493 | 12.471 | 15.108 | 83.625 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 258.494 | 306.173 | 473.232 | 83.625 | 0.04x |

