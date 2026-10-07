# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 85eb3583e98587844415f5d32f85a61cbedfcf49
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
| users.json | strata | 6.266 | 6.804 | 7.338 | 68.172 | 1.00x |
| users.json | orjson | 9.465 | 10.334 | 11.294 | 68.172 | 0.66x |
| users.json | msgspec | 9.454 | 10.119 | 11.023 | 68.172 | 0.67x |
| users.json | ujson | 12.659 | 13.743 | 14.875 | 68.172 | 0.50x |
| users.json | pysimdjson | 128.421 | 130.255 | 140.100 | 68.172 | 0.05x |
| users.json | json | 15.227 | 16.538 | 21.636 | 68.172 | 0.41x |
| flat.json | strata | 0.620 | 0.689 | 0.869 | 100.109 | 1.00x |
| flat.json | orjson | 0.862 | 0.976 | 1.411 | 100.109 | 0.71x |
| flat.json | msgspec | 0.793 | 0.849 | 0.956 | 100.109 | 0.81x |
| flat.json | ujson | 1.199 | 1.358 | 1.571 | 100.109 | 0.51x |
| flat.json | pysimdjson | 12.854 | 14.237 | 15.823 | 100.109 | 0.05x |
| flat.json | json | 1.398 | 1.628 | 2.423 | 100.109 | 0.42x |
| nested.json | strata | 0.545 | 0.574 | 0.691 | 100.109 | 1.00x |
| nested.json | orjson | 0.747 | 0.912 | 1.025 | 100.109 | 0.63x |
| nested.json | msgspec | 0.703 | 0.777 | 0.931 | 100.109 | 0.74x |
| nested.json | ujson | 1.262 | 1.390 | 1.537 | 100.109 | 0.41x |
| nested.json | pysimdjson | 10.701 | 11.160 | 12.905 | 100.109 | 0.05x |
| nested.json | json | 1.446 | 1.552 | 1.874 | 100.109 | 0.37x |
| wide_arrays.json | strata | 3.128 | 4.039 | 5.579 | 102.844 | 1.00x |
| wide_arrays.json | orjson | 3.671 | 5.143 | 10.103 | 102.844 | 0.79x |
| wide_arrays.json | msgspec | 4.108 | 5.700 | 7.479 | 102.844 | 0.71x |
| wide_arrays.json | ujson | 5.356 | 7.315 | 10.318 | 102.844 | 0.55x |
| wide_arrays.json | pysimdjson | 66.210 | 79.711 | 97.652 | 102.844 | 0.05x |
| wide_arrays.json | json | 7.234 | 8.919 | 10.097 | 102.844 | 0.45x |
| mixed.json | strata | 0.153 | 0.175 | 0.337 | 102.906 | 1.00x |
| mixed.json | orjson | 0.183 | 0.222 | 0.393 | 102.906 | 0.79x |
| mixed.json | msgspec | 0.194 | 0.214 | 0.339 | 102.906 | 0.82x |
| mixed.json | ujson | 0.250 | 0.316 | 0.540 | 102.906 | 0.55x |
| mixed.json | pysimdjson | 2.815 | 3.245 | 3.499 | 102.906 | 0.05x |
| mixed.json | json | 0.387 | 0.449 | 1.024 | 102.906 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.546 | 1.637 | 1.947 | 76.406 | 1.00x |
| users.json | orjson | 2.340 | 2.448 | 2.895 | 76.406 | 0.67x |
| users.json | msgspec | 3.116 | 3.228 | 4.050 | 76.406 | 0.51x |
| users.json | ujson | 9.119 | 9.308 | 11.114 | 76.406 | 0.18x |
| users.json | json | 16.905 | 17.696 | 20.244 | 76.406 | 0.09x |
| flat.json | strata | 0.242 | 0.268 | 0.781 | 100.109 | 1.00x |
| flat.json | orjson | 0.267 | 0.315 | 0.666 | 100.109 | 0.85x |
| flat.json | msgspec | 0.335 | 0.398 | 0.560 | 100.109 | 0.67x |
| flat.json | ujson | 0.796 | 0.940 | 1.068 | 100.109 | 0.29x |
| flat.json | json | 1.445 | 1.975 | 2.393 | 100.109 | 0.14x |
| nested.json | strata | 0.136 | 0.160 | 0.211 | 100.109 | 1.00x |
| nested.json | orjson | 0.235 | 0.272 | 0.341 | 100.109 | 0.59x |
| nested.json | msgspec | 0.306 | 0.349 | 0.566 | 100.109 | 0.46x |
| nested.json | ujson | 0.831 | 0.946 | 1.360 | 100.109 | 0.17x |
| nested.json | json | 1.688 | 1.818 | 2.157 | 100.109 | 0.09x |
| wide_arrays.json | strata | 1.188 | 1.532 | 2.022 | 102.844 | 1.00x |
| wide_arrays.json | orjson | 1.745 | 1.930 | 2.595 | 102.844 | 0.79x |
| wide_arrays.json | msgspec | 2.293 | 2.750 | 3.180 | 102.844 | 0.56x |
| wide_arrays.json | ujson | 5.235 | 6.346 | 6.789 | 102.844 | 0.24x |
| wide_arrays.json | json | 12.520 | 15.142 | 16.946 | 102.844 | 0.10x |
| mixed.json | strata | 0.057 | 0.061 | 0.068 | 102.906 | 1.00x |
| mixed.json | orjson | 0.057 | 0.072 | 0.229 | 102.906 | 0.86x |
| mixed.json | msgspec | 0.068 | 0.082 | 0.123 | 102.906 | 0.75x |
| mixed.json | ujson | 0.208 | 0.233 | 0.266 | 102.906 | 0.26x |
| mixed.json | json | 0.412 | 0.444 | 0.680 | 102.906 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.809 | 7.651 | 9.259 | 87.578 | 1.00x |
| users.json | orjson | 10.396 | 11.445 | 15.431 | 87.578 | 0.67x |
| users.json | msgspec | 9.703 | 11.307 | 12.691 | 87.578 | 0.68x |
| users.json | ujson | 13.732 | 15.461 | 18.392 | 87.578 | 0.49x |
| users.json | json | 16.050 | 16.758 | 20.795 | 87.578 | 0.46x |
| flat.json | strata | 0.690 | 0.760 | 0.902 | 100.109 | 1.00x |
| flat.json | orjson | 1.097 | 1.249 | 1.389 | 100.109 | 0.61x |
| flat.json | msgspec | 0.902 | 0.999 | 1.086 | 100.109 | 0.76x |
| flat.json | ujson | 1.215 | 1.373 | 1.567 | 100.109 | 0.55x |
| flat.json | json | 1.481 | 1.601 | 1.755 | 100.109 | 0.47x |
| nested.json | strata | 0.570 | 0.699 | 0.843 | 100.109 | 1.00x |
| nested.json | orjson | 0.875 | 1.043 | 1.247 | 100.109 | 0.67x |
| nested.json | msgspec | 0.789 | 0.895 | 1.280 | 100.109 | 0.78x |
| nested.json | ujson | 1.073 | 1.256 | 1.870 | 100.109 | 0.56x |
| nested.json | json | 1.526 | 1.866 | 2.197 | 100.109 | 0.37x |
| wide_arrays.json | strata | 3.171 | 3.892 | 4.856 | 102.844 | 1.00x |
| wide_arrays.json | orjson | 3.894 | 5.070 | 6.137 | 102.844 | 0.77x |
| wide_arrays.json | msgspec | 4.248 | 5.721 | 6.744 | 102.844 | 0.68x |
| wide_arrays.json | ujson | 5.708 | 7.668 | 7.910 | 102.844 | 0.51x |
| wide_arrays.json | json | 6.918 | 8.467 | 9.407 | 102.844 | 0.46x |
| mixed.json | strata | 0.229 | 0.268 | 0.386 | 102.906 | 1.00x |
| mixed.json | orjson | 0.397 | 0.536 | 1.099 | 102.906 | 0.50x |
| mixed.json | msgspec | 0.302 | 0.399 | 0.496 | 102.906 | 0.67x |
| mixed.json | ujson | 0.390 | 0.460 | 0.816 | 102.906 | 0.58x |
| mixed.json | json | 0.502 | 0.544 | 0.710 | 102.906 | 0.49x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.522 | 8.669 | 13.335 | 100.109 | 1.00x |
| users.ndjson | orjson | 13.373 | 15.832 | 22.042 | 100.109 | 0.55x |
| users.ndjson | msgspec | 13.827 | 15.073 | 25.807 | 100.109 | 0.58x |
| users.ndjson | ujson | 17.331 | 19.104 | 23.845 | 100.109 | 0.45x |
| users.ndjson | json | 21.435 | 24.586 | 25.736 | 100.109 | 0.35x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.020 | 2.160 | 2.749 | 94.531 | 1.00x |
| users.json | orjson | 2.948 | 3.128 | 4.037 | 94.531 | 0.69x |
| users.json | msgspec | 3.562 | 3.869 | 5.943 | 94.531 | 0.56x |
| users.json | ujson | 10.043 | 10.567 | 14.212 | 94.531 | 0.20x |
| users.json | json | 17.404 | 18.797 | 19.624 | 94.531 | 0.11x |
| flat.json | strata | 0.423 | 0.588 | 1.080 | 100.109 | 1.00x |
| flat.json | orjson | 0.522 | 0.713 | 0.861 | 100.109 | 0.82x |
| flat.json | msgspec | 0.567 | 0.838 | 1.609 | 100.109 | 0.70x |
| flat.json | ujson | 1.071 | 1.289 | 1.773 | 100.109 | 0.46x |
| flat.json | json | 1.677 | 2.019 | 2.325 | 100.109 | 0.29x |
| nested.json | strata | 0.266 | 0.437 | 0.886 | 100.109 | 1.00x |
| nested.json | orjson | 0.358 | 0.573 | 0.699 | 100.109 | 0.76x |
| nested.json | msgspec | 0.588 | 0.767 | 1.319 | 100.109 | 0.57x |
| nested.json | ujson | 1.004 | 1.195 | 1.596 | 100.109 | 0.37x |
| nested.json | json | 1.768 | 2.216 | 2.620 | 100.109 | 0.20x |
| wide_arrays.json | strata | 1.599 | 2.062 | 2.310 | 102.891 | 1.00x |
| wide_arrays.json | orjson | 1.948 | 2.571 | 3.182 | 102.891 | 0.80x |
| wide_arrays.json | msgspec | 2.752 | 3.530 | 4.582 | 102.891 | 0.58x |
| wide_arrays.json | ujson | 5.637 | 7.153 | 7.693 | 102.891 | 0.29x |
| wide_arrays.json | json | 12.695 | 15.548 | 16.305 | 102.891 | 0.13x |
| mixed.json | strata | 0.298 | 0.345 | 0.451 | 102.906 | 1.00x |
| mixed.json | orjson | 0.321 | 0.397 | 1.026 | 102.906 | 0.87x |
| mixed.json | msgspec | 0.284 | 0.406 | 0.984 | 102.906 | 0.85x |
| mixed.json | ujson | 0.487 | 0.549 | 1.093 | 102.906 | 0.63x |
| mixed.json | json | 0.703 | 0.782 | 0.871 | 102.906 | 0.44x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.091 | 0.125 | 0.173 | 94.562 | 1.00x |
| users.json $[*].id | jmespath | 0.359 | 0.424 | 0.532 | 94.562 | 0.29x |
| users.json $[*].id | jsonpath-ng | 1.655 | 2.255 | 3.236 | 94.562 | 0.06x |
| users.json $[*].orders[*].total | strata | 0.405 | 0.522 | 0.670 | 94.688 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.751 | 2.041 | 2.597 | 94.688 | 0.26x |
| users.json $[*].orders[*].total | jsonpath-ng | 11.030 | 12.211 | 14.124 | 94.688 | 0.04x |
| users.json $..total | strata | 1.322 | 1.509 | 2.408 | 94.719 | 1.00x |
| users.json $..total | jsonpath-ng | 199.142 | 204.724 | 226.925 | 94.719 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.705 | 3.985 | 5.710 | 94.609 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.619 | 11.184 | 27.219 | 94.609 | 0.36x |
| users.json $[*].id | orjson+jsonpath-ng | 12.685 | 14.675 | 16.287 | 94.609 | 0.27x |
| users.json $[*].orders[*].total | strata | 3.777 | 4.409 | 7.077 | 94.719 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 11.474 | 13.210 | 21.830 | 94.719 | 0.33x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 22.735 | 26.904 | 39.349 | 94.719 | 0.16x |
| users.json $..total | strata | 8.171 | 9.824 | 13.661 | 94.750 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 216.158 | 246.812 | 272.541 | 94.750 | 0.04x |

