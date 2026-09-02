# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 8cdb525
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: x86_64
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.107 | 11.306 | 28.718 | 59.566 | 1.00x |
| users.json | orjson | 13.511 | 13.994 | 29.354 | 59.566 | 0.81x |
| users.json | msgspec | 13.127 | 13.841 | 30.719 | 59.566 | 0.82x |
| users.json | ujson | 19.345 | 20.004 | 43.484 | 59.566 | 0.57x |
| users.json | pysimdjson | 19.595 | 20.993 | 26.341 | 59.566 | 0.54x |
| users.json | json | 22.990 | 23.420 | 25.986 | 59.566 | 0.48x |
| flat.json | strata | 0.924 | 0.949 | 1.008 | 68.477 | 1.00x |
| flat.json | orjson | 1.000 | 1.020 | 1.223 | 68.477 | 0.93x |
| flat.json | msgspec | 1.020 | 1.050 | 1.393 | 68.477 | 0.90x |
| flat.json | ujson | 1.502 | 1.650 | 1.976 | 68.477 | 0.58x |
| flat.json | pysimdjson | 1.589 | 1.611 | 1.892 | 68.477 | 0.59x |
| flat.json | json | 1.920 | 1.933 | 2.019 | 68.477 | 0.49x |
| nested.json | strata | 0.855 | 0.875 | 0.964 | 68.477 | 1.00x |
| nested.json | orjson | 1.008 | 1.022 | 1.174 | 68.477 | 0.86x |
| nested.json | msgspec | 1.020 | 1.033 | 1.142 | 68.477 | 0.85x |
| nested.json | ujson | 1.514 | 1.635 | 1.859 | 68.477 | 0.54x |
| nested.json | pysimdjson | 1.433 | 1.497 | 2.236 | 68.477 | 0.58x |
| nested.json | json | 2.054 | 2.072 | 2.301 | 68.477 | 0.42x |
| wide_arrays.json | strata | 4.850 | 4.943 | 5.066 | 72.480 | 1.00x |
| wide_arrays.json | orjson | 5.089 | 5.368 | 5.787 | 72.480 | 0.92x |
| wide_arrays.json | msgspec | 5.628 | 5.872 | 6.011 | 72.480 | 0.84x |
| wide_arrays.json | ujson | 7.278 | 7.515 | 8.126 | 72.480 | 0.66x |
| wide_arrays.json | pysimdjson | 6.181 | 6.520 | 7.110 | 72.480 | 0.76x |
| wide_arrays.json | json | 9.897 | 10.131 | 11.194 | 72.480 | 0.49x |
| mixed.json | strata | 0.204 | 0.209 | 0.222 | 72.543 | 1.00x |
| mixed.json | orjson | 0.232 | 0.236 | 0.253 | 72.543 | 0.88x |
| mixed.json | msgspec | 0.241 | 0.246 | 0.288 | 72.543 | 0.85x |
| mixed.json | ujson | 0.306 | 0.320 | 0.361 | 72.543 | 0.65x |
| mixed.json | pysimdjson | 0.299 | 0.303 | 0.318 | 72.543 | 0.69x |
| mixed.json | json | 0.477 | 0.492 | 0.507 | 72.543 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.577 | 2.697 | 2.919 | 58.680 | 1.00x |
| users.json | orjson | 2.948 | 3.052 | 3.124 | 58.680 | 0.88x |
| users.json | msgspec | 3.862 | 3.919 | 4.088 | 58.680 | 0.69x |
| users.json | ujson | 15.115 | 15.310 | 15.590 | 58.680 | 0.18x |
| users.json | json | 21.934 | 22.307 | 22.928 | 58.680 | 0.12x |
| flat.json | strata | 0.264 | 0.281 | 0.668 | 68.477 | 1.00x |
| flat.json | orjson | 0.332 | 0.345 | 0.601 | 68.477 | 0.81x |
| flat.json | msgspec | 0.442 | 0.466 | 0.558 | 68.477 | 0.60x |
| flat.json | ujson | 1.342 | 1.363 | 1.692 | 68.477 | 0.21x |
| flat.json | json | 1.866 | 1.910 | 2.026 | 68.477 | 0.15x |
| nested.json | strata | 0.247 | 0.252 | 0.265 | 68.477 | 1.00x |
| nested.json | orjson | 0.289 | 0.292 | 0.299 | 68.477 | 0.86x |
| nested.json | msgspec | 0.410 | 0.419 | 0.441 | 68.477 | 0.60x |
| nested.json | ujson | 1.446 | 1.456 | 1.508 | 68.477 | 0.17x |
| nested.json | json | 2.371 | 2.382 | 2.471 | 68.477 | 0.11x |
| wide_arrays.json | strata | 1.637 | 1.652 | 1.718 | 72.480 | 1.00x |
| wide_arrays.json | orjson | 1.827 | 1.840 | 1.910 | 72.480 | 0.90x |
| wide_arrays.json | msgspec | 2.695 | 2.716 | 2.759 | 72.480 | 0.61x |
| wide_arrays.json | ujson | 8.755 | 8.863 | 9.102 | 72.480 | 0.19x |
| wide_arrays.json | json | 16.534 | 16.656 | 17.293 | 72.480 | 0.10x |
| mixed.json | strata | 0.065 | 0.067 | 0.071 | 72.543 | 1.00x |
| mixed.json | orjson | 0.064 | 0.067 | 0.079 | 72.543 | 1.00x |
| mixed.json | msgspec | 0.085 | 0.087 | 0.100 | 72.543 | 0.78x |
| mixed.json | ujson | 0.319 | 0.327 | 0.338 | 72.543 | 0.21x |
| mixed.json | json | 0.510 | 0.520 | 0.547 | 72.543 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.841 | 12.209 | 14.396 | 71.090 | 1.00x |
| users.json | orjson | 13.952 | 14.399 | 15.445 | 71.090 | 0.85x |
| users.json | msgspec | 14.245 | 14.540 | 15.324 | 71.090 | 0.84x |
| users.json | ujson | 20.044 | 22.237 | 23.636 | 71.090 | 0.55x |
| users.json | json | 23.366 | 23.648 | 25.289 | 71.090 | 0.52x |
| flat.json | strata | 0.989 | 1.071 | 1.302 | 68.477 | 1.00x |
| flat.json | orjson | 1.118 | 1.158 | 1.414 | 68.477 | 0.92x |
| flat.json | msgspec | 1.128 | 1.195 | 1.348 | 68.477 | 0.90x |
| flat.json | ujson | 1.803 | 1.877 | 2.056 | 68.477 | 0.57x |
| flat.json | json | 2.022 | 2.062 | 2.259 | 68.477 | 0.52x |
| nested.json | strata | 0.907 | 0.922 | 1.001 | 68.477 | 1.00x |
| nested.json | orjson | 1.068 | 1.093 | 1.663 | 68.477 | 0.84x |
| nested.json | msgspec | 1.081 | 1.108 | 1.245 | 68.477 | 0.83x |
| nested.json | ujson | 1.570 | 1.629 | 1.981 | 68.477 | 0.57x |
| nested.json | json | 2.112 | 2.136 | 2.330 | 68.477 | 0.43x |
| wide_arrays.json | strata | 4.972 | 5.209 | 5.576 | 72.543 | 1.00x |
| wide_arrays.json | orjson | 5.254 | 6.034 | 6.326 | 72.543 | 0.86x |
| wide_arrays.json | msgspec | 5.804 | 6.155 | 6.557 | 72.543 | 0.85x |
| wide_arrays.json | ujson | 7.660 | 8.047 | 8.381 | 72.543 | 0.65x |
| wide_arrays.json | json | 10.034 | 10.497 | 10.971 | 72.543 | 0.50x |
| mixed.json | strata | 0.226 | 0.231 | 0.248 | 72.543 | 1.00x |
| mixed.json | orjson | 0.274 | 0.278 | 0.317 | 72.543 | 0.83x |
| mixed.json | msgspec | 0.281 | 0.288 | 0.303 | 72.543 | 0.80x |
| mixed.json | ujson | 0.363 | 0.375 | 0.392 | 72.543 | 0.62x |
| mixed.json | json | 0.512 | 0.515 | 0.539 | 72.543 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 12.573 | 13.613 | 14.203 | 68.477 | 1.00x |
| users.ndjson | orjson | 18.261 | 19.048 | 20.650 | 68.477 | 0.71x |
| users.ndjson | msgspec | 17.333 | 18.903 | 20.697 | 68.477 | 0.72x |
| users.ndjson | ujson | 23.490 | 24.829 | 27.063 | 68.477 | 0.55x |
| users.ndjson | json | 30.438 | 32.309 | 33.328 | 68.477 | 0.42x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.263 | 3.355 | 3.934 | 67.105 | 1.00x |
| users.json | orjson | 3.655 | 3.751 | 4.728 | 67.105 | 0.89x |
| users.json | msgspec | 4.580 | 4.730 | 4.887 | 67.105 | 0.71x |
| users.json | ujson | 15.951 | 16.125 | 16.691 | 67.105 | 0.21x |
| users.json | json | 22.772 | 23.041 | 27.169 | 67.105 | 0.15x |
| flat.json | strata | 0.434 | 0.446 | 0.506 | 68.477 | 1.00x |
| flat.json | orjson | 0.508 | 0.538 | 0.583 | 68.477 | 0.83x |
| flat.json | msgspec | 0.617 | 0.658 | 0.700 | 68.477 | 0.68x |
| flat.json | ujson | 1.536 | 1.578 | 1.610 | 68.477 | 0.28x |
| flat.json | json | 2.104 | 2.156 | 2.194 | 68.477 | 0.21x |
| nested.json | strata | 0.368 | 0.388 | 0.432 | 68.477 | 1.00x |
| nested.json | orjson | 0.425 | 0.454 | 0.524 | 68.477 | 0.85x |
| nested.json | msgspec | 0.542 | 0.562 | 0.663 | 68.477 | 0.69x |
| nested.json | ujson | 1.567 | 1.615 | 1.685 | 68.477 | 0.24x |
| nested.json | json | 2.548 | 2.580 | 2.632 | 68.477 | 0.15x |
| wide_arrays.json | strata | 2.115 | 2.148 | 2.547 | 72.543 | 1.00x |
| wide_arrays.json | orjson | 2.359 | 2.379 | 2.491 | 72.543 | 0.90x |
| wide_arrays.json | msgspec | 3.192 | 3.236 | 3.297 | 72.543 | 0.66x |
| wide_arrays.json | ujson | 9.315 | 9.472 | 10.154 | 72.543 | 0.23x |
| wide_arrays.json | json | 17.110 | 17.383 | 18.227 | 72.543 | 0.12x |
| mixed.json | strata | 0.160 | 0.164 | 0.237 | 72.543 | 1.00x |
| mixed.json | orjson | 0.173 | 0.177 | 0.198 | 72.543 | 0.92x |
| mixed.json | msgspec | 0.196 | 0.203 | 0.233 | 72.543 | 0.81x |
| mixed.json | ujson | 0.439 | 0.465 | 0.493 | 72.543 | 0.35x |
| mixed.json | json | 0.628 | 0.643 | 0.705 | 72.543 | 0.25x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.071 | 0.074 | 0.084 | 67.105 | 1.00x |
| users.json $[*].id | jmespath | 0.517 | 0.539 | 0.551 | 67.105 | 0.14x |
| users.json $[*].id | jsonpath-ng | 2.971 | 3.092 | 3.393 | 67.105 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.448 | 0.483 | 0.499 | 68.113 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.275 | 3.351 | 3.670 | 68.113 | 0.14x |
| users.json $[*].orders[*].total | jsonpath-ng | 20.346 | 21.545 | 24.089 | 68.113 | 0.02x |
| users.json $..total | strata | 1.696 | 1.751 | 1.824 | 72.184 | 1.00x |
| users.json $..total | jsonpath-ng | 395.754 | 399.366 | 401.939 | 72.184 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.299 | 3.318 | 3.379 | 68.113 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.185 | 14.493 | 15.430 | 68.113 | 0.23x |
| users.json $[*].id | orjson+jsonpath-ng | 16.665 | 17.034 | 17.715 | 68.113 | 0.19x |
| users.json $[*].orders[*].total | strata | 3.536 | 3.601 | 3.785 | 72.184 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.232 | 17.839 | 19.605 | 72.184 | 0.20x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 38.241 | 39.886 | 42.197 | 72.184 | 0.09x |
| users.json $..total | strata | 14.998 | 15.710 | 16.693 | 72.199 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 416.609 | 418.921 | 422.921 | 72.199 | 0.04x |

