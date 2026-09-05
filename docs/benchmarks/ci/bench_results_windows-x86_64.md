# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 1cc7049
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD
- compiler_flags: clang-cl /std:c++20 /O2 /arch:AVX2 -fprofile-use (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.807 | 9.135 | 13.855 | 45.680 | 1.00x |
| users.json | orjson | 13.099 | 14.521 | 19.325 | 45.680 | 0.63x |
| users.json | msgspec | 12.532 | 12.843 | 20.706 | 45.680 | 0.71x |
| users.json | ujson | 21.090 | 21.592 | 24.133 | 45.680 | 0.42x |
| users.json | json | 22.145 | 22.474 | 25.714 | 45.680 | 0.41x |
| flat.json | strata | 0.999 | 1.040 | 1.112 | 50.660 | 1.00x |
| flat.json | orjson | 1.078 | 1.122 | 1.163 | 50.660 | 0.93x |
| flat.json | msgspec | 1.085 | 1.130 | 1.188 | 50.660 | 0.92x |
| flat.json | ujson | 2.110 | 2.171 | 2.198 | 50.660 | 0.48x |
| flat.json | json | 1.966 | 1.997 | 2.099 | 50.660 | 0.52x |
| nested.json | strata | 0.751 | 0.768 | 1.227 | 50.406 | 1.00x |
| nested.json | orjson | 1.038 | 1.080 | 1.648 | 50.406 | 0.71x |
| nested.json | msgspec | 0.992 | 1.030 | 1.691 | 50.406 | 0.75x |
| nested.json | ujson | 1.544 | 1.585 | 2.659 | 50.406 | 0.48x |
| nested.json | json | 2.125 | 2.133 | 2.892 | 50.406 | 0.36x |
| wide_arrays.json | strata | 4.158 | 4.220 | 4.289 | 52.391 | 1.00x |
| wide_arrays.json | orjson | 5.538 | 5.627 | 5.759 | 52.391 | 0.75x |
| wide_arrays.json | msgspec | 5.929 | 6.026 | 6.200 | 52.391 | 0.70x |
| wide_arrays.json | ujson | 8.292 | 8.352 | 9.080 | 52.391 | 0.51x |
| wide_arrays.json | json | 11.537 | 11.595 | 12.744 | 52.391 | 0.36x |
| mixed.json | strata | 0.183 | 0.185 | 0.225 | 52.602 | 1.00x |
| mixed.json | orjson | 0.210 | 0.212 | 0.254 | 52.602 | 0.87x |
| mixed.json | msgspec | 0.234 | 0.237 | 0.240 | 52.602 | 0.78x |
| mixed.json | ujson | 0.342 | 0.348 | 0.394 | 52.602 | 0.53x |
| mixed.json | json | 0.466 | 0.474 | 0.517 | 52.602 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.069 | 3.104 | 3.303 | 44.977 | 1.00x |
| users.json | orjson | 3.689 | 3.740 | 4.130 | 44.977 | 0.83x |
| users.json | msgspec | 5.436 | 5.497 | 5.797 | 44.977 | 0.56x |
| users.json | ujson | 13.992 | 14.166 | 14.512 | 44.977 | 0.22x |
| users.json | json | 23.375 | 23.708 | 24.184 | 44.977 | 0.13x |
| flat.json | strata | 0.273 | 0.276 | 0.323 | 50.930 | 1.00x |
| flat.json | orjson | 0.356 | 0.381 | 0.412 | 50.930 | 0.72x |
| flat.json | msgspec | 0.491 | 0.503 | 0.556 | 50.930 | 0.55x |
| flat.json | ujson | 1.442 | 1.481 | 1.526 | 50.930 | 0.19x |
| flat.json | json | 1.976 | 2.024 | 2.180 | 50.930 | 0.14x |
| nested.json | strata | 0.280 | 0.280 | 0.413 | 50.465 | 1.00x |
| nested.json | orjson | 0.351 | 0.368 | 0.417 | 50.465 | 0.76x |
| nested.json | msgspec | 0.468 | 0.474 | 0.536 | 50.465 | 0.59x |
| nested.json | ujson | 1.255 | 1.283 | 1.316 | 50.465 | 0.22x |
| nested.json | json | 2.506 | 2.628 | 2.767 | 50.465 | 0.11x |
| wide_arrays.json | strata | 1.868 | 1.900 | 2.164 | 53.711 | 1.00x |
| wide_arrays.json | orjson | 2.239 | 2.384 | 2.540 | 53.711 | 0.80x |
| wide_arrays.json | msgspec | 3.598 | 4.028 | 4.637 | 53.711 | 0.47x |
| wide_arrays.json | ujson | 7.513 | 7.608 | 7.676 | 53.711 | 0.25x |
| wide_arrays.json | json | 19.358 | 19.643 | 26.839 | 53.711 | 0.10x |
| mixed.json | strata | 0.069 | 0.071 | 0.075 | 52.633 | 1.00x |
| mixed.json | orjson | 0.068 | 0.069 | 0.081 | 52.633 | 1.02x |
| mixed.json | msgspec | 0.091 | 0.092 | 0.127 | 52.633 | 0.77x |
| mixed.json | ujson | 0.260 | 0.264 | 0.333 | 52.633 | 0.27x |
| mixed.json | json | 0.518 | 0.523 | 0.566 | 52.633 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.569 | 10.986 | 13.279 | 55.000 | 1.00x |
| users.json | orjson | 14.190 | 14.749 | 16.501 | 55.000 | 0.74x |
| users.json | msgspec | 13.666 | 14.243 | 14.670 | 55.000 | 0.77x |
| users.json | ujson | 26.105 | 26.688 | 28.268 | 55.000 | 0.41x |
| users.json | json | 23.114 | 23.997 | 25.392 | 55.000 | 0.46x |
| flat.json | strata | 1.212 | 1.252 | 1.285 | 50.176 | 1.00x |
| flat.json | orjson | 1.416 | 1.442 | 1.703 | 50.176 | 0.87x |
| flat.json | msgspec | 1.217 | 1.270 | 1.365 | 50.176 | 0.99x |
| flat.json | ujson | 2.731 | 2.792 | 2.908 | 50.176 | 0.45x |
| flat.json | json | 2.225 | 2.275 | 2.660 | 50.176 | 0.55x |
| nested.json | strata | 0.846 | 0.896 | 0.908 | 50.629 | 1.00x |
| nested.json | orjson | 1.165 | 1.206 | 1.260 | 50.629 | 0.74x |
| nested.json | msgspec | 1.111 | 1.141 | 1.165 | 50.629 | 0.79x |
| nested.json | ujson | 1.946 | 1.963 | 2.005 | 50.629 | 0.46x |
| nested.json | json | 2.245 | 2.268 | 2.309 | 50.629 | 0.40x |
| wide_arrays.json | strata | 4.649 | 4.748 | 8.093 | 53.711 | 1.00x |
| wide_arrays.json | orjson | 6.080 | 6.189 | 8.805 | 53.711 | 0.77x |
| wide_arrays.json | msgspec | 6.414 | 6.637 | 10.181 | 53.711 | 0.72x |
| wide_arrays.json | ujson | 11.412 | 11.586 | 13.534 | 53.711 | 0.41x |
| wide_arrays.json | json | 11.996 | 12.270 | 12.497 | 53.711 | 0.39x |
| mixed.json | strata | 0.262 | 0.266 | 0.291 | 50.273 | 1.00x |
| mixed.json | orjson | 0.401 | 0.407 | 0.495 | 50.273 | 0.65x |
| mixed.json | msgspec | 0.357 | 0.361 | 0.443 | 50.273 | 0.74x |
| mixed.json | ujson | 0.556 | 0.569 | 0.660 | 50.273 | 0.47x |
| mixed.json | json | 0.588 | 0.608 | 0.819 | 50.273 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.179 | 11.198 | 13.012 | 51.750 | 1.00x |
| users.ndjson | orjson | 17.207 | 18.028 | 19.021 | 51.750 | 0.62x |
| users.ndjson | msgspec | 17.443 | 17.794 | 19.753 | 51.750 | 0.63x |
| users.ndjson | ujson | 25.085 | 25.688 | 27.807 | 51.750 | 0.44x |
| users.ndjson | json | 29.646 | 30.978 | 33.807 | 51.750 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.952 | 4.040 | 4.251 | 54.059 | 1.00x |
| users.json | orjson | 4.417 | 4.503 | 4.734 | 54.059 | 0.90x |
| users.json | msgspec | 5.756 | 5.881 | 6.429 | 54.059 | 0.69x |
| users.json | ujson | 23.216 | 23.605 | 36.589 | 54.059 | 0.17x |
| users.json | json | 32.001 | 32.389 | 42.425 | 54.059 | 0.12x |
| flat.json | strata | 0.616 | 0.636 | 0.692 | 50.840 | 1.00x |
| flat.json | orjson | 0.749 | 0.804 | 3.799 | 50.840 | 0.79x |
| flat.json | msgspec | 0.859 | 0.898 | 12.819 | 50.840 | 0.71x |
| flat.json | ujson | 2.848 | 2.888 | 3.529 | 50.840 | 0.22x |
| flat.json | json | 3.592 | 3.641 | 3.684 | 50.840 | 0.17x |
| nested.json | strata | 0.612 | 0.622 | 0.696 | 50.590 | 1.00x |
| nested.json | orjson | 0.667 | 0.703 | 0.748 | 50.590 | 0.89x |
| nested.json | msgspec | 0.809 | 0.843 | 0.938 | 50.590 | 0.74x |
| nested.json | ujson | 2.352 | 2.389 | 2.579 | 50.590 | 0.26x |
| nested.json | json | 3.697 | 3.723 | 4.182 | 50.590 | 0.17x |
| wide_arrays.json | strata | 2.628 | 2.673 | 3.015 | 53.711 | 1.00x |
| wide_arrays.json | orjson | 2.930 | 3.136 | 3.384 | 53.711 | 0.85x |
| wide_arrays.json | msgspec | 4.307 | 4.716 | 5.097 | 53.711 | 0.57x |
| wide_arrays.json | ujson | 14.298 | 14.442 | 14.872 | 53.711 | 0.19x |
| wide_arrays.json | json | 26.108 | 26.430 | 27.997 | 53.711 | 0.10x |
| mixed.json | strata | 0.375 | 0.386 | 0.438 | 50.445 | 1.00x |
| mixed.json | orjson | 0.377 | 0.385 | 0.400 | 50.445 | 1.00x |
| mixed.json | msgspec | 0.401 | 0.424 | 0.559 | 50.445 | 0.91x |
| mixed.json | ujson | 0.741 | 0.784 | 0.814 | 50.445 | 0.49x |
| mixed.json | json | 0.993 | 0.998 | 1.090 | 50.445 | 0.39x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.081 | 0.083 | 0.113 | 54.094 | 1.00x |
| users.json $[*].id | jmespath | 0.439 | 0.453 | 0.483 | 54.094 | 0.18x |
| users.json $[*].id | jsonpath-ng | 2.443 | 2.520 | 4.739 | 54.094 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.496 | 0.552 | 0.824 | 54.113 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.855 | 2.899 | 5.227 | 54.113 | 0.19x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.200 | 23.720 | 31.594 | 54.113 | 0.02x |
| users.json $..total | strata | 1.915 | 1.983 | 2.657 | 54.113 | 1.00x |
| users.json $..total | jsonpath-ng | 335.861 | 342.204 | 352.498 | 54.113 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.770 | 3.833 | 3.985 | 54.113 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.504 | 14.648 | 16.110 | 54.113 | 0.26x |
| users.json $[*].id | orjson+jsonpath-ng | 16.647 | 17.230 | 18.221 | 54.113 | 0.22x |
| users.json $[*].orders[*].total | strata | 4.039 | 4.129 | 5.901 | 54.113 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.839 | 18.786 | 21.523 | 54.113 | 0.22x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 38.339 | 41.102 | 52.557 | 54.113 | 0.10x |
| users.json $..total | strata | 13.052 | 13.514 | 13.948 | 54.113 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 355.158 | 358.778 | 385.418 | 54.113 | 0.04x |

