# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c16eaa65c61f1cf9d4fac46e9f6d8e6b9f3b087f
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
| users.json | strata | 8.301 | 8.778 | 11.304 | 66.953 | 1.00x |
| users.json | orjson | 10.229 | 11.792 | 14.099 | 66.953 | 0.74x |
| users.json | msgspec | 10.740 | 11.137 | 13.413 | 66.953 | 0.79x |
| users.json | ujson | 16.875 | 18.657 | 22.162 | 66.953 | 0.47x |
| users.json | pysimdjson | 17.627 | 18.904 | 20.578 | 66.953 | 0.46x |
| users.json | json | 16.690 | 19.001 | 20.972 | 66.953 | 0.46x |
| flat.json | strata | 0.781 | 0.923 | 1.007 | 81.426 | 1.00x |
| flat.json | orjson | 0.864 | 0.976 | 1.065 | 81.426 | 0.95x |
| flat.json | msgspec | 0.841 | 0.924 | 1.019 | 81.426 | 1.00x |
| flat.json | ujson | 1.477 | 1.660 | 1.723 | 81.426 | 0.56x |
| flat.json | pysimdjson | 1.297 | 1.719 | 1.841 | 81.426 | 0.54x |
| flat.json | json | 1.341 | 1.512 | 1.727 | 81.426 | 0.61x |
| nested.json | strata | 0.583 | 0.880 | 0.996 | 81.426 | 1.00x |
| nested.json | orjson | 0.672 | 0.969 | 1.136 | 81.426 | 0.91x |
| nested.json | msgspec | 0.677 | 0.968 | 1.085 | 81.426 | 0.91x |
| nested.json | ujson | 1.224 | 1.485 | 1.558 | 81.426 | 0.59x |
| nested.json | pysimdjson | 1.126 | 1.421 | 1.556 | 81.426 | 0.62x |
| nested.json | json | 1.537 | 1.793 | 1.962 | 81.426 | 0.49x |
| wide_arrays.json | strata | 3.649 | 4.721 | 5.811 | 85.961 | 1.00x |
| wide_arrays.json | orjson | 4.353 | 6.051 | 6.618 | 85.961 | 0.78x |
| wide_arrays.json | msgspec | 4.846 | 5.819 | 6.916 | 85.961 | 0.81x |
| wide_arrays.json | ujson | 5.235 | 6.986 | 7.969 | 85.961 | 0.68x |
| wide_arrays.json | pysimdjson | 4.282 | 6.677 | 7.255 | 85.961 | 0.71x |
| wide_arrays.json | json | 11.130 | 12.004 | 12.630 | 85.961 | 0.39x |
| mixed.json | strata | 0.130 | 0.141 | 0.206 | 85.961 | 1.00x |
| mixed.json | orjson | 0.155 | 0.163 | 0.213 | 85.961 | 0.86x |
| mixed.json | msgspec | 0.162 | 0.175 | 0.211 | 85.961 | 0.80x |
| mixed.json | ujson | 0.243 | 0.256 | 0.276 | 85.961 | 0.55x |
| mixed.json | pysimdjson | 0.214 | 0.231 | 0.253 | 85.961 | 0.61x |
| mixed.json | json | 0.302 | 0.326 | 0.361 | 85.961 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.065 | 2.308 | 2.476 | 65.336 | 1.00x |
| users.json | orjson | 2.079 | 2.293 | 2.475 | 65.336 | 1.01x |
| users.json | msgspec | 3.157 | 3.349 | 3.503 | 65.336 | 0.69x |
| users.json | ujson | 8.082 | 8.226 | 8.568 | 65.336 | 0.28x |
| users.json | json | 12.605 | 12.866 | 13.827 | 65.336 | 0.18x |
| flat.json | strata | 0.276 | 0.349 | 0.432 | 81.426 | 1.00x |
| flat.json | orjson | 0.243 | 0.316 | 0.415 | 81.426 | 1.11x |
| flat.json | msgspec | 0.375 | 0.447 | 0.552 | 81.426 | 0.78x |
| flat.json | ujson | 0.687 | 0.773 | 0.846 | 81.426 | 0.45x |
| flat.json | json | 1.168 | 1.277 | 1.342 | 81.426 | 0.27x |
| nested.json | strata | 0.140 | 0.156 | 0.183 | 81.426 | 1.00x |
| nested.json | orjson | 0.161 | 0.167 | 0.199 | 81.426 | 0.93x |
| nested.json | msgspec | 0.270 | 0.283 | 0.299 | 81.426 | 0.55x |
| nested.json | ujson | 0.700 | 0.713 | 0.732 | 81.426 | 0.22x |
| nested.json | json | 1.333 | 1.352 | 1.403 | 81.426 | 0.12x |
| wide_arrays.json | strata | 1.340 | 1.527 | 1.887 | 85.961 | 1.00x |
| wide_arrays.json | orjson | 1.108 | 1.161 | 1.362 | 85.961 | 1.31x |
| wide_arrays.json | msgspec | 1.906 | 2.174 | 2.397 | 85.961 | 0.70x |
| wide_arrays.json | ujson | 3.867 | 4.173 | 4.807 | 85.961 | 0.37x |
| wide_arrays.json | json | 10.728 | 11.281 | 20.741 | 85.961 | 0.14x |
| mixed.json | strata | 0.040 | 0.043 | 0.051 | 85.961 | 1.00x |
| mixed.json | orjson | 0.035 | 0.037 | 0.039 | 85.961 | 1.18x |
| mixed.json | msgspec | 0.057 | 0.057 | 0.059 | 85.961 | 0.76x |
| mixed.json | ujson | 0.154 | 0.159 | 0.167 | 85.961 | 0.27x |
| mixed.json | json | 0.306 | 0.310 | 0.317 | 85.961 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.450 | 8.995 | 10.247 | 83.062 | 1.00x |
| users.json | orjson | 11.461 | 11.941 | 13.355 | 83.062 | 0.75x |
| users.json | msgspec | 9.586 | 11.927 | 13.311 | 83.062 | 0.75x |
| users.json | ujson | 17.752 | 18.967 | 21.284 | 83.062 | 0.47x |
| users.json | json | 17.044 | 18.930 | 19.623 | 83.062 | 0.48x |
| flat.json | strata | 0.855 | 1.045 | 2.562 | 81.426 | 1.00x |
| flat.json | orjson | 1.070 | 1.185 | 3.514 | 81.426 | 0.88x |
| flat.json | msgspec | 1.036 | 1.115 | 2.480 | 81.426 | 0.94x |
| flat.json | ujson | 1.709 | 1.767 | 3.362 | 81.426 | 0.59x |
| flat.json | json | 1.504 | 1.672 | 4.534 | 81.426 | 0.62x |
| nested.json | strata | 0.940 | 0.983 | 1.090 | 81.426 | 1.00x |
| nested.json | orjson | 1.066 | 1.176 | 1.196 | 81.426 | 0.84x |
| nested.json | msgspec | 0.987 | 1.174 | 1.263 | 81.426 | 0.84x |
| nested.json | ujson | 1.224 | 1.503 | 1.655 | 81.426 | 0.65x |
| nested.json | json | 1.696 | 1.963 | 2.026 | 81.426 | 0.50x |
| wide_arrays.json | strata | 3.415 | 3.471 | 3.613 | 85.961 | 1.00x |
| wide_arrays.json | orjson | 4.378 | 4.581 | 4.924 | 85.961 | 0.76x |
| wide_arrays.json | msgspec | 4.758 | 4.875 | 5.282 | 85.961 | 0.71x |
| wide_arrays.json | ujson | 5.611 | 5.816 | 11.623 | 85.961 | 0.60x |
| wide_arrays.json | json | 10.385 | 10.643 | 10.928 | 85.961 | 0.33x |
| mixed.json | strata | 0.159 | 0.178 | 0.236 | 85.961 | 1.00x |
| mixed.json | orjson | 0.220 | 0.248 | 0.311 | 85.961 | 0.72x |
| mixed.json | msgspec | 0.218 | 0.242 | 0.288 | 85.961 | 0.73x |
| mixed.json | ujson | 0.319 | 0.345 | 0.385 | 85.961 | 0.52x |
| mixed.json | json | 0.363 | 0.388 | 0.417 | 85.961 | 0.46x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 11.825 | 12.053 | 12.365 | 81.426 | 1.00x |
| users.ndjson | orjson | 16.524 | 16.721 | 17.541 | 81.426 | 0.72x |
| users.ndjson | msgspec | 16.397 | 16.932 | 17.256 | 81.426 | 0.71x |
| users.ndjson | ujson | 21.740 | 22.416 | 23.390 | 81.426 | 0.54x |
| users.ndjson | json | 25.814 | 26.566 | 26.890 | 81.426 | 0.45x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.225 | 2.502 | 2.871 | 83.062 | 1.00x |
| users.json | orjson | 2.427 | 2.711 | 3.071 | 83.062 | 0.92x |
| users.json | msgspec | 3.399 | 4.023 | 4.157 | 83.062 | 0.62x |
| users.json | ujson | 8.700 | 8.974 | 9.620 | 83.062 | 0.28x |
| users.json | json | 13.068 | 13.559 | 13.766 | 83.062 | 0.18x |
| flat.json | strata | 0.658 | 0.703 | 2.478 | 81.426 | 1.00x |
| flat.json | orjson | 0.661 | 0.777 | 2.783 | 81.426 | 0.91x |
| flat.json | msgspec | 0.805 | 0.926 | 1.055 | 81.426 | 0.76x |
| flat.json | ujson | 1.150 | 1.248 | 1.402 | 81.426 | 0.56x |
| flat.json | json | 1.637 | 1.708 | 3.311 | 81.426 | 0.41x |
| nested.json | strata | 0.218 | 0.236 | 0.323 | 81.426 | 1.00x |
| nested.json | orjson | 0.251 | 0.270 | 0.377 | 81.426 | 0.87x |
| nested.json | msgspec | 0.354 | 0.377 | 0.441 | 81.426 | 0.63x |
| nested.json | ujson | 0.760 | 0.801 | 0.850 | 81.426 | 0.29x |
| nested.json | json | 1.400 | 1.424 | 1.510 | 81.426 | 0.17x |
| wide_arrays.json | strata | 1.963 | 2.210 | 2.463 | 85.961 | 1.00x |
| wide_arrays.json | orjson | 1.813 | 2.071 | 2.214 | 85.961 | 1.07x |
| wide_arrays.json | msgspec | 2.634 | 2.912 | 3.389 | 85.961 | 0.76x |
| wide_arrays.json | ujson | 4.595 | 4.935 | 5.524 | 85.961 | 0.45x |
| wide_arrays.json | json | 11.564 | 11.762 | 12.212 | 85.961 | 0.19x |
| mixed.json | strata | 0.141 | 0.149 | 0.168 | 85.961 | 1.00x |
| mixed.json | orjson | 0.156 | 0.163 | 0.184 | 85.961 | 0.91x |
| mixed.json | msgspec | 0.169 | 0.182 | 0.196 | 85.961 | 0.82x |
| mixed.json | ujson | 0.281 | 0.297 | 0.317 | 85.961 | 0.50x |
| mixed.json | json | 0.438 | 0.452 | 0.478 | 85.961 | 0.33x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.055 | 0.095 | 0.137 | 83.062 | 1.00x |
| users.json $[*].id | jmespath | 0.297 | 0.391 | 0.462 | 83.062 | 0.24x |
| users.json $[*].id | jsonpath-ng | 1.905 | 2.096 | 2.279 | 83.062 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.258 | 0.268 | 1.090 | 83.062 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.718 | 1.738 | 3.095 | 83.062 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 14.677 | 14.982 | 15.381 | 83.062 | 0.02x |
| users.json $..total | strata | 1.316 | 1.981 | 2.208 | 83.062 | 1.00x |
| users.json $..total | jsonpath-ng | 225.624 | 228.771 | 234.366 | 83.062 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 2.173 | 2.197 | 2.322 | 83.062 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.179 | 15.340 | 18.018 | 83.062 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 16.058 | 18.057 | 18.592 | 83.062 | 0.12x |
| users.json $[*].orders[*].total | strata | 2.336 | 2.366 | 2.454 | 83.062 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 16.327 | 17.994 | 19.743 | 83.062 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 32.833 | 36.061 | 37.803 | 83.062 | 0.07x |
| users.json $..total | strata | 14.123 | 18.192 | 20.247 | 83.062 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 252.760 | 260.294 | 267.800 | 83.062 | 0.07x |

