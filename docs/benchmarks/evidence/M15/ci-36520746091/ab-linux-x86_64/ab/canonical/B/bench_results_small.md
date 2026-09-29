# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: afd1550cbabc9433e8292644444a23bb27bbc4e9
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: AMD EPYC 9V74 80-Core Processor
- compiler_flags: -fno-strict-overflow -Wsign-compare -DNDEBUG -g -O3 -Wall -fPIC -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -fomit-frame-pointer -march=native -flto=thin -fprofile-use=/home/runner/work/_temp/strata-arm/build/pgo/strata.profdata -fsplit-machine-functions -shared -Wl,--rpath=/opt/hostedtoolcache/Python/3.12.14/x64/lib (24 recorded commands; see the JSON companion)
- repeats: 60
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.666 | 7.921 | 8.082 | 65.477 | 1.00x |
| users.json | orjson | 10.595 | 11.163 | 11.326 | 65.477 | 0.71x |
| users.json | msgspec | 10.522 | 10.982 | 11.146 | 65.477 | 0.72x |
| users.json | ujson | 14.308 | 14.923 | 15.272 | 65.477 | 0.53x |
| users.json | pysimdjson | 14.533 | 15.569 | 15.771 | 65.477 | 0.51x |
| users.json | json | 16.158 | 16.669 | 17.059 | 65.477 | 0.48x |
| flat.json | strata | 0.665 | 0.676 | 0.688 | 64.570 | 1.00x |
| flat.json | orjson | 0.794 | 0.812 | 0.824 | 64.570 | 0.83x |
| flat.json | msgspec | 0.783 | 0.802 | 0.820 | 64.570 | 0.84x |
| flat.json | ujson | 1.181 | 1.207 | 1.233 | 64.570 | 0.56x |
| flat.json | pysimdjson | 1.251 | 1.277 | 1.308 | 64.570 | 0.53x |
| flat.json | json | 1.345 | 1.363 | 1.385 | 64.570 | 0.50x |
| nested.json | strata | 0.617 | 0.631 | 0.640 | 64.570 | 1.00x |
| nested.json | orjson | 0.780 | 0.794 | 0.815 | 64.570 | 0.79x |
| nested.json | msgspec | 0.743 | 0.761 | 0.775 | 64.570 | 0.83x |
| nested.json | ujson | 1.097 | 1.127 | 1.148 | 64.570 | 0.56x |
| nested.json | pysimdjson | 1.098 | 1.116 | 1.133 | 64.570 | 0.57x |
| nested.json | json | 1.408 | 1.429 | 1.448 | 64.570 | 0.44x |
| wide_arrays.json | strata | 3.362 | 3.474 | 3.531 | 78.551 | 1.00x |
| wide_arrays.json | orjson | 4.266 | 4.364 | 4.431 | 78.551 | 0.80x |
| wide_arrays.json | msgspec | 4.712 | 4.804 | 4.871 | 78.551 | 0.72x |
| wide_arrays.json | ujson | 5.894 | 5.989 | 6.066 | 78.551 | 0.58x |
| wide_arrays.json | pysimdjson | 4.890 | 5.020 | 5.079 | 78.551 | 0.69x |
| wide_arrays.json | json | 7.642 | 7.769 | 7.867 | 78.551 | 0.45x |
| mixed.json | strata | 0.148 | 0.154 | 0.162 | 78.801 | 1.00x |
| mixed.json | orjson | 0.182 | 0.186 | 0.200 | 78.801 | 0.82x |
| mixed.json | msgspec | 0.187 | 0.192 | 0.206 | 78.801 | 0.80x |
| mixed.json | ujson | 0.236 | 0.245 | 0.257 | 78.801 | 0.63x |
| mixed.json | pysimdjson | 0.234 | 0.242 | 0.255 | 78.801 | 0.63x |
| mixed.json | json | 0.346 | 0.355 | 0.367 | 78.801 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.879 | 1.902 | 1.917 | 47.504 | 1.00x |
| users.json | orjson | 2.048 | 2.077 | 2.099 | 47.504 | 0.92x |
| users.json | msgspec | 3.317 | 3.335 | 3.373 | 47.504 | 0.57x |
| users.json | ujson | 8.874 | 9.046 | 9.240 | 47.504 | 0.21x |
| users.json | json | 16.828 | 17.014 | 17.159 | 47.504 | 0.11x |
| flat.json | strata | 0.232 | 0.242 | 0.252 | 64.570 | 1.00x |
| flat.json | orjson | 0.241 | 0.250 | 0.260 | 64.570 | 0.97x |
| flat.json | msgspec | 0.363 | 0.374 | 0.386 | 64.570 | 0.65x |
| flat.json | ujson | 0.795 | 0.808 | 0.816 | 64.570 | 0.30x |
| flat.json | json | 1.425 | 1.450 | 1.468 | 64.570 | 0.17x |
| nested.json | strata | 0.175 | 0.178 | 0.192 | 64.570 | 1.00x |
| nested.json | orjson | 0.220 | 0.225 | 0.236 | 64.570 | 0.79x |
| nested.json | msgspec | 0.318 | 0.326 | 0.337 | 64.570 | 0.55x |
| nested.json | ujson | 0.822 | 0.842 | 0.854 | 64.570 | 0.21x |
| nested.json | json | 1.814 | 1.841 | 1.875 | 64.570 | 0.10x |
| wide_arrays.json | strata | 1.353 | 1.375 | 1.396 | 78.551 | 1.00x |
| wide_arrays.json | orjson | 1.457 | 1.481 | 1.502 | 78.551 | 0.93x |
| wide_arrays.json | msgspec | 2.349 | 2.380 | 2.406 | 78.551 | 0.58x |
| wide_arrays.json | ujson | 4.946 | 5.012 | 5.044 | 78.551 | 0.27x |
| wide_arrays.json | json | 12.883 | 13.011 | 13.083 | 78.551 | 0.11x |
| mixed.json | strata | 0.047 | 0.050 | 0.057 | 78.801 | 1.00x |
| mixed.json | orjson | 0.047 | 0.049 | 0.054 | 78.801 | 1.01x |
| mixed.json | msgspec | 0.065 | 0.068 | 0.075 | 78.801 | 0.73x |
| mixed.json | ujson | 0.177 | 0.182 | 0.190 | 78.801 | 0.27x |
| mixed.json | json | 0.394 | 0.403 | 0.416 | 78.801 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.200 | 8.765 | 8.900 | 66.121 | 1.00x |
| users.json | orjson | 11.115 | 12.997 | 13.295 | 66.121 | 0.67x |
| users.json | msgspec | 10.988 | 12.234 | 12.538 | 66.121 | 0.72x |
| users.json | ujson | 15.570 | 17.684 | 18.521 | 66.121 | 0.50x |
| users.json | json | 16.661 | 17.958 | 18.239 | 66.121 | 0.49x |
| flat.json | strata | 0.687 | 0.704 | 0.720 | 64.570 | 1.00x |
| flat.json | orjson | 0.845 | 0.860 | 0.879 | 64.570 | 0.82x |
| flat.json | msgspec | 0.822 | 0.846 | 0.860 | 64.570 | 0.83x |
| flat.json | ujson | 1.261 | 1.284 | 1.309 | 64.570 | 0.55x |
| flat.json | json | 1.392 | 1.412 | 1.442 | 64.570 | 0.50x |
| nested.json | strata | 0.631 | 0.650 | 0.668 | 64.570 | 1.00x |
| nested.json | orjson | 0.816 | 0.834 | 0.852 | 64.570 | 0.78x |
| nested.json | msgspec | 0.776 | 0.791 | 0.813 | 64.570 | 0.82x |
| nested.json | ujson | 1.169 | 1.187 | 1.207 | 64.570 | 0.55x |
| nested.json | json | 1.441 | 1.458 | 1.487 | 64.570 | 0.45x |
| wide_arrays.json | strata | 3.449 | 3.532 | 3.598 | 78.551 | 1.00x |
| wide_arrays.json | orjson | 4.372 | 4.469 | 4.598 | 78.551 | 0.79x |
| wide_arrays.json | msgspec | 4.813 | 4.928 | 5.069 | 78.551 | 0.72x |
| wide_arrays.json | ujson | 5.989 | 6.154 | 6.473 | 78.551 | 0.57x |
| wide_arrays.json | json | 7.591 | 7.730 | 7.879 | 78.551 | 0.46x |
| mixed.json | strata | 0.162 | 0.168 | 0.179 | 78.801 | 1.00x |
| mixed.json | orjson | 0.220 | 0.227 | 0.244 | 78.801 | 0.74x |
| mixed.json | msgspec | 0.221 | 0.228 | 0.243 | 78.801 | 0.74x |
| mixed.json | ujson | 0.285 | 0.293 | 0.303 | 78.801 | 0.57x |
| mixed.json | json | 0.381 | 0.395 | 0.409 | 78.801 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 8.314 | 8.513 | 8.607 | 64.570 | 1.00x |
| users.ndjson | orjson | 13.931 | 14.155 | 14.306 | 64.570 | 0.60x |
| users.ndjson | msgspec | 13.746 | 14.057 | 14.206 | 64.570 | 0.61x |
| users.ndjson | ujson | 18.340 | 18.827 | 19.213 | 64.570 | 0.45x |
| users.ndjson | json | 22.359 | 22.759 | 23.017 | 64.570 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.458 | 2.526 | 2.566 | 66.121 | 1.00x |
| users.json | orjson | 2.670 | 2.719 | 2.755 | 66.121 | 0.93x |
| users.json | msgspec | 3.924 | 3.968 | 4.005 | 66.121 | 0.64x |
| users.json | ujson | 9.524 | 9.666 | 9.759 | 66.121 | 0.26x |
| users.json | json | 17.770 | 17.905 | 18.098 | 66.121 | 0.14x |
| flat.json | strata | 0.458 | 0.484 | 0.515 | 64.570 | 1.00x |
| flat.json | orjson | 0.486 | 0.511 | 0.548 | 64.570 | 0.95x |
| flat.json | msgspec | 0.607 | 0.633 | 0.659 | 64.570 | 0.76x |
| flat.json | ujson | 1.054 | 1.078 | 1.111 | 64.570 | 0.45x |
| flat.json | json | 1.695 | 1.729 | 1.770 | 64.570 | 0.28x |
| nested.json | strata | 0.381 | 0.402 | 0.432 | 64.664 | 1.00x |
| nested.json | orjson | 0.446 | 0.474 | 0.507 | 64.664 | 0.85x |
| nested.json | msgspec | 0.546 | 0.565 | 0.600 | 64.664 | 0.71x |
| nested.json | ujson | 1.056 | 1.086 | 1.125 | 64.664 | 0.37x |
| nested.json | json | 2.059 | 2.151 | 17.520 | 64.664 | 0.19x |
| wide_arrays.json | strata | 1.798 | 1.847 | 1.888 | 78.801 | 1.00x |
| wide_arrays.json | orjson | 1.907 | 1.970 | 2.003 | 78.801 | 0.94x |
| wide_arrays.json | msgspec | 2.815 | 2.866 | 2.925 | 78.801 | 0.64x |
| wide_arrays.json | ujson | 5.467 | 5.539 | 5.609 | 78.801 | 0.33x |
| wide_arrays.json | json | 13.447 | 13.598 | 13.698 | 78.801 | 0.14x |
| mixed.json | strata | 0.225 | 0.246 | 0.270 | 78.801 | 1.00x |
| mixed.json | orjson | 0.245 | 0.263 | 0.291 | 78.801 | 0.93x |
| mixed.json | msgspec | 0.262 | 0.287 | 0.306 | 78.801 | 0.86x |
| mixed.json | ujson | 0.387 | 0.411 | 0.438 | 78.801 | 0.60x |
| mixed.json | json | 0.602 | 0.633 | 0.663 | 78.801 | 0.39x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.055 | 0.057 | 0.059 | 66.121 | 1.00x |
| users.json $[*].id | jmespath | 0.360 | 0.369 | 0.381 | 66.121 | 0.15x |
| users.json $[*].id | jsonpath-ng | 2.099 | 2.134 | 2.172 | 66.121 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.331 | 0.343 | 0.354 | 66.133 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.288 | 2.328 | 2.358 | 66.133 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 14.790 | 15.400 | 15.661 | 66.133 | 0.02x |
| users.json $..total | strata | 1.417 | 1.452 | 1.484 | 66.145 | 1.00x |
| users.json $..total | jsonpath-ng | 304.163 | 306.772 | 309.066 | 66.145 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 2.568 | 2.597 | 2.628 | 66.133 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.350 | 12.830 | 13.408 | 66.133 | 0.20x |
| users.json $[*].id | orjson+jsonpath-ng | 14.186 | 14.677 | 15.107 | 66.133 | 0.18x |
| users.json $[*].orders[*].total | strata | 2.742 | 2.773 | 2.805 | 66.145 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 14.943 | 15.591 | 15.921 | 66.145 | 0.18x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 30.929 | 32.379 | 33.190 | 66.145 | 0.09x |
| users.json $..total | strata | 11.339 | 12.803 | 13.256 | 66.152 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 326.898 | 330.884 | 332.399 | 66.152 | 0.04x |

