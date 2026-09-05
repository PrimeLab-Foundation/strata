# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 53fa480
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 25 Model 17 Stepping 1, AuthenticAMD
- compiler_flags: clang-cl /std:c++20 /O2 /arch:AVX2 -fprofile-use (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.105 | 7.284 | 11.587 | 45.613 | 1.00x |
| users.json | orjson | 10.842 | 11.327 | 13.034 | 45.613 | 0.64x |
| users.json | msgspec | 10.188 | 10.304 | 17.977 | 45.613 | 0.71x |
| users.json | ujson | 15.482 | 16.835 | 22.726 | 45.613 | 0.43x |
| users.json | json | 17.354 | 17.673 | 19.286 | 45.613 | 0.41x |
| flat.json | strata | 0.891 | 0.928 | 1.155 | 49.988 | 1.00x |
| flat.json | orjson | 0.990 | 1.040 | 1.103 | 49.988 | 0.89x |
| flat.json | msgspec | 0.918 | 0.993 | 1.076 | 49.988 | 0.93x |
| flat.json | ujson | 1.491 | 1.701 | 1.843 | 49.988 | 0.55x |
| flat.json | json | 1.503 | 1.519 | 1.585 | 49.988 | 0.61x |
| nested.json | strata | 0.590 | 0.595 | 0.620 | 49.691 | 1.00x |
| nested.json | orjson | 0.848 | 0.876 | 0.898 | 49.691 | 0.68x |
| nested.json | msgspec | 0.753 | 0.766 | 0.805 | 49.691 | 0.78x |
| nested.json | ujson | 1.221 | 1.254 | 1.315 | 49.691 | 0.47x |
| nested.json | json | 1.592 | 1.621 | 1.632 | 49.691 | 0.37x |
| wide_arrays.json | strata | 3.418 | 3.444 | 3.733 | 51.660 | 1.00x |
| wide_arrays.json | orjson | 4.535 | 4.667 | 4.744 | 51.660 | 0.74x |
| wide_arrays.json | msgspec | 4.577 | 4.604 | 4.682 | 51.660 | 0.75x |
| wide_arrays.json | ujson | 6.209 | 6.241 | 6.355 | 51.660 | 0.55x |
| wide_arrays.json | json | 8.832 | 8.899 | 9.462 | 51.660 | 0.39x |
| mixed.json | strata | 0.146 | 0.149 | 0.165 | 49.438 | 1.00x |
| mixed.json | orjson | 0.227 | 0.231 | 0.264 | 49.438 | 0.65x |
| mixed.json | msgspec | 0.181 | 0.182 | 0.212 | 49.438 | 0.82x |
| mixed.json | ujson | 0.278 | 0.284 | 0.313 | 49.438 | 0.53x |
| mixed.json | json | 0.351 | 0.363 | 0.382 | 49.438 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.473 | 2.535 | 2.633 | 44.949 | 1.00x |
| users.json | orjson | 2.604 | 2.673 | 2.788 | 44.949 | 0.95x |
| users.json | msgspec | 4.129 | 4.397 | 5.572 | 44.949 | 0.58x |
| users.json | ujson | 9.966 | 10.064 | 10.568 | 44.949 | 0.25x |
| users.json | json | 18.067 | 18.341 | 18.750 | 44.949 | 0.14x |
| flat.json | strata | 0.235 | 0.239 | 0.264 | 49.957 | 1.00x |
| flat.json | orjson | 0.259 | 0.267 | 0.346 | 49.957 | 0.90x |
| flat.json | msgspec | 0.429 | 0.438 | 0.527 | 49.957 | 0.55x |
| flat.json | ujson | 1.135 | 1.149 | 1.231 | 49.957 | 0.21x |
| flat.json | json | 1.526 | 1.568 | 1.747 | 49.957 | 0.15x |
| nested.json | strata | 0.197 | 0.201 | 0.220 | 50.102 | 1.00x |
| nested.json | orjson | 0.233 | 0.236 | 0.268 | 50.102 | 0.85x |
| nested.json | msgspec | 0.398 | 0.409 | 0.472 | 50.102 | 0.49x |
| nested.json | ujson | 0.782 | 0.800 | 0.954 | 50.102 | 0.25x |
| nested.json | json | 1.948 | 1.967 | 1.991 | 50.102 | 0.10x |
| wide_arrays.json | strata | 1.626 | 1.645 | 2.015 | 51.492 | 1.00x |
| wide_arrays.json | orjson | 2.021 | 2.050 | 2.071 | 51.492 | 0.80x |
| wide_arrays.json | msgspec | 3.515 | 3.564 | 3.745 | 51.492 | 0.46x |
| wide_arrays.json | ujson | 6.022 | 6.061 | 6.136 | 51.492 | 0.27x |
| wide_arrays.json | json | 14.178 | 14.237 | 20.944 | 51.492 | 0.12x |
| mixed.json | strata | 0.057 | 0.058 | 0.059 | 49.570 | 1.00x |
| mixed.json | orjson | 0.052 | 0.053 | 0.079 | 49.570 | 1.09x |
| mixed.json | msgspec | 0.079 | 0.080 | 0.178 | 49.570 | 0.72x |
| mixed.json | ujson | 0.199 | 0.211 | 0.231 | 49.570 | 0.27x |
| mixed.json | json | 0.402 | 0.410 | 0.452 | 49.570 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.111 | 8.502 | 9.836 | 54.988 | 1.00x |
| users.json | orjson | 11.823 | 12.226 | 14.693 | 54.988 | 0.70x |
| users.json | msgspec | 11.003 | 11.366 | 13.830 | 54.988 | 0.75x |
| users.json | ujson | 19.039 | 20.171 | 21.136 | 54.988 | 0.42x |
| users.json | json | 18.278 | 18.490 | 18.887 | 54.988 | 0.46x |
| flat.json | strata | 1.095 | 1.152 | 1.543 | 49.465 | 1.00x |
| flat.json | orjson | 1.189 | 1.264 | 1.385 | 49.465 | 0.91x |
| flat.json | msgspec | 1.038 | 1.121 | 1.605 | 49.465 | 1.03x |
| flat.json | ujson | 1.923 | 2.114 | 3.048 | 49.465 | 0.54x |
| flat.json | json | 1.715 | 1.778 | 2.800 | 49.465 | 0.65x |
| nested.json | strata | 0.669 | 0.684 | 0.725 | 49.699 | 1.00x |
| nested.json | orjson | 0.934 | 0.971 | 1.028 | 49.699 | 0.70x |
| nested.json | msgspec | 0.856 | 0.874 | 0.908 | 49.699 | 0.78x |
| nested.json | ujson | 1.442 | 1.482 | 1.513 | 49.699 | 0.46x |
| nested.json | json | 1.685 | 1.726 | 1.784 | 49.699 | 0.40x |
| wide_arrays.json | strata | 3.898 | 3.958 | 4.169 | 51.492 | 1.00x |
| wide_arrays.json | orjson | 4.959 | 5.107 | 5.707 | 51.492 | 0.77x |
| wide_arrays.json | msgspec | 5.041 | 5.241 | 6.293 | 51.492 | 0.76x |
| wide_arrays.json | ujson | 8.236 | 8.527 | 9.480 | 51.492 | 0.46x |
| wide_arrays.json | json | 9.272 | 9.445 | 9.605 | 51.492 | 0.42x |
| mixed.json | strata | 0.206 | 0.225 | 0.239 | 49.449 | 1.00x |
| mixed.json | orjson | 0.324 | 0.339 | 0.360 | 49.449 | 0.66x |
| mixed.json | msgspec | 0.277 | 0.288 | 0.313 | 49.449 | 0.78x |
| mixed.json | ujson | 0.410 | 0.436 | 0.461 | 49.449 | 0.52x |
| mixed.json | json | 0.447 | 0.466 | 0.513 | 49.449 | 0.48x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.226 | 10.569 | 11.872 | 50.832 | 1.00x |
| users.ndjson | orjson | 15.238 | 16.535 | 17.008 | 50.832 | 0.64x |
| users.ndjson | msgspec | 14.899 | 15.798 | 17.361 | 50.832 | 0.67x |
| users.ndjson | ujson | 21.283 | 23.764 | 31.525 | 50.832 | 0.44x |
| users.ndjson | json | 25.416 | 27.084 | 42.352 | 50.832 | 0.39x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.298 | 3.387 | 4.532 | 51.383 | 1.00x |
| users.json | orjson | 3.231 | 3.646 | 70.418 | 51.383 | 0.93x |
| users.json | msgspec | 4.897 | 5.212 | 5.311 | 51.383 | 0.65x |
| users.json | ujson | 16.978 | 17.171 | 17.426 | 51.383 | 0.20x |
| users.json | json | 25.450 | 25.776 | 27.157 | 51.383 | 0.13x |
| flat.json | strata | 0.545 | 0.584 | 0.661 | 50.066 | 1.00x |
| flat.json | orjson | 0.579 | 0.611 | 0.660 | 50.066 | 0.96x |
| flat.json | msgspec | 0.732 | 0.764 | 0.829 | 50.066 | 0.76x |
| flat.json | ujson | 2.207 | 2.284 | 2.346 | 50.066 | 0.26x |
| flat.json | json | 2.650 | 2.810 | 2.956 | 50.066 | 0.21x |
| nested.json | strata | 0.468 | 0.500 | 0.599 | 50.219 | 1.00x |
| nested.json | orjson | 0.519 | 0.553 | 0.583 | 50.219 | 0.90x |
| nested.json | msgspec | 0.680 | 0.715 | 0.850 | 50.219 | 0.70x |
| nested.json | ujson | 1.610 | 1.660 | 1.686 | 50.219 | 0.30x |
| nested.json | json | 2.768 | 2.797 | 2.969 | 50.219 | 0.18x |
| wide_arrays.json | strata | 2.240 | 2.354 | 3.300 | 51.531 | 1.00x |
| wide_arrays.json | orjson | 2.597 | 2.631 | 100.028 | 51.531 | 0.89x |
| wide_arrays.json | msgspec | 4.079 | 4.191 | 4.298 | 51.531 | 0.56x |
| wide_arrays.json | ujson | 11.223 | 11.426 | 15.252 | 51.531 | 0.21x |
| wide_arrays.json | json | 19.326 | 19.681 | 21.177 | 51.531 | 0.12x |
| mixed.json | strata | 0.301 | 0.314 | 0.324 | 49.590 | 1.00x |
| mixed.json | orjson | 0.300 | 0.308 | 0.343 | 49.590 | 1.02x |
| mixed.json | msgspec | 0.332 | 0.337 | 0.382 | 49.590 | 0.93x |
| mixed.json | ujson | 0.574 | 0.590 | 0.639 | 49.590 | 0.53x |
| mixed.json | json | 0.786 | 0.795 | 0.847 | 49.590 | 0.39x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.082 | 0.086 | 0.092 | 51.664 | 1.00x |
| users.json $[*].id | jmespath | 0.331 | 0.341 | 0.364 | 51.664 | 0.25x |
| users.json $[*].id | jsonpath-ng | 1.966 | 2.294 | 2.907 | 51.664 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.418 | 0.442 | 0.483 | 51.930 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.067 | 2.167 | 3.119 | 51.930 | 0.20x |
| users.json $[*].orders[*].total | jsonpath-ng | 13.212 | 14.443 | 15.820 | 51.930 | 0.03x |
| users.json $..total | strata | 1.529 | 1.569 | 2.707 | 52.949 | 1.00x |
| users.json $..total | jsonpath-ng | 249.820 | 252.713 | 297.101 | 52.949 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.247 | 3.308 | 4.208 | 51.684 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.999 | 13.177 | 13.733 | 51.684 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 14.607 | 15.259 | 15.743 | 51.684 | 0.22x |
| users.json $[*].orders[*].total | strata | 3.451 | 3.484 | 3.628 | 52.934 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.080 | 15.294 | 20.765 | 52.934 | 0.23x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 28.933 | 29.587 | 33.898 | 52.934 | 0.12x |
| users.json $..total | strata | 10.645 | 11.160 | 12.668 | 52.574 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 268.707 | 271.993 | 289.317 | 52.574 | 0.04x |

