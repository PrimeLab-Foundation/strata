# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 37a96fb
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
| users.json | strata | 7.208 | 7.849 | 13.481 | 63.516 | 1.00x |
| users.json | orjson | 10.477 | 12.368 | 20.066 | 63.516 | 0.63x |
| users.json | msgspec | 10.123 | 10.922 | 12.719 | 63.516 | 0.72x |
| users.json | ujson | 13.609 | 16.525 | 26.922 | 63.516 | 0.47x |
| users.json | pysimdjson | 139.667 | 151.423 | 190.693 | 63.516 | 0.05x |
| users.json | json | 16.465 | 17.941 | 32.151 | 63.516 | 0.44x |
| flat.json | strata | 0.638 | 0.648 | 0.722 | 83.938 | 1.00x |
| flat.json | orjson | 0.795 | 0.815 | 0.863 | 83.938 | 0.79x |
| flat.json | msgspec | 0.752 | 0.764 | 0.807 | 83.938 | 0.85x |
| flat.json | ujson | 1.199 | 1.233 | 1.414 | 83.938 | 0.53x |
| flat.json | pysimdjson | 12.220 | 12.295 | 13.937 | 83.938 | 0.05x |
| flat.json | json | 1.382 | 1.434 | 1.527 | 83.938 | 0.45x |
| nested.json | strata | 0.567 | 0.579 | 1.888 | 68.828 | 1.00x |
| nested.json | orjson | 0.780 | 0.827 | 2.069 | 68.828 | 0.70x |
| nested.json | msgspec | 0.727 | 0.752 | 0.837 | 68.828 | 0.77x |
| nested.json | ujson | 1.146 | 1.230 | 3.254 | 68.828 | 0.47x |
| nested.json | pysimdjson | 10.644 | 13.469 | 19.701 | 68.828 | 0.04x |
| nested.json | json | 1.492 | 1.546 | 3.469 | 68.828 | 0.37x |
| wide_arrays.json | strata | 3.794 | 3.894 | 5.280 | 78.219 | 1.00x |
| wide_arrays.json | orjson | 3.997 | 4.061 | 8.318 | 78.219 | 0.96x |
| wide_arrays.json | msgspec | 4.534 | 4.630 | 5.177 | 78.219 | 0.84x |
| wide_arrays.json | ujson | 5.796 | 6.021 | 6.197 | 78.219 | 0.65x |
| wide_arrays.json | pysimdjson | 65.713 | 66.722 | 84.703 | 78.219 | 0.06x |
| wide_arrays.json | json | 7.192 | 7.447 | 7.727 | 78.219 | 0.52x |
| mixed.json | strata | 0.148 | 0.156 | 0.328 | 79.891 | 1.00x |
| mixed.json | orjson | 0.174 | 0.185 | 0.216 | 79.891 | 0.84x |
| mixed.json | msgspec | 0.184 | 0.195 | 0.570 | 79.891 | 0.80x |
| mixed.json | ujson | 0.237 | 0.279 | 0.881 | 79.891 | 0.56x |
| mixed.json | pysimdjson | 2.580 | 2.640 | 2.917 | 79.891 | 0.06x |
| mixed.json | json | 0.350 | 0.375 | 0.453 | 79.891 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.776 | 1.948 | 3.874 | 72.734 | 1.00x |
| users.json | orjson | 2.572 | 2.677 | 2.875 | 72.734 | 0.73x |
| users.json | msgspec | 3.112 | 3.351 | 5.755 | 72.734 | 0.58x |
| users.json | ujson | 11.647 | 12.885 | 15.627 | 72.734 | 0.15x |
| users.json | json | 16.622 | 17.601 | 19.995 | 72.734 | 0.11x |
| flat.json | strata | 0.207 | 0.240 | 0.758 | 77.250 | 1.00x |
| flat.json | orjson | 0.280 | 0.463 | 1.116 | 77.250 | 0.52x |
| flat.json | msgspec | 0.367 | 0.380 | 0.608 | 77.250 | 0.63x |
| flat.json | ujson | 0.984 | 1.015 | 2.536 | 77.250 | 0.24x |
| flat.json | json | 1.411 | 1.467 | 2.407 | 77.250 | 0.16x |
| nested.json | strata | 0.134 | 0.144 | 0.485 | 68.844 | 1.00x |
| nested.json | orjson | 0.235 | 0.264 | 0.651 | 68.844 | 0.55x |
| nested.json | msgspec | 0.323 | 0.460 | 1.160 | 68.844 | 0.31x |
| nested.json | ujson | 1.119 | 1.206 | 2.365 | 68.844 | 0.12x |
| nested.json | json | 1.670 | 1.737 | 3.923 | 68.844 | 0.08x |
| wide_arrays.json | strata | 1.188 | 1.240 | 1.524 | 78.406 | 1.00x |
| wide_arrays.json | orjson | 1.399 | 1.458 | 1.605 | 78.406 | 0.85x |
| wide_arrays.json | msgspec | 2.348 | 2.427 | 2.467 | 78.406 | 0.51x |
| wide_arrays.json | ujson | 6.829 | 6.871 | 7.295 | 78.406 | 0.18x |
| wide_arrays.json | json | 12.212 | 12.371 | 12.698 | 78.406 | 0.10x |
| mixed.json | strata | 0.045 | 0.048 | 0.051 | 79.891 | 1.00x |
| mixed.json | orjson | 0.048 | 0.050 | 0.060 | 79.891 | 0.96x |
| mixed.json | msgspec | 0.059 | 0.062 | 0.071 | 79.891 | 0.78x |
| mixed.json | ujson | 0.230 | 0.233 | 0.267 | 79.891 | 0.21x |
| mixed.json | json | 0.369 | 0.375 | 0.399 | 79.891 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.760 | 8.022 | 10.742 | 84.094 | 1.00x |
| users.json | orjson | 11.178 | 12.138 | 16.124 | 84.094 | 0.66x |
| users.json | msgspec | 10.504 | 11.375 | 21.033 | 84.094 | 0.71x |
| users.json | ujson | 15.002 | 17.445 | 24.026 | 84.094 | 0.46x |
| users.json | json | 16.413 | 18.207 | 32.085 | 84.094 | 0.44x |
| flat.json | strata | 0.733 | 0.754 | 0.979 | 74.781 | 1.00x |
| flat.json | orjson | 1.070 | 1.095 | 2.802 | 74.781 | 0.69x |
| flat.json | msgspec | 0.876 | 0.923 | 1.884 | 74.781 | 0.82x |
| flat.json | ujson | 1.253 | 1.329 | 3.522 | 74.781 | 0.57x |
| flat.json | json | 1.524 | 1.556 | 3.858 | 74.781 | 0.48x |
| nested.json | strata | 0.687 | 0.708 | 1.415 | 69.141 | 1.00x |
| nested.json | orjson | 1.049 | 1.123 | 1.313 | 69.141 | 0.63x |
| nested.json | msgspec | 0.863 | 0.944 | 2.647 | 69.141 | 0.75x |
| nested.json | ujson | 1.238 | 1.310 | 1.673 | 69.141 | 0.54x |
| nested.json | json | 1.607 | 1.705 | 2.099 | 69.141 | 0.42x |
| wide_arrays.json | strata | 3.822 | 3.868 | 4.018 | 78.422 | 1.00x |
| wide_arrays.json | orjson | 4.070 | 4.214 | 6.724 | 78.422 | 0.92x |
| wide_arrays.json | msgspec | 4.675 | 4.763 | 5.332 | 78.422 | 0.81x |
| wide_arrays.json | ujson | 6.239 | 6.416 | 7.086 | 78.422 | 0.60x |
| wide_arrays.json | json | 7.513 | 7.676 | 10.718 | 78.422 | 0.50x |
| mixed.json | strata | 0.200 | 0.241 | 0.646 | 79.891 | 1.00x |
| mixed.json | orjson | 0.457 | 0.485 | 1.099 | 79.891 | 0.50x |
| mixed.json | msgspec | 0.301 | 0.332 | 0.535 | 79.891 | 0.73x |
| mixed.json | ujson | 0.348 | 0.371 | 0.783 | 79.891 | 0.65x |
| mixed.json | json | 0.440 | 0.477 | 0.901 | 79.891 | 0.51x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.894 | 8.185 | 15.363 | 87.203 | 1.00x |
| users.ndjson | orjson | 12.526 | 13.316 | 19.149 | 87.203 | 0.61x |
| users.ndjson | msgspec | 12.239 | 13.038 | 22.465 | 87.203 | 0.63x |
| users.ndjson | ujson | 15.216 | 16.049 | 22.208 | 87.203 | 0.51x |
| users.ndjson | json | 19.467 | 21.103 | 25.367 | 87.203 | 0.39x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.073 | 2.377 | 5.945 | 81.438 | 1.00x |
| users.json | orjson | 3.038 | 3.262 | 6.695 | 81.438 | 0.73x |
| users.json | msgspec | 3.494 | 4.016 | 7.103 | 81.438 | 0.59x |
| users.json | ujson | 12.400 | 15.674 | 23.911 | 81.438 | 0.15x |
| users.json | json | 17.254 | 19.075 | 30.666 | 81.438 | 0.12x |
| flat.json | strata | 0.522 | 0.584 | 1.191 | 74.781 | 1.00x |
| flat.json | orjson | 0.609 | 0.671 | 0.727 | 74.781 | 0.87x |
| flat.json | msgspec | 0.655 | 0.713 | 0.835 | 74.781 | 0.82x |
| flat.json | ujson | 1.280 | 1.343 | 1.659 | 74.781 | 0.43x |
| flat.json | json | 1.758 | 1.893 | 2.406 | 74.781 | 0.31x |
| nested.json | strata | 0.423 | 0.463 | 0.517 | 69.141 | 1.00x |
| nested.json | orjson | 0.524 | 0.670 | 0.755 | 69.141 | 0.69x |
| nested.json | msgspec | 0.616 | 0.753 | 0.867 | 69.141 | 0.61x |
| nested.json | ujson | 1.453 | 1.588 | 1.697 | 69.141 | 0.29x |
| nested.json | json | 2.026 | 2.110 | 2.619 | 69.141 | 0.22x |
| wide_arrays.json | strata | 1.767 | 1.891 | 3.690 | 79.875 | 1.00x |
| wide_arrays.json | orjson | 2.029 | 2.170 | 3.695 | 79.875 | 0.87x |
| wide_arrays.json | msgspec | 2.958 | 3.055 | 6.365 | 79.875 | 0.62x |
| wide_arrays.json | ujson | 7.729 | 7.870 | 9.171 | 79.875 | 0.24x |
| wide_arrays.json | json | 12.889 | 13.256 | 19.808 | 79.875 | 0.14x |
| mixed.json | strata | 0.250 | 0.333 | 0.807 | 79.891 | 1.00x |
| mixed.json | orjson | 0.259 | 0.369 | 0.448 | 79.891 | 0.90x |
| mixed.json | msgspec | 0.297 | 0.371 | 1.213 | 79.891 | 0.90x |
| mixed.json | ujson | 0.482 | 0.561 | 1.293 | 79.891 | 0.59x |
| mixed.json | json | 0.633 | 0.731 | 1.188 | 79.891 | 0.46x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.093 | 0.121 | 0.230 | 81.500 | 1.00x |
| users.json $[*].id | jmespath | 0.360 | 0.417 | 1.009 | 81.500 | 0.29x |
| users.json $[*].id | jsonpath-ng | 1.716 | 1.826 | 3.073 | 81.500 | 0.07x |
| users.json $[*].orders[*].total | strata | 0.529 | 0.775 | 1.646 | 81.734 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.983 | 2.444 | 4.611 | 81.734 | 0.32x |
| users.json $[*].orders[*].total | jsonpath-ng | 12.620 | 13.511 | 16.491 | 81.734 | 0.06x |
| users.json $..total | strata | 1.410 | 1.583 | 4.230 | 83.609 | 1.00x |
| users.json $..total | jsonpath-ng | 214.660 | 225.909 | 243.193 | 83.609 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.838 | 3.973 | 4.925 | 81.578 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.977 | 12.588 | 21.750 | 81.578 | 0.32x |
| users.json $[*].id | orjson+jsonpath-ng | 12.313 | 13.207 | 21.840 | 81.578 | 0.30x |
| users.json $[*].orders[*].total | strata | 3.919 | 4.252 | 7.606 | 83.578 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 13.266 | 14.017 | 26.614 | 83.578 | 0.30x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 25.643 | 27.744 | 43.290 | 83.578 | 0.15x |
| users.json $..total | strata | 9.699 | 10.980 | 13.418 | 83.641 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 226.943 | 267.926 | 307.803 | 83.641 | 0.04x |

