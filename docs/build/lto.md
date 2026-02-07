# Link-Time Optimization (LTO)

LTO is available for performance builds and is **disabled by default**.

## CMake (C++ tests)

```bash
# Release + LTO (bench preset)
cmake --preset bench
cmake --build --preset bench
ctest --preset bench --output-on-failure
```

Manual configuration:

```bash
cmake -S . -B build/cmake-bench -DCMAKE_BUILD_TYPE=Release -DSTRATA_ENABLE_LTO=ON
cmake --build build/cmake-bench
ctest --test-dir build/cmake-bench --output-on-failure
```

## Python extension build (setuptools)

```bash
STRATA_ENABLE_LTO=1 python -m pip install -e .
```

## Verify LTO

- CMake: verify `INTERPROCEDURAL_OPTIMIZATION` is enabled for targets.
- Setuptools: ensure `-flto` is present in compile/link flags.
