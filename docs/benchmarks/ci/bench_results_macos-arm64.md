# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: e6c5e36
- python: 3.12.10
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit
- machine: arm64
- processor: Apple M1 (Virtual)
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.450 | 7.926 | 13.454 | 63.281 | 1.00x |
| users.json | orjson | 10.407 | 11.693 | 16.616 | 63.281 | 0.68x |
| users.json | msgspec | 10.802 | 14.828 | 19.197 | 63.281 | 0.53x |
| users.json | ujson | 13.757 | 16.054 | 22.313 | 63.281 | 0.49x |
| users.json | pysimdjson | 132.437 | 154.296 | 182.552 | 63.281 | 0.05x |
| users.json | json | 14.530 | 18.013 | 32.483 | 63.281 | 0.44x |
| flat.json | strata | 0.614 | 0.649 | 1.064 | 92.109 | 1.00x |
| flat.json | orjson | 0.761 | 0.830 | 1.138 | 92.109 | 0.78x |
| flat.json | msgspec | 0.701 | 0.767 | 1.118 | 92.109 | 0.85x |
| flat.json | ujson | 1.191 | 1.327 | 1.731 | 92.109 | 0.49x |
| flat.json | pysimdjson | 11.962 | 12.349 | 13.393 | 92.109 | 0.05x |
| flat.json | json | 1.357 | 1.405 | 1.511 | 92.109 | 0.46x |
| nested.json | strata | 0.568 | 0.604 | 0.675 | 92.141 | 1.00x |
| nested.json | orjson | 0.794 | 0.835 | 0.870 | 92.141 | 0.72x |
| nested.json | msgspec | 0.674 | 0.794 | 0.841 | 92.141 | 0.76x |
| nested.json | ujson | 1.077 | 1.315 | 1.384 | 92.141 | 0.46x |
| nested.json | pysimdjson | 10.748 | 11.124 | 12.584 | 92.141 | 0.05x |
| nested.json | json | 1.463 | 1.529 | 1.852 | 92.141 | 0.39x |
| wide_arrays.json | strata | 3.903 | 4.065 | 4.859 | 95.578 | 1.00x |
| wide_arrays.json | orjson | 4.335 | 4.486 | 5.278 | 95.578 | 0.91x |
| wide_arrays.json | msgspec | 4.594 | 4.890 | 6.278 | 95.578 | 0.83x |
| wide_arrays.json | ujson | 6.075 | 6.499 | 6.757 | 95.578 | 0.63x |
| wide_arrays.json | pysimdjson | 65.699 | 71.917 | 81.502 | 95.578 | 0.06x |
| wide_arrays.json | json | 7.779 | 8.130 | 11.343 | 95.578 | 0.50x |
| mixed.json | strata | 0.142 | 0.164 | 0.510 | 95.594 | 1.00x |
| mixed.json | orjson | 0.175 | 0.199 | 0.268 | 95.594 | 0.82x |
| mixed.json | msgspec | 0.192 | 0.205 | 0.222 | 95.594 | 0.80x |
| mixed.json | ujson | 0.234 | 0.409 | 0.979 | 95.594 | 0.40x |
| mixed.json | pysimdjson | 2.578 | 2.829 | 3.252 | 95.594 | 0.06x |
| mixed.json | json | 0.332 | 0.392 | 1.055 | 95.594 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.868 | 1.981 | 2.756 | 77.828 | 1.00x |
| users.json | orjson | 2.465 | 2.696 | 5.275 | 77.828 | 0.74x |
| users.json | msgspec | 3.057 | 3.382 | 3.664 | 77.828 | 0.59x |
| users.json | ujson | 11.923 | 13.038 | 19.523 | 77.828 | 0.15x |
| users.json | json | 17.732 | 19.196 | 26.122 | 77.828 | 0.10x |
| flat.json | strata | 0.213 | 0.249 | 1.808 | 92.125 | 1.00x |
| flat.json | orjson | 0.274 | 0.290 | 1.727 | 92.125 | 0.86x |
| flat.json | msgspec | 0.334 | 0.352 | 0.391 | 92.125 | 0.71x |
| flat.json | ujson | 0.956 | 0.981 | 1.080 | 92.125 | 0.25x |
| flat.json | json | 1.436 | 1.497 | 1.546 | 92.125 | 0.17x |
| nested.json | strata | 0.149 | 0.164 | 0.217 | 92.141 | 1.00x |
| nested.json | orjson | 0.251 | 0.265 | 0.303 | 92.141 | 0.62x |
| nested.json | msgspec | 0.309 | 0.324 | 0.485 | 92.141 | 0.51x |
| nested.json | ujson | 1.090 | 1.220 | 1.297 | 92.141 | 0.13x |
| nested.json | json | 1.699 | 1.765 | 2.113 | 92.141 | 0.09x |
| wide_arrays.json | strata | 1.320 | 1.408 | 2.075 | 95.578 | 1.00x |
| wide_arrays.json | orjson | 1.641 | 1.868 | 2.085 | 95.578 | 0.75x |
| wide_arrays.json | msgspec | 2.332 | 2.417 | 3.616 | 95.578 | 0.58x |
| wide_arrays.json | ujson | 7.099 | 7.336 | 11.163 | 95.578 | 0.19x |
| wide_arrays.json | json | 12.725 | 13.001 | 14.293 | 95.578 | 0.11x |
| mixed.json | strata | 0.046 | 0.056 | 0.070 | 95.594 | 1.00x |
| mixed.json | orjson | 0.050 | 0.058 | 0.076 | 95.594 | 0.96x |
| mixed.json | msgspec | 0.055 | 0.191 | 0.367 | 95.594 | 0.29x |
| mixed.json | ujson | 0.218 | 0.246 | 0.286 | 95.594 | 0.23x |
| mixed.json | json | 0.345 | 0.388 | 0.452 | 95.594 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.510 | 9.471 | 19.175 | 89.359 | 1.00x |
| users.json | orjson | 11.273 | 15.330 | 34.229 | 89.359 | 0.62x |
| users.json | msgspec | 10.357 | 13.667 | 28.760 | 89.359 | 0.69x |
| users.json | ujson | 15.018 | 17.320 | 22.294 | 89.359 | 0.55x |
| users.json | json | 16.992 | 18.200 | 36.715 | 89.359 | 0.52x |
| flat.json | strata | 0.680 | 0.745 | 0.822 | 92.125 | 1.00x |
| flat.json | orjson | 0.902 | 1.108 | 1.277 | 92.125 | 0.67x |
| flat.json | msgspec | 0.782 | 0.842 | 1.656 | 92.125 | 0.88x |
| flat.json | ujson | 1.104 | 1.309 | 1.641 | 92.125 | 0.57x |
| flat.json | json | 1.430 | 1.552 | 1.784 | 92.125 | 0.48x |
| nested.json | strata | 0.661 | 0.708 | 1.030 | 92.141 | 1.00x |
| nested.json | orjson | 1.005 | 1.124 | 2.937 | 92.141 | 0.63x |
| nested.json | msgspec | 0.868 | 0.955 | 1.214 | 92.141 | 0.74x |
| nested.json | ujson | 1.224 | 1.372 | 1.738 | 92.141 | 0.52x |
| nested.json | json | 1.668 | 1.807 | 4.282 | 92.141 | 0.39x |
| wide_arrays.json | strata | 3.914 | 4.057 | 4.366 | 95.578 | 1.00x |
| wide_arrays.json | orjson | 4.250 | 4.416 | 5.910 | 95.578 | 0.92x |
| wide_arrays.json | msgspec | 4.664 | 4.830 | 7.696 | 95.578 | 0.84x |
| wide_arrays.json | ujson | 6.318 | 6.657 | 10.218 | 95.578 | 0.61x |
| wide_arrays.json | json | 7.151 | 7.977 | 10.750 | 95.578 | 0.51x |
| mixed.json | strata | 0.166 | 0.201 | 0.244 | 95.594 | 1.00x |
| mixed.json | orjson | 0.443 | 0.519 | 0.569 | 95.594 | 0.39x |
| mixed.json | msgspec | 0.228 | 0.275 | 0.329 | 95.594 | 0.73x |
| mixed.json | ujson | 0.270 | 0.328 | 0.361 | 95.594 | 0.61x |
| mixed.json | json | 0.382 | 0.426 | 0.487 | 95.594 | 0.47x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.631 | 8.888 | 13.170 | 92.109 | 1.00x |
| users.ndjson | orjson | 12.746 | 13.576 | 15.030 | 92.109 | 0.65x |
| users.ndjson | msgspec | 12.306 | 13.365 | 21.950 | 92.109 | 0.67x |
| users.ndjson | ujson | 15.205 | 16.451 | 18.674 | 92.109 | 0.54x |
| users.ndjson | json | 20.671 | 22.364 | 23.850 | 92.109 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.409 | 2.666 | 8.925 | 86.328 | 1.00x |
| users.json | orjson | 2.953 | 3.378 | 8.544 | 86.328 | 0.79x |
| users.json | msgspec | 3.664 | 4.443 | 15.824 | 86.328 | 0.60x |
| users.json | ujson | 11.974 | 15.579 | 21.276 | 86.328 | 0.17x |
| users.json | json | 17.348 | 20.383 | 28.241 | 86.328 | 0.13x |
| flat.json | strata | 0.486 | 0.530 | 0.709 | 92.125 | 1.00x |
| flat.json | orjson | 0.492 | 0.635 | 1.398 | 92.125 | 0.83x |
| flat.json | msgspec | 0.514 | 0.720 | 1.693 | 92.125 | 0.74x |
| flat.json | ujson | 1.146 | 1.369 | 1.603 | 92.125 | 0.39x |
| flat.json | json | 1.701 | 1.953 | 3.054 | 92.125 | 0.27x |
| nested.json | strata | 0.446 | 0.514 | 0.598 | 92.141 | 1.00x |
| nested.json | orjson | 0.562 | 0.615 | 0.715 | 92.141 | 0.83x |
| nested.json | msgspec | 0.796 | 0.902 | 1.010 | 92.141 | 0.57x |
| nested.json | ujson | 1.476 | 1.665 | 2.104 | 92.141 | 0.31x |
| nested.json | json | 2.095 | 2.247 | 2.414 | 92.141 | 0.23x |
| wide_arrays.json | strata | 1.526 | 1.843 | 2.290 | 95.578 | 1.00x |
| wide_arrays.json | orjson | 1.915 | 2.234 | 3.135 | 95.578 | 0.82x |
| wide_arrays.json | msgspec | 2.542 | 2.962 | 3.234 | 95.578 | 0.62x |
| wide_arrays.json | ujson | 7.070 | 7.831 | 8.452 | 95.578 | 0.24x |
| wide_arrays.json | json | 11.958 | 13.607 | 15.803 | 95.578 | 0.14x |
| mixed.json | strata | 0.231 | 0.329 | 0.376 | 95.594 | 1.00x |
| mixed.json | orjson | 0.255 | 0.320 | 0.431 | 95.594 | 1.03x |
| mixed.json | msgspec | 0.280 | 0.329 | 0.572 | 95.594 | 1.00x |
| mixed.json | ujson | 0.466 | 0.552 | 0.617 | 95.594 | 0.60x |
| mixed.json | json | 0.666 | 0.720 | 0.908 | 95.594 | 0.46x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.064 | 0.150 | 0.341 | 86.406 | 1.00x |
| users.json $[*].id | jmespath | 0.312 | 0.407 | 1.005 | 86.406 | 0.37x |
| users.json $[*].id | jsonpath-ng | 1.647 | 1.805 | 3.056 | 86.406 | 0.08x |
| users.json $[*].orders[*].total | strata | 0.423 | 0.737 | 1.180 | 86.609 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.968 | 2.178 | 6.536 | 86.609 | 0.34x |
| users.json $[*].orders[*].total | jsonpath-ng | 11.735 | 14.054 | 19.435 | 86.609 | 0.05x |
| users.json $..total | strata | 1.451 | 1.617 | 4.082 | 88.453 | 1.00x |
| users.json $..total | jsonpath-ng | 212.578 | 246.752 | 279.607 | 88.453 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.811 | 4.211 | 5.831 | 86.516 | 1.00x |
| users.json $[*].id | orjson+jmespath | 11.398 | 11.906 | 26.905 | 86.516 | 0.35x |
| users.json $[*].id | orjson+jsonpath-ng | 12.469 | 13.736 | 19.467 | 86.516 | 0.31x |
| users.json $[*].orders[*].total | strata | 4.051 | 4.185 | 5.327 | 88.438 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.256 | 15.132 | 20.773 | 88.438 | 0.28x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 25.829 | 32.405 | 45.546 | 88.438 | 0.13x |
| users.json $..total | strata | 9.037 | 10.353 | 31.344 | 88.500 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 222.421 | 250.416 | 276.530 | 88.500 | 0.04x |

