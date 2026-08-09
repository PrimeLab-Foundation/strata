# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 67f0cbd
- python: 3.14.3
- implementation: CPython
- platform: macOS-26.3-arm64-arm-64bit-Mach-O
- machine: arm64
- processor: Apple M1 Max
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.988 | 10.138 | 10.482 | 52.719 | 1.00x |
| users.json | orjson | 10.685 | 10.951 | 11.436 | 52.719 | 0.93x |
| users.json | msgspec | 10.834 | 10.992 | 11.463 | 52.719 | 0.92x |
| users.json | ujson | 14.322 | 14.709 | 15.922 | 52.719 | 0.69x |
| users.json | json | 18.997 | 19.157 | 19.932 | 52.719 | 0.53x |
| flat.json | strata | 0.998 | 1.009 | 1.076 | 67.844 | 1.00x |
| flat.json | orjson | 0.856 | 0.861 | 0.923 | 67.844 | 1.17x |
| flat.json | msgspec | 0.897 | 0.906 | 0.928 | 67.844 | 1.11x |
| flat.json | ujson | 1.329 | 1.359 | 1.464 | 67.844 | 0.74x |
| flat.json | json | 1.850 | 1.865 | 1.913 | 67.844 | 0.54x |
| nested.json | strata | 0.794 | 0.799 | 0.806 | 67.859 | 1.00x |
| nested.json | orjson | 1.550 | 1.582 | 1.775 | 67.859 | 0.51x |
| nested.json | msgspec | 1.555 | 1.577 | 1.634 | 67.859 | 0.51x |
| nested.json | ujson | 1.902 | 1.937 | 2.113 | 67.859 | 0.41x |
| nested.json | json | 2.506 | 2.532 | 2.697 | 67.859 | 0.32x |
| wide_arrays.json | strata | 4.728 | 4.758 | 4.851 | 67.859 | 1.00x |
| wide_arrays.json | orjson | 4.892 | 4.989 | 5.407 | 67.859 | 0.95x |
| wide_arrays.json | msgspec | 5.771 | 5.843 | 6.398 | 67.859 | 0.81x |
| wide_arrays.json | ujson | 7.346 | 7.428 | 7.944 | 67.859 | 0.64x |
| wide_arrays.json | json | 10.402 | 10.603 | 11.155 | 67.859 | 0.45x |
| mixed.json | strata | 0.209 | 0.215 | 0.225 | 67.859 | 1.00x |
| mixed.json | orjson | 0.165 | 0.170 | 0.178 | 67.859 | 1.26x |
| mixed.json | msgspec | 0.180 | 0.189 | 0.203 | 67.859 | 1.13x |
| mixed.json | ujson | 0.236 | 0.246 | 0.280 | 67.859 | 0.87x |
| mixed.json | json | 0.388 | 0.403 | 0.418 | 67.859 | 0.53x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.711 | 2.735 | 2.831 | 55.172 | 1.00x |
| users.json | orjson | 2.572 | 2.599 | 2.687 | 55.172 | 1.05x |
| users.json | msgspec | 3.374 | 3.402 | 3.522 | 55.172 | 0.80x |
| users.json | ujson | 14.111 | 14.171 | 14.504 | 55.172 | 0.19x |
| users.json | json | 19.650 | 19.775 | 20.001 | 55.172 | 0.14x |
| flat.json | strata | 0.302 | 0.305 | 0.328 | 67.859 | 1.00x |
| flat.json | orjson | 0.292 | 0.297 | 0.310 | 67.859 | 1.03x |
| flat.json | msgspec | 0.383 | 0.389 | 0.542 | 67.859 | 0.78x |
| flat.json | ujson | 1.133 | 1.138 | 1.169 | 67.859 | 0.27x |
| flat.json | json | 1.626 | 1.656 | 1.685 | 67.859 | 0.18x |
| nested.json | strata | 0.230 | 0.233 | 0.239 | 67.859 | 1.00x |
| nested.json | orjson | 0.259 | 0.261 | 0.264 | 67.859 | 0.89x |
| nested.json | msgspec | 0.349 | 0.357 | 0.391 | 67.859 | 0.65x |
| nested.json | ujson | 1.270 | 1.276 | 1.317 | 67.859 | 0.18x |
| nested.json | json | 2.067 | 2.094 | 2.107 | 67.859 | 0.11x |
| wide_arrays.json | strata | 1.868 | 1.884 | 1.992 | 67.859 | 1.00x |
| wide_arrays.json | orjson | 1.451 | 1.466 | 1.528 | 67.859 | 1.29x |
| wide_arrays.json | msgspec | 2.523 | 2.533 | 2.632 | 67.859 | 0.74x |
| wide_arrays.json | ujson | 7.890 | 7.955 | 8.233 | 67.859 | 0.24x |
| wide_arrays.json | json | 13.939 | 14.228 | 14.571 | 67.859 | 0.13x |
| mixed.json | strata | 0.063 | 0.068 | 0.074 | 67.859 | 1.00x |
| mixed.json | orjson | 0.055 | 0.059 | 0.063 | 67.859 | 1.14x |
| mixed.json | msgspec | 0.064 | 0.069 | 0.083 | 67.859 | 0.98x |
| mixed.json | ujson | 0.276 | 0.284 | 0.301 | 67.859 | 0.24x |
| mixed.json | json | 0.437 | 0.462 | 0.512 | 67.859 | 0.15x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.324 | 10.372 | 11.163 | 70.203 | 1.00x |
| users.json | orjson | 13.092 | 13.550 | 14.231 | 70.203 | 0.77x |
| users.json | msgspec | 13.151 | 13.389 | 14.053 | 70.203 | 0.77x |
| users.json | ujson | 16.880 | 17.301 | 18.101 | 70.203 | 0.60x |
| users.json | json | 21.461 | 21.550 | 22.179 | 70.203 | 0.48x |
| flat.json | strata | 1.066 | 1.099 | 1.217 | 67.859 | 1.00x |
| flat.json | orjson | 0.934 | 0.955 | 1.037 | 67.859 | 1.15x |
| flat.json | msgspec | 0.982 | 1.027 | 1.056 | 67.859 | 1.07x |
| flat.json | ujson | 1.458 | 1.491 | 1.612 | 67.859 | 0.74x |
| flat.json | json | 1.924 | 1.957 | 2.020 | 67.859 | 0.56x |
| nested.json | strata | 0.870 | 0.885 | 0.945 | 67.859 | 1.00x |
| nested.json | orjson | 1.768 | 1.806 | 1.854 | 67.859 | 0.49x |
| nested.json | msgspec | 1.776 | 1.817 | 2.051 | 67.859 | 0.49x |
| nested.json | ujson | 2.165 | 2.194 | 2.348 | 67.859 | 0.40x |
| nested.json | json | 2.716 | 2.774 | 3.038 | 67.859 | 0.32x |
| wide_arrays.json | strata | 4.876 | 4.934 | 5.267 | 67.859 | 1.00x |
| wide_arrays.json | orjson | 5.443 | 5.532 | 6.062 | 67.859 | 0.89x |
| wide_arrays.json | msgspec | 6.406 | 6.520 | 6.987 | 67.859 | 0.76x |
| wide_arrays.json | ujson | 8.118 | 8.314 | 9.085 | 67.859 | 0.59x |
| wide_arrays.json | json | 11.036 | 11.140 | 11.776 | 67.859 | 0.44x |
| mixed.json | strata | 0.254 | 0.271 | 0.326 | 67.859 | 1.00x |
| mixed.json | orjson | 0.219 | 0.231 | 0.257 | 67.859 | 1.17x |
| mixed.json | msgspec | 0.243 | 0.251 | 0.303 | 67.859 | 1.08x |
| mixed.json | ujson | 0.315 | 0.334 | 0.368 | 67.859 | 0.81x |
| mixed.json | json | 0.443 | 0.465 | 0.522 | 67.859 | 0.58x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.812 | 9.984 | 10.412 | 66.984 | 1.00x |
| users.ndjson | orjson | 13.501 | 13.669 | 14.831 | 66.984 | 0.73x |
| users.ndjson | msgspec | 13.157 | 13.386 | 14.049 | 66.984 | 0.75x |
| users.ndjson | ujson | 16.721 | 16.969 | 18.032 | 66.984 | 0.59x |
| users.ndjson | json | 23.173 | 24.220 | 24.828 | 66.984 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.175 | 3.198 | 3.311 | 67.250 | 1.00x |
| users.json | orjson | 2.994 | 3.044 | 3.157 | 67.250 | 1.05x |
| users.json | msgspec | 3.761 | 3.839 | 3.945 | 67.250 | 0.83x |
| users.json | ujson | 14.069 | 14.166 | 14.354 | 67.250 | 0.23x |
| users.json | json | 20.169 | 20.235 | 20.577 | 67.250 | 0.16x |
| flat.json | strata | 0.483 | 0.509 | 0.536 | 67.859 | 1.00x |
| flat.json | orjson | 0.452 | 0.475 | 0.533 | 67.859 | 1.07x |
| flat.json | msgspec | 0.531 | 0.551 | 0.644 | 67.859 | 0.92x |
| flat.json | ujson | 1.295 | 1.320 | 1.467 | 67.859 | 0.39x |
| flat.json | json | 1.779 | 1.827 | 1.936 | 67.859 | 0.28x |
| nested.json | strata | 0.394 | 0.428 | 0.456 | 67.859 | 1.00x |
| nested.json | orjson | 0.403 | 0.413 | 0.508 | 67.859 | 1.04x |
| nested.json | msgspec | 0.488 | 0.495 | 0.562 | 67.859 | 0.86x |
| nested.json | ujson | 1.498 | 1.523 | 1.563 | 67.859 | 0.28x |
| nested.json | json | 2.227 | 2.261 | 2.362 | 67.859 | 0.19x |
| wide_arrays.json | strata | 2.236 | 2.278 | 2.443 | 67.859 | 1.00x |
| wide_arrays.json | orjson | 1.776 | 1.837 | 1.921 | 67.859 | 1.24x |
| wide_arrays.json | msgspec | 2.826 | 2.887 | 3.060 | 67.859 | 0.79x |
| wide_arrays.json | ujson | 8.251 | 8.302 | 8.480 | 67.859 | 0.27x |
| wide_arrays.json | json | 14.388 | 14.438 | 14.890 | 67.859 | 0.16x |
| mixed.json | strata | 0.203 | 0.235 | 0.313 | 67.891 | 1.00x |
| mixed.json | orjson | 0.184 | 0.203 | 0.267 | 67.891 | 1.16x |
| mixed.json | msgspec | 0.180 | 0.201 | 0.344 | 67.891 | 1.17x |
| mixed.json | ujson | 0.416 | 0.438 | 0.453 | 67.891 | 0.54x |
| mixed.json | json | 0.573 | 0.585 | 0.694 | 67.891 | 0.40x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.064 | 0.074 | 0.091 | 67.609 | 1.00x |
| users.json $[*].id | jmespath | 0.428 | 0.432 | 0.491 | 67.609 | 0.17x |
| users.json $[*].id | jsonpath-ng | 4.196 | 4.227 | 4.500 | 67.609 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.418 | 0.438 | 0.456 | 68.047 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.660 | 2.738 | 3.181 | 68.047 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 14.586 | 14.710 | 16.300 | 68.047 | 0.03x |
| users.json $..total | strata | 1.771 | 1.799 | 1.843 | 69.172 | 1.00x |
| users.json $..total | jsonpath-ng | 270.967 | 273.133 | 274.578 | 69.172 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.917 | 5.036 | 5.161 | 67.734 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.625 | 13.947 | 14.986 | 67.734 | 0.36x |
| users.json $[*].id | orjson+jsonpath-ng | 15.194 | 15.443 | 16.367 | 67.734 | 0.33x |
| users.json $[*].orders[*].total | strata | 4.932 | 5.124 | 5.243 | 69.141 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.994 | 16.133 | 16.881 | 69.141 | 0.32x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 25.660 | 26.048 | 28.012 | 69.141 | 0.20x |
| users.json $..total | strata | 14.687 | 14.979 | 16.004 | 70.109 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 282.485 | 284.176 | 290.286 | 70.109 | 0.05x |

