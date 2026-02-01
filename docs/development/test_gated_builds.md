# Test-Gated Builds

## Overview

**Per Rules 14+15**: Strata enforces **mandatory test gates** as part of the default build process. Tests are NOT optional extras—they are an integral part of building the package.

Tests run at two critical points:

1. **Pre-build**: C++ tests validate core functionality before compilation (Rule 15: DEFAULT)
1. **Post-build**: Python tests validate the Python extension after compilation (Rule 14: gate)

## Quick Start

```bash
# Standard install (DEFAULT: tests run automatically, Rules 14+15)
make install
pip install -e .

# Comprehensive gate (build + tests + coverage, Rules 14+15)
make gate

# Emergency escape hatch (STRONGLY DISCOURAGED, logged, not for CI/release)
make install-skip-tests
SKIP_TESTS=1 pip install -e .
```

**IMPORTANT**: C++ and Python tests run **by default** during `pip install`. This is mandatory behavior.

## Build Modes

### Standard Build (Default)

```bash
make install
# or
pip install -e .
```

**Build Process:**

1. Configure + run C++ tests (CMake/ctest)
1. Build Python extension
1. Run Python tests
1. Installation complete

**Failure behavior:**

- Any C++ or Python test failure aborts the build (non-zero).
- Missing CMake/pytest is treated as an error (install dependencies first).

### Comprehensive Gate (Recommended)

```bash
make gate
```

This runs:

1. C++ tests
1. Python tests
1. Coverage collection (best effort)

## Test Structure

### C++ Tests (`tests/cpp/`)

All C++ tests mirror Python functionality:

| C++ Test                   | Python Test (tests/py + tests/unit)     | Purpose            |
| -------------------------- | --------------------------------------- | ------------------ |
| `test_json_parse.cpp`      | `test_parsing_correctness.py`           | JSON parsing       |
| `test_json_cursor.cpp`     | `test_json_cursor_interface.py`         | Cursor API         |
| `test_json_document.cpp`   | (Python integration)                    | Document API       |
| `test_json_serialize.cpp`  | `test_serialize.py`                     | JSON serialization |
| `test_jsonpath.cpp`        | `test_jsonpath.py`                      | JSONPath queries   |
| `test_ndjson.cpp`          | `test_ndjson.py`                        | NDJSON streaming   |
| `test_float_precision.cpp` | `test_serialize.py::TestFloatPrecision` | Float formatting   |

### Python Tests (`tests/py/` and `tests/unit/`)

Strata keeps two Python test trees for clarity and parity:

- `tests/py/`: integration and higher-level API coverage. This includes
  benchmark-harness sanity checks (`test_bench_*`).
- `tests/unit/`: contract and edge-case coverage. These tests intentionally
  mirror core behavior so changes stay aligned with `tests/py/`.

Both trees are run by default (see `pyproject.toml`), and changes to one should
be reflected in the other when the contract is the same.

## Running Tests Manually

### C++ Tests Only

```bash
make test-cpp
```

This runs `scripts/run_cpp_tests.sh` (Rule 16: Make → scripts). Alternatively, using CMake:

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

### Python Tests Only

```bash
make test-py
# or
pytest tests/py tests/unit -v

# contract/unit only
pytest tests/unit -v

# integration + bench sanity only
pytest tests/py -v
```

### All Tests

```bash
make test
```

This runs:

1. C++ tests (via ctest)
1. Python tests (via pytest)

## CI/CD Integration

### Active GitHub Actions Workflows

- `/.github/workflows/ci.yml`: multi-platform test matrix (Linux/macOS/Windows). Uses the test-gated build (`pip install -e .[dev]`) and runs `pytest` explicitly.
- `/.github/workflows/benchmark.yml`: weekly scheduled benchmarks on Ubuntu. Runs `bench_main` and uploads `docs/benchmarks/bench_results.md`; fails on ERROR rows.

### GitHub Actions Example

