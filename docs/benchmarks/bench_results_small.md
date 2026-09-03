# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c88786c
- python: 3.14.7
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit-Mach-O
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
| users.json | strata | 6.615 | 6.665 | 7.167 | 51.438 | 1.00x |
| users.json | orjson | 7.782 | 7.833 | 8.707 | 51.438 | 0.85x |
| users.json | msgspec | 7.931 | 7.970 | 8.756 | 51.438 | 0.84x |
| users.json | ujson | 10.759 | 10.882 | 12.133 | 51.438 | 0.61x |
| users.json | json | 15.032 | 15.117 | 16.140 | 51.438 | 0.44x |
| flat.json | strata | 0.593 | 0.598 | 0.606 | 68.188 | 1.00x |
| flat.json | orjson | 0.642 | 0.648 | 0.657 | 68.188 | 0.92x |
| flat.json | msgspec | 0.676 | 0.682 | 0.736 | 68.188 | 0.88x |
| flat.json | ujson | 0.999 | 1.009 | 1.020 | 68.188 | 0.59x |
| flat.json | json | 1.419 | 1.427 | 1.455 | 68.188 | 0.42x |
| nested.json | strata | 0.530 | 0.533 | 0.538 | 68.203 | 1.00x |
| nested.json | orjson | 0.604 | 0.607 | 0.649 | 68.203 | 0.88x |
| nested.json | msgspec | 0.604 | 0.606 | 0.614 | 68.203 | 0.88x |
| nested.json | ujson | 0.887 | 0.889 | 0.903 | 68.203 | 0.60x |
| nested.json | json | 1.398 | 1.404 | 1.449 | 68.203 | 0.38x |
| wide_arrays.json | strata | 3.161 | 3.168 | 3.185 | 68.266 | 1.00x |
| wide_arrays.json | orjson | 3.156 | 3.175 | 3.199 | 68.266 | 1.00x |
| wide_arrays.json | msgspec | 3.874 | 3.911 | 4.055 | 68.266 | 0.81x |
| wide_arrays.json | ujson | 5.095 | 5.132 | 5.164 | 68.266 | 0.62x |
| wide_arrays.json | json | 7.439 | 7.504 | 7.623 | 68.266 | 0.42x |
| mixed.json | strata | 0.129 | 0.131 | 0.137 | 68.266 | 1.00x |
| mixed.json | orjson | 0.125 | 0.127 | 0.135 | 68.266 | 1.03x |
| mixed.json | msgspec | 0.139 | 0.141 | 0.168 | 68.266 | 0.93x |
| mixed.json | ujson | 0.180 | 0.183 | 0.210 | 68.266 | 0.71x |
| mixed.json | json | 0.300 | 0.301 | 0.317 | 68.266 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.633 | 1.636 | 1.685 | 55.469 | 1.00x |
| users.json | orjson | 1.984 | 1.994 | 2.038 | 55.469 | 0.82x |
| users.json | msgspec | 2.617 | 2.635 | 2.663 | 55.469 | 0.62x |
| users.json | ujson | 10.805 | 10.849 | 11.048 | 55.469 | 0.15x |
| users.json | json | 15.433 | 15.511 | 16.834 | 55.469 | 0.11x |
| flat.json | strata | 0.210 | 0.215 | 0.228 | 68.203 | 1.00x |
| flat.json | orjson | 0.223 | 0.226 | 0.236 | 68.203 | 0.95x |
| flat.json | msgspec | 0.293 | 0.295 | 0.316 | 68.203 | 0.73x |
| flat.json | ujson | 0.901 | 0.909 | 0.957 | 68.203 | 0.24x |
| flat.json | json | 1.342 | 1.350 | 1.481 | 68.203 | 0.16x |
| nested.json | strata | 0.142 | 0.143 | 0.148 | 68.250 | 1.00x |
| nested.json | orjson | 0.203 | 0.204 | 0.211 | 68.250 | 0.70x |
| nested.json | msgspec | 0.271 | 0.274 | 0.283 | 68.250 | 0.52x |
| nested.json | ujson | 0.961 | 0.965 | 1.046 | 68.250 | 0.15x |
| nested.json | json | 1.619 | 1.636 | 1.664 | 68.250 | 0.09x |
| wide_arrays.json | strata | 0.980 | 0.989 | 1.008 | 68.266 | 1.00x |
| wide_arrays.json | orjson | 1.116 | 1.121 | 1.143 | 68.266 | 0.88x |
| wide_arrays.json | msgspec | 1.929 | 1.941 | 1.968 | 68.266 | 0.51x |
| wide_arrays.json | ujson | 5.980 | 6.014 | 6.183 | 68.266 | 0.16x |
| wide_arrays.json | json | 11.014 | 11.159 | 11.974 | 68.266 | 0.09x |
| mixed.json | strata | 0.040 | 0.041 | 0.044 | 68.312 | 1.00x |
| mixed.json | orjson | 0.042 | 0.042 | 0.044 | 68.312 | 0.97x |
| mixed.json | msgspec | 0.048 | 0.049 | 0.056 | 68.312 | 0.83x |
| mixed.json | ujson | 0.215 | 0.216 | 0.220 | 68.312 | 0.19x |
| mixed.json | json | 0.349 | 0.353 | 0.359 | 68.312 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.926 | 7.035 | 7.282 | 69.078 | 1.00x |
| users.json | orjson | 8.139 | 8.261 | 8.369 | 69.078 | 0.85x |
| users.json | msgspec | 8.324 | 8.395 | 8.788 | 69.078 | 0.84x |
| users.json | ujson | 11.345 | 11.527 | 11.716 | 69.078 | 0.61x |
| users.json | json | 15.448 | 15.503 | 15.552 | 69.078 | 0.45x |
| flat.json | strata | 0.661 | 0.678 | 0.713 | 68.203 | 1.00x |
| flat.json | orjson | 0.724 | 0.739 | 0.781 | 68.203 | 0.92x |
| flat.json | msgspec | 0.759 | 0.776 | 0.825 | 68.203 | 0.87x |
| flat.json | ujson | 1.128 | 1.158 | 1.188 | 68.203 | 0.59x |
| flat.json | json | 1.505 | 1.519 | 1.544 | 68.203 | 0.45x |
| nested.json | strata | 0.592 | 0.600 | 0.612 | 68.250 | 1.00x |
| nested.json | orjson | 0.670 | 0.678 | 0.685 | 68.250 | 0.89x |
| nested.json | msgspec | 0.669 | 0.676 | 0.695 | 68.250 | 0.89x |
| nested.json | ujson | 0.975 | 0.978 | 0.985 | 68.250 | 0.61x |
| nested.json | json | 1.463 | 1.471 | 1.494 | 68.250 | 0.41x |
| wide_arrays.json | strata | 3.335 | 3.366 | 3.471 | 68.266 | 1.00x |
| wide_arrays.json | orjson | 3.368 | 3.434 | 3.484 | 68.266 | 0.98x |
| wide_arrays.json | msgspec | 4.161 | 4.187 | 4.263 | 68.266 | 0.80x |
| wide_arrays.json | ujson | 5.457 | 5.514 | 5.610 | 68.266 | 0.61x |
| wide_arrays.json | json | 7.683 | 7.756 | 7.817 | 68.266 | 0.43x |
| mixed.json | strata | 0.171 | 0.174 | 0.182 | 68.312 | 1.00x |
| mixed.json | orjson | 0.166 | 0.173 | 0.184 | 68.312 | 1.00x |
| mixed.json | msgspec | 0.185 | 0.194 | 0.205 | 68.312 | 0.90x |
| mixed.json | ujson | 0.234 | 0.239 | 0.253 | 68.312 | 0.73x |
| mixed.json | json | 0.342 | 0.346 | 0.359 | 68.312 | 0.50x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.063 | 7.098 | 7.262 | 67.328 | 1.00x |
| users.ndjson | orjson | 9.881 | 9.988 | 10.076 | 67.328 | 0.71x |
| users.ndjson | msgspec | 9.746 | 9.934 | 10.059 | 67.328 | 0.71x |
| users.ndjson | ujson | 12.663 | 12.777 | 12.922 | 67.328 | 0.56x |
| users.ndjson | json | 17.675 | 17.819 | 18.017 | 67.328 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.020 | 2.081 | 2.508 | 67.109 | 1.00x |
| users.json | orjson | 2.373 | 2.485 | 3.153 | 67.109 | 0.84x |
| users.json | msgspec | 2.997 | 3.107 | 3.359 | 67.109 | 0.67x |
| users.json | ujson | 10.936 | 11.107 | 11.426 | 67.109 | 0.19x |
| users.json | json | 15.632 | 16.028 | 16.379 | 67.109 | 0.13x |
| flat.json | strata | 0.356 | 0.412 | 1.664 | 68.203 | 1.00x |
| flat.json | orjson | 0.379 | 0.395 | 2.226 | 68.203 | 1.04x |
| flat.json | msgspec | 0.460 | 0.485 | 0.925 | 68.203 | 0.85x |
| flat.json | ujson | 1.061 | 1.270 | 1.469 | 68.203 | 0.32x |
| flat.json | json | 1.505 | 1.568 | 1.837 | 68.203 | 0.26x |
| nested.json | strata | 0.276 | 0.307 | 0.738 | 68.266 | 1.00x |
| nested.json | orjson | 0.340 | 0.377 | 0.709 | 68.266 | 0.82x |
| nested.json | msgspec | 0.407 | 0.426 | 0.747 | 68.266 | 0.72x |
| nested.json | ujson | 1.120 | 1.153 | 1.170 | 68.266 | 0.27x |
| nested.json | json | 1.757 | 1.832 | 2.245 | 68.266 | 0.17x |
| wide_arrays.json | strata | 1.297 | 1.326 | 1.467 | 68.266 | 1.00x |
| wide_arrays.json | orjson | 1.418 | 1.442 | 1.555 | 68.266 | 0.92x |
| wide_arrays.json | msgspec | 2.258 | 2.269 | 2.359 | 68.266 | 0.58x |
| wide_arrays.json | ujson | 6.351 | 6.389 | 6.651 | 68.266 | 0.21x |
| wide_arrays.json | json | 11.761 | 11.894 | 12.109 | 68.266 | 0.11x |
| mixed.json | strata | 0.157 | 0.188 | 0.567 | 68.344 | 1.00x |
| mixed.json | orjson | 0.165 | 0.194 | 0.487 | 68.344 | 0.97x |
| mixed.json | msgspec | 0.171 | 0.191 | 0.239 | 68.344 | 0.99x |
| mixed.json | ujson | 0.365 | 0.385 | 0.643 | 68.344 | 0.49x |
| mixed.json | json | 0.489 | 0.506 | 0.542 | 68.344 | 0.37x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.049 | 0.051 | 0.064 | 67.312 | 1.00x |
| users.json $[*].id | jmespath | 0.274 | 0.276 | 0.285 | 67.312 | 0.18x |
| users.json $[*].id | jsonpath-ng | 1.424 | 1.432 | 1.501 | 67.312 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.329 | 0.337 | 0.415 | 67.641 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.664 | 1.686 | 1.747 | 67.641 | 0.20x |
| users.json $[*].orders[*].total | jsonpath-ng | 9.410 | 9.475 | 9.837 | 67.641 | 0.04x |
| users.json $..total | strata | 1.383 | 1.411 | 1.559 | 69.516 | 1.00x |
| users.json $..total | jsonpath-ng | 183.308 | 185.059 | 215.887 | 69.516 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.417 | 3.446 | 3.519 | 67.359 | 1.00x |
| users.json $[*].id | orjson+jmespath | 8.315 | 8.388 | 8.705 | 67.359 | 0.41x |
| users.json $[*].id | orjson+jsonpath-ng | 9.464 | 9.548 | 9.853 | 67.359 | 0.36x |
| users.json $[*].orders[*].total | strata | 3.473 | 3.502 | 3.719 | 69.359 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 9.697 | 9.769 | 9.885 | 69.359 | 0.36x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 18.816 | 18.953 | 20.816 | 69.359 | 0.18x |
| users.json $..total | strata | 8.353 | 8.438 | 9.010 | 69.531 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 193.276 | 194.803 | 197.319 | 69.531 | 0.04x |

