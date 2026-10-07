# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 32c5fa4
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
| users.json | strata | 6.475 | 7.985 | 13.327 | 63.609 | 1.00x |
| users.json | orjson | 10.225 | 11.310 | 28.677 | 63.609 | 0.71x |
| users.json | msgspec | 9.747 | 10.672 | 21.653 | 63.609 | 0.75x |
| users.json | ujson | 13.315 | 15.843 | 26.186 | 63.609 | 0.50x |
| users.json | pysimdjson | 127.415 | 131.146 | 144.423 | 63.609 | 0.06x |
| users.json | json | 16.063 | 16.604 | 28.070 | 63.609 | 0.48x |
| flat.json | strata | 0.659 | 0.702 | 0.839 | 89.234 | 1.00x |
| flat.json | orjson | 0.831 | 0.860 | 1.833 | 89.234 | 0.82x |
| flat.json | msgspec | 0.758 | 0.820 | 2.002 | 89.234 | 0.86x |
| flat.json | ujson | 1.183 | 1.376 | 2.431 | 89.234 | 0.51x |
| flat.json | pysimdjson | 12.613 | 13.317 | 15.742 | 89.234 | 0.05x |
| flat.json | json | 1.435 | 1.707 | 4.448 | 89.234 | 0.41x |
| nested.json | strata | 0.540 | 0.571 | 0.751 | 89.250 | 1.00x |
| nested.json | orjson | 0.784 | 0.807 | 0.860 | 89.250 | 0.71x |
| nested.json | msgspec | 0.720 | 0.743 | 0.822 | 89.250 | 0.77x |
| nested.json | ujson | 1.185 | 1.232 | 1.286 | 89.250 | 0.46x |
| nested.json | pysimdjson | 10.588 | 10.848 | 11.134 | 89.250 | 0.05x |
| nested.json | json | 1.485 | 1.549 | 1.758 | 89.250 | 0.37x |
| wide_arrays.json | strata | 3.169 | 3.222 | 3.295 | 92.016 | 1.00x |
| wide_arrays.json | orjson | 3.912 | 4.036 | 4.101 | 92.016 | 0.80x |
| wide_arrays.json | msgspec | 4.356 | 4.442 | 4.711 | 92.016 | 0.73x |
| wide_arrays.json | ujson | 5.522 | 5.739 | 6.203 | 92.016 | 0.56x |
| wide_arrays.json | pysimdjson | 65.167 | 65.754 | 67.695 | 92.016 | 0.05x |
| wide_arrays.json | json | 7.174 | 7.321 | 7.615 | 92.016 | 0.44x |
| mixed.json | strata | 0.126 | 0.130 | 0.143 | 92.031 | 1.00x |
| mixed.json | orjson | 0.161 | 0.175 | 0.187 | 92.031 | 0.74x |
| mixed.json | msgspec | 0.176 | 0.180 | 0.195 | 92.031 | 0.72x |
| mixed.json | ujson | 0.221 | 0.368 | 0.538 | 92.031 | 0.35x |
| mixed.json | pysimdjson | 2.553 | 2.587 | 2.620 | 92.031 | 0.05x |
| mixed.json | json | 0.333 | 0.342 | 0.353 | 92.031 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.623 | 1.767 | 3.068 | 76.391 | 1.00x |
| users.json | orjson | 2.352 | 2.413 | 4.575 | 76.391 | 0.73x |
| users.json | msgspec | 3.076 | 3.215 | 5.021 | 76.391 | 0.55x |
| users.json | ujson | 9.192 | 9.367 | 10.201 | 76.391 | 0.19x |
| users.json | json | 16.211 | 16.455 | 19.837 | 76.391 | 0.11x |
| flat.json | strata | 0.268 | 0.290 | 0.596 | 89.234 | 1.00x |
| flat.json | orjson | 0.293 | 0.308 | 0.644 | 89.234 | 0.94x |
| flat.json | msgspec | 0.343 | 0.379 | 0.803 | 89.234 | 0.77x |
| flat.json | ujson | 0.841 | 0.907 | 2.362 | 89.234 | 0.32x |
| flat.json | json | 1.565 | 1.695 | 1.872 | 89.234 | 0.17x |
| nested.json | strata | 0.148 | 0.159 | 0.172 | 89.250 | 1.00x |
| nested.json | orjson | 0.237 | 0.247 | 0.277 | 89.250 | 0.64x |
| nested.json | msgspec | 0.310 | 0.400 | 0.500 | 89.250 | 0.40x |
| nested.json | ujson | 0.841 | 1.020 | 1.049 | 89.250 | 0.16x |
| nested.json | json | 1.737 | 1.788 | 1.864 | 89.250 | 0.09x |
| wide_arrays.json | strata | 1.123 | 1.175 | 1.391 | 92.016 | 1.00x |
| wide_arrays.json | orjson | 1.474 | 1.627 | 1.687 | 92.016 | 0.72x |
| wide_arrays.json | msgspec | 2.192 | 2.249 | 2.398 | 92.016 | 0.52x |
| wide_arrays.json | ujson | 5.026 | 5.142 | 5.314 | 92.016 | 0.23x |
| wide_arrays.json | json | 12.077 | 12.193 | 12.241 | 92.016 | 0.10x |
| mixed.json | strata | 0.041 | 0.045 | 0.052 | 92.031 | 1.00x |
| mixed.json | orjson | 0.049 | 0.055 | 0.064 | 92.031 | 0.81x |
| mixed.json | msgspec | 0.059 | 0.148 | 0.288 | 92.031 | 0.30x |
| mixed.json | ujson | 0.179 | 0.182 | 0.209 | 92.031 | 0.25x |
| mixed.json | json | 0.368 | 0.379 | 0.480 | 92.031 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.937 | 6.998 | 8.866 | 88.438 | 1.00x |
| users.json | orjson | 10.538 | 10.788 | 11.213 | 88.438 | 0.65x |
| users.json | msgspec | 10.304 | 10.680 | 10.958 | 88.438 | 0.66x |
| users.json | ujson | 13.983 | 14.638 | 18.330 | 88.438 | 0.48x |
| users.json | json | 16.193 | 16.767 | 18.140 | 88.438 | 0.42x |
| flat.json | strata | 0.727 | 0.764 | 0.836 | 89.234 | 1.00x |
| flat.json | orjson | 1.013 | 1.090 | 1.175 | 89.234 | 0.70x |
| flat.json | msgspec | 0.878 | 0.929 | 2.030 | 89.234 | 0.82x |
| flat.json | ujson | 1.262 | 1.347 | 2.344 | 89.234 | 0.57x |
| flat.json | json | 1.559 | 1.600 | 1.758 | 89.234 | 0.48x |
| nested.json | strata | 0.618 | 0.647 | 0.672 | 89.250 | 1.00x |
| nested.json | orjson | 0.986 | 1.067 | 1.115 | 89.250 | 0.61x |
| nested.json | msgspec | 0.839 | 0.872 | 0.899 | 89.250 | 0.74x |
| nested.json | ujson | 1.123 | 1.170 | 1.317 | 89.250 | 0.55x |
| nested.json | json | 1.589 | 1.619 | 1.671 | 89.250 | 0.40x |
| wide_arrays.json | strata | 3.260 | 3.334 | 3.412 | 92.016 | 1.00x |
| wide_arrays.json | orjson | 4.020 | 4.165 | 4.475 | 92.016 | 0.80x |
| wide_arrays.json | msgspec | 4.625 | 4.748 | 5.046 | 92.016 | 0.70x |
| wide_arrays.json | ujson | 5.893 | 6.172 | 6.284 | 92.016 | 0.54x |
| wide_arrays.json | json | 7.383 | 7.593 | 8.142 | 92.016 | 0.44x |
| mixed.json | strata | 0.185 | 0.194 | 0.219 | 92.031 | 1.00x |
| mixed.json | orjson | 0.432 | 0.454 | 0.467 | 92.031 | 0.43x |
| mixed.json | msgspec | 0.274 | 0.287 | 0.305 | 92.031 | 0.67x |
| mixed.json | ujson | 0.318 | 0.336 | 0.377 | 92.031 | 0.58x |
| mixed.json | json | 0.413 | 0.429 | 0.460 | 92.031 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.316 | 7.419 | 8.527 | 89.219 | 1.00x |
| users.ndjson | orjson | 12.485 | 12.789 | 13.312 | 89.219 | 0.58x |
| users.ndjson | msgspec | 12.249 | 12.660 | 17.068 | 89.219 | 0.59x |
| users.ndjson | ujson | 14.891 | 16.593 | 28.178 | 89.219 | 0.45x |
| users.ndjson | json | 19.233 | 20.796 | 27.632 | 89.219 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.223 | 2.427 | 3.687 | 85.469 | 1.00x |
| users.json | orjson | 3.131 | 3.346 | 4.263 | 85.469 | 0.73x |
| users.json | msgspec | 3.609 | 3.955 | 4.768 | 85.469 | 0.61x |
| users.json | ujson | 10.297 | 10.839 | 16.384 | 85.469 | 0.22x |
| users.json | json | 17.857 | 18.832 | 23.323 | 85.469 | 0.13x |
| flat.json | strata | 0.493 | 0.572 | 1.288 | 89.234 | 1.00x |
| flat.json | orjson | 0.535 | 0.587 | 1.206 | 89.234 | 0.97x |
| flat.json | msgspec | 0.573 | 0.656 | 1.207 | 89.234 | 0.87x |
| flat.json | ujson | 1.106 | 1.202 | 1.559 | 89.234 | 0.48x |
| flat.json | json | 1.829 | 2.100 | 3.548 | 89.234 | 0.27x |
| nested.json | strata | 0.419 | 0.452 | 0.523 | 89.250 | 1.00x |
| nested.json | orjson | 0.540 | 0.582 | 1.792 | 89.250 | 0.78x |
| nested.json | msgspec | 0.652 | 0.764 | 1.796 | 89.250 | 0.59x |
| nested.json | ujson | 1.304 | 1.374 | 2.706 | 89.250 | 0.33x |
| nested.json | json | 1.995 | 2.098 | 4.500 | 89.250 | 0.22x |
| wide_arrays.json | strata | 1.569 | 1.650 | 1.810 | 92.016 | 1.00x |
| wide_arrays.json | orjson | 1.960 | 2.098 | 3.063 | 92.016 | 0.79x |
| wide_arrays.json | msgspec | 2.735 | 2.878 | 3.022 | 92.016 | 0.57x |
| wide_arrays.json | ujson | 5.520 | 5.764 | 6.502 | 92.016 | 0.29x |
| wide_arrays.json | json | 12.547 | 13.127 | 13.807 | 92.016 | 0.13x |
| mixed.json | strata | 0.263 | 0.352 | 0.392 | 92.031 | 1.00x |
| mixed.json | orjson | 0.277 | 0.395 | 0.452 | 92.031 | 0.89x |
| mixed.json | msgspec | 0.310 | 0.368 | 0.574 | 92.031 | 0.96x |
| mixed.json | ujson | 0.419 | 0.489 | 0.597 | 92.031 | 0.72x |
| mixed.json | json | 0.608 | 0.664 | 1.112 | 92.031 | 0.53x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.073 | 0.142 | 0.352 | 85.547 | 1.00x |
| users.json $[*].id | jmespath | 0.320 | 0.405 | 1.130 | 85.547 | 0.35x |
| users.json $[*].id | jsonpath-ng | 1.570 | 1.712 | 5.071 | 85.547 | 0.08x |
| users.json $[*].orders[*].total | strata | 0.596 | 0.686 | 0.767 | 85.688 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.010 | 2.237 | 3.049 | 85.688 | 0.31x |
| users.json $[*].orders[*].total | jsonpath-ng | 11.965 | 13.110 | 13.513 | 85.688 | 0.05x |
| users.json $..total | strata | 1.394 | 1.491 | 1.897 | 86.578 | 1.00x |
| users.json $..total | jsonpath-ng | 197.157 | 200.745 | 207.644 | 86.578 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.720 | 3.803 | 4.156 | 85.594 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.607 | 10.919 | 11.809 | 85.594 | 0.35x |
| users.json $[*].id | orjson+jsonpath-ng | 12.023 | 12.673 | 13.991 | 85.594 | 0.30x |
| users.json $[*].orders[*].total | strata | 3.855 | 4.016 | 4.240 | 86.547 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.821 | 13.370 | 14.761 | 86.547 | 0.30x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 26.448 | 28.803 | 35.492 | 86.547 | 0.14x |
| users.json $..total | strata | 8.562 | 8.749 | 9.289 | 86.578 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 208.507 | 212.439 | 218.260 | 86.578 | 0.04x |

