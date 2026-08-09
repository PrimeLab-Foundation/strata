# Benchmark results - medium

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 67e8915
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
| users.json | strata | 60.392 | 63.216 | 65.936 | 137.500 | 1.00x |
| users.json | orjson | 50.141 | 53.381 | 55.207 | 137.500 | 1.18x |
| users.json | msgspec | 50.919 | 54.358 | 56.706 | 137.500 | 1.16x |
| users.json | ujson | 70.392 | 75.280 | 79.038 | 137.500 | 0.84x |
| users.json | json | 90.604 | 92.388 | 95.998 | 137.500 | 0.68x |
| flat.json | strata | 4.169 | 4.217 | 4.411 | 403.109 | 1.00x |
| flat.json | orjson | 3.408 | 3.494 | 3.738 | 403.109 | 1.21x |
| flat.json | msgspec | 3.395 | 3.496 | 3.717 | 403.109 | 1.21x |
| flat.json | ujson | 4.787 | 4.861 | 5.198 | 403.109 | 0.87x |
| flat.json | json | 6.294 | 6.359 | 6.723 | 403.109 | 0.66x |
| nested.json | strata | 3.341 | 3.386 | 3.534 | 405.156 | 1.00x |
| nested.json | orjson | 2.870 | 2.921 | 3.212 | 405.156 | 1.16x |
| nested.json | msgspec | 2.836 | 2.921 | 3.155 | 405.156 | 1.16x |
| nested.json | ujson | 3.944 | 4.037 | 4.337 | 405.156 | 0.84x |
| nested.json | json | 5.696 | 5.744 | 5.955 | 405.156 | 0.59x |
| wide_arrays.json | strata | 19.751 | 20.065 | 20.798 | 425.766 | 1.00x |
| wide_arrays.json | orjson | 12.864 | 13.086 | 13.514 | 425.766 | 1.53x |
| wide_arrays.json | msgspec | 16.086 | 16.188 | 17.084 | 425.766 | 1.24x |
| wide_arrays.json | ujson | 20.653 | 20.790 | 21.086 | 425.766 | 0.97x |
| wide_arrays.json | json | 30.059 | 30.816 | 31.109 | 425.766 | 0.65x |
| mixed.json | strata | 0.786 | 0.792 | 0.832 | 422.969 | 1.00x |
| mixed.json | orjson | 1.034 | 1.052 | 1.113 | 422.969 | 0.75x |
| mixed.json | msgspec | 1.084 | 1.107 | 1.182 | 422.969 | 0.72x |
| mixed.json | ujson | 1.239 | 1.254 | 1.309 | 422.969 | 0.63x |
| mixed.json | json | 1.693 | 1.720 | 2.031 | 422.969 | 0.46x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 35.503 | 35.671 | 36.226 | 444.844 | 1.00x |
| users.json | orjson | 12.971 | 13.260 | 13.588 | 444.844 | 2.69x |
| users.json | msgspec | 15.925 | 16.104 | 16.492 | 444.844 | 2.22x |
| users.json | ujson | 63.733 | 64.291 | 64.837 | 444.844 | 0.55x |
| users.json | json | 87.653 | 88.103 | 94.953 | 444.844 | 0.40x |
| flat.json | strata | 2.374 | 2.417 | 2.524 | 403.203 | 1.00x |
| flat.json | orjson | 0.840 | 0.851 | 0.928 | 403.203 | 2.84x |
| flat.json | msgspec | 1.111 | 1.165 | 1.206 | 403.203 | 2.07x |
| flat.json | ujson | 3.410 | 3.432 | 3.756 | 403.203 | 0.70x |
| flat.json | json | 4.910 | 4.955 | 5.115 | 403.203 | 0.49x |
| nested.json | strata | 2.152 | 2.180 | 2.340 | 405.219 | 1.00x |
| nested.json | orjson | 0.779 | 0.786 | 0.811 | 405.219 | 2.77x |
| nested.json | msgspec | 1.012 | 1.019 | 1.072 | 405.219 | 2.14x |
| nested.json | ujson | 4.119 | 4.144 | 4.334 | 405.219 | 0.53x |
| nested.json | json | 5.895 | 5.937 | 6.025 | 405.219 | 0.37x |
| wide_arrays.json | strata | 17.668 | 17.915 | 18.137 | 423.969 | 1.00x |
| wide_arrays.json | orjson | 4.522 | 4.590 | 4.639 | 423.969 | 3.90x |
| wide_arrays.json | msgspec | 7.958 | 8.052 | 8.326 | 423.969 | 2.22x |
| wide_arrays.json | ujson | 24.480 | 24.725 | 25.040 | 423.969 | 0.72x |
| wide_arrays.json | json | 42.991 | 43.360 | 44.150 | 423.969 | 0.41x |
| mixed.json | strata | 0.393 | 0.398 | 0.411 | 423.000 | 1.00x |
| mixed.json | orjson | 0.151 | 0.153 | 0.165 | 423.000 | 2.61x |
| mixed.json | msgspec | 0.165 | 0.168 | 0.195 | 423.000 | 2.37x |
| mixed.json | ujson | 0.811 | 0.816 | 0.839 | 423.000 | 0.49x |
| mixed.json | json | 1.220 | 1.240 | 1.299 | 423.000 | 0.32x |

