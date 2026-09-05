# Benchmark results - ci-linux-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 1cc7049
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
| users.json | strata | 8.964 | 9.028 | 10.766 | 51.840 | 1.00x |
| users.json | orjson | 11.553 | 11.602 | 12.981 | 51.840 | 0.78x |
| users.json | msgspec | 12.123 | 12.179 | 13.430 | 51.840 | 0.74x |
| users.json | ujson | 16.175 | 16.298 | 18.557 | 51.840 | 0.55x |
| users.json | pysimdjson | 16.232 | 16.467 | 18.061 | 51.840 | 0.55x |
| users.json | json | 20.334 | 20.443 | 21.144 | 51.840 | 0.44x |
| flat.json | strata | 0.845 | 0.891 | 0.910 | 58.703 | 1.00x |
| flat.json | orjson | 0.889 | 0.919 | 0.933 | 58.703 | 0.97x |
| flat.json | msgspec | 0.935 | 0.949 | 0.962 | 58.703 | 0.94x |
| flat.json | ujson | 1.487 | 1.516 | 1.559 | 58.703 | 0.59x |
| flat.json | pysimdjson | 1.527 | 1.564 | 1.634 | 58.703 | 0.57x |
| flat.json | json | 1.807 | 1.835 | 1.911 | 58.703 | 0.49x |
| nested.json | strata | 0.809 | 0.817 | 0.830 | 58.703 | 1.00x |
| nested.json | orjson | 0.864 | 0.890 | 0.914 | 58.703 | 0.92x |
| nested.json | msgspec | 0.972 | 0.995 | 1.027 | 58.703 | 0.82x |
| nested.json | ujson | 1.401 | 1.423 | 1.472 | 58.703 | 0.57x |
| nested.json | pysimdjson | 1.384 | 1.408 | 1.431 | 58.703 | 0.58x |
| nested.json | json | 1.959 | 1.987 | 2.024 | 58.703 | 0.41x |
| wide_arrays.json | strata | 4.444 | 4.593 | 4.736 | 60.270 | 1.00x |
| wide_arrays.json | orjson | 4.018 | 4.247 | 4.414 | 60.270 | 1.08x |
| wide_arrays.json | msgspec | 4.985 | 5.157 | 5.283 | 60.270 | 0.89x |
| wide_arrays.json | ujson | 6.394 | 6.643 | 6.769 | 60.270 | 0.69x |
| wide_arrays.json | pysimdjson | 5.297 | 5.455 | 5.755 | 60.270 | 0.84x |
| wide_arrays.json | json | 9.475 | 9.753 | 9.869 | 60.270 | 0.47x |
| mixed.json | strata | 0.190 | 0.199 | 0.221 | 60.270 | 1.00x |
| mixed.json | orjson | 0.211 | 0.221 | 0.263 | 60.270 | 0.90x |
| mixed.json | msgspec | 0.232 | 0.242 | 0.273 | 60.270 | 0.82x |
| mixed.json | ujson | 0.311 | 0.316 | 0.340 | 60.270 | 0.63x |
| mixed.json | pysimdjson | 0.290 | 0.303 | 0.327 | 60.270 | 0.66x |
| mixed.json | json | 0.464 | 0.473 | 0.490 | 60.270 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.058 | 2.066 | 2.084 | 50.934 | 1.00x |
| users.json | orjson | 2.573 | 2.586 | 2.606 | 50.934 | 0.80x |
| users.json | msgspec | 3.304 | 3.318 | 3.339 | 50.934 | 0.62x |
| users.json | ujson | 10.480 | 10.506 | 10.608 | 50.934 | 0.20x |
| users.json | json | 18.921 | 18.968 | 19.182 | 50.934 | 0.11x |
| flat.json | strata | 0.235 | 0.241 | 0.268 | 58.703 | 1.00x |
| flat.json | orjson | 0.303 | 0.306 | 0.324 | 58.703 | 0.79x |
| flat.json | msgspec | 0.388 | 0.397 | 0.420 | 58.703 | 0.61x |
| flat.json | ujson | 1.003 | 1.009 | 1.026 | 58.703 | 0.24x |
| flat.json | json | 1.723 | 1.736 | 1.752 | 58.703 | 0.14x |
| nested.json | strata | 0.236 | 0.242 | 0.270 | 58.703 | 1.00x |
| nested.json | orjson | 0.288 | 0.292 | 0.318 | 58.703 | 0.83x |
| nested.json | msgspec | 0.380 | 0.397 | 0.408 | 58.703 | 0.61x |
| nested.json | ujson | 1.081 | 1.100 | 1.110 | 58.703 | 0.22x |
| nested.json | json | 2.169 | 2.203 | 2.235 | 58.703 | 0.11x |
| wide_arrays.json | strata | 1.302 | 1.334 | 1.364 | 60.270 | 1.00x |
| wide_arrays.json | orjson | 1.578 | 1.596 | 1.637 | 60.270 | 0.84x |
| wide_arrays.json | msgspec | 2.328 | 2.341 | 2.365 | 60.270 | 0.57x |
| wide_arrays.json | ujson | 4.753 | 4.789 | 4.812 | 60.270 | 0.28x |
| wide_arrays.json | json | 13.556 | 13.666 | 13.737 | 60.270 | 0.10x |
| mixed.json | strata | 0.072 | 0.078 | 0.103 | 60.270 | 1.00x |
| mixed.json | orjson | 0.068 | 0.070 | 0.073 | 60.270 | 1.12x |
| mixed.json | msgspec | 0.086 | 0.088 | 0.110 | 60.270 | 0.89x |
| mixed.json | ujson | 0.249 | 0.253 | 0.257 | 60.270 | 0.31x |
| mixed.json | json | 0.496 | 0.511 | 0.529 | 60.270 | 0.15x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.125 | 9.184 | 9.914 | 63.102 | 1.00x |
| users.json | orjson | 11.653 | 11.784 | 12.209 | 63.102 | 0.78x |
| users.json | msgspec | 12.266 | 12.358 | 12.664 | 63.102 | 0.74x |
| users.json | ujson | 16.516 | 16.746 | 17.771 | 63.102 | 0.55x |
| users.json | json | 20.580 | 20.651 | 20.838 | 63.102 | 0.44x |
| flat.json | strata | 0.943 | 0.959 | 0.985 | 58.703 | 1.00x |
| flat.json | orjson | 1.026 | 1.048 | 1.093 | 58.703 | 0.91x |
| flat.json | msgspec | 1.061 | 1.085 | 1.112 | 58.703 | 0.88x |
| flat.json | ujson | 1.635 | 1.673 | 1.689 | 58.703 | 0.57x |
| flat.json | json | 1.920 | 1.931 | 2.009 | 58.703 | 0.50x |
| nested.json | strata | 0.842 | 0.859 | 0.874 | 58.703 | 1.00x |
| nested.json | orjson | 0.962 | 0.975 | 0.999 | 58.703 | 0.88x |
| nested.json | msgspec | 1.068 | 1.081 | 1.124 | 58.703 | 0.79x |
| nested.json | ujson | 1.484 | 1.513 | 1.555 | 58.703 | 0.57x |
| nested.json | json | 2.022 | 2.050 | 2.083 | 58.703 | 0.42x |
| wide_arrays.json | strata | 4.558 | 4.760 | 4.962 | 60.270 | 1.00x |
| wide_arrays.json | orjson | 4.385 | 4.567 | 4.728 | 60.270 | 1.04x |
| wide_arrays.json | msgspec | 5.216 | 5.522 | 5.649 | 60.270 | 0.86x |
| wide_arrays.json | ujson | 6.876 | 7.156 | 7.332 | 60.270 | 0.67x |
| wide_arrays.json | json | 9.719 | 10.027 | 10.282 | 60.270 | 0.47x |
| mixed.json | strata | 0.229 | 0.242 | 0.270 | 60.270 | 1.00x |
| mixed.json | orjson | 0.289 | 0.317 | 0.344 | 60.270 | 0.76x |
| mixed.json | msgspec | 0.308 | 0.324 | 0.369 | 60.270 | 0.75x |
| mixed.json | ujson | 0.397 | 0.413 | 0.451 | 60.270 | 0.59x |
| mixed.json | json | 0.520 | 0.549 | 0.583 | 60.270 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.744 | 10.488 | 10.691 | 58.695 | 1.00x |
| users.ndjson | orjson | 15.263 | 15.983 | 16.290 | 58.695 | 0.66x |
| users.ndjson | msgspec | 16.151 | 16.333 | 16.460 | 58.695 | 0.64x |
| users.ndjson | ujson | 20.861 | 21.234 | 21.461 | 58.695 | 0.49x |
| users.ndjson | json | 27.079 | 27.636 | 27.826 | 58.695 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.488 | 2.512 | 2.601 | 61.102 | 1.00x |
| users.json | orjson | 3.018 | 3.074 | 3.169 | 61.102 | 0.82x |
| users.json | msgspec | 3.758 | 3.813 | 3.882 | 61.102 | 0.66x |
| users.json | ujson | 11.075 | 11.146 | 11.232 | 61.102 | 0.23x |
| users.json | json | 19.490 | 19.684 | 19.727 | 61.102 | 0.13x |
| flat.json | strata | 0.409 | 0.455 | 0.512 | 58.703 | 1.00x |
| flat.json | orjson | 0.508 | 0.552 | 0.619 | 58.703 | 0.82x |
| flat.json | msgspec | 0.593 | 0.644 | 0.694 | 58.703 | 0.71x |
| flat.json | ujson | 1.224 | 1.272 | 1.313 | 58.703 | 0.36x |
| flat.json | json | 1.928 | 1.970 | 2.027 | 58.703 | 0.23x |
| nested.json | strata | 0.383 | 0.412 | 0.460 | 58.703 | 1.00x |
| nested.json | orjson | 0.468 | 0.494 | 1.275 | 58.703 | 0.83x |
| nested.json | msgspec | 0.557 | 0.590 | 0.633 | 58.703 | 0.70x |
| nested.json | ujson | 1.298 | 1.317 | 1.383 | 58.703 | 0.31x |
| nested.json | json | 2.371 | 2.410 | 2.530 | 58.703 | 0.17x |
| wide_arrays.json | strata | 1.657 | 1.856 | 1.945 | 60.270 | 1.00x |
| wide_arrays.json | orjson | 1.953 | 2.145 | 2.198 | 60.270 | 0.87x |
| wide_arrays.json | msgspec | 2.675 | 2.872 | 2.963 | 60.270 | 0.65x |
| wide_arrays.json | ujson | 5.149 | 5.359 | 5.446 | 60.270 | 0.35x |
| wide_arrays.json | json | 13.977 | 14.261 | 14.369 | 60.270 | 0.13x |
| mixed.json | strata | 0.187 | 0.205 | 0.230 | 60.270 | 1.00x |
| mixed.json | orjson | 0.195 | 0.249 | 0.289 | 60.270 | 0.82x |
| mixed.json | msgspec | 0.220 | 0.245 | 0.262 | 60.270 | 0.84x |
| mixed.json | ujson | 0.392 | 0.421 | 0.492 | 60.270 | 0.49x |
| mixed.json | json | 0.639 | 0.678 | 0.754 | 60.270 | 0.30x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.100 | 0.102 | 0.103 | 61.102 | 1.00x |
| users.json $[*].id | jmespath | 0.466 | 0.472 | 0.483 | 61.102 | 0.22x |
| users.json $[*].id | jsonpath-ng | 2.412 | 2.457 | 2.476 | 61.102 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.625 | 0.639 | 0.646 | 61.230 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.982 | 3.009 | 3.095 | 61.230 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.226 | 17.390 | 18.000 | 61.230 | 0.04x |
| users.json $..total | strata | 1.705 | 1.725 | 1.732 | 62.113 | 1.00x |
| users.json $..total | jsonpath-ng | 295.565 | 296.437 | 297.173 | 62.113 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.290 | 3.297 | 3.351 | 61.230 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.214 | 12.281 | 12.447 | 61.230 | 0.27x |
| users.json $[*].id | orjson+jsonpath-ng | 14.093 | 14.191 | 14.322 | 61.230 | 0.23x |
| users.json $[*].orders[*].total | strata | 3.413 | 3.478 | 3.517 | 62.113 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 14.803 | 14.880 | 15.206 | 62.113 | 0.23x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 33.091 | 33.220 | 33.597 | 62.113 | 0.10x |
| users.json $..total | strata | 11.870 | 12.424 | 12.998 | 62.328 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 314.129 | 316.565 | 318.788 | 62.328 | 0.04x |

