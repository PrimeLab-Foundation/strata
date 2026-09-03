# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 74164cd
- python: 3.12.10
- implementation: CPython
- platform: macOS-15.7.9-x86_64-i386-64bit
- machine: x86_64
- processor: Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 19.973 | 21.996 | 26.973 | 52.621 | 1.00x |
| users.json | orjson | 29.686 | 33.814 | 40.631 | 52.621 | 0.65x |
| users.json | msgspec | 30.050 | 32.700 | 40.393 | 52.621 | 0.67x |
| users.json | ujson | 41.072 | 45.647 | 53.967 | 52.621 | 0.48x |
| users.json | pysimdjson | 173.681 | 180.352 | 205.991 | 52.621 | 0.12x |
| users.json | json | 47.235 | 49.616 | 62.224 | 52.621 | 0.44x |
| flat.json | strata | 1.320 | 1.434 | 1.763 | 61.219 | 1.00x |
| flat.json | orjson | 1.426 | 1.547 | 1.884 | 61.219 | 0.93x |
| flat.json | msgspec | 1.613 | 1.733 | 2.687 | 61.219 | 0.83x |
| flat.json | ujson | 2.886 | 2.962 | 3.319 | 61.219 | 0.48x |
| flat.json | pysimdjson | 14.706 | 15.728 | 41.002 | 61.219 | 0.09x |
| flat.json | json | 3.177 | 3.462 | 4.646 | 61.219 | 0.41x |
| nested.json | strata | 1.572 | 1.728 | 1.905 | 58.477 | 1.00x |
| nested.json | orjson | 1.767 | 1.890 | 2.298 | 58.477 | 0.91x |
| nested.json | msgspec | 2.072 | 2.220 | 2.839 | 58.477 | 0.78x |
| nested.json | ujson | 3.284 | 3.580 | 4.236 | 58.477 | 0.48x |
| nested.json | pysimdjson | 13.887 | 14.186 | 19.757 | 58.477 | 0.12x |
| nested.json | json | 4.138 | 4.265 | 5.375 | 58.477 | 0.41x |
| wide_arrays.json | strata | 8.769 | 9.335 | 13.612 | 62.762 | 1.00x |
| wide_arrays.json | orjson | 9.478 | 10.766 | 16.100 | 62.762 | 0.87x |
| wide_arrays.json | msgspec | 10.252 | 11.684 | 15.168 | 62.762 | 0.80x |
| wide_arrays.json | ujson | 13.851 | 15.980 | 18.331 | 62.762 | 0.58x |
| wide_arrays.json | pysimdjson | 78.337 | 89.224 | 151.787 | 62.762 | 0.10x |
| wide_arrays.json | json | 17.458 | 18.956 | 26.773 | 62.762 | 0.49x |
| mixed.json | strata | 0.401 | 0.493 | 0.606 | 59.777 | 1.00x |
| mixed.json | orjson | 0.472 | 0.606 | 0.705 | 59.777 | 0.81x |
| mixed.json | msgspec | 0.521 | 0.603 | 0.856 | 59.777 | 0.82x |
| mixed.json | ujson | 0.687 | 0.872 | 1.058 | 59.777 | 0.57x |
| mixed.json | pysimdjson | 3.408 | 4.018 | 4.551 | 59.777 | 0.12x |
| mixed.json | json | 0.962 | 1.182 | 1.445 | 59.777 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.030 | 3.176 | 3.468 | 48.438 | 1.00x |
| users.json | orjson | 3.688 | 3.777 | 3.984 | 48.438 | 0.84x |
| users.json | msgspec | 6.239 | 6.455 | 7.197 | 48.438 | 0.49x |
| users.json | ujson | 32.917 | 33.815 | 36.244 | 48.438 | 0.09x |
| users.json | json | 43.257 | 45.737 | 49.067 | 48.438 | 0.07x |
| flat.json | strata | 0.354 | 0.398 | 0.461 | 58.340 | 1.00x |
| flat.json | orjson | 0.451 | 0.551 | 0.742 | 58.340 | 0.72x |
| flat.json | msgspec | 0.583 | 0.620 | 0.760 | 58.340 | 0.64x |
| flat.json | ujson | 2.761 | 3.212 | 4.146 | 58.340 | 0.12x |
| flat.json | json | 3.836 | 4.689 | 5.120 | 58.340 | 0.08x |
| nested.json | strata | 0.274 | 0.298 | 0.343 | 53.379 | 1.00x |
| nested.json | orjson | 0.366 | 0.390 | 0.477 | 53.379 | 0.76x |
| nested.json | msgspec | 0.576 | 0.595 | 0.643 | 53.379 | 0.50x |
| nested.json | ujson | 2.851 | 2.974 | 4.278 | 53.379 | 0.10x |
| nested.json | json | 4.691 | 4.782 | 5.395 | 53.379 | 0.06x |
| wide_arrays.json | strata | 2.041 | 2.242 | 2.392 | 61.285 | 1.00x |
| wide_arrays.json | orjson | 2.612 | 2.802 | 3.072 | 61.285 | 0.80x |
| wide_arrays.json | msgspec | 3.228 | 3.786 | 4.784 | 61.285 | 0.59x |
| wide_arrays.json | ujson | 15.197 | 17.218 | 19.357 | 61.285 | 0.13x |
| wide_arrays.json | json | 35.311 | 36.635 | 38.185 | 61.285 | 0.06x |
| mixed.json | strata | 0.096 | 0.121 | 0.140 | 56.457 | 1.00x |
| mixed.json | orjson | 0.099 | 0.134 | 0.165 | 56.457 | 0.90x |
| mixed.json | msgspec | 0.140 | 0.179 | 0.266 | 56.457 | 0.68x |
| mixed.json | ujson | 0.660 | 0.802 | 1.005 | 56.457 | 0.15x |
| mixed.json | json | 1.042 | 1.355 | 1.653 | 56.457 | 0.09x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 19.653 | 20.913 | 24.153 | 60.980 | 1.00x |
| users.json | orjson | 29.122 | 31.047 | 32.724 | 60.980 | 0.67x |
| users.json | msgspec | 30.230 | 33.503 | 36.282 | 60.980 | 0.62x |
| users.json | ujson | 41.452 | 44.406 | 50.635 | 60.980 | 0.47x |
| users.json | json | 47.054 | 50.964 | 56.955 | 60.980 | 0.41x |
| flat.json | strata | 1.495 | 1.653 | 2.054 | 58.340 | 1.00x |
| flat.json | orjson | 1.625 | 1.853 | 2.205 | 58.340 | 0.89x |
| flat.json | msgspec | 1.896 | 2.057 | 2.341 | 58.340 | 0.80x |
| flat.json | ujson | 3.138 | 3.492 | 4.321 | 58.340 | 0.47x |
| flat.json | json | 3.447 | 3.742 | 5.296 | 58.340 | 0.44x |
| nested.json | strata | 1.537 | 1.574 | 1.682 | 53.379 | 1.00x |
| nested.json | orjson | 1.767 | 1.816 | 13.914 | 53.379 | 0.87x |
| nested.json | msgspec | 2.031 | 2.119 | 2.487 | 53.379 | 0.74x |
| nested.json | ujson | 3.183 | 3.267 | 3.426 | 53.379 | 0.48x |
| nested.json | json | 3.925 | 3.955 | 4.343 | 53.379 | 0.40x |
| wide_arrays.json | strata | 8.831 | 9.602 | 13.736 | 61.285 | 1.00x |
| wide_arrays.json | orjson | 9.716 | 10.613 | 13.975 | 61.285 | 0.90x |
| wide_arrays.json | msgspec | 10.612 | 11.109 | 15.742 | 61.285 | 0.86x |
| wide_arrays.json | ujson | 13.758 | 15.649 | 18.431 | 61.285 | 0.61x |
| wide_arrays.json | json | 17.364 | 19.533 | 25.633 | 61.285 | 0.49x |
| mixed.json | strata | 0.505 | 0.530 | 0.686 | 56.457 | 1.00x |
| mixed.json | orjson | 0.606 | 0.727 | 0.925 | 56.457 | 0.73x |
| mixed.json | msgspec | 0.660 | 0.822 | 1.067 | 56.457 | 0.64x |
| mixed.json | ujson | 0.885 | 1.118 | 1.441 | 56.457 | 0.47x |
| mixed.json | json | 1.090 | 1.251 | 1.681 | 56.457 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 23.034 | 25.733 | 29.622 | 61.559 | 1.00x |
| users.ndjson | orjson | 29.938 | 38.776 | 44.894 | 61.559 | 0.66x |
| users.ndjson | msgspec | 30.954 | 38.490 | 82.737 | 61.559 | 0.67x |
| users.ndjson | ujson | 49.815 | 59.479 | 155.088 | 61.559 | 0.43x |
| users.ndjson | json | 61.204 | 68.380 | 245.558 | 61.559 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.915 | 4.675 | 5.543 | 55.465 | 1.00x |
| users.json | orjson | 4.613 | 5.751 | 7.268 | 55.465 | 0.81x |
| users.json | msgspec | 6.855 | 8.286 | 11.614 | 55.465 | 0.56x |
| users.json | ujson | 33.852 | 37.636 | 59.827 | 55.465 | 0.12x |
| users.json | json | 46.046 | 51.871 | 61.450 | 55.465 | 0.09x |
| flat.json | strata | 0.748 | 0.982 | 2.091 | 58.340 | 1.00x |
| flat.json | orjson | 0.893 | 1.062 | 5.028 | 58.340 | 0.92x |
| flat.json | msgspec | 1.088 | 1.369 | 3.476 | 58.340 | 0.72x |
| flat.json | ujson | 3.530 | 3.811 | 13.332 | 58.340 | 0.26x |
| flat.json | json | 4.218 | 5.363 | 14.285 | 58.340 | 0.18x |
| nested.json | strata | 0.587 | 0.614 | 0.668 | 53.379 | 1.00x |
| nested.json | orjson | 0.717 | 0.741 | 0.954 | 53.379 | 0.83x |
| nested.json | msgspec | 0.918 | 0.967 | 1.076 | 53.379 | 0.64x |
| nested.json | ujson | 3.118 | 3.231 | 3.321 | 53.379 | 0.19x |
| nested.json | json | 4.870 | 5.087 | 5.331 | 53.379 | 0.12x |
| wide_arrays.json | strata | 2.703 | 2.855 | 5.634 | 61.285 | 1.00x |
| wide_arrays.json | orjson | 3.334 | 3.834 | 6.701 | 61.285 | 0.74x |
| wide_arrays.json | msgspec | 4.194 | 4.507 | 7.093 | 61.285 | 0.63x |
| wide_arrays.json | ujson | 16.372 | 17.460 | 24.914 | 61.285 | 0.16x |
| wide_arrays.json | json | 35.292 | 39.529 | 59.256 | 61.285 | 0.07x |
| mixed.json | strata | 0.405 | 0.458 | 0.548 | 56.457 | 1.00x |
| mixed.json | orjson | 0.447 | 0.539 | 0.656 | 56.457 | 0.85x |
| mixed.json | msgspec | 0.433 | 0.553 | 0.611 | 56.457 | 0.83x |
| mixed.json | ujson | 1.053 | 1.122 | 1.413 | 56.457 | 0.41x |
| mixed.json | json | 1.409 | 1.659 | 2.004 | 56.457 | 0.28x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.198 | 0.210 | 0.235 | 55.535 | 1.00x |
| users.json $[*].id | jmespath | 1.041 | 1.074 | 1.286 | 55.535 | 0.20x |
| users.json $[*].id | jsonpath-ng | 5.647 | 5.833 | 7.034 | 55.535 | 0.04x |
| users.json $[*].orders[*].total | strata | 1.184 | 1.322 | 1.451 | 55.375 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 6.445 | 6.751 | 7.646 | 55.375 | 0.20x |
| users.json $[*].orders[*].total | jsonpath-ng | 38.145 | 39.444 | 43.732 | 55.375 | 0.03x |
| users.json $..total | strata | 3.614 | 3.861 | 5.784 | 57.500 | 1.00x |
| users.json $..total | jsonpath-ng | 746.663 | 787.247 | 870.636 | 57.500 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.931 | 4.195 | 4.902 | 59.578 | 1.00x |
| users.json $[*].id | orjson+jmespath | 29.213 | 31.375 | 34.563 | 59.578 | 0.13x |
| users.json $[*].id | orjson+jsonpath-ng | 34.493 | 37.067 | 40.339 | 59.578 | 0.11x |
| users.json $[*].orders[*].total | strata | 4.294 | 4.444 | 6.186 | 56.656 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 35.198 | 37.631 | 41.715 | 56.656 | 0.12x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 76.623 | 81.543 | 98.455 | 56.656 | 0.05x |
| users.json $..total | strata | 22.963 | 24.844 | 56.547 | 56.754 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 720.011 | 800.246 | 925.831 | 56.754 | 0.03x |

