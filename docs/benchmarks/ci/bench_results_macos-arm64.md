# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 49acca3
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
| users.json | strata | 7.167 | 7.542 | 13.807 | 63.656 | 1.00x |
| users.json | orjson | 10.053 | 10.334 | 19.372 | 63.656 | 0.73x |
| users.json | msgspec | 9.619 | 10.623 | 11.982 | 63.656 | 0.71x |
| users.json | ujson | 13.195 | 14.764 | 27.962 | 63.656 | 0.51x |
| users.json | pysimdjson | 124.996 | 144.000 | 170.045 | 63.656 | 0.05x |
| users.json | json | 15.650 | 17.039 | 19.872 | 63.656 | 0.44x |
| flat.json | strata | 0.598 | 0.607 | 0.681 | 87.141 | 1.00x |
| flat.json | orjson | 0.739 | 0.769 | 0.834 | 87.141 | 0.79x |
| flat.json | msgspec | 0.725 | 0.731 | 0.804 | 87.141 | 0.83x |
| flat.json | ujson | 1.203 | 1.223 | 1.293 | 87.141 | 0.50x |
| flat.json | pysimdjson | 12.075 | 12.134 | 12.609 | 87.141 | 0.05x |
| flat.json | json | 1.356 | 1.379 | 1.630 | 87.141 | 0.44x |
| nested.json | strata | 0.543 | 0.561 | 0.808 | 87.156 | 1.00x |
| nested.json | orjson | 0.749 | 0.806 | 1.163 | 87.156 | 0.70x |
| nested.json | msgspec | 0.695 | 0.734 | 0.821 | 87.156 | 0.76x |
| nested.json | ujson | 1.038 | 1.185 | 1.253 | 87.156 | 0.47x |
| nested.json | pysimdjson | 10.649 | 10.843 | 15.901 | 87.156 | 0.05x |
| nested.json | json | 1.449 | 1.491 | 1.592 | 87.156 | 0.38x |
| wide_arrays.json | strata | 3.594 | 3.805 | 6.885 | 90.500 | 1.00x |
| wide_arrays.json | orjson | 3.628 | 4.365 | 6.882 | 90.500 | 0.87x |
| wide_arrays.json | msgspec | 4.378 | 4.802 | 6.939 | 90.500 | 0.79x |
| wide_arrays.json | ujson | 6.013 | 6.381 | 10.249 | 90.500 | 0.60x |
| wide_arrays.json | pysimdjson | 65.884 | 69.109 | 97.555 | 90.500 | 0.06x |
| wide_arrays.json | json | 7.033 | 7.678 | 11.685 | 90.500 | 0.50x |
| mixed.json | strata | 0.137 | 0.147 | 0.180 | 90.531 | 1.00x |
| mixed.json | orjson | 0.165 | 0.181 | 0.231 | 90.531 | 0.81x |
| mixed.json | msgspec | 0.177 | 0.187 | 0.210 | 90.531 | 0.79x |
| mixed.json | ujson | 0.223 | 0.362 | 0.485 | 90.531 | 0.41x |
| mixed.json | pysimdjson | 2.516 | 2.599 | 3.841 | 90.531 | 0.06x |
| mixed.json | json | 0.344 | 0.349 | 0.382 | 90.531 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.720 | 1.819 | 1.917 | 72.828 | 1.00x |
| users.json | orjson | 2.290 | 2.440 | 2.581 | 72.828 | 0.75x |
| users.json | msgspec | 3.033 | 3.105 | 3.315 | 72.828 | 0.59x |
| users.json | ujson | 11.195 | 11.298 | 11.754 | 72.828 | 0.16x |
| users.json | json | 15.781 | 16.352 | 16.738 | 72.828 | 0.11x |
| flat.json | strata | 0.213 | 0.223 | 0.278 | 87.156 | 1.00x |
| flat.json | orjson | 0.301 | 0.373 | 0.382 | 87.156 | 0.60x |
| flat.json | msgspec | 0.337 | 0.346 | 0.378 | 87.156 | 0.64x |
| flat.json | ujson | 0.963 | 0.986 | 1.000 | 87.156 | 0.23x |
| flat.json | json | 1.519 | 1.545 | 1.633 | 87.156 | 0.14x |
| nested.json | strata | 0.141 | 0.147 | 0.214 | 87.172 | 1.00x |
| nested.json | orjson | 0.229 | 0.244 | 0.312 | 87.172 | 0.60x |
| nested.json | msgspec | 0.303 | 0.422 | 0.526 | 87.172 | 0.35x |
| nested.json | ujson | 1.060 | 1.182 | 1.421 | 87.172 | 0.12x |
| nested.json | json | 1.668 | 1.723 | 2.584 | 87.172 | 0.09x |
| wide_arrays.json | strata | 1.134 | 1.313 | 1.429 | 90.516 | 1.00x |
| wide_arrays.json | orjson | 1.550 | 1.704 | 4.608 | 90.516 | 0.77x |
| wide_arrays.json | msgspec | 2.206 | 2.445 | 3.016 | 90.516 | 0.54x |
| wide_arrays.json | ujson | 6.859 | 7.353 | 10.026 | 90.516 | 0.18x |
| wide_arrays.json | json | 12.128 | 12.820 | 14.577 | 90.516 | 0.10x |
| mixed.json | strata | 0.045 | 0.049 | 0.072 | 90.531 | 1.00x |
| mixed.json | orjson | 0.049 | 0.061 | 0.076 | 90.531 | 0.81x |
| mixed.json | msgspec | 0.057 | 0.067 | 0.212 | 90.531 | 0.74x |
| mixed.json | ujson | 0.228 | 0.241 | 0.339 | 90.531 | 0.20x |
| mixed.json | json | 0.363 | 0.384 | 0.415 | 90.531 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.608 | 8.122 | 10.634 | 85.391 | 1.00x |
| users.json | orjson | 10.317 | 11.436 | 11.885 | 85.391 | 0.71x |
| users.json | msgspec | 9.505 | 11.112 | 12.151 | 85.391 | 0.73x |
| users.json | ujson | 14.173 | 15.060 | 16.495 | 85.391 | 0.54x |
| users.json | json | 16.463 | 17.568 | 18.792 | 85.391 | 0.46x |
| flat.json | strata | 0.689 | 0.777 | 0.945 | 87.156 | 1.00x |
| flat.json | orjson | 0.910 | 1.088 | 1.315 | 87.156 | 0.71x |
| flat.json | msgspec | 0.795 | 1.029 | 1.392 | 87.156 | 0.76x |
| flat.json | ujson | 1.199 | 1.382 | 1.778 | 87.156 | 0.56x |
| flat.json | json | 1.397 | 1.590 | 1.929 | 87.156 | 0.49x |
| nested.json | strata | 0.662 | 0.722 | 1.186 | 87.172 | 1.00x |
| nested.json | orjson | 1.013 | 1.073 | 4.134 | 87.172 | 0.67x |
| nested.json | msgspec | 0.838 | 0.910 | 2.309 | 87.172 | 0.79x |
| nested.json | ujson | 1.151 | 1.286 | 2.924 | 87.172 | 0.56x |
| nested.json | json | 1.591 | 1.698 | 6.459 | 87.172 | 0.42x |
| wide_arrays.json | strata | 3.810 | 3.937 | 5.572 | 90.516 | 1.00x |
| wide_arrays.json | orjson | 4.155 | 4.394 | 6.580 | 90.516 | 0.90x |
| wide_arrays.json | msgspec | 4.712 | 5.069 | 11.339 | 90.516 | 0.78x |
| wide_arrays.json | ujson | 6.263 | 6.860 | 48.448 | 90.516 | 0.57x |
| wide_arrays.json | json | 7.664 | 8.132 | 13.687 | 90.516 | 0.48x |
| mixed.json | strata | 0.170 | 0.202 | 0.245 | 90.531 | 1.00x |
| mixed.json | orjson | 0.286 | 0.382 | 0.559 | 90.531 | 0.53x |
| mixed.json | msgspec | 0.254 | 0.269 | 0.326 | 90.531 | 0.75x |
| mixed.json | ujson | 0.311 | 0.413 | 1.069 | 90.531 | 0.49x |
| mixed.json | json | 0.386 | 0.464 | 0.667 | 90.531 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.661 | 7.926 | 8.360 | 87.141 | 1.00x |
| users.ndjson | orjson | 12.181 | 12.441 | 13.207 | 87.141 | 0.64x |
| users.ndjson | msgspec | 11.986 | 12.315 | 13.283 | 87.141 | 0.64x |
| users.ndjson | ujson | 14.923 | 15.160 | 16.953 | 87.141 | 0.52x |
| users.ndjson | json | 19.121 | 19.605 | 21.944 | 87.141 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.110 | 2.277 | 2.906 | 81.391 | 1.00x |
| users.json | orjson | 2.821 | 2.975 | 3.187 | 81.391 | 0.77x |
| users.json | msgspec | 3.488 | 3.667 | 4.515 | 81.391 | 0.62x |
| users.json | ujson | 11.715 | 12.467 | 12.968 | 81.391 | 0.18x |
| users.json | json | 16.763 | 17.075 | 17.681 | 81.391 | 0.13x |
| flat.json | strata | 0.370 | 0.473 | 6.725 | 87.156 | 1.00x |
| flat.json | orjson | 0.443 | 0.509 | 12.629 | 87.156 | 0.93x |
| flat.json | msgspec | 0.499 | 0.568 | 18.334 | 87.156 | 0.83x |
| flat.json | ujson | 1.133 | 1.203 | 12.986 | 87.156 | 0.39x |
| flat.json | json | 1.709 | 1.760 | 15.636 | 87.156 | 0.27x |
| nested.json | strata | 0.397 | 0.465 | 1.157 | 87.172 | 1.00x |
| nested.json | orjson | 0.460 | 0.562 | 0.763 | 87.172 | 0.83x |
| nested.json | msgspec | 0.565 | 0.842 | 2.145 | 87.172 | 0.55x |
| nested.json | ujson | 1.534 | 1.765 | 3.173 | 87.172 | 0.26x |
| nested.json | json | 2.005 | 2.141 | 4.960 | 87.172 | 0.22x |
| wide_arrays.json | strata | 1.560 | 1.643 | 2.517 | 90.516 | 1.00x |
| wide_arrays.json | orjson | 1.959 | 2.038 | 2.443 | 90.516 | 0.81x |
| wide_arrays.json | msgspec | 2.633 | 2.834 | 3.057 | 90.516 | 0.58x |
| wide_arrays.json | ujson | 7.057 | 8.129 | 8.735 | 90.516 | 0.20x |
| wide_arrays.json | json | 12.721 | 13.235 | 18.181 | 90.516 | 0.12x |
| mixed.json | strata | 0.188 | 0.231 | 0.299 | 90.531 | 1.00x |
| mixed.json | orjson | 0.214 | 0.263 | 0.319 | 90.531 | 0.88x |
| mixed.json | msgspec | 0.224 | 0.285 | 0.486 | 90.531 | 0.81x |
| mixed.json | ujson | 0.399 | 0.472 | 0.505 | 90.531 | 0.49x |
| mixed.json | json | 0.515 | 0.622 | 0.966 | 90.531 | 0.37x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.063 | 0.075 | 0.112 | 81.438 | 1.00x |
| users.json $[*].id | jmespath | 0.308 | 0.325 | 0.394 | 81.438 | 0.23x |
| users.json $[*].id | jsonpath-ng | 1.562 | 1.590 | 1.677 | 81.438 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.351 | 0.526 | 0.700 | 82.625 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.836 | 2.150 | 2.783 | 82.625 | 0.24x |
| users.json $[*].orders[*].total | jsonpath-ng | 10.903 | 11.614 | 14.871 | 82.625 | 0.05x |
| users.json $..total | strata | 1.329 | 1.407 | 1.703 | 84.500 | 1.00x |
| users.json $..total | jsonpath-ng | 195.756 | 196.832 | 210.761 | 84.500 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.759 | 3.807 | 3.856 | 82.484 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.539 | 10.716 | 11.048 | 82.484 | 0.36x |
| users.json $[*].id | orjson+jsonpath-ng | 11.938 | 12.072 | 12.402 | 82.484 | 0.32x |
| users.json $[*].orders[*].total | strata | 3.894 | 4.271 | 4.735 | 84.469 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.381 | 13.854 | 15.754 | 84.469 | 0.31x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 25.540 | 28.119 | 31.305 | 84.469 | 0.15x |
| users.json $..total | strata | 9.129 | 9.213 | 11.198 | 84.516 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 206.432 | 209.545 | 285.685 | 84.516 | 0.04x |

