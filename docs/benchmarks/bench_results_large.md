# Benchmark results - large

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c88786c
- python: 3.14.7
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit-Mach-O
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
| users.json | strata | 315.857 | 327.388 | 350.194 | 493.250 | 1.00x |
| users.json | orjson | 380.473 | 394.025 | 407.954 | 493.250 | 0.83x |
| users.json | msgspec | 389.050 | 401.000 | 416.308 | 493.250 | 0.82x |
| users.json | ujson | 532.458 | 549.105 | 559.943 | 493.250 | 0.60x |
| users.json | json | 783.244 | 795.250 | 809.030 | 493.250 | 0.41x |
| flat.json | strata | 5.842 | 5.873 | 6.015 | 347.750 | 1.00x |
| flat.json | orjson | 6.935 | 6.956 | 7.095 | 347.750 | 0.84x |
| flat.json | msgspec | 7.128 | 7.169 | 7.455 | 347.750 | 0.82x |
| flat.json | ujson | 10.698 | 10.847 | 11.388 | 347.750 | 0.54x |
| flat.json | json | 14.550 | 14.592 | 15.116 | 347.750 | 0.40x |
| nested.json | strata | 5.401 | 5.428 | 5.462 | 350.062 | 1.00x |
| nested.json | orjson | 6.040 | 6.089 | 6.113 | 350.062 | 0.89x |
| nested.json | msgspec | 6.016 | 6.048 | 6.382 | 350.062 | 0.90x |
| nested.json | ujson | 8.956 | 9.030 | 9.702 | 350.062 | 0.60x |
| nested.json | json | 13.779 | 13.825 | 14.020 | 350.062 | 0.39x |
| wide_arrays.json | strata | 32.592 | 32.785 | 33.143 | 421.141 | 1.00x |
| wide_arrays.json | orjson | 34.591 | 35.273 | 36.010 | 421.141 | 0.93x |
| wide_arrays.json | msgspec | 42.414 | 43.179 | 45.242 | 421.141 | 0.76x |
| wide_arrays.json | ujson | 54.490 | 55.308 | 56.237 | 421.141 | 0.59x |
| wide_arrays.json | json | 79.097 | 79.725 | 80.302 | 421.141 | 0.41x |
| mixed.json | strata | 1.315 | 1.319 | 1.364 | 774.297 | 1.00x |
| mixed.json | orjson | 1.474 | 1.480 | 1.518 | 774.297 | 0.89x |
| mixed.json | msgspec | 1.602 | 1.608 | 1.611 | 774.297 | 0.82x |
| mixed.json | ujson | 1.994 | 2.002 | 2.021 | 774.297 | 0.66x |
| mixed.json | json | 3.271 | 3.283 | 3.291 | 774.297 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 71.965 | 73.301 | 80.024 | 2379.062 | 1.00x |
| users.json | orjson | 84.301 | 85.791 | 89.400 | 2379.062 | 0.85x |
| users.json | msgspec | 111.718 | 112.930 | 115.812 | 2379.062 | 0.65x |
| users.json | ujson | 443.163 | 460.518 | 465.962 | 2379.062 | 0.16x |
| users.json | json | 624.283 | 628.935 | 635.766 | 2379.062 | 0.12x |
| flat.json | strata | 2.013 | 2.022 | 2.048 | 349.750 | 1.00x |
| flat.json | orjson | 2.140 | 2.147 | 2.387 | 349.750 | 0.94x |
| flat.json | msgspec | 2.816 | 2.823 | 2.854 | 349.750 | 0.72x |
| flat.json | ujson | 8.769 | 8.774 | 8.815 | 349.750 | 0.23x |
| flat.json | json | 12.994 | 13.029 | 13.913 | 349.750 | 0.16x |
| nested.json | strata | 1.409 | 1.420 | 1.480 | 350.172 | 1.00x |
| nested.json | orjson | 2.009 | 2.024 | 2.068 | 350.172 | 0.70x |
| nested.json | msgspec | 2.607 | 2.614 | 2.675 | 350.172 | 0.54x |
| nested.json | ujson | 9.590 | 9.697 | 9.781 | 350.172 | 0.15x |
| nested.json | json | 15.325 | 15.548 | 16.502 | 350.172 | 0.09x |
| wide_arrays.json | strata | 11.262 | 11.443 | 12.770 | 625.797 | 1.00x |
| wide_arrays.json | orjson | 11.448 | 11.538 | 11.769 | 625.797 | 0.99x |
| wide_arrays.json | msgspec | 19.770 | 20.230 | 20.847 | 625.797 | 0.57x |
| wide_arrays.json | ujson | 62.055 | 62.462 | 63.383 | 625.797 | 0.18x |
| wide_arrays.json | json | 110.615 | 112.930 | 116.161 | 625.797 | 0.10x |
| mixed.json | strata | 0.363 | 0.366 | 0.384 | 774.422 | 1.00x |
| mixed.json | orjson | 0.372 | 0.377 | 0.380 | 774.422 | 0.97x |
| mixed.json | msgspec | 0.411 | 0.414 | 0.426 | 774.422 | 0.88x |
| mixed.json | ujson | 2.103 | 2.106 | 2.112 | 774.422 | 0.17x |
| mixed.json | json | 3.296 | 3.311 | 3.437 | 774.422 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 325.974 | 343.373 | 363.386 | 849.859 | 1.00x |
| users.json | orjson | 393.539 | 411.080 | 438.951 | 849.859 | 0.84x |
| users.json | msgspec | 397.703 | 409.949 | 444.879 | 849.859 | 0.84x |
| users.json | ujson | 553.359 | 566.321 | 576.876 | 849.859 | 0.61x |
| users.json | json | 787.410 | 798.237 | 828.393 | 849.859 | 0.43x |
| flat.json | strata | 6.239 | 6.306 | 6.390 | 350.016 | 1.00x |
| flat.json | orjson | 7.264 | 7.327 | 7.652 | 350.016 | 0.86x |
| flat.json | msgspec | 7.471 | 7.672 | 8.633 | 350.016 | 0.82x |
| flat.json | ujson | 11.294 | 11.478 | 11.736 | 350.016 | 0.55x |
| flat.json | json | 14.994 | 15.137 | 15.372 | 350.016 | 0.42x |
| nested.json | strata | 5.653 | 5.698 | 5.846 | 353.125 | 1.00x |
| nested.json | orjson | 6.260 | 6.301 | 6.477 | 353.125 | 0.90x |
| nested.json | msgspec | 6.262 | 6.320 | 6.417 | 353.125 | 0.90x |
| nested.json | ujson | 9.304 | 9.375 | 9.622 | 353.125 | 0.61x |
| nested.json | json | 13.911 | 13.990 | 14.114 | 353.125 | 0.41x |
| wide_arrays.json | strata | 33.747 | 34.278 | 35.076 | 640.797 | 1.00x |
| wide_arrays.json | orjson | 36.476 | 36.733 | 37.093 | 640.797 | 0.93x |
| wide_arrays.json | msgspec | 44.006 | 44.617 | 45.822 | 640.797 | 0.77x |
| wide_arrays.json | ujson | 57.537 | 58.579 | 59.127 | 640.797 | 0.59x |
| wide_arrays.json | json | 80.996 | 81.323 | 81.715 | 640.797 | 0.42x |
| mixed.json | strata | 1.421 | 1.429 | 1.479 | 774.422 | 1.00x |
| mixed.json | orjson | 1.561 | 1.572 | 1.614 | 774.422 | 0.91x |
| mixed.json | msgspec | 1.681 | 1.702 | 1.715 | 774.422 | 0.84x |
| mixed.json | ujson | 2.131 | 2.150 | 2.184 | 774.422 | 0.66x |
| mixed.json | json | 3.319 | 3.342 | 3.367 | 774.422 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 315.574 | 324.251 | 329.847 | 378.141 | 1.00x |
| users.ndjson | orjson | 662.072 | 666.032 | 678.637 | 378.141 | 0.49x |
| users.ndjson | msgspec | 669.726 | 673.946 | 683.448 | 378.141 | 0.48x |
| users.ndjson | ujson | 838.245 | 844.417 | 859.684 | 378.141 | 0.38x |
| users.ndjson | json | 1015.354 | 1024.348 | 1034.376 | 378.141 | 0.32x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 84.237 | 85.997 | 109.787 | 2061.094 | 1.00x |
| users.json | orjson | 93.253 | 95.413 | 119.421 | 2061.094 | 0.90x |
| users.json | msgspec | 119.790 | 122.255 | 146.864 | 2061.094 | 0.70x |
| users.json | ujson | 439.846 | 444.328 | 475.162 | 2061.094 | 0.19x |
| users.json | json | 616.127 | 618.305 | 644.659 | 2061.094 | 0.14x |
| flat.json | strata | 2.407 | 2.426 | 2.504 | 350.016 | 1.00x |
| flat.json | orjson | 2.530 | 2.574 | 2.643 | 350.016 | 0.94x |
| flat.json | msgspec | 3.243 | 3.265 | 3.437 | 350.016 | 0.74x |
| flat.json | ujson | 9.324 | 9.401 | 9.589 | 350.016 | 0.26x |
| flat.json | json | 13.550 | 13.693 | 13.888 | 350.016 | 0.18x |
| nested.json | strata | 1.728 | 1.779 | 2.030 | 353.125 | 1.00x |
| nested.json | orjson | 2.347 | 2.442 | 2.702 | 353.125 | 0.73x |
| nested.json | msgspec | 2.954 | 3.065 | 3.306 | 353.125 | 0.58x |
| nested.json | ujson | 9.982 | 10.083 | 11.182 | 353.125 | 0.18x |
| nested.json | json | 15.986 | 16.200 | 18.270 | 353.125 | 0.11x |
| wide_arrays.json | strata | 13.055 | 13.442 | 13.909 | 773.141 | 1.00x |
| wide_arrays.json | orjson | 13.218 | 13.509 | 14.314 | 773.141 | 1.00x |
| wide_arrays.json | msgspec | 21.945 | 22.509 | 23.313 | 773.141 | 0.60x |
| wide_arrays.json | ujson | 64.242 | 65.095 | 65.401 | 773.141 | 0.21x |
| wide_arrays.json | json | 113.444 | 114.219 | 114.771 | 773.141 | 0.12x |
| mixed.json | strata | 0.582 | 0.605 | 0.665 | 774.422 | 1.00x |
| mixed.json | orjson | 0.580 | 0.634 | 0.678 | 774.422 | 0.95x |
| mixed.json | msgspec | 0.629 | 0.659 | 0.686 | 774.422 | 0.92x |
| mixed.json | ujson | 2.320 | 2.395 | 2.510 | 774.422 | 0.25x |
| mixed.json | json | 3.506 | 3.558 | 3.642 | 774.422 | 0.17x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.723 | 0.747 | 0.796 | 463.875 | 1.00x |
| users.json $[*].id | jmespath | 1.882 | 1.914 | 1.982 | 463.875 | 0.39x |
| users.json $[*].id | jsonpath-ng | 7.103 | 7.183 | 16.814 | 463.875 | 0.10x |
| users.json $[*].orders[*].total | strata | 13.766 | 13.983 | 14.194 | 548.812 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 31.283 | 32.039 | 32.476 | 548.812 | 0.44x |
| users.json $[*].orders[*].total | jsonpath-ng | 159.375 | 164.011 | 172.464 | 548.812 | 0.09x |
| users.json $..total | strata | 54.237 | 54.489 | 55.021 | 498.734 | 1.00x |
| users.json $..total | jsonpath-ng | 7620.705 | 7632.936 | 7654.028 | 498.734 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 140.576 | 141.498 | 143.249 | 770.328 | 1.00x |
| users.json $[*].id | orjson+jmespath | 387.654 | 392.109 | 396.689 | 770.328 | 0.36x |
| users.json $[*].id | orjson+jsonpath-ng | 472.012 | 477.479 | 487.163 | 770.328 | 0.30x |
| users.json $[*].orders[*].total | strata | 141.387 | 141.897 | 142.620 | 791.656 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 416.979 | 425.080 | 428.954 | 791.656 | 0.33x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 1152.936 | 1159.947 | 1167.863 | 791.656 | 0.12x |
| users.json $..total | strata | 377.620 | 384.600 | 390.100 | 804.688 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 8614.761 | 8631.183 | 8654.588 | 804.688 | 0.04x |

