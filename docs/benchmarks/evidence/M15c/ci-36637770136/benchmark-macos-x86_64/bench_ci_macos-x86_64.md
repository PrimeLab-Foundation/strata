# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 33465c22eee0fda8e1b52898c16aac6982901a21
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
| users.json | strata | 16.426 | 16.945 | 19.948 | 57.145 | 1.00x |
| users.json | orjson | 22.229 | 24.794 | 27.343 | 57.145 | 0.68x |
| users.json | msgspec | 22.610 | 24.622 | 31.153 | 57.145 | 0.69x |
| users.json | ujson | 34.138 | 36.589 | 38.819 | 57.145 | 0.46x |
| users.json | pysimdjson | 150.769 | 152.901 | 161.253 | 57.145 | 0.11x |
| users.json | json | 39.072 | 40.106 | 45.085 | 57.145 | 0.42x |
| flat.json | strata | 1.106 | 1.135 | 1.443 | 67.898 | 1.00x |
| flat.json | orjson | 1.245 | 1.274 | 1.332 | 67.898 | 0.89x |
| flat.json | msgspec | 1.434 | 1.481 | 1.581 | 67.898 | 0.77x |
| flat.json | ujson | 2.505 | 2.574 | 2.681 | 67.898 | 0.44x |
| flat.json | pysimdjson | 13.563 | 13.751 | 14.088 | 67.898 | 0.08x |
| flat.json | json | 2.975 | 3.004 | 3.058 | 67.898 | 0.38x |
| nested.json | strata | 1.320 | 1.339 | 1.528 | 64.398 | 1.00x |
| nested.json | orjson | 1.515 | 1.550 | 1.639 | 64.398 | 0.86x |
| nested.json | msgspec | 1.664 | 1.715 | 1.777 | 64.398 | 0.78x |
| nested.json | ujson | 2.785 | 2.831 | 2.937 | 64.398 | 0.47x |
| nested.json | pysimdjson | 12.460 | 12.634 | 12.925 | 64.398 | 0.11x |
| nested.json | json | 3.568 | 3.637 | 3.721 | 64.398 | 0.37x |
| wide_arrays.json | strata | 6.737 | 7.077 | 7.917 | 68.816 | 1.00x |
| wide_arrays.json | orjson | 8.224 | 8.919 | 9.370 | 68.816 | 0.79x |
| wide_arrays.json | msgspec | 9.131 | 9.885 | 9.946 | 68.816 | 0.72x |
| wide_arrays.json | ujson | 11.686 | 12.270 | 14.308 | 68.816 | 0.58x |
| wide_arrays.json | pysimdjson | 74.284 | 74.894 | 76.362 | 68.816 | 0.09x |
| wide_arrays.json | json | 15.520 | 16.090 | 17.339 | 68.816 | 0.44x |
| mixed.json | strata | 0.320 | 0.330 | 0.345 | 66.719 | 1.00x |
| mixed.json | orjson | 0.392 | 0.407 | 0.425 | 66.719 | 0.81x |
| mixed.json | msgspec | 0.419 | 0.427 | 0.448 | 66.719 | 0.77x |
| mixed.json | ujson | 0.577 | 0.586 | 0.607 | 66.719 | 0.56x |
| mixed.json | pysimdjson | 3.004 | 3.036 | 3.351 | 66.719 | 0.11x |
| mixed.json | json | 0.811 | 0.834 | 0.860 | 66.719 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.128 | 2.243 | 2.436 | 52.980 | 1.00x |
| users.json | orjson | 2.974 | 3.066 | 3.417 | 52.980 | 0.73x |
| users.json | msgspec | 4.536 | 4.710 | 5.320 | 52.980 | 0.48x |
| users.json | ujson | 23.009 | 23.297 | 24.179 | 52.980 | 0.10x |
| users.json | json | 38.386 | 39.596 | 40.125 | 52.980 | 0.06x |
| flat.json | strata | 0.275 | 0.286 | 0.364 | 64.270 | 1.00x |
| flat.json | orjson | 0.340 | 0.346 | 0.423 | 64.270 | 0.83x |
| flat.json | msgspec | 0.453 | 0.484 | 0.520 | 64.270 | 0.59x |
| flat.json | ujson | 2.063 | 2.084 | 2.126 | 64.270 | 0.14x |
| flat.json | json | 3.383 | 3.412 | 3.482 | 64.270 | 0.08x |
| nested.json | strata | 0.190 | 0.200 | 0.298 | 58.070 | 1.00x |
| nested.json | orjson | 0.298 | 0.328 | 0.467 | 58.070 | 0.61x |
| nested.json | msgspec | 0.477 | 0.496 | 0.610 | 58.070 | 0.40x |
| nested.json | ujson | 2.115 | 2.230 | 2.372 | 58.070 | 0.09x |
| nested.json | json | 4.175 | 4.301 | 5.032 | 58.070 | 0.05x |
| wide_arrays.json | strata | 1.538 | 1.663 | 1.958 | 64.840 | 1.00x |
| wide_arrays.json | orjson | 2.055 | 2.209 | 2.628 | 64.840 | 0.75x |
| wide_arrays.json | msgspec | 2.988 | 3.155 | 4.140 | 64.840 | 0.53x |
| wide_arrays.json | ujson | 9.645 | 9.994 | 11.038 | 64.840 | 0.17x |
| wide_arrays.json | json | 31.922 | 32.743 | 33.522 | 64.840 | 0.05x |
| mixed.json | strata | 0.053 | 0.057 | 0.080 | 63.480 | 1.00x |
| mixed.json | orjson | 0.065 | 0.073 | 0.086 | 63.480 | 0.78x |
| mixed.json | msgspec | 0.094 | 0.099 | 0.110 | 63.480 | 0.57x |
| mixed.json | ujson | 0.421 | 0.425 | 0.437 | 63.480 | 0.13x |
| mixed.json | json | 0.898 | 0.911 | 0.954 | 63.480 | 0.06x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 17.177 | 17.361 | 18.406 | 63.688 | 1.00x |
| users.json | orjson | 22.922 | 23.410 | 24.383 | 63.688 | 0.74x |
| users.json | msgspec | 23.085 | 23.154 | 24.073 | 63.688 | 0.75x |
| users.json | ujson | 34.619 | 35.338 | 37.034 | 63.688 | 0.49x |
| users.json | json | 41.005 | 41.706 | 42.391 | 63.688 | 0.42x |
| flat.json | strata | 1.175 | 1.228 | 1.331 | 64.270 | 1.00x |
| flat.json | orjson | 1.308 | 1.401 | 1.819 | 64.270 | 0.88x |
| flat.json | msgspec | 1.500 | 1.566 | 1.686 | 64.270 | 0.78x |
| flat.json | ujson | 2.557 | 2.797 | 4.085 | 64.270 | 0.44x |
| flat.json | json | 2.983 | 3.133 | 3.211 | 64.270 | 0.39x |
| nested.json | strata | 1.396 | 1.507 | 1.703 | 58.352 | 1.00x |
| nested.json | orjson | 1.606 | 1.726 | 2.095 | 58.352 | 0.87x |
| nested.json | msgspec | 1.752 | 1.865 | 2.153 | 58.352 | 0.81x |
| nested.json | ujson | 2.838 | 3.014 | 3.146 | 58.352 | 0.50x |
| nested.json | json | 3.600 | 3.847 | 4.568 | 58.352 | 0.39x |
| wide_arrays.json | strata | 6.639 | 6.863 | 7.078 | 66.992 | 1.00x |
| wide_arrays.json | orjson | 8.513 | 8.843 | 9.459 | 66.992 | 0.78x |
| wide_arrays.json | msgspec | 9.297 | 9.572 | 10.108 | 66.992 | 0.72x |
| wide_arrays.json | ujson | 12.134 | 12.300 | 12.857 | 66.992 | 0.56x |
| wide_arrays.json | json | 15.429 | 16.081 | 16.705 | 66.992 | 0.43x |
| mixed.json | strata | 0.361 | 0.369 | 0.433 | 63.480 | 1.00x |
| mixed.json | orjson | 0.469 | 0.484 | 0.562 | 63.480 | 0.76x |
| mixed.json | msgspec | 0.498 | 0.506 | 0.572 | 63.480 | 0.73x |
| mixed.json | ujson | 0.663 | 0.670 | 0.724 | 63.480 | 0.55x |
| mixed.json | json | 0.883 | 0.896 | 0.963 | 63.480 | 0.41x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 17.250 | 17.966 | 18.673 | 67.020 | 1.00x |
| users.ndjson | orjson | 24.894 | 26.094 | 27.688 | 67.020 | 0.69x |
| users.ndjson | msgspec | 25.637 | 26.405 | 27.805 | 67.020 | 0.68x |
| users.ndjson | ujson | 36.765 | 37.326 | 39.327 | 67.020 | 0.48x |
| users.ndjson | json | 45.240 | 46.575 | 49.087 | 67.020 | 0.39x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.087 | 3.320 | 3.862 | 63.762 | 1.00x |
| users.json | orjson | 3.956 | 4.195 | 46.859 | 63.762 | 0.79x |
| users.json | msgspec | 5.802 | 5.932 | 48.808 | 63.762 | 0.56x |
| users.json | ujson | 24.278 | 24.910 | 67.349 | 63.762 | 0.13x |
| users.json | json | 40.868 | 41.247 | 84.048 | 63.762 | 0.08x |
| flat.json | strata | 0.536 | 0.582 | 0.629 | 64.270 | 1.00x |
| flat.json | orjson | 0.621 | 0.634 | 0.853 | 64.270 | 0.92x |
| flat.json | msgspec | 0.740 | 0.766 | 0.927 | 64.270 | 0.76x |
| flat.json | ujson | 2.347 | 2.426 | 2.557 | 64.270 | 0.24x |
| flat.json | json | 3.701 | 3.817 | 4.087 | 64.270 | 0.15x |
| nested.json | strata | 0.520 | 0.539 | 0.579 | 58.352 | 1.00x |
| nested.json | orjson | 0.633 | 0.696 | 0.746 | 58.352 | 0.77x |
| nested.json | msgspec | 0.827 | 0.877 | 0.958 | 58.352 | 0.61x |
| nested.json | ujson | 2.552 | 2.610 | 3.008 | 58.352 | 0.21x |
| nested.json | json | 4.738 | 4.832 | 5.618 | 58.352 | 0.11x |
| wide_arrays.json | strata | 2.087 | 2.225 | 2.990 | 66.992 | 1.00x |
| wide_arrays.json | orjson | 2.829 | 2.919 | 3.296 | 66.992 | 0.76x |
| wide_arrays.json | msgspec | 3.610 | 3.692 | 4.086 | 66.992 | 0.60x |
| wide_arrays.json | ujson | 10.477 | 10.889 | 17.036 | 66.992 | 0.20x |
| wide_arrays.json | json | 33.336 | 33.857 | 36.502 | 66.992 | 0.07x |
| mixed.json | strata | 0.245 | 0.275 | 0.364 | 63.480 | 1.00x |
| mixed.json | orjson | 0.283 | 0.306 | 0.376 | 63.480 | 0.90x |
| mixed.json | msgspec | 0.309 | 0.318 | 0.406 | 63.480 | 0.86x |
| mixed.json | ujson | 0.651 | 0.698 | 0.743 | 63.480 | 0.39x |
| mixed.json | json | 1.117 | 1.151 | 1.316 | 63.480 | 0.24x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.121 | 0.125 | 0.146 | 63.832 | 1.00x |
| users.json $[*].id | jmespath | 0.862 | 0.869 | 0.946 | 63.832 | 0.14x |
| users.json $[*].id | jsonpath-ng | 4.856 | 4.921 | 5.072 | 63.832 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.745 | 0.767 | 0.885 | 61.277 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.304 | 5.533 | 5.794 | 61.277 | 0.14x |
| users.json $[*].orders[*].total | jsonpath-ng | 31.560 | 32.774 | 34.161 | 61.277 | 0.02x |
| users.json $..total | strata | 2.949 | 2.997 | 3.166 | 61.340 | 1.00x |
| users.json $..total | jsonpath-ng | 648.481 | 650.235 | 653.689 | 61.340 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.750 | 3.778 | 4.286 | 63.910 | 1.00x |
| users.json $[*].id | orjson+jmespath | 23.648 | 25.092 | 26.850 | 63.910 | 0.15x |
| users.json $[*].id | orjson+jsonpath-ng | 27.561 | 28.271 | 30.923 | 63.910 | 0.13x |
| users.json $[*].orders[*].total | strata | 3.993 | 4.081 | 4.330 | 61.277 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 30.547 | 32.005 | 35.017 | 61.277 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 62.058 | 67.949 | 79.734 | 61.277 | 0.06x |
| users.json $..total | strata | 20.740 | 21.130 | 22.697 | 61.340 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 681.438 | 686.761 | 692.699 | 61.340 | 0.03x |

