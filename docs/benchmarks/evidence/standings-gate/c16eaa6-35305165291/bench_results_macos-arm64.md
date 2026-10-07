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
| users.json | strata | 6.389 | 6.862 | 7.656 | 68.203 | 1.00x |
| users.json | orjson | 10.123 | 10.944 | 17.945 | 68.203 | 0.63x |
| users.json | msgspec | 9.830 | 10.481 | 11.796 | 68.203 | 0.65x |
| users.json | ujson | 12.759 | 14.404 | 21.854 | 68.203 | 0.48x |
| users.json | pysimdjson | 132.626 | 138.824 | 198.274 | 68.203 | 0.05x |
| users.json | json | 15.548 | 17.019 | 24.764 | 68.203 | 0.40x |
| flat.json | strata | 0.576 | 0.627 | 0.664 | 94.422 | 1.00x |
| flat.json | orjson | 0.768 | 0.838 | 0.872 | 94.422 | 0.75x |
| flat.json | msgspec | 0.729 | 0.759 | 0.843 | 94.422 | 0.83x |
| flat.json | ujson | 1.107 | 1.263 | 1.368 | 94.422 | 0.50x |
| flat.json | pysimdjson | 12.323 | 12.656 | 13.147 | 94.422 | 0.05x |
| flat.json | json | 1.380 | 1.455 | 1.619 | 94.422 | 0.43x |
| nested.json | strata | 0.520 | 0.580 | 0.723 | 94.422 | 1.00x |
| nested.json | orjson | 0.722 | 0.801 | 0.980 | 94.422 | 0.72x |
| nested.json | msgspec | 0.664 | 0.753 | 0.826 | 94.422 | 0.77x |
| nested.json | ujson | 0.985 | 1.180 | 1.344 | 94.422 | 0.49x |
| nested.json | pysimdjson | 10.742 | 11.182 | 11.499 | 94.422 | 0.05x |
| nested.json | json | 1.515 | 1.585 | 1.733 | 94.422 | 0.37x |
| wide_arrays.json | strata | 2.805 | 3.181 | 3.634 | 97.188 | 1.00x |
| wide_arrays.json | orjson | 3.506 | 3.924 | 4.273 | 97.188 | 0.81x |
| wide_arrays.json | msgspec | 3.771 | 4.291 | 4.690 | 97.188 | 0.74x |
| wide_arrays.json | ujson | 4.915 | 5.566 | 7.434 | 97.188 | 0.57x |
| wide_arrays.json | pysimdjson | 60.539 | 66.858 | 89.498 | 97.188 | 0.05x |
| wide_arrays.json | json | 6.396 | 7.176 | 7.451 | 97.188 | 0.44x |
| mixed.json | strata | 0.114 | 0.116 | 0.119 | 97.203 | 1.00x |
| mixed.json | orjson | 0.146 | 0.154 | 0.168 | 97.203 | 0.76x |
| mixed.json | msgspec | 0.157 | 0.161 | 0.189 | 97.203 | 0.72x |
| mixed.json | ujson | 0.194 | 0.256 | 0.348 | 97.203 | 0.46x |
| mixed.json | pysimdjson | 2.339 | 2.363 | 2.796 | 97.203 | 0.05x |
| mixed.json | json | 0.298 | 0.304 | 0.399 | 97.203 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.473 | 1.663 | 3.479 | 77.547 | 1.00x |
| users.json | orjson | 2.347 | 2.493 | 3.162 | 77.547 | 0.67x |
| users.json | msgspec | 2.940 | 3.480 | 9.256 | 77.547 | 0.48x |
| users.json | ujson | 9.227 | 10.026 | 13.257 | 77.547 | 0.17x |
| users.json | json | 16.593 | 17.536 | 21.221 | 77.547 | 0.09x |
| flat.json | strata | 0.233 | 0.268 | 0.435 | 94.422 | 1.00x |
| flat.json | orjson | 0.266 | 0.320 | 0.521 | 94.422 | 0.84x |
| flat.json | msgspec | 0.338 | 0.361 | 0.475 | 94.422 | 0.74x |
| flat.json | ujson | 0.822 | 0.861 | 1.032 | 94.422 | 0.31x |
| flat.json | json | 1.450 | 1.632 | 1.761 | 94.422 | 0.16x |
| nested.json | strata | 0.134 | 0.147 | 0.186 | 94.422 | 1.00x |
| nested.json | orjson | 0.242 | 0.263 | 0.274 | 94.422 | 0.56x |
| nested.json | msgspec | 0.349 | 0.474 | 0.667 | 94.422 | 0.31x |
| nested.json | ujson | 0.984 | 1.029 | 1.129 | 94.422 | 0.14x |
| nested.json | json | 1.760 | 1.803 | 1.949 | 94.422 | 0.08x |
| wide_arrays.json | strata | 1.069 | 1.260 | 1.411 | 97.188 | 1.00x |
| wide_arrays.json | orjson | 1.465 | 1.669 | 1.871 | 97.188 | 0.75x |
| wide_arrays.json | msgspec | 2.356 | 2.442 | 2.734 | 97.188 | 0.52x |
| wide_arrays.json | ujson | 5.075 | 5.459 | 6.224 | 97.188 | 0.23x |
| wide_arrays.json | json | 12.103 | 12.992 | 13.645 | 97.188 | 0.10x |
| mixed.json | strata | 0.044 | 0.053 | 0.097 | 97.203 | 1.00x |
| mixed.json | orjson | 0.052 | 0.066 | 0.250 | 97.203 | 0.80x |
| mixed.json | msgspec | 0.063 | 0.074 | 0.250 | 97.203 | 0.72x |
| mixed.json | ujson | 0.186 | 0.200 | 0.225 | 97.203 | 0.26x |
| mixed.json | json | 0.382 | 0.404 | 0.563 | 97.203 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.512 | 7.127 | 12.138 | 87.922 | 1.00x |
| users.json | orjson | 10.207 | 12.283 | 17.158 | 87.922 | 0.58x |
| users.json | msgspec | 9.853 | 10.661 | 18.270 | 87.922 | 0.67x |
| users.json | ujson | 14.421 | 17.964 | 26.390 | 87.922 | 0.40x |
| users.json | json | 16.251 | 17.934 | 24.775 | 87.922 | 0.40x |
| flat.json | strata | 0.665 | 0.703 | 0.811 | 94.422 | 1.00x |
| flat.json | orjson | 0.989 | 1.058 | 1.174 | 94.422 | 0.66x |
| flat.json | msgspec | 0.849 | 0.874 | 1.160 | 94.422 | 0.80x |
| flat.json | ujson | 1.228 | 1.297 | 1.736 | 94.422 | 0.54x |
| flat.json | json | 1.512 | 1.546 | 1.677 | 94.422 | 0.45x |
| nested.json | strata | 0.598 | 0.657 | 0.740 | 94.422 | 1.00x |
| nested.json | orjson | 0.979 | 1.025 | 1.129 | 94.422 | 0.64x |
| nested.json | msgspec | 0.844 | 0.881 | 0.986 | 94.422 | 0.75x |
| nested.json | ujson | 1.116 | 1.227 | 1.317 | 94.422 | 0.54x |
| nested.json | json | 1.612 | 1.682 | 1.837 | 94.422 | 0.39x |
| wide_arrays.json | strata | 3.311 | 3.549 | 4.700 | 97.188 | 1.00x |
| wide_arrays.json | orjson | 3.986 | 4.227 | 19.369 | 97.188 | 0.84x |
| wide_arrays.json | msgspec | 4.599 | 4.868 | 17.614 | 97.188 | 0.73x |
| wide_arrays.json | ujson | 5.895 | 6.431 | 8.832 | 97.188 | 0.55x |
| wide_arrays.json | json | 7.547 | 8.046 | 8.755 | 97.188 | 0.44x |
| mixed.json | strata | 0.178 | 0.210 | 0.253 | 97.203 | 1.00x |
| mixed.json | orjson | 0.270 | 0.317 | 0.525 | 97.203 | 0.66x |
| mixed.json | msgspec | 0.270 | 0.299 | 0.434 | 97.203 | 0.70x |
| mixed.json | ujson | 0.350 | 0.536 | 0.629 | 97.203 | 0.39x |
| mixed.json | json | 0.415 | 0.464 | 0.507 | 97.203 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.528 | 7.799 | 8.074 | 94.422 | 1.00x |
| users.ndjson | orjson | 12.229 | 13.514 | 14.272 | 94.422 | 0.58x |
| users.ndjson | msgspec | 12.246 | 13.480 | 14.271 | 94.422 | 0.58x |
| users.ndjson | ujson | 15.391 | 16.808 | 23.724 | 94.422 | 0.46x |
| users.ndjson | json | 19.875 | 21.890 | 27.964 | 94.422 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.912 | 2.121 | 8.782 | 88.734 | 1.00x |
| users.json | orjson | 2.690 | 3.066 | 8.322 | 88.734 | 0.69x |
| users.json | msgspec | 3.546 | 3.836 | 8.756 | 88.734 | 0.55x |
| users.json | ujson | 9.386 | 10.438 | 16.702 | 88.734 | 0.20x |
| users.json | json | 16.013 | 17.672 | 26.171 | 88.734 | 0.12x |
| flat.json | strata | 0.373 | 0.401 | 6.949 | 94.422 | 1.00x |
| flat.json | orjson | 0.398 | 0.443 | 11.908 | 94.422 | 0.91x |
| flat.json | msgspec | 0.466 | 0.512 | 0.762 | 94.422 | 0.78x |
| flat.json | ujson | 0.964 | 1.027 | 1.180 | 94.422 | 0.39x |
| flat.json | json | 1.634 | 1.768 | 9.520 | 94.422 | 0.23x |
| nested.json | strata | 0.263 | 0.288 | 0.455 | 94.422 | 1.00x |
| nested.json | orjson | 0.362 | 0.442 | 0.483 | 94.422 | 0.65x |
| nested.json | msgspec | 0.431 | 0.541 | 0.696 | 94.422 | 0.53x |
| nested.json | ujson | 1.096 | 1.154 | 1.252 | 94.422 | 0.25x |
| nested.json | json | 1.841 | 1.897 | 1.941 | 94.422 | 0.15x |
| wide_arrays.json | strata | 1.254 | 1.418 | 1.758 | 97.188 | 1.00x |
| wide_arrays.json | orjson | 1.538 | 1.713 | 2.078 | 97.188 | 0.83x |
| wide_arrays.json | msgspec | 2.396 | 2.462 | 2.765 | 97.188 | 0.58x |
| wide_arrays.json | ujson | 4.953 | 5.043 | 6.077 | 97.188 | 0.28x |
| wide_arrays.json | json | 11.362 | 11.644 | 12.700 | 97.188 | 0.12x |
| mixed.json | strata | 0.137 | 0.164 | 0.239 | 97.203 | 1.00x |
| mixed.json | orjson | 0.157 | 0.201 | 0.298 | 97.203 | 0.82x |
| mixed.json | msgspec | 0.159 | 0.256 | 0.330 | 97.203 | 0.64x |
| mixed.json | ujson | 0.278 | 0.296 | 0.447 | 97.203 | 0.55x |
| mixed.json | json | 0.475 | 0.544 | 0.621 | 97.203 | 0.30x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.051 | 0.059 | 0.133 | 88.797 | 1.00x |
| users.json $[*].id | jmespath | 0.275 | 0.293 | 0.312 | 88.797 | 0.20x |
| users.json $[*].id | jsonpath-ng | 1.461 | 1.517 | 1.965 | 88.797 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.432 | 0.491 | 0.597 | 88.953 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.703 | 1.999 | 2.120 | 88.953 | 0.25x |
| users.json $[*].orders[*].total | jsonpath-ng | 11.082 | 11.943 | 16.750 | 88.953 | 0.04x |
| users.json $..total | strata | 1.313 | 1.477 | 1.578 | 88.969 | 1.00x |
| users.json $..total | jsonpath-ng | 202.075 | 207.066 | 215.725 | 88.969 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.699 | 3.864 | 4.913 | 88.844 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.858 | 11.685 | 13.158 | 88.844 | 0.33x |
| users.json $[*].id | orjson+jsonpath-ng | 11.889 | 12.331 | 15.337 | 88.844 | 0.31x |
| users.json $[*].orders[*].total | strata | 3.821 | 3.887 | 4.804 | 88.953 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 11.878 | 13.072 | 16.420 | 88.953 | 0.30x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 24.304 | 25.803 | 28.153 | 88.953 | 0.15x |
| users.json $..total | strata | 7.495 | 8.965 | 10.428 | 89.000 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 201.583 | 220.836 | 285.332 | 89.000 | 0.04x |

