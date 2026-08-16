# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 9c9d784
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
| users.json | strata | 24.532 | 25.524 | 35.240 | 52.809 | 1.00x |
| users.json | orjson | 33.005 | 34.422 | 41.961 | 52.809 | 0.74x |
| users.json | msgspec | 31.803 | 34.130 | 39.489 | 52.809 | 0.75x |
| users.json | ujson | 46.274 | 48.952 | 61.417 | 52.809 | 0.52x |
| users.json | pysimdjson | 206.409 | 210.489 | 228.762 | 52.809 | 0.12x |
| users.json | json | 52.338 | 56.410 | 62.773 | 52.809 | 0.45x |
| flat.json | strata | 1.837 | 1.859 | 2.613 | 62.430 | 1.00x |
| flat.json | orjson | 1.923 | 1.951 | 2.289 | 62.430 | 0.95x |
| flat.json | msgspec | 2.171 | 2.254 | 2.502 | 62.430 | 0.82x |
| flat.json | ujson | 3.792 | 4.081 | 5.129 | 62.430 | 0.46x |
| flat.json | pysimdjson | 20.690 | 21.038 | 22.135 | 62.430 | 0.09x |
| flat.json | json | 4.332 | 4.425 | 6.259 | 62.430 | 0.42x |
| nested.json | strata | 2.029 | 2.078 | 2.259 | 50.621 | 1.00x |
| nested.json | orjson | 2.236 | 2.352 | 2.851 | 50.621 | 0.88x |
| nested.json | msgspec | 2.478 | 2.562 | 2.902 | 50.621 | 0.81x |
| nested.json | ujson | 4.123 | 4.287 | 4.670 | 50.621 | 0.48x |
| nested.json | pysimdjson | 18.349 | 18.853 | 20.131 | 50.621 | 0.11x |
| nested.json | json | 5.252 | 5.438 | 6.451 | 50.621 | 0.38x |
| wide_arrays.json | strata | 11.446 | 11.764 | 14.484 | 62.086 | 1.00x |
| wide_arrays.json | orjson | 12.091 | 12.636 | 16.335 | 62.086 | 0.93x |
| wide_arrays.json | msgspec | 13.638 | 14.027 | 18.056 | 62.086 | 0.84x |
| wide_arrays.json | ujson | 17.342 | 19.111 | 23.325 | 62.086 | 0.62x |
| wide_arrays.json | pysimdjson | 109.193 | 114.797 | 129.167 | 62.086 | 0.10x |
| wide_arrays.json | json | 22.241 | 24.009 | 26.937 | 62.086 | 0.49x |
| mixed.json | strata | 0.499 | 0.541 | 0.830 | 58.383 | 1.00x |
| mixed.json | orjson | 0.578 | 0.647 | 0.957 | 58.383 | 0.84x |
| mixed.json | msgspec | 0.613 | 0.667 | 0.920 | 58.383 | 0.81x |
| mixed.json | ujson | 0.845 | 0.996 | 1.308 | 58.383 | 0.54x |
| mixed.json | pysimdjson | 4.402 | 5.208 | 6.046 | 58.383 | 0.10x |
| mixed.json | json | 1.217 | 1.376 | 1.682 | 58.383 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.865 | 4.214 | 6.102 | 47.531 | 1.00x |
| users.json | orjson | 4.209 | 4.820 | 6.325 | 47.531 | 0.87x |
| users.json | msgspec | 7.687 | 7.965 | 9.019 | 47.531 | 0.53x |
| users.json | ujson | 39.590 | 39.948 | 48.028 | 47.531 | 0.11x |
| users.json | json | 53.594 | 54.157 | 68.437 | 47.531 | 0.08x |
| flat.json | strata | 0.444 | 0.463 | 0.516 | 50.242 | 1.00x |
| flat.json | orjson | 0.532 | 0.583 | 0.735 | 50.242 | 0.79x |
| flat.json | msgspec | 0.725 | 0.935 | 1.011 | 50.242 | 0.49x |
| flat.json | ujson | 3.595 | 3.803 | 5.134 | 50.242 | 0.12x |
| flat.json | json | 4.872 | 5.175 | 6.123 | 50.242 | 0.09x |
| nested.json | strata | 0.349 | 0.368 | 0.624 | 50.504 | 1.00x |
| nested.json | orjson | 0.479 | 0.504 | 0.738 | 50.504 | 0.73x |
| nested.json | msgspec | 0.753 | 0.817 | 1.207 | 50.504 | 0.45x |
| nested.json | ujson | 3.764 | 3.890 | 5.329 | 50.504 | 0.09x |
| nested.json | json | 6.212 | 6.450 | 9.320 | 50.504 | 0.06x |
| wide_arrays.json | strata | 2.568 | 2.715 | 3.493 | 61.488 | 1.00x |
| wide_arrays.json | orjson | 3.091 | 3.316 | 5.203 | 61.488 | 0.82x |
| wide_arrays.json | msgspec | 4.339 | 4.585 | 6.843 | 61.488 | 0.59x |
| wide_arrays.json | ujson | 20.638 | 22.060 | 29.297 | 61.488 | 0.12x |
| wide_arrays.json | json | 46.482 | 48.311 | 64.071 | 61.488 | 0.06x |
| mixed.json | strata | 0.116 | 0.127 | 0.222 | 56.172 | 1.00x |
| mixed.json | orjson | 0.107 | 0.116 | 0.202 | 56.172 | 1.09x |
| mixed.json | msgspec | 0.156 | 0.161 | 0.232 | 56.172 | 0.79x |
| mixed.json | ujson | 0.836 | 1.058 | 1.430 | 56.172 | 0.12x |
| mixed.json | json | 1.303 | 1.470 | 1.813 | 56.172 | 0.09x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 26.780 | 29.934 | 113.774 | 60.047 | 1.00x |
| users.json | orjson | 33.934 | 44.919 | 157.644 | 60.047 | 0.67x |
| users.json | msgspec | 34.787 | 43.468 | 140.314 | 60.047 | 0.69x |
| users.json | ujson | 52.406 | 62.250 | 78.424 | 60.047 | 0.48x |
| users.json | json | 54.636 | 71.041 | 224.879 | 60.047 | 0.42x |
| flat.json | strata | 1.940 | 2.006 | 2.729 | 50.383 | 1.00x |
| flat.json | orjson | 2.040 | 2.122 | 2.626 | 50.383 | 0.95x |
| flat.json | msgspec | 2.325 | 2.430 | 2.873 | 50.383 | 0.83x |
| flat.json | ujson | 3.939 | 4.252 | 4.565 | 50.383 | 0.47x |
| flat.json | json | 4.424 | 4.687 | 5.454 | 50.383 | 0.43x |
| nested.json | strata | 2.115 | 2.202 | 2.878 | 50.504 | 1.00x |
| nested.json | orjson | 2.385 | 2.624 | 8.730 | 50.504 | 0.84x |
| nested.json | msgspec | 2.593 | 2.894 | 16.935 | 50.504 | 0.76x |
| nested.json | ujson | 4.255 | 4.588 | 10.315 | 50.504 | 0.48x |
| nested.json | json | 5.257 | 5.851 | 7.352 | 50.504 | 0.38x |
| wide_arrays.json | strata | 12.722 | 14.881 | 20.138 | 61.488 | 1.00x |
| wide_arrays.json | orjson | 12.784 | 15.623 | 19.548 | 61.488 | 0.95x |
| wide_arrays.json | msgspec | 14.060 | 17.397 | 21.081 | 61.488 | 0.86x |
| wide_arrays.json | ujson | 18.847 | 24.471 | 28.805 | 61.488 | 0.61x |
| wide_arrays.json | json | 23.714 | 28.002 | 37.111 | 61.488 | 0.53x |
| mixed.json | strata | 0.628 | 0.726 | 1.050 | 56.172 | 1.00x |
| mixed.json | orjson | 0.755 | 0.883 | 1.209 | 56.172 | 0.82x |
| mixed.json | msgspec | 0.801 | 0.967 | 1.403 | 56.172 | 0.75x |
| mixed.json | ujson | 1.033 | 1.150 | 1.774 | 56.172 | 0.63x |
| mixed.json | json | 1.371 | 1.524 | 2.432 | 56.172 | 0.48x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 28.413 | 30.817 | 38.113 | 61.395 | 1.00x |
| users.ndjson | orjson | 36.962 | 40.697 | 58.624 | 61.395 | 0.76x |
| users.ndjson | msgspec | 37.570 | 42.133 | 59.059 | 61.395 | 0.73x |
| users.ndjson | ujson | 55.072 | 59.739 | 74.615 | 61.395 | 0.52x |
| users.ndjson | json | 67.795 | 73.880 | 80.364 | 61.395 | 0.42x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 5.331 | 6.394 | 9.044 | 54.613 | 1.00x |
| users.json | orjson | 5.827 | 6.270 | 9.905 | 54.613 | 1.02x |
| users.json | msgspec | 8.398 | 9.827 | 11.375 | 54.613 | 0.65x |
| users.json | ujson | 45.845 | 51.463 | 80.729 | 54.613 | 0.12x |
| users.json | json | 59.627 | 68.390 | 168.742 | 54.613 | 0.09x |
| flat.json | strata | 0.856 | 0.882 | 1.221 | 50.383 | 1.00x |
| flat.json | orjson | 0.983 | 1.082 | 1.494 | 50.383 | 0.82x |
| flat.json | msgspec | 1.124 | 1.299 | 1.725 | 50.383 | 0.68x |
| flat.json | ujson | 4.071 | 4.258 | 5.108 | 50.383 | 0.21x |
| flat.json | json | 5.289 | 5.606 | 6.778 | 50.383 | 0.16x |
| nested.json | strata | 0.750 | 0.809 | 2.084 | 50.504 | 1.00x |
| nested.json | orjson | 0.910 | 1.090 | 4.467 | 50.504 | 0.74x |
| nested.json | msgspec | 1.205 | 1.329 | 3.127 | 50.504 | 0.61x |
| nested.json | ujson | 4.208 | 5.343 | 18.773 | 50.504 | 0.15x |
| nested.json | json | 6.739 | 7.211 | 35.781 | 50.504 | 0.11x |
| wide_arrays.json | strata | 3.506 | 4.149 | 19.765 | 61.488 | 1.00x |
| wide_arrays.json | orjson | 4.314 | 5.133 | 22.963 | 61.488 | 0.81x |
| wide_arrays.json | msgspec | 5.292 | 5.860 | 8.287 | 61.488 | 0.71x |
| wide_arrays.json | ujson | 21.663 | 23.609 | 29.332 | 61.488 | 0.18x |
| wide_arrays.json | json | 48.027 | 59.888 | 101.522 | 61.488 | 0.07x |
| mixed.json | strata | 0.518 | 0.601 | 0.944 | 56.172 | 1.00x |
| mixed.json | orjson | 0.530 | 0.603 | 2.001 | 56.172 | 1.00x |
| mixed.json | msgspec | 0.549 | 0.603 | 0.836 | 56.172 | 1.00x |
| mixed.json | ujson | 1.263 | 1.440 | 1.749 | 56.172 | 0.42x |
| mixed.json | json | 1.773 | 1.843 | 2.519 | 56.172 | 0.33x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.182 | 0.210 | 0.302 | 54.691 | 1.00x |
| users.json $[*].id | jmespath | 1.279 | 1.322 | 1.792 | 54.691 | 0.16x |
| users.json $[*].id | jsonpath-ng | 7.117 | 7.387 | 9.093 | 54.691 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.986 | 1.117 | 1.360 | 55.492 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 7.045 | 7.284 | 7.859 | 55.492 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 43.306 | 45.683 | 51.595 | 55.492 | 0.02x |
| users.json $..total | strata | 4.110 | 4.470 | 4.964 | 56.438 | 1.00x |
| users.json $..total | jsonpath-ng | 873.990 | 914.527 | 992.789 | 56.438 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 5.219 | 5.322 | 5.803 | 58.531 | 1.00x |
| users.json $[*].id | orjson+jmespath | 32.706 | 34.816 | 36.994 | 58.531 | 0.15x |
| users.json $[*].id | orjson+jsonpath-ng | 37.890 | 39.209 | 40.891 | 58.531 | 0.14x |
| users.json $[*].orders[*].total | strata | 5.538 | 5.690 | 6.758 | 56.402 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 37.745 | 38.749 | 52.014 | 56.402 | 0.15x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 81.043 | 85.460 | 115.027 | 56.402 | 0.07x |
| users.json $..total | strata | 28.798 | 29.712 | 34.567 | 56.609 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 910.815 | 925.093 | 971.993 | 56.609 | 0.03x |

