# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: b32d39824b8fea15839872cab8834a1b0fef1294
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
| users.json | strata | 7.802 | 7.888 | 11.664 | 64.406 | 1.00x |
| users.json | orjson | 10.661 | 10.840 | 13.680 | 64.406 | 0.73x |
| users.json | msgspec | 10.490 | 10.602 | 14.556 | 64.406 | 0.74x |
| users.json | ujson | 14.052 | 14.343 | 18.598 | 64.406 | 0.55x |
| users.json | pysimdjson | 14.863 | 15.204 | 18.021 | 64.406 | 0.52x |
| users.json | json | 16.314 | 16.606 | 17.701 | 64.406 | 0.47x |
| flat.json | strata | 0.670 | 0.685 | 0.693 | 63.246 | 1.00x |
| flat.json | orjson | 0.801 | 0.818 | 0.823 | 63.246 | 0.84x |
| flat.json | msgspec | 0.764 | 0.778 | 0.806 | 63.246 | 0.88x |
| flat.json | ujson | 1.175 | 1.191 | 1.218 | 63.246 | 0.57x |
| flat.json | pysimdjson | 1.235 | 1.266 | 1.297 | 63.246 | 0.54x |
| flat.json | json | 1.323 | 1.335 | 1.350 | 63.246 | 0.51x |
| nested.json | strata | 0.619 | 0.631 | 0.651 | 63.246 | 1.00x |
| nested.json | orjson | 0.775 | 0.793 | 0.812 | 63.246 | 0.80x |
| nested.json | msgspec | 0.739 | 0.747 | 0.766 | 63.246 | 0.84x |
| nested.json | ujson | 1.096 | 1.120 | 1.145 | 63.246 | 0.56x |
| nested.json | pysimdjson | 1.104 | 1.111 | 1.180 | 63.246 | 0.57x |
| nested.json | json | 1.417 | 1.431 | 1.461 | 63.246 | 0.44x |
| wide_arrays.json | strata | 3.456 | 3.503 | 3.698 | 80.789 | 1.00x |
| wide_arrays.json | orjson | 4.327 | 4.373 | 4.483 | 80.789 | 0.80x |
| wide_arrays.json | msgspec | 4.740 | 4.794 | 4.957 | 80.789 | 0.73x |
| wide_arrays.json | ujson | 5.905 | 5.975 | 6.032 | 80.789 | 0.59x |
| wide_arrays.json | pysimdjson | 4.953 | 5.085 | 5.181 | 80.789 | 0.69x |
| wide_arrays.json | json | 7.654 | 7.711 | 7.907 | 80.789 | 0.45x |
| mixed.json | strata | 0.150 | 0.152 | 0.163 | 80.852 | 1.00x |
| mixed.json | orjson | 0.183 | 0.186 | 0.191 | 80.852 | 0.82x |
| mixed.json | msgspec | 0.184 | 0.196 | 0.206 | 80.852 | 0.77x |
| mixed.json | ujson | 0.239 | 0.243 | 0.255 | 80.852 | 0.62x |
| mixed.json | pysimdjson | 0.236 | 0.241 | 0.251 | 80.852 | 0.63x |
| mixed.json | json | 0.344 | 0.349 | 0.359 | 80.852 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.799 | 1.815 | 1.845 | 47.250 | 1.00x |
| users.json | orjson | 1.989 | 2.000 | 2.033 | 47.250 | 0.91x |
| users.json | msgspec | 3.216 | 3.229 | 3.678 | 47.250 | 0.56x |
| users.json | ujson | 8.729 | 8.805 | 9.038 | 47.250 | 0.21x |
| users.json | json | 16.481 | 16.629 | 16.781 | 47.250 | 0.11x |
| flat.json | strata | 0.241 | 0.247 | 0.260 | 63.246 | 1.00x |
| flat.json | orjson | 0.241 | 0.248 | 0.256 | 63.246 | 1.00x |
| flat.json | msgspec | 0.370 | 0.374 | 0.388 | 63.246 | 0.66x |
| flat.json | ujson | 0.801 | 0.831 | 0.852 | 63.246 | 0.30x |
| flat.json | json | 1.444 | 1.456 | 1.495 | 63.246 | 0.17x |
| nested.json | strata | 0.174 | 0.177 | 0.196 | 63.246 | 1.00x |
| nested.json | orjson | 0.218 | 0.223 | 0.231 | 63.246 | 0.79x |
| nested.json | msgspec | 0.323 | 0.332 | 0.344 | 63.246 | 0.53x |
| nested.json | ujson | 0.835 | 0.840 | 0.856 | 63.246 | 0.21x |
| nested.json | json | 1.808 | 1.825 | 1.844 | 63.246 | 0.10x |
| wide_arrays.json | strata | 1.376 | 1.383 | 1.420 | 80.789 | 1.00x |
| wide_arrays.json | orjson | 1.466 | 1.484 | 1.631 | 80.789 | 0.93x |
| wide_arrays.json | msgspec | 2.365 | 2.383 | 2.505 | 80.789 | 0.58x |
| wide_arrays.json | ujson | 4.958 | 4.990 | 5.045 | 80.789 | 0.28x |
| wide_arrays.json | json | 13.046 | 13.089 | 13.206 | 80.789 | 0.11x |
| mixed.json | strata | 0.047 | 0.048 | 0.050 | 80.852 | 1.00x |
| mixed.json | orjson | 0.047 | 0.048 | 0.052 | 80.852 | 1.01x |
| mixed.json | msgspec | 0.066 | 0.068 | 0.071 | 80.852 | 0.71x |
| mixed.json | ujson | 0.176 | 0.178 | 0.182 | 80.852 | 0.27x |
| mixed.json | json | 0.398 | 0.408 | 0.415 | 80.852 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.238 | 8.367 | 9.735 | 64.848 | 1.00x |
| users.json | orjson | 11.203 | 11.463 | 11.943 | 64.848 | 0.73x |
| users.json | msgspec | 10.975 | 11.167 | 11.616 | 64.848 | 0.75x |
| users.json | ujson | 15.339 | 15.808 | 17.560 | 64.848 | 0.53x |
| users.json | json | 16.816 | 17.100 | 17.380 | 64.848 | 0.49x |
| flat.json | strata | 0.706 | 0.711 | 0.730 | 63.246 | 1.00x |
| flat.json | orjson | 0.835 | 0.864 | 0.903 | 63.246 | 0.82x |
| flat.json | msgspec | 0.808 | 0.824 | 0.833 | 63.246 | 0.86x |
| flat.json | ujson | 1.229 | 1.260 | 1.299 | 63.246 | 0.56x |
| flat.json | json | 1.374 | 1.386 | 1.694 | 63.246 | 0.51x |
| nested.json | strata | 0.637 | 0.651 | 0.663 | 63.246 | 1.00x |
| nested.json | orjson | 0.824 | 0.839 | 0.861 | 63.246 | 0.78x |
| nested.json | msgspec | 0.779 | 0.790 | 0.812 | 63.246 | 0.82x |
| nested.json | ujson | 1.156 | 1.164 | 1.211 | 63.246 | 0.56x |
| nested.json | json | 1.469 | 1.482 | 1.552 | 63.246 | 0.44x |
| wide_arrays.json | strata | 3.455 | 3.485 | 3.573 | 80.852 | 1.00x |
| wide_arrays.json | orjson | 4.308 | 4.356 | 4.392 | 80.852 | 0.80x |
| wide_arrays.json | msgspec | 4.830 | 4.864 | 4.915 | 80.852 | 0.72x |
| wide_arrays.json | ujson | 6.001 | 6.045 | 6.082 | 80.852 | 0.58x |
| wide_arrays.json | json | 7.610 | 7.685 | 7.932 | 80.852 | 0.45x |
| mixed.json | strata | 0.164 | 0.169 | 0.183 | 80.852 | 1.00x |
| mixed.json | orjson | 0.220 | 0.225 | 0.271 | 80.852 | 0.75x |
| mixed.json | msgspec | 0.220 | 0.229 | 0.243 | 80.852 | 0.74x |
| mixed.json | ujson | 0.281 | 0.291 | 0.303 | 80.852 | 0.58x |
| mixed.json | json | 0.380 | 0.389 | 0.397 | 80.852 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 8.316 | 8.464 | 8.790 | 63.246 | 1.00x |
| users.ndjson | orjson | 13.854 | 14.055 | 14.321 | 63.246 | 0.60x |
| users.ndjson | msgspec | 13.768 | 13.956 | 14.148 | 63.246 | 0.61x |
| users.ndjson | ujson | 17.909 | 18.111 | 18.533 | 63.246 | 0.47x |
| users.ndjson | json | 22.427 | 22.766 | 24.003 | 63.246 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.226 | 2.286 | 2.321 | 64.848 | 1.00x |
| users.json | orjson | 2.443 | 2.474 | 2.515 | 64.848 | 0.92x |
| users.json | msgspec | 3.679 | 3.717 | 3.784 | 64.848 | 0.62x |
| users.json | ujson | 9.323 | 9.382 | 9.479 | 64.848 | 0.24x |
| users.json | json | 17.379 | 17.549 | 17.654 | 64.848 | 0.13x |
| flat.json | strata | 0.343 | 0.363 | 0.379 | 63.246 | 1.00x |
| flat.json | orjson | 0.356 | 0.366 | 0.381 | 63.246 | 0.99x |
| flat.json | msgspec | 0.480 | 0.501 | 0.521 | 63.246 | 0.72x |
| flat.json | ujson | 0.935 | 0.952 | 0.979 | 63.246 | 0.38x |
| flat.json | json | 1.575 | 1.598 | 1.621 | 63.246 | 0.23x |
| nested.json | strata | 0.259 | 0.274 | 0.290 | 63.246 | 1.00x |
| nested.json | orjson | 0.324 | 0.332 | 0.346 | 63.246 | 0.82x |
| nested.json | msgspec | 0.423 | 0.435 | 0.445 | 63.246 | 0.63x |
| nested.json | ujson | 0.941 | 0.950 | 0.968 | 63.246 | 0.29x |
| nested.json | json | 1.935 | 1.945 | 1.961 | 63.246 | 0.14x |
| wide_arrays.json | strata | 1.694 | 1.707 | 1.722 | 80.852 | 1.00x |
| wide_arrays.json | orjson | 1.788 | 1.820 | 1.874 | 80.852 | 0.94x |
| wide_arrays.json | msgspec | 2.716 | 2.745 | 2.768 | 80.852 | 0.62x |
| wide_arrays.json | ujson | 5.327 | 5.358 | 5.396 | 80.852 | 0.32x |
| wide_arrays.json | json | 13.357 | 13.425 | 13.473 | 80.852 | 0.13x |
| mixed.json | strata | 0.110 | 0.116 | 0.123 | 80.852 | 1.00x |
| mixed.json | orjson | 0.119 | 0.124 | 0.135 | 80.852 | 0.94x |
| mixed.json | msgspec | 0.138 | 0.142 | 0.161 | 80.852 | 0.82x |
| mixed.json | ujson | 0.257 | 0.265 | 0.275 | 80.852 | 0.44x |
| mixed.json | json | 0.478 | 0.494 | 0.515 | 80.852 | 0.23x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.055 | 0.057 | 0.058 | 64.848 | 1.00x |
| users.json $[*].id | jmespath | 0.359 | 0.367 | 0.377 | 64.848 | 0.15x |
| users.json $[*].id | jsonpath-ng | 2.121 | 2.152 | 2.257 | 64.848 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.337 | 0.350 | 0.360 | 64.863 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.306 | 2.342 | 2.433 | 64.863 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 15.014 | 15.201 | 15.418 | 64.863 | 0.02x |
| users.json $..total | strata | 1.389 | 1.416 | 1.473 | 64.879 | 1.00x |
| users.json $..total | jsonpath-ng | 296.743 | 300.040 | 301.259 | 64.879 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 2.596 | 2.610 | 2.631 | 64.863 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.206 | 12.441 | 12.737 | 64.863 | 0.21x |
| users.json $[*].id | orjson+jsonpath-ng | 13.909 | 14.219 | 14.461 | 64.863 | 0.18x |
| users.json $[*].orders[*].total | strata | 2.796 | 2.834 | 2.849 | 64.879 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 14.989 | 15.165 | 15.313 | 64.879 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 30.390 | 31.091 | 31.525 | 64.879 | 0.09x |
| users.json $..total | strata | 11.014 | 11.848 | 12.297 | 64.883 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 318.213 | 321.621 | 323.224 | 64.883 | 0.04x |

