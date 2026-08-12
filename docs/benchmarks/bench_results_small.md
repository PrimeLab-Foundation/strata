# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: e0e780e
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
| users.json | strata | 7.040 | 7.176 | 7.546 | 52.531 | 1.00x |
| users.json | orjson | 8.282 | 8.458 | 9.422 | 52.531 | 0.85x |
| users.json | msgspec | 8.333 | 8.589 | 9.396 | 52.531 | 0.84x |
| users.json | ujson | 11.179 | 11.492 | 12.175 | 52.531 | 0.62x |
| users.json | json | 14.557 | 14.827 | 15.575 | 52.531 | 0.48x |
| flat.json | strata | 0.637 | 0.645 | 0.716 | 73.703 | 1.00x |
| flat.json | orjson | 0.624 | 0.627 | 0.641 | 73.703 | 1.03x |
| flat.json | msgspec | 0.663 | 0.670 | 0.699 | 73.703 | 0.96x |
| flat.json | ujson | 0.958 | 0.965 | 0.989 | 73.703 | 0.67x |
| flat.json | json | 1.340 | 1.352 | 1.442 | 73.703 | 0.48x |
| nested.json | strata | 0.555 | 0.564 | 0.609 | 73.734 | 1.00x |
| nested.json | orjson | 1.203 | 1.226 | 1.563 | 73.734 | 0.46x |
| nested.json | msgspec | 1.205 | 1.251 | 1.431 | 73.734 | 0.45x |
| nested.json | ujson | 1.476 | 1.556 | 1.992 | 73.734 | 0.36x |
| nested.json | json | 1.917 | 1.967 | 2.309 | 73.734 | 0.29x |
| wide_arrays.json | strata | 3.205 | 3.228 | 3.467 | 73.734 | 1.00x |
| wide_arrays.json | orjson | 3.783 | 3.825 | 4.077 | 73.734 | 0.84x |
| wide_arrays.json | msgspec | 4.461 | 4.519 | 4.634 | 73.734 | 0.71x |
| wide_arrays.json | ujson | 5.670 | 5.809 | 6.176 | 73.734 | 0.56x |
| wide_arrays.json | json | 7.960 | 8.128 | 8.647 | 73.734 | 0.40x |
| mixed.json | strata | 0.136 | 0.144 | 0.155 | 73.891 | 1.00x |
| mixed.json | orjson | 0.124 | 0.131 | 0.158 | 73.891 | 1.10x |
| mixed.json | msgspec | 0.135 | 0.143 | 0.164 | 73.891 | 1.01x |
| mixed.json | ujson | 0.179 | 0.182 | 0.195 | 73.891 | 0.79x |
| mixed.json | json | 0.294 | 0.304 | 0.335 | 73.891 | 0.47x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.919 | 1.954 | 2.037 | 55.625 | 1.00x |
| users.json | orjson | 1.958 | 1.979 | 2.075 | 55.625 | 0.99x |
| users.json | msgspec | 2.596 | 2.611 | 2.752 | 55.625 | 0.75x |
| users.json | ujson | 10.512 | 10.604 | 10.883 | 55.625 | 0.18x |
| users.json | json | 14.926 | 14.999 | 15.220 | 55.625 | 0.13x |
| flat.json | strata | 0.212 | 0.215 | 0.228 | 73.719 | 1.00x |
| flat.json | orjson | 0.221 | 0.224 | 0.250 | 73.719 | 0.96x |
| flat.json | msgspec | 0.291 | 0.297 | 0.305 | 73.719 | 0.73x |
| flat.json | ujson | 0.861 | 0.872 | 0.917 | 73.719 | 0.25x |
| flat.json | json | 1.237 | 1.248 | 1.402 | 73.719 | 0.17x |
| nested.json | strata | 0.153 | 0.160 | 0.202 | 73.734 | 1.00x |
| nested.json | orjson | 0.197 | 0.200 | 0.204 | 73.734 | 0.80x |
| nested.json | msgspec | 0.266 | 0.271 | 0.283 | 73.734 | 0.59x |
| nested.json | ujson | 0.971 | 0.974 | 1.003 | 73.734 | 0.16x |
| nested.json | json | 1.567 | 1.588 | 1.658 | 73.734 | 0.10x |
| wide_arrays.json | strata | 1.108 | 1.143 | 1.192 | 73.875 | 1.00x |
| wide_arrays.json | orjson | 1.101 | 1.110 | 1.242 | 73.875 | 1.03x |
| wide_arrays.json | msgspec | 1.906 | 1.938 | 1.988 | 73.875 | 0.59x |
| wide_arrays.json | ujson | 5.968 | 6.035 | 6.490 | 73.875 | 0.19x |
| wide_arrays.json | json | 10.605 | 10.765 | 11.020 | 73.875 | 0.11x |
| mixed.json | strata | 0.043 | 0.046 | 0.055 | 73.891 | 1.00x |
| mixed.json | orjson | 0.042 | 0.043 | 0.047 | 73.891 | 1.08x |
| mixed.json | msgspec | 0.048 | 0.050 | 0.057 | 73.891 | 0.92x |
| mixed.json | ujson | 0.210 | 0.215 | 0.224 | 73.891 | 0.22x |
| mixed.json | json | 0.331 | 0.337 | 0.353 | 73.891 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.248 | 7.408 | 7.708 | 72.219 | 1.00x |
| users.json | orjson | 10.306 | 10.807 | 11.392 | 72.219 | 0.69x |
| users.json | msgspec | 10.576 | 10.702 | 11.240 | 72.219 | 0.69x |
| users.json | ujson | 13.370 | 13.752 | 15.217 | 72.219 | 0.54x |
| users.json | json | 16.706 | 17.125 | 18.349 | 72.219 | 0.43x |
| flat.json | strata | 0.700 | 0.720 | 0.782 | 73.719 | 1.00x |
| flat.json | orjson | 0.695 | 0.713 | 0.866 | 73.719 | 1.01x |
| flat.json | msgspec | 0.733 | 0.752 | 0.880 | 73.719 | 0.96x |
| flat.json | ujson | 1.061 | 1.100 | 1.172 | 73.719 | 0.66x |
| flat.json | json | 1.413 | 1.430 | 1.529 | 73.719 | 0.50x |
| nested.json | strata | 0.618 | 0.630 | 0.646 | 73.734 | 1.00x |
| nested.json | orjson | 1.372 | 1.417 | 1.840 | 73.734 | 0.45x |
| nested.json | msgspec | 1.369 | 1.407 | 1.495 | 73.734 | 0.45x |
| nested.json | ujson | 1.669 | 1.736 | 1.891 | 73.734 | 0.36x |
| nested.json | json | 2.082 | 2.116 | 2.478 | 73.734 | 0.30x |
| wide_arrays.json | strata | 3.420 | 3.458 | 3.714 | 73.891 | 1.00x |
| wide_arrays.json | orjson | 4.271 | 4.319 | 4.446 | 73.891 | 0.80x |
| wide_arrays.json | msgspec | 5.043 | 5.112 | 5.486 | 73.891 | 0.68x |
| wide_arrays.json | ujson | 6.354 | 6.423 | 6.581 | 73.891 | 0.54x |
| wide_arrays.json | json | 8.536 | 8.619 | 9.215 | 73.891 | 0.40x |
| mixed.json | strata | 0.177 | 0.183 | 0.211 | 73.891 | 1.00x |
| mixed.json | orjson | 0.170 | 0.185 | 0.212 | 73.891 | 0.99x |
| mixed.json | msgspec | 0.184 | 0.191 | 0.214 | 73.891 | 0.96x |
| mixed.json | ujson | 0.245 | 0.259 | 0.340 | 73.891 | 0.71x |
| mixed.json | json | 0.343 | 0.363 | 0.413 | 73.891 | 0.50x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.436 | 7.548 | 7.893 | 72.828 | 1.00x |
| users.ndjson | orjson | 10.267 | 10.585 | 11.622 | 72.828 | 0.71x |
| users.ndjson | msgspec | 10.136 | 10.329 | 11.498 | 72.828 | 0.73x |
| users.ndjson | ujson | 12.960 | 13.125 | 14.152 | 72.828 | 0.58x |
| users.ndjson | json | 17.736 | 18.047 | 19.069 | 72.828 | 0.42x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.298 | 2.353 | 2.429 | 70.641 | 1.00x |
| users.json | orjson | 2.297 | 2.362 | 2.428 | 70.641 | 1.00x |
| users.json | msgspec | 2.958 | 2.996 | 3.250 | 70.641 | 0.79x |
| users.json | ujson | 10.799 | 10.912 | 11.501 | 70.641 | 0.22x |
| users.json | json | 15.104 | 15.368 | 16.229 | 70.641 | 0.15x |
| flat.json | strata | 0.344 | 0.360 | 0.403 | 73.719 | 1.00x |
| flat.json | orjson | 0.363 | 0.377 | 0.454 | 73.719 | 0.95x |
| flat.json | msgspec | 0.417 | 0.434 | 0.492 | 73.719 | 0.83x |
| flat.json | ujson | 1.030 | 1.052 | 1.150 | 73.719 | 0.34x |
| flat.json | json | 1.381 | 1.402 | 1.530 | 73.719 | 0.26x |
| nested.json | strata | 0.275 | 0.303 | 0.349 | 73.734 | 1.00x |
| nested.json | orjson | 0.323 | 0.345 | 0.456 | 73.734 | 0.88x |
| nested.json | msgspec | 0.398 | 0.421 | 0.456 | 73.734 | 0.72x |
| nested.json | ujson | 1.138 | 1.162 | 1.352 | 73.734 | 0.26x |
| nested.json | json | 1.704 | 1.748 | 1.952 | 73.734 | 0.17x |
| wide_arrays.json | strata | 1.367 | 1.403 | 1.546 | 73.891 | 1.00x |
| wide_arrays.json | orjson | 1.371 | 1.416 | 1.525 | 73.891 | 0.99x |
| wide_arrays.json | msgspec | 2.187 | 2.226 | 2.323 | 73.891 | 0.63x |
| wide_arrays.json | ujson | 6.356 | 6.405 | 6.674 | 73.891 | 0.22x |
| wide_arrays.json | json | 10.932 | 11.096 | 11.445 | 73.891 | 0.13x |
| mixed.json | strata | 0.141 | 0.161 | 0.246 | 73.922 | 1.00x |
| mixed.json | orjson | 0.141 | 0.156 | 0.223 | 73.922 | 1.04x |
| mixed.json | msgspec | 0.145 | 0.168 | 0.255 | 73.922 | 0.96x |
| mixed.json | ujson | 0.322 | 0.338 | 0.375 | 73.922 | 0.48x |
| mixed.json | json | 0.439 | 0.478 | 0.527 | 73.922 | 0.34x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.051 | 0.057 | 0.064 | 71.234 | 1.00x |
| users.json $[*].id | jmespath | 0.327 | 0.331 | 0.340 | 71.234 | 0.17x |
| users.json $[*].id | jsonpath-ng | 3.333 | 3.377 | 3.566 | 71.234 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.337 | 0.349 | 0.363 | 71.875 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.020 | 2.045 | 2.072 | 71.875 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 11.132 | 11.235 | 11.520 | 71.875 | 0.03x |
| users.json $..total | strata | 1.355 | 1.372 | 1.415 | 73.016 | 1.00x |
| users.json $..total | jsonpath-ng | 208.436 | 209.148 | 209.769 | 73.016 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.200 | 3.254 | 3.345 | 71.312 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.751 | 10.898 | 11.330 | 71.312 | 0.30x |
| users.json $[*].id | orjson+jsonpath-ng | 11.940 | 12.095 | 12.681 | 71.312 | 0.27x |
| users.json $[*].orders[*].total | strata | 3.293 | 3.375 | 3.499 | 72.969 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.489 | 12.645 | 13.177 | 72.969 | 0.27x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 19.953 | 20.232 | 20.642 | 72.969 | 0.17x |
| users.json $..total | strata | 10.817 | 11.044 | 11.543 | 73.969 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 215.068 | 217.328 | 221.662 | 73.969 | 0.05x |

