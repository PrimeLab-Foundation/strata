# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 75cfb422ede723193706740309d7509adf327f2b
- python: 3.12.10
- implementation: CPython
- platform: macOS-15.7.9-x86_64-i386-64bit
- machine: x86_64
- processor: Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz
- compiler_flags: [["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","x86_64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/bindings/python_document.cpp","-o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/bindings/python_document.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","x86_64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/bindings/python_dumps.cpp","-o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/bindings/python_dumps.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","x86_64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/bindings/python_files.cpp","-o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/bindings/python_files.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","x86_64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/bindings/python_folder.cpp","-o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/bindings/python_folder.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","x86_64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/bindings/python_jsonpath.cpp","-o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/bindings/python_jsonpath.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","x86_64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/bindings/python_loads.cpp","-o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/bindings/python_loads.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","x86_64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/bindings/python_module.cpp","-o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/bindings/python_module.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","x86_64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/bindings/python_ndjson.cpp","-o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/bindings/python_ndjson.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","x86_64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/json/json_cursor.cpp","-o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/json/json_cursor.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","x86_64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/json/json_document.cpp","-o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/json/json_document.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","x86_64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/json/json_parse.cpp","-o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/json/json_parse.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","x86_64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/json/json_serialize.cpp","-o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/json/json_serialize.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","x86_64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/json/ndjson_stream.cpp","-o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/json/ndjson_stream.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","x86_64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/search/jsonpath_compile.cpp","-o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/search/jsonpath_compile.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","x86_64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/search/jsonpath_eval.cpp","-o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/search/jsonpath_eval.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","x86_64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/util/dtoa.cpp","-o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/util/dtoa.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","x86_64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/util/folder.cpp","-o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/util/folder.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-arch","x86_64","-Iinclude","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-I/Users/runner/work/strata/strata/.venv/include","-I/Library/Frameworks/Python.framework/Versions/3.12/include/python3.12","-c","src/strata/util/scan.cpp","-o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/util/scan.o","-std=c++20","-O3","-D_LIBCPP_DISABLE_AVAILABILITY","-fomit-frame-pointer","-march=native","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"],["c++","-fno-strict-overflow","-Wsign-compare","-Wunreachable-code","-fno-common","-dynamic","-DNDEBUG","-g","-O3","-Wall","-g","-bundle","-undefined","dynamic_lookup","-g","-arch","x86_64","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/bindings/python_document.o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/bindings/python_dumps.o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/bindings/python_files.o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/bindings/python_folder.o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/bindings/python_jsonpath.o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/bindings/python_loads.o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/bindings/python_module.o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/bindings/python_ndjson.o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/json/json_cursor.o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/json/json_document.o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/json/json_parse.o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/json/json_serialize.o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/json/ndjson_stream.o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/search/jsonpath_compile.o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/search/jsonpath_eval.o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/util/dtoa.o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/util/folder.o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmph7rv1z_j.build-temp/src/strata/util/scan.o","-o","/var/folders/20/jp1_0n3n7kndh6rnbqb5344m0000gn/T/tmpzx5vs8_o.build-lib/strata/_strata.cpython-312-darwin.so","-flto=thin","-fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata"]]
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 18.497 | 18.804 | 23.574 | 57.023 | 1.00x |
| users.json | orjson | 26.354 | 27.953 | 30.944 | 57.023 | 0.67x |
| users.json | msgspec | 26.075 | 27.639 | 30.686 | 57.023 | 0.68x |
| users.json | ujson | 38.835 | 40.852 | 44.180 | 57.023 | 0.46x |
| users.json | pysimdjson | 172.211 | 175.668 | 179.984 | 57.023 | 0.11x |
| users.json | json | 44.660 | 45.855 | 48.135 | 57.023 | 0.41x |
| flat.json | strata | 1.220 | 1.239 | 1.300 | 67.930 | 1.00x |
| flat.json | orjson | 1.362 | 1.388 | 1.494 | 67.930 | 0.89x |
| flat.json | msgspec | 1.562 | 1.574 | 1.638 | 67.930 | 0.79x |
| flat.json | ujson | 2.760 | 2.784 | 3.107 | 67.930 | 0.44x |
| flat.json | pysimdjson | 14.726 | 14.821 | 15.489 | 67.930 | 0.08x |
| flat.json | json | 3.188 | 3.208 | 3.578 | 67.930 | 0.39x |
| nested.json | strata | 1.440 | 1.477 | 1.621 | 66.406 | 1.00x |
| nested.json | orjson | 1.663 | 1.698 | 1.798 | 66.406 | 0.87x |
| nested.json | msgspec | 1.792 | 1.847 | 2.021 | 66.406 | 0.80x |
| nested.json | ujson | 3.033 | 3.092 | 3.337 | 66.406 | 0.48x |
| nested.json | pysimdjson | 13.532 | 14.053 | 14.824 | 66.406 | 0.11x |
| nested.json | json | 3.878 | 4.078 | 4.224 | 66.406 | 0.36x |
| wide_arrays.json | strata | 7.481 | 7.589 | 8.299 | 72.375 | 1.00x |
| wide_arrays.json | orjson | 9.242 | 9.510 | 9.770 | 72.375 | 0.80x |
| wide_arrays.json | msgspec | 10.155 | 10.556 | 10.931 | 72.375 | 0.72x |
| wide_arrays.json | ujson | 12.989 | 13.138 | 13.873 | 72.375 | 0.58x |
| wide_arrays.json | pysimdjson | 79.743 | 80.117 | 86.754 | 72.375 | 0.09x |
| wide_arrays.json | json | 17.054 | 17.168 | 18.319 | 72.375 | 0.44x |
| mixed.json | strata | 0.358 | 0.366 | 0.381 | 65.168 | 1.00x |
| mixed.json | orjson | 0.444 | 0.464 | 0.497 | 65.168 | 0.79x |
| mixed.json | msgspec | 0.464 | 0.489 | 0.511 | 65.168 | 0.75x |
| mixed.json | ujson | 0.646 | 0.666 | 0.684 | 65.168 | 0.55x |
| mixed.json | pysimdjson | 3.339 | 3.393 | 3.501 | 65.168 | 0.11x |
| mixed.json | json | 0.914 | 0.949 | 0.981 | 65.168 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.534 | 2.633 | 2.859 | 52.352 | 1.00x |
| users.json | orjson | 3.330 | 3.418 | 3.914 | 52.352 | 0.77x |
| users.json | msgspec | 5.127 | 5.309 | 5.817 | 52.352 | 0.50x |
| users.json | ujson | 24.992 | 25.716 | 27.204 | 52.352 | 0.10x |
| users.json | json | 42.186 | 43.103 | 44.355 | 52.352 | 0.06x |
| flat.json | strata | 0.309 | 0.319 | 0.332 | 66.445 | 1.00x |
| flat.json | orjson | 0.386 | 0.397 | 0.407 | 66.445 | 0.80x |
| flat.json | msgspec | 0.510 | 0.532 | 0.562 | 66.445 | 0.60x |
| flat.json | ujson | 2.208 | 2.215 | 2.231 | 66.445 | 0.14x |
| flat.json | json | 3.628 | 3.675 | 3.975 | 66.445 | 0.09x |
| nested.json | strata | 0.248 | 0.256 | 0.259 | 66.539 | 1.00x |
| nested.json | orjson | 0.346 | 0.352 | 0.382 | 66.539 | 0.73x |
| nested.json | msgspec | 0.541 | 0.565 | 0.623 | 66.539 | 0.45x |
| nested.json | ujson | 2.325 | 2.337 | 2.359 | 66.539 | 0.11x |
| nested.json | json | 4.635 | 4.663 | 4.698 | 66.539 | 0.06x |
| wide_arrays.json | strata | 1.816 | 1.975 | 2.170 | 66.102 | 1.00x |
| wide_arrays.json | orjson | 2.320 | 2.483 | 2.843 | 66.102 | 0.80x |
| wide_arrays.json | msgspec | 3.408 | 3.549 | 3.861 | 66.102 | 0.56x |
| wide_arrays.json | ujson | 10.558 | 11.233 | 11.945 | 66.102 | 0.18x |
| wide_arrays.json | json | 34.597 | 35.188 | 36.611 | 66.102 | 0.06x |
| mixed.json | strata | 0.062 | 0.067 | 0.072 | 61.969 | 1.00x |
| mixed.json | orjson | 0.071 | 0.081 | 0.092 | 61.969 | 0.83x |
| mixed.json | msgspec | 0.105 | 0.113 | 0.117 | 61.969 | 0.60x |
| mixed.json | ujson | 0.466 | 0.468 | 0.473 | 61.969 | 0.14x |
| mixed.json | json | 0.981 | 0.989 | 1.099 | 61.969 | 0.07x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 18.259 | 18.527 | 19.589 | 63.164 | 1.00x |
| users.json | orjson | 25.673 | 26.978 | 29.068 | 63.164 | 0.69x |
| users.json | msgspec | 26.078 | 27.015 | 29.583 | 63.164 | 0.69x |
| users.json | ujson | 38.521 | 40.059 | 42.578 | 63.164 | 0.46x |
| users.json | json | 44.176 | 45.021 | 46.477 | 63.164 | 0.41x |
| flat.json | strata | 1.332 | 1.381 | 1.466 | 66.445 | 1.00x |
| flat.json | orjson | 1.507 | 1.567 | 1.685 | 66.445 | 0.88x |
| flat.json | msgspec | 1.723 | 1.800 | 1.907 | 66.445 | 0.77x |
| flat.json | ujson | 2.915 | 3.062 | 3.150 | 66.445 | 0.45x |
| flat.json | json | 3.315 | 3.370 | 3.575 | 66.445 | 0.41x |
| nested.json | strata | 1.551 | 1.620 | 1.761 | 66.539 | 1.00x |
| nested.json | orjson | 1.808 | 1.868 | 1.949 | 66.539 | 0.87x |
| nested.json | msgspec | 2.009 | 2.103 | 2.172 | 66.539 | 0.77x |
| nested.json | ujson | 3.215 | 3.331 | 3.424 | 66.539 | 0.49x |
| nested.json | json | 4.030 | 4.194 | 4.424 | 66.539 | 0.39x |
| wide_arrays.json | strata | 7.513 | 7.818 | 8.293 | 67.332 | 1.00x |
| wide_arrays.json | orjson | 9.492 | 9.989 | 10.766 | 67.332 | 0.78x |
| wide_arrays.json | msgspec | 10.574 | 10.965 | 11.580 | 67.332 | 0.71x |
| wide_arrays.json | ujson | 13.389 | 13.598 | 14.580 | 67.332 | 0.57x |
| wide_arrays.json | json | 17.526 | 17.778 | 19.069 | 67.332 | 0.44x |
| mixed.json | strata | 0.414 | 0.440 | 0.474 | 61.969 | 1.00x |
| mixed.json | orjson | 0.529 | 0.568 | 0.605 | 61.969 | 0.78x |
| mixed.json | msgspec | 0.581 | 0.600 | 0.683 | 61.969 | 0.73x |
| mixed.json | ujson | 0.791 | 0.815 | 0.851 | 61.969 | 0.54x |
| mixed.json | json | 1.000 | 1.024 | 1.062 | 61.969 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 18.398 | 18.920 | 19.210 | 67.508 | 1.00x |
| users.ndjson | orjson | 26.702 | 27.453 | 28.173 | 67.508 | 0.69x |
| users.ndjson | msgspec | 27.359 | 27.787 | 28.417 | 67.508 | 0.68x |
| users.ndjson | ujson | 39.140 | 40.097 | 40.481 | 67.508 | 0.47x |
| users.ndjson | json | 49.657 | 51.138 | 52.217 | 67.508 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.387 | 3.560 | 4.261 | 63.180 | 1.00x |
| users.json | orjson | 4.266 | 4.517 | 4.760 | 63.180 | 0.79x |
| users.json | msgspec | 6.187 | 6.311 | 6.776 | 63.180 | 0.56x |
| users.json | ujson | 26.540 | 27.597 | 29.563 | 63.180 | 0.13x |
| users.json | json | 44.161 | 45.673 | 95.309 | 63.180 | 0.08x |
| flat.json | strata | 0.662 | 0.723 | 0.890 | 66.445 | 1.00x |
| flat.json | orjson | 0.743 | 0.824 | 0.924 | 66.445 | 0.88x |
| flat.json | msgspec | 0.938 | 0.974 | 1.046 | 66.445 | 0.74x |
| flat.json | ujson | 2.657 | 2.731 | 3.080 | 66.445 | 0.26x |
| flat.json | json | 4.144 | 4.265 | 5.077 | 66.445 | 0.17x |
| nested.json | strata | 0.613 | 0.637 | 0.674 | 66.539 | 1.00x |
| nested.json | orjson | 0.744 | 0.777 | 0.886 | 66.539 | 0.82x |
| nested.json | msgspec | 0.950 | 0.972 | 1.011 | 66.539 | 0.66x |
| nested.json | ujson | 2.762 | 2.873 | 2.996 | 66.539 | 0.22x |
| nested.json | json | 5.096 | 5.347 | 5.606 | 66.539 | 0.12x |
| wide_arrays.json | strata | 2.509 | 2.732 | 2.937 | 66.102 | 1.00x |
| wide_arrays.json | orjson | 3.196 | 3.358 | 3.464 | 66.102 | 0.81x |
| wide_arrays.json | msgspec | 4.426 | 4.553 | 4.857 | 66.102 | 0.60x |
| wide_arrays.json | ujson | 12.425 | 12.526 | 12.754 | 66.102 | 0.22x |
| wide_arrays.json | json | 36.447 | 37.239 | 38.240 | 66.102 | 0.07x |
| mixed.json | strata | 0.313 | 0.352 | 0.426 | 61.969 | 1.00x |
| mixed.json | orjson | 0.362 | 0.405 | 0.479 | 61.969 | 0.87x |
| mixed.json | msgspec | 0.393 | 0.425 | 0.481 | 61.969 | 0.83x |
| mixed.json | ujson | 0.737 | 0.798 | 0.844 | 61.969 | 0.44x |
| mixed.json | json | 1.245 | 1.337 | 1.378 | 61.969 | 0.26x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.135 | 0.177 | 0.212 | 63.242 | 1.00x |
| users.json $[*].id | jmespath | 0.996 | 1.066 | 1.350 | 63.242 | 0.17x |
| users.json $[*].id | jsonpath-ng | 5.472 | 5.743 | 5.861 | 63.242 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.927 | 1.099 | 1.194 | 60.488 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 6.246 | 6.649 | 7.153 | 60.488 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 36.601 | 38.843 | 39.600 | 60.488 | 0.03x |
| users.json $..total | strata | 3.214 | 3.600 | 4.062 | 60.543 | 1.00x |
| users.json $..total | jsonpath-ng | 710.774 | 758.297 | 987.309 | 60.543 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.094 | 4.111 | 5.017 | 63.320 | 1.00x |
| users.json $[*].id | orjson+jmespath | 27.236 | 27.782 | 29.952 | 63.320 | 0.15x |
| users.json $[*].id | orjson+jsonpath-ng | 32.353 | 32.713 | 34.224 | 63.320 | 0.13x |
| users.json $[*].orders[*].total | strata | 4.428 | 4.441 | 4.531 | 60.543 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 31.504 | 32.247 | 33.772 | 60.543 | 0.14x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 66.585 | 67.239 | 69.261 | 60.543 | 0.07x |
| users.json $..total | strata | 21.759 | 22.956 | 25.152 | 60.543 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 740.338 | 778.048 | 1092.150 | 60.543 | 0.03x |

