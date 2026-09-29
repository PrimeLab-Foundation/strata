# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 38eaa9f248f509d9a4beb75b267189d011c1109e
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: AMD EPYC 9V45 96-Core Processor
- compiler_flags: -fno-strict-overflow -Wsign-compare -DNDEBUG -g -O3 -Wall -fPIC -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -fomit-frame-pointer -march=native -flto=thin -fprofile-use=/home/runner/work/_temp/strata-arm/build/pgo/strata.profdata -fsplit-machine-functions -shared -Wl,--rpath=/opt/hostedtoolcache/Python/3.12.14/x64/lib (19 recorded commands; see the JSON companion)
- repeats: 60
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.084 | 8.323 | 8.821 | 68.719 | 1.00x |
| users.json | orjson | 9.684 | 11.858 | 12.314 | 68.719 | 0.70x |
| users.json | msgspec | 9.173 | 11.228 | 11.670 | 68.719 | 0.74x |
| users.json | ujson | 13.693 | 17.148 | 18.014 | 68.719 | 0.49x |
| users.json | pysimdjson | 15.279 | 18.166 | 18.679 | 68.719 | 0.46x |
| users.json | json | 16.392 | 18.642 | 19.030 | 68.719 | 0.45x |
| flat.json | strata | 0.576 | 0.615 | 1.094 | 64.574 | 1.00x |
| flat.json | orjson | 0.695 | 0.723 | 1.528 | 64.574 | 0.85x |
| flat.json | msgspec | 0.627 | 0.649 | 1.044 | 64.574 | 0.95x |
| flat.json | ujson | 0.924 | 1.097 | 2.090 | 64.574 | 0.56x |
| flat.json | pysimdjson | 1.031 | 1.100 | 1.780 | 64.574 | 0.56x |
| flat.json | json | 1.307 | 1.334 | 1.889 | 64.574 | 0.46x |
| nested.json | strata | 0.434 | 0.451 | 0.462 | 64.574 | 1.00x |
| nested.json | orjson | 0.565 | 0.586 | 0.606 | 64.574 | 0.77x |
| nested.json | msgspec | 0.538 | 0.557 | 0.577 | 64.574 | 0.81x |
| nested.json | ujson | 0.794 | 0.840 | 0.943 | 64.574 | 0.54x |
| nested.json | pysimdjson | 0.780 | 0.811 | 0.850 | 64.574 | 0.56x |
| nested.json | json | 1.326 | 1.382 | 1.407 | 64.574 | 0.33x |
| wide_arrays.json | strata | 2.662 | 3.012 | 3.669 | 77.004 | 1.00x |
| wide_arrays.json | orjson | 3.622 | 4.487 | 5.165 | 77.004 | 0.67x |
| wide_arrays.json | msgspec | 3.881 | 4.462 | 5.220 | 77.004 | 0.68x |
| wide_arrays.json | ujson | 4.629 | 5.367 | 6.053 | 77.004 | 0.56x |
| wide_arrays.json | pysimdjson | 4.137 | 5.020 | 6.058 | 77.004 | 0.60x |
| wide_arrays.json | json | 9.417 | 10.217 | 11.264 | 77.004 | 0.29x |
| mixed.json | strata | 0.112 | 0.121 | 0.131 | 77.004 | 1.00x |
| mixed.json | orjson | 0.139 | 0.151 | 0.160 | 77.004 | 0.80x |
| mixed.json | msgspec | 0.140 | 0.154 | 0.165 | 77.004 | 0.78x |
| mixed.json | ujson | 0.197 | 0.216 | 0.226 | 77.004 | 0.56x |
| mixed.json | pysimdjson | 0.189 | 0.204 | 0.212 | 77.004 | 0.59x |
| mixed.json | json | 0.294 | 0.306 | 0.319 | 77.004 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.504 | 1.663 | 1.862 | 47.699 | 1.00x |
| users.json | orjson | 1.444 | 1.556 | 1.713 | 47.699 | 1.07x |
| users.json | msgspec | 2.529 | 2.699 | 2.853 | 47.699 | 0.62x |
| users.json | ujson | 6.573 | 7.026 | 7.420 | 47.699 | 0.24x |
| users.json | json | 12.111 | 12.690 | 13.234 | 47.699 | 0.13x |
| flat.json | strata | 0.193 | 0.200 | 0.210 | 64.574 | 1.00x |
| flat.json | orjson | 0.177 | 0.188 | 0.208 | 64.574 | 1.06x |
| flat.json | msgspec | 0.286 | 0.297 | 0.307 | 64.574 | 0.68x |
| flat.json | ujson | 0.604 | 0.622 | 0.636 | 64.574 | 0.32x |
| flat.json | json | 1.023 | 1.057 | 1.082 | 64.574 | 0.19x |
| nested.json | strata | 0.123 | 0.126 | 0.134 | 64.574 | 1.00x |
| nested.json | orjson | 0.138 | 0.141 | 0.148 | 64.574 | 0.90x |
| nested.json | msgspec | 0.244 | 0.249 | 0.262 | 64.574 | 0.51x |
| nested.json | ujson | 0.616 | 0.632 | 0.642 | 64.574 | 0.20x |
| nested.json | json | 1.267 | 1.288 | 1.314 | 64.574 | 0.10x |
| wide_arrays.json | strata | 1.151 | 1.279 | 1.374 | 77.004 | 1.00x |
| wide_arrays.json | orjson | 1.087 | 1.157 | 1.213 | 77.004 | 1.11x |
| wide_arrays.json | msgspec | 1.881 | 1.985 | 2.049 | 77.004 | 0.64x |
| wide_arrays.json | ujson | 3.639 | 3.885 | 3.993 | 77.004 | 0.33x |
| wide_arrays.json | json | 9.611 | 9.901 | 10.141 | 77.004 | 0.13x |
| mixed.json | strata | 0.040 | 0.042 | 0.049 | 77.004 | 1.00x |
| mixed.json | orjson | 0.033 | 0.035 | 0.042 | 77.004 | 1.20x |
| mixed.json | msgspec | 0.051 | 0.053 | 0.067 | 77.004 | 0.79x |
| mixed.json | ujson | 0.136 | 0.140 | 0.207 | 77.004 | 0.30x |
| mixed.json | json | 0.297 | 0.303 | 0.490 | 77.004 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.320 | 8.264 | 10.036 | 66.180 | 1.00x |
| users.json | orjson | 9.517 | 11.261 | 13.579 | 66.180 | 0.73x |
| users.json | msgspec | 9.127 | 10.800 | 12.738 | 66.180 | 0.77x |
| users.json | ujson | 13.961 | 16.031 | 19.020 | 66.180 | 0.52x |
| users.json | json | 15.799 | 17.467 | 19.000 | 66.180 | 0.47x |
| flat.json | strata | 0.574 | 0.600 | 0.656 | 64.574 | 1.00x |
| flat.json | orjson | 0.728 | 0.750 | 0.795 | 64.574 | 0.80x |
| flat.json | msgspec | 0.661 | 0.680 | 0.735 | 64.574 | 0.88x |
| flat.json | ujson | 0.995 | 1.136 | 1.370 | 64.574 | 0.53x |
| flat.json | json | 1.340 | 1.355 | 1.434 | 64.574 | 0.44x |
| nested.json | strata | 0.470 | 0.485 | 0.515 | 64.574 | 1.00x |
| nested.json | orjson | 0.614 | 0.641 | 0.704 | 64.574 | 0.76x |
| nested.json | msgspec | 0.580 | 0.601 | 0.647 | 64.574 | 0.81x |
| nested.json | ujson | 0.841 | 0.905 | 1.048 | 64.574 | 0.54x |
| nested.json | json | 1.395 | 1.442 | 1.514 | 64.574 | 0.34x |
| wide_arrays.json | strata | 3.565 | 3.837 | 4.016 | 77.004 | 1.00x |
| wide_arrays.json | orjson | 4.704 | 5.912 | 6.204 | 77.004 | 0.65x |
| wide_arrays.json | msgspec | 5.041 | 5.813 | 6.412 | 77.004 | 0.66x |
| wide_arrays.json | ujson | 5.928 | 6.945 | 7.405 | 77.004 | 0.55x |
| wide_arrays.json | json | 10.848 | 11.416 | 12.195 | 77.004 | 0.34x |
| mixed.json | strata | 0.122 | 0.141 | 0.165 | 77.004 | 1.00x |
| mixed.json | orjson | 0.176 | 0.194 | 0.359 | 77.004 | 0.72x |
| mixed.json | msgspec | 0.170 | 0.196 | 0.386 | 77.004 | 0.72x |
| mixed.json | ujson | 0.242 | 0.277 | 0.434 | 77.004 | 0.51x |
| mixed.json | json | 0.310 | 0.342 | 0.639 | 77.004 | 0.41x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.780 | 9.842 | 10.359 | 64.574 | 1.00x |
| users.ndjson | orjson | 11.531 | 13.786 | 14.920 | 64.574 | 0.71x |
| users.ndjson | msgspec | 10.915 | 13.735 | 14.630 | 64.574 | 0.72x |
| users.ndjson | ujson | 14.990 | 19.306 | 20.761 | 64.574 | 0.51x |
| users.ndjson | json | 19.872 | 23.984 | 25.431 | 64.574 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.060 | 2.303 | 4.163 | 66.188 | 1.00x |
| users.json | orjson | 2.032 | 2.330 | 2.688 | 66.188 | 0.99x |
| users.json | msgspec | 3.141 | 3.413 | 3.927 | 66.188 | 0.67x |
| users.json | ujson | 7.333 | 7.773 | 9.178 | 66.188 | 0.30x |
| users.json | json | 12.668 | 13.398 | 16.412 | 66.188 | 0.17x |
| flat.json | strata | 0.389 | 0.430 | 0.469 | 64.574 | 1.00x |
| flat.json | orjson | 0.378 | 0.420 | 0.546 | 64.574 | 1.02x |
| flat.json | msgspec | 0.477 | 0.533 | 0.641 | 64.574 | 0.81x |
| flat.json | ujson | 0.824 | 0.878 | 1.007 | 64.574 | 0.49x |
| flat.json | json | 1.228 | 1.312 | 2.085 | 64.574 | 0.33x |
| nested.json | strata | 0.296 | 0.336 | 0.383 | 64.574 | 1.00x |
| nested.json | orjson | 0.327 | 0.367 | 0.411 | 64.574 | 0.92x |
| nested.json | msgspec | 0.438 | 0.471 | 0.509 | 64.574 | 0.71x |
| nested.json | ujson | 0.809 | 0.849 | 0.898 | 64.574 | 0.40x |
| nested.json | json | 1.462 | 1.514 | 1.591 | 64.574 | 0.22x |
| wide_arrays.json | strata | 2.131 | 2.347 | 2.617 | 77.004 | 1.00x |
| wide_arrays.json | orjson | 1.952 | 2.176 | 2.308 | 77.004 | 1.08x |
| wide_arrays.json | msgspec | 2.772 | 2.979 | 3.093 | 77.004 | 0.79x |
| wide_arrays.json | ujson | 4.462 | 4.914 | 5.464 | 77.004 | 0.48x |
| wide_arrays.json | json | 10.523 | 10.881 | 11.186 | 77.004 | 0.22x |
| mixed.json | strata | 0.204 | 0.235 | 0.263 | 77.004 | 1.00x |
| mixed.json | orjson | 0.218 | 0.247 | 2.048 | 77.004 | 0.95x |
| mixed.json | msgspec | 0.233 | 0.258 | 0.384 | 77.004 | 0.91x |
| mixed.json | ujson | 0.326 | 0.356 | 0.423 | 77.004 | 0.66x |
| mixed.json | json | 0.487 | 0.517 | 0.545 | 77.004 | 0.45x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.043 | 0.049 | 0.054 | 66.188 | 1.00x |
| users.json $[*].id | jmespath | 0.268 | 0.279 | 0.296 | 66.188 | 0.17x |
| users.json $[*].id | jsonpath-ng | 1.744 | 1.921 | 2.019 | 66.188 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.237 | 0.256 | 0.309 | 66.199 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.617 | 1.678 | 2.156 | 66.199 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 12.239 | 13.784 | 16.911 | 66.199 | 0.02x |
| users.json $..total | strata | 1.016 | 1.069 | 1.168 | 66.211 | 1.00x |
| users.json $..total | jsonpath-ng | 217.570 | 227.340 | 233.972 | 66.211 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 2.122 | 2.214 | 2.250 | 66.199 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.879 | 12.379 | 13.088 | 66.199 | 0.18x |
| users.json $[*].id | orjson+jsonpath-ng | 12.532 | 13.997 | 14.764 | 66.199 | 0.16x |
| users.json $[*].orders[*].total | strata | 2.247 | 2.352 | 2.429 | 66.211 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.784 | 13.327 | 14.086 | 66.211 | 0.18x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 25.705 | 28.434 | 30.693 | 66.211 | 0.08x |
| users.json $..total | strata | 11.156 | 16.766 | 17.473 | 66.215 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 241.194 | 256.183 | 260.354 | 66.215 | 0.07x |

