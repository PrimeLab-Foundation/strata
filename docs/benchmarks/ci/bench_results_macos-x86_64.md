# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: a969975
- python: 3.12.10
- implementation: CPython
- platform: macOS-15.7.9-x86_64-i386-64bit
- machine: x86_64
- processor: Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 20.653 | 21.493 | 23.979 | 52.586 | 1.00x |
| users.json | orjson | 26.661 | 28.605 | 30.151 | 52.586 | 0.75x |
| users.json | msgspec | 27.258 | 28.712 | 31.690 | 52.586 | 0.75x |
| users.json | ujson | 39.880 | 42.079 | 43.842 | 52.586 | 0.51x |
| users.json | pysimdjson | 174.856 | 181.018 | 186.352 | 52.586 | 0.12x |
| users.json | json | 44.990 | 46.981 | 49.443 | 52.586 | 0.46x |
| flat.json | strata | 1.328 | 1.367 | 1.547 | 60.754 | 1.00x |
| flat.json | orjson | 1.415 | 1.450 | 1.615 | 60.754 | 0.94x |
| flat.json | msgspec | 1.646 | 1.670 | 1.799 | 60.754 | 0.82x |
| flat.json | ujson | 2.865 | 2.911 | 3.115 | 60.754 | 0.47x |
| flat.json | pysimdjson | 15.522 | 15.702 | 16.861 | 60.754 | 0.09x |
| flat.json | json | 3.347 | 3.395 | 3.604 | 60.754 | 0.40x |
| nested.json | strata | 1.590 | 1.847 | 5.892 | 49.098 | 1.00x |
| nested.json | orjson | 1.825 | 1.956 | 3.707 | 49.098 | 0.94x |
| nested.json | msgspec | 2.041 | 2.332 | 4.153 | 49.098 | 0.79x |
| nested.json | ujson | 3.386 | 3.968 | 187.271 | 49.098 | 0.47x |
| nested.json | pysimdjson | 14.867 | 18.442 | 79.910 | 49.098 | 0.10x |
| nested.json | json | 4.389 | 6.799 | 98.207 | 49.098 | 0.27x |
| wide_arrays.json | strata | 9.396 | 9.550 | 9.960 | 60.402 | 1.00x |
| wide_arrays.json | orjson | 9.884 | 9.981 | 10.553 | 60.402 | 0.96x |
| wide_arrays.json | msgspec | 11.024 | 11.111 | 11.344 | 60.402 | 0.86x |
| wide_arrays.json | ujson | 14.288 | 14.528 | 15.524 | 60.402 | 0.66x |
| wide_arrays.json | pysimdjson | 88.399 | 89.071 | 91.530 | 60.402 | 0.11x |
| wide_arrays.json | json | 18.557 | 18.722 | 19.841 | 60.402 | 0.51x |
| mixed.json | strata | 0.366 | 0.371 | 0.406 | 59.164 | 1.00x |
| mixed.json | orjson | 0.440 | 0.445 | 0.471 | 59.164 | 0.83x |
| mixed.json | msgspec | 0.464 | 0.476 | 0.488 | 59.164 | 0.78x |
| mixed.json | ujson | 0.647 | 0.656 | 0.697 | 59.164 | 0.57x |
| mixed.json | pysimdjson | 3.304 | 3.329 | 3.481 | 59.164 | 0.11x |
| mixed.json | json | 0.896 | 0.915 | 0.980 | 59.164 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.696 | 2.811 | 2.964 | 48.242 | 1.00x |
| users.json | orjson | 3.524 | 3.713 | 4.101 | 48.242 | 0.76x |
| users.json | msgspec | 5.552 | 6.114 | 6.435 | 48.242 | 0.46x |
| users.json | ujson | 33.459 | 34.071 | 37.426 | 48.242 | 0.08x |
| users.json | json | 44.225 | 45.910 | 50.192 | 48.242 | 0.06x |
| flat.json | strata | 0.315 | 0.322 | 0.334 | 49.531 | 1.00x |
| flat.json | orjson | 0.400 | 0.431 | 0.602 | 49.531 | 0.75x |
| flat.json | msgspec | 0.543 | 0.564 | 0.585 | 49.531 | 0.57x |
| flat.json | ujson | 2.805 | 2.972 | 3.184 | 49.531 | 0.11x |
| flat.json | json | 3.867 | 3.968 | 4.224 | 49.531 | 0.08x |
| nested.json | strata | 0.254 | 0.262 | 0.307 | 48.844 | 1.00x |
| nested.json | orjson | 0.375 | 0.397 | 0.574 | 48.844 | 0.66x |
| nested.json | msgspec | 0.590 | 0.603 | 0.709 | 48.844 | 0.44x |
| nested.json | ujson | 3.008 | 3.084 | 3.369 | 48.844 | 0.09x |
| nested.json | json | 4.993 | 5.114 | 5.328 | 48.844 | 0.05x |
| wide_arrays.json | strata | 2.029 | 2.110 | 2.220 | 62.270 | 1.00x |
| wide_arrays.json | orjson | 2.482 | 2.529 | 2.775 | 62.270 | 0.83x |
| wide_arrays.json | msgspec | 3.366 | 3.404 | 3.560 | 62.270 | 0.62x |
| wide_arrays.json | ujson | 16.400 | 16.635 | 17.398 | 62.270 | 0.13x |
| wide_arrays.json | json | 37.099 | 37.452 | 38.261 | 62.270 | 0.06x |
| mixed.json | strata | 0.077 | 0.080 | 0.087 | 57.969 | 1.00x |
| mixed.json | orjson | 0.072 | 0.074 | 0.087 | 57.969 | 1.07x |
| mixed.json | msgspec | 0.106 | 0.110 | 0.123 | 57.969 | 0.72x |
| mixed.json | ujson | 0.629 | 0.634 | 0.670 | 57.969 | 0.13x |
| mixed.json | json | 0.982 | 0.988 | 1.001 | 57.969 | 0.08x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 20.940 | 21.210 | 22.460 | 60.574 | 1.00x |
| users.json | orjson | 28.003 | 28.528 | 32.889 | 60.574 | 0.74x |
| users.json | msgspec | 28.382 | 28.960 | 32.286 | 60.574 | 0.73x |
| users.json | ujson | 40.844 | 43.202 | 46.448 | 60.574 | 0.49x |
| users.json | json | 46.209 | 47.435 | 53.215 | 60.574 | 0.45x |
| flat.json | strata | 1.398 | 1.439 | 1.530 | 48.871 | 1.00x |
| flat.json | orjson | 1.531 | 1.580 | 1.670 | 48.871 | 0.91x |
| flat.json | msgspec | 1.792 | 1.808 | 1.885 | 48.871 | 0.80x |
| flat.json | ujson | 3.042 | 3.096 | 3.363 | 48.871 | 0.46x |
| flat.json | json | 3.480 | 3.495 | 3.569 | 48.871 | 0.41x |
| nested.json | strata | 1.740 | 1.779 | 1.949 | 48.844 | 1.00x |
| nested.json | orjson | 1.982 | 2.025 | 2.461 | 48.844 | 0.88x |
| nested.json | msgspec | 2.182 | 2.226 | 2.494 | 48.844 | 0.80x |
| nested.json | ujson | 3.573 | 3.637 | 3.868 | 48.844 | 0.49x |
| nested.json | json | 4.395 | 4.684 | 4.896 | 48.844 | 0.38x |
| wide_arrays.json | strata | 8.832 | 8.945 | 9.603 | 62.270 | 1.00x |
| wide_arrays.json | orjson | 9.388 | 9.552 | 10.101 | 62.270 | 0.94x |
| wide_arrays.json | msgspec | 10.459 | 10.699 | 11.523 | 62.270 | 0.84x |
| wide_arrays.json | ujson | 13.725 | 13.951 | 15.210 | 62.270 | 0.64x |
| wide_arrays.json | json | 17.352 | 17.705 | 19.603 | 62.270 | 0.51x |
| mixed.json | strata | 0.429 | 0.442 | 0.474 | 57.969 | 1.00x |
| mixed.json | orjson | 0.530 | 0.557 | 0.583 | 57.969 | 0.79x |
| mixed.json | msgspec | 0.549 | 0.581 | 0.625 | 57.969 | 0.76x |
| mixed.json | ujson | 0.730 | 0.775 | 0.807 | 57.969 | 0.57x |
| mixed.json | json | 0.987 | 1.011 | 1.063 | 57.969 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 20.389 | 22.662 | 24.040 | 61.094 | 1.00x |
| users.ndjson | orjson | 27.469 | 31.885 | 35.721 | 61.094 | 0.71x |
| users.ndjson | msgspec | 28.359 | 32.238 | 33.932 | 61.094 | 0.70x |
| users.ndjson | ujson | 40.123 | 43.849 | 49.693 | 61.094 | 0.52x |
| users.ndjson | json | 50.362 | 55.037 | 61.629 | 61.094 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.722 | 3.825 | 4.403 | 59.645 | 1.00x |
| users.json | orjson | 4.576 | 4.608 | 5.568 | 59.645 | 0.83x |
| users.json | msgspec | 6.320 | 6.528 | 7.451 | 59.645 | 0.59x |
| users.json | ujson | 35.059 | 36.085 | 37.710 | 59.645 | 0.11x |
| users.json | json | 46.579 | 48.128 | 69.545 | 59.645 | 0.08x |
| flat.json | strata | 0.637 | 0.669 | 0.746 | 48.871 | 1.00x |
| flat.json | orjson | 0.740 | 0.774 | 0.854 | 48.871 | 0.86x |
| flat.json | msgspec | 0.888 | 0.936 | 0.980 | 48.871 | 0.71x |
| flat.json | ujson | 3.129 | 3.186 | 3.277 | 48.871 | 0.21x |
| flat.json | json | 4.230 | 4.287 | 4.419 | 48.871 | 0.16x |
| nested.json | strata | 0.554 | 0.635 | 0.749 | 48.844 | 1.00x |
| nested.json | orjson | 0.705 | 0.801 | 0.869 | 48.844 | 0.79x |
| nested.json | msgspec | 0.917 | 1.046 | 1.087 | 48.844 | 0.61x |
| nested.json | ujson | 3.468 | 3.566 | 3.734 | 48.844 | 0.18x |
| nested.json | json | 5.579 | 5.688 | 6.091 | 48.844 | 0.11x |
| wide_arrays.json | strata | 2.527 | 2.747 | 3.180 | 62.270 | 1.00x |
| wide_arrays.json | orjson | 3.065 | 3.256 | 3.734 | 62.270 | 0.84x |
| wide_arrays.json | msgspec | 3.868 | 4.216 | 5.830 | 62.270 | 0.65x |
| wide_arrays.json | ujson | 16.428 | 16.698 | 67.506 | 62.270 | 0.16x |
| wide_arrays.json | json | 36.228 | 37.027 | 44.001 | 62.270 | 0.07x |
| mixed.json | strata | 0.359 | 0.399 | 0.543 | 57.969 | 1.00x |
| mixed.json | orjson | 0.361 | 0.409 | 0.500 | 57.969 | 0.97x |
| mixed.json | msgspec | 0.374 | 0.447 | 0.486 | 57.969 | 0.89x |
| mixed.json | ujson | 0.930 | 0.981 | 1.116 | 57.969 | 0.41x |
| mixed.json | json | 1.256 | 1.324 | 1.561 | 57.969 | 0.30x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.142 | 0.148 | 0.161 | 59.699 | 1.00x |
| users.json $[*].id | jmespath | 1.011 | 1.031 | 1.059 | 59.699 | 0.14x |
| users.json $[*].id | jsonpath-ng | 5.648 | 5.686 | 5.922 | 59.699 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.872 | 1.040 | 1.259 | 56.828 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 6.267 | 6.690 | 7.326 | 56.828 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 36.882 | 40.565 | 42.454 | 56.828 | 0.03x |
| users.json $..total | strata | 3.588 | 3.840 | 4.365 | 56.977 | 1.00x |
| users.json $..total | jsonpath-ng | 755.329 | 801.599 | 825.508 | 56.977 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.384 | 4.399 | 4.422 | 59.777 | 1.00x |
| users.json $[*].id | orjson+jmespath | 28.631 | 28.835 | 40.492 | 59.777 | 0.15x |
| users.json $[*].id | orjson+jsonpath-ng | 33.486 | 34.076 | 35.102 | 59.777 | 0.13x |
| users.json $[*].orders[*].total | strata | 4.419 | 4.524 | 4.548 | 56.879 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 32.446 | 33.102 | 36.015 | 56.879 | 0.14x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 67.348 | 68.433 | 70.569 | 56.879 | 0.07x |
| users.json $..total | strata | 24.694 | 26.508 | 36.477 | 57.035 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 775.822 | 836.567 | 1007.512 | 57.035 | 0.03x |

