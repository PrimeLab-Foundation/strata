# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: a969975
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
| users.json | strata | 11.114 | 13.400 | 28.795 | 45.473 | 1.00x |
| users.json | orjson | 16.945 | 19.403 | 99.830 | 45.473 | 0.69x |
| users.json | msgspec | 14.420 | 16.752 | 32.317 | 45.473 | 0.80x |
| users.json | ujson | 23.491 | 29.493 | 56.203 | 45.473 | 0.45x |
| users.json | json | 25.670 | 27.954 | 54.711 | 45.473 | 0.48x |
| flat.json | strata | 1.149 | 1.274 | 2.075 | 49.617 | 1.00x |
| flat.json | orjson | 1.188 | 1.281 | 1.613 | 49.617 | 1.00x |
| flat.json | msgspec | 1.117 | 1.430 | 2.167 | 49.617 | 0.89x |
| flat.json | ujson | 2.359 | 2.488 | 3.603 | 49.617 | 0.51x |
| flat.json | json | 2.036 | 2.253 | 4.206 | 49.617 | 0.57x |
| nested.json | strata | 0.802 | 1.141 | 1.603 | 49.500 | 1.00x |
| nested.json | orjson | 1.174 | 1.279 | 2.192 | 49.500 | 0.89x |
| nested.json | msgspec | 1.011 | 1.111 | 2.020 | 49.500 | 1.03x |
| nested.json | ujson | 1.846 | 2.451 | 3.026 | 49.500 | 0.47x |
| nested.json | json | 2.128 | 2.236 | 3.098 | 49.500 | 0.51x |
| wide_arrays.json | strata | 4.914 | 5.250 | 8.375 | 51.469 | 1.00x |
| wide_arrays.json | orjson | 6.200 | 6.823 | 10.786 | 51.469 | 0.77x |
| wide_arrays.json | msgspec | 6.037 | 6.323 | 10.893 | 51.469 | 0.83x |
| wide_arrays.json | ujson | 8.688 | 9.204 | 10.202 | 51.469 | 0.57x |
| wide_arrays.json | json | 12.703 | 13.338 | 13.693 | 51.469 | 0.39x |
| mixed.json | strata | 0.198 | 0.202 | 0.231 | 48.359 | 1.00x |
| mixed.json | orjson | 0.216 | 0.221 | 0.246 | 48.359 | 0.91x |
| mixed.json | msgspec | 0.237 | 0.247 | 0.334 | 48.359 | 0.82x |
| mixed.json | ujson | 0.373 | 0.394 | 0.426 | 48.359 | 0.51x |
| mixed.json | json | 0.475 | 0.495 | 0.591 | 48.359 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.420 | 4.000 | 8.628 | 44.719 | 1.00x |
| users.json | orjson | 4.200 | 4.702 | 5.471 | 44.719 | 0.85x |
| users.json | msgspec | 6.519 | 6.747 | 7.476 | 44.719 | 0.59x |
| users.json | ujson | 18.840 | 19.934 | 21.222 | 44.719 | 0.20x |
| users.json | json | 26.386 | 29.493 | 30.786 | 44.719 | 0.14x |
| flat.json | strata | 0.345 | 0.424 | 0.514 | 50.090 | 1.00x |
| flat.json | orjson | 0.369 | 0.438 | 0.688 | 50.090 | 0.97x |
| flat.json | msgspec | 0.509 | 0.873 | 0.987 | 50.090 | 0.49x |
| flat.json | ujson | 1.776 | 1.850 | 2.559 | 50.090 | 0.23x |
| flat.json | json | 1.966 | 2.071 | 3.130 | 50.090 | 0.20x |
| nested.json | strata | 0.291 | 0.305 | 0.586 | 49.816 | 1.00x |
| nested.json | orjson | 0.332 | 0.353 | 0.649 | 49.816 | 0.86x |
| nested.json | msgspec | 0.477 | 0.492 | 0.523 | 49.816 | 0.62x |
| nested.json | ujson | 1.322 | 1.359 | 1.385 | 49.816 | 0.22x |
| nested.json | json | 2.469 | 2.508 | 4.857 | 49.816 | 0.12x |
| wide_arrays.json | strata | 2.405 | 2.442 | 4.395 | 51.086 | 1.00x |
| wide_arrays.json | orjson | 2.778 | 2.916 | 4.582 | 51.086 | 0.84x |
| wide_arrays.json | msgspec | 4.311 | 4.429 | 6.475 | 51.086 | 0.55x |
| wide_arrays.json | ujson | 10.055 | 10.763 | 13.688 | 51.086 | 0.23x |
| wide_arrays.json | json | 19.479 | 20.203 | 35.979 | 51.086 | 0.12x |
| mixed.json | strata | 0.080 | 0.084 | 0.094 | 48.562 | 1.00x |
| mixed.json | orjson | 0.071 | 0.074 | 0.092 | 48.562 | 1.14x |
| mixed.json | msgspec | 0.094 | 0.099 | 0.128 | 48.562 | 0.85x |
| mixed.json | ujson | 0.338 | 0.347 | 0.396 | 48.562 | 0.24x |
| mixed.json | json | 0.504 | 0.520 | 0.550 | 48.562 | 0.16x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 15.971 | 17.663 | 111.460 | 54.746 | 1.00x |
| users.json | orjson | 17.964 | 23.125 | 104.443 | 54.746 | 0.76x |
| users.json | msgspec | 17.117 | 20.256 | 81.998 | 54.746 | 0.87x |
| users.json | ujson | 36.074 | 44.949 | 172.553 | 54.746 | 0.39x |
| users.json | json | 30.862 | 46.051 | 130.388 | 54.746 | 0.38x |
| flat.json | strata | 1.457 | 1.779 | 2.364 | 49.270 | 1.00x |
| flat.json | orjson | 1.524 | 1.616 | 2.686 | 49.270 | 1.10x |
| flat.json | msgspec | 1.347 | 1.956 | 2.649 | 49.270 | 0.91x |
| flat.json | ujson | 2.940 | 3.301 | 5.412 | 49.270 | 0.54x |
| flat.json | json | 2.493 | 4.243 | 5.350 | 49.270 | 0.42x |
| nested.json | strata | 0.928 | 1.061 | 1.544 | 49.516 | 1.00x |
| nested.json | orjson | 1.369 | 1.478 | 2.386 | 49.516 | 0.72x |
| nested.json | msgspec | 1.215 | 1.307 | 2.190 | 49.516 | 0.81x |
| nested.json | ujson | 2.141 | 2.724 | 3.657 | 49.516 | 0.39x |
| nested.json | json | 2.269 | 2.681 | 4.117 | 49.516 | 0.40x |
| wide_arrays.json | strata | 5.602 | 6.137 | 10.262 | 51.086 | 1.00x |
| wide_arrays.json | orjson | 7.245 | 7.647 | 11.052 | 51.086 | 0.80x |
| wide_arrays.json | msgspec | 6.721 | 7.399 | 8.608 | 51.086 | 0.83x |
| wide_arrays.json | ujson | 12.552 | 13.016 | 19.033 | 51.086 | 0.47x |
| wide_arrays.json | json | 13.325 | 13.968 | 17.408 | 51.086 | 0.44x |
| mixed.json | strata | 0.292 | 0.309 | 0.425 | 48.379 | 1.00x |
| mixed.json | orjson | 0.338 | 0.402 | 0.503 | 48.379 | 0.77x |
| mixed.json | msgspec | 0.352 | 0.368 | 0.434 | 48.379 | 0.84x |
| mixed.json | ujson | 0.578 | 0.601 | 0.632 | 48.379 | 0.51x |
| mixed.json | json | 0.584 | 0.620 | 0.802 | 48.379 | 0.50x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 15.694 | 17.025 | 19.518 | 50.516 | 1.00x |
| users.ndjson | orjson | 22.305 | 25.157 | 43.084 | 50.516 | 0.68x |
| users.ndjson | msgspec | 23.705 | 26.220 | 54.511 | 50.516 | 0.65x |
| users.ndjson | ujson | 34.317 | 36.992 | 63.127 | 50.516 | 0.46x |
| users.ndjson | json | 38.948 | 42.247 | 44.538 | 50.516 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.443 | 5.127 | 12.682 | 51.812 | 1.00x |
| users.json | orjson | 5.107 | 6.356 | 15.689 | 51.812 | 0.81x |
| users.json | msgspec | 7.180 | 7.779 | 9.233 | 51.812 | 0.66x |
| users.json | ujson | 30.334 | 32.641 | 46.163 | 51.812 | 0.16x |
| users.json | json | 38.638 | 41.748 | 63.195 | 51.812 | 0.12x |
| flat.json | strata | 0.800 | 0.931 | 11.096 | 49.996 | 1.00x |
| flat.json | orjson | 0.800 | 1.161 | 1.670 | 49.996 | 0.80x |
| flat.json | msgspec | 0.971 | 1.083 | 3.511 | 49.996 | 0.86x |
| flat.json | ujson | 3.300 | 4.081 | 7.498 | 49.996 | 0.23x |
| flat.json | json | 3.609 | 4.188 | 11.047 | 49.996 | 0.22x |
| nested.json | strata | 0.643 | 0.771 | 1.198 | 49.805 | 1.00x |
| nested.json | orjson | 0.735 | 0.795 | 1.002 | 49.805 | 0.97x |
| nested.json | msgspec | 0.823 | 0.923 | 1.169 | 49.805 | 0.84x |
| nested.json | ujson | 2.446 | 2.526 | 2.940 | 49.805 | 0.31x |
| nested.json | json | 3.600 | 3.680 | 3.980 | 49.805 | 0.21x |
| wide_arrays.json | strata | 3.234 | 3.796 | 5.432 | 50.086 | 1.00x |
| wide_arrays.json | orjson | 3.394 | 4.201 | 13.612 | 50.086 | 0.90x |
| wide_arrays.json | msgspec | 4.916 | 5.900 | 7.574 | 50.086 | 0.64x |
| wide_arrays.json | ujson | 17.536 | 20.167 | 29.788 | 50.086 | 0.19x |
| wide_arrays.json | json | 26.262 | 33.027 | 40.119 | 50.086 | 0.11x |
| mixed.json | strata | 0.396 | 0.431 | 0.636 | 48.562 | 1.00x |
| mixed.json | orjson | 0.390 | 0.416 | 0.556 | 48.562 | 1.03x |
| mixed.json | msgspec | 0.421 | 0.431 | 0.560 | 48.562 | 1.00x |
| mixed.json | ujson | 0.845 | 0.882 | 1.382 | 48.562 | 0.49x |
| mixed.json | json | 1.046 | 1.079 | 1.942 | 48.562 | 0.40x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.105 | 0.123 | 0.165 | 50.215 | 1.00x |
| users.json $[*].id | jmespath | 0.465 | 0.566 | 0.968 | 50.215 | 0.22x |
| users.json $[*].id | jsonpath-ng | 2.629 | 3.296 | 4.853 | 50.215 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.494 | 0.623 | 1.223 | 50.480 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.173 | 4.000 | 5.369 | 50.480 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 22.156 | 24.934 | 26.779 | 50.480 | 0.02x |
| users.json $..total | strata | 1.862 | 1.954 | 3.343 | 52.645 | 1.00x |
| users.json $..total | jsonpath-ng | 352.570 | 375.223 | 444.855 | 52.645 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.452 | 4.719 | 7.228 | 50.262 | 1.00x |
| users.json $[*].id | orjson+jmespath | 18.226 | 20.751 | 30.771 | 50.262 | 0.23x |
| users.json $[*].id | orjson+jsonpath-ng | 21.976 | 24.583 | 41.531 | 50.262 | 0.19x |
| users.json $[*].orders[*].total | strata | 4.648 | 5.491 | 12.026 | 52.246 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 19.921 | 26.188 | 53.103 | 52.246 | 0.21x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 46.089 | 51.479 | 74.109 | 52.246 | 0.11x |
| users.json $..total | strata | 18.389 | 21.850 | 41.787 | 52.270 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 461.617 | 498.708 | 575.702 | 52.270 | 0.04x |

