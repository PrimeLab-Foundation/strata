# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 75cfb422ede723193706740309d7509adf327f2b
- python: 3.12.10
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit
- machine: arm64
- processor: Apple M1 (Virtual)
- compiler_flags: [["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","arm64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/bindings/python_document.cpp","-o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/bindings/python_document.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","arm64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/bindings/python_dumps.cpp","-o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/bindings/python_dumps.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","arm64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/bindings/python_files.cpp","-o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/bindings/python_files.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","arm64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/bindings/python_folder.cpp","-o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/bindings/python_folder.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","arm64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/bindings/python_jsonpath.cpp","-o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/bindings/python_jsonpath.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","arm64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/bindings/python_loads.cpp","-o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/bindings/python_loads.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","arm64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/bindings/python_module.cpp","-o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/bindings/python_module.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","arm64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/bindings/python_ndjson.cpp","-o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/bindings/python_ndjson.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","arm64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/json/json_cursor.cpp","-o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/json/json_cursor.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","arm64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/json/json_document.cpp","-o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/json/json_document.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","arm64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/json/json_parse.cpp","-o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/json/json_parse.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","arm64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/json/json_serialize.cpp","-o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/json/json_serialize.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","arm64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/json/ndjson_stream.cpp","-o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/json/ndjson_stream.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","arm64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/search/jsonpath_compile.cpp","-o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/search/jsonpath_compile.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","arm64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/search/jsonpath_eval.cpp","-o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/search/jsonpath_eval.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","arm64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/util/dtoa.cpp","-o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/util/dtoa.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","arm64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/util/folder.cpp","-o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/util/folder.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","arm64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/util/scan.cpp","-o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/util/scan.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-bundle","-undefined","dynamic_lookup","-g","-arch","arm64","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/bindings/python_document.o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/bindings/python_dumps.o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/bindings/python_files.o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/bindings/python_folder.o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/bindings/python_jsonpath.o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/bindings/python_loads.o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/bindings/python_module.o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/bindings/python_ndjson.o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/json/json_cursor.o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/json/json_document.o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/json/json_parse.o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/json/json_serialize.o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/json/ndjson_stream.o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/search/jsonpath_compile.o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/search/jsonpath_eval.o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/util/dtoa.o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/util/folder.o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmp6hlxwzdg.build-temp/src/strata/util/scan.o","-o","/var/folders/d8/hvxvltxn0fl4rmnd52sncbth0000gn/T/tmptov0dg1t.build-lib/strata/_strata.cpython-312-darwin.so","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"]]
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.048 | 7.653 | 8.703 | 67.891 | 1.00x |
| users.json | orjson | 9.215 | 11.802 | 14.025 | 67.891 | 0.65x |
| users.json | msgspec | 8.998 | 12.187 | 15.065 | 67.891 | 0.63x |
| users.json | ujson | 12.411 | 15.826 | 23.845 | 67.891 | 0.48x |
| users.json | pysimdjson | 123.183 | 138.283 | 162.916 | 67.891 | 0.06x |
| users.json | json | 15.307 | 16.469 | 76.024 | 67.891 | 0.46x |
| flat.json | strata | 0.604 | 0.635 | 0.695 | 96.516 | 1.00x |
| flat.json | orjson | 0.782 | 0.821 | 0.887 | 96.516 | 0.77x |
| flat.json | msgspec | 0.740 | 0.771 | 0.847 | 96.516 | 0.82x |
| flat.json | ujson | 1.197 | 1.246 | 1.430 | 96.516 | 0.51x |
| flat.json | pysimdjson | 12.454 | 12.686 | 13.782 | 96.516 | 0.05x |
| flat.json | json | 1.402 | 1.452 | 1.499 | 96.516 | 0.44x |
| nested.json | strata | 0.527 | 0.534 | 0.569 | 96.531 | 1.00x |
| nested.json | orjson | 0.755 | 0.768 | 0.820 | 96.531 | 0.69x |
| nested.json | msgspec | 0.696 | 0.706 | 0.726 | 96.531 | 0.76x |
| nested.json | ujson | 1.184 | 1.195 | 1.236 | 96.531 | 0.45x |
| nested.json | pysimdjson | 10.546 | 10.587 | 10.689 | 96.531 | 0.05x |
| nested.json | json | 1.444 | 1.458 | 1.515 | 96.531 | 0.37x |
| wide_arrays.json | strata | 3.005 | 3.123 | 3.291 | 100.344 | 1.00x |
| wide_arrays.json | orjson | 3.722 | 3.855 | 4.169 | 100.344 | 0.81x |
| wide_arrays.json | msgspec | 4.153 | 4.252 | 4.398 | 100.344 | 0.73x |
| wide_arrays.json | ujson | 5.058 | 5.512 | 5.817 | 100.344 | 0.57x |
| wide_arrays.json | pysimdjson | 62.500 | 65.094 | 65.636 | 100.344 | 0.05x |
| wide_arrays.json | json | 6.819 | 7.165 | 7.328 | 100.344 | 0.44x |
| mixed.json | strata | 0.138 | 0.143 | 0.179 | 101.203 | 1.00x |
| mixed.json | orjson | 0.174 | 0.192 | 0.264 | 101.203 | 0.75x |
| mixed.json | msgspec | 0.187 | 0.201 | 0.238 | 101.203 | 0.71x |
| mixed.json | ujson | 0.242 | 0.374 | 0.549 | 101.203 | 0.38x |
| mixed.json | pysimdjson | 2.641 | 2.733 | 2.801 | 101.203 | 0.05x |
| mixed.json | json | 0.360 | 0.399 | 0.427 | 101.203 | 0.36x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.515 | 1.612 | 2.042 | 75.594 | 1.00x |
| users.json | orjson | 2.308 | 2.560 | 2.820 | 75.594 | 0.63x |
| users.json | msgspec | 3.008 | 3.199 | 3.926 | 75.594 | 0.50x |
| users.json | ujson | 9.072 | 9.318 | 11.314 | 75.594 | 0.17x |
| users.json | json | 15.530 | 16.437 | 17.849 | 75.594 | 0.10x |
| flat.json | strata | 0.238 | 0.261 | 0.267 | 96.531 | 1.00x |
| flat.json | orjson | 0.282 | 0.297 | 0.363 | 96.531 | 0.88x |
| flat.json | msgspec | 0.333 | 0.374 | 0.581 | 96.531 | 0.70x |
| flat.json | ujson | 0.795 | 0.854 | 1.069 | 96.531 | 0.31x |
| flat.json | json | 1.451 | 1.583 | 1.786 | 96.531 | 0.16x |
| nested.json | strata | 0.133 | 0.142 | 0.153 | 96.531 | 1.00x |
| nested.json | orjson | 0.216 | 0.233 | 0.249 | 96.531 | 0.61x |
| nested.json | msgspec | 0.275 | 0.293 | 0.338 | 96.531 | 0.48x |
| nested.json | ujson | 0.985 | 1.070 | 1.148 | 96.531 | 0.13x |
| nested.json | json | 1.593 | 1.660 | 1.773 | 96.531 | 0.09x |
| wide_arrays.json | strata | 1.156 | 1.212 | 1.870 | 100.344 | 1.00x |
| wide_arrays.json | orjson | 1.530 | 1.733 | 3.082 | 100.344 | 0.70x |
| wide_arrays.json | msgspec | 2.239 | 2.486 | 4.569 | 100.344 | 0.49x |
| wide_arrays.json | ujson | 5.133 | 5.342 | 7.472 | 100.344 | 0.23x |
| wide_arrays.json | json | 12.283 | 12.881 | 14.125 | 100.344 | 0.09x |
| mixed.json | strata | 0.048 | 0.052 | 0.062 | 101.203 | 1.00x |
| mixed.json | orjson | 0.052 | 0.060 | 0.074 | 101.203 | 0.87x |
| mixed.json | msgspec | 0.060 | 0.179 | 0.282 | 101.203 | 0.29x |
| mixed.json | ujson | 0.180 | 0.198 | 0.218 | 101.203 | 0.26x |
| mixed.json | json | 0.381 | 0.395 | 0.424 | 101.203 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.610 | 6.800 | 7.205 | 90.359 | 1.00x |
| users.json | orjson | 9.724 | 10.168 | 10.249 | 90.359 | 0.67x |
| users.json | msgspec | 9.281 | 9.774 | 10.056 | 90.359 | 0.70x |
| users.json | ujson | 12.763 | 13.710 | 14.192 | 90.359 | 0.50x |
| users.json | json | 15.823 | 16.073 | 16.465 | 90.359 | 0.42x |
| flat.json | strata | 0.629 | 0.690 | 0.728 | 96.531 | 1.00x |
| flat.json | orjson | 0.886 | 0.970 | 1.163 | 96.531 | 0.71x |
| flat.json | msgspec | 0.781 | 0.831 | 0.928 | 96.531 | 0.83x |
| flat.json | ujson | 1.124 | 1.256 | 1.329 | 96.531 | 0.55x |
| flat.json | json | 1.361 | 1.533 | 1.615 | 96.531 | 0.45x |
| nested.json | strata | 0.545 | 0.590 | 0.656 | 96.531 | 1.00x |
| nested.json | orjson | 0.852 | 1.030 | 1.156 | 96.531 | 0.57x |
| nested.json | msgspec | 0.726 | 0.792 | 0.821 | 96.531 | 0.74x |
| nested.json | ujson | 1.023 | 1.099 | 1.149 | 96.531 | 0.54x |
| nested.json | json | 1.422 | 1.532 | 1.609 | 96.531 | 0.38x |
| wide_arrays.json | strata | 3.178 | 3.386 | 3.603 | 101.188 | 1.00x |
| wide_arrays.json | orjson | 3.748 | 4.101 | 4.429 | 101.188 | 0.83x |
| wide_arrays.json | msgspec | 4.338 | 4.686 | 5.037 | 101.188 | 0.72x |
| wide_arrays.json | ujson | 5.766 | 6.005 | 6.162 | 101.188 | 0.56x |
| wide_arrays.json | json | 7.102 | 7.486 | 7.805 | 101.188 | 0.45x |
| mixed.json | strata | 0.202 | 0.220 | 0.259 | 101.203 | 1.00x |
| mixed.json | orjson | 0.405 | 0.457 | 0.500 | 101.203 | 0.48x |
| mixed.json | msgspec | 0.292 | 0.316 | 0.361 | 101.203 | 0.70x |
| mixed.json | ujson | 0.354 | 0.392 | 0.532 | 101.203 | 0.56x |
| mixed.json | json | 0.433 | 0.470 | 0.559 | 101.203 | 0.47x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.542 | 6.956 | 7.190 | 96.516 | 1.00x |
| users.ndjson | orjson | 11.186 | 11.988 | 12.251 | 96.516 | 0.58x |
| users.ndjson | msgspec | 11.207 | 11.888 | 12.320 | 96.516 | 0.59x |
| users.ndjson | ujson | 13.842 | 14.904 | 15.496 | 96.516 | 0.47x |
| users.ndjson | json | 18.887 | 19.189 | 21.107 | 96.516 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.984 | 2.315 | 3.495 | 90.781 | 1.00x |
| users.json | orjson | 2.867 | 3.273 | 3.768 | 90.781 | 0.71x |
| users.json | msgspec | 3.662 | 3.904 | 4.301 | 90.781 | 0.59x |
| users.json | ujson | 10.076 | 10.916 | 11.864 | 90.781 | 0.21x |
| users.json | json | 18.335 | 19.266 | 22.832 | 90.781 | 0.12x |
| flat.json | strata | 0.328 | 0.412 | 0.469 | 96.531 | 1.00x |
| flat.json | orjson | 0.374 | 0.439 | 0.533 | 96.531 | 0.94x |
| flat.json | msgspec | 0.427 | 0.500 | 0.641 | 96.531 | 0.82x |
| flat.json | ujson | 0.881 | 0.988 | 1.077 | 96.531 | 0.42x |
| flat.json | json | 1.445 | 1.639 | 1.906 | 96.531 | 0.25x |
| nested.json | strata | 0.279 | 0.321 | 0.663 | 96.531 | 1.00x |
| nested.json | orjson | 0.374 | 0.418 | 0.573 | 96.531 | 0.77x |
| nested.json | msgspec | 0.440 | 0.626 | 0.823 | 96.531 | 0.51x |
| nested.json | ujson | 1.059 | 1.132 | 1.439 | 96.531 | 0.28x |
| nested.json | json | 1.817 | 1.894 | 2.269 | 96.531 | 0.17x |
| wide_arrays.json | strata | 1.455 | 1.570 | 1.781 | 101.188 | 1.00x |
| wide_arrays.json | orjson | 1.776 | 2.088 | 2.365 | 101.188 | 0.75x |
| wide_arrays.json | msgspec | 2.598 | 2.792 | 3.308 | 101.188 | 0.56x |
| wide_arrays.json | ujson | 5.416 | 5.825 | 6.644 | 101.188 | 0.27x |
| wide_arrays.json | json | 12.470 | 13.207 | 14.233 | 101.188 | 0.12x |
| mixed.json | strata | 0.216 | 0.246 | 0.289 | 101.203 | 1.00x |
| mixed.json | orjson | 0.212 | 0.275 | 0.342 | 101.203 | 0.89x |
| mixed.json | msgspec | 0.218 | 0.356 | 0.460 | 101.203 | 0.69x |
| mixed.json | ujson | 0.373 | 0.411 | 0.452 | 101.203 | 0.60x |
| mixed.json | json | 0.535 | 0.601 | 0.634 | 101.203 | 0.41x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.070 | 0.090 | 0.135 | 90.828 | 1.00x |
| users.json $[*].id | jmespath | 0.309 | 0.391 | 0.460 | 90.828 | 0.23x |
| users.json $[*].id | jsonpath-ng | 1.670 | 1.819 | 1.906 | 90.828 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.349 | 0.498 | 0.580 | 91.000 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.952 | 2.044 | 2.290 | 91.000 | 0.24x |
| users.json $[*].orders[*].total | jsonpath-ng | 11.788 | 12.605 | 12.731 | 91.000 | 0.04x |
| users.json $..total | strata | 1.313 | 1.467 | 1.733 | 91.062 | 1.00x |
| users.json $..total | jsonpath-ng | 191.809 | 195.728 | 219.667 | 91.062 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.780 | 4.049 | 4.334 | 90.922 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.759 | 11.966 | 12.671 | 90.922 | 0.34x |
| users.json $[*].id | orjson+jsonpath-ng | 12.709 | 13.081 | 13.538 | 90.922 | 0.31x |
| users.json $[*].orders[*].total | strata | 4.047 | 4.201 | 4.469 | 91.031 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.811 | 13.601 | 14.452 | 91.031 | 0.31x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 25.591 | 27.858 | 31.294 | 91.031 | 0.15x |
| users.json $..total | strata | 8.206 | 8.551 | 9.646 | 91.109 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 203.756 | 207.456 | 208.956 | 91.109 | 0.04x |

