# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 33465c22eee0fda8e1b52898c16aac6982901a21
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
| users.json | strata | 9.897 | 9.998 | 13.928 | 68.148 | 1.00x |
| users.json | orjson | 13.475 | 13.709 | 17.220 | 68.148 | 0.73x |
| users.json | msgspec | 13.352 | 13.606 | 16.647 | 68.148 | 0.73x |
| users.json | ujson | 18.055 | 18.765 | 23.571 | 68.148 | 0.53x |
| users.json | pysimdjson | 19.046 | 19.767 | 23.381 | 68.148 | 0.51x |
| users.json | json | 22.277 | 22.683 | 24.038 | 68.148 | 0.44x |
| flat.json | strata | 0.844 | 0.851 | 0.865 | 82.984 | 1.00x |
| flat.json | orjson | 0.981 | 0.988 | 1.007 | 82.984 | 0.86x |
| flat.json | msgspec | 1.000 | 1.016 | 1.070 | 82.984 | 0.84x |
| flat.json | ujson | 1.461 | 1.501 | 1.590 | 82.984 | 0.57x |
| flat.json | pysimdjson | 1.543 | 1.555 | 1.581 | 82.984 | 0.55x |
| flat.json | json | 1.844 | 1.865 | 1.875 | 82.984 | 0.46x |
| nested.json | strata | 0.810 | 0.817 | 1.085 | 82.984 | 1.00x |
| nested.json | orjson | 1.001 | 1.008 | 1.084 | 82.984 | 0.81x |
| nested.json | msgspec | 1.022 | 1.031 | 1.064 | 82.984 | 0.79x |
| nested.json | ujson | 1.448 | 1.480 | 1.586 | 82.984 | 0.55x |
| nested.json | pysimdjson | 1.415 | 1.428 | 1.446 | 82.984 | 0.57x |
| nested.json | json | 2.018 | 2.039 | 2.226 | 82.984 | 0.40x |
| wide_arrays.json | strata | 4.067 | 4.154 | 4.290 | 84.984 | 1.00x |
| wide_arrays.json | orjson | 5.253 | 5.354 | 5.863 | 84.984 | 0.78x |
| wide_arrays.json | msgspec | 5.599 | 5.754 | 6.051 | 84.984 | 0.72x |
| wide_arrays.json | ujson | 7.066 | 7.168 | 7.657 | 84.984 | 0.58x |
| wide_arrays.json | pysimdjson | 6.229 | 6.517 | 6.744 | 84.984 | 0.64x |
| wide_arrays.json | json | 9.806 | 9.912 | 10.295 | 84.984 | 0.42x |
| mixed.json | strata | 0.189 | 0.192 | 0.207 | 84.984 | 1.00x |
| mixed.json | orjson | 0.230 | 0.233 | 0.245 | 84.984 | 0.82x |
| mixed.json | msgspec | 0.241 | 0.251 | 0.259 | 84.984 | 0.76x |
| mixed.json | ujson | 0.299 | 0.309 | 0.327 | 84.984 | 0.62x |
| mixed.json | pysimdjson | 0.296 | 0.304 | 0.317 | 84.984 | 0.63x |
| mixed.json | json | 0.472 | 0.479 | 0.493 | 84.984 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.332 | 2.350 | 2.522 | 66.531 | 1.00x |
| users.json | orjson | 2.880 | 2.918 | 3.017 | 66.531 | 0.81x |
| users.json | msgspec | 3.860 | 3.883 | 4.000 | 66.531 | 0.61x |
| users.json | ujson | 11.289 | 11.414 | 11.514 | 66.531 | 0.21x |
| users.json | json | 22.237 | 22.568 | 22.750 | 66.531 | 0.10x |
| flat.json | strata | 0.273 | 0.275 | 0.295 | 82.984 | 1.00x |
| flat.json | orjson | 0.324 | 0.326 | 0.341 | 82.984 | 0.84x |
| flat.json | msgspec | 0.423 | 0.436 | 0.452 | 82.984 | 0.63x |
| flat.json | ujson | 0.997 | 1.003 | 1.042 | 82.984 | 0.27x |
| flat.json | json | 1.855 | 1.871 | 1.908 | 82.984 | 0.15x |
| nested.json | strata | 0.228 | 0.239 | 0.253 | 82.984 | 1.00x |
| nested.json | orjson | 0.287 | 0.290 | 0.324 | 82.984 | 0.83x |
| nested.json | msgspec | 0.400 | 0.404 | 0.432 | 82.984 | 0.59x |
| nested.json | ujson | 1.056 | 1.070 | 1.123 | 82.984 | 0.22x |
| nested.json | json | 2.348 | 2.380 | 2.389 | 82.984 | 0.10x |
| wide_arrays.json | strata | 1.667 | 1.689 | 1.707 | 84.984 | 1.00x |
| wide_arrays.json | orjson | 1.812 | 1.824 | 1.856 | 84.984 | 0.93x |
| wide_arrays.json | msgspec | 2.746 | 2.760 | 2.836 | 84.984 | 0.61x |
| wide_arrays.json | ujson | 6.339 | 6.427 | 6.842 | 84.984 | 0.26x |
| wide_arrays.json | json | 16.473 | 16.669 | 17.415 | 84.984 | 0.10x |
| mixed.json | strata | 0.059 | 0.060 | 0.061 | 84.984 | 1.00x |
| mixed.json | orjson | 0.063 | 0.065 | 0.081 | 84.984 | 0.92x |
| mixed.json | msgspec | 0.082 | 0.083 | 0.084 | 84.984 | 0.73x |
| mixed.json | ujson | 0.228 | 0.232 | 0.246 | 84.984 | 0.26x |
| mixed.json | json | 0.513 | 0.519 | 0.536 | 84.984 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.733 | 11.193 | 13.350 | 83.359 | 1.00x |
| users.json | orjson | 13.882 | 14.039 | 14.979 | 83.359 | 0.80x |
| users.json | msgspec | 13.886 | 14.332 | 14.591 | 83.359 | 0.78x |
| users.json | ujson | 19.163 | 20.491 | 21.839 | 83.359 | 0.55x |
| users.json | json | 22.997 | 23.189 | 23.759 | 83.359 | 0.48x |
| flat.json | strata | 0.861 | 0.897 | 0.923 | 82.984 | 1.00x |
| flat.json | orjson | 1.037 | 1.047 | 1.121 | 82.984 | 0.86x |
| flat.json | msgspec | 1.066 | 1.087 | 1.145 | 82.984 | 0.82x |
| flat.json | ujson | 1.573 | 1.626 | 1.747 | 82.984 | 0.55x |
| flat.json | json | 1.896 | 1.908 | 1.943 | 82.984 | 0.47x |
| nested.json | strata | 0.832 | 0.846 | 0.890 | 82.984 | 1.00x |
| nested.json | orjson | 1.046 | 1.060 | 1.103 | 82.984 | 0.80x |
| nested.json | msgspec | 1.059 | 1.078 | 1.125 | 82.984 | 0.79x |
| nested.json | ujson | 1.523 | 1.547 | 1.653 | 82.984 | 0.55x |
| nested.json | json | 2.051 | 2.077 | 2.194 | 82.984 | 0.41x |
| wide_arrays.json | strata | 4.203 | 4.272 | 4.431 | 84.984 | 1.00x |
| wide_arrays.json | orjson | 5.219 | 5.348 | 5.457 | 84.984 | 0.80x |
| wide_arrays.json | msgspec | 5.828 | 5.881 | 6.024 | 84.984 | 0.73x |
| wide_arrays.json | ujson | 7.292 | 7.347 | 7.542 | 84.984 | 0.58x |
| wide_arrays.json | json | 9.835 | 9.946 | 10.296 | 84.984 | 0.43x |
| mixed.json | strata | 0.203 | 0.204 | 0.227 | 84.984 | 1.00x |
| mixed.json | orjson | 0.275 | 0.291 | 0.296 | 84.984 | 0.70x |
| mixed.json | msgspec | 0.284 | 0.289 | 0.304 | 84.984 | 0.71x |
| mixed.json | ujson | 0.351 | 0.372 | 0.400 | 84.984 | 0.55x |
| mixed.json | json | 0.507 | 0.527 | 0.543 | 84.984 | 0.39x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.788 | 11.308 | 12.182 | 82.984 | 1.00x |
| users.ndjson | orjson | 17.357 | 17.624 | 19.401 | 82.984 | 0.64x |
| users.ndjson | msgspec | 17.385 | 17.966 | 19.327 | 82.984 | 0.63x |
| users.ndjson | ujson | 22.771 | 23.589 | 25.557 | 82.984 | 0.48x |
| users.ndjson | json | 30.333 | 30.719 | 31.373 | 82.984 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.042 | 3.125 | 3.489 | 83.359 | 1.00x |
| users.json | orjson | 3.743 | 3.865 | 4.117 | 83.359 | 0.81x |
| users.json | msgspec | 4.586 | 4.669 | 5.088 | 83.359 | 0.67x |
| users.json | ujson | 12.257 | 12.407 | 14.059 | 83.359 | 0.25x |
| users.json | json | 22.946 | 23.471 | 24.337 | 83.359 | 0.13x |
| flat.json | strata | 0.504 | 0.568 | 0.790 | 82.984 | 1.00x |
| flat.json | orjson | 0.559 | 0.603 | 3.191 | 82.984 | 0.94x |
| flat.json | msgspec | 0.663 | 0.723 | 0.868 | 82.984 | 0.79x |
| flat.json | ujson | 1.282 | 1.319 | 1.431 | 82.984 | 0.43x |
| flat.json | json | 2.145 | 2.224 | 2.373 | 82.984 | 0.26x |
| nested.json | strata | 0.414 | 0.451 | 0.497 | 82.984 | 1.00x |
| nested.json | orjson | 0.491 | 0.574 | 0.738 | 82.984 | 0.79x |
| nested.json | msgspec | 0.605 | 0.643 | 0.757 | 82.984 | 0.70x |
| nested.json | ujson | 1.272 | 1.342 | 3.848 | 82.984 | 0.34x |
| nested.json | json | 2.645 | 2.707 | 2.873 | 82.984 | 0.17x |
| wide_arrays.json | strata | 2.224 | 2.314 | 2.398 | 84.984 | 1.00x |
| wide_arrays.json | orjson | 2.382 | 2.415 | 3.363 | 84.984 | 0.96x |
| wide_arrays.json | msgspec | 3.251 | 3.417 | 7.633 | 84.984 | 0.68x |
| wide_arrays.json | ujson | 6.984 | 7.125 | 9.705 | 84.984 | 0.32x |
| wide_arrays.json | json | 17.312 | 17.407 | 17.553 | 84.984 | 0.13x |
| mixed.json | strata | 0.225 | 0.296 | 0.874 | 84.984 | 1.00x |
| mixed.json | orjson | 0.238 | 0.322 | 0.418 | 84.984 | 0.92x |
| mixed.json | msgspec | 0.271 | 0.330 | 0.735 | 84.984 | 0.90x |
| mixed.json | ujson | 0.435 | 0.493 | 0.598 | 84.984 | 0.60x |
| mixed.json | json | 0.726 | 0.822 | 0.919 | 84.984 | 0.36x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.065 | 0.069 | 0.072 | 83.359 | 1.00x |
| users.json $[*].id | jmespath | 0.499 | 0.507 | 0.558 | 83.359 | 0.14x |
| users.json $[*].id | jsonpath-ng | 2.919 | 3.057 | 3.322 | 83.359 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.440 | 0.463 | 0.497 | 83.363 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.099 | 3.155 | 3.208 | 83.363 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 19.928 | 20.484 | 21.038 | 83.363 | 0.02x |
| users.json $..total | strata | 1.666 | 1.706 | 1.868 | 84.590 | 1.00x |
| users.json $..total | jsonpath-ng | 397.497 | 401.095 | 403.376 | 84.590 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.262 | 3.275 | 3.297 | 83.363 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.934 | 15.129 | 16.330 | 83.363 | 0.22x |
| users.json $[*].id | orjson+jsonpath-ng | 17.410 | 17.758 | 19.064 | 83.363 | 0.18x |
| users.json $[*].orders[*].total | strata | 3.549 | 3.585 | 3.622 | 83.363 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.952 | 18.610 | 20.067 | 83.363 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 38.523 | 40.368 | 41.918 | 83.363 | 0.09x |
| users.json $..total | strata | 14.116 | 15.143 | 17.904 | 84.590 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 419.620 | 424.507 | 427.196 | 84.590 | 0.04x |

