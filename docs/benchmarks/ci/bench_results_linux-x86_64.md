# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 32c5fa4
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: x86_64
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.520 | 9.700 | 13.475 | 58.531 | 1.00x |
| users.json | orjson | 12.836 | 13.009 | 16.095 | 58.531 | 0.75x |
| users.json | msgspec | 12.844 | 13.088 | 15.655 | 58.531 | 0.74x |
| users.json | ujson | 17.335 | 18.038 | 22.965 | 58.531 | 0.54x |
| users.json | pysimdjson | 18.132 | 18.822 | 24.072 | 58.531 | 0.52x |
| users.json | json | 22.172 | 22.764 | 24.177 | 58.531 | 0.43x |
| flat.json | strata | 0.879 | 0.899 | 0.913 | 68.570 | 1.00x |
| flat.json | orjson | 0.995 | 0.998 | 1.012 | 68.570 | 0.90x |
| flat.json | msgspec | 1.019 | 1.033 | 1.051 | 68.570 | 0.87x |
| flat.json | ujson | 1.474 | 1.485 | 1.535 | 68.570 | 0.61x |
| flat.json | pysimdjson | 1.559 | 1.578 | 1.589 | 68.570 | 0.57x |
| flat.json | json | 1.920 | 1.937 | 1.944 | 68.570 | 0.46x |
| nested.json | strata | 0.809 | 0.821 | 0.832 | 68.570 | 1.00x |
| nested.json | orjson | 0.994 | 0.998 | 1.037 | 68.570 | 0.82x |
| nested.json | msgspec | 1.009 | 1.020 | 1.034 | 68.570 | 0.81x |
| nested.json | ujson | 1.431 | 1.451 | 1.587 | 68.570 | 0.57x |
| nested.json | pysimdjson | 1.399 | 1.414 | 1.507 | 68.570 | 0.58x |
| nested.json | json | 2.000 | 2.026 | 2.066 | 68.570 | 0.41x |
| wide_arrays.json | strata | 4.056 | 4.086 | 4.387 | 73.699 | 1.00x |
| wide_arrays.json | orjson | 5.061 | 5.135 | 5.450 | 73.699 | 0.80x |
| wide_arrays.json | msgspec | 5.619 | 5.730 | 5.892 | 73.699 | 0.71x |
| wide_arrays.json | ujson | 6.935 | 7.075 | 7.521 | 73.699 | 0.58x |
| wide_arrays.json | pysimdjson | 6.057 | 6.121 | 6.844 | 73.699 | 0.67x |
| wide_arrays.json | json | 9.586 | 9.693 | 9.987 | 73.699 | 0.42x |
| mixed.json | strata | 0.190 | 0.191 | 0.206 | 73.762 | 1.00x |
| mixed.json | orjson | 0.226 | 0.228 | 0.241 | 73.762 | 0.84x |
| mixed.json | msgspec | 0.236 | 0.238 | 0.253 | 73.762 | 0.80x |
| mixed.json | ujson | 0.296 | 0.300 | 0.312 | 73.762 | 0.64x |
| mixed.json | pysimdjson | 0.296 | 0.308 | 0.342 | 73.762 | 0.62x |
| mixed.json | json | 0.467 | 0.478 | 0.496 | 73.762 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.402 | 2.422 | 2.439 | 57.664 | 1.00x |
| users.json | orjson | 2.894 | 2.910 | 2.949 | 57.664 | 0.83x |
| users.json | msgspec | 3.827 | 3.849 | 3.990 | 57.664 | 0.63x |
| users.json | ujson | 11.342 | 11.393 | 11.619 | 57.664 | 0.21x |
| users.json | json | 21.521 | 21.643 | 21.979 | 57.664 | 0.11x |
| flat.json | strata | 0.260 | 0.262 | 0.275 | 68.570 | 1.00x |
| flat.json | orjson | 0.328 | 0.338 | 0.350 | 68.570 | 0.77x |
| flat.json | msgspec | 0.430 | 0.437 | 0.447 | 68.570 | 0.60x |
| flat.json | ujson | 1.029 | 1.037 | 1.047 | 68.570 | 0.25x |
| flat.json | json | 1.836 | 1.861 | 1.932 | 68.570 | 0.14x |
| nested.json | strata | 0.257 | 0.265 | 0.363 | 68.574 | 1.00x |
| nested.json | orjson | 0.294 | 0.306 | 0.319 | 68.574 | 0.87x |
| nested.json | msgspec | 0.403 | 0.406 | 0.430 | 68.574 | 0.65x |
| nested.json | ujson | 1.106 | 1.113 | 1.121 | 68.574 | 0.24x |
| nested.json | json | 2.328 | 2.340 | 2.407 | 68.574 | 0.11x |
| wide_arrays.json | strata | 1.488 | 1.494 | 1.521 | 73.699 | 1.00x |
| wide_arrays.json | orjson | 1.753 | 1.761 | 1.774 | 73.699 | 0.85x |
| wide_arrays.json | msgspec | 2.667 | 2.682 | 2.721 | 73.699 | 0.56x |
| wide_arrays.json | ujson | 6.270 | 6.333 | 6.438 | 73.699 | 0.24x |
| wide_arrays.json | json | 16.351 | 16.459 | 17.208 | 73.699 | 0.09x |
| mixed.json | strata | 0.062 | 0.063 | 0.096 | 73.762 | 1.00x |
| mixed.json | orjson | 0.065 | 0.066 | 0.068 | 73.762 | 0.96x |
| mixed.json | msgspec | 0.084 | 0.086 | 0.092 | 73.762 | 0.74x |
| mixed.json | ujson | 0.230 | 0.232 | 0.239 | 73.762 | 0.27x |
| mixed.json | json | 0.508 | 0.517 | 0.530 | 73.762 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.512 | 10.933 | 11.934 | 75.172 | 1.00x |
| users.json | orjson | 13.510 | 13.843 | 14.285 | 75.172 | 0.79x |
| users.json | msgspec | 13.480 | 13.835 | 14.118 | 75.172 | 0.79x |
| users.json | ujson | 18.666 | 19.620 | 20.836 | 75.172 | 0.56x |
| users.json | json | 22.898 | 23.146 | 23.525 | 75.172 | 0.47x |
| flat.json | strata | 0.918 | 0.926 | 0.945 | 68.570 | 1.00x |
| flat.json | orjson | 1.055 | 1.062 | 1.088 | 68.570 | 0.87x |
| flat.json | msgspec | 1.077 | 1.089 | 1.107 | 68.570 | 0.85x |
| flat.json | ujson | 1.562 | 1.575 | 1.616 | 68.570 | 0.59x |
| flat.json | json | 1.990 | 1.997 | 2.010 | 68.570 | 0.46x |
| nested.json | strata | 0.833 | 0.843 | 0.868 | 68.574 | 1.00x |
| nested.json | orjson | 1.040 | 1.047 | 1.106 | 68.574 | 0.81x |
| nested.json | msgspec | 1.064 | 1.069 | 1.086 | 68.574 | 0.79x |
| nested.json | ujson | 1.495 | 1.511 | 1.578 | 68.574 | 0.56x |
| nested.json | json | 2.072 | 2.081 | 2.130 | 68.574 | 0.40x |
| wide_arrays.json | strata | 4.144 | 4.181 | 4.308 | 73.762 | 1.00x |
| wide_arrays.json | orjson | 5.106 | 5.149 | 5.262 | 73.762 | 0.81x |
| wide_arrays.json | msgspec | 5.745 | 5.789 | 5.988 | 73.762 | 0.72x |
| wide_arrays.json | ujson | 7.196 | 7.227 | 7.266 | 73.762 | 0.58x |
| wide_arrays.json | json | 9.654 | 9.720 | 9.926 | 73.762 | 0.43x |
| mixed.json | strata | 0.209 | 0.211 | 0.237 | 73.762 | 1.00x |
| mixed.json | orjson | 0.272 | 0.273 | 0.285 | 73.762 | 0.77x |
| mixed.json | msgspec | 0.277 | 0.287 | 0.311 | 73.762 | 0.73x |
| mixed.json | ujson | 0.352 | 0.355 | 0.374 | 73.762 | 0.59x |
| mixed.json | json | 0.511 | 0.526 | 0.531 | 73.762 | 0.40x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.966 | 10.445 | 11.544 | 68.570 | 1.00x |
| users.ndjson | orjson | 16.473 | 16.933 | 17.871 | 68.570 | 0.62x |
| users.ndjson | msgspec | 16.361 | 16.944 | 17.551 | 68.570 | 0.62x |
| users.ndjson | ujson | 21.314 | 21.741 | 22.769 | 68.570 | 0.48x |
| users.ndjson | json | 28.849 | 29.795 | 30.628 | 68.570 | 0.35x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.000 | 3.041 | 3.078 | 73.203 | 1.00x |
| users.json | orjson | 3.558 | 3.610 | 3.673 | 73.203 | 0.84x |
| users.json | msgspec | 4.408 | 4.479 | 4.535 | 73.203 | 0.68x |
| users.json | ujson | 12.113 | 12.252 | 12.756 | 73.203 | 0.25x |
| users.json | json | 22.371 | 22.750 | 22.872 | 73.203 | 0.13x |
| flat.json | strata | 0.403 | 0.421 | 0.498 | 68.570 | 1.00x |
| flat.json | orjson | 0.489 | 0.495 | 0.530 | 68.570 | 0.85x |
| flat.json | msgspec | 0.606 | 0.622 | 0.636 | 68.570 | 0.68x |
| flat.json | ujson | 1.198 | 1.207 | 1.250 | 68.570 | 0.35x |
| flat.json | json | 2.025 | 2.040 | 2.077 | 68.570 | 0.21x |
| nested.json | strata | 0.371 | 0.377 | 0.409 | 68.574 | 1.00x |
| nested.json | orjson | 0.423 | 0.433 | 0.492 | 68.574 | 0.87x |
| nested.json | msgspec | 0.535 | 0.559 | 0.588 | 68.574 | 0.67x |
| nested.json | ujson | 1.274 | 1.296 | 1.319 | 68.574 | 0.29x |
| nested.json | json | 2.514 | 2.548 | 2.576 | 68.574 | 0.15x |
| wide_arrays.json | strata | 1.890 | 1.918 | 1.974 | 73.762 | 1.00x |
| wide_arrays.json | orjson | 2.183 | 2.205 | 2.383 | 73.762 | 0.87x |
| wide_arrays.json | msgspec | 3.094 | 3.138 | 3.188 | 73.762 | 0.61x |
| wide_arrays.json | ujson | 6.803 | 6.847 | 6.934 | 73.762 | 0.28x |
| wide_arrays.json | json | 16.892 | 16.956 | 17.220 | 73.762 | 0.11x |
| mixed.json | strata | 0.151 | 0.153 | 0.185 | 73.762 | 1.00x |
| mixed.json | orjson | 0.169 | 0.170 | 0.179 | 73.762 | 0.90x |
| mixed.json | msgspec | 0.188 | 0.191 | 0.227 | 73.762 | 0.80x |
| mixed.json | ujson | 0.343 | 0.351 | 0.380 | 73.762 | 0.44x |
| mixed.json | json | 0.615 | 0.637 | 0.681 | 73.762 | 0.24x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.064 | 0.067 | 0.079 | 73.207 | 1.00x |
| users.json $[*].id | jmespath | 0.491 | 0.503 | 0.522 | 73.207 | 0.13x |
| users.json $[*].id | jsonpath-ng | 2.803 | 2.903 | 3.020 | 73.207 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.425 | 0.451 | 0.476 | 73.207 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.083 | 3.098 | 3.126 | 73.207 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 19.147 | 19.599 | 20.147 | 73.207 | 0.02x |
| users.json $..total | strata | 1.691 | 1.714 | 1.789 | 73.207 | 1.00x |
| users.json $..total | jsonpath-ng | 383.377 | 387.155 | 388.601 | 73.207 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.272 | 3.285 | 3.316 | 73.207 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.996 | 14.167 | 14.307 | 73.207 | 0.23x |
| users.json $[*].id | orjson+jsonpath-ng | 16.278 | 16.510 | 16.993 | 73.207 | 0.20x |
| users.json $[*].orders[*].total | strata | 3.519 | 3.558 | 3.600 | 73.207 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 16.682 | 17.111 | 17.708 | 73.207 | 0.21x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 35.733 | 37.373 | 39.206 | 73.207 | 0.10x |
| users.json $..total | strata | 12.735 | 13.539 | 13.878 | 73.191 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 404.599 | 407.781 | 408.879 | 73.191 | 0.03x |

