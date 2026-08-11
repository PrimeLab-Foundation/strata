# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 67d6d68
- python: 3.14.3
- implementation: CPython
- platform: macOS-26.3-arm64-arm-64bit-Mach-O
- machine: arm64
- processor: Apple M1 Max
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 30
- warmup: 3

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.905 | 7.019 | 7.713 | 52.797 | 1.00x |
| users.json | orjson | 8.250 | 8.411 | 9.028 | 52.797 | 0.83x |
| users.json | msgspec | 8.336 | 8.446 | 9.539 | 52.797 | 0.83x |
| users.json | ujson | 11.159 | 11.537 | 12.693 | 52.797 | 0.61x |
| users.json | json | 14.569 | 14.885 | 15.546 | 52.797 | 0.47x |
| flat.json | strata | 0.669 | 0.675 | 0.687 | 67.656 | 1.00x |
| flat.json | orjson | 0.649 | 0.653 | 0.672 | 67.656 | 1.03x |
| flat.json | msgspec | 0.684 | 0.689 | 0.718 | 67.656 | 0.98x |
| flat.json | ujson | 1.012 | 1.022 | 1.089 | 67.656 | 0.66x |
| flat.json | json | 1.404 | 1.416 | 1.477 | 67.656 | 0.48x |
| nested.json | strata | 0.558 | 0.566 | 0.606 | 67.672 | 1.00x |
| nested.json | orjson | 1.194 | 1.204 | 1.297 | 67.672 | 0.47x |
| nested.json | msgspec | 1.195 | 1.208 | 1.355 | 67.672 | 0.47x |
| nested.json | ujson | 1.461 | 1.479 | 1.538 | 67.672 | 0.38x |
| nested.json | json | 1.907 | 1.921 | 2.211 | 67.672 | 0.29x |
| wide_arrays.json | strata | 3.191 | 3.204 | 3.517 | 67.688 | 1.00x |
| wide_arrays.json | orjson | 3.775 | 3.803 | 4.101 | 67.688 | 0.84x |
| wide_arrays.json | msgspec | 4.448 | 4.484 | 5.130 | 67.688 | 0.71x |
| wide_arrays.json | ujson | 5.636 | 5.702 | 5.952 | 67.688 | 0.56x |
| wide_arrays.json | json | 7.882 | 7.966 | 8.513 | 67.688 | 0.40x |
| mixed.json | strata | 0.134 | 0.136 | 0.165 | 62.984 | 1.00x |
| mixed.json | orjson | 0.125 | 0.128 | 0.142 | 62.984 | 1.06x |
| mixed.json | msgspec | 0.137 | 0.139 | 0.162 | 62.984 | 0.98x |
| mixed.json | ujson | 0.180 | 0.183 | 0.200 | 62.984 | 0.75x |
| mixed.json | json | 0.295 | 0.300 | 0.322 | 62.984 | 0.45x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.936 | 1.956 | 2.068 | 55.750 | 1.00x |
| users.json | orjson | 1.953 | 1.986 | 2.107 | 55.750 | 0.99x |
| users.json | msgspec | 2.570 | 2.612 | 2.732 | 55.750 | 0.75x |
| users.json | ujson | 10.675 | 10.805 | 11.093 | 55.750 | 0.18x |
| users.json | json | 14.850 | 15.056 | 15.386 | 55.750 | 0.13x |
| flat.json | strata | 0.206 | 0.210 | 0.225 | 67.672 | 1.00x |
| flat.json | orjson | 0.221 | 0.225 | 0.230 | 67.672 | 0.93x |
| flat.json | msgspec | 0.288 | 0.293 | 0.319 | 67.672 | 0.72x |
| flat.json | ujson | 0.880 | 0.885 | 0.903 | 67.672 | 0.24x |
| flat.json | json | 1.231 | 1.237 | 1.388 | 67.672 | 0.17x |
| nested.json | strata | 0.154 | 0.156 | 0.170 | 67.672 | 1.00x |
| nested.json | orjson | 0.196 | 0.200 | 0.244 | 67.672 | 0.78x |
| nested.json | msgspec | 0.266 | 0.269 | 0.278 | 67.672 | 0.58x |
| nested.json | ujson | 0.952 | 0.962 | 1.015 | 67.672 | 0.16x |
| nested.json | json | 1.596 | 1.609 | 1.665 | 67.672 | 0.10x |
| wide_arrays.json | strata | 1.073 | 1.080 | 1.151 | 68.219 | 1.00x |
| wide_arrays.json | orjson | 1.094 | 1.113 | 1.163 | 68.219 | 0.97x |
| wide_arrays.json | msgspec | 1.913 | 1.938 | 1.992 | 68.219 | 0.56x |
| wide_arrays.json | ujson | 5.939 | 5.969 | 6.160 | 68.219 | 0.18x |
| wide_arrays.json | json | 10.607 | 10.655 | 10.986 | 68.219 | 0.10x |
| mixed.json | strata | 0.043 | 0.045 | 0.058 | 62.984 | 1.00x |
| mixed.json | orjson | 0.042 | 0.044 | 0.049 | 62.984 | 1.02x |
| mixed.json | msgspec | 0.049 | 0.051 | 0.061 | 62.984 | 0.89x |
| mixed.json | ujson | 0.210 | 0.213 | 0.225 | 62.984 | 0.21x |
| mixed.json | json | 0.337 | 0.342 | 0.373 | 62.984 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.145 | 7.333 | 7.881 | 70.375 | 1.00x |
| users.json | orjson | 10.432 | 10.801 | 11.752 | 70.375 | 0.68x |
| users.json | msgspec | 10.497 | 11.160 | 11.501 | 70.375 | 0.66x |
| users.json | ujson | 13.452 | 14.008 | 15.080 | 70.375 | 0.52x |
| users.json | json | 16.721 | 17.396 | 18.972 | 70.375 | 0.42x |
| flat.json | strata | 0.724 | 0.735 | 0.771 | 67.672 | 1.00x |
| flat.json | orjson | 0.716 | 0.727 | 0.824 | 67.672 | 1.01x |
| flat.json | msgspec | 0.750 | 0.763 | 0.857 | 67.672 | 0.96x |
| flat.json | ujson | 1.114 | 1.129 | 1.194 | 67.672 | 0.65x |
| flat.json | json | 1.468 | 1.484 | 1.515 | 67.672 | 0.50x |
| nested.json | strata | 0.620 | 0.634 | 0.692 | 67.672 | 1.00x |
| nested.json | orjson | 1.369 | 1.409 | 1.477 | 67.672 | 0.45x |
| nested.json | msgspec | 1.370 | 1.405 | 1.871 | 67.672 | 0.45x |
| nested.json | ujson | 1.664 | 1.717 | 2.125 | 67.672 | 0.37x |
| nested.json | json | 2.077 | 2.134 | 2.446 | 67.672 | 0.30x |
| wide_arrays.json | strata | 3.341 | 3.385 | 3.632 | 68.234 | 1.00x |
| wide_arrays.json | orjson | 4.226 | 4.276 | 4.706 | 68.234 | 0.79x |
| wide_arrays.json | msgspec | 4.968 | 5.027 | 5.426 | 68.234 | 0.67x |
| wide_arrays.json | ujson | 6.269 | 6.356 | 7.026 | 68.234 | 0.53x |
| wide_arrays.json | json | 8.394 | 8.480 | 9.214 | 68.234 | 0.40x |
| mixed.json | strata | 0.177 | 0.187 | 0.233 | 62.984 | 1.00x |
| mixed.json | orjson | 0.169 | 0.182 | 0.245 | 62.984 | 1.03x |
| mixed.json | msgspec | 0.183 | 0.192 | 0.265 | 62.984 | 0.98x |
| mixed.json | ujson | 0.238 | 0.256 | 0.321 | 62.984 | 0.73x |
| mixed.json | json | 0.340 | 0.353 | 0.439 | 62.984 | 0.53x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.353 | 7.490 | 7.879 | 66.797 | 1.00x |
| users.ndjson | orjson | 10.342 | 10.604 | 11.989 | 66.797 | 0.71x |
| users.ndjson | msgspec | 10.088 | 10.371 | 11.485 | 66.797 | 0.72x |
| users.ndjson | ujson | 12.982 | 13.291 | 14.929 | 66.797 | 0.56x |
| users.ndjson | json | 17.651 | 18.085 | 19.791 | 66.797 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.275 | 2.329 | 2.407 | 68.375 | 1.00x |
| users.json | orjson | 2.324 | 2.401 | 2.602 | 68.375 | 0.97x |
| users.json | msgspec | 2.913 | 2.974 | 3.097 | 68.375 | 0.78x |
| users.json | ujson | 10.948 | 11.069 | 11.490 | 68.375 | 0.21x |
| users.json | json | 15.299 | 15.399 | 15.870 | 68.375 | 0.15x |
| flat.json | strata | 0.329 | 0.350 | 0.407 | 67.672 | 1.00x |
| flat.json | orjson | 0.347 | 0.367 | 0.421 | 67.672 | 0.95x |
| flat.json | msgspec | 0.412 | 0.432 | 0.478 | 67.672 | 0.81x |
| flat.json | ujson | 0.988 | 1.040 | 1.131 | 67.672 | 0.34x |
| flat.json | json | 1.372 | 1.393 | 1.509 | 67.672 | 0.25x |
| nested.json | strata | 0.270 | 0.281 | 0.326 | 67.672 | 1.00x |
| nested.json | orjson | 0.312 | 0.322 | 0.420 | 67.672 | 0.87x |
| nested.json | msgspec | 0.380 | 0.395 | 0.458 | 67.672 | 0.71x |
| nested.json | ujson | 1.076 | 1.092 | 1.171 | 67.672 | 0.26x |
| nested.json | json | 1.711 | 1.736 | 1.811 | 67.672 | 0.16x |
| wide_arrays.json | strata | 1.306 | 1.349 | 1.408 | 68.234 | 1.00x |
| wide_arrays.json | orjson | 1.364 | 1.402 | 1.450 | 68.234 | 0.96x |
| wide_arrays.json | msgspec | 2.185 | 2.220 | 2.414 | 68.234 | 0.61x |
| wide_arrays.json | ujson | 6.256 | 6.308 | 6.528 | 68.234 | 0.21x |
| wide_arrays.json | json | 10.932 | 11.010 | 11.357 | 68.234 | 0.12x |
| mixed.json | strata | 0.132 | 0.142 | 0.169 | 63.016 | 1.00x |
| mixed.json | orjson | 0.132 | 0.146 | 0.161 | 63.016 | 0.97x |
| mixed.json | msgspec | 0.143 | 0.150 | 0.168 | 63.016 | 0.95x |
| mixed.json | ujson | 0.310 | 0.331 | 0.437 | 63.016 | 0.43x |
| mixed.json | json | 0.435 | 0.450 | 0.519 | 63.016 | 0.32x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.051 | 0.057 | 0.082 | 68.875 | 1.00x |
| users.json $[*].id | jmespath | 0.328 | 0.337 | 0.396 | 68.875 | 0.17x |
| users.json $[*].id | jsonpath-ng | 3.357 | 3.469 | 3.953 | 68.875 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.344 | 0.355 | 0.472 | 69.406 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.026 | 2.093 | 2.443 | 69.406 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 11.124 | 11.492 | 12.710 | 69.406 | 0.03x |
| users.json $..total | strata | 1.357 | 1.379 | 1.444 | 62.766 | 1.00x |
| users.json $..total | jsonpath-ng | 205.793 | 207.568 | 211.367 | 62.766 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.191 | 3.277 | 3.352 | 69.000 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.782 | 11.095 | 11.634 | 69.000 | 0.30x |
| users.json $[*].id | orjson+jsonpath-ng | 11.921 | 12.176 | 12.844 | 69.000 | 0.27x |
| users.json $[*].orders[*].total | strata | 3.306 | 3.394 | 3.515 | 69.500 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.540 | 12.969 | 13.998 | 69.500 | 0.26x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 19.974 | 20.556 | 22.293 | 69.500 | 0.17x |
| users.json $..total | strata | 10.603 | 11.031 | 11.971 | 68.922 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 213.526 | 216.266 | 222.175 | 68.922 | 0.05x |

