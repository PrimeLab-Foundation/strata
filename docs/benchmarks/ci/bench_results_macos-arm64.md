# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 79fa3df
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
| users.json | strata | 5.802 | 5.989 | 6.934 | 63.625 | 1.00x |
| users.json | orjson | 8.703 | 9.413 | 10.708 | 63.625 | 0.64x |
| users.json | msgspec | 8.312 | 8.763 | 10.439 | 63.625 | 0.68x |
| users.json | ujson | 10.994 | 12.162 | 14.525 | 63.625 | 0.49x |
| users.json | pysimdjson | 115.395 | 118.982 | 133.780 | 63.625 | 0.05x |
| users.json | json | 13.676 | 14.685 | 17.529 | 63.625 | 0.41x |
| flat.json | strata | 0.546 | 0.551 | 0.559 | 88.781 | 1.00x |
| flat.json | orjson | 0.666 | 0.681 | 0.695 | 88.781 | 0.81x |
| flat.json | msgspec | 0.644 | 0.646 | 0.656 | 88.781 | 0.85x |
| flat.json | ujson | 1.004 | 1.029 | 1.146 | 88.781 | 0.54x |
| flat.json | pysimdjson | 11.075 | 11.099 | 11.207 | 88.781 | 0.05x |
| flat.json | json | 1.245 | 1.256 | 1.264 | 88.781 | 0.44x |
| nested.json | strata | 0.477 | 0.479 | 0.503 | 88.781 | 1.00x |
| nested.json | orjson | 0.640 | 0.655 | 0.672 | 88.781 | 0.73x |
| nested.json | msgspec | 0.633 | 0.636 | 0.642 | 88.781 | 0.75x |
| nested.json | ujson | 0.978 | 1.007 | 1.445 | 88.781 | 0.48x |
| nested.json | pysimdjson | 9.685 | 9.732 | 9.826 | 88.781 | 0.05x |
| nested.json | json | 1.320 | 1.327 | 1.349 | 88.781 | 0.36x |
| wide_arrays.json | strata | 2.776 | 2.805 | 3.300 | 92.719 | 1.00x |
| wide_arrays.json | orjson | 3.302 | 3.343 | 3.831 | 92.719 | 0.84x |
| wide_arrays.json | msgspec | 3.791 | 3.829 | 4.351 | 92.719 | 0.73x |
| wide_arrays.json | ujson | 4.846 | 5.030 | 5.690 | 92.719 | 0.56x |
| wide_arrays.json | pysimdjson | 59.635 | 59.926 | 65.831 | 92.719 | 0.05x |
| wide_arrays.json | json | 6.364 | 6.424 | 7.280 | 92.719 | 0.44x |
| mixed.json | strata | 0.117 | 0.130 | 0.146 | 93.141 | 1.00x |
| mixed.json | orjson | 0.151 | 0.170 | 0.211 | 93.141 | 0.77x |
| mixed.json | msgspec | 0.168 | 0.188 | 0.204 | 93.141 | 0.69x |
| mixed.json | ujson | 0.209 | 0.275 | 0.582 | 93.141 | 0.47x |
| mixed.json | pysimdjson | 2.552 | 2.721 | 2.904 | 93.141 | 0.05x |
| mixed.json | json | 0.319 | 0.350 | 0.727 | 93.141 | 0.37x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.399 | 1.414 | 1.424 | 70.328 | 1.00x |
| users.json | orjson | 2.126 | 2.142 | 2.203 | 70.328 | 0.66x |
| users.json | msgspec | 2.829 | 2.853 | 2.902 | 70.328 | 0.50x |
| users.json | ujson | 8.280 | 8.306 | 8.444 | 70.328 | 0.17x |
| users.json | json | 14.823 | 14.890 | 15.002 | 70.328 | 0.09x |
| flat.json | strata | 0.190 | 0.199 | 0.212 | 88.781 | 1.00x |
| flat.json | orjson | 0.231 | 0.234 | 0.325 | 88.781 | 0.85x |
| flat.json | msgspec | 0.288 | 0.291 | 0.310 | 88.781 | 0.68x |
| flat.json | ujson | 0.723 | 0.732 | 0.816 | 88.781 | 0.27x |
| flat.json | json | 1.308 | 1.321 | 1.410 | 88.781 | 0.15x |
| nested.json | strata | 0.123 | 0.124 | 0.125 | 88.781 | 1.00x |
| nested.json | orjson | 0.204 | 0.206 | 0.228 | 88.781 | 0.60x |
| nested.json | msgspec | 0.267 | 0.269 | 0.279 | 88.781 | 0.46x |
| nested.json | ujson | 0.836 | 0.846 | 0.867 | 88.781 | 0.15x |
| nested.json | json | 1.519 | 1.527 | 1.608 | 88.781 | 0.08x |
| wide_arrays.json | strata | 0.940 | 1.064 | 1.220 | 92.734 | 1.00x |
| wide_arrays.json | orjson | 1.263 | 1.369 | 1.517 | 92.734 | 0.78x |
| wide_arrays.json | msgspec | 2.124 | 2.166 | 2.292 | 92.734 | 0.49x |
| wide_arrays.json | ujson | 4.468 | 4.912 | 5.060 | 92.734 | 0.22x |
| wide_arrays.json | json | 11.052 | 11.980 | 12.641 | 92.734 | 0.09x |
| mixed.json | strata | 0.035 | 0.038 | 0.046 | 93.141 | 1.00x |
| mixed.json | orjson | 0.044 | 0.046 | 0.054 | 93.141 | 0.81x |
| mixed.json | msgspec | 0.052 | 0.061 | 0.089 | 93.141 | 0.62x |
| mixed.json | ujson | 0.168 | 0.184 | 0.199 | 93.141 | 0.21x |
| mixed.json | json | 0.346 | 0.360 | 0.419 | 93.141 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.149 | 6.188 | 6.341 | 83.688 | 1.00x |
| users.json | orjson | 8.952 | 9.038 | 9.115 | 83.688 | 0.68x |
| users.json | msgspec | 8.660 | 8.712 | 8.804 | 83.688 | 0.71x |
| users.json | ujson | 11.570 | 11.824 | 12.127 | 83.688 | 0.52x |
| users.json | json | 14.114 | 14.197 | 15.233 | 83.688 | 0.44x |
| flat.json | strata | 0.577 | 0.583 | 0.594 | 88.781 | 1.00x |
| flat.json | orjson | 0.731 | 0.748 | 0.783 | 88.781 | 0.78x |
| flat.json | msgspec | 0.687 | 0.694 | 0.729 | 88.781 | 0.84x |
| flat.json | ujson | 1.026 | 1.037 | 1.104 | 88.781 | 0.56x |
| flat.json | json | 1.283 | 1.292 | 1.477 | 88.781 | 0.45x |
| nested.json | strata | 0.508 | 0.514 | 0.540 | 88.781 | 1.00x |
| nested.json | orjson | 0.711 | 0.716 | 0.795 | 88.781 | 0.72x |
| nested.json | msgspec | 0.678 | 0.686 | 0.699 | 88.781 | 0.75x |
| nested.json | ujson | 0.940 | 0.947 | 0.971 | 88.781 | 0.54x |
| nested.json | json | 1.358 | 1.366 | 1.508 | 88.781 | 0.38x |
| wide_arrays.json | strata | 2.949 | 2.977 | 3.031 | 92.734 | 1.00x |
| wide_arrays.json | orjson | 3.524 | 3.536 | 3.744 | 92.734 | 0.84x |
| wide_arrays.json | msgspec | 4.115 | 4.143 | 4.185 | 92.734 | 0.72x |
| wide_arrays.json | ujson | 5.389 | 5.409 | 5.442 | 92.734 | 0.55x |
| wide_arrays.json | json | 6.692 | 6.717 | 6.762 | 92.734 | 0.44x |
| mixed.json | strata | 0.149 | 0.169 | 0.184 | 93.141 | 1.00x |
| mixed.json | orjson | 0.199 | 0.242 | 0.492 | 93.141 | 0.70x |
| mixed.json | msgspec | 0.210 | 0.224 | 0.291 | 93.141 | 0.75x |
| mixed.json | ujson | 0.253 | 0.277 | 0.312 | 93.141 | 0.61x |
| mixed.json | json | 0.346 | 0.375 | 0.416 | 93.141 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.252 | 6.283 | 6.484 | 88.766 | 1.00x |
| users.ndjson | orjson | 10.628 | 10.793 | 10.863 | 88.766 | 0.58x |
| users.ndjson | msgspec | 10.611 | 10.643 | 10.846 | 88.766 | 0.59x |
| users.ndjson | ujson | 13.136 | 13.193 | 13.435 | 88.766 | 0.48x |
| users.ndjson | json | 17.173 | 17.262 | 17.599 | 88.766 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.663 | 1.804 | 1.996 | 86.828 | 1.00x |
| users.json | orjson | 2.494 | 2.564 | 2.973 | 86.828 | 0.70x |
| users.json | msgspec | 3.145 | 3.445 | 3.826 | 86.828 | 0.52x |
| users.json | ujson | 8.940 | 9.068 | 10.076 | 86.828 | 0.20x |
| users.json | json | 15.218 | 15.871 | 17.073 | 86.828 | 0.11x |
| flat.json | strata | 0.294 | 0.326 | 0.630 | 88.781 | 1.00x |
| flat.json | orjson | 0.338 | 0.354 | 0.773 | 88.781 | 0.92x |
| flat.json | msgspec | 0.395 | 0.422 | 0.751 | 88.781 | 0.77x |
| flat.json | ujson | 0.824 | 0.842 | 0.916 | 88.781 | 0.39x |
| flat.json | json | 1.420 | 1.503 | 1.732 | 88.781 | 0.22x |
| nested.json | strata | 0.217 | 0.228 | 0.512 | 88.781 | 1.00x |
| nested.json | orjson | 0.301 | 0.311 | 0.803 | 88.781 | 0.73x |
| nested.json | msgspec | 0.368 | 0.489 | 0.597 | 88.781 | 0.47x |
| nested.json | ujson | 0.868 | 0.919 | 1.066 | 88.781 | 0.25x |
| nested.json | json | 1.626 | 1.661 | 1.763 | 88.781 | 0.14x |
| wide_arrays.json | strata | 1.243 | 1.333 | 2.294 | 93.125 | 1.00x |
| wide_arrays.json | orjson | 1.489 | 1.593 | 1.951 | 93.125 | 0.84x |
| wide_arrays.json | msgspec | 2.223 | 2.450 | 3.051 | 93.125 | 0.54x |
| wide_arrays.json | ujson | 4.789 | 4.906 | 5.821 | 93.125 | 0.27x |
| wide_arrays.json | json | 11.309 | 11.466 | 13.032 | 93.125 | 0.12x |
| mixed.json | strata | 0.149 | 0.199 | 0.574 | 93.141 | 1.00x |
| mixed.json | orjson | 0.149 | 0.209 | 0.559 | 93.141 | 0.95x |
| mixed.json | msgspec | 0.167 | 0.211 | 0.254 | 93.141 | 0.95x |
| mixed.json | ujson | 0.308 | 0.374 | 0.896 | 93.141 | 0.53x |
| mixed.json | json | 0.505 | 0.572 | 0.822 | 93.141 | 0.35x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.062 | 0.066 | 0.081 | 86.906 | 1.00x |
| users.json $[*].id | jmespath | 0.289 | 0.313 | 0.350 | 86.906 | 0.21x |
| users.json $[*].id | jsonpath-ng | 1.518 | 1.577 | 1.650 | 86.906 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.264 | 0.271 | 0.603 | 87.062 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.560 | 1.658 | 2.220 | 87.062 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 9.531 | 10.114 | 12.589 | 87.062 | 0.03x |
| users.json $..total | strata | 1.196 | 1.212 | 1.380 | 87.875 | 1.00x |
| users.json $..total | jsonpath-ng | 178.279 | 179.678 | 181.497 | 87.875 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.374 | 3.398 | 3.647 | 86.969 | 1.00x |
| users.json $[*].id | orjson+jmespath | 9.160 | 9.246 | 10.464 | 86.969 | 0.37x |
| users.json $[*].id | orjson+jsonpath-ng | 10.337 | 10.404 | 11.791 | 86.969 | 0.33x |
| users.json $[*].orders[*].total | strata | 3.441 | 3.602 | 3.873 | 87.875 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 10.490 | 10.641 | 11.575 | 87.875 | 0.34x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 20.398 | 20.548 | 20.822 | 87.875 | 0.18x |
| users.json $..total | strata | 7.428 | 7.607 | 8.497 | 87.938 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 187.697 | 189.807 | 210.601 | 87.938 | 0.04x |

