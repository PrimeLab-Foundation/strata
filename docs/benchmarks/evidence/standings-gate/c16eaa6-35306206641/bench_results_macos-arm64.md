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
| users.json | strata | 6.435 | 6.702 | 7.252 | 69.531 | 1.00x |
| users.json | orjson | 9.536 | 10.147 | 10.818 | 69.531 | 0.66x |
| users.json | msgspec | 9.328 | 9.677 | 10.084 | 69.531 | 0.69x |
| users.json | ujson | 12.508 | 13.165 | 15.176 | 69.531 | 0.51x |
| users.json | pysimdjson | 127.408 | 129.485 | 148.910 | 69.531 | 0.05x |
| users.json | json | 15.440 | 15.937 | 36.175 | 69.531 | 0.42x |
| flat.json | strata | 0.567 | 0.587 | 0.610 | 99.516 | 1.00x |
| flat.json | orjson | 0.718 | 0.760 | 0.807 | 99.516 | 0.77x |
| flat.json | msgspec | 0.674 | 0.714 | 0.730 | 99.516 | 0.82x |
| flat.json | ujson | 1.087 | 1.219 | 1.266 | 99.516 | 0.48x |
| flat.json | pysimdjson | 11.554 | 11.957 | 12.422 | 99.516 | 0.05x |
| flat.json | json | 1.293 | 1.354 | 1.521 | 99.516 | 0.43x |
| nested.json | strata | 0.492 | 0.518 | 0.593 | 99.547 | 1.00x |
| nested.json | orjson | 0.685 | 0.723 | 0.765 | 99.547 | 0.72x |
| nested.json | msgspec | 0.656 | 0.675 | 0.723 | 99.547 | 0.77x |
| nested.json | ujson | 1.021 | 1.136 | 1.204 | 99.547 | 0.46x |
| nested.json | pysimdjson | 10.083 | 10.279 | 10.638 | 99.547 | 0.05x |
| nested.json | json | 1.378 | 1.420 | 1.457 | 99.547 | 0.36x |
| wide_arrays.json | strata | 2.912 | 3.170 | 3.424 | 103.328 | 1.00x |
| wide_arrays.json | orjson | 3.510 | 3.905 | 4.229 | 103.328 | 0.81x |
| wide_arrays.json | msgspec | 3.952 | 4.245 | 4.392 | 103.328 | 0.75x |
| wide_arrays.json | ujson | 5.079 | 5.555 | 6.168 | 103.328 | 0.57x |
| wide_arrays.json | pysimdjson | 62.220 | 65.352 | 65.755 | 103.328 | 0.05x |
| wide_arrays.json | json | 6.621 | 7.104 | 7.395 | 103.328 | 0.45x |
| mixed.json | strata | 0.124 | 0.129 | 0.155 | 106.922 | 1.00x |
| mixed.json | orjson | 0.157 | 0.170 | 0.181 | 106.922 | 0.76x |
| mixed.json | msgspec | 0.173 | 0.176 | 0.190 | 106.922 | 0.73x |
| mixed.json | ujson | 0.217 | 0.309 | 0.480 | 106.922 | 0.42x |
| mixed.json | pysimdjson | 2.555 | 2.592 | 2.795 | 106.922 | 0.05x |
| mixed.json | json | 0.329 | 0.337 | 0.567 | 106.922 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.546 | 1.647 | 1.842 | 83.875 | 1.00x |
| users.json | orjson | 2.392 | 2.632 | 2.949 | 83.875 | 0.63x |
| users.json | msgspec | 2.984 | 3.095 | 3.409 | 83.875 | 0.53x |
| users.json | ujson | 9.183 | 9.430 | 9.951 | 83.875 | 0.17x |
| users.json | json | 16.942 | 17.275 | 18.191 | 83.875 | 0.10x |
| flat.json | strata | 0.223 | 0.226 | 0.238 | 99.531 | 1.00x |
| flat.json | orjson | 0.257 | 0.263 | 0.268 | 99.531 | 0.86x |
| flat.json | msgspec | 0.320 | 0.326 | 0.361 | 99.531 | 0.69x |
| flat.json | ujson | 0.761 | 0.785 | 0.791 | 99.531 | 0.29x |
| flat.json | json | 1.513 | 1.560 | 1.612 | 99.531 | 0.14x |
| nested.json | strata | 0.128 | 0.134 | 0.138 | 99.547 | 1.00x |
| nested.json | orjson | 0.224 | 0.239 | 0.257 | 99.547 | 0.56x |
| nested.json | msgspec | 0.293 | 0.295 | 0.313 | 99.547 | 0.45x |
| nested.json | ujson | 0.824 | 1.007 | 1.133 | 99.547 | 0.13x |
| nested.json | json | 1.691 | 1.734 | 1.860 | 99.547 | 0.08x |
| wide_arrays.json | strata | 1.151 | 1.251 | 1.475 | 106.578 | 1.00x |
| wide_arrays.json | orjson | 1.501 | 1.528 | 1.597 | 106.578 | 0.82x |
| wide_arrays.json | msgspec | 2.237 | 2.377 | 2.453 | 106.578 | 0.53x |
| wide_arrays.json | ujson | 5.025 | 5.138 | 5.586 | 106.578 | 0.24x |
| wide_arrays.json | json | 12.152 | 12.358 | 12.558 | 106.578 | 0.10x |
| mixed.json | strata | 0.036 | 0.037 | 0.042 | 106.922 | 1.00x |
| mixed.json | orjson | 0.046 | 0.047 | 0.053 | 106.922 | 0.79x |
| mixed.json | msgspec | 0.052 | 0.052 | 0.301 | 106.922 | 0.70x |
| mixed.json | ujson | 0.171 | 0.177 | 0.193 | 106.922 | 0.21x |
| mixed.json | json | 0.360 | 0.370 | 0.384 | 106.922 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.085 | 7.293 | 8.115 | 93.906 | 1.00x |
| users.json | orjson | 10.153 | 11.064 | 11.847 | 93.906 | 0.66x |
| users.json | msgspec | 9.692 | 11.005 | 11.397 | 93.906 | 0.66x |
| users.json | ujson | 13.115 | 15.497 | 17.228 | 93.906 | 0.47x |
| users.json | json | 15.578 | 17.871 | 18.728 | 93.906 | 0.41x |
| flat.json | strata | 0.639 | 0.655 | 0.703 | 99.531 | 1.00x |
| flat.json | orjson | 0.980 | 1.027 | 1.101 | 99.531 | 0.64x |
| flat.json | msgspec | 0.795 | 0.806 | 0.958 | 99.531 | 0.81x |
| flat.json | ujson | 1.148 | 1.172 | 1.316 | 99.531 | 0.56x |
| flat.json | json | 1.413 | 1.430 | 1.523 | 99.531 | 0.46x |
| nested.json | strata | 0.564 | 0.584 | 0.600 | 99.547 | 1.00x |
| nested.json | orjson | 0.869 | 0.900 | 0.934 | 99.547 | 0.65x |
| nested.json | msgspec | 0.765 | 0.779 | 0.847 | 99.547 | 0.75x |
| nested.json | ujson | 1.059 | 1.084 | 1.152 | 99.547 | 0.54x |
| nested.json | json | 1.511 | 1.545 | 1.627 | 99.547 | 0.38x |
| wide_arrays.json | strata | 3.108 | 3.292 | 3.400 | 106.578 | 1.00x |
| wide_arrays.json | orjson | 3.723 | 3.993 | 4.174 | 106.578 | 0.82x |
| wide_arrays.json | msgspec | 4.292 | 4.512 | 4.615 | 106.578 | 0.73x |
| wide_arrays.json | ujson | 5.542 | 5.897 | 6.222 | 106.578 | 0.56x |
| wide_arrays.json | json | 6.977 | 7.336 | 7.538 | 106.578 | 0.45x |
| mixed.json | strata | 0.154 | 0.167 | 0.288 | 106.922 | 1.00x |
| mixed.json | orjson | 0.222 | 0.350 | 0.527 | 106.922 | 0.48x |
| mixed.json | msgspec | 0.220 | 0.241 | 0.347 | 106.922 | 0.69x |
| mixed.json | ujson | 0.271 | 0.292 | 0.382 | 106.922 | 0.57x |
| mixed.json | json | 0.370 | 0.396 | 0.468 | 106.922 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.516 | 7.643 | 7.886 | 99.516 | 1.00x |
| users.ndjson | orjson | 11.305 | 13.090 | 13.586 | 99.516 | 0.58x |
| users.ndjson | msgspec | 11.023 | 13.012 | 13.148 | 99.516 | 0.59x |
| users.ndjson | ujson | 13.808 | 16.117 | 16.420 | 99.516 | 0.47x |
| users.ndjson | json | 18.019 | 21.181 | 21.752 | 99.516 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.770 | 2.084 | 2.289 | 93.906 | 1.00x |
| users.json | orjson | 2.745 | 3.058 | 4.788 | 93.906 | 0.68x |
| users.json | msgspec | 3.503 | 3.615 | 4.127 | 93.906 | 0.58x |
| users.json | ujson | 9.411 | 10.128 | 10.890 | 93.906 | 0.21x |
| users.json | json | 16.787 | 17.491 | 23.194 | 93.906 | 0.12x |
| flat.json | strata | 0.346 | 0.370 | 0.435 | 99.547 | 1.00x |
| flat.json | orjson | 0.399 | 0.447 | 0.665 | 99.547 | 0.83x |
| flat.json | msgspec | 0.467 | 0.491 | 0.741 | 99.547 | 0.75x |
| flat.json | ujson | 0.922 | 0.950 | 0.986 | 99.547 | 0.39x |
| flat.json | json | 1.639 | 1.732 | 1.846 | 99.547 | 0.21x |
| nested.json | strata | 0.261 | 0.292 | 0.368 | 99.547 | 1.00x |
| nested.json | orjson | 0.359 | 0.397 | 0.712 | 99.547 | 0.74x |
| nested.json | msgspec | 0.448 | 0.535 | 0.795 | 99.547 | 0.55x |
| nested.json | ujson | 0.967 | 1.127 | 1.240 | 99.547 | 0.26x |
| nested.json | json | 1.849 | 1.876 | 2.008 | 99.547 | 0.16x |
| wide_arrays.json | strata | 1.449 | 1.494 | 1.775 | 106.906 | 1.00x |
| wide_arrays.json | orjson | 1.575 | 1.735 | 1.850 | 106.906 | 0.86x |
| wide_arrays.json | msgspec | 2.459 | 2.595 | 2.785 | 106.906 | 0.58x |
| wide_arrays.json | ujson | 5.347 | 5.681 | 5.836 | 106.906 | 0.26x |
| wide_arrays.json | json | 12.258 | 13.093 | 13.436 | 106.906 | 0.11x |
| mixed.json | strata | 0.199 | 0.232 | 0.311 | 106.922 | 1.00x |
| mixed.json | orjson | 0.225 | 0.309 | 0.417 | 106.922 | 0.75x |
| mixed.json | msgspec | 0.212 | 0.290 | 0.628 | 106.922 | 0.80x |
| mixed.json | ujson | 0.347 | 0.435 | 0.537 | 106.922 | 0.53x |
| mixed.json | json | 0.550 | 0.644 | 0.734 | 106.922 | 0.36x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.057 | 0.065 | 0.135 | 93.969 | 1.00x |
| users.json $[*].id | jmespath | 0.291 | 0.324 | 0.375 | 93.969 | 0.20x |
| users.json $[*].id | jsonpath-ng | 1.547 | 1.589 | 2.122 | 93.969 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.348 | 0.354 | 0.433 | 94.047 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.720 | 1.740 | 1.822 | 94.047 | 0.20x |
| users.json $[*].orders[*].total | jsonpath-ng | 10.629 | 10.830 | 11.026 | 94.047 | 0.03x |
| users.json $..total | strata | 1.297 | 1.442 | 1.814 | 94.078 | 1.00x |
| users.json $..total | jsonpath-ng | 190.182 | 197.021 | 219.559 | 94.078 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.466 | 3.697 | 3.766 | 94.016 | 1.00x |
| users.json $[*].id | orjson+jmespath | 9.604 | 10.428 | 10.573 | 94.016 | 0.35x |
| users.json $[*].id | orjson+jsonpath-ng | 10.864 | 11.787 | 12.133 | 94.016 | 0.31x |
| users.json $[*].orders[*].total | strata | 3.586 | 3.762 | 3.867 | 94.047 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 11.177 | 11.977 | 12.723 | 94.047 | 0.31x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 21.905 | 23.961 | 25.953 | 94.047 | 0.16x |
| users.json $..total | strata | 8.034 | 8.243 | 8.935 | 94.125 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 201.439 | 207.894 | 212.709 | 94.125 | 0.04x |

