# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c20ac86eedff410e10c973bc3b1f19f6e9a5f56e
- python: 3.12.10
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit
- machine: arm64
- processor: Apple M2 Pro (Virtual)
- compiler_flags: -fno-strict-overflow -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -O3 -Wall -arch arm64 -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -march=native -flto=thin -fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata -bundle -undefined (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 5.847 | 5.949 | 6.322 | 68.672 | 1.00x |
| users.json | orjson | 8.839 | 9.576 | 10.385 | 68.672 | 0.62x |
| users.json | msgspec | 8.427 | 9.271 | 10.123 | 68.672 | 0.64x |
| users.json | ujson | 11.247 | 12.381 | 13.807 | 68.672 | 0.48x |
| users.json | pysimdjson | 118.908 | 122.671 | 128.969 | 68.672 | 0.05x |
| users.json | json | 13.836 | 15.202 | 20.258 | 68.672 | 0.39x |
| flat.json | strata | 0.507 | 0.555 | 0.653 | 103.531 | 1.00x |
| flat.json | orjson | 0.647 | 0.658 | 0.692 | 103.531 | 0.84x |
| flat.json | msgspec | 0.639 | 0.672 | 0.752 | 103.531 | 0.83x |
| flat.json | ujson | 0.974 | 1.046 | 1.110 | 103.531 | 0.53x |
| flat.json | pysimdjson | 11.213 | 11.681 | 11.739 | 103.531 | 0.05x |
| flat.json | json | 1.205 | 1.258 | 1.296 | 103.531 | 0.44x |
| nested.json | strata | 0.432 | 0.476 | 0.495 | 103.594 | 1.00x |
| nested.json | orjson | 0.634 | 0.691 | 0.830 | 103.594 | 0.69x |
| nested.json | msgspec | 0.600 | 0.632 | 0.669 | 103.594 | 0.75x |
| nested.json | ujson | 0.869 | 0.943 | 1.014 | 103.594 | 0.50x |
| nested.json | pysimdjson | 9.832 | 10.324 | 10.736 | 103.594 | 0.05x |
| nested.json | json | 1.275 | 1.358 | 1.454 | 103.594 | 0.35x |
| wide_arrays.json | strata | 2.724 | 2.865 | 3.039 | 107.938 | 1.00x |
| wide_arrays.json | orjson | 3.325 | 3.486 | 3.801 | 107.938 | 0.82x |
| wide_arrays.json | msgspec | 3.566 | 3.783 | 4.207 | 107.938 | 0.76x |
| wide_arrays.json | ujson | 4.723 | 5.032 | 5.288 | 107.938 | 0.57x |
| wide_arrays.json | pysimdjson | 61.660 | 63.538 | 64.371 | 107.938 | 0.05x |
| wide_arrays.json | json | 6.052 | 6.285 | 6.542 | 107.938 | 0.46x |
| mixed.json | strata | 0.110 | 0.111 | 0.124 | 107.953 | 1.00x |
| mixed.json | orjson | 0.142 | 0.143 | 0.156 | 107.953 | 0.78x |
| mixed.json | msgspec | 0.156 | 0.158 | 0.171 | 107.953 | 0.70x |
| mixed.json | ujson | 0.191 | 0.197 | 0.222 | 107.953 | 0.57x |
| mixed.json | pysimdjson | 2.465 | 2.474 | 2.545 | 107.953 | 0.04x |
| mixed.json | json | 0.300 | 0.306 | 0.330 | 107.953 | 0.36x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.531 | 1.567 | 1.604 | 75.453 | 1.00x |
| users.json | orjson | 2.157 | 2.281 | 2.496 | 75.453 | 0.69x |
| users.json | msgspec | 2.686 | 2.762 | 2.959 | 75.453 | 0.57x |
| users.json | ujson | 8.050 | 8.360 | 8.474 | 75.453 | 0.19x |
| users.json | json | 15.051 | 15.180 | 15.543 | 75.453 | 0.10x |
| flat.json | strata | 0.181 | 0.195 | 0.211 | 103.578 | 1.00x |
| flat.json | orjson | 0.218 | 0.228 | 0.244 | 103.578 | 0.86x |
| flat.json | msgspec | 0.281 | 0.285 | 0.290 | 103.578 | 0.68x |
| flat.json | ujson | 0.655 | 0.675 | 0.687 | 103.578 | 0.29x |
| flat.json | json | 1.226 | 1.267 | 1.301 | 103.578 | 0.15x |
| nested.json | strata | 0.111 | 0.117 | 0.133 | 103.609 | 1.00x |
| nested.json | orjson | 0.195 | 0.204 | 0.233 | 103.609 | 0.57x |
| nested.json | msgspec | 0.251 | 0.258 | 0.274 | 103.609 | 0.45x |
| nested.json | ujson | 0.748 | 0.782 | 0.855 | 103.609 | 0.15x |
| nested.json | json | 1.453 | 1.492 | 1.567 | 103.609 | 0.08x |
| wide_arrays.json | strata | 0.984 | 1.098 | 1.213 | 107.938 | 1.00x |
| wide_arrays.json | orjson | 1.424 | 1.501 | 1.687 | 107.938 | 0.73x |
| wide_arrays.json | msgspec | 1.986 | 2.053 | 2.218 | 107.938 | 0.53x |
| wide_arrays.json | ujson | 4.454 | 4.649 | 4.987 | 107.938 | 0.24x |
| wide_arrays.json | json | 10.745 | 11.419 | 11.620 | 107.938 | 0.10x |
| mixed.json | strata | 0.033 | 0.034 | 0.035 | 107.953 | 1.00x |
| mixed.json | orjson | 0.041 | 0.044 | 0.060 | 107.953 | 0.78x |
| mixed.json | msgspec | 0.048 | 0.050 | 0.054 | 107.953 | 0.68x |
| mixed.json | ujson | 0.161 | 0.163 | 0.176 | 107.953 | 0.21x |
| mixed.json | json | 0.328 | 0.331 | 0.383 | 107.953 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.057 | 6.508 | 6.857 | 88.797 | 1.00x |
| users.json | orjson | 9.577 | 10.001 | 10.250 | 88.797 | 0.65x |
| users.json | msgspec | 9.169 | 10.049 | 10.712 | 88.797 | 0.65x |
| users.json | ujson | 13.257 | 13.498 | 14.042 | 88.797 | 0.48x |
| users.json | json | 15.046 | 15.751 | 15.885 | 88.797 | 0.41x |
| flat.json | strata | 0.530 | 0.572 | 0.619 | 103.578 | 1.00x |
| flat.json | orjson | 0.701 | 0.746 | 0.907 | 103.578 | 0.77x |
| flat.json | msgspec | 0.683 | 0.733 | 0.771 | 103.578 | 0.78x |
| flat.json | ujson | 1.000 | 1.084 | 1.108 | 103.578 | 0.53x |
| flat.json | json | 1.230 | 1.308 | 1.330 | 103.578 | 0.44x |
| nested.json | strata | 0.459 | 0.498 | 0.518 | 103.609 | 1.00x |
| nested.json | orjson | 0.696 | 0.763 | 0.877 | 103.609 | 0.65x |
| nested.json | msgspec | 0.646 | 0.682 | 0.823 | 103.609 | 0.73x |
| nested.json | ujson | 0.890 | 0.970 | 1.048 | 103.609 | 0.51x |
| nested.json | json | 1.286 | 1.393 | 1.428 | 103.609 | 0.36x |
| wide_arrays.json | strata | 2.818 | 2.962 | 3.040 | 107.938 | 1.00x |
| wide_arrays.json | orjson | 3.489 | 3.701 | 3.972 | 107.938 | 0.80x |
| wide_arrays.json | msgspec | 3.797 | 4.043 | 4.247 | 107.938 | 0.73x |
| wide_arrays.json | ujson | 5.057 | 5.408 | 5.998 | 107.938 | 0.55x |
| wide_arrays.json | json | 6.178 | 6.450 | 6.700 | 107.938 | 0.46x |
| mixed.json | strata | 0.131 | 0.137 | 0.141 | 107.953 | 1.00x |
| mixed.json | orjson | 0.170 | 0.180 | 0.198 | 107.953 | 0.76x |
| mixed.json | msgspec | 0.185 | 0.194 | 0.209 | 107.953 | 0.71x |
| mixed.json | ujson | 0.231 | 0.242 | 0.257 | 107.953 | 0.57x |
| mixed.json | json | 0.324 | 0.347 | 0.364 | 107.953 | 0.39x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.661 | 6.887 | 7.206 | 102.953 | 1.00x |
| users.ndjson | orjson | 11.792 | 12.225 | 12.489 | 102.953 | 0.56x |
| users.ndjson | msgspec | 11.576 | 11.998 | 12.430 | 102.953 | 0.57x |
| users.ndjson | ujson | 13.798 | 15.057 | 15.284 | 102.953 | 0.46x |
| users.ndjson | json | 17.527 | 18.995 | 19.350 | 102.953 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.706 | 1.872 | 2.046 | 98.750 | 1.00x |
| users.json | orjson | 2.422 | 2.671 | 2.844 | 98.750 | 0.70x |
| users.json | msgspec | 3.145 | 3.451 | 4.303 | 98.750 | 0.54x |
| users.json | ujson | 8.871 | 9.114 | 10.135 | 98.750 | 0.21x |
| users.json | json | 14.813 | 15.578 | 25.710 | 98.750 | 0.12x |
| flat.json | strata | 0.300 | 0.331 | 0.464 | 103.578 | 1.00x |
| flat.json | orjson | 0.359 | 0.408 | 0.486 | 103.578 | 0.81x |
| flat.json | msgspec | 0.418 | 0.451 | 1.052 | 103.578 | 0.73x |
| flat.json | ujson | 0.818 | 0.890 | 0.972 | 103.578 | 0.37x |
| flat.json | json | 1.331 | 1.435 | 1.559 | 103.578 | 0.23x |
| nested.json | strata | 0.239 | 0.271 | 0.375 | 103.609 | 1.00x |
| nested.json | orjson | 0.304 | 0.350 | 0.429 | 103.609 | 0.77x |
| nested.json | msgspec | 0.429 | 0.488 | 0.575 | 103.609 | 0.56x |
| nested.json | ujson | 0.863 | 0.898 | 1.017 | 103.609 | 0.30x |
| nested.json | json | 1.627 | 1.670 | 1.899 | 103.609 | 0.16x |
| wide_arrays.json | strata | 1.245 | 1.476 | 1.601 | 107.938 | 1.00x |
| wide_arrays.json | orjson | 1.429 | 1.745 | 1.995 | 107.938 | 0.85x |
| wide_arrays.json | msgspec | 2.095 | 2.484 | 2.771 | 107.938 | 0.59x |
| wide_arrays.json | ujson | 4.752 | 4.884 | 5.085 | 107.938 | 0.30x |
| wide_arrays.json | json | 11.197 | 11.757 | 11.985 | 107.938 | 0.13x |
| mixed.json | strata | 0.114 | 0.168 | 0.247 | 107.953 | 1.00x |
| mixed.json | orjson | 0.128 | 0.161 | 0.404 | 107.953 | 1.04x |
| mixed.json | msgspec | 0.147 | 0.158 | 0.244 | 107.953 | 1.07x |
| mixed.json | ujson | 0.253 | 0.292 | 0.629 | 107.953 | 0.58x |
| mixed.json | json | 0.424 | 0.457 | 0.595 | 107.953 | 0.37x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.039 | 0.041 | 0.055 | 98.797 | 1.00x |
| users.json $[*].id | jmespath | 0.234 | 0.247 | 0.297 | 98.797 | 0.17x |
| users.json $[*].id | jsonpath-ng | 1.347 | 1.387 | 1.504 | 98.797 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.268 | 0.283 | 0.306 | 98.938 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.511 | 1.537 | 1.591 | 98.938 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 10.495 | 10.743 | 11.443 | 98.938 | 0.03x |
| users.json $..total | strata | 1.185 | 1.315 | 2.331 | 99.203 | 1.00x |
| users.json $..total | jsonpath-ng | 178.317 | 184.367 | 200.181 | 99.203 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.276 | 3.438 | 3.526 | 98.875 | 1.00x |
| users.json $[*].id | orjson+jmespath | 9.708 | 10.427 | 11.144 | 98.875 | 0.33x |
| users.json $[*].id | orjson+jsonpath-ng | 10.816 | 11.585 | 11.693 | 98.875 | 0.30x |
| users.json $[*].orders[*].total | strata | 3.331 | 3.530 | 3.725 | 98.984 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 11.116 | 11.471 | 12.471 | 98.984 | 0.31x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 21.263 | 23.036 | 24.882 | 98.984 | 0.15x |
| users.json $..total | strata | 7.869 | 8.518 | 9.046 | 99.297 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 192.445 | 204.644 | 206.746 | 99.297 | 0.04x |

