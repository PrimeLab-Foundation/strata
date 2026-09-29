# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 38eaa9f248f509d9a4beb75b267189d011c1109e
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: AMD EPYC 7763 64-Core Processor
- compiler_flags: -fno-strict-overflow -Wsign-compare -DNDEBUG -g -O3 -Wall -fPIC -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -fomit-frame-pointer -march=native -flto=thin -fprofile-use=/home/runner/work/_temp/strata-arm/build/pgo/strata.profdata -fsplit-machine-functions -shared -Wl,--rpath=/opt/hostedtoolcache/Python/3.12.14/x64/lib (19 recorded commands; see the JSON companion)
- repeats: 60
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.932 | 11.409 | 13.356 | 64.961 | 1.00x |
| users.json | orjson | 13.384 | 14.906 | 17.017 | 64.961 | 0.77x |
| users.json | msgspec | 13.854 | 14.986 | 17.394 | 64.961 | 0.76x |
| users.json | ujson | 18.142 | 21.617 | 24.365 | 64.961 | 0.53x |
| users.json | pysimdjson | 18.798 | 21.580 | 23.974 | 64.961 | 0.53x |
| users.json | json | 22.831 | 24.793 | 26.110 | 64.961 | 0.46x |
| flat.json | strata | 0.825 | 0.870 | 0.953 | 83.211 | 1.00x |
| flat.json | orjson | 0.973 | 1.008 | 1.103 | 83.211 | 0.86x |
| flat.json | msgspec | 1.020 | 1.070 | 1.118 | 83.211 | 0.81x |
| flat.json | ujson | 1.465 | 1.669 | 1.880 | 83.211 | 0.52x |
| flat.json | pysimdjson | 1.534 | 1.653 | 1.891 | 83.211 | 0.53x |
| flat.json | json | 1.841 | 1.884 | 1.957 | 83.211 | 0.46x |
| nested.json | strata | 0.795 | 0.812 | 0.869 | 83.211 | 1.00x |
| nested.json | orjson | 0.989 | 1.016 | 1.089 | 83.211 | 0.80x |
| nested.json | msgspec | 1.010 | 1.032 | 1.086 | 83.211 | 0.79x |
| nested.json | ujson | 1.423 | 1.520 | 1.880 | 83.211 | 0.53x |
| nested.json | pysimdjson | 1.379 | 1.417 | 1.506 | 83.211 | 0.57x |
| nested.json | json | 2.010 | 2.052 | 2.177 | 83.211 | 0.40x |
| wide_arrays.json | strata | 4.052 | 4.449 | 5.642 | 85.215 | 1.00x |
| wide_arrays.json | orjson | 5.018 | 5.868 | 7.010 | 85.215 | 0.76x |
| wide_arrays.json | msgspec | 5.645 | 6.181 | 7.358 | 85.215 | 0.72x |
| wide_arrays.json | ujson | 6.997 | 7.687 | 9.082 | 85.215 | 0.58x |
| wide_arrays.json | pysimdjson | 5.950 | 6.793 | 8.065 | 85.215 | 0.65x |
| wide_arrays.json | json | 9.634 | 10.567 | 12.042 | 85.215 | 0.42x |
| mixed.json | strata | 0.190 | 0.196 | 0.216 | 85.215 | 1.00x |
| mixed.json | orjson | 0.228 | 0.238 | 0.253 | 85.215 | 0.82x |
| mixed.json | msgspec | 0.243 | 0.254 | 0.278 | 85.215 | 0.77x |
| mixed.json | ujson | 0.300 | 0.326 | 0.364 | 85.215 | 0.60x |
| mixed.json | pysimdjson | 0.294 | 0.310 | 0.330 | 85.215 | 0.63x |
| mixed.json | json | 0.477 | 0.495 | 0.520 | 85.215 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.429 | 2.599 | 2.879 | 64.094 | 1.00x |
| users.json | orjson | 2.988 | 3.099 | 3.515 | 64.094 | 0.84x |
| users.json | msgspec | 3.949 | 4.105 | 4.526 | 64.094 | 0.63x |
| users.json | ujson | 11.520 | 12.002 | 12.746 | 64.094 | 0.22x |
| users.json | json | 22.671 | 23.419 | 23.883 | 64.094 | 0.11x |
| flat.json | strata | 0.273 | 0.291 | 0.322 | 83.211 | 1.00x |
| flat.json | orjson | 0.329 | 0.345 | 0.361 | 83.211 | 0.84x |
| flat.json | msgspec | 0.429 | 0.450 | 0.495 | 83.211 | 0.65x |
| flat.json | ujson | 1.006 | 1.044 | 1.109 | 83.211 | 0.28x |
| flat.json | json | 1.865 | 1.896 | 1.948 | 83.211 | 0.15x |
| nested.json | strata | 0.244 | 0.251 | 0.267 | 83.215 | 1.00x |
| nested.json | orjson | 0.295 | 0.303 | 0.324 | 83.215 | 0.83x |
| nested.json | msgspec | 0.407 | 0.419 | 0.438 | 83.215 | 0.60x |
| nested.json | ujson | 1.069 | 1.083 | 1.098 | 83.215 | 0.23x |
| nested.json | json | 2.450 | 2.473 | 2.508 | 83.215 | 0.10x |
| wide_arrays.json | strata | 1.646 | 1.709 | 2.456 | 85.215 | 1.00x |
| wide_arrays.json | orjson | 1.844 | 1.905 | 2.133 | 85.215 | 0.90x |
| wide_arrays.json | msgspec | 2.751 | 2.843 | 3.081 | 85.215 | 0.60x |
| wide_arrays.json | ujson | 6.370 | 6.564 | 7.229 | 85.215 | 0.26x |
| wide_arrays.json | json | 16.503 | 17.179 | 18.017 | 85.215 | 0.10x |
| mixed.json | strata | 0.059 | 0.062 | 0.073 | 85.215 | 1.00x |
| mixed.json | orjson | 0.063 | 0.066 | 0.079 | 85.215 | 0.94x |
| mixed.json | msgspec | 0.083 | 0.088 | 0.103 | 85.215 | 0.71x |
| mixed.json | ujson | 0.228 | 0.234 | 0.248 | 85.215 | 0.27x |
| mixed.json | json | 0.511 | 0.529 | 0.555 | 85.215 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.893 | 12.877 | 13.970 | 81.250 | 1.00x |
| users.json | orjson | 13.677 | 15.793 | 17.367 | 81.250 | 0.82x |
| users.json | msgspec | 14.308 | 16.101 | 18.070 | 81.250 | 0.80x |
| users.json | ujson | 20.514 | 23.136 | 24.707 | 81.250 | 0.56x |
| users.json | json | 23.320 | 25.694 | 26.621 | 81.250 | 0.50x |
| flat.json | strata | 0.851 | 0.933 | 1.083 | 83.211 | 1.00x |
| flat.json | orjson | 1.032 | 1.120 | 1.234 | 83.211 | 0.83x |
| flat.json | msgspec | 1.088 | 1.180 | 1.314 | 83.211 | 0.79x |
| flat.json | ujson | 1.549 | 1.915 | 2.137 | 83.211 | 0.49x |
| flat.json | json | 1.916 | 1.990 | 2.129 | 83.211 | 0.47x |
| nested.json | strata | 0.830 | 0.872 | 1.050 | 83.215 | 1.00x |
| nested.json | orjson | 1.055 | 1.124 | 1.352 | 83.215 | 0.78x |
| nested.json | msgspec | 1.082 | 1.124 | 1.267 | 83.215 | 0.78x |
| nested.json | ujson | 1.548 | 1.625 | 1.840 | 83.215 | 0.54x |
| nested.json | json | 2.088 | 2.154 | 2.364 | 83.215 | 0.41x |
| wide_arrays.json | strata | 4.268 | 4.772 | 5.227 | 85.215 | 1.00x |
| wide_arrays.json | orjson | 5.404 | 6.008 | 6.599 | 85.215 | 0.79x |
| wide_arrays.json | msgspec | 5.970 | 6.516 | 7.450 | 85.215 | 0.73x |
| wide_arrays.json | ujson | 7.305 | 8.265 | 8.961 | 85.215 | 0.58x |
| wide_arrays.json | json | 10.008 | 10.781 | 11.400 | 85.215 | 0.44x |
| mixed.json | strata | 0.212 | 0.231 | 0.263 | 85.215 | 1.00x |
| mixed.json | orjson | 0.281 | 0.321 | 0.365 | 85.215 | 0.72x |
| mixed.json | msgspec | 0.288 | 0.317 | 0.394 | 85.215 | 0.73x |
| mixed.json | ujson | 0.367 | 0.426 | 0.492 | 85.215 | 0.54x |
| mixed.json | json | 0.520 | 0.556 | 0.613 | 85.215 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.480 | 12.815 | 14.569 | 83.211 | 1.00x |
| users.ndjson | orjson | 16.610 | 18.826 | 21.080 | 83.211 | 0.68x |
| users.ndjson | msgspec | 17.099 | 18.971 | 21.183 | 83.211 | 0.68x |
| users.ndjson | ujson | 21.789 | 24.553 | 28.084 | 83.211 | 0.52x |
| users.ndjson | json | 29.192 | 32.657 | 34.739 | 83.211 | 0.39x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.158 | 3.366 | 3.691 | 81.250 | 1.00x |
| users.json | orjson | 3.798 | 4.026 | 4.699 | 81.250 | 0.84x |
| users.json | msgspec | 4.694 | 5.017 | 5.815 | 81.250 | 0.67x |
| users.json | ujson | 12.566 | 13.214 | 14.199 | 81.250 | 0.25x |
| users.json | json | 23.766 | 24.719 | 25.234 | 81.250 | 0.14x |
| flat.json | strata | 0.481 | 0.523 | 0.580 | 83.211 | 1.00x |
| flat.json | orjson | 0.574 | 0.613 | 0.826 | 83.211 | 0.85x |
| flat.json | msgspec | 0.671 | 0.716 | 0.896 | 83.211 | 0.73x |
| flat.json | ujson | 1.265 | 1.311 | 1.402 | 83.211 | 0.40x |
| flat.json | json | 2.160 | 2.206 | 2.325 | 83.211 | 0.24x |
| nested.json | strata | 0.393 | 0.420 | 0.471 | 83.215 | 1.00x |
| nested.json | orjson | 0.486 | 0.518 | 0.564 | 83.215 | 0.81x |
| nested.json | msgspec | 0.594 | 0.627 | 0.677 | 83.215 | 0.67x |
| nested.json | ujson | 1.296 | 1.327 | 1.359 | 83.215 | 0.32x |
| nested.json | json | 2.671 | 2.708 | 2.781 | 83.215 | 0.16x |
| wide_arrays.json | strata | 2.233 | 2.360 | 2.874 | 85.215 | 1.00x |
| wide_arrays.json | orjson | 2.439 | 2.552 | 2.952 | 85.215 | 0.92x |
| wide_arrays.json | msgspec | 3.299 | 3.467 | 3.979 | 85.215 | 0.68x |
| wide_arrays.json | ujson | 7.145 | 7.417 | 8.346 | 85.215 | 0.32x |
| wide_arrays.json | json | 17.466 | 18.093 | 18.737 | 85.215 | 0.13x |
| mixed.json | strata | 0.207 | 0.221 | 0.287 | 85.215 | 1.00x |
| mixed.json | orjson | 0.233 | 0.250 | 0.307 | 85.215 | 0.88x |
| mixed.json | msgspec | 0.250 | 0.269 | 0.317 | 85.215 | 0.82x |
| mixed.json | ujson | 0.410 | 0.443 | 0.481 | 85.215 | 0.50x |
| mixed.json | json | 0.699 | 0.739 | 0.779 | 85.215 | 0.30x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.067 | 0.074 | 0.085 | 81.250 | 1.00x |
| users.json $[*].id | jmespath | 0.489 | 0.512 | 0.566 | 81.250 | 0.14x |
| users.json $[*].id | jsonpath-ng | 3.027 | 3.216 | 3.396 | 81.250 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.448 | 0.477 | 0.544 | 81.262 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.092 | 3.212 | 3.462 | 81.262 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 20.524 | 22.739 | 25.300 | 81.262 | 0.02x |
| users.json $..total | strata | 1.678 | 1.825 | 2.260 | 84.480 | 1.00x |
| users.json $..total | jsonpath-ng | 386.774 | 393.267 | 396.541 | 84.480 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.288 | 3.348 | 3.382 | 81.262 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.193 | 16.646 | 18.430 | 81.262 | 0.20x |
| users.json $[*].id | orjson+jsonpath-ng | 17.692 | 19.377 | 21.516 | 81.262 | 0.17x |
| users.json $[*].orders[*].total | strata | 3.588 | 3.641 | 3.703 | 81.285 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.436 | 21.089 | 23.384 | 81.285 | 0.17x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 38.562 | 46.206 | 50.018 | 81.285 | 0.08x |
| users.json $..total | strata | 14.184 | 18.809 | 22.731 | 84.852 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 415.611 | 422.855 | 429.156 | 84.852 | 0.04x |

