# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 6e4d93e74610755e4c46406e91c8aaf4ab6817a1
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: INTEL(R) XEON(R) PLATINUM 8573C
- compiler_flags: -fno-strict-overflow -Wsign-compare -DNDEBUG -g -O3 -Wall -fPIC -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -fomit-frame-pointer -march=native -flto=thin -fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata -fsplit-machine-functions -shared -Wl,--rpath=/opt/hostedtoolcache/Python/3.12.14/x64/lib (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.530 | 8.665 | 10.746 | 65.652 | 1.00x |
| users.json | orjson | 12.385 | 12.452 | 14.329 | 65.652 | 0.70x |
| users.json | msgspec | 12.758 | 12.879 | 14.831 | 65.652 | 0.67x |
| users.json | ujson | 16.218 | 16.577 | 19.750 | 65.652 | 0.52x |
| users.json | pysimdjson | 16.221 | 16.342 | 18.324 | 65.652 | 0.53x |
| users.json | json | 19.105 | 19.201 | 20.058 | 65.652 | 0.45x |
| flat.json | strata | 0.761 | 0.772 | 0.866 | 79.309 | 1.00x |
| flat.json | orjson | 0.914 | 0.933 | 0.951 | 79.309 | 0.83x |
| flat.json | msgspec | 0.854 | 0.879 | 1.119 | 79.309 | 0.88x |
| flat.json | ujson | 1.200 | 1.223 | 1.455 | 79.309 | 0.63x |
| flat.json | pysimdjson | 1.315 | 1.337 | 1.548 | 79.309 | 0.58x |
| flat.json | json | 1.546 | 1.617 | 1.904 | 79.309 | 0.48x |
| nested.json | strata | 0.634 | 0.642 | 0.647 | 79.340 | 1.00x |
| nested.json | orjson | 0.788 | 0.808 | 0.817 | 79.340 | 0.79x |
| nested.json | msgspec | 0.803 | 0.817 | 0.826 | 79.340 | 0.79x |
| nested.json | ujson | 1.150 | 1.160 | 1.195 | 79.340 | 0.55x |
| nested.json | pysimdjson | 1.114 | 1.120 | 1.153 | 79.340 | 0.57x |
| nested.json | json | 1.599 | 1.618 | 1.642 | 79.340 | 0.40x |
| wide_arrays.json | strata | 3.369 | 3.389 | 3.479 | 83.344 | 1.00x |
| wide_arrays.json | orjson | 4.489 | 4.658 | 4.731 | 83.344 | 0.73x |
| wide_arrays.json | msgspec | 4.839 | 4.873 | 6.015 | 83.344 | 0.70x |
| wide_arrays.json | ujson | 5.969 | 6.015 | 6.437 | 83.344 | 0.56x |
| wide_arrays.json | pysimdjson | 4.871 | 4.926 | 4.988 | 83.344 | 0.69x |
| wide_arrays.json | json | 8.095 | 8.136 | 8.248 | 83.344 | 0.42x |
| mixed.json | strata | 0.170 | 0.170 | 0.183 | 83.344 | 1.00x |
| mixed.json | orjson | 0.204 | 0.210 | 0.232 | 83.344 | 0.81x |
| mixed.json | msgspec | 0.212 | 0.218 | 0.231 | 83.344 | 0.78x |
| mixed.json | ujson | 0.267 | 0.279 | 0.309 | 83.344 | 0.61x |
| mixed.json | pysimdjson | 0.264 | 0.275 | 0.283 | 83.344 | 0.62x |
| mixed.json | json | 0.423 | 0.506 | 0.539 | 83.344 | 0.34x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.664 | 1.678 | 1.709 | 64.781 | 1.00x |
| users.json | orjson | 2.044 | 2.069 | 2.131 | 64.781 | 0.81x |
| users.json | msgspec | 3.386 | 3.437 | 3.503 | 64.781 | 0.49x |
| users.json | ujson | 9.848 | 9.893 | 9.948 | 64.781 | 0.17x |
| users.json | json | 17.812 | 17.901 | 18.131 | 64.781 | 0.09x |
| flat.json | strata | 0.241 | 0.250 | 0.266 | 79.340 | 1.00x |
| flat.json | orjson | 0.243 | 0.255 | 0.272 | 79.340 | 0.98x |
| flat.json | msgspec | 0.370 | 0.377 | 0.391 | 79.340 | 0.66x |
| flat.json | ujson | 0.881 | 0.894 | 0.924 | 79.340 | 0.28x |
| flat.json | json | 1.582 | 1.599 | 1.636 | 79.340 | 0.16x |
| nested.json | strata | 0.159 | 0.167 | 0.187 | 79.340 | 1.00x |
| nested.json | orjson | 0.223 | 0.228 | 0.232 | 79.340 | 0.73x |
| nested.json | msgspec | 0.329 | 0.333 | 0.342 | 79.340 | 0.50x |
| nested.json | ujson | 0.918 | 0.928 | 0.956 | 79.340 | 0.18x |
| nested.json | json | 1.966 | 1.985 | 2.056 | 79.340 | 0.08x |
| wide_arrays.json | strata | 1.297 | 1.308 | 1.325 | 83.344 | 1.00x |
| wide_arrays.json | orjson | 1.392 | 1.396 | 1.408 | 83.344 | 0.94x |
| wide_arrays.json | msgspec | 2.283 | 2.296 | 2.315 | 83.344 | 0.57x |
| wide_arrays.json | ujson | 4.924 | 4.969 | 5.103 | 83.344 | 0.26x |
| wide_arrays.json | json | 13.046 | 13.097 | 13.310 | 83.344 | 0.10x |
| mixed.json | strata | 0.053 | 0.055 | 0.060 | 83.344 | 1.00x |
| mixed.json | orjson | 0.052 | 0.053 | 0.056 | 83.344 | 1.04x |
| mixed.json | msgspec | 0.068 | 0.075 | 0.094 | 83.344 | 0.73x |
| mixed.json | ujson | 0.203 | 0.204 | 0.219 | 83.344 | 0.27x |
| mixed.json | json | 0.481 | 0.501 | 0.555 | 83.344 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.781 | 8.929 | 9.665 | 77.992 | 1.00x |
| users.json | orjson | 12.494 | 12.592 | 13.090 | 77.992 | 0.71x |
| users.json | msgspec | 12.985 | 13.048 | 13.234 | 77.992 | 0.68x |
| users.json | ujson | 16.857 | 17.575 | 18.209 | 77.992 | 0.51x |
| users.json | json | 19.355 | 19.450 | 19.768 | 77.992 | 0.46x |
| flat.json | strata | 0.790 | 0.807 | 0.829 | 79.340 | 1.00x |
| flat.json | orjson | 0.974 | 0.988 | 1.022 | 79.340 | 0.82x |
| flat.json | msgspec | 0.908 | 0.931 | 0.943 | 79.340 | 0.87x |
| flat.json | ujson | 1.296 | 1.314 | 1.346 | 79.340 | 0.61x |
| flat.json | json | 1.589 | 1.657 | 1.728 | 79.340 | 0.49x |
| nested.json | strata | 0.650 | 0.660 | 0.711 | 79.340 | 1.00x |
| nested.json | orjson | 0.861 | 0.865 | 0.907 | 79.340 | 0.76x |
| nested.json | msgspec | 0.861 | 0.877 | 0.913 | 79.340 | 0.75x |
| nested.json | ujson | 1.227 | 1.246 | 1.273 | 79.340 | 0.53x |
| nested.json | json | 1.651 | 1.670 | 1.703 | 79.340 | 0.40x |
| wide_arrays.json | strata | 3.396 | 3.425 | 3.490 | 83.344 | 1.00x |
| wide_arrays.json | orjson | 4.414 | 4.555 | 4.662 | 83.344 | 0.75x |
| wide_arrays.json | msgspec | 4.910 | 4.941 | 5.035 | 83.344 | 0.69x |
| wide_arrays.json | ujson | 6.096 | 6.175 | 6.209 | 83.344 | 0.55x |
| wide_arrays.json | json | 8.192 | 8.229 | 8.307 | 83.344 | 0.42x |
| mixed.json | strata | 0.183 | 0.186 | 0.236 | 83.344 | 1.00x |
| mixed.json | orjson | 0.256 | 0.265 | 0.291 | 83.344 | 0.70x |
| mixed.json | msgspec | 0.264 | 0.277 | 0.290 | 83.344 | 0.67x |
| mixed.json | ujson | 0.335 | 0.343 | 0.362 | 83.344 | 0.54x |
| mixed.json | json | 0.472 | 0.548 | 0.609 | 83.344 | 0.34x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 8.886 | 8.939 | 9.893 | 79.309 | 1.00x |
| users.ndjson | orjson | 14.088 | 14.152 | 15.652 | 79.309 | 0.63x |
| users.ndjson | msgspec | 14.134 | 14.230 | 14.417 | 79.309 | 0.63x |
| users.ndjson | ujson | 17.970 | 18.085 | 18.346 | 79.309 | 0.49x |
| users.ndjson | json | 23.070 | 23.165 | 25.704 | 79.309 | 0.39x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.109 | 2.145 | 2.178 | 77.992 | 1.00x |
| users.json | orjson | 2.511 | 2.587 | 2.623 | 77.992 | 0.83x |
| users.json | msgspec | 3.866 | 3.907 | 4.242 | 77.992 | 0.55x |
| users.json | ujson | 10.401 | 10.515 | 11.464 | 77.992 | 0.20x |
| users.json | json | 18.440 | 18.513 | 19.539 | 77.992 | 0.12x |
| flat.json | strata | 0.345 | 0.360 | 0.437 | 79.340 | 1.00x |
| flat.json | orjson | 0.359 | 0.372 | 0.412 | 79.340 | 0.97x |
| flat.json | msgspec | 0.502 | 0.514 | 0.534 | 79.340 | 0.70x |
| flat.json | ujson | 1.046 | 1.060 | 95.886 | 79.340 | 0.34x |
| flat.json | json | 1.737 | 1.758 | 1.836 | 79.340 | 0.20x |
| nested.json | strata | 0.242 | 0.251 | 0.305 | 79.340 | 1.00x |
| nested.json | orjson | 0.322 | 0.342 | 0.362 | 79.340 | 0.73x |
| nested.json | msgspec | 0.420 | 0.440 | 0.459 | 79.340 | 0.57x |
| nested.json | ujson | 1.024 | 1.036 | 101.168 | 79.340 | 0.24x |
| nested.json | json | 2.101 | 2.182 | 136.191 | 79.340 | 0.11x |
| wide_arrays.json | strata | 1.653 | 1.683 | 35.292 | 83.344 | 1.00x |
| wide_arrays.json | orjson | 1.753 | 1.782 | 1.827 | 83.344 | 0.94x |
| wide_arrays.json | msgspec | 2.623 | 2.671 | 2.705 | 83.344 | 0.63x |
| wide_arrays.json | ujson | 5.359 | 5.398 | 5.416 | 83.344 | 0.31x |
| wide_arrays.json | json | 13.535 | 13.588 | 13.650 | 83.344 | 0.12x |
| mixed.json | strata | 0.112 | 0.117 | 0.132 | 83.344 | 1.00x |
| mixed.json | orjson | 0.128 | 0.130 | 0.138 | 83.344 | 0.90x |
| mixed.json | msgspec | 0.140 | 0.145 | 0.157 | 83.344 | 0.81x |
| mixed.json | ujson | 0.290 | 0.294 | 0.305 | 83.344 | 0.40x |
| mixed.json | json | 0.564 | 0.582 | 0.660 | 83.344 | 0.20x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.060 | 0.062 | 0.077 | 77.992 | 1.00x |
| users.json $[*].id | jmespath | 0.424 | 0.441 | 0.462 | 77.992 | 0.14x |
| users.json $[*].id | jsonpath-ng | 2.060 | 2.080 | 2.161 | 77.992 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.438 | 0.448 | 0.634 | 78.016 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.593 | 2.639 | 4.046 | 78.016 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 14.336 | 14.439 | 16.724 | 78.016 | 0.03x |
| users.json $..total | strata | 1.569 | 1.598 | 2.223 | 80.945 | 1.00x |
| users.json $..total | jsonpath-ng | 274.627 | 276.125 | 313.556 | 80.945 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 2.488 | 2.637 | 2.788 | 78.016 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.966 | 13.011 | 13.208 | 78.016 | 0.20x |
| users.json $[*].id | orjson+jsonpath-ng | 14.555 | 14.670 | 15.064 | 78.016 | 0.18x |
| users.json $[*].orders[*].total | strata | 2.698 | 2.813 | 2.865 | 80.945 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.110 | 15.218 | 16.604 | 80.945 | 0.18x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 31.775 | 32.038 | 33.150 | 80.945 | 0.09x |
| users.json $..total | strata | 11.322 | 12.097 | 12.937 | 80.945 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 288.159 | 293.236 | 300.176 | 80.945 | 0.04x |

