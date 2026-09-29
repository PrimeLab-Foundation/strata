# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: bc6d9ba83ad33f6b9e2f3b35d87b07d0b4dcc11e
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
| users.json | strata | 10.001 | 10.678 | 14.035 | 63.836 | 1.00x |
| users.json | orjson | 13.864 | 14.062 | 17.067 | 63.836 | 0.76x |
| users.json | msgspec | 13.542 | 13.940 | 16.494 | 63.836 | 0.77x |
| users.json | ujson | 18.278 | 19.524 | 24.091 | 63.836 | 0.55x |
| users.json | pysimdjson | 19.162 | 19.879 | 23.672 | 63.836 | 0.54x |
| users.json | json | 22.469 | 22.886 | 24.191 | 63.836 | 0.47x |
| flat.json | strata | 0.886 | 0.911 | 0.936 | 80.125 | 1.00x |
| flat.json | orjson | 1.051 | 1.071 | 1.144 | 80.125 | 0.85x |
| flat.json | msgspec | 1.038 | 1.051 | 1.085 | 80.125 | 0.87x |
| flat.json | ujson | 1.534 | 1.584 | 1.733 | 80.125 | 0.58x |
| flat.json | pysimdjson | 1.558 | 1.579 | 1.641 | 80.125 | 0.58x |
| flat.json | json | 1.858 | 1.877 | 1.956 | 80.125 | 0.49x |
| nested.json | strata | 0.820 | 0.833 | 0.852 | 80.125 | 1.00x |
| nested.json | orjson | 1.028 | 1.033 | 1.083 | 80.125 | 0.81x |
| nested.json | msgspec | 1.033 | 1.039 | 1.058 | 80.125 | 0.80x |
| nested.json | ujson | 1.495 | 1.556 | 1.610 | 80.125 | 0.54x |
| nested.json | pysimdjson | 1.444 | 1.455 | 1.546 | 80.125 | 0.57x |
| nested.json | json | 2.082 | 2.093 | 2.121 | 80.125 | 0.40x |
| wide_arrays.json | strata | 4.160 | 4.270 | 4.384 | 84.188 | 1.00x |
| wide_arrays.json | orjson | 5.622 | 5.854 | 6.962 | 84.188 | 0.73x |
| wide_arrays.json | msgspec | 5.729 | 5.926 | 6.534 | 84.188 | 0.72x |
| wide_arrays.json | ujson | 7.192 | 7.439 | 8.101 | 84.188 | 0.57x |
| wide_arrays.json | pysimdjson | 6.445 | 6.641 | 6.852 | 84.188 | 0.64x |
| wide_arrays.json | json | 9.895 | 10.060 | 10.609 | 84.188 | 0.42x |
| mixed.json | strata | 0.192 | 0.194 | 0.208 | 84.188 | 1.00x |
| mixed.json | orjson | 0.235 | 0.237 | 0.254 | 84.188 | 0.82x |
| mixed.json | msgspec | 0.243 | 0.247 | 0.259 | 84.188 | 0.79x |
| mixed.json | ujson | 0.300 | 0.307 | 0.338 | 84.188 | 0.63x |
| mixed.json | pysimdjson | 0.300 | 0.302 | 0.322 | 84.188 | 0.64x |
| mixed.json | json | 0.474 | 0.481 | 0.495 | 84.188 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.342 | 2.372 | 2.495 | 62.582 | 1.00x |
| users.json | orjson | 2.987 | 3.009 | 3.101 | 62.582 | 0.79x |
| users.json | msgspec | 3.935 | 3.947 | 4.136 | 62.582 | 0.60x |
| users.json | ujson | 11.407 | 11.543 | 11.798 | 62.582 | 0.21x |
| users.json | json | 22.485 | 22.603 | 22.792 | 62.582 | 0.10x |
| flat.json | strata | 0.275 | 0.277 | 0.300 | 80.125 | 1.00x |
| flat.json | orjson | 0.327 | 0.331 | 0.351 | 80.125 | 0.84x |
| flat.json | msgspec | 0.431 | 0.441 | 0.453 | 80.125 | 0.63x |
| flat.json | ujson | 1.006 | 1.013 | 1.051 | 80.125 | 0.27x |
| flat.json | json | 1.863 | 1.873 | 1.920 | 80.125 | 0.15x |
| nested.json | strata | 0.227 | 0.228 | 0.252 | 80.125 | 1.00x |
| nested.json | orjson | 0.286 | 0.289 | 0.302 | 80.125 | 0.79x |
| nested.json | msgspec | 0.405 | 0.422 | 0.448 | 80.125 | 0.54x |
| nested.json | ujson | 1.058 | 1.068 | 1.083 | 80.125 | 0.21x |
| nested.json | json | 2.398 | 2.412 | 2.688 | 80.125 | 0.09x |
| wide_arrays.json | strata | 1.688 | 1.713 | 2.299 | 84.188 | 1.00x |
| wide_arrays.json | orjson | 1.912 | 1.940 | 2.215 | 84.188 | 0.88x |
| wide_arrays.json | msgspec | 2.763 | 2.801 | 2.953 | 84.188 | 0.61x |
| wide_arrays.json | ujson | 6.378 | 6.408 | 6.483 | 84.188 | 0.27x |
| wide_arrays.json | json | 16.601 | 16.630 | 17.602 | 84.188 | 0.10x |
| mixed.json | strata | 0.060 | 0.061 | 0.074 | 84.188 | 1.00x |
| mixed.json | orjson | 0.063 | 0.064 | 0.065 | 84.188 | 0.94x |
| mixed.json | msgspec | 0.084 | 0.086 | 0.100 | 84.188 | 0.71x |
| mixed.json | ujson | 0.232 | 0.234 | 0.251 | 84.188 | 0.26x |
| mixed.json | json | 0.520 | 0.524 | 0.547 | 84.188 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.020 | 11.392 | 12.597 | 81.758 | 1.00x |
| users.json | orjson | 14.094 | 14.455 | 14.854 | 81.758 | 0.79x |
| users.json | msgspec | 14.087 | 14.386 | 15.019 | 81.758 | 0.79x |
| users.json | ujson | 19.775 | 20.878 | 22.844 | 81.758 | 0.55x |
| users.json | json | 22.982 | 23.539 | 23.994 | 81.758 | 0.48x |
| flat.json | strata | 0.916 | 0.934 | 0.962 | 80.125 | 1.00x |
| flat.json | orjson | 1.125 | 1.138 | 1.163 | 80.125 | 0.82x |
| flat.json | msgspec | 1.079 | 1.096 | 1.121 | 80.125 | 0.85x |
| flat.json | ujson | 1.623 | 1.657 | 1.709 | 80.125 | 0.56x |
| flat.json | json | 1.931 | 1.936 | 1.959 | 80.125 | 0.48x |
| nested.json | strata | 0.877 | 0.884 | 0.938 | 80.125 | 1.00x |
| nested.json | orjson | 1.081 | 1.098 | 1.181 | 80.125 | 0.81x |
| nested.json | msgspec | 1.089 | 1.096 | 1.176 | 80.125 | 0.81x |
| nested.json | ujson | 1.552 | 1.593 | 1.665 | 80.125 | 0.55x |
| nested.json | json | 2.120 | 2.152 | 2.230 | 80.125 | 0.41x |
| wide_arrays.json | strata | 4.277 | 4.336 | 4.650 | 84.188 | 1.00x |
| wide_arrays.json | orjson | 5.652 | 5.757 | 5.948 | 84.188 | 0.75x |
| wide_arrays.json | msgspec | 5.896 | 6.090 | 6.387 | 84.188 | 0.71x |
| wide_arrays.json | ujson | 7.323 | 7.432 | 7.566 | 84.188 | 0.58x |
| wide_arrays.json | json | 9.890 | 10.047 | 10.279 | 84.188 | 0.43x |
| mixed.json | strata | 0.208 | 0.210 | 0.233 | 84.188 | 1.00x |
| mixed.json | orjson | 0.285 | 0.297 | 0.312 | 84.188 | 0.71x |
| mixed.json | msgspec | 0.291 | 0.294 | 0.316 | 84.188 | 0.71x |
| mixed.json | ujson | 0.366 | 0.374 | 0.399 | 84.188 | 0.56x |
| mixed.json | json | 0.518 | 0.529 | 0.542 | 84.188 | 0.40x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.981 | 11.734 | 12.603 | 80.125 | 1.00x |
| users.ndjson | orjson | 17.729 | 17.983 | 18.925 | 80.125 | 0.65x |
| users.ndjson | msgspec | 17.652 | 18.103 | 18.612 | 80.125 | 0.65x |
| users.ndjson | ujson | 22.908 | 23.687 | 25.805 | 80.125 | 0.50x |
| users.ndjson | json | 30.277 | 31.007 | 31.756 | 80.125 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.089 | 3.120 | 6.770 | 81.758 | 1.00x |
| users.json | orjson | 3.806 | 3.833 | 3.886 | 81.758 | 0.81x |
| users.json | msgspec | 4.641 | 4.731 | 4.766 | 81.758 | 0.66x |
| users.json | ujson | 12.549 | 12.643 | 12.881 | 81.758 | 0.25x |
| users.json | json | 23.129 | 23.339 | 26.480 | 81.758 | 0.13x |
| flat.json | strata | 0.485 | 0.515 | 0.567 | 80.125 | 1.00x |
| flat.json | orjson | 0.567 | 0.585 | 0.612 | 80.125 | 0.88x |
| flat.json | msgspec | 0.687 | 0.703 | 0.739 | 80.125 | 0.73x |
| flat.json | ujson | 1.282 | 1.308 | 2.511 | 80.125 | 0.39x |
| flat.json | json | 2.180 | 2.215 | 2.262 | 80.125 | 0.23x |
| nested.json | strata | 0.419 | 0.436 | 0.527 | 80.125 | 1.00x |
| nested.json | orjson | 0.497 | 0.521 | 0.575 | 80.125 | 0.84x |
| nested.json | msgspec | 0.618 | 0.650 | 0.727 | 80.125 | 0.67x |
| nested.json | ujson | 1.305 | 1.331 | 1.352 | 80.125 | 0.33x |
| nested.json | json | 2.629 | 2.669 | 6.760 | 80.125 | 0.16x |
| wide_arrays.json | strata | 2.213 | 2.268 | 2.684 | 84.188 | 1.00x |
| wide_arrays.json | orjson | 2.452 | 2.477 | 2.594 | 84.188 | 0.92x |
| wide_arrays.json | msgspec | 3.295 | 3.344 | 3.486 | 84.188 | 0.68x |
| wide_arrays.json | ujson | 7.058 | 7.082 | 9.497 | 84.188 | 0.32x |
| wide_arrays.json | json | 17.384 | 17.586 | 19.095 | 84.188 | 0.13x |
| mixed.json | strata | 0.205 | 0.227 | 2.857 | 84.188 | 1.00x |
| mixed.json | orjson | 0.233 | 0.249 | 0.275 | 84.188 | 0.91x |
| mixed.json | msgspec | 0.256 | 0.263 | 0.297 | 84.188 | 0.86x |
| mixed.json | ujson | 0.410 | 0.431 | 0.447 | 84.188 | 0.53x |
| mixed.json | json | 0.701 | 0.719 | 0.770 | 84.188 | 0.32x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.069 | 0.070 | 0.073 | 81.758 | 1.00x |
| users.json $[*].id | jmespath | 0.508 | 0.520 | 0.540 | 81.758 | 0.14x |
| users.json $[*].id | jsonpath-ng | 2.943 | 3.164 | 3.271 | 81.758 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.437 | 0.460 | 0.524 | 81.762 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.128 | 3.173 | 4.660 | 81.762 | 0.14x |
| users.json $[*].orders[*].total | jsonpath-ng | 20.235 | 21.352 | 22.511 | 81.762 | 0.02x |
| users.json $..total | strata | 1.721 | 1.730 | 1.770 | 81.762 | 1.00x |
| users.json $..total | jsonpath-ng | 394.069 | 397.545 | 399.073 | 81.762 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.288 | 3.303 | 3.332 | 81.762 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.246 | 15.522 | 16.445 | 81.762 | 0.21x |
| users.json $[*].id | orjson+jsonpath-ng | 17.767 | 18.148 | 20.287 | 81.762 | 0.18x |
| users.json $[*].orders[*].total | strata | 3.549 | 3.572 | 3.623 | 81.762 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.283 | 19.232 | 22.002 | 81.762 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 40.514 | 41.868 | 42.801 | 81.762 | 0.09x |
| users.json $..total | strata | 14.083 | 16.519 | 19.359 | 81.762 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 416.254 | 421.631 | 423.694 | 81.762 | 0.04x |

