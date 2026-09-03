# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 74164cd
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: x86_64
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.372 | 8.781 | 12.601 | 60.242 | 1.00x |
| users.json | orjson | 11.090 | 11.452 | 22.522 | 60.242 | 0.77x |
| users.json | msgspec | 10.820 | 11.160 | 18.532 | 60.242 | 0.79x |
| users.json | ujson | 15.078 | 18.025 | 29.929 | 60.242 | 0.49x |
| users.json | pysimdjson | 16.547 | 17.142 | 24.692 | 60.242 | 0.51x |
| users.json | json | 16.485 | 17.740 | 18.781 | 60.242 | 0.49x |
| flat.json | strata | 0.730 | 0.751 | 0.766 | 70.402 | 1.00x |
| flat.json | orjson | 0.849 | 0.860 | 0.897 | 70.402 | 0.87x |
| flat.json | msgspec | 0.792 | 0.816 | 0.836 | 70.402 | 0.92x |
| flat.json | ujson | 1.270 | 1.312 | 2.017 | 70.402 | 0.57x |
| flat.json | pysimdjson | 1.291 | 1.329 | 1.462 | 70.402 | 0.57x |
| flat.json | json | 1.401 | 1.465 | 1.473 | 70.402 | 0.51x |
| nested.json | strata | 0.620 | 0.639 | 0.656 | 70.402 | 1.00x |
| nested.json | orjson | 0.778 | 0.794 | 0.816 | 70.402 | 0.80x |
| nested.json | msgspec | 0.745 | 0.757 | 0.779 | 70.402 | 0.84x |
| nested.json | ujson | 1.115 | 1.128 | 1.223 | 70.402 | 0.57x |
| nested.json | pysimdjson | 1.130 | 1.147 | 1.181 | 70.402 | 0.56x |
| nested.json | json | 1.425 | 1.541 | 1.591 | 70.402 | 0.41x |
| wide_arrays.json | strata | 3.920 | 4.205 | 5.130 | 75.680 | 1.00x |
| wide_arrays.json | orjson | 4.457 | 4.825 | 5.798 | 75.680 | 0.87x |
| wide_arrays.json | msgspec | 4.770 | 5.171 | 6.050 | 75.680 | 0.81x |
| wide_arrays.json | ujson | 6.102 | 6.368 | 8.148 | 75.680 | 0.66x |
| wide_arrays.json | pysimdjson | 5.050 | 5.484 | 6.394 | 75.680 | 0.77x |
| wide_arrays.json | json | 7.688 | 7.892 | 9.489 | 75.680 | 0.53x |
| mixed.json | strata | 0.152 | 0.163 | 0.166 | 75.742 | 1.00x |
| mixed.json | orjson | 0.187 | 0.193 | 0.208 | 75.742 | 0.84x |
| mixed.json | msgspec | 0.191 | 0.200 | 0.208 | 75.742 | 0.82x |
| mixed.json | ujson | 0.241 | 0.256 | 0.271 | 75.742 | 0.64x |
| mixed.json | pysimdjson | 0.237 | 0.244 | 0.253 | 75.742 | 0.67x |
| mixed.json | json | 0.352 | 0.363 | 0.483 | 75.742 | 0.45x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.996 | 2.066 | 2.230 | 59.336 | 1.00x |
| users.json | orjson | 2.017 | 2.029 | 2.844 | 59.336 | 1.02x |
| users.json | msgspec | 3.252 | 3.295 | 5.276 | 59.336 | 0.63x |
| users.json | ujson | 11.312 | 11.517 | 13.480 | 59.336 | 0.18x |
| users.json | json | 17.050 | 17.244 | 17.681 | 59.336 | 0.12x |
| flat.json | strata | 0.227 | 0.234 | 0.269 | 70.402 | 1.00x |
| flat.json | orjson | 0.241 | 0.248 | 0.252 | 70.402 | 0.94x |
| flat.json | msgspec | 0.372 | 0.383 | 0.397 | 70.402 | 0.61x |
| flat.json | ujson | 0.968 | 0.983 | 1.033 | 70.402 | 0.24x |
| flat.json | json | 1.457 | 1.470 | 1.691 | 70.402 | 0.16x |
| nested.json | strata | 0.182 | 0.187 | 0.195 | 70.402 | 1.00x |
| nested.json | orjson | 0.220 | 0.224 | 0.234 | 70.402 | 0.83x |
| nested.json | msgspec | 0.327 | 0.336 | 0.349 | 70.402 | 0.56x |
| nested.json | ujson | 1.045 | 1.057 | 1.231 | 70.402 | 0.18x |
| nested.json | json | 1.824 | 1.852 | 1.883 | 70.402 | 0.10x |
| wide_arrays.json | strata | 1.360 | 1.375 | 2.047 | 75.680 | 1.00x |
| wide_arrays.json | orjson | 1.400 | 1.414 | 1.479 | 75.680 | 0.97x |
| wide_arrays.json | msgspec | 2.316 | 2.333 | 2.376 | 75.680 | 0.59x |
| wide_arrays.json | ujson | 6.600 | 6.669 | 6.782 | 75.680 | 0.21x |
| wide_arrays.json | json | 13.021 | 13.178 | 13.862 | 75.680 | 0.10x |
| mixed.json | strata | 0.053 | 0.054 | 0.056 | 75.742 | 1.00x |
| mixed.json | orjson | 0.049 | 0.051 | 0.063 | 75.742 | 1.05x |
| mixed.json | msgspec | 0.069 | 0.069 | 0.072 | 75.742 | 0.77x |
| mixed.json | ujson | 0.239 | 0.241 | 0.284 | 75.742 | 0.22x |
| mixed.json | json | 0.398 | 0.407 | 0.419 | 75.742 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.971 | 10.232 | 11.378 | 74.984 | 1.00x |
| users.json | orjson | 11.723 | 12.307 | 12.861 | 74.984 | 0.83x |
| users.json | msgspec | 11.329 | 11.933 | 12.637 | 74.984 | 0.86x |
| users.json | ujson | 15.720 | 17.204 | 18.880 | 74.984 | 0.59x |
| users.json | json | 17.151 | 17.880 | 23.013 | 74.984 | 0.57x |
| flat.json | strata | 0.762 | 0.771 | 0.812 | 70.402 | 1.00x |
| flat.json | orjson | 0.895 | 0.902 | 0.945 | 70.402 | 0.85x |
| flat.json | msgspec | 0.849 | 0.861 | 0.886 | 70.402 | 0.90x |
| flat.json | ujson | 1.274 | 1.323 | 1.369 | 70.402 | 0.58x |
| flat.json | json | 1.411 | 1.447 | 1.495 | 70.402 | 0.53x |
| nested.json | strata | 0.662 | 0.673 | 1.054 | 70.402 | 1.00x |
| nested.json | orjson | 0.846 | 0.861 | 1.134 | 70.402 | 0.78x |
| nested.json | msgspec | 0.792 | 0.820 | 0.943 | 70.402 | 0.82x |
| nested.json | ujson | 1.194 | 1.229 | 1.269 | 70.402 | 0.55x |
| nested.json | json | 1.503 | 1.531 | 1.625 | 70.402 | 0.44x |
| wide_arrays.json | strata | 4.018 | 4.075 | 4.193 | 75.742 | 1.00x |
| wide_arrays.json | orjson | 4.496 | 4.639 | 4.870 | 75.742 | 0.88x |
| wide_arrays.json | msgspec | 4.938 | 5.011 | 5.312 | 75.742 | 0.81x |
| wide_arrays.json | ujson | 6.315 | 6.360 | 6.892 | 75.742 | 0.64x |
| wide_arrays.json | json | 7.758 | 7.861 | 8.258 | 75.742 | 0.52x |
| mixed.json | strata | 0.169 | 0.175 | 0.184 | 75.742 | 1.00x |
| mixed.json | orjson | 0.222 | 0.229 | 0.304 | 75.742 | 0.76x |
| mixed.json | msgspec | 0.224 | 0.234 | 0.242 | 75.742 | 0.75x |
| mixed.json | ujson | 0.286 | 0.296 | 0.327 | 75.742 | 0.59x |
| mixed.json | json | 0.380 | 0.389 | 0.401 | 75.742 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.706 | 11.583 | 13.541 | 70.402 | 1.00x |
| users.ndjson | orjson | 14.874 | 16.131 | 17.922 | 70.402 | 0.72x |
| users.ndjson | msgspec | 14.465 | 15.804 | 17.785 | 70.402 | 0.73x |
| users.ndjson | ujson | 18.425 | 21.047 | 22.997 | 70.402 | 0.55x |
| users.ndjson | json | 23.121 | 25.842 | 28.023 | 70.402 | 0.45x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.499 | 11.444 | 62.055 | 72.031 | 1.00x |
| users.json | orjson | 2.606 | 2.668 | 56.847 | 72.031 | 4.29x |
| users.json | msgspec | 3.752 | 3.812 | 52.104 | 72.031 | 3.00x |
| users.json | ujson | 12.075 | 12.214 | 49.604 | 72.031 | 0.94x |
| users.json | json | 17.917 | 18.240 | 57.009 | 72.031 | 0.63x |
| flat.json | strata | 0.343 | 0.368 | 0.456 | 70.402 | 1.00x |
| flat.json | orjson | 0.370 | 0.409 | 14.715 | 70.402 | 0.90x |
| flat.json | msgspec | 0.513 | 0.524 | 0.701 | 70.402 | 0.70x |
| flat.json | ujson | 1.130 | 1.148 | 1.539 | 70.402 | 0.32x |
| flat.json | json | 1.639 | 1.664 | 81.492 | 70.402 | 0.22x |
| nested.json | strata | 0.272 | 0.298 | 0.390 | 70.402 | 1.00x |
| nested.json | orjson | 0.327 | 0.347 | 0.407 | 70.402 | 0.86x |
| nested.json | msgspec | 0.433 | 0.459 | 144.974 | 70.402 | 0.65x |
| nested.json | ujson | 1.171 | 1.198 | 1.297 | 70.402 | 0.25x |
| nested.json | json | 1.965 | 2.030 | 22.500 | 70.402 | 0.15x |
| wide_arrays.json | strata | 1.694 | 1.782 | 16.002 | 75.742 | 1.00x |
| wide_arrays.json | orjson | 1.779 | 1.812 | 15.718 | 75.742 | 0.98x |
| wide_arrays.json | msgspec | 2.669 | 23.732 | 106.235 | 75.742 | 0.08x |
| wide_arrays.json | ujson | 7.079 | 7.189 | 12.921 | 75.742 | 0.25x |
| wide_arrays.json | json | 13.526 | 13.666 | 37.674 | 75.742 | 0.13x |
| mixed.json | strata | 0.128 | 0.136 | 0.160 | 75.742 | 1.00x |
| mixed.json | orjson | 0.135 | 0.141 | 12.465 | 75.742 | 0.97x |
| mixed.json | msgspec | 0.150 | 0.154 | 0.176 | 75.742 | 0.89x |
| mixed.json | ujson | 0.332 | 0.340 | 0.354 | 75.742 | 0.40x |
| mixed.json | json | 0.490 | 0.510 | 0.534 | 75.742 | 0.27x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.058 | 0.061 | 0.064 | 72.031 | 1.00x |
| users.json $[*].id | jmespath | 0.372 | 0.378 | 0.405 | 72.031 | 0.16x |
| users.json $[*].id | jsonpath-ng | 2.182 | 2.284 | 2.390 | 72.031 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.334 | 0.359 | 0.439 | 72.031 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.421 | 2.448 | 2.489 | 72.031 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 15.175 | 15.834 | 16.953 | 72.031 | 0.02x |
| users.json $..total | strata | 1.414 | 1.489 | 2.448 | 73.016 | 1.00x |
| users.json $..total | jsonpath-ng | 306.238 | 307.414 | 311.770 | 73.016 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 2.584 | 2.600 | 2.625 | 72.031 | 1.00x |
| users.json $[*].id | orjson+jmespath | 11.974 | 12.265 | 12.397 | 72.031 | 0.21x |
| users.json $[*].id | orjson+jsonpath-ng | 13.773 | 13.999 | 14.270 | 72.031 | 0.19x |
| users.json $[*].orders[*].total | strata | 2.787 | 2.838 | 2.890 | 73.016 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 14.402 | 14.973 | 15.205 | 73.016 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 31.267 | 32.029 | 35.005 | 73.016 | 0.09x |
| users.json $..total | strata | 11.563 | 12.420 | 16.176 | 73.016 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 324.149 | 328.553 | 330.343 | 73.016 | 0.04x |

