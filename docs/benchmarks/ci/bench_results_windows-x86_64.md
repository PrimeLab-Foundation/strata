# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c88786c
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
| users.json | strata | 7.954 | 8.235 | 10.806 | 45.453 | 1.00x |
| users.json | orjson | 11.117 | 11.438 | 13.069 | 45.453 | 0.72x |
| users.json | msgspec | 10.089 | 10.245 | 12.967 | 45.453 | 0.80x |
| users.json | ujson | 16.412 | 17.069 | 19.882 | 45.453 | 0.48x |
| users.json | json | 17.622 | 18.695 | 27.883 | 45.453 | 0.44x |
| flat.json | strata | 0.864 | 0.895 | 0.936 | 48.672 | 1.00x |
| flat.json | orjson | 0.948 | 0.986 | 1.024 | 48.672 | 0.91x |
| flat.json | msgspec | 0.866 | 0.896 | 1.226 | 48.672 | 1.00x |
| flat.json | ujson | 1.713 | 1.742 | 2.104 | 48.672 | 0.51x |
| flat.json | json | 1.481 | 1.521 | 1.592 | 48.672 | 0.59x |
| nested.json | strata | 0.616 | 0.622 | 0.723 | 48.594 | 1.00x |
| nested.json | orjson | 0.827 | 0.881 | 0.902 | 48.594 | 0.71x |
| nested.json | msgspec | 0.748 | 0.764 | 0.790 | 48.594 | 0.81x |
| nested.json | ujson | 1.174 | 1.215 | 1.250 | 48.594 | 0.51x |
| nested.json | json | 1.577 | 1.610 | 1.621 | 48.594 | 0.39x |
| wide_arrays.json | strata | 3.715 | 3.774 | 4.380 | 50.691 | 1.00x |
| wide_arrays.json | orjson | 4.628 | 4.814 | 5.123 | 50.691 | 0.78x |
| wide_arrays.json | msgspec | 4.544 | 4.660 | 7.362 | 50.691 | 0.81x |
| wide_arrays.json | ujson | 6.327 | 6.435 | 10.629 | 50.691 | 0.59x |
| wide_arrays.json | json | 8.796 | 9.025 | 14.692 | 50.691 | 0.42x |
| mixed.json | strata | 0.151 | 0.153 | 0.178 | 48.574 | 1.00x |
| mixed.json | orjson | 0.164 | 0.169 | 0.237 | 48.574 | 0.90x |
| mixed.json | msgspec | 0.176 | 0.180 | 0.200 | 48.574 | 0.85x |
| mixed.json | ujson | 0.267 | 0.273 | 0.294 | 48.574 | 0.56x |
| mixed.json | json | 0.351 | 0.359 | 0.397 | 48.574 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.650 | 2.692 | 4.015 | 44.688 | 1.00x |
| users.json | orjson | 2.695 | 2.734 | 2.794 | 44.688 | 0.98x |
| users.json | msgspec | 4.415 | 4.477 | 4.586 | 44.688 | 0.60x |
| users.json | ujson | 12.338 | 12.613 | 13.855 | 44.688 | 0.21x |
| users.json | json | 18.536 | 18.679 | 25.816 | 44.688 | 0.14x |
| flat.json | strata | 0.257 | 0.267 | 0.358 | 48.820 | 1.00x |
| flat.json | orjson | 0.255 | 0.258 | 0.295 | 48.820 | 1.04x |
| flat.json | msgspec | 0.421 | 0.424 | 0.449 | 48.820 | 0.63x |
| flat.json | ujson | 1.262 | 1.282 | 1.327 | 48.820 | 0.21x |
| flat.json | json | 1.504 | 1.536 | 1.744 | 48.820 | 0.17x |
| nested.json | strata | 0.219 | 0.234 | 0.331 | 49.070 | 1.00x |
| nested.json | orjson | 0.236 | 0.246 | 0.350 | 49.070 | 0.95x |
| nested.json | msgspec | 0.395 | 0.398 | 0.601 | 49.070 | 0.59x |
| nested.json | ujson | 1.027 | 1.048 | 1.658 | 49.070 | 0.22x |
| nested.json | json | 2.013 | 2.027 | 3.368 | 49.070 | 0.12x |
| wide_arrays.json | strata | 1.865 | 1.891 | 2.836 | 50.668 | 1.00x |
| wide_arrays.json | orjson | 2.018 | 2.043 | 2.781 | 50.668 | 0.93x |
| wide_arrays.json | msgspec | 3.486 | 3.542 | 4.727 | 50.668 | 0.53x |
| wide_arrays.json | ujson | 7.808 | 7.898 | 7.989 | 50.668 | 0.24x |
| wide_arrays.json | json | 14.005 | 14.273 | 20.025 | 50.668 | 0.13x |
| mixed.json | strata | 0.059 | 0.061 | 0.085 | 48.645 | 1.00x |
| mixed.json | orjson | 0.050 | 0.051 | 0.071 | 48.645 | 1.20x |
| mixed.json | msgspec | 0.075 | 0.077 | 0.104 | 48.645 | 0.80x |
| mixed.json | ujson | 0.254 | 0.257 | 0.300 | 48.645 | 0.24x |
| mixed.json | json | 0.397 | 0.406 | 0.443 | 48.645 | 0.15x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.562 | 10.279 | 12.111 | 54.723 | 1.00x |
| users.json | orjson | 12.284 | 13.226 | 19.219 | 54.723 | 0.78x |
| users.json | msgspec | 11.256 | 11.699 | 13.988 | 54.723 | 0.88x |
| users.json | ujson | 20.427 | 21.184 | 29.780 | 54.723 | 0.49x |
| users.json | json | 18.802 | 19.213 | 29.830 | 54.723 | 0.53x |
| flat.json | strata | 1.130 | 1.178 | 1.922 | 48.363 | 1.00x |
| flat.json | orjson | 1.200 | 1.268 | 1.775 | 48.363 | 0.93x |
| flat.json | msgspec | 0.992 | 1.092 | 1.577 | 48.363 | 1.08x |
| flat.json | ujson | 2.215 | 2.326 | 3.282 | 48.363 | 0.51x |
| flat.json | json | 1.765 | 1.807 | 2.709 | 48.363 | 0.65x |
| nested.json | strata | 0.725 | 0.748 | 0.833 | 48.738 | 1.00x |
| nested.json | orjson | 0.994 | 1.043 | 1.147 | 48.738 | 0.72x |
| nested.json | msgspec | 0.885 | 0.916 | 0.973 | 48.738 | 0.82x |
| nested.json | ujson | 1.474 | 1.536 | 1.597 | 48.738 | 0.49x |
| nested.json | json | 1.751 | 1.776 | 1.846 | 48.738 | 0.42x |
| wide_arrays.json | strata | 4.322 | 4.517 | 7.147 | 50.668 | 1.00x |
| wide_arrays.json | orjson | 5.225 | 5.447 | 5.973 | 50.668 | 0.83x |
| wide_arrays.json | msgspec | 5.281 | 5.382 | 6.181 | 50.668 | 0.84x |
| wide_arrays.json | ujson | 8.580 | 8.698 | 9.548 | 50.668 | 0.52x |
| wide_arrays.json | json | 9.450 | 9.697 | 16.623 | 50.668 | 0.47x |
| mixed.json | strata | 0.226 | 0.233 | 0.255 | 48.707 | 1.00x |
| mixed.json | orjson | 0.265 | 0.302 | 0.332 | 48.707 | 0.77x |
| mixed.json | msgspec | 0.280 | 0.284 | 0.416 | 48.707 | 0.82x |
| mixed.json | ujson | 0.409 | 0.427 | 0.459 | 48.707 | 0.55x |
| mixed.json | json | 0.458 | 0.473 | 0.661 | 48.707 | 0.49x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.580 | 10.623 | 11.647 | 49.738 | 1.00x |
| users.ndjson | orjson | 14.841 | 15.611 | 21.287 | 49.738 | 0.68x |
| users.ndjson | msgspec | 14.276 | 14.856 | 20.545 | 49.738 | 0.72x |
| users.ndjson | ujson | 20.855 | 21.460 | 22.909 | 49.738 | 0.50x |
| users.ndjson | json | 24.450 | 25.451 | 38.404 | 49.738 | 0.42x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.485 | 3.592 | 6.042 | 51.773 | 1.00x |
| users.json | orjson | 3.454 | 3.538 | 3.759 | 51.773 | 1.02x |
| users.json | msgspec | 5.208 | 5.332 | 6.979 | 51.773 | 0.67x |
| users.json | ujson | 19.710 | 19.941 | 28.108 | 51.773 | 0.18x |
| users.json | json | 25.212 | 25.524 | 30.537 | 51.773 | 0.14x |
| flat.json | strata | 0.546 | 0.593 | 0.681 | 49.375 | 1.00x |
| flat.json | orjson | 0.555 | 0.573 | 0.623 | 49.375 | 1.04x |
| flat.json | msgspec | 0.735 | 0.777 | 0.860 | 49.375 | 0.76x |
| flat.json | ujson | 2.360 | 2.451 | 2.593 | 49.375 | 0.24x |
| flat.json | json | 2.633 | 2.791 | 2.962 | 49.375 | 0.21x |
| nested.json | strata | 0.545 | 0.575 | 0.635 | 49.059 | 1.00x |
| nested.json | orjson | 0.555 | 0.571 | 0.609 | 49.059 | 1.01x |
| nested.json | msgspec | 0.711 | 0.745 | 0.790 | 49.059 | 0.77x |
| nested.json | ujson | 1.868 | 1.913 | 1.945 | 49.059 | 0.30x |
| nested.json | json | 2.894 | 2.923 | 2.980 | 49.059 | 0.20x |
| wide_arrays.json | strata | 2.550 | 2.586 | 2.690 | 50.668 | 1.00x |
| wide_arrays.json | orjson | 2.591 | 2.678 | 2.857 | 50.668 | 0.97x |
| wide_arrays.json | msgspec | 4.086 | 4.153 | 4.299 | 50.668 | 0.62x |
| wide_arrays.json | ujson | 13.103 | 13.360 | 21.625 | 50.668 | 0.19x |
| wide_arrays.json | json | 19.412 | 19.597 | 27.946 | 50.668 | 0.13x |
| mixed.json | strata | 0.304 | 0.319 | 0.406 | 48.777 | 1.00x |
| mixed.json | orjson | 0.286 | 0.305 | 0.323 | 48.777 | 1.04x |
| mixed.json | msgspec | 0.322 | 0.336 | 0.380 | 48.777 | 0.95x |
| mixed.json | ujson | 0.630 | 0.652 | 0.899 | 48.777 | 0.49x |
| mixed.json | json | 0.786 | 0.807 | 0.888 | 48.777 | 0.40x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.084 | 0.090 | 0.103 | 51.789 | 1.00x |
| users.json $[*].id | jmespath | 0.349 | 0.354 | 0.365 | 51.789 | 0.25x |
| users.json $[*].id | jsonpath-ng | 1.969 | 2.245 | 2.276 | 51.789 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.407 | 0.420 | 0.444 | 51.797 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.111 | 2.152 | 2.470 | 51.797 | 0.20x |
| users.json $[*].orders[*].total | jsonpath-ng | 12.956 | 13.487 | 14.078 | 51.797 | 0.03x |
| users.json $..total | strata | 1.428 | 1.450 | 1.463 | 52.426 | 1.00x |
| users.json $..total | jsonpath-ng | 252.449 | 266.653 | 279.705 | 52.426 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.485 | 3.547 | 3.701 | 51.797 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.831 | 12.983 | 13.602 | 51.797 | 0.27x |
| users.json $[*].id | orjson+jsonpath-ng | 14.574 | 14.951 | 23.109 | 51.797 | 0.24x |
| users.json $[*].orders[*].total | strata | 3.750 | 3.809 | 3.976 | 52.422 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.543 | 16.255 | 26.454 | 52.422 | 0.23x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 29.343 | 32.214 | 35.817 | 52.422 | 0.12x |
| users.json $..total | strata | 11.920 | 12.525 | 14.246 | 52.426 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 270.388 | 277.959 | 293.421 | 52.426 | 0.05x |

