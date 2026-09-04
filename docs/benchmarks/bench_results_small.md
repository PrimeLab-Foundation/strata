# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: f3ddc4c
- python: 3.14.7
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit-Mach-O
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
| users.json | strata | 6.470 | 6.740 | 7.281 | 52.656 | 1.00x |
| users.json | orjson | 8.019 | 8.446 | 8.937 | 52.656 | 0.80x |
| users.json | msgspec | 8.317 | 8.526 | 8.966 | 52.656 | 0.79x |
| users.json | ujson | 11.490 | 11.962 | 13.089 | 52.656 | 0.56x |
| users.json | json | 15.464 | 15.941 | 16.487 | 52.656 | 0.42x |
| flat.json | strata | 0.624 | 0.667 | 0.717 | 74.312 | 1.00x |
| flat.json | orjson | 0.663 | 0.690 | 0.735 | 74.312 | 0.97x |
| flat.json | msgspec | 0.680 | 0.705 | 0.893 | 74.312 | 0.95x |
| flat.json | ujson | 1.062 | 1.131 | 1.251 | 74.312 | 0.59x |
| flat.json | json | 1.439 | 1.483 | 1.565 | 74.312 | 0.45x |
| nested.json | strata | 0.534 | 0.558 | 0.589 | 74.406 | 1.00x |
| nested.json | orjson | 0.601 | 0.631 | 0.708 | 74.406 | 0.88x |
| nested.json | msgspec | 0.607 | 0.621 | 0.705 | 74.406 | 0.90x |
| nested.json | ujson | 0.923 | 0.971 | 1.064 | 74.406 | 0.57x |
| nested.json | json | 1.422 | 1.468 | 1.710 | 74.406 | 0.38x |
| wide_arrays.json | strata | 3.000 | 3.044 | 3.499 | 74.453 | 1.00x |
| wide_arrays.json | orjson | 3.191 | 3.342 | 3.470 | 74.453 | 0.91x |
| wide_arrays.json | msgspec | 3.907 | 3.978 | 4.397 | 74.453 | 0.77x |
| wide_arrays.json | ujson | 5.160 | 5.536 | 5.880 | 74.453 | 0.55x |
| wide_arrays.json | json | 7.556 | 7.630 | 8.072 | 74.453 | 0.40x |
| mixed.json | strata | 0.127 | 0.131 | 0.159 | 74.453 | 1.00x |
| mixed.json | orjson | 0.129 | 0.133 | 0.151 | 74.453 | 0.98x |
| mixed.json | msgspec | 0.146 | 0.157 | 0.172 | 74.453 | 0.83x |
| mixed.json | ujson | 0.188 | 0.195 | 0.237 | 74.453 | 0.67x |
| mixed.json | json | 0.306 | 0.337 | 0.416 | 74.453 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.434 | 1.511 | 1.683 | 57.453 | 1.00x |
| users.json | orjson | 1.995 | 2.082 | 2.181 | 57.453 | 0.73x |
| users.json | msgspec | 2.624 | 2.718 | 2.852 | 57.453 | 0.56x |
| users.json | ujson | 11.040 | 11.225 | 11.401 | 57.453 | 0.13x |
| users.json | json | 15.836 | 16.293 | 16.618 | 57.453 | 0.09x |
| flat.json | strata | 0.180 | 0.185 | 0.197 | 74.312 | 1.00x |
| flat.json | orjson | 0.229 | 0.240 | 0.261 | 74.312 | 0.77x |
| flat.json | msgspec | 0.297 | 0.317 | 0.341 | 74.312 | 0.58x |
| flat.json | ujson | 0.893 | 0.904 | 0.981 | 74.312 | 0.20x |
| flat.json | json | 1.359 | 1.509 | 1.587 | 74.312 | 0.12x |
| nested.json | strata | 0.133 | 0.143 | 0.173 | 74.453 | 1.00x |
| nested.json | orjson | 0.202 | 0.218 | 0.268 | 74.453 | 0.66x |
| nested.json | msgspec | 0.266 | 0.274 | 0.305 | 74.453 | 0.52x |
| nested.json | ujson | 0.956 | 0.972 | 1.099 | 74.453 | 0.15x |
| nested.json | json | 1.660 | 1.857 | 2.060 | 74.453 | 0.08x |
| wide_arrays.json | strata | 0.917 | 0.945 | 1.075 | 74.453 | 1.00x |
| wide_arrays.json | orjson | 1.105 | 1.121 | 1.154 | 74.453 | 0.84x |
| wide_arrays.json | msgspec | 1.922 | 1.968 | 2.143 | 74.453 | 0.48x |
| wide_arrays.json | ujson | 5.980 | 6.098 | 6.374 | 74.453 | 0.16x |
| wide_arrays.json | json | 11.091 | 11.873 | 12.090 | 74.453 | 0.08x |
| mixed.json | strata | 0.036 | 0.038 | 0.043 | 74.453 | 1.00x |
| mixed.json | orjson | 0.043 | 0.048 | 0.063 | 74.453 | 0.79x |
| mixed.json | msgspec | 0.048 | 0.052 | 0.058 | 74.453 | 0.73x |
| mixed.json | ujson | 0.219 | 0.229 | 0.294 | 74.453 | 0.17x |
| mixed.json | json | 0.363 | 0.427 | 0.452 | 74.453 | 0.09x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.928 | 7.047 | 7.269 | 72.375 | 1.00x |
| users.json | orjson | 8.407 | 8.612 | 8.864 | 72.375 | 0.82x |
| users.json | msgspec | 8.677 | 8.831 | 9.074 | 72.375 | 0.80x |
| users.json | ujson | 12.109 | 12.408 | 12.685 | 72.375 | 0.57x |
| users.json | json | 15.872 | 16.101 | 16.981 | 72.375 | 0.44x |
| flat.json | strata | 0.712 | 0.754 | 0.798 | 74.312 | 1.00x |
| flat.json | orjson | 0.766 | 0.826 | 1.002 | 74.312 | 0.91x |
| flat.json | msgspec | 0.765 | 0.815 | 0.952 | 74.312 | 0.92x |
| flat.json | ujson | 1.163 | 1.198 | 1.357 | 74.312 | 0.63x |
| flat.json | json | 1.499 | 1.609 | 1.777 | 74.312 | 0.47x |
| nested.json | strata | 0.622 | 0.669 | 0.732 | 74.453 | 1.00x |
| nested.json | orjson | 0.692 | 0.741 | 0.960 | 74.453 | 0.90x |
| nested.json | msgspec | 0.686 | 0.758 | 0.871 | 74.453 | 0.88x |
| nested.json | ujson | 1.043 | 1.119 | 1.243 | 74.453 | 0.60x |
| nested.json | json | 1.507 | 1.577 | 1.764 | 74.453 | 0.42x |
| wide_arrays.json | strata | 3.224 | 3.313 | 3.440 | 74.453 | 1.00x |
| wide_arrays.json | orjson | 3.354 | 3.523 | 3.806 | 74.453 | 0.94x |
| wide_arrays.json | msgspec | 4.164 | 4.519 | 4.669 | 74.453 | 0.73x |
| wide_arrays.json | ujson | 5.715 | 5.939 | 6.212 | 74.453 | 0.56x |
| wide_arrays.json | json | 7.919 | 8.152 | 8.319 | 74.453 | 0.41x |
| mixed.json | strata | 0.160 | 0.201 | 0.241 | 74.453 | 1.00x |
| mixed.json | orjson | 0.175 | 0.195 | 0.241 | 74.453 | 1.03x |
| mixed.json | msgspec | 0.186 | 0.218 | 0.282 | 74.453 | 0.92x |
| mixed.json | ujson | 0.242 | 0.262 | 0.287 | 74.453 | 0.77x |
| mixed.json | json | 0.350 | 0.371 | 0.393 | 74.453 | 0.54x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.940 | 7.210 | 7.510 | 73.453 | 1.00x |
| users.ndjson | orjson | 10.457 | 10.892 | 11.143 | 73.453 | 0.66x |
| users.ndjson | msgspec | 10.422 | 10.771 | 11.575 | 73.453 | 0.67x |
| users.ndjson | ujson | 13.850 | 14.341 | 14.804 | 73.453 | 0.50x |
| users.ndjson | json | 18.774 | 19.315 | 20.287 | 73.453 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.835 | 1.934 | 2.069 | 70.969 | 1.00x |
| users.json | orjson | 2.477 | 2.567 | 2.709 | 70.969 | 0.75x |
| users.json | msgspec | 3.116 | 3.236 | 3.413 | 70.969 | 0.60x |
| users.json | ujson | 11.247 | 11.579 | 11.977 | 70.969 | 0.17x |
| users.json | json | 16.494 | 16.939 | 17.437 | 70.969 | 0.11x |
| flat.json | strata | 0.317 | 0.362 | 0.425 | 74.375 | 1.00x |
| flat.json | orjson | 0.362 | 0.413 | 0.484 | 74.375 | 0.88x |
| flat.json | msgspec | 0.443 | 0.481 | 0.544 | 74.375 | 0.75x |
| flat.json | ujson | 1.054 | 1.099 | 1.186 | 74.375 | 0.33x |
| flat.json | json | 1.495 | 1.660 | 1.749 | 74.375 | 0.22x |
| nested.json | strata | 0.293 | 0.316 | 0.405 | 74.453 | 1.00x |
| nested.json | orjson | 0.356 | 0.411 | 0.471 | 74.453 | 0.77x |
| nested.json | msgspec | 0.435 | 0.462 | 0.541 | 74.453 | 0.68x |
| nested.json | ujson | 1.133 | 1.185 | 1.356 | 74.453 | 0.27x |
| nested.json | json | 1.861 | 2.107 | 2.196 | 74.453 | 0.15x |
| wide_arrays.json | strata | 1.237 | 1.314 | 1.649 | 74.453 | 1.00x |
| wide_arrays.json | orjson | 1.438 | 1.484 | 1.655 | 74.453 | 0.89x |
| wide_arrays.json | msgspec | 2.221 | 2.324 | 2.610 | 74.453 | 0.57x |
| wide_arrays.json | ujson | 6.358 | 6.622 | 6.997 | 74.453 | 0.20x |
| wide_arrays.json | json | 11.699 | 11.997 | 12.639 | 74.453 | 0.11x |
| mixed.json | strata | 0.168 | 0.190 | 0.245 | 74.484 | 1.00x |
| mixed.json | orjson | 0.140 | 0.182 | 0.246 | 74.484 | 1.04x |
| mixed.json | msgspec | 0.175 | 0.205 | 0.221 | 74.484 | 0.93x |
| mixed.json | ujson | 0.348 | 0.381 | 0.429 | 74.484 | 0.50x |
| mixed.json | json | 0.570 | 0.634 | 0.707 | 74.484 | 0.30x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.056 | 0.077 | 0.142 | 71.234 | 1.00x |
| users.json $[*].id | jmespath | 0.282 | 0.314 | 0.365 | 71.234 | 0.24x |
| users.json $[*].id | jsonpath-ng | 1.469 | 1.551 | 1.743 | 71.234 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.334 | 0.404 | 0.489 | 71.406 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.658 | 1.896 | 2.020 | 71.406 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 10.479 | 10.794 | 11.172 | 71.406 | 0.04x |
| users.json $..total | strata | 1.390 | 1.470 | 1.589 | 72.625 | 1.00x |
| users.json $..total | jsonpath-ng | 189.379 | 190.518 | 194.363 | 72.625 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.497 | 3.569 | 3.741 | 71.266 | 1.00x |
| users.json $[*].id | orjson+jmespath | 8.707 | 8.783 | 9.206 | 71.266 | 0.41x |
| users.json $[*].id | orjson+jsonpath-ng | 10.020 | 10.379 | 10.660 | 71.266 | 0.34x |
| users.json $[*].orders[*].total | strata | 3.522 | 3.664 | 3.756 | 72.578 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 10.010 | 10.624 | 11.120 | 72.578 | 0.34x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 19.579 | 20.808 | 21.566 | 72.578 | 0.18x |
| users.json $..total | strata | 8.246 | 8.520 | 8.671 | 73.531 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 199.416 | 201.599 | 203.295 | 73.531 | 0.04x |

