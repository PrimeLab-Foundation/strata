# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: ab20434
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
| users.json | strata | 6.984 | 7.434 | 8.954 | 63.594 | 1.00x |
| users.json | orjson | 8.962 | 10.578 | 12.383 | 63.594 | 0.70x |
| users.json | msgspec | 9.004 | 10.438 | 12.955 | 63.594 | 0.71x |
| users.json | ujson | 11.537 | 14.715 | 19.107 | 63.594 | 0.51x |
| users.json | pysimdjson | 128.747 | 134.121 | 152.066 | 63.594 | 0.06x |
| users.json | json | 14.983 | 16.381 | 21.227 | 63.594 | 0.45x |
| flat.json | strata | 0.598 | 0.674 | 0.802 | 89.250 | 1.00x |
| flat.json | orjson | 0.783 | 0.819 | 0.965 | 89.250 | 0.82x |
| flat.json | msgspec | 0.741 | 0.787 | 0.856 | 89.250 | 0.86x |
| flat.json | ujson | 1.171 | 1.293 | 1.446 | 89.250 | 0.52x |
| flat.json | pysimdjson | 12.032 | 12.639 | 12.995 | 89.250 | 0.05x |
| flat.json | json | 1.340 | 1.469 | 1.572 | 89.250 | 0.46x |
| nested.json | strata | 0.548 | 0.568 | 1.351 | 89.281 | 1.00x |
| nested.json | orjson | 0.757 | 0.791 | 1.146 | 89.281 | 0.72x |
| nested.json | msgspec | 0.707 | 0.719 | 0.768 | 89.281 | 0.79x |
| nested.json | ujson | 1.090 | 1.194 | 1.311 | 89.281 | 0.48x |
| nested.json | pysimdjson | 10.654 | 11.001 | 13.264 | 89.281 | 0.05x |
| nested.json | json | 1.457 | 1.497 | 2.018 | 89.281 | 0.38x |
| wide_arrays.json | strata | 3.935 | 4.165 | 5.930 | 92.047 | 1.00x |
| wide_arrays.json | orjson | 3.907 | 4.679 | 9.761 | 92.047 | 0.89x |
| wide_arrays.json | msgspec | 4.398 | 4.977 | 5.652 | 92.047 | 0.84x |
| wide_arrays.json | ujson | 6.441 | 6.834 | 8.716 | 92.047 | 0.61x |
| wide_arrays.json | pysimdjson | 69.916 | 74.101 | 78.336 | 92.047 | 0.06x |
| wide_arrays.json | json | 7.542 | 8.412 | 14.109 | 92.047 | 0.50x |
| mixed.json | strata | 0.148 | 0.166 | 0.242 | 92.062 | 1.00x |
| mixed.json | orjson | 0.184 | 0.204 | 0.627 | 92.062 | 0.82x |
| mixed.json | msgspec | 0.200 | 0.211 | 0.764 | 92.062 | 0.79x |
| mixed.json | ujson | 0.247 | 0.421 | 1.056 | 92.062 | 0.40x |
| mixed.json | pysimdjson | 2.717 | 2.845 | 3.687 | 92.062 | 0.06x |
| mixed.json | json | 0.371 | 0.405 | 1.040 | 92.062 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.752 | 1.960 | 4.147 | 79.172 | 1.00x |
| users.json | orjson | 2.199 | 2.609 | 3.471 | 79.172 | 0.75x |
| users.json | msgspec | 2.988 | 3.560 | 6.012 | 79.172 | 0.55x |
| users.json | ujson | 11.574 | 12.865 | 18.808 | 79.172 | 0.15x |
| users.json | json | 16.027 | 19.339 | 27.760 | 79.172 | 0.10x |
| flat.json | strata | 0.226 | 0.266 | 0.473 | 89.266 | 1.00x |
| flat.json | orjson | 0.265 | 0.410 | 0.470 | 89.266 | 0.65x |
| flat.json | msgspec | 0.338 | 0.356 | 0.481 | 89.266 | 0.75x |
| flat.json | ujson | 1.012 | 1.035 | 1.101 | 89.266 | 0.26x |
| flat.json | json | 1.473 | 1.629 | 3.219 | 89.266 | 0.16x |
| nested.json | strata | 0.140 | 0.152 | 0.200 | 89.281 | 1.00x |
| nested.json | orjson | 0.232 | 0.249 | 0.272 | 89.281 | 0.61x |
| nested.json | msgspec | 0.412 | 0.441 | 0.511 | 89.281 | 0.34x |
| nested.json | ujson | 1.092 | 1.145 | 1.197 | 89.281 | 0.13x |
| nested.json | json | 1.736 | 1.817 | 1.915 | 89.281 | 0.08x |
| wide_arrays.json | strata | 1.130 | 1.294 | 1.696 | 92.047 | 1.00x |
| wide_arrays.json | orjson | 1.523 | 1.598 | 3.069 | 92.047 | 0.81x |
| wide_arrays.json | msgspec | 2.359 | 2.521 | 2.883 | 92.047 | 0.51x |
| wide_arrays.json | ujson | 6.777 | 7.299 | 9.104 | 92.047 | 0.18x |
| wide_arrays.json | json | 12.602 | 13.341 | 14.563 | 92.047 | 0.10x |
| mixed.json | strata | 0.034 | 0.038 | 0.056 | 92.062 | 1.00x |
| mixed.json | orjson | 0.039 | 0.041 | 0.071 | 92.062 | 0.92x |
| mixed.json | msgspec | 0.047 | 0.048 | 0.059 | 92.062 | 0.79x |
| mixed.json | ujson | 0.202 | 0.205 | 0.217 | 92.062 | 0.18x |
| mixed.json | json | 0.320 | 0.327 | 0.364 | 92.062 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.288 | 8.596 | 11.788 | 89.203 | 1.00x |
| users.json | orjson | 10.422 | 11.903 | 17.101 | 89.203 | 0.72x |
| users.json | msgspec | 10.096 | 12.378 | 22.506 | 89.203 | 0.69x |
| users.json | ujson | 13.942 | 17.790 | 28.959 | 89.203 | 0.48x |
| users.json | json | 15.200 | 18.959 | 26.317 | 89.203 | 0.45x |
| flat.json | strata | 0.736 | 0.761 | 1.019 | 89.266 | 1.00x |
| flat.json | orjson | 0.964 | 1.046 | 1.964 | 89.266 | 0.73x |
| flat.json | msgspec | 0.879 | 0.938 | 1.682 | 89.266 | 0.81x |
| flat.json | ujson | 1.198 | 1.407 | 3.106 | 89.266 | 0.54x |
| flat.json | json | 1.505 | 1.603 | 2.232 | 89.266 | 0.47x |
| nested.json | strata | 0.612 | 0.653 | 0.794 | 89.281 | 1.00x |
| nested.json | orjson | 0.940 | 1.020 | 1.657 | 89.281 | 0.64x |
| nested.json | msgspec | 0.792 | 0.890 | 1.538 | 89.281 | 0.73x |
| nested.json | ujson | 1.112 | 1.203 | 1.958 | 89.281 | 0.54x |
| nested.json | json | 1.523 | 1.622 | 2.039 | 89.281 | 0.40x |
| wide_arrays.json | strata | 3.696 | 4.236 | 6.215 | 92.047 | 1.00x |
| wide_arrays.json | orjson | 3.818 | 4.649 | 5.914 | 92.047 | 0.91x |
| wide_arrays.json | msgspec | 4.493 | 5.159 | 5.633 | 92.047 | 0.82x |
| wide_arrays.json | ujson | 5.846 | 7.059 | 13.707 | 92.047 | 0.60x |
| wide_arrays.json | json | 7.263 | 8.234 | 11.672 | 92.047 | 0.51x |
| mixed.json | strata | 0.139 | 0.142 | 0.172 | 92.062 | 1.00x |
| mixed.json | orjson | 0.178 | 0.226 | 0.314 | 92.062 | 0.63x |
| mixed.json | msgspec | 0.184 | 0.192 | 0.237 | 92.062 | 0.74x |
| mixed.json | ujson | 0.226 | 0.238 | 0.299 | 92.062 | 0.59x |
| mixed.json | json | 0.323 | 0.328 | 0.444 | 92.062 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.865 | 8.095 | 9.619 | 89.250 | 1.00x |
| users.ndjson | orjson | 11.659 | 12.894 | 20.981 | 89.250 | 0.63x |
| users.ndjson | msgspec | 11.862 | 12.681 | 21.391 | 89.250 | 0.64x |
| users.ndjson | ujson | 13.468 | 15.456 | 27.195 | 89.250 | 0.52x |
| users.ndjson | json | 18.546 | 20.449 | 37.440 | 89.250 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.173 | 2.655 | 6.847 | 85.594 | 1.00x |
| users.json | orjson | 2.829 | 3.367 | 7.665 | 85.594 | 0.79x |
| users.json | msgspec | 3.316 | 4.348 | 7.848 | 85.594 | 0.61x |
| users.json | ujson | 11.481 | 13.847 | 17.922 | 85.594 | 0.19x |
| users.json | json | 16.132 | 18.799 | 21.280 | 85.594 | 0.14x |
| flat.json | strata | 0.451 | 0.500 | 0.621 | 89.266 | 1.00x |
| flat.json | orjson | 0.462 | 0.495 | 0.677 | 89.266 | 1.01x |
| flat.json | msgspec | 0.517 | 0.566 | 0.811 | 89.266 | 0.88x |
| flat.json | ujson | 1.174 | 1.232 | 1.347 | 89.266 | 0.41x |
| flat.json | json | 1.671 | 1.730 | 1.920 | 89.266 | 0.29x |
| nested.json | strata | 0.323 | 0.448 | 0.582 | 89.281 | 1.00x |
| nested.json | orjson | 0.381 | 0.555 | 0.675 | 89.281 | 0.81x |
| nested.json | msgspec | 0.640 | 0.762 | 0.909 | 89.281 | 0.59x |
| nested.json | ujson | 1.361 | 1.649 | 1.742 | 89.281 | 0.27x |
| nested.json | json | 1.984 | 2.224 | 2.375 | 89.281 | 0.20x |
| wide_arrays.json | strata | 1.467 | 2.032 | 4.144 | 92.047 | 1.00x |
| wide_arrays.json | orjson | 1.661 | 2.393 | 4.248 | 92.047 | 0.85x |
| wide_arrays.json | msgspec | 2.770 | 3.266 | 4.146 | 92.047 | 0.62x |
| wide_arrays.json | ujson | 8.034 | 8.571 | 12.436 | 92.047 | 0.24x |
| wide_arrays.json | json | 12.492 | 14.339 | 15.007 | 92.047 | 0.14x |
| mixed.json | strata | 0.140 | 0.275 | 0.383 | 92.062 | 1.00x |
| mixed.json | orjson | 0.164 | 0.296 | 0.376 | 92.062 | 0.93x |
| mixed.json | msgspec | 0.175 | 0.431 | 0.800 | 92.062 | 0.64x |
| mixed.json | ujson | 0.404 | 0.546 | 0.616 | 92.062 | 0.50x |
| mixed.json | json | 0.567 | 0.724 | 1.667 | 92.062 | 0.38x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.117 | 0.209 | 3.828 | 85.625 | 1.00x |
| users.json $[*].id | jmespath | 0.399 | 0.519 | 1.317 | 85.625 | 0.40x |
| users.json $[*].id | jsonpath-ng | 1.688 | 1.901 | 6.084 | 85.625 | 0.11x |
| users.json $[*].orders[*].total | strata | 0.406 | 0.825 | 2.585 | 85.797 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.882 | 2.345 | 3.549 | 85.797 | 0.35x |
| users.json $[*].orders[*].total | jsonpath-ng | 12.692 | 16.365 | 21.254 | 85.797 | 0.05x |
| users.json $..total | strata | 1.435 | 1.694 | 2.418 | 87.656 | 1.00x |
| users.json $..total | jsonpath-ng | 238.233 | 267.152 | 323.463 | 87.656 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.825 | 4.467 | 6.163 | 85.672 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.658 | 14.981 | 20.099 | 85.672 | 0.30x |
| users.json $[*].id | orjson+jsonpath-ng | 11.700 | 16.362 | 23.542 | 85.672 | 0.27x |
| users.json $[*].orders[*].total | strata | 4.151 | 4.774 | 11.352 | 87.609 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 13.703 | 16.373 | 21.975 | 87.609 | 0.29x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 24.678 | 37.703 | 87.237 | 87.609 | 0.13x |
| users.json $..total | strata | 8.692 | 10.879 | 18.504 | 87.656 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 209.471 | 251.773 | 276.796 | 87.656 | 0.04x |

