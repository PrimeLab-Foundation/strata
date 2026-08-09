# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: a1b3831
- python: 3.14.3
- implementation: CPython
- platform: macOS-26.3-arm64-arm-64bit-Mach-O
- machine: arm64
- processor: Apple M1 Max
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.960 | 10.162 | 10.471 | 52.734 | 1.00x |
| users.json | orjson | 10.664 | 10.894 | 11.350 | 52.734 | 0.93x |
| users.json | msgspec | 10.970 | 11.231 | 12.161 | 52.734 | 0.90x |
| users.json | ujson | 14.381 | 15.033 | 15.444 | 52.734 | 0.68x |
| users.json | json | 19.254 | 19.756 | 20.093 | 52.734 | 0.51x |
| flat.json | strata | 1.000 | 1.007 | 1.079 | 70.672 | 1.00x |
| flat.json | orjson | 0.847 | 0.853 | 0.914 | 70.672 | 1.18x |
| flat.json | msgspec | 0.890 | 0.901 | 0.952 | 70.672 | 1.12x |
| flat.json | ujson | 1.320 | 1.332 | 1.398 | 70.672 | 0.76x |
| flat.json | json | 1.835 | 1.845 | 2.031 | 70.672 | 0.55x |
| nested.json | strata | 0.785 | 0.794 | 0.871 | 70.797 | 1.00x |
| nested.json | orjson | 1.557 | 1.619 | 1.742 | 70.797 | 0.49x |
| nested.json | msgspec | 1.539 | 1.579 | 1.808 | 70.797 | 0.50x |
| nested.json | ujson | 1.889 | 1.922 | 2.180 | 70.797 | 0.41x |
| nested.json | json | 2.485 | 2.584 | 2.856 | 70.797 | 0.31x |
| wide_arrays.json | strata | 4.683 | 4.701 | 5.110 | 71.000 | 1.00x |
| wide_arrays.json | orjson | 4.850 | 4.888 | 5.397 | 71.000 | 0.96x |
| wide_arrays.json | msgspec | 5.777 | 5.818 | 6.452 | 71.000 | 0.81x |
| wide_arrays.json | ujson | 7.344 | 7.406 | 7.726 | 71.000 | 0.63x |
| wide_arrays.json | json | 10.337 | 10.421 | 10.753 | 71.000 | 0.45x |
| mixed.json | strata | 0.205 | 0.217 | 0.226 | 72.094 | 1.00x |
| mixed.json | orjson | 0.163 | 0.170 | 0.193 | 72.094 | 1.28x |
| mixed.json | msgspec | 0.181 | 0.187 | 0.248 | 72.094 | 1.16x |
| mixed.json | ujson | 0.242 | 0.248 | 0.278 | 72.094 | 0.88x |
| mixed.json | json | 0.390 | 0.406 | 0.485 | 72.094 | 0.54x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.524 | 3.545 | 3.584 | 58.594 | 1.00x |
| users.json | orjson | 2.574 | 2.598 | 2.705 | 58.594 | 1.36x |
| users.json | msgspec | 3.400 | 3.414 | 3.578 | 58.594 | 1.04x |
| users.json | ujson | 13.765 | 13.846 | 14.249 | 58.594 | 0.26x |
| users.json | json | 19.562 | 19.737 | 20.259 | 58.594 | 0.18x |
| flat.json | strata | 0.342 | 0.349 | 0.356 | 70.766 | 1.00x |
| flat.json | orjson | 0.293 | 0.295 | 0.305 | 70.766 | 1.18x |
| flat.json | msgspec | 0.383 | 0.387 | 0.461 | 70.766 | 0.90x |
| flat.json | ujson | 1.131 | 1.143 | 1.196 | 70.766 | 0.30x |
| flat.json | json | 1.625 | 1.665 | 1.819 | 70.766 | 0.21x |
| nested.json | strata | 0.278 | 0.285 | 0.302 | 70.797 | 1.00x |
| nested.json | orjson | 0.261 | 0.269 | 0.283 | 70.797 | 1.06x |
| nested.json | msgspec | 0.353 | 0.371 | 0.511 | 70.797 | 0.77x |
| nested.json | ujson | 1.245 | 1.266 | 1.388 | 70.797 | 0.23x |
| nested.json | json | 2.115 | 2.160 | 2.300 | 70.797 | 0.13x |
| wide_arrays.json | strata | 1.956 | 1.978 | 2.071 | 71.000 | 1.00x |
| wide_arrays.json | orjson | 1.450 | 1.463 | 1.617 | 71.000 | 1.35x |
| wide_arrays.json | msgspec | 2.504 | 2.539 | 2.750 | 71.000 | 0.78x |
| wide_arrays.json | ujson | 7.819 | 7.882 | 7.929 | 71.000 | 0.25x |
| wide_arrays.json | json | 13.980 | 14.181 | 14.235 | 71.000 | 0.14x |
| mixed.json | strata | 0.082 | 0.084 | 0.087 | 72.094 | 1.00x |
| mixed.json | orjson | 0.057 | 0.059 | 0.071 | 72.094 | 1.41x |
| mixed.json | msgspec | 0.064 | 0.067 | 0.073 | 72.094 | 1.25x |
| mixed.json | ujson | 0.276 | 0.281 | 0.288 | 72.094 | 0.30x |
| mixed.json | json | 0.444 | 0.453 | 0.471 | 72.094 | 0.18x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.237 | 10.420 | 10.626 | 72.094 | 1.00x |
| users.json | orjson | 13.006 | 13.216 | 13.785 | 72.094 | 0.79x |
| users.json | msgspec | 13.134 | 13.372 | 13.693 | 72.094 | 0.78x |
| users.json | ujson | 16.997 | 17.125 | 18.661 | 72.094 | 0.61x |
| users.json | json | 21.387 | 22.114 | 23.060 | 72.094 | 0.47x |
| flat.json | strata | 1.067 | 1.086 | 1.125 | 70.766 | 1.00x |
| flat.json | orjson | 0.918 | 0.947 | 1.079 | 70.766 | 1.15x |
| flat.json | msgspec | 0.969 | 0.982 | 1.055 | 70.766 | 1.11x |
| flat.json | ujson | 1.451 | 1.463 | 1.552 | 70.766 | 0.74x |
| flat.json | json | 1.911 | 1.926 | 2.151 | 70.766 | 0.56x |
| nested.json | strata | 0.860 | 0.869 | 0.924 | 70.797 | 1.00x |
| nested.json | orjson | 1.736 | 1.769 | 1.922 | 70.797 | 0.49x |
| nested.json | msgspec | 1.741 | 1.779 | 1.933 | 70.797 | 0.49x |
| nested.json | ujson | 2.120 | 2.189 | 2.258 | 70.797 | 0.40x |
| nested.json | json | 2.703 | 2.729 | 2.813 | 70.797 | 0.32x |
| wide_arrays.json | strata | 4.877 | 4.940 | 5.036 | 71.000 | 1.00x |
| wide_arrays.json | orjson | 5.442 | 5.730 | 5.963 | 71.000 | 0.86x |
| wide_arrays.json | msgspec | 6.462 | 6.685 | 7.055 | 71.000 | 0.74x |
| wide_arrays.json | ujson | 8.182 | 8.584 | 8.890 | 71.000 | 0.58x |
| wide_arrays.json | json | 11.076 | 11.535 | 11.914 | 71.000 | 0.43x |
| mixed.json | strata | 0.257 | 0.269 | 0.293 | 72.094 | 1.00x |
| mixed.json | orjson | 0.222 | 0.233 | 0.302 | 72.094 | 1.15x |
| mixed.json | msgspec | 0.233 | 0.253 | 0.327 | 72.094 | 1.06x |
| mixed.json | ujson | 0.310 | 0.342 | 0.408 | 72.094 | 0.79x |
| mixed.json | json | 0.442 | 0.465 | 0.513 | 72.094 | 0.58x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.789 | 9.869 | 10.117 | 69.812 | 1.00x |
| users.ndjson | orjson | 13.414 | 13.805 | 15.010 | 69.812 | 0.71x |
| users.ndjson | msgspec | 13.104 | 13.286 | 13.803 | 69.812 | 0.74x |
| users.ndjson | ujson | 16.635 | 16.992 | 17.828 | 69.812 | 0.58x |
| users.ndjson | json | 23.264 | 23.944 | 25.269 | 69.812 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.972 | 4.026 | 4.266 | 67.125 | 1.00x |
| users.json | orjson | 3.048 | 3.111 | 3.341 | 67.125 | 1.29x |
| users.json | msgspec | 3.834 | 3.874 | 4.096 | 67.125 | 1.04x |
| users.json | ujson | 14.104 | 14.233 | 14.407 | 67.125 | 0.28x |
| users.json | json | 20.028 | 20.263 | 20.499 | 67.125 | 0.20x |
| flat.json | strata | 0.526 | 0.536 | 0.635 | 70.781 | 1.00x |
| flat.json | orjson | 0.451 | 0.474 | 0.521 | 70.781 | 1.13x |
| flat.json | msgspec | 0.535 | 0.553 | 0.647 | 70.781 | 0.97x |
| flat.json | ujson | 1.350 | 1.385 | 1.488 | 70.781 | 0.39x |
| flat.json | json | 1.803 | 1.829 | 1.897 | 70.781 | 0.29x |
| nested.json | strata | 0.437 | 0.473 | 0.546 | 70.797 | 1.00x |
| nested.json | orjson | 0.403 | 0.424 | 0.490 | 70.797 | 1.12x |
| nested.json | msgspec | 0.496 | 0.512 | 0.567 | 70.797 | 0.92x |
| nested.json | ujson | 1.424 | 1.457 | 1.504 | 70.797 | 0.32x |
| nested.json | json | 2.264 | 2.303 | 2.419 | 70.797 | 0.21x |
| wide_arrays.json | strata | 2.346 | 2.405 | 2.561 | 72.094 | 1.00x |
| wide_arrays.json | orjson | 1.773 | 1.823 | 1.923 | 72.094 | 1.32x |
| wide_arrays.json | msgspec | 2.821 | 2.865 | 2.954 | 72.094 | 0.84x |
| wide_arrays.json | ujson | 8.190 | 8.318 | 8.649 | 72.094 | 0.29x |
| wide_arrays.json | json | 14.363 | 14.523 | 14.765 | 72.094 | 0.17x |
| mixed.json | strata | 0.226 | 0.250 | 0.361 | 65.391 | 1.00x |
| mixed.json | orjson | 0.166 | 0.207 | 0.228 | 65.391 | 1.21x |
| mixed.json | msgspec | 0.184 | 0.196 | 0.283 | 65.391 | 1.28x |
| mixed.json | ujson | 0.402 | 0.427 | 0.679 | 65.391 | 0.59x |
| mixed.json | json | 0.562 | 0.575 | 0.707 | 65.391 | 0.43x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.064 | 0.076 | 0.099 | 67.438 | 1.00x |
| users.json $[*].id | jmespath | 0.435 | 0.446 | 0.545 | 67.438 | 0.17x |
| users.json $[*].id | jsonpath-ng | 4.207 | 4.261 | 4.605 | 67.438 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.436 | 0.460 | 0.560 | 68.859 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.703 | 2.872 | 3.218 | 68.859 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 14.655 | 15.051 | 16.686 | 68.859 | 0.03x |
| users.json $..total | strata | 1.768 | 1.787 | 1.799 | 71.016 | 1.00x |
| users.json $..total | jsonpath-ng | 272.436 | 273.987 | 277.852 | 71.016 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.833 | 4.902 | 5.062 | 68.578 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.462 | 14.299 | 15.100 | 68.578 | 0.34x |
| users.json $[*].id | orjson+jsonpath-ng | 15.004 | 15.620 | 16.339 | 68.578 | 0.31x |
| users.json $[*].orders[*].total | strata | 4.936 | 5.010 | 5.093 | 70.938 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.718 | 15.914 | 16.642 | 70.938 | 0.31x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 25.467 | 26.038 | 27.484 | 70.938 | 0.19x |
| users.json $..total | strata | 14.510 | 14.967 | 16.047 | 71.938 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 282.926 | 284.581 | 288.136 | 71.938 | 0.05x |

