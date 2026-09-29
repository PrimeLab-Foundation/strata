# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 271a2a0864aa430c4690ca9b609e4201e13cfb51
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: AMD EPYC 9V45 96-Core Processor
- compiler_flags: -fno-strict-overflow -Wsign-compare -DNDEBUG -g -O3 -Wall -fPIC -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -fomit-frame-pointer -march=native -flto=thin -fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata -fsplit-machine-functions -shared -Wl,--rpath=/opt/hostedtoolcache/Python/3.12.14/x64/lib (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 5.244 | 5.297 | 8.250 | 65.156 | 1.00x |
| users.json | orjson | 7.906 | 8.052 | 10.378 | 65.156 | 0.66x |
| users.json | msgspec | 7.658 | 7.742 | 10.102 | 65.156 | 0.68x |
| users.json | ujson | 10.696 | 10.958 | 14.131 | 65.156 | 0.48x |
| users.json | pysimdjson | 10.635 | 10.908 | 13.517 | 65.156 | 0.49x |
| users.json | json | 13.737 | 13.808 | 14.797 | 65.156 | 0.38x |
| flat.json | strata | 0.508 | 0.518 | 0.557 | 65.070 | 1.00x |
| flat.json | orjson | 0.637 | 0.651 | 0.688 | 65.070 | 0.79x |
| flat.json | msgspec | 0.598 | 0.607 | 0.612 | 65.070 | 0.85x |
| flat.json | ujson | 0.935 | 0.942 | 0.971 | 65.070 | 0.55x |
| flat.json | pysimdjson | 0.971 | 0.980 | 0.988 | 65.070 | 0.53x |
| flat.json | json | 1.228 | 1.246 | 1.292 | 65.070 | 0.42x |
| nested.json | strata | 0.426 | 0.433 | 0.442 | 65.070 | 1.00x |
| nested.json | orjson | 0.552 | 0.556 | 0.569 | 65.070 | 0.78x |
| nested.json | msgspec | 0.527 | 0.532 | 0.549 | 65.070 | 0.81x |
| nested.json | ujson | 0.809 | 0.820 | 0.835 | 65.070 | 0.53x |
| nested.json | pysimdjson | 0.766 | 0.780 | 0.784 | 65.070 | 0.55x |
| nested.json | json | 1.288 | 1.296 | 1.353 | 65.070 | 0.33x |
| wide_arrays.json | strata | 2.414 | 2.454 | 2.637 | 77.469 | 1.00x |
| wide_arrays.json | orjson | 3.180 | 3.230 | 3.471 | 77.469 | 0.76x |
| wide_arrays.json | msgspec | 3.591 | 3.667 | 3.939 | 77.469 | 0.67x |
| wide_arrays.json | ujson | 4.262 | 4.356 | 4.660 | 77.469 | 0.56x |
| wide_arrays.json | pysimdjson | 3.591 | 3.661 | 3.871 | 77.469 | 0.67x |
| wide_arrays.json | json | 8.787 | 8.908 | 9.594 | 77.469 | 0.28x |
| mixed.json | strata | 0.099 | 0.100 | 0.110 | 77.469 | 1.00x |
| mixed.json | orjson | 0.126 | 0.130 | 0.137 | 77.469 | 0.77x |
| mixed.json | msgspec | 0.129 | 0.131 | 0.135 | 77.469 | 0.77x |
| mixed.json | ujson | 0.178 | 0.184 | 0.193 | 77.469 | 0.55x |
| mixed.json | pysimdjson | 0.166 | 0.168 | 0.186 | 77.469 | 0.60x |
| mixed.json | json | 0.265 | 0.269 | 0.281 | 77.469 | 0.37x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.268 | 1.275 | 1.281 | 47.344 | 1.00x |
| users.json | orjson | 1.227 | 1.246 | 1.265 | 47.344 | 1.02x |
| users.json | msgspec | 2.302 | 2.326 | 2.379 | 47.344 | 0.55x |
| users.json | ujson | 6.317 | 6.430 | 6.511 | 47.344 | 0.20x |
| users.json | json | 11.416 | 11.455 | 11.525 | 47.344 | 0.11x |
| flat.json | strata | 0.186 | 0.189 | 0.197 | 65.070 | 1.00x |
| flat.json | orjson | 0.175 | 0.177 | 0.184 | 65.070 | 1.06x |
| flat.json | msgspec | 0.280 | 0.283 | 0.309 | 65.070 | 0.67x |
| flat.json | ujson | 0.594 | 0.607 | 0.622 | 65.070 | 0.31x |
| flat.json | json | 0.999 | 1.008 | 1.039 | 65.070 | 0.19x |
| nested.json | strata | 0.118 | 0.120 | 0.131 | 65.070 | 1.00x |
| nested.json | orjson | 0.134 | 0.135 | 0.149 | 65.070 | 0.89x |
| nested.json | msgspec | 0.237 | 0.239 | 0.258 | 65.070 | 0.50x |
| nested.json | ujson | 0.611 | 0.618 | 0.671 | 65.070 | 0.19x |
| nested.json | json | 1.201 | 1.207 | 1.286 | 65.070 | 0.10x |
| wide_arrays.json | strata | 1.073 | 1.080 | 1.115 | 77.469 | 1.00x |
| wide_arrays.json | orjson | 1.002 | 1.007 | 1.046 | 77.469 | 1.07x |
| wide_arrays.json | msgspec | 1.833 | 1.842 | 1.931 | 77.469 | 0.59x |
| wide_arrays.json | ujson | 3.547 | 3.591 | 3.756 | 77.469 | 0.30x |
| wide_arrays.json | json | 8.969 | 9.078 | 9.442 | 77.469 | 0.12x |
| mixed.json | strata | 0.035 | 0.036 | 0.036 | 77.469 | 1.00x |
| mixed.json | orjson | 0.031 | 0.032 | 0.032 | 77.469 | 1.13x |
| mixed.json | msgspec | 0.045 | 0.046 | 0.048 | 77.469 | 0.78x |
| mixed.json | ujson | 0.128 | 0.130 | 0.137 | 77.469 | 0.27x |
| mixed.json | json | 0.271 | 0.276 | 0.285 | 77.469 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 5.711 | 5.869 | 7.299 | 66.609 | 1.00x |
| users.json | orjson | 8.693 | 8.926 | 9.300 | 66.609 | 0.66x |
| users.json | msgspec | 8.204 | 8.410 | 8.737 | 66.609 | 0.70x |
| users.json | ujson | 11.788 | 12.756 | 14.233 | 66.609 | 0.46x |
| users.json | json | 14.618 | 14.824 | 15.087 | 66.609 | 0.40x |
| flat.json | strata | 0.518 | 0.522 | 0.530 | 65.070 | 1.00x |
| flat.json | orjson | 0.661 | 0.667 | 0.673 | 65.070 | 0.78x |
| flat.json | msgspec | 0.624 | 0.631 | 0.637 | 65.070 | 0.83x |
| flat.json | ujson | 0.979 | 0.989 | 1.025 | 65.070 | 0.53x |
| flat.json | json | 1.249 | 1.258 | 1.262 | 65.070 | 0.41x |
| nested.json | strata | 0.443 | 0.453 | 0.463 | 65.070 | 1.00x |
| nested.json | orjson | 0.578 | 0.599 | 0.610 | 65.070 | 0.76x |
| nested.json | msgspec | 0.561 | 0.573 | 0.585 | 65.070 | 0.79x |
| nested.json | ujson | 0.848 | 0.876 | 0.891 | 65.070 | 0.52x |
| nested.json | json | 1.314 | 1.356 | 1.384 | 65.070 | 0.33x |
| wide_arrays.json | strata | 2.500 | 2.533 | 2.577 | 77.469 | 1.00x |
| wide_arrays.json | orjson | 3.338 | 3.366 | 3.468 | 77.469 | 0.75x |
| wide_arrays.json | msgspec | 3.751 | 3.776 | 3.832 | 77.469 | 0.67x |
| wide_arrays.json | ujson | 4.476 | 4.502 | 4.609 | 77.469 | 0.56x |
| wide_arrays.json | json | 8.929 | 8.979 | 9.278 | 77.469 | 0.28x |
| mixed.json | strata | 0.107 | 0.107 | 0.112 | 77.469 | 1.00x |
| mixed.json | orjson | 0.148 | 0.149 | 0.151 | 77.469 | 0.72x |
| mixed.json | msgspec | 0.149 | 0.151 | 0.163 | 77.469 | 0.71x |
| mixed.json | ujson | 0.201 | 0.203 | 0.222 | 77.469 | 0.53x |
| mixed.json | json | 0.289 | 0.295 | 0.301 | 77.469 | 0.36x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 5.683 | 5.737 | 5.925 | 65.070 | 1.00x |
| users.ndjson | orjson | 9.951 | 10.128 | 10.417 | 65.070 | 0.57x |
| users.ndjson | msgspec | 9.744 | 10.036 | 10.496 | 65.070 | 0.57x |
| users.ndjson | ujson | 13.368 | 14.045 | 15.112 | 65.070 | 0.41x |
| users.ndjson | json | 18.026 | 18.345 | 18.891 | 65.070 | 0.31x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.820 | 1.841 | 1.860 | 66.609 | 1.00x |
| users.json | orjson | 1.777 | 1.805 | 1.838 | 66.609 | 1.02x |
| users.json | msgspec | 2.854 | 2.942 | 4.682 | 66.609 | 0.63x |
| users.json | ujson | 6.882 | 6.940 | 7.039 | 66.609 | 0.27x |
| users.json | json | 11.878 | 11.981 | 14.459 | 66.609 | 0.15x |
| flat.json | strata | 0.389 | 0.394 | 0.423 | 65.070 | 1.00x |
| flat.json | orjson | 0.376 | 0.393 | 0.416 | 65.070 | 1.00x |
| flat.json | msgspec | 0.482 | 0.498 | 2.728 | 65.070 | 0.79x |
| flat.json | ujson | 0.800 | 0.816 | 2.158 | 65.070 | 0.48x |
| flat.json | json | 1.198 | 1.233 | 1.277 | 65.070 | 0.32x |
| nested.json | strata | 0.292 | 0.311 | 2.596 | 65.070 | 1.00x |
| nested.json | orjson | 0.323 | 0.329 | 1.748 | 65.070 | 0.94x |
| nested.json | msgspec | 0.417 | 0.434 | 0.464 | 65.070 | 0.72x |
| nested.json | ujson | 0.778 | 0.793 | 0.824 | 65.070 | 0.39x |
| nested.json | json | 1.396 | 1.412 | 1.446 | 65.070 | 0.22x |
| wide_arrays.json | strata | 1.473 | 1.505 | 1.544 | 77.469 | 1.00x |
| wide_arrays.json | orjson | 1.400 | 1.426 | 1.512 | 77.469 | 1.06x |
| wide_arrays.json | msgspec | 2.232 | 2.275 | 3.936 | 77.469 | 0.66x |
| wide_arrays.json | ujson | 3.982 | 4.018 | 4.150 | 77.469 | 0.37x |
| wide_arrays.json | json | 9.337 | 9.454 | 10.908 | 77.469 | 0.16x |
| mixed.json | strata | 0.185 | 0.191 | 0.212 | 77.469 | 1.00x |
| mixed.json | orjson | 0.186 | 0.201 | 0.252 | 77.469 | 0.95x |
| mixed.json | msgspec | 0.197 | 0.212 | 0.235 | 77.469 | 0.90x |
| mixed.json | ujson | 0.293 | 0.303 | 0.336 | 77.469 | 0.63x |
| mixed.json | json | 0.438 | 0.441 | 0.462 | 77.469 | 0.43x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.036 | 0.036 | 0.043 | 66.609 | 1.00x |
| users.json $[*].id | jmespath | 0.249 | 0.250 | 0.268 | 66.609 | 0.14x |
| users.json $[*].id | jsonpath-ng | 1.519 | 1.532 | 1.551 | 66.609 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.231 | 0.233 | 0.241 | 66.621 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.607 | 1.618 | 1.634 | 66.621 | 0.14x |
| users.json $[*].orders[*].total | jsonpath-ng | 10.723 | 10.843 | 10.985 | 66.621 | 0.02x |
| users.json $..total | strata | 0.956 | 0.962 | 0.981 | 66.703 | 1.00x |
| users.json $..total | jsonpath-ng | 213.006 | 213.433 | 217.546 | 66.703 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 2.023 | 2.048 | 2.194 | 66.621 | 1.00x |
| users.json $[*].id | orjson+jmespath | 9.370 | 9.616 | 10.070 | 66.621 | 0.21x |
| users.json $[*].id | orjson+jsonpath-ng | 10.642 | 10.845 | 11.661 | 66.621 | 0.19x |
| users.json $[*].orders[*].total | strata | 2.127 | 2.149 | 3.191 | 66.703 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 10.990 | 11.272 | 21.619 | 66.703 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 22.587 | 22.995 | 23.811 | 66.703 | 0.09x |
| users.json $..total | strata | 7.937 | 9.176 | 10.055 | 66.707 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 223.101 | 227.270 | 242.805 | 66.707 | 0.04x |

