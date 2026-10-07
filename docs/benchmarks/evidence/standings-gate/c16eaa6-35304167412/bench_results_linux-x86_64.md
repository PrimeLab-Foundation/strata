# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c16eaa65c61f1cf9d4fac46e9f6d8e6b9f3b087f
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: AMD EPYC 7763 64-Core Processor
- compiler_flags: -fno-strict-overflow -Wsign-compare -DNDEBUG -g -O3 -Wall -fPIC -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -fomit-frame-pointer -march=native -flto=thin -fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata -fsplit-machine-functions -shared -Wl,--rpath=/opt/hostedtoolcache/Python/3.12.14/x64/lib (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.505 | 10.944 | 15.159 | 65.805 | 1.00x |
| users.json | orjson | 13.810 | 14.228 | 17.202 | 65.805 | 0.77x |
| users.json | msgspec | 13.559 | 13.667 | 18.613 | 65.805 | 0.80x |
| users.json | ujson | 19.320 | 20.648 | 28.248 | 65.805 | 0.53x |
| users.json | pysimdjson | 20.233 | 20.947 | 23.734 | 65.805 | 0.52x |
| users.json | json | 23.229 | 23.882 | 31.056 | 65.805 | 0.46x |
| flat.json | strata | 0.848 | 0.863 | 0.888 | 82.566 | 1.00x |
| flat.json | orjson | 0.983 | 0.987 | 1.017 | 82.566 | 0.87x |
| flat.json | msgspec | 1.024 | 1.033 | 1.058 | 82.566 | 0.84x |
| flat.json | ujson | 1.513 | 1.579 | 1.623 | 82.566 | 0.55x |
| flat.json | pysimdjson | 1.508 | 1.519 | 1.558 | 82.566 | 0.57x |
| flat.json | json | 1.897 | 1.903 | 1.915 | 82.566 | 0.45x |
| nested.json | strata | 0.791 | 0.810 | 0.851 | 82.566 | 1.00x |
| nested.json | orjson | 1.010 | 1.025 | 1.092 | 82.566 | 0.79x |
| nested.json | msgspec | 1.023 | 1.039 | 1.165 | 82.566 | 0.78x |
| nested.json | ujson | 1.489 | 1.568 | 1.586 | 82.566 | 0.52x |
| nested.json | pysimdjson | 1.404 | 1.419 | 1.491 | 82.566 | 0.57x |
| nested.json | json | 2.031 | 2.068 | 2.096 | 82.566 | 0.39x |
| wide_arrays.json | strata | 4.184 | 4.310 | 4.458 | 86.566 | 1.00x |
| wide_arrays.json | orjson | 5.318 | 5.633 | 6.453 | 86.566 | 0.77x |
| wide_arrays.json | msgspec | 5.682 | 5.970 | 6.892 | 86.566 | 0.72x |
| wide_arrays.json | ujson | 7.092 | 7.495 | 8.079 | 86.566 | 0.58x |
| wide_arrays.json | pysimdjson | 6.212 | 6.752 | 7.129 | 86.566 | 0.64x |
| wide_arrays.json | json | 10.052 | 10.700 | 11.207 | 86.566 | 0.40x |
| mixed.json | strata | 0.192 | 0.195 | 0.208 | 86.566 | 1.00x |
| mixed.json | orjson | 0.233 | 0.240 | 0.268 | 86.566 | 0.82x |
| mixed.json | msgspec | 0.241 | 0.247 | 0.272 | 86.566 | 0.79x |
| mixed.json | ujson | 0.305 | 0.313 | 0.344 | 86.566 | 0.62x |
| mixed.json | pysimdjson | 0.302 | 0.309 | 0.327 | 86.566 | 0.63x |
| mixed.json | json | 0.476 | 0.493 | 0.495 | 86.566 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.325 | 2.364 | 2.481 | 64.906 | 1.00x |
| users.json | orjson | 2.906 | 3.012 | 3.137 | 64.906 | 0.78x |
| users.json | msgspec | 3.877 | 3.938 | 4.063 | 64.906 | 0.60x |
| users.json | ujson | 11.604 | 11.719 | 12.072 | 64.906 | 0.20x |
| users.json | json | 21.618 | 21.827 | 22.153 | 64.906 | 0.11x |
| flat.json | strata | 0.283 | 0.292 | 0.314 | 82.566 | 1.00x |
| flat.json | orjson | 0.325 | 0.338 | 0.404 | 82.566 | 0.87x |
| flat.json | msgspec | 0.427 | 0.441 | 0.477 | 82.566 | 0.66x |
| flat.json | ujson | 1.019 | 1.046 | 1.146 | 82.566 | 0.28x |
| flat.json | json | 1.847 | 1.892 | 2.000 | 82.566 | 0.15x |
| nested.json | strata | 0.225 | 0.229 | 0.241 | 82.566 | 1.00x |
| nested.json | orjson | 0.287 | 0.294 | 0.303 | 82.566 | 0.78x |
| nested.json | msgspec | 0.401 | 0.410 | 0.421 | 82.566 | 0.56x |
| nested.json | ujson | 1.105 | 1.148 | 1.219 | 82.566 | 0.20x |
| nested.json | json | 2.384 | 2.406 | 2.461 | 82.566 | 0.10x |
| wide_arrays.json | strata | 1.624 | 1.649 | 1.689 | 86.566 | 1.00x |
| wide_arrays.json | orjson | 1.822 | 1.844 | 1.932 | 86.566 | 0.89x |
| wide_arrays.json | msgspec | 2.762 | 2.794 | 2.829 | 86.566 | 0.59x |
| wide_arrays.json | ujson | 6.366 | 6.552 | 7.695 | 86.566 | 0.25x |
| wide_arrays.json | json | 16.451 | 16.930 | 17.682 | 86.566 | 0.10x |
| mixed.json | strata | 0.060 | 0.061 | 0.062 | 86.566 | 1.00x |
| mixed.json | orjson | 0.065 | 0.065 | 0.067 | 86.566 | 0.94x |
| mixed.json | msgspec | 0.083 | 0.087 | 0.101 | 86.566 | 0.70x |
| mixed.json | ujson | 0.235 | 0.245 | 0.256 | 86.566 | 0.25x |
| mixed.json | json | 0.510 | 0.522 | 0.541 | 86.566 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.583 | 12.882 | 14.890 | 81.516 | 1.00x |
| users.json | orjson | 14.606 | 15.512 | 16.827 | 81.516 | 0.83x |
| users.json | msgspec | 14.151 | 15.311 | 16.444 | 81.516 | 0.84x |
| users.json | ujson | 20.959 | 22.428 | 25.538 | 81.516 | 0.57x |
| users.json | json | 23.859 | 25.060 | 27.263 | 81.516 | 0.51x |
| flat.json | strata | 0.884 | 0.899 | 0.952 | 82.566 | 1.00x |
| flat.json | orjson | 1.052 | 1.062 | 1.304 | 82.566 | 0.85x |
| flat.json | msgspec | 1.074 | 1.114 | 1.147 | 82.566 | 0.81x |
| flat.json | ujson | 1.614 | 1.705 | 1.777 | 82.566 | 0.53x |
| flat.json | json | 1.962 | 1.976 | 2.007 | 82.566 | 0.46x |
| nested.json | strata | 0.814 | 0.833 | 0.965 | 82.566 | 1.00x |
| nested.json | orjson | 1.066 | 1.085 | 1.353 | 82.566 | 0.77x |
| nested.json | msgspec | 1.068 | 1.104 | 1.184 | 82.566 | 0.75x |
| nested.json | ujson | 1.547 | 1.594 | 1.638 | 82.566 | 0.52x |
| nested.json | json | 2.102 | 2.127 | 2.164 | 82.566 | 0.39x |
| wide_arrays.json | strata | 4.281 | 4.344 | 4.474 | 86.566 | 1.00x |
| wide_arrays.json | orjson | 5.359 | 5.470 | 5.673 | 86.566 | 0.79x |
| wide_arrays.json | msgspec | 5.923 | 5.988 | 6.181 | 86.566 | 0.73x |
| wide_arrays.json | ujson | 7.444 | 7.522 | 7.775 | 86.566 | 0.58x |
| wide_arrays.json | json | 10.014 | 10.090 | 10.358 | 86.566 | 0.43x |
| mixed.json | strata | 0.208 | 0.213 | 0.224 | 86.566 | 1.00x |
| mixed.json | orjson | 0.285 | 0.293 | 0.341 | 86.566 | 0.73x |
| mixed.json | msgspec | 0.289 | 0.299 | 0.324 | 86.566 | 0.71x |
| mixed.json | ujson | 0.369 | 0.377 | 0.431 | 86.566 | 0.57x |
| mixed.json | json | 0.524 | 0.537 | 0.556 | 86.566 | 0.40x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 12.400 | 13.030 | 13.917 | 82.566 | 1.00x |
| users.ndjson | orjson | 18.710 | 19.206 | 19.744 | 82.566 | 0.68x |
| users.ndjson | msgspec | 18.341 | 19.206 | 20.430 | 82.566 | 0.68x |
| users.ndjson | ujson | 24.660 | 25.995 | 26.876 | 82.566 | 0.50x |
| users.ndjson | json | 32.220 | 33.013 | 35.113 | 82.566 | 0.39x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.063 | 3.139 | 3.652 | 81.516 | 1.00x |
| users.json | orjson | 3.847 | 4.021 | 4.225 | 81.516 | 0.78x |
| users.json | msgspec | 4.761 | 4.871 | 5.074 | 81.516 | 0.64x |
| users.json | ujson | 12.939 | 13.156 | 13.379 | 81.516 | 0.24x |
| users.json | json | 22.564 | 23.132 | 23.449 | 81.516 | 0.14x |
| flat.json | strata | 0.420 | 0.446 | 0.455 | 82.566 | 1.00x |
| flat.json | orjson | 0.484 | 0.502 | 0.558 | 82.566 | 0.89x |
| flat.json | msgspec | 0.585 | 0.599 | 0.619 | 82.566 | 0.74x |
| flat.json | ujson | 1.197 | 1.221 | 1.309 | 82.566 | 0.37x |
| flat.json | json | 2.038 | 2.058 | 2.155 | 82.566 | 0.22x |
| nested.json | strata | 0.356 | 0.367 | 0.384 | 82.566 | 1.00x |
| nested.json | orjson | 0.431 | 0.452 | 0.481 | 82.566 | 0.81x |
| nested.json | msgspec | 0.541 | 0.560 | 0.592 | 82.566 | 0.65x |
| nested.json | ujson | 1.241 | 1.270 | 1.498 | 82.566 | 0.29x |
| nested.json | json | 2.535 | 2.593 | 2.617 | 82.566 | 0.14x |
| wide_arrays.json | strata | 2.069 | 2.099 | 2.198 | 86.566 | 1.00x |
| wide_arrays.json | orjson | 2.271 | 2.332 | 2.500 | 86.566 | 0.90x |
| wide_arrays.json | msgspec | 3.229 | 3.261 | 3.828 | 86.566 | 0.64x |
| wide_arrays.json | ujson | 6.921 | 7.031 | 7.241 | 86.566 | 0.30x |
| wide_arrays.json | json | 17.086 | 17.288 | 17.835 | 86.566 | 0.12x |
| mixed.json | strata | 0.149 | 0.152 | 0.185 | 86.566 | 1.00x |
| mixed.json | orjson | 0.167 | 0.175 | 0.205 | 86.566 | 0.87x |
| mixed.json | msgspec | 0.194 | 0.207 | 0.231 | 86.566 | 0.73x |
| mixed.json | ujson | 0.350 | 0.356 | 0.383 | 86.566 | 0.43x |
| mixed.json | json | 0.623 | 0.635 | 0.682 | 86.566 | 0.24x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.074 | 0.086 | 0.111 | 81.516 | 1.00x |
| users.json $[*].id | jmespath | 0.531 | 0.575 | 0.737 | 81.516 | 0.15x |
| users.json $[*].id | jsonpath-ng | 3.070 | 3.366 | 3.709 | 81.516 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.464 | 0.474 | 0.663 | 81.516 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.174 | 3.250 | 3.585 | 81.516 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 21.342 | 21.876 | 24.432 | 81.516 | 0.02x |
| users.json $..total | strata | 1.645 | 2.080 | 2.409 | 81.574 | 1.00x |
| users.json $..total | jsonpath-ng | 398.220 | 400.235 | 420.506 | 81.574 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.351 | 3.456 | 3.595 | 81.516 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.983 | 18.126 | 22.468 | 81.516 | 0.19x |
| users.json $[*].id | orjson+jsonpath-ng | 18.545 | 21.974 | 25.188 | 81.516 | 0.16x |
| users.json $[*].orders[*].total | strata | 3.623 | 3.721 | 3.910 | 81.516 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 19.584 | 20.687 | 22.163 | 81.516 | 0.18x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 43.058 | 44.600 | 46.631 | 81.516 | 0.08x |
| users.json $..total | strata | 14.375 | 16.652 | 17.894 | 84.203 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 417.646 | 421.165 | 425.925 | 84.203 | 0.04x |

