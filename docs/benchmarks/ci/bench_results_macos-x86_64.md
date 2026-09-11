# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c20ac86eedff410e10c973bc3b1f19f6e9a5f56e
- python: 3.12.10
- implementation: CPython
- platform: macOS-15.7.9-x86_64-i386-64bit
- machine: x86_64
- processor: Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz
- compiler_flags: -fno-strict-overflow -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -O3 -Wall -arch x86_64 -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -fomit-frame-pointer -march=native -flto=thin -fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata -bundle -undefined (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 16.642 | 18.124 | 23.236 | 57.234 | 1.00x |
| users.json | orjson | 23.848 | 27.982 | 32.962 | 57.234 | 0.65x |
| users.json | msgspec | 25.071 | 27.041 | 34.246 | 57.234 | 0.67x |
| users.json | ujson | 36.157 | 38.228 | 41.529 | 57.234 | 0.47x |
| users.json | pysimdjson | 154.701 | 158.652 | 191.897 | 57.234 | 0.11x |
| users.json | json | 39.694 | 44.109 | 58.704 | 57.234 | 0.41x |
| flat.json | strata | 1.172 | 1.214 | 1.639 | 67.355 | 1.00x |
| flat.json | orjson | 1.323 | 1.357 | 1.847 | 67.355 | 0.89x |
| flat.json | msgspec | 1.500 | 1.542 | 2.114 | 67.355 | 0.79x |
| flat.json | ujson | 2.639 | 2.790 | 3.605 | 67.355 | 0.44x |
| flat.json | pysimdjson | 13.936 | 14.187 | 15.437 | 67.355 | 0.09x |
| flat.json | json | 3.031 | 3.076 | 3.423 | 67.355 | 0.39x |
| nested.json | strata | 1.400 | 1.446 | 1.513 | 65.840 | 1.00x |
| nested.json | orjson | 1.606 | 1.650 | 1.826 | 65.840 | 0.88x |
| nested.json | msgspec | 1.779 | 1.849 | 1.994 | 65.840 | 0.78x |
| nested.json | ujson | 2.939 | 3.028 | 3.266 | 65.840 | 0.48x |
| nested.json | pysimdjson | 12.806 | 13.062 | 19.031 | 65.840 | 0.11x |
| nested.json | json | 3.677 | 3.865 | 4.290 | 65.840 | 0.37x |
| wide_arrays.json | strata | 7.041 | 7.229 | 7.647 | 71.809 | 1.00x |
| wide_arrays.json | orjson | 8.969 | 9.572 | 12.206 | 71.809 | 0.76x |
| wide_arrays.json | msgspec | 9.762 | 10.172 | 10.791 | 71.809 | 0.71x |
| wide_arrays.json | ujson | 12.229 | 12.888 | 13.659 | 71.809 | 0.56x |
| wide_arrays.json | pysimdjson | 74.764 | 76.014 | 78.991 | 71.809 | 0.10x |
| wide_arrays.json | json | 15.972 | 16.600 | 17.327 | 71.809 | 0.44x |
| mixed.json | strata | 0.339 | 0.413 | 0.513 | 64.602 | 1.00x |
| mixed.json | orjson | 0.427 | 0.445 | 0.611 | 64.602 | 0.93x |
| mixed.json | msgspec | 0.435 | 0.461 | 0.642 | 64.602 | 0.90x |
| mixed.json | ujson | 0.609 | 0.710 | 0.875 | 64.602 | 0.58x |
| mixed.json | pysimdjson | 3.056 | 3.664 | 4.037 | 64.602 | 0.11x |
| mixed.json | json | 0.858 | 0.868 | 1.266 | 64.602 | 0.48x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.345 | 2.674 | 2.798 | 52.520 | 1.00x |
| users.json | orjson | 3.153 | 3.398 | 4.455 | 52.520 | 0.79x |
| users.json | msgspec | 4.916 | 5.248 | 8.073 | 52.520 | 0.51x |
| users.json | ujson | 23.353 | 23.668 | 26.164 | 52.520 | 0.11x |
| users.json | json | 39.497 | 40.180 | 45.416 | 52.520 | 0.07x |
| flat.json | strata | 0.322 | 0.336 | 0.345 | 65.879 | 1.00x |
| flat.json | orjson | 0.399 | 0.411 | 0.432 | 65.879 | 0.82x |
| flat.json | msgspec | 0.534 | 0.555 | 0.593 | 65.879 | 0.61x |
| flat.json | ujson | 2.126 | 2.147 | 2.243 | 65.879 | 0.16x |
| flat.json | json | 3.448 | 3.496 | 3.630 | 65.879 | 0.10x |
| nested.json | strata | 0.212 | 0.235 | 0.270 | 65.977 | 1.00x |
| nested.json | orjson | 0.320 | 0.337 | 0.415 | 65.977 | 0.70x |
| nested.json | msgspec | 0.508 | 0.533 | 0.577 | 65.977 | 0.44x |
| nested.json | ujson | 2.161 | 2.192 | 2.266 | 65.977 | 0.11x |
| nested.json | json | 4.302 | 4.353 | 4.692 | 65.977 | 0.05x |
| wide_arrays.json | strata | 1.772 | 2.050 | 2.712 | 65.535 | 1.00x |
| wide_arrays.json | orjson | 2.313 | 2.627 | 3.453 | 65.535 | 0.78x |
| wide_arrays.json | msgspec | 3.441 | 3.618 | 4.390 | 65.535 | 0.57x |
| wide_arrays.json | ujson | 10.296 | 10.640 | 12.228 | 65.535 | 0.19x |
| wide_arrays.json | json | 32.218 | 33.229 | 37.973 | 65.535 | 0.06x |
| mixed.json | strata | 0.069 | 0.082 | 0.107 | 61.402 | 1.00x |
| mixed.json | orjson | 0.090 | 0.115 | 0.158 | 61.402 | 0.71x |
| mixed.json | msgspec | 0.115 | 0.147 | 0.183 | 61.402 | 0.56x |
| mixed.json | ujson | 0.452 | 0.553 | 0.600 | 61.402 | 0.15x |
| mixed.json | json | 0.915 | 1.095 | 1.433 | 61.402 | 0.07x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 16.823 | 17.392 | 18.786 | 62.551 | 1.00x |
| users.json | orjson | 23.878 | 26.895 | 66.022 | 62.551 | 0.65x |
| users.json | msgspec | 24.452 | 25.664 | 55.601 | 62.551 | 0.68x |
| users.json | ujson | 35.561 | 38.234 | 55.403 | 62.551 | 0.45x |
| users.json | json | 40.053 | 42.635 | 64.650 | 62.551 | 0.41x |
| flat.json | strata | 1.313 | 1.440 | 1.908 | 65.879 | 1.00x |
| flat.json | orjson | 1.516 | 1.644 | 1.895 | 65.879 | 0.88x |
| flat.json | msgspec | 1.718 | 1.782 | 2.333 | 65.879 | 0.81x |
| flat.json | ujson | 2.842 | 2.963 | 3.745 | 65.879 | 0.49x |
| flat.json | json | 3.161 | 3.435 | 3.957 | 65.879 | 0.42x |
| nested.json | strata | 1.517 | 1.600 | 2.003 | 65.977 | 1.00x |
| nested.json | orjson | 1.749 | 1.905 | 2.491 | 65.977 | 0.84x |
| nested.json | msgspec | 1.950 | 2.260 | 2.765 | 65.977 | 0.71x |
| nested.json | ujson | 3.083 | 3.384 | 4.261 | 65.977 | 0.47x |
| nested.json | json | 3.904 | 4.268 | 4.921 | 65.977 | 0.37x |
| wide_arrays.json | strata | 7.019 | 7.199 | 10.220 | 66.766 | 1.00x |
| wide_arrays.json | orjson | 9.322 | 9.705 | 12.025 | 66.766 | 0.74x |
| wide_arrays.json | msgspec | 10.090 | 10.915 | 15.539 | 66.766 | 0.66x |
| wide_arrays.json | ujson | 12.758 | 13.483 | 16.187 | 66.766 | 0.53x |
| wide_arrays.json | json | 16.472 | 16.961 | 22.216 | 66.766 | 0.42x |
| mixed.json | strata | 0.457 | 0.561 | 1.242 | 61.402 | 1.00x |
| mixed.json | orjson | 0.584 | 0.766 | 1.016 | 61.402 | 0.73x |
| mixed.json | msgspec | 0.620 | 0.838 | 1.238 | 61.402 | 0.67x |
| mixed.json | ujson | 0.762 | 0.836 | 1.524 | 61.402 | 0.67x |
| mixed.json | json | 0.982 | 1.607 | 1.939 | 61.402 | 0.35x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 17.159 | 18.147 | 22.251 | 66.934 | 1.00x |
| users.ndjson | orjson | 25.132 | 26.607 | 32.989 | 66.934 | 0.68x |
| users.ndjson | msgspec | 25.544 | 27.349 | 34.308 | 66.934 | 0.66x |
| users.ndjson | ujson | 36.650 | 39.897 | 45.233 | 66.934 | 0.45x |
| users.ndjson | json | 46.922 | 48.801 | 59.620 | 66.934 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.138 | 3.414 | 3.975 | 62.578 | 1.00x |
| users.json | orjson | 4.185 | 4.397 | 4.944 | 62.578 | 0.78x |
| users.json | msgspec | 5.859 | 6.083 | 6.690 | 62.578 | 0.56x |
| users.json | ujson | 24.747 | 24.957 | 26.848 | 62.578 | 0.14x |
| users.json | json | 40.077 | 41.158 | 48.371 | 62.578 | 0.08x |
| flat.json | strata | 0.644 | 0.692 | 0.836 | 65.879 | 1.00x |
| flat.json | orjson | 0.716 | 0.786 | 0.932 | 65.879 | 0.88x |
| flat.json | msgspec | 0.868 | 0.957 | 1.060 | 65.879 | 0.72x |
| flat.json | ujson | 2.457 | 2.524 | 2.906 | 65.879 | 0.27x |
| flat.json | json | 3.850 | 3.993 | 4.070 | 65.879 | 0.17x |
| nested.json | strata | 0.562 | 0.659 | 0.747 | 65.977 | 1.00x |
| nested.json | orjson | 0.732 | 0.837 | 0.957 | 65.977 | 0.79x |
| nested.json | msgspec | 0.913 | 1.002 | 1.469 | 65.977 | 0.66x |
| nested.json | ujson | 2.649 | 3.016 | 3.991 | 65.977 | 0.22x |
| nested.json | json | 4.820 | 5.365 | 7.570 | 65.977 | 0.12x |
| wide_arrays.json | strata | 2.574 | 2.940 | 3.839 | 65.535 | 1.00x |
| wide_arrays.json | orjson | 3.186 | 3.603 | 4.830 | 65.535 | 0.82x |
| wide_arrays.json | msgspec | 4.365 | 4.570 | 6.132 | 65.535 | 0.64x |
| wide_arrays.json | ujson | 11.612 | 12.716 | 16.127 | 65.535 | 0.23x |
| wide_arrays.json | json | 33.443 | 40.675 | 70.609 | 65.535 | 0.07x |
| mixed.json | strata | 0.293 | 0.368 | 0.394 | 61.402 | 1.00x |
| mixed.json | orjson | 0.374 | 0.402 | 0.634 | 61.402 | 0.92x |
| mixed.json | msgspec | 0.400 | 0.433 | 0.525 | 61.402 | 0.85x |
| mixed.json | ujson | 0.761 | 0.773 | 0.846 | 61.402 | 0.48x |
| mixed.json | json | 1.224 | 1.273 | 1.316 | 61.402 | 0.29x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.133 | 0.146 | 0.269 | 62.645 | 1.00x |
| users.json $[*].id | jmespath | 0.895 | 0.950 | 1.011 | 62.645 | 0.15x |
| users.json $[*].id | jsonpath-ng | 4.900 | 5.188 | 5.716 | 62.645 | 0.03x |
| users.json $[*].orders[*].total | strata | 1.072 | 1.224 | 1.976 | 59.816 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 6.022 | 6.421 | 7.912 | 59.816 | 0.19x |
| users.json $[*].orders[*].total | jsonpath-ng | 36.065 | 37.607 | 45.636 | 59.816 | 0.03x |
| users.json $..total | strata | 3.177 | 3.447 | 4.400 | 59.961 | 1.00x |
| users.json $..total | jsonpath-ng | 668.583 | 686.290 | 758.547 | 59.961 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.688 | 4.106 | 4.288 | 62.715 | 1.00x |
| users.json $[*].id | orjson+jmespath | 26.856 | 30.131 | 37.674 | 62.715 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 32.188 | 35.274 | 43.371 | 62.715 | 0.12x |
| users.json $[*].orders[*].total | strata | 4.000 | 4.234 | 6.512 | 59.867 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 29.642 | 33.984 | 40.953 | 59.867 | 0.12x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 65.606 | 73.661 | 78.338 | 59.867 | 0.06x |
| users.json $..total | strata | 21.339 | 21.457 | 29.157 | 59.988 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 697.054 | 711.851 | 813.482 | 59.988 | 0.03x |

