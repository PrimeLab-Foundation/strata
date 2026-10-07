# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 85eb3583e98587844415f5d32f85a61cbedfcf49
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
| users.json | strata | 19.530 | 21.295 | 25.130 | 57.043 | 1.00x |
| users.json | orjson | 28.955 | 30.749 | 37.281 | 57.043 | 0.69x |
| users.json | msgspec | 29.041 | 30.701 | 34.372 | 57.043 | 0.69x |
| users.json | ujson | 41.626 | 47.357 | 50.191 | 57.043 | 0.45x |
| users.json | pysimdjson | 178.401 | 186.031 | 193.797 | 57.043 | 0.11x |
| users.json | json | 46.550 | 50.677 | 57.260 | 57.043 | 0.42x |
| flat.json | strata | 1.428 | 1.582 | 2.257 | 66.160 | 1.00x |
| flat.json | orjson | 1.570 | 1.845 | 2.136 | 66.160 | 0.86x |
| flat.json | msgspec | 1.799 | 1.922 | 2.874 | 66.160 | 0.82x |
| flat.json | ujson | 3.143 | 3.658 | 4.622 | 66.160 | 0.43x |
| flat.json | pysimdjson | 16.653 | 17.995 | 20.207 | 66.160 | 0.09x |
| flat.json | json | 3.614 | 4.138 | 5.860 | 66.160 | 0.38x |
| nested.json | strata | 1.700 | 1.770 | 2.564 | 65.305 | 1.00x |
| nested.json | orjson | 1.938 | 2.043 | 2.502 | 65.305 | 0.87x |
| nested.json | msgspec | 2.119 | 2.254 | 3.002 | 65.305 | 0.79x |
| nested.json | ujson | 3.524 | 3.571 | 4.646 | 65.305 | 0.50x |
| nested.json | pysimdjson | 15.601 | 17.199 | 19.229 | 65.305 | 0.10x |
| nested.json | json | 4.457 | 4.652 | 6.222 | 65.305 | 0.38x |
| wide_arrays.json | strata | 8.786 | 9.423 | 10.864 | 71.266 | 1.00x |
| wide_arrays.json | orjson | 11.093 | 11.543 | 14.478 | 71.266 | 0.82x |
| wide_arrays.json | msgspec | 11.980 | 13.201 | 15.634 | 71.266 | 0.71x |
| wide_arrays.json | ujson | 14.879 | 15.599 | 16.995 | 71.266 | 0.60x |
| wide_arrays.json | pysimdjson | 92.164 | 100.339 | 105.377 | 71.266 | 0.09x |
| wide_arrays.json | json | 19.584 | 20.024 | 25.125 | 71.266 | 0.47x |
| mixed.json | strata | 0.413 | 0.525 | 0.688 | 64.062 | 1.00x |
| mixed.json | orjson | 0.503 | 0.546 | 0.785 | 64.062 | 0.96x |
| mixed.json | msgspec | 0.538 | 0.705 | 0.930 | 64.062 | 0.75x |
| mixed.json | ujson | 0.734 | 0.807 | 1.117 | 64.062 | 0.65x |
| mixed.json | pysimdjson | 3.808 | 4.284 | 5.094 | 64.062 | 0.12x |
| mixed.json | json | 1.042 | 1.080 | 1.589 | 64.062 | 0.49x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.722 | 2.825 | 3.301 | 52.328 | 1.00x |
| users.json | orjson | 3.635 | 3.839 | 4.697 | 52.328 | 0.74x |
| users.json | msgspec | 5.529 | 5.816 | 7.624 | 52.328 | 0.49x |
| users.json | ujson | 26.358 | 27.291 | 29.466 | 52.328 | 0.10x |
| users.json | json | 44.693 | 45.329 | 46.953 | 52.328 | 0.06x |
| flat.json | strata | 0.380 | 0.387 | 0.536 | 64.691 | 1.00x |
| flat.json | orjson | 0.467 | 0.474 | 0.479 | 64.691 | 0.82x |
| flat.json | msgspec | 0.626 | 0.639 | 0.660 | 64.691 | 0.61x |
| flat.json | ujson | 2.512 | 2.533 | 3.912 | 64.691 | 0.15x |
| flat.json | json | 4.150 | 4.201 | 4.875 | 64.691 | 0.09x |
| nested.json | strata | 0.280 | 0.290 | 0.318 | 65.434 | 1.00x |
| nested.json | orjson | 0.411 | 0.429 | 0.447 | 65.434 | 0.68x |
| nested.json | msgspec | 0.644 | 0.654 | 0.686 | 65.434 | 0.44x |
| nested.json | ujson | 2.660 | 2.693 | 2.942 | 65.434 | 0.11x |
| nested.json | json | 5.270 | 5.305 | 5.389 | 65.434 | 0.05x |
| wide_arrays.json | strata | 2.305 | 2.492 | 3.512 | 64.992 | 1.00x |
| wide_arrays.json | orjson | 2.960 | 3.900 | 4.756 | 64.992 | 0.64x |
| wide_arrays.json | msgspec | 4.311 | 5.122 | 5.979 | 64.992 | 0.49x |
| wide_arrays.json | ujson | 12.642 | 15.337 | 18.651 | 64.992 | 0.16x |
| wide_arrays.json | json | 38.916 | 48.671 | 51.353 | 64.992 | 0.05x |
| mixed.json | strata | 0.081 | 0.120 | 0.155 | 60.863 | 1.00x |
| mixed.json | orjson | 0.101 | 0.141 | 0.185 | 60.863 | 0.85x |
| mixed.json | msgspec | 0.138 | 0.190 | 0.295 | 60.863 | 0.63x |
| mixed.json | ujson | 0.543 | 0.661 | 0.806 | 60.863 | 0.18x |
| mixed.json | json | 1.108 | 1.345 | 1.683 | 60.863 | 0.09x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 18.811 | 19.743 | 22.752 | 63.133 | 1.00x |
| users.json | orjson | 26.333 | 27.823 | 37.097 | 63.133 | 0.71x |
| users.json | msgspec | 26.438 | 28.416 | 30.584 | 63.133 | 0.69x |
| users.json | ujson | 38.802 | 42.168 | 51.496 | 63.133 | 0.47x |
| users.json | json | 43.433 | 46.888 | 53.978 | 63.133 | 0.42x |
| flat.json | strata | 1.550 | 1.609 | 2.543 | 65.344 | 1.00x |
| flat.json | orjson | 1.770 | 2.025 | 2.745 | 65.344 | 0.79x |
| flat.json | msgspec | 2.005 | 2.119 | 3.092 | 65.344 | 0.76x |
| flat.json | ujson | 3.372 | 3.829 | 5.120 | 65.344 | 0.42x |
| flat.json | json | 3.801 | 3.922 | 5.346 | 65.344 | 0.41x |
| nested.json | strata | 1.826 | 1.844 | 2.720 | 65.434 | 1.00x |
| nested.json | orjson | 2.090 | 2.132 | 2.225 | 65.434 | 0.86x |
| nested.json | msgspec | 2.315 | 2.353 | 2.512 | 65.434 | 0.78x |
| nested.json | ujson | 3.676 | 3.722 | 5.764 | 65.434 | 0.50x |
| nested.json | json | 4.645 | 4.681 | 4.759 | 65.434 | 0.39x |
| wide_arrays.json | strata | 8.722 | 9.993 | 13.580 | 66.223 | 1.00x |
| wide_arrays.json | orjson | 10.900 | 11.929 | 15.738 | 66.223 | 0.84x |
| wide_arrays.json | msgspec | 12.015 | 13.402 | 16.342 | 66.223 | 0.75x |
| wide_arrays.json | ujson | 15.180 | 16.002 | 19.642 | 66.223 | 0.62x |
| wide_arrays.json | json | 19.669 | 22.809 | 25.914 | 66.223 | 0.44x |
| mixed.json | strata | 0.508 | 0.546 | 0.798 | 60.863 | 1.00x |
| mixed.json | orjson | 0.654 | 0.675 | 0.818 | 60.863 | 0.81x |
| mixed.json | msgspec | 0.704 | 0.742 | 1.045 | 60.863 | 0.74x |
| mixed.json | ujson | 0.923 | 1.083 | 1.318 | 60.863 | 0.50x |
| mixed.json | json | 1.169 | 1.212 | 1.730 | 60.863 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 20.667 | 22.865 | 26.406 | 65.734 | 1.00x |
| users.ndjson | orjson | 30.700 | 32.623 | 40.205 | 65.734 | 0.70x |
| users.ndjson | msgspec | 31.828 | 35.889 | 41.073 | 65.734 | 0.64x |
| users.ndjson | ujson | 44.310 | 49.880 | 58.972 | 65.734 | 0.46x |
| users.ndjson | json | 55.725 | 58.519 | 68.164 | 65.734 | 0.39x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.202 | 3.497 | 6.064 | 63.148 | 1.00x |
| users.json | orjson | 4.333 | 4.585 | 5.204 | 63.148 | 0.76x |
| users.json | msgspec | 6.044 | 6.394 | 8.108 | 63.148 | 0.55x |
| users.json | ujson | 26.399 | 27.699 | 36.687 | 63.148 | 0.13x |
| users.json | json | 44.083 | 46.692 | 54.422 | 63.148 | 0.07x |
| flat.json | strata | 0.751 | 0.814 | 1.055 | 65.344 | 1.00x |
| flat.json | orjson | 0.898 | 1.084 | 1.290 | 65.344 | 0.75x |
| flat.json | msgspec | 1.052 | 1.124 | 1.432 | 65.344 | 0.72x |
| flat.json | ujson | 2.999 | 3.144 | 3.974 | 65.344 | 0.26x |
| flat.json | json | 4.729 | 5.463 | 6.493 | 65.344 | 0.15x |
| nested.json | strata | 0.640 | 0.664 | 1.000 | 65.434 | 1.00x |
| nested.json | orjson | 0.835 | 0.890 | 1.026 | 65.434 | 0.75x |
| nested.json | msgspec | 1.057 | 1.084 | 1.209 | 65.434 | 0.61x |
| nested.json | ujson | 3.127 | 3.240 | 4.382 | 65.434 | 0.20x |
| nested.json | json | 5.865 | 6.393 | 7.415 | 65.434 | 0.10x |
| wide_arrays.json | strata | 3.176 | 3.386 | 3.975 | 64.992 | 1.00x |
| wide_arrays.json | orjson | 3.827 | 4.136 | 4.883 | 64.992 | 0.82x |
| wide_arrays.json | msgspec | 4.994 | 5.505 | 6.402 | 64.992 | 0.62x |
| wide_arrays.json | ujson | 13.805 | 15.489 | 17.316 | 64.992 | 0.22x |
| wide_arrays.json | json | 40.393 | 45.529 | 50.464 | 64.992 | 0.07x |
| mixed.json | strata | 0.409 | 0.584 | 0.622 | 60.863 | 1.00x |
| mixed.json | orjson | 0.488 | 0.542 | 0.647 | 60.863 | 1.08x |
| mixed.json | msgspec | 0.513 | 0.556 | 0.864 | 60.863 | 1.05x |
| mixed.json | ujson | 0.920 | 1.007 | 1.483 | 60.863 | 0.58x |
| mixed.json | json | 1.483 | 1.657 | 2.443 | 60.863 | 0.35x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.134 | 0.147 | 0.176 | 63.215 | 1.00x |
| users.json $[*].id | jmespath | 0.962 | 1.007 | 1.129 | 63.215 | 0.15x |
| users.json $[*].id | jsonpath-ng | 5.235 | 5.341 | 5.501 | 63.215 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.897 | 1.072 | 1.330 | 60.387 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 6.247 | 6.564 | 7.288 | 60.387 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 36.222 | 37.669 | 43.083 | 60.387 | 0.03x |
| users.json $..total | strata | 3.489 | 3.712 | 4.222 | 60.480 | 1.00x |
| users.json $..total | jsonpath-ng | 761.812 | 782.819 | 847.614 | 60.480 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.878 | 3.924 | 4.018 | 63.266 | 1.00x |
| users.json $[*].id | orjson+jmespath | 26.902 | 28.241 | 32.894 | 63.266 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 32.085 | 32.950 | 36.063 | 63.266 | 0.12x |
| users.json $[*].orders[*].total | strata | 4.655 | 4.752 | 5.258 | 60.406 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 35.628 | 37.113 | 39.771 | 60.406 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 73.450 | 76.374 | 86.671 | 60.406 | 0.06x |
| users.json $..total | strata | 25.120 | 25.772 | 29.277 | 60.543 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 837.772 | 887.878 | 1016.903 | 60.543 | 0.03x |

