# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 79fa3df
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
| users.json | strata | 5.797 | 6.195 | 6.911 | 63.719 | 1.00x |
| users.json | orjson | 8.631 | 9.212 | 11.051 | 63.719 | 0.67x |
| users.json | msgspec | 8.303 | 8.886 | 10.193 | 63.719 | 0.70x |
| users.json | ujson | 11.017 | 11.474 | 14.741 | 63.719 | 0.54x |
| users.json | pysimdjson | 115.530 | 119.126 | 139.378 | 63.719 | 0.05x |
| users.json | json | 13.539 | 13.777 | 16.339 | 63.719 | 0.45x |
| flat.json | strata | 0.544 | 0.552 | 0.572 | 90.469 | 1.00x |
| flat.json | orjson | 0.668 | 0.676 | 0.721 | 90.469 | 0.82x |
| flat.json | msgspec | 0.654 | 0.658 | 0.678 | 90.469 | 0.84x |
| flat.json | ujson | 1.007 | 1.041 | 1.145 | 90.469 | 0.53x |
| flat.json | pysimdjson | 11.081 | 11.120 | 11.621 | 90.469 | 0.05x |
| flat.json | json | 1.247 | 1.261 | 1.394 | 90.469 | 0.44x |
| nested.json | strata | 0.472 | 0.474 | 0.491 | 90.484 | 1.00x |
| nested.json | orjson | 0.643 | 0.648 | 0.673 | 90.484 | 0.73x |
| nested.json | msgspec | 0.631 | 0.632 | 0.647 | 90.484 | 0.75x |
| nested.json | ujson | 0.961 | 0.975 | 1.048 | 90.484 | 0.49x |
| nested.json | pysimdjson | 9.687 | 9.729 | 9.874 | 90.484 | 0.05x |
| nested.json | json | 1.300 | 1.309 | 1.328 | 90.484 | 0.36x |
| wide_arrays.json | strata | 2.782 | 2.803 | 3.080 | 93.578 | 1.00x |
| wide_arrays.json | orjson | 3.288 | 3.313 | 3.717 | 93.578 | 0.85x |
| wide_arrays.json | msgspec | 3.772 | 3.824 | 4.195 | 93.578 | 0.73x |
| wide_arrays.json | ujson | 4.833 | 4.965 | 5.377 | 93.578 | 0.56x |
| wide_arrays.json | pysimdjson | 59.685 | 60.357 | 65.042 | 93.578 | 0.05x |
| wide_arrays.json | json | 6.337 | 6.439 | 7.041 | 93.578 | 0.44x |
| mixed.json | strata | 0.111 | 0.111 | 0.113 | 93.594 | 1.00x |
| mixed.json | orjson | 0.142 | 0.147 | 0.151 | 93.594 | 0.76x |
| mixed.json | msgspec | 0.155 | 0.157 | 0.171 | 93.594 | 0.71x |
| mixed.json | ujson | 0.192 | 0.214 | 0.250 | 93.594 | 0.52x |
| mixed.json | pysimdjson | 2.337 | 2.350 | 2.375 | 93.594 | 0.05x |
| mixed.json | json | 0.299 | 0.300 | 0.305 | 93.594 | 0.37x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.384 | 1.408 | 1.547 | 76.781 | 1.00x |
| users.json | orjson | 2.078 | 2.147 | 2.193 | 76.781 | 0.66x |
| users.json | msgspec | 2.709 | 2.736 | 2.857 | 76.781 | 0.51x |
| users.json | ujson | 8.272 | 8.296 | 8.819 | 76.781 | 0.17x |
| users.json | json | 14.685 | 14.837 | 15.023 | 76.781 | 0.09x |
| flat.json | strata | 0.195 | 0.201 | 0.240 | 90.484 | 1.00x |
| flat.json | orjson | 0.234 | 0.242 | 0.253 | 90.484 | 0.83x |
| flat.json | msgspec | 0.289 | 0.299 | 0.355 | 90.484 | 0.67x |
| flat.json | ujson | 0.717 | 0.733 | 1.000 | 90.484 | 0.27x |
| flat.json | json | 1.298 | 1.339 | 1.374 | 90.484 | 0.15x |
| nested.json | strata | 0.124 | 0.125 | 0.130 | 90.500 | 1.00x |
| nested.json | orjson | 0.206 | 0.207 | 0.233 | 90.500 | 0.60x |
| nested.json | msgspec | 0.265 | 0.266 | 0.288 | 90.500 | 0.47x |
| nested.json | ujson | 0.842 | 0.858 | 1.057 | 90.500 | 0.15x |
| nested.json | json | 1.521 | 1.534 | 3.063 | 90.500 | 0.08x |
| wide_arrays.json | strata | 0.967 | 0.979 | 0.996 | 93.578 | 1.00x |
| wide_arrays.json | orjson | 1.297 | 1.320 | 1.410 | 93.578 | 0.74x |
| wide_arrays.json | msgspec | 1.938 | 1.947 | 2.003 | 93.578 | 0.50x |
| wide_arrays.json | ujson | 4.468 | 4.485 | 4.516 | 93.578 | 0.22x |
| wide_arrays.json | json | 10.947 | 10.979 | 11.237 | 93.578 | 0.09x |
| mixed.json | strata | 0.032 | 0.033 | 0.035 | 93.594 | 1.00x |
| mixed.json | orjson | 0.039 | 0.041 | 0.042 | 93.594 | 0.79x |
| mixed.json | msgspec | 0.046 | 0.073 | 0.158 | 93.594 | 0.45x |
| mixed.json | ujson | 0.157 | 0.159 | 0.168 | 93.594 | 0.20x |
| mixed.json | json | 0.327 | 0.332 | 0.346 | 93.594 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.168 | 6.200 | 6.354 | 88.672 | 1.00x |
| users.json | orjson | 8.905 | 8.985 | 9.060 | 88.672 | 0.69x |
| users.json | msgspec | 8.677 | 8.704 | 8.963 | 88.672 | 0.71x |
| users.json | ujson | 11.686 | 11.809 | 11.925 | 88.672 | 0.53x |
| users.json | json | 14.024 | 14.098 | 15.280 | 88.672 | 0.44x |
| flat.json | strata | 0.593 | 0.621 | 0.730 | 90.484 | 1.00x |
| flat.json | orjson | 0.768 | 0.888 | 0.918 | 90.484 | 0.70x |
| flat.json | msgspec | 0.726 | 0.759 | 0.800 | 90.484 | 0.82x |
| flat.json | ujson | 1.064 | 1.084 | 1.156 | 90.484 | 0.57x |
| flat.json | json | 1.304 | 1.364 | 1.504 | 90.484 | 0.46x |
| nested.json | strata | 0.504 | 0.510 | 0.524 | 90.500 | 1.00x |
| nested.json | orjson | 0.715 | 0.737 | 0.785 | 90.500 | 0.69x |
| nested.json | msgspec | 0.673 | 0.682 | 0.818 | 90.500 | 0.75x |
| nested.json | ujson | 0.941 | 0.952 | 1.007 | 90.500 | 0.54x |
| nested.json | json | 1.346 | 1.358 | 1.365 | 90.500 | 0.38x |
| wide_arrays.json | strata | 2.943 | 2.975 | 3.405 | 93.578 | 1.00x |
| wide_arrays.json | orjson | 3.510 | 3.526 | 3.596 | 93.578 | 0.84x |
| wide_arrays.json | msgspec | 4.106 | 4.158 | 4.192 | 93.578 | 0.72x |
| wide_arrays.json | ujson | 5.280 | 5.326 | 5.529 | 93.578 | 0.56x |
| wide_arrays.json | json | 6.666 | 6.713 | 7.055 | 93.578 | 0.44x |
| mixed.json | strata | 0.130 | 0.132 | 0.134 | 93.594 | 1.00x |
| mixed.json | orjson | 0.174 | 0.221 | 0.285 | 93.594 | 0.60x |
| mixed.json | msgspec | 0.183 | 0.188 | 0.203 | 93.594 | 0.70x |
| mixed.json | ujson | 0.228 | 0.230 | 0.251 | 93.594 | 0.57x |
| mixed.json | json | 0.326 | 0.328 | 0.371 | 93.594 | 0.40x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.266 | 6.313 | 7.158 | 90.469 | 1.00x |
| users.ndjson | orjson | 10.721 | 10.779 | 12.056 | 90.469 | 0.59x |
| users.ndjson | msgspec | 10.596 | 10.640 | 12.004 | 90.469 | 0.59x |
| users.ndjson | ujson | 13.195 | 13.285 | 14.959 | 90.469 | 0.48x |
| users.ndjson | json | 17.013 | 17.131 | 18.961 | 90.469 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.668 | 1.765 | 2.787 | 85.703 | 1.00x |
| users.json | orjson | 2.508 | 2.585 | 3.268 | 85.703 | 0.68x |
| users.json | msgspec | 3.092 | 3.188 | 3.499 | 85.703 | 0.55x |
| users.json | ujson | 8.928 | 9.039 | 9.908 | 85.703 | 0.20x |
| users.json | json | 15.598 | 15.667 | 16.626 | 85.703 | 0.11x |
| flat.json | strata | 0.297 | 0.312 | 0.324 | 90.484 | 1.00x |
| flat.json | orjson | 0.338 | 0.341 | 0.439 | 90.484 | 0.91x |
| flat.json | msgspec | 0.388 | 0.395 | 0.411 | 90.484 | 0.79x |
| flat.json | ujson | 0.823 | 0.841 | 0.878 | 90.484 | 0.37x |
| flat.json | json | 1.437 | 1.462 | 1.558 | 90.484 | 0.21x |
| nested.json | strata | 0.221 | 0.227 | 0.255 | 90.500 | 1.00x |
| nested.json | orjson | 0.295 | 0.307 | 0.383 | 90.500 | 0.74x |
| nested.json | msgspec | 0.367 | 0.433 | 0.555 | 90.500 | 0.53x |
| nested.json | ujson | 0.863 | 0.902 | 0.945 | 90.500 | 0.25x |
| nested.json | json | 1.627 | 1.642 | 1.671 | 90.500 | 0.14x |
| wide_arrays.json | strata | 1.241 | 1.312 | 1.476 | 93.578 | 1.00x |
| wide_arrays.json | orjson | 1.594 | 1.631 | 1.882 | 93.578 | 0.80x |
| wide_arrays.json | msgspec | 2.240 | 2.309 | 2.679 | 93.578 | 0.57x |
| wide_arrays.json | ujson | 4.851 | 5.005 | 5.631 | 93.578 | 0.26x |
| wide_arrays.json | json | 11.289 | 11.458 | 12.348 | 93.578 | 0.11x |
| mixed.json | strata | 0.110 | 0.122 | 0.144 | 93.594 | 1.00x |
| mixed.json | orjson | 0.119 | 0.130 | 0.211 | 93.594 | 0.94x |
| mixed.json | msgspec | 0.130 | 0.141 | 0.261 | 93.594 | 0.86x |
| mixed.json | ujson | 0.253 | 0.268 | 0.503 | 93.594 | 0.45x |
| mixed.json | json | 0.410 | 0.426 | 0.533 | 93.594 | 0.29x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.044 | 0.045 | 0.051 | 85.781 | 1.00x |
| users.json $[*].id | jmespath | 0.252 | 0.255 | 0.291 | 85.781 | 0.18x |
| users.json $[*].id | jsonpath-ng | 1.395 | 1.410 | 1.438 | 85.781 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.264 | 0.274 | 0.349 | 85.953 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.528 | 1.756 | 1.891 | 85.953 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 9.573 | 9.656 | 10.415 | 85.953 | 0.03x |
| users.json $..total | strata | 1.198 | 1.216 | 1.401 | 86.984 | 1.00x |
| users.json $..total | jsonpath-ng | 176.654 | 177.714 | 179.715 | 86.984 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.372 | 3.395 | 3.529 | 85.844 | 1.00x |
| users.json $[*].id | orjson+jmespath | 9.080 | 9.130 | 9.319 | 85.844 | 0.37x |
| users.json $[*].id | orjson+jsonpath-ng | 10.289 | 10.340 | 10.434 | 85.844 | 0.33x |
| users.json $[*].orders[*].total | strata | 3.488 | 3.533 | 3.556 | 86.953 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 10.408 | 10.551 | 10.723 | 86.953 | 0.33x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 20.238 | 20.441 | 20.559 | 86.953 | 0.17x |
| users.json $..total | strata | 7.462 | 7.646 | 8.572 | 86.797 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 187.013 | 191.534 | 208.345 | 86.797 | 0.04x |

