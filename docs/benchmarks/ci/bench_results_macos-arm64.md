# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 4d4a4be
- python: 3.12.10
- implementation: CPython
- platform: macOS-26.5.2-arm64-arm-64bit
- machine: arm64
- processor: Apple M1 (Virtual)
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.563 | 8.840 | 10.306 | 63.609 | 1.00x |
| users.json | orjson | 10.174 | 11.436 | 14.419 | 63.609 | 0.77x |
| users.json | msgspec | 9.585 | 11.326 | 16.276 | 63.609 | 0.78x |
| users.json | ujson | 13.453 | 14.927 | 19.624 | 63.609 | 0.59x |
| users.json | pysimdjson | 135.610 | 140.328 | 167.076 | 63.609 | 0.06x |
| users.json | json | 16.262 | 19.228 | 22.823 | 63.609 | 0.46x |
| flat.json | strata | 0.669 | 0.736 | 0.889 | 85.656 | 1.00x |
| flat.json | orjson | 0.829 | 0.883 | 1.308 | 85.656 | 0.83x |
| flat.json | msgspec | 0.746 | 0.794 | 1.929 | 85.656 | 0.93x |
| flat.json | ujson | 1.266 | 1.451 | 3.405 | 85.656 | 0.51x |
| flat.json | pysimdjson | 12.560 | 13.822 | 18.115 | 85.656 | 0.05x |
| flat.json | json | 1.361 | 1.467 | 1.717 | 85.656 | 0.50x |
| nested.json | strata | 0.591 | 0.749 | 0.808 | 85.688 | 1.00x |
| nested.json | orjson | 0.753 | 0.942 | 1.008 | 85.688 | 0.79x |
| nested.json | msgspec | 0.749 | 0.885 | 1.111 | 85.688 | 0.85x |
| nested.json | ujson | 1.137 | 1.432 | 1.989 | 85.688 | 0.52x |
| nested.json | pysimdjson | 10.668 | 13.540 | 15.935 | 85.688 | 0.06x |
| nested.json | json | 1.459 | 1.843 | 2.593 | 85.688 | 0.41x |
| wide_arrays.json | strata | 3.343 | 3.700 | 5.487 | 88.422 | 1.00x |
| wide_arrays.json | orjson | 3.475 | 4.104 | 6.011 | 88.422 | 0.90x |
| wide_arrays.json | msgspec | 3.945 | 4.568 | 9.244 | 88.422 | 0.81x |
| wide_arrays.json | ujson | 5.644 | 5.907 | 7.359 | 88.422 | 0.63x |
| wide_arrays.json | pysimdjson | 65.667 | 68.111 | 101.180 | 88.422 | 0.05x |
| wide_arrays.json | json | 7.002 | 7.485 | 7.851 | 88.422 | 0.49x |
| mixed.json | strata | 0.145 | 0.178 | 0.203 | 88.438 | 1.00x |
| mixed.json | orjson | 0.178 | 0.199 | 0.216 | 88.438 | 0.89x |
| mixed.json | msgspec | 0.182 | 0.193 | 0.254 | 88.438 | 0.92x |
| mixed.json | ujson | 0.402 | 0.445 | 0.499 | 88.438 | 0.40x |
| mixed.json | pysimdjson | 2.652 | 2.801 | 3.431 | 88.438 | 0.06x |
| mixed.json | json | 0.343 | 0.400 | 0.891 | 88.438 | 0.44x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.210 | 2.504 | 3.187 | 73.219 | 1.00x |
| users.json | orjson | 2.455 | 2.905 | 4.221 | 73.219 | 0.86x |
| users.json | msgspec | 2.975 | 3.618 | 5.198 | 73.219 | 0.69x |
| users.json | ujson | 11.329 | 13.478 | 26.530 | 73.219 | 0.19x |
| users.json | json | 16.487 | 20.346 | 27.916 | 73.219 | 0.12x |
| flat.json | strata | 0.208 | 0.226 | 0.263 | 85.672 | 1.00x |
| flat.json | orjson | 0.407 | 0.453 | 0.489 | 85.672 | 0.50x |
| flat.json | msgspec | 0.342 | 0.354 | 0.394 | 85.672 | 0.64x |
| flat.json | ujson | 0.960 | 1.000 | 1.067 | 85.672 | 0.23x |
| flat.json | json | 1.389 | 1.457 | 1.712 | 85.672 | 0.15x |
| nested.json | strata | 0.189 | 0.225 | 0.794 | 85.688 | 1.00x |
| nested.json | orjson | 0.253 | 0.307 | 1.040 | 85.688 | 0.73x |
| nested.json | msgspec | 0.496 | 0.553 | 1.321 | 85.688 | 0.41x |
| nested.json | ujson | 1.275 | 1.433 | 2.952 | 85.688 | 0.16x |
| nested.json | json | 1.840 | 2.181 | 4.589 | 85.688 | 0.10x |
| wide_arrays.json | strata | 1.237 | 1.305 | 1.448 | 88.422 | 1.00x |
| wide_arrays.json | orjson | 1.336 | 1.568 | 1.854 | 88.422 | 0.83x |
| wide_arrays.json | msgspec | 2.188 | 2.364 | 2.804 | 88.422 | 0.55x |
| wide_arrays.json | ujson | 6.777 | 6.892 | 7.198 | 88.422 | 0.19x |
| wide_arrays.json | json | 12.170 | 12.700 | 13.235 | 88.422 | 0.10x |
| mixed.json | strata | 0.051 | 0.064 | 0.140 | 88.438 | 1.00x |
| mixed.json | orjson | 0.049 | 0.152 | 0.553 | 88.438 | 0.42x |
| mixed.json | msgspec | 0.059 | 0.069 | 0.441 | 88.438 | 0.92x |
| mixed.json | ujson | 0.233 | 0.267 | 0.459 | 88.438 | 0.24x |
| mixed.json | json | 0.371 | 0.419 | 0.610 | 88.438 | 0.15x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.728 | 10.462 | 17.098 | 83.266 | 1.00x |
| users.json | orjson | 11.252 | 12.443 | 19.423 | 83.266 | 0.84x |
| users.json | msgspec | 10.739 | 12.463 | 18.589 | 83.266 | 0.84x |
| users.json | ujson | 16.733 | 18.561 | 29.901 | 83.266 | 0.56x |
| users.json | json | 19.223 | 21.823 | 46.507 | 83.266 | 0.48x |
| flat.json | strata | 0.733 | 1.038 | 1.235 | 85.672 | 1.00x |
| flat.json | orjson | 0.987 | 1.181 | 2.312 | 85.672 | 0.88x |
| flat.json | msgspec | 0.844 | 0.987 | 1.180 | 85.672 | 1.05x |
| flat.json | ujson | 1.265 | 1.342 | 1.692 | 85.672 | 0.77x |
| flat.json | json | 1.498 | 1.712 | 2.195 | 85.672 | 0.61x |
| nested.json | strata | 0.761 | 0.840 | 1.195 | 85.688 | 1.00x |
| nested.json | orjson | 1.097 | 1.261 | 1.504 | 85.688 | 0.67x |
| nested.json | msgspec | 0.865 | 1.030 | 1.563 | 85.688 | 0.82x |
| nested.json | ujson | 1.192 | 1.475 | 4.380 | 85.688 | 0.57x |
| nested.json | json | 1.824 | 1.994 | 4.755 | 85.688 | 0.42x |
| wide_arrays.json | strata | 3.736 | 3.882 | 4.137 | 88.422 | 1.00x |
| wide_arrays.json | orjson | 4.021 | 4.304 | 4.824 | 88.422 | 0.90x |
| wide_arrays.json | msgspec | 4.562 | 4.996 | 5.947 | 88.422 | 0.78x |
| wide_arrays.json | ujson | 6.153 | 6.385 | 7.491 | 88.422 | 0.61x |
| wide_arrays.json | json | 7.432 | 7.823 | 9.203 | 88.422 | 0.50x |
| mixed.json | strata | 0.157 | 0.177 | 0.272 | 88.438 | 1.00x |
| mixed.json | orjson | 0.190 | 0.319 | 1.809 | 88.438 | 0.55x |
| mixed.json | msgspec | 0.196 | 0.218 | 0.407 | 88.438 | 0.81x |
| mixed.json | ujson | 0.246 | 0.270 | 0.422 | 88.438 | 0.65x |
| mixed.json | json | 0.342 | 0.365 | 0.519 | 88.438 | 0.48x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 8.456 | 9.230 | 17.164 | 85.641 | 1.00x |
| users.ndjson | orjson | 12.335 | 13.928 | 19.853 | 85.641 | 0.66x |
| users.ndjson | msgspec | 12.363 | 13.948 | 22.853 | 85.641 | 0.66x |
| users.ndjson | ujson | 15.405 | 19.189 | 27.699 | 85.641 | 0.48x |
| users.ndjson | json | 19.473 | 23.125 | 33.875 | 85.641 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.112 | 3.431 | 3.987 | 80.922 | 1.00x |
| users.json | orjson | 3.611 | 3.887 | 9.558 | 80.922 | 0.88x |
| users.json | msgspec | 3.989 | 4.289 | 4.632 | 80.922 | 0.80x |
| users.json | ujson | 12.604 | 13.652 | 16.080 | 80.922 | 0.25x |
| users.json | json | 17.883 | 18.901 | 23.688 | 80.922 | 0.18x |
| flat.json | strata | 0.468 | 0.655 | 2.912 | 85.672 | 1.00x |
| flat.json | orjson | 0.481 | 0.725 | 2.102 | 85.672 | 0.90x |
| flat.json | msgspec | 0.517 | 0.740 | 0.965 | 85.672 | 0.89x |
| flat.json | ujson | 1.132 | 1.449 | 2.181 | 85.672 | 0.45x |
| flat.json | json | 1.632 | 1.822 | 2.318 | 85.672 | 0.36x |
| nested.json | strata | 0.428 | 0.553 | 1.073 | 85.688 | 1.00x |
| nested.json | orjson | 0.527 | 0.610 | 0.758 | 85.688 | 0.91x |
| nested.json | msgspec | 0.586 | 0.939 | 1.077 | 85.688 | 0.59x |
| nested.json | ujson | 1.544 | 1.778 | 3.189 | 85.688 | 0.31x |
| nested.json | json | 2.332 | 2.552 | 3.828 | 85.688 | 0.22x |
| wide_arrays.json | strata | 1.625 | 1.883 | 2.823 | 88.422 | 1.00x |
| wide_arrays.json | orjson | 1.751 | 2.144 | 2.796 | 88.422 | 0.88x |
| wide_arrays.json | msgspec | 2.550 | 2.918 | 3.263 | 88.422 | 0.65x |
| wide_arrays.json | ujson | 7.601 | 8.209 | 9.023 | 88.422 | 0.23x |
| wide_arrays.json | json | 12.471 | 14.116 | 16.036 | 88.422 | 0.13x |
| mixed.json | strata | 0.174 | 0.222 | 0.699 | 88.438 | 1.00x |
| mixed.json | orjson | 0.156 | 0.282 | 0.796 | 88.438 | 0.79x |
| mixed.json | msgspec | 0.192 | 0.307 | 0.489 | 88.438 | 0.72x |
| mixed.json | ujson | 0.382 | 0.480 | 1.031 | 88.438 | 0.46x |
| mixed.json | json | 0.496 | 0.590 | 0.893 | 88.438 | 0.38x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.091 | 0.119 | 0.207 | 80.984 | 1.00x |
| users.json $[*].id | jmespath | 0.363 | 0.392 | 0.461 | 80.984 | 0.30x |
| users.json $[*].id | jsonpath-ng | 1.675 | 1.828 | 2.015 | 80.984 | 0.06x |
| users.json $[*].orders[*].total | strata | 0.427 | 0.636 | 2.098 | 81.172 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.771 | 2.091 | 3.955 | 81.172 | 0.30x |
| users.json $[*].orders[*].total | jsonpath-ng | 11.541 | 12.570 | 17.809 | 81.172 | 0.05x |
| users.json $..total | strata | 1.359 | 1.605 | 1.939 | 82.203 | 1.00x |
| users.json $..total | jsonpath-ng | 203.539 | 220.417 | 268.996 | 82.203 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.606 | 3.988 | 5.808 | 81.062 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.705 | 11.465 | 15.795 | 81.062 | 0.35x |
| users.json $[*].id | orjson+jsonpath-ng | 12.541 | 13.687 | 15.285 | 81.062 | 0.29x |
| users.json $[*].orders[*].total | strata | 4.176 | 4.468 | 6.079 | 82.203 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.401 | 17.570 | 23.872 | 82.203 | 0.25x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 27.196 | 36.458 | 49.530 | 82.203 | 0.12x |
| users.json $..total | strata | 10.259 | 11.804 | 14.234 | 82.203 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 230.025 | 254.902 | 295.300 | 82.203 | 0.05x |

