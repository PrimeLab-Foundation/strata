# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c89aea890fd23de6c299c86e4099ab38c8418300
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
| users.json | strata | 5.993 | 6.829 | 8.013 | 68.047 | 1.00x |
| users.json | orjson | 9.380 | 10.685 | 11.675 | 68.047 | 0.64x |
| users.json | msgspec | 8.738 | 9.854 | 10.561 | 68.047 | 0.69x |
| users.json | ujson | 12.034 | 13.639 | 15.327 | 68.047 | 0.50x |
| users.json | pysimdjson | 125.087 | 127.583 | 136.954 | 68.047 | 0.05x |
| users.json | json | 14.922 | 16.197 | 18.310 | 68.047 | 0.42x |
| flat.json | strata | 0.552 | 0.570 | 0.606 | 97.219 | 1.00x |
| flat.json | orjson | 0.685 | 0.725 | 0.781 | 97.219 | 0.79x |
| flat.json | msgspec | 0.684 | 0.719 | 0.763 | 97.219 | 0.79x |
| flat.json | ujson | 1.043 | 1.088 | 1.186 | 97.219 | 0.52x |
| flat.json | pysimdjson | 11.530 | 11.775 | 12.122 | 97.219 | 0.05x |
| flat.json | json | 1.286 | 1.338 | 1.373 | 97.219 | 0.43x |
| nested.json | strata | 0.539 | 0.587 | 0.657 | 97.234 | 1.00x |
| nested.json | orjson | 0.754 | 0.834 | 0.878 | 97.234 | 0.70x |
| nested.json | msgspec | 0.723 | 0.774 | 0.830 | 97.234 | 0.76x |
| nested.json | ujson | 1.068 | 1.192 | 1.320 | 97.234 | 0.49x |
| nested.json | pysimdjson | 10.994 | 11.376 | 11.632 | 97.234 | 0.05x |
| nested.json | json | 1.504 | 1.594 | 1.757 | 97.234 | 0.37x |
| wide_arrays.json | strata | 2.902 | 3.113 | 3.171 | 101.047 | 1.00x |
| wide_arrays.json | orjson | 3.511 | 3.894 | 3.991 | 101.047 | 0.80x |
| wide_arrays.json | msgspec | 3.929 | 4.218 | 4.330 | 101.047 | 0.74x |
| wide_arrays.json | ujson | 5.105 | 5.492 | 5.657 | 101.047 | 0.57x |
| wide_arrays.json | pysimdjson | 62.234 | 65.191 | 66.280 | 101.047 | 0.05x |
| wide_arrays.json | json | 6.615 | 7.063 | 7.317 | 101.047 | 0.44x |
| mixed.json | strata | 0.128 | 0.133 | 0.152 | 101.078 | 1.00x |
| mixed.json | orjson | 0.164 | 0.167 | 0.188 | 101.078 | 0.79x |
| mixed.json | msgspec | 0.175 | 0.178 | 0.187 | 101.078 | 0.75x |
| mixed.json | ujson | 0.216 | 0.246 | 0.503 | 101.078 | 0.54x |
| mixed.json | pysimdjson | 2.527 | 2.611 | 2.751 | 101.078 | 0.05x |
| mixed.json | json | 0.334 | 0.348 | 0.498 | 101.078 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.432 | 1.583 | 1.795 | 80.719 | 1.00x |
| users.json | orjson | 2.272 | 2.386 | 2.514 | 80.719 | 0.66x |
| users.json | msgspec | 2.891 | 3.068 | 3.149 | 80.719 | 0.52x |
| users.json | ujson | 8.687 | 9.237 | 48.645 | 80.719 | 0.17x |
| users.json | json | 15.246 | 16.309 | 293.662 | 80.719 | 0.10x |
| flat.json | strata | 0.214 | 0.216 | 0.245 | 97.234 | 1.00x |
| flat.json | orjson | 0.319 | 0.330 | 0.410 | 97.234 | 0.65x |
| flat.json | msgspec | 0.335 | 0.340 | 0.360 | 97.234 | 0.63x |
| flat.json | ujson | 0.780 | 0.789 | 0.805 | 97.234 | 0.27x |
| flat.json | json | 1.474 | 1.506 | 1.590 | 97.234 | 0.14x |
| nested.json | strata | 0.139 | 0.150 | 0.167 | 97.250 | 1.00x |
| nested.json | orjson | 0.237 | 0.263 | 0.344 | 97.250 | 0.57x |
| nested.json | msgspec | 0.391 | 0.440 | 0.517 | 97.250 | 0.34x |
| nested.json | ujson | 0.852 | 0.945 | 1.161 | 97.250 | 0.16x |
| nested.json | json | 1.784 | 1.850 | 2.195 | 97.250 | 0.08x |
| wide_arrays.json | strata | 1.253 | 1.294 | 1.353 | 101.047 | 1.00x |
| wide_arrays.json | orjson | 1.437 | 1.511 | 1.587 | 101.047 | 0.86x |
| wide_arrays.json | msgspec | 2.284 | 2.370 | 2.468 | 101.047 | 0.55x |
| wide_arrays.json | ujson | 4.870 | 5.100 | 5.167 | 101.047 | 0.25x |
| wide_arrays.json | json | 11.438 | 12.047 | 12.266 | 101.047 | 0.11x |
| mixed.json | strata | 0.036 | 0.041 | 0.043 | 101.078 | 1.00x |
| mixed.json | orjson | 0.046 | 0.050 | 0.061 | 101.078 | 0.82x |
| mixed.json | msgspec | 0.053 | 0.057 | 0.059 | 101.078 | 0.71x |
| mixed.json | ujson | 0.175 | 0.177 | 0.196 | 101.078 | 0.23x |
| mixed.json | json | 0.356 | 0.369 | 0.398 | 101.078 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.680 | 7.065 | 8.713 | 90.734 | 1.00x |
| users.json | orjson | 9.732 | 10.861 | 13.340 | 90.734 | 0.65x |
| users.json | msgspec | 9.663 | 10.143 | 11.440 | 90.734 | 0.70x |
| users.json | ujson | 12.280 | 14.253 | 29.169 | 90.734 | 0.50x |
| users.json | json | 15.736 | 16.349 | 20.538 | 90.734 | 0.43x |
| flat.json | strata | 0.624 | 0.689 | 0.704 | 97.234 | 1.00x |
| flat.json | orjson | 0.906 | 0.958 | 1.064 | 97.234 | 0.72x |
| flat.json | msgspec | 0.789 | 0.817 | 1.009 | 97.234 | 0.84x |
| flat.json | ujson | 1.134 | 1.183 | 1.370 | 97.234 | 0.58x |
| flat.json | json | 1.399 | 1.473 | 1.676 | 97.234 | 0.47x |
| nested.json | strata | 0.571 | 0.588 | 0.613 | 97.250 | 1.00x |
| nested.json | orjson | 0.916 | 0.963 | 1.120 | 97.250 | 0.61x |
| nested.json | msgspec | 0.780 | 0.796 | 0.873 | 97.250 | 0.74x |
| nested.json | ujson | 1.065 | 1.094 | 1.155 | 97.250 | 0.54x |
| nested.json | json | 1.526 | 1.546 | 1.590 | 97.250 | 0.38x |
| wide_arrays.json | strata | 3.123 | 3.308 | 3.383 | 101.047 | 1.00x |
| wide_arrays.json | orjson | 3.701 | 4.008 | 4.234 | 101.047 | 0.83x |
| wide_arrays.json | msgspec | 4.295 | 4.490 | 4.576 | 101.047 | 0.74x |
| wide_arrays.json | ujson | 5.506 | 5.801 | 5.836 | 101.047 | 0.57x |
| wide_arrays.json | json | 7.287 | 7.338 | 7.881 | 101.047 | 0.45x |
| mixed.json | strata | 0.160 | 0.164 | 0.192 | 101.078 | 1.00x |
| mixed.json | orjson | 0.223 | 0.332 | 0.470 | 101.078 | 0.49x |
| mixed.json | msgspec | 0.225 | 0.238 | 0.254 | 101.078 | 0.69x |
| mixed.json | ujson | 0.275 | 0.295 | 0.332 | 101.078 | 0.56x |
| mixed.json | json | 0.381 | 0.395 | 0.407 | 101.078 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.624 | 7.012 | 7.297 | 97.219 | 1.00x |
| users.ndjson | orjson | 11.494 | 11.935 | 12.422 | 97.219 | 0.59x |
| users.ndjson | msgspec | 11.363 | 11.897 | 12.339 | 97.219 | 0.59x |
| users.ndjson | ujson | 13.835 | 14.790 | 15.067 | 97.219 | 0.47x |
| users.ndjson | json | 18.096 | 19.388 | 20.157 | 97.219 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.868 | 2.061 | 2.227 | 91.516 | 1.00x |
| users.json | orjson | 2.834 | 2.990 | 3.204 | 91.516 | 0.69x |
| users.json | msgspec | 3.423 | 3.760 | 3.959 | 91.516 | 0.55x |
| users.json | ujson | 9.842 | 10.351 | 10.809 | 91.516 | 0.20x |
| users.json | json | 16.994 | 17.945 | 18.745 | 91.516 | 0.11x |
| flat.json | strata | 0.431 | 0.473 | 0.570 | 97.234 | 1.00x |
| flat.json | orjson | 0.517 | 0.606 | 0.793 | 97.234 | 0.78x |
| flat.json | msgspec | 0.537 | 0.623 | 0.772 | 97.234 | 0.76x |
| flat.json | ujson | 1.010 | 1.157 | 1.292 | 97.234 | 0.41x |
| flat.json | json | 1.754 | 1.851 | 2.062 | 97.234 | 0.26x |
| nested.json | strata | 0.260 | 0.275 | 0.354 | 97.250 | 1.00x |
| nested.json | orjson | 0.362 | 0.374 | 0.428 | 97.250 | 0.73x |
| nested.json | msgspec | 0.440 | 0.555 | 1.081 | 97.250 | 0.49x |
| nested.json | ujson | 0.976 | 1.041 | 1.376 | 97.250 | 0.26x |
| nested.json | json | 1.815 | 1.852 | 1.936 | 97.250 | 0.15x |
| wide_arrays.json | strata | 1.437 | 1.557 | 2.086 | 101.047 | 1.00x |
| wide_arrays.json | orjson | 1.657 | 1.833 | 1.969 | 101.047 | 0.85x |
| wide_arrays.json | msgspec | 2.452 | 2.634 | 3.175 | 101.047 | 0.59x |
| wide_arrays.json | ujson | 5.155 | 5.634 | 6.538 | 101.047 | 0.28x |
| wide_arrays.json | json | 11.837 | 12.556 | 13.337 | 101.047 | 0.12x |
| mixed.json | strata | 0.142 | 0.184 | 0.504 | 101.078 | 1.00x |
| mixed.json | orjson | 0.162 | 0.214 | 0.462 | 101.078 | 0.86x |
| mixed.json | msgspec | 0.171 | 0.207 | 0.970 | 101.078 | 0.89x |
| mixed.json | ujson | 0.303 | 0.326 | 0.797 | 101.078 | 0.57x |
| mixed.json | json | 0.480 | 0.552 | 0.783 | 101.078 | 0.33x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.054 | 0.062 | 0.067 | 91.562 | 1.00x |
| users.json $[*].id | jmespath | 0.288 | 0.295 | 0.302 | 91.562 | 0.21x |
| users.json $[*].id | jsonpath-ng | 1.538 | 1.557 | 1.647 | 91.562 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.342 | 0.351 | 0.655 | 91.719 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.717 | 1.753 | 1.912 | 91.719 | 0.20x |
| users.json $[*].orders[*].total | jsonpath-ng | 10.916 | 11.003 | 11.237 | 91.719 | 0.03x |
| users.json $..total | strata | 1.259 | 1.341 | 1.459 | 91.766 | 1.00x |
| users.json $..total | jsonpath-ng | 193.139 | 197.994 | 200.072 | 91.766 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.496 | 3.697 | 3.790 | 91.641 | 1.00x |
| users.json $[*].id | orjson+jmespath | 9.648 | 10.458 | 10.642 | 91.641 | 0.35x |
| users.json $[*].id | orjson+jsonpath-ng | 10.949 | 11.865 | 12.376 | 91.641 | 0.31x |
| users.json $[*].orders[*].total | strata | 3.598 | 3.817 | 3.950 | 91.750 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 11.115 | 12.111 | 12.320 | 91.750 | 0.32x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 23.164 | 23.821 | 24.428 | 91.750 | 0.16x |
| users.json $..total | strata | 7.842 | 8.576 | 9.523 | 91.797 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 204.557 | 211.912 | 227.502 | 91.797 | 0.04x |

