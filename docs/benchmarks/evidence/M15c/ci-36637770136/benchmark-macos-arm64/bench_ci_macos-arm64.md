# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 33465c22eee0fda8e1b52898c16aac6982901a21
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
| users.json | strata | 6.479 | 6.755 | 8.737 | 68.344 | 1.00x |
| users.json | orjson | 9.707 | 10.537 | 14.251 | 68.344 | 0.64x |
| users.json | msgspec | 9.589 | 10.421 | 13.082 | 68.344 | 0.65x |
| users.json | ujson | 11.565 | 13.807 | 16.475 | 68.344 | 0.49x |
| users.json | pysimdjson | 127.393 | 130.495 | 141.954 | 68.344 | 0.05x |
| users.json | json | 15.423 | 16.532 | 18.241 | 68.344 | 0.41x |
| flat.json | strata | 0.593 | 0.699 | 0.898 | 98.078 | 1.00x |
| flat.json | orjson | 0.805 | 0.864 | 0.936 | 98.078 | 0.81x |
| flat.json | msgspec | 0.811 | 0.878 | 0.951 | 98.078 | 0.80x |
| flat.json | ujson | 1.252 | 1.418 | 1.589 | 98.078 | 0.49x |
| flat.json | pysimdjson | 13.076 | 14.062 | 14.255 | 98.078 | 0.05x |
| flat.json | json | 1.505 | 1.686 | 1.792 | 98.078 | 0.41x |
| nested.json | strata | 0.495 | 0.551 | 0.590 | 98.078 | 1.00x |
| nested.json | orjson | 0.705 | 0.807 | 0.833 | 98.078 | 0.68x |
| nested.json | msgspec | 0.662 | 0.730 | 0.744 | 98.078 | 0.75x |
| nested.json | ujson | 1.014 | 1.149 | 1.253 | 98.078 | 0.48x |
| nested.json | pysimdjson | 10.096 | 11.106 | 11.601 | 98.078 | 0.05x |
| nested.json | json | 1.398 | 1.569 | 1.615 | 98.078 | 0.35x |
| wide_arrays.json | strata | 3.394 | 3.729 | 3.988 | 100.016 | 1.00x |
| wide_arrays.json | orjson | 4.367 | 4.658 | 4.989 | 100.016 | 0.80x |
| wide_arrays.json | msgspec | 4.637 | 4.854 | 5.610 | 100.016 | 0.77x |
| wide_arrays.json | ujson | 6.112 | 6.436 | 8.416 | 100.016 | 0.58x |
| wide_arrays.json | pysimdjson | 71.662 | 73.670 | 81.591 | 100.016 | 0.05x |
| wide_arrays.json | json | 7.906 | 8.383 | 9.573 | 100.016 | 0.44x |
| mixed.json | strata | 0.130 | 0.144 | 0.185 | 100.031 | 1.00x |
| mixed.json | orjson | 0.166 | 0.180 | 0.196 | 100.031 | 0.80x |
| mixed.json | msgspec | 0.185 | 0.190 | 0.307 | 100.031 | 0.76x |
| mixed.json | ujson | 0.229 | 0.315 | 0.480 | 100.031 | 0.46x |
| mixed.json | pysimdjson | 2.524 | 2.742 | 2.950 | 100.031 | 0.05x |
| mixed.json | json | 0.341 | 0.387 | 0.477 | 100.031 | 0.37x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.610 | 1.845 | 1.998 | 80.609 | 1.00x |
| users.json | orjson | 2.440 | 2.879 | 3.157 | 80.609 | 0.64x |
| users.json | msgspec | 3.083 | 3.376 | 3.997 | 80.609 | 0.55x |
| users.json | ujson | 8.935 | 9.675 | 11.230 | 80.609 | 0.19x |
| users.json | json | 15.503 | 17.387 | 19.525 | 80.609 | 0.11x |
| flat.json | strata | 0.256 | 0.275 | 0.380 | 98.078 | 1.00x |
| flat.json | orjson | 0.297 | 0.307 | 0.339 | 98.078 | 0.90x |
| flat.json | msgspec | 0.355 | 0.415 | 0.553 | 98.078 | 0.66x |
| flat.json | ujson | 0.939 | 1.048 | 1.218 | 98.078 | 0.26x |
| flat.json | json | 1.605 | 1.698 | 2.001 | 98.078 | 0.16x |
| nested.json | strata | 0.119 | 0.135 | 0.144 | 98.078 | 1.00x |
| nested.json | orjson | 0.207 | 0.238 | 0.281 | 98.078 | 0.57x |
| nested.json | msgspec | 0.276 | 0.385 | 0.465 | 98.078 | 0.35x |
| nested.json | ujson | 0.774 | 0.840 | 0.923 | 98.078 | 0.16x |
| nested.json | json | 1.622 | 1.722 | 1.799 | 98.078 | 0.08x |
| wide_arrays.json | strata | 1.232 | 1.392 | 1.856 | 100.016 | 1.00x |
| wide_arrays.json | orjson | 1.585 | 1.758 | 1.877 | 100.016 | 0.79x |
| wide_arrays.json | msgspec | 2.482 | 2.559 | 2.942 | 100.016 | 0.54x |
| wide_arrays.json | ujson | 5.225 | 5.537 | 6.050 | 100.016 | 0.25x |
| wide_arrays.json | json | 12.840 | 13.494 | 14.060 | 100.016 | 0.10x |
| mixed.json | strata | 0.042 | 0.052 | 0.090 | 100.031 | 1.00x |
| mixed.json | orjson | 0.054 | 0.064 | 0.076 | 100.031 | 0.82x |
| mixed.json | msgspec | 0.061 | 0.071 | 0.414 | 100.031 | 0.74x |
| mixed.json | ujson | 0.179 | 0.197 | 0.250 | 100.031 | 0.26x |
| mixed.json | json | 0.368 | 0.387 | 0.433 | 100.031 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.783 | 7.344 | 8.688 | 91.891 | 1.00x |
| users.json | orjson | 10.470 | 10.697 | 12.178 | 91.891 | 0.69x |
| users.json | msgspec | 9.238 | 10.285 | 11.707 | 91.891 | 0.71x |
| users.json | ujson | 13.375 | 15.079 | 17.613 | 91.891 | 0.49x |
| users.json | json | 16.269 | 17.107 | 18.775 | 91.891 | 0.43x |
| flat.json | strata | 0.660 | 0.708 | 0.793 | 98.078 | 1.00x |
| flat.json | orjson | 0.973 | 1.065 | 1.143 | 98.078 | 0.66x |
| flat.json | msgspec | 0.858 | 0.946 | 1.002 | 98.078 | 0.75x |
| flat.json | ujson | 1.239 | 1.329 | 1.470 | 98.078 | 0.53x |
| flat.json | json | 1.516 | 1.609 | 1.836 | 98.078 | 0.44x |
| nested.json | strata | 0.538 | 0.599 | 0.643 | 98.078 | 1.00x |
| nested.json | orjson | 0.840 | 0.899 | 0.956 | 98.078 | 0.67x |
| nested.json | msgspec | 0.720 | 0.800 | 0.988 | 98.078 | 0.75x |
| nested.json | ujson | 1.011 | 1.105 | 1.177 | 98.078 | 0.54x |
| nested.json | json | 1.482 | 1.539 | 1.666 | 98.078 | 0.39x |
| wide_arrays.json | strata | 3.406 | 3.536 | 3.736 | 100.016 | 1.00x |
| wide_arrays.json | orjson | 4.029 | 4.309 | 4.803 | 100.016 | 0.82x |
| wide_arrays.json | msgspec | 4.501 | 5.059 | 5.354 | 100.016 | 0.70x |
| wide_arrays.json | ujson | 6.039 | 6.283 | 7.555 | 100.016 | 0.56x |
| wide_arrays.json | json | 7.677 | 7.973 | 8.587 | 100.016 | 0.44x |
| mixed.json | strata | 0.218 | 0.237 | 0.502 | 100.031 | 1.00x |
| mixed.json | orjson | 0.326 | 0.480 | 0.637 | 100.031 | 0.49x |
| mixed.json | msgspec | 0.310 | 0.343 | 0.427 | 100.031 | 0.69x |
| mixed.json | ujson | 0.314 | 0.427 | 0.466 | 100.031 | 0.55x |
| mixed.json | json | 0.458 | 0.524 | 0.557 | 100.031 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.566 | 6.903 | 8.829 | 98.062 | 1.00x |
| users.ndjson | orjson | 11.204 | 12.162 | 15.111 | 98.062 | 0.57x |
| users.ndjson | msgspec | 11.677 | 12.636 | 14.628 | 98.062 | 0.55x |
| users.ndjson | ujson | 14.018 | 16.266 | 18.103 | 98.062 | 0.42x |
| users.ndjson | json | 18.006 | 20.173 | 23.539 | 98.062 | 0.34x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.340 | 2.763 | 3.612 | 92.484 | 1.00x |
| users.json | orjson | 3.091 | 3.423 | 4.701 | 92.484 | 0.81x |
| users.json | msgspec | 3.698 | 4.325 | 4.868 | 92.484 | 0.64x |
| users.json | ujson | 9.512 | 11.141 | 16.776 | 92.484 | 0.25x |
| users.json | json | 16.849 | 18.966 | 20.645 | 92.484 | 0.15x |
| flat.json | strata | 0.410 | 0.447 | 0.561 | 98.078 | 1.00x |
| flat.json | orjson | 0.485 | 0.520 | 0.713 | 98.078 | 0.86x |
| flat.json | msgspec | 0.525 | 0.560 | 0.979 | 98.078 | 0.80x |
| flat.json | ujson | 1.076 | 1.154 | 1.278 | 98.078 | 0.39x |
| flat.json | json | 1.770 | 1.896 | 1.968 | 98.078 | 0.24x |
| nested.json | strata | 0.262 | 0.320 | 0.494 | 98.078 | 1.00x |
| nested.json | orjson | 0.376 | 0.436 | 0.631 | 98.078 | 0.73x |
| nested.json | msgspec | 0.541 | 0.630 | 0.768 | 98.078 | 0.51x |
| nested.json | ujson | 0.985 | 1.170 | 1.393 | 98.078 | 0.27x |
| nested.json | json | 1.770 | 1.920 | 2.083 | 98.078 | 0.17x |
| wide_arrays.json | strata | 1.627 | 1.731 | 2.284 | 100.016 | 1.00x |
| wide_arrays.json | orjson | 2.000 | 2.363 | 2.658 | 100.016 | 0.73x |
| wide_arrays.json | msgspec | 2.825 | 3.082 | 3.590 | 100.016 | 0.56x |
| wide_arrays.json | ujson | 6.099 | 6.592 | 6.801 | 100.016 | 0.26x |
| wide_arrays.json | json | 13.292 | 14.327 | 14.763 | 100.016 | 0.12x |
| mixed.json | strata | 0.214 | 0.296 | 0.342 | 100.031 | 1.00x |
| mixed.json | orjson | 0.252 | 0.306 | 0.341 | 100.031 | 0.97x |
| mixed.json | msgspec | 0.194 | 0.406 | 7.197 | 100.031 | 0.73x |
| mixed.json | ujson | 0.316 | 0.453 | 8.010 | 100.031 | 0.65x |
| mixed.json | json | 0.539 | 0.706 | 7.460 | 100.031 | 0.42x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.070 | 0.122 | 0.354 | 92.516 | 1.00x |
| users.json $[*].id | jmespath | 0.345 | 0.387 | 0.448 | 92.516 | 0.31x |
| users.json $[*].id | jsonpath-ng | 1.616 | 1.775 | 2.741 | 92.516 | 0.07x |
| users.json $[*].orders[*].total | strata | 0.357 | 0.531 | 1.286 | 92.672 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.872 | 2.093 | 2.980 | 92.672 | 0.25x |
| users.json $[*].orders[*].total | jsonpath-ng | 11.941 | 14.592 | 19.260 | 92.672 | 0.04x |
| users.json $..total | strata | 1.478 | 1.556 | 2.092 | 92.688 | 1.00x |
| users.json $..total | jsonpath-ng | 201.440 | 243.661 | 261.043 | 92.688 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.660 | 4.565 | 5.776 | 92.594 | 1.00x |
| users.json $[*].id | orjson+jmespath | 11.107 | 13.464 | 21.521 | 92.594 | 0.34x |
| users.json $[*].id | orjson+jsonpath-ng | 12.510 | 16.846 | 54.599 | 92.594 | 0.27x |
| users.json $[*].orders[*].total | strata | 3.849 | 4.030 | 4.289 | 92.672 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 11.271 | 13.041 | 18.867 | 92.672 | 0.31x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 21.984 | 26.365 | 28.684 | 92.672 | 0.15x |
| users.json $..total | strata | 8.907 | 9.491 | 12.848 | 92.719 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 230.047 | 245.098 | 265.888 | 92.719 | 0.04x |

