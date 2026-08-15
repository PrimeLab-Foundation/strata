# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: dc84475
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
| users.json | strata | 7.579 | 8.430 | 8.858 | 63.734 | 1.00x |
| users.json | orjson | 9.017 | 11.267 | 14.971 | 63.734 | 0.75x |
| users.json | msgspec | 9.555 | 10.789 | 11.603 | 63.734 | 0.78x |
| users.json | ujson | 13.099 | 14.674 | 22.185 | 63.734 | 0.57x |
| users.json | pysimdjson | 127.809 | 143.730 | 212.523 | 63.734 | 0.06x |
| users.json | json | 15.647 | 17.949 | 27.621 | 63.734 | 0.47x |
| flat.json | strata | 0.674 | 0.766 | 1.697 | 91.188 | 1.00x |
| flat.json | orjson | 0.757 | 0.889 | 3.249 | 91.188 | 0.86x |
| flat.json | msgspec | 0.785 | 0.886 | 2.040 | 91.188 | 0.86x |
| flat.json | ujson | 1.204 | 1.581 | 2.488 | 91.188 | 0.48x |
| flat.json | pysimdjson | 12.973 | 17.231 | 23.086 | 91.188 | 0.04x |
| flat.json | json | 1.442 | 1.747 | 3.200 | 91.188 | 0.44x |
| nested.json | strata | 0.631 | 0.726 | 1.192 | 91.219 | 1.00x |
| nested.json | orjson | 0.770 | 0.993 | 2.195 | 91.219 | 0.73x |
| nested.json | msgspec | 0.725 | 0.848 | 17.961 | 91.219 | 0.86x |
| nested.json | ujson | 1.151 | 1.626 | 13.912 | 91.219 | 0.45x |
| nested.json | pysimdjson | 12.038 | 16.943 | 41.188 | 91.219 | 0.04x |
| nested.json | json | 1.587 | 2.028 | 4.858 | 91.219 | 0.36x |
| wide_arrays.json | strata | 3.934 | 5.476 | 29.920 | 95.188 | 1.00x |
| wide_arrays.json | orjson | 4.240 | 5.024 | 5.933 | 95.188 | 1.09x |
| wide_arrays.json | msgspec | 4.467 | 5.497 | 8.207 | 95.188 | 1.00x |
| wide_arrays.json | ujson | 6.172 | 6.788 | 12.339 | 95.188 | 0.81x |
| wide_arrays.json | pysimdjson | 73.399 | 86.387 | 108.528 | 95.188 | 0.06x |
| wide_arrays.json | json | 7.571 | 10.092 | 24.693 | 95.188 | 0.54x |
| mixed.json | strata | 0.141 | 0.148 | 0.183 | 95.203 | 1.00x |
| mixed.json | orjson | 0.160 | 0.166 | 0.193 | 95.203 | 0.89x |
| mixed.json | msgspec | 0.168 | 0.181 | 0.203 | 95.203 | 0.82x |
| mixed.json | ujson | 0.207 | 0.332 | 0.552 | 95.203 | 0.44x |
| mixed.json | pysimdjson | 2.455 | 2.547 | 3.017 | 95.203 | 0.06x |
| mixed.json | json | 0.320 | 0.336 | 0.432 | 95.203 | 0.44x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.233 | 2.476 | 2.994 | 78.797 | 1.00x |
| users.json | orjson | 2.381 | 2.596 | 3.396 | 78.797 | 0.95x |
| users.json | msgspec | 3.066 | 3.295 | 4.204 | 78.797 | 0.75x |
| users.json | ujson | 11.558 | 12.327 | 13.193 | 78.797 | 0.20x |
| users.json | json | 15.237 | 17.503 | 22.564 | 78.797 | 0.14x |
| flat.json | strata | 0.262 | 0.379 | 0.670 | 91.203 | 1.00x |
| flat.json | orjson | 0.266 | 0.364 | 0.486 | 91.203 | 1.04x |
| flat.json | msgspec | 0.343 | 0.424 | 1.285 | 91.203 | 0.89x |
| flat.json | ujson | 1.047 | 1.083 | 1.244 | 91.203 | 0.35x |
| flat.json | json | 1.456 | 1.577 | 5.400 | 91.203 | 0.24x |
| nested.json | strata | 0.187 | 0.215 | 0.677 | 91.219 | 1.00x |
| nested.json | orjson | 0.244 | 0.287 | 0.506 | 91.219 | 0.75x |
| nested.json | msgspec | 0.382 | 0.608 | 1.114 | 91.219 | 0.35x |
| nested.json | ujson | 1.210 | 1.690 | 2.215 | 91.219 | 0.13x |
| nested.json | json | 1.768 | 2.620 | 4.588 | 91.219 | 0.08x |
| wide_arrays.json | strata | 1.335 | 1.627 | 3.691 | 95.188 | 1.00x |
| wide_arrays.json | orjson | 1.545 | 1.747 | 2.061 | 95.188 | 0.93x |
| wide_arrays.json | msgspec | 2.363 | 2.669 | 3.091 | 95.188 | 0.61x |
| wide_arrays.json | ujson | 7.056 | 7.959 | 9.269 | 95.188 | 0.20x |
| wide_arrays.json | json | 11.827 | 13.963 | 17.214 | 95.188 | 0.12x |
| mixed.json | strata | 0.053 | 0.072 | 0.099 | 95.203 | 1.00x |
| mixed.json | orjson | 0.052 | 0.058 | 0.083 | 95.203 | 1.25x |
| mixed.json | msgspec | 0.060 | 0.071 | 0.525 | 95.203 | 1.02x |
| mixed.json | ujson | 0.235 | 0.253 | 0.287 | 95.203 | 0.29x |
| mixed.json | json | 0.378 | 0.423 | 0.796 | 95.203 | 0.17x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.250 | 9.891 | 18.296 | 89.266 | 1.00x |
| users.json | orjson | 9.818 | 11.875 | 27.141 | 89.266 | 0.83x |
| users.json | msgspec | 10.061 | 11.595 | 18.934 | 89.266 | 0.85x |
| users.json | ujson | 13.999 | 15.510 | 17.369 | 89.266 | 0.64x |
| users.json | json | 15.931 | 18.426 | 21.914 | 89.266 | 0.54x |
| flat.json | strata | 0.771 | 0.882 | 1.601 | 91.203 | 1.00x |
| flat.json | orjson | 1.009 | 1.397 | 2.665 | 91.203 | 0.63x |
| flat.json | msgspec | 0.890 | 1.057 | 2.611 | 91.203 | 0.83x |
| flat.json | ujson | 1.327 | 1.656 | 2.041 | 91.203 | 0.53x |
| flat.json | json | 1.600 | 1.852 | 4.464 | 91.203 | 0.48x |
| nested.json | strata | 0.688 | 0.781 | 0.902 | 91.219 | 1.00x |
| nested.json | orjson | 1.000 | 1.141 | 16.587 | 91.219 | 0.68x |
| nested.json | msgspec | 0.832 | 0.901 | 2.291 | 91.219 | 0.87x |
| nested.json | ujson | 1.247 | 1.475 | 1.892 | 91.219 | 0.53x |
| nested.json | json | 1.678 | 1.894 | 2.869 | 91.219 | 0.41x |
| wide_arrays.json | strata | 3.633 | 4.264 | 5.146 | 95.188 | 1.00x |
| wide_arrays.json | orjson | 3.819 | 4.784 | 5.744 | 95.188 | 0.89x |
| wide_arrays.json | msgspec | 4.559 | 5.397 | 7.410 | 95.188 | 0.79x |
| wide_arrays.json | ujson | 6.036 | 7.218 | 8.051 | 95.188 | 0.59x |
| wide_arrays.json | json | 7.399 | 8.444 | 28.990 | 95.188 | 0.50x |
| mixed.json | strata | 0.206 | 0.238 | 0.249 | 95.203 | 1.00x |
| mixed.json | orjson | 0.246 | 0.331 | 0.549 | 95.203 | 0.72x |
| mixed.json | msgspec | 0.227 | 0.293 | 0.404 | 95.203 | 0.81x |
| mixed.json | ujson | 0.324 | 0.602 | 1.026 | 95.203 | 0.40x |
| mixed.json | json | 0.456 | 0.501 | 1.122 | 95.203 | 0.48x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 8.500 | 9.116 | 26.093 | 91.188 | 1.00x |
| users.ndjson | orjson | 12.284 | 13.785 | 19.634 | 91.188 | 0.66x |
| users.ndjson | msgspec | 12.189 | 13.284 | 25.254 | 91.188 | 0.69x |
| users.ndjson | ujson | 14.957 | 16.312 | 25.471 | 91.188 | 0.56x |
| users.ndjson | json | 19.574 | 21.602 | 35.107 | 91.188 | 0.42x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.593 | 2.927 | 8.796 | 87.469 | 1.00x |
| users.json | orjson | 2.767 | 3.057 | 8.846 | 87.469 | 0.96x |
| users.json | msgspec | 3.343 | 3.560 | 9.620 | 87.469 | 0.82x |
| users.json | ujson | 11.576 | 12.152 | 19.378 | 87.469 | 0.24x |
| users.json | json | 17.085 | 17.570 | 26.773 | 87.469 | 0.17x |
| flat.json | strata | 0.453 | 0.536 | 14.140 | 91.203 | 1.00x |
| flat.json | orjson | 0.482 | 0.723 | 12.145 | 91.203 | 0.74x |
| flat.json | msgspec | 0.588 | 0.747 | 5.904 | 91.203 | 0.72x |
| flat.json | ujson | 1.276 | 1.438 | 13.336 | 91.203 | 0.37x |
| flat.json | json | 1.676 | 2.578 | 14.157 | 91.203 | 0.21x |
| nested.json | strata | 0.410 | 0.476 | 1.165 | 91.219 | 1.00x |
| nested.json | orjson | 0.474 | 0.571 | 1.012 | 91.219 | 0.83x |
| nested.json | msgspec | 0.496 | 0.730 | 6.178 | 91.219 | 0.65x |
| nested.json | ujson | 1.428 | 1.910 | 12.408 | 91.219 | 0.25x |
| nested.json | json | 2.040 | 3.022 | 14.100 | 91.219 | 0.16x |
| wide_arrays.json | strata | 1.585 | 1.735 | 2.572 | 95.188 | 1.00x |
| wide_arrays.json | orjson | 1.736 | 1.835 | 2.044 | 95.188 | 0.95x |
| wide_arrays.json | msgspec | 2.594 | 2.685 | 2.918 | 95.188 | 0.65x |
| wide_arrays.json | ujson | 6.966 | 7.623 | 9.185 | 95.188 | 0.23x |
| wide_arrays.json | json | 12.141 | 12.935 | 15.683 | 95.188 | 0.13x |
| mixed.json | strata | 0.184 | 0.225 | 0.263 | 95.203 | 1.00x |
| mixed.json | orjson | 0.187 | 0.241 | 0.607 | 95.203 | 0.93x |
| mixed.json | msgspec | 0.186 | 0.229 | 0.594 | 95.203 | 0.98x |
| mixed.json | ujson | 0.363 | 0.423 | 0.452 | 95.203 | 0.53x |
| mixed.json | json | 0.491 | 0.598 | 0.770 | 95.203 | 0.38x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.058 | 0.070 | 0.358 | 87.531 | 1.00x |
| users.json $[*].id | jmespath | 0.263 | 0.324 | 0.415 | 87.531 | 0.21x |
| users.json $[*].id | jsonpath-ng | 1.526 | 1.654 | 2.985 | 87.531 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.525 | 0.752 | 0.907 | 87.766 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.809 | 2.364 | 2.933 | 87.766 | 0.32x |
| users.json $[*].orders[*].total | jsonpath-ng | 11.252 | 13.635 | 16.378 | 87.766 | 0.06x |
| users.json $..total | strata | 1.394 | 1.634 | 3.070 | 87.578 | 1.00x |
| users.json $..total | jsonpath-ng | 216.823 | 237.534 | 388.541 | 87.578 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.814 | 4.002 | 4.226 | 87.609 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.276 | 12.251 | 13.390 | 87.609 | 0.33x |
| users.json $[*].id | orjson+jsonpath-ng | 12.221 | 13.992 | 14.933 | 87.609 | 0.29x |
| users.json $[*].orders[*].total | strata | 3.889 | 4.074 | 4.761 | 87.562 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.588 | 14.199 | 20.229 | 87.562 | 0.29x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 25.842 | 30.715 | 37.937 | 87.562 | 0.13x |
| users.json $..total | strata | 9.832 | 10.429 | 13.127 | 87.594 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 220.314 | 239.061 | 300.315 | 87.594 | 0.04x |

