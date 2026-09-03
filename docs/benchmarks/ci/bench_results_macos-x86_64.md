# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 07e47e6
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
| users.json | strata | 19.958 | 20.533 | 24.697 | 52.645 | 1.00x |
| users.json | orjson | 27.062 | 29.047 | 36.674 | 52.645 | 0.71x |
| users.json | msgspec | 27.920 | 29.200 | 39.824 | 52.645 | 0.70x |
| users.json | ujson | 39.899 | 42.924 | 53.437 | 52.645 | 0.48x |
| users.json | pysimdjson | 167.817 | 170.803 | 185.653 | 52.645 | 0.12x |
| users.json | json | 45.100 | 46.458 | 53.556 | 52.645 | 0.44x |
| flat.json | strata | 1.201 | 1.261 | 1.332 | 61.195 | 1.00x |
| flat.json | orjson | 1.301 | 1.359 | 1.855 | 61.195 | 0.93x |
| flat.json | msgspec | 1.535 | 1.588 | 2.032 | 61.195 | 0.79x |
| flat.json | ujson | 2.629 | 2.706 | 3.087 | 61.195 | 0.47x |
| flat.json | pysimdjson | 14.102 | 14.644 | 14.840 | 61.195 | 0.09x |
| flat.json | json | 3.016 | 3.105 | 3.469 | 61.195 | 0.41x |
| nested.json | strata | 1.365 | 1.430 | 1.475 | 58.457 | 1.00x |
| nested.json | orjson | 1.557 | 1.593 | 1.699 | 58.457 | 0.90x |
| nested.json | msgspec | 1.799 | 1.811 | 2.093 | 58.457 | 0.79x |
| nested.json | ujson | 2.909 | 2.956 | 3.089 | 58.457 | 0.48x |
| nested.json | pysimdjson | 12.921 | 13.059 | 13.376 | 58.457 | 0.11x |
| nested.json | json | 3.696 | 3.733 | 3.983 | 58.457 | 0.38x |
| wide_arrays.json | strata | 7.972 | 8.454 | 8.967 | 62.738 | 1.00x |
| wide_arrays.json | orjson | 8.566 | 9.014 | 10.042 | 62.738 | 0.94x |
| wide_arrays.json | msgspec | 9.391 | 9.949 | 10.555 | 62.738 | 0.85x |
| wide_arrays.json | ujson | 12.046 | 13.064 | 13.713 | 62.738 | 0.65x |
| wide_arrays.json | pysimdjson | 74.661 | 79.893 | 81.693 | 62.738 | 0.11x |
| wide_arrays.json | json | 15.807 | 16.656 | 17.247 | 62.738 | 0.51x |
| mixed.json | strata | 0.367 | 0.374 | 0.390 | 59.746 | 1.00x |
| mixed.json | orjson | 0.426 | 0.440 | 0.456 | 59.746 | 0.85x |
| mixed.json | msgspec | 0.462 | 0.474 | 0.513 | 59.746 | 0.79x |
| mixed.json | ujson | 0.629 | 0.639 | 0.721 | 59.746 | 0.59x |
| mixed.json | pysimdjson | 3.142 | 3.161 | 3.299 | 59.746 | 0.12x |
| mixed.json | json | 0.867 | 0.874 | 0.955 | 59.746 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.556 | 2.780 | 3.390 | 48.449 | 1.00x |
| users.json | orjson | 3.343 | 3.585 | 3.985 | 48.449 | 0.78x |
| users.json | msgspec | 5.993 | 6.124 | 7.198 | 48.449 | 0.45x |
| users.json | ujson | 31.905 | 32.214 | 34.101 | 48.449 | 0.09x |
| users.json | json | 42.976 | 43.416 | 45.640 | 48.449 | 0.06x |
| flat.json | strata | 0.281 | 0.299 | 0.330 | 58.320 | 1.00x |
| flat.json | orjson | 0.375 | 0.394 | 0.624 | 58.320 | 0.76x |
| flat.json | msgspec | 0.493 | 0.529 | 0.563 | 58.320 | 0.57x |
| flat.json | ujson | 2.505 | 2.554 | 2.682 | 58.320 | 0.12x |
| flat.json | json | 3.479 | 3.545 | 3.967 | 58.320 | 0.08x |
| nested.json | strata | 0.220 | 0.227 | 0.245 | 53.359 | 1.00x |
| nested.json | orjson | 0.320 | 0.336 | 0.400 | 53.359 | 0.68x |
| nested.json | msgspec | 0.525 | 0.532 | 0.564 | 53.359 | 0.43x |
| nested.json | ujson | 2.592 | 2.652 | 2.698 | 53.359 | 0.09x |
| nested.json | json | 4.326 | 4.440 | 4.662 | 53.359 | 0.05x |
| wide_arrays.json | strata | 1.510 | 1.550 | 1.748 | 61.258 | 1.00x |
| wide_arrays.json | orjson | 2.112 | 2.238 | 2.328 | 61.258 | 0.69x |
| wide_arrays.json | msgspec | 2.929 | 3.034 | 3.358 | 61.258 | 0.51x |
| wide_arrays.json | ujson | 14.208 | 14.605 | 15.636 | 61.258 | 0.11x |
| wide_arrays.json | json | 31.943 | 33.494 | 35.404 | 61.258 | 0.05x |
| mixed.json | strata | 0.064 | 0.070 | 0.082 | 56.426 | 1.00x |
| mixed.json | orjson | 0.078 | 0.084 | 0.093 | 56.426 | 0.84x |
| mixed.json | msgspec | 0.107 | 0.120 | 0.140 | 56.426 | 0.59x |
| mixed.json | ujson | 0.595 | 0.603 | 0.901 | 56.426 | 0.12x |
| mixed.json | json | 0.921 | 0.938 | 1.009 | 56.426 | 0.07x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 18.807 | 19.112 | 20.390 | 60.984 | 1.00x |
| users.json | orjson | 24.747 | 25.202 | 27.466 | 60.984 | 0.76x |
| users.json | msgspec | 25.464 | 26.095 | 28.809 | 60.984 | 0.73x |
| users.json | ujson | 36.927 | 38.083 | 39.266 | 60.984 | 0.50x |
| users.json | json | 41.763 | 43.087 | 45.655 | 60.984 | 0.44x |
| flat.json | strata | 1.299 | 1.317 | 1.355 | 58.320 | 1.00x |
| flat.json | orjson | 1.434 | 1.456 | 1.586 | 58.320 | 0.90x |
| flat.json | msgspec | 1.675 | 1.697 | 1.803 | 58.320 | 0.78x |
| flat.json | ujson | 2.790 | 2.816 | 2.966 | 58.320 | 0.47x |
| flat.json | json | 3.115 | 3.139 | 3.252 | 58.320 | 0.42x |
| nested.json | strata | 1.510 | 1.539 | 1.671 | 53.359 | 1.00x |
| nested.json | orjson | 1.690 | 1.778 | 1.801 | 53.359 | 0.87x |
| nested.json | msgspec | 1.879 | 1.970 | 2.028 | 53.359 | 0.78x |
| nested.json | ujson | 3.036 | 3.184 | 5.873 | 53.359 | 0.48x |
| nested.json | json | 3.790 | 3.979 | 4.138 | 53.359 | 0.39x |
| wide_arrays.json | strata | 8.160 | 8.700 | 9.642 | 61.258 | 1.00x |
| wide_arrays.json | orjson | 8.628 | 9.377 | 10.350 | 61.258 | 0.93x |
| wide_arrays.json | msgspec | 9.879 | 10.569 | 11.282 | 61.258 | 0.82x |
| wide_arrays.json | ujson | 12.883 | 13.610 | 14.890 | 61.258 | 0.64x |
| wide_arrays.json | json | 16.116 | 16.999 | 18.319 | 61.258 | 0.51x |
| mixed.json | strata | 0.387 | 0.432 | 0.473 | 56.426 | 1.00x |
| mixed.json | orjson | 0.477 | 0.527 | 0.562 | 56.426 | 0.82x |
| mixed.json | msgspec | 0.542 | 0.571 | 0.599 | 56.426 | 0.76x |
| mixed.json | ujson | 0.706 | 0.730 | 0.777 | 56.426 | 0.59x |
| mixed.json | json | 0.904 | 0.967 | 0.995 | 56.426 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 19.576 | 20.035 | 20.663 | 61.535 | 1.00x |
| users.ndjson | orjson | 26.312 | 27.782 | 28.516 | 61.535 | 0.72x |
| users.ndjson | msgspec | 27.909 | 28.206 | 30.142 | 61.535 | 0.71x |
| users.ndjson | ujson | 39.485 | 40.077 | 41.125 | 61.535 | 0.50x |
| users.ndjson | json | 48.720 | 50.074 | 52.783 | 61.535 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.142 | 3.237 | 4.224 | 55.434 | 1.00x |
| users.json | orjson | 3.961 | 4.105 | 4.610 | 55.434 | 0.79x |
| users.json | msgspec | 5.896 | 6.550 | 7.347 | 55.434 | 0.49x |
| users.json | ujson | 30.725 | 31.161 | 33.151 | 55.434 | 0.10x |
| users.json | json | 40.882 | 42.000 | 44.899 | 55.434 | 0.08x |
| flat.json | strata | 0.570 | 0.623 | 0.743 | 58.320 | 1.00x |
| flat.json | orjson | 0.700 | 0.761 | 0.797 | 58.320 | 0.82x |
| flat.json | msgspec | 0.838 | 0.880 | 1.399 | 58.320 | 0.71x |
| flat.json | ujson | 2.878 | 2.938 | 3.709 | 58.320 | 0.21x |
| flat.json | json | 3.820 | 3.942 | 4.165 | 58.320 | 0.16x |
| nested.json | strata | 0.509 | 0.533 | 0.604 | 53.359 | 1.00x |
| nested.json | orjson | 0.599 | 0.709 | 0.754 | 53.359 | 0.75x |
| nested.json | msgspec | 0.822 | 0.835 | 1.106 | 53.359 | 0.64x |
| nested.json | ujson | 2.965 | 2.998 | 3.167 | 53.359 | 0.18x |
| nested.json | json | 4.658 | 4.705 | 4.813 | 53.359 | 0.11x |
| wide_arrays.json | strata | 2.149 | 2.347 | 2.533 | 61.258 | 1.00x |
| wide_arrays.json | orjson | 3.007 | 3.200 | 4.024 | 61.258 | 0.73x |
| wide_arrays.json | msgspec | 3.487 | 3.975 | 4.164 | 61.258 | 0.59x |
| wide_arrays.json | ujson | 14.993 | 15.893 | 17.629 | 61.258 | 0.15x |
| wide_arrays.json | json | 32.843 | 34.099 | 38.185 | 61.258 | 0.07x |
| mixed.json | strata | 0.292 | 0.348 | 1.484 | 56.426 | 1.00x |
| mixed.json | orjson | 0.318 | 0.358 | 1.354 | 56.426 | 0.97x |
| mixed.json | msgspec | 0.376 | 0.421 | 179.977 | 56.426 | 0.83x |
| mixed.json | ujson | 0.882 | 0.925 | 0.998 | 56.426 | 0.38x |
| mixed.json | json | 1.194 | 1.220 | 1.311 | 56.426 | 0.29x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.124 | 0.135 | 0.152 | 55.523 | 1.00x |
| users.json $[*].id | jmespath | 0.877 | 0.898 | 1.039 | 55.523 | 0.15x |
| users.json $[*].id | jsonpath-ng | 4.895 | 4.968 | 5.203 | 55.523 | 0.03x |
| users.json $[*].orders[*].total | strata | 1.020 | 1.210 | 2.227 | 55.434 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 6.074 | 6.646 | 9.509 | 55.434 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 34.989 | 37.945 | 42.666 | 55.434 | 0.03x |
| users.json $..total | strata | 3.135 | 3.289 | 3.871 | 57.457 | 1.00x |
| users.json $..total | jsonpath-ng | 657.271 | 685.668 | 762.776 | 57.457 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.471 | 3.499 | 3.771 | 59.543 | 1.00x |
| users.json $[*].id | orjson+jmespath | 24.919 | 25.282 | 33.800 | 59.543 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 29.382 | 29.912 | 41.728 | 59.543 | 0.12x |
| users.json $[*].orders[*].total | strata | 4.125 | 4.181 | 4.550 | 56.672 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 31.821 | 32.530 | 35.026 | 56.672 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 63.829 | 67.226 | 73.471 | 56.672 | 0.06x |
| users.json $..total | strata | 22.949 | 23.637 | 24.923 | 56.719 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 704.785 | 717.774 | 807.135 | 56.719 | 0.03x |

