# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 4d4a4be
- python: 3.12.13
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
| users.json | strata | 10.295 | 10.703 | 27.492 | 58.168 | 1.00x |
| users.json | orjson | 13.110 | 13.495 | 22.408 | 58.168 | 0.79x |
| users.json | msgspec | 13.016 | 13.330 | 18.283 | 58.168 | 0.80x |
| users.json | ujson | 18.894 | 19.616 | 23.026 | 58.168 | 0.55x |
| users.json | pysimdjson | 18.427 | 19.792 | 22.272 | 58.168 | 0.54x |
| users.json | json | 22.272 | 22.699 | 25.733 | 58.168 | 0.47x |
| flat.json | strata | 0.889 | 0.901 | 0.915 | 70.770 | 1.00x |
| flat.json | orjson | 0.973 | 0.984 | 1.001 | 70.770 | 0.92x |
| flat.json | msgspec | 0.993 | 1.016 | 1.049 | 70.770 | 0.89x |
| flat.json | ujson | 1.472 | 1.545 | 1.666 | 70.770 | 0.58x |
| flat.json | pysimdjson | 1.526 | 1.569 | 1.631 | 70.770 | 0.57x |
| flat.json | json | 1.884 | 1.899 | 1.916 | 70.770 | 0.47x |
| nested.json | strata | 0.844 | 0.853 | 1.086 | 70.801 | 1.00x |
| nested.json | orjson | 1.000 | 1.014 | 1.038 | 70.801 | 0.84x |
| nested.json | msgspec | 1.015 | 1.026 | 1.043 | 70.801 | 0.83x |
| nested.json | ujson | 1.466 | 1.509 | 1.553 | 70.801 | 0.57x |
| nested.json | pysimdjson | 1.398 | 1.408 | 1.533 | 70.801 | 0.61x |
| nested.json | json | 2.092 | 2.108 | 2.574 | 70.801 | 0.40x |
| wide_arrays.json | strata | 4.760 | 4.828 | 4.903 | 74.801 | 1.00x |
| wide_arrays.json | orjson | 5.051 | 5.173 | 8.415 | 74.801 | 0.93x |
| wide_arrays.json | msgspec | 5.587 | 5.696 | 7.471 | 74.801 | 0.85x |
| wide_arrays.json | ujson | 7.210 | 7.294 | 8.021 | 74.801 | 0.66x |
| wide_arrays.json | pysimdjson | 5.971 | 6.118 | 6.561 | 74.801 | 0.79x |
| wide_arrays.json | json | 9.771 | 10.040 | 10.535 | 74.801 | 0.48x |
| mixed.json | strata | 0.202 | 0.206 | 0.222 | 74.863 | 1.00x |
| mixed.json | orjson | 0.230 | 0.236 | 0.325 | 74.863 | 0.87x |
| mixed.json | msgspec | 0.237 | 0.247 | 0.262 | 74.863 | 0.83x |
| mixed.json | ujson | 0.306 | 0.320 | 0.362 | 74.863 | 0.64x |
| mixed.json | pysimdjson | 0.294 | 0.301 | 0.324 | 74.863 | 0.68x |
| mixed.json | json | 0.468 | 0.486 | 0.499 | 74.863 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.703 | 2.732 | 2.809 | 59.645 | 1.00x |
| users.json | orjson | 2.973 | 3.002 | 3.150 | 59.645 | 0.91x |
| users.json | msgspec | 3.862 | 3.882 | 3.984 | 59.645 | 0.70x |
| users.json | ujson | 14.076 | 14.245 | 14.673 | 59.645 | 0.19x |
| users.json | json | 21.875 | 22.088 | 22.243 | 59.645 | 0.12x |
| flat.json | strata | 0.271 | 0.274 | 0.286 | 70.770 | 1.00x |
| flat.json | orjson | 0.326 | 0.340 | 0.345 | 70.770 | 0.81x |
| flat.json | msgspec | 0.431 | 0.441 | 0.468 | 70.770 | 0.62x |
| flat.json | ujson | 1.208 | 1.228 | 1.247 | 70.770 | 0.22x |
| flat.json | json | 1.887 | 1.900 | 2.251 | 70.770 | 0.14x |
| nested.json | strata | 0.247 | 0.262 | 0.276 | 70.801 | 1.00x |
| nested.json | orjson | 0.292 | 0.306 | 0.321 | 70.801 | 0.86x |
| nested.json | msgspec | 0.405 | 0.412 | 0.423 | 70.801 | 0.64x |
| nested.json | ujson | 1.322 | 1.331 | 1.343 | 70.801 | 0.20x |
| nested.json | json | 2.426 | 2.459 | 2.526 | 70.801 | 0.11x |
| wide_arrays.json | strata | 1.608 | 1.623 | 1.644 | 74.801 | 1.00x |
| wide_arrays.json | orjson | 1.791 | 1.824 | 2.003 | 74.801 | 0.89x |
| wide_arrays.json | msgspec | 2.680 | 2.687 | 2.719 | 74.801 | 0.60x |
| wide_arrays.json | ujson | 8.594 | 8.659 | 8.721 | 74.801 | 0.19x |
| wide_arrays.json | json | 16.407 | 16.445 | 16.549 | 74.801 | 0.10x |
| mixed.json | strata | 0.069 | 0.072 | 0.086 | 74.863 | 1.00x |
| mixed.json | orjson | 0.063 | 0.066 | 0.084 | 74.863 | 1.08x |
| mixed.json | msgspec | 0.084 | 0.087 | 0.102 | 74.863 | 0.82x |
| mixed.json | ujson | 0.295 | 0.305 | 0.319 | 74.863 | 0.23x |
| mixed.json | json | 0.513 | 0.521 | 0.537 | 74.863 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.218 | 11.853 | 13.397 | 73.559 | 1.00x |
| users.json | orjson | 13.655 | 14.200 | 15.036 | 73.559 | 0.83x |
| users.json | msgspec | 13.784 | 13.979 | 14.408 | 73.559 | 0.85x |
| users.json | ujson | 20.006 | 20.524 | 22.455 | 73.559 | 0.58x |
| users.json | json | 22.993 | 23.228 | 24.089 | 73.559 | 0.51x |
| flat.json | strata | 0.926 | 0.938 | 0.979 | 70.770 | 1.00x |
| flat.json | orjson | 1.032 | 1.045 | 1.095 | 70.770 | 0.90x |
| flat.json | msgspec | 1.057 | 1.072 | 1.862 | 70.770 | 0.88x |
| flat.json | ujson | 1.558 | 1.606 | 3.189 | 70.770 | 0.58x |
| flat.json | json | 1.948 | 1.963 | 2.027 | 70.770 | 0.48x |
| nested.json | strata | 0.865 | 0.880 | 0.935 | 70.801 | 1.00x |
| nested.json | orjson | 1.047 | 1.060 | 1.120 | 70.801 | 0.83x |
| nested.json | msgspec | 1.061 | 1.075 | 1.088 | 70.801 | 0.82x |
| nested.json | ujson | 1.555 | 1.570 | 1.597 | 70.801 | 0.56x |
| nested.json | json | 2.129 | 2.156 | 2.206 | 70.801 | 0.41x |
| wide_arrays.json | strata | 4.915 | 5.032 | 5.085 | 74.863 | 1.00x |
| wide_arrays.json | orjson | 5.191 | 5.306 | 5.478 | 74.863 | 0.95x |
| wide_arrays.json | msgspec | 5.701 | 5.864 | 5.941 | 74.863 | 0.86x |
| wide_arrays.json | ujson | 7.470 | 7.518 | 7.690 | 74.863 | 0.67x |
| wide_arrays.json | json | 9.890 | 10.125 | 10.276 | 74.863 | 0.50x |
| mixed.json | strata | 0.226 | 0.231 | 0.250 | 74.863 | 1.00x |
| mixed.json | orjson | 0.278 | 0.292 | 0.307 | 74.863 | 0.79x |
| mixed.json | msgspec | 0.285 | 0.288 | 0.302 | 74.863 | 0.80x |
| mixed.json | ujson | 0.364 | 0.381 | 0.406 | 74.863 | 0.61x |
| mixed.json | json | 0.517 | 0.531 | 0.595 | 74.863 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 11.202 | 11.942 | 13.607 | 70.770 | 1.00x |
| users.ndjson | orjson | 16.767 | 17.227 | 19.060 | 70.770 | 0.69x |
| users.ndjson | msgspec | 16.236 | 16.904 | 18.362 | 70.770 | 0.71x |
| users.ndjson | ujson | 21.442 | 23.182 | 25.165 | 70.770 | 0.52x |
| users.ndjson | json | 28.942 | 30.362 | 33.555 | 70.770 | 0.39x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.276 | 3.336 | 3.530 | 69.621 | 1.00x |
| users.json | orjson | 3.604 | 3.653 | 3.744 | 69.621 | 0.91x |
| users.json | msgspec | 4.527 | 4.577 | 4.692 | 69.621 | 0.73x |
| users.json | ujson | 15.113 | 15.288 | 18.174 | 69.621 | 0.22x |
| users.json | json | 22.615 | 23.172 | 26.327 | 69.621 | 0.14x |
| flat.json | strata | 0.408 | 0.421 | 0.522 | 70.770 | 1.00x |
| flat.json | orjson | 0.489 | 0.501 | 0.542 | 70.770 | 0.84x |
| flat.json | msgspec | 0.594 | 0.609 | 0.633 | 70.770 | 0.69x |
| flat.json | ujson | 1.418 | 1.453 | 1.497 | 70.770 | 0.29x |
| flat.json | json | 2.062 | 2.085 | 2.158 | 70.770 | 0.20x |
| nested.json | strata | 0.365 | 0.388 | 0.418 | 70.801 | 1.00x |
| nested.json | orjson | 0.425 | 0.438 | 0.473 | 70.801 | 0.89x |
| nested.json | msgspec | 0.539 | 0.559 | 0.587 | 70.801 | 0.69x |
| nested.json | ujson | 1.461 | 1.479 | 1.509 | 70.801 | 0.26x |
| nested.json | json | 2.572 | 2.611 | 2.687 | 70.801 | 0.15x |
| wide_arrays.json | strata | 2.057 | 2.090 | 2.137 | 74.863 | 1.00x |
| wide_arrays.json | orjson | 2.275 | 2.314 | 2.338 | 74.863 | 0.90x |
| wide_arrays.json | msgspec | 3.133 | 3.185 | 3.280 | 74.863 | 0.66x |
| wide_arrays.json | ujson | 9.144 | 9.246 | 9.695 | 74.863 | 0.23x |
| wide_arrays.json | json | 16.946 | 17.054 | 17.245 | 74.863 | 0.12x |
| mixed.json | strata | 0.156 | 0.165 | 0.199 | 74.863 | 1.00x |
| mixed.json | orjson | 0.170 | 0.179 | 0.203 | 74.863 | 0.92x |
| mixed.json | msgspec | 0.188 | 0.193 | 0.230 | 74.863 | 0.85x |
| mixed.json | ujson | 0.409 | 0.421 | 0.474 | 74.863 | 0.39x |
| mixed.json | json | 0.622 | 0.647 | 0.677 | 74.863 | 0.25x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.070 | 0.076 | 0.081 | 69.621 | 1.00x |
| users.json $[*].id | jmespath | 0.475 | 0.491 | 0.506 | 69.621 | 0.16x |
| users.json $[*].id | jsonpath-ng | 2.896 | 3.015 | 3.170 | 69.621 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.446 | 0.457 | 0.480 | 70.613 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.237 | 3.277 | 3.450 | 70.613 | 0.14x |
| users.json $[*].orders[*].total | jsonpath-ng | 19.171 | 19.855 | 20.360 | 70.613 | 0.02x |
| users.json $..total | strata | 1.652 | 1.680 | 1.788 | 76.336 | 1.00x |
| users.json $..total | jsonpath-ng | 382.981 | 385.505 | 389.017 | 76.336 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.002 | 3.033 | 3.091 | 70.613 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.192 | 14.558 | 14.739 | 70.613 | 0.21x |
| users.json $[*].id | orjson+jsonpath-ng | 16.551 | 16.799 | 18.490 | 70.613 | 0.18x |
| users.json $[*].orders[*].total | strata | 3.263 | 3.314 | 3.483 | 74.746 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.406 | 17.739 | 17.927 | 74.746 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 36.837 | 38.056 | 39.804 | 74.746 | 0.09x |
| users.json $..total | strata | 14.055 | 14.271 | 15.516 | 75.586 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 411.223 | 413.590 | 418.568 | 75.586 | 0.03x |

