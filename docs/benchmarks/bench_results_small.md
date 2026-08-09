# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: bcff500
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
| users.json | strata | 12.519 | 12.650 | 13.110 | 51.062 | 1.00x |
| users.json | orjson | 10.721 | 11.036 | 11.346 | 51.062 | 1.15x |
| users.json | msgspec | 10.902 | 10.973 | 11.958 | 51.062 | 1.15x |
| users.json | ujson | 14.380 | 14.597 | 15.475 | 51.062 | 0.87x |
| users.json | json | 19.443 | 19.575 | 20.628 | 51.062 | 0.65x |
| flat.json | strata | 1.290 | 1.304 | 1.367 | 69.250 | 1.00x |
| flat.json | orjson | 0.834 | 0.842 | 0.882 | 69.250 | 1.55x |
| flat.json | msgspec | 0.890 | 0.901 | 0.972 | 69.250 | 1.45x |
| flat.json | ujson | 1.320 | 1.328 | 1.455 | 69.250 | 0.98x |
| flat.json | json | 1.827 | 1.839 | 1.886 | 69.250 | 0.71x |
| nested.json | strata | 1.046 | 1.053 | 1.153 | 69.312 | 1.00x |
| nested.json | orjson | 1.532 | 1.573 | 1.743 | 69.312 | 0.67x |
| nested.json | msgspec | 1.541 | 1.603 | 1.810 | 69.312 | 0.66x |
| nested.json | ujson | 1.890 | 1.926 | 2.050 | 69.312 | 0.55x |
| nested.json | json | 2.479 | 2.517 | 2.664 | 69.312 | 0.42x |
| wide_arrays.json | strata | 6.301 | 6.319 | 6.408 | 69.328 | 1.00x |
| wide_arrays.json | orjson | 4.860 | 4.904 | 5.275 | 69.328 | 1.29x |
| wide_arrays.json | msgspec | 5.808 | 5.880 | 6.389 | 69.328 | 1.07x |
| wide_arrays.json | ujson | 7.353 | 7.442 | 8.120 | 69.328 | 0.85x |
| wide_arrays.json | json | 10.307 | 10.434 | 11.221 | 69.328 | 0.61x |
| mixed.json | strata | 0.254 | 0.261 | 0.296 | 64.078 | 1.00x |
| mixed.json | orjson | 0.164 | 0.172 | 0.395 | 64.078 | 1.52x |
| mixed.json | msgspec | 0.179 | 0.185 | 0.222 | 64.078 | 1.41x |
| mixed.json | ujson | 0.237 | 0.243 | 0.253 | 64.078 | 1.08x |
| mixed.json | json | 0.388 | 0.396 | 0.467 | 64.078 | 0.66x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.276 | 6.351 | 6.429 | 58.578 | 1.00x |
| users.json | orjson | 2.565 | 2.608 | 2.701 | 58.578 | 2.44x |
| users.json | msgspec | 3.387 | 3.406 | 3.494 | 58.578 | 1.86x |
| users.json | ujson | 13.877 | 13.969 | 14.124 | 58.578 | 0.45x |
| users.json | json | 19.502 | 19.688 | 20.264 | 58.578 | 0.32x |
| flat.json | strata | 0.541 | 0.555 | 0.593 | 69.297 | 1.00x |
| flat.json | orjson | 0.291 | 0.300 | 0.321 | 69.297 | 1.85x |
| flat.json | msgspec | 0.383 | 0.384 | 0.461 | 69.297 | 1.44x |
| flat.json | ujson | 1.130 | 1.139 | 1.155 | 69.297 | 0.49x |
| flat.json | json | 1.627 | 1.651 | 1.689 | 69.297 | 0.34x |
| nested.json | strata | 0.500 | 0.507 | 0.518 | 69.312 | 1.00x |
| nested.json | orjson | 0.261 | 0.266 | 0.283 | 69.312 | 1.91x |
| nested.json | msgspec | 0.349 | 0.358 | 0.378 | 69.312 | 1.42x |
| nested.json | ujson | 1.309 | 1.323 | 1.368 | 69.312 | 0.38x |
| nested.json | json | 2.108 | 2.161 | 2.260 | 69.312 | 0.23x |
| wide_arrays.json | strata | 3.387 | 3.405 | 3.517 | 69.328 | 1.00x |
| wide_arrays.json | orjson | 1.453 | 1.469 | 1.591 | 69.328 | 2.32x |
| wide_arrays.json | msgspec | 2.532 | 2.553 | 2.614 | 69.328 | 1.33x |
| wide_arrays.json | ujson | 7.889 | 7.930 | 8.180 | 69.328 | 0.43x |
| wide_arrays.json | json | 13.958 | 14.118 | 14.401 | 69.328 | 0.24x |
| mixed.json | strata | 0.137 | 0.138 | 0.149 | 64.078 | 1.00x |
| mixed.json | orjson | 0.057 | 0.059 | 0.064 | 64.078 | 2.36x |
| mixed.json | msgspec | 0.064 | 0.068 | 0.082 | 64.078 | 2.05x |
| mixed.json | ujson | 0.277 | 0.279 | 0.289 | 64.078 | 0.50x |
| mixed.json | json | 0.436 | 0.443 | 0.452 | 64.078 | 0.31x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 12.922 | 13.063 | 13.358 | 72.750 | 1.00x |
| users.json | orjson | 13.064 | 13.335 | 15.038 | 72.750 | 0.98x |
| users.json | msgspec | 13.269 | 13.392 | 14.051 | 72.750 | 0.98x |
| users.json | ujson | 17.050 | 17.330 | 17.594 | 72.750 | 0.75x |
| users.json | json | 21.922 | 22.652 | 22.889 | 72.750 | 0.58x |
| flat.json | strata | 1.368 | 1.406 | 1.511 | 69.297 | 1.00x |
| flat.json | orjson | 0.913 | 0.939 | 0.995 | 69.297 | 1.50x |
| flat.json | msgspec | 0.973 | 0.988 | 1.036 | 69.297 | 1.42x |
| flat.json | ujson | 1.447 | 1.482 | 1.683 | 69.297 | 0.95x |
| flat.json | json | 1.906 | 1.932 | 2.087 | 69.297 | 0.73x |
| nested.json | strata | 1.121 | 1.157 | 1.318 | 69.312 | 1.00x |
| nested.json | orjson | 1.748 | 1.810 | 1.950 | 69.312 | 0.64x |
| nested.json | msgspec | 1.747 | 1.786 | 1.913 | 69.312 | 0.65x |
| nested.json | ujson | 2.124 | 2.160 | 2.278 | 69.312 | 0.54x |
| nested.json | json | 2.682 | 2.753 | 2.950 | 69.312 | 0.42x |
| wide_arrays.json | strata | 6.546 | 6.612 | 6.672 | 69.328 | 1.00x |
| wide_arrays.json | orjson | 5.422 | 5.504 | 5.710 | 69.328 | 1.20x |
| wide_arrays.json | msgspec | 6.447 | 6.577 | 6.890 | 69.328 | 1.01x |
| wide_arrays.json | ujson | 8.162 | 8.259 | 8.953 | 69.328 | 0.80x |
| wide_arrays.json | json | 10.985 | 11.034 | 11.745 | 69.328 | 0.60x |
| mixed.json | strata | 0.301 | 0.311 | 0.330 | 64.078 | 1.00x |
| mixed.json | orjson | 0.219 | 0.235 | 0.255 | 64.078 | 1.32x |
| mixed.json | msgspec | 0.236 | 0.258 | 0.274 | 64.078 | 1.20x |
| mixed.json | ujson | 0.317 | 0.324 | 0.438 | 64.078 | 0.96x |
| mixed.json | json | 0.442 | 0.456 | 0.472 | 64.078 | 0.68x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 13.955 | 14.106 | 14.420 | 68.391 | 1.00x |
| users.ndjson | orjson | 13.420 | 13.574 | 14.561 | 68.391 | 1.04x |
| users.ndjson | msgspec | 13.138 | 13.250 | 14.250 | 68.391 | 1.06x |
| users.ndjson | ujson | 16.623 | 16.744 | 17.970 | 68.391 | 0.84x |
| users.ndjson | json | 23.504 | 23.735 | 25.272 | 68.391 | 0.59x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.720 | 6.791 | 6.947 | 68.812 | 1.00x |
| users.json | orjson | 3.017 | 3.107 | 3.266 | 68.812 | 2.19x |
| users.json | msgspec | 3.817 | 3.853 | 4.047 | 68.812 | 1.76x |
| users.json | ujson | 14.155 | 14.308 | 14.545 | 68.812 | 0.47x |
| users.json | json | 19.833 | 20.010 | 20.407 | 68.812 | 0.34x |
| flat.json | strata | 0.711 | 0.755 | 0.923 | 69.312 | 1.00x |
| flat.json | orjson | 0.438 | 0.458 | 0.540 | 69.312 | 1.65x |
| flat.json | msgspec | 0.525 | 0.558 | 0.607 | 69.312 | 1.35x |
| flat.json | ujson | 1.331 | 1.360 | 1.589 | 69.312 | 0.56x |
| flat.json | json | 1.803 | 1.824 | 1.981 | 69.312 | 0.41x |
| nested.json | strata | 0.662 | 0.694 | 0.708 | 69.328 | 1.00x |
| nested.json | orjson | 0.399 | 0.414 | 0.477 | 69.328 | 1.67x |
| nested.json | msgspec | 0.485 | 0.507 | 0.608 | 69.328 | 1.37x |
| nested.json | ujson | 1.403 | 1.416 | 1.498 | 69.328 | 0.49x |
| nested.json | json | 2.262 | 2.282 | 2.363 | 69.328 | 0.30x |
| wide_arrays.json | strata | 3.683 | 3.773 | 3.827 | 69.328 | 1.00x |
| wide_arrays.json | orjson | 1.761 | 1.778 | 1.858 | 69.328 | 2.12x |
| wide_arrays.json | msgspec | 2.835 | 2.866 | 2.955 | 69.328 | 1.32x |
| wide_arrays.json | ujson | 8.207 | 8.302 | 8.375 | 69.328 | 0.45x |
| wide_arrays.json | json | 14.323 | 14.412 | 14.516 | 69.328 | 0.26x |
| mixed.json | strata | 0.295 | 0.318 | 0.461 | 64.109 | 1.00x |
| mixed.json | orjson | 0.183 | 0.213 | 0.251 | 64.109 | 1.49x |
| mixed.json | msgspec | 0.180 | 0.196 | 0.348 | 64.109 | 1.63x |
| mixed.json | ujson | 0.407 | 0.425 | 0.504 | 64.109 | 0.75x |
| mixed.json | json | 0.568 | 0.592 | 0.665 | 64.109 | 0.54x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.065 | 0.073 | 0.090 | 69.031 | 1.00x |
| users.json $[*].id | jmespath | 0.426 | 0.441 | 0.458 | 69.031 | 0.17x |
| users.json $[*].id | jsonpath-ng | 4.182 | 4.281 | 4.478 | 69.031 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.413 | 0.433 | 0.544 | 69.406 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.649 | 2.689 | 3.127 | 69.406 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 14.528 | 14.716 | 16.808 | 69.406 | 0.03x |
| users.json $..total | strata | 1.797 | 1.809 | 1.841 | 71.484 | 1.00x |
| users.json $..total | jsonpath-ng | 271.792 | 273.810 | 279.389 | 71.484 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 15.348 | 15.560 | 16.176 | 69.109 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.566 | 14.051 | 14.816 | 69.109 | 1.11x |
| users.json $[*].id | orjson+jsonpath-ng | 15.162 | 15.519 | 15.942 | 69.109 | 1.00x |
| users.json $[*].orders[*].total | strata | 15.856 | 16.138 | 16.719 | 71.469 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 16.103 | 16.362 | 16.969 | 71.469 | 0.99x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 25.875 | 26.519 | 27.754 | 71.469 | 0.61x |
| users.json $..total | strata | 17.290 | 17.573 | 18.284 | 71.484 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 283.527 | 284.004 | 287.024 | 71.484 | 0.06x |

