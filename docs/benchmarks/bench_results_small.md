# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 9c9d784
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
| users.json | strata | 6.775 | 6.996 | 7.646 | 52.812 | 1.00x |
| users.json | orjson | 8.420 | 8.910 | 9.403 | 52.812 | 0.79x |
| users.json | msgspec | 8.450 | 8.905 | 9.728 | 52.812 | 0.79x |
| users.json | ujson | 11.285 | 12.012 | 12.709 | 52.812 | 0.58x |
| users.json | json | 14.863 | 15.630 | 16.332 | 52.812 | 0.45x |
| flat.json | strata | 0.640 | 0.658 | 0.703 | 71.938 | 1.00x |
| flat.json | orjson | 0.650 | 0.697 | 0.746 | 71.938 | 0.94x |
| flat.json | msgspec | 0.682 | 0.724 | 0.775 | 71.938 | 0.91x |
| flat.json | ujson | 1.012 | 1.053 | 1.100 | 71.938 | 0.62x |
| flat.json | json | 1.424 | 1.439 | 1.530 | 71.938 | 0.46x |
| nested.json | strata | 0.556 | 0.577 | 0.623 | 71.953 | 1.00x |
| nested.json | orjson | 1.210 | 1.267 | 1.547 | 71.953 | 0.46x |
| nested.json | msgspec | 1.214 | 1.268 | 1.547 | 71.953 | 0.46x |
| nested.json | ujson | 1.487 | 1.590 | 1.826 | 71.953 | 0.36x |
| nested.json | json | 1.970 | 2.034 | 2.301 | 71.953 | 0.28x |
| wide_arrays.json | strata | 3.283 | 3.493 | 3.821 | 71.953 | 1.00x |
| wide_arrays.json | orjson | 3.887 | 4.068 | 4.217 | 71.953 | 0.86x |
| wide_arrays.json | msgspec | 4.581 | 4.802 | 5.257 | 71.953 | 0.73x |
| wide_arrays.json | ujson | 5.724 | 5.978 | 6.284 | 71.953 | 0.58x |
| wide_arrays.json | json | 8.115 | 8.528 | 9.380 | 71.953 | 0.41x |
| mixed.json | strata | 0.130 | 0.135 | 0.165 | 73.188 | 1.00x |
| mixed.json | orjson | 0.126 | 0.132 | 0.178 | 73.188 | 1.03x |
| mixed.json | msgspec | 0.140 | 0.144 | 0.166 | 73.188 | 0.94x |
| mixed.json | ujson | 0.182 | 0.187 | 0.216 | 73.188 | 0.72x |
| mixed.json | json | 0.298 | 0.312 | 0.368 | 73.188 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.530 | 1.602 | 1.676 | 56.750 | 1.00x |
| users.json | orjson | 1.980 | 2.059 | 2.191 | 56.750 | 0.78x |
| users.json | msgspec | 2.600 | 2.732 | 2.850 | 56.750 | 0.59x |
| users.json | ujson | 10.671 | 10.885 | 11.293 | 56.750 | 0.15x |
| users.json | json | 15.339 | 15.634 | 16.006 | 56.750 | 0.10x |
| flat.json | strata | 0.181 | 0.187 | 0.197 | 71.953 | 1.00x |
| flat.json | orjson | 0.221 | 0.231 | 0.251 | 71.953 | 0.81x |
| flat.json | msgspec | 0.291 | 0.301 | 0.321 | 71.953 | 0.62x |
| flat.json | ujson | 0.867 | 0.900 | 0.943 | 71.953 | 0.21x |
| flat.json | json | 1.270 | 1.308 | 1.397 | 71.953 | 0.14x |
| nested.json | strata | 0.129 | 0.134 | 0.157 | 71.953 | 1.00x |
| nested.json | orjson | 0.199 | 0.205 | 0.230 | 71.953 | 0.66x |
| nested.json | msgspec | 0.271 | 0.281 | 0.308 | 71.953 | 0.48x |
| nested.json | ujson | 0.975 | 1.013 | 1.125 | 71.953 | 0.13x |
| nested.json | json | 1.639 | 1.663 | 1.752 | 71.953 | 0.08x |
| wide_arrays.json | strata | 1.087 | 1.112 | 1.255 | 73.047 | 1.00x |
| wide_arrays.json | orjson | 1.093 | 1.163 | 1.233 | 73.047 | 0.96x |
| wide_arrays.json | msgspec | 1.911 | 1.941 | 2.194 | 73.047 | 0.57x |
| wide_arrays.json | ujson | 5.980 | 6.131 | 6.499 | 73.047 | 0.18x |
| wide_arrays.json | json | 10.765 | 10.857 | 11.171 | 73.047 | 0.10x |
| mixed.json | strata | 0.037 | 0.040 | 0.052 | 73.188 | 1.00x |
| mixed.json | orjson | 0.042 | 0.043 | 0.048 | 73.188 | 0.93x |
| mixed.json | msgspec | 0.048 | 0.051 | 0.054 | 73.188 | 0.79x |
| mixed.json | ujson | 0.212 | 0.217 | 0.226 | 73.188 | 0.19x |
| mixed.json | json | 0.338 | 0.349 | 0.379 | 73.188 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.287 | 7.491 | 7.810 | 70.391 | 1.00x |
| users.json | orjson | 11.037 | 11.975 | 12.542 | 70.391 | 0.63x |
| users.json | msgspec | 10.683 | 11.726 | 12.593 | 70.391 | 0.64x |
| users.json | ujson | 14.402 | 15.035 | 17.300 | 70.391 | 0.50x |
| users.json | json | 17.099 | 18.395 | 20.038 | 70.391 | 0.41x |
| flat.json | strata | 0.703 | 0.732 | 0.885 | 71.953 | 1.00x |
| flat.json | orjson | 0.711 | 0.746 | 0.858 | 71.953 | 0.98x |
| flat.json | msgspec | 0.773 | 0.803 | 0.896 | 71.953 | 0.91x |
| flat.json | ujson | 1.133 | 1.172 | 1.299 | 71.953 | 0.62x |
| flat.json | json | 1.489 | 1.534 | 1.623 | 71.953 | 0.48x |
| nested.json | strata | 0.618 | 0.647 | 0.754 | 71.953 | 1.00x |
| nested.json | orjson | 1.459 | 1.489 | 1.764 | 71.953 | 0.43x |
| nested.json | msgspec | 1.433 | 1.516 | 1.736 | 71.953 | 0.43x |
| nested.json | ujson | 1.719 | 1.830 | 2.179 | 71.953 | 0.35x |
| nested.json | json | 2.150 | 2.240 | 2.581 | 71.953 | 0.29x |
| wide_arrays.json | strata | 3.442 | 3.635 | 3.838 | 73.188 | 1.00x |
| wide_arrays.json | orjson | 4.344 | 4.477 | 4.599 | 73.188 | 0.81x |
| wide_arrays.json | msgspec | 5.041 | 5.293 | 5.772 | 73.188 | 0.69x |
| wide_arrays.json | ujson | 6.386 | 6.716 | 7.501 | 73.188 | 0.54x |
| wide_arrays.json | json | 8.639 | 9.017 | 9.478 | 73.188 | 0.40x |
| mixed.json | strata | 0.174 | 0.201 | 0.237 | 73.188 | 1.00x |
| mixed.json | orjson | 0.176 | 0.202 | 0.242 | 73.188 | 1.00x |
| mixed.json | msgspec | 0.184 | 0.199 | 0.245 | 73.188 | 1.01x |
| mixed.json | ujson | 0.246 | 0.274 | 0.366 | 73.188 | 0.73x |
| mixed.json | json | 0.346 | 0.374 | 0.390 | 73.188 | 0.54x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.328 | 7.587 | 7.863 | 71.078 | 1.00x |
| users.ndjson | orjson | 10.743 | 11.517 | 11.863 | 71.078 | 0.66x |
| users.ndjson | msgspec | 10.466 | 11.093 | 11.701 | 71.078 | 0.68x |
| users.ndjson | ujson | 13.160 | 14.014 | 14.479 | 71.078 | 0.54x |
| users.ndjson | json | 18.269 | 19.079 | 20.117 | 71.078 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.927 | 2.003 | 2.187 | 66.516 | 1.00x |
| users.json | orjson | 2.367 | 2.462 | 2.611 | 66.516 | 0.81x |
| users.json | msgspec | 2.954 | 3.096 | 3.293 | 66.516 | 0.65x |
| users.json | ujson | 10.981 | 11.186 | 11.482 | 66.516 | 0.18x |
| users.json | json | 15.697 | 15.935 | 16.409 | 66.516 | 0.13x |
| flat.json | strata | 0.295 | 0.333 | 0.366 | 71.953 | 1.00x |
| flat.json | orjson | 0.341 | 0.373 | 0.436 | 71.953 | 0.89x |
| flat.json | msgspec | 0.416 | 0.444 | 0.503 | 71.953 | 0.75x |
| flat.json | ujson | 0.995 | 1.037 | 1.196 | 71.953 | 0.32x |
| flat.json | json | 1.406 | 1.438 | 1.537 | 71.953 | 0.23x |
| nested.json | strata | 0.247 | 0.272 | 0.342 | 71.953 | 1.00x |
| nested.json | orjson | 0.326 | 0.358 | 0.452 | 71.953 | 0.76x |
| nested.json | msgspec | 0.400 | 0.422 | 0.482 | 71.953 | 0.64x |
| nested.json | ujson | 1.144 | 1.176 | 1.245 | 71.953 | 0.23x |
| nested.json | json | 1.740 | 1.787 | 1.987 | 71.953 | 0.15x |
| wide_arrays.json | strata | 1.409 | 1.465 | 1.675 | 73.188 | 1.00x |
| wide_arrays.json | orjson | 1.451 | 1.509 | 1.661 | 73.188 | 0.97x |
| wide_arrays.json | msgspec | 2.277 | 2.305 | 2.466 | 73.188 | 0.64x |
| wide_arrays.json | ujson | 6.336 | 6.522 | 6.648 | 73.188 | 0.22x |
| wide_arrays.json | json | 11.127 | 11.409 | 11.612 | 73.188 | 0.13x |
| mixed.json | strata | 0.136 | 0.178 | 0.230 | 73.219 | 1.00x |
| mixed.json | orjson | 0.149 | 0.187 | 0.252 | 73.219 | 0.95x |
| mixed.json | msgspec | 0.161 | 0.178 | 0.291 | 73.219 | 1.00x |
| mixed.json | ujson | 0.320 | 0.354 | 0.387 | 73.219 | 0.50x |
| mixed.json | json | 0.457 | 0.481 | 0.553 | 73.219 | 0.37x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.058 | 0.063 | 0.072 | 66.688 | 1.00x |
| users.json $[*].id | jmespath | 0.332 | 0.348 | 0.387 | 66.688 | 0.18x |
| users.json $[*].id | jsonpath-ng | 3.368 | 3.418 | 3.780 | 66.688 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.339 | 0.348 | 0.538 | 67.047 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.018 | 2.034 | 2.478 | 67.047 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 11.202 | 11.281 | 12.066 | 67.047 | 0.03x |
| users.json $..total | strata | 1.362 | 1.382 | 1.503 | 70.172 | 1.00x |
| users.json $..total | jsonpath-ng | 208.952 | 213.389 | 216.481 | 70.172 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.233 | 3.263 | 3.372 | 66.812 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.805 | 11.058 | 11.235 | 66.812 | 0.30x |
| users.json $[*].id | orjson+jsonpath-ng | 12.045 | 12.132 | 12.557 | 66.812 | 0.27x |
| users.json $[*].orders[*].total | strata | 3.346 | 3.415 | 3.495 | 70.031 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.667 | 13.132 | 14.250 | 70.031 | 0.26x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 19.847 | 20.410 | 21.208 | 70.031 | 0.17x |
| users.json $..total | strata | 10.799 | 11.345 | 12.031 | 70.188 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 219.396 | 221.475 | 224.376 | 70.188 | 0.05x |

