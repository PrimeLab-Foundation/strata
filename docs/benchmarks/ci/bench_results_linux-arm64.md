# Benchmark results - ci-linux-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 53fa480
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-aarch64-with-glibc2.39
- machine: aarch64
- processor: aarch64
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.939 | 9.070 | 10.945 | 51.816 | 1.00x |
| users.json | orjson | 11.611 | 11.703 | 13.277 | 51.816 | 0.78x |
| users.json | msgspec | 12.155 | 12.251 | 13.542 | 51.816 | 0.74x |
| users.json | ujson | 16.464 | 16.703 | 19.028 | 51.816 | 0.54x |
| users.json | pysimdjson | 16.529 | 16.670 | 18.647 | 51.816 | 0.54x |
| users.json | json | 20.655 | 20.838 | 21.354 | 51.816 | 0.44x |
| flat.json | strata | 0.824 | 0.835 | 0.860 | 58.680 | 1.00x |
| flat.json | orjson | 0.857 | 0.880 | 0.887 | 58.680 | 0.95x |
| flat.json | msgspec | 0.901 | 0.916 | 0.922 | 58.680 | 0.91x |
| flat.json | ujson | 1.413 | 1.435 | 1.469 | 58.680 | 0.58x |
| flat.json | pysimdjson | 1.456 | 1.471 | 1.484 | 58.680 | 0.57x |
| flat.json | json | 1.762 | 1.774 | 1.789 | 58.680 | 0.47x |
| nested.json | strata | 0.826 | 0.836 | 0.841 | 58.680 | 1.00x |
| nested.json | orjson | 0.877 | 0.893 | 0.899 | 58.680 | 0.94x |
| nested.json | msgspec | 0.996 | 1.004 | 1.015 | 58.680 | 0.83x |
| nested.json | ujson | 1.403 | 1.416 | 1.492 | 58.680 | 0.59x |
| nested.json | pysimdjson | 1.395 | 1.412 | 1.434 | 58.680 | 0.59x |
| nested.json | json | 1.978 | 1.989 | 2.005 | 58.680 | 0.42x |
| wide_arrays.json | strata | 4.399 | 4.432 | 4.469 | 60.246 | 1.00x |
| wide_arrays.json | orjson | 3.985 | 4.042 | 4.068 | 60.246 | 1.10x |
| wide_arrays.json | msgspec | 4.998 | 5.030 | 5.068 | 60.246 | 0.88x |
| wide_arrays.json | ujson | 6.434 | 6.476 | 6.499 | 60.246 | 0.68x |
| wide_arrays.json | pysimdjson | 5.202 | 5.250 | 5.315 | 60.246 | 0.84x |
| wide_arrays.json | json | 9.447 | 9.500 | 9.527 | 60.246 | 0.47x |
| mixed.json | strata | 0.196 | 0.198 | 0.225 | 60.246 | 1.00x |
| mixed.json | orjson | 0.216 | 0.220 | 0.246 | 60.246 | 0.90x |
| mixed.json | msgspec | 0.236 | 0.240 | 0.268 | 60.246 | 0.82x |
| mixed.json | ujson | 0.300 | 0.310 | 0.329 | 60.246 | 0.64x |
| mixed.json | pysimdjson | 0.289 | 0.296 | 0.323 | 60.246 | 0.67x |
| mixed.json | json | 0.458 | 0.470 | 0.482 | 60.246 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.071 | 2.081 | 2.090 | 50.910 | 1.00x |
| users.json | orjson | 2.574 | 2.587 | 2.618 | 50.910 | 0.80x |
| users.json | msgspec | 3.312 | 3.325 | 3.366 | 50.910 | 0.63x |
| users.json | ujson | 10.534 | 10.587 | 10.654 | 50.910 | 0.20x |
| users.json | json | 18.949 | 19.059 | 19.134 | 50.910 | 0.11x |
| flat.json | strata | 0.234 | 0.237 | 0.261 | 58.680 | 1.00x |
| flat.json | orjson | 0.301 | 0.303 | 0.328 | 58.680 | 0.78x |
| flat.json | msgspec | 0.386 | 0.399 | 0.415 | 58.680 | 0.59x |
| flat.json | ujson | 0.986 | 0.998 | 1.010 | 58.680 | 0.24x |
| flat.json | json | 1.686 | 1.698 | 1.716 | 58.680 | 0.14x |
| nested.json | strata | 0.230 | 0.238 | 0.254 | 58.680 | 1.00x |
| nested.json | orjson | 0.285 | 0.286 | 0.313 | 58.680 | 0.83x |
| nested.json | msgspec | 0.369 | 0.373 | 0.394 | 58.680 | 0.64x |
| nested.json | ujson | 1.109 | 1.127 | 1.165 | 58.680 | 0.21x |
| nested.json | json | 2.148 | 2.161 | 2.177 | 58.680 | 0.11x |
| wide_arrays.json | strata | 1.277 | 1.281 | 1.290 | 60.246 | 1.00x |
| wide_arrays.json | orjson | 1.548 | 1.563 | 1.586 | 60.246 | 0.82x |
| wide_arrays.json | msgspec | 2.310 | 2.324 | 2.338 | 60.246 | 0.55x |
| wide_arrays.json | ujson | 4.713 | 4.729 | 4.763 | 60.246 | 0.27x |
| wide_arrays.json | json | 13.475 | 13.513 | 13.548 | 60.246 | 0.09x |
| mixed.json | strata | 0.068 | 0.072 | 0.102 | 60.246 | 1.00x |
| mixed.json | orjson | 0.065 | 0.067 | 0.069 | 60.246 | 1.08x |
| mixed.json | msgspec | 0.079 | 0.081 | 0.097 | 60.246 | 0.90x |
| mixed.json | ujson | 0.240 | 0.243 | 0.262 | 60.246 | 0.30x |
| mixed.json | json | 0.487 | 0.501 | 0.514 | 60.246 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.254 | 9.390 | 10.140 | 63.078 | 1.00x |
| users.json | orjson | 11.969 | 12.145 | 12.461 | 63.078 | 0.77x |
| users.json | msgspec | 12.506 | 12.610 | 12.929 | 63.078 | 0.74x |
| users.json | ujson | 17.168 | 17.576 | 18.432 | 63.078 | 0.53x |
| users.json | json | 21.121 | 21.335 | 21.423 | 63.078 | 0.44x |
| flat.json | strata | 0.862 | 0.874 | 0.882 | 58.680 | 1.00x |
| flat.json | orjson | 0.934 | 0.946 | 0.957 | 58.680 | 0.92x |
| flat.json | msgspec | 0.975 | 0.982 | 0.989 | 58.680 | 0.89x |
| flat.json | ujson | 1.515 | 1.529 | 1.545 | 58.680 | 0.57x |
| flat.json | json | 1.816 | 1.831 | 1.844 | 58.680 | 0.48x |
| nested.json | strata | 0.867 | 0.875 | 0.885 | 58.680 | 1.00x |
| nested.json | orjson | 0.955 | 0.964 | 0.981 | 58.680 | 0.91x |
| nested.json | msgspec | 1.068 | 1.079 | 1.110 | 58.680 | 0.81x |
| nested.json | ujson | 1.489 | 1.499 | 1.539 | 58.680 | 0.58x |
| nested.json | json | 2.042 | 2.054 | 2.075 | 58.680 | 0.43x |
| wide_arrays.json | strata | 4.474 | 4.521 | 4.559 | 60.246 | 1.00x |
| wide_arrays.json | orjson | 4.069 | 4.118 | 4.162 | 60.246 | 1.10x |
| wide_arrays.json | msgspec | 5.111 | 5.165 | 5.231 | 60.246 | 0.88x |
| wide_arrays.json | ujson | 6.642 | 6.728 | 6.787 | 60.246 | 0.67x |
| wide_arrays.json | json | 9.569 | 9.615 | 9.698 | 60.246 | 0.47x |
| mixed.json | strata | 0.220 | 0.223 | 0.245 | 60.246 | 1.00x |
| mixed.json | orjson | 0.277 | 0.283 | 0.305 | 60.246 | 0.79x |
| mixed.json | msgspec | 0.291 | 0.298 | 0.328 | 60.246 | 0.75x |
| mixed.json | ujson | 0.372 | 0.379 | 0.408 | 60.246 | 0.59x |
| mixed.json | json | 0.505 | 0.517 | 0.549 | 60.246 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.353 | 9.589 | 9.775 | 58.672 | 1.00x |
| users.ndjson | orjson | 14.652 | 14.867 | 15.117 | 58.672 | 0.65x |
| users.ndjson | msgspec | 14.966 | 15.154 | 15.521 | 58.672 | 0.63x |
| users.ndjson | ujson | 19.459 | 19.747 | 20.218 | 58.672 | 0.49x |
| users.ndjson | json | 26.073 | 26.337 | 27.001 | 58.672 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.592 | 2.636 | 2.682 | 59.078 | 1.00x |
| users.json | orjson | 3.134 | 3.171 | 3.221 | 59.078 | 0.83x |
| users.json | msgspec | 3.859 | 3.911 | 3.949 | 59.078 | 0.67x |
| users.json | ujson | 11.223 | 11.395 | 11.672 | 59.078 | 0.23x |
| users.json | json | 19.770 | 19.977 | 20.188 | 59.078 | 0.13x |
| flat.json | strata | 0.388 | 0.420 | 0.437 | 58.680 | 1.00x |
| flat.json | orjson | 0.494 | 0.508 | 0.800 | 58.680 | 0.83x |
| flat.json | msgspec | 0.582 | 0.607 | 0.627 | 58.680 | 0.69x |
| flat.json | ujson | 1.204 | 1.231 | 1.268 | 58.680 | 0.34x |
| flat.json | json | 1.922 | 1.939 | 1.951 | 58.680 | 0.22x |
| nested.json | strata | 0.373 | 0.386 | 0.407 | 58.680 | 1.00x |
| nested.json | orjson | 0.455 | 0.476 | 0.501 | 58.680 | 0.81x |
| nested.json | msgspec | 0.536 | 0.561 | 0.581 | 58.680 | 0.69x |
| nested.json | ujson | 1.289 | 1.305 | 1.349 | 58.680 | 0.30x |
| nested.json | json | 2.340 | 2.361 | 2.399 | 58.680 | 0.16x |
| wide_arrays.json | strata | 1.626 | 1.683 | 1.762 | 60.246 | 1.00x |
| wide_arrays.json | orjson | 1.960 | 1.991 | 2.037 | 60.246 | 0.85x |
| wide_arrays.json | msgspec | 2.717 | 2.746 | 2.790 | 60.246 | 0.61x |
| wide_arrays.json | ujson | 5.183 | 5.214 | 5.260 | 60.246 | 0.32x |
| wide_arrays.json | json | 13.878 | 13.997 | 14.075 | 60.246 | 0.12x |
| mixed.json | strata | 0.173 | 0.195 | 0.206 | 60.246 | 1.00x |
| mixed.json | orjson | 0.191 | 0.207 | 0.231 | 60.246 | 0.94x |
| mixed.json | msgspec | 0.209 | 0.225 | 0.256 | 60.246 | 0.87x |
| mixed.json | ujson | 0.392 | 0.409 | 0.429 | 60.246 | 0.48x |
| mixed.json | json | 0.638 | 0.663 | 0.677 | 60.246 | 0.29x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.104 | 0.108 | 0.116 | 59.078 | 1.00x |
| users.json $[*].id | jmespath | 0.471 | 0.484 | 0.492 | 59.078 | 0.22x |
| users.json $[*].id | jsonpath-ng | 2.469 | 2.555 | 2.602 | 59.078 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.628 | 0.646 | 0.671 | 59.242 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.979 | 3.023 | 3.052 | 59.242 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.887 | 18.022 | 18.556 | 59.242 | 0.04x |
| users.json $..total | strata | 1.721 | 1.728 | 1.744 | 63.246 | 1.00x |
| users.json $..total | jsonpath-ng | 294.325 | 294.582 | 295.581 | 63.246 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.365 | 3.376 | 3.407 | 59.242 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.447 | 12.552 | 12.739 | 59.242 | 0.27x |
| users.json $[*].id | orjson+jsonpath-ng | 14.247 | 14.400 | 14.483 | 59.242 | 0.23x |
| users.json $[*].orders[*].total | strata | 3.543 | 3.554 | 3.576 | 63.246 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.237 | 15.384 | 15.510 | 63.246 | 0.23x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 33.655 | 33.940 | 34.592 | 63.246 | 0.10x |
| users.json $..total | strata | 11.556 | 11.669 | 11.873 | 62.211 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 312.483 | 314.454 | 315.022 | 62.211 | 0.04x |

