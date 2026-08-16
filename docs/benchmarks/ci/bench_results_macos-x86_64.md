# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 6309d05
- python: 3.12.10
- implementation: CPython
- platform: macOS-15.7.7-x86_64-i386-64bit
- machine: x86_64
- processor: Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 24.673 | 26.842 | 28.264 | 52.879 | 1.00x |
| users.json | orjson | 33.109 | 34.508 | 37.595 | 52.879 | 0.78x |
| users.json | msgspec | 32.603 | 34.637 | 41.365 | 52.879 | 0.77x |
| users.json | ujson | 47.825 | 50.792 | 58.543 | 52.879 | 0.53x |
| users.json | pysimdjson | 204.568 | 222.140 | 228.341 | 52.879 | 0.12x |
| users.json | json | 56.246 | 58.581 | 66.081 | 52.879 | 0.46x |
| flat.json | strata | 1.519 | 1.546 | 1.558 | 61.883 | 1.00x |
| flat.json | orjson | 1.582 | 1.613 | 1.634 | 61.883 | 0.96x |
| flat.json | msgspec | 1.827 | 1.848 | 1.877 | 61.883 | 0.84x |
| flat.json | ujson | 3.180 | 3.224 | 4.407 | 61.883 | 0.48x |
| flat.json | pysimdjson | 17.309 | 17.528 | 17.829 | 61.883 | 0.09x |
| flat.json | json | 3.681 | 3.707 | 3.740 | 61.883 | 0.42x |
| nested.json | strata | 1.707 | 1.763 | 1.819 | 50.227 | 1.00x |
| nested.json | orjson | 1.905 | 1.993 | 2.057 | 50.227 | 0.88x |
| nested.json | msgspec | 2.114 | 2.196 | 2.270 | 50.227 | 0.80x |
| nested.json | ujson | 3.557 | 3.634 | 4.005 | 50.227 | 0.49x |
| nested.json | pysimdjson | 15.993 | 16.274 | 16.923 | 50.227 | 0.11x |
| nested.json | json | 4.561 | 4.682 | 4.829 | 50.227 | 0.38x |
| wide_arrays.json | strata | 9.822 | 10.360 | 10.917 | 61.523 | 1.00x |
| wide_arrays.json | orjson | 10.256 | 10.918 | 11.184 | 61.523 | 0.95x |
| wide_arrays.json | msgspec | 11.366 | 12.127 | 12.471 | 61.523 | 0.85x |
| wide_arrays.json | ujson | 14.763 | 16.118 | 16.533 | 61.523 | 0.64x |
| wide_arrays.json | pysimdjson | 91.119 | 97.975 | 99.558 | 61.523 | 0.11x |
| wide_arrays.json | json | 19.169 | 19.955 | 21.103 | 61.523 | 0.52x |
| mixed.json | strata | 0.404 | 0.409 | 0.651 | 60.281 | 1.00x |
| mixed.json | orjson | 0.471 | 0.476 | 0.504 | 60.281 | 0.86x |
| mixed.json | msgspec | 0.505 | 0.509 | 0.519 | 60.281 | 0.80x |
| mixed.json | ujson | 0.695 | 0.705 | 0.741 | 60.281 | 0.58x |
| mixed.json | pysimdjson | 3.593 | 3.643 | 3.768 | 60.281 | 0.11x |
| mixed.json | json | 0.977 | 0.983 | 1.015 | 60.281 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.002 | 4.275 | 6.033 | 48.559 | 1.00x |
| users.json | orjson | 4.459 | 4.688 | 7.313 | 48.559 | 0.91x |
| users.json | msgspec | 7.481 | 8.212 | 9.652 | 48.559 | 0.52x |
| users.json | ujson | 42.610 | 45.938 | 52.789 | 48.559 | 0.09x |
| users.json | json | 58.978 | 62.192 | 65.724 | 48.559 | 0.07x |
| flat.json | strata | 0.345 | 0.358 | 0.405 | 50.664 | 1.00x |
| flat.json | orjson | 0.427 | 0.452 | 0.640 | 50.664 | 0.79x |
| flat.json | msgspec | 0.584 | 0.610 | 0.675 | 50.664 | 0.59x |
| flat.json | ujson | 3.115 | 3.181 | 3.260 | 50.664 | 0.11x |
| flat.json | json | 4.255 | 4.305 | 5.389 | 50.664 | 0.08x |
| nested.json | strata | 0.291 | 0.299 | 0.308 | 49.965 | 1.00x |
| nested.json | orjson | 0.405 | 0.418 | 0.474 | 49.965 | 0.72x |
| nested.json | msgspec | 0.632 | 0.667 | 0.724 | 49.965 | 0.45x |
| nested.json | ujson | 3.284 | 3.323 | 3.629 | 49.965 | 0.09x |
| nested.json | json | 5.455 | 5.502 | 5.723 | 49.965 | 0.05x |
| wide_arrays.json | strata | 2.149 | 2.203 | 2.484 | 63.387 | 1.00x |
| wide_arrays.json | orjson | 2.635 | 2.726 | 3.129 | 63.387 | 0.81x |
| wide_arrays.json | msgspec | 3.545 | 3.652 | 3.969 | 63.387 | 0.60x |
| wide_arrays.json | ujson | 17.075 | 17.412 | 18.831 | 63.387 | 0.13x |
| wide_arrays.json | json | 39.112 | 39.387 | 39.950 | 63.387 | 0.06x |
| mixed.json | strata | 0.079 | 0.086 | 0.091 | 59.086 | 1.00x |
| mixed.json | orjson | 0.074 | 0.079 | 0.088 | 59.086 | 1.10x |
| mixed.json | msgspec | 0.109 | 0.115 | 0.138 | 59.086 | 0.75x |
| mixed.json | ujson | 0.676 | 0.685 | 0.699 | 59.086 | 0.13x |
| mixed.json | json | 1.053 | 1.066 | 1.088 | 59.086 | 0.08x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 26.106 | 27.650 | 29.317 | 61.023 | 1.00x |
| users.json | orjson | 34.528 | 38.027 | 44.722 | 61.023 | 0.73x |
| users.json | msgspec | 34.607 | 37.518 | 40.563 | 61.023 | 0.74x |
| users.json | ujson | 51.215 | 56.565 | 61.088 | 61.023 | 0.49x |
| users.json | json | 58.254 | 61.479 | 66.700 | 61.023 | 0.45x |
| flat.json | strata | 1.600 | 1.668 | 1.734 | 50.004 | 1.00x |
| flat.json | orjson | 1.693 | 1.768 | 1.836 | 50.004 | 0.94x |
| flat.json | msgspec | 1.935 | 1.992 | 2.120 | 50.004 | 0.84x |
| flat.json | ujson | 3.331 | 3.404 | 3.685 | 50.004 | 0.49x |
| flat.json | json | 3.788 | 3.905 | 4.306 | 50.004 | 0.43x |
| nested.json | strata | 1.826 | 1.865 | 2.303 | 49.965 | 1.00x |
| nested.json | orjson | 2.090 | 2.109 | 2.501 | 49.965 | 0.88x |
| nested.json | msgspec | 2.288 | 2.318 | 2.380 | 49.965 | 0.80x |
| nested.json | ujson | 3.737 | 3.775 | 4.107 | 49.965 | 0.49x |
| nested.json | json | 4.744 | 4.800 | 4.942 | 49.965 | 0.39x |
| wide_arrays.json | strata | 9.606 | 9.657 | 10.102 | 63.387 | 1.00x |
| wide_arrays.json | orjson | 10.023 | 10.109 | 10.359 | 63.387 | 0.96x |
| wide_arrays.json | msgspec | 11.206 | 11.339 | 11.674 | 63.387 | 0.85x |
| wide_arrays.json | ujson | 14.703 | 14.848 | 15.418 | 63.387 | 0.65x |
| wide_arrays.json | json | 18.653 | 18.877 | 19.370 | 63.387 | 0.51x |
| mixed.json | strata | 0.483 | 0.496 | 0.521 | 59.086 | 1.00x |
| mixed.json | orjson | 0.584 | 0.599 | 0.619 | 59.086 | 0.83x |
| mixed.json | msgspec | 0.612 | 0.627 | 0.673 | 59.086 | 0.79x |
| mixed.json | ujson | 0.810 | 0.840 | 0.929 | 59.086 | 0.59x |
| mixed.json | json | 1.084 | 1.095 | 1.176 | 59.086 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 23.216 | 23.819 | 26.281 | 62.223 | 1.00x |
| users.ndjson | orjson | 31.139 | 31.964 | 35.133 | 62.223 | 0.75x |
| users.ndjson | msgspec | 31.096 | 32.160 | 35.413 | 62.223 | 0.74x |
| users.ndjson | ujson | 45.503 | 46.544 | 51.880 | 62.223 | 0.51x |
| users.ndjson | json | 57.171 | 58.784 | 63.350 | 62.223 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.744 | 5.032 | 5.187 | 58.059 | 1.00x |
| users.json | orjson | 5.283 | 5.675 | 6.839 | 58.059 | 0.89x |
| users.json | msgspec | 7.409 | 7.831 | 9.319 | 58.059 | 0.64x |
| users.json | ujson | 41.013 | 42.963 | 43.941 | 58.059 | 0.12x |
| users.json | json | 53.742 | 56.282 | 58.798 | 58.059 | 0.09x |
| flat.json | strata | 0.715 | 0.766 | 0.909 | 50.004 | 1.00x |
| flat.json | orjson | 0.796 | 0.859 | 0.953 | 50.004 | 0.89x |
| flat.json | msgspec | 0.953 | 1.009 | 1.119 | 50.004 | 0.76x |
| flat.json | ujson | 3.496 | 3.587 | 3.716 | 50.004 | 0.21x |
| flat.json | json | 4.620 | 4.816 | 5.163 | 50.004 | 0.16x |
| nested.json | strata | 0.586 | 0.632 | 0.732 | 49.965 | 1.00x |
| nested.json | orjson | 0.694 | 0.758 | 0.863 | 49.965 | 0.83x |
| nested.json | msgspec | 0.968 | 0.999 | 1.109 | 49.965 | 0.63x |
| nested.json | ujson | 3.643 | 3.714 | 3.888 | 49.965 | 0.17x |
| nested.json | json | 5.879 | 5.932 | 5.999 | 49.965 | 0.11x |
| wide_arrays.json | strata | 2.815 | 3.023 | 3.409 | 63.387 | 1.00x |
| wide_arrays.json | orjson | 3.299 | 3.562 | 4.418 | 63.387 | 0.85x |
| wide_arrays.json | msgspec | 4.243 | 4.502 | 4.670 | 63.387 | 0.67x |
| wide_arrays.json | ujson | 17.847 | 18.337 | 20.149 | 63.387 | 0.16x |
| wide_arrays.json | json | 38.992 | 40.130 | 47.351 | 63.387 | 0.08x |
| mixed.json | strata | 0.298 | 0.356 | 0.615 | 59.086 | 1.00x |
| mixed.json | orjson | 0.306 | 0.372 | 0.453 | 59.086 | 0.96x |
| mixed.json | msgspec | 0.340 | 0.419 | 0.497 | 59.086 | 0.85x |
| mixed.json | ujson | 0.951 | 1.022 | 1.186 | 59.086 | 0.35x |
| mixed.json | json | 1.329 | 1.416 | 1.639 | 59.086 | 0.25x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.164 | 0.186 | 0.203 | 58.117 | 1.00x |
| users.json $[*].id | jmespath | 1.177 | 1.225 | 1.265 | 58.117 | 0.15x |
| users.json $[*].id | jsonpath-ng | 6.489 | 6.616 | 6.757 | 58.117 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.895 | 1.079 | 1.164 | 55.246 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 6.417 | 6.685 | 7.100 | 55.246 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 38.565 | 39.369 | 41.503 | 55.246 | 0.03x |
| users.json $..total | strata | 3.953 | 4.209 | 4.580 | 56.273 | 1.00x |
| users.json $..total | jsonpath-ng | 818.812 | 855.629 | 1062.407 | 56.273 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.766 | 4.898 | 5.053 | 58.164 | 1.00x |
| users.json $[*].id | orjson+jmespath | 29.993 | 31.417 | 33.533 | 58.164 | 0.16x |
| users.json $[*].id | orjson+jsonpath-ng | 35.014 | 36.018 | 37.480 | 58.164 | 0.14x |
| users.json $[*].orders[*].total | strata | 4.880 | 4.934 | 5.199 | 56.250 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 34.914 | 35.309 | 37.542 | 56.250 | 0.14x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 71.971 | 73.498 | 83.072 | 56.250 | 0.07x |
| users.json $..total | strata | 25.580 | 27.205 | 29.429 | 56.273 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 800.918 | 853.327 | 884.192 | 56.273 | 0.03x |

