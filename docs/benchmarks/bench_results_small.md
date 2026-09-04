# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 523bc54
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
| users.json | strata | 6.379 | 6.476 | 6.741 | 53.172 | 1.00x |
| users.json | orjson | 7.916 | 8.036 | 8.178 | 53.172 | 0.81x |
| users.json | msgspec | 7.970 | 8.072 | 8.322 | 53.172 | 0.80x |
| users.json | ujson | 10.851 | 11.014 | 12.487 | 53.172 | 0.59x |
| users.json | json | 15.167 | 15.598 | 16.349 | 53.172 | 0.42x |
| flat.json | strata | 0.588 | 0.592 | 0.600 | 70.750 | 1.00x |
| flat.json | orjson | 0.634 | 0.637 | 0.648 | 70.750 | 0.93x |
| flat.json | msgspec | 0.674 | 0.678 | 0.704 | 70.750 | 0.87x |
| flat.json | ujson | 0.998 | 1.007 | 1.017 | 70.750 | 0.59x |
| flat.json | json | 1.402 | 1.407 | 1.436 | 70.750 | 0.42x |
| nested.json | strata | 0.535 | 0.537 | 0.557 | 70.781 | 1.00x |
| nested.json | orjson | 0.606 | 0.609 | 0.620 | 70.781 | 0.88x |
| nested.json | msgspec | 0.607 | 0.612 | 0.624 | 70.781 | 0.88x |
| nested.json | ujson | 0.874 | 0.880 | 0.903 | 70.781 | 0.61x |
| nested.json | json | 1.406 | 1.411 | 1.444 | 70.781 | 0.38x |
| wide_arrays.json | strata | 2.987 | 3.001 | 3.103 | 70.844 | 1.00x |
| wide_arrays.json | orjson | 3.174 | 3.194 | 3.229 | 70.844 | 0.94x |
| wide_arrays.json | msgspec | 3.926 | 3.945 | 3.988 | 70.844 | 0.76x |
| wide_arrays.json | ujson | 5.115 | 5.131 | 5.661 | 70.844 | 0.58x |
| wide_arrays.json | json | 7.503 | 7.533 | 8.148 | 70.844 | 0.40x |
| mixed.json | strata | 0.128 | 0.130 | 0.132 | 71.109 | 1.00x |
| mixed.json | orjson | 0.128 | 0.129 | 0.133 | 71.109 | 1.01x |
| mixed.json | msgspec | 0.141 | 0.142 | 0.145 | 71.109 | 0.91x |
| mixed.json | ujson | 0.181 | 0.184 | 0.187 | 71.109 | 0.71x |
| mixed.json | json | 0.305 | 0.306 | 0.327 | 71.109 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.432 | 1.438 | 1.508 | 55.766 | 1.00x |
| users.json | orjson | 2.009 | 2.015 | 2.067 | 55.766 | 0.71x |
| users.json | msgspec | 2.624 | 2.628 | 2.692 | 55.766 | 0.55x |
| users.json | ujson | 10.850 | 10.879 | 10.901 | 55.766 | 0.13x |
| users.json | json | 15.526 | 15.608 | 16.142 | 55.766 | 0.09x |
| flat.json | strata | 0.178 | 0.180 | 0.192 | 70.766 | 1.00x |
| flat.json | orjson | 0.223 | 0.225 | 0.230 | 70.766 | 0.80x |
| flat.json | msgspec | 0.294 | 0.297 | 0.308 | 70.766 | 0.61x |
| flat.json | ujson | 0.888 | 0.897 | 0.937 | 70.766 | 0.20x |
| flat.json | json | 1.327 | 1.331 | 1.513 | 70.766 | 0.14x |
| nested.json | strata | 0.136 | 0.140 | 0.147 | 70.844 | 1.00x |
| nested.json | orjson | 0.199 | 0.209 | 0.333 | 70.844 | 0.67x |
| nested.json | msgspec | 0.268 | 0.278 | 0.291 | 70.844 | 0.50x |
| nested.json | ujson | 0.950 | 0.965 | 0.985 | 70.844 | 0.15x |
| nested.json | json | 1.609 | 1.644 | 1.917 | 70.844 | 0.09x |
| wide_arrays.json | strata | 0.906 | 0.917 | 0.994 | 70.844 | 1.00x |
| wide_arrays.json | orjson | 1.104 | 1.127 | 1.287 | 70.844 | 0.81x |
| wide_arrays.json | msgspec | 1.926 | 1.959 | 2.146 | 70.844 | 0.47x |
| wide_arrays.json | ujson | 5.937 | 6.121 | 6.537 | 70.844 | 0.15x |
| wide_arrays.json | json | 11.007 | 11.292 | 12.021 | 70.844 | 0.08x |
| mixed.json | strata | 0.035 | 0.036 | 0.041 | 71.156 | 1.00x |
| mixed.json | orjson | 0.042 | 0.044 | 0.047 | 71.156 | 0.82x |
| mixed.json | msgspec | 0.048 | 0.049 | 0.063 | 71.156 | 0.74x |
| mixed.json | ujson | 0.214 | 0.217 | 0.226 | 71.156 | 0.16x |
| mixed.json | json | 0.352 | 0.358 | 0.371 | 71.156 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.615 | 6.777 | 6.898 | 67.406 | 1.00x |
| users.json | orjson | 8.177 | 8.353 | 8.626 | 67.406 | 0.81x |
| users.json | msgspec | 8.240 | 8.403 | 8.540 | 67.406 | 0.81x |
| users.json | ujson | 11.382 | 11.536 | 11.864 | 67.406 | 0.59x |
| users.json | json | 15.435 | 15.647 | 16.113 | 67.406 | 0.43x |
| flat.json | strata | 0.647 | 0.652 | 0.672 | 70.766 | 1.00x |
| flat.json | orjson | 0.700 | 0.703 | 0.723 | 70.766 | 0.93x |
| flat.json | msgspec | 0.740 | 0.750 | 0.760 | 70.766 | 0.87x |
| flat.json | ujson | 1.087 | 1.099 | 1.138 | 70.766 | 0.59x |
| flat.json | json | 1.461 | 1.473 | 1.487 | 70.766 | 0.44x |
| nested.json | strata | 0.595 | 0.612 | 0.637 | 70.844 | 1.00x |
| nested.json | orjson | 0.674 | 0.691 | 0.818 | 70.844 | 0.89x |
| nested.json | msgspec | 0.673 | 0.694 | 0.738 | 70.844 | 0.88x |
| nested.json | ujson | 0.972 | 0.986 | 1.028 | 70.844 | 0.62x |
| nested.json | json | 1.473 | 1.492 | 1.543 | 70.844 | 0.41x |
| wide_arrays.json | strata | 3.170 | 3.211 | 3.294 | 70.844 | 1.00x |
| wide_arrays.json | orjson | 3.333 | 3.442 | 3.804 | 70.844 | 0.93x |
| wide_arrays.json | msgspec | 4.178 | 4.271 | 4.450 | 70.844 | 0.75x |
| wide_arrays.json | ujson | 5.486 | 5.546 | 5.685 | 70.844 | 0.58x |
| wide_arrays.json | json | 7.754 | 7.874 | 7.979 | 70.844 | 0.41x |
| mixed.json | strata | 0.168 | 0.175 | 0.183 | 71.156 | 1.00x |
| mixed.json | orjson | 0.175 | 0.180 | 0.192 | 71.156 | 0.98x |
| mixed.json | msgspec | 0.186 | 0.191 | 0.207 | 71.156 | 0.92x |
| mixed.json | ujson | 0.238 | 0.245 | 0.273 | 71.156 | 0.71x |
| mixed.json | json | 0.350 | 0.354 | 0.383 | 71.156 | 0.50x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.910 | 6.967 | 7.347 | 69.891 | 1.00x |
| users.ndjson | orjson | 10.167 | 10.269 | 10.751 | 69.891 | 0.68x |
| users.ndjson | msgspec | 10.013 | 10.116 | 10.710 | 69.891 | 0.69x |
| users.ndjson | ujson | 12.890 | 13.051 | 13.342 | 69.891 | 0.53x |
| users.ndjson | json | 18.118 | 18.317 | 19.392 | 69.891 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.803 | 1.821 | 2.071 | 65.703 | 1.00x |
| users.json | orjson | 2.341 | 2.376 | 2.571 | 65.703 | 0.77x |
| users.json | msgspec | 2.983 | 2.998 | 3.222 | 65.703 | 0.61x |
| users.json | ujson | 11.004 | 11.216 | 11.482 | 65.703 | 0.16x |
| users.json | json | 15.693 | 15.960 | 17.078 | 65.703 | 0.11x |
| flat.json | strata | 0.307 | 0.332 | 0.398 | 70.781 | 1.00x |
| flat.json | orjson | 0.366 | 0.382 | 0.409 | 70.781 | 0.87x |
| flat.json | msgspec | 0.429 | 0.455 | 0.464 | 70.781 | 0.73x |
| flat.json | ujson | 1.055 | 1.074 | 1.108 | 70.781 | 0.31x |
| flat.json | json | 1.474 | 1.505 | 1.572 | 70.781 | 0.22x |
| nested.json | strata | 0.280 | 0.294 | 0.309 | 70.844 | 1.00x |
| nested.json | orjson | 0.335 | 0.358 | 0.394 | 70.844 | 0.82x |
| nested.json | msgspec | 0.402 | 0.422 | 0.469 | 70.844 | 0.70x |
| nested.json | ujson | 1.104 | 1.133 | 1.293 | 70.844 | 0.26x |
| nested.json | json | 1.824 | 1.858 | 1.962 | 70.844 | 0.16x |
| wide_arrays.json | strata | 1.178 | 1.195 | 1.221 | 71.109 | 1.00x |
| wide_arrays.json | orjson | 1.393 | 1.420 | 1.441 | 71.109 | 0.84x |
| wide_arrays.json | msgspec | 2.224 | 2.238 | 2.460 | 71.109 | 0.53x |
| wide_arrays.json | ujson | 6.315 | 6.365 | 6.680 | 71.109 | 0.19x |
| wide_arrays.json | json | 11.345 | 11.384 | 11.888 | 71.109 | 0.10x |
| mixed.json | strata | 0.152 | 0.158 | 0.186 | 71.188 | 1.00x |
| mixed.json | orjson | 0.159 | 0.167 | 0.218 | 71.188 | 0.95x |
| mixed.json | msgspec | 0.166 | 0.174 | 0.244 | 71.188 | 0.91x |
| mixed.json | ujson | 0.344 | 0.368 | 0.641 | 71.188 | 0.43x |
| mixed.json | json | 0.478 | 0.489 | 0.543 | 71.188 | 0.32x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.048 | 0.049 | 0.078 | 65.891 | 1.00x |
| users.json $[*].id | jmespath | 0.271 | 0.278 | 0.340 | 65.891 | 0.18x |
| users.json $[*].id | jsonpath-ng | 1.413 | 1.432 | 1.501 | 65.891 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.321 | 0.332 | 0.486 | 66.188 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.644 | 1.660 | 1.730 | 66.188 | 0.20x |
| users.json $[*].orders[*].total | jsonpath-ng | 9.448 | 9.507 | 10.546 | 66.188 | 0.03x |
| users.json $..total | strata | 1.387 | 1.399 | 1.449 | 67.406 | 1.00x |
| users.json $..total | jsonpath-ng | 183.943 | 185.306 | 187.213 | 67.406 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.392 | 3.417 | 3.665 | 65.938 | 1.00x |
| users.json $[*].id | orjson+jmespath | 8.602 | 8.703 | 8.897 | 65.938 | 0.39x |
| users.json $[*].id | orjson+jsonpath-ng | 9.833 | 9.913 | 10.080 | 65.938 | 0.34x |
| users.json $[*].orders[*].total | strata | 3.452 | 3.492 | 3.595 | 67.312 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 9.929 | 10.048 | 10.210 | 67.312 | 0.35x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 19.084 | 19.161 | 19.336 | 67.312 | 0.18x |
| users.json $..total | strata | 8.121 | 8.190 | 8.423 | 68.297 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 193.740 | 195.132 | 196.973 | 68.297 | 0.04x |

