# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 04160cb
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
| users.json | strata | 8.150 | 9.125 | 14.803 | 63.469 | 1.00x |
| users.json | orjson | 10.334 | 12.735 | 39.190 | 63.469 | 0.72x |
| users.json | msgspec | 9.973 | 11.998 | 38.913 | 63.469 | 0.76x |
| users.json | ujson | 13.478 | 16.418 | 22.510 | 63.469 | 0.56x |
| users.json | pysimdjson | 135.565 | 149.390 | 186.968 | 63.469 | 0.06x |
| users.json | json | 16.370 | 18.963 | 28.022 | 63.469 | 0.48x |
| flat.json | strata | 0.670 | 0.755 | 0.966 | 93.703 | 1.00x |
| flat.json | orjson | 0.754 | 0.854 | 1.040 | 93.703 | 0.88x |
| flat.json | msgspec | 0.715 | 0.772 | 1.654 | 93.703 | 0.98x |
| flat.json | ujson | 1.070 | 1.325 | 2.566 | 93.703 | 0.57x |
| flat.json | pysimdjson | 12.205 | 12.764 | 15.720 | 93.703 | 0.06x |
| flat.json | json | 1.353 | 1.432 | 2.043 | 93.703 | 0.53x |
| nested.json | strata | 0.675 | 0.752 | 1.618 | 93.719 | 1.00x |
| nested.json | orjson | 0.836 | 0.891 | 2.302 | 93.719 | 0.84x |
| nested.json | msgspec | 0.752 | 0.879 | 2.348 | 93.719 | 0.86x |
| nested.json | ujson | 1.180 | 1.347 | 1.589 | 93.719 | 0.56x |
| nested.json | pysimdjson | 11.634 | 16.358 | 31.051 | 93.719 | 0.05x |
| nested.json | json | 1.586 | 2.028 | 4.468 | 93.719 | 0.37x |
| wide_arrays.json | strata | 3.262 | 3.941 | 5.449 | 96.547 | 1.00x |
| wide_arrays.json | orjson | 3.417 | 4.070 | 6.201 | 96.547 | 0.97x |
| wide_arrays.json | msgspec | 4.074 | 4.638 | 5.386 | 96.547 | 0.85x |
| wide_arrays.json | ujson | 5.155 | 6.065 | 6.953 | 96.547 | 0.65x |
| wide_arrays.json | pysimdjson | 61.256 | 67.641 | 92.583 | 96.547 | 0.06x |
| wide_arrays.json | json | 6.402 | 7.817 | 8.974 | 96.547 | 0.50x |
| mixed.json | strata | 0.130 | 0.133 | 0.135 | 96.578 | 1.00x |
| mixed.json | orjson | 0.145 | 0.150 | 0.156 | 96.578 | 0.88x |
| mixed.json | msgspec | 0.157 | 0.160 | 0.162 | 96.578 | 0.83x |
| mixed.json | ujson | 0.197 | 0.241 | 0.411 | 96.578 | 0.55x |
| mixed.json | pysimdjson | 2.381 | 2.396 | 2.434 | 96.578 | 0.06x |
| mixed.json | json | 0.300 | 0.303 | 0.334 | 96.578 | 0.44x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.779 | 2.158 | 4.490 | 80.406 | 1.00x |
| users.json | orjson | 2.203 | 2.677 | 3.004 | 80.406 | 0.81x |
| users.json | msgspec | 2.761 | 3.272 | 4.558 | 80.406 | 0.66x |
| users.json | ujson | 10.557 | 12.655 | 17.585 | 80.406 | 0.17x |
| users.json | json | 15.230 | 17.789 | 20.814 | 80.406 | 0.12x |
| flat.json | strata | 0.204 | 0.221 | 0.253 | 93.719 | 1.00x |
| flat.json | orjson | 0.271 | 0.378 | 0.445 | 93.719 | 0.58x |
| flat.json | msgspec | 0.338 | 0.373 | 0.561 | 93.719 | 0.59x |
| flat.json | ujson | 0.949 | 1.053 | 1.178 | 93.719 | 0.21x |
| flat.json | json | 1.396 | 1.557 | 1.924 | 93.719 | 0.14x |
| nested.json | strata | 0.155 | 0.210 | 0.668 | 93.734 | 1.00x |
| nested.json | orjson | 0.258 | 0.321 | 0.709 | 93.734 | 0.65x |
| nested.json | msgspec | 0.377 | 0.503 | 0.976 | 93.734 | 0.42x |
| nested.json | ujson | 1.186 | 1.477 | 3.132 | 93.734 | 0.14x |
| nested.json | json | 2.042 | 2.438 | 4.799 | 93.734 | 0.09x |
| wide_arrays.json | strata | 1.130 | 1.164 | 1.377 | 96.562 | 1.00x |
| wide_arrays.json | orjson | 1.245 | 1.361 | 1.614 | 96.562 | 0.86x |
| wide_arrays.json | msgspec | 2.019 | 2.109 | 2.435 | 96.562 | 0.55x |
| wide_arrays.json | ujson | 6.178 | 6.314 | 7.117 | 96.562 | 0.18x |
| wide_arrays.json | json | 11.217 | 11.367 | 11.829 | 96.562 | 0.10x |
| mixed.json | strata | 0.034 | 0.035 | 0.036 | 96.578 | 1.00x |
| mixed.json | orjson | 0.040 | 0.041 | 0.043 | 96.578 | 0.84x |
| mixed.json | msgspec | 0.047 | 0.083 | 0.145 | 96.578 | 0.42x |
| mixed.json | ujson | 0.206 | 0.207 | 0.219 | 96.578 | 0.17x |
| mixed.json | json | 0.326 | 0.329 | 0.332 | 96.578 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.369 | 7.533 | 8.834 | 91.688 | 1.00x |
| users.json | orjson | 9.079 | 9.252 | 12.334 | 91.688 | 0.81x |
| users.json | msgspec | 8.691 | 8.891 | 11.527 | 91.688 | 0.85x |
| users.json | ujson | 11.733 | 13.054 | 16.568 | 91.688 | 0.58x |
| users.json | json | 14.280 | 14.730 | 18.818 | 91.688 | 0.51x |
| flat.json | strata | 0.762 | 0.891 | 1.468 | 93.719 | 1.00x |
| flat.json | orjson | 0.933 | 1.156 | 1.355 | 93.719 | 0.77x |
| flat.json | msgspec | 0.842 | 1.060 | 1.881 | 93.719 | 0.84x |
| flat.json | ujson | 1.214 | 1.508 | 2.937 | 93.719 | 0.59x |
| flat.json | json | 1.557 | 1.740 | 2.917 | 93.719 | 0.51x |
| nested.json | strata | 0.709 | 0.867 | 2.039 | 93.734 | 1.00x |
| nested.json | orjson | 1.111 | 1.241 | 1.488 | 93.734 | 0.70x |
| nested.json | msgspec | 0.892 | 0.990 | 1.789 | 93.734 | 0.88x |
| nested.json | ujson | 1.284 | 1.618 | 3.742 | 93.734 | 0.54x |
| nested.json | json | 1.746 | 1.965 | 3.799 | 93.734 | 0.44x |
| wide_arrays.json | strata | 3.453 | 3.559 | 3.870 | 96.562 | 1.00x |
| wide_arrays.json | orjson | 3.598 | 3.694 | 4.196 | 96.562 | 0.96x |
| wide_arrays.json | msgspec | 4.188 | 4.252 | 4.801 | 96.562 | 0.84x |
| wide_arrays.json | ujson | 5.578 | 5.739 | 6.446 | 96.562 | 0.62x |
| wide_arrays.json | json | 6.819 | 6.927 | 7.668 | 96.562 | 0.51x |
| mixed.json | strata | 0.149 | 0.152 | 0.173 | 96.578 | 1.00x |
| mixed.json | orjson | 0.230 | 0.236 | 0.315 | 96.578 | 0.64x |
| mixed.json | msgspec | 0.188 | 0.190 | 0.204 | 96.578 | 0.80x |
| mixed.json | ujson | 0.228 | 0.234 | 0.243 | 96.578 | 0.65x |
| mixed.json | json | 0.323 | 0.327 | 0.338 | 96.578 | 0.46x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.127 | 9.237 | 10.836 | 93.703 | 1.00x |
| users.ndjson | orjson | 13.235 | 14.084 | 24.137 | 93.703 | 0.66x |
| users.ndjson | msgspec | 12.984 | 13.566 | 34.086 | 93.703 | 0.68x |
| users.ndjson | ujson | 15.439 | 16.806 | 20.223 | 93.703 | 0.55x |
| users.ndjson | json | 19.556 | 22.154 | 24.133 | 93.703 | 0.42x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.013 | 2.142 | 2.238 | 88.891 | 1.00x |
| users.json | orjson | 2.586 | 2.695 | 3.262 | 88.891 | 0.79x |
| users.json | msgspec | 3.114 | 3.199 | 3.622 | 88.891 | 0.67x |
| users.json | ujson | 11.099 | 11.384 | 12.210 | 88.891 | 0.19x |
| users.json | json | 15.658 | 16.179 | 17.109 | 88.891 | 0.13x |
| flat.json | strata | 0.479 | 0.583 | 0.887 | 93.719 | 1.00x |
| flat.json | orjson | 0.500 | 0.664 | 0.944 | 93.719 | 0.88x |
| flat.json | msgspec | 0.659 | 0.720 | 0.881 | 93.719 | 0.81x |
| flat.json | ujson | 1.360 | 1.443 | 2.213 | 93.719 | 0.40x |
| flat.json | json | 1.975 | 2.106 | 2.362 | 93.719 | 0.28x |
| nested.json | strata | 0.350 | 0.398 | 0.885 | 93.734 | 1.00x |
| nested.json | orjson | 0.444 | 0.489 | 0.591 | 93.734 | 0.81x |
| nested.json | msgspec | 0.528 | 0.677 | 0.926 | 93.734 | 0.59x |
| nested.json | ujson | 1.318 | 1.474 | 9.485 | 93.734 | 0.27x |
| nested.json | json | 2.002 | 2.370 | 4.643 | 93.734 | 0.17x |
| wide_arrays.json | strata | 1.549 | 1.685 | 2.103 | 96.562 | 1.00x |
| wide_arrays.json | orjson | 1.789 | 1.816 | 2.257 | 96.562 | 0.93x |
| wide_arrays.json | msgspec | 2.334 | 2.616 | 3.291 | 96.562 | 0.64x |
| wide_arrays.json | ujson | 6.625 | 7.384 | 9.801 | 96.562 | 0.23x |
| wide_arrays.json | json | 11.864 | 12.568 | 14.029 | 96.562 | 0.13x |
| mixed.json | strata | 0.109 | 0.127 | 0.392 | 96.578 | 1.00x |
| mixed.json | orjson | 0.118 | 0.129 | 0.235 | 96.578 | 0.98x |
| mixed.json | msgspec | 0.127 | 0.164 | 0.358 | 96.578 | 0.77x |
| mixed.json | ujson | 0.291 | 0.318 | 0.425 | 96.578 | 0.40x |
| mixed.json | json | 0.409 | 0.421 | 0.578 | 96.578 | 0.30x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.043 | 0.057 | 0.108 | 88.938 | 1.00x |
| users.json $[*].id | jmespath | 0.257 | 0.306 | 0.422 | 88.938 | 0.19x |
| users.json $[*].id | jsonpath-ng | 1.452 | 1.524 | 1.837 | 88.938 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.480 | 0.691 | 1.893 | 89.234 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.143 | 2.386 | 16.328 | 89.234 | 0.29x |
| users.json $[*].orders[*].total | jsonpath-ng | 13.127 | 15.014 | 31.367 | 89.234 | 0.05x |
| users.json $..total | strata | 1.466 | 1.571 | 1.848 | 90.094 | 1.00x |
| users.json $..total | jsonpath-ng | 200.292 | 212.632 | 228.359 | 90.094 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.902 | 4.104 | 6.825 | 89.031 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.822 | 12.308 | 14.909 | 89.031 | 0.33x |
| users.json $[*].id | orjson+jsonpath-ng | 13.516 | 14.665 | 19.763 | 89.031 | 0.28x |
| users.json $[*].orders[*].total | strata | 3.755 | 4.242 | 8.227 | 90.031 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.126 | 13.882 | 14.526 | 90.031 | 0.31x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 23.966 | 28.655 | 29.739 | 90.031 | 0.15x |
| users.json $..total | strata | 8.685 | 9.000 | 12.889 | 90.109 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 191.896 | 216.875 | 269.160 | 90.109 | 0.04x |

