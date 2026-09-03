# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: fb7997e
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
| users.json | strata | 7.148 | 9.271 | 17.834 | 63.766 | 1.00x |
| users.json | orjson | 9.876 | 12.003 | 15.030 | 63.766 | 0.77x |
| users.json | msgspec | 9.525 | 12.035 | 13.277 | 63.766 | 0.77x |
| users.json | ujson | 14.162 | 17.576 | 20.729 | 63.766 | 0.53x |
| users.json | pysimdjson | 131.799 | 151.871 | 187.149 | 63.766 | 0.06x |
| users.json | json | 15.840 | 19.539 | 32.716 | 63.766 | 0.47x |
| flat.json | strata | 0.626 | 0.726 | 1.004 | 83.875 | 1.00x |
| flat.json | orjson | 0.828 | 0.905 | 1.315 | 83.875 | 0.80x |
| flat.json | msgspec | 0.817 | 0.886 | 0.990 | 83.875 | 0.82x |
| flat.json | ujson | 1.396 | 1.535 | 2.080 | 83.875 | 0.47x |
| flat.json | pysimdjson | 12.790 | 14.109 | 16.220 | 83.875 | 0.05x |
| flat.json | json | 1.424 | 1.618 | 2.142 | 83.875 | 0.45x |
| nested.json | strata | 0.543 | 0.683 | 0.815 | 83.906 | 1.00x |
| nested.json | orjson | 0.748 | 0.961 | 1.205 | 83.906 | 0.71x |
| nested.json | msgspec | 0.737 | 0.956 | 1.465 | 83.906 | 0.71x |
| nested.json | ujson | 1.136 | 1.428 | 1.873 | 83.906 | 0.48x |
| nested.json | pysimdjson | 10.728 | 13.611 | 15.073 | 83.906 | 0.05x |
| nested.json | json | 1.486 | 1.870 | 2.246 | 83.906 | 0.37x |
| wide_arrays.json | strata | 3.806 | 4.255 | 6.183 | 87.719 | 1.00x |
| wide_arrays.json | orjson | 3.728 | 4.491 | 5.152 | 87.719 | 0.95x |
| wide_arrays.json | msgspec | 4.337 | 5.057 | 5.785 | 87.719 | 0.84x |
| wide_arrays.json | ujson | 5.806 | 7.010 | 7.478 | 87.719 | 0.61x |
| wide_arrays.json | pysimdjson | 69.598 | 73.636 | 91.181 | 87.719 | 0.06x |
| wide_arrays.json | json | 6.991 | 8.392 | 14.426 | 87.719 | 0.51x |
| mixed.json | strata | 0.150 | 0.157 | 0.214 | 87.766 | 1.00x |
| mixed.json | orjson | 0.176 | 0.195 | 0.359 | 87.766 | 0.81x |
| mixed.json | msgspec | 0.203 | 0.214 | 0.316 | 87.766 | 0.74x |
| mixed.json | ujson | 0.275 | 0.430 | 0.882 | 87.766 | 0.37x |
| mixed.json | pysimdjson | 2.879 | 3.093 | 3.538 | 87.766 | 0.05x |
| mixed.json | json | 0.373 | 0.404 | 0.532 | 87.766 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.697 | 1.759 | 2.099 | 67.531 | 1.00x |
| users.json | orjson | 2.286 | 2.442 | 2.924 | 67.531 | 0.72x |
| users.json | msgspec | 2.978 | 3.191 | 4.275 | 67.531 | 0.55x |
| users.json | ujson | 11.299 | 11.613 | 13.477 | 67.531 | 0.15x |
| users.json | json | 15.886 | 16.955 | 19.865 | 67.531 | 0.10x |
| flat.json | strata | 0.266 | 0.286 | 0.305 | 83.906 | 1.00x |
| flat.json | orjson | 0.293 | 0.309 | 0.428 | 83.906 | 0.93x |
| flat.json | msgspec | 0.336 | 0.377 | 0.422 | 83.906 | 0.76x |
| flat.json | ujson | 0.979 | 1.160 | 1.484 | 83.906 | 0.25x |
| flat.json | json | 1.615 | 1.768 | 2.444 | 83.906 | 0.16x |
| nested.json | strata | 0.159 | 0.184 | 0.595 | 83.906 | 1.00x |
| nested.json | orjson | 0.270 | 0.296 | 0.698 | 83.906 | 0.62x |
| nested.json | msgspec | 0.392 | 0.505 | 0.584 | 83.906 | 0.36x |
| nested.json | ujson | 1.351 | 1.499 | 2.300 | 83.906 | 0.12x |
| nested.json | json | 1.891 | 2.099 | 4.260 | 83.906 | 0.09x |
| wide_arrays.json | strata | 1.223 | 1.394 | 1.535 | 87.719 | 1.00x |
| wide_arrays.json | orjson | 1.581 | 1.851 | 2.059 | 87.719 | 0.75x |
| wide_arrays.json | msgspec | 2.434 | 2.610 | 2.884 | 87.719 | 0.53x |
| wide_arrays.json | ujson | 7.253 | 8.011 | 9.586 | 87.719 | 0.17x |
| wide_arrays.json | json | 12.782 | 13.509 | 15.804 | 87.719 | 0.10x |
| mixed.json | strata | 0.042 | 0.051 | 0.069 | 87.766 | 1.00x |
| mixed.json | orjson | 0.051 | 0.145 | 0.267 | 87.766 | 0.35x |
| mixed.json | msgspec | 0.066 | 0.071 | 0.087 | 87.766 | 0.73x |
| mixed.json | ujson | 0.237 | 0.259 | 0.308 | 87.766 | 0.20x |
| mixed.json | json | 0.361 | 0.430 | 0.513 | 87.766 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.639 | 8.716 | 10.516 | 80.844 | 1.00x |
| users.json | orjson | 11.345 | 13.309 | 19.731 | 80.844 | 0.65x |
| users.json | msgspec | 11.346 | 12.336 | 15.471 | 80.844 | 0.71x |
| users.json | ujson | 15.437 | 17.424 | 30.263 | 80.844 | 0.50x |
| users.json | json | 16.900 | 20.392 | 25.988 | 80.844 | 0.43x |
| flat.json | strata | 0.794 | 0.854 | 0.933 | 83.906 | 1.00x |
| flat.json | orjson | 1.053 | 1.161 | 1.312 | 83.906 | 0.74x |
| flat.json | msgspec | 0.955 | 1.036 | 1.343 | 83.906 | 0.82x |
| flat.json | ujson | 1.398 | 1.463 | 1.764 | 83.906 | 0.58x |
| flat.json | json | 1.760 | 1.849 | 2.095 | 83.906 | 0.46x |
| nested.json | strata | 0.722 | 0.778 | 0.983 | 83.906 | 1.00x |
| nested.json | orjson | 1.090 | 1.151 | 2.037 | 83.906 | 0.68x |
| nested.json | msgspec | 0.939 | 1.035 | 1.397 | 83.906 | 0.75x |
| nested.json | ujson | 1.278 | 1.432 | 1.637 | 83.906 | 0.54x |
| nested.json | json | 1.766 | 1.901 | 2.997 | 83.906 | 0.41x |
| wide_arrays.json | strata | 3.981 | 4.610 | 7.127 | 87.719 | 1.00x |
| wide_arrays.json | orjson | 4.199 | 4.945 | 5.440 | 87.719 | 0.93x |
| wide_arrays.json | msgspec | 4.905 | 5.477 | 6.139 | 87.719 | 0.84x |
| wide_arrays.json | ujson | 6.722 | 7.594 | 8.150 | 87.719 | 0.61x |
| wide_arrays.json | json | 8.123 | 8.932 | 9.368 | 87.719 | 0.52x |
| mixed.json | strata | 0.198 | 0.226 | 0.670 | 87.766 | 1.00x |
| mixed.json | orjson | 0.419 | 0.459 | 0.948 | 87.766 | 0.49x |
| mixed.json | msgspec | 0.292 | 0.315 | 0.552 | 87.766 | 0.72x |
| mixed.json | ujson | 0.347 | 0.383 | 0.476 | 87.766 | 0.59x |
| mixed.json | json | 0.452 | 0.473 | 0.704 | 87.766 | 0.48x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.423 | 8.884 | 10.827 | 83.844 | 1.00x |
| users.ndjson | orjson | 11.703 | 13.555 | 16.660 | 83.844 | 0.66x |
| users.ndjson | msgspec | 12.045 | 12.979 | 16.822 | 83.844 | 0.68x |
| users.ndjson | ujson | 14.451 | 17.253 | 20.027 | 83.844 | 0.51x |
| users.ndjson | json | 19.459 | 20.432 | 24.978 | 83.844 | 0.43x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.529 | 3.046 | 3.863 | 76.125 | 1.00x |
| users.json | orjson | 3.362 | 3.824 | 4.786 | 76.125 | 0.80x |
| users.json | msgspec | 4.136 | 4.482 | 5.929 | 76.125 | 0.68x |
| users.json | ujson | 13.529 | 14.657 | 17.295 | 76.125 | 0.21x |
| users.json | json | 17.913 | 20.953 | 23.037 | 76.125 | 0.15x |
| flat.json | strata | 0.478 | 0.635 | 0.966 | 83.906 | 1.00x |
| flat.json | orjson | 0.526 | 0.713 | 1.019 | 83.906 | 0.89x |
| flat.json | msgspec | 0.562 | 0.714 | 1.354 | 83.906 | 0.89x |
| flat.json | ujson | 1.366 | 1.547 | 1.916 | 83.906 | 0.41x |
| flat.json | json | 1.884 | 2.208 | 4.471 | 83.906 | 0.29x |
| nested.json | strata | 0.339 | 0.436 | 1.559 | 83.906 | 1.00x |
| nested.json | orjson | 0.470 | 0.544 | 1.246 | 83.906 | 0.80x |
| nested.json | msgspec | 0.537 | 0.807 | 1.542 | 83.906 | 0.54x |
| nested.json | ujson | 1.357 | 1.652 | 2.386 | 83.906 | 0.26x |
| nested.json | json | 2.050 | 2.356 | 2.997 | 83.906 | 0.19x |
| wide_arrays.json | strata | 1.790 | 1.976 | 2.328 | 87.750 | 1.00x |
| wide_arrays.json | orjson | 2.233 | 2.361 | 3.493 | 87.750 | 0.84x |
| wide_arrays.json | msgspec | 2.871 | 3.332 | 3.806 | 87.750 | 0.59x |
| wide_arrays.json | ujson | 7.934 | 8.674 | 11.203 | 87.750 | 0.23x |
| wide_arrays.json | json | 14.327 | 15.273 | 18.279 | 87.750 | 0.13x |
| mixed.json | strata | 0.257 | 0.335 | 0.909 | 87.766 | 1.00x |
| mixed.json | orjson | 0.316 | 0.466 | 0.561 | 87.766 | 0.72x |
| mixed.json | msgspec | 0.274 | 0.427 | 0.713 | 87.766 | 0.79x |
| mixed.json | ujson | 0.504 | 0.620 | 0.869 | 87.766 | 0.54x |
| mixed.json | json | 0.658 | 0.794 | 1.027 | 87.766 | 0.42x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.070 | 0.121 | 0.270 | 76.188 | 1.00x |
| users.json $[*].id | jmespath | 0.338 | 0.410 | 0.436 | 76.188 | 0.29x |
| users.json $[*].id | jsonpath-ng | 1.580 | 1.915 | 3.091 | 76.188 | 0.06x |
| users.json $[*].orders[*].total | strata | 0.487 | 0.568 | 1.197 | 79.312 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.824 | 2.036 | 2.621 | 79.312 | 0.28x |
| users.json $[*].orders[*].total | jsonpath-ng | 11.920 | 12.837 | 18.633 | 79.312 | 0.04x |
| users.json $..total | strata | 1.520 | 1.631 | 2.319 | 81.188 | 1.00x |
| users.json $..total | jsonpath-ng | 206.342 | 235.738 | 245.246 | 81.188 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.754 | 4.213 | 4.797 | 79.234 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.405 | 11.783 | 14.606 | 79.234 | 0.36x |
| users.json $[*].id | orjson+jsonpath-ng | 11.617 | 12.381 | 15.342 | 79.234 | 0.34x |
| users.json $[*].orders[*].total | strata | 4.186 | 4.895 | 7.456 | 81.172 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 13.299 | 14.327 | 22.602 | 81.172 | 0.34x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 28.749 | 32.622 | 44.982 | 81.172 | 0.15x |
| users.json $..total | strata | 9.171 | 10.489 | 11.787 | 81.188 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 230.275 | 245.624 | 267.190 | 81.188 | 0.04x |

