# Benchmark results - medium

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: bcff500
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
| users.json | strata | 76.210 | 79.726 | 82.478 | 139.312 | 1.00x |
| users.json | orjson | 65.786 | 68.046 | 72.151 | 139.312 | 1.17x |
| users.json | msgspec | 67.708 | 70.786 | 73.455 | 139.312 | 1.13x |
| users.json | ujson | 91.892 | 93.721 | 99.033 | 139.312 | 0.85x |
| users.json | json | 115.731 | 118.383 | 122.427 | 139.312 | 0.67x |
| flat.json | strata | 5.170 | 5.236 | 5.552 | 200.656 | 1.00x |
| flat.json | orjson | 4.509 | 4.585 | 5.111 | 200.656 | 1.14x |
| flat.json | msgspec | 4.646 | 4.742 | 5.152 | 200.656 | 1.10x |
| flat.json | ujson | 6.428 | 6.585 | 7.029 | 200.656 | 0.80x |
| flat.json | json | 8.386 | 8.482 | 8.856 | 200.656 | 0.62x |
| nested.json | strata | 4.194 | 4.244 | 4.439 | 200.812 | 1.00x |
| nested.json | orjson | 3.908 | 3.970 | 4.052 | 200.812 | 1.07x |
| nested.json | msgspec | 3.920 | 4.027 | 4.241 | 200.812 | 1.05x |
| nested.json | ujson | 5.317 | 5.420 | 5.871 | 200.812 | 0.78x |
| nested.json | json | 7.695 | 7.760 | 8.443 | 200.812 | 0.55x |
| wide_arrays.json | strata | 25.777 | 25.930 | 26.553 | 187.172 | 1.00x |
| wide_arrays.json | orjson | 17.076 | 17.349 | 18.436 | 187.172 | 1.49x |
| wide_arrays.json | msgspec | 21.131 | 21.325 | 22.339 | 187.172 | 1.22x |
| wide_arrays.json | ujson | 26.940 | 27.235 | 28.732 | 187.172 | 0.95x |
| wide_arrays.json | json | 39.173 | 39.400 | 40.118 | 187.172 | 0.66x |
| mixed.json | strata | 0.982 | 0.988 | 1.014 | 190.594 | 1.00x |
| mixed.json | orjson | 1.501 | 1.511 | 1.531 | 190.594 | 0.65x |
| mixed.json | msgspec | 1.569 | 1.581 | 1.765 | 190.594 | 0.63x |
| mixed.json | ujson | 1.771 | 1.792 | 1.887 | 190.594 | 0.55x |
| mixed.json | json | 2.374 | 2.406 | 2.608 | 190.594 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 37.113 | 37.196 | 37.453 | 445.766 | 1.00x |
| users.json | orjson | 16.713 | 17.024 | 17.395 | 445.766 | 2.18x |
| users.json | msgspec | 20.681 | 20.770 | 21.019 | 445.766 | 1.79x |
| users.json | ujson | 83.628 | 84.174 | 84.795 | 445.766 | 0.44x |
| users.json | json | 115.567 | 116.055 | 116.560 | 445.766 | 0.32x |
| flat.json | strata | 2.150 | 2.166 | 2.270 | 200.781 | 1.00x |
| flat.json | orjson | 1.140 | 1.157 | 1.254 | 200.781 | 1.87x |
| flat.json | msgspec | 1.497 | 1.508 | 1.624 | 200.781 | 1.44x |
| flat.json | ujson | 4.647 | 4.722 | 4.952 | 200.781 | 0.46x |
| flat.json | json | 6.419 | 6.556 | 6.756 | 200.781 | 0.33x |
| nested.json | strata | 1.975 | 2.020 | 2.100 | 200.812 | 1.00x |
| nested.json | orjson | 1.029 | 1.043 | 1.086 | 200.812 | 1.94x |
| nested.json | msgspec | 1.361 | 1.385 | 1.472 | 200.812 | 1.46x |
| nested.json | ujson | 4.964 | 4.993 | 5.084 | 200.812 | 0.40x |
| nested.json | json | 8.004 | 8.098 | 8.277 | 200.812 | 0.25x |
| wide_arrays.json | strata | 13.975 | 14.058 | 14.361 | 182.359 | 1.00x |
| wide_arrays.json | orjson | 6.108 | 6.294 | 6.426 | 182.359 | 2.23x |
| wide_arrays.json | msgspec | 10.294 | 10.418 | 10.600 | 182.359 | 1.35x |
| wide_arrays.json | ujson | 32.640 | 33.108 | 33.630 | 182.359 | 0.42x |
| wide_arrays.json | json | 56.309 | 56.828 | 57.289 | 182.359 | 0.25x |
| mixed.json | strata | 0.530 | 0.545 | 0.591 | 190.594 | 1.00x |
| mixed.json | orjson | 0.202 | 0.209 | 0.295 | 190.594 | 2.60x |
| mixed.json | msgspec | 0.224 | 0.228 | 0.242 | 190.594 | 2.39x |
| mixed.json | ujson | 1.093 | 1.111 | 1.138 | 190.594 | 0.49x |
| mixed.json | json | 1.704 | 1.733 | 1.921 | 190.594 | 0.31x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 78.179 | 80.263 | 83.678 | 506.734 | 1.00x |
| users.json | orjson | 80.400 | 82.581 | 86.277 | 506.734 | 0.97x |
| users.json | msgspec | 82.546 | 85.188 | 89.247 | 506.734 | 0.94x |
| users.json | ujson | 107.568 | 111.384 | 114.051 | 506.734 | 0.72x |
| users.json | json | 130.023 | 133.636 | 137.155 | 506.734 | 0.60x |
| flat.json | strata | 5.382 | 5.411 | 5.501 | 200.781 | 1.00x |
| flat.json | orjson | 4.989 | 5.082 | 5.391 | 200.781 | 1.06x |
| flat.json | msgspec | 5.127 | 5.182 | 5.609 | 200.781 | 1.04x |
| flat.json | ujson | 7.130 | 7.169 | 7.568 | 200.781 | 0.75x |
| flat.json | json | 8.871 | 8.910 | 9.406 | 200.781 | 0.61x |
| nested.json | strata | 4.316 | 4.354 | 4.554 | 200.812 | 1.00x |
| nested.json | orjson | 4.403 | 4.489 | 4.565 | 200.812 | 0.97x |
| nested.json | msgspec | 4.436 | 4.518 | 4.720 | 200.812 | 0.96x |
| nested.json | ujson | 5.973 | 6.017 | 6.725 | 200.812 | 0.72x |
| nested.json | json | 8.198 | 8.299 | 8.805 | 200.812 | 0.52x |
| wide_arrays.json | strata | 26.589 | 26.772 | 27.095 | 191.578 | 1.00x |
| wide_arrays.json | orjson | 19.644 | 19.781 | 20.084 | 191.578 | 1.35x |
| wide_arrays.json | msgspec | 23.870 | 24.099 | 24.424 | 191.578 | 1.11x |
| wide_arrays.json | ujson | 30.498 | 30.857 | 31.324 | 191.578 | 0.87x |
| wide_arrays.json | json | 42.086 | 42.315 | 42.642 | 191.578 | 0.63x |
| mixed.json | strata | 1.057 | 1.105 | 1.144 | 190.594 | 1.00x |
| mixed.json | orjson | 1.704 | 1.888 | 2.024 | 190.594 | 0.59x |
| mixed.json | msgspec | 1.773 | 1.876 | 2.151 | 190.594 | 0.59x |
| mixed.json | ujson | 2.013 | 2.075 | 2.250 | 190.594 | 0.53x |
| mixed.json | json | 2.567 | 2.709 | 2.941 | 190.594 | 0.41x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 76.765 | 77.178 | 77.535 | 205.594 | 1.00x |
| users.ndjson | orjson | 82.127 | 82.693 | 85.077 | 205.594 | 0.93x |
| users.ndjson | msgspec | 81.683 | 83.166 | 84.953 | 205.594 | 0.93x |
| users.ndjson | ujson | 103.701 | 105.330 | 107.254 | 205.594 | 0.73x |
| users.ndjson | json | 134.208 | 135.381 | 136.216 | 205.594 | 0.57x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 38.802 | 39.258 | 44.121 | 617.875 | 1.00x |
| users.json | orjson | 18.577 | 19.426 | 20.017 | 617.875 | 2.02x |
| users.json | msgspec | 22.506 | 23.337 | 24.656 | 617.875 | 1.68x |
| users.json | ujson | 83.454 | 84.440 | 86.518 | 617.875 | 0.46x |
| users.json | json | 117.477 | 118.104 | 119.204 | 617.875 | 0.33x |
| flat.json | strata | 2.456 | 2.525 | 2.620 | 200.812 | 1.00x |
| flat.json | orjson | 1.370 | 1.413 | 1.672 | 200.812 | 1.79x |
| flat.json | msgspec | 1.751 | 1.783 | 2.014 | 200.812 | 1.42x |
| flat.json | ujson | 4.743 | 4.828 | 4.967 | 200.812 | 0.52x |
| flat.json | json | 6.702 | 6.797 | 7.007 | 200.812 | 0.37x |
| nested.json | strata | 2.264 | 2.305 | 2.347 | 200.922 | 1.00x |
| nested.json | orjson | 1.272 | 1.319 | 1.477 | 200.922 | 1.75x |
| nested.json | msgspec | 1.596 | 1.647 | 1.688 | 200.922 | 1.40x |
| nested.json | ujson | 5.224 | 5.283 | 5.578 | 200.922 | 0.44x |
| nested.json | json | 8.289 | 8.358 | 8.768 | 200.922 | 0.28x |
| wide_arrays.json | strata | 15.030 | 15.192 | 15.722 | 190.594 | 1.00x |
| wide_arrays.json | orjson | 7.278 | 7.447 | 7.706 | 190.594 | 2.04x |
| wide_arrays.json | msgspec | 11.273 | 11.508 | 11.882 | 190.594 | 1.32x |
| wide_arrays.json | ujson | 34.326 | 34.506 | 34.961 | 190.594 | 0.44x |
| wide_arrays.json | json | 57.537 | 58.091 | 58.863 | 190.594 | 0.26x |
| mixed.json | strata | 0.686 | 0.713 | 0.807 | 190.594 | 1.00x |
| mixed.json | orjson | 0.333 | 0.347 | 0.390 | 190.594 | 2.05x |
| mixed.json | msgspec | 0.362 | 0.371 | 0.413 | 190.594 | 1.92x |
| mixed.json | ujson | 1.240 | 1.263 | 1.288 | 190.594 | 0.56x |
| mixed.json | json | 1.838 | 1.869 | 2.030 | 190.594 | 0.38x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.203 | 0.214 | 0.235 | 618.203 | 1.00x |
| users.json $[*].id | jmespath | 1.007 | 1.039 | 1.109 | 618.203 | 0.21x |
| users.json $[*].id | jsonpath-ng | 14.952 | 15.181 | 15.833 | 618.203 | 0.01x |
| users.json $[*].orders[*].total | strata | 2.718 | 2.791 | 3.184 | 498.375 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 10.491 | 10.945 | 11.525 | 498.375 | 0.25x |
| users.json $[*].orders[*].total | jsonpath-ng | 56.879 | 57.737 | 61.403 | 498.375 | 0.05x |
| users.json $..total | strata | 10.271 | 10.356 | 10.647 | 160.359 | 1.00x |
| users.json $..total | jsonpath-ng | 1631.190 | 1632.954 | 1635.044 | 160.359 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 88.948 | 91.898 | 94.947 | 528.172 | 1.00x |
| users.json $[*].id | orjson+jmespath | 80.567 | 82.079 | 84.397 | 528.172 | 1.12x |
| users.json $[*].id | orjson+jsonpath-ng | 83.338 | 84.705 | 87.514 | 528.172 | 1.08x |
| users.json $[*].orders[*].total | strata | 91.901 | 93.197 | 96.115 | 531.500 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 89.471 | 90.312 | 91.468 | 531.500 | 1.03x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 126.126 | 127.259 | 132.879 | 531.500 | 0.73x |
| users.json $..total | strata | 99.780 | 100.328 | 102.019 | 242.812 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 1693.135 | 1695.873 | 1701.711 | 242.812 | 0.06x |

