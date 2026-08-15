# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: dc84475
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD
- compiler_flags: -std=c++20 -O3 -march=native
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.895 | 12.578 | 14.890 | 45.621 | 1.00x |
| users.json | orjson | 14.182 | 15.006 | 18.188 | 45.621 | 0.84x |
| users.json | msgspec | 13.264 | 14.335 | 23.704 | 45.621 | 0.88x |
| users.json | ujson | 23.434 | 25.895 | 43.987 | 45.621 | 0.49x |
| users.json | json | 23.889 | 24.306 | 33.199 | 45.621 | 0.52x |
| flat.json | strata | 1.793 | 1.868 | 2.857 | 50.012 | 1.00x |
| flat.json | orjson | 1.669 | 1.766 | 1.974 | 50.012 | 1.06x |
| flat.json | msgspec | 1.793 | 1.925 | 2.115 | 50.012 | 0.97x |
| flat.json | ujson | 3.432 | 3.524 | 3.644 | 50.012 | 0.53x |
| flat.json | json | 3.413 | 3.521 | 4.209 | 50.012 | 0.53x |
| nested.json | strata | 1.532 | 1.573 | 1.667 | 49.477 | 1.00x |
| nested.json | orjson | 1.744 | 1.830 | 2.166 | 49.477 | 0.86x |
| nested.json | msgspec | 1.694 | 1.727 | 1.822 | 49.477 | 0.91x |
| nested.json | ujson | 2.828 | 2.914 | 3.288 | 49.477 | 0.54x |
| nested.json | json | 3.724 | 3.803 | 4.050 | 49.477 | 0.41x |
| wide_arrays.json | strata | 9.773 | 10.652 | 11.266 | 51.438 | 1.00x |
| wide_arrays.json | orjson | 9.976 | 10.516 | 10.967 | 51.438 | 1.01x |
| wide_arrays.json | msgspec | 10.273 | 10.931 | 11.756 | 51.438 | 0.97x |
| wide_arrays.json | ujson | 14.120 | 14.715 | 62.563 | 51.438 | 0.72x |
| wide_arrays.json | json | 21.254 | 23.018 | 26.414 | 51.438 | 0.46x |
| mixed.json | strata | 0.249 | 0.436 | 0.612 | 50.820 | 1.00x |
| mixed.json | orjson | 0.287 | 0.404 | 1.168 | 50.820 | 1.08x |
| mixed.json | msgspec | 0.305 | 0.457 | 1.298 | 50.820 | 0.96x |
| mixed.json | ujson | 0.451 | 0.636 | 1.502 | 50.820 | 0.69x |
| mixed.json | json | 0.659 | 0.938 | 3.184 | 50.820 | 0.47x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.332 | 4.485 | 5.049 | 44.172 | 1.00x |
| users.json | orjson | 3.854 | 3.960 | 4.219 | 44.172 | 1.13x |
| users.json | msgspec | 5.292 | 5.539 | 5.981 | 44.172 | 0.81x |
| users.json | ujson | 16.834 | 16.980 | 28.150 | 44.172 | 0.26x |
| users.json | json | 23.690 | 24.014 | 42.793 | 44.172 | 0.19x |
| flat.json | strata | 0.713 | 0.760 | 0.813 | 50.402 | 1.00x |
| flat.json | orjson | 0.559 | 0.616 | 0.656 | 50.402 | 1.23x |
| flat.json | msgspec | 0.734 | 0.789 | 0.842 | 50.402 | 0.96x |
| flat.json | ujson | 2.639 | 2.711 | 2.802 | 50.402 | 0.28x |
| flat.json | json | 3.193 | 3.453 | 4.255 | 50.402 | 0.22x |
| nested.json | strata | 0.608 | 0.629 | 0.748 | 50.023 | 1.00x |
| nested.json | orjson | 0.479 | 0.494 | 0.554 | 50.023 | 1.27x |
| nested.json | msgspec | 0.705 | 0.714 | 0.827 | 50.023 | 0.88x |
| nested.json | ujson | 2.222 | 2.256 | 2.415 | 50.023 | 0.28x |
| nested.json | json | 4.037 | 4.255 | 4.683 | 50.023 | 0.15x |
| wide_arrays.json | strata | 3.852 | 3.952 | 4.461 | 51.012 | 1.00x |
| wide_arrays.json | orjson | 3.672 | 3.799 | 3.916 | 51.012 | 1.04x |
| wide_arrays.json | msgspec | 6.119 | 6.276 | 6.590 | 51.012 | 0.63x |
| wide_arrays.json | ujson | 16.005 | 16.479 | 64.973 | 51.012 | 0.24x |
| wide_arrays.json | json | 31.690 | 32.280 | 81.152 | 51.012 | 0.12x |
| mixed.json | strata | 0.122 | 0.162 | 0.182 | 50.863 | 1.00x |
| mixed.json | orjson | 0.081 | 0.110 | 0.133 | 50.863 | 1.48x |
| mixed.json | msgspec | 0.102 | 0.130 | 0.167 | 50.863 | 1.25x |
| mixed.json | ujson | 0.445 | 0.509 | 0.675 | 50.863 | 0.32x |
| mixed.json | json | 0.672 | 0.860 | 1.057 | 50.863 | 0.19x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 13.854 | 14.711 | 17.084 | 54.203 | 1.00x |
| users.json | orjson | 14.919 | 15.633 | 16.868 | 54.203 | 0.94x |
| users.json | msgspec | 14.322 | 15.011 | 22.793 | 54.203 | 0.98x |
| users.json | ujson | 28.162 | 28.866 | 44.058 | 54.203 | 0.51x |
| users.json | json | 24.474 | 25.107 | 35.890 | 54.203 | 0.59x |
| flat.json | strata | 2.305 | 2.525 | 2.729 | 49.363 | 1.00x |
| flat.json | orjson | 2.090 | 2.235 | 3.144 | 49.363 | 1.13x |
| flat.json | msgspec | 2.022 | 2.181 | 2.933 | 49.363 | 1.16x |
| flat.json | ujson | 4.208 | 4.345 | 4.608 | 49.363 | 0.58x |
| flat.json | json | 3.784 | 3.916 | 50.711 | 49.363 | 0.64x |
| nested.json | strata | 1.900 | 2.041 | 2.536 | 49.629 | 1.00x |
| nested.json | orjson | 1.910 | 2.038 | 2.428 | 49.629 | 1.00x |
| nested.json | msgspec | 1.984 | 2.073 | 2.243 | 49.629 | 0.98x |
| nested.json | ujson | 3.465 | 3.663 | 3.860 | 49.629 | 0.56x |
| nested.json | json | 4.160 | 4.385 | 4.685 | 49.629 | 0.47x |
| wide_arrays.json | strata | 7.490 | 10.357 | 12.241 | 50.258 | 1.00x |
| wide_arrays.json | orjson | 6.831 | 9.607 | 11.783 | 50.258 | 1.08x |
| wide_arrays.json | msgspec | 6.924 | 9.923 | 25.368 | 50.258 | 1.04x |
| wide_arrays.json | ujson | 12.386 | 15.092 | 19.893 | 50.258 | 0.69x |
| wide_arrays.json | json | 13.475 | 18.812 | 23.247 | 50.258 | 0.55x |
| mixed.json | strata | 0.327 | 0.492 | 0.600 | 49.195 | 1.00x |
| mixed.json | orjson | 0.534 | 0.588 | 0.665 | 49.195 | 0.84x |
| mixed.json | msgspec | 0.405 | 0.581 | 0.652 | 49.195 | 0.85x |
| mixed.json | ujson | 0.756 | 0.879 | 0.984 | 49.195 | 0.56x |
| mixed.json | json | 0.794 | 0.992 | 1.172 | 49.195 | 0.50x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 25.674 | 26.400 | 75.220 | 50.617 | 1.00x |
| users.ndjson | orjson | 30.256 | 31.117 | 31.890 | 50.617 | 0.85x |
| users.ndjson | msgspec | 30.893 | 32.216 | 79.410 | 50.617 | 0.82x |
| users.ndjson | ujson | 45.021 | 46.562 | 93.795 | 50.617 | 0.57x |
| users.ndjson | json | 53.255 | 54.867 | 61.936 | 50.617 | 0.48x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 5.419 | 5.552 | 5.624 | 53.234 | 1.00x |
| users.json | orjson | 4.560 | 4.660 | 4.871 | 53.234 | 1.19x |
| users.json | msgspec | 6.003 | 6.328 | 8.693 | 53.234 | 0.88x |
| users.json | ujson | 25.768 | 26.374 | 38.453 | 53.234 | 0.21x |
| users.json | json | 32.456 | 33.236 | 38.129 | 53.234 | 0.17x |
| flat.json | strata | 1.400 | 1.530 | 1.669 | 49.938 | 1.00x |
| flat.json | orjson | 1.108 | 1.254 | 1.451 | 49.938 | 1.22x |
| flat.json | msgspec | 1.312 | 1.471 | 47.478 | 49.938 | 1.04x |
| flat.json | ujson | 4.826 | 5.009 | 5.440 | 49.938 | 0.31x |
| flat.json | json | 5.630 | 6.054 | 6.649 | 49.938 | 0.25x |
| nested.json | strata | 1.238 | 1.279 | 1.366 | 50.031 | 1.00x |
| nested.json | orjson | 0.992 | 1.068 | 1.455 | 50.031 | 1.20x |
| nested.json | msgspec | 1.207 | 1.315 | 2.421 | 50.031 | 0.97x |
| nested.json | ujson | 3.907 | 4.159 | 36.805 | 50.031 | 0.31x |
| nested.json | json | 5.970 | 6.142 | 10.013 | 50.031 | 0.21x |
| wide_arrays.json | strata | 3.624 | 4.451 | 5.094 | 51.930 | 1.00x |
| wide_arrays.json | orjson | 3.206 | 4.485 | 18.482 | 51.930 | 0.99x |
| wide_arrays.json | msgspec | 4.691 | 6.733 | 7.286 | 51.930 | 0.66x |
| wide_arrays.json | ujson | 17.622 | 24.799 | 54.865 | 51.930 | 0.18x |
| wide_arrays.json | json | 27.816 | 36.893 | 50.134 | 51.930 | 0.12x |
| mixed.json | strata | 0.649 | 0.726 | 8.906 | 49.316 | 1.00x |
| mixed.json | orjson | 0.523 | 0.559 | 0.932 | 49.316 | 1.30x |
| mixed.json | msgspec | 0.569 | 0.615 | 0.913 | 49.316 | 1.18x |
| mixed.json | ujson | 1.272 | 1.361 | 1.446 | 49.316 | 0.53x |
| mixed.json | json | 1.671 | 1.716 | 51.102 | 49.316 | 0.42x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.099 | 0.108 | 0.164 | 53.250 | 1.00x |
| users.json $[*].id | jmespath | 0.449 | 0.461 | 0.888 | 53.250 | 0.23x |
| users.json $[*].id | jsonpath-ng | 2.583 | 2.864 | 4.201 | 53.250 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.509 | 0.566 | 0.619 | 51.723 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.897 | 3.060 | 3.481 | 51.723 | 0.19x |
| users.json $[*].orders[*].total | jsonpath-ng | 18.142 | 18.991 | 19.638 | 51.723 | 0.03x |
| users.json $..total | strata | 1.987 | 2.068 | 2.430 | 52.691 | 1.00x |
| users.json $..total | jsonpath-ng | 332.208 | 355.961 | 383.361 | 52.691 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 5.522 | 5.608 | 9.344 | 51.406 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.577 | 15.727 | 17.459 | 51.406 | 0.36x |
| users.json $[*].id | orjson+jsonpath-ng | 17.313 | 18.513 | 24.586 | 51.406 | 0.30x |
| users.json $[*].orders[*].total | strata | 5.856 | 5.993 | 9.851 | 52.594 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.301 | 19.054 | 22.862 | 52.594 | 0.31x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 38.314 | 40.634 | 75.904 | 52.594 | 0.15x |
| users.json $..total | strata | 17.958 | 18.514 | 31.191 | 52.316 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 356.417 | 380.848 | 755.521 | 52.316 | 0.05x |

