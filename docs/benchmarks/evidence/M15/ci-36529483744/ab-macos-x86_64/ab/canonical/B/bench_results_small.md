# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: afd1550cbabc9433e8292644444a23bb27bbc4e9
- python: 3.12.10
- implementation: CPython
- platform: macOS-15.7.9-x86_64-i386-64bit
- machine: x86_64
- processor: Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz
- compiler_flags: -fno-strict-overflow -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -O3 -Wall -arch x86_64 -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -fomit-frame-pointer -march=native -flto=thin -fprofile-use=/Users/runner/work/_temp/strata-arm/build/pgo/strata.profdata -bundle -undefined (24 recorded commands; see the JSON companion)
- repeats: 60
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 16.945 | 19.528 | 22.582 | 56.734 | 1.00x |
| users.json | orjson | 23.585 | 28.873 | 35.395 | 56.734 | 0.68x |
| users.json | msgspec | 23.680 | 28.931 | 33.085 | 56.734 | 0.67x |
| users.json | ujson | 35.476 | 42.078 | 48.941 | 56.734 | 0.46x |
| users.json | pysimdjson | 158.003 | 177.490 | 188.785 | 56.734 | 0.11x |
| users.json | json | 41.448 | 47.841 | 55.670 | 56.734 | 0.41x |
| flat.json | strata | 1.164 | 1.228 | 1.317 | 68.395 | 1.00x |
| flat.json | orjson | 1.296 | 1.374 | 1.509 | 68.395 | 0.89x |
| flat.json | msgspec | 1.495 | 1.567 | 1.719 | 68.395 | 0.78x |
| flat.json | ujson | 2.611 | 2.752 | 3.034 | 68.395 | 0.45x |
| flat.json | pysimdjson | 14.020 | 14.352 | 14.958 | 68.395 | 0.09x |
| flat.json | json | 3.041 | 3.120 | 3.418 | 68.395 | 0.39x |
| nested.json | strata | 1.476 | 1.553 | 1.846 | 61.344 | 1.00x |
| nested.json | orjson | 1.670 | 1.784 | 2.170 | 61.344 | 0.87x |
| nested.json | msgspec | 1.838 | 1.964 | 2.289 | 61.344 | 0.79x |
| nested.json | ujson | 3.067 | 3.202 | 3.697 | 61.344 | 0.48x |
| nested.json | pysimdjson | 13.641 | 13.933 | 14.903 | 61.344 | 0.11x |
| nested.json | json | 3.916 | 4.136 | 4.651 | 61.344 | 0.38x |
| wide_arrays.json | strata | 7.097 | 7.723 | 8.386 | 70.258 | 1.00x |
| wide_arrays.json | orjson | 8.804 | 9.889 | 11.013 | 70.258 | 0.78x |
| wide_arrays.json | msgspec | 9.414 | 10.654 | 11.630 | 70.258 | 0.72x |
| wide_arrays.json | ujson | 11.851 | 13.360 | 14.400 | 70.258 | 0.58x |
| wide_arrays.json | pysimdjson | 76.172 | 80.421 | 83.778 | 70.258 | 0.10x |
| wide_arrays.json | json | 15.634 | 17.643 | 18.688 | 70.258 | 0.44x |
| mixed.json | strata | 0.319 | 0.377 | 0.426 | 68.234 | 1.00x |
| mixed.json | orjson | 0.394 | 0.453 | 0.580 | 68.234 | 0.83x |
| mixed.json | msgspec | 0.422 | 0.460 | 0.531 | 68.234 | 0.82x |
| mixed.json | ujson | 0.573 | 0.660 | 0.921 | 68.234 | 0.57x |
| mixed.json | pysimdjson | 3.003 | 3.244 | 3.874 | 68.234 | 0.12x |
| mixed.json | json | 0.815 | 0.907 | 1.116 | 68.234 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.276 | 2.768 | 3.451 | 47.699 | 1.00x |
| users.json | orjson | 3.197 | 3.628 | 4.477 | 47.699 | 0.76x |
| users.json | msgspec | 4.795 | 5.478 | 6.843 | 47.699 | 0.51x |
| users.json | ujson | 23.037 | 25.371 | 29.000 | 47.699 | 0.11x |
| users.json | json | 39.309 | 43.604 | 47.322 | 47.699 | 0.06x |
| flat.json | strata | 0.328 | 0.366 | 0.447 | 66.578 | 1.00x |
| flat.json | orjson | 0.403 | 0.459 | 0.580 | 66.578 | 0.80x |
| flat.json | msgspec | 0.538 | 0.599 | 0.687 | 66.578 | 0.61x |
| flat.json | ujson | 2.198 | 2.360 | 2.714 | 66.578 | 0.16x |
| flat.json | json | 3.606 | 3.877 | 4.327 | 66.578 | 0.09x |
| nested.json | strata | 0.247 | 0.292 | 0.404 | 61.480 | 1.00x |
| nested.json | orjson | 0.364 | 0.413 | 0.681 | 61.480 | 0.71x |
| nested.json | msgspec | 0.567 | 0.629 | 0.924 | 61.480 | 0.46x |
| nested.json | ujson | 2.400 | 2.512 | 3.501 | 61.480 | 0.12x |
| nested.json | json | 4.839 | 5.066 | 6.549 | 61.480 | 0.06x |
| wide_arrays.json | strata | 1.651 | 2.022 | 2.347 | 69.172 | 1.00x |
| wide_arrays.json | orjson | 2.201 | 2.475 | 3.061 | 69.172 | 0.82x |
| wide_arrays.json | msgspec | 3.170 | 3.479 | 3.991 | 69.172 | 0.58x |
| wide_arrays.json | ujson | 9.659 | 10.410 | 10.825 | 69.172 | 0.19x |
| wide_arrays.json | json | 32.125 | 33.572 | 35.990 | 69.172 | 0.06x |
| mixed.json | strata | 0.059 | 0.073 | 0.088 | 63.805 | 1.00x |
| mixed.json | orjson | 0.071 | 0.089 | 0.106 | 63.805 | 0.82x |
| mixed.json | msgspec | 0.099 | 0.123 | 0.146 | 63.805 | 0.60x |
| mixed.json | ujson | 0.434 | 0.455 | 0.483 | 63.805 | 0.16x |
| mixed.json | json | 0.911 | 0.951 | 1.007 | 63.805 | 0.08x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 17.625 | 18.859 | 20.508 | 62.711 | 1.00x |
| users.json | orjson | 24.293 | 27.850 | 31.807 | 62.711 | 0.68x |
| users.json | msgspec | 24.381 | 27.840 | 31.496 | 62.711 | 0.68x |
| users.json | ujson | 35.979 | 40.489 | 44.187 | 62.711 | 0.47x |
| users.json | json | 40.926 | 45.455 | 50.260 | 62.711 | 0.41x |
| flat.json | strata | 1.404 | 1.461 | 1.803 | 66.578 | 1.00x |
| flat.json | orjson | 1.608 | 1.678 | 1.812 | 66.578 | 0.87x |
| flat.json | msgspec | 1.837 | 1.884 | 2.206 | 66.578 | 0.78x |
| flat.json | ujson | 3.114 | 3.182 | 3.470 | 66.578 | 0.46x |
| flat.json | json | 3.446 | 3.534 | 3.853 | 66.578 | 0.41x |
| nested.json | strata | 1.486 | 1.628 | 1.816 | 61.480 | 1.00x |
| nested.json | orjson | 1.719 | 1.858 | 2.290 | 61.480 | 0.88x |
| nested.json | msgspec | 1.881 | 2.064 | 2.274 | 61.480 | 0.79x |
| nested.json | ujson | 3.037 | 3.264 | 3.810 | 61.480 | 0.50x |
| nested.json | json | 3.807 | 4.074 | 4.589 | 61.480 | 0.40x |
| wide_arrays.json | strata | 7.029 | 7.741 | 8.328 | 69.172 | 1.00x |
| wide_arrays.json | orjson | 8.958 | 9.935 | 10.890 | 69.172 | 0.78x |
| wide_arrays.json | msgspec | 9.765 | 10.835 | 12.072 | 69.172 | 0.71x |
| wide_arrays.json | ujson | 12.746 | 13.776 | 14.743 | 69.172 | 0.56x |
| wide_arrays.json | json | 16.770 | 17.874 | 19.303 | 69.172 | 0.43x |
| mixed.json | strata | 0.408 | 0.468 | 0.488 | 63.805 | 1.00x |
| mixed.json | orjson | 0.530 | 0.597 | 0.670 | 63.805 | 0.78x |
| mixed.json | msgspec | 0.555 | 0.638 | 0.676 | 63.805 | 0.73x |
| mixed.json | ujson | 0.737 | 0.820 | 0.932 | 63.805 | 0.57x |
| mixed.json | json | 0.971 | 1.059 | 1.250 | 63.805 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 17.869 | 19.041 | 19.957 | 67.770 | 1.00x |
| users.ndjson | orjson | 25.301 | 27.421 | 29.706 | 67.770 | 0.69x |
| users.ndjson | msgspec | 26.394 | 28.237 | 30.637 | 67.770 | 0.67x |
| users.ndjson | ujson | 37.351 | 40.291 | 42.795 | 67.770 | 0.47x |
| users.ndjson | json | 46.665 | 50.737 | 53.603 | 67.770 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.233 | 3.623 | 4.560 | 62.922 | 1.00x |
| users.json | orjson | 4.284 | 4.650 | 5.262 | 62.922 | 0.78x |
| users.json | msgspec | 5.863 | 6.394 | 7.129 | 62.922 | 0.57x |
| users.json | ujson | 24.444 | 26.110 | 27.372 | 62.922 | 0.14x |
| users.json | json | 40.461 | 43.276 | 46.215 | 62.922 | 0.08x |
| flat.json | strata | 0.681 | 0.739 | 0.858 | 66.578 | 1.00x |
| flat.json | orjson | 0.776 | 0.864 | 1.170 | 66.578 | 0.86x |
| flat.json | msgspec | 0.909 | 1.021 | 1.194 | 66.578 | 0.72x |
| flat.json | ujson | 2.674 | 2.835 | 3.340 | 66.578 | 0.26x |
| flat.json | json | 4.074 | 4.336 | 5.413 | 66.578 | 0.17x |
| nested.json | strata | 0.475 | 0.548 | 0.645 | 61.480 | 1.00x |
| nested.json | orjson | 0.621 | 0.707 | 0.797 | 61.480 | 0.78x |
| nested.json | msgspec | 0.796 | 0.890 | 1.071 | 61.480 | 0.62x |
| nested.json | ujson | 2.494 | 2.644 | 2.998 | 61.480 | 0.21x |
| nested.json | json | 4.696 | 4.975 | 5.442 | 61.480 | 0.11x |
| wide_arrays.json | strata | 2.330 | 2.774 | 2.996 | 69.172 | 1.00x |
| wide_arrays.json | orjson | 2.967 | 3.284 | 3.641 | 69.172 | 0.84x |
| wide_arrays.json | msgspec | 3.851 | 4.435 | 4.811 | 69.172 | 0.63x |
| wide_arrays.json | ujson | 10.625 | 11.533 | 12.147 | 69.172 | 0.24x |
| wide_arrays.json | json | 33.508 | 35.750 | 37.497 | 69.172 | 0.08x |
| mixed.json | strata | 0.264 | 0.354 | 0.402 | 63.805 | 1.00x |
| mixed.json | orjson | 0.312 | 0.392 | 0.457 | 63.805 | 0.90x |
| mixed.json | msgspec | 0.339 | 0.411 | 0.498 | 63.805 | 0.86x |
| mixed.json | ujson | 0.707 | 0.787 | 0.869 | 63.805 | 0.45x |
| mixed.json | json | 1.175 | 1.276 | 1.562 | 63.805 | 0.28x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.124 | 0.146 | 0.216 | 63.004 | 1.00x |
| users.json $[*].id | jmespath | 0.903 | 0.949 | 1.084 | 63.004 | 0.15x |
| users.json $[*].id | jsonpath-ng | 4.895 | 5.104 | 5.706 | 63.004 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.777 | 1.005 | 1.311 | 61.438 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.491 | 6.083 | 7.141 | 61.438 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 32.551 | 36.547 | 39.417 | 61.438 | 0.03x |
| users.json $..total | strata | 2.989 | 3.546 | 3.936 | 61.461 | 1.00x |
| users.json $..total | jsonpath-ng | 688.219 | 722.124 | 753.763 | 61.461 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.606 | 3.820 | 4.160 | 63.074 | 1.00x |
| users.json $[*].id | orjson+jmespath | 24.297 | 28.137 | 31.784 | 63.074 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 30.013 | 32.800 | 36.219 | 63.074 | 0.12x |
| users.json $[*].orders[*].total | strata | 4.038 | 4.216 | 4.705 | 61.457 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 29.763 | 33.058 | 38.676 | 61.457 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 61.829 | 69.630 | 78.788 | 61.457 | 0.06x |
| users.json $..total | strata | 21.693 | 22.787 | 23.902 | 61.461 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 720.136 | 756.545 | 784.812 | 61.461 | 0.03x |

