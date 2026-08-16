# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: f35a664
- python: 3.12.10
- implementation: CPython
- platform: macOS-15.7.7-x86_64-i386-64bit
- machine: x86_64
- processor: Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 20.146 | 21.336 | 23.271 | 52.625 | 1.00x |
| users.json | orjson | 27.275 | 28.954 | 32.454 | 52.625 | 0.74x |
| users.json | msgspec | 27.433 | 28.989 | 40.136 | 52.625 | 0.74x |
| users.json | ujson | 41.127 | 43.337 | 46.569 | 52.625 | 0.49x |
| users.json | pysimdjson | 167.028 | 177.643 | 182.718 | 52.625 | 0.12x |
| users.json | json | 46.187 | 48.920 | 58.420 | 52.625 | 0.44x |
| flat.json | strata | 1.209 | 1.253 | 1.497 | 61.859 | 1.00x |
| flat.json | orjson | 1.280 | 1.307 | 1.446 | 61.859 | 0.96x |
| flat.json | msgspec | 1.457 | 1.498 | 1.537 | 61.859 | 0.84x |
| flat.json | ujson | 2.574 | 2.698 | 3.036 | 61.859 | 0.46x |
| flat.json | pysimdjson | 13.355 | 13.872 | 14.178 | 61.859 | 0.09x |
| flat.json | json | 2.928 | 2.995 | 3.283 | 61.859 | 0.42x |
| nested.json | strata | 1.319 | 1.427 | 1.589 | 56.074 | 1.00x |
| nested.json | orjson | 1.481 | 1.568 | 1.648 | 56.074 | 0.91x |
| nested.json | msgspec | 1.635 | 1.710 | 1.777 | 56.074 | 0.83x |
| nested.json | ujson | 2.674 | 2.875 | 2.995 | 56.074 | 0.50x |
| nested.json | pysimdjson | 12.379 | 12.717 | 13.368 | 56.074 | 0.11x |
| nested.json | json | 3.481 | 3.690 | 3.806 | 56.074 | 0.39x |
| wide_arrays.json | strata | 7.602 | 8.304 | 8.713 | 60.434 | 1.00x |
| wide_arrays.json | orjson | 8.079 | 8.996 | 9.874 | 60.434 | 0.92x |
| wide_arrays.json | msgspec | 9.476 | 9.862 | 10.563 | 60.434 | 0.84x |
| wide_arrays.json | ujson | 12.250 | 12.673 | 13.803 | 60.434 | 0.66x |
| wide_arrays.json | pysimdjson | 74.409 | 75.567 | 76.233 | 60.434 | 0.11x |
| wide_arrays.json | json | 16.059 | 16.494 | 17.023 | 60.434 | 0.50x |
| mixed.json | strata | 0.339 | 0.347 | 0.384 | 60.383 | 1.00x |
| mixed.json | orjson | 0.388 | 0.400 | 0.427 | 60.383 | 0.87x |
| mixed.json | msgspec | 0.417 | 0.426 | 0.464 | 60.383 | 0.82x |
| mixed.json | ujson | 0.552 | 0.588 | 0.617 | 60.383 | 0.59x |
| mixed.json | pysimdjson | 3.016 | 3.033 | 3.234 | 60.383 | 0.11x |
| mixed.json | json | 0.783 | 0.830 | 0.907 | 60.383 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.540 | 3.049 | 3.460 | 49.207 | 1.00x |
| users.json | orjson | 3.369 | 4.181 | 5.101 | 49.207 | 0.73x |
| users.json | msgspec | 5.405 | 6.195 | 8.094 | 49.207 | 0.49x |
| users.json | ujson | 30.478 | 33.787 | 35.864 | 49.207 | 0.09x |
| users.json | json | 40.643 | 44.539 | 45.996 | 49.207 | 0.07x |
| flat.json | strata | 0.270 | 0.293 | 0.323 | 57.578 | 1.00x |
| flat.json | orjson | 0.357 | 0.374 | 0.409 | 57.578 | 0.78x |
| flat.json | msgspec | 0.486 | 0.500 | 0.581 | 57.578 | 0.59x |
| flat.json | ujson | 2.467 | 2.492 | 2.534 | 57.578 | 0.12x |
| flat.json | json | 3.392 | 3.444 | 3.531 | 57.578 | 0.08x |
| nested.json | strata | 0.218 | 0.222 | 0.235 | 50.492 | 1.00x |
| nested.json | orjson | 0.321 | 0.324 | 0.365 | 50.492 | 0.69x |
| nested.json | msgspec | 0.508 | 0.513 | 0.611 | 50.492 | 0.43x |
| nested.json | ujson | 2.604 | 2.614 | 2.882 | 50.492 | 0.09x |
| nested.json | json | 4.325 | 4.357 | 4.436 | 50.492 | 0.05x |
| wide_arrays.json | strata | 1.674 | 1.810 | 2.056 | 63.465 | 1.00x |
| wide_arrays.json | orjson | 2.198 | 2.248 | 2.401 | 63.465 | 0.81x |
| wide_arrays.json | msgspec | 3.068 | 3.135 | 3.490 | 63.465 | 0.58x |
| wide_arrays.json | ujson | 14.264 | 14.492 | 14.810 | 63.465 | 0.12x |
| wide_arrays.json | json | 31.851 | 32.111 | 33.036 | 63.465 | 0.06x |
| mixed.json | strata | 0.057 | 0.072 | 0.117 | 56.125 | 1.00x |
| mixed.json | orjson | 0.059 | 0.068 | 0.087 | 56.125 | 1.04x |
| mixed.json | msgspec | 0.091 | 0.101 | 0.115 | 56.125 | 0.71x |
| mixed.json | ujson | 0.548 | 0.582 | 0.624 | 56.125 | 0.12x |
| mixed.json | json | 0.853 | 0.907 | 1.619 | 56.125 | 0.08x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 20.047 | 22.362 | 51.539 | 61.562 | 1.00x |
| users.json | orjson | 27.672 | 30.588 | 35.655 | 61.562 | 0.73x |
| users.json | msgspec | 26.656 | 30.378 | 36.425 | 61.562 | 0.74x |
| users.json | ujson | 41.155 | 44.437 | 63.339 | 61.562 | 0.50x |
| users.json | json | 44.249 | 51.253 | 67.431 | 61.562 | 0.44x |
| flat.json | strata | 1.297 | 1.348 | 1.650 | 57.578 | 1.00x |
| flat.json | orjson | 1.401 | 1.408 | 1.510 | 57.578 | 0.96x |
| flat.json | msgspec | 1.564 | 1.617 | 1.701 | 57.578 | 0.83x |
| flat.json | ujson | 2.655 | 2.732 | 2.880 | 57.578 | 0.49x |
| flat.json | json | 3.022 | 3.084 | 3.160 | 57.578 | 0.44x |
| nested.json | strata | 1.421 | 1.554 | 1.896 | 49.984 | 1.00x |
| nested.json | orjson | 1.553 | 1.720 | 1.802 | 49.984 | 0.90x |
| nested.json | msgspec | 1.717 | 1.896 | 1.989 | 49.984 | 0.82x |
| nested.json | ujson | 2.805 | 3.035 | 3.170 | 49.984 | 0.51x |
| nested.json | json | 3.653 | 3.830 | 4.240 | 49.984 | 0.41x |
| wide_arrays.json | strata | 7.860 | 8.195 | 8.574 | 63.465 | 1.00x |
| wide_arrays.json | orjson | 8.331 | 8.774 | 9.597 | 63.465 | 0.93x |
| wide_arrays.json | msgspec | 9.367 | 9.690 | 10.252 | 63.465 | 0.85x |
| wide_arrays.json | ujson | 12.486 | 12.744 | 13.149 | 63.465 | 0.64x |
| wide_arrays.json | json | 15.965 | 16.136 | 16.842 | 63.465 | 0.51x |
| mixed.json | strata | 0.417 | 0.431 | 0.493 | 56.125 | 1.00x |
| mixed.json | orjson | 0.501 | 0.514 | 0.569 | 56.125 | 0.84x |
| mixed.json | msgspec | 0.532 | 0.541 | 0.599 | 56.125 | 0.80x |
| mixed.json | ujson | 0.704 | 0.713 | 0.781 | 56.125 | 0.60x |
| mixed.json | json | 0.923 | 0.935 | 1.020 | 56.125 | 0.46x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 18.559 | 19.268 | 20.455 | 60.242 | 1.00x |
| users.ndjson | orjson | 24.989 | 25.409 | 26.680 | 60.242 | 0.76x |
| users.ndjson | msgspec | 25.680 | 25.952 | 27.292 | 60.242 | 0.74x |
| users.ndjson | ujson | 36.560 | 37.304 | 38.833 | 60.242 | 0.52x |
| users.ndjson | json | 46.454 | 46.940 | 47.834 | 60.242 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.438 | 3.571 | 3.775 | 57.648 | 1.00x |
| users.json | orjson | 4.185 | 4.312 | 4.703 | 57.648 | 0.83x |
| users.json | msgspec | 6.141 | 6.254 | 6.876 | 57.648 | 0.57x |
| users.json | ujson | 31.024 | 31.309 | 31.857 | 57.648 | 0.11x |
| users.json | json | 40.517 | 40.719 | 41.856 | 57.648 | 0.09x |
| flat.json | strata | 0.622 | 0.679 | 0.992 | 57.578 | 1.00x |
| flat.json | orjson | 0.710 | 0.937 | 1.241 | 57.578 | 0.72x |
| flat.json | msgspec | 0.860 | 1.092 | 1.847 | 57.578 | 0.62x |
| flat.json | ujson | 2.838 | 2.912 | 3.262 | 57.578 | 0.23x |
| flat.json | json | 3.785 | 4.029 | 4.685 | 57.578 | 0.17x |
| nested.json | strata | 0.572 | 0.603 | 0.661 | 49.984 | 1.00x |
| nested.json | orjson | 0.681 | 0.719 | 1.586 | 49.984 | 0.84x |
| nested.json | msgspec | 0.870 | 0.899 | 1.351 | 49.984 | 0.67x |
| nested.json | ujson | 3.087 | 3.213 | 15.614 | 49.984 | 0.19x |
| nested.json | json | 4.788 | 4.890 | 5.331 | 49.984 | 0.12x |
| wide_arrays.json | strata | 2.434 | 2.477 | 2.805 | 63.465 | 1.00x |
| wide_arrays.json | orjson | 2.737 | 2.921 | 3.121 | 63.465 | 0.85x |
| wide_arrays.json | msgspec | 3.447 | 3.708 | 4.268 | 63.465 | 0.67x |
| wide_arrays.json | ujson | 14.866 | 15.075 | 15.912 | 63.465 | 0.16x |
| wide_arrays.json | json | 31.731 | 33.108 | 33.634 | 63.465 | 0.07x |
| mixed.json | strata | 0.303 | 0.380 | 0.418 | 56.125 | 1.00x |
| mixed.json | orjson | 0.317 | 0.382 | 0.555 | 56.125 | 1.00x |
| mixed.json | msgspec | 0.337 | 0.418 | 0.478 | 56.125 | 0.91x |
| mixed.json | ujson | 0.840 | 0.887 | 0.986 | 56.125 | 0.43x |
| mixed.json | json | 1.162 | 1.254 | 1.716 | 56.125 | 0.30x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.137 | 0.177 | 0.307 | 56.336 | 1.00x |
| users.json $[*].id | jmespath | 0.840 | 0.962 | 1.089 | 56.336 | 0.18x |
| users.json $[*].id | jsonpath-ng | 4.631 | 5.487 | 13.344 | 56.336 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.939 | 1.168 | 1.676 | 55.945 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.720 | 6.122 | 6.751 | 55.945 | 0.19x |
| users.json $[*].orders[*].total | jsonpath-ng | 33.842 | 38.816 | 47.777 | 55.945 | 0.03x |
| users.json $..total | strata | 2.892 | 3.188 | 3.756 | 57.961 | 1.00x |
| users.json $..total | jsonpath-ng | 650.745 | 655.224 | 688.705 | 57.961 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.031 | 4.370 | 4.544 | 57.367 | 1.00x |
| users.json $[*].id | orjson+jmespath | 29.783 | 31.075 | 33.347 | 57.367 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 33.087 | 35.919 | 38.983 | 57.367 | 0.12x |
| users.json $[*].orders[*].total | strata | 4.223 | 4.249 | 4.295 | 57.160 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 28.839 | 31.001 | 34.027 | 57.160 | 0.14x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 61.806 | 63.714 | 68.769 | 57.160 | 0.07x |
| users.json $..total | strata | 22.512 | 22.984 | 23.481 | 57.219 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 682.340 | 688.344 | 709.746 | 57.219 | 0.03x |

