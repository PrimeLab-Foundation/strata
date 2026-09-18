# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: baba505678994faef24df367a2dc4b93d5d4bef0
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
| users.json | strata | 6.476 | 7.290 | 13.284 | 67.938 | 1.00x |
| users.json | orjson | 9.656 | 11.512 | 25.467 | 67.938 | 0.63x |
| users.json | msgspec | 9.465 | 10.698 | 15.320 | 67.938 | 0.68x |
| users.json | ujson | 12.282 | 14.563 | 21.648 | 67.938 | 0.50x |
| users.json | pysimdjson | 129.138 | 140.643 | 183.924 | 67.938 | 0.05x |
| users.json | json | 15.691 | 16.475 | 28.043 | 67.938 | 0.44x |
| flat.json | strata | 0.572 | 0.618 | 0.762 | 97.172 | 1.00x |
| flat.json | orjson | 0.732 | 0.778 | 0.838 | 97.172 | 0.79x |
| flat.json | msgspec | 0.733 | 0.766 | 0.784 | 97.172 | 0.81x |
| flat.json | ujson | 1.174 | 1.257 | 1.397 | 97.172 | 0.49x |
| flat.json | pysimdjson | 12.161 | 12.419 | 12.943 | 97.172 | 0.05x |
| flat.json | json | 1.323 | 1.414 | 1.690 | 97.172 | 0.44x |
| nested.json | strata | 0.529 | 0.551 | 0.568 | 97.203 | 1.00x |
| nested.json | orjson | 0.739 | 0.753 | 0.842 | 97.203 | 0.73x |
| nested.json | msgspec | 0.700 | 0.716 | 0.739 | 97.203 | 0.77x |
| nested.json | ujson | 1.060 | 1.139 | 1.267 | 97.203 | 0.48x |
| nested.json | pysimdjson | 10.659 | 10.823 | 12.053 | 97.203 | 0.05x |
| nested.json | json | 1.454 | 1.521 | 1.562 | 97.203 | 0.36x |
| wide_arrays.json | strata | 2.928 | 3.268 | 4.445 | 99.781 | 1.00x |
| wide_arrays.json | orjson | 3.565 | 3.908 | 4.216 | 99.781 | 0.84x |
| wide_arrays.json | msgspec | 3.975 | 4.309 | 6.386 | 99.781 | 0.76x |
| wide_arrays.json | ujson | 5.340 | 5.635 | 6.298 | 99.781 | 0.58x |
| wide_arrays.json | pysimdjson | 62.248 | 66.526 | 73.198 | 99.781 | 0.05x |
| wide_arrays.json | json | 6.632 | 7.193 | 12.677 | 99.781 | 0.45x |
| mixed.json | strata | 0.137 | 0.150 | 0.173 | 104.469 | 1.00x |
| mixed.json | orjson | 0.171 | 0.193 | 0.532 | 104.469 | 0.78x |
| mixed.json | msgspec | 0.182 | 0.197 | 0.302 | 104.469 | 0.76x |
| mixed.json | ujson | 0.235 | 0.263 | 0.596 | 104.469 | 0.57x |
| mixed.json | pysimdjson | 2.699 | 2.766 | 3.244 | 104.469 | 0.05x |
| mixed.json | json | 0.348 | 0.361 | 0.400 | 104.469 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.444 | 1.567 | 1.680 | 76.656 | 1.00x |
| users.json | orjson | 2.253 | 2.443 | 2.674 | 76.656 | 0.64x |
| users.json | msgspec | 2.849 | 2.997 | 3.267 | 76.656 | 0.52x |
| users.json | ujson | 8.676 | 9.247 | 9.439 | 76.656 | 0.17x |
| users.json | json | 15.397 | 16.083 | 17.452 | 76.656 | 0.10x |
| flat.json | strata | 0.224 | 0.238 | 0.259 | 97.172 | 1.00x |
| flat.json | orjson | 0.262 | 0.273 | 0.330 | 97.172 | 0.87x |
| flat.json | msgspec | 0.326 | 0.337 | 0.374 | 97.172 | 0.71x |
| flat.json | ujson | 0.798 | 0.821 | 0.886 | 97.172 | 0.29x |
| flat.json | json | 1.510 | 1.580 | 2.205 | 97.172 | 0.15x |
| nested.json | strata | 0.133 | 0.140 | 0.203 | 97.203 | 1.00x |
| nested.json | orjson | 0.233 | 0.268 | 0.392 | 97.203 | 0.52x |
| nested.json | msgspec | 0.315 | 0.464 | 0.590 | 97.203 | 0.30x |
| nested.json | ujson | 0.906 | 0.964 | 2.176 | 97.203 | 0.15x |
| nested.json | json | 1.681 | 1.762 | 1.903 | 97.203 | 0.08x |
| wide_arrays.json | strata | 1.129 | 1.307 | 1.582 | 99.781 | 1.00x |
| wide_arrays.json | orjson | 1.397 | 1.582 | 3.204 | 99.781 | 0.83x |
| wide_arrays.json | msgspec | 2.289 | 2.567 | 4.467 | 99.781 | 0.51x |
| wide_arrays.json | ujson | 4.943 | 5.364 | 10.071 | 99.781 | 0.24x |
| wide_arrays.json | json | 12.303 | 13.225 | 21.590 | 99.781 | 0.10x |
| mixed.json | strata | 0.043 | 0.046 | 0.052 | 104.469 | 1.00x |
| mixed.json | orjson | 0.052 | 0.060 | 0.065 | 104.469 | 0.76x |
| mixed.json | msgspec | 0.062 | 0.096 | 0.297 | 104.469 | 0.47x |
| mixed.json | ujson | 0.189 | 0.207 | 0.209 | 104.469 | 0.22x |
| mixed.json | json | 0.385 | 0.429 | 0.480 | 104.469 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.655 | 6.779 | 7.301 | 88.312 | 1.00x |
| users.json | orjson | 9.441 | 10.194 | 10.523 | 88.312 | 0.67x |
| users.json | msgspec | 9.155 | 9.814 | 9.967 | 88.312 | 0.69x |
| users.json | ujson | 12.719 | 13.425 | 14.118 | 88.312 | 0.50x |
| users.json | json | 15.282 | 15.945 | 17.116 | 88.312 | 0.43x |
| flat.json | strata | 0.634 | 0.693 | 2.084 | 97.203 | 1.00x |
| flat.json | orjson | 0.893 | 0.991 | 1.688 | 97.203 | 0.70x |
| flat.json | msgspec | 0.853 | 0.916 | 0.958 | 97.203 | 0.76x |
| flat.json | ujson | 1.202 | 1.251 | 1.330 | 97.203 | 0.55x |
| flat.json | json | 1.419 | 1.509 | 1.703 | 97.203 | 0.46x |
| nested.json | strata | 0.597 | 0.648 | 1.045 | 97.203 | 1.00x |
| nested.json | orjson | 0.920 | 1.010 | 1.139 | 97.203 | 0.64x |
| nested.json | msgspec | 0.793 | 0.867 | 0.970 | 97.203 | 0.75x |
| nested.json | ujson | 1.113 | 1.185 | 1.303 | 97.203 | 0.55x |
| nested.json | json | 1.586 | 1.651 | 1.769 | 97.203 | 0.39x |
| wide_arrays.json | strata | 3.520 | 3.698 | 5.389 | 103.375 | 1.00x |
| wide_arrays.json | orjson | 4.139 | 4.492 | 5.557 | 103.375 | 0.82x |
| wide_arrays.json | msgspec | 4.809 | 5.001 | 6.580 | 103.375 | 0.74x |
| wide_arrays.json | ujson | 6.186 | 6.451 | 7.799 | 103.375 | 0.57x |
| wide_arrays.json | json | 7.725 | 8.453 | 12.922 | 103.375 | 0.44x |
| mixed.json | strata | 0.158 | 0.192 | 0.255 | 104.469 | 1.00x |
| mixed.json | orjson | 0.214 | 0.366 | 0.591 | 104.469 | 0.52x |
| mixed.json | msgspec | 0.237 | 0.265 | 0.310 | 104.469 | 0.72x |
| mixed.json | ujson | 0.288 | 0.315 | 0.386 | 104.469 | 0.61x |
| mixed.json | json | 0.395 | 0.423 | 0.465 | 104.469 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.505 | 7.054 | 7.937 | 96.547 | 1.00x |
| users.ndjson | orjson | 11.334 | 12.250 | 13.299 | 96.547 | 0.58x |
| users.ndjson | msgspec | 11.352 | 12.084 | 12.378 | 96.547 | 0.58x |
| users.ndjson | ujson | 13.861 | 14.874 | 16.155 | 96.547 | 0.47x |
| users.ndjson | json | 17.859 | 19.299 | 21.036 | 96.547 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.732 | 2.013 | 2.510 | 92.609 | 1.00x |
| users.json | orjson | 2.745 | 2.909 | 3.220 | 92.609 | 0.69x |
| users.json | msgspec | 3.384 | 3.591 | 4.187 | 92.609 | 0.56x |
| users.json | ujson | 9.287 | 9.924 | 10.384 | 92.609 | 0.20x |
| users.json | json | 15.771 | 17.070 | 18.357 | 92.609 | 0.12x |
| flat.json | strata | 0.412 | 0.513 | 0.649 | 97.203 | 1.00x |
| flat.json | orjson | 0.462 | 0.591 | 0.812 | 97.203 | 0.87x |
| flat.json | msgspec | 0.541 | 0.684 | 0.832 | 97.203 | 0.75x |
| flat.json | ujson | 1.043 | 1.166 | 1.287 | 97.203 | 0.44x |
| flat.json | json | 1.628 | 1.857 | 2.061 | 97.203 | 0.28x |
| nested.json | strata | 0.332 | 0.397 | 0.606 | 97.203 | 1.00x |
| nested.json | orjson | 0.467 | 0.516 | 0.613 | 97.203 | 0.77x |
| nested.json | msgspec | 0.507 | 0.688 | 0.830 | 97.203 | 0.58x |
| nested.json | ujson | 1.109 | 1.413 | 1.742 | 97.203 | 0.28x |
| nested.json | json | 1.951 | 2.084 | 3.241 | 97.203 | 0.19x |
| wide_arrays.json | strata | 1.536 | 1.715 | 1.914 | 104.453 | 1.00x |
| wide_arrays.json | orjson | 2.005 | 2.287 | 2.936 | 104.453 | 0.75x |
| wide_arrays.json | msgspec | 2.857 | 3.077 | 3.791 | 104.453 | 0.56x |
| wide_arrays.json | ujson | 5.922 | 6.733 | 7.648 | 104.453 | 0.25x |
| wide_arrays.json | json | 13.331 | 14.083 | 14.880 | 104.453 | 0.12x |
| mixed.json | strata | 0.159 | 0.224 | 0.290 | 104.469 | 1.00x |
| mixed.json | orjson | 0.186 | 0.246 | 0.385 | 104.469 | 0.91x |
| mixed.json | msgspec | 0.204 | 0.273 | 0.564 | 104.469 | 0.82x |
| mixed.json | ujson | 0.346 | 0.361 | 0.455 | 104.469 | 0.62x |
| mixed.json | json | 0.538 | 0.584 | 0.653 | 104.469 | 0.38x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.052 | 0.070 | 0.106 | 92.641 | 1.00x |
| users.json $[*].id | jmespath | 0.266 | 0.311 | 0.508 | 92.641 | 0.23x |
| users.json $[*].id | jsonpath-ng | 1.443 | 1.708 | 2.482 | 92.641 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.389 | 0.491 | 1.044 | 92.812 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.787 | 2.061 | 3.189 | 92.812 | 0.24x |
| users.json $[*].orders[*].total | jsonpath-ng | 11.359 | 11.654 | 19.785 | 92.812 | 0.04x |
| users.json $..total | strata | 1.348 | 1.477 | 1.769 | 92.844 | 1.00x |
| users.json $..total | jsonpath-ng | 190.033 | 199.184 | 234.476 | 92.844 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.544 | 3.813 | 4.324 | 92.703 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.049 | 10.688 | 11.606 | 92.703 | 0.36x |
| users.json $[*].id | orjson+jsonpath-ng | 11.609 | 12.330 | 15.533 | 92.703 | 0.31x |
| users.json $[*].orders[*].total | strata | 3.727 | 3.899 | 4.150 | 92.812 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 11.959 | 12.342 | 14.079 | 92.812 | 0.32x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 22.902 | 23.549 | 25.403 | 92.812 | 0.17x |
| users.json $..total | strata | 8.079 | 8.538 | 10.056 | 92.891 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 204.427 | 214.056 | 229.225 | 92.891 | 0.04x |

