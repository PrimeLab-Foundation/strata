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
| users.json | strata | 8.765 | 8.827 | 10.725 | 57.164 | 1.00x |
| users.json | orjson | 11.611 | 11.683 | 13.305 | 57.164 | 0.76x |
| users.json | msgspec | 12.162 | 12.228 | 13.670 | 57.164 | 0.72x |
| users.json | ujson | 16.260 | 16.421 | 18.947 | 57.164 | 0.54x |
| users.json | pysimdjson | 16.307 | 16.393 | 18.520 | 57.164 | 0.54x |
| users.json | json | 20.661 | 20.727 | 21.540 | 57.164 | 0.43x |
| flat.json | strata | 0.790 | 0.807 | 0.815 | 68.039 | 1.00x |
| flat.json | orjson | 0.851 | 0.867 | 0.872 | 68.039 | 0.93x |
| flat.json | msgspec | 0.879 | 0.894 | 0.907 | 68.039 | 0.90x |
| flat.json | ujson | 1.388 | 1.401 | 1.410 | 68.039 | 0.58x |
| flat.json | pysimdjson | 1.435 | 1.442 | 1.455 | 68.039 | 0.56x |
| flat.json | json | 1.736 | 1.751 | 1.756 | 68.039 | 0.46x |
| nested.json | strata | 0.781 | 0.797 | 0.827 | 68.039 | 1.00x |
| nested.json | orjson | 0.859 | 0.880 | 0.891 | 68.039 | 0.91x |
| nested.json | msgspec | 0.985 | 0.994 | 1.000 | 68.039 | 0.80x |
| nested.json | ujson | 1.378 | 1.397 | 1.422 | 68.039 | 0.57x |
| nested.json | pysimdjson | 1.382 | 1.396 | 1.411 | 68.039 | 0.57x |
| nested.json | json | 1.962 | 1.965 | 1.979 | 68.039 | 0.41x |
| wide_arrays.json | strata | 3.814 | 3.827 | 3.874 | 69.602 | 1.00x |
| wide_arrays.json | orjson | 4.039 | 4.066 | 4.084 | 69.602 | 0.94x |
| wide_arrays.json | msgspec | 5.055 | 5.082 | 5.099 | 69.602 | 0.75x |
| wide_arrays.json | ujson | 6.458 | 6.485 | 6.529 | 69.602 | 0.59x |
| wide_arrays.json | pysimdjson | 5.235 | 5.266 | 5.320 | 69.602 | 0.73x |
| wide_arrays.json | json | 9.457 | 9.497 | 9.548 | 69.602 | 0.40x |
| mixed.json | strata | 0.186 | 0.188 | 0.205 | 69.602 | 1.00x |
| mixed.json | orjson | 0.211 | 0.214 | 0.230 | 69.602 | 0.88x |
| mixed.json | msgspec | 0.233 | 0.239 | 0.255 | 69.602 | 0.79x |
| mixed.json | ujson | 0.299 | 0.305 | 0.342 | 69.602 | 0.62x |
| mixed.json | pysimdjson | 0.289 | 0.297 | 0.313 | 69.602 | 0.63x |
| mixed.json | json | 0.450 | 0.461 | 0.470 | 69.602 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.924 | 1.937 | 1.979 | 56.262 | 1.00x |
| users.json | orjson | 2.582 | 2.588 | 2.597 | 56.262 | 0.75x |
| users.json | msgspec | 3.296 | 3.313 | 3.327 | 56.262 | 0.58x |
| users.json | ujson | 10.452 | 10.519 | 10.579 | 56.262 | 0.18x |
| users.json | json | 18.915 | 18.949 | 19.024 | 56.262 | 0.10x |
| flat.json | strata | 0.233 | 0.236 | 0.238 | 68.039 | 1.00x |
| flat.json | orjson | 0.299 | 0.302 | 0.313 | 68.039 | 0.78x |
| flat.json | msgspec | 0.384 | 0.387 | 0.402 | 68.039 | 0.61x |
| flat.json | ujson | 0.985 | 0.990 | 0.999 | 68.039 | 0.24x |
| flat.json | json | 1.689 | 1.698 | 1.721 | 68.039 | 0.14x |
| nested.json | strata | 0.219 | 0.222 | 0.241 | 68.043 | 1.00x |
| nested.json | orjson | 0.283 | 0.286 | 0.311 | 68.043 | 0.78x |
| nested.json | msgspec | 0.370 | 0.376 | 0.391 | 68.043 | 0.59x |
| nested.json | ujson | 1.073 | 1.079 | 1.096 | 68.043 | 0.21x |
| nested.json | json | 2.136 | 2.160 | 2.179 | 68.043 | 0.10x |
| wide_arrays.json | strata | 1.307 | 1.316 | 1.336 | 69.602 | 1.00x |
| wide_arrays.json | orjson | 1.578 | 1.594 | 1.616 | 69.602 | 0.83x |
| wide_arrays.json | msgspec | 2.360 | 2.370 | 2.388 | 69.602 | 0.56x |
| wide_arrays.json | ujson | 4.732 | 4.746 | 4.767 | 69.602 | 0.28x |
| wide_arrays.json | json | 13.552 | 13.562 | 13.591 | 69.602 | 0.10x |
| mixed.json | strata | 0.059 | 0.060 | 0.063 | 69.602 | 1.00x |
| mixed.json | orjson | 0.062 | 0.063 | 0.065 | 69.602 | 0.94x |
| mixed.json | msgspec | 0.076 | 0.077 | 0.078 | 69.602 | 0.77x |
| mixed.json | ujson | 0.236 | 0.238 | 0.253 | 69.602 | 0.25x |
| mixed.json | json | 0.475 | 0.483 | 0.492 | 69.602 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.968 | 9.024 | 9.739 | 68.473 | 1.00x |
| users.json | orjson | 11.767 | 11.899 | 12.441 | 68.473 | 0.76x |
| users.json | msgspec | 12.337 | 12.472 | 12.627 | 68.473 | 0.72x |
| users.json | ujson | 16.954 | 17.099 | 18.179 | 68.473 | 0.53x |
| users.json | json | 20.912 | 20.994 | 21.118 | 68.473 | 0.43x |
| flat.json | strata | 0.833 | 0.849 | 0.867 | 68.039 | 1.00x |
| flat.json | orjson | 0.925 | 0.940 | 0.956 | 68.039 | 0.90x |
| flat.json | msgspec | 0.962 | 0.967 | 0.978 | 68.039 | 0.88x |
| flat.json | ujson | 1.499 | 1.516 | 1.540 | 68.039 | 0.56x |
| flat.json | json | 1.805 | 1.814 | 1.833 | 68.039 | 0.47x |
| nested.json | strata | 0.823 | 0.833 | 0.843 | 68.043 | 1.00x |
| nested.json | orjson | 0.941 | 0.948 | 0.966 | 68.043 | 0.88x |
| nested.json | msgspec | 1.050 | 1.063 | 1.082 | 68.043 | 0.78x |
| nested.json | ujson | 1.470 | 1.484 | 1.535 | 68.043 | 0.56x |
| nested.json | json | 2.012 | 2.021 | 2.040 | 68.043 | 0.41x |
| wide_arrays.json | strata | 3.817 | 3.836 | 3.872 | 69.602 | 1.00x |
| wide_arrays.json | orjson | 4.030 | 4.061 | 4.082 | 69.602 | 0.94x |
| wide_arrays.json | msgspec | 5.071 | 5.094 | 5.142 | 69.602 | 0.75x |
| wide_arrays.json | ujson | 6.606 | 6.652 | 6.680 | 69.602 | 0.58x |
| wide_arrays.json | json | 9.557 | 9.581 | 9.657 | 69.602 | 0.40x |
| mixed.json | strata | 0.212 | 0.215 | 0.223 | 69.602 | 1.00x |
| mixed.json | orjson | 0.271 | 0.273 | 0.285 | 69.602 | 0.79x |
| mixed.json | msgspec | 0.291 | 0.292 | 0.309 | 69.602 | 0.73x |
| mixed.json | ujson | 0.372 | 0.390 | 0.414 | 69.602 | 0.55x |
| mixed.json | json | 0.504 | 0.518 | 0.531 | 69.602 | 0.41x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.277 | 9.353 | 9.416 | 68.035 | 1.00x |
| users.ndjson | orjson | 14.605 | 14.686 | 14.820 | 68.035 | 0.64x |
| users.ndjson | msgspec | 14.993 | 15.066 | 15.216 | 68.035 | 0.62x |
| users.ndjson | ujson | 19.483 | 19.695 | 19.999 | 68.035 | 0.47x |
| users.ndjson | json | 25.682 | 25.757 | 26.022 | 68.035 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.383 | 2.401 | 2.444 | 68.473 | 1.00x |
| users.json | orjson | 3.078 | 3.109 | 3.134 | 68.473 | 0.77x |
| users.json | msgspec | 3.788 | 3.823 | 3.876 | 68.473 | 0.63x |
| users.json | ujson | 11.076 | 11.138 | 11.199 | 68.473 | 0.22x |
| users.json | json | 19.501 | 19.635 | 19.691 | 68.473 | 0.12x |
| flat.json | strata | 0.382 | 0.392 | 0.429 | 68.039 | 1.00x |
| flat.json | orjson | 0.467 | 0.483 | 0.512 | 68.039 | 0.81x |
| flat.json | msgspec | 0.565 | 0.581 | 0.605 | 68.039 | 0.68x |
| flat.json | ujson | 1.175 | 1.194 | 1.218 | 68.039 | 0.33x |
| flat.json | json | 1.879 | 1.911 | 1.936 | 68.039 | 0.21x |
| nested.json | strata | 0.349 | 0.377 | 0.413 | 68.043 | 1.00x |
| nested.json | orjson | 0.445 | 0.471 | 0.501 | 68.043 | 0.80x |
| nested.json | msgspec | 0.527 | 0.569 | 0.589 | 68.043 | 0.66x |
| nested.json | ujson | 1.266 | 1.292 | 1.336 | 68.043 | 0.29x |
| nested.json | json | 2.321 | 2.355 | 2.398 | 68.043 | 0.16x |
| wide_arrays.json | strata | 1.636 | 1.679 | 1.709 | 69.602 | 1.00x |
| wide_arrays.json | orjson | 1.980 | 2.004 | 2.030 | 69.602 | 0.84x |
| wide_arrays.json | msgspec | 2.742 | 2.775 | 2.799 | 69.602 | 0.60x |
| wide_arrays.json | ujson | 5.178 | 5.216 | 5.232 | 69.602 | 0.32x |
| wide_arrays.json | json | 13.954 | 13.999 | 14.035 | 69.602 | 0.12x |
| mixed.json | strata | 0.164 | 0.172 | 0.203 | 69.602 | 1.00x |
| mixed.json | orjson | 0.188 | 0.197 | 0.230 | 69.602 | 0.87x |
| mixed.json | msgspec | 0.206 | 0.209 | 0.236 | 69.602 | 0.82x |
| mixed.json | ujson | 0.378 | 0.384 | 0.413 | 69.602 | 0.45x |
| mixed.json | json | 0.610 | 0.637 | 0.648 | 69.602 | 0.27x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.103 | 0.106 | 0.118 | 68.473 | 1.00x |
| users.json $[*].id | jmespath | 0.464 | 0.472 | 0.485 | 68.473 | 0.22x |
| users.json $[*].id | jsonpath-ng | 2.427 | 2.459 | 2.506 | 68.473 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.618 | 0.637 | 0.647 | 68.609 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.967 | 2.992 | 3.014 | 68.609 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.383 | 17.474 | 17.727 | 68.609 | 0.04x |
| users.json $..total | strata | 1.712 | 1.725 | 1.744 | 69.625 | 1.00x |
| users.json $..total | jsonpath-ng | 293.073 | 293.820 | 294.376 | 69.625 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.159 | 3.192 | 3.222 | 68.609 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.417 | 12.637 | 12.764 | 68.609 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 14.347 | 14.491 | 14.620 | 68.609 | 0.22x |
| users.json $[*].orders[*].total | strata | 3.346 | 3.367 | 3.390 | 69.625 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.114 | 15.237 | 15.451 | 69.625 | 0.22x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 33.467 | 33.626 | 33.769 | 69.625 | 0.10x |
| users.json $..total | strata | 11.416 | 11.542 | 11.945 | 69.672 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 310.534 | 311.938 | 314.489 | 69.672 | 0.04x |

