# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 38eaa9f248f509d9a4beb75b267189d011c1109e
- python: 3.12.10
- implementation: CPython
- platform: macOS-15.7.9-x86_64-i386-64bit
- machine: x86_64
- processor: Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz
- compiler_flags: -fno-strict-overflow -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -O3 -Wall -arch x86_64 -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -fomit-frame-pointer -march=native -flto=thin -fprofile-use=/Users/runner/work/_temp/strata-arm/build/pgo/strata.profdata -bundle -undefined (19 recorded commands; see the JSON companion)
- repeats: 60
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 16.715 | 17.519 | 19.041 | 56.750 | 1.00x |
| users.json | orjson | 23.487 | 26.042 | 32.618 | 56.750 | 0.67x |
| users.json | msgspec | 23.847 | 26.757 | 32.403 | 56.750 | 0.65x |
| users.json | ujson | 34.960 | 38.372 | 44.267 | 56.750 | 0.46x |
| users.json | pysimdjson | 152.794 | 159.790 | 174.423 | 56.750 | 0.11x |
| users.json | json | 38.951 | 43.276 | 49.914 | 56.750 | 0.40x |
| flat.json | strata | 1.267 | 2.024 | 7.293 | 68.664 | 1.00x |
| flat.json | orjson | 1.383 | 1.885 | 6.941 | 68.664 | 1.07x |
| flat.json | msgspec | 1.545 | 2.036 | 7.568 | 68.664 | 0.99x |
| flat.json | ujson | 2.689 | 3.186 | 10.397 | 68.664 | 0.64x |
| flat.json | pysimdjson | 14.384 | 18.711 | 54.039 | 68.664 | 0.11x |
| flat.json | json | 3.086 | 4.599 | 15.643 | 68.664 | 0.44x |
| nested.json | strata | 1.505 | 1.627 | 2.201 | 65.914 | 1.00x |
| nested.json | orjson | 1.703 | 1.834 | 2.643 | 65.914 | 0.89x |
| nested.json | msgspec | 1.890 | 2.062 | 2.590 | 65.914 | 0.79x |
| nested.json | ujson | 3.085 | 3.316 | 4.036 | 65.914 | 0.49x |
| nested.json | pysimdjson | 13.451 | 14.350 | 16.682 | 65.914 | 0.11x |
| nested.json | json | 3.869 | 4.272 | 5.383 | 65.914 | 0.38x |
| wide_arrays.json | strata | 7.118 | 7.890 | 8.916 | 72.242 | 1.00x |
| wide_arrays.json | orjson | 9.164 | 10.481 | 12.133 | 72.242 | 0.75x |
| wide_arrays.json | msgspec | 9.669 | 10.841 | 13.615 | 72.242 | 0.73x |
| wide_arrays.json | ujson | 12.021 | 13.808 | 16.920 | 72.242 | 0.57x |
| wide_arrays.json | pysimdjson | 75.377 | 80.084 | 91.705 | 72.242 | 0.10x |
| wide_arrays.json | json | 16.299 | 17.490 | 19.507 | 72.242 | 0.45x |
| mixed.json | strata | 0.334 | 0.366 | 0.496 | 67.105 | 1.00x |
| mixed.json | orjson | 0.411 | 0.450 | 0.557 | 67.105 | 0.81x |
| mixed.json | msgspec | 0.437 | 0.474 | 0.595 | 67.105 | 0.77x |
| mixed.json | ujson | 0.593 | 0.647 | 0.777 | 67.105 | 0.57x |
| mixed.json | pysimdjson | 3.088 | 3.239 | 3.712 | 67.105 | 0.11x |
| mixed.json | json | 0.848 | 0.907 | 1.077 | 67.105 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.362 | 2.718 | 3.346 | 48.395 | 1.00x |
| users.json | orjson | 3.258 | 3.613 | 4.300 | 48.395 | 0.75x |
| users.json | msgspec | 5.121 | 5.475 | 6.082 | 48.395 | 0.50x |
| users.json | ujson | 22.915 | 24.249 | 25.376 | 48.395 | 0.11x |
| users.json | json | 39.748 | 41.626 | 43.740 | 48.395 | 0.07x |
| flat.json | strata | 0.332 | 0.370 | 0.506 | 65.980 | 1.00x |
| flat.json | orjson | 0.404 | 0.450 | 0.650 | 65.980 | 0.82x |
| flat.json | msgspec | 0.521 | 0.597 | 0.815 | 65.980 | 0.62x |
| flat.json | ujson | 2.252 | 2.342 | 3.207 | 65.980 | 0.16x |
| flat.json | json | 3.662 | 3.807 | 4.979 | 65.980 | 0.10x |
| nested.json | strata | 0.223 | 0.275 | 0.491 | 66.102 | 1.00x |
| nested.json | orjson | 0.329 | 0.394 | 0.659 | 66.102 | 0.70x |
| nested.json | msgspec | 0.532 | 0.614 | 0.908 | 66.102 | 0.45x |
| nested.json | ujson | 2.244 | 2.372 | 3.298 | 66.102 | 0.12x |
| nested.json | json | 4.467 | 4.836 | 6.543 | 66.102 | 0.06x |
| wide_arrays.json | strata | 1.922 | 2.348 | 3.155 | 66.559 | 1.00x |
| wide_arrays.json | orjson | 2.388 | 2.923 | 3.844 | 66.559 | 0.80x |
| wide_arrays.json | msgspec | 3.360 | 4.004 | 5.481 | 66.559 | 0.59x |
| wide_arrays.json | ujson | 10.197 | 11.857 | 14.675 | 66.559 | 0.20x |
| wide_arrays.json | json | 33.142 | 38.901 | 45.353 | 66.559 | 0.06x |
| mixed.json | strata | 0.059 | 0.078 | 0.123 | 63.074 | 1.00x |
| mixed.json | orjson | 0.070 | 0.097 | 0.158 | 63.074 | 0.80x |
| mixed.json | msgspec | 0.097 | 0.128 | 0.161 | 63.074 | 0.61x |
| mixed.json | ujson | 0.426 | 0.465 | 0.633 | 63.074 | 0.17x |
| mixed.json | json | 0.895 | 0.979 | 1.269 | 63.074 | 0.08x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 16.532 | 17.501 | 18.170 | 63.637 | 1.00x |
| users.json | orjson | 22.402 | 24.749 | 27.922 | 63.637 | 0.71x |
| users.json | msgspec | 22.308 | 25.270 | 28.242 | 63.637 | 0.69x |
| users.json | ujson | 35.040 | 36.826 | 40.589 | 63.637 | 0.48x |
| users.json | json | 40.028 | 41.846 | 45.089 | 63.637 | 0.42x |
| flat.json | strata | 1.365 | 1.456 | 8.000 | 65.980 | 1.00x |
| flat.json | orjson | 1.549 | 1.666 | 2.736 | 65.980 | 0.87x |
| flat.json | msgspec | 1.724 | 1.832 | 2.896 | 65.980 | 0.79x |
| flat.json | ujson | 2.892 | 3.100 | 4.873 | 65.980 | 0.47x |
| flat.json | json | 3.197 | 3.418 | 5.717 | 65.980 | 0.43x |
| nested.json | strata | 1.644 | 1.922 | 2.800 | 66.102 | 1.00x |
| nested.json | orjson | 1.937 | 2.208 | 2.952 | 66.102 | 0.87x |
| nested.json | msgspec | 2.170 | 2.545 | 3.511 | 66.102 | 0.76x |
| nested.json | ujson | 3.464 | 3.875 | 5.028 | 66.102 | 0.50x |
| nested.json | json | 4.309 | 4.939 | 6.470 | 66.102 | 0.39x |
| wide_arrays.json | strata | 7.502 | 8.046 | 8.667 | 67.938 | 1.00x |
| wide_arrays.json | orjson | 9.035 | 9.800 | 11.270 | 67.938 | 0.82x |
| wide_arrays.json | msgspec | 10.183 | 10.905 | 13.014 | 67.938 | 0.74x |
| wide_arrays.json | ujson | 12.846 | 14.176 | 16.405 | 67.938 | 0.57x |
| wide_arrays.json | json | 16.503 | 17.903 | 20.263 | 67.938 | 0.45x |
| mixed.json | strata | 0.411 | 0.476 | 0.671 | 63.074 | 1.00x |
| mixed.json | orjson | 0.534 | 0.627 | 0.824 | 63.074 | 0.76x |
| mixed.json | msgspec | 0.544 | 0.676 | 0.885 | 63.074 | 0.70x |
| mixed.json | ujson | 0.710 | 0.860 | 1.135 | 63.074 | 0.55x |
| mixed.json | json | 0.939 | 1.099 | 1.312 | 63.074 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 18.306 | 19.041 | 24.125 | 68.035 | 1.00x |
| users.ndjson | orjson | 26.599 | 29.471 | 41.755 | 68.035 | 0.65x |
| users.ndjson | msgspec | 27.343 | 29.888 | 42.577 | 68.035 | 0.64x |
| users.ndjson | ujson | 39.502 | 41.894 | 57.752 | 68.035 | 0.45x |
| users.ndjson | json | 48.052 | 50.747 | 81.699 | 68.035 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.188 | 3.362 | 3.923 | 60.797 | 1.00x |
| users.json | orjson | 3.938 | 4.148 | 4.577 | 60.797 | 0.81x |
| users.json | msgspec | 5.576 | 5.958 | 6.619 | 60.797 | 0.56x |
| users.json | ujson | 23.472 | 24.705 | 25.834 | 60.797 | 0.14x |
| users.json | json | 39.807 | 41.192 | 42.929 | 60.797 | 0.08x |
| flat.json | strata | 0.687 | 0.759 | 0.913 | 65.980 | 1.00x |
| flat.json | orjson | 0.767 | 0.861 | 1.132 | 65.980 | 0.88x |
| flat.json | msgspec | 0.889 | 0.988 | 1.171 | 65.980 | 0.77x |
| flat.json | ujson | 2.593 | 2.779 | 3.298 | 65.980 | 0.27x |
| flat.json | json | 3.936 | 4.237 | 5.036 | 65.980 | 0.18x |
| nested.json | strata | 0.598 | 0.689 | 1.014 | 66.102 | 1.00x |
| nested.json | orjson | 0.757 | 0.915 | 1.624 | 66.102 | 0.75x |
| nested.json | msgspec | 0.995 | 1.162 | 1.808 | 66.102 | 0.59x |
| nested.json | ujson | 2.811 | 3.065 | 4.094 | 66.102 | 0.22x |
| nested.json | json | 5.223 | 5.768 | 8.150 | 66.102 | 0.12x |
| wide_arrays.json | strata | 2.562 | 3.155 | 3.907 | 67.938 | 1.00x |
| wide_arrays.json | orjson | 3.176 | 3.889 | 4.889 | 67.938 | 0.81x |
| wide_arrays.json | msgspec | 4.076 | 4.999 | 6.234 | 67.938 | 0.63x |
| wide_arrays.json | ujson | 11.110 | 12.925 | 16.011 | 67.938 | 0.24x |
| wide_arrays.json | json | 34.363 | 38.643 | 43.720 | 67.938 | 0.08x |
| mixed.json | strata | 0.314 | 0.412 | 0.550 | 63.074 | 1.00x |
| mixed.json | orjson | 0.358 | 0.459 | 0.645 | 63.074 | 0.90x |
| mixed.json | msgspec | 0.389 | 0.506 | 0.670 | 63.074 | 0.82x |
| mixed.json | ujson | 0.746 | 0.906 | 1.172 | 63.074 | 0.46x |
| mixed.json | json | 1.213 | 1.405 | 1.847 | 63.074 | 0.29x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.117 | 0.140 | 0.213 | 59.297 | 1.00x |
| users.json $[*].id | jmespath | 0.844 | 0.908 | 1.021 | 59.297 | 0.15x |
| users.json $[*].id | jsonpath-ng | 4.849 | 4.965 | 5.791 | 59.297 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.736 | 0.893 | 1.218 | 60.875 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.255 | 5.747 | 6.643 | 60.875 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 32.147 | 34.139 | 37.212 | 60.875 | 0.03x |
| users.json $..total | strata | 3.000 | 3.291 | 3.906 | 60.938 | 1.00x |
| users.json $..total | jsonpath-ng | 659.375 | 675.656 | 703.373 | 60.938 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.551 | 3.648 | 3.940 | 63.609 | 1.00x |
| users.json $[*].id | orjson+jmespath | 23.566 | 27.072 | 30.730 | 63.609 | 0.13x |
| users.json $[*].id | orjson+jsonpath-ng | 28.409 | 30.258 | 34.286 | 63.609 | 0.12x |
| users.json $[*].orders[*].total | strata | 3.998 | 4.055 | 4.426 | 60.879 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 28.520 | 30.911 | 34.180 | 60.879 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 59.678 | 65.254 | 76.450 | 60.879 | 0.06x |
| users.json $..total | strata | 20.333 | 21.762 | 23.728 | 60.992 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 693.636 | 719.237 | 790.664 | 60.992 | 0.03x |

