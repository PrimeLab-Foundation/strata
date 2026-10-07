# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c20ac86eedff410e10c973bc3b1f19f6e9a5f56e
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
| users.json | strata | 9.990 | 10.297 | 13.770 | 64.840 | 1.00x |
| users.json | orjson | 13.151 | 13.595 | 16.379 | 64.840 | 0.76x |
| users.json | msgspec | 13.118 | 13.412 | 16.126 | 64.840 | 0.77x |
| users.json | ujson | 17.957 | 18.486 | 26.526 | 64.840 | 0.56x |
| users.json | pysimdjson | 18.743 | 19.305 | 23.304 | 64.840 | 0.53x |
| users.json | json | 22.606 | 22.932 | 31.632 | 64.840 | 0.45x |
| flat.json | strata | 0.819 | 0.832 | 0.893 | 82.105 | 1.00x |
| flat.json | orjson | 0.976 | 0.978 | 0.987 | 82.105 | 0.85x |
| flat.json | msgspec | 1.008 | 1.025 | 1.034 | 82.105 | 0.81x |
| flat.json | ujson | 1.453 | 1.460 | 1.477 | 82.105 | 0.57x |
| flat.json | pysimdjson | 1.530 | 1.540 | 1.582 | 82.105 | 0.54x |
| flat.json | json | 1.903 | 1.924 | 1.941 | 82.105 | 0.43x |
| nested.json | strata | 0.826 | 0.839 | 0.849 | 82.105 | 1.00x |
| nested.json | orjson | 1.020 | 1.027 | 1.036 | 82.105 | 0.82x |
| nested.json | msgspec | 1.060 | 1.063 | 1.085 | 82.105 | 0.79x |
| nested.json | ujson | 1.506 | 1.526 | 1.549 | 82.105 | 0.55x |
| nested.json | pysimdjson | 1.440 | 1.459 | 1.485 | 82.105 | 0.58x |
| nested.json | json | 2.072 | 2.079 | 2.095 | 82.105 | 0.40x |
| wide_arrays.json | strata | 4.053 | 4.195 | 4.580 | 84.105 | 1.00x |
| wide_arrays.json | orjson | 5.122 | 5.355 | 5.734 | 84.105 | 0.78x |
| wide_arrays.json | msgspec | 5.647 | 5.902 | 6.083 | 84.105 | 0.71x |
| wide_arrays.json | ujson | 7.058 | 7.334 | 7.483 | 84.105 | 0.57x |
| wide_arrays.json | pysimdjson | 6.146 | 6.372 | 6.675 | 84.105 | 0.66x |
| wide_arrays.json | json | 9.797 | 10.046 | 10.473 | 84.105 | 0.42x |
| mixed.json | strata | 0.188 | 0.189 | 0.222 | 84.168 | 1.00x |
| mixed.json | orjson | 0.224 | 0.228 | 0.244 | 84.168 | 0.83x |
| mixed.json | msgspec | 0.234 | 0.238 | 0.270 | 84.168 | 0.79x |
| mixed.json | ujson | 0.296 | 0.301 | 0.316 | 84.168 | 0.63x |
| mixed.json | pysimdjson | 0.296 | 0.300 | 0.315 | 84.168 | 0.63x |
| mixed.json | json | 0.464 | 0.474 | 0.485 | 84.168 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.424 | 2.434 | 2.453 | 63.938 | 1.00x |
| users.json | orjson | 2.893 | 2.908 | 2.971 | 63.938 | 0.84x |
| users.json | msgspec | 3.839 | 3.849 | 3.878 | 63.938 | 0.63x |
| users.json | ujson | 11.179 | 11.365 | 11.465 | 63.938 | 0.21x |
| users.json | json | 21.354 | 21.426 | 21.644 | 63.938 | 0.11x |
| flat.json | strata | 0.266 | 0.267 | 0.280 | 82.105 | 1.00x |
| flat.json | orjson | 0.330 | 0.332 | 0.345 | 82.105 | 0.80x |
| flat.json | msgspec | 0.428 | 0.430 | 0.450 | 82.105 | 0.62x |
| flat.json | ujson | 0.999 | 1.010 | 1.084 | 82.105 | 0.26x |
| flat.json | json | 1.835 | 1.847 | 1.856 | 82.105 | 0.14x |
| nested.json | strata | 0.266 | 0.269 | 0.288 | 82.105 | 1.00x |
| nested.json | orjson | 0.304 | 0.307 | 0.318 | 82.105 | 0.88x |
| nested.json | msgspec | 0.403 | 0.410 | 0.430 | 82.105 | 0.66x |
| nested.json | ujson | 1.084 | 1.110 | 1.129 | 82.105 | 0.24x |
| nested.json | json | 2.402 | 2.421 | 2.438 | 82.105 | 0.11x |
| wide_arrays.json | strata | 1.574 | 1.588 | 1.612 | 84.105 | 1.00x |
| wide_arrays.json | orjson | 1.806 | 1.825 | 1.860 | 84.105 | 0.87x |
| wide_arrays.json | msgspec | 2.779 | 2.788 | 2.854 | 84.105 | 0.57x |
| wide_arrays.json | ujson | 6.383 | 6.397 | 7.321 | 84.105 | 0.25x |
| wide_arrays.json | json | 16.524 | 16.591 | 16.777 | 84.105 | 0.10x |
| mixed.json | strata | 0.066 | 0.067 | 0.068 | 84.168 | 1.00x |
| mixed.json | orjson | 0.063 | 0.065 | 0.077 | 84.168 | 1.04x |
| mixed.json | msgspec | 0.082 | 0.084 | 0.096 | 84.168 | 0.80x |
| mixed.json | ujson | 0.227 | 0.230 | 0.250 | 84.168 | 0.29x |
| mixed.json | json | 0.496 | 0.507 | 0.519 | 84.168 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.438 | 10.665 | 12.116 | 81.434 | 1.00x |
| users.json | orjson | 13.415 | 13.695 | 14.274 | 81.434 | 0.78x |
| users.json | msgspec | 13.466 | 13.788 | 14.076 | 81.434 | 0.77x |
| users.json | ujson | 18.509 | 19.123 | 20.923 | 81.434 | 0.56x |
| users.json | json | 22.871 | 23.156 | 23.744 | 81.434 | 0.46x |
| flat.json | strata | 0.851 | 0.865 | 0.875 | 82.105 | 1.00x |
| flat.json | orjson | 1.035 | 1.036 | 1.069 | 82.105 | 0.83x |
| flat.json | msgspec | 1.060 | 1.080 | 1.106 | 82.105 | 0.80x |
| flat.json | ujson | 1.535 | 1.550 | 1.600 | 82.105 | 0.56x |
| flat.json | json | 1.966 | 1.982 | 2.025 | 82.105 | 0.44x |
| nested.json | strata | 0.857 | 0.870 | 0.878 | 82.105 | 1.00x |
| nested.json | orjson | 1.074 | 1.078 | 1.107 | 82.105 | 0.81x |
| nested.json | msgspec | 1.113 | 1.125 | 1.133 | 82.105 | 0.77x |
| nested.json | ujson | 1.582 | 1.601 | 1.621 | 82.105 | 0.54x |
| nested.json | json | 2.128 | 2.145 | 2.237 | 82.105 | 0.41x |
| wide_arrays.json | strata | 4.183 | 4.244 | 4.290 | 84.168 | 1.00x |
| wide_arrays.json | orjson | 5.116 | 5.274 | 5.379 | 84.168 | 0.80x |
| wide_arrays.json | msgspec | 5.795 | 5.882 | 5.961 | 84.168 | 0.72x |
| wide_arrays.json | ujson | 7.261 | 7.349 | 7.603 | 84.168 | 0.58x |
| wide_arrays.json | json | 9.814 | 9.890 | 10.248 | 84.168 | 0.43x |
| mixed.json | strata | 0.222 | 0.225 | 0.229 | 84.168 | 1.00x |
| mixed.json | orjson | 0.271 | 0.273 | 0.275 | 84.168 | 0.82x |
| mixed.json | msgspec | 0.294 | 0.299 | 0.309 | 84.168 | 0.75x |
| mixed.json | ujson | 0.354 | 0.356 | 0.372 | 84.168 | 0.63x |
| mixed.json | json | 0.510 | 0.514 | 0.516 | 84.168 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.193 | 10.269 | 10.487 | 82.105 | 1.00x |
| users.ndjson | orjson | 16.620 | 16.741 | 16.973 | 82.105 | 0.61x |
| users.ndjson | msgspec | 16.658 | 16.723 | 16.827 | 82.105 | 0.61x |
| users.ndjson | ujson | 21.599 | 21.778 | 22.504 | 82.105 | 0.47x |
| users.ndjson | json | 28.891 | 29.305 | 30.197 | 82.105 | 0.35x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.954 | 2.985 | 3.062 | 81.434 | 1.00x |
| users.json | orjson | 3.530 | 3.571 | 3.651 | 81.434 | 0.84x |
| users.json | msgspec | 4.401 | 4.435 | 4.481 | 81.434 | 0.67x |
| users.json | ujson | 12.005 | 12.072 | 12.192 | 81.434 | 0.25x |
| users.json | json | 22.162 | 22.217 | 22.341 | 81.434 | 0.13x |
| flat.json | strata | 0.404 | 0.407 | 0.419 | 82.105 | 1.00x |
| flat.json | orjson | 0.481 | 0.490 | 0.501 | 82.105 | 0.83x |
| flat.json | msgspec | 0.582 | 0.598 | 0.622 | 82.105 | 0.68x |
| flat.json | ujson | 1.191 | 1.194 | 1.210 | 82.105 | 0.34x |
| flat.json | json | 2.008 | 2.017 | 2.053 | 82.105 | 0.20x |
| nested.json | strata | 0.381 | 0.389 | 0.439 | 82.105 | 1.00x |
| nested.json | orjson | 0.435 | 0.455 | 0.482 | 82.105 | 0.85x |
| nested.json | msgspec | 0.533 | 0.554 | 0.669 | 82.105 | 0.70x |
| nested.json | ujson | 1.254 | 1.260 | 1.383 | 82.105 | 0.31x |
| nested.json | json | 2.565 | 2.584 | 2.653 | 82.105 | 0.15x |
| wide_arrays.json | strata | 1.965 | 2.038 | 2.087 | 84.168 | 1.00x |
| wide_arrays.json | orjson | 2.228 | 2.286 | 2.380 | 84.168 | 0.89x |
| wide_arrays.json | msgspec | 3.197 | 3.239 | 3.266 | 84.168 | 0.63x |
| wide_arrays.json | ujson | 6.819 | 6.893 | 6.972 | 84.168 | 0.30x |
| wide_arrays.json | json | 16.930 | 17.083 | 20.059 | 84.168 | 0.12x |
| mixed.json | strata | 0.153 | 0.157 | 0.172 | 84.168 | 1.00x |
| mixed.json | orjson | 0.168 | 0.171 | 0.207 | 84.168 | 0.92x |
| mixed.json | msgspec | 0.184 | 0.190 | 0.207 | 84.168 | 0.83x |
| mixed.json | ujson | 0.340 | 0.356 | 0.391 | 84.168 | 0.44x |
| mixed.json | json | 0.618 | 0.632 | 0.662 | 84.168 | 0.25x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.062 | 0.063 | 0.065 | 81.434 | 1.00x |
| users.json $[*].id | jmespath | 0.488 | 0.500 | 0.510 | 81.434 | 0.13x |
| users.json $[*].id | jsonpath-ng | 2.798 | 2.823 | 2.864 | 81.434 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.417 | 0.448 | 0.458 | 81.441 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.102 | 3.119 | 3.161 | 81.441 | 0.14x |
| users.json $[*].orders[*].total | jsonpath-ng | 19.245 | 19.492 | 19.682 | 81.441 | 0.02x |
| users.json $..total | strata | 1.666 | 1.675 | 1.729 | 82.469 | 1.00x |
| users.json $..total | jsonpath-ng | 388.745 | 390.040 | 392.694 | 82.469 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.153 | 3.174 | 3.200 | 81.441 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.504 | 14.639 | 14.905 | 81.441 | 0.22x |
| users.json $[*].id | orjson+jsonpath-ng | 16.852 | 16.979 | 17.135 | 81.441 | 0.19x |
| users.json $[*].orders[*].total | strata | 3.444 | 3.467 | 3.544 | 81.441 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.829 | 18.130 | 18.397 | 81.441 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 37.735 | 38.295 | 39.410 | 81.441 | 0.09x |
| users.json $..total | strata | 13.635 | 14.363 | 14.546 | 82.469 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 405.358 | 408.142 | 413.951 | 82.469 | 0.04x |

