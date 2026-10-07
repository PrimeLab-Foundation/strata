# Benchmark results - ci-linux-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c20ac86eedff410e10c973bc3b1f19f6e9a5f56e
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-aarch64-with-glibc2.39
- machine: aarch64
- processor: aarch64
- compiler_flags: -fno-strict-overflow -Wsign-compare -DNDEBUG -g -O3 -Wall -fPIC -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -march=native -flto=thin -fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata -shared -Wl,--rpath=/opt/hostedtoolcache/Python/3.12.14/arm64/lib (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.696 | 8.809 | 10.894 | 57.125 | 1.00x |
| users.json | orjson | 11.540 | 11.725 | 13.456 | 57.125 | 0.75x |
| users.json | msgspec | 12.065 | 12.252 | 13.826 | 57.125 | 0.72x |
| users.json | ujson | 16.205 | 16.565 | 19.002 | 57.125 | 0.53x |
| users.json | pysimdjson | 16.195 | 16.528 | 18.423 | 57.125 | 0.53x |
| users.json | json | 20.377 | 20.596 | 21.362 | 57.125 | 0.43x |
| flat.json | strata | 0.811 | 0.817 | 0.826 | 68.031 | 1.00x |
| flat.json | orjson | 0.858 | 0.876 | 0.885 | 68.031 | 0.93x |
| flat.json | msgspec | 0.905 | 0.926 | 0.927 | 68.031 | 0.88x |
| flat.json | ujson | 1.444 | 1.453 | 1.471 | 68.031 | 0.56x |
| flat.json | pysimdjson | 1.496 | 1.509 | 1.521 | 68.031 | 0.54x |
| flat.json | json | 1.792 | 1.801 | 1.812 | 68.031 | 0.45x |
| nested.json | strata | 0.785 | 0.801 | 0.836 | 68.031 | 1.00x |
| nested.json | orjson | 0.879 | 0.885 | 0.901 | 68.031 | 0.90x |
| nested.json | msgspec | 0.988 | 0.993 | 1.001 | 68.031 | 0.81x |
| nested.json | ujson | 1.382 | 1.393 | 1.518 | 68.031 | 0.58x |
| nested.json | pysimdjson | 1.387 | 1.394 | 1.427 | 68.031 | 0.57x |
| nested.json | json | 1.946 | 1.963 | 1.985 | 68.031 | 0.41x |
| wide_arrays.json | strata | 3.836 | 3.852 | 3.891 | 69.590 | 1.00x |
| wide_arrays.json | orjson | 4.066 | 4.093 | 4.147 | 69.590 | 0.94x |
| wide_arrays.json | msgspec | 5.069 | 5.089 | 5.110 | 69.590 | 0.76x |
| wide_arrays.json | ujson | 6.507 | 6.540 | 6.575 | 69.590 | 0.59x |
| wide_arrays.json | pysimdjson | 5.256 | 5.293 | 5.336 | 69.590 | 0.73x |
| wide_arrays.json | json | 9.455 | 9.483 | 9.555 | 69.590 | 0.41x |
| mixed.json | strata | 0.190 | 0.192 | 0.226 | 69.590 | 1.00x |
| mixed.json | orjson | 0.210 | 0.214 | 0.233 | 69.590 | 0.90x |
| mixed.json | msgspec | 0.231 | 0.236 | 0.260 | 69.590 | 0.81x |
| mixed.json | ujson | 0.299 | 0.302 | 0.322 | 69.590 | 0.64x |
| mixed.json | pysimdjson | 0.294 | 0.295 | 0.325 | 69.590 | 0.65x |
| mixed.json | json | 0.450 | 0.452 | 0.473 | 69.590 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.930 | 1.941 | 1.955 | 56.223 | 1.00x |
| users.json | orjson | 2.578 | 2.598 | 2.619 | 56.223 | 0.75x |
| users.json | msgspec | 3.314 | 3.332 | 3.348 | 56.223 | 0.58x |
| users.json | ujson | 10.498 | 10.543 | 10.614 | 56.223 | 0.18x |
| users.json | json | 18.916 | 18.991 | 19.056 | 56.223 | 0.10x |
| flat.json | strata | 0.235 | 0.236 | 0.254 | 68.031 | 1.00x |
| flat.json | orjson | 0.297 | 0.304 | 0.323 | 68.031 | 0.78x |
| flat.json | msgspec | 0.382 | 0.388 | 0.408 | 68.031 | 0.61x |
| flat.json | ujson | 0.978 | 0.987 | 1.002 | 68.031 | 0.24x |
| flat.json | json | 1.706 | 1.715 | 1.722 | 68.031 | 0.14x |
| nested.json | strata | 0.221 | 0.226 | 0.243 | 68.031 | 1.00x |
| nested.json | orjson | 0.284 | 0.288 | 0.305 | 68.031 | 0.79x |
| nested.json | msgspec | 0.367 | 0.372 | 0.387 | 68.031 | 0.61x |
| nested.json | ujson | 1.072 | 1.081 | 1.098 | 68.031 | 0.21x |
| nested.json | json | 2.128 | 2.167 | 2.197 | 68.031 | 0.10x |
| wide_arrays.json | strata | 1.312 | 1.323 | 1.331 | 69.590 | 1.00x |
| wide_arrays.json | orjson | 1.591 | 1.605 | 1.617 | 69.590 | 0.82x |
| wide_arrays.json | msgspec | 2.378 | 2.380 | 2.396 | 69.590 | 0.56x |
| wide_arrays.json | ujson | 4.744 | 4.755 | 4.768 | 69.590 | 0.28x |
| wide_arrays.json | json | 13.549 | 13.572 | 13.593 | 69.590 | 0.10x |
| mixed.json | strata | 0.061 | 0.062 | 0.063 | 69.590 | 1.00x |
| mixed.json | orjson | 0.063 | 0.064 | 0.065 | 69.590 | 0.97x |
| mixed.json | msgspec | 0.076 | 0.091 | 0.097 | 69.590 | 0.68x |
| mixed.json | ujson | 0.252 | 0.257 | 0.271 | 69.590 | 0.24x |
| mixed.json | json | 0.490 | 0.496 | 0.503 | 69.590 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.941 | 9.147 | 9.683 | 68.465 | 1.00x |
| users.json | orjson | 11.841 | 12.082 | 12.218 | 68.465 | 0.76x |
| users.json | msgspec | 12.413 | 12.562 | 12.719 | 68.465 | 0.73x |
| users.json | ujson | 16.923 | 17.523 | 18.332 | 68.465 | 0.52x |
| users.json | json | 20.805 | 21.172 | 21.389 | 68.465 | 0.43x |
| flat.json | strata | 0.834 | 0.854 | 0.865 | 68.031 | 1.00x |
| flat.json | orjson | 0.930 | 0.951 | 0.963 | 68.031 | 0.90x |
| flat.json | msgspec | 0.993 | 1.000 | 1.013 | 68.031 | 0.85x |
| flat.json | ujson | 1.537 | 1.551 | 1.579 | 68.031 | 0.55x |
| flat.json | json | 1.868 | 1.879 | 1.899 | 68.031 | 0.45x |
| nested.json | strata | 0.835 | 0.843 | 0.849 | 68.031 | 1.00x |
| nested.json | orjson | 0.951 | 0.956 | 0.977 | 68.031 | 0.88x |
| nested.json | msgspec | 1.066 | 1.071 | 1.085 | 68.031 | 0.79x |
| nested.json | ujson | 1.483 | 1.494 | 1.509 | 68.031 | 0.56x |
| nested.json | json | 2.022 | 2.048 | 2.068 | 68.031 | 0.41x |
| wide_arrays.json | strata | 3.835 | 3.857 | 3.892 | 69.590 | 1.00x |
| wide_arrays.json | orjson | 4.039 | 4.063 | 4.105 | 69.590 | 0.95x |
| wide_arrays.json | msgspec | 5.034 | 5.086 | 5.125 | 69.590 | 0.76x |
| wide_arrays.json | ujson | 6.587 | 6.643 | 6.688 | 69.590 | 0.58x |
| wide_arrays.json | json | 9.489 | 9.536 | 9.562 | 69.590 | 0.40x |
| mixed.json | strata | 0.215 | 0.228 | 0.249 | 69.590 | 1.00x |
| mixed.json | orjson | 0.276 | 0.300 | 0.310 | 69.590 | 0.76x |
| mixed.json | msgspec | 0.295 | 0.303 | 0.321 | 69.590 | 0.75x |
| mixed.json | ujson | 0.374 | 0.383 | 0.403 | 69.590 | 0.59x |
| mixed.json | json | 0.505 | 0.525 | 0.542 | 69.590 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.315 | 9.352 | 9.662 | 68.027 | 1.00x |
| users.ndjson | orjson | 14.597 | 14.717 | 14.876 | 68.027 | 0.64x |
| users.ndjson | msgspec | 15.018 | 15.045 | 15.154 | 68.027 | 0.62x |
| users.ndjson | ujson | 19.658 | 19.710 | 20.281 | 68.027 | 0.47x |
| users.ndjson | json | 25.616 | 25.826 | 26.461 | 68.027 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.403 | 2.480 | 2.518 | 68.465 | 1.00x |
| users.json | orjson | 3.121 | 3.175 | 3.209 | 68.465 | 0.78x |
| users.json | msgspec | 3.830 | 3.888 | 3.963 | 68.465 | 0.64x |
| users.json | ujson | 11.127 | 11.185 | 11.254 | 68.465 | 0.22x |
| users.json | json | 19.589 | 19.727 | 19.913 | 68.465 | 0.13x |
| flat.json | strata | 0.384 | 0.408 | 0.438 | 68.031 | 1.00x |
| flat.json | orjson | 0.482 | 0.513 | 0.525 | 68.031 | 0.80x |
| flat.json | msgspec | 0.578 | 0.601 | 0.620 | 68.031 | 0.68x |
| flat.json | ujson | 1.207 | 1.228 | 1.249 | 68.031 | 0.33x |
| flat.json | json | 1.906 | 1.937 | 1.971 | 68.031 | 0.21x |
| nested.json | strata | 0.353 | 0.367 | 0.389 | 68.031 | 1.00x |
| nested.json | orjson | 0.452 | 0.472 | 0.505 | 68.031 | 0.78x |
| nested.json | msgspec | 0.532 | 0.554 | 0.578 | 68.031 | 0.66x |
| nested.json | ujson | 1.264 | 1.283 | 1.321 | 68.031 | 0.29x |
| nested.json | json | 2.315 | 2.353 | 2.383 | 68.031 | 0.16x |
| wide_arrays.json | strata | 1.680 | 1.715 | 1.772 | 69.590 | 1.00x |
| wide_arrays.json | orjson | 2.016 | 2.046 | 2.071 | 69.590 | 0.84x |
| wide_arrays.json | msgspec | 2.787 | 2.817 | 2.842 | 69.590 | 0.61x |
| wide_arrays.json | ujson | 5.209 | 5.265 | 5.311 | 69.590 | 0.33x |
| wide_arrays.json | json | 13.995 | 14.046 | 14.087 | 69.590 | 0.12x |
| mixed.json | strata | 0.168 | 0.173 | 0.193 | 69.590 | 1.00x |
| mixed.json | orjson | 0.191 | 0.197 | 0.223 | 69.590 | 0.88x |
| mixed.json | msgspec | 0.208 | 0.213 | 0.231 | 69.590 | 0.81x |
| mixed.json | ujson | 0.385 | 0.395 | 0.412 | 69.590 | 0.44x |
| mixed.json | json | 0.618 | 0.631 | 0.651 | 69.590 | 0.27x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.106 | 0.109 | 0.113 | 68.465 | 1.00x |
| users.json $[*].id | jmespath | 0.478 | 0.490 | 0.503 | 68.465 | 0.22x |
| users.json $[*].id | jsonpath-ng | 2.433 | 2.520 | 2.604 | 68.465 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.636 | 0.651 | 0.690 | 68.602 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.029 | 3.042 | 3.094 | 68.602 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.649 | 18.099 | 19.043 | 68.602 | 0.04x |
| users.json $..total | strata | 1.716 | 1.732 | 1.744 | 69.617 | 1.00x |
| users.json $..total | jsonpath-ng | 296.624 | 297.128 | 297.485 | 69.617 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.194 | 3.230 | 3.312 | 68.602 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.449 | 12.707 | 13.025 | 68.602 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 14.341 | 14.634 | 14.800 | 68.602 | 0.22x |
| users.json $[*].orders[*].total | strata | 3.363 | 3.403 | 3.441 | 69.617 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.340 | 15.447 | 15.749 | 69.617 | 0.22x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 33.703 | 34.029 | 34.710 | 69.617 | 0.10x |
| users.json $..total | strata | 11.824 | 12.073 | 12.439 | 69.664 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 312.488 | 314.895 | 316.316 | 69.664 | 0.04x |

