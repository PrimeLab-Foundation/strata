# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 38eaa9f248f509d9a4beb75b267189d011c1109e
- python: 3.12.10
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit
- machine: arm64
- processor: Apple M1 (Virtual)
- compiler_flags: -fno-strict-overflow -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -O3 -Wall -arch arm64 -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -march=native -flto=thin -fprofile-use=/Users/runner/work/_temp/strata-arm/build/pgo/strata.profdata -bundle -undefined (19 recorded commands; see the JSON companion)
- repeats: 60
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.186 | 7.453 | 10.340 | 68.219 | 1.00x |
| users.json | orjson | 9.491 | 11.748 | 18.274 | 68.219 | 0.63x |
| users.json | msgspec | 9.149 | 11.504 | 16.618 | 68.219 | 0.65x |
| users.json | ujson | 12.124 | 15.753 | 21.558 | 68.219 | 0.47x |
| users.json | pysimdjson | 129.995 | 143.598 | 173.181 | 68.219 | 0.05x |
| users.json | json | 15.070 | 18.119 | 22.887 | 68.219 | 0.41x |
| flat.json | strata | 0.613 | 0.669 | 0.852 | 93.078 | 1.00x |
| flat.json | orjson | 0.793 | 0.884 | 1.316 | 93.078 | 0.76x |
| flat.json | msgspec | 0.755 | 0.807 | 1.016 | 93.078 | 0.83x |
| flat.json | ujson | 1.195 | 1.407 | 2.394 | 93.078 | 0.48x |
| flat.json | pysimdjson | 12.515 | 13.497 | 18.372 | 93.078 | 0.05x |
| flat.json | json | 1.417 | 1.552 | 2.382 | 93.078 | 0.43x |
| nested.json | strata | 0.561 | 0.622 | 0.811 | 93.094 | 1.00x |
| nested.json | orjson | 0.813 | 0.886 | 1.265 | 93.094 | 0.70x |
| nested.json | msgspec | 0.727 | 0.813 | 1.312 | 93.094 | 0.77x |
| nested.json | ujson | 1.098 | 1.311 | 2.480 | 93.094 | 0.47x |
| nested.json | pysimdjson | 10.909 | 12.501 | 17.506 | 93.094 | 0.05x |
| nested.json | json | 1.520 | 1.673 | 2.626 | 93.094 | 0.37x |
| wide_arrays.json | strata | 3.516 | 4.003 | 7.999 | 96.875 | 1.00x |
| wide_arrays.json | orjson | 4.171 | 5.033 | 8.108 | 96.875 | 0.80x |
| wide_arrays.json | msgspec | 4.592 | 5.599 | 9.815 | 96.875 | 0.71x |
| wide_arrays.json | ujson | 6.052 | 6.982 | 11.477 | 96.875 | 0.57x |
| wide_arrays.json | pysimdjson | 70.059 | 83.193 | 100.266 | 96.875 | 0.05x |
| wide_arrays.json | json | 7.635 | 8.724 | 12.828 | 96.875 | 0.46x |
| mixed.json | strata | 0.129 | 0.149 | 0.276 | 97.938 | 1.00x |
| mixed.json | orjson | 0.164 | 0.194 | 0.471 | 97.938 | 0.77x |
| mixed.json | msgspec | 0.177 | 0.195 | 0.612 | 97.938 | 0.76x |
| mixed.json | ujson | 0.220 | 0.388 | 0.833 | 97.938 | 0.38x |
| mixed.json | pysimdjson | 2.550 | 2.745 | 6.466 | 97.938 | 0.05x |
| mixed.json | json | 0.339 | 0.377 | 1.011 | 97.938 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.533 | 1.850 | 2.196 | 71.469 | 1.00x |
| users.json | orjson | 2.291 | 2.694 | 3.264 | 71.469 | 0.69x |
| users.json | msgspec | 2.950 | 3.380 | 4.183 | 71.469 | 0.55x |
| users.json | ujson | 8.882 | 10.208 | 13.191 | 71.469 | 0.18x |
| users.json | json | 15.897 | 17.989 | 21.371 | 71.469 | 0.10x |
| flat.json | strata | 0.242 | 0.273 | 0.733 | 93.078 | 1.00x |
| flat.json | orjson | 0.267 | 0.307 | 0.585 | 93.078 | 0.89x |
| flat.json | msgspec | 0.335 | 0.375 | 0.733 | 93.078 | 0.73x |
| flat.json | ujson | 0.794 | 0.859 | 1.222 | 93.078 | 0.32x |
| flat.json | json | 1.500 | 1.707 | 2.507 | 93.078 | 0.16x |
| nested.json | strata | 0.131 | 0.156 | 0.405 | 93.094 | 1.00x |
| nested.json | orjson | 0.232 | 0.270 | 0.487 | 93.094 | 0.58x |
| nested.json | msgspec | 0.296 | 0.334 | 0.556 | 93.094 | 0.47x |
| nested.json | ujson | 0.866 | 1.010 | 1.329 | 93.094 | 0.15x |
| nested.json | json | 1.698 | 1.890 | 3.278 | 93.094 | 0.08x |
| wide_arrays.json | strata | 1.176 | 1.436 | 2.134 | 96.875 | 1.00x |
| wide_arrays.json | orjson | 1.529 | 1.827 | 2.551 | 96.875 | 0.79x |
| wide_arrays.json | msgspec | 2.257 | 2.702 | 3.912 | 96.875 | 0.53x |
| wide_arrays.json | ujson | 5.397 | 5.829 | 7.867 | 96.875 | 0.25x |
| wide_arrays.json | json | 12.802 | 13.916 | 16.675 | 96.875 | 0.10x |
| mixed.json | strata | 0.040 | 0.055 | 0.076 | 97.938 | 1.00x |
| mixed.json | orjson | 0.050 | 0.067 | 0.176 | 97.938 | 0.82x |
| mixed.json | msgspec | 0.057 | 0.073 | 0.222 | 97.938 | 0.75x |
| mixed.json | ujson | 0.179 | 0.200 | 0.294 | 97.938 | 0.27x |
| mixed.json | json | 0.368 | 0.415 | 0.607 | 97.938 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.588 | 7.616 | 10.355 | 81.578 | 1.00x |
| users.json | orjson | 9.786 | 11.717 | 16.952 | 81.578 | 0.65x |
| users.json | msgspec | 9.412 | 11.149 | 15.236 | 81.578 | 0.68x |
| users.json | ujson | 12.787 | 16.065 | 21.750 | 81.578 | 0.47x |
| users.json | json | 15.342 | 18.957 | 24.341 | 81.578 | 0.40x |
| flat.json | strata | 0.729 | 0.827 | 1.694 | 93.078 | 1.00x |
| flat.json | orjson | 1.038 | 1.206 | 2.720 | 93.078 | 0.69x |
| flat.json | msgspec | 0.904 | 1.036 | 1.688 | 93.078 | 0.80x |
| flat.json | ujson | 1.282 | 1.495 | 3.543 | 93.078 | 0.55x |
| flat.json | json | 1.567 | 1.834 | 4.509 | 93.078 | 0.45x |
| nested.json | strata | 0.576 | 0.684 | 1.148 | 93.094 | 1.00x |
| nested.json | orjson | 0.907 | 1.049 | 1.468 | 93.094 | 0.65x |
| nested.json | msgspec | 0.782 | 0.921 | 2.032 | 93.094 | 0.74x |
| nested.json | ujson | 1.071 | 1.236 | 1.749 | 93.094 | 0.55x |
| nested.json | json | 1.507 | 1.702 | 1.943 | 93.094 | 0.40x |
| wide_arrays.json | strata | 3.472 | 3.973 | 7.288 | 96.984 | 1.00x |
| wide_arrays.json | orjson | 4.119 | 4.980 | 9.599 | 96.984 | 0.80x |
| wide_arrays.json | msgspec | 4.544 | 5.605 | 10.794 | 96.984 | 0.71x |
| wide_arrays.json | ujson | 6.016 | 7.367 | 13.345 | 96.984 | 0.54x |
| wide_arrays.json | json | 7.578 | 9.100 | 15.633 | 96.984 | 0.44x |
| mixed.json | strata | 0.165 | 0.216 | 0.321 | 97.938 | 1.00x |
| mixed.json | orjson | 0.227 | 0.385 | 0.567 | 97.938 | 0.56x |
| mixed.json | msgspec | 0.223 | 0.307 | 0.815 | 97.938 | 0.70x |
| mixed.json | ujson | 0.284 | 0.362 | 0.668 | 97.938 | 0.60x |
| mixed.json | json | 0.394 | 0.474 | 0.595 | 97.938 | 0.46x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.199 | 8.248 | 11.685 | 93.062 | 1.00x |
| users.ndjson | orjson | 12.533 | 13.860 | 19.454 | 93.062 | 0.60x |
| users.ndjson | msgspec | 12.671 | 14.130 | 19.036 | 93.062 | 0.58x |
| users.ndjson | ujson | 14.846 | 17.458 | 23.082 | 93.062 | 0.47x |
| users.ndjson | json | 19.617 | 23.154 | 33.218 | 93.062 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.841 | 2.205 | 3.327 | 87.328 | 1.00x |
| users.json | orjson | 2.865 | 3.130 | 5.635 | 87.328 | 0.70x |
| users.json | msgspec | 3.563 | 3.850 | 5.696 | 87.328 | 0.57x |
| users.json | ujson | 9.808 | 10.305 | 12.055 | 87.328 | 0.21x |
| users.json | json | 16.824 | 17.978 | 23.341 | 87.328 | 0.12x |
| flat.json | strata | 0.454 | 0.604 | 0.825 | 93.078 | 1.00x |
| flat.json | orjson | 0.530 | 0.663 | 0.845 | 93.078 | 0.91x |
| flat.json | msgspec | 0.600 | 0.733 | 1.258 | 93.078 | 0.82x |
| flat.json | ujson | 1.151 | 1.372 | 2.296 | 93.078 | 0.44x |
| flat.json | json | 1.742 | 2.093 | 3.267 | 93.078 | 0.29x |
| nested.json | strata | 0.390 | 0.460 | 0.737 | 93.094 | 1.00x |
| nested.json | orjson | 0.465 | 0.613 | 1.053 | 93.094 | 0.75x |
| nested.json | msgspec | 0.525 | 0.753 | 1.053 | 93.094 | 0.61x |
| nested.json | ujson | 1.188 | 1.393 | 2.295 | 93.094 | 0.33x |
| nested.json | json | 1.993 | 2.303 | 3.135 | 93.094 | 0.20x |
| wide_arrays.json | strata | 1.483 | 1.864 | 3.098 | 97.922 | 1.00x |
| wide_arrays.json | orjson | 1.943 | 2.320 | 3.220 | 97.922 | 0.80x |
| wide_arrays.json | msgspec | 2.677 | 3.246 | 4.381 | 97.922 | 0.57x |
| wide_arrays.json | ujson | 5.578 | 6.330 | 9.980 | 97.922 | 0.29x |
| wide_arrays.json | json | 12.951 | 14.565 | 22.062 | 97.922 | 0.13x |
| mixed.json | strata | 0.189 | 0.309 | 0.753 | 97.938 | 1.00x |
| mixed.json | orjson | 0.189 | 0.342 | 0.751 | 97.938 | 0.90x |
| mixed.json | msgspec | 0.203 | 0.385 | 0.679 | 97.938 | 0.80x |
| mixed.json | ujson | 0.368 | 0.504 | 0.822 | 97.938 | 0.61x |
| mixed.json | json | 0.574 | 0.733 | 1.604 | 97.938 | 0.42x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.052 | 0.083 | 0.148 | 87.406 | 1.00x |
| users.json $[*].id | jmespath | 0.278 | 0.351 | 0.521 | 87.406 | 0.24x |
| users.json $[*].id | jsonpath-ng | 1.502 | 1.720 | 2.250 | 87.406 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.312 | 0.737 | 1.855 | 87.516 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.856 | 2.438 | 4.122 | 87.516 | 0.30x |
| users.json $[*].orders[*].total | jsonpath-ng | 11.078 | 14.541 | 22.165 | 87.516 | 0.05x |
| users.json $..total | strata | 1.405 | 1.792 | 4.711 | 87.562 | 1.00x |
| users.json $..total | jsonpath-ng | 210.980 | 250.868 | 293.609 | 87.562 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.599 | 4.342 | 6.885 | 87.438 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.021 | 14.325 | 21.523 | 87.438 | 0.30x |
| users.json $[*].id | orjson+jsonpath-ng | 11.461 | 16.036 | 24.650 | 87.438 | 0.27x |
| users.json $[*].orders[*].total | strata | 3.812 | 4.342 | 6.708 | 87.562 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.420 | 15.186 | 21.659 | 87.562 | 0.29x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 24.977 | 32.674 | 50.324 | 87.562 | 0.13x |
| users.json $..total | strata | 9.227 | 12.089 | 17.659 | 87.625 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 240.682 | 290.202 | 392.618 | 87.625 | 0.04x |

