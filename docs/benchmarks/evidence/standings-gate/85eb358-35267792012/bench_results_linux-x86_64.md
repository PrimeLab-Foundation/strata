# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 85eb3583e98587844415f5d32f85a61cbedfcf49
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
| users.json | strata | 9.753 | 10.314 | 16.802 | 66.129 | 1.00x |
| users.json | orjson | 13.822 | 14.198 | 19.039 | 66.129 | 0.73x |
| users.json | msgspec | 13.799 | 14.061 | 19.302 | 66.129 | 0.73x |
| users.json | ujson | 18.060 | 18.965 | 24.998 | 66.129 | 0.54x |
| users.json | pysimdjson | 18.599 | 19.785 | 23.535 | 66.129 | 0.52x |
| users.json | json | 20.936 | 21.250 | 22.914 | 66.129 | 0.49x |
| flat.json | strata | 0.896 | 0.923 | 0.974 | 79.398 | 1.00x |
| flat.json | orjson | 1.096 | 1.109 | 1.120 | 79.398 | 0.83x |
| flat.json | msgspec | 1.078 | 1.109 | 1.133 | 79.398 | 0.83x |
| flat.json | ujson | 1.600 | 1.713 | 1.788 | 79.398 | 0.54x |
| flat.json | pysimdjson | 1.676 | 1.716 | 1.767 | 79.398 | 0.54x |
| flat.json | json | 1.740 | 1.760 | 1.796 | 79.398 | 0.52x |
| nested.json | strata | 0.803 | 0.819 | 0.845 | 79.434 | 1.00x |
| nested.json | orjson | 0.997 | 1.019 | 1.071 | 79.434 | 0.80x |
| nested.json | msgspec | 0.973 | 0.994 | 1.029 | 79.434 | 0.82x |
| nested.json | ujson | 1.479 | 1.515 | 1.610 | 79.434 | 0.54x |
| nested.json | pysimdjson | 1.398 | 1.420 | 1.517 | 79.434 | 0.58x |
| nested.json | json | 1.830 | 1.854 | 1.877 | 79.434 | 0.44x |
| wide_arrays.json | strata | 4.496 | 4.770 | 5.524 | 84.992 | 1.00x |
| wide_arrays.json | orjson | 5.980 | 6.733 | 7.102 | 84.992 | 0.71x |
| wide_arrays.json | msgspec | 6.308 | 6.907 | 7.388 | 84.992 | 0.69x |
| wide_arrays.json | ujson | 7.827 | 8.352 | 8.894 | 84.992 | 0.57x |
| wide_arrays.json | pysimdjson | 6.703 | 7.361 | 7.932 | 84.992 | 0.65x |
| wide_arrays.json | json | 10.221 | 10.627 | 11.523 | 84.992 | 0.45x |
| mixed.json | strata | 0.190 | 0.193 | 0.199 | 84.992 | 1.00x |
| mixed.json | orjson | 0.236 | 0.239 | 0.255 | 84.992 | 0.81x |
| mixed.json | msgspec | 0.243 | 0.250 | 0.269 | 84.992 | 0.77x |
| mixed.json | ujson | 0.303 | 0.315 | 0.319 | 84.992 | 0.61x |
| mixed.json | pysimdjson | 0.299 | 0.307 | 0.317 | 84.992 | 0.63x |
| mixed.json | json | 0.445 | 0.454 | 0.469 | 84.992 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.309 | 2.366 | 2.391 | 64.516 | 1.00x |
| users.json | orjson | 3.045 | 3.066 | 3.131 | 64.516 | 0.77x |
| users.json | msgspec | 4.123 | 4.147 | 4.267 | 64.516 | 0.57x |
| users.json | ujson | 11.393 | 11.494 | 11.792 | 64.516 | 0.21x |
| users.json | json | 21.591 | 21.759 | 22.293 | 64.516 | 0.11x |
| flat.json | strata | 0.312 | 0.318 | 0.371 | 79.434 | 1.00x |
| flat.json | orjson | 0.361 | 0.373 | 0.421 | 79.434 | 0.85x |
| flat.json | msgspec | 0.475 | 0.479 | 0.517 | 79.434 | 0.66x |
| flat.json | ujson | 1.044 | 1.053 | 1.067 | 79.434 | 0.30x |
| flat.json | json | 1.880 | 1.892 | 1.928 | 79.434 | 0.17x |
| nested.json | strata | 0.230 | 0.239 | 0.311 | 79.434 | 1.00x |
| nested.json | orjson | 0.298 | 0.302 | 0.318 | 79.434 | 0.79x |
| nested.json | msgspec | 0.419 | 0.428 | 0.432 | 79.434 | 0.56x |
| nested.json | ujson | 1.076 | 1.087 | 1.110 | 79.434 | 0.22x |
| nested.json | json | 2.357 | 2.370 | 2.406 | 79.434 | 0.10x |
| wide_arrays.json | strata | 1.826 | 1.883 | 2.140 | 84.992 | 1.00x |
| wide_arrays.json | orjson | 1.961 | 1.994 | 2.060 | 84.992 | 0.94x |
| wide_arrays.json | msgspec | 3.075 | 3.157 | 3.240 | 84.992 | 0.60x |
| wide_arrays.json | ujson | 6.396 | 6.553 | 6.733 | 84.992 | 0.29x |
| wide_arrays.json | json | 16.991 | 17.330 | 17.815 | 84.992 | 0.11x |
| mixed.json | strata | 0.061 | 0.064 | 0.073 | 84.992 | 1.00x |
| mixed.json | orjson | 0.067 | 0.069 | 0.082 | 84.992 | 0.93x |
| mixed.json | msgspec | 0.085 | 0.088 | 0.104 | 84.992 | 0.72x |
| mixed.json | ujson | 0.222 | 0.227 | 0.238 | 84.992 | 0.28x |
| mixed.json | json | 0.506 | 0.513 | 0.535 | 84.992 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.002 | 11.420 | 12.933 | 81.035 | 1.00x |
| users.json | orjson | 14.577 | 15.244 | 16.340 | 81.035 | 0.75x |
| users.json | msgspec | 14.490 | 14.982 | 15.338 | 81.035 | 0.76x |
| users.json | ujson | 19.844 | 21.032 | 22.422 | 81.035 | 0.54x |
| users.json | json | 21.988 | 22.442 | 23.029 | 81.035 | 0.51x |
| flat.json | strata | 0.939 | 0.959 | 0.987 | 79.434 | 1.00x |
| flat.json | orjson | 1.156 | 1.168 | 1.188 | 79.434 | 0.82x |
| flat.json | msgspec | 1.138 | 1.155 | 1.172 | 79.434 | 0.83x |
| flat.json | ujson | 1.725 | 1.757 | 1.817 | 79.434 | 0.55x |
| flat.json | json | 1.787 | 1.819 | 1.865 | 79.434 | 0.53x |
| nested.json | strata | 0.851 | 0.865 | 0.955 | 79.434 | 1.00x |
| nested.json | orjson | 1.088 | 1.132 | 1.174 | 79.434 | 0.76x |
| nested.json | msgspec | 1.026 | 1.079 | 1.111 | 79.434 | 0.80x |
| nested.json | ujson | 1.557 | 1.610 | 1.723 | 79.434 | 0.54x |
| nested.json | json | 1.904 | 1.931 | 2.002 | 79.434 | 0.45x |
| wide_arrays.json | strata | 4.532 | 4.665 | 4.884 | 84.992 | 1.00x |
| wide_arrays.json | orjson | 5.721 | 5.917 | 6.193 | 84.992 | 0.79x |
| wide_arrays.json | msgspec | 6.339 | 6.511 | 6.805 | 84.992 | 0.72x |
| wide_arrays.json | ujson | 7.912 | 8.267 | 8.424 | 84.992 | 0.56x |
| wide_arrays.json | json | 10.051 | 10.351 | 10.535 | 84.992 | 0.45x |
| mixed.json | strata | 0.214 | 0.224 | 0.252 | 84.992 | 1.00x |
| mixed.json | orjson | 0.293 | 0.304 | 0.342 | 84.992 | 0.74x |
| mixed.json | msgspec | 0.290 | 0.303 | 0.331 | 84.992 | 0.74x |
| mixed.json | ujson | 0.367 | 0.395 | 0.417 | 84.992 | 0.57x |
| mixed.json | json | 0.499 | 0.517 | 0.557 | 84.992 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 12.784 | 13.156 | 13.694 | 79.398 | 1.00x |
| users.ndjson | orjson | 19.621 | 19.928 | 20.965 | 79.398 | 0.66x |
| users.ndjson | msgspec | 19.756 | 20.241 | 21.412 | 79.398 | 0.65x |
| users.ndjson | ujson | 25.812 | 26.000 | 27.061 | 79.398 | 0.51x |
| users.ndjson | json | 31.415 | 31.732 | 33.447 | 79.398 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.882 | 2.973 | 3.139 | 81.035 | 1.00x |
| users.json | orjson | 3.744 | 3.844 | 4.375 | 81.035 | 0.77x |
| users.json | msgspec | 4.703 | 4.863 | 5.034 | 81.035 | 0.61x |
| users.json | ujson | 12.193 | 12.362 | 12.946 | 81.035 | 0.24x |
| users.json | json | 22.528 | 22.741 | 23.593 | 81.035 | 0.13x |
| flat.json | strata | 0.471 | 0.491 | 0.516 | 79.434 | 1.00x |
| flat.json | orjson | 0.545 | 0.557 | 0.585 | 79.434 | 0.88x |
| flat.json | msgspec | 0.658 | 0.673 | 0.699 | 79.434 | 0.73x |
| flat.json | ujson | 1.229 | 1.252 | 1.299 | 79.434 | 0.39x |
| flat.json | json | 2.086 | 2.121 | 2.162 | 79.434 | 0.23x |
| nested.json | strata | 0.357 | 0.374 | 0.409 | 79.434 | 1.00x |
| nested.json | orjson | 0.469 | 0.482 | 0.493 | 79.434 | 0.78x |
| nested.json | msgspec | 0.580 | 0.606 | 0.650 | 79.434 | 0.62x |
| nested.json | ujson | 1.235 | 1.254 | 1.271 | 79.434 | 0.30x |
| nested.json | json | 2.531 | 2.573 | 2.653 | 79.434 | 0.15x |
| wide_arrays.json | strata | 2.153 | 2.185 | 2.556 | 84.992 | 1.00x |
| wide_arrays.json | orjson | 2.364 | 2.393 | 2.553 | 84.992 | 0.91x |
| wide_arrays.json | msgspec | 3.459 | 3.518 | 3.554 | 84.992 | 0.62x |
| wide_arrays.json | ujson | 6.831 | 6.882 | 6.951 | 84.992 | 0.32x |
| wide_arrays.json | json | 17.127 | 17.239 | 17.435 | 84.992 | 0.13x |
| mixed.json | strata | 0.147 | 0.152 | 0.195 | 84.992 | 1.00x |
| mixed.json | orjson | 0.176 | 0.178 | 0.189 | 84.992 | 0.85x |
| mixed.json | msgspec | 0.189 | 0.196 | 0.215 | 84.992 | 0.78x |
| mixed.json | ujson | 0.338 | 0.349 | 0.375 | 84.992 | 0.43x |
| mixed.json | json | 0.633 | 0.648 | 0.665 | 84.992 | 0.23x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.071 | 0.080 | 0.094 | 81.035 | 1.00x |
| users.json $[*].id | jmespath | 0.476 | 0.487 | 0.497 | 81.035 | 0.16x |
| users.json $[*].id | jsonpath-ng | 2.784 | 3.018 | 3.138 | 81.035 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.418 | 0.434 | 0.462 | 81.035 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.947 | 2.982 | 3.021 | 81.035 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 19.491 | 20.082 | 21.887 | 81.035 | 0.02x |
| users.json $..total | strata | 1.777 | 1.827 | 1.955 | 81.035 | 1.00x |
| users.json $..total | jsonpath-ng | 384.764 | 386.367 | 387.879 | 81.035 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.224 | 3.254 | 3.287 | 81.035 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.768 | 16.234 | 17.461 | 81.035 | 0.20x |
| users.json $[*].id | orjson+jsonpath-ng | 18.023 | 18.787 | 19.612 | 81.035 | 0.17x |
| users.json $[*].orders[*].total | strata | 3.460 | 3.491 | 3.558 | 81.035 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.868 | 18.996 | 20.244 | 81.035 | 0.18x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 39.118 | 40.193 | 45.613 | 81.035 | 0.09x |
| users.json $..total | strata | 13.982 | 17.196 | 18.887 | 81.035 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 410.749 | 413.580 | 416.764 | 81.035 | 0.04x |

