# Benchmark results - ci-linux-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: b32d39824b8fea15839872cab8834a1b0fef1294
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
| users.json | strata | 8.731 | 8.833 | 10.548 | 57.227 | 1.00x |
| users.json | orjson | 11.801 | 11.894 | 13.249 | 57.227 | 0.74x |
| users.json | msgspec | 12.254 | 12.393 | 13.636 | 57.227 | 0.71x |
| users.json | ujson | 16.631 | 16.775 | 18.826 | 57.227 | 0.53x |
| users.json | pysimdjson | 16.577 | 16.783 | 18.283 | 57.227 | 0.53x |
| users.json | json | 20.664 | 20.762 | 21.485 | 57.227 | 0.43x |
| flat.json | strata | 0.806 | 0.850 | 0.880 | 68.113 | 1.00x |
| flat.json | orjson | 0.872 | 0.885 | 0.897 | 68.113 | 0.96x |
| flat.json | msgspec | 0.917 | 0.939 | 0.945 | 68.113 | 0.90x |
| flat.json | ujson | 1.480 | 1.496 | 1.510 | 68.113 | 0.57x |
| flat.json | pysimdjson | 1.495 | 1.507 | 1.529 | 68.113 | 0.56x |
| flat.json | json | 1.794 | 1.807 | 1.811 | 68.113 | 0.47x |
| nested.json | strata | 0.802 | 0.820 | 0.835 | 68.113 | 1.00x |
| nested.json | orjson | 0.876 | 0.895 | 0.901 | 68.113 | 0.92x |
| nested.json | msgspec | 1.005 | 1.013 | 1.024 | 68.113 | 0.81x |
| nested.json | ujson | 1.432 | 1.445 | 1.492 | 68.113 | 0.57x |
| nested.json | pysimdjson | 1.415 | 1.427 | 1.443 | 68.113 | 0.57x |
| nested.json | json | 1.975 | 1.992 | 2.019 | 68.113 | 0.41x |
| wide_arrays.json | strata | 3.908 | 3.964 | 4.033 | 69.691 | 1.00x |
| wide_arrays.json | orjson | 4.203 | 4.241 | 4.318 | 69.691 | 0.93x |
| wide_arrays.json | msgspec | 5.145 | 5.176 | 5.251 | 69.691 | 0.77x |
| wide_arrays.json | ujson | 6.557 | 6.608 | 6.657 | 69.691 | 0.60x |
| wide_arrays.json | pysimdjson | 5.396 | 5.434 | 5.481 | 69.691 | 0.73x |
| wide_arrays.json | json | 9.566 | 9.629 | 9.698 | 69.691 | 0.41x |
| mixed.json | strata | 0.193 | 0.196 | 0.228 | 69.691 | 1.00x |
| mixed.json | orjson | 0.219 | 0.223 | 0.244 | 69.691 | 0.88x |
| mixed.json | msgspec | 0.236 | 0.243 | 0.262 | 69.691 | 0.80x |
| mixed.json | ujson | 0.311 | 0.318 | 0.343 | 69.691 | 0.62x |
| mixed.json | pysimdjson | 0.302 | 0.313 | 0.327 | 69.691 | 0.63x |
| mixed.json | json | 0.459 | 0.467 | 0.493 | 69.691 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.933 | 1.943 | 1.963 | 56.336 | 1.00x |
| users.json | orjson | 2.592 | 2.608 | 2.615 | 56.336 | 0.74x |
| users.json | msgspec | 3.320 | 3.342 | 3.368 | 56.336 | 0.58x |
| users.json | ujson | 10.495 | 10.537 | 10.597 | 56.336 | 0.18x |
| users.json | json | 18.893 | 18.946 | 19.032 | 56.336 | 0.10x |
| flat.json | strata | 0.239 | 0.247 | 0.263 | 68.113 | 1.00x |
| flat.json | orjson | 0.301 | 0.312 | 0.326 | 68.113 | 0.79x |
| flat.json | msgspec | 0.398 | 0.409 | 0.430 | 68.113 | 0.60x |
| flat.json | ujson | 0.989 | 0.995 | 1.004 | 68.113 | 0.25x |
| flat.json | json | 1.682 | 1.709 | 1.733 | 68.113 | 0.14x |
| nested.json | strata | 0.225 | 0.227 | 0.247 | 68.113 | 1.00x |
| nested.json | orjson | 0.287 | 0.290 | 0.306 | 68.113 | 0.78x |
| nested.json | msgspec | 0.379 | 0.383 | 0.412 | 68.113 | 0.59x |
| nested.json | ujson | 1.083 | 1.094 | 1.105 | 68.113 | 0.21x |
| nested.json | json | 2.173 | 2.195 | 2.231 | 68.113 | 0.10x |
| wide_arrays.json | strata | 1.353 | 1.370 | 1.402 | 69.691 | 1.00x |
| wide_arrays.json | orjson | 1.622 | 1.636 | 1.650 | 69.691 | 0.84x |
| wide_arrays.json | msgspec | 2.396 | 2.411 | 2.441 | 69.691 | 0.57x |
| wide_arrays.json | ujson | 4.800 | 4.843 | 4.949 | 69.691 | 0.28x |
| wide_arrays.json | json | 13.630 | 13.705 | 13.802 | 69.691 | 0.10x |
| mixed.json | strata | 0.064 | 0.068 | 0.070 | 69.691 | 1.00x |
| mixed.json | orjson | 0.068 | 0.070 | 0.084 | 69.691 | 0.97x |
| mixed.json | msgspec | 0.082 | 0.084 | 0.096 | 69.691 | 0.80x |
| mixed.json | ujson | 0.244 | 0.258 | 0.272 | 69.691 | 0.26x |
| mixed.json | json | 0.489 | 0.495 | 0.514 | 69.691 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.930 | 9.019 | 9.709 | 68.555 | 1.00x |
| users.json | orjson | 11.841 | 12.019 | 12.322 | 68.555 | 0.75x |
| users.json | msgspec | 12.456 | 12.543 | 12.727 | 68.555 | 0.72x |
| users.json | ujson | 17.019 | 17.235 | 18.135 | 68.555 | 0.52x |
| users.json | json | 20.846 | 20.951 | 21.218 | 68.555 | 0.43x |
| flat.json | strata | 0.878 | 0.886 | 0.896 | 68.113 | 1.00x |
| flat.json | orjson | 0.943 | 0.973 | 0.980 | 68.113 | 0.91x |
| flat.json | msgspec | 1.013 | 1.023 | 1.030 | 68.113 | 0.87x |
| flat.json | ujson | 1.571 | 1.589 | 1.603 | 68.113 | 0.56x |
| flat.json | json | 1.859 | 1.866 | 1.882 | 68.113 | 0.47x |
| nested.json | strata | 0.852 | 0.875 | 0.892 | 68.113 | 1.00x |
| nested.json | orjson | 0.961 | 0.975 | 0.988 | 68.113 | 0.90x |
| nested.json | msgspec | 1.074 | 1.093 | 1.108 | 68.113 | 0.80x |
| nested.json | ujson | 1.517 | 1.535 | 1.566 | 68.113 | 0.57x |
| nested.json | json | 2.042 | 2.064 | 2.086 | 68.113 | 0.42x |
| wide_arrays.json | strata | 3.914 | 3.951 | 4.031 | 69.691 | 1.00x |
| wide_arrays.json | orjson | 4.206 | 4.364 | 4.641 | 69.691 | 0.91x |
| wide_arrays.json | msgspec | 5.174 | 5.300 | 5.684 | 69.691 | 0.75x |
| wide_arrays.json | ujson | 6.731 | 6.927 | 7.246 | 69.691 | 0.57x |
| wide_arrays.json | json | 9.651 | 9.740 | 9.896 | 69.691 | 0.41x |
| mixed.json | strata | 0.219 | 0.231 | 0.269 | 69.691 | 1.00x |
| mixed.json | orjson | 0.285 | 0.302 | 0.323 | 69.691 | 0.77x |
| mixed.json | msgspec | 0.302 | 0.323 | 0.354 | 69.691 | 0.72x |
| mixed.json | ujson | 0.383 | 0.410 | 0.441 | 69.691 | 0.56x |
| mixed.json | json | 0.521 | 0.533 | 0.578 | 69.691 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.329 | 9.657 | 10.225 | 68.109 | 1.00x |
| users.ndjson | orjson | 14.749 | 15.060 | 15.666 | 68.109 | 0.64x |
| users.ndjson | msgspec | 15.147 | 15.370 | 15.810 | 68.109 | 0.63x |
| users.ndjson | ujson | 19.680 | 20.415 | 21.160 | 68.109 | 0.47x |
| users.ndjson | json | 25.642 | 26.720 | 27.551 | 68.109 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.408 | 2.435 | 2.543 | 68.555 | 1.00x |
| users.json | orjson | 3.097 | 3.166 | 3.285 | 68.555 | 0.77x |
| users.json | msgspec | 3.815 | 3.869 | 4.022 | 68.555 | 0.63x |
| users.json | ujson | 11.090 | 11.223 | 11.316 | 68.555 | 0.22x |
| users.json | json | 19.678 | 19.726 | 19.963 | 68.555 | 0.12x |
| flat.json | strata | 0.411 | 0.426 | 0.453 | 68.113 | 1.00x |
| flat.json | orjson | 0.498 | 0.516 | 0.551 | 68.113 | 0.83x |
| flat.json | msgspec | 0.588 | 0.613 | 0.644 | 68.113 | 0.69x |
| flat.json | ujson | 1.222 | 1.238 | 1.258 | 68.113 | 0.34x |
| flat.json | json | 1.932 | 1.952 | 2.025 | 68.113 | 0.22x |
| nested.json | strata | 0.374 | 0.392 | 0.429 | 68.113 | 1.00x |
| nested.json | orjson | 0.469 | 0.502 | 0.524 | 68.113 | 0.78x |
| nested.json | msgspec | 0.559 | 0.584 | 0.618 | 68.113 | 0.67x |
| nested.json | ujson | 1.336 | 1.346 | 1.365 | 68.113 | 0.29x |
| nested.json | json | 2.371 | 2.414 | 2.468 | 68.113 | 0.16x |
| wide_arrays.json | strata | 1.791 | 1.864 | 2.184 | 69.691 | 1.00x |
| wide_arrays.json | orjson | 2.096 | 2.149 | 2.476 | 69.691 | 0.87x |
| wide_arrays.json | msgspec | 2.869 | 2.934 | 3.269 | 69.691 | 0.64x |
| wide_arrays.json | ujson | 5.337 | 5.412 | 5.812 | 69.691 | 0.34x |
| wide_arrays.json | json | 14.181 | 14.246 | 14.592 | 69.691 | 0.13x |
| mixed.json | strata | 0.198 | 0.214 | 0.247 | 69.691 | 1.00x |
| mixed.json | orjson | 0.223 | 0.239 | 0.265 | 69.691 | 0.90x |
| mixed.json | msgspec | 0.236 | 0.244 | 0.261 | 69.691 | 0.88x |
| mixed.json | ujson | 0.423 | 0.437 | 0.472 | 69.691 | 0.49x |
| mixed.json | json | 0.657 | 0.686 | 0.704 | 69.691 | 0.31x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.109 | 0.113 | 0.129 | 68.555 | 1.00x |
| users.json $[*].id | jmespath | 0.479 | 0.497 | 0.510 | 68.555 | 0.23x |
| users.json $[*].id | jsonpath-ng | 2.506 | 2.583 | 2.618 | 68.555 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.653 | 0.680 | 0.692 | 68.680 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.055 | 3.064 | 3.087 | 68.680 | 0.22x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.897 | 18.055 | 18.608 | 68.680 | 0.04x |
| users.json $..total | strata | 1.747 | 1.775 | 1.825 | 69.688 | 1.00x |
| users.json $..total | jsonpath-ng | 296.312 | 296.666 | 296.809 | 69.688 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.186 | 3.207 | 3.235 | 68.680 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.586 | 12.675 | 12.763 | 68.680 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 14.564 | 14.653 | 14.758 | 68.680 | 0.22x |
| users.json $[*].orders[*].total | strata | 3.365 | 3.419 | 3.472 | 69.688 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.328 | 15.543 | 16.615 | 69.688 | 0.22x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 34.381 | 34.690 | 37.336 | 69.688 | 0.10x |
| users.json $..total | strata | 11.466 | 11.651 | 12.213 | 69.746 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 311.547 | 313.342 | 314.883 | 69.746 | 0.04x |

