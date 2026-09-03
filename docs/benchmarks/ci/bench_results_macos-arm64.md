# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 2f12155
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
| users.json | strata | 7.501 | 10.800 | 15.009 | 63.500 | 1.00x |
| users.json | orjson | 10.467 | 14.498 | 18.316 | 63.500 | 0.74x |
| users.json | msgspec | 10.478 | 12.752 | 23.831 | 63.500 | 0.85x |
| users.json | ujson | 13.374 | 15.572 | 25.782 | 63.500 | 0.69x |
| users.json | pysimdjson | 131.761 | 176.109 | 191.963 | 63.500 | 0.06x |
| users.json | json | 16.258 | 23.151 | 36.864 | 63.500 | 0.47x |
| flat.json | strata | 0.612 | 0.738 | 1.828 | 92.078 | 1.00x |
| flat.json | orjson | 0.796 | 1.210 | 2.419 | 92.078 | 0.61x |
| flat.json | msgspec | 0.770 | 0.818 | 1.261 | 92.078 | 0.90x |
| flat.json | ujson | 1.296 | 1.487 | 3.233 | 92.078 | 0.50x |
| flat.json | pysimdjson | 13.017 | 15.255 | 20.384 | 92.078 | 0.05x |
| flat.json | json | 1.461 | 2.779 | 5.491 | 92.078 | 0.27x |
| nested.json | strata | 0.638 | 0.658 | 0.798 | 92.094 | 1.00x |
| nested.json | orjson | 0.804 | 0.884 | 1.728 | 92.094 | 0.74x |
| nested.json | msgspec | 0.794 | 0.835 | 1.375 | 92.094 | 0.79x |
| nested.json | ujson | 1.203 | 1.316 | 3.357 | 92.094 | 0.50x |
| nested.json | pysimdjson | 11.487 | 12.619 | 14.169 | 92.094 | 0.05x |
| nested.json | json | 1.530 | 1.802 | 4.529 | 92.094 | 0.37x |
| wide_arrays.json | strata | 4.161 | 4.818 | 11.162 | 93.625 | 1.00x |
| wide_arrays.json | orjson | 4.169 | 4.773 | 5.535 | 93.625 | 1.01x |
| wide_arrays.json | msgspec | 4.457 | 5.504 | 7.663 | 93.625 | 0.88x |
| wide_arrays.json | ujson | 6.122 | 7.298 | 10.003 | 93.625 | 0.66x |
| wide_arrays.json | pysimdjson | 68.806 | 80.509 | 90.678 | 93.625 | 0.06x |
| wide_arrays.json | json | 7.580 | 9.451 | 11.967 | 93.625 | 0.51x |
| mixed.json | strata | 0.148 | 0.164 | 0.177 | 94.031 | 1.00x |
| mixed.json | orjson | 0.181 | 0.195 | 3.504 | 94.031 | 0.84x |
| mixed.json | msgspec | 0.187 | 0.214 | 0.462 | 94.031 | 0.77x |
| mixed.json | ujson | 0.250 | 0.369 | 0.770 | 94.031 | 0.44x |
| mixed.json | pysimdjson | 2.866 | 3.158 | 6.012 | 94.031 | 0.05x |
| mixed.json | json | 0.374 | 0.431 | 0.583 | 94.031 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.879 | 1.965 | 2.132 | 77.359 | 1.00x |
| users.json | orjson | 2.494 | 2.602 | 2.851 | 77.359 | 0.76x |
| users.json | msgspec | 3.132 | 3.329 | 3.946 | 77.359 | 0.59x |
| users.json | ujson | 11.793 | 12.221 | 14.037 | 77.359 | 0.16x |
| users.json | json | 16.638 | 17.155 | 21.295 | 77.359 | 0.11x |
| flat.json | strata | 0.258 | 0.464 | 0.742 | 92.094 | 1.00x |
| flat.json | orjson | 0.307 | 0.349 | 0.789 | 92.094 | 1.33x |
| flat.json | msgspec | 0.352 | 0.394 | 1.159 | 92.094 | 1.18x |
| flat.json | ujson | 1.001 | 1.078 | 2.982 | 92.094 | 0.43x |
| flat.json | json | 1.520 | 2.220 | 3.825 | 92.094 | 0.21x |
| nested.json | strata | 0.139 | 0.180 | 0.312 | 92.094 | 1.00x |
| nested.json | orjson | 0.231 | 0.285 | 0.407 | 92.094 | 0.63x |
| nested.json | msgspec | 0.281 | 0.387 | 0.757 | 92.094 | 0.46x |
| nested.json | ujson | 1.137 | 1.426 | 1.524 | 92.094 | 0.13x |
| nested.json | json | 1.841 | 2.001 | 2.829 | 92.094 | 0.09x |
| wide_arrays.json | strata | 1.281 | 1.451 | 1.959 | 93.625 | 1.00x |
| wide_arrays.json | orjson | 1.519 | 1.633 | 1.907 | 93.625 | 0.89x |
| wide_arrays.json | msgspec | 2.391 | 2.680 | 3.657 | 93.625 | 0.54x |
| wide_arrays.json | ujson | 7.455 | 7.767 | 11.600 | 93.625 | 0.19x |
| wide_arrays.json | json | 13.887 | 14.906 | 17.154 | 93.625 | 0.10x |
| mixed.json | strata | 0.053 | 0.065 | 0.087 | 94.031 | 1.00x |
| mixed.json | orjson | 0.049 | 0.060 | 0.177 | 94.031 | 1.09x |
| mixed.json | msgspec | 0.059 | 0.072 | 0.083 | 94.031 | 0.91x |
| mixed.json | ujson | 0.246 | 0.267 | 0.356 | 94.031 | 0.24x |
| mixed.json | json | 0.395 | 0.449 | 0.475 | 94.031 | 0.15x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.189 | 8.789 | 11.269 | 88.828 | 1.00x |
| users.json | orjson | 11.652 | 12.760 | 29.638 | 88.828 | 0.69x |
| users.json | msgspec | 11.060 | 11.668 | 25.323 | 88.828 | 0.75x |
| users.json | ujson | 16.177 | 17.569 | 27.216 | 88.828 | 0.50x |
| users.json | json | 18.327 | 20.474 | 27.209 | 88.828 | 0.43x |
| flat.json | strata | 0.715 | 0.853 | 2.520 | 92.094 | 1.00x |
| flat.json | orjson | 1.098 | 1.248 | 2.711 | 92.094 | 0.68x |
| flat.json | msgspec | 0.922 | 1.018 | 1.504 | 92.094 | 0.84x |
| flat.json | ujson | 1.309 | 1.488 | 2.619 | 92.094 | 0.57x |
| flat.json | json | 1.659 | 1.837 | 3.777 | 92.094 | 0.46x |
| nested.json | strata | 0.716 | 0.773 | 2.280 | 92.094 | 1.00x |
| nested.json | orjson | 1.057 | 1.187 | 1.617 | 92.094 | 0.65x |
| nested.json | msgspec | 0.929 | 1.006 | 1.132 | 92.094 | 0.77x |
| nested.json | ujson | 1.209 | 1.358 | 4.199 | 92.094 | 0.57x |
| nested.json | json | 1.642 | 1.996 | 4.818 | 92.094 | 0.39x |
| wide_arrays.json | strata | 3.641 | 4.137 | 5.112 | 93.625 | 1.00x |
| wide_arrays.json | orjson | 3.643 | 4.572 | 5.419 | 93.625 | 0.90x |
| wide_arrays.json | msgspec | 4.869 | 5.404 | 8.287 | 93.625 | 0.77x |
| wide_arrays.json | ujson | 6.141 | 7.084 | 9.152 | 93.625 | 0.58x |
| wide_arrays.json | json | 7.619 | 8.552 | 11.101 | 93.625 | 0.48x |
| mixed.json | strata | 0.181 | 0.203 | 0.245 | 94.031 | 1.00x |
| mixed.json | orjson | 0.299 | 0.369 | 0.646 | 94.031 | 0.55x |
| mixed.json | msgspec | 0.259 | 0.314 | 0.389 | 94.031 | 0.65x |
| mixed.json | ujson | 0.319 | 0.369 | 0.489 | 94.031 | 0.55x |
| mixed.json | json | 0.454 | 0.482 | 0.497 | 94.031 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 8.444 | 10.240 | 11.853 | 92.047 | 1.00x |
| users.ndjson | orjson | 14.353 | 16.085 | 28.973 | 92.047 | 0.64x |
| users.ndjson | msgspec | 14.118 | 15.445 | 21.892 | 92.047 | 0.66x |
| users.ndjson | ujson | 16.104 | 19.464 | 23.586 | 92.047 | 0.53x |
| users.ndjson | json | 23.149 | 25.060 | 37.336 | 92.047 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.290 | 2.720 | 3.182 | 86.266 | 1.00x |
| users.json | orjson | 2.873 | 3.298 | 5.201 | 86.266 | 0.82x |
| users.json | msgspec | 3.666 | 4.092 | 8.297 | 86.266 | 0.66x |
| users.json | ujson | 12.909 | 14.891 | 16.771 | 86.266 | 0.18x |
| users.json | json | 17.763 | 19.628 | 26.093 | 86.266 | 0.14x |
| flat.json | strata | 0.483 | 0.744 | 1.479 | 92.094 | 1.00x |
| flat.json | orjson | 0.596 | 0.701 | 1.325 | 92.094 | 1.06x |
| flat.json | msgspec | 0.640 | 0.803 | 1.973 | 92.094 | 0.93x |
| flat.json | ujson | 1.249 | 1.448 | 6.237 | 92.094 | 0.51x |
| flat.json | json | 1.775 | 1.941 | 2.318 | 92.094 | 0.38x |
| nested.json | strata | 0.327 | 0.485 | 0.651 | 92.094 | 1.00x |
| nested.json | orjson | 0.496 | 0.571 | 0.795 | 92.094 | 0.85x |
| nested.json | msgspec | 0.584 | 0.814 | 1.645 | 92.094 | 0.60x |
| nested.json | ujson | 1.547 | 1.690 | 2.771 | 92.094 | 0.29x |
| nested.json | json | 2.065 | 2.270 | 3.757 | 92.094 | 0.21x |
| wide_arrays.json | strata | 1.398 | 1.763 | 2.424 | 94.016 | 1.00x |
| wide_arrays.json | orjson | 1.739 | 2.102 | 2.455 | 94.016 | 0.84x |
| wide_arrays.json | msgspec | 2.775 | 2.936 | 3.361 | 94.016 | 0.60x |
| wide_arrays.json | ujson | 7.781 | 8.053 | 9.499 | 94.016 | 0.22x |
| wide_arrays.json | json | 12.519 | 13.766 | 14.599 | 94.016 | 0.13x |
| mixed.json | strata | 0.201 | 0.273 | 6.051 | 94.031 | 1.00x |
| mixed.json | orjson | 0.216 | 0.274 | 12.360 | 94.031 | 1.00x |
| mixed.json | msgspec | 0.221 | 0.271 | 5.643 | 94.031 | 1.01x |
| mixed.json | ujson | 0.387 | 0.469 | 11.873 | 94.031 | 0.58x |
| mixed.json | json | 0.544 | 0.647 | 11.773 | 94.031 | 0.42x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.083 | 0.117 | 0.445 | 86.328 | 1.00x |
| users.json $[*].id | jmespath | 0.314 | 0.411 | 1.071 | 86.328 | 0.28x |
| users.json $[*].id | jsonpath-ng | 1.641 | 1.840 | 3.761 | 86.328 | 0.06x |
| users.json $[*].orders[*].total | strata | 0.596 | 0.733 | 2.756 | 86.531 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.979 | 2.400 | 5.487 | 86.531 | 0.31x |
| users.json $[*].orders[*].total | jsonpath-ng | 11.044 | 14.423 | 25.009 | 86.531 | 0.05x |
| users.json $..total | strata | 1.409 | 1.597 | 3.451 | 87.578 | 1.00x |
| users.json $..total | jsonpath-ng | 261.153 | 290.528 | 406.358 | 87.578 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.977 | 4.442 | 6.630 | 86.406 | 1.00x |
| users.json $[*].id | orjson+jmespath | 11.970 | 14.119 | 30.062 | 86.406 | 0.31x |
| users.json $[*].id | orjson+jsonpath-ng | 13.243 | 14.964 | 19.473 | 86.406 | 0.30x |
| users.json $[*].orders[*].total | strata | 4.452 | 4.710 | 6.910 | 87.547 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 13.597 | 15.289 | 25.555 | 87.547 | 0.31x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 27.180 | 31.123 | 86.444 | 87.547 | 0.15x |
| users.json $..total | strata | 10.436 | 11.333 | 22.160 | 87.422 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 251.769 | 316.419 | 450.661 | 87.422 | 0.04x |

