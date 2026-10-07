# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 75cfb422ede723193706740309d7509adf327f2b
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: AMD EPYC 9V74 80-Core Processor
- compiler_flags: [["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-c","src/strata/bindings/python_document.cpp","-o","/tmp/tmpfbkw_5ui.build-temp/src/strata/bindings/python_document.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata","-fsplit-machine-functions"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-c","src/strata/bindings/python_dumps.cpp","-o","/tmp/tmpfbkw_5ui.build-temp/src/strata/bindings/python_dumps.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata","-fsplit-machine-functions"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-c","src/strata/bindings/python_files.cpp","-o","/tmp/tmpfbkw_5ui.build-temp/src/strata/bindings/python_files.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata","-fsplit-machine-functions"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-c","src/strata/bindings/python_folder.cpp","-o","/tmp/tmpfbkw_5ui.build-temp/src/strata/bindings/python_folder.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata","-fsplit-machine-functions"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-c","src/strata/bindings/python_jsonpath.cpp","-o","/tmp/tmpfbkw_5ui.build-temp/src/strata/bindings/python_jsonpath.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata","-fsplit-machine-functions"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-c","src/strata/bindings/python_loads.cpp","-o","/tmp/tmpfbkw_5ui.build-temp/src/strata/bindings/python_loads.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata","-fsplit-machine-functions"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-c","src/strata/bindings/python_module.cpp","-o","/tmp/tmpfbkw_5ui.build-temp/src/strata/bindings/python_module.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata","-fsplit-machine-functions"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-c","src/strata/bindings/python_ndjson.cpp","-o","/tmp/tmpfbkw_5ui.build-temp/src/strata/bindings/python_ndjson.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata","-fsplit-machine-functions"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-c","src/strata/json/json_cursor.cpp","-o","/tmp/tmpfbkw_5ui.build-temp/src/strata/json/json_cursor.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata","-fsplit-machine-functions"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-c","src/strata/json/json_document.cpp","-o","/tmp/tmpfbkw_5ui.build-temp/src/strata/json/json_document.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata","-fsplit-machine-functions"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-c","src/strata/json/json_parse.cpp","-o","/tmp/tmpfbkw_5ui.build-temp/src/strata/json/json_parse.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata","-fsplit-machine-functions"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-c","src/strata/json/json_serialize.cpp","-o","/tmp/tmpfbkw_5ui.build-temp/src/strata/json/json_serialize.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata","-fsplit-machine-functions"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-c","src/strata/json/ndjson_stream.cpp","-o","/tmp/tmpfbkw_5ui.build-temp/src/strata/json/ndjson_stream.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata","-fsplit-machine-functions"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-c","src/strata/search/jsonpath_compile.cpp","-o","/tmp/tmpfbkw_5ui.build-temp/src/strata/search/jsonpath_compile.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata","-fsplit-machine-functions"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-c","src/strata/search/jsonpath_eval.cpp","-o","/tmp/tmpfbkw_5ui.build-temp/src/strata/search/jsonpath_eval.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata","-fsplit-machine-functions"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-c","src/strata/util/dtoa.cpp","-o","/tmp/tmpfbkw_5ui.build-temp/src/strata/util/dtoa.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata","-fsplit-machine-functions"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-c","src/strata/util/folder.cpp","-o","/tmp/tmpfbkw_5ui.build-temp/src/strata/util/folder.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata","-fsplit-machine-functions"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/x64/include/python3.12","-c","src/strata/util/scan.cpp","-o","/tmp/tmpfbkw_5ui.build-temp/src/strata/util/scan.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata","-fsplit-machine-functions"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-shared","-Wl,--rpath=/opt/hostedtoolcache/Python/3.12.14/x64/lib","-Wl,--rpath=/opt/hostedtoolcache/Python/3.12.14/x64/lib","/tmp/tmpfbkw_5ui.build-temp/src/strata/bindings/python_document.o","/tmp/tmpfbkw_5ui.build-temp/src/strata/bindings/python_dumps.o","/tmp/tmpfbkw_5ui.build-temp/src/strata/bindings/python_files.o","/tmp/tmpfbkw_5ui.build-temp/src/strata/bindings/python_folder.o","/tmp/tmpfbkw_5ui.build-temp/src/strata/bindings/python_jsonpath.o","/tmp/tmpfbkw_5ui.build-temp/src/strata/bindings/python_loads.o","/tmp/tmpfbkw_5ui.build-temp/src/strata/bindings/python_module.o","/tmp/tmpfbkw_5ui.build-temp/src/strata/bindings/python_ndjson.o","/tmp/tmpfbkw_5ui.build-temp/src/strata/json/json_cursor.o","/tmp/tmpfbkw_5ui.build-temp/src/strata/json/json_document.o","/tmp/tmpfbkw_5ui.build-temp/src/strata/json/json_parse.o","/tmp/tmpfbkw_5ui.build-temp/src/strata/json/json_serialize.o","/tmp/tmpfbkw_5ui.build-temp/src/strata/json/ndjson_stream.o","/tmp/tmpfbkw_5ui.build-temp/src/strata/search/jsonpath_compile.o","/tmp/tmpfbkw_5ui.build-temp/src/strata/search/jsonpath_eval.o","/tmp/tmpfbkw_5ui.build-temp/src/strata/util/dtoa.o","/tmp/tmpfbkw_5ui.build-temp/src/strata/util/folder.o","/tmp/tmpfbkw_5ui.build-temp/src/strata/util/scan.o","-L/opt/hostedtoolcache/Python/3.12.14/x64/lib","-o","/tmp/tmp_equia8r.build-lib/strata/_strata.cpython-312-x86_64-linux-gnu.so","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata","-fsplit-machine-functions"]]
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.843 | 10.080 | 14.648 | 66.047 | 1.00x |
| users.json | orjson | 13.811 | 14.075 | 17.751 | 66.047 | 0.72x |
| users.json | msgspec | 13.709 | 14.184 | 17.344 | 66.047 | 0.71x |
| users.json | ujson | 18.111 | 18.415 | 23.634 | 66.047 | 0.55x |
| users.json | pysimdjson | 18.643 | 19.176 | 23.069 | 66.047 | 0.53x |
| users.json | json | 21.410 | 21.770 | 22.912 | 66.047 | 0.46x |
| flat.json | strata | 0.835 | 0.855 | 0.956 | 80.527 | 1.00x |
| flat.json | orjson | 1.035 | 1.043 | 1.054 | 80.527 | 0.82x |
| flat.json | msgspec | 1.073 | 1.088 | 1.113 | 80.527 | 0.79x |
| flat.json | ujson | 1.528 | 1.553 | 1.586 | 80.527 | 0.55x |
| flat.json | pysimdjson | 1.635 | 1.655 | 1.680 | 80.527 | 0.52x |
| flat.json | json | 1.727 | 1.761 | 1.807 | 80.527 | 0.49x |
| nested.json | strata | 0.804 | 0.816 | 0.832 | 80.527 | 1.00x |
| nested.json | orjson | 1.012 | 1.021 | 1.029 | 80.527 | 0.80x |
| nested.json | msgspec | 0.973 | 0.991 | 1.026 | 80.527 | 0.82x |
| nested.json | ujson | 1.415 | 1.432 | 1.449 | 80.527 | 0.57x |
| nested.json | pysimdjson | 1.390 | 1.409 | 1.419 | 80.527 | 0.58x |
| nested.json | json | 1.847 | 1.868 | 1.894 | 80.527 | 0.44x |
| wide_arrays.json | strata | 4.420 | 4.460 | 4.521 | 84.527 | 1.00x |
| wide_arrays.json | orjson | 5.575 | 5.634 | 5.709 | 84.527 | 0.79x |
| wide_arrays.json | msgspec | 6.121 | 6.199 | 6.255 | 84.527 | 0.72x |
| wide_arrays.json | ujson | 7.599 | 7.655 | 7.755 | 84.527 | 0.58x |
| wide_arrays.json | pysimdjson | 6.466 | 6.528 | 6.581 | 84.527 | 0.68x |
| wide_arrays.json | json | 9.892 | 9.923 | 9.954 | 84.527 | 0.45x |
| mixed.json | strata | 0.191 | 0.194 | 0.209 | 84.590 | 1.00x |
| mixed.json | orjson | 0.239 | 0.248 | 0.333 | 84.590 | 0.78x |
| mixed.json | msgspec | 0.251 | 0.255 | 0.367 | 84.590 | 0.76x |
| mixed.json | ujson | 0.308 | 0.314 | 0.336 | 84.590 | 0.62x |
| mixed.json | pysimdjson | 0.305 | 0.314 | 0.319 | 84.590 | 0.62x |
| mixed.json | json | 0.455 | 0.466 | 0.480 | 84.590 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.441 | 2.448 | 2.515 | 65.238 | 1.00x |
| users.json | orjson | 3.121 | 3.131 | 3.149 | 65.238 | 0.78x |
| users.json | msgspec | 4.120 | 4.142 | 4.202 | 65.238 | 0.59x |
| users.json | ujson | 11.374 | 11.508 | 11.619 | 65.238 | 0.21x |
| users.json | json | 21.443 | 21.598 | 21.912 | 65.238 | 0.11x |
| flat.json | strata | 0.313 | 0.320 | 0.342 | 80.527 | 1.00x |
| flat.json | orjson | 0.362 | 0.368 | 0.382 | 80.527 | 0.87x |
| flat.json | msgspec | 0.470 | 0.487 | 0.502 | 80.527 | 0.66x |
| flat.json | ujson | 1.023 | 1.041 | 1.157 | 80.527 | 0.31x |
| flat.json | json | 1.856 | 1.874 | 2.384 | 80.527 | 0.17x |
| nested.json | strata | 0.241 | 0.245 | 0.265 | 80.527 | 1.00x |
| nested.json | orjson | 0.299 | 0.302 | 0.316 | 80.527 | 0.81x |
| nested.json | msgspec | 0.410 | 0.419 | 0.429 | 80.527 | 0.58x |
| nested.json | ujson | 1.065 | 1.073 | 1.086 | 80.527 | 0.23x |
| nested.json | json | 2.325 | 2.338 | 2.374 | 80.527 | 0.10x |
| wide_arrays.json | strata | 1.852 | 1.871 | 1.914 | 84.527 | 1.00x |
| wide_arrays.json | orjson | 1.984 | 2.006 | 2.019 | 84.527 | 0.93x |
| wide_arrays.json | msgspec | 3.091 | 3.107 | 3.127 | 84.527 | 0.60x |
| wide_arrays.json | ujson | 6.411 | 6.457 | 6.507 | 84.527 | 0.29x |
| wide_arrays.json | json | 16.628 | 16.694 | 16.824 | 84.527 | 0.11x |
| mixed.json | strata | 0.063 | 0.065 | 0.079 | 84.590 | 1.00x |
| mixed.json | orjson | 0.068 | 0.070 | 0.072 | 84.590 | 0.93x |
| mixed.json | msgspec | 0.086 | 0.089 | 0.102 | 84.590 | 0.73x |
| mixed.json | ujson | 0.228 | 0.231 | 0.245 | 84.590 | 0.28x |
| mixed.json | json | 0.513 | 0.527 | 0.543 | 84.590 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.425 | 10.555 | 12.310 | 82.164 | 1.00x |
| users.json | orjson | 14.457 | 14.600 | 15.477 | 82.164 | 0.72x |
| users.json | msgspec | 14.266 | 14.420 | 14.851 | 82.164 | 0.73x |
| users.json | ujson | 19.151 | 19.513 | 21.551 | 82.164 | 0.54x |
| users.json | json | 21.801 | 22.116 | 22.476 | 82.164 | 0.48x |
| flat.json | strata | 0.875 | 0.886 | 0.915 | 80.527 | 1.00x |
| flat.json | orjson | 1.095 | 1.107 | 1.122 | 80.527 | 0.80x |
| flat.json | msgspec | 1.122 | 1.129 | 1.143 | 80.527 | 0.79x |
| flat.json | ujson | 1.601 | 1.630 | 1.664 | 80.527 | 0.54x |
| flat.json | json | 1.771 | 1.798 | 1.811 | 80.527 | 0.49x |
| nested.json | strata | 0.841 | 0.857 | 1.100 | 80.527 | 1.00x |
| nested.json | orjson | 1.075 | 1.087 | 1.117 | 80.527 | 0.79x |
| nested.json | msgspec | 1.038 | 1.056 | 1.190 | 80.527 | 0.81x |
| nested.json | ujson | 1.466 | 1.484 | 1.766 | 80.527 | 0.58x |
| nested.json | json | 1.901 | 1.932 | 2.998 | 80.527 | 0.44x |
| wide_arrays.json | strata | 4.431 | 4.479 | 4.507 | 84.590 | 1.00x |
| wide_arrays.json | orjson | 5.584 | 5.628 | 5.684 | 84.590 | 0.80x |
| wide_arrays.json | msgspec | 6.195 | 6.230 | 6.299 | 84.590 | 0.72x |
| wide_arrays.json | ujson | 7.731 | 7.802 | 7.863 | 84.590 | 0.57x |
| wide_arrays.json | json | 9.772 | 9.880 | 9.925 | 84.590 | 0.45x |
| mixed.json | strata | 0.215 | 0.225 | 0.233 | 84.590 | 1.00x |
| mixed.json | orjson | 0.295 | 0.305 | 0.350 | 84.590 | 0.74x |
| mixed.json | msgspec | 0.294 | 0.307 | 0.325 | 84.590 | 0.73x |
| mixed.json | ujson | 0.370 | 0.380 | 0.406 | 84.590 | 0.59x |
| mixed.json | json | 0.499 | 0.519 | 0.541 | 84.590 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.679 | 10.826 | 11.120 | 80.527 | 1.00x |
| users.ndjson | orjson | 18.061 | 18.203 | 18.404 | 80.527 | 0.59x |
| users.ndjson | msgspec | 18.002 | 18.252 | 18.468 | 80.527 | 0.59x |
| users.ndjson | ujson | 22.670 | 22.975 | 23.305 | 80.527 | 0.47x |
| users.ndjson | json | 29.088 | 29.484 | 29.880 | 80.527 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.952 | 2.993 | 3.028 | 82.164 | 1.00x |
| users.json | orjson | 3.662 | 3.697 | 3.709 | 82.164 | 0.81x |
| users.json | msgspec | 4.677 | 4.706 | 4.754 | 82.164 | 0.64x |
| users.json | ujson | 12.062 | 12.097 | 12.612 | 82.164 | 0.25x |
| users.json | json | 22.315 | 22.698 | 22.875 | 82.164 | 0.13x |
| flat.json | strata | 0.461 | 0.479 | 0.493 | 80.527 | 1.00x |
| flat.json | orjson | 0.532 | 0.541 | 0.555 | 80.527 | 0.88x |
| flat.json | msgspec | 0.645 | 0.659 | 0.679 | 80.527 | 0.73x |
| flat.json | ujson | 1.210 | 1.221 | 1.256 | 80.527 | 0.39x |
| flat.json | json | 2.042 | 2.069 | 2.094 | 80.527 | 0.23x |
| nested.json | strata | 0.360 | 0.375 | 0.388 | 80.527 | 1.00x |
| nested.json | orjson | 0.446 | 0.466 | 0.484 | 80.527 | 0.81x |
| nested.json | msgspec | 0.550 | 0.570 | 0.614 | 80.527 | 0.66x |
| nested.json | ujson | 1.213 | 1.227 | 1.243 | 80.527 | 0.31x |
| nested.json | json | 2.557 | 2.603 | 2.638 | 80.527 | 0.14x |
| wide_arrays.json | strata | 2.251 | 2.269 | 2.283 | 84.590 | 1.00x |
| wide_arrays.json | orjson | 2.391 | 2.419 | 2.461 | 84.590 | 0.94x |
| wide_arrays.json | msgspec | 3.487 | 3.510 | 3.539 | 84.590 | 0.65x |
| wide_arrays.json | ujson | 6.896 | 6.922 | 6.942 | 84.590 | 0.33x |
| wide_arrays.json | json | 17.144 | 17.252 | 17.560 | 84.590 | 0.13x |
| mixed.json | strata | 0.159 | 0.161 | 0.166 | 84.590 | 1.00x |
| mixed.json | orjson | 0.178 | 0.186 | 0.229 | 84.590 | 0.87x |
| mixed.json | msgspec | 0.192 | 0.200 | 0.251 | 84.590 | 0.81x |
| mixed.json | ujson | 0.351 | 0.354 | 0.398 | 84.590 | 0.45x |
| mixed.json | json | 0.630 | 0.641 | 0.694 | 84.590 | 0.25x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.073 | 0.075 | 0.090 | 82.164 | 1.00x |
| users.json $[*].id | jmespath | 0.466 | 0.476 | 0.511 | 82.164 | 0.16x |
| users.json $[*].id | jsonpath-ng | 2.714 | 2.756 | 2.795 | 82.164 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.422 | 0.440 | 0.687 | 82.164 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.020 | 3.087 | 3.533 | 82.164 | 0.14x |
| users.json $[*].orders[*].total | jsonpath-ng | 19.293 | 19.666 | 26.008 | 82.164 | 0.02x |
| users.json $..total | strata | 1.793 | 1.849 | 1.877 | 82.164 | 1.00x |
| users.json $..total | jsonpath-ng | 389.821 | 391.553 | 394.480 | 82.164 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.251 | 3.270 | 3.280 | 82.164 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.522 | 15.639 | 16.969 | 82.164 | 0.21x |
| users.json $[*].id | orjson+jsonpath-ng | 17.635 | 17.935 | 19.016 | 82.164 | 0.18x |
| users.json $[*].orders[*].total | strata | 3.479 | 3.500 | 3.519 | 82.164 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.588 | 18.763 | 18.924 | 82.164 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 38.402 | 39.064 | 39.508 | 82.164 | 0.09x |
| users.json $..total | strata | 13.445 | 14.006 | 15.189 | 82.164 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 412.700 | 415.061 | 416.356 | 82.164 | 0.03x |

