# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 6309d05
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 25 Model 17 Stepping 1, AuthenticAMD
- compiler_flags: /std:c++20 /O2 /arch:AVX2 /GL /LTCG /USEPROFILE (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 12.902 | 13.616 | 20.405 | 45.457 | 1.00x |
| users.json | orjson | 15.962 | 16.765 | 20.862 | 45.457 | 0.81x |
| users.json | msgspec | 15.280 | 15.657 | 17.853 | 45.457 | 0.87x |
| users.json | ujson | 23.751 | 25.613 | 41.078 | 45.457 | 0.53x |
| users.json | json | 27.217 | 27.728 | 43.916 | 45.457 | 0.49x |
| flat.json | strata | 1.362 | 1.401 | 1.998 | 50.359 | 1.00x |
| flat.json | orjson | 1.258 | 1.310 | 2.013 | 50.359 | 1.07x |
| flat.json | msgspec | 1.312 | 1.363 | 2.095 | 50.359 | 1.03x |
| flat.json | ujson | 2.410 | 2.492 | 3.896 | 50.359 | 0.56x |
| flat.json | json | 2.224 | 2.323 | 3.529 | 50.359 | 0.60x |
| nested.json | strata | 1.037 | 1.072 | 1.196 | 50.180 | 1.00x |
| nested.json | orjson | 1.233 | 1.264 | 1.298 | 50.180 | 0.85x |
| nested.json | msgspec | 1.148 | 1.177 | 1.225 | 50.180 | 0.91x |
| nested.json | ujson | 1.833 | 1.872 | 2.229 | 50.180 | 0.57x |
| nested.json | json | 2.496 | 2.577 | 2.645 | 50.180 | 0.42x |
| wide_arrays.json | strata | 6.233 | 6.490 | 11.815 | 52.438 | 1.00x |
| wide_arrays.json | orjson | 6.787 | 7.099 | 10.525 | 52.438 | 0.91x |
| wide_arrays.json | msgspec | 7.271 | 7.407 | 11.390 | 52.438 | 0.88x |
| wide_arrays.json | ujson | 9.435 | 9.646 | 10.713 | 52.438 | 0.67x |
| wide_arrays.json | json | 13.938 | 14.347 | 21.001 | 52.438 | 0.45x |
| mixed.json | strata | 0.244 | 0.247 | 0.266 | 50.211 | 1.00x |
| mixed.json | orjson | 0.240 | 0.254 | 0.316 | 50.211 | 0.97x |
| mixed.json | msgspec | 0.281 | 0.290 | 0.321 | 50.211 | 0.85x |
| mixed.json | ujson | 0.390 | 0.407 | 0.440 | 50.211 | 0.61x |
| mixed.json | json | 0.554 | 0.566 | 0.838 | 50.211 | 0.44x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.779 | 3.871 | 3.943 | 44.621 | 1.00x |
| users.json | orjson | 3.990 | 4.064 | 4.215 | 44.621 | 0.95x |
| users.json | msgspec | 6.169 | 6.347 | 6.661 | 44.621 | 0.61x |
| users.json | ujson | 18.506 | 18.990 | 26.965 | 44.621 | 0.20x |
| users.json | json | 29.410 | 30.027 | 43.674 | 44.621 | 0.13x |
| flat.json | strata | 0.409 | 0.429 | 0.458 | 50.758 | 1.00x |
| flat.json | orjson | 0.376 | 0.388 | 0.483 | 50.758 | 1.11x |
| flat.json | msgspec | 0.539 | 0.558 | 0.622 | 50.758 | 0.77x |
| flat.json | ujson | 1.705 | 1.739 | 2.044 | 50.758 | 0.25x |
| flat.json | json | 2.369 | 2.429 | 2.599 | 50.758 | 0.18x |
| nested.json | strata | 0.298 | 0.304 | 0.457 | 50.688 | 1.00x |
| nested.json | orjson | 0.324 | 0.344 | 0.519 | 50.688 | 0.88x |
| nested.json | msgspec | 0.529 | 0.547 | 0.875 | 50.688 | 0.56x |
| nested.json | ujson | 1.493 | 1.545 | 2.501 | 50.688 | 0.20x |
| nested.json | json | 3.133 | 3.284 | 5.202 | 50.688 | 0.09x |
| wide_arrays.json | strata | 2.684 | 3.738 | 26.302 | 51.965 | 1.00x |
| wide_arrays.json | orjson | 3.154 | 4.346 | 29.508 | 51.965 | 0.86x |
| wide_arrays.json | msgspec | 4.440 | 5.989 | 23.407 | 51.965 | 0.62x |
| wide_arrays.json | ujson | 11.410 | 17.190 | 28.448 | 51.965 | 0.22x |
| wide_arrays.json | json | 30.703 | 36.757 | 91.432 | 51.965 | 0.10x |
| mixed.json | strata | 0.084 | 0.090 | 0.181 | 50.316 | 1.00x |
| mixed.json | orjson | 0.071 | 0.076 | 0.106 | 50.316 | 1.18x |
| mixed.json | msgspec | 0.100 | 0.104 | 0.149 | 50.316 | 0.86x |
| mixed.json | ujson | 0.363 | 0.391 | 0.573 | 50.316 | 0.23x |
| mixed.json | json | 0.634 | 0.648 | 1.076 | 50.316 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 15.719 | 16.229 | 17.099 | 54.000 | 1.00x |
| users.json | orjson | 16.923 | 17.574 | 18.542 | 54.000 | 0.92x |
| users.json | msgspec | 16.460 | 17.703 | 33.995 | 54.000 | 0.92x |
| users.json | ujson | 28.695 | 30.637 | 41.960 | 54.000 | 0.53x |
| users.json | json | 28.239 | 28.965 | 46.771 | 54.000 | 0.56x |
| flat.json | strata | 1.530 | 1.659 | 1.831 | 50.547 | 1.00x |
| flat.json | orjson | 1.486 | 1.649 | 1.743 | 50.547 | 1.01x |
| flat.json | msgspec | 1.412 | 1.473 | 1.532 | 50.547 | 1.13x |
| flat.json | ujson | 2.589 | 2.971 | 3.077 | 50.547 | 0.56x |
| flat.json | json | 2.541 | 2.578 | 2.831 | 50.547 | 0.64x |
| nested.json | strata | 1.221 | 1.262 | 1.367 | 50.371 | 1.00x |
| nested.json | orjson | 1.404 | 1.426 | 1.559 | 50.371 | 0.89x |
| nested.json | msgspec | 1.282 | 1.370 | 1.515 | 50.371 | 0.92x |
| nested.json | ujson | 2.123 | 2.160 | 2.215 | 50.371 | 0.58x |
| nested.json | json | 2.645 | 2.702 | 2.873 | 50.371 | 0.47x |
| wide_arrays.json | strata | 8.649 | 12.445 | 28.536 | 51.289 | 1.00x |
| wide_arrays.json | orjson | 7.870 | 14.162 | 54.879 | 51.289 | 0.88x |
| wide_arrays.json | msgspec | 8.358 | 11.292 | 20.370 | 51.289 | 1.10x |
| wide_arrays.json | ujson | 13.027 | 19.547 | 49.176 | 51.289 | 0.64x |
| wide_arrays.json | json | 15.394 | 21.794 | 294.337 | 51.289 | 0.57x |
| mixed.json | strata | 0.333 | 0.356 | 0.416 | 50.238 | 1.00x |
| mixed.json | orjson | 0.379 | 0.432 | 0.522 | 50.238 | 0.82x |
| mixed.json | msgspec | 0.403 | 0.448 | 0.495 | 50.238 | 0.79x |
| mixed.json | ujson | 0.565 | 0.604 | 0.678 | 50.238 | 0.59x |
| mixed.json | json | 0.681 | 0.707 | 1.015 | 50.238 | 0.50x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 17.509 | 17.852 | 25.186 | 51.188 | 1.00x |
| users.ndjson | orjson | 20.815 | 21.721 | 22.668 | 51.188 | 0.82x |
| users.ndjson | msgspec | 21.273 | 22.168 | 31.363 | 51.188 | 0.81x |
| users.ndjson | ujson | 30.634 | 31.995 | 51.943 | 51.188 | 0.56x |
| users.ndjson | json | 37.319 | 38.163 | 57.442 | 51.188 | 0.47x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.919 | 5.175 | 7.420 | 52.238 | 1.00x |
| users.json | orjson | 5.163 | 5.617 | 11.582 | 52.238 | 0.92x |
| users.json | msgspec | 6.755 | 7.039 | 9.883 | 52.238 | 0.74x |
| users.json | ujson | 27.450 | 27.796 | 67.161 | 52.238 | 0.19x |
| users.json | json | 38.755 | 39.159 | 66.502 | 52.238 | 0.13x |
| flat.json | strata | 0.798 | 0.834 | 1.246 | 51.020 | 1.00x |
| flat.json | orjson | 0.787 | 0.840 | 1.136 | 51.020 | 0.99x |
| flat.json | msgspec | 0.932 | 1.009 | 1.274 | 51.020 | 0.83x |
| flat.json | ujson | 3.140 | 3.249 | 5.553 | 51.020 | 0.26x |
| flat.json | json | 3.835 | 4.014 | 6.351 | 51.020 | 0.21x |
| nested.json | strata | 0.708 | 0.764 | 0.914 | 50.977 | 1.00x |
| nested.json | orjson | 0.743 | 0.792 | 1.075 | 50.977 | 0.96x |
| nested.json | msgspec | 0.925 | 0.989 | 1.304 | 50.977 | 0.77x |
| nested.json | ujson | 2.626 | 2.676 | 4.631 | 50.977 | 0.29x |
| nested.json | json | 4.268 | 4.372 | 6.974 | 50.977 | 0.17x |
| wide_arrays.json | strata | 3.340 | 3.450 | 4.063 | 54.172 | 1.00x |
| wide_arrays.json | orjson | 3.052 | 3.097 | 3.441 | 54.172 | 1.11x |
| wide_arrays.json | msgspec | 4.550 | 4.683 | 5.094 | 54.172 | 0.74x |
| wide_arrays.json | ujson | 17.670 | 18.183 | 33.163 | 54.172 | 0.19x |
| wide_arrays.json | json | 29.252 | 29.506 | 33.999 | 54.172 | 0.12x |
| mixed.json | strata | 0.401 | 0.422 | 0.457 | 50.305 | 1.00x |
| mixed.json | orjson | 0.400 | 0.418 | 0.472 | 50.305 | 1.01x |
| mixed.json | msgspec | 0.425 | 0.466 | 0.580 | 50.305 | 0.91x |
| mixed.json | ujson | 0.852 | 0.870 | 0.900 | 50.305 | 0.49x |
| mixed.json | json | 1.142 | 1.201 | 1.253 | 50.305 | 0.35x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.102 | 0.112 | 0.165 | 52.469 | 1.00x |
| users.json $[*].id | jmespath | 0.532 | 0.541 | 0.903 | 52.469 | 0.21x |
| users.json $[*].id | jsonpath-ng | 3.091 | 3.153 | 3.659 | 52.469 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.595 | 0.634 | 0.713 | 52.480 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.461 | 3.526 | 4.229 | 52.480 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 20.713 | 21.456 | 33.140 | 52.480 | 0.03x |
| users.json $..total | strata | 2.103 | 2.206 | 3.686 | 52.438 | 1.00x |
| users.json $..total | jsonpath-ng | 379.568 | 397.530 | 413.674 | 52.438 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 5.762 | 5.817 | 8.246 | 52.281 | 1.00x |
| users.json $[*].id | orjson+jmespath | 18.373 | 18.598 | 25.503 | 52.281 | 0.31x |
| users.json $[*].id | orjson+jsonpath-ng | 20.813 | 21.518 | 32.492 | 52.281 | 0.27x |
| users.json $[*].orders[*].total | strata | 6.065 | 6.143 | 6.303 | 52.363 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 22.641 | 23.014 | 34.522 | 52.363 | 0.27x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 45.405 | 46.663 | 58.089 | 52.363 | 0.13x |
| users.json $..total | strata | 19.445 | 20.732 | 30.167 | 52.398 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 404.829 | 425.202 | 437.344 | 52.398 | 0.05x |

