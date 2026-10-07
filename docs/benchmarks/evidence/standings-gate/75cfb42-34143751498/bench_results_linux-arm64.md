# Benchmark results - ci-linux-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 75cfb422ede723193706740309d7509adf327f2b
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-aarch64-with-glibc2.39
- machine: aarch64
- processor: aarch64
- compiler_flags: [["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-c","src/strata/bindings/python_document.cpp","-o","/tmp/tmpoclp8x_k.build-temp/src/strata/bindings/python_document.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-c","src/strata/bindings/python_dumps.cpp","-o","/tmp/tmpoclp8x_k.build-temp/src/strata/bindings/python_dumps.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-c","src/strata/bindings/python_files.cpp","-o","/tmp/tmpoclp8x_k.build-temp/src/strata/bindings/python_files.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-c","src/strata/bindings/python_folder.cpp","-o","/tmp/tmpoclp8x_k.build-temp/src/strata/bindings/python_folder.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-c","src/strata/bindings/python_jsonpath.cpp","-o","/tmp/tmpoclp8x_k.build-temp/src/strata/bindings/python_jsonpath.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-c","src/strata/bindings/python_loads.cpp","-o","/tmp/tmpoclp8x_k.build-temp/src/strata/bindings/python_loads.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-c","src/strata/bindings/python_module.cpp","-o","/tmp/tmpoclp8x_k.build-temp/src/strata/bindings/python_module.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-c","src/strata/bindings/python_ndjson.cpp","-o","/tmp/tmpoclp8x_k.build-temp/src/strata/bindings/python_ndjson.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-c","src/strata/json/json_cursor.cpp","-o","/tmp/tmpoclp8x_k.build-temp/src/strata/json/json_cursor.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-c","src/strata/json/json_document.cpp","-o","/tmp/tmpoclp8x_k.build-temp/src/strata/json/json_document.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-c","src/strata/json/json_parse.cpp","-o","/tmp/tmpoclp8x_k.build-temp/src/strata/json/json_parse.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-c","src/strata/json/json_serialize.cpp","-o","/tmp/tmpoclp8x_k.build-temp/src/strata/json/json_serialize.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-c","src/strata/json/ndjson_stream.cpp","-o","/tmp/tmpoclp8x_k.build-temp/src/strata/json/ndjson_stream.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-c","src/strata/search/jsonpath_compile.cpp","-o","/tmp/tmpoclp8x_k.build-temp/src/strata/search/jsonpath_compile.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-c","src/strata/search/jsonpath_eval.cpp","-o","/tmp/tmpoclp8x_k.build-temp/src/strata/search/jsonpath_eval.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-c","src/strata/util/dtoa.cpp","-o","/tmp/tmpoclp8x_k.build-temp/src/strata/util/dtoa.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-c","src/strata/util/folder.cpp","-o","/tmp/tmpoclp8x_k.build-temp/src/strata/util/folder.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-fPIC","-Iinclude","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-I/home/runner/work/strata/strata/.venv/include","-I/opt/hostedtoolcache/Python/3.12.14/arm64/include/python3.12","-c","src/strata/util/scan.cpp","-o","/tmp/tmpoclp8x_k.build-temp/src/strata/util/scan.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata"],["clang++","-fno-strict-overflow","-Wsign-compare","-DNDEBUG","-g","-O3","-Wall","-shared","-Wl,--rpath=/opt/hostedtoolcache/Python/3.12.14/arm64/lib","-Wl,--rpath=/opt/hostedtoolcache/Python/3.12.14/arm64/lib","/tmp/tmpoclp8x_k.build-temp/src/strata/bindings/python_document.o","/tmp/tmpoclp8x_k.build-temp/src/strata/bindings/python_dumps.o","/tmp/tmpoclp8x_k.build-temp/src/strata/bindings/python_files.o","/tmp/tmpoclp8x_k.build-temp/src/strata/bindings/python_folder.o","/tmp/tmpoclp8x_k.build-temp/src/strata/bindings/python_jsonpath.o","/tmp/tmpoclp8x_k.build-temp/src/strata/bindings/python_loads.o","/tmp/tmpoclp8x_k.build-temp/src/strata/bindings/python_module.o","/tmp/tmpoclp8x_k.build-temp/src/strata/bindings/python_ndjson.o","/tmp/tmpoclp8x_k.build-temp/src/strata/json/json_cursor.o","/tmp/tmpoclp8x_k.build-temp/src/strata/json/json_document.o","/tmp/tmpoclp8x_k.build-temp/src/strata/json/json_parse.o","/tmp/tmpoclp8x_k.build-temp/src/strata/json/json_serialize.o","/tmp/tmpoclp8x_k.build-temp/src/strata/json/ndjson_stream.o","/tmp/tmpoclp8x_k.build-temp/src/strata/search/jsonpath_compile.o","/tmp/tmpoclp8x_k.build-temp/src/strata/search/jsonpath_eval.o","/tmp/tmpoclp8x_k.build-temp/src/strata/util/dtoa.o","/tmp/tmpoclp8x_k.build-temp/src/strata/util/folder.o","/tmp/tmpoclp8x_k.build-temp/src/strata/util/scan.o","-L/opt/hostedtoolcache/Python/3.12.14/arm64/lib","-o","/tmp/tmpzuvapeqs.build-lib/strata/_strata.cpython-312-aarch64-linux-gnu.so","-flto=thin","-fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata"]]
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.730 | 8.757 | 10.502 | 57.273 | 1.00x |
| users.json | orjson | 11.562 | 11.684 | 13.179 | 57.273 | 0.75x |
| users.json | msgspec | 12.164 | 12.219 | 13.614 | 57.273 | 0.72x |
| users.json | ujson | 16.267 | 16.414 | 18.608 | 57.273 | 0.53x |
| users.json | pysimdjson | 16.243 | 16.443 | 18.037 | 57.273 | 0.53x |
| users.json | json | 20.455 | 20.572 | 21.219 | 57.273 | 0.43x |
| flat.json | strata | 0.781 | 0.799 | 0.819 | 67.191 | 1.00x |
| flat.json | orjson | 0.846 | 0.861 | 0.870 | 67.191 | 0.93x |
| flat.json | msgspec | 0.893 | 0.909 | 0.941 | 67.191 | 0.88x |
| flat.json | ujson | 1.406 | 1.415 | 1.423 | 67.191 | 0.56x |
| flat.json | pysimdjson | 1.461 | 1.470 | 1.476 | 67.191 | 0.54x |
| flat.json | json | 1.750 | 1.768 | 1.773 | 67.191 | 0.45x |
| nested.json | strata | 0.779 | 0.796 | 0.801 | 67.191 | 1.00x |
| nested.json | orjson | 0.880 | 0.884 | 0.893 | 67.191 | 0.90x |
| nested.json | msgspec | 0.985 | 0.995 | 1.004 | 67.191 | 0.80x |
| nested.json | ujson | 1.391 | 1.398 | 1.424 | 67.191 | 0.57x |
| nested.json | pysimdjson | 1.396 | 1.411 | 1.420 | 67.191 | 0.56x |
| nested.json | json | 1.957 | 1.970 | 2.005 | 67.191 | 0.40x |
| wide_arrays.json | strata | 3.821 | 3.844 | 3.873 | 69.684 | 1.00x |
| wide_arrays.json | orjson | 4.052 | 4.097 | 4.146 | 69.684 | 0.94x |
| wide_arrays.json | msgspec | 5.048 | 5.097 | 5.134 | 69.684 | 0.75x |
| wide_arrays.json | ujson | 6.509 | 6.539 | 6.583 | 69.684 | 0.59x |
| wide_arrays.json | pysimdjson | 5.235 | 5.302 | 5.323 | 69.684 | 0.73x |
| wide_arrays.json | json | 9.453 | 9.496 | 9.530 | 69.684 | 0.40x |
| mixed.json | strata | 0.187 | 0.190 | 0.206 | 69.684 | 1.00x |
| mixed.json | orjson | 0.211 | 0.213 | 0.233 | 69.684 | 0.89x |
| mixed.json | msgspec | 0.231 | 0.240 | 0.280 | 69.684 | 0.79x |
| mixed.json | ujson | 0.297 | 0.302 | 0.321 | 69.684 | 0.63x |
| mixed.json | pysimdjson | 0.289 | 0.291 | 0.315 | 69.684 | 0.65x |
| mixed.json | json | 0.449 | 0.459 | 0.475 | 69.684 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.021 | 2.029 | 2.052 | 56.344 | 1.00x |
| users.json | orjson | 2.575 | 2.590 | 2.606 | 56.344 | 0.78x |
| users.json | msgspec | 3.305 | 3.310 | 3.349 | 56.344 | 0.61x |
| users.json | ujson | 10.510 | 10.547 | 10.576 | 56.344 | 0.19x |
| users.json | json | 18.929 | 18.970 | 19.044 | 56.344 | 0.11x |
| flat.json | strata | 0.238 | 0.239 | 0.265 | 67.191 | 1.00x |
| flat.json | orjson | 0.301 | 0.306 | 0.324 | 67.191 | 0.78x |
| flat.json | msgspec | 0.383 | 0.387 | 0.406 | 67.191 | 0.62x |
| flat.json | ujson | 0.994 | 1.004 | 1.020 | 67.191 | 0.24x |
| flat.json | json | 1.679 | 1.697 | 1.726 | 67.191 | 0.14x |
| nested.json | strata | 0.237 | 0.239 | 0.259 | 67.195 | 1.00x |
| nested.json | orjson | 0.284 | 0.287 | 0.302 | 67.195 | 0.83x |
| nested.json | msgspec | 0.363 | 0.365 | 0.386 | 67.195 | 0.65x |
| nested.json | ujson | 1.071 | 1.079 | 1.094 | 67.195 | 0.22x |
| nested.json | json | 2.128 | 2.163 | 2.185 | 67.195 | 0.11x |
| wide_arrays.json | strata | 1.322 | 1.332 | 1.353 | 69.684 | 1.00x |
| wide_arrays.json | orjson | 1.603 | 1.611 | 1.628 | 69.684 | 0.83x |
| wide_arrays.json | msgspec | 2.371 | 2.383 | 2.417 | 69.684 | 0.56x |
| wide_arrays.json | ujson | 4.771 | 4.783 | 4.883 | 69.684 | 0.28x |
| wide_arrays.json | json | 13.540 | 13.555 | 13.589 | 69.684 | 0.10x |
| mixed.json | strata | 0.064 | 0.066 | 0.076 | 69.684 | 1.00x |
| mixed.json | orjson | 0.064 | 0.065 | 0.068 | 69.684 | 1.02x |
| mixed.json | msgspec | 0.078 | 0.078 | 0.093 | 69.684 | 0.85x |
| mixed.json | ujson | 0.241 | 0.244 | 0.263 | 69.684 | 0.27x |
| mixed.json | json | 0.471 | 0.481 | 0.495 | 69.684 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.921 | 8.970 | 9.679 | 68.582 | 1.00x |
| users.json | orjson | 11.800 | 11.898 | 12.306 | 68.582 | 0.75x |
| users.json | msgspec | 12.382 | 12.443 | 12.728 | 68.582 | 0.72x |
| users.json | ujson | 16.848 | 17.037 | 17.988 | 68.582 | 0.53x |
| users.json | json | 20.769 | 20.854 | 21.016 | 68.582 | 0.43x |
| flat.json | strata | 0.826 | 0.845 | 0.854 | 67.191 | 1.00x |
| flat.json | orjson | 0.926 | 0.940 | 0.952 | 67.191 | 0.90x |
| flat.json | msgspec | 0.982 | 0.986 | 0.993 | 67.191 | 0.86x |
| flat.json | ujson | 1.532 | 1.538 | 1.563 | 67.191 | 0.55x |
| flat.json | json | 1.828 | 1.832 | 1.850 | 67.191 | 0.46x |
| nested.json | strata | 0.818 | 0.840 | 0.875 | 67.195 | 1.00x |
| nested.json | orjson | 0.945 | 0.953 | 0.957 | 67.195 | 0.88x |
| nested.json | msgspec | 1.051 | 1.059 | 1.094 | 67.195 | 0.79x |
| nested.json | ujson | 1.466 | 1.478 | 1.506 | 67.195 | 0.57x |
| nested.json | json | 2.011 | 2.029 | 2.039 | 67.195 | 0.41x |
| wide_arrays.json | strata | 3.846 | 3.869 | 3.889 | 69.684 | 1.00x |
| wide_arrays.json | orjson | 4.056 | 4.091 | 4.172 | 69.684 | 0.95x |
| wide_arrays.json | msgspec | 5.072 | 5.107 | 5.192 | 69.684 | 0.76x |
| wide_arrays.json | ujson | 6.652 | 6.680 | 6.752 | 69.684 | 0.58x |
| wide_arrays.json | json | 9.535 | 9.576 | 9.633 | 69.684 | 0.40x |
| mixed.json | strata | 0.215 | 0.217 | 0.221 | 69.684 | 1.00x |
| mixed.json | orjson | 0.275 | 0.280 | 0.288 | 69.684 | 0.77x |
| mixed.json | msgspec | 0.296 | 0.306 | 0.317 | 69.684 | 0.71x |
| mixed.json | ujson | 0.395 | 0.398 | 0.401 | 69.684 | 0.54x |
| mixed.json | json | 0.508 | 0.513 | 0.533 | 69.684 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.212 | 9.262 | 9.301 | 67.188 | 1.00x |
| users.ndjson | orjson | 14.525 | 14.666 | 14.749 | 67.188 | 0.63x |
| users.ndjson | msgspec | 14.978 | 15.088 | 15.260 | 67.188 | 0.61x |
| users.ndjson | ujson | 19.422 | 19.736 | 20.024 | 67.188 | 0.47x |
| users.ndjson | json | 25.528 | 25.652 | 25.812 | 67.188 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.466 | 2.493 | 2.580 | 68.582 | 1.00x |
| users.json | orjson | 3.057 | 3.117 | 3.166 | 68.582 | 0.80x |
| users.json | msgspec | 3.806 | 3.825 | 3.850 | 68.582 | 0.65x |
| users.json | ujson | 11.133 | 11.399 | 11.579 | 68.582 | 0.22x |
| users.json | json | 19.543 | 19.662 | 19.896 | 68.582 | 0.13x |
| flat.json | strata | 0.391 | 0.396 | 0.432 | 67.191 | 1.00x |
| flat.json | orjson | 0.483 | 0.497 | 0.509 | 67.191 | 0.80x |
| flat.json | msgspec | 0.568 | 0.598 | 0.612 | 67.191 | 0.66x |
| flat.json | ujson | 1.205 | 1.217 | 1.231 | 67.191 | 0.33x |
| flat.json | json | 1.892 | 1.906 | 1.931 | 67.191 | 0.21x |
| nested.json | strata | 0.369 | 0.395 | 0.435 | 67.195 | 1.00x |
| nested.json | orjson | 0.454 | 0.492 | 0.518 | 67.195 | 0.80x |
| nested.json | msgspec | 0.523 | 0.558 | 0.563 | 67.195 | 0.71x |
| nested.json | ujson | 1.279 | 1.304 | 1.335 | 67.195 | 0.30x |
| nested.json | json | 2.329 | 2.371 | 2.390 | 67.195 | 0.17x |
| wide_arrays.json | strata | 1.686 | 1.717 | 1.748 | 69.684 | 1.00x |
| wide_arrays.json | orjson | 1.995 | 2.030 | 2.050 | 69.684 | 0.85x |
| wide_arrays.json | msgspec | 2.752 | 2.782 | 2.814 | 69.684 | 0.62x |
| wide_arrays.json | ujson | 5.206 | 5.228 | 5.290 | 69.684 | 0.33x |
| wide_arrays.json | json | 13.967 | 14.000 | 14.176 | 69.684 | 0.12x |
| mixed.json | strata | 0.173 | 0.190 | 0.208 | 69.684 | 1.00x |
| mixed.json | orjson | 0.193 | 0.207 | 0.233 | 69.684 | 0.92x |
| mixed.json | msgspec | 0.205 | 0.228 | 0.257 | 69.684 | 0.83x |
| mixed.json | ujson | 0.398 | 0.414 | 0.444 | 69.684 | 0.46x |
| mixed.json | json | 0.612 | 0.646 | 0.676 | 69.684 | 0.29x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.105 | 0.106 | 0.120 | 68.582 | 1.00x |
| users.json $[*].id | jmespath | 0.471 | 0.473 | 0.489 | 68.582 | 0.22x |
| users.json $[*].id | jsonpath-ng | 2.432 | 2.455 | 2.483 | 68.582 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.644 | 0.670 | 0.678 | 68.695 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.998 | 3.038 | 3.075 | 68.695 | 0.22x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.448 | 17.743 | 18.058 | 68.695 | 0.04x |
| users.json $..total | strata | 1.729 | 1.736 | 1.757 | 68.824 | 1.00x |
| users.json $..total | jsonpath-ng | 291.551 | 292.820 | 293.418 | 68.824 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.183 | 3.230 | 3.279 | 68.695 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.443 | 12.651 | 13.037 | 68.695 | 0.26x |
| users.json $[*].id | orjson+jsonpath-ng | 14.368 | 14.525 | 14.838 | 68.695 | 0.22x |
| users.json $[*].orders[*].total | strata | 3.331 | 3.390 | 3.430 | 68.824 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.143 | 15.244 | 15.956 | 68.824 | 0.22x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 33.870 | 33.998 | 34.400 | 68.824 | 0.10x |
| users.json $..total | strata | 11.422 | 11.702 | 12.333 | 68.824 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 310.794 | 312.619 | 314.032 | 68.824 | 0.04x |

