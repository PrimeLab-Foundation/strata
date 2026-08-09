# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 5292dd0
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
| users.json | strata | 7.603 | 7.783 | 8.027 | 52.656 | 1.00x |
| users.json | orjson | 8.275 | 8.528 | 8.748 | 52.656 | 0.91x |
| users.json | msgspec | 8.433 | 8.614 | 9.255 | 52.656 | 0.90x |
| users.json | ujson | 11.184 | 11.439 | 11.943 | 52.656 | 0.68x |
| users.json | json | 14.790 | 15.193 | 15.720 | 52.656 | 0.51x |
| flat.json | strata | 0.765 | 0.775 | 0.846 | 65.906 | 1.00x |
| flat.json | orjson | 0.650 | 0.662 | 0.756 | 65.906 | 1.17x |
| flat.json | msgspec | 0.686 | 0.698 | 0.772 | 65.906 | 1.11x |
| flat.json | ujson | 1.016 | 1.039 | 1.076 | 65.906 | 0.75x |
| flat.json | json | 1.422 | 1.434 | 1.536 | 65.906 | 0.54x |
| nested.json | strata | 0.598 | 0.605 | 0.635 | 65.922 | 1.00x |
| nested.json | orjson | 1.189 | 1.213 | 1.374 | 65.922 | 0.50x |
| nested.json | msgspec | 1.192 | 1.252 | 1.621 | 65.922 | 0.48x |
| nested.json | ujson | 1.466 | 1.505 | 1.632 | 65.922 | 0.40x |
| nested.json | json | 1.912 | 1.943 | 2.044 | 65.922 | 0.31x |
| wide_arrays.json | strata | 3.531 | 3.553 | 3.816 | 69.172 | 1.00x |
| wide_arrays.json | orjson | 3.784 | 3.847 | 4.174 | 69.172 | 0.92x |
| wide_arrays.json | msgspec | 4.430 | 4.548 | 5.031 | 69.172 | 0.78x |
| wide_arrays.json | ujson | 5.644 | 5.735 | 6.263 | 69.172 | 0.62x |
| wide_arrays.json | json | 7.938 | 8.062 | 8.556 | 69.172 | 0.44x |
| mixed.json | strata | 0.156 | 0.163 | 0.177 | 69.703 | 1.00x |
| mixed.json | orjson | 0.124 | 0.129 | 0.145 | 69.703 | 1.27x |
| mixed.json | msgspec | 0.137 | 0.141 | 0.166 | 69.703 | 1.16x |
| mixed.json | ujson | 0.183 | 0.189 | 0.224 | 69.703 | 0.86x |
| mixed.json | json | 0.298 | 0.306 | 0.340 | 69.703 | 0.53x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.941 | 1.955 | 2.100 | 57.297 | 1.00x |
| users.json | orjson | 1.945 | 1.973 | 2.030 | 57.297 | 0.99x |
| users.json | msgspec | 2.593 | 2.614 | 2.737 | 57.297 | 0.75x |
| users.json | ujson | 10.703 | 10.786 | 11.156 | 57.297 | 0.18x |
| users.json | json | 14.840 | 14.979 | 15.438 | 57.297 | 0.13x |
| flat.json | strata | 0.210 | 0.213 | 0.230 | 65.922 | 1.00x |
| flat.json | orjson | 0.222 | 0.225 | 0.241 | 65.922 | 0.95x |
| flat.json | msgspec | 0.287 | 0.292 | 0.301 | 65.922 | 0.73x |
| flat.json | ujson | 0.863 | 0.871 | 0.931 | 65.922 | 0.25x |
| flat.json | json | 1.238 | 1.252 | 1.315 | 65.922 | 0.17x |
| nested.json | strata | 0.157 | 0.164 | 0.184 | 65.922 | 1.00x |
| nested.json | orjson | 0.195 | 0.200 | 0.215 | 65.922 | 0.82x |
| nested.json | msgspec | 0.266 | 0.273 | 0.296 | 65.922 | 0.60x |
| nested.json | ujson | 0.997 | 1.018 | 1.057 | 65.922 | 0.16x |
| nested.json | json | 1.568 | 1.608 | 1.720 | 65.922 | 0.10x |
| wide_arrays.json | strata | 1.091 | 1.108 | 1.223 | 69.719 | 1.00x |
| wide_arrays.json | orjson | 1.090 | 1.109 | 1.159 | 69.719 | 1.00x |
| wide_arrays.json | msgspec | 1.887 | 1.913 | 1.995 | 69.719 | 0.58x |
| wide_arrays.json | ujson | 5.937 | 6.016 | 6.114 | 69.719 | 0.18x |
| wide_arrays.json | json | 10.551 | 10.785 | 11.133 | 69.719 | 0.10x |
| mixed.json | strata | 0.045 | 0.048 | 0.055 | 69.703 | 1.00x |
| mixed.json | orjson | 0.042 | 0.043 | 0.051 | 69.703 | 1.13x |
| mixed.json | msgspec | 0.049 | 0.051 | 0.058 | 69.703 | 0.95x |
| mixed.json | ujson | 0.212 | 0.218 | 0.231 | 69.703 | 0.22x |
| mixed.json | json | 0.336 | 0.342 | 0.360 | 69.703 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.877 | 8.133 | 8.904 | 72.531 | 1.00x |
| users.json | orjson | 10.434 | 10.913 | 11.979 | 72.531 | 0.75x |
| users.json | msgspec | 10.537 | 10.981 | 11.931 | 72.531 | 0.74x |
| users.json | ujson | 13.478 | 14.109 | 15.284 | 72.531 | 0.58x |
| users.json | json | 16.893 | 17.445 | 18.960 | 72.531 | 0.47x |
| flat.json | strata | 0.827 | 0.849 | 0.895 | 65.922 | 1.00x |
| flat.json | orjson | 0.719 | 0.740 | 0.792 | 65.922 | 1.15x |
| flat.json | msgspec | 0.755 | 0.772 | 0.838 | 65.922 | 1.10x |
| flat.json | ujson | 1.120 | 1.153 | 1.267 | 65.922 | 0.74x |
| flat.json | json | 1.480 | 1.504 | 1.631 | 65.922 | 0.56x |
| nested.json | strata | 0.664 | 0.684 | 0.739 | 65.922 | 1.00x |
| nested.json | orjson | 1.371 | 1.454 | 1.667 | 65.922 | 0.47x |
| nested.json | msgspec | 1.375 | 1.448 | 1.667 | 65.922 | 0.47x |
| nested.json | ujson | 1.689 | 1.766 | 2.101 | 65.922 | 0.39x |
| nested.json | json | 2.093 | 2.171 | 2.344 | 65.922 | 0.32x |
| wide_arrays.json | strata | 3.722 | 3.806 | 4.000 | 69.719 | 1.00x |
| wide_arrays.json | orjson | 4.259 | 4.384 | 4.808 | 69.719 | 0.87x |
| wide_arrays.json | msgspec | 5.028 | 5.125 | 5.465 | 69.719 | 0.74x |
| wide_arrays.json | ujson | 6.323 | 6.551 | 6.998 | 69.719 | 0.58x |
| wide_arrays.json | json | 8.500 | 8.780 | 9.131 | 69.719 | 0.43x |
| mixed.json | strata | 0.199 | 0.216 | 0.258 | 69.703 | 1.00x |
| mixed.json | orjson | 0.173 | 0.188 | 0.224 | 69.703 | 1.15x |
| mixed.json | msgspec | 0.186 | 0.200 | 0.246 | 69.703 | 1.08x |
| mixed.json | ujson | 0.239 | 0.257 | 0.294 | 69.703 | 0.84x |
| mixed.json | json | 0.345 | 0.362 | 0.384 | 69.703 | 0.60x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.570 | 7.714 | 8.018 | 65.031 | 1.00x |
| users.ndjson | orjson | 10.409 | 10.626 | 11.906 | 65.031 | 0.73x |
| users.ndjson | msgspec | 10.212 | 10.430 | 11.046 | 65.031 | 0.74x |
| users.ndjson | ujson | 13.036 | 13.382 | 14.712 | 65.031 | 0.58x |
| users.ndjson | json | 17.979 | 18.386 | 19.609 | 65.031 | 0.42x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.313 | 2.392 | 2.511 | 69.875 | 1.00x |
| users.json | orjson | 2.389 | 2.750 | 3.043 | 69.875 | 0.87x |
| users.json | msgspec | 2.968 | 3.032 | 3.153 | 69.875 | 0.79x |
| users.json | ujson | 10.898 | 11.086 | 11.386 | 69.875 | 0.22x |
| users.json | json | 15.174 | 15.507 | 15.859 | 69.875 | 0.15x |
| flat.json | strata | 0.347 | 0.381 | 0.473 | 65.922 | 1.00x |
| flat.json | orjson | 0.338 | 0.364 | 0.456 | 65.922 | 1.05x |
| flat.json | msgspec | 0.415 | 0.436 | 0.520 | 65.922 | 0.87x |
| flat.json | ujson | 0.984 | 1.013 | 1.156 | 65.922 | 0.38x |
| flat.json | json | 1.369 | 1.399 | 1.551 | 65.922 | 0.27x |
| nested.json | strata | 0.300 | 0.329 | 0.384 | 65.922 | 1.00x |
| nested.json | orjson | 0.315 | 0.339 | 0.383 | 65.922 | 0.97x |
| nested.json | msgspec | 0.383 | 0.403 | 0.491 | 65.922 | 0.82x |
| nested.json | ujson | 1.153 | 1.191 | 1.365 | 65.922 | 0.28x |
| nested.json | json | 1.724 | 1.752 | 1.906 | 65.922 | 0.19x |
| wide_arrays.json | strata | 1.389 | 1.484 | 1.920 | 69.703 | 1.00x |
| wide_arrays.json | orjson | 1.394 | 1.444 | 1.692 | 69.703 | 1.03x |
| wide_arrays.json | msgspec | 2.188 | 2.250 | 2.607 | 69.703 | 0.66x |
| wide_arrays.json | ujson | 6.304 | 6.382 | 6.607 | 69.703 | 0.23x |
| wide_arrays.json | json | 10.966 | 11.108 | 11.522 | 69.703 | 0.13x |
| mixed.json | strata | 0.172 | 0.204 | 0.260 | 69.734 | 1.00x |
| mixed.json | orjson | 0.155 | 0.175 | 0.227 | 69.734 | 1.17x |
| mixed.json | msgspec | 0.157 | 0.173 | 0.232 | 69.734 | 1.18x |
| mixed.json | ujson | 0.334 | 0.379 | 0.469 | 69.734 | 0.54x |
| mixed.json | json | 0.458 | 0.487 | 0.585 | 69.734 | 0.42x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.051 | 0.059 | 0.091 | 70.203 | 1.00x |
| users.json $[*].id | jmespath | 0.336 | 0.348 | 0.389 | 70.203 | 0.17x |
| users.json $[*].id | jsonpath-ng | 3.360 | 3.509 | 3.867 | 70.203 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.337 | 0.357 | 0.396 | 70.781 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.049 | 2.084 | 2.329 | 70.781 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 11.154 | 11.395 | 12.419 | 70.781 | 0.03x |
| users.json $..total | strata | 1.347 | 1.361 | 1.454 | 71.391 | 1.00x |
| users.json $..total | jsonpath-ng | 206.979 | 208.243 | 213.403 | 71.391 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.696 | 3.808 | 3.984 | 70.359 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.868 | 11.085 | 11.718 | 70.359 | 0.34x |
| users.json $[*].id | orjson+jsonpath-ng | 12.090 | 12.457 | 13.062 | 70.359 | 0.31x |
| users.json $[*].orders[*].total | strata | 3.785 | 3.900 | 3.972 | 72.844 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.661 | 13.118 | 13.790 | 72.844 | 0.30x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 19.983 | 20.311 | 21.619 | 72.844 | 0.19x |
| users.json $..total | strata | 11.496 | 11.855 | 12.392 | 71.375 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 216.003 | 217.222 | 222.511 | 71.375 | 0.05x |

