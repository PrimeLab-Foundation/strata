# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: b294ccd
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
| users.json | strata | 10.289 | 10.966 | 18.564 | 45.285 | 1.00x |
| users.json | orjson | 13.662 | 14.213 | 16.345 | 45.285 | 0.77x |
| users.json | msgspec | 13.006 | 13.367 | 15.056 | 45.285 | 0.82x |
| users.json | ujson | 21.415 | 23.071 | 49.927 | 45.285 | 0.48x |
| users.json | json | 22.759 | 23.990 | 33.841 | 45.285 | 0.46x |
| flat.json | strata | 1.103 | 1.144 | 1.245 | 48.930 | 1.00x |
| flat.json | orjson | 1.125 | 1.204 | 1.238 | 48.930 | 0.95x |
| flat.json | msgspec | 1.095 | 1.144 | 1.691 | 48.930 | 1.00x |
| flat.json | ujson | 2.278 | 2.365 | 2.521 | 48.930 | 0.48x |
| flat.json | json | 2.017 | 2.033 | 2.121 | 48.930 | 0.56x |
| nested.json | strata | 0.795 | 0.815 | 1.010 | 48.875 | 1.00x |
| nested.json | orjson | 1.097 | 1.250 | 1.422 | 48.875 | 0.65x |
| nested.json | msgspec | 1.012 | 1.083 | 1.227 | 48.875 | 0.75x |
| nested.json | ujson | 1.632 | 1.743 | 2.008 | 48.875 | 0.47x |
| nested.json | json | 2.144 | 2.218 | 2.522 | 48.875 | 0.37x |
| wide_arrays.json | strata | 4.864 | 5.461 | 8.309 | 50.723 | 1.00x |
| wide_arrays.json | orjson | 5.825 | 6.300 | 10.819 | 50.723 | 0.87x |
| wide_arrays.json | msgspec | 6.000 | 6.393 | 10.574 | 50.723 | 0.85x |
| wide_arrays.json | ujson | 8.681 | 9.140 | 11.252 | 50.723 | 0.60x |
| wide_arrays.json | json | 12.297 | 13.121 | 14.164 | 50.723 | 0.42x |
| mixed.json | strata | 0.208 | 0.365 | 0.413 | 48.605 | 1.00x |
| mixed.json | orjson | 0.239 | 0.417 | 0.513 | 48.605 | 0.87x |
| mixed.json | msgspec | 0.248 | 0.483 | 0.575 | 48.605 | 0.75x |
| mixed.json | ujson | 0.404 | 0.682 | 0.914 | 48.605 | 0.54x |
| mixed.json | json | 0.486 | 0.946 | 1.823 | 48.605 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.246 | 3.434 | 4.212 | 44.590 | 1.00x |
| users.json | orjson | 3.785 | 4.167 | 6.070 | 44.590 | 0.82x |
| users.json | msgspec | 5.505 | 5.815 | 10.003 | 44.590 | 0.59x |
| users.json | ujson | 17.373 | 17.962 | 26.684 | 44.590 | 0.19x |
| users.json | json | 24.043 | 25.002 | 44.693 | 44.590 | 0.14x |
| flat.json | strata | 0.319 | 0.387 | 0.614 | 49.477 | 1.00x |
| flat.json | orjson | 0.396 | 0.614 | 0.664 | 49.477 | 0.63x |
| flat.json | msgspec | 0.540 | 0.842 | 4.085 | 49.477 | 0.46x |
| flat.json | ujson | 1.939 | 2.698 | 5.905 | 49.477 | 0.14x |
| flat.json | json | 1.976 | 3.265 | 4.211 | 49.477 | 0.12x |
| nested.json | strata | 0.292 | 0.316 | 0.343 | 49.402 | 1.00x |
| nested.json | orjson | 0.332 | 0.339 | 0.389 | 49.402 | 0.93x |
| nested.json | msgspec | 0.480 | 0.487 | 0.593 | 49.402 | 0.65x |
| nested.json | ujson | 1.371 | 1.445 | 1.649 | 49.402 | 0.22x |
| nested.json | json | 2.453 | 2.528 | 2.822 | 49.402 | 0.13x |
| wide_arrays.json | strata | 2.245 | 2.575 | 3.556 | 50.328 | 1.00x |
| wide_arrays.json | orjson | 2.598 | 3.249 | 3.920 | 50.328 | 0.79x |
| wide_arrays.json | msgspec | 4.136 | 4.729 | 6.769 | 50.328 | 0.54x |
| wide_arrays.json | ujson | 10.126 | 11.082 | 15.987 | 50.328 | 0.23x |
| wide_arrays.json | json | 19.134 | 23.179 | 80.104 | 50.328 | 0.11x |
| mixed.json | strata | 0.079 | 0.086 | 0.123 | 48.762 | 1.00x |
| mixed.json | orjson | 0.073 | 0.111 | 0.185 | 48.762 | 0.78x |
| mixed.json | msgspec | 0.100 | 0.102 | 0.157 | 48.762 | 0.85x |
| mixed.json | ujson | 0.335 | 0.355 | 0.603 | 48.762 | 0.24x |
| mixed.json | json | 0.522 | 0.574 | 0.998 | 48.762 | 0.15x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.934 | 12.661 | 13.451 | 54.617 | 1.00x |
| users.json | orjson | 14.271 | 15.122 | 26.100 | 54.617 | 0.84x |
| users.json | msgspec | 14.104 | 15.591 | 28.673 | 54.617 | 0.81x |
| users.json | ujson | 27.669 | 28.926 | 43.162 | 54.617 | 0.44x |
| users.json | json | 24.215 | 25.025 | 39.906 | 54.617 | 0.51x |
| flat.json | strata | 1.239 | 1.859 | 2.221 | 49.059 | 1.00x |
| flat.json | orjson | 1.293 | 2.003 | 2.234 | 49.059 | 0.93x |
| flat.json | msgspec | 1.353 | 2.014 | 2.400 | 49.059 | 0.92x |
| flat.json | ujson | 2.762 | 4.029 | 4.346 | 49.059 | 0.46x |
| flat.json | json | 2.091 | 3.326 | 3.722 | 49.059 | 0.56x |
| nested.json | strata | 0.901 | 1.041 | 1.647 | 49.402 | 1.00x |
| nested.json | orjson | 1.237 | 1.336 | 2.233 | 49.402 | 0.78x |
| nested.json | msgspec | 1.149 | 1.266 | 1.406 | 49.402 | 0.82x |
| nested.json | ujson | 2.070 | 2.289 | 2.467 | 49.402 | 0.45x |
| nested.json | json | 2.297 | 2.432 | 2.971 | 49.402 | 0.43x |
| wide_arrays.json | strata | 5.632 | 5.931 | 9.507 | 50.328 | 1.00x |
| wide_arrays.json | orjson | 6.387 | 6.860 | 10.969 | 50.328 | 0.86x |
| wide_arrays.json | msgspec | 6.881 | 7.318 | 10.063 | 50.328 | 0.81x |
| wide_arrays.json | ujson | 11.702 | 13.004 | 20.337 | 50.328 | 0.46x |
| wide_arrays.json | json | 13.090 | 13.442 | 16.254 | 50.328 | 0.44x |
| mixed.json | strata | 0.288 | 0.299 | 0.337 | 48.770 | 1.00x |
| mixed.json | orjson | 0.351 | 0.399 | 0.486 | 48.770 | 0.75x |
| mixed.json | msgspec | 0.365 | 0.409 | 0.447 | 48.770 | 0.73x |
| mixed.json | ujson | 0.585 | 0.649 | 0.804 | 48.770 | 0.46x |
| mixed.json | json | 0.592 | 0.611 | 1.326 | 48.770 | 0.49x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 13.578 | 14.275 | 23.373 | 50.176 | 1.00x |
| users.ndjson | orjson | 20.566 | 21.205 | 24.318 | 50.176 | 0.67x |
| users.ndjson | msgspec | 19.719 | 21.319 | 23.098 | 50.176 | 0.67x |
| users.ndjson | ujson | 28.689 | 30.372 | 33.598 | 50.176 | 0.47x |
| users.ndjson | json | 31.943 | 34.559 | 50.646 | 50.176 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.071 | 4.277 | 6.219 | 50.734 | 1.00x |
| users.json | orjson | 4.474 | 4.850 | 10.153 | 50.734 | 0.88x |
| users.json | msgspec | 6.420 | 6.566 | 10.310 | 50.734 | 0.65x |
| users.json | ujson | 26.513 | 27.367 | 30.946 | 50.734 | 0.16x |
| users.json | json | 33.038 | 33.975 | 51.074 | 50.734 | 0.13x |
| flat.json | strata | 0.728 | 0.902 | 1.247 | 49.367 | 1.00x |
| flat.json | orjson | 0.769 | 0.816 | 1.181 | 49.367 | 1.11x |
| flat.json | msgspec | 0.896 | 0.998 | 1.416 | 49.367 | 0.90x |
| flat.json | ujson | 3.232 | 3.328 | 5.796 | 49.367 | 0.27x |
| flat.json | json | 3.524 | 3.796 | 7.098 | 49.367 | 0.24x |
| nested.json | strata | 0.662 | 0.716 | 1.032 | 49.402 | 1.00x |
| nested.json | orjson | 0.689 | 0.774 | 0.863 | 49.402 | 0.93x |
| nested.json | msgspec | 0.851 | 0.968 | 1.106 | 49.402 | 0.74x |
| nested.json | ujson | 2.467 | 2.693 | 3.055 | 49.402 | 0.27x |
| nested.json | json | 3.526 | 3.708 | 3.931 | 49.402 | 0.19x |
| wide_arrays.json | strata | 3.208 | 3.310 | 4.864 | 50.328 | 1.00x |
| wide_arrays.json | orjson | 3.447 | 3.792 | 17.574 | 50.328 | 0.87x |
| wide_arrays.json | msgspec | 4.839 | 5.094 | 6.815 | 50.328 | 0.65x |
| wide_arrays.json | ujson | 17.014 | 18.020 | 34.322 | 50.328 | 0.18x |
| wide_arrays.json | json | 26.610 | 29.715 | 45.329 | 50.328 | 0.11x |
| mixed.json | strata | 0.415 | 0.446 | 0.662 | 48.941 | 1.00x |
| mixed.json | orjson | 0.392 | 0.426 | 0.553 | 48.941 | 1.05x |
| mixed.json | msgspec | 0.422 | 0.454 | 0.619 | 48.941 | 0.98x |
| mixed.json | ujson | 0.836 | 0.898 | 1.791 | 48.941 | 0.50x |
| mixed.json | json | 1.017 | 1.143 | 1.692 | 48.941 | 0.39x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.087 | 0.094 | 0.149 | 50.746 | 1.00x |
| users.json $[*].id | jmespath | 0.447 | 0.498 | 0.622 | 50.746 | 0.19x |
| users.json $[*].id | jsonpath-ng | 2.510 | 2.670 | 3.036 | 50.746 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.462 | 0.522 | 0.657 | 50.750 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.800 | 3.024 | 3.397 | 50.750 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 18.000 | 19.824 | 32.367 | 50.750 | 0.03x |
| users.json $..total | strata | 1.830 | 2.048 | 3.795 | 52.379 | 1.00x |
| users.json $..total | jsonpath-ng | 356.082 | 379.049 | 397.319 | 52.379 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.045 | 4.231 | 6.012 | 50.750 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.074 | 15.849 | 27.478 | 50.750 | 0.27x |
| users.json $[*].id | orjson+jsonpath-ng | 17.033 | 18.150 | 19.349 | 50.750 | 0.23x |
| users.json $[*].orders[*].total | strata | 4.256 | 4.374 | 4.668 | 52.375 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.177 | 19.301 | 32.027 | 52.375 | 0.23x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 35.643 | 38.291 | 61.318 | 52.375 | 0.11x |
| users.json $..total | strata | 15.936 | 17.058 | 19.504 | 52.379 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 375.139 | 402.029 | 413.481 | 52.379 | 0.04x |

