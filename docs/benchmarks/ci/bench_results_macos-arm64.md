# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: cd9d20b6ab3ec573716c10b12fe76ae4b70a707c
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
| users.json | strata | 6.972 | 8.013 | 13.659 | 71.266 | 1.00x |
| users.json | orjson | 11.242 | 12.767 | 16.557 | 71.266 | 0.63x |
| users.json | msgspec | 11.133 | 11.676 | 19.917 | 71.266 | 0.69x |
| users.json | ujson | 13.640 | 17.458 | 22.406 | 71.266 | 0.46x |
| users.json | pysimdjson | 146.927 | 160.424 | 208.466 | 71.266 | 0.05x |
| users.json | json | 16.999 | 23.846 | 31.180 | 71.266 | 0.34x |
| flat.json | strata | 0.585 | 0.608 | 0.823 | 95.828 | 1.00x |
| flat.json | orjson | 0.771 | 0.782 | 0.986 | 95.828 | 0.78x |
| flat.json | msgspec | 0.726 | 0.752 | 0.803 | 95.828 | 0.81x |
| flat.json | ujson | 1.157 | 1.211 | 1.438 | 95.828 | 0.50x |
| flat.json | pysimdjson | 12.059 | 12.268 | 15.206 | 95.828 | 0.05x |
| flat.json | json | 1.384 | 1.443 | 1.615 | 95.828 | 0.42x |
| nested.json | strata | 0.520 | 0.547 | 0.911 | 95.844 | 1.00x |
| nested.json | orjson | 0.743 | 0.766 | 1.012 | 95.844 | 0.71x |
| nested.json | msgspec | 0.695 | 0.706 | 0.840 | 95.844 | 0.78x |
| nested.json | ujson | 1.111 | 1.144 | 2.149 | 95.844 | 0.48x |
| nested.json | pysimdjson | 10.534 | 10.686 | 13.260 | 95.844 | 0.05x |
| nested.json | json | 1.443 | 1.473 | 1.684 | 95.844 | 0.37x |
| wide_arrays.json | strata | 3.073 | 3.186 | 3.846 | 98.625 | 1.00x |
| wide_arrays.json | orjson | 3.757 | 3.925 | 5.346 | 98.625 | 0.81x |
| wide_arrays.json | msgspec | 4.142 | 4.353 | 5.293 | 98.625 | 0.73x |
| wide_arrays.json | ujson | 5.397 | 5.646 | 6.702 | 98.625 | 0.56x |
| wide_arrays.json | pysimdjson | 65.048 | 66.108 | 79.620 | 98.625 | 0.05x |
| wide_arrays.json | json | 7.031 | 7.221 | 8.336 | 98.625 | 0.44x |
| mixed.json | strata | 0.142 | 0.155 | 0.220 | 98.641 | 1.00x |
| mixed.json | orjson | 0.180 | 0.239 | 0.734 | 98.641 | 0.65x |
| mixed.json | msgspec | 0.207 | 0.233 | 0.430 | 98.641 | 0.66x |
| mixed.json | ujson | 0.261 | 0.376 | 0.887 | 98.641 | 0.41x |
| mixed.json | pysimdjson | 2.746 | 3.287 | 4.005 | 98.641 | 0.05x |
| mixed.json | json | 0.368 | 0.464 | 0.739 | 98.641 | 0.33x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.477 | 2.063 | 3.453 | 76.453 | 1.00x |
| users.json | orjson | 2.445 | 2.821 | 3.964 | 76.453 | 0.73x |
| users.json | msgspec | 3.021 | 3.715 | 5.228 | 76.453 | 0.56x |
| users.json | ujson | 10.222 | 12.347 | 17.484 | 76.453 | 0.17x |
| users.json | json | 17.266 | 20.443 | 23.315 | 76.453 | 0.10x |
| flat.json | strata | 0.209 | 0.218 | 0.228 | 95.844 | 1.00x |
| flat.json | orjson | 0.249 | 0.283 | 0.619 | 95.844 | 0.77x |
| flat.json | msgspec | 0.311 | 0.328 | 0.422 | 95.844 | 0.67x |
| flat.json | ujson | 0.748 | 0.771 | 1.148 | 95.844 | 0.28x |
| flat.json | json | 1.385 | 1.480 | 1.602 | 95.844 | 0.15x |
| nested.json | strata | 0.130 | 0.143 | 0.160 | 95.844 | 1.00x |
| nested.json | orjson | 0.238 | 0.252 | 0.287 | 95.844 | 0.57x |
| nested.json | msgspec | 0.298 | 0.353 | 0.527 | 95.844 | 0.41x |
| nested.json | ujson | 1.002 | 1.052 | 1.267 | 95.844 | 0.14x |
| nested.json | json | 1.689 | 1.774 | 2.097 | 95.844 | 0.08x |
| wide_arrays.json | strata | 1.163 | 1.196 | 1.233 | 98.625 | 1.00x |
| wide_arrays.json | orjson | 1.383 | 1.526 | 1.591 | 98.625 | 0.78x |
| wide_arrays.json | msgspec | 2.222 | 2.256 | 2.321 | 98.625 | 0.53x |
| wide_arrays.json | ujson | 5.089 | 5.149 | 5.303 | 98.625 | 0.23x |
| wide_arrays.json | json | 12.097 | 12.251 | 12.418 | 98.625 | 0.10x |
| mixed.json | strata | 0.046 | 0.053 | 0.099 | 98.641 | 1.00x |
| mixed.json | orjson | 0.059 | 0.067 | 0.139 | 98.641 | 0.79x |
| mixed.json | msgspec | 0.069 | 0.074 | 0.088 | 98.641 | 0.71x |
| mixed.json | ujson | 0.183 | 0.228 | 0.302 | 98.641 | 0.23x |
| mixed.json | json | 0.375 | 0.419 | 0.528 | 98.641 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.026 | 7.539 | 9.988 | 89.922 | 1.00x |
| users.json | orjson | 10.009 | 11.309 | 14.598 | 89.922 | 0.67x |
| users.json | msgspec | 9.611 | 10.717 | 18.281 | 89.922 | 0.70x |
| users.json | ujson | 14.744 | 16.110 | 29.173 | 89.922 | 0.47x |
| users.json | json | 16.789 | 17.690 | 26.785 | 89.922 | 0.43x |
| flat.json | strata | 0.594 | 0.598 | 0.616 | 95.844 | 1.00x |
| flat.json | orjson | 0.810 | 0.854 | 0.940 | 95.844 | 0.70x |
| flat.json | msgspec | 0.746 | 0.754 | 0.803 | 95.844 | 0.79x |
| flat.json | ujson | 1.106 | 1.110 | 1.182 | 95.844 | 0.54x |
| flat.json | json | 1.360 | 1.366 | 1.384 | 95.844 | 0.44x |
| nested.json | strata | 0.573 | 0.640 | 0.687 | 95.844 | 1.00x |
| nested.json | orjson | 0.940 | 1.026 | 1.191 | 95.844 | 0.62x |
| nested.json | msgspec | 0.748 | 0.846 | 0.905 | 95.844 | 0.76x |
| nested.json | ujson | 1.055 | 1.143 | 1.426 | 95.844 | 0.56x |
| nested.json | json | 1.481 | 1.617 | 1.730 | 95.844 | 0.40x |
| wide_arrays.json | strata | 3.253 | 3.358 | 3.594 | 98.625 | 1.00x |
| wide_arrays.json | orjson | 3.940 | 4.229 | 4.508 | 98.625 | 0.79x |
| wide_arrays.json | msgspec | 4.486 | 4.648 | 5.702 | 98.625 | 0.72x |
| wide_arrays.json | ujson | 5.866 | 6.172 | 7.237 | 98.625 | 0.54x |
| wide_arrays.json | json | 7.400 | 7.652 | 8.452 | 98.625 | 0.44x |
| mixed.json | strata | 0.176 | 0.195 | 0.210 | 98.641 | 1.00x |
| mixed.json | orjson | 0.266 | 0.292 | 0.329 | 98.641 | 0.67x |
| mixed.json | msgspec | 0.239 | 0.270 | 0.302 | 98.641 | 0.72x |
| mixed.json | ujson | 0.302 | 0.322 | 0.379 | 98.641 | 0.60x |
| mixed.json | json | 0.397 | 0.451 | 0.492 | 98.641 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.893 | 7.359 | 7.667 | 95.828 | 1.00x |
| users.ndjson | orjson | 11.860 | 12.529 | 13.231 | 95.828 | 0.59x |
| users.ndjson | msgspec | 11.859 | 12.614 | 13.669 | 95.828 | 0.58x |
| users.ndjson | ujson | 14.730 | 15.265 | 15.847 | 95.828 | 0.48x |
| users.ndjson | json | 18.842 | 19.481 | 20.362 | 95.828 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.102 | 2.270 | 2.762 | 90.281 | 1.00x |
| users.json | orjson | 2.917 | 3.378 | 3.948 | 90.281 | 0.67x |
| users.json | msgspec | 3.670 | 3.888 | 4.122 | 90.281 | 0.58x |
| users.json | ujson | 9.755 | 10.580 | 11.051 | 90.281 | 0.21x |
| users.json | json | 16.851 | 18.162 | 18.709 | 90.281 | 0.12x |
| flat.json | strata | 0.343 | 0.443 | 0.676 | 95.844 | 1.00x |
| flat.json | orjson | 0.530 | 0.700 | 0.971 | 95.844 | 0.63x |
| flat.json | msgspec | 0.508 | 0.730 | 1.368 | 95.844 | 0.61x |
| flat.json | ujson | 0.920 | 1.301 | 2.538 | 95.844 | 0.34x |
| flat.json | json | 1.670 | 1.870 | 3.297 | 95.844 | 0.24x |
| nested.json | strata | 0.285 | 0.305 | 0.375 | 95.844 | 1.00x |
| nested.json | orjson | 0.371 | 0.449 | 1.019 | 95.844 | 0.68x |
| nested.json | msgspec | 0.427 | 0.543 | 0.826 | 95.844 | 0.56x |
| nested.json | ujson | 0.977 | 1.118 | 1.897 | 95.844 | 0.27x |
| nested.json | json | 1.854 | 1.915 | 2.445 | 95.844 | 0.16x |
| wide_arrays.json | strata | 1.698 | 2.046 | 7.909 | 98.625 | 1.00x |
| wide_arrays.json | orjson | 2.216 | 2.981 | 7.717 | 98.625 | 0.69x |
| wide_arrays.json | msgspec | 3.027 | 3.424 | 8.868 | 98.625 | 0.60x |
| wide_arrays.json | ujson | 6.416 | 7.256 | 14.139 | 98.625 | 0.28x |
| wide_arrays.json | json | 14.247 | 15.677 | 27.427 | 98.625 | 0.13x |
| mixed.json | strata | 0.175 | 0.209 | 0.779 | 98.641 | 1.00x |
| mixed.json | orjson | 0.192 | 0.259 | 0.313 | 98.641 | 0.81x |
| mixed.json | msgspec | 0.195 | 0.235 | 1.638 | 98.641 | 0.89x |
| mixed.json | ujson | 0.331 | 0.372 | 0.773 | 98.641 | 0.56x |
| mixed.json | json | 0.502 | 0.590 | 0.742 | 98.641 | 0.36x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.053 | 0.056 | 0.127 | 90.328 | 1.00x |
| users.json $[*].id | jmespath | 0.276 | 0.293 | 0.402 | 90.328 | 0.19x |
| users.json $[*].id | jsonpath-ng | 1.526 | 1.558 | 1.767 | 90.328 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.347 | 0.730 | 1.083 | 90.391 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.919 | 2.691 | 4.648 | 90.391 | 0.27x |
| users.json $[*].orders[*].total | jsonpath-ng | 10.824 | 15.429 | 18.068 | 90.391 | 0.05x |
| users.json $..total | strata | 1.330 | 1.511 | 2.250 | 90.406 | 1.00x |
| users.json $..total | jsonpath-ng | 194.088 | 210.052 | 243.769 | 90.406 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.921 | 4.250 | 4.955 | 90.359 | 1.00x |
| users.json $[*].id | orjson+jmespath | 11.509 | 13.441 | 14.907 | 90.359 | 0.32x |
| users.json $[*].id | orjson+jsonpath-ng | 13.224 | 13.858 | 18.134 | 90.359 | 0.31x |
| users.json $[*].orders[*].total | strata | 3.549 | 4.043 | 4.742 | 90.406 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 10.774 | 14.719 | 16.085 | 90.406 | 0.27x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 21.659 | 29.536 | 35.723 | 90.406 | 0.14x |
| users.json $..total | strata | 7.898 | 8.623 | 9.868 | 90.406 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 199.794 | 212.939 | 241.495 | 90.406 | 0.04x |

