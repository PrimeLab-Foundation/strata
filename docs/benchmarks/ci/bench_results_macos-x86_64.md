# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 59554e7
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
| users.json | strata | 19.263 | 20.683 | 54.597 | 52.547 | 1.00x |
| users.json | orjson | 26.193 | 32.317 | 85.251 | 52.547 | 0.64x |
| users.json | msgspec | 26.876 | 30.650 | 64.245 | 52.547 | 0.67x |
| users.json | ujson | 37.774 | 42.088 | 69.610 | 52.547 | 0.49x |
| users.json | pysimdjson | 162.921 | 179.206 | 304.415 | 52.547 | 0.12x |
| users.json | json | 42.665 | 49.485 | 121.638 | 52.547 | 0.42x |
| flat.json | strata | 1.345 | 1.513 | 2.395 | 61.246 | 1.00x |
| flat.json | orjson | 1.397 | 1.628 | 1.854 | 61.246 | 0.93x |
| flat.json | msgspec | 1.606 | 1.662 | 2.225 | 61.246 | 0.91x |
| flat.json | ujson | 2.777 | 3.117 | 3.563 | 61.246 | 0.49x |
| flat.json | pysimdjson | 14.569 | 16.125 | 24.983 | 61.246 | 0.09x |
| flat.json | json | 3.149 | 3.553 | 11.053 | 61.246 | 0.43x |
| nested.json | strata | 1.337 | 1.493 | 1.581 | 56.547 | 1.00x |
| nested.json | orjson | 1.517 | 1.680 | 2.144 | 56.547 | 0.89x |
| nested.json | msgspec | 1.685 | 1.812 | 1.907 | 56.547 | 0.82x |
| nested.json | ujson | 2.701 | 3.033 | 3.250 | 56.547 | 0.49x |
| nested.json | pysimdjson | 12.605 | 13.394 | 14.773 | 56.547 | 0.11x |
| nested.json | json | 3.591 | 3.868 | 4.345 | 56.547 | 0.39x |
| wide_arrays.json | strata | 7.976 | 8.517 | 10.964 | 60.824 | 1.00x |
| wide_arrays.json | orjson | 8.339 | 9.351 | 11.700 | 60.824 | 0.91x |
| wide_arrays.json | msgspec | 9.171 | 9.936 | 12.206 | 60.824 | 0.86x |
| wide_arrays.json | ujson | 12.305 | 13.175 | 15.595 | 60.824 | 0.65x |
| wide_arrays.json | pysimdjson | 78.030 | 78.793 | 85.141 | 60.824 | 0.11x |
| wide_arrays.json | json | 16.368 | 17.045 | 21.192 | 60.824 | 0.50x |
| mixed.json | strata | 0.338 | 0.384 | 0.459 | 58.051 | 1.00x |
| mixed.json | orjson | 0.395 | 0.450 | 0.469 | 58.051 | 0.85x |
| mixed.json | msgspec | 0.422 | 0.437 | 0.471 | 58.051 | 0.88x |
| mixed.json | ujson | 0.578 | 0.642 | 0.682 | 58.051 | 0.60x |
| mixed.json | pysimdjson | 3.009 | 3.258 | 4.568 | 58.051 | 0.12x |
| mixed.json | json | 0.827 | 0.914 | 0.945 | 58.051 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.558 | 2.789 | 3.434 | 51.340 | 1.00x |
| users.json | orjson | 3.280 | 3.514 | 4.297 | 51.340 | 0.79x |
| users.json | msgspec | 4.989 | 5.371 | 5.917 | 51.340 | 0.52x |
| users.json | ujson | 30.504 | 31.774 | 34.070 | 51.340 | 0.09x |
| users.json | json | 39.371 | 42.391 | 43.166 | 51.340 | 0.07x |
| flat.json | strata | 0.324 | 0.334 | 0.624 | 56.414 | 1.00x |
| flat.json | orjson | 0.419 | 0.435 | 0.702 | 56.414 | 0.77x |
| flat.json | msgspec | 0.552 | 0.564 | 0.973 | 56.414 | 0.59x |
| flat.json | ujson | 2.688 | 2.750 | 2.821 | 56.414 | 0.12x |
| flat.json | json | 3.689 | 3.762 | 3.931 | 56.414 | 0.09x |
| nested.json | strata | 0.213 | 0.251 | 0.277 | 56.684 | 1.00x |
| nested.json | orjson | 0.315 | 0.342 | 0.607 | 56.684 | 0.73x |
| nested.json | msgspec | 0.523 | 0.556 | 0.598 | 56.684 | 0.45x |
| nested.json | ujson | 2.617 | 2.792 | 2.931 | 56.684 | 0.09x |
| nested.json | json | 4.335 | 4.603 | 5.750 | 56.684 | 0.05x |
| wide_arrays.json | strata | 1.975 | 2.255 | 2.593 | 58.922 | 1.00x |
| wide_arrays.json | orjson | 2.475 | 3.158 | 4.334 | 58.922 | 0.71x |
| wide_arrays.json | msgspec | 3.116 | 3.575 | 4.692 | 58.922 | 0.63x |
| wide_arrays.json | ujson | 15.415 | 17.232 | 22.470 | 58.922 | 0.13x |
| wide_arrays.json | json | 34.544 | 37.496 | 44.528 | 58.922 | 0.06x |
| mixed.json | strata | 0.093 | 0.097 | 0.138 | 55.914 | 1.00x |
| mixed.json | orjson | 0.071 | 0.077 | 0.087 | 55.914 | 1.26x |
| mixed.json | msgspec | 0.106 | 0.111 | 0.320 | 55.914 | 0.88x |
| mixed.json | ujson | 0.613 | 0.632 | 0.653 | 55.914 | 0.15x |
| mixed.json | json | 0.905 | 0.982 | 1.025 | 55.914 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 19.449 | 20.118 | 30.593 | 61.199 | 1.00x |
| users.json | orjson | 24.716 | 26.157 | 35.713 | 61.199 | 0.77x |
| users.json | msgspec | 24.693 | 27.498 | 30.602 | 61.199 | 0.73x |
| users.json | ujson | 35.304 | 41.104 | 49.349 | 61.199 | 0.49x |
| users.json | json | 43.295 | 46.472 | 59.759 | 61.199 | 0.43x |
| flat.json | strata | 1.349 | 1.407 | 1.606 | 56.414 | 1.00x |
| flat.json | orjson | 1.484 | 1.542 | 1.682 | 56.414 | 0.91x |
| flat.json | msgspec | 1.694 | 1.762 | 1.844 | 56.414 | 0.80x |
| flat.json | ujson | 2.723 | 2.949 | 3.470 | 56.414 | 0.48x |
| flat.json | json | 3.244 | 3.304 | 3.879 | 56.414 | 0.43x |
| nested.json | strata | 1.406 | 1.575 | 1.643 | 56.684 | 1.00x |
| nested.json | orjson | 1.615 | 1.769 | 2.208 | 56.684 | 0.89x |
| nested.json | msgspec | 1.769 | 1.927 | 2.531 | 56.684 | 0.82x |
| nested.json | ujson | 2.961 | 3.195 | 3.774 | 56.684 | 0.49x |
| nested.json | json | 3.660 | 4.074 | 4.503 | 56.684 | 0.39x |
| wide_arrays.json | strata | 8.156 | 8.620 | 9.936 | 60.148 | 1.00x |
| wide_arrays.json | orjson | 8.801 | 9.359 | 10.418 | 60.148 | 0.92x |
| wide_arrays.json | msgspec | 9.735 | 10.282 | 12.330 | 60.148 | 0.84x |
| wide_arrays.json | ujson | 12.644 | 13.510 | 15.323 | 60.148 | 0.64x |
| wide_arrays.json | json | 15.983 | 16.900 | 18.038 | 60.148 | 0.51x |
| mixed.json | strata | 0.376 | 0.448 | 0.494 | 55.914 | 1.00x |
| mixed.json | orjson | 0.466 | 0.557 | 0.608 | 55.914 | 0.80x |
| mixed.json | msgspec | 0.487 | 0.593 | 0.646 | 55.914 | 0.76x |
| mixed.json | ujson | 0.672 | 0.791 | 1.158 | 55.914 | 0.57x |
| mixed.json | json | 0.891 | 1.034 | 1.290 | 55.914 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 18.699 | 20.584 | 25.932 | 60.285 | 1.00x |
| users.ndjson | orjson | 26.436 | 28.457 | 47.581 | 60.285 | 0.72x |
| users.ndjson | msgspec | 27.524 | 29.481 | 42.394 | 60.285 | 0.70x |
| users.ndjson | ujson | 38.806 | 41.749 | 57.347 | 60.285 | 0.49x |
| users.ndjson | json | 47.700 | 50.965 | 84.956 | 60.285 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.189 | 3.568 | 4.189 | 56.344 | 1.00x |
| users.json | orjson | 3.936 | 4.267 | 5.027 | 56.344 | 0.84x |
| users.json | msgspec | 5.896 | 6.298 | 7.533 | 56.344 | 0.57x |
| users.json | ujson | 31.362 | 32.659 | 33.671 | 56.344 | 0.11x |
| users.json | json | 41.069 | 42.484 | 44.542 | 56.344 | 0.08x |
| flat.json | strata | 0.554 | 0.676 | 0.742 | 56.414 | 1.00x |
| flat.json | orjson | 0.702 | 0.806 | 0.890 | 56.414 | 0.84x |
| flat.json | msgspec | 0.767 | 0.934 | 1.048 | 56.414 | 0.72x |
| flat.json | ujson | 2.700 | 3.085 | 3.441 | 56.414 | 0.22x |
| flat.json | json | 3.713 | 3.991 | 5.575 | 56.414 | 0.17x |
| nested.json | strata | 0.493 | 0.580 | 0.718 | 56.684 | 1.00x |
| nested.json | orjson | 0.617 | 0.723 | 0.840 | 56.684 | 0.80x |
| nested.json | msgspec | 0.801 | 0.949 | 1.421 | 56.684 | 0.61x |
| nested.json | ujson | 2.990 | 3.218 | 3.830 | 56.684 | 0.18x |
| nested.json | json | 4.710 | 4.987 | 6.200 | 56.684 | 0.12x |
| wide_arrays.json | strata | 2.168 | 2.436 | 2.642 | 58.988 | 1.00x |
| wide_arrays.json | orjson | 2.734 | 3.117 | 4.743 | 58.988 | 0.78x |
| wide_arrays.json | msgspec | 3.473 | 3.992 | 4.337 | 58.988 | 0.61x |
| wide_arrays.json | ujson | 15.675 | 16.342 | 18.050 | 58.988 | 0.15x |
| wide_arrays.json | json | 32.924 | 34.493 | 36.346 | 58.988 | 0.07x |
| mixed.json | strata | 0.311 | 0.396 | 0.638 | 55.914 | 1.00x |
| mixed.json | orjson | 0.314 | 0.414 | 0.491 | 55.914 | 0.96x |
| mixed.json | msgspec | 0.336 | 0.425 | 0.777 | 55.914 | 0.93x |
| mixed.json | ujson | 0.832 | 0.958 | 1.208 | 55.914 | 0.41x |
| mixed.json | json | 1.168 | 1.299 | 1.677 | 55.914 | 0.30x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.118 | 0.149 | 0.205 | 56.418 | 1.00x |
| users.json $[*].id | jmespath | 0.881 | 0.974 | 1.284 | 56.418 | 0.15x |
| users.json $[*].id | jsonpath-ng | 4.851 | 5.221 | 6.399 | 56.418 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.751 | 0.934 | 1.178 | 55.660 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.460 | 5.839 | 6.413 | 55.660 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 32.433 | 34.506 | 40.876 | 55.660 | 0.03x |
| users.json $..total | strata | 2.918 | 3.466 | 3.758 | 57.723 | 1.00x |
| users.json $..total | jsonpath-ng | 695.752 | 714.096 | 786.508 | 57.723 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.031 | 4.483 | 7.438 | 58.449 | 1.00x |
| users.json $[*].id | orjson+jmespath | 25.983 | 31.260 | 43.336 | 58.449 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 28.838 | 36.596 | 49.383 | 58.449 | 0.12x |
| users.json $[*].orders[*].total | strata | 4.121 | 4.360 | 4.840 | 56.945 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 31.137 | 32.862 | 34.991 | 56.945 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 63.970 | 68.674 | 77.273 | 56.945 | 0.06x |
| users.json $..total | strata | 22.427 | 23.005 | 30.698 | 57.012 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 725.032 | 736.973 | 798.154 | 57.012 | 0.03x |

