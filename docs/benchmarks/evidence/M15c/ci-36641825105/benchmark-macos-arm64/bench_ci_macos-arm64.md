# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 271a2a0864aa430c4690ca9b609e4201e13cfb51
- python: 3.12.10
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit
- machine: arm64
- processor: Apple M1 (Virtual)
- compiler_flags: -fno-strict-overflow -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -O3 -Wall -arch arm64 -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -march=native -flto=thin -fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata -bundle -undefined (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.587 | 7.010 | 11.656 | 68.391 | 1.00x |
| users.json | orjson | 10.506 | 11.073 | 13.177 | 68.391 | 0.63x |
| users.json | msgspec | 9.950 | 10.825 | 16.745 | 68.391 | 0.65x |
| users.json | ujson | 13.900 | 14.421 | 22.916 | 68.391 | 0.49x |
| users.json | pysimdjson | 127.807 | 138.951 | 154.650 | 68.391 | 0.05x |
| users.json | json | 16.324 | 17.446 | 21.271 | 68.391 | 0.40x |
| flat.json | strata | 0.605 | 0.616 | 0.691 | 97.234 | 1.00x |
| flat.json | orjson | 0.786 | 0.809 | 0.895 | 97.234 | 0.76x |
| flat.json | msgspec | 0.742 | 0.756 | 0.944 | 97.234 | 0.82x |
| flat.json | ujson | 1.243 | 1.283 | 1.394 | 97.234 | 0.48x |
| flat.json | pysimdjson | 11.986 | 12.254 | 12.577 | 97.234 | 0.05x |
| flat.json | json | 1.354 | 1.390 | 1.509 | 97.234 | 0.44x |
| nested.json | strata | 0.557 | 0.587 | 0.612 | 97.891 | 1.00x |
| nested.json | orjson | 0.782 | 0.831 | 0.920 | 97.891 | 0.71x |
| nested.json | msgspec | 0.736 | 0.760 | 0.794 | 97.891 | 0.77x |
| nested.json | ujson | 1.239 | 1.279 | 1.637 | 97.891 | 0.46x |
| nested.json | pysimdjson | 10.697 | 10.876 | 11.513 | 97.891 | 0.05x |
| nested.json | json | 1.481 | 1.539 | 1.586 | 97.891 | 0.38x |
| wide_arrays.json | strata | 3.401 | 3.689 | 3.905 | 100.047 | 1.00x |
| wide_arrays.json | orjson | 4.008 | 4.464 | 4.815 | 100.047 | 0.83x |
| wide_arrays.json | msgspec | 4.624 | 4.824 | 5.598 | 100.047 | 0.76x |
| wide_arrays.json | ujson | 5.848 | 6.010 | 8.037 | 100.047 | 0.61x |
| wide_arrays.json | pysimdjson | 66.216 | 67.972 | 72.900 | 100.047 | 0.05x |
| wide_arrays.json | json | 7.451 | 7.635 | 10.537 | 100.047 | 0.48x |
| mixed.json | strata | 0.132 | 0.144 | 0.167 | 100.062 | 1.00x |
| mixed.json | orjson | 0.171 | 0.184 | 0.219 | 100.062 | 0.78x |
| mixed.json | msgspec | 0.182 | 0.189 | 0.205 | 100.062 | 0.76x |
| mixed.json | ujson | 0.239 | 0.326 | 0.424 | 100.062 | 0.44x |
| mixed.json | pysimdjson | 2.564 | 2.585 | 2.709 | 100.062 | 0.06x |
| mixed.json | json | 0.342 | 0.351 | 0.372 | 100.062 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.635 | 1.729 | 2.055 | 83.109 | 1.00x |
| users.json | orjson | 2.368 | 2.530 | 2.613 | 83.109 | 0.68x |
| users.json | msgspec | 3.052 | 3.167 | 3.285 | 83.109 | 0.55x |
| users.json | ujson | 9.141 | 9.260 | 9.725 | 83.109 | 0.19x |
| users.json | json | 16.345 | 16.663 | 20.713 | 83.109 | 0.10x |
| flat.json | strata | 0.226 | 0.234 | 0.343 | 97.234 | 1.00x |
| flat.json | orjson | 0.276 | 0.279 | 0.635 | 97.234 | 0.84x |
| flat.json | msgspec | 0.341 | 0.351 | 0.417 | 97.234 | 0.66x |
| flat.json | ujson | 0.805 | 0.821 | 0.871 | 97.234 | 0.28x |
| flat.json | json | 1.409 | 1.428 | 3.041 | 97.234 | 0.16x |
| nested.json | strata | 0.139 | 0.146 | 0.157 | 97.891 | 1.00x |
| nested.json | orjson | 0.245 | 0.254 | 0.275 | 97.891 | 0.57x |
| nested.json | msgspec | 0.318 | 0.551 | 0.570 | 97.891 | 0.26x |
| nested.json | ujson | 0.822 | 0.879 | 1.183 | 97.891 | 0.17x |
| nested.json | json | 1.719 | 1.759 | 1.839 | 97.891 | 0.08x |
| wide_arrays.json | strata | 1.222 | 1.325 | 1.625 | 100.047 | 1.00x |
| wide_arrays.json | orjson | 1.589 | 1.714 | 1.824 | 100.047 | 0.77x |
| wide_arrays.json | msgspec | 2.579 | 2.652 | 2.918 | 100.047 | 0.50x |
| wide_arrays.json | ujson | 5.237 | 5.274 | 8.258 | 100.047 | 0.25x |
| wide_arrays.json | json | 12.556 | 12.770 | 13.154 | 100.047 | 0.10x |
| mixed.json | strata | 0.042 | 0.048 | 0.068 | 100.062 | 1.00x |
| mixed.json | orjson | 0.052 | 0.058 | 0.078 | 100.062 | 0.83x |
| mixed.json | msgspec | 0.060 | 0.142 | 0.285 | 100.062 | 0.34x |
| mixed.json | ujson | 0.179 | 0.189 | 0.211 | 100.062 | 0.25x |
| mixed.json | json | 0.379 | 0.405 | 0.803 | 100.062 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.911 | 7.033 | 7.653 | 93.203 | 1.00x |
| users.json | orjson | 10.864 | 11.037 | 11.351 | 93.203 | 0.64x |
| users.json | msgspec | 10.581 | 10.819 | 11.513 | 93.203 | 0.65x |
| users.json | ujson | 14.551 | 14.835 | 15.976 | 93.203 | 0.47x |
| users.json | json | 16.975 | 17.151 | 17.900 | 93.203 | 0.41x |
| flat.json | strata | 0.704 | 0.718 | 0.849 | 97.891 | 1.00x |
| flat.json | orjson | 1.004 | 1.028 | 1.129 | 97.891 | 0.70x |
| flat.json | msgspec | 0.873 | 0.899 | 0.982 | 97.891 | 0.80x |
| flat.json | ujson | 1.237 | 1.263 | 1.508 | 97.891 | 0.57x |
| flat.json | json | 1.462 | 1.511 | 1.640 | 97.891 | 0.48x |
| nested.json | strata | 0.623 | 0.649 | 0.707 | 97.891 | 1.00x |
| nested.json | orjson | 1.034 | 1.083 | 1.260 | 97.891 | 0.60x |
| nested.json | msgspec | 0.865 | 0.881 | 0.923 | 97.891 | 0.74x |
| nested.json | ujson | 1.164 | 1.182 | 1.244 | 97.891 | 0.55x |
| nested.json | json | 1.629 | 1.663 | 1.700 | 97.891 | 0.39x |
| wide_arrays.json | strata | 3.323 | 3.379 | 3.557 | 100.047 | 1.00x |
| wide_arrays.json | orjson | 4.102 | 4.176 | 4.293 | 100.047 | 0.81x |
| wide_arrays.json | msgspec | 4.656 | 4.724 | 4.853 | 100.047 | 0.72x |
| wide_arrays.json | ujson | 5.999 | 6.093 | 6.569 | 100.047 | 0.55x |
| wide_arrays.json | json | 7.414 | 7.662 | 8.277 | 100.047 | 0.44x |
| mixed.json | strata | 0.196 | 0.208 | 0.262 | 100.062 | 1.00x |
| mixed.json | orjson | 0.410 | 0.439 | 0.587 | 100.062 | 0.47x |
| mixed.json | msgspec | 0.282 | 0.297 | 0.326 | 100.062 | 0.70x |
| mixed.json | ujson | 0.332 | 0.361 | 0.431 | 100.062 | 0.58x |
| mixed.json | json | 0.435 | 0.452 | 0.482 | 100.062 | 0.46x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.277 | 7.335 | 7.502 | 97.234 | 1.00x |
| users.ndjson | orjson | 12.309 | 12.576 | 12.849 | 97.234 | 0.58x |
| users.ndjson | msgspec | 12.386 | 12.626 | 13.120 | 97.234 | 0.58x |
| users.ndjson | ujson | 15.207 | 15.355 | 15.999 | 97.234 | 0.48x |
| users.ndjson | json | 19.551 | 19.811 | 20.916 | 97.234 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.986 | 2.189 | 2.307 | 93.422 | 1.00x |
| users.json | orjson | 3.014 | 3.138 | 3.524 | 93.422 | 0.70x |
| users.json | msgspec | 3.711 | 3.803 | 3.875 | 93.422 | 0.58x |
| users.json | ujson | 9.863 | 10.000 | 11.070 | 93.422 | 0.22x |
| users.json | json | 17.084 | 17.436 | 18.365 | 93.422 | 0.13x |
| flat.json | strata | 0.516 | 0.579 | 0.957 | 97.891 | 1.00x |
| flat.json | orjson | 0.580 | 0.650 | 1.068 | 97.891 | 0.89x |
| flat.json | msgspec | 0.658 | 0.709 | 0.906 | 97.891 | 0.82x |
| flat.json | ujson | 1.141 | 1.204 | 1.436 | 97.891 | 0.48x |
| flat.json | json | 1.776 | 1.909 | 2.072 | 97.891 | 0.30x |
| nested.json | strata | 0.414 | 0.486 | 1.063 | 97.891 | 1.00x |
| nested.json | orjson | 0.574 | 0.652 | 0.758 | 97.891 | 0.75x |
| nested.json | msgspec | 0.612 | 0.843 | 0.971 | 97.891 | 0.58x |
| nested.json | ujson | 1.378 | 1.443 | 2.068 | 97.891 | 0.34x |
| nested.json | json | 2.105 | 2.302 | 3.542 | 97.891 | 0.21x |
| wide_arrays.json | strata | 1.755 | 1.865 | 2.092 | 100.047 | 1.00x |
| wide_arrays.json | orjson | 2.281 | 2.470 | 2.715 | 100.047 | 0.76x |
| wide_arrays.json | msgspec | 2.935 | 3.197 | 3.458 | 100.047 | 0.58x |
| wide_arrays.json | ujson | 5.911 | 6.194 | 6.864 | 100.047 | 0.30x |
| wide_arrays.json | json | 13.114 | 13.434 | 13.902 | 100.047 | 0.14x |
| mixed.json | strata | 0.269 | 0.305 | 0.366 | 100.062 | 1.00x |
| mixed.json | orjson | 0.292 | 0.343 | 0.623 | 100.062 | 0.89x |
| mixed.json | msgspec | 0.334 | 0.453 | 0.919 | 100.062 | 0.67x |
| mixed.json | ujson | 0.446 | 0.514 | 0.623 | 100.062 | 0.59x |
| mixed.json | json | 0.647 | 0.718 | 0.772 | 100.062 | 0.42x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.103 | 0.128 | 0.175 | 93.500 | 1.00x |
| users.json $[*].id | jmespath | 0.346 | 0.383 | 0.411 | 93.500 | 0.33x |
| users.json $[*].id | jsonpath-ng | 1.608 | 1.736 | 2.854 | 93.500 | 0.07x |
| users.json $[*].orders[*].total | strata | 0.638 | 0.694 | 0.743 | 93.578 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.983 | 2.104 | 2.176 | 93.578 | 0.33x |
| users.json $[*].orders[*].total | jsonpath-ng | 12.485 | 12.725 | 13.508 | 93.578 | 0.05x |
| users.json $..total | strata | 1.461 | 1.539 | 2.181 | 93.609 | 1.00x |
| users.json $..total | jsonpath-ng | 195.619 | 208.431 | 240.164 | 93.609 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.716 | 4.024 | 4.832 | 93.547 | 1.00x |
| users.json $[*].id | orjson+jmespath | 11.708 | 12.426 | 16.120 | 93.547 | 0.32x |
| users.json $[*].id | orjson+jsonpath-ng | 13.023 | 13.393 | 17.955 | 93.547 | 0.30x |
| users.json $[*].orders[*].total | strata | 3.768 | 3.922 | 4.571 | 93.594 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 13.096 | 13.538 | 19.203 | 93.594 | 0.29x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 27.298 | 28.435 | 31.134 | 93.594 | 0.14x |
| users.json $..total | strata | 8.798 | 8.956 | 9.303 | 93.656 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 211.250 | 213.903 | 220.899 | 93.656 | 0.04x |

