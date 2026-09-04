# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: af84ee6
- python: 3.14.7
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit-Mach-O
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
| users.json | strata | 6.309 | 6.357 | 6.800 | 53.141 | 1.00x |
| users.json | orjson | 8.140 | 8.218 | 8.504 | 53.141 | 0.77x |
| users.json | msgspec | 7.942 | 8.039 | 8.475 | 53.141 | 0.79x |
| users.json | ujson | 10.795 | 10.984 | 11.623 | 53.141 | 0.58x |
| users.json | json | 15.166 | 15.251 | 15.454 | 53.141 | 0.42x |
| flat.json | strata | 0.616 | 0.623 | 0.645 | 69.875 | 1.00x |
| flat.json | orjson | 0.669 | 0.676 | 0.698 | 69.875 | 0.92x |
| flat.json | msgspec | 0.693 | 0.697 | 0.708 | 69.875 | 0.89x |
| flat.json | ujson | 1.042 | 1.047 | 1.076 | 69.875 | 0.59x |
| flat.json | json | 1.458 | 1.465 | 1.503 | 69.875 | 0.43x |
| nested.json | strata | 0.540 | 0.542 | 0.546 | 69.922 | 1.00x |
| nested.json | orjson | 0.625 | 0.627 | 0.632 | 69.922 | 0.87x |
| nested.json | msgspec | 0.611 | 0.617 | 0.635 | 69.922 | 0.88x |
| nested.json | ujson | 0.883 | 0.887 | 0.894 | 69.922 | 0.61x |
| nested.json | json | 1.421 | 1.428 | 1.444 | 69.922 | 0.38x |
| wide_arrays.json | strata | 2.991 | 3.020 | 3.107 | 69.984 | 1.00x |
| wide_arrays.json | orjson | 3.278 | 3.324 | 3.382 | 69.984 | 0.91x |
| wide_arrays.json | msgspec | 3.929 | 3.973 | 4.083 | 69.984 | 0.76x |
| wide_arrays.json | ujson | 5.126 | 5.178 | 5.306 | 69.984 | 0.58x |
| wide_arrays.json | json | 7.505 | 7.581 | 7.629 | 69.984 | 0.40x |
| mixed.json | strata | 0.127 | 0.132 | 0.156 | 69.984 | 1.00x |
| mixed.json | orjson | 0.134 | 0.139 | 0.147 | 69.984 | 0.95x |
| mixed.json | msgspec | 0.142 | 0.145 | 0.168 | 69.984 | 0.91x |
| mixed.json | ujson | 0.185 | 0.193 | 0.195 | 69.984 | 0.69x |
| mixed.json | json | 0.311 | 0.319 | 0.340 | 69.984 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.425 | 1.435 | 1.485 | 55.672 | 1.00x |
| users.json | orjson | 1.999 | 2.016 | 2.140 | 55.672 | 0.71x |
| users.json | msgspec | 2.619 | 2.639 | 2.794 | 55.672 | 0.54x |
| users.json | ujson | 10.779 | 10.823 | 11.239 | 55.672 | 0.13x |
| users.json | json | 15.458 | 15.648 | 16.312 | 55.672 | 0.09x |
| flat.json | strata | 0.177 | 0.179 | 0.184 | 69.906 | 1.00x |
| flat.json | orjson | 0.222 | 0.225 | 0.230 | 69.906 | 0.80x |
| flat.json | msgspec | 0.293 | 0.295 | 0.299 | 69.906 | 0.61x |
| flat.json | ujson | 0.885 | 0.892 | 0.899 | 69.906 | 0.20x |
| flat.json | json | 1.319 | 1.329 | 1.386 | 69.906 | 0.13x |
| nested.json | strata | 0.134 | 0.136 | 0.142 | 69.984 | 1.00x |
| nested.json | orjson | 0.197 | 0.200 | 0.204 | 69.984 | 0.68x |
| nested.json | msgspec | 0.268 | 0.271 | 0.286 | 69.984 | 0.50x |
| nested.json | ujson | 0.953 | 0.958 | 1.068 | 69.984 | 0.14x |
| nested.json | json | 1.603 | 1.639 | 1.686 | 69.984 | 0.08x |
| wide_arrays.json | strata | 0.912 | 0.917 | 0.950 | 69.984 | 1.00x |
| wide_arrays.json | orjson | 1.105 | 1.109 | 1.291 | 69.984 | 0.83x |
| wide_arrays.json | msgspec | 1.930 | 1.936 | 2.049 | 69.984 | 0.47x |
| wide_arrays.json | ujson | 5.976 | 6.014 | 6.117 | 69.984 | 0.15x |
| wide_arrays.json | json | 11.025 | 11.152 | 12.172 | 69.984 | 0.08x |
| mixed.json | strata | 0.036 | 0.037 | 0.041 | 70.078 | 1.00x |
| mixed.json | orjson | 0.044 | 0.047 | 0.052 | 70.078 | 0.79x |
| mixed.json | msgspec | 0.049 | 0.050 | 0.059 | 70.078 | 0.74x |
| mixed.json | ujson | 0.217 | 0.222 | 0.232 | 70.078 | 0.17x |
| mixed.json | json | 0.358 | 0.365 | 0.381 | 70.078 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.572 | 6.676 | 6.991 | 67.281 | 1.00x |
| users.json | orjson | 8.490 | 8.549 | 9.367 | 67.281 | 0.78x |
| users.json | msgspec | 8.230 | 8.340 | 8.472 | 67.281 | 0.80x |
| users.json | ujson | 11.262 | 11.482 | 11.595 | 67.281 | 0.58x |
| users.json | json | 15.524 | 15.638 | 16.021 | 67.281 | 0.43x |
| flat.json | strata | 0.677 | 0.689 | 0.726 | 69.906 | 1.00x |
| flat.json | orjson | 0.740 | 0.750 | 0.836 | 69.906 | 0.92x |
| flat.json | msgspec | 0.761 | 0.770 | 0.850 | 69.906 | 0.90x |
| flat.json | ujson | 1.136 | 1.149 | 1.300 | 69.906 | 0.60x |
| flat.json | json | 1.527 | 1.536 | 1.627 | 69.906 | 0.45x |
| nested.json | strata | 0.605 | 0.610 | 0.662 | 69.984 | 1.00x |
| nested.json | orjson | 0.688 | 0.699 | 0.709 | 69.984 | 0.87x |
| nested.json | msgspec | 0.676 | 0.686 | 0.703 | 69.984 | 0.89x |
| nested.json | ujson | 0.970 | 0.979 | 0.988 | 69.984 | 0.62x |
| nested.json | json | 1.495 | 1.501 | 1.508 | 69.984 | 0.41x |
| wide_arrays.json | strata | 3.172 | 3.186 | 3.399 | 69.984 | 1.00x |
| wide_arrays.json | orjson | 3.510 | 3.531 | 3.686 | 69.984 | 0.90x |
| wide_arrays.json | msgspec | 4.174 | 4.216 | 4.261 | 69.984 | 0.76x |
| wide_arrays.json | ujson | 5.451 | 5.502 | 5.573 | 69.984 | 0.58x |
| wide_arrays.json | json | 7.701 | 7.779 | 8.194 | 69.984 | 0.41x |
| mixed.json | strata | 0.165 | 0.170 | 0.189 | 70.078 | 1.00x |
| mixed.json | orjson | 0.179 | 0.183 | 0.209 | 70.078 | 0.93x |
| mixed.json | msgspec | 0.183 | 0.193 | 0.240 | 70.078 | 0.88x |
| mixed.json | ujson | 0.237 | 0.243 | 0.283 | 70.078 | 0.70x |
| mixed.json | json | 0.350 | 0.354 | 0.359 | 70.078 | 0.48x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.714 | 6.775 | 7.195 | 69.016 | 1.00x |
| users.ndjson | orjson | 10.192 | 10.297 | 10.541 | 69.016 | 0.66x |
| users.ndjson | msgspec | 9.831 | 9.894 | 10.349 | 69.016 | 0.68x |
| users.ndjson | ujson | 12.755 | 12.794 | 13.338 | 69.016 | 0.53x |
| users.ndjson | json | 17.819 | 18.047 | 18.407 | 69.016 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.795 | 1.851 | 2.120 | 67.750 | 1.00x |
| users.json | orjson | 2.377 | 2.445 | 2.559 | 67.750 | 0.76x |
| users.json | msgspec | 3.025 | 3.076 | 3.196 | 67.750 | 0.60x |
| users.json | ujson | 10.913 | 11.024 | 11.230 | 67.750 | 0.17x |
| users.json | json | 15.949 | 16.164 | 16.323 | 67.750 | 0.11x |
| flat.json | strata | 0.303 | 0.315 | 0.334 | 69.906 | 1.00x |
| flat.json | orjson | 0.357 | 0.364 | 0.386 | 69.906 | 0.87x |
| flat.json | msgspec | 0.423 | 0.434 | 0.446 | 69.906 | 0.73x |
| flat.json | ujson | 1.055 | 1.062 | 1.117 | 69.906 | 0.30x |
| flat.json | json | 1.474 | 1.486 | 1.509 | 69.906 | 0.21x |
| nested.json | strata | 0.270 | 0.280 | 0.298 | 69.984 | 1.00x |
| nested.json | orjson | 0.328 | 0.341 | 0.355 | 69.984 | 0.82x |
| nested.json | msgspec | 0.396 | 0.411 | 0.476 | 69.984 | 0.68x |
| nested.json | ujson | 1.105 | 1.124 | 1.254 | 69.984 | 0.25x |
| nested.json | json | 1.752 | 1.786 | 1.818 | 69.984 | 0.16x |
| wide_arrays.json | strata | 1.196 | 1.237 | 1.297 | 69.984 | 1.00x |
| wide_arrays.json | orjson | 1.399 | 1.432 | 1.533 | 69.984 | 0.86x |
| wide_arrays.json | msgspec | 2.217 | 2.252 | 2.366 | 69.984 | 0.55x |
| wide_arrays.json | ujson | 6.340 | 6.400 | 6.476 | 69.984 | 0.19x |
| wide_arrays.json | json | 11.340 | 11.405 | 11.706 | 69.984 | 0.11x |
| mixed.json | strata | 0.140 | 0.156 | 0.168 | 70.109 | 1.00x |
| mixed.json | orjson | 0.154 | 0.178 | 0.229 | 70.109 | 0.88x |
| mixed.json | msgspec | 0.155 | 0.170 | 0.252 | 70.109 | 0.92x |
| mixed.json | ujson | 0.338 | 0.358 | 0.386 | 70.109 | 0.44x |
| mixed.json | json | 0.478 | 0.493 | 0.522 | 70.109 | 0.32x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.049 | 0.053 | 0.065 | 67.922 | 1.00x |
| users.json $[*].id | jmespath | 0.271 | 0.276 | 0.296 | 67.922 | 0.19x |
| users.json $[*].id | jsonpath-ng | 1.423 | 1.434 | 1.519 | 67.922 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.321 | 0.333 | 0.514 | 70.000 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.636 | 1.710 | 2.005 | 70.000 | 0.19x |
| users.json $[*].orders[*].total | jsonpath-ng | 9.443 | 9.958 | 11.422 | 70.000 | 0.03x |
| users.json $..total | strata | 1.383 | 1.392 | 1.417 | 69.156 | 1.00x |
| users.json $..total | jsonpath-ng | 184.082 | 185.119 | 187.112 | 69.156 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.381 | 3.406 | 3.636 | 69.625 | 1.00x |
| users.json $[*].id | orjson+jmespath | 8.732 | 8.797 | 9.017 | 69.625 | 0.39x |
| users.json $[*].id | orjson+jsonpath-ng | 9.894 | 9.924 | 10.072 | 69.625 | 0.34x |
| users.json $[*].orders[*].total | strata | 3.448 | 3.502 | 3.563 | 69.109 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 10.190 | 10.309 | 11.048 | 69.109 | 0.34x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 19.431 | 19.506 | 19.737 | 69.109 | 0.18x |
| users.json $..total | strata | 8.097 | 8.146 | 8.942 | 70.109 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 194.785 | 195.438 | 197.311 | 70.109 | 0.04x |

