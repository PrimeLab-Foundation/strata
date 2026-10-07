# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 38cf28416465e176915ffd24d827a95015eacdbd
- python: 3.12.10
- implementation: CPython
- platform: macOS-15.7.9-x86_64-i386-64bit
- machine: x86_64
- processor: Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz
- compiler_flags: -fno-strict-overflow -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -O3 -Wall -arch x86_64 -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -fomit-frame-pointer -march=native -flto=thin -fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata -bundle -undefined (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 19.640 | 20.848 | 24.727 | 57.113 | 1.00x |
| users.json | orjson | 29.691 | 32.271 | 37.401 | 57.113 | 0.65x |
| users.json | msgspec | 29.561 | 31.666 | 38.211 | 57.113 | 0.66x |
| users.json | ujson | 43.868 | 47.027 | 51.689 | 57.113 | 0.44x |
| users.json | pysimdjson | 183.865 | 190.903 | 198.613 | 57.113 | 0.11x |
| users.json | json | 47.501 | 51.729 | 61.883 | 57.113 | 0.40x |
| flat.json | strata | 1.228 | 1.239 | 1.356 | 67.988 | 1.00x |
| flat.json | orjson | 1.368 | 1.397 | 1.462 | 67.988 | 0.89x |
| flat.json | msgspec | 1.542 | 1.574 | 1.614 | 67.988 | 0.79x |
| flat.json | ujson | 2.701 | 2.773 | 3.147 | 67.988 | 0.45x |
| flat.json | pysimdjson | 14.728 | 14.840 | 15.106 | 67.988 | 0.08x |
| flat.json | json | 3.145 | 3.163 | 3.256 | 67.988 | 0.39x |
| nested.json | strata | 1.419 | 1.440 | 1.552 | 66.473 | 1.00x |
| nested.json | orjson | 1.602 | 1.655 | 1.827 | 66.473 | 0.87x |
| nested.json | msgspec | 1.765 | 1.811 | 1.955 | 66.473 | 0.79x |
| nested.json | ujson | 2.966 | 2.995 | 3.290 | 66.473 | 0.48x |
| nested.json | pysimdjson | 12.957 | 13.403 | 13.888 | 66.473 | 0.11x |
| nested.json | json | 3.767 | 3.858 | 4.423 | 66.473 | 0.37x |
| wide_arrays.json | strata | 7.664 | 8.133 | 8.788 | 72.438 | 1.00x |
| wide_arrays.json | orjson | 9.174 | 9.876 | 13.477 | 72.438 | 0.82x |
| wide_arrays.json | msgspec | 9.834 | 10.404 | 12.469 | 72.438 | 0.78x |
| wide_arrays.json | ujson | 12.323 | 13.786 | 15.421 | 72.438 | 0.59x |
| wide_arrays.json | pysimdjson | 76.757 | 81.944 | 90.292 | 72.438 | 0.10x |
| wide_arrays.json | json | 16.233 | 18.481 | 20.631 | 72.438 | 0.44x |
| mixed.json | strata | 0.367 | 0.378 | 0.407 | 65.234 | 1.00x |
| mixed.json | orjson | 0.451 | 0.459 | 0.469 | 65.234 | 0.82x |
| mixed.json | msgspec | 0.469 | 0.485 | 0.558 | 65.234 | 0.78x |
| mixed.json | ujson | 0.644 | 0.662 | 0.672 | 65.234 | 0.57x |
| mixed.json | pysimdjson | 3.341 | 3.360 | 3.470 | 65.234 | 0.11x |
| mixed.json | json | 0.912 | 0.927 | 0.945 | 65.234 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.925 | 3.645 | 5.963 | 52.438 | 1.00x |
| users.json | orjson | 3.974 | 4.331 | 5.592 | 52.438 | 0.84x |
| users.json | msgspec | 5.942 | 6.198 | 7.170 | 52.438 | 0.59x |
| users.json | ujson | 27.473 | 31.124 | 31.967 | 52.438 | 0.12x |
| users.json | json | 46.416 | 51.055 | 57.312 | 52.438 | 0.07x |
| flat.json | strata | 0.311 | 0.327 | 0.370 | 66.512 | 1.00x |
| flat.json | orjson | 0.378 | 0.397 | 0.441 | 66.512 | 0.82x |
| flat.json | msgspec | 0.517 | 0.532 | 0.562 | 66.512 | 0.61x |
| flat.json | ujson | 2.174 | 2.211 | 2.235 | 66.512 | 0.15x |
| flat.json | json | 3.573 | 3.604 | 3.646 | 66.512 | 0.09x |
| nested.json | strata | 0.222 | 0.227 | 0.242 | 66.605 | 1.00x |
| nested.json | orjson | 0.327 | 0.341 | 0.496 | 66.605 | 0.67x |
| nested.json | msgspec | 0.523 | 0.529 | 0.570 | 66.605 | 0.43x |
| nested.json | ujson | 2.210 | 2.230 | 2.296 | 66.605 | 0.10x |
| nested.json | json | 4.420 | 4.448 | 4.499 | 66.605 | 0.05x |
| wide_arrays.json | strata | 1.969 | 2.134 | 2.553 | 66.164 | 1.00x |
| wide_arrays.json | orjson | 2.495 | 2.689 | 2.980 | 66.164 | 0.79x |
| wide_arrays.json | msgspec | 3.212 | 3.770 | 4.484 | 66.164 | 0.57x |
| wide_arrays.json | ujson | 9.708 | 11.464 | 12.200 | 66.164 | 0.19x |
| wide_arrays.json | json | 34.277 | 35.475 | 38.309 | 66.164 | 0.06x |
| mixed.json | strata | 0.067 | 0.077 | 0.135 | 62.035 | 1.00x |
| mixed.json | orjson | 0.085 | 0.093 | 0.106 | 62.035 | 0.83x |
| mixed.json | msgspec | 0.120 | 0.129 | 0.150 | 62.035 | 0.59x |
| mixed.json | ujson | 0.474 | 0.480 | 0.745 | 62.035 | 0.16x |
| mixed.json | json | 0.992 | 1.010 | 1.071 | 62.035 | 0.08x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 19.944 | 21.001 | 21.555 | 63.219 | 1.00x |
| users.json | orjson | 29.992 | 31.693 | 34.069 | 63.219 | 0.66x |
| users.json | msgspec | 30.779 | 31.550 | 33.975 | 63.219 | 0.67x |
| users.json | ujson | 43.837 | 45.604 | 51.937 | 63.219 | 0.46x |
| users.json | json | 49.656 | 51.963 | 57.578 | 63.219 | 0.40x |
| flat.json | strata | 1.283 | 1.335 | 1.386 | 66.512 | 1.00x |
| flat.json | orjson | 1.503 | 1.529 | 1.634 | 66.512 | 0.87x |
| flat.json | msgspec | 1.679 | 1.720 | 1.762 | 66.512 | 0.78x |
| flat.json | ujson | 2.808 | 2.901 | 3.192 | 66.512 | 0.46x |
| flat.json | json | 3.150 | 3.189 | 3.260 | 66.512 | 0.42x |
| nested.json | strata | 1.503 | 1.521 | 1.591 | 66.605 | 1.00x |
| nested.json | orjson | 1.710 | 1.760 | 1.820 | 66.605 | 0.86x |
| nested.json | msgspec | 1.910 | 1.925 | 1.966 | 66.605 | 0.79x |
| nested.json | ujson | 3.068 | 3.116 | 3.482 | 66.605 | 0.49x |
| nested.json | json | 3.872 | 3.918 | 4.233 | 66.605 | 0.39x |
| wide_arrays.json | strata | 7.468 | 8.393 | 8.637 | 67.395 | 1.00x |
| wide_arrays.json | orjson | 9.418 | 9.977 | 10.539 | 67.395 | 0.84x |
| wide_arrays.json | msgspec | 10.116 | 11.024 | 13.284 | 67.395 | 0.76x |
| wide_arrays.json | ujson | 13.071 | 13.761 | 15.631 | 67.395 | 0.61x |
| wide_arrays.json | json | 16.847 | 17.391 | 19.104 | 67.395 | 0.48x |
| mixed.json | strata | 0.441 | 0.463 | 0.477 | 62.035 | 1.00x |
| mixed.json | orjson | 0.553 | 0.600 | 0.613 | 62.035 | 0.77x |
| mixed.json | msgspec | 0.618 | 0.635 | 0.652 | 62.035 | 0.73x |
| mixed.json | ujson | 0.792 | 0.825 | 0.840 | 62.035 | 0.56x |
| mixed.json | json | 1.034 | 1.057 | 1.085 | 62.035 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 19.948 | 21.384 | 24.011 | 67.566 | 1.00x |
| users.ndjson | orjson | 28.878 | 31.551 | 41.622 | 67.566 | 0.68x |
| users.ndjson | msgspec | 28.060 | 31.730 | 35.870 | 67.566 | 0.67x |
| users.ndjson | ujson | 40.159 | 44.516 | 54.526 | 67.566 | 0.48x |
| users.ndjson | json | 51.179 | 57.359 | 66.672 | 67.566 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.573 | 3.994 | 5.563 | 63.234 | 1.00x |
| users.json | orjson | 4.747 | 5.136 | 5.940 | 63.234 | 0.78x |
| users.json | msgspec | 6.540 | 7.059 | 9.216 | 63.234 | 0.57x |
| users.json | ujson | 27.723 | 28.750 | 30.872 | 63.234 | 0.14x |
| users.json | json | 44.757 | 47.303 | 56.326 | 63.234 | 0.08x |
| flat.json | strata | 0.674 | 0.719 | 3.865 | 66.512 | 1.00x |
| flat.json | orjson | 0.758 | 0.851 | 1.319 | 66.512 | 0.84x |
| flat.json | msgspec | 0.893 | 1.008 | 1.642 | 66.512 | 0.71x |
| flat.json | ujson | 2.540 | 2.618 | 3.217 | 66.512 | 0.27x |
| flat.json | json | 3.915 | 3.988 | 4.440 | 66.512 | 0.18x |
| nested.json | strata | 0.517 | 0.558 | 0.569 | 66.605 | 1.00x |
| nested.json | orjson | 0.644 | 0.665 | 0.700 | 66.605 | 0.84x |
| nested.json | msgspec | 0.846 | 0.896 | 1.095 | 66.605 | 0.62x |
| nested.json | ujson | 2.572 | 2.617 | 2.807 | 66.605 | 0.21x |
| nested.json | json | 4.801 | 4.901 | 5.222 | 66.605 | 0.11x |
| wide_arrays.json | strata | 2.683 | 2.876 | 3.012 | 66.164 | 1.00x |
| wide_arrays.json | orjson | 3.288 | 3.475 | 3.590 | 66.164 | 0.83x |
| wide_arrays.json | msgspec | 4.402 | 4.684 | 5.209 | 66.164 | 0.61x |
| wide_arrays.json | ujson | 12.485 | 12.820 | 13.888 | 66.164 | 0.22x |
| wide_arrays.json | json | 36.451 | 36.811 | 37.729 | 66.164 | 0.08x |
| mixed.json | strata | 0.374 | 0.398 | 0.426 | 62.035 | 1.00x |
| mixed.json | orjson | 0.391 | 0.452 | 0.481 | 62.035 | 0.88x |
| mixed.json | msgspec | 0.428 | 0.465 | 0.502 | 62.035 | 0.85x |
| mixed.json | ujson | 0.792 | 0.875 | 0.952 | 62.035 | 0.45x |
| mixed.json | json | 1.265 | 1.341 | 1.384 | 62.035 | 0.30x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.168 | 0.189 | 0.262 | 63.328 | 1.00x |
| users.json $[*].id | jmespath | 0.991 | 1.054 | 1.318 | 63.328 | 0.18x |
| users.json $[*].id | jsonpath-ng | 5.351 | 5.776 | 6.885 | 63.328 | 0.03x |
| users.json $[*].orders[*].total | strata | 1.182 | 1.310 | 1.855 | 60.488 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 7.225 | 7.733 | 8.480 | 60.488 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 41.526 | 44.954 | 51.169 | 60.488 | 0.03x |
| users.json $..total | strata | 3.637 | 4.097 | 5.890 | 60.562 | 1.00x |
| users.json $..total | jsonpath-ng | 728.171 | 892.940 | 975.857 | 60.562 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.053 | 4.224 | 4.822 | 63.398 | 1.00x |
| users.json $[*].id | orjson+jmespath | 29.832 | 32.221 | 35.301 | 63.398 | 0.13x |
| users.json $[*].id | orjson+jsonpath-ng | 34.482 | 36.348 | 39.989 | 63.398 | 0.12x |
| users.json $[*].orders[*].total | strata | 4.701 | 5.054 | 6.698 | 60.535 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 37.127 | 39.096 | 49.015 | 60.535 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 79.791 | 86.479 | 98.419 | 60.535 | 0.06x |
| users.json $..total | strata | 22.559 | 24.228 | 25.508 | 60.562 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 783.044 | 799.444 | 832.117 | 60.562 | 0.03x |

