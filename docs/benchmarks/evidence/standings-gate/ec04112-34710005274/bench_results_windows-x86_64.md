# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: ec0411225b6d58a1df905844c946a766d3c39f0a
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD
- compiler_flags: /nologo /O2 /W3 /DNDEBUG /MD /EHsc /std:c++20 /Zc:__cplusplus /arch:AVX2 /clang:-O3 /clang:-fprofile-use=D:\a\strata\strata\build\pgo\strata.profdata /DLL /MANIFEST:EMBED,ID=2 /MANIFESTUAC:NO /EXPORT:PyInit__strata (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.430 | 11.347 | 17.495 | 49.027 | 1.00x |
| users.json | orjson | 14.112 | 15.055 | 19.140 | 49.027 | 0.75x |
| users.json | msgspec | 13.429 | 14.675 | 16.364 | 49.027 | 0.77x |
| users.json | ujson | 23.215 | 24.519 | 29.891 | 49.027 | 0.46x |
| users.json | json | 23.726 | 24.441 | 25.413 | 49.027 | 0.46x |
| flat.json | strata | 0.857 | 0.901 | 0.981 | 57.988 | 1.00x |
| flat.json | orjson | 1.110 | 1.171 | 1.645 | 57.988 | 0.77x |
| flat.json | msgspec | 1.107 | 1.158 | 1.214 | 57.988 | 0.78x |
| flat.json | ujson | 2.269 | 2.394 | 2.534 | 57.988 | 0.38x |
| flat.json | json | 2.048 | 2.109 | 2.568 | 57.988 | 0.43x |
| nested.json | strata | 0.761 | 0.824 | 1.376 | 57.176 | 1.00x |
| nested.json | orjson | 1.072 | 1.110 | 1.750 | 57.176 | 0.74x |
| nested.json | msgspec | 1.034 | 1.120 | 1.898 | 57.176 | 0.74x |
| nested.json | ujson | 1.647 | 1.709 | 3.059 | 57.176 | 0.48x |
| nested.json | json | 2.180 | 2.235 | 4.467 | 57.176 | 0.37x |
| wide_arrays.json | strata | 4.528 | 5.612 | 6.335 | 59.211 | 1.00x |
| wide_arrays.json | orjson | 6.510 | 7.164 | 11.917 | 59.211 | 0.78x |
| wide_arrays.json | msgspec | 6.514 | 7.348 | 14.135 | 59.211 | 0.76x |
| wide_arrays.json | ujson | 9.107 | 9.798 | 15.602 | 59.211 | 0.57x |
| wide_arrays.json | json | 11.996 | 13.621 | 14.308 | 59.211 | 0.41x |
| mixed.json | strata | 0.189 | 0.190 | 0.193 | 57.137 | 1.00x |
| mixed.json | orjson | 0.212 | 0.215 | 0.227 | 57.137 | 0.88x |
| mixed.json | msgspec | 0.235 | 0.237 | 0.250 | 57.137 | 0.80x |
| mixed.json | ujson | 0.371 | 0.408 | 0.454 | 57.137 | 0.47x |
| mixed.json | json | 0.471 | 0.478 | 0.520 | 57.137 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.139 | 3.218 | 3.836 | 49.262 | 1.00x |
| users.json | orjson | 3.660 | 3.910 | 4.615 | 49.262 | 0.82x |
| users.json | msgspec | 5.004 | 5.293 | 5.848 | 49.262 | 0.61x |
| users.json | ujson | 14.144 | 14.279 | 14.799 | 49.262 | 0.23x |
| users.json | json | 23.368 | 23.612 | 31.960 | 49.262 | 0.14x |
| flat.json | strata | 0.316 | 0.336 | 0.386 | 57.797 | 1.00x |
| flat.json | orjson | 0.370 | 0.399 | 0.614 | 57.797 | 0.84x |
| flat.json | msgspec | 0.519 | 0.545 | 0.630 | 57.797 | 0.62x |
| flat.json | ujson | 1.420 | 1.468 | 2.648 | 57.797 | 0.23x |
| flat.json | json | 1.980 | 2.019 | 3.691 | 57.797 | 0.17x |
| nested.json | strata | 0.295 | 0.301 | 0.320 | 57.848 | 1.00x |
| nested.json | orjson | 0.329 | 0.341 | 0.373 | 57.848 | 0.89x |
| nested.json | msgspec | 0.477 | 0.483 | 0.512 | 57.848 | 0.62x |
| nested.json | ujson | 1.221 | 1.261 | 1.401 | 57.848 | 0.24x |
| nested.json | json | 2.447 | 2.488 | 2.706 | 57.848 | 0.12x |
| wide_arrays.json | strata | 2.005 | 2.166 | 2.391 | 58.586 | 1.00x |
| wide_arrays.json | orjson | 2.556 | 2.659 | 2.860 | 58.586 | 0.81x |
| wide_arrays.json | msgspec | 4.006 | 4.186 | 5.310 | 58.586 | 0.52x |
| wide_arrays.json | ujson | 7.812 | 7.938 | 8.395 | 58.586 | 0.27x |
| wide_arrays.json | json | 19.187 | 19.477 | 20.436 | 58.586 | 0.11x |
| mixed.json | strata | 0.072 | 0.076 | 0.135 | 57.469 | 1.00x |
| mixed.json | orjson | 0.070 | 0.072 | 0.116 | 57.469 | 1.06x |
| mixed.json | msgspec | 0.097 | 0.102 | 0.124 | 57.469 | 0.75x |
| mixed.json | ujson | 0.263 | 0.268 | 0.334 | 57.469 | 0.29x |
| mixed.json | json | 0.521 | 0.534 | 0.602 | 57.469 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.538 | 11.959 | 18.370 | 59.289 | 1.00x |
| users.json | orjson | 15.243 | 15.571 | 16.358 | 59.289 | 0.77x |
| users.json | msgspec | 14.668 | 15.285 | 15.585 | 59.289 | 0.78x |
| users.json | ujson | 27.207 | 28.788 | 30.055 | 59.289 | 0.42x |
| users.json | json | 24.507 | 24.794 | 33.158 | 59.289 | 0.48x |
| flat.json | strata | 1.248 | 1.339 | 2.210 | 57.594 | 1.00x |
| flat.json | orjson | 1.423 | 1.545 | 2.190 | 57.594 | 0.87x |
| flat.json | msgspec | 1.358 | 1.479 | 2.521 | 57.594 | 0.91x |
| flat.json | ujson | 3.007 | 3.657 | 6.273 | 57.594 | 0.37x |
| flat.json | json | 2.323 | 2.491 | 12.378 | 57.594 | 0.54x |
| nested.json | strata | 0.938 | 0.997 | 1.667 | 57.852 | 1.00x |
| nested.json | orjson | 1.297 | 1.430 | 2.527 | 57.852 | 0.70x |
| nested.json | msgspec | 1.243 | 1.317 | 1.728 | 57.852 | 0.76x |
| nested.json | ujson | 2.175 | 2.337 | 2.649 | 57.852 | 0.43x |
| nested.json | json | 2.395 | 2.522 | 4.996 | 57.852 | 0.40x |
| wide_arrays.json | strata | 4.897 | 5.070 | 5.572 | 58.586 | 1.00x |
| wide_arrays.json | orjson | 6.443 | 6.582 | 6.832 | 58.586 | 0.77x |
| wide_arrays.json | msgspec | 6.713 | 6.828 | 7.140 | 58.586 | 0.74x |
| wide_arrays.json | ujson | 11.550 | 11.917 | 16.434 | 58.586 | 0.43x |
| wide_arrays.json | json | 12.576 | 12.714 | 13.763 | 58.586 | 0.40x |
| mixed.json | strata | 0.280 | 0.292 | 0.340 | 57.473 | 1.00x |
| mixed.json | orjson | 0.342 | 0.355 | 0.381 | 57.473 | 0.82x |
| mixed.json | msgspec | 0.363 | 0.377 | 0.427 | 57.473 | 0.77x |
| mixed.json | ujson | 0.577 | 0.602 | 0.712 | 57.473 | 0.48x |
| mixed.json | json | 0.607 | 0.618 | 0.691 | 57.473 | 0.47x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 13.130 | 13.579 | 15.163 | 58.422 | 1.00x |
| users.ndjson | orjson | 19.701 | 21.098 | 22.303 | 58.422 | 0.64x |
| users.ndjson | msgspec | 20.305 | 21.401 | 24.693 | 58.422 | 0.63x |
| users.ndjson | ujson | 30.054 | 31.022 | 32.849 | 58.422 | 0.44x |
| users.ndjson | json | 33.986 | 34.322 | 35.431 | 58.422 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.145 | 4.256 | 4.630 | 58.344 | 1.00x |
| users.json | orjson | 4.788 | 5.005 | 7.204 | 58.344 | 0.85x |
| users.json | msgspec | 6.192 | 6.525 | 6.761 | 58.344 | 0.65x |
| users.json | ujson | 23.743 | 24.093 | 24.341 | 58.344 | 0.18x |
| users.json | json | 32.575 | 33.094 | 34.826 | 58.344 | 0.13x |
| flat.json | strata | 0.716 | 0.855 | 1.287 | 57.738 | 1.00x |
| flat.json | orjson | 0.729 | 0.816 | 1.194 | 57.738 | 1.05x |
| flat.json | msgspec | 0.952 | 1.006 | 1.439 | 57.738 | 0.85x |
| flat.json | ujson | 2.851 | 3.074 | 4.789 | 57.738 | 0.28x |
| flat.json | json | 3.450 | 3.551 | 6.128 | 57.738 | 0.24x |
| nested.json | strata | 0.682 | 0.748 | 1.190 | 57.859 | 1.00x |
| nested.json | orjson | 0.722 | 0.772 | 1.550 | 57.859 | 0.97x |
| nested.json | msgspec | 0.890 | 0.940 | 1.371 | 57.859 | 0.80x |
| nested.json | ujson | 2.359 | 2.406 | 2.861 | 57.859 | 0.31x |
| nested.json | json | 3.585 | 3.643 | 3.710 | 57.859 | 0.21x |
| wide_arrays.json | strata | 2.950 | 3.020 | 3.133 | 58.590 | 1.00x |
| wide_arrays.json | orjson | 3.378 | 3.633 | 3.793 | 58.590 | 0.83x |
| wide_arrays.json | msgspec | 4.928 | 5.102 | 5.411 | 58.590 | 0.59x |
| wide_arrays.json | ujson | 14.714 | 15.144 | 15.597 | 58.590 | 0.20x |
| wide_arrays.json | json | 25.811 | 26.738 | 33.810 | 58.590 | 0.11x |
| mixed.json | strata | 0.406 | 0.449 | 0.501 | 57.473 | 1.00x |
| mixed.json | orjson | 0.403 | 0.427 | 0.481 | 57.473 | 1.05x |
| mixed.json | msgspec | 0.432 | 0.444 | 0.522 | 57.473 | 1.01x |
| mixed.json | ujson | 0.787 | 0.835 | 0.942 | 57.473 | 0.54x |
| mixed.json | json | 1.042 | 1.097 | 1.184 | 57.473 | 0.41x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.099 | 0.105 | 0.122 | 58.395 | 1.00x |
| users.json $[*].id | jmespath | 0.469 | 0.477 | 0.526 | 58.395 | 0.22x |
| users.json $[*].id | jsonpath-ng | 2.596 | 3.007 | 3.160 | 58.395 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.501 | 0.548 | 1.425 | 58.422 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.916 | 3.045 | 7.813 | 58.422 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 18.427 | 20.802 | 27.027 | 58.422 | 0.03x |
| users.json $..total | strata | 2.086 | 2.190 | 4.071 | 58.422 | 1.00x |
| users.json $..total | jsonpath-ng | 352.508 | 368.975 | 413.766 | 58.422 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.070 | 4.099 | 4.254 | 58.422 | 1.00x |
| users.json $[*].id | orjson+jmespath | 16.094 | 16.653 | 18.341 | 58.422 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 18.303 | 19.218 | 20.828 | 58.422 | 0.21x |
| users.json $[*].orders[*].total | strata | 4.327 | 4.455 | 6.184 | 58.422 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 19.675 | 21.979 | 27.983 | 58.422 | 0.20x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 40.705 | 42.329 | 48.501 | 58.422 | 0.11x |
| users.json $..total | strata | 17.946 | 20.293 | 23.504 | 58.422 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 370.018 | 386.382 | 398.895 | 58.422 | 0.05x |

