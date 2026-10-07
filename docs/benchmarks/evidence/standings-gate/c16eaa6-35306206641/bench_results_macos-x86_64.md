# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c16eaa65c61f1cf9d4fac46e9f6d8e6b9f3b087f
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
| users.json | strata | 19.333 | 23.216 | 42.434 | 57.055 | 1.00x |
| users.json | orjson | 28.680 | 32.894 | 39.915 | 57.055 | 0.71x |
| users.json | msgspec | 30.235 | 32.771 | 62.308 | 57.055 | 0.71x |
| users.json | ujson | 42.111 | 48.115 | 57.454 | 57.055 | 0.48x |
| users.json | pysimdjson | 182.413 | 205.803 | 316.460 | 57.055 | 0.11x |
| users.json | json | 48.280 | 58.104 | 121.685 | 57.055 | 0.40x |
| flat.json | strata | 1.294 | 1.769 | 8.284 | 67.945 | 1.00x |
| flat.json | orjson | 1.431 | 1.977 | 9.793 | 67.945 | 0.89x |
| flat.json | msgspec | 1.635 | 1.881 | 7.339 | 67.945 | 0.94x |
| flat.json | ujson | 2.997 | 4.357 | 16.793 | 67.945 | 0.41x |
| flat.json | pysimdjson | 15.694 | 17.722 | 66.674 | 67.945 | 0.10x |
| flat.json | json | 3.372 | 4.305 | 11.112 | 67.945 | 0.41x |
| nested.json | strata | 1.606 | 1.723 | 2.208 | 66.422 | 1.00x |
| nested.json | orjson | 1.854 | 2.034 | 3.012 | 66.422 | 0.85x |
| nested.json | msgspec | 2.058 | 2.335 | 3.125 | 66.422 | 0.74x |
| nested.json | ujson | 3.339 | 3.824 | 4.856 | 66.422 | 0.45x |
| nested.json | pysimdjson | 14.189 | 14.851 | 18.196 | 66.422 | 0.12x |
| nested.json | json | 4.281 | 4.447 | 6.313 | 66.422 | 0.39x |
| wide_arrays.json | strata | 8.172 | 8.596 | 10.185 | 72.387 | 1.00x |
| wide_arrays.json | orjson | 10.817 | 11.576 | 14.588 | 72.387 | 0.74x |
| wide_arrays.json | msgspec | 11.087 | 11.546 | 15.270 | 72.387 | 0.74x |
| wide_arrays.json | ujson | 14.081 | 14.830 | 20.469 | 72.387 | 0.58x |
| wide_arrays.json | pysimdjson | 83.950 | 90.567 | 105.655 | 72.387 | 0.09x |
| wide_arrays.json | json | 18.233 | 22.182 | 28.096 | 72.387 | 0.39x |
| mixed.json | strata | 0.386 | 0.391 | 0.399 | 65.184 | 1.00x |
| mixed.json | orjson | 0.475 | 0.478 | 0.648 | 65.184 | 0.82x |
| mixed.json | msgspec | 0.491 | 0.504 | 0.607 | 65.184 | 0.78x |
| mixed.json | ujson | 0.680 | 0.697 | 1.059 | 65.184 | 0.56x |
| mixed.json | pysimdjson | 3.389 | 3.438 | 3.605 | 65.184 | 0.11x |
| mixed.json | json | 0.958 | 0.970 | 1.099 | 65.184 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.690 | 3.336 | 5.019 | 52.387 | 1.00x |
| users.json | orjson | 3.662 | 3.937 | 6.102 | 52.387 | 0.85x |
| users.json | msgspec | 5.796 | 6.596 | 8.582 | 52.387 | 0.51x |
| users.json | ujson | 25.941 | 30.802 | 35.315 | 52.387 | 0.11x |
| users.json | json | 46.553 | 48.508 | 65.387 | 52.387 | 0.07x |
| flat.json | strata | 0.368 | 0.595 | 0.874 | 66.461 | 1.00x |
| flat.json | orjson | 0.461 | 0.673 | 1.099 | 66.461 | 0.88x |
| flat.json | msgspec | 0.604 | 0.895 | 1.556 | 66.461 | 0.66x |
| flat.json | ujson | 3.091 | 3.619 | 4.335 | 66.461 | 0.16x |
| flat.json | json | 3.965 | 6.199 | 7.045 | 66.461 | 0.10x |
| nested.json | strata | 0.263 | 0.294 | 0.493 | 66.555 | 1.00x |
| nested.json | orjson | 0.393 | 0.455 | 0.707 | 66.555 | 0.65x |
| nested.json | msgspec | 0.631 | 0.662 | 0.918 | 66.555 | 0.44x |
| nested.json | ujson | 2.486 | 2.532 | 3.330 | 66.555 | 0.12x |
| nested.json | json | 4.941 | 5.502 | 6.145 | 66.555 | 0.05x |
| wide_arrays.json | strata | 2.349 | 2.565 | 3.097 | 66.113 | 1.00x |
| wide_arrays.json | orjson | 2.894 | 3.192 | 3.666 | 66.113 | 0.80x |
| wide_arrays.json | msgspec | 3.928 | 4.563 | 5.984 | 66.113 | 0.56x |
| wide_arrays.json | ujson | 11.261 | 13.030 | 15.832 | 66.113 | 0.20x |
| wide_arrays.json | json | 36.202 | 41.139 | 46.842 | 66.113 | 0.06x |
| mixed.json | strata | 0.079 | 0.085 | 0.131 | 61.984 | 1.00x |
| mixed.json | orjson | 0.099 | 0.107 | 0.156 | 61.984 | 0.80x |
| mixed.json | msgspec | 0.136 | 0.141 | 0.224 | 61.984 | 0.60x |
| mixed.json | ujson | 0.492 | 0.507 | 0.757 | 61.984 | 0.17x |
| mixed.json | json | 1.026 | 1.051 | 1.415 | 61.984 | 0.08x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 18.976 | 20.125 | 23.610 | 63.176 | 1.00x |
| users.json | orjson | 28.176 | 29.839 | 34.473 | 63.176 | 0.67x |
| users.json | msgspec | 28.656 | 30.876 | 32.926 | 63.176 | 0.65x |
| users.json | ujson | 40.995 | 44.273 | 49.247 | 63.176 | 0.45x |
| users.json | json | 47.089 | 50.430 | 57.012 | 63.176 | 0.40x |
| flat.json | strata | 1.443 | 1.493 | 2.135 | 66.461 | 1.00x |
| flat.json | orjson | 1.634 | 1.719 | 2.450 | 66.461 | 0.87x |
| flat.json | msgspec | 1.839 | 1.949 | 2.842 | 66.461 | 0.77x |
| flat.json | ujson | 3.110 | 3.281 | 5.188 | 66.461 | 0.46x |
| flat.json | json | 3.512 | 3.814 | 5.255 | 66.461 | 0.39x |
| nested.json | strata | 1.741 | 2.228 | 19.993 | 66.555 | 1.00x |
| nested.json | orjson | 2.002 | 2.580 | 3.057 | 66.555 | 0.86x |
| nested.json | msgspec | 2.265 | 3.061 | 3.632 | 66.555 | 0.73x |
| nested.json | ujson | 3.518 | 4.102 | 5.411 | 66.555 | 0.54x |
| nested.json | json | 4.436 | 4.910 | 5.637 | 66.555 | 0.45x |
| wide_arrays.json | strata | 8.274 | 8.459 | 12.027 | 67.344 | 1.00x |
| wide_arrays.json | orjson | 10.272 | 10.902 | 15.467 | 67.344 | 0.78x |
| wide_arrays.json | msgspec | 11.491 | 12.434 | 17.798 | 67.344 | 0.68x |
| wide_arrays.json | ujson | 14.392 | 16.028 | 37.625 | 67.344 | 0.53x |
| wide_arrays.json | json | 19.074 | 19.167 | 23.922 | 67.344 | 0.44x |
| mixed.json | strata | 0.478 | 0.512 | 0.703 | 61.984 | 1.00x |
| mixed.json | orjson | 0.612 | 0.653 | 0.879 | 61.984 | 0.78x |
| mixed.json | msgspec | 0.644 | 0.676 | 0.957 | 61.984 | 0.76x |
| mixed.json | ujson | 0.849 | 0.887 | 0.970 | 61.984 | 0.58x |
| mixed.json | json | 1.096 | 1.191 | 1.920 | 61.984 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 20.303 | 23.436 | 55.676 | 67.523 | 1.00x |
| users.ndjson | orjson | 30.499 | 36.207 | 88.100 | 67.523 | 0.65x |
| users.ndjson | msgspec | 34.193 | 38.361 | 44.900 | 67.523 | 0.61x |
| users.ndjson | ujson | 47.714 | 55.666 | 64.919 | 67.523 | 0.42x |
| users.ndjson | json | 55.578 | 76.094 | 105.805 | 67.523 | 0.31x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.420 | 5.244 | 6.832 | 63.188 | 1.00x |
| users.json | orjson | 5.467 | 6.982 | 8.724 | 63.188 | 0.75x |
| users.json | msgspec | 7.723 | 8.892 | 10.747 | 63.188 | 0.59x |
| users.json | ujson | 31.523 | 33.565 | 39.163 | 63.188 | 0.16x |
| users.json | json | 48.538 | 52.805 | 84.702 | 63.188 | 0.10x |
| flat.json | strata | 0.744 | 0.806 | 1.102 | 66.461 | 1.00x |
| flat.json | orjson | 0.865 | 0.922 | 1.272 | 66.461 | 0.87x |
| flat.json | msgspec | 1.012 | 1.102 | 1.535 | 66.461 | 0.73x |
| flat.json | ujson | 2.761 | 3.119 | 3.661 | 66.461 | 0.26x |
| flat.json | json | 4.351 | 4.614 | 5.322 | 66.461 | 0.17x |
| nested.json | strata | 0.631 | 0.950 | 18.051 | 66.555 | 1.00x |
| nested.json | orjson | 0.805 | 1.012 | 7.197 | 66.555 | 0.94x |
| nested.json | msgspec | 0.955 | 1.244 | 7.890 | 66.555 | 0.76x |
| nested.json | ujson | 2.832 | 3.952 | 14.757 | 66.555 | 0.24x |
| nested.json | json | 5.384 | 6.674 | 44.175 | 66.555 | 0.14x |
| wide_arrays.json | strata | 3.230 | 3.808 | 4.391 | 66.113 | 1.00x |
| wide_arrays.json | orjson | 3.746 | 4.892 | 6.042 | 66.113 | 0.78x |
| wide_arrays.json | msgspec | 4.789 | 5.830 | 7.431 | 66.113 | 0.65x |
| wide_arrays.json | ujson | 13.487 | 16.084 | 21.104 | 66.113 | 0.24x |
| wide_arrays.json | json | 37.462 | 47.015 | 56.017 | 66.113 | 0.08x |
| mixed.json | strata | 0.401 | 0.456 | 0.558 | 61.984 | 1.00x |
| mixed.json | orjson | 0.450 | 0.525 | 0.589 | 61.984 | 0.87x |
| mixed.json | msgspec | 0.502 | 0.562 | 0.810 | 61.984 | 0.81x |
| mixed.json | ujson | 0.881 | 1.000 | 1.377 | 61.984 | 0.46x |
| mixed.json | json | 1.467 | 1.744 | 2.407 | 61.984 | 0.26x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.189 | 0.203 | 0.377 | 63.266 | 1.00x |
| users.json $[*].id | jmespath | 1.082 | 1.311 | 1.607 | 63.266 | 0.15x |
| users.json $[*].id | jsonpath-ng | 5.952 | 6.470 | 9.496 | 63.266 | 0.03x |
| users.json $[*].orders[*].total | strata | 1.580 | 1.784 | 3.482 | 60.473 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 7.012 | 9.419 | 10.989 | 60.473 | 0.19x |
| users.json $[*].orders[*].total | jsonpath-ng | 41.795 | 52.736 | 66.226 | 60.473 | 0.03x |
| users.json $..total | strata | 3.672 | 3.920 | 4.066 | 60.523 | 1.00x |
| users.json $..total | jsonpath-ng | 821.851 | 875.545 | 1975.950 | 60.523 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.118 | 5.271 | 6.582 | 63.324 | 1.00x |
| users.json $[*].id | orjson+jmespath | 32.221 | 41.326 | 49.419 | 63.324 | 0.13x |
| users.json $[*].id | orjson+jsonpath-ng | 42.267 | 44.329 | 52.147 | 63.324 | 0.12x |
| users.json $[*].orders[*].total | strata | 4.602 | 5.137 | 7.171 | 60.492 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 37.393 | 38.824 | 45.836 | 60.492 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 80.263 | 87.286 | 99.332 | 60.492 | 0.06x |
| users.json $..total | strata | 24.176 | 27.227 | 111.762 | 60.523 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 860.627 | 913.708 | 1015.096 | 60.523 | 0.03x |

