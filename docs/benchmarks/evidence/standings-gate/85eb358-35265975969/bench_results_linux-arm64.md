# Benchmark results - ci-linux-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 85eb3583e98587844415f5d32f85a61cbedfcf49
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
| users.json | strata | 8.774 | 8.821 | 10.766 | 57.207 | 1.00x |
| users.json | orjson | 11.635 | 11.720 | 13.307 | 57.207 | 0.75x |
| users.json | msgspec | 12.170 | 12.222 | 13.747 | 57.207 | 0.72x |
| users.json | ujson | 16.340 | 16.482 | 19.017 | 57.207 | 0.54x |
| users.json | pysimdjson | 16.350 | 16.451 | 18.412 | 57.207 | 0.54x |
| users.json | json | 20.551 | 20.592 | 21.352 | 57.207 | 0.43x |
| flat.json | strata | 0.806 | 0.826 | 0.837 | 67.996 | 1.00x |
| flat.json | orjson | 0.843 | 0.861 | 0.867 | 67.996 | 0.96x |
| flat.json | msgspec | 0.911 | 0.917 | 0.927 | 67.996 | 0.90x |
| flat.json | ujson | 1.431 | 1.440 | 1.456 | 67.996 | 0.57x |
| flat.json | pysimdjson | 1.480 | 1.494 | 1.512 | 67.996 | 0.55x |
| flat.json | json | 1.755 | 1.771 | 1.781 | 67.996 | 0.47x |
| nested.json | strata | 0.805 | 0.822 | 0.832 | 67.996 | 1.00x |
| nested.json | orjson | 0.879 | 0.886 | 0.891 | 67.996 | 0.93x |
| nested.json | msgspec | 0.991 | 0.996 | 1.004 | 67.996 | 0.83x |
| nested.json | ujson | 1.380 | 1.401 | 1.413 | 67.996 | 0.59x |
| nested.json | pysimdjson | 1.387 | 1.395 | 1.403 | 67.996 | 0.59x |
| nested.json | json | 1.938 | 1.958 | 1.966 | 67.996 | 0.42x |
| wide_arrays.json | strata | 3.820 | 3.857 | 3.896 | 69.570 | 1.00x |
| wide_arrays.json | orjson | 4.030 | 4.082 | 4.126 | 69.570 | 0.94x |
| wide_arrays.json | msgspec | 5.056 | 5.080 | 5.123 | 69.570 | 0.76x |
| wide_arrays.json | ujson | 6.492 | 6.536 | 6.624 | 69.570 | 0.59x |
| wide_arrays.json | pysimdjson | 5.280 | 5.295 | 5.345 | 69.570 | 0.73x |
| wide_arrays.json | json | 9.461 | 9.494 | 9.667 | 69.570 | 0.41x |
| mixed.json | strata | 0.190 | 0.193 | 0.211 | 69.570 | 1.00x |
| mixed.json | orjson | 0.212 | 0.217 | 0.240 | 69.570 | 0.89x |
| mixed.json | msgspec | 0.229 | 0.236 | 0.257 | 69.570 | 0.82x |
| mixed.json | ujson | 0.303 | 0.310 | 0.327 | 69.570 | 0.62x |
| mixed.json | pysimdjson | 0.291 | 0.294 | 0.323 | 69.570 | 0.66x |
| mixed.json | json | 0.452 | 0.459 | 0.480 | 69.570 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.920 | 1.927 | 1.937 | 56.316 | 1.00x |
| users.json | orjson | 2.575 | 2.588 | 2.600 | 56.316 | 0.74x |
| users.json | msgspec | 3.306 | 3.321 | 3.351 | 56.316 | 0.58x |
| users.json | ujson | 10.461 | 10.489 | 10.577 | 56.316 | 0.18x |
| users.json | json | 18.868 | 18.909 | 19.007 | 56.316 | 0.10x |
| flat.json | strata | 0.233 | 0.236 | 0.251 | 67.996 | 1.00x |
| flat.json | orjson | 0.300 | 0.301 | 0.317 | 67.996 | 0.78x |
| flat.json | msgspec | 0.386 | 0.395 | 0.415 | 67.996 | 0.60x |
| flat.json | ujson | 0.982 | 0.987 | 1.001 | 67.996 | 0.24x |
| flat.json | json | 1.690 | 1.700 | 1.715 | 67.996 | 0.14x |
| nested.json | strata | 0.211 | 0.215 | 0.226 | 68.000 | 1.00x |
| nested.json | orjson | 0.278 | 0.281 | 0.297 | 68.000 | 0.77x |
| nested.json | msgspec | 0.361 | 0.364 | 0.382 | 68.000 | 0.59x |
| nested.json | ujson | 1.084 | 1.090 | 1.098 | 68.000 | 0.20x |
| nested.json | json | 2.138 | 2.159 | 2.197 | 68.000 | 0.10x |
| wide_arrays.json | strata | 1.338 | 1.353 | 1.415 | 69.570 | 1.00x |
| wide_arrays.json | orjson | 1.593 | 1.618 | 1.646 | 69.570 | 0.84x |
| wide_arrays.json | msgspec | 2.362 | 2.375 | 2.411 | 69.570 | 0.57x |
| wide_arrays.json | ujson | 4.753 | 4.789 | 4.830 | 69.570 | 0.28x |
| wide_arrays.json | json | 13.573 | 13.587 | 13.639 | 69.570 | 0.10x |
| mixed.json | strata | 0.059 | 0.061 | 0.085 | 69.570 | 1.00x |
| mixed.json | orjson | 0.063 | 0.064 | 0.085 | 69.570 | 0.96x |
| mixed.json | msgspec | 0.076 | 0.078 | 0.090 | 69.570 | 0.79x |
| mixed.json | ujson | 0.240 | 0.244 | 0.266 | 69.570 | 0.25x |
| mixed.json | json | 0.478 | 0.493 | 0.509 | 69.570 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.920 | 9.037 | 9.765 | 68.430 | 1.00x |
| users.json | orjson | 11.823 | 11.984 | 12.415 | 68.430 | 0.75x |
| users.json | msgspec | 12.380 | 12.506 | 12.774 | 68.430 | 0.72x |
| users.json | ujson | 16.966 | 17.263 | 18.171 | 68.430 | 0.52x |
| users.json | json | 20.825 | 20.949 | 21.301 | 68.430 | 0.43x |
| flat.json | strata | 0.839 | 0.843 | 0.853 | 67.996 | 1.00x |
| flat.json | orjson | 0.926 | 0.941 | 0.956 | 67.996 | 0.90x |
| flat.json | msgspec | 0.993 | 0.995 | 1.002 | 67.996 | 0.85x |
| flat.json | ujson | 1.540 | 1.555 | 1.576 | 67.996 | 0.54x |
| flat.json | json | 1.834 | 1.848 | 1.862 | 67.996 | 0.46x |
| nested.json | strata | 0.848 | 0.857 | 0.869 | 68.000 | 1.00x |
| nested.json | orjson | 0.956 | 0.963 | 0.968 | 68.000 | 0.89x |
| nested.json | msgspec | 1.070 | 1.076 | 1.090 | 68.000 | 0.80x |
| nested.json | ujson | 1.490 | 1.506 | 1.539 | 68.000 | 0.57x |
| nested.json | json | 2.029 | 2.041 | 2.066 | 68.000 | 0.42x |
| wide_arrays.json | strata | 3.835 | 3.871 | 3.888 | 69.570 | 1.00x |
| wide_arrays.json | orjson | 4.054 | 4.099 | 4.142 | 69.570 | 0.94x |
| wide_arrays.json | msgspec | 5.104 | 5.124 | 5.147 | 69.570 | 0.76x |
| wide_arrays.json | ujson | 6.672 | 6.705 | 6.742 | 69.570 | 0.58x |
| wide_arrays.json | json | 9.527 | 9.592 | 9.631 | 69.570 | 0.40x |
| mixed.json | strata | 0.210 | 0.214 | 0.236 | 69.570 | 1.00x |
| mixed.json | orjson | 0.274 | 0.280 | 0.303 | 69.570 | 0.77x |
| mixed.json | msgspec | 0.292 | 0.295 | 0.315 | 69.570 | 0.73x |
| mixed.json | ujson | 0.378 | 0.379 | 0.402 | 69.570 | 0.57x |
| mixed.json | json | 0.505 | 0.517 | 0.530 | 69.570 | 0.41x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.345 | 9.406 | 9.506 | 67.988 | 1.00x |
| users.ndjson | orjson | 14.610 | 14.738 | 14.865 | 67.988 | 0.64x |
| users.ndjson | msgspec | 15.065 | 15.140 | 15.281 | 67.988 | 0.62x |
| users.ndjson | ujson | 19.663 | 19.802 | 20.213 | 67.988 | 0.47x |
| users.ndjson | json | 25.632 | 25.778 | 25.895 | 67.988 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.354 | 2.408 | 2.442 | 68.430 | 1.00x |
| users.json | orjson | 3.091 | 3.131 | 3.164 | 68.430 | 0.77x |
| users.json | msgspec | 3.806 | 3.839 | 3.873 | 68.430 | 0.63x |
| users.json | ujson | 11.079 | 11.168 | 11.464 | 68.430 | 0.22x |
| users.json | json | 19.528 | 19.630 | 30.535 | 68.430 | 0.12x |
| flat.json | strata | 0.405 | 0.418 | 0.453 | 67.996 | 1.00x |
| flat.json | orjson | 0.496 | 0.513 | 0.526 | 67.996 | 0.82x |
| flat.json | msgspec | 0.569 | 0.609 | 0.638 | 67.996 | 0.69x |
| flat.json | ujson | 1.198 | 1.229 | 1.238 | 67.996 | 0.34x |
| flat.json | json | 1.922 | 1.945 | 1.964 | 67.996 | 0.21x |
| nested.json | strata | 0.350 | 0.379 | 0.399 | 68.000 | 1.00x |
| nested.json | orjson | 0.467 | 0.475 | 0.502 | 68.000 | 0.80x |
| nested.json | msgspec | 0.534 | 0.568 | 0.591 | 68.000 | 0.67x |
| nested.json | ujson | 1.279 | 1.301 | 1.324 | 68.000 | 0.29x |
| nested.json | json | 2.353 | 2.365 | 2.426 | 68.000 | 0.16x |
| wide_arrays.json | strata | 1.718 | 1.724 | 1.772 | 69.570 | 1.00x |
| wide_arrays.json | orjson | 2.025 | 2.053 | 2.066 | 69.570 | 0.84x |
| wide_arrays.json | msgspec | 2.771 | 2.800 | 2.839 | 69.570 | 0.62x |
| wide_arrays.json | ujson | 5.228 | 5.249 | 5.293 | 69.570 | 0.33x |
| wide_arrays.json | json | 13.994 | 14.046 | 14.208 | 69.570 | 0.12x |
| mixed.json | strata | 0.161 | 0.172 | 0.191 | 69.570 | 1.00x |
| mixed.json | orjson | 0.189 | 0.200 | 0.241 | 69.570 | 0.86x |
| mixed.json | msgspec | 0.204 | 0.216 | 0.237 | 69.570 | 0.80x |
| mixed.json | ujson | 0.381 | 0.394 | 0.418 | 69.570 | 0.44x |
| mixed.json | json | 0.610 | 0.634 | 0.650 | 69.570 | 0.27x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.104 | 0.105 | 0.116 | 68.430 | 1.00x |
| users.json $[*].id | jmespath | 0.472 | 0.476 | 0.491 | 68.430 | 0.22x |
| users.json $[*].id | jsonpath-ng | 2.453 | 2.492 | 2.525 | 68.430 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.634 | 0.648 | 0.675 | 68.555 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.037 | 3.061 | 3.083 | 68.555 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.535 | 17.759 | 18.155 | 68.555 | 0.04x |
| users.json $..total | strata | 1.718 | 1.724 | 1.735 | 69.562 | 1.00x |
| users.json $..total | jsonpath-ng | 296.469 | 296.817 | 297.345 | 69.562 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.133 | 3.180 | 3.215 | 68.555 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.483 | 12.709 | 12.813 | 68.555 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 14.415 | 14.518 | 14.801 | 68.555 | 0.22x |
| users.json $[*].orders[*].total | strata | 3.349 | 3.368 | 3.380 | 69.562 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.223 | 15.482 | 15.746 | 69.562 | 0.22x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 33.850 | 33.938 | 34.066 | 69.562 | 0.10x |
| users.json $..total | strata | 11.228 | 11.629 | 11.923 | 69.625 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 311.048 | 311.797 | 312.929 | 69.625 | 0.04x |

