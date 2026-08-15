# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c36c406
- python: 3.14.3
- implementation: CPython
- platform: macOS-26.3-arm64-arm-64bit-Mach-O
- machine: arm64
- processor: Apple M1 Max
- compiler_flags: -std=c++20 -O3 -march=native
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.639 | 6.760 | 7.482 | 52.859 | 1.00x |
| users.json | orjson | 8.311 | 8.399 | 8.798 | 52.859 | 0.80x |
| users.json | msgspec | 8.362 | 8.479 | 8.901 | 52.859 | 0.80x |
| users.json | ujson | 11.065 | 11.239 | 11.677 | 52.859 | 0.60x |
| users.json | json | 14.590 | 14.777 | 15.169 | 52.859 | 0.46x |
| flat.json | strata | 0.641 | 0.647 | 0.675 | 69.953 | 1.00x |
| flat.json | orjson | 0.655 | 0.659 | 0.699 | 69.953 | 0.98x |
| flat.json | msgspec | 0.687 | 0.693 | 0.700 | 69.953 | 0.93x |
| flat.json | ujson | 1.009 | 1.016 | 1.030 | 69.953 | 0.64x |
| flat.json | json | 1.416 | 1.430 | 1.521 | 69.953 | 0.45x |
| nested.json | strata | 0.523 | 0.534 | 0.592 | 69.984 | 1.00x |
| nested.json | orjson | 1.205 | 1.231 | 1.254 | 69.984 | 0.43x |
| nested.json | msgspec | 1.209 | 1.226 | 1.344 | 69.984 | 0.44x |
| nested.json | ujson | 1.473 | 1.486 | 1.565 | 69.984 | 0.36x |
| nested.json | json | 1.934 | 1.942 | 2.039 | 69.984 | 0.27x |
| wide_arrays.json | strata | 3.308 | 3.351 | 3.845 | 70.047 | 1.00x |
| wide_arrays.json | orjson | 3.785 | 3.841 | 4.000 | 70.047 | 0.87x |
| wide_arrays.json | msgspec | 4.500 | 4.587 | 5.168 | 70.047 | 0.73x |
| wide_arrays.json | ujson | 5.671 | 5.773 | 6.244 | 70.047 | 0.58x |
| wide_arrays.json | json | 8.030 | 8.163 | 8.932 | 70.047 | 0.41x |
| mixed.json | strata | 0.129 | 0.131 | 0.258 | 70.047 | 1.00x |
| mixed.json | orjson | 0.125 | 0.130 | 0.136 | 70.047 | 1.01x |
| mixed.json | msgspec | 0.136 | 0.140 | 0.161 | 70.047 | 0.94x |
| mixed.json | ujson | 0.183 | 0.188 | 0.197 | 70.047 | 0.70x |
| mixed.json | json | 0.298 | 0.303 | 0.355 | 70.047 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.715 | 1.739 | 1.787 | 56.766 | 1.00x |
| users.json | orjson | 1.979 | 1.995 | 2.068 | 56.766 | 0.87x |
| users.json | msgspec | 2.574 | 2.617 | 2.735 | 56.766 | 0.66x |
| users.json | ujson | 10.906 | 11.050 | 11.377 | 56.766 | 0.16x |
| users.json | json | 14.944 | 15.005 | 15.421 | 56.766 | 0.12x |
| flat.json | strata | 0.182 | 0.186 | 0.206 | 69.984 | 1.00x |
| flat.json | orjson | 0.222 | 0.229 | 0.238 | 69.984 | 0.81x |
| flat.json | msgspec | 0.292 | 0.297 | 0.322 | 69.984 | 0.63x |
| flat.json | ujson | 0.884 | 0.897 | 0.959 | 69.984 | 0.21x |
| flat.json | json | 1.251 | 1.275 | 1.403 | 69.984 | 0.15x |
| nested.json | strata | 0.148 | 0.151 | 0.176 | 70.047 | 1.00x |
| nested.json | orjson | 0.199 | 0.201 | 0.248 | 70.047 | 0.75x |
| nested.json | msgspec | 0.266 | 0.270 | 0.283 | 70.047 | 0.56x |
| nested.json | ujson | 0.995 | 1.010 | 1.048 | 70.047 | 0.15x |
| nested.json | json | 1.565 | 1.581 | 1.604 | 70.047 | 0.10x |
| wide_arrays.json | strata | 1.093 | 1.107 | 1.162 | 70.047 | 1.00x |
| wide_arrays.json | orjson | 1.121 | 1.131 | 1.172 | 70.047 | 0.98x |
| wide_arrays.json | msgspec | 1.926 | 1.950 | 2.116 | 70.047 | 0.57x |
| wide_arrays.json | ujson | 5.992 | 6.046 | 6.229 | 70.047 | 0.18x |
| wide_arrays.json | json | 10.657 | 10.759 | 11.197 | 70.047 | 0.10x |
| mixed.json | strata | 0.041 | 0.042 | 0.047 | 70.094 | 1.00x |
| mixed.json | orjson | 0.042 | 0.043 | 0.047 | 70.094 | 0.97x |
| mixed.json | msgspec | 0.048 | 0.050 | 0.059 | 70.094 | 0.84x |
| mixed.json | ujson | 0.214 | 0.215 | 0.243 | 70.094 | 0.19x |
| mixed.json | json | 0.331 | 0.333 | 0.346 | 70.094 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.934 | 7.054 | 7.440 | 70.047 | 1.00x |
| users.json | orjson | 10.338 | 10.469 | 10.900 | 70.047 | 0.67x |
| users.json | msgspec | 10.494 | 10.559 | 11.729 | 70.047 | 0.67x |
| users.json | ujson | 13.435 | 13.749 | 14.957 | 70.047 | 0.51x |
| users.json | json | 16.705 | 17.142 | 17.625 | 70.047 | 0.41x |
| flat.json | strata | 0.713 | 0.747 | 0.811 | 69.984 | 1.00x |
| flat.json | orjson | 0.727 | 0.743 | 0.834 | 69.984 | 1.01x |
| flat.json | msgspec | 0.761 | 0.796 | 0.906 | 69.984 | 0.94x |
| flat.json | ujson | 1.112 | 1.173 | 1.294 | 69.984 | 0.64x |
| flat.json | json | 1.489 | 1.515 | 1.678 | 69.984 | 0.49x |
| nested.json | strata | 0.592 | 0.601 | 0.647 | 70.047 | 1.00x |
| nested.json | orjson | 1.366 | 1.412 | 1.455 | 70.047 | 0.43x |
| nested.json | msgspec | 1.367 | 1.405 | 1.501 | 70.047 | 0.43x |
| nested.json | ujson | 1.674 | 1.711 | 1.919 | 70.047 | 0.35x |
| nested.json | json | 2.114 | 2.169 | 2.647 | 70.047 | 0.28x |
| wide_arrays.json | strata | 3.504 | 3.553 | 3.603 | 70.047 | 1.00x |
| wide_arrays.json | orjson | 4.254 | 4.305 | 4.499 | 70.047 | 0.83x |
| wide_arrays.json | msgspec | 5.034 | 5.112 | 5.375 | 70.047 | 0.70x |
| wide_arrays.json | ujson | 6.323 | 6.422 | 6.886 | 70.047 | 0.55x |
| wide_arrays.json | json | 8.504 | 8.625 | 9.104 | 70.047 | 0.41x |
| mixed.json | strata | 0.166 | 0.179 | 0.214 | 70.094 | 1.00x |
| mixed.json | orjson | 0.169 | 0.175 | 0.232 | 70.094 | 1.02x |
| mixed.json | msgspec | 0.184 | 0.187 | 0.208 | 70.094 | 0.96x |
| mixed.json | ujson | 0.236 | 0.245 | 0.258 | 70.094 | 0.73x |
| mixed.json | json | 0.339 | 0.346 | 0.378 | 70.094 | 0.52x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.132 | 7.249 | 7.561 | 69.094 | 1.00x |
| users.ndjson | orjson | 10.297 | 10.538 | 11.326 | 69.094 | 0.69x |
| users.ndjson | msgspec | 10.161 | 10.298 | 11.160 | 69.094 | 0.70x |
| users.ndjson | ujson | 12.999 | 13.117 | 14.330 | 69.094 | 0.55x |
| users.ndjson | json | 17.730 | 18.062 | 19.513 | 69.094 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.057 | 2.129 | 2.311 | 68.422 | 1.00x |
| users.json | orjson | 2.359 | 2.414 | 2.477 | 68.422 | 0.88x |
| users.json | msgspec | 2.961 | 3.026 | 3.144 | 68.422 | 0.70x |
| users.json | ujson | 11.069 | 11.310 | 11.552 | 68.422 | 0.19x |
| users.json | json | 15.260 | 15.456 | 15.969 | 68.422 | 0.14x |
| flat.json | strata | 0.296 | 0.325 | 0.361 | 69.984 | 1.00x |
| flat.json | orjson | 0.352 | 0.364 | 0.403 | 69.984 | 0.89x |
| flat.json | msgspec | 0.411 | 0.420 | 0.460 | 69.984 | 0.77x |
| flat.json | ujson | 1.007 | 1.019 | 1.074 | 69.984 | 0.32x |
| flat.json | json | 1.375 | 1.401 | 1.634 | 69.984 | 0.23x |
| nested.json | strata | 0.264 | 0.298 | 0.532 | 70.047 | 1.00x |
| nested.json | orjson | 0.317 | 0.347 | 0.379 | 70.047 | 0.86x |
| nested.json | msgspec | 0.393 | 0.423 | 0.497 | 70.047 | 0.70x |
| nested.json | ujson | 1.147 | 1.175 | 1.200 | 70.047 | 0.25x |
| nested.json | json | 1.709 | 1.729 | 1.889 | 70.047 | 0.17x |
| wide_arrays.json | strata | 1.370 | 1.412 | 1.735 | 70.047 | 1.00x |
| wide_arrays.json | orjson | 1.412 | 1.429 | 1.688 | 70.047 | 0.99x |
| wide_arrays.json | msgspec | 2.212 | 2.242 | 2.608 | 70.047 | 0.63x |
| wide_arrays.json | ujson | 6.266 | 6.351 | 6.757 | 70.047 | 0.22x |
| wide_arrays.json | json | 11.004 | 11.147 | 11.536 | 70.047 | 0.13x |
| mixed.json | strata | 0.133 | 0.149 | 0.217 | 70.125 | 1.00x |
| mixed.json | orjson | 0.143 | 0.160 | 0.170 | 70.125 | 0.93x |
| mixed.json | msgspec | 0.151 | 0.168 | 0.269 | 70.125 | 0.89x |
| mixed.json | ujson | 0.327 | 0.348 | 0.393 | 70.125 | 0.43x |
| mixed.json | json | 0.438 | 0.461 | 0.531 | 70.125 | 0.32x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.052 | 0.066 | 0.077 | 68.734 | 1.00x |
| users.json $[*].id | jmespath | 0.337 | 0.342 | 0.418 | 68.734 | 0.19x |
| users.json $[*].id | jsonpath-ng | 3.355 | 3.473 | 3.984 | 68.734 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.340 | 0.353 | 0.432 | 69.172 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.038 | 2.058 | 2.325 | 69.172 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 11.256 | 11.475 | 12.479 | 69.172 | 0.03x |
| users.json $..total | strata | 1.352 | 1.365 | 1.385 | 70.234 | 1.00x |
| users.json $..total | jsonpath-ng | 206.038 | 207.853 | 214.033 | 70.234 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.185 | 3.239 | 3.351 | 68.797 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.800 | 10.918 | 11.441 | 68.797 | 0.30x |
| users.json $[*].id | orjson+jsonpath-ng | 11.976 | 12.062 | 12.814 | 68.797 | 0.27x |
| users.json $[*].orders[*].total | strata | 3.300 | 3.348 | 3.567 | 70.203 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.602 | 12.769 | 14.244 | 70.203 | 0.26x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 20.095 | 20.236 | 21.337 | 70.203 | 0.17x |
| users.json $..total | strata | 10.507 | 10.720 | 11.736 | 70.234 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 215.964 | 216.364 | 222.244 | 70.234 | 0.05x |

