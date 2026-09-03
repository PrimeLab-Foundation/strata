# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: ddc9cb7
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
| users.json | strata | 7.396 | 10.170 | 14.241 | 63.875 | 1.00x |
| users.json | orjson | 10.264 | 12.394 | 25.781 | 63.875 | 0.82x |
| users.json | msgspec | 10.985 | 13.428 | 24.191 | 63.875 | 0.76x |
| users.json | ujson | 15.227 | 19.592 | 28.423 | 63.875 | 0.52x |
| users.json | pysimdjson | 161.915 | 180.023 | 207.434 | 63.875 | 0.06x |
| users.json | json | 18.480 | 21.546 | 31.587 | 63.875 | 0.47x |
| flat.json | strata | 0.629 | 0.736 | 1.356 | 87.328 | 1.00x |
| flat.json | orjson | 0.768 | 0.995 | 1.870 | 87.328 | 0.74x |
| flat.json | msgspec | 0.754 | 0.875 | 2.119 | 87.328 | 0.84x |
| flat.json | ujson | 1.164 | 1.357 | 2.528 | 87.328 | 0.54x |
| flat.json | pysimdjson | 16.447 | 20.497 | 24.791 | 87.328 | 0.04x |
| flat.json | json | 1.499 | 1.668 | 3.628 | 87.328 | 0.44x |
| nested.json | strata | 0.552 | 0.650 | 1.230 | 87.344 | 1.00x |
| nested.json | orjson | 0.771 | 0.876 | 6.337 | 87.344 | 0.74x |
| nested.json | msgspec | 0.717 | 0.831 | 2.683 | 87.344 | 0.78x |
| nested.json | ujson | 1.177 | 1.526 | 6.696 | 87.344 | 0.43x |
| nested.json | pysimdjson | 10.637 | 13.697 | 26.614 | 87.344 | 0.05x |
| nested.json | json | 1.513 | 1.639 | 2.274 | 87.344 | 0.40x |
| wide_arrays.json | strata | 3.556 | 3.895 | 6.829 | 90.984 | 1.00x |
| wide_arrays.json | orjson | 3.716 | 3.995 | 7.316 | 90.984 | 0.97x |
| wide_arrays.json | msgspec | 4.206 | 4.857 | 9.897 | 90.984 | 0.80x |
| wide_arrays.json | ujson | 5.725 | 6.379 | 8.938 | 90.984 | 0.61x |
| wide_arrays.json | pysimdjson | 76.976 | 92.785 | 107.653 | 90.984 | 0.04x |
| wide_arrays.json | json | 7.644 | 8.489 | 17.595 | 90.984 | 0.46x |
| mixed.json | strata | 0.145 | 0.153 | 0.495 | 91.000 | 1.00x |
| mixed.json | orjson | 0.170 | 0.194 | 0.534 | 91.000 | 0.79x |
| mixed.json | msgspec | 0.189 | 0.211 | 0.523 | 91.000 | 0.72x |
| mixed.json | ujson | 0.224 | 0.269 | 1.004 | 91.000 | 0.57x |
| mixed.json | pysimdjson | 2.584 | 2.808 | 4.541 | 91.000 | 0.05x |
| mixed.json | json | 0.357 | 0.394 | 1.112 | 91.000 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.732 | 1.969 | 5.521 | 75.750 | 1.00x |
| users.json | orjson | 2.571 | 2.704 | 4.000 | 75.750 | 0.73x |
| users.json | msgspec | 3.114 | 3.292 | 5.159 | 75.750 | 0.60x |
| users.json | ujson | 12.101 | 14.033 | 18.322 | 75.750 | 0.14x |
| users.json | json | 17.761 | 23.623 | 26.903 | 75.750 | 0.08x |
| flat.json | strata | 0.237 | 0.277 | 0.368 | 87.344 | 1.00x |
| flat.json | orjson | 0.266 | 0.317 | 0.804 | 87.344 | 0.87x |
| flat.json | msgspec | 0.340 | 0.392 | 0.902 | 87.344 | 0.71x |
| flat.json | ujson | 0.947 | 1.152 | 2.377 | 87.344 | 0.24x |
| flat.json | json | 1.441 | 1.725 | 3.402 | 87.344 | 0.16x |
| nested.json | strata | 0.154 | 0.238 | 0.590 | 87.344 | 1.00x |
| nested.json | orjson | 0.259 | 0.278 | 0.641 | 87.344 | 0.86x |
| nested.json | msgspec | 0.432 | 0.481 | 1.104 | 87.344 | 0.50x |
| nested.json | ujson | 1.134 | 1.306 | 1.471 | 87.344 | 0.18x |
| nested.json | json | 1.770 | 1.949 | 4.432 | 87.344 | 0.12x |
| wide_arrays.json | strata | 1.156 | 1.362 | 3.396 | 90.984 | 1.00x |
| wide_arrays.json | orjson | 1.483 | 1.805 | 4.068 | 90.984 | 0.75x |
| wide_arrays.json | msgspec | 2.459 | 2.768 | 5.250 | 90.984 | 0.49x |
| wide_arrays.json | ujson | 7.444 | 8.910 | 12.615 | 90.984 | 0.15x |
| wide_arrays.json | json | 12.737 | 18.939 | 23.488 | 90.984 | 0.07x |
| mixed.json | strata | 0.048 | 0.056 | 0.151 | 91.000 | 1.00x |
| mixed.json | orjson | 0.053 | 0.063 | 0.150 | 91.000 | 0.89x |
| mixed.json | msgspec | 0.068 | 0.078 | 0.263 | 91.000 | 0.71x |
| mixed.json | ujson | 0.239 | 0.269 | 0.577 | 91.000 | 0.21x |
| mixed.json | json | 0.392 | 0.406 | 1.306 | 91.000 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.182 | 9.137 | 12.699 | 87.969 | 1.00x |
| users.json | orjson | 11.205 | 12.693 | 16.304 | 87.969 | 0.72x |
| users.json | msgspec | 12.124 | 12.980 | 15.655 | 87.969 | 0.70x |
| users.json | ujson | 15.565 | 17.096 | 27.182 | 87.969 | 0.53x |
| users.json | json | 18.842 | 22.895 | 30.963 | 87.969 | 0.40x |
| flat.json | strata | 0.736 | 0.803 | 3.765 | 87.344 | 1.00x |
| flat.json | orjson | 0.956 | 1.177 | 2.456 | 87.344 | 0.68x |
| flat.json | msgspec | 0.837 | 0.977 | 2.347 | 87.344 | 0.82x |
| flat.json | ujson | 1.444 | 1.665 | 25.022 | 87.344 | 0.48x |
| flat.json | json | 1.551 | 1.784 | 7.029 | 87.344 | 0.45x |
| nested.json | strata | 0.666 | 0.778 | 1.200 | 87.344 | 1.00x |
| nested.json | orjson | 1.098 | 1.432 | 2.814 | 87.344 | 0.54x |
| nested.json | msgspec | 0.913 | 1.285 | 3.049 | 87.344 | 0.61x |
| nested.json | ujson | 1.218 | 1.390 | 3.712 | 87.344 | 0.56x |
| nested.json | json | 1.646 | 1.913 | 4.217 | 87.344 | 0.41x |
| wide_arrays.json | strata | 3.996 | 5.716 | 6.796 | 90.984 | 1.00x |
| wide_arrays.json | orjson | 4.434 | 5.828 | 9.157 | 90.984 | 0.98x |
| wide_arrays.json | msgspec | 4.594 | 6.012 | 10.946 | 90.984 | 0.95x |
| wide_arrays.json | ujson | 6.426 | 9.603 | 14.522 | 90.984 | 0.60x |
| wide_arrays.json | json | 9.321 | 13.288 | 17.910 | 90.984 | 0.43x |
| mixed.json | strata | 0.216 | 0.270 | 0.573 | 91.000 | 1.00x |
| mixed.json | orjson | 0.322 | 0.400 | 0.761 | 91.000 | 0.67x |
| mixed.json | msgspec | 0.309 | 0.338 | 0.376 | 91.000 | 0.80x |
| mixed.json | ujson | 0.351 | 0.398 | 2.418 | 91.000 | 0.68x |
| mixed.json | json | 0.481 | 0.565 | 1.210 | 91.000 | 0.48x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.639 | 9.066 | 18.927 | 86.688 | 1.00x |
| users.ndjson | orjson | 12.083 | 15.762 | 18.821 | 86.688 | 0.58x |
| users.ndjson | msgspec | 11.856 | 14.531 | 18.646 | 86.688 | 0.62x |
| users.ndjson | ujson | 15.097 | 21.573 | 31.687 | 86.688 | 0.42x |
| users.ndjson | json | 19.263 | 26.203 | 34.068 | 86.688 | 0.35x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.143 | 2.231 | 3.843 | 86.703 | 1.00x |
| users.json | orjson | 2.985 | 3.105 | 3.304 | 86.703 | 0.72x |
| users.json | msgspec | 3.563 | 3.816 | 7.461 | 86.703 | 0.58x |
| users.json | ujson | 11.940 | 13.604 | 17.161 | 86.703 | 0.16x |
| users.json | json | 16.655 | 18.840 | 24.990 | 86.703 | 0.12x |
| flat.json | strata | 0.375 | 0.459 | 0.504 | 87.344 | 1.00x |
| flat.json | orjson | 0.407 | 0.543 | 1.222 | 87.344 | 0.85x |
| flat.json | msgspec | 0.453 | 0.597 | 0.776 | 87.344 | 0.77x |
| flat.json | ujson | 1.108 | 1.244 | 3.172 | 87.344 | 0.37x |
| flat.json | json | 1.709 | 1.886 | 3.427 | 87.344 | 0.24x |
| nested.json | strata | 0.277 | 0.338 | 0.385 | 87.344 | 1.00x |
| nested.json | orjson | 0.383 | 0.409 | 1.170 | 87.344 | 0.83x |
| nested.json | msgspec | 0.558 | 0.590 | 1.430 | 87.344 | 0.57x |
| nested.json | ujson | 1.238 | 1.344 | 1.537 | 87.344 | 0.25x |
| nested.json | json | 1.847 | 1.946 | 4.747 | 87.344 | 0.17x |
| wide_arrays.json | strata | 1.608 | 1.771 | 2.552 | 90.984 | 1.00x |
| wide_arrays.json | orjson | 1.897 | 2.303 | 5.511 | 90.984 | 0.77x |
| wide_arrays.json | msgspec | 2.798 | 3.114 | 8.289 | 90.984 | 0.57x |
| wide_arrays.json | ujson | 7.719 | 9.222 | 36.482 | 90.984 | 0.19x |
| wide_arrays.json | json | 12.897 | 14.801 | 24.906 | 90.984 | 0.12x |
| mixed.json | strata | 0.172 | 0.265 | 0.551 | 91.000 | 1.00x |
| mixed.json | orjson | 0.192 | 0.257 | 0.393 | 91.000 | 1.03x |
| mixed.json | msgspec | 0.183 | 0.300 | 0.716 | 91.000 | 0.88x |
| mixed.json | ujson | 0.360 | 0.474 | 1.301 | 91.000 | 0.56x |
| mixed.json | json | 0.487 | 0.645 | 0.863 | 91.000 | 0.41x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.060 | 0.073 | 0.111 | 86.750 | 1.00x |
| users.json $[*].id | jmespath | 0.282 | 0.329 | 1.081 | 86.750 | 0.22x |
| users.json $[*].id | jsonpath-ng | 1.523 | 1.720 | 2.687 | 86.750 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.336 | 0.470 | 0.593 | 87.000 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.751 | 2.418 | 6.002 | 87.000 | 0.19x |
| users.json $[*].orders[*].total | jsonpath-ng | 12.370 | 18.632 | 24.272 | 87.000 | 0.03x |
| users.json $..total | strata | 1.416 | 1.743 | 2.332 | 86.844 | 1.00x |
| users.json $..total | jsonpath-ng | 253.645 | 285.639 | 326.009 | 86.844 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.002 | 4.428 | 6.492 | 86.828 | 1.00x |
| users.json $[*].id | orjson+jmespath | 11.078 | 15.534 | 29.229 | 86.828 | 0.29x |
| users.json $[*].id | orjson+jsonpath-ng | 13.212 | 16.585 | 27.645 | 86.828 | 0.27x |
| users.json $[*].orders[*].total | strata | 4.006 | 6.844 | 8.976 | 86.844 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.602 | 23.830 | 38.173 | 86.844 | 0.29x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 32.489 | 51.721 | 79.069 | 86.844 | 0.13x |
| users.json $..total | strata | 8.787 | 9.557 | 15.249 | 86.875 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 256.751 | 280.421 | 369.268 | 86.875 | 0.03x |