```yaml
name: Test-Gated Build

on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      
      - name: Install dependencies
        run: |
          sudo apt-get install cmake
          pip install pytest
      
      - name: Test-gated install
        run: make install-safe
      
      - name: Verify installation
        run: python -c "import strata; print(strata.dumps({'test': 'ok'}))"
```

### Pre-commit Hook

```bash
#!/bin/bash
# .git/hooks/pre-commit

echo "Running test-gated build..."
make install-safe

if [ $? -ne 0 ]; then
    echo "❌ Tests failed - commit rejected"
    exit 1
fi

echo "✅ All tests passed - proceeding with commit"
```

## Requirements

### For Test-Gated Builds

- **CMake** (3.20+): For C++ test compilation
- **pytest**: For Python test execution
- **C++20 compiler**: clang++ or g++

### Checking Requirements

```bash
cmake --version      # Should be 3.20+
pytest --version     # Any recent version
clang++ --version    # or g++ --version
```

## Troubleshooting

### "CMake not found" Error

```bash
# macOS
brew install cmake

# Ubuntu/Debian
sudo apt-get install cmake

# Windows
# Download from https://cmake.org/download/
```

### "pytest not found" Error

```bash
pip install pytest
```

### C++ Tests Fail

1. Check C++ compiler is installed
1. Verify code compiles: `make test-cpp`
1. Check test output for specific failures
1. Fix code and retry

### Python Tests Fail

1. Verify Python extension built: `python -c "import strata"`
1. Run tests with verbose output: `pytest tests/unit/ -vv`
1. Check for import errors or runtime issues
1. Fix code and retry

### Build Fails in CI/CD

Common causes:

- Missing CMake in CI environment
- Missing pytest in CI environment
- Test data not available
- Platform-specific compilation issues

**Solution:** Ensure all build dependencies are installed in CI environment.

## Test Parity Guidelines

When adding new features, ensure:

1. **C++ tests cover core logic**

   - Add tests in `tests/cpp/test_*.cpp`
   - Follow existing test patterns
   - Use CMakeLists.txt to register tests

1. **Python tests cover integration and contract**

   - Add tests in `tests/py/test_*.py` for integration behavior
   - Mirror contract/edge cases in `tests/unit/test_*.py`
   - Verify Python API surface and error handling

1. **Both test suites should validate**:

   - Correctness (output matches expected)
   - Edge cases (empty, null, large inputs)
   - Error conditions (invalid input)
   - Performance (reasonable time/memory)

## Benefits of Test-Gated Builds

✅ **Prevent broken builds**: Catch errors before installation
✅ **Faster debugging**: Know immediately when tests fail\
✅ **Test parity**: Ensure C++ and Python tests stay synchronized\
✅ **Production safety**: Only deploy code that passes all tests\
✅ **Developer confidence**: Know builds are reliable

## Best Practices

1. **Run `make install-safe` before committing** major changes
1. **Add tests for new features** in both C++ and Python
1. **Keep tests fast** (\< 30s total for quick iteration)
1. **Fix failing tests immediately** (don't skip or disable)
1. **Use standard install** (`make install`) for rapid development
1. **Use test-gated install** (`make install-safe`) before releases

## Environment Variables

| Variable           | Values           | Purpose                          |
| ------------------ | ---------------- | -------------------------------- |
| `STRATA_TEST_GATE` | `true`/`false`   | Enable test gates in pip install |
| `PGO_MODE`         | `generate`/`use` | Profile-guided optimization      |

## Example Workflow

```bash
# 1. Make code changes
vim src/strata/json/json_parse.cpp

# 2. Quick test (fast)
make test

# 3. If tests pass, install for testing
make install
python -c "import strata; ..."

# 4. Before committing, verify with test-gated build
make install-safe

# 5. Commit if all tests pass
git add .
git commit -m "Improve JSON parsing"
```

______________________________________________________________________

*For more information, see*:

- `docs/benchmarks/methodology.md` - Benchmark testing guidelines
- `Makefile` - All available build commands
- `CMakeLists.txt` - C++ test configuration
- `setup.py` - Python build configuration
