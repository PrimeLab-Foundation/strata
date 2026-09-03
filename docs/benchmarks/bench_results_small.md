# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 1fb880b
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
| users.json | strata | 6.775 | 6.826 | 7.328 | 51.484 | 1.00x |
| users.json | orjson | 7.879 | 7.961 | 8.359 | 51.484 | 0.86x |
| users.json | msgspec | 7.999 | 8.055 | 8.473 | 51.484 | 0.85x |
| users.json | ujson | 10.799 | 10.968 | 11.822 | 51.484 | 0.62x |
| users.json | json | 15.044 | 15.093 | 15.629 | 51.484 | 0.45x |
| flat.json | strata | 0.609 | 0.612 | 0.647 | 68.094 | 1.00x |
| flat.json | orjson | 0.641 | 0.644 | 0.690 | 68.094 | 0.95x |
| flat.json | msgspec | 0.677 | 0.687 | 0.738 | 68.094 | 0.89x |
| flat.json | ujson | 0.995 | 1.002 | 1.080 | 68.094 | 0.61x |
| flat.json | json | 1.389 | 1.399 | 1.528 | 68.094 | 0.44x |
| nested.json | strata | 0.534 | 0.539 | 0.545 | 68.109 | 1.00x |
| nested.json | orjson | 0.606 | 0.610 | 0.619 | 68.109 | 0.88x |
| nested.json | msgspec | 0.609 | 0.615 | 0.649 | 68.109 | 0.88x |
| nested.json | ujson | 0.888 | 0.892 | 0.925 | 68.109 | 0.60x |
| nested.json | json | 1.393 | 1.404 | 1.418 | 68.109 | 0.38x |
| wide_arrays.json | strata | 3.133 | 3.174 | 3.318 | 68.188 | 1.00x |
| wide_arrays.json | orjson | 3.191 | 3.263 | 3.541 | 68.188 | 0.97x |
| wide_arrays.json | msgspec | 3.917 | 4.027 | 4.537 | 68.188 | 0.79x |
| wide_arrays.json | ujson | 5.136 | 5.185 | 5.313 | 68.188 | 0.61x |
| wide_arrays.json | json | 7.569 | 7.706 | 8.100 | 68.188 | 0.41x |
| mixed.json | strata | 0.132 | 0.136 | 0.142 | 68.188 | 1.00x |
| mixed.json | orjson | 0.130 | 0.132 | 0.143 | 68.188 | 1.03x |
| mixed.json | msgspec | 0.143 | 0.145 | 0.149 | 68.188 | 0.94x |
| mixed.json | ujson | 0.185 | 0.187 | 0.205 | 68.188 | 0.73x |
| mixed.json | json | 0.307 | 0.313 | 0.328 | 68.188 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.431 | 1.434 | 1.542 | 55.312 | 1.00x |
| users.json | orjson | 1.990 | 1.997 | 2.112 | 55.312 | 0.72x |
| users.json | msgspec | 2.621 | 2.653 | 2.882 | 55.312 | 0.54x |
| users.json | ujson | 10.774 | 10.813 | 11.439 | 55.312 | 0.13x |
| users.json | json | 15.461 | 15.584 | 16.407 | 55.312 | 0.09x |
| flat.json | strata | 0.177 | 0.177 | 0.180 | 68.109 | 1.00x |
| flat.json | orjson | 0.222 | 0.223 | 0.226 | 68.109 | 0.79x |
| flat.json | msgspec | 0.291 | 0.292 | 0.296 | 68.109 | 0.61x |
| flat.json | ujson | 0.896 | 0.899 | 0.906 | 68.109 | 0.20x |
| flat.json | json | 1.321 | 1.324 | 1.335 | 68.109 | 0.13x |
| nested.json | strata | 0.133 | 0.136 | 0.149 | 68.188 | 1.00x |
| nested.json | orjson | 0.201 | 0.204 | 0.207 | 68.188 | 0.67x |
| nested.json | msgspec | 0.267 | 0.269 | 0.278 | 68.188 | 0.50x |
| nested.json | ujson | 0.964 | 0.970 | 0.979 | 68.188 | 0.14x |
| nested.json | json | 1.622 | 1.641 | 1.689 | 68.188 | 0.08x |
| wide_arrays.json | strata | 0.912 | 0.917 | 0.969 | 68.188 | 1.00x |
| wide_arrays.json | orjson | 1.120 | 1.125 | 1.155 | 68.188 | 0.81x |
| wide_arrays.json | msgspec | 1.936 | 1.940 | 1.969 | 68.188 | 0.47x |
| wide_arrays.json | ujson | 5.947 | 5.979 | 6.061 | 68.188 | 0.15x |
| wide_arrays.json | json | 11.019 | 11.131 | 11.665 | 68.188 | 0.08x |
| mixed.json | strata | 0.035 | 0.037 | 0.040 | 68.234 | 1.00x |
| mixed.json | orjson | 0.042 | 0.046 | 0.050 | 68.234 | 0.80x |
| mixed.json | msgspec | 0.048 | 0.051 | 0.062 | 68.234 | 0.73x |
| mixed.json | ujson | 0.215 | 0.220 | 0.223 | 68.234 | 0.17x |
| mixed.json | json | 0.351 | 0.360 | 0.370 | 68.234 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.096 | 7.140 | 7.933 | 68.766 | 1.00x |
| users.json | orjson | 8.302 | 8.348 | 8.463 | 68.766 | 0.86x |
| users.json | msgspec | 8.362 | 8.430 | 8.527 | 68.766 | 0.85x |
| users.json | ujson | 11.298 | 11.525 | 11.927 | 68.766 | 0.62x |
| users.json | json | 15.379 | 15.522 | 15.998 | 68.766 | 0.46x |
| flat.json | strata | 0.665 | 0.675 | 0.751 | 68.109 | 1.00x |
| flat.json | orjson | 0.714 | 0.723 | 0.780 | 68.109 | 0.93x |
| flat.json | msgspec | 0.751 | 0.757 | 0.775 | 68.109 | 0.89x |
| flat.json | ujson | 1.096 | 1.119 | 1.170 | 68.109 | 0.60x |
| flat.json | json | 1.464 | 1.474 | 1.506 | 68.109 | 0.46x |
| nested.json | strata | 0.596 | 0.601 | 0.607 | 68.188 | 1.00x |
| nested.json | orjson | 0.670 | 0.679 | 0.685 | 68.188 | 0.88x |
| nested.json | msgspec | 0.670 | 0.678 | 0.699 | 68.188 | 0.89x |
| nested.json | ujson | 0.965 | 0.986 | 1.003 | 68.188 | 0.61x |
| nested.json | json | 1.462 | 1.474 | 1.494 | 68.188 | 0.41x |
| wide_arrays.json | strata | 3.296 | 3.318 | 3.369 | 68.188 | 1.00x |
| wide_arrays.json | orjson | 3.372 | 3.426 | 3.475 | 68.188 | 0.97x |
| wide_arrays.json | msgspec | 4.138 | 4.220 | 4.304 | 68.188 | 0.79x |
| wide_arrays.json | ujson | 5.464 | 5.521 | 5.554 | 68.188 | 0.60x |
| wide_arrays.json | json | 7.756 | 7.840 | 8.063 | 68.188 | 0.42x |
| mixed.json | strata | 0.171 | 0.173 | 0.183 | 68.234 | 1.00x |
| mixed.json | orjson | 0.171 | 0.177 | 0.203 | 68.234 | 0.98x |
| mixed.json | msgspec | 0.188 | 0.193 | 0.219 | 68.234 | 0.90x |
| mixed.json | ujson | 0.237 | 0.244 | 0.252 | 68.234 | 0.71x |
| mixed.json | json | 0.346 | 0.352 | 0.362 | 68.234 | 0.49x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.347 | 7.393 | 7.509 | 67.234 | 1.00x |
| users.ndjson | orjson | 10.109 | 10.332 | 10.656 | 67.234 | 0.72x |
| users.ndjson | msgspec | 9.967 | 10.170 | 10.284 | 67.234 | 0.73x |
| users.ndjson | ujson | 12.801 | 12.978 | 13.342 | 67.234 | 0.57x |
| users.ndjson | json | 17.961 | 18.203 | 18.884 | 67.234 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.783 | 1.937 | 2.050 | 66.797 | 1.00x |
| users.json | orjson | 2.428 | 2.514 | 2.803 | 66.797 | 0.77x |
| users.json | msgspec | 3.070 | 3.133 | 3.256 | 66.797 | 0.62x |
| users.json | ujson | 10.973 | 11.119 | 11.210 | 66.797 | 0.17x |
| users.json | json | 16.046 | 16.081 | 16.771 | 66.797 | 0.12x |
| flat.json | strata | 0.323 | 0.345 | 0.372 | 68.109 | 1.00x |
| flat.json | orjson | 0.372 | 0.399 | 0.417 | 68.109 | 0.87x |
| flat.json | msgspec | 0.442 | 0.466 | 0.494 | 68.109 | 0.74x |
| flat.json | ujson | 1.055 | 1.072 | 1.129 | 68.109 | 0.32x |
| flat.json | json | 1.480 | 1.511 | 1.558 | 68.109 | 0.23x |
| nested.json | strata | 0.287 | 0.315 | 0.408 | 68.188 | 1.00x |
| nested.json | orjson | 0.362 | 0.382 | 0.434 | 68.188 | 0.82x |
| nested.json | msgspec | 0.428 | 0.435 | 0.463 | 68.188 | 0.72x |
| nested.json | ujson | 1.140 | 1.161 | 4.465 | 68.188 | 0.27x |
| nested.json | json | 1.826 | 1.860 | 4.772 | 68.188 | 0.17x |
| wide_arrays.json | strata | 1.217 | 1.302 | 1.347 | 68.188 | 1.00x |
| wide_arrays.json | orjson | 1.461 | 1.531 | 1.570 | 68.188 | 0.85x |
| wide_arrays.json | msgspec | 2.302 | 2.348 | 2.464 | 68.188 | 0.55x |
| wide_arrays.json | ujson | 6.350 | 6.419 | 6.509 | 68.188 | 0.20x |
| wide_arrays.json | json | 11.498 | 11.584 | 11.713 | 68.188 | 0.11x |
| mixed.json | strata | 0.158 | 0.170 | 0.187 | 68.266 | 1.00x |
| mixed.json | orjson | 0.163 | 0.189 | 0.262 | 68.266 | 0.90x |
| mixed.json | msgspec | 0.167 | 0.191 | 0.210 | 68.266 | 0.89x |
| mixed.json | ujson | 0.341 | 0.360 | 0.380 | 68.266 | 0.47x |
| mixed.json | json | 0.486 | 0.498 | 0.511 | 68.266 | 0.34x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.050 | 0.052 | 0.067 | 67.000 | 1.00x |
| users.json $[*].id | jmespath | 0.275 | 0.278 | 0.285 | 67.000 | 0.19x |
| users.json $[*].id | jsonpath-ng | 1.413 | 1.428 | 1.447 | 67.000 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.331 | 0.341 | 0.351 | 67.203 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.669 | 1.678 | 1.747 | 67.203 | 0.20x |
| users.json $[*].orders[*].total | jsonpath-ng | 9.488 | 9.529 | 9.586 | 67.203 | 0.04x |
| users.json $..total | strata | 1.391 | 1.397 | 1.435 | 67.375 | 1.00x |
| users.json $..total | jsonpath-ng | 184.218 | 184.703 | 186.604 | 67.375 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.377 | 3.427 | 3.580 | 67.062 | 1.00x |
| users.json $[*].id | orjson+jmespath | 8.470 | 8.612 | 8.652 | 67.062 | 0.40x |
| users.json $[*].id | orjson+jsonpath-ng | 9.798 | 9.847 | 10.458 | 67.062 | 0.35x |
| users.json $[*].orders[*].total | strata | 3.454 | 3.480 | 3.609 | 67.312 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 10.013 | 10.134 | 10.291 | 67.312 | 0.34x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 19.163 | 19.412 | 19.909 | 67.312 | 0.18x |
| users.json $..total | strata | 8.561 | 8.670 | 8.913 | 67.391 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 193.764 | 194.903 | 199.864 | 67.391 | 0.04x |

