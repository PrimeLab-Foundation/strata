# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: b6e2a27
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
| users.json | strata | 16.781 | 17.084 | 19.514 | 52.688 | 1.00x |
| users.json | orjson | 23.142 | 23.510 | 26.932 | 52.688 | 0.73x |
| users.json | msgspec | 20.932 | 23.534 | 28.243 | 52.688 | 0.73x |
| users.json | ujson | 34.325 | 36.579 | 52.553 | 52.688 | 0.47x |
| users.json | pysimdjson | 150.298 | 152.188 | 167.182 | 52.688 | 0.11x |
| users.json | json | 38.458 | 40.868 | 47.275 | 52.688 | 0.42x |
| flat.json | strata | 1.057 | 1.170 | 1.215 | 58.090 | 1.00x |
| flat.json | orjson | 1.180 | 1.314 | 1.353 | 58.090 | 0.89x |
| flat.json | msgspec | 1.352 | 1.476 | 1.521 | 58.090 | 0.79x |
| flat.json | ujson | 2.336 | 2.579 | 2.916 | 58.090 | 0.45x |
| flat.json | pysimdjson | 13.137 | 13.858 | 14.080 | 58.090 | 0.08x |
| flat.json | json | 2.730 | 2.986 | 3.204 | 58.090 | 0.39x |
| nested.json | strata | 1.209 | 1.342 | 1.441 | 46.484 | 1.00x |
| nested.json | orjson | 1.434 | 1.560 | 1.653 | 46.484 | 0.86x |
| nested.json | msgspec | 1.562 | 1.710 | 2.207 | 46.484 | 0.78x |
| nested.json | ujson | 2.719 | 2.846 | 3.203 | 46.484 | 0.47x |
| nested.json | pysimdjson | 11.856 | 12.648 | 13.913 | 46.484 | 0.11x |
| nested.json | json | 3.413 | 3.641 | 4.365 | 46.484 | 0.37x |
| wide_arrays.json | strata | 6.615 | 7.242 | 7.426 | 57.934 | 1.00x |
| wide_arrays.json | orjson | 7.944 | 8.800 | 9.608 | 57.934 | 0.82x |
| wide_arrays.json | msgspec | 8.770 | 9.487 | 10.296 | 57.934 | 0.76x |
| wide_arrays.json | ujson | 11.164 | 12.006 | 12.816 | 57.934 | 0.60x |
| wide_arrays.json | pysimdjson | 74.201 | 74.405 | 76.590 | 57.934 | 0.10x |
| wide_arrays.json | json | 14.695 | 15.810 | 17.509 | 57.934 | 0.46x |
| mixed.json | strata | 0.324 | 0.332 | 0.384 | 53.191 | 1.00x |
| mixed.json | orjson | 0.402 | 0.414 | 0.651 | 53.191 | 0.80x |
| mixed.json | msgspec | 0.424 | 0.437 | 0.524 | 53.191 | 0.76x |
| mixed.json | ujson | 0.583 | 0.596 | 0.637 | 53.191 | 0.56x |
| mixed.json | pysimdjson | 3.022 | 3.060 | 3.145 | 53.191 | 0.11x |
| mixed.json | json | 0.823 | 0.843 | 1.033 | 53.191 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.343 | 2.447 | 2.719 | 48.172 | 1.00x |
| users.json | orjson | 3.274 | 3.316 | 5.634 | 48.172 | 0.74x |
| users.json | msgspec | 4.943 | 5.182 | 6.521 | 48.172 | 0.47x |
| users.json | ujson | 23.154 | 23.424 | 26.766 | 48.172 | 0.10x |
| users.json | json | 39.386 | 39.983 | 60.663 | 48.172 | 0.06x |
| flat.json | strata | 0.232 | 0.273 | 0.306 | 46.027 | 1.00x |
| flat.json | orjson | 0.350 | 0.394 | 0.572 | 46.027 | 0.69x |
| flat.json | msgspec | 0.429 | 0.487 | 0.533 | 46.027 | 0.56x |
| flat.json | ujson | 2.026 | 2.057 | 2.338 | 46.027 | 0.13x |
| flat.json | json | 3.149 | 3.397 | 3.652 | 46.027 | 0.08x |
| nested.json | strata | 0.198 | 0.226 | 0.240 | 46.340 | 1.00x |
| nested.json | orjson | 0.288 | 0.324 | 0.838 | 46.340 | 0.70x |
| nested.json | msgspec | 0.479 | 0.523 | 0.775 | 46.340 | 0.43x |
| nested.json | ujson | 2.004 | 2.175 | 2.559 | 46.340 | 0.10x |
| nested.json | json | 4.139 | 4.390 | 4.410 | 46.340 | 0.05x |
| wide_arrays.json | strata | 1.385 | 1.542 | 1.638 | 56.172 | 1.00x |
| wide_arrays.json | orjson | 1.865 | 2.065 | 3.149 | 56.172 | 0.75x |
| wide_arrays.json | msgspec | 2.708 | 2.963 | 3.062 | 56.172 | 0.52x |
| wide_arrays.json | ujson | 9.589 | 9.723 | 10.564 | 56.172 | 0.16x |
| wide_arrays.json | json | 31.449 | 32.050 | 33.348 | 56.172 | 0.05x |
| mixed.json | strata | 0.058 | 0.059 | 0.072 | 50.980 | 1.00x |
| mixed.json | orjson | 0.070 | 0.073 | 0.087 | 50.980 | 0.81x |
| mixed.json | msgspec | 0.098 | 0.100 | 0.102 | 50.980 | 0.59x |
| mixed.json | ujson | 0.422 | 0.424 | 0.440 | 50.980 | 0.14x |
| mixed.json | json | 0.889 | 0.893 | 0.948 | 50.980 | 0.07x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 17.822 | 18.096 | 19.209 | 60.621 | 1.00x |
| users.json | orjson | 23.699 | 23.924 | 24.824 | 60.621 | 0.76x |
| users.json | msgspec | 24.106 | 24.208 | 24.876 | 60.621 | 0.75x |
| users.json | ujson | 35.652 | 36.812 | 38.983 | 60.621 | 0.49x |
| users.json | json | 37.738 | 39.174 | 41.520 | 60.621 | 0.46x |
| flat.json | strata | 1.231 | 1.280 | 1.386 | 46.215 | 1.00x |
| flat.json | orjson | 1.275 | 1.432 | 1.574 | 46.215 | 0.89x |
| flat.json | msgspec | 1.453 | 1.627 | 1.707 | 46.215 | 0.79x |
| flat.json | ujson | 2.432 | 2.708 | 2.799 | 46.215 | 0.47x |
| flat.json | json | 2.865 | 3.100 | 3.235 | 46.215 | 0.41x |
| nested.json | strata | 1.363 | 1.441 | 1.509 | 46.340 | 1.00x |
| nested.json | orjson | 1.501 | 1.699 | 1.768 | 46.340 | 0.85x |
| nested.json | msgspec | 1.673 | 1.860 | 2.307 | 46.340 | 0.77x |
| nested.json | ujson | 2.721 | 3.007 | 3.404 | 46.340 | 0.48x |
| nested.json | json | 3.457 | 3.772 | 4.068 | 46.340 | 0.38x |
| wide_arrays.json | strata | 7.259 | 7.403 | 7.509 | 56.297 | 1.00x |
| wide_arrays.json | orjson | 8.137 | 9.038 | 10.358 | 56.297 | 0.82x |
| wide_arrays.json | msgspec | 8.897 | 9.915 | 10.127 | 56.297 | 0.75x |
| wide_arrays.json | ujson | 12.655 | 12.849 | 12.988 | 56.297 | 0.58x |
| wide_arrays.json | json | 16.222 | 16.447 | 16.791 | 56.297 | 0.45x |
| mixed.json | strata | 0.390 | 0.407 | 0.455 | 50.980 | 1.00x |
| mixed.json | orjson | 0.449 | 0.517 | 0.593 | 50.980 | 0.79x |
| mixed.json | msgspec | 0.461 | 0.533 | 0.608 | 50.980 | 0.76x |
| mixed.json | ujson | 0.606 | 0.697 | 0.829 | 50.980 | 0.58x |
| mixed.json | json | 0.817 | 0.920 | 0.986 | 50.980 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 17.466 | 17.816 | 19.527 | 58.562 | 1.00x |
| users.ndjson | orjson | 24.330 | 26.379 | 27.798 | 58.562 | 0.68x |
| users.ndjson | msgspec | 24.220 | 26.229 | 28.386 | 58.562 | 0.68x |
| users.ndjson | ujson | 36.895 | 37.589 | 38.948 | 58.562 | 0.47x |
| users.ndjson | json | 45.380 | 47.050 | 49.922 | 58.562 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.757 | 3.098 | 3.269 | 57.719 | 1.00x |
| users.json | orjson | 4.027 | 4.412 | 5.065 | 57.719 | 0.70x |
| users.json | msgspec | 5.753 | 6.001 | 6.311 | 57.719 | 0.52x |
| users.json | ujson | 23.502 | 24.402 | 25.048 | 57.719 | 0.13x |
| users.json | json | 39.537 | 40.434 | 41.895 | 57.719 | 0.08x |
| flat.json | strata | 0.574 | 0.625 | 0.704 | 46.242 | 1.00x |
| flat.json | orjson | 0.647 | 0.724 | 0.771 | 46.242 | 0.86x |
| flat.json | msgspec | 0.740 | 0.863 | 1.041 | 46.242 | 0.72x |
| flat.json | ujson | 2.169 | 2.457 | 2.546 | 46.242 | 0.25x |
| flat.json | json | 3.741 | 3.840 | 4.136 | 46.242 | 0.16x |
| nested.json | strata | 0.470 | 0.562 | 0.634 | 46.340 | 1.00x |
| nested.json | orjson | 0.543 | 0.691 | 0.785 | 46.340 | 0.81x |
| nested.json | msgspec | 0.742 | 0.903 | 0.964 | 46.340 | 0.62x |
| nested.json | ujson | 2.571 | 2.603 | 2.928 | 46.340 | 0.22x |
| nested.json | json | 4.667 | 4.866 | 5.210 | 46.340 | 0.12x |
| wide_arrays.json | strata | 1.988 | 2.175 | 2.337 | 56.297 | 1.00x |
| wide_arrays.json | orjson | 2.586 | 2.899 | 3.111 | 56.297 | 0.75x |
| wide_arrays.json | msgspec | 3.497 | 3.643 | 4.015 | 56.297 | 0.60x |
| wide_arrays.json | ujson | 9.954 | 10.452 | 11.151 | 56.297 | 0.21x |
| wide_arrays.json | json | 32.501 | 33.142 | 34.385 | 56.297 | 0.07x |
| mixed.json | strata | 0.255 | 0.306 | 0.499 | 50.980 | 1.00x |
| mixed.json | orjson | 0.276 | 0.335 | 0.414 | 50.980 | 0.91x |
| mixed.json | msgspec | 0.320 | 0.359 | 0.492 | 50.980 | 0.85x |
| mixed.json | ujson | 0.643 | 0.725 | 1.009 | 50.980 | 0.42x |
| mixed.json | json | 1.094 | 1.184 | 1.272 | 50.980 | 0.26x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.130 | 0.165 | 0.297 | 57.781 | 1.00x |
| users.json $[*].id | jmespath | 0.851 | 0.940 | 1.001 | 57.781 | 0.18x |
| users.json $[*].id | jsonpath-ng | 4.836 | 4.971 | 5.403 | 57.781 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.737 | 0.841 | 0.929 | 55.098 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.224 | 5.551 | 5.715 | 55.098 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 31.317 | 32.626 | 35.638 | 55.098 | 0.03x |
| users.json $..total | strata | 2.860 | 3.093 | 3.376 | 56.184 | 1.00x |
| users.json $..total | jsonpath-ng | 649.040 | 651.667 | 667.180 | 56.184 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.752 | 3.783 | 5.452 | 58.004 | 1.00x |
| users.json $[*].id | orjson+jmespath | 23.334 | 27.040 | 39.152 | 58.004 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 28.898 | 29.744 | 36.800 | 58.004 | 0.13x |
| users.json $[*].orders[*].total | strata | 3.943 | 3.968 | 4.045 | 56.273 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 27.012 | 29.896 | 32.544 | 56.273 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 58.118 | 63.669 | 71.130 | 56.273 | 0.06x |
| users.json $..total | strata | 20.232 | 21.147 | 22.189 | 56.375 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 681.760 | 689.477 | 748.737 | 56.375 | 0.03x |

