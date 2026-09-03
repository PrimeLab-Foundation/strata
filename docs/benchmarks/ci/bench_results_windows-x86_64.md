# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 2b2f55a
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
| users.json | strata | 10.405 | 11.071 | 20.171 | 45.449 | 1.00x |
| users.json | orjson | 14.674 | 15.747 | 21.606 | 45.449 | 0.70x |
| users.json | msgspec | 13.336 | 14.052 | 24.592 | 45.449 | 0.79x |
| users.json | ujson | 22.113 | 24.145 | 30.938 | 45.449 | 0.46x |
| users.json | json | 22.966 | 23.500 | 27.226 | 45.449 | 0.47x |
| flat.json | strata | 1.613 | 1.684 | 1.830 | 50.473 | 1.00x |
| flat.json | orjson | 1.723 | 1.760 | 2.077 | 50.473 | 0.96x |
| flat.json | msgspec | 1.735 | 1.773 | 1.925 | 50.473 | 0.95x |
| flat.json | ujson | 3.176 | 3.209 | 3.293 | 50.473 | 0.52x |
| flat.json | json | 3.177 | 3.222 | 4.054 | 50.473 | 0.52x |
| nested.json | strata | 1.298 | 1.322 | 1.591 | 50.285 | 1.00x |
| nested.json | orjson | 1.742 | 1.759 | 2.004 | 50.285 | 0.75x |
| nested.json | msgspec | 1.608 | 1.627 | 1.679 | 50.285 | 0.81x |
| nested.json | ujson | 2.602 | 2.655 | 3.550 | 50.285 | 0.50x |
| nested.json | json | 3.499 | 3.526 | 50.032 | 50.285 | 0.37x |
| wide_arrays.json | strata | 7.784 | 8.095 | 11.977 | 52.258 | 1.00x |
| wide_arrays.json | orjson | 9.351 | 9.802 | 64.595 | 52.258 | 0.83x |
| wide_arrays.json | msgspec | 9.668 | 11.318 | 57.262 | 52.258 | 0.72x |
| wide_arrays.json | ujson | 13.127 | 13.863 | 19.072 | 52.258 | 0.58x |
| wide_arrays.json | json | 19.391 | 19.856 | 26.805 | 52.258 | 0.41x |
| mixed.json | strata | 0.320 | 0.329 | 0.363 | 50.059 | 1.00x |
| mixed.json | orjson | 0.353 | 0.389 | 0.467 | 50.059 | 0.85x |
| mixed.json | msgspec | 0.388 | 0.397 | 0.434 | 50.059 | 0.83x |
| mixed.json | ujson | 0.561 | 0.598 | 0.626 | 50.059 | 0.55x |
| mixed.json | json | 0.786 | 0.795 | 0.860 | 50.059 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.341 | 3.391 | 3.453 | 44.621 | 1.00x |
| users.json | orjson | 3.933 | 3.972 | 5.639 | 44.621 | 0.85x |
| users.json | msgspec | 5.811 | 5.966 | 8.352 | 44.621 | 0.57x |
| users.json | ujson | 16.550 | 16.731 | 28.089 | 44.621 | 0.20x |
| users.json | json | 23.342 | 23.742 | 37.534 | 44.621 | 0.14x |
| flat.json | strata | 0.457 | 0.482 | 2.323 | 50.602 | 1.00x |
| flat.json | orjson | 0.552 | 0.583 | 0.674 | 50.602 | 0.83x |
| flat.json | msgspec | 0.753 | 0.862 | 3.286 | 50.602 | 0.56x |
| flat.json | ujson | 2.545 | 2.638 | 56.211 | 50.602 | 0.18x |
| flat.json | json | 3.532 | 5.151 | 46.573 | 50.602 | 0.09x |
| nested.json | strata | 0.409 | 0.420 | 0.611 | 50.730 | 1.00x |
| nested.json | orjson | 0.472 | 0.479 | 2.070 | 50.730 | 0.88x |
| nested.json | msgspec | 0.733 | 0.744 | 0.876 | 50.730 | 0.56x |
| nested.json | ujson | 2.096 | 2.110 | 3.162 | 50.730 | 0.20x |
| nested.json | json | 4.208 | 4.303 | 6.030 | 50.730 | 0.10x |
| wide_arrays.json | strata | 3.348 | 3.382 | 5.181 | 52.223 | 1.00x |
| wide_arrays.json | orjson | 3.778 | 3.970 | 5.579 | 52.223 | 0.85x |
| wide_arrays.json | msgspec | 6.145 | 6.382 | 9.788 | 52.223 | 0.53x |
| wide_arrays.json | ujson | 14.960 | 15.303 | 55.490 | 52.223 | 0.22x |
| wide_arrays.json | json | 29.333 | 34.106 | 75.797 | 52.223 | 0.10x |
| mixed.json | strata | 0.107 | 0.116 | 0.139 | 50.176 | 1.00x |
| mixed.json | orjson | 0.103 | 0.109 | 0.142 | 50.176 | 1.07x |
| mixed.json | msgspec | 0.146 | 0.153 | 0.174 | 50.176 | 0.76x |
| mixed.json | ujson | 0.508 | 0.522 | 2.217 | 50.176 | 0.22x |
| mixed.json | json | 0.892 | 0.934 | 52.720 | 50.176 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.760 | 12.922 | 19.809 | 54.645 | 1.00x |
| users.json | orjson | 15.724 | 16.380 | 21.636 | 54.645 | 0.79x |
| users.json | msgspec | 14.495 | 14.872 | 16.121 | 54.645 | 0.87x |
| users.json | ujson | 25.738 | 26.858 | 30.470 | 54.645 | 0.48x |
| users.json | json | 23.534 | 24.396 | 32.234 | 54.645 | 0.53x |
| flat.json | strata | 1.801 | 1.986 | 2.040 | 50.465 | 1.00x |
| flat.json | orjson | 2.009 | 2.266 | 2.472 | 50.465 | 0.88x |
| flat.json | msgspec | 1.952 | 1.976 | 2.044 | 50.465 | 1.00x |
| flat.json | ujson | 3.952 | 4.009 | 4.409 | 50.465 | 0.50x |
| flat.json | json | 3.560 | 3.607 | 3.760 | 50.465 | 0.55x |
| nested.json | strata | 1.448 | 1.545 | 1.951 | 50.453 | 1.00x |
| nested.json | orjson | 1.872 | 2.116 | 4.299 | 50.453 | 0.73x |
| nested.json | msgspec | 1.772 | 2.004 | 4.360 | 50.453 | 0.77x |
| nested.json | ujson | 3.019 | 3.386 | 5.151 | 50.453 | 0.46x |
| nested.json | json | 3.677 | 4.035 | 54.973 | 50.453 | 0.38x |
| wide_arrays.json | strata | 8.532 | 8.905 | 12.845 | 52.223 | 1.00x |
| wide_arrays.json | orjson | 9.795 | 10.278 | 69.652 | 52.223 | 0.87x |
| wide_arrays.json | msgspec | 10.314 | 10.776 | 15.934 | 52.223 | 0.83x |
| wide_arrays.json | ujson | 16.667 | 17.382 | 30.247 | 52.223 | 0.51x |
| wide_arrays.json | json | 19.866 | 20.481 | 123.655 | 52.223 | 0.43x |
| mixed.json | strata | 0.421 | 0.427 | 0.458 | 50.184 | 1.00x |
| mixed.json | orjson | 0.488 | 0.541 | 2.414 | 50.184 | 0.79x |
| mixed.json | msgspec | 0.528 | 0.553 | 0.579 | 50.184 | 0.77x |
| mixed.json | ujson | 0.762 | 0.798 | 0.856 | 50.184 | 0.54x |
| mixed.json | json | 0.937 | 0.948 | 2.767 | 50.184 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 18.376 | 26.220 | 101.790 | 51.734 | 1.00x |
| users.ndjson | orjson | 28.238 | 32.424 | 42.306 | 51.734 | 0.81x |
| users.ndjson | msgspec | 28.029 | 34.438 | 42.609 | 51.734 | 0.76x |
| users.ndjson | ujson | 41.349 | 55.921 | 91.010 | 51.734 | 0.47x |
| users.ndjson | json | 51.017 | 71.624 | 164.334 | 51.734 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.390 | 4.584 | 6.358 | 51.836 | 1.00x |
| users.json | orjson | 4.846 | 5.174 | 7.269 | 51.836 | 0.89x |
| users.json | msgspec | 6.856 | 7.215 | 12.522 | 51.836 | 0.64x |
| users.json | ujson | 26.000 | 26.668 | 51.584 | 51.836 | 0.17x |
| users.json | json | 33.032 | 33.491 | 44.274 | 51.836 | 0.14x |
| flat.json | strata | 1.023 | 1.102 | 2.868 | 50.711 | 1.00x |
| flat.json | orjson | 1.091 | 1.110 | 3.058 | 50.711 | 0.99x |
| flat.json | msgspec | 1.291 | 1.382 | 3.166 | 50.711 | 0.80x |
| flat.json | ujson | 5.093 | 6.882 | 8.590 | 50.711 | 0.16x |
| flat.json | json | 6.122 | 7.984 | 11.538 | 50.711 | 0.14x |
| nested.json | strata | 0.917 | 0.966 | 47.932 | 50.855 | 1.00x |
| nested.json | orjson | 0.965 | 1.060 | 3.151 | 50.855 | 0.91x |
| nested.json | msgspec | 1.235 | 1.368 | 3.251 | 50.855 | 0.71x |
| nested.json | ujson | 3.921 | 5.747 | 6.567 | 50.855 | 0.17x |
| nested.json | json | 6.009 | 7.815 | 9.587 | 50.855 | 0.12x |
| wide_arrays.json | strata | 4.338 | 4.523 | 6.327 | 52.227 | 1.00x |
| wide_arrays.json | orjson | 4.626 | 5.255 | 6.840 | 52.227 | 0.86x |
| wide_arrays.json | msgspec | 7.014 | 7.134 | 9.096 | 52.227 | 0.63x |
| wide_arrays.json | ujson | 27.012 | 27.769 | 37.995 | 52.227 | 0.16x |
| wide_arrays.json | json | 41.598 | 43.180 | 108.175 | 52.227 | 0.10x |
| mixed.json | strata | 0.499 | 0.521 | 0.580 | 50.375 | 1.00x |
| mixed.json | orjson | 0.493 | 0.530 | 1.226 | 50.375 | 0.98x |
| mixed.json | msgspec | 0.541 | 0.589 | 2.609 | 50.375 | 0.88x |
| mixed.json | ujson | 1.239 | 1.257 | 3.044 | 50.375 | 0.41x |
| mixed.json | json | 1.594 | 1.651 | 3.714 | 50.375 | 0.32x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.104 | 0.110 | 0.251 | 51.438 | 1.00x |
| users.json $[*].id | jmespath | 0.426 | 0.454 | 0.747 | 51.438 | 0.24x |
| users.json $[*].id | jsonpath-ng | 2.479 | 2.675 | 4.284 | 51.438 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.553 | 0.582 | 0.633 | 51.680 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.794 | 2.843 | 2.884 | 51.680 | 0.20x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.286 | 17.541 | 18.313 | 51.680 | 0.03x |
| users.json $..total | strata | 1.894 | 2.462 | 6.304 | 52.703 | 1.00x |
| users.json $..total | jsonpath-ng | 357.682 | 426.212 | 535.969 | 52.703 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.345 | 4.375 | 4.647 | 51.445 | 1.00x |
| users.json $[*].id | orjson+jmespath | 16.099 | 16.965 | 24.510 | 51.445 | 0.26x |
| users.json $[*].id | orjson+jsonpath-ng | 18.298 | 18.780 | 24.968 | 51.445 | 0.23x |
| users.json $[*].orders[*].total | strata | 5.137 | 8.469 | 12.905 | 52.703 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 21.236 | 28.125 | 47.705 | 52.703 | 0.30x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 42.493 | 53.584 | 110.652 | 52.703 | 0.16x |
| users.json $..total | strata | 23.739 | 33.287 | 41.025 | 52.703 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 792.093 | 860.970 | 984.665 | 52.703 | 0.04x |

