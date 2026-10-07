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
| users.json | strata | 5.304 | 5.393 | 8.288 | 63.445 | 1.00x |
| users.json | orjson | 7.900 | 8.186 | 10.389 | 63.445 | 0.66x |
| users.json | msgspec | 7.586 | 7.735 | 9.841 | 63.445 | 0.70x |
| users.json | ujson | 9.861 | 10.379 | 14.801 | 63.445 | 0.52x |
| users.json | pysimdjson | 10.341 | 11.735 | 14.350 | 63.445 | 0.46x |
| users.json | json | 13.817 | 14.208 | 15.153 | 63.445 | 0.38x |
| flat.json | strata | 0.514 | 0.520 | 0.525 | 77.113 | 1.00x |
| flat.json | orjson | 0.652 | 0.660 | 0.663 | 77.113 | 0.79x |
| flat.json | msgspec | 0.602 | 0.608 | 0.613 | 77.113 | 0.85x |
| flat.json | ujson | 0.848 | 0.857 | 0.904 | 77.113 | 0.61x |
| flat.json | pysimdjson | 0.978 | 0.982 | 0.998 | 77.113 | 0.53x |
| flat.json | json | 1.244 | 1.254 | 1.264 | 77.113 | 0.41x |
| nested.json | strata | 0.423 | 0.427 | 0.434 | 77.113 | 1.00x |
| nested.json | orjson | 0.545 | 0.552 | 0.559 | 77.113 | 0.77x |
| nested.json | msgspec | 0.520 | 0.524 | 0.533 | 77.113 | 0.81x |
| nested.json | ujson | 0.757 | 0.762 | 0.771 | 77.113 | 0.56x |
| nested.json | pysimdjson | 0.766 | 0.774 | 0.785 | 77.113 | 0.55x |
| nested.json | json | 1.306 | 1.317 | 1.356 | 77.113 | 0.32x |
| wide_arrays.json | strata | 2.478 | 2.500 | 2.616 | 81.371 | 1.00x |
| wide_arrays.json | orjson | 3.283 | 3.339 | 3.495 | 81.371 | 0.75x |
| wide_arrays.json | msgspec | 3.643 | 3.681 | 3.859 | 81.371 | 0.68x |
| wide_arrays.json | ujson | 4.313 | 4.358 | 4.511 | 81.371 | 0.57x |
| wide_arrays.json | pysimdjson | 3.650 | 3.689 | 3.830 | 81.371 | 0.68x |
| wide_arrays.json | json | 8.881 | 8.902 | 9.243 | 81.371 | 0.28x |
| mixed.json | strata | 0.100 | 0.101 | 0.109 | 81.371 | 1.00x |
| mixed.json | orjson | 0.127 | 0.127 | 0.129 | 81.371 | 0.79x |
| mixed.json | msgspec | 0.126 | 0.128 | 0.135 | 81.371 | 0.79x |
| mixed.json | ujson | 0.164 | 0.167 | 0.174 | 81.371 | 0.61x |
| mixed.json | pysimdjson | 0.170 | 0.172 | 0.181 | 81.371 | 0.59x |
| mixed.json | json | 0.269 | 0.272 | 0.281 | 81.371 | 0.37x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.263 | 1.278 | 1.343 | 62.004 | 1.00x |
| users.json | orjson | 1.237 | 1.249 | 1.310 | 62.004 | 1.02x |
| users.json | msgspec | 2.307 | 2.327 | 2.476 | 62.004 | 0.55x |
| users.json | ujson | 6.233 | 6.286 | 6.678 | 62.004 | 0.20x |
| users.json | json | 11.399 | 11.464 | 12.272 | 62.004 | 0.11x |
| flat.json | strata | 0.183 | 0.184 | 0.191 | 77.113 | 1.00x |
| flat.json | orjson | 0.174 | 0.175 | 0.186 | 77.113 | 1.05x |
| flat.json | msgspec | 0.281 | 0.284 | 0.291 | 77.113 | 0.65x |
| flat.json | ujson | 0.591 | 0.601 | 0.610 | 77.113 | 0.31x |
| flat.json | json | 1.014 | 1.018 | 1.023 | 77.113 | 0.18x |
| nested.json | strata | 0.115 | 0.115 | 0.123 | 77.113 | 1.00x |
| nested.json | orjson | 0.132 | 0.133 | 0.143 | 77.113 | 0.86x |
| nested.json | msgspec | 0.234 | 0.235 | 0.263 | 77.113 | 0.49x |
| nested.json | ujson | 0.575 | 0.585 | 0.598 | 77.113 | 0.20x |
| nested.json | json | 1.255 | 1.258 | 1.271 | 77.113 | 0.09x |
| wide_arrays.json | strata | 1.047 | 1.050 | 1.098 | 81.371 | 1.00x |
| wide_arrays.json | orjson | 0.973 | 0.981 | 1.024 | 81.371 | 1.07x |
| wide_arrays.json | msgspec | 1.812 | 1.817 | 1.947 | 81.371 | 0.58x |
| wide_arrays.json | ujson | 3.491 | 3.511 | 3.718 | 81.371 | 0.30x |
| wide_arrays.json | json | 9.078 | 9.096 | 9.198 | 81.371 | 0.12x |
| mixed.json | strata | 0.034 | 0.034 | 0.035 | 81.371 | 1.00x |
| mixed.json | orjson | 0.030 | 0.030 | 0.031 | 81.371 | 1.14x |
| mixed.json | msgspec | 0.045 | 0.045 | 0.046 | 81.371 | 0.76x |
| mixed.json | ujson | 0.128 | 0.130 | 0.135 | 81.371 | 0.26x |
| mixed.json | json | 0.270 | 0.272 | 0.279 | 81.371 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 5.685 | 6.114 | 8.343 | 78.750 | 1.00x |
| users.json | orjson | 8.319 | 9.183 | 10.060 | 78.750 | 0.67x |
| users.json | msgspec | 7.995 | 8.628 | 8.774 | 78.750 | 0.71x |
| users.json | ujson | 11.182 | 11.808 | 12.918 | 78.750 | 0.52x |
| users.json | json | 14.393 | 15.551 | 15.803 | 78.750 | 0.39x |
| flat.json | strata | 0.525 | 0.527 | 0.528 | 77.113 | 1.00x |
| flat.json | orjson | 0.676 | 0.677 | 0.681 | 77.113 | 0.78x |
| flat.json | msgspec | 0.626 | 0.631 | 0.636 | 77.113 | 0.84x |
| flat.json | ujson | 0.890 | 0.896 | 0.901 | 77.113 | 0.59x |
| flat.json | json | 1.266 | 1.271 | 1.282 | 77.113 | 0.41x |
| nested.json | strata | 0.434 | 0.442 | 0.451 | 77.113 | 1.00x |
| nested.json | orjson | 0.569 | 0.579 | 0.586 | 77.113 | 0.76x |
| nested.json | msgspec | 0.544 | 0.553 | 0.559 | 77.113 | 0.80x |
| nested.json | ujson | 0.791 | 0.801 | 0.808 | 77.113 | 0.55x |
| nested.json | json | 1.346 | 1.362 | 1.400 | 77.113 | 0.32x |
| wide_arrays.json | strata | 2.498 | 2.527 | 2.547 | 81.371 | 1.00x |
| wide_arrays.json | orjson | 3.298 | 3.392 | 3.484 | 81.371 | 0.74x |
| wide_arrays.json | msgspec | 3.714 | 3.761 | 3.846 | 81.371 | 0.67x |
| wide_arrays.json | ujson | 4.421 | 4.500 | 4.567 | 81.371 | 0.56x |
| wide_arrays.json | json | 8.903 | 8.987 | 9.064 | 81.371 | 0.28x |
| mixed.json | strata | 0.108 | 0.109 | 0.120 | 81.371 | 1.00x |
| mixed.json | orjson | 0.150 | 0.155 | 0.160 | 81.371 | 0.70x |
| mixed.json | msgspec | 0.150 | 0.151 | 0.153 | 81.371 | 0.72x |
| mixed.json | ujson | 0.192 | 0.193 | 0.200 | 81.371 | 0.56x |
| mixed.json | json | 0.290 | 0.292 | 0.296 | 81.371 | 0.37x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 5.673 | 5.735 | 5.799 | 77.113 | 1.00x |
| users.ndjson | orjson | 9.975 | 10.070 | 10.394 | 77.113 | 0.57x |
| users.ndjson | msgspec | 9.849 | 9.893 | 10.119 | 77.113 | 0.58x |
| users.ndjson | ujson | 12.884 | 13.400 | 13.916 | 77.113 | 0.43x |
| users.ndjson | json | 18.293 | 18.500 | 18.783 | 77.113 | 0.31x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.663 | 1.764 | 1.827 | 78.750 | 1.00x |
| users.json | orjson | 1.664 | 1.766 | 1.857 | 78.750 | 1.00x |
| users.json | msgspec | 2.727 | 2.892 | 2.971 | 78.750 | 0.61x |
| users.json | ujson | 6.765 | 7.155 | 7.288 | 78.750 | 0.25x |
| users.json | json | 11.959 | 12.688 | 12.927 | 78.750 | 0.14x |
| flat.json | strata | 0.273 | 0.275 | 0.299 | 77.113 | 1.00x |
| flat.json | orjson | 0.272 | 0.275 | 0.282 | 77.113 | 1.00x |
| flat.json | msgspec | 0.379 | 0.381 | 0.388 | 77.113 | 0.72x |
| flat.json | ujson | 0.690 | 0.713 | 0.730 | 77.113 | 0.39x |
| flat.json | json | 1.121 | 1.137 | 1.185 | 77.113 | 0.24x |
| nested.json | strata | 0.187 | 0.189 | 0.232 | 77.113 | 1.00x |
| nested.json | orjson | 0.218 | 0.219 | 0.230 | 77.113 | 0.86x |
| nested.json | msgspec | 0.318 | 0.320 | 0.329 | 77.113 | 0.59x |
| nested.json | ujson | 0.682 | 0.689 | 0.739 | 77.113 | 0.27x |
| nested.json | json | 1.358 | 1.367 | 1.407 | 77.113 | 0.14x |
| wide_arrays.json | strata | 1.331 | 1.403 | 1.533 | 81.371 | 1.00x |
| wide_arrays.json | orjson | 1.265 | 1.348 | 1.411 | 81.371 | 1.04x |
| wide_arrays.json | msgspec | 2.106 | 2.202 | 2.296 | 81.371 | 0.64x |
| wide_arrays.json | ujson | 3.848 | 3.955 | 4.146 | 81.371 | 0.35x |
| wide_arrays.json | json | 9.724 | 9.812 | 10.251 | 81.371 | 0.14x |
| mixed.json | strata | 0.083 | 0.084 | 0.088 | 81.371 | 1.00x |
| mixed.json | orjson | 0.085 | 0.087 | 0.090 | 81.371 | 0.96x |
| mixed.json | msgspec | 0.100 | 0.101 | 0.109 | 81.371 | 0.83x |
| mixed.json | ujson | 0.187 | 0.193 | 0.203 | 81.371 | 0.44x |
| mixed.json | json | 0.336 | 0.341 | 0.351 | 81.371 | 0.25x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.039 | 0.040 | 0.048 | 78.750 | 1.00x |
| users.json $[*].id | jmespath | 0.253 | 0.256 | 0.272 | 78.750 | 0.15x |
| users.json $[*].id | jsonpath-ng | 1.554 | 1.569 | 1.602 | 78.750 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.240 | 0.244 | 0.258 | 78.750 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.628 | 1.644 | 1.675 | 78.750 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 12.168 | 12.485 | 13.086 | 78.750 | 0.02x |
| users.json $..total | strata | 0.958 | 0.969 | 1.008 | 78.750 | 1.00x |
| users.json $..total | jsonpath-ng | 212.293 | 212.967 | 221.026 | 78.750 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 2.038 | 2.054 | 2.102 | 78.750 | 1.00x |
| users.json $[*].id | orjson+jmespath | 9.204 | 9.354 | 10.869 | 78.750 | 0.22x |
| users.json $[*].id | orjson+jsonpath-ng | 10.377 | 10.537 | 11.451 | 78.750 | 0.19x |
| users.json $[*].orders[*].total | strata | 2.238 | 2.278 | 2.343 | 78.750 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.149 | 12.474 | 13.302 | 78.750 | 0.18x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 24.400 | 25.188 | 28.194 | 78.750 | 0.09x |
| users.json $..total | strata | 8.601 | 10.148 | 11.388 | 78.750 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 229.082 | 231.591 | 243.897 | 78.750 | 0.04x |

