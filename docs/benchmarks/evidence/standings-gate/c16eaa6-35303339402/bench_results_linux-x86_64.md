# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c16eaa65c61f1cf9d4fac46e9f6d8e6b9f3b087f
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: AMD EPYC 9V74 80-Core Processor
- compiler_flags: -fno-strict-overflow -Wsign-compare -DNDEBUG -g -O3 -Wall -fPIC -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -fomit-frame-pointer -march=native -flto=thin -fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata -fsplit-machine-functions -shared -Wl,--rpath=/opt/hostedtoolcache/Python/3.12.14/x64/lib (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.282 | 12.571 | 17.520 | 65.164 | 1.00x |
| users.json | orjson | 14.881 | 15.884 | 20.459 | 65.164 | 0.79x |
| users.json | msgspec | 15.015 | 16.082 | 20.211 | 65.164 | 0.78x |
| users.json | ujson | 21.844 | 22.861 | 27.769 | 65.164 | 0.55x |
| users.json | pysimdjson | 21.996 | 23.996 | 27.408 | 65.164 | 0.52x |
| users.json | json | 22.087 | 23.627 | 24.375 | 65.164 | 0.53x |
| flat.json | strata | 0.909 | 0.948 | 0.983 | 78.656 | 1.00x |
| flat.json | orjson | 1.085 | 1.105 | 1.130 | 78.656 | 0.86x |
| flat.json | msgspec | 1.085 | 1.103 | 1.133 | 78.656 | 0.86x |
| flat.json | ujson | 1.738 | 1.758 | 1.816 | 78.656 | 0.54x |
| flat.json | pysimdjson | 1.717 | 1.765 | 1.837 | 78.656 | 0.54x |
| flat.json | json | 1.742 | 1.771 | 1.788 | 78.656 | 0.54x |
| nested.json | strata | 0.807 | 0.820 | 0.874 | 78.688 | 1.00x |
| nested.json | orjson | 1.009 | 1.022 | 1.037 | 78.688 | 0.80x |
| nested.json | msgspec | 1.028 | 1.045 | 1.095 | 78.688 | 0.78x |
| nested.json | ujson | 1.489 | 1.562 | 1.615 | 78.688 | 0.52x |
| nested.json | pysimdjson | 1.410 | 1.431 | 1.484 | 78.688 | 0.57x |
| nested.json | json | 1.845 | 1.866 | 1.986 | 78.688 | 0.44x |
| wide_arrays.json | strata | 4.500 | 4.726 | 5.107 | 84.191 | 1.00x |
| wide_arrays.json | orjson | 5.688 | 6.003 | 6.484 | 84.191 | 0.79x |
| wide_arrays.json | msgspec | 6.164 | 6.319 | 6.710 | 84.191 | 0.75x |
| wide_arrays.json | ujson | 7.787 | 7.953 | 8.542 | 84.191 | 0.59x |
| wide_arrays.json | pysimdjson | 6.601 | 6.843 | 7.588 | 84.191 | 0.69x |
| wide_arrays.json | json | 9.942 | 10.230 | 11.258 | 84.191 | 0.46x |
| mixed.json | strata | 0.199 | 0.203 | 0.218 | 84.191 | 1.00x |
| mixed.json | orjson | 0.243 | 0.249 | 0.284 | 84.191 | 0.81x |
| mixed.json | msgspec | 0.252 | 0.266 | 0.279 | 84.191 | 0.76x |
| mixed.json | ujson | 0.334 | 0.353 | 0.378 | 84.191 | 0.57x |
| mixed.json | pysimdjson | 0.315 | 0.330 | 0.352 | 84.191 | 0.62x |
| mixed.json | json | 0.465 | 0.480 | 0.501 | 84.191 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.371 | 2.433 | 2.555 | 64.293 | 1.00x |
| users.json | orjson | 3.095 | 3.166 | 3.293 | 64.293 | 0.77x |
| users.json | msgspec | 4.230 | 4.281 | 4.473 | 64.293 | 0.57x |
| users.json | ujson | 11.555 | 11.805 | 12.220 | 64.293 | 0.21x |
| users.json | json | 21.563 | 22.025 | 22.465 | 64.293 | 0.11x |
| flat.json | strata | 0.329 | 0.349 | 0.388 | 78.688 | 1.00x |
| flat.json | orjson | 0.370 | 0.394 | 0.424 | 78.688 | 0.89x |
| flat.json | msgspec | 0.488 | 0.511 | 0.527 | 78.688 | 0.68x |
| flat.json | ujson | 1.040 | 1.069 | 1.095 | 78.688 | 0.33x |
| flat.json | json | 1.875 | 1.918 | 2.006 | 78.688 | 0.18x |
| nested.json | strata | 0.229 | 0.237 | 0.255 | 78.688 | 1.00x |
| nested.json | orjson | 0.301 | 0.308 | 0.315 | 78.688 | 0.77x |
| nested.json | msgspec | 0.419 | 0.440 | 0.444 | 78.688 | 0.54x |
| nested.json | ujson | 1.080 | 1.094 | 1.115 | 78.688 | 0.22x |
| nested.json | json | 2.398 | 2.420 | 2.447 | 78.688 | 0.10x |
| wide_arrays.json | strata | 1.778 | 1.807 | 1.874 | 84.191 | 1.00x |
| wide_arrays.json | orjson | 1.940 | 1.954 | 1.981 | 84.191 | 0.92x |
| wide_arrays.json | msgspec | 3.090 | 3.116 | 3.148 | 84.191 | 0.58x |
| wide_arrays.json | ujson | 6.386 | 6.453 | 6.863 | 84.191 | 0.28x |
| wide_arrays.json | json | 16.778 | 16.991 | 17.346 | 84.191 | 0.11x |
| mixed.json | strata | 0.062 | 0.067 | 0.068 | 84.191 | 1.00x |
| mixed.json | orjson | 0.070 | 0.072 | 0.084 | 84.191 | 0.93x |
| mixed.json | msgspec | 0.092 | 0.095 | 0.115 | 84.191 | 0.71x |
| mixed.json | ujson | 0.232 | 0.235 | 0.242 | 84.191 | 0.28x |
| mixed.json | json | 0.514 | 0.541 | 0.555 | 84.191 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.494 | 12.372 | 13.967 | 80.293 | 1.00x |
| users.json | orjson | 14.700 | 15.415 | 16.724 | 80.293 | 0.80x |
| users.json | msgspec | 14.911 | 15.455 | 16.017 | 80.293 | 0.80x |
| users.json | ujson | 21.581 | 22.071 | 24.791 | 80.293 | 0.56x |
| users.json | json | 22.536 | 22.810 | 23.557 | 80.293 | 0.54x |
| flat.json | strata | 0.962 | 0.987 | 1.068 | 78.688 | 1.00x |
| flat.json | orjson | 1.163 | 1.186 | 1.214 | 78.688 | 0.83x |
| flat.json | msgspec | 1.147 | 1.181 | 1.211 | 78.688 | 0.84x |
| flat.json | ujson | 1.767 | 1.821 | 1.878 | 78.688 | 0.54x |
| flat.json | json | 1.820 | 1.849 | 1.881 | 78.688 | 0.53x |
| nested.json | strata | 0.851 | 0.889 | 0.923 | 78.688 | 1.00x |
| nested.json | orjson | 1.073 | 1.101 | 1.719 | 78.688 | 0.81x |
| nested.json | msgspec | 1.089 | 1.110 | 1.335 | 78.688 | 0.80x |
| nested.json | ujson | 1.577 | 1.622 | 1.685 | 78.688 | 0.55x |
| nested.json | json | 1.900 | 1.943 | 2.027 | 78.688 | 0.46x |
| wide_arrays.json | strata | 4.632 | 4.883 | 4.966 | 84.191 | 1.00x |
| wide_arrays.json | orjson | 5.914 | 6.177 | 6.677 | 84.191 | 0.79x |
| wide_arrays.json | msgspec | 6.389 | 6.638 | 6.877 | 84.191 | 0.74x |
| wide_arrays.json | ujson | 8.062 | 8.378 | 8.557 | 84.191 | 0.58x |
| wide_arrays.json | json | 10.024 | 10.269 | 10.494 | 84.191 | 0.48x |
| mixed.json | strata | 0.209 | 0.228 | 0.242 | 84.191 | 1.00x |
| mixed.json | orjson | 0.292 | 0.314 | 0.354 | 84.191 | 0.73x |
| mixed.json | msgspec | 0.301 | 0.313 | 0.339 | 84.191 | 0.73x |
| mixed.json | ujson | 0.397 | 0.419 | 0.426 | 84.191 | 0.54x |
| mixed.json | json | 0.507 | 0.520 | 0.541 | 84.191 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 12.748 | 13.868 | 14.768 | 78.656 | 1.00x |
| users.ndjson | orjson | 19.501 | 20.834 | 21.823 | 78.656 | 0.67x |
| users.ndjson | msgspec | 19.914 | 21.009 | 22.371 | 78.656 | 0.66x |
| users.ndjson | ujson | 26.123 | 27.617 | 28.564 | 78.656 | 0.50x |
| users.ndjson | json | 32.028 | 32.943 | 33.843 | 78.656 | 0.42x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.924 | 3.006 | 3.081 | 80.293 | 1.00x |
| users.json | orjson | 3.833 | 3.874 | 3.992 | 80.293 | 0.78x |
| users.json | msgspec | 4.829 | 4.973 | 5.110 | 80.293 | 0.60x |
| users.json | ujson | 12.256 | 12.471 | 12.753 | 80.293 | 0.24x |
| users.json | json | 22.257 | 22.553 | 22.892 | 80.293 | 0.13x |
| flat.json | strata | 0.489 | 0.510 | 0.553 | 78.688 | 1.00x |
| flat.json | orjson | 0.548 | 0.574 | 0.603 | 78.688 | 0.89x |
| flat.json | msgspec | 0.675 | 0.693 | 0.721 | 78.688 | 0.74x |
| flat.json | ujson | 1.242 | 1.269 | 1.376 | 78.688 | 0.40x |
| flat.json | json | 2.088 | 2.123 | 2.286 | 78.688 | 0.24x |
| nested.json | strata | 0.352 | 0.372 | 0.413 | 78.688 | 1.00x |
| nested.json | orjson | 0.447 | 0.463 | 0.515 | 78.688 | 0.80x |
| nested.json | msgspec | 0.578 | 0.592 | 0.612 | 78.688 | 0.63x |
| nested.json | ujson | 1.240 | 1.256 | 1.296 | 78.688 | 0.30x |
| nested.json | json | 2.542 | 2.586 | 2.659 | 78.688 | 0.14x |
| wide_arrays.json | strata | 2.237 | 2.306 | 2.502 | 84.191 | 1.00x |
| wide_arrays.json | orjson | 2.435 | 2.512 | 2.603 | 84.191 | 0.92x |
| wide_arrays.json | msgspec | 3.574 | 3.649 | 3.841 | 84.191 | 0.63x |
| wide_arrays.json | ujson | 7.036 | 7.177 | 7.590 | 84.191 | 0.32x |
| wide_arrays.json | json | 17.802 | 18.042 | 18.794 | 84.191 | 0.13x |
| mixed.json | strata | 0.158 | 0.166 | 0.180 | 84.191 | 1.00x |
| mixed.json | orjson | 0.182 | 0.194 | 0.214 | 84.191 | 0.86x |
| mixed.json | msgspec | 0.199 | 0.207 | 0.252 | 84.191 | 0.80x |
| mixed.json | ujson | 0.362 | 0.377 | 0.398 | 84.191 | 0.44x |
| mixed.json | json | 0.646 | 0.673 | 0.687 | 84.191 | 0.25x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.080 | 0.085 | 0.087 | 80.293 | 1.00x |
| users.json $[*].id | jmespath | 0.474 | 0.488 | 0.508 | 80.293 | 0.17x |
| users.json $[*].id | jsonpath-ng | 2.886 | 3.086 | 3.176 | 80.293 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.447 | 0.459 | 0.492 | 80.293 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.026 | 3.065 | 3.157 | 80.293 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 20.734 | 21.505 | 22.480 | 80.293 | 0.02x |
| users.json $..total | strata | 1.861 | 1.880 | 1.948 | 80.293 | 1.00x |
| users.json $..total | jsonpath-ng | 385.774 | 388.981 | 391.538 | 80.293 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.265 | 3.277 | 3.297 | 80.293 | 1.00x |
| users.json $[*].id | orjson+jmespath | 16.543 | 17.601 | 18.156 | 80.293 | 0.19x |
| users.json $[*].id | orjson+jsonpath-ng | 19.035 | 20.244 | 21.426 | 80.293 | 0.16x |
| users.json $[*].orders[*].total | strata | 3.545 | 3.587 | 3.614 | 80.293 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 20.247 | 20.750 | 23.444 | 80.293 | 0.17x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 42.068 | 45.102 | 47.018 | 80.293 | 0.08x |
| users.json $..total | strata | 15.680 | 17.910 | 19.484 | 80.293 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 413.796 | 417.517 | 420.466 | 80.293 | 0.04x |

