# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: a969975
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
| users.json | strata | 6.412 | 6.494 | 7.170 | 63.891 | 1.00x |
| users.json | orjson | 8.671 | 8.728 | 9.809 | 63.891 | 0.74x |
| users.json | msgspec | 8.350 | 8.530 | 9.306 | 63.891 | 0.76x |
| users.json | ujson | 11.060 | 11.215 | 13.101 | 63.891 | 0.58x |
| users.json | pysimdjson | 115.968 | 117.952 | 151.548 | 63.891 | 0.06x |
| users.json | json | 13.789 | 13.931 | 15.366 | 63.891 | 0.47x |
| flat.json | strata | 0.554 | 0.568 | 0.651 | 93.828 | 1.00x |
| flat.json | orjson | 0.654 | 0.681 | 0.749 | 93.828 | 0.83x |
| flat.json | msgspec | 0.665 | 0.668 | 0.823 | 93.828 | 0.85x |
| flat.json | ujson | 1.035 | 1.111 | 1.273 | 93.828 | 0.51x |
| flat.json | pysimdjson | 11.105 | 11.136 | 11.711 | 93.828 | 0.05x |
| flat.json | json | 1.266 | 1.281 | 1.427 | 93.828 | 0.44x |
| nested.json | strata | 0.492 | 0.495 | 0.507 | 93.859 | 1.00x |
| nested.json | orjson | 0.651 | 0.663 | 0.681 | 93.859 | 0.75x |
| nested.json | msgspec | 0.638 | 0.643 | 0.672 | 93.859 | 0.77x |
| nested.json | ujson | 0.948 | 0.980 | 1.102 | 93.859 | 0.50x |
| nested.json | pysimdjson | 9.726 | 9.792 | 10.382 | 93.859 | 0.05x |
| nested.json | json | 1.325 | 1.348 | 1.533 | 93.859 | 0.37x |
| wide_arrays.json | strata | 3.218 | 3.244 | 3.597 | 96.672 | 1.00x |
| wide_arrays.json | orjson | 3.300 | 3.342 | 3.790 | 96.672 | 0.97x |
| wide_arrays.json | msgspec | 3.777 | 3.821 | 4.325 | 96.672 | 0.85x |
| wide_arrays.json | ujson | 4.981 | 5.104 | 5.440 | 96.672 | 0.64x |
| wide_arrays.json | pysimdjson | 59.833 | 60.630 | 61.824 | 96.672 | 0.05x |
| wide_arrays.json | json | 6.350 | 6.457 | 6.884 | 96.672 | 0.50x |
| mixed.json | strata | 0.123 | 0.125 | 0.153 | 96.688 | 1.00x |
| mixed.json | orjson | 0.144 | 0.149 | 0.191 | 96.688 | 0.84x |
| mixed.json | msgspec | 0.157 | 0.159 | 0.175 | 96.688 | 0.79x |
| mixed.json | ujson | 0.194 | 0.236 | 0.396 | 96.688 | 0.53x |
| mixed.json | pysimdjson | 2.337 | 2.365 | 2.545 | 96.688 | 0.05x |
| mixed.json | json | 0.303 | 0.307 | 0.342 | 96.688 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.657 | 1.685 | 1.739 | 76.938 | 1.00x |
| users.json | orjson | 2.142 | 2.189 | 2.341 | 76.938 | 0.77x |
| users.json | msgspec | 2.689 | 2.705 | 2.891 | 76.938 | 0.62x |
| users.json | ujson | 10.238 | 10.304 | 10.627 | 76.938 | 0.16x |
| users.json | json | 14.712 | 14.802 | 15.712 | 76.938 | 0.11x |
| flat.json | strata | 0.184 | 0.191 | 0.197 | 93.828 | 1.00x |
| flat.json | orjson | 0.235 | 0.264 | 0.356 | 93.828 | 0.73x |
| flat.json | msgspec | 0.289 | 0.297 | 0.352 | 93.828 | 0.65x |
| flat.json | ujson | 0.874 | 0.880 | 0.979 | 93.828 | 0.22x |
| flat.json | json | 1.266 | 1.308 | 1.465 | 93.828 | 0.15x |
| nested.json | strata | 0.124 | 0.126 | 0.127 | 93.859 | 1.00x |
| nested.json | orjson | 0.198 | 0.205 | 0.211 | 93.859 | 0.61x |
| nested.json | msgspec | 0.267 | 0.347 | 0.366 | 93.859 | 0.36x |
| nested.json | ujson | 0.926 | 0.995 | 1.152 | 93.859 | 0.13x |
| nested.json | json | 1.528 | 1.532 | 1.562 | 93.859 | 0.08x |
| wide_arrays.json | strata | 1.008 | 1.059 | 1.187 | 96.672 | 1.00x |
| wide_arrays.json | orjson | 1.269 | 1.317 | 1.378 | 96.672 | 0.80x |
| wide_arrays.json | msgspec | 1.933 | 1.948 | 2.059 | 96.672 | 0.54x |
| wide_arrays.json | ujson | 6.065 | 6.153 | 6.708 | 96.672 | 0.17x |
| wide_arrays.json | json | 10.928 | 11.000 | 11.687 | 96.672 | 0.10x |
| mixed.json | strata | 0.037 | 0.038 | 0.047 | 96.688 | 1.00x |
| mixed.json | orjson | 0.039 | 0.040 | 0.172 | 96.688 | 0.95x |
| mixed.json | msgspec | 0.047 | 0.048 | 0.057 | 96.688 | 0.80x |
| mixed.json | ujson | 0.204 | 0.205 | 0.221 | 96.688 | 0.19x |
| mixed.json | json | 0.321 | 0.324 | 0.388 | 96.688 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.801 | 6.824 | 7.423 | 90.406 | 1.00x |
| users.json | orjson | 8.938 | 9.065 | 9.627 | 90.406 | 0.75x |
| users.json | msgspec | 8.727 | 8.803 | 9.757 | 90.406 | 0.78x |
| users.json | ujson | 11.620 | 11.788 | 13.983 | 90.406 | 0.58x |
| users.json | json | 14.188 | 14.282 | 16.168 | 90.406 | 0.48x |
| flat.json | strata | 0.584 | 0.590 | 0.712 | 93.859 | 1.00x |
| flat.json | orjson | 0.751 | 0.800 | 1.075 | 93.859 | 0.74x |
| flat.json | msgspec | 0.716 | 0.724 | 0.835 | 93.859 | 0.81x |
| flat.json | ujson | 1.045 | 1.071 | 1.238 | 93.859 | 0.55x |
| flat.json | json | 1.301 | 1.306 | 1.448 | 93.859 | 0.45x |
| nested.json | strata | 0.525 | 0.532 | 0.725 | 93.859 | 1.00x |
| nested.json | orjson | 0.783 | 0.906 | 1.090 | 93.859 | 0.59x |
| nested.json | msgspec | 0.689 | 0.703 | 0.947 | 93.859 | 0.76x |
| nested.json | ujson | 0.947 | 0.962 | 1.130 | 93.859 | 0.55x |
| nested.json | json | 1.370 | 1.395 | 1.524 | 93.859 | 0.38x |
| wide_arrays.json | strata | 3.435 | 3.480 | 3.550 | 96.672 | 1.00x |
| wide_arrays.json | orjson | 3.519 | 3.540 | 3.626 | 96.672 | 0.98x |
| wide_arrays.json | msgspec | 4.131 | 4.153 | 4.203 | 96.672 | 0.84x |
| wide_arrays.json | ujson | 5.410 | 5.472 | 6.013 | 96.672 | 0.64x |
| wide_arrays.json | json | 6.682 | 6.750 | 7.285 | 96.672 | 0.52x |
| mixed.json | strata | 0.140 | 0.144 | 0.193 | 96.688 | 1.00x |
| mixed.json | orjson | 0.176 | 0.241 | 0.408 | 96.688 | 0.60x |
| mixed.json | msgspec | 0.183 | 0.192 | 0.285 | 96.688 | 0.75x |
| mixed.json | ujson | 0.227 | 0.235 | 0.291 | 96.688 | 0.61x |
| mixed.json | json | 0.326 | 0.332 | 0.394 | 96.688 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.827 | 6.887 | 7.102 | 93.172 | 1.00x |
| users.ndjson | orjson | 10.769 | 10.886 | 11.199 | 93.172 | 0.63x |
| users.ndjson | msgspec | 10.671 | 10.695 | 11.799 | 93.172 | 0.64x |
| users.ndjson | ujson | 13.137 | 13.222 | 14.765 | 93.172 | 0.52x |
| users.ndjson | json | 17.089 | 17.155 | 17.433 | 93.172 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.972 | 2.248 | 2.583 | 88.344 | 1.00x |
| users.json | orjson | 2.585 | 2.871 | 3.235 | 88.344 | 0.78x |
| users.json | msgspec | 3.168 | 3.632 | 3.936 | 88.344 | 0.62x |
| users.json | ujson | 11.059 | 12.585 | 13.902 | 88.344 | 0.18x |
| users.json | json | 15.523 | 16.994 | 17.900 | 88.344 | 0.13x |
| flat.json | strata | 0.284 | 0.308 | 0.359 | 93.859 | 1.00x |
| flat.json | orjson | 0.350 | 0.449 | 0.561 | 93.859 | 0.69x |
| flat.json | msgspec | 0.414 | 0.427 | 0.442 | 93.859 | 0.72x |
| flat.json | ujson | 0.983 | 1.001 | 1.286 | 93.859 | 0.31x |
| flat.json | json | 1.386 | 1.476 | 1.561 | 93.859 | 0.21x |
| nested.json | strata | 0.228 | 0.244 | 0.255 | 93.859 | 1.00x |
| nested.json | orjson | 0.303 | 0.331 | 0.667 | 93.859 | 0.74x |
| nested.json | msgspec | 0.453 | 0.537 | 0.613 | 93.859 | 0.46x |
| nested.json | ujson | 1.064 | 1.132 | 1.408 | 93.859 | 0.22x |
| nested.json | json | 1.650 | 1.676 | 1.785 | 93.859 | 0.15x |
| wide_arrays.json | strata | 1.317 | 1.340 | 1.580 | 96.672 | 1.00x |
| wide_arrays.json | orjson | 1.561 | 1.658 | 1.879 | 96.672 | 0.81x |
| wide_arrays.json | msgspec | 2.252 | 2.278 | 2.589 | 96.672 | 0.59x |
| wide_arrays.json | ujson | 6.606 | 6.684 | 6.782 | 96.672 | 0.20x |
| wide_arrays.json | json | 11.297 | 11.477 | 11.881 | 96.672 | 0.12x |
| mixed.json | strata | 0.124 | 0.148 | 0.625 | 96.688 | 1.00x |
| mixed.json | orjson | 0.132 | 0.160 | 0.339 | 96.688 | 0.92x |
| mixed.json | msgspec | 0.137 | 0.167 | 0.256 | 96.688 | 0.89x |
| mixed.json | ujson | 0.305 | 0.344 | 0.385 | 96.688 | 0.43x |
| mixed.json | json | 0.413 | 0.452 | 0.521 | 96.688 | 0.33x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.045 | 0.047 | 0.063 | 88.391 | 1.00x |
| users.json $[*].id | jmespath | 0.252 | 0.255 | 0.292 | 88.391 | 0.18x |
| users.json $[*].id | jsonpath-ng | 1.390 | 1.414 | 1.496 | 88.391 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.298 | 0.350 | 0.647 | 88.578 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.673 | 1.803 | 2.414 | 88.578 | 0.19x |
| users.json $[*].orders[*].total | jsonpath-ng | 10.722 | 11.752 | 13.577 | 88.578 | 0.03x |
| users.json $..total | strata | 1.190 | 1.232 | 1.637 | 90.594 | 1.00x |
| users.json $..total | jsonpath-ng | 179.966 | 195.919 | 206.042 | 90.594 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.453 | 3.493 | 3.779 | 88.453 | 1.00x |
| users.json $[*].id | orjson+jmespath | 9.128 | 9.237 | 9.324 | 88.453 | 0.38x |
| users.json $[*].id | orjson+jsonpath-ng | 10.360 | 10.430 | 12.397 | 88.453 | 0.33x |
| users.json $[*].orders[*].total | strata | 3.525 | 3.724 | 4.025 | 90.578 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 10.463 | 10.738 | 13.219 | 90.578 | 0.35x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 20.412 | 20.679 | 25.769 | 90.578 | 0.18x |
| users.json $..total | strata | 8.086 | 9.006 | 9.812 | 90.641 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 188.562 | 195.129 | 230.357 | 90.641 | 0.05x |

