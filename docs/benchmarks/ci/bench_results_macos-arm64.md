# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: b6e2a27
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
| users.json | strata | 6.127 | 6.779 | 7.351 | 63.688 | 1.00x |
| users.json | orjson | 9.027 | 10.265 | 12.076 | 63.688 | 0.66x |
| users.json | msgspec | 8.695 | 9.822 | 11.173 | 63.688 | 0.69x |
| users.json | ujson | 11.548 | 13.463 | 14.808 | 63.688 | 0.50x |
| users.json | pysimdjson | 122.063 | 134.305 | 140.628 | 63.688 | 0.05x |
| users.json | json | 14.530 | 16.571 | 17.773 | 63.688 | 0.41x |
| flat.json | strata | 0.618 | 0.751 | 1.580 | 87.719 | 1.00x |
| flat.json | orjson | 0.853 | 1.053 | 2.005 | 87.719 | 0.71x |
| flat.json | msgspec | 0.768 | 0.962 | 1.969 | 87.719 | 0.78x |
| flat.json | ujson | 1.380 | 1.812 | 2.315 | 87.719 | 0.41x |
| flat.json | pysimdjson | 12.983 | 16.401 | 26.137 | 87.719 | 0.05x |
| flat.json | json | 1.403 | 1.566 | 3.056 | 87.719 | 0.48x |
| nested.json | strata | 0.568 | 0.677 | 1.031 | 87.719 | 1.00x |
| nested.json | orjson | 0.853 | 0.907 | 1.347 | 87.719 | 0.75x |
| nested.json | msgspec | 0.760 | 0.814 | 1.485 | 87.719 | 0.83x |
| nested.json | ujson | 1.183 | 1.263 | 2.490 | 87.719 | 0.54x |
| nested.json | pysimdjson | 11.169 | 12.378 | 16.364 | 87.719 | 0.05x |
| nested.json | json | 1.508 | 1.715 | 4.001 | 87.719 | 0.39x |
| wide_arrays.json | strata | 3.038 | 3.487 | 3.862 | 91.531 | 1.00x |
| wide_arrays.json | orjson | 3.894 | 4.284 | 4.583 | 91.531 | 0.81x |
| wide_arrays.json | msgspec | 4.362 | 4.871 | 5.247 | 91.531 | 0.72x |
| wide_arrays.json | ujson | 6.282 | 6.468 | 7.131 | 91.531 | 0.54x |
| wide_arrays.json | pysimdjson | 68.543 | 74.130 | 77.874 | 91.531 | 0.05x |
| wide_arrays.json | json | 7.476 | 8.210 | 11.543 | 91.531 | 0.42x |
| mixed.json | strata | 0.143 | 0.161 | 0.221 | 91.562 | 1.00x |
| mixed.json | orjson | 0.186 | 0.215 | 0.247 | 91.562 | 0.75x |
| mixed.json | msgspec | 0.202 | 0.241 | 0.293 | 91.562 | 0.67x |
| mixed.json | ujson | 0.261 | 0.476 | 0.674 | 91.562 | 0.34x |
| mixed.json | pysimdjson | 2.840 | 3.035 | 3.584 | 91.562 | 0.05x |
| mixed.json | json | 0.364 | 0.414 | 0.898 | 91.562 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.683 | 1.853 | 1.952 | 73.406 | 1.00x |
| users.json | orjson | 2.351 | 2.576 | 2.729 | 73.406 | 0.72x |
| users.json | msgspec | 3.085 | 3.327 | 4.059 | 73.406 | 0.56x |
| users.json | ujson | 9.668 | 9.948 | 10.432 | 73.406 | 0.19x |
| users.json | json | 16.866 | 17.655 | 19.777 | 73.406 | 0.10x |
| flat.json | strata | 0.259 | 0.412 | 1.165 | 87.719 | 1.00x |
| flat.json | orjson | 0.294 | 0.368 | 1.445 | 87.719 | 1.12x |
| flat.json | msgspec | 0.359 | 0.425 | 0.568 | 87.719 | 0.97x |
| flat.json | ujson | 0.869 | 1.155 | 2.229 | 87.719 | 0.36x |
| flat.json | json | 1.486 | 2.026 | 3.299 | 87.719 | 0.20x |
| nested.json | strata | 0.160 | 0.197 | 0.481 | 87.734 | 1.00x |
| nested.json | orjson | 0.257 | 0.290 | 0.395 | 87.734 | 0.68x |
| nested.json | msgspec | 0.308 | 0.401 | 0.530 | 87.734 | 0.49x |
| nested.json | ujson | 0.940 | 1.233 | 2.633 | 87.734 | 0.16x |
| nested.json | json | 1.749 | 2.007 | 3.291 | 87.734 | 0.10x |
| wide_arrays.json | strata | 1.258 | 1.323 | 1.695 | 91.547 | 1.00x |
| wide_arrays.json | orjson | 1.571 | 1.736 | 1.901 | 91.547 | 0.76x |
| wide_arrays.json | msgspec | 2.440 | 2.766 | 5.460 | 91.547 | 0.48x |
| wide_arrays.json | ujson | 5.547 | 5.995 | 7.094 | 91.547 | 0.22x |
| wide_arrays.json | json | 12.910 | 13.688 | 18.092 | 91.547 | 0.10x |
| mixed.json | strata | 0.059 | 0.066 | 0.244 | 91.562 | 1.00x |
| mixed.json | orjson | 0.054 | 0.072 | 0.092 | 91.562 | 0.92x |
| mixed.json | msgspec | 0.071 | 0.173 | 0.283 | 91.562 | 0.38x |
| mixed.json | ujson | 0.203 | 0.227 | 0.606 | 91.562 | 0.29x |
| mixed.json | json | 0.422 | 0.468 | 0.687 | 91.562 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.546 | 7.064 | 7.738 | 84.984 | 1.00x |
| users.json | orjson | 9.538 | 10.332 | 10.693 | 84.984 | 0.68x |
| users.json | msgspec | 9.182 | 10.074 | 11.268 | 84.984 | 0.70x |
| users.json | ujson | 12.359 | 13.384 | 14.015 | 84.984 | 0.53x |
| users.json | json | 14.997 | 15.925 | 17.304 | 84.984 | 0.44x |
| flat.json | strata | 0.805 | 0.880 | 1.325 | 87.719 | 1.00x |
| flat.json | orjson | 1.102 | 1.258 | 2.130 | 87.719 | 0.70x |
| flat.json | msgspec | 0.892 | 1.130 | 1.472 | 87.719 | 0.78x |
| flat.json | ujson | 1.499 | 1.657 | 2.470 | 87.719 | 0.53x |
| flat.json | json | 1.720 | 1.956 | 3.458 | 87.719 | 0.45x |
| nested.json | strata | 0.683 | 0.778 | 1.450 | 87.734 | 1.00x |
| nested.json | orjson | 1.055 | 1.283 | 2.151 | 87.734 | 0.61x |
| nested.json | msgspec | 0.897 | 0.973 | 2.353 | 87.734 | 0.80x |
| nested.json | ujson | 1.230 | 1.379 | 2.397 | 87.734 | 0.56x |
| nested.json | json | 1.638 | 1.782 | 2.537 | 87.734 | 0.44x |
| wide_arrays.json | strata | 3.393 | 3.772 | 4.188 | 91.547 | 1.00x |
| wide_arrays.json | orjson | 4.367 | 4.700 | 5.029 | 91.547 | 0.80x |
| wide_arrays.json | msgspec | 4.838 | 5.246 | 5.523 | 91.547 | 0.72x |
| wide_arrays.json | ujson | 6.529 | 6.950 | 7.735 | 91.547 | 0.54x |
| wide_arrays.json | json | 7.974 | 8.736 | 9.108 | 91.547 | 0.43x |
| mixed.json | strata | 0.268 | 0.288 | 0.502 | 91.562 | 1.00x |
| mixed.json | orjson | 0.494 | 0.613 | 1.189 | 91.562 | 0.47x |
| mixed.json | msgspec | 0.371 | 0.442 | 0.549 | 91.562 | 0.65x |
| mixed.json | ujson | 0.449 | 0.553 | 1.203 | 91.562 | 0.52x |
| mixed.json | json | 0.534 | 0.579 | 1.366 | 91.562 | 0.50x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.551 | 9.498 | 10.295 | 87.062 | 1.00x |
| users.ndjson | orjson | 13.551 | 15.517 | 19.248 | 87.062 | 0.61x |
| users.ndjson | msgspec | 13.852 | 15.136 | 17.387 | 87.062 | 0.63x |
| users.ndjson | ujson | 18.104 | 20.291 | 21.193 | 87.062 | 0.47x |
| users.ndjson | json | 22.989 | 24.154 | 28.054 | 87.062 | 0.39x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.894 | 2.488 | 2.840 | 83.156 | 1.00x |
| users.json | orjson | 2.717 | 3.177 | 3.733 | 83.156 | 0.78x |
| users.json | msgspec | 3.320 | 3.952 | 4.723 | 83.156 | 0.63x |
| users.json | ujson | 9.339 | 10.748 | 11.927 | 83.156 | 0.23x |
| users.json | json | 16.879 | 19.375 | 20.174 | 83.156 | 0.13x |
| flat.json | strata | 0.597 | 0.683 | 0.910 | 87.719 | 1.00x |
| flat.json | orjson | 0.557 | 0.731 | 0.956 | 87.719 | 0.93x |
| flat.json | msgspec | 0.719 | 0.791 | 1.053 | 87.719 | 0.86x |
| flat.json | ujson | 1.232 | 1.363 | 1.528 | 87.719 | 0.50x |
| flat.json | json | 2.004 | 2.263 | 3.102 | 87.719 | 0.30x |
| nested.json | strata | 0.414 | 0.533 | 0.580 | 87.734 | 1.00x |
| nested.json | orjson | 0.589 | 0.761 | 1.379 | 87.734 | 0.70x |
| nested.json | msgspec | 0.740 | 0.878 | 1.256 | 87.734 | 0.61x |
| nested.json | ujson | 1.464 | 1.800 | 3.767 | 87.734 | 0.30x |
| nested.json | json | 2.193 | 2.404 | 5.422 | 87.734 | 0.22x |
| wide_arrays.json | strata | 1.896 | 2.143 | 3.255 | 91.547 | 1.00x |
| wide_arrays.json | orjson | 2.189 | 2.951 | 3.983 | 91.547 | 0.73x |
| wide_arrays.json | msgspec | 3.187 | 3.700 | 4.853 | 91.547 | 0.58x |
| wide_arrays.json | ujson | 6.538 | 6.913 | 7.555 | 91.547 | 0.31x |
| wide_arrays.json | json | 13.806 | 15.061 | 20.206 | 91.547 | 0.14x |
| mixed.json | strata | 0.318 | 0.467 | 0.808 | 91.562 | 1.00x |
| mixed.json | orjson | 0.361 | 0.424 | 0.647 | 91.562 | 1.10x |
| mixed.json | msgspec | 0.291 | 0.504 | 1.323 | 91.562 | 0.93x |
| mixed.json | ujson | 0.355 | 0.739 | 1.442 | 91.562 | 0.63x |
| mixed.json | json | 0.695 | 0.841 | 1.425 | 91.562 | 0.56x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.072 | 0.083 | 0.172 | 83.234 | 1.00x |
| users.json $[*].id | jmespath | 0.311 | 0.389 | 0.462 | 83.234 | 0.21x |
| users.json $[*].id | jsonpath-ng | 1.629 | 1.820 | 2.360 | 83.234 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.465 | 0.542 | 0.704 | 83.328 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.858 | 2.043 | 2.321 | 83.328 | 0.27x |
| users.json $[*].orders[*].total | jsonpath-ng | 11.646 | 12.268 | 14.190 | 83.328 | 0.04x |
| users.json $..total | strata | 1.328 | 1.462 | 1.740 | 85.250 | 1.00x |
| users.json $..total | jsonpath-ng | 203.229 | 212.079 | 245.002 | 85.250 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.860 | 4.023 | 4.472 | 83.281 | 1.00x |
| users.json $[*].id | orjson+jmespath | 11.108 | 11.789 | 13.645 | 83.281 | 0.34x |
| users.json $[*].id | orjson+jsonpath-ng | 12.581 | 13.288 | 14.228 | 83.281 | 0.30x |
| users.json $[*].orders[*].total | strata | 3.901 | 4.196 | 4.422 | 85.203 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.403 | 13.849 | 15.424 | 85.203 | 0.30x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 25.221 | 28.656 | 31.492 | 85.203 | 0.15x |
| users.json $..total | strata | 9.076 | 10.495 | 14.900 | 85.250 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 236.877 | 269.147 | 306.109 | 85.250 | 0.04x |

