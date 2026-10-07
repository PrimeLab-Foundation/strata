# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 6e4d93e74610755e4c46406e91c8aaf4ab6817a1
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
| users.json | strata | 6.029 | 6.398 | 9.417 | 68.438 | 1.00x |
| users.json | orjson | 9.042 | 9.946 | 14.557 | 68.438 | 0.64x |
| users.json | msgspec | 8.645 | 9.704 | 10.760 | 68.438 | 0.66x |
| users.json | ujson | 11.569 | 12.594 | 16.530 | 68.438 | 0.51x |
| users.json | pysimdjson | 121.093 | 125.155 | 133.236 | 68.438 | 0.05x |
| users.json | json | 14.174 | 15.063 | 20.974 | 68.438 | 0.42x |
| flat.json | strata | 0.543 | 0.547 | 0.565 | 96.891 | 1.00x |
| flat.json | orjson | 0.666 | 0.679 | 0.715 | 96.891 | 0.81x |
| flat.json | msgspec | 0.656 | 0.663 | 0.679 | 96.891 | 0.83x |
| flat.json | ujson | 1.029 | 1.059 | 1.147 | 96.891 | 0.52x |
| flat.json | pysimdjson | 11.132 | 11.149 | 11.173 | 96.891 | 0.05x |
| flat.json | json | 1.260 | 1.262 | 1.593 | 96.891 | 0.43x |
| nested.json | strata | 0.473 | 0.477 | 0.479 | 96.906 | 1.00x |
| nested.json | orjson | 0.644 | 0.658 | 0.692 | 96.906 | 0.72x |
| nested.json | msgspec | 0.631 | 0.634 | 0.650 | 96.906 | 0.75x |
| nested.json | ujson | 0.942 | 0.975 | 1.069 | 96.906 | 0.49x |
| nested.json | pysimdjson | 9.721 | 9.745 | 9.930 | 96.906 | 0.05x |
| nested.json | json | 1.323 | 1.347 | 1.391 | 96.906 | 0.35x |
| wide_arrays.json | strata | 2.790 | 2.795 | 2.826 | 100.109 | 1.00x |
| wide_arrays.json | orjson | 3.328 | 3.352 | 3.858 | 100.109 | 0.83x |
| wide_arrays.json | msgspec | 3.756 | 3.781 | 4.902 | 100.109 | 0.74x |
| wide_arrays.json | ujson | 4.823 | 5.003 | 6.406 | 100.109 | 0.56x |
| wide_arrays.json | pysimdjson | 59.794 | 59.916 | 69.936 | 100.109 | 0.05x |
| wide_arrays.json | json | 6.343 | 6.388 | 7.773 | 100.109 | 0.44x |
| mixed.json | strata | 0.112 | 0.115 | 0.118 | 100.188 | 1.00x |
| mixed.json | orjson | 0.145 | 0.149 | 0.150 | 100.188 | 0.77x |
| mixed.json | msgspec | 0.157 | 0.159 | 0.170 | 100.188 | 0.73x |
| mixed.json | ujson | 0.192 | 0.217 | 0.315 | 100.188 | 0.53x |
| mixed.json | pysimdjson | 2.347 | 2.365 | 2.396 | 100.188 | 0.05x |
| mixed.json | json | 0.293 | 0.296 | 0.312 | 100.188 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.438 | 1.463 | 1.546 | 81.297 | 1.00x |
| users.json | orjson | 2.231 | 2.261 | 2.420 | 81.297 | 0.65x |
| users.json | msgspec | 2.821 | 2.846 | 3.069 | 81.297 | 0.51x |
| users.json | ujson | 8.617 | 8.678 | 9.664 | 81.297 | 0.17x |
| users.json | json | 15.657 | 15.713 | 21.925 | 81.297 | 0.09x |
| flat.json | strata | 0.197 | 0.198 | 0.209 | 96.906 | 1.00x |
| flat.json | orjson | 0.233 | 0.234 | 0.237 | 96.906 | 0.84x |
| flat.json | msgspec | 0.291 | 0.293 | 0.305 | 96.906 | 0.68x |
| flat.json | ujson | 0.727 | 0.728 | 0.827 | 96.906 | 0.27x |
| flat.json | json | 1.313 | 1.322 | 1.434 | 96.906 | 0.15x |
| nested.json | strata | 0.113 | 0.114 | 0.115 | 96.906 | 1.00x |
| nested.json | orjson | 0.205 | 0.206 | 0.212 | 96.906 | 0.55x |
| nested.json | msgspec | 0.267 | 0.269 | 0.269 | 96.906 | 0.42x |
| nested.json | ujson | 0.793 | 0.797 | 0.813 | 96.906 | 0.14x |
| nested.json | json | 1.518 | 1.532 | 1.539 | 96.906 | 0.07x |
| wide_arrays.json | strata | 1.019 | 1.040 | 1.059 | 100.109 | 1.00x |
| wide_arrays.json | orjson | 1.372 | 1.420 | 1.566 | 100.109 | 0.73x |
| wide_arrays.json | msgspec | 1.983 | 2.011 | 2.182 | 100.109 | 0.52x |
| wide_arrays.json | ujson | 4.613 | 4.641 | 5.007 | 100.109 | 0.22x |
| wide_arrays.json | json | 11.113 | 11.156 | 11.567 | 100.109 | 0.09x |
| mixed.json | strata | 0.031 | 0.032 | 0.032 | 100.188 | 1.00x |
| mixed.json | orjson | 0.040 | 0.040 | 0.042 | 100.188 | 0.79x |
| mixed.json | msgspec | 0.046 | 0.047 | 0.057 | 100.188 | 0.67x |
| mixed.json | ujson | 0.157 | 0.158 | 0.167 | 100.188 | 0.20x |
| mixed.json | json | 0.322 | 0.326 | 0.328 | 100.188 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.409 | 6.466 | 6.835 | 91.484 | 1.00x |
| users.json | orjson | 9.317 | 9.450 | 10.023 | 91.484 | 0.68x |
| users.json | msgspec | 8.741 | 9.112 | 9.629 | 91.484 | 0.71x |
| users.json | ujson | 12.237 | 12.518 | 13.905 | 91.484 | 0.52x |
| users.json | json | 14.272 | 14.745 | 15.268 | 91.484 | 0.44x |
| flat.json | strata | 0.577 | 0.581 | 0.621 | 96.906 | 1.00x |
| flat.json | orjson | 0.757 | 0.802 | 0.930 | 96.906 | 0.72x |
| flat.json | msgspec | 0.710 | 0.719 | 0.727 | 96.906 | 0.81x |
| flat.json | ujson | 1.047 | 1.054 | 1.487 | 96.906 | 0.55x |
| flat.json | json | 1.303 | 1.319 | 1.696 | 96.906 | 0.44x |
| nested.json | strata | 0.505 | 0.511 | 0.525 | 96.906 | 1.00x |
| nested.json | orjson | 0.731 | 0.795 | 0.842 | 96.906 | 0.64x |
| nested.json | msgspec | 0.695 | 0.705 | 0.723 | 96.906 | 0.73x |
| nested.json | ujson | 0.950 | 0.966 | 1.093 | 96.906 | 0.53x |
| nested.json | json | 1.370 | 1.391 | 1.424 | 96.906 | 0.37x |
| wide_arrays.json | strata | 2.993 | 3.020 | 3.206 | 100.109 | 1.00x |
| wide_arrays.json | orjson | 3.531 | 3.572 | 3.796 | 100.109 | 0.85x |
| wide_arrays.json | msgspec | 4.113 | 4.137 | 4.297 | 100.109 | 0.73x |
| wide_arrays.json | ujson | 5.408 | 5.432 | 5.764 | 100.109 | 0.56x |
| wide_arrays.json | json | 6.673 | 6.723 | 7.020 | 100.109 | 0.45x |
| mixed.json | strata | 0.133 | 0.136 | 0.143 | 100.188 | 1.00x |
| mixed.json | orjson | 0.183 | 0.212 | 0.401 | 100.188 | 0.64x |
| mixed.json | msgspec | 0.188 | 0.196 | 0.216 | 100.188 | 0.69x |
| mixed.json | ujson | 0.234 | 0.239 | 0.309 | 100.188 | 0.57x |
| mixed.json | json | 0.328 | 0.333 | 0.360 | 100.188 | 0.41x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.276 | 6.305 | 6.336 | 96.891 | 1.00x |
| users.ndjson | orjson | 10.778 | 10.891 | 11.169 | 96.891 | 0.58x |
| users.ndjson | msgspec | 10.648 | 10.703 | 11.390 | 96.891 | 0.59x |
| users.ndjson | ujson | 13.208 | 13.330 | 13.572 | 96.891 | 0.47x |
| users.ndjson | json | 17.104 | 17.159 | 18.578 | 96.891 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.728 | 1.813 | 1.983 | 92.953 | 1.00x |
| users.json | orjson | 2.564 | 2.621 | 2.747 | 92.953 | 0.69x |
| users.json | msgspec | 3.153 | 3.209 | 3.320 | 92.953 | 0.56x |
| users.json | ujson | 8.941 | 9.062 | 9.856 | 92.953 | 0.20x |
| users.json | json | 15.549 | 15.669 | 16.025 | 92.953 | 0.12x |
| flat.json | strata | 0.327 | 0.381 | 0.479 | 96.906 | 1.00x |
| flat.json | orjson | 0.381 | 0.394 | 0.458 | 96.906 | 0.97x |
| flat.json | msgspec | 0.435 | 0.448 | 0.470 | 96.906 | 0.85x |
| flat.json | ujson | 0.861 | 0.947 | 1.084 | 96.906 | 0.40x |
| flat.json | json | 1.465 | 1.498 | 1.639 | 96.906 | 0.25x |
| nested.json | strata | 0.244 | 0.254 | 0.658 | 96.906 | 1.00x |
| nested.json | orjson | 0.342 | 0.361 | 0.525 | 96.906 | 0.70x |
| nested.json | msgspec | 0.421 | 0.517 | 0.654 | 96.906 | 0.49x |
| nested.json | ujson | 0.922 | 1.001 | 1.324 | 96.906 | 0.25x |
| nested.json | json | 1.694 | 1.718 | 2.126 | 96.906 | 0.15x |
| wide_arrays.json | strata | 1.355 | 1.425 | 1.769 | 100.172 | 1.00x |
| wide_arrays.json | orjson | 1.656 | 1.736 | 1.964 | 100.172 | 0.82x |
| wide_arrays.json | msgspec | 2.424 | 2.499 | 2.975 | 100.172 | 0.57x |
| wide_arrays.json | ujson | 5.059 | 5.231 | 6.151 | 100.172 | 0.27x |
| wide_arrays.json | json | 11.448 | 11.612 | 12.255 | 100.172 | 0.12x |
| mixed.json | strata | 0.118 | 0.126 | 0.151 | 100.188 | 1.00x |
| mixed.json | orjson | 0.135 | 0.152 | 0.916 | 100.188 | 0.83x |
| mixed.json | msgspec | 0.138 | 0.153 | 0.271 | 100.188 | 0.83x |
| mixed.json | ujson | 0.257 | 0.274 | 0.297 | 100.188 | 0.46x |
| mixed.json | json | 0.419 | 0.440 | 0.784 | 100.188 | 0.29x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.047 | 0.049 | 0.063 | 92.984 | 1.00x |
| users.json $[*].id | jmespath | 0.258 | 0.261 | 0.276 | 92.984 | 0.19x |
| users.json $[*].id | jsonpath-ng | 1.399 | 1.412 | 1.638 | 92.984 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.270 | 0.274 | 0.286 | 93.109 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.542 | 1.549 | 1.568 | 93.109 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 9.716 | 9.803 | 9.834 | 93.109 | 0.03x |
| users.json $..total | strata | 1.204 | 1.340 | 1.406 | 93.156 | 1.00x |
| users.json $..total | jsonpath-ng | 178.383 | 182.390 | 197.699 | 93.156 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.376 | 3.402 | 3.506 | 93.047 | 1.00x |
| users.json $[*].id | orjson+jmespath | 9.238 | 9.292 | 9.370 | 93.047 | 0.37x |
| users.json $[*].id | orjson+jsonpath-ng | 10.418 | 10.449 | 10.898 | 93.047 | 0.33x |
| users.json $[*].orders[*].total | strata | 3.418 | 3.551 | 3.983 | 93.109 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 10.480 | 10.893 | 11.697 | 93.109 | 0.33x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 20.615 | 21.071 | 23.219 | 93.109 | 0.17x |
| users.json $..total | strata | 7.557 | 8.285 | 10.792 | 93.203 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 191.961 | 214.967 | 248.826 | 93.203 | 0.04x |

