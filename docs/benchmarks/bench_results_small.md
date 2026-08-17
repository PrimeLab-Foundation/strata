# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 688ea51
- python: 3.14.3
- implementation: CPython
- platform: macOS-26.3-arm64-arm-64bit-Mach-O
- machine: arm64
- processor: Apple M1 Max
- compiler_flags: -std=c++20 -O3 -march=native
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.898 | 7.041 | 7.371 | 52.953 | 1.00x |
| users.json | orjson | 8.501 | 8.873 | 9.057 | 52.953 | 0.79x |
| users.json | msgspec | 8.565 | 8.942 | 9.377 | 52.953 | 0.79x |
| users.json | ujson | 11.482 | 11.949 | 13.346 | 52.953 | 0.59x |
| users.json | json | 15.269 | 15.587 | 16.098 | 52.953 | 0.45x |
| flat.json | strata | 0.647 | 0.663 | 0.693 | 69.312 | 1.00x |
| flat.json | orjson | 0.648 | 0.653 | 0.737 | 69.312 | 1.01x |
| flat.json | msgspec | 0.688 | 0.699 | 0.731 | 69.312 | 0.95x |
| flat.json | ujson | 1.007 | 1.018 | 1.153 | 69.312 | 0.65x |
| flat.json | json | 1.396 | 1.415 | 1.514 | 69.312 | 0.47x |
| nested.json | strata | 0.540 | 0.548 | 0.710 | 69.328 | 1.00x |
| nested.json | orjson | 1.205 | 1.244 | 1.373 | 69.328 | 0.44x |
| nested.json | msgspec | 1.201 | 1.235 | 1.632 | 69.328 | 0.44x |
| nested.json | ujson | 1.481 | 1.533 | 1.664 | 69.328 | 0.36x |
| nested.json | json | 1.913 | 1.930 | 2.162 | 69.328 | 0.28x |
| wide_arrays.json | strata | 3.255 | 3.342 | 3.674 | 69.328 | 1.00x |
| wide_arrays.json | orjson | 3.809 | 4.047 | 4.528 | 69.328 | 0.83x |
| wide_arrays.json | msgspec | 4.478 | 4.558 | 5.396 | 69.328 | 0.73x |
| wide_arrays.json | ujson | 5.757 | 5.919 | 6.479 | 69.328 | 0.56x |
| wide_arrays.json | json | 8.011 | 8.396 | 8.929 | 69.328 | 0.40x |
| mixed.json | strata | 0.130 | 0.134 | 0.145 | 69.453 | 1.00x |
| mixed.json | orjson | 0.127 | 0.131 | 0.136 | 69.453 | 1.02x |
| mixed.json | msgspec | 0.137 | 0.140 | 0.145 | 69.453 | 0.96x |
| mixed.json | ujson | 0.182 | 0.187 | 0.211 | 69.453 | 0.71x |
| mixed.json | json | 0.305 | 0.319 | 0.370 | 69.453 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.514 | 1.530 | 1.610 | 56.938 | 1.00x |
| users.json | orjson | 1.960 | 2.020 | 2.098 | 56.938 | 0.76x |
| users.json | msgspec | 2.587 | 2.652 | 2.774 | 56.938 | 0.58x |
| users.json | ujson | 10.456 | 10.592 | 10.948 | 56.938 | 0.14x |
| users.json | json | 14.899 | 15.245 | 15.477 | 56.938 | 0.10x |
| flat.json | strata | 0.179 | 0.184 | 0.223 | 69.312 | 1.00x |
| flat.json | orjson | 0.225 | 0.228 | 0.257 | 69.312 | 0.80x |
| flat.json | msgspec | 0.293 | 0.299 | 0.316 | 69.312 | 0.61x |
| flat.json | ujson | 0.861 | 0.873 | 0.978 | 69.312 | 0.21x |
| flat.json | json | 1.243 | 1.273 | 1.426 | 69.312 | 0.14x |
| nested.json | strata | 0.127 | 0.133 | 0.141 | 69.328 | 1.00x |
| nested.json | orjson | 0.199 | 0.205 | 0.212 | 69.328 | 0.65x |
| nested.json | msgspec | 0.265 | 0.273 | 0.309 | 69.328 | 0.49x |
| nested.json | ujson | 0.960 | 0.975 | 1.067 | 69.328 | 0.14x |
| nested.json | json | 1.587 | 1.647 | 1.721 | 69.328 | 0.08x |
| wide_arrays.json | strata | 1.111 | 1.178 | 1.203 | 69.453 | 1.00x |
| wide_arrays.json | orjson | 1.106 | 1.167 | 1.218 | 69.453 | 1.01x |
| wide_arrays.json | msgspec | 1.913 | 2.080 | 2.492 | 69.453 | 0.57x |
| wide_arrays.json | ujson | 6.009 | 6.200 | 6.265 | 69.453 | 0.19x |
| wide_arrays.json | json | 10.815 | 11.057 | 11.378 | 69.453 | 0.11x |
| mixed.json | strata | 0.038 | 0.047 | 0.057 | 69.453 | 1.00x |
| mixed.json | orjson | 0.043 | 0.047 | 0.053 | 69.453 | 1.00x |
| mixed.json | msgspec | 0.050 | 0.054 | 0.073 | 69.453 | 0.86x |
| mixed.json | ujson | 0.212 | 0.215 | 0.227 | 69.453 | 0.22x |
| mixed.json | json | 0.341 | 0.357 | 0.453 | 69.453 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.035 | 7.249 | 7.497 | 70.578 | 1.00x |
| users.json | orjson | 10.345 | 10.718 | 11.232 | 70.578 | 0.68x |
| users.json | msgspec | 10.622 | 11.061 | 11.574 | 70.578 | 0.66x |
| users.json | ujson | 13.522 | 14.127 | 15.010 | 70.578 | 0.51x |
| users.json | json | 16.865 | 17.805 | 18.017 | 70.578 | 0.41x |
| flat.json | strata | 0.723 | 0.746 | 0.813 | 69.312 | 1.00x |
| flat.json | orjson | 0.724 | 0.754 | 0.843 | 69.312 | 0.99x |
| flat.json | msgspec | 0.752 | 0.836 | 0.951 | 69.312 | 0.89x |
| flat.json | ujson | 1.125 | 1.162 | 1.296 | 69.312 | 0.64x |
| flat.json | json | 1.476 | 1.528 | 1.637 | 69.312 | 0.49x |
| nested.json | strata | 0.605 | 0.617 | 0.667 | 69.328 | 1.00x |
| nested.json | orjson | 1.396 | 1.423 | 1.472 | 69.328 | 0.43x |
| nested.json | msgspec | 1.381 | 1.405 | 1.892 | 69.328 | 0.44x |
| nested.json | ujson | 1.678 | 1.732 | 1.859 | 69.328 | 0.36x |
| nested.json | json | 2.095 | 2.127 | 2.469 | 69.328 | 0.29x |
| wide_arrays.json | strata | 3.447 | 3.579 | 3.724 | 69.453 | 1.00x |
| wide_arrays.json | orjson | 4.350 | 4.531 | 4.907 | 69.453 | 0.79x |
| wide_arrays.json | msgspec | 5.211 | 5.443 | 5.648 | 69.453 | 0.66x |
| wide_arrays.json | ujson | 6.449 | 6.736 | 6.996 | 69.453 | 0.53x |
| wide_arrays.json | json | 8.505 | 8.903 | 9.467 | 69.453 | 0.40x |
| mixed.json | strata | 0.179 | 0.193 | 0.256 | 69.453 | 1.00x |
| mixed.json | orjson | 0.191 | 0.226 | 0.275 | 69.453 | 0.86x |
| mixed.json | msgspec | 0.208 | 0.223 | 0.258 | 69.453 | 0.87x |
| mixed.json | ujson | 0.254 | 0.285 | 0.340 | 69.453 | 0.68x |
| mixed.json | json | 0.353 | 0.382 | 0.430 | 69.453 | 0.51x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.112 | 7.391 | 7.546 | 68.453 | 1.00x |
| users.ndjson | orjson | 10.390 | 10.678 | 11.543 | 68.453 | 0.69x |
| users.ndjson | msgspec | 10.115 | 10.478 | 11.220 | 68.453 | 0.71x |
| users.ndjson | ujson | 12.979 | 13.519 | 13.915 | 68.453 | 0.55x |
| users.ndjson | json | 17.949 | 18.666 | 19.632 | 68.453 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.903 | 1.975 | 11.525 | 67.625 | 1.00x |
| users.json | orjson | 2.364 | 2.448 | 7.687 | 67.625 | 0.81x |
| users.json | msgspec | 2.976 | 3.023 | 3.410 | 67.625 | 0.65x |
| users.json | ujson | 10.809 | 11.012 | 11.299 | 67.625 | 0.18x |
| users.json | json | 15.374 | 15.489 | 15.961 | 67.625 | 0.13x |
| flat.json | strata | 0.315 | 0.343 | 0.389 | 69.328 | 1.00x |
| flat.json | orjson | 0.366 | 0.391 | 0.459 | 69.328 | 0.88x |
| flat.json | msgspec | 0.417 | 0.453 | 0.514 | 69.328 | 0.76x |
| flat.json | ujson | 1.017 | 1.041 | 1.097 | 69.328 | 0.33x |
| flat.json | json | 1.387 | 1.474 | 1.557 | 69.328 | 0.23x |
| nested.json | strata | 0.253 | 0.272 | 0.310 | 69.328 | 1.00x |
| nested.json | orjson | 0.320 | 0.340 | 0.502 | 69.328 | 0.80x |
| nested.json | msgspec | 0.393 | 0.423 | 0.432 | 69.328 | 0.64x |
| nested.json | ujson | 1.122 | 1.153 | 1.277 | 69.328 | 0.24x |
| nested.json | json | 1.713 | 1.754 | 1.833 | 69.328 | 0.15x |
| wide_arrays.json | strata | 1.407 | 1.459 | 1.738 | 69.453 | 1.00x |
| wide_arrays.json | orjson | 1.451 | 1.484 | 1.779 | 69.453 | 0.98x |
| wide_arrays.json | msgspec | 2.217 | 2.299 | 2.390 | 69.453 | 0.63x |
| wide_arrays.json | ujson | 6.307 | 6.490 | 6.724 | 69.453 | 0.22x |
| wide_arrays.json | json | 11.106 | 11.349 | 11.615 | 69.453 | 0.13x |
| mixed.json | strata | 0.146 | 0.158 | 0.238 | 69.484 | 1.00x |
| mixed.json | orjson | 0.157 | 0.172 | 0.203 | 69.484 | 0.92x |
| mixed.json | msgspec | 0.159 | 0.183 | 0.280 | 69.484 | 0.86x |
| mixed.json | ujson | 0.333 | 0.349 | 0.415 | 69.484 | 0.45x |
| mixed.json | json | 0.454 | 0.482 | 0.530 | 69.484 | 0.33x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.057 | 0.065 | 0.082 | 67.906 | 1.00x |
| users.json $[*].id | jmespath | 0.336 | 0.363 | 0.435 | 67.906 | 0.18x |
| users.json $[*].id | jsonpath-ng | 3.391 | 3.546 | 3.941 | 67.906 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.350 | 0.374 | 0.393 | 68.297 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.041 | 2.071 | 2.461 | 68.297 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 11.215 | 11.606 | 12.735 | 68.297 | 0.03x |
| users.json $..total | strata | 1.351 | 1.372 | 1.449 | 70.562 | 1.00x |
| users.json $..total | jsonpath-ng | 209.428 | 211.568 | 215.979 | 70.562 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.267 | 3.336 | 3.498 | 68.016 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.756 | 11.137 | 11.581 | 68.016 | 0.30x |
| users.json $[*].id | orjson+jsonpath-ng | 11.955 | 12.234 | 12.961 | 68.016 | 0.27x |
| users.json $[*].orders[*].total | strata | 3.357 | 3.426 | 3.573 | 70.500 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.523 | 12.826 | 13.642 | 70.500 | 0.27x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 20.114 | 20.403 | 21.563 | 70.500 | 0.17x |
| users.json $..total | strata | 10.523 | 10.938 | 11.590 | 70.547 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 218.600 | 219.614 | 221.140 | 70.547 | 0.05x |

