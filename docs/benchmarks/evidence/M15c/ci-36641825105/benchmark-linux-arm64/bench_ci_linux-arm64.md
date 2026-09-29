# Benchmark results - ci-linux-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 271a2a0864aa430c4690ca9b609e4201e13cfb51
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-aarch64-with-glibc2.39
- machine: aarch64
- processor: aarch64
- compiler_flags: -fno-strict-overflow -Wsign-compare -DNDEBUG -g -O3 -Wall -fPIC -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -march=native -flto=thin -fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata -shared -Wl,--rpath=/opt/hostedtoolcache/Python/3.12.14/arm64/lib (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.973 | 9.004 | 10.839 | 57.270 | 1.00x |
| users.json | orjson | 11.624 | 11.719 | 13.264 | 57.270 | 0.77x |
| users.json | msgspec | 12.224 | 12.302 | 13.835 | 57.270 | 0.73x |
| users.json | ujson | 16.243 | 16.325 | 19.208 | 57.270 | 0.55x |
| users.json | pysimdjson | 16.309 | 16.427 | 18.411 | 57.270 | 0.55x |
| users.json | json | 20.568 | 20.671 | 21.423 | 57.270 | 0.44x |
| flat.json | strata | 0.829 | 0.844 | 0.847 | 68.535 | 1.00x |
| flat.json | orjson | 0.875 | 0.885 | 0.890 | 68.535 | 0.95x |
| flat.json | msgspec | 0.916 | 0.930 | 0.935 | 68.535 | 0.91x |
| flat.json | ujson | 1.421 | 1.430 | 1.443 | 68.535 | 0.59x |
| flat.json | pysimdjson | 1.477 | 1.491 | 1.505 | 68.535 | 0.57x |
| flat.json | json | 1.769 | 1.790 | 1.806 | 68.535 | 0.47x |
| nested.json | strata | 0.811 | 0.828 | 0.834 | 68.535 | 1.00x |
| nested.json | orjson | 0.872 | 0.887 | 0.896 | 68.535 | 0.93x |
| nested.json | msgspec | 1.001 | 1.010 | 1.029 | 68.535 | 0.82x |
| nested.json | ujson | 1.384 | 1.395 | 1.421 | 68.535 | 0.59x |
| nested.json | pysimdjson | 1.396 | 1.405 | 1.437 | 68.535 | 0.59x |
| nested.json | json | 1.978 | 1.991 | 1.996 | 68.535 | 0.42x |
| wide_arrays.json | strata | 3.915 | 3.922 | 3.949 | 70.168 | 1.00x |
| wide_arrays.json | orjson | 4.070 | 4.094 | 4.125 | 70.168 | 0.96x |
| wide_arrays.json | msgspec | 5.105 | 5.127 | 5.149 | 70.168 | 0.76x |
| wide_arrays.json | ujson | 6.454 | 6.472 | 6.531 | 70.168 | 0.61x |
| wide_arrays.json | pysimdjson | 5.260 | 5.284 | 5.292 | 70.168 | 0.74x |
| wide_arrays.json | json | 9.476 | 9.512 | 9.634 | 70.168 | 0.41x |
| mixed.json | strata | 0.192 | 0.193 | 0.231 | 70.168 | 1.00x |
| mixed.json | orjson | 0.213 | 0.217 | 0.237 | 70.168 | 0.89x |
| mixed.json | msgspec | 0.233 | 0.235 | 0.252 | 70.168 | 0.82x |
| mixed.json | ujson | 0.300 | 0.314 | 0.329 | 70.168 | 0.62x |
| mixed.json | pysimdjson | 0.293 | 0.298 | 0.317 | 70.168 | 0.65x |
| mixed.json | json | 0.446 | 0.452 | 0.477 | 70.168 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.907 | 1.915 | 1.931 | 56.367 | 1.00x |
| users.json | orjson | 2.568 | 2.585 | 2.596 | 56.367 | 0.74x |
| users.json | msgspec | 3.267 | 3.286 | 3.369 | 56.367 | 0.58x |
| users.json | ujson | 10.493 | 10.533 | 10.571 | 56.367 | 0.18x |
| users.json | json | 19.222 | 19.262 | 19.316 | 56.367 | 0.10x |
| flat.json | strata | 0.235 | 0.236 | 0.250 | 68.535 | 1.00x |
| flat.json | orjson | 0.301 | 0.303 | 0.315 | 68.535 | 0.78x |
| flat.json | msgspec | 0.391 | 0.393 | 0.409 | 68.535 | 0.60x |
| flat.json | ujson | 0.990 | 0.997 | 1.011 | 68.535 | 0.24x |
| flat.json | json | 1.695 | 1.703 | 1.710 | 68.535 | 0.14x |
| nested.json | strata | 0.214 | 0.216 | 0.231 | 68.539 | 1.00x |
| nested.json | orjson | 0.284 | 0.289 | 0.303 | 68.539 | 0.75x |
| nested.json | msgspec | 0.371 | 0.375 | 0.387 | 68.539 | 0.58x |
| nested.json | ujson | 1.074 | 1.082 | 1.094 | 68.539 | 0.20x |
| nested.json | json | 2.155 | 2.164 | 2.198 | 68.539 | 0.10x |
| wide_arrays.json | strata | 1.307 | 1.316 | 1.328 | 70.168 | 1.00x |
| wide_arrays.json | orjson | 1.550 | 1.568 | 1.590 | 70.168 | 0.84x |
| wide_arrays.json | msgspec | 2.354 | 2.376 | 2.384 | 70.168 | 0.55x |
| wide_arrays.json | ujson | 4.733 | 4.744 | 4.757 | 70.168 | 0.28x |
| wide_arrays.json | json | 13.537 | 13.556 | 13.593 | 70.168 | 0.10x |
| mixed.json | strata | 0.060 | 0.061 | 0.063 | 70.168 | 1.00x |
| mixed.json | orjson | 0.063 | 0.064 | 0.069 | 70.168 | 0.96x |
| mixed.json | msgspec | 0.076 | 0.077 | 0.094 | 70.168 | 0.79x |
| mixed.json | ujson | 0.235 | 0.238 | 0.254 | 70.168 | 0.26x |
| mixed.json | json | 0.471 | 0.490 | 0.497 | 70.168 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.092 | 9.139 | 9.810 | 70.168 | 1.00x |
| users.json | orjson | 11.750 | 11.820 | 12.142 | 70.168 | 0.77x |
| users.json | msgspec | 12.383 | 12.462 | 12.666 | 70.168 | 0.73x |
| users.json | ujson | 16.623 | 16.751 | 17.732 | 70.168 | 0.55x |
| users.json | json | 20.746 | 20.814 | 20.934 | 70.168 | 0.44x |
| flat.json | strata | 0.867 | 0.880 | 0.884 | 68.535 | 1.00x |
| flat.json | orjson | 0.941 | 0.960 | 0.971 | 68.535 | 0.92x |
| flat.json | msgspec | 0.999 | 1.006 | 1.017 | 68.535 | 0.87x |
| flat.json | ujson | 1.526 | 1.534 | 1.552 | 68.535 | 0.57x |
| flat.json | json | 1.849 | 1.856 | 1.868 | 68.535 | 0.47x |
| nested.json | strata | 0.858 | 0.864 | 0.890 | 68.539 | 1.00x |
| nested.json | orjson | 0.939 | 0.957 | 0.988 | 68.539 | 0.90x |
| nested.json | msgspec | 1.077 | 1.081 | 1.092 | 68.539 | 0.80x |
| nested.json | ujson | 1.479 | 1.491 | 1.511 | 68.539 | 0.58x |
| nested.json | json | 2.058 | 2.063 | 2.081 | 68.539 | 0.42x |
| wide_arrays.json | strata | 3.895 | 3.917 | 3.935 | 70.168 | 1.00x |
| wide_arrays.json | orjson | 4.043 | 4.082 | 4.122 | 70.168 | 0.96x |
| wide_arrays.json | msgspec | 5.112 | 5.143 | 5.173 | 70.168 | 0.76x |
| wide_arrays.json | ujson | 6.614 | 6.638 | 6.668 | 70.168 | 0.59x |
| wide_arrays.json | json | 9.552 | 9.583 | 9.617 | 70.168 | 0.41x |
| mixed.json | strata | 0.218 | 0.220 | 0.238 | 70.168 | 1.00x |
| mixed.json | orjson | 0.274 | 0.280 | 0.281 | 70.168 | 0.78x |
| mixed.json | msgspec | 0.297 | 0.300 | 0.308 | 70.168 | 0.73x |
| mixed.json | ujson | 0.376 | 0.381 | 0.397 | 70.168 | 0.58x |
| mixed.json | json | 0.510 | 0.530 | 0.533 | 70.168 | 0.41x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.483 | 9.510 | 9.567 | 68.535 | 1.00x |
| users.ndjson | orjson | 14.567 | 14.663 | 14.693 | 68.535 | 0.65x |
| users.ndjson | msgspec | 15.093 | 15.119 | 15.139 | 68.535 | 0.63x |
| users.ndjson | ujson | 19.418 | 19.482 | 19.535 | 68.535 | 0.49x |
| users.ndjson | json | 25.622 | 25.704 | 25.768 | 68.535 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.398 | 2.447 | 2.544 | 70.168 | 1.00x |
| users.json | orjson | 3.183 | 3.214 | 3.281 | 70.168 | 0.76x |
| users.json | msgspec | 3.848 | 3.868 | 3.944 | 70.168 | 0.63x |
| users.json | ujson | 11.121 | 11.193 | 11.262 | 70.168 | 0.22x |
| users.json | json | 19.570 | 19.618 | 19.684 | 70.168 | 0.12x |
| flat.json | strata | 0.441 | 0.469 | 0.530 | 68.535 | 1.00x |
| flat.json | orjson | 0.543 | 0.564 | 0.605 | 68.535 | 0.83x |
| flat.json | msgspec | 0.644 | 0.671 | 0.720 | 68.535 | 0.70x |
| flat.json | ujson | 1.252 | 1.270 | 1.355 | 68.535 | 0.37x |
| flat.json | json | 1.966 | 2.012 | 2.041 | 68.535 | 0.23x |
| nested.json | strata | 0.411 | 0.425 | 0.435 | 68.539 | 1.00x |
| nested.json | orjson | 0.515 | 0.537 | 0.564 | 68.539 | 0.79x |
| nested.json | msgspec | 0.620 | 0.639 | 0.672 | 68.539 | 0.67x |
| nested.json | ujson | 1.347 | 1.361 | 1.386 | 68.539 | 0.31x |
| nested.json | json | 2.426 | 2.441 | 2.466 | 68.539 | 0.17x |
| wide_arrays.json | strata | 1.735 | 1.750 | 1.795 | 70.168 | 1.00x |
| wide_arrays.json | orjson | 2.025 | 2.055 | 2.083 | 70.168 | 0.85x |
| wide_arrays.json | msgspec | 2.842 | 2.868 | 2.888 | 70.168 | 0.61x |
| wide_arrays.json | ujson | 5.236 | 5.274 | 5.320 | 70.168 | 0.33x |
| wide_arrays.json | json | 14.028 | 14.072 | 14.138 | 70.168 | 0.12x |
| mixed.json | strata | 0.213 | 0.221 | 0.229 | 70.168 | 1.00x |
| mixed.json | orjson | 0.244 | 0.257 | 0.309 | 70.168 | 0.86x |
| mixed.json | msgspec | 0.261 | 0.274 | 0.298 | 70.168 | 0.81x |
| mixed.json | ujson | 0.438 | 0.461 | 0.474 | 70.168 | 0.48x |
| mixed.json | json | 0.691 | 0.699 | 0.712 | 70.168 | 0.32x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.105 | 0.107 | 0.116 | 70.168 | 1.00x |
| users.json $[*].id | jmespath | 0.474 | 0.483 | 0.487 | 70.168 | 0.22x |
| users.json $[*].id | jsonpath-ng | 2.439 | 2.491 | 2.524 | 70.168 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.641 | 0.657 | 0.671 | 70.172 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.014 | 3.033 | 3.061 | 70.172 | 0.22x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.394 | 17.539 | 17.710 | 70.172 | 0.04x |
| users.json $..total | strata | 1.704 | 1.712 | 1.721 | 70.172 | 1.00x |
| users.json $..total | jsonpath-ng | 290.043 | 290.490 | 291.035 | 70.172 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.115 | 3.127 | 3.140 | 70.172 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.339 | 12.385 | 12.479 | 70.172 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 14.274 | 14.371 | 14.448 | 70.172 | 0.22x |
| users.json $[*].orders[*].total | strata | 3.286 | 3.293 | 3.299 | 70.172 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.029 | 15.138 | 15.223 | 70.172 | 0.22x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 33.631 | 33.945 | 34.098 | 70.172 | 0.10x |
| users.json $..total | strata | 11.119 | 11.269 | 11.348 | 70.172 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 308.112 | 308.689 | 309.416 | 70.172 | 0.04x |

