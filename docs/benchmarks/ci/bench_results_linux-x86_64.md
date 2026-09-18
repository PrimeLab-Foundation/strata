# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: baba505678994faef24df367a2dc4b93d5d4bef0
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
| users.json | strata | 10.810 | 11.329 | 16.007 | 65.773 | 1.00x |
| users.json | orjson | 13.689 | 14.416 | 20.616 | 65.773 | 0.79x |
| users.json | msgspec | 13.631 | 14.226 | 19.499 | 65.773 | 0.80x |
| users.json | ujson | 19.772 | 20.785 | 25.632 | 65.773 | 0.55x |
| users.json | pysimdjson | 20.295 | 20.995 | 24.015 | 65.773 | 0.54x |
| users.json | json | 23.104 | 23.836 | 24.664 | 65.773 | 0.48x |
| flat.json | strata | 0.849 | 0.855 | 0.918 | 79.672 | 1.00x |
| flat.json | orjson | 0.981 | 1.003 | 1.039 | 79.672 | 0.85x |
| flat.json | msgspec | 1.028 | 1.053 | 1.093 | 79.672 | 0.81x |
| flat.json | ujson | 1.598 | 1.668 | 1.786 | 79.672 | 0.51x |
| flat.json | pysimdjson | 1.508 | 1.562 | 1.733 | 79.672 | 0.55x |
| flat.json | json | 1.912 | 1.928 | 1.959 | 79.672 | 0.44x |
| nested.json | strata | 0.812 | 0.830 | 0.847 | 79.672 | 1.00x |
| nested.json | orjson | 1.017 | 1.022 | 1.074 | 79.672 | 0.81x |
| nested.json | msgspec | 1.026 | 1.039 | 1.052 | 79.672 | 0.80x |
| nested.json | ujson | 1.529 | 1.588 | 1.676 | 79.672 | 0.52x |
| nested.json | pysimdjson | 1.431 | 1.467 | 1.513 | 79.672 | 0.57x |
| nested.json | json | 2.044 | 2.071 | 2.133 | 79.672 | 0.40x |
| wide_arrays.json | strata | 4.125 | 4.450 | 5.162 | 84.996 | 1.00x |
| wide_arrays.json | orjson | 5.378 | 5.884 | 8.526 | 84.996 | 0.76x |
| wide_arrays.json | msgspec | 5.705 | 5.983 | 6.670 | 84.996 | 0.74x |
| wide_arrays.json | ujson | 7.179 | 7.307 | 8.583 | 84.996 | 0.61x |
| wide_arrays.json | pysimdjson | 6.202 | 6.664 | 7.078 | 84.996 | 0.67x |
| wide_arrays.json | json | 10.203 | 10.631 | 11.293 | 84.996 | 0.42x |
| mixed.json | strata | 0.193 | 0.199 | 0.219 | 84.996 | 1.00x |
| mixed.json | orjson | 0.232 | 0.241 | 0.256 | 84.996 | 0.83x |
| mixed.json | msgspec | 0.243 | 0.248 | 0.291 | 84.996 | 0.80x |
| mixed.json | ujson | 0.311 | 0.321 | 0.336 | 84.996 | 0.62x |
| mixed.json | pysimdjson | 0.306 | 0.311 | 0.329 | 84.996 | 0.64x |
| mixed.json | json | 0.483 | 0.493 | 0.518 | 84.996 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.529 | 2.688 | 3.050 | 64.871 | 1.00x |
| users.json | orjson | 2.950 | 3.095 | 3.273 | 64.871 | 0.87x |
| users.json | msgspec | 3.921 | 3.981 | 4.128 | 64.871 | 0.68x |
| users.json | ujson | 11.584 | 11.669 | 12.058 | 64.871 | 0.23x |
| users.json | json | 22.106 | 22.473 | 23.246 | 64.871 | 0.12x |
| flat.json | strata | 0.298 | 0.302 | 0.321 | 79.672 | 1.00x |
| flat.json | orjson | 0.331 | 0.336 | 0.346 | 79.672 | 0.90x |
| flat.json | msgspec | 0.432 | 0.444 | 0.453 | 79.672 | 0.68x |
| flat.json | ujson | 1.024 | 1.040 | 1.057 | 79.672 | 0.29x |
| flat.json | json | 1.867 | 1.875 | 1.934 | 79.672 | 0.16x |
| nested.json | strata | 0.232 | 0.240 | 0.256 | 79.672 | 1.00x |
| nested.json | orjson | 0.293 | 0.302 | 0.329 | 79.672 | 0.80x |
| nested.json | msgspec | 0.406 | 0.420 | 0.427 | 79.672 | 0.57x |
| nested.json | ujson | 1.081 | 1.099 | 1.115 | 79.672 | 0.22x |
| nested.json | json | 2.375 | 2.399 | 2.425 | 79.672 | 0.10x |
| wide_arrays.json | strata | 1.665 | 1.922 | 2.305 | 84.996 | 1.00x |
| wide_arrays.json | orjson | 1.829 | 1.843 | 2.079 | 84.996 | 1.04x |
| wide_arrays.json | msgspec | 2.772 | 2.793 | 2.913 | 84.996 | 0.69x |
| wide_arrays.json | ujson | 6.375 | 6.420 | 6.526 | 84.996 | 0.30x |
| wide_arrays.json | json | 16.532 | 17.131 | 17.407 | 84.996 | 0.11x |
| mixed.json | strata | 0.060 | 0.062 | 0.076 | 84.996 | 1.00x |
| mixed.json | orjson | 0.065 | 0.066 | 0.080 | 84.996 | 0.94x |
| mixed.json | msgspec | 0.085 | 0.088 | 0.100 | 84.996 | 0.71x |
| mixed.json | ujson | 0.233 | 0.237 | 0.251 | 84.996 | 0.26x |
| mixed.json | json | 0.521 | 0.531 | 0.540 | 84.996 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 12.112 | 12.654 | 14.959 | 80.695 | 1.00x |
| users.json | orjson | 14.335 | 15.123 | 16.919 | 80.695 | 0.84x |
| users.json | msgspec | 14.488 | 15.299 | 16.924 | 80.695 | 0.83x |
| users.json | ujson | 21.920 | 24.295 | 27.104 | 80.695 | 0.52x |
| users.json | json | 23.595 | 24.535 | 27.165 | 80.695 | 0.52x |
| flat.json | strata | 0.873 | 0.892 | 0.914 | 79.672 | 1.00x |
| flat.json | orjson | 1.057 | 1.073 | 1.113 | 79.672 | 0.83x |
| flat.json | msgspec | 1.089 | 1.112 | 1.135 | 79.672 | 0.80x |
| flat.json | ujson | 1.675 | 1.730 | 1.790 | 79.672 | 0.52x |
| flat.json | json | 1.972 | 1.987 | 1.994 | 79.672 | 0.45x |
| nested.json | strata | 0.838 | 0.854 | 0.871 | 79.672 | 1.00x |
| nested.json | orjson | 1.068 | 1.077 | 1.102 | 79.672 | 0.79x |
| nested.json | msgspec | 1.082 | 1.091 | 1.117 | 79.672 | 0.78x |
| nested.json | ujson | 1.577 | 1.627 | 1.665 | 79.672 | 0.52x |
| nested.json | json | 2.098 | 2.119 | 2.137 | 79.672 | 0.40x |
| wide_arrays.json | strata | 4.221 | 4.589 | 4.741 | 84.996 | 1.00x |
| wide_arrays.json | orjson | 5.439 | 5.592 | 5.819 | 84.996 | 0.82x |
| wide_arrays.json | msgspec | 6.014 | 6.174 | 6.296 | 84.996 | 0.74x |
| wide_arrays.json | ujson | 7.492 | 7.751 | 7.973 | 84.996 | 0.59x |
| wide_arrays.json | json | 10.054 | 10.419 | 10.794 | 84.996 | 0.44x |
| mixed.json | strata | 0.211 | 0.214 | 0.228 | 84.996 | 1.00x |
| mixed.json | orjson | 0.278 | 0.290 | 0.298 | 84.996 | 0.74x |
| mixed.json | msgspec | 0.285 | 0.290 | 0.305 | 84.996 | 0.74x |
| mixed.json | ujson | 0.369 | 0.382 | 0.396 | 84.996 | 0.56x |
| mixed.json | json | 0.516 | 0.536 | 0.547 | 84.996 | 0.40x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 13.184 | 13.541 | 15.180 | 79.672 | 1.00x |
| users.ndjson | orjson | 18.714 | 20.223 | 22.884 | 79.672 | 0.67x |
| users.ndjson | msgspec | 18.760 | 19.350 | 20.885 | 79.672 | 0.70x |
| users.ndjson | ujson | 25.354 | 25.759 | 26.860 | 79.672 | 0.53x |
| users.ndjson | json | 32.673 | 33.348 | 35.699 | 79.672 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.040 | 3.245 | 3.504 | 80.695 | 1.00x |
| users.json | orjson | 3.766 | 4.056 | 4.328 | 80.695 | 0.80x |
| users.json | msgspec | 4.709 | 5.002 | 5.218 | 80.695 | 0.65x |
| users.json | ujson | 12.953 | 13.542 | 13.908 | 80.695 | 0.24x |
| users.json | json | 23.184 | 23.660 | 24.323 | 80.695 | 0.14x |
| flat.json | strata | 0.431 | 0.455 | 0.505 | 79.672 | 1.00x |
| flat.json | orjson | 0.501 | 0.529 | 0.547 | 79.672 | 0.86x |
| flat.json | msgspec | 0.617 | 0.627 | 0.645 | 79.672 | 0.73x |
| flat.json | ujson | 1.213 | 1.225 | 1.282 | 79.672 | 0.37x |
| flat.json | json | 2.051 | 2.082 | 2.140 | 79.672 | 0.22x |
| nested.json | strata | 0.360 | 0.382 | 0.410 | 79.672 | 1.00x |
| nested.json | orjson | 0.442 | 0.476 | 0.482 | 79.672 | 0.80x |
| nested.json | msgspec | 0.551 | 0.570 | 0.608 | 79.672 | 0.67x |
| nested.json | ujson | 1.240 | 1.258 | 1.284 | 79.672 | 0.30x |
| nested.json | json | 2.544 | 2.567 | 2.593 | 79.672 | 0.15x |
| wide_arrays.json | strata | 2.109 | 2.155 | 2.458 | 84.996 | 1.00x |
| wide_arrays.json | orjson | 2.328 | 2.353 | 2.444 | 84.996 | 0.92x |
| wide_arrays.json | msgspec | 3.269 | 3.328 | 3.407 | 84.996 | 0.65x |
| wide_arrays.json | ujson | 7.007 | 7.134 | 7.484 | 84.996 | 0.30x |
| wide_arrays.json | json | 17.566 | 17.968 | 18.597 | 84.996 | 0.12x |
| mixed.json | strata | 0.149 | 0.156 | 0.171 | 84.996 | 1.00x |
| mixed.json | orjson | 0.176 | 0.183 | 0.211 | 84.996 | 0.85x |
| mixed.json | msgspec | 0.194 | 0.202 | 0.239 | 84.996 | 0.77x |
| mixed.json | ujson | 0.354 | 0.368 | 0.391 | 84.996 | 0.42x |
| mixed.json | json | 0.639 | 0.658 | 0.677 | 84.996 | 0.24x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.078 | 0.088 | 0.107 | 80.695 | 1.00x |
| users.json $[*].id | jmespath | 0.529 | 0.569 | 0.660 | 80.695 | 0.15x |
| users.json $[*].id | jsonpath-ng | 3.230 | 3.488 | 3.769 | 80.695 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.471 | 0.501 | 0.787 | 80.695 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.209 | 3.268 | 3.482 | 80.695 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 21.640 | 23.910 | 24.806 | 80.695 | 0.02x |
| users.json $..total | strata | 1.742 | 2.068 | 2.327 | 80.695 | 1.00x |
| users.json $..total | jsonpath-ng | 396.309 | 398.432 | 399.606 | 80.695 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.352 | 3.370 | 3.460 | 80.695 | 1.00x |
| users.json $[*].id | orjson+jmespath | 16.186 | 16.728 | 17.127 | 80.695 | 0.20x |
| users.json $[*].id | orjson+jsonpath-ng | 19.094 | 20.258 | 21.447 | 80.695 | 0.17x |
| users.json $[*].orders[*].total | strata | 3.616 | 3.643 | 3.819 | 80.695 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 19.094 | 19.730 | 21.863 | 80.695 | 0.18x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 41.982 | 43.043 | 51.745 | 80.695 | 0.08x |
| users.json $..total | strata | 16.501 | 19.340 | 20.650 | 81.309 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 421.236 | 425.043 | 426.463 | 81.309 | 0.05x |

