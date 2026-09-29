# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 38eaa9f248f509d9a4beb75b267189d011c1109e
- python: 3.12.10
- implementation: CPython
- platform: macOS-15.7.9-x86_64-i386-64bit
- machine: x86_64
- processor: Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz
- compiler_flags: -fno-strict-overflow -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -O3 -Wall -arch x86_64 -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -fomit-frame-pointer -march=native -flto=thin -fprofile-use=/Users/runner/work/_temp/strata-arm/build/pgo/strata.profdata -bundle -undefined (19 recorded commands; see the JSON companion)
- repeats: 60
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 16.952 | 20.030 | 25.360 | 56.785 | 1.00x |
| users.json | orjson | 24.460 | 31.527 | 39.526 | 56.785 | 0.64x |
| users.json | msgspec | 23.985 | 30.445 | 39.863 | 56.785 | 0.66x |
| users.json | ujson | 35.386 | 42.727 | 57.981 | 56.785 | 0.47x |
| users.json | pysimdjson | 155.561 | 183.920 | 224.104 | 56.785 | 0.11x |
| users.json | json | 41.173 | 49.791 | 65.136 | 56.785 | 0.40x |
| flat.json | strata | 1.279 | 1.470 | 2.085 | 70.191 | 1.00x |
| flat.json | orjson | 1.448 | 1.598 | 2.264 | 70.191 | 0.92x |
| flat.json | msgspec | 1.650 | 1.920 | 2.426 | 70.191 | 0.77x |
| flat.json | ujson | 2.849 | 3.300 | 4.339 | 70.191 | 0.45x |
| flat.json | pysimdjson | 15.133 | 16.807 | 19.223 | 70.191 | 0.09x |
| flat.json | json | 3.234 | 3.847 | 4.985 | 70.191 | 0.38x |
| nested.json | strata | 1.581 | 1.870 | 2.499 | 60.930 | 1.00x |
| nested.json | orjson | 1.809 | 2.133 | 2.771 | 60.930 | 0.88x |
| nested.json | msgspec | 1.994 | 2.359 | 3.084 | 60.930 | 0.79x |
| nested.json | ujson | 3.254 | 3.964 | 4.839 | 60.930 | 0.47x |
| nested.json | pysimdjson | 14.130 | 15.870 | 19.370 | 60.930 | 0.12x |
| nested.json | json | 4.195 | 4.857 | 6.329 | 60.930 | 0.39x |
| wide_arrays.json | strata | 7.559 | 8.208 | 9.954 | 70.867 | 1.00x |
| wide_arrays.json | orjson | 9.525 | 11.130 | 13.275 | 70.867 | 0.74x |
| wide_arrays.json | msgspec | 10.139 | 11.602 | 15.437 | 70.867 | 0.71x |
| wide_arrays.json | ujson | 13.002 | 14.776 | 19.376 | 70.867 | 0.56x |
| wide_arrays.json | pysimdjson | 79.764 | 83.842 | 100.700 | 70.867 | 0.10x |
| wide_arrays.json | json | 16.989 | 18.650 | 22.349 | 70.867 | 0.44x |
| mixed.json | strata | 0.371 | 0.403 | 0.506 | 67.297 | 1.00x |
| mixed.json | orjson | 0.464 | 0.500 | 0.688 | 67.297 | 0.81x |
| mixed.json | msgspec | 0.488 | 0.524 | 0.714 | 67.297 | 0.77x |
| mixed.json | ujson | 0.652 | 0.737 | 1.005 | 67.297 | 0.55x |
| mixed.json | pysimdjson | 3.357 | 3.533 | 4.271 | 67.297 | 0.11x |
| mixed.json | json | 0.931 | 0.995 | 1.277 | 67.297 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.988 | 3.370 | 4.226 | 52.574 | 1.00x |
| users.json | orjson | 3.812 | 4.366 | 6.125 | 52.574 | 0.77x |
| users.json | msgspec | 6.247 | 7.047 | 8.693 | 52.574 | 0.48x |
| users.json | ujson | 25.579 | 28.795 | 34.137 | 52.574 | 0.12x |
| users.json | json | 45.066 | 51.945 | 58.817 | 52.574 | 0.06x |
| flat.json | strata | 0.337 | 0.369 | 0.511 | 60.969 | 1.00x |
| flat.json | orjson | 0.415 | 0.457 | 0.632 | 60.969 | 0.81x |
| flat.json | msgspec | 0.552 | 0.591 | 0.782 | 60.969 | 0.62x |
| flat.json | ujson | 2.221 | 2.337 | 2.723 | 60.969 | 0.16x |
| flat.json | json | 3.637 | 3.811 | 4.785 | 60.969 | 0.10x |
| nested.json | strata | 0.221 | 0.260 | 0.320 | 60.980 | 1.00x |
| nested.json | orjson | 0.346 | 0.397 | 0.488 | 60.980 | 0.65x |
| nested.json | msgspec | 0.549 | 0.603 | 0.716 | 60.980 | 0.43x |
| nested.json | ujson | 2.244 | 2.419 | 2.623 | 60.980 | 0.11x |
| nested.json | json | 4.494 | 4.853 | 5.319 | 60.980 | 0.05x |
| wide_arrays.json | strata | 2.220 | 2.930 | 3.484 | 67.387 | 1.00x |
| wide_arrays.json | orjson | 2.832 | 3.602 | 4.598 | 67.387 | 0.81x |
| wide_arrays.json | msgspec | 3.787 | 4.848 | 5.921 | 67.387 | 0.60x |
| wide_arrays.json | ujson | 11.328 | 13.862 | 18.117 | 67.387 | 0.21x |
| wide_arrays.json | json | 36.424 | 46.245 | 57.359 | 67.387 | 0.06x |
| mixed.json | strata | 0.063 | 0.080 | 0.093 | 62.871 | 1.00x |
| mixed.json | orjson | 0.073 | 0.096 | 0.119 | 62.871 | 0.83x |
| mixed.json | msgspec | 0.103 | 0.133 | 0.164 | 62.871 | 0.60x |
| mixed.json | ujson | 0.447 | 0.480 | 0.510 | 62.871 | 0.17x |
| mixed.json | json | 0.937 | 0.997 | 1.064 | 62.871 | 0.08x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 18.910 | 21.633 | 25.615 | 65.836 | 1.00x |
| users.json | orjson | 28.406 | 33.862 | 41.482 | 65.836 | 0.64x |
| users.json | msgspec | 29.202 | 33.994 | 40.848 | 65.836 | 0.64x |
| users.json | ujson | 41.183 | 47.190 | 57.703 | 65.836 | 0.46x |
| users.json | json | 47.119 | 53.105 | 62.887 | 65.836 | 0.41x |
| flat.json | strata | 1.378 | 1.547 | 2.151 | 60.969 | 1.00x |
| flat.json | orjson | 1.593 | 1.746 | 2.209 | 60.969 | 0.89x |
| flat.json | msgspec | 1.786 | 2.042 | 2.433 | 60.969 | 0.76x |
| flat.json | ujson | 2.954 | 3.496 | 4.452 | 60.969 | 0.44x |
| flat.json | json | 3.303 | 3.909 | 4.952 | 60.969 | 0.40x |
| nested.json | strata | 1.502 | 1.588 | 1.766 | 60.980 | 1.00x |
| nested.json | orjson | 1.739 | 1.838 | 2.134 | 60.980 | 0.86x |
| nested.json | msgspec | 1.910 | 2.031 | 2.425 | 60.980 | 0.78x |
| nested.json | ujson | 3.071 | 3.206 | 3.599 | 60.980 | 0.50x |
| nested.json | json | 3.857 | 4.038 | 4.357 | 60.980 | 0.39x |
| wide_arrays.json | strata | 8.027 | 8.703 | 10.343 | 67.387 | 1.00x |
| wide_arrays.json | orjson | 9.982 | 11.659 | 13.744 | 67.387 | 0.75x |
| wide_arrays.json | msgspec | 10.965 | 12.410 | 14.986 | 67.387 | 0.70x |
| wide_arrays.json | ujson | 14.262 | 16.023 | 20.191 | 67.387 | 0.54x |
| wide_arrays.json | json | 18.272 | 20.456 | 26.123 | 67.387 | 0.43x |
| mixed.json | strata | 0.428 | 0.520 | 0.755 | 62.871 | 1.00x |
| mixed.json | orjson | 0.540 | 0.667 | 0.962 | 62.871 | 0.78x |
| mixed.json | msgspec | 0.583 | 0.697 | 1.049 | 62.871 | 0.75x |
| mixed.json | ujson | 0.775 | 0.882 | 1.334 | 62.871 | 0.59x |
| mixed.json | json | 0.992 | 1.129 | 1.643 | 62.871 | 0.46x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 19.479 | 21.870 | 26.256 | 69.520 | 1.00x |
| users.ndjson | orjson | 29.140 | 33.927 | 41.773 | 69.520 | 0.64x |
| users.ndjson | msgspec | 29.835 | 34.008 | 41.045 | 69.520 | 0.64x |
| users.ndjson | ujson | 42.293 | 50.115 | 58.690 | 69.520 | 0.44x |
| users.ndjson | json | 51.847 | 60.677 | 75.333 | 69.520 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.655 | 4.112 | 5.376 | 61.668 | 1.00x |
| users.json | orjson | 4.541 | 5.325 | 6.900 | 61.668 | 0.77x |
| users.json | msgspec | 6.580 | 7.421 | 9.497 | 61.668 | 0.55x |
| users.json | ujson | 26.312 | 28.586 | 33.863 | 61.668 | 0.14x |
| users.json | json | 44.473 | 49.460 | 57.332 | 61.668 | 0.08x |
| flat.json | strata | 0.701 | 0.818 | 1.061 | 60.969 | 1.00x |
| flat.json | orjson | 0.796 | 0.965 | 1.253 | 60.969 | 0.85x |
| flat.json | msgspec | 0.970 | 1.111 | 1.367 | 60.969 | 0.74x |
| flat.json | ujson | 2.777 | 3.027 | 3.850 | 60.969 | 0.27x |
| flat.json | json | 4.305 | 4.984 | 6.087 | 60.969 | 0.16x |
| nested.json | strata | 0.511 | 0.580 | 0.721 | 60.980 | 1.00x |
| nested.json | orjson | 0.640 | 0.768 | 0.895 | 60.980 | 0.75x |
| nested.json | msgspec | 0.866 | 0.975 | 1.178 | 60.980 | 0.60x |
| nested.json | ujson | 2.579 | 2.778 | 3.354 | 60.980 | 0.21x |
| nested.json | json | 4.744 | 5.225 | 6.072 | 60.980 | 0.11x |
| wide_arrays.json | strata | 2.985 | 3.501 | 4.278 | 67.387 | 1.00x |
| wide_arrays.json | orjson | 3.788 | 4.263 | 5.135 | 67.387 | 0.82x |
| wide_arrays.json | msgspec | 4.487 | 5.038 | 5.957 | 67.387 | 0.69x |
| wide_arrays.json | ujson | 12.107 | 13.320 | 16.259 | 67.387 | 0.26x |
| wide_arrays.json | json | 36.407 | 39.664 | 47.015 | 67.387 | 0.09x |
| mixed.json | strata | 0.388 | 0.462 | 0.611 | 62.871 | 1.00x |
| mixed.json | orjson | 0.440 | 0.517 | 0.757 | 62.871 | 0.89x |
| mixed.json | msgspec | 0.468 | 0.577 | 0.781 | 62.871 | 0.80x |
| mixed.json | ujson | 0.870 | 0.990 | 1.337 | 62.871 | 0.47x |
| mixed.json | json | 1.386 | 1.578 | 2.165 | 62.871 | 0.29x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.185 | 0.232 | 0.383 | 61.738 | 1.00x |
| users.json $[*].id | jmespath | 1.010 | 1.155 | 1.768 | 61.738 | 0.20x |
| users.json $[*].id | jsonpath-ng | 5.398 | 6.243 | 9.764 | 61.738 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.886 | 1.299 | 1.674 | 62.367 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.817 | 6.988 | 8.565 | 62.367 | 0.19x |
| users.json $[*].orders[*].total | jsonpath-ng | 35.832 | 40.734 | 49.721 | 62.367 | 0.03x |
| users.json $..total | strata | 3.551 | 4.403 | 5.562 | 62.395 | 1.00x |
| users.json $..total | jsonpath-ng | 706.819 | 811.295 | 955.492 | 62.395 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.790 | 4.087 | 5.193 | 65.582 | 1.00x |
| users.json $[*].id | orjson+jmespath | 27.428 | 31.760 | 39.277 | 65.582 | 0.13x |
| users.json $[*].id | orjson+jsonpath-ng | 33.090 | 37.499 | 43.430 | 65.582 | 0.11x |
| users.json $[*].orders[*].total | strata | 4.014 | 4.200 | 5.159 | 62.395 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 29.474 | 34.159 | 42.171 | 62.395 | 0.12x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 63.527 | 73.695 | 85.175 | 62.395 | 0.06x |
| users.json $..total | strata | 23.449 | 26.616 | 34.785 | 62.395 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 775.423 | 861.614 | 964.347 | 62.395 | 0.03x |

