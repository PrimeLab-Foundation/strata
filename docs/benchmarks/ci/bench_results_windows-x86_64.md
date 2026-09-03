# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c3d92f5
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD
- compiler_flags: /std:c++20 /O2 /arch:AVX2 /GL /LTCG /USEPROFILE (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.885 | 12.357 | 14.255 | 45.512 | 1.00x |
| users.json | orjson | 14.600 | 15.619 | 23.632 | 45.512 | 0.79x |
| users.json | msgspec | 14.710 | 15.713 | 36.365 | 45.512 | 0.79x |
| users.json | ujson | 25.547 | 27.082 | 30.910 | 45.512 | 0.46x |
| users.json | json | 24.818 | 28.453 | 30.059 | 45.512 | 0.43x |
| flat.json | strata | 1.091 | 1.125 | 1.201 | 51.117 | 1.00x |
| flat.json | orjson | 1.110 | 1.128 | 1.284 | 51.117 | 1.00x |
| flat.json | msgspec | 1.118 | 1.130 | 2.983 | 51.117 | 1.00x |
| flat.json | ujson | 2.110 | 2.142 | 4.032 | 51.117 | 0.53x |
| flat.json | json | 1.942 | 1.958 | 3.273 | 51.117 | 0.57x |
| nested.json | strata | 0.805 | 0.826 | 1.371 | 51.207 | 1.00x |
| nested.json | orjson | 1.069 | 1.096 | 2.125 | 51.207 | 0.75x |
| nested.json | msgspec | 0.999 | 1.037 | 1.741 | 51.207 | 0.80x |
| nested.json | ujson | 1.595 | 1.647 | 3.191 | 51.207 | 0.50x |
| nested.json | json | 2.147 | 2.162 | 4.008 | 51.207 | 0.38x |
| wide_arrays.json | strata | 4.710 | 4.773 | 7.917 | 53.168 | 1.00x |
| wide_arrays.json | orjson | 5.645 | 5.754 | 9.062 | 53.168 | 0.83x |
| wide_arrays.json | msgspec | 5.865 | 5.945 | 7.771 | 53.168 | 0.80x |
| wide_arrays.json | ujson | 8.235 | 8.349 | 8.887 | 53.168 | 0.57x |
| wide_arrays.json | json | 11.600 | 11.893 | 20.390 | 53.168 | 0.40x |
| mixed.json | strata | 0.201 | 0.215 | 0.298 | 51.133 | 1.00x |
| mixed.json | orjson | 0.212 | 0.215 | 0.242 | 51.133 | 1.00x |
| mixed.json | msgspec | 0.237 | 0.238 | 0.302 | 51.133 | 0.90x |
| mixed.json | ujson | 0.368 | 0.372 | 0.420 | 51.133 | 0.58x |
| mixed.json | json | 0.471 | 0.483 | 0.652 | 51.133 | 0.45x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.254 | 3.850 | 4.743 | 44.719 | 1.00x |
| users.json | orjson | 3.915 | 4.580 | 6.462 | 44.719 | 0.84x |
| users.json | msgspec | 5.866 | 6.650 | 7.910 | 44.719 | 0.58x |
| users.json | ujson | 20.288 | 21.725 | 47.744 | 44.719 | 0.18x |
| users.json | json | 27.532 | 29.723 | 59.698 | 44.719 | 0.13x |
| flat.json | strata | 0.309 | 0.333 | 0.660 | 51.719 | 1.00x |
| flat.json | orjson | 0.353 | 0.378 | 0.531 | 51.719 | 0.88x |
| flat.json | msgspec | 0.489 | 0.520 | 0.749 | 51.719 | 0.64x |
| flat.json | ujson | 1.810 | 1.830 | 2.675 | 51.719 | 0.18x |
| flat.json | json | 1.881 | 1.974 | 3.525 | 51.719 | 0.17x |
| nested.json | strata | 0.287 | 0.290 | 0.326 | 51.625 | 1.00x |
| nested.json | orjson | 0.323 | 0.348 | 0.505 | 51.625 | 0.84x |
| nested.json | msgspec | 0.481 | 0.488 | 0.881 | 51.625 | 0.60x |
| nested.json | ujson | 1.450 | 1.496 | 1.577 | 51.625 | 0.19x |
| nested.json | json | 2.475 | 2.537 | 3.192 | 51.625 | 0.11x |
| wide_arrays.json | strata | 2.254 | 2.273 | 3.219 | 52.090 | 1.00x |
| wide_arrays.json | orjson | 2.070 | 2.299 | 2.682 | 52.090 | 0.99x |
| wide_arrays.json | msgspec | 3.348 | 3.727 | 3.786 | 52.090 | 0.61x |
| wide_arrays.json | ujson | 10.730 | 10.822 | 11.972 | 52.090 | 0.21x |
| wide_arrays.json | json | 18.029 | 18.475 | 32.923 | 52.090 | 0.12x |
| mixed.json | strata | 0.076 | 0.078 | 0.098 | 51.195 | 1.00x |
| mixed.json | orjson | 0.068 | 0.069 | 0.092 | 51.195 | 1.13x |
| mixed.json | msgspec | 0.092 | 0.094 | 0.119 | 51.195 | 0.83x |
| mixed.json | ujson | 0.339 | 0.346 | 0.384 | 51.195 | 0.22x |
| mixed.json | json | 0.509 | 0.526 | 0.563 | 51.195 | 0.15x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.433 | 12.929 | 17.913 | 54.746 | 1.00x |
| users.json | orjson | 15.226 | 17.043 | 18.608 | 54.746 | 0.76x |
| users.json | msgspec | 15.744 | 17.970 | 31.533 | 54.746 | 0.72x |
| users.json | ujson | 29.122 | 31.407 | 67.795 | 54.746 | 0.41x |
| users.json | json | 26.270 | 28.476 | 40.472 | 54.746 | 0.45x |
| flat.json | strata | 1.058 | 1.221 | 1.318 | 51.566 | 1.00x |
| flat.json | orjson | 1.296 | 1.453 | 1.542 | 51.566 | 0.84x |
| flat.json | msgspec | 1.217 | 1.301 | 1.399 | 51.566 | 0.94x |
| flat.json | ujson | 2.633 | 2.753 | 2.846 | 51.566 | 0.44x |
| flat.json | json | 2.082 | 2.102 | 2.249 | 51.566 | 0.58x |
| nested.json | strata | 0.884 | 0.925 | 1.005 | 51.359 | 1.00x |
| nested.json | orjson | 1.183 | 1.229 | 1.285 | 51.359 | 0.75x |
| nested.json | msgspec | 1.111 | 1.161 | 1.187 | 51.359 | 0.80x |
| nested.json | ujson | 2.001 | 2.043 | 2.364 | 51.359 | 0.45x |
| nested.json | json | 2.273 | 2.313 | 2.525 | 51.359 | 0.40x |
| wide_arrays.json | strata | 5.163 | 5.234 | 6.833 | 52.090 | 1.00x |
| wide_arrays.json | orjson | 5.993 | 6.057 | 6.337 | 52.090 | 0.86x |
| wide_arrays.json | msgspec | 6.249 | 6.351 | 6.803 | 52.090 | 0.82x |
| wide_arrays.json | ujson | 11.239 | 11.449 | 16.838 | 52.090 | 0.46x |
| wide_arrays.json | json | 12.015 | 12.214 | 21.958 | 52.090 | 0.43x |
| mixed.json | strata | 0.274 | 0.290 | 0.430 | 51.352 | 1.00x |
| mixed.json | orjson | 0.324 | 0.339 | 0.487 | 51.352 | 0.86x |
| mixed.json | msgspec | 0.347 | 0.353 | 0.802 | 51.352 | 0.82x |
| mixed.json | ujson | 0.547 | 0.584 | 0.808 | 51.352 | 0.50x |
| mixed.json | json | 0.582 | 0.643 | 1.007 | 51.352 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.853 | 11.389 | 18.363 | 52.195 | 1.00x |
| users.ndjson | orjson | 16.912 | 17.550 | 25.720 | 52.195 | 0.65x |
| users.ndjson | msgspec | 16.783 | 17.270 | 25.532 | 52.195 | 0.66x |
| users.ndjson | ujson | 25.026 | 26.830 | 35.336 | 52.195 | 0.42x |
| users.ndjson | json | 29.479 | 30.998 | 32.468 | 52.195 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.144 | 4.579 | 9.352 | 53.316 | 1.00x |
| users.json | orjson | 4.444 | 5.132 | 10.319 | 53.316 | 0.89x |
| users.json | msgspec | 6.133 | 7.005 | 20.272 | 53.316 | 0.65x |
| users.json | ujson | 28.477 | 31.921 | 38.043 | 53.316 | 0.14x |
| users.json | json | 35.939 | 36.850 | 73.474 | 53.316 | 0.12x |
| flat.json | strata | 0.663 | 0.714 | 0.740 | 51.570 | 1.00x |
| flat.json | orjson | 0.700 | 0.758 | 0.809 | 51.570 | 0.94x |
| flat.json | msgspec | 0.847 | 0.924 | 0.996 | 51.570 | 0.77x |
| flat.json | ujson | 3.151 | 3.223 | 3.379 | 51.570 | 0.22x |
| flat.json | json | 3.305 | 3.524 | 3.623 | 51.570 | 0.20x |
| nested.json | strata | 0.639 | 0.662 | 0.876 | 51.691 | 1.00x |
| nested.json | orjson | 0.674 | 0.719 | 0.925 | 51.691 | 0.92x |
| nested.json | msgspec | 0.849 | 0.873 | 1.210 | 51.691 | 0.76x |
| nested.json | ujson | 2.552 | 2.575 | 2.609 | 51.691 | 0.26x |
| nested.json | json | 3.537 | 3.622 | 3.737 | 51.691 | 0.18x |
| wide_arrays.json | strata | 2.895 | 2.961 | 37.086 | 52.090 | 1.00x |
| wide_arrays.json | orjson | 2.743 | 2.942 | 3.214 | 52.090 | 1.01x |
| wide_arrays.json | msgspec | 3.989 | 4.348 | 4.444 | 52.090 | 0.68x |
| wide_arrays.json | ujson | 17.466 | 17.630 | 26.458 | 52.090 | 0.17x |
| wide_arrays.json | json | 24.606 | 25.107 | 33.204 | 52.090 | 0.12x |
| mixed.json | strata | 0.384 | 0.394 | 0.486 | 51.371 | 1.00x |
| mixed.json | orjson | 0.376 | 0.385 | 0.443 | 51.371 | 1.02x |
| mixed.json | msgspec | 0.402 | 0.421 | 0.583 | 51.371 | 0.93x |
| mixed.json | ujson | 0.820 | 0.836 | 0.862 | 51.371 | 0.47x |
| mixed.json | json | 0.985 | 1.033 | 1.055 | 51.371 | 0.38x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.091 | 0.121 | 2.206 | 53.328 | 1.00x |
| users.json $[*].id | jmespath | 0.437 | 0.448 | 0.992 | 53.328 | 0.27x |
| users.json $[*].id | jsonpath-ng | 2.606 | 3.065 | 5.202 | 53.328 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.479 | 0.541 | 0.941 | 53.336 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.788 | 2.890 | 3.843 | 53.336 | 0.19x |
| users.json $[*].orders[*].total | jsonpath-ng | 18.214 | 19.375 | 22.877 | 53.336 | 0.03x |
| users.json $..total | strata | 1.815 | 1.895 | 2.591 | 54.340 | 1.00x |
| users.json $..total | jsonpath-ng | 336.812 | 362.614 | 416.097 | 54.340 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.969 | 4.346 | 5.256 | 53.336 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.717 | 16.539 | 22.269 | 53.336 | 0.26x |
| users.json $[*].id | orjson+jsonpath-ng | 18.217 | 18.692 | 29.907 | 53.336 | 0.23x |
| users.json $[*].orders[*].total | strata | 4.320 | 4.518 | 5.904 | 54.336 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 19.065 | 20.288 | 23.805 | 54.336 | 0.22x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 37.691 | 41.559 | 65.173 | 54.336 | 0.11x |
| users.json $..total | strata | 14.024 | 14.719 | 24.352 | 54.344 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 351.610 | 382.641 | 422.619 | 54.344 | 0.04x |

