# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c89aea890fd23de6c299c86e4099ab38c8418300
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
| users.json | strata | 23.130 | 26.345 | 32.288 | 57.223 | 1.00x |
| users.json | orjson | 34.677 | 40.934 | 79.546 | 57.223 | 0.64x |
| users.json | msgspec | 35.806 | 42.135 | 49.593 | 57.223 | 0.63x |
| users.json | ujson | 50.977 | 60.711 | 74.549 | 57.223 | 0.43x |
| users.json | pysimdjson | 218.204 | 257.675 | 350.155 | 57.223 | 0.10x |
| users.json | json | 54.455 | 63.660 | 121.679 | 57.223 | 0.41x |
| flat.json | strata | 1.653 | 1.767 | 2.283 | 66.164 | 1.00x |
| flat.json | orjson | 1.819 | 1.997 | 2.792 | 66.164 | 0.88x |
| flat.json | msgspec | 2.077 | 2.782 | 14.099 | 66.164 | 0.64x |
| flat.json | ujson | 3.608 | 4.587 | 15.582 | 66.164 | 0.39x |
| flat.json | pysimdjson | 19.517 | 23.535 | 51.368 | 66.164 | 0.08x |
| flat.json | json | 4.158 | 4.687 | 7.182 | 66.164 | 0.38x |
| nested.json | strata | 2.087 | 2.719 | 3.122 | 64.637 | 1.00x |
| nested.json | orjson | 2.257 | 2.860 | 7.082 | 64.637 | 0.95x |
| nested.json | msgspec | 2.465 | 3.083 | 10.219 | 64.637 | 0.88x |
| nested.json | ujson | 4.418 | 5.034 | 6.882 | 64.637 | 0.54x |
| nested.json | pysimdjson | 19.454 | 21.407 | 24.103 | 64.637 | 0.13x |
| nested.json | json | 5.508 | 6.618 | 8.610 | 64.637 | 0.41x |
| wide_arrays.json | strata | 10.086 | 13.347 | 18.700 | 70.602 | 1.00x |
| wide_arrays.json | orjson | 12.505 | 16.560 | 20.906 | 70.602 | 0.81x |
| wide_arrays.json | msgspec | 13.961 | 19.114 | 23.691 | 70.602 | 0.70x |
| wide_arrays.json | ujson | 17.335 | 22.638 | 85.428 | 70.602 | 0.59x |
| wide_arrays.json | pysimdjson | 110.705 | 139.075 | 207.642 | 70.602 | 0.10x |
| wide_arrays.json | json | 29.788 | 31.954 | 43.423 | 70.602 | 0.42x |
| mixed.json | strata | 0.504 | 0.785 | 1.075 | 63.398 | 1.00x |
| mixed.json | orjson | 0.631 | 0.876 | 1.346 | 63.398 | 0.90x |
| mixed.json | msgspec | 0.647 | 1.000 | 1.277 | 63.398 | 0.78x |
| mixed.json | ujson | 0.923 | 1.465 | 1.991 | 63.398 | 0.54x |
| mixed.json | pysimdjson | 4.420 | 6.235 | 7.219 | 63.398 | 0.13x |
| mixed.json | json | 1.246 | 1.904 | 2.376 | 63.398 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.309 | 3.437 | 5.779 | 52.535 | 1.00x |
| users.json | orjson | 4.511 | 4.746 | 6.503 | 52.535 | 0.72x |
| users.json | msgspec | 6.864 | 7.108 | 8.358 | 52.535 | 0.48x |
| users.json | ujson | 32.298 | 33.115 | 40.487 | 52.535 | 0.10x |
| users.json | json | 54.917 | 55.722 | 66.931 | 52.535 | 0.06x |
| flat.json | strata | 0.438 | 0.680 | 0.728 | 64.676 | 1.00x |
| flat.json | orjson | 0.628 | 0.724 | 0.971 | 64.676 | 0.94x |
| flat.json | msgspec | 0.729 | 0.998 | 1.298 | 64.676 | 0.68x |
| flat.json | ujson | 2.989 | 4.026 | 5.395 | 64.676 | 0.17x |
| flat.json | json | 5.559 | 7.571 | 9.194 | 64.676 | 0.09x |
| nested.json | strata | 0.335 | 0.423 | 0.594 | 64.770 | 1.00x |
| nested.json | orjson | 0.472 | 0.568 | 0.842 | 64.770 | 0.75x |
| nested.json | msgspec | 0.761 | 1.129 | 3.768 | 64.770 | 0.37x |
| nested.json | ujson | 3.428 | 3.914 | 4.536 | 64.770 | 0.11x |
| nested.json | json | 6.189 | 7.247 | 9.364 | 64.770 | 0.06x |
| wide_arrays.json | strata | 2.574 | 3.709 | 7.900 | 64.328 | 1.00x |
| wide_arrays.json | orjson | 3.212 | 4.887 | 29.452 | 64.328 | 0.76x |
| wide_arrays.json | msgspec | 4.676 | 7.215 | 53.289 | 64.328 | 0.51x |
| wide_arrays.json | ujson | 14.836 | 20.903 | 74.710 | 64.328 | 0.18x |
| wide_arrays.json | json | 46.437 | 55.413 | 165.569 | 64.328 | 0.07x |
| mixed.json | strata | 0.094 | 0.170 | 0.200 | 60.199 | 1.00x |
| mixed.json | orjson | 0.123 | 0.182 | 0.218 | 60.199 | 0.93x |
| mixed.json | msgspec | 0.163 | 0.250 | 0.330 | 60.199 | 0.68x |
| mixed.json | ujson | 0.949 | 0.976 | 1.086 | 60.199 | 0.17x |
| mixed.json | json | 1.342 | 2.145 | 2.575 | 60.199 | 0.08x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 23.676 | 26.473 | 34.278 | 62.539 | 1.00x |
| users.json | orjson | 35.096 | 37.920 | 44.044 | 62.539 | 0.70x |
| users.json | msgspec | 35.157 | 38.820 | 45.290 | 62.539 | 0.68x |
| users.json | ujson | 50.615 | 56.795 | 66.284 | 62.539 | 0.47x |
| users.json | json | 56.413 | 60.929 | 69.139 | 62.539 | 0.43x |
| flat.json | strata | 1.819 | 2.251 | 2.971 | 64.676 | 1.00x |
| flat.json | orjson | 2.028 | 2.602 | 3.087 | 64.676 | 0.86x |
| flat.json | msgspec | 2.310 | 2.697 | 3.101 | 64.676 | 0.83x |
| flat.json | ujson | 3.832 | 4.408 | 5.915 | 64.676 | 0.51x |
| flat.json | json | 4.314 | 5.191 | 6.724 | 64.676 | 0.43x |
| nested.json | strata | 2.096 | 2.330 | 3.257 | 64.770 | 1.00x |
| nested.json | orjson | 2.408 | 2.638 | 3.554 | 64.770 | 0.88x |
| nested.json | msgspec | 2.704 | 3.054 | 3.876 | 64.770 | 0.76x |
| nested.json | ujson | 4.215 | 4.937 | 6.039 | 64.770 | 0.47x |
| nested.json | json | 5.323 | 6.132 | 7.770 | 64.770 | 0.38x |
| wide_arrays.json | strata | 9.521 | 13.252 | 17.307 | 65.559 | 1.00x |
| wide_arrays.json | orjson | 11.543 | 18.478 | 91.224 | 65.559 | 0.72x |
| wide_arrays.json | msgspec | 12.961 | 21.722 | 108.522 | 65.559 | 0.61x |
| wide_arrays.json | ujson | 22.459 | 28.357 | 60.744 | 65.559 | 0.47x |
| wide_arrays.json | json | 22.757 | 37.474 | 124.550 | 65.559 | 0.35x |
| mixed.json | strata | 0.598 | 0.910 | 1.078 | 60.199 | 1.00x |
| mixed.json | orjson | 0.839 | 1.208 | 1.328 | 60.199 | 0.75x |
| mixed.json | msgspec | 0.885 | 1.243 | 1.457 | 60.199 | 0.73x |
| mixed.json | ujson | 1.135 | 1.504 | 1.682 | 60.199 | 0.61x |
| mixed.json | json | 1.450 | 1.903 | 2.142 | 60.199 | 0.48x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 24.395 | 28.114 | 34.677 | 65.098 | 1.00x |
| users.ndjson | orjson | 37.777 | 44.117 | 116.076 | 65.098 | 0.64x |
| users.ndjson | msgspec | 38.729 | 44.620 | 138.512 | 65.098 | 0.63x |
| users.ndjson | ujson | 55.406 | 66.226 | 73.647 | 65.098 | 0.42x |
| users.ndjson | json | 70.165 | 80.504 | 120.792 | 65.098 | 0.35x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.924 | 6.219 | 9.474 | 62.578 | 1.00x |
| users.json | orjson | 6.785 | 8.156 | 9.761 | 62.578 | 0.76x |
| users.json | msgspec | 8.425 | 10.844 | 54.934 | 62.578 | 0.57x |
| users.json | ujson | 34.699 | 39.043 | 77.598 | 62.578 | 0.16x |
| users.json | json | 63.508 | 70.155 | 91.554 | 62.578 | 0.09x |
| flat.json | strata | 0.861 | 0.920 | 1.175 | 64.676 | 1.00x |
| flat.json | orjson | 1.026 | 1.265 | 1.454 | 64.676 | 0.73x |
| flat.json | msgspec | 1.206 | 1.321 | 1.607 | 64.676 | 0.70x |
| flat.json | ujson | 3.648 | 3.922 | 5.151 | 64.676 | 0.23x |
| flat.json | json | 5.541 | 5.861 | 7.711 | 64.676 | 0.16x |
| nested.json | strata | 0.754 | 0.796 | 1.349 | 64.770 | 1.00x |
| nested.json | orjson | 0.996 | 1.109 | 1.941 | 64.770 | 0.72x |
| nested.json | msgspec | 1.227 | 1.305 | 2.098 | 64.770 | 0.61x |
| nested.json | ujson | 3.703 | 4.162 | 5.893 | 64.770 | 0.19x |
| nested.json | json | 6.921 | 8.172 | 10.312 | 64.770 | 0.10x |
| wide_arrays.json | strata | 3.974 | 5.181 | 5.889 | 64.328 | 1.00x |
| wide_arrays.json | orjson | 4.593 | 6.137 | 19.943 | 64.328 | 0.84x |
| wide_arrays.json | msgspec | 6.968 | 8.094 | 22.008 | 64.328 | 0.64x |
| wide_arrays.json | ujson | 17.664 | 24.044 | 33.703 | 64.328 | 0.22x |
| wide_arrays.json | json | 49.263 | 69.110 | 128.432 | 64.328 | 0.07x |
| mixed.json | strata | 0.443 | 0.773 | 1.752 | 60.199 | 1.00x |
| mixed.json | orjson | 0.513 | 0.838 | 0.953 | 60.199 | 0.92x |
| mixed.json | msgspec | 0.593 | 0.750 | 1.003 | 60.199 | 1.03x |
| mixed.json | ujson | 1.030 | 1.667 | 2.010 | 60.199 | 0.46x |
| mixed.json | json | 1.750 | 2.630 | 22.844 | 60.199 | 0.29x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.208 | 0.226 | 0.307 | 62.641 | 1.00x |
| users.json $[*].id | jmespath | 1.274 | 1.316 | 1.507 | 62.641 | 0.17x |
| users.json $[*].id | jsonpath-ng | 6.946 | 7.384 | 9.389 | 62.641 | 0.03x |
| users.json $[*].orders[*].total | strata | 1.108 | 1.471 | 2.227 | 59.836 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 7.347 | 8.520 | 14.534 | 59.836 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 45.397 | 53.398 | 97.917 | 59.836 | 0.03x |
| users.json $..total | strata | 4.497 | 5.142 | 7.183 | 59.883 | 1.00x |
| users.json $..total | jsonpath-ng | 925.054 | 1039.703 | 1269.579 | 59.883 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 5.004 | 6.400 | 8.134 | 62.707 | 1.00x |
| users.json $[*].id | orjson+jmespath | 34.811 | 44.138 | 48.226 | 62.707 | 0.15x |
| users.json $[*].id | orjson+jsonpath-ng | 40.855 | 49.892 | 61.638 | 62.707 | 0.13x |
| users.json $[*].orders[*].total | strata | 5.457 | 7.103 | 9.599 | 59.871 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 43.241 | 49.286 | 53.010 | 59.871 | 0.14x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 98.398 | 109.545 | 122.610 | 59.871 | 0.06x |
| users.json $..total | strata | 28.854 | 32.658 | 39.687 | 59.883 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 1010.086 | 1090.790 | 1276.241 | 59.883 | 0.03x |

