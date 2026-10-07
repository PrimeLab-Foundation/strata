# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c16eaa65c61f1cf9d4fac46e9f6d8e6b9f3b087f
- python: 3.12.10
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit
- machine: arm64
- processor: Apple M1 (Virtual)
- compiler_flags: -fno-strict-overflow -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -O3 -Wall -arch arm64 -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -march=native -flto=thin -fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata -bundle -undefined (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.134 | 6.583 | 8.781 | 67.891 | 1.00x |
| users.json | orjson | 8.804 | 9.974 | 13.802 | 67.891 | 0.66x |
| users.json | msgspec | 8.546 | 9.331 | 13.268 | 67.891 | 0.71x |
| users.json | ujson | 11.966 | 13.325 | 16.387 | 67.891 | 0.49x |
| users.json | pysimdjson | 119.746 | 130.754 | 164.893 | 67.891 | 0.05x |
| users.json | json | 14.413 | 15.758 | 25.045 | 67.891 | 0.42x |
| flat.json | strata | 0.559 | 0.561 | 0.611 | 101.125 | 1.00x |
| flat.json | orjson | 0.683 | 0.712 | 0.796 | 101.125 | 0.79x |
| flat.json | msgspec | 0.666 | 0.672 | 0.781 | 101.125 | 0.84x |
| flat.json | ujson | 1.046 | 1.100 | 1.176 | 101.125 | 0.51x |
| flat.json | pysimdjson | 11.114 | 11.223 | 11.423 | 101.125 | 0.05x |
| flat.json | json | 1.290 | 1.300 | 1.486 | 101.125 | 0.43x |
| nested.json | strata | 0.480 | 0.560 | 0.650 | 101.125 | 1.00x |
| nested.json | orjson | 0.677 | 0.795 | 0.850 | 101.125 | 0.70x |
| nested.json | msgspec | 0.646 | 0.745 | 0.835 | 101.125 | 0.75x |
| nested.json | ujson | 1.093 | 1.229 | 1.667 | 101.125 | 0.46x |
| nested.json | pysimdjson | 9.728 | 10.762 | 11.286 | 101.125 | 0.05x |
| nested.json | json | 1.313 | 1.521 | 1.830 | 101.125 | 0.37x |
| wide_arrays.json | strata | 2.790 | 3.294 | 3.975 | 103.891 | 1.00x |
| wide_arrays.json | orjson | 3.338 | 3.978 | 4.475 | 103.891 | 0.83x |
| wide_arrays.json | msgspec | 3.775 | 4.501 | 6.903 | 103.891 | 0.73x |
| wide_arrays.json | ujson | 4.963 | 5.936 | 7.032 | 103.891 | 0.55x |
| wide_arrays.json | pysimdjson | 60.849 | 67.670 | 72.734 | 103.891 | 0.05x |
| wide_arrays.json | json | 6.508 | 7.377 | 8.468 | 103.891 | 0.45x |
| mixed.json | strata | 0.119 | 0.124 | 0.139 | 104.422 | 1.00x |
| mixed.json | orjson | 0.150 | 0.165 | 0.185 | 104.422 | 0.75x |
| mixed.json | msgspec | 0.166 | 0.177 | 0.202 | 104.422 | 0.70x |
| mixed.json | ujson | 0.204 | 0.366 | 0.457 | 104.422 | 0.34x |
| mixed.json | pysimdjson | 2.437 | 2.510 | 2.650 | 104.422 | 0.05x |
| mixed.json | json | 0.317 | 0.326 | 0.442 | 104.422 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.375 | 1.646 | 2.086 | 85.281 | 1.00x |
| users.json | orjson | 2.088 | 2.404 | 3.105 | 85.281 | 0.68x |
| users.json | msgspec | 2.726 | 3.038 | 5.190 | 85.281 | 0.54x |
| users.json | ujson | 8.421 | 8.796 | 12.045 | 85.281 | 0.19x |
| users.json | json | 15.092 | 15.479 | 22.725 | 85.281 | 0.11x |
| flat.json | strata | 0.189 | 0.192 | 0.216 | 101.125 | 1.00x |
| flat.json | orjson | 0.232 | 0.235 | 0.258 | 101.125 | 0.81x |
| flat.json | msgspec | 0.291 | 0.341 | 0.539 | 101.125 | 0.56x |
| flat.json | ujson | 0.705 | 0.715 | 0.763 | 101.125 | 0.27x |
| flat.json | json | 1.263 | 1.349 | 1.397 | 101.125 | 0.14x |
| nested.json | strata | 0.114 | 0.115 | 0.135 | 101.125 | 1.00x |
| nested.json | orjson | 0.205 | 0.208 | 0.441 | 101.125 | 0.55x |
| nested.json | msgspec | 0.322 | 0.327 | 0.408 | 101.125 | 0.35x |
| nested.json | ujson | 0.799 | 0.808 | 1.034 | 101.125 | 0.14x |
| nested.json | json | 1.536 | 1.560 | 1.953 | 101.125 | 0.07x |
| wide_arrays.json | strata | 1.088 | 1.229 | 1.769 | 103.891 | 1.00x |
| wide_arrays.json | orjson | 1.437 | 1.574 | 1.767 | 103.891 | 0.78x |
| wide_arrays.json | msgspec | 2.244 | 2.320 | 2.467 | 103.891 | 0.53x |
| wide_arrays.json | ujson | 5.071 | 5.183 | 5.515 | 103.891 | 0.24x |
| wide_arrays.json | json | 12.317 | 12.529 | 12.778 | 103.891 | 0.10x |
| mixed.json | strata | 0.039 | 0.047 | 0.170 | 104.422 | 1.00x |
| mixed.json | orjson | 0.049 | 0.054 | 0.173 | 104.422 | 0.87x |
| mixed.json | msgspec | 0.057 | 0.062 | 0.226 | 104.422 | 0.75x |
| mixed.json | ujson | 0.178 | 0.184 | 0.220 | 104.422 | 0.26x |
| mixed.json | json | 0.362 | 0.380 | 0.397 | 104.422 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.124 | 6.423 | 8.877 | 95.344 | 1.00x |
| users.json | orjson | 8.926 | 9.236 | 10.917 | 95.344 | 0.70x |
| users.json | msgspec | 8.710 | 8.986 | 11.147 | 95.344 | 0.71x |
| users.json | ujson | 11.886 | 12.169 | 15.527 | 95.344 | 0.53x |
| users.json | json | 14.034 | 14.356 | 18.907 | 95.344 | 0.45x |
| flat.json | strata | 0.588 | 0.606 | 0.710 | 101.125 | 1.00x |
| flat.json | orjson | 0.763 | 0.855 | 1.331 | 101.125 | 0.71x |
| flat.json | msgspec | 0.724 | 0.769 | 2.389 | 101.125 | 0.79x |
| flat.json | ujson | 1.061 | 1.137 | 1.658 | 101.125 | 0.53x |
| flat.json | json | 1.322 | 1.353 | 1.641 | 101.125 | 0.45x |
| nested.json | strata | 0.512 | 0.592 | 0.697 | 101.125 | 1.00x |
| nested.json | orjson | 0.772 | 0.924 | 1.055 | 101.125 | 0.64x |
| nested.json | msgspec | 0.694 | 0.826 | 0.874 | 101.125 | 0.72x |
| nested.json | ujson | 0.965 | 1.103 | 1.254 | 101.125 | 0.54x |
| nested.json | json | 1.351 | 1.533 | 2.004 | 101.125 | 0.39x |
| wide_arrays.json | strata | 3.212 | 3.340 | 4.113 | 103.891 | 1.00x |
| wide_arrays.json | orjson | 3.819 | 4.055 | 4.493 | 103.891 | 0.82x |
| wide_arrays.json | msgspec | 4.492 | 4.649 | 4.912 | 103.891 | 0.72x |
| wide_arrays.json | ujson | 5.818 | 6.032 | 6.911 | 103.891 | 0.55x |
| wide_arrays.json | json | 7.416 | 7.493 | 8.478 | 103.891 | 0.45x |
| mixed.json | strata | 0.178 | 0.187 | 0.211 | 104.422 | 1.00x |
| mixed.json | orjson | 0.251 | 0.427 | 0.571 | 104.422 | 0.44x |
| mixed.json | msgspec | 0.263 | 0.281 | 0.499 | 104.422 | 0.66x |
| mixed.json | ujson | 0.293 | 0.339 | 0.573 | 104.422 | 0.55x |
| mixed.json | json | 0.414 | 0.436 | 0.464 | 104.422 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.313 | 7.358 | 7.702 | 101.125 | 1.00x |
| users.ndjson | orjson | 10.869 | 12.762 | 13.433 | 101.125 | 0.58x |
| users.ndjson | msgspec | 10.677 | 12.611 | 13.063 | 101.125 | 0.58x |
| users.ndjson | ujson | 13.323 | 15.594 | 15.985 | 101.125 | 0.47x |
| users.ndjson | json | 17.143 | 20.101 | 21.667 | 101.125 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.685 | 1.794 | 1.898 | 95.359 | 1.00x |
| users.json | orjson | 2.555 | 2.677 | 2.845 | 95.359 | 0.67x |
| users.json | msgspec | 3.211 | 3.300 | 3.469 | 95.359 | 0.54x |
| users.json | ujson | 8.859 | 8.978 | 10.773 | 95.359 | 0.20x |
| users.json | json | 15.356 | 15.785 | 17.048 | 95.359 | 0.11x |
| flat.json | strata | 0.289 | 0.305 | 0.564 | 101.125 | 1.00x |
| flat.json | orjson | 0.338 | 0.375 | 0.450 | 101.125 | 0.82x |
| flat.json | msgspec | 0.400 | 0.417 | 0.608 | 101.125 | 0.73x |
| flat.json | ujson | 0.837 | 0.848 | 1.069 | 101.125 | 0.36x |
| flat.json | json | 1.393 | 1.446 | 1.632 | 101.125 | 0.21x |
| nested.json | strata | 0.233 | 0.260 | 0.350 | 101.125 | 1.00x |
| nested.json | orjson | 0.326 | 0.387 | 0.459 | 101.125 | 0.67x |
| nested.json | msgspec | 0.396 | 0.594 | 0.808 | 101.125 | 0.44x |
| nested.json | ujson | 0.969 | 1.152 | 1.197 | 101.125 | 0.23x |
| nested.json | json | 1.678 | 1.709 | 2.149 | 101.125 | 0.15x |
| wide_arrays.json | strata | 1.357 | 1.409 | 1.687 | 104.406 | 1.00x |
| wide_arrays.json | orjson | 1.652 | 1.869 | 2.274 | 104.406 | 0.75x |
| wide_arrays.json | msgspec | 2.618 | 2.787 | 3.181 | 104.406 | 0.51x |
| wide_arrays.json | ujson | 5.148 | 5.399 | 6.147 | 104.406 | 0.26x |
| wide_arrays.json | json | 11.975 | 12.701 | 13.927 | 104.406 | 0.11x |
| mixed.json | strata | 0.169 | 0.203 | 0.321 | 104.422 | 1.00x |
| mixed.json | orjson | 0.197 | 0.254 | 0.380 | 104.422 | 0.80x |
| mixed.json | msgspec | 0.200 | 0.372 | 0.468 | 104.422 | 0.55x |
| mixed.json | ujson | 0.370 | 0.436 | 0.577 | 104.422 | 0.47x |
| mixed.json | json | 0.564 | 0.613 | 0.854 | 104.422 | 0.33x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.047 | 0.053 | 0.075 | 95.438 | 1.00x |
| users.json $[*].id | jmespath | 0.257 | 0.260 | 0.273 | 95.438 | 0.20x |
| users.json $[*].id | jsonpath-ng | 1.396 | 1.420 | 1.510 | 95.438 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.417 | 0.525 | 0.649 | 95.578 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.903 | 1.954 | 2.296 | 95.578 | 0.27x |
| users.json $[*].orders[*].total | jsonpath-ng | 11.711 | 12.258 | 13.139 | 95.578 | 0.04x |
| users.json $..total | strata | 1.331 | 1.461 | 1.586 | 95.594 | 1.00x |
| users.json $..total | jsonpath-ng | 205.322 | 207.674 | 211.904 | 95.594 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.350 | 3.642 | 3.866 | 95.484 | 1.00x |
| users.json $[*].id | orjson+jmespath | 9.189 | 10.439 | 11.546 | 95.484 | 0.35x |
| users.json $[*].id | orjson+jsonpath-ng | 10.405 | 12.596 | 13.321 | 95.484 | 0.29x |
| users.json $[*].orders[*].total | strata | 3.421 | 3.908 | 4.053 | 95.594 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 10.639 | 12.864 | 18.019 | 95.594 | 0.30x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 20.829 | 22.293 | 27.372 | 95.594 | 0.18x |
| users.json $..total | strata | 8.182 | 9.221 | 9.995 | 95.625 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 211.879 | 221.631 | 256.343 | 95.625 | 0.04x |

