# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c20ac86eedff410e10c973bc3b1f19f6e9a5f56e
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
| users.json | strata | 7.705 | 8.626 | 12.311 | 66.055 | 1.00x |
| users.json | orjson | 10.913 | 11.472 | 14.299 | 66.055 | 0.75x |
| users.json | msgspec | 10.836 | 11.037 | 14.614 | 66.055 | 0.78x |
| users.json | ujson | 14.602 | 17.997 | 20.834 | 66.055 | 0.48x |
| users.json | pysimdjson | 15.141 | 17.270 | 19.849 | 66.055 | 0.50x |
| users.json | json | 16.401 | 16.852 | 18.258 | 66.055 | 0.51x |
| flat.json | strata | 0.716 | 0.747 | 0.801 | 78.336 | 1.00x |
| flat.json | orjson | 0.848 | 0.866 | 0.952 | 78.336 | 0.86x |
| flat.json | msgspec | 0.828 | 0.853 | 0.910 | 78.336 | 0.88x |
| flat.json | ujson | 1.396 | 1.443 | 1.615 | 78.336 | 0.52x |
| flat.json | pysimdjson | 1.339 | 1.375 | 1.568 | 78.336 | 0.54x |
| flat.json | json | 1.386 | 1.399 | 1.438 | 78.336 | 0.53x |
| nested.json | strata | 0.624 | 0.644 | 0.886 | 78.336 | 1.00x |
| nested.json | orjson | 0.786 | 0.809 | 0.988 | 78.336 | 0.80x |
| nested.json | msgspec | 0.757 | 0.793 | 0.869 | 78.336 | 0.81x |
| nested.json | ujson | 1.182 | 1.245 | 1.282 | 78.336 | 0.52x |
| nested.json | pysimdjson | 1.112 | 1.153 | 1.387 | 78.336 | 0.56x |
| nested.json | json | 1.427 | 1.464 | 1.584 | 78.336 | 0.44x |
| wide_arrays.json | strata | 3.472 | 3.499 | 3.854 | 82.527 | 1.00x |
| wide_arrays.json | orjson | 4.390 | 4.509 | 5.176 | 82.527 | 0.78x |
| wide_arrays.json | msgspec | 4.810 | 4.893 | 5.293 | 82.527 | 0.72x |
| wide_arrays.json | ujson | 5.917 | 6.098 | 6.559 | 82.527 | 0.57x |
| wide_arrays.json | pysimdjson | 4.984 | 5.133 | 6.500 | 82.527 | 0.68x |
| wide_arrays.json | json | 7.694 | 7.982 | 9.037 | 82.527 | 0.44x |
| mixed.json | strata | 0.151 | 0.156 | 0.165 | 82.590 | 1.00x |
| mixed.json | orjson | 0.185 | 0.192 | 0.199 | 82.590 | 0.82x |
| mixed.json | msgspec | 0.189 | 0.194 | 0.204 | 82.590 | 0.80x |
| mixed.json | ujson | 0.245 | 0.249 | 0.271 | 82.590 | 0.63x |
| mixed.json | pysimdjson | 0.239 | 0.249 | 0.263 | 82.590 | 0.63x |
| mixed.json | json | 0.348 | 0.368 | 0.414 | 82.590 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.971 | 2.034 | 2.109 | 64.984 | 1.00x |
| users.json | orjson | 2.046 | 2.118 | 2.188 | 64.984 | 0.96x |
| users.json | msgspec | 3.290 | 3.357 | 3.753 | 64.984 | 0.61x |
| users.json | ujson | 8.992 | 9.166 | 10.437 | 64.984 | 0.22x |
| users.json | json | 17.019 | 17.315 | 17.755 | 64.984 | 0.12x |
| flat.json | strata | 0.242 | 0.260 | 0.330 | 78.336 | 1.00x |
| flat.json | orjson | 0.255 | 0.275 | 0.312 | 78.336 | 0.95x |
| flat.json | msgspec | 0.388 | 0.398 | 0.455 | 78.336 | 0.65x |
| flat.json | ujson | 0.810 | 0.836 | 0.915 | 78.336 | 0.31x |
| flat.json | json | 1.503 | 1.532 | 1.613 | 78.336 | 0.17x |
| nested.json | strata | 0.187 | 0.190 | 0.207 | 78.336 | 1.00x |
| nested.json | orjson | 0.220 | 0.224 | 0.231 | 78.336 | 0.85x |
| nested.json | msgspec | 0.335 | 0.339 | 0.374 | 78.336 | 0.56x |
| nested.json | ujson | 0.840 | 0.850 | 0.861 | 78.336 | 0.22x |
| nested.json | json | 1.856 | 1.868 | 1.899 | 78.336 | 0.10x |
| wide_arrays.json | strata | 1.396 | 1.442 | 1.904 | 82.527 | 1.00x |
| wide_arrays.json | orjson | 1.522 | 1.541 | 1.694 | 82.527 | 0.94x |
| wide_arrays.json | msgspec | 2.407 | 2.458 | 3.723 | 82.527 | 0.59x |
| wide_arrays.json | ujson | 5.031 | 5.140 | 6.410 | 82.527 | 0.28x |
| wide_arrays.json | json | 13.159 | 13.341 | 14.975 | 82.527 | 0.11x |
| mixed.json | strata | 0.052 | 0.053 | 0.055 | 82.590 | 1.00x |
| mixed.json | orjson | 0.047 | 0.050 | 0.053 | 82.590 | 1.07x |
| mixed.json | msgspec | 0.068 | 0.072 | 0.077 | 82.590 | 0.74x |
| mixed.json | ujson | 0.176 | 0.178 | 0.189 | 82.590 | 0.30x |
| mixed.json | json | 0.409 | 0.417 | 0.476 | 82.590 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.484 | 10.382 | 11.600 | 77.789 | 1.00x |
| users.json | orjson | 12.109 | 12.384 | 13.297 | 77.789 | 0.84x |
| users.json | msgspec | 11.693 | 12.306 | 12.986 | 77.789 | 0.84x |
| users.json | ujson | 16.936 | 18.421 | 20.709 | 77.789 | 0.56x |
| users.json | json | 17.669 | 18.209 | 19.131 | 77.789 | 0.57x |
| flat.json | strata | 0.794 | 0.861 | 0.927 | 78.336 | 1.00x |
| flat.json | orjson | 0.957 | 1.009 | 1.107 | 78.336 | 0.85x |
| flat.json | msgspec | 0.911 | 0.967 | 1.048 | 78.336 | 0.89x |
| flat.json | ujson | 1.521 | 1.602 | 1.707 | 78.336 | 0.54x |
| flat.json | json | 1.482 | 1.519 | 1.657 | 78.336 | 0.57x |
| nested.json | strata | 0.665 | 0.699 | 0.966 | 78.336 | 1.00x |
| nested.json | orjson | 0.848 | 0.886 | 1.148 | 78.336 | 0.79x |
| nested.json | msgspec | 0.810 | 0.839 | 1.137 | 78.336 | 0.83x |
| nested.json | ujson | 1.238 | 1.313 | 1.425 | 78.336 | 0.53x |
| nested.json | json | 1.465 | 1.550 | 1.806 | 78.336 | 0.45x |
| wide_arrays.json | strata | 3.530 | 3.592 | 3.940 | 82.590 | 1.00x |
| wide_arrays.json | orjson | 4.476 | 4.569 | 4.949 | 82.590 | 0.79x |
| wide_arrays.json | msgspec | 4.917 | 4.982 | 5.348 | 82.590 | 0.72x |
| wide_arrays.json | ujson | 6.084 | 6.234 | 6.677 | 82.590 | 0.58x |
| wide_arrays.json | json | 7.828 | 7.867 | 8.199 | 82.590 | 0.46x |
| mixed.json | strata | 0.169 | 0.175 | 0.202 | 82.590 | 1.00x |
| mixed.json | orjson | 0.224 | 0.232 | 0.265 | 82.590 | 0.75x |
| mixed.json | msgspec | 0.223 | 0.232 | 0.247 | 82.590 | 0.76x |
| mixed.json | ujson | 0.288 | 0.301 | 0.334 | 82.590 | 0.58x |
| mixed.json | json | 0.385 | 0.394 | 0.431 | 82.590 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.803 | 11.378 | 12.665 | 78.336 | 1.00x |
| users.ndjson | orjson | 15.333 | 16.751 | 18.371 | 78.336 | 0.68x |
| users.ndjson | msgspec | 14.786 | 16.512 | 18.478 | 78.336 | 0.69x |
| users.ndjson | ujson | 20.794 | 22.571 | 23.671 | 78.336 | 0.50x |
| users.ndjson | json | 23.745 | 26.592 | 27.511 | 78.336 | 0.43x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.407 | 2.483 | 2.567 | 77.789 | 1.00x |
| users.json | orjson | 2.577 | 2.598 | 3.878 | 77.789 | 0.96x |
| users.json | msgspec | 3.790 | 3.825 | 3.997 | 77.789 | 0.65x |
| users.json | ujson | 9.506 | 9.697 | 70.802 | 77.789 | 0.26x |
| users.json | json | 17.901 | 18.038 | 18.193 | 77.789 | 0.14x |
| flat.json | strata | 0.507 | 0.553 | 0.657 | 78.336 | 1.00x |
| flat.json | orjson | 0.488 | 0.556 | 13.156 | 78.336 | 0.99x |
| flat.json | msgspec | 0.614 | 0.767 | 8.468 | 78.336 | 0.72x |
| flat.json | ujson | 1.070 | 1.171 | 1.308 | 78.336 | 0.47x |
| flat.json | json | 1.715 | 1.869 | 1.974 | 78.336 | 0.30x |
| nested.json | strata | 0.280 | 0.292 | 0.321 | 78.336 | 1.00x |
| nested.json | orjson | 0.332 | 0.345 | 129.414 | 78.336 | 0.85x |
| nested.json | msgspec | 0.444 | 0.464 | 0.524 | 78.336 | 0.63x |
| nested.json | ujson | 0.960 | 0.971 | 1.015 | 78.336 | 0.30x |
| nested.json | json | 1.969 | 1.989 | 2.021 | 78.336 | 0.15x |
| wide_arrays.json | strata | 1.764 | 1.842 | 2.464 | 82.590 | 1.00x |
| wide_arrays.json | orjson | 1.909 | 1.966 | 39.970 | 82.590 | 0.94x |
| wide_arrays.json | msgspec | 2.795 | 2.836 | 3.002 | 82.590 | 0.65x |
| wide_arrays.json | ujson | 5.463 | 5.724 | 44.189 | 82.590 | 0.32x |
| wide_arrays.json | json | 13.730 | 14.040 | 15.478 | 82.590 | 0.13x |
| mixed.json | strata | 0.118 | 0.124 | 0.151 | 82.590 | 1.00x |
| mixed.json | orjson | 0.126 | 0.131 | 0.151 | 82.590 | 0.95x |
| mixed.json | msgspec | 0.145 | 0.150 | 0.173 | 82.590 | 0.83x |
| mixed.json | ujson | 0.265 | 0.274 | 6.591 | 82.590 | 0.45x |
| mixed.json | json | 0.494 | 0.503 | 0.537 | 82.590 | 0.25x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.066 | 0.070 | 0.078 | 77.789 | 1.00x |
| users.json $[*].id | jmespath | 0.380 | 0.391 | 0.416 | 77.789 | 0.18x |
| users.json $[*].id | jsonpath-ng | 2.240 | 2.416 | 2.431 | 77.789 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.357 | 0.391 | 0.421 | 77.910 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.344 | 2.454 | 4.515 | 77.910 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 16.231 | 17.383 | 18.466 | 77.910 | 0.02x |
| users.json $..total | strata | 1.434 | 1.488 | 1.638 | 79.973 | 1.00x |
| users.json $..total | jsonpath-ng | 305.988 | 307.989 | 308.646 | 79.973 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 2.635 | 2.660 | 2.695 | 77.910 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.931 | 13.427 | 14.349 | 77.910 | 0.20x |
| users.json $[*].id | orjson+jsonpath-ng | 14.888 | 15.546 | 17.133 | 77.910 | 0.17x |
| users.json $[*].orders[*].total | strata | 2.836 | 2.853 | 2.896 | 79.973 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 16.087 | 16.411 | 17.020 | 79.973 | 0.17x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 32.390 | 34.950 | 36.700 | 79.973 | 0.08x |
| users.json $..total | strata | 13.791 | 16.206 | 20.800 | 79.973 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 327.660 | 330.634 | 360.349 | 79.973 | 0.05x |

