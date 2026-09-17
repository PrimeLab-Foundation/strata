# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c89aea890fd23de6c299c86e4099ab38c8418300
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: AMD EPYC 7763 64-Core Processor
- compiler_flags: -fno-strict-overflow -Wsign-compare -DNDEBUG -g -O3 -Wall -fPIC -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -fomit-frame-pointer -march=native -flto=thin -fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata -fsplit-machine-functions -shared -Wl,--rpath=/opt/hostedtoolcache/Python/3.12.14/x64/lib (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.509 | 9.699 | 14.124 | 64.598 | 1.00x |
| users.json | orjson | 12.923 | 13.130 | 16.688 | 64.598 | 0.74x |
| users.json | msgspec | 12.963 | 13.094 | 16.761 | 64.598 | 0.74x |
| users.json | ujson | 17.252 | 17.500 | 23.178 | 64.598 | 0.55x |
| users.json | pysimdjson | 17.807 | 18.031 | 22.869 | 64.598 | 0.54x |
| users.json | json | 21.927 | 22.132 | 23.910 | 64.598 | 0.44x |
| flat.json | strata | 0.843 | 0.848 | 0.866 | 81.562 | 1.00x |
| flat.json | orjson | 0.973 | 0.978 | 0.985 | 81.562 | 0.87x |
| flat.json | msgspec | 1.001 | 1.011 | 1.062 | 81.562 | 0.84x |
| flat.json | ujson | 1.473 | 1.481 | 1.500 | 81.562 | 0.57x |
| flat.json | pysimdjson | 1.535 | 1.560 | 1.578 | 81.562 | 0.54x |
| flat.json | json | 1.897 | 1.908 | 1.928 | 81.562 | 0.44x |
| nested.json | strata | 0.790 | 0.804 | 0.851 | 81.562 | 1.00x |
| nested.json | orjson | 0.990 | 0.996 | 1.031 | 81.562 | 0.81x |
| nested.json | msgspec | 1.005 | 1.018 | 1.055 | 81.562 | 0.79x |
| nested.json | ujson | 1.440 | 1.454 | 1.506 | 81.562 | 0.55x |
| nested.json | pysimdjson | 1.394 | 1.406 | 1.503 | 81.562 | 0.57x |
| nested.json | json | 2.005 | 2.023 | 2.057 | 81.562 | 0.40x |
| wide_arrays.json | strata | 4.060 | 4.110 | 4.180 | 84.594 | 1.00x |
| wide_arrays.json | orjson | 5.085 | 5.172 | 9.643 | 84.594 | 0.79x |
| wide_arrays.json | msgspec | 5.612 | 5.679 | 7.274 | 84.594 | 0.72x |
| wide_arrays.json | ujson | 7.085 | 7.138 | 7.288 | 84.594 | 0.58x |
| wide_arrays.json | pysimdjson | 6.085 | 6.133 | 6.215 | 84.594 | 0.67x |
| wide_arrays.json | json | 9.696 | 9.807 | 9.975 | 84.594 | 0.42x |
| mixed.json | strata | 0.188 | 0.191 | 0.214 | 84.594 | 1.00x |
| mixed.json | orjson | 0.227 | 0.230 | 0.333 | 84.594 | 0.83x |
| mixed.json | msgspec | 0.235 | 0.238 | 0.390 | 84.594 | 0.80x |
| mixed.json | ujson | 0.301 | 0.306 | 0.337 | 84.594 | 0.62x |
| mixed.json | pysimdjson | 0.299 | 0.308 | 0.318 | 84.594 | 0.62x |
| mixed.json | json | 0.469 | 0.480 | 0.494 | 84.594 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.276 | 2.303 | 2.365 | 63.695 | 1.00x |
| users.json | orjson | 2.871 | 2.900 | 2.946 | 63.695 | 0.79x |
| users.json | msgspec | 3.821 | 3.840 | 4.425 | 63.695 | 0.60x |
| users.json | ujson | 11.220 | 11.364 | 11.446 | 63.695 | 0.20x |
| users.json | json | 21.426 | 21.495 | 21.749 | 63.695 | 0.11x |
| flat.json | strata | 0.276 | 0.278 | 0.291 | 81.562 | 1.00x |
| flat.json | orjson | 0.324 | 0.329 | 0.349 | 81.562 | 0.84x |
| flat.json | msgspec | 0.423 | 0.435 | 0.492 | 81.562 | 0.64x |
| flat.json | ujson | 1.018 | 1.030 | 1.051 | 81.562 | 0.27x |
| flat.json | json | 1.834 | 1.852 | 1.924 | 81.562 | 0.15x |
| nested.json | strata | 0.225 | 0.226 | 0.246 | 81.566 | 1.00x |
| nested.json | orjson | 0.289 | 0.297 | 0.330 | 81.566 | 0.76x |
| nested.json | msgspec | 0.397 | 0.399 | 0.410 | 81.566 | 0.57x |
| nested.json | ujson | 1.073 | 1.082 | 1.117 | 81.566 | 0.21x |
| nested.json | json | 2.372 | 2.393 | 2.418 | 81.566 | 0.09x |
| wide_arrays.json | strata | 1.617 | 1.632 | 1.672 | 84.594 | 1.00x |
| wide_arrays.json | orjson | 1.792 | 1.805 | 1.827 | 84.594 | 0.90x |
| wide_arrays.json | msgspec | 2.729 | 2.750 | 2.805 | 84.594 | 0.59x |
| wide_arrays.json | ujson | 6.333 | 6.369 | 6.501 | 84.594 | 0.26x |
| wide_arrays.json | json | 16.449 | 16.542 | 17.354 | 84.594 | 0.10x |
| mixed.json | strata | 0.061 | 0.062 | 0.074 | 84.594 | 1.00x |
| mixed.json | orjson | 0.065 | 0.066 | 0.077 | 84.594 | 0.94x |
| mixed.json | msgspec | 0.083 | 0.085 | 0.098 | 84.594 | 0.73x |
| mixed.json | ujson | 0.227 | 0.233 | 0.245 | 84.594 | 0.26x |
| mixed.json | json | 0.508 | 0.517 | 0.530 | 84.594 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.114 | 10.375 | 12.043 | 83.195 | 1.00x |
| users.json | orjson | 13.245 | 13.663 | 14.666 | 83.195 | 0.76x |
| users.json | msgspec | 13.464 | 14.078 | 14.557 | 83.195 | 0.74x |
| users.json | ujson | 18.330 | 19.355 | 20.775 | 83.195 | 0.54x |
| users.json | json | 22.497 | 22.743 | 23.249 | 83.195 | 0.46x |
| flat.json | strata | 0.858 | 0.874 | 0.915 | 81.562 | 1.00x |
| flat.json | orjson | 1.029 | 1.037 | 1.099 | 81.562 | 0.84x |
| flat.json | msgspec | 1.060 | 1.080 | 1.804 | 81.562 | 0.81x |
| flat.json | ujson | 1.567 | 1.585 | 1.798 | 81.562 | 0.55x |
| flat.json | json | 1.964 | 1.979 | 2.009 | 81.562 | 0.44x |
| nested.json | strata | 0.813 | 0.827 | 0.833 | 81.566 | 1.00x |
| nested.json | orjson | 1.042 | 1.046 | 1.063 | 81.566 | 0.79x |
| nested.json | msgspec | 1.060 | 1.067 | 1.111 | 81.566 | 0.78x |
| nested.json | ujson | 1.509 | 1.532 | 1.774 | 81.566 | 0.54x |
| nested.json | json | 2.069 | 2.078 | 2.099 | 81.566 | 0.40x |
| wide_arrays.json | strata | 4.146 | 4.202 | 4.235 | 84.594 | 1.00x |
| wide_arrays.json | orjson | 5.153 | 5.197 | 5.354 | 84.594 | 0.81x |
| wide_arrays.json | msgspec | 5.782 | 5.868 | 5.971 | 84.594 | 0.72x |
| wide_arrays.json | ujson | 7.228 | 7.351 | 7.425 | 84.594 | 0.57x |
| wide_arrays.json | json | 9.679 | 9.772 | 9.947 | 84.594 | 0.43x |
| mixed.json | strata | 0.203 | 0.206 | 0.253 | 84.594 | 1.00x |
| mixed.json | orjson | 0.275 | 0.291 | 0.346 | 84.594 | 0.71x |
| mixed.json | msgspec | 0.282 | 0.290 | 0.337 | 84.594 | 0.71x |
| mixed.json | ujson | 0.359 | 0.362 | 0.405 | 84.594 | 0.57x |
| mixed.json | json | 0.513 | 0.528 | 0.536 | 84.594 | 0.39x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.873 | 10.019 | 10.146 | 81.562 | 1.00x |
| users.ndjson | orjson | 16.471 | 16.622 | 17.154 | 81.562 | 0.60x |
| users.ndjson | msgspec | 16.487 | 16.638 | 18.347 | 81.562 | 0.60x |
| users.ndjson | ujson | 21.281 | 21.753 | 22.768 | 81.562 | 0.46x |
| users.ndjson | json | 28.799 | 29.011 | 29.256 | 81.562 | 0.35x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.847 | 2.862 | 2.927 | 83.195 | 1.00x |
| users.json | orjson | 3.531 | 3.561 | 3.629 | 83.195 | 0.80x |
| users.json | msgspec | 4.392 | 4.441 | 4.522 | 83.195 | 0.64x |
| users.json | ujson | 12.066 | 12.157 | 12.456 | 83.195 | 0.24x |
| users.json | json | 22.158 | 22.250 | 22.503 | 83.195 | 0.13x |
| flat.json | strata | 0.413 | 0.428 | 0.445 | 81.562 | 1.00x |
| flat.json | orjson | 0.477 | 0.497 | 0.559 | 81.562 | 0.86x |
| flat.json | msgspec | 0.574 | 0.594 | 0.721 | 81.562 | 0.72x |
| flat.json | ujson | 1.190 | 1.215 | 1.301 | 81.562 | 0.35x |
| flat.json | json | 2.016 | 2.034 | 2.116 | 81.562 | 0.21x |
| nested.json | strata | 0.333 | 0.345 | 0.366 | 81.566 | 1.00x |
| nested.json | orjson | 0.422 | 0.436 | 0.443 | 81.566 | 0.79x |
| nested.json | msgspec | 0.531 | 0.544 | 0.566 | 81.566 | 0.63x |
| nested.json | ujson | 1.216 | 1.234 | 1.264 | 81.566 | 0.28x |
| nested.json | json | 2.521 | 2.537 | 2.548 | 81.566 | 0.14x |
| wide_arrays.json | strata | 2.009 | 2.033 | 2.060 | 84.594 | 1.00x |
| wide_arrays.json | orjson | 2.221 | 2.251 | 2.297 | 84.594 | 0.90x |
| wide_arrays.json | msgspec | 3.158 | 3.200 | 3.262 | 84.594 | 0.64x |
| wide_arrays.json | ujson | 6.818 | 6.843 | 6.886 | 84.594 | 0.30x |
| wide_arrays.json | json | 17.038 | 17.093 | 17.272 | 84.594 | 0.12x |
| mixed.json | strata | 0.142 | 0.146 | 0.177 | 84.594 | 1.00x |
| mixed.json | orjson | 0.167 | 0.168 | 0.195 | 84.594 | 0.87x |
| mixed.json | msgspec | 0.187 | 0.191 | 0.212 | 84.594 | 0.76x |
| mixed.json | ujson | 0.342 | 0.346 | 0.381 | 84.594 | 0.42x |
| mixed.json | json | 0.623 | 0.647 | 0.668 | 84.594 | 0.22x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.062 | 0.063 | 0.092 | 83.195 | 1.00x |
| users.json $[*].id | jmespath | 0.490 | 0.503 | 0.525 | 83.195 | 0.12x |
| users.json $[*].id | jsonpath-ng | 2.792 | 2.816 | 2.925 | 83.195 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.413 | 0.447 | 0.457 | 83.199 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.058 | 3.088 | 3.549 | 83.199 | 0.14x |
| users.json $[*].orders[*].total | jsonpath-ng | 19.163 | 19.487 | 20.049 | 83.199 | 0.02x |
| users.json $..total | strata | 1.643 | 1.686 | 1.732 | 83.199 | 1.00x |
| users.json $..total | jsonpath-ng | 390.590 | 394.352 | 396.066 | 83.199 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.278 | 3.313 | 3.340 | 83.199 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.378 | 14.525 | 14.760 | 83.199 | 0.23x |
| users.json $[*].id | orjson+jsonpath-ng | 16.730 | 16.927 | 17.555 | 83.199 | 0.20x |
| users.json $[*].orders[*].total | strata | 3.532 | 3.552 | 3.748 | 83.199 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.476 | 17.659 | 23.212 | 83.199 | 0.20x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 36.959 | 37.300 | 38.637 | 83.199 | 0.10x |
| users.json $..total | strata | 13.451 | 14.234 | 14.841 | 83.199 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 402.706 | 409.052 | 412.058 | 83.199 | 0.03x |

