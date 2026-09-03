# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 571381e
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
| users.json | strata | 7.679 | 7.849 | 8.219 | 63.438 | 1.00x |
| users.json | orjson | 10.490 | 11.100 | 11.947 | 63.438 | 0.71x |
| users.json | msgspec | 9.838 | 10.754 | 12.135 | 63.438 | 0.73x |
| users.json | ujson | 12.168 | 14.901 | 16.012 | 63.438 | 0.53x |
| users.json | pysimdjson | 134.753 | 139.735 | 158.387 | 63.438 | 0.06x |
| users.json | json | 15.929 | 17.129 | 19.588 | 63.438 | 0.46x |
| flat.json | strata | 0.595 | 0.638 | 0.672 | 90.406 | 1.00x |
| flat.json | orjson | 0.704 | 0.781 | 0.807 | 90.406 | 0.82x |
| flat.json | msgspec | 0.694 | 0.755 | 0.791 | 90.406 | 0.85x |
| flat.json | ujson | 1.065 | 1.223 | 1.330 | 90.406 | 0.52x |
| flat.json | pysimdjson | 12.070 | 12.938 | 13.079 | 90.406 | 0.05x |
| flat.json | json | 1.342 | 1.440 | 1.474 | 90.406 | 0.44x |
| nested.json | strata | 0.564 | 0.580 | 0.619 | 90.422 | 1.00x |
| nested.json | orjson | 0.782 | 0.795 | 0.838 | 90.422 | 0.73x |
| nested.json | msgspec | 0.742 | 0.759 | 0.799 | 90.422 | 0.76x |
| nested.json | ujson | 1.147 | 1.203 | 1.350 | 90.422 | 0.48x |
| nested.json | pysimdjson | 11.026 | 11.291 | 11.389 | 90.422 | 0.05x |
| nested.json | json | 1.419 | 1.572 | 1.596 | 90.422 | 0.37x |
| wide_arrays.json | strata | 3.561 | 4.168 | 5.272 | 93.531 | 1.00x |
| wide_arrays.json | orjson | 3.817 | 4.357 | 9.707 | 93.531 | 0.96x |
| wide_arrays.json | msgspec | 4.316 | 4.980 | 5.220 | 93.531 | 0.84x |
| wide_arrays.json | ujson | 5.912 | 6.694 | 12.014 | 93.531 | 0.62x |
| wide_arrays.json | pysimdjson | 71.636 | 74.907 | 85.931 | 93.531 | 0.06x |
| wide_arrays.json | json | 7.688 | 8.239 | 10.595 | 93.531 | 0.51x |
| mixed.json | strata | 0.128 | 0.133 | 0.153 | 93.547 | 1.00x |
| mixed.json | orjson | 0.151 | 0.163 | 0.187 | 93.547 | 0.82x |
| mixed.json | msgspec | 0.168 | 0.175 | 0.205 | 93.547 | 0.76x |
| mixed.json | ujson | 0.207 | 0.299 | 0.438 | 93.547 | 0.45x |
| mixed.json | pysimdjson | 2.498 | 2.537 | 2.803 | 93.547 | 0.05x |
| mixed.json | json | 0.318 | 0.346 | 0.363 | 93.547 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.662 | 1.787 | 2.132 | 74.500 | 1.00x |
| users.json | orjson | 2.198 | 2.421 | 2.657 | 74.500 | 0.74x |
| users.json | msgspec | 2.865 | 3.031 | 3.401 | 74.500 | 0.59x |
| users.json | ujson | 10.991 | 11.324 | 12.200 | 74.500 | 0.16x |
| users.json | json | 15.596 | 16.169 | 17.599 | 74.500 | 0.11x |
| flat.json | strata | 0.215 | 0.227 | 0.237 | 90.422 | 1.00x |
| flat.json | orjson | 0.466 | 0.508 | 0.551 | 90.422 | 0.45x |
| flat.json | msgspec | 0.353 | 0.368 | 0.409 | 90.422 | 0.62x |
| flat.json | ujson | 1.019 | 1.036 | 1.315 | 90.422 | 0.22x |
| flat.json | json | 1.475 | 1.510 | 1.605 | 90.422 | 0.15x |
| nested.json | strata | 0.128 | 0.132 | 0.138 | 90.438 | 1.00x |
| nested.json | orjson | 0.215 | 0.220 | 0.235 | 90.438 | 0.60x |
| nested.json | msgspec | 0.283 | 0.285 | 0.292 | 90.438 | 0.46x |
| nested.json | ujson | 1.104 | 1.120 | 1.247 | 90.438 | 0.12x |
| nested.json | json | 1.610 | 1.631 | 1.667 | 90.438 | 0.08x |
| wide_arrays.json | strata | 1.279 | 1.416 | 1.466 | 93.531 | 1.00x |
| wide_arrays.json | orjson | 1.482 | 1.644 | 1.808 | 93.531 | 0.86x |
| wide_arrays.json | msgspec | 2.282 | 2.544 | 2.756 | 93.531 | 0.56x |
| wide_arrays.json | ujson | 7.539 | 7.714 | 8.272 | 93.531 | 0.18x |
| wide_arrays.json | json | 13.168 | 13.607 | 16.851 | 93.531 | 0.10x |
| mixed.json | strata | 0.037 | 0.039 | 0.046 | 93.547 | 1.00x |
| mixed.json | orjson | 0.041 | 0.045 | 0.051 | 93.547 | 0.88x |
| mixed.json | msgspec | 0.047 | 0.083 | 0.223 | 93.547 | 0.47x |
| mixed.json | ujson | 0.212 | 0.220 | 0.230 | 93.547 | 0.18x |
| mixed.json | json | 0.341 | 0.349 | 0.394 | 93.547 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.148 | 7.706 | 8.496 | 87.453 | 1.00x |
| users.json | orjson | 9.544 | 10.092 | 11.315 | 87.453 | 0.76x |
| users.json | msgspec | 9.232 | 9.757 | 11.006 | 87.453 | 0.79x |
| users.json | ujson | 12.500 | 13.055 | 15.721 | 87.453 | 0.59x |
| users.json | json | 14.852 | 15.481 | 17.379 | 87.453 | 0.50x |
| flat.json | strata | 0.689 | 0.701 | 0.741 | 90.422 | 1.00x |
| flat.json | orjson | 0.945 | 1.037 | 1.096 | 90.422 | 0.68x |
| flat.json | msgspec | 0.835 | 0.868 | 0.916 | 90.422 | 0.81x |
| flat.json | ujson | 1.234 | 1.269 | 1.535 | 90.422 | 0.55x |
| flat.json | json | 1.499 | 1.552 | 1.969 | 90.422 | 0.45x |
| nested.json | strata | 0.557 | 0.611 | 0.674 | 90.438 | 1.00x |
| nested.json | orjson | 0.875 | 1.033 | 1.168 | 90.438 | 0.59x |
| nested.json | msgspec | 0.730 | 0.849 | 0.956 | 90.438 | 0.72x |
| nested.json | ujson | 1.016 | 1.158 | 1.230 | 90.438 | 0.53x |
| nested.json | json | 1.437 | 1.552 | 1.751 | 90.438 | 0.39x |
| wide_arrays.json | strata | 4.024 | 4.141 | 4.350 | 93.531 | 1.00x |
| wide_arrays.json | orjson | 4.214 | 4.439 | 5.495 | 93.531 | 0.93x |
| wide_arrays.json | msgspec | 4.887 | 5.103 | 6.705 | 93.531 | 0.81x |
| wide_arrays.json | ujson | 6.518 | 6.766 | 7.726 | 93.531 | 0.61x |
| wide_arrays.json | json | 7.947 | 8.103 | 8.413 | 93.531 | 0.51x |
| mixed.json | strata | 0.149 | 0.159 | 0.254 | 93.547 | 1.00x |
| mixed.json | orjson | 0.201 | 0.297 | 0.614 | 93.547 | 0.54x |
| mixed.json | msgspec | 0.205 | 0.232 | 0.318 | 93.547 | 0.69x |
| mixed.json | ujson | 0.246 | 0.285 | 0.373 | 93.547 | 0.56x |
| mixed.json | json | 0.349 | 0.375 | 0.497 | 93.547 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.253 | 8.110 | 8.481 | 90.391 | 1.00x |
| users.ndjson | orjson | 11.793 | 12.929 | 13.848 | 90.391 | 0.63x |
| users.ndjson | msgspec | 11.389 | 12.616 | 13.814 | 90.391 | 0.64x |
| users.ndjson | ujson | 13.927 | 15.941 | 16.970 | 90.391 | 0.51x |
| users.ndjson | json | 18.454 | 20.787 | 21.233 | 90.391 | 0.39x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.995 | 2.178 | 2.528 | 87.578 | 1.00x |
| users.json | orjson | 2.595 | 2.814 | 2.863 | 87.578 | 0.77x |
| users.json | msgspec | 3.265 | 3.428 | 3.622 | 87.578 | 0.64x |
| users.json | ujson | 11.438 | 11.917 | 13.059 | 87.578 | 0.18x |
| users.json | json | 16.248 | 17.149 | 18.385 | 87.578 | 0.13x |
| flat.json | strata | 0.346 | 0.370 | 0.466 | 90.422 | 1.00x |
| flat.json | orjson | 0.417 | 0.437 | 0.555 | 90.422 | 0.85x |
| flat.json | msgspec | 0.477 | 0.497 | 0.578 | 90.422 | 0.75x |
| flat.json | ujson | 1.140 | 1.164 | 1.264 | 90.422 | 0.32x |
| flat.json | json | 1.617 | 1.675 | 1.804 | 90.422 | 0.22x |
| nested.json | strata | 0.271 | 0.349 | 0.385 | 90.438 | 1.00x |
| nested.json | orjson | 0.363 | 0.449 | 0.566 | 90.438 | 0.78x |
| nested.json | msgspec | 0.436 | 0.591 | 0.742 | 90.438 | 0.59x |
| nested.json | ujson | 1.276 | 1.437 | 1.690 | 90.438 | 0.24x |
| nested.json | json | 1.911 | 2.003 | 2.171 | 90.438 | 0.17x |
| wide_arrays.json | strata | 1.457 | 1.701 | 7.536 | 93.531 | 1.00x |
| wide_arrays.json | orjson | 1.731 | 1.997 | 5.392 | 93.531 | 0.85x |
| wide_arrays.json | msgspec | 2.689 | 2.785 | 3.002 | 93.531 | 0.61x |
| wide_arrays.json | ujson | 7.399 | 7.755 | 8.072 | 93.531 | 0.22x |
| wide_arrays.json | json | 13.110 | 13.215 | 19.650 | 93.531 | 0.13x |
| mixed.json | strata | 0.122 | 0.135 | 0.169 | 93.547 | 1.00x |
| mixed.json | orjson | 0.142 | 0.154 | 0.415 | 93.547 | 0.88x |
| mixed.json | msgspec | 0.138 | 0.174 | 0.599 | 93.547 | 0.78x |
| mixed.json | ujson | 0.329 | 0.360 | 0.392 | 93.547 | 0.37x |
| mixed.json | json | 0.452 | 0.482 | 0.526 | 93.547 | 0.28x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.045 | 0.053 | 0.079 | 87.688 | 1.00x |
| users.json $[*].id | jmespath | 0.268 | 0.280 | 0.362 | 87.688 | 0.19x |
| users.json $[*].id | jsonpath-ng | 1.478 | 1.502 | 1.596 | 87.688 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.293 | 0.473 | 0.783 | 87.859 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.640 | 1.870 | 2.406 | 87.859 | 0.25x |
| users.json $[*].orders[*].total | jsonpath-ng | 10.359 | 11.130 | 14.079 | 87.859 | 0.04x |
| users.json $..total | strata | 1.337 | 1.587 | 2.251 | 87.688 | 1.00x |
| users.json $..total | jsonpath-ng | 197.175 | 220.619 | 252.844 | 87.688 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.664 | 4.061 | 4.422 | 87.734 | 1.00x |
| users.json $[*].id | orjson+jmespath | 9.932 | 11.841 | 12.321 | 87.734 | 0.34x |
| users.json $[*].id | orjson+jsonpath-ng | 11.084 | 13.302 | 14.127 | 87.734 | 0.31x |
| users.json $[*].orders[*].total | strata | 4.075 | 4.418 | 5.131 | 87.672 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.852 | 14.438 | 15.684 | 87.672 | 0.31x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 27.491 | 30.041 | 34.663 | 87.672 | 0.15x |
| users.json $..total | strata | 8.650 | 10.114 | 11.977 | 87.734 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 205.677 | 230.875 | 238.121 | 87.734 | 0.04x |

